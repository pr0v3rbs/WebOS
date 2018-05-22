#
# Makefile for USB Core files and filesystem
#

# mstar_porting_plat >>>
EXTRA_CFLAGS      += -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/usb
# mstar_porting_plat <<<
usbcore-y := usb.o hub.o hcd.o urb.o message.o driver.o
usbcore-y += config.o file.o buffer.o sysfs.o endpoint.o
usbcore-y += devio.o notify.o generic.o quirks.o devices.o
# mstar_porting_plat >>>
#usbcore-y += port.o
usbcore-y += port.o bc-mstar.o mstar-lib.o
# mstar_porting_plat <<<

usbcore-$(CONFIG_PCI)		+= hcd-pci.o
usbcore-$(CONFIG_ACPI)		+= usb-acpi.o

obj-$(CONFIG_USB)		+= usbcore.o
