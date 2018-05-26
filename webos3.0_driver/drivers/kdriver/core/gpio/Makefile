#*******************************************************************************#
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA			      	#
#  Copyright(c) 2013 by LG Electronics Inc.					#
# 										#
#  This program is free software; you can redistribute it and/or		#
#  modify it under the terms of the GNU General Public License			#
#  version 2 as published by the Free Software Foundation.			#
# 										#
#  This program is distributed in the hope that it will be useful,		#
#  but WITHOUT ANY WARRANTY; without even the implied warranty of		#
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		#
#  GNU General Public License for more details.					#
#*******************************************************************************#  

#******************************************************************************#
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	ingyu.yang (ingyu.yang@lge.com)
#	DATE        :	2009.12.30
#	DESCRIPTION	:	Makefile for building gpio module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

gpio_MODULE			:= gpio

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_gpio-objs		:= gpio_cfg.o
kdrv_gpio-objs		+= gpio_drv.o
kdrv_gpio-objs		+= gpio_proc.o
kdrv_gpio-objs		+= gpio_core.o

ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_gpio-objs		+= ../../chip/h13/gpio/gpio_hal_h13.o
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_gpio-objs		+= ../../chip/m14/gpio/gpio_hal_m14.o
endif

ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_gpio-objs		+= ../../chip/h14/gpio/gpio_hal_h14.o
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_gpio-objs		+= ../../chip/h15/gpio/gpio_hal_h15.o
endif


#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_gpio-CFLAGS	+=
kdrv_gpio-CFLAGS	+=
kdrv_gpio-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_gpio-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(gpio_MODULE)/,$(kdrv_gpio-objs))
else
obj-m				:= kdrv_gpio.o
endif
