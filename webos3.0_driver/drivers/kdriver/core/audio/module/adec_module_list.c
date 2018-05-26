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
 *	@file adec_module_list.c
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-19
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC_COMMON
 *	@{
 */

#include "common/adec_common.h"

#include "adec_module_list.h"

static ModuleInfo gModuleInfo[ADEC_MODULE_MAX] = {
	{
		ADEC_MODULE_NOT_DEF,
		"NOT_DEF",
		ADEC_MOD_TYPE_NOT_DEF,
		ADEC_CORE_NOT_DEF,
		0,
		0,
		{
			ADEC_BUF_NOT_DEF,
			ADEC_BUF_NOT_DEF,
			ADEC_BUF_NOT_DEF,
			ADEC_BUF_NOT_DEF,
		}
	},
#ifdef ADEC_MODULE_DEF
#undef ADEC_MODULE_DEF
#endif
#ifdef __BUF_END
#undef __BUF_END
#endif

#ifdef _MSC_VER
#define __BUF_END(...)	ADEC_BUF_NOT_DEF
#define __MOD_DECL(__MODULE, __MODULE_NAME, __MODULE_TYPE, __LOCATION, __NUM_IN, __NUM_OUT, __PORT_0_TYPE) \
	{\
		__MODULE,\
		__MODULE_NAME,\
		__MODULE_TYPE,\
		__LOCATION,\
		__NUM_IN,\
		__NUM_OUT,\
		{\
			__PORT_0_TYPE,\
		},\
	},
#else
#define __BUF_END(...)	ADEC_BUF_NOT_DEF,ADEC_BUF_NOT_DEF,ADEC_BUF_NOT_DEF,ADEC_BUF_NOT_DEF
#define __MOD_DECL(__MODULE, __MODULE_NAME, __MODULE_TYPE, __LOCATION, __NUM_IN, __NUM_OUT, __PORT_0_TYPE, __PORT_1_TYPE, __PORT_2_TYPE, __PORT_3_TYPE, ...) \
	{\
		__MODULE,\
		__MODULE_NAME,\
		__MODULE_TYPE,\
		__LOCATION,\
		__NUM_IN,\
		__NUM_OUT,\
		{\
			__PORT_0_TYPE,\
			__PORT_1_TYPE,\
			__PORT_2_TYPE,\
			__PORT_3_TYPE,\
		},\
	},
#endif
#define ADEC_MODULE_DEF(__MODULE, __MODULE_NAME, __MODULE_TYPE, __LOCATION, __NUM_IN, __NUM_OUT, ...)\
	__MOD_DECL(__MODULE, __MODULE_NAME, __MODULE_TYPE, __LOCATION, __NUM_IN, __NUM_OUT, __VA_ARGS__)
#include "adec_module_def.h"
#undef ADEC_MODULE_DEF
#undef __MOD_DECL
#undef __BUF_END

};

static ADEC_CORE_TYPE gCurrentCoreType = ADEC_CORE_NOT_DEF;

ADEC_RESULT		ModuleList_SetPlatform(ADEC_CORE_TYPE _core)
{
	gCurrentCoreType = _core;
	return ADEC_ERR_NONE;
}
ADEC_CORE_TYPE	ModuleList_GetPlatform(void)
{
	return gCurrentCoreType;
}

ADEC_CORE_TYPE	ModuleList_FindModule(ADEC_MODULE_ID _module)
{
	if ( _module >= ADEC_MODULE_MAX || _module <= 0)
	{
		return ADEC_CORE_NOT_DEF;
	}

	return gModuleInfo[_module].location;
}

ADEC_RESULT		ModuleList_GetModuleInfo(ADEC_MODULE_ID _module, ModuleInfo* _info)
{
	ADEC_ASSERT(_info != NULL,
	            return ADEC_ERR_NULL;);
	if ( _module >= ADEC_MODULE_MAX || _module <= 0)
	{
		return ADEC_ERR_RANGE;
	}

	//*_info = gModuleInfo[_module];
	ADEC_MEMCPY(NULL, _info, &gModuleInfo[_module], sizeof(ModuleInfo));

	return ADEC_ERR_NONE;
}

int ModuleList_GetModuleList(ADEC_MODULE_TYPE _type, ADEC_MODULE_ID *_result_array, int _array_size)
{
    int i;
    int result_cnt;
    ADEC_ASSERT(_result_array != NULL, return ADEC_ERR_NULL;);

    for (i = 0, result_cnt = 0 ; i < ADEC_MODULE_MAX && result_cnt < _array_size ; i ++) {
        if (gModuleInfo[i].type == _type) {
            _result_array[result_cnt] = (ADEC_MODULE_ID)i;
            result_cnt ++;
        }
    }

    return result_cnt;
}

const char 	   *ModuleList_GetModuleName(ADEC_MODULE_ID _module)
{
	if ( _module >= ADEC_MODULE_MAX || _module <= 0)
	{
		return "";
	}
	//*_info = gModuleInfo[_module];
	return gModuleInfo[_module].name;
}

static const char* gModuleTypeName[ADEC_MOD_TYPE_MAX+1] =
{
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_NOT_DEF],		"NOT_DEF"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_SOURCE],		"SOURCE"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_REFOMATTER],	"REFORM"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_SINK],			"SINK"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_DECODER],		"DEC"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_ENCODER],	 	"ENC"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_PCMCVT],		"PCMCVT"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_MIXER],		"MIX"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_SE],			"SE"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_LIPSYNC],		"LIPSYNC"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_INPUTCTRL],	"INPUTCTRL"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_OUTCTRL],		"OUTCTRL"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_ESOUTCTRL],    "ESOUTCTRL"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_BYPASS],		"BYPASS"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_MANAGER],		"MAN"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_DBG_MOR],		"DBG_MOR"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_SOLA],	    	"SOLA"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_CTRLSRC],		"CTRLSRC"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_DTOCTRL],		"DTOCTRL"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_EXT_BUF],      "EXTBUF"),
    __ADEC_FILED_INIT([ADEC_MOD_TYPE_RTS],          "RTS"),
	__ADEC_FILED_INIT([ADEC_MOD_TYPE_MAX],          "INVALID")
};

const char	   *ModuleList_GetModuleTypeName(ADEC_MODULE_TYPE _module_type)
{
	if (_module_type > ADEC_MOD_TYPE_MAX || _module_type < 0)
	{
		return "INVALID";
	}

	if (gModuleTypeName[_module_type] == NULL) {
	    return "UNKNOWN";
	}

	return gModuleTypeName[_module_type];
}


/** @} */
