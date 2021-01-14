#include "lgnc_wrapper.h"
#include "lgnc_system.h"

#include <dlfcn.h>

static void *_lgncopenapi_so = NULL;

wrapper_func _LGNC_SYSTEM_Initialize;
wrapper_func _LGNC_SYSTEM_Finalize;

wrapper_func _LGNC_DIRECTAUDIO_CheckBuffer;
wrapper_func _LGNC_DIRECTAUDIO_Close;
wrapper_func _LGNC_DIRECTAUDIO_Open;
wrapper_func _LGNC_DIRECTAUDIO_Play;

wrapper_func _LGNC_DIRECTVIDEO_Close;
wrapper_func _LGNC_DIRECTVIDEO_Open;
wrapper_func _LGNC_DIRECTVIDEO_Play;

int LGNC_SYSTEM_Initialize(int argc, char **argv, LGNC_SYSTEM_CALLBACKS_T *callbacks)
{
    if (!_lgncopenapi_so)
    {
        _lgncopenapi_so = dlopen("lgncopenapi.so", RTLD_NOW);
        if (!_lgncopenapi_so)
        {
            return -1;
        }
        _LGNC_SYSTEM_Initialize = dlsym(_lgncopenapi_so, "LGNC_SYSTEM_Initialize");
        _LGNC_SYSTEM_Finalize = dlsym(_lgncopenapi_so, "LGNC_SYSTEM_Finalize");
        
        _LGNC_DIRECTAUDIO_CheckBuffer = dlsym(_lgncopenapi_so, "LGNC_DIRECTAUDIO_CheckBuffer");
        _LGNC_DIRECTAUDIO_Close = dlsym(_lgncopenapi_so, "LGNC_DIRECTAUDIO_Close");
        _LGNC_DIRECTAUDIO_Open = dlsym(_lgncopenapi_so, "LGNC_DIRECTAUDIO_Open");
        _LGNC_DIRECTAUDIO_Play = dlsym(_lgncopenapi_so, "LGNC_DIRECTAUDIO_Play");

        _LGNC_DIRECTVIDEO_Close = dlsym(_lgncopenapi_so, "LGNC_DIRECTVIDEO_Close");
        _LGNC_DIRECTVIDEO_Open = dlsym(_lgncopenapi_so, "LGNC_DIRECTVIDEO_Open");
        _LGNC_DIRECTVIDEO_Play = dlsym(_lgncopenapi_so, "LGNC_DIRECTVIDEO_Play");
    }
    _LGNC_SYSTEM_Initialize(argc, argv, callbacks);
}

int LGNC_SYSTEM_Finalize(void)
{
    _LGNC_SYSTEM_Finalize();
    dlclose(_lgncopenapi_so);
}