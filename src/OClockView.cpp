#include <stdlib.h>

#include <GraphicsDefs.h>
#include <Picture.h>
#include <Rect.h>
#include <View.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "OClockView.h"
#include "OClockWindow.h"
#include "PopupMenu.h"

#define BORDER 5

OClockView::OClockView(OClockWindow *window, BRect frame)
           :BView(frame, "oclockview", B_FOLLOW_ALL, B_WILL_DRAW | B_PULSE_NEEDED)
{
	fWindow = window;   
	fIsMoving = 0;
	fDiscardNextMouseMoved = 0;      
//	SetViewColor(255, 255, 255);
	SetViewColor(B_TRANSPARENT_32_BIT);
//	MakeFocus();
//	SetEventMask(B_KEYBOARD_EVENTS);
}

OClockView::~OClockView()
{
}

void 
OClockView::Pulse(void)
{
/*
#ifndef B_BEOS_VERSION_DANO
	// hack to fake a bit of transparency
	if (Window()) {
		Window()->Hide();
		Window()->Show();
	}
#else
*/
	Invalidate(Bounds());
//#endif
}

void 
OClockView::Draw(BRect /*updateRect*/)
{
	time_t today;
	struct tm *theTime;
	BRect bnds;
	
//printf("{%f, %f, %f, %f}\n", bnds.top, bnds.left, bnds.bottom, bnds.right);

	time(&today);
	theTime = localtime((const time_t *)&today);

	if (fWindow->fIsHollow)
		BeginPicture(fWindow->fPic);

	/*
	bnds.top += BORDER/2;
	bnds.left += BORDER/2;
	bnds.bottom -= BORDER/2;
	bnds.right -= BORDER/2;
	*/
	//SetHighColor(255, 255, 255);

	if (!fWindow->fIsHollow) {
		bnds = Bounds();
		bnds.InsetBy(BORDER-1, BORDER-1);
		SetHighColor(fWindow->fBgColor);
		FillEllipse(bnds);
	}
	
	bnds = Bounds();
	bnds.InsetBy(BORDER/2, BORDER/2);
	SetHighColor(fWindow->fBorderColor);
	SetPenSize(BORDER);
	StrokeEllipse(bnds);
	
	
//	printf("%d %d %d %d %d %d \n", theTime->tm_year, theTime->tm_mon, theTime->tm_mday, theTime->tm_hour, theTime->tm_min, theTime->tm_sec);

	// Draw hours (offset by the minutes, so it looks more like a real mecanical clock)
	DrawHand((2 * M_PI * ((((float)(theTime->tm_hour%12)) + (float)theTime->tm_min/60) / 12)), 0.5, 5, fWindow->fHoursHandColor);
	// Draw minutes
	DrawHand(2 * M_PI * theTime->tm_min / 60, 0.75, 3, fWindow->fMinsHandColor);
	// Draw seconds
	if (true) {// settings->DrawSeconds
		DrawHand(2 * M_PI * theTime->tm_sec / 60, 0.85, 1, fWindow->fSecsHandColor);
	}

	if (fWindow->fIsHollow) {
		fWindow->fPic = EndPicture();

#ifdef B_BEOS_VERSION_DANO
		const BPoint offset(0, 0);
		fWindow->ClipWindowToPicture(fWindow->fPic, offset, 0);
#endif

		bnds = Bounds();
		bnds.InsetBy(BORDER/2, BORDER/2);
		SetHighColor(fWindow->fBorderColor);
		SetPenSize(BORDER);
		StrokeEllipse(bnds);
		
		DrawHand((2 * M_PI * ((((float)(theTime->tm_hour%12)) + (float)theTime->tm_min/60) / 12)), 0.5, 5, fWindow->fHoursHandColor);
		// Draw minutes
		DrawHand(2 * M_PI * theTime->tm_min / 60, 0.75, 3, fWindow->fMinsHandColor);
		// Draw seconds
		if (true) {// settings->DrawSeconds
			DrawHand(2 * M_PI * theTime->tm_sec / 60, 0.85, 1, fWindow->fSecsHandColor);
		}

	}
}

void OClockView::MouseDown(BPoint where)
{
	int32 buttons;
	BMessage *message = Window()->CurrentMessage();
	
	message->FindInt32("buttons", &buttons);
	
	if (buttons > 1) {
		ConvertToScreen(&where);
		//menu will delete itself (see constructor of ConfigMenu),
		//so all we're concerned about is calling Go() asynchronously
		//ConfigMenu *menu = new ConfigMenu(this, _useMag);
		OClockPopup *p = new OClockPopup(fWindow);
		where.x-=5;
		where.y-=5;
		p->Go(where, true, true, BRect(0,0,-1,-1)/*ConvertToScreen(Bounds())*/, true);
	} else {
		if (!fIsMoving)
			resume_thread(spawn_thread(MoverTask, "Window Mover", B_NORMAL_PRIORITY, (void *)this));
	}
}

int32 OClockView::MoverTask(void *pthis)
{
	BPoint cursor, moveOrigin;
	uint32 buttons, org_button;
	bigtime_t startTime;
	OClockView *_this = (OClockView *)pthis;
	
	if (atomic_add(&_this->fIsMoving, 1) > 0) {
		atomic_add(&_this->fIsMoving, -1);
		return B_OK;
	}
	startTime = system_time();
	_this->LockLooper();
	_this->GetMouse(&cursor, &buttons);
	org_button = buttons;
	moveOrigin = cursor;
	_this->ConvertToScreen(&moveOrigin);
	_this->UnlockLooper();
	while (buttons && buttons == org_button) {
		snooze(30000);
		_this->LockLooper();
		_this->ConvertToScreen(&cursor);
		if (cursor != moveOrigin) {
			_this->Window()->MoveBy(cursor.x - moveOrigin.x, cursor.y - moveOrigin.y);
			moveOrigin = cursor;
		}
		_this->GetMouse(&cursor, &buttons);
		_this->UnlockLooper();
	}
	if (system_time() - startTime < 200000) // assume we want to raise the window
		_this->Window()->Activate(true);
	atomic_add(&_this->fIsMoving, -1);
	return B_OK;
}

void OClockView::KeyDown(const char *bytes, int32 numBytes)
{
	// doesn't seem to work...
	if (numBytes != 1 || (bytes[0] != 'm' && bytes[0] != 'M'))
		return;
	BPoint where;
	where.x = (Bounds().right - Bounds().left) / 2;
	where.y = (Bounds().bottom - Bounds().top) / 2;
	
	ConvertToScreen(&where);
	OClockPopup *p = new OClockPopup(fWindow);
	p->Go(where, true, true, BRect(0,0,-1,-1)/*ConvertToScreen(Bounds())*/, true);
}

// angle is in degree
void 
OClockView::DrawHand(float angle, float length, int width, rgb_color color)
{
	BRect bnds(Bounds());
	BPoint center((bnds.right-bnds.left)/2, (bnds.bottom-bnds.top)/2);
	BPoint endpoint(center);
	
	if (angle < 0) angle = 0;
	if (angle > 360) angle = 360;
	
	endpoint.x += length * center.x * sinf(angle);
	endpoint.y += length * center.y * (-1) * cosf(angle);
	
	PushState();
	SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
	SetPenSize((float)width);
	SetHighColor(color);
	StrokeLine(center, endpoint);
	PopState();
}
