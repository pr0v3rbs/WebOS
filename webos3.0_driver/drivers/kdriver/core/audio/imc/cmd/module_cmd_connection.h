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

#ifndef __MODULE_CMD_CONNECTION_H__
#define __MODULE_CMD_CONNECTION_H__

#include "imc/adec_imc_cmd.h"


#define CM_CMD_CONNECT					ADEC_CMD(0xF000, CmCmdConnect)
#define CM_CMD_DISCONNECT				ADEC_CMD(0xF001, CmCmdDisconnect)
#define CM_CMD_ALLOC_BUF				ADEC_CMD(0xF010, CmCmdAllocBuf)
#define CM_CMD_DEALLOC_BUF				ADEC_CMD(0xF011, CmCmdDeallocBuf)
#define CM_CMD_CREATE_MODULE			ADEC_CMD(0xF020, CmCmdCreateModule)
#define CM_CMD_DESTROY_MODULE			ADEC_CMD(0xF021, CmCmdDestroyModule)
#define CM_CMD_GET_FREE_CLIENT			ADEC_CMD(0xF080, CmCmdGetFreeClient)
#define CM_CMD_GET_PORT_INFO			ADEC_CMD(0xF081, CmCmdGetPortInfo)
#define CM_CMD_SET_REF_PORTS			ADEC_CMD(0xF090, CmCmdSetRefPorts)
#define CM_CMD_CLR_REF_PORTS			ADEC_CMD(0xF091, CmCmdClrRefPorts)
#define CM_CMD_CLR_ALL_REF_PORTS		ADEC_CMD(0xF092, CmCmdClrAllRef)
#define CM_CMD_GET_DELAY                ADEC_CMD(0xF0D0, CmCmdGetDelay)


/**
 * Connection Manager Connect command를 정의한 구조체.
 */
typedef struct _CmCmdConnect
{
	unsigned int		input_module;
	unsigned int		input_port;
	unsigned int		output_module;
	unsigned int		output_port;
}CmCmdConnect;

/**
 * Connection Manager Disconnect command를 정의한 구조체.
 */
typedef struct _CmCmdDisconnect
{
	unsigned int		module;
	unsigned int		port;
}CmCmdDisconnect;

/**
 * Connection Manager Allocate Buffer command를 정의한 구조체.
 */
typedef struct _CmCmdAllocBuf
{
	unsigned int		module;
	unsigned int		port;
	unsigned int		physical_addr;
	unsigned int		register_addr_offset;
	unsigned int		is_reader;				///< If reader 1, else 0. (explicit check)
}CmCmdAllocBuf;

/**
 * Connection Manager Release Buffer command를 정의한 구조체.
 */
typedef struct _CmCmdDeallocBuf
{
	unsigned int		module;
	unsigned int		port;
}CmCmdDeallocBuf;

/**
 * Connection Manager module specific를 정의한 유니온.
 */
typedef union _CmCmdModuleSpecific
{
	struct{
		unsigned int		media_type;
	}					codec_param;
	struct{
		unsigned int		src_type;			// 0 for SW, 1 for HW
	}					src_param;
}CmCmdModuleSpecific;

/**
 * Connection Manager Create Module command를 정의한 구조체.
 */
typedef struct _CmCmdCreateModule
{
	unsigned int		module_id;
	CmCmdModuleSpecific module_param;
}CmCmdCreateModule;


/**
 * Connection Manager Destory Module command를 정의한 구조체. 
 */
typedef struct _CmCmdDestroyModule
{
	unsigned int		module_id;
}CmCmdDestroyModule;


/**
 * Connection Manager Get Free Client command를 정의한 구조체. 
 */
typedef struct _CmCmdGetFreeClient
{
	unsigned int		module;
	unsigned int		out_port;
}CmCmdGetFreeClient;

/**
 * Connection Manager Get Free Client response를 정의한 구조체. 
 */
typedef struct _CmRspGetFreeClient
{
	unsigned int		total;
	unsigned int		num_of_free;
}CmRspGetFreeClient;


/**
 * Connection Manager Get Port Info command를 정의한 구조체. 
 */
typedef struct _CmCmdGetPortInfo
{
	unsigned int		module;
	unsigned int		port;
}CmCmdGetPortInfo;

/**
 * Connection Manager Get Port Info response를 정의한 구조체. 
 */
typedef struct _CmRspGetPortInfo
{
	unsigned int		direction; 		///< if 0 input, else output
	unsigned int		buf_type;		///< Refer to ADEC_BUF_TYPE
	// Bellows are valid when connect_type is 0.
	unsigned int		connected_module;
	unsigned int		connected_port;
}CmRspGetPortInfo;

/**
 * Connection Manager Register Reference Port command를 정의한 구조체. 
 */
typedef struct _CmCmdSetRefPorts
{
	unsigned int		ref_module;		///< A module to register reference
	unsigned int		num_of_ref;		///< Num of references in this command
	unsigned int		ref_port[4];
	unsigned int		src_module[4];
	unsigned int		src_port[4];
}CmCmdSetRefPorts;



/**
 * Connection Manager Clear Reference Port command를 정의한 구조체. 
 */
typedef struct _CmCmdClrRefPorts
{
	unsigned int		module;
	unsigned int		num_of_ref;
	unsigned int		ref_port[4];
}CmCmdClrRefPorts;

/**
 * Connection Manager Clear All Reference Port command를 정의한 구조체. 
 */
typedef struct _CmCmdClrAllRef
{
	unsigned int		module;
}CmCmdClrAllRef;

/**
 * Connection Manager enum. 
 */
enum {
	CM_CNST_MODULE_LIST_MAX	= 8,
	CM_CNST_DEFAULT_BYTES_PER_SEC = 48000*4*2,
};

/**
 * Connection Manager Get delay by the module buffers command를 정의한 구조체. 
 */
typedef struct _CmCmdGetDelay
{
    unsigned int        num_of_modules;     ///< Num of modules connected in series
    struct {
        unsigned int        module_id;      ///< The module ID
        unsigned int        module_port;    ///< Port of the module. Recommended : Using input port only
        unsigned int        bytes_per_sec;  ///< Average bytes per second of the data. For example 48Khz 32Bit PCM : 48000 * 4 * 2
                                            // If this value is less than or equal to 0, choose the defulat value(48000 * 4 * 2)
    }                   module_list[CM_CNST_MODULE_LIST_MAX];
}CmCmdGetDelay;

/**
 * Connection Manager Get delay by the module buffers response를 정의한 구조체. 
 */
typedef struct _CmRspGetDelay
{
    unsigned int        total_delay;        ///< Total delay in ms
    unsigned int        num_of_modules;     ///< Num of modules used to estimate delay
    struct {
        unsigned int        module_id;      ///< Module ID
        unsigned int        module_port;    ///< Port
        unsigned int        bytes_in_buf;   ///< Total bytes in buf
        unsigned int        delay_in_ms;    ///< Estimated delay in ms
    }                   module_list[CM_CNST_MODULE_LIST_MAX];
}CmRspGetDelay;

#endif
