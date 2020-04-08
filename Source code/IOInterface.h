/**
	IOInterface.h
		Ncurse library based Input/Output interface.
	Implements 'IOInterface.h'.

	@author Sergiu Constantinescu
*/
#ifndef __IOINTERFACE_H_
#define __IOINTERFACE_H_

#include <string>
#include "GameSettings.h"
#include "Utils.h"

class IOInterface {
public:
	virtual char read_char() = 0;
	virtual std::string read_string() = 0;
	virtual int read_int() = 0;
	// added for future cross platform compatibility
	virtual void print_clear() = 0;
	virtual void io_update_settings(GameSettings* settings) = 0;
	virtual void print_menu(int menu_level) = 0;
	virtual void print_str(std::string message) = 0;
	virtual void println_str(std::string message) = 0;
	virtual void print_header() = 0;
	virtual void print_diff_constraints(std::string name, int min, int max, bool err) = 0;
	virtual void print_board(char visible_field[][MAT_WIDTH], int c_x, int c_y, int marked, double percent) = 0;
	virtual void print_win_message() = 0;
	virtual void print_lose_message() = 0;
	virtual void init_IO(bool menu_type_scr) = 0;
	virtual void close_IO() = 0;
	virtual ~IOInterface() { };
};

#endif // __IOINTERFACE_H_
