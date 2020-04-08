/**
	GameSettings.cpp

	@author Sergiu Constantinescu
*/
#include "GameSettings.h"


GameSettings::GameSettings() {
	// game starts with the easy default difficulty 
	this->set_diff(1);
}

int GameSettings::get_diff() {
	return difficulty;
}

void GameSettings::set_diff(int new_diff) {
	if(new_diff > -1 && new_diff < 5) {
		difficulty = new_diff;
	}
	switch(difficulty) {
		case 0:
			break;
		case 1: // novice - 12.3% bombs
			field_height = 9;
			field_width = 9;
			field_bombs = 10;
			break;
		case 2: // adept - 15.6% bombs
			field_height = 14;
			field_width = 19;
			field_bombs = 42;
			break;
		case 3: // master - 20.65% bombs
			field_height = 14;
			field_width = 34;
			field_bombs = 98;
			break;
		case 4: // master (fullscreen) - 21% bombs
			field_height = 14;
			field_width = 68;
			field_bombs = 200;
			break;
		default:
			field_height = 9;
			field_width = 9;
			field_bombs = 10;
	}
}

void GameSettings::set_custom_diff(int height, int width, int bombs) {
	field_height = height;
	field_width = width;
	field_bombs = bombs;
}

int GameSettings::get_height() {
	return field_height;
}

int GameSettings::get_width() {
	return field_width;
}

int GameSettings::get_bombs() {
	return field_bombs;
}
