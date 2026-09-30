#include "score.h"
#include "board.h"

#include <string.h>

int score = 0;
float score_scale = 1.0f;
SocrePopup score_popups[MAX_SCORE_POPUPS] = {0};

static float score_scale_velocity = 0.0f;
static bool score_animating = false;

void add_score_popup(int x, int y, int amount, Vector2 grid_origin) {
    for (int i = 0; i < MAX_SCORE_POPUPS; i++)
    {
        if (!score_popups[i].active)
        {
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
    for (int i = 0; i < MAX_SCORE_POPUPS; i++)
    {
        if (score_popups[i].active)
        {
            score_popups[i].lifetime -= dt;
            score_popups[i].position.y -= 30 * dt;
            score_popups[i].alpha = score_popups[i].lifetime;

            if (score_popups[i].lifetime <= 0.0f)
            {
                score_popups[i].active = false;
            }
        };
    };

    // update the score animation
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
