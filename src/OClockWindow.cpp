#include <stdlib.h>

#include <Alert.h>
#include <Picture.h>
#include <Rect.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <Screen.h>
#include <StorageKit.h>
#include <View.h>

#include "Lang.h"
#include "PopupMenu.h"
#include "ColorMenu.h"
#include "OClockWindow.h"

static BRect spotsize(0, 0, 30, 30);
static const BRect defaultclockframe(100, 100, 200, 200);
static const BPoint offset(0, 0);
BRect screen;

OClockWindow::OClockWindow()
           :BWindow(defaultclockframe
                    , "BeOClock"
                    , B_NO_BORDER_WINDOW_LOOK
                    , B_NORMAL_WINDOW_FEEL
                    , B_QUIT_ON_WINDOW_CLOSE)
            
{
	LoadSettings();
	if (fTopMost)
		SetFeel((window_feel)(Feel() | B_FLOATING_ALL_WINDOW_FEEL));
	if (fShowBorder)
		SetLook((window_look)(Look() & ~B_NO_BORDER_WINDOW_LOOK));
	fView = new OClockView(this, Bounds());
	AddChild(fView);
	
	fView->BeginPicture(new BPicture);
	// start out with nothing, otherwise we always get one dot at (100,100)
	fView->FillEllipse(Bounds());
	fPic = fView->EndPicture();
	
#ifdef B_BEOS_VERSION_DANO
	if (!fIsHollow) {
		ClipWindowToPicture(fPic, offset, 0);
	}
#endif
	fView->Invalidate(fView->Bounds());
}


bool OClockWindow::QuitRequested()
{
	delete fPic;
	SaveSettings();
	return true;
}

void 
OClockWindow::MessageReceived(BMessage *Message)
{
	rgb_color col;

	// from Tracker: tracker/DeskWindow.cpp
	if (Message->WasDropped()) {
		const rgb_color *color;
		int32 size;
		BPoint where, off;
		
		// handle "roColour"-style color drops
		if (Message->FindData("RGBColor", 'RGBC', (const void **)&color, &size) == B_OK) {
			Message->FindPoint("_drop_point_", &where);
			Message->FindPoint("_drop_offset_", &off);
			//where += off;
			//menu will delete itself (see constructor of ConfigMenu),
			//so all we're concerned about is calling Go() asynchronously
			//ConfigMenu *menu = new ConfigMenu(this, _useMag);
			OClockColorPopup *p = new OClockColorPopup(this, *color);
			where.x-=5;
			where.y-=5;
			p->Go(where, true, true, BRect(0,0,-1,-1)/*ConvertToScreen(Bounds())*/, true);
			return;
		}
	}
	
	switch( Message->what )
	{
		case 'topM' :
			SetFeel((window_feel)(Feel() | (Feel() & B_FLOATING_ALL_WINDOW_FEEL)?(0):(B_FLOATING_ALL_WINDOW_FEEL)));
			fTopMost = Feel() & B_FLOATING_ALL_WINDOW_FEEL;
			break;
		case 'shoB' :
			SetLook((window_look)(Look() | (Look() & B_NO_BORDER_WINDOW_LOOK)?(0):(B_NO_BORDER_WINDOW_LOOK)));
			fShowBorder = !(Look() & B_NO_BORDER_WINDOW_LOOK);
			break;
		case 'holO' :
			fIsHollow = !fIsHollow;
			fView->Invalidate(fView->Bounds());
			if (!fIsHollow) {	
				fView->BeginPicture(fPic);
				// start out with nothing, otherwise we always get one dot at (100,100)
				fView->FillEllipse(Bounds());
				fPic = fView->EndPicture();
#ifdef B_BEOS_VERSION_DANO
				ClipWindowToPicture(fPic, offset, 0);
#endif
			}
			break;
		case '_abR' :
		{
			BAlert *alert = new BAlert("title", 
				_T("BeOClock, (c) 2002, mmu_man\n\nDrag colors on it !\nTips:\n\n- use right button to drag a background color,\n- use 'Show border' to move and resize it.\n\nEnjoy ! :-)"),
				_T("Ok"), NULL, NULL,
				B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_INFO_ALERT);
			alert->SetShortcut(0, B_ENTER);
			alert->Go();
			break;
		}
		case '_KYD' :
		{
			BPoint where;
			where.x = (Bounds().right - Bounds().left) / 2;
			where.y = (Bounds().bottom - Bounds().top) / 2;
	
			ConvertToScreen(&where);
			OClockPopup *p = new OClockPopup(this);
			p->Go(where, true, true, BRect(0,0,-1,-1)/*ConvertToScreen(Bounds())*/, true);
			break;
		}
		case 'CoBg' :
			Message->FindInt32("color", (int32 *)&col);
			fBgColor = col;
			break;
		case 'CoBo' :
			Message->FindInt32("color", (int32 *)&col);
			fBorderColor = col;
			break;
		case 'CoHo' :
			Message->FindInt32("color", (int32 *)&col);
			fHoursHandColor = col;
			break;
		case 'CoMi' :
			Message->FindInt32("color", (int32 *)&col);
			fMinsHandColor = col;
			break;
		case 'CoSe' :
			Message->FindInt32("color", (int32 *)&col);
			fSecsHandColor = col;
			break;
		default:
			BWindow::MessageReceived(Message);
	}
}

void OClockWindow::FrameResized(float, float)
{
	if (fIsHollow)
		return;
	fView->BeginPicture(fPic);
	// start out with nothing, otherwise we always get one dot at (100,100)
	fView->FillEllipse(Bounds());
	fPic = fView->EndPicture();
#ifdef B_BEOS_VERSION_DANO
	ClipWindowToPicture(fPic, offset, 0);
#endif
}

void 
OClockWindow::Show()
{
	BWindow::Show();
	//SetPulseRate(1000000);
	// moved to main.cpp
}

void 
OClockWindow::LoadSettings()
{
	BFile settingsFile;
	BPath prefPath;
	BMessage settingsMessage;
	BRect frame;
	

	fHoursHandColor.red = 189;
	fHoursHandColor.green = 208;
	fHoursHandColor.blue = 213;
	fHoursHandColor.alpha = 255;
	fBorderColor = tint_color(fHoursHandColor, 0.5);
	fMinsHandColor = tint_color(fHoursHandColor, 0.75);
	fSecsHandColor = tint_color(fHoursHandColor, 0.80);
	fBgColor.red = 255;
	fBgColor.green = 255;
	fBgColor.blue = 255;
	fBgColor.alpha = 255;
	
	fTopMost = false;
	fShowBorder = false;
	fIsHollow = false;
	
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefPath);
	prefPath.Append(SETTINGS_FILE);
		
	settingsFile.SetTo(prefPath.Path(), B_READ_ONLY);
	if (settingsMessage.Unflatten(&settingsFile) != B_OK)
		return;
	settingsMessage.FindInt32("hourshandcolor", (int32 *)&fHoursHandColor);
	settingsMessage.FindInt32("minshandcolor", (int32 *)&fMinsHandColor);
	settingsMessage.FindInt32("secshandcolor", (int32 *)&fSecsHandColor);
	settingsMessage.FindInt32("bordercolor", (int32 *)&fBorderColor);
	settingsMessage.FindInt32("bgcolor", (int32 *)&fBgColor);
	settingsMessage.FindRect("frame", &frame);
	settingsMessage.FindBool("topmost", &fTopMost);
	settingsMessage.FindBool("showborder", &fShowBorder);
	settingsMessage.FindBool("ishollow", &fIsHollow);
	MoveTo(frame.left, frame.top);
	ResizeTo(frame.right - frame.left, frame.bottom - frame.top);
	settingsFile.Unset();
}

void 
OClockWindow::SaveSettings()
{
	BFile settingsFile;
	BPath prefPath;
	BMessage settingsMessage('sett');
	
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefPath);
	prefPath.Append(SETTINGS_FILE);
	settingsFile.SetTo(prefPath.Path(), B_READ_WRITE | B_ERASE_FILE | B_CREATE_FILE);

	settingsMessage.AddInt32("hourshandcolor", *(int32 *)(&fHoursHandColor));
	settingsMessage.AddInt32("minshandcolor", *(int32 *)(&fMinsHandColor));
	settingsMessage.AddInt32("secshandcolor", *(int32 *)(&fSecsHandColor));
	settingsMessage.AddInt32("bordercolor", *(int32 *)(&fBorderColor));
	settingsMessage.AddInt32("bgcolor", *(int32 *)(&fBgColor));
	settingsMessage.AddRect("frame", Frame());
	settingsMessage.AddBool("topmost", fTopMost);
	settingsMessage.AddBool("showborder", fShowBorder);
	settingsMessage.AddBool("ishollow", fIsHollow);
	settingsMessage.Flatten(&settingsFile);
	settingsFile.Unset();
}

