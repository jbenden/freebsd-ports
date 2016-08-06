--- libbanshee/banshee-bpmdetector.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-bpmdetector.c
@@ -30,6 +30,7 @@
 #  include "config.h"
 #endif
 
+#include <stdio.h>
 #include <string.h>
 #include <glib/gi18n.h>
 
@@ -50,7 +51,7 @@ struct BansheeBpmDetector {
 
     /*
      * You can run this pipeline on the cmd line with:
-     * gst-launch -m filesrc location=/path/to/my.mp3 ! decodebin2 ! \
+     * gst-launch -m filesrc location=/path/to/my.mp3 ! decodebin ! \
      *    audioconvert ! bpmdetect ! fakesink
      */
 
@@ -168,14 +169,14 @@ bbd_new_decoded_pad(GstElement *decodebi
 
     g_return_if_fail(detector != NULL);
 
-    audiopad = gst_element_get_pad(detector->audioconvert, "sink");
+    audiopad = gst_element_get_static_pad(detector->audioconvert, "sink");
     
     if(GST_PAD_IS_LINKED(audiopad)) {
         g_object_unref(audiopad);
         return;
     }
 
-    caps = gst_pad_get_caps(pad);
+    caps = gst_pad_query_caps(pad, NULL);
     str = gst_caps_get_structure(caps, 0);
     
     if(!g_strrstr(gst_structure_get_name(str), "audio")) {
@@ -209,9 +210,9 @@ bbd_pipeline_construct (BansheeBpmDetect
         return FALSE;
     }
   
-    detector->decodebin = gst_element_factory_make ("decodebin2", "decodebin2");
+    detector->decodebin = gst_element_factory_make ("decodebin", "decodebin2");
     if (detector->decodebin == NULL) {
-        bbd_raise_error (detector, _("Could not create decodebin2 plugin"), NULL);
+        bbd_raise_error (detector, _("Could not create decodebin plugin"), NULL);
         return FALSE;
     }
 
