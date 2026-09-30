#include "game.h"
#include "audio.h"
#include "board.h"
#include "score.h"

#include <math.h>

typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY,
} TileState;

Vector2 selected_tile = {-1, -1};

static TileState tile_state;
static float fall_speed = 480.0f;
static float match_delay_timer = 0.0f;
static const float MATCH_DELAY_DURATION = 0.2f;

void init_game(void) {
    init_board();
    tile_state = STATE_IDLE;
}

void handle_input(void) {
    if (tile_state != STATE_IDLE || !IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();
    int x = (int)floorf((mouse.x - grid_origin.x) / TILE_SIZE);
    int y = (int)floorf((mouse.y - grid_origin.y) / TILE_SIZE);
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        return;
    }

    Vector2 curreent_tile = (Vector2) {x, y};
    if (selected_tile.x < 0)
    {
        selected_tile = curreent_tile;
        PlaySound(select_sound);
        return;
    }

    if (!are_tiles_adjacent(selected_tile, curreent_tile))
    {
        selected_tile = curreent_tile;
        PlaySound(select_sound);
        return;
    }

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
