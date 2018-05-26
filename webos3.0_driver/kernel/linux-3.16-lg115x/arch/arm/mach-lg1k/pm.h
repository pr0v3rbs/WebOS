/*
 * linux/arch/arm/mach-lg1k/pm.h
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/timex.h>
#include <asm/processor.h>

/*
 * This C function and macro are moved and excuted in SRAM.
 * Don't use DRAM resident variables and absolute branch.
 * If This is modified, please check if assemble code is
 * excutable in SRAM.
 */

#define _lg1210_pm_ddr_refresh_enter_internel(ddrc_base) \
	__raw_writel(0x00000000, ddrc_base + 0x0c0); /* pid 0 read disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x0e0); /* pid 1 write disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x100); /* pid 2 read disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x120); /* pid 3 write disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x140); /* pid 4 read disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x160); /* pid 5 write disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x180); /* pid 6 read disable */ \
	__raw_writel(0x00000000, ddrc_base + 0x1a0); /* pid 7 write disable */ \
	__raw_writel(0x8b000000, ddrc_base + 0x000); /* RAS/RFC max off, ZQC off */ \
	__raw_writel(0x00050001, ddrc_base + 0x0b0); /* Dynamic ODT to '0' */ \
	__raw_writel(0x000d0001, ddrc_base + 0x0b0); /* All Bank pre-charge */ \
	__raw_writel(0x000b0001, ddrc_base + 0x0b0); /* Do manual Refresh */ \
	__raw_writel(0x000c0001, ddrc_base + 0x0b0); /* Self-refresh Entry */ \
	isb(); \
	while (__raw_readl(ddrc_base + 0x0b0) != 0x000c0000) \
		cpu_relax(); \
	__raw_writel(0x00000001, ddrc_base + 0x0b4); /* set turn_off_adrctrl 1 */ \
	__raw_writel(0x40bb00b1, ddrc_base + 0x320); /* turn off receiver */ \
	isb(); \

inline void __noreturn _lg1210_pm_ddr_refresh_enter(void)
{
	#define DDRC_M0_BASE 0xc8804000
	#define DDRC_M1_BASE 0xc8404000
	#define DDRC_M2_BASE 0xc8504000
	#define RET_DIS_GPIO_BASE 0xfd410000 /* gpio_num(11)/8 */
	#define RET_DIS_GPIO_CTRL 0x8 /* 1 << (gpio_num(11) % 8)) */
	#define DELAY_LOOP 0x100000 /*approx. 5ms */

	register void __iomem *ioaddr asm("r1");
	register unsigned int gpio_ctrl asm("r2");
	register unsigned int delay asm("r3");

	ioaddr = (void __iomem *)DDRC_M0_BASE;
	_lg1210_pm_ddr_refresh_enter_internel(ioaddr);

	ioaddr = (void __iomem *)DDRC_M1_BASE;
	_lg1210_pm_ddr_refresh_enter_internel(ioaddr);

	ioaddr = (void __iomem *)DDRC_M2_BASE;
	_lg1210_pm_ddr_refresh_enter_internel(ioaddr);

	/* wait for short while */
	delay = DELAY_LOOP;
	while(--delay) cpu_relax();

	isb();

	ioaddr = (void __iomem *)RET_DIS_GPIO_BASE;
	gpio_ctrl = RET_DIS_GPIO_CTRL;

	/* set RETENTION_DISABLE to 0 */
	/* set pin direction to output */
	__raw_writel(__raw_readl(ioaddr + 0x400) | gpio_ctrl, ioaddr + 0x400);
	/* set pin to low */
	__raw_writel(0x0, ioaddr + (gpio_ctrl << 2));

	/* Don't return now */
	for (;;);
} __attribute__((aligned(8)))

inline void _end_lg1210_pm_ddr_refresh_enter(void){}
