#
# Makefile for MStar System device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif


# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/system_LG
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/system_LG
ifdef CONFIG_MSTAR_SPI_FLASH
EXTRA_CFLAGS      +=  -Idrivers/mstar2/flash
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include/sys/common
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include/sys/drv
endif
# specific options
EXTRA_CFLAGS      +=

# files
obj-$(CONFIG_SYSTEM_LG)	        := mdrv-system-LG.o
mdrv-system-LG-objs		:= mdrv_system_LG.o
mdrv-system-LG-objs	    += mmapger.o
mdrv-system-LG-objs		+= mdrv_system_io_LG.o
