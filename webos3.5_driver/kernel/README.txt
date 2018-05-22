cd ./git/kernel
PATH=/home/rla5072/Desktop/webos_driver/toolchain/arm-lg115x-linux-gnueabi-4.8-2014.11-x86_64/bin:$PATH
make ARCH=arm m2r_defconfig
make ARCH=arm modules -j8
make ARCH=arm MSTAR_UIMAGE=1 uImage -j8
make ARCH=arm pak
