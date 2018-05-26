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
 */

#ifndef _VES_AUIB_H_
#define _VES_AUIB_H_


#include <linux/wait.h>

#include "vdec_type_defs.h"
#include "../hal/pdec_hal_api.h"

#define	VES_AUIB_NUM_OF_NODE				0x2710



#ifdef __cplusplus
extern "C" {
#endif

typedef	E_PDEC_AU_T		E_VES_AU_T;

typedef struct{
	// LQ AU Indexing Information
	/*	AU_INFO_1
		Access Unit Type			: 4,	//	 0: 3
		End Address Check Mode	: 4,	//	 4: 5
								: 2,	//	 6: 7
		Flush Befor Run			: 1,	//	 8
		Start Address of Access Unit	:23;	//	 9:31
	*/
	UINT32			ui32AuStartAddr;		// 512bytes aligned size
	UINT32			ui32EndAddrCheckMode;
	E_VES_AU_T		eAuType;

	UINT32			bCHB_Valid;
	UINT32			bStccDiscontinuity;
	UINT32			bGopHeader;
	UINT32			bWrapAround;

	/*	AU_INFO_2
		DTS						:28,	//	 0:27
								: 1,	//	28
		Ignore STD DTS Matching	: 1,	//	29
		WaitSync0				: 1,	//	30
		WaitSync1				: 1,	//	31
	*/
	UINT32			ui32DTS;			// real size

	/*	AU_INFO_3
		PTS 						:28,	//	 0:27
		PTS Error					: 1,	//	28
								: 3,	//	29:31
	*/
	UINT32			ui32PTS;

	UINT32			DTS_PTS_flag;
	UINT32			ui32AUPaircheck;

	UINT64			ui64TimeStamp;
	UINT32			ui32UId;

	/*	AU_INFO_4
		AUI next address			:32,	//	 0:31
	*/
	UINT32			ui32AuEndAddr;
	UINT32			ui32AuSize;

	//for Buffered Play Path
	UINT32 			ui32FrameRateRes;
	UINT32 			ui32FrameRateDiv;

	BOOLEAN			b512bytesAligned;

}S_VES_AU_T;


void VES_AUIB_Init(void);
wait_queue_head_t *VES_AUIB_GetWaitQueueHead(UINT8 ui8VesCh);
void VES_AUIB_Reset(UINT8 ui8VesCh);
void VES_AUIB_Flush(UINT8 ui8VesCh);
BOOLEAN VES_AUIB_Open(UINT8 ui8VesCh, UINT8 ui8PdecCh, BOOLEAN bIsHwPath, BOOLEAN bFromTVP, E_PDEC_CODEC_TYPE_T eCodecType);
void VES_AUIB_Close(UINT8 ui8VesCh);
void VES_AUIB_UpdateWrPtr(UINT8 ui8VesCh, UINT32 ui32WrPhyAddr);
void VES_AUIB_ConsumeOneAU(UINT8 ui8VesCh, BOOLEAN bFeedPop);
void VES_AUIB_RestoreOneAU(UINT8 ui8VesCh, BOOLEAN bFeedPop);
BOOLEAN VES_AUIB_Push(UINT8 ui8VesCh, S_VES_AU_T *pVesAu);
BOOLEAN VES_AUIB_CheckAuType(UINT8 ui8VesCh, E_VES_AU_T *eAuType);
BOOLEAN VES_AUIB_Pop(UINT8 ui8VesCh, S_VES_AU_T *pVesAu);
BOOLEAN VES_AUIB_GetAUInfo(UINT8 ui8VesCh, S_VES_AU_T *pVesAu);
UINT32 VES_AUIB_NumActive(UINT8 ui8VesCh);
UINT32 VES_AUIB_NumActiveTsDb(UINT8 ui8VesCh);
UINT32 VES_AUIB_NumFree(UINT8 ui8VesCh);

#ifdef __cplusplus
}
#endif

#endif /* _VES_AUIB_H_ */

