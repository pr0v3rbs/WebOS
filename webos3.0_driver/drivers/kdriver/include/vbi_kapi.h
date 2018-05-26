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



/** @file vbi_kapi.h
 *
 *  application interface header for vbi device
 *
 *  @author		won.hur (won.hur@lge.com)
 *  @version		1.0
 *  @date		2012.04.23
 *
 *  @addtogroup lg115x_vbi
 *	@{
 */

#ifndef	_VBI_KAPI_H_
#define	_VBI_KAPI_H_

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
#define	VBI_IOC_MAGIC		't'
/**
@name VBI IOCTL List
ioctl list for vbi device.

@{

@def VBI_TTX_IO_INIT
Creates VBI buffer on kernel memory, and initialize VBI parameters.

@def VBI_TTX_IOW_ONOFF_INTERRUPT
Sets ON/OFF the TTX interrupt
TRUE : ON the TTX interrupt, FALSE : Off the TTX Interrrupt

@def VBI_TTX_IOW_ONOFF_SLICER
Sets ON/OFF the TTX SLICER
TRUE : ON the TTX interrupt, FALSE : Off the TTX Interrrupt

@def VBI_TTX_IO_RESET_TTX_BUFFER
Resets TTX buffer

@def VBI_TTX_IO_RESET_VPS_BUFFER
Resets VPS buffer

@def VBI_TTX_IO_RESET_WSS_BUFFER
Resets WSS buffer

@def VBI_TTX_IOR_GET_PACKET_COUNT
Read Received TTX Packet Number in kernel buffer.

@def VBI_TTX_IOR_GET_STATUS
Read received TTX status(received data type) from kernel driver.

@def VBI_TTX_IOR_GET_VPS_PACKET
Read VPS packets(15bytes) from kernel driver

@def VBI_TTX_IOR_GET_WSS_PACKET
Read WSS packets(4bytes) from kernel driver.

@def VBI_TTX_IOR_GET_TTX_PACKET
Read TTX packets and size from kernel driver.

@def VBI_TTX_IOR_GET_BUFFER_INFO
Read TTX buffer information(TTX size, vps offset, wss offset, putaddr offset, getaddr offset, etc)

@def VBI_TTX_IOR_GET_PARAMETER_OFFSET
Read Offset address (from MMAP address)of TTX Parameter structure.

@def VBI_TTX_IOR_GET_MMAP_SIZE
Read MMAP size of TTX driver.

@def VBI_TTX_IO_RESET_TTX
Reset TTX parameters

@def VBI_TTX_IO_RELEASE
Release TTX device(Free VBI buffer)

@def VBI_TTX_IOW_ENABLE_VPS_SLICER
Enable/Disable VPS Slicing on VBI Line 16. If disabled(default condition) TTX slicing is executed on Line 16.

@def VBI_CC_IO_INIT
Creates CC VBI buffer on kernel memory, and initializes VBI parameters.

@def VBI_CC_IOW_ONOFF_INTERRUPT
Sets ON/OFF the CC interrupt
TRUE : On the CC interrup, FALSE : OFF the CC interrupt

@def VBI_CC_IOW_ONOFF_SLICER
Sets ON/OFF the CC Slicer
TRUE : On the CC slicer, FALSE : OFF the CC slicer

@def VBI_CC_IOR_GET_PACKET_COUNT
Read Received CC Packet Number in kernel buffer.

@def VBI_CC_IOR_GET_PARAMETER_OFFSET
Read Offset address (from MMAP address)of CC Parameter structure.

@def VBI_CC_IOR_GET_MMAP_SIZE
Read MMAP size of CC driver.

@def VBI_CC_IO_RESET_CC
Reset CC parameters

@def VBI_CC_IO_RELEASE
Release CC device(Free VBI buffer & stop interrupt)

@def VBI_CC_IO_RESET_CC_BUFFER
Resets Kernel CC buffer

@def VBI_CC_IOWR_GET_CC_PACKET
Read CC data1/2 and field indication from kernel driver

#def VBI_CC_IOW_SET_DEBUGMODE_CC_PRINT
Print incoming CC data to shell, which is only used for debugging

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define	VBI_TTX_IO_INIT						_IO(VBI_IOC_MAGIC,  1)
#define VBI_TTX_IOW_ONOFF_INTERRUPT			_IOW(VBI_IOC_MAGIC, 2, UINT8)
#define VBI_TTX_IOW_ONOFF_SLICER			_IOW(VBI_IOC_MAGIC, 3, UINT8)
#define VBI_TTX_IO_RESET_TTX_BUFFER			_IO(VBI_IOC_MAGIC,  4)
#define VBI_TTX_IO_RESET_VPS_BUFFER			_IO(VBI_IOC_MAGIC,  5)
#define VBI_TTX_IO_RESET_WSS_BUFFER			_IO(VBI_IOC_MAGIC,  6)
#define VBI_TTX_IOR_GET_PARAMETER_OFFSET	_IOR(VBI_IOC_MAGIC, 7, UINT32)
#define VBI_TTX_IOR_GET_MMAP_SIZE			_IOR(VBI_IOC_MAGIC, 8, UINT32)
#define VBI_TTX_IO_RESET_TTX				_IO(VBI_IOC_MAGIC,  9)
#define VBI_TTX_IO_RELEASE					_IO(VBI_IOC_MAGIC,  10)
#define VBI_TTX_IOW_ENABLE_VPS_SLICER		_IOW(VBI_IOC_MAGIC, 11, BOOLEAN)

#define	VBI_CC_IO_RESET						_IO(VBI_IOC_MAGIC,  12 )
#define	VBI_CC_IO_INIT						_IO(VBI_IOC_MAGIC,  13)
#define VBI_CC_IOW_ONOFF_INTERRUPT			_IOW(VBI_IOC_MAGIC, 14, BOOLEAN)
#define VBI_CC_IOW_ONOFF_SLICER				_IOW(VBI_IOC_MAGIC, 15, BOOLEAN)
#define	VBI_CC_IOR_GET_PACKET_COUNT			_IOR(VBI_IOC_MAGIC, 16, UINT32)
#define VBI_CC_IO_RESET_CC_BUFFER			_IO(VBI_IOC_MAGIC,  17)
#define VBI_CC_IOR_GET_CC_PACKET			_IOWR(VBI_IOC_MAGIC, 18, LX_CC_DATA_T)

#define	VBI_TTX_IOW_VBI_SET_OPTION_LPF		_IOW(VBI_IOC_MAGIC, 19, BOOLEAN)
#define VBI_TTX_IOW_CHECK_LINE318			_IOW(VBI_IOC_MAGIC, 20, BOOLEAN)
#define VBI_TTX_IOW_SET_TYPE_LINE318		_IOW(VBI_IOC_MAGIC, 21, BOOLEAN)
#define VBI_TTX_IOW_SET_TT_SLICER_MODE		_IOW(VBI_IOC_MAGIC, 22, UINT32)

#define VBI_CC_IOW_SET_DEBUGMODE_PRINT		_IOW(VBI_IOC_MAGIC, 23, UINT32)
#define VBI_TTX_IOW_SET_DEBUGMODE_PRINT		_IOW(VBI_IOC_MAGIC, 24, UINT32)

#define VBI_IO_SET_INIT_CGMS_DATA			_IO(VBI_IOC_MAGIC, 25)
#define VBI_IO_CHECK_VLINE625				_IO(VBI_IOC_MAGIC, 26)
#define VBI_IO_GET_ANALOG_CP				_IO(VBI_IOC_MAGIC, 27) 
#define VBI_IOW_SET_LOGM_PRINT				_IOW(VBI_IOC_MAGIC, 28, LX_VBI_LOGM_SETTING_T)
#define VBI_IOC_MAXNR				29
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define LX_VPS_BUFFER_SIZE	15
#define LX_WSS_BUFFER_SIZE	4
#define LX_TTX_PACKET_UNIT	48
#define LX_TTX_NUM_LINES	35 * 1 /* 6 ~ 22 & 318 ~ 335 */
#define LX_TTX_DATA_BUFFER_SIZE	(LX_TTX_PACKET_UNIT*LX_TTX_NUM_LINES*5) ///< 5 Frames
#define LX_VPS_OFFSET
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
  * Received data type
  * Set if the TTX_TYPE is sliced, clear if ther TTX_TYPE isn't sliced
  * @see VBI_TTX_IOR_GET_STATUS, LX_TTX_PARAMETER_T
  */
typedef enum {
	LX_TTX_TYPE_TTX=1,	///< ttx type ttx
	LX_TTX_TYPE_VPS=2,	///< ttx type vps
	LX_TTX_TYPE_WSS=4,	///< ttx type wss
	LX_TTX_TYPE_CGMS=8,
	LX_TTX_TYPE_ALL=15, ///< ttx type all
} LX_TTX_TYPE_T;

/**
  * Return COPY PROTECTION(CGMS) type
  */
typedef enum {
	LX_VBI_CP_TYPE_NONE		=0,		///< None detected(CGMS type 0 or WSS no CP)
	LX_VBI_CP_TYPE_CGMS_0	=1,		///< CGMS TYPE 0 (0x00)
	LX_VBI_CP_TYPE_CGMS_1	=2, 	///< CGMS TYPE 1 (0x01)
	LX_VBI_CP_TYPE_CGMS_2	=4, 	///< CGMS TYPE 2 (0x10)
	LX_VBI_CP_TYPE_CGMS_3	=8,		///< CGMS TYPE 3 (0x11)
	LX_VBI_CP_TYPE_WSS_0    =16,		///< WSS Copy protection
	LX_VBI_CP_TYPE_WSS_1    =32,		///< WSS Copy protection
	LX_VBI_CP_TYPE_ANALOG_1 =64,	///< Analog protection Type 1
	LX_VBI_CP_TYPE_ANALOG_2 =128,	///< Analog protection Type 2
	LX_VBI_CP_TYPE_ANALOG_3 =256,	///< Analog protection Type 3
	LX_VBI_CP_TYPE_MAXNUM		///< MAX NUM of enumeration
} LX_VBI_CP_TYPE_T;


/**
  * Structure to read TTX data & size
  * @see VBI_TTX_IOR_GET_TTX_PACKET
  */
typedef struct {
	UINT8	*pTTX_Packet_Buffer;	///< TTX packet buffer
	UINT32	TTX_Packet_Unit;		///< TTX packet unit(number of received ttx lines, one line is 48bytes long)
}	LX_TTX_PACKET_TYPE_T;

/**
  * Structure of TTX Parameters
  * @see VBI_TTX_IOR_GET_PARAMETER_OFFSET
  */
typedef struct {
	  UINT32	GetAddr;				///< offset address of get address (unit of 48bytes)
	  UINT32	PutAddr;				///< offset address of put address (unit of 48bytes)
	  UINT32	Count;
	  UINT32	CGMS_Offset;
	  UINT32	VPS_Offset;				///< offset address of VPS data buffer
	  UINT32	WSS_Offset;				///< offset address of WSS data buffer
	  UINT32	Status;					///< TTX buffer information( @see TTX_IOR_GET_STATUS)
	  UINT32	TTX_Data_Count;
}	LX_TTX_PARAMETER_T;

typedef struct {
	UINT8	data1;		///< CC data1
	UINT8	data2;		///< CC data2
	UINT8	field;		///< odd/even field indication(0:Even field, 1:Odd filed)
	UINT8	status; 	///< status(reserved)
	UINT16	line_number; ///< sliced vbi line number
} LX_CC_DATA_T;

typedef enum
{
	VBI_SKEEPER_TTX_INITIALIZE = 0,
	VBI_SKEEPER_TTX_ONOFF_INTERRUPT,
	VBI_SKEEPER_TTX_GET_MMAP_SIZE,
	VBI_SKEEPER_TTX_ENABLE_VPS_SLICER,
	VBI_SKEEPER_CC_INITIALIZE,
	VBI_SKEEPER_CC_ONOFF_INTERRUPT,
	VBI_SKEEPER_VBI_DATA_ERROR_STATUS,
	VBI_SKEEPER_VBI_MODULE_ERROR_STATUS,
	VBI_SKEEPER_STATUS_MAXNUM
} VBI_SKEEPER_STATUS_TYPE_T;

typedef struct 
{
	UINT32 logLevel;
	BOOLEAN onOff;	 
} LX_VBI_LOGM_SETTING_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VBI_DRV_H_ */

/** @} */
