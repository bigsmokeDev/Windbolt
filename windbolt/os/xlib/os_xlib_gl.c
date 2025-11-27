#include "base.h"
#include "os/os_gfx.h"
#include "os_xlib.h"
#include "types.h"

#include "vendor/glad/glad.h"
#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
local GLXFBConfig best_fb_config;
local GLXContext glx_context;

Visual *os_xlib_get_glx_visual(void)
{
	GLint major_glx_version = 0, minor_glx_version = 0;
    glXQueryVersion(os.dpy, &major_glx_version, &minor_glx_version);
    ASSERT(major_glx_version >= 1 && minor_glx_version >= 4, "GLX version is too old");
    
    // framebuffer config
    GLint glx_attribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		None
	};
    
    s32 fb_count;
    GLXFBConfig *fb_config = glXChooseFBConfig(os.dpy, os.screen, glx_attribs, &fb_count);
    ASSERT(fb_config, "glXChooseFBConfig failure");
    
    // pick the framebuffer config with the most samples per pixel
    s32 best_fb_config_index = -1, worst_fb_config_index = -1, best_num_samp = -1, worst_num_samp = 999;
	for (s32 i = 0; i < fb_count; ++i)
	{
		XVisualInfo *current_visual_info = glXGetVisualFromFBConfig(os.dpy, fb_config[i]);
		if (current_visual_info != 0)
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib(os.dpy, fb_config[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(os.dpy, fb_config[i], GLX_SAMPLES       , &samples );
            
			if (best_fb_config_index < 0 || (samp_buf && samples > best_num_samp))
			{
				best_fb_config_index = i;
				best_num_samp = samples;
			}
			if (worst_fb_config_index < 0 || !samp_buf || samples < worst_num_samp)
				worst_fb_config_index = i;
			worst_num_samp = samples;
		}
		XFree(current_visual_info);
	}
	best_fb_config = fb_config[best_fb_config_index];
	XFree(fb_config);
    
    XVisualInfo *visual_info = glXGetVisualFromFBConfig(os.dpy, best_fb_config);
    ASSERT(visual_info, "glXGetVisualFromFBConfig failure");

	ASSERT(os.screen == visual_info->screen, "screen and visual info screen do not match");

	return visual_info->visual;
}

void os_gfx_init(void)
{
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    
    s32 context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };
    
    const char *glx_extensions = glXQueryExtensionsString(os.dpy, os.screen);
    if (!strstr(glx_extensions, "GLX_ARB_create_context"))
        glx_context = glXCreateNewContext(os.dpy, best_fb_config, GLX_RGBA_TYPE, 0, True);
    else
        glx_context = glXCreateContextAttribsARB(os.dpy, best_fb_config, 0, true, context_attribs);
    XSync(os.dpy, False);
    
    glXMakeCurrent(os.dpy, os.window, glx_context);

	gladLoadGLLoader((GLADloadproc)os_gfx_get_proc_address);
}

void os_gfx_swap_buffers(void)
{
	glXSwapBuffers(os.dpy, os.window);
}

OS_ProcAddress os_gfx_get_proc_address(const char *name)
{
	return (OS_ProcAddress)glXGetProcAddress((const GLubyte*)name);
}
