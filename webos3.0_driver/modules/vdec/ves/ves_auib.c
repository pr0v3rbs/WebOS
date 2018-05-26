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
 * main driver implementation for vdec device.
 * vdec device will teach you how to make device driver with new platform.
 *
 * author     junghyun.son@lge.com
 * version    1.0
 * date       2013.10.29
 * note       Additional information.
 *
 * @addtogroup vdec_ves
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ves_auib.h"
#include "ves_cpb.h"

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "hal/ipc_reg_api.h"
#include "hal/top_hal_api.h"
#include "hal/vdec_base.h"

#include "log.h"
/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define	VES_AUIB_NUM_OF_MEM			(VDEC_NUM_OF_CHANNEL * 2)		// H/W Queue & S/W Queue

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	UINT64		ui64TimeStamp;
	UINT32		ui32UId;
	BOOLEAN		b512bytesAligned;
	UINT32		ui32AuStartAddr;	// for not 512 bytes aligned buf
	UINT32 		ui32AuSize;
} S_SW_AUIB_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static struct
{
	UINT8		ui8PdecCh;

	UINT32 		ui32PhyBaseAddr_HwQ;	// constant
	UINT32 		*pui32VirBasePtr_HwQ;	// constant
	UINT32 		ui32BufSize_HwQ;		// constant

	UINT32 		ui32PhyBaseAddr_SwQ;	// constant
	UINT32 		*pui32VirBasePtr_SwQ;	// constant
	UINT32 		ui32BufSize_SwQ;		// constant

	BOOLEAN		bIsHwPath;
	BOOLEAN		bFromTVP;

	// for Debug
	UINT32 		ui32WrOffset;		// variable
	UINT32 		ui32RdOffset;		// variable

	UINT32		ui32RdOffsetForTsMatch;
	UINT32		ui32UidCounter;

	UINT32		ui32PrevPts;
	UINT64		ui64PrevTs;

	E_PDEC_CODEC_TYPE_T eCodecType;

	wait_queue_head_t wq;
} gsVesAuib[VDEC_NUM_OF_CHANNEL];

static struct
{
	BOOLEAN		bUsed;
	UINT32		ui32BufAddr;
	UINT32		ui32BufSize;
} gsAuibMem[VES_AUIB_NUM_OF_MEM];

/**
********************************************************************************
* @brief
* initializing ves auib module
* @remarks
* initializing global values of ves_auib
* @par requirements:
* NONE
* @param
* NONE
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_Init(void)
{
	UINT32	i;

	logm_noti(vdec_ves, "[AUIB] Init \n" );


	for( i = 0; i < VDEC_NUM_OF_CHANNEL; i++ )
	{
		gsVesAuib[i].ui32PhyBaseAddr_HwQ = 0x0;
		gsVesAuib[i].pui32VirBasePtr_HwQ = NULL;
		gsVesAuib[i].ui32BufSize_HwQ = 0x0;

		gsVesAuib[i].ui32PhyBaseAddr_SwQ = 0x0;
		gsVesAuib[i].pui32VirBasePtr_SwQ = NULL;
		gsVesAuib[i].ui32BufSize_SwQ = 0x0;

		gsVesAuib[i].bIsHwPath = TRUE;
		gsVesAuib[i].bFromTVP = FALSE;

		gsVesAuib[i].ui32WrOffset = 0x0;
		gsVesAuib[i].ui32RdOffset = 0x0;
		gsVesAuib[i].ui32RdOffsetForTsMatch = 0x0;
		gsVesAuib[i].ui32UidCounter = 0;
		gsVesAuib[i].ui32PrevPts = UNKNOWN_PTS;
		gsVesAuib[i].ui64PrevTs = -1;

		init_waitqueue_head(&gsVesAuib[i].wq);
	}
}

/**
********************************************************************************
* @brief
* gives wait queue of ves auib
* @remarks
* gives wait queue head of ves auib
* its for vdec es flow handling
* @par requirements:
* int
* @param
* ves channel number
* @return
* wait queue head
********************************************************************************
*/
wait_queue_head_t *VES_AUIB_GetWaitQueueHead(UINT8 ui8VesCh)
{
	return &gsVesAuib[ui8VesCh].wq;
}

/**
********************************************************************************
* @brief
* allocating auib memory pool
* @remarks
* ves auib is using vdec allocator(hma).
* this function allocates memory inputed buffer size.
* @par requirements:
* int
* @param
* buffer size
* @return
* alloced physical address
********************************************************************************
*/
static UINT32 _VES_AUIB_Alloc(UINT32 ui32BufSize)
{
	UINT32		i;
	UINT32		ui32BufAddr;

	for( i = 0; i < VES_AUIB_NUM_OF_MEM; i++ )
	{
		if( gsAuibMem[i].bUsed == FALSE )
		{
			break;
		}
	}

	if( i == VES_AUIB_NUM_OF_MEM )
	{
		logm_error(vdec_ves, "[AUIB] Num of AUIB Pool, Request Buf Size: 0x%X\n", ui32BufSize );
		for( i = 0; i < VES_AUIB_NUM_OF_MEM; i++ )
			logm_error(vdec_ves, "[AUIB] Used:%d, BufAddr:0x%X, BufSize:0x%X\n", gsAuibMem[i].bUsed, gsAuibMem[i].ui32BufAddr, gsAuibMem[i].ui32BufSize );
		return 0x0;
	}

	ui32BufAddr = vdec_alloc ( ui32BufSize + PDEC_AUI_SIZE, 1<<12, "auib");
	if( ui32BufAddr == 0x0 )
	{
		logm_error(vdec_ves, "[AUIB] Failed to mem alloc, Request Buf Size: 0x%X\n", ui32BufSize );
		for( i = 0; i < VES_AUIB_NUM_OF_MEM; i++ )
			logm_error(vdec_ves, "[AUIB] Used:%d, BufAddr:0x%X, BufSize:0x%X\n", gsAuibMem[i].bUsed, gsAuibMem[i].ui32BufAddr, gsAuibMem[i].ui32BufSize );
		return 0x0;
	}
	ui32BufAddr = PDEC_AUIB_CEILING_16BYTES( ui32BufAddr );

	gsAuibMem[i].bUsed = TRUE;
	gsAuibMem[i].ui32BufAddr = ui32BufAddr;
	gsAuibMem[i].ui32BufSize = ui32BufSize;

	logm_noti(vdec_ves, "[AUIB] Alloced Addr: 0x%X, Size: 0x%X\n", ui32BufAddr, ui32BufSize);

	return ui32BufAddr;
}

/**
********************************************************************************
* @brief
* free auib memory pool
* @remarks
* free auib memory with address, size.
* @par requirements:
* int, int
* @param
* auib pool address, pool size
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
static BOOLEAN _VES_AUIB_Free(UINT32 ui32BufAddr, UINT32 ui32BufSize)
{
	UINT32		i;

	for( i = 0; i < VES_AUIB_NUM_OF_MEM; i++ )
	{
		if( (gsAuibMem[i].bUsed == TRUE) &&
			(gsAuibMem[i].ui32BufAddr == ui32BufAddr) &&
			(gsAuibMem[i].ui32BufSize == ui32BufSize) )
		{
			vdec_free( ui32BufAddr );

			gsAuibMem[i].bUsed = FALSE;

			logm_noti(vdec_ves, "[AUIB] Free Addr: 0x%X, Size: 0x%X\n", ui32BufAddr, ui32BufSize);

			return TRUE;
		}
	}

	logm_error(vdec_ves, "[AUIB] Not Matched AUIB Pool - 0x%X/0x%X \n", ui32BufAddr, ui32BufSize );

	return FALSE;
}

/**
********************************************************************************
* @brief
* update auib write pointer
* @remarks
* update quib write pointer
* @par requirements:
* int
* @param
* physical address of write pointer
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_UpdateWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr)
{
    gsVesAuib[ui8VesCh].ui32WrOffset = ui32WrPhyAddr - gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ;
}

/**
********************************************************************************
* @brief
* reset ves auib module
* @remarks
* reset ves auib's global variables
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_Reset(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return;
	}

	logm_noti(vdec_ves, "[AUIB%d] reset all auib's db \n", ui8VesCh );

	// for Debug
	gsVesAuib[ui8VesCh].ui32WrOffset = 0;
	gsVesAuib[ui8VesCh].ui32RdOffset = 0;
	gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch = 0;
	gsVesAuib[ui8VesCh].ui32UidCounter = 0;
	gsVesAuib[ui8VesCh].ui32PrevPts = UNKNOWN_PTS;
	gsVesAuib[ui8VesCh].ui64PrevTs = -1;

	if (gsVesAuib[ui8VesCh].bIsHwPath)
	{
		PDEC_HAL_AUIB_Init(gsVesAuib[ui8VesCh].ui8PdecCh,
				gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
	}
}

/**
********************************************************************************
* @brief
* flushing ves auib channel
* @remarks
* move read pointer to current write pointer.
* it doesn't destroy cpb or auib.
* just moving pointer.
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_Flush(UINT8 ui8VesCh)
{
	UINT32	ui32WrOffset;
	UINT32	ui32RdOffset;
	UINT32  ui32RdOffsetForTs;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;
	ui32RdOffsetForTs = gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch;

	logm_noti(vdec_ves, "[AUIB%d] AUIB will be flushed WrOffset:%X, RdOffset:%X, RdforTs:%X, BufSize:%X, \n"
			, ui8VesCh, ui32WrOffset, ui32RdOffset, ui32RdOffsetForTs, gsVesAuib[ui8VesCh].ui32BufSize_HwQ );

	gsVesAuib[ui8VesCh].ui32RdOffset = ui32WrOffset;
	gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch = ui32WrOffset;

    gsVesAuib[ui8VesCh].ui32PrevPts = UNKNOWN_PTS;
    gsVesAuib[ui8VesCh].ui64PrevTs = UNKNOWN_PTS;

    wake_up (&gsVesAuib[ui8VesCh].wq);
}

/**
********************************************************************************
* @brief
* setup ves auib's database to playing
* @remarks
* if channel starts, alloced memory pool will remap to virtual address.
* else remapped memory will unmap.
* several setting value from ves will save at auib's db.
* @par requirements:
* int, int, int, int, int, int, bool, bool
* @param
* ves channel number, pdec channel number, auib pool address of hw queue,
* auib pool size of hw queue, auib pool address of sw queue,
* auib pool size of sw queue, hw path or not, tvp or not
* @return
* NONE
********************************************************************************
*/
static void _VES_AUIB_SetDb(UINT8 ui8VesCh, UINT8 ui8PdecCh, UINT32 ui32AuibBufAddr_HwQ, UINT32 ui32AuibBufSize_HwQ, UINT32 ui32AuibBufAddr_SwQ, UINT32 ui32AuibBufSize_SwQ, BOOLEAN bIsHwPath, BOOLEAN bFromTVP, E_PDEC_CODEC_TYPE_T eCodecType)
{
	logm_noti(vdec_ves, "[AUIB%d] AUIB Hw Base: 0x%X, Size: 0x%X, Sw Base: 0x%X, Size: 0x%X \n", ui8VesCh, ui32AuibBufAddr_HwQ, ui32AuibBufSize_HwQ, ui32AuibBufAddr_SwQ, ui32AuibBufSize_SwQ);

	if( ui32AuibBufAddr_HwQ && ui32AuibBufSize_HwQ && ui32AuibBufAddr_SwQ && ui32AuibBufSize_SwQ )
	{
		gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ = vdec_remap(ui32AuibBufAddr_HwQ, ui32AuibBufSize_HwQ);
		gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ = vdec_remap(ui32AuibBufAddr_SwQ, ui32AuibBufSize_SwQ);
	}
	else
	{
		vdec_unmap((void *)gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ);
		vdec_unmap((void *)gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ);
		gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ = 0x0;
		gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ = 0x0;
	}

	gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ = ui32AuibBufAddr_HwQ;
	gsVesAuib[ui8VesCh].ui32BufSize_HwQ = ui32AuibBufSize_HwQ;
	gsVesAuib[ui8VesCh].ui32PhyBaseAddr_SwQ = ui32AuibBufAddr_SwQ;
	gsVesAuib[ui8VesCh].ui32BufSize_SwQ = ui32AuibBufSize_SwQ;

	gsVesAuib[ui8VesCh].bIsHwPath = bIsHwPath;
	gsVesAuib[ui8VesCh].bFromTVP = bFromTVP;
	gsVesAuib[ui8VesCh].ui8PdecCh = ui8PdecCh;

	gsVesAuib[ui8VesCh].ui32WrOffset = 0;
	gsVesAuib[ui8VesCh].ui32RdOffset = 0;
	gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch = 0;

	gsVesAuib[ui8VesCh].eCodecType = eCodecType;

	if( bIsHwPath == TRUE )
		PDEC_HAL_AUIB_SetAuibVirBase(gsVesAuib[ui8VesCh].ui8PdecCh, gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ);

}


/**
********************************************************************************
* @brief
* is current au type picture or not
* @par requirements:
* int, E_VES_AU_T
* @param
* ves channel number, au type
* @return
* TRUE, if picture, FALSE, else
********************************************************************************
*/
static BOOLEAN _VES_AUIB_IsPicture(UINT8 ui8VesCh, E_VES_AU_T eAuType)
{

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh);
		return FALSE;
	}

	if( gsVesAuib[ui8VesCh].eCodecType == PDEC_CODEC_HEVC )
	{
		if( (E_PDEC_HEVC_AU_T)eAuType == AU_HEVC_SLICE_SEGMENT )
			return TRUE;
	}
	else
	{
		if( eAuType >= AU_PICTURE_I && eAuType <= AU_PICTURE_B_SKIP )
			return TRUE;
	}
	return FALSE;
}

/**
********************************************************************************
* @brief
* opening one ves auib channel.
* @remarks
* when opens auib channel,
* first allocating auib memory pool both hw queue and sw queue.
* second, setting values from ves and remapping physical memory to virtual.
* if hw path and tvp path, disables pdec and just using memory pool for that path.
* else if its live path, initializing pdec auib & enabling interrupts.
* @par requirements:
* int, int, bool, bool
* @param
* ves channel number, pdec channel number, hw path or not, tvp path or not
* @return
* TRUE, succeeded or processing
* FALSE, failed at least one job
********************************************************************************
*/
BOOLEAN VES_AUIB_Open(UINT8 ui8VesCh, UINT8 ui8PdecCh, BOOLEAN bIsHwPath, BOOLEAN bFromTVP, E_PDEC_CODEC_TYPE_T eCodecType)
{
	UINT32 		ui32AuibBufAddr_HwQ;
	UINT32 		ui32AuibBufAddr_SwQ;
	UINT32 		ui32AuibBufSize_HwQ = VES_AUIB_NUM_OF_NODE * PDEC_AUI_SIZE;
	UINT32 		ui32AuibBufSize_SwQ = VES_AUIB_NUM_OF_NODE * sizeof(S_SW_AUIB_T);

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return FALSE;
	}

	ui32AuibBufAddr_HwQ = _VES_AUIB_Alloc( ui32AuibBufSize_HwQ );
	if( ui32AuibBufAddr_HwQ == 0x0 )
	{
		logm_error(vdec_ves, "[AUIB%d] HW AUIB Queue Alloc Error \n", ui8VesCh );
		return FALSE;
	}
	ui32AuibBufAddr_SwQ = _VES_AUIB_Alloc( ui32AuibBufSize_SwQ );
	if( ui32AuibBufAddr_SwQ == 0x0 )
	{
		logm_error(vdec_ves, "[AUIB%d] SW AUIB Queue Alloc Error \n", ui8VesCh );
		_VES_AUIB_Free(ui32AuibBufAddr_HwQ, ui32AuibBufSize_HwQ);
		return FALSE;
	}
	logm_noti(vdec_ves, "[AUIB%d] AUIB Open: # of Node: %d \n", ui8VesCh, ui32AuibBufSize_HwQ/PDEC_AUI_SIZE);

	_VES_AUIB_SetDb(ui8VesCh, ui8PdecCh, ui32AuibBufAddr_HwQ, ui32AuibBufSize_HwQ, ui32AuibBufAddr_SwQ, ui32AuibBufSize_SwQ, bIsHwPath, bFromTVP, eCodecType);

	if( bIsHwPath == TRUE )
	{
		PDEC_HAL_AUIB_Init(ui8PdecCh, ui32AuibBufAddr_HwQ, ui32AuibBufSize_HwQ);
		PDEC_HAL_AUIB_SetBufALevel(ui8PdecCh, 80, 1);

		switch( ui8PdecCh )
		{
			case 0 :
				TOP_HAL_EnableBufIntr(PDEC0_AUB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC0_AUB_ALMOST_EMPTY);
				break;
			case 1 :
				TOP_HAL_EnableBufIntr(PDEC1_AUB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC1_AUB_ALMOST_EMPTY);
				break;
			case 2 :
				TOP_HAL_EnableBufIntr(PDEC2_AUB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC2_AUB_ALMOST_EMPTY);
				break;
			default :
				logm_error(vdec_ves, "[AUIB%d] pdec ch wrong:%d \n", ui8VesCh,ui8PdecCh);
		}
	}

	return TRUE;
}

/**
********************************************************************************
* @brief
* closing one ves auib channel
* @remarks
* if live path, initializing pdec auib hw and disables pdec auib interrupts
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_Close(UINT8 ui8VesCh)
{
	UINT8 ui8PdecCh;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return;
	}

	if( gsVesAuib[ui8VesCh].bIsHwPath == TRUE )
	{
		ui8PdecCh = gsVesAuib[ui8VesCh].ui8PdecCh;

		PDEC_HAL_AUIB_Init(ui8PdecCh, 0x0, 0x0);

		switch( ui8PdecCh )
		{
		case 0 :
			TOP_HAL_ClearBufIntr(PDEC0_AUB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC0_AUB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC0_AUB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC0_AUB_ALMOST_EMPTY);
			break;
		case 1 :
			TOP_HAL_ClearBufIntr(PDEC1_AUB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC1_AUB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC1_AUB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC1_AUB_ALMOST_EMPTY);
			break;
		case 2 :
			TOP_HAL_ClearBufIntr(PDEC2_AUB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC2_AUB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC2_AUB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC2_AUB_ALMOST_EMPTY);
			break;
		default :
			logm_error(vdec_ves, "[AUIB%d] pdec ch wrong:%d \n", ui8VesCh,ui8PdecCh);
		}
	}

	_VES_AUIB_Free(gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
	_VES_AUIB_Free(gsVesAuib[ui8VesCh].ui32PhyBaseAddr_SwQ, gsVesAuib[ui8VesCh].ui32BufSize_SwQ);

	_VES_AUIB_SetDb(ui8VesCh, 0, 0x0, 0x0, 0x0, 0x0, FALSE, FALSE, PDEC_CODEC_INVALID);

	logm_noti(vdec_ves, "[AUIB%d] Close \n", ui8VesCh );
}

/**
********************************************************************************
* @brief
* pushing one au information to aui buffer
* @remarks
* this function is used by fileplay path when update buffer ioctl called.
* mandatory information : timestamp, au start address, au size
* @par requirements:
* int, S_VES_AU_T*
* @param
* ves channel number, au structure for saving information
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_AUIB_Push(UINT8 ui8VesCh, S_VES_AU_T *pVesAu)
{
	UINT32		ui32WrOffset, ui32WrOffset_Next;
	UINT32		ui32RdOffset;
	UINT32 		ui32AU_Info0;
	UINT32 		*pui32WrVirPtr_HwQ;
	S_SW_AUIB_T 	*psWrVirPtr_SwQ;
	UINT32		ui32WrIndex;
	UINT32		ui32AuType;


	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return FALSE;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;

	ui32WrOffset_Next = PDEC_AUIB_NEXT_START_OFFSET(ui32WrOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
	if( ui32WrOffset_Next == ui32RdOffset )
	{
		logm_error(vdec_ves, "[AUIB%d] Overflow\n", ui8VesCh );
		return FALSE;
	}

	pui32WrVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32WrOffset>>2);
	psWrVirPtr_SwQ = (S_SW_AUIB_T *)gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ;

	ui32AuType = pVesAu->eAuType;
	if( ui32AuType & ~0xF )
		logm_warning(vdec_ves,  "[AUIB] Au Type Err? 0x%X\n", ui32AuType );
	ui32AuType &= 0xF;

	ui32WrIndex = ui32WrOffset / PDEC_AUI_SIZE;
	psWrVirPtr_SwQ[ui32WrIndex].ui64TimeStamp = pVesAu->ui64TimeStamp;
	psWrVirPtr_SwQ[ui32WrIndex].ui32UId = pVesAu->ui32UId;
	psWrVirPtr_SwQ[ui32WrIndex].b512bytesAligned = pVesAu->b512bytesAligned;
	psWrVirPtr_SwQ[ui32WrIndex].ui32AuSize = pVesAu->ui32AuSize;

	if( pVesAu->b512bytesAligned == TRUE )
	{
		if( pVesAu->ui32AuStartAddr & 0x1FF ) // check 512 bytes align
			logm_error(vdec_ves,  "[AUIB] AU Start Address: 0x%X is not aligned\n", pVesAu->ui32AuStartAddr );
		pVesAu->ui32AuStartAddr &= 0xFFFFFE00;

		// save original start address at software queue
		psWrVirPtr_SwQ[ui32WrIndex].ui32AuStartAddr = pVesAu->ui32AuStartAddr;

		ui32AU_Info0 = pVesAu->ui32AuStartAddr | (ui32AuType << 0);
	}
	else
	{
		psWrVirPtr_SwQ[ui32WrIndex].ui32AuStartAddr = pVesAu->ui32AuStartAddr;

		ui32AU_Info0 = ui32AuType << 0;
	}
	pui32WrVirPtr_HwQ[0] = ui32AU_Info0;
	pui32WrVirPtr_HwQ[1] = pVesAu->ui32DTS;
	pui32WrVirPtr_HwQ[2] = pVesAu->ui32PTS;
	pui32WrVirPtr_HwQ[3] = pVesAu->ui32AuEndAddr;

	/*logm_debug(vdec_ves, "[AUIB%d] AUIB Pushed - Type:%d, TS:%llu, Start:%x, End:%x \n",ui8VesCh
			,ui32AuType,psWrVirPtr_SwQ[ui32WrIndex].ui64TimeStamp, psWrVirPtr_SwQ[ui32WrIndex].ui32AuStartAddr
			,pui32WrVirPtr_HwQ[3]);*/

	gsVesAuib[ui8VesCh].ui32WrOffset = ui32WrOffset_Next;

	return TRUE;
}

/**
********************************************************************************
* @brief
*  common interface for both file play and broadcast
* @remarks
*  this function is used by vdc for timestamp matching.
*  vdc will get au information and comparing au end address and decoder's read pointer.
* @par requirements:
* int, S_VES_AU_T*
* @param
* ves channel number, au structure for saving information
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_AUIB_GetAUInfo(UINT8 ui8VesCh, S_VES_AU_T *pVesAu)
{
	UINT32 		*pui32RdVirPtr_HwQ;
	S_SW_AUIB_T 	*psRdVirPtr_SwQ;
	UINT32 		ui32AU_Info0;
	UINT32		ui32WrOffset;
	UINT32		ui32RdIndex;
	UINT32		ui32RdOffset, ui32RdOffset_Next = 0xFFFFFFFF;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return FALSE;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch;
	if( ui32WrOffset == ui32RdOffset )
	{
		logm_error(vdec_ves, "[AUIB%d] Underrun \n", ui8VesCh);
		return FALSE;
	}

	pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset>>2);
	psRdVirPtr_SwQ = (S_SW_AUIB_T *)gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ;
	ui32AU_Info0 = pui32RdVirPtr_HwQ[0];
	pVesAu->eAuType = (E_VES_AU_T)ui32AU_Info0 & 0x0000000F;

	if( gsVesAuib[ui8VesCh].bIsHwPath == TRUE )
	{
		// 2: for End Address
		ui32RdOffset_Next = PDEC_AUIB_NEXT_START_OFFSET(ui32RdOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
		if( ui32WrOffset == ui32RdOffset_Next )
		{
			logm_error(vdec_ves, "[AUIB%d] Underrun 2 \n", ui8VesCh);
			return FALSE;
		}

		pVesAu->ui32AuStartAddr = ui32AU_Info0 & 0xFFFFFE00;
		if( pui32RdVirPtr_HwQ[2] != UNKNOWN_PTS )
			pVesAu->ui32PTS = pui32RdVirPtr_HwQ[2];
		else
			pVesAu->ui32PTS = UNKNOWN_PTS;
		pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset_Next>>2);
		ui32AU_Info0 = pui32RdVirPtr_HwQ[0];
		pVesAu->ui32AuEndAddr = ui32AU_Info0 & 0xFFFFFE00;

	}
	else
	{
		pVesAu->ui32AuEndAddr = pui32RdVirPtr_HwQ[3];
		ui32RdIndex = ui32RdOffset / PDEC_AUI_SIZE;
		pVesAu->ui64TimeStamp = psRdVirPtr_SwQ[ui32RdIndex].ui64TimeStamp;
		if( psRdVirPtr_SwQ[ui32RdIndex].b512bytesAligned == TRUE )
			pVesAu->ui32AuStartAddr =  ui32AU_Info0 & 0xFFFFFE00;
		else
		{
			pVesAu->ui32AuStartAddr =  psRdVirPtr_SwQ[ui32RdIndex].ui32AuStartAddr;
		}
		pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset>>2);
	}
	return TRUE;
}

/**
********************************************************************************
* @brief
* checking au type
* @remarks
* this function is used by vdc for checking feed condition.*
* @par requirements:
* int, E_VES_AU_T*
* @param
* ves channel number, au type pointer
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_AUIB_CheckAuType(UINT8 ui8VesCh, E_VES_AU_T *eAuType)
{
	UINT32 		ui32WrOffset;
	UINT32 		ui32RdOffset = 0xFFFFFFFF;
	UINT32 		ui32AU_Info0;
	UINT32 		*pui32RdVirPtr_HwQ;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return FALSE;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;

	if( ui32WrOffset == ui32RdOffset )
	{
		logm_error(vdec_ves, "[AUIB%d][Warning][AUIB] Underrun \n", ui8VesCh);
		return FALSE;
	}

	pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset>>2);
	ui32AU_Info0 = pui32RdVirPtr_HwQ[0];
	*eAuType = (E_VES_AU_T)ui32AU_Info0 & 0x0000000F;

	return TRUE;
}

/**
********************************************************************************
* @brief
* pop au to feed decoder
* @remarks
* this function is core of ves auib.
* both live and fileplay are using this function.
* @par requirements:
* int, E_VES_AU_T*
* @param
* ves channel number, au type pointer
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_AUIB_Pop(UINT8 ui8VesCh, S_VES_AU_T *pVesAu)
{
	UINT32 		ui32WrOffset;
	UINT32 		ui32RdOffset, ui32RdOffset_Next = 0xFFFFFFFF;
	UINT32 		*pui32RdVirPtr_HwQ;
	S_SW_AUIB_T 	*psRdVirPtr_SwQ;
	UINT32 		ui32AU_Info0;
	UINT32		ui32RdIndex;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d] \n", ui8VesCh );
		return FALSE;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;

	if( ui32WrOffset == ui32RdOffset )
	{
		logm_error(vdec_ves, "[AUIB%d] Underrun \n", ui8VesCh);
		return FALSE;
	}

	pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset>>2);
	psRdVirPtr_SwQ = (S_SW_AUIB_T *)gsVesAuib[ui8VesCh].pui32VirBasePtr_SwQ;
	ui32AU_Info0 = pui32RdVirPtr_HwQ[0];
	pVesAu->eAuType = (E_VES_AU_T)ui32AU_Info0 & 0x0000000F;
	pVesAu->bStccDiscontinuity = (ui32AU_Info0 & 0x0000080) >> 7;
	pVesAu->bWrapAround = (ui32AU_Info0 & 0x0000010) >> 4;

	if( gsVesAuib[ui8VesCh].bIsHwPath == TRUE )
	{
		// 2: for End Address
		ui32RdOffset_Next = PDEC_AUIB_NEXT_START_OFFSET(ui32RdOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
		if( ui32WrOffset == ui32RdOffset_Next )
		{
			logm_error(vdec_ves, "[AUIB%d] Underrun 2 \n", ui8VesCh);
			return FALSE;
		}

		pVesAu->ui32AuStartAddr = ui32AU_Info0 & 0xFFFFFE00;
		if( pVesAu->ui32AuStartAddr < VES_CPB_GetBufferBaseAddr(ui8VesCh) ) {
			logm_error(vdec_ves, "[AUIB%d] AU Start Addr: 0x%X, Base Addr: 0x%X\n", ui8VesCh, pVesAu->ui32AuStartAddr, VES_CPB_GetBufferBaseAddr(ui8VesCh) );
			return FALSE;
		}

		pVesAu->bCHB_Valid = (ui32AU_Info0 & 0x0000100) >> 8;
		pVesAu->bGopHeader = (ui32AU_Info0 & 0x0000020) >> 5;
		pVesAu->ui32DTS = pui32RdVirPtr_HwQ[1];
		pVesAu->ui32PTS = pui32RdVirPtr_HwQ[2];
		pVesAu->DTS_PTS_flag = ((pui32RdVirPtr_HwQ[3] & 0xC0000000) >> 30);
		pVesAu->ui32AUPaircheck = (pui32RdVirPtr_HwQ[3] & 0x00000001);

		//logm_noti(vdec_ves, "[AUIB%d][AUIB] dump Hw Queue 0:%x 1:%x 2:%x 3:%x \n",
		//		ui8VesCh,pui32RdVirPtr_HwQ[0], pui32RdVirPtr_HwQ[1], pui32RdVirPtr_HwQ[2], pui32RdVirPtr_HwQ[3]);

		pVesAu->ui64TimeStamp = 0;
		pVesAu->ui32UId = gsVesAuib[ui8VesCh].ui32UidCounter++;

		pVesAu->b512bytesAligned = TRUE;

		if( _VES_AUIB_IsPicture(ui8VesCh, pVesAu->eAuType) ) {
			if( pVesAu->ui32PTS == gsVesAuib[ui8VesCh].ui32PrevPts ) {
				logm_info(vdec_ves, "[AUIB%d] Ignored duplicated pts:%9u(0x%08X) \n",ui8VesCh,pVesAu->ui32PTS,pVesAu->ui32PTS);
				pVesAu->ui32PTS = pui32RdVirPtr_HwQ[2] = UNKNOWN_PTS;
			}
			else {
				gsVesAuib[ui8VesCh].ui32PrevPts = pVesAu->ui32PTS;
			}
		}

		pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset_Next>>2);
		ui32AU_Info0 = pui32RdVirPtr_HwQ[0];
		pVesAu->ui32AuEndAddr = ui32AU_Info0 & 0xFFFFFE00;

		if( pVesAu->ui32AuEndAddr >= pVesAu->ui32AuStartAddr )
			pVesAu->ui32AuSize = pVesAu->ui32AuEndAddr - pVesAu->ui32AuStartAddr;
		else
			pVesAu->ui32AuSize = pVesAu->ui32AuEndAddr + VES_CPB_GetBufferSize(ui8VesCh) - pVesAu->ui32AuStartAddr;

		logm_debug(vdec_ves, "[AUIB%d] Popped(L) - Type:%d, PTS:%9u, Start:0x%8X, End:0x%8X, CPBWr:0x%8X, UId:%d \n"
				,ui8VesCh,pVesAu->eAuType,pVesAu->ui32PTS,pVesAu->ui32AuStartAddr
				,pVesAu->ui32AuEndAddr,VES_CPB_GetPhyWrPtr(ui8VesCh),pVesAu->ui32UId);
	}
	else
	{
		pVesAu->ui32AuEndAddr = pui32RdVirPtr_HwQ[3];
		pVesAu->ui32DTS = pui32RdVirPtr_HwQ[1];
		pVesAu->ui32PTS = pui32RdVirPtr_HwQ[2];
		pVesAu->bWrapAround = FALSE;

		ui32RdIndex = ui32RdOffset / PDEC_AUI_SIZE;
		pVesAu->ui64TimeStamp = psRdVirPtr_SwQ[ui32RdIndex].ui64TimeStamp;

		if( pVesAu->eAuType != AU_SEQUENCE_HEADER ) {
			if( unlikely(pVesAu->ui64TimeStamp == gsVesAuib[ui8VesCh].ui64PrevTs) ) {
				logm_noti(vdec_ves, "[AUIB%d] Ignored duplicated ts:%lld \n"
						,ui8VesCh,pVesAu->ui64TimeStamp);
				pVesAu->ui64TimeStamp = psRdVirPtr_SwQ[ui32RdIndex].ui64TimeStamp = UNKNOWN_PTS;
			}
			else {
				gsVesAuib[ui8VesCh].ui64PrevTs = pVesAu->ui64TimeStamp;
			}
		}
		//pVesAu->ui32UId = psRdVirPtr_SwQ[ui32RdIndex].ui32UId;
		pVesAu->ui32UId = gsVesAuib[ui8VesCh].ui32UidCounter++;
		pVesAu->b512bytesAligned = psRdVirPtr_SwQ[ui32RdIndex].b512bytesAligned;
		pVesAu->ui32AuSize = psRdVirPtr_SwQ[ui32RdIndex].ui32AuSize;
		if( pVesAu->b512bytesAligned == TRUE )
			pVesAu->ui32AuStartAddr =  ui32AU_Info0 & 0xFFFFFE00;
		else
		{
			pVesAu->ui32AuStartAddr =  psRdVirPtr_SwQ[ui32RdIndex].ui32AuStartAddr;
		}

		pui32RdVirPtr_HwQ = gsVesAuib[ui8VesCh].pui32VirBasePtr_HwQ + (ui32RdOffset>>2);

		logm_debug(vdec_ves, "[AUIB%d] Popped(F) - Type:%d, TS:%9llu, Start:0x%8x, End:0x%8x, CPBWr:0x%8x\n"
				,ui8VesCh,pVesAu->eAuType,pVesAu->ui64TimeStamp ,pVesAu->ui32AuStartAddr
				,pVesAu->ui32AuEndAddr,VES_CPB_GetPhyWrPtr(ui8VesCh));

		//wake_up (&gsVesAuib[ui8VesCh].wq);
	}

	return TRUE;
}

/**
********************************************************************************
* @brief
* update aui read pointer for popped au
* @remarks
* if au are successfully parsed, update read pointer to pop next one
* @par requirements:
* int, bool
* @param
* ves channel number, feed pop or ts match pop
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_ConsumeOneAU(UINT8 ui8VesCh, BOOLEAN bFeedPop)
{
	UINT32 ui32RdOffset;
	UINT32 ui32RdOffset_Next;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d][Err] \n", ui8VesCh );
		return;
	}

	if( bFeedPop )
		ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;
	else
		ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch;

	ui32RdOffset_Next = PDEC_AUIB_NEXT_START_OFFSET(ui32RdOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);

	if( bFeedPop )
		gsVesAuib[ui8VesCh].ui32RdOffset = ui32RdOffset_Next;
	else
		gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch = ui32RdOffset_Next;

	if(gsVesAuib[ui8VesCh].bIsHwPath && bFeedPop)
	{
		ui32RdOffset_Next = gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ + ui32RdOffset_Next;
		PDEC_HAL_AUIB_SetRdPtr(gsVesAuib[ui8VesCh].ui8PdecCh, ui32RdOffset_Next);
	}

	if( bFeedPop )
		wake_up (&gsVesAuib[ui8VesCh].wq);
}

/**
********************************************************************************
* @brief
* restoring aui read pointer to reuse current au
* @remarks
* if au are not enough to used, restroing it and pop it next time
* @par requirements:
* int, bool
* @param
* ves channel number, feed pop or ts match pop
* @return
* NONE
********************************************************************************
*/
void VES_AUIB_RestoreOneAU(UINT8 ui8VesCh, BOOLEAN bFeedPop)
{
	UINT32 ui32RdOffset;
	UINT32 ui32RdOffset_Prev;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d][Err] \n", ui8VesCh );
		return;
	}

	if( bFeedPop )
		ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;
	else
		ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch;

	ui32RdOffset_Prev = PDEC_AUIB_PREV_START_OFFSET(ui32RdOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);

	if( bFeedPop )
		gsVesAuib[ui8VesCh].ui32RdOffset = ui32RdOffset_Prev;
	else
		gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch = ui32RdOffset_Prev;

	if( bFeedPop && gsVesAuib[ui8VesCh].bIsHwPath )
	{
		ui32RdOffset_Prev = gsVesAuib[ui8VesCh].ui32PhyBaseAddr_HwQ + ui32RdOffset_Prev;
		PDEC_HAL_AUIB_SetRdPtr(gsVesAuib[ui8VesCh].ui8PdecCh, ui32RdOffset_Prev);
	}
	else if( bFeedPop && !gsVesAuib[ui8VesCh].bIsHwPath )
	{
		wake_up (&gsVesAuib[ui8VesCh].wq);
	}
}

/**
********************************************************************************
* @brief
* report number of usable au
* @remarks
* report number of usable au in buffer
* sometimes even if one frame of es doesn't fully write on cpb,
* au detected interrupt occurs.
* for that case, inserting routine for checking current au's es data fully writed on cpb
* @par requirements:
* int
* @param
* ves channel number
* @return
* number of usable au
********************************************************************************
*/
UINT32 VES_AUIB_NumActive(UINT8 ui8VesCh)
{
	UINT32	ui32UsedBuf;
	UINT32	ui32WrOffset;
	UINT32	ui32RdOffset;
	UINT32  ui32NumActive;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d][Err] \n", ui8VesCh );
		return 0;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;

	if( gsVesAuib[ui8VesCh].bIsHwPath == TRUE && ui32WrOffset != ui32RdOffset )
	{
		ui32WrOffset = PDEC_AUIB_PREV_START_OFFSET(ui32WrOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
	}

	if( ui32WrOffset >= ui32RdOffset )
		ui32UsedBuf = ui32WrOffset - ui32RdOffset;
	else
		ui32UsedBuf = ui32WrOffset + gsVesAuib[ui8VesCh].ui32BufSize_HwQ - ui32RdOffset;

	ui32NumActive = ui32UsedBuf / PDEC_AUI_SIZE;

	return ui32NumActive;
}

/**
********************************************************************************
* @brief
* report number of au db for ts matching
* @remarks
* report number of au db for ts matching.
* we manage different read pointer about popping and ts matching,
* so divided function.
* @par requirements:
* int
* @param
* ves channel number
* @return
* number of au db for ts matching
********************************************************************************
*/
UINT32 VES_AUIB_NumActiveTsDb(UINT8 ui8VesCh)
{
	UINT32	ui32UsedBuf;
	UINT32	ui32WrOffset;
	UINT32	ui32RdOffset;
	UINT32  ui32NumActive;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d][Err] \n\n", ui8VesCh );
		return 0;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffsetForTsMatch;

	if( gsVesAuib[ui8VesCh].bIsHwPath == TRUE && ui32WrOffset != ui32RdOffset)
	{
		ui32WrOffset = PDEC_AUIB_PREV_START_OFFSET(ui32WrOffset, gsVesAuib[ui8VesCh].ui32BufSize_HwQ);
	}

	if( ui32WrOffset >= ui32RdOffset )
		ui32UsedBuf = ui32WrOffset - ui32RdOffset;
	else
		ui32UsedBuf = ui32WrOffset + gsVesAuib[ui8VesCh].ui32BufSize_HwQ - ui32RdOffset;

	ui32NumActive = ui32UsedBuf / PDEC_AUI_SIZE;

	return ui32NumActive;
}

/**
********************************************************************************
* @brief
* report number of free space in auib
* @remarks
* report number of free space in auib
* @par requirements:
* int
* @param
* ves channel number
* @return
* number of free space in auib
********************************************************************************
*/
UINT32 VES_AUIB_NumFree(UINT8 ui8VesCh)
{
	UINT32	ui32FreeBuf;
	UINT32	ui32WrOffset;
	UINT32	ui32RdOffset;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[AUIB%d][Err] \n\n", ui8VesCh );
		return 0;
	}

	ui32WrOffset = gsVesAuib[ui8VesCh].ui32WrOffset;
	ui32RdOffset = gsVesAuib[ui8VesCh].ui32RdOffset;

	if( ui32WrOffset < ui32RdOffset )
		ui32FreeBuf = ui32RdOffset - ui32WrOffset;
	else
		ui32FreeBuf = ui32RdOffset + gsVesAuib[ui8VesCh].ui32BufSize_HwQ - ui32WrOffset;

	return ((ui32FreeBuf / PDEC_AUI_SIZE) - 1);
}
