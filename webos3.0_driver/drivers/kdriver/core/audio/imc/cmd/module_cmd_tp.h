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

#ifndef __MODULE_CMD_TP_H__
#define __MODULE_CMD_TP_H__

#include "imc/adec_imc_cmd.h"

#define TP_CMD_SET_PARAM		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_TP, 0x00), TpCmdSetParam)



/**
  * TP Module Parameter ±¸Á¶Ã¼.
  * codec_type : PES decoder codec type
  *                    - ADEC_MEDIA_MS10_DDC = 2,
  *                    - ADEC_MEDIA_MS10_DDT = 3,
  *                    - ADEC_MEDIA_DRA      = 4,
  *                    - ADEC_MEDIA_MP3      = 5,
  * interval : TP Timer Interval
  *              - Normal : 0x2000 (default)
  *              - MHEG : 0x200
  * interrupt_type : TP Interrupt Processing Type
  *                      - INT_TYPE_TP =  1 : TP Interrupt
  *                      - INT_TYPE_TIMER = 2 : Timer Interrupt (Default)
  */
typedef struct _TpCmdSetParam
{
    unsigned int codec_type;            	///< PES decoder codec type 
    unsigned int interrupt_type;         	///< TP Interrupt Type 
    unsigned int interval;                  ///< TP Timer Interval 
} TpCmdSetParam;


#endif // #ifndef __MODULE_CMD_TP_H__

