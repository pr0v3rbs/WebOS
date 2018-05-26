obj-m := fbdriver.o

KDIR := kernel/linux-3.16-lg115x
EXTRA_CFLAGS := -I$(PWD)/drivers/kdriver/core/fbdev -I$(PWD)/drivers/kdriver/core/base -I$(PWD)/drivers/kdriver/include -I$(PWD)/drivers/kdriver/chip

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=`pwd`/toolchain/arm-lg115x-linux-gnueabi-4.8-2014.11-x86_64/bin/arm-lg115x-linux-gnueabi- -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* .*.cmd
