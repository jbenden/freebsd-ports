reated by: Toni Gundogdu <legatvs@gmail.com>
# $FreeBSD$

PORTNAME=	quvi
PORTVERSION=	0.4.2
PORTREVISION=	2
CATEGORIES=	multimedia devel
MASTER_SITES=	SF/${PORTNAME}/${PORTVERSION:R}/${PORTNAME}/

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Command line tool for parsing flash media stream URLs

LICENSE=	LGPL21 LGPL3
LICENSE_COMB=	dual

BUILD_DEPENDS=	libquvi>=0.4.0:multimedia/libquvi
LIB_DEPENDS=	libcurl.so:ftp/curl
RUN_DEPENDS:=	${BUILD_DEPENDS}

PORTSCOUT=	limit:0.4.[0-9]*

GNU_CONFIGURE=	yes
USES=		pkgconfig tar:xz

PLIST_FILES=	bin/quvi \
		man/man1/quvi.1.gz

.include <bsd.port.mk>
