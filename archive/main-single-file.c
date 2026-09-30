#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5
#define SCORE_FONT_SIZE 32
#define MAX_SCORE_POPUPS 32
void add_score_popup(int x, int y, int amount, Vector2 grid_origin);

const char title_chars[TILE_TYPES] = {'#', '@', '$', '%', '&'};
char board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};


int score = 0;
Vector2 grid_origin;
Texture2D background;
Font score_font;
Vector2 selected_tile = {-1, -1};
float fall_speed = 480.0f;
float match_delay_timer = 0.0f;
const float MATCH_DELAY_DURATION = 0.2f;
float score_scale = 1.0f;
float score_scale_velocity = 0.0f;
bool score_animating = false;

Music background_music;
Sound match_sound;
Sound select_sound;
Sound swap_sound;
Sound invalid_sound;
Sound combo_sound;


typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY,
} TileState;

TileState tile_state;
typedef struct {
    Vector2 position;
    int amount;
    float lifetime;
    float alpha;
    bool active;
} SocrePopup;

SocrePopup score_popups[MAX_SCORE_POPUPS] = {0};

char random_tile() {
    return title_chars[rand() % TILE_TYPES];
}


bool find_matches () {
    bool found = false;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int  x = 0; x < BOARD_SIZE; x++)
        {
         matched[y][x] = false;
        };
        
    };
    
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE - 2; x++)
        {
            char t = board[y][x];
            if (t == board[y][x + 1] && t == board[y][x + 2])
            {
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;
                // update score
                score += 10;
                found = true;
                score_animating = true;
                score_scale = 2.0f;
                score_scale_velocity = -2.5f;
                add_score_popup(x, y, 10, grid_origin);
            };
        };

    };


    for (int y = 0; y < BOARD_SIZE - 2; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            char t = board[y][x];
            if (t == board[y + 1][x] && t == board[y + 2][x])
            {
                matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;
                // update score
                score += 10;
                found = true;
                score_animating = true;
                score_scale = 2.0f;
                score_scale_velocity = -2.5f;
                add_score_popup(x, y, 10, grid_origin);
            };
        };
    };
    
    return found;
}


void resolve_matches () {
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--)
        {
            if (!matched[y][x])
            {
                if (y != write_y)
                {
                    board[write_y][x] = board[y][x];
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board[y][x] = ' ';
                }
                
                write_y--;
            }
        }

        // fill empty spots with new random tiles
        while (write_y >= 0)
        {
            board[write_y][x] = random_tile();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }
    memset(matched, 0, sizeof(matched));
    tile_state = STATE_ANIMATING;
};

void swap_tiles(int x1, int y1, int x2, int y2) {
    char temp = board[y1][x1];
    board[y1][x1] = board[y2][x2];
    board[y2][x2] = temp;
}

bool are_tiles_adjacent(Vector2 a, Vector2 b){
    return (abs((int)a.x - (int)b.x) + abs((int)a.y - (int)b.y)) == 1; 
}

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

void init_board() {
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board[y][x] = random_tile();
        };
        
    };
    
    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
    grid_origin = (Vector2) {
        (GetScreenWidth() -  grid_width) / 2,
        (GetScreenHeight() - grid_height) / 2,
    };

    while (find_matches())
    {
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++)
                if (matched[y][x]) board[y][x] = random_tile();
    }
    memset(matched, 0, sizeof(matched));
    memset(score_popups, 0, sizeof(score_popups));
    score = 0;
    tile_state = STATE_IDLE;
};



int main(void)
{
    const int screen_width = 800;
    const int screen_hieght = 450;

    InitWindow(screen_width, screen_hieght, "Match Game");
    SetTargetFPS(60);
    srand(time(NULL));

    InitAudioDevice();

    background = LoadTexture(TextFormat("%sresources/background.png", GetApplicationDirectory()));
    score_font = LoadFontEx(TextFormat("%sresources/fonts/04b03.ttf", GetApplicationDirectory()), SCORE_FONT_SIZE, NULL, 0);
    background_music = LoadMusicStream(TextFormat("%sresources/audio/music.ogg", GetApplicationDirectory()));
    match_sound = LoadSound(TextFormat("%sresources/audio/match.wav", GetApplicationDirectory()));
    select_sound = LoadSound(TextFormat("%sresources/audio/select.wav", GetApplicationDirectory()));
    swap_sound = LoadSound(TextFormat("%sresources/audio/swap.wav", GetApplicationDirectory()));
    invalid_sound = LoadSound(TextFormat("%sresources/audio/invalid.wav", GetApplicationDirectory()));
    combo_sound = LoadSound(TextFormat("%sresources/audio/combo.wav", GetApplicationDirectory()));


    SetTextureFilter(score_font.texture, TEXTURE_FILTER_POINT);
    PlayMusicStream(background_music);
    init_board();
    Vector2 mouse = {0, 0};


    while (!WindowShouldClose())
    {
        UpdateMusicStream(background_music);
        mouse = GetMousePosition();
        if (tile_state == STATE_IDLE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int x = (int)floorf((mouse.x - grid_origin.x) / TILE_SIZE);
            int y = (int)floorf((mouse.y - grid_origin.y) / TILE_SIZE);
            if(x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                Vector2 curreent_tile = (Vector2) {x,y};
                if (selected_tile.x < 0)
                {
                    selected_tile = curreent_tile;
                    PlaySound(select_sound);
                } else {
                    if (are_tiles_adjacent(selected_tile ,curreent_tile))
                    {
                        swap_tiles(selected_tile.x, selected_tile.y, curreent_tile.x,curreent_tile.y);
                        PlaySound(swap_sound);
                        if (find_matches())
                        {
                            PlaySound(match_sound);
                            tile_state = STATE_MATCH_DELAY;
                            match_delay_timer = MATCH_DELAY_DURATION;
                        } else {
                            swap_tiles(selected_tile.x, selected_tile.y, curreent_tile.x,curreent_tile.y);
                            PlaySound(invalid_sound);
                        };
                        selected_tile = (Vector2) {-1, -1};
                    } else {
                        selected_tile = curreent_tile;
                        PlaySound(select_sound);
                    };
                };
            };  
        };

        if (tile_state == STATE_ANIMATING)
        {
            bool stil_animating = false;

            for (int y = 0; y < BOARD_SIZE; y++)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    if (fall_offset[y][x] > 0)
                    {
                        fall_offset[y][x] -= fall_speed * GetFrameTime();
                        if (fall_offset[y][x] < 0)
                        {
                            fall_offset[y][x] = 0;
                        } else {
                            stil_animating = true;
                        }
                        
                    }
                    
                }
                
            }

            if (!stil_animating)
            {
                if (find_matches())
                {
                    PlaySound(combo_sound);
                    tile_state = STATE_MATCH_DELAY;
                    match_delay_timer = MATCH_DELAY_DURATION;
                } else {
                    tile_state = STATE_IDLE;
                };
            }

        }

        if (tile_state == STATE_MATCH_DELAY)
        {
            match_delay_timer -= GetFrameTime();
            if (match_delay_timer <= 0.0f)
            {
                resolve_matches();
            }

        }
        
        // update score popups array 
        for (int i = 0; i < MAX_SCORE_POPUPS; i++)
        {
            if (score_popups[i].active)
            {
                score_popups[i].lifetime -= GetFrameTime();
                score_popups[i].position.y -= 30 * GetFrameTime();
                score_popups[i].alpha = score_popups[i].lifetime;

                if (score_popups[i].lifetime <= 0.0f)
                {
                    score_popups[i].active = false;
                }
                
            };
            
        };
        
        //update the score anmation
        if (score_animating)
        {
            score_scale += score_scale_velocity * GetFrameTime();
            if (score_scale <= 1.0f)
            {
                score_scale = 1.0f;
                score_animating = false;
            }
            
        }
        
        BeginDrawing();
        ClearBackground(BLACK); //RAYWHITE
        DrawTexturePro(
            background,
            (Rectangle) {
            0, 0, background.width, background.height
            },
            (Rectangle) {
                0, 0, GetScreenWidth(), GetScreenHeight(),
            },
            (Vector2) {0, 0},
            0.0f,
            WHITE
        );


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
         // drow selected tile
         if (selected_tile.x >= 0)
         {
            DrawRectangleLinesEx((Rectangle) {
                grid_origin.x + (selected_tile.x * TILE_SIZE),
                grid_origin.y + (selected_tile.y * TILE_SIZE),
                TILE_SIZE, TILE_SIZE
            }, 2, YELLOW);
         }
         
           DrawTextEx(
            score_font,
            TextFormat("SCORE: %d", score),
            (Vector2){20, 20}, SCORE_FONT_SIZE * score_scale, 1.0f, YELLOW
           );

        // dro score popups 
        for (int i = 0; i < MAX_SCORE_POPUPS; i++)
        {
            if (score_popups[i].active)
            {
                Color c = Fade(YELLOW, score_popups[i].alpha);
                DrawText(TextFormat("+%d", score_popups[i].amount), score_popups[i].position.x, score_popups[i].position.y, 20, c);
            }
            
        }
        
        EndDrawing();
    }
    
    StopMusicStream(background_music);
    UnloadMusicStream(background_music);
    UnloadSound(match_sound);
    UnloadSound(select_sound);
    UnloadSound(swap_sound);
    UnloadSound(invalid_sound);
    UnloadSound(combo_sound);
    UnloadTexture(background);
    UnloadFont(score_font);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
