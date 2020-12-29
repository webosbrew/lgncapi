#include <glib-2.0/glib.h>

#include "callbacks.h"

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
