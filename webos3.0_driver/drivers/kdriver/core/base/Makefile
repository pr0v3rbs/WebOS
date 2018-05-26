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
#	AUTHOR		:				                                               #
#	DATE        :	                                                           #
#	DESCRIPTION	:	Makefile for building base module                          #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

USE_TLSF_ALLOCATOR	:= NO
USE_HMA_ALLOCATOR	:= YES

base_MODULE			:= base

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_base-objs		:= base_drv.o
kdrv_base-objs		+= base_device.o
kdrv_base-objs		+= base_devmem.o
kdrv_base-objs		+= base_proc.o
kdrv_base-objs		+= os_util.o
kdrv_base-objs		+= proc_util.o
kdrv_base-objs		+= debug_util.o
ifeq ($(USE_TLSF_ALLOCATOR), YES)
kdrv_base-objs		+= lib/tlsf-2.4.6/src/tlsf.o
endif
#kdrv_base-objs		+= misc_util.o
#kdrv_base-objs		+= noti_util.o

kdrv_base-objs		+= ../../platform/$(KDRV_PLATFORM_CFG)/platform_init.o

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_base-CFLAGS	+= $(call cond_define, YES, USE_TLSF_ALLOCATOR)
kdrv_base-CFLAGS	+= $(call cond_define, YES, USE_HMA_ALLOCATOR)

ifeq ($(USE_TLSF_ALLOCATOR), YES)
kdrv_base-CFLAGS	+= -I$(KDRV_TOP_DIR)/$(base_MODULE)/lib/tlsf-2.4.6/src
kdrv_base-CFLAGS	+= -DTLSF_USE_LOCKS=0
kdrv_base-CFLAGS	+= -DTLSF_USE_MMAP=0
kdrv_base-CFLAGS	+= -DTLSF_USE_SBRK=0
kdrv_base-CFLAGS	+= -DTLSF_STATISTIC=1
endif

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_base-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
KDRV_MODULE_OBJS	+= $(addprefix $(base_MODULE)/,$(kdrv_base-objs))
else
obj-m				:= kdrv_base.o
endif
