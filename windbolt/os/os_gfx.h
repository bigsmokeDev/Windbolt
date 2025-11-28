#ifndef OS_GFX_H
#define OS_GFX_H

#include "types.h"

typedef void *OS_ProcAddress;

global void os_gfx_init(void);
global void os_gfx_shutdown(void);
global void os_gfx_swap_buffers(void);

#endif // OS_GFX_H
