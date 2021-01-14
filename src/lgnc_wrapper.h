#pragma once

typedef void *(*wrapper_func)();

extern wrapper_func _LGNC_SYSTEM_Initialize;
extern wrapper_func _LGNC_SYSTEM_Finalize;

extern wrapper_func _LGNC_DIRECTAUDIO_CheckBuffer;
extern wrapper_func _LGNC_DIRECTAUDIO_Close;
extern wrapper_func _LGNC_DIRECTAUDIO_Open;
extern wrapper_func _LGNC_DIRECTAUDIO_Play;

extern wrapper_func _LGNC_DIRECTVIDEO_Close;
extern wrapper_func _LGNC_DIRECTVIDEO_Open;
extern wrapper_func _LGNC_DIRECTVIDEO_Play;