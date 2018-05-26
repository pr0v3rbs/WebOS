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
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.05.03
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "audio_kapi_master.h"
#include "audio_cfg.h"
#include "audio_drv.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
AUDIO Register configuration.
*/
LX_AUD_REG_CFG_T g_stAudRegCfg[] =
{
	// for LG1154(H13)
	{
		.chip_name			= "LG1154(H13)",
		.reg_base_addr		= H13_AUD_BASE,
		.reg_size			= 0x00000700,	//0x700
		.irq0_num			= H13_IRQ_AUD0,
		.irq1_num			= H13_IRQ_AUD1,
	},
	// for LG1311(M14 Ax)
	{
		.chip_name			= "LG1311(M14 Ax)",
		.reg_base_addr		= M14_A0_AUD_BASE,
		.reg_size			= 0x00000700,	//0x700
		.irq0_num			= M14_A0_IRQ_AUD0,
		.irq1_num			= M14_A0_IRQ_AUD1,
	},
	// for LG1311(M14 Bx)
	{
		.chip_name			= "LG1311(M14 Bx)",
		.reg_base_addr		= M14_B0_AUD_BASE,
		.reg_size			= 0x00000700,	//0x700
		.irq0_num			= M14_B0_IRQ_AUD0,
		.irq1_num			= M14_B0_IRQ_AUD1,
	},
	// for LG1156(H14)
	{
		.chip_name			= "LG1156(H14)",
		.reg_base_addr		= H14_AUD_BASE,
		.reg_size			= 0x00000700,	//0x700
		.irq0_num			= H14_IRQ_AUD0,
		.irq1_num			= H14_IRQ_AUD1,
	},
	// for LG1210(H15)
	{
		.chip_name			= "LG1210(H15)",
		.reg_base_addr		= H15_AUD_BASE,
		.reg_size			= 0x00000700,	//0x700
		.irq0_num			= H15_IRQ_AUD0,
		.irq1_num			= H15_IRQ_AUD1,
	},
};

/**
AUDIO Memory Map configuration.
*/
LX_AUD_MEM_CFG_S_T g_stMemCfgAud[] =
{
	// for LG1210(H15)
	{
		.dsp0_memory_name 		= "aud_dsp0",		/* dsp0 running memory */
		.dsp0_memory_base 		= 0x00000000,
		.dsp0_memory_size 		= 0x00B00000,		/* 11.0 MB for f/w download */

		.dsp1_memory_name 		= "aud_dsp1",		/* dsp1 running memory */
		.dsp1_memory_base 		= 0x00000000,
		.dsp1_memory_size 		= 0x00700000,		/* 7.0 MB for f/w download */

		.dpb0_memory_name 		= "aud_dpb0",		/* dsp1 decoded PCM buffer 0 memory */
		.dpb0_memory_base 		= 0x00000000,
		.dpb0_memory_size 		= 0x00080000,		/* 0.5 MB  */

		.dpb1_memory_name		= "aud_dpb1",		/* dsp1 decoded PCM buffer 1 memory */
		.dpb1_memory_base		= 0x00000000,
		.dpb1_memory_size		= 0x00080000,		/* 0.5 MB */

		.cpb0_memory_name 		= "aud_cpb0",		/* dsp0 coded PCM buffer 0 memory */
		.cpb0_memory_base 		= 0x00000000,
		.cpb0_memory_size 		= 0x00080000,		/* 0.5 MB */

		.cpb1_memory_name 		= "aud_cpb1",		/* dsp0 coded PCM buffer 1 memory */
		.cpb1_memory_base 		= 0x00000000,
		.cpb1_memory_size 		= 0x00080000,		/* 0.5 MB */

		.mpb0_memory_name 		= "aud_mpb0",		/* dsp1 mixed PCM buffer 0 memory */
		.mpb0_memory_base 		= 0x00000000,
		.mpb0_memory_size		= 0x00080000,		/* 512 KB */

		.mpb1_memory_name 		= "aud_mpb1",		/* dsp1 mixed PCM buffer 1 memory */
		.mpb1_memory_base 		= 0x00000000,
		.mpb1_memory_size		= 0x00080000,		/* 512 KB */

		.mpb2_memory_name 		= "aud_mpb2",		/* dsp1 mixed PCM buffer 2 memory */
		.mpb2_memory_base 		= 0x00000000,
		.mpb2_memory_size 		= 0x00080000,		/* 512 KB */

		.mpb3_memory_name		= "aud_mpb3",		/* dsp1 mixed PCM buffer 3 memory */
		.mpb3_memory_base		= 0x00000000,
		.mpb3_memory_size		= 0x00080000,		/* 512 KB */

		.mpb4_memory_name		= "aud_mpb4",		/* dsp1 mixed PCM buffer 4 memory */
		.mpb4_memory_base		= 0x00000000,
		.mpb4_memory_size		= 0x00080000,		/* 512 KB */

		.mpb5_memory_name		= "aud_mpb5",		/* dsp1 mixed PCM buffer 5 memory */
		.mpb5_memory_base		= 0x00000000,
		.mpb5_memory_size		= 0x00080000,		/* 512 KB */

		.mpb6_memory_name		= "aud_mpb6",		/* dsp1 mixed PCM buffer 6 memory */
		.mpb6_memory_base		= 0x00000000,
		.mpb6_memory_size		= 0x00080000,		/* 512 KB */

		.mpb7_memory_name		= "aud_mpb7",		/* dsp1 mixed PCM buffer 7 memory */
		.mpb7_memory_base		= 0x00000000,
		.mpb7_memory_size		= 0x00080000,		/* 512 KB */

		.iec0_memory_name 		= "aud_iec0",		/* IEC0(SPDIF) buffer memory */
		.iec0_memory_base 		= 0x00000000,
		.iec0_memory_size 		= 0x00080000,		/* 0.5 MB */

		.iec1_memory_name 		= "aud_iec1",		/* IEC1(SPDIF) buffer memory */
		.iec1_memory_base 		= 0x00000000,
		.iec1_memory_size 		= 0x00080000,		/* 0.5 MB */

		.se_memory_name 		= "aud_se",			/* audio sound engine parameter memory */
		.se_memory_base 		= 0x00000000,
		.se_memory_size 		= 0x00020000,		/* 128 KB */

		.enc_memory_name 		= "aud_enc ",		/* encoder buffer memory */
		.enc_memory_base 		= 0x00000000,
		.enc_memory_size 		= 0x00040000,		/* 256 KB */

		.cap0_memory_name 		= "aud_cap0",		/* capture buffer memory */
		.cap0_memory_base 		= 0x00000000,
		.cap0_memory_size 		= 0x00080000,		/* 256 KB */

		.cap1_memory_name 		= "aud_cap1",		/* capture buffer memory */
		.cap1_memory_base 		= 0x00000000,
		.cap1_memory_size 		= 0x00040000,		/* 256 KB */

		.cap2_memory_name 		= "aud_cap2",		/* capture buffer memory */
		.cap2_memory_base 		= 0x00000000,
		.cap2_memory_size 		= 0x00040000,		/* 256 KB */

		.cap3_memory_name 		= "aud_cap3",		/* capture buffer memory */
		.cap3_memory_base 		= 0x00000000,
		.cap3_memory_size 		= 0x00040000,		/* 256 KB */

		.cap4_memory_name 		= "aud_cap4",		/* capture buffer memory */
		.cap4_memory_base 		= 0x00000000,
		.cap4_memory_size 		= 0x00040000,		/* 256 KB */

		.cap5_memory_name 		= "aud_cap5",		/* capture buffer memory */
		.cap5_memory_base 		= 0x00000000,
		.cap5_memory_size 		= 0x00040000,		/* 256 KB */

		.cap6_memory_name 		= "aud_cap6",		/* capture buffer memory */
		.cap6_memory_base 		= 0x00000000,
		.cap6_memory_size 		= 0x00040000,		/* 256 KB */

		.cap7_memory_name 		= "aud_cap7",		/* capture buffer memory */
		.cap7_memory_base 		= 0x00000000,
		.cap7_memory_size 		= 0x00040000,		/* 256 KB */

		.ipc_memory_name 		= "aud_ipc",		/* audio IPC memory */
		.ipc_memory_base 		= 0x00000000,
		.ipc_memory_size 		= 0x000F4000,		/* 976 KB */
	},
};

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
/**
AUDIO Register definition.
*/
LX_AUD_REG_CFG_T *pstAudRegCfg = &g_stAudRegCfg[0];	// for Real. Chip

/**
AUDIO Memory Map definition.
*/
LX_AUD_MEM_CFG_S_T *pstMemCfgAud = &g_stMemCfgAud[0];	// for Real. Chip


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/


