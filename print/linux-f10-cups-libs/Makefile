# Created by: Stas Timokhin <devel@stasyan.com>
# $FreeBSD: head/print/linux-f10-cups-libs/Makefile 340722 2014-01-22 17:00:46Z mat $

PORTNAME=	cups-libs
PORTVERSION=	1.3.11
PORTREVISION=	1
CATEGORIES=	print linux

MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Common UNIX Printing System libraries

USE_LINUX=	f10
USE_LINUX_RPM=	yes
USE_LINUX_APPS=	gnutls
SRC_DISTFILES=	cups-${PORTVERSION}-${RPMVERSION}.src.rpm
RPMVERSION=	4.fc10
USE_LDCONFIG=	yes

PLIST_FILES=	usr/lib/libcups.so.2 usr/lib/libcupsimage.so.2
DESCR=		${.CURDIR}/../cups/pkg-descr

.include <bsd.port.mk>
