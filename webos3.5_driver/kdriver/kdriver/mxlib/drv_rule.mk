
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

unexport HAL_DIR
unexport DRV_SUB
unexport DRV_NAME
unexport DRV_SRC
unexport DRV_CC_INCS


# **********************************************
# DRV Name
# **********************************************
#DRV_NAME = ddd
#DRV_SUB = \
#            sss

# **********************************************
# HAL Source Files
# **********************************************
#HAL_DIR = \
#            hhh		                                             \


# **********************************************
# Rules
# **********************************************
# if API_NAME is defined the driver will be built into API library
# else driver will be built in single driver library
ifneq ($(API_NAME), )
LIB_NAME = api$(API_NAME)
T_DIR_NAME = $(API_NAME)_T
else
LIB_NAME = drv$(DRV_NAME)
T_DIR_NAME = $(DRV_NAME)_T
endif

DRV_SRC     = $(wildcard $(CURDIR)/*.c)
DRV_SRC    += $(foreach dir, $(DRV_SUB), $(wildcard $(CURDIR)/$(dir)/*.c))
DRV_OBJ     = $(foreach file, $(notdir $(patsubst %.c, %.o, $(DRV_SRC))), $(OBJ_PATH)/$(file))
DRV_IMPORT_SRC = $(foreach dir, $(CURDIR), $(wildcard $(CURDIR)/obj_import/$(OS_TYPE)/$(MCU_TYPE)/*.src))
DRV_IMPORT_OBJ = $(foreach file, $(notdir $(patsubst %.src, %.o, $(DRV_IMPORT_SRC))), $(OBJ_PATH)/$(file))
HAL_SRC     = $(foreach dir, $(HAL_DIR), $(wildcard $(DDIHAL)/$(CHIP)/$(dir)/*.c))
HAL_SRC_S   = $(foreach dir, $(HAL_DIR), $(wildcard $(DDIHAL)/$(CHIP)/$(dir)/*.s))
HAL_OBJ     = $(foreach file, $(notdir $(patsubst %.c, %.o, $(HAL_SRC))), $(OBJ_PATH)/$(file))
HAL_OBJ_S   = $(foreach file, $(notdir $(patsubst %.s, %.o, $(HAL_SRC_S))), $(OBJ_PATH)/$(file))
HAL_IMPORT_SRC = $(foreach dir, $(HAL_DIR), $(wildcard $(DDIHAL)/$(CHIP)/$(dir)/obj_import/$(OS_TYPE)/$(MCU_TYPE)/*.src))
HAL_IMPORT_OBJ = $(foreach file, $(notdir $(patsubst %.src, %.o, $(HAL_IMPORT_SRC))), $(OBJ_PATH)/$(file))
DRV_INCDIR  = $(CURDIR) $(foreach dir,$(DRV_SUB),$(CURDIR)/$(dir)) $(foreach dir,$(HAL_DIR),$(DDIHAL)/$(CHIP)/$(dir))
DRV_CC_INCS = $(foreach dir,$(DRV_INCDIR),-I$(dir) -I$(PUBINC))

ifeq ($(MAKE_TYPE),lint)
ifeq ($(shell uname -o),Cygwin)
LINT_INC   += $(foreach dir,$(shell cygpath -m $(DRV_INCDIR)),-i"$(dir)")
else
LINT_INC   += $(foreach dir,$(DRV_INCDIR),-i"$(dir)")
endif
endif

HAL_OBJ_T = $(foreach file, $(notdir $(patsubst %.o, %.T, $(HAL_OBJ))), $(OBJ_PATH)/$(T_DIR_NAME)/$(file))
HAL_IMPORT_OBJ_T = $(foreach file, $(notdir $(patsubst %.o, %.T, $(HAL_IMPORT_OBJ))), $(OBJ_PATH)/$(T_DIR_NAME)/$(file))
DRV_OBJ_T = $(foreach file, $(notdir $(patsubst %.o, %.T, $(DRV_OBJ))), $(OBJ_PATH)/$(T_DIR_NAME)/$(file))
DRV_IMPORT_OBJ_T = $(foreach file, $(notdir $(patsubst %.o, %.T, $(DRV_IMPORT_OBJ))), $(OBJ_PATH)/$(T_DIR_NAME)/$(file))

VPATH = $(DRV_SUB) $(foreach dir, $(HAL_DIR), $(DDIHAL)/$(CHIP)/$(dir))

ifeq ("$(UTOPIA_BSP_VERSION)", "")
UTOPIA_BSP_VERSION = 000000
else
CC_DEFS += -DUTOPIA_BSP_VERSION=$(UTOPIA_BSP_VERSION)
endif

ifeq ("$(P4PORT)", "")
SW_VERSION_HAL_$(DRV_NAME) = 0
else
#HAL_SW_DIR = $(shell echo $(HAL_DIR) | sed 's/ vpu//g')
#SW_VERSION_HAL_$(DRV_NAME) = $(shell p4 changes -m1 ./../../hal/$(CHIP)/$(HAL_SW_DIR)/...\#have | egrep Change | awk '{print $$2}')
ifeq ("$(SW_VERSION_HAL_$(DRV_NAME))", "")
SW_VERSION_HAL_$(DRV_NAME) = 0
endif
endif
CC_DEFS += -DSW_VERSION_HAL_$(DRV_NAME)=$(SW_VERSION_HAL_$(DRV_NAME))

ifeq ("$(P4PORT)", "")
SW_VERSION_DRV_$(DRV_NAME) = 0
else
#SW_VERSION_DRV_$(DRV_NAME) = $(shell p4 changes -m1 "./...\#have" | egrep Change | awk '{print $$2}')
ifeq ("$(SW_VERSION_DRV_$(DRV_NAME))", "")
SW_VERSION_DRV_$(DRV_NAME) = 0
endif
endif
CC_DEFS += -DSW_VERSION_DRV_$(DRV_NAME)=$(SW_VERSION_DRV_$(DRV_NAME))

ifeq ("$(HAL_DIR)", "demod")
CC_DEFS += -D$(CHIP)
endif

all : $(DRV_OBJ) $(DRV_IMPORT_OBJ) $(HAL_OBJ_S) $(HAL_OBJ) $(HAL_IMPORT_OBJ) $(HAL_OBJ_T) $(HAL_IMPORT_OBJ_T) $(DRV_OBJ_T) $(DRV_IMPORT_OBJ_T)

ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
ifneq ($(BUILDING_API),1)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) $(DRV_OBJ) $(DRV_IMPORT_OBJ) $(HAL_OBJ_S) $(HAL_OBJ) $(HAL_IMPORT_OBJ) -s -o $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
endif
else
	@$(AR) cru $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $(DRV_OBJ) $(DRV_IMPORT_OBJ) $(HAL_OBJ) $(HAL_IMPORT_OBJ)
	@$(STRIP) -S $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
endif
endif

ifeq ($(PARA), 1)
	@if [ -f $(CURDIR)/cpptestscan.bdf ]; then	\
		rm -f $(CURDIR)/cpptestscan.bdf;                                             \
	fi
endif


ifeq ($(TOOLCHAIN),mips-linux-gnu)
ifeq ($(OS_TYPE),nos)
SW_MBOOT_FLOW = TRUE
endif
endif


ifneq ($(TOOLCHAIN),mipsisa32-elf)
ifneq ($(OS_TYPE),ucos)
ifneq ($(SW_MBOOT_FLOW),TRUE)
ifneq ($(SW_API_FLOW_ENABLE),TRUE)
ifneq ($(PARA), 1)
ifneq ($(APPEND_STRING),FALSE)
	echo "++MSTAR::UTOPIA_BSP_VERSION::$(UTOPIA_BSP_VERSION)::MSTAR++" >> $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	echo "++MSTAR::SW_VERSION_HAL_$(DRV_NAME)::$(SW_VERSION_HAL_$(DRV_NAME))::MSTAR++" >> $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	echo "++MSTAR::SW_VERSION_DRV_$(DRV_NAME)::$(SW_VERSION_DRV_$(DRV_NAME))::MSTAR++" >> $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
endif
endif
endif
endif
endif
endif

$(HAL_OBJ_T) $(HAL_IMPORT_OBJ_T) $(DRV_OBJ_T) $(DRV_IMPORT_OBJ_T): | $(DRV_OBJ) $(DRV_IMPORT_OBJ) $(HAL_OBJ_S) $(HAL_OBJ) $(HAL_IMPORT_OBJ)

$(DRV_OBJ) $(DRV_IMPORT_OBJ) $(HAL_OBJ_S) $(HAL_OBJ) $(HAL_IMPORT_OBJ) $(HAL_OBJ_T) $(HAL_IMPORT_OBJ_T) $(DRV_OBJ_T) $(DRV_IMPORT_OBJ_T) : | setup

lib : setup all

$(DRV_IMPORT_OBJ) :
	echo find drv import_obj cp from $(filter %$(notdir $*).src,$(DRV_IMPORT_SRC)) to $@
	cp $(filter %$(notdir $*).src,$(DRV_IMPORT_SRC)) $@

$(HAL_IMPORT_OBJ):
	echo find hal import_obj cp from $(filter %$(notdir $*).src,$(HAL_IMPORT_SRC)) to $@
	cp $(filter %$(notdir $*).src,$(HAL_IMPORT_SRC)) $@

$(HAL_OBJ_S) : $(OBJ_PATH)/%.o : %.s
	@echo [AS] $(notdir $<)
	$(AS) -march=armv7-a -mfpu=neon -o $@ $<;

$(DRV_OBJ) $(HAL_OBJ) : $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@echo [CC] $(notdir $<)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) $(DRV_CC_INCS) -o $@ $<;
ifeq ($(PARA), 1)
	@if [ ! -f $(CURDIR)/cpptestscan.bdf ]; then                                         \
		echo ERROR! No $(CURDIR)/cpptestscan.bdf;                                    \
	fi
	@if [ -f $(CURDIR)/cpptestscan.bdf ]; then                                           \
		cat $(CURDIR)/cpptestscan.bdf >> $(PROJ)/cpptestscan.bdf;    \
		rm -f $(CURDIR)/cpptestscan.bdf;                                                             \
	fi
endif


else
	@echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
#	@echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
#	@echo $< >>$(LINT_FILES)
endif
endif


$(HAL_OBJ_T) $(HAL_IMPORT_OBJ_T) $(DRV_OBJ_T) $(DRV_IMPORT_OBJ_T):
ifeq ($(BUILDING_API),1)
	cp  $(OBJ_PATH)/$(notdir $(patsubst %.T, %.o, $@)) $@;
endif


setup:
ifeq ($(LINK_TYPE),dynamic)
	@echo [drv_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
else
	@echo [drv_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
endif

ifeq ($(BUILDING_API),1)
	@mkdir -p $(OBJ_PATH)/$(T_DIR_NAME)
endif

clean :
	@rm -f $(HAL_OBJ_S)
	@rm -f $(HAL_OBJ)
	@rm -f $(HAL_IMPORT_OBJ)
	@rm -f $(DRV_OBJ)
	@rm -f $(DRV_IMPORT_OBJ)
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).a
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).so



