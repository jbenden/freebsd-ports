# Created by: Jason Bacon <bacon4000@gmail.com>
# $FreeBSD$

PORTNAME=	qterminal
PORTVERSION=	0.6.0
CATEGORIES=	x11

MAINTAINER=	bacon4000@gmail.com
COMMENT=	Lightweight Qt-based terminal emulator

LICENSE=	GPLv2

LIB_DEPENDS+=	libqtermwidget5.so:x11-toolkits/qtermwidget

USES=		cmake
USE_XORG+=	x11
USE_QT5+=	core gui widgets x11extras linguist buildtools_build qmake_build

CMAKE_ARGS+=	-DUSE_SYSTEM_QXT=0 -DUSE_QT5=1

USE_GITHUB=	yes
GH_ACCOUNT=	lxde

LDFLAGS+=	-L${LOCALBASE}/lib -lulog

.include <bsd.port.mk>
