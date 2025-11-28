#ifndef OS_WIN32_H
#define OS_WIN32_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct
{
	HWND window;
} OS_Win32BackendData;
OS_Win32BackendData os = {0};

#endif // OS_WIN32_H
