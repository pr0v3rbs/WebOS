
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

MXLIB = \
            $(ROOTLIB)/drv/gpio                                     \
            $(ROOTLIB)/drv/cpu                                      \
            $(ROOTLIB)/drv/ve                                       \
            $(ROOTLIB)/drv/dscmb2                                   \
            $(ROOTLIB)/drv/flash/serial                             \
            $(ROOTLIB)/drv/hwi2c                                    \
            $(ROOTLIB)/drv/ddc2bi                                   \
            $(ROOTLIB)/drv/mvop                                     \
            $(ROOTLIB)/drv/pm                                       \
            $(ROOTLIB)/drv/vbi                                      \
            $(ROOTLIB)/drv/mpif                                     \
            $(ROOTLIB)/drv/pwm                                      \
            $(ROOTLIB)/drv/irq                                      \
            $(ROOTLIB)/drv/cipher                                   \
            $(ROOTLIB)/drv/bdma                                     \
            $(ROOTLIB)/drv/mspi                                     \
            $(ROOTLIB)/drv/wble                                     \
            $(ROOTLIB)/api/dac                                      \
            $(ROOTLIB)/api/acp                                      \
            $(ROOTLIB)/api/gfx                                      \
            $(ROOTLIB)/api/gop                                      \
            $(ROOTLIB)/api/vdec                                     \
            $(ROOTLIB)/api/jpeg                                     \
            $(ROOTLIB)/api/dmx                                      \
            $(ROOTLIB)/drv/rasp                                     \
            $(ROOTLIB)/api/pnl                                      \
            $(ROOTLIB)/api/xc                                       \
            $(ROOTLIB)/api/ace                                      \
            $(ROOTLIB)/api/dlc                                      \
            $(ROOTLIB)/drv/ipauth                                   \
            $(ROOTLIB)/drv/sar                                      \
            $(ROOTLIB)/drv/sem                                      \
            $(ROOTLIB)/drv/urdma                                    \
            $(ROOTLIB)/drv/uart                                     \
            $(ROOTLIB)/drv/rtc                                      \
            $(ROOTLIB)/drv/wdt                                      \
            $(ROOTLIB)/drv/ir                                       \
            $(ROOTLIB)/drv/mmfi2                                    \
            $(ROOTLIB)/api/cec                                      \
            $(ROOTLIB)/api/hdmitx                                   \
            $(ROOTLIB)/api/hdmitx2                                  \
            $(ROOTLIB)/drv/pws                                      \
            $(ROOTLIB)/api/swi2c                                    \
            $(ROOTLIB)/drv/demod                                    \
            $(ROOTLIB)/drv/dvb_extdemod                             \
            $(ROOTLIB)/drv/sys                                      \
            $(ROOTLIB)/drv/pcmcia                                   \
            $(ROOTLIB)/drv/ca                                       \
            $(ROOTLIB)/drv/nsk2                                     \

ifneq ($(OS_TYPE),nos)
MXLIB += \
            $(ROOTLIB)/api/audio	                            \
            $(ROOTLIB)/drv/audio                                    \
            $(ROOTLIB)/drv/audsp
endif

ifeq ($(OS_TYPE),ecos)

ifeq ($(USB_TYPE),usb)
MXLIB +=  $(ROOTLIB)/drv/usb_ecos/usb
else ifeq ($(USB_TYPE),usbhost)
MXLIB +=  $(ROOTLIB)/drv/usb_ecos/usbhost
else
MXLIB +=
endif

else
MXLIB += \
            $(ROOTLIB)/drv/usbhost/source/usb_host_p1               \
            $(ROOTLIB)/drv/usbhost/source/usb_hid_p1                \
            $(ROOTLIB)/drv/usbhost/source2/usb_host_p2              \
            $(ROOTLIB)/drv/usbhost/source2/usb_hid_p2
endif

ifneq ($(OS_TYPE),nos)
ifneq ($(OS_TYPE),ucos)
MXLIB +=    $(ROOTLIB)/msfs/$(OS_TYPE)
endif
endif

# for code release purpose...

ifneq ($(OS_TYPE),nos)
MXLIB +=    $(ROOTLIB)/drv/sc                                       \
            $(ROOTLIB)/drv/nds                                      \

MXLIB += $(ROOTLIB)/api/hsl
endif
