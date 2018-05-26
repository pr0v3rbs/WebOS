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

pvr_MODULE			:= pvr

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_pvr-objs		:= pvr_cfg.o
kdrv_pvr-objs		+= pvr_drv.o
kdrv_pvr-objs		+= pvr_proc.o
kdrv_pvr-objs		+= pvr_reg.o
kdrv_pvr-objs		+= pvr_core.o
kdrv_pvr-objs		+= pvr_pie.o
kdrv_pvr-objs		+= pvr_isr.o
#kdrv_pvr-objs		+= pvr_parser.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_pvr-CFLAGS	+=
kdrv_pvr-CFLAGS	+=
kdrv_pvr-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_pvr-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(pvr_MODULE)/,$(kdrv_pvr-objs)) 
else
obj-m				:= kdrv_pvr.o
endif
