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

#include "hdmi20_hal_h15ax.h"
#include "hdmi_rx_ctrl.h"
#include "hdmi20_drv.h"
#include <linux/delay.h>

#include "hdmi20_rx_port1.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define HDMI20_LINK		HDMI20_DBG1

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern HDMI20_HAL_RX_DEVICE_DATA gHDMI20RxHandler[HDMI20_HAL_NUMBER_OF_LINK];

extern int synopsys_disable;
extern int gVideoDebugLevel;
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
irqreturn_t __HDMI20_HAL_RX1_5V_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs)
{
	int __5vLevel;
	UINT32 data;
	UINT32 rise_mask = HDMI20_RX_H15AX_AD_INTR_FLAG_5V_REDGE;
	UINT32 fall_mask = HDMI20_RX_H15AX_AD_INTR_FLAG_5V_FEDGE;

	/* Clear IRQ */
	data = (rise_mask | fall_mask);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_RX_H15AX_AD_INTR_CLR, data);

	HDMI20_HAL_Check5VLevelInfo(HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1, &__5vLevel);

	/* Disable IRQ */
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(1, 0);
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(0, 0);

	gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].locking_time = 0;

	if(__5vLevel) {
		HDMI20_LINK("HDMI2.0 Port[%d] 5V IRQ : UP \n", HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1);
		HDMI20_HAL_ConnectCable(HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1, 1);
	}
	else{

		HDMI20_LINK("HDMI2.0 Port[%d] 5V IRQ : DOWN\n", HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1);
		HDMI20_HAL_ConnectCable(HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1, 0);
	}


	/* Enable IRQ */
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(1, 1);
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(0, 1);

	return IRQ_HANDLED;
}

void HDMI20_HAL_H15Ax_RX1_Reset_TOP_SyncInfo(void)
{
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_RX_H15AX_AD_INFO_CLR, HDMI20_RX_H15AX_INFO_CLR_DATA);
}


void HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(int bUp, int bOnOff)
{
	UINT32 data;
	UINT32 rise_mask = HDMI20_RX_H15AX_AD_INTR_FLAG_5V_REDGE;
	UINT32 fall_mask = HDMI20_RX_H15AX_AD_INTR_FLAG_5V_FEDGE;
	UINT32 mask;
	
	if(bUp) mask = rise_mask;
	else mask = fall_mask; 

	data = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(HDMI20_RX_H15AX_AD_INTR_EN);

	if(bOnOff){
		data |= mask;
		//HDMI20_LINK("HDMI2.0 Port[%d] 5V IRQ Enabled for Mask[0x%08x] \n", HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1,data);
	}
	else{
		data &= (~mask);
		//HDMI20_LINK("HDMI2.0 Port[%d] 5V IRQ Disabled for Mask[0x%08x] \n", HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1,data);
	}

	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_RX_H15AX_AD_INTR_EN, data);
}

UINT32 __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(UINT32 addr)
{
	UINT32 ret;
	
	if(gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].topmem.pSwAddr != NULL){
		ret = (UINT32)(*((UINT32 *)((UINT32)gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].topmem.pSwAddr + (UINT32)addr)));
	}
	else{
		HDMI20_LINK("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
		ret = 0;
	}

	return ret;
}

void   __HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(UINT32 addr, UINT32 data)
{
	UINT32* dest;

	if(gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].topmem.pSwAddr != NULL){
		dest = 	((UINT32 *)((UINT32)gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].topmem.pSwAddr + (UINT32)addr));
		//HDMI20_LINK("Writing for Port[%d] : [0x%08x] <= [0x%08x] \n", HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1, (unsigned int)dest, (unsigned int)data);
		*dest = data;	
	}
	else{
		HDMI20_LINK("[%s:%d] Error! Device Driver access without initialization.\n",__F__ , __L__);
	}
}

void __HDMI20_HAL_H15Ax_Log_Error_Port1(const char *file, int line, const char *fmt, ...)
{
	va_list args;
    char buf[1024];

	va_start(args, fmt);
    vsprintf(buf, fmt, args);

    HDMI20_LINK(buf, args);

	va_end(args);
}

void __HDMI20_HAL_H15Ax_Log_Info_Port1(const char *fmt, ...)
{
	va_list args;
    char buf[1024];

	if(gVideoDebugLevel == 0) return;

	va_start(args, fmt);
    vsprintf(buf, fmt, args);

    HDMI20_LINK(buf, args);

	va_end(args);
}

void __HDMI20_HAL_H15Ax_Device_Reset_Port1(bool enable)
{
	HDMI20_LINK("[%s:%d] Function called [%d]\n", __F__, __L__, enable);
}

void __HDMI20_HAL_H15Ax_Device_Reset_HDMI_Port1(bool enable)
{
	UINT32 temp;
	
	HDMI20_LINK("[%s:%d] Function called [%d]\n", __F__, __L__, enable);

	/* Port 0 : SW RESET ADDR REG BASE => 0xFF0 UL */
	temp = __HDMI20_HAL_H15Ax_BSP_Read_Port1(0xFF0);
	temp &= ~(1 << 2);
	temp |= (enable & 1) << 2;
	
	__HDMI20_HAL_H15Ax_BSP_Write_Port1(0xFF0, temp);
}

int __HDMI20_HAL_H15Ax_Update_EDID_Port1(void)
{
	HDMI20_LINK("[%s:%d] Function called\n", __F__, __L__);

	/* Need to setup call back flag for KADAPTOR HDMI20 */
	
	return 0;
}

UINT32 __HDMI20_HAL_H15Ax_BSP_Read_Port1(UINT16 addr)
{
	UINT32 ret;
	
	if(gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].linkmem.pSwAddr != NULL){
		ret = (UINT32)(*((UINT32 *)((UINT32)gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].linkmem.pSwAddr + (UINT32)addr)));
	}
	else{
		HDMI20_LINK("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
		ret = 0;
	}

	return ret;
}

void __HDMI20_HAL_H15Ax_BSP_Write_Port1(UINT16 addr, UINT32 data)
{
	UINT32* dest;

	if(synopsys_disable >0){
		return;
	}

	if(gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].linkmem.pSwAddr != NULL){
		dest = 	((UINT32 *)((UINT32)gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].linkmem.pSwAddr + (UINT32)addr));
		*dest = data;	
	}
	else{
		HDMI20_LINK("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
	}
}

void __HDMI20_HAL_H15Ax_Rx_Phy_ZCAL_Reset_Port1(int enable)
{
	//HDMI20_LINK("[%s:%d] Function called [%d]\n", __F__, __L__, enable);
	/* Empty function :
	 * Was used when zero-calibration feature was used out-side-of-chip */
}

UINT32 __HDMI20_HAL_H15Ax_Rx_Phy_Read_Port1(UINT16 reg_address)
{
	return hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].ctrl, HDMI20_RX1_PHY_GEN3_I2C_SLAVE_ADDR, reg_address);
}

void __HDMI20_HAL_H15Ax_Rx_Phy_Write_Port1(UINT16 reg_address, UINT32 data)
{
	hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].ctrl, HDMI20_RX1_PHY_GEN3_I2C_SLAVE_ADDR, reg_address, data);
}

void __HDMI20_HAL_H15Ax_Rx_Phy_Svsretmode_Port1(int enable)
{
	hdmi_rx_ctrl_svsmode(&gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].ctrl, enable);
}

void __HDMI20_HAL_H15Ax_Rx_Phy_Reset_Port1(bool enable)
{
	hdmi_rx_ctrl_phy_reset(&gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].ctrl, enable);
}

void __HDMI20_HAL_H15Ax_Rx_Phy_Pddq_Port1(int enable)
{
	hdmi_rx_ctrl_pddq(&gHDMI20RxHandler[HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1].ctrl, enable);
}

UINT16 __HDMI20_HAL_H15Ax_Rx_Phy_Zcal_Done_Port1(void)
{
	//HDMI20_LINK("[%s:%d] Function called \n", __F__, __L__);
	/* Empty function :
	 * Was used when zero-calibration feature was used out-side-of-chip */
	return 0;
}
