#******************************************************************************#
#	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA						   #
#	Copyright(c) 2013 by LG Electronics Inc.								   #
#																			   #
#	This program is free software; you can redistribute it and/or			   #
#	modify it under the terms of the GNU General Public License				   #
#	version 2 as published by the Free Software Foundation.					   #
#																			   #
#	This program is distributed in the hope that it will be useful,			   #
#	but WITHOUT ANY WARRANTY; without even the implied warranty of			   #
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			   #
#	GNU General Public License for more details.							   #
#																			   #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	wunseo.yeo												   #
#	DATE        :	2014.08
#	DESCRIPTION	:	Makefile for building slt module						   #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

slt_MODULE			:= slt

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_slt-objs		:= slt_cfg.o
kdrv_slt-objs		+= slt_drv.o
kdrv_slt-objs		+= slt_hal.o
kdrv_slt-objs		+= slt_proc.o
kdrv_slt-objs		+= ../../chip/h15/slt/slt_h15.o
kdrv_slt-objs		+= ../../chip/h13/slt/slt_h13.o
kdrv_slt-objs		+= ../../chip/m14/slt/slt_m14.o


#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_slt-CFLAGS	+=
kdrv_slt-CFLAGS	+=
kdrv_slt-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_slt-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(slt_MODULE)/,$(kdrv_slt-objs)) 
else
obj-m				:= kdrv_slt.o
endif
