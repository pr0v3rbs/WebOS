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
 *  ipc def header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.08
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _IPC_DEF_H_
#define _IPC_DEF_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define MCU_IPC_DEBUG

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define IPC_TOTAL_SIZE		 	0x20
#define IPC_BUFF_NUM			 100
#define MSG_STR_MAX			 2048

#define IPC_FROM_DE_SIZE	 0x8
#define IPC_FROM_CPU_SIZE	 ((IPC_TOTAL_SIZE-IPC_FROM_DE_SIZE)/2)
#define IPC_FROM_MCU_SIZE	 ((IPC_TOTAL_SIZE-IPC_FROM_DE_SIZE)/2)

#define IPC_STR_MAX			 IPC_FROM_CPU_SIZE

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	FWI_IPC_SIZE ,
	FWI_IPC_CONT ,
	FWI_IPC_FROM ,
	FWI_IPC_TYPE ,
	FWI_IPC_DATA ,
} FWI_IPC_CMD_T;

typedef enum {
	FWI_IPC_FROM_UART = 1 ,
	FWI_IPC_FROM_HOST = 2 ,
	FWI_IPC_FROM_MAX = 0xFFFF
} FWI_IPC_FROM_T;

typedef enum { 
	FWI_IPC_CMD_CLI = 1 ,
	FWI_IPC_CMD_VAL = 2 ,
	FWI_IPC_CMD_RET = 3 ,	// MCU -> CPU return value
	FWI_IPC_CMD_DBG = 4, 	// MCU -> CPU request for DEBUG : switch UART0 (MCU to CPU), MCU reset etc.
	FWI_IPC_CMD_SYNC = 5,	// MCU -> CPU interrupt : VSYNC rate changed -> PWM frequency change
	FWI_IPC_CMD_MAX = 0xFFFF
} FWI_IPC_DATA_TYPE_T;

typedef enum {
	FWI_IPC_CMD_DBG_UART_SWITCH	= 0xF0000000,	// DEBUG only (MCU -> CPU)
	FWI_IPC_CMD_DBG_RESET
} FWI_IPC_CMD_DBG_TYPE_T;

typedef enum {
	FWI_IPC_OK,
	FWI_IPC_BUSY,
	FWI_IPC_EMPTY,
	FWI_IPC_LAST,
} FWI_IPC_STATUS_T;

typedef struct {
	UINT16 u16From;
	UINT16 u16Type;
	UINT32 u32Cmd;
	UINT32 u32Data;
} FWI_IPC_MSG_T;

typedef struct {
	UINT32 wInx;
	UINT32 rInx;
	UINT32 queMax;
	UINT32 queOverflow;
	UINT8  str[IPC_BUFF_NUM][IPC_STR_MAX];
} FWI_EXCHANGE_T;

typedef enum {
	INT_HWI,
	INT_SWI
} BE_INT_TYPE_T;

typedef enum {
	INT_CPU_MCU_ACK,
	INT_CPU_MCU_IPC,
	INT_MCU_CPU_ACK,
	INT_MCU_CPU_IPC,
	INT_DE_MCU_ACK,
	INT_DE_MCU_IPC,
	INT_TYPE_MAX
} BE_INT_DIR_TYPE_T;

typedef enum {
	MCU_INT_WDG = 0,
	MCU_INT_GPIO,
	MCU_INT_DMA,
	MCU_INT_SWI,
	MCU_INT_MAX = 32
} BE_MCU_INT_TYPE_T;  

typedef enum {
	CPU_INT_WDG = 0,
	CPU_INT_DMA,
	CPU_INT_LED,
	CPU_INT_DPPA,
	CPU_INT_SRE,
	CPU_INT_LRX,
	CPU_INT_DVO,
	CPU_INT_DVI,
	CPU_INT_H3D0,
	CPU_INT_H3D1,
	CPU_INT_SWI = 15, // 15 ~ 31
	CPU_INT_MAX = 32
} BE_CPU_INT_TYPE_T;

typedef enum {
	DE_INT_WDG = 0,
	DE_INT_DMA,
	DE_INT_LED,
	DE_INT_DPPA,
	DE_INT_SRE,
	DE_INT_LRX,
	DE_INT_DVO,
	DE_INT_DVI,
	DE_INT_H3D = 8,  // 8 ~ 9

	DE_INT_SWI = 10, // 10 ~ 31

	DE_INT_MAX = 32
} BE_DE_INT_TYPE_T;

typedef enum {
	SWI_TYPE_IPC = 0x00000001,	
	SWI_TYPE_ACK = 0x00000002,

	SWI_MAX
} BE_SWI_TYPE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _IPC_DEF_H_ */

/** @} */











