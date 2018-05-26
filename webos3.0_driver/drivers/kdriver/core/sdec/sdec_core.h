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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef _SDEC_CORE_H_
#define _SDEC_CORE_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_reg_ctrl.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define GPB_BASE_ADDR_MASK		0xF0000000


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define SDEC_GPB_LOCK(h, ch_idx, flt_idx)		spin_lock_irqsave(&h->stSdecGpbSpinlock[ch_idx][flt_idx], flags);
#define SDEC_GPB_UNLOCK(h, ch_idx, flt_idx)		spin_unlock_irqrestore(&h->stSdecGpbSpinlock[ch_idx][flt_idx], flags);


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32 	ui8Channel : 8,
			ui16StatusInfo : 16,
		 	ui8index : 8;
	UINT32 	ui32ReadPtr;
	UINT32 	ui32WritePtr;
} S_GPB_INFO_T;


typedef struct
{
	UINT8 	ui8Channel;
	UINT32 	STC_hi_value;
	UINT32 	STC_lo_value;
	UINT32 	PCR_hi_value;
	UINT32 	PCR_lo_value;
} S_PCR_STC_T;

typedef struct
{
	UINT8 ui8Channel;
	UINT8 ui8PIDIdx;
} S_TPI_INTR_T;

typedef union
{
	struct
	{
		UINT16 SDEC_FLTSTATE_FREE			:1;
		UINT16 SDEC_FLTSTATE_ALLOC			:1;
		UINT16 SDEC_FLTSTATE_ENABLE			:1;
		UINT16 SDEC_FLTSTATE_SCRAMBLED		:1;
		UINT16 SDEC_FLTSTATE_DATAREADY		:1;
		UINT16 SDEC_FLTSTATE_SCRMREADY		:1;
		UINT16  							:6;
		UINT16 SDEC_FLTSTATE_OVERFLOW		:1;
		UINT16 								:3;
	}f;
	UINT16 w;
} S_STATUS_INFO_T;

typedef struct{

	UINT32  flag : 1, //enable or disable
			mode : 8,
			used : 1,
			reserved : 22;
	S_STATUS_INFO_T stStatusInfo;
} S_FILTER_MAP_T;


typedef struct
{
	UINT32 ui32Baseptr;
	UINT32 ui32Endptr;
	UINT32 ui32Readptr;
	UINT32 ui32UsrReadptr;
	GPB_BND stGpbBnd;
	UINT8 ui8PidFltIdx;
} S_SDEC_MEMORY_INFO_T;


/* Register Backup for InstantBoot */
typedef struct
{
	UINT32 top[50]; // TODO: the value should be macro-defined
	UINT32 io[2][90];
	UINT32 mpg[2][2][390];

} SDEC_REG_BACKUP_T;

typedef struct
{
	//releated OS
	OS_SEM_T		stSdecMutex;
	spinlock_t 		stSdecNotiSpinlock;
	spinlock_t 		stSdecGpbSpinlock[SDEC_IO_CH_NUM][SDEC_IO_FLT_NUM];
	spinlock_t 		stSdecPesSpinlock;
	spinlock_t 		stSdecSecSpinlock;
	spinlock_t 		stSdecResetSpinlock;
	spinlock_t		stSdecPidfSpinlock;

	// Event for SW Parser
	OS_EVENT_T		stSdecSWPEvent;
	OS_EVENT_T		stSdecRFOUTEvent; /* LIVE_HEVC */

	wait_queue_head_t wq;
	int				  wq_condition;

	//work queue
	struct workqueue_struct *workqueue;
	struct work_struct Notify;
	struct work_struct PcrRecovery;
	struct work_struct PesProc;
	struct work_struct SecProc;
	struct work_struct TPIIntr;

	LX_SDEC_CFG_T	*cfg;

	S_GPB_INFO_T	stPesGPBInfo[LX_SDEC_MAX_PESINFO_DATA];
	UINT8 			ui8PesGpbInfoWIdx;
	UINT8 			ui8PesGpbInfoRIdx;

	S_PCR_STC_T stPCR_STC;
	S_TPI_INTR_T stTPI_Intr;

	S_FILTER_MAP_T stPIDMap[SDEC_IO_CH_NUM][SDEC_IO_FLT_NUM];
	S_FILTER_MAP_T stSecMap[SDEC_IO_CH_NUM][SDEC_IO_FLT_NUM];

	S_SDEC_MEMORY_INFO_T stSdecMeminfo[SDEC_IO_CH_NUM][SDEC_IO_FLT_NUM];

//	S_GPB_DATA_T stGPBData[MAX_GPB_DATA];
	LX_SDEC_NOTIFY_PARAM_T stGPBInfo[LX_SDEC_MAX_GPB_DATA];


	UINT8 ui8CurrentCh;
	UINT8 ui8CurrentMode;

	UINT32 ui32MsgMask;

	UINT8 bPcrRecoveryFlag[SDEC_IO_CH_NUM];

	UINT8 ui8GpbInfoWIdx;
	UINT8 ui8GpbInfoRIdx;

	LX_SDEC_INPUT_T eInputPath[SDEC_IO_CH_NUM];

	UINT8 ui8CDICResetMode;
	UINT8 ui8SDMWCResetMode;
	UINT8 ui8CDICResetNum;
	UINT8 ui8SDMWCResetNum;

	/* for JP MCU Descrambler Control Mode in H13 */
	UINT8 ui8McuDescramblerCtrlMode;

	/* for using original external demod clock in serial input, ex> Columbia */
	UINT8 ui8UseOrgExtDemodClk;

	/* for UHD bypass channel */
	UINT8 ui8RFBypassChannel;

	/* for UHD SDT channel in H13 */
	UINT8 ui8SDTChannel;

	/* for TVCT Problem */
	UINT8 ui8ClearTVCTGathering;

	SDEC_REG_BACKUP_T *reg_backup;
}S_SDEC_PARAM_T, SDEC_PARAM_T;




/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int SDEC_Initialize(S_SDEC_PARAM_T *stpSdecParam);
int	SDEC_InitHW(S_SDEC_PARAM_T *stpSdecParam);
int	SDEC_CleanupHW(S_SDEC_PARAM_T *stpSdecParam);

int SDEC_MutexInitialize(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_WorkQueueInitialize(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_WorkQueueDestroy(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_WaitQueueInitialize(S_SDEC_PARAM_T *stpSdecParam);

int SDEC_IO_InitialaizeModule(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CAP_T*	stpLXSdecCap);
int SDEC_IO_GetChipCfg(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CHIP_CFG_T *stpLXSdecGetChipCfg);

void SDEC_Notify(struct work_struct *work);
BOOLEAN SDEC_SetNotifyParam(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_NOTIFY_PARAM_T notiParam);
void SDEC_CheckNotifyOvf(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_NOTIFY_PARAM_T *pNotiParam);
void SDEC_DeleteInNotify(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui32SecFltId);


int SDEC_IO_GetGPBBaseOffset(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GPB_BASE_OFFSET_T *stpLXSdecGPBBaseOffset);
int SDEC_GpbSet(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8ch,UINT32 ui32GpbSize, UINT32 ui32GpbBaseAddr, UINT32 ui32GpbIdx);
int SDEC_DummyGpbSet(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8ch, LX_SDEC_GPB_SIZE_T eGpbSize, UINT32 ui32GpbBaseAddr, UINT32 ui32GpbIdx);

int SDEC_IO_SetMCUDescramblerCtrlMode(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_MCUMODE_T *stpLXSdecSetMcumode);
int SDEC_IO_SetTVCTMode(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_TVCTMODE_T *stpLXSdecClearTVCTGathering);
int SDEC_IO_UseOrgExtDemodCLKinSerialInput(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_SERIALCLKMODE_T *stpLXSdecSetSerialClkMode);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_CORE_H_ */

/** @} */
