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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2011.02.23
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
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
#include <asm-generic/errno-base.h>
#include <linux/string.h>
#include <asm/io.h>

#include <linux/workqueue.h>

#include "os_util.h"

#include "de_model.h"
#include "de_ver_def.h"

#ifdef USE_KDRV_CODES_FOR_M14
#include "de_reg_def_m14.h"
#include "de_ipc_def_m14.h"

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

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_M14_Init(UINT32 mcu_id,           \
					UINT32 *pVideoBuffToMCU, \
					UINT32 *pVideoBuffFrMCU)
{
	int ret = RET_OK;
#ifdef USE_QEMU_SYSTEM
   *pVideoBuffToMCU     = (UINT32)OS_KMalloc(DTVSOC_IPC_FROM_CPU_SIZE);
   *pVideoBuffFrMCU     = (UINT32)OS_KMalloc(DTVSOC_IPC_FROM_MCU_SIZE);
#else
   switch(mcu_id)
   {
	   case 0:
		   if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) ) {
			   *pVideoBuffToMCU = (UINT32)ioremap( \
					   DTVSOC_IPC_FROM_CPU_M14B_BASE,\
					   DTVSOC_IPC_FROM_CPU_SIZE);
			   *pVideoBuffFrMCU = (UINT32)ioremap(	\
					   DTVSOC_IPC_FROM_MCU_M14B_BASE,\
					   DTVSOC_IPC_FROM_MCU_SIZE);
		   } else {
			   *pVideoBuffToMCU = (UINT32)ioremap( \
					   DTVSOC_IPC_FROM_CPU_M14_BASE,\
					   DTVSOC_IPC_FROM_CPU_SIZE);
			   *pVideoBuffFrMCU = (UINT32)ioremap(	\
					   DTVSOC_IPC_FROM_MCU_M14_BASE,\
					   DTVSOC_IPC_FROM_MCU_SIZE);
		   }
		   break;
	   case 1:
		   if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) ) {

		   } else {
			   *pVideoBuffToMCU = (UINT32)ioremap( \
					   DTVSOC_IPC_1_FROM_CPU_M14_BASE,\
					   IPC_1_FROM_CPU_SIZE);
			   *pVideoBuffFrMCU = (UINT32)ioremap(	\
					   DTVSOC_IPC_1_FROM_MCU_M14_BASE,\
					   IPC_1_FROM_MCU_SIZE);
		   }
		   break;
	   default:
		   ret = RET_ERROR;
		   break;
   }
#endif

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize IPC
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_M14_Free(UINT32 *pVideoBuffToMCU, UINT32 *pVideoBuffFrMCU)
{
#ifdef USE_QEMU_SYSTEM
   OS_Free(*pVideoBuffToMCU);
   OS_Free(*pVideoBuffFrMCU);
#else
	if (*pVideoBuffToMCU) iounmap((void *)(*pVideoBuffToMCU));
	if (*pVideoBuffFrMCU) iounmap((void *)(*pVideoBuffFrMCU));
#endif

	return RET_OK;
}
#endif

/**  @} */
