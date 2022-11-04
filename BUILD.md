# How to build your MirageOS unikernel

This document describes how to build your MirageOS unikernel running on top of a Cortex(r)-R5F core of your TI AM64x series processor.

## Prerequisites

### Install SDKs for AM64x

You need to install AM64x MCU+ SDK 08.03.00.18 and sysconfig-1.12.1_2446 on your Linux PC.

URL: https://www.ti.com/tool/download/MCU-PLUS-SDK-AM64X/08.03.00.18

You can find both installers named `mcu_plus_sdk_am64x_08_03_00_18-linux-x64-installer.run` and `sysconfig-1.12.1_2446-setup.run` there.

<u>Note that only x86_64 platforms are supported.</u>

### Build required libraries using GCC 10

Default GCC used for pre-built libraries is too old!

> GNU Tools for Arm Embedded Processors 7-2017-q4-major) 7.2.1 20170904 (release)

Therefore, re-compile libraries by using GCC 10.

### Install GCC 10  

URL: https://developer.arm.com/downloads/-/gnu-rm  
Get the the version 10.3-2021.10 (gcc-arm-none-eabi-10.3-2021.10).  
GCC 11 or later does not work due to an error on the interrupt attribute below.  
```
error: FP registers might be clobbered despite 'interrupt' attribute: compile with '-mgeneral-regs-only' [-Werror=attributes]
```

### GCC path setting

Change the default GCC path (CGT_GCC_ARM_PATH) in /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/imports.mak.  
```
...
#        CGT_GCC_ARM_PATH=$(TOOLS_PATH)/gcc-arm-none-eabi-7-2017-q4-major
        CGT_GCC_ARM_PATH=/path/to/gcc-arm-none-eabi-10.3-2021.10
...
```

### TI library header file modification

Add `(void)readBuf;` for the 2nd argument of `UART_getCharFifo` in `${MCU_SDK_TOP}/source/drivers/uart/v0/uart.h`.

```c
static inline uint8_t UART_getCharFifo(uint32_t baseAddr, uint8_t *readBuf)
{
    (void)readBuf; /* Newly added */
    ...
    ...
}
```

### Build libraries

(You can ignore building the ENET ICSSG driver and LWIP libraries below as the current Solo5 implementation does not any networking device)

```bash
(FreeRTOS library : change a heap scheme)
$ cd /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/source/kernel/freertos
$ sed -i s/heap_3/heap_1/ makefile.am64x.r5f.gcc-armv7
$ make -f makefile.am64x.r5f.gcc-armv7
```
```bash
(driver library)
$ cd /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/source/drivers
$ make -f makefile.am64x.r5f.gcc-armv7
```
```bash
(board library)
$ cd /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/source/board
$ make -f makefile.am64x.r5f.gcc-armv7
```

```bash
(ENET ICSSG dirver library)
$ cd /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/source/networking/enet
$ make -f makefile.icssg.am64x.r5f.ti-arm-clang # makefile for clang can be used for GCC
```

```bash
(LWIP libraries)
$ cd /path/to/ti/mcu_plus_sdk_am64x_08_03_00_18/source/networking/lwip
$ make -f makefile.contrib.am64x.r5f.ti-arm-clang # makefile for clang can be used for GCC
$ make -f makefile.stack.am64x.r5f.ti-arm-clang # makefile for clang can be used for GCC
```

### Set variables on your bash

```bash
$ export MCU_SDK_TOP=/path/to/ti/mcu_plus_sdk_am64x_08_03_00_18
$ export SYSCFG_TOP=/path/to/ti/sysconfig_1.12.1
```

## Register a repository for MCUs with opam

Follow [this instruction](https://github.com/TImada/mirage-embedded-repo/blob/main/HOWTO.md).

## Solo5/MirageOS installation

```bash
# Install the mirage package
$ opam install mirage

# Install the solo5-frt-am64x-r5 package for your target processor (target processor: "am64x-r5")
$ opam install solo5-frt-am64x-r5

# Install the Solo5 frt configuration package to select the processor "am64x-r5" as a target processor for the Solo5 frt
$ opam install conf-frt.am64x-r5
```

## Build your MirageOS unikernel

Finally, you are now ready to build your MirageOS unikernel. Let's build the hello world console sample!

```bash
# Get a collection of MirageOS unikernel samples
$ git clone https://github.com/mirage/mirage-skeleton
$ cd ./mirage-skeleton/device-usage/console

# Configure MirageOS so that it can use the Solo5 frt
$ mirage configure -t frt

# Compile the target unikernel (taking long time if this is the first compilation)
$ make
```

## Tips

### System configuration by syscfg

This package uses `mirageos.syscfg` for system configuration to enable UART0 and ICSSG.
