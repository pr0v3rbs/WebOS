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
#include "ves_cpb.h"

#include <linux/kernel.h>
#include <asm/uaccess.h> // copy_from_user
#include <asm/cacheflush.h>
#include <linux/version.h>
#include <linux/dma-mapping.h>

#include "hal/vdec_base.h"
#include "hal/pdec_hal_api.h"
#include "hal/top_hal_api.h"
#include "hal/ipc_reg_api.h"

#include "log.h"
/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define		VES_CPB_ALIGNED_OFFSET( _offset, _cpbsize )	\
					((VES_CPB_CEILING_512BYTES(_offset) >= (_cpbsize)) ? 0 : VES_CPB_CEILING_512BYTES(_offset))
#define		VES_CPB_ALIGNED_ADDR( _addr, _cpbbase, _cpbsize )	\
					((VES_CPB_CEILING_512BYTES(_addr) >= ((_cpbbase) + (_cpbsize))) ? (_cpbbase) : VES_CPB_CEILING_512BYTES(_addr))

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

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
static void _VES_CPB_SetDb(UINT8 ui8VesCh, UINT8 ui8PdecCh, UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize, BOOLEAN bIsHwPath, BOOLEAN bFromTVP);

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static struct
{
	UINT32 		ui32PhyBaseAddr;	// constant
	UINT8 		*pui8VirBasePtr;		// constant
	UINT32 		ui32BufSize;			// constant
	BOOLEAN		bIsHwPath;
	BOOLEAN		bFromTVP;

	//for Divx3, VP8
	UINT16 		ui16PicWidth;
	UINT16          ui16PicHeight;
	E_PDEC_CODEC_TYPE_T eCodecType;

	UINT8		ui8PdecCh;

	// for Debug
	UINT32 		ui32WrOffset;		// variable
	UINT32 		ui32RdOffset;		// variable

	UINT32		ui32SwPrevRdPtr;
	UINT32		ui32HwPrevRdPtr;

#ifdef CPB_USE_NEW_UPDATE_BUFFER
	// for Reserved CPB memory
	UINT32		ui32RsOffset;		// variable;
#endif
} gsVesCpb[VDEC_NUM_OF_CHANNEL];



/**
********************************************************************************
* @brief
* initialize ves cpb
* @remarks
* initializing global variables
* @par requirements:
* NONE
* @param
* NONE
* @return
* NONE
********************************************************************************
*/
void VES_CPB_Init(void)
{
	UINT32	i;

	for( i = 0; i < VDEC_NUM_OF_CHANNEL; i++ )
	{
		gsVesCpb[i].ui32PhyBaseAddr = 0x0;
		gsVesCpb[i].pui8VirBasePtr = NULL;
		gsVesCpb[i].ui32BufSize = 0x0;
		gsVesCpb[i].bIsHwPath = TRUE;
		gsVesCpb[i].bFromTVP = FALSE;

		gsVesCpb[i].ui32WrOffset = 0x0;
		gsVesCpb[i].ui32RdOffset = 0x0;

#ifdef CPB_USE_NEW_UPDATE_BUFFER
		gsVesCpb[i].ui32RsOffset = 0x0;
#endif
	}
}

/**
********************************************************************************
* @brief
* checking is target belong between start and end address
* @remarks
* checking is target belong between start and end address
* @par requirements:
* int, int, int
* @param
* start address, end address, target address
* @return
* TRUE, target is contained
* FALSE, else
********************************************************************************
*/
static BOOLEAN _VES_CPB_CheckBelongToAddress(UINT32 ui32StartAddr, UINT32 ui32EndAddr, UINT32 ui32TargetAddr)
{
	if( ui32StartAddr <= ui32EndAddr )
	{
		if( (ui32TargetAddr > ui32StartAddr) &&
			(ui32TargetAddr <= ui32EndAddr) )
			return TRUE;
	}
	else
	{
		if( (ui32TargetAddr > ui32StartAddr) ||
			(ui32TargetAddr <= ui32EndAddr) )
			return TRUE;
	}

	return FALSE;
}

/**
********************************************************************************
* @brief
* update ves cpb's write pointer
* @remarks
* update ves cpb's write pointer if cpb doesn't overflow
* @par requirements:
* int, int
* @param
* ves channel number, write pointer for updating
* @return
* NONE
********************************************************************************
*/
void VES_CPB_UpdateWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr)
{
	UINT32		ui32RdOffset;
	UINT32		ui32WrOffset, ui32WrOffset_Next;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return;
	}

	ui32WrOffset_Next = ui32WrPhyAddr - gsVesCpb[ui8VesCh].ui32PhyBaseAddr;

	if( ui32WrOffset_Next > gsVesCpb[ui8VesCh].ui32BufSize )
	{
		logm_error(vdec_ves, "[CPB%d] Invalid Write Addr:0x%X\n", ui8VesCh, ui32WrPhyAddr );
		return;
	}
	if( ui32WrOffset_Next == gsVesCpb[ui8VesCh].ui32BufSize )
	{
		ui32WrOffset_Next = 0;
	}

	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);
	ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);

	if( _VES_CPB_CheckBelongToAddress(ui32WrOffset, ui32WrOffset_Next, ui32RdOffset) == TRUE )
	{
		logm_error(vdec_ves, "[CPB%d] Overwrite - Write:0x%X, Write_Next:0x%X, Read:0x%X\n"
				, ui8VesCh, ui32WrOffset, ui32WrOffset_Next, ui32RdOffset );
		ui32WrOffset_Next = ui32RdOffset;
		ui32WrPhyAddr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset_Next;
	}

	logm_trace(vdec_ves, "[CPB%d] WritePtr Updated:%X -> %X \n",ui8VesCh,ui32WrOffset,ui32WrOffset_Next);

	IPC_REG_CPB_SetWrOffset(ui8VesCh, ui32WrOffset_Next);

	// for Debug
	gsVesCpb[ui8VesCh].ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	gsVesCpb[ui8VesCh].ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);
}

/**
********************************************************************************
* @brief
* update ves cpb's read pointer
* @remarks
* update ves cpb's read pointer
* @par requirements:
* int, int
* @param
* ves channel number, read pointer for updating
* @return
* NONE
********************************************************************************
*/
void VES_CPB_UpdateRdPtr(UINT8 ui8VesCh, UINT32 ui32RdPhyAddr)
{
	UINT32		ui32RdOffset, ui32RdOffset_Next;
	UINT32		ui32WrOffset;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return;
	}

	ui32RdOffset_Next = ui32RdPhyAddr - gsVesCpb[ui8VesCh].ui32PhyBaseAddr;

	if( ui32RdOffset_Next > gsVesCpb[ui8VesCh].ui32BufSize )
	{
		logm_error(vdec_ves, "[CPB%d] Invalid Read Addr:0x%X, Base Addr:0x%X\n"
				, ui8VesCh, ui32RdPhyAddr, gsVesCpb[ui8VesCh].ui32PhyBaseAddr );
		return;
	}
	if( ui32RdOffset_Next == gsVesCpb[ui8VesCh].ui32BufSize )
	{
		ui32RdOffset_Next = 0;
	}

	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);
	ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);

	if( _VES_CPB_CheckBelongToAddress(ui32RdOffset, ui32RdOffset_Next, ui32WrOffset) == TRUE )
	{
		if( ui32RdOffset_Next != ui32WrOffset )
		{
			logm_error(vdec_ves, "[CPB%d] Overread - Read:0x%X, Read_Next:0x%X, Write:0x%X\n"
					, ui8VesCh, ui32RdOffset, ui32RdOffset_Next, ui32WrOffset );
			ui32RdOffset_Next = ui32WrOffset;
			ui32RdPhyAddr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32RdOffset_Next;
		}
	}

	gsVesCpb[ui8VesCh].ui32SwPrevRdPtr = ui32RdOffset;

	logm_trace(vdec_ves, "[CPB%d] ReadPtr Updated:%X -> %X \n",ui8VesCh,ui32RdOffset,ui32RdOffset_Next);

	IPC_REG_CPB_SetRdOffset(ui8VesCh, ui32RdOffset_Next);

	// for Debug
	gsVesCpb[ui8VesCh].ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	gsVesCpb[ui8VesCh].ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);

	if( gsVesCpb[ui8VesCh].bIsHwPath == TRUE )
	{
		gsVesCpb[ui8VesCh].ui32HwPrevRdPtr = PDEC_HAL_CPB_GetRdPtr(gsVesCpb[ui8VesCh].ui8PdecCh);
		PDEC_HAL_CPB_SetRdPtr(gsVesCpb[ui8VesCh].ui8PdecCh, ui32RdPhyAddr);
	}
}

/**
********************************************************************************
* @brief
* reset ves cpb module
* @remarks
* reset ves cpb's read & write pointer.
* if live path, call pdec cpb initialzing
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_CPB_Reset(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return;
	}

	logm_noti(vdec_ves, "[CPB%d] \n", ui8VesCh);

	IPC_REG_CPB_SetWrOffset(ui8VesCh, 0);
	IPC_REG_CPB_SetRdOffset(ui8VesCh, 0);

	// for Debug
	gsVesCpb[ui8VesCh].ui32WrOffset = 0;
	gsVesCpb[ui8VesCh].ui32RdOffset = 0;

#ifdef CPB_USE_NEW_UPDATE_BUFFER
	gsVesCpb[ui8VesCh].ui32RsOffset = 0;
#endif

	if( gsVesCpb[ui8VesCh].bIsHwPath == TRUE )
	{
		PDEC_HAL_CPB_Init(gsVesCpb[ui8VesCh].ui8PdecCh, gsVesCpb[ui8VesCh].ui32PhyBaseAddr, gsVesCpb[ui8VesCh].ui32BufSize);
	}
}

/**
********************************************************************************
* @brief
* flushing ves cpb channel
* @remarks
* flushing ves cpb's read pointer to write pointer.
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_CPB_Flush(UINT8 ui8VesCh)
{
	UINT32		ui32WrOffset;
	UINT32		ui32RdOffset;
	//UINT8		*pui8CpbRdVirPtr;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return;
	}

	ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);

	logm_noti(vdec_ves, "[CPB%d][DBG] WrOffset:0x%8X, RdOffset:0x%8X, BufSize:%d\n"
			,ui8VesCh, ui32WrOffset, ui32RdOffset, gsVesCpb[ui8VesCh].ui32BufSize);

	VES_CPB_UpdateRdPtr(ui8VesCh, gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset);
//	VES_CPB_Reset(ui8VesCh);

	// no memset when cpb flushed
	/*
	if( gsVesCpb[ui8VesCh].pui8VirBasePtr != NULL )
	{ // for Ring Buffer
		pui8CpbRdVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32RdOffset;
		if( ui32WrOffset >= ui32RdOffset )
		{
			memset(pui8CpbRdVirPtr, 0x0, (ui32WrOffset - ui32RdOffset));
		}
		else
		{
			memset(pui8CpbRdVirPtr, 0x0, (gsVesCpb[ui8VesCh].ui32BufSize - ui32RdOffset));
			if( ui32WrOffset )
			{
				pui8CpbRdVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr;
				memset(pui8CpbRdVirPtr, 0x0, ui32WrOffset);
			}
		}
	}*/
}

/**
********************************************************************************
* @brief
* set ves cpb's db
* @remarks
* setting cpb's db like pdec channel number, cpb address, size.
* @par requirements:
* int, int, int, int, bool, bool
* @param
* ves channel number, pdec channel number, cpb address, cpb size, live?, tvp?
* @return
* NONE
********************************************************************************
*/
static void _VES_CPB_SetDb(UINT8 ui8VesCh, UINT8 ui8PdecCh, UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize, BOOLEAN bIsHwPath, BOOLEAN bFromTVP)
{
	logm_noti(vdec_ves, "[CPB%d] CPB Base: 0x%X, Size: 0x%X \n", ui8VesCh, ui32CpbBufAddr, ui32CpbBufSize);

	if( ui32CpbBufAddr && ui32CpbBufSize )
	{
		if( bFromTVP == FALSE )
			gsVesCpb[ui8VesCh].pui8VirBasePtr = (UINT8 *)vdec_remap(ui32CpbBufAddr, ui32CpbBufSize);
		else
			gsVesCpb[ui8VesCh].pui8VirBasePtr = NULL;
	}
	else
	{
		if( bFromTVP == FALSE )
			vdec_unmap((void *)gsVesCpb[ui8VesCh].pui8VirBasePtr );

		gsVesCpb[ui8VesCh].pui8VirBasePtr = NULL;
	}

	gsVesCpb[ui8VesCh].ui32PhyBaseAddr = ui32CpbBufAddr;
	gsVesCpb[ui8VesCh].ui32BufSize = ui32CpbBufSize;

	gsVesCpb[ui8VesCh].bIsHwPath = bIsHwPath;
	gsVesCpb[ui8VesCh].bFromTVP = bFromTVP;
	gsVesCpb[ui8VesCh].ui8PdecCh = ui8PdecCh;

	gsVesCpb[ui8VesCh].ui32WrOffset = 0;
	gsVesCpb[ui8VesCh].ui32RdOffset = 0;
	IPC_REG_CPB_SetWrOffset(ui8VesCh, 0);
	IPC_REG_CPB_SetRdOffset(ui8VesCh, 0);

#ifdef CPB_USE_NEW_UPDATE_BUFFER
	gsVesCpb[ui8VesCh].ui32RsOffset = 0;
#endif

	if( bIsHwPath == TRUE)
		PDEC_HAL_CPB_SetCpbVirBase(gsVesCpb[ui8VesCh].ui8PdecCh, gsVesCpb[ui8VesCh].pui8VirBasePtr);

}

/**
********************************************************************************
* @brief
* ves cpb channel open
* @remarks
* open one ves cpb channel.
* @par requirements:
* int, int, int, int, bool, bool
* @param
* ves channel number, pdec channel number, cpb address, cpb size, live? tvp?
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_CPB_Open(UINT8 ui8VesCh, UINT8 ui8PdecCh, UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize, BOOLEAN bIsHwPath, BOOLEAN bFromTVP, E_PDEC_CODEC_TYPE_T eCodecType)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return FALSE;
	}

	if( ui32CpbBufAddr == 0x0 )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return FALSE;
	}

	logm_noti(vdec_ves, "[CPB%d] Addr: 0x%X, Size: 0x%X\n", ui8VesCh, ui32CpbBufAddr, ui32CpbBufSize );

	ui32CpbBufAddr = VES_CPB_CEILING_512BYTES( ui32CpbBufAddr );
	ui32CpbBufSize = VES_CPB_BOTTOM_512BYTES( ui32CpbBufSize );

	gsVesCpb[ui8VesCh].eCodecType = eCodecType;

	_VES_CPB_SetDb(ui8VesCh, ui8PdecCh, ui32CpbBufAddr, ui32CpbBufSize, bIsHwPath, bFromTVP);

	if( bIsHwPath == TRUE )
	{
		PDEC_HAL_CPB_Init(ui8PdecCh, ui32CpbBufAddr, ui32CpbBufSize);

		PDEC_HAL_CPB_SetBufALevel(ui8PdecCh, 90, 2);

		switch( ui8PdecCh )
		{
			case 0 :
				TOP_HAL_EnableBufIntr(PDEC0_CPB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC0_CPB_ALMOST_EMPTY);
				break;
			case 1 :
				TOP_HAL_EnableBufIntr(PDEC1_CPB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC1_CPB_ALMOST_EMPTY);
				break;
			case 2 :
				TOP_HAL_EnableBufIntr(PDEC2_CPB_ALMOST_FULL);
				TOP_HAL_EnableBufIntr(PDEC2_CPB_ALMOST_EMPTY);
				break;
			default :
				logm_error(vdec_ves, "[CPB%d] Pdec Num Error:%d \n", ui8VesCh,ui8PdecCh);
		}
	}

	/* ??? */
	IPC_REG_VES_SetDb (bFromTVP);

	return TRUE;
}

/**
********************************************************************************
* @brief
* ves cpb channel closing
* @remarks
* closing one ves cpb channel
* @par requirements:
* int
* @param
* ves channel number
* @return
* NONE
********************************************************************************
*/
void VES_CPB_Close(UINT8 ui8VesCh)
{
	if( gsVesCpb[ui8VesCh].bIsHwPath == TRUE )
	{
		PDEC_HAL_CPB_Init(gsVesCpb[ui8VesCh].ui8PdecCh, 0x0, 0x0);

		switch( gsVesCpb[ui8VesCh].ui8PdecCh )
		{
		case 0 :
			TOP_HAL_ClearBufIntr(PDEC0_CPB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC0_CPB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC0_CPB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC0_CPB_ALMOST_EMPTY);
			break;
		case 1 :
			TOP_HAL_ClearBufIntr(PDEC1_CPB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC1_CPB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC1_CPB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC1_CPB_ALMOST_EMPTY);
			break;
		case 2 :
			TOP_HAL_ClearBufIntr(PDEC2_CPB_ALMOST_FULL);
			TOP_HAL_DisableBufIntr(PDEC2_CPB_ALMOST_FULL);
			TOP_HAL_ClearBufIntr(PDEC2_CPB_ALMOST_EMPTY);
			TOP_HAL_DisableBufIntr(PDEC2_CPB_ALMOST_EMPTY);
			break;
		default :
			logm_error(vdec_ves, "[CPB%d] PDEC Num Error:%d \n", ui8VesCh,gsVesCpb[ui8VesCh].ui8PdecCh);
		}
	}

	_VES_CPB_SetDb(ui8VesCh,0 ,0x0, 0x0, FALSE, FALSE);

	logm_noti(vdec_ves, "[CPB%d] Close \n",ui8VesCh);
}

/**
********************************************************************************
* @brief
* get current cpb's write pointer
* @remarks
* get current cpb's write pointer
* @par requirements:
* int
* @param
* ves channel number
* @return
* current cpb's write pointer
********************************************************************************
*/
UINT32 VES_CPB_GetPhyWrPtr(UINT8 ui8VesCh)
{
	UINT32		ui32WrPhyAddr;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0x0;
	}

	if( !gsVesCpb[ui8VesCh].ui32PhyBaseAddr )
	{
		logm_warning(vdec_ves, "[CPB%d] Not Initialised\n", ui8VesCh);
		return 0x0;
	}

	ui32WrPhyAddr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + IPC_REG_CPB_GetWrOffset(ui8VesCh);

	return ui32WrPhyAddr;
}

/**
********************************************************************************
* @brief
* get current cpb's read pointer
* @remarks
* get current cpb's read pointer
* @par requirements:
* int
* @param
* ves channel number
* @return
* current cpb's read pointer
********************************************************************************
*/
UINT32 VES_CPB_GetPhyRdPtr(UINT8 ui8VesCh)
{
	UINT32		ui32RdPhyAddr;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0x0;
	}

	if( !gsVesCpb[ui8VesCh].ui32PhyBaseAddr )
	{
		logm_error(vdec_ves, "[CPB%d] Not Initialised\n", ui8VesCh);
		return 0x0;
	}

	if( gsVesCpb[ui8VesCh].bIsHwPath == TRUE )
		ui32RdPhyAddr = PDEC_HAL_CPB_GetRdPtr( gsVesCpb[ui8VesCh].ui8PdecCh );
	else
		ui32RdPhyAddr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + IPC_REG_CPB_GetRdOffset(ui8VesCh);

	return ui32RdPhyAddr;
}

/**
********************************************************************************
* @brief
* get current cpb's virtual write pointer
* @remarks
* get current cpb's virtual write pointer
* @par requirements:
* int, int
* @param
* ves channel number, physical address of cpb base
* @return
* pointer of cpb's virtual write pointer
********************************************************************************
*/
UINT8 *VES_CPB_TranslatePhyWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr)
{
	UINT32		ui32WrOffset;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0x0;
	}

	if( gsVesCpb[ui8VesCh].pui8VirBasePtr == NULL )
	{
		logm_error(vdec_ves, "[CPB%d] Not Initialised\n", ui8VesCh);
		return 0x0;
	}

	ui32WrOffset = ui32WrPhyAddr - gsVesCpb[ui8VesCh].ui32PhyBaseAddr;

	return gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32WrOffset;
}

/**
********************************************************************************
* @brief
* check cpb's overflow
* @remarks
* checking cpb's overflow
* @par requirements:
* int, int, int*, int*, int
* @param
* ves channel number, au size(it can be 512 bytes aligned), au start address,
* aligned end address, current cpb's read pointer
* @return
*
********************************************************************************
*/
static BOOLEAN _VES_CPB_CheckOverflow(UINT8 ui8VesCh, UINT32 ui32AuSize_Modified, UINT32 *pui32WrOffset_AuStart, UINT32 *pui32WrOffset_AlignedEnd, UINT32 ui32RdOffset)
{
	UINT32 ui32WrOffset_Org = *pui32WrOffset_AuStart;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return TRUE;
	}

	if( (*pui32WrOffset_AuStart + ui32AuSize_Modified) >= gsVesCpb[ui8VesCh].ui32BufSize )
	{
        *pui32WrOffset_AlignedEnd = (*pui32WrOffset_AuStart + ui32AuSize_Modified) - gsVesCpb[ui8VesCh].ui32BufSize;
	}
	else
	{
		*pui32WrOffset_AlignedEnd = *pui32WrOffset_AuStart + ui32AuSize_Modified;
	}

	if( _VES_CPB_CheckBelongToAddress(ui32WrOffset_Org, *pui32WrOffset_AlignedEnd, ui32RdOffset) == TRUE )
	{
		logm_error(vdec_ves, "[CPB%d] Overflow - Write:0x%X, Size:0x%X, Read:0x%X\n"
				,ui8VesCh, ui32WrOffset_Org, ui32AuSize_Modified, ui32RdOffset );
		return TRUE;
	}

	return FALSE;
}

/**
********************************************************************************
* @brief
* write es data at cpb
* @remarks
* writing es data at cpb, this function only used in fileplay
* @par requirements:
* int, int, int, int, function*, bool, int*, int*
* @param
* ves channel number, au type, au start address, au size, memory copy function,
* is 512bytes aligned, aligned au size, au end address at cpb
* @return
* au start address at cpb
********************************************************************************
*/
UINT32 VES_CPB_Write(	UINT8 ui8VesCh,
							UINT32 eAuType,
							UINT32 ui32AuStartAddr,
							UINT32 ui32AuSize,
							BOOLEAN b512bytesAligned,
							UINT32 *pui32AuSizeModified,
							UINT32 *pui32CpbWrPhyAddr_End)
{
/*
	ui32WrOffset,
				ui32WrOffset_AuStart + ui32AuSize = ui32WrOffset_AuEnd,
																ui32WrOffset_AlignedEnd
*/
	UINT32		ui32WrOffset, ui32WrOffset_AuStart, ui32WrOffset_AuEnd, ui32WrOffset_AlignedEnd;
	UINT32		ui32RdOffset;
	UINT32		ui32CpbWrPhyAddr = 0x0;
	UINT8		*pui8CpbWrVirPtr;
	char		*pui8AuStartAddr = (char*)ui32AuStartAddr;
	UINT32		ui32AuSize_Modified;
#if 0
	char		CheckStart[32] = {0, };
#endif

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0;
	}

	if( b512bytesAligned == TRUE )
		//ui32AuSize_Modified = (VES_CPB_CEILING_512BYTES(ui32AuSize) + 1024);
		ui32AuSize_Modified = VES_CPB_CEILING_512BYTES(ui32AuSize);
	else
		ui32AuSize_Modified = ui32AuSize;

	ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);
	ui32WrOffset_AuStart = ui32WrOffset;

	// 1. Check CPB Overflow
	if( _VES_CPB_CheckOverflow(ui8VesCh, ui32AuSize_Modified, &ui32WrOffset_AuStart, &ui32WrOffset_AlignedEnd, ui32RdOffset) == TRUE )
		return 0x0;

	// pad buffer end
	if( ui32WrOffset > ui32WrOffset_AuStart )
	{
		UINT32		ui32PadSize;

		logm_debug(vdec_ves, "[CPB%d] Pad CPB End, WrOffset:0x%08X/0x%08X/0x%08X, RdOffset:0x%08X\n"
				,ui8VesCh, ui32WrOffset, ui32WrOffset_AuStart, ui32WrOffset_AlignedEnd, ui32RdOffset);

		if( ui32WrOffset_AuStart != 0 )
		{
			logm_error(vdec_ves, "[CPB%d] WrOffset:0x%08X/0x%08X/0x%08X, RdOffset:0x%08X\n"
					,ui8VesCh, ui32WrOffset, ui32WrOffset_AuStart, ui32WrOffset_AlignedEnd, ui32RdOffset);
			return 0x0;
		}

		pui8CpbWrVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32WrOffset;
		ui32PadSize = gsVesCpb[ui8VesCh].ui32BufSize - ui32WrOffset;

		memset(pui8CpbWrVirPtr, 0x0, ui32PadSize);
	}

	ui32CpbWrPhyAddr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset_AuStart;
	pui8CpbWrVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32WrOffset_AuStart;

	// 2. Write User Data to CPB
	if( (ui32WrOffset_AuStart + ui32AuSize) > gsVesCpb[ui8VesCh].ui32BufSize )
	{ // Wrap Around
		UINT32		ui32AuSize_Front;
		UINT32		ui32AuSize_Back;

		logm_debug(vdec_ves, "[CPB%d] Cpb Wrap Around, WrOffset:0x%08X/0x%08X/0x%08X, RdOffset:0x%08X\n"
				,ui8VesCh, ui32WrOffset, ui32WrOffset_AuStart, ui32WrOffset_AlignedEnd, ui32RdOffset);

		ui32AuSize_Front = gsVesCpb[ui8VesCh].ui32BufSize - ui32WrOffset_AuStart;

		if ( copy_from_user(pui8CpbWrVirPtr, pui8AuStartAddr, ui32AuSize_Front) > 0)
			return 0x0;

		pui8CpbWrVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr;
		pui8AuStartAddr += ui32AuSize_Front;

		ui32AuSize_Back = (ui32WrOffset_AuStart + ui32AuSize) - gsVesCpb[ui8VesCh].ui32BufSize;

		if ( copy_from_user(pui8CpbWrVirPtr, pui8AuStartAddr, ui32AuSize_Back) > 0)
			return 0x0;

		pui8CpbWrVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32AuSize_Back;
		ui32WrOffset_AuEnd = ui32AuSize_Back;
	}
	else
	{
		if ( copy_from_user(pui8CpbWrVirPtr, pui8AuStartAddr, ui32AuSize) > 0)
			return 0x0;

		pui8CpbWrVirPtr += ui32AuSize;
		ui32WrOffset_AuEnd = ui32WrOffset_AuStart + ui32AuSize;
	}

	// pad au end
	if( ui32WrOffset_AuEnd > ui32WrOffset_AlignedEnd )
	{
		memset(pui8CpbWrVirPtr, 0x0, gsVesCpb[ui8VesCh].ui32BufSize - ui32WrOffset_AuEnd);

		if( ui32WrOffset_AlignedEnd > 0 )
		{
			pui8CpbWrVirPtr = gsVesCpb[ui8VesCh].pui8VirBasePtr;
			memset(pui8CpbWrVirPtr, 0x0, ui32WrOffset_AlignedEnd);
		}
	}
	else if( ui32WrOffset_AuEnd < ui32WrOffset_AlignedEnd )
	{
		memset(pui8CpbWrVirPtr, 0x0, ui32WrOffset_AlignedEnd - ui32WrOffset_AuEnd);
	}

	*pui32CpbWrPhyAddr_End = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset_AlignedEnd;
	*pui32AuSizeModified = ui32AuSize_Modified;

	return ui32CpbWrPhyAddr;
}

/**
********************************************************************************
* @brief
* read es data at cpb
* @remarks
* read es data at cpb
* @par requirements:
* int, int, int, int*
* @param
* ves channel number, au start address, au size, buffer for copying
* @return
* TRUE, success
* FALSE, failed
********************************************************************************
*/
BOOLEAN VES_CPB_Read(UINT8 ui8VesCh, UINT32 ui32Start, UINT32 ui32Size, UINT8 *pui8Buf)
{
	UINT32	ui32RdOffset;
	UINT8*	pui8RdVirAddr;
	UINT32	ui32Size_Front, ui32Size_Back;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return FALSE;
	}

	if( gsVesCpb[ui8VesCh].bFromTVP == TRUE )
	{
		// CANNOT ACCESS TO VIRTUAL MEMORY WHEN TVP PATH
		return FALSE;
	}
	else
	{
		// calculate offset
		ui32RdOffset = ui32Start - gsVesCpb[ui8VesCh].ui32PhyBaseAddr;

		if( ui32RdOffset == gsVesCpb[ui8VesCh].ui32BufSize )
		{
			ui32RdOffset = 0;
		}
		logm_info(vdec_ves,"[CPB%d] rd offset:%x \n",ui8VesCh,ui32RdOffset);

		// calculate vir addr
		pui8RdVirAddr = gsVesCpb[ui8VesCh].pui8VirBasePtr + ui32RdOffset;
		logm_info(vdec_ves,"[CPB%d] vir rd:0x%08x \n", ui8VesCh, (unsigned int)pui8RdVirAddr);

		// addr + size가 buffer를 초과하면
		if( ui32RdOffset + ui32Size > gsVesCpb[ui8VesCh].ui32BufSize) {
			ui32Size_Front = gsVesCpb[ui8VesCh].ui32BufSize - ui32RdOffset;
			ui32Size_Back = ui32Size - ui32Size_Front;

			logm_info(vdec_ves,"[CPB%d] wrap arounded: Front:%d, Back:%d \n",ui8VesCh,ui32Size_Front,ui32Size_Back);

			memcpy(pui8Buf, pui8RdVirAddr, ui32Size_Front);
			memcpy(pui8Buf+ui32Size_Front, gsVesCpb[ui8VesCh].pui8VirBasePtr, ui32Size_Back);
		}
		else memcpy(pui8Buf, pui8RdVirAddr, ui32Size);

		return TRUE;
	}
}

/**
********************************************************************************
* @brief
* get cpb usage
* @remarks
* get cpb usage
* @par requirements:
* int
* @param
* ves channel number
* @return
* amount of cpb usage (bytes)
********************************************************************************
*/
UINT32 VES_CPB_GetUsedBuffer(UINT8 ui8VesCh)
{
	UINT32		ui32UseBytes;
	UINT32		ui32WrOffset;
	UINT32		ui32RdOffset;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0;
	}

	ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);

	if( ui32WrOffset >= ui32RdOffset )
		ui32UseBytes = ui32WrOffset - ui32RdOffset;
	else
		ui32UseBytes = ui32WrOffset + gsVesCpb[ui8VesCh].ui32BufSize - ui32RdOffset;

	return ui32UseBytes;
}

/**
********************************************************************************
* @brief
* get cpb base address
* @remarks
* get cpb base address
* @par requirements:
* int
* @param
* ves channel number
* @return
* cpb base address
********************************************************************************
*/
UINT32 VES_CPB_GetBufferBaseAddr(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0;
	}

	return gsVesCpb[ui8VesCh].ui32PhyBaseAddr;
}

/**
********************************************************************************
* @brief
* get cpb size
* @remarks
* get cpb size
* @par requirements:
* int
* @param
* ves channel number
* @return
* cpb size
********************************************************************************
*/
UINT32 VES_CPB_GetBufferSize(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d][Err] \n", ui8VesCh);
		return 0;
	}

	return gsVesCpb[ui8VesCh].ui32BufSize;
}


/**
********************************************************************************
* @brief
* get remained buffer size
* @remarks
* get remained buffer size
* @par requirements:
* int
* @param
* ves channel number
* @return
* amount of remained buffer (bytes)
********************************************************************************
*/
UINT32 VES_CPB_GetRemainedBuffer(UINT8 ui8VesCh)
{
	return ( gsVesCpb[ui8VesCh].ui32BufSize - VES_CPB_GetUsedBuffer(ui8VesCh) );
}

#ifdef CPB_USE_NEW_UPDATE_BUFFER

/**
********************************************************************************
* @brief
* allocate cbp buffer
* @remarks
* allocate cbp buffer
* @param ui8VesCh ves channel number
* @param ui32Size cbp buffer size
* @return
* amount of buffer (bytes)
********************************************************************************
*/
UINT32 VES_CPB_AllocBuffer(UINT8 ui8VesCh, UINT32 ui32AuSize, CPB_MEM_BUF_T * pstBufFront, CPB_MEM_BUF_T *pstBufBack, BOOLEAN b512bytesAligned)
{
	UINT32		ui32WrOffset_AuStart, ui32WrOffset_AlignedEnd;
	UINT32		ui32RdOffset;
	UINT32		ui32RsOffset;
	UINT32		ui32AuSize_Modified;

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[CPB%d] \n", ui8VesCh);
		return 0;
	}

	if ( pstBufFront == NULL || pstBufBack == NULL )
	{
		logm_error(vdec_ves, "[CPB%d] buffer arguments are null\n");
		return 0;
	}

	if( b512bytesAligned == TRUE )
		//ui32AuSize_Modified = (VES_CPB_CEILING_512BYTES(ui32AuSize) + 1024);
		ui32AuSize_Modified = VES_CPB_CEILING_512BYTES(ui32AuSize);
	else
		// to avoid alignment trap when copy es to cpb
		//ui32AuSize_Modified = ((ui32AuSize) + 0x0F) & (~0x0F);
		ui32AuSize_Modified = ui32AuSize;
	
	//ui32WrOffset = IPC_REG_CPB_GetWrOffset(ui8VesCh);
	ui32RdOffset = IPC_REG_CPB_GetRdOffset(ui8VesCh);
	ui32RsOffset = gsVesCpb[ui8VesCh].ui32RsOffset;
	
	ui32WrOffset_AuStart = ui32RsOffset;

	// 1. Check CPB Overflow
	if (_VES_CPB_CheckOverflow(ui8VesCh, ui32AuSize, &ui32WrOffset_AuStart, &ui32WrOffset_AlignedEnd, ui32RdOffset) == TRUE)
		return 0x0;

	if ((ui32WrOffset_AuStart + ui32AuSize_Modified) >= gsVesCpb[ui8VesCh].ui32BufSize )
	{
		// over-flow
		pstBufFront->addr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset_AuStart;
		pstBufFront->length = gsVesCpb[ui8VesCh].ui32BufSize - ui32WrOffset_AuStart;

		pstBufBack->addr = gsVesCpb[ui8VesCh].ui32PhyBaseAddr;
		pstBufBack->length = (ui32WrOffset_AuStart + ui32AuSize_Modified) - gsVesCpb[ui8VesCh].ui32BufSize;

		gsVesCpb[ui8VesCh].ui32RsOffset = pstBufBack->length;
	}
	else
	{
		pstBufFront->addr =  gsVesCpb[ui8VesCh].ui32PhyBaseAddr + ui32WrOffset_AuStart;
		//pstBufFront->length = ui32AuSize;
		pstBufFront->length = ui32AuSize_Modified;

		gsVesCpb[ui8VesCh].ui32RsOffset += ui32AuSize_Modified;
	}

	logm_debug(vdec_ves, "[CPB%d] Alloc new buffer [0x%08X(%d)],[0x%08X(%d)](%d)\n",
		ui8VesCh, pstBufFront->addr, pstBufFront->length, pstBufBack->addr, pstBufBack->length,
		ui32AuSize_Modified == pstBufFront->length + pstBufBack->length ? ui32AuSize_Modified : -1 );

	return ui32AuSize_Modified;
}
#endif

