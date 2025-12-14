#include "base.h"
#include "os_win32.h"
#include "os/os_window.h"
#include "os/os_input.h"

#include <windowsx.h>

local bool should_close = false;
local u32 window_width, window_height;

local f32 clock_freq;
local LARGE_INTEGER start_time;

local LRESULT CALLBACK win32_window_proc(
    HWND window,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
)
{
    OS_InputState *input_state = os_input_get_state();

    switch (msg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;

        case WM_CLOSE:
        {
            should_close = true;
        } break;

        case WM_SIZE:
        {
            window_width = LOWORD(lParam);
            window_height = HIWORD(lParam);
        } break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            input_state->keys[wParam] = true;
            return 0;
        } break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            input_state->keys[wParam] = false;
            return 0;
        } break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            OS_InputMouseButton mouse_button = OS_MAX_MOUSE_BUTTONS;
            switch (msg)
            {
                case WM_LBUTTONDOWN:
                {
                    mouse_button = OS_MOUSE_BUTTON_LEFT;
                } break;
                case WM_MBUTTONDOWN:
                {
                    mouse_button = OS_MOUSE_BUTTON_MIDDLE;
                } break;
                case WM_RBUTTONDOWN:
                {
                    mouse_button = OS_MOUSE_BUTTON_RIGHT;
                } break;
                default: break;
            }
            input_state->mouse_buttons[mouse_button] = true;
            return 0;
        } break;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            OS_InputMouseButton mouse_button = OS_MAX_MOUSE_BUTTONS;
            switch (msg)
            {
                case WM_LBUTTONUP:
                {
                    mouse_button = OS_MOUSE_BUTTON_LEFT;
                } break;
                case WM_MBUTTONUP:
                {
                    mouse_button = OS_MOUSE_BUTTON_MIDDLE;
                } break;
                case WM_RBUTTONUP:
                {
                    mouse_button = OS_MOUSE_BUTTON_RIGHT;
                } break;
            }
            input_state->mouse_buttons[mouse_button] = false;
            return 0;
        } break;

        case WM_MOUSEMOVE:
        {
            input_state->mouse_x = GET_X_LPARAM(lParam);
            input_state->mouse_y = GET_Y_LPARAM(lParam);
        } break;

        default: break;
    }

    return DefWindowProc(window, msg, wParam, lParam);
}

void os_window_init(u32 width, u32 height, const char *title)
{
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASS window_class = {0};
    window_class.lpfnWndProc = win32_window_proc;
    window_class.hInstance = instance;
    window_class.lpszClassName = "WindboltWindowClass";

    ASSERT(RegisterClass(&window_class), "failed to register window class");

    os.window =
        CreateWindowEx(
            0,
            window_class.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            0, 0, instance, 0);
    ASSERT(os.window, "failed to create win32 window");

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clock_freq= 1.0f / (f32)freq.QuadPart;
    QueryPerformanceCounter(&start_time);

    window_width = width;
    window_height = height;
}

void os_window_shutdown(void)
{
    DestroyWindow(os.window);
}

bool os_window_should_close(void)
{
    return should_close;
}

void os_window_poll_events(void)
{
    MSG message;
    while (PeekMessageA(&message, os.window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void os_window_get_size(u32 *width, u32 *height)
{
    *width = window_width;
    *height = window_height;
}

f64 os_get_time(void)
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f32)(now_time.QuadPart - start_time.QuadPart) * clock_freq;
}
