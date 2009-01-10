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

#include "myappview.h"
#include "cairo_samples.h"

#include <cairo.h>
#include <cairo-symbian.h>

const TInt KOneSecond = 1000*1000;

CMyAppView* CMyAppView::NewL( const TRect& aRect )
    {
    CMyAppView* self = CMyAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

CMyAppView* CMyAppView::NewLC( const TRect& aRect )
    {
    CMyAppView* self = new ( ELeave ) CMyAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

void CMyAppView::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    SetRect( aRect );
    ActivateL();

    iSurface = cairo_symbian_surface_create(&Window());
    User::LeaveIfNull(iSurface);
    
    iContext = cairo_create(iSurface);
    User::LeaveIfNull(iContext);
    cairo_status_t err = cairo_status(iContext);
    if (err == CAIRO_STATUS_NO_MEMORY)
    	{
    	User::Leave(KErrNoMemory);
    	}

    iNumberOfSamples = number_of_cairo_samples();
    iTimer = CPeriodic::NewL(CActive::EPriorityLow);
    }

CMyAppView::CMyAppView()
    {
    }

CMyAppView::~CMyAppView()
    {
    delete iTimer;
    
	__ASSERT_DEBUG(cairo_get_reference_count(iContext) == 1, User::Invariant());
    cairo_destroy(iContext);
    cairo_surface_destroy(iSurface);
    cairo_debug_reset_static_data();
    }

void CMyAppView::Draw( const TRect& aRect) const
    {
    /* do not mix native and Cairo rendering to the same window, it is not supported */
    if (!iStarted)
    	{
    	SystemGc().Clear();
    	return;
    	}

    cairo_t* cr = Context();
    cairo_save(cr);

    /* clear surface to opaque white */
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);

    /* draw Cairo samples */
	draw_function draw = cairo_samples(iIdx);
	draw(cr);

	cairo_restore(cr);

    iIdx = (iIdx+1) % iNumberOfSamples;
    }

void CMyAppView::SizeChanged()
    {
    }

TInt CMyAppView::TimerCallBack(TAny* aArg)
	{
	CMyAppView* av = (CMyAppView*) aArg;
	av->DrawNow();	
	
	return KErrNone;
	}

void CMyAppView::Start()
	{
	iStarted = ETrue;
    iTimer->Start(KOneSecond, KOneSecond, TCallBack(CMyAppView::TimerCallBack, this));
	}

void CMyAppView::Stop()
	{
	iTimer->Cancel();
	iStarted = EFalse;
	DrawNow();
	}
