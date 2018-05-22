cmd_arch/arm/kernel/sigreturn_codes.o := arm-lg115x-linux-gnueabi-gcc -Wp,-MD,arch/arm/kernel/.sigreturn_codes.o.d  -nostdinc -isystem /home/rla5072/Desktop/webos_driver/toolchain/arm-lg115x-linux-gnueabi-4.8-2014.11-x86_64/bin/../lib/gcc/arm-lg115x-linux-gnueabi/4.8.3/include -I./arch/arm/include -Iarch/arm/include/generated/uapi -Iarch/arm/include/generated  -Iinclude -I./arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/include -Iarch/arm/include/mstar -Iarch/arm/mach-"mustang" -Iarch/arm/mach-"mustang"/board -Iarch/arm/mach-mustang/include -Iarch/arm/plat-mstar/include -Iarch/arm/plat-versatile/include   -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float -Wa,-gdwarf-2  -DCC_HAVE_ASM_GOTO            -c -o arch/arm/kernel/sigreturn_codes.o arch/arm/kernel/sigreturn_codes.S

source_arch/arm/kernel/sigreturn_codes.o := arch/arm/kernel/sigreturn_codes.S

deps_arch/arm/kernel/sigreturn_codes.o := \
    $(wildcard include/config/cpu/thumbonly.h) \
  arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/cpu/v7m.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  arch/arm/include/asm/unistd.h \
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/oabi/compat.h) \
  arch/arm/include/uapi/asm/unistd.h \

arch/arm/kernel/sigreturn_codes.o: $(deps_arch/arm/kernel/sigreturn_codes.o)

$(deps_arch/arm/kernel/sigreturn_codes.o):
