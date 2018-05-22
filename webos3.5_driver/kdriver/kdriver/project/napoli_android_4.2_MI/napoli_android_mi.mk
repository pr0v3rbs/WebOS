################################################################################
#
# Copyright (c) 2008-2012 MStar Semiconductor, Inc.
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

MXLIB := $(ROOTLIB)/api/ace \
         $(ROOTLIB)/api/audio \
         $(ROOTLIB)/api/gfx \
         $(ROOTLIB)/api/gop \
         $(ROOTLIB)/api/gpd \
         $(ROOTLIB)/api/njpeg_ex \
         $(ROOTLIB)/api/mfe_ex \
         $(ROOTLIB)/api/pnl \
         $(ROOTLIB)/api/vdec_ex \
         $(ROOTLIB)/api/xc \
         $(ROOTLIB)/drv/audio \
         $(ROOTLIB)/drv/audsp \
         $(ROOTLIB)/drv/dip \
         $(ROOTLIB)/drv/flash/serial \
         $(ROOTLIB)/drv/ipauth \
         $(ROOTLIB)/drv/mfe_ex \
         $(ROOTLIB)/drv/mvop \
         $(ROOTLIB)/drv/gpio \
         $(ROOTLIB)/drv/pwm \
         $(ROOTLIB)/drv/sar \
         $(ROOTLIB)/drv/mbx \
         $(ROOTLIB)/api/mbx \
         $(ROOTLIB)/drv/ve \
	 $(ROOTLIB)/drv/aesdma \
	 $(ROOTLIB)/msfs/linux \
         $(ROOTLIB)/api/cec \
         $(ROOTLIB)/api/dlc \
         $(ROOTLIB)/api/dmx \
         $(ROOTLIB)/api/mhl \
         $(ROOTLIB)/api/swi2c \
         $(ROOTLIB)/drv/avd \
         $(ROOTLIB)/drv/cpu \
         $(ROOTLIB)/drv/ddc2bi \
         $(ROOTLIB)/drv/demod \
         $(ROOTLIB)/drv/dscmb \
         $(ROOTLIB)/drv/ir \
         $(ROOTLIB)/drv/hwi2c \
         $(ROOTLIB)/drv/mmio \
         $(ROOTLIB)/drv/mspi \
         $(ROOTLIB)/drv/pcmcia \
         $(ROOTLIB)/drv/pm \
         $(ROOTLIB)/drv/pws \
         $(ROOTLIB)/drv/rtc \
         $(ROOTLIB)/drv/sc \
         $(ROOTLIB)/drv/sys \
         $(ROOTLIB)/drv/vbi \
         $(ROOTLIB)/drv/vif \
         $(ROOTLIB)/drv/wdt 
