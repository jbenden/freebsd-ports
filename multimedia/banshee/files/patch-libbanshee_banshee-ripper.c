--- libbanshee/banshee-ripper.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-ripper.c
@@ -85,7 +85,6 @@ br_raise_error (BansheeRipper *ripper, c
 static gboolean
 br_iterate_timeout (BansheeRipper *ripper)
 {
-    GstFormat format = GST_FORMAT_TIME;
     GstState state;
     gint64 position;
     
@@ -96,7 +95,7 @@ br_iterate_timeout (BansheeRipper *rippe
         return TRUE;
     }
 
-    if (!gst_element_query_position (ripper->cddasrc, &format, &position)) {
+    if (!gst_element_query_position (ripper->cddasrc, GST_FORMAT_TIME, &position)) {
         return TRUE;
     }
     
@@ -142,8 +141,9 @@ br_encoder_probe_mime_type (GstBin *bin)
         GstIterator *pad_iter = gst_element_iterate_src_pads (element);
         
         BANSHEE_GST_ITERATOR_ITERATE (pad_iter, GstPad *, pad, TRUE, {
-            GstStructure *str = GST_PAD_CAPS (pad) != NULL
-                ? gst_caps_get_structure (GST_PAD_CAPS (pad), 0)
+            GstCaps *caps = gst_pad_get_current_caps (pad);
+            GstStructure *str = caps != NULL
+                ? gst_caps_get_structure (caps, 0)
                 : NULL;
 
             if (str != NULL) {
@@ -172,10 +172,8 @@ br_encoder_probe_mime_type (GstBin *bin)
                 }
             }
             
-            gst_object_unref (pad);
+            gst_caps_unref (caps);
         });
-        
-        gst_object_unref (element);
     });
 
     return preferred_mimetype;
@@ -271,7 +269,7 @@ br_pipeline_construct (BansheeRipper *ri
         return FALSE;
     }
 
-    ripper->cddasrc = gst_element_make_from_uri (GST_URI_SRC, "cdda://1", "cddasrc");
+    ripper->cddasrc = gst_element_make_from_uri (GST_URI_SRC, "cdda://1", "cddasrc", NULL);
     if (ripper->cddasrc == NULL) {
         br_raise_error (ripper, _("Could not initialize element from cdda URI"), NULL);
         return FALSE;
@@ -405,7 +403,6 @@ br_rip_track (BansheeRipper *ripper, gin
             
             // We'll warn the user in the UI if we can't tag the encoded audio files
             *tagging_supported = TRUE;
-            gst_object_unref (element);
         }
     });
     
