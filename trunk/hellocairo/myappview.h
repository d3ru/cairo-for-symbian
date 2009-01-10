/*
 * Copyright © 2009 iwanj@users.sourceforge.net
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef __MYAPPVIEW_H__
#define __MYAPPVIEW_H__

#include <e32base.h>
#include <coecntrl.h>
#include <cairo.h>

class CMyAppView : public CCoeControl
    {
   	friend class CMyAppUi;

public:
	static CMyAppView* NewL(const TRect& aRect);
	static CMyAppView* NewLC(const TRect& aRect);
	virtual ~CMyAppView();

	void Draw(const TRect& aRect) const;
	virtual void SizeChanged();

	static TInt TimerCallBack(TAny* aArg);
	void Start();
	void Stop();

private:
    void ConstructL(const TRect& aRect);
    CMyAppView();
    cairo_t* Context() const {return iContext;}

private:
	CPeriodic* iTimer;
	mutable TInt iIdx;
	TInt iNumberOfSamples;
	TBool iStarted;
	
	cairo_surface_t* iSurface;
	cairo_t* iContext;
    };

#endif

