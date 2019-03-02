#ifndef COLOR_POPUP_H
#define COLOR_POPUP_H

#include <Application.h>
#include <GraphicsDefs.h>
#include <PopUpMenu.h>
#include <MenuBar.h>
#include <MenuItem.h>

class OClockWindow;

class OClockColorPopup : public BPopUpMenu
{
public:
	OClockColorPopup(OClockWindow *p, rgb_color c);
	virtual ~OClockColorPopup();
private:
	OClockWindow *pwin;
};




#endif //POPUP_H
