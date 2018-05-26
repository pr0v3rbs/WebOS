
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
#	AUTHOR		:	justine.jeong@lge.com
#	DATE		:	2012.07.06
#	DESCRIPTION	:	Makefile for building apr module                           #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

apr_MODULE			:= apr

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_apr-objs		+= apr_cfg.o
kdrv_apr-objs		:= apr_drv.o
kdrv_apr-objs		+= apr_core.o
kdrv_apr-objs		+= apr_hal.o
kdrv_apr-objs		+= apr_osd.o
kdrv_apr-objs		+= apr_ldf.o
kdrv_apr-objs		+= apr_hfd.o
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_apr-objs		+= ../../chip/h15/apr/apr_reg_h15.o
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_apr-objs		+= ../../chip/h14/apr/apr_reg_h14.o
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_apr-objs		+= ../../chip/m14/apr/apr_reg_m14.o
endif
ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_apr-objs		+= ../../chip/h13/apr/apr_reg_h13.o
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/apr
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/de
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/pe
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/common/reg
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/h13/util
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/h13/hst
kdrv_apr-CFLAGS   += -I$(KDRV_TOP_DIR)/venc

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_apr-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(apr_MODULE)/,$(kdrv_apr-objs))
else
obj-m				:= kdrv_apr.o
endif

