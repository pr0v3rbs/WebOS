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

#ifndef __MODULE_CMD_LIPSYNC_H__
#define __MODULE_CMD_LIPSYNC_H__

#include "imc/adec_imc_cmd.h"


/**
 * Lipsync Onoff mode를 정의한 구조체.
 */
typedef enum _eLIPSYNC_ONOFF_MODE
{
    LIPSYNC_OFF,		///< OFF
	LIPSYNC_ON,			///< ON
    LIPSYNC_ONOFF_MAX,	///<
}eLIPSYNC_ONOFF_MODE;

/**
 * Lipsync Onoff mode를 정의한 구조체.
 */
typedef enum _eLIPSYNC_CLOCK_TYPE
{
    LIPSYNC_CLOCK_PCRM			= 0x01,			///< pcr main
    LIPSYNC_CLOCK_PCRA			= 0x02,			///< pcr associate
    LIPSYNC_CLOCK_NONPCR		= 0x11,			///< non pcr
    LIPSYNC_CLOCK_GSTC 			= 0x100,		///< gstc
    LIPSYNC_CLOCK_AUDIOMASTER	= 0x200,		///< audio master
}eLIPSYNC_CLOCK_TYPE;

/**
 * Set lip-sync data type을 정의한 enum.
 */
typedef enum _eLIPSYNC_DATA_TYPE{
	LIPSYNC_DATATYPE_PCM=0,
	LIPSYNC_DATATYPE_ES
}eLIPSYNC_DATA_TYPE;


#define LIPSYNC_CMD_SET_ONOFF			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x00), LipsyncCmdSetOnoff)
#define LIPSYNC_CMD_SET_BOUND			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x01), LipsyncCmdSetBound)
#define LIPSYNC_CMD_SET_CLOCKTYPE		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x02), LipsyncCmdSetclocktype)
#define LIPSYNC_CMD_SET_BASE			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x03), LipsyncCmdSetBase)
#define LIPSYNC_CMD_SET_FS              ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x04), LipsyncCmdSetFs)
#define LIPSYNC_CMD_SET_DEBUGPRINT      ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x05), LipsyncCmdSetDebugprint)
#define LIPSYNC_CMD_SET_NODELAY			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x06), LipsyncCmdSetNodelay)
#define LIPSYNC_CMD_SET_RATE			ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x07), LipsyncCmdSetRate)
#define LIPSYNC_CMD_SET_DATATYPE		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x08), LipsyncCmdSetDatatype)
#define LIPSYNC_CMD_SET_BUFAFTERLIP		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x09), LipsyncCmdSetBufAfterLip)
#define LIPSYNC_CMD_SET_IDLETIME		ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0X0A), LipsyncCmdSetIdletime)
#define LIPSYNC_CMD_PLAYTO				ADEC_CMD(ADEC_CMD_MOD_ID(CMD_LIPSYNC, 0x0B), LipsyncCmdPlayto)


/**
 * Set lip-sync on/off를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetOnoff{
	// 0 : off ,  1 : on
	eLIPSYNC_ONOFF_MODE onoff;
}LipsyncCmdSetOnoff;

/**
 * Set lip-sync boundary를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetBound{
	unsigned int lbound;
	unsigned int ubound;
	unsigned int offset;
	unsigned int freerunlbound;
	unsigned int freerunubound;
}LipsyncCmdSetBound;

/**
 * Set clock type을 정의한 구조체.
 */
typedef struct _LipsyncCmdSetclocktype{
	// 0x01 : pcrM,  0x02 : pcrA, 0x11 : None pcr, 0x100 : gstc
	eLIPSYNC_CLOCK_TYPE clocktype;
}LipsyncCmdSetclocktype;

/**
 * Set lip-sync Base를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetBase{
	unsigned int clockbase;
	unsigned int streambase;
}LipsyncCmdSetBase;

/**
 * Set lip-sync Fs를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetFs{
	unsigned int Fs;
}LipsyncCmdSetFs;

/**
 * Set lip-sync debug용 print 시간 간격을 정의한 구조체.
 */
typedef struct _LipsyncCmdSetDebugprint{
	unsigned int interval;				///< ms, 0 is off,
}LipsyncCmdSetDebugprint;

/**
 * Set lip-sync no-delay를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetNodelay{
	unsigned int onoff;
	unsigned int upperthreshold;		///< msec
	unsigned int lowerthreshold;		///< msec
	unsigned int prebyteper1sec;		///< bytes(per sample) * channel * Hz (ex 4byte * 2ch * 48000 Hz)
	unsigned int posbyteper1sec;		///< maybe 384000 in H13 (4byte * 2ch * 48000 Hz)
}LipsyncCmdSetNodelay;

/**
 * Set lip-sync rate를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetRate{
	unsigned int in; 					///< ex) x1 : 100 , x2 : 200 , x0.5 : 50
	unsigned int out;					///< alway 100
}LipsyncCmdSetRate;

/**
 * Set lip-sync data type을 정의한 구조체.
 */
typedef struct _LipsyncCmdSetDatatype{
	eLIPSYNC_DATA_TYPE datatype;				///< eLIPSYNC_DATA_TYPE, 0 is pcm, 1 is ES
}LipsyncCmdSetDatatype;

/**
 * Set lip-sync 이후의 buffer를 정의한 구조체.
 */
typedef struct _LipsyncCmdSetBufAfterLip
{
    unsigned int		num_of_modules;     ///< Num of modules connected in series
    struct {
        unsigned int	module_id;      	///< The module ID
        unsigned int	module_port;    	///< Port of the module. Recommended : Using input port only
    }module_list[5];
}LipsyncCmdSetBufAfterLip;

/**
 * Set lip-sync 의 idle에서 빠져 나오는 time
 */
typedef struct _LipsyncCmdSetIdletime
{
	unsigned int msec;						// ms ;
}LipsyncCmdSetIdletime;

/**
 * 내려준 pts까지 running 하고 stop 하는 cmd
 */
typedef struct _LipsyncCmdPlayto
{
	unsigned int time_stamp;						
}LipsyncCmdPlayto;

#endif
