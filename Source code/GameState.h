/**
	GameState.h
		Template class that implements the game logic.
	Templates are used so that the backend of the game is somewhat independent
	of how the game looks, or its frontend. The game uses an Input/Output 
	object to interact with the player. This object's implementation does not 
	affect the game's functionality but its class must inherit 'IOInterface.h'
	so that compatibility is ensured.

	@author Sergiu Constantinescu
*/
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "GameSettings.h"
#include "Utils.h"


template <class IO>
class GameState {
private:
	IO* io_mode;
	char visible_field[MAT_HEIGHT][MAT_WIDTH];
	char hidden_field[MAT_HEIGHT][MAT_WIDTH];
	int height;
	int width;
	int bombs;
	int cursor_x, cursor_y;
	int nr_of_tiles;
	int marked_tiles;
	int discovered_tiles;
	int safe_tiles;
	// percentage that represents the discovered safe tiles
	double percentage_disc;
	bool quit_game;
	bool game_not_over;

public:
	GameState(IO* io_mod);

	void reset_game();
	void game_loop(GameSettings *settings);
	// actions that need to be done when the game ends
	void game_over(bool won);
	void reveal_bombs();
	void set_borders();
	void place_bombs();
	void plant_flag();
	void reveal_tile(int x, int y);
	int check_tile();
	void move_up();
	void move_down();
	void move_left();
	void move_right();
	void place_numbers();
	// extracts the settings from the settings object as separate values
	void get_settings(GameSettings *settings);
	void quit();
};

#include "GameState.hpp"

#endif // _GAMESTATE_H_
