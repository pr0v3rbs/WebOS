#-----------------------------------------------------------------------------------------
# arch_dev.mk describles the default device description
#-----------------------------------------------------------------------------------------
include $(KDRV_TOP_DIR)/../chip/h14/os/arch_dev.mk

# TODO: List all modules whcih is always set to NO 
# - some modules can't be tested with FPGA env.
# - some modules have their own test plan.
#
INCLUDE_KDRV_SAMPLE := NO
INCLUDE_KDRV_AAD    := NO
INCLUDE_KDRV_AUDIO  := NO
INCLUDE_KDRV_AFE    := NO
INCLUDE_KDRV_CI     := NO
INCLUDE_KDRV_DEMOD  := NO
INCLUDE_KDRV_DENC   := NO
INCLUDE_KDRV_GPIO   := NO
INCLUDE_KDRV_I2C    := NO
INCLUDE_KDRV_VBI    := NO
INCLUDE_KDRV_SE     := NO
INCLUDE_KDRV_SCI    := NO
INCLUDE_KDRV_CLK    := NO
INCLUDE_KDRV_MONITOR:= NO
INCLUDE_KDRV_APR    := NO
INCLUDE_KDRV_MEMPROT:= NO
INCLUDE_KDRV_LGBUS  := NO

# TODO : List all modules which is always set to YES
#
#
INCLUDE_KDRV_LOGM   := YES

# TODO: enable/disable module based on your FPGA test plan.
#
#
#INCLUDE_KDRV_DE     := YES
#INCLUDE_KDRV_FBDEV  := YES
#INCLUDE_KDRV_GFX    := YES
#INCLUDE_KDRV_GPU    := YES
#INCLUDE_KDRV_HDMI   := YES
#INCLUDE_KDRV_PE     := YES
#INCLUDE_KDRV_PVR    := YES
#INCLUDE_KDRV_SDEC   := YES
#INCLUDE_KDRV_MMCU   := YES
#INCLUDE_KDRV_VDEC   := YES
#INCLUDE_KDRV_VENC   := YES
#INCLUDE_KDRV_PNG    := YES
#INCLUDE_KDRV_SYS    := YES
#INCLUDE_KDRV_MJPEG  := YES
#INCLUDE_KDRV_PM     := YES
#INCLUDE_KDRV_BE     := YES
#INCLUDE_KDRV_OVI    := YES

INCLUDE_KDRV_DE     := YES
INCLUDE_KDRV_FBDEV  := YES
INCLUDE_KDRV_GFX    := YES
INCLUDE_KDRV_GPU    := NO
INCLUDE_KDRV_HDMI   := NO
INCLUDE_KDRV_PE     := NO
INCLUDE_KDRV_PVR    := YES
INCLUDE_KDRV_SDEC   := YES
INCLUDE_KDRV_MMCU   := NO
INCLUDE_KDRV_VDEC   := YES
INCLUDE_KDRV_VENC   := YES
INCLUDE_KDRV_PNG    := YES
INCLUDE_KDRV_SYS    := YES
INCLUDE_KDRV_MJPEG  := YES
INCLUDE_KDRV_PM     := YES
INCLUDE_KDRV_BE     := NO
INCLUDE_KDRV_OVI    := NO
