LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#ifeq ($(PLATFORM_CHIP_VER),lg1311)
#==============================================================================
# local specific variable
#==============================================================================

-include $(SOC_LINUX_DIR)/NC_LGSOC_Specific.mk

#==============================================================================
# NetCast rules
#==============================================================================

#need discussion
LOCAL_MODULE := $(TARGET_PLATFORM)_kdriver

.PHONY: $(LOCAL_MODULE) clean clean-$(LOCAL_MODULE)

$(LOCAL_MODULE): PRIVATE_PATH := $(LOCAL_PATH)/core
$(LOCAL_MODULE): REDIRECTED_DIR := $(if $(filter mk_nfs,$(MAKECMDGOALS)),$(OUT_DIR)/mnt/lg,$(TEMP_DIR)/webos/mnt/lg)
$(LOCAL_MODULE):
	@$(MAKE) $(JOB_NR) -C $(PRIVATE_PATH) $(KERNEL_MAKE_OPTS) $(MAKE_OPTS) $(GOAL) $(REL) ARCH=arm  OUT_DIR=$(REDIRECTED_DIR) || exit 1

ifeq ($(GOAL),)
#	@mkdir -p $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/de/firmware
	@mkdir -p $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/build
	@rsync -uz $(PRIVATE_PATH)/build/* $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/build
#	@rsync -uz $(PRIVATE_PATH)/de/firmware/*  $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/de/firmware
#	@rsync -z $(PRIVATE_PATH)/vdec/vdc/vdu/*.ko  $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/build
#	@rsync -z $(PRIVATE_PATH)/venc/h13b0/*.ko  $(PREBUILT_ETC_FILES)/$(TARGET_PLATFORM)_kdriver/build
endif
clean:	clean-$(LOCAL_MODULE)
clean-$(LOCAL_MODULE) : PRIVATE_PATH := $(LOCAL_PATH)/core
clean-$(LOCAL_MODULE) :
	@echo "Cleaning $(PRIVATE_PATH) ...."
	@$(MAKE) -C $(PRIVATE_PATH) $(KERNEL_MAKE_OPTS) $(MAKE_OPTS) clean || exit 1

ifeq ($(OS_LG115X_LINUX),)
ALL_PREBUILT += $(LOCAL_MODULE)
endif


#LOCAL_EXPORT_COMPONENT := kdriver-goldfinger-$(shell date +%Y%m%d)-1
#LOCAL_EXPORT_ETC_FILES := $(call all-files-under, core/build, "*.ko")
LOCAL_EXPORT_COMPONENT_NAME := kdriver
LOCAL_EXPORT_ETC_FILES := $(call all-files-under, core/build, "*.ko")
LOCAL_EXPORT_ETC_FILES += $(call all-files-under, ../../result/kdriver_builtin, "kdrv_$(TARGET_PLATFORM_CHIP).o")
LOCAL_EXPORT_ETC_FILES += $(call all-files-under, ../../result/kdriver_builtin, "logfunnel.o")
LOCAL_EXPORT_ETC_FILES += $(call all-files-under, ../../result/kdriver_builtin, "hma.o")
LOCAL_EXPORT_ETC_FILES += $(call all-files-under, ../../result/kdriver_builtin, "vdec.o")
LOCAL_EXPORT_ETC_FILES += $(call all-files-under, builtin, "Makefile.$(TARGET_PLATFORM_CHIP)")
LOCAL_EXPORT_C_INCLUDES := $(call all-c-headers-under, include)
LOCAL_EXPORT_C_INCLUDES += $(call all-c-headers-under, core/hma)
LOCAL_EXPORT_C_INCLUDES += $(call all-c-headers-under, core/logfunnel)
LOCAL_EXPORT_UPSTART_FILES := $(call all-files-under, files/launch, "*")
LOCAL_EXPORT_UPSTART2_FILES := $(call all-files-under, files/launch2, "*")

include $(COPY_FILES)

#include $(call first-makefiles-under, $(LOCAL_PATH))
include $(LOCAL_PATH)/core/logfunnel/NetCast.mk \
	$(LOCAL_PATH)/core/hma/NetCast.mk

.PHONY: test
test:
	echo $(PRIVATE_PATH)
	echo $(KERNEL_MAKE_OPTS)
	echo $(MAKE_OPTS)
#==============================================================================
#endif
