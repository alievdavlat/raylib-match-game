#include "render.h"
#include "board.h"
#include "game.h"
#include "score.h"

#include <raylib.h>
#include <stddef.h>

#define SCORE_FONT_SIZE 32

static Texture2D background;
static Font score_font;

void load_graphics(void) {
    background = LoadTexture(TextFormat("%sresources/background.png", GetApplicationDirectory()));
    score_font = LoadFontEx(TextFormat("%sresources/fonts/04b03.ttf", GetApplicationDirectory()), SCORE_FONT_SIZE, NULL, 0);
    SetTextureFilter(score_font.texture, TEXTURE_FILTER_POINT);
}

static void draw_board(void) {
    BeginScissorMode(grid_origin.x, grid_origin.y, BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE);
    DrawRectangle(
        grid_origin.x,
        grid_origin.y,
        BOARD_SIZE * TILE_SIZE,
        BOARD_SIZE * TILE_SIZE,
        Fade(DARKGRAY, 0.60f)
    );
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Rectangle rect = {
                grid_origin.x + (x * TILE_SIZE),
                grid_origin.y + (y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };

            DrawRectangleLinesEx(rect, 1, Fade(SKYBLUE, 0.3f));
            if (board[y][x] != ' ')
            {
                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board[y][x]),
                    (Vector2) {
                        rect.x + 12,
                        rect.y + 8 - fall_offset[y][x]
                    },
                    20, // fontSize
                    2,  // spacing
                    matched[y][x] ? GREEN : WHITE // tint
                );
            }
        };
    };
    EndScissorMode();
}

static void draw_selected_tile(void) {
    if (selected_tile.x < 0)
    {
        return;
    }
    DrawRectangleLinesEx((Rectangle) {
        grid_origin.x + (selected_tile.x * TILE_SIZE),
        grid_origin.y + (selected_tile.y * TILE_SIZE),
        TILE_SIZE, TILE_SIZE
    }, 2, YELLOW);
}

static void draw_score(void) {
    DrawTextEx(
        score_font,
        TextFormat("SCORE: %d", score),
        (Vector2){20, 20}, SCORE_FONT_SIZE * score_scale, 1.0f, YELLOW
    );

    for (int i = 0; i < MAX_SCORE_POPUPS; i++)
    {
        if (score_popups[i].active)
        {
            Color c = Fade(YELLOW, score_popups[i].alpha);
            DrawText(TextFormat("+%d", score_popups[i].amount), score_popups[i].position.x, score_popups[i].position.y, 20, c);
        }
    }
}

void draw_game(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        background,
        (Rectangle) { 0, 0, background.width, background.height },
        (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() },
        (Vector2) {0, 0},
        0.0f,
        WHITE
    );
    draw_board();
    draw_selected_tile();
    draw_score();
    EndDrawing();
}

void unload_graphics(void) {
    UnloadTexture(background);
    UnloadFont(score_font);
}
