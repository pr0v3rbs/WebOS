BUILDDIR = build
TOOLCHAIN=`pwd`/toolchain/arm-lg115x-linux-gnueabi-4.8-2014.11-x86_64

all: $(BUILDDIR) $(BUILDDIR)/fbdriver.ko

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/fbdriver.ko: $(BUILDDIR)/fbdriver.o $(BUILDDIR)/fbdriver.mod.o
	arm-lg115x-linux-gnueabi-ld -EL -r  -T ./scripts/module-common.lds --build-id  -o $(BUILDDIR)/fbdriver.ko $(BUILDDIR)/fbdriver.o $(BUILDDIR)/fbdriver.mod.o

$(BUILDDIR)/fbdriver.o: fbdriver.c
	arm-lg115x-linux-gnueabi-gcc -Wp,-MD,$(BUILDDIR)/.fbdriver.o.d  -nostdinc -isystem $(TOOLCHAIN)/lib/gcc/arm-lg115x-linux-gnueabi/4.8.3/include -I./arch/arm/include -Iarch/arm/include/generated/uapi -Iarch/arm/include/generated  -Iinclude -I./arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/include -Iarch/arm/include/mstar -Iarch/arm/mach-"mustang" -Iarch/arm/mach-"mustang"/board -Iarch/arm/mach-mustang/include -Iarch/arm/plat-mstar/include -Iarch/arm/plat-versatile/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-peephole2 -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -fno-ipa-sra -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -g -pg -fno-inline-functions-called-once -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(fbdriver)"  -D"KBUILD_MODNAME=KBUILD_STR(fbdriver)" -c -o $(BUILDDIR)/fbdriver.o fbdriver.c

$(BUILDDIR)/fbdriver.mod.o: fbdriver.mod.c
	arm-lg115x-linux-gnueabi-gcc -Wp,-MD,$(BUILDDIR)/.fbdriver.mod.o.d  -nostdinc -isystem $(TOOLCHAIN)/lib/gcc/arm-lg115x-linux-gnueabi/4.8.3/include -I./arch/arm/include -Iarch/arm/include/generated/uapi -Iarch/arm/include/generated  -Iinclude -I./arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/include -Iarch/arm/include/mstar -Iarch/arm/mach-"mustang" -Iarch/arm/mach-"mustang"/board -Iarch/arm/mach-mustang/include -Iarch/arm/plat-mstar/include -Iarch/arm/plat-versatile/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-peephole2 -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -fno-ipa-sra -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -g -pg -fno-inline-functions-called-once -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO  -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(fbdriver.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(fbdriver)" -DMODULE  -c -o $(BUILDDIR)/fbdriver.mod.o fbdriver.mod.c

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
