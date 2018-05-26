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
#	AUTHOR		:	sanghyeun.kim
#	DATE        :	2012.08.07
#	DESCRIPTION	:	Makefile for building memprot module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

memprot_MODULE			:= memprot

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_memprot-objs		:= memprot_cfg.o
kdrv_memprot-objs		+= memprot_drv.o
kdrv_memprot-objs		+= memprot_proc.o
kdrv_memprot-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
ifeq ($(PLATFORM_CHIP_REV),B0)
kdrv_memprot-CFLAGS	+= $(call cond_assign, YES, INCLUDE_KDRV_MEMPROT,-DINCLUDE_KDRV_MEMPROT=0xB0,)
else
kdrv_memprot-CFLAGS	+= $(call cond_define, YES, INCLUDE_KDRV_MEMPROT)
endif
kdrv_memprot-CFLAGS	+=
kdrv_memprot-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_memprot-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(memprot_MODULE)/,$(kdrv_memprot-objs))
else
obj-m				:= kdrv_memprot.o
endif
