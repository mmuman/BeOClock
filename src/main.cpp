#include <stdlib.h>

#include <Application.h>
#include "Lang.h"
#include "OClockWindow.h"

int main()
{
	BApplication app("application/x-vnd.mmu.BeOClock");

	// Load language from system application lanuage directory
	be_locale.LoadLanguageFile("/boot/beos/etc/Language/Dictionaries/SysApps/BeOClock");


	OClockWindow *w = new OClockWindow();
	w->Show();
	w->SetPulseRate(1000000);
	app.Run();
		
	return 0;
}
