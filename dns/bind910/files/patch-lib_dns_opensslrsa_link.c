$OpenBSD: patch-lib_dns_opensslrsa_link_c,v 1.2.2.1 2015/12/17 17:06:39 sthen Exp $
--- lib/dns/opensslrsa_link.c.orig	Sun Dec  6 12:37:44 2015
+++ lib/dns/opensslrsa_link.c	Thu Dec 17 17:00:19 2015
@@ -773,7 +773,7 @@ opensslrsa_generate(dst_key_t *key, int exp, void (*ca
 	} u;
 	RSA *rsa = RSA_new();
 	BIGNUM *e = BN_new();
-#if OPENSSL_VERSION_NUMBER < 0x10100000L
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
 	BN_GENCB _cb;
 #endif
 	BN_GENCB *cb = BN_GENCB_new();
