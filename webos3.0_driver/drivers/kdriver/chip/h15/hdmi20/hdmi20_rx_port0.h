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

#ifndef	_HDMI20_RX_PORT0_H_
#define	_HDMI20_RX_PORT0_H_

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
/* IRQ handler for 5V detector */
irqreturn_t __HDMI20_HAL_RX0_5V_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs);

/* TOP ctrl read/write reg */
UINT32 __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(UINT32 addr);
void   __HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(UINT32 addr, UINT32 data);

/* HDMI2.0 driver function wrapper specific for port1 */
void __HDMI20_HAL_H15Ax_Log_Error_Port0(const char *file, int line, const char *fmt, ...);
void __HDMI20_HAL_H15Ax_Log_Info_Port0(const char *fmt, ...);
void __HDMI20_HAL_H15Ax_Device_Reset_Port0(bool enable);
void __HDMI20_HAL_H15Ax_Device_Reset_HDMI_Port0(bool enable);
int __HDMI20_HAL_H15Ax_Update_EDID_Port0(void);
UINT32 __HDMI20_HAL_H15Ax_BSP_Read_Port0(UINT16 addr);
void __HDMI20_HAL_H15Ax_BSP_Write_Port0(UINT16 addr, UINT32 data);
void __HDMI20_HAL_H15Ax_Rx_Phy_ZCAL_Reset_Port0(int enable);
UINT32 __HDMI20_HAL_H15Ax_Rx_Phy_Read_Port0(UINT16 reg_address);
void __HDMI20_HAL_H15Ax_Rx_Phy_Write_Port0(UINT16 reg_address, UINT32 data);
void __HDMI20_HAL_H15Ax_Rx_Phy_Svsretmode_Port0(int enable);
void __HDMI20_HAL_H15Ax_Rx_Phy_Reset_Port0(bool enable);
void __HDMI20_HAL_H15Ax_Rx_Phy_Pddq_Port0(int enable);
UINT16 __HDMI20_HAL_H15Ax_Rx_Phy_Zcal_Done_Port0(void);

void HDMI20_HAL_H15Ax_RX0_Enable_5V_IRQ(int bUp, int bOnOff);

void HDMI20_HAL_H15Ax_RX0_Reset_TOP_SyncInfo(void);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

