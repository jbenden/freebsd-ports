--- libbanshee/banshee-player.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player.c
@@ -250,12 +250,11 @@ bp_set_position (BansheePlayer *player, 
 P_INVOKE guint64
 bp_get_position (BansheePlayer *player)
 {
-    static GstFormat format = GST_FORMAT_TIME;
     gint64 position;
 
     g_return_val_if_fail (IS_BANSHEE_PLAYER (player), 0);
 
-    if (player->playbin != NULL && gst_element_query_position (player->playbin, &format, &position)) {
+    if (player->playbin != NULL && gst_element_query_position (player->playbin, GST_FORMAT_TIME, &position)) {
         return position / GST_MSECOND;
     }
     
@@ -265,12 +264,11 @@ bp_get_position (BansheePlayer *player)
 P_INVOKE guint64
 bp_get_duration (BansheePlayer *player)
 {
-    static GstFormat format = GST_FORMAT_TIME;
     gint64 duration;
 
     g_return_val_if_fail (IS_BANSHEE_PLAYER (player), 0);
 
-    if (player->playbin != NULL && gst_element_query_duration (player->playbin, &format, &duration)) {
+    if (player->playbin != NULL && gst_element_query_duration (player->playbin, GST_FORMAT_TIME, &duration)) {
         return duration / GST_MSECOND;
     }
     
@@ -446,7 +444,6 @@ bp_set_subtitle_uri (BansheePlayer *play
     g_return_if_fail (IS_BANSHEE_PLAYER (player));
     gint64 pos = -1;
     GstState state;
-    GstFormat format = GST_FORMAT_BYTES;
     gboolean paused = FALSE;
 
     // Gstreamer playbin do not support to set suburi during playback
@@ -454,7 +451,7 @@ bp_set_subtitle_uri (BansheePlayer *play
     gst_element_get_state (player->playbin, &state, NULL, 0);
     paused = (state == GST_STATE_PAUSED);
     if (state >= GST_STATE_PAUSED) {
-        gst_element_query_position (player->playbin, &format, &pos);
+        gst_element_query_position (player->playbin, GST_FORMAT_BYTES, &pos);
         gst_element_set_state (player->playbin, GST_STATE_READY);
         // Force to wait asynch operation
         gst_element_get_state (player->playbin, &state, NULL, -1);
@@ -467,7 +464,7 @@ bp_set_subtitle_uri (BansheePlayer *play
     gst_element_get_state (player->playbin, &state, NULL, -1);
 
     if (pos != -1) {
-        gst_element_seek_simple (player->playbin, format, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, pos);
+        gst_element_seek_simple (player->playbin, GST_FORMAT_BYTES, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, pos);
     }
 }
 
