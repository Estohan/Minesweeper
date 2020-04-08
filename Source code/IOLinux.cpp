/**
	IOLinux.cpp
		Ncurse library based Input/Output interface.
	Implements 'IOInterface.h'.

	@author Sergiu Constantinescu
*/
#include <ncurses.h>
#include <string>
#include <sstream>
#include "IOLinux.h"
#include "GameSettings.h"
#include "Utils.h"

IOLinux::IOLinux(GameSettings* settings):
	settings(settings),
	k_print_clear(
	"                                                                    "),
	k_input_clear(
	">                                                                   "),
	k_header_height(3),
	k_menu_height(16),
	k_menu_width(70),
	k_bottom_height(4),
	k_options_pos_x(28),
	k_prompt('>'),
	k_cursor('+')
	{}

WINDOW* IOLinux::create_win(int hei, int wid, int start_x, int start_y) {
	WINDOW *win;

	win = newwin(hei, wid, start_y, start_x);
	box(win, 0 , 0);
	wrefresh(win);

	return win;
}

void IOLinux::destroy_win(WINDOW* win) {
	werase(win);
	wrefresh(win);
	delwin(win);
}

void IOLinux::init_IO(bool menu_type_scr) {
	initscr();
	curs_set(0);
	init_colors();
	refresh();

	header_params.start_y = 0;
	header_params.start_x = 0;
	header_params.height = k_header_height;
	header_params.width = k_menu_width;
	if(menu_type_scr) { // to create a menu window
		screen_params.start_y = header_params.height;
		screen_params.start_x = 0;
		screen_params.height = k_menu_height;
		screen_params.width = k_menu_width;

		bottom_params.start_y = k_header_height + k_menu_height;
		bottom_params.start_x = 0;
		bottom_params.height = k_bottom_height;
		bottom_params.width = k_menu_width;
	} else { // to create a game window
		screen_params.start_y = header_params.height;
		screen_params.start_x = (MAT_WIDTH - settings->get_width() - 3)/2;
		screen_params.height = settings->get_height() + 2;
		screen_params.width = settings->get_width() + 2;

		bottom_params.start_y = k_header_height + settings->get_height() + 2;
		bottom_params.start_x = 0;
		bottom_params.height = k_bottom_height;
		bottom_params.width = k_menu_width;
	}

	header = create_win(header_params.height,
						header_params.width,
						header_params.start_x,
						header_params.start_y);
	screen = create_win(screen_params.height,
						screen_params.width,
						screen_params.start_x,
						screen_params.start_y);
	bottom = create_win(bottom_params.height,
						bottom_params.width,
						bottom_params.start_x,
						bottom_params.start_y);
	print_header();
}

void IOLinux::print_header() {
	mvwprintw(header, 1, 24, "M I N E S W E E P E R");
    wrefresh(header);
}

void IOLinux::close_IO() {
	destroy_win(header);
	destroy_win(screen);
	destroy_win(bottom);
	endwin();
	refresh();
}

void IOLinux::_debug(WINDOW* win, const char* msg, int x, int y) {
	mvwprintw(win, x, y, msg);
	wrefresh(win);
}

// sets a tile's color based on its content
void IOLinux::set_tile_color(char ch, bool attr_on, int print_type) {
	if (ch >= '1' && ch <= '8') {
		set_color(screen, "Number", attr_on);
		return;
	}

	switch(ch) {
		case ' ': {
			set_color(screen, "Undiscovered", attr_on);
			break;
		}
		case '.': {
			set_color(screen, "Empty", attr_on);
			break;
		}
		case 'F': {
			set_color(screen, "Flag", attr_on);
			break;
		}
		case 'o': {
			set_color(screen, "Highlight", attr_on);
			break;
		}
		case '+': {
			if(print_type == 1) {
				set_color(screen, "Cursor_1", attr_on);
			} else {
				set_color(screen, "Cursor_2", attr_on);
			}
			break;
		}
		case 'x': {
			if(print_type == 1) {
				set_color(screen, "Highlight", attr_on);
			} else {
				set_color(screen, "Important", attr_on);
			}
			break;
		}
		default:
			break;
	}
}

void IOLinux::print_board(char visible_field[][MAT_WIDTH], int c_x, int c_y, int marked, double percent) {
	int height = settings->get_height() + 2;
	int width = settings->get_width() + 2;

	for(int i = 1; i < height-1; i ++) {
		for(int j = 1; j < width-1; j++) {
			if(i != c_x || j != c_y) {
				set_tile_color(visible_field[i][j], true, 0);
				mvwaddch(screen, i, j, visible_field[i][j]);
				set_tile_color(visible_field[i][j], false, 0);
			} else {
				if(visible_field[i][j] == '.') {
					set_tile_color(k_cursor, true, 1);
				} else {
					set_tile_color(k_cursor, true, 0);
				}

				mvwaddch(screen, i, j, k_cursor);

				if(visible_field[i][j] == '.') {
					set_tile_color(k_cursor, false, 1);
				} else {
					set_tile_color(k_cursor, false, 0);
				}
			}
		}
	}

	wrefresh(screen);

	print_stats(marked, settings->get_bombs(), percent);
}

void IOLinux::print_stats(int marked, int bombs, double percent) {
	int line_start = 1;
	int offset = 0;

	mvwprintw(bottom, 1, line_start, k_print_clear);
	mvwprintw(bottom, 1, line_start, "Marked ");
	if(marked > bombs) {
		set_color(bottom, "Important", true);
	} else {
		set_color(bottom, "Highlight", true);
	}

	offset += 7;
	mvwprintw(bottom, 1, line_start + offset, "%d", marked);
	if(marked > bombs) {
		set_color(bottom, "Important", false);
	} else {
		set_color(bottom, "Highlight", false);
	}

	offset += get_nr_of_digits(marked) + 1;
	mvwprintw(bottom, 1, line_start + offset, "of");
	set_color(bottom, "Highlight", true);
	offset += 3;
	mvwprintw(bottom, 1, line_start + offset, "%d", bombs);
	set_color(bottom, "Highlight", false);

	offset += get_nr_of_digits(bombs);
	mvwprintw(bottom, 1, line_start + offset, ". Solved ");
	set_color(bottom, "Highlight", true);
	offset += 9;
	mvwprintw(bottom, 1, line_start + offset, "%.1f%%", percent);
	set_color(bottom, "Highlight", false);
	offset += get_nr_of_digits((int)percent) + 3;
	mvwprintw(bottom, 1, line_start + offset, ".");
	wrefresh(bottom);
}

int IOLinux::get_nr_of_digits(int n) {
	if(n < 10) return 1;
	if(n < 100) return 2;
	if(n < 1000) return 3;
	return 4;
}

void IOLinux::print_revealed_board(char visible_field[][MAT_WIDTH], bool won) {
	int height = settings->get_height() + 2;
	int width = settings->get_width() + 2;
	int print_type;

	if(won) {
		print_type = 1;
	} else {
		print_type = 2;
	}

	for(int i = 1; i < height-1; i ++) {
		for(int j = 1; j < width-1; j++) {
			set_tile_color(visible_field[i][j], true, print_type);
			mvwaddch(screen, i, j, visible_field[i][j]);
			set_tile_color(visible_field[i][j], false, print_type);
		}
	}
	wrefresh(screen);
}

void IOLinux::print_win_message() {
	noecho();
	cbreak();
	switch(settings->get_diff()) {
		case 0:
			print_str("You won! Congratulations!");
			getch();
			break;
		case 1:	
			print_str("You won! Well done!");
			getch();
			break;
		case 2:
			print_str("You won! Impressive!");
			getch();
			break;
		case 3:
			print_str("You won! A master indeed!");
			getch();
			break;
		default:
			break;
	}
	print_str("Play gain? (y/n)");
	nocbreak();
	echo();
}

void IOLinux::print_lose_message() {
	noecho();
	cbreak();
	switch(settings->get_diff()) {
		case 0:
			print_str("Baaam! Better luck next time!");
			getch();
			break;
		case 1:	
			print_str("Baaam! Try again, you can do it!");
			getch();
			break;
		case 2:
			print_str("Baaam! Stay focused!");
			getch();
			break;
		case 3:
			print_str("Baaam! Perseverance is key!");
			getch();
			break;
		default:
			break;
	}
	print_str("Play gain? (y/n)");
	nocbreak();
	echo();
}

void IOLinux::print_menu(int menu_level) {

	for(int i = 1; i < k_menu_height - 1; i ++) {
		mvwprintw(screen, i, 1, k_print_clear);
	}

	switch(menu_level) {
		case 0: {
			mvwprintw(screen, 3, k_options_pos_x, "[1] New game");
			mvwprintw(screen, 4, k_options_pos_x, "[2] Options");
			mvwprintw(screen, 5, k_options_pos_x, "[3] Exit");
			break;
		}
		case 1: {
			mvwprintw(screen, 3, k_options_pos_x, "[1] Rules");
			mvwprintw(screen, 4, k_options_pos_x, "[2] Difficulty");
			mvwprintw(screen, 6, k_options_pos_x, "[3] Back");
			break;
		}
		case 2: {
			int row = 1;
			std::stringstream ss;
			for(auto it = rules.begin(); it != rules.end(); it ++) {
					ss << *it;
					mvwprintw(screen,
								row,
								1,
								(ss.str()).c_str());
					ss.str(std::string()); // clear contents
					row ++;
			}

			mvwprintw(screen, 14, k_options_pos_x, "[1] Back");
			break;
		}
		case 3: {
			mvwprintw(screen, 3, k_options_pos_x, "[1] Novice");
			mvwprintw(screen, 4, k_options_pos_x, "[2] Adept");
			mvwprintw(screen, 5, k_options_pos_x, "[3] Master");
			mvwprintw(screen, 6, k_options_pos_x, "[4] Fullscreen Master");
			mvwprintw(screen, 7, k_options_pos_x, "[5] Custom");
			mvwprintw(screen, 9, k_options_pos_x, "[6] Back");

			int info_y_loc = screen_params.height - 2;
			int info_x_loc = 2;
			switch(settings->get_diff()) {
				case 0:
					mvwprintw(screen, info_y_loc, info_x_loc, "(Custom");
					break;
				case 1:
					mvwprintw(screen, info_y_loc, info_x_loc, "(Novice");
					break;
				case 2:
					mvwprintw(screen, info_y_loc, info_x_loc, "(Adept");
					break;
				case 3:
					mvwprintw(screen, info_y_loc, info_x_loc, "(Master");
					break;
				case 4:
					mvwprintw(screen, info_y_loc, info_x_loc, "(Master");
					break;
				default:
					break;
			}
			std::stringstream ss;
			ss << " difficulty - " << settings->get_height() << "x" 
									<< settings->get_width() << ", "
									<< settings->get_bombs() << " bombs)";

			mvwprintw(screen,
						info_y_loc,
						info_x_loc + 7,
						(ss.str()).c_str());
			break;
		}
		default:
			mvwprintw(screen, 2, 2, "ERROR");
	}

	wrefresh(screen);
}

void IOLinux::clear_bottom_input() {
    mvwprintw(bottom, 2, 1, k_print_clear);
    move(bottom_params.start_y + 2, 2);
    wrefresh(bottom);
}

char IOLinux::read_char() {

	cbreak();
	clear_bottom_input();
	char c = getch();
	nocbreak();
	return c;
}

std::string IOLinux::read_string() {
	std::string input;
	int chars = 0;

	clear_bottom_input();
	mvwaddch(bottom, 2, 1, k_prompt);
	wrefresh(bottom);
	echo();
    nocbreak();
    int c = getch();
    chars ++;

    while ( c != '\n'  && MAT_WIDTH - 3)
    {
        input.push_back(c);
        c = getch();
        chars ++;
    }

    return input;
}

int IOLinux::read_int() {
	std::string input;
	int ret;

	clear_bottom_input();
	mvwaddch(bottom, 2, 1, k_prompt);
	wrefresh(bottom);

	while(true) {
		input = read_string();

		std::stringstream inputStream(input);
		if(inputStream >> ret) {
			mvwprintw(bottom, 1, 1, k_print_clear);
			clear_bottom_input();
			return ret;
		}
	}
}

void IOLinux::print_str(std::string message) {
	mvwprintw(bottom, 1, 1, k_print_clear);
	mvwprintw(bottom, 1, 1, message.c_str());
	wrefresh(bottom);
}

void IOLinux::println_str(std::string message) {
	print_str(message);
}

// not necessary here
void IOLinux::print_clear() {
}

void IOLinux::print_diff_constraints(std::string name, int min, int max, bool err) {
	if(err) {
		std::stringstream ss;
		ss << "Bad input, the " << name << " must be between "
			<< min << " and " << max << ".";
		print_str((ss.str()).c_str());
	} else {
		name[0] = toupper(name[0]);
		std::stringstream ss;
		ss << name << " (between " << min << " and " << max << "):";
		print_str((ss.str()).c_str());
	}
}

void IOLinux::init_colors() {
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Highlight
	attr_types["Highlight"] = 1;
	init_pair(2, COLOR_BLACK, COLOR_BLACK); // Undiscovered tile
	attr_types["Undiscovered"] = 2;
	init_pair(3, COLOR_CYAN, COLOR_CYAN); // Empty tile
	attr_types["Empty"] = 3;
	init_pair(4, COLOR_GREEN, COLOR_BLACK); // Number tile
	attr_types["Number"] = 4;
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // Flag
	attr_types["Flag"] = 5;
	init_pair(6, COLOR_RED, COLOR_CYAN); // Cursor_1
	attr_types["Cursor_1"] = 6;
	init_pair(7, COLOR_RED, COLOR_BLACK); // Cursor_2
	attr_types["Cursor_2"] = 7;
	init_pair(8, COLOR_RED, COLOR_BLACK); // Important info
	attr_types["Important"] = 8;
}

void IOLinux::set_color(WINDOW* win, std::string type, bool attr_on) {

	switch(attr_types[type]) {
		case 1: { // Default
			if(attr_on) {
				wattrset(win, COLOR_PAIR(1) | A_BOLD);
			} else {
				wattroff(win, COLOR_PAIR(1) | A_BOLD);
			}
			break;
		}
		case 2: { // Undiscovered tile
			if(attr_on) {
				wattron(win, COLOR_PAIR(2));
			} else {
				wattroff(win, COLOR_PAIR(2));
			}
			break;
		}
		case 3: { // Empty tile
			if(attr_on) {
				wattron(win, COLOR_PAIR(3));
			} else {
				wattroff(win, COLOR_PAIR(3));
			}
			break;
		}
		case 4: { // Number tile
			if(attr_on) {
				wattron(win, COLOR_PAIR(4));
			} else {
				wattroff(win, COLOR_PAIR(4));
			}
			break;
		}
		case 5: { // Flag tile
			if(attr_on) {
				wattron(win, COLOR_PAIR(5) | A_BOLD);
			} else {
				wattroff(win, COLOR_PAIR(5) | A_BOLD);
			}
			break;
		}
		case 6: { // Cursor_1
			if(attr_on) {
				wattron(win, COLOR_PAIR(6) | A_BOLD);
			} else {
				wattroff(win, COLOR_PAIR(6) | A_BOLD);
			}
			break;
		}
		case 7: { // Cursor_2
			if(attr_on) {
				wattron(win, COLOR_PAIR(7) | A_BOLD);
			} else {
				wattroff(win, COLOR_PAIR(7) | A_BOLD);
			}
			break;
		}
		case 8: { // Important info
			if(attr_on) {
				wattron(win, COLOR_PAIR(8));
			} else {
				wattroff(win, COLOR_PAIR(8));
			}
			break;
		}
		default:
			break;
	}
}

void IOLinux::io_update_settings(GameSettings* settings) {
	this->settings = settings;
}
