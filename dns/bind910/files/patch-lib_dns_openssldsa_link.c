$OpenBSD: patch-lib_dns_openssldsa_link_c,v 1.1.2.1 2015/12/17 17:06:39 sthen Exp $
--- lib/dns/openssldsa_link.c.orig	Wed Sep 16 14:01:23 2015
+++ lib/dns/openssldsa_link.c	Wed Sep 16 14:02:22 2015
@@ -359,7 +359,7 @@ openssldsa_generate(dst_key_t *key, int unused, void (
 	isc_result_t result;
 #if OPENSSL_VERSION_NUMBER > 0x00908000L
 	BN_GENCB *cb;
-#if OPENSSL_VERSION_NUMBER < 0x10100000L
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
 	BN_GENCB _cb;
 #endif
 	union {
@@ -383,7 +383,7 @@ openssldsa_generate(dst_key_t *key, int unused, void (
 	if (dsa == NULL)
 		return (dst__openssl_toresult(DST_R_OPENSSLFAILURE));
 	cb = BN_GENCB_new();
-#if OPENSSL_VERSION_NUMBER >= 0x10100000L
+#if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
 	if (cb == NULL) {
 		DSA_free(dsa);
 		return (dst__openssl_toresult(DST_R_OPENSSLFAILURE));
