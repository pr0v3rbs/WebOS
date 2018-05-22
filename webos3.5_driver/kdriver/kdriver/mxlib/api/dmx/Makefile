
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

# **********************************************
# DRV Name
# **********************************************
API_NAME = DMX

# **********************************************
# DRV Source Files
# **********************************************
# tsp2 is prior than rasp

ifneq ($(USERSPACE_STRIP),1)

BOX_CHIPS = u4 j2 k1 k2 kappa
TSP3_CHIPS = agate eiffel monaco miami clippers muji munich mustang
TSP4_CHIPS = kaiser keltic keres

ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
DRV_DIR = \
            tsp2                                                    \
            rasp                                                    \

else ifeq ($(CHIP),maria10)
DRV_DIR = \
            minitsp                                                \

else
DRV_DIR = \
            tsp                            \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP3_CHIPS)))
DRV_DIR = \
            tsp3                                                \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
DRV_DIR = \
            tsp4                                                \
            rasp                                                \

endif

FQ_CHIPS = kappa keltic keres monaco 
ifeq ($(CHIP),$(filter $(CHIP),$(FQ_CHIPS)))
DRV_DIR += fq
endif

MMFI_CHIPS = t12 j2 a2 a1 k1 kappa a5 a7 a3 k2 a5p amethyst eagle edison macaw12 emerald eden euler einstein nike madison nugget nikon einstein3 napoli
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI_CHIPS)))
DRV_DIR += mmfi
endif

MMFI2_CHIPS = agate eiffel kaiser keltic keres monaco miami clippers muji munich mustang
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI2_CHIPS)))
DRV_DIR += mmfi2
endif

TSO_CHIPS = agate einstein nike kaiser keltic einstein3 keres napoli
ifeq ($(CHIP), $(filter $(CHIP),$(TSO_CHIPS)))
DRV_DIR += tso
endif

TSO2_CHIPS = monaco miami munich mustang muji
ifeq ($(CHIP), $(filter $(CHIP),$(TSO2_CHIPS)))
DRV_DIR += tso2
endif

else
DRV_DIR = \

endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

