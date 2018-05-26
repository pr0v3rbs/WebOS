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

sdec_MODULE			:= sdec

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_sdec-objs		:= sdec_cfg.o
kdrv_sdec-objs		+= sdec_drv.o
kdrv_sdec-objs		+= sdec_proc.o
kdrv_sdec-objs		+= sdec_core.o
kdrv_sdec-objs		+= sdec_io.o
kdrv_sdec-objs		+= sdec_filter.o
kdrv_sdec-objs		+= sdec_pcr.o
kdrv_sdec-objs		+= sdec_cipher.o
kdrv_sdec-objs		+= sdec_isr.o
kdrv_sdec-objs		+= sdec_mm.o
kdrv_sdec-objs		+= sdec_pes.o
kdrv_sdec-objs		+= sdec_hal_top.o
kdrv_sdec-objs		+= sdec_hal_io.o
kdrv_sdec-objs		+= sdec_hal_mpg.o
kdrv_sdec-objs		+= sdec_swparser.o
kdrv_sdec-objs		+= ciplus/sdec_ciplus.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_sdec-CFLAGS	+=
kdrv_sdec-CFLAGS	+=
kdrv_sdec-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_sdec-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(sdec_MODULE)/,$(kdrv_sdec-objs))
else
obj-m				:= kdrv_sdec.o
endif
