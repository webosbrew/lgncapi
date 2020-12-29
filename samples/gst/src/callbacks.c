#include <glib-2.0/glib.h>

#include "callbacks.h"
#include "main.h"

LGNC_STATUS_T _MsgEventHandler(LGNC_MSG_TYPE_T msg, unsigned int submsg, char *pData, unsigned short dataSize)

{
    switch (msg)
    {
    case LGNC_MSG_FOCUS_IN:
        g_message("LGNC_MSG_FOCUS_IN", NULL);
        return LGNC_OK;
    case LGNC_MSG_FOCUS_OUT:
        g_message("LGNC_MSG_FOCUS_OUT", NULL);
        return LGNC_OK;
    case LGNC_MSG_TERMINATE:
        g_message("LGNC_MSG_TERMINATE", NULL);
        break;
    case LGNC_MSG_HOST_EVENT:
        g_message("LGNC_MSG_HOST_EVENT", NULL);
        break;
    case LGNC_MSG_PAUSE:
        g_message("LGNC_MSG_PAUSE", NULL);
        return LGNC_OK;
    case LGNC_MSG_RESUME:
        g_message("LGNC_MSG_RESUME", NULL);
        return LGNC_OK;
    }
    return LGNC_OK;
}

unsigned int _KeyEventCallback(unsigned int key, LGNC_KEY_COND_T keyCond, LGNC_ADDITIONAL_INPUT_INFO_T *keyInput)
{
    g_message("KeyEvent key=%d, cond=%d", key, keyCond, NULL);
    return 1;
}

unsigned int _MouseEventCallback(int posX, int posY, unsigned int key, LGNC_KEY_COND_T keyCond, LGNC_ADDITIONAL_INPUT_INFO_T *keyInput)
{
    g_message("MouseEvent x=%d, y=%d key=%d, cond=%d", posX, posY, key, keyCond, NULL);
    if (key == 412 /* remote control back */ && keyCond == LGNC_KEY_RELEASE)
    {
        request_exit();
        return 1;
    }
    return 0;
}