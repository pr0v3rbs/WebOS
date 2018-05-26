#/*
# * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# * Copyright(c) 2013 by LG Electronics Inc.
# *
# * This program is free software; you can redistribute it and/or
# * modify it under the terms of the GNU General Public License
# * version 2 as published by the Free Software Foundation.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# * GNU General Public License for more details.
# */
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	hwang.hayun (hwang.hayun@lge.com)
#	DATE        :	2013.10.29
#	DESCRIPTION	:	Makefile for building logm module                      	   #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

logm_MODULE			:= logm

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_logm-objs		:= logm_cfg.o
kdrv_logm-objs		+= logm_drv.o
kdrv_logm-objs		+= logm_proc.o
kdrv_logm-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_logm-CFLAGS	+=
kdrv_logm-CFLAGS	+=
kdrv_logm-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_logm-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(logm_MODULE)/,$(kdrv_logm-objs)) 
else
obj-m				:= kdrv_logm.o
endif
