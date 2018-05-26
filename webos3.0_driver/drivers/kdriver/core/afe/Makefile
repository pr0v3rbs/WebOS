
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
#	AUTHOR		:	wonsik.do (wonsik.do@lge.com)
#	DATE        :	2009.12.30
#	DESCRIPTION	:	Makefile for building afe module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

afe_MODULE			:= afe

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_afe-objs		:= afe_cfg.o
kdrv_afe-objs		+= afe_drv.o
kdrv_afe-objs		+= afe_proc.o
kdrv_afe-objs		+= adc_AAD.o
kdrv_afe-objs           += adc_IBD.o
kdrv_afe-objs           += adc_IFM.o
kdrv_afe-objs           += adc_AGOC.o
kdrv_afe-objs           += adc_control.o
kdrv_afe-objs           += adc_module.o
kdrv_afe-objs           += resolution.o
kdrv_afe-objs           += cvd_module.o

ifeq ($(INCLUDE_H13_CHIP_KDRV),YES)
kdrv_afe-objs			+= ../../chip/h13/afe/adc_hw_h13a0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_control_h13a0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_hw_h13a0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_task_h13a0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_control_h13b0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_hw_h13b0.o	
kdrv_afe-objs			+= ../../chip/h13/afe/cvd_task_h13b0.o
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV),YES)
kdrv_afe-objs			+= ../../chip/h14/afe/adc_hw_h14a0.o	
kdrv_afe-objs			+= ../../chip/h14/afe/cvd_control_h14a0.o	
kdrv_afe-objs			+= ../../chip/h14/afe/cvd_hw_h14a0.o	
kdrv_afe-objs			+= ../../chip/h14/afe/cvd_task_h14a0.o
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV),YES)
kdrv_afe-objs			+= ../../chip/m14/afe/adc_hw_m14a0.o	
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_control_m14a0.o	
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_hw_m14a0.o	
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_task_m14a0.o
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_control_m14b0.o	
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_hw_m14b0.o	
kdrv_afe-objs			+= ../../chip/m14/afe/cvd_task_m14b0.o
endif
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_afe-objs			+= ../../chip/h15/afe/adc_hw_h15a0.o	
kdrv_afe-objs			+= ../../chip/h15/afe/cvd_control_h15a0.o	
kdrv_afe-objs			+= ../../chip/h15/afe/cvd_hw_h15a0.o	
kdrv_afe-objs			+= ../../chip/h15/afe/cvd_task_h15a0.o
endif 


#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_afe-CFLAGS	+= -I$(KDRV_TOP_DIR)/pe
kdrv_afe-CFLAGS	+= -I$(KDRV_TOP_DIR)/pe/common/reg
kdrv_afe-CFLAGS	+= -I$(KDRV_TOP_DIR)/pe/h13/reg
kdrv_afe-CFLAGS	+= -I$(KDRV_TOP_DIR)/pe/h15/reg

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_afe-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(afe_MODULE)/,$(kdrv_afe-objs))
else
obj-m				:= kdrv_afe.o
endif
