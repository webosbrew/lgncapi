#include "lgnc_directaudio.h"
#include "lgnc_wrapper.h"

int LGNC_DIRECTAUDIO_CheckBuffer() {
    return _LGNC_DIRECTAUDIO_CheckBuffer();
}

int LGNC_DIRECTAUDIO_Close() {
    return _LGNC_DIRECTVIDEO_Close();
}

int LGNC_DIRECTAUDIO_Open(LGNC_ADEC_DATA_INFO_T *info) {
    return _LGNC_DIRECTAUDIO_Open(info);
}

int LGNC_DIRECTAUDIO_Play(void *data, unsigned int size) {
    return LGNC_DIRECTAUDIO_Play(data, size);
}