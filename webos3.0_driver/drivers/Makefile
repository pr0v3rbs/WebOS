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
#	FILE NAME	:	Makefile for Kdriver                           #
#	AUTHOR		:	sanghyeun.kim@lge.com                          #
#	AUTHOR		:	youngjun.jang@lge.com - modifier               #
#	DATE       	:	2013/12/06                                     #
#******************************************************************************#

include ../cmn_def.inc
#-----------------------------------------------------------------------------
#		Kdriver Definition
#-----------------------------------------------------------------------------
KDRIVER_NAME		:= kdriver
KDRIVER_PATH		:= $(shell pwd)/kdriver/core
KERNEL_PATH		:= ../kernel
MODULES_NAME		:= modules

#-----------------------------------------------------------------------------
#		make command options	
#-----------------------------------------------------------------------------
PARALLEL_B	:= 	-j16
MFLAGS		:=	-C $(KERNEL_PATH)/$(KERNEL_NAME) M=$(KDRIVER_PATH)  ARCH=arm --no-print-directory
MAKE		:=	$(Q)make $(PARALLEL_B) $(MFLAGS) 

#------------------------------------------------------------------------------
#		Build rules 
#------------------------------------------------------------------------------
.PHONY: prebuild build clean

all: prebuild build 


prebuild:
	$(ECHO) '\n@@@ Making symbolic link to vdec ...'
	$(Q)cd $(KDRIVER_PATH) && \
		ln -sf ../../../modules/hma && \
		ln -sf ../../../modules/fwload && \
		ln -sf ../../../modules/vdec

build:
	$(ECHO) '\n@@@ Compling kdriver ...'
	$(MAKE)  modules KDRV_MODULE_BUILD=YES

clean:
	$(ECHO) '\n@@@ Cleaning kdriver ...'
	$(Q)rm -rf $(KDRIVER_PATH)/target
	$(Q)find -name "*.o" -delete -or -name "*.cmd" -delete
	$(Q)find ../modules -name "*.o" -delete -or -name "*.cmd" -delete -or -name "*.ko" -delete
	$(MAKE)  clean KDRV_MODULE_BUILD=YES

#------------------------------------------------------------------------------
