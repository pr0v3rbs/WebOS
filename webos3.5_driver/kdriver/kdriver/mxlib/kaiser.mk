
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

ifeq ($(CHIP), kaiser)

MXLIB =\
            $(ROOTLIB)/drv/gpio                                     \
            $(ROOTLIB)/drv/cpu                                      \
            $(ROOTLIB)/drv/ve                                       \
            $(ROOTLIB)/drv/flash/serial                             \
            $(ROOTLIB)/drv/hwi2c                                    \
            $(ROOTLIB)/drv/ddc2bi                                   \
            $(ROOTLIB)/drv/mvop                                     \
            $(ROOTLIB)/drv/pm                                       \
            $(ROOTLIB)/drv/vbi                                      \
            $(ROOTLIB)/drv/mpif                                     \
            $(ROOTLIB)/drv/pwm                                      \
            $(ROOTLIB)/drv/cipher                                   \
            $(ROOTLIB)/drv/dscmb2                                   \
            $(ROOTLIB)/drv/wble                                     \
            $(ROOTLIB)/api/dac                                      \
            $(ROOTLIB)/api/acp                                      \
            $(ROOTLIB)/api/gfx                                      \
            $(ROOTLIB)/api/gop                                      \
            $(ROOTLIB)/api/dlc                                      \
            $(ROOTLIB)/api/dmx                                      \
            $(ROOTLIB)/drv/ipauth                                   \
            $(ROOTLIB)/drv/sar                                      \
            $(ROOTLIB)/drv/urdma                                    \
            $(ROOTLIB)/drv/rtc                                      \
            $(ROOTLIB)/drv/wdt                                      \
            $(ROOTLIB)/drv/ir                                       \
            $(ROOTLIB)/api/cec                                      \
            $(ROOTLIB)/api/hdmitx                                   \
            $(ROOTLIB)/drv/pws                                      \
            $(ROOTLIB)/api/swi2c                                    \
            $(ROOTLIB)/drv/demod                                    \
            $(ROOTLIB)/drv/pcmcia                                   \
            $(ROOTLIB)/api/gpd                                      \
            $(ROOTLIB)/drv/rasp                                     \
            $(ROOTLIB)/drv/mmfi2                                   \
            $(ROOTLIB)/drv/tso                                     \
            $(ROOTLIB)/drv/dip                                     \
            $(ROOTLIB)/api/jpeg_ex	                                \
            $(ROOTLIB)/api/vdec_ex                                  \
            $(ROOTLIB)/api/ace_ex                                   \
            $(ROOTLIB)/api/gopscd                             \
            $(ROOTLIB)/api/pnl_ex                                   \
            $(ROOTLIB)/api/xc_ex				                            \
			      $(ROOTLIB)/drv/ch34                                     \
            $(ROOTLIB)/drv/ca2                                      \

MXLIB += $(ROOTLIB)/api/mbx

ifneq ($(OS_TYPE),nos)
MXLIB += \
            $(ROOTLIB)/api/audio	                                \
            $(ROOTLIB)/drv/audsp                                    \

endif

ifeq ($(NSK2_ENBALBE), y)
MXLIB +=  $(ROOTLIB)/drv/nsk2                                    
endif

ifneq ($(OS_TYPE),nos)
ifneq ($(OS_TYPE),ucos)
MXLIB +=    $(ROOTLIB)/msfs/$(OS_TYPE)
endif
endif

# for code release purpose...

ifeq ($(OS_TYPE), linux)
MXLIB += $(ROOTLIB)/api/mfe_ex
MXLIB += $(ROOTLIB)/drv/mfe_ex
MXLIB += $(ROOTLIB)/api/vdec
MXLIB += $(ROOTLIB)/drv/sc

else # nos
MXLIB += $(ROOTLIB)/api/vdec                                     \
         $(ROOTLIB)/drv/miu                                      \
         $(ROOTLIB)/drv/irq                                      \
         $(ROOTLIB)/drv/sys                                      \
         $(ROOTLIB)/drv/uart                                     \
         $(ROOTLIB)/drv/bdma                                     \
         $(ROOTLIB)/drv/sem                                      \
         $(ROOTLIB)/drv/mmio         
endif

endif

