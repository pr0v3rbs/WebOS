
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.

# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
 

#------------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	sj.youm (sj.youm@lge.com)                                  #
#	DATE		:	2011.06.22                                                 #
#	DESCRIPTION	:	Makefile for building pe module                            #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

pe_MODULE			:= pe

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------
kdrv_pe-objs		:= pe_drv.o
kdrv_pe-objs		+= pe_io.o
kdrv_pe-objs		+= ccm/pe_ccm.o
kdrv_pe-objs		+= cmg/pe_cmg.o
kdrv_pe-objs		+= cmn/pe_cmn.o
kdrv_pe-objs		+= csc/pe_csc.o
kdrv_pe-objs		+= dcm/pe_dcm.o
kdrv_pe-objs		+= dnt/pe_dnt.o
kdrv_pe-objs		+= nrd/pe_nrd.o
kdrv_pe-objs		+= shp/pe_shp.o
kdrv_pe-objs		+= win/pe_win.o
kdrv_pe-objs		+= util/pe_spd.o
kdrv_pe-objs		+= util/pe_etc.o
kdrv_pe-objs		+= reg/pe_reg.o
kdrv_pe-objs		+= hst/pe_hst.o


ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_pe-objs		+= ../../chip/h13/pe/reg/pe_reg_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/cmn/pe_cmn_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/ccm/pe_ccm_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/cmg/pe_cmg_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/csc/pe_csc_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/dcm/pe_dcm_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/nrd/pe_nrd_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/dnt/pe_dnt_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/shp/pe_shp_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/util/pe_fwi_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/util/pe_tsk_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/util/pe_inf_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/win/pe_win_hw_h13.o
kdrv_pe-objs		+= ../../chip/h13/pe/hst/pe_hst_hw_h13.o
endif

ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_pe-objs		+= ../../chip/h14/pe/reg/pe_reg_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/cmn/pe_cmn_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/ccm/pe_ccm_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/cmg/pe_cmg_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/csc/pe_csc_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/dcm/pe_dcm_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/nrd/pe_nrd_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/dnt/pe_dnt_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/shp/pe_shp_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/util/pe_fwi_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/util/pe_tsk_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/util/pe_inf_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/win/pe_win_hw_h14.o
kdrv_pe-objs		+= ../../chip/h14/pe/hst/pe_hst_hw_h14.o
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_pe-objs		+= ../../chip/m14/pe/reg/pe_reg_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/cmn/pe_cmn_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/ccm/pe_ccm_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/cmg/pe_cmg_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/csc/pe_csc_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/dcm/pe_dcm_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/nrd/pe_nrd_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/dnt/pe_dnt_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/shp/pe_shp_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/util/pe_fwi_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/util/pe_tsk_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/util/pe_inf_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/win/pe_win_hw_m14.o
kdrv_pe-objs		+= ../../chip/m14/pe/hst/pe_hst_hw_m14.o
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_pe-objs		+= ../../chip/h15/pe/reg/pe_reg_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/cmn/pe_cmn_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/ccm/pe_ccm_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/cmg/pe_cmg_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/csc/pe_csc_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/dcm/pe_dcm_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/nrd/pe_nrd_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/dnt/pe_dnt_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/shp/pe_shp_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/util/pe_fwi_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/util/pe_tsk_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/util/pe_inf_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/win/pe_win_hw_h15.o
kdrv_pe-objs		+= ../../chip/h15/pe/hst/pe_hst_hw_h15.o
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_pe-CFLAGS   := -I$(KDRV_TOP_DIR)/de
kdrv_pe-CFLAGS   := -I$(KDRV_TOP_DIR)/be
kdrv_pe-CFLAGS   := -I$(KDRV_TOP_DIR)/be/fwi
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/util
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/reg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/cmn
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/ccm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/dcm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/win
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/csc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/cmg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/nrd
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/dnt
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/shp
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/pe/hst


ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/de
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/reg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/util
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/cmn
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/ccm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/cmg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/csc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/dcm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/nrd
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/dnt
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/shp
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/win
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h13/pe/hst
endif

ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/de
kdrv_pe-CFLAGS	 += -I$(KDRV_TOP_DIR)/../chip/h14/pe
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/reg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/util
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/cmn
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/ccm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/cmg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/csc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/dcm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/nrd
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/dnt
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/shp
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/win
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h14/pe/hst
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/de
kdrv_pe-CFLAGS	 += -I$(KDRV_TOP_DIR)/../chip/m14/pe
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/reg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/util
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/cmn
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/ccm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/cmg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/csc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/dcm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/nrd
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/dnt
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/shp
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/win
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/m14/pe/hst
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/de
kdrv_pe-CFLAGS	 += -I$(KDRV_TOP_DIR)/../chip/h15/pe
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/reg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/util
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/cmn
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/ccm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/cmg
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/csc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/dcm
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/nrd
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/dnt
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/shp
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/win
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/hst
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/hw_param
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/hw_param/drc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/hw_param/msc
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/pe/hw_param/h15a0
kdrv_pe-CFLAGS   += -I$(KDRV_TOP_DIR)/../chip/h15/afe
endif

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_pe-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(pe_MODULE)/,$(kdrv_pe-objs))
else
obj-m				:= kdrv_pe.o
endif
