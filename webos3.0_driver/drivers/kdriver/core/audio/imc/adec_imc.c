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



/**
 *	@file adec_imc.c
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-22
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC
 *	@{
 */
#include "adec_imc.h"
#include "adec_imc_debug.h"
#include "adec_imc_cmd.h"
#include "adec_imc_tracer.h"

#include "common/adec_common.h"
#include "module/adec_module_list.h"
#include "ipc/adec_ipc.h"
#include "util/adec_rbtree.h"

/******************************************************************************
  Local Type Definitions
 ******************************************************************************/
typedef struct _ImcNotiInfo
{
	IMC_ACTION_REPEAT_TYPE repeatType;
	PFN_ImcNoti			notiFunc;
	void*				notiParam;
	unsigned int		notiID;
}ImcNotiInfo;

typedef struct _ImcResponseInfo
{
	long long			insertTime;
	PFN_ImcResponse		responseFunc;
	void*				responseParam;
	unsigned int		responseID;
}ImcResponseInfo;

typedef struct _ImcCmdProcInfo
{
	PFN_ImcCmdProc		proc;
	void*				moduleParam;

}ImcCmdProcInfo;

typedef struct _ImcActionInfo
{
	ImcActionParameter		actionParam;
	unsigned int			actionID;
}ImcActionInfo;


#define IMC_ACTION_ID(__INDEX, __CORE) (((__CORE&0x0F) << 20) | (__INDEX & 0xFFFFF))
#define IMC_EVENT_ID(__EVENT_ID, __OWNER) (((__EVENT_ID & 0xFFFFFF) << 8) | (__OWNER & 0xFF))
#define IMC_OPTION_FLAG(__RSP, __ACT, __RPT) \
	(((__RSP & 0x1) )|\
	 ((__ACT & 0x1) << 1) |\
	 ((__RPT & 0x1) << 2))
#define IMC_GET_OPT_RSP(__OPT)	(__OPT & 0x01)
#define IMC_GET_OPT_ACT(__OPT)	((__OPT >> 1) & 0x01)
#define IMC_GET_OPT_RPT(__OPT)	((__OPT >> 2) & 0x01)

#define IMC_CMD_DEBUG_FLAG			0x80
#define IMC_CMD_COMMAND_FLAG		0x40
#define IMC_CMD_RESPONSE_FLAG		0x20
#define IMC_CMD_REGISTER_FLAG		0x10
#define IMC_CMD_CANCEL_FLAG			0x08
#define IMC_CMD_NOTIFICATION_FLAG	0x04


/******************************************************************************
  Static Variable Declarations
 ******************************************************************************/
typedef struct _ImcInfo
{
	ImcNotiInfo		imcNotiArray[ADEC_CNST_IMC_MAX_NOTI_COUNT];
	ImcResponseInfo imcResponseArray[ADEC_CNST_IMC_MAX_RESPONSE_COUNT];
	ImcCmdProcInfo	imcCmdProcArray[ADEC_MODULE_MAX];
	void*			imcEventTree;
	void*			ipcWriter[ADEC_CORE_MAX];
	void*			ipcReader[ADEC_CORE_MAX];
	unsigned int	imcIndex;
	unsigned int	imcActionIndex;
	unsigned int	imcResponseCount;
	unsigned int	imcNotiCount;
	ADEC_CORE_TYPE	core;
    PFN_ImcCmdProcNotDef    defaultProc;
    void*           defaultProcParam;
    void*           imcTracer;
}ImcInfo;

typedef struct _ImcEventInfo
{
	void*			eventParam;
	int				eventLength;
	ImcActionInfo	imcActionArray[ADEC_CNST_IMC_MAX_ACTION_COUNT];
	int             actionCount;
}ImcEventInfo;

/******************************************************************************
  Static Variable Declarations
 ******************************************************************************/
ADEC_MUTEX_DECLARE(gImcMutex);
static int gMutexInitialized = 0;
static void *IMC_MEM_MOD;
static void *LOCAL_IMC;

/******************************************************************************
  Static Function Declarations
 ******************************************************************************/


static ADEC_INLINE ADEC_RESULT _IMC_SendLocalCommand(
	ImcInfo*				_imc,
	ImcCommandParameter*	_imcCommandParam,
	unsigned int*			_responseID);

static ADEC_INLINE ADEC_RESULT _IMC_SendRemoteCommand(
	ImcInfo*				_imc,
	ADEC_CORE_TYPE			_myCore,
	ADEC_CORE_TYPE			_targetCore,
	ImcCommandParameter*	_imcCommandParam,
	unsigned int*			_responseID);


static ADEC_INLINE ADEC_RESULT _IMC_RegisterLocalAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	unsigned int	_actionID,
	ImcActionParameter* _imcActionParam);

static ADEC_INLINE ADEC_RESULT _IMC_RegisterRemoteAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	ADEC_CORE_TYPE	_targetCore,
	unsigned int	_actionID,
	ImcActionParameter* _imcActionParam);

static ADEC_INLINE ADEC_RESULT _IMC_CancelLocalAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	unsigned int	_actionID);

static ADEC_INLINE ADEC_RESULT _IMC_CancelRemoteAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	ADEC_CORE_TYPE	_targetCore,
	unsigned int	_actionID);


static ADEC_INLINE ADEC_RESULT _IMC_InsertResponseToList(
	ImcInfo*		_imc,
	PFN_ImcResponse	_responseFunc,
	void*			_responseParam,
	unsigned int	_responseID);

static ADEC_INLINE ADEC_RESULT _IMC_InsertNotiToList(
	ImcInfo*		_imc,
	IMC_ACTION_REPEAT_TYPE _repeatType,
	PFN_ImcNoti		_noti,
	void*			_param,
	unsigned int	_actionID);

static ADEC_INLINE ADEC_RESULT _IMC_InsertActionToList(
	ImcEventInfo*			_event,
	ImcActionParameter*		_imcActionParam,
	unsigned int			_actionID);


static ADEC_INLINE ADEC_RESULT _IMC_DeleteActionFromList(
	ImcEventInfo*		_eventInfo,
	unsigned int		_actionID);

static ADEC_INLINE ADEC_RESULT _IMC_FindAndClearResponse(
	ImcInfo*			_imc,
	unsigned int		_responseID,
	ImcResponseInfo*	_info);

static ADEC_INLINE ADEC_RESULT _IMC_FindAndClearNotification(
	ImcInfo*			_imc,
	unsigned int		_actionID,
	ImcNotiInfo*		_imcNotiInfo);


static ADEC_INLINE ADEC_RESULT _IMC_ProcessImc(
	ImcInfo*			_imc,
	ADEC_MODULE_ID		_target,
	int					_cmd,
	void*				_param,
	int					_paramLen,
	ImcSenderInfo*		_sender);

static ADEC_INLINE ADEC_RESULT _IMC_Notification(
	void*				_imc,
	ADEC_MODULE_ID		_target,
	ImcEventParameter*	_ImcEventParameter,
	unsigned int		_actionID);

/******************************************************************************
  Static Function Implementations
 ******************************************************************************/

static ADEC_INLINE ADEC_RESULT _IMC_SendLocalCommand(
	ImcInfo*				_imc,
	ImcCommandParameter*	_imcCommandParam,
	unsigned int*			_responseID)
{
	ImcSenderInfo	senderInfo;
	//ImcCmdProcInfo* procInfo;
	int				msgID;

	ADEC_ASSERT(_imc != NULL, return ADEC_ERR_NULL);

	msgID = (_imc->imcIndex++)&0xFFFFFF;
	if (_responseID != NULL)
	{
		*_responseID = msgID;
	}

	senderInfo.module = _imcCommandParam->source;
	senderInfo.id = msgID;
	senderInfo.requesetReponse = _imcCommandParam->requestResponse;

	if (_imcCommandParam->requestResponse)
	{
		_IMC_InsertResponseToList(
			_imc,
			_imcCommandParam->callback,
			_imcCommandParam->callbackParam,
			msgID);
	}

	_IMC_ProcessImc(
		_imc,
		_imcCommandParam->target,
		_imcCommandParam->cmd,
		_imcCommandParam->param,
		_imcCommandParam->paramLength,
		&senderInfo);

	return ADEC_ERR_NONE;
}

static ADEC_INLINE ADEC_RESULT _IMC_SendRemoteCommand(
	ImcInfo*				_imc,
	ADEC_CORE_TYPE			_myCore,
	ADEC_CORE_TYPE			_targetCore,
	ImcCommandParameter*	_imcCommandParam,
	unsigned int*			_responseID)
{
	IpcHeader		ipcHeader;
	void*			ipcWriter;
	unsigned int	msgID;

	ipcWriter = _imc->ipcWriter[_targetCore];

	if(ipcWriter == NULL)
	{
		return ADEC_ERR_NOT_DEFINED;
	}

	msgID = (_imc->imcIndex++)&0xFFFFFF;
	*_responseID = msgID;

	ipcHeader.cmd = (unsigned int)(_imcCommandParam->cmd);
	ipcHeader.paramLength = (unsigned int)(_imcCommandParam->paramLength);
	ipcHeader.senderInfo.strVal.identifier = msgID;
	ipcHeader.senderInfo.strVal.sender = _imcCommandParam->source;
	ipcHeader.targetInfo.strVal.actionFlag = IMC_CMD_COMMAND_FLAG;
	ipcHeader.targetInfo.strVal.optionFlag = IMC_OPTION_FLAG(_imcCommandParam->requestResponse, 0, 0);
	ipcHeader.targetInfo.strVal.target = _imcCommandParam->target;

	if (_imcCommandParam->requestResponse)
	{
		_IMC_InsertResponseToList(
			_imc,
			_imcCommandParam->callback,
			_imcCommandParam->callbackParam,
			msgID);
	}

	return IPC_WriteCommand(
		ipcWriter,
		&ipcHeader,
		_imcCommandParam->param,
		_imcCommandParam->requestInterrupt);
}

static ADEC_INLINE ADEC_RESULT _IMC_RegisterLocalAction(
	ImcInfo*			_imc,
	unsigned int		_eventID,
	unsigned int		_actionID,
	ImcActionParameter* _imcActionParam)
{
	void*				RBInfo;
	ImcEventInfo*		eventInfo;
	ADEC_RESULT			result;

	/* Regist Action */
	result = RBTree_Search(_imc->imcEventTree, _eventID, &RBInfo);
	if(result == ADEC_ERR_NOT_FOUND)
	{
		RBInfo = ADEC_MALLOC(IMC_MEM_MOD, sizeof(ImcEventInfo), ImcEventInfo);
		ADEC_MEMSET(IMC_MEM_MOD, RBInfo, 0, sizeof(ImcEventInfo));
		RBTree_CreateNode(_imc->imcEventTree, _eventID, RBInfo);
	}
	eventInfo = (ImcEventInfo*)RBInfo;
	result = _IMC_InsertActionToList(
		eventInfo,
		_imcActionParam,
		_actionID);

	NOTI_DBG("Receive Register_Notification(%s) : eventID(%xd), actionID(%x) \n",
		IMC_PLATFORM_NAME,
		_eventID,
		_actionID);

	return result;
}

static ADEC_INLINE ADEC_RESULT _IMC_RegisterRemoteAction(
	ImcInfo*			_imc,
	unsigned int		_eventID,
	ADEC_CORE_TYPE		_targetCore,
	unsigned int		_actionID,
	ImcActionParameter* _imcActionParam)
{
	IpcHeader		ipcHeader;
	void*			ipcWriter;

	/* Write Action to IPC */
	ipcWriter = _imc->ipcWriter[_targetCore];

	ADEC_ASSERT(ipcWriter != NULL, return ADEC_ERR_NOT_DEFINED);

	ipcHeader.senderInfo.strVal.identifier = _actionID;
	ipcHeader.senderInfo.strVal.sender = _imcActionParam->target;
	ipcHeader.targetInfo.strVal.actionFlag = IMC_CMD_REGISTER_FLAG;
	ipcHeader.targetInfo.strVal.target = _imcActionParam->actionParam.notiParam.level;
	ADEC_ASSERT((_imcActionParam->actionType & (~1)) == 0,
		return ADEC_ERR_RANGE;);
	ADEC_ASSERT((_imcActionParam->repeatType & (~1)) == 0,
		return ADEC_ERR_RANGE;);
	ipcHeader.targetInfo.strVal.optionFlag = IMC_OPTION_FLAG(0,_imcActionParam->actionType,_imcActionParam->repeatType);
	ipcHeader.extField = _eventID;

	if (_imcActionParam->actionType == IMC_ACTION_EXCUTE_CMD)
	{
		ipcHeader.cmd = (unsigned int)(_imcActionParam->actionParam.cmdParam.cmd);
		ipcHeader.paramLength = (unsigned int)(_imcActionParam->actionParam.cmdParam.paramLen);

		return IPC_WriteCommand(
			ipcWriter,
			&ipcHeader,
			_imcActionParam->actionParam.cmdParam.param,
			_imcActionParam->actionParam.cmdParam.paramLen);
	}
	else if (_imcActionParam->actionType == IMC_ACTION_GET_CALLBACK)
	{
		ipcHeader.cmd = 0;
		ipcHeader.paramLength = 0;

		return IPC_WriteCommand(
			ipcWriter,
			&ipcHeader,
			NULL,
			0);
	}

	return ADEC_ERR_RANGE;
}

static ADEC_INLINE ADEC_RESULT _IMC_CancelLocalAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	unsigned int	_actionID)
{
	void*				RBInfo;
	ImcEventInfo*		eventInfo;
	ADEC_RESULT			result;

	/* Delete Action */
	result = RBTree_Search(_imc->imcEventTree, _eventID, &RBInfo);
	if(result != ADEC_ERR_NONE)
	{
		ADEC_ASSERT(result == ADEC_ERR_NOT_FOUND, return result);
		return result;
	}

	eventInfo = (ImcEventInfo*)RBInfo;

	result = _IMC_DeleteActionFromList(eventInfo, _actionID);
	if(result == ADEC_ERR_RANGE || eventInfo->actionCount == 0)
	{
		result = RBTree_Delete(_imc->imcEventTree, _eventID);
	}

	NOTI_DBG("Receive Cancel_Notification(%s) : eventID(%x), actionID(%x) \n",
		IMC_PLATFORM_NAME,
		_eventID,
		_actionID);

	return result;
}

static ADEC_INLINE ADEC_RESULT _IMC_CancelRemoteAction(
	ImcInfo*		_imc,
	unsigned int	_eventID,
	ADEC_CORE_TYPE	_targetCore,
	unsigned int	_actionID)
{
	IpcHeader	ipcHeader;
	void*		ipcWriter;

	ipcWriter = _imc->ipcWriter[_targetCore];

	ADEC_ASSERT(ipcWriter != NULL, return ADEC_ERR_NOT_DEFINED);

	ipcHeader.cmd = IMC_CMD_CANCEL_FLAG;
	ipcHeader.targetInfo.strVal.actionFlag = IMC_CMD_CANCEL_FLAG;
	ipcHeader.senderInfo.strVal.identifier = _actionID;
	ipcHeader.paramLength = 0;
	ipcHeader.extField = _eventID;

	return IPC_WriteCommand(
		ipcWriter,
		&ipcHeader,
		NULL,
		IMC_INTERRUPT_NOT_REQUIRED);
}

////////////////////////////////////////////////////////////////////////////////
// Cuation : Do not call this function in the interrrupt service routine
////////////////////////////////////////////////////////////////////////////////
static ADEC_INLINE ADEC_RESULT _IMC_InsertResponseToList(
	ImcInfo*		_imc,
	PFN_ImcResponse	_responseFunc,
	void*			_responseParam,
	unsigned int	_responseID)
{
	ImcResponseInfo* response;

	ADEC_MUTEX_LOCK(&gImcMutex);

	if (_imc->imcResponseCount >= ADEC_CNST_IMC_MAX_RESPONSE_COUNT)
	{
		ADEC_MUTEX_RELEASE(&gImcMutex);
		return ADEC_ERR_OVERFLOW;
	}

	response = &_imc->imcResponseArray[_imc->imcResponseCount];

	response->responseFunc = _responseFunc;
	response->responseParam = _responseParam;
	response->responseID = _responseID;
	response->insertTime = ADEC_GET_CURRENT_CLOCK();

	_imc->imcResponseCount++;

	ADEC_MUTEX_RELEASE(&gImcMutex);

	return ADEC_ERR_NONE;
}

static ADEC_INLINE ADEC_RESULT _IMC_InsertNotiToList(
	ImcInfo*		_imc,
	IMC_ACTION_REPEAT_TYPE _repeatType,
	PFN_ImcNoti		_noti,
	void*			_param,
	unsigned int	_actionID)
{
	ImcNotiInfo* notiInfo;

	ADEC_MUTEX_LOCK(&gImcMutex);

	if (_imc->imcNotiCount >= ADEC_CNST_IMC_MAX_NOTI_COUNT)
	{
		//ADEC_MUTEX_RELEASE(&gImcMutex);
		//return ADEC_ERR_OVERFLOW;
		_imc->imcNotiCount = 0;
	}

	notiInfo = &_imc->imcNotiArray[_imc->imcNotiCount];

	notiInfo->repeatType = _repeatType;
	notiInfo->notiFunc = _noti;
	notiInfo->notiID = _actionID;
	notiInfo->notiParam = _param;

	_imc->imcNotiCount++;

	ADEC_MUTEX_RELEASE(&gImcMutex);

	return ADEC_ERR_NONE;
}

static ADEC_INLINE ADEC_RESULT _IMC_InsertActionToList(
	ImcEventInfo*			_event,
	ImcActionParameter*		_imcActionParam,
	unsigned int			_actionID)
{
	int actionCnt;
	ADEC_MUTEX_LOCK(&gImcMutex);

	if (_event->actionCount >= ADEC_CNST_IMC_MAX_ACTION_COUNT)
	{
//		ADEC_MUTEX_RELEASE(&gImcMutex);
//		return ADEC_ERR_OVERFLOW;
		_event->actionCount = 0;
	}

	actionCnt = _event->actionCount;
	ADEC_MEMCPY(IMC_MEM_MOD,
				&_event->imcActionArray[actionCnt].actionParam,
				_imcActionParam,
				sizeof(ImcActionParameter));
	_event->imcActionArray[_event->actionCount].actionID = _actionID;

	if (_imcActionParam->actionType == IMC_ACTION_EXCUTE_CMD)
	{
		void *cmdParam;
		unsigned int paramLen;

		cmdParam = _imcActionParam->actionParam.cmdParam.param;
		paramLen = _imcActionParam->actionParam.cmdParam.paramLen;
		if ( cmdParam != NULL &&
			 paramLen != 0)
		{
			void *dupParam;
			dupParam = ADEC_MALLOC(IMC_MEM_MOD, paramLen, void);

			if (dupParam == NULL)
			{
				ADEC_MUTEX_RELEASE(&gImcMutex);
				return ADEC_ERR_NOT_ENOUGH_MEM;
			}

			ADEC_MEMCPY(IMC_MEM_MOD, dupParam, cmdParam, paramLen);

			_event->imcActionArray[actionCnt].actionParam.actionParam.cmdParam.param = dupParam;
			_event->imcActionArray[actionCnt].actionParam.actionParam.cmdParam.paramLen = paramLen;
		}
		else
		{
			_event->imcActionArray[actionCnt].actionParam.actionParam.cmdParam.param = NULL;
			_event->imcActionArray[actionCnt].actionParam.actionParam.cmdParam.paramLen = 0;
		}

	}
	else
	{
		if(!(_imcActionParam->actionParam.notiParam.level > 0 && _imcActionParam->actionParam.notiParam.level <= 1000))
		{
			_event->imcActionArray[_event->actionCount].actionParam.actionParam.notiParam.level = 1;
			_event->imcActionArray[_event->actionCount].actionParam.actionParam.notiParam.levelCnt = 1;
		}
		else
		{
			_event->imcActionArray[_event->actionCount].actionParam.actionParam.notiParam.level = _imcActionParam->actionParam.notiParam.level;
			_event->imcActionArray[_event->actionCount].actionParam.actionParam.notiParam.levelCnt = _imcActionParam->actionParam.notiParam.level;
		}
	}

	_event->actionCount++;

	ADEC_MUTEX_RELEASE(&gImcMutex);

	return ADEC_ERR_NONE;
}

static ADEC_INLINE ADEC_RESULT _IMC_DeleteActionFromList(
	ImcEventInfo*	_eventInfo,
	unsigned int	_actionID)
{
	int i;
	ImcActionInfo *curAction;

	ADEC_MUTEX_LOCK(&gImcMutex);

	if (_eventInfo->actionCount >= ADEC_CNST_IMC_MAX_ACTION_COUNT) {
        IMC_DBG_ERR("Action count is too big. Struct seems like to be broken.\n");
        ADEC_MUTEX_RELEASE(&gImcMutex);
        return ADEC_ERR_CORRUPTED;
    }

	for(i = 0 ; i < _eventInfo->actionCount ; i++)
	{
		if(_eventInfo->imcActionArray[i].actionID == _actionID)
		{
#if 0
                ADEC_ASSERT(_eventInfo->actionCount > 0, return ADEC_ERR_RANGE);
#else
                if(_eventInfo->actionCount <= 0)
                {
                    IMC_DBG_ERR("[#ERROR#]_eventInfo->actionCount[%d] is not proper !!!\n", _eventInfo->actionCount);
                    ADEC_MUTEX_RELEASE(&gImcMutex);
                    return ADEC_ERR_RANGE;
                }
#endif

			curAction = &_eventInfo->imcActionArray[i];
			if (curAction->actionParam.actionType == IMC_ACTION_EXCUTE_CMD)
			{
				if (curAction->actionParam.actionParam.cmdParam.param != NULL)
				{
					ADEC_FREE(IMC_MEM_MOD, curAction->actionParam.actionParam.cmdParam.param);
				}
			}
			_eventInfo->actionCount--;

			if (_eventInfo->actionCount > i)
            {
                ADEC_MEMCPY(IMC_MEM_MOD,
                    &(_eventInfo->imcActionArray[i]),
                    &(_eventInfo->imcActionArray[i+1]),
                    sizeof(ImcActionInfo) * (_eventInfo->actionCount - i));
            }
			ADEC_MUTEX_RELEASE(&gImcMutex);
			return ADEC_ERR_NONE;
		}
	}
	ADEC_MUTEX_RELEASE(&gImcMutex);
	return ADEC_ERR_NOT_FOUND;
}
////////////////////////////////////////////////////////////////////////////////
// Cuation : Do not call this function in the interrrupt service routine
////////////////////////////////////////////////////////////////////////////////
static ADEC_INLINE ADEC_RESULT _IMC_FindAndClearResponse(
	ImcInfo*			_imc,
	unsigned int		_responseID,
	ImcResponseInfo*	_info)
{
	ImcResponseInfo* response = NULL;
	unsigned int i;

	ADEC_MUTEX_LOCK(&gImcMutex);
	// Find
	for (i = 0 ; i < _imc->imcResponseCount ; i++)
	{
		if (_imc->imcResponseArray[i].responseID == _responseID)
		{
			*_info = _imc->imcResponseArray[i];
			response = &_imc->imcResponseArray[i];
			break;
		}
	}

	// Clear from the list
	if (response != NULL)
	{
		if (_imc->imcResponseCount > 1)
		{
			_imc->imcResponseCount--;
			_imc->imcResponseArray[i] = _imc->imcResponseArray[_imc->imcResponseCount];
		}
		else
		{
			_imc->imcResponseCount = 0;
		}

		ADEC_MUTEX_RELEASE(&gImcMutex);
		return ADEC_ERR_NONE;
	}

	ADEC_MUTEX_RELEASE(&gImcMutex);

	return ADEC_ERR_NOT_FOUND;
}

static ADEC_INLINE ADEC_RESULT _IMC_FindAndClearNotification(
	ImcInfo*			_imc,
	unsigned int		_actionID,
	ImcNotiInfo*		_imcNotiInfo)
{
	ImcNotiInfo* noti = NULL;
	unsigned int i;

	ADEC_MUTEX_LOCK(&gImcMutex);
	// Find
	for (i = 0 ; i < _imc->imcNotiCount ; i++)
	{
		if (_imc->imcNotiArray[i].notiID == _actionID)
		{
			*_imcNotiInfo = _imc->imcNotiArray[i];
			noti = &_imc->imcNotiArray[i];
			break;
		}
	}

	// Clear from the list
	if (noti != NULL)
	{
		 if(noti->repeatType == IMC_ACTION_ONCE)
		 {
			if (_imc->imcNotiCount > 1)
			{
				_imc->imcNotiCount--;

				ADEC_MEMCPY(IMC_MEM_MOD, &_imc->imcNotiArray[i], &_imc->imcNotiArray[_imc->imcNotiCount], sizeof(ImcNotiInfo));
			}
			else
			{
				_imc->imcNotiCount = 0;
			}
		}
		ADEC_MUTEX_RELEASE(&gImcMutex);
		return ADEC_ERR_NONE;
	}

	ADEC_MUTEX_RELEASE(&gImcMutex);
	return ADEC_ERR_NOT_FOUND;
}

static ADEC_INLINE ADEC_RESULT _IMC_ProcessImc(
	ImcInfo*			_imc,
	ADEC_MODULE_ID		_target,
	int					_cmd,
	void*				_param,
	int					_paramLen,
	ImcSenderInfo*		_sender)
{
	ImcCmdProcInfo* cmdProcInfo;

	ADEC_ASSERT(ModuleList_FindModule(_target) == _imc->core,
		return ADEC_ERR_INVALID_LOCATION;);
	cmdProcInfo = &_imc->imcCmdProcArray[_target];

	if (cmdProcInfo->proc != NULL)
	{
		cmdProcInfo->proc(cmdProcInfo->moduleParam, _cmd, _sender, _param, _paramLen);
	} else if (_imc->defaultProc != NULL) {
        IMC_DBG_LOW("Undefined CMD processor for module %d\n",_target);
        _imc->defaultProc(_imc->defaultProcParam, _target, _cmd, _sender, _param, _paramLen);
    } else {
        IMC_DBG_ERR("Undefiend default processor\n");
        return ADEC_ERR_NOT_DEFINED;
    }

	return ADEC_ERR_NONE;
}

static ADEC_INLINE ADEC_RESULT _IMC_Notification(
	void*				_imc,
	ADEC_MODULE_ID		_target,
	ImcEventParameter*	_ImcEventParameter,
	unsigned int		_actionID)
{
	ImcInfo*		imc;
	IpcHeader		ipc;
	ImcNotiInfo		notiInfo;
	ADEC_CORE_TYPE	core;
	ADEC_CORE_TYPE	myCore;
	ADEC_RESULT		result;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT(_imc != NULL, return ADEC_ERR_NULL);
	core = ModuleList_FindModule(_target);
	myCore = imc->core;

	// Local Notification
	if (core == myCore)
	{
		result = _IMC_FindAndClearNotification(imc, _actionID, &notiInfo);
		if (result != ADEC_ERR_NONE)
		{
			IMC_DBG_ERR("Fail to find notification (%d)\n", result);
			return result;
		}

		if (notiInfo.notiFunc)
		{
			notiInfo.notiFunc(_ImcEventParameter->param,
							  _ImcEventParameter->paramLen,
							  notiInfo.notiParam);
		}
	}
	// Remote Notification
	else
	{
		ipc.cmd = IMC_CMD_NOTIFICATION_FLAG;
		ipc.paramLength = (unsigned int)(_ImcEventParameter->paramLen);
		ipc.senderInfo.strVal.identifier = _actionID;
		ipc.targetInfo.strVal.actionFlag = IMC_CMD_NOTIFICATION_FLAG;

		ADEC_ASSERT_STR(
			imc->ipcWriter[core] != NULL,
			return ADEC_ERR_NOT_DEFINED,
			"IPC writer (to core %d) is not defined\n",
			core);

		result = IPC_WriteCommand(
			imc->ipcWriter[core],
			&ipc,
			_ImcEventParameter->param,
			_ImcEventParameter->requestInterrupt);

		return result;
	}

	return ADEC_ERR_NONE;
}

/******************************************************************************
  Extern Function Implementaions
 ******************************************************************************/

ADEC_RESULT IMC_Create(
	void**			_imc,
	ADEC_CORE_TYPE	_core)
{
	ImcInfo* imc;
	int structSize = sizeof(ImcInfo);

	*_imc = imc = ADEC_MALLOC(IMC_MEM_MOD, structSize, ImcInfo);
	if (imc == NULL)
	{
		return ADEC_ERR_NOT_ENOUGH_MEM;
	}

	return IMC_Init(imc,_core);
}

void IMC_Terminate(
	void*			_imc)
{
	if (_imc != NULL)
	{
		ADEC_FREE(IMC_MEM_MOD, _imc);
	}
}

int	IMC_GetStructSize(void)
{
	return sizeof(ImcInfo);
}

static void _ImcEventInfo_Terminate(void *_ptr)
{
    if (_ptr != NULL) {
        ADEC_FREE(IMC_MEM_MOD, _ptr);
    }
}

ADEC_RESULT IMC_Init(
	void* _imc,
	ADEC_CORE_TYPE _core)
{
	ImcInfo* imc;
	ADEC_RESULT result;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);

	ADEC_MEMSET(IMC_MEM_MOD, _imc, 0 ,sizeof(ImcInfo));
	if (_core > ADEC_CORE_NOT_DEF && _core < ADEC_CORE_MAX)
	{
		imc->core = _core;
	}
	else
	{
		imc->core = ModuleList_GetPlatform();
	}
	result= RBTree_CreateTree(&imc->imcEventTree, _ImcEventInfo_Terminate);

	ADEC_MUTEX_CREATE(&gImcMutex);
	gMutexInitialized = 1;

	return result;
}

ADEC_RESULT IMC_Finalize(
    void*           _imc)
{
    ImcInfo* imc;
	ADEC_RESULT result;

    imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);

    result= RBTree_DeleteTree(imc->imcEventTree);

    return result;
}


ADEC_RESULT IMC_SendCommand(
	void*				 _imc,
	ImcCommandParameter* _imcCommandParam,
	unsigned int*		 _responseID)
{
	ImcInfo*		imc;
	ADEC_CORE_TYPE	myCore;
	ADEC_CORE_TYPE	targetCore;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT(
		_imc != NULL,
		return ADEC_ERR_NULL);
	ADEC_ASSERT(
		_imcCommandParam != NULL,
		return ADEC_ERR_NULL);

	myCore = imc->core;
	targetCore = ModuleList_FindModule(_imcCommandParam->target);

	ADEC_ASSERT(
		myCore < ADEC_CORE_MAX && myCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);
	ADEC_ASSERT(
		targetCore < ADEC_CORE_MAX && targetCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);


	// Send message to the target, located same core
	if (myCore == targetCore)
	{
		return _IMC_SendLocalCommand(
			(ImcInfo*)_imc,
			_imcCommandParam,
			_responseID);
	}
	// Send message to the target, located different core
	else
	{
		return _IMC_SendRemoteCommand(
			(ImcInfo*)_imc,
			myCore,
			targetCore,
			_imcCommandParam,
			_responseID);
	}
}

ADEC_RESULT IMC_Response(
	void*					_imc,
	ImcResponseParameter*	_ImcResponseParameter)
{
	ImcInfo*		imc;
	ImcResponseInfo response;
	long long		currentTime;
	unsigned int	tmpIpcParam[ADEC_CNST_IPC_MAX_PARAM_CNT];
	IpcHeader		ipc;
	ADEC_CORE_TYPE	core;
	ADEC_CORE_TYPE	myCore;
	ADEC_RESULT		result;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	core = ModuleList_FindModule(_ImcResponseParameter->target.module);
	myCore = imc->core;

	// Local response
	if (core == myCore)
	{
		result = _IMC_FindAndClearResponse(imc, _ImcResponseParameter->target.id, &response);
		if (result != ADEC_ERR_NONE)
		{
			IMC_DBG_ERR("Fail to find response (%d)\n", result);
			return result;
		}
		currentTime = ADEC_GET_CURRENT_CLOCK();

		IMC_DBG_LOW(
			"Response requeseted 0x%016"__PRT64X", posted 0x%016"__PRT64X"\n",
			response.insertTime,
			currentTime);

		if (response.responseFunc)
		{
			response.responseFunc(_ImcResponseParameter->result, _ImcResponseParameter->param, _ImcResponseParameter->paramLen, response.responseParam);
		}
	}
	// Remote response
	else
	{
		ipc.cmd = (unsigned int)(_ImcResponseParameter->cmd);
		ipc.paramLength = (unsigned int)(_ImcResponseParameter->paramLen) + 4;
		ipc.senderInfo.strVal.identifier = _ImcResponseParameter->target.id;
		ipc.senderInfo.strVal.sender = _ImcResponseParameter->sender;
		ipc.targetInfo.strVal.target = _ImcResponseParameter->target.module;
		ipc.targetInfo.strVal.optionFlag = 0;
		ipc.targetInfo.strVal.actionFlag = IMC_CMD_RESPONSE_FLAG;

		tmpIpcParam[0] = _ImcResponseParameter->result;
		if (_ImcResponseParameter->param && _ImcResponseParameter->paramLen > 0)
		{
			ADEC_MEMCPY(IMC_MEM_MOD, tmpIpcParam+1, _ImcResponseParameter->param, _ImcResponseParameter->paramLen);
		}

		ADEC_ASSERT(
			_ImcResponseParameter->paramLen < ADEC_CNST_IPC_MAX_PARAM_CNT - 4,
			return ADEC_ERR_DATA_SIZE);
		ADEC_ASSERT_STR(
			imc->ipcWriter[core] != NULL,
			return ADEC_ERR_NOT_DEFINED,
			"IPC writer (to core %d) is not defined\n",
			core);

		result = IPC_WriteCommand(
			imc->ipcWriter[core],
			&ipc,
			tmpIpcParam,
			_ImcResponseParameter->requestInterrupt);

		return result;
	}

	return ADEC_ERR_NONE;
}

ADEC_RESULT IMC_SetTargetWriter(
	void* _imc,
	ADEC_CORE_TYPE _coreType,
	void* _ipcWriter)
{
	ImcInfo* imc;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT(
		_coreType < ADEC_CORE_MAX && _coreType >= 0,
		return ADEC_ERR_RANGE);

	ADEC_ASSERT(
		_coreType != imc->core,
		return ADEC_ERR_RANGE);

	imc->ipcWriter[_coreType] = _ipcWriter;

	return ADEC_ERR_NONE;
}

ADEC_RESULT IMC_SetTargetReader(
	void* _imc,
	ADEC_CORE_TYPE _coreType,
	void* _ipcReader)
{
	ImcInfo* imc;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT(
		_coreType < ADEC_CORE_MAX && _coreType >= 0,
		return ADEC_ERR_RANGE);

	ADEC_ASSERT(
		_coreType != imc->core,
		return ADEC_ERR_RANGE);

	imc->ipcReader[_coreType] = _ipcReader;

	return ADEC_ERR_NONE;
}

ADEC_RESULT IMC_RegisterCmdProcessor(
	void* _imc,
	ADEC_MODULE_ID _module,
	PFN_ImcCmdProc _proc,
	void* _moduleParam)
{
	ImcInfo* imc;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT(
		_module < ADEC_MODULE_MAX && _module >= 0,
		return ADEC_ERR_RANGE);

	imc->imcCmdProcArray[_module].moduleParam = _moduleParam;
	imc->imcCmdProcArray[_module].proc = _proc;

	return ADEC_ERR_NONE;
}

ADEC_RESULT IMC_RegisterEvent(
	void*			_imc,
	unsigned int	_event,
	ADEC_MODULE_ID	_owner,
	unsigned int*	_actionID,
	ImcActionParameter* _imcActionParam)
{
	ImcInfo*		imc;
	ADEC_CORE_TYPE	myCore;
	ADEC_CORE_TYPE	targetCore;
	unsigned int	eventID;
	ADEC_RESULT		result;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT( _imcActionParam!=NULL, return ADEC_ERR_NULL);

	eventID = IMC_EVENT_ID(_event, _owner);
	*_actionID = IMC_ACTION_ID(imc->imcActionIndex++, imc->core);

	/* Check Core */
	myCore = imc->core;
	targetCore = ModuleList_FindModule(_owner);

	ADEC_ASSERT(
		myCore < ADEC_CORE_MAX && myCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);
	ADEC_ASSERT(
		targetCore < ADEC_CORE_MAX && targetCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);

	/* Regist Notification */
	if(_imcActionParam->actionType == IMC_ACTION_GET_CALLBACK)
	{
		result = _IMC_InsertNotiToList(
			imc,
			_imcActionParam->repeatType,
			_imcActionParam->actionParam.notiParam.noti,
			_imcActionParam->actionParam.notiParam.param,
			*_actionID);

			ADEC_ASSERT(result == ADEC_ERR_NONE, return result);
	}

	NOTI_DBG("Send Register_Notification(%s) : eventID(%x), actionID(%x) \n",
		IMC_PLATFORM_NAME,
		eventID,
		*_actionID);

	/* Regist Action */
	if (myCore == targetCore)
	{
		return _IMC_RegisterLocalAction(
			(ImcInfo*)_imc,
			eventID,
			*_actionID,
			_imcActionParam);
	}
	else
	{
		return _IMC_RegisterRemoteAction(
			(ImcInfo*)_imc,
			eventID,
			targetCore,
			*_actionID,
			_imcActionParam);
	}
}

ADEC_RESULT IMC_CancelEvent(
	void*			_imc,
	unsigned int	_event,
	ADEC_MODULE_ID	_owner,
	unsigned int	_actionID)
{
	ImcInfo*		imc;
	ADEC_CORE_TYPE	myCore;
	ADEC_CORE_TYPE	targetCore;
	unsigned int	eventID;
	unsigned int	i;

	imc = (ImcInfo*)_imc;
	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);

	eventID = IMC_EVENT_ID(_event, _owner);

	/* Delete Notification */
	ADEC_MUTEX_LOCK(&gImcMutex);
	for (i = 0 ; i < imc->imcNotiCount ; i++)
	{
		if (imc->imcNotiArray[i].notiID == _actionID)
		{
			if (imc->imcNotiCount > 1)
			{
				imc->imcNotiCount--;
				ADEC_MEMCPY(IMC_MEM_MOD, &imc->imcNotiArray[i], &imc->imcNotiArray[imc->imcNotiCount], sizeof(ImcNotiInfo));
			}
			else
			{
				imc->imcNotiCount = 0;
			}
			break;
		}
	}
	ADEC_MUTEX_RELEASE(&gImcMutex);

	NOTI_DBG("Send Cancel_Notification(%s) : eventID(%x), actionID(%x) \n",
		IMC_PLATFORM_NAME,
		eventID,
		_actionID);

	/* Check Core */
	myCore = imc->core;
	targetCore = ModuleList_FindModule(_owner);

	ADEC_ASSERT(
		myCore < ADEC_CORE_MAX && myCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);
	ADEC_ASSERT(
		targetCore < ADEC_CORE_MAX && targetCore > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);

	/* Cancel Action */
	if (myCore == targetCore)
	{
		return _IMC_CancelLocalAction(
			(ImcInfo*)_imc,
			eventID,
			_actionID);
	}
	else
	{
		return _IMC_CancelRemoteAction(
			(ImcInfo*)_imc,
			eventID,
			targetCore,
			_actionID);
	}
}

ADEC_RESULT IMC_RaiseEvent(
	void*				_imc,
	unsigned int		_event,
	ADEC_MODULE_ID		_owner,
	ImcEventParameter*	_imcEventParameter)
{
	ImcInfo*		imc;
	unsigned int	eventID;
	void*			RBInfo;
	unsigned int	responseID;
	int             i;

	ImcEventInfo*	eventInfo;
	ImcActionInfo*	actionInfo;
	ImcCommandParameter imcCommandParam;
	ADEC_RESULT		result;

	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc != NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT(
		imc->core < ADEC_CORE_MAX && imc->core > ADEC_CORE_NOT_DEF,
		return ADEC_ERR_RANGE);

	eventID = IMC_EVENT_ID(_event, _owner);

	result = RBTree_Search(imc->imcEventTree, eventID, &RBInfo);

	if (RBInfo == NULL || result != ADEC_ERR_NONE)
	{
		return result;
	}

	eventInfo = (ImcEventInfo*)RBInfo;

	if (eventInfo->actionCount > ADEC_CNST_IMC_MAX_ACTION_COUNT) {
	    IMC_DBG_ERR("Action count is too big. Struct seems like to be broken.\n");
	    //TODO : Modified bellow
	    while(1);
	    return ADEC_ERR_CORRUPTED;
	}

	for(i = 0 ; i < eventInfo->actionCount ; i++)
	{
		actionInfo = &eventInfo->imcActionArray[i];

		if(actionInfo->actionParam.actionType == IMC_ACTION_EXCUTE_CMD)
		{
			imcCommandParam.cmd = actionInfo->actionParam.actionParam.cmdParam.cmd;
			imcCommandParam.target = actionInfo->actionParam.target;
			imcCommandParam.source = _owner;
			imcCommandParam.requestResponse = IMC_RESPONSE_NOT_REQUIRED;
			imcCommandParam.requestInterrupt = IMC_INTERRUPT_REQUIRED;
			imcCommandParam.param = actionInfo->actionParam.actionParam.cmdParam.param;
			imcCommandParam.paramLength = actionInfo->actionParam.actionParam.cmdParam.paramLen;

			result = IMC_SendCommand(_imc, &imcCommandParam, &responseID);
			if(result != ADEC_ERR_NONE)
			{
				IMC_DBG_ERR("IMC_SendCommand fail\n");
			}

		}
		else if (actionInfo->actionParam.actionType == IMC_ACTION_GET_CALLBACK)
		{
			if(actionInfo->actionParam.actionParam.notiParam.level == actionInfo->actionParam.actionParam.notiParam.levelCnt)
			{
				_IMC_Notification(_imc,
								  actionInfo->actionParam.target,
								  _imcEventParameter,
								  actionInfo->actionID);
			}
			actionInfo->actionParam.actionParam.notiParam.levelCnt--;
			if(actionInfo->actionParam.actionParam.notiParam.levelCnt == 0)
			{
				actionInfo->actionParam.actionParam.notiParam.levelCnt = actionInfo->actionParam.actionParam.notiParam.level;
			}
		}
		else
		{
		    IMC_DBG_ERR("Unknown action type - %d\n", actionInfo->actionParam.actionType);
		    //TODO : Modified bellow
		    while(1);
		    continue;
		}

		// 한번만 수행하도록 설정된 액션 삭제
		if(actionInfo->actionParam.repeatType == IMC_ACTION_ONCE)
		{
			if (actionInfo->actionParam.actionType == IMC_ACTION_EXCUTE_CMD &&
				actionInfo->actionParam.actionParam.cmdParam.param != NULL)
			{
				ADEC_FREE(IMC_MEM_MOD, actionInfo->actionParam.actionParam.cmdParam.param);
			}
			eventInfo->actionCount--;

			if (eventInfo->actionCount > i)
			{
			    ADEC_MEMCPY(IMC_MEM_MOD,
			        &(eventInfo->imcActionArray[i]),
			        &(eventInfo->imcActionArray[i+1]),
			        sizeof(ImcActionInfo) * (eventInfo->actionCount - i));
			}

			/*for(j = i ; j < eventInfo->actionCount ; j++)
			{
				ADEC_MEMCPY(IMC_MEM_MOD,
							&eventInfo->imcActionArray[j],
							&eventInfo->imcActionArray[j+1],
							sizeof(ImcActionInfo));
			}*/
			i--;
		}
	}
	if( eventInfo->actionCount == 0 )
	{
		result = RBTree_Delete(imc->imcEventTree, eventID);
	}

	return result;
}

ADEC_RESULT IMC_Process(void* _imc)
{
	ImcInfo*	imc;
	void*		ipcReader;
	int			i;

	imc = (ImcInfo*)_imc;
	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);

	for (i = 1; i < ADEC_CORE_MAX ; i ++)
	{
		ipcReader = imc->ipcReader[i];
		if(ipcReader)
		{
			IMC_ReadFromIpc(_imc, ipcReader);
		}

		// TODO:
		// Send response
	}

	return ADEC_ERR_NONE;
}

ADEC_RESULT IMC_ReadFromIpc(
	void* _imc,
	void* _ipcReader)
{
	ImcInfo*		imc;
	IpcHeader*		ipcHeader;
	unsigned int*	param;
	unsigned int*	responseParam;

	ImcSenderInfo	senderInfo;
	ImcResponseInfo response;
	unsigned int	paramLength;
	ADEC_RESULT		result;

	ImcNotiInfo		notiInfo;
	void*			RBInfo;
	ImcEventInfo*	eventInfo;


	imc = (ImcInfo*)_imc;

	ADEC_ASSERT( _imc!= NULL, return ADEC_ERR_NULL);
	ADEC_ASSERT(_ipcReader != NULL, return ADEC_ERR_NULL;);

	while(1)
	{
		unsigned int actionFlag;
		result = IPC_ReadCommand(_ipcReader, &ipcHeader, &paramLength);
		if (result != 0)
		{
			// TODO : Process Err
			break;
		}

		actionFlag = ipcHeader->targetInfo.strVal.actionFlag;
		// Process Command
		if (actionFlag & IMC_CMD_COMMAND_FLAG)
		{
			senderInfo.id =
				ipcHeader->senderInfo.strVal.identifier;
			senderInfo.module =
				(ADEC_MODULE_ID)ipcHeader->senderInfo.strVal.sender;
			senderInfo.requesetReponse =
				(IMC_RESPONSE_TYPE)IMC_GET_OPT_RSP(ipcHeader->targetInfo.strVal.optionFlag);

			ADEC_CACHE_INVALIDATE(ipcHeader, ADEC_CNST_IPC_CACHE_SIZE + paramLength);
            TRACE_CMD(imc->imcTracer,
                (ADEC_MODULE_ID)ipcHeader->targetInfo.strVal.target,
                ipcHeader->cmd,
                &senderInfo,
                paramLength,
                &ipcHeader[1]);

			result = _IMC_ProcessImc(
				imc,
				(ADEC_MODULE_ID)ipcHeader->targetInfo.strVal.target,
				ipcHeader->cmd,
				&ipcHeader[1],
				paramLength, &senderInfo);
		}

		// Process response
		else if (actionFlag & IMC_CMD_RESPONSE_FLAG)
		{
            ADEC_ASSERT_STR(paramLength >= 4,
				return ADEC_ERR_DATA_CORRUPTED,
				"The response parameter length should be greater than or equal to 4\n");
			param = (unsigned int*)&ipcHeader[1];
            responseParam = paramLength > 4 ? &param[1] : NULL;

            TRACE_RESPONSE(imc->imcTracer,
                ipcHeader->senderInfo.strVal.identifier,
                param[0],
                paramLength - 4,
                responseParam);

            result = _IMC_FindAndClearResponse(imc, ipcHeader->senderInfo.strVal.identifier, &response);

			if (result != ADEC_ERR_NONE)
			{
				// Skip current response
				IMC_DBG_ERR("Fail to find response (%d)\n", result);
				continue;
			}

			if (response.responseFunc != NULL)
			{
				response.responseFunc(
					param[0],
					responseParam,
					ipcHeader->paramLength - 4,
					response.responseParam);
			}
		}

		// Process Notification
		else if (actionFlag & IMC_CMD_NOTIFICATION_FLAG)
		{
			unsigned int actionID;

			actionID = ipcHeader->senderInfo.strVal.identifier;
            TRACE_NOTI(imc->imcTracer, actionID, ipcHeader->paramLength, &ipcHeader[1]);

			result = _IMC_FindAndClearNotification(imc, actionID, &notiInfo);
			if (result == ADEC_ERR_NOT_FOUND)
			{
				IMC_DBG("Fail to find notification (%d)\n", result);
				//return ADEC_ERR_NONE;
				continue;
			}

			if (notiInfo.notiFunc)
			{
				notiInfo.notiFunc(&ipcHeader[1], ipcHeader->paramLength, notiInfo.notiParam);
			}
		}
		// Register Action
		else if (actionFlag & IMC_CMD_REGISTER_FLAG)
		{
			ImcActionParameter actionParam;
			unsigned int actionID;
			unsigned int eventID;
			unsigned int option;

			actionID = ipcHeader->senderInfo.strVal.identifier;
			eventID = ipcHeader->extField;
			option = ipcHeader->targetInfo.strVal.optionFlag;
			actionParam.actionType = (IMC_ACTION_TYPE)IMC_GET_OPT_ACT(option);
			actionParam.repeatType = (IMC_ACTION_REPEAT_TYPE)IMC_GET_OPT_RPT(option);
			actionParam.target = (ADEC_MODULE_ID)ipcHeader->senderInfo.strVal.sender;

			if (actionParam.actionType == IMC_ACTION_EXCUTE_CMD)
			{
				actionParam.actionParam.cmdParam.cmd = (int)(ipcHeader->cmd);
				actionParam.actionParam.cmdParam.paramLen = (int)(ipcHeader->paramLength);
				actionParam.actionParam.cmdParam.param = &ipcHeader[1];
			}
			else
			{
				actionParam.actionParam.notiParam.level = ipcHeader->targetInfo.strVal.target;
			}

            TRACE_REGISTER(imc->imcTracer,
                actionParam.target,
                actionID,
                eventID,
                actionParam.actionType,
                actionParam.repeatType,
                ipcHeader->cmd,
                ipcHeader->paramLength,
                &ipcHeader[1]);

			result = RBTree_Search(imc->imcEventTree, eventID, &RBInfo);
			if(result == ADEC_ERR_NOT_FOUND)
			{
				RBInfo = ADEC_MALLOC(IMC_MEM_MOD, sizeof(ImcEventInfo), ImcEventInfo);

				ADEC_MEMSET(IMC_MEM_MOD, RBInfo, 0, sizeof(ImcEventInfo));
				result = RBTree_CreateNode(imc->imcEventTree, eventID, RBInfo);

				if (result != ADEC_ERR_NONE)
				{
				    IMC_DBG_ERR("Fail to insert event(0x%x) to tree with err %d\n", eventID, result);
				    ADEC_FREE(IMC_MEM_MOD, RBInfo);
				    return result;
				}
			}
			eventInfo = (ImcEventInfo*)RBInfo;

			result =_IMC_InsertActionToList(
				eventInfo,
				&actionParam,
				actionID);

			NOTI_DBG("Receive Register_Notification(%s) : eventID(%x), actionID(%x) \n",
				IMC_PLATFORM_NAME,
				eventID,
				actionID);
		}

		// Cancel Action
		else if(actionFlag & IMC_CMD_CANCEL_FLAG)
		{
			unsigned int eventID;
			unsigned int actionID;

			eventID = ipcHeader->extField;
			actionID = ipcHeader->senderInfo.strVal.identifier;

            TRACE_CANCEL(imc->imcTracer, eventID, actionID);
			result = RBTree_Search(imc->imcEventTree, eventID, &RBInfo);

			if(result == ADEC_ERR_NOT_FOUND)
			{
				return ADEC_ERR_NOT_FOUND;
			}

			ADEC_ASSERT(result == ADEC_ERR_NONE,
			            return result;)

			eventInfo = (ImcEventInfo*)RBInfo;
			result = _IMC_DeleteActionFromList(eventInfo, actionID);
			if(result == ADEC_ERR_RANGE || eventInfo->actionCount == 0)
			{
				result =  RBTree_Delete(imc->imcEventTree, eventID);
			}

			NOTI_DBG("Receive Cancel_Notification(%s) : eventID(%x), actionID(%x) \n",
				IMC_PLATFORM_NAME,
				eventID,
				actionID);
		}
		else
		{
			ADEC_ASSERT_STR(0,
				return ADEC_ERR_RANGE,
				"Unkown IMC cmd category");
		}
	}
	return result;
}

ADEC_RESULT IMC_SetLocalImc(
	void*			_imc)
{
	LOCAL_IMC = _imc;

	return ADEC_ERR_NONE;
}

void* IMC_GetLocalImc(
	int _create_not_exist)
{
	if (LOCAL_IMC == NULL && _create_not_exist != 0)
	{
		IMC_Create(&LOCAL_IMC, ModuleList_GetPlatform());
	}

	return LOCAL_IMC;
}

void IMC_SetDefaultProc(void *_imc,
    PFN_ImcCmdProcNotDef _proc,
    void *_param) {
    ImcInfo *imc = (ImcInfo*)_imc;
    if (_imc == NULL) {
        return;
    }
    imc->defaultProc = _proc;
    imc->defaultProcParam = _param;
}

void IMC_SetImcTracer(void *_imc, void *_tracer) {
    ImcInfo *imc = (ImcInfo*)_imc;
    if (_imc == NULL) {
        return;
    }
    imc->imcTracer = _tracer;
}

/** @} */
