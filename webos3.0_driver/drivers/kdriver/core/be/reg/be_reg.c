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
 *  BE reg driver file for BE device
 *
 *  author		won.hur
 *  version		1.0
 *  date		2012.04.18
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_reg.h"
#include "be_kapi.h"
#include "h13/be/reg/be_reg_hw_h13.h"
#include "h14/be/reg/be_reg_hw_h14.h"
#include "m14/be/reg/be_reg_hw_m14.h"
#include "h15/be/reg/be_reg_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	int (*pfnInitializeREG)(void);
	int (*pfnCloseREG)(void);
	int (*pfnReadREG)(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 *data);
	int (*pfnWriteREG)(BE_REG_RW_TYPE_T type, UINT32 addr, UINT32 data);
} BE_REG_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_REG_HW_FUNCTION_T gstBeRegHwFunction;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_REG_Init(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV( H15, A0))
	{
		BE_PRINT("BE REG chip revision is set to H15 A0\n");
		gstBeRegHwFunction.pfnInitializeREG	= BE_REG_HW_H15_Initialize;
		gstBeRegHwFunction.pfnCloseREG		= BE_REG_HW_H15_Close;
		gstBeRegHwFunction.pfnReadREG		= BE_REG_HW_H15_Read;
		gstBeRegHwFunction.pfnWriteREG		= BE_REG_HW_H15_Write;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV( H14, A0))
	{
		BE_PRINT("BE REG chip revision is set to H14 A0\n");
		gstBeRegHwFunction.pfnInitializeREG	= BE_REG_HW_H14_Initialize;
		gstBeRegHwFunction.pfnCloseREG		= BE_REG_HW_H14_Close;
		gstBeRegHwFunction.pfnReadREG		= BE_REG_HW_H14_Read;
		gstBeRegHwFunction.pfnWriteREG		= BE_REG_HW_H14_Write;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV( M14, A0))
	{
		BE_PRINT("BE REG chip revision is set to M14 A0\n");
		gstBeRegHwFunction.pfnInitializeREG	= BE_REG_HW_M14_Initialize;
		gstBeRegHwFunction.pfnCloseREG		= BE_REG_HW_M14_Close;
		gstBeRegHwFunction.pfnReadREG		= BE_REG_HW_M14_Read;
		gstBeRegHwFunction.pfnWriteREG		= BE_REG_HW_M14_Write;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV( H13, A0))
	{
		BE_PRINT("BE REG chip revision is set to H13 A0\n");
		gstBeRegHwFunction.pfnInitializeREG	= BE_REG_HW_H13_Initialize;
		gstBeRegHwFunction.pfnCloseREG		= BE_REG_HW_H13_Close;
		gstBeRegHwFunction.pfnReadREG		= BE_REG_HW_H13_Read;
		gstBeRegHwFunction.pfnWriteREG		= BE_REG_HW_H13_Write;
	}
#endif
	else
	{
		BE_ERROR("BE REG ERROR! Unknown chip revision at BE REG module\n");
		gstBeRegHwFunction.pfnInitializeREG	= NULL;
		gstBeRegHwFunction.pfnCloseREG		= NULL;
		gstBeRegHwFunction.pfnReadREG		= NULL;
		gstBeRegHwFunction.pfnWriteREG		= NULL;

		ret = RET_ERROR;
	}

	if (gstBeRegHwFunction.pfnInitializeREG)
	{
		ret = gstBeRegHwFunction.pfnInitializeREG();
	}

	return ret;
}

int BE_REG_Close(void)
{
	int ret = RET_OK;

	if(gstBeRegHwFunction.pfnCloseREG)
	{
		ret = gstBeRegHwFunction.pfnCloseREG();
	}

	gstBeRegHwFunction.pfnInitializeREG	= NULL;
	gstBeRegHwFunction.pfnCloseREG		= NULL;
	gstBeRegHwFunction.pfnReadREG		= NULL;
	gstBeRegHwFunction.pfnWriteREG		= NULL;

	return ret;
}

int BE_REG_RegAccess(UINT32 arg)
{
	int ret = RET_ERROR;
	BE_REG_RW_T reg_addr_data_t;

	if ( copy_from_user(&reg_addr_data_t, (void __user *)arg, sizeof(BE_REG_RW_T)))
	{
		return -EFAULT;
	}

	BE_DEBUG("BE_REG_RegAccess(%d, %d, 0x%08x, 0x%08x) is called. \n", reg_addr_data_t.type, reg_addr_data_t.isRead, reg_addr_data_t.addr, reg_addr_data_t.data);

	if (reg_addr_data_t.isRead)
	{
		ret = gstBeRegHwFunction.pfnReadREG(reg_addr_data_t.type, reg_addr_data_t.addr, &reg_addr_data_t.data);
		if (ret == RET_OK)
		{
			ret = copy_to_user((void __user *)arg, &reg_addr_data_t, sizeof(BE_REG_RW_T));
		}
	}
	else
	{
		ret = gstBeRegHwFunction.pfnWriteREG(reg_addr_data_t.type, reg_addr_data_t.addr, reg_addr_data_t.data);
	}

	return ret;
}

