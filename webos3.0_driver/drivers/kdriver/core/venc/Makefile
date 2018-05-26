#******************************************************************************#
#	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA                          #
#	Copyright(c) 2013 by LG Electronics Inc.                                   #
#	                                                                           #
#	This program is free software; you can redistribute it and/or              #
#	modify it under the terms of the GNU General Public License                #
#	version 2 as published by the Free Software Foundation.                    #
#	                                                                           #
#	This program is distributed in the hope that it will be useful,            #
#	but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               #
#	GNU General Public License for more details.                               #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	jaeseop.so (jaeseop.so@lge.com)                            #
#	DATE        :	2011.07.20                                                 #
#	DESCRIPTION	:	Makefile for building venc module                          #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

venc_MODULE			:= venc

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------

kdrv_venc-objs		+= venc_drv.o
kdrv_venc-objs		+= venc_cfg.o
kdrv_venc-objs		+= venc_proc.o
kdrv_venc-objs		+= venc_hal.o
kdrv_venc-objs		+= venc_reg.o
kdrv_venc-objs		+= venc_clockgate.o

include $(KDRV_TOP_DIR)/$(venc_MODULE)/hxenc/subdir.mk
#include $(KDRV_TOP_DIR)/$(venc_MODULE)/ench1/subdir.mk

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_venc-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_venc-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(venc_MODULE)/,$(kdrv_venc-objs))
else
obj-m				:= kdrv_venc.o
endif
