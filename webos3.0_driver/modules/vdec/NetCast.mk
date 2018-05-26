LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := vdec_driver

LOCAL_EXPORT_C_INCLUDES += $(call all-c-headers-under, include)

include $(COPY_FILES)
