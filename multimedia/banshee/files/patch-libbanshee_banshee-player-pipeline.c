--- libbanshee/banshee-player-pipeline.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-pipeline.c
@@ -76,7 +76,7 @@ playbin_stream_changed_cb (GstElement * 
     GstMessage *msg;
 
     // We're being called from the streaming thread, so don't do anything here
-    msg = gst_message_new_application (GST_OBJECT (player->playbin), gst_structure_new ("stream-changed", NULL));
+    msg = gst_message_new_application (GST_OBJECT (player->playbin), gst_structure_new_empty ("stream-changed"));
     gst_element_post_message (player->playbin, msg);
 }
 
@@ -218,6 +218,11 @@ bp_pipeline_bus_callback (GstBus *bus, G
             break;
         }
 
+        case GST_MESSAGE_STREAM_START: {
+            bp_next_track_starting (player);
+            break;
+        }
+
         case GST_MESSAGE_APPLICATION: {
             const gchar * name;
             const GstStructure * s = gst_message_get_structure (message);
@@ -289,11 +294,11 @@ _bp_pipeline_construct (BansheePlayer *p
     
     // Playbin is the core element that handles autoplugging (finding the right
     // source and decoder elements) based on source URI and stream content
-    player->playbin = gst_element_factory_make ("playbin2", "playbin");
+    player->playbin = gst_element_factory_make ("playbin", "playbin");
 
 #ifdef ENABLE_GAPLESS
     // FIXME: Connect a proxy about-to-finish callback that will generate a next-track-starting callback.
-    // This can be removed once playbin2 generates its own next-track signal.
+    // This can be removed once playbin generates its own next-track signal.
     // bgo#584987 - this is included in >= 0.10.26
     g_signal_connect (player->playbin, "about-to-finish", G_CALLBACK (bp_about_to_finish_callback), player);
 #endif //ENABLE_GAPLESS
@@ -305,18 +310,13 @@ _bp_pipeline_construct (BansheePlayer *p
     g_signal_connect (player->playbin, "audio-changed", G_CALLBACK (playbin_stream_changed_cb), player);
     g_signal_connect (player->playbin, "text-changed", G_CALLBACK (playbin_stream_changed_cb), player);
 
-    // Try to find an audio sink, prefer gconf, which typically is set to auto these days,
-    // fall back on auto, which should work on windows, and as a last ditch, try alsa
-    audiosink = gst_element_factory_make ("gconfaudiosink", "audiosink");
-    if (audiosink == NULL) {
-        audiosink = gst_element_factory_make ("directsoundsink", "audiosink");
-        if (audiosink != NULL) {
-            g_object_set (G_OBJECT (audiosink), "volume", 1.0, NULL);
-        } else {
-            audiosink = gst_element_factory_make ("autoaudiosink", "audiosink");
-            if (audiosink == NULL) {
-                audiosink = gst_element_factory_make ("alsasink", "audiosink");
-            }
+    audiosink = gst_element_factory_make ("directsoundsink", "audiosink");
+    if (audiosink != NULL) {
+        g_object_set (G_OBJECT (audiosink), "volume", 1.0, NULL);
+    } else {
+        audiosink = gst_element_factory_make ("autoaudiosink", "audiosink");
+        if (audiosink == NULL) {
+            audiosink = gst_element_factory_make ("alsasink", "audiosink");
         }
     }
     
@@ -341,7 +341,6 @@ _bp_pipeline_construct (BansheePlayer *p
         GstIterator *elem_iter = gst_bin_iterate_recurse (GST_BIN (audiosink));
         BANSHEE_GST_ITERATOR_ITERATE (elem_iter, GstElement *, element, TRUE, {
             player->audiosink_has_volume |= g_object_class_find_property (G_OBJECT_GET_CLASS (element), "volume") != NULL;
-            gst_object_unref (element);
         });
     }
     bp_debug ("Audiosink has volume: %s",
@@ -386,7 +385,7 @@ _bp_pipeline_construct (BansheePlayer *p
     }
    
     // Ghost pad the audio bin so audio is passed from the bin into the tee
-    teepad = gst_element_get_pad (player->audiotee, "sink");
+    teepad = gst_element_get_static_pad (player->audiotee, "sink");
     gst_element_add_pad (player->audiobin, gst_ghost_pad_new ("sink", teepad));
     gst_object_unref (teepad);
 
@@ -414,8 +413,8 @@ _bp_pipeline_construct (BansheePlayer *p
     gst_bus_add_watch (bus, bp_pipeline_bus_callback, player);
 
     // Link the first tee pad to the primary audio sink queue
-    GstPad *sinkpad = gst_element_get_pad (audiosinkqueue, "sink");
-    pad = gst_element_get_request_pad (player->audiotee, "src%d");
+    GstPad *sinkpad = gst_element_get_static_pad (audiosinkqueue, "sink");
+    pad = gst_element_get_request_pad (player->audiotee, "src_%u");
     g_object_set(player->audiotee, "alloc-pad", pad, NULL);
     gst_pad_link (pad, sinkpad);
     gst_object_unref (GST_OBJECT (pad));
