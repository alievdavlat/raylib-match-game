#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include "audio.h"
#include "game.h"
#include "render.h"

int main(void)
{
    const int screen_width = 800;
    const int screen_hieght = 450;

    InitWindow(screen_width, screen_hieght, "Match Game");
    SetTargetFPS(60);
    srand(time(NULL));

    load_audio();
    load_graphics();
    init_game();

    while (!WindowShouldClose())
    {
        update_audio();
        handle_input();
        update_game(GetFrameTime());
        draw_game();
    }

    unload_graphics();
    unload_audio();
    CloseWindow();
    return 0;
}
