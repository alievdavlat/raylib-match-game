// A .c file includes its own header first. That way the compiler checks that
// the prototypes in board.h match the real functions below; a mismatch
// (different parameters or return type) becomes a compile error.
//
// "quotes" = our own headers, searched first in this project's folder.
// <angle brackets> = system/library headers (C standard library, raylib).
#include "board.h"
#include "score.h"   // for add_score() and reset_score()

#include <stdlib.h>  // rand(), abs()
#include <string.h>  // memset()

// "static" at file level means private: only this file can see it.
// Other files cannot use title_chars or random_tile().
static const char title_chars[TILE_TYPES] = {'#', '@', '$', '%', '&'};

// The real variables that board.h declares as "extern".
// board[y][x]: y is the row (top to bottom), x is the column (left to right).
char board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};
Vector2 grid_origin;

static char random_tile(void) {
    // rand() % TILE_TYPES gives a number from 0 to TILE_TYPES - 1
    return title_chars[rand() % TILE_TYPES];
}

// Marks every cell that is part of 3 or more equal tiles in a row or column.
// Returns true if at least one match was found.
bool find_matches(void) {
    bool found = false;

    // Forget the result of the previous check.
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            matched[y][x] = false;
        };
    };

    // Horizontal: compare each tile with the two tiles to its right.
    // x stops at BOARD_SIZE - 3 so that x + 2 never goes past the last column.
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE - 2; x++)
        {
            char t = board[y][x];
            if (t == board[y][x + 1] && t == board[y][x + 2])
            {
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;
                found = true;
                add_score(x, y, 10);
            };
        };
    };

    // Vertical: compare each tile with the two tiles below it.
    // Here y is the one that stops early, so that y + 2 stays inside the board.
    for (int y = 0; y < BOARD_SIZE - 2; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            char t = board[y][x];
            if (t == board[y + 1][x] && t == board[y + 2][x])
            {
                matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;
                found = true;
                add_score(x, y, 10);
            };
        };
    };

    return found;
}

// Removes matched tiles, lets the tiles above fall down ("gravity"),
// and fills the empty cells at the top with new random tiles.
// Works one column at a time.
void resolve_matches(void) {
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        // write_y is the lowest cell in this column that still needs a tile.
        // We read the column from the bottom up; every tile that survives
        // is moved down to write_y, then write_y moves one cell up.
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--)
        {
            if (!matched[y][x])
            {
                if (y != write_y)
                {
                    board[write_y][x] = board[y][x];
                    // The tile jumps to its new cell right away, but it is drawn
                    // (write_y - y) cells higher, so it looks like it is still up there.
                    // update_fall_offsets() then shrinks this to 0, which animates the fall.
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board[y][x] = ' ';
                }

                write_y--;
            }
        }

        // fill empty spots with new random tiles
        // Everything from write_y up to row 0 is now empty.
        // New tiles start above the top of the board and fall in.
        while (write_y >= 0)
        {
            board[write_y][x] = random_tile();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }

    // memset fills the whole array with zeros, i.e. every cell becomes false.
    memset(matched, 0, sizeof(matched));
}

void swap_tiles(int x1, int y1, int x2, int y2) {
    char temp = board[y1][x1];
    board[y1][x1] = board[y2][x2];
    board[y2][x2] = temp;
}

// Two cells are neighbours when they differ by exactly 1 step
// horizontally or vertically (diagonal gives 2, so it does not count).
bool are_tiles_adjacent(Vector2 a, Vector2 b) {
    return (abs((int)a.x - (int)b.x) + abs((int)a.y - (int)b.y)) == 1;
}

// Moves every falling tile "amount" pixels closer to its cell.
// Returns true while at least one tile is still falling.
bool update_fall_offsets(float amount) {
    bool stil_animating = false;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (fall_offset[y][x] > 0)
            {
                fall_offset[y][x] -= amount;
                if (fall_offset[y][x] < 0)
                {
                    fall_offset[y][x] = 0;
                } else {
                    stil_animating = true;
                }
            }
        }
    }
    return stil_animating;
}

void init_board(void) {
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board[y][x] = random_tile();
        };
    };

    // Center the board in the window.
    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
    grid_origin = (Vector2) {
        (GetScreenWidth() - grid_width) / 2,
        (GetScreenHeight() - grid_height) / 2,
    };

    // A random board can already contain matches. Replace those tiles
    // (without any animation) until no match is left, so the player
    // starts with a clean board.
    while (find_matches())
    {
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++)
                if (matched[y][x]) board[y][x] = random_tile();
    }
    memset(matched, 0, sizeof(matched));

    // find_matches() above also added score and popups; throw them away.
    reset_score();
}
