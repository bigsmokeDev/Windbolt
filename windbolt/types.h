#ifndef TYPES_H
#define TYPES_H

///////////////
// basic types
typedef char s8;
typedef short s16;
typedef int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

// NOTE(smoke): thank you, clang...
#if defined(__clang__)
typedef long s64;
typedef unsigned long u64;
#else
typedef long long s64;
typedef unsigned long long u64;
#endif

////////////
// booleans
typedef u8 bool;
#define false (u8)0
#define true (u8)1

/////////////
// functions
#define global
#define local  static

#endif // TYPES_H
