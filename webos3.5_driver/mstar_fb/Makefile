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
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/mstar_fb/gop/
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/gop

# specific options
EXTRA_CFLAGS      +=  -DRED_LION

# files
obj-y						 := mstar-fb.o

mstar-fb-objs		:= mstar_fb.o
mstar-fb-objs		+= gop/film.o
mstar-fb-objs		+= gop/drvGOP.o
mstar-fb-objs		+=  ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/gop/mhal_GOP.o
