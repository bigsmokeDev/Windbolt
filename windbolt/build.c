//////////////////
// context macros
#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
#define OS_LINUX 1
#endif

//////
// os
#include "os/os_input.c"

#if OS_LINUX
#include "os/xlib/os_xlib_window.c"
#include "os/xlib/os_xlib_keys.c"
#include "os/xlib/os_xlib_gl.c"
#elif OS_WINDOWS
#include "os/win32/os_win32_window.c"
#include "os/win32/os_win32_gl.c"
#endif

//////
// ds
#include "ds/arena.c"

////////
// math
#include "linmath/linmath.c"

///////
// gfx
#include "gfx/gfx.c"
#include "gfx/gl/gfx_gl.c"

/////////
// audio
