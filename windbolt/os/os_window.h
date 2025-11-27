#ifndef OS_WINDOW_H
#define OS_WINDOW_H

#include "types.h"

// NOTE(smoke): we only need 1 window for the game,
// so no OS_Window struct.

global void os_window_init(u32 width, u32 height, const char *title);
global void os_window_shutdown(void);

global bool os_window_should_close(void);
global void os_window_poll_events(void);
global void os_window_get_size(u32 *width, u32 *height);
global f64  os_get_time(void);

#endif // OS_WINDOW_H
