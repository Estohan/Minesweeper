/**
	IOLinux.h
		Ncurse library based Input/Output interface.
	Implements 'IOInterface.h'.

	@author Sergiu Constantinescu
*/
#ifndef _IOLINUX_H_
#define _IOLINUX_H_

#include <string>
#include <map>
#include "GameSettings.h"
#include "Utils.h"
#include "IOInterface.h"

// used to group the main parameters of a window
struct win_params {
	int start_x;
	int start_y;
	int height;
	int width;
};

class IOLinux : public IOInterface {
private:
	GameSettings* settings;
	// scene windows and their parameters
	WINDOW* header;
	win_params header_params;
	WINDOW* screen;
	win_params screen_params;
	WINDOW* bottom;
	win_params bottom_params;
	// a mapping of used to identify colors by strings
	std::map<std::string, int> attr_types;
	// used to clear rows
	const char* k_print_clear;
	const char* k_input_clear;
	const int k_header_height;
	const int k_menu_height;
	const int k_menu_width;
	const int k_bottom_height;
	const int k_options_pos_x;
	const int k_prompt;
	const char k_cursor;

public:
	IOLinux(GameSettings* settings);

	char read_char();
	std::string read_string();
	int read_int();
	void print_clear();
	void io_update_settings(GameSettings* settings);
	void print_menu(int menu_level);
	void print_str(std::string message);
	void println_str(std::string message);
	void print_header();
	void print_diff_constraints(std::string name, int min, int max, bool err);
	void print_board(char visible_field[][MAT_WIDTH], int c_x, int c_y, int marked, double percent);
	void print_win_message();
	void print_lose_message();
	void print_revealed_board(char visible_field[][MAT_WIDTH], bool won);
	void init_IO(bool menu_type_scr);
	void close_IO();
	// clear bottom window's input space
	void clear_bottom_input();
	void init_colors();
	void set_tile_color(char ch, bool attr_on, int print_type);
	void set_color(WINDOW* win, std::string type, bool attr_on);
	// debugging function, no use while running the game
	void _debug(WINDOW* win, const char* msg, int x, int y);

	WINDOW* create_win(int height, int width, int start_y, int start_x);
	void destroy_win(WINDOW* win/*, win_params win_p*/);
	void print_stats(int marked, int bombs, double percent);
	// calculates the number of digits a number has
	int get_nr_of_digits(int n);
};

#endif // _IOLINUX_H_
