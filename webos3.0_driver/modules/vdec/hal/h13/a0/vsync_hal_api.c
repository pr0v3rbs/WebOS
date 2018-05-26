/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     youngki.lyu@lge.com
 * version    0.1
 * date       2010.03.11
 * note       Additional information.
 *
 * @addtogroup h13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define LOG_NAME vdec_hal_vsync

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include "asm/div64.h"

#include "vdec_type_defs.h"

#include "hal/vdec_base.h"
#include "hal/vsync_reg.h"
#include "hal/vsync_hal_api.h"

#if defined (CHIP_NAME_d13)
#include "hal/d13/vd3_top_hal.h"
#elif defined (CHIP_NAME_d14)
#include "hal/d14/vd3_top_hal.h"
#else
#include "hal/top_hal_api.h"
#endif

#include "log.h"

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
#ifndef CHIP_NAME_d14
static volatile Sync_REG_T			*stpSync_Reg = NULL;
static Sync_REG_T *reg_backup;
static SINT32 i32PhaseShift[VSYNC_HW_NUM_OF_CH] = {0, };
static BOOLEAN bCurInterlaced[VSYNC_HW_NUM_OF_CH];
#endif
static int		gVsyncShiftOff = 0;

module_param_named (vsync_shift_off, gVsyncShiftOff, int, 0644);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_Init(void)
{
#ifndef CHIP_NAME_d14
	UINT32		chip, rev;
	UINT32		u32RegBasePhy;

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

	VDEC_GET_VSYNC_BASE(rev, u32RegBasePhy);

	stpSync_Reg		= (volatile Sync_REG_T *)ioremap(u32RegBasePhy, 0x20);
	VSync_HAL_Reset();
#endif
}

void VSync_HAL_Suspend(void)
{
#ifndef CHIP_NAME_d14
	log_noti ("suspend...\n");

	reg_backup = kmalloc (sizeof(*reg_backup), GFP_ATOMIC);
	if (!reg_backup)
	{
		log_error ("no memory for register backup\n");
		return;
	}

	memcpy ((void*)reg_backup, (void*)stpSync_Reg,
			sizeof (*stpSync_Reg));

	log_noti ("suspend... done.\n");
#endif
}

void VSync_HAL_Resume(void)
{
#ifndef CHIP_NAME_d14
	log_noti ("resume...\n");

	VSync_HAL_Reset();

	if (reg_backup)
	{
		memcpy ((void*)stpSync_Reg, (void*)reg_backup,
				sizeof (*stpSync_Reg));

		kfree (reg_backup);
	}
	else
		log_error ("no reg_backup\n");

	log_noti ("resume... done.\n");
#endif
}

static int vsync_irq_gap_print = 700;
module_param (vsync_irq_gap_print, int, 0644);

static void (*_fpHandler)(UINT8 ch);

DEFINE_SPINLOCK (VSync_HAL_IRQLock);

static irqreturn_t _IRQHandler (int num, void *arg)
{
	unsigned int irqstatus = 0;
	unsigned int int_clear = 0;

	irqstatus = TOP_HAL_GetVsyncIntrStatus();

	if (!irqstatus)
		return IRQ_NONE;

	log_trace ("status: %08x\n", irqstatus);

	spin_lock (&VSync_HAL_IRQLock);
	if (irqstatus & (1<<VSYNC0))
	{
		static unsigned long long pre;
		static int gap_pre;

		unsigned long long now;
		int gap_now;

		now = sched_clock ();

		if (_fpHandler)
			_fpHandler (0);

		gap_now = ((int)(now-pre))/1000;

		if (gap_now-gap_pre > vsync_irq_gap_print ||
				gap_pre-gap_now > vsync_irq_gap_print)
			log_noti ("cpu:%d interval %dus, pre %dus\n",
					smp_processor_id(),gap_now, gap_pre);
		else
			log_debug ("interval %dus, pre %dus\n",
					gap_now, gap_pre);

		gap_pre = gap_now;
		pre = now;

		int_clear |= 1<<VSYNC0;
	}
	if (irqstatus & (1<<VSYNC1))
	{
		if (_fpHandler)
			_fpHandler (1);

		int_clear |= 1<<VSYNC1;
	}
#if !defined (CHIP_NAME_d14)
	if (irqstatus & (1<<VSYNC2))
	{
		if (_fpHandler)
			_fpHandler (2);

		int_clear |= 1<<VSYNC2;
	}
	if (irqstatus & (1<<VSYNC3))
	{
		if (_fpHandler)
			_fpHandler (3);

		int_clear |= 1<<VSYNC3;
	}
#endif
	spin_unlock (&VSync_HAL_IRQLock);

	TOP_HAL_ClearVsyncIntrMsk(int_clear);

	log_trace ("done\n");

	if (int_clear)
		return IRQ_HANDLED;
	else
		return IRQ_NONE;
}

int VSync_HAL_IRQ_Init (void (*fpHandler)(UINT8 ch))
{
	int ret;
	void *dev = (void*)(('v'<<0)|('s'<<8)|('y'<<16)|('n'<<24));

	_fpHandler = fpHandler;

	ret = request_irq (VDEC_IRQ_NUM_SYNC,
			_IRQHandler, IRQF_SHARED, "vsync", dev);
	if (ret < 0)
	{
		log_error ("request_irq failed\n");
		return ret;
	}

	return 0;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32 VSync_HAL_GetVersion(void)
{
#ifndef CHIP_NAME_d14
	return stpSync_Reg->version;
#else
	return 0;
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_Reset(void)
{
#ifndef CHIP_NAME_d14
	VDISP_CONF		sync_conf;
	UINT32			i;

	for(i=0; i<VSYNC_HW_NUM_OF_CH; i++)
	{
		i32PhaseShift[i] = 0;
		bCurInterlaced[i] = TRUE;
	}

	sync_conf = stpSync_Reg->sync_conf;

	sync_conf.reg_int_vsync0_field_sel = 1;
	sync_conf.reg_int_vsync1_field_sel = 1;
	sync_conf.reg_int_vsync2_field_sel = 1;
	sync_conf.reg_int_vsync3_field_sel = 1;
	sync_conf.reg_vsync0_first_pul_en = 1;
	sync_conf.reg_vsync1_first_pul_en = 1;
	sync_conf.reg_vsync2_first_pul_en = 1;
	sync_conf.reg_vsync3_first_pul_en = 1;
	sync_conf.reg_vsync0_srst = 1;
	sync_conf.reg_vsync1_srst = 1;
	sync_conf.reg_vsync2_srst = 1;
	sync_conf.reg_vsync3_srst = 1;
	sync_conf.reg_vsync0_en = 1;
	sync_conf.reg_vsync1_en = 1;
	sync_conf.reg_vsync2_en = 1;
	sync_conf.reg_vsync3_en = 1;
	sync_conf.reg_ext_vsync0_field_sel = 1;
	sync_conf.reg_ext_vsync1_field_sel = 1;
	sync_conf.reg_ext_vsync2_field_sel = 1;
	sync_conf.reg_ext_vsync3_field_sel = 1;

	stpSync_Reg->sync_conf = sync_conf;
#endif
}

void VSync_HAL_SelVsync(UINT32 vsync_ch, UINT32 out_port)
{
	return;
}

void VSync_HAL_EnableVsync(UINT32 u32VsyncCh)
{
#ifndef CHIP_NAME_d14
	UINT32		sync_conf;

	sync_conf = *(UINT32*)&(stpSync_Reg->sync_conf);

	sync_conf = sync_conf | (1<<(28+u32VsyncCh));

	stpSync_Reg->sync_conf = *(VDISP_CONF*)&sync_conf;
#endif
}

void VSync_HAL_DisableVsync(UINT32 u32VsyncCh)
{
#ifndef CHIP_NAME_d14
	UINT32		sync_conf;

	sync_conf = *(UINT32*)&(stpSync_Reg->sync_conf);

	sync_conf = sync_conf & (~(1 << (28+u32VsyncCh)));

	stpSync_Reg->sync_conf = *(VDISP_CONF*)&sync_conf;
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
/*
BOOLEAN VSync_HAL_SetVsyncFrameInterval(UINT8 ui8SyncCh, UINT32 ui32Interval, BOOLEAN bInterlaced)
{
	if( ui8SyncCh >= VSYNC_HW_NUM_OF_CH )
	{
		log_error ("[VSync%d][Err] Channel Number, %s", ui8SyncCh, __FUNCTION__ );
		return FALSE;
	}

	if( (ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0) )
	{
		log_error ("[VSync%d][ERR] Frame Rate Residual(%d)/Divider(%d)", ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv );
		return FALSE;
	}

}
*/

void VSync_HAL_SetVsyncShift(UINT8 ui8SyncCh, SINT32 i32Delta)
{
#ifndef CHIP_NAME_d14
	if (gVsyncShiftOff)
	{
		i32PhaseShift[ui8SyncCh] = 0;
		return;
	}

	log_noti("ch %d phase shift %d\n", ui8SyncCh, i32Delta);
	i32PhaseShift[ui8SyncCh] = i32Delta;
#endif
}

BOOLEAN VSync_HAL_SetVsyncField(UINT8 ui8SyncCh, UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced)
{
#ifndef CHIP_NAME_d14
	UINT32	ui32FieldNum;
	UINT32	ui32FieldNum2;

	VDISP_UPDATE		sync_update;
	UINT64  u64Temp;

	if( ui8SyncCh >= VSYNC_HW_NUM_OF_CH )
	{
		log_error ("[VSync%d][Err] Channel Number, %s", ui8SyncCh, __FUNCTION__ );
		return FALSE;
	}

	if( (ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0) )
	{
		log_error ("[VSync%d][ERR] Frame Rate Residual(%d)/Divider(%d)",
				ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv );
		return FALSE;
	}

	u64Temp = (UINT64)27000000 * (UINT64)ui32FrameRateDiv;
	do_div(u64Temp, ui32FrameRateRes);
	ui32FieldNum = (UINT32)u64Temp;
	ui32FieldNum += i32PhaseShift[ui8SyncCh];
	ui32FieldNum2 = ui32FieldNum;
	ui32FieldNum >>= 1;
	ui32FieldNum2 -= ui32FieldNum;

	if( bInterlaced != bCurInterlaced[ui8SyncCh])
	{
		if( bInterlaced )
		{
			(*(UINT32*)&stpSync_Reg->sync_conf)	|= (1<<ui8SyncCh);
			(*(UINT32*)&stpSync_Reg->sync_conf)	|= (1<<(12+ui8SyncCh));
		}
		else
		{
			(*(UINT32*)&stpSync_Reg->sync_conf)	&= ~(1<<ui8SyncCh);
			(*(UINT32*)&stpSync_Reg->sync_conf)	&= ~(1<<(12+ui8SyncCh));
		}
		log_noti("[VSync%d] ScanType %d->%d  %X\n", ui8SyncCh, bCurInterlaced[ui8SyncCh],
				bInterlaced, (*(UINT32*)&stpSync_Reg->sync_conf) );
		bCurInterlaced[ui8SyncCh] = bInterlaced;
	}

	log_debug ("[VSync%d][DBG] Vsync Rate Res(%d)/Div(%d) Intlc %d, FieldNum:%d Phase %d",
			ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv, bInterlaced, ui32FieldNum,
			i32PhaseShift[ui8SyncCh]);

	stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field0_num = ui32FieldNum;
	stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field1_num = ui32FieldNum2;

	sync_update = stpSync_Reg->sync_update;
	switch( ui8SyncCh )
	{
		case 0:
			sync_update.reg_vsync0_update = 1;
			break;
		case 1:
			sync_update.reg_vsync1_update = 1;
			break;
		case 2:
			sync_update.reg_vsync2_update = 1;
			break;
		case 3:
			sync_update.reg_vsync3_update = 1;
			break;
		default:
			log_error ("[VSYNC%d][ERR] Frame Rate", ui8SyncCh );
			break;
	}
	stpSync_Reg->sync_update = sync_update;

	return TRUE;
#else
	return TRUE;
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_RestartAllVsync(void)
{
#ifndef CHIP_NAME_d14
	UINT8		ui8SyncCh;
	UINT32		ui32FieldNum[VSYNC_HW_NUM_OF_CH][2];

	log_debug ("[SYNC][DBG] %s", __FUNCTION__ );

	// back up the register values
	for( ui8SyncCh = 0; ui8SyncCh < VSYNC_HW_NUM_OF_CH; ui8SyncCh++ )
	{
		ui32FieldNum[ui8SyncCh][0] = stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field0_num;
		ui32FieldNum[ui8SyncCh][1] = stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field1_num;

		stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field0_num = 0x0006DF92;
		stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field1_num = 0x0006DF92;
	}

	VSync_HAL_Reset();

	// restore register values
	for( ui8SyncCh = 0; ui8SyncCh < VSYNC_HW_NUM_OF_CH; ui8SyncCh++ )
	{
		stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field0_num = ui32FieldNum[ui8SyncCh][0];
		stpSync_Reg->vsync_field_num[ui8SyncCh].reg_field1_num = ui32FieldNum[ui8SyncCh][1];

		switch( ui8SyncCh )
		{
		case 0:
			stpSync_Reg->sync_update.reg_vsync0_update = 1;
			break;
		case 1:
			stpSync_Reg->sync_update.reg_vsync1_update = 1;
			break;
		case 2:
			stpSync_Reg->sync_update.reg_vsync2_update = 1;
			break;
		case 3:
			stpSync_Reg->sync_update.reg_vsync3_update = 1;
			break;
		}

	}
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32  VSync_HAL_IntField(UINT8 ui8SyncCh)
{
#ifndef CHIP_NAME_d14
	return stpSync_Reg->vsync_int_field[ui8SyncCh].reg_int_vsync_field & 0x1;
#else
	return 0;
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32  VSync_HAL_ExtField(UINT8 ui8SyncCh)
{
#ifndef CHIP_NAME_d14
	return stpSync_Reg->vsync_ext_field[ui8SyncCh].reg_ext_vsync_field & 0x1;
#else
	return 0;
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_SetUpdateMode(UINT8 ui8TimerCh, UINT8 ui8Mode)
{
#ifndef CHIP_NAME_d14
	switch( ui8TimerCh )
	{
	case 0:
		stpSync_Reg->timer_conf.reg_timer0_update_mode = ui8Mode; break;
	case 1:
		stpSync_Reg->timer_conf.reg_timer1_update_mode = ui8Mode; break;
	case 2:
		stpSync_Reg->timer_conf.reg_timer2_update_mode = ui8Mode; break;
	case 3:
		stpSync_Reg->timer_conf.reg_timer3_update_mode = ui8Mode; break;
	default:
		break;
	}
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_ResetTimer(UINT8 ui8TimerCh)
{
#ifndef CHIP_NAME_d14
	switch( ui8TimerCh )
	{
	case 0:
		stpSync_Reg->timer_conf.reg_timer0_srst = 1; break;
	case 1:
		stpSync_Reg->timer_conf.reg_timer1_srst = 1; break;
	case 2:
		stpSync_Reg->timer_conf.reg_timer2_srst = 1; break;
	case 3:
		stpSync_Reg->timer_conf.reg_timer3_srst = 1; break;
	default:
		break;
	}
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_EnableTimer(UINT8 ui8TimerCh)
{
#ifndef CHIP_NAME_d14
	switch( ui8TimerCh )
	{
	case 0:
		stpSync_Reg->timer_conf.reg_timer0_en = 1; break;
	case 1:
		stpSync_Reg->timer_conf.reg_timer1_en = 1; break;
	case 2:
		stpSync_Reg->timer_conf.reg_timer2_en = 1; break;
	case 3:
		stpSync_Reg->timer_conf.reg_timer3_en = 1; break;
	default:
		break;
	}
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_DisableTimer(UINT8 ui8TimerCh)
{
#ifndef CHIP_NAME_d14
	switch( ui8TimerCh )
	{
	case 0:
		stpSync_Reg->timer_conf.reg_timer0_en = 0; break;
	case 1:
		stpSync_Reg->timer_conf.reg_timer1_en = 0; break;
	case 2:
		stpSync_Reg->timer_conf.reg_timer2_en = 0; break;
	case 3:
		stpSync_Reg->timer_conf.reg_timer3_en = 0; break;
	default:
		break;
	}
#endif
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void VSync_HAL_SetTimer(UINT8 ui8TimerCh, UINT32 ui32Value)
{
#ifndef CHIP_NAME_d14
	switch( ui8TimerCh )
	{
	case 0:
		stpSync_Reg->timer0_num.reg_timer0_num = ui32Value; break;
	case 1:
		stpSync_Reg->timer1_num.reg_timer1_num = ui32Value; break;
	case 2:
		stpSync_Reg->timer2_num.reg_timer2_num = ui32Value; break;
	case 3:
		stpSync_Reg->timer3_num.reg_timer3_num = ui32Value; break;
	default:
		break;
	}
#endif
}

/** @} */

