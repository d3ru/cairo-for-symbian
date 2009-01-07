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
 * Contributors:
 * 		Iwan Junianto <iwanj@users.sourceforge.net>
 */

#ifndef CAIRO_SYMBIAN_SURFACE_PRIVATE_H
#define CAIRO_SYMBIAN_SURFACE_PRIVATE_H

#include "cairo-symbian.h"
#include "cairo-surface-private.h"

#include <e32base.h>
#include <gdi.h>

class TCairoSymbianUtil
	{
public:
	static cairo_content_t ToCairoContent(TDisplayMode);
	static cairo_format_t ToCairoFormat(TDisplayMode);
	static TDisplayMode ToDisplayMode(cairo_format_t);
	static TDisplayMode ToDisplayMode(cairo_content_t, int);
	};

class RWsSession;
class RWindow;
class CWindowGc;
class CWsScreenDevice;
class CWsBitmap;

/**
 * Note that we do not derive from CBase in this case to make sure cairo_surface_t member variable is
 * at offset 0 as expected by Cairo generic layer.
 * Cairo will deallocate surface using free() so we must use malloc and placement new to instantiate
 * this class
 * 
 */
class XCairoSymbianSurface
	{
public:
	static XCairoSymbianSurface* Create(symbian_window_t);
	~XCairoSymbianSurface();

	int Width() const {return iSize.iWidth;}
	int Height() const {return iSize.iHeight;}
	int Depth() const {return iDepth;}
	cairo_rectangle_int_t Extents() const;

	void Resize(const TSize&);	
	cairo_image_surface_t* Lock();
	void Unlock();
	void Flush();

private:
	XCairoSymbianSurface(RWindow*);
	TInt Construct();
	
private:
	cairo_surface_t iBase;

	TSize iSize;
	int iDepth;
	RWsSession* iWs;
	RWindow* iWin;
	CWindowGc* iGc;
	CWsScreenDevice* iScr;
	CWsBitmap* iBuffer;
	};

#endif /* CAIRO_SYMBIAN_SURFACE_PRIVATE_H */
