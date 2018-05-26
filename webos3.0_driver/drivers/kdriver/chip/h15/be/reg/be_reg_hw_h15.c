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
#include "be_reg_hw_h15.h"
#include "be_reg_def_h15.h"
#include "be_reg_dpe_h15.h"
#include "be_reg_pwm_h15.h"

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

} BE_REG_HW_H15_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
PE_PWM_REG_H15_T gPE_PWM_H15;
DPE_LED_REG_H15_T gDPE_LED_H15;

PE1_DRC_REG_H15_T gPE1_DRC_H15;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_REG_HW_H15_INFO_T gstBeRegHwH15Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_REG_HW_H15_Initialize(void)
{
	int ret = RET_OK;

	do {
		gPE_PWM_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(PE_PWM_REG_H15A0_T));
		gDPE_LED_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(DPE_LED_REG_H15A0_T));
		gPE1_DRC_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(PE1_DRC_REG_H15A0_T));

		gPE_PWM_H15.phys.addr = (volatile UINT32 *)ioremap(PE_PWM_REG_H15A0_BASE, sizeof(PE_PWM_REG_H15A0_T));
		gDPE_LED_H15.phys.addr = (volatile UINT32 *)ioremap(DPE_LED_REG_H15A0_BASE, sizeof(DPE_LED_REG_H15A0_T));
		gPE1_DRC_H15.phys.addr = (volatile UINT32 *)ioremap(PE1_DRC_REG_H15A0_CONTROL_ADDR, sizeof(PE1_DRC_REG_H15A0_T));
	} while(0);

	return ret;
}

int BE_REG_HW_H15_Close(void)
{
	int ret = RET_OK;

	OS_Free((void *)gPE_PWM_H15.shdw.addr);
	OS_Free((void *)gDPE_LED_H15.shdw.addr);
	OS_Free((void *)gPE1_DRC_H15.shdw.addr);

	if (gPE_PWM_H15.phys.addr) iounmap((void *)gPE_PWM_H15.phys.addr);
	if (gDPE_LED_H15.phys.addr) iounmap((void *)gDPE_LED_H15.phys.addr);
	if (gPE1_DRC_H15.phys.addr) iounmap((void *)gPE1_DRC_H15.phys.addr);

	return ret;
}

int BE_REG_HW_H15_Read(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 *data)
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
			case BE_REG_RW_TYPE_DPPB_LED:
				recvAddr += (UINT32)gDPE_LED_H15.phys.addr;
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

int BE_REG_HW_H15_Write(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 data)
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
			case BE_REG_RW_TYPE_DPPB_LED:
				recvAddr += (UINT32)gDPE_LED_H15.phys.addr;
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
