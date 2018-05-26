
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
#	AUTHOR		:	dj911.kim@lge.com
#	DATE        :	2012.09.09
#	DESCRIPTION	:	Makefile for building ovi module                           #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

ovi_MODULE			:= ovi


#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_ovi-objs		+= ovi_drv.o
kdrv_ovi-objs		+= ovi_cfg.o
kdrv_ovi-objs		+= ovi_hal.o
kdrv_ovi-objs		+= ovi_proc.o
ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_ovi-objs		+= ../../chip/h13/ovi/ovi_hw_h13.o
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_ovi-objs		+= ../../chip/h14/ovi/ovi_hw_h14.o
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_ovi-objs		+= ../../chip/m14/ovi/ovi_hw_m14.o
endif
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_ovi-objs		+= ../../chip/h15/ovi/ovi_hw_h15.o
endif

#------------------------------------------------------------------------------
# REVISION maker using MD5
# -----------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_ovi-CFLAGS   += -I$(KDRV_TOP_DIR)/ovi
ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_ovi-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/ovi
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_ovi-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/ovi
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_ovi-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/ovi
endif
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_ovi-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/ovi
endif

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_ovi-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(ovi_MODULE)/,$(kdrv_ovi-objs))
else
obj-m				:= kdrv_ovi.o
endif
