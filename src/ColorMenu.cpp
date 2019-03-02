#include "ColorMenu.h"
#include "OClockWindow.h"
#include "Lang.h"

OClockColorPopup::OClockColorPopup(OClockWindow *p, rgb_color c) : BPopUpMenu("color popup menu", false, false)
{
	pwin = p;
	BMenuItem *tmpi;
	BMessage *msg;
//	fMenuBar = new BMenuBar(BRect(0,0,10000,20), "menubar");

	SetFont(be_plain_font);

	msg = new BMessage('CoBg');
	msg->AddInt32("color", *(int32 *)&c);
	tmpi = new BMenuItem(_T("Background"), msg);
	tmpi->SetTarget(NULL, pwin);
	AddItem(tmpi);
	
	msg = new BMessage('CoBo');
	msg->AddInt32("color", *(int32 *)&c);
	tmpi = new BMenuItem(_T("Border"), msg);
	tmpi->SetTarget(NULL, pwin);
	AddItem(tmpi);
	
	msg = new BMessage('CoHo');
	msg->AddInt32("color", *(int32 *)&c);
	tmpi = new BMenuItem(_T("Hours"), msg);
	tmpi->SetTarget(NULL, pwin);
	AddItem(tmpi);
	
	msg = new BMessage('CoMi');
	msg->AddInt32("color", *(int32 *)&c);
	tmpi = new BMenuItem(_T("Minutes"), msg);
	tmpi->SetTarget(NULL, pwin);
	AddItem(tmpi);
	
	msg = new BMessage('CoSe');
	msg->AddInt32("color", *(int32 *)&c);
	tmpi = new BMenuItem(_T("Seconds"), msg);
	tmpi->SetTarget(NULL, pwin);
	AddItem(tmpi);
	
	SetTargetForItems(pwin);

	SetAsyncAutoDestruct(true);
	

}

OClockColorPopup::~OClockColorPopup()
{
}

