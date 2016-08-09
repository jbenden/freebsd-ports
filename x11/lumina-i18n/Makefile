# Created by: Ken Moore <ken@pcbsd.org>
# $FreeBSD$

PORTNAME=	lumina-i18n
PORTVERSION=	1.0.0
DISTVERSIONPREFIX=	v
DISTVERSIONSUFFIX=	-Release
#PORTREVISION=	1
CATEGORIES=	x11

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	Lumina desktop environment localization files

LICENSE=	BSD3CLAUSE

RUN_DEPENDS=	lumina>=0:x11/lumina

USES=		tar:xz
USE_QT5=	linguisttools_build

USE_GITHUB=	yes
GH_ACCOUNT=	trueos

NO_BUILD=	yes

do-install:
	@cd ${WRKSRC}/scripts && ./mkrelease.sh
	${MKDIR} ${STAGEDIR}${PREFIX}/share/lumina-desktop/i18n
	tar xvf ${WRKSRC}/dist/${PORTNAME}.txz -C ${STAGEDIR}${PREFIX}/share/lumina-desktop/i18n

.include <bsd.port.mk>
