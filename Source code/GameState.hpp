/**
	GameState.hpp
		Template class that implements the game logic.
		Contains the implementation of functions declared in 'GameState.h'.

	@author Sergiu Constantinescu
*/
#ifndef __GAMESTATE_HPP_
#define __GAMESTATE_HPP_

#include <stdlib.h>
#include <time.h>


template <class IO>
GameState<IO>::GameState(IO* io_mode) :
	io_mode(io_mode)
	,height(9)
	,width(9)
	,bombs(10)
	,cursor_x(1)
	,cursor_y(1),
	nr_of_tiles(49),
	marked_tiles(0),
	discovered_tiles(0),
	safe_tiles(37),
	percentage_disc(0.0),
	quit_game(false),
	game_not_over(true)
	{}

template <class IO>
void GameState<IO>::reset_game() {
	for(int i = 0; i < MAT_HEIGHT; i ++) {
		for(int j = 0; j < MAT_WIDTH; j ++) {
			visible_field[i][j] = EMPTYH;
			hidden_field[i][j] = EMPTYH;
		}
	}

	game_not_over = true;
	discovered_tiles = 0;
	percentage_disc = 0.0;
	marked_tiles = 0;
	cursor_x = 1;
	cursor_y = 1;
}

// The game's loop, it can be generally described by the following steps:
// loop 1:
// - prepare board
//		loop 2:
//			- wait for input
//			- check win/lose conditions
// - play again?

template <class IO>
void GameState<IO>::game_loop(GameSettings *settings) {

	char input;

	get_settings(settings);
	io_mode->io_update_settings(settings);
	io_mode->init_IO(false);
	reset_game();
	place_bombs();
	place_numbers();
	set_borders();

	game_not_over = true;
	quit_game = false;

	io_mode->print_board(visible_field, 
							cursor_x, 
							cursor_y, 
							marked_tiles, 
							percentage_disc);

	while(game_not_over) {

		input = io_mode->read_char();
		switch(input) {
			case 'w':
				move_up();
				break;
			case 's':
				move_down();
				break;
			case 'a':
				move_left();
				break;
			case 'd':
				move_right();
				break;
			case ' ': {
				int event = check_tile();
				if(event != 0) { // clicked on flag, nothing happens
					if(event == -1) { // lose condition
						game_not_over = false;
					} else {
						reveal_tile(cursor_x, cursor_y);
						percentage_disc = ((double)discovered_tiles / 
											(nr_of_tiles - bombs)) * 100.00;
					}
				}
				break;
			}
			case 'e':
				plant_flag();
				break;
			case 'q':
				// among other things, sets the value of quit_game to True
				quit();
				break;
			default:
				break;
		}

		if(!quit_game) {
			if(game_not_over) {
				io_mode->print_board(visible_field, 
										cursor_x, 
										cursor_y, 
										marked_tiles, 
										percentage_disc);
			} else {
				// false parameter means the game was lost
				game_over(false);
			}
		}

		// win condition: uncover all safe tiles
		if(discovered_tiles == safe_tiles) {
			// true parameter means the game was won
			game_over(true);
		}
	}

	io_mode->close_IO();
}

// reveals the board, prints game over message and 
// asks the player if they want to start a new game
// (if the answer is yes, it also resets the game)
template <class IO>
void GameState<IO>::game_over(bool won) {
	
	reveal_bombs();
	io_mode->print_revealed_board(visible_field, won);
	if(won) {
		io_mode->print_win_message();
	} else {
		io_mode->print_lose_message();
	}

	char input;
	while(true) {
		input = io_mode->read_char();
		if(input == 'y') {
			reset_game();
			place_bombs();
			place_numbers();
			set_borders();
			io_mode->print_board(visible_field,
									cursor_x,
									cursor_y,
									marked_tiles,
									percentage_disc);
			break;
		} else if (input == 'n'){
			game_not_over = false;
			break;
		}
	}
}

// reveals the bombs, representing the correctly marked bombs with the GOODFT
// character (defined in 'Utils.h') and the ones that remained untouched 
// with BOMBT
template <class IO>
void GameState<IO>::reveal_bombs() {
	for(int i = 0; i < height + 2; i ++) {
		for(int j = 0; j < width + 2; j ++) {
			if(hidden_field[i][j] == BOMBT) {
				if(visible_field[i][j] == FLAGT) {
					visible_field[i][j] = GOODFT;
				} else {
					visible_field[i][j] = BOMBT;
				}
			}
		}
	}
}

// sets borders marked with WALL (defined in 'Utils.h') around the game
// board according to the board dimensions
template <class IO>
void GameState<IO>::set_borders() {
	for(int i = 0; i < height + 2; i ++) {
		hidden_field[i][0] = WALL;
		hidden_field[i][width+1] = WALL;
		visible_field[i][0] = WALL;
		visible_field[i][width+1] = WALL;
	}

	for(int i = 0; i < width + 2; i ++) {
		hidden_field[0][i] = WALL;
		hidden_field[height+1][i] = WALL;
		visible_field[0][i] = WALL;
		visible_field[height+1][i] = WALL;
	}
}

// randomly (using device's time as seed) populates the game board
// with mines.
template <class IO>
void GameState<IO>::place_bombs() {
	int bombs_left = this->bombs;
	int bomb_x, bomb_y;
	srand(time(NULL));

	while(bombs_left) {
		bomb_y = (rand() % (this->width)) + 1;
		bomb_x = (rand() % (this->height)) + 1;

		if(hidden_field[bomb_x][bomb_y] == EMPTYH) {
			hidden_field[bomb_x][bomb_y] = BOMBT;
			bombs_left --;
		}
	}
}

// plants/removes the flag (marked with FLAGT, defined in 'Utils.h')
// at the cursor's position
template <class IO>
void GameState<IO>::plant_flag() {

	if(visible_field[cursor_x][cursor_y] == EMPTYH) {
		visible_field[cursor_x][cursor_y] = FLAGT;
		marked_tiles++;
	} else if(visible_field[cursor_x][cursor_y] == FLAGT) {
		visible_field[cursor_x][cursor_y] = EMPTYH;
		marked_tiles--;
	}
}

// recursively reveals a portion of the board starting with an empty tile
// it reveals all adjacent empty tiles and numbers
template <class IO>
void GameState<IO>::reveal_tile(int x, int y) {
	if(hidden_field[x][y] == EMPTYH) { // hidden empty tile
		if(visible_field[x][y] != EMPTYD) { // discovered empty tile
			discovered_tiles ++;
		}
		visible_field[x][y] = EMPTYD;
		hidden_field[x][y] = EMPTYD;
		// up
		if(x > 1) {
			reveal_tile(x - 1, y);
			if(y > 1) {
				// up left
				reveal_tile(x - 1, y - 1);
			}
			if(y < width) {
				// up right
				reveal_tile(x - 1, y + 1);
			}
		}

		// left
		if(y > 1) {
			reveal_tile(x, y - 1);
		}
		// right
		if(y < width) {
			reveal_tile(x, y + 1);
		}

		// down
		if(x < height) {
			reveal_tile(x + 1, y);
			if(y > 1) {
				// down left
				reveal_tile(x + 1, y - 1);
			}
			if(y < width) {
				// down right
				reveal_tile(x + 1, y + 1);
			}
		}
	} else if (hidden_field[x][y] > '0' &&
				hidden_field[x][y] < '9') {
		if(visible_field[x][y] != hidden_field[x][y]) {
			visible_field[x][y] = hidden_field[x][y];
			discovered_tiles ++;
		}
	}
}

// checks if the player tries to check a mined tile
template <class IO>
int GameState<IO>::check_tile() {
	if(visible_field[cursor_x][cursor_y] == FLAGT) {
		return 0; // can't check a flagged tile
	} else if(hidden_field[cursor_x][cursor_y] == BOMBT) {
			return -1;
	}
	return 1;
}

template <class IO>
void GameState<IO>::move_up() {
	if(cursor_x > 1) {
		cursor_x --;
	}
}

template <class IO>
void GameState<IO>::move_down() {
	if(cursor_x < height) {
		cursor_x ++;
	}
}

template <class IO>
void GameState<IO>::move_left() {
	if(cursor_y > 1) {
		cursor_y --;
	}
}

template <class IO>
void GameState<IO>::move_right() {
	if(cursor_y < width) {
		cursor_y ++;
	}
}

template <class IO>
// for each bomb in the field increments the numbers around it
// (or puts numbers around it) so that at the end the board will
// contain a correct configuration of numbers and mines
void GameState<IO>::place_numbers() {
	for(int i = 1; i < this->height+1; i ++) {
		for(int j = 1; j < this->width+1; j ++) {
			if(hidden_field[i][j] == BOMBT) {
				// upper left corner
				if(hidden_field[i-1][j-1] != BOMBT) {
					if(hidden_field[i-1][j-1] > '0' && 
						hidden_field[i-1][j-1] < '8') {
						hidden_field[i-1][j-1] ++;
					} else {
						hidden_field[i-1][j-1] = '1';
					}
				}
				// upper middle
				if(hidden_field[i-1][j] != BOMBT) {
					if(hidden_field[i-1][j] > '0' && 
						hidden_field[i-1][j] < '8') {
						hidden_field[i-1][j] ++;
					} else {
						hidden_field[i-1][j] = '1';
					}
				}
				// upper right corner
				if(hidden_field[i-1][j+1] != BOMBT) {
					if(hidden_field[i-1][j+1] > '0' && 
						hidden_field[i-1][j+1] < '8') {
						hidden_field[i-1][j+1] ++;
					} else {
						hidden_field[i-1][j+1] = '1';
					}
				}
				// left
				if(hidden_field[i][j-1] != BOMBT) {
					if(hidden_field[i][j-1] > '0' && 
						hidden_field[i][j-1] < '8') {
						hidden_field[i][j-1] ++;
					} else {
						hidden_field[i][j-1] = '1';
					}
				}
				// right
				if(hidden_field[i][j+1] != BOMBT) {
					if(hidden_field[i][j+1] > '0' && 
						hidden_field[i][j+1] < '8') {
						hidden_field[i][j+1] ++;
					} else {
						hidden_field[i][j+1] = '1';
					}
				}
				// lower left corner
				if(hidden_field[i+1][j-1] != BOMBT) {
					if(hidden_field[i+1][j-1] > '0' && 
						hidden_field[i+1][j-1] < '8') {
						hidden_field[i+1][j-1] ++;
					} else {
						hidden_field[i+1][j-1] = '1';
					}
				}
				// lower middle
				if(hidden_field[i+1][j] != BOMBT) {
					if(hidden_field[i+1][j] > '0' && 
						hidden_field[i+1][j] < '8') {
						hidden_field[i+1][j] ++;
					} else {
						hidden_field[i+1][j] = '1';
					}
				}
				// lower right corner
				if(hidden_field[i+1][j+1] != BOMBT) {
					if(hidden_field[i+1][j+1] > '0' && 
						hidden_field[i+1][j+1] < '8') {
						hidden_field[i+1][j+1] ++;
					} else {
						hidden_field[i+1][j+1] = '1';
					}
				}
			}
		}
	}
}

template <class IO>
void GameState<IO>::get_settings(GameSettings *settings) {
	height = settings->get_height();
	width = settings->get_width();
	bombs = settings->get_bombs();

	nr_of_tiles = (height) * (width);
	safe_tiles = nr_of_tiles - bombs;
}

template <class IO>
void GameState<IO>::quit() {
	io_mode->println_str("Do you really want to exit? (y/n)");

	char input;
	while(true) {
		input = io_mode->read_char();

		if(input == 'y') {
			reset_game();
			game_not_over = false;
			quit_game = true;
			break;
		} else {
			break;
		}
	}
}

#endif // __GAMESTATE_HPP_
