--- libbanshee/banshee-transcoder.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-transcoder.c
@@ -64,14 +64,13 @@ gst_transcoder_raise_error(GstTranscoder
 static gboolean
 gst_transcoder_iterate_timeout(GstTranscoder *transcoder)
 {
-    GstFormat format = GST_FORMAT_TIME;
     gint64 position;
     gint64 duration;
 
     g_return_val_if_fail(transcoder != NULL, FALSE);
 
-    if(!gst_element_query_duration(transcoder->pipeline, &format, &duration) ||
-        !gst_element_query_position(transcoder->sink_bin, &format, &position)) {
+    if(!gst_element_query_duration(transcoder->pipeline, GST_FORMAT_TIME, &duration) ||
+        !gst_element_query_position(transcoder->sink_bin, GST_FORMAT_TIME, &position)) {
         return TRUE;
     }
 
@@ -196,14 +195,14 @@ gst_transcoder_new_decoded_pad(GstElemen
 
     g_return_if_fail(transcoder != NULL);
 
-    audiopad = gst_element_get_pad(transcoder->sink_bin, "sink");
+    audiopad = gst_element_get_static_pad(transcoder->sink_bin, "sink");
     
     if(GST_PAD_IS_LINKED(audiopad)) {
         g_object_unref(audiopad);
         return;
     }
 
-    caps = gst_pad_get_caps(pad);
+    caps = gst_pad_query_caps(pad, NULL);
     str = gst_caps_get_structure(caps, 0);
     
     if(!g_strrstr(gst_structure_get_name(str), "audio")) {
@@ -235,19 +234,19 @@ gst_transcoder_create_pipeline(GstTransc
     
     transcoder->pipeline = gst_pipeline_new("pipeline");
 
-    source_elem = gst_element_make_from_uri(GST_URI_SRC, input_uri, "source");
+    source_elem = gst_element_make_from_uri(GST_URI_SRC, input_uri, "source", NULL);
     if(source_elem == NULL) {
         gst_transcoder_raise_error(transcoder, _("Could not create source element"), NULL);
         return FALSE;
     }
 
-    decoder_elem = gst_element_factory_make("decodebin2", "decodebin2");
+    decoder_elem = gst_element_factory_make("decodebin", "decodebin");
     if(decoder_elem == NULL) {
-        gst_transcoder_raise_error(transcoder, _("Could not create decodebin2 plugin"), NULL);
+        gst_transcoder_raise_error(transcoder, _("Could not create decodebin plugin"), NULL);
         return FALSE;
     }
     
-    sink_elem = gst_element_make_from_uri(GST_URI_SINK, output_uri, "sink");
+    sink_elem = gst_element_make_from_uri(GST_URI_SINK, output_uri, "sink", NULL);
     if(sink_elem == NULL) {
         gst_transcoder_raise_error(transcoder, _("Could not create sink element"), NULL);
         return FALSE;
@@ -277,7 +276,7 @@ gst_transcoder_create_pipeline(GstTransc
          return FALSE;
     }
 
-    encoder_pad = gst_element_get_pad(conv_elem, "sink");
+    encoder_pad = gst_element_get_static_pad(conv_elem, "sink");
     if(encoder_pad == NULL) {
         gst_transcoder_raise_error(transcoder, _("Could not get sink pad from encoder"), NULL);
         return FALSE;
