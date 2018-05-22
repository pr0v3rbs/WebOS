/*MMC  raw driver on mstar for the MMC oops/panic logger*/

#include <linux/delay.h>
#include <linux/io.h>
#include <mach/io.h>

#include "ll_mmc.h"

#define DRV_NAME            "ll-mmc-mstar"

#define EMMC_BLOCK_SIZE     512

#define EMMC_EMERG_DEBUG    0

static void __iomem *fcie_mie_func_ctl = (void *)IO_ADDRESS(0x1f22261C);
#if EMMC_EMERG_DEBUG
static void __iomem *fcie_ddr_mode = (void *)IO_ADDRESS(0x1f22263C);
#endif

extern u32 eMMC_CMD18_MIU(u32, u8*, u16);
extern u32 eMMC_CMD25_MIU(u32, u8*, u16);

static s32 mstar_emmc_init(void)
{
    #if EMMC_EMERG_DEBUG
    if( readl(fcie_ddr_mode) == 0x1100 )
        printk("HS400 mode\n");
    #endif

    return 0;
}

static s32 mstar_emmc_write(u8 *buffer, u64 address, u32 size)
{
    s32 err = 0;
    u32 u32_err = 0;

    #if EMMC_EMERG_DEBUG
    printk("%s:\n", __FUNCTION__);
    printk("buffer=%p\n", buffer);
    printk("address=%llx\n", address);
    printk("size=%x\n", size);
    #endif

	if (!IS_ALIGNED(address, EMMC_BLOCK_SIZE))
		return -1;

	if (!IS_ALIGNED(size, EMMC_BLOCK_SIZE))
		return -1;

    while( readl(fcie_mie_func_ctl) & (1<<12) )
        udelay(1);

    u32_err = eMMC_CMD25_MIU((u32)(address>>9), buffer, (u16)(size>>9));
    if( u32_err )
    {
        printk("eMMC_CMD25_MIU returns %X\n", u32_err);
        err = -1;
    }
    #if EMMC_EMERG_DEBUG
    else
    {
        printk("mstar_emmc_write ok\n");
    }
    #endif

	return err;
}

static s32 mstar_emmc_read(u8 *buffer, u64 address, u32 size)
{
    s32 err = 0;
    u32 u32_err = 0;

    #if EMMC_EMERG_DEBUG
    printk("%s:\n", __FUNCTION__);
    printk("buffer=%p\n", buffer);
    printk("address=%llx\n", address);
    printk("size=%x\n", size);
    #endif

	if (!IS_ALIGNED(address, EMMC_BLOCK_SIZE))
		return -1;

	if (!IS_ALIGNED(size, EMMC_BLOCK_SIZE))
		return -1;

    while( readl(fcie_mie_func_ctl) & (1<<12) )
        udelay(1);

    u32_err = eMMC_CMD18_MIU((u32)(address>>9), buffer, (u16)(size>>9));
    if( u32_err )
    {
        printk("eMMC_CMD18_MIU returns %X\n", u32_err);
        err = -1;
    }
    #if EMMC_EMERG_DEBUG
    else
        printk("mstar_emmc_read ok\n");
    #endif

	return 0;
}

struct ll_mmc_ops mstar_ll_mmc_ops = {
	.name           = DRV_NAME,
	.ll_mmc_init    = mstar_emmc_init,
	.ll_mmc_write   = mstar_emmc_write,
	.ll_mmc_read    = mstar_emmc_read,
};

static int __init mstar_ll_mmc_init(void)
{
	register_ll_mmc_ops(&mstar_ll_mmc_ops);
	return 0;
}
early_initcall(mstar_ll_mmc_init);
