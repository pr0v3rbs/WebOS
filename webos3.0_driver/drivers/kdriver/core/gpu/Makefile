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
#	AUTHOR		:	BongRae Cho (bongrae.cho@lge.com)
#	DATE        :	2011.09.24
#	DESCRIPTION	:	Makefile for building gpu module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

gpu_MODULE			:= gpu

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_gpu-objs		:= gpu_cfg.o
kdrv_gpu-objs		+= gpu_drv.o
kdrv_gpu-objs		+= gpu_ctrl.o
kdrv_gpu-objs		+= gpu_proc.o
ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_gpu-objs		+= ../../chip/h13/gpu/gpu_drv_h13.o
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_gpu-objs		+= ../../chip/m14/gpu/gpu_drv_m14.o
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_gpu-objs		+= ../../chip/h14/gpu/gpu_drv_h14.o
endif
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_gpu-objs		+= ../../chip/h15/gpu/gpu_drv_h15.o
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_gpu-CFLAGS	+=
kdrv_gpu-CFLAGS	+=
kdrv_gpu-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_gpu-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(gpu_MODULE)/,$(kdrv_gpu-objs))
else
obj-m				:= kdrv_gpu.o
endif
