/**
 * @file hdmi_rx_ctrl_hdcp.c
 * @brief HDMI RX controller HDCP module
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */
#include "hdmi_rx_ctrl_hdcp.h"
#include <linux/kernel.h>

/*
 * Miscellaneous
 */
/** HDCP key write tries */
#define HDCP_KEY_WR_TRIES		(5)
/** Maximum downstream cascade */
#define MAX_CASCADE				(7)
/** Maximum downstream devices */
#define MAX_DEVICES				(127)
/** KSV FIFO size, HW setting */
#define KSV_FIFO_SIZE			(127)

/** File name */
static const char *file = "hdmi_rx_ctrl_hdcp";

/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#define MSK(m, n)				(((1 << (m)) - 1) << (n))
/**
 * Bit mask
 * @param n shift
 */
#define BIT(n)					MSK(1, (n))

#define		HDCP_STABLE_TIMEOUT			(30)

/*
 * HDCP registers
 */
/** Register address: control */
#define RA_HDCP_CTRL			(0x0C0UL)
/** HDCP key decryption */
#define		KEY_DECRYPT_ENABLE		BIT(1)
/** HDCP activation */
#define		HDCP_ENABLE				BIT(0)
#define		HDCP_CTRL_ENABLE		BIT(24)
/** Register address: configuration */
#define RA_HDCP_SETTINGS		(0x0C4UL)
/** Register address: key description seed */
#define RA_HDCP_SEED			(0x0C8UL)
/** Register address: receiver key selection */
#define RA_HDCP_BKSV1			(0x0CCUL)
/** Register address: receiver key selection */
#define RA_HDCP_BKSV0 			(0x0D0UL)
/** Register address: key index */
#define RA_HDCP_KIDX			(0x0D4UL)
/** Register address: encrypted key */
#define RA_HDCP_KEY1			(0x0D8UL)
/** Register address: encrypted key */
#define RA_HDCP_KEY0			(0x0DCUL)
/** Register address: debug */
#define RA_HDCP_DBG				(0x0E0UL)
/** Register address: transmitter key selection vector */
#define RA_HDCP_AKSV1			(0x0E4UL)
/** Register address: transmitter key selection vector */
#define RA_HDCP_AKSV0 			(0x0E8UL)
/** Register address: session random number */
#define RA_HDCP_AN1				(0x0ECUL)
/** Register address: session random number */
#define RA_HDCP_AN0 			(0x0F0UL)
/** Register address: EESS, WOO */
#define RA_HDCP_EESS_WOO		(0x0F4UL)
/** Register address: key set writing status */
#define RA_HDCP_STS				(0x0FCUL)
/** HDCP key set writing status */
#define		HDCP_KEY_WR_OK_STS		BIT(0)
/** Register address: repeater KSV list control */
#define	RA_HDCP_RPT_CTRL		(0x600UL)
/** KSV list key set writing status */
#define		KSV_HOLD				BIT(6)
/** KSV list waiting status */
#define		WAITING_KSV				BIT(5)
/** Repeater capability */
#define		REPEATER				BIT(3)
/** KSV list ready */
#define		KSVLIST_READY			BIT(2)
/** Register address: repeater status */
#define	RA_HDCP_RPT_BSTATUS		(0x604UL)
/** Topology error indicator */
#define		MAX_CASCADE_EXCEEDED	BIT(11)
/** Repeater cascade depth */
#define		DEPTH					MSK(3, 8)
/** Topology error indicator */
#define		MAX_DEVS_EXCEEDED		BIT(7)
/** Attached downstream device count */
#define		DEVICE_COUNT			MSK(7, 0)
/** Register address: repeater KSV FIFO control */
#define	RA_HDCP_RPT_KSVFIFOCTRL	(0x608UL)
/** Register address: repeater KSV FIFO */
#define	RA_HDCP_RPT_KSVFIFO1	(0x60CUL)
/** Register address: repeater KSV FIFO */
#define	RA_HDCP_RPT_KSVFIFO0	(0x610UL)

/* ***************************************************************************
 * Data handling
 * ***************************************************************************/
/**
 * Find first (least significant) bit set
 * @param[in] data word to search
 * @return bit position or 32 if none is set
 */
static unsigned first_bit_set(uint32_t data)
{
   unsigned n = 32;

   if (data != 0) {
	   for (n = 0; (data & 1) == 0; n++) {
		   data >>= 1;
	   }
   }
   return n;
}

/**
 * Get bit field
 * @param[in] data raw data
 * @param[in] mask bit field mask
 * @return bit field value
 */
static uint32_t get(uint32_t data, uint32_t mask)
{
	return (data & mask) >> first_bit_set(mask);
}

/**
 * Set bit field
 * @param[in] data raw data
 * @param[in] mask bit field mask
 * @param[in] value new value
 * @return new raw data
 */
static uint32_t set(uint32_t data, uint32_t mask, uint32_t value)
{
	return ((value << first_bit_set(mask)) & mask) | (data & ~mask);
}

/**
 * Read bit field from device
 * @param[in,out] ctx context information
 * @param[in] addr bus address
 * @param[in] mask bitfield mask
 * @return zero if error or bit field value
 */
static uint32_t io_read(struct hdmi_rx_ctrl *ctx, uint16_t addr, uint32_t mask)
{
	return (ctx == 0)? 0 : get(ctx->bsp_read(addr), mask);
}

/**
 * Write bit field into device
 * @param[in,out] ctx context information
 * @param[in] addr bus address
 * @param[in] mask bit field mask
 * @param[in] value new value
 * @return error code
 */
static int io_write(struct hdmi_rx_ctrl *ctx, uint16_t addr, uint32_t mask, uint32_t value)
{
	int error = 0;
	unsigned long flags = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	error |= ctx->spin_lock_irq_save(&ctx->spin, &flags);
	if (error == 0) {
		ctx->bsp_write(addr, set(ctx->bsp_read(addr), mask, value));
		error |= ctx->spin_unlock_irq_restore(&ctx->spin, flags);
	}
	return error;
}

/* ***************************************************************************
 * Interface and configuration
 * ***************************************************************************/
int hdmi_rx_ctrl_hdcp_reset(struct hdmi_rx_ctrl *ctx, const struct hdmi_rx_ctrl_hdcp *hdcp)
{
	int error = 0;
	unsigned i = 0;
	
	if (ctx == 0 || hdcp == 0) {
		return -EINVAL;
	}
	if (ctx->status == 0) {		/* is closed? */
		return -EPERM;
	}

	ctx->log_info("Port[%d] : Reset HDCP1.4", ctx->device_id);	

	error |= io_write(ctx, RA_HDCP_CTRL, HDCP_CTRL_ENABLE, 0);
	
	for (i = 0; i < HDCP_STABLE_TIMEOUT; i++) { /* time for TMDS to stabilise */
		;
	}
	
	error |= io_write(ctx, RA_HDCP_CTRL, HDCP_CTRL_ENABLE, 1);

	return error;
}

/**
 * Configure HDCP
 * @param[in,out] ctx context information
 * @param[in] hdcp HDCP configuration
 * @return error code
 */
int hdmi_rx_ctrl_hdcp_config(struct hdmi_rx_ctrl *ctx, const struct hdmi_rx_ctrl_hdcp *hdcp)
{
	int error = 0;
	unsigned i = 0;
	unsigned k = 0;
	if (ctx == 0 || hdcp == 0) {
		return -EINVAL;
	}
	if (ctx->status == 0) {		/* is closed? */
		return -EPERM;
	}

	ctx->log_info("Port[%d] : Initiate HDCP1.4", ctx->device_id);	
	
	//error |= io_write(ctx, RA_HDCP_CTRL, HDCP_ENABLE, 0);
	
	//error |= io_write(ctx, RA_HDCP_CTRL, HDCP_ENABLE, 1);
	error |= io_write(ctx, RA_HDCP_CTRL, KEY_DECRYPT_ENABLE, 0); // Disable Encryption for LGE
	ctx->bsp_write(RA_HDCP_SEED, hdcp->seed);
	for (i = 0; i < HDCP_KEYS_SIZE; i += 2) {
		for (k = 0; k < HDCP_KEY_WR_TRIES; k++) {
			if (io_read(ctx, RA_HDCP_STS, HDCP_KEY_WR_OK_STS) != 0) {
			break;
			}
		}
		if (k < HDCP_KEY_WR_TRIES) {
			ctx->bsp_write(RA_HDCP_KEY1, hdcp->keys[i + 1]);
			ctx->bsp_write(RA_HDCP_KEY0, hdcp->keys[i + 0]);
		} else {
			error = -EAGAIN;
			break;
		}
	}
	ctx->bsp_write(RA_HDCP_BKSV1, hdcp->bksv[1]);
	ctx->bsp_write(RA_HDCP_BKSV0, hdcp->bksv[0]);
	error |= io_write(ctx, RA_HDCP_RPT_CTRL, REPEATER, hdcp->repeat? 1 : 0);
	ctx->bsp_write(RA_HDCP_RPT_BSTATUS, 0);	/* nothing attached downstream */
	if (error == 0) {
		error |= io_write(ctx, RA_HDCP_CTRL, HDCP_ENABLE, 1);
	}
	
	return error;
}

/**
 * Configure KSV list
 * @param[in,out] ctx context information
 * @param[in] list KSV list, 0: high order, 1: low order
 * @param[in] count device count, if too big MAX_DEVS_EXCEEDED will be set
 * @param[in] depth cascade depth, if too big MAX_CASCADE_EXCEEDED will be set
 * @return error code
 */
int hdmi_rx_ctrl_hdcp_ksv_list(struct hdmi_rx_ctrl *ctx, const uint32_t *list, unsigned count, unsigned depth)
{
	int error = 0;
	unsigned i = 0;
	unsigned k = 0;

	if (ctx == 0 || list == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is configured? */
		return -EPERM;
	}
	if (io_read(ctx, RA_HDCP_RPT_CTRL, REPEATER) == 0) {
		error = -EPERM;			/* not configured in repeater mode */
	}
	if (io_read(ctx, RA_HDCP_RPT_CTRL, WAITING_KSV) == 0) {
		error = -EAGAIN;		/* not waiting for KSV list writing */
	}
	if (count <= MAX_DEVICES && count <= KSV_FIFO_SIZE) {
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, MAX_DEVS_EXCEEDED, 0);
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, DEVICE_COUNT, count);
	} else {
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, MAX_DEVS_EXCEEDED, 1);
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, DEVICE_COUNT, 0);
		count = 0;		/* don't write KSV list */
	}
	if (depth <= MAX_CASCADE) {
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, MAX_CASCADE_EXCEEDED, 0);
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, DEPTH, depth);
	} else {
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, MAX_CASCADE_EXCEEDED, 1);
		error |= io_write(ctx, RA_HDCP_RPT_BSTATUS, DEPTH, 0);
		count = 0;		/* don't write KSV list */
	}
	for (i = 0; i < count; i++) {
		for (k = 0; k < HDCP_KEY_WR_TRIES; k++) {
			if (io_read(ctx, RA_HDCP_RPT_CTRL, KSV_HOLD) != 0) {
				break;
			}
		}
		if (k < HDCP_KEY_WR_TRIES) {
			ctx->bsp_write(RA_HDCP_RPT_KSVFIFOCTRL, i);
			ctx->bsp_write(RA_HDCP_RPT_KSVFIFO1, list[i * 2 + 0]);
			ctx->bsp_write(RA_HDCP_RPT_KSVFIFO0, list[i * 2 + 1]);
		} else {
			error = -EAGAIN;
			break;
		}
	}
	if (error == 0) {
		error = io_write(ctx, RA_HDCP_RPT_CTRL, KSVLIST_READY, 1);
	}
	return error;
}

/**
 * Debug HDCP
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_hdcp_debug(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is configured? */
		return -EPERM;
	}
	ctx->log_info("%s", file);
	ctx->log_info("  DBG: %08X", ctx->bsp_read(RA_HDCP_DBG));
	return error;
}
