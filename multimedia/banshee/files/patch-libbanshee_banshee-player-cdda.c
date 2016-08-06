--- libbanshee/banshee-player-cdda.c.orig	2014-02-18 20:40:14 UTC
+++ libbanshee/banshee-player-cdda.c
@@ -28,7 +28,8 @@
 // WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 //
 
-#include <gst/cdda/gstcddabasesrc.h>
+#include <stdlib.h>
+#include <gst/audio/gstaudiosrc.h>
 #include "banshee-player-cdda.h"
 
 // ---------------------------------------------------------------------------
@@ -46,7 +47,7 @@ bp_cdda_get_cdda_source (GstElement *pla
     
     g_object_get (playbin, "source", &source, NULL);
     
-    if (source == NULL || !GST_IS_CDDA_BASE_SRC (source)) {
+    if (source == NULL || !GST_IS_AUDIO_CD_SRC (source)) {
         if (source != NULL) {
             g_object_unref (source);
         }
