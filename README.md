# solo5-frt-am64x-r5

This package provides a variety of files required to build a MirageOS unikernel with Solo5 running on top of a Cortex(r)-R5 core of your TI AM64x processor.

## How to build your MirageOS unikernel

Follow instructions described in [BUILD.md](./BUILD.md).

## What is done during installation of this package

(in GNUMakefile)

1. Generate AM64x device code files (.c and .h) from its SDKs
2. Compile the code files above then build a static library `libsoc.a`

(in install.sh)

3. Copy `Makefile.soc` and `solo5_frt.lds` to `$HOME/.opam/$(opam var prefix)/solo5-frt-am64x-r5`
4. Copy `FreeRTOSConfig.h`, `platform_binding.h`, and `platform_tender.h` to `$HOME/.opam/$(opam var prefix)/solo5-frt-am64x-r5/include`
5. Copy `libsoc.a` to `$HOME/.opam/$(opam var prefix)/solo5-frt-am64x-r5/lib`

See also https://github.com/TImada/solo5-embedded for more information.
