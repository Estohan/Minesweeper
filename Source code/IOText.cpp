/**
	IOText.cpp
		Simple text character based input/output interface
	for the game. Contains the implementations of the functions
	defined in 'IOText.h'.

	@author Sergiu Constantinescu
*/
#include <iostream>
#include <string>
#include <sstream>
#include "IOText.h"

IOText::IOText(GameSettings* settings) {
	this->settings = settings;
}

// not necessary here
void IOText::init_IO(bool menu_type_scr) {
}

// not necessary here
void IOText::close_IO() {
}

char IOText::read_char() {
	std::string input;
	getline(std::cin, input);
	return input[0];
}

std::string IOText::read_string() {
	std::string input;
	getline(std::cin, input);
	return input;
}

int IOText::read_int() {
	std::string input;
	int ret;

	while(true) {
		getline(std::cin, input);

		std::stringstream inputStream(input);
		if(inputStream >> ret) {
			return ret;
		}
		println_str("A number please!");
	}
}

// 'clears the screen' by writing 23 new lines to stdout
void IOText::print_clear() {
	for(int i = 0; i < 23; i ++) {
		std::cout << std::endl;
	}
}

void IOText::print_header() {
	std::cout << LINE01 << LINE01 << std::endl;
	std::cout << LINE02 << std::endl;
	std::cout << LINE01 << LINE01 << std::endl;
}

void IOText::print_menu(int menu_level) {
	print_clear();
	print_header();
	switch(menu_level) {
		case 0:
			std::cout << std::endl; // space
			std::cout << "\t[1] New game" << std::endl;
			std::cout << "\t[2] Options" << std::endl;
			std::cout << "\t[3] Exit" << std::endl;
			std::cout << std::endl; // space
			std::cout << LINE01 << LINE01 << std::endl; // border
			break;
		case 1:
			std::cout << std::endl; // space
			std::cout << "\t[1] Rules" << std::endl;
			std::cout << "\t[2] Difficulty" << std::endl;
			std::cout << std::endl; // space
			std::cout << "\t[3] Back" << std::endl;
			std::cout << std::endl; // space
			std::cout << LINE01 << LINE01 << std::endl; // border
			break;
		case 2:
			std::cout << std::endl; // space
			for(auto it = rules.begin(); it != rules.end(); it ++) {
				std::cout << *it << std::endl;
			}
			std::cout << std::endl; // space
			std::cout << "\t[1] Back" << std::endl;
			std::cout << std::endl; // space
			std::cout << LINE01 << LINE01 << std::endl; // border
			break;
		case 3:
			std::cout << std::endl; // space
			std::cout << "\t[1] Novice" << std::endl;
			std::cout << "\t[2] Adept" << std::endl;
			std::cout << "\t[3] Master" << std::endl;
			std::cout << "\t[4] Fullscreen Master" << std::endl;
			std::cout << "\t[5] Custom" << std::endl;
			std::cout << std::endl; // space
			std::cout << "\t[6] Back" << std::endl;
			std::cout << std::endl; // space
			std::cout << "(";
				switch(settings->get_diff()) {
					case 0:
						std::cout << "Custom";
						break;
					case 1:
						std::cout << "Novice";
						break;
					case 2:
						std::cout << "Adept";
						break;
					case 3:
						std::cout << "Master";
						break;
					case 4:
						std::cout << "Master";
						break;
					default:
						break;
				}
			std::cout << " difficulty - " << settings->get_height();
			std::cout << "x" << settings->get_width();
			std::cout << ", " << settings->get_bombs();
			std::cout << "bombs)" << std::endl;
			std::cout << LINE01 << LINE01 << std::endl; // border
			break;
		default:
			std::cout << "MENU DISPLAYING ERROR" << std::endl;
	}
}

void IOText::print_str(std::string message) {
	std::cout << message;
}

void IOText::println_str(std::string message) {
	std::cout << message << std::endl;
}

void IOText::print_diff_constraints(std::string name,
										int min,
										int max,
										bool err) {
	if(err) {
		std::cout << "Bad input, the " << name << " must be between "
					<< min << " and " << max << ".";
		std::cout << std::endl << "> ";
	} else {
		name[0] = toupper(name[0]);
		std::cout << name << " (between " << min << " and " << max << "):";
		std::cout << std::endl << "> ";
	}
}

void IOText::io_update_settings(GameSettings* settings) {
	this->settings = settings;
}

void IOText::print_board(char visible_field[][MAT_WIDTH],
							int c_x,
							int c_y,
							int marked,
							double percent) {
	int height = settings->get_height() + 2;
	int width = settings->get_width() + 2;

	print_clear();
	print_header();
	for(int i = 0; i < height; i ++) {
		for(int j = 0; j < width; j++) {
			this->visible_field[i][j] = visible_field[i][j];
			if(i != c_x || j != c_y) {
				std::cout << this->visible_field[i][j];
			} else {
				std::cout << '+';
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Marked " << marked << " of " << settings->get_bombs()
				<< " bombs. Solved " << (int)percent << "%%." << std::endl;
}

void IOText::print_revealed_board(char visible_field[][MAT_WIDTH], bool won) {
	int height = settings->get_height() + 2;
	int width = settings->get_width() + 2;

	print_clear();
	print_header();
	for(int i = 0; i < height; i ++) {
		for(int j = 0; j < width; j++) {
			std::cout << visible_field[i][j];
		}
		std::cout << std::endl;
	}
}

void IOText::print_win_message() {
	switch(settings->get_diff()) {
		case 0:
			std::cout << "You won! Congratulations!" << std::endl;
			break;
		case 1:
			std::cout << "You won! Well done!" << std::endl;
			break;
		case 2:
			std::cout << "You won! Impressive!" << std::endl;
			break;
		case 3:
			std::cout << "You won! A master indeed!" << std::endl;
			break;
		default:
			break;
	}
	std::cout << "Play again? (y/n)" << std::endl << "> ";
}

void IOText::print_lose_message() {
	switch(settings->get_diff()) {
		case 0:
			std::cout << "Baaam! Better luck next time!" << std::endl;
			break;
		case 1:
			std::cout << "Baaam! Try again, you can do it!" << std::endl;
			break;
		case 2:
			std::cout << "Baaam! Stay focused!" << std::endl;
			break;
		case 3:
			std::cout << "Baaam! Perseverance is key!" << std::endl;
			break;
		default:
			break;
	}
	std::cout << "Play again? (y/n)" << std::endl << "> ";
}
