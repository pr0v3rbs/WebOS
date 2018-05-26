
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
#	DATE		:	2012.02.13
#	DESCRIPTION	:	Makefile for building be module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

be_MODULE			:= be


#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_be-objs		:= be_drv.o
kdrv_be-objs		+= be_cfg.o


kdrv_be-objs		+= top/be_top_drv.o
kdrv_be-objs		+= top/be_top_hal.o
kdrv_be-objs		+= reg/be_reg.o
kdrv_be-objs		+= fwi/fwl_drv.o
kdrv_be-objs		+= fwi/fwl_hal.o
kdrv_be-objs		+= fwi/ipc_drv.o
kdrv_be-objs		+= fwi/ipc_hal.o
kdrv_be-objs		+= fwi/int_drv.o
kdrv_be-objs		+= fwi/int_hal.o
kdrv_be-objs		+= led/led_drv.o
kdrv_be-objs		+= led/led_hal.o
kdrv_be-objs		+= frc/frc_drv.o
kdrv_be-objs		+= frc/frc_hal.o

kdrv_be-objs		+= h3d/h3d_drv.o
kdrv_be-objs		+= h3d/h3d_hal.o
kdrv_be-objs		+= pwm_pe/pwm_pe_drv.o
kdrv_be-objs		+= pwm_pe/pwm_hal.o
kdrv_be-objs		+= sre/sre_drv.o
kdrv_be-objs		+= sre/sre_hal.o

kdrv_be-objs		+= fwi/mcu/mcu_ipc_test.o
kdrv_be-objs		+= video_status_keeper.o

ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_be-objs		+= ../../chip/h13/be/top/be_top_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/frc/be_frc_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/led/be_led_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/sre/be_sre_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/h3d/be_h3d_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/fwi/be_fwl_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/fwi/be_ipc_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/fwi/be_int_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/pwm_pe/be_pwm_hw_h13.o
kdrv_be-objs		+= ../../chip/h13/be/reg/be_reg_hw_h13.o
endif
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_be-objs		+= ../../chip/h14/be/top/be_top_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/frc/be_frc_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/led/be_led_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/sre/be_sre_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/h3d/be_h3d_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/fwi/be_fwl_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/fwi/be_ipc_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/fwi/be_int_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/pwm_pe/be_pwm_hw_h14.o
kdrv_be-objs		+= ../../chip/h14/be/reg/be_reg_hw_h14.o
endif
ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_be-objs		+= ../../chip/m14/be/top/be_top_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/frc/be_frc_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/led/be_led_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/sre/be_sre_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/h3d/be_h3d_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/fwi/be_fwl_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/fwi/be_ipc_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/fwi/be_int_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/pwm_pe/be_pwm_hw_m14.o
kdrv_be-objs		+= ../../chip/m14/be/reg/be_reg_hw_m14.o
endif
ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_be-objs		+= ../../chip/h15/be/top/be_top_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/led/be_led_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/h3d/be_h3d_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/fwi/be_fwl_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/fwi/be_ipc_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/fwi/be_int_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/pwm_pe/be_pwm_hw_h15.o
kdrv_be-objs		+= ../../chip/h15/be/reg/be_reg_hw_h15.o
endif

#------------------------------------------------------------------------------
# REVISION maker using MD5
# -----------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/top
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/reg
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/fwi
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/h3d
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/frc
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/sre
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/led
kdrv_be-CFLAGS   += -I$(KDRV_TOP_DIR)/be/pwm_pe

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_be-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(be_MODULE)/,$(kdrv_be-objs))
else
obj-m				:= kdrv_be.o
endif

