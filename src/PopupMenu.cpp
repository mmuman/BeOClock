#include "PopupMenu.h"
#include "OClockWindow.h"
#include "Lang.h"

OClockPopup::OClockPopup(OClockWindow *p) : BPopUpMenu("popup menu", false, false)
{
	pwin = p;
	BMenuItem *tmpi;

	SetFont(be_plain_font);

	tmpi = new BMenuItem(_T("Show border"), new BMessage('shoB')/*, 'Q'*/);
	tmpi->SetTarget(NULL, pwin);
	tmpi->SetMarked((pwin->Look() & B_NO_BORDER_WINDOW_LOOK) == 0);
	AddItem(tmpi);
	
	tmpi = new BMenuItem(_T("Always on top"), new BMessage('topM'));
	tmpi->SetTarget(NULL, pwin);
	tmpi->SetMarked((pwin->Feel() & B_FLOATING_ALL_WINDOW_FEEL) != 0);
	AddItem(tmpi);

	tmpi = new BMenuItem(_T("Hollow"), new BMessage('holO'));
	tmpi->SetTarget(NULL, pwin);
	tmpi->SetMarked(pwin->fIsHollow);
	AddItem(tmpi);

	AddSeparatorItem();


	BString tmpstring;
	tmpstring << _T("About") << B_UTF8_ELLIPSIS;
	tmpi = new BMenuItem(tmpstring.String(), new BMessage('_abR')/*, 'A'*/);
//	tmpi = new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED)/*, 'A'*/);
	tmpi->SetTarget(NULL, pwin);
//	tmpi->SetTarget(NULL, be_app);
	AddItem(tmpi);

	tmpi = new BMenuItem(_T("Quit"), new BMessage(B_QUIT_REQUESTED)/*, 'Q'*/);
	tmpi->SetTarget(NULL, be_app);
	AddItem(tmpi);

	SetTargetForItems(pwin);

	SetAsyncAutoDestruct(true);
	

}

OClockPopup::~OClockPopup()
{
}

