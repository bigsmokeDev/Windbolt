#include "base.h"
#include "sfx.h"

local ma_engine engine;

void sfx_init(void)
{
    ma_result result = ma_engine_init(0, &engine);
    ASSERT(result == MA_SUCCESS, "failed to init miniaudio, ma_result: %d\n", result);
}

void sfx_shutdown(void)
{
    ma_engine_uninit(&engine);
}

void sfx_sound_init(Sfx_Sound *sound, const char *sound_path)
{
    ma_result result = ma_sound_init_from_file(&engine, sound_path, 0, 0, 0, sound);
    ASSERT(result == MA_SUCCESS, "failed to load %s, ma_result: %d\n", sound_path, result);
}

void sfx_sound_destroy(Sfx_Sound *sound)
{
    ma_sound_uninit(sound);
}

void sfx_sound_play(Sfx_Sound *sound)
{
    ma_sound_start(sound);
}
