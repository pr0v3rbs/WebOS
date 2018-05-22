obj-y := clock.o
obj-$(CONFIG_PLAT_MSTAR_LOCAL_TIMERS) += localtimer.o
obj-$(CONFIG_PLAT_MSTAR_CLCD) += clcd.o
obj-$(CONFIG_PLAT_MSTAR_LEDS) += leds.o
ifeq ($(KERNELVERSION),3.8.13)
obj-$(CONFIG_PLAT_MSTAR_SCHED_CLOCK) += 3.8.13-sched-clock.o
else
obj-$(CONFIG_PLAT_MSTAR_SCHED_CLOCK) += sched-clock.o
endif
obj-$(CONFIG_SMP) += headsmp.o platsmp.o
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o

obj-y += memory.o

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
EXTRA_CFLAGS += -Werror
EXTRA_CFLAGS += -Iarch/arm/mach-$(CONFIG_MSTAR_CHIP_NAME)/
EXTRA_CFLAGS += -Iarch/arm/mach-$(CONFIG_MSTAR_CHIP_NAME)/board
