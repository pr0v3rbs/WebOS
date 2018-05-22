#
# Makefile for MStar VIF device drivers.
#

# dhjung LGE
ifeq ($(MAKE_TYPE), MODULE_STANDALONE)
include $(TOP_DIR)/modules.mk
endif

# dhjung LGE
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/vif
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/vif

# specific options
#EXTRA_CFLAGS      +=  -Iinclude/asm-mips/titania
EXTRA_CFLAGS      +=  -DTITANIA
EXTRA_CFLAGS      +=  -DMSOS_TYPE_LINUX

# files
##dhjung LGE obj-$(CONFIG_MSTAR_VIF)             += mdrv_vif.o
##dhjung LGE obj-$(CONFIG_MSTAR_VIF)             += mdrv_vif_io.o
##dhjung LGE obj-$(CONFIG_MSTAR_VIF)             += $(CONFIG_MSTAR_CHIP_NAME)/

# dhjung LGE
obj-$(CONFIG_MSTAR_VIF)	+= mdrv-vif.o
mdrv-vif-objs			+= mdrv_vif.o mdrv_vif_io.o
mdrv-vif-objs			+= ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/vif/mhal_vif.o

# export header files
EXPORT_H_FILES    += mdrv_vif_io.h
