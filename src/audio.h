// Background music and sound effects

// Include guard: skip this file if it was already included (see board.h).
#ifndef AUDIO_H
#define AUDIO_H

// Needed for the Sound type.
#include <raylib.h>

// The sounds are "extern" so game.c can play them with PlaySound().
// They are defined (created) once, in audio.c.
// The music is not listed: only audio.c uses it, so it stays static there.
extern Sound match_sound;
extern Sound select_sound;
extern Sound swap_sound;
extern Sound invalid_sound;
extern Sound combo_sound;

void load_audio(void);
void update_audio(void);
void unload_audio(void);

#endif // AUDIO_H
