#! /bin/sh

prefix=${1:-$PREFIX}
if [ "$prefix" = "" ]; then
    prefix=`opam var prefix`
fi

DESTDIR=${prefix}/solo5-frt-am64x-r5
mkdir -p ${DESTDIR}
mkdir ${DESTDIR}/include ${DESTDIR}/lib

# Copy the SoC specific Makefile
cp Makefile.soc ${DESTDIR}

# Copy the linker file
cp solo5_frt.lds ${DESTDIR}

# Copy header files
cp FreeRTOSConfig.h platform_*.h generated/*.h ${DESTDIR}/include

# Copy the library file
cp -r libsoc.a ${DESTDIR}/lib
