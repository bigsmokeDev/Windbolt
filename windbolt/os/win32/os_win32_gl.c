#include "types.h"
#include "os_win32.h"
#include "vendor/glad/glad.h"

local HDC   dc;
local HGLRC gl_context;

typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hdc, HGLRC shareContext, const int *attribList);
typedef BOOL  (WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList,
                                                       UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;
static PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB    = 0;

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126

void os_gfx_init(void)
{
    WNDCLASSA wc = {0};
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = DefWindowProcA;
    wc.hInstance     = GetModuleHandleA(NULL);
    wc.lpszClassName = "DummyWindowClass";
    ATOM reg_ok = RegisterClassA(&wc);
    ASSERT(reg_ok != 0, "RegisterClassA failed");

    HWND dummy_window = CreateWindowA(wc.lpszClassName, "Dummy", 0,
                                      CW_USEDEFAULT, CW_USEDEFAULT, 64, 64,
                                      NULL, NULL, wc.hInstance, NULL);
    ASSERT(dummy_window != NULL, "CreateWindowA (dummy) failed");

    HDC dummy_dc = GetDC(dummy_window);
    ASSERT(dummy_dc != NULL, "GetDC (dummy) failed");

    const DWORD pfd_flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    PIXELFORMATDESCRIPTOR dpfd = {
        sizeof(dpfd), 1,
        pfd_flags,
        PFD_TYPE_RGBA,
        32, // color bits
        0,0,0,0,0,0,
        8,  // alpha
        0,0,0,0,0,0,
        24, // depth
        8,  // stencil
        0,
        PFD_MAIN_PLANE,
        0,0,0,0
    };
    int dummy_pf = ChoosePixelFormat(dummy_dc, &dpfd);
    ASSERT(dummy_pf != 0, "ChoosePixelFormat (dummy) failed");
    BOOL spf_dummy = SetPixelFormat(dummy_dc, dummy_pf, &dpfd);
    ASSERT(spf_dummy == TRUE, "SetPixelFormat (dummy) failed");

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    ASSERT(dummy_context != NULL, "wglCreateContext (dummy) failed");

    BOOL mk_dummy = wglMakeCurrent(dummy_dc, dummy_context);
    ASSERT(mk_dummy == TRUE, "wglMakeCurrent (dummy) failed");

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    ASSERT(wglCreateContextAttribsARB != NULL, "wglGetProcAddress(wglCreateContextAttribsARB) failed");

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
    ASSERT(wglChoosePixelFormatARB != NULL, "wglGetProcAddress(wglChoosePixelFormatARB) failed");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);

    ASSERT(os.window != NULL, "os.window is NULL");
    dc = GetDC(os.window);
    ASSERT(dc != NULL, "GetDC(os.window) failed");

    const int pf_attribs[] = {
        0x2003, 0x2027,  // WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB
        0x2001, 1,       // WGL_DRAW_TO_WINDOW_ARB, TRUE
        0x2010, 1,       // WGL_SUPPORT_OPENGL_ARB, TRUE
        0x2011, 1,       // WGL_DOUBLE_BUFFER_ARB, TRUE
        0x2013, 0x202B,  // WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB
        0x2014, 32,      // WGL_COLOR_BITS_ARB
        0x2015, 8,       // WGL_RED_BITS_ARB
        0x2017, 8,       // WGL_GREEN_BITS_ARB
        0x2019, 8,       // WGL_BLUE_BITS_ARB
        0x201B, 8,       // WGL_ALPHA_BITS_ARB
        0x2022, 24,      // WGL_DEPTH_BITS_ARB
        0, 0             // terminator
    };

    int pixel_format = 0;
    UINT num_formats = 0;
    BOOL ok_choose = wglChoosePixelFormatARB(dc, pf_attribs, NULL, 1, &pixel_format, &num_formats);
    ASSERT(ok_choose == TRUE, "wglChoosePixelFormatARB failed");
    ASSERT(num_formats > 0, "no matching WGL pixel format found");

    BOOL ok_setpf = SetPixelFormat(dc, pixel_format, NULL);
    ASSERT(ok_setpf == TRUE, "SetPixelFormat (real DC) failed");

    const int context_attribs[] = {
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        0, 0
    };

    gl_context = wglCreateContextAttribsARB(dc, NULL, context_attribs);
    ASSERT(gl_context != NULL, "wglCreateContextAttribsARB failed");

    BOOL mk_real = wglMakeCurrent(dc, gl_context);
    ASSERT(mk_real == TRUE, "wglMakeCurrent (real) failed");

    int glad_ok = gladLoadGL();
    ASSERT(glad_ok != 0, "gladLoadGL failed");

    ShowWindow(os.window, SW_SHOW);
    UpdateWindow(os.window);
}

void os_gfx_shutdown(void)
{
    wglMakeCurrent(0, 0);
    if (gl_context)
	{
        wglDeleteContext(gl_context);
        gl_context = NULL;
    }
    if (dc)
	{
        ReleaseDC(os.window, dc);
        dc = NULL;
    }
}

void os_gfx_swap_buffers(void)
{
    ASSERT(dc != NULL, "SwapBuffers: DC is NULL");
    SwapBuffers(dc);
}
