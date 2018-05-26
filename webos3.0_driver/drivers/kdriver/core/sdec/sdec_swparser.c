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
 *  @author	jinhwan.bae
 *  @version	1.0
 *  @date		2012-05-16
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "sdec_impl.h"

#include "sdec_swparser.h"
#include "sdec_io.h"
#include "sdec_pes.h"
#include "sdec_mm.h"
#include "sdec_hal.h"

#include "pvr_kapi.h"
#include "pvr_core.h"
#include "pvr_reg.h"
#include "pvr_cfg.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#define SDEC_SWPARSER_WITH_ARRAYED_DATA		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SWP_DN_PHYS_TO_VIRT(x)	(x + _gVirtOffset)
#define SWP_DN_VIRT_TO_PHYS(x)	(x - _gVirtOffset)
#define SWP_SEC_PHYS_TO_VIRT(x)	(x + _gSecCirVirtOffset)
#define SWP_SEC_VIRT_TO_PHYS(x)	(x - _gSecCirVirtOffset)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

struct task_struct	*stSWTSParser_Thread;
TsFilter	gSWTSFilter[NUM_OF_MAX_CH_C_FILTER];


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _SDEC_SWTSParser_Thread(void *data);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/* for using physical <-> virtual address conversion */
static SINT32 _gVirtOffset = 0;
static UINT32 _gVirtAddr = 0;
static SINT32 _gSecCirVirtOffset = 0;
static UINT32 _gSecCirVirtAddr = 0;


#ifdef SDEC_SWPARSER_WITH_ARRAYED_DATA

UINT8 temp_ERR_MGT[TS_PACKET_SIZE]=
{ 	0x47, 0x40, 0x00, 0x10, 0x00, 0xC7, 0xF0, 0x24, 0x00, 0x00,
	0xC3, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0xFF, 0xFB,
	0xE1, 0x00, 0x00, 0x00, 0x3C, 0xF0, 0x00, 0x03, 0x04, 0xFF,
	0xFB, 0xE1, 0x00, 0x00, 0x00, 0xD7, 0xF0, 0x00, 0xF0, 0x00,
	0x91, 0xA0, 0xAF, 0x6E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


UINT8 temp_SDT1[TS_PACKET_SIZE * 4] =
{	0x47, 0x40, 0x11, 0x1C, 0x00, 0x42, 0xF0, 0xDF, 0x00, 0x01, 0xC1, 0x00,
	0x00, 0x01, 0x10, 0xFF, 0x00, 0x67, 0xFC, 0x80, 0x31, 0x48, 0x15, 0x01,
	0x08, 0x4D, 0x65, 0x64, 0x69, 0x61, 0x73, 0x65, 0x74, 0x0A, 0x41, 0x6C,
	0x74, 0x72, 0x6F, 0x20, 0x46, 0x72, 0x65, 0x65, 0x64, 0x18, 0x00, 0xF0,
	0xC7, 0x10, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x41, 0x00,
	0x00, 0x08, 0x9B, 0xC0, 0x04, 0x68, 0x00, 0x00, 0x00, 0x00, 0x03, 0xEE,
	0xFC, 0x80, 0x98, 0x48, 0x14, 0x01, 0x08, 0x4D, 0x65, 0x64, 0x69, 0x61,
	0x73, 0x65, 0x74, 0x09, 0x49, 0x74, 0x61, 0x6C, 0x69, 0x61, 0x75, 0x6E,
	0x6F, 0x64, 0x18, 0x00, 0xF0, 0x65, 0x10, 0xBF, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x0C, 0xB8, 0xC0, 0x03, 0x1C, 0x00,
	0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x66, 0x10, 0xBF, 0x80, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x12, 0x98, 0xC0, 0x06,
	0x83, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x77, 0x10, 0xBF,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x0C, 0xB8,
	0xC0, 0x02, 0x9F, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x7D,
	0x10, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x47, 0x00, 0x11, 0x1D, 0x05, 0x60, 0x00, 0x00, 0x06, 0xD8, 0xC0, 0x02,
	0x9F, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x7E, 0x10, 0xBF,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x08, 0x50,
	0xC0, 0x02, 0x9F, 0x00, 0x00, 0x00, 0x00, 0xD3, 0x9B, 0xDE, 0x26, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x47, 0x40, 0x11, 0x1E, 0x00, 0x42, 0xF0, 0xDF, 0x00, 0x01, 0xC1, 0x00,
	0x00, 0x01, 0x10, 0xFF, 0x00, 0x67, 0xFC, 0x80, 0x31, 0x48, 0x15, 0x01,
	0x08, 0x4D, 0x65, 0x64, 0x69, 0x61, 0x73, 0x65, 0x74, 0x0A, 0x41, 0x6C,
	0x74, 0x72, 0x6F, 0x20, 0x46, 0x72, 0x65, 0x65, 0x64, 0x18, 0x00, 0xF0,
	0xC7, 0x10, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x41, 0x00,
	0x00, 0x08, 0x9B, 0xC0, 0x04, 0x68, 0x00, 0x00, 0x00, 0x00, 0x03, 0xEE,
	0xFC, 0x80, 0x98, 0x48, 0x14, 0x01, 0x08, 0x4D, 0x65, 0x64, 0x69, 0x61,
	0x73, 0x65, 0x74, 0x09, 0x49, 0x74, 0x61, 0x6C, 0x69, 0x61, 0x75, 0x6E,
	0x6F, 0x64, 0x18, 0x00, 0xF0, 0x65, 0x10, 0xBF, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x0C, 0xB8, 0xC0, 0x03, 0x1C, 0x00,
	0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x66, 0x10, 0xBF, 0x80, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x12, 0x98, 0xC0, 0x06,
	0x83, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x77, 0x10, 0xBF,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x0C, 0xB8,
	0xC0, 0x02, 0x9F, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x7D,
	0x10, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x47, 0x00, 0x11, 0x1F, 0x05, 0x60, 0x00, 0x00, 0x06, 0xD8, 0xC0, 0x02,
	0x9F, 0x00, 0x00, 0x00, 0x00, 0x64, 0x18, 0x00, 0xF0, 0x7E, 0x10, 0xBF,
	0x80, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x05, 0x60, 0x00, 0x00, 0x08, 0x50,	/* insert CRC Error */
	0xC0, 0x02, 0x9F, 0x00, 0x00, 0x00, 0x00, 0xD3, 0x9B, 0xDE, 0x27, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
#endif

static const UINT32 _gCrc32_table[256] = {
	0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
    0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
    0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
    0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
    0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
    0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
    0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
    0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
    0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
    0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
    0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
    0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
    0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
    0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
    0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

/*========================================================================================
	Implementation Group
========================================================================================*/



/**
 * SDEC Software Parser Init
 * Create Kernel Thread to get the data from DVR Buffer / Process TS / Make Section / Register WQ as normal section
 *
 * @param	stpSdecParam		[in	]  S_SDEC_PARAM_T
 * @return	void
 * @see
 */
void SDEC_SWP_Init(S_SDEC_PARAM_T *stpSdecParam)
{
	/* Create SW Section Parser Thread */
	stSWTSParser_Thread = kthread_create( _SDEC_SWTSParser_Thread, (void*)stpSdecParam, "sdec_thread");

	if(stSWTSParser_Thread)
	{
		wake_up_process(stSWTSParser_Thread);
		SDEC_PRINT_NOTI("Run SDEC SWTS Parser Thread PID:[%d]", stSWTSParser_Thread->pid);
	}
	else
	{
		SDEC_PRINT_NOTI("SDEC SWTS Parser Thread Already Created");
	}

	return;
}


/**
 * SDEC Software Parser redundancy Init
 * ioremap with PVR Download Buffer
 *
 * @param	void
 * @return	void
 * @see
 */
void SDEC_SWP_ComponentInit(S_SDEC_PARAM_T *stpSdecParam)
{
	UINT8 ch = stpSdecParam->ui8SDTChannel;
	UINT32	baseAddr = 0, size = 0;

#if 1	/* channel extension 20140122 jinhwan.bae */
	/* iounmap of previous section buffer, the reason of location : to avoid redundancy process affect to system */
	if(_gVirtAddr != 0)
	{
		iounmap((void*)_gVirtAddr);
		SDEC_PRINT_SWPARSER( "iounmapped previous ioremap pointer");
	}
#endif

#if 0 /* move to set section buffer */
	/* Set Section Circular Buffer */
	tLXSdecMMAlloc.uiGpbSize = SDEC_SWP_SEC_CIR_BUF_SIZE;
	ret = SDEC_MM_Alloc((UINT32)&tLXSdecMMAlloc);
	if(ret != RET_OK)
	{
		SDEC_PRINT_ERROR("SDEC_MM_Alloc Error in SWTSParser Thread [%d]",ret);
		return;
	}
	pui8SecBuffer = (UINT8*)tLXSdecMMAlloc.uiGpbAddr;

	baseAddr = pui8SecBuffer;
	size	 = SDEC_SWP_SEC_CIR_BUF_SIZE;
	virtAddr = (UINT32) ioremap(baseAddr, size);
	_gSecCirVirtOffset = virtAddr - baseAddr;

	gSWTSFilter[0].pui8BufStart = gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr = virtAddr;
	gSWTSFilter[0].pui8BufEnd = gSWTSFilter[0].pui8BufStart + SDEC_SWP_SEC_CIR_BUF_SIZE;
#endif

	/* IOREMAP of PVR DN B Buffer - Only one time ioremap not iounmap because DVR DN Buffer will not be changed forever
	     If DN Buffer Address changed at real time , iounmap should be inserted !
	     2012. 11. 17 PVR_RELATED To Modify DN Channel B to A, PVR Use DN Channel B , SDT will use Ch A */
	/* M14_TBD, H14_TBD, remove H13 contraint */
//	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))

	/* 20130122 jinhwan.bae
	    divide the case with channnel to support UHD DVB webOS PVR & SDT */
	if( ( lx_chip_rev() >= LX_CHIP_REV(M14, B0) ) && ( lx_chip_rev() < LX_CHIP_REV(H14, A0) ) )
	{
		/* 20130927 jinhwan.bae
		     only to protect latch up, M14Bx PVR memory is reduced by 1 channel quantity,
		     should be used gMemCfgDvr[1] configuration.
		     actually, swparser and rfout component is not used M14xx chip, but insert this routine to protect latch up caused by invalid access */
		ch = 0;
	}

	baseAddr = gMemCfgPvr[ch][LX_PVR_MEM_DN].base;
	size	 = gMemCfgPvr[ch][LX_PVR_MEM_DN].size;
	_gVirtAddr = (UINT32) ioremap(baseAddr, size);
	_gVirtOffset = _gVirtAddr - baseAddr;

	SDEC_PRINT_SWPARSER( "SDT DN_BUF [0x%x] size [0x%x]", baseAddr, size);
	SDEC_PRINT_SWPARSER( "virt addr [0x%x]", _gVirtAddr);
	SDEC_PRINT_SWPARSER( "virt offset [0x%x]", _gVirtOffset);

	return;
}


/**
 * SDEC Software Parser Set Section Filter Pattern
 * For Software Section Filtering
 *
 * @param	stpLXSecFltPattern		[in	]  Filter Pattern
 * @return	void
 * @see
 */
void SDEC_SWP_SetSectionFilterPattern(LX_SDEC_SECFLT_PATTERN_T *stpLXSecFltPattern)
{
	UINT8 ui8Mode = 0;
	UINT32 *ui32pPattern;
	UINT32 *ui32pMask;
	UINT32 *ui32pNeg;

	SDEC_PRINT_TRACE( "<--SDEC_SWP_SetSectionFilterPattern");

	ui32pPattern = (UINT32 *)stpLXSecFltPattern->pucPattern;
	ui32pMask = (UINT32 *)stpLXSecFltPattern->pucMask;
	ui32pNeg = (UINT32 *)stpLXSecFltPattern->pucNotEqual;

	memcpy(&gSWTSFilter[0].ui32Pattern_H, ui32pPattern, sizeof(UINT32));
	ui32pPattern++;
	memcpy(&gSWTSFilter[0].ui32Pattern_L, ui32pPattern, sizeof(UINT32));
	memcpy(&gSWTSFilter[0].ui32Mask_H, ui32pMask, sizeof(UINT32));
	ui32pMask++;
	memcpy(&gSWTSFilter[0].ui32Mask_L, ui32pMask, sizeof(UINT32));
	memcpy(&gSWTSFilter[0].ui32Neg_H, ui32pNeg, sizeof(UINT32));
	ui32pNeg++;
	memcpy(&gSWTSFilter[0].ui32Neg_L, ui32pNeg, sizeof(UINT32));

	/* Mask is set by 0 if filtering is needed, but SW filtering need 1 to filter, so inverting */
	gSWTSFilter[0].ui32Mask_H = ~gSWTSFilter[0].ui32Mask_H;
	gSWTSFilter[0].ui32Mask_L = ~gSWTSFilter[0].ui32Mask_L;

	/* If not ver, clear version mask */
	/* If neg, mask set to none. Netcast Dependant, but for fast process, it's convenient.
    	Because Mask and Neg are set as same value, I gave more priority to Neg than Mask */
    if(gSWTSFilter[0].ui32Neg_L != 0)
	{
		gSWTSFilter[0].ui32Mask_L = (gSWTSFilter[0].ui32Mask_L & 0xFFFFFFC1);
    }

	ui8Mode = (UINT8)stpLXSecFltPattern->uiSecFltMode;

	if( ui8Mode & LX_SDEC_FLTMODE_NOCRCCHK )		gSWTSFilter[0].ui8CrcCheck = 0;
	else											gSWTSFilter[0].ui8CrcCheck = 1;

	SDEC_PRINT_TRACE( "-->SDEC_SWP_SetSectionFilterPattern");

	return;
}


/**
 * SDEC Software Parser Set Section Buffer Pointer
 * Init
 *
 * @param	stpSdecParam		[in	]  S_SDEC_PARAM_T
 * @return	void
 * @see
 */
void SDEC_SWP_SetSectionBuffer(S_SDEC_PARAM_T *stpSdecParam)
{
	UINT32	baseAddr = 0, size = 0;

	SDEC_PRINT_TRACE( "<--SDEC_SWP_SetSectionBuffer");

	baseAddr = stpSdecParam->stSdecMeminfo[LX_SDEC_CH_C][0].ui32Baseptr;	/* 0 means index 0 */
	size	 = stpSdecParam->stSdecMeminfo[LX_SDEC_CH_C][0].ui32Endptr - stpSdecParam->stSdecMeminfo[LX_SDEC_CH_C][0].ui32Baseptr;

	SDEC_PRINT_SWPARSER( "baseAddr[0x%x]", baseAddr);
	SDEC_PRINT_SWPARSER( "size[0x%x]", size);

	/* iounmap of previous section buffer, the reason of location : to avoid redundancy process affect to system */
	if(_gSecCirVirtAddr != 0)
	{
		iounmap((void*)_gSecCirVirtAddr);
		SDEC_PRINT_SWPARSER( "iounmapped previous ioremap pointer");
	}

	_gSecCirVirtAddr = (UINT32) ioremap(baseAddr, size);
	_gSecCirVirtOffset = _gSecCirVirtAddr - (UINT32)baseAddr;

	SDEC_PRINT_SWPARSER( "virtAddr[0x%x]", _gSecCirVirtAddr);
	SDEC_PRINT_SWPARSER( "_gSecCirVirtOffset[0x%x]", _gSecCirVirtOffset);

	gSWTSFilter[0].pui8BufStart = gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr = (UINT8 *)_gSecCirVirtAddr;
	gSWTSFilter[0].pui8BufEnd = (UINT8*)(gSWTSFilter[0].pui8BufStart + size);

	SDEC_PRINT_SWPARSER( "gSWTSFilter[0].pui8BufStart[0x%x]", (UINT32)gSWTSFilter[0].pui8BufStart);
	SDEC_PRINT_SWPARSER( "gSWTSFilter[0].pui8ReadPtr[0x%x]", (UINT32)gSWTSFilter[0].pui8ReadPtr);
	SDEC_PRINT_SWPARSER( "gSWTSFilter[0].pui8WritePtr[0x%x]", (UINT32)gSWTSFilter[0].pui8WritePtr);
	SDEC_PRINT_SWPARSER( "gSWTSFilter[0].pui8BufEnd[0x%x]", (UINT32)gSWTSFilter[0].pui8BufEnd);

	SDEC_PRINT_TRACE( "-->SDEC_SWP_SetSectionBuffer");
}


/**
 * SDEC Software Parser Set PID
 * for PID Filter in TS Packet Process
 *
 * @param	ui32PidFltId		[in	]  PID Filter ID
 * @param	ui32PidValue		[in	]  PID Value
 * @return	void
 * @see
 */
void SDEC_SWP_SetPID(UINT32 ui32PidFltId, UINT32 ui32PidValue)
{
	SDEC_PRINT_TRACE( "<--SDEC_SWP_SetSectionFilterPattern");

	if(ui32PidFltId != 0)
	{
		SDEC_PRINT_ERROR("ERROR: SDEC SWTS Parser Set PID with PIDFLT Index [%d] \n",ui32PidFltId);
	}

	gSWTSFilter[ui32PidFltId].ui16PID = ui32PidValue;

	SDEC_PRINT_TRACE( "-->SDEC_SWP_SetSectionFilterPattern");
}


/**
 * SDEC Software Parser Enable Control
 * Send Message to Kernel Thread to Start / Stop
 *
 * @param	stpSdecParam		[in	]  S_SDEC_PARAM_T
 * @param	ui32Arg			[in	]  LX_SDEC_SECFLT_ENABLE_SWPARSER_T arg for IOCTRL
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 * @see
 */
int SDEC_SWP_EnableSWParser
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_ENABLE_SWPARSER_T *stpLXSdecSecfltEnableSWParser)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 uiPidFltId = 0x0;
	UINT8 bEnable = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltEnableSWParser == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_SWP_EnableSWParser");

	ui8Ch = stpLXSdecSecfltEnableSWParser->eCh;
	uiPidFltId = stpLXSdecSecfltEnableSWParser->uiPidFltId;
	bEnable = stpLXSdecSecfltEnableSWParser->bEnable;

	/* TODO :: if you need more filter processing, process based on pid filter index */

	/* Now Just Run Thread */
	if(bEnable)
	{
		SDEC_PRINT_SWPARSER( "Send START to SDEC SWTS Parser Thread CH[%d] \n", ui8Ch);
		/* Set H13 SDT Channel */
		stpSdecParam->ui8SDTChannel = ui8Ch;
		OS_SendEvent( &stpSdecParam->stSdecSWPEvent, SWP_EVENT_START );
	}
	else
	{
		SDEC_PRINT_SWPARSER( "Send STOP to SDEC SWTS Parser Thread CH[%d] \n", ui8Ch);
		LX_SDEC_CHECK_CODE( stpSdecParam->ui8SDTChannel != ui8Ch, goto exit, "ch[%d] is not started for sdt", ui8Ch );
		OS_SendEvent( &stpSdecParam->stSdecSWPEvent, SWP_EVENT_STOP );
	}

	SDEC_PRINT_TRACE( "-->SDEC_SWP_EnableSWParser");

	eRet = RET_OK;

exit:

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return eRet;
}


/**
 * SDEC Software Parser TS Packet Proc.
 * Simple TS Header Checker and Pointing Payload
 *
 * @param	pBuffer		[in	]  TS Data Buffer
 * @param	tsPacket		[in	]  Processed Information about TS
 * @return	if succeeded - 0, else - other
 * @see
 */
static int SDEC_SWP_ProcTsPacket(UINT8 *pBuffer, TsPacket *tsPacket)
{
	UINT8 ui8AdfCtrl = 0;
	UINT8 aTSHeadBuf[TS_HEADER_SIZE];

#if 0
	UINT8 i=0;

	SDEC_PRINT_ERROR("\nGot It! Address [0x%x]\n",&pBuffer[0]);

	for(i=0;i<188;i++)
	{
		if(!(i%10)) printk("\n");
		printk("[%02x]",pBuffer[i]);
	}
#endif

	LX_MEMCPY(aTSHeadBuf, pBuffer, TS_HEADER_SIZE, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);

	/* Check sync_byte */
	if( aTSHeadBuf[0] != TP_SYNC_BYTE )
	{
		SDEC_PRINT_ERROR("ERROR TP_SYNC_BYTE");
		return -TS_SYNC_ERROR;
	}
	/* Check transport_error_indicator */
	if( ((aTSHeadBuf[1] >> 7) & 0x01) == 0x01 )
	{
		SDEC_PRINT_ERROR("ERROR TP_ERROR_INDICATOR");
		return -TP_ERROR_INDICATOR;
	}
	/* Check transport_scrambling_control */
	if( ((aTSHeadBuf[3] >> 6) & 0x03) != 0x00 )
	{
		SDEC_PRINT_ERROR("ERROR TP_SCRAMBLE");
		return -TP_SCRAMBLE;
	}
	/* Check adaptation_field_control */
	ui8AdfCtrl = ((aTSHeadBuf[3] >> 4) & 0x03);
	if( ( ui8AdfCtrl == 0 ) || ( ui8AdfCtrl == 2 ))
	{
		SDEC_PRINT_ERROR("ERROR NO_PAYLOAD");
		return -NO_PAYLOAD;
	}

	/* All Check Done */
	tsPacket->ui8Payload_unit_start_indicator 	= ((aTSHeadBuf[1] >> 6) & 0x01);
	tsPacket->ui16PID							= (((aTSHeadBuf[1] & 0x1F) << 8) | aTSHeadBuf[2]);
	tsPacket->ui8Continuity_counter				= (aTSHeadBuf[3] & 0x0F);

	if(ui8AdfCtrl == 1)
	{
		tsPacket->ui8PayloadSize = TS_PACKET_SIZE - TS_HEADER_SIZE + 1;	/* +1 : Adaptation Field Length */
		LX_ADVANCE_BUFF(pBuffer, TS_HEADER_SIZE - 1, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
		//	tsPacket->pui8Payload =  pBuffer + TS_HEADER_SIZE - 1; 			/* -1 : Adaptation Field Length */
		tsPacket->pui8Payload =  pBuffer;
	}
	else /* ui8AdfCtrl == 3 */
	{
		tsPacket->ui8PayloadSize = TS_PACKET_SIZE - TS_HEADER_SIZE - pBuffer[4];
		LX_ADVANCE_BUFF(pBuffer, pBuffer[4] + TS_HEADER_SIZE, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
		// tsPacket->pui8Payload = pBuffer + pBuffer[4] + TS_HEADER_SIZE;	/* pBuffer[4] : Adaptation Field Length */
		tsPacket->pui8Payload =  pBuffer;
	}

	return 0;
}


/**
 * SDEC Software Parser TS Packet Error Checker
 * PID, Continuity Counter, Payload Size Checker
 *
 * @param	tsPacket		[in	]  Processed Information about TS
 * @return	if succeeded - 0, else - other
 * @see
 */
static int SDEC_SWP_CheckTSError(TsPacket *tsPacket)
{
	/* Check Payload Size */
	if(tsPacket->ui8PayloadSize == 0)
	{
		SDEC_PRINT_ERROR("Payload Size is 0");
		return -PAYLOAD_SIZE_ERR;
	}

	/* Check PID, only 0 is used for SDT , temp */
	if(gSWTSFilter[0].ui16PID != tsPacket->ui16PID)
	{
		SDEC_PRINT_ERROR("PID is not matched [0x%x][0x%x]\n",gSWTSFilter[0].ui16PID,tsPacket->ui16PID);
		return -PID_ERROR;
	}

	/* Check continuity counter */
	if(gSWTSFilter[0].ui8WaitFirstTS == 1)
	{
		gSWTSFilter[0].ui8Continuity_counter = tsPacket->ui8Continuity_counter; /* Set with New CC */
		gSWTSFilter[0].ui8WaitFirstTS = 0;
	}
	else
	{
		if(gSWTSFilter[0].ui8Continuity_counter == tsPacket->ui8Continuity_counter)
		{
			SDEC_PRINT_ERROR("Same CC, Duplicated TS");
			return -DUPLICATED_PACKET;
		}

		gSWTSFilter[0].ui8Continuity_counter = ((gSWTSFilter[0].ui8Continuity_counter + 1) & 0xF);	/* Increase CC as +1 */

		/* If CC Error Happened, We always wait new one , no consideration of previous CC */
		if(gSWTSFilter[0].ui8Continuity_counter != tsPacket->ui8Continuity_counter)
		{
			SDEC_PRINT_ERROR("CC Mismatched [%d][%d]",gSWTSFilter[0].ui8Continuity_counter, tsPacket->ui8Continuity_counter);
			gSWTSFilter[0].ui8Continuity_counter = tsPacket->ui8Continuity_counter; /* Set with New CC */
			return -CC_ERROR;
		}
	}

	return 0;
}


/**
 * SDEC Software Parser Software Section Filter
 *
 * @param	pBuffer		[in	]  Data Buffer
 * @return	if succeeded - 0, else - other
 * @see
 */
static int SDEC_SWP_SectionFiltering(UINT8 *pBuffer)
{
	UINT8 ui8Version = 0, ui8Pattern = 0;
	UINT32 ui32Hdata = 0, ui32Ldata = 0;

	/*---------------------------------------------------------------------------------
	   pBuffer[0]   | pBuffer[1]                                             | pBuffer[2]      | pBuffer[3]
	   ---------------------------------------------------------------------------------
	   table_id (8) | s_syntax_ indi (1) | '0'(1) | rsvd (2) | section length (12) | id/number - MSB(8)
	   ---------------------------------------------------------------------------------
	   pBuffer[4]              | pBuffer[5]                                                       | pBuffer[6]  | pBuffer[7]
	   ---------------------------------------------------------------------------------
	   id/number-LSB (8) | rsvd(2) | version_number (5) | cur_nex_indi(1) | sec_no (8) | l_sec_no (8)
	   ---------------------------------------------------------------------------------*/


	/*---------------------------------------------------------------------------------
	   Pattern, Mask, Neg H Order
	   ---------------------------------------------------------------------------------
	   id/number (16)                                    |s_syntax_ indi (1) | '0'(1) | ----- (6) | table_id (8)
	   ---------------------------------------------------------------------------------
	   Pattern, Mask, Neg L Order
	   ---------------------------------------------------------------------------------
	   proto_ver (8) | last_sec_no (8) | sec_no (8) |rsvd(2) | version_number (5) | cur_nex_indi(1)
	    --------------------------------------------------------------------------------*/

#if 0
	{
		int i;

		printk("\n");
		for(i=0;i<8;i++)
		{
			printk("[%02x]",pBuffer[i]);
		}
		printk("\n");

		printk("ui32Pattern_H : %x\n",gSWTSFilter[0].ui32Pattern_H);
		printk("ui32Pattern_L : %x\n",gSWTSFilter[0].ui32Pattern_L);
		printk("ui32Mask_H : %x\n",gSWTSFilter[0].ui32Mask_H);
		printk("ui32Mask_L : %x\n",gSWTSFilter[0].ui32Mask_L);
		printk("ui32Neg_H : %x\n",gSWTSFilter[0].ui32Neg_H);
		printk("ui32Neg_L : %x\n",gSWTSFilter[0].ui32Neg_L);
	}
#endif


	/* Not Version Filtering */
	if(gSWTSFilter[0].ui32Neg_L != 0)
	{
		ui8Version = ((pBuffer[5] & 0x3E) >> 1);
		ui8Pattern = (UINT8)( (gSWTSFilter[0].ui32Pattern_L & 0x0000003E) >> 1 );
		if( ui8Version == ui8Pattern )
		{
			SDEC_PRINT_SWPARSER( "NOT Ver. NG DATA[0x%x] PATTERN[0x%x]", ui8Version, ui8Pattern);
			return -1;
		}
	}

	/* Filtering */
	if(gSWTSFilter[0].ui32Mask_H != 0)
	{
		ui32Hdata = ( pBuffer[0] | (pBuffer[1] << 8) | (pBuffer[3] << 16) | (pBuffer[4] << 24) );
		if( (ui32Hdata & gSWTSFilter[0].ui32Mask_H) != (gSWTSFilter[0].ui32Pattern_H & gSWTSFilter[0].ui32Mask_H) )
		{
			SDEC_PRINT_SWPARSER( "H NG DATA[0x%08x] PATTERN[0x%08x] MASK[0x%08x]", ui32Hdata, gSWTSFilter[0].ui32Pattern_H, gSWTSFilter[0].ui32Mask_H );
			return -1;
		}
	}

	if(gSWTSFilter[0].ui32Mask_L != 0)
	{
		ui32Ldata = ( pBuffer[5] | (pBuffer[6] << 8) | (pBuffer[7] << 16));
		if( (ui32Ldata & gSWTSFilter[0].ui32Mask_L) != (gSWTSFilter[0].ui32Pattern_L & gSWTSFilter[0].ui32Mask_L) )
		{
			SDEC_PRINT_SWPARSER( "L NG DATA[0x%08x] PATTERN[0x%08x] MASK[0x%08x]", ui32Ldata, gSWTSFilter[0].ui32Pattern_L, gSWTSFilter[0].ui32Mask_L );
			return -1;
		}
	}

	return 0;
}


/**
 * SDEC Software Parser CRC Calculator
 *
 * @param	pBuffer		[in	]  Data Buffer
 * @return	Calculated CRC
 * @see
 */
static UINT32 SDEC_SWP_CalCRC32 (UINT8 *pBuffer)
{
	UINT32 i;
	UINT32 crc = 0xffffffff;
	UINT32 len = gSWTSFilter[0].uiSectionLength - 4 + 3;

	if ( pBuffer == NULL )
		return crc;

	for (i = 0; i < len; i++) {
		crc = (crc << 8) ^ _gCrc32_table[((crc >> 24) ^ *pBuffer++) & 0xff];

		if(pBuffer == gSWTSFilter[0].pui8BufEnd) pBuffer = gSWTSFilter[0].pui8BufStart;
	}

	return crc;
}


/**
 * SDEC Software Parser Extract CRC in Data
 *
 * @param	pBuffer		[in	]  Data Buffer
 * @return	Extracted CRC
 * @see
 */
static UINT32 SDEC_SWP_GetCRC32 (UINT8 *pBuffer)
{
	UINT32 	crc = 0;
	UINT8	crcBuf[4];
	UINT8 	*bufPtr = pBuffer;
	UINT32  len = gSWTSFilter[0].uiSectionLength - 4 + 3;

	if ( pBuffer == NULL )
		return crc;

	LX_ADVANCE_BUFF(bufPtr, len, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
	LX_MEMCPY(crcBuf, bufPtr, 4, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);

	SDEC_PRINT_SWPARSER( "CRC Check Section Data 0-8");
	SDEC_PRINT_SWPARSER( "[%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x]",
								pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3]
								,pBuffer[4],pBuffer[5],pBuffer[6],pBuffer[7]);

	crc |= crcBuf[0] << 24;
	crc |= crcBuf[1] << 16;
	crc |= crcBuf[2] << 8;
	crc |= crcBuf[3] << 0;

	return crc;
}


/**
 * SDEC Software Parser CRC Checker
 *
 * @param	pDataPtr		[in	]  Data Buffer
 * @return	if succeeded - 0, else - other
 * @see
 */
static int SDEC_SWP_CheckCRC(UINT8 *pDataPtr)
{
	UINT32 crc32 = 0, calcCrc32 = 0;

	crc32 		= SDEC_SWP_GetCRC32(pDataPtr);
	calcCrc32 	= SDEC_SWP_CalCRC32(pDataPtr);

	SDEC_PRINT_SWPARSER( "CRC[0x%x] CALCRC[0x%x]", crc32, calcCrc32);

	if( calcCrc32 != crc32 )
	{
		SDEC_PRINT_ERROR("CRC Error [%x][%x]",crc32,calcCrc32);
		return -SECTION_CRC;
	}

	return 0;
}


/**
 * SDEC Software Parser Section Proc
 * It's almost same as Normal Section (CH_A/B) Process
 * to process one way with A/B Section
 *
 * @param	stpSdecParam		[in	]  S_SDEC_PARAM_T
 * @param	ui8Ch			[in	]  Channel , always CH_C
 * @param	ui8SecFltIdx		[in	]  Section Filter Index, always 0
 * @return	if the section is registered to Q - 1, else - 0
 * @see
 */
BOOLEAN SDEC_SWP_Section_Proc(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8SecFltIdx)
{
	UINT8 ui8PidFltIdx = 0x0;
	UINT32 ui32GpbWrPtr = 0x0;
	UINT32 ui32GpbRdPtr = 0x0;
	UINT32 ui32UsrRdPtr = 0x0;
	UINT32 ui32GpbBase = 0x0;
	UINT32 ui32GpbEnd = 0x0;
	BOOLEAN isFound = FALSE;
	unsigned long flags = 0;
	LX_SDEC_NOTIFY_PARAM_T notiParam;
	UINT8 core = 0, org_ch = ui8Ch;

	SDEC_PRINT_ISR( "GPB_DATA_CHC");

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	SDEC_GPB_LOCK(stpSdecParam, org_ch, ui8SecFltIdx);
	ui32GpbWrPtr	= SWP_SEC_VIRT_TO_PHYS((UINT32)gSWTSFilter[0].pui8WritePtr);
	ui32GpbRdPtr	= SWP_SEC_VIRT_TO_PHYS((UINT32)gSWTSFilter[0].pui8ReadPtr);
	/* use s/w read pointer to implement h/w overflow */
	ui32UsrRdPtr	= stpSdecParam->stSdecMeminfo[org_ch][ui8SecFltIdx].ui32UsrReadptr;
	ui32GpbBase		= stpSdecParam->stSdecMeminfo[org_ch][ui8SecFltIdx].ui32Baseptr;
	ui32GpbEnd		= stpSdecParam->stSdecMeminfo[org_ch][ui8SecFltIdx].ui32Endptr;

	if( !ui32GpbBase || !ui32GpbEnd || !ui32GpbWrPtr || !ui32GpbRdPtr )
	{
		SDEC_PRINT_ERROR("in ISR, address is not valid : B[%08x] E[%08x] W[%08x] R[%08x]", ui32GpbBase, ui32GpbEnd, ui32GpbWrPtr, ui32GpbRdPtr);
		SDEC_GPB_UNLOCK(stpSdecParam, org_ch, ui8SecFltIdx);
		return isFound;
	}

	/* jinhwan.bae TODO : Check it */
	// 20110331 kibeom.kim : add the mask to apply the gpb_base_addr (logical address in L9) value
	// ui32GpbRdPtr & ui32GpbWrPtr is restricted to 28 bit.
	ui32GpbWrPtr	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);
	ui32GpbRdPtr 	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);
	ui32UsrRdPtr 	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);

	/* in some case write pointer jump to wrong place, so resetting write pointer as base address - it's not CH_C Case Check It */
	if( ui32GpbWrPtr < ui32GpbBase || ui32GpbEnd < ui32GpbWrPtr )
	{
		SDEC_PRINT_ERROR( "CHC GPB_Data GPB BND ERROR Idx[%d] b[%08x] w[%08x] e[%08x]",
							 ui8SecFltIdx, ui32GpbBase, ui32GpbWrPtr, ui32GpbEnd );

		/* continue to next */
		SDEC_GPB_UNLOCK(stpSdecParam, org_ch, ui8SecFltIdx);
		return isFound;
	}
	stpSdecParam->stSecMap[org_ch][ui8SecFltIdx].stStatusInfo.f.SDEC_FLTSTATE_DATAREADY = 0x1;

	SDEC_PRINT_ISR( "CHC GPB_Data GPB Idx[%d] rp[%d] wp[%d]",
		ui8SecFltIdx, stpSdecParam->ui8GpbInfoRIdx, stpSdecParam->ui8GpbInfoWIdx);

	notiParam.channel	= org_ch;
	notiParam.index 	= ui8SecFltIdx;
	notiParam.readPtr	= ui32GpbRdPtr;
	notiParam.writePtr	= ui32GpbWrPtr;
	notiParam.status	= LX_SDEC_FLTSTATE_DATAREADY;
	notiParam.msec		= OS_GetMsecTicks();

#if ( LX_SDEC_USE_GPB_OW == 1 )
	/* check gpb overflow */
	if( ui32GpbRdPtr < ui32GpbWrPtr )
	{
		/* case 1.
		 *          ui32GpbRdPtr   ui32GpbWrPtr
		 *              V               V
		 * |---------------------------------------------------------|
		 *                  ^
		 *	           ui32UsrRdPtr
		 */
		if( ui32GpbRdPtr < ui32UsrRdPtr &&
			ui32UsrRdPtr < ui32GpbWrPtr )
		{
			SDEC_PRINT_ERROR( "1 CHC GPB_Data GPB Idx[%d] rp[%08x] wp[%08x] OVERFLOW",
								ui8SecFltIdx,ui32GpbRdPtr, ui32GpbWrPtr);
			/* check previous queue */
			SDEC_CheckNotifyOvf(stpSdecParam, &notiParam);
		}
	}
	else
	{
		/* case 2.
		 *      ui32GpbWrPtr                             ui32GpbRdPtr
		 *          V                                          V
		 * |---------------------------------------------------------|
		 *                                                         ^
		 *	                                                 ui32UsrRdPtr
		 *
		 *     ui32GpbWrPtr                              ui32GpbRdPtr
		 *          V                                          V
		 * |---------------------------------------------------------|
		 *      ^
		 *  ui32UsrRdPtr
		 */
		if( ui32GpbRdPtr < ui32UsrRdPtr ||
			ui32UsrRdPtr < ui32GpbWrPtr )
		{
			SDEC_PRINT_ERROR( "2 CHC GPB_Data GPB Idx[%d] rp[%08x] wp[%08x] OVERFLOW",
								ui8SecFltIdx,ui32GpbRdPtr, ui32GpbWrPtr);
			/* check previous queue */
			SDEC_CheckNotifyOvf(stpSdecParam, &notiParam);
		}
	}

#endif
	isFound = SDEC_SetNotifyParam( stpSdecParam, notiParam);

	SDEC_PRINT_ISR( "[%d] rp[0x%08x] wp[0x%08x] b[0x%08x]] e[0x%08x]"
											, ui8SecFltIdx
											, ui32GpbRdPtr
											, ui32GpbWrPtr
											, ui32GpbBase
											, ui32GpbEnd
											);

	/* Write Pointer Update is done in Parser Thread - delete A/B Case Routine */

	/* if oneshot mode is true */
	if (check_one_bit(stpSdecParam->stSecMap[org_ch][ui8SecFltIdx].mode, 1))
	{
		PVR_CTRL_T *ctrl = PVR_GetCtrl(stpSdecParam->ui8SDTChannel);
		SDEC_PRINT_ISR( "CHC GPB_Data GPB Idx[%d] one shot  mode",  ui8SecFltIdx);

		/* Get PID Filter index */
		ui8PidFltIdx = 0;

		SDEC_PRINT_ISR( "CHC GPB_Data GPB Idx[%d] Disable pid filter %d", ui8SecFltIdx, ui8PidFltIdx);

		/* 2012. 11. 17 PVR_RELATED To Modify DN Channel B to A, PVR Use DN Channel B , SDT will use Ch A */
		/* 20140123 To change for supporting both channel in webOS UHD DVB */
		PVR_StopDownloadSDT(ctrl);
		SDEC_HAL_CDIC2PIDFSetPidfData(core, ui8PidFltIdx, 0x1FFF);
		SDEC_HAL_CDIC2PIDFEnablePidFilter(core, ui8PidFltIdx, SDEC_HAL_DISABLE);	/* seperate enable as L9 correction */
	}

	SDEC_GPB_UNLOCK(stpSdecParam, org_ch, ui8SecFltIdx);

	return isFound;
}


/**
 * SDEC Software Parser Main Kernel Thread
 *
 * @param	data		[in	]  S_SDEC_PARAM_T
 * @return
 * @see
 */
static int _SDEC_SWTSParser_Thread(void *data)
{
	S_SDEC_PARAM_T *stpSdecParam = (S_SDEC_PARAM_T *)data;
	int ret = 0, uiSize = 0;
	UINT32 ui32ReceivedEvent = 0;
	UINT8 *pui8DNBuffer = NULL;
	TsPacket	tsPacket;
	UINT8 ui8RemainSize = 0;
	UINT8 *pui8Data = NULL;
	UINT8 *puiDest = NULL;
	SWPEventType eWaitEvent = SWP_EVENT_START;
	UINT32 uiTimeout = (UINT32)SDEC_SWP_TIMEOUT_INFINITE;
	BOOLEAN isFound = FALSE;
	UINT8 continuous_section = 0;
#if 0
	static UINT8 bFirst = 1;
#endif
	static LX_PVR_CH_T pvr_ch = LX_PVR_CH_A;
	PVR_CTRL_T *ctrl = NULL;

	/* DN Buffer Stub */
	UINT32 ui32PhyDNBufStart, ui32PhyDNBufEnd, ui32PhyDNWritePtr;

#ifdef SDEC_SWPARSER_WITH_ARRAYED_DATA
	gSWTSFilter[0].ui8WaitFirstTS = 1; 	// at real, set in request
	gSWTSFilter[0].ui16PID = 0x11;		//  at real, set in request , we need to modify following real cir. routines about these variables.
#endif

	gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_NEED;

	while(!kthread_should_stop())
	{
		// change to event from OS_MsecSleep(SWTSPARSER_THREAD_TIMEOUT);
		ret = OS_RecvEvent( &stpSdecParam->stSdecSWPEvent, (UINT32)eWaitEvent, &ui32ReceivedEvent, OS_EVENT_RECEIVE_ANY, uiTimeout );
		if ( ret == RET_OK )
		{
			/* Check need event */
			if( ui32ReceivedEvent != (UINT32)eWaitEvent )
			{
				SDEC_PRINT_ERROR("SDEC SWP OS_RecvEvent Error Wait[%d] Received[%d]",(UINT32)eWaitEvent, ui32ReceivedEvent );
				continue;
			}
			else
			{
				if( ui32ReceivedEvent == (UINT32)SWP_EVENT_START)	/* Received START */
				{
					pvr_ch = (LX_PVR_CH_T)(stpSdecParam->ui8SDTChannel);
					SDEC_PRINT_SWPARSER( "SDEC Parser Thread Received Start CH[%d] \n",(UINT32)pvr_ch);

					ctrl = PVR_GetCtrl(pvr_ch);
#if 0
					/* If First time, ioremap DN Buffer  */
					if(bFirst == 1)
					{
						SDEC_SWP_ComponentInit();
						bFirst = 0;
					}
					else
					{
						gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr = gSWTSFilter[0].pui8BufStart;
					}
#else
					SDEC_SWP_ComponentInit(stpSdecParam);
#endif

					eWaitEvent = SWP_EVENT_STOP;
					uiTimeout = (UINT32)SDEC_SWP_TIMEOUT;	/* Polling Period */

					gSWTSFilter[0].ui8WaitFirstTS = 1;
					gSWTSFilter[0].ui16PID = 0x11;

					/* Set PVR_CH_FOR_SDT DN R/W Pointer */
					/* Already Set in Start Download , so we only need to set local DB, SW Parser should be followed by DN Start */
					DVR_DN_GetBufBoundReg(pvr_ch, &ui32PhyDNBufStart, &ui32PhyDNBufEnd);
					gSWTSFilter[0].pui8DNBufStart = (UINT8*)SWP_DN_PHYS_TO_VIRT(ui32PhyDNBufStart);
					gSWTSFilter[0].pui8DNBufEnd = (UINT8*)SWP_DN_PHYS_TO_VIRT(ui32PhyDNBufEnd);
					/* It's really different from actual DN Buf write ptr, already data received, wrptr updated at this time */
					gSWTSFilter[0].pui8DNReadPtr = gSWTSFilter[0].pui8DNWritePtr = gSWTSFilter[0].pui8DNBufStart;
					SDEC_PRINT_SWPARSER( "Init DN Buf Start[0x%x]End[0x%x] VirS[0x%x]VirE[0x%x] VirR[0x%x]VirW[0x%x] \n",
						(UINT32)ui32PhyDNBufStart, (UINT32)ui32PhyDNBufEnd, (UINT32)gSWTSFilter[0].pui8DNBufStart,
						(UINT32)gSWTSFilter[0].pui8DNBufEnd, (UINT32)gSWTSFilter[0].pui8DNReadPtr, (UINT32)gSWTSFilter[0].pui8DNWritePtr);
				}
				else	/* Received STOP */
				{
					SDEC_PRINT_SWPARSER( "SDEC Parser Thread Received Stop \n");
					eWaitEvent = SWP_EVENT_START;
					uiTimeout = (UINT32)SDEC_SWP_TIMEOUT_INFINITE;	/* Polling Period */
					ctrl = NULL;

					/* Set Information to PIDF Index 0 Only - Only for SDT temp */
					gSWTSFilter[0].ui8WaitFirstTS = 1;
					gSWTSFilter[0].ui16PID = 0x1FFF;
					gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr = gSWTSFilter[0].pui8BufStart;

					/* if not continue, read 1 TS and Log Out Error 0x1FFF 0x11
					    or if Rptr is updated and Wptr is set to default caused by START following STOP, psuedo Wrap Around Case Read, size is 7077696 */
					continue;
				}
			}
		}
		else
		{
			if(ctrl == NULL) continue;
		}

		/* Get Data From PVR DN Buffer */
		uiSize = 0;
		DVR_DN_GetWritePtrReg(ctrl->ch, &ui32PhyDNWritePtr);

		gSWTSFilter[0].pui8DNWritePtr = (UINT8*)SWP_DN_PHYS_TO_VIRT(ui32PhyDNWritePtr);

		/* Aligned 192 Byte ? --> No, currently 2012. 06.26 check , write pointer updated by 192*n */

		SDEC_PRINT_SWPARSER( "PVR DN Write Addr = 0x%x ", (UINT32)ui32PhyDNWritePtr);
		SDEC_PRINT_SWPARSER( "PVR DN Virtual Write Addr = 0x%x ", (UINT32)gSWTSFilter[0].pui8DNWritePtr);

#ifdef SDEC_SWPARSER_WITH_ARRAYED_DATA
		// temp
		pui8DNBuffer = temp_SDT1;
		uiSize = TS_PACKET_SIZE*4;
#else
		pui8DNBuffer = gSWTSFilter[0].pui8DNReadPtr;
		if(gSWTSFilter[0].pui8DNWritePtr >= gSWTSFilter[0].pui8DNReadPtr)
		{
			uiSize = gSWTSFilter[0].pui8DNWritePtr - gSWTSFilter[0].pui8DNReadPtr;
			SDEC_PRINT_SWPARSER( "Normal Wptr[0x%x] Rptr[0x%x] ", (UINT32)gSWTSFilter[0].pui8DNWritePtr, (UINT32)gSWTSFilter[0].pui8DNReadPtr);
		}
		else
		{
			/* Wrap Around Case */
			// printk("\n\n\n\n\n\n\n\n\n\n WRAP AROUND CASE HAPPENED \n\n\n\n\n\n\n\n\n\n");
			uiSize = ( (gSWTSFilter[0].pui8DNWritePtr - gSWTSFilter[0].pui8DNBufStart) + ( gSWTSFilter[0].pui8DNBufEnd - gSWTSFilter[0].pui8DNReadPtr));
			SDEC_PRINT_SWPARSER( "Wrap Wptr[0x%x] Rptr[0x%x] Sptr[0x%x] Eptr[0x%x] ",
				(UINT32)gSWTSFilter[0].pui8DNWritePtr, (UINT32)gSWTSFilter[0].pui8DNReadPtr, (UINT32)gSWTSFilter[0].pui8DNBufStart, (UINT32)gSWTSFilter[0].pui8DNBufEnd);
		}
#endif
		SDEC_PRINT_SWPARSER( "Get Data From PVR Buffer Size[%d]", uiSize);

		/* Data Unit is 188*n , it means it could be some sections + part of another section */
		while((uiSize - TS_PACKET_SIZE + 4) >= 0) /* + 4 is timestamp , temp */
		{
			continuous_section = 0;

			LX_ADVANCE_BUFF(pui8DNBuffer, 4, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd); /* + 4 is timestamp , temp */

			ret = SDEC_SWP_ProcTsPacket(pui8DNBuffer, &tsPacket);
			if(ret != 0)
			{
				SDEC_PRINT_ERROR("SDEC_SWP_ProcTsPacket Error [%d]",ret);
				goto NEXT_TS;
			}

			SDEC_PRINT_SWPARSER( "Get TS Payload Size[%d]", tsPacket.ui8PayloadSize);

			ret = SDEC_SWP_CheckTSError(&tsPacket);
			if(ret != 0)
			{
				SDEC_PRINT_ERROR("SDEC_SWP_CheckTSError Error [%d]",ret);
				goto NEXT_TS;
			}

			/* Check Initial State and Payload Unit Start Indicator, Recalulate Pointer and Size */
			if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_HEADER_NEED)
			{
				if(tsPacket.ui8Payload_unit_start_indicator == 1)
				{
					/* Update Data Start Pointer */
					tsPacket.ui8PayloadSize -= tsPacket.pui8Payload[0] + 1;
					//tsPacket.pui8Payload += tsPacket.pui8Payload[0] + 1;
					LX_ADVANCE_BUFF(tsPacket.pui8Payload, tsPacket.pui8Payload[0] + 1, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
					gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;
					gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_ING;
					SDEC_PRINT_SWPARSER( "PUSI Case, Cal again TS Payload with PUSI , Remain P Size[%d]", tsPacket.ui8PayloadSize);
				}
				else /* if(tsPacket.ui8Payload_unit_start_indicator != 1) */
				{
					SDEC_PRINT_SWPARSER( "Waiting Payload Unit Start Indicator");
					goto NEXT_TS;
				}
			}
			else
			{
				/* Check PUSI and add 1 byte to get payload, in case of PUSI TS -> PUSI TS Case, TNT_TS_R3_PROD_PATCHE_DIFF_R7_20130416.TS, 2013. 04. 24 */
				/* To make less side effect, add this line to defend the problem. TODO in 14~ : Continuity Counter Check and Consistent Parsing of PUSI case */
				if(tsPacket.ui8Payload_unit_start_indicator == 1)
				{
					/* Update Data Start Pointer just 1, in this case, we only consider PUSI -> PUSI case, remain start section is discarded and try to get next period */
					tsPacket.ui8PayloadSize -= 1;
					LX_ADVANCE_BUFF(tsPacket.pui8Payload, 1, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);

					continuous_section = 1;
				}
			}

			/* ui8RemainSize 는 188 보다 작은 값 */
			ui8RemainSize = tsPacket.ui8PayloadSize;
			pui8Data = tsPacket.pui8Payload;

			/* Policy -  If 1 TS packet have more than 2 section data, skip later one.  :: TODO if you need more filters, change this point! */
			// while(ui8RemainSize > 0)
			/* Policy Changed! 20131128
			    to fix the problem stream dtg_mhegts_ocsets(uk)_v7.70.ts
			    2 Sections compose 1 Table, and last section start point is continuous from previous section data in 1TS,
			    the middle of TS packet among 2 sections TS have pusi and pointer.
			    previously, we discard remained data at this case and wait next pusi, it causes last section missing.
			    add continuous section processing. checked also TNT_TS_R3_PROD_PATCHE_DIFF_R7_20130416.TS.
			    add this label , continuous_section and ui8RemainSize */
parse_continuous_section:
			{
				if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_HEADER_ING)
				{
					SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_HEADER_ING");

					puiDest = gSWTSFilter[0].au8HeaderBuffer + SECTION_FILTER_LENGTH - gSWTSFilter[0].ui8NeedHeaderSize;

					if(ui8RemainSize >= gSWTSFilter[0].ui8NeedHeaderSize)
					{
						// memcpy(puiDest, pui8Data, gSWTSFilter[0].ui8NeedHeaderSize);
						LX_MEMCPY(puiDest,pui8Data,gSWTSFilter[0].ui8NeedHeaderSize,gSWTSFilter[0].pui8DNBufStart,gSWTSFilter[0].pui8DNBufEnd);
						ui8RemainSize -= gSWTSFilter[0].ui8NeedHeaderSize;
						//pui8Data += gSWTSFilter[0].ui8NeedHeaderSize;
						LX_ADVANCE_BUFF(pui8Data, gSWTSFilter[0].ui8NeedHeaderSize, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
						gSWTSFilter[0].ui8NeedHeaderSize = 0;
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_DONE;
						SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_HEADER_DONE ui8RemainSize[%d]",ui8RemainSize);
						SDEC_PRINT_SWPARSER( "Get Section Header");
						SDEC_PRINT_SWPARSER( "[%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x]",gSWTSFilter[0].au8HeaderBuffer[0],gSWTSFilter[0].au8HeaderBuffer[1],
														gSWTSFilter[0].au8HeaderBuffer[2],gSWTSFilter[0].au8HeaderBuffer[3],gSWTSFilter[0].au8HeaderBuffer[4],
														gSWTSFilter[0].au8HeaderBuffer[5],gSWTSFilter[0].au8HeaderBuffer[6],gSWTSFilter[0].au8HeaderBuffer[7]);
					}
					else /* Remain Size is less than Need Size , Save only Remain Size */
					{
						SDEC_PRINT_SWPARSER( "Remain Size is less than Need Size , Save only Remain Size");
						//memcpy(puiDest, pui8Data, ui8RemainSize);
						LX_MEMCPY(puiDest,pui8Data,ui8RemainSize,gSWTSFilter[0].pui8DNBufStart,gSWTSFilter[0].pui8DNBufEnd);
						gSWTSFilter[0].ui8NeedHeaderSize -= ui8RemainSize;
						// gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_ING; , Still ING
						goto NEXT_TS;
					}
				}

				/* Section Header Filtering */
				if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_HEADER_DONE)
				{
					SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_HEADER_DONE");

					ret = SDEC_SWP_SectionFiltering(&(gSWTSFilter[0].au8HeaderBuffer[0]));
					if(ret != 0)
					{
						SDEC_PRINT_SWPARSER( "SDEC_SWP_SectionFiltering Error [%d]",ret);
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_NEED;
						gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;
						goto NEXT_TS;
					}
					else
					{
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_FILTER_DONE;
						gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;
					}
				}

				/* Copy Section Header and Remain Payload to Circular Buffer */
				if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_HEADER_FILTER_DONE)
				{
					SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_HEADER_FILTER_DONE");

					/* Get Section Length */
					gSWTSFilter[0].uiSectionLength = (((gSWTSFilter[0].au8HeaderBuffer[1] & 0x0F) << 8) | gSWTSFilter[0].au8HeaderBuffer[2]);
					SDEC_PRINT_SWPARSER( "Section Header Filtering Done SecLen[%d]",gSWTSFilter[0].uiSectionLength);
					/* Copy Section Header */
					LX_MEMCPY_TO_CIRCULAR(gSWTSFilter[0].pui8WritePtr, gSWTSFilter[0].au8HeaderBuffer, SECTION_FILTER_LENGTH, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
					LX_ADVANCE_BUFF(gSWTSFilter[0].pui8WritePtr, SECTION_FILTER_LENGTH, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
					/* Update Remain Section Length */
					gSWTSFilter[0].uiRemainSectionData = gSWTSFilter[0].uiSectionLength + 3 - SECTION_FILTER_LENGTH;
					SDEC_PRINT_SWPARSER( "Header Copy Done, remain Section Data [%d]",gSWTSFilter[0].uiRemainSectionData);
					/* Copy Remain Payload */
					if(ui8RemainSize > gSWTSFilter[0].uiRemainSectionData)
					{
						/* 1 Section / 1 TS Case and Last TS of Section Case */
						//LX_MEMCPY_TO_CIRCULAR(gSWTSFilter[0].pui8WritePtr, pui8Data, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						LX_MEMCPY_CIR_TO_CIR(gSWTSFilter[0].pui8WritePtr, pui8Data, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd );
						LX_ADVANCE_BUFF(pui8Data, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
						LX_ADVANCE_BUFF(gSWTSFilter[0].pui8WritePtr, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						/* Update Remain Section Length */
						ui8RemainSize -= gSWTSFilter[0].uiRemainSectionData;
						gSWTSFilter[0].uiRemainSectionData = 0;
					}
					else if(ui8RemainSize > 0)
					{
						//LX_MEMCPY_TO_CIRCULAR(gSWTSFilter[0].pui8WritePtr, pui8Data, ui8RemainSize, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						LX_MEMCPY_CIR_TO_CIR(gSWTSFilter[0].pui8WritePtr, pui8Data, ui8RemainSize, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd );
						// all consumed  LX_ADVANCE_BUFF(pui8Data, ui8RemainSize, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
						LX_ADVANCE_BUFF(gSWTSFilter[0].pui8WritePtr, ui8RemainSize, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						/* Update Remain Section Length */
						gSWTSFilter[0].uiRemainSectionData -= ui8RemainSize;
						ui8RemainSize = 0;
					}

					SDEC_PRINT_SWPARSER( "Cur. SecData in Remain TS Payload Copy Done, Final Remain SecData [%d]",gSWTSFilter[0].uiRemainSectionData);

					if(gSWTSFilter[0].uiRemainSectionData == 0)
					{
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_ALL_COPY_DONE;
						SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_ALL_COPY_DONE 1 ui8RemainSize[%d]",ui8RemainSize);
					}
					else
					{
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_COPY_DONE;
						goto NEXT_TS;
					}
				}


				/* If all payload is continued data from previous TS packet, just copy */
				if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_HEADER_COPY_DONE)
				{
					SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_HEADER_COPY_DONE");

					if(ui8RemainSize >= gSWTSFilter[0].uiRemainSectionData)
					{
						LX_MEMCPY_CIR_TO_CIR(gSWTSFilter[0].pui8WritePtr, pui8Data, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd );
						LX_ADVANCE_BUFF(pui8Data, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
						LX_ADVANCE_BUFF(gSWTSFilter[0].pui8WritePtr, gSWTSFilter[0].uiRemainSectionData, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						/* Update Remain Section Length */
						ui8RemainSize -= gSWTSFilter[0].uiRemainSectionData;
						gSWTSFilter[0].uiRemainSectionData = 0;
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_ALL_COPY_DONE;
						SDEC_PRINT_SWPARSER( "SWP_FLTSTAT_SEC_ALL_COPY_DONE 2 ui8RemainSize[%d]",ui8RemainSize);
					}
					else
					{
						LX_MEMCPY_CIR_TO_CIR(gSWTSFilter[0].pui8WritePtr, pui8Data, ui8RemainSize, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd );
						// all consumed  LX_ADVANCE_BUFF(pui8Data, ui8RemainSize, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd);
						LX_ADVANCE_BUFF(gSWTSFilter[0].pui8WritePtr, ui8RemainSize, gSWTSFilter[0].pui8BufStart, gSWTSFilter[0].pui8BufEnd);
						/* Update Remain Section Length */
						gSWTSFilter[0].uiRemainSectionData -= ui8RemainSize;
						ui8RemainSize = 0;
						SDEC_PRINT_SWPARSER( "Copy Section Body gSWTSFilter[0].uiRemainSectionData[%d] ui8RemainSize[%d]",gSWTSFilter[0].uiRemainSectionData,ui8RemainSize);
						// gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_COPY_DONE; Still HEADER + Part of PAYLOAD
					}
				}

				/* Check CRC and Callback - Option?? */
				if(gSWTSFilter[0].eFilterState == SWP_FLTSTAT_SEC_ALL_COPY_DONE)
				{
					ret = 0;
					if( gSWTSFilter[0].ui8CrcCheck != 0 )
					{
						ret = SDEC_SWP_CheckCRC(gSWTSFilter[0].pui8ReadPtr);
					}

					if(ret == 0)
					{
						SDEC_PRINT_SWPARSER( "Section Complete Callback!");

						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_NEED;
						gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;
#if 0
						{
							int temp_i = 0;
							for(temp_i=0;temp_i<gSWTSFilter[0].uiSectionLength + 3;temp_i++)
							{
								if(!(temp_i % 10)) printk("\n");
								printk("[%02x]",gSWTSFilter[0].pui8ReadPtr[temp_i]);
							}
							 printk("\n");
						}
#endif
						//printk("CALLBACKED ReadPtr(0x%x), WritePtr(0x%x)\n", gSWTSFilter[0].pui8ReadPtr, gSWTSFilter[0].pui8WritePtr);
						isFound = SDEC_SWP_Section_Proc(stpSdecParam, LX_SDEC_CH_C, 0); /* Section Filter Index Always 0 */

						/* if found notifying section data, notify to appl layer */
						if(isFound) queue_work(stpSdecParam->workqueue, &stpSdecParam->Notify);

						isFound = FALSE;

						/* Update Read Pointer, Maybe CB Pointer or UsrReadPtr as A/B is needed */
						gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr;

						if(continuous_section == 1)
						{
							gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;
							gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_ING;

							SDEC_PRINT_SWPARSER( "Continue to Parse Continuous Section ui8RemainSize[%d]!!!\n",ui8RemainSize);
							goto parse_continuous_section;
						}
					}
					else
					{
						SDEC_PRINT_ERROR("SDEC_SWP_CheckCRC Error [%d]",ret);
						gSWTSFilter[0].eFilterState = SWP_FLTSTAT_SEC_HEADER_NEED;
						gSWTSFilter[0].ui8NeedHeaderSize = SECTION_FILTER_LENGTH;

						/* Update Read Pointer, Maybe CB Pointer or UsrReadPtr as A/B is needed */
						gSWTSFilter[0].pui8ReadPtr = gSWTSFilter[0].pui8WritePtr;
						goto NEXT_TS;
					}
				}
			}

NEXT_TS:
			uiSize -= TS_PACKET_SIZE + 4;			/* + 4 is timestamp , temp */
			LX_ADVANCE_BUFF(pui8DNBuffer, TS_PACKET_SIZE, gSWTSFilter[0].pui8DNBufStart, gSWTSFilter[0].pui8DNBufEnd); /* + 4 will be done in start of routine timestamp , temp */
			SDEC_PRINT_SWPARSER( "NOW NEXT_TS Remain PVR Data [%d]",uiSize);
		}

		/* Update Read Pointer - 필요 없을 듯 Check 해서 삭제 요 */
		if( RET_OK != PVR_SetDownloadReadAddr(ctrl, (UINT32)SWP_DN_VIRT_TO_PHYS(pui8DNBuffer)))
		{
			SDEC_PRINT_ERROR("KADP_PVR_DnGetWriteAddr Error");
			continue;
		}
		gSWTSFilter[0].pui8DNReadPtr = pui8DNBuffer;

	}

	SDEC_PRINT_ERROR("Stop SDEC SW Parser thread\n");

	return 0;
}


/*-----------------------------------------------------------------
  *                                       LIVE HEVC STUB
  *----------------------------------------------------------------*/
typedef struct _RFOUT_BUF_
{
	UINT8	*pStart;
	UINT8	*pEnd;
	UINT8	*pRead;
	UINT8	*pWrite;
}SDEC_RFOUT_BUF_T;

SDEC_RFOUT_BUF_T gVa_DNBuf;
SDEC_RFOUT_BUF_T gVa_SecBuf;

static UINT32 _gRFoutDnVirtAddr = 0;
static UINT32 _gRFoutDnVirtOffset = 0;
static SINT32 _gRFoutSecCirVirtOffset = 0;
static UINT32 _gRFoutSecCirVirtAddr = 0;

#define RFOUT_DN_PHYS_TO_VIRT(x)	(x + _gRFoutDnVirtOffset)
#define RFOUT_DN_VIRT_TO_PHYS(x)	(x - _gRFoutDnVirtOffset)
#define RFOUT_SEC_PHYS_TO_VIRT(x)	(x + _gRFoutSecCirVirtOffset)
#define RFOUT_SEC_VIRT_TO_PHYS(x)	(x - _gRFoutSecCirVirtOffset)


void SDEC_RFOUT_ComponentInit(S_SDEC_PARAM_T *stpSdecParam)
{
	UINT8 ch = stpSdecParam->ui8RFBypassChannel;
	UINT32 baseAddr = 0, size = 0;

	/* iounmap of previous section buffer, the reason of location : to avoid redundancy process affect to system */
	if(_gRFoutDnVirtAddr != 0)
	{
		iounmap((void*)_gRFoutDnVirtAddr);
		SDEC_PRINT_RFOUT( "iounmapped previous ioremap pointer");
	}

	if( ( lx_chip_rev() >= LX_CHIP_REV(M14, B0) ) && ( lx_chip_rev() < LX_CHIP_REV(H14, A0) ) )
	{
		/* 20130927 jinhwan.bae
		     only to protect latch up, M14Bx PVR memory is reduced by 1 channel quantity,
		     should be used gMemCfgDvr[1] configuration.
		     actually, swparser and rfout component is not used M14xx chip, but insert this routine to protect latch up caused by invalid access */
		ch = 0;
	}
	baseAddr = gMemCfgPvr[ch][LX_PVR_MEM_DN].base;
	size	 = gMemCfgPvr[ch][LX_PVR_MEM_DN].size;

	_gRFoutDnVirtAddr = (UINT32) ioremap(baseAddr, size);
	_gRFoutDnVirtOffset = _gRFoutDnVirtAddr - baseAddr;

	SDEC_PRINT_RFOUT( "RF Out DN_BUF [0x%x] size [0x%x]", baseAddr, size);
	SDEC_PRINT_RFOUT( "virtAddr[0x%x]", _gRFoutDnVirtAddr);
	SDEC_PRINT_RFOUT( "_gSecCirVirtOffset[0x%x]", _gRFoutDnVirtOffset);

	gVa_DNBuf.pStart = gVa_DNBuf.pRead = gVa_DNBuf.pWrite = (UINT8 *)_gRFoutDnVirtAddr;
	gVa_DNBuf.pEnd = (UINT8*)(gVa_DNBuf.pStart + size);

	SDEC_PRINT_RFOUT( "gVa_DNBuf.pStart[0x%x]", (UINT32)gVa_DNBuf.pStart);
	SDEC_PRINT_RFOUT( "gVa_DNBuf.pRead[0x%x]", (UINT32)gVa_DNBuf.pRead);
	SDEC_PRINT_RFOUT( "gVa_DNBuf.pWrite[0x%x]", (UINT32)gVa_DNBuf.pWrite);
	SDEC_PRINT_RFOUT( "gVa_DNBuf.pEnd[0x%x]", (UINT32)gVa_DNBuf.pEnd);

	return;
}

void SDEC_RFOUT_SetSectionBuffer(S_SDEC_PARAM_T *stpSdecParam)
{
	/* for RFOUT BDRC Section Buffer, we always use CH_A GPB regardless of DN Channel Composition */
//	UINT8 ch = stpSdecParam->ui8RFBypassChannel;
	UINT32 baseAddr = 0, size = 0;

	SDEC_PRINT_TRACE( "<--SDEC_RFOUT_SetSectionBuffer");

	baseAddr = stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][LX_SDEC_BDRC_GPB_NO].ui32Baseptr;	/* 0 means index 0 */
	size	 = stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][LX_SDEC_BDRC_GPB_NO].ui32Endptr - stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][LX_SDEC_BDRC_GPB_NO].ui32Baseptr;

	SDEC_PRINT_RFOUT( "baseAddr[0x%x]", baseAddr);
	SDEC_PRINT_RFOUT( "size[0x%x]", size);

	/* iounmap of previous section buffer, the reason of location : to avoid redundancy process affect to system */
	if(_gRFoutSecCirVirtAddr != 0)
	{
		iounmap((void*)_gRFoutSecCirVirtAddr);
		SDEC_PRINT_RFOUT( "iounmapped previous ioremap pointer");
	}

	_gRFoutSecCirVirtAddr = (UINT32) ioremap(baseAddr, size);
	_gRFoutSecCirVirtOffset = _gRFoutSecCirVirtAddr - (UINT32)baseAddr;

	SDEC_PRINT_RFOUT( "virtAddr[0x%x]", _gRFoutSecCirVirtAddr);
	SDEC_PRINT_RFOUT( "_gSecCirVirtOffset[0x%x]", _gRFoutSecCirVirtOffset);

	gVa_SecBuf.pStart = gVa_SecBuf.pRead = gVa_SecBuf.pWrite = (UINT8 *)_gRFoutSecCirVirtAddr;
	gVa_SecBuf.pEnd = (UINT8*)(gVa_SecBuf.pStart + size);

	SDEC_PRINT_RFOUT( "gVa_SecBuf.pStart[0x%x]", (UINT32)gVa_SecBuf.pStart);
	SDEC_PRINT_RFOUT( "gVa_SecBuf.pRead[0x%x]", (UINT32)gVa_SecBuf.pRead);
	SDEC_PRINT_RFOUT( "gVa_SecBuf.pWrite[0x%x]", (UINT32)gVa_SecBuf.pWrite);
	SDEC_PRINT_RFOUT( "gVa_SecBuf.pEnd[0x%x]", (UINT32)gVa_SecBuf.pEnd);

	SDEC_PRINT_TRACE( "-->SDEC_RFOUT_SetSectionBuffer");
}



struct task_struct	*stRFOUT_Thread;
#define SDEC_RFOUT_TIMEOUT			1 // 1ms, check wptr-rptr output packet every 1ms
#define SDEC_RFOUT_THRESHOLD		1 // temp 1 byte


static int _SDEC_RFOUT_Thread(void *data)
{
	S_SDEC_PARAM_T *stpSdecParam = (S_SDEC_PARAM_T *)data;
	int ret = 0;
	UINT32 ui32ReceivedEvent = 0;
	RFOUTEventType eWaitEvent = RFOUT_EVENT_START;
	UINT32 uiTimeout = (UINT32)SDEC_SWP_TIMEOUT_INFINITE;
	UINT32 uiPhyDNWritePtr = 0, uiPhyDNReadPtr = 0, uiPhyDNBasePtr = 0, uiPhyDNEndPtr = 0;
	UINT32 data_len = 0,  packet_cnt = 0;
	UINT32 i = 0;
	UINT8 ch = 0;
	PVR_CTRL_T *ctrl = NULL;

	/* Init
	    Initialize saved gpb write pointer */

	/* Check write pointer of PVR DN Buffer
	    If read pvr write poitner - prev pvr read pointer  > threshold,
	    copy 188 byte only to gpb buffer
	    set gpb write pointer and call update BDRC write pointer. */

	while(!kthread_should_stop())
	{
		// change to event from OS_MsecSleep(SWTSPARSER_THREAD_TIMEOUT);
		ret = OS_RecvEvent( &stpSdecParam->stSdecRFOUTEvent, (UINT32)eWaitEvent, &ui32ReceivedEvent, OS_EVENT_RECEIVE_ANY, uiTimeout );
		if ( ret == RET_OK )
		{
			/* Check need event */
			if( ui32ReceivedEvent != (UINT32)eWaitEvent )
			{
				SDEC_PRINT_ERROR("SDEC RFOUT OS_RecvEvent Error Wait[%d] Received[%d]",(UINT32)eWaitEvent, ui32ReceivedEvent );
				continue;
			}
			else
			{
				if( ui32ReceivedEvent == (UINT32)RFOUT_EVENT_START)	/* Received START */
				{
					SDEC_PRINT_RFOUT( "SDEC RFOUT Thread Received Start \n");

					SDEC_RFOUT_ComponentInit(stpSdecParam);
					SDEC_RFOUT_SetSectionBuffer(stpSdecParam);

					ch = stpSdecParam->ui8RFBypassChannel;
					ctrl = PVR_GetCtrl(ch);

					uiPhyDNWritePtr = ctrl->buf.dn.wptr;
					uiPhyDNReadPtr =  ctrl->buf.dn.rptr;
					uiPhyDNBasePtr = ctrl->buf.dn.base;
					uiPhyDNEndPtr = ctrl->buf.dn.end;

	//				uiPhySECWritePtr =  stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][1].ui32Baseptr;
	//				uiPhySECReadPtr =  stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][1].ui32Baseptr;
	//				uiPhySECBasePtr =  stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][1].ui32Baseptr;
	//				uiPhySECEndPtr =  stpSdecParam->stSdecMeminfo[LX_SDEC_CH_A][1].ui32Endptr;

					eWaitEvent = RFOUT_EVENT_STOP;
					uiTimeout = (UINT32)SDEC_RFOUT_TIMEOUT;	/* Polling Period */
				}
				else	/* Received STOP */
				{
					SDEC_PRINT_SWPARSER( "SDEC Parser Thread Received Stop \n");
					eWaitEvent = SWP_EVENT_START;
					uiTimeout = (UINT32)SDEC_SWP_TIMEOUT_INFINITE;	/* Polling Period */
					ctrl = NULL;
					continue;
				}
			}
		}
		else
		{
			if(ctrl == NULL) continue;
		}

		/* Get PVR DN write pointer */
		DVR_DN_GetWritePtrReg(ctrl->ch, &uiPhyDNWritePtr);

		gVa_DNBuf.pWrite =(UINT8*)RFOUT_DN_PHYS_TO_VIRT(uiPhyDNWritePtr);

		/* wrap around process */
		if(gVa_DNBuf.pWrite > gVa_DNBuf.pRead)
		{
			data_len = gVa_DNBuf.pWrite - gVa_DNBuf.pRead;
		}
		else
		{
			data_len = (gVa_DNBuf.pEnd - gVa_DNBuf.pRead) + (gVa_DNBuf.pWrite - gVa_DNBuf.pStart);
		}

		SDEC_PRINT_RFOUT( "DN w[0x%x]r[0x%x]data_len[0x%x] ", (UINT32)(gVa_DNBuf.pWrite), (UINT32)(gVa_DNBuf.pRead), data_len );

		if(data_len > SDEC_RFOUT_THRESHOLD)
		{
			/* 1. cal packet count = length / 192 */
			/* loop to packet count */
			/* 2. detect 0x47 at pBuf[4] */
			/* 3. copy pBuf[4] * 188 to pBuf[0] */
			/* 4. set gpb write pointer */

			packet_cnt = ( data_len / 192 );

			for(i=0;i<packet_cnt;i++)
			{
				if(gVa_DNBuf.pRead[4] != 0x47)
				{
					SDEC_PRINT_RFOUT( "SYNC ERROR [0x%x] ", gVa_DNBuf.pRead[4]);
					/* ERROR PROCESSING NEEDED */
					// SEARCH_SYNCBYTE();
					break; // temp, should be continue!!!
				}

				/* Need BDRC Buffer Check ?? */
				//uiPhyGpbReadPtr = SDEC_HAL_GPBGetReadPtr(LX_SDEC_CH_A, 1);
				//uiPhyGpbReadPtr |= 0x60000000;

				LX_MEMCPY_CIR_TO_CIR_RFOUT(gVa_SecBuf.pWrite, &gVa_DNBuf.pRead[4], 188, gVa_SecBuf.pStart, gVa_SecBuf.pEnd, gVa_DNBuf.pStart, gVa_DNBuf.pEnd );
				LX_ADVANCE_BUFF(gVa_DNBuf.pRead, 192, gVa_DNBuf.pStart, gVa_DNBuf.pEnd );
				LX_ADVANCE_BUFF(gVa_SecBuf.pWrite, 188, gVa_SecBuf.pStart, gVa_SecBuf.pEnd );
			}

			/* we always use CH_A GPB for RF OUT */
			SDEC_HAL_GPBSetWritePtr(LX_SDEC_CH_A, LX_SDEC_BDRC_GPB_NO, RFOUT_SEC_VIRT_TO_PHYS((UINT32)gVa_SecBuf.pWrite));

			/* BDRC 3 is only available for TS out */
			SDEC_HAL_BDRC_SetWptrUpdate( _SDEC_CORE_0, 3 );

		}
	}

	SDEC_PRINT_ERROR("Stop SDEC RFOUT thread\n");

	return 0;
}


void SDEC_RFOUT_Init(S_SDEC_PARAM_T *stpSdecParam)
{
	/* Create SW Section Parser Thread */
	stRFOUT_Thread = kthread_create( _SDEC_RFOUT_Thread, (void*)stpSdecParam, "sdec_rfout");

	if(stRFOUT_Thread)
	{
		wake_up_process(stRFOUT_Thread);
		SDEC_PRINT_NOTI( "Run SDEC RFOUT Thread PID:[%d]\n", stRFOUT_Thread->pid);
	}
	else
	{
		SDEC_PRINT_NOTI( "SDEC RFOUT Thread Already Created\n");
	}

	return;
}


int SDEC_RFOUT_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_RFOUT_ENABLE_T *stpLXSdecRFOUTEnable)
{
	int eRet = RET_ERROR;
	UINT8 ch = 0;
	UINT8 bEnable = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecRFOUTEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_RFOUT_Enable");

	ch = (UINT8)(stpLXSdecRFOUTEnable->eCh);
	bEnable = stpLXSdecRFOUTEnable->bEnable;

	/* Now Just Run Thread */
	if(bEnable)
	{
		SDEC_PRINT_RFOUT( "Send START to SDEC RFOUT Thread\n");
		/* for UHD bypass channel */
		stpSdecParam->ui8RFBypassChannel = ch;
		OS_SendEvent( &stpSdecParam->stSdecRFOUTEvent, RFOUT_EVENT_START );
	}
	else
	{
		SDEC_PRINT_RFOUT( "Send STOP to SDEC RFOUT Thread\n");
		LX_SDEC_CHECK_CODE( stpSdecParam->ui8RFBypassChannel != ch, goto exit, "ch[%d] is not started for bypass", (UINT8)(stpLXSdecRFOUTEnable->eCh) );
		OS_SendEvent( &stpSdecParam->stSdecRFOUTEvent, RFOUT_EVENT_STOP );
	}

	SDEC_PRINT_TRACE( "-->SDEC_RFOUT_Enable");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return eRet;
}


/* End of LIVE_HEVC */




