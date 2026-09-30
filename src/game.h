// Game state: mouse input and the IDLE / MATCH_DELAY / ANIMATING state machine
//
// Only the parts other files need are listed here. The state enum and the
// timers stay private inside game.c (declared "static" there), so no other
// file can change the game state by accident.

// Include guard: skip this file if it was already included (see board.h).
#ifndef GAME_H
#define GAME_H

// Needed here because Vector2 is a raylib type used below.
// A header should include everything it itself uses, so it works on its own.
#include <raylib.h>

// render.c reads this to draw the yellow frame around the selected tile.
extern Vector2 selected_tile;

void init_game(void);
void handle_input(void);
void update_game(float dt);

#endif // GAME_H
