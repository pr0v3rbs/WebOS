# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	raxis
#	DATE        :	2010.02.01
#	DESCRIPTION	:	Makefile for building fbdev module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

fbdev_MODULE			:= fbdev

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_fbdev-objs		:= fbdev_cfg.o
kdrv_fbdev-objs		+= fbdev_drv.o
kdrv_fbdev-objs		+= fbdev_proc.o
kdrv_fbdev-objs     += fbdev_module.o

ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_fbdev-objs     += ../../chip/h13/fbdev/fbdev_drv_h13.o
kdrv_fbdev-objs		+= ../../chip/h13/fbdev/fbdev_hw_h13cmn.o
kdrv_fbdev-objs		+= ../../chip/h13/fbdev/fbdev_hw_h13a0.o
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_fbdev-objs     += ../../chip/m14/fbdev/fbdev_drv_m14.o
kdrv_fbdev-objs		+= ../../chip/m14/fbdev/fbdev_hw_m14cmn.o
kdrv_fbdev-objs		+= ../../chip/m14/fbdev/fbdev_hw_m14a0.o
endif

ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_fbdev-objs     += ../../chip/h14/fbdev/fbdev_drv_h14.o
kdrv_fbdev-objs		+= ../../chip/h14/fbdev/fbdev_hw_h14cmn.o
kdrv_fbdev-objs		+= ../../chip/h14/fbdev/fbdev_hw_h14a0.o
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_fbdev-objs     += ../../chip/h15/fbdev/fbdev_drv_h15.o
kdrv_fbdev-objs		+= ../../chip/h15/fbdev/fbdev_hw_h15cmn.o
kdrv_fbdev-objs		+= ../../chip/h15/fbdev/fbdev_hw_h15a0.o
kdrv_fbdev-objs		+= ../../chip/h15/fbdev/fbdev_hw_h15test.o
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_fbdev-CFLAGS	+=
kdrv_fbdev-CFLAGS	+=
kdrv_fbdev-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_fbdev-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(fbdev_MODULE)/,$(kdrv_fbdev-objs))
else
obj-m				:= kdrv_fbdev.o
endif
