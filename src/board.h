// Board state: tiles, match detection, falling, swapping
#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5

extern char board[BOARD_SIZE][BOARD_SIZE];
extern bool matched[BOARD_SIZE][BOARD_SIZE];
extern float fall_offset[BOARD_SIZE][BOARD_SIZE];
extern Vector2 grid_origin;

void init_board(void);
bool find_matches(void);
void resolve_matches(void);
void swap_tiles(int x1, int y1, int x2, int y2);
bool are_tiles_adjacent(Vector2 a, Vector2 b);
bool update_fall_offsets(float amount);

#endif
