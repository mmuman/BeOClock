#ifndef POPUP_H
#define POPUP_H

#include <Application.h>
#include <PopUpMenu.h>
#include <MenuBar.h>
#include <MenuItem.h>

class OClockWindow;

class OClockPopup : public BPopUpMenu
{
public:
	OClockPopup(OClockWindow *p);
	virtual ~OClockPopup();
private:
	OClockWindow *pwin;
};




#endif //POPUP_H
