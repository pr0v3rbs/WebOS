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
 *	BE REG module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>	/**< printk() */
#include <asm/io.h>			/**< For ioremap_nocache */
#include "os_util.h"

#include "be_dbg.h"
#include "be_def.h"
#include "be_reg_hw_m14.h"
#include "be_reg_def_m14.h"
#include "be_reg_dppb_m14.h"
#include "be_reg_frc_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define REG_WD( addr, value)	( *( volatile UINT32 * )( addr )) = ( UINT32 )( value)
#define REG_RD( addr )			( *( volatile UINT32 * )( addr ))
#define GET_BITS(val,nd,wd)		(((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_PVAL(val,nd,wd)		(GET_BITS(val,nd,wd)<<(nd))

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {

} BE_REG_HW_M14_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
FRC_LRX_REG_M14_T gFRC_LRX_M14;
FRC_P3D_REG_M14_T gFRC_P3D_M14;
FRC_DVI_REG_M14_T gFRC_DVI_M14;
FRC_DVO_REG_M14_T gFRC_DVO_M14;

DPPB_TOP_REG_M14_T gDPPB_TOP_M14;
DPPB_TXM_REG_M14_T gDPPB_TXM_M14;
DPPB_LED_REG_M14_T gDPPB_LED_M14;

PE1_HDR_REG_M14_T gPE1_HDR_M14B;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_REG_HW_M14_INFO_T gstBeRegHwM14Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_REG_HW_M14_Initialize(void)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			gFRC_DVO_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(FRC_DVO_REG_M14B0_T));
			gDPPB_LED_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DPPB_LED_REG_M14B0_T));
			gPE1_HDR_M14B.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(PE1_HDR_REG_M14_T));

			gFRC_DVO_M14.phys.addr = (volatile UINT32 *)ioremap(FRC_DVO_REG_M14B0_BASE, sizeof(FRC_DVO_REG_M14B0_T));
			gDPPB_LED_M14.phys.addr = (volatile UINT32 *)ioremap(DPPB_LED_REG_M14B0_BASE, sizeof(DPPB_LED_REG_M14B0_T));
			gPE1_HDR_M14B.phys.addr = (volatile UINT32 *)ioremap(PE1_HDR_REG_M14B0_CONTROL_ADDR, sizeof(PE1_HDR_REG_M14B0_T));
		}
		else
		{
			gFRC_LRX_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(FRC_LRX_REG_M14A0_T));
			gFRC_P3D_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(FRC_P3D_REG_M14A0_T));
			gFRC_DVI_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(FRC_DVI_REG_M14A0_T));
			gFRC_DVO_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(FRC_DVO_REG_M14A0_T));

			gDPPB_TOP_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DPPB_TOP_REG_M14A0_T));
			gDPPB_TXM_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DPPB_TXM_REG_M14A0_T));
			gDPPB_LED_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DPPB_LED_REG_M14A0_T));

			gFRC_LRX_M14.phys.addr = (volatile UINT32 *)ioremap(FRC_LRX_REG_M14A0_BASE, sizeof(FRC_LRX_REG_M14A0_T));
			gFRC_P3D_M14.phys.addr = (volatile UINT32 *)ioremap(FRC_P3D_REG_M14A0_BASE, sizeof(FRC_P3D_REG_M14A0_T));
			gFRC_DVI_M14.phys.addr = (volatile UINT32 *)ioremap(FRC_DVI_REG_M14A0_BASE, sizeof(FRC_DVI_REG_M14A0_T));
			gFRC_DVO_M14.phys.addr = (volatile UINT32 *)ioremap(FRC_DVO_REG_M14A0_BASE, sizeof(FRC_DVO_REG_M14A0_T));

			gDPPB_TOP_M14.phys.addr = (volatile UINT32 *)ioremap(DPPB_TOP_REG_M14A0_BASE, sizeof(DPPB_TOP_REG_M14A0_T));
			gDPPB_TXM_M14.phys.addr = (volatile UINT32 *)ioremap(DPPB_TXM_REG_M14A0_BASE, sizeof(DPPB_TXM_REG_M14A0_T));
			gDPPB_LED_M14.phys.addr = (volatile UINT32 *)ioremap(DPPB_LED_REG_M14A0_BASE, sizeof(DPPB_LED_REG_M14A0_T));
		}
	} while(0);

	return ret;
}

int BE_REG_HW_M14_Close(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		OS_Free((void *)gFRC_DVO_M14.shdw.addr);
		OS_Free((void *)gDPPB_LED_M14.shdw.addr);
		OS_Free((void *)gPE1_HDR_M14B.shdw.addr);

		if (gFRC_DVO_M14.phys.addr) iounmap((void *)gFRC_DVO_M14.phys.addr);
		if (gDPPB_LED_M14.phys.addr) iounmap((void *)gDPPB_LED_M14.phys.addr);
		if (gPE1_HDR_M14B.phys.addr) iounmap((void *)gPE1_HDR_M14B.phys.addr);
	}
	else
	{
		OS_Free((void *)gFRC_LRX_M14.shdw.addr);
		OS_Free((void *)gFRC_P3D_M14.shdw.addr);
		OS_Free((void *)gFRC_DVI_M14.shdw.addr);
		OS_Free((void *)gFRC_DVO_M14.shdw.addr);

		OS_Free((void *)gDPPB_TOP_M14.shdw.addr);
		OS_Free((void *)gDPPB_TXM_M14.shdw.addr);
		OS_Free((void *)gDPPB_LED_M14.shdw.addr);

	    	if (gFRC_LRX_M14.phys.addr) iounmap((void *)gFRC_LRX_M14.phys.addr);
	    	if (gFRC_P3D_M14.phys.addr) iounmap((void *)gFRC_P3D_M14.phys.addr);
	    	if (gFRC_DVI_M14.phys.addr) iounmap((void *)gFRC_DVI_M14.phys.addr);
		if (gFRC_DVO_M14.phys.addr) iounmap((void *)gFRC_DVO_M14.phys.addr);

	    	if (gDPPB_TOP_M14.phys.addr) iounmap((void *)gDPPB_TOP_M14.phys.addr);
	    	if (gDPPB_TXM_M14.phys.addr) iounmap((void *)gDPPB_TXM_M14.phys.addr);
	    	if (gDPPB_LED_M14.phys.addr) iounmap((void *)gDPPB_LED_M14.phys.addr);
	}

	return ret;
}

int BE_REG_HW_M14_Read(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;

	do{
		recvAddr  = addr;

		switch(type)
		{
			case BE_REG_RW_TYPE_DPPB_TXM:
				recvAddr += (UINT32)gDPPB_TXM_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_DPPB_TOP:
				recvAddr += (UINT32)gDPPB_TOP_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_DPPB_LED:
				recvAddr += (UINT32)gDPPB_LED_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_NONE:
			default :
				return RET_ERROR;
		}

		currAddr = recvAddr;
		/* for safety */
		currAddr >>= 2;
		currAddr <<= 2;

		value = REG_RD(currAddr);
		if(currAddr == recvAddr) break;
		nextValue = REG_RD(currAddr+4);
		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);
		value  = GET_BITS(value, dBit, wBit);
		value += GET_PVAL(nextValue, wBit, dBit);
	} while(0);

	*data = value;

	return ret;
}

int BE_REG_HW_M14_Write(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 data)
{
	int ret = RET_OK;
	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 currValue;
	UINT32 dBit;
	UINT32 wBit;

	do{
		recvAddr  = addr;
		value = data;

		switch(type)
		{
			case BE_REG_RW_TYPE_DPPB_TXM:
				recvAddr += (UINT32)gDPPB_TXM_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_DPPB_TOP:
				recvAddr += (UINT32)gDPPB_TOP_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_DPPB_LED:
				recvAddr += (UINT32)gDPPB_LED_M14.phys.addr;
				break;
			case BE_REG_RW_TYPE_NONE:
			default :
				return RET_ERROR;
		}

		currAddr = recvAddr;

		/* for safety */
		currAddr >>= 2;
		currAddr <<= 2;

		if(currAddr == recvAddr)
		{
			REG_WD(recvAddr, value);
			break;
		}
		currValue = REG_RD(currAddr);
		nextValue = REG_RD((currAddr+4));
		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);
		currValue  = GET_BITS(currValue, 0, dBit);
		currValue += GET_PVAL(value    , dBit, wBit);
		nextValue  = GET_PVAL(nextValue, dBit, wBit);
		nextValue += GET_BITS(value    , 0	 , dBit);
		REG_WD(currAddr	, currValue);
		REG_WD((currAddr+4), nextValue);
	}
	while(0);

	return ret;
}

/** @} */
