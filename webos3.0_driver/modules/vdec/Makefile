
vdec_src_top := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))


#
# determain chip name and revision
#

CHIP_NAME_$(CONFIG_XTENSA_PLATFORM_D13VDEC)	:= d13
CHIP_REV_$(CONFIG_XTENSA_PLATFORM_D13VDEC)	:= A0
CHIP_NAME_$(CONFIG_XTENSA_PLATFORM_D14VDEC)	:= d14
CHIP_REV_$(CONFIG_XTENSA_PLATFORM_D14VDEC)	:= A0
CHIP_NAME_$(CONFIG_XTENSA_PLATFORM_M14VDEC)	:= m14
CHIP_REV_$(CONFIG_XTENSA_PLATFORM_M14VDEC)	:= A0
CHIP_NAME_$(CONFIG_XTENSA_PLATFORM_H15VDEC)	:= h15
CHIP_REV_$(CONFIG_XTENSA_PLATFORM_H15VDEC)	:= A0
CHIP_NAME_$(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC) := h15
CHIP_REV_$(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC)	:= A0

# in case of xtensa firmware build
ifneq ($(CHIP_NAME_y),)
CHIP_NAME ?= $(CHIP_NAME_y)
CHIP_REV ?= $(CHIP_REV_y)
endif


# in case of tvservice buildrule
ifneq ($(KDRV_PLATFORM_CFG),)
CHIP_NAME ?= $(shell echo $(KDRV_PLATFORM_CFG) | cut -b -3 )
endif


# in case of pioneer buildrule
ifneq ($(PLATFORM_CHIP_NAME),)
CHIP_NAME ?= $(PLATFORM_CHIP_NAME)
endif
ifneq ($(PLATFORM_CHIP_REV),)
CHIP_REV ?= $(PLATFORM_CHIP_REV)
endif


CHIP_NAME ?= a99
CHIP_REV ?= FF

#CHIP_NAME := m16
#CHIP_REV := A0
#PLATFORM_CHIP_TYPE := FPGA



ifneq ($(CONFIG_XTENSA_PLATFORM_D13VDEC),)
CONFIG_LG_BUILTIN_KDRIVER := y
else ifneq ($(CONFIG_XTENSA_PLATFORM_D14VDEC),)
CONFIG_LG_BUILTIN_KDRIVER := y
else ifneq ($(CONFIG_XTENSA_PLATFORM_M14VDEC),)
CONFIG_LG_BUILTIN_KDRIVER := y
else ifneq ($(CONFIG_XTENSA_PLATFORM_H15VDEC),)
CONFIG_LG_BUILTIN_KDRIVER := y
else ifneq ($(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC),)
CONFIG_LG_BUILTIN_KDRIVER := y
endif
ifeq ($(KDRV_MODULE_BUILD), YES)
CONFIG_LG_BUILTIN_KDRIVER := m
endif


#
# ipc driver
#
ifneq ($(filter $(CHIP_NAME),h15),)
ifneq ($(CONFIG_XTENSA),)
obj-y += ipc.o
else
obj-$(CONFIG_LG_BUILTIN_KDRIVER) += ipc.o
endif

ccflags-y		+= -I$(abspath $(vdec_src_top)/../fwload)

ipc-objs += ipc_core.o
ipc-objs += ipc_vdec.o
ifeq ($(CONFIG_XTENSA),)
ipc-objs += ipc_vdec_dummy.o
else
ipc-objs += ipc_vdec_inf.o
endif
endif





#
# HAL implementation directory
#
vdec_haldir_h13_A0  :=  h13/a0
vdec_haldir_h13_A1  :=  h13/a0
vdec_haldir_h13_B0  :=  h13/b0
vdec_haldir_h14_A0  :=  h13/b0
vdec_haldir_m14_A0  :=  m14/a0
vdec_haldir_m14_A1  :=  m14/a0
vdec_haldir_m14_B0  :=  m14/a0
vdec_haldir_m14_C0  :=  m14/a0
vdec_haldir_h15_A0  :=  h15/a0
vdec_haldir_h15_A1  :=  h15/a0
vdec_haldir_m16_A0  :=  m16/a0
vdec_haldir_d13_A0  :=  d13
vdec_haldir_d14_A0  :=  d14

chip_name := $(CHIP_NAME)_$(CHIP_REV)
vdec_haldir := $(vdec_haldir_$(chip_name))

ifeq ($(vdec_haldir),)
$(info unknown chip name for vdec hal directory, $(chip_name))
vdec_haldir := unknown/chip/name
endif


#
# compiled objects
#
vdec-objs		:=
vdec-objs		+= driver.o
vdec-objs		+= audioinf.o
vdec-objs		+= proc.o
vdec-objs		+= output.o
vdec-objs		+= vo.o

vdec-objs		+= ves/ves_drv.o
vdec-objs		+= ves/ves_cpb.o
vdec-objs		+= ves/ves_auib.o

vdec-objs		+= vdc/vdc_drv.o
vdec-objs		+= vdc/vdec_stc_timer.o
vdec-objs		+= vdc/vdc_util.o
vdec-objs		+= vdc/vdc_proc.o
vdec-objs		+= vdc/vdu/vdu_drv.o
vdec-objs		+= vdc/vdu/vdu_debug.o

vdec-objs		+= vds/disp_q.o
vdec-objs		+= vds/disp_clear_q.o
vdec-objs		+= vds/vdisp_drv.o
vdec-objs		+= vds/vdec_rate.o
vdec-objs		+= vds/pts_drv.o
vdec-objs		+= vds/vsync_drv.o
vdec-objs		+= vds/de_if_drv.o
ifeq ($(INCLUDE_KDRV_MMCU),YES)
vdec-objs		+= vds/mcu_if.o
endif
vdec-objs		+= vds/proc.o

vdec-objs		+= hal/hal_proc.o
vdec-objs		+= hal/de_ipc_hal_api.o

vdec-objs		+= hal/$(vdec_haldir)/de_vdo_hal_api.o
vdec-objs		+= hal/$(vdec_haldir)/av_lipsync_hal_api.o
vdec-objs		+= hal/$(vdec_haldir)/ipc_reg_api.o
vdec-objs		+= hal/$(vdec_haldir)/pdec_hal_api.o
vdec-objs		+= hal/$(vdec_haldir)/top_hal_api.o
vdec-objs		+= hal/$(vdec_haldir)/vdc_hal_api.o

ifeq ($(CHIP_NAME),h13)
vdec-objs		+= hal/$(vdec_haldir)/lq_hal_api.o
endif
vdec-objs		+= hal/$(vdec_haldir)/vsync_hal_api.o
ifneq ($(CONFIG_XTENSA_PLATFORM_D13VDEC),)
vdec-objs		+= hal/$(vdec_haldir)/vd3_top_hal.o
endif
ifneq ($(CONFIG_XTENSA_PLATFORM_D14VDEC),)
vdec-objs		+= hal/$(vdec_haldir)/vd3_top_hal.o
vdec-objs		+= hal/$(vdec_haldir)/vcp_reg.o
vdec-objs		+= hal/$(vdec_haldir)/hal.o
endif
vdec-objs		+= hal/$(vdec_haldir)/dif_hal.o

#
# final kernel module
#
obj-$(CONFIG_LG_BUILTIN_KDRIVER) += vdec.o


#
# compile flags
#
ccflags-y		+= -I$(abspath $(vdec_src_top)/../hma)
ccflags-y		+= -I$(vdec_src_top)
ifeq ($(CONFIG_XTENSA_PLATFORM_D13VDEC)$(CONFIG_XTENSA_PLATFORM_D14VDEC)$(CONFIG_XTENSA_PLATFORM_H15VDEC)$(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC),)
ccflags-y		+= -I$(abspath $(vdec_src_top)/../../include)
ccflags-y		+= -I$(abspath $(vdec_src_top)/../base)
ccflags-y		+= -I$(abspath $(vdec_src_top)/../sys)
ccflags-y		+= -I$(abspath $(vdec_src_top)/../..)
ccflags-y		+= -I$(abspath $(vdec_src_top)/../../chip)
endif

ccflags-y		+= -DCHIP_NAME=\"$(CHIP_NAME)\"
ccflags-y		+= -DCHIP_NAME_$(CHIP_NAME)
ccflags-y		+= -DCHIP_REV=0x$(CHIP_REV)

ifeq ($(INCLUDE_KDRV_MMCU),YES)
ccflags-y		+= -DVDEC_USE_MCU
endif

ifeq ($(PLATFORM_CHIP_TYPE),FPGA)
ccflags-y		+= -DCONFIG_VDEC_ON_FPGA
endif

ifneq ($(CONFIG_XTENSA_PLATFORM_H15FPGAVDEC),)
ccflags-y		+= -DCONFIG_VDEC_ON_FPGA
endif


subdir-ccflags-y	+= $(ccflags-y)





#
# D14 control driver
#
ifeq ($(CHIP_NAME),h13)
vdec-objs		+= d14.o
vdec-objs		+= d14_api.o
ccflags-y		+= -I$(abspath $(vdec_src_top)/..)
endif



