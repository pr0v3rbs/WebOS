cmd_arch/arm/boot/uImage := /bin/bash ./scripts/mkuboot.sh -A arm -O linux -C none  -T kernel -a 0x20208000 -e 0x20208000 -n 'Linux-4.4.3' -d arch/arm/boot/zImage arch/arm/boot/uImage
