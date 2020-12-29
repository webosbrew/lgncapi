#include <stdlib.h>
#include <stdio.h>

#include <glib-2.0/glib.h>
#include <lgnc_system.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "callbacks.h"
#include "graphics.h"
#include "gst_sample.h"

__attribute__((constructor)) static void constructor()
{
    int fdout = open("/tmp/lgnc.gst-sample.out.log", O_RDWR | O_CREAT | O_APPEND, 0666);
    int fderr = open("/tmp/lgnc.gst-sample.err.log", O_RDWR | O_CREAT | O_APPEND, 0666);
    // make stdout go to file
    dup2(fdout, 1);
    // make stderr go to file
    dup2(fderr, 2);

    // fd no longer needed - the dup'ed handles are sufficient
    close(fdout);
    close(fderr);
}
__attribute__((destructor)) static void destructor()
{
    printf("Bye!\n");
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    int ncret = 0;

    LGNC_SYSTEM_CALLBACKS_T callbacks = {
        .pfnJoystickEventCallback = NULL,
        .pfnMsgHandler = _MsgEventHandler,
        .pfnKeyEventCallback = NULL,
        .pfnMouseEventCallback = NULL};
    g_message("LGNC_SYSTEM_Initialize", NULL);
    ncret = LGNC_SYSTEM_Initialize(argc, argv, &callbacks);
    g_message("LGNC_SYSTEM_Initialize done. ret=%d", ncret, NULL);
    if (ncret != 0)
    {
        goto finalize;
    }

    g_message("LGNC_SYSTEM_GetDisplayId", NULL);
    int displayId;
    ncret = LGNC_SYSTEM_GetDisplayId(&displayId);
    g_message("LGNC_SYSTEM_GetDisplayId done. ret=%d", ncret, NULL);
    if (ncret != 0)
    {
        goto finalize;
    }

    g_message("open_display", NULL);
    ncret = open_display(1280, 720, displayId);
    g_message("open_display done. ret=%d", ncret, NULL);
    if (ncret != 0)
    {
        goto finalize;
    }

    gst_sample_initialize();
    gfx_loop();
    gst_sample_finalize();

finalize:
    LGNC_SYSTEM_Finalize();
    return 0;
}
