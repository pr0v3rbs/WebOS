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
 *  main driver implementation for pvr device.
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		murugan.d (murugan.d@lge.com)
 *  author		modified by ki beom kim (kibeom.kim@lge.com)
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kthread.h>

#include "pvr_impl.h"
#include "pvr_drv.h"
#include "pvr_reg.h"
#include "pvr_reg_ctrl.h"
#include "pvr_isr.h"
#include "pvr_core.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PVR_CORE_TRACE_PARAM_ENTER(ctrl,fmt,args...) \
		PVR_PRINT_TRACE("<== PVR_CORE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)
#define PVR_CORE_TRACE_PARAM_EXIT(ctrl,fmt,args...) \
		PVR_PRINT_TRACE("==> PVR_CORE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)

#define PVR_CORE_TRACE_EX_PARAM_ENTER(ctrl,fmt,args...) \
		PVR_PRINT_TRACE_EX("<== PVR_CORE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)
#define PVR_CORE_TRACE_EX_PARAM_EXIT(ctrl,fmt,args...) \
		PVR_PRINT_TRACE_EX("==> PVR_CORE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)


#define PVR_CORE_TRACE_ENTER(ctrl)		PVR_CORE_TRACE_PARAM_ENTER(ctrl,"")
#define PVR_CORE_TRACE_EXIT(ctrl)		PVR_CORE_TRACE_PARAM_EXIT(ctrl,"")

#define PVR_CORE_TRACE_EX_ENTER(ctrl)	PVR_CORE_TRACE_EX_PARAM_ENTER(ctrl,"")
#define PVR_CORE_TRACE_Ex_EXIT(ctrl)	PVR_CORE_TRACE_EX_PARAM_EXIT(ctrl,"")


#define PVR_UP_LOCK(ctrl)			mutex_lock(&ctrl->up_lock)
#define PVR_UP_UNLOCK(ctrl)			mutex_unlock(&ctrl->up_lock)

/*----------------------------------------------------------------------------------------
 *	function prototypes
 *----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Global variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int TimeStampCopyThread(void *data);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static PVR_CTRL_T* pvr_ctrl[LX_PVR_CH_NUM];
static struct task_struct *timestamp_copy_thread;



/*========================================================================================
	Implementation Group
========================================================================================*/

PVR_CTRL_T* PVR_GetCtrl(LX_PVR_CH_T ch)
{
	return (ch >= LX_PVR_CH_NUM) ? NULL : pvr_ctrl[ch];
}

int PVR_InitDevice(void)
{
	LX_PVR_CH_T ch;
	PVR_CTRL_T *ctrl;
	LX_PVR_CFG_T* cfg = PVR_CFG_GetConfig();
	int rc;

	for(ch=LX_PVR_CH_A; ch<LX_PVR_CH_NUM; ch++)
	{
		ctrl = (PVR_CTRL_T*)OS_KMalloc(sizeof(PVR_CTRL_T));
		PVR_CHECK_ERROR(ctrl == NULL, return RET_OUT_OF_MEMORY, "can't alloc");

		memset(ctrl, 0, sizeof(PVR_CTRL_T));

		ctrl->ch = ch;
		mutex_init(&ctrl->up_lock);

		pvr_ctrl[ch] = ctrl;
	}

	rc = PVR_REG_Init();
	PVR_CHECK_ERROR(rc != 0, return rc, "reg init fail");

	rc = PVR_ISR_Init();
	PVR_CHECK_ERROR(rc != 0, return rc, "isr init fail");

	if(cfg->fixup_timestamp)
	{
		timestamp_copy_thread = kthread_create(TimeStampCopyThread, NULL, "timestampcopy");
		PVR_CHECK_ERROR(timestamp_copy_thread == NULL, return RET_ERROR, "can't create thread");

		wake_up_process(timestamp_copy_thread);
		PVR_PRINT_NOTI("Start TimeStampCopy Thread...\n");
	}

	return RET_OK;
}

void PVR_CleanupDevice(void)
{
	LX_PVR_CH_T ch;

	PVR_ISR_Cleanup();

	for(ch=LX_PVR_CH_A; ch<LX_PVR_CH_NUM; ch++)
	{
		OS_KFree(pvr_ctrl[ch]);
	}
}


void PVR_SetDefaultMemoryMap (PVR_CTRL_T *ctrl)
{
	LX_MEMCFG_T *mem_cfg = PVR_CFG_GetMemConfig(ctrl->ch);
	PVR_BUF_T *buf = &ctrl->buf;

	buf->dn.base	= mem_cfg[LX_PVR_MEM_DN].base;
	buf->dn.end		= mem_cfg[LX_PVR_MEM_DN].base;
	buf->dn.size	= 0;
	buf->dn.rptr	= mem_cfg[LX_PVR_MEM_DN].base;
	buf->dn.wptr	= mem_cfg[LX_PVR_MEM_DN].base;

	buf->up.base	= mem_cfg[LX_PVR_MEM_UP].base;
	buf->up.end		= mem_cfg[LX_PVR_MEM_UP].base;
	buf->up.size	= 0;
	buf->up.rptr	= mem_cfg[LX_PVR_MEM_UP].base;
	buf->up.wptr	= mem_cfg[LX_PVR_MEM_UP].base;

	/* UP Buffer base */
	DVR_UP_SetBufBoundReg(ctrl->ch, buf->up.base, buf->up.end );

	/* DN Buffer base */
	DVR_DN_SetBufBoundReg(ctrl->ch, buf->dn.base, buf->dn.end );

}



/**
 * PVR_InitModule
 * note:
 * @def PVR_IOW_INIT
 * Initialize pvr module.
 * Set PVR to default status.
 */
int PVR_InitModule(PVR_CTRL_T *ctrl)
{
	LX_PVR_CFG_T* cfg;

	PVR_CORE_TRACE_ENTER(ctrl);

	cfg = PVR_CFG_GetConfig();

	/* Initialize the registers with default value */
	if(ctrl->ch < cfg->num_updev)
	{
		DVR_UP_ResetBlock (ctrl->ch);
		DVR_UP_EnableReg(ctrl->ch, FALSE);
		DVR_UP_SetInputTSMode(ctrl->ch, 192);
		DVR_UP_SetNumOfSyncError(ctrl->ch, 0);
		DVR_UP_SetTimeStampClkSource(ctrl->ch, 1);
		DVR_UP_SetWaitCycle(ctrl->ch, 0x108B);	/* 8MBytes/sec */

		DVR_UP_SetTimeStampCheck(ctrl->ch, TRUE);
		DVR_UP_ChangePlaymode(ctrl->ch, LX_PVR_UPMODE_NORMAL);
		ctrl->state.up_mode 		= LX_PVR_UPMODE_NORMAL;
		ctrl->state.use_timestamp	= TRUE;
	}
	ctrl->ui8PacketLen = 192;

	if(ctrl->ch < cfg->num_dndev)
	{
		DVR_DN_ResetBlock (ctrl->ch);
		DVR_DN_EnableReg(ctrl->ch, FALSE);
		DVR_PIE_ResetBlock (ctrl->ch);

		PVR_SetDownloadUnitBufPktCnt(ctrl, PVR_DN_UNIT_BUF_PKT_CNT);

		// Set PIE_MODE Register
		DVR_PIE_SetModeReg(ctrl->ch, PIE_MODE_ISR_BOTH);
	}

	/* Initialize default buffer base addresses */
	PVR_SetDefaultMemoryMap(ctrl);

	ctrl->state.eUpState = LX_PVR_UP_STATE_IDLE;

	return RET_OK;
}

/**
 * PVR_Terminate
 * note:
 * @def PVR_IOW_TERMINATE
 * Terminate pvr module.
 */

int	PVR_Terminate ( void )
{
	/**/
	return RET_OK;
}

int PVR_GetHwInfo(LX_PVR_HW_INFO_T *info)
{
	LX_PVR_CFG_T* cfg = PVR_CFG_GetConfig();

	info->num_dndev = cfg->num_dndev;
	info->num_updev = cfg->num_updev;
	info->mem_shared = cfg->mem_shared;

	return RET_OK;
}

/**
 * PVR_StartDownload
 * note:
 * @def PVR_IOW_DN_START
 * Start PVR download for specified PVR channel
 */
int PVR_StartDownload (PVR_CTRL_T *ctrl)
{
	UINT32 rtrn = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if (ctrl->state.eDnState != LX_PVR_DN_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_DN> One recording already active in [%d] channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eDnState );
	#if 0
		/* Stop the previous download and restart again, but the problem has to be fixed in middleware !!!*/
		PVR_StopDownload(ctrl);
		/*return -1;*/
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_DN> Restarting download[%d]channel !!!!\n",
				ctrl->ch);
	#else
		return -1;
	#endif
	}
	//Reset download block
	DVR_DN_ResetBlock(ctrl->ch);

	/* Reset the error variables */
	ctrl->stats.dn_unitbuf	= 0;
	ctrl->stats.dn_overflow	= 0;
	ctrl->buf_state.dn		= LX_PVR_BUF_STAT_READY;

	ctrl->unit_buf.curr		= 0;
	ctrl->unit_buf.buf_num	= 0;

	ctrl->dn.warn_bufsize	= ctrl->buf.dn.size * PVR_DN_BUF_WARNIG_PERCENT / 100;
	ctrl->dn.warn_interval	= PVR_DN_BUF_WARNIG_INTERVAL;
	ctrl->dn.warn_time		= 0;

	ctrl->dn.errmsg_enable	= 1;

	//Configure base address & write pointer
	DVR_DN_SetBufBoundReg(ctrl->ch, ctrl->buf.dn.base, ctrl->buf.dn.end );
	ctrl->buf.dn.wptr = ctrl->buf.dn.base;
	ctrl->buf.dn.rptr = ctrl->buf.dn.base;

	//Configure write pointer
	DVR_DN_SetWritePtrReg(ctrl->ch, ctrl->buf.dn.base );

	DVR_DN_EnableINTReg(ctrl->ch, TRUE);
	DVR_DN_PauseReg(ctrl->ch, FALSE);
	DVR_DN_EnableReg(ctrl->ch, TRUE);

	ctrl->state.eDnState = LX_PVR_DN_STATE_RECORD;

#if 0	/* stream 정보 관련 하여 사용되지 않아 파일 분리 함. 제대로 동작 및 사용되지 않아 막아 둠 */
	/* define function in pvr_parser.c to initialize below variables */
	gStreamInfo.bFoundSeqSPS = FALSE;
	gStreamInfo.bitRate = 0;
	gStreamInfo.frRate = LX_PVR_FR_UNKNOWN;
	gPictureNum = 0;
	gRefTimeStamp = 0;
	gbValidSPS = FALSE;
#endif


	return rtrn;
}

/**
 * PVR_StopDownload
 * note:
 * @def PVR_IOW_DN_STOP
 * Stop PVR download for specified PVR channel
 */
int PVR_StopDownload (PVR_CTRL_T *ctrl)
{
	UINT32 rtrn = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if ( ctrl->state.eDnState == LX_PVR_DN_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_DN> Recording not active in [%d]channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eDnState );
		//rtrn = -1;
		/* return -1; */
		/* Still go down and clear all the registers and state */
	}

	DVR_DN_EnableReg(ctrl->ch, FALSE);
	DVR_DN_EnableINTReg(ctrl->ch, FALSE);

	//Also clear the PIE pid register
	DVR_DN_SetPIDReg(ctrl->ch, 0x1FFF);

	ctrl->state.eDnState = LX_PVR_DN_STATE_IDLE;
	ctrl->state.ePieState = LX_PVR_PIE_STATE_IDLE;


	return rtrn;
}

/**
 * PVR_StopDownloadSDT
 * note:
 * @def PVR_IOW_DN_STOP
 * Stop PVR download for specified PVR channel
 * jinhwan.bae 20140124, to support H13 UHD DVB download and SDT
 */
int PVR_StopDownloadSDT (PVR_CTRL_T *ctrl)
{
	UINT32 rtrn = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		/* just return, not available from M14B0 */
		return rtrn;
	}

	if ( ctrl->state.eDnState == LX_PVR_DN_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_DN> Recording not active in [%d]channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eDnState );
		//rtrn = -1;
		/* return -1; */
		/* Still go down and clear all the registers and state */
	}

	DVR_DN_EnableReg(ctrl->ch, FALSE);
	DVR_DN_EnableINTReg(ctrl->ch, FALSE);

#if 0 // jinhwan.bae here is differenct points from normal stop download - index related one
	//Disable PIE interrupt also
	rtrn |= DVR_PIE_EnableINTReg(ch, FALSE);

	//Also clear the PIE pid register
	DVR_DN_SetPIDReg(ch, 0x1FFF);
#endif

	// jinhwan.bae PIE block reset needed
	// if not, PIE module is not working after SDT cancel 20140124
	DVR_PIE_ResetBlock(ctrl->ch);

	ctrl->state.eDnState = LX_PVR_DN_STATE_IDLE;
//	ctrl->state.ePieState = LX_PVR_PIE_STATE_IDLE;


	return rtrn;
}


/**
 * PVR_SetDownloadbuffer
 * note:
 * @def PVR_IOW_DN_SET_BUF
 * Set the buffer base, end, read/write pointers for download buffer
 */
int PVR_SetDownloadBuffer (PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize)
{
	UINT32 rtrn = RET_OK;
	UINT32 ptrBase;
	UINT32 ptrEnd;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "uiBufAddr=0x%08x,uiBufSize=%d", uiBufAddr, uiBufSize);

	ptrBase	= uiBufAddr;
	ptrEnd	= uiBufAddr + uiBufSize;

	/* Set DN_BUF_SPTR, EPTR, WPTR, not RPTR, actually no RPTR in Register Map */
	DVR_DN_SetBufBoundReg(ctrl->ch, ptrBase, ptrEnd);
	DVR_DN_SetWritePtrReg(ctrl->ch, ptrBase );

	/* Set Buffer DB with Physical Address */
	ctrl->buf.dn.base	= ptrBase;
	ctrl->buf.dn.end 	= ptrEnd;
	ctrl->buf.dn.size	= uiBufSize;

	ctrl->buf.dn.rptr 	= ptrBase;
	ctrl->buf.dn.wptr 	= ptrBase;

	/* Configure unit buffer */
	ctrl->unit_buf.size		= ctrl->unit_buf.pkt_cnt * 192;
	ctrl->unit_buf.total	= ctrl->buf.dn.size / ctrl->unit_buf.size;

	return rtrn;
}


/**
 * PVR_SetDownloadReadAddr
 * note:
 * @def PVR_IOW_DN_SET_READ_ADDR
 * Set the buffer read pointer for download buffer. This is used for overflow detection
 */
int PVR_SetDownloadReadAddr(PVR_CTRL_T *ctrl, UINT32 ui32ReadPtr)
{
	UINT32 rtrn = RET_OK;

	PVR_CORE_TRACE_EX_PARAM_ENTER(ctrl, "ui32ReadPtr=0x%08x", ui32ReadPtr);

	ctrl->buf.dn.rptr = ui32ReadPtr;

	return rtrn;
}

int PVR_GetDownloadWritePtr(PVR_CTRL_T *ctrl, UINT32 *ptr)
{
	UINT32 rtrn = RET_OK;
	UINT32 wptr, rptr, offset;
	UINT32 dn_size;

	/* Caution! read once wptr & rptr (updated other TASK/ISR) */
	wptr = ctrl->buf.dn.wptr;
	rptr = ctrl->buf.dn.rptr;

	if(ctrl->buf.dn.size == 0)
	{
		*ptr = wptr;
		return RET_ERROR;
	}

	/*
	 * If the Picture Index exists in the end of the unit buffer,
	 * it can be detected when next unit buffer interrupt occured.
	 * So, we have to return the write ptr minus unit buffer size
	 */
	dn_size = (wptr + ctrl->buf.dn.size - rptr) % ctrl->buf.dn.size;
	if(dn_size >= ctrl->unit_buf.size)
	{
		offset = wptr - ctrl->buf.dn.base;
		*ptr = ctrl->buf.dn.base + (offset + ctrl->buf.dn.size - ctrl->unit_buf.size) % ctrl->buf.dn.size;
	}
	else
	{
		*ptr = rptr;		/* EMPTY */
	}

	PVR_CORE_TRACE_EX_PARAM_EXIT(ctrl, "*ptr=0x%08x", *ptr);

	return rtrn;
}



/**
 * PVR_GetUploadReadAddr
 * note:
 * @def PVR_IOR_UP_GET_READ_ADD
 * Get the buffer write pointer for upload buffer
 */
int PVR_GetUploadReadAddr (PVR_CTRL_T *ctrl, UINT32 *pui32WritePtr, UINT32 *pui32ReadPtr)
{
	UINT32 rtrn = RET_OK;

	DVR_UP_GetPointersReg(
			ctrl->ch,
			pui32WritePtr,
			pui32ReadPtr );

	PVR_CORE_TRACE_EX_PARAM_EXIT(ctrl, "*pui32WritePtr=0x%08x,*pui32ReadPtr=0x%08x", *pui32WritePtr, *pui32ReadPtr);

	return rtrn;
}



/**
 * PVR_SetUploadbuffer
 * note:
 * @def PVR_IOW_UP_SET_BUF
 * Set the buffer base, end, read/write pointers for upload buffer
 */
int PVR_SetUploadbuffer (PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize)
{
	UINT32 rtrn = RET_OK;
	UINT32 ptrBase;
	UINT32 ptrEnd;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "uiBufAddr=0x%08x,uiBufSize=%d", uiBufAddr, uiBufSize);

	ptrBase = uiBufAddr;
	ptrEnd = uiBufAddr + uiBufSize;

	//Set buffer bound based on start and end
	DVR_UP_SetBufBoundReg(ctrl->ch, ptrBase, ptrEnd);

	//set the write pointer to the start of buffer
	DVR_UP_SetWritePtrReg(ctrl->ch, ptrBase);

	//Set the read pointer to the start of buffer
	DVR_UP_SetReadPtrReg(ctrl->ch, ptrBase);

	DVR_UP_ResetBlock(ctrl->ch);	//Reset the upload block for the new bound to take effect

	/* Memorize the buffer base, end and read/write pointers */
	ctrl->buf.up.base	= ptrBase;
	ctrl->buf.up.end	= ptrEnd;
	ctrl->buf.up.size	= uiBufSize;

	ctrl->buf.up.rptr	= ptrBase;
	ctrl->buf.up.wptr	= ptrBase;

	return rtrn;
}

/**
 * PVR_RestartUpload
 * note:
 * @def PVR_IOW_UP_RESTART
 * Set the buffer base, end, read/write pointers for upload buffer
 */
int PVR_RestartUpload(PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize)
{
	int rtrn = RET_OK;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "uiBufAddr=0x%08x,uiBufSize=%d", uiBufAddr, uiBufSize);

	PVR_UP_LOCK(ctrl);

	/* Disable the Upload, so that the readptr doesn't move */
	DVR_UP_EnableReg(ctrl->ch, FALSE);

	//To reset the buffer read and write pointers
	PVR_SetUploadbuffer (ctrl, uiBufAddr, uiBufSize);

	//After resetting this pointers, enable the upload during the next data transfer
	ctrl->ui8UploadFirstTime = TRUE;

	PVR_UP_UNLOCK(ctrl);

	return rtrn;
}
/*
 * PVR_ReSetUpload
 * note:
 * @def PVR_IOW_UP_RESET
 * Reset the buffer base, end, read/write pointers for upload buffer
 */
int PVR_ReSetUpload (PVR_CTRL_T *ctrl)
{
	int rtrn = RET_OK;
	UINT32 ptrBase;

	PVR_CORE_TRACE_ENTER(ctrl);

	ptrBase = ctrl->buf.up.base;

	PVR_UP_LOCK(ctrl);

	/* Murugan-18/11/2010 - Added the reset block code to reset the read and write pointers to the base */
	DVR_UP_ResetBlock(ctrl->ch);	//Reset the upload block for the new bound to take effect

	/* Disable the Upload, so that the readptr doesn't move */
	DVR_UP_EnableReg(ctrl->ch, FALSE);

	//set the write pointer to the start of buffer
	DVR_UP_SetWritePtrReg(ctrl->ch, ptrBase);

	//Set the read pointer to the start of buffer
	DVR_UP_SetReadPtrReg(ctrl->ch, ptrBase);

	//After resetting this pointers, enable the upload during the next data transfer
	ctrl->ui8UploadFirstTime = TRUE;

	PVR_UP_UNLOCK(ctrl);

	return rtrn;
}

static int CopyUploadTimeStamp(PVR_CTRL_T *ctrl)
{
	if(ctrl->state.use_timestamp &&	ctrl->ui8PacketLen == 192)
	{
		DVR_UP_CopyTimeStamp(ctrl->ch);
	}

	return RET_OK;
}

/**
 * PVR_UpUploadbuffer
 * note:
 * @def PVR_IOW_UP_UPLOAD_BUFFER
 * Set the new write pointers for upload buffer
 */
int PVR_UpUploadbuffer (PVR_CTRL_T *ctrl, UINT32 wptr)
{
	LX_PVR_CFG_T* cfg = PVR_CFG_GetConfig();

	PVR_CORE_TRACE_EX_PARAM_ENTER(ctrl, "wptr=0x%08x", wptr);

	if(wptr < ctrl->buf.up.base || wptr >= ctrl->buf.up.end)
	{
		PVR_PRINT_ERROR("invalid addr:0x%08x, range:0x%08x~0x%08x",
			wptr, ctrl->buf.up.base, ctrl->buf.up.end);
		return RET_ERROR;
	}

	PVR_UP_LOCK(ctrl);

	//set the write pointer to the start of buffer
	DVR_UP_SetWritePtrReg(ctrl->ch, wptr);

	ctrl->buf.up.wptr = wptr;

	/* Clear the IACK register to acknowledge the overlap error due to under flow */
	if(ctrl->err_stat.up_overlap)
	{
		CopyUploadTimeStamp(ctrl);	/* copy time stamp before resume */

		ctrl->err_stat.up_overlap = 0;	// clear value before clear H/W
		DVR_UP_SetIntrAck(ctrl->ch, UP_CONF_IACK_ERR_IACK);	//Clear only err stat
		/* Maintain the underflow status for the middleware to know */
		ctrl->buf_state.up = LX_PVR_BUF_STAT_EMPTY;
	}
	else
	{
		//Clear the state back to normal if no error
		ctrl->buf_state.up = LX_PVR_BUF_STAT_READY;
	}

	if ( ctrl->ui8UploadFirstTime )
	{
		if(ctrl->ui8PacketLen == 192 && cfg->fixup_timestamp == 0)
			DVR_UP_AutoTimeStampCopyReg(ctrl->ch, TRUE);
		else
			DVR_UP_AutoTimeStampCopyReg(ctrl->ch, FALSE);

		CopyUploadTimeStamp(ctrl);	/* copy time stamp before resume */

		DVR_UP_EnableReg(ctrl->ch, TRUE);
		ctrl->ui8UploadFirstTime = FALSE;
	}

	PVR_UP_UNLOCK(ctrl);

	return RET_OK;
}


static void WaitUploadPauseState(PVR_CTRL_T *ctrl)
{
	UINT32 max_time;
	UINT32 state;

#define MAX_PAUSE_WAIT_TIME	50	/* ms */

	max_time = jiffies_to_msecs(jiffies) + MAX_PAUSE_WAIT_TIME;
	do {
		state = DVR_UP_GetStat0(ctrl->ch) & 0x1F;
		//PVR_PRINT_WARNING("STAT0:0x%08x", state);
		if(state == 0x1D) break;		/* 0x1D = PAUSE STATE in PVR STATE MACHINE */
		if(state == 0xF)				/* 0xF = TIMESTAMP COUNT WAIT STATE */
		{
			DVR_UP_SetTimeStampCheck(ctrl->ch, FALSE);
			OS_UsecDelay(10);
		}

		if(max_time < jiffies_to_msecs(jiffies))
		{
			PVR_PRINT_ERROR("timeout : can't change to pause state");
			break;
		}
	} while(1);
	DVR_UP_SetTimeStampCheck(ctrl->ch, TRUE);

}

/**
 * Set Upload Mode
 *
 * @def PVR_IOW_UP_MODE
 * @param	ctrl		[in	] PVR Ctrl
 * @param	mode		[in ] Normal/Trick
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 */
int PVR_SetUploadMode(PVR_CTRL_T *ctrl, LX_PVR_UP_MODE_T mode)
{
	UINT8 use_timestamp, playmode;
	LX_PVR_CFG_T* cfg = PVR_CFG_GetConfig();

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "mode=%d", mode);

	PVR_CHECK_ERROR(mode >= LX_PVR_UPMODE_MAX, return RET_ERROR, "invalid mode[%d]", mode);

#if 0
	if(ctrl->state.up_mode == mode)
		return RET_OK;
#endif

	PVR_PRINT_NOTI("SetUploadMode : %s", (mode == LX_PVR_UPMODE_TRICK_MODE) ? "TRICK" :
										(mode == LX_PVR_UPMODE_SLOW) ? "SLOW" : "NORMAL");

	PVR_UP_LOCK(ctrl);

	use_timestamp = (mode == LX_PVR_UPMODE_TRICK_MODE) ? FALSE: TRUE;
	playmode = (mode == LX_PVR_UPMODE_SLOW) ? 2 : 0;

	if(cfg->fixup_timestamp && use_timestamp)
	{
		BOOLEAN enable, pause;

		enable = DVR_UP_GetEnable(ctrl->ch);
		if(enable)
		{
			pause = DVR_UP_GetPause(ctrl->ch);
			if(!pause)
			{
				DVR_UP_PauseReg(ctrl->ch, TRUE);
			}

			WaitUploadPauseState(ctrl);

			DVR_UP_ChangePlaymode(ctrl->ch, playmode);
			DVR_UP_CopyTimeStamp(ctrl->ch);

			if(!pause)
			{
				DVR_UP_PauseReg(ctrl->ch, FALSE);
			}
		}
		else
		{
			DVR_UP_SetTimeStampCheck(ctrl->ch, TRUE);
			DVR_UP_ChangePlaymode(ctrl->ch, playmode);
		}
	}
	else
	{
		DVR_UP_SetTimeStampCheck(ctrl->ch, use_timestamp);
		DVR_UP_ChangePlaymode(ctrl->ch, playmode);
	}

	ctrl->state.up_mode			= mode;
	ctrl->state.use_timestamp	= use_timestamp;

	PVR_UP_UNLOCK(ctrl);

	return RET_OK;
}


/**
 * Set Upload Speed
 * Only used if not using timestamp
 *
 * @def PVR_IOW_UP_SET_SPEED
 * @param	ctrl		[in	] PVR Ctrl
 * @param	speed		[in	] Upload Speed - bytes/sec
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 */
int PVR_SetUploadSpeed(PVR_CTRL_T *ctrl, UINT32 speed)
{
	UINT32 kbps;
	UINT16 wait_cycle;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "speed=%d", speed);

	/*
	 * WaitCycle = 188*8*CLK_TE/BIT_RATE - 202
	 *           = 188*8*198*1000*1000/BIT_RATE - 202
	 *           = 188*198*1000*1000/BYTE_RATE - 202
	 *           = 327224000000/BYTE_RATE - 202
	 *           = 36351562/KBYTE_RATE - 202
	 *           = 35499/MBYTE_RATE -202
	 *
	 * if WaitCycle = 0x2000 then
	 *	 DataRate(Bytes/sec) = 188*198*1000*1000/(8192+202) = 4.229(Mbytes/sec)
	 */

#define UPLOAD_SPEED_MIN	(1*1024)
#define UPLOAD_SPEED_MAX	(10*1024)

	kbps = speed >> 10;
	if(kbps < UPLOAD_SPEED_MIN)
	{
		PVR_PRINT_WARNING("Too slow... Up to %dMBps", UPLOAD_SPEED_MIN/1024);
		kbps = UPLOAD_SPEED_MIN;
	}
	else if(kbps > UPLOAD_SPEED_MAX)
	{
		PVR_PRINT_WARNING("Too fast... Down to %dMBps", UPLOAD_SPEED_MAX/1024);
		kbps = UPLOAD_SPEED_MAX;
	}

	wait_cycle = (UINT16)(36351562/kbps - 202);

	DVR_UP_SetWaitCycle(ctrl->ch, wait_cycle);

	return RET_OK;
}


/**
 * PVR_StartUpload
 * note:
 * @def PVR_IOW_UP_START
 * Stop PVR upload for specified PVR channel
 */
int PVR_StartUpload (PVR_CTRL_T *ctrl, UINT8 ui8PacketLen)
{
	int ret = 0;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "ui8PacketLen=%d", ui8PacketLen);

	if ( ctrl->state.eUpState != LX_PVR_UP_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_UP> One playback already active in [%d]channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eUpState );
		/* return -1; */
		/* Previous upload seem to be active, so close it and restart again */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_UP> Restarting playback[%d]channel !!!!\n",
				ctrl->ch );
		PVR_StopUpload(ctrl);
	}

	PVR_UP_LOCK(ctrl);

	/* Reset the error variables */
	ctrl->err_stat.up_overlap = 0;

	ctrl->stats.up_almost_empty = 0;
	ctrl->stats.up_overlap = 0;
	ctrl->stats.up_sync_err = 0;


	PVR_KDRV_LOG ( PVR_UPLOAD, "> Start Upload [%c] setting...\n", ctrl->ch ? 'B' : 'A' );
	DVR_UP_PauseReg(ctrl->ch, FALSE);

	/* Disable Almost empty and almost full interrupt */
	DVR_UP_EnableEmptyLevelReg(ctrl->ch, FALSE);

	/* Enable Error Interrupt */
	DVR_UP_EnableErrIntr(ctrl->ch, TRUE);

	/*
	 * The packet length is memorized, and used when the upload
	 * is started after copying first block of data
	 */
	DVR_UP_SetInputTSMode(ctrl->ch, ui8PacketLen);
	ctrl->ui8PacketLen = ui8PacketLen;

	//This flag is for enabling the upload after data copy for first time
	ctrl->ui8UploadFirstTime = TRUE;

	ctrl->state.eUpState = LX_PVR_UP_STATE_PLAY;

	PVR_UP_UNLOCK(ctrl);

	PVR_CORE_TRACE_PARAM_EXIT(ctrl, "ret=%d", ret);

	return ret;
}

/**
 * PVR_StopUpload
 * note:
 * @def PVR_IOW_UP_STOP
 * Stop PVR upload for specified PVR channel
 */
int PVR_StopUpload (PVR_CTRL_T *ctrl)
{
	int ret = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if ( ctrl->state.eUpState == LX_PVR_UP_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_UP> Playback not active in [%d]channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eUpState );
		return -1;
	}

	PVR_UP_LOCK(ctrl);

	DVR_UP_EnableEmptyLevelReg(ctrl->ch, FALSE);	//Disable the interrupt
	DVR_UP_EnableReg(ctrl->ch, FALSE);

	/* Clear the Pause state just in case */
	DVR_UP_PauseReg (ctrl->ch, FALSE );

	ctrl->state.eUpState = LX_PVR_UP_STATE_IDLE;

	PVR_UP_UNLOCK(ctrl);

	PVR_CORE_TRACE_PARAM_EXIT(ctrl, "ret=%d", ret);
	return ret;
}

/**
 * PVR_PauseUpload
 * note:
 * @def PVR_IOW_UP_PAUSE
 * Pause PVR upload for specified PVR channel
 */
int PVR_PauseUpload (PVR_CTRL_T *ctrl )
{
	int ret = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if ( ctrl->state.eUpState == LX_PVR_UP_STATE_IDLE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_UP> Playback not active in [%d]channel, State[%d]\n",
				ctrl->ch,
				(UINT32)ctrl->state.eUpState );
		return 0;
	}

	PVR_UP_LOCK(ctrl);

	/* Change to Pause state */
	DVR_UP_PauseReg (ctrl->ch, TRUE );

	ctrl->state.eUpState = LX_PVR_UP_STATE_PAUSE;

	PVR_UP_UNLOCK(ctrl);

	return ret;
}

/**
 * PVR_ResumeUpload
 * note:
 * @def PVR_IOW_UP_PAUSE
 * Resume PVR upload for specified PVR channel
 */
int PVR_ResumeUpload (PVR_CTRL_T *ctrl )
{
	int ret = RET_OK;

	PVR_CORE_TRACE_ENTER(ctrl);

	if ( ctrl->state.eUpState != LX_PVR_UP_STATE_PAUSE )
	{
		/* One recording already going on */
		PVR_KDRV_LOG ( PVR_WARNING, "DVR_UP> Playback not in Pause state-[%d]channel, State[%d] but Set TCP\n",
				ctrl->ch,
				(UINT32)ctrl->state.eUpState );
		return 0;
	}

	PVR_UP_LOCK(ctrl);

	/* Change to play state */
	if(ctrl->err_stat.up_overlap == 0)
		CopyUploadTimeStamp(ctrl);
	DVR_UP_PauseReg (ctrl->ch, FALSE );

	ctrl->state.eUpState = LX_PVR_UP_STATE_PLAY;

	PVR_UP_UNLOCK(ctrl);

	return ret;
}


int PVR_SetDownloadVPID(PVR_CTRL_T *ctrl, UINT32 vpid)
{
	int ret = RET_OK;

	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "ui32Pid=0x%x", vpid);

	DVR_DN_SetPIDReg(ctrl->ch, vpid);

	return ret;
}

int PVR_SetDownloadUnitBufPktCnt(PVR_CTRL_T *ctrl, UINT32 pktCnt)
{
	PVR_CORE_TRACE_PARAM_ENTER(ctrl, "pktCnt=%d", pktCnt);

	ctrl->unit_buf.pkt_cnt	= pktCnt;
	ctrl->unit_buf.size		= pktCnt * 192;
	ctrl->unit_buf.total	= ctrl->buf.dn.size / ctrl->unit_buf.size;

	PVR_PRINT_PIE("MAX Unit Buffer Count[%d]", ctrl->unit_buf.total);

	DVR_DN_ConfigureIntrLevel(ctrl->ch, PVR_DN_BUF_NUM_LIM, pktCnt);

	return 0;
}

/**
 * PVR_DD_GetUploadReadAddr
 * note:
 * @def PVR_IOW_MM_Init
 * Get the buffer write pointer for upload buffer
 */
int PVR_GetMemoryInfo (LX_PVR_GPB_INFO_T *stpLXPvrGPBInfo)
{
	int ch, type;
	LX_MEMCFG_T *mem_cfg;

	for(ch=0; ch<LX_PVR_CH_NUM; ch++)
	{
		mem_cfg = PVR_CFG_GetMemConfig(ch);
		for(type=0; type<LX_PVR_MEM_NUM; type++)
		{
			stpLXPvrGPBInfo->buf[ch][type].base = mem_cfg[type].base;
			stpLXPvrGPBInfo->buf[ch][type].size = mem_cfg[type].size;
		}
	}

	// TODO: Get base address & total size of the GPB from the pvr_cfg.c
	stpLXPvrGPBInfo->base = stpLXPvrGPBInfo->buf[0][0].base;
	stpLXPvrGPBInfo->size = stpLXPvrGPBInfo->buf[LX_PVR_CH_NUM-1][LX_PVR_MEM_NUM-1].base +
							stpLXPvrGPBInfo->buf[LX_PVR_CH_NUM-1][LX_PVR_MEM_NUM-1].size -
							stpLXPvrGPBInfo->buf[0][0].base;

	return RET_OK;
}


ULONG PVR_GetRegBaseAddr(LX_PVR_BLOCK_T block)
{
	ULONG	base_addr;

	switch(block)
	{
		case LX_PVR_BLOCK_TOP:		base_addr = (ULONG)PVR_TOP_REG_PHYS; break;
		case LX_PVR_BLOCK_PIE_A:	base_addr = (ULONG)PVR_PIE_REG_PHYS(LX_PVR_CH_A); break;
		case LX_PVR_BLOCK_PIE_B:	base_addr = (ULONG)PVR_PIE_REG_PHYS(LX_PVR_CH_B); break;
		case LX_PVR_BLOCK_DN_A:		base_addr = (ULONG)PVR_DN_REG_PHYS(LX_PVR_CH_A); break;
		case LX_PVR_BLOCK_DN_B:		base_addr = (ULONG)PVR_DN_REG_PHYS(LX_PVR_CH_B); break;
		case LX_PVR_BLOCK_UP_A:		base_addr = (ULONG)PVR_UP_REG_PHYS(LX_PVR_CH_A); break;
		case LX_PVR_BLOCK_UP_B:		base_addr = (ULONG)PVR_UP_REG_PHYS(LX_PVR_CH_B); break;
		default : base_addr = 0;
	}
	return base_addr;
}

UINT32 PVR_GetRegSize(LX_PVR_BLOCK_T block)
{
	UINT32 size;

	switch(block)
	{
		case LX_PVR_BLOCK_TOP:		size = PVR_TOP_REG_SIZE; break;
		case LX_PVR_BLOCK_PIE_A:
		case LX_PVR_BLOCK_PIE_B:	size = PVR_PIE_REG_SIZE; break;
		case LX_PVR_BLOCK_DN_A:
		case LX_PVR_BLOCK_DN_B:		size = PVR_DN_REG_SIZE; break;
		case LX_PVR_BLOCK_UP_A:
		case LX_PVR_BLOCK_UP_B:		size = PVR_UP_REG_SIZE; break;
		default : size = 0;
	}
	return size;
}

int PVR_GetRegister(LX_PVR_GET_REG_T *reg)
{
	ULONG	base_addr;

	PVR_CHECK_ERROR( reg == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	PVR_CHECK_ERROR( (reg->offset%4) != 0, return RET_INVALID_PARAMS, "Not aligned offset");
	PVR_CHECK_ERROR( reg->block < LX_PVR_BLOCK_TOP || reg->block >= LX_PVR_BLOCK_MAX,
		return RET_INVALID_PARAMS, "Invalid argument" );
	PVR_CHECK_ERROR( reg->offset >= PVR_GetRegSize(reg->block), return RET_INVALID_PARAMS, "Invalid argument" );

	base_addr = PVR_GetRegBaseAddr(reg->block);
	if(base_addr == 0) return -1;

	reg->value = PVR_REG_READ(base_addr + reg->offset);

	return 0;
}

int PVR_SetRegister(LX_PVR_GET_REG_T *reg)
{
	ULONG	base_addr;

	PVR_CHECK_ERROR( reg == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	PVR_CHECK_ERROR( reg->block < LX_PVR_BLOCK_TOP || reg->block >= LX_PVR_BLOCK_MAX,
						return RET_INVALID_PARAMS, "Invalid argument" );
	PVR_CHECK_ERROR( (reg->offset%4) != 0, return RET_INVALID_PARAMS, "Not aligned offset");
	PVR_CHECK_ERROR( reg->offset >= PVR_GetRegSize(reg->block), return RET_INVALID_PARAMS, "Invalid argument" );

	base_addr = PVR_GetRegBaseAddr(reg->block);
	if(base_addr == 0) return -1;

	PVR_REG_WRITE(base_addr + reg->offset, reg->value);

	return 0;
}

int PVR_GetStatus(PVR_CTRL_T *ctrl, LX_PVR_STATUS_T *status)
{
	UINT32 size;
	UINT32 wptr,rptr;

	memset(status, 0, sizeof(LX_PVR_STATUS_T));
	if(ctrl->state.eUpState != LX_PVR_UP_STATE_IDLE)
	{
		size = ctrl->buf.up.end - ctrl->buf.up.base;

		DVR_UP_GetPointersReg(ctrl->ch, &wptr, &rptr);
		if( ((UINT32)wptr >= ctrl->buf.up.base && wptr < ctrl->buf.up.end) &&
			(rptr >= ctrl->buf.up.base && wptr < ctrl->buf.up.end))
		{
			status->data_size[LX_PVR_MEM_UP] = (size + wptr - rptr) % size;
		}
	}

	if(ctrl->state.eDnState != LX_PVR_DN_STATE_IDLE)
	{
		size = ctrl->buf.dn.end - ctrl->buf.dn.base;

		wptr = ctrl->buf.dn.wptr;
		rptr =  ctrl->buf.dn.rptr;

		status->data_size[LX_PVR_MEM_DN] = (size + wptr - rptr) % size;
	}

	return 0;
}


#define TIMESTAMP_MS_TO_CLOCK(x)		((x)*27000)
#define UPLOAD_TIMESTAMP_MAX_JITTER		TIMESTAMP_MS_TO_CLOCK(500)

static int TimeStampCopyThread(void *data)
{
	PVR_CTRL_T* ctrl;
	LX_PVR_CFG_T* cfg = PVR_CFG_GetConfig();
	LX_PVR_CH_T ch;
	int jitter;
#define TIMESTAMP_CHECK_INTERVAL		100

	while(!kthread_should_stop())
	{
		for(ch=LX_PVR_CH_A; ch<cfg->num_updev; ch++)
		{
			ctrl = PVR_GetCtrl(ch);
			if(ctrl == NULL)
				continue;

			PVR_UP_LOCK(ctrl);
			if(ctrl->state.eUpState == LX_PVR_UP_STATE_PLAY &&
				ctrl->state.use_timestamp &&
				ctrl->ui8UploadFirstTime == FALSE &&
				ctrl->ui8PacketLen == 192)
			{
				DVR_UP_GetTSCJitterReg(ch, (UINT32*)&jitter);
				if(abs(jitter) > UPLOAD_TIMESTAMP_MAX_JITTER)
				{
					BOOLEAN pause;

					PVR_PRINT_WARNING("CH[%d] OVER MAX JITTER:[%d]\n", ch, jitter);

					pause = DVR_UP_GetPause(ctrl->ch);
					if(!pause)
					{
						DVR_UP_PauseReg(ctrl->ch, TRUE);
					}
					WaitUploadPauseState(ctrl);
					DVR_UP_CopyTimeStamp(ctrl->ch);

					if(!pause)
					{
						DVR_UP_PauseReg(ctrl->ch, FALSE);
					}
				}
			}
			PVR_UP_UNLOCK(ctrl);
		}

		OS_MsecSleep(TIMESTAMP_CHECK_INTERVAL);
	}

	PVR_PRINT_ERROR("Stop TimeStampCopyThread\n");

	return 0;
}


