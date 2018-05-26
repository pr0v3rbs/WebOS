
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
#	AUTHOR		:	won.hur (won.hur@lge.com)
#	DATE        :	2012.04.18                                                 
#	DESCRIPTION	:	Makefile for building HDMI20 module                        #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

hdmi20_MODULE			:= hdmi20

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_hdmi20-objs		:= hdmi20_cfg.o
kdrv_hdmi20-objs		+= hdmi20_drv.o
kdrv_hdmi20-objs		+= hdmi20_module.o
kdrv_hdmi20-objs		+= hdmi20_proc.o

kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi_rx_phy.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi_rx_ctrl_hdcp.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi_rx_ctrl.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi20_hal_h15ax.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi20_rx_port0.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi20_rx_port1.o
kdrv_hdmi20-objs		+= ../../chip/h15/hdmi20/hdmi20_tx_port.o



#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_hdmi20-CFLAGS	+=
kdrv_hdmi20-CFLAGS	+=
kdrv_hdmi20-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_hdmi20-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(hdmi20_MODULE)/,$(kdrv_hdmi20-objs))
else
obj-m				:= kdrv_hdmi20.o
endif
