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
 *  hal api implementation for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2012.07.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <asm/io.h>
#include "os_util.h"
#include "venc_drv.h"
#include "venc_hal.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#define VENC_CHECK_CODE(__checker, __if_action, fmt, args...) \
			__CHECK_IF_ERROR(__checker, VENC_ERROR, __if_action, fmt, ##args)

#define E_INTER_EN						0x0C00		// IPC, VSYNC

#define RUN_API(__function, args...) ( pstHalApi != NULL && pstHalApi->__function != NULL) ? \
													pstHalApi->__function(args) : RET_ERROR
#define CHECK_API(__function)		 ( pstHalApi != NULL && pstHalApi->__function != NULL ) ? TRUE : FALSE

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/
VENC_HAL_API_T *VENC_HAL_InitializeH13Bx( void );

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
VENC_HAL_API_T * pstHalApi;

/*========================================================================================
	Implementation Group
========================================================================================*/
static int _HAL_APIInit( void )
{
	if (lx_chip_rev() >= LX_CHIP_REV(H13,B0))
	{
		pstHalApi = VENC_HAL_InitializeH13Bx();
	} 
	else
	{
		pstHalApi = NULL;
	}

	if ( pstHalApi == NULL )
	{
		return RET_ERROR;
	}

	VENC_INFO("VENC API VER: 0x%08x\n", pstHalApi->version);
	
	return RET_OK;
}

int VENC_HAL_DeviceInitialize ( void )
{
	if ( _HAL_APIInit() != RET_OK )
	{
		return RET_ERROR;
	}

	return RUN_API( DeviceInitialize );
}

int VENC_HAL_DeviceFinalize ( void )
{
	return RUN_API( DeviceFinalize );
}

int VENC_HAL_Initialize( void )
{
	return RUN_API( Initialize );
}

int VENC_HAL_Suspend( void )
{
	return RUN_API( Suspend );
}

int VENC_HAL_Resume( void )
{
	return RUN_API( Resume );
}

int VENC_HAL_Open( int channel )
{
	return RUN_API( Open, channel );
}

int VENC_HAL_Close( int channel )
{
	return RUN_API( Close, channel );
}

int VENC_HAL_SetInputConfig ( int channel, LX_VENC_RECORD_INPUT_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( SetInputConfig, channel, pstParams );
}

int VENC_HAL_SetCommand ( int channel, LX_VENC_RECORD_COMMAND_T eParam )
{
	return RUN_API( SetCommand, channel, eParam);
}

int VENC_HAL_SetGOP ( int channel, UINT32 ui32Param )
{
	return RUN_API( SetGOP, channel, ui32Param );
}

int VENC_HAL_SetRateControl( int channel, LX_VENC_RECORD_RC_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( SetRateControl, channel, pstParams->eRcType, pstParams->value );
}

int VENC_HAL_SetEncodeType( int channel, LX_VENC_ENCODE_TYPE_T eParam )
{
	return RUN_API( SetEncodeType, channel, eParam );
}

int VENC_HAL_GetBufferInfo ( int channel, LX_VENC_BUFFER_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( GetBufferInfo, channel, pstParams );
}

int VENC_HAL_GetEncodeInfo( int channel, LX_VENC_RECORD_INFO_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( GetEncodeInfo, channel, pstParams );
}

int VENC_HAL_GetFrameImage( int channel, LX_VENC_RECORD_FRAME_IMAGE_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( GetFrameImage, channel, pstParams );
}

int VENC_HAL_GetOutputBuffer( int channel, LX_VENC_RECORD_OUTPUT_T *pstParams )
{
	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	return RUN_API( GetOutputBuffer, channel, pstParams );
}

int VENC_HAL_CheckIPCRegister( int channel )
{
	return RUN_API( CheckIPCRegister, channel );
}

int VENC_HAL_PROC_ReadDeviceStatus( char * buffer )
{
	if ( buffer == NULL )
	{
		return 0;
	}

	return RUN_API( PROC_ReadDeviceStatus, buffer );
}

int VENC_HAL_PROC_ReadMemoryStatus( char * buffer )
{
	if ( buffer == NULL )
	{
		return 0;
	}

	return RUN_API( PROC_ReadMemoryStatus, buffer );
}

irqreturn_t VENC_HAL_ISRHandler( void )
{
	return RUN_API( ISRHandler );
}

#ifdef SUPPORT_VENC_DEVICE_ENC_API
int VENC_HAL_ENC_Create( int channel, LX_VENC_ENC_CREATE_T *pstParams )
{
	return RUN_API( ENC_Create, channel, pstParams );
}

int VENC_HAL_ENC_Destroy( int channel, LX_VENC_ENC_DESTROY_T *pstParams )
{
	return RUN_API( ENC_Destroy, channel, pstParams );
}

int VENC_HAL_ENC_Encode( int channel, LX_VENC_ENC_ENCODE_T *pstParams )
{
	return RUN_API( ENC_Encode, channel, pstParams );
}
#endif

/*-----------------------------------------------------------------------------
	Static Function Definition
-----------------------------------------------------------------------------*/


/** @} */

