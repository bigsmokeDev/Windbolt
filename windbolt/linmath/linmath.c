#include "linmath.h"
#include <math.h>

////////////
// vector 2
Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return v2(a.x + b.x, a.y + b.y);
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
    return v2(a.x - b.x, a.y - b.y);
}

Vec2 vec2_scale(Vec2 vec, f32 scale)
{
    return v2(vec.x * scale, vec.y * scale);
}

Vec2 vec2_mul(Vec2 a, Vec2 b)
{
    return v2(a.x * b.x, a.y * b.y);
}

Vec2 vec2_div(Vec2 a, Vec2 b)
{
    return v2(a.x / b.x, a.y / b.y);
}

f32 vec2_len(Vec2 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

Vec2 vec2_norm(Vec2 vec)
{
    const f32 length = vec2_len(vec);
    return v2(vec.x / length, vec.y / length);
}

f32 vec2_dot(Vec2 a, Vec2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

////////////
// vector 2
Vec4 vec4_add(Vec4 a, Vec4 b)
{
    return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4 vec4_sub(Vec4 a, Vec4 b)
{
    return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vec4 vec4_scale(Vec4 vec, f32 scale)
{
    return v4(vec.x * scale, vec.y * scale, vec.z * scale, vec.w * scale);
}

Vec4 vec4_mul(Vec4 a, Vec4 b)
{
    return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Vec4 vec4_div(Vec4 a, Vec4 b)
{
    return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

f32 vec4_len(Vec4 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

Vec4 vec4_norm(Vec4 vec)
{
    const f32 length = vec4_len(vec);
    return v4(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
}

f32 vec4_dot(Vec4 a, Vec4 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vec4 vec4_mul_mat4(Vec4 vec, Mat4 mat)
{
    Vec4 result;
    result.x = mat.m0*vec.x + mat.m4*vec.y + mat.m8*vec.z + mat.m12*vec.w;
    result.y = mat.m1*vec.x + mat.m5*vec.y + mat.m9*vec.z + mat.m13*vec.w;
    result.z = mat.m2*vec.x + mat.m6*vec.y + mat.m10*vec.z + mat.m14*vec.w;
    result.w = mat.m3*vec.x + mat.m7*vec.y + mat.m11*vec.z + mat.m15*vec.w;
    return result;
}

//////////////
// 4x4 matrix
Mat4 mat4_identity(void)
{
    return (Mat4){
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

Mat4 mat4_add(Mat4 a, Mat4 b)
{
    return (Mat4){
        a.m0 + b.m0, a.m1 + b.m1, a.m2 + b.m2, a.m3 + b.m3,
        a.m4 + b.m4, a.m5 + b.m5, a.m6 + b.m6, a.m7 + b.m7,
        a.m8 + b.m8, a.m9 + b.m9, a.m10 + b.m10, a.m11 + b.m11,
        a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m15 + b.m15,
    };
}

Mat4 mat4_sub(Mat4 a, Mat4 b)
{
    return (Mat4){
        a.m0 - b.m0, a.m1 - b.m1, a.m2 - b.m2, a.m3 - b.m3,
        a.m4 - b.m4, a.m5 - b.m5, a.m6 - b.m6, a.m7 - b.m7,
        a.m8 - b.m8, a.m9 - b.m9, a.m10 - b.m10, a.m11 - b.m11,
        a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m15 - b.m15,
    };
}

Mat4 mat4_scalar(Mat4 mat, f32 scale)
{
    return (Mat4){
        mat.m0 * scale, mat.m1 * scale, mat.m2 * scale, mat.m3 * scale,
        mat.m4 * scale, mat.m5 * scale, mat.m6 * scale, mat.m7 * scale,
        mat.m8 * scale, mat.m9 * scale, mat.m10 * scale, mat.m11 * scale,
        mat.m12 * scale, mat.m13 * scale, mat.m14 * scale, mat.m15 * scale,
    };
}

// NOTE(smoke): god is dead
Mat4 mat4_mul(Mat4 a, Mat4 b)
{
    Mat4 result = {0};

    result.m0 = a.m0*b.m0 + a.m1*b.m4 + a.m2*b.m8 + a.m3*b.m12;
    result.m1 = a.m0*b.m1 + a.m1*b.m5 + a.m2*b.m9 + a.m3*b.m13;
    result.m2 = a.m0*b.m2 + a.m1*b.m6 + a.m2*b.m10 + a.m3*b.m14;
    result.m3 = a.m0*b.m3 + a.m1*b.m7 + a.m2*b.m11 + a.m3*b.m15;
    result.m4 = a.m4*b.m0 + a.m5*b.m4 + a.m6*b.m8 + a.m7*b.m12;
    result.m5 = a.m4*b.m1 + a.m5*b.m5 + a.m6*b.m9 + a.m7*b.m13;
    result.m6 = a.m4*b.m2 + a.m5*b.m6 + a.m6*b.m10 + a.m7*b.m14;
    result.m7 = a.m4*b.m3 + a.m5*b.m7 + a.m6*b.m11 + a.m7*b.m15;
    result.m8 = a.m8*b.m0 + a.m9*b.m4 + a.m10*b.m8 + a.m11*b.m12;
    result.m9 = a.m8*b.m1 + a.m9*b.m5 + a.m10*b.m9 + a.m11*b.m13;
    result.m10 = a.m8*b.m2 + a.m9*b.m6 + a.m10*b.m10 + a.m11*b.m14;
    result.m11 = a.m8*b.m3 + a.m9*b.m7 + a.m10*b.m11 + a.m11*b.m15;
    result.m12 = a.m12*b.m0 + a.m13*b.m4 + a.m14*b.m8 + a.m15*b.m12;
    result.m13 = a.m12*b.m1 + a.m13*b.m5 + a.m14*b.m9 + a.m15*b.m13;
    result.m14 = a.m12*b.m2 + a.m13*b.m6 + a.m14*b.m10 + a.m15*b.m14;
    result.m15 = a.m12*b.m3 + a.m13*b.m7 + a.m14*b.m11 + a.m15*b.m15;

    return result;
}

Mat4 mat4_scale(Mat4 mat, Vec2 vec)
{
    Mat4 result = mat4_identity();
    result.m0 = vec.x;
    result.m5 = vec.y;
    return mat4_mul(mat, result);
}

Mat4 mat4_translate(Mat4 mat, Vec2 vec)
{
    Mat4 result = mat4_identity();
    result.m12 = vec.x;
    result.m13 = vec.y;
    result.m14 = 0.0f;
    return mat4_mul(mat, result);
}

Mat4 mat4_rotate_z(Mat4 mat, f32 angle)
{
    const f32 cosine = cosf(angle);
    const f32 sine = sinf(angle);
    Mat4 result = mat4_identity();
    result.m0 = cosine;
    result.m4 = -sine;
    result.m1 = sine;
    result.m5 = cosine;
    return mat4_mul(mat, result);
}

Mat4 mat4_ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near_plane, f32 far_plane)
{
    Mat4 result = {0};

    const f32 rl = right - left;
    const f32 tb = top - bottom;
    const f32 fn = far_plane - near_plane;

    result.m0 = 2.0f / rl;
    result.m5 = 2.0f / tb;
    result.m10 = -2.0f / fn;
    result.m12 = -(left + right) / rl;
    result.m13 = -(top + bottom) / tb;
    result.m14 = -(far_plane + near_plane) / fn;
    result.m15 = 1.0f;

    return result;
}
