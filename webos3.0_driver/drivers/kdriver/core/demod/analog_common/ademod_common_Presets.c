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

/*****************************************************************************
**
**  Name: ademod_common_Preset.c
**
**  Description:   Preset user settings.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_Preset_Set
**
**  References:     
**
**  Exports:  
**
**  Dependencies:   ademod_common.h 
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/


#if defined( __cplusplus )
extern "C"
{
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_Menu.h"
#include "ademod_common_Presets.h"


LX_ADEMOD_Result ADEMOD_Preset_Set(Handle_t hDevice, LX_ADEMOD_SmartTuneDescriptor *pSmartTune, UINT32 Section)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 smartTuneFilter = 0;
	
	DEMOD_PRINT("[KDRV_DEMOD] ADEMOD_Preset_Set Start !! \n");

	if(pSmartTune == NULL) smartTuneFilter = 0;
	else  smartTuneFilter = pSmartTune->Filter;

	if(NULL == g_demod_hal.ADEMOD_Preset_Set)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}

	status = g_demod_hal.ADEMOD_Preset_Set(hDevice, smartTuneFilter, Section );

	DEMOD_PRINT("[KDRV_DEMOD] ADEMOD_Preset_Set End !! \n");

	
	return status;
}

#if defined( __cplusplus )
}
#endif
