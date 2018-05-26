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



/** @file HDMI2020_kapi.h
 *
 *  application interface header for HDMI20 device
 *
 *  @author		won.hur (won.hur@lge.com)
 *  @version	1.0	
 *  @date		2014.04.29
 *
 *  @addtogroup lg115x_HDMI20
 *	@{
 */

#ifndef	_HDMI20_KAPI_H_
#define	_HDMI20_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "hdmi_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	HDMI20_IOC_MAGIC		'h'

#define HDMI20_NUM_OF_RX_PORTS		2	
#define HDMI20_NUM_OF_TX_PORTS		1	

#define HDMI20_NUM_OF_PORTS		(HDMI20_NUM_OF_TX_PORTS + HDMI20_NUM_OF_RX_PORTS)

/** Receiver key selection size - 40 bits */
#define HDMI20_HDCP_BKSV_SIZE	(2 *  1)
/** Encrypted keys size - 40 bits x 40 keys */
#define HDMI20_HDCP_KEYS_SIZE	(2 * 40)
#define HDMI20_HDCP_CUSTOMER_SIZE	288

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
/* HDMI20 Link Control */
#define HDMI20_IOC_COMMAND			_IOWR(HDMI20_IOC_MAGIC,  0, LX_HDMI20_CMD_T *)
#define HDMI20_IOC_SET_LOGM			_IOWR(HDMI20_IOC_MAGIC,  1, LX_HDMI20_LOGM_SETTING_T *)
#define HDMI20_IOC_MAXNR			(_IOC_NR(HDMI20_IOC_SET_LOGM) + 1)	

/* HDCP2.2 Control */
#define ESM_HOSTLIB_COMMAND _IOWR('q', 1, esm_hostlib_cmd_t *)
#define ESM_HOSTLIB_STATUS  _IOWR('q', 2, esm_hostlib_status_t *)


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/* IOCTL Commands */
#define HDMI20_CMD_RX_INITIALIZE			1
#define HDMI20_CMD_RX_UNINITIALIZE			2
#define HDMI20_CMD_TX_INITIALIZE			3
#define HDMI20_CMD_TX_UNINITIALIZE			4
#define HDMI20_CMD_RX_GET_TIMINGINFO		5
#define HDMI20_CMD_RX_GET_AVIINFO			6
#define HDMI20_CMD_RX_GET_SPDINFO			7
#define HDMI20_CMD_RX_GET_VSIINFO			8
#define HDMI20_CMD_RX_GET_COLORINFO			9
#define HDMI20_CMD_RX_GET_AKSVDATA			10	
#define HDMI20_CMD_RX_GET_PHYSTATUS			11
#define HDMI20_CMD_RX_SET_RESETTIMINGINFO	12
#define HDMI20_CMD_RX_SET_EDID				13
#define HDMI20_CMD_RX_GET_EDID				14
#define HDMI20_CMD_RX_SET_HDCP14			15
#define HDMI20_CMD_RX_GET_HDCP14			16
#define HDMI20_CMD_RX_SET_ENABLEHPD			17
#define HDMI20_CMD_RX_SET_ENABLEEDID		18
#define HDMI20_CMD_RX_GET_AUDIOINFO			19
#define HDMI20_CMD_RX_GET_AUDIOCOPYINFO		20
#define HDMI20_CMD_RX_SET_HDMIARC			21
#define HDMI20_CMD_RX_SET_MUTE				22
#define HDMI20_CMD_RX_GET_AUDIODBGINFO		23	
#define HDMI20_CMD_TX_SET_INPUTMUX			24
#define HDMI20_CMD_TX_GET_INPUTMUX			25

#define HDMI20_CMD_DBG_SET_PRESETS			26
#define HDMI20_CMD_RX_GET_ASPECTRATIO		27
#define HDMI20_CMD_RX_GET_HDMISTATE			28
#define HDMI20_CMD_RX_SET_HDCP22INFO		29
#define HDMI20_CMD_RX_GET_SRCINFO			30
#define HDMI20_CMD_RX_SET_PHY_ACCESS		31
#define HDMI20_CMD_RX_OPEN					32
#define HDMI20_CMD_RX_SET_ENABLE_OPERATION	33


#define HDMI20_CMD_RX_DBG_TOOL				34	

#define HDMI20_CMD_LAST			HDMI20_CMD_RX_DBG_TOOL
#define HDMI20_CMD_MAXNUM		(HDMI20_CMD_LAST	+ 1)		

/* COMMAND STATUS */
#define HDMI20_CMD_STATUS_PROCESS_ERROR			-7
#define HDMI20_CMD_STATUS_COPY_ERROR			-6
#define HDMI20_CMD_STATUS_IOCTL_ERROR	 		-5
#define HDMI20_CMD_STATUS_SIZE_ERROR	 		-4
#define HDMI20_CMD_STATUS_ATTRI_ERROR	 		-3
#define HDMI20_CMD_STATUS_HANDLE_ERROR 			-2
#define HDMI20_CMD_STATUS_CMD_ERROR				-1
#define HDMI20_CMD_STATUS_OK			 		 0

typedef struct 
{
	int 	command;
	void*	handle;
	int		size;
	int		attributes;
	int		status;
} LX_HDMI20_CMD_T;

typedef struct 
{
	UINT32 logLevel;
	BOOLEAN onOff;	 
} LX_HDMI20_LOGM_SETTING_T;

typedef enum
{
	LX_HDMI20_TX_INPUT_MAINVIDEO = 0,
	LX_HDMI20_TX_INPUT_OSD,
	LX_HDMI20_TX_INPUT_SUBVIDEO,
	LX_HDMI20_TX_INPUT_INTERLACED_OIF,
	LX_HDMI20_TX_INPUT_MAXNUM
} LX_HDMI20_TX_INPUTMUX_TYPE_T;

typedef struct
{
	unsigned int addr;
	unsigned int value;
} LX_HDMI20_REG_T;

typedef struct
{
	char index[100];
	LX_HDMI20_REG_T* regs;
	UINT32 count;
} LX_HDMI20_REG_SETTING_T;

typedef struct
{
	LX_HDMI20_REG_SETTING_T	*pRegSettings;
	UINT32 number_of_modes;
} LX_HDMI20_REG_SETS_T;

typedef struct 
{
	BOOLEAN repeat;
	UINT32 seed;
	/**
	 * Receiver key selection
	 * @note 0: high order, 1: low order
	 */
	UINT32 bksv[HDMI20_HDCP_BKSV_SIZE];
	/**
	 * Encrypted keys
	 * @note 0: high order, 1: low order
	 */
	UINT32 keys[HDMI20_HDCP_KEYS_SIZE];
} LX_HDMI20_HDCP14_DATA_T;

typedef enum
{
	HDMI20_HAL_IPC_REQUEST_HPD = 0x00000001,
	HDMI20_HAL_IPC_GOOD_VIDEO  = 0x00000002,
	HDMI20_HAL_IPC_BAD_VIDEO   = 0x00000004,
	HDMI20_HAL_IPC_RESET_VIDEO = 0x00000008,
	HDMI20_HAL_IPC_MAXNUM
} HDMI20_HAL_IPC_T;
	
typedef struct
{
	int number_of_ports;
	unsigned int ipc_address[HDMI20_NUM_OF_PORTS];
} LX_HDMI20_INIT_T;

typedef struct
{
	BOOLEAN isHDMI20Device;
} LX_HDMI20_SRCINFO_T;

typedef struct
{
	BOOLEAN isWrite;
	UINT8 slaveAddr;
	UINT8 regAddr;
	UINT32 data;
} LX_HDMI20_PHY_ACCESS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _HDMI20_DRV_H_ */

/** @} */
