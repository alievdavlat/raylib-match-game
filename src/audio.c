#include "audio.h"

Sound match_sound;
Sound select_sound;
Sound swap_sound;
Sound invalid_sound;
Sound combo_sound;

static Music background_music;

void load_audio(void) {
    InitAudioDevice();

    background_music = LoadMusicStream(TextFormat("%sresources/audio/music.ogg", GetApplicationDirectory()));
    match_sound = LoadSound(TextFormat("%sresources/audio/match.wav", GetApplicationDirectory()));
    select_sound = LoadSound(TextFormat("%sresources/audio/select.wav", GetApplicationDirectory()));
    swap_sound = LoadSound(TextFormat("%sresources/audio/swap.wav", GetApplicationDirectory()));
    invalid_sound = LoadSound(TextFormat("%sresources/audio/invalid.wav", GetApplicationDirectory()));
    combo_sound = LoadSound(TextFormat("%sresources/audio/combo.wav", GetApplicationDirectory()));

    PlayMusicStream(background_music);
}

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
