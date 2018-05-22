cmd_arch/arm/vfp/built-in.o :=  arm-lg115x-linux-gnueabi-ld -EL  --no-warn-mismatch   -r -o arch/arm/vfp/built-in.o arch/arm/vfp/vfp.o ; scripts/mod/modpost arch/arm/vfp/built-in.o
