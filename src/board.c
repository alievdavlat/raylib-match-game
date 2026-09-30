#include "board.h"
#include "score.h"

#include <stdlib.h>
#include <string.h>

static const char title_chars[TILE_TYPES] = {'#', '@', '$', '%', '&'};

char board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};
Vector2 grid_origin;

static char random_tile(void) {
    return title_chars[rand() % TILE_TYPES];
}

bool find_matches(void) {
    bool found = false;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            matched[y][x] = false;
        };
    };

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

void resolve_matches(void) {
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--)
        {
            if (!matched[y][x])
            {
                if (y != write_y)
                {
                    board[write_y][x] = board[y][x];
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board[y][x] = ' ';
                }

                write_y--;
            }
        }

        // fill empty spots with new random tiles
        while (write_y >= 0)
        {
            board[write_y][x] = random_tile();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }
    memset(matched, 0, sizeof(matched));
}

void swap_tiles(int x1, int y1, int x2, int y2) {
    char temp = board[y1][x1];
    board[y1][x1] = board[y2][x2];
    board[y2][x2] = temp;
}

bool are_tiles_adjacent(Vector2 a, Vector2 b) {
    return (abs((int)a.x - (int)b.x) + abs((int)a.y - (int)b.y)) == 1;
}

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

    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
    grid_origin = (Vector2) {
        (GetScreenWidth() - grid_width) / 2,
        (GetScreenHeight() - grid_height) / 2,
    };

    while (find_matches())
    {
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++)
                if (matched[y][x]) board[y][x] = random_tile();
    }
    memset(matched, 0, sizeof(matched));
    reset_score();
}
