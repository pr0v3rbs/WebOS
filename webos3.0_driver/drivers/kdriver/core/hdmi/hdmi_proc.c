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
 *  Linux proc interface for hdmi device.
 *	hdmi device will teach you how to make device driver with new platform.
 *
 *  author		sh.myoung (sh.myoung@lge.com)
 *  version		1.0
 *  date		2010.01.06
 *  note		Additional information.
 *
 *  @addtogroup lg1150_hdmi
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//#include <asm/uaccess.h>
#include "hdmi_drv.h"
#include "hdmi_module.h"
#if 0
#include"l9/afe/vport_i2c_l9a0.h"
#endif

#include "proc_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR  ,
	PROC_ID_COMMAND ,
	PROC_ID_READ ,
	PROC_ID_WRITE ,
	PROC_ID_TIMING	,
	PROC_ID_VERSION	,
	PROC_ID_AVI 	,
	PROC_ID_MAX
};

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
static OS_PROC_DESC_TABLE_T	_g_hdmi_device_proc_table[] =
{
	{ "author",			PROC_ID_AUTHOR  , OS_PROC_FLAG_READ },
	{ "command",		PROC_ID_COMMAND , OS_PROC_FLAG_WRITE },
	{ "w",				PROC_ID_WRITE	, OS_PROC_FLAG_WRITE },
	{ "r",				PROC_ID_READ	, OS_PROC_FLAG_WRITE },
	{ "version",		PROC_ID_VERSION	, OS_PROC_FLAG_READ },
	{ "info_timing",	PROC_ID_TIMING	, OS_PROC_FLAG_READ },
	{ "info_AVI",		PROC_ID_AVI 	, OS_PROC_FLAG_READ },
	{ NULL, 			PROC_ID_MAX		, 0 }
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * read_proc implementation of hdmi device
 *
*/
static int	_HDMI_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret= RET_ERROR;
	int	len;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "sh.myoung (sh.myoung@lge.com)" );
		}
		break;

		case PROC_ID_VERSION:
		{
#if 0
#ifndef PLATFORM_M14_FPGA
			VPORT_I2C_Read((UINT32*)&gl9a_version_0);
			ret = sprintf( buffer, "version 0x%x\n", gl9a_version_0.l9a_version );
#endif
#endif
		}
		break;

		case PROC_ID_TIMING:
		{
			LX_HDMI_TIMING_INFO_T info;

			info.prt_num = -1;	// previous selected port

			ret = HDMI_GetTimingInfo(&info);

			if (ret == 0)
			{
				len = 0;
				len += sprintf( buffer +len, "H Freq 0x%x * 100Hz\n", info.hFreq);
				len += sprintf( buffer +len, "V Freq 0x%x * 1/10Hz\n", info.vFreq);
				len += sprintf( buffer +len, "H Total 0x%x pixels\n", info.hTotal);
				len += sprintf( buffer +len, "V Total 0x%x lines\n", info.vTotal);
				len += sprintf( buffer +len, "H Start 0x%x pixels\n", info.hStart);
				len += sprintf( buffer +len, "V Start 0x%x lines\n", info.vStart);
				len += sprintf( buffer +len, "H Active 0x%x pixels\n", info.hActive);
				len += sprintf( buffer +len, "V Active 0x%x lines\n", info.vActive);
				len += sprintf( buffer +len, "Scan type %x [0:interlace, 1:progressive\n", info.scanType);

				ret = len;
			}
			else
			{
				ret = sprintf( buffer, "error %d\n", ret);
			}
		}
		break;

		case PROC_ID_AVI:			
		{
			ret = -1;
			ret = sprintf( buffer, "error(not support) %d\n",ret);
		}
		break;

		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}

	return ret;
}

/*
 * write_proc implementation of hdmi device
 *
*/
static int _HDMI_WriteProcFunction( UINT32 procId, char* command )
{
#if 0
	VPORT_I2C_DATA_T i2cData = {0, };
	UINT32	a, b, c;
#endif

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			printk("command string : %s\n", command );
		}
		break;

		case PROC_ID_WRITE:
		{
#if 0
			sscanf( command, "%x %x %x", &a, &b, &c);
			i2cData.slaveAddr = a;
			i2cData.regAddr = b;
			i2cData.data = c;

#ifndef PLATFORM_M14_FPGA
			VPORT_I2C_Write((UINT32*)&i2cData);
#endif
			printk("Writing Slave[0x%x] reg[0x%x] data : 0x%x\n", i2cData.slaveAddr, i2cData.regAddr, i2cData.data );
#endif
		}
		break;

		case PROC_ID_READ:
		{
#if 0
			sscanf( command, "%x %x", &a, &b);
			i2cData.slaveAddr = a;
			i2cData.regAddr = b;

#ifndef PLATFORM_M14_FPGA
			VPORT_I2C_Read((UINT32*)&i2cData);
#endif

			printk("Reading Slave[0x%x] reg[0x%x] data : 0x%x\n", i2cData.slaveAddr, i2cData.regAddr, i2cData.data );
#endif
		}
		break;

		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for hdmi device
 *
 * @see HDMI_Init
*/
void	HDMI_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( HDMI_MODULE, _g_hdmi_device_proc_table,
											_HDMI_ReadProcFunction,
											_HDMI_WriteProcFunction );
}

/**
 * cleanup proc utility for hdmi device
 *
 * @see HDMI_Cleanup
*/
void	HDMI_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( HDMI_MODULE );
}

/** @} */

