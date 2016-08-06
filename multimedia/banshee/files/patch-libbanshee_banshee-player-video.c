--- libbanshee/banshee-player-video.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-video.c
@@ -37,54 +37,54 @@
 #if defined(GDK_WINDOWING_X11) || defined(GDK_WINDOWING_WIN32)
 
 static gboolean
-bp_video_find_xoverlay (BansheePlayer *player)
+bp_video_find_video_overlay (BansheePlayer *player)
 {
     GstElement *video_sink = NULL;
-    GstElement *xoverlay;
-    GstXOverlay *previous_xoverlay;
-    gboolean    found_xoverlay;
+    GstElement *video_overlay;
+    GstVideoOverlay *previous_video_overlay;
+    gboolean found_video_overlay;
 
     g_object_get (player->playbin, "video-sink", &video_sink, NULL);
 
     g_mutex_lock (player->video_mutex);
-    previous_xoverlay = player->xoverlay;
+    previous_video_overlay = player->video_overlay;
 
     if (video_sink == NULL) {
-        player->xoverlay = NULL;
-        if (previous_xoverlay != NULL) {
-            gst_object_unref (previous_xoverlay);
+        player->video_overlay = NULL;
+        if (previous_video_overlay != NULL) {
+            gst_object_unref (previous_video_overlay);
         }
         g_mutex_unlock (player->video_mutex);
         return FALSE;
     }
-   
-    xoverlay = GST_IS_BIN (video_sink)
-        ? gst_bin_get_by_interface (GST_BIN (video_sink), GST_TYPE_X_OVERLAY)
+
+    video_overlay = GST_IS_BIN (video_sink)
+        ? gst_bin_get_by_interface (GST_BIN (video_sink), GST_TYPE_VIDEO_OVERLAY) 
         : video_sink;
-    
-    player->xoverlay = GST_IS_X_OVERLAY (xoverlay) ? GST_X_OVERLAY (xoverlay) : NULL;
-    
-    if (previous_xoverlay != NULL) {
-        gst_object_unref (previous_xoverlay);
+
+    player->video_overlay = GST_IS_VIDEO_OVERLAY (video_overlay) ? GST_VIDEO_OVERLAY (video_overlay) : NULL;
+
+    if (previous_video_overlay != NULL) {
+        gst_object_unref (previous_video_overlay);
     }
-        
+
 #if !defined(GDK_WINDOWING_WIN32) // We can't rely on aspect ratio from dshowvideosink
-    if (player->xoverlay != NULL && g_object_class_find_property (
-        G_OBJECT_GET_CLASS (player->xoverlay), "force-aspect-ratio")) {
-        g_object_set (G_OBJECT (player->xoverlay), "force-aspect-ratio", TRUE, NULL);
+    if (player->video_overlay != NULL && g_object_class_find_property (
+        G_OBJECT_GET_CLASS (player->video_overlay), "force-aspect-ratio")) {
+        g_object_set (G_OBJECT (player->video_overlay), "force-aspect-ratio", TRUE, NULL);
     }
 #endif
     
-    if (player->xoverlay != NULL && g_object_class_find_property (
-        G_OBJECT_GET_CLASS (player->xoverlay), "handle-events")) {
-        g_object_set (G_OBJECT (player->xoverlay), "handle-events", FALSE, NULL);
+    if (player->video_overlay != NULL && g_object_class_find_property (
+        G_OBJECT_GET_CLASS (player->video_overlay), "handle-events")) {
+        g_object_set (G_OBJECT (player->video_overlay), "handle-events", FALSE, NULL);
     }
 
     gst_object_unref (video_sink);
-    found_xoverlay = (player->xoverlay != NULL) ? TRUE : FALSE;
+    found_video_overlay = (player->video_overlay != NULL) ? TRUE : FALSE;
 
     g_mutex_unlock (player->video_mutex);
-    return found_xoverlay;
+    return found_video_overlay;
 }
 
 #endif /* GDK_WINDOWING_X11 || GDK_WINDOWING_WIN32 */
@@ -130,27 +130,27 @@ bp_video_sink_element_added (GstBin *vid
     g_return_if_fail (IS_BANSHEE_PLAYER (player));
 
     #if defined(GDK_WINDOWING_X11) || defined(GDK_WINDOWING_WIN32)
-    bp_video_find_xoverlay (player);
+    bp_video_find_video_overlay (player);
     #endif
 }
 
 static void
 bp_video_bus_element_sync_message (GstBus *bus, GstMessage *message, BansheePlayer *player)
 {
-    gboolean found_xoverlay;
+    gboolean found_video_overlay;
     
     g_return_if_fail (IS_BANSHEE_PLAYER (player));
 
     #if defined(GDK_WINDOWING_X11) || defined(GDK_WINDOWING_WIN32)
 
-    if (message->structure == NULL || !gst_structure_has_name (message->structure, "prepare-xwindow-id")) {
+    if (!gst_is_video_overlay_prepare_window_handle_message (message)) {
         return;
     }
 
-    found_xoverlay = bp_video_find_xoverlay (player);
+    found_video_overlay = bp_video_find_video_overlay (player);
 
-    if (found_xoverlay) {
-        gst_x_overlay_set_xwindow_id (player->xoverlay, player->video_window_xid);
+    if (found_video_overlay) {
+        gst_video_overlay_set_window_handle (player->video_overlay, player->video_window_xid);
     }
 
     #endif
@@ -164,7 +164,7 @@ static void
 cb_caps_set (GObject *obj, GParamSpec *pspec, BansheePlayer *p)
 {
     GstStructure * s = NULL;
-    GstCaps * caps = gst_pad_get_negotiated_caps (GST_PAD (obj));
+    GstCaps * caps = gst_pad_get_current_caps (GST_PAD (obj));
 
     if (G_UNLIKELY (!caps)) {
         return;
@@ -219,7 +219,7 @@ _bp_parse_stream_info (BansheePlayer *pl
     }
 
     if (G_LIKELY (vpad)) {
-        GstCaps *caps = gst_pad_get_negotiated_caps (vpad);
+        GstCaps *caps = gst_pad_get_current_caps (vpad);
         if (G_LIKELY (caps)) {
             cb_caps_set (G_OBJECT (vpad), NULL, player);
             gst_caps_unref (caps);
@@ -249,21 +249,18 @@ _bp_video_pipeline_setup (BansheePlayer 
 
     player->video_display_context_type = BP_VIDEO_DISPLAY_CONTEXT_GDK_WINDOW;
     
-    videosink = gst_element_factory_make ("gconfvideosink", "videosink");
+    videosink = gst_element_factory_make ("autovideosink", "videosink");
     if (videosink == NULL) {
-        videosink = gst_element_factory_make ("autovideosink", "videosink");
-        if (videosink == NULL) {
-            player->video_display_context_type = BP_VIDEO_DISPLAY_CONTEXT_UNSUPPORTED;
-            videosink = gst_element_factory_make ("fakesink", "videosink");
-            if (videosink != NULL) {
-                g_object_set (G_OBJECT (videosink), "sync", TRUE, NULL);
-            }
+        player->video_display_context_type = BP_VIDEO_DISPLAY_CONTEXT_UNSUPPORTED;
+        videosink = gst_element_factory_make ("fakesink", "videosink");
+        if (videosink != NULL) {
+            g_object_set (G_OBJECT (videosink), "sync", TRUE, NULL);
         }
     }
     
     g_object_set (G_OBJECT (player->playbin), "video-sink", videosink, NULL);
     
-    gst_bus_set_sync_handler (bus, gst_bus_sync_signal_handler, player);
+    gst_bus_set_sync_handler (bus, gst_bus_sync_signal_handler, player, NULL);
     g_signal_connect (bus, "sync-message::element", G_CALLBACK (bp_video_bus_element_sync_message), player);
         
     if (GST_IS_BIN (videosink)) {
@@ -349,21 +346,21 @@ bp_video_window_expose (BansheePlayer *p
 {
     g_return_if_fail (IS_BANSHEE_PLAYER (player));
     
-    if (direct && player->xoverlay != NULL && GST_IS_X_OVERLAY (player->xoverlay)) {
-        gst_x_overlay_expose (player->xoverlay);
+    if (direct && player->video_overlay != NULL && GST_IS_VIDEO_OVERLAY (player->video_overlay)) {
+        gst_video_overlay_expose (player->video_overlay);
         return;
     }
 
-    if (player->xoverlay == NULL && !bp_video_find_xoverlay (player)) {
+    if (player->video_overlay == NULL && !bp_video_find_video_overlay (player)) {
         return;
     }
     
-    gst_object_ref (player->xoverlay);
+    gst_object_ref (player->video_overlay);
 
-    gst_x_overlay_set_xwindow_id (player->xoverlay, player->video_window_xid);
-    gst_x_overlay_expose (player->xoverlay);
+    gst_video_overlay_set_window_handle (player->video_overlay, player->video_window_xid);
+    gst_video_overlay_expose (player->video_overlay);
 
-    gst_object_unref (player->xoverlay);
+    gst_object_unref (player->video_overlay);
 }
 
 // MUST be called from the GTK main thread; calling it in OnRealized will do the right thing.
@@ -379,7 +376,7 @@ bp_video_window_realize (BansheePlayer *
 //    //Explicitly create the native window.  GDK_WINDOW_XWINDOW will call this
 //    //function anyway, but this way we can raise a more useful message should it fail.
 //    if (!gdk_window_ensure_native (window)) {
-//        banshee_log (BANSHEE_LOG_TYPE_ERROR, "player-video", "Couldn't create native window needed for GstXOverlay!");
+//        banshee_log (BANSHEE_LOG_TYPE_ERROR, "player-video", "Couldn't create native window needed for GstVideoOverlay!");
 //    }
 //#endif
 
