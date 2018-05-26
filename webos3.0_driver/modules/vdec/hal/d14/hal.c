
#include <linux/kernel.h>
#include <linux/module.h>

#include "vdec_base.h"
#include "hma_alloc.h"

unsigned long vdec_alloc (int size, int align, const char *name)
{
	unsigned long r;

	r = hma_alloc_user ("vdec0", size, align, name);
	if (!r)
		r = hma_alloc_user ("vdec1", size, align, name);
	if (!r)
		return 0;

	r -= VDEC_BUS_OFFSET;
	if (r >= 0x10000000)
		r = r-0x10000000+0x80000000;

	return r;
}
EXPORT_SYMBOL(vdec_alloc);


void vdec_free (unsigned long addr)
{
	unsigned long a = addr;

	if (a < 0x80000000)
	{
		a += VDEC_BUS_OFFSET;
		hma_free ("vdec0", a);
	}
	else
	{
		a = a-0x80000000+0x10000000;
		a += VDEC_BUS_OFFSET;
		hma_free ("vdec1", a);
	}
}
EXPORT_SYMBOL(vdec_free);

void *vdec_remap (unsigned long addr, int size)
{
	unsigned long a = addr;
	int s = size;

	if (a >= 0x80000000)
		a = a-0x80000000+0x10000000;
	a += VDEC_BUS_OFFSET;

	return (void*)a;
}
EXPORT_SYMBOL(vdec_remap);
