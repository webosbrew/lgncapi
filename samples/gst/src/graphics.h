#include <lgnc_egl.h>

int open_display(int width, int height, EGLNativeWindowType displayId);

int gfx_loop();

int init_egl(EGLNativeWindowType displayId);
void finalize_egl();