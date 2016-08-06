--- libbanshee/banshee-player-missing-elements.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-missing-elements.c
@@ -170,7 +170,7 @@ _bp_missing_elements_handle_state_change
     }
     #endif
     
-    install_return = gst_install_plugins_async (details, player->install_plugins_context, 
+    install_return = gst_install_plugins_async ((const gchar * const*) details, player->install_plugins_context, 
         bp_missing_elements_handle_install_result, player);
     
     if (install_return != GST_INSTALL_PLUGINS_STARTED_OK) {
