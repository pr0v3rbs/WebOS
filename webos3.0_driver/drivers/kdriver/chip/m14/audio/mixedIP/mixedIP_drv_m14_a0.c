/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/



/** @file
 *
 *  driver interface header for Mixed IP device. ( used only within kdriver )
 *	Mixed IP device will teach you how to make device driver with new platform.
 *
 *  @author		Jong-Sang Oh(jongsang.oh@lge.com)
 *  @version	1.0
 *  @date		2013.04.03
 *  note		Additional information.
 *
 *  @addtogroup lg1152_adec
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "os_util.h" //for alloc functions

#include "audio_cfg.h"
#include "audio_kapi.h"
#include "audio_drv.h"
#include "audio_drv_master.h"
#include "audio_imc_func.h"

#include "debug_util.h"
#include "mixedIP_drv_m14_a0.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* SPDIF Sampling Clock frequency Number : 48, 44.1, 32Khz */
#define SPDIF_DTO_D_REG_NUMBER		3

/* SPDIF DTO Clock frequency and Rate Table */
static const UINT32 g_AUD_SpdifDtoDTable[SPDIF_ES_OUT_FREQ_NUMBER][LX_AUD_SPDIF_DTO_RATE_NUMBER][SPDIF_DTO_D_REG_NUMBER] =
{
	{
		/*	Sample Frequency : 48Khz */
		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0x26ABF, 0x13D9EFA8, 0x0009,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0x01193, 0x13DAAE4E, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x60ADF, 0x13DAD8AC, 0x0007,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x101CF, 0x13DB030A, 0x0006,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0x1355F, 0x13DB2D68, 0x0004,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0x182B7, 0x13DB57C6, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x0ABDF, 0x13DB8224, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x3056F, 0x13DBAC81, 0x000C,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x60ADF, 0x13DBD6DF, 0x000B,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0xF1B2F, 0x13DBF04B, 0x0005,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x00338, 0x13DC013D, 0x0009,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0xF1B2F, 0x13DC1230, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x2191B, 0x13DC29E9, 0x000A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x3056F, 0x13DC55F9, 0x0006,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0x02EDF, 0x13DC8057, 0x0004,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x182B7, 0x13DCAAB5, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x03DDF, 0x13DCD513, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x055EF, 0x13DCFF71, 0x0000,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x60ADF, 0x13DD29CE, 0x000B,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0C15B, 0x13DD542C, 0x0009,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0x26ABF, 0x13DE12D3, 0x0002,
		},
	},
	{
		/*	Sample Frequency : 44.1Khz */
		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0xC15BF, 0x123CDC5B, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0xC15BF, 0x123D85D2, 0x000A,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x60ADF, 0x123DC55F, 0x0008,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x101CF, 0x123DEFBD, 0x0006,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0x60ADF, 0x123E1A1B, 0x0004,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0x04D57, 0x123E4479, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x2039F, 0x123E6ED7, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x3056F, 0x123E9934, 0x000D,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x6B6BF, 0x123EC7CF, 0x0006,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0xF1B2F, 0x123EDCFE, 0x0005,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x0055E, 0x123EEDF0, 0x000A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0xF1B2F, 0x123EFEE3, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x09C3F, 0x123F1412, 0x0000,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x0464F, 0x123F42AC, 0x0006,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0x2039F, 0x123F6D0A, 0x0005,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x182B7, 0x123F9768, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x60ADF, 0x123FC1C6, 0x0001,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x0338F, 0x123FEC23, 0x000D,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x60ADF, 0x12401681, 0x000C,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0C15B, 0x124040DF, 0x000A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0xC15BF, 0x1240FF86, 0x0002,
		},
	},
	{
		/*	Sample Frequency : 32Khz */

		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0x182B7, 0x0D3B59C2, 0x000C,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0x04073, 0x0D3C033A, 0x0002,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x60ADF, 0x0D3C2D98, 0x0000,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x3056F, 0x0D3C57F5, 0x0011,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0xC15BF, 0x0D3C6D24, 0x0010,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0xC15BF, 0x0D3C9782, 0x000D,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x05DBF, 0x0D3CC1E0, 0x000B,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x101CF, 0x0D3D016D, 0x0007,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x28487, 0x0D3D3444, 0x0008,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0x157BF, 0x0D3D40FA, 0x0004,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x009AA, 0x0D3D5629, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0xC15BF, 0x0D3D6B58, 0x0002,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x78D97, 0x0D3D780D, 0x0010,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x0464F, 0x0D3DAAE4, 0x0011,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0xC15BF, 0x0D3DEA71, 0x000E,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x02AF7, 0x0D3DFFA0, 0x000D,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x1355F, 0x0D3E29FE, 0x000A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x3056F, 0x0D3E545C, 0x0008,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x2039F, 0x0D3E7EBA, 0x0005,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0C15B, 0x0D3EA918, 0x0003,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0x182B7, 0x0D3F528F, 0x000D,
		},
	},
};

#define SPDIF_DTO_A_REG_NUMBER		9

/* SPDIF DTO Clock frequency and Rate Table */
static const UINT32 g_AUD_SpdifDtoATable[SPDIF_ES_OUT_FREQ_NUMBER][LX_AUD_SPDIF_DTO_RATE_NUMBER][SPDIF_DTO_A_REG_NUMBER] =
{
	{
		/*	Sample Frequency : 48Khz */
		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x68,	0xBE,	0x2F,	0x00,	0x06,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x69,	0x87,	0x83,	0x00,	0x01,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x69,	0xA9,	0x11,	0x00,	0x00,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x69,	0xCA,	0x9E,	0x00,	0x0C,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0x0D,	0xBA,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0x2F,	0x48,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0x50,	0xD6,	0x00,	0x09,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0x72,	0x64,	0x00,	0x08,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0xB5,	0x80,	0x00,	0x06,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0xD7,	0x0E,	0x00,	0x05,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6A,	0xF8,	0x9C,	0x00,	0x04,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6B,	0x1A,	0x2A,	0x00,	0x04,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6B,	0x5D,	0x46,	0x00,	0x02,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6B,	0x7E,	0xD4,	0x00,	0x01,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6B,	0xA0,	0x62,	0x00,	0x00,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6B,	0xC1,	0xEF,	0x00,	0x0C,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6C,	0x05,	0x0B,	0x00,	0x0B,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6C,	0x26,	0x99,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6C,	0x48,	0x27,	0x00,	0x09,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6C,	0x69,	0xB5,	0x00,	0x08
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x13,	0x6D,	0x54,	0x97,	0x00,	0x02,
		},
	},
	{
		/*	Sample Frequency : 44.1Khz */
		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD5,	0x4D,	0xA4,	0x00,	0x0B,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD5,	0x6F,	0x32,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD5,	0xB2,	0x4E,	0x00,	0x08,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD5,	0xD3,	0xDC,	0x00,	0x07,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD5,	0xF5,	0x6A,	0x00,	0x06,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD6,	0x16,	0xF8,	0x00,	0x05,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD6,	0x5A,	0x14,	0x00,	0x04,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD6,	0x7B,	0xA2,	0x00,	0x03,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD6,	0x9D,	0x30,	0x00,	0x02,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD6,	0xBE,	0xBE,	0x00,	0x01,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0x01,	0xD9,	0x00,	0x0D,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0x23,	0x67,	0x00,	0x0C,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0x44,	0xF5,	0x00,	0x0B,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0x66,	0x83,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0xA9,	0x9F,	0x00,	0x09,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0xCB,	0x2D,	0x00,	0x08,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD7,	0xEC,	0xBB,	0x00,	0x07,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD8,	0x0E,	0x49,	0x00,	0x06,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD8,	0x51,	0x65,	0x00,	0x04,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD8,	0x72,	0xF3,	0x00,	0x03,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x11,	0xD8,	0x94,	0x81,	0x00,	0x02,
		},
	},
	{
		/*	Sample Frequency : 32Khz */
		/*	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEE,	0x6B,	0x83,	0x00,	0x00,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEE,	0xAE,	0x9E,	0x00,	0x11,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEE,	0xF1,	0xBA,	0x00,	0x0F,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEF,	0x77,	0xF2,	0x00,	0x09,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEF,	0xBB,	0x0E,	0x00,	0x07,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xEF,	0xFE,	0x2A,	0x00,	0x04,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF0,	0x41,	0x46,	0x00,	0x02,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF0,	0xC7,	0x7D,	0x00,	0x10,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF1,	0x0A,	0x99,	0x00,	0x0E,
		},
		/*	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF1,	0x4D,	0xB5,	0x00,	0x0B,
		},
		/*	LX_AUD_SPDIF_DTO_NORMAL */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF1,	0x90,	0xD1,	0x00,	0x08,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF2,	0x17,	0x09,	0x00,	0x03,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF2,	0x5A,	0x25,	0x00,	0x01,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF2,	0x9D,	0x40,	0x00,	0x12,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF2,	0xE0,	0x5C,	0x00,	0x0F,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF3,	0x66,	0x94,	0x00,	0x0A,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF3,	0xA9,	0xB0,	0x00,	0x08,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF3,	0xEC,	0xCC,	0x00,	0x05,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF4,	0x2F,	0xE8,	0x00,	0x02,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF4,	0xB6,	0x1F,	0x00,	0x11,
		},
		/*	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES */
		{
			0x0F,	0x42,	0x3F,	0x0C,	0xF4,	0xD7,	0xAD,	0x00,	0x10,
		},
	},
};


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
//ADEC KDRV Structure
extern LX_AUD_KDRV_T g_AudKdrv;

// DTO Rate
extern LX_AUD_SPDIF_DTO_RATE_T	g_prevDtoDRate[SPDIF_ES_OUT_DTO_CLOCK_TYPE][SPDIF_ES_OUT_FREQ_NUMBER];
extern LX_AUD_CLOCK_SRC_T		g_setClockSrc;


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
//Current SCART Boost Gain Setting
static UINT16 gScartBoostGain_M14A0 = 0;

//Current ADEC Clock Source Setting
static LX_AUD_CLOCK_SRC_T gAdecClockSource_M14A0 = LX_AUD_CLOCK_UNKNOWN;

//Current AAD Bypass Enabled
static BOOLEAN gAadBypassEnabled_M14A0 = FALSE;

//Current SRC Input Sampling Frequency Setting
static LX_AUD_SAMPLING_FREQ_T gAdecSrcFreq_M14A0 = LX_AUD_SAMPLING_FREQ_NONE;

//Current SPDIF Sampling Frequency Setting
static LX_AUD_SAMPLING_FREQ_T gAdecSpdifFreq_M14A0[AUD_DTO_MAX] = {LX_AUD_SAMPLING_FREQ_NONE, LX_AUD_SAMPLING_FREQ_NONE};

//Current PCM clock Rate Setting
static UINT32 	gAdecPcmClock_M14A0 = APLL_DSP_VALUE_12_288MHZ;	//18,000

//Current SPDIF DTO Rate Setting
static LX_AUD_SPDIF_DTO_RATE_T gAdecSpdifDtoRate_M14A0[AUD_DTO_MAX] = {LX_AUD_SPDIF_DTO_UNKNOWN, LX_AUD_SPDIF_DTO_UNKNOWN};

//Current ADEC SPDIF Output Type
static LX_AUD_MASTER_SPDIF_OUTPUT_T gAdecSpdifOutput_M14A0 = LX_AUD_MASTER_SPDIF_PCM;

//Current LX_AUD_SIF_INPUT_T sifSource
static LX_AUD_SIF_INPUT_T gSifSource = LX_AUD_SIF_INPUT_INTERNAL;


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 *  M14A0_ANALOG_ReadRegister
 *
 *  @parm UINT32 *
 *  @return int
*/
int M14A0_ANALOG_ReadRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 *value)
{
	int 	ret = RET_OK;

	//Read I2C register
	ret = (UINT8)ACE_RegRead(slaveAddr << 1, regAddr, value);

//	AUD_KDRV_MIP_I2C("M14A0 Read  : Slave 0x%2x, reg 0x%2x  Data = 0x%2x\n", slaveAddr, regAddr, *value);
//	AUD_KDRV_ERROR("M14A0 Read  : Slave 0x%2x, reg 0x%2x  Data = 0x%2x\n", slaveAddr, regAddr, *value);

	return ret;
}

/**
 *  M14A0_ANALOG_WriteRegister
 *
 *  @parm UINT32 *
 *  @return int
*/
int M14A0_ANALOG_WriteRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 value)
{
	int 	ret = RET_OK;

	//Write I2C register
	(void)ACE_RegWrite(slaveAddr << 1, regAddr, value);

	AUD_KDRV_MIP_I2C("M14A0 Write : Slave 0x%2x, reg 0x%2x  Data = 0x%2x\n", slaveAddr, regAddr, value);

	return ret;
}

/**
 * Initializes the clock and reset module register value.
 * @see ADEC_Init
*/
int M14A0_MIXED_IP_ADEC_InitClockRegisters( void )
{
	//Initialize clock registers.
	//clock setting for APLL block
	ACE_REG_M14A0_RdFL(audio_pll_0);
	ACE_REG_M14A0_Wr01(audio_pll_0, apll_od, 	3);		// Value of output divider(low for Audio PLL application)
	ACE_REG_M14A0_Wr01(audio_pll_0, apll_pdb, 	1);		// 0 :power down, 1 : normal operation,  Analog & digital block power down pin
	ACE_REG_M14A0_WrFL(audio_pll_0);

	ACE_REG_M14A0_RdFL(audio_pll_1);
	ACE_REG_M14A0_Wr01(audio_pll_1, apll_fcw_2,	0x04);	// FCW2 : Value of output divider (low for Audio PLL application)
	ACE_REG_M14A0_WrFL(audio_pll_1);

	ACE_REG_M14A0_RdFL(audio_pll_2);
	ACE_REG_M14A0_Wr01(audio_pll_2, apll_fcw_1,	0x65);	// FCW1 : Value of output divider (low for Audio PLL application)
	ACE_REG_M14A0_WrFL(audio_pll_2);

	ACE_REG_M14A0_RdFL(audio_pll_3);
	ACE_REG_M14A0_Wr01(audio_pll_3, apll_fcw_0,	0x00);	// FCW0 : Value of output divider (low for Audio PLL application)
	ACE_REG_M14A0_WrFL(audio_pll_3);

	ACE_REG_M14A0_RdFL(audio_pll_4);
	ACE_REG_M14A0_Wr01(audio_pll_4, apll_ci,	0x03);	// Icp Control for CP
	ACE_REG_M14A0_WrFL(audio_pll_4);

	ACE_REG_M14A0_RdFL(audio_pll_5);
	ACE_REG_M14A0_Wr01(audio_pll_5, apll_pre_fd,  3);	// Feedbck divider value
	ACE_REG_M14A0_Wr01(audio_pll_5, apll_udex, 0x00);	// Spread ratio control
	ACE_REG_M14A0_Wr01(audio_pll_5, apll_ibw,     0);	// Duty ratio compensation control
	ACE_REG_M14A0_WrFL(audio_pll_5);

	ACE_REG_M14A0_RdFL(audio_pll_6);
	ACE_REG_M14A0_Wr01(audio_pll_6, apll_dco_g, 0x3);	// DAC output swing control
	ACE_REG_M14A0_Wr01(audio_pll_6, apll_nsc,   0x4);	// Feedback dividing valdue control (Swallow counter value)
	ACE_REG_M14A0_WrFL(audio_pll_6);

	ACE_REG_M14A0_RdFL(audio_pll_7);
	ACE_REG_M14A0_Wr01(audio_pll_7, apll_m,    0x03);	// Forward input dividing value control
	ACE_REG_M14A0_WrFL(audio_pll_7);

	ACE_REG_M14A0_RdFL(audio_pll_8);
	ACE_REG_M14A0_Wr01(audio_pll_8, apll_npc,  0x07);	// Feedback dividing value control (Programmable counter value)
	ACE_REG_M14A0_WrFL(audio_pll_8);

	ACE_REG_M14A0_RdFL(audio_pll_9);
	ACE_REG_M14A0_Wr01(audio_pll_9, apll_msex_1,  0);	// Modulation speed control
	ACE_REG_M14A0_WrFL(audio_pll_9);

	ACE_REG_M14A0_RdFL(audio_pll_10);
	ACE_REG_M14A0_Wr01(audio_pll_10, apll_msex_0, 0x00);// Modulation speed control
	ACE_REG_M14A0_WrFL(audio_pll_10);

	ACE_REG_M14A0_RdFL(audio_pll_11);
	ACE_REG_M14A0_Wr01(audio_pll_11, apll_dco_lpf, 		1);	// LPF cutoff frequency control (00: Fcutoff=24MHz, 01: Fcutoff=15.2MHz, 11: Fcutoff=8.8MHz)
	ACE_REG_M14A0_Wr01(audio_pll_11, apll_dco_resetb,   1);	// DCO power down(0: power down, 1: power on)
	ACE_REG_M14A0_Wr01(audio_pll_11, apll_dco_rom_test, 0);	// DCO ROM test
	ACE_REG_M14A0_Wr01(audio_pll_11, apll_mode, 	  0x0);	// Audio mode enable pin
	ACE_REG_M14A0_WrFL(audio_pll_11);

	//clock DCO value setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_8);
	ACE_REG_M14A0_Wr01(afe_aad_8,  aad_dco_fcw_2, 0x0E);

	ACE_REG_M14A0_RdFL(afe_aad_9);
	ACE_REG_M14A0_Wr01(afe_aad_9,  aad_dco_fcw_1, 0x90);

	ACE_REG_M14A0_RdFL(afe_aad_10);
	ACE_REG_M14A0_Wr01(afe_aad_10, aad_dco_fcw_0, 0x45);

	// For Sync Write Mode
	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 1);
	ACE_REG_M14A0_WrFL(top_control_2);

	//Update aad dco registers.
	ACE_REG_M14A0_WrFL(afe_aad_8);
	ACE_REG_M14A0_WrFL(afe_aad_9);
	ACE_REG_M14A0_WrFL(afe_aad_10);

	// For Sync Write Mode
	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_force_sync, 1);
	ACE_REG_M14A0_WrFL(top_control_2);

	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 	 0);
	ACE_REG_M14A0_WrFL(top_control_2);

	ACE_REG_M14A0_RdFL(afe_aad_11);
	ACE_REG_M14A0_Wr01(afe_aad_11, aad_dco_lpfon, 	 1);	// LPF cutoff frequency control (00: Fcutoff=24MHz, 01: Fcutoff=15.2MHz, 11: Fcutoff=8.8MHz)
	ACE_REG_M14A0_WrFL(afe_aad_11);

	ACE_REG_M14A0_RdFL(afe_aad_12);
	ACE_REG_M14A0_Wr01(afe_aad_12, aad_spll_n, 	  0x07);	// Feedback dividing value control (Programmable counter value)
	ACE_REG_M14A0_WrFL(afe_aad_12);

	ACE_REG_M14A0_RdFL(afe_aad_13);
	ACE_REG_M14A0_Wr01(afe_aad_13, aad_spll_od, 	 3);	// Outout divider value control
	ACE_REG_M14A0_Wr01(afe_aad_13, aad_spll_ci,    0x3);	// Icp Control
	ACE_REG_M14A0_WrFL(afe_aad_13);

	//clock divide value setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_15);
	ACE_REG_M14A0_Wr01(afe_aad_15, aad_adcclk_test, 0);
	ACE_REG_M14A0_Wr01(afe_aad_15, aad_fs00clk_sel, 0);
	ACE_REG_M14A0_Wr01(afe_aad_15, aad_fs01clk_sel, 0);
	ACE_REG_M14A0_Wr01(afe_aad_15, aad_fs02clk_sel, 0);
	ACE_REG_M14A0_WrFL(afe_aad_15);

	ACE_REG_M14A0_RdFL(afe_aad_16);
	ACE_REG_M14A0_Wr01(afe_aad_16, aad_spll_mode, 0x7);		// Audio mode enable
	ACE_REG_M14A0_Wr01(afe_aad_16, aad_spll_m, 	 0x01);		// Forward input dividing value control
	ACE_REG_M14A0_WrFL(afe_aad_16);

	ACE_REG_M14A0_RdFL(afe_aad_17);
	ACE_REG_M14A0_Wr01(afe_aad_17, aad_spll_ibw,    0);		// Duty ratio compensation control
	ACE_REG_M14A0_Wr01(afe_aad_17, aad_spll_nsc,  0x4);		// Feedback dividing valdue control (Swallow counter value)
	ACE_REG_M14A0_WrFL(afe_aad_17);

	ACE_REG_M14A0_RdFL(afe_aad_18);
	ACE_REG_M14A0_Wr01(afe_aad_18, aad_spll_udex,0x00);		// Spread ratio control
	ACE_REG_M14A0_WrFL(afe_aad_18);

	ACE_REG_M14A0_RdFL(afe_aad_19);
	ACE_REG_M14A0_Wr01(afe_aad_19, aad_spll_msex_1, 0);		// Modulation speed control
	ACE_REG_M14A0_WrFL(afe_aad_19);

	ACE_REG_M14A0_RdFL(afe_aad_20);
	ACE_REG_M14A0_Wr01(afe_aad_20, aad_spll_msex_0, 0x00);	// Modulation speed control
	ACE_REG_M14A0_WrFL(afe_aad_20);

	ACE_REG_M14A0_RdFL(afe_aad_21);
	ACE_REG_M14A0_Wr01(afe_aad_21, aad_spll_dco_g,	0x3);	// DAC output swing control pin (Default = 011)
	ACE_REG_M14A0_Wr01(afe_aad_21, aad_spll_pre_fd,	0x2);	// Feedbck divider value
	ACE_REG_M14A0_WrFL(afe_aad_21);

	//clock setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_7);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_selref, 3);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_pdb, 	  0);	// AFE ADC power down(0 :power down, 1 : normal operation)
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_rtest,  0);	// DCO ROM test
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_resetb, 0);	// DCO power down(0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_7);

	ACE_REG_M14A0_RdFL(afe_aad_14);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 	 0);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 0);	// SPLL power down (0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_14);

	//delay before sw reset control
	udelay(MIXEDIP_RESET_DELAY_100US);

	//clock setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_7);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_selref, 3);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_pdb, 	  1);	// AFE ADC power down(0 :power down, 1 : normal operation)
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_rtest,  0);	// DCO ROM test
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_resetb, 1);	// DCO power down(0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_7);

	ACE_REG_M14A0_RdFL(afe_aad_14);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 	 0);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 1);	// SPLL power down (0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_14);

	//Select a ABB SIF input
	ACE_REG_M14A0_RdFL(abb_3);
	ACE_REG_M14A0_Wr01(abb_3, abb_mux_sel_aad, 1);			// AAD_SEL(0 : RC Path, 1: ADC Path)
	ACE_REG_M14A0_WrFL(abb_3);

	//Fix AAD_AFE data output setup time error : change fs00_clk_out inversion : 2011.10.27
	ACE_REG_M14A0_RdFL(clock_control_2);
	ACE_REG_M14A0_Wr01(clock_control_2, fs00_clk, 1);
	ACE_REG_M14A0_WrFL(clock_control_2);

	//Setting Audio Clock Mux
	//Read clock mux reg.
	ACE_REG_M14A0_RdFL(soft_reset_6);
	ACE_REG_M14A0_RdFL(soft_reset_7);
	ACE_REG_M14A0_RdFL(soft_reset_8);
	ACE_REG_M14A0_RdFL(soft_reset_9);
	ACE_REG_M14A0_RdFL(soft_reset_10);

	ACE_REG_M14A0_Wr01(soft_reset_6, aclk_mux1_src,  0);
	ACE_REG_M14A0_Wr01(soft_reset_7, aclk_mux1_div,  1);
	ACE_REG_M14A0_Wr01(soft_reset_7, aclk_mux2_src,  1);	// ADC : A-PLL(1)
	ACE_REG_M14A0_Wr01(soft_reset_8, aclk_mux2_div,  0);	// 0:bypass
	ACE_REG_M14A0_Wr01(soft_reset_8, aclk_mux3_src,  1);
	ACE_REG_M14A0_Wr01(soft_reset_9, aclk_mux3_div,  0);
	ACE_REG_M14A0_Wr01(soft_reset_9, aclk_mux4_src,  2);	// 2:Digital DTO
	ACE_REG_M14A0_Wr01(soft_reset_10,aclk_mux4_div,  0);	// 0:bypass

	//Write clock mux reg.
	ACE_REG_M14A0_WrFL(soft_reset_6);
	ACE_REG_M14A0_WrFL(soft_reset_7);
	ACE_REG_M14A0_WrFL(soft_reset_8);
	ACE_REG_M14A0_WrFL(soft_reset_9);
	ACE_REG_M14A0_WrFL(soft_reset_10);

	//Setting HDMI Main CLK for CTOP Control.(0 : Non-Inversion, 1 : Inversion)
	CTOP_CTRL_M14A0_RdFL(ctr00);
	CTOP_CTRL_M14A0_Wr01(ctr00, aud_hdmi_mclk_sel, 0x0);
	CTOP_CTRL_M14A0_WrFL(ctr00);

	//Setting Main MCLK for CTOP Control.
	CTOP_CTRL_M14A0_RdFL(ctr80);
	CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk, 0x0);
	CTOP_CTRL_M14A0_WrFL(ctr80);

	//Setting D-DTO clock for CTOP Control.
	//Update add / error value register : 2012.01.11 by joonil.lee request
	CTOP_CTRL_M14A0_RdFL(ctr67);
	CTOP_CTRL_M14A0_Wr01(ctr67, adto_add_value, 0x13DC013D);
	CTOP_CTRL_M14A0_WrFL(ctr67);

	CTOP_CTRL_M14A0_RdFL(ctr68);
	CTOP_CTRL_M14A0_Wr01(ctr68, adto_err_value, 0x009);
	CTOP_CTRL_M14A0_WrFL(ctr68);

	//Update loop value register in last : 2012.01.11 by joonil.lee request
	CTOP_CTRL_M14A0_RdFL(ctr66);
	CTOP_CTRL_M14A0_Wr01(ctr66, adto_loop_value, 0x00338);
	CTOP_CTRL_M14A0_WrFL(ctr66);

	//delay before sw reset control
	udelay(MIXEDIP_RESET_DELAY_100US);

	//sw reset setting for mixed-IP block
	ACE_REG_M14A0_RdFL(soft_reset_1);
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 0);	//audio codec : dac0
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 0);	//audio codec : dac1
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auad, 0);
	ACE_REG_M14A0_WrFL(soft_reset_1);

	ACE_REG_M14A0_RdFL(soft_reset_2);
	ACE_REG_M14A0_Wr01(soft_reset_2, swrst_fs00, 0);
	ACE_REG_M14A0_WrFL(soft_reset_2);

	ACE_REG_M14A0_RdFL(soft_reset_4);
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_adto, 0);	//SPDIF
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_aclk_mux2, 0);
	ACE_REG_M14A0_WrFL(soft_reset_4);

	ACE_REG_M14A0_RdFL(soft_reset_5);
//	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs25clk, 0);		//SRC output I2S clock
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs24clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs23clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs21clk, 0);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 0);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out,  0);		//D-AMP I2S clock
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out_sub, 0);	//dac sub I2S clock
	ACE_REG_M14A0_WrFL(soft_reset_5);

	ACE_REG_M14A0_RdFL(soft_reset_14);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 0);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 0);
	ACE_REG_M14A0_WrFL(soft_reset_14);

	//Reset D-DTO and Codec IP clock.
	CTOP_CTRL_M14A0_RdFL(ctr00);
	CTOP_CTRL_M14A0_Wr01(ctr00, swrst_adto,  0x0);
	CTOP_CTRL_M14A0_WrFL(ctr00);

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_InitClockRegisters\n");
	return RET_OK;
}

/**
 * Resets the clock register value.
 * @see ADEC_Init
*/
int M14A0_MIXED_IP_ADEC_ResetClockRegisters( void )
{
	//sw reset setting for mixed-IP block
	ACE_REG_M14A0_RdFL(soft_reset_1);
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 1);	//audio codec : dac0
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 1);	//audio codec : dac1
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auad, 1);
	ACE_REG_M14A0_WrFL(soft_reset_1);

	ACE_REG_M14A0_RdFL(soft_reset_2);
	ACE_REG_M14A0_Wr01(soft_reset_2, swrst_fs00, 1);
	ACE_REG_M14A0_WrFL(soft_reset_2);

	ACE_REG_M14A0_RdFL(soft_reset_4);
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_adto, 1);
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_aclk_mux2, 1);
	ACE_REG_M14A0_WrFL(soft_reset_4);

	ACE_REG_M14A0_RdFL(soft_reset_5);
//	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs25clk, 1); 	//SRC output I2S clock
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs24clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs23clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs21clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out,	1); 	//D-AMP I2S clock
//	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out_sub, 1);		//dac sub I2S clock
	ACE_REG_M14A0_WrFL(soft_reset_5);

	ACE_REG_M14A0_RdFL(soft_reset_14);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 1);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 1);
	ACE_REG_M14A0_WrFL(soft_reset_14);

	//Reset D-DTO and Codec IP clock.
	CTOP_CTRL_M14A0_RdFL(ctr00);
	CTOP_CTRL_M14A0_Wr01(ctr00, swrst_adto,  0x1);
	CTOP_CTRL_M14A0_WrFL(ctr00);

	//add delay for clock setting
	udelay(MIXEDIP_RESET_DELAY_100US * 2);

	//sw reset setting for mixed-IP block
	ACE_REG_M14A0_RdFL(soft_reset_1);
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 0);	//audio codec : dac0
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 0);	//audio codec : dac1
	ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auad, 0);
	ACE_REG_M14A0_WrFL(soft_reset_1);

	ACE_REG_M14A0_RdFL(soft_reset_2);
	ACE_REG_M14A0_Wr01(soft_reset_2, swrst_fs00, 0);
	ACE_REG_M14A0_WrFL(soft_reset_2);

	ACE_REG_M14A0_RdFL(soft_reset_4);
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_adto, 0);
	ACE_REG_M14A0_Wr01(soft_reset_4, swrst_aclk_mux2, 0);
	ACE_REG_M14A0_WrFL(soft_reset_4);

	ACE_REG_M14A0_RdFL(soft_reset_5);
//	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs25clk, 0); 	//SRC output I2S clock
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs24clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs23clk, 1);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs21clk, 0);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 0);
	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out,	0); 	//D-AMP I2S clock
//	ACE_REG_M14A0_Wr01(soft_reset_5, swrst_audclk_out_sub, 0);	//dac sub I2S clock
	ACE_REG_M14A0_WrFL(soft_reset_5);

	ACE_REG_M14A0_RdFL(soft_reset_14);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 0);
	ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 0);
	ACE_REG_M14A0_WrFL(soft_reset_14);

	//Reset D-DTO and Codec IP clock.
	CTOP_CTRL_M14A0_RdFL(ctr00);
	CTOP_CTRL_M14A0_Wr01(ctr00, swrst_adto,  0x0);
	CTOP_CTRL_M14A0_WrFL(ctr00);

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_ResetClockRegisters\n");
	return RET_OK;
}

/**
 * Resets the AAD clock register value.
 * @see ADEC_Init
*/
int M14A0_MIXED_IP_ADEC_ResetAADClockRegisters( void )
{
#if 1
	AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_ResetAADClockRegisters\n");
	return RET_OK;
#else
	//Initialize AAD clock registers.
	/* Workaround code for AAD clock latchup. */
	ACE_REG_M14A0_RdFL(main_pll_4);
	ACE_REG_M14A0_Wr01(main_pll_4, dr3p_pdb, 0);
	ACE_REG_M14A0_WrFL(main_pll_4);

	ACE_REG_M14A0_RdFL(main_pll_4);
	ACE_REG_M14A0_Wr01(main_pll_4, dr3p_pdb, 1);
	ACE_REG_M14A0_WrFL(main_pll_4);
	/* ****************************** */

	//delay before sw reset control
	udelay(MIXEDIP_RESET_DELAY_100US);

	//clock DCO value setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_8);
	ACE_REG_M14A0_Wr01(afe_aad_8,  aad_dco_fcw_2, 0x0E);

	ACE_REG_M14A0_RdFL(afe_aad_9);
	ACE_REG_M14A0_Wr01(afe_aad_9,  aad_dco_fcw_1, 0x90);

	ACE_REG_M14A0_RdFL(afe_aad_10);
	ACE_REG_M14A0_Wr01(afe_aad_10, aad_dco_fcw_0, 0x45);

	// For Sync Write Mode
	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 1);
	ACE_REG_M14A0_WrFL(top_control_2);

	//Update add / error value register : 2012.01.11 by joonil.lee request
	ACE_REG_M14A0_WrFL(afe_aad_8);
	ACE_REG_M14A0_WrFL(afe_aad_9);
	ACE_REG_M14A0_WrFL(afe_aad_10);

	// For Sync Write Mode
	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_force_sync, 1);
	ACE_REG_M14A0_WrFL(top_control_2);

	ACE_REG_M14A0_RdFL(top_control_2);
	ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 0);
	ACE_REG_M14A0_WrFL(top_control_2);

	//clock setting for aad/btsc block
	//Change DCO clock reset sequence to workaround ATV noise.(2013.08.20)
	ACE_REG_M14A0_RdFL(afe_aad_7);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_selref, 3);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_pdb, 	  0);	// 0 :power down, 1 : normal operation
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_rtest,  1);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_resetb, 0);	// 0 :power down, 1 : normal operation
	ACE_REG_M14A0_WrFL(afe_aad_7);

	//clock SPLL value setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_14);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 	 0);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 0);	// SPLL power down (0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_14);

	//delay before sw reset control
	udelay(MIXEDIP_RESET_DELAY_100US);

	ACE_REG_M14A0_RdFL(afe_aad_7);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_selref, 3);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_adc_pdb, 	  1);	// 0 :power down, 1 : normal operation
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_rtest,  1);
	ACE_REG_M14A0_Wr01(afe_aad_7, aad_dco_resetb, 1);	// 0 :power down, 1 : normal operation
	ACE_REG_M14A0_WrFL(afe_aad_7);

	//clock SPLL value setting for aad/btsc block
	ACE_REG_M14A0_RdFL(afe_aad_14);
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);

	/* Set a SIF Input Source for A_DEMOD & AAD Module. */
	if(gSifSource == LX_AUD_SIF_INPUT_INTERNAL)
	{
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 0);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
	}
	else
	{
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 1);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
	}
	ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 1);	// SPLL power down (0: power down, 1: power on)
	ACE_REG_M14A0_WrFL(afe_aad_14);

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_ResetAADClockRegisters\n");
	return RET_OK;
#endif
}

/**
 * Initializes the Mixed IP ADC, MIC ADC, DAC and HP module register value.
 * @see ADEC_DRV_InitModule
*/
int M14A0_MIXED_IP_ADEC_SetClockSource( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, LX_AUD_CLOCK_SRC_T ui8AdecClockSrc, LX_AUD_MASTER_SPDIF_OUTPUT_T spdifOutput )
{
	BOOLEAN bAadBypassEnabled = FALSE;

	DtoCtrlCmdSetDtorate		setDtoRate;
	LX_AUD_SPDIF_DTO_RATE_T		spdifDTORate;

	//Check previous clock source
	if(g_AudKdrv.bInitDone == TRUE && gAdecSrcFreq_M14A0 == ui32SrcFreq && gAdecClockSource_M14A0 == ui8AdecClockSrc && gAdecSpdifOutput_M14A0 == spdifOutput)
	{
		AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetClockSource : same freq = %d, src = %d\n", ui32SrcFreq, ui8AdecClockSrc);
		return RET_OK;
	}

	//Check AAD Bypass Enabled Status
	if( (ui8AdecClockSrc == LX_AUD_CLOCK_ATV)
	  ||(ui8AdecClockSrc == LX_AUD_CLOCK_ADC)
	  ||(ui8AdecClockSrc == LX_AUD_CLOCK_HDMI) )
	{
		bAadBypassEnabled = TRUE;
	}
	else
	{
		bAadBypassEnabled = FALSE;
	}

	//Reset on for Mixed IP Module
	if(bAadBypassEnabled == FALSE || gAadBypassEnabled_M14A0 != bAadBypassEnabled)
	{
		ACE_REG_M14A0_RdFL(acodec_20);
		ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 1);	// 1 bit : Line DAC CH0 Mute Control
		ACE_REG_M14A0_WrFL(acodec_20);

		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 1);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);
		}

		OS_MsecSleep(MIXEDIP_RESET_DELAY_10MS);

		ACE_REG_M14A0_RdFL(soft_reset_14);
		ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 1);	//audio codec : dac 0

		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 1);	//audio codec : dac 1
		}
		ACE_REG_M14A0_WrFL(soft_reset_14);

		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(soft_reset_1);
		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 1);	//audio codec : dac0

		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 1);	//audio codec : dac1
		}
		ACE_REG_M14A0_WrFL(soft_reset_1);

		ACE_REG_M14A0_RdFL(soft_reset_5);
		ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 1);	//spk, hpdrv, scart clock change only
		ACE_REG_M14A0_WrFL(soft_reset_5);
	}
	else	//SCART port is not reset.
	{
		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 1);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);

			OS_MsecSleep(MIXEDIP_RESET_DELAY_10MS);

			ACE_REG_M14A0_RdFL(soft_reset_14);
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 1);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_14);

			udelay(MIXEDIP_RESET_DELAY_100US);

			ACE_REG_M14A0_RdFL(soft_reset_1);
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 1);	//audio codec : dac1
			ACE_REG_M14A0_WrFL(soft_reset_1);

			ACE_REG_M14A0_RdFL(soft_reset_5);
			ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 1);	//spk, hpdrv, scart, clock change only
			ACE_REG_M14A0_WrFL(soft_reset_5);
		}
	}

	//add delay for clock setting
	udelay(MIXEDIP_RESET_DELAY_100US * 2);

	//Set default PCM frequency to 48Khz and notify input sampling frequency to DSP
	(void)M14A0_MIXED_IP_ADEC_SetPcmClockRate(ui32SrcFreq, APLL_DSP_VALUE_12_288MHZ);

	//Set default SPDIF sampling frequency to 48Khz
	spdifDTORate = MIXED_IP_AUDIO_GetDTORate(ui8AdecClockSrc, LX_AUD_SAMPLING_FREQ_48_KHZ, AUD_DTO_A);
	(void)M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq(LX_AUD_SAMPLING_FREQ_48_KHZ, spdifDTORate, AUD_DTO_A);

	//Setting Audio Clock Mux
	//Read clock mux reg.
	ACE_REG_M14A0_RdFL(soft_reset_14);
	ACE_REG_M14A0_RdFL(soft_reset_15);

	//Setting Main MCLK for CTOP Control.
	CTOP_CTRL_M14A0_RdFL(ctr80);
	CTOP_CTRL_M14A0_RdFL(ctr81);

	if( (ui8AdecClockSrc == LX_AUD_CLOCK_DTV)		//DTV to SPK(default)
      ||(ui8AdecClockSrc == LX_AUD_CLOCK_ADC) ) 	//ADC to SPK, SCART AAD bypass
	{
		AUD_KDRV_MIP_DEBUG("LX_AUD_CLOCK_DTV\n");

		if( ui8AdecClockSrc == LX_AUD_CLOCK_DTV )	//DTV to SPK(default)
		{
			//Disable SCART bypass function
			ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_src,  1);
			ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_div,  0);
			ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_src, 1);
			ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_div, 0);

			CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_src0, 	 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_div0, 	 0x0);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_src, 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_src, 0x2);	//SPDIF : AAD(0), A-DTO(2), D-DTO(4)
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_src, 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_src, 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_div, 0x2);
		}
   		else	//( ui8AdecClockSrc == LX_AUD_CLOCK_ADC ) //ADC to SPK, SCART AAD bypass
		{
			//Enable SCART bypass function
			ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_src,  0);
			ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_div,  1);
			ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_src, 1);
			ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_div, 0);

			CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_src0, 	 0x1);	// 1:APLL
			CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_div0, 	 0x0);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_src, 0x1);	// 1:APLL
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_src, 0x2);	//SPDIF : AAD(0), A-DTO(2), D-DTO(4)
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_src, 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_div, 0x2);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_src, 0x1);
			CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_div, 0x2);
		}
	}
	else if(ui8AdecClockSrc == LX_AUD_CLOCK_HDMI) //HDMI to SPK, SCART AAD bypass
	{
		AUD_KDRV_MIP_DEBUG("LX_AUD_CLOCK_HDMI\n");

		//Enable SCART bypass function
		ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_src,  0);
		ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_div,  1);
		ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_src, 1);		// 0x1 : APLL, 0x2 : A-DTO
		ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_div, 0);

		CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_src0, 	 0x1);		// 0x1 : APLL, 0x2 : A-DTO
		CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_div0, 	 0x0);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_src, 0x1);		//SPK, HP, SCART => 0x1 : APLL, 0x2 : A-DTO
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_src, 0x2);		//SPDIF : AAD(0), A-DTO(2), D-DTO(4)
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_src, 0x3);		//HW SRC IN  => 0x3 : HDMI
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_src, 0x2);		//HW SRC OUT => 0x1 : APLL, 0x2 : A-DTO
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_div, 0x2);
	}
	else if( ui8AdecClockSrc == LX_AUD_CLOCK_ATV ) //Analog TV to SPK, SCART AAD bypass
	{
		AUD_KDRV_MIP_DEBUG("LX_AUD_CLOCK_ATV\n");

		//Enable SCART bypass function
		ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_src,  0);
		ACE_REG_M14A0_Wr01(soft_reset_14, aclk_mux9_div,  1);
		ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_src, 1);		// 0x0 : AAD, 0x1 : APLL
		ACE_REG_M14A0_Wr01(soft_reset_15, aclk_mux10_div, 0);

		CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_src0, 	 0x1);		// 0x0 : AAD, 0x1 : APLL
		CTOP_CTRL_M14A0_Wr01(ctr80, sel_audclk_div0, 	 0x0);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_src, 0x1);		// 0x0 : AAD, 0x1 : APLL
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs20clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_src, 0x2);		//SPDIF : AAD(0), A-DTO(2), D-DTO(4)
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs21clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_src, 0x0);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs23clk_div, 0x2);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_src, 0x0);
		CTOP_CTRL_M14A0_Wr01(ctr81, sel_aud_fs24clk_div, 0x2);
	}
	else
	{
		AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_SetClockSource : 0x%X(Not Supported!!!)\n", ui8AdecClockSrc);
		return RET_ERROR;
	}

	//Write clock mux reg.
	ACE_REG_M14A0_WrFL(soft_reset_14);
	ACE_REG_M14A0_WrFL(soft_reset_15);

	//Setting Main MCLK for CTOP Control.
	CTOP_CTRL_M14A0_WrFL(ctr80);
	CTOP_CTRL_M14A0_WrFL(ctr81);

	//add delay for clock setting
	udelay(MIXEDIP_RESET_DELAY_100US * 2);

	//Reset off for Mixed IP Module
	if(bAadBypassEnabled == FALSE || gAadBypassEnabled_M14A0 != bAadBypassEnabled)
	{
		//Wait for SCART & HP reset
		ACE_REG_M14A0_RdFL(soft_reset_5);
		ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 0);	//hpdrv
		ACE_REG_M14A0_WrFL(soft_reset_5);

		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			ACE_REG_M14A0_RdFL(soft_reset_1);
	//		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 0);	//audio IP reset : dac 0,
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 0);	//audio IP reset : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_1);

			udelay(MIXEDIP_RESET_DELAY_100US);

			ACE_REG_M14A0_RdFL(soft_reset_14);
	//		ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 0);	//bclk reset : dac 0
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 0);	//bclk reset : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_14);

			OS_MsecSleep(MIXEDIP_RESET_DELAY_10MS);

	//		ACE_REG_M14A0_RdFL(acodec_20);
	//		ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 0);		// 1 bit : Line DAC CH0 Mute Control
	//		ACE_REG_M14A0_WrFL(acodec_20);

			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 0);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);

			/* To protect mute off timing : 2013.02.04 */
			OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);	//30ms
		}
	}
	else	//SCART port is not reset.
	{
		/* Set a HP reset for init only */
		if(g_AudKdrv.bInitDone == FALSE)
		{
			//Wait for SCART & HP reset
			ACE_REG_M14A0_RdFL(soft_reset_5);
			ACE_REG_M14A0_Wr01(soft_reset_5, swrst_aud_fs20clk, 0);	//hpdrv, clock change only
			ACE_REG_M14A0_WrFL(soft_reset_5);

			ACE_REG_M14A0_RdFL(soft_reset_1);
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 0);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_1);

			udelay(MIXEDIP_RESET_DELAY_100US);

			ACE_REG_M14A0_RdFL(soft_reset_14);
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 0);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_14);

			OS_MsecSleep(MIXEDIP_RESET_DELAY_10MS);

			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 0);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);

			/* To protect mute off timing : 2013.02.04 */
			OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);	//30ms
		}
	}

	//Send a IMC command for DTO setting
	if(g_AudKdrv.bInitDone == TRUE)
	{
		// Set DTO Rate
		setDtoRate.dtorate			= (UINT32)spdifDTORate;
		setDtoRate.force_cnt_clear	= (UINT32)1;
		AUDIO_IMC_SendCmdParam(DTOCTRL_CMD_SET_DTORATE, ADEC_MODULE_DTO_0, sizeof(DtoCtrlCmdSetDtorate), &setDtoRate);
	}

	//Save current SRC frequency
	gAdecSrcFreq_M14A0 = ui32SrcFreq;

	//Save current clock source
	gAdecClockSource_M14A0 = ui8AdecClockSrc;

	//Save current SCART bypass status
	gAadBypassEnabled_M14A0 = bAadBypassEnabled;

	//Save current SPDIF Output
	gAdecSpdifOutput_M14A0 = spdifOutput;

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetClockSource : %d\n", ui8AdecClockSrc);
	return RET_OK;
}

/**
 * Sets a PCM Clock frequency and Rate to sync with SPDIF clock for SPDIF output.
 * @see ADEC_L9_InitModule
 * APLL FCW default value for 12.288MHZ : low 4 bit and high 2 bit is ignored,
 * use 16 bit value and value and freq. is reversed.
 *  Fout = ((2^23 / FCW[21:0]) / (2^M[1:0] * 2^OD[1:0])) * Fin
 *
 *  Fin=27Mhz
 *  FCW[21:0]=288000, M[1:0]=2,  OD[1:0]=4
 *  Fout = ((2^23 / 288000) / (2^2 * 2^4)) * 27,000,000 = 12.288MHz  이고

 *  FCW[21:0]=329088 인 경우는 10.754MHz
 *  FCW[21:0]=256000 인 경우는 13.824MHz
*/
int M14A0_MIXED_IP_ADEC_SetPcmClockRate ( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, UINT32 ui32PcmClock )
{
	UINT32	ui32FCWValue = 0;

	//Check a previous status.
	if(	g_AudKdrv.bInitDone == TRUE && gAdecSrcFreq_M14A0 == ui32SrcFreq && gAdecPcmClock_M14A0 == ui32PcmClock)
	{
		AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetPcmClockRate : same freq = %d, rate = %d\n", ui32SrcFreq, ui32PcmClock);
		return RET_OK;
	}

	//Change PCM clock to Audio PLL FCW value
	if(ui32PcmClock >= APLL_DSP_VALUE_12_288MHZ)
	{
		ui32FCWValue = APLL_CLK_FCW_VALUE_12_288MHZ - ((ui32PcmClock - APLL_DSP_VALUE_12_288MHZ) << 6);		//0x40(64)
	}
	else
	{
		ui32FCWValue = APLL_CLK_FCW_VALUE_12_288MHZ + ((APLL_DSP_VALUE_12_288MHZ - ui32PcmClock) << 6);		//0x40(64)
	}

	//Check a PCM clock rate for min and max value.
	if(ui32FCWValue > APLL_CLK_FCW_VALUE_10_754MHZ)
	{
		AUD_KDRV_ERROR("SetPcmClockRate : freq = %dHz, MIN rate = %d, fcw = %d\n", ui32SrcFreq, ui32PcmClock, ui32FCWValue);

		ui32FCWValue = APLL_CLK_FCW_VALUE_10_754MHZ;
	}
	else if(ui32FCWValue < APLL_CLK_FCW_VALUE_13_824MHZ)
	{
		AUD_KDRV_ERROR("SetPcmClockRate : freq = %dHz, MAX rate = %d, fcw = %d\n", ui32SrcFreq, ui32PcmClock, ui32FCWValue);

		ui32FCWValue = APLL_CLK_FCW_VALUE_13_824MHZ;
	}

	//Read a PCM Clock registers for audio PLL. Change apll_fcw_0 first(우 영신.2012.05.11)
	ACE_REG_M14A0_RdFL(audio_pll_3);
	ACE_REG_M14A0_RdFL(audio_pll_2);
	ACE_REG_M14A0_RdFL(audio_pll_1);

	//Set a FCW value.
	ACE_REG_M14A0_Wr01(audio_pll_3, apll_fcw_0, (ui32FCWValue >>  0) & 0xFF);
	ACE_REG_M14A0_Wr01(audio_pll_2, apll_fcw_1, (ui32FCWValue >>  8) & 0xFF);
	ACE_REG_M14A0_Wr01(audio_pll_1, apll_fcw_2, (ui32FCWValue >> 16) & 0x07);	//upper 3 bit is zero padding.

	//Write a APLL PCM Clock Value.
	ACE_REG_M14A0_WrFL(audio_pll_3);
	ACE_REG_M14A0_WrFL(audio_pll_2);
	ACE_REG_M14A0_WrFL(audio_pll_1);

	//Save current SRC frequency
	gAdecSrcFreq_M14A0  = ui32SrcFreq;

	//Save current PCM Clock Rate
	gAdecPcmClock_M14A0 = ui32PcmClock;

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetPcmClockRate : freq = %dHz, rate = %d, fcw = %d\n", ui32SrcFreq, ui32PcmClock, ui32FCWValue);
	//AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_SetPcmClockRate : freq = %dHz, rate = %d, fcw = %d(0x%X)\n", ui32SrcFreq, ui32PcmClock, ui32FCWValue, ui32FCWValue);
	return RET_OK;
}

int M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq ( LX_AUD_SAMPLING_FREQ_T ui32SamplingFreq, LX_AUD_SPDIF_DTO_RATE_T stSpdifDtoRate, AUD_DTO_T dto )
{
	UINT8	freqTableNum = 0;
	UINT8	rateTableNum = 0;

	DtoCtrlCmdSetDtorate	setDtoRate;

	if(dto >= AUD_DTO_MAX)
	{
		AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq : dto(%d) is not valid.\n", dto);
		return RET_OK;
	}

	//Check a previous status.
	if(	g_AudKdrv.bInitDone == TRUE && gAdecSpdifFreq_M14A0[dto] == ui32SamplingFreq && gAdecSpdifDtoRate_M14A0[dto] == stSpdifDtoRate)
	{
		AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq : same freq = %d_KHZ, rate = %d\n",  ui32SamplingFreq, stSpdifDtoRate);
		return RET_OK;
	}

	//Read a SPDIF DTO registers.
	if(dto == AUD_DTO_D)
	{
		CTOP_CTRL_M14A0_RdFL(ctr67);
		CTOP_CTRL_M14A0_RdFL(ctr68);
		CTOP_CTRL_M14A0_RdFL(ctr66);
	}
	else
	{
		ACE_REG_M14A0_RdFL(audio_dto_0);
		ACE_REG_M14A0_RdFL(audio_dto_1);
		ACE_REG_M14A0_RdFL(audio_dto_2);
		ACE_REG_M14A0_RdFL(audio_dto_3);
		ACE_REG_M14A0_RdFL(audio_dto_4);
		ACE_REG_M14A0_RdFL(audio_dto_5);
		ACE_REG_M14A0_RdFL(audio_dto_6);
		ACE_REG_M14A0_RdFL(audio_dto_7);
		ACE_REG_M14A0_RdFL(audio_dto_8);
	}

	if( ui32SamplingFreq == LX_AUD_SAMPLING_FREQ_48_KHZ )
	{
		AUD_KDRV_PRINT("48_KHZ\n" );

		//Set a SPDIF DTO frequency table.
		freqTableNum = SPDIF_ES_OUT_FREQ_48KHZ;
	}
	else if( ui32SamplingFreq == LX_AUD_SAMPLING_FREQ_44_1KHZ )
	{
		AUD_KDRV_PRINT("44_1KHZ\n" );

		//Set a SPDIF DTO frequency table.
		freqTableNum = SPDIF_ES_OUT_FREQ_44_1KHZ;
	}
	else if( ui32SamplingFreq == LX_AUD_SAMPLING_FREQ_32_KHZ )
	{
		AUD_KDRV_PRINT("32_KHZ\n" );

		//Set a SPDIF DTO frequency table.
		freqTableNum = SPDIF_ES_OUT_FREQ_32KHZ;
	}
	else
	{
		AUD_KDRV_ERROR("SetSPDIFSamplingFreq : freq = %d\n", ui32SamplingFreq);

		//Set a SPDIF DTO frequency table.
		freqTableNum = SPDIF_ES_OUT_FREQ_48KHZ;
	}

	/* Find a DTO_A New Table Value. */
	stSpdifDtoRate = stSpdifDtoRate - DTO_A_TABLE_DIFF_VALUE;

	//Set a SPDIF DTO rate table and Rate for IPC.
	if(stSpdifDtoRate <= LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES)
	{
		AUD_KDRV_ERROR("SetSPDIFSamplingFreq : freq = %dHz, MIN rate = %d\n", ui32SamplingFreq, stSpdifDtoRate);

		rateTableNum = LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES - 1;	//LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES
		stSpdifDtoRate = LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES;
	}
	else if( stSpdifDtoRate >= LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES)
	{
		AUD_KDRV_ERROR("SetSPDIFSamplingFreq : freq = %dHz, MAX rate = %d\n", ui32SamplingFreq, stSpdifDtoRate);

		rateTableNum = LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES - 1;	//LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES
		stSpdifDtoRate = LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES;
	}
	else
	{
		rateTableNum = (UINT8)stSpdifDtoRate - 1;	//Rate table starts 0 value.
	}

	//Set a SPDIF DTO Rate
	if(dto == AUD_DTO_D)
	{
		if( (freqTableNum < SPDIF_ES_OUT_FREQ_NUMBER) && (rateTableNum < LX_AUD_SPDIF_DTO_RATE_NUMBER) )
		{
			CTOP_CTRL_M14A0_Wr01(ctr66, adto_loop_value, g_AUD_SpdifDtoDTable[freqTableNum][rateTableNum][0]);
			CTOP_CTRL_M14A0_Wr01(ctr67, adto_add_value,  g_AUD_SpdifDtoDTable[freqTableNum][rateTableNum][1]);
			CTOP_CTRL_M14A0_Wr01(ctr68, adto_err_value,  g_AUD_SpdifDtoDTable[freqTableNum][rateTableNum][2]);
		}
	}
	else
	{
		if( (freqTableNum < SPDIF_ES_OUT_FREQ_NUMBER) && (rateTableNum < LX_AUD_SPDIF_DTO_RATE_NUMBER) )
		{
			ACE_REG_M14A0_Wr01(audio_dto_0, reg_adto_loop_value_2, g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][0]);
			ACE_REG_M14A0_Wr01(audio_dto_1, reg_adto_loop_value_1, g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][1]);
			ACE_REG_M14A0_Wr01(audio_dto_2, reg_adto_loop_value_0, g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][2]);
			ACE_REG_M14A0_Wr01(audio_dto_3, reg_adto_add_value_3,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][3]);
			ACE_REG_M14A0_Wr01(audio_dto_4, reg_adto_add_value_2,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][4]);
			ACE_REG_M14A0_Wr01(audio_dto_5, reg_adto_add_value_1,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][5]);
			ACE_REG_M14A0_Wr01(audio_dto_6, reg_adto_add_value_0,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][6]);
			ACE_REG_M14A0_Wr01(audio_dto_7, reg_adto_err_value_1,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][7]);
			ACE_REG_M14A0_Wr01(audio_dto_8, reg_adto_err_value_0,  g_AUD_SpdifDtoATable[freqTableNum][rateTableNum][8]);
		}
	}

	//Updating D-DTO clock for CTOP Control.
	if(dto == AUD_DTO_D)
	{
		//Update add / error value register : 2012.01.11 by joonil.lee request
		CTOP_CTRL_M14A0_WrFL(ctr67);
		CTOP_CTRL_M14A0_WrFL(ctr68);

		//Update loop value register in last : 2012.01.11 by joonil.lee request
		CTOP_CTRL_M14A0_WrFL(ctr66);
	}
	else
	{
		if( (freqTableNum < SPDIF_ES_OUT_FREQ_NUMBER) && (rateTableNum < LX_AUD_SPDIF_DTO_RATE_NUMBER) )
		{
			// For Sync Write Mode
			ACE_REG_M14A0_RdFL(top_control_2);
			ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 1);
			ACE_REG_M14A0_WrFL(top_control_2);

			//Update add / error value register : 2012.01.11 by joonil.lee request
			ACE_REG_M14A0_WrFL(audio_dto_3);
			ACE_REG_M14A0_WrFL(audio_dto_4);
			ACE_REG_M14A0_WrFL(audio_dto_5);
			ACE_REG_M14A0_WrFL(audio_dto_6);
			ACE_REG_M14A0_WrFL(audio_dto_7);
			ACE_REG_M14A0_WrFL(audio_dto_8);

			//Update loop value register in last : 2012.01.11 by joonil.lee request
			ACE_REG_M14A0_WrFL(audio_dto_0);
			ACE_REG_M14A0_WrFL(audio_dto_1);
			ACE_REG_M14A0_WrFL(audio_dto_2);

			// For Sync Write Mode
			ACE_REG_M14A0_RdFL(top_control_2);
			ACE_REG_M14A0_Wr01(top_control_2, srw_wr_force_sync, 1);
			ACE_REG_M14A0_WrFL(top_control_2);

			ACE_REG_M14A0_RdFL(top_control_2);
			ACE_REG_M14A0_Wr01(top_control_2, srw_wr_mode, 0);
			ACE_REG_M14A0_WrFL(top_control_2);
		}
	}

	//Send a IMC command for DTO setting
	if(g_AudKdrv.bInitDone == TRUE)
	{
		/* Set a DTO_A New Table Value. */
		stSpdifDtoRate = stSpdifDtoRate + DTO_A_TABLE_DIFF_VALUE;

		setDtoRate.dtorate 		   = (UINT32)stSpdifDtoRate;
		setDtoRate.force_cnt_clear = (UINT32)0;
		AUDIO_IMC_SendCmdParam(DTOCTRL_CMD_SET_DTORATE, ADEC_MODULE_DTO_0, sizeof(DtoCtrlCmdSetDtorate), &setDtoRate);

		//Save current SPDIF sampling frequency
		gAdecSpdifFreq_M14A0[dto]    = ui32SamplingFreq;

		//Save current SPDIF DTO Rate
		gAdecSpdifDtoRate_M14A0[dto] = stSpdifDtoRate;

		MIXED_IP_AUDIO_SetDTORate(g_setClockSrc, ui32SamplingFreq, dto, stSpdifDtoRate);
	}

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq : dto = %d, freq = %dHz, rate = %d\n", dto, ui32SamplingFreq, stSpdifDtoRate);
	//AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq : dto = %d, freq = %dHz, rate = %d\n", dto, ui32SamplingFreq, stSpdifDtoRate);	return RET_OK;
	return RET_OK;
}

/**
 * Initializes the Mixed IP ADC, MIC ADC, DAC and HP module register value.
 * @see ADEC_DRV_InitModule
*/
int M14A0_MIXED_IP_ADEC_InitCodecRegisters ( void )
{
	//Setting Mixed IP
	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_InitCodecRegisters : Start\n");

	M14A0_ANALOG_WriteRegister(0x1D, 0x00, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x01, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x03, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x04, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x05, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x0B, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x0E, 0x00);		//DAC0 Volume -125dB
	M14A0_ANALOG_WriteRegister(0x1D, 0x0F, 0x13);		//0x13(SCART DTV) => default value workaround, 2012.05.02
	M14A0_ANALOG_WriteRegister(0x1D, 0x11, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x13, 0x01);		//DAC0 mute rate control, 2013.02.04(0x01 : 30ms)

	//////////////////////////
	//M14A0_ANALOG_WriteRegister(0x1D, 0x14, 0x01);		//Enable DAC0 by-pass, 2011.06.13
	M14A0_ANALOG_WriteRegister(0x1D, 0x14, 0x09);		//Enable DAC0 DC Set-up, 2012.09.17(H13 only)

	M14A0_ANALOG_WriteRegister(0x1D, 0x16, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x17, 0x00);		//0x00, HP(DAC1 Volume) 0dB
	M14A0_ANALOG_WriteRegister(0x1D, 0x18, 0x00);		//0x00, HP(DAC1 Volume) 0dB
	M14A0_ANALOG_WriteRegister(0x1D, 0x1A, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x1C, 0x01);		//DAC1 mute rate control, 2013. 02.04(0x01 : 30 ms)

	//////////////////////////
	//M14A0_ANALOG_WriteRegister(0x1D, 0x1D, 0x01);		//Enable DAC1 by-pass, 2011.06.13
	M14A0_ANALOG_WriteRegister(0x1D, 0x1D, 0x09);		//Enable DAC1 DC Set-up, 2012.09.17(H13 only)

	M14A0_ANALOG_WriteRegister(0x1D, 0x1F, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x20, 0x40);		//DAC0/1 I2S settimg : Normal(0x40)
	M14A0_ANALOG_WriteRegister(0x1D, 0x2A, 0x02); 		//bug fixed, change with dsp setting, H13 ACE, ADC I2S falling(0x02) for I2S output, 2012.06.08

	M14A0_ANALOG_WriteRegister(0x1D, 0x2B, 0x00);

	//Workaround for 0x2D register not to update issue : 2012.10.25
//	M14A0_ANALOG_WriteRegister(0x1D, 0x2C, 0x0C);		//Amplify ADC digital gain : +3.00 dB, 2012.06.08
	M14A0_ANALOG_WriteRegister(0x1D, 0x2D, 0x0B);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1A->0x0B)

	//Amplify ADC digital gain : +3.00 dB, 2012.10.25 => Workaround for 0x2C register update issue
	M14A0_ANALOG_WriteRegister(0x1D, 0x2C, 0x0D);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1D -> 0x0D)
	M14A0_ANALOG_WriteRegister(0x1D, 0x2C, 0x0C);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1C -> 0x0C)

	M14A0_ANALOG_WriteRegister(0x1D, 0x2F, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x31, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x33, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x37, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x39, 0x00);
	M14A0_ANALOG_WriteRegister(0x1D, 0x3A, 0x00);

	//This register is added in H13 A0 chip for power down control
	M14A0_ANALOG_WriteRegister(0x1D, 0x48, 0x03);		//ADC power-down : Normal
	M14A0_ANALOG_WriteRegister(0x1D, 0x49, 0xFF);		//DAC power-down : Normal

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_InitCodecRegisters : Done!!!\n");
	return RET_OK;
}

/**
 * Sets the Mixed IP AAD SIF Input Port.
 * @see .
*/
int M14A0_MIXED_IP_ADEC_SetAADSIFInputPort ( LX_AUD_SIF_INPUT_T sifSource )
{
	/* Set a SIF Input Source for A_DEMOD & AAD Module. */
	if(sifSource == LX_AUD_SIF_INPUT_INTERNAL)
	{
		ACE_REG_M14A0_RdFL(afe_aad_14);
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 	 0);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 1);	// SPLL power down (0: power down, 1: power on)
		ACE_REG_M14A0_WrFL(afe_aad_14);

		ACE_REG_M14A0_RdFL(abb_3);
		ACE_REG_M14A0_Wr01(abb_3, abb_mux_sel_aad, 1);			// AAD_SEL(0 : RC Path, 1: ADC Path)
		ACE_REG_M14A0_WrFL(abb_3);
	}
	else
	{
		ACE_REG_M14A0_RdFL(afe_aad_14);
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spllclk_test, 0);
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_sel, 	 1);	// AAD_ADC_SEL_SIF(0 : ABB(Internal), 1: GBB(External))
		ACE_REG_M14A0_Wr01(afe_aad_14, aad_spll_pdb, 	 1);	// SPLL power down (0: power down, 1: power on)
		ACE_REG_M14A0_WrFL(afe_aad_14);

		ACE_REG_M14A0_RdFL(abb_3);
		ACE_REG_M14A0_Wr01(abb_3, abb_mux_sel_aad, 1);			// AAD_SEL(0 : RC Path, 1: ADC Path)
		ACE_REG_M14A0_WrFL(abb_3);
	}

	//Save current SIF Input
	gSifSource = sifSource;

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetAADSIFInputPort : sifSource = %d!!!\n", sifSource);
	return RET_OK;
}

/**
 * Start the Mixed IP ADC, MIC ADC, DAC and HP module to mute off.(only upper L8 B0 Chip)
 * @see ADEC_DRV_InitModule
*/
int M14A0_MIXED_IP_ADEC_StartCodec ( void )
{
	//Setting Mixed IP
	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_StartCodec : Start\n");

	M14A0_ANALOG_WriteRegister(0x1D, 0x00, 0x7C);
	M14A0_ANALOG_WriteRegister(0x1D, 0x01, 0x3A);	//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x32->0x3A)

	//Hum noise issue after DV-2nd Board
//	M14A0_ANALOG_WriteRegister(0x1D, 0x02, 0x1B);

	//Hum noise issue to DV-2nd Board(2011.10.24)
//	M14A0_ANALOG_WriteRegister(0x1D, 0x02, 0x40);
	M14A0_ANALOG_WriteRegister(0x1D, 0x02, 0x00);	//Amplify ADC digital gain : 1.00 dB, 2011.11.08

	M14A0_ANALOG_WriteRegister(0x1D, 0x03, 0x7B);
	M14A0_ANALOG_WriteRegister(0x1D, 0x10, 0x02);
	M14A0_ANALOG_WriteRegister(0x1D, 0x16, 0x02);
	M14A0_ANALOG_WriteRegister(0x1D, 0x2B, 0x40);
	M14A0_ANALOG_WriteRegister(0x1D, 0x2F, 0x0F);	//to supress pop-noise. ADC volume rate control, 2011.12.29
	M14A0_ANALOG_WriteRegister(0x1D, 0x31, 0x01);	//to supress pop-noise. ADC mute rate control, 2013.02.04

	M14A0_ANALOG_WriteRegister(0x1D, 0x14, 0x0D);	//DAC0 Mute on, SCART, Enable DAC0 DC Set-up, 2013.10.09
//	M14A0_ANALOG_WriteRegister(0x1D, 0x1D, 0x05);	//DAC1 Mute on, HPDRV
	M14A0_ANALOG_WriteRegister(0x1D, 0x32, 0x04);	//ADC Mute on

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_StartCodec : Done\n");
	return RET_OK;
}

/**
 * Stop the Mixed IP ADC, MIC ADC, DAC and HP module to mute off.(only upper L8 B0 Chip)
 * @see ADEC_DRV_InitModule
*/
int M14A0_MIXED_IP_ADEC_StopCodec ( void )
{

	AUD_KDRV_DEBUG_TMP("M14A0_MIXED_IP_ADEC_StopCodec : Not Implemented!!!\n");
	return RET_OK;
}

/**
 * Sets a ADC port number for ADEC I2S input.
 * @see ADEC_DRV_SetSource
*/
int M14A0_MIXED_IP_ADEC_SetADCPortNumber ( UINT8 ui8PortNum )
{
	if(ui8PortNum < 4)
	{
		ACE_REG_M14A0_RdFL(acodec_50);
		ACE_REG_M14A0_Wr01(acodec_50, ladc_mute_ena, 1);
		ACE_REG_M14A0_WrFL(acodec_50);

		/* Wait for pop-noise supression */
		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(soft_reset_1);
		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auad, 1);
		ACE_REG_M14A0_WrFL(soft_reset_1);

		udelay(MIXEDIP_RESET_DELAY_100US);

		/* Set a ADC port number */
		ACE_REG_M14A0_RdFL(afe_acodec_1);
		//3A_01_18:ADC ch1='floating' 19:ADC ch2=AV2  1A:ADC ch3=AV1  1B:ADC ch4=COMP2 1C:ADC ch5=PCIN
		ACE_REG_M14A0_Wr01(afe_acodec_1, auad_gcon, 0x7);	//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x6->0x7)
		ACE_REG_M14A0_Wr01(afe_acodec_1, auad_ch_sel, ui8PortNum);
		ACE_REG_M14A0_WrFL(afe_acodec_1);

		//workaround for not to update 0x2D register after IP reset : 2012.10.25
		M14A0_ANALOG_WriteRegister(0x1D, 0x2D, 0x0B);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1A->0x0B)

		//workaround for not to mute off command : 2013.02.02
		M14A0_ANALOG_WriteRegister(0x1D, 0x2C, 0x0D);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1D->0x0D)

		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(soft_reset_1);
		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auad, 0);
		ACE_REG_M14A0_WrFL(soft_reset_1);

		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(acodec_50);
		ACE_REG_M14A0_Wr01(acodec_50, ladc_mute_ena, 0);
		ACE_REG_M14A0_WrFL(acodec_50);

		/* To protect mute off timing : 2013.02.04 */
		OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);	//30ms

		//workaround for not to mute off command : 2013.02.02
		M14A0_ANALOG_WriteRegister(0x1D, 0x2C, 0x0C);		//Amplify ADC Gain(-0.062 ~ -0.092dBFS) : 2013.08.27(0x1C->0x0C)
	}
	else
	{
		AUD_KDRV_ERROR("M14A0_MIXED_IP_ADEC_SetADCPortNumber : ADC Port = %d(Not Supported.)\n", ui8PortNum);
		return RET_ERROR;
	}

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetADCPortNumber : ADC Port = %d\n", ui8PortNum);
	return RET_OK;
}

/**
 * Sets the volume of DAC output.
 * @see ADEC_DRV_SetDACOutVolume
*/
int M14A0_MIXED_IP_ADEC_SetDACOutVolume ( LX_AUD_DAC_VOL_T stDACOutVol )
{
	UINT8	dac0MuteEnable = 0;
	UINT16	dac0_vol_con_1 = 0, dac1_vol_con_1 = 0;

	//L9 B0 Support Function : DAC0 -> SCART, DAC1 -> HPDRV
	//Set a SCART DAC volume for NICAM : Boost
	if ( stDACOutVol.mode & LX_AUD_DAC_SCART )
	{
		//Save gScartBoostSetted
		gScartBoostGain_M14A0 = stDACOutVol.scartGain;

		//Check a current mute status
		ACE_REG_M14A0_RdFL(acodec_20);
		ACE_REG_M14A0_Rd01(acodec_20, dac0_mute_ena, dac0MuteEnable);
		if(dac0MuteEnable == 0)
		{
			ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 1);		// 1 bit : Line DAC CH0 Mute Control
			ACE_REG_M14A0_WrFL(acodec_20);

			/* To protect pop-noise */
			OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);	//30ms
		}

		ACE_REG_M14A0_RdFL(acodec_14);
		ACE_REG_M14A0_Rd01(acodec_14, dac0_vol_con_1, dac0_vol_con_1);
		dac0_vol_con_1 &= 0x03;							// 2 bit : Line DAC CH0 Volume Control
		dac0_vol_con_1 |= (UINT8)(stDACOutVol.scartGain << 2);	// 4 bit : Line DAC CH0 Volume Control
		ACE_REG_M14A0_Wr01(acodec_14, dac0_vol_con_1, dac0_vol_con_1);
		ACE_REG_M14A0_WrFL(acodec_14);

		if(dac0MuteEnable == 0)
		{
			ACE_REG_M14A0_RdFL(acodec_20);
			ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 0);	// 1 bit : Line DAC CH0 Mute Control
			ACE_REG_M14A0_WrFL(acodec_20);
		}

		AUD_KDRV_MIP_DEBUG("SetDACOutVolume : SCART Gain = %d(dac0 = 0x%X)\n", stDACOutVol.scartGain, stDACOutVol.scartGain);

#ifdef ENABLE_MUTE_DEBUG
		AUD_KDRV_ERROR("SCART Gain = %d\n", gScartBoostGain_M14A0);
#endif

		return RET_OK;
	}

	//Set a SCART DAC volume : Attunuation
	if ( stDACOutVol.portNum & LX_AUD_OUTPUT_DAC0 )
	{
		//Check Mute case if volume is 0x3FF
		if(stDACOutVol.hwVolume == 0x3FF)
		{
			ACE_REG_M14A0_RdFL(acodec_15);
			ACE_REG_M14A0_Wr01(acodec_15, dac0_vol_con_0, (UINT8)(stDACOutVol.hwVolume & 0xFF));	// 8 bit : Line DAC CH0 Volume Control
			ACE_REG_M14A0_WrFL(acodec_15);

			ACE_REG_M14A0_RdFL(acodec_14);
			dac0_vol_con_1  = (UINT8)((stDACOutVol.hwVolume >> 8) & 0x03);// 2 bit : Line DAC CH0 Volume Control
			dac0_vol_con_1 |= (gScartBoostGain_M14A0 << 2);	// 4 bit : Line DAC CH0 Volume Control
			ACE_REG_M14A0_Wr01(acodec_14, dac0_vol_con_1, dac0_vol_con_1);
			ACE_REG_M14A0_WrFL(acodec_14);
		}
		else
		{
			ACE_REG_M14A0_RdFL(acodec_15);
			ACE_REG_M14A0_Wr01(acodec_15, dac0_vol_con_0, (UINT8)(stDACOutVol.hwVolume & 0xFF));	// 8 bit : Line DAC CH0 Volume Control
			ACE_REG_M14A0_WrFL(acodec_15);

			//Check gScartBoostGain_M14A0 value
			if(gScartBoostGain_M14A0)
			{
				ACE_REG_M14A0_RdFL(acodec_14);
				dac0_vol_con_1  = (UINT8)((stDACOutVol.hwVolume >> 8) & 0x03);// 2 bit : Line DAC CH0 Volume Control
				dac0_vol_con_1 |= (gScartBoostGain_M14A0 << 2);	// 4 bit : Line DAC CH0 Volume Control
				ACE_REG_M14A0_Wr01(acodec_14, dac0_vol_con_1, dac0_vol_con_1);
				ACE_REG_M14A0_WrFL(acodec_14);
			}
			else
			{
				ACE_REG_M14A0_RdFL(acodec_14);
				dac0_vol_con_1  = (UINT8)((stDACOutVol.hwVolume >> 8) & 0x03);// 2 bit : Line DAC CH0 Volume Control
				ACE_REG_M14A0_Wr01(acodec_14, dac0_vol_con_1, dac0_vol_con_1);
				ACE_REG_M14A0_WrFL(acodec_14);
			}
		}

		AUD_KDRV_MIP_DEBUG("SetDACOutVolume : SCART(dac0_0 = 0x%X, dac0_1 = 0x%X)\n", stDACOutVol.hwVolume, stDACOutVol.hwVolume >> 8);
	}

	//Set a HeadPhone DAC volume
	if ( stDACOutVol.portNum & LX_AUD_OUTPUT_DAC2 )
	{
		ACE_REG_M14A0_RdFL(acodec_24);
		ACE_REG_M14A0_Wr01(acodec_24, dac1_vol_con_0, (UINT8)(stDACOutVol.hwVolume & 0xFF));	// 8 bit : Line DAC CH1 Volume Control
		ACE_REG_M14A0_WrFL(acodec_24);

		ACE_REG_M14A0_RdFL(acodec_23);
		dac1_vol_con_1 = (UINT8)((stDACOutVol.hwVolume >> 8) & 0x03);// 2 bit : Line DAC CH1 Volume Control
		ACE_REG_M14A0_Wr01(acodec_23, dac1_vol_con_1, dac1_vol_con_1);
		ACE_REG_M14A0_WrFL(acodec_23);

		AUD_KDRV_MIP_DEBUG("SetDACOutVolume : HPDRV(dac1_0 = 0x%X, dac1_1 = 0x%X)\n", stDACOutVol.hwVolume, stDACOutVol.hwVolume >> 8);
	}

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetDACOutVolume : Port = %d, Volume = 0x%X\n", stDACOutVol.portNum, stDACOutVol.hwVolume);
	return RET_OK;
}

/**
 * Sets the mute of DAC output.
 * @see ADEC_DRV_SetOutputPortMute
*/
int M14A0_MIXED_IP_ADEC_SetDACOutMute ( LX_AUD_OUTPUT_PORT_MUTE_T stOutPortMute )
{
	//H13 Support Function : DAC0 -> SCART, DAC1 -> HPDRV

	//Set a SCART DAC Reset
	if ( stOutPortMute.mode & LX_AUD_DAC_SCART_RESET )
	{
		ACE_REG_M14A0_RdFL(soft_reset_14);
		ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 1);	//audio codec : dac 0
		ACE_REG_M14A0_WrFL(soft_reset_14);

		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(soft_reset_1);
		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 1);	//audio codec : dac 0
		ACE_REG_M14A0_WrFL(soft_reset_1);

		//add delay for clock setting
		udelay(MIXEDIP_RESET_DELAY_100US * 2);

		ACE_REG_M14A0_RdFL(soft_reset_1);
		ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk0, 0);	//audio codec : dac 0,
		ACE_REG_M14A0_WrFL(soft_reset_1);

		udelay(MIXEDIP_RESET_DELAY_100US);

		ACE_REG_M14A0_RdFL(soft_reset_14);
		ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk0, 0);	//audio codec : dac 0
		ACE_REG_M14A0_WrFL(soft_reset_14);

		AUD_KDRV_MIP_DEBUG("SCART : Reset\n");

		return RET_OK;
	}

	//Set a SCART DAC mute
	if ( stOutPortMute.portNum & LX_AUD_OUTPUT_DAC0 )
	{
		if( stOutPortMute.bHwOnOff == TRUE )
		{
			ACE_REG_M14A0_RdFL(acodec_20);
			ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 1);		// 1 bit : Line DAC CH0 Mute Control
			ACE_REG_M14A0_WrFL(acodec_20);

			AUD_KDRV_MIP_DEBUG("SCART : Mute On\n");

#ifdef ENABLE_MUTE_DEBUG
			AUD_KDRV_ERROR("SCART : Mute On\n");
#endif
		}
		else	//un-mute
		{
			ACE_REG_M14A0_RdFL(acodec_20);
			ACE_REG_M14A0_Wr01(acodec_20, dac0_mute_ena, 0);		// 1 bit : Line DAC CH0 Mute Control
			ACE_REG_M14A0_WrFL(acodec_20);

			AUD_KDRV_MIP_DEBUG("SCART : Mute Off\n");

#ifdef ENABLE_MUTE_DEBUG
			AUD_KDRV_ERROR("SCART : Mute Off\n");
#endif
		}
	}

	//Set a HeadPhone DAC mute : *** Not Used in L9 B0 ***
	if ( stOutPortMute.portNum & LX_AUD_OUTPUT_DAC2 )
	{
		//mute
		if( stOutPortMute.bHwOnOff == TRUE )
		{
			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 1);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);

			OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);

			ACE_REG_M14A0_RdFL(soft_reset_14);
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 1);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_14);

			ACE_REG_M14A0_RdFL(soft_reset_1);
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 1);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_1);

			//add delay for clock setting
			udelay(MIXEDIP_RESET_DELAY_100US * 2);

			AUD_KDRV_MIP_DEBUG("HPDRV : Mute On\n");

#ifdef ENABLE_MUTE_DEBUG
			AUD_KDRV_ERROR("HPDRV : Mute On\n");
#endif
		}
		else	//un-mute
		{
			ACE_REG_M14A0_RdFL(soft_reset_1);
			ACE_REG_M14A0_Wr01(soft_reset_1, swrst_auda_mclk1, 0);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_1);

			ACE_REG_M14A0_RdFL(soft_reset_14);
			ACE_REG_M14A0_Wr01(soft_reset_14, swrst_auda_f256fs_clk1, 0);	//audio codec : dac 1
			ACE_REG_M14A0_WrFL(soft_reset_14);

			OS_MsecSleep(MIXEDIP_MUTE_DELAY_30MS);

			ACE_REG_M14A0_RdFL(acodec_29);
			ACE_REG_M14A0_Wr01(acodec_29, dac1_mute_ena, 0);	// 1 bit : Line DAC CH1 Mute Control
			ACE_REG_M14A0_WrFL(acodec_29);

			AUD_KDRV_MIP_DEBUG("HPDRV : Mute Off\n");

#ifdef ENABLE_MUTE_DEBUG
			AUD_KDRV_ERROR("HPDRV : Mute Off\n");
#endif
		}
	}

	AUD_KDRV_MIP_DEBUG("M14A0_MIXED_IP_ADEC_SetDACOutMute : Port = %d, mute = 0x%X\n", stOutPortMute.portNum, stOutPortMute.bHwOnOff);
	return RET_OK;
}

/** @} */



