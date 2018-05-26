#
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

include $(KDRV_TOP_DIR)/modules.mk

vip_MODULE			:= vip

VIP_BUILD=0
ifeq ($(PLATFORM_CHIP), lg1210)
VIP_BUILD=1
endif

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
ifeq ($(VIP_BUILD),1)
kdrv_vip-objs		:= vip_cfg.o
kdrv_vip-objs		+= vip_drv.o
kdrv_vip-objs		+= vip_core.o
kdrv_vip-objs		+= vip_usb.o
kdrv_vip-objs		+= vip_queue.o
kdrv_vip-objs		+= vip_mem.o
kdrv_vip-objs		+= vip_proc.o
kdrv_vip-objs		+= vip_debug.o
kdrv_vip-objs		+= vip_drv_hal.o
kdrv_vip-objs		+= vip_drv_img.o
kdrv_vip-objs		+= h15/vip_drv_hal_h15_a0.o
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_vip-CFLAGS	+=

kdrv_vip-CFLAGS	+= -DVIP_FIRM_DOWN_FROM_KADAP
kdrv_vip-CFLAGS	+= -DVIP_GESTURE_ON

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_vip-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(vip_MODULE)/,$(kdrv_vip-objs))
else
obj-m				:= kdrv_vip.o
endif
