#ifndef OCLOCKVIEW_H
#define OCLOCKVIEW_H

#include <View.h>

class OClockWindow;

class OClockView : public BView
{
	public:
		OClockView(OClockWindow *window, BRect frame);
		~OClockView();
		virtual void Pulse(void);
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint where);
		virtual void KeyDown(const char *bytes, int32 numBytes);
		void DrawHand(float angle, float length, int width, rgb_color color);
		static int32 MoverTask(void *pthis);
		
	private:
		OClockWindow*		fWindow;
		vint32				fIsMoving;
		int				fDiscardNextMouseMoved;
};

#endif

