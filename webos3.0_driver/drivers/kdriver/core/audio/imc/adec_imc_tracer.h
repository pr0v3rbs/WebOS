#ifndef __ADEC_IMC_TRACER_H__
#define __ADEC_IMC_TRACER_H__

#include "adec_imc.h"

typedef void (*PFN_TraceCmd)(
    void *_tracer,
    ADEC_MODULE_ID _module,
    unsigned int _cmd,
    ImcSenderInfo *_sender,
    unsigned int _param_len,
    void *_param);

typedef void (*PFN_TraceResponse)(
    void *_tracer,
    unsigned int _action_id,
    int _result,
    unsigned int _param_len,
    void *_param);

typedef void (*PFN_TraceNotificationi)(
    void *_tracer,
    unsigned int _action_id,
    unsigned int _param_len,
    void *_param);

typedef void (*PFN_TraceCmdRegister)(
    void *_tracer,
    ADEC_MODULE_ID _module,
    unsigned int _action_id,
    unsigned int _event_id,
    IMC_ACTION_TYPE _action_type,
    IMC_ACTION_REPEAT_TYPE _repeat_type,
    unsigned int _cmd,
    unsigned int _param_len,
    void *_param);

typedef void (*PFN_TraceCmdCancel)(
    void *_trace,
    unsigned int _event_id,
    unsigned int _action_id);

typedef struct _ImcTracer {
    PFN_TraceCmd            cmd;
    PFN_TraceResponse       response;
    PFN_TraceNotificationi  noti;
    PFN_TraceCmdRegister    register_cmd;
    PFN_TraceCmdCancel      cancel_cmd;
}ImcTracer;

#ifdef ADEC_ENABLE_IMC_TRACE
#define TRACE_CMD(_TRACER_,_MODULE_,_CMD_,_SENDER_,_PARAM_LEN_,_PARAM_)\
    if(_TRACER_!=NULL) ((ImcTracer*)_TRACER_)->cmd(_TRACER_,_MODULE_,_CMD_,_SENDER_,_PARAM_LEN_,_PARAM_);
#define TRACE_RESPONSE(_TRACER_,_ACTION_ID_,_RESULT_,_PARAM_LEN_,_PARAM_)\
    if(_TRACER_!=NULL) ((ImcTracer*)_TRACER_)->response(_TRACER_,_ACTION_ID_,_RESULT_,_PARAM_LEN_,_PARAM_);
#define TRACE_NOTI(_TRACER_,_ACTION_ID_,_PARAM_LEN_,_PARAM_)\
    if(_TRACER_!=NULL) ((ImcTracer*)_TRACER_)->noti(_TRACER_,_ACTION_ID_,_PARAM_LEN_,_PARAM_);
#define TRACE_REGISTER(_TRACER_,_MODULE_,_ACTION_ID_,_EVENT_ID_,\
    _ACTION_TYPE_,_REPEAT_TYPE_,_CMD_,_PARAM_LEN_,_PARAM_)\
    if(_TRACER_!=NULL) ((ImcTracer*)_TRACER_)->register_cmd(_TRACER_,_MODULE_,_ACTION_ID_,_EVENT_ID_,\
    _ACTION_TYPE_,_REPEAT_TYPE_,_CMD_,_PARAM_LEN_,_PARAM_);
#define TRACE_CANCEL(_TRACER_,_EVENT_ID_,_ACTION_ID_)\
    if(_TRACER_!=NULL) ((ImcTracer*)_TRACER_)->cancel_cmd(_TRACER_,_EVENT_ID_,_ACTION_ID_);
#else
#define TRACE_CMD(...)
#define TRACE_RESPONSE(...)
#define TRACE_NOTI(...)
#define TRACE_REGISTER(...)
#define TRACE_CANCEL(...)
#endif



#endif
