# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.

# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                       #
#	VERSION		:	1.0                                            #
#	AUTHOR		:	ingyu.yang (ingyu.yang@lge.com                 #
#	DATE            :       2009.12.30                                     #
#	DESCRIPTION	:	Makefile for building pm module                #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk
include $(pwd)
LOCAL_PATH := $(call my-dir)
pm_MODULE			:= pm

# TODO: define your driver source file
#-------------------------------------------------------------------------------

kdrv_pm-objs		:= pm_cfg.o
kdrv_pm-objs		+= pm_drv.o
kdrv_pm-objs		+= pm_proc.o
kdrv_pm-objs		+= pm_core.o
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_pm-objs		+= ../../chip/h15/pm/pm_h15proc.o
kdrv_pm-objs		+= ../../chip/h15/pm/pm_h15func.o
kdrv_pm-objs            += ../../chip/h15/pm/pm_h15governor.o
endif
#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_pm-CFLAGS	+= -I./
kdrv_pm-CFLAGS	+=
kdrv_pm-CFLAGS	+=
kdrv_pm-ASFLAGS	+= -I./

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_pm-CFLAGS)
EXTRA_CFLAGS		+= $(kdrv_pm-ASFLAGS)
EXTRA_CFLAGS		+= -I./
EXTRA_CFLAGS		+= -I/dtv/spt/jun.kong/git/os/lg115x_linux/drivers/kdriver/core/pm/

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(pm_MODULE)/,$(kdrv_pm-objs))
else
obj-m				:= kdrv_pm.o
endif
