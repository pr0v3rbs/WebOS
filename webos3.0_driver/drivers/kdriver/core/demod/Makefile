
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
#	AUTHOR		:	jeongpil.yun (jeongpil.yun@lge.com)
#	DATE        :	2009.12.30
#	DESCRIPTION	:	Makefile for building demod module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

demod_MODULE			:= demod

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_demod-objs		:= demod_cfg.o
kdrv_demod-objs		+= demod_drv.o
kdrv_demod-objs		+= demod_proc.o
kdrv_demod-objs		+= demod_module.o
kdrv_demod-objs		+= demod_hw.o


ifeq ($(INCLUDE_L9_CHIP_KDRV), YES)
kdrv_demod-objs		+= ../../chip/l9/demod/demod_drv_l9.o
kdrv_demod-objs		+= ../../chip/l9/demod/demod_common_l9.o
kdrv_demod-objs		+= ../../chip/l9/demod/demod_dvb_l9.o
kdrv_demod-objs		+= ../../chip/l9/demod/demod_vqi_l9.o
else
kdrv_demod-objs 		+= ../../chip/l9/demod/demod_l9_drv_fake.o
endif

ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_demod-objs		+= ../../chip/h13/demod/demod_drv_h13.o
kdrv_demod-objs		+= ../../chip/h13/demod/demod_common_h13.o
kdrv_demod-objs		+= ../../chip/h13/demod/demod_dvb_h13.o
kdrv_demod-objs		+= ../../chip/h13/demod/demod_vqi_h13.o
else
kdrv_demod-objs 		+= ../../chip/h13/demod/demod_h13_drv_fake.o
endif

ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_demod-objs		+= ../../chip/h14/demod/h14gbb/demod_drv_h14gbb.o
kdrv_demod-objs		+= ../../chip/h14/demod/h14gbb/demod_common_h14gbb.o
kdrv_demod-objs		+= ../../chip/h14/demod/h14gbb/demod_dvb_h14gbb.o
kdrv_demod-objs		+= ../../chip/h14/demod/h14gbb/demod_vqi_h14gbb.o
#kdrv_demod-objs		+= ../../chip/h14/demod/h14eu/demod_drv_h14eu.o
#kdrv_demod-objs		+= ../../chip/h14/demod/h14eu/demod_common_h14eu.o
#kdrv_demod-objs		+= ../../chip/h14/demod/h14eu/demod_dvb_h14eu.o
else
kdrv_demod-objs 		+= ../../chip/h14/demod/h14eu/demod_h14eu_drv_fake.o
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_demod-objs		+= ../../chip/m14/demod/m14a0/demod_drv_m14a0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14a0/demod_common_m14a0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14a0/demod_dvb_m14a0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14a0/demod_vqi_m14a0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14a0/demod_analog_m14a0.o

kdrv_demod-objs		+= ../../chip/m14/demod/m14b0/demod_analog_m14b0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14b0/demod_drv_m14b0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14b0/demod_common_m14b0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14b0/demod_dvb_m14b0.o
kdrv_demod-objs		+= ../../chip/m14/demod/m14b0/demod_vqi_m14b0.o

kdrv_demod-objs		+= analog_common/ademod_common.o
kdrv_demod-objs		+= analog_common/ademod_common_demod.o
kdrv_demod-objs		+= analog_common/ademod_common_Menu.o
kdrv_demod-objs		+= analog_common/ademod_common_Paramtable.o
kdrv_demod-objs		+= analog_common/ademod_common_Presets.o
kdrv_demod-objs		+= analog_common/ademod_common_Regtable.o
kdrv_demod-objs		+= analog_common/ademod_common_userdef.o
kdrv_demod-objs		+= analog_common/ademod_common_UserUtilities.o


kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_agc.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_agc_dig.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_audio.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_bsp.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_demod.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_factory.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_hostI2C.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_legacy.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_lowif.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_Presets.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_UserSettings.o
kdrv_demod-objs		+= ../../chip/m14/demod/comm_abb/ademod_m14_video.o

kdrv_demod-CFLAGS	+= -I$(KDRV_TOP_DIR)/../chip/m14/demod/comm_abb
kdrv_demod-CFLAGS	+= -I$(KDRV_TOP_DIR)/demod/analog_common

else
kdrv_demod-objs 		+= ../../chip/m14/demod/m14a0/demod_m14a0_drv_fake.o
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_demod-objs		+= ../../chip/h15/demod/demod_drv_h15.o
kdrv_demod-objs		+= ../../chip/h15/demod/demod_common_h15.o
kdrv_demod-objs		+= ../../chip/h15/demod/demod_dvb_h15.o
kdrv_demod-objs		+= ../../chip/h15/demod/demod_vqi_h15.o
else
kdrv_demod-objs 		+= ../../chip/h13/demod/demod_h15_drv_fake.o
endif



#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_demod-CFLAGS	+= 
kdrv_demod-CFLAGS	+=
kdrv_demod-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_demod-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(demod_MODULE)/,$(kdrv_demod-objs))
else
obj-m				:= kdrv_demod.o
endif
