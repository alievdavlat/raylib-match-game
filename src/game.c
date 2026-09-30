// Own header first, then the modules this file uses (see board.c).
#include "game.h"
#include "audio.h"   // the sounds to play
#include "board.h"   // board functions, BOARD_SIZE, TILE_SIZE, grid_origin
#include "score.h"   // update_score()

#include <math.h>    // floorf()

// The game is always in exactly one of these states.
// Each frame only the work of the current state is done:
//   IDLE        -> waiting for the player to click
//   MATCH_DELAY -> a match was found; it stays green for a short moment
//   ANIMATING   -> matched tiles are gone and the tiles above are falling
// Flow: IDLE -> MATCH_DELAY -> ANIMATING -> (new match? MATCH_DELAY : IDLE)
typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY,
} TileState;

// {-1, -1} means "nothing is selected".
Vector2 selected_tile = {-1, -1};

static TileState tile_state;
static float fall_speed = 480.0f;               // pixels per second
static float match_delay_timer = 0.0f;
static const float MATCH_DELAY_DURATION = 0.2f; // seconds

void init_game(void) {
    init_board();
    tile_state = STATE_IDLE;
}

void handle_input(void) {
    // Ignore clicks while tiles are moving.
    if (tile_state != STATE_IDLE || !IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return;
    }

    // Convert the mouse position (pixels) into a board cell (column, row).
    // floorf rounds down, so a click just left of the board gives -1, not 0.
    Vector2 mouse = GetMousePosition();
    int x = (int)floorf((mouse.x - grid_origin.x) / TILE_SIZE);
    int y = (int)floorf((mouse.y - grid_origin.y) / TILE_SIZE);
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        return;
    }

    // First click: remember the tile.
    Vector2 curreent_tile = (Vector2) {x, y};
    if (selected_tile.x < 0)
    {
        selected_tile = curreent_tile;
        PlaySound(select_sound);
        return;
    }

    // Second click on a tile that is not a neighbour: select that one instead.
    if (!are_tiles_adjacent(selected_tile, curreent_tile))
    {
        selected_tile = curreent_tile;
        PlaySound(select_sound);
        return;
    }

    // Second click on a neighbour: try the swap.
    // If it creates no match, swap back so the move is cancelled.
    swap_tiles(selected_tile.x, selected_tile.y, curreent_tile.x, curreent_tile.y);
    PlaySound(swap_sound);
    if (find_matches())
    {
        PlaySound(match_sound);
        tile_state = STATE_MATCH_DELAY;
        match_delay_timer = MATCH_DELAY_DURATION;
    } else {
        swap_tiles(selected_tile.x, selected_tile.y, curreent_tile.x, curreent_tile.y);
        PlaySound(invalid_sound);
    };
    selected_tile = (Vector2) {-1, -1};
}

void update_game(float dt) {
    // Tiles are falling. When the last one lands, check the board again:
    // the fallen tiles may have formed a new match (a cascade / combo).
    if (tile_state == STATE_ANIMATING && !update_fall_offsets(fall_speed * dt))
    {
        if (find_matches())
        {
            PlaySound(combo_sound);
            tile_state = STATE_MATCH_DELAY;
            match_delay_timer = MATCH_DELAY_DURATION;
        } else {
            tile_state = STATE_IDLE;
        };
    }

    // Count down the short pause, then remove the matched tiles
    // and start the falling animation.
    if (tile_state == STATE_MATCH_DELAY)
    {
        match_delay_timer -= dt;
        if (match_delay_timer <= 0.0f)
        {
            resolve_matches();
            tile_state = STATE_ANIMATING;
        }
    }

    update_score(dt);
}
