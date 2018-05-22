#
# Makefile for MStar SAR device drivers.
#

ifeq ($(MAKE_TYPE), MODULE_STANDALONE)
include $(TOP_DIR)/modules.mk
endif

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/sar
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/sar

# specific options
EXTRA_CFLAGS      +=

# files
obj-$(CONFIG_MSTAR_SAR) += mdrv-sar.o
mdrv-sar-objs     += mdrv_sar.o mdrv_sar_io.o
mdrv-sar-objs     += ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/sar/mhal_sar.o

# export header files
EXPORT_H_FILES    += mdrv_sar_io.h