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
 *  @author     won.hur
 *  @version    1.0
 *  @date       2014-05-19
 *  @note       Additional information.
 */

#ifndef	_HDMI20_TX_PORT0_H_
#define	_HDMI20_TX_PORT0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
/* TOP ctrl read/write reg */
UINT32 HDMI20_HAL_H15Ax_Tx_TOP_Read_Port(UINT32 addr);
void   HDMI20_HAL_H15Ax_Tx_TOP_Write_Port(UINT32 addr, UINT32 data);

int	HDMI20_HAL_H15Ax_Tx_InputMux(int isRead, LX_HDMI20_TX_INPUTMUX_TYPE_T *type);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

