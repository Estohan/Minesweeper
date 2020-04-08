/**
	IOText.h
		Simple text character based input/output interface
	for the game. Inherits IOInterface to ensure compatibility
	with the game.

	@author Sergiu Constantinescu
*/
#ifndef _IOTEXT_H_
#define _IOTEXT_H_

#include <string>
#include "GameSettings.h"
#include "Utils.h"
#include "IOInterface.h"

class IOText : public IOInterface {
private:
	GameSettings* settings;
	char visible_field[MAT_HEIGHT][MAT_WIDTH];

public:
	IOText(GameSettings* settings);

	char read_char();
	std::string read_string();
	int read_int();
	// clears the screen
	void print_clear();
	void io_update_settings(GameSettings* settings);
	void print_menu(int menu_level);
	void print_str(std::string message);
	void println_str(std::string message);
	void print_header();
	// when choosing custom values for games difficulty
	void print_diff_constraints(std::string name, int min, int max, bool err);
	void print_board(char visible_field[][MAT_WIDTH], int c_x, int c_y, int marked, double percent);
	void print_win_message();
	void print_lose_message();
	void print_revealed_board(char visible_field[][MAT_WIDTH], bool won);
	void init_IO(bool menu_type_scr);
	void close_IO();
};

#endif // _IOTEXT_H_
