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

/**
 * cairo_symbian_surface_create:
 * @window: an RWindow
 *
 * Creates a Symbian surface that draws to the given window.
 *
 * Note:
 * The function cairo_symbian_surface_set_size() must be called whenever the size of the
 * window changes.
 *
 * Return value: the newly created surface
 **/
cairo_surface_t *
cairo_symbian_surface_create (symbian_window_t window) {
 
	if (window == NULL)
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NULL_POINTER));
    
    XCairoSymbianSurface *surface = NULL;
    surface = XCairoSymbianSurface::Create(window);
    if (surface == NULL)
    return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NO_MEMORY));
    
    return (cairo_surface_t *)surface;
}

void
cairo_symbian_surface_set_size (cairo_surface_t *surface,
								int width,
								int height)
{
	__ASSERT_DEBUG(surface != NULL && width > 0 && height > 0, User::Invariant());
	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *)surface;

	sym_surface->Resize(TSize(width, height));
}

int
cairo_symbian_surface_get_depth (cairo_surface_t *surface) {

	__ASSERT_DEBUG(surface != NULL, User::Invariant());
	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *)surface;
	
	return sym_surface->Depth();
}

int
cairo_symbian_surface_get_width (cairo_surface_t *surface) {

	__ASSERT_DEBUG(surface != NULL, User::Invariant());
	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *)surface;
	
	return sym_surface->Width();
}

int
cairo_symbian_surface_get_height (cairo_surface_t *surface) {

	__ASSERT_DEBUG(surface != NULL, User::Invariant());
	XCairoSymbianSurface *sym_surface = (XCairoSymbianSurface *)surface;
	
	return sym_surface->Height();
}
