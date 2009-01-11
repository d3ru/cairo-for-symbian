/* -*- Mode: c; tab-width: 8; c-basic-offset: 4; indent-tabs-mode: t; -*- */
/* Cairo - a vector graphics library with display and print output
 *
 * Copyright © 2009 Iwan Junianto
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Iwan Junianto.
 *
 * Contributor(s):
 *      Iwan Junianto <iwanj@users.sourceforge.net>
 */

#include "cairoint.h"
#include "cairo-symbian-surface-private.h"

#include <e32std.h>
#include <gdi.h>
#include <w32std.h>
#include <coemain.h>

static cairo_status_t 
_cairo_symbian_surface_finish(void *surface) {

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) surface;
	sym_surface->~XCairoSymbianSurface();
	
	return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t
_cairo_symbian_surface_acquire_source_image (void *abstract_surface,
			 						cairo_image_surface_t **image_out,
			 						void **image_extra) {

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) abstract_surface;
	*image_out = sym_surface->Lock();
	*image_extra = NULL;

	return CAIRO_STATUS_SUCCESS;	
}

static void
_cairo_symbian_surface_release_source_image (void *abstract_surface,
			 							cairo_image_surface_t *image,
				 							void *image_extra) {

	cairo_surface_destroy((cairo_surface_t*) image);

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) abstract_surface;
	sym_surface->Unlock();
}

static cairo_status_t
_cairo_symbian_surface_acquire_dest_image (void *abstract_surface,
			 						cairo_rectangle_int_t *interest_rect,
			 						cairo_image_surface_t **image_out,
			 						cairo_rectangle_int_t *image_rect,
			 						void **image_extra) {

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) abstract_surface;
	*image_out = sym_surface->Lock();
	*image_rect = sym_surface->Extents();
	*image_extra = NULL;

	return CAIRO_STATUS_SUCCESS;	
}

static void
_cairo_symbian_surface_release_dest_image (void *abstract_surface,
			 							cairo_rectangle_int_t *interest_rect,
			 							cairo_image_surface_t *image,
			 							cairo_rectangle_int_t *image_rect,
			 							void *image_extra) {

	cairo_surface_destroy((cairo_surface_t*) image);

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) abstract_surface;
	sym_surface->Unlock();

	sym_surface->Flush();
}

static cairo_int_status_t
_cairo_symbian_surface_get_extents (void *surface,
			 						cairo_rectangle_int_t *extents) {

	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *) surface;
	*extents = sym_surface->Extents();

	return (cairo_int_status_t)CAIRO_STATUS_SUCCESS;
}

static const cairo_surface_backend_t cairo_symbian_surface_backend = {
	CAIRO_SURFACE_TYPE_SYMBIAN,
	NULL, //create_similar
	_cairo_symbian_surface_finish,
	_cairo_symbian_surface_acquire_source_image,
	_cairo_symbian_surface_release_source_image,
	_cairo_symbian_surface_acquire_dest_image,
	_cairo_symbian_surface_release_dest_image,
	NULL, //clone_similar
	NULL, //composite
	NULL, //fill_rectangles
	NULL, //composite_trapezoids
	NULL, //copy_page
	NULL, //show_page
	NULL, //set_clip_region
	NULL, //intersect_clip_path
	_cairo_symbian_surface_get_extents,
	NULL, //old_show_glyphs
	NULL, //get_font_options
	NULL, //flush
	NULL, //mark_dirty_rectangle
	NULL, //scaled_font_fini
	NULL, //scaled_glyph_fini
	NULL, //paint
	NULL, //mask
	NULL, //stroke
	NULL, //fill
	NULL, //show_glyphs
	NULL, //snapshot
	NULL, //is_similar
	NULL, //reset
	NULL, //fill_stroke
	NULL  //create_solid_pattern_surface
};

/**
 * TCairoSymbianUtil implementation
 * 
 */
cairo_content_t TCairoSymbianUtil::ToCairoContent(TDisplayMode aMode) 
	{
	switch (aMode) 
		{
		case EColor16MA:
			return CAIRO_CONTENT_COLOR_ALPHA;
		case EColor16MU:
			return CAIRO_CONTENT_COLOR;
		case EGray2:
		case EGray256:
			return CAIRO_CONTENT_ALPHA;
		}
	
	__ASSERT_ALWAYS(EFalse, User::Invariant());
	return CAIRO_CONTENT_COLOR_ALPHA; // to get rid of warning
	}

cairo_format_t TCairoSymbianUtil::ToCairoFormat(TDisplayMode aMode) 
	{
	switch (aMode) 
		{
		case EColor16MA:
			return CAIRO_FORMAT_ARGB32;
		case EColor16MU:
			return CAIRO_FORMAT_RGB24;
		case EGray256:
			return CAIRO_FORMAT_A8;
		case EGray2:
			return CAIRO_FORMAT_A1;
		}

	__ASSERT_ALWAYS(EFalse, User::Invariant());
	return CAIRO_FORMAT_ARGB32; // to get rid of warning
	}

/**
 * XCairoSymbianSurface implementation
 * 
 */
XCairoSymbianSurface* XCairoSymbianSurface::Create(symbian_window_t aWindow)
	{
	__ASSERT_DEBUG(aWindow, User::Invariant());

	void* buf = malloc(sizeof(XCairoSymbianSurface));
	if (!buf)
		{
		return NULL;
		}

	XCairoSymbianSurface *self = new(buf) XCairoSymbianSurface(reinterpret_cast<RWindow*>(aWindow));
	TInt err = self->Construct();
	if (err != KErrNone)
		{
		/* if Construct() fails, all internal allocations must have been freed before that func returns */
		free(buf);
		self = NULL;
		}
	
	return self;
	}

XCairoSymbianSurface::XCairoSymbianSurface(RWindow *aWindow):
	iWin(aWindow)
	{
	}

XCairoSymbianSurface::~XCairoSymbianSurface()
	{
	__ASSERT_DEBUG(iCache, User::Invariant());
	delete iCache;
	}

TInt XCairoSymbianSurface::Construct() 
	{
	iSize = iWin->Size();
	__ASSERT_DEBUG(iSize != TSize(0,0), User::Invariant());

	const TDisplayMode dm = iWin->DisplayMode();
	__ASSERT_DEBUG(dm == EColor16MU || dm == EColor16MA, User::Invariant());

	iDepth = TDisplayModeUtils::NumDisplayModeBitsPerPixel(dm);

	iWs = &CCoeEnv::Static()->WsSession();
	iScr = CCoeEnv::Static()->ScreenDevice();
	iGc = &CCoeEnv::Static()->SystemGc();
	
	iCache = new CWsBitmap(*iWs);
	if (!iCache)
		{
		return KErrNoMemory;
		}

	TInt err = iCache->Create(iSize, dm);
	if (err != KErrNone)
		{
		delete iCache;
		iCache = NULL;
		return err;
		}

	iCache->LockHeap();
	Mem::FillZ(iCache->DataAddress(), iCache->DataStride() * iSize.iHeight);
	iCache->UnlockHeap();

	_cairo_surface_init (&iBase, &cairo_symbian_surface_backend, TCairoSymbianUtil::ToCairoContent(dm));
	return KErrNone;
	}

void XCairoSymbianSurface::SetSize(TInt aWidth, TInt aHeight) 
	{
	TSize newSize(aWidth, aHeight);
	if (iSize == newSize)
		{
		return;
		}

	TInt err = iCache->Resize(newSize);
	if (err != KErrNone)
		{
		_cairo_error(CAIRO_STATUS_NO_MEMORY);
		return;
		}

	iSize = newSize;
	}

cairo_rectangle_int_t XCairoSymbianSurface::Extents() const
	{
	cairo_rectangle_int_t r;
	r.x = r.y = 0;
	r.width = iSize.iWidth;
	r.height = iSize.iHeight;
	
	return r;
	}

cairo_image_surface_t* XCairoSymbianSurface::Lock()
	{
	iCache->LockHeap();
	cairo_surface_t* s = cairo_image_surface_create_for_data((unsigned char*)iCache->DataAddress(),
																	TCairoSymbianUtil::ToCairoFormat(iCache->DisplayMode()),
																	iSize.iWidth,
																	iSize.iHeight,
																	iCache->DataStride());

	return (cairo_image_surface_t*)s;
	}

void XCairoSymbianSurface::Unlock()
	{
	iCache->UnlockHeap();
	}

void XCairoSymbianSurface::Flush()
	{
	/* must be called when GC is active and within RWindow::BeginRedraw/EndRedraw block */
	iGc->BitBlt(TPoint(), iCache);
	iWs->Flush();
	}

