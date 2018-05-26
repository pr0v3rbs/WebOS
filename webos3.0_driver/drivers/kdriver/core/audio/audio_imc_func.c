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
 * adec functions through IMC.
 * Communicate adec firmware through IMC instead registers.
 *
 * author     wonchang.shin (wonchang.shinu@lge.com)
 * version    1.0
 * date       2010.06.19
 *
 * @addtogroup lg1152_adec
 * @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>

#include "audio_drv.h"
#include "audio_drv_hal.h"

#include "audio_cfg.h"
#include "imc/adec_imc.h"
#include "ipc/adec_ipc.h"
#include "ipc/adec_ipc_dbg.h"
#include "common/adec_debug_index.h"
#include "audio_imc_func.h"
#include "audio_drv_manager.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
// TODO: check
#define AUDIO_IPC_MAGIC_CODE			0x01444542
#define AUDIO_IPC_BUF_SIZE				0x18400			// 97Kbytes
#define	AUDIO_IPC_DBG_MAGIC_CODE		0x19C0DB90


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#if 0
OS_SEM_T	g_AUDIO_IMC_Sema;		// IMC semaphore

#define AUDIO_IMC_LOCK_INIT()		OS_InitMutex(&g_AUDIO_IMC_Sema, OS_SEM_ATTR_DEFAULT)
#define AUDIO_IMC_LOCK()			OS_LockMutex(&g_AUDIO_IMC_Sema)
#define AUDIO_IMC_UNLOCK()			OS_UnlockMutex(&g_AUDIO_IMC_Sema)
#else
spinlock_t	g_AUDIO_IMC_Sema;		// IMC spin_lock

#define AUDIO_IMC_LOCK_INIT()		spin_lock_init(&g_AUDIO_IMC_Sema)
#define AUDIO_IMC_LOCK()			spin_lock(&g_AUDIO_IMC_Sema)
#define AUDIO_IMC_UNLOCK()			spin_unlock(&g_AUDIO_IMC_Sema)
#endif

#define AUDIO_IMC_WQ_LOCK_INIT()	OS_InitMutex(&g_AUDIO_IMC_WQ_Sema, OS_SEM_ATTR_DEFAULT)
#define AUDIO_IMC_WQ_LOCK()			OS_LockMutex(&g_AUDIO_IMC_WQ_Sema)
#define AUDIO_IMC_WQ_UNLOCK()		OS_UnlockMutex(&g_AUDIO_IMC_WQ_Sema)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
struct _AUDIO_IPC_INFO_T
{
	void	*pIpc;
	void	*pWriteOffset;
	void	*pReadOffset;
	UINT32	syncWord;
	UINT32	bufSize;
	UINT32	physicalAddr;
	void	*pVirtualAddr;
	IpcRole	role;
};

enum			// IPC list index
{
	IPC_WRITE_TO_DSP0	= 0,
	IPC_READ_FROM_DSP0	= 1,
	IPC_WRITE_TO_DSP1	= 2,
	IPC_READ_FROM_DSP1	= 3,
	IPC_LIST_MAX		= 4,
};

enum			// IPC Debug list index
{
	IPC_DBG_DSP0,
	IPC_DBG_DSP1,
	IPC_DBG_LIST_MAX,
};


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
UINT32		g_audModIpcPrint = 0;
UINT32		g_audIpcPrintCheckErrorFlag = 0;

extern UINT32	*g_pui32IPC_VirAddr;
extern IPC_RW_OFFSET_ADDR_T g_IPCRWOffset;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
OS_SEM_T	g_AUDIO_IMC_WQ_Sema;	// IMC WQ(WorkQueue) semaphore
OS_SEM_T	g_AUDIO_IPC_DEBUG_Sema;		// semaphore


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
// IMC workqueue
static void _AUDIO_IMC_ReceiveWorkQueue(struct work_struct *data);
DECLARE_WORK(_AUDIO_IMC_WORKQUEUE, _AUDIO_IMC_ReceiveWorkQueue);

// IMC Debug workqueue
static void _AUDIO_IPC_DBG_ReceiveWorkQueue(struct work_struct *data);
DECLARE_WORK(_AUDIO_IPC_DBG_WORKQUEUE, _AUDIO_IPC_DBG_ReceiveWorkQueue);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
// IMC workqueue
static struct workqueue_struct	*_gpIMCWorkQueue = NULL;

// IMC Debug workqueue
static struct workqueue_struct	*_gpIpcDbgWorkQueue = NULL;

void *_gpImc;	// static void *_gpImc;
static struct _AUDIO_IPC_INFO_T	_gIPCList[IPC_LIST_MAX];
static struct _AUDIO_IPC_INFO_T	_gIpcDbgList[IPC_DBG_LIST_MAX];


/*========================================================================================
	Implementation Group
========================================================================================*/
static void _IMC_Manager(void *_module, int _cmd, ImcSenderInfo *_sender, void *_param, int _paramLen)
{
	AUD_KDRV_IMC_DEBUG("ADEC_MODULE_MAN_ARM\n");
	AUD_KDRV_IMC_DEBUG("cmd = %d\n", _cmd);

	if (_sender != NULL)
	{
		AUD_KDRV_IMC_DEBUG("SENDER INFO] module=%d, core=%d, requestResponse=%d, id=%d, paramLen=%d\n",
			_sender->module, _sender->core, _sender->requesetReponse, _sender->id, _paramLen);
	}
}

static void _IMC_ResponseCB(int _iParam, void *_pParam, int _paramLen, void *_cbParam)
{
	AUD_KDRV_IMC_DEBUG("ADEC_MODULE_MAN_ARM\n");
	AUD_KDRV_IMC_DEBUG("iParam=%d, paramLen=%d\n", _iParam, _paramLen);
}

static void _IMC_ResponseInitCB(int _iParam, void *_pParam, int _paramLen, void *_cbParam)
{
	AUD_KDRV_IMC_DEBUG("Reponse of INIT CMD\n");
	AUD_KDRV_IMC_DEBUG("iParam=%d, paramLen=%d\n", _iParam, _paramLen);
}

static void _IMC_ResponseStartCB(int _iParam, void *_pParam, int _paramLen, void *_cbParam)
{
	AUD_KDRV_IMC_DEBUG("Reponse of START CMD\n");
	AUD_KDRV_IMC_DEBUG("iParam=%d, paramLen=%d\n", _iParam, _paramLen);
}

static void _IMC_ResponseStopCB(int _iParam, void *_pParam, int _paramLen, void *_cbParam)
{
	AUD_KDRV_IMC_DEBUG("Reponse of STOP CMD\n");
	AUD_KDRV_IMC_DEBUG("iParam=%d, paramLen=%d\n", _iParam, _paramLen);
}

static void _IMC_ResponseFlushCB(int _iParam, void *_pParam, int _paramLen, void *_cbParam)
{
	AUD_KDRV_IMC_DEBUG("Reponse of FLUSH CMD\n");
	AUD_KDRV_IMC_DEBUG("iParam=%d, paramLen=%d\n", _iParam, _paramLen);
}

static void _IMC_SendIntrruptToDsp0(void *_param)
{
	AUD_KDRV_IMC_DEBUG("ADEC_MODULE_MAN_ARM\n");

	(void)AUDIO_WriteReg(LX_AUD_REG_DSP0_IRQ, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_DSP0_IRQ, 0x0);

	return;
}

static void _IMC_SendIntrruptToDsp1(void *_param)
{
	AUD_KDRV_IMC_DEBUG("ADEC_MODULE_MAN_ARM\n");

	(void)AUDIO_WriteReg(LX_AUD_REG_DSP1_IRQ, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_DSP1_IRQ, 0x0);

	return;
}

// IMC workqueue
static void _AUDIO_IMC_ReceiveWorkQueue(struct work_struct *data)
{
	//Lock a AUDIO IMC Workqueue Process Semaphore, Do not use spin lock to avoid recursive call.
	AUDIO_IMC_WQ_LOCK();

	IMC_Process(_gpImc);

	//Unlock a AUDIO IMC Workqueue Process Semaphore
	AUDIO_IMC_WQ_UNLOCK();

	return;
}

static SINT32 _IMC_SetLocalImc(void *_imc)
{
	_gpImc = _imc;

	return ADEC_ERR_NONE;
}

static void* _IMC_GetLocalImc(	void)
{
	return _gpImc;
}

// IMC ISR Function
void AUDIO_IMC_Process(void)
{
	queue_work(_gpIMCWorkQueue, &_AUDIO_IMC_WORKQUEUE);

	return;
}

SINT32 AUDIO_IMC_Init(void)
{
	SINT32	result;
	void*	moduleParam = NULL;
	void*	Param = NULL;
	UINT32	ipcIndex;

	//Check initialization
	if(_gpIMCWorkQueue != NULL)
	{
		AUDIO_IMC_WQ_LOCK();
		AUDIO_IMC_LOCK();
	}

	// IPC
	// DSP0 - Write: IPC_WRITE_TO_DSP0
	_gIPCList[IPC_WRITE_TO_DSP0].pIpc		 		= NULL,

	_gIPCList[IPC_WRITE_TO_DSP0].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_a2d_wptr,
	_gIPCList[IPC_WRITE_TO_DSP0].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_a2d_rptr,
	_gIPCList[IPC_WRITE_TO_DSP0].syncWord			= AUDIO_IPC_MAGIC_CODE,
	_gIPCList[IPC_WRITE_TO_DSP0].bufSize			= AUDIO_IPC_BUF_SIZE,
	_gIPCList[IPC_WRITE_TO_DSP0].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 0),
	_gIPCList[IPC_WRITE_TO_DSP0].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + (AUDIO_IPC_BUF_SIZE * 0)),
	_gIPCList[IPC_WRITE_TO_DSP0].role				= IPC_ROLE_WRITER,

	// DSP0 - Read:  IPC_READ_FROM_DSP0
	_gIPCList[IPC_READ_FROM_DSP0].pIpc		 		= NULL,
	_gIPCList[IPC_READ_FROM_DSP0].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_d2a_wptr,
	_gIPCList[IPC_READ_FROM_DSP0].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_d2a_rptr,
	_gIPCList[IPC_READ_FROM_DSP0].syncWord			= AUDIO_IPC_MAGIC_CODE,
	_gIPCList[IPC_READ_FROM_DSP0].bufSize			= AUDIO_IPC_BUF_SIZE,
	_gIPCList[IPC_READ_FROM_DSP0].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 1),
	_gIPCList[IPC_READ_FROM_DSP0].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + ((AUDIO_IPC_BUF_SIZE * 1) >> 2)),
	_gIPCList[IPC_READ_FROM_DSP0].role				= IPC_ROLE_READER,

	// DSP1 - Write: IPC_WRITE_TO_DSP1
	_gIPCList[IPC_WRITE_TO_DSP1].pIpc		 		= NULL,
	_gIPCList[IPC_WRITE_TO_DSP1].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_a2p_wptr,
	_gIPCList[IPC_WRITE_TO_DSP1].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_a2p_rptr,
	_gIPCList[IPC_WRITE_TO_DSP1].syncWord			= AUDIO_IPC_MAGIC_CODE,
	_gIPCList[IPC_WRITE_TO_DSP1].bufSize			= AUDIO_IPC_BUF_SIZE,
	_gIPCList[IPC_WRITE_TO_DSP1].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 2),
	_gIPCList[IPC_WRITE_TO_DSP1].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + ((AUDIO_IPC_BUF_SIZE * 2) >> 2)),
	_gIPCList[IPC_WRITE_TO_DSP1].role				= IPC_ROLE_WRITER,

	// DSP1 - Read: IPC_READ_FROM_DSP1
	_gIPCList[IPC_READ_FROM_DSP1].pIpc		 		= NULL,
	_gIPCList[IPC_READ_FROM_DSP1].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_p2a_wptr,
	_gIPCList[IPC_READ_FROM_DSP1].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_p2a_rptr,
	_gIPCList[IPC_READ_FROM_DSP1].syncWord			= AUDIO_IPC_MAGIC_CODE,
	_gIPCList[IPC_READ_FROM_DSP1].bufSize			= AUDIO_IPC_BUF_SIZE,
	_gIPCList[IPC_READ_FROM_DSP1].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 3),
	_gIPCList[IPC_READ_FROM_DSP1].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + ((AUDIO_IPC_BUF_SIZE * 3) >> 2)),
	_gIPCList[IPC_READ_FROM_DSP1].role				= IPC_ROLE_READER,

	// malloc
	_gpImc				= kmalloc(IMC_GetStructSize(), GFP_KERNEL);

	for (ipcIndex = 0; ipcIndex < IPC_LIST_MAX; ipcIndex++)
	{
		_gIPCList[ipcIndex].pIpc = kmalloc(IPC_GetStructSize(), GFP_KERNEL);
	}

	// IMC
	result = IMC_Init(_gpImc, ADEC_CORE_ARM);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_Init() Error!!!\n");
		goto IMC_INIT_ERROR;
	}

	_IMC_SetLocalImc(_gpImc);

	// init IPC
	for (ipcIndex = 0; ipcIndex < IPC_LIST_MAX; ipcIndex++)
	{
		result = IPC_Init(_gIPCList[ipcIndex].pIpc,
							_gIPCList[ipcIndex].pWriteOffset,
							_gIPCList[ipcIndex].pReadOffset,
							_gIPCList[ipcIndex].syncWord,
							_gIPCList[ipcIndex].bufSize,
							(void *)&_gIPCList[ipcIndex].physicalAddr,
							_gIPCList[ipcIndex].pVirtualAddr,
							_gIPCList[ipcIndex].role);
		if (result != 0)
		{
			AUD_KDRV_ERROR("IPC_Init() Error - index=%u !!!\n", ipcIndex);
			goto IMC_INIT_ERROR;
		}
		// clean IPC read/write pointer
		(void)IPC_ClearRegister(_gIPCList[ipcIndex].pIpc, 1);
	}

	// set target - DSP0
	result = IMC_SetTargetWriter(_gpImc, ADEC_CORE_DSP0, _gIPCList[IPC_WRITE_TO_DSP0].pIpc);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SetTargetWriter() Error!!!\n");
		goto IMC_INIT_ERROR;
	}
	result = IMC_SetTargetReader(_gpImc, ADEC_CORE_DSP0, _gIPCList[IPC_READ_FROM_DSP0].pIpc);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SetTargetReader() Error!!!\n");
		goto IMC_INIT_ERROR;
	}

	// set target - DSP1
	result = IMC_SetTargetWriter(_gpImc, ADEC_CORE_DSP1, _gIPCList[IPC_WRITE_TO_DSP1].pIpc);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SetTargetWriter() Error!!!\n");
		goto IMC_INIT_ERROR;
	}
	result = IMC_SetTargetReader(_gpImc, ADEC_CORE_DSP1, _gIPCList[IPC_READ_FROM_DSP1].pIpc);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SetTargetReader() Error!!!\n");
		goto IMC_INIT_ERROR;
	}

	// register command process
	result = IMC_RegisterCmdProcessor(_gpImc, ADEC_MODULE_MAN_ARM, _IMC_Manager, moduleParam);
		// 여기서 등록한 것은 IMC_Process() 함수가 호출되면 호출된다.
		// IMC_Process() 함수는 어디서 호출할 것인가?
		// Remote IMC를 처리한다.
		// 이 함수는 IPC 인터럽트가 발생하였을 때 호출하면 된다.
		// 내부적으로는 등록된 IMC_SetTargetReader함수를 이용하여 등록된 IPC Reader에 대하여
		// IMC_ReadFromIpc를 수행한다.
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_RegisterCmdProcessor() Error!!!\n");
		goto IMC_INIT_ERROR;
	}

	// register command interrupt
	result = IPC_SetInterrupt(_gIPCList[IPC_WRITE_TO_DSP0].pIpc, _IMC_SendIntrruptToDsp0, Param);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IPC_SetInterrupt() Error!!!\n");
		goto IMC_INIT_ERROR;
	}
	result = IPC_SetInterrupt(_gIPCList[IPC_WRITE_TO_DSP1].pIpc, _IMC_SendIntrruptToDsp1, Param);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IPC_SetInterrupt() Error!!!\n");
		goto IMC_INIT_ERROR;
	}

	/*
	*	Reader로 등록된 IPC의 interrupt handler에서 IMC_Process()를 호출하면 IMC_RegisterCmdProcessor() 함수에서
	*	등록한 manager가 호출된다.
	*	즉, IPC interrupt handler에서 IMC_Process()를 호출하면 된다?
	*	IPC의 interrupt handler가 간단히 동작하도록 하여야 하는데...
	IMC_Process(_gpImc);
	*/

	//Check initialization
	if(_gpIMCWorkQueue != NULL)
	{
		AUDIO_IMC_UNLOCK();
		AUDIO_IMC_WQ_UNLOCK();
	}

	// Initialize workqueue
	if(_gpIMCWorkQueue == NULL)
	{
		_gpIMCWorkQueue = create_workqueue("AUDIO_IMC_WORKQUEUE");

		//Initialize AUDIO IMC semaphore
		AUDIO_IMC_LOCK_INIT();

		//Initialize AUDIO IMC Process semaphore
		AUDIO_IMC_WQ_LOCK_INIT();
	}

	return RET_OK;

IMC_INIT_ERROR:
	kfree(_gpImc);

	for (ipcIndex = 0; ipcIndex < IPC_LIST_MAX; ipcIndex++)
	{
		kfree(_gIPCList[ipcIndex].pIpc);
	}

	//Check initialization
	if(_gpIMCWorkQueue != NULL)
	{
		AUDIO_IMC_UNLOCK();
		AUDIO_IMC_WQ_UNLOCK();
	}

	return RET_ERROR;
}

void AUDIO_IMC_Finalize(void)
{
	UINT32 ipcIndex;

	AUDIO_IMC_LOCK();

	IMC_Finalize(_gpImc);

	kfree(_gpImc);
	for (ipcIndex = 0; ipcIndex < IPC_LIST_MAX; ipcIndex++)
	{
		kfree(_gIPCList[ipcIndex].pIpc);
	}

	AUDIO_IMC_UNLOCK();

	return;
}

SINT32 AUDIO_IMC_SendCmd(int cmd, ADEC_MODULE_ID targetModule)
{
	int result;
	ImcCommandParameter *imcCmdParam;
	int responseID;
	AUD_MOD_INFO_T *pModInfo;

	//Check a ADEC module infomation.
	pModInfo = AUDIO_GetModuleInfo(targetModule);
	if(pModInfo == NULL)
	{
		AUD_KDRV_ERROR("Check ARG : targetModule[%d], pMod[%p] \n", targetModule, pModInfo);
		return RET_ERROR;
	}

	//Check a BYPASS Module not to send init command
	if( ((targetModule >= ADEC_MODULE_BYPASS_D0) && (targetModule <= ADEC_MODULE_BYPASS_P1))	\
	  &&(cmd == ADEC_CMD_INIT) )
	{
		AUD_KDRV_IPC_DEBUG("ADEC_CMD_INIT for BYPASS module(targetModule = %d)\n", targetModule);
		return RET_OK;
	}

	AUDIO_IMC_LOCK();

	imcCmdParam = (ImcCommandParameter *)kmalloc(sizeof(ImcCommandParameter), GFP_KERNEL);
	if(imcCmdParam == NULL)
	{
		AUD_KDRV_ERROR("kmalloc Err!!!\n");

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	imcCmdParam->cmd				= cmd;					// ADEC_CMD_START;
	imcCmdParam->target				= targetModule;			// ADEC_MODULE_MAN_DSP1;
	imcCmdParam->source				= ADEC_MODULE_MAN_ARM;
	//imcCmdParam->requestResponse	= IMC_RESPONSE_REQUIRED;
	imcCmdParam->requestResponse	= IMC_RESPONSE_NOT_REQUIRED;
	imcCmdParam->requestInterrupt	= IMC_INTERRUPT_REQUIRED;

	switch (cmd)
	{
		case ADEC_CMD_INIT:
			imcCmdParam->callback	= _IMC_ResponseInitCB;
			pModInfo->status = AUD_STATUS_INIT;
			break;

		case ADEC_CMD_START:
			imcCmdParam->callback	= _IMC_ResponseStartCB;
			pModInfo->status = AUD_STATUS_START;
			break;

		case ADEC_CMD_STOP:
			imcCmdParam->callback	= _IMC_ResponseStopCB;
			pModInfo->status = AUD_STATUS_STOP;
			break;

		case ADEC_CMD_FLUSH:
			imcCmdParam->callback	= _IMC_ResponseFlushCB;
			pModInfo->status = AUD_STATUS_FLUSH;
			break;

		default:
			imcCmdParam->callback	= _IMC_ResponseCB;
			break;
	}

	imcCmdParam->callbackParam		= NULL;
	imcCmdParam->paramLength		= 0;
	imcCmdParam->param				= NULL;

	// TODO: manage responseID
	result = IMC_SendCommand(_gpImc, (ImcCommandParameter*)imcCmdParam, &responseID);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SendCommand(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);
		kfree(imcCmdParam);

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	kfree(imcCmdParam);
	AUDIO_IMC_UNLOCK();

	AUD_KDRV_IMC_COM("(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);
	return RET_OK;
}

SINT32 AUDIO_IMC_SendCmdEx(int cmd, ADEC_MODULE_ID targetModule, PFN_ImcResponse cb, void *cbParam)
{
	int result;
	ImcCommandParameter imcCmdParam;
	int responseID;

	AUDIO_IMC_LOCK();

	imcCmdParam.cmd					= cmd;
	imcCmdParam.target				= targetModule;
	imcCmdParam.source				= ADEC_MODULE_MAN_ARM;
	if (cb != NULL)
		imcCmdParam.requestResponse	= IMC_RESPONSE_REQUIRED;
	else
		imcCmdParam.requestResponse	= IMC_RESPONSE_NOT_REQUIRED;
	imcCmdParam.requestInterrupt	= IMC_INTERRUPT_REQUIRED;
	imcCmdParam.callback			= cb,
	imcCmdParam.callbackParam		= cbParam;
	imcCmdParam.paramLength			= 0;
	imcCmdParam.param				= NULL;

	// TODO: manage responseID
	result = IMC_SendCommand(_gpImc, &imcCmdParam, &responseID);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SendCommand(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	AUDIO_IMC_UNLOCK();

	AUD_KDRV_IMC_COM("(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n",cmd, targetModule, result);
	return RET_OK;
}


SINT32 AUDIO_IMC_SendCmdParam(int cmd, ADEC_MODULE_ID targetModule, unsigned int bodySize, void *pParam)
{
	int result;
	ImcCommandParameter *imcCmdParam;
	int responseID;

	AUDIO_IMC_LOCK();

	imcCmdParam = (ImcCommandParameter *)kmalloc(sizeof(ImcCommandParameter), GFP_KERNEL);
	if (imcCmdParam == NULL)
	{
		AUD_KDRV_ERROR("kmalloc Err!!!\n");

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	imcCmdParam->cmd				= cmd;					// ADEC_CMD_START;
	imcCmdParam->target				= targetModule;			// ADEC_MODULE_MAN_DSP1;
	imcCmdParam->source				= ADEC_MODULE_MAN_ARM;
	//imcCmdParam->requestResponse	= IMC_RESPONSE_REQUIRED;
	imcCmdParam->requestResponse	= IMC_RESPONSE_NOT_REQUIRED;
	imcCmdParam->requestInterrupt	= IMC_INTERRUPT_REQUIRED;
	switch (cmd)
	{
		case ADEC_CMD_START:
			imcCmdParam->callback	= _IMC_ResponseStartCB;
			break;
		case ADEC_CMD_STOP:
			imcCmdParam->callback	= _IMC_ResponseStopCB;
			break;
		case ADEC_CMD_FLUSH:
			imcCmdParam->callback	= _IMC_ResponseFlushCB;
			break;
		default:
			imcCmdParam->callback	= _IMC_ResponseCB;
			break;
	}
	imcCmdParam->callbackParam		= NULL;
	imcCmdParam->paramLength		= bodySize;
	imcCmdParam->param				= pParam;

	// TODO: manage responseID
	result = IMC_SendCommand(_gpImc, (ImcCommandParameter*)imcCmdParam, &responseID);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SendCommand(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);
		kfree(imcCmdParam);

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	kfree(imcCmdParam);

	AUDIO_IMC_UNLOCK();

	AUD_KDRV_IMC_COM("(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);

	return RET_OK;
}

SINT32 AUDIO_IMC_SendCmdRsp(int cmd, ADEC_MODULE_ID targetModule, unsigned int bodySize, void *pParam,
							PFN_ImcResponse rspCb, void *pCbParam)
{
	int result;
	ImcCommandParameter *imcCmdParam;
	int responseID;

	AUDIO_IMC_LOCK();

	imcCmdParam = (ImcCommandParameter *)kmalloc(sizeof(ImcCommandParameter), GFP_KERNEL);
	if (imcCmdParam == NULL)
	{
		AUD_KDRV_ERROR("kmalloc Err!!!\n");

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	imcCmdParam->cmd				= cmd;					// ADEC_CMD_START;
	imcCmdParam->target				= targetModule;			// ADEC_MODULE_MAN_DSP1;
	imcCmdParam->source				= ADEC_MODULE_MAN_ARM;
	imcCmdParam->requestResponse	= IMC_RESPONSE_REQUIRED;
	imcCmdParam->requestInterrupt	= IMC_INTERRUPT_REQUIRED;
	imcCmdParam->callback			= rspCb;
	imcCmdParam->callbackParam		= pCbParam;
	imcCmdParam->paramLength		= bodySize;
	imcCmdParam->param				= pParam;

	// TODO: manage responseID
	result = IMC_SendCommand(_gpImc, (ImcCommandParameter*)imcCmdParam, &responseID);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IMC_SendCommand(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);
		kfree(imcCmdParam);

		AUDIO_IMC_UNLOCK();
		return RET_ERROR;
	}

	kfree(imcCmdParam);

	AUDIO_IMC_UNLOCK();

	AUD_KDRV_IMC_COM("(cmd=0x%08X, targetModule=%d) Error(result=%d)!!!\n", cmd, targetModule, result);

	return RET_OK;
}

UINT32 AUDIO_IMC_RegisterEvent(void* pDevInfo, PFN_ImcNoti pFuncImcNoti, ADEC_MODULE_ID modID, UINT32 event,
						IMC_ACTION_REPEAT_TYPE repeatType, SINT32 notiLevel)
{
	UINT32			actionID;
	ImcActionParameter 	actionParam;
	ADEC_RESULT		retVal = ADEC_ERR_NONE;

	actionParam.actionType = IMC_ACTION_GET_CALLBACK;
	actionParam.repeatType = repeatType;
	actionParam.target = ADEC_MODULE_MAN_ARM;
	actionParam.actionParam.notiParam.noti = (PFN_ImcNoti)pFuncImcNoti;
	actionParam.actionParam.notiParam.param = pDevInfo;
	actionParam.actionParam.notiParam.level = notiLevel;
	retVal = IMC_RegisterEvent(_IMC_GetLocalImc(), event, modID, &actionID, &actionParam);
	if(retVal != ADEC_ERR_NONE)
	{
		AUD_KDRV_ERROR("IMC_RegisterEvent : [%d]%s\n", retVal, ADEC_GetErrorString(retVal));
	}

	return actionID;
}

void AUDIO_IMC_CancelEvent(UINT32 event , ADEC_MODULE_ID moduleID, UINT32 actionID)
{
	ADEC_RESULT		retVal = ADEC_ERR_NONE;

	retVal = IMC_CancelEvent(_gpImc, event , moduleID, actionID);
	if(retVal != ADEC_ERR_NONE)
	{
		AUD_KDRV_ERROR("IMC_CancelEvent : [%d]%s\n", retVal, ADEC_GetErrorString(retVal));
	}

	return;
}

//For DSP Debug Print Service
static void _IPC_DBG_SendIntrruptToDsp0(void *_param)
{
	AUD_KDRV_IPC_DEBUG("ADEC_MODULE_MAN_ARM\n");

	(void)AUDIO_WriteReg(LX_AUD_REG_DSP0_IRQ, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_DSP0_IRQ, 0x0);

	return;
}

static void _IPC_DBG_SendIntrruptToDsp1(void *_param)
{
	AUD_KDRV_IPC_DEBUG("ADEC_MODULE_MAN_ARM\n");

	(void)AUDIO_WriteReg(LX_AUD_REG_DSP1_IRQ, 0x1);
	(void)AUDIO_WriteReg(LX_AUD_REG_DSP1_IRQ, 0x0);

	return;
}

static void _IPC_PrintFuncDSP0(unsigned int _timeStamp, int _module, IPC_DEBUG_TYPE _flag, char *_string)
{
	// TODO : impelement local mask (_module, _flag)
	// Detect the rate of the message, if the rate is too high then disalbe message.
	// TODO : change the debug function
//	if(_module & g_audModIpcPrint)
	if(g_audModIpcPrint)
	{
		if (_flag == IPC_DEBUG_ERROR)
		{
			AUD_KDRV_IPC_DBG_ERR("[%06X][%7s] %s",
				_timeStamp,
				DbgIndex_GetIndexName(_module),
				_string);
 		}
		else
		{
			if(g_audIpcPrintCheckErrorFlag == FALSE)
			{
				AUD_KDRV_IPC_MSG0("[%06X][%7s] %s",
					_timeStamp,
					DbgIndex_GetIndexName(_module),
					_string);
			}
		}
	}

	return;
}

static void _IPC_PrintFuncDSP1(unsigned int _timeStamp, int _module, IPC_DEBUG_TYPE _flag, char *_string)
{
	// TODO : impelement local mask (_module, _flag)
	// Detect the rate of the message, if the rate is too high then disalbe message.
	// TODO : change the debug function
//	if(_module & g_audModIpcPrint)
	if(g_audModIpcPrint)
	{
		if (_flag == IPC_DEBUG_ERROR)
		{
			AUD_KDRV_IPC_DBG_ERR("[%06X][%7s] %s",
				_timeStamp,
				DbgIndex_GetIndexName(_module),
				_string);
 		}
		else
		{
			if(g_audIpcPrintCheckErrorFlag == FALSE)
			{
				AUD_KDRV_IPC_MSG1("[%06X][%7s] %s",
					_timeStamp,
					DbgIndex_GetIndexName(_module),
					_string);
			}
		}
	}

	return;
}

ADEC_RESULT _IPC_DebugProcess(void* _ipc, PFN_IpcDbgPrint _dbgPrint)
{
	ADEC_RESULT result;
	ADEC_ASSERT( _ipc!= NULL, return ADEC_ERR_NULL);

	do
	{
		result = IPC_ReadDebug(_ipc, _dbgPrint);
	}while(result == ADEC_ERR_NONE);

	if (result != ADEC_ERR_UNDERFLOW)
	{
		AUD_KDRV_ERROR("IPC DBG Result : [%d]%s\n", result, ADEC_GetErrorString(result));
		if (result == ADEC_ERR_DATA_CORRUPTED)
		{
			IPC_ClearRegister(_ipc, 1);
		}
	}

	return ADEC_ERR_NONE;
}

// IMC Debug workqueue
static void _AUDIO_IPC_DBG_ReceiveWorkQueue(struct work_struct *data)
{
	//Lock a AUDIO IPC Debug Workqueue Semaphore, Do not use spin lock to avoid recursive call.
	AUDIO_IMC_WQ_LOCK();

	_IPC_DebugProcess(_gIpcDbgList[IPC_DBG_DSP0].pIpc, _IPC_PrintFuncDSP0);
	_IPC_DebugProcess(_gIpcDbgList[IPC_DBG_DSP1].pIpc, _IPC_PrintFuncDSP1);

	//Unlock a AUDIO IPC Debug Workqueue Semaphore
	AUDIO_IMC_WQ_UNLOCK();

	return;
}

void AUDIO_IPC_ProcessDebug(void)
{
	queue_work(_gpIpcDbgWorkQueue, &_AUDIO_IPC_DBG_WORKQUEUE);

	return;
}

SINT32 AUDIO_IPC_DBG_Init(void)
{
	SINT32	result;
	void*	Param = NULL;
	UINT32	ipcDbgIndex;

	//Check initialization
	if(_gpIpcDbgWorkQueue != NULL)
	{
		AUDIO_IMC_WQ_LOCK();
	}

	_gIpcDbgList[IPC_DBG_DSP0].pIpc		 		= NULL;
	_gIpcDbgList[IPC_DBG_DSP0].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_dbgd_wptr;
	_gIpcDbgList[IPC_DBG_DSP0].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_dbgd_rptr;
	_gIpcDbgList[IPC_DBG_DSP0].syncWord			= AUDIO_IPC_DBG_MAGIC_CODE,
	_gIpcDbgList[IPC_DBG_DSP0].bufSize			= AUDIO_IPC_BUF_SIZE << 1,
	_gIpcDbgList[IPC_DBG_DSP0].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 6);
	_gIpcDbgList[IPC_DBG_DSP0].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + ((AUDIO_IPC_BUF_SIZE * 6)>>2));
	_gIpcDbgList[IPC_DBG_DSP0].role				= IPC_ROLE_DBG_READER;

	_gIpcDbgList[IPC_DBG_DSP1].pIpc		 		= NULL;
	_gIpcDbgList[IPC_DBG_DSP1].pWriteOffset		= (void *)g_IPCRWOffset.p_aud_ipc_dbgp_wptr;
	_gIpcDbgList[IPC_DBG_DSP1].pReadOffset		= (void *)g_IPCRWOffset.p_aud_ipc_dbgp_rptr;
	_gIpcDbgList[IPC_DBG_DSP1].syncWord			= AUDIO_IPC_DBG_MAGIC_CODE;
	_gIpcDbgList[IPC_DBG_DSP1].bufSize			= AUDIO_IPC_BUF_SIZE << 1;
	_gIpcDbgList[IPC_DBG_DSP1].physicalAddr		= pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 8);
	_gIpcDbgList[IPC_DBG_DSP1].pVirtualAddr		= (void *)(g_pui32IPC_VirAddr + ((AUDIO_IPC_BUF_SIZE * 8)>>2));
	_gIpcDbgList[IPC_DBG_DSP1].role				= IPC_ROLE_DBG_READER;

	// kmalloc
	for (ipcDbgIndex = 0; ipcDbgIndex < IPC_DBG_LIST_MAX; ipcDbgIndex++)
	{
		_gIpcDbgList[ipcDbgIndex].pIpc = kmalloc(IPC_GetStructSize(), GFP_KERNEL);

		result = IPC_Init(_gIpcDbgList[ipcDbgIndex].pIpc,
							_gIpcDbgList[ipcDbgIndex].pWriteOffset,
							_gIpcDbgList[ipcDbgIndex].pReadOffset,
							_gIpcDbgList[ipcDbgIndex].syncWord,
							_gIpcDbgList[ipcDbgIndex].bufSize,
							(void *)&_gIpcDbgList[ipcDbgIndex].physicalAddr,
							_gIpcDbgList[ipcDbgIndex].pVirtualAddr,
							_gIpcDbgList[ipcDbgIndex].role);
		if (result != 0)
		{
			AUD_KDRV_ERROR("IPC_Init() Error - index=%u !!!\n", ipcDbgIndex);
			goto IPC_INIT_ERROR;
		}
		// clean IPC read/write pointer
		(void)IPC_ClearRegister(_gIpcDbgList[ipcDbgIndex].pIpc, 1);
	}

	// register command interrupt
	result = IPC_SetInterrupt(_gIpcDbgList[IPC_DBG_DSP0].pIpc, _IPC_DBG_SendIntrruptToDsp0, Param);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IPC_SetInterrupt() Error!!!\n");
		goto IPC_INIT_ERROR;
	}
	result = IPC_SetInterrupt(_gIpcDbgList[IPC_DBG_DSP1].pIpc, _IPC_DBG_SendIntrruptToDsp1, Param);
	if (result != 0)
	{
		AUD_KDRV_ERROR("IPC_SetInterrupt() Error!!!\n");
		goto IPC_INIT_ERROR;
	}

	//Check initialization
	if(_gpIpcDbgWorkQueue != NULL)
	{
		AUDIO_IMC_WQ_UNLOCK();
	}

	// workqueue
	if(_gpIpcDbgWorkQueue == NULL)
	{
		_gpIpcDbgWorkQueue = create_workqueue("AUDIO_IPC_DBG_WORKQUEUE");
	}

	return RET_OK;

IPC_INIT_ERROR:

	for (ipcDbgIndex = 0; ipcDbgIndex < IPC_DBG_LIST_MAX; ipcDbgIndex++)
	{
		kfree(_gIpcDbgList[ipcDbgIndex].pIpc);
	}

	//Check initialization
	if(_gpIpcDbgWorkQueue != NULL)
	{
		AUDIO_IMC_WQ_UNLOCK();
	}

	return RET_ERROR;
}


void AUDIO_IPC_DBG_Finalize(void)
{
	UINT32 ipcDbgIndex;

	AUDIO_IMC_WQ_LOCK();

	for (ipcDbgIndex = 0; ipcDbgIndex < IPC_DBG_LIST_MAX; ipcDbgIndex++)
	{
		kfree(_gIpcDbgList[ipcDbgIndex].pIpc);
	}

	AUDIO_IMC_WQ_UNLOCK();

	return;
}

#if 0
void _AUDIO_RegisterDecoderNoti(AUD_DECODER_INFO_T* pDecInfo, PFN_ImcNoti pFuncImcNoti, UINT32 allocMod,	\
									   UINT32 event, IMC_ACTION_REPEAT_TYPE repeatType, SINT32 notiLevel)
{
	UINT32							actionID;
	AUD_EVENT_T						*pDecEvent 		= NULL;
	ImcActionParameter 				actionParam;

	if(pDecInfo->ui32EventNum >= AUD_EVENT_NUM)
	{
		AUD_KDRV_ERROR("DecEventNum(%d) is over AUD_EVENT_NUM. \n", pDecInfo->ui32EventNum);
		return;
	}

	actionParam.actionType = IMC_ACTION_GET_CALLBACK;
	actionParam.repeatType = repeatType;
	actionParam.target = ADEC_MODULE_MAN_ARM;
	actionParam.actionParam.notiParam.noti = (PFN_ImcNoti)pFuncImcNoti;
	actionParam.actionParam.notiParam.param = pDecInfo;
	actionParam.actionParam.notiParam.level = notiLevel;
	IMC_RegisterEvent(IMC_GetLocalImc(0), event, allocMod, &actionID, &actionParam);

	//if(repeatType != IMC_ACTION_ONCE)
	{
		pDecEvent = &pDecInfo->decEvent[pDecInfo->ui32EventNum];
		pDecEvent->event = event;
		pDecEvent->actionID = actionID;
		pDecEvent->moduleID = allocMod;
		pDecInfo->ui32EventNum++;
	}

	AUD_KDRV_PRINT("ev(0x%x) actID(0x%x), mod(0x%x).\n", event, actionID, allocMod);

	return;
}
#endif

