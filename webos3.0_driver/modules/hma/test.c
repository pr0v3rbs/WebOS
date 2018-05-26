#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhma.h"

int main (int argc, char **argv)
{
	struct
	{
		int size, align;
		unsigned long paddr;
		void *vaddr;
	} info[16];
#define info_num	(sizeof(info)/sizeof(info[0]))
	char in[256];

	memset (info, 0, sizeof(info));
	while (1)
	{
		int a, e;

		e = -1;
		printf ("memlist:\n");
		for (a=0; a<info_num; a++)
		{
			if (info[a].paddr != 0)
				printf ("%2d: paddr: %08lx, size: %08x, align: %08x\n", a,
						info[a].paddr, info[a].size,
						info[a].align);
			else
				e = a;
		}

		printf ( "a <size> <align> - allocate\n");
		printf ( "f <nth> - free nth memory chunk\n");

		if (fgets (in, sizeof(in), stdin) == NULL)
			break;

		if (in[0] == 'a')
		{
			int size, align;
			char *tmp;

			if (e == -1)
			{
				printf ("no empty slot\n");
				continue;
			}

			size = strtol (in+1,&tmp, 16);
			align = strtol (tmp+1, NULL, 16);

			printf ("allocate, size: %08x, align: %08x\n",
					size, align);

			//info[e].paddr = libhma_alloc ("test", size, align, &info[e].vaddr);
			info[e].paddr = libhma_alloc (NULL, size, align, &info[e].vaddr);
			info[e].size = size;
			info[e].align = align;
		}
		else if (in[0] == 'f')
		{
			int slot;

			slot = atoi (in+1);
			printf ("free %dth slot\n", slot);
			if (slot < info_num)
			{
				libhma_free (info[slot].paddr);
				info[slot].paddr = 0;
			}
			else
				printf ("wrong slot %d\n", slot);
		}
	}

	return 0;
}
