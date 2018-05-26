#******************************************************************************#
#	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA                          #
#	Copyright(c) 2013 by LG Electronics Inc.                                   #
#	                                                                           #
#	This program is free software; you can redistribute it and/or              #
#	modify it under the terms of the GNU General Public License                #
#	version 2 as published by the Free Software Foundation.                    #
#	                                                                           #
#	This program is distributed in the hope that it will be useful,            #
#	but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               #
#	GNU General Public License for more details.                               #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	raxis.lim (raxis.lim@lge.com)
#	DATE        :	2010.10.05
#	DESCRIPTION	:	Makefile for building png module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

png_MODULE			:= png

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_png-objs		:= png_cfg.o
kdrv_png-objs		+= png_reg.o
kdrv_png-objs		+= png_drv.o


kdrv_png-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_png-CFLAGS	+=
kdrv_png-CFLAGS	+=
kdrv_png-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_png-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(png_MODULE)/,$(kdrv_png-objs)) 
else
obj-m				:= kdrv_png.o
endif
