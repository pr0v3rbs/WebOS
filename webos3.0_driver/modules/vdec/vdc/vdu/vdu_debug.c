/* *****************************************************************************
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * ****************************************************************************/

/** @file
 *
 * Video decoding unit debug implementation for VDEC device.
 * VDEC device will teach you how to make device driver with lg1154 platform.
 *
 * @author     Youngwoo Jin(youngwoo.jin@lge.com)
 * @version    1.0
 * @date       2012.10.03
 * @note       Additional information.
 *
 * @addtogroup lg1154_vdec
 * @{
 */

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	File Inclusions
------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "vdu_debug.h"
#include "vdu_drv.h"
#include "../../proc.h"
#include "../../log.h"

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define PERFORMANCE_PRINT_PERIOD			(1000) // 1 Second
#define PERFORMANCE_NUMBER_OF_DATA			(100)
#define PERFORMANCE_STRING_LENGTH			(15 * VDU_MAX_NUM_OF_INSTANCES + 6 + 1)		// Inst(15), Avg(6), NULL(1)

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Type Definitions
------------------------------------------------------------------------------*/
typedef struct proc_dir_entry		PROC_DIR_ENTRY_T;
typedef struct file_operations		FILE_OPERATIONS;
typedef struct timer_list			TIMER_LIST_T;
typedef struct kernel_param_ops		KERNEL_PARAM_T;

typedef struct {
	VDU_HANDLE_T			hInstance;
	UINT64					ui64StartTimeInNs;
	UINT64					ui64EndTimeInNs;
	SINT32					si32DecTimeInUs;
} DEC_TIME_T;

typedef struct {
	spinlock_t				stLock;
	BOOLEAN					bEnable;

	BOOLEAN					bRun;
	TIMER_LIST_T			stTimer;

	UINT32					ui32EnableFlags;

	UINT64					aui64StartTimeInNs[VDU_MAX_NUM_OF_INSTANCES];
	UINT16					ui16DecCount;
	UINT16					ui16WritePointer;
	DEC_TIME_T*				astDecTime;
} PERFOM_INFO_T;

typedef struct {
	BOOLEAN					bEnabled;
	UINT32					ui32DecRate100;
	UINT32					ui32AvgHwDecTime100;
	UINT32					ui32AvgDecTime100;
} PERFORM_RESULT_T;

typedef struct {
	spinlock_t				stLock;

	int						iStepCount;
	BOOLEAN					bWaiting[VDU_MAX_NUM_OF_INSTANCES];
} STEP_DEC_INFO_T;

typedef struct {
	char					acName[10];
	PROC_DIR_ENTRY_T*		pstRoot;
} PROC_DATA_T;

typedef struct {
	BOOLEAN					bInUsed;
	PROC_DATA_T				stProcData;
	VDU_OPEN_PARAM_T		stOpenParam;

	VDU_HANDLE_T			hInstance;
} DEBUG_INFO_T;

/*------------------------------------------------------------------------------
	External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Static Function Prototypes Declarations
------------------------------------------------------------------------------*/
static int		_OpenStatus(struct inode *inode, struct file *file);
static int		_ShowStatus(struct seq_file *seq_file, void *data);
static int		_OpenPerform(struct inode *inode, struct file *file);
static int		_CalcPerform(struct seq_file *seq_file, void *data);
static ssize_t	_WritePerform(struct file *file, const char __user *buf,
								size_t size, loff_t *off);
static void		_InitPerformance(void);
static void		_OpenPerformance(VDU_HANDLE_T hInstance);
static void		_ClosePerformance(VDU_HANDLE_T hInstance);
static void		_StartPerformance(void);
static void		_StopPerformance(void);
static void		_CalcPerformance(PERFOM_INFO_T* pstPerformance,
									PERFORM_RESULT_T* astPerformResult);
static char*	_PrintPerformance(PERFORM_RESULT_T* astResult,
									char* pcBuffer, unsigned int uiBufSize);
static void		_PrintDecRate(ULONG ulData);
static void		_PrintResultInfo(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult);
static void		_PrintFrameInfo(VDU_HANDLE_T hInstance, VDU_FRM_INFO_T* pstFrameInfo);
static int		_ParamSetIntHook(const char *val, const struct kernel_param *kp);

/*------------------------------------------------------------------------------
	global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Static Variables
------------------------------------------------------------------------------*/
static PROC_DIR_ENTRY_T*	_gpstProcRootEntry;
static PERFOM_INFO_T		_gstPerformance;
static STEP_DEC_INFO_T		_gstStepDecInfo;

static DEBUG_INFO_T			_gastDebugInfo[VDU_MAX_NUM_OF_INSTANCES];

static KERNEL_PARAM_T		_gstParam = {	.set = _ParamSetIntHook,
											.get = param_get_int, };

static FILE_OPERATIONS		_gstStatusFops = {	.open = _OpenStatus,
												.read = seq_read,
												.release = single_release,		};

static FILE_OPERATIONS		_gstPerformFops = {	.open = _OpenPerform,
												.write = _WritePerform,
												.read = seq_read,
												.release = single_release,		};

module_param_cb(vdu_step_decode, &_gstParam, &_gstStepDecInfo.iStepCount, 0644);

/*==============================================================================
    Implementation Group
==============================================================================*/
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
void		VDU_DBG_Init(void)
{
	UINT32		ui32Index;
	ULONG		ulFlags;

	_gpstProcRootEntry = proc_mkdir("vdu", vdec_proc_root_entry);
	proc_create_data("performance", 0660, _gpstProcRootEntry,
						&_gstPerformFops, &_gstPerformance);

	// Init performance
	_InitPerformance();

	// Init Step decoding
	spin_lock_init(&_gstStepDecInfo.stLock);
	spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
	_gstStepDecInfo.iStepCount = -1;
	for ( ui32Index = 0; ui32Index < VDU_MAX_NUM_OF_INSTANCES; ++ui32Index ) {
		_gstStepDecInfo.bWaiting[ui32Index] = FALSE;
	}
	spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);
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
void		VDU_DBG_Open(VDU_HANDLE_T hInstance, VDU_OPEN_PARAM_T *pstOpenParam)
{
	UINT32			ui32Index = (UINT32)hInstance;
	PROC_DATA_T*	pstProcData = NULL;
	DEBUG_INFO_T*	pstDebugInfo = &_gastDebugInfo[ui32Index];
	ULONG			ulFlags = 0x00;

	if ( ui32Index >= VDU_MAX_NUM_OF_INSTANCES ) {
		goto GOTO_END;
	}

	pstDebugInfo->bInUsed = TRUE;
	pstDebugInfo->stOpenParam = *pstOpenParam;
	pstDebugInfo->hInstance = hInstance;
	pstProcData = &pstDebugInfo->stProcData;

	snprintf(pstProcData->acName, 10, "Inst_%d", (UINT32)hInstance);
	pstProcData->pstRoot = proc_mkdir(pstProcData->acName, _gpstProcRootEntry);
	proc_create_data("status", 0440, pstProcData->pstRoot, &_gstStatusFops, pstDebugInfo);

	_OpenPerformance(hInstance);

	spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
	_gstStepDecInfo.bWaiting[ui32Index] = FALSE;
	spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);

GOTO_END:
	return;
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
void		VDU_DBG_Close(VDU_HANDLE_T hInstance)
{
	UINT32			ui32Index = (UINT32)hInstance;
	PROC_DATA_T*	pstProcData = NULL;
	ULONG			ulFlags = 0x00;

	if ( ui32Index >= VDU_MAX_NUM_OF_INSTANCES ) {
		goto GOTO_END;
	}

	_ClosePerformance(hInstance);

	spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
	_gstStepDecInfo.bWaiting[ui32Index] = FALSE;
	spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);

	pstProcData = &_gastDebugInfo[ui32Index].stProcData;
	remove_proc_entry("status", pstProcData->pstRoot);
	remove_proc_entry(pstProcData->acName, _gpstProcRootEntry);

	_gastDebugInfo[ui32Index].bInUsed = FALSE;

GOTO_END:
	return;
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
void		VDU_DBG_CheckDecTime(VDU_HANDLE_T hInstance, SINT32 si32DecTimeInUs)
{
	UINT32			ui32Index = (UINT32)hInstance;
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	ULONG			ulFlags = 0x00;

	if ( ui32Index >= VDU_MAX_NUM_OF_INSTANCES ) {
		goto GOTO_END;
	}

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	if ( (pstPerformance->astDecTime != NULL) &&
		 (pstPerformance->ui32EnableFlags & (1 << ui32Index)) ) {
		if ( si32DecTimeInUs == VDU_DBG_START_CHECK ) {
			pstPerformance->aui64StartTimeInNs[ui32Index] = sched_clock();
		} else if ( (si32DecTimeInUs >= 0) ||
					(si32DecTimeInUs == VDU_DBG_POSTPONE_CHECK) ) {
			UINT16		ui16WritePtr = pstPerformance->ui16WritePointer;
			DEC_TIME_T*	pstDecTime = &pstPerformance->astDecTime[ui16WritePtr];

			pstDecTime->hInstance = hInstance;
			pstDecTime->ui64StartTimeInNs = pstPerformance->aui64StartTimeInNs[ui32Index];
			pstDecTime->ui64EndTimeInNs = sched_clock();
			pstDecTime->si32DecTimeInUs = si32DecTimeInUs;

			pstPerformance->ui16WritePointer += 1;
			pstPerformance->ui16WritePointer %= PERFORMANCE_NUMBER_OF_DATA;

			if ( pstPerformance->ui16DecCount < PERFORMANCE_NUMBER_OF_DATA ) {
				++pstPerformance->ui16DecCount;
			}
		}

		if ( (pstPerformance->bRun == FALSE) &&
			 logm_enabled(vdec_vdu, log_level_noti) ) {
			pstPerformance->bRun = TRUE;
			mod_timer(&pstPerformance->stTimer, get_jiffies_64() + msecs_to_jiffies(PERFORMANCE_PRINT_PERIOD));
		}
	}
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);

GOTO_END:
	return;
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
BOOLEAN		VDU_DBG_WaitStepDecoding(VDU_HANDLE_T hInstance)
{
	BOOLEAN		bRetVal = FALSE;
	ULONG		ulFlags;

	spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
	if ( _gstStepDecInfo.iStepCount == -1 ) {
		bRetVal = FALSE;
		goto GOTO_END;
	}

	if ( _gstStepDecInfo.iStepCount == 0 ) {
		_gstStepDecInfo.bWaiting[(UINT32)hInstance] = TRUE;
		bRetVal = TRUE;
	}

	_gstStepDecInfo.iStepCount--;

GOTO_END:
	spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);

	return bRetVal;
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
void		VDU_DBG_PrintResultInfo(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult)
{
	_PrintResultInfo(hInstance, pstResult);
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
void		VDU_DBG_PrintFrameInfo(VDU_HANDLE_T hInstance, VDU_FRM_INFO_T* pstFrameInfo)
{
	_PrintFrameInfo(hInstance, pstFrameInfo);
}

/*==============================================================================
    Static Implementation Group
==============================================================================*/
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
static int		_OpenStatus(struct inode* inode, struct file* file)
{
	DEBUG_INFO_T*		pstDebugInfo = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	PROC_DIR_ENTRY_T*	pstPde = NULL;

	pstPde = PDE(inode);
	pstDebugInfo = (DEBUG_INFO_T*)pstPde->data;
#else

	pstDebugInfo = (DEBUG_INFO_T*)PDE_DATA(inode);
#endif

	return single_open(file, _ShowStatus, (void*)pstDebugInfo);
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
static int		_ShowStatus(struct seq_file* file, void* data)
{
	DEBUG_INFO_T*		pstDebugInfo = (DEBUG_INFO_T*)file->private;
	VDU_OPEN_PARAM_T*	pstOpenParam = &pstDebugInfo->stOpenParam;

	seq_printf(file, "Codec type         : %d\n",
						(UINT32)pstOpenParam->eCodecType);
	seq_printf(file, "LinearBufMode      : %d\n",
						(UINT32)pstOpenParam->eExternLinearBufMode);
	seq_printf(file, "No delay mode      : %s\n",
						(pstOpenParam->bNoDelayMode)? "TRUE" : "FALSE");
	seq_printf(file, "One frame mode     : %s\n",
						(pstOpenParam->bOneFrameDecoding)? "TRUE" : "FALSE");
	seq_printf(file, "Extern linear mode : %d\n",
						(UINT32)pstOpenParam->eExternLinearBufMode);
	seq_printf(file, "Valid threshold    : %d\n",
						pstOpenParam->ui16ValidThreshold);
	seq_printf(file, "CPB address & size : 0x%08X(0x%08X)\n",
						pstOpenParam->ui32CPBAddr,
						pstOpenParam->ui32CPBSize);

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
static int		_OpenPerform(struct inode* inode, struct file* file)
{
	PERFOM_INFO_T*		pstPerformance = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	PROC_DIR_ENTRY_T*	pstPde = NULL;

	pstPde = PDE(inode);
	pstPerformance = (PERFOM_INFO_T*)pstPde->data;
#else

	pstPerformance = (PERFOM_INFO_T*)PDE_DATA(inode);
#endif

	return single_open(file, _CalcPerform, (void*)pstPerformance);
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
static int		_CalcPerform(struct seq_file* file, void* data)
{
//	PERFOM_INFO_T*		pstPerformance = (PERFOM_INFO_T*)file->private;
	PERFORM_RESULT_T	astResult[VDU_MAX_NUM_OF_INSTANCES] = { { 0, }, };
	char				acBuffer[100] = { 0, };
	char*				pcEnd = NULL;

	_CalcPerformance(&_gstPerformance, astResult);
	pcEnd = _PrintPerformance(astResult, acBuffer, 100);
	*pcEnd = '\0';

	seq_printf(file, "%s\n", acBuffer);

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
static ssize_t	_WritePerform(struct file *file, const char __user *buf,
								size_t size, loff_t *off)
{
	ssize_t			ret_val = -EFAULT;
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	char			acBuffer[64];
	char*			pcEndPointer = NULL;
	SINT32			si32InputVal = 0;

	if ( size == 0 ) {
		ret_val = 0;
		goto GOTO_END;
	}

	/* we only allow single write */
	if ( *off != 0 ) {
		ret_val = -EINVAL;
		goto GOTO_END;
	}

	if ( size > (sizeof(acBuffer) - 1) ) {
		logm_warning (vdec_vdu, "Too long string.\n");
		ret_val = -EINVAL;
		goto GOTO_END;
	}

	if ( copy_from_user(acBuffer, buf, size) > 0 ) {
		ret_val = -EFAULT;
		goto GOTO_END;
	}

	acBuffer[size - 1] = 0;
	si32InputVal = simple_strtoul(acBuffer, &pcEndPointer, 0);
	if ( acBuffer == pcEndPointer ) {
		logm_warning (vdec_vdu, "Invalid input value.\n");
		ret_val = -EINVAL;
		goto GOTO_END;
	}

	if ( si32InputVal <= 0 ) {
		pstPerformance->bEnable = FALSE;
		_StopPerformance();
	} else if ( pstPerformance->bEnable == FALSE ) {
		pstPerformance->bEnable = TRUE;
		_StartPerformance();
	}

	logm_debug (vdec_vdu, "%s performance\n",
					(si32InputVal > 0)? "Enable" : "Disable");

	ret_val = size;

GOTO_END:
	return ret_val;
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
static void		_InitPerformance(void)
{
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;

	spin_lock_init(&pstPerformance->stLock);

	pstPerformance->bEnable = FALSE;
	pstPerformance->bRun = FALSE;
	pstPerformance->ui32EnableFlags = 0x00;

	init_timer(&pstPerformance->stTimer);
    pstPerformance->stTimer.function= _PrintDecRate;

	logm_noti(vdec_vdu, "Initialize decoding performance\n");
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
static void		_OpenPerformance(VDU_HANDLE_T hInstance)
{
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	ULONG			ulFlags = 0x00;

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	pstPerformance->ui32EnableFlags |= (1 << (UINT32)hInstance);
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);

	_StartPerformance();
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
static void		_ClosePerformance(VDU_HANDLE_T hInstance)
{
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	ULONG			ulFlags = 0x00;

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	if ( (pstPerformance->astDecTime != NULL) &&
		 (pstPerformance->ui32EnableFlags & (1 << (UINT32)hInstance)) ) {
		UINT16		ui16Index = 0;

		for ( ui16Index = 0; ui16Index < pstPerformance->ui16DecCount; ++ui16Index ) {
			DEC_TIME_T*	pstDecTime = &pstPerformance->astDecTime[ui16Index];

			if ( pstDecTime->hInstance == hInstance ) {
				pstDecTime->si32DecTimeInUs = VDU_DBG_POSTPONE_CHECK;
			}
		}
	}

	pstPerformance->ui32EnableFlags &= ~(1 << (UINT32)hInstance);
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);

	_StopPerformance();
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
static void		_StartPerformance(void)
{
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	ULONG			ulFlags = 0x00;

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	if ( (pstPerformance->bEnable == TRUE) &&
		 (pstPerformance->ui32EnableFlags != 0x00) ) {
		if ( pstPerformance->astDecTime == NULL ) {
			pstPerformance->astDecTime = kmalloc(sizeof(DEC_TIME_T) * PERFORMANCE_NUMBER_OF_DATA, GFP_ATOMIC);
			if ( pstPerformance->astDecTime == NULL ) {
				logm_error(vdec_vdu, "Debug allocation error\n");
				goto GOTO_END;
			}

			pstPerformance->ui16DecCount = 0;
			pstPerformance->ui16WritePointer = 0;
		}

		if ( (pstPerformance->bRun == FALSE) &&
			 logm_enabled(vdec_vdu, log_level_noti) ) {
			pstPerformance->bRun = TRUE;
			mod_timer(&pstPerformance->stTimer, get_jiffies_64() + msecs_to_jiffies(PERFORMANCE_PRINT_PERIOD));
		}

		logm_noti(vdec_vdu, "Start performance timer\n");
	}
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);

GOTO_END:
	return;
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
static void		_StopPerformance(void)
{
	PERFOM_INFO_T*	pstPerformance = &_gstPerformance;
	ULONG			ulFlags = 0x00;

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	if ( (pstPerformance->bEnable == FALSE) ||
		 (pstPerformance->ui32EnableFlags == 0x00) ) {
   		if ( pstPerformance->astDecTime != NULL ) {
			kfree(pstPerformance->astDecTime);
			pstPerformance->astDecTime = NULL;
		}

		if ( pstPerformance->bRun == TRUE ) {
			pstPerformance->bRun = FALSE;
			del_timer(&pstPerformance->stTimer);
		}

		logm_noti(vdec_vdu, "Stop performance timer\n");
	}
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);
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
static void		_CalcPerformance(PERFOM_INFO_T* pstPerformance,
									PERFORM_RESULT_T* astPerformResult)
{
	UINT64		ui64StartOfPeriodInNs[VDU_MAX_NUM_OF_INSTANCES] = { 0, };
	UINT64		ui64EndOfPeriodInNs[VDU_MAX_NUM_OF_INSTANCES] = { (UINT64)-1, };
	UINT16		ui16Index = 0;
	ULONG		ulFlags = 0x00;

	spin_lock_irqsave(&pstPerformance->stLock, ulFlags);
	if ( pstPerformance->ui16DecCount == 0 ) {
		goto GOTO_END;
	}

	ui16Index = pstPerformance->ui16WritePointer;
	if ( pstPerformance->ui16DecCount < PERFORMANCE_NUMBER_OF_DATA ) {
		ui16Index = 0;
	}

	do {
		DEC_TIME_T*			pstDecTime = NULL;
		UINT32				ui32HandleIdx = 0;
		PERFORM_RESULT_T*	pstResult = NULL;
		UINT64				ui64StartTimeInNs = 0;
		UINT64				ui64EndTimeInNs = 0;
		UINT32				ui32UsedTime = 0;

		ui16Index = ui16Index % PERFORMANCE_NUMBER_OF_DATA;

		pstDecTime = &pstPerformance->astDecTime[ui16Index];
		ui32HandleIdx = (UINT32)pstDecTime->hInstance;
		pstResult = &astPerformResult[ui32HandleIdx];

		if ( (ui32HandleIdx > VDU_MAX_NUM_OF_INSTANCES) ||
			 !(pstPerformance->ui32EnableFlags & (1 << ui32HandleIdx)) ) {
			continue;
		}

		ui64StartTimeInNs = pstDecTime->ui64StartTimeInNs;
		ui64EndTimeInNs = pstDecTime->ui64EndTimeInNs;
		ui32UsedTime = (UINT32)(ui64EndTimeInNs - ui64StartTimeInNs);

		if ( (pstDecTime->si32DecTimeInUs >= 0) &&
			 (ui64StartOfPeriodInNs[ui32HandleIdx] == 0) ) {
			ui64StartOfPeriodInNs[ui32HandleIdx] = ui64EndTimeInNs;
			continue;
		}

		if ( pstDecTime->si32DecTimeInUs >= 0 ) {
			pstResult->ui32DecRate100++;
			pstResult->ui32AvgHwDecTime100 += pstDecTime->si32DecTimeInUs;
			pstResult->ui32AvgDecTime100 += ui32UsedTime;
			ui64EndOfPeriodInNs[ui32HandleIdx] = ui64EndTimeInNs;
		}
	} while( (++ui16Index % PERFORMANCE_NUMBER_OF_DATA)
				!= pstPerformance->ui16WritePointer );

	for ( ui16Index = 0; ui16Index < VDU_MAX_NUM_OF_INSTANCES; ++ui16Index ) {
		PERFORM_RESULT_T*	pstResult = &astPerformResult[ui16Index];
		UINT32				ui32DecCount = 0;
		UINT32				ui32PeriodInUs = 0;

		if ( (pstPerformance->ui32EnableFlags & (1 << ui16Index)) == 0x00 ) {
			pstResult->bEnabled = FALSE;
			continue;
		}

		pstResult->bEnabled = TRUE;
		ui32DecCount = pstResult->ui32DecRate100;
		ui32PeriodInUs = (UINT32)(ui64EndOfPeriodInNs[ui16Index]
								- ui64StartOfPeriodInNs[ui16Index]) / 1000;

		if ( (ui32PeriodInUs > 0) && (ui32DecCount > 0) ) {
			pstResult->ui32DecRate100 = ui32DecCount * 1000 * 1000;
			pstResult->ui32DecRate100 /= ui32PeriodInUs / 100;

			pstResult->ui32AvgHwDecTime100 /= ui32DecCount * 10;
			pstResult->ui32AvgDecTime100 /= ui32DecCount * 1000 * 10;
		} else {
			pstResult->ui32DecRate100 = 0;
			pstResult->ui32AvgHwDecTime100 = 0;
			pstResult->ui32AvgDecTime100 = 0;
		}
	}

GOTO_END:
	spin_unlock_irqrestore(&pstPerformance->stLock, ulFlags);
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
static char*	_PrintPerformance(PERFORM_RESULT_T* astResult,
												char* pcBuffer, unsigned int uiBufSize) {
	UINT32			ui32Index;
	char*			pcCurrent = pcBuffer;
	unsigned int	uiRemain = uiBufSize;

	for ( ui32Index = 0; ui32Index < VDU_MAX_NUM_OF_INSTANCES; ++ui32Index ) {
		PERFORM_RESULT_T*	pstResult = &astResult[ui32Index];
		unsigned int		uiWritten = 0;

		if ( pstResult->bEnabled == FALSE ) {
			continue;
		}

		uiWritten = snprintf(pcCurrent, uiRemain,
							"[%d]%2d.%02d(%2d.%02d/%2d.%02d)", ui32Index,
							pstResult->ui32DecRate100 / 100,
							pstResult->ui32DecRate100 % 100,
							pstResult->ui32AvgHwDecTime100 / 100,
							pstResult->ui32AvgHwDecTime100 % 100,
							pstResult->ui32AvgDecTime100 / 100,
							pstResult->ui32AvgDecTime100 % 100);
		pcCurrent += uiWritten;
		uiRemain -= uiWritten;
	}

	return pcCurrent;
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
static void		_PrintDecRate(ULONG ulData)
{
	PERFOM_INFO_T*		pstPerformance = &_gstPerformance;
	PERFORM_RESULT_T	astResult[VDU_MAX_NUM_OF_INSTANCES] = { { 0, }, };
	char				acBuffer[100] = { 0, };
	char*				pcEnd = NULL;

	if ( (pstPerformance->bRun == TRUE) &&
		 logm_enabled(vdec_vdu, log_level_noti) ) {
		UINT64	ui64Timeout = get_jiffies_64() + msecs_to_jiffies(PERFORMANCE_PRINT_PERIOD);

		mod_timer(&pstPerformance->stTimer, ui64Timeout);
	}

	_CalcPerformance(pstPerformance, astResult);
	pcEnd = _PrintPerformance(astResult, acBuffer, 100);
	*pcEnd = '\0';

	logm_noti(vdec_vdu, "%s\n", acBuffer);
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
static void		_PrintResultInfo(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult)
{
	char acString[] = "/ Display: -1";

	if ( pstResult->si8FrmIndex < 0 ) {
		snprintf(acString, sizeof(acString), "");
	} else {
		snprintf(acString, sizeof(acString), "/ Display: %2d", pstResult->si8FrmIndex);
	}

	switch ( pstResult->eNotiType ) {
	case VDU_NOTI_SUCCESS:
		logm_info(vdec_vdu, "[%d]Success %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_DISPLAY:
		logm_info(vdec_vdu, "[%d]Only display %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_DECODING_ERROR:
		logm_error(vdec_vdu, "[%d]Unknown decoding error %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_CPB_EMPTY:
		logm_info(vdec_vdu, "[%d]CPB Empty %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_HEADER_NOT_FOUND:
		logm_noti(vdec_vdu, "[%d]Header not found %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_NOT_SUPPORTED:
		logm_noti(vdec_vdu, "[%d]SeqInit Fail %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_LACK_OF_RESOURCES:
		logm_noti(vdec_vdu, "[%d]Lack of resources %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_DPB_FULL:
		logm_info(vdec_vdu, "[%d]DPB Full %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_DPB_EMPTY:
		logm_info(vdec_vdu, "[%d]DPB Empty %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_SUSPEND:
		logm_info(vdec_vdu, "[%d]supend decoding %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_FRAME_SKIP:
		logm_info(vdec_vdu, "[%d]Frame skip %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_SKIP_EMPTY:
		logm_info(vdec_vdu, "[%d]Frame skip and CPB Empty %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_REQUEST_AU:
		logm_info(vdec_vdu, "[%d]Request Au %s\n", (UINT32)hInstance, acString);
		break;
	case VDU_NOTI_END_OF_STREAM:
		logm_info(vdec_vdu, "[%d]End of stream %s\n", (UINT32)hInstance, acString);
		break;
	default:
		logm_error(vdec_vdu, "[%d]Unknown notiType %d %s\n",(UINT32)hInstance,
								(SINT32)(pstResult->eNotiType), acString);
		break;
	}
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
static void		_PrintFrameInfo(VDU_HANDLE_T hInstance, VDU_FRM_INFO_T* pstFrameInfo)
{
	if ( pstFrameInfo->pstMapInfo == NULL ) {
		goto GOTO_END;
	}

	logm_debug(vdec_vdu, "[%d]%4d(%4d)*%4d|%2d/%2d/%2d/%2d|%d/%3d/%3d|%d|%4d/%4d(%d)|%2d/%d\n", (UINT32)hInstance,
							pstFrameInfo->stRect.ui32PicWidth, pstFrameInfo->stRect.ui32Stride, pstFrameInfo->stRect.ui32PicHeight,
							pstFrameInfo->stRect.ui16CropLeft, pstFrameInfo->stRect.ui16CropRight, pstFrameInfo->stRect.ui16CropTop, pstFrameInfo->stRect.ui16CropBottom,
							pstFrameInfo->stAspectRatio.ui8Mpeg2Dar, pstFrameInfo->stAspectRatio.ui16ParW, pstFrameInfo->stAspectRatio.ui16ParH,
							pstFrameInfo->stDispInfo.eFrameScanType,
							pstFrameInfo->stFrameRate.ui32Residual, pstFrameInfo->stFrameRate.ui32Divider, pstFrameInfo->stFrameRate.bVariable,
							pstFrameInfo->stDispInfo.si8FramePackArrange, pstFrameInfo->stDispInfo.eOrderOf3D);
	logm_debug(vdec_vdu, "%08X/%08X/%08X|%08X(%08X)|%X|%2d|%3d|%d|%d|%d|%d\n",
							pstFrameInfo->stAddress.ui32Y, pstFrameInfo->stAddress.ui32Cb, pstFrameInfo->stAddress.ui32Cr,
							pstFrameInfo->stUserData.ui32Address, pstFrameInfo->stUserData.ui32Size, pstFrameInfo->stUserData.ui8ActiveFmtDesc,
							pstFrameInfo->ui8NumOfFrames, pstFrameInfo->stDispInfo.ui8ErrorRate,
							pstFrameInfo->stDispInfo.bFieldPicture, pstFrameInfo->stDispInfo.ui8DisplayPeriod,
							pstFrameInfo->stDispInfo.ui8PictureType, pstFrameInfo->stDispInfo.bLowDelay);
	logm_debug(vdec_vdu, "%d|%08X|%d|%04X/%04X|%2d\n",
							pstFrameInfo->pstMapInfo->eFrameMapType, pstFrameInfo->pstMapInfo->ui32TiledBase,
							pstFrameInfo->pstMapInfo->eBalanceMode, pstFrameInfo->pstMapInfo->ui16M0Offset, pstFrameInfo->pstMapInfo->ui16M1Offset,
							pstFrameInfo->pstMapInfo->ui8FrameIndex);

GOTO_END:
	return;
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
static int		_ParamSetIntHook(const char *val, const struct kernel_param *kp)
{
	int		iRetVal = 0;

	iRetVal = param_set_int(val, kp);

	if ( (int*)(kp->arg) == &_gstStepDecInfo.iStepCount ) {
		UINT32		ui32Index;
		ULONG		ulFlags;

		spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
		if ( _gstStepDecInfo.iStepCount != 0 ) {
			for ( ui32Index = 0; ui32Index < VDU_MAX_NUM_OF_INSTANCES; ++ui32Index ) {
				if ( _gstStepDecInfo.bWaiting[ui32Index] == TRUE ) {
					spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);

					VDU_StartDecoding((VDU_HANDLE_T)ui32Index, NULL);

					spin_lock_irqsave(&_gstStepDecInfo.stLock, ulFlags);
				}
			}
		}
		spin_unlock_irqrestore(&_gstStepDecInfo.stLock, ulFlags);
	}

	return  iRetVal;
}
/** @} */

