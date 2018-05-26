#----------------------------------------------------------------------------------------
#
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
#
#----------------------------------------------------------------------------------------
DDK_TOP			:= $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
DDK_VER			:= 5.0.11.p3

DDK_SRC_DIR		:= $(DDK_TOP)/$(DDK_VER)
include $(DDK_SRC_DIR)/make.mk

EXTRA_CFLAGS	:= $(DDK_CFLAGS)
EXTRA_CFLAGS	+= $(addprefix -I$(DDK_SRC_DIR)/,$(DDK_INC_PATH))

obj-m			+= galcore.o
galcore-objs	:= $(addprefix $(DDK_VER)/,$(DDK_OBJS))
