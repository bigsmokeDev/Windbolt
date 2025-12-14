#ifndef SFX_H
#define SFX_H

#include "types.h"

#include "vendor/miniaudio/miniaudio.h"

global void sfx_init(void);
global void sfx_shutdown(void);

typedef ma_sound Sfx_Sound;

global void sfx_sound_init(Sfx_Sound *sound, const char *sound_path);
global void sfx_sound_destroy(Sfx_Sound *sound);
global void sfx_sound_play(Sfx_Sound *sound);

#define sfx_sound_set_volume(sound, volume) ma_sound_set_volume(sound, volume)
#define sfx_sound_set_pan(sound, pan) ma_sound_set_pan(sound, pan)
#define sfx_sound_set_position(sound, position) ma_sound_set_position(sound, position.x, position.y, 0.0f)

#endif // SFX_H
