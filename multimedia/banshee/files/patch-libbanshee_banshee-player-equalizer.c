--- libbanshee/banshee-player-equalizer.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-equalizer.c
@@ -122,7 +122,7 @@ bp_equalizer_set_gain (BansheePlayer *pl
     g_return_if_fail (IS_BANSHEE_PLAYER (player));
     
     if (player->equalizer != NULL) {
-        GstObject *band;
+        GObject *band;
 
         g_return_if_fail (bandnum < gst_child_proxy_get_children_count (GST_CHILD_PROXY (player->equalizer)));
 
@@ -188,7 +188,7 @@ bp_equalizer_get_frequencies (BansheePla
     count = gst_child_proxy_get_children_count (GST_CHILD_PROXY (player->equalizer));
     
     for (i = 0; i < count; i++) {
-        GstObject *band;
+        GObject *band;
         
         band = gst_child_proxy_get_child_by_index (GST_CHILD_PROXY (player->equalizer), i);
         g_object_get (G_OBJECT (band), "freq", &(*freq)[i], NULL);
