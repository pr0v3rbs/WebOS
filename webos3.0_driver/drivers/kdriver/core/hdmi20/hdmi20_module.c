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
 *  Brief description. 
 *  Detailed description starts here. 
 *
 *  @author		won.hur
 *  @version	1.0 
 *  @date		2014-05-07
 *  @note		Additional information. 
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "os_util.h"

#include "hdmi20_kapi.h"
#include "hdmi20_drv.h"
#include "../../chip/h15/hdmi20/hdmi20_hal_h15ax.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define H15AX_HDMI20_INTERRUPT_MASK	0x00000000

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int (*HDMI20_Chip_Process_Cmd)(LX_HDMI20_CMD_T* cmd);
int (*HDMI20_Chip_Suspend)(void);
int (*HDMI20_Chip_Resume)(void);
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

/*========================================================================================
	Implementation Group
========================================================================================*/

int HDMI20_Module_InitChipRev(void)
{
	int ret = RET_OK;

	do{
		if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
		{
			HDMI20_INFO("[%s:%d] chip revision is set to H15 A0\n", __F__, __L__);
			HDMI20_Chip_Process_Cmd = HDMI20_H15A0_Process_Cmd;
			HDMI20_Chip_Suspend = HDMI20_H15A0_Suspend;
			HDMI20_Chip_Resume = HDMI20_H15A0_Resume;
		}
		else
		{
			HDMI20_ERROR("[%s:%d] Error! Unknown chip revision \n", __F__, __L__);
			HDMI20_Chip_Process_Cmd = NULL;
			HDMI20_Chip_Suspend = NULL;
			HDMI20_Chip_Resume = NULL;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

int HDMI20_Module_Suspend(void)
{
	int ret = RET_ERROR;

	do{
		/* Step 1. Check if Function pointer is NULL */
		if(HDMI20_Chip_Suspend == NULL){
			HDMI20_ERROR("[%s:%d] Error! HDMI20 chip ID was not selected!\n", __F__, __L__);
			ret = RET_OK;
			break;
		}
		
		/* Step 2. Execute Chip driver */
		ret = HDMI20_Chip_Suspend();

	} while(0);

	return ret;

}

int HDMI20_Module_Resume(void)
{
	int ret = RET_ERROR;

	do{
		/* Step 1. Check if Function pointer is NULL */
		if(HDMI20_Chip_Resume == NULL){
			HDMI20_ERROR("[%s:%d] Error! HDMI20 chip ID was not selected!\n", __F__, __L__);
			ret = RET_OK;
			break;
		}
		
		/* Step 2. Execute Chip driver */
		ret = HDMI20_Chip_Resume();

	} while(0);

	return ret;
}

int HDMI20_Module_Process_Cmd(LX_HDMI20_CMD_T* cmd)
{
	int ret = RET_ERROR;

	do{
		/* Step 1. Check if Function pointer is NULL */
		if(HDMI20_Chip_Process_Cmd == NULL){
			HDMI20_ERROR("[%s:%d] Error! HDMI20 chip ID was not selected!\n", __F__, __L__);
			ret = RET_ERROR;
			break;
		}
		
		/* Step 2. Execute Chip driver */
		ret = HDMI20_Chip_Process_Cmd((LX_HDMI20_CMD_T *)cmd);

	} while(0);

	return ret;
}

