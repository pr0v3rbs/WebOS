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
#	DESCRIPTION	:	Makefile for building i2c module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

i2c_MODULE			:= i2c

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_i2c-objs		:= i2c_cfg.o
kdrv_i2c-objs		+= i2c_drv.o
kdrv_i2c-objs		+= i2c_proc.o
kdrv_i2c-objs		+= i2c_core.o
kdrv_i2c-objs		+= i2c_gpio.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_i2c-CFLAGS	+=
kdrv_i2c-CFLAGS	+=
kdrv_i2c-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_i2c-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
#KDRV_MODULE_OBJS	+= $(call global_link_object i2c)
KDRV_MODULE_OBJS	+= $(addprefix $(i2c_MODULE)/,$(kdrv_i2c-objs))
else
obj-m				:= kdrv_i2c.o
endif
