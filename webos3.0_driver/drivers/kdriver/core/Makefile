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
#   FILE NAME   :   makefile                                                   #
#   VERSION     :   1.0                                                        #
#   AUTHOR      :   raxis@lge.com                                              #
#   DATE        :   2010/11/17                                                 #
#   DESCRIPTION :   Makefile for building LG1150 kernel driver                 #
#******************************************************************************#

ifeq ($(KERNEL_NAME),)
$(error KERNEL_NAME is not defined, please define this variable above )
endif
ifeq ($(PLATFORM_CHIP),)
$(error PLATFORM_CHIP is not defined, please define this variable above )
endif

ifeq ($(V),)
Q ?= @
endif

#==============================================================================
#
#	BUILD FLAGS
#
#==============================================================================
# if KDRV_GLOBAL_LINK is YES, the big one kernel module is created.
# if NO, all modules creates its own kernel module.
# default value is YES
KDRV_GLOBAL_LINK	?= YES

# if you want to test a power management, set KDRV_CONFIG_PM to YES
# default value is NO
KDRV_CONFIG_PM		?= YES

# enable debug feature
DEBUG			?= y

SIC_LAB_BOARD		?= NO

export KDRV_GLOBAL_LINK KDRV_CONFIG_PM DEBUG

ifeq ($(KERNELVERSION),)
#==============================================================================
#
#	KERNEL MODULE
#
#==============================================================================
KDRV_TOP_DIR		:= $(shell pwd)
KDRV_BUILD_DIR		:= $(KDRV_TOP_DIR)/build
KDRV_FW_DIR			:= $(realpath $(KDRV_TOP_DIR)/../../../firmware)
ROOT_DIR 			:= $(shell cd ../../../.. && pwd)

OUT_DIR				?= /share/global_platform/$(PLATFORM_CHIP)/$(USER)/res/lglib
TFTP_DIR			?= /tftpboot/$(PLATFORM_CHIP)/$(USER)/pak-pioneer_columbus/res/lglib/kdrv

# KERNEL_NAME should be defined outside of this makefile
# If you want to fix KERNEL_NAME for test. uncomment the below line
# KERNEL_NAME		?= linux-2.6.33-mvista
ifeq ($(KERNEL_PATH),)
KERNEL_DIR			:= $(KDRV_TOP_DIR)/../../../kernel/$(KERNEL_NAME)
else
KERNEL_DIR			:= $(KERNEL_PATH)
endif

# this flag makes kdriver to be kernel module regardless of kernel configuration
KDRV_MODULE_BUILD 	?= YES

# export core environment variable for each module Makefile
export KDRV_TOP_DIR OUT_DIR KDRV_OUT_DIR TFTP_DIR KERNEL_DIR KERNEL_NAME
export KDRV_MODULE_BUILD

include $(KDRV_TOP_DIR)/modules.mk

export INCLUDE_L8_CHIP_KDRV
export INCLUDE_L9_CHIP_KDRV
export INCLUDE_H13_CHIP_KDRV
export INCLUDE_H15_CHIP_KDRV

KDIFF				= ../../../tools/kdiff.sh

PLATFORM_CHIP_NAME ?= $(shell echo $(KDRV_PLATFORM_CFG) | cut -b -3 )

#------------------------------------------------------------------------------
# default target
#------------------------------------------------------------------------------
all : build_kdriver
	# KDRIVER BUILD DONE...

clean : clean_kdriver
	# KDRIVER CLEAN DONE...

#------------------------------------------------------------------------------
# PHONEY target list
#------------------------------------------------------------------------------
.PHONEY: pre_build_kdriver

ifeq ($(KERNEL_PATH),)
build_kdriver : pre_build_kdriver
else
build_kdriver :
endif
	$(Q)rm hma -Rf && ln -sf ../../../modules/hma
	$(Q)ln -sf ../../../modules/vdec
#	$(Q)ln -sf ../../../modules/vdec_cnm
#	$(Q)ln -sf ../../../modules/venc_hxenc
	$(Q)ln -sf ../../../modules/fwload
ifeq ($(KDRV_MODULE_BUILD), YES)
	$(Q)$(ECHO) "- build kdriver modules..."
	$(Q)$(MAKE) -C $(KERNEL_DIR) M=$(KDRV_TOP_DIR) modules

	$(Q)$(ECHO) "- install kdriver modules..."
	$(Q)rm -rf $(KDRV_TOP_DIR)/target
	$(Q)$(MAKE) -C $(KERNEL_DIR) M=$(KDRV_TOP_DIR) modules_install INSTALL_MOD_PATH=$(KDRV_TOP_DIR)/target
	$(Q)find ${KDRV_TOP_DIR}/target -name \*.ko -print0 | xargs -0 cp -t ${KDRV_BUILD_DIR}

	$(Q)$(ECHO) "- copy prebuilt ko..."
	$(Q)for ko in $(PREBUILT_KO); do cp $$ko ${KDRV_BUILD_DIR}; done;

	$(Q)$(ECHO) "- strip debug symbol..."
	$(Q)for ko in $(KDRV_BUILD_DIR)/*.ko; do $(STRIP) -d $$ko; done;

	$(Q)$(ECHO) "- copy firmware..."
	$(Q)$(RM) -rf $(KDRV_FW_OUT_DIR)
	$(Q)$(MKDIR)  $(KDRV_FW_OUT_DIR)
	$(Q)[ -d $(KDRV_FW_DIR) ] && for fw in `/bin/ls $(KDRV_FW_DIR)` ; do echo "  INSTALL $(KDRV_FW_DIR)/$$fw"; $(CP) -f $(KDRV_FW_DIR)/$$fw $(KDRV_FW_OUT_DIR); done

ifeq ($(USE_CITRIX_SERVER), YES)
	$(Q)$(MAKE) prodgsync_kdriver
else
ifeq ($(KERNEL_PATH),)
	$(Q)cd $(KDRV_BUILD_DIR) && find . -name \*.ko -exec $(IS_DIFFER) {} $(KDRV_OUT_DIR) $(OUT_DIR) \; | $(CPIO_CMD) $(KDRV_OUT_DIR)
endif
endif
endif
clean_kdriver :
	$(Q)rm -rf $(KDRV_TOP_DIR)/target
	$(Q)find $(KDRV_TOP_DIR)/.. -name "*.o" -exec rm {} \;
	$(Q)-$(MAKE) -C $(KERNEL_DIR) M=$(KDRV_TOP_DIR) clean

#------------------------------------------------------------------------------
# copy kernel driver to predefined directory ( USE_CITRIX_SERVER=YES )
#------------------------------------------------------------------------------
prodgsync_kdriver:
	$(Q)$(ECHO)
	$(Q)$(ECHO) "<!> LG KERNEL DRIVER : COPYING(SYNC) DRIVER.."
	$(Q)$(ECHO)

	$(Q)$(RM) $(CHANGE_FILE_LIST) && touch $(CHANGE_FILE_LIST)

ifeq ($(KDRV_GLOBAL_LINK), YES)
ifeq ($(KDRV_MODULE_BUILD), YES)
ifeq ($(PLATFORM_CHIP), lg1311)
	$(Q)ln -vnsf ./kdrv_$(PLATFORM_CHIP).ko $(KDRV_BUILD_DIR)/kdrv_$(PLATFORM).ko
endif
	$(Q)cd $(KDRV_BUILD_DIR) && find . -name \*.ko -exec $(IS_DIFFER) {} $(KDRV_OUT_DIR) $(OUT_DIR) \; | $(CPIO_CMD) $(KDRV_OUT_DIR)
endif
endif
	$(Q)cd $(KDRV_BUILD_DIR) && find lgmknods.sh	 -exec $(IS_DIFFER) {} $(KDRV_OUT_DIR) $(OUT_DIR) \; | $(CPIO_CMD) $(KDRV_OUT_DIR)

	# Copying firmware
	$(Q)cd $(KDRV_TOP_DIR)/../../../firmware && find '*' -exec $(IS_DIFFER) {} $(KDRV_FW_OUT_DIR) $(OUT_DIR) \; | $(CPIO_CMD) $(KDRV_FW_OUT_DIR)

	$(Q)$(TRANS_IMAGES) $(OUT_DIR)

	# progsync kernel driver if file is not empty...
	$(Q)if [ -s $(CHANGE_FILE_LIST) ]; then						\
		GZIP="--fast" $(PROD_GSYNC) $(OUT_DIR) | tr ":" ".";	\
	else														\
		touch $(CHANGE_FILE_FLAG);								\
	fi


check_kver:
ifeq ($(PLATFORM_CHIP),lg1154)
	$(Q)$(MAKE) -C ../../../kernel -f Makefile_webos check_kver
else
	$(Q)$(MAKE) -C ../../../kernel check_kver
endif

#------------------------------------------------------------------------------
# TODO) check if kdriver is patched successfully or not
#------------------------------------------------------------------------------
#pre_build_kdriver : check_kver
pre_build_kdriver :
	$(Q)$(ECHO)
	$(Q)$(ECHO) "<!> LG KERNEL DRIVER : MAKING OUT_DIR & TFTP_DIR..."
	$(Q)$(ECHO)
	$(Q)$(MKDIR) $(OUT_DIR)
	$(Q)$(MKDIR) $(TFTP_DIR)



else # KERNELVERSION

#==============================================================================
#
#	KERNEL BUILTIN
#
#==============================================================================
# the below paramter is used when kdriver is built into kernel
# KDRV_TOP_DIR means the "top" directory for kdriver core file(s)
#
KDRV_TOP_DIR        := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
export KDRV_TOP_DIR

include $(KDRV_TOP_DIR)/modules.mk
include $(addprefix $(KDRV_TOP_DIR)/,$(addsuffix /Makefile,$(BASE_MODULE)))

# raxis.lim (2014/11/14)
# if USE_FAKE_WEBOS_OEBUILDis defined, we won't make kdrv_lg115x.ko becuase kdriver is built-in into kernel.
# KDRV_MODULE_BUILD parser will result in the unncessary kdriver module build.
#
ifeq ($(USE_FAKE_WEBOS_OEBUILD), YES)
#
# Do Nothing !!
#
else
# if KDRV_MODULE_BUILD is enabled, we will make kdrv_lg115x.ko regardless of kernel configuration.
# KDRV_MODULE_BUILD shouild be defined at platform Makefile.
#
ifeq ($(KDRV_MODULE_BUILD), YES)
CONFIG_LG_BUILTIN_KDRIVER := m
endif

endif

obj-$(CONFIG_LG_BUILTIN_KDRIVER)	+= kdrv_$(PLATFORM_CHIP).o
kdrv_$(PLATFORM_CHIP)-objs 			:= $(KDRV_MODULE_OBJS)

ifneq ($(filter $(PLATFORM_CHIP_NAME),h15),)
obj-y	+= fwload/
endif
obj-y	+= vdec/
#obj-y	+= vdec_cnm/
#obj-y	+= venc_hxenc/
obj-y	+= hma/

ifeq ($(PLATFORM_CHIP), lg1210)
obj-y	+= ../chip/h15/cpc/
#obj-y	+= ../chip/h15/esm/esm_hdmi_sdk/esm_host_lib/host_lib_driver/linux/kernel/
endif


endif # KERNELVERSION
