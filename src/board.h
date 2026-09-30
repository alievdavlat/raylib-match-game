// Board state: tiles, match detection, falling, swapping
//
// What is a header (.h) file?
// A .c file is compiled on its own and knows nothing about other .c files.
// A header is a list of what board.c offers to the rest of the program:
// constants, variables and function signatures, but no function bodies.
// Other files write #include "board.h" to use them (like `import` in JS).

// Include guard (#ifndef ... #define ... #endif):
// #include simply copy-pastes this file's text into the file that includes it.
// If one .c file ends up including board.h twice (directly and through another
// header), everything below would be declared twice and the compiler would fail.
//   1st time: BOARD_H is not defined yet -> define it and read the file.
//   2nd time: BOARD_H is already defined -> skip everything down to #endif.
// The name BOARD_H is just a unique label, by convention the file name in capitals.
#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>
#include <stdbool.h>

// #define creates a constant that the compiler replaces with its value
// before compiling (BOARD_SIZE becomes 8 everywhere). It is in the header
// because other files (render.c, score.c, game.c) need the same numbers.
#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5

// "extern" means: this variable exists, but it is created in another file (board.c).
// The header only announces it; the real variable is defined once in board.c.
// If the header created it (without extern), every .c file that includes it
// would get its own copy and the linker would report "multiple definition".
extern char board[BOARD_SIZE][BOARD_SIZE];          // the tile symbol in each cell, ' ' = empty
extern bool matched[BOARD_SIZE][BOARD_SIZE];        // true for cells that are part of a match
extern float fall_offset[BOARD_SIZE][BOARD_SIZE];   // how many pixels above its cell a tile is drawn
extern Vector2 grid_origin;                         // screen position of the board's top-left corner

// Function prototypes: name, parameters and return type, ending with ';'.
// They tell other files how to call these functions; the bodies are in board.c.
// (void) means "takes no parameters". In C, an empty () would mean
// "unknown parameters", so (void) is the stricter way to write it.
void init_board(void);
bool find_matches(void);
void resolve_matches(void);
void swap_tiles(int x1, int y1, int x2, int y2);
bool are_tiles_adjacent(Vector2 a, Vector2 b);
bool update_fall_offsets(float amount);

// End of the include guard started by #ifndef BOARD_H.
#endif
