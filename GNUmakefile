ifndef MCU_SDK_TOP
$(error MCU_SDK_TOP must be set)
endif

ifndef SYSCFG_TOP
$(error SYSCFG_TOP must be set)
endif

# Compiler settings
TARGET_CC := arm-none-eabi-gcc
TARGET_AR := arm-none-eabi-ar

# Target library file name
SOC_LIB := libsoc.a

# Target linker script file name
LINKER_SCRIPT := solo5_frt.lds

# Parameters for library building
FILES_PATH_device = generated
FILES_device := \
	ti_board_config.c \
	ti_board_open_close.c \
	ti_dpl_config.c \
	ti_drivers_config.c \
	ti_drivers_open_close.c \
	ti_enet_config.c \
	ti_enet_open_close.c \
	ti_pinmux_config.c \
	ti_power_clock_config.c

FILES_PATH_enet = $(MCU_SDK_TOP)/examples/networking/lwip/enet_lwip_cpsw
FILES_enet := \
	test_enet.c \
	test_enet_cpsw.c

FILES_PATH_board = $(FILES_PATH_enet)/am64x-evm/r5fss0-0_freertos
FILES_board := \
	board.c

FILES_PATH_frt = frt
FILES_frt := \
	platform_am64x_binding.c \
	platform_am64x_tender.c

INCLUDES_device := \
	-I$(MCU_SDK_TOP)/source \
    -I$(MCU_SDK_TOP)/source/kernel/dpl \
    -I$(MCU_SDK_TOP)/source/kernel/freertos/dpl/common \
    -I$(MCU_SDK_TOP)/source/kernel/freertos/FreeRTOS-Kernel/include \
    -I$(MCU_SDK_TOP)/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F \
	-I$(MCU_SDK_TOP)/source/networking/enet \
	-I$(MCU_SDK_TOP)/source/networking/enet/utils/include \
	-I$(MCU_SDK_TOP)/source/networking/enet/utils/V3 \
	-I$(MCU_SDK_TOP)/source/networking/enet/core \
	-I$(MCU_SDK_TOP)/source/networking/enet/core/include/phy \
	-I$(MCU_SDK_TOP)/source/networking/enet/core/hw_include \
	-I$(MCU_SDK_TOP)/source/networking/enet/hw_include \
	-I$(MCU_SDK_TOP)/source/networking/enet/hw_include/mdio/V5 \
	-I$(MCU_SDK_TOP)/source/networking/lwip/lwip-stack/src/include \
	-I$(MCU_SDK_TOP)/source/networking/lwip/lwip-port/freertos/include \
	-I$(MCU_SDK_TOP)/source/networking/enet/core/lwipif/inc \
	-I$(MCU_SDK_TOP)/source/networking/lwip/lwip-contrib \
	-I$(MCU_SDK_TOP)/source/networking/lwip/lwip-config/am64x \
	-I$(MCU_SDK_TOP)/source/networking/lwip/lwip-port/freertos/include \
	-I$(MCU_SDK_TOP)/examples/networking/lwip/enet_lwip_cpsw/extPhyMgmt \
	-I./ \
	-Igenerated
DEFINES_device := -DSOC_AM64X -DENET_ENABLE_PER_CPSW=1
CFLAGS_device := \
	-march=armv7-r+vfpv3-d16 \
	-mfloat-abi=hard \
	-mfpu=vfpv3-d16 \
	-mthumb \
	-g \
	-Wno-gnu-variable-sized-type-not-at-end \
	-Wno-unused-function \
	-ffunction-sections  \
	-fdata-sections

CFLAGS_debug := -D_DEBUG_=1
FILES := $(FILES_device) $(FILES_enet) $(FILES_board) $(FILES_frt)
ASMFILES := $(ASMFILES_device) $(ASMFILES_enet) $(ASMFILES_board) $(ASMFILES_frt)
FILES_PATH := $(FILES_PATH_device) $(FILES_PATH_enet) $(FILES_PATH_board) $(FILES_PATH_frt)
CFLAGS := $(CFLAGS_device) $(CFLAGS_enet) $(CFLAGS_board) $(CFLAGS_frt)
DEFINES := $(DEFINES_device) $(DEFINES_enet) $(DEFINES_board) $(DEFINES_frt)
INCLUDES := $(INCLUDES_device) $(INCLUDES_enet) $(INCLUDES_board) $(INCLUDE_frt)

OBJDIR := obj/
OBJS := $(FILES:%.c=%.o)
OBJS += $(ASMFILES:%.S=%.o)
DEPS := $(FILES:%.c=%.d)

vpath %.o $(OBJDIR)
vpath %.c $(FILES_PATH)
vpath %.S $(FILES_PATH)

$(OBJDIR)/%.o %.o: %.c
	$(TARGET_CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) -MMD -o $(OBJDIR)/$@ $<

$(OBJDIR)/%.o %.o: %.S
	$(TARGET_CC) -c $(CFLAGS) -o $(OBJDIR)/$@ $<

all : $(SOC_LIB)

$(OBJS) : | $(OBJDIR)
$(OBJDIR) :
	mkdir $@

# Parameters for source file generation
SYSCFG_NODE := $(SYSCFG_TOP)/nodejs/node
SYSCFG_CLI_PATH := $(SYSCFG_TOP)
SYSCFG_NWJS := $(SYSCFG_PATH)/nw/nw
SYSCFG_SDKPRODUCT := $(MCU_SDK_TOP)/.metadata/product.json

SYSCFG_FILE := example.syscfg
ifeq ($(AM64X_SK),true)
$(SYSCFG_FILE) :
	cp $(FILES_PATH_enet)/am64x-evm/r5fss0-0_freertos/$(SYSCFG_FILE) ./$(SYSCFG_FILE)
	patch -p1 < ./patch/am64x-sk.patch
else
$(SYSCFG_FILE) :
	cp $(FILES_PATH_enet)/am64x-evm/r5fss0-0_freertos/$(SYSCFG_FILE) ./$(SYSCFG_FILE)
endif

SYSCFG_GEN_FILES=generated/ti_drivers_config.c generated/ti_drivers_config.h
SYSCFG_GEN_FILES+=generated/ti_drivers_open_close.c generated/ti_drivers_open_close.h
SYSCFG_GEN_FILES+=generated/ti_dpl_config.c generated/ti_dpl_config.h
SYSCFG_GEN_FILES+=generated/ti_pinmux_config.c generated/ti_power_clock_config.c
SYSCFG_GEN_FILES+=generated/ti_board_config.c generated/ti_board_config.h
SYSCFG_GEN_FILES+=generated/ti_board_open_close.c generated/ti_board_open_close.h
SYSCFG_GEN_FILES+=generated/ti_enet_config.c generated/ti_enet_config.h
SYSCFG_GEN_FILES+=generated/ti_enet_open_close.c generated/ti_enet_open_close.h
SYSCFG_GEN_FILES+=generated/ti_power_clock_config.c generated/ti_power_clock_config.h

.PHONY: all $(SOC_LIB) syscfg clean

.NOTPARALLEL:

# Library file
$(SOC_LIB) : syscfg $(SYSCFG_GEN_FILES) $(OBJS)
	@echo Generating the AM64x Cortex R5 specific library ...
	$(TARGET_AR) rc $@ $(addprefix $(OBJDIR), $(OBJS))

.INTERMEDIATE: syscfg
$(SYSCFG_GEN_FILES): syscfg $(LINKER_SCRIPT)

# Driver source files
syscfg: $(SYSCFG_FILE)
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --device AM64x_beta --context r5fss0-0 --part Default --package ALV --output generated/ $(SYSCFG_FILE)

# Linker script file
$(LINKER_SCRIPT): solo5_frt.lds.in
	gcc -E -P -x c $^ > $@

# Commands
clean:
	@echo  Cleaning: files generated by SysConfig
	rm -rf generated/
	rm -rf $(OBJDIR)
	rm -f $(SYSCFG_FILE)
	rm -f $(SOC_LIB)
	rm -f $(LINKER_SCRIPT)

