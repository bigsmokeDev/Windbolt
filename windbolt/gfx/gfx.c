#include "gfx.h"

// NOTE(smoke): graphics API independent code goes here

#include "vendor/stb/stb_image.h"

/////////
// image
Gfx_Image gfx_image_create(Arena *arena, const char *image_path)
{
	Gfx_Image image = {0};

	s32 width, height, channels;
	u8 *pixels = stbi_load(image_path, &width, &height, &channels, 4);
	ASSERT(pixels, "cannot find image");

	image.pixels = arena_alloc(arena, width * height * channels);
	memcpy(image.pixels, pixels, width * height * channels);

	stbi_image_free(pixels);

	image.width = width;
	image.height = height;

	return image;
}
