# $FreeBSD$

KMOD=	lkey
SRCS=	lkey.c imageFile_key.h

##
## TBD: JRH - may want to test with relative paths, to avoid assuming /usr/src/ location
##      Also, note dependency on xxd (not part of base freebsd, required on build host)
##
imageFile_key.h: /usr/src/priv/imageFile.key
	(cd /usr/src/priv/; /usr/local/bin/xxd -i imageFile.key) > ${.CURDIR}/imageFile_key.h

.include <bsd.kmod.mk>
