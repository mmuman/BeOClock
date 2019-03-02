BeOClock, round clock for BeOS
(c) 2002, mmu_man, revol@free.fr
Released under MIT licence, 
usual disclaimer applies, blah blah blah...


This application is an elliptic clock for BeOS, just like oclock does for UNIX users, but much better of course :P
It use a new features from the last BeOS version (non-rectangular windows).


Installation:
- put the AutoRaise folder where you want (I suggest /boot/apps),
- you can delete the src/ folder if you aren't interested in the sources,
- double-click on BeOClock.

You can add it to your ~/config/boot/USerBootscript so it will be run each time you boot.

Customisation:
* left click to move the window (you can also use 'Show Border' before moving so it's easier)
* right click brings a menu:
	* Show Border: check it to activate the window border (this allows you to move and resize the clock).
	* Always On Top: check this to have it always overlap other windows (as a side effect the clock is also on every workspace).
	* Hollow: check this to remove the background of the clock, it now only has the round border and hands. (you'll have to hit a tiny target to get the menu... Hint: hit 'm' with the mouse on the clock)
	* About: usual info box.
	* Quit: to exit BeOClock.

* Use your favourite color picker to drag colors on it, it will bring a menu so you can tell it which part you want the color to be used for.
I suggest Colors! as a color picker if you don't have one already. Get it at BeBits:
http://www.bebits.com/app/2429

Informations:
This application needs special API that isn't available in BEOS R5, so it will only work in BEOS R5.1 or YellowTab Zeta.

The code was based on a sample code called "spots" by Alan Ellis (I think).

Enjoy,
François.
