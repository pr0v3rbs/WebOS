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

#ifndef __MODULE_CMD_CTRLSRC_H__
#define __MODULE_CMD_CTRLSRC_H__

#include "imc/adec_imc_cmd.h"

#define CTRLSRC_CMD_SET_SRC_MODULE          ADEC_CMD(ADEC_CMD_MOD_ID(CMD_CTRL_SRC, 0x00), CtrlsrcCmdSetSrcModule)
#define CTRLSRC_CMD_SET_INTINFO             ADEC_CMD(ADEC_CMD_MOD_ID(CMD_CTRL_SRC, 0x01), CtrlsrcCmdSetIntInfo)
#define CTRLSRC_CMD_SET_SRCRUNNINGMODE		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_CTRL_SRC, 0x02), CtrlsrcCmdSetSrcrunningmode)

/**
 * Set src module to control command를 정의한 구조체.
 */
typedef struct _CtrlsrcCmdSetSrcModule
{
    unsigned int        src_module_id;
}CtrlsrcCmdSetSrcModule;

/*typedef enum _INTCHECK_INDEX
{
    INTCHECK_INDEX_NOTDEF,
    INTCHECK_INDEX_PCM,
    INTCHECK_INDEX_PCMSUB,
    INTCHECK_INDEX_SPDIF,
    INTCHECK_INDEX_HWSRCI,
    INTCHECK_INDEX_HWSRCO,
    INTCHECK_INDEX_HDMI0IN,    
    INTCHECK_INDEX_HDMI1IN,    
    INTCHECK_INDEX_ADCIN,    
    INTCHECK_INDEX_AADIN,  
    INTCHECK_INDEX_FS23,
    INTCHECK_INDEX_FS24,    
    INTCHECK_INDEX_MAX,
}INTCHECK_INDEX;
*/

/**
 * Set interrupt infomation command를 정의한 구조체.
 */
typedef struct _CtrlsrcCmdSetIntInfo
{
	unsigned int RefInt;
	unsigned int RefIntFs;
	unsigned int TarInt;
	unsigned int TarIntFs;	
}CtrlsrcCmdSetIntInfo;

/**
 * Set src running mode command를 정의한 구조체.
 */
typedef struct _CtrlsrcCmdSetSrcrunningmode
{
    unsigned int	src_running_mode;		///< 0 is default, 1 is fastmode ;
}CtrlsrcCmdSetSrcrunningmode;


#endif
