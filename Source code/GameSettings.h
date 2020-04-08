/**
	GameSettings.h

	@author Sergiu Constantinescu
*/
#ifndef _GAMESETTINGS_H_
#define _GAMESETTINGS_H_


class GameSettings {
private:
	// 0 - custom
	// 1 - novice
	// 2 - adept
	// 3 - master
	// 4 - master (fullscreen)
	int difficulty;
	int field_height;
	int field_width;
	int field_bombs;

public:
	GameSettings();

	int get_diff();
	void set_diff(int new_diff);
	void set_custom_diff(int height, int width, int bombs);
	int get_height();
	int get_width();
	int get_bombs();
};

#endif // _GAMESETTINGS_H_
