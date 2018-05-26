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
#	FILE NAME	:	Makefile for kernel                            #
#	AUTHOR		:	sanghyeun.kim@lge.com                          #
#	DATE       	:	2013/12/06                                     #
#******************************************************************************#

include ../cmn_def.inc
#-----------------------------------------------------------------------------
#		Make command options 
#-----------------------------------------------------------------------------
KDRIVER_PATH	:=	$(shell pwd)/../drivers/kdriver
PARALLEL_B	:= 	-j16
MFLAGS		:=	-C $(KERNEL_NAME) ARCH=arm --no-print-directory
MAKE		:=	$(Q)make $(PARALLEL_B)  $(MFLAGS)

#------------------------------------------------------------------------------
#		Build rules 
#------------------------------------------------------------------------------
.PHONY: extract config build clean

all: extract config build 

extract:
	$(Q)if [ ! -d $(KERNEL_NAME) ] ; then \
		echo -e "\n@@@ Extracting $(KERNEL_NAME).tgz ..."; \
		tar -xf $(KERNEL_NAME).tgz  > /dev/null; \
	fi

config:
	$(ECHO) '\n@@@ Configuring($(KERNEL_DEFCONFIG)) kernel ... '
	$(RM) -rf  $(KERNEL_NAME)/drivers/lg_kdriver	
	$(LN) $(KDRIVER_PATH) $(KERNEL_NAME)/drivers/lg_kdriver 
	$(MAKE) $(KERNEL_DEFCONFIG)

build:
	$(ECHO) '\n@@@ Compiling $(KERNEL_NAME) ...'
	$(MAKE) uImage  

clean:
	$(ECHO) '\n@@@ Cleaning $(KERNEL_NAME) ...'
	$(Q)if [ -d $(KERNEL_NAME) ] ; then \
		rm -rf $(KERNEL_NAME) ; \
	fi

#------------------------------------------------------------------------------
