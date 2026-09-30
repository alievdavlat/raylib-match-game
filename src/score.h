// Score, "+10" popups and the score bounce animation

// Include guard: skip this file if it was already included (see board.h).
#ifndef SCORE_H
#define SCORE_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_SCORE_POPUPS 32

// A struct groups several values into one type.
// typedef gives it the short name SocrePopup, so we can write
// "SocrePopup p;" instead of "struct { ... } p;".
// It lives in the header because render.c needs to read the fields to draw them.
typedef struct {
    Vector2 position;  // screen position, moves up over time
    int amount;        // the number shown after "+"
    float lifetime;    // seconds left before it disappears
    float alpha;       // opacity: 1.0 = solid, 0.0 = invisible
    bool active;       // false = this array slot is free
} SocrePopup;

// Created in score.c; render.c reads them to draw the score.
extern int score;
extern float score_scale;
extern SocrePopup score_popups[MAX_SCORE_POPUPS];

void add_score(int x, int y, int amount);
void add_score_popup(int x, int y, int amount, Vector2 grid_origin);
void reset_score(void);
void update_score(float dt);

#endif // SCORE_H
