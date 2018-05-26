#******************************************************************************#
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
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	Makefile                                       #
#	AUTHOR		:	sanghyeun.kim@lge.com                          #
#	AUTHOR		:	youngjun.jang@lge.com - modifier               #
#	DATE       	:	2014/12/30                                     #
#	DESCRIPTION	:	Makefile only to compile kernel&kdriver.       #
#******************************************************************************#
#-----------------------------------------------------------------------------
#		Toolchain Definition
#-----------------------------------------------------------------------------
CROSS_TOOL              :=   arm-lg115x-linux-gnueabi-
CC			:=   $(CROSS_TOOL)gcc
CXX			:=   $(CROSS_TOOL)g++
AS			:=   $(CROSS_TOOL)as
AR			:=   $(CROSS_TOOL)ar
LD			:=   $(CROSS_TOOL)ld
STRIP                   :=   $(CROSS_TOOL)strip
OBJCOPY                 :=   $(CROSS_TOOL)objcopy
NM                      :=   $(CROSS_TOOL)nm -na
#-----------------------------------------------------------------------------
#		Shell command	
#-----------------------------------------------------------------------------
SHELL	= /bin/bash

ifeq ($(V),)
Q	?= 	@
endif

ECHO	:=	$(Q)echo -e
RM	:=	$(Q)rm
LN	:=	$(Q)ln -s

#-----------------------------------------------------------------------------
#		Kernel Definition
#-----------------------------------------------------------------------------
KERNEL_SUFFIX		:= -lg115x
KERNEL_VERSION		:= 3.16
KERNEL_NAME		:= linux-$(KERNEL_VERSION)$(KERNEL_SUFFIX)
KERNEL_DATE		:=
KERNEL_DEFCONFIG	:= lg1k_defconfig
export KERNEL_NAME KERNEL_DATE

#-----------------------------------------------------------------------------
