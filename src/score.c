// Own header first, then the modules this file uses (see board.c).
#include "score.h"
#include "board.h"   // TILE_SIZE and grid_origin, to place popups on the board

#include <string.h>  // memset()

// These are the definitions of the variables that score.h declares "extern".
// Each one must be defined in exactly one .c file.
int score = 0;
float score_scale = 1.0f; // 1.0 = normal size; the score text is drawn this many times bigger
SocrePopup score_popups[MAX_SCORE_POPUPS] = {0};

static float score_scale_velocity = 0.0f; // how fast score_scale changes, per second
static bool score_animating = false;

// Popups use a fixed-size array instead of malloc: a free slot is one with
// active == false. When a popup ends, its slot becomes free for the next one.
// If all 32 slots are busy, the new popup is simply skipped.
void add_score_popup(int x, int y, int amount, Vector2 grid_origin) {
    for (int i = 0; i < MAX_SCORE_POPUPS; i++)
    {
        if (!score_popups[i].active)
        {
            // Center of cell (x, y) on the screen.
            score_popups[i].position = (Vector2) {
                grid_origin.x + x * TILE_SIZE + TILE_SIZE / 2,
                grid_origin.y + y * TILE_SIZE + TILE_SIZE / 2,
            };
            score_popups[i].amount = amount;
            score_popups[i].lifetime = 1.0f;
            score_popups[i].alpha = 1.0f;
            score_popups[i].active = true;
            break;
        }
    }
}

void add_score(int x, int y, int amount) {
    score += amount;

    // Make the score text jump to 2x its size; update_score() shrinks it back to 1x.
    score_animating = true;
    score_scale = 2.0f;
    score_scale_velocity = -2.5f;

    add_score_popup(x, y, amount, grid_origin);
}

void reset_score(void) {
    memset(score_popups, 0, sizeof(score_popups));
    score = 0;
}

void update_score(float dt) {
    // update score popups array
    // Each popup floats up 30 px per second and fades out over 1 second.
    for (int i = 0; i < MAX_SCORE_POPUPS; i++)
    {
        if (score_popups[i].active)
        {
            score_popups[i].lifetime -= dt;
            score_popups[i].position.y -= 30 * dt;
            score_popups[i].alpha = score_popups[i].lifetime; // 1.0 -> 0.0 as it dies

            if (score_popups[i].lifetime <= 0.0f)
            {
                score_popups[i].active = false;
            }
        };
    };

    // update the score animation
    // Shrink from 2.0 by 2.5 per second, so it is back to 1.0 after 0.4 s.
    if (score_animating)
    {
        score_scale += score_scale_velocity * dt;
        if (score_scale <= 1.0f)
        {
            score_scale = 1.0f;
            score_animating = false;
        }
    }
}
