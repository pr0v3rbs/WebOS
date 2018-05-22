
################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
# Makefile used for building DDI
#
#
################################################################################

MXLIB := $(ROOTLIB)/api/gop                                      \
         $(ROOTLIB)/api/gfx                                      \
         $(ROOTLIB)/api/audio                                    \
         $(ROOTLIB)/api/dmx                                      \
         $(ROOTLIB)/api/pnl                                      \
         $(ROOTLIB)/api/xc                                       \
         $(ROOTLIB)/api/ace                                      \
         $(ROOTLIB)/api/dlc                                      \
         $(ROOTLIB)/api/cec                                      \
         $(ROOTLIB)/api/gpd                                      \
         $(ROOTLIB)/api/swi2c                                    \
         $(ROOTLIB)/api/njpeg_ex                                 \
         $(ROOTLIB)/api/vdec_ex                                  \
         $(ROOTLIB)/api/mhl                                      \
         $(ROOTLIB)/api/hsl                                      \
         $(ROOTLIB)/drv/pws                                      \
         $(ROOTLIB)/drv/vif                                      \
         $(ROOTLIB)/drv/rtc                                      \
         $(ROOTLIB)/drv/cpu                                      \
         $(ROOTLIB)/drv/pwm                                      \
         $(ROOTLIB)/drv/pm                                       \
         $(ROOTLIB)/drv/wdt                                      \
         $(ROOTLIB)/drv/urdma                                    \
         $(ROOTLIB)/drv/flash/serial                             \
         $(ROOTLIB)/drv/aesdma                                   \
         $(ROOTLIB)/drv/audsp                                    \
         $(ROOTLIB)/drv/sar                                      \
         $(ROOTLIB)/drv/pcmcia                                   \
         $(ROOTLIB)/drv/dscmb                                    \
         $(ROOTLIB)/drv/mvop                                     \
         $(ROOTLIB)/drv/tsp                                      \
         $(ROOTLIB)/drv/mpif                                     \
         $(ROOTLIB)/drv/dip                                      \
         $(ROOTLIB)/drv/avd                                      \
         $(ROOTLIB)/drv/vbi                                      \
         $(ROOTLIB)/drv/ir                                       \
         $(ROOTLIB)/drv/ve                                       \
         $(ROOTLIB)/drv/usbhost/source/usb_host_p1               \
         $(ROOTLIB)/drv/usbhost/source/usb_hid_p1                \
         $(ROOTLIB)/drv/usbhost/source2/usb_host_p2              \
         $(ROOTLIB)/drv/usbhost/source2/usb_hid_p2               \
         $(ROOTLIB)/drv/usbhost/source3/usb_host_p3              \
         $(ROOTLIB)/drv/usbhost/source3/usb_hid_p3               \
         $(ROOTLIB)/drv/ipauth                                   \
         $(ROOTLIB)/drv/hwi2c                                    \
         $(ROOTLIB)/drv/gpio                                     \
         $(ROOTLIB)/drv/mmfi                                     \
         $(ROOTLIB)/drv/ddc2bi                                   \
         $(ROOTLIB)/drv/demod                                    \
         $(ROOTLIB)/drv/audio		                         \
         $(ROOTLIB)/drv/mspi

ifeq ($(OS_TYPE), linux) 
ifeq ($(PLATFORM), android) # android
         $(ROOTLIB)/api/mfe_ex                                   \
         $(ROOTLIB)/drv/mfe_ex                                   \
         $(ROOTLIB)/drv/sc

else # linux
MXLIB += $(ROOTLIB)/msfs/$(OS_TYPE)                              \
         $(ROOTLIB)/api/mfe                                      \
         $(ROOTLIB)/drv/mfe                                      \
         $(ROOTLIB)/drv/sc                                       \
endif

else # nos
MXLIB += $(ROOTLIB)/drv/miu                                      \
         $(ROOTLIB)/drv/irq                                      \
         $(ROOTLIB)/drv/sys                                      \
         $(ROOTLIB)/drv/uart                                     \
         $(ROOTLIB)/drv/bdma                                     \
         $(ROOTLIB)/drv/sem                                      \
         $(ROOTLIB)/drv/mmio                                     \
         $(ROOTLIB)/drv/mbx                                      \
endif
