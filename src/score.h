// Score, "+10" popups and the score bounce animation
#ifndef SCORE_H
#define SCORE_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_SCORE_POPUPS 32

typedef struct {
    Vector2 position;
    int amount;
    float lifetime;
    float alpha;
    bool active;
} SocrePopup;

extern int score;
extern float score_scale;
extern SocrePopup score_popups[MAX_SCORE_POPUPS];

void add_score(int x, int y, int amount);
void add_score_popup(int x, int y, int amount, Vector2 grid_origin);
void reset_score(void);
void update_score(float dt);

#endif
