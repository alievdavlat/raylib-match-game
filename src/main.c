#include <raylib.h>
#include <stdlib.h>  // srand()
#include <time.h>    // time()

// main.c only needs the top-level functions of each module.
// It does not know about the board or the score: game.c and render.c handle that.
#include "audio.h"
#include "game.h"
#include "render.h"

int main(void)
{
    const int screen_width = 800;
    const int screen_hieght = 450;

    InitWindow(screen_width, screen_hieght, "Match Game");
    SetTargetFPS(60);

    // Seed the random generator with the current time,
    // otherwise rand() would produce the same board on every run.
    srand(time(NULL));

    // Everything that needs a window or an audio device
    // must be loaded after InitWindow().
    load_audio();
    load_graphics();
    init_game();

    // The game loop runs once per frame (60 times per second).
    // Every frame does the same three steps:
    //   1. input  - what did the player do?
    //   2. update - change the game data based on that and on elapsed time
    //   3. draw   - redraw the whole screen from the current data
    // The screen is never "edited": it is cleared and drawn again every frame.
    while (!WindowShouldClose())
    {
        update_audio();
        handle_input();

        // GetFrameTime() is the time in seconds since the last frame (~0.016 at 60 FPS).
        // Multiplying speeds by it keeps movement the same even if the FPS drops.
        update_game(GetFrameTime());
        draw_game();
    }

    // Free GPU textures and audio buffers in reverse order of loading.
    unload_graphics();
    unload_audio();
    CloseWindow();
    return 0;
}
