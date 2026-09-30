// Background music and sound effects
#ifndef AUDIO_H
#define AUDIO_H

#include <raylib.h>

extern Sound match_sound;
extern Sound select_sound;
extern Sound swap_sound;
extern Sound invalid_sound;
extern Sound combo_sound;

void load_audio(void);
void update_audio(void);
void unload_audio(void);

#endif
