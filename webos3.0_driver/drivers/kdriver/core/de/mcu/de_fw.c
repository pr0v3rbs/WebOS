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
 * date       2010.07.16
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "proc_util.h"
#include "debug_util.h"

#include "de_kapi.h"
#include "de_model.h"
#include "de_def.h"

#include "de_fw_def.h"
#include "hal/de_hal.h"
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
#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_FW_Download(int fromFile)
{
	int ret = RET_OK;
	UINT32 count = 0;
	#define FW_DOWNLOAD_TIMEOUT	1000

	if (fromFile) {
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_FILE
		ret = DE_HAL_FW_DownloadFromBin();
#endif
	} else {
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
		ret = DE_HAL_FW_DownloadFromHead();
#endif
	}

	while (1) {
		if (DE_HAL_IPCisAlive()) break;
		msleep(1);
		count++;
		if(count == FW_DOWNLOAD_TIMEOUT)
		{
			if (fromFile) {
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_FILE
				ret = DE_HAL_FW_DownloadFromBin();
#endif
			} else {
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
				ret = DE_HAL_FW_DownloadFromHead();
#endif
			}
		}
		if(count == (FW_DOWNLOAD_TIMEOUT<<1))
		{
			ret = RET_TIMEOUT;
			break;
		}
	}

	return ret;
}

int	DE_FW_DownloadFromFile( DE_FW_FILE_T *pFwFile )
{
	int ret = RET_OK;
	char*		fw_data;
	int			file_size;
	OS_FILE_T	fw_file;
	LX_DE_FW_DWLD_T stParams;

	do {
		CHECK_KNULL(pFwFile);
		CHECK_KNULL(pFwFile->fwName);
		/* open config file and read */
		if ( RET_OK != OS_OpenFile( &fw_file, pFwFile->fwName, O_RDONLY | O_LARGEFILE, 0666 ) )
		{
			DE_ERROR("<error> can't open fw_file (%s)\n", pFwFile->fwName );
			ret =  RET_ERROR;
			break;
		}

		file_size = OS_SizeOfFile( &fw_file );
		if (file_size < 0) {
			ret = file_size;
			break;
		}
		fw_data = OS_KMalloc( file_size );
		OS_ReadFile (&fw_file, fw_data, file_size );
		OS_CloseFile( &fw_file );

		//(void)_OS_DEBUG_SetPrintMask( fw_data );
		stParams.inx   = pFwFile->inx;
		stParams.size  = file_size;
		stParams.pData = fw_data;
		ret = DE_HAL_FW_Download(&stParams);
		OS_KFree( fw_data );
	} while (0);

	return ret;
}

#endif

/**  @} */
