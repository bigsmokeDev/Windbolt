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

#endif // SFX_H
