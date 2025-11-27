#include "os/os_window.h"
#include "os/os_input.h"
#include "os_xlib.h"
#include "base.h"

#include <time.h>

local bool should_close = false;
local u32 window_width, window_height;

local f64 clock_freq = 0.000000001;
local struct timespec start_time;

void os_window_init(u32 width, u32 height, const char *title)
{
	os.dpy = XOpenDisplay(NULL);
	ASSERT(os.dpy, "XOpenDisplay failure");
	os.screen = DefaultScreen(os.dpy);
	os.root_window = RootWindow(os.dpy, os.screen);
	os.visual = os_xlib_get_glx_visual();
	os.colormap = XCreateColormap(os.dpy, os.root_window, os.visual, AllocNone);
	
	XSetWindowAttributes attributes = {0};
    attributes.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

	os.window = XCreateWindow(os.dpy, os.root_window, 0, 0, width, height,
			0, CopyFromParent, InputOutput, CopyFromParent,
			CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &attributes);
	ASSERT(os.window, "XCreateWindow failure");
	XStoreName(os.dpy, os.window, title);

	XMapWindow(os.dpy, os.window);
	XFlush(os.dpy);

	os.wm_delete_window = XInternAtom(os.dpy, "WM_DELETE_WINDOW", False);
	ASSERT(XSetWMProtocols(os.dpy, os.window, &os.wm_delete_window, 1), "couldn't register WM_DELETE_WINDOW property");

	clock_gettime(CLOCK_MONOTONIC, &start_time);

	window_width = width;
	window_height = height;
}

void os_window_shutdown(void)
{
	XDestroyWindow(os.dpy, os.window);
	XCloseDisplay(os.dpy);
}

bool os_window_should_close(void)
{
	return should_close;
}

void os_window_poll_events(void)
{
	XEvent event;
	OS_InputState *input_state = os_input_get_state();
	while (XPending(os.dpy) > 0)
	{
		XNextEvent(os.dpy, &event);
		switch (event.type)
		{
			case ClientMessage:
			{
				XClientMessageEvent *ev = (XClientMessageEvent*)&event;
				if ((Atom)ev->data.l[0] == os.wm_delete_window)
					should_close = true;
			} break;

			case ConfigureNotify:
			{
				XConfigureEvent *ev = (XConfigureEvent*)&event;
				if (ev->width != window_width || ev->height != window_height)
				{
					window_width = ev->width;
					window_height = ev->height;
				}
			} break;

			case KeyPress:
			{
				OS_InputKey key = os_input_xkey_to_key(&event.xkey);
				input_state->keys[key] = true;
			} break;
			case KeyRelease:
			{
				OS_InputKey key = os_input_xkey_to_key(&event.xkey);
				input_state->keys[key] = false;
			} break;

            case ButtonPress:
            case ButtonRelease:
            {
                bool pressed = (event.type == ButtonPress);
                OS_InputMouseButton mouse_button = OS_MAX_MOUSE_BUTTONS;
                
                switch (event.xbutton.button)
                {
                    case Button1:
                    {
                        mouse_button = OS_MOUSE_BUTTON_LEFT;
                    }
                    break;
                    case Button2:
                    {
                        mouse_button = OS_MOUSE_BUTTON_MIDDLE;
                    }
                    break;
                    case Button3:
                    {
                        mouse_button = OS_MOUSE_BUTTON_RIGHT;
                    }
                    break;
                }
                
				input_state->mouse_buttons[mouse_button] = pressed;
            }
            break;
            case MotionNotify:
            {
                input_state->mouse_x = event.xmotion.x;
                input_state->mouse_y = event.xmotion.y;
            }
            break;

			default: break;
		}
	}
}

void os_window_get_size(u32 *width, u32 *height)
{
	*width = window_width;
	*height = window_height;
}

f64 os_get_time(void)
{
	struct timespec now_time;
    clock_gettime(CLOCK_MONOTONIC, &now_time);
    return (f64)(now_time.tv_sec - start_time.tv_sec) + (now_time.tv_nsec - start_time.tv_nsec) * clock_freq;
}
