/**
	main.cpp
		Contains the flow of the entire game, putting together
	the menu and the actual game.

	@author Sergiu Constantinescu
*/
#include <iostream>
#include <string>
#include <ncurses.h>
#include "GameState.h"
#include "GameSettings.h"
#include "MainMenu.h"
#include "IOText.h"

#ifdef __WIN32
	// wip for version 1.1
	#include "IOWindows.h"
	#define GRAPHICS IOWindows
#elif __linux__
	#include "IOLinux.h"
	#define GRAPHICS IOLinux
#endif


void usage() {
	std::cout << "Usage: './Minesweeper [GRAPHICS MODE]'" << std::endl;
	std::cout << "[GRAPHICS MODE] :" << std::endl;
	std::cout << "\t1 - Text mode" << std::endl;
	std::cout << "\t2 - Fancy graphics (default)" << std::endl;	
}

int main(int argc, char* argv[]) {
 
	// true as long as the exit option was not selectected
	bool game_running = true;
	// false if game is started in Text mode
	bool io_mode_color = true;

	GameSettings *settings = new GameSettings();

	if(argc == 2) {
		if(std::string(argv[1]) == "1") {
			io_mode_color = false;
		} else if (std::string(argv[1]) != "2") {
			usage();
			return 0;
		}
	}

	IOText* io_text = new IOText(settings);
	GRAPHICS* io_color = new GRAPHICS(settings);

	// object containing the game logic
	GameState<IOText> *game_state_t = new GameState<IOText>(io_text);
	GameState<GRAPHICS> *game_state_c = new GameState<GRAPHICS>(io_color);
	// object containing the main menu options and functions
	MainMenu<IOText> *main_menu_t = new MainMenu<IOText>(io_text);
	MainMenu<GRAPHICS> *main_menu_c = new MainMenu<GRAPHICS>(io_color);

	while(game_running) {
		int input;

		if(io_mode_color) {
			input = main_menu_c->menu_loop(settings);
		} else {
			input = main_menu_t->menu_loop(settings);
		}

		if(input == 0) { // new game
			if(io_mode_color) {
				game_state_c->game_loop(settings);
			} else {
				game_state_t->game_loop(settings);
			}
		}

		if(input == 1) { // exit game
			game_running = false;
		}
	}

	delete game_state_t;
	delete game_state_c;
	delete main_menu_t;
	delete main_menu_c;
	delete io_text;
	delete io_color;
	delete settings;
	endwin();

	return 0;
}
