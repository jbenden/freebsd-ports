--- libbanshee/banshee-tagger.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-tagger.c
@@ -69,7 +69,7 @@ bt_tag_list_foreach (const GstTagList *l
 GstTagList *
 bt_tag_list_new ()
 {
-    return gst_tag_list_new ();
+    return gst_tag_list_new_empty ();
 }
 
 void
