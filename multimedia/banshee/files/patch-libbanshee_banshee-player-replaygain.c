--- libbanshee/banshee-player-replaygain.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-replaygain.c
@@ -79,17 +79,13 @@ static void on_target_gain_changed (GstE
     _bp_rgvolume_print_volume (player);
 }
 
-static void
-pad_block_cb (GstPad *srcPad, gboolean blocked, gpointer user_data)
+static GstPadProbeReturn
+pad_block_cb (GstPad *srcPad, GstPadProbeInfo *info, gpointer user_data)
 {
     BansheePlayer* player;
 
-    if (!blocked) {
-        return;
-    }
-
     player = (BansheePlayer*) user_data;
-    g_return_if_fail (IS_BANSHEE_PLAYER (player));
+    g_return_val_if_fail (IS_BANSHEE_PLAYER (player), GST_PAD_PROBE_OK);
 
     // The pad_block_cb can get triggered multiple times, on different threads.
     // Lock around the link/unlink code, so we don't end up going through here
@@ -99,11 +95,9 @@ pad_block_cb (GstPad *srcPad, gboolean b
     if ((player->replaygain_enabled && player->rgvolume_in_pipeline) ||
         (!player->replaygain_enabled && !player->rgvolume_in_pipeline)) {
         // The pipeline is already in the correct state.  Unblock the pad, and return.
+        player->rg_pad_block_id = 0;
         g_mutex_unlock (player->replaygain_mutex);
-        if (gst_pad_is_blocked (srcPad)) {
-            gst_pad_set_blocked_async (srcPad, FALSE, &pad_block_cb, player);
-        }
-        return;
+        return GST_PAD_PROBE_REMOVE;
     }
 
     if (player->rgvolume_in_pipeline) {
@@ -139,13 +133,12 @@ pad_block_cb (GstPad *srcPad, gboolean b
     }
 
     // Our state is now consistent
+    player->rg_pad_block_id = 0;
     g_mutex_unlock (player->replaygain_mutex);
 
-    if (gst_pad_is_blocked (srcPad)) {
-        gst_pad_set_blocked_async (srcPad, FALSE, &pad_block_cb, player);
-    }
-
     _bp_rgvolume_print_volume (player);
+
+    return GST_PAD_PROBE_REMOVE;
 }
 
 // ---------------------------------------------------------------------------
@@ -187,9 +180,9 @@ void _bp_replaygain_pipeline_rebuild (Ba
     srcPad = gst_element_get_static_pad (player->before_rgvolume, "src");
 
     if (gst_pad_is_active (srcPad) && !gst_pad_is_blocked (srcPad)) {
-        gst_pad_set_blocked_async (srcPad, TRUE, &pad_block_cb, player);
-    } else if (srcPad->block_callback == NULL) {
-        pad_block_cb (srcPad, TRUE, player);
+        player->rg_pad_block_id = gst_pad_add_probe (srcPad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, &pad_block_cb, player, NULL);
+    } else if (!player->rg_pad_block_id) {
+        pad_block_cb (srcPad, NULL, player);
     }
 }
 
