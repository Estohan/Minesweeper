/**
	Utils.h
		Macros and constants shared by some other game source files.

	@author Sergiu Constantinescu
*/
#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

#define LINE01 "==================================="
#define LINE02 "                        M I N E S W E E P E R"

// max dimensions of the game board, counting the borders too
#define MAT_HEIGHT  17
#define MAT_WIDTH	71
// customizable game board limits
#define MAX_HEIGHT 	14 // 22
#define MIN_HEIGHT	3
#define MAX_WIDTH 	68 // 52
#define MIN_WIDTH	3
#define MIN_BOMBS	1

// symbols used on or 'under' the game board
#define WALL 	'#'
#define FLAGT	'F'
#define BOMBT	'x'
#define EMPTYH	' '
#define EMPTYD	'.'
#define GOODFT	'o'

const std::vector<std::string> rules{
"   The game board is represented by tiles that you can check.",
"   There are 3 types of tiles: bombs, numbers and empty spaces.",
"   The goal of the game is to uncover all the tiles that do not",
" contain bombs. A number tile is a safe tile and it tells you how",
" many bombs there are in total under its 8 neighbours. Therefore",
" numbers will range from 1 to 8. A tile that has no bombs near it",
" is an empty tile and is safe to reveal. You can mark suspicious",
" tiles with flags.",
"   Controls: w/a/s/d - move the cursor ('+') around,",
"             space   - reveal tile,",
"             e       - drop/take flag ('F'),",
"             q       - quit.",
};

#endif // _UTILS_H_
