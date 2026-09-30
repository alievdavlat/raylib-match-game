// Own header first (see board.c).
#include "audio.h"

// Definitions of the sounds that audio.h declares "extern".
Sound match_sound;
Sound select_sound;
Sound swap_sound;
Sound invalid_sound;
Sound combo_sound;

// Only this file touches the music, so it is private (static).
static Music background_music;

void load_audio(void) {
    InitAudioDevice();

    // Sound = short effect, loaded fully into memory, plays instantly.
    // Music = long track, streamed from disk piece by piece.
    background_music = LoadMusicStream(TextFormat("%sresources/audio/music.ogg", GetApplicationDirectory()));
    match_sound = LoadSound(TextFormat("%sresources/audio/match.wav", GetApplicationDirectory()));
    select_sound = LoadSound(TextFormat("%sresources/audio/select.wav", GetApplicationDirectory()));
    swap_sound = LoadSound(TextFormat("%sresources/audio/swap.wav", GetApplicationDirectory()));
    invalid_sound = LoadSound(TextFormat("%sresources/audio/invalid.wav", GetApplicationDirectory()));
    combo_sound = LoadSound(TextFormat("%sresources/audio/combo.wav", GetApplicationDirectory()));

    // Music loops by default.
    PlayMusicStream(background_music);
}

// Must be called every frame: it refills the stream buffer.
// Without it the music plays for a moment and then stops.
void update_audio(void) {
    UpdateMusicStream(background_music);
}

void unload_audio(void) {
    StopMusicStream(background_music);
    UnloadMusicStream(background_music);
    UnloadSound(match_sound);
    UnloadSound(select_sound);
    UnloadSound(swap_sound);
    UnloadSound(invalid_sound);
    UnloadSound(combo_sound);
    CloseAudioDevice();
}
