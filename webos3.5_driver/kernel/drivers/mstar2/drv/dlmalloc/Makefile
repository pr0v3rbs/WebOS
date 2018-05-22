#
# Makefile for MStar device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

# general options
#EXTRA_CFLAGS     +=  -Idrivers/mstar/common
EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/mstar_fb/dlmalloc/

# specific options
EXTRA_CFLAGS      +=  -DRED_LION

# files
obj-y       += dlmalloc.o

#obj-$(CONFIG_MSTAR_DLMALLOC)						 := mdrv-dlmalloc.o
#mdrv-dlmalloc-objs	    := dlmalloc.o
