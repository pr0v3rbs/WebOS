
fwload_src_top := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))

ccflags-y += -I$(abspath $(fwload_src_top)/../hma)
ccflags-y += -I$(abspath $(fwload_src_top)/../sys)
ccflags-y += -I$(abspath $(fwload_src_top)/../..)
ccflags-y += -I$(abspath $(fwload_src_top)/../../include)
ccflags-y += -I$(abspath $(fwload_src_top)/../../chip)
ccflags-y += -DCHIP_NAME_$(PLATFORM_CHIP_NAME)


export KDRV_MODULE_BUILD
ifeq ($(KDRV_MODULE_BUILD),YES)
CONFIG_LG_BUILTIN_KDRIVER := m
endif
ifeq ($(CONFIG_LG_BUILTIN_KDRIVER),)
CONFIG_LG_BUILTIN_KDRIVER := m
endif


chip_name := $(PLATFORM_CHIP_NAME)_$(PLATFORM_CHIP_REV)


obj-$(CONFIG_LG_BUILTIN_KDRIVER) := fwload.o
fwload-objs += driver.o
fwload-objs += mcu_vdec.o
fwload-objs += mcu_cip.o

#mcus-h13_B0 += mcu_vdec_h13b0.o
#mcus-h13_B0 += mcu_vdec_h13b0.o
#mcus-m14_A0 += mcu_vdec_m14.o
#fwload-objs += $(mcus-$(chip_name))

