#include "os/os_window.h"
#include "os/os_input.h"
#include "os/os_gfx.h"

#include "gfx/gfx.h"
#include "sfx/sfx.h"

int main(int argc, char *argv[])
{
	os_window_init(800, 600, "from scratch");
	os_gfx_init();
	gfx_init();

	Arena images_arena = arena_create(GFX_ATLAS_SIZE * GFX_ATLAS_SIZE * 4);
	Gfx_Image images[] = {
		gfx_image_create(&images_arena, "res/player.png"),
	};
	gfx_sprite_atlas_init(images, 1);
	arena_destroy(&images_arena);

	gfx_font_init("res/font.ttf");

	Gfx_Camera camera = {0};
	camera.zoom = 1.0f;
	Vec2 pos = v2_zero();
	f32 current_time = 0.0f, delta_time = 0.0f, last_time = 0.0f;

    sfx_init();
    Sfx_Sound peace;
    sfx_sound_init(&peace, "res/peace.wav");
    sfx_sound_play(&peace);

	while (!os_window_should_close())
	{
		os_window_poll_events();
		current_time = os_get_time();
		delta_time = current_time - last_time;
		last_time = current_time;

		pos.x += (os_input_key_is_down(OS_KEY_D) - os_input_key_is_down(OS_KEY_A)) * 100.0f * delta_time;
		pos.y += (os_input_key_is_down(OS_KEY_S) - os_input_key_is_down(OS_KEY_W)) * 100.0f * delta_time;
		if (os_input_mouse_button_is_pressed(OS_MOUSE_BUTTON_LEFT))
			os_input_mouse_position_get(&pos.x, &pos.y);

		gfx_clear(v4(0.2f, 0.2f, 0.2f, 1.0f));
		gfx_resize();

		gfx_begin(&camera);
		gfx_draw_sprite(images[0], pos, v2(50.0f, 50.0f));
		gfx_draw_text("hello world!", v2(100.0f, 100.0f), 0.3f, v4_one());
		gfx_end();

		os_gfx_swap_buffers();
		os_input_update();
	}

    sfx_shutdown();
	os_window_shutdown();
	return 0;
}
