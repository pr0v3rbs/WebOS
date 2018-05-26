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
 *  @date		2014-05-19
 *  @note		Additional information. 
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/interrupt.h>
#include "os_util.h"

#include "hdmi20_hal_h15ax.h"
#include "hdmi20_drv.h"

#include "hdmi20_tx_port.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define HDMI20_TX_INPUT_MUX_MASK	0x0000001F

#define HDMI20_TX_INPUT_OIF			0x00000018
#define HDMI20_TX_INPUT_OSD			0x00000014
#define HDMI20_TX_INPUT_MAINVIDEO	0x00000012
#define HDMI20_TX_INPUT_SUBVIDEO	0x00000011	

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern HDMI20_HAL_TX_DEVICE_DATA gHDMI20TxHandler;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
UINT32 HDMI20_HAL_H15Ax_Tx_TOP_Read_Port(UINT32 addr)
{
	UINT32 ret;
	
	if(gHDMI20TxHandler.topmem.pSwAddr != NULL){
		ret = (UINT32)(*((UINT32 *)((UINT32)gHDMI20TxHandler.topmem.pSwAddr + (UINT32)addr)));
	}
	else{
		HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
		ret = 0;
	}

	return ret;
}

void   HDMI20_HAL_H15Ax_Tx_TOP_Write_Port(UINT32 addr, UINT32 data)
{
	UINT32* dest;

	if(gHDMI20TxHandler.topmem.pSwAddr != NULL){
		dest = 	((UINT32 *)((UINT32)gHDMI20TxHandler.topmem.pSwAddr + (UINT32)addr));
		*dest = data;	
	}
	else{
		HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n",__F__ , __L__);
	}
}

int	HDMI20_HAL_H15Ax_Tx_InputMux(int isRead, LX_HDMI20_TX_INPUTMUX_TYPE_T *type)
{
	int ret = RET_ERROR;
	unsigned int value = 0;
	LX_HDMI20_TX_INPUTMUX_TYPE_T currentType;

	do {
		if((unsigned int)type == 0) {
			HDMI20_ERROR("Warning! Null pointer recieved!\n");
			break;
		}

		value  = HDMI20_HAL_H15Ax_Tx_TOP_Read_Port(HDMI20_TX_H15AX_AD_MAIN_MUX_CTRL);

		if(isRead){
			value &= HDMI20_TX_INPUT_MUX_MASK;

			switch(value)
			{
				case HDMI20_TX_INPUT_OIF:	
					currentType = LX_HDMI20_TX_INPUT_INTERLACED_OIF;
					ret = RET_OK;
					break;
				case HDMI20_TX_INPUT_OSD:	
					currentType = LX_HDMI20_TX_INPUT_OSD;
					ret = RET_OK;
					break;
				case HDMI20_TX_INPUT_MAINVIDEO:
					currentType = LX_HDMI20_TX_INPUT_MAINVIDEO;
					ret = RET_OK;
					break;
				case HDMI20_TX_INPUT_SUBVIDEO:
					currentType = LX_HDMI20_TX_INPUT_SUBVIDEO;
					ret = RET_OK;
					break;
				default:
					HDMI20_ERROR("Unknown read value[0x%08x]\n", value);
					currentType = value;				
					break;
			}
			
			ret = copy_to_user((void *)type, &currentType, sizeof(LX_HDMI20_TX_INPUTMUX_TYPE_T));
			if(ret){
				HDMI20_ERROR("Error during copying to user\n");
			}
		}
		else{
			ret = copy_from_user(&currentType, (LX_HDMI20_TX_INPUTMUX_TYPE_T *)type, sizeof(LX_HDMI20_TX_INPUTMUX_TYPE_T));
			if(ret){
				HDMI20_ERROR("Error during copying from user\n");
				break;
			}

			switch(currentType)
			{
				value &= (~HDMI20_TX_INPUT_MUX_MASK);

				case LX_HDMI20_TX_INPUT_INTERLACED_OIF:
					value |= HDMI20_TX_INPUT_OIF;
					ret = RET_OK;
					break;
				case LX_HDMI20_TX_INPUT_OSD:
					value |= HDMI20_TX_INPUT_OSD;
					ret = RET_OK;
					break;
				case LX_HDMI20_TX_INPUT_MAINVIDEO:
					value |= HDMI20_TX_INPUT_MAINVIDEO;
					ret = RET_OK;
					break;
				case LX_HDMI20_TX_INPUT_SUBVIDEO:
					value |= HDMI20_TX_INPUT_SUBVIDEO;
					ret = RET_OK;
					break;
				default :
					break;
			}

			if(ret == RET_OK){
				HDMI20_INFO("Switch HDMI20 TX input mux to [0x%08x].([3:0]=1:SV,2:MV,4:OSD,8:OIF)\n", value);
				HDMI20_HAL_H15Ax_Tx_TOP_Write_Port(HDMI20_TX_H15AX_AD_MAIN_MUX_CTRL, value);
			}
		}
	} while(0);

	return ret;
}

