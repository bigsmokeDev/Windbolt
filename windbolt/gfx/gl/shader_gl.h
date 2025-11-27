#ifndef SHADER_GL_H
#define SHADER_GL_H

const char *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec2 a_pos;"
"layout (location = 1) in vec4 a_color;"
"layout (location = 2) in vec2 a_uv;"
"layout (location = 3) in float a_texid;"
"out vec4 color;"
"out vec2 uv;"
"out float texid;"
"uniform mat4 u_proj;"
"void main() {"
	"gl_Position = u_proj * vec4(a_pos, 0.0, 1.0);"
	"color = a_color;"
	"uv = a_uv;"
	"texid = a_texid;"
"}";
const char *fragment_shader_source = "#version 330 core\n"
"out vec4 out_color;"
"in vec4 color;"
"in vec2 uv;"
"in float texid;"
"uniform sampler2D u_smp, u_font_smp;"
"void main() {"
	"if (texid == 0.0) {"
		"out_color = texture(u_smp, uv) * color;"
	"} else if (texid == 1.0) {"
		"out_color = vec4(color.xyz, texture(u_font_smp, uv).r);"
	"}"
"}";

#include "vendor/glad/glad.h"
#include <stdio.h>

unsigned int gl_shader_create(const char *vs_src, const char *fs_src)
{
	unsigned int vs, fs, shd;
	int success;
	char info_log[2048];

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_src, 0);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 2048, 0, info_log);
		printf("vertex shader error: %s\n", info_log);
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_src, 0);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 2048, 0, info_log);
		printf("fragment shader error: %s\n", info_log);
	}

	shd = glCreateProgram();
	glAttachShader(shd, vs);
	glAttachShader(shd, fs);
	glLinkProgram(shd);
	glGetProgramiv(shd, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shd, 2048, 0, info_log);
		printf("shader linking error: %s\n", info_log);
	}

	return shd;
}

#endif // SHADER_GL_H
