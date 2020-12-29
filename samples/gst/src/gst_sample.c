#include "gst_sample.h"

#include <memory.h>
#include <stdlib.h>

#include <lgnc_directaudio.h>
#include <lgnc_directvideo.h>

static GstElement *pipeline;

int gst_sample_initialize()
{
    GstStateChangeReturn ret;

    GstElement *audiosink, *videosink;
    pipeline = gst_parse_launch("filesrc location=./assets/test.mp4 ! qtdemux name=demux \
    demux.audio_0 ! queue ! decodebin ! audioconvert ! audio/x-raw,format=S16LE ! appsink name=audsink \
    demux.video_0 ! queue ! h264parse ! appsink name=vidsink",
                                NULL);

    g_assert(pipeline);

    audiosink = gst_bin_get_by_name(GST_BIN(pipeline), "audsink");
    g_assert(audiosink);

    videosink = gst_bin_get_by_name(GST_BIN(pipeline), "vidsink");
    g_assert(videosink);

    GstAppSinkCallbacks audioCallbacks = {
        .eos = audioEos,
        .new_preroll = audioNewPreroll,
        .new_sample = audioNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(audiosink), &audioCallbacks, NULL, NULL);

    GstAppSinkCallbacks videoCallbacks = {
        .eos = videoEos,
        .new_preroll = videoNewPreroll,
        .new_sample = videoNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(videosink), &videoCallbacks, NULL, NULL);

    /* Start playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    return 0;
}

int gst_sample_wait_for_eos()
{
    GstBus *bus;
    GstMessage *msg;

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL)
    {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr("Unexpected message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    /* Free resources */
    gst_object_unref(bus);
}

int gst_sample_finalize()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

void audioEos(GstAppSink *appsink, gpointer user_data)
{
    LGNC_DIRECTAUDIO_Close();
}

GstFlowReturn audioNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);
    int channels = 0, rate = 0;
    gst_structure_get_int(cap, "channels", &channels);
    gst_structure_get_int(cap, "rate", &rate);

    gst_sample_unref(preroll);

    LGNC_ADEC_DATA_INFO_T info = {
        .codec = LGNC_ADEC_FMT_PCM,
        .AChannel = LGNC_ADEC_CH_INDEX_MAIN,
        .samplingFreq = LGNC_ADEC_SAMPLING_FREQ_OF(rate),
        .numberOfChannel = channels,
        .bitPerSample = 16};
    if (LGNC_DIRECTAUDIO_Open(&info) != 0)
    {
        return GST_FLOW_ERROR;
    }
    return GST_FLOW_OK;
}

GstFlowReturn audioNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    gsize bufsize = gst_buffer_get_size(buf);
    gpointer rawbuf = malloc(bufsize);

    gst_buffer_extract(buf, 0, rawbuf, bufsize);

    int ncret = LGNC_DIRECTAUDIO_Play(rawbuf, bufsize);

    free(rawbuf);
    gst_sample_unref(sample);
    return ncret == 0 ? GST_FLOW_OK : GST_FLOW_ERROR;
}

void videoEos(GstAppSink *appsink, gpointer user_data)
{
    LGNC_DIRECTVIDEO_Close();
}

GstFlowReturn videoNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);
    int channels = 0;
    g_message(gst_caps_to_string(caps));

    int width, height;
    g_assert(gst_structure_get_int(cap, "width", &width));
    g_assert(gst_structure_get_int(cap, "height", &height));

    gst_sample_unref(preroll);

    LGNC_VDEC_DATA_INFO_T info = {
        .width = width,
        .height = height,
        .vdecFmt = LGNC_VDEC_FMT_H264,
        .trid_type = LGNC_VDEC_3D_TYPE_NONE};
    if (LGNC_DIRECTVIDEO_Open(&info) != 0)
    {
        return GST_FLOW_ERROR;
    }
    return GST_FLOW_OK;
}

GstFlowReturn videoNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    gsize bufsize = gst_buffer_get_size(buf);
    gpointer rawbuf = malloc(bufsize);

    gst_buffer_extract(buf, 0, rawbuf, bufsize);

    int ncret = LGNC_DIRECTVIDEO_Play(rawbuf, bufsize, 0x20000, 0x32270);

    free(rawbuf);
    gst_sample_unref(sample);
    return ncret == 0 ? GST_FLOW_OK : GST_FLOW_ERROR;
}