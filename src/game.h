// Game state: mouse input and the IDLE / MATCH_DELAY / ANIMATING state machine
#ifndef GAME_H
#define GAME_H

#include <raylib.h>

extern Vector2 selected_tile;

void init_game(void);
void handle_input(void);
void update_game(float dt);

#endif
