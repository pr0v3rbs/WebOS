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


/** @file denc_kapi.h
 *
 *  application interface header for denc device
 *
 *  @author		wonsik.do (wonsik.do@lge.com)
 *  @version		1.0
 *  @date		2010.01.06
 *
 *  @addtogroup lg1150_denc
 *	@{
 */

#ifndef	_DENC_KAPI_H_
#define	_DENC_KAPI_H_

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
#define	DENC_IOC_MAGIC		'r'
/**
@name DENC IOCTL List
ioctl list for denc device.

@{
@def DENC_IO_INIT
Writes initial register values to DENC(CVE)

@def DENC_IOW_ONOFF
NTSC/PAL/SECAM encoder enable(TRUE : enable, FALSE : disable)

@def DENC_IOW_SET_OUTPUT_VIDEO_STD
Select the color standard of CVE.

@def DENC_IOW_SET_SOURCE
Select the input source of CVE.

@def DENC_IOW_TTX_ENABLE
Teletext insertion enable (TRUE : enable, FALSE : disable)

@def DENC_IOW_WSS_ENABLE
WSS insertion enable (TRUE : enable, FALSE : disable)

@def DENC_IOW_VPS_ENABLE
VPS insertion enable (TRUE : enable, FALSE : disable)

@def DENC_IOW_SET_TTX_DATA
Inserts Teletext data to VBI lines(One VBI Line).
Line_Number : indicates the line the TTX data to be inserted. (Refer to TTX_LINE in TTX_VER_CTRL1 register)
Size : Size of TTX data to be inserted(Normally 48 byte)
Packer_Addr : Start address of TTX data.

@def DENC_IOW_SET_WSS_DATA
Inserts WSS data to VBI line.

@def DENC_IOW_SET_VPS_DATA
Inserts VPS data to VBI lines.

@def DENC_IOW_COLORBAR_ENABLE
Enable/disable internal colorbar pattern (TRUE : enable, FALSE : disable)

@def	DENC_IOW_VDAC_POWER_CONTROL
Turn On/Off VDAC and Buffer Power of DENC to save power consumption.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
//#define	DENC_IO_RESET				_IO(DENC_IOC_MAGIC,  0 )
//#define DENC_IOR_CHIP_REV_INFO		_IOR(DENC_IOC_MAGIC, 1, CHIP_REV_INFO_T )
//#define DENC_IOW_WRITE_UINT32		_IOW(DENC_IOC_MAGIC, 2, UINT32 )
//#define DENC_IOW_WRITE_STRUCT		_IOW(DENC_IOC_MAGIC, 3, DENC_IOCTL_PARAM_T)
#define DENC_IO_INIT		_IO(DENC_IOC_MAGIC, 4)
#define	DENC_IOW_ONOFF			_IOW(DENC_IOC_MAGIC, 5, BOOLEAN)
#define DENC_IOW_SET_OUTPUT_VIDEO_STD	_IOW(DENC_IOC_MAGIC, 6, LX_DENC_VIDEO_SYSTEM_T)
#define	DENC_IOW_SET_SOURCE		_IOW(DENC_IOC_MAGIC, 7, LX_DENC_VIDEO_SOURCE_T)
#define	DENC_IOW_TTX_ENABLE		_IOW(DENC_IOC_MAGIC, 8, BOOLEAN)
#define DENC_IOW_WSS_ENABLE		_IOW(DENC_IOC_MAGIC, 9, BOOLEAN)
#define DENC_IOW_VPS_ENABLE		_IOW(DENC_IOC_MAGIC, 10, BOOLEAN)
//#define DENC_IOW_CC_ENABLE		_IOW(DENC_IOC_MAGIC, 11, BOOLEAN)
#define	DENC_IOW_SET_TTX_DATA		_IOW(DENC_IOC_MAGIC, 12, LX_DENC_TTX_DATA_T)
#define DENC_IOW_SET_WSS_DATA		_IOW(DENC_IOC_MAGIC, 13, LX_DENC_WSS_DATA_T)
#define DENC_IOW_SET_VPS_DATA		_IOW(DENC_IOC_MAGIC, 14, LX_DENC_VPS_DATA_T)
//#define DENC_IOW_SET_CC_DATA		_IOW(DENC_IOC_MAGIC, 15, LX_DENC_CC_DATA_T)

#define	DENC_IOW_COLORBAR_ENABLE		_IOW(DENC_IOC_MAGIC, 20, BOOLEAN)
#define	DENC_IOW_VDAC_POWER_CONTROL			_IOW(DENC_IOC_MAGIC, 21,	UINT32)
#define	DENC_IOW_MUTE_CONTROL			_IOW(DENC_IOC_MAGIC, 22,	BOOLEAN)
#define	DENC_IOW_STRIP_CAPTURE_CONTROL			_IOW(DENC_IOC_MAGIC, 23,	BOOLEAN)

#define DENC_IOC_MAXNR				24
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * Enum used to select DENC color standard.
 * @see DENC_IOW_SET_OUTPUT_VIDEO_STD
 */

typedef enum {
	LX_DENC_NTSC_M,			///< NTSC M system
	LX_DENC_PAL_BG,			///< PAL BG system
	LX_DENC_PAL_M,			///< PAL M system
	LX_DENC_PAL_N,			///< PAL N system
	LX_DENC_SECAM,			///< SECAM system
	LX_DENC_NTSC_J,			///< NTSC Japan
	LX_DENC_VIDEO_SYS_NUM,	///< max number
}
LX_DENC_VIDEO_SYSTEM_T;

/**
 * Enum used to select source of CVBS output.
 * @see DENC_IOW_SET_SOURCE
 */
typedef enum {
	LX_DENC_SOURCE_CVBS1_WITH_CLAMPING,		///< L8 cvbs port 1 with clamping(when cvbs1 is NOT selected by CVD)
	LX_DENC_SOURCE_CVBS1_WITHOUT_CLAMPING,	///< L8 cvbs port 1(when cvbs1 is selected by CVD)
	LX_DENC_SOURCE_CVBS3_WITH_CLAMPING,		///< L8 cvbs port 3(when cvbs3 is NOT selected by CVD)
	LX_DENC_SOURCE_CVBS3_WITHOUT_CLAMPING,	///< L8 cvbs port 3(when cvbs3 is selected by CVD)
	LX_DENC_SOURCE_MONITOR,					///< L8 cvbs monitor out
	LX_DENC_SOURCE_NUM,						///< max number
}
LX_DENC_VIDEO_SOURCE_T;

/**
  * Structure used to send TTX data and parameters to kernel driver
  * @see DENC_IOW_SET_TTX_DATA
  */
typedef struct {
	UINT32 Line_Number;		///< tele-text line flag(VBI Line number to Insert ttx packet)
	UINT32 Size;			///< tele-text packet size(Normally 48)
	UINT8 *pPacket_Addr;		///< tele-text packet address
}
LX_DENC_TTX_DATA_T;

/**
  * Structure used to send WSS data and parameters to kernel driver
  * @see DENC_IOW_SET_WSS_DATA
  */
typedef struct {
	BOOLEAN Enable;			///< WSS ON/OFF
	UINT16 WSS_DATA;		///< WSS data (2 byte WSS data)
}
LX_DENC_WSS_DATA_T;
/**
  * Structure used to send VPS data and parameters to kernel driver
  * @see DENC_IOW_SET_VPS_DATA
  */
typedef struct {
	BOOLEAN Enable;			///< VPS ON/OFF
	UINT8 *pVPS_DATA;			///< VPS data pointer(Normally 15 byte long)
}
LX_DENC_VPS_DATA_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DENC_DRV_H_ */

/** @} */
