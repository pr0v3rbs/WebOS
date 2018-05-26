/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  main configuration file for slt device
 *  slt device will teach you how to make device driver with new platform.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
 *  @{
 */

#ifndef __SLT_H15_H__
#define __SLT_H15_H__

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "slt_kapi.h"
#include <linux/completion.h>

#ifdef  __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
    Constant Definitions
----------------------------------------------------------------------------------------*/
/*
| Error Code Definitions
*/
#define	RET_OK					0			///< success
#define RET_ERROR				-EIO		///< general error
#define RET_INVALID_PARAMS		-EINVAL		///< invalid paramter
#define RET_INVALID_IOCTL		-ENOTTY		///< invalid ioctl request
#define RET_OUT_OF_MEMORY		-ENOMEM		///< out ot memory
#define RET_TIMEOUT				-ETIME		///< timeout
#define RET_TRY_AGAIN			-EAGAIN		///< try again
#define RET_INTR_CALL			-EINTR		///< interrupted system call


/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/

/*
 * SLT_REG		: REAL Register 
 * SLT_REG_SHADOW	: SHADOW Register
 *
 * @def SLT_RdFL : Read  FLushing : Shadow <- Real
 * @def SLT_WrFL : Write FLushing : Shadow -> Real
 * @def SLT_Rd   : Read  whole register(UINT32) from Shadow register.
 * @def SLT_Wr   : Write whole register(UINT32) from Shadow register.
 */

#if 1
#define SLT_RdFL( _r )		( (SLT_REG_Shadow->_r) = (SLT_REG->_r) )
#define SLT_WrFL( _r )		( (SLT_REG->_r) = (SLT_REG_Shadow->_r) )

#define SLT_Rd( _r )		*((UINT32*)(&(SLT_REG_Shadow->_r)))
#define SLT_Wr( _r, _v)		((SLT_Rd(_r))=((UINT32)(_v)))
#else
#define SLT_RdFL( _r )		
#define SLT_WrFL( _r )

#define SLT_Rd( _r )		( *((volatile UINT32*)(&(SLT_REG->_r))) )                    
#define SLT_Wr( _r, _v)		( *((volatile UINT32*)(&(SLT_REG->_r))) ) = ((volatile UINT32)(_v))
#endif


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*
; SLT structure.
*/

/*-------------------------------
 * 0x0000 SLT_CRC00
 *-----------------------------*/
typedef struct {
    UINT32
    crc_enable			: 1;
} SLT_CRC00_T;


/*-------------------------------
 * 0x0004 SLT_CRC01
 *-----------------------------*/
typedef struct {
    UINT32
    crc_reset			: 1;
} SLT_CRC01_T;


/*-------------------------------
 * 0x0008 SLT_CRC02
 *-----------------------------*/
typedef struct {
    UINT32
    hs_level			: 1,
    vs_level			: 1;
} SLT_CRC02_T;


/*-------------------------------
 * 0x000C SLT_CRC03
 *-----------------------------*/
typedef struct {
    UINT32
    auto_reset_disable	: 1;
} SLT_CRC03_T;


/*-------------------------------
 * 0x0010 SLT_CRC04
 *-----------------------------*/
typedef struct {
    UINT32
    crc_intr_enable		: 1;
} SLT_CRC04_T;


/*-------------------------------
 * 0x0014 SLT_CRC05
 *-----------------------------*/
typedef struct {
    UINT32
    intr_status			: 2;
} SLT_CRC05_T;


/*-------------------------------
 * 0x0018 SLT_CRC06
 *-----------------------------*/
typedef struct {
    UINT32
    intr_clear_0		: 1,
    intr_clear_1		: 1;
} SLT_CRC06_T;


/*-------------------------------
 * 0x001C SLT_CRC07
 *-----------------------------*/
typedef struct {
    UINT32
    hs_mask				: 1,
    vs_mask				: 1;
} SLT_CRC07_T;


/*-------------------------------
 * 0x0020 SLT_CRC08
 *-----------------------------*/
typedef struct {
    UINT32			data0_mask;
} SLT_CRC08_T;


/*-------------------------------
 * 0x0024 SLT_CRC09
 *-----------------------------*/
typedef struct {
    UINT32			data1_mask;
} SLT_CRC09_T;


/*-------------------------------
 * 0x0040 SLT_CRC10
 *-----------------------------*/
typedef struct {
    UINT32			crc_data0;
} SLT_CRC10_T;


/*-------------------------------
 * 0x0044 SLT_CRC11
 *-----------------------------*/
typedef struct {
    UINT32			crc_data1;
} SLT_CRC11_T;



typedef struct {
	SLT_CRC00_T			crc_enable;				// 0x0000
	SLT_CRC01_T			crc_reset; 				// 0x0004
	SLT_CRC02_T			crc_level; 				// 0x0008
	SLT_CRC03_T			auto_reset_disable;		// 0x000C

	SLT_CRC04_T			crc_intr_enable;		// 0x0010
	SLT_CRC05_T			crc_intr_status;		// 0x0014
	SLT_CRC06_T			crc_intr_clear; 		// 0x0018
	SLT_CRC07_T			crc_mask;				// 0x001C

	SLT_CRC08_T			crc_data0_mask;			// 0x0020			
	SLT_CRC09_T			crc_data1_mask;			// 0x0024			
    UINT32				empty1;					// 0x0028
    UINT32				empty2;					// 0x002C

    UINT32				empty3;					// 0x0030
    UINT32				empty4;					// 0x0034
    UINT32				empty5;					// 0x0038
    UINT32				empty6;					// 0x003C

	SLT_CRC10_T			crc_data0;				// 0x0040
	SLT_CRC11_T			crc_data1;				// 0x0044
    UINT32				empty7;		 			// 0x0048
    UINT32				empty8;		 			// 0x004C
} H15_SLT_REG_T;



/*----------------------------------------------------------------------------------------
    Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Functions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

static volatile H15_SLT_REG_T		*SLT_REG = NULL;
static volatile H15_SLT_REG_T		SLT_REG_Shadow[1];

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SLT_H15_H__ */

