--- libbanshee/banshee-player-private.h.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-private.h
@@ -42,14 +42,14 @@
 #include <gst/fft/gstfftf32.h>
 #include <gst/pbutils/pbutils.h>
 #include <gst/tag/tag.h>
-#include <gst/interfaces/navigation.h>
+#include <gst/video/navigation.h>
 
 #if defined(GDK_WINDOWING_X11)
 #  include <gdk/gdkx.h>
-#  include <gst/interfaces/xoverlay.h>
+#  include <gst/video/videooverlay.h>
 #elif defined(GDK_WINDOWING_WIN32)
 #  include <gdk/gdkwin32.h>
-#  include <gst/interfaces/xoverlay.h>
+#  include <gst/video/videooverlay.h>
 #endif
 
 #include "banshee-gst.h"
@@ -155,11 +155,11 @@ struct BansheePlayer {
     // Video State
     BpVideoDisplayContextType video_display_context_type;
     #if defined(GDK_WINDOWING_X11)
-    GstXOverlay *xoverlay;
+    GstVideoOverlay *video_overlay;
     GdkWindow *video_window;
     XID video_window_xid;
     #elif defined(GDK_WINDOWING_WIN32)
-    GstXOverlay *xoverlay;
+    GstVideoOverlay *video_overlay;
     GdkWindow *video_window;
     HWND video_window_xid;
     #endif
@@ -179,6 +179,8 @@ struct BansheePlayer {
     GstFFTF32 *vis_fft;
     GstFFTF32Complex *vis_fft_buffer;
     gfloat *vis_fft_sample_buffer;
+    GstPad *vis_event_probe_pad;
+    gulong vis_event_probe_id;
     
     // Plugin Installer State
     GdkWindow *window;
@@ -197,6 +199,7 @@ struct BansheePlayer {
     // http://replaygain.hydrogenaudio.org/player_scale.html
     gdouble rg_gain_history[10];
     gint history_size;
+    gulong rg_pad_block_id;
 
     //dvd navigation
     GstNavigation *navigation;
