#include <stdio.h>
#include <lgnc_system.h>

#include "main.h"
#include "callbacks.h"
#include "graphics.h"

#include "debughelper.h"

static int exit_requested_;

int main(int argc, char *argv[]) {
    REDIR_STDOUT("lgnc.egl-sample");

    int ncret = 0;

    LGNC_SYSTEM_CALLBACKS_T callbacks = {
            .pfnJoystickEventCallback = NULL,
            .pfnMsgHandler = _MsgEventHandler,
            .pfnKeyEventCallback = _KeyEventCallback,
            .pfnMouseEventCallback = _MouseEventCallback};
    printf("LGNC_SYSTEM_Initialize\n");
    ncret = LGNC_SYSTEM_Initialize(argc, argv, &callbacks);
    printf("LGNC_SYSTEM_Initialize done. ret=%d\n", ncret);
    if (ncret != 0) {
        goto finalize;
    }

    printf("LGNC_SYSTEM_GetDisplayId\n");
    int displayId;
    ncret = LGNC_SYSTEM_GetDisplayId(&displayId);
    printf("LGNC_SYSTEM_GetDisplayId done. ret=%d, displayId=%d\n", ncret, displayId);
    if (ncret != 0) {
        goto finalize;
    }

    printf("open_display\n");
    ncret = open_display(1280, 720, displayId);
    printf("open_display done. ret=%d\n", ncret);
    if (ncret != 0) {
        goto finalize;
    }

    gfx_loop();

    finalize:
    LGNC_SYSTEM_Finalize();
    return 0;
}

void request_exit() {
    exit_requested_ = 1;
}

int exit_requested() {
    return exit_requested_;
}