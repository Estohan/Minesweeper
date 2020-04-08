/**
	MainMenu.hpp
		Game's menu functionalities implementations.

	@author Sergiu Constantinescu
*/
#ifndef __MAINMENU_HPP_
#define __MAINMENU_HPP_

#include <iostream>
#include "GameSettings.h"
#include "Utils.h"


template <class IO>
MainMenu<IO>::MainMenu(IO* io_mod){
	io_mode = io_mod;
};

// Game's menu loop. It's structure looks like this:
// New game
// Options
//		Rules
//			Rules text
//			Back
//		Difficulty
//			Novice
//			Adept
//			Master
//			Master (fullscreen)
//			Custom
//			Back
//		Back
// Exit
template <class IO>
int MainMenu<IO>::menu_loop(GameSettings *settings) {

	int input;
	int menu_level = 0;
	int exit_code = -1;
	io_mode->init_IO(true);

	while(exit_code < 0) {
		io_mode->io_update_settings(settings);
		io_mode->print_menu(menu_level);
		input = io_mode->read_char();
		switch(menu_level) {
			case 0:
				if(input == '1') {
					exit_code = 0; // new game
				} else if (input == '2') {
					menu_level = 1; // options
				} else if (input == '3') {
					exit_code = 1; // exit game
				}
				break;
			case 1:
				if(input == '1') {
					menu_level = 2; // rules
				} else if (input == '2') {
					menu_level = 3; // difficulty
				} else if (input == '3') {
					menu_level = 0; // back
				}
				break;
			case 2:
				if(input == '1') {
					menu_level = 1; // back
				}
				break;
			case 3:
				if(input == '1') { // novice
					settings->set_diff(1);
				} else if(input == '2') { // adept
					settings->set_diff(2);
				} else if(input == '3') { // master
					settings->set_diff(3);
				} else if(input == '4') { // master (fulscreen)
					settings->set_diff(4);
				} else if(input == '5') { // custom
					choose_difficulty(settings);
					settings->set_diff(0);
				} else if(input == '6') { // back
					menu_level = 1;
				}
				break;
			default:
				menu_level = 0;
		}
	}

	io_mode->close_IO();
	return exit_code;
}

// ensures that the player introduces valid data to describe
// their desired custom configuration of the board
template <class IO>
void MainMenu<IO>::choose_difficulty(GameSettings *settings) {
	int height = 0;
	int width = 0;
	int bombs = 0;

	int min_height = MIN_HEIGHT;
	int max_height = MAX_HEIGHT;
	int min_width = MIN_WIDTH;
	int max_width = MAX_WIDTH;

	io_mode->print_diff_constraints("height", min_height, max_height, false);
	while(true) {
		height = io_mode->read_int();
		if(height < min_height || height > max_height) {
			io_mode->print_diff_constraints("height", 
											min_height, 
											max_height, 
											true);
		} else {
			break;
		}
	}

	io_mode->print_diff_constraints("width", min_width, max_width, false);
	while(true) {
		width = io_mode->read_int();
		if(width < min_width || width > max_width) {
			io_mode->print_diff_constraints("width", 
											min_width, 
											max_width, 
											true);
		} else {
			break;
		}
	}

	// 75% is the maximum amount of bomb tiles
	int max_possible_bombs = (height * width) * 0.75;

	io_mode->print_diff_constraints("number of bombs", 
									MIN_BOMBS,
									max_possible_bombs,
									false);
	while(true) {
		bombs = io_mode->read_int();
		if(bombs < MIN_BOMBS || bombs > max_possible_bombs) {
			io_mode->print_diff_constraints("number of bombs", 
											MIN_BOMBS,
											max_possible_bombs,
											true);
		} else {
			break;
		}
	}

	settings->set_custom_diff(height, width, bombs);
}

#endif // __MAINMENU_HPP_
