--- libbanshee/banshee-player-vis.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-vis.c
@@ -27,17 +27,17 @@
 //
 
 #include <math.h>
+#include <gst/audio/audio.h>
 
 #include "banshee-player-vis.h"
 
 #define SLICE_SIZE 735
 
 static GstStaticCaps vis_data_sink_caps = GST_STATIC_CAPS (
-    "audio/x-raw-float, "
+    "audio/x-raw, "
+    "format = (string) " GST_AUDIO_NE(F32) ", "
     "rate = (int) 44100, "
-    "channels = (int) 2, "
-    "endianness = (int) BYTE_ORDER, "
-    "width = (int) 32"
+    "channels = (int) 2"
 );
 
 // ---------------------------------------------------------------------------
@@ -48,6 +48,7 @@ static void
 bp_vis_pcm_handoff (GstElement *sink, GstBuffer *buffer, GstPad *pad, gpointer userdata)
 {
     BansheePlayer *player = (BansheePlayer*)userdata;
+    GstCaps *caps;
     GstStructure *structure;
     gint channels, wanted_size;
     gfloat *data;
@@ -69,14 +70,16 @@ bp_vis_pcm_handoff (GstElement *sink, Gs
         player->vis_thawing = FALSE;
     }
     
-    structure = gst_caps_get_structure (gst_buffer_get_caps (buffer), 0);
+    caps = gst_pad_get_current_caps (pad);
+    structure = gst_caps_get_structure (caps, 0);
     gst_structure_get_int (structure, "channels", &channels);
+    gst_caps_unref (caps);
     
     wanted_size = channels * SLICE_SIZE * sizeof (gfloat);
 
-    gst_adapter_push (player->vis_buffer, gst_buffer_copy (buffer));
+    gst_adapter_push (player->vis_buffer, gst_buffer_ref (buffer));
     
-    while ((data = (gfloat *)gst_adapter_peek (player->vis_buffer, wanted_size)) != NULL) {
+    while ((data = (gfloat *)gst_adapter_map (player->vis_buffer, wanted_size)) != NULL) {
         gfloat *deinterlaced = g_malloc (wanted_size);
         gfloat *specbuf = g_new (gfloat, SLICE_SIZE * 2);
 
@@ -124,6 +127,7 @@ bp_vis_pcm_handoff (GstElement *sink, Gs
         g_free (deinterlaced);
         g_free (specbuf);
 
+        gst_adapter_unmap (player->vis_buffer);
         gst_adapter_flush (player->vis_buffer, wanted_size);
     }
 }
@@ -132,65 +136,28 @@ bp_vis_pcm_handoff (GstElement *sink, Gs
 // Internal Functions
 // ---------------------------------------------------------------------------
 
-static void
-_bp_vis_pipeline_block_callback (GstPad *pad, gboolean blocked, gpointer data)
+static GstPadProbeReturn
+_bp_vis_pipeline_event_probe (GstPad *pad, GstPadProbeInfo *info, gpointer data)
 {
     BansheePlayer *player = (BansheePlayer *) data;
+    GstEvent *event;
 
-    if (!blocked) {
-        // Set thawing mode (discards buffers that are too old from the queue).
-        player->vis_thawing = TRUE;
-    }
-}
-
-static void
-_bp_vis_pipeline_set_blocked (BansheePlayer *player, gboolean blocked)
-{
-    GstPad *queue_sink;
-
-    if (player->vis_resampler == NULL)
-        return;
-
-    queue_sink = gst_element_get_static_pad (player->vis_resampler, "src");
-
-    gst_pad_set_blocked_async (queue_sink, blocked, _bp_vis_pipeline_block_callback, (gpointer) player);
-
-    gst_object_unref (GST_OBJECT (queue_sink));
-}
-
-static gboolean
-_bp_vis_pipeline_event_probe (GstPad *pad, GstEvent *event, gpointer data)
-{
-    BansheePlayer *player = (BansheePlayer *) data;
+    if ((info->type & GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM) == 0)
+        return GST_PAD_PROBE_PASS;
 
+    event = GST_EVENT (info->data);
     switch (GST_EVENT_TYPE (event)) {
         case GST_EVENT_FLUSH_START:
         case GST_EVENT_FLUSH_STOP:
         case GST_EVENT_SEEK:
-        case GST_EVENT_NEWSEGMENT:
+        case GST_EVENT_SEGMENT:
         case GST_EVENT_CUSTOM_DOWNSTREAM:
             player->vis_thawing = TRUE;
 
         default: break;
     }
 
-    if (player->vis_enabled)
-        return TRUE;
-
-    switch (GST_EVENT_TYPE (event)) {
-    case GST_EVENT_EOS:
-    case GST_EVENT_CUSTOM_DOWNSTREAM_OOB:
-        _bp_vis_pipeline_set_blocked (player, FALSE);
-        break;
-    case GST_EVENT_CUSTOM_DOWNSTREAM:
-    case GST_EVENT_NEWSEGMENT:
-        _bp_vis_pipeline_set_blocked (player, TRUE);
-        break;
-
-    default: break;
-    }
-
-    return TRUE;
+    return GST_PAD_PROBE_PASS;
 }
 
 void
@@ -212,9 +179,8 @@ _bp_vis_pipeline_setup (BansheePlayer *p
     // Core elements, if something fails here, it's the end of the world
     audiosinkqueue = gst_element_factory_make ("queue", "vis-queue");
 
-    pad = gst_element_get_static_pad (audiosinkqueue, "sink");
-    gst_pad_add_event_probe (pad, G_CALLBACK (_bp_vis_pipeline_event_probe), player);
-    gst_object_unref (GST_OBJECT (pad));
+    player->vis_event_probe_pad = gst_element_get_static_pad (audiosinkqueue, "sink");
+    player->vis_event_probe_id = gst_pad_add_probe (player->vis_event_probe_pad, GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM, _bp_vis_pipeline_event_probe, player, NULL);
 
     resampler = gst_element_factory_make ("audioresample", "vis-resample");
     converter = gst_element_factory_make ("audioconvert", "vis-convert");
@@ -255,7 +221,7 @@ _bp_vis_pipeline_setup (BansheePlayer *p
                       converter, fakesink, NULL);
     
     pad = gst_element_get_static_pad (audiosinkqueue, "sink");
-    teepad = gst_element_get_request_pad (player->audiotee, "src%d");
+    teepad = gst_element_get_request_pad (player->audiotee, "src_%u");
     gst_pad_link (teepad, pad);
     gst_object_unref (GST_OBJECT (teepad));
     gst_object_unref (GST_OBJECT (pad));
@@ -270,14 +236,17 @@ _bp_vis_pipeline_setup (BansheePlayer *p
     player->vis_resampler = resampler;
     player->vis_thawing = FALSE;
     player->vis_enabled = FALSE;
-
-    // Disable the pipeline till we hear otherwise from managed land.
-    _bp_vis_pipeline_set_blocked (player, TRUE);
 }
 
 void
 _bp_vis_pipeline_destroy (BansheePlayer *player)
 {
+    if (player->vis_event_probe_pad) {
+        gst_pad_remove_probe (player->vis_event_probe_pad, player->vis_event_probe_id);
+        gst_object_unref (GST_OBJECT (player->vis_event_probe_pad));
+        player->vis_event_probe_pad = NULL;
+    }
+
     if (player->vis_buffer != NULL) {
         gst_object_unref (player->vis_buffer);
         player->vis_buffer = NULL;
