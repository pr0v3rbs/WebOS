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
 *	BE SRE module control Drivers linux module
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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>		/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>		/**< For ioremap_nocache */
#include "os_util.h"

#include "be_dbg.h"
#include "be_def.h"

#include "../fwi/ipc_def_h14.h"
#include "ipc_drv.h"
#include "be_sre_hw_h14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

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
	UINT8 u8SrControl;
	UINT8 u8InputResolution;	//0:HD, 1:SD
	UINT8 u8Input3DMode;	//0:2D, 1:3D
	UINT8 u8DbGrade;
	UINT8 u8SrStrengthLvl;
	UINT8 u8SrAGlobalScale;
	UINT8 u8SrBGlobalScale;
	UINT8 u8ModeLocalScale;
	UINT8 u8SrALocalScale;
	UINT8 u8SrBLocalScale;
	UINT8 u8PP_XFilter;
	UINT8 u8PP_YFilter;
	UINT8 u8PP_GlobalScale;
	UINT8 u8PP_LocalScale;
} BE_SRE_HW_H14_INFO_T;

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
static BE_SRE_HW_H14_INFO_T	_beSREHwH14Info =
{
	.u8SrControl 		= 0xFF,
	.u8InputResolution = 0xFF,
	.u8Input3DMode 	= 0xFF,
	.u8DbGrade 		= 0xFF,
	.u8SrStrengthLvl 	= 0xFF,
	.u8SrAGlobalScale	= 0xFF,
	.u8SrBGlobalScale	= 0xFF,
	.u8ModeLocalScale= 0xFF,
	.u8SrALocalScale 	= 0xFF,
	.u8SrBLocalScale 	= 0xFF,
	.u8PP_XFilter 		= 0xFF,
	.u8PP_YFilter 		= 0xFF,
	.u8PP_GlobalScale = 0xFF,
	.u8PP_LocalScale 	= 0xFF
}; 

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_SRE_HW_H14_Initialize(void)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( H14, B0))
		{

		}
		else
		{

		}
	} while (0);

	return ret;
}

int BE_SRE_HW_H14_Close(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( H14, B0))
	{

	}
	else
	{

	}

	return ret;
}	

int BE_SRE_HW_H14_Set_Command(LX_BE_SRE_CMN_T *pstParams)
{
	int ret = RET_OK;
	int updateCount = 0; 

	IPC_SET_GET_PARAM(BE_IPC_H3D_MODE_T, stIpcH3dModeParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_GLOBAL_SCALE_T, stGScaleParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_INPUT_MODE_T, stInputModeParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_UPDATE_ALL_T, stUpdateParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_CONTROL_T, stCtrlParams);

	do {
		/* change value LX_BE to BE_IPC */
		/* Step 1-1. */
		stCtrlParams.u4Control = (UINT32)pstParams->u8SrControl;

		/* Step 1-2. */
		stGScaleParams.u8SRAGlobalScale = (UINT16)pstParams->u8SrAGlobalScale;
		stGScaleParams.u8SRBGlobalScale = (UINT16)pstParams->u8SrBGlobalScale;

		/* Step 1-3. */
		switch(pstParams->u8ResolutionType)
		{
			case 0 : // FHD
				stInputModeParams.u4InputResolution = 0;
				break;
			case 1 : // SD
				stInputModeParams.u4InputResolution = 1;
				break;
			case 2 : // UD
			default :
				stInputModeParams.u4InputResolution = 2;
				break;
		}

		ret = BE_KIPC_SetData(BE_H3D_SET_MODE, (void *)&stIpcH3dModeParams, sizeof(BE_IPC_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stIpcH3dModeParams, sizeof(BE_IPC_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);

		if (stIpcH3dModeParams.bA3CMode || stIpcH3dModeParams.bT3CMode)
		{
			stInputModeParams.u4Input3DMode    = 1;	// 3D
		}
		else
		{
			stInputModeParams.u4Input3DMode    = 0;	// 2D
		}

		/* Send command  to MCU via IPC & Check result */
		/* Step 2-1.  */
		if (_beSREHwH14Info.u8SrControl != (UINT8)stCtrlParams.u4Control)
		{
			/* return unexpected value */
			if (*(ULONG *)&stCtrlParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_CONTROL, &stCtrlParams, sizeof(BE_IPC_SRE_CONTROL_T)); //added
			if(ret) BREAK_WRONG(ret);

			_beSREHwH14Info.u8SrControl = (UINT8)stCtrlParams.u4Control;
			updateCount++;
		}

		/* Step 2-2.  */
		if ( (_beSREHwH14Info.u8SrAGlobalScale != (UINT8)stGScaleParams.u8SRAGlobalScale)
			|| (_beSREHwH14Info.u8SrBGlobalScale != (UINT8)stGScaleParams.u8SRBGlobalScale) )
		{
			/* return unexpected value */
			if (*(ULONG *)&stGScaleParams== (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			//BE_SRE_HW_H14_Set_Global_Scale(&stGScaleParams);
			ret = BE_KIPC_SetData(BE_SRE_GLOBAL_SCALE, &stGScaleParams, sizeof(BE_IPC_SRE_GLOBAL_SCALE_T));
			if(ret) BREAK_WRONG(ret);

			_beSREHwH14Info.u8SrAGlobalScale = (UINT8)stGScaleParams.u8SRAGlobalScale;
			_beSREHwH14Info.u8SrBGlobalScale = (UINT8)stGScaleParams.u8SRBGlobalScale;
			updateCount++;
		}

		/* Step 2-3.  */
		if ( (_beSREHwH14Info.u8InputResolution != (UINT8)stInputModeParams.u4InputResolution)
			|| (_beSREHwH14Info.u8Input3DMode != (UINT8)stInputModeParams.u4Input3DMode) )
		{
			/* return unexpected value */
			if (*(ULONG *)&stInputModeParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_SET_INPUT_MODE, &stInputModeParams, sizeof(BE_IPC_SRE_INPUT_MODE_T));
			if(ret) BREAK_WRONG(ret);
			_beSREHwH14Info.u8InputResolution = (UINT8)stInputModeParams.u4InputResolution;
			_beSREHwH14Info.u8Input3DMode    = (UINT8)stInputModeParams.u4Input3DMode;
			updateCount++;
		}

		/* Step 2-4.  */
		if (updateCount)
		{
			stUpdateParams.bUpdate = TRUE;
			/* return unexpected value */
			if (*(ULONG *)&stUpdateParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_UPDATE_ALL, &stUpdateParams, sizeof(BE_IPC_SRE_UPDATE_ALL_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_SRE_HW_H14_Set_Params(LX_BE_SRE_PRAM_T *pstParams)
{
	int ret = RET_OK;
	int updateCount = 0;

	IPC_SET_GET_PARAM(BE_IPC_H3D_MODE_T, stIpcH3dModeParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_DB_GRADE_T, stDbGradeParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_LOCAL_SCALE_T, stLScaleParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_PREPROCESSING_T, stPreProcParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_INPUT_MODE_T, stInputModeParams);
	IPC_SET_GET_PARAM(BE_IPC_SRE_UPDATE_ALL_T, stUpdateParams);

	do {
		/* Step 1. Set value for argument */
		/* Step 1-1. */
		stDbGradeParams.u8DBGrade = (UINT32)pstParams->u8DbGrade;

		/* Step 1-2. */
		stLScaleParams.u4ModeLocalScale = pstParams->u8ModeLocalScale;
		stLScaleParams.u4SRALocalScale   = pstParams->u8SrALocalScale;
		stLScaleParams.u4SRBLocalScale   = pstParams->u8SrBLocalScale;

		/* Step 1-3. */
		stPreProcParams.u8XFilter 	  	= pstParams->u8PP_XFilter;
		stPreProcParams.u8YFilter	  	= pstParams->u8PP_YFilter;
		stPreProcParams.u8GlobalScale	= pstParams->u8PP_GlobalScale;
		stPreProcParams.u8LocalScale	= pstParams->u8PP_LocalScale;

		/* Step 1-4. */
		switch(pstParams->u8ResolutionType)
		{
			case 0 : // FHD
				stInputModeParams.u4InputResolution = 0;
				break;
			case 1 : // SD
				stInputModeParams.u4InputResolution = 1;
				break;
			case 2 : // UD
			default :
				stInputModeParams.u4InputResolution = 2;
				break;
		}
		
		ret = BE_KIPC_SetData(BE_H3D_SET_MODE, (void *)&stIpcH3dModeParams, sizeof(BE_IPC_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stIpcH3dModeParams, sizeof(BE_IPC_H3D_MODE_T));
		if(ret) BREAK_WRONG(ret);

		if (stIpcH3dModeParams.bA3CMode || stIpcH3dModeParams.bT3CMode)
		{
			stInputModeParams.u4Input3DMode    = 1;	// 3D
		}
		else
		{
			stInputModeParams.u4Input3DMode    = 0;	// 2D
		}

		/* Step 2.  Send command  to MCU via IPC & Check result */
		/* Step 2-1.  */
		if ( _beSREHwH14Info.u8DbGrade != (UINT8)stDbGradeParams.u8DBGrade )
		{
			/* return unexpected value */
			if (*(ULONG *)&stDbGradeParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}

			ret = BE_KIPC_SetData(BE_SRE_SET_DB_GRADE, &stDbGradeParams, sizeof(BE_IPC_SRE_DB_GRADE_T));
			if(ret) BREAK_WRONG(ret);
			_beSREHwH14Info.u8DbGrade = (UINT8)stDbGradeParams.u8DBGrade;
			updateCount++;
		}

		/* Step 2-2.  */
		if ( (_beSREHwH14Info.u8ModeLocalScale != stLScaleParams.u4ModeLocalScale)
			|| (_beSREHwH14Info.u8SrALocalScale != stLScaleParams.u4SRALocalScale)
			|| (_beSREHwH14Info.u8SrBLocalScale != stLScaleParams.u4SRBLocalScale) )
		{
				/* return unexpected value */
				if (*(ULONG *)&stLScaleParams == (ULONG)0xFFFFFFFF)
				{
					return ret;
				}
			ret = BE_KIPC_SetData(BE_SRE_LOCAL_SCALE, &stLScaleParams, sizeof(BE_IPC_SRE_LOCAL_SCALE_T));
			if(ret) BREAK_WRONG(ret);
			_beSREHwH14Info.u8ModeLocalScale = stLScaleParams.u4ModeLocalScale;
			_beSREHwH14Info.u8SrALocalScale    = stLScaleParams.u4SRALocalScale;
			_beSREHwH14Info.u8SrBLocalScale    = stLScaleParams.u4SRBLocalScale;
			updateCount++;
		}

		/* Step 2-3.  */
		if ( (_beSREHwH14Info.u8PP_XFilter != stPreProcParams.u8XFilter)
			|| (_beSREHwH14Info.u8PP_YFilter != stPreProcParams.u8YFilter)
			|| (_beSREHwH14Info.u8PP_GlobalScale != stPreProcParams.u8GlobalScale)
			|| (_beSREHwH14Info.u8PP_LocalScale  != stPreProcParams.u8LocalScale) )
		{
			/* return unexpected value */
			if (*(ULONG *)&stPreProcParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_PREPROCESSING, &stPreProcParams, sizeof(BE_IPC_SRE_PREPROCESSING_T));
			if(ret) BREAK_WRONG(ret);
			_beSREHwH14Info.u8PP_XFilter 		= stPreProcParams.u8XFilter;
			_beSREHwH14Info.u8PP_YFilter    	= stPreProcParams.u8YFilter;
			_beSREHwH14Info.u8PP_GlobalScale   = stPreProcParams.u8GlobalScale;
			_beSREHwH14Info.u8PP_LocalScale    = stPreProcParams.u8LocalScale;
			updateCount++;
		}

		/* Step 2-4.  */
		if ( (_beSREHwH14Info.u8InputResolution != (UINT8)stInputModeParams.u4InputResolution)
			|| (_beSREHwH14Info.u8Input3DMode != (UINT8)stInputModeParams.u4Input3DMode) )
		{
			/* return unexpected value */
			if (*(ULONG *)&stInputModeParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_SET_INPUT_MODE, &stInputModeParams, sizeof(BE_IPC_SRE_INPUT_MODE_T));
			if(ret) BREAK_WRONG(ret);
			_beSREHwH14Info.u8InputResolution = (UINT8)stInputModeParams.u4InputResolution;
			_beSREHwH14Info.u8Input3DMode    = (UINT8)stInputModeParams.u4Input3DMode;
			updateCount++;
		}

		/* Step 2-5.  */
		if (updateCount)
		{
			stUpdateParams.bUpdate = TRUE;
			/* return unexpected value */
			if (*(ULONG *)&stUpdateParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}
			ret = BE_KIPC_SetData(BE_SRE_UPDATE_ALL, &stUpdateParams, sizeof(BE_IPC_SRE_UPDATE_ALL_T));
			if(ret) BREAK_WRONG(ret);			
		}
	} while (0);

	return ret;
}

int BE_SRE_HW_H14_Get_Version(LX_BE_SRE_VERSION_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_SRE_VERSION_T, stHwParams);

	do {
		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_SRE_GET_VERSION, &stHwParams, sizeof(BE_IPC_SRE_VERSION_T));
		if (ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_SRE_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		/* change value BE_IPC to LX_BE */
		pstParams->u16Version		= stHwParams.u16Version;
		pstParams->u16SubVersion	= stHwParams.u16Version;
	} while (0);

	return ret;
}

int BE_SRE_HW_H14_Set_Input_Format(LX_BE_SRE_INPUT_MODE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_SRE_INPUT_MODE_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.u4InputResolution	= pstParams->u16InputResolution;
		stHwParams.u4Input3DMode	= pstParams->u16Input3DMode;

		/* return unexpected value */
		if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_SRE_SET_INPUT_MODE, &stHwParams, sizeof(BE_IPC_SRE_INPUT_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

/** @} */
