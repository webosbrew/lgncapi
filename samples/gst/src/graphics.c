#include "graphics.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLES2/gl2.h>
#include <glib-2.0/glib.h>

EGLDisplay g_pstEglDisplay = NULL;
EGLConfig g_pstEglConfig = NULL;
EGLSurface g_pstEglSurface = NULL;
EGLContext g_pstEglContext = NULL;

int open_display(int width, int height, EGLNativeWindowType displayId)
{
    if (init_egl(displayId) == 0)
    {
        finalize_egl();
        return -1;
    }
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    LGNC_eglSwapBuffers(g_pstEglDisplay, g_pstEglSurface);
    return 0;
}

int gfx_loop()
{

    while (1)
    {
        glClearColor(1.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        eglSwapBuffers(g_pstEglDisplay, g_pstEglSurface);
        usleep(16 * 1000);
    }

    return 0;
}

int init_egl(EGLNativeWindowType displayId)
{
    EGLint major, minor, count, n, size;
    EGLConfig *configs;
    int i;

    EGLint configAttributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE};

    static const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE};

    g_pstEglDisplay = LGNC_eglGetDisplay();
    if (g_pstEglDisplay == EGL_NO_DISPLAY)
    {
        g_error("ERROR, cannot create create egl g_pstDisplay", NULL);
        return 0;
    }

    if (LGNC_eglInitialize(g_pstEglDisplay, &major, &minor) != EGL_TRUE)
    {
        g_error("ERROR, cannot initialize egl g_pstDisplay", NULL);
        return 0;
    }

    if (LGNC_eglGetConfigs(g_pstEglDisplay, NULL, 0, &count) != EGL_TRUE)
    {
        g_error("ERROR, eglGetConfigs failed", NULL);
        return 0;
    }
    configs = (EGLConfig *)calloc(count, sizeof(EGLConfig));
    LGNC_eglChooseConfig(g_pstEglDisplay, configAttributes, configs, count, &n);
    // simply choose the first config
    g_pstEglConfig = configs[0];

    g_pstEglSurface = LGNC_eglCreateWindowSurface(g_pstEglDisplay, g_pstEglConfig, displayId, NULL);
    if (g_pstEglSurface == EGL_NO_SURFACE)
    {
        g_error("ERROR, cannot create create egl g_pstEglSurface", NULL);
        return 0;
    }

    g_pstEglContext = LGNC_eglCreateContext(g_pstEglDisplay, g_pstEglConfig, EGL_NO_CONTEXT, contextAttributes);
    if (g_pstEglContext == EGL_NO_CONTEXT)
    {
        g_error("ERROR, cannot create create egl g_pstEglContext", NULL);
        return 0;
    }

    if (LGNC_eglMakeCurrent(g_pstEglDisplay, g_pstEglSurface, g_pstEglSurface, g_pstEglContext) != EGL_TRUE)
    {
        g_error("ERROR, cannot make current", NULL);
        return 0;
    }
    LGNC_eglSwapInterval(g_pstEglDisplay, 1);
    return 1;
}

void finalize_egl()
{
    if (g_pstEglDisplay == NULL)
    {
        return;
    }
    LGNC_eglMakeCurrent(g_pstEglDisplay, 0, 0, 0);
    LGNC_eglDestroySurface(g_pstEglDisplay, g_pstEglSurface);
    LGNC_eglDestroyContext(g_pstEglDisplay, g_pstEglContext);
    LGNC_eglTerminate(g_pstEglDisplay);
}