# Created by: vanilla
# $FreeBSD$

PORTNAME=	qrencode
PORTVERSION=	0.4
PORTREVISION=	1
CATEGORIES=	graphics pear
MASTER_SITES=	LOCAL/vanilla
PKGNAMEPREFIX=	pecl-
DIST_SUBDIR=	PECL

MAINTAINER=	vanilla@FreeBSD.org
COMMENT=	PECL extension for generate qrcode

LIB_DEPENDS=	libqrencode.so:graphics/libqrencode

USES=	php:ext tar:bzip2
IGNORE_WITH_PHP=	70

CONFIGURE_ARGS=	--with-qrencode=${LOCALBASE}
LDFLAGS+=	-L${LOCALBASE}/lib ${EXTRA_LIBS}
CFLAGS+=	-I${LOCALBASE}/include

.include <bsd.port.mk>
