#-----------------------------------------------------------------------------------------
# arch_dev.mk describles the default device description
#-----------------------------------------------------------------------------------------
include $(KDRV_TOP_DIR)/../chip/h13/os/arch_dev.mk

INCLUDE_KDRV_LOGM       := YES

ccflags-y += -D__ANDROID__
