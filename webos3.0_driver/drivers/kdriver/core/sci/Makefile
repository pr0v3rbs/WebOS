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

sci_MODULE			:= sci

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_sci-objs		+= sci_drv.o
kdrv_sci-objs		+= sci_core.o
kdrv_sci-objs		+= sci_isr.o
kdrv_sci-objs		+= sci_reg.o
kdrv_sci-objs		+= sci_ctop_cfg.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_sci-CFLAGS	+=
kdrv_sci-CFLAGS	+=
kdrv_sci-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_sci-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(sci_MODULE)/,$(kdrv_sci-objs))
else
obj-m				:= kdrv_sci.o
endif
