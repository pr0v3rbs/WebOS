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
#	AUTHOR		:	ks.hyun (ks.hyun@lge.com)
#	DATE        :	2010.12.13
#	DESCRIPTION	:	Makefile for building sys module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

sys_MODULE			:= sys

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_sys-objs		:= sys_cfg.o
kdrv_sys-objs		+= sys_drv.o
kdrv_sys-objs		+= sys_proc.o
kdrv_sys-objs		+= sys_io.o
kdrv_sys-objs		+= sys_diag.o
kdrv_sys-objs		+= reg_ctrl.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_sys-CFLAGS	+=
kdrv_sys-CFLAGS	+=
kdrv_sys-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_sys-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(sys_MODULE)/,$(kdrv_sys-objs))
else
obj-m				:= kdrv_sys.o
endif
