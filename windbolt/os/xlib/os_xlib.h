#ifndef OS_XLIB_H
#define OS_XLIB_H

#include "types.h"
#include "os/os_input.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct
{
    Display *dpy;
    s32 screen;
    Window window, root_window;
    Colormap colormap;
    Visual *visual;
    Atom wm_delete_window;
} OS_XlibBackendData;
OS_XlibBackendData os = {0};

global Visual *os_xlib_get_glx_visual(void);
global OS_InputKey os_input_xkey_to_key(XKeyEvent *event);

#endif // OS_XLIB_H
