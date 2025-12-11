#ifndef GFX_H
#define GFX_H

// NOTE(smoke): simple 2D renderer

#include "types.h"
#include "ds/arena.h"
#include "linmath/linmath.h"

/////////
// image
typedef struct
{
    u8 *pixels;
    u32 width, height;
    Vec4 uv; // NOTE(smoke): for sprite atlas
} Gfx_Image;

global Gfx_Image gfx_image_create(Arena *arena, const char *image_path);

///////////////
// 2d renderer
typedef struct
{
    Vec2 position;
    Vec4 color;
    Vec2 uv;
    f32 tex_id;
} Gfx_Vertex;
typedef Gfx_Vertex Gfx_Quad[4];

#define GFX_MAX_QUADS 8192
#define GFX_MAX_VERTICES GFX_MAX_QUADS * 4
#define GFX_ATLAS_SIZE 2048

typedef struct
{
    Mat4 proj;
    Vec2 position;
    f32 zoom, rotation;
} Gfx_Camera;

global void gfx_init(void);
global void gfx_shutdown(void);

global void gfx_clear(Vec4 clear_color);
global void gfx_resize(void);
global void gfx_begin(Gfx_Camera *camera);
global void gfx_end(void);

global void gfx_sprite_atlas_init(Gfx_Image images[], u32 image_count);
global void gfx_font_init(const char *font_path);

global void gfx_draw_rect(Vec2 position, Vec2 size, Vec4 color);
global void gfx_draw_sprite(Gfx_Image image, Vec2 position, Vec2 size);
global void gfx_draw_text(const char *text, Vec2 position, f32 scale, Vec4 color);
global void gfx_draw_rect_ex(Vec2 position, Vec2 size, Vec4 color, f32 rotation, Vec2 origin);
global void gfx_draw_sprite_ex(Gfx_Image image, Vec2 position, Vec2 size, f32 rotation, Vec2 origin);
// NOTE(smoke): rotating text?

#endif // GFX_H
