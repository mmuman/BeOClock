#ifndef OCLOCKWINDOW_H
#define OCLOCKWINDOW_H

#include <Window.h>
#include "OClockView.h"

class OClockWindow : public BWindow
{
	public:
		OClockWindow();
		virtual bool QuitRequested();
		virtual void Show();
		virtual void MessageReceived(BMessage* Message);
		virtual void FrameResized(float new_width, float new_height);
		
		void LoadSettings();
		void SaveSettings();

		rgb_color		fBgColor, fBorderColor;
		rgb_color		fHoursHandColor, fMinsHandColor, fSecsHandColor;
		bool			fTopMost;
		bool			fShowBorder;
		bool			fIsHollow;
		BPicture*			fPic;

	private:
		OClockView*			fView;
};

#define SETTINGS_FILE "x-vnd.mmu.BeOClock_settings"

#endif

