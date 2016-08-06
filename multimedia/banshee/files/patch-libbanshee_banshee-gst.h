--- libbanshee/banshee-gst.h.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-gst.h
@@ -41,9 +41,11 @@
 #define BANSHEE_GST_ITERATOR_ITERATE(iter,child_type,child_name,free,block) { \
     gboolean iter##_done = FALSE; \
     while (!iter##_done) { \
+        GValue child_value = {0, }; \
         child_type child_name; \
-        switch (gst_iterator_next (iter, (gpointer)&child_name)) { \
+        switch (gst_iterator_next (iter, &child_value)) { \
             case GST_ITERATOR_OK: { \
+                child_name = (child_type) g_value_get_object(&child_value); \
                 { block; } \
                 break; \
             } \
