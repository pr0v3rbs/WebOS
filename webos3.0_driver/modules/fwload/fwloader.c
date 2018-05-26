
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "fwload.h"

void oops (const char *f, ...)
{
	va_list a;

	va_start (a, f);
	vprintf (f, a);
	va_end (a);

	exit (1);
}

#define reset(h)	do{ \
if (ioctl (dev, FWLOAD_IOCTL_RESET, h) < 0) \
	oops ("FWLOAD_IOCTL_RESET failed, %s\n", strerror (errno)); \
} while(0)
#define halt(h)		{ \
if (ioctl (dev, FWLOAD_IOCTL_HALT, h) < 0) \
	oops ("FWLOAD_IOCTL_HALT failed, %s\n", strerror (errno)); \
} while(0)

int main (int argc, char **argv)
{
	struct fwload_image image;
	int dev;
	int rom = -1, ram = -1;
	int opt;
	char *tmp;
	int rom_memsize = 0;
	int ram_memsize = 0;
	char *rom_image = NULL;
	char *ram_image = NULL;
	int do_reset = 0;
	int do_halt = 0;
	char *dev_node = "/dev/fwload";
	const unsigned char *rom_data = NULL;
	const unsigned char *ram_data = NULL;
	unsigned int rom_size = 0;
	unsigned int ram_size = 0;

	while ((opt = getopt (argc, argv, "-n:M:m:rhH")) != -1)
	{
		switch (opt)
		{
		default:
		case '?':
			printf (
					"firmware loader.\n"
					"  $ ./fwloader <arguments> ... <rom image> [<ram image>]\n"
					"arguments:\n"
					"  -n <devnode>     : specifi device node name\n"
					"  -M <rom_memsize> : rom memory size, 16M for 16 mega bytes\n"
					"  -m <ram_memsize> : ram memory size\n"
					"  -r               : do reset\n"
					"  -h               : hold halt\n"
					"  -H               : release halt\n"
			       );
			return 1;

		case 'n':
			dev_node = optarg;
			break;

		case 'M':
			rom_memsize = strtoul (optarg, &tmp, 0);
			if (*tmp == 'm' || *tmp == 'M')
				rom_memsize *= 1024*1024;
			else if (*tmp == 'k' || *tmp == 'K')
				rom_memsize *= 1024;

			break;

		case 'm':
			ram_memsize = strtoul (optarg, &tmp, 0);
			if (*tmp == 'm' || *tmp == 'M')
				ram_memsize *= 1024*1024;
			else if (*tmp == 'k' || *tmp == 'K')
				ram_memsize *= 1024;

			break;

		case 'r':
			do_reset = 1;
			break;

		case 'h':
			do_halt = 1;
			break;

		case 'H':
			do_halt = 2;
			break;

		case 1:
			if (!rom_image)
				rom_image = optarg;
			else
				ram_image = optarg;
			break;
		}
	};

	/* open device */
	printf ("device node     : %s\n", dev_node);
	dev = open (dev_node, O_RDWR);
	if (dev < 0)
		oops ("device open failed, %s, %s\n",
				dev_node, strerror (errno));

	if (do_reset)
	{
		printf ("do reset\n");

		reset (1);
		usleep (100000);
		reset (0);

		return 0;
	}

	if (do_halt)
	{
		printf ("%s halt\n", do_halt == 1?"hold":"release");

		halt (do_halt == 1?1:0);

		return 0;
	}

	printf ("rom memory size : %d\n", rom_memsize);
	printf ("ram memory size : %d\n", ram_memsize);
	printf ("rom image file  : %s\n", rom_image);
	printf ("ram image file  : %s\n", ram_image);

	if (!rom_image)
		oops ("no rom image found\n");

	rom = open (rom_image, O_RDONLY);
	if (rom < 0)
		oops ("rom_image, %s, open failed. %s\n",
				rom_image, strerror (errno));
	rom_size = lseek (rom, 0, SEEK_END);
	lseek (rom, 0, SEEK_SET);
	printf ("rom image size  : %d\n", rom_size);

	rom_data = mmap (NULL, rom_size, PROT_READ, MAP_SHARED, rom, 0);
	if (rom_data == MAP_FAILED)
		oops ("mmap failed, %s\n", strerror (errno));

	if (ram_image)
	{
		ram = open (ram_image, O_RDONLY);
		if (ram < 0)
			oops ("ram_image, %s, open failed. %s\n",
					ram_image, strerror (errno));
		ram_size = lseek (ram, 0, SEEK_END);
		lseek (ram, 0, SEEK_SET);
		printf ("ram image size  : %d\n", ram_size);

		ram_data = mmap (NULL, ram_size, PROT_READ, MAP_SHARED, ram, 0);
		if (ram_data == MAP_FAILED)
			oops ("mmap failed, %s\n", strerror (errno));
	}

	halt (1);
	reset (1);

	/* load rom image */
	printf ("rom image size  : %d\n", rom_size);
	image.type = fwload_image_type_rom;
	image.data = rom_data;
	image.size = rom_size;
	image.total_size = rom_memsize;
	if (ioctl (dev, FWLOAD_IOCTL_LOAD, &image) < 0)
		oops ("FWLOAD_IOCTL_LOAD failed, %s\n", strerror (errno));

	/* load ram image */
	if (ram_data || ram_memsize)
	{
		printf ("ram image size  : %d\n", ram_size);
		image.type = fwload_image_type_ram;
		if (ram_data)
		{
			image.data = ram_data;
			image.size = ram_size;
		}
		else
		{
			image.data = NULL;
			image.size = 0;
		}
		image.total_size = ram_memsize;
		if (ioctl (dev, FWLOAD_IOCTL_LOAD, &image) < 0)
			oops ("FWLOAD_IOCTL_LOAD failed, %s\n", strerror (errno));
	}

	reset (0);
	halt (0);

	return 0;
}

