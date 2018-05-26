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
 *	VBI module control Drivers linux module
 *
 *  author      won.hur (won.hur@lge.com)
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_VBI
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
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/interrupt.h>

#include "vbi_kapi.h"

#include "vbi_module.h"
#include "vbi_drv.h"
#include "vbi_hw.h"
#include "vbi_data.h"

#include "os_util.h"

#ifdef INCLUDE_H15_CHIP_KDRV
#include "../../chip/h15/afe/de_cvd_reg_h15ax.h"	/**< H15 Register access */
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
#include "../../chip/m14/afe/de_cvd_reg_m14bx.h"	/**< M14 Register access */
#include "../../chip/m14/afe/de_cvd_reg_m14ax.h"	/**< M14 Register access */
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
#include "../../chip/h14/afe/de_cvd_reg_h14ax.h"	/**< H14 Register access */
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
#include "../../chip/h13/afe/de_cvd_reg_h13ax.h"	/**< H13 Register access */
#include "../../chip/h13/afe/de_cvd_reg_h13bx.h"	/**< H13 Register access */
#endif
#include "../be/video_status_keeper.h"
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
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int (*VBI_CheckVline625)(void);
int (*VBI_CheckAnalogCP)(void);

int (*VBI_TTX_HWInitialize)(void);
int (*VBI_TTX_SetInputFormat)(eVBIDataType type);
int (*VBI_TTX_SetInterrupt)(int on_off);
int (*VBI_TTX_ParsingVBIData)(sHeaderData *data);
int (*VBI_TTX_PutVBIData)(sHeaderData *data);
int (*VBI_TTX_SetInterruptClear)(void);
int (*VBI_TTX_Enable_VPS_Slicer)(BOOLEAN Enable);
int (*VBI_TTX_Set_TT_Slicer_Mode)(unsigned int mode);
int (*VBI_TTX_CheckOnLine318)(int mode);
int (*VBI_TTX_SetTypeLine318)(int onoff);
int (*VBI_TTX_SetVBILPF)(int onoff);
int (*VBI_TTX_DebugPrintMode)(int mode);

int (*VBI_CC_HWInitialize)(void);
int (*VBI_CC_SetInputFormat)(eVBIDataType type);
int (*VBI_CC_SetInterrupt)(int on_off);
int (*VBI_CC_ParsingVBIData)(unsigned int *packet);
int (*VBI_CC_SetInterruptClear)(void);
int (*VBI_CC_DebugPrintMode)(int mode);
#ifdef	KDRV_CONFIG_PM
int (*VBI_CC_RunSuspend)(void);
int (*VBI_CC_RunResume)(void);
int (*VBI_TTX_RunSuspend)(void);
int (*VBI_TTX_RunResume)(void);
#endif
/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
#ifdef INCLUDE_M14_CHIP_KDRV
extern volatile DE_CVD_REG_M14Bx_T __iomem *gpRegCVD_M14Bx;
extern volatile DE_CVD_REG_M14Bx_T __iomem *gpRealRegCVD_M14Bx;

extern volatile DE_CVD_REG_M14Ax_T __iomem *gpRegCVD_M14Ax;
extern volatile DE_CVD_REG_M14Ax_T __iomem *gpRealRegCVD_M14Ax;
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
extern volatile DE_CVD_REG_H14Ax_T __iomem *gpRegCVD_H14Ax;
extern volatile DE_CVD_REG_H14Ax_T __iomem *gpRealRegCVD_H14Ax;
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
extern volatile DE_CVD_REG_H13Bx_T __iomem *gpRegCVD_H13Bx;
extern volatile DE_CVD_REG_H13Bx_T __iomem *gpRealRegCVD_H13Bx;

extern volatile DE_CVD_REG_H13Ax_T __iomem *gpRegCVD_H13Ax;
extern volatile DE_CVD_REG_H13Ax_T __iomem *gpRealRegCVD_H13Ax;
#endif

extern LX_VBI_INIT_MODE_T gVBIInitiateMode;
extern LX_VBI_INIT_MODE_T gVBIEnabledMode;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
static BOOLEAN IS_VBI_INTERRUPT_REGISTERED	= false;

BOOLEAN ENABLE_ANALOGCC	= false;
BOOLEAN ENABLE_ANALOGTTX	= false;

static BOOLEAN gEnableLineFollowOnCCData = 1;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
void _VBI_TTX_InterruptRoutine(unsigned long unused);
DECLARE_TASKLET(VBI_TTX_ProcessData , _VBI_TTX_InterruptRoutine , 0);

void _VBI_CC_InterruptRoutine(unsigned long unused);
DECLARE_TASKLET(VBI_CC_ProcessData , _VBI_CC_InterruptRoutine , 0);

//static irqreturn_t VBI_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs);

static int __VBI_TTX_InitChipRev(void);
static int __VBI_CC_InitChipRev(void);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static int gEnablePrintResultofLineDetect = 0;


/*========================================================================================
	Implementation Group
========================================================================================*/

/**
* VBI TTX Module Chip revision converter
*
* @param void
* @return int
*/

static int __VBI_TTX_InitChipRev(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to H15 A0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_H15Ax;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_H15Ax;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_H15Ax;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_H15Ax;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_H15Ax;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_H15Ax;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_H15Ax;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_H15Ax;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_H15Ax;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_H15Ax;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_H15Ax;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_H15Ax;
		VBI_CheckVline625			= TTX_CheckVline625_H15Ax;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_H15Ax;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_H15Ax;
		VBI_TTX_RunResume	    	= TTX_RunResume_H15Ax;
#endif
    }
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to H14 A0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_H14Ax;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_H14Ax;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_H14Ax;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_H14Ax;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_H14Ax;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_H14Ax;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_H14Ax;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_H14Ax;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_H14Ax;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_H14Ax;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_H14Ax;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_H14Ax;
		VBI_CheckVline625			= TTX_CheckVline625_H14Ax;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_H14Ax;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_H14Ax;
		VBI_TTX_RunResume	    	= TTX_RunResume_H14Ax;
#endif

    }
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, B0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to M14 B0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_M14Bx;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_M14Bx;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_M14Bx;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_M14Bx;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_M14Bx;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_M14Bx;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_M14Bx;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_M14Bx;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_M14Bx;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_M14Bx;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_M14Bx;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_M14Bx;
		VBI_CheckVline625			= TTX_CheckVline625_M14Bx;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_M14Bx;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_M14Bx;
		VBI_TTX_RunResume	    	= TTX_RunResume_M14Bx;
#endif

    }
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to M14 A0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_M14Ax;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_M14Ax;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_M14Ax;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_M14Ax;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_M14Ax;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_M14Ax;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_M14Ax;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_M14Ax;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_M14Ax;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_M14Ax;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_M14Ax;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_M14Ax;
		VBI_CheckVline625			= TTX_CheckVline625_M14Ax;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_M14Ax;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_M14Ax;
		VBI_TTX_RunResume	    	= TTX_RunResume_M14Ax;
#endif

    }
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, B0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to H13 B0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_H13Bx;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_H13Bx;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_H13Bx;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_H13Bx;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_H13Bx;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_H13Bx;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_H13Bx;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_H13Bx;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_H13Bx;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_H13Bx;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_H13Bx;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_H13Bx;
		VBI_CheckVline625			= TTX_CheckVline625_H13Bx;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_H13Bx;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_H13Bx;
		VBI_TTX_RunResume	    	= TTX_RunResume_H13Bx;
#endif

    }
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
    {
		VBI_PRINT("VBI TTX chip revision is set to H13 A0\n");
		VBI_TTX_HWInitialize	    = TTX_HWInitialize_H13Ax;
		VBI_TTX_SetInputFormat	    = TTX_SetInputFormat_H13Ax;
		VBI_TTX_SetInterrupt	    = TTX_SetInterrupt_H13Ax;
		VBI_TTX_SetInterruptClear	= TTX_SetInterruptClear_H13Ax;
		VBI_TTX_ParsingVBIData	    = TTX_ParsingVBIData_H13Ax;
		VBI_TTX_PutVBIData		    = TTX_PutVBIData_H13Ax;
		VBI_TTX_SetVBILPF	        = TTX_SetVBILPF_H13Ax;
		VBI_TTX_CheckOnLine318	    = TTX_CheckOnLine318_H13Ax;
		VBI_TTX_SetTypeLine318	    = TTX_SetTypeLine318_H13Ax;
        VBI_TTX_Enable_VPS_Slicer   = TTX_Enable_VPS_Slicer_H13Ax;
        VBI_TTX_Set_TT_Slicer_Mode  = TTX_Set_TT_Slicer_Mode_H13Ax;
		VBI_TTX_DebugPrintMode		= TTX_DebugPrintMode_H13Ax;
		VBI_CheckVline625			= TTX_CheckVline625_H13Ax;
		VBI_CheckAnalogCP			= TTX_CheckAnalogCP_H13Ax;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend  		= TTX_RunSuspend_H13Ax;
		VBI_TTX_RunResume	    	= TTX_RunResume_H13Ax;
#endif

    }
#endif
	else
	{
		VBI_PRINT("VBI ERROR! Unknown chip revision at VBI module\n");
	    VBI_TTX_HWInitialize    	= VBI_Empty_Function_void;
		VBI_TTX_SetInputFormat  	= VBI_Empty_Function_int;
		VBI_TTX_SetInterrupt	    = VBI_Empty_Function_int;
		VBI_TTX_SetInterruptClear	= VBI_Empty_Function_void;
		VBI_TTX_ParsingVBIData	    = VBI_Empty_Function_int;
		VBI_TTX_PutVBIData	    	= VBI_Empty_Function_int;
		VBI_TTX_SetVBILPF       	= VBI_Empty_Function_int;
		VBI_TTX_CheckOnLine318  	= VBI_Empty_Function_int;
		VBI_TTX_SetTypeLine318  	= VBI_Empty_Function_int;
        VBI_TTX_Enable_VPS_Slicer   = VBI_Empty_Function_bool;
        VBI_TTX_Set_TT_Slicer_Mode  = VBI_Empty_Function_int;
		VBI_CheckVline625			= VBI_Empty_Function_void;
		VBI_CheckAnalogCP			= VBI_Empty_Function_void;
#ifdef	KDRV_CONFIG_PM
		VBI_TTX_RunSuspend	    	= VBI_Empty_Function_void;
		VBI_TTX_RunResume	    	= VBI_Empty_Function_void;
#endif
        /* Return error if chip revision is unknown */
        (void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS, __L__);
		ret = RET_ERROR;
	}

	return ret;
}



/**
* VBI TTX Module initialize
*
* @param void
* @return int
*/
int VBI_TTX_Initialize(void)
{
	int ret = RET_ERROR;
	__VBI_CC_InitChipRev();
    __VBI_TTX_InitChipRev();

	ret = VBI_TTX_HWInitialize();
	if(ret < 0) {
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS, __L__);
		return -1;
	};

	VBI_TTX_SetInputFormat(eDisable);
	VBI_TTX_InitDataMemory();

	VBI_TTX_SetInputFormat(CURRENT_TTX);
	VBI_TTX_InterruptControl(ON);

	ENABLE_ANALOGTTX	= true;

	ret = RET_OK;

#if 0
	if(!IS_VBI_INTERRUPT_REGISTERED){
		if(lx_chip_rev() >= LX_CHIP_REV( H14,A0)){
			VBI_PRINT("VBI TTX Interrupt registering to H14 Ax\n");
			ret = request_irq(H14_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0)){
			VBI_PRINT("VBI TTX Interrupt registering to M14 Bx\n");
			ret = request_irq(M14_B0_IRQ_DE_IDS_CVD, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0)){
			VBI_PRINT("VBI TTX Interrupt registering to M14 Ax\n");
			ret = request_irq(M14_A0_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0)){
			VBI_PRINT("VBI TTX Interrupt registering to H13... \n");
			ret = request_irq(H13_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}

		if(ret) VBI_ERROR("ERROR! VBI Interrupt(request_irq) has failed! VBI will not work properly!\n");
		else VBI_PRINT("VBI TTX Interrupt register succeed!\n");

		IS_VBI_INTERRUPT_REGISTERED = true;
	}
#endif

	gVBIInitiateMode = LX_VBI_INIT_PAL;

	return ret;
}


/**
* VBI interrupt control
*
* @parm void
* @return int
*/
int VBI_TTX_InterruptControl(int onoff)
{
	int ret = RET_ERROR;

    VBI_TTX_SetInterrupt(onoff);
    ret = RET_OK;

	return ret;
}


void _VBI_TTX_InterruptRoutine(unsigned long unused)
{
    //int ret = RET_OK;
	sHeaderData data;

	if(VBI_TTX_ParsingVBIData(&data) >= 0) VBI_TTX_PutVBIData(&data);
}



static int __VBI_CC_InitChipRev(void)
{
	int ret = RET_OK;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to H15 A0\n");
	    VBI_CC_HWInitialize		    = CC_H15Ax_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_H15Ax_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_H15Ax_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_H15Ax_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_H15Ax_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_H15Ax_DebugPrintMode;
		VBI_CheckVline625			= CC_H15Ax_CheckVline625;
		VBI_CheckAnalogCP			= CC_H15Ax_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_H15Ax_RunSuspend;
		VBI_CC_RunResume		    = CC_H15Ax_RunResume;
#endif

	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to H14 A0\n");
	    VBI_CC_HWInitialize		    = CC_H14Ax_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_H14Ax_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_H14Ax_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_H14Ax_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_H14Ax_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_H14Ax_DebugPrintMode;
		VBI_CheckVline625			= CC_H14Ax_CheckVline625;
		VBI_CheckAnalogCP			= CC_H14Ax_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_H14Ax_RunSuspend;
		VBI_CC_RunResume		    = CC_H14Ax_RunResume;
#endif

	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, B0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to M14 B0\n");
	    VBI_CC_HWInitialize		    = CC_M14Bx_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_M14Bx_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_M14Bx_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_M14Bx_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_M14Bx_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_M14Bx_DebugPrintMode;
		VBI_CheckVline625			= CC_M14Bx_CheckVline625;
		VBI_CheckAnalogCP			= CC_M14Bx_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_M14Bx_RunSuspend;
		VBI_CC_RunResume		    = CC_M14Bx_RunResume;
#endif
	}
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to M14 A0\n");
	    VBI_CC_HWInitialize		    = CC_M14Ax_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_M14Ax_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_M14Ax_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_M14Ax_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_M14Ax_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_M14Ax_DebugPrintMode;
		VBI_CheckVline625			= CC_M14Ax_CheckVline625;
		VBI_CheckAnalogCP			= CC_M14Ax_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_M14Ax_RunSuspend;
		VBI_CC_RunResume		    = CC_M14Ax_RunResume;
#endif
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
    else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, B0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to H13 B0\n");
	    VBI_CC_HWInitialize		    = CC_H13Bx_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_H13Bx_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_H13Bx_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_H13Bx_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_H13Bx_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_H13Bx_DebugPrintMode;
		VBI_CheckVline625			= CC_H13Bx_CheckVline625;
		VBI_CheckAnalogCP			= CC_H13Bx_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_H13Bx_RunSuspend;
		VBI_CC_RunResume		    = CC_H13Bx_RunResume;
#endif
	}
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
    {
		VBI_PRINT("VBI CC Chip revision is set to H13 A0\n");
	    VBI_CC_HWInitialize		    = CC_H13Ax_HWInitialize;
		VBI_CC_SetInputFormat	    = CC_H13Ax_SetInputFormat;
		VBI_CC_SetInterrupt		    = CC_H13Ax_SetInterrupt;
		VBI_CC_SetInterruptClear	= CC_H13Ax_SetInterruptClear;
		VBI_CC_ParsingVBIData   	= CC_H13Ax_ParsingVBIData;
		VBI_CC_DebugPrintMode		= CC_H13Ax_DebugPrintMode;
		VBI_CheckVline625			= CC_H13Ax_CheckVline625;
		VBI_CheckAnalogCP			= CC_H13Ax_CheckAnalogCP;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = CC_H13Ax_RunSuspend;
		VBI_CC_RunResume		    = CC_H13Ax_RunResume;
#endif
	}
#endif
	else
	{
		VBI_ERROR("VBI Error! Unknown chip revision at VBI module\n");
       	VBI_CC_HWInitialize		    = VBI_Empty_Function_void;
		VBI_CC_SetInputFormat	    = VBI_Empty_Function_int;
		VBI_CC_SetInterrupt		    = VBI_Empty_Function_int;
		VBI_CC_SetInterruptClear	= VBI_Empty_Function_void;
		VBI_CC_ParsingVBIData	    = VBI_Empty_Function_int;
		VBI_CC_DebugPrintMode		= VBI_Empty_Function_int;
		VBI_CheckVline625			= VBI_Empty_Function_void;
		VBI_CheckAnalogCP			= VBI_Empty_Function_void;
#ifdef	KDRV_CONFIG_PM
		VBI_CC_RunSuspend		    = VBI_Empty_Function_void;
		VBI_CC_RunResume	    	= VBI_Empty_Function_void;
#endif
	    /* Return error if chip revision is unknown */
        (void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS, __L__);
		ret = RET_ERROR;
    }
	return ret;
}



int VBI_CC_Initialize(void)
{
    int ret = RET_ERROR;

	__VBI_TTX_InitChipRev();
	__VBI_CC_InitChipRev();

	ret = VBI_CC_HWInitialize();
	if(ret < 0) {
		VBI_ERROR("VBI_CC_HWInitialize failed!\n");
		(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS, __L__);
		return -1;
	}

	VBI_CC_SetInputFormat(eDisable);
	VBI_CC_InitDataMemory();

	VBI_CC_SetInputFormat(eClosedCaptionUS);

	ENABLE_ANALOGCC	= true;

	ret = RET_OK;

#if 0
	if(!IS_VBI_INTERRUPT_REGISTERED){
		if(lx_chip_rev() >= LX_CHIP_REV( H14,A0)){
			VBI_PRINT("VBI CC Interrupt registering to H14 Ax\n");
			ret = request_irq(H14_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0)){
			VBI_PRINT("VBI CC Interrupt registering to M14 Bx\n");
			ret = request_irq(M14_B0_IRQ_DE_IDS_CVD, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0)){
			VBI_PRINT("VBI CC Interrupt registering to M14 Ax\n");
			ret = request_irq(M14_A0_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0)){
			VBI_PRINT("VBI CC Interrupt registering to H13... \n");
			ret = request_irq(H13_IRQ_CVD_BCPU, (irq_handler_t)VBI_IRQ_Handler, 0, "CVD", NULL);
		}

		if(ret) VBI_ERROR("ERROR! VBI Interrupt(request_irq) has failed! VBI will not work properly!\n");
		else VBI_PRINT("VBI CC Interrupt register succeed!\n");

		IS_VBI_INTERRUPT_REGISTERED = true;
	}
#endif

	gVBIInitiateMode = LX_VBI_INIT_NTSC;

    return ret;
}

/**
* VBI interrupt service routine
*
* @param void
* @return int
*/
int VBI_TTX_InterruptFunc(unsigned int status)
{
    int ret = RET_OK;

	tasklet_schedule(&VBI_TTX_ProcessData);
	VBI_TTX_SetInterruptClear();

	return ret;
}


int VBI_CC_InterruptControl(int onoff)
{
    int ret = RET_OK;

    VBI_CC_SetInterrupt(onoff);

    return ret;
}

void _VBI_CC_InterruptRoutine(unsigned long unused)
{
	int ret = RET_ERROR;

	static unsigned int data_array[6] = {0,};
	static unsigned int line_number_count[3] ={0,} ;
	static unsigned int line_number_sum[3] ={0,} ;
	static unsigned int raw_line_number_count[3] ={0,};
	static unsigned int raw_line_number_sum[3] ={0,};
	static unsigned int curr_cc_int_time, prev_cc_int_time = 0;
	static unsigned int max_line_number = 21;
	static unsigned int prev_max_line_number = 21;
	static unsigned int curr_max_line_number = 21;
	static unsigned int max_value = 0;
	static unsigned int raw_max_value = 0;

	int loop_count;
	int line_number;
	int line_data;
	int parity_valid_flag = 0;

    if ( gEnableLineFollowOnCCData > 0 )
	{
		ret = VBI_CC_ParsingVBIData(data_array);

		if(ret >= 1)
		{
			curr_cc_int_time = jiffies_to_msecs(jiffies);	// get current time

			for(loop_count = 0;loop_count <ret;loop_count ++)
			{
				line_number = (data_array[loop_count]&0xFFC00000) >> 22;
				line_data = (data_array[loop_count]&0x0000FFFF);

				parity_valid_flag = VBI_CC_CheckDataParity(line_data);

				//if(line_data == 0x8080) CC_PRINT("Data on line [%d] is [0x8080]\n", line_number);

				if((line_number == 18) || (line_number == 281))
				{
					raw_line_number_count[0]++;
					if((line_data != 0x8080)&&(parity_valid_flag == 1)) line_number_count[0] ++;
				}
				else if((line_number == 21) || (line_number ==284))
				{
					raw_line_number_count[1]++;
					if((line_data != 0x8080)&&(parity_valid_flag == 1))line_number_count[1] ++;
				}
				else if((line_number == 22) || (line_number ==285))
				{
					raw_line_number_count[2]++;
					if((line_data != 0x8080) &&(parity_valid_flag == 1))line_number_count[2] ++;
				}
				else
				{
		  		  	//CC_PRINT("ERROR BAD LINE NUMBER [%d], data_array[0x%x]\n", line_number, data_array[loop_count]);
				}

				// Select most frequent CC line number for CC slicing
				if( (curr_cc_int_time - prev_cc_int_time ) > 500)
				{
					memcpy(raw_line_number_sum, raw_line_number_count, sizeof(raw_line_number_count));
					memset(raw_line_number_count, 0, sizeof(raw_line_number_count));
					memcpy(line_number_sum, line_number_count, sizeof(line_number_count));
					memset(line_number_count, 0, sizeof(line_number_count));
					prev_cc_int_time = jiffies_to_msecs(jiffies);	// get current time
					max_value = 0;
					raw_max_value = 0;
		        	//CC_PRINT("LINE18[%d], LINE21[%d], LINE22[%d]\n", line_number_sum[0], line_number_sum[1], line_number_sum[2]);

					if(raw_line_number_sum[0] > ((raw_line_number_sum[1])*3) )
					{
						curr_max_line_number = 18;
						max_line_number = curr_max_line_number;
						max_value = line_number_sum[0];
						raw_max_value = raw_line_number_sum[0];
					}
					else if(raw_line_number_sum[0] >= ((raw_line_number_sum[1]>>2)+(raw_line_number_sum[1]>>3)) )
					{
						//CC_PRINT("Difficuilt line selection #1...\n");
						if(	line_number_sum[0] > line_number_sum[1])
						{

							if(raw_line_number_sum[0] >= VALID_LOWER_RANGE_OF_CC_LINE )
							{
								curr_max_line_number = 18;
								if(prev_max_line_number == curr_max_line_number) max_line_number = curr_max_line_number;
								max_value = line_number_sum[0];
								raw_max_value = raw_line_number_sum[0];
							}
							else if(raw_line_number_sum[1] >= VALID_LOWER_RANGE_OF_CC_LINE)
							{
								curr_max_line_number = 21;
								if(prev_max_line_number == curr_max_line_number) max_line_number = curr_max_line_number;
								max_value = line_number_sum[1];
								raw_max_value = raw_line_number_sum[1];
							}
							else
							{
								//CC_PRINT(" State 1 : use previous...\n");
								// ?? ???쿡?? ??~ ?Ǻ?; ?????ϳ?, ????° ???????? ?Ǻ?; '?Ͽ? ???? ????: d?صд?.
								if(raw_line_number_sum[0] >= raw_line_number_sum[1])
								{
									max_value = line_number_sum[0];
									raw_max_value = raw_line_number_sum[0];
								}
								else
								{
									max_value = line_number_sum[1];
									raw_max_value = raw_line_number_sum[1];
								}
							}
						}
						else if(line_number_sum[0] < line_number_sum[1])
						{

							if(raw_line_number_sum[1] >= VALID_LOWER_RANGE_OF_CC_LINE )
							{
								curr_max_line_number = 21;
								if(prev_max_line_number == curr_max_line_number) max_line_number = curr_max_line_number;
								max_value = line_number_sum[1];
								raw_max_value = raw_line_number_sum[1];
							}
							else
							{
								//CC_PRINT(" State 1 : use previous...\n");
								if(raw_line_number_sum[1] >= raw_line_number_sum[0])
								{
									max_value = line_number_sum[1];
									raw_max_value = raw_line_number_sum[1];
								}
								else
								{
									max_value = line_number_sum[0];
									raw_max_value = raw_line_number_sum[0];
								}
							}
						}
						else
						{
							//CC_PRINT(" State 1 : use previous...\n");
							if(raw_line_number_sum[0] >= raw_line_number_sum[1])
							{
								raw_max_value = raw_line_number_sum[0];
								max_value = line_number_sum[0];
							}
							else
							{
								raw_max_value = raw_line_number_sum[1];
								max_value = line_number_sum[1];
							}
						}
					}
					else
					{
						curr_max_line_number = 21;
						max_line_number = curr_max_line_number;
						max_value = line_number_sum[1];
						raw_max_value = raw_line_number_sum[1];
					}

					if(raw_line_number_sum[2] > (raw_max_value*3))
					{
						curr_max_line_number = 22;
						max_line_number = curr_max_line_number;
						max_value = line_number_sum[2];
						raw_max_value = raw_line_number_sum[2];
					}
					else if(raw_line_number_sum[2] >= ((raw_max_value>>2)+(raw_max_value>>3)) )
					{
						//CC_PRINT("Difficuilt line selection #2...\n");
						if(line_number_sum[2] > max_value)
						{
							if(raw_line_number_sum[2] >= VALID_LOWER_RANGE_OF_CC_LINE)
							{
								curr_max_line_number = 22;
								if(prev_max_line_number == curr_max_line_number) max_line_number = curr_max_line_number;
								max_value = line_number_sum[2];
								raw_max_value = raw_line_number_sum[2];
							}
							else if(raw_max_value >= VALID_LOWER_RANGE_OF_CC_LINE)
							{
								//CC_PRINT(" State 2 : use previous...\n");
								// do nothing...
							}
							else
							{
								// do nothing...
								//CC_PRINT(" State 2 : use previous...\n");
							}
						}
						else if(line_number_sum[2] < max_value)
						{
							// do nothing
							//CC_PRINT(" State 2 : use previous...\n");
						}
						else
						{
							// do nothing
							//CC_PRINT(" State 2 : use previous...\n");
						}
					}
					else
					{
						// do nothing
					}

					if(gEnablePrintResultofLineDetect>0)
					{
						CC_PRINT("Vote value : 18:[%d], 21:[%d], 22:[%d]\n", line_number_sum[0], line_number_sum[1], line_number_sum[2]);
						CC_PRINT("RAW Vote value : 18:[%d], 21:[%d], 22:[%d]\n", raw_line_number_sum[0], raw_line_number_sum[1], raw_line_number_sum[2]);
						CC_PRINT("Max Line Number = [%d], curr = [%d], prev = [%d]\n", max_line_number, curr_max_line_number, prev_max_line_number);
					}

					prev_max_line_number = curr_max_line_number;;
				}
				if( (max_line_number == 18) && ( (line_number == 18) || (line_number ==281)) )
				{
					if(raw_line_number_sum[0] > 6) VBI_CC_PutVBIData(&data_array[loop_count] , 1);
				}
				else if( (max_line_number == 21) && ( (line_number == 21) || (line_number ==284)) )
				{
					if(raw_line_number_sum[1] > 6) VBI_CC_PutVBIData(&data_array[loop_count] , 1);
				}
				else if ( (max_line_number == 22) && ( (line_number == 22) || (line_number ==285)) )
				{
					if(raw_line_number_sum[2] > 6) VBI_CC_PutVBIData(&data_array[loop_count] , 1);
				}
				else
				{
					//CC_PRINT("garbage CC packet???\n");
				}
			}
		}
	}
	else
    {
		ret = VBI_CC_ParsingVBIData(data_array);

		for(loop_count = 0;loop_count <ret;loop_count ++)
		{
			/* VBI_CC_PutVBIData ???? size ?? 1 ?? ?? ??!
			 * ??? buffer? ?? overflow ?? ??? ??!
			 * ?? ??? size? ?? 1? ??? ?! */
			VBI_CC_PutVBIData(&data_array[loop_count], 1);
    	}
	}
}

/**
* VBI interrupt service routine
*
* @param void
* @return int
*/

int VBI_CC_InterruptFunc(unsigned int status)
{
	tasklet_schedule(&VBI_CC_ProcessData);
	VBI_CC_SetInterruptClear();
	return 0;
}

int VBI_CC_SetPrintResultOfLineDetect(int onoff)
{
	if(onoff>0) gEnablePrintResultofLineDetect = 1;
	else gEnablePrintResultofLineDetect = 0;

	return 0;
}

#if 0
static irqreturn_t VBI_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs)
{
	UINT32 cvd_int_status = 0;
	UINT32 vbi_int_status = 0;

	/* Step 1 : Get interrupt status */
	if( lx_chip_rev() >= LX_CHIP_REV( H14, A0))
	{
		CVD_H14Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H14Ax_Rd(cvd_intr1_intr0);

		CVD_H14Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H14Ax_Rd(cvd_intr1_intr1);

		VBI_INTR_PRINT("VBI ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if((vbi_int_status & H14A0_VBI_INTERRUPT_MASK) )
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		CVD_H14Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H14Ax_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_H14Ax_WrFL(cvd_intr1_clear0);
		CVD_H14Ax_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		CVD_M14Bx_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_M14Bx_Rd(cvd_intr1_intr0);

		CVD_M14Bx_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_M14Bx_Rd(cvd_intr1_intr1);

		VBI_INTR_PRINT("VBI ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & M14B0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}

		}

		CVD_M14Bx_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_M14Bx_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_M14Bx_WrFL(cvd_intr1_clear0);
		CVD_M14Bx_WrFL(cvd_intr1_clear1);

	}
	else if( lx_chip_rev() >= LX_CHIP_REV( M14, A0))
	{
		CVD_M14Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_M14Ax_Rd(cvd_intr1_intr0);

		CVD_M14Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_M14Ax_Rd(cvd_intr1_intr1);

		VBI_INTR_PRINT("VBI ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & M14A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		CVD_M14Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_M14Ax_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_M14Ax_WrFL(cvd_intr1_clear0);
		CVD_M14Ax_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( H13, B0))
	{
		CVD_H13Bx_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H13Bx_Rd(cvd_intr1_intr0);

		CVD_H13Bx_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H13Bx_Rd(cvd_intr1_intr1);

		VBI_INTR_PRINT("VBI ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & H13A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}


		CVD_H13Bx_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H13Bx_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_H13Bx_WrFL(cvd_intr1_clear0);
		CVD_H13Bx_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( H13, A0))
	{
		CVD_H13Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H13Ax_Rd(cvd_intr1_intr0);

		CVD_H13Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H13Ax_Rd(cvd_intr1_intr1);

		VBI_INTR_PRINT("VBI ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & H13A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		CVD_H13Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H13Ax_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_H13Ax_WrFL(cvd_intr1_clear0);
		CVD_H13Ax_WrFL(cvd_intr1_clear1);

	}
	else{
		VBI_ERROR("WARNING! Unknown chip revision at VBI IRQ!\n");
		// Add statement for other chip version
	}

	return IRQ_HANDLED;
}
#endif

int VBI_TOP_UnInitialize(void)
{
	int ret = RET_OK;

#if 0
	if(IS_VBI_INTERRUPT_REGISTERED){
		if(lx_chip_rev() >= LX_CHIP_REV( H14,A0))
			free_irq(H14_IRQ_CVD_BCPU, NULL);
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0))
			free_irq(M14_B0_IRQ_DE_IDS_CVD, NULL);
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0))
			free_irq(M14_A0_IRQ_CVD_BCPU, NULL);
		else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0))
			free_irq(H13_IRQ_CVD_BCPU, NULL);

		IS_VBI_INTERRUPT_REGISTERED = false;
	}
#endif

	return ret;
}

int VBI_SetLogmPrint(unsigned long arg)
{
	int ret = RET_ERROR;
	LX_VBI_LOGM_SETTING_T stParams;

	do{
		/* Step 1. Copy data from application */
		ret = copy_from_user(&stParams, (LX_VBI_LOGM_SETTING_T *)arg, sizeof(LX_VBI_LOGM_SETTING_T));
		if (ret) break;

		/* Step 2. Check validty of logLevel, whether it is in the LOGM Level*/
		if((stParams.logLevel < 0)||(stParams.logLevel >= LX_LOGM_LEVEL_MAX)){
			VBI_ERROR("Unknown LOGM Level[%d]\n", stParams.logLevel);
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_VBI, VBI_SKEEPER_VBI_MODULE_ERROR_STATUS, __L__);
			ret = RET_ERROR;
		}

		/* Step 3. Operate Logm Setting */
		if(stParams.onOff){
			OS_DEBUG_EnableModuleByIndex  ( g_vbi_debug_fd, stParams.logLevel, DBG_COLOR_NONE );
			VBI_INFO("VBI LOGM Enabled for LogLevel[%d]\n", stParams.logLevel);
		}
		else{
			OS_DEBUG_DisableModuleByIndex ( g_vbi_debug_fd, stParams.logLevel);
			VBI_INFO("VBI LOGM disabled for LogLevel[%d]\n", stParams.logLevel);
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

int VBI_Module_Resume(LX_VBI_INIT_MODE_T mode, int isrOnOff)
{
	int ret = RET_ERROR;

	VBI_PRINT("[%s:%d] Called with mode[%d], isrOnOff[%d]\n", __F__, __L__, mode, isrOnOff);

	if(mode == LX_VBI_INIT_NONE){
		VBI_PRINT("[%s:%d] Nothing to resume because, it was not initiated before suspend\n", __F__, __L__);
		return RET_OK;
	}

	do{
		/* Resume Initial setting & Interrupt setting */
		if(mode == LX_VBI_INIT_NTSC){
			VBI_PRINT("[%s:%d] ReInitiating VBI to previous mode to [NTSC]\n", __F__, __L__);
			ret = VBI_CC_RunResume();

			if(ret<0) {
				VBI_PRINT("[%s:%d] Error! Could not Re-Init. Ret was [%d]\n", __F__, __L__, ret);
				break;
			}

			if(isrOnOff){
				VBI_PRINT("[%s:%d] Re-Activating VBI ISR to [NTSC]\n", __F__, __L__);
				ret =  VBI_CC_SetInterrupt(isrOnOff);
			}

			if(ret<0) {
				VBI_PRINT("[%s:%d] Error! Could not Re-Activate ISR. Ret was [%d]\n", __F__, __L__, ret);
				break;
			}
		}
		else if(mode == LX_VBI_INIT_PAL){
			VBI_PRINT("[%s:%d] ReInitiating VBI to previous mode to [PAL/SECAM]\n", __F__, __L__);
			ret = VBI_TTX_RunResume();

			if(ret<0) {
				VBI_PRINT("[%s:%d] Error! Could not Re-Init. Ret was [%d]\n", __F__, __L__, ret);
				break;
			}

			if(isrOnOff){
				VBI_PRINT("[%s:%d] Re-Activating VBI ISR to [PAL/SECAM]\n", __F__, __L__);
				ret =  VBI_TTX_SetInterrupt(isrOnOff);
			}

			if(ret<0) {
				VBI_PRINT("[%s:%d] Error! Could not Re-Activate ISR. Ret was [%d]\n", __F__, __L__, ret);
				break;
			}

		}
		else{
			VBI_PRINT("[%s:%d] Warning! Unknown previous mode type[%d]. Maybe snapshot memory curruption\n", __F__, __L__, mode);
			ret = RET_ERROR;
			break;
		}


	}while(0);

	if(ret<0) VBI_PRINT("[%s:%d] VBI Resume failed\n", __F__, __L__);
	else VBI_PRINT("[%s:%d] finished job OK!\n", __F__, __L__);

	return ret;
}

int VBI_Empty_Function_void(void)
{
	VBI_ERROR("Warning! VBI function called without Chip-initialization\n");
	return 0;
}

int VBI_Empty_Function_int(int empty)
{
	VBI_ERROR("Warning! VBI function called without Chip-initialization\n");
	return 0;
}

int VBI_Empty_Function_bool(BOOLEAN empty)
{
	VBI_ERROR("Warning! VBI function called without Chip-initialization\n");
	return 0;
}

/** @} */
