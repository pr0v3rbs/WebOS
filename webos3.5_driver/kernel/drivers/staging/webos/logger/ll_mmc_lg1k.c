/*MMC  raw driver on lg1k for the MMC oops/panic logger*/

#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/delay.h>
#include "ll_mmc.h"

#define DRV_NAME "ll-mmc-lg1k"

#define EMMC_PACKET_SIZE			16384
#define EMMC_BLOCK_SIZE				512

#define EMMC_WAIT_CMD				0
#define EMMC_WAIT_DATA				1

#define EMMC_BYTE_MODE				0
#define EMMC_SECTOR_MODE			1

#define EMMC_CHECK_ERROR			0x8000
#define EMMC_CMD_END				0x1
#define EMMC_DATA_END				0x2

#define EMMC_BLOCK_COUNT			(0x4)
#define EMMC_ARG				(0x8)
#define EMMC_COMMAND				(0xC)
#define EMMC_RESPONSE_0				(0x10)
#define EMMC_PIO_BUF				(0x20)
#define EMMC_STATE				(0x24)
#define EMMC_CTRL				(0x28)
#define EMMC_CLOCK_CTRL				(0x2C)
#define EMMC_INT_STA				(0x30)
#define EMMC_INT_ENABLE				(0x34)
#define EMMC_INT_SIGNAL				(0x38)
#define EMMC_CAPABLITY				(0x40)

#define EMMC_CMD0_IDLE_STATE			0
#define EMMC_CMD1_SEND_OP_COND			1
#define EMMC_CMD2_ALL_SEND_CID			2
#define EMMC_CMD3_SET_RELATIVE_ADDR		3
#define EMMC_CMD4_SET_DSR			4
#define EMMC_CMD5_SLEEP_AWAKE			5
#define EMMC_CMD6_SWITCH			6
#define EMMC_CMD7_SELDESEL_CARD			7
#define EMMC_CMD8_SEND_EXT_CSD			8
#define EMMC_CMD9_SEND_CSD			9
#define EMMC_CMD10_SEND_CID			10
#define EMMC_CMD13_SEND_STATUS			13

#define EMMC_CMD16_SET_BLOCK_LENGTH		16
#define EMMC_CMD17_READ_SINGLE			17
#define EMMC_CMD18_READ_MULTIE			18

#define EMMC_CMD23_SET_BLOCK_COUNT		23
#define EMMC_CMD24_WR_SINGLE			24
#define EMMC_CMD25_WR_MULTIE			25

#define VDD_180					0x04000000
#define VDD_300					0x02000000
#define VDD_330					0x01000000

#define EMMC_VOL_165_195			0x00000080
#define EMMC_VOL_29_30				0x00020000
#define EMMC_VOL_30_31				0x00040000
#define EMMC_VOL_32_33				0x00100000
#define EMMC_VOL_33_34				0x00200000

static void __iomem *emmc_base;
static u32 emmc_address_mode = EMMC_SECTOR_MODE;

static s32 lg1k_emmc_check_flag(u32 data_wait)
{
	u32 temp;
	u32 outcount;

	outcount = 1000;
	while (1) {
		temp = readl(emmc_base + EMMC_INT_STA);
		if (temp & EMMC_CHECK_ERROR) {
			writel(temp, emmc_base + EMMC_INT_STA);
			return -1;
		} else if ((temp & EMMC_CMD_END) == EMMC_CMD_END) {
			writel(EMMC_CMD_END, emmc_base + EMMC_INT_STA);
			break;
		}

		udelay(10);

		if (outcount == 0)
			return -1;

		outcount--;
	}

	if (data_wait == EMMC_WAIT_CMD) {
		writel(temp, emmc_base + EMMC_INT_STA);
		return 0;
	}

	outcount = 1000;
	while (1) {
		temp = readl(emmc_base + EMMC_INT_STA);
		if (temp & EMMC_CHECK_ERROR) {
			pr_err("EMMC DATA ERR : 0x%X\n", temp);
			writel(temp, emmc_base + EMMC_INT_STA);
			return -1;
		} else if (temp & EMMC_DATA_END) {
			writel(temp, emmc_base + EMMC_INT_STA);
			break;
		}

		udelay(1000);

		if (outcount == 0) {
			pr_err("EMMC DATA TIMEOUT : 0x%X\n", temp);
			return -1;
		}

		outcount--;
	}

	return 0;
}

static s32 lg1k_emmc_ready_delay(void)
{
	u32 temp;
	u32 outcount = 1000;

	while (1) {
		temp = readl(emmc_base + EMMC_STATE) & 0x7FFFF;
		if (temp == 0x70000)
			return 0;

		udelay(1000);

		if (outcount == 0)
			return -1;

		outcount--;
	}
}

static s32 lg1k_emmc_send_cmd(u8 cmd, u8 flag, u8 oprmode)
{
	s32 err = -1;

	err = lg1k_emmc_ready_delay();
	if (err < 0)
		return err;

	writel((cmd << (16 + 8)) + (flag << (16)) + oprmode,
		emmc_base + EMMC_COMMAND);

	return err;
}

static unsigned int lg1k_get_ocr(u32 ocr_response)
{
	u32 capab;
	u32 ocr;
	u32 ocr_avail;
	u32 bit;

	capab = readl(emmc_base + EMMC_CAPABLITY);

	ocr_avail = 0;
	if (capab & VDD_330)
		ocr_avail |= (EMMC_VOL_33_34 | EMMC_VOL_32_33);

	if (capab & VDD_300)
		ocr_avail |= (EMMC_VOL_29_30 | EMMC_VOL_30_31);

	if (capab & VDD_180)
		ocr_avail |= EMMC_VOL_165_195;

	ocr_response &= ocr_avail;
	bit = ffs(ocr_response);

	if (bit) {
		bit -= 1;
		ocr = ocr_response & (3 << bit);
	} else {
		ocr = 0;
	}
	return ocr;
}

static s32 lg1k_emmc_set_8bit_bus(void)
{
	s32 err = -1;
	u32 arg;
	u32 response;
	u32 i;

	arg = (3 << 24) + (0xB7 << 16) + (2 << 8);
	writel(arg, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD6_SWITCH, 0x1B, 0x00);
	if (err < 0)
		return err;

	err = lg1k_emmc_check_flag(EMMC_WAIT_DATA);
	if (err < 0)
		return err;

	for (i = 0; i < 1000; i++) {
		writel(0x10000, emmc_base + EMMC_ARG);
		err = lg1k_emmc_send_cmd(EMMC_CMD13_SEND_STATUS, 0x1A, 0x00);
		if (err < 0)
			return err;

		err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
		if (err < 0)
			return err;

		response = readl(emmc_base + EMMC_RESPONSE_0);
		if (response == 0x900)
			break;
	}

	return err;
}

static s32 lg1k_emmc_init(void)
{
	u32 temp;
	u32 reg;
	u32 ocr;
	s32 err = -1;
	u32 response;
	u32 outcount = 0;

	if (!emmc_base)
		return -1;

	/* HOST CONTROLLER RESET */
	writel(0x01000000, emmc_base + EMMC_CLOCK_CTRL);
	outcount = 10;
	while (readl(emmc_base + EMMC_CLOCK_CTRL) & 0x01000000) {
		udelay(1000);
		if (outcount == 0)
			goto fail;

		outcount--;
	}

	writel(0x1FF0EFB, emmc_base + EMMC_INT_ENABLE);
	writel(0x0, emmc_base + EMMC_INT_SIGNAL);

	/* Data Transfer Mode 4bit, SD Power On, 1.8V */
	reg = 0xB02;
	writel(reg, emmc_base + EMMC_CTRL);
	outcount = 100;
	while (1) {
		temp =	readl(emmc_base + EMMC_CTRL);
		if (temp == reg)
			break;

		writel(reg, emmc_base + EMMC_CTRL);

		udelay(1000);
		if (outcount == 0)
			goto fail;

		outcount--;
	}

	reg = 0x0047;
	writel(reg, emmc_base + EMMC_CLOCK_CTRL);
	outcount = 100;
	while (1) {
		temp =	readl(emmc_base + EMMC_CLOCK_CTRL);
		if (temp == reg)
			break;

		writel(0x0047, emmc_base + EMMC_CLOCK_CTRL);

		udelay(1000);
		if (outcount == 0)
			goto fail;

		outcount--;
	}

	udelay(2000);

	/* GOTO IDLE */
	writel(0x0, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD0_IDLE_STATE, 0x00, 0x00);
	if (err < 0)
		goto fail;

	err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
	if (err < 0)
		goto fail;

	udelay(1000);

	writel(0x40000000, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD1_SEND_OP_COND, 0x02, 0x00);
	if (err < 0)
		return -1;

	err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
	if (err < 0)
		return -1;

	response = readl(emmc_base + EMMC_RESPONSE_0);

	udelay(1000);

	ocr = lg1k_get_ocr(response) | 0x40000000;

	outcount = 1000;
	while (1) {
		writel(ocr, emmc_base + EMMC_ARG);
		err = lg1k_emmc_send_cmd(EMMC_CMD1_SEND_OP_COND, 0x02, 0x00);
		if (err < 0)
			goto fail;

		err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
		if (err < 0)
			goto fail;

		response = readl(emmc_base + EMMC_RESPONSE_0);
		if (response & 0x80000000)
			break;

		udelay(1000);

		if (outcount == 0)
			goto fail;

		outcount--;
	}

	if (response & 0x40000000)
		emmc_address_mode = EMMC_SECTOR_MODE;
	else
		emmc_address_mode = EMMC_BYTE_MODE;

	/* GET CID REGISTER */
	writel(0x0, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD2_ALL_SEND_CID, 0x09, 0x00);
	if (err < 0)
		goto fail;

	err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
	if (err < 0)
		goto fail;

	/* SET RELATIVE ADDRESS ( set 1 ) */
	writel(0x10000, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD3_SET_RELATIVE_ADDR, 0x1A, 0x00);
	if (err < 0)
		goto fail;

	err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
	if (err < 0)
		goto fail;

	/* GET CSD */
	writel(0x10000, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD9_SEND_CSD, 0x09, 0x00);
	if (err < 0)
		goto fail;

	err = lg1k_emmc_check_flag(EMMC_WAIT_CMD);
	if (err < 0)
		goto fail;

	/* SET RCA SELECT CARD and OTHER CARD IS DESELECT */
	writel(0x10000, emmc_base + EMMC_ARG);
	err = lg1k_emmc_send_cmd(EMMC_CMD7_SELDESEL_CARD, 0x1B, 0x00);
	if (err < 0)
		goto fail;

	err = lg1k_emmc_check_flag(EMMC_WAIT_DATA);
	if (err < 0)
		goto fail;

	writel(0xE0407, emmc_base + EMMC_CLOCK_CTRL);

	lg1k_emmc_set_8bit_bus();

	writel(0xB20, emmc_base + EMMC_CTRL);

	return 0;

fail:
	return -1;
}

static s32 lg1k_emmc_prepare_data(u8 *buffer, u64 address, u32 size)
{
	u32 emmc_address;
	u16 block_count;

	if (!IS_ALIGNED(size, EMMC_BLOCK_SIZE))
		return -1;

	if (emmc_address_mode == EMMC_SECTOR_MODE)
		emmc_address = (unsigned int)(address / EMMC_BLOCK_SIZE);
	else
		emmc_address = (unsigned int)address;

	block_count = (size / EMMC_BLOCK_SIZE);

	writel((block_count << 16) + EMMC_BLOCK_SIZE,
		emmc_base + EMMC_BLOCK_COUNT);

	writel(emmc_address, emmc_base + EMMC_ARG);

	return 0;
}

static s32 lg1k_emmc_write_atomic(u8 *buffer, u64 address, u32 size)
{
	s32 err = -1;
	u32 i, j;
	u32 outcount;
	u32 *data;

	err = lg1k_emmc_prepare_data(buffer, address, size);
	if (err < 0)
		return -1;

	err = lg1k_emmc_send_cmd(EMMC_CMD25_WR_MULTIE, 0x3A, 0x26);
	if (err < 0)
		return -2;

	outcount = 10000;
	while (1) {
		if (readl(emmc_base + EMMC_INT_STA) & 0x10)
			break;

		udelay(10);
		outcount--;

		if (outcount == 0)
			return -4;
	}

	data = (u32 *)buffer;

	for (i = 0; i < (size / EMMC_BLOCK_SIZE); i++) {
		outcount = 500000;
		while ((readl(emmc_base + EMMC_STATE) & 0x400) != 0x400) {
			udelay(1);
			outcount--;

			if (outcount == 0)
				return -4;
		}

		for (j = 0; j < (EMMC_BLOCK_SIZE / 4); j++)
			writel(data[(i * EMMC_BLOCK_SIZE / 4) + j],
				emmc_base + EMMC_PIO_BUF);
	}

	err = lg1k_emmc_check_flag(EMMC_WAIT_DATA);
	if (err < 0)
		err = -3;

	return err;
}

static s32 lg1k_emmc_write(u8 *buffer, u64 address, u32 size)
{
	s32 err = -1;
	u32 mod_size;
	u32 mod_count;
	u32 i = 0;

	if (!IS_ALIGNED(address, EMMC_BLOCK_SIZE))
		return -1;

	if (!IS_ALIGNED(size, EMMC_BLOCK_SIZE))
		return -1;

	mod_size = size % EMMC_PACKET_SIZE;
	mod_count = size / EMMC_PACKET_SIZE;

	for (i = 0; i < mod_count; i++) {
		err = lg1k_emmc_write_atomic(buffer + (EMMC_PACKET_SIZE * i),
			address + (EMMC_PACKET_SIZE * i), EMMC_PACKET_SIZE);
		if (err < 0)
			return err;
	}

	if (mod_size) {
		err = lg1k_emmc_write_atomic(buffer + (EMMC_PACKET_SIZE * i),
			address + (EMMC_PACKET_SIZE * i), mod_size);
		if (err < 0)
			return err;
	}

	return err;
}

static s32 lg1k_emmc_read_atomic(u8 *buffer, u64 address, u32 size)
{
	s32 err = -1;
	u32 i;
	u32 *data;
	u32 outcount;

	err = lg1k_emmc_prepare_data(buffer, address, size);
	if (err < 0)
		return -1;

	err = lg1k_emmc_send_cmd(EMMC_CMD18_READ_MULTIE, 0x3A, 0x36);
	if (err < 0)
		return -2;

	outcount = 10000;
	while (1) {
		if (readl(emmc_base + EMMC_INT_STA) & 0x20) {
			writel(0x20, emmc_base + EMMC_INT_STA);
			break;
		}

		udelay(10);
		outcount--;

		if (outcount == 0)
			return -4;
	}

	data = (u32 *)buffer;

	for (i = 0; i < (size / 4); i++)
		data[i] = readl(emmc_base + EMMC_PIO_BUF);

	err = lg1k_emmc_check_flag(EMMC_WAIT_DATA);
	if (err < 0)
		err = -3;

	return err;
}

static s32 lg1k_emmc_read(u8 *buffer, u64 address, u32 size)
{
	s32 err = -1;
	u32 mod_size;
	u32 mod_count;
	u32 i = 0;

	if (!IS_ALIGNED(address, EMMC_BLOCK_SIZE))
		return -1;

	if (!IS_ALIGNED(size, EMMC_BLOCK_SIZE))
		return -1;

	mod_size = size % EMMC_PACKET_SIZE;
	mod_count = size / EMMC_PACKET_SIZE;

	for (i = 0; i < mod_count; i++) {
		err = lg1k_emmc_read_atomic(buffer + (EMMC_PACKET_SIZE * i),
			address + (EMMC_PACKET_SIZE * i), EMMC_PACKET_SIZE);
		if (err < 0)
			return -1;
	}

	if (mod_size) {
		err = lg1k_emmc_read_atomic(buffer + (EMMC_PACKET_SIZE * i),
			address + (EMMC_PACKET_SIZE * i), mod_size);
		if (err < 0)
			return err;
	}

	return err;
}

struct ll_mmc_ops lg1k_ll_mmc_ops = {
	.name			= DRV_NAME,
	.ll_mmc_init		= lg1k_emmc_init,
	.ll_mmc_write		= lg1k_emmc_write,
	.ll_mmc_read		= lg1k_emmc_read,
};

static int __init lg1k_ll_mmc_init(void)
{
	struct device_node *np;
	register_ll_mmc_ops(&lg1k_ll_mmc_ops);

	np =  of_find_node_by_name(NULL, "mmc");
	if (!np)
		return -1;

	emmc_base = of_iomap(np, 0);
	of_node_put(np);

	return 0;
}
early_initcall(lg1k_ll_mmc_init);
