// Drawing: background, board, selected tile, score and popups
//
// The smallest header in the project: main.c only needs to load the graphics,
// draw a frame and free them. The helpers (draw_board, draw_score, ...) and the
// textures are "static" in render.c, so they are not listed here.

// Include guard: skip this file if it was already included (see board.h).
#ifndef RENDER_H
#define RENDER_H

// No #include needed: these functions use no types other than void.

void load_graphics(void);
void draw_game(void);
void unload_graphics(void);

#endif // RENDER_H
