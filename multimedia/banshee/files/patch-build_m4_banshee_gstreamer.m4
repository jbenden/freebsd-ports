--- build/m4/banshee/gstreamer.m4.orig	2014-02-18 20:40:14 UTC
+++ build/m4/banshee/gstreamer.m4
@@ -1,17 +1,20 @@
 AC_DEFUN([BANSHEE_CHECK_GSTREAMER],
 [
-	GSTREAMER_REQUIRED_VERSION=0.10.26
+	GSTREAMER_REQUIRED_VERSION=1.2.0
 	AC_SUBST(GSTREAMER_REQUIRED_VERSION)
 
 	PKG_CHECK_MODULES(GST,
-		gstreamer-0.10 >= $GSTREAMER_REQUIRED_VERSION
-		gstreamer-base-0.10 >= $GSTREAMER_REQUIRED_VERSION
-		gstreamer-plugins-base-0.10 >= $GSTREAMER_REQUIRED_VERSION
-		gstreamer-controller-0.10 >= $GSTREAMER_REQUIRED_VERSION
-		gstreamer-dataprotocol-0.10 >= $GSTREAMER_REQUIRED_VERSION
-		gstreamer-fft-0.10 >= $GSTREAMER_REQUIRED_VERSION)
+		gstreamer-1.0 >= $GSTREAMER_REQUIRED_VERSION
+		gstreamer-base-1.0 >= $GSTREAMER_REQUIRED_VERSION
+		gstreamer-plugins-base-1.0 >= $GSTREAMER_REQUIRED_VERSION
+		gstreamer-controller-1.0 >= $GSTREAMER_REQUIRED_VERSION
+        gstreamer-audio-1.0 >= $GSTREAMER_REQUIRED_VERSION
+		gstreamer-pbutils-1.0 >= $GSTREAMER_REQUIRED_VERSION
+        gstreamer-tag-1.0 >= $GSTREAMER_REQUIRED_VERSION
+        gstreamer-video-1.0 >= $GSTREAMER_REQUIRED_VERSION
+		gstreamer-fft-1.0 >= $GSTREAMER_REQUIRED_VERSION)
 
-	GST_LIBS="$GST_LIBS -lgstvideo-0.10 -lgstinterfaces-0.10 -lgstcdda-0.10 -lgstpbutils-0.10 -lgsttag-0.10"
+	GST_LIBS="$GST_LIBS -lgstvideo-1.0 -lgstpbutils-1.0 -lgsttag-1.0"
 
 	AC_SUBST(GST_CFLAGS)
 	AC_SUBST(GST_LIBS)
