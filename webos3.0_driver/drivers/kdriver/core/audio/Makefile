
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
#	AUTHOR		:	jongsang.oh (jongsang.oh@lge.com)
#	DATE        :	2012.07.04
#	DESCRIPTION	:	Makefile for building adec module                      #
#******************************************************************************#
include $(KDRV_TOP_DIR)/modules.mk

#INCLUDE_AUDIO_CHIP_KDRV ?= YES

audio_MODULE			:= audio

#-------------------------------------------------------------------------------
# TODO: define your driver source file
#-------------------------------------------------------------------------------

kdrv_audio-objs		:= audio_proc.o
kdrv_audio-objs		+= audio_cfg.o

kdrv_audio-objs		+= audio_drv.o
kdrv_audio-objs		+= audio_drv_hal.o
kdrv_audio-objs		+= audio_drv_hal_mixedIP.o

kdrv_audio-objs		+= audio_drv_decoder.o
kdrv_audio-objs		+= audio_drv_encoder.o
kdrv_audio-objs		+= audio_drv_capturer.o
kdrv_audio-objs		+= audio_drv_renderer.o
kdrv_audio-objs		+= audio_drv_master.o
kdrv_audio-objs		+= audio_drv_isr.o
kdrv_audio-objs		+= audio_drv_manager.o
kdrv_audio-objs		+= audio_drv_manager_debug.o
kdrv_audio-objs		+= audio_drv_debug.o

ifeq ($(INCLUDE_AUDIO_CHIP_KDRV), YES)
ifeq ($(INCLUDE_H13_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/h13/audio/audio_drv_hal_h13.o
kdrv_audio-objs		+= ../../chip/h13/audio/mixedIP/mixedIP_drv_h13_b0.o
endif
endif

ifeq ($(INCLUDE_M14_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/m14/audio/audio_drv_hal_m14_a0.o
ifeq ($(INCLUDE_AUDIO_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/m14/audio/mixedIP/mixedIP_drv_m14_a0.o
endif
kdrv_audio-objs		+= ../../chip/m14/audio/mixedIP/mixedIP_drv_m14_b0.o
endif

ifeq ($(INCLUDE_AUDIO_CHIP_KDRV), YES)
ifeq ($(INCLUDE_H14_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/h14/audio/audio_drv_hal_h14_a0.o
kdrv_audio-objs		+= ../../chip/h14/audio/mixedIP/mixedIP_drv_h14_a0.o
endif
endif

ifeq ($(INCLUDE_H15_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/h15/audio/audio_drv_hal_h15_a0.o
kdrv_audio-objs		+= ../../chip/h15/audio/mixedIP/mixedIP_drv_h15_a0.o
endif

#INCLUDE_H15UBB_CHIP_KDRV ?= YES

ifeq ($(INCLUDE_H15UBB_CHIP_KDRV), YES)
kdrv_audio-objs		+= ../../chip/h15/audio/mixedIP/mixedIP_drv_h15_ubb.o
endif

kdrv_audio-objs		+= audio_buffer.o
kdrv_audio-objs		+= audio_imc_func.o

kdrv_audio-objs		+= buffer/adec_buf_dbg.o
kdrv_audio-objs		+= buffer/adec_buf_helper.o
kdrv_audio-objs		+= buffer/adec_common_buf.o
kdrv_audio-objs		+= buffer/adec_inter_buf.o
kdrv_audio-objs		+= buffer/adec_inter_buf_reader.o
kdrv_audio-objs		+= buffer/adec_inter_buf_writer.o

kdrv_audio-objs		+= common/adec_error.o
kdrv_audio-objs		+= common/adec_processor_adap.o
kdrv_audio-objs		+= common/adec_reg_api.o
kdrv_audio-objs		+= common/adec_debug_index.o

kdrv_audio-objs		+= imc/adec_imc.o
kdrv_audio-objs		+= ipc/adec_ipc.o
kdrv_audio-objs		+= ipc/adec_ipc_dbg.o
kdrv_audio-objs		+= module/adec_module_list.o
kdrv_audio-objs		+= util/adec_rbtree.o

kdrv_audio-objs		+= alsa/alsa_machine.o
kdrv_audio-objs		+= alsa/alsa_codec.o
kdrv_audio-objs		+= alsa/alsa_platform_pcm.o
kdrv_audio-objs		+= alsa/alsa_platform_dai.o

kdrv_audio-objs		+=

#-------------------------------------------------------------------------------
# TODO: define your driver specific CFLAGS
#-------------------------------------------------------------------------------
kdrv_audio-CFLAGS	+= -D_ADEC_ARM
kdrv_audio-CFLAGS	+=
kdrv_audio-CFLAGS	+=

#-------------------------------------------------------------------------------
# DO NOT change the below part
#-------------------------------------------------------------------------------
EXTRA_CFLAGS		+= $(kdrv_audio-CFLAGS)

ifeq ($(KDRV_GLOBAL_LINK), YES)
# KDRV_MODULE_OBJS is used for KDRV_GLOBAL_LINK mode
KDRV_MODULE_OBJS	+= $(addprefix $(audio_MODULE)/,$(kdrv_audio-objs))
else
obj-m				:= kdrv_audio.o
endif
