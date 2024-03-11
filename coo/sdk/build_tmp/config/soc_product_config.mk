user_panel := yes
PRODUCT_CFG_LOG_LEVEL := 1
PRODUCT_CFG_BUILD_RELEASE := yes
PRODUCT_CFG_CHIP_SOLUTION_NAME := "DW21"
PRODUCT_CFG_VERSION_STR := "V100R001C00SPC121B012"
PRODUCT_CFG_FULL_VERSION_STR := "DW21_V100R001C00SPC121B012"
PRODUCT_CFG_FILE_VERSION_STR := "2.0.1.0"
PRODUCT_CFG_BOOTLOADER_VERSION_STR := "0.0.6"
PRODUCT_CFG_BUILD_DATE := "2022-04-29"
PRODUCT_CFG_BUILD_TIME := "10:00:00"
PRODUCT_CFG_BUILD_DATA_TIME := "2022-04-29 10:00:00"
EXT_MODULE_MODULE_APP := yes
EXT_MODULE_ADC := yes
EXT_MODULE_SYS_COMMON := yes
EXT_MODULE_DRV_COLLECTDATA := yes
EXT_MODULE_DRV_DFX := yes
EXT_MODULE_FLASH_RAM := yes
EXT_MODULE_IO_CONFIG := yes
EXT_MODULE_PGA := yes
EXT_MODULE_PWM := yes
EXT_MODULE_RAW_TIMER := yes
EXT_MODULE_TSENSOR := yes
EXT_MODULE_UART := yes
EXT_MODULE_CPUP := yes
EXT_MODULE_NVM := yes
EXT_MODULE_PARTITION_TABLE := yes
EXT_MODULE_REMAP_USR := yes
EXT_MODULE_SYS := yes
overall := yes
REMAP_CFLAGS :=  -DUSE_REMAP -DREMAP_WITH_LRU -DREMAP_SECTOR_32K 
ROM_CFLAGS :=  -DUSE_ROM 
MAIN_CFLAGS :=  -nostdinc -falign-functions=4 -fno-aggressive-loop-optimizations -fno-builtin  -gdwarf-2 -std=c99 -Wpointer-arith -Wstrict-prototypes -mno-thumb-interwork  -ffunction-sections -fdata-sections -fno-exceptions -fstack-protector-strong  -fno-short-enums -fno-common -fno-delete-null-pointer-checks -fsigned-char -freg-struct-return -fvisibility=hidden -fno-strict-aliasing -pipe -mcpu=cortex-m3 -mthumb  -DCYGPKG_POSIX_SIGNALS  -D__ECOS__   -D__RTOS_ -O2 
WARNNING_CFLAGS :=  -W -Wall -Wundef -Werror -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow -Wtrampolines
MEM_DEBUG_CFLAGS :=  -D__MEM_HEAP_STAT_PATCH__ 
DEBUG_CFLAGS :=  -g 
MAIN_ASFLAGS :=  -mcpu=cortex-m3 -mthumb 
HB_SAVE_COMPILE_LOG_ENABLE := 
CFLAGS := $(MAIN_CFLAGS) $(WARNNING_CFLAGS) $(REMAP_CFLAGS) $(ROM_CFLAGS)
ASFLAGS := $(MAIN_ASFLAGS)
RM := rm -rf
MAKE := make
MV := mv
MAKE_DIR := mkdir -p
Q := @
ARFLAGS := cr
CROSS_COMPILE_DIR := $(MAIN_TOPDIR)/tools/toolchains/gcc-arm-none-eabi-10.3-2021.07/bin
CROSS_COMPILE := $(CROSS_COMPILE_DIR)/arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
EXT_MODULE_MBEDTLS := yes
EXT_MODULE_UPG_21 := yes
EXT_MODULE_UPG_TRANS_FILE := yes
EXT_MODULE_PLC_FREQ := yes
EXT_MODULE_NV := no
EXT_MODULE_DBK := no