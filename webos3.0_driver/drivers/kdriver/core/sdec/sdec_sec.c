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


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>



#include "os_util.h"


#include "sdec_kapi.h"
#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_pes.h"
#include "sdec_sec.h"
#include "sdec_hal.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/* for using physical <-> virtual address conversion */
static UINT32 _gVirtOffset = 0;

/* Information of pes GPB */
SecInfo	_gSecInfo = {
	.used 		= 0,
	.running	= 0,
	.processing	= 0,

	.pid		= 0x1fff,

	.gpbBaseP	= 0,
	.gpbEndPt	= 0,
	.gpbRdPtr	= 0,
	.gpbWrPtr	= 0,
	.dstBaseP	= 0,
	.dstEndPt	= 0,
	.dstRdPtr	= 0,
	.dstWrPtr	= 0,
	.secPacketSize	= 0,
	.copiedSize	= 0,
	.isStart	= 0
} ;

void SDEC_SEC_Init(void)
{
	SDEC_VDECHAL_PDECInit();
	SDEC_SEC_SetVirtOffset();
}


UINT8 SDEC_SEC_IsSecFlt(void)
{
	return _gSecInfo.used;
}

void SDEC_SEC_SetSecFlt(void)
{
	_gSecInfo.used = 1;
}

void SDEC_SEC_ClearSecFlt(void)
{
	_gSecInfo.used = 0;
}

UINT8 SDEC_SEC_IsPDECRunning(void)
{
	return _gSecInfo.running;
}

void SDEC_SEC_SetSrcBuf(UINT32 gpbBaseP, UINT32 gpbSize)
{
	_gSecInfo.gpbBaseP 	= gpbBaseP;
	_gSecInfo.gpbEndPt		= gpbBaseP  + gpbSize;
	_gSecInfo.gpbRdPtr 	= _gSecInfo.gpbBaseP;
	_gSecInfo.gpbWrPtr 	= _gSecInfo.gpbBaseP;
}

void SDEC_SEC_ResetSrcBuf(void)
{
	_gSecInfo.gpbRdPtr 	= _gSecInfo.gpbWrPtr;
}


void SDEC_SEC_SetDstBuf(UINT32 dstBaseP, UINT32 dstSize)
{
	_gSecInfo.dstBaseP 	= dstBaseP;
	_gSecInfo.dstEndPt		= dstBaseP  + dstSize;
	_gSecInfo.dstRdPtr 	= _gSecInfo.dstBaseP;
	_gSecInfo.dstWrPtr 	= _gSecInfo.dstBaseP;

	_gSecInfo.secPacketSize		= 0;
	_gSecInfo.copiedSize			= 0;
}

void SDEC_SEC_SetSecFltPattern(UINT8 *pucPattern, UINT8 *pucMask, UINT8 *pucNotEqual)
{
	int idx = 0;

	memcpy(_gSecInfo.pucPattern, 	pucPattern,	8);
	memcpy(_gSecInfo.pucNotEqual,	pucNotEqual,	8);

	/* 뒤집어야 한다. */
	for ( idx = 0; idx < 8 ; idx++ )
	{
		_gSecInfo.pucMask[idx] = ~pucMask[idx];
	}

}


/*
 * compare section filter pattern
 * return 0 : identical
 * return 1 : different
 */
int SDEC_SEC_ComparePattern(UINT8 *pPattern)
{
	int idx = 0;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;
	idx++;
	if( ( pPattern[idx] & _gSecInfo.pucMask[idx] ) != ( _gSecInfo.pucPattern[idx] & _gSecInfo.pucMask[idx] ) ) 	return 1;

	return 0;
}


void SDEC_SEC_SetVirtOffset(void)
{
	LX_SDEC_CFG_T	*pSdecConf;
	UINT32			baseAddr = 0, size = 0;
	UINT32			virtAddr = 0;

	pSdecConf = SDEC_CFG_GetConfig();
	baseAddr 	= gMemCfgSDEC.base;
	size		= gMemCfgSDEC.size;

	virtAddr = (UINT32) ioremap(baseAddr, size);
	_gVirtOffset = virtAddr - baseAddr;

}

void SDEC_SEC_CopyPacket(UINT32 va_gpbRdPtr, UINT32 sizeToCopy)
{
	/* physical address */
	UINT32  gpbBase = 0, gpbEnd = 0;

	/* virtual address */
	UINT32	va_gpbBaseP = 0, va_gpbEndPt = 0;
	UINT32  va_dstWrPtr = 0, va_dstBaseP = 0, va_dstEndPt = 0;

	UINT32	remainSize = 0, wrappedSize = 0, tailSize = 0;

	/* get gpb information */
	gpbBase 	= _gSecInfo.gpbBaseP;
	gpbEnd		= _gSecInfo.gpbEndPt;

	/* get virtual memory of gpb & destination buffer */
	va_gpbBaseP = LX_PHYS_TO_VIRT(gpbBase);
	va_gpbEndPt = LX_PHYS_TO_VIRT(gpbEnd);
	va_dstWrPtr = LX_PHYS_TO_VIRT(_gSecInfo.dstWrPtr);
	va_dstBaseP = LX_PHYS_TO_VIRT(_gSecInfo.dstBaseP);
	va_dstEndPt = LX_PHYS_TO_VIRT(_gSecInfo.dstEndPt);

	//printk("1 gp_rp[%08x] dt_wp[%08x] sizeToCopy[%d]\n", va_gpbRdPtr,  va_dstWrPtr, sizeToCopy);

	/* memory copy */
	if( (va_gpbRdPtr + sizeToCopy) > va_gpbEndPt)
	{
		/* wrap around happen */
		wrappedSize = va_gpbEndPt - va_gpbRdPtr;
		remainSize	= sizeToCopy;

		//printk("SIZE[%4d] Wraped[%4d]\n", remainSize, wrappedSize);
		//printk("1 gp_rp[%08x] dt_wp[%08x]\n", va_gpbRdPtr,  va_dstWrPtr);

		if( ( va_dstWrPtr + wrappedSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, wrappedSize);
			va_dstWrPtr = va_dstWrPtr + wrappedSize;
			//printk("Normal copy\n");
		}
		else
		{
			/*
			 * wrapped around copy scheme
			 *
			 * step 1 :
			 *										 va_gpbRdPtr
			 *											  V
			 * src :  |------------------------------------------------------------|
			 *			   ^
			 *		  va_gpbWrPtr
			 *												   va_dstWrPtr	 va_dstEndPt
			 *														 V			   V
			 * dst :  |----------------------------------------------++++++++++++++|
			 *														 ^ ----------- ^
			 *															 tailSize
			 */

			tailSize = va_dstEndPt - va_dstWrPtr;
			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbRdPtr + tailSize),  wrappedSize - tailSize);
			va_dstWrPtr = va_dstBaseP + wrappedSize - tailSize;

			//printk("Segment Copy tailSize[%x]\n", tailSize);
		}

		//printk("2 gp_rp[%08x] dt_wp[%08x]\n", va_gpbRdPtr, va_dstWrPtr);

		remainSize -= wrappedSize;

		if( ( va_dstWrPtr + remainSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbBaseP, remainSize);
			va_dstWrPtr = va_dstWrPtr + remainSize;
			//printk("Normal copy\n");
		}
		else
		{
			tailSize = va_dstEndPt - va_dstWrPtr;

			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbBaseP, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbBaseP + tailSize),  remainSize - tailSize);
			va_dstWrPtr = va_dstBaseP + remainSize - tailSize;

			//printk("Segment Copy tailSize[%x]\n", tailSize);
		}
		//printk("3 gp_rp[%08x] dt_wp[%08x]\n", va_gpbRdPtr, va_dstWrPtr);

		//va_gpbRdPtr = va_gpbBaseP + (sizeToCopy - wrappedSize);

	}
	else
	{
		remainSize = sizeToCopy;

		//printk("SIZE[%4d]\n", remainSize);
		//printk("1 gp_rp[%08x] dt_wp[%08x]\n", va_gpbRdPtr, va_dstWrPtr);

		if( ( va_dstWrPtr + remainSize ) <= va_dstEndPt)
		{
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, remainSize);
			va_dstWrPtr = va_dstWrPtr + remainSize;
			//printk("Normal copy\n");
		}
		else
		{
			/*
			 * wrapped around copy scheme
			 *
			 * step 1 :
			 *										 va_gpbRdPtr
			 *											  V
			 * src :  |------------------------------------------------------------|
			 *														   ^
			 *													  va_gpbWrPtr
			 *												   va_dstWrPtr	 va_dstEndPt
			 *														 V			   V
			 * dst :  |----------------------------------------------++++++++++++++|
			 *														 ^ ----------- ^
			 *															 tailSize
			 *
			 */

			tailSize = va_dstEndPt - va_dstWrPtr;

			/* copy first part to destination segment */
			memcpy((void*)va_dstWrPtr, (void*)va_gpbRdPtr, tailSize);
			/* copy second part to destination base pointer */
			memcpy((void*)va_dstBaseP, (void*)(va_gpbRdPtr + tailSize),  remainSize - tailSize);
			va_dstWrPtr = va_dstBaseP + remainSize - tailSize;
			//printk("Segment Copy tailSize[%x]\n", tailSize);
		}
		//printk("2 gp_rp[%08x] dt_wp[%08x]\n", va_gpbRdPtr, va_dstWrPtr);

		//va_gpbRdPtr += sizeToCopy;
	}

	_gSecInfo.dstWrPtr = (UINT32) LX_VIRT_TO_PHYS(va_dstWrPtr);

	//return va_gpbRdPtr;
}


void SDEC_SEC_Proc(struct work_struct *work)
{
	S_SDEC_PARAM_T *stpSdecParam;

	/* physical address */
	UINT32  pa_gpbBaseP = 0, pa_gpbEndPt = 0, pa_gpbRdPtr = 0, pa_gpbWrPtr = 0, gpbSize = 0;
	/* virtual address */
	UINT32	va_gpbBaseP = 0, va_gpbEndPt = 0, va_gpbRdPtr = 0, va_gpbWrPtr = 0;

	SINT32	remainSize = 0, wrappedSize = 0, readSize = 0, sectionSize = 0;
	UINT8 	*pPtr;

	UINT8	pattern[8];

	unsigned long flags = 0;

	LX_SDEC_NOTIFY_PARAM_T notiParam;
	BOOLEAN 				isFound = FALSE;

	LX_SDEC_CHECK_CODE( work == NULL, return, "Invalid parameters" );

	stpSdecParam =	container_of (work, S_SDEC_PARAM_T, SecProc);

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return, "Invalid parameters" );

#if 0
	pa_gpbRdPtr 	= _gSecInfo.gpbRdPtr;
	pa_gpbWrPtr 	= SDEC_VDECHAL_PDECCPB_GetWrPtr();

	printk("SDEC_SEC_Proc -> pa_gpbRdPtr : 0x%08x, pa_gpbWrPtr = 0x%08x\n", pa_gpbRdPtr, pa_gpbWrPtr);
#endif

	spin_lock_irqsave(&stpSdecParam->stSdecSecSpinlock, flags);

	_gSecInfo.processing = 1;

	/* get gpb information */
	pa_gpbBaseP 	= _gSecInfo.gpbBaseP;
	pa_gpbEndPt 	= _gSecInfo.gpbEndPt;
	pa_gpbRdPtr 	= _gSecInfo.gpbRdPtr;
	pa_gpbWrPtr 	= SDEC_VDECHAL_PDECCPB_GetWrPtr();

	/* gpb buffer is not empty */
	if(pa_gpbRdPtr != pa_gpbWrPtr)
	{
		gpbSize 		= pa_gpbEndPt - pa_gpbBaseP;

		//printk("pa_gpbRdPtr = 0x%08x, pa_gpbWrPtr = 0x%08x\n", pa_gpbRdPtr, pa_gpbWrPtr);
		if( SDEC_SEC_IsSecFlt() )
		{
			/* get virtual memory of gpb & destination buffer */
			va_gpbRdPtr = LX_PHYS_TO_VIRT(pa_gpbRdPtr);
			va_gpbBaseP = LX_PHYS_TO_VIRT(pa_gpbBaseP);
			va_gpbEndPt = LX_PHYS_TO_VIRT(pa_gpbEndPt);
			va_gpbWrPtr = LX_PHYS_TO_VIRT(pa_gpbWrPtr);

			/* get packet size */
			if( pa_gpbRdPtr > pa_gpbWrPtr)
			{
				/* wrap around happen */
				wrappedSize = pa_gpbEndPt - pa_gpbRdPtr;
				remainSize	= wrappedSize + (pa_gpbWrPtr - pa_gpbBaseP );
			}
			else
			{
				wrappedSize = 0;
				remainSize	= pa_gpbWrPtr - pa_gpbRdPtr;
			}

			//printk("remainSize : %u, va_gpbRdPtr = 0x%08x\n", remainSize, va_gpbRdPtr);

			if( remainSize < 4096 )
			{
				do
				{
					readSize = 1;
					pPtr = (UINT8 *) va_gpbRdPtr;

					if( _gSecInfo.isStart == TRUE )
					{
						readSize = ( ( _gSecInfo.secPacketSize - _gSecInfo.copiedSize ) > remainSize ) ? remainSize : ( _gSecInfo.secPacketSize - _gSecInfo.copiedSize );

						SDEC_SEC_CopyPacket(va_gpbRdPtr, readSize);

						_gSecInfo.copiedSize += readSize;
						//printk("2 sectionSize = %d, remainSize = %d, readSize = %d _gSecInfo.copiedSize = %d\n", sectionSize, remainSize, readSize, _gSecInfo.copiedSize);
						//printk("dstRdPtr = 0x%08x dstRdPtr = 0x%08x size = %d\n", _gSecInfo.dstRdPtr, _gSecInfo.dstWrPtr, ( _gSecInfo.dstWrPtr - _gSecInfo.dstRdPtr));
						if( _gSecInfo.copiedSize == _gSecInfo.secPacketSize )
						{
							//printk("Find!!!\n");

							/* stored buf info	*/
							notiParam.channel	= 2;
							notiParam.index 	= 0;
							notiParam.readPtr	= _gSecInfo.dstRdPtr;
							notiParam.writePtr	= _gSecInfo.dstWrPtr;
							notiParam.status	= LX_SDEC_FLTSTATE_DATAREADY;

							isFound = SDEC_SetNotifyParam( stpSdecParam, notiParam);

							_gSecInfo.dstRdPtr = _gSecInfo.dstWrPtr;
							_gSecInfo.isStart = FALSE;
						}
					}
					else
					{
						/* byte scanning for finding table id */
						if( LX_SEC_COMPARE_TABLEID(pPtr[0], _gSecInfo.pucPattern[0], _gSecInfo.pucMask[0]) )
						{
							LX_GET_SEC_HEADER(pattern, va_gpbRdPtr, va_gpbBaseP, va_gpbEndPt);

							/* find section header */
							if( SDEC_SEC_ComparePattern(pattern) == 0 )
							{
								sectionSize = LX_SEC_SECTION_LENGTH(pPtr);
								//printk("sectionSize = %d, remainSize = %d\n", sectionSize, remainSize);

								/* if packet size is enough, report to user */
								if( sectionSize < 4096 && sectionSize > 3 ) //&& remainSize >= ( sectionSize + 3 ))
								{
									_gSecInfo.secPacketSize 	= sectionSize;
									_gSecInfo.isStart 			= TRUE;
									_gSecInfo.copiedSize		= 0;

									readSize = ( sectionSize > remainSize ) ? remainSize : sectionSize;
									SDEC_SEC_CopyPacket( va_gpbRdPtr, readSize);

									_gSecInfo.copiedSize = readSize;
									//printk("1 sectionSize = %d, remainSize = %d, readSize = %d\n", sectionSize, remainSize, readSize);

								}
							}
						}
					}

					/* advance gpb read pointer */
					LX_ADVANCE_BUFF(va_gpbRdPtr, readSize, va_gpbBaseP, va_gpbEndPt);

					remainSize -= readSize;
				} while(remainSize > 0);
			}
		}
		_gSecInfo.gpbRdPtr = pa_gpbWrPtr;
	}

	_gSecInfo.processing = 0;
	spin_unlock_irqrestore(&stpSdecParam->stSdecSecSpinlock, flags);

	/* if found notifying section data, notify to appl layer */
	//if(isFound) queue_work(stpSdecParam->workqueue, &stpSdecParam->Notify);
	SDEC_Notify(&stpSdecParam->Notify);

}

void SDEC_SEC_Thread(S_SDEC_PARAM_T 	*stpSdecParam)
{
	/* physical address */
	UINT32  pa_gpbBaseP = 0, pa_gpbEndPt = 0, pa_gpbRdPtr = 0, pa_gpbWrPtr = 0;
	SINT32	remainSize = 0, wrappedSize = 0;

	BOOLEAN		isFound = FALSE;

	/* get gpb information */
	pa_gpbBaseP 	= _gSecInfo.gpbBaseP;
	pa_gpbEndPt 	= _gSecInfo.gpbEndPt;
	pa_gpbRdPtr 	= _gSecInfo.gpbRdPtr;
	pa_gpbWrPtr 	= SDEC_VDECHAL_PDECCPB_GetWrPtr();

	//printk("11 pa_gpbRdPtr = %08x pa_gpbWrPtr = %08x\n", pa_gpbRdPtr, pa_gpbWrPtr );

	/* gpb buffer is not empty */
	if(pa_gpbRdPtr != pa_gpbWrPtr)
	{
		/* set as pdec running */
		_gSecInfo.running = 1;

		if( SDEC_SEC_IsSecFlt() && _gSecInfo.processing != 1)
		{
			/* get packet size */
			if( pa_gpbRdPtr > pa_gpbWrPtr)
			{
				/* wrap around happen */
				wrappedSize = pa_gpbEndPt - pa_gpbRdPtr;
				remainSize	= wrappedSize + (pa_gpbWrPtr - pa_gpbBaseP );
			}
			else
			{
				wrappedSize = 0;
				remainSize	= pa_gpbWrPtr - pa_gpbRdPtr;
			}

			//printk("remainSize = %d\n", remainSize );

			/* set isFound True */
			isFound = TRUE;
		}
	}

	/* if found notifying section data, notify to appl layer */
	if(isFound) queue_work(stpSdecParam->workqueue, &stpSdecParam->SecProc);

}

