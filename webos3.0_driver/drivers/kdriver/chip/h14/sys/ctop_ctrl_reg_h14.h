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
 * Register access for CTOP
 *
 * author     ks.hyun (ks.hyun@lge.com)
 * version    1.0
 * date       2012.04.27
 * note       Additional information.
 *
 * @addtogroup lg115x_sys
 * @{
 */

#ifndef  __CTOP_CTRL_REG_H14_H__
#define  __CTOP_CTRL_REG_H14_H__

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sys_reg_base.h"

#include "ctop_ctrl_reg_h14a0.h"
//#include "ctop_ctrl_reg_h14b0.h"


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define CTOP_H14A0_SHDW		(gCTOP_CTRL_H14.shdw.a0)
#define CTOP_H14B0_SHDW		(gCTOP_CTRL_H14.shdw.b0)

#define CTOP_H14A0_PHYS		(gCTOP_CTRL_H14.phys.a0)
#define CTOP_H14B0_PHYS		(gCTOP_CTRL_H14.phys.b0)



/* H14 COMMON */
#define CTOP_REG_H14xx_RdFL(_chip,_r)		_REG_RdFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)
#define CTOP_REG_H14xx_WrFL(_chip,_r)		_REG_WrFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)

#define CTOP_REG_H14xx_Rd(_chip,_r)			_REG_Rd(CTOP_##_chip##_SHDW,_r)
#define CTOP_REG_H14xx_Wr(_chip,_r,_v)		_REG_Wr(CTOP_##_chip##_SHDW,_r,_v)


#define CTOP_REG_H14xx_RdFd(_chip,_r,_f01) \
	_REG_RdFd(CTOP_##_chip##_SHDW,_r,_f01)

#define CTOP_REG_H14xx_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_H14xx_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_H14xx_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CTOP_REG_H14xx_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_H14xx_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_H14xx_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* H14A0 */
#define CTOP_CTRL_H14A0_RdFL(_r)		CTOP_REG_H14xx_RdFL(H14A0,_r)
#define CTOP_CTRL_H14A0_WrFL(_r)		CTOP_REG_H14xx_WrFL(H14A0,_r)

#define CTOP_CTRL_H14A0_Rd(_r)			CTOP_REG_H14xx_Rd(H14A0,_r)
#define CTOP_CTRL_H14A0_Wr(_r,_v)		CTOP_REG_H14xx_Wr(H14A0,_r,_v)


#define CTOP_CTRL_H14A0_RdFd(_r,_f01)					CTOP_REG_H14xx_RdFd(H14A0,_r,_f01)

#define CTOP_CTRL_H14A0_Rd01(_r,_f01,_v01)				CTOP_REG_H14xx_Rd01(H14A0,_r,_f01,_v01)
#define CTOP_CTRL_H14A0_Rd02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H14xx_Rd02(H14A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H14xx_Rd03(H14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_H14A0_Wr01(_r,_f01,_v01)				CTOP_REG_H14xx_Wr01(H14A0,_r,_f01,_v01)
#define CTOP_CTRL_H14A0_Wr02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H14xx_Wr02(H14A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H14xx_Wr03(H14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* H14B0 */
#define CTOP_CTRL_H14B0_RdFL(_r)		CTOP_REG_H14xx_RdFL(H14B0,_r)
#define CTOP_CTRL_H14B0_WrFL(_r)		CTOP_REG_H14xx_WrFL(H14B0,_r)

#define CTOP_CTRL_H14B0_Rd(_r)			CTOP_REG_H14xx_Rd(H14B0,_r)
#define CTOP_CTRL_H14B0_Wr(_r,_v)		CTOP_REG_H14xx_Wr(H14B0,_r,_v)

#define CTOP_CTRL_H14B0_RdFd(_r,_f01)					CTOP_REG_H14xx_RdFd(H14B0,_r,_f01)

#define CTOP_CTRL_H14B0_Rd01(_r,_f01,_v01)				CTOP_REG_H14xx_Rd01(H14B0,_r,_f01,_v01)
#define CTOP_CTRL_H14B0_Rd02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H14xx_Rd02(H14B0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H14B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H14xx_Rd03(H14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_H14B0_Wr01(_r,_f01,_v01)				CTOP_REG_H14xx_Wr01(H14B0,_r,_f01,_v01)
#define CTOP_CTRL_H14B0_Wr02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H14xx_Wr02(H14B0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H14B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H14xx_Wr03(H14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CTOP_CTRL_H14_RdFL(_r)							CTOP_CTRL_H14A0_RdFL(_r)
#define CTOP_CTRL_H14_WrFL(_r)							CTOP_CTRL_H14A0_WrFL(_r)
#define CTOP_CTRL_H14_Rd(_r)							CTOP_CTRL_H14A0_Rd(_r)
#define CTOP_CTRL_H14_Wr(_r, _v)						CTOP_CTRL_H14A0_Wr(_r, _v)
#define CTOP_CTRL_H14_Rd01(_r, _f01, _v01)				CTOP_CTRL_H14A0_Rd01(_r, _f01, _v01)
#define CTOP_CTRL_H14_Wr01(_r, _f01, _v01)				CTOP_CTRL_H14A0_Wr01(_r, _f01, _v01)


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32              *addr;
		CTOP_CTRL_REG_H14A0_T   *a0;
//		CTOP_CTRL_REG_H14B0_T   *b0;
	} shdw;

	union {
		volatile UINT32                 *addr;
		volatile CTOP_CTRL_REG_H14A0_T  *a0;
//		volatile CTOP_CTRL_REG_H14B0_T  *b0;
	} phys;
} CTOP_CTRL_REG_H14_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern CTOP_CTRL_REG_H14_T gCTOP_CTRL_H14;


static inline UINT32 CTOP_CTRL_H14_READ(UINT32 offset)
{
	return _REG_OfsRd(gCTOP_CTRL_H14.shdw.addr, gCTOP_CTRL_H14.phys.addr, offset);
}

static inline void CTOP_CTRL_H14_WRITE(UINT32 offset, UINT32 value)
{
	_REG_OfsWr(gCTOP_CTRL_H14.shdw.addr, gCTOP_CTRL_H14.phys.addr, offset, value);
}
#endif
/**  @} */
