/**
	MainMenu.h
		Game's menu functionalities.

	@author Sergiu Constantinescu
*/
#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "GameSettings.h"


template <class IO>
class MainMenu {
private:
	IO* io_mode;

public:
	MainMenu(IO* io_mod);
	
	int menu_loop(GameSettings *settings);
	void choose_difficulty(GameSettings *settings);
};

#include "MainMenu.hpp"

#endif // _MAINMENU_H_
