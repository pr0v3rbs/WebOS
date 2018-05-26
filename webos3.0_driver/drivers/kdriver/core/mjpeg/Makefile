#******************************************************************************#
#    SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA                         #
#    Copyright(c) 1998-2013 by LG Electronics Inc.                             #
#                                                                              #
#    This program is free software; you can redistribute it and/or             #
#    modify it under the terms of the GNU General Public License               #
#    version 2 as published by the Free Software Foundation.                   #
#                                                                              #
#    This program is distributed in the hope that it will be useful,           #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of            #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              #
#    GNU General Public License for more details.                              #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	youngwoo.jin (youngwoo.jin@lge.com)                        #
#	DATE       	:	2011.04.09                                                 #
#	DESCRIPTION	:	Makefile for building mjpeg module                         #
#******************************************************************************#

#include $(KDRV_TOP_DIR)/modules.mk

mjpeg_MODULE		:= mjpeg

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_mjpeg-objs		:= mjpeg_cfg.o
kdrv_mjpeg-objs		+= mjpeg_drv.o
kdrv_mjpeg-objs		+= mjpeg_proc.o
kdrv_mjpeg-objs		+= mjpeg_core.o
kdrv_mjpeg-objs		+= mjpeg_header.o
kdrv_mjpeg-objs		+= icod_clockgate.o
kdrv_mjpeg-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_mjpeg-CFLAGS	+=
kdrv_mjpeg-CFLAGS	+=
kdrv_mjpeg-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_mjpeg-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(mjpeg_MODULE)/,$(kdrv_mjpeg-objs))
else
obj-m				:= kdrv_mjpeg.o
endif
