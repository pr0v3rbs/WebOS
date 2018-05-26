/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file ci_kapi.h
 *
 *  application interface header for ci device
 *
 *  @author		eunjoo.kim (eunjoo.kim@lge.com)
 *  @author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  @version		1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef	_CI_KAPI_H_
#define	_CI_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	CI_IOC_MAGIC		'a'
/**
@name CI IOCTL List
ioctl list for ci device. Also the list includes IOCTLs for ci plus

@{
@def CI_IO_RESET
Reset ci module.
When ci device receives above IOCTL, then reset itself.

@def CI_IO_CHECK_CIS
Reads CIS(Card Information Struncture) from CI module

@def CI_IO_WRITE_COR
write COR(Configuration Option Register) data in the attribution memory of ci module.

@def CI_IOR_DETECT_CARD
Get the status of insertion or removal of CI module.

@def CI_IOR_READ_DATA
Read data from CI.

@def CI_IOR_NEGOTIATE_BUF_SIZE
Get the negotiated buffer size.
Host reads memory size supported by CI module from its CIS. Host compares its memory size with memory size read from CI module. And Host decides the size of which memory will be used to read and write between Hose and moduel
- Host choices the smaller one between the sizes supported by Host & CI module.

@def CI_IOR_READ_DA_STATUS
Read from stauts register if Data Abailable is set.

@def CI_IOW_SWITCH_BYPASS_MODE
Set or Clear the Bypass mode.

@def CI_IOW_WRITE_DATA
Write data to CI module

@def CI_IO_SET_PHY_RESET
Reset Physical Interface for PCMCIA module.
This is for CI plus

@def CI_IOR_READ_IIR_STATUS
Read IIR(Initialze Interface Request) from status register.
This is for CI plus.
CICAM can request Physical Interface Reset by setting IIR. If CICAM set IIR, it shall not send APDU.

@def CI_IOR_GET_DATA_RATE
Get the maxmimum data rate supported by Host.
This is for CI plus
Host must support 72Mbps, and support for 96Mbps is optional.

@def CI_IOR_CHECK_CAPABILITY
Get the type of CI CAM from CIS data.
If CAM is for CI, it returns 0
If CAM is for CI Plus, it returens 1.

@def CI_IOR_CAM_INIT
CAM initialize (manual hot swap)

@def CI_IOR_CAM_POWEROFF
CAM Power off

@def CI_IOR_CAM_POWERONCOMPLETED
CAM Power on


*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define	CI_IO_RESET						_IO(CI_IOC_MAGIC, 0 )
#define CI_IO_CHECK_CIS					_IO(CI_IOC_MAGIC, 1 )
#define CI_IO_WRITE_COR					_IO(CI_IOC_MAGIC, 2 )
#define CI_IOR_DETECT_CARD				_IOR(CI_IOC_MAGIC, 3, UINT32 )
#define	CI_IOR_READ_DATA				_IOR(CI_IOC_MAGIC, 4, LX_CI_IOCTL_PARAM_T )
#define CI_IOR_NEGOTIATE_BUF_SIZE		_IOR(CI_IOC_MAGIC, 5, UINT32 )
#define CI_IOR_READ_DA_STATUS			_IOR(CI_IOC_MAGIC, 6, UINT32 )
//#define CI_IOW_SWITCH_BYPASS_MODE		_IOW(CI_IOC_MAGIC, 7, UINT32 )
#define CI_IOW_WRITE_DATA				_IOW(CI_IOC_MAGIC, 8, LX_CI_IOCTL_PARAM_T)
#define	CI_IO_SET_PHY_RESET				_IO(CI_IOC_MAGIC, 9 )
#define	CI_IOR_READ_IIR_STATUS			_IOR(CI_IOC_MAGIC, 10, UINT32)
//#define CI_IOR_GET_DATA_RATE			_IOR(CI_IOC_MAGIC, 11, UINT32)
#define	CI_IOR_CHECK_CAPABILITY			_IOR(CI_IOC_MAGIC, 12, LX_CI_IOCTL_PARAM_CAMTYPE)
#define	CI_IOR_CAM_INIT					_IOR(CI_IOC_MAGIC, 13, UINT32)
#define	CI_IOR_PRINT_REG				_IOR(CI_IOC_MAGIC, 14, UINT32)
#define	CI_IOR_CAM_POWEROFF				_IOR(CI_IOC_MAGIC, 15, UINT32)
#define	CI_IOR_CAM_POWERONCOMPLETED		_IOR(CI_IOC_MAGIC, 16, UINT32)
#define	CI_IOR_GET_CIPLUS_VERSION		_IOR(CI_IOC_MAGIC, 17, UINT32)
#define	CI_IOR_GET_CIPLUS_OPROFILE		_IOR(CI_IOC_MAGIC, 18, UINT32)

#define	CI_IOW_CAM_SET_DELAY			_IOW(CI_IOC_MAGIC, 19, LX_CI_IOCTL_PARAM_SETDELAY)
#define	CI_IO_CAM_PRINT_DELAY_VALUES	_IO(CI_IOC_MAGIC, 20 )

#define	CI_IO_READ_REGISTERS			_IO(CI_IOC_MAGIC, 21 )
#define	CI_IOW_WRITE_REGISTER			_IOW(CI_IOC_MAGIC, 22, LX_CI_IOCTL_PARAM_REGISTER )
#define CI_IO_SET_RS					_IO(CI_IOC_MAGIC, 23 )
#define CI_IOW_SET_PCMCIA_SPEED			_IOW(CI_IOC_MAGIC, 24, LX_CI_IOCTL_PARAM_SPEED )

// Debug
#define CI_IOW_ENABLE_LOG				_IOW(CI_IOC_MAGIC, 25, LX_CI_IOCTL_PARAM_LOG_ENABLE)

#define CI_IOW_CHANGE_ACCESSMODE		_IOW(CI_IOC_MAGIC, 26, LX_CI_IOCTL_PARAM_ACCESSMODE_CHANGE)

#define CI_IO_DATA_TEST					_IO(CI_IOC_MAGIC, 27 )

#define CI_IOWR_OPTION					_IOWR(CI_IOC_MAGIC, 28, LX_CI_IOCTL_PARAM_OPTION)

#define CI_IOC_MAXNR				29
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * ci delay type
 * used in interface btw kadp/kdrv and in kdrv functions
 * @see
 */
typedef enum
{
	CI_D_CIS_CONFIG_FIRST_TUPLE_OK = 0,			// 5
	CI_D_CIS_CONFIG_FIRST_TUPLE_NG,				// 10
	CI_D_CIS_END_WRITE_COR,						// 100
	CI_D_CIS_DURING_READ_TUPLE,					// 5
	CI_D_CIS_END_READ_TUPLE_INITIAL,			// SLEEP_VALUE_INIT, 20
	CI_D_CIS_PARSE_NON_CI_TUPLE,				// 3
	CI_D_INIT_POWER_CONTROL,					// H13 Blocked, L9 5
	CI_D_INIT_AFTER_INTERRUPT_ENABLE,			// 10
	CI_D_CAM_INIT_BTW_VCC_CARDRESET,			// 300
	CI_D_CAM_INIT_BTW_CARDRESET_NOTRESET,		// 5
	CI_D_IO_SOFT_RESET_CHECK_FR,				// 10
	CI_D_IO_END_SOFT_RESET,						// L9 Blocked, 0 Originally (10)
	CI_D_IO_NEGOBUF_BEFORE_SOFTRESET,			// L9 Blocked, 0 Originally (100)
	CI_D_IO_NEGOBUF_CHECK_DA,					// 10
	CI_D_IO_NEGOBUF_CHECK_FR,					// 10
	CI_D_IO_NEGOBUF_AFTER_WRITE_DATA,			// 5 , Previously (10)
	CI_D_IO_READ_CHECK_DA,						// 10
	CI_D_IO_WRITE_CHECK_DA,						// 10
	CI_D_IO_WRITE_CHECK_FR,						// 10
	CI_D_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE,		// 10
	CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE,			// 10
	CI_D_IO_WRITE_LAST_BYTE_CHECK_WE,			// 10
	CI_D_ENUM_MAX
}CI_DELAY_TYPE_T;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see CI_IOW_WRITE_STRUCT
 */
typedef struct
{
	UINT8*	pBuf;				///< type of codec
	UINT16	sLength;		///< optional parameter.
}
LX_CI_IOCTL_PARAM_T;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see CI_IOR_CHECK_CAPABILITY
 */
typedef struct LX_CI_IOCTL_PARAM_CAMTYPE_T
{
	SINT8 uwRtnValue;			///< return value
	UINT8 uwCheckCAMType;		///< CAM type.
}
LX_CI_IOCTL_PARAM_CAMTYPE;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see CI_IOR_GET_CIPLUS_VERSION
 */
typedef struct LX_CI_IOCTL_PARAM_VERSION_T
{
	SINT8 uwRtnValue;			///< return value
	UINT32 uwVersion;			///< Support Version
}
LX_CI_IOCTL_PARAM_VERSION;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see CI_IOR_CHECK_CAPABILITY
 */
typedef struct LX_CI_IOCTL_PARAM_OPROFILE_T
{
	SINT8 uwRtnValue;			///< return value
	UINT32 uwProfile;			///< Operator Profile
}
LX_CI_IOCTL_PARAM_OPROFILE;




/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see
 */
typedef struct
{
	CI_DELAY_TYPE_T eDelayType;		///< Target Delay
	UINT32 uiDelayValue;			///< Delay Value in ms
}
LX_CI_IOCTL_PARAM_SETDELAY;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see
 */
typedef struct
{
	UINT32 uiOffset;		///< Target Delay
	UINT32 uiValue;			///< Delay Value in ms
}
LX_CI_IOCTL_PARAM_REGISTER;


/**
 * ci ioctl parameter.
 * useful when complex parameter should be passed to ci kernel driver.
 * @see
 */
typedef struct
{
	UINT8 ui8Speed;			///< Target Speed
}
LX_CI_IOCTL_PARAM_SPEED;

typedef struct
{
	UINT8 mask;			///< Print mask
}
LX_CI_IOCTL_PARAM_LOG_ENABLE;


typedef struct
{
	UINT32 ui32Mode;			///< Target Mode
}
LX_CI_IOCTL_PARAM_ACCESSMODE_CHANGE;


typedef struct
{
	UINT32 ui32Option;
	UINT32 ui32Value;
}
LX_CI_IOCTL_PARAM_OPTION;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_DRV_H_ */

/** @} */
