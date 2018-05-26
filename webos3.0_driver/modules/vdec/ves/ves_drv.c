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
 * main driver implementation for vdec device.
 * vdec device will teach you how to make device driver with new platform.
 *
 * author     junghyun.son@lge.com
 * version    1.0
 * date       2013.10.29
 * note       Additional information.
 *
 * @addtogroup vdec_ves
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ves_drv.h"
#include "vdc/vdec_stc_timer.h"

#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <asm/uaccess.h> // copy_from_user
#include <asm/div64.h> // do_div
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "include/vdec_kapi.h"
#include "hal/top_hal_api.h"
#include "hal/ipc_reg_api.h"
#include "hal/vdec_base.h"

#include "log.h"

logm_define (vdec_ves, log_level_warning);

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define	VDEC_SPEED_MAX						16
/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
static DEFINE_MUTEX (ves_lock);
/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum
{
	DTS_JITTER,
	DTS_SAME,
	DTS_NORMAL,
	DTS_WRAPAROUND,
	DTS_UINT32 = 0x20110725
} E_DTS_DISCONTINUITY_T;

typedef struct
{
	enum
	{
		VES_NULL = 0,
		VES_READY = 10,
		VES_PLAY = 20,
		VES_PAUSE,
	} State;
	struct
	{
		E_VES_SRC_T	eVesSrcCh;
		void		*priv;
		E_PDEC_CODEC_TYPE_T eCodecType_Config;
		BOOLEAN		b512bytesAligned;
		VES_EVENT_T 	fpBufferUpdated;
		void 		*pBufferUpdatedArg;

	} Config;
	struct
	{
		BOOLEAN		bSTCCDiscontinuity;
		UINT32		ui32DetectedCount;
		UINT32		ui32DetectedCountIRQ;

		volatile BOOLEAN 	bFlushing;
	} Status;
} S_VDEC_VES_DB_T;

typedef struct
{
	UINT8	ui8VesCh;
	UINT32	ui32au_type;
	UINT32	ui32UserBuf;
	UINT32	ui32UserBufSize;
	BOOLEAN	bIsDTS;
	UINT32	ui32UId;
	UINT64	ui64TimeStamp;
	UINT32	ui32TimeStamp_90kHzTick;
} S_IPC_MCUWRITE_T;


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
UINT32	ui32WorkfuncParam = 0x0;
static void		_VES_ISR_PIC_Detected(UINT8 ui8VesCh);
static void 		_VES_PIC_Detected_workfunc(struct work_struct *data);
static UINT8		_VES_GetVesCh(UINT8 ui8PdecCh);
static irqreturn_t	_VES_IrqHandler(int irq, void *dev_id);

DECLARE_WORK( _VDEC_VES_work, _VES_PIC_Detected_workfunc );
/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
volatile S_VDEC_VES_DB_T gsVesDrv[VDEC_NUM_OF_CHANNEL];

#if (LINUX_VERSION_CODE < KERNEL_VERSION (3,7,0))
extern struct workqueue_struct *system_wq;
#define vdec_wq	system_wq
#else
extern struct workqueue_struct *system_highpri_wq;
#define vdec_wq	system_highpri_wq
#endif

static spinlock_t	stVdecVesSpinlock;

/**
 ********************************************************************************
 * @brief
 * translating upper layer's structure to current layer
 * @remarks
 * LX_VDEC_CODEC_T is defined at vdec_kapi.h, and it used by driver.c, vo.c, lxvdec.c
 * E_PDEC_CODEC_TYPE_T is defined at pdec_hal_api.h, it used by ves_drv.c
 * this function is translating codec type of LX_VDEC_CODEC_T to E_PDEC_CODEC_TYPE_T
 * @par requirements:
 * LX_VDEC_CODEC_T
 * @param
 * LX_VDEC_CODEC_H264_HP to LX_VDEC_CODEC_HEVC
 * @return
 * translated E_PDEC_CODEC_TYPE_T codec type
 ********************************************************************************
 */
static E_PDEC_CODEC_TYPE_T _VES_GetTranslateCodecType(LX_VDEC_CODEC_T codec)
{
	switch (codec)
	{
		case LX_VDEC_CODEC_H264_HP:
		case LX_VDEC_CODEC_H264_MVC:
			return PDEC_CODEC_H264;

		case LX_VDEC_CODEC_VC1_RCV_V1:
		case LX_VDEC_CODEC_VC1_RCV_V2:
		case LX_VDEC_CODEC_VC1_ES:
			return PDEC_CODEC_VC1;

		case LX_VDEC_CODEC_MPEG2_HP:
			return PDEC_CODEC_MPEG2;

		case LX_VDEC_CODEC_MPEG4_ASP:
		case LX_VDEC_CODEC_XVID:
		case LX_VDEC_CODEC_DIVX3:
		case LX_VDEC_CODEC_DIVX4:
		case LX_VDEC_CODEC_DIVX5:
		case LX_VDEC_CODEC_H263_SORENSON:
			return PDEC_CODEC_MPEG4;

		case LX_VDEC_CODEC_RVX:
			return PDEC_CODEC_RVx;

		case LX_VDEC_CODEC_AVS:
			return PDEC_CODEC_AVS;
		case LX_VDEC_CODEC_HEVC:
			return PDEC_CODEC_HEVC;
		case LX_VDEC_CODEC_VP8:
		case LX_VDEC_CODEC_THEORA:
        case LX_VDEC_CODEC_VP9:
			return PDEC_CODEC_NOT_SUPPORT;
		default:
			logm_warning (vdec_ves, "unknown codec type %d\n", codec);
			return PDEC_CODEC_INVALID;

			//case LX_VDEC_CODEC_JPEG:
			//	return PDEC_CODEC_JPEG;
	}
}

/**
 ********************************************************************************
 * @brief
 * get ves channel using pdec channel
 * @remarks
 * get ves channel using pdec channel
 * @par requirements:
 * int
 * @param
 * pdec channel number
 * @return
 * ves channel number
 ********************************************************************************
 */
static UINT8 _VES_GetVesCh(UINT8 ui8PdecCh)
{
	UINT8 idx;

	for(idx = 0; idx < VDEC_NUM_OF_CHANNEL; idx++)
	{
		if( gsVesDrv[idx].State == VES_NULL )
			continue;

		if( ui8PdecCh == gsVesDrv[idx].Config.eVesSrcCh )
			break;
	}

	return idx;
}

/**
 ********************************************************************************
 * @brief
 * processing picture detected work
 * @remarks
 * this function called by ves work_queue.
 * if it called first time after flushing, flushing pdec. *
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
static void _VES_PIC_Detected(UINT8 ui8VesCh)
{
	switch( ui8VesCh )
	{
		case 0 :
		case 1 :
		case 2 :
			if( gsVesDrv[ui8VesCh].Status.bFlushing == TRUE )
			{
				PDEC_HAL_Flush(gsVesDrv[ui8VesCh].Config.eVesSrcCh);
                /* i think below codes make difference of hw/sw pdec wr/rd pointer.
                 * if you want to update wr ptr after flushing,
                 * rd ptr must be updated also.
				VES_AUIB_UpdateWrPtr(ui8VesCh, PDEC_HAL_AUIB_GetWrPtr(gsVesDrv[ui8VesCh].Config.eVesSrcCh));
				VES_CPB_UpdateWrPtr(ui8VesCh, PDEC_HAL_CPB_GetWrPtr(gsVesDrv[ui8VesCh].Config.eVesSrcCh)); */

				gsVesDrv[ui8VesCh].Status.bFlushing = FALSE;
			}

			if( gsVesDrv[ui8VesCh].State == VES_PLAY)
			{
				VES_UpdatePicture(ui8VesCh,0x0,0x0,0,0,-1);
			}
			else
			{
				logm_noti(vdec_ves, "[VES%d] Not playing but Detected..Closing? State:%d \n",
						ui8VesCh, gsVesDrv[ui8VesCh].State);
			}
			break;
		default :
			logm_error(vdec_ves, "[VES%d][Err] Channel Number\n", ui8VesCh);
			break;
	}
}

/**
 ********************************************************************************
 * @brief
 * pop one ves work (bottom half part)
 * @remarks
 * this function is registered for ves worker.
 * if kernel version is higher than 3.7, work queue will has high priority property.
 * @par requirements:
 * work_struct*
 * @param
 * data(ves channel number)
 * @return
 * NONE
 ********************************************************************************
 */
static void _VES_PIC_Detected_workfunc(struct work_struct *data)
{
	UINT8	ui8VesCh = 0;
	UINT32	ui32VesChBitMask = ui32WorkfuncParam;
	ui32WorkfuncParam = 0x0;

	mutex_lock (&ves_lock);

	while( ui32VesChBitMask )
	{
		if( ui32VesChBitMask & 0x1 )
			_VES_PIC_Detected(ui8VesCh);

		ui32VesChBitMask >>= 1;
		ui8VesCh++;
	}

	mutex_unlock (&ves_lock);
}

/**
 ********************************************************************************
 * @brief
 * push one ves work (top half part)
 * @remarks
 * if pdec hw parsed one AU, interrupt will happen.
 * after received interrupt, ves pushes one work to queue.
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
static void _VES_ISR_PIC_Detected(UINT8 ui8VesCh)
{
	ui32WorkfuncParam |= (1 << ui8VesCh);
	queue_work(vdec_wq,  &_VDEC_VES_work);

	if (gsVesDrv[ui8VesCh].Status.ui32DetectedCountIRQ < 3)
	{
		logm_noti (vdec_ves, "[VES%d] PIC Detected %d\n", ui8VesCh,
				gsVesDrv[ui8VesCh].Status.ui32DetectedCountIRQ);

		gsVesDrv[ui8VesCh].Status.ui32DetectedCountIRQ ++;
	}
}

/**
 ********************************************************************************
 * @brief
 * ves irq handler
 * @remarks
 * live channel must use pdec hw.
 * pdec notices below interrupt to driver.
 * CPB_ALMOST_FULL
 * CPB_ALMOST_EMPTY
 * AUIB_ALMOST_FULL
 * AUIB_ALMOST_EMPTY
 * AU_DETECT
 * LQC0_ALMOST_EMPTY
 * actually, LQC0_ALMOST_EMPTY is used by feed timer. (it named before L9)
 * it's purpose is hw watchdog for vdc.
 * @par requirements:
 * int, void*
 * @param
 * irq number, device id
 * @return
 * irq return value
 ********************************************************************************
 */
static irqreturn_t _VES_IrqHandler(int irq, void *dev_id)
{
	irqreturn_t ret = IRQ_NONE;
	UINT32 ui32IntReason = 0;
	UINT32 ui32BufIntReason = 0;
	UINT8 ui8VesCh;

	ui32IntReason = TOP_HAL_GetIntrStatus();

	if( ui32IntReason & (1 << BUFFER_STATUS))
	{
		ui32BufIntReason = TOP_HAL_GetBufIntrStatus();

		TOP_HAL_ClearIntrMsk(1 << BUFFER_STATUS);
		// handle pdec isr
		if( ui32BufIntReason & (1 << PDEC0_CPB_ALMOST_FULL) )
		{
			ui8VesCh = _VES_GetVesCh(0);
			PDEC_HAL_CPB_AlmostFull(0);

			logm_warning(vdec_ves, "[VES%d][PDEC%d]CPB Almost Full - Buffer Level:%d, Status:%d, AUIB:%d \n"
					, ui8VesCh, 0, PDEC_HAL_CPB_GetBufferLevel(0)
					, PDEC_HAL_GetBufferStatus(0), VES_AUIB_NumActive(ui8VesCh) );

			if( gsVesDrv[ui8VesCh].Config.fpBufferUpdated )
				gsVesDrv[ui8VesCh].Config.fpBufferUpdated (ui8VesCh,
						gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg,
						VES_CPB_ALMOST_FULL);
		}
		if( ui32BufIntReason & (1 << PDEC0_CPB_ALMOST_EMPTY) )
		{
			PDEC_HAL_CPB_AlmostEmpty(0);
		}
		if( ui32BufIntReason & (1 << PDEC0_AUB_ALMOST_FULL) )
		{
			PDEC_HAL_AUIB_AlmostFull(0);
		}
		if( ui32BufIntReason & (1 << PDEC0_AUB_ALMOST_EMPTY) )
		{
			PDEC_HAL_AUIB_AlmostEmpty(0);
		}
		if( ui32BufIntReason & (1 << PDEC1_CPB_ALMOST_FULL) )
		{
			ui8VesCh = _VES_GetVesCh(1);
			PDEC_HAL_CPB_AlmostFull(1);

			logm_warning(vdec_ves, "[VES%d][PDEC%d]CPB Almost Full - Buffer Level:%d, Status:%d, AUIB:%d \n"
					, ui8VesCh, 1, PDEC_HAL_CPB_GetBufferLevel(1)
					, PDEC_HAL_GetBufferStatus(1), VES_AUIB_NumActive(ui8VesCh) );

			if( gsVesDrv[ui8VesCh].Config.fpBufferUpdated )
				gsVesDrv[ui8VesCh].Config.fpBufferUpdated (ui8VesCh,
						gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg,
						VES_CPB_ALMOST_FULL);
		}
		if( ui32BufIntReason & (1 << PDEC1_CPB_ALMOST_EMPTY) )
		{
			PDEC_HAL_CPB_AlmostEmpty(1);
		}
		if( ui32BufIntReason & (1 << PDEC1_AUB_ALMOST_FULL) )
		{
			PDEC_HAL_AUIB_AlmostFull(1);
		}
		if( ui32BufIntReason & (1 << PDEC1_AUB_ALMOST_EMPTY) )
		{
			PDEC_HAL_AUIB_AlmostEmpty(1);
		}
		if( ui32BufIntReason & (1 << PDEC2_CPB_ALMOST_FULL) )
		{
			ui8VesCh = _VES_GetVesCh(2);
			PDEC_HAL_CPB_AlmostFull(2);

			logm_warning(vdec_ves, "[VES%d][PDEC%d]CPB Almost Full - Buffer Level:%d, Status:%d, AUIB:%d \n"
					, ui8VesCh, 2, PDEC_HAL_CPB_GetBufferLevel(2)
					, PDEC_HAL_GetBufferStatus(2), VES_AUIB_NumActive(ui8VesCh) );

			if( gsVesDrv[ui8VesCh].Config.fpBufferUpdated )
				gsVesDrv[ui8VesCh].Config.fpBufferUpdated (ui8VesCh,
						gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg,
						VES_CPB_ALMOST_FULL);
		}
		if( ui32BufIntReason & (1 << PDEC2_CPB_ALMOST_EMPTY) )
		{
			PDEC_HAL_CPB_AlmostEmpty(2);
		}
		if( ui32BufIntReason & (1 << PDEC2_AUB_ALMOST_FULL) )
		{
			PDEC_HAL_AUIB_AlmostFull(2);
		}
		if( ui32BufIntReason & (1 << PDEC2_AUB_ALMOST_EMPTY) )
		{
			PDEC_HAL_AUIB_AlmostEmpty(2);
		}
		// handle feed timer
		if( ui32BufIntReason & (1 << LQC0_ALMOST_EMPTY) )
		{
			STC_TIMER_ISR(0);
		}

		ret = IRQ_HANDLED;
	}

	// pdec au detect
	if( ui32IntReason & (1 << PDEC0_AU_DETECT) )
	{
		_VES_ISR_PIC_Detected(_VES_GetVesCh(0));
		TOP_HAL_ClearIntrMsk(1 << PDEC0_AU_DETECT);

		ret = IRQ_HANDLED;
	}
	if( ui32IntReason & (1 << PDEC1_AU_DETECT) )
	{
		_VES_ISR_PIC_Detected(_VES_GetVesCh(1));
		TOP_HAL_ClearIntrMsk(1 << PDEC1_AU_DETECT);

		ret = IRQ_HANDLED;
	}
	if( ui32IntReason & (1 << PDEC2_AU_DETECT) )
	{
		_VES_ISR_PIC_Detected(_VES_GetVesCh(2));
		TOP_HAL_ClearIntrMsk(1 << PDEC2_AU_DETECT);

		ret = IRQ_HANDLED;
	}

	if( ui32IntReason & (1 <<PDEC_SRST_DONE) ) {
		logm_debug(vdec_ves,  "[VES] Intr PDEC_SRST_DONE\n");
		TOP_HAL_ClearIntrMsk(1 << PDEC_SRST_DONE);
		ret = IRQ_HANDLED;
	}

	return ret;
}
/**
 ********************************************************************************
 * @brief
 * reset & ready pdec hardware
 * @remarks
 * first, if au_detected enabled, disable it.
 * second, disable pdec input and pdec hal, and reset pdec hal.
 * third, set codec to pdec, enable pdec input and enable au detected.
 * if channel is playing, enable pdec hal.
 * else if channel is paused, disable pdec hal.
 * @par requirements:
 * int, int, E_VES_SRC_T
 * @param
 * channel number, codec type, pdec source(sdec) channel number
 * @return
 * NONE
 ********************************************************************************
 */
static void _PDEC_HW_Reset(UINT8 ui8VesCh, E_PDEC_CODEC_TYPE_T eCodecType_Config, E_VES_SRC_T eVesSrcCh)
{
	if( eVesSrcCh >= VES_SRC_TVP )
	{
		logm_error(vdec_ves, "[VES%d][Err] Channel Number \n", ui8VesCh);
		return;
	}
	if( gsVesDrv[ui8VesCh].State < VES_READY ) {
		logm_error(vdec_ves, "[VES%d-%d][Err] Current State \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
		return;
	}

	logm_noti(vdec_ves, "[VES%d] Codec:%d, PDEC Ch:%d \n", ui8VesCh, eCodecType_Config, eVesSrcCh);

	if( TOP_HAL_IsIntrEnable(PDEC0_AU_DETECT+eVesSrcCh) )
	{
		TOP_HAL_DisableIntr(PDEC0_AU_DETECT+eVesSrcCh);
		TOP_HAL_ClearIntr(PDEC0_AU_DETECT+eVesSrcCh);
	}

	TOP_HAL_DisablePdecInput(eVesSrcCh);
	PDEC_HAL_Disable(eVesSrcCh);

	PDEC_HAL_Reset(eVesSrcCh);

	VES_AUIB_Reset(ui8VesCh);
	VES_CPB_Reset(ui8VesCh);

	PDEC_HAL_SetVideoStandard(eVesSrcCh, eCodecType_Config);

	TOP_HAL_EnablePdecInput(eVesSrcCh);
	//                             pdec ch, te ch
	TOP_HAL_SetPdecInputSelection(eVesSrcCh, eVesSrcCh);

	TOP_HAL_ClearIntr(PDEC0_AU_DETECT+eVesSrcCh);
	TOP_HAL_EnableIntr(PDEC0_AU_DETECT+eVesSrcCh);

	if( gsVesDrv[ui8VesCh].State == VES_PLAY ) {
		if( !TOP_HAL_IsIntrEnable(PDEC0_AU_DETECT+eVesSrcCh) )
			logm_error(vdec_ves, "[VES%d-%d][Err] Disabled PIC_Detected Interrupt \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
		PDEC_HAL_Enable(eVesSrcCh);
	}
}

/**
 ********************************************************************************
 * @brief
 * initializing ves module
 * @remarks
 * initializing ves spinlock
 * registering requesti_irq for pdec
 * initializing ves channel's
 * @par requirements:
 * NONE
 * @param
 * NONE
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Init()
{
	UINT8	ui8VesCh;

	logm_trace (vdec_ves, "\n");

	spin_lock_init(&stVdecVesSpinlock);

	if( request_irq(VDEC_IRQ_NUM_PDEC, (irq_handler_t)_VES_IrqHandler,
				IRQF_SHARED, "ves", (void*)0xdecdec) ) {
		logm_error(vdec_ves, "[VES] request_irq failed \n");
	}
	else
	{
		logm_noti(vdec_ves, "[VES] request_irq success \n");
	}

	for( ui8VesCh = 0; ui8VesCh < VDEC_NUM_OF_CHANNEL; ui8VesCh++ )
	{
		gsVesDrv[ui8VesCh].State = VES_NULL;

		if( ui8VesCh < PDEC_NUM_OF_CHANNEL )
		{
			TOP_HAL_DisablePdecInput(ui8VesCh);
			PDEC_HAL_Disable(ui8VesCh);
		}
	}
}

/**
 ********************************************************************************
 * @brief
 * opening one ves channel
 * @remarks
 * opening submodules of ves, (ves_auib, ves_cpb)
 * setting global structure of ves
 * @par requirements:
 * E_VES_SRC_T, int, int, int, VES_EVENT_T, void *
 * @param
 * source channel(sdec) number, codec type, cpb base address, cpb size, callback function,
 * callback argument
 * @return
 * opened channel number
 ********************************************************************************
 */
UINT8 VES_Open( E_VES_SRC_T eVesSrcCh,
		UINT8 ui8CodecType_Config,
		UINT32 ui32CpbBufAddr, UINT32 ui32CpbBufSize,
		VES_EVENT_T updated, void *updated_arg)
{
	UINT8		ui8VesCh = 0xFF;
	BOOLEAN		bIsHwPath = FALSE;
	BOOLEAN		bFromTVP = FALSE;
	BOOLEAN		b512bytesAligned = FALSE;
	E_PDEC_CODEC_TYPE_T eCodecType;

	mutex_lock (&ves_lock);

	for( ui8VesCh = 0; ui8VesCh < VDEC_NUM_OF_CHANNEL; ui8VesCh++ )
	{
		if( gsVesDrv[ui8VesCh].State == VES_NULL )
			break;
	}
	if( ui8VesCh == VDEC_NUM_OF_CHANNEL )
	{
		logm_warning(vdec_ves, "[VES] Not Enough Channel - VES Src:%d, \n", eVesSrcCh);
		for( ui8VesCh = 0; ui8VesCh < VDEC_NUM_OF_CHANNEL; ui8VesCh++ )
			logm_warning(vdec_ves, "[VES%d] State: %d\n", ui8VesCh, gsVesDrv[ui8VesCh].State );

		mutex_unlock (&ves_lock);

		return 0xFF;
	}

	if( eVesSrcCh == VES_SRC_TVP )
	{
		bFromTVP = TRUE;
		logm_noti(vdec_ves, "[VES%d-%d] Trust Video Path \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
	}
	else
	{
		bIsHwPath = ( eVesSrcCh <= VES_SRC_SDEC2 ) ? TRUE : FALSE;
		b512bytesAligned = ( eVesSrcCh <= VES_SRC_SDEC2 ) ? TRUE : FALSE;
	}

	logm_noti(vdec_ves, "[VES%d-%d] PDEC ch:%d Codec:%d CPB Base:0x%8X Size:0x%6X \n"
			, ui8VesCh, gsVesDrv[ui8VesCh].State, eVesSrcCh, ui8CodecType_Config, ui32CpbBufAddr, ui32CpbBufSize );

	eCodecType = _VES_GetTranslateCodecType( ui8CodecType_Config );

	if( VES_AUIB_Open( ui8VesCh, eVesSrcCh, bIsHwPath, bFromTVP, eCodecType) == FALSE )
	{
		mutex_unlock (&ves_lock);

		return 0xFF;
	}

	if( VES_CPB_Open( ui8VesCh, eVesSrcCh, ui32CpbBufAddr, ui32CpbBufSize, bIsHwPath, bFromTVP, eCodecType) == FALSE )
	{
		VES_AUIB_Close(ui8VesCh);

		mutex_unlock (&ves_lock);

		return 0xFF;
	}

	gsVesDrv[ui8VesCh].State = VES_READY;
	gsVesDrv[ui8VesCh].Config.eVesSrcCh = eVesSrcCh;
	gsVesDrv[ui8VesCh].Config.eCodecType_Config = eCodecType;
	gsVesDrv[ui8VesCh].Config.fpBufferUpdated = updated;
	gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg = updated_arg;
	gsVesDrv[ui8VesCh].Config.b512bytesAligned = b512bytesAligned;
	gsVesDrv[ui8VesCh].Status.bSTCCDiscontinuity = FALSE;
	gsVesDrv[ui8VesCh].Status.ui32DetectedCount = 0;
	gsVesDrv[ui8VesCh].Status.ui32DetectedCountIRQ = 0;
	gsVesDrv[ui8VesCh].Status.bSTCCDiscontinuity = FALSE;

	gsVesDrv[ui8VesCh].Status.bFlushing = FALSE;

	if( bIsHwPath == TRUE )
	{
		_PDEC_HW_Reset(ui8VesCh, eCodecType, eVesSrcCh);
	}

	mutex_unlock (&ves_lock);

	return ui8VesCh;
}


/**
 ********************************************************************************
 * @brief
 * get private information
 * @remarks
 * get ves's private information to vdec top
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * private information
 ********************************************************************************
 */
void * VES_GetVdecPriv(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d] Channel Number\n", ui8VesCh);
		return NULL;
	}

	return gsVesDrv[ui8VesCh].Config.priv;
}

/**
 ********************************************************************************
 * @brief
 * closing one ves channel
 * @remarks
 * closing submodules of ves (ves_auib, ves_cpb)
 * if live, disable pdec & interrupt
 * initializing state, private
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Close(UINT8 ui8VesCh)
{
	mutex_lock (&ves_lock);

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d] Channel Number\n", ui8VesCh);

		goto finish;
	}

	logm_noti(vdec_ves, "[VES%d] state changes: %d -> %d \n", ui8VesCh, gsVesDrv[ui8VesCh].State, VES_NULL);

	if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 )
	{
		TOP_HAL_DisableIntr(PDEC0_AU_DETECT+gsVesDrv[ui8VesCh].Config.eVesSrcCh);
		TOP_HAL_ClearIntr(PDEC0_AU_DETECT+gsVesDrv[ui8VesCh].Config.eVesSrcCh);
		TOP_HAL_DisablePdecInput(gsVesDrv[ui8VesCh].Config.eVesSrcCh);
		PDEC_HAL_Disable(gsVesDrv[ui8VesCh].Config.eVesSrcCh);
	}

	gsVesDrv[ui8VesCh].State = VES_NULL;

	VES_AUIB_Close(ui8VesCh);
	VES_CPB_Close(ui8VesCh);

	gsVesDrv[ui8VesCh].Config.eVesSrcCh = 0xFF;
	gsVesDrv[ui8VesCh].Config.priv = NULL;
finish:
	mutex_unlock (&ves_lock);
}

void VES_Suspend (void)
{
}

void VES_Resume (void)
{
}

/**
 ********************************************************************************
 * @brief
 * reset one ves channel
 * @remarks
 * initializing global channel values
 * if live, reset pdec
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Reset(UINT8 ui8VesCh)
{
	mutex_lock (&ves_lock);

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d][Err] Channel Number\n", ui8VesCh);

		goto finish;
	}

	logm_noti(vdec_ves, "[VES%d-%d][DBG] \n", ui8VesCh, gsVesDrv[ui8VesCh].State);

	gsVesDrv[ui8VesCh].Status.bSTCCDiscontinuity = FALSE;
	gsVesDrv[ui8VesCh].Status.ui32DetectedCount = 0;
	gsVesDrv[ui8VesCh].Status.bFlushing = FALSE;

	if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 ) // live
	{
		_PDEC_HW_Reset(ui8VesCh, gsVesDrv[ui8VesCh].Config.eCodecType_Config, gsVesDrv[ui8VesCh].Config.eVesSrcCh);
	}
	else // fileplay, tvp
	{
		VES_AUIB_Reset(ui8VesCh);
		VES_CPB_Reset(ui8VesCh);
	}

finish:
	mutex_unlock (&ves_lock);
}

/**
 ********************************************************************************
 * @brief
 * start channel
 * @remarks
 * start one ves channel, change channel state to play
 * if live, enable pdec.
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Start(UINT8 ui8VesCh)
{
	mutex_lock (&ves_lock);

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d][Err] Channel Number\n", ui8VesCh);
		goto finish;
	}

	switch( gsVesDrv[ui8VesCh].State )
	{
		case VES_READY :
		case VES_PAUSE :
			gsVesDrv[ui8VesCh].State = VES_PLAY;

			if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 )
				PDEC_HAL_Enable(gsVesDrv[ui8VesCh].Config.eVesSrcCh);

			logm_noti(vdec_ves, "[VES%d-%d] Play \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
		case VES_PLAY :
			logm_noti(vdec_ves, "[VES%d-%d] Already Played\n", ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
		case VES_NULL :
		default :
			logm_error(vdec_ves, "[VES%d-%d] Current State\n", ui8VesCh, gsVesDrv[ui8VesCh].State);
	}

finish:
	mutex_unlock (&ves_lock);
}

/**
 ********************************************************************************
 * @brief
 * stopping one ves channel
 * @remarks
 * stopping ves before closing for avoiding deadlock with vdec top
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Stop(UINT8 ui8VesCh)
{
	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d] Channel Number\n", ui8VesCh);

		return;
	}

	logm_noti(vdec_ves, "[VES%d] state changes: %d -> %d \n", ui8VesCh, gsVesDrv[ui8VesCh].State, VES_PAUSE);

	switch( gsVesDrv[ui8VesCh].State )
	{
		case VES_PLAY :
			gsVesDrv[ui8VesCh].State = VES_PAUSE;

			if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 )
			{
				PDEC_HAL_Disable(gsVesDrv[ui8VesCh].Config.eVesSrcCh);

				flush_workqueue (vdec_wq);
			}
			break;
		case VES_NULL :
		case VES_READY :
		case VES_PAUSE :
		default :
			logm_error(vdec_ves, "[VES%d-%d][Err] Current State\n", ui8VesCh, gsVesDrv[ui8VesCh].State);
	}
}

/**
 ********************************************************************************
 * @brief
 * flushing one ves channel
 * @remarks
 * initializing some global values
 * flushing submodules of ves (ves_auib, ves_cpb)
 * @par requirements:
 * int
 * @param
 * ves channel number
 * @return
 * NONE
 ********************************************************************************
 */
void VES_Flush(UINT8 ui8VesCh)
{
	mutex_lock (&ves_lock);

	if( ui8VesCh >= VDEC_NUM_OF_CHANNEL )
	{
		logm_error(vdec_ves, "[VES%d] Channel Number\n", ui8VesCh);

		goto finish;
	}

	logm_info(vdec_ves, "[VES%d-%d] \n", ui8VesCh, gsVesDrv[ui8VesCh].State);

	switch( gsVesDrv[ui8VesCh].State )
	{
		case VES_PLAY :
		case VES_READY :
		case VES_PAUSE :
			gsVesDrv[ui8VesCh].Status.bSTCCDiscontinuity = FALSE;
			gsVesDrv[ui8VesCh].Status.ui32DetectedCount = 0;
			gsVesDrv[ui8VesCh].Status.bFlushing = TRUE;

			VES_AUIB_Flush(ui8VesCh);
			VES_CPB_Flush(ui8VesCh);
			break;
		case VES_NULL :
		default :
			logm_error(vdec_ves, "[VES%d-%d] Current State\n", ui8VesCh, gsVesDrv[ui8VesCh].State);
	}

finish:
	mutex_unlock (&ves_lock);
}


/**
 ********************************************************************************
 * @brief
 * update picture when one au parsed
 * @remarks
 * this function is core of ves.
 * historically, it was separated function for live path and fileplay path.
 * now it integrated on UpdatePicture. it called PIC_DETECT(work_queue) at live,
 * IO_UPDATE_BUFFER(system call) at fileplay.
 * UpdatePicture processes below jobs :
 * STEP1.  Get AU Information
 * STEP2.  Update CPB status
 * STEP3.  Check stc discontinuity (optional)
 * STEP4.  Update AUIB status
 * STEP5.  Call-back vdec top for kicking vdc
 * @par requirements:
 * int, E_VES_AU_T, int, int, function*, int, int, int, int
 * @param
 * ves channel number, au type, es addr in user space, es size, memory copy function,
 * unique id, timestamp, numerator of framerate, denominator of framerate
 * @return
 * TRUE, success
 * FALSE, failed
 ********************************************************************************
 */
BOOLEAN VES_UpdatePicture(UINT8 ui8VesCh,
		E_VES_AU_T eAuType,
		UINT32 ui32UserBuf,
		UINT32 ui32UserBufSize,
		UINT32 ui32UId,
		UINT64 ui64TimeStamp)
{
	UINT32		ui32CpbWrPhyAddr = 0x0;
	UINT32		ui32CpbWrPhyAddr_End = 0x0;
	UINT32		ui32AuSize_modified = 0x0;
	S_VES_AU_T	sVesAu = {0 };
	S_PDEC_AUI_T	sPdecAu = {0 };
	BOOLEAN		b512bytesAligned = FALSE;

	b512bytesAligned = gsVesDrv[ui8VesCh].Config.b512bytesAligned;

	switch( gsVesDrv[ui8VesCh].State )
	{
		case VES_READY : // FALL-THROUGH
		case VES_PAUSE :
			logm_warning(vdec_ves, "[VES%d] State:%d but update pic called \n",
					ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
		case VES_PLAY :
			// Check AUIB Overflow
			if( VES_AUIB_NumFree(ui8VesCh) < 1 )
			{
				logm_error(vdec_ves, "[VES%d] AUIB Overflow \n", ui8VesCh);
				return FALSE;
			}

			// STEP1: get au information
			if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 ) // live
			{
				while( ( PDEC_HAL_GetLatestDecodedAUI(gsVesDrv[ui8VesCh].Config.eVesSrcCh, &sPdecAu) ) != 0x0 )
				{
					// for debug logging
					if( gsVesDrv[ui8VesCh].Status.ui32DetectedCount == 0 )
					{
						logm_noti(vdec_ves, "[VES%d] Start UpdatePicture \n", ui8VesCh );
					}

					if (logm_enabled (vdec_ves, log_level_info))
					{
						UINT32 ui32Stc;
						SINT32 i32Diff;

						ui32Stc = PDEC_HAL_GetStc(gsVesDrv[ui8VesCh].Config.eVesSrcCh)&0xFFFFFFFF;
						i32Diff = (sPdecAu.ui32PTS - ui32Stc)/90;
						logm_info(vdec_ves, "[VES%d] AU Detected - Type:%d, PTS:%9u, DTS:%9u, STC:%9u(%dms), StcDis:%d, Remained:%8x, Start:0x%x Counted:%d \n"
								,ui8VesCh, sPdecAu.eAuType, sPdecAu.ui32PTS, sPdecAu.ui32DTS,
								ui32Stc, i32Diff,
								sPdecAu.bStccDiscontinuity, VES_CPB_GetRemainedBuffer(ui8VesCh),
								sPdecAu.ui32AuStartAddr, gsVesDrv[ui8VesCh].Status.ui32DetectedCount);
					}

					VES_CPB_UpdateWrPtr( ui8VesCh, PDEC_HAL_CPB_GetWrPtr(gsVesDrv[ui8VesCh].Config.eVesSrcCh));
					VES_AUIB_UpdateWrPtr (ui8VesCh, PDEC_HAL_AUIB_GetWrPtr(gsVesDrv[ui8VesCh].Config.eVesSrcCh));
					gsVesDrv[ui8VesCh].Status.ui32DetectedCount++;
				}


			}
			else // fileplay
			{
				// for debug logging
				if( gsVesDrv[ui8VesCh].Status.ui32DetectedCount == 0 )
				{
					logm_noti(vdec_ves, "[VES%d] Start UpdatePicture \n", ui8VesCh );
				}

				sVesAu.eAuType = eAuType;
				sVesAu.ui32PTS = UNKNOWN_PTS;
				sVesAu.ui64TimeStamp = ui64TimeStamp;
				sVesAu.b512bytesAligned = b512bytesAligned;

				if( gsVesDrv[ui8VesCh].Config.eVesSrcCh == VES_SRC_TVP ) // tvp
				{
					ui32CpbWrPhyAddr = ui32UserBuf;
					ui32CpbWrPhyAddr_End = ui32UserBuf + ui32UserBufSize;

					// Check the validation for the au address.
					if (ui32CpbWrPhyAddr < VES_CPB_GetBufferBaseAddr(ui8VesCh) ||
						ui32CpbWrPhyAddr > (VES_CPB_GetBufferBaseAddr(ui8VesCh) + VES_CPB_GetBufferSize(ui8VesCh)))
					{
						if (sVesAu.eAuType != AU_EOS)
						{
							logm_error(vdec_ves, "[VES%d] Invalid Write Addr:0x%X\n", ui8VesCh, ui32CpbWrPhyAddr);
							return FALSE;
						}
					}

					if( ui32CpbWrPhyAddr_End >=
							(VES_CPB_GetBufferBaseAddr(ui8VesCh) + VES_CPB_GetBufferSize(ui8VesCh) ))
						ui32CpbWrPhyAddr_End -= VES_CPB_GetBufferSize(ui8VesCh);
				}
				else
				{
					logm_debug(vdec_ves, "[VES%d] updated user buf addr:%08X, size:%d \n", ui8VesCh, ui32UserBuf, ui32UserBufSize);
					ui32CpbWrPhyAddr = VES_CPB_Write(ui8VesCh, eAuType, ui32UserBuf, ui32UserBufSize,
							b512bytesAligned, &ui32AuSize_modified, &ui32CpbWrPhyAddr_End );
					if( ui32CpbWrPhyAddr == 0x0 )
					{
						logm_error(vdec_ves, "[VES%d] VES_CPB_Write Failed \n", ui8VesCh);
						return FALSE;
					}
				}

				// STEP2: updating cpb information
				VES_CPB_UpdateWrPtr( ui8VesCh, ui32CpbWrPhyAddr_End );

				// STEP3: updating auib information
				sVesAu.ui32AuStartAddr = ui32CpbWrPhyAddr;
				sVesAu.ui32AuEndAddr = ui32CpbWrPhyAddr_End;
				if( b512bytesAligned ) sVesAu.ui32AuSize = ui32AuSize_modified;
				else sVesAu.ui32AuSize = ui32UserBufSize;

				if( eAuType == AU_SEQUENCE_HEADER )
				{
					logm_noti(vdec_ves, "[VES%d] Received Sequence Header(Size:%d) \n",
							ui8VesCh, ui32UserBufSize );
				}
				else
				{
					logm_info(vdec_ves, "[VES%d] Pic Updated - Type:%d, TS:%11llu, Start:0x%8x, End:0x%8x, Size:%8x, Remained:%8x, Auib:%d counted:%d \n"
							, ui8VesCh, eAuType, sVesAu.ui64TimeStamp,
							sVesAu.ui32AuStartAddr, sVesAu.ui32AuEndAddr,
							sVesAu.ui32AuSize,
							VES_CPB_GetRemainedBuffer(ui8VesCh),
							VES_AUIB_NumActive(ui8VesCh),
							gsVesDrv[ui8VesCh].Status.ui32DetectedCount);
				}

				VES_AUIB_Push(ui8VesCh, &sVesAu);
				gsVesDrv[ui8VesCh].Status.ui32DetectedCount++;
			}

			// STEP4: calling-back for kicking vdc
			if( gsVesDrv[ui8VesCh].Config.fpBufferUpdated )
				gsVesDrv[ui8VesCh].Config.fpBufferUpdated (ui8VesCh,
						gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg,
						VES_PIC_DETECTED);
			break;
		case VES_NULL :
		default :
			logm_error(vdec_ves, "[VES%d] State:%d \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
	}

	return TRUE;
}

#ifdef CPB_USE_NEW_UPDATE_BUFFER

/**
********************************************************************************
* @brief
* allocate cbp buffer
* @remarks
* allocate cbp buffer
* @param ui8VesCh ves channel number
* @param ui32Size cbp buffer size
* @return
* amount of buffer (bytes)
********************************************************************************
*/
UINT32 VES_AllocBuffer(UINT8 ui8VesCh, UINT32 ui32AuSize, CPB_MEM_BUF_T * pstBufFront, CPB_MEM_BUF_T *pstBufBack)
{
	BOOLEAN		b512bytesAligned = FALSE;

	b512bytesAligned = gsVesDrv[ui8VesCh].Config.b512bytesAligned;

	return VES_CPB_AllocBuffer(ui8VesCh, ui32AuSize, pstBufFront, pstBufBack, b512bytesAligned);
}

/**
 ********************************************************************************
 * @brief
 * update picture when one au parsed
 * @remarks
 * this function is core of ves.
 * historically, it was separated function for live path and fileplay path.
 * now it integrated on UpdatePicture. it called PIC_DETECT(work_queue) at live,
 * IO_UPDATE_BUFFER(system call) at fileplay.
 * UpdatePicture processes below jobs :
 * STEP1.  Get AU Information
 * STEP2.  Update CPB status
 * STEP3.  Check stc discontinuity (optional)
 * STEP4.  Update AUIB status
 * STEP5.  Call-back vdec top for kicking vdc
 * @par requirements:
 * int, E_VES_AU_T, int, int, function*, int, int, int, int
 * @param
 * ves channel number, au type, es addr in user space, es size, memory copy function,
 * unique id, timestamp, numerator of framerate, denominator of framerate
 * @return
 * TRUE, success
 * FALSE, failed
 ********************************************************************************
 */
BOOLEAN VES_UpdatePictureEx(UINT8 ui8VesCh,
		E_VES_AU_T eAuType,
		CPB_MEM_BUF_T *pstPhysBuf0,
		CPB_MEM_BUF_T *pstPhysBuf1,
		UINT32 ui32UId,
		UINT64 ui64TimeStamp)
{
	UINT32		ui32CpbWrPhyAddr = 0x0;
	UINT32		ui32CpbWrPhyAddr_End = 0x0;
	UINT32		ui32AuSize_modified = 0x0;
	S_VES_AU_T	sVesAu = {0 };
	S_PDEC_AUI_T	sPdecAu = {0 };
	BOOLEAN		b512bytesAligned = FALSE;

	UINT32		ui32BufSize = 0;

	if ( pstPhysBuf0 == NULL || pstPhysBuf1 == NULL )
	{
		logm_error(vdec_ves, "[VES%d] buffer arguments are NULL\n", ui8VesCh);
			return FALSE;
	}

	b512bytesAligned = gsVesDrv[ui8VesCh].Config.b512bytesAligned;

	switch( gsVesDrv[ui8VesCh].State )
	{
		case VES_READY : // FALL-THROUGH
		case VES_PAUSE :
			logm_warning(vdec_ves, "[VES%d] State:%d but update pic called \n",
					ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
		case VES_PLAY :
			// Check AUIB Overflow
			if( VES_AUIB_NumFree(ui8VesCh) < 1 )
			{
				logm_error(vdec_ves, "[VES%d] AUIB Overflow \n", ui8VesCh);
				return FALSE;
			}

			// STEP1: get au information
			if( gsVesDrv[ui8VesCh].Config.eVesSrcCh <= VES_SRC_SDEC2 ) // live
			{
				logm_error(vdec_ves, "[VES%d] Not supported for LIVE\n", ui8VesCh);
				return FALSE;
			}
			else // fileplay
			{
                // for debug logging
                if( gsVesDrv[ui8VesCh].Status.ui32DetectedCount == 0 )
                {
                    logm_noti(vdec_ves, "[VES%d] Start UpdatePicture \n", ui8VesCh );
                }

				sVesAu.eAuType = eAuType;
				sVesAu.ui32PTS = UNKNOWN_PTS;
				sVesAu.ui64TimeStamp = ui64TimeStamp;
				sVesAu.b512bytesAligned = b512bytesAligned;

				if( gsVesDrv[ui8VesCh].Config.eVesSrcCh == VES_SRC_TVP ) // tvp
				{
					ui32CpbWrPhyAddr = pstPhysBuf0->addr;
					ui32CpbWrPhyAddr_End = pstPhysBuf0->addr + pstPhysBuf0->length;
					if (ui32CpbWrPhyAddr_End >=
						(VES_CPB_GetBufferBaseAddr(ui8VesCh) + VES_CPB_GetBufferSize(ui8VesCh)))
						ui32CpbWrPhyAddr_End -= VES_CPB_GetBufferSize(ui8VesCh);

					ui32BufSize = pstPhysBuf0->length;
					//ui32AuSize_modified = pstPhysBuf0->length;
				}
				else
				{
					//logm_debug(vdec_ves, "[VES%d] updated user buf addr:%08X, size:%d \n", ui8VesCh, ui32UserBuf, ui32UserBufSize);

					ui32CpbWrPhyAddr = pstPhysBuf0->addr;
					ui32AuSize_modified = 0;

					if ( pstPhysBuf1->addr == 0 || pstPhysBuf1->length == 0 )
					{
						ui32CpbWrPhyAddr_End = pstPhysBuf0->addr + pstPhysBuf0->length;
						ui32BufSize = pstPhysBuf0->length;
						ui32AuSize_modified = ui32BufSize;
					}
					else
					{
						ui32CpbWrPhyAddr_End = pstPhysBuf1->addr + pstPhysBuf1->length;
						ui32BufSize = pstPhysBuf0->length + pstPhysBuf1->length;
						ui32AuSize_modified = ui32BufSize;
					}

					if( ui32CpbWrPhyAddr == 0x0 )
					{
						logm_error(vdec_ves, "[VES%d] VES_CPB_Write Failed \n", ui8VesCh);
						return FALSE;
					}
				}

				// STEP2: updating cpb information
				VES_CPB_UpdateWrPtr( ui8VesCh, ui32CpbWrPhyAddr_End );

				// STEP3: updating auib information
				sVesAu.ui32AuStartAddr = ui32CpbWrPhyAddr;
				sVesAu.ui32AuEndAddr = ui32CpbWrPhyAddr_End;
				if( b512bytesAligned ) sVesAu.ui32AuSize = ui32AuSize_modified;
				else sVesAu.ui32AuSize = ui32BufSize;

				// TODO: Check b512bytesAligned

				if( eAuType == AU_SEQUENCE_HEADER )
				{
					logm_noti(vdec_ves, "[VES%d] Received Sequence Header(Size:%d) \n",
							ui8VesCh, ui32BufSize );
				}
				else
				{
					logm_info(vdec_ves, "[VES%d] Pic Updated - Type:%d, TS:%11llu, Start:0x%8x, End:0x%8x, Size:%8x, Remained:%8x, Auib:%d counted:%d \n"
							, ui8VesCh, eAuType, sVesAu.ui64TimeStamp,
							sVesAu.ui32AuStartAddr, sVesAu.ui32AuEndAddr,
							sVesAu.ui32AuSize,
							VES_CPB_GetRemainedBuffer(ui8VesCh),
							VES_AUIB_NumActive(ui8VesCh),
							gsVesDrv[ui8VesCh].Status.ui32DetectedCount);
				}

				VES_AUIB_Push(ui8VesCh, &sVesAu);
				gsVesDrv[ui8VesCh].Status.ui32DetectedCount++;

			}

			// STEP4: calling-back for kicking vdc
			if( gsVesDrv[ui8VesCh].Config.fpBufferUpdated )
				gsVesDrv[ui8VesCh].Config.fpBufferUpdated (ui8VesCh,
						gsVesDrv[ui8VesCh].Config.pBufferUpdatedArg,
						VES_PIC_DETECTED);
			break;
		case VES_NULL :
		default :
			logm_error(vdec_ves, "[VES%d] State:%d \n", ui8VesCh, gsVesDrv[ui8VesCh].State);
			break;
	}

	return TRUE;
}

#endif

