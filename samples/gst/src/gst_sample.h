#pragma once

#include <gst/gst.h>
#include <gst/app/app.h>

int gst_sample_initialize();
int gst_sample_finalize();

void audioEos(GstAppSink *appsink, void *userData);
GstFlowReturn audioNewPreroll(GstAppSink *appsink, void *userData);
GstFlowReturn audioNewSample(GstAppSink *appsink, void *userData);
void videoEos(GstAppSink *appsink, void *userData);
GstFlowReturn videoNewPreroll(GstAppSink *appsink, void *userData);
GstFlowReturn videoNewSample(GstAppSink *appsink, void *userData);