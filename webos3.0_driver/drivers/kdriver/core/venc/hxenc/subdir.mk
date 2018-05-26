#******************************************************************************#
#   SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA                          #
#   Copyright(c) 2013 by LG Electronics Inc.                                   #
#                                                                              #
#   This program is free software; you can redistribute it and/or              #
#   modify it under the terms of the GNU General Public License                #
#   version 2 as published by the Free Software Foundation.                    #
#                                                                              #
#   This program is distributed in the hope that it will be useful,            #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#   GNU General Public License for more details.                               #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#    FILE NAME   :    subdir.mk                                                #
#    VERSION     :    1.0                                                      #
#    AUTHOR      :    jaeseop.so (jaeseop.so@lge.com)                          #
#    DATE        :    2011.08.14                                               #
#    DESCRIPTION :    Makefile for building venc module                        #
#******************************************************************************#

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------

include $(src)/venc/hxenc/hxenc_cfg.mk

ifeq ($(VENC_HXENC_BUILTIN), YES)
include $(src)/venc_hxenc/hxenc.mk
else
kdrv_venc-objs          += hxenc/h1encode_wrapper.o
endif

kdrv_venc-objs          += hxenc/venc_hal_hxenc.o
kdrv_venc-objs          += hxenc/h1encode.o
kdrv_venc-objs          += hxenc/h1encode_common.o
kdrv_venc-objs          += hxenc/h1encode_h264.o
kdrv_venc-objs          += hxenc/h1encode_vp8.o
kdrv_venc-objs          += hxenc/h1encode_aui.o
kdrv_venc-objs          += hxenc/hx280enc.o
kdrv_venc-objs          += hxenc/memalloc.o

ifeq ($(VENC_HXENC_BUILTIN), YES)
kdrv_venc-objs          += $(HXENC_OBJS)
kdrv_venc-CFLAGS        += $(HXENC_CFLAGS)
endif

ifeq ($(H1ENCODE_SUPPORT_MULTICORE), YES)
kdrv_venc-CFLAGS        += -DH1ENCODE_SUPPORT_MULTICORE
endif

ifeq ($(VENC_HXENC_NONBLOCKING_MODE), YES)
kdrv_venc-CFLAGS	+= -DH1ENCODE_USE_NONBLOCKING_MODE
endif

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------

