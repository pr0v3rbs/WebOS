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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef _SDEC_SEC_H
#define _SDEC_SEC_H

#ifdef __cplusplus
	 extern "C" {
#endif

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define LX_SEC_COMPARE_TABLEID(ptr, tableId, mask)	( (ptr & mask) == (tableId & mask) )
#define LX_SEC_SECTION_LENGTH(x)	((((*((x)+1)&0x0F)<<8)|(*((x)+2))) + 3 )

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
   enum
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
   structre
----------------------------------------------------------------------------------------*/

typedef struct
{
	UINT8		used;
	UINT8		running;
	UINT8		processing;

	UINT16		pid;

	UINT32		gpbBaseP;
	UINT32		gpbEndPt;
	UINT32		gpbRdPtr;
	UINT32		gpbWrPtr;

	UINT32		dstBaseP;
	UINT32		dstEndPt;
	UINT32		dstRdPtr;
	UINT32		dstWrPtr;

	UINT32		secPacketSize;
	UINT32		copiedSize;

	UINT8		pucPattern[8];
	UINT8		pucMask[8];
	UINT8		pucNotEqual[8];

	BOOLEAN		isStart;
} SecInfo;

/*----------------------------------------------------------------------------------------
   API
----------------------------------------------------------------------------------------*/
void SDEC_SEC_Init(void);
UINT8 SDEC_SEC_IsSecFlt(void);
UINT8 SDEC_SEC_IsPDECRunning(void);
void SDEC_SEC_SetPid(UINT16 pid);
void SDEC_SEC_SetVirtOffset(void);
void SDEC_SEC_SetSecFlt(void);
void SDEC_SEC_ClearSecFlt(void);
void SDEC_SEC_ResetSrcBuf(void);
void SDEC_SEC_SetSrcBuf(UINT32 gpbBaseP, UINT32 gpbSize);
void SDEC_SEC_SetDstBuf(UINT32 gpbBaseP, UINT32 gpbSize);
void SDEC_SEC_SetSecFltPattern(UINT8 *pucPattern, UINT8 *pucMask, UINT8 *pucNotEqual);
void SDEC_SEC_Proc(struct work_struct *work);
void SDEC_SEC_Thread(S_SDEC_PARAM_T 	*stpSdecParam);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _SDEC_SEC_H */

