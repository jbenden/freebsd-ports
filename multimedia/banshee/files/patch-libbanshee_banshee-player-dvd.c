--- libbanshee/banshee-player-dvd.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-dvd.c
@@ -296,7 +296,7 @@ bp_dvd_go_to_next_chapter (BansheePlayer
 {
     gint64 index;
     GstFormat format = gst_format_get_by_nick ("chapter");
-    gst_element_query_position (player->playbin, &format, &index);
+    gst_element_query_position (player->playbin, format, &index);
     gst_element_seek (player->playbin, 1.0, format, GST_SEEK_FLAG_FLUSH,
         GST_SEEK_TYPE_SET, index + 1, GST_SEEK_TYPE_NONE, 0);
 }
@@ -306,7 +306,7 @@ bp_dvd_go_to_previous_chapter (BansheePl
 {
     gint64 index;
     GstFormat format = gst_format_get_by_nick ("chapter");
-    gst_element_query_position (player->playbin, &format, &index);
+    gst_element_query_position (player->playbin, format, &index);
     gst_element_seek (player->playbin, 1.0, format, GST_SEEK_FLAG_FLUSH,
         GST_SEEK_TYPE_SET, index - 1, GST_SEEK_TYPE_NONE, 0);
 }
