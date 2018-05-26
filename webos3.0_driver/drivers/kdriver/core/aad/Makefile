
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
#	AUTHOR		:	jongsang.oh (jongsang.oh@lge.com)
#	DATE        :	2010.01.26
#	DESCRIPTION	:	Makefile for building aad module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

aad_MODULE			:= aad

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_aad-objs		:= aad_cfg.o
kdrv_aad-objs		+= aad_drv.o
kdrv_aad-objs		+= aad_proc.o
kdrv_aad-objs		+= aad_coredrv.o
kdrv_aad-objs		+= aad_isr.o
kdrv_aad-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_aad-CFLAGS	+=
kdrv_aad-CFLAGS	+=
kdrv_aad-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_aad-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
#KDRV_MODULE_OBJS	+= $(call global_link_object aad)
KDRV_MODULE_OBJS	+= $(addprefix $(aad_MODULE)/,$(kdrv_aad-objs))
else
obj-m				:= kdrv_aad.o
endif
