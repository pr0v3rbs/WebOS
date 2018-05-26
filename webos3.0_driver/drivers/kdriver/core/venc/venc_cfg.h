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
 *  main configuration file for venc device
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.05.19
 *  note		Additional information.
 *
 *  @addtogroup lg1152_venc
 *	@{
 */

#ifndef	_VENC_CFG_H_
#define	_VENC_CFG_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
//#define VENC_USE_LOCK
//#define VENC_ENABLE_DEBUG
#define VENC_BIT_RATE_BUFFER	10

// 2ndTV 및 Wifi play를 위해 TS생성 문제에 대한 workaround로
// DVR 블럭을 통하지 않고 직접 down하기 위한 코드 추가
#define VENC_ENABLE_TS_OUT

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define	VENC_MODULE			"venc"
#define VENC_MAX_DEVICE		2
//#define VENC_MAX_MEM_CONFIG	4

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/

typedef enum
{
	LX_VENC_CFG_FLAG_NONE = 0x0,
	LX_VENC_CFG_FLAG_PM   = 0x1,
} LX_VENC_CFG_FLAG_T;

typedef struct
{
	char*	pcMEMName;
	UINT32	uiMEMBase;
	UINT32	uiMEMSize;
} LX_VENC_MEM_CFG_T;

typedef struct
{
	char*	pcInBufName;
	UINT32	uiInBufBase;
	UINT32	uiInBufSize;

	char*	pcVencBufName;
	UINT32	uiVencBufBase;
	UINT32	uiVencBufSize;
} LX_VENC_MEM_CFG_S_T;

typedef struct
{
	char*	name;
	UINT32	phys;
	UINT32	size;
} LX_VENC_MEM_UNIT_T;

typedef struct
{
	UINT32	total;
	LX_VENC_MEM_UNIT_T	esb;
	LX_VENC_MEM_UNIT_T	auib;
	LX_VENC_MEM_UNIT_T	cpb;
	LX_VENC_MEM_UNIT_T	input;
	LX_VENC_MEM_UNIT_T	thumb;
	LX_VENC_MEM_UNIT_T	hxenc;
} LX_VENC_MEM_USAGE_T;

typedef struct	
{
	UINT32	reg_base_core;		// h/w register
	UINT32	reg_size_core;		// h/w register
	UINT32	reg_base_internal;	// internal ipc register
	UINT32	reg_size_internal;	// internal ipc register
	UINT32	reg_base_external;	// externel ipc register (DE)
	UINT32	reg_size_external;	// externel ipc register (DE)

	UINT32	irq_num;
	UINT32	hw_clock;
	UINT32	hw_clock_mhz;

	LX_VENC_MEM_USAGE_T const *mem_usage;
} LX_VENC_CFG_CORE_T;

typedef struct	
{
	UINT32	chip;
	UINT32	num_device;	
	LX_VENC_CFG_FLAG_T	cfg_flag;
	LX_VENC_MEM_CFG_S_T const *mem_cfg;

	LX_VENC_CFG_CORE_T	core[VENC_MAX_DEVICE];
} LX_VENC_CFG_T;


/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
void VENC_CFG_MemoryMap ( void );
int VENC_CFG_Init( void );

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/
extern LX_VENC_CFG_T const	*gpstVencConfig;
extern LX_VENC_MEM_CFG_S_T 	*gpstVencMemConfig;
#if 0
extern LX_VENC_MEM_CFG_T 	gMemCfgVenc[VENC_MAX_MEM_CONFIG];
#else
extern LX_VENC_MEM_CFG_T	gMemCfgVenc;
#endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VENC_CFG_H_ */

/** @} */

