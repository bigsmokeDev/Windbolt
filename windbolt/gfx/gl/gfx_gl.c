#include "base.h"
#include "os/os_window.h"
#include "gfx/gfx.h"
#include "shader_gl.h"

#include <stddef.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include "vendor/stb/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "vendor/stb/stb_truetype.h"

///////////////
// 2d renderer
local struct
{
	u32 shd;
	u32 vbo, ibo, vao;
	s32 proj_loc;

	Gfx_Quad quads[GFX_MAX_QUADS];
	u32 quad_count;

	stbtt_bakedchar chardata[96];
} gfx_data = {0};

void gfx_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	gfx_data.shd = gl_shader_create(vertex_shader_source, fragment_shader_source);
	glUseProgram(gfx_data.shd);

	gfx_data.proj_loc = glGetUniformLocation(gfx_data.shd, "u_proj");
	glUniform1i(glGetUniformLocation(gfx_data.shd, "u_smp"), 0);
	glUniform1i(glGetUniformLocation(gfx_data.shd, "u_font_smp"), 1);

	glGenVertexArrays(1, &gfx_data.vao);
	glBindVertexArray(gfx_data.vao);

	glGenBuffers(1, &gfx_data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gfx_data.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Gfx_Vertex) * GFX_MAX_VERTICES, 0, GL_DYNAMIC_DRAW);

	u16 indices[GFX_MAX_QUADS * 6];
	for (u32 i = 0; i < GFX_MAX_QUADS; ++i)
	{
		indices[i * 6] = i * 4;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 3;
		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
	}
	glGenBuffers(1, &gfx_data.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx_data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Gfx_Vertex), (void*)offsetof(Gfx_Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Gfx_Vertex), (void*)offsetof(Gfx_Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Gfx_Vertex), (void*)offsetof(Gfx_Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Gfx_Vertex), (void*)offsetof(Gfx_Vertex, tex_id));
	glEnableVertexAttribArray(3);
}

void gfx_shutdown(void)
{
	glDeleteBuffers(1, &gfx_data.vbo);
	glDeleteBuffers(1, &gfx_data.ibo);
	glDeleteVertexArrays(1, &gfx_data.vao);
	glDeleteShader(gfx_data.shd);
}

void gfx_clear(Vec4 clear_color)
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_resize(void)
{
	u32 width, height;
	os_window_get_size(&width, &height);
	glViewport(0, 0, width, height);
}

void gfx_begin(Gfx_Camera *camera)
{
	gfx_data.quad_count = 0;

	u32 width, height;
	os_window_get_size(&width, &height);

	camera->proj = mat4_ortho(0.0f, (f32)width, 0.0f, (f32)height, -1.0f, 1.0f);
	camera->proj = mat4_scale(camera->proj, v2(camera->zoom, camera->zoom));
	camera->proj = mat4_rotate_z(camera->proj, camera->rotation);
	camera->proj = mat4_translate(camera->proj, v2(-camera->position.x, -camera->position.y));
	glUniformMatrix4fv(gfx_data.proj_loc, 1, GL_FALSE, &camera->proj.m0);
}

void gfx_end(void)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Gfx_Quad) * gfx_data.quad_count, gfx_data.quads);
	glDrawElements(GL_TRIANGLES, gfx_data.quad_count * 6, GL_UNSIGNED_SHORT, 0);
}

void gfx_init_sprite_atlas(Gfx_Image images[])
{
	const u32 pixel_count = GFX_ATLAS_SIZE * GFX_ATLAS_SIZE * 4;
	const u32 node_count = GFX_ATLAS_SIZE;

	// NOTE(smoke): im getting cancelled for this one
	u32 image_count = 0;
	for (u32 i = 0; images[i].pixels; i++)
	{
		image_count++;
	}

	Arena atlas_arena = arena_create(pixel_count + (sizeof(stbrp_node) * node_count) + (sizeof(stbrp_rect) * image_count));
	arena_reset(&atlas_arena);

	u8 *atlas_pixels = arena_alloc(&atlas_arena, pixel_count);

	stbrp_context ctx;
	stbrp_node *nodes = arena_alloc(&atlas_arena, sizeof(*nodes) * node_count);
	stbrp_init_target(&ctx, GFX_ATLAS_SIZE, GFX_ATLAS_SIZE, nodes, image_count);

	stbrp_rect *rects = arena_alloc(&atlas_arena, sizeof(*nodes) * image_count);
	for (u32 i = 0; i < image_count; ++i)
	{
		ASSERT(!rects[i].was_packed, "atlas size not enough");
		rects[i].id = i;
		rects[i].w = images[i].width;
		rects[i].h = images[i].height;
	}
	stbrp_pack_rects(&ctx, rects, image_count);

	for (u32 i = 0; i < image_count; ++i)
	{
		stbrp_rect r = rects[i];
		if (!r.was_packed)
			continue;

		u8 *src = images[i].pixels;
		for (u32 y = 0; y < r.h; y++)
		{
			u8 *dst_row = atlas_pixels + ((r.y + y) * GFX_ATLAS_SIZE + r.x) * 4;
			u8 *src_row = src + (y * images[i].width) * 4;
			memcpy(dst_row, src_row, r.w * 4);
		}

		images[i].uv.x = (f32)r.x / (f32)GFX_ATLAS_SIZE;
		images[i].uv.y = (f32)r.y / (f32)GFX_ATLAS_SIZE;
		images[i].uv.z = (f32)r.w / (f32)GFX_ATLAS_SIZE;
		images[i].uv.w = (f32)r.h / (f32)GFX_ATLAS_SIZE;
	}

	glActiveTexture(GL_TEXTURE0);
	u32 gl_texture;
	glGenTextures(1, &gl_texture);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GFX_ATLAS_SIZE, GFX_ATLAS_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas_pixels);

	arena_destroy(&atlas_arena);
}

void gfx_init_font(const char *font_path)
{
	Arena font_file_arena = arena_create((1<<20) + (1024 * 1024));
	u8 *buffer = arena_alloc(&font_file_arena, 1<<20);
	fread(buffer, 1, 1<<20, fopen(font_path, "rb"));
	u8 *bitmap = arena_alloc(&font_file_arena, 1024 * 1024);

	ASSERT(stbtt_BakeFontBitmap(buffer, 0, 128, bitmap, 1024, 1024, 32, 96, gfx_data.chardata) > 0, font_path);

	glActiveTexture(GL_TEXTURE1);
	u32 gl_texture;
	glGenTextures(1, &gl_texture);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	arena_destroy(&font_file_arena);
}

void gfx_draw_rect(Vec2 position, Vec2 size, Vec4 color)
{
	ASSERT(gfx_data.quad_count < GFX_MAX_QUADS, "ran out of quads");

	gfx_data.quads[gfx_data.quad_count][0] = (Gfx_Vertex){ position, color, v2_zero(), 0.0f };
	gfx_data.quads[gfx_data.quad_count][1] = (Gfx_Vertex){ { position.x + size.x, position.y }, color, v2_zero(), 0.0f };
	gfx_data.quads[gfx_data.quad_count][2] = (Gfx_Vertex){ { position.x + size.x, position.y + size.y }, color, v2_zero(), 0.0f };
	gfx_data.quads[gfx_data.quad_count][3] = (Gfx_Vertex){ { position.x, position.y + size.y }, color, v2_zero(), 0.0f };
	gfx_data.quad_count++;
}

void gfx_draw_sprite(Gfx_Image image, Vec2 position, Vec2 size)
{
	ASSERT(gfx_data.quad_count < GFX_MAX_QUADS, "ran out of quads");

	gfx_data.quads[gfx_data.quad_count][0] = (Gfx_Vertex){ position, v4_one(), { image.uv.x, image.uv.y }, 0.0f };
	gfx_data.quads[gfx_data.quad_count][1] = (Gfx_Vertex){ { position.x + size.x, position.y }, v4_one(), { image.uv.x + image.uv.z, image.uv.y }, 0.0f };
	gfx_data.quads[gfx_data.quad_count][2] = (Gfx_Vertex){ { position.x + size.x, position.y + size.y }, v4_one(), { image.uv.x + image.uv.z, image.uv.y + image.uv.w }, 0.0f };
	gfx_data.quads[gfx_data.quad_count][3] = (Gfx_Vertex){ { position.x, position.y + size.y }, v4_one(), { image.uv.x, image.uv.y + image.uv.w }, 0.0f };
	gfx_data.quad_count++;
}

void gfx_draw_text(const char *text, Vec2 position, f32 scale, Vec4 color)
{
	const float inv_width = 1.0f / 1024.0f;
    const float inv_height = 1.0f / 1024.0f;

	for (u32 i = 0; text[i] != '\0'; i++)
	{
		ASSERT(gfx_data.quad_count < GFX_MAX_QUADS, "ran out of quads");

		stbtt_bakedchar *b = &gfx_data.chardata[text[i] - 32];

		float x0 = position.x + b->xoff * scale;
		float y0 = position.y + b->yoff * scale;
		float w  = (b->x1 - b->x0) * scale;
		float h  = (b->y1 - b->y0) * scale;

		float x1 = x0 + w;
		float y1 = y0 + h;

		float s0 = (b->x0) * inv_width;
		float t0 = b->y0 * inv_height;
		float s1 = (b->x1) * inv_width;
		float t1 = b->y1 * inv_height;

		gfx_data.quads[gfx_data.quad_count][0] = (Gfx_Vertex){ v2(x0, y0), color, { s0, t0 }, 1.0f };
		gfx_data.quads[gfx_data.quad_count][1] = (Gfx_Vertex){ v2(x1, y0), color, { s1, t0 }, 1.0f };
		gfx_data.quads[gfx_data.quad_count][2] = (Gfx_Vertex){ v2(x1, y1), color, { s1, t1 }, 1.0f };
		gfx_data.quads[gfx_data.quad_count][3] = (Gfx_Vertex){ v2(x0, y1), color, { s0, t1 }, 1.0f };
		gfx_data.quad_count++;

		position.x += b->xadvance * scale;
	}
}
