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

#include <avkon.hrh>
#include "myappui.h"
#include "myappview.h"
#include "myappcmd.h"

void CMyAppUi::ConstructL()
    {
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    iAppView = CMyAppView::NewL(ClientRect());
    AddToStackL(iAppView);
    iAppView->ActivateL();
    }

CMyAppUi::CMyAppUi()
    {
    }

CMyAppUi::~CMyAppUi()
    {
    if (iAppView)
        {
        RemoveFromStack(iAppView);
        delete iAppView;
        }
    }

void CMyAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EUserCmdStart:
        	iAppView->Start();
        	break;
        case EUserCmdStop:
        	iAppView->Stop();
        	break;
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;
        }
    }

void CMyAppUi::HandleStatusPaneSizeChange()
{
    iAppView->SetRect(ClientRect());
}

TKeyResponse CMyAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
	{
	TKeyResponse response = EKeyWasConsumed;
	switch (aKeyEvent.iCode)
		{
	case EKeyEscape:
		Exit();
		break;
	default:
		response = EKeyWasNotConsumed;
		};

	return response;
	}

