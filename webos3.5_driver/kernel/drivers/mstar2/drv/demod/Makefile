#
# Makefile for MStar Demodulator device drivers.
#

# dhjung LGE
ifeq ($(MAKE_TYPE), MODULE_STANDALONE)
include $(TOP_DIR)/modules.mk
endif

# dhjung LGE
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/demod
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/demod
#EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)
#EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)/board



#EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
#EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/emac
#EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/emac
#EXTRA_CFLAGS      +=  -Iinclude/linux

# specific options
# EXTRA_CFLAGS      +=  -DTITANIA
# EXTRA_CFLAGS      +=  -DMSOS_TYPE_LINUX

# files
##dhjung LGE obj-$(CONFIG_MSTAR_DEMOD)		+= mdrv_demod.o
##dhjung LGE obj-$(CONFIG_MSTAR_DEMOD)		+= $(CONFIG_MSTAR_CHIP_NAME)/

# dhjung LGE
obj-$(CONFIG_MSTAR_DEMOD)	+= mdrv-demod.o
mdrv-demod-objs 		+= mdrv_demod.o
#mdrv-demod-objs			+= $(CONFIG_MSTAR_CHIP_NAME)/mhal_demod.o
mdrv-demod-objs    += ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/demod/mhal_demod.o
