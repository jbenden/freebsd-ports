# Created by: Joseph Benden <joe@thrallingpenguin.com>
# $FreeBSD$

PORTNAME=	flexelint
PORTVERSION=	8.00x
CATEGORIES=	devel

MAINTAINER=	joe@thrallingpenguin.com
COMMENT=	Static Source Code Analyizer for C and C++

LICENSE=	FLEXELINT
LICENSE_NAME=	Flexelint Commercial License
LICENSE_TEXT=	License is included with the purchase of a license.
LICENSE_PERMS=	none
NOFETCHFILES=	yes
RESTRICTED=	Commercial software license prevents all distribution

NO_TEST=	yes
NO_INSTALL=	yes

CC?=	clang
CFLAGS+=	-Wno-parentheses -Wno-unused-value -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types-discards-qualifiers

.include <bsd.port.pre.mk>

.if !exists(${DISTDIR}/${DISTNAME}${EXTRACT_SUFX})
IGNORE=	may not be redistributed because of licensing reasons. Please visit http://www.gimple.com/ to accept their license and download ${DISTFILES} into ${DISTDIR}
.endif

build:
	cd ${WRKSRC} && ${CC} ${CFLAGS} -o flexelint a*.c custom.c

post-install:
	cd ${WRKSRC} && ${INSTALL} -m 0755 flexelint ${STAGEDIR}${PREFIX}/bin/flexelint
	${STRIP_CMD} ${STAGEDIR}${PREFIX}/bin/flexelint

.include <bsd.port.post.mk>
