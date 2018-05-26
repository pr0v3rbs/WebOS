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
 * version    1.0
 * date       2011.11.08
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */


/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define LOG_NAME	vdec_vsync

/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include "vsync_drv.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
//#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#include "vdisp_cfg.h"
#include "../hal/vsync_hal_api.h"
#include "../hal/top_hal_api.h"

#if defined(CHIP_NAME_d13)
#include "../hal/d13/mcu_base.h"
#endif
#if defined(CHIP_NAME_d14)
#include "../hal/d14/mcu_base.h"
#endif

#include "log.h"
#include <asm/div64.h>

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/
#define		VSYNC_MAX_CONNECT			3
#define		VSYNC_INVAL_SRC				0xFF
#define		VSYNC_INVAL_CH				0xFF
#define VDISP_INVALID_CHANNEL	0xff

#define		VSYNC_NUM_OF_WORK_Q				0x80
//#define  VSYNC_ISR_PERFORMANCE_EVAL
#ifdef ARM_NOT_USING_MMCU
#define		VVSYNC_TIMING			1
#else
#if defined(CHIP_NAME_d13) || defined(CHIP_NAME_d14)
#define		VVSYNC_TIMING			(0)
#else
#define		VVSYNC_TIMING			(-5)
#endif
#endif
/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/
//#define MS_TO_NS(x) (x * 1E6L)
#define MS_TO_NS(x) (x * 1000000L)
//restrict vsync change delta value and vsync shift range
#if defined (CHIP_NAME_h13) || (CHIP_NAME_h15)
#define VSYNC_SHIFT_LIMIT		2		// kppm
#define VSYNC_SHIFT_LIMIT2		1		// kppm, for none standard rate
#define VSYNC_SHIFT_DELTA		5		// 27 MHz ticks
#else
#define VSYNC_SHIFT_LIMIT		5		// kppm
#define VSYNC_SHIFT_LIMIT2		2		// kppm, for none standard rate
#define VSYNC_SHIFT_DELTA		300		// 27 MHz ticks
#endif
#define VSYNC_SHIFT_DELTA_LIMIT  300

#define VSYNC_SRC_CH 0xFF
#define VSYNC_DUAL_3D_SYNC_SET 0
static int vdec_vsync_shift_delta_limit = VSYNC_SHIFT_DELTA_LIMIT;
module_param (vdec_vsync_shift_delta_limit, int, 0644);

static int vdec_vsync_src_ch = VSYNC_SRC_CH;
module_param (vdec_vsync_src_ch, int, 0644);

static int vdec_vsync_shift_limit = VSYNC_SHIFT_LIMIT;
module_param (vdec_vsync_shift_limit, int, 0644);

static int vdec_vsync_shift_limit2 = VSYNC_SHIFT_LIMIT2;
module_param (vdec_vsync_shift_limit2, int, 0644);

static int vdec_vsync_shift_delta = VSYNC_SHIFT_DELTA;
module_param (vdec_vsync_shift_delta, int, 0644);

static int vdec_3d_dual_sync_setting = VSYNC_DUAL_3D_SYNC_SET;
module_param (vdec_3d_dual_sync_setting, int, 0644);
/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef struct
{
	BOOLEAN			bUse;
	UINT8 			vsync_ch;
} VSYNC_LINK_T;

typedef enum
{
	VSYNC_HW_INACTIVE,
	VSYNC_HW_ACTIVE
} VSYNC_HW_ST;

typedef struct
{
	VSYNC_HW_ST		status;
	UINT8			vsync_hw_ch;
	UINT8			main_vsync;
	UINT8			n_connected;
	VSYNC_LINK_T	links[VSYNC_MAX_CONNECT];
} VSYNC_HW;

typedef struct
{
	SINT32		i32PhaseShift;
	SINT32		i32NextPhaseShift;
	UINT32		u32NextResidual;
	UINT32		u32NextDivider;
	BOOLEAN		bNextInterlaced;

	BOOLEAN		bTimerSet;
	SINT32		max_shift;

	UINT32		dco_intv27M;
	UINT32		cur_intv27M;
	UINT32		min_intv27M;
	UINT32		max_intv27M;
	UINT32		base_intv27M;

	UINT8		vsync_src_ch;

	struct hrtimer	hr_timer;

	struct
	{
		UINT32		ui32FrameResidual;
		UINT32		ui32FrameDivider;
		UINT32		ui32FrameDuration90K;
		BOOLEAN		bInterlaced;
	} Rate;

	struct
	{
		UINT8		u8VsyncCh;
		VSYNC_LINK_T astSyncChTag[VSYNC_MAX_CONNECT];
	} Config;

	struct
	{
		volatile UINT32		ui32ActiveDeIfChMask;
	} Status;

	struct
	{
		UINT32		ui32LogTick_ISR;
		UINT32		ui32LogTick_Dual;
		UINT32		ui32GSTC_Prev;
		UINT32		ui32DurationTick;
		UINT32		ui32DualTickDiff;
	} Debug;

} VSYNC_CH_T;



/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/
extern void VDISP_VVsyncCallback(UINT8 u8VsyncCh, UINT8 u8nVdisp,
		UINT8 au8VdispCh[], UINT8 u8FieldInfo);

/*------------------------------------------------------------------------------
 *   External Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Functions
 *----------------------------------------------------------------------------*/
void _vsync_set_rate_info(VSYNC_CH_T *pstVsync, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced);

/*------------------------------------------------------------------------------
 *   global Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *----------------------------------------------------------------------------*/
static void VDEC_ISR_VVSync(UINT8 ui8VSyncCh);
static void _ISR_VSync(UINT8 ui8VSyncCh);

/*------------------------------------------------------------------------------
 *   Static Variables
 *----------------------------------------------------------------------------*/
static VSYNC_CH_T	gsVSync[VSYNC_NUM_OF_CH];
static VSYNC_HW		vsync_hws[VSYNC_NUMOF_SRC];
static BOOLEAN bPrePhaseShift[VSYNC_NUM_OF_CH];
//static VSYNC_CB fnVsyncCallback = NULL;
static VSYNC_CB fnFeedCallback = NULL;
#ifdef  VSYNC_ISR_PERFORMANCE_EVAL
static struct timeval tv_start;
#endif
static int phaseshift_disable = 0;
module_param_named(vsync_disable_phaseshift, phaseshift_disable, int, 0644);

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

void _vsync_reset(VSYNC_CH_T	*pstVsync)
{
	pstVsync->i32PhaseShift = 0;
	pstVsync->i32NextPhaseShift = 0;
	pstVsync->dco_intv27M = 0;

	pstVsync->Debug.ui32LogTick_ISR = 0;
	pstVsync->Debug.ui32LogTick_Dual = 0;
	pstVsync->Debug.ui32GSTC_Prev = 0xFFFFFFFF;
	pstVsync->Debug.ui32DurationTick = 0;
	pstVsync->Debug.ui32DualTickDiff = 0;

	pstVsync->bTimerSet = FALSE;
	pstVsync->max_shift = 10800;
}
static BOOLEAN bTimerSet=FALSE;
static struct hrtimer hr_timer;


void VSync_Init(VSYNC_CB fnCallback)
{
	UINT8	ui8UseCh, i;

	log_noti("[VSYNC] Vsync Init: 0x%X, %s\n", (UINT32)gsVSync, __FUNCTION__);

	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

	fnFeedCallback = fnCallback;

	for (i=0; i<VSYNC_NUMOF_SRC; i++)
	{
		VSYNC_HW	*vsync_hw;

		vsync_hw = &vsync_hws[i];
		vsync_hw->status = VSYNC_HW_INACTIVE;
		vsync_hw->vsync_hw_ch = i;
		vsync_hw->n_connected = 0;

		for (ui8UseCh = 0; ui8UseCh < VSYNC_MAX_CONNECT; ui8UseCh++)
		{
			vsync_hw->links[ui8UseCh].bUse = FALSE;
			vsync_hw->links[ui8UseCh].vsync_ch = VSYNC_INVAL_CH;
		}
	}

	for (i = 0; i < VSYNC_NUM_OF_CH; i++)
	{
		gsVSync[i].Rate.ui32FrameResidual = 0x0;
		gsVSync[i].Rate.ui32FrameDivider = 0x1;
		gsVSync[i].Rate.ui32FrameDuration90K = 0x0;
		gsVSync[i].Rate.bInterlaced = TRUE;

		gsVSync[i].dco_intv27M = 0;
		gsVSync[i].cur_intv27M = 0;
		gsVSync[i].base_intv27M = 0;
		gsVSync[i].min_intv27M = 0;
		gsVSync[i].max_intv27M = 0xFFFFFFFF;

		gsVSync[i].vsync_src_ch = VSYNC_INVAL_SRC;

		gsVSync[i].Config.u8VsyncCh = i;
		for (ui8UseCh = 0; ui8UseCh < VSYNC_MAX_CONNECT; ui8UseCh++)
		{
			gsVSync[i].Config.astSyncChTag[ui8UseCh].bUse = FALSE;
			gsVSync[i].Config.astSyncChTag[ui8UseCh].vsync_ch = VSYNC_INVAL_CH;
		}

		gsVSync[i].Status.ui32ActiveDeIfChMask = 0x0;
		bPrePhaseShift[i] = FALSE;
	}

#if defined(CHIP_NAME_d13)
	{
		UINT32		intr_en;
		intr_en = GetReg(AD_M1_INTR_ENABLE);
		intr_en |= VDO_HW_INT_EN; //VDO interrupt
		SetReg(AD_M1_INTR_ENABLE, intr_en);
	}
#endif
#if defined(CHIP_NAME_d14)
	{
		UINT32		intr_en;
		//VCP interrupt enable -> D14
		intr_en = 0;
		//0xB00800E8 MCU register setting
		intr_en = GetReg(AD_M1_INTR_ENABLE);
		intr_en |= SYNC_HW_INT_EN; //VCP interrupt
		//intr_en |= 0x6E;	//EnableBit [1] HEVC0, [2] HEVC1, [3]H.264, [5]TS {CM3_1, CM3_0, PDEC, TE}, [6]SYNC{VDO, VCP}
		SetReg(AD_M1_INTR_ENABLE, intr_en);

		intr_en = 0;
		// set [6] DISP_INTR = {VDO_intr, VCP_intr}
		//0xB0080218 MCU register setting
		intr_en = GetReg(AD_M1_SYNC_INTR_EN);
		intr_en |= 0x01; //[1] VDO(0), [0] VCP(1)
		SetReg(AD_M1_SYNC_INTR_EN, intr_en);
	}
#endif


	VSync_HAL_Init();
	VSync_HAL_IRQ_Init(_ISR_VSync);
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
enum hrtimer_restart hrtimer_callback(struct hrtimer *timer)
{
	VSYNC_CH_T *pstVsync;
	UINT8		ui8VSyncCh;

	// TODO : vsync ch retrieving
	pstVsync = container_of(timer, VSYNC_CH_T, hr_timer);
	ui8VSyncCh = pstVsync->Config.u8VsyncCh;
	//pstVsync = &gsVSync[ui8VSyncCh];
	pstVsync->bTimerSet = FALSE;

	log_user3("	timer expired %X\n",(UINT32)pstVsync);
	VDEC_ISR_VVSync(ui8VSyncCh);

	return HRTIMER_NORESTART;
}

void _vsync_start_sw_sync(void);
enum hrtimer_restart sw_vsync_callback(struct hrtimer *timer)
{
	VSYNC_HW	*vsync_src = &vsync_hws[VSYNC_SWSYNC_CH];

	// TODO : vsync ch retrieving
	bTimerSet = FALSE;

	log_user3("	sw vsync\n");

	_ISR_VSync(VSYNC_SWSYNC_CH);

	if (vsync_src->status == VSYNC_HW_ACTIVE)
		_vsync_start_sw_sync();

	return HRTIMER_NORESTART;
}

void _vsync_free_hw_src(UINT8 vsync_ch, UINT8 vsync_src_ch)
{
	VSYNC_CH_T	*pstVsync;
	VSYNC_HW	*pstVsyncHw;
	UINT8		slot;
	VSYNC_LINK_T	*links;
	BOOLEAN		is_enable;

	log_noti("[VSYNC%u] free vsync source to %u\n", vsync_ch, vsync_src_ch);

	pstVsync = &gsVSync[vsync_ch];
	if (vsync_src_ch == pstVsync->vsync_src_ch)
	{
		log_trace("[VSYNC%d] already set same vsync src(%u)\n",
			vsync_ch, vsync_src_ch);

		return;
	}

	vsync_src_ch = pstVsync->vsync_src_ch;
	if (vsync_src_ch == 0xFF)
	{
		log_noti("[vsync%u] already no vsync src\n", vsync_ch);
		return;
	}

	pstVsyncHw = &vsync_hws[vsync_src_ch];

	// TODO - remove slot if new vsync source is invalid
	// find empty slot
	links = pstVsyncHw->links;
	for (slot = 0; slot < VSYNC_MAX_CONNECT; slot++)
	{
		if (links[slot].vsync_ch == vsync_ch)
		{
			log_trace("[VSYNC%u] found vsync ch, slot:%u of vsync_src_ch:%u\n",
				vsync_ch, slot, vsync_src_ch);
			break;
		}
	}

	if (slot == VSYNC_MAX_CONNECT)
	{
		log_error("[VSYNC%d] can't find from vsync src slot %d\n",
				vsync_ch, vsync_src_ch);
		return;
	}
	pstVsync->vsync_src_ch = 0xFF;
	links[slot].bUse = FALSE;
	links[slot].vsync_ch = 0xFF;

	pstVsyncHw->n_connected--;

	if (pstVsyncHw->status == VSYNC_HW_ACTIVE && pstVsyncHw->n_connected == 0)
	{
		//_vsync_reset(pstVsync);
		pstVsync->Debug.ui32LogTick_ISR = 0;
		pstVsync->Debug.ui32LogTick_Dual = 0;
		pstVsync->Debug.ui32GSTC_Prev = 0xFFFFFFFF;
		pstVsync->Debug.ui32DurationTick = 0;
		pstVsync->Debug.ui32DualTickDiff = 0;

#ifdef VDEC_USE_MCU
		hrtimer_cancel(&pstVsync->hr_timer);
#endif

		if (vsync_src_ch != VSYNC_SWSYNC_CH)
		{
			is_enable = TOP_HAL_IsVsyncIntrEnable(VSYNC0+vsync_src_ch);
			if (is_enable == FALSE)
				log_error("[VSYNC%u] VSync Intr %u Already Disabled\n",
						vsync_ch, vsync_src_ch);

			TOP_HAL_DisableVsyncIntr(VSYNC0+vsync_src_ch);
			TOP_HAL_ClearVsyncIntr(VSYNC0+vsync_src_ch);
		}
		else
		{
			log_noti("[VSYNC%u] turn off sw sync\n", vsync_ch);
			//hrtimer_cancel(&hr_timer);
		}

		log_noti("[VSYNC%u] Vsync src %u disable interrupt\n",
				vsync_ch, vsync_src_ch);

		pstVsyncHw->status = VSYNC_HW_INACTIVE;
	}

}

void _vsync_start_sw_sync(void)
{
	ktime_t ktime;
	UINT32		u32Time90K = 3003;

	if (bTimerSet)
	{
		log_warning("timer aleady set\n");
		return;
	}
	bTimerSet = TRUE;

	ktime = ktime_set(0, MS_TO_NS(u32Time90K/90));


	hr_timer.function = &sw_vsync_callback;

	log_user3("Starting timer %ums %u\n", u32Time90K/90, u32Time90K );

	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );
	log_user3("--complete timer setting %ums %u\n", u32Time90K/90, u32Time90K );
}

void _vsync_set_hw_src(UINT8 vsync_ch, UINT8 vsync_src_ch)
{
	VSYNC_CH_T	*pstVsync;
	VSYNC_HW	*pstVsyncHw;
	UINT8		i;
	UINT8		slot = 0xFF;
	VSYNC_LINK_T	*links;

	log_noti("[VSYNC%u] set vsync source to src %u\n", vsync_ch, vsync_src_ch);

	pstVsync = &gsVSync[vsync_ch];
	if (vsync_src_ch == pstVsync->vsync_src_ch)
	{
		log_trace("[VSYNC%d] already set same vsync src(%u)\n",
			vsync_ch, vsync_src_ch);

		return;
	}

	pstVsyncHw = &vsync_hws[vsync_src_ch];
	pstVsync->vsync_src_ch = vsync_src_ch;

	// TODO - remove slot if new vsync source is invalid
	// find empty slot
	links = pstVsyncHw->links;
	for (i = 0; i < VSYNC_MAX_CONNECT; i++)
	{
		if (slot == 0xFF && links[i].bUse == FALSE)
		{
			slot = i;
		}
		else if (links[i].vsync_ch == vsync_ch)
		{
			log_error("[VSYNC%u] Already Set, slot:%u, vsync_src_ch:%u\n",
				vsync_ch, i, vsync_src_ch);
			return;
		}
	}

	if (slot == 0xFF)
	{
		log_error("[VSYNC%d] Not enough mux channel for vsync src %d\n",
				vsync_ch, vsync_src_ch);
		return;
	}

	links[slot].bUse = TRUE;
	links[slot].vsync_ch = vsync_ch;

	pstVsyncHw->n_connected++;

	if (vsync_src_ch != VSYNC_SWSYNC_CH)
		VSync_HAL_SetVsyncField(vsync_src_ch, pstVsync->Rate.ui32FrameResidual,
				pstVsync->Rate.ui32FrameDivider, pstVsync->Rate.bInterlaced);

	if (pstVsyncHw->status == VSYNC_HW_INACTIVE && pstVsyncHw->n_connected > 0)
	{
		_vsync_reset(pstVsync);

		if (vsync_src_ch != VSYNC_SWSYNC_CH)
		{
			// TODO : move to initial time
			TOP_HAL_SetLQSyncMode(vsync_src_ch, vsync_src_ch);

#ifdef ARM_USING_MMCU
			TOP_HAL_EnableInterIntr(VSYNC0+vsync_src_ch);
#endif

			TOP_HAL_ClearVsyncIntr(VSYNC0+vsync_src_ch);
			TOP_HAL_EnableVsyncIntr(VSYNC0+vsync_src_ch);
			VSync_HAL_EnableVsync(vsync_src_ch);
		}
		else
		{
			_vsync_start_sw_sync();
		}

		log_noti("[VSYNC%d] Enable Interrupt Vsync Src %d, %s \n",
				vsync_ch, vsync_src_ch, __FUNCTION__ );

		pstVsyncHw->status = VSYNC_HW_ACTIVE;
	}

}

UINT8 VSync_SetVsyncSrc(UINT8 vsync_ch, UINT8 vsync_src_ch)
{
	UINT8		pre_src_ch = VSYNC_INVAL_SRC;
	VSYNC_CH_T	*pstVsync;

	log_noti("[VSYNC%u] set vsync source to %u\n", vsync_ch, vsync_src_ch);

	pstVsync = &gsVSync[vsync_ch];
	pre_src_ch = pstVsync->vsync_src_ch;

	if (vsync_src_ch < VSYNC_NUMOF_SRC)
	{
		_vsync_free_hw_src(vsync_ch, 0xFF);
		if (vsync_src_ch != 0xFF)
			_vsync_set_hw_src(vsync_ch, vsync_src_ch);
	}
	else
	{
		log_noti("[VSYNC%u] inval vsync %u\n", vsync_ch, vsync_src_ch);
		//_vsync_set_sw_src();
	}

	return pre_src_ch;
}

UINT8 VSync_Open(UINT8 vsync_ch, BOOLEAN bIsDualDecoding,
		UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced, UINT8 u8Dual3dtype)
{
	VSYNC_CH_T	*pstVsync;
	UINT8		vsync_src_ch = VSYNC_INVAL_SRC;
	BOOLEAN		bIsIntrEnable;
	UINT8   vsync_src_ch_pair;

	if (vsync_ch >= VSYNC_NUM_OF_CH)
	{
		log_error("[VSYNC] try to open wrong vsync ch num %u\n", vsync_ch);
		return VSYNC_INVAL_CH;
	}
	pstVsync = &gsVSync[vsync_ch];

	pstVsync->u32NextResidual = ui32FrameRateRes;
	pstVsync->u32NextDivider = ui32FrameRateDiv;
	pstVsync->bNextInterlaced = bInterlaced;
	bPrePhaseShift[vsync_ch] = FALSE;
	// vsync src alloc
	if (bIsDualDecoding)
	{
#if defined (CHIP_NAME_h15) || (CHIP_NAME_m14)
		if (u8Dual3dtype == 0)
			vsync_src_ch = vsync_ch;
		else if (u8Dual3dtype == 1)
			vsync_src_ch = (vsync_ch^0x1);
		else if (u8Dual3dtype == 2)
			vsync_src_ch = vsync_ch & (~(0x01));
		else
		{
			vsync_src_ch = 0;
			log_warning ("[VSYNC%d] Dual 3D type is wrong type:%d, Default vsync src ch(%d) run\n"
											, vsync_ch, u8Dual3dtype, vsync_src_ch);
		}
#else
		vsync_src_ch = vsync_ch & (~(0x01));
#endif
		log_noti("[VSYNC%d] Dual channel - vsync src %d\n",
				vsync_ch, vsync_src_ch);
		//For Debug
		if (vdec_vsync_src_ch != 0xFF)
		{
			vsync_src_ch = vdec_vsync_src_ch;
		}
		if (vdec_3d_dual_sync_setting)
		{
			vsync_src_ch_pair = vsync_src_ch^0x1;
			if (vsync_src_ch_pair != VSYNC_SWSYNC_CH)
			{
				VSync_HAL_SetVsyncField(vsync_src_ch_pair, pstVsync->Rate.ui32FrameResidual,
												pstVsync->Rate.ui32FrameDivider, pstVsync->Rate.bInterlaced);
				VSync_HAL_EnableVsync(vsync_src_ch_pair);
			}
		}
	}
	else
		vsync_src_ch = vsync_ch;

	_vsync_set_rate_info(pstVsync, ui32FrameRateRes,
			ui32FrameRateDiv, bInterlaced);

	VSync_SetVsyncSrc(vsync_ch, vsync_src_ch);

	log_noti("[VSYNC%d] Vsync Open Request : vsync src %d DualDec:%d\n",
			vsync_ch, vsync_src_ch, bIsDualDecoding);

	bIsIntrEnable = TOP_HAL_IsVsyncIntrEnable(VSYNC0+vsync_src_ch);
	if( bIsIntrEnable == FALSE )
		log_error("[VSYNC%d] VSync Src Interrupt Disabled\n", vsync_src_ch);

	return vsync_ch;
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
void VSync_Close(UINT8 ui8VSyncCh, UINT8 ui8VdispCh)
{
	VSYNC_CH_T		*pstVsync;

	if (ui8VSyncCh >= VSYNC_NUMOF_CH)
	{
		log_error("[VSYNC%d][Err] %s\n", ui8VSyncCh, __FUNCTION__ );
		return;
	}

	log_noti("[VSYNC%d] close, vdisp %d\n", ui8VSyncCh, ui8VdispCh);
	pstVsync = &gsVSync[ui8VSyncCh];

	_vsync_free_hw_src(ui8VSyncCh, 0xFF);
	log_noti("[VSYNC%d] Sync:%d, %s \n", ui8VSyncCh, ui8VdispCh, __FUNCTION__);

	pstVsync->Rate.ui32FrameResidual = 0x0;
	pstVsync->Rate.ui32FrameDivider = 0x1;
	pstVsync->Rate.ui32FrameDuration90K = 0x0;
	pstVsync->Rate.bInterlaced = TRUE;
}

int VSync_IsActive (UINT8 ui8VSyncCh)
{
	if (ui8VSyncCh >= VSYNC_NUMOF_CH)
		return 0;

	return gsVSync[ui8VSyncCh].Status.ui32ActiveDeIfChMask != 0;
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
UINT32 VSync_CalFrameDuration(UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv)
{
	//use do_div() function
	UINT64 		ui64FrameRateDiv90K_Scaled;

	if( ui32FrameRateRes == 0 )
		return 0;

	ui64FrameRateDiv90K_Scaled = (UINT64)ui32FrameRateDiv * 90000;
	do_div( ui64FrameRateDiv90K_Scaled, ui32FrameRateRes ); //do_div(x,y) -> x = x/y

	return ui64FrameRateDiv90K_Scaled;
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
UINT32 VSync_IntField (UINT8 ui8VSyncCh)
{
	return VSync_HAL_IntField(ui8VSyncCh);
}

void VSync_SetMaxShift(UINT8 ui8VSyncCh, SINT32 max)
{
	log_noti("[VSYNC%d] shift %d\n", ui8VSyncCh, max);

	gsVSync[ui8VSyncCh].max_shift = max;
	gsVSync[ui8VSyncCh].max_intv27M = gsVSync[ui8VSyncCh].cur_intv27M+max;
	gsVSync[ui8VSyncCh].min_intv27M = gsVSync[ui8VSyncCh].cur_intv27M-max;
}

void VSync_SetPhaseShift(UINT8 ui8VSyncCh, SINT32 i32ShiftDelta90K)
{
	SINT32 i32ShiftDelta27M;

	if (phaseshift_disable)
		return;

	log_noti("[VSYNC%d] shift %d\n", ui8VSyncCh, i32ShiftDelta90K);

	i32ShiftDelta27M = 27000000/90000 * i32ShiftDelta90K;

	gsVSync[ui8VSyncCh].i32NextPhaseShift = i32ShiftDelta27M;
}

void VSync_SetDcoFrameRate(UINT8 ui8VSyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv)
{
	VSYNC_CH_T	*pstVsync;
	UINT64		t;

	if (ui8VSyncCh >= VSYNC_NUM_OF_CH)
	{
		log_error("[VSYNC%d][Err] Channel Number, %s\n",
				ui8VSyncCh, __FUNCTION__);
		return;
	}

	if (ui32FrameRateRes == 0)
	{
		log_error("[VSYNC%d][Err] wrong frame info. res %d div %d\n",
				ui8VSyncCh, ui32FrameRateRes, ui32FrameRateDiv);
		return;
	}

	pstVsync = &gsVSync[ui8VSyncCh];

	t = (UINT64)ui32FrameRateDiv * 27000000;
	do_div(t, ui32FrameRateRes );

	pstVsync->dco_intv27M = (UINT32)t;

	log_noti("[VSYNC%d] Set dco rate. res/div %u/%u, 27M %u",
			ui8VSyncCh, ui32FrameRateRes, ui32FrameRateDiv,
			pstVsync->dco_intv27M);
	return;
}

BOOLEAN VSync_SetNextVsyncField(UINT8 ui8VSyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced)
{
	if( ui8VSyncCh >= VSYNC_NUM_OF_CH )
	{
		log_error("[VSYNC%d][Err] Channel Number, %s\n", ui8VSyncCh, __FUNCTION__ );
		return FALSE;
	}
	gsVSync[ui8VSyncCh].u32NextResidual = ui32FrameRateRes;
	gsVSync[ui8VSyncCh].u32NextDivider = ui32FrameRateDiv;
	gsVSync[ui8VSyncCh].bNextInterlaced = bInterlaced;

	return TRUE;
}

UINT32 VSync_GetVsyncIntv(UINT8 ui8VSyncCh)
{
	VSYNC_CH_T	*pstVsync;
	UINT32		ret;

	if( ui8VSyncCh >= VSYNC_NUM_OF_CH )
	{
		log_error("[VSYNC%d][Err] Channel Number, %s\n", ui8VSyncCh, __FUNCTION__ );
		return 3003;
	}

	pstVsync = &gsVSync[ui8VSyncCh];
	ret = pstVsync->Rate.ui32FrameDuration90K;

	if (pstVsync->Rate.bInterlaced)
		ret /= 2;

	return ret;
}

UINT32 _vsync_get_base_intv27M(UINT32 intv27M)
{
	UINT32 ret;
	SINT32 diff10k;

	if (intv27M < 900000/2-1000)
		return 0;
	else if (intv27M < 900900/2+1000)		// 59.94, 60 Hz
		ret = 900000/2;
	else if (intv27M < 1081080/2+1000)		// 49.5, 50 Hz
		ret = 1080000/2;
	else if (intv27M < 900900+1000)			// 29.97, 30 Hz
		ret = 900000;
	else if (intv27M < 1081080+1000)		// 24.97, 25 Hz
		ret = 1080000;
	else if (intv27M < 1126125+1000)		// 23.97, 24 Hz
		ret = 1125000;
	else
		return 0;

	diff10k = ((SINT32)(intv27M-ret))*10000;
	diff10k /= ret;

	if (diff10k>11 || diff10k<-11)
		ret = 0;

	return ret;
}

void _vsync_set_rate_info(VSYNC_CH_T *pstVsync, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced)
{
	UINT32		ui32FrameDuration90K;
	UINT64		t;

	ui32FrameDuration90K = VSync_CalFrameDuration(ui32FrameRateRes,
			ui32FrameRateDiv);

	pstVsync->Rate.ui32FrameResidual = ui32FrameRateRes;
	pstVsync->Rate.ui32FrameDivider = ui32FrameRateDiv;
	pstVsync->Rate.bInterlaced = bInterlaced;
	pstVsync->Rate.ui32FrameDuration90K = ui32FrameDuration90K;

	t = (UINT64)27000000*ui32FrameRateDiv;
	do_div(t, ui32FrameRateRes);
	pstVsync->cur_intv27M = (UINT32)t;
	pstVsync->base_intv27M = _vsync_get_base_intv27M(pstVsync->cur_intv27M);
	//24 or 23.976Hz video could be miss sync lock for DE/U14/URSA9
	//so, VDED vsync shift variation range should be restricted in H13
#if defined (CHIP_NAME_h13) || (CHIP_NAME_h15)
	if (pstVsync->base_intv27M == 1125000)
	{
		pstVsync->min_intv27M = pstVsync->base_intv27M-vdec_vsync_shift_delta_limit;
		pstVsync->max_intv27M = pstVsync->base_intv27M+vdec_vsync_shift_delta_limit;
	}
	else
#endif
	if (pstVsync->base_intv27M != 0)
	{
		t = (UINT64)pstVsync->base_intv27M* (1000-vdec_vsync_shift_limit);
		do_div(t,1000);
		pstVsync->min_intv27M = t+20;
		t = (UINT64)pstVsync->base_intv27M* (1000+vdec_vsync_shift_limit);
		do_div(t,1000);
		pstVsync->max_intv27M = t-20;
	}
	else
	{
		pstVsync->base_intv27M = pstVsync->cur_intv27M;
		t = (UINT64)pstVsync->base_intv27M* (1000-vdec_vsync_shift_limit2);
		do_div(t,1000);
		pstVsync->min_intv27M = t+20;
		t = (UINT64)pstVsync->base_intv27M* (1000+vdec_vsync_shift_limit2);
		do_div(t,1000);
		pstVsync->max_intv27M = t-20;
	}
}

void VSync_SetPrePhaseShift(UINT8 u8VSyncCh,BOOLEAN bFirstFrameAfterSetBase)
{
	bPrePhaseShift[u8VSyncCh] = bFirstFrameAfterSetBase;
}

BOOLEAN VSync_SetVsyncField(UINT8 ui8VSyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced)
{
	SINT32		i32NextPhaseShift=0;
	BOOLEAN		bUpdated = FALSE;
	VSYNC_CH_T	*pstVsync;

	if( ui8VSyncCh >= VSYNC_NUM_OF_CH )
	{
		log_error("[VSYNC%d][Err] Channel Number, %s\n", ui8VSyncCh, __FUNCTION__ );
		return FALSE;
	}

	pstVsync = &gsVSync[ui8VSyncCh];

	if( (ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0) )
	{
		//log_error("[VSYNC%d] FrameRateRes:%d, FrameRateDiv:%d, %s\n",
		//		ui8VSyncCh, ui32FrameRateRes, ui32FrameRateDiv, __FUNCTION__);
		return FALSE;
	}

	// If we need to change vsync rate dynamically
	// for example the case of camera app.
	if (pstVsync->dco_intv27M)
		i32NextPhaseShift = pstVsync->dco_intv27M - pstVsync->cur_intv27M;

	if (pstVsync->i32NextPhaseShift!=0)
	{
		// dynamic rate change or just phase shift?

		// case for just pahse shift
		if (i32NextPhaseShift==0)
		{
			i32NextPhaseShift = pstVsync->i32NextPhaseShift;
		}
		// case for dynamic rate change & phase shift
		else if (i32NextPhaseShift>0)
		{
			if (pstVsync->i32NextPhaseShift>i32NextPhaseShift)
				i32NextPhaseShift = pstVsync->i32NextPhaseShift;
			else if (pstVsync->i32NextPhaseShift<0)
				i32NextPhaseShift += pstVsync->i32NextPhaseShift;
		}
		else
		{
			if (pstVsync->i32NextPhaseShift<i32NextPhaseShift)
				i32NextPhaseShift = pstVsync->i32NextPhaseShift;
			else if (pstVsync->i32NextPhaseShift>0)
				i32NextPhaseShift += pstVsync->i32NextPhaseShift;
		}
	}

	log_user2("[VSYNC%d] avg %u avg diff %d chgto %d  cur %d shift %d\n",
			ui8VSyncCh, pstVsync->dco_intv27M,
			pstVsync->dco_intv27M - pstVsync->cur_intv27M,
			i32NextPhaseShift,
			pstVsync->i32PhaseShift,
			pstVsync->i32NextPhaseShift
			);

	// limit changing delta.
	if (!bPrePhaseShift[ui8VSyncCh])
	{
		// because chnaging vsync interval too much make disply(DE, FRC) wrong
		if (pstVsync->i32PhaseShift-i32NextPhaseShift > vdec_vsync_shift_delta)
			i32NextPhaseShift = pstVsync->i32PhaseShift - vdec_vsync_shift_delta;
		if (pstVsync->i32PhaseShift-i32NextPhaseShift < -vdec_vsync_shift_delta)
			i32NextPhaseShift = pstVsync->i32PhaseShift + vdec_vsync_shift_delta;

		// limit absolute value
		// make vsync interval not too larg nor small
		if (pstVsync->cur_intv27M+i32NextPhaseShift>pstVsync->max_intv27M)
			i32NextPhaseShift = pstVsync->max_intv27M-pstVsync->cur_intv27M;
		else if (pstVsync->cur_intv27M+i32NextPhaseShift<pstVsync->min_intv27M)
			i32NextPhaseShift = pstVsync->min_intv27M-pstVsync->cur_intv27M;
	}

	// if there is chnage of vsync interval
	if( (pstVsync->Rate.ui32FrameResidual != ui32FrameRateRes) ||
			(pstVsync->Rate.ui32FrameDivider != ui32FrameRateDiv) ||
			(pstVsync->Rate.bInterlaced != bInterlaced) ||
			pstVsync->i32PhaseShift != i32NextPhaseShift )
	{
		UINT32		ui32FieldRate_Prev;
		UINT32		ui32FieldRate_Curr;
		UINT32		pre_intv90K;

		pre_intv90K = pstVsync->Rate.ui32FrameDuration90K;

		_vsync_set_rate_info(pstVsync, ui32FrameRateRes,
				ui32FrameRateDiv, bInterlaced);

		ui32FieldRate_Prev = (pstVsync->Rate.bInterlaced == TRUE) ? 2 : 1;
		ui32FieldRate_Curr = (bInterlaced == TRUE) ? 2 : 1;

		log_noti("[VSYNC%d]FieldDuration90k: %d --> %d %d/%d shift %d\n",
				ui8VSyncCh,
				pstVsync->Rate.ui32FrameDuration90K / ui32FieldRate_Prev,
				pre_intv90K / ui32FieldRate_Curr,
				pre_intv90K, ui32FieldRate_Curr,
				i32NextPhaseShift/300);

		if( !VSync_HAL_IntField(ui8VSyncCh) )
		{
			pstVsync->i32PhaseShift = i32NextPhaseShift;
			VSync_HAL_SetVsyncShift(ui8VSyncCh, pstVsync->i32PhaseShift);
		}

		VSync_HAL_SetVsyncField(ui8VSyncCh, ui32FrameRateRes,
				ui32FrameRateDiv, bInterlaced);

		bUpdated = TRUE;
	}

	if (pstVsync->i32NextPhaseShift && !VSync_HAL_IntField(ui8VSyncCh))
	{
		SINT32 pre_break, abs;

		// Calculate when we need to break vsync shift.
		// Because we can't set vsync shift value to 0 from big shift value.
		// Therefore we calculate the timming to break
		// and start to decrease shift value slowly
		pre_break = pstVsync->i32PhaseShift*pstVsync->i32PhaseShift;
		pre_break /= 2*vdec_vsync_shift_delta;

		if (pstVsync->i32NextPhaseShift>0)
			abs = pstVsync->i32NextPhaseShift;
		else
			abs = -(pstVsync->i32NextPhaseShift);

		if (abs >= pre_break)
			pstVsync->i32NextPhaseShift -= pstVsync->i32PhaseShift;
		else
			pstVsync->i32NextPhaseShift=0;
	}

	return bUpdated;
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
static inline void _VSYNC_Debug_Log(VSYNC_CH_T *pstVsync, UINT32 u32Gstc)
{
	UINT32			ui32DurationTick = 0;
	UINT8			ui8VSyncCh = pstVsync->Config.u8VsyncCh;
	VSYNC_LINK_T	*pstSync;

	pstVsync->Debug.ui32LogTick_ISR++;
	if( pstVsync->Debug.ui32LogTick_ISR >= 0x100 )
	{
		static UINT8		ui8UseCh = 0;

		if( pstVsync->Config.astSyncChTag[ui8UseCh].bUse )
		{
			pstSync = &pstVsync->Config.astSyncChTag[ui8UseCh];
			log_noti("[VSy%d:%d,VDS%d] STC:0x%X DBG:0x%X Rate:%d, "
					"DualTickDiff:%d\n",
					ui8VSyncCh, ui8UseCh, pstSync->vsync_ch,
					u32Gstc, u32Gstc,
					90000 / (u32Gstc - pstVsync->Debug.ui32GSTC_Prev),
					pstVsync->Debug.ui32DualTickDiff);

			pstVsync->Debug.ui32LogTick_ISR = 0;
		}

		ui8UseCh++;
		if( ui8UseCh >= VSYNC_MAX_CONNECT )
			ui8UseCh=0;
	}

	if( pstVsync->Debug.ui32GSTC_Prev != 0xFFFFFFFF )
		ui32DurationTick = u32Gstc - pstVsync->Debug.ui32GSTC_Prev;

	if( pstVsync->Debug.ui32DurationTick )
	{
		SINT32		intv_diff;

		intv_diff = ui32DurationTick - pstVsync->Debug.ui32DurationTick;
		if (intv_diff<0)
			intv_diff = -(intv_diff);

		if( intv_diff > 270 )	// > 3ms
			log_noti("[VSYNC%d] Unstable vsync: intv %X->%X\n",
					ui8VSyncCh, pstVsync->Debug.ui32DurationTick, ui32DurationTick);
	}

	pstVsync->Debug.ui32DurationTick = ui32DurationTick;
	pstVsync->Debug.ui32GSTC_Prev = u32Gstc;

#ifdef  VSYNC_ISR_PERFORMANCE_EVAL
	// performance check
	{
		struct timeval tv_end;
		static UINT32 u32TotalTime=0, u32Cnt=0, u32Max=0;
		UINT32 t;

		do_gettimeofday(&tv_end);

		u32Cnt++;
		t = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
				tv_end.tv_usec - tv_start.tv_usec;
		if( t>u32Max ) u32Max = t;

		u32TotalTime += t;

		if( u32Cnt >= 200 )
		{
			log_error("Ev.t %u Max %u\n", u32TotalTime/u32Cnt, u32Max);
			u32Cnt = 0;
			u32TotalTime = 0;
			u32Max = 0;
		}
	}
#endif

}

UINT32 VSYNC_GetFieldDuration(UINT8 u8VSyncCh)
{
	UINT32 u32Ret;

	u32Ret = gsVSync[u8VSyncCh].Rate.ui32FrameDuration90K;
	if( gsVSync[u8VSyncCh].Rate.bInterlaced )
		u32Ret = u32Ret>>1;

	return u32Ret;
}

void VSYNC_SetVVsyncTimer(UINT8 u8VSyncCh, UINT32 u32Time90K)
{
	VSYNC_CH_T	*pstVsync;
	ktime_t ktime;

	pstVsync = &gsVSync[u8VSyncCh];

	if (pstVsync->Status.ui32ActiveDeIfChMask == 0x0)
		return;

	if (pstVsync->bTimerSet)
	{
		log_warning("timer aleady set\n");
		return;
	}
	pstVsync->bTimerSet = TRUE;

	ktime = ktime_set(0, MS_TO_NS(u32Time90K/90));

	hrtimer_init(&pstVsync->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

	pstVsync->hr_timer.function = &hrtimer_callback;

	log_user3("Starting timer %ums %u\n", u32Time90K/90, u32Time90K );

	hrtimer_start( &pstVsync->hr_timer, ktime, HRTIMER_MODE_REL );

}


static void VDEC_ISR_VVSync(UINT8 ui8VSyncCh)
{
	VSYNC_CH_T *pstVsync;
	UINT8			i;
	UINT32			u32Gstc;
	UINT32			u32IntnField;
	UINT32			u32nSync = 0;
	UINT8			au8SyncCh[VSYNC_MAX_CONNECT];

	if (ui8VSyncCh >= VSYNC_NUM_OF_CH)
	{
		log_error("[VVSync%d][Err] Channel NO. %s\n", ui8VSyncCh, __FUNCTION__ );
		goto _vvsync_exit;
	}

	pstVsync = &gsVSync[ui8VSyncCh];
	// sync scan
	for( i = 0; i < VSYNC_MAX_CONNECT; i++ )
	{
		if( pstVsync->Config.astSyncChTag[i].bUse == TRUE )
			au8SyncCh[u32nSync++] = pstVsync->Config.astSyncChTag[i].vsync_ch;
	}

	u32IntnField = (VSync_HAL_IntField(ui8VSyncCh));
	u32Gstc = TOP_HAL_GetGSTCC();

	log_user3("[VVsync%d] GSTC %d  Field %d\n", ui8VSyncCh, u32Gstc, u32IntnField);

	VDISP_VVsyncCallback(ui8VSyncCh, u32nSync, au8SyncCh, u32IntnField);

_vvsync_exit:

	return;
}

static inline void _vsync_set_vvsync_timer(UINT8 ui8VSyncCh, SINT8 i8RelativeMs)
{
	UINT32 u32FieldIntv;	// 90K
	SINT32 i32TimerDelta;	// 90K

	u32FieldIntv = VSYNC_GetFieldDuration(ui8VSyncCh);
	i32TimerDelta = u32FieldIntv + (i8RelativeMs*90);	// before 4ms from next vsync

	if (i32TimerDelta<1*90)
		i32TimerDelta = 1*90;

	VSYNC_SetVVsyncTimer(ui8VSyncCh, (UINT32)i32TimerDelta);
}

static void _ISR_VSync(UINT8 vsync_src_ch)
{
	VSYNC_CH_T		*pstVsync = NULL;
	UINT8			i;
	UINT32			u32Gstc;
	UINT32			u32IntnField;
	UINT32			u32nSync = 0;
	UINT8			au8SyncCh[VSYNC_MAX_CONNECT];
	VSYNC_HW		*pstVsyncHw;

	if (vsync_src_ch >= VSYNC_NUM_OF_CH)
	{
		log_error("[VSRC%d] Channel NO. %s\n", vsync_src_ch, __FUNCTION__ );
		goto _vsync_exit;
	}

#ifdef  VSYNC_ISR_PERFORMANCE_EVAL
	{
		//static struct timeval tv_pre_start;
		do_gettimeofday(&tv_start);
		//log_error("intv %u\n",
		//		(tv_start.tv_sec - tv_pre_start.tv_sec) * 1000000 +
		//		tv_start.tv_usec - tv_pre_start.tv_usec);
		//tv_pre_start = tv_start;
	}
#endif

	pstVsyncHw = &vsync_hws[vsync_src_ch];

	// sync scan
	for( i = 0; i < VSYNC_MAX_CONNECT; i++ )
	{
		if (pstVsyncHw->links[i].bUse == TRUE)
			au8SyncCh[u32nSync++] = pstVsyncHw->links[i].vsync_ch;
	}

	if (u32nSync > 0)
		pstVsync = &gsVSync[au8SyncCh[0]];

	if (vsync_src_ch != VSYNC_SWSYNC_CH)
		u32IntnField = (VSync_HAL_IntField(vsync_src_ch));
	else
		u32IntnField = 0;

	u32Gstc = TOP_HAL_GetGSTCC();

	log_user3("[VSRC%d] GSTC %d  Field %d\n", vsync_src_ch,
			u32Gstc, u32IntnField);

#if (VVSYNC_TIMING>0)
	VDISP_VVsyncCallback(vsync_src_ch, u32nSync, au8SyncCh, u32IntnField);
#endif

#ifndef ARM_USING_MMCU
	if( fnFeedCallback != NULL )
		fnFeedCallback(vsync_src_ch, u32nSync, au8SyncCh, u32IntnField);
#endif

	if (vsync_src_ch != VSYNC_SWSYNC_CH && pstVsync != NULL)
		VSync_SetVsyncField(vsync_src_ch, pstVsync->u32NextResidual,
				pstVsync->u32NextDivider, pstVsync->bNextInterlaced );

#if (VVSYNC_TIMING<0)
	_vsync_set_vvsync_timer(vsync_src_ch, VVSYNC_TIMING);
#elif (VVSYNC_TIMING==0)
	VDISP_VVsyncCallback(vsync_src_ch, u32nSync, au8SyncCh, u32IntnField);
#endif

	//_VSYNC_Debug_Log(pstVsync, u32Gstc);

_vsync_exit:

	return;
}

#define VDS_DPRINT(frm, args...)		seq_printf(m, frm, ##args )
void _vsync_print_src(struct seq_file *m, VSYNC_HW *vsync_src)
{
	UINT8		i;

	VDS_DPRINT(" status       : %u\n", vsync_src->status);
	VDS_DPRINT(" main_vsync   : %u\n", vsync_src->main_vsync);
	VDS_DPRINT(" n_connected  : %u\n", vsync_src->n_connected);

	for (i=0; i<VSYNC_MAX_CONNECT; i++)
		VDS_DPRINT(" - vsync ch : %u\n",
				vsync_src->links[i].vsync_ch);
}

void _vsync_print_channel(struct seq_file *m, VSYNC_CH_T *pstVsyncCh)
{
	VDS_DPRINT(" i32PhaseShift     : %d\n", (UINT32)pstVsyncCh->i32PhaseShift);
	VDS_DPRINT(" i32NextPhaseShift : %d\n", (UINT32)pstVsyncCh->i32NextPhaseShift);
	VDS_DPRINT(" u32NextResidual   : %u\n", (UINT32)pstVsyncCh->u32NextResidual);
	VDS_DPRINT(" u32NextDivider    : %u\n", (UINT32)pstVsyncCh->u32NextDivider);
	VDS_DPRINT(" bNextInterlaced   : %u\n", (UINT32)pstVsyncCh->bNextInterlaced);
	VDS_DPRINT(" bTimerSet         : %u\n", (UINT32)pstVsyncCh->bTimerSet);
	VDS_DPRINT(" ui32FrameResidual    : %u\n", (UINT32)pstVsyncCh->Rate.ui32FrameResidual);
	VDS_DPRINT(" ui32FrameDivider     : %u\n", (UINT32)pstVsyncCh->Rate.ui32FrameDivider);
	VDS_DPRINT(" ui32FrameDuration90K : %u\n", (UINT32)pstVsyncCh->Rate.ui32FrameDuration90K);
	VDS_DPRINT(" bInterlaced          : %u\n", (UINT32)pstVsyncCh->Rate.bInterlaced);
	VDS_DPRINT(" u8VsyncCh   : %u\n", (UINT32)pstVsyncCh->Config.u8VsyncCh);

	VDS_DPRINT(" max_shift         : %d\n", (UINT32)pstVsyncCh->max_shift);
	VDS_DPRINT(" dco_intv27M       : %u\n", pstVsyncCh->dco_intv27M);
	VDS_DPRINT(" cur_intv27M       : %u\n", pstVsyncCh->cur_intv27M);
	VDS_DPRINT(" min_intv27M       : %u\n", pstVsyncCh->min_intv27M);
	VDS_DPRINT(" max_intv27M       : %u\n", pstVsyncCh->max_intv27M);
	VDS_DPRINT(" base_intv27M      : %u\n", pstVsyncCh->base_intv27M);

	VDS_DPRINT(" vsync_src_ch         : %u\n", pstVsyncCh->vsync_src_ch);

	VDS_DPRINT(" ui32ActiveDeIfChMask  : %u\n", pstVsyncCh->Status.ui32ActiveDeIfChMask);

	VDS_DPRINT(" ui32LogTick_ISR  : %u\n", (UINT32)pstVsyncCh->Debug.ui32LogTick_ISR);
	VDS_DPRINT(" ui32LogTick_Dual : %u\n", (UINT32)pstVsyncCh->Debug.ui32LogTick_Dual);
	VDS_DPRINT(" ui32GSTC_Prev    : %u\n", (UINT32)pstVsyncCh->Debug.ui32GSTC_Prev);
	VDS_DPRINT(" ui32DurationTick : %u\n", (UINT32)pstVsyncCh->Debug.ui32DurationTick);
	VDS_DPRINT(" ui32DualTickDiff : %u\n", (UINT32)pstVsyncCh->Debug.ui32DualTickDiff);
	VDS_DPRINT("\n\n");
}

void VSYNC_PrintDebug(struct seq_file *m)
{
	VSYNC_CH_T		*pstVsyncCh;
	VSYNC_HW		*vsync_hw;
	UINT32			i;

	for (i=0; i<VSYNC_NUMOF_SRC; i++)
	{
		vsync_hw = &vsync_hws[i];
		VDS_DPRINT("====  Vsync Src #%d  ====\n", i);
		_vsync_print_src(m, vsync_hw);
	}

	VDS_DPRINT("\n\n");

	for (i=0;i<VSYNC_NUM_OF_CH;i++)
	{
		pstVsyncCh = &gsVSync[i];
		VDS_DPRINT("====  Vsync #%d [%X]  ====\n", i, (UINT32)pstVsyncCh);
		_vsync_print_channel(m, pstVsyncCh);
	}

#undef VDS_DPRINT
}

void VSYNC_Suspend(void)
{
	VSync_HAL_Suspend();
}

void VSYNC_Resume(void)
{
	VSync_HAL_Resume();
}
