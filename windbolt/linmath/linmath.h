#ifndef LINMATH_H
#define LINMATH_H

// NOTE(smoke): small math library for 2D games

#include "types.h"

////////////
// vector 2
typedef struct
{
    f32 x, y;
} Vec2;
#define v2(x,y) (Vec2){ x, y }
#define v2_zero() (Vec2){ 0 }
#define v2_one() (Vec2){ 1.0f, 1.0f }

global Vec2 vec2_add(Vec2 a, Vec2 b);
global Vec2 vec2_sub(Vec2 a, Vec2 b);
global Vec2 vec2_scale(Vec2 vec, f32 scale);
global Vec2 vec2_mul(Vec2 a, Vec2 b);
global Vec2 vec2_div(Vec2 a, Vec2 b);
global f32  vec2_len(Vec2 vec);
global Vec2 vec2_norm(Vec2 vec);
global f32  vec2_dot(Vec2 a, Vec2 b);
global Vec2 vec2_lerp(Vec2 start, Vec2 end, f32 amount);

////////////
// vector 4
typedef struct
{
    f32 x, y, z, w;
} Vec4;
#define v4(x,y,z,w) (Vec4){ x, y, z, w }
#define v4_zero() (Vec4){ 0 }
#define v4_one() (Vec4){ 1.0f, 1.0f, 1.0f, 1.0f }

global Vec4 vec4_add(Vec4 a, Vec4 b);
global Vec4 vec4_sub(Vec4 a, Vec4 b);
global Vec4 vec4_scale(Vec4 vec, f32 scale);
global Vec4 vec4_mul(Vec4 a, Vec4 b);
global Vec4 vec4_div(Vec4 a, Vec4 b);
global f32  vec4_len(Vec4 vec);
global Vec4 vec4_norm(Vec4 vec);
global f32  vec4_dot(Vec4 a, Vec4 b);
global Vec4 vec4_lerp(Vec4 start, Vec4 end, f32 amount);

//////////////
// 4x4 matrix
typedef struct
{
    float m0,  m1,  m2,  m3;
    float m4,  m5,  m6,  m7;
    float m8,  m9,  m10, m11;
    float m12, m13, m14, m15;
} Mat4;

global Mat4 mat4_identity(void);
global Mat4 mat4_add(Mat4 a, Mat4 b);
global Mat4 mat4_sub(Mat4 a, Mat4 b);
global Mat4 mat4_scalar(Mat4 mat, f32 scale);
global Mat4 mat4_mul(Mat4 a, Mat4 b);
global Mat4 mat4_scale(Mat4 mat, Vec2 vec);
global Mat4 mat4_translate(Mat4 mat, Vec2 vec);
global Mat4 mat4_rotate_z(Mat4 mat, f32 angle);

global Mat4 mat4_ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near_plane, f32 far_plane);
global Vec4 vec4_mul_mat4(Vec4 vec, Mat4 mat);

#endif // LINMATH_H
