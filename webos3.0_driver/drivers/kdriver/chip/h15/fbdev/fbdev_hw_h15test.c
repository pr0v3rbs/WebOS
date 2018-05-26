/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author		raxis.lim
 *  @version	1.0
 *  @date		2014-03-01
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef	FBDEV_H15_TEST_BLIT_VERIFY		/* verify gfx cmd buffer with simple blit */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_drv.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"
#include "fbdev_hw_h15.h"
#include "fbdev_reg_h15.h"
#include <linux/kthread.h>
#include <linux/version.h>

#ifdef	FBDEV_H15_CPB_BW_TEST

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#undef FBDEV_DEBUG
//#define FBDEV_DEBUG		printk

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	CCO_CTRL_REG		0xc8024000
#define CCO_CTRL_SIZE		0x100

#define CCO_AFBC0_REG		0xc8024700
#define CCO_AFBC1_REG		0xc8024800
#define CC0_AFBC_SIZE		0x100

#define GFX_REG				0xc8441000
#define GFX_SIZE			0x1000

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	gfx_cmd_buf;
	UINT32	src0_fb_phys;
	UINT32	src1_fb_phys;
	UINT32	dst_cpb_fb_phys;
	UINT32	dst_cpb_hdr_phys;
}
FBDEV_H15_TEST_CPB_PRAM_T;

typedef struct
{
	UINT32						cpb_mode;			/* 0: none, 1: FBDC */
	FBDEV_H15_TEST_CPB_PRAM_T	p[3];				/* 0: cpb0, 1: cpb1, 2:blit */
	int							draw_cpb_idx;
	int							view_cpb_idx;

#define	FBDEV_H15TEST_CPB_STATUS_HW_INITIALIZED	0x001
#define	FBDEV_H15TEST_CPB_STATUS_HW_NEW_IMAGE	0x002
	UINT32						cpb_hw_status;

	UINT32						vsync_wait;			/* for vsync noti */
	struct completion   		vsync_completion;	/* for vsync noti */
}
FBDEV_H15_TEST_CPB_CTX_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern  volatile OSD_H15_REG_T*     g_osd_h15_reg;
extern  OSD_H15_REG_T*              g_osd_h15_reg_cached;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static FBDEV_H15_TEST_CPB_CTX_T	g_cpb_test_ctx =
{
	/* @note the below phys_addr is fixed for H15 verification */
	.p[0] = { 0x25000000 /*0x1A020000*/, 0x20000000, 0x1E000000, /*cpb_fb*/0x18000000, /*cpb_hdr*/0x1A000000 },	/* CPB test #0 */
	.p[1] = { 0x25100000 /*0x1D020000*/, 0x20000000, 0x1E000000, /*cpb_fb*/0x1B000000, /*cpb_hdr*/0x1D000000 },	/* CPB test #1 */

	.p[2] = { 0x25200000 /*0x1D020000*/, 0x1b000000, 0x1b000000, /*cpb_fb*/0x20000000, /*cpb_hdr*/0x00000000 },	/* Simple Blit */

	.draw_cpb_idx 	= 0,
	.view_cpb_idx 	= 0,
	.cpb_mode 		= 0,

	.cpb_hw_status	= 0x0,
	.vsync_wait		= 0,
};

static UINT32 gfx_cpb_cmd_buf0[] =
{
0x08010e00,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08014af8,0x00000000,
0x08014af9,0x00000000,
0x08014afa,0x00000000,
0x08014afb,0x00000000,
0x08014afc,0x00000000,
0x08014afd,0x00000000,
0x08014afe,0x00000000,
0x08014aff,0x00000000,
0x08014ac8,0x02000000,
0x08014ac8,0x32000000,
0x08014ac9,0x02000000,
0x08014ac9,0x32000000,
0x08014aca,0x02000000,
0x08014aca,0x32000000,
0x08014acb,0x02000000,
0x08014acb,0x32000000,
0x08014acc,0x02000000,
0x08014acc,0x32000000,
0x08014acd,0x02000000,
0x08014acd,0x32000000,
0x08014ace,0x02000000,
0x08014ace,0x32000000,
0x08014acf,0x02000000,
0x08014acf,0x32000000,
0x08014ac8,0x32000000,
0x08014ac8,0x32000000,
0x08014ac9,0x32000000,
0x08014ac9,0x32000000,
0x08014aca,0x32000000,
0x08014aca,0x32000000,
0x08014acb,0x32000000,
0x08014acb,0x32000000,
0x08014acc,0x32000000,
0x08014acc,0x32000000,
0x08014acd,0x32000000,
0x08014acd,0x32000000,
0x08014ace,0x32000000,
0x08014ace,0x32000000,
0x08014acf,0x32000000,
0x08014acf,0x32000000,
0x08014ac8,0x32000100,
0x08014ac8,0x32001100,
0x08014ac9,0x32000100,
0x08014ac9,0x32001100,
0x08014aca,0x32000100,
0x08014aca,0x32001100,
0x08014acb,0x32000100,
0x08014acb,0x32001100,
0x08014acc,0x32000100,
0x08014acc,0x32001100,
0x08014acd,0x32000100,
0x08014acd,0x32001100,
0x08014ace,0x32000100,
0x08014ace,0x32001100,
0x08014acf,0x32000100,
0x08014acf,0x32001100,
0x08014b00,0x80000000,
0x08014b08,0x32000000,
0x08014b01,0x80000000,
0x08014b09,0x32000000,
0x08014b02,0x80000000,
0x08014b0a,0x32000000,
0x08014b03,0x80000000,
0x08014b0b,0x32000000,
0x08014b04,0x80000000,
0x08014b0c,0x32000000,
0x08014b05,0x80000000,
0x08014b0d,0x32000000,
0x08014b06,0x80000000,
0x08014b0e,0x32000000,
0x08014b07,0x80000000,
0x08014b0f,0x32000000,
0x08014b10,0x00000000,
0x08014b11,0x00000000,
0x08014b12,0x00000000,
0x08014b13,0x00000000,
0x08014b14,0x00000000,
0x08014b15,0x00000000,
0x08014b16,0x00000000,
0x08014b17,0x00000000,
0x08014b18,0x00000000,
0x08014ac0,0x00000000,
0x08014ac1,0x00000001,
0x08014ac2,0x00000001,
0x08014ac3,0x00000001,
0x08014ac4,0x00000001,
0x08014ac5,0x00000001,
0x08014ac6,0x00000001,
0x08014ac7,0x00000001,
0x0801048d,0x00000000,
0x08010483,0x00000000,
0x0801048a,0x18000000,
0x0801048b,0x00003c00,
0x0801048a,0x18000000,
0x0801048b,0x00003c00,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x080104af,0x00000000,
0x08014af8,0x00000000,
0x08014af9,0x00000000,
0x08014afa,0x00000000,
0x08014afb,0x00000000,
0x08014afc,0x00000000,
0x08014afd,0x00000000,
0x08014afe,0x00000000,
0x08014aff,0x00000000,
0x080104ad,0x00000870,
0x0801048c,0x00000f00,
0x08010498,0x00000000,
0x08010499,0x08700f00,
0x0801048d,0x01000006,
0x08010480,0x0bde0900,
0x08010481,0x00004000,
0x08010480,0x0bde0900,
0x08010481,0x00004000,
0x08010480,0x0bde0900,
0x080104af,0x00000000,
0x08010483,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x08010483,0x00000000,
0x08010483,0x00000000,
0x080104b5,0x00000000,
0x080104ae,0x00000870,
0x08010482,0x00001000,
0x080104c1,0x0cec0900,
0x08010483,0x06000006,
0x08010483,0x06000006,
0x08010486,0x00000000,
0x080104b7,0x00000000,
0x08010484,0x00000000,
0x08010485,0x08701000,
0x08010497,0x0030cccc,
0x08014ab8,0x0030cccc,
0x08014ab9,0x0030cccc,
0x08014aba,0x0030cccc,
0x08014abb,0x0030cccc,
0x08014abc,0x0030cccc,
0x08014abd,0x0030cccc,
0x08014abe,0x0030cccc,
0x08014abf,0x0030cccc,
0x08014a80,0x20000000,
0x08014a88,0x00004000,
0x08014af8,0x00000000,
0x08014a98,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014af0,0x00000870,
0x08014a90,0x00001000,
0x08014a98,0x00000000,
0x08014b18,0x00000000,
0x08014a80,0x20000000,
0x08014b38,0x0cec0900,
0x08014a98,0x06000000,
0x08014a98,0x06000000,
0x08014ab0,0x00000000,
0x08014b28,0x00000000,
0x08014a81,0x1e000000,
0x08014a89,0x00004000,
0x08014af9,0x00000000,
0x08014a99,0x00000000,
0x08014b31,0x00000000,
0x08014b31,0x00000000,
0x08014b31,0x00000000,
0x08014af1,0x00000870,
0x08014a91,0x00001000,
0x08014a99,0x00000000,
0x08014b19,0x00000000,
0x08014a81,0x1e000000,
0x08014b39,0x0f080a00,
0x08014a99,0x06000000,
0x08014a99,0x06000000,
0x08014ab1,0x00000000,
0x08014b29,0x00000000,
0x08014a82,0x10160b00,
0x08014a8a,0x00004000,
0x08014afa,0x00000000,
0x08014a9a,0x00000000,
0x08014b32,0x00000000,
0x08014b32,0x00000000,
0x08014b32,0x00000000,
0x08014af2,0x00000870,
0x08014a92,0x00001000,
0x08014a9a,0x00000000,
0x08014b1a,0x00000000,
0x08014a82,0x10160b00,
0x08014b3a,0x11240b00,
0x08014a9a,0x06000000,
0x08014a9a,0x06000000,
0x08014ab2,0x00000000,
0x08014b2a,0x00000000,
0x08014a83,0x12320c00,
0x08014a8b,0x00004000,
0x08014afb,0x00000000,
0x08014a9b,0x00000000,
0x08014b33,0x00000000,
0x08014b33,0x00000000,
0x08014b33,0x00000000,
0x08014af3,0x00000870,
0x08014a93,0x00001000,
0x08014a9b,0x00000000,
0x08014b1b,0x00000000,
0x08014a83,0x12320c00,
0x08014b3b,0x13400c00,
0x08014a9b,0x06000000,
0x08014a9b,0x06000000,
0x08014ab3,0x00000000,
0x08014b2b,0x00000000,
0x08014a84,0x144e0d00,
0x08014a8c,0x00004000,
0x08014afc,0x00000000,
0x08014a9c,0x00000000,
0x08014b34,0x00000000,
0x08014b34,0x00000000,
0x08014b34,0x00000000,
0x08014af4,0x00000870,
0x08014a94,0x00001000,
0x08014a9c,0x00000000,
0x08014b1c,0x00000000,
0x08014a84,0x144e0d00,
0x08014b3c,0x155c0d00,
0x08014a9c,0x06000000,
0x08014a9c,0x06000000,
0x08014ab4,0x00000000,
0x08014b2c,0x00000000,
0x08014a85,0x166a0e00,
0x08014a8d,0x00004000,
0x08014afd,0x00000000,
0x08014a9d,0x00000000,
0x08014b35,0x00000000,
0x08014b35,0x00000000,
0x08014b35,0x00000000,
0x08014af5,0x00000870,
0x08014a95,0x00001000,
0x08014a9d,0x00000000,
0x08014b1d,0x00000000,
0x08014a85,0x166a0e00,
0x08014b3d,0x17780e00,
0x08014a9d,0x06000000,
0x08014a9d,0x06000000,
0x08014ab5,0x00000000,
0x08014b2d,0x00000000,
0x08014a86,0x18860f00,
0x08014a8e,0x00004000,
0x08014afe,0x00000000,
0x08014a9e,0x00000000,
0x08014b36,0x00000000,
0x08014b36,0x00000000,
0x08014b36,0x00000000,
0x08014af6,0x00000870,
0x08014a96,0x00001000,
0x08014a9e,0x00000000,
0x08014b1e,0x00000000,
0x08014a86,0x18860f00,
0x08014b3e,0x19940f00,
0x08014a9e,0x06000000,
0x08014a9e,0x06000000,
0x08014ab6,0x00000000,
0x08014b2e,0x00000000,
0x08014a87,0x1aa21000,
0x08014a8f,0x00004000,
0x08014aff,0x00000000,
0x08014a9f,0x00000000,
0x08014b37,0x00000000,
0x08014b37,0x00000000,
0x08014b37,0x00000000,
0x08014af7,0x00000870,
0x08014a97,0x00001000,
0x08014a9f,0x00000000,
0x08014b1f,0x00000000,
0x08014a87,0x1aa21000,
0x08014b3f,0x1bb01000,
0x08014a9f,0x06000000,
0x08014a9f,0x06000000,
0x08014ab7,0x00000000,
0x08014b2f,0x00000000,
0x08014aa0,0x00000000,
0x08014aa8,0x08700f00,
0x08014aa1,0x00000000,
0x08014aa9,0x08700f00,
0x08014aa2,0x00000000,
0x08014aaa,0x08700f00,
0x08014aa3,0x00000000,
0x08014aab,0x08700f00,
0x08014aa4,0x00000000,
0x08014aac,0x08700f00,
0x08014aa5,0x00000000,
0x08014aad,0x08700f00,
0x08014aa6,0x00000000,
0x08014aae,0x08700f00,
0x08014aa7,0x00000000,
0x08014aaf,0x08700f00,
0x080104c2,0x00000401,
0x080104cb,0x00010000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104cb,0x00050000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104e1,0x1a000000,
0x080104ca,0x00000000,
0x080104ca,0x00000000,
0x080104e0,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000020,
0x08010484,0x00000000,
0x08010485,0x08700f00,
0x0801048d,0x01000006,
0x0801048d,0x09000006,
0x0801048d,0x09000006,
0x080104c2,0x00000401,
0x0801048d,0x09008006,
0x20000100,0x00000000,
0x00000000,0x08700f00,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010594,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000021,
0x080104cb,0x00040000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104cb,0x00000000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x10000000,0x00000000
};

static UINT32 gfx_cpb_cmd_buf1[] =
{
0x08010e00,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08014af8,0x00000000,
0x08014af9,0x00000000,
0x08014afa,0x00000000,
0x08014afb,0x00000000,
0x08014afc,0x00000000,
0x08014afd,0x00000000,
0x08014afe,0x00000000,
0x08014aff,0x00000000,
0x08014ac8,0x02000000,
0x08014ac8,0x32000000,
0x08014ac9,0x02000000,
0x08014ac9,0x32000000,
0x08014aca,0x02000000,
0x08014aca,0x32000000,
0x08014acb,0x02000000,
0x08014acb,0x32000000,
0x08014acc,0x02000000,
0x08014acc,0x32000000,
0x08014acd,0x02000000,
0x08014acd,0x32000000,
0x08014ace,0x02000000,
0x08014ace,0x32000000,
0x08014acf,0x02000000,
0x08014acf,0x32000000,
0x08014ac8,0x32000000,
0x08014ac8,0x32000000,
0x08014ac9,0x32000000,
0x08014ac9,0x32000000,
0x08014aca,0x32000000,
0x08014aca,0x32000000,
0x08014acb,0x32000000,
0x08014acb,0x32000000,
0x08014acc,0x32000000,
0x08014acc,0x32000000,
0x08014acd,0x32000000,
0x08014acd,0x32000000,
0x08014ace,0x32000000,
0x08014ace,0x32000000,
0x08014acf,0x32000000,
0x08014acf,0x32000000,
0x08014ac8,0x32000100,
0x08014ac8,0x32001100,
0x08014ac9,0x32000100,
0x08014ac9,0x32001100,
0x08014aca,0x32000100,
0x08014aca,0x32001100,
0x08014acb,0x32000100,
0x08014acb,0x32001100,
0x08014acc,0x32000100,
0x08014acc,0x32001100,
0x08014acd,0x32000100,
0x08014acd,0x32001100,
0x08014ace,0x32000100,
0x08014ace,0x32001100,
0x08014acf,0x32000100,
0x08014acf,0x32001100,
0x08014b00,0x80000000,
0x08014b08,0x32000000,
0x08014b01,0x80000000,
0x08014b09,0x32000000,
0x08014b02,0x80000000,
0x08014b0a,0x32000000,
0x08014b03,0x80000000,
0x08014b0b,0x32000000,
0x08014b04,0x80000000,
0x08014b0c,0x32000000,
0x08014b05,0x80000000,
0x08014b0d,0x32000000,
0x08014b06,0x80000000,
0x08014b0e,0x32000000,
0x08014b07,0x80000000,
0x08014b0f,0x32000000,
0x08014b10,0x00000000,
0x08014b11,0x00000000,
0x08014b12,0x00000000,
0x08014b13,0x00000000,
0x08014b14,0x00000000,
0x08014b15,0x00000000,
0x08014b16,0x00000000,
0x08014b17,0x00000000,
0x08014b18,0x00000000,
0x08014ac0,0x00000000,
0x08014ac1,0x00000001,
0x08014ac2,0x00000001,
0x08014ac3,0x00000001,
0x08014ac4,0x00000001,
0x08014ac5,0x00000001,
0x08014ac6,0x00000001,
0x08014ac7,0x00000001,
0x0801048d,0x00000000,
0x08010483,0x00000000,
0x0801048a,0x1b000000,
0x0801048b,0x00003c00,
0x0801048a,0x1b000000,
0x0801048b,0x00003c00,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x080104af,0x00000000,
0x08014af8,0x00000000,
0x08014af9,0x00000000,
0x08014afa,0x00000000,
0x08014afb,0x00000000,
0x08014afc,0x00000000,
0x08014afd,0x00000000,
0x08014afe,0x00000000,
0x08014aff,0x00000000,
0x080104ad,0x00000870,
0x0801048c,0x00000f00,
0x08010498,0x00000000,
0x08010499,0x08700f00,
0x0801048d,0x01000006,
0x08010480,0x0bde0900,
0x08010481,0x00004000,
0x08010480,0x0bde0900,
0x08010481,0x00004000,
0x08010480,0x0bde0900,
0x080104af,0x00000000,
0x08010483,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x08010483,0x00000000,
0x08010483,0x00000000,
0x080104b5,0x00000000,
0x080104ae,0x00000870,
0x08010482,0x00001000,
0x080104c1,0x0cec0900,
0x08010483,0x06000006,
0x08010483,0x06000006,
0x08010486,0x00000000,
0x080104b7,0x00000000,
0x08010484,0x00000000,
0x08010485,0x08701000,
0x08010497,0x0030cccc,
0x08014ab8,0x0030cccc,
0x08014ab9,0x0030cccc,
0x08014aba,0x0030cccc,
0x08014abb,0x0030cccc,
0x08014abc,0x0030cccc,
0x08014abd,0x0030cccc,
0x08014abe,0x0030cccc,
0x08014abf,0x0030cccc,
0x08014a80,0x20000000,
0x08014a88,0x00004000,
0x08014af8,0x00000000,
0x08014a98,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014af0,0x00000870,
0x08014a90,0x00001000,
0x08014a98,0x00000000,
0x08014b18,0x00000000,
0x08014a80,0x20000000,
0x08014b38,0x0cec0900,
0x08014a98,0x06000000,
0x08014a98,0x06000000,
0x08014ab0,0x00000000,
0x08014b28,0x00000000,
0x08014a81,0x1e000000,
0x08014a89,0x00004000,
0x08014af9,0x00000000,
0x08014a99,0x00000000,
0x08014b31,0x00000000,
0x08014b31,0x00000000,
0x08014b31,0x00000000,
0x08014af1,0x00000870,
0x08014a91,0x00001000,
0x08014a99,0x00000000,
0x08014b19,0x00000000,
0x08014a81,0x1e000000,
0x08014b39,0x0f080a00,
0x08014a99,0x06000000,
0x08014a99,0x06000000,
0x08014ab1,0x00000000,
0x08014b29,0x00000000,
0x08014a82,0x10160b00,
0x08014a8a,0x00004000,
0x08014afa,0x00000000,
0x08014a9a,0x00000000,
0x08014b32,0x00000000,
0x08014b32,0x00000000,
0x08014b32,0x00000000,
0x08014af2,0x00000870,
0x08014a92,0x00001000,
0x08014a9a,0x00000000,
0x08014b1a,0x00000000,
0x08014a82,0x10160b00,
0x08014b3a,0x11240b00,
0x08014a9a,0x06000000,
0x08014a9a,0x06000000,
0x08014ab2,0x00000000,
0x08014b2a,0x00000000,
0x08014a83,0x12320c00,
0x08014a8b,0x00004000,
0x08014afb,0x00000000,
0x08014a9b,0x00000000,
0x08014b33,0x00000000,
0x08014b33,0x00000000,
0x08014b33,0x00000000,
0x08014af3,0x00000870,
0x08014a93,0x00001000,
0x08014a9b,0x00000000,
0x08014b1b,0x00000000,
0x08014a83,0x12320c00,
0x08014b3b,0x13400c00,
0x08014a9b,0x06000000,
0x08014a9b,0x06000000,
0x08014ab3,0x00000000,
0x08014b2b,0x00000000,
0x08014a84,0x144e0d00,
0x08014a8c,0x00004000,
0x08014afc,0x00000000,
0x08014a9c,0x00000000,
0x08014b34,0x00000000,
0x08014b34,0x00000000,
0x08014b34,0x00000000,
0x08014af4,0x00000870,
0x08014a94,0x00001000,
0x08014a9c,0x00000000,
0x08014b1c,0x00000000,
0x08014a84,0x144e0d00,
0x08014b3c,0x155c0d00,
0x08014a9c,0x06000000,
0x08014a9c,0x06000000,
0x08014ab4,0x00000000,
0x08014b2c,0x00000000,
0x08014a85,0x166a0e00,
0x08014a8d,0x00004000,
0x08014afd,0x00000000,
0x08014a9d,0x00000000,
0x08014b35,0x00000000,
0x08014b35,0x00000000,
0x08014b35,0x00000000,
0x08014af5,0x00000870,
0x08014a95,0x00001000,
0x08014a9d,0x00000000,
0x08014b1d,0x00000000,
0x08014a85,0x166a0e00,
0x08014b3d,0x17780e00,
0x08014a9d,0x06000000,
0x08014a9d,0x06000000,
0x08014ab5,0x00000000,
0x08014b2d,0x00000000,
0x08014a86,0x18860f00,
0x08014a8e,0x00004000,
0x08014afe,0x00000000,
0x08014a9e,0x00000000,
0x08014b36,0x00000000,
0x08014b36,0x00000000,
0x08014b36,0x00000000,
0x08014af6,0x00000870,
0x08014a96,0x00001000,
0x08014a9e,0x00000000,
0x08014b1e,0x00000000,
0x08014a86,0x18860f00,
0x08014b3e,0x19940f00,
0x08014a9e,0x06000000,
0x08014a9e,0x06000000,
0x08014ab6,0x00000000,
0x08014b2e,0x00000000,
0x08014a87,0x1aa21000,
0x08014a8f,0x00004000,
0x08014aff,0x00000000,
0x08014a9f,0x00000000,
0x08014b37,0x00000000,
0x08014b37,0x00000000,
0x08014b37,0x00000000,
0x08014af7,0x00000870,
0x08014a97,0x00001000,
0x08014a9f,0x00000000,
0x08014b1f,0x00000000,
0x08014a87,0x1aa21000,
0x08014b3f,0x1bb01000,
0x08014a9f,0x06000000,
0x08014a9f,0x06000000,
0x08014ab7,0x00000000,
0x08014b2f,0x00000000,
0x08014aa0,0x00000000,
0x08014aa8,0x08700f00,
0x08014aa1,0x00000000,
0x08014aa9,0x08700f00,
0x08014aa2,0x00000000,
0x08014aaa,0x08700f00,
0x08014aa3,0x00000000,
0x08014aab,0x08700f00,
0x08014aa4,0x00000000,
0x08014aac,0x08700f00,
0x08014aa5,0x00000000,
0x08014aad,0x08700f00,
0x08014aa6,0x00000000,
0x08014aae,0x08700f00,
0x08014aa7,0x00000000,
0x08014aaf,0x08700f00,
0x080104c2,0x00000401,
0x080104cb,0x00010000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104cb,0x00050000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104e1,0x1d000000,
0x080104ca,0x00000000,
0x080104ca,0x00000000,
0x080104e0,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000020,
0x08010484,0x00000000,
0x08010485,0x08700f00,
0x0801048d,0x01000006,
0x0801048d,0x09000006,
0x0801048d,0x09000006,
0x080104c2,0x00000401,
0x0801048d,0x09008006,
0x20000100,0x00000000,
0x00000000,0x08700f00,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010594,0x00000001,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000021,
0x080104cb,0x00040000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x080104cb,0x00000000,
0x08014b30,0x00000000,
0x08014b31,0x00000000,
0x08014b32,0x00000000,
0x08014b33,0x00000000,
0x08014b34,0x00000000,
0x08014b35,0x00000000,
0x08014b36,0x00000000,
0x08014b37,0x00000000,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x10000000,0x00000000
};

static UINT32 gfx_blit_cmd_buf0[] =
{
0x08010e00,0x00000001,
0x0801048d,0x00000000,
0x08010483,0x00000000,
0x0801048a,0x20000000,
0x0801048b,0x00003c00,
0x0801048a,0x20000000,
0x0801048b,0x00003c00,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x0801048d,0x00000006,
0x080104af,0x00000000,
0x08014af8,0x00000000,
0x08014af9,0x00000000,
0x08014afa,0x00000000,
0x08014afb,0x00000000,
0x08014afc,0x00000000,
0x08014afd,0x00000000,
0x08014afe,0x00000000,
0x08014aff,0x00000000,
0x080104ad,0x00000870,
0x0801048c,0x00000f00,
0x08010498,0x00000000,
0x08010499,0x08700f00,
0x08010480,0x020d8200,
0x08010481,0x00003c00,
0x08010480,0x020d8200,
0x08010481,0x00003c00,
0x08010480,0x020d8200,
0x080104af,0x00000000,
0x08010483,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x080104c0,0x00000000,
0x08010483,0x00000000,
0x08010483,0x00000000,
0x080104b5,0x00000000,
0x080104ae,0x00000870,
0x08010482,0x00000f00,
0x080104c1,0x030aa200,
0x08010483,0x06000006,
0x08010483,0x06000006,
0x08010486,0x00000000,
0x080104b7,0x00000000,
0x08010484,0x00000000,
0x08010485,0x08700f00,
0x08010497,0x0030cccc,
0x08014ab8,0x0030cccc,
0x08014a80,0x1b000000,
0x08014a88,0x00003c00,
0x08014af8,0x00000000,
0x08014a98,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014b30,0x00000000,
0x08014af0,0x00000870,
0x08014a90,0x00000f00,
0x08014a98,0x00000000,
0x08014b18,0x00000000,
0x08014a80,0x1b000000,
0x08014b38,0x030aa200,
0x08014a98,0x06000000,
0x08014a98,0x06000000,
0x08014ab0,0x00000000,
0x08014b28,0x00000000,
0x08014aa0,0x00000000,
0x08014aa8,0x08700f00,
0x080104c2,0x00000300,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000020,
0x08010484,0x00000000,
0x08010485,0x08700f00,
0x0801048d,0x00000006,
0x0801048d,0x08000006,
0x0801048d,0x08000006,
0x080104c2,0x00000300,
0x0801048d,0x08008006,
0x20000100,0x00000000,
0x00000000,0x08700f00,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e01,0x00000021,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x08010e03,0x00000008,
0x08010e02,0x00000701,
0x48000000,0x00000701,
0x10000000,0x00000000
};

static volatile void* g_fbdev_h15test_cco_reg	  = NULL;
static volatile void* g_fbdev_h15test_afbc_reg[2] = {NULL,NULL};
static volatile void* g_fbdev_h15test_gfx_reg	  = NULL;

static volatile void* g_fbdev_h15test_gfx_blit_cmd_buf0	= NULL;
static volatile void* g_fbdev_h15test_gfx_cpb_cmd_buf0	= NULL;
static volatile void* g_fbdev_h15test_gfx_cpb_cmd_buf1	= NULL;

struct task_struct*	g_fbdev_h15_cpb_test_thread	= NULL;
static DEFINE_SPINLOCK(g_fbdev_h15test_cpb_spinlock);

static int  _g_fbdev_h15_cpb_bw_test_mode_req  = 0x0;
module_param_named( fbdev_h15_cpb_bw_test_mode, _g_fbdev_h15_cpb_bw_test_mode_req, int, 0644 );

/*----------------------------------------------------------------------------------------
	Static Functions
----------------------------------------------------------------------------------------*/
static void		TEST_WrReg ( volatile void* reg_base, UINT32 offset, UINT32 val );
static UINT32	TEST_RdReg ( volatile void* reg_base, UINT32 offset );

static int		FBDEV_H15TEST_CPBTask(void* data);

static void		FBDEV_H15TEST_MakeCPB(int idx);
static void		FBDEV_H15TEST_WaitCPB(int idx);
static void 	FBDEV_H15TEST_WriteOSDHdr(int idx);

/*========================================================================================
    Implementation Group ( )
========================================================================================*/
BOOLEAN	FBDEV_H15TEST_IsCPBTestMode(void)
{
	return (g_cpb_test_ctx.cpb_mode>0)? TRUE:FALSE;
}

void	FBDEV_H15TEST_InitCPBTest (void)
{
	/* ioremap CCO & AFBC register & GFX cmd buf */
	g_fbdev_h15test_cco_reg	 	=(volatile void*)ioremap( CCO_CTRL_REG,  CCO_CTRL_SIZE );
	g_fbdev_h15test_afbc_reg[0] =(volatile void*)ioremap( CCO_AFBC0_REG, CC0_AFBC_SIZE );
	g_fbdev_h15test_afbc_reg[1] =(volatile void*)ioremap( CCO_AFBC1_REG, CC0_AFBC_SIZE );
	g_fbdev_h15test_gfx_reg		=(volatile void*)ioremap( GFX_REG,		 GFX_SIZE );

	g_fbdev_h15test_gfx_cpb_cmd_buf0	=(volatile void*)ioremap( g_cpb_test_ctx.p[0].gfx_cmd_buf, 0x2000); /* 8k */
	g_fbdev_h15test_gfx_cpb_cmd_buf1	=(volatile void*)ioremap( g_cpb_test_ctx.p[1].gfx_cmd_buf, 0x2000); /* 8k */
	g_fbdev_h15test_gfx_blit_cmd_buf0	=(volatile void*)ioremap( g_cpb_test_ctx.p[2].gfx_cmd_buf, 0x2000); /* 8k */

	/* write pre-defined GFX cmd data */
	memcpy((void*)g_fbdev_h15test_gfx_cpb_cmd_buf0, gfx_cpb_cmd_buf0, sizeof(gfx_cpb_cmd_buf0));
	memcpy((void*)g_fbdev_h15test_gfx_cpb_cmd_buf1, gfx_cpb_cmd_buf1, sizeof(gfx_cpb_cmd_buf1));
	memcpy((void*)g_fbdev_h15test_gfx_blit_cmd_buf0,gfx_blit_cmd_buf0, sizeof(gfx_blit_cmd_buf0));

	/* init vsync wait */
	init_completion(&g_cpb_test_ctx.vsync_completion);

	/* create rendering thread */
	g_fbdev_h15_cpb_test_thread = kthread_create( FBDEV_H15TEST_CPBTask, (void*)NULL, "fbdev_cpb_test");

	FBDEV_DEBUG("CPB render task created\n");
    if(g_fbdev_h15_cpb_test_thread)
    {
        wake_up_process(g_fbdev_h15_cpb_test_thread);
    }
}

/** CPB rendering task
 *
 */
static int	FBDEV_H15TEST_CPBTask(void* data)
{
	int	ret;
	int	cpb_idx;
	ULONG flags;

	FBDEV_DEBUG("starting CPB render task !!!\n");

	while(1)
	{
		/* check test status per 1 sec */
		if(_g_fbdev_h15_cpb_bw_test_mode_req==0)
		{
			OS_MsecSleep(1000); continue;
		}
#if 0
		/* TEST. if you want the single decoding test, enable this */
		if (g_cpb_test_ctx.draw_cpb_idx==1) { OS_MsecSleep(1000); continue; }
#endif
		g_cpb_test_ctx.cpb_mode = _g_fbdev_h15_cpb_bw_test_mode_req;

		/* get the curent draw_cpb_idx */
		spin_lock_irqsave(&g_fbdev_h15test_cpb_spinlock, flags);
#ifdef FBDEV_H15_TEST_BLIT_VERIFY
		g_cpb_test_ctx.draw_cpb_idx = 2;
#endif
		cpb_idx = g_cpb_test_ctx.draw_cpb_idx;
		spin_unlock_irqrestore(&g_fbdev_h15test_cpb_spinlock, flags);
		FBDEV_DEBUG("RENDER BEGIN : cpb_idx = %d\n", cpb_idx );

#if 1
		/* make CPB image */
		FBDEV_H15TEST_MakeCPB(cpb_idx);
		FBDEV_H15TEST_WaitCPB(cpb_idx);
#endif
		spin_lock_irqsave(&g_fbdev_h15test_cpb_spinlock, flags);

		g_cpb_test_ctx.view_cpb_idx	= cpb_idx;
		g_cpb_test_ctx.draw_cpb_idx = cpb_idx ^ 1;
#ifdef FBDEV_H15_TEST_BLIT_VERIFY
		g_cpb_test_ctx.draw_cpb_idx = 2;
#endif
		g_cpb_test_ctx.cpb_hw_status |= FBDEV_H15TEST_CPB_STATUS_HW_NEW_IMAGE;
		FBDEV_DEBUG("new view_cb_idx = %d, new draw_cpb_idx = %d\n", g_cpb_test_ctx.view_cpb_idx, g_cpb_test_ctx.draw_cpb_idx );

		if( g_cpb_test_ctx.vsync_wait == 0 )
		{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
			reinit_completion(&g_cpb_test_ctx.vsync_completion);
#else
			INIT_COMPLETION(g_cpb_test_ctx.vsync_completion);
#endif
		}
		g_cpb_test_ctx.vsync_wait++;

		FBDEV_DEBUG("RENDER END : cpb_idx = %d\n\n", cpb_idx );
		spin_unlock_irqrestore(&g_fbdev_h15test_cpb_spinlock, flags);

		ret = wait_for_completion_interruptible(&g_cpb_test_ctx.vsync_completion);
		FBDEV_CHECK_ERROR( ret!=RET_OK, /* nop */, "event wait error. ret=%d\n", ret );

//		OS_MsecSleep(5000);
//		FBDEV_H15CMN_SetOSDEnable(0, TRUE);
	}

	return 0;
}

void	FBDEV_H15TEST_ISRHandler(void)
{
	ULONG	flags;

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	if (g_cpb_test_ctx.cpb_mode==0x0) return;

	spin_lock_irqsave(&g_fbdev_h15test_cpb_spinlock, flags);

//	if (g_cpb_test_ctx.cpb_hw_status & FBDEV_H15TEST_CPB_STATUS_HW_NEW_IMAGE )
	if (g_cpb_test_ctx.vsync_wait>0)
	{
		int	cpb_idx = g_cpb_test_ctx.view_cpb_idx;
		FBDEV_DEBUG("ISR-%d\n", cpb_idx );
#ifdef FBDEV_H15_TEST_BLIT_VERIFY
		/* nop */
#else
		if(!(g_cpb_test_ctx.cpb_hw_status & FBDEV_H15TEST_CPB_STATUS_HW_INITIALIZED))
		{
			FBDEV_DEBUG("ISR - Init CPB HW..\n");

			/* change OSD ctrl */
			OSDX_H15_RdFL(osdx_ctrl[0], osd_ctrl_main);
			OSDX_H15_RdFL(osdx_ctrl[1], osd_ctrl_main);

			OSDX_H15_Wr01(osdx_ctrl[0], osd_ctrl_main,   osd_bitmap_only_mode_en,	0x1 );
			OSDX_H15_Wr01(osdx_ctrl[1], osd_ctrl_main,   osd_bitmap_only_mode_en,	0x1 );

			OSDX_H15_WrFL(osdx_ctrl[0], osd_ctrl_main);
			OSDX_H15_WrFL(osdx_ctrl[1], osd_ctrl_main);

			/* init CCO */
			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x60, 0x000FF100 );	// init FBDEV decoder. OSD0/1
			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x68, 0x000FF100 );	// init FBDEV decdoer. OSD0/1

			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x30, 0x00015000 );	// adjust disp_pulse delay (test)
			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x0c, 0xC8876420 );	// ??? HW default ?

			if ( hw_ctx->conn_ctrl[0].conn_type == LX_FBDEV_CONN_OSD_VIDEO_SEPARATE )
			{
				TEST_WrReg( g_fbdev_h15test_cco_reg, 0x38, 0x01110111 );	//  CTRL_AUTO_INIT_AFBC_0/1 (S.OSD)
			}
			else
			{
				TEST_WrReg( g_fbdev_h15test_cco_reg, 0x38, 0x00110011 );	//  CTRL_AUTO_INIT_AFBC_0/1 (B.OSD)
			}

			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x3c, 0x00010001 );	//  CTRL_AUTO_INIT_LENCON_0/1

			/* init AFBC_0 */
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x04, g_cpb_test_ctx.p[cpb_idx].dst_cpb_hdr_phys );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x08, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x0c, 0x0FFF0000 );

			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x10, 0x00000010 );	// img_width : 0x10 fixed when FBDC mode (not 0x100 manual error)

			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x14, 0x03BF00FF );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x18, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x1c, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x20, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x24, 0xFFFFFFFF );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x28, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x2c, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x30, 0x00117F54 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x34, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x38, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x3c, 0x00000000 );

			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xd0, 0xFF8080FF );		// if rgba_argb_flag = 1
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x00, 0x00000000 );		// 0 : reset

			/* init AFBC_1 */
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x04, g_cpb_test_ctx.p[cpb_idx].dst_cpb_hdr_phys );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x08, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x0c, 0x0FFF0000 );

			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x10, 0x00000010 );	// img_width : 0x10 fixed when FBDC mode (not 0x100 manual error)

			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x14, 0x03BF00FF );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x18, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x1c, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x20, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x24, 0xFFFFFFFF );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x28, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x2c, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x30, 0x00117F54 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x34, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x38, 0x00000000 );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x3c, 0x00000000 );

			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xd0, 0xFF8080FF );		// if rgba_argb_flag = 1
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x00, 0x00000000 );		// 0 : reset

			/* init LENCON_0 */
#if 1
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xe4, 0x003C0000 );		// LENCON+04	-- OSD0/1 = 0, OSD2/3 = 1
#else
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xe4, 0x003C0001 );		// LENCON+04	-- OSD0/1 = 0, OSD2/3 = 1
#endif
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xe8, 0x001D0000 );		// LENCON+08
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xec, 0x086F0000 );		// LENCON+0c
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0xe0, 0x00000000 );		// LENCON+00

			/* init LENCON_1 */
#if 1
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xe4, 0x003C0000 );		// LENCON+04	-- OSD0/1 = 0, OSD2/3 = 1
#else
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xe4, 0x003C0001 );		// LENCON+04	-- OSD0/1 = 0, OSD2/3 = 1
#endif
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xe8, 0x003B001E );		// LENCON+08
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xec, 0x086F0000 );		// LENCON+0c
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0xe0, 0x00000000 );		// LENCON+00


			/* re-initialize AUTO PIC INIT */
			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x08, 0x55555531 );		// reinit - OSD0/1 auto init
			TEST_WrReg( g_fbdev_h15test_cco_reg, 0x00, 0x00ff0000 );		// reinit - AUTO PIC INIT

			g_cpb_test_ctx.cpb_hw_status |= FBDEV_H15TEST_CPB_STATUS_HW_INITIALIZED;
		}
		else
		{
			/* write new base_addr */
			TEST_WrReg(g_fbdev_h15test_afbc_reg[0], 0x04, g_cpb_test_ctx.p[cpb_idx].dst_cpb_hdr_phys );
			TEST_WrReg(g_fbdev_h15test_afbc_reg[1], 0x04, g_cpb_test_ctx.p[cpb_idx].dst_cpb_hdr_phys );
		}
#endif
		/* write OSD header */
		FBDEV_H15TEST_WriteOSDHdr(cpb_idx);

		/* clear NEW_IMAGE flag */
		g_cpb_test_ctx.cpb_hw_status &= ~FBDEV_H15TEST_CPB_STATUS_HW_NEW_IMAGE;

		FBDEV_HW_DEBUG("ISR END : view_cpb_idx = %d\n\n", cpb_idx );

		/* wake up vsync wait */
//		if (g_cpb_test_ctx.vsync_wait > 0 )
		{
			complete_all(&g_cpb_test_ctx.vsync_completion);
			g_cpb_test_ctx.vsync_wait = 0;
		}
	}
	spin_unlock_irqrestore(&g_fbdev_h15test_cpb_spinlock, flags);
}

static void		FBDEV_H15TEST_MakeCPB( int idx )
{
	FBDEV_CHECK_CODE( idx<0 || idx>2, return, "invalid idx %d\n", idx );
	FBDEV_DEBUG("idx %d, cmdbuf 0x%08x, src0 0x%08x src1 0x%08x -> dst_hdr 0x%08x dst_fb 0x%08x\n",
		idx, g_cpb_test_ctx.p[idx].gfx_cmd_buf,
		g_cpb_test_ctx.p[idx].src0_fb_phys, g_cpb_test_ctx.p[idx].src1_fb_phys,
		g_cpb_test_ctx.p[idx].dst_cpb_hdr_phys, g_cpb_test_ctx.p[idx].dst_cpb_fb_phys );

	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x000, 0x00071100 );
	OS_UsecDelay(10);
	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x000, 0x00070100 );

	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x014, 0xFFFFFFFF );	// INT enable
	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x558, 0x00800005 );	// AXI REORDER_DIS
	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x654, g_cpb_test_ctx.p[idx].gfx_cmd_buf );	// CMD_BUF_ADDR
	TEST_WrReg( g_fbdev_h15test_gfx_reg, 0x658, 0xFFFFFFFF );	// START_DMA
}

static void		FBDEV_H15TEST_WaitCPB( int idx )
{
	UINT64	tk[2];
	UINT64	tk_tmp;

	FBDEV_CHECK_CODE( idx<0 || idx>2, return, "invalid idx %d\n", idx );

	tk[0] = OS_GetMsecTicks();
	while(1)
	{
		OS_MsecSleep(100);	/* min 90 msec, avg 100 ms */

		//FBDEV_DEBUG("GFX VAL = 0x%08x\n", TEST_RdReg(g_fbdev_h15test_gfx_reg, 0x65c));

		if( 0 == TEST_RdReg(g_fbdev_h15test_gfx_reg, 0x65c) ) break;

		tk_tmp = OS_GetMsecTicks();

		/* notifiy some warning if GFX waiting takes too long time */
		if((int)(tk_tmp-tk[0]) > 10000) { FBDEV_DEBUG("++ GFX CPB OP wait for %d ms\n", (int)(tk_tmp-tk[0]) ); }
	}
	tk[1] = OS_GetMsecTicks();

	FBDEV_DEBUG("++ GFX CPB OP DONE @ %d ms\n", (int)(tk[1]-tk[0]) );
}

static void		TEST_WrReg ( volatile void* reg_base, UINT32 offset, UINT32 val )
{
	UINT8* vaddr = (UINT8*)reg_base + offset;

	 __raw_writel( val, (volatile void*)vaddr );
}

static UINT32	TEST_RdReg ( volatile void* reg_base, UINT32 offset )
{
	UINT8* vaddr = (UINT8*)reg_base + offset;

	return __raw_readl( (volatile void*)vaddr );
}

static void FBDEV_H15TEST_WriteOSDHdr(int idx)
{
	FBDEV_H15_OSD_HDR_T osd_hdr;
	FBDEV_H15_OSD_EXHDR_T osd_exhdr;

	UINT32	disp_width	= LX_RES_GET_WIDTH(lx_chip_graphic_res());
	UINT32	disp_height	= LX_RES_GET_HEIGHT(lx_chip_graphic_res());

	memset( &osd_hdr, 0x0, sizeof(FBDEV_H15_OSD_HDR_T));

	osd_hdr.osd_hdr_w_out       = disp_width;
	osd_hdr.osd_hdr_h_out       = disp_height;
	osd_hdr.osd_hdr_xpos        = 0;
	osd_hdr.osd_hdr_ypos        = 0;
	osd_hdr.osd_hdr_w_mem       = disp_width;
	osd_hdr.osd_hdr_h_mem       = disp_height;

	osd_hdr.osd_hdr_wpl         = (disp_width*4)>>3;

    osd_hdr.osd_hdr_format		= FBDEV_GetOSDPxlFmt(32);
    osd_hdr.osd_hdr_depth		= FBDEV_GetOSDPxlDepth(32);

	osd_hdr.osd_hdr_global_alpha_en = 0x1;	/* make opaque to view blending output */
	osd_hdr.osd_hdr_global_alpha	= 0xff;
	osd_hdr.osd_hdr_color_key		= 0xff;

	osd_hdr.osd_hdr_ptr_bmp		= g_cpb_test_ctx.p[idx].dst_cpb_fb_phys;

	osd_exhdr.bits_per_pixel 	= 32;

	FBDEV_H15CMN_WriteHdrRegs(0, &osd_hdr, &osd_exhdr);
}

#endif	/* FBDEV_H15_CPB_BW_TEST */
