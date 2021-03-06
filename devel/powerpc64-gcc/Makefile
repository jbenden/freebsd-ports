# $FreeBSD$

PORTNAME=	gcc
PORTVERSION=	5.3.0
CATEGORIES=	devel
MASTER_SITES=	GCC/releases/gcc-${DISTVERSION}
PKGNAMEPREFIX?=	powerpc64-

MAINTAINER=	bapt@FreeBSD.org
COMMENT=	Cross GNU Compiler Collection for ${PKGNAMEPREFIX:C/-//g}

LICENSE=	GPLv3 GPLv3RLE
LICENSE_COMB=	multi

USES=	gmake iconv libtool tar:bzip2 makeinfo compiler
PLIST_SUB=	TARGETARCH=${PKGNAMEPREFIX:C/-//g} \
		OPSYS=${OPSYS:tl} \
		GCC_TARGET=${GCC_TARGET} \
		GCC_VERSION=${PORTVERSION}

LIB_DEPENDS=	libgmp.so:math/gmp \
		libmpfr.so:math/mpfr \
		libmpc.so:math/mpc
BUILD_DEPENDS=	${BU_PREFIX}-as:devel/${PKGNAMEPREFIX}binutils
RUN_DEPENDS=	${BU_PREFIX}-as:devel/${PKGNAMEPREFIX}binutils

.if empty(GCC_TARGET)
# We are building for a FreeBSD target
GCC_TARGET?=	${PKGNAMEPREFIX}portbld-${OPSYS:tl}${OSREL}
BU_PREFIX?=	${PKGNAMEPREFIX}${OPSYS:tl}
EXTRA_PATCHES+= ${FILESDIR}/freebsd-format-extensions
.else
BU_PREFIX?=	${GCC_TARGET}
.if defined(FREEBSD_EXTENSION)
EXTRA_PATCHES+= ${FILESDIR}/freebsd-format-extensions
.endif
.endif

CONFIGURE_ENV=	LC_ALL=C
MAKE_ENV=	LC_ALL=C
GNU_CONFIGURE=	yes
CONFIGURE_OUTSOURCE=	yes
CONFIGURE_ARGS+=--target=${GCC_TARGET} --disable-nls --enable-languages=c,c++ \
		--without-headers \
		--with-gmp=${LOCALBASE} \
		--with-pkgversion="FreeBSD Ports Collection for ${PKGNAMEPREFIX:C/-//g}" \
		--with-system-zlib \
		--with-as=${LOCALBASE}/bin/${BU_PREFIX}-as \
		--with-ld=${LOCALBASE}/bin/${BU_PREFIX}-ld

ALL_TARGET?=	all-gcc
INSTALL_TARGET?=	install-gcc
BROKEN_FreeBSD9=	plist is different on 9.x that on other arches

.include <bsd.port.options.mk>

.if ${ARCH} == "amd64"
CONFIGURE_TARGET=	x86_64-portbld-${OPSYS:tl}${OSREL}
.else
CONFIGURE_TARGET=	${ARCH}-portbld-${OPSYS:tl}${OSREL}
.endif

post-install:
	@if [ -f ${STAGEDIR}${PREFIX}/bin/cpp ] ; then \
		mv ${STAGEDIR}${PREFIX}/bin/cpp  \
		    ${STAGEDIR}${PREFIX}/bin/${GCC_TARGET}-cpp ; \
	fi
.for f in c++ cpp g++ gcc gcc-ar gcc-nm gcc-ranlib gcov gcov-tool
	@${RM} ${STAGEDIR}${PREFIX}/bin/$f
	@${RM} ${STAGEDIR}${PREFIX}/man/man1/$f.1
	@${RM} ${STAGEDIR}${PREFIX}/man/man1/$f.1.gz
.endfor
	@${RM} ${STAGEDIR}${PREFIX}/info/*
	@${RM} ${STAGEDIR}${PREFIX}/man/man7/*

.include <bsd.port.mk>
