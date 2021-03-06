$OpenBSD: patch-lib_dns_dst_openssl_h,v 1.1.2.1 2015/12/17 17:06:39 sthen Exp $
--- lib/dns/dst_openssl.h.orig	Wed Sep 16 14:00:47 2015
+++ lib/dns/dst_openssl.h	Wed Sep 16 14:02:42 2015
@@ -36,7 +36,7 @@
 #define USE_ENGINE 1
 #endif
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
 /*
  * These are new in OpenSSL 1.1.0.  BN_GENCB _cb needs to be declared in
  * the function like this before the BN_GENCB_new call:
