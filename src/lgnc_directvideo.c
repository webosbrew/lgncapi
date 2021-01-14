#include "lgnc_directvideo.h"
#include "lgnc_wrapper.h"

int LGNC_DIRECTVIDEO_Close()
{
    return _LGNC_DIRECTAUDIO_Close();
}

int LGNC_DIRECTVIDEO_Open(LGNC_VDEC_DATA_INFO_T *info)
{
    return _LGNC_DIRECTVIDEO_Open(info);
}

int LGNC_DIRECTVIDEO_Play(void *data, unsigned int size)
{
    return _LGNC_DIRECTVIDEO_Play(data, size);
}