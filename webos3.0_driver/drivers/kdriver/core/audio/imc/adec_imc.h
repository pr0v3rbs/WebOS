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
 *	@file adec_imc.h
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


/******************************************************************************
  Header File Guarder
 ******************************************************************************/
#ifndef __ADEC_IMC_H__
#define __ADEC_IMC_H__

#include "common/adec_common.h"
#include "module/adec_module_list.h"


typedef enum _IMC_RESPONSE_TYPE
{
	IMC_RESPONSE_NOT_REQUIRED,
	IMC_RESPONSE_REQUIRED,
}IMC_RESPONSE_TYPE;

typedef enum _IMC_INTERRUPT_TYPE
{
	IMC_INTERRUPT_NOT_REQUIRED,
	IMC_INTERRUPT_REQUIRED,
}IMC_INTERRUPT_TYPE;

typedef enum _IMC_ACTION_TYPE
{
	IMC_ACTION_EXCUTE_CMD,
	IMC_ACTION_GET_CALLBACK,
}IMC_ACTION_TYPE;

typedef enum _IMC_ACTION_REPEAT_TYPE
{
	IMC_ACTION_ONCE,
	IMC_ACTION_REPEAT,
}IMC_ACTION_REPEAT_TYPE;

typedef struct _ImcSenderInfo
{
	ADEC_MODULE_ID	module;
	ADEC_CORE_TYPE		core;
	IMC_RESPONSE_TYPE	requesetReponse;
	int					id;
}ImcSenderInfo;

typedef void (*PFN_ImcResponse)(
	int _iParam,
	void* _pParam,
	int _paramLen,
	void* _cbParam);

typedef void (*PFN_ImcCmdProc)(
	void* _module,
	int _cmd,
	ImcSenderInfo* _sender,
	void* _param,
	int _paramLen);

typedef void (*PFN_ImcCmdProcNotDef)(
    void* _self,
    ADEC_MODULE_ID  module,
    int _cmd,
    ImcSenderInfo* _sender,
    void* _param,
    int _paramLen);

typedef void (*PFN_ImcNoti)(
	void* _param,
	int	_paramLen,
	void* _cbParam);

typedef struct _ImcCommandParameter
{
	int					cmd;
	ADEC_MODULE_ID		target;
	ADEC_MODULE_ID		source;
	IMC_RESPONSE_TYPE	requestResponse;
	IMC_INTERRUPT_TYPE	requestInterrupt;
	PFN_ImcResponse		callback;
	void*				callbackParam;
	int					paramLength;
	void*				param;
}ImcCommandParameter;

typedef struct _ImcResponseParameter
{
	ImcSenderInfo		target;
	ADEC_MODULE_ID		sender;
	IMC_INTERRUPT_TYPE	requestInterrupt;
	int					cmd;
	ADEC_RESULT			result;
	void*				param;
	int					paramLen;
}ImcResponseParameter;

typedef struct _ImcActionParameter
{
	IMC_ACTION_TYPE			actionType;
	IMC_ACTION_REPEAT_TYPE	repeatType;
	ADEC_MODULE_ID			target;
	union {
		struct {
			void*					param;
			int						cmd;
			int						paramLen;
		}					cmdParam;
		struct {
			PFN_ImcNoti				noti;
			void*					param;
			int						level;
			int						levelCnt;
		}					notiParam;
	}						actionParam;
}ImcActionParameter;

typedef struct _ImcEventParameter
{
	IMC_INTERRUPT_TYPE		requestInterrupt;
	void*					param;
	int						paramLen;
}ImcEventParameter;

ADEC_RESULT IMC_Create(
	void**			_imc,
	ADEC_CORE_TYPE	_core);

void IMC_Terminate(
	void*			_imc);

int	IMC_GetStructSize(void);

ADEC_RESULT IMC_Init(
	void*			_imc,
	ADEC_CORE_TYPE	_core);

ADEC_RESULT IMC_Finalize(
    void*           _imc);

ADEC_RESULT IMC_SendCommand(
	void*					_imc,
	ImcCommandParameter*	_imcCommandParam,
	unsigned int*			_responseID);

ADEC_RESULT IMC_Response(
	void*					_imc,
	ImcResponseParameter*	_ImcResponseParameter);

ADEC_RESULT IMC_SetTargetWriter(
	void*			_imc,
	ADEC_CORE_TYPE	_coreType,
	void*			_ipcWriter);

ADEC_RESULT IMC_SetTargetReader(
	void*			_imc,
	ADEC_CORE_TYPE	_coreType,
	void*			_ipcReader);

ADEC_RESULT IMC_CancelResponse(
	void*			_imc,
	unsigned int	_reponseID);

ADEC_RESULT IMC_RegisterCmdProcessor(
	void*			_imc,
	ADEC_MODULE_ID	_module,
	PFN_ImcCmdProc	_proc,
	void*			_moduleParam);

ADEC_RESULT IMC_RegisterEvent(
	void*			_imc,
	unsigned int	_event,
	ADEC_MODULE_ID	_owner,
	unsigned int*	_actionID,
	ImcActionParameter* _imcActionParam);

ADEC_RESULT IMC_CancelEvent(
	void*			_imc,
	unsigned int	_event,
	ADEC_MODULE_ID	_owner,
	unsigned int	_actionID);

ADEC_RESULT IMC_RaiseEvent(
	void*			_imc,
	unsigned int	_event,
	ADEC_MODULE_ID	_owner,
	ImcEventParameter* _imcEventParam);

/**
 * Read IMC from IPC
 * Call this function when the IPC packet is arrived.
 * But do not call this function in the interrupt routine
 * If you do that, this function may pend the interrupt routine.
 */
ADEC_RESULT IMC_Process(void* _imc);

ADEC_RESULT IMC_ReadFromIpc(
	void*			_imc,
	void*			_ipcReader);

ADEC_RESULT IMC_SetLocalImc(
	void*			_imc);

void* IMC_GetLocalImc(
	int _create_not_exist);

void IMC_SetDefaultProc(void *_imc,
    PFN_ImcCmdProcNotDef _proc,
    void *_param);

void IMC_SetImcTracer(void *_imc, void *_tracer);

#endif //__ADEC_IMC_H__
/** @} */
