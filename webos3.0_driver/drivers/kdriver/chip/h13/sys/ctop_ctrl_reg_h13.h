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

#ifndef  __CTOP_CTRL_REG_H13_H__
#define  __CTOP_CTRL_REG_H13_H__

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sys_reg_base.h"

#include "ctop_ctrl_reg_h13a0.h"
#include "ctop_ctrl_reg_h13b0.h"


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define CTOP_H13A0_SHDW		(gCTOP_CTRL_H13.shdw.a0)
#define CTOP_H13B0_SHDW		(gCTOP_CTRL_H13.shdw.b0)

#define CTOP_H13A0_PHYS		(gCTOP_CTRL_H13.phys.a0)
#define CTOP_H13B0_PHYS		(gCTOP_CTRL_H13.phys.b0)



/* H13 COMMON */
#define CTOP_REG_H13xx_RdFL(_chip,_r)		_REG_RdFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)
#define CTOP_REG_H13xx_WrFL(_chip,_r)		_REG_WrFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)

#define CTOP_REG_H13xx_Rd(_chip,_r)			_REG_Rd(CTOP_##_chip##_SHDW,_r)
#define CTOP_REG_H13xx_Wr(_chip,_r,_v)		_REG_Wr(CTOP_##_chip##_SHDW,_r,_v)


#define CTOP_REG_H13xx_RdFd(_chip,_r,_f01) \
	_REG_RdFd(CTOP_##_chip##_SHDW,_r,_f01)

#define CTOP_REG_H13xx_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_H13xx_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_H13xx_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CTOP_REG_H13xx_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_H13xx_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_H13xx_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* H13A0 */
#define CTOP_CTRL_H13A0_RdFL(_r)		CTOP_REG_H13xx_RdFL(H13A0,_r)
#define CTOP_CTRL_H13A0_WrFL(_r)		CTOP_REG_H13xx_WrFL(H13A0,_r)

#define CTOP_CTRL_H13A0_Rd(_r)			CTOP_REG_H13xx_Rd(H13A0,_r)
#define CTOP_CTRL_H13A0_Wr(_r,_v)		CTOP_REG_H13xx_Wr(H13A0,_r,_v)


#define CTOP_CTRL_H13A0_RdFd(_r,_f01)					CTOP_REG_H13xx_RdFd(H13A0,_r,_f01)

#define CTOP_CTRL_H13A0_Rd01(_r,_f01,_v01)				CTOP_REG_H13xx_Rd01(H13A0,_r,_f01,_v01)
#define CTOP_CTRL_H13A0_Rd02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H13xx_Rd02(H13A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H13A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H13xx_Rd03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_H13A0_Wr01(_r,_f01,_v01)				CTOP_REG_H13xx_Wr01(H13A0,_r,_f01,_v01)
#define CTOP_CTRL_H13A0_Wr02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H13xx_Wr02(H13A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H13A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H13xx_Wr03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* H13B0 */
#define CTOP_CTRL_H13B0_RdFL(_r)		CTOP_REG_H13xx_RdFL(H13B0,_r)
#define CTOP_CTRL_H13B0_WrFL(_r)		CTOP_REG_H13xx_WrFL(H13B0,_r)

#define CTOP_CTRL_H13B0_Rd(_r)			CTOP_REG_H13xx_Rd(H13B0,_r)
#define CTOP_CTRL_H13B0_Wr(_r,_v)		CTOP_REG_H13xx_Wr(H13B0,_r,_v)

#define CTOP_CTRL_H13B0_RdFd(_r,_f01)					CTOP_REG_H13xx_RdFd(H13B0,_r,_f01)

#define CTOP_CTRL_H13B0_Rd01(_r,_f01,_v01)				CTOP_REG_H13xx_Rd01(H13B0,_r,_f01,_v01)
#define CTOP_CTRL_H13B0_Rd02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H13xx_Rd02(H13B0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H13B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H13xx_Rd03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_H13B0_Wr01(_r,_f01,_v01)				CTOP_REG_H13xx_Wr01(H13B0,_r,_f01,_v01)
#define CTOP_CTRL_H13B0_Wr02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_H13xx_Wr02(H13B0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H13B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_H13xx_Wr03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CTOP_CTRL_H13_RdFL(_r)							CTOP_CTRL_H13A0_RdFL(_r)
#define CTOP_CTRL_H13_WrFL(_r)							CTOP_CTRL_H13A0_WrFL(_r)
#define CTOP_CTRL_H13_Rd(_r)							CTOP_CTRL_H13A0_Rd(_r)
#define CTOP_CTRL_H13_Wr(_r, _v)						CTOP_CTRL_H13A0_Wr(_r, _v)
#define CTOP_CTRL_H13_Rd01(_r, _f01, _v01)				CTOP_CTRL_H13A0_Rd01(_r, _f01, _v01)
#define CTOP_CTRL_H13_Wr01(_r, _f01, _v01)				CTOP_CTRL_H13A0_Wr01(_r, _f01, _v01)
#define CTOP_CTRL_H13_WfCM(_r, _f, _c, _m)				CTOP_CTRL_H13A0_WfCM(_r, _f, _c, _m)
#define CTOP_CTRL_H13_WfCV(_r, _f, _c, _v1, _v2)		CTOP_CTRL_H13A0_WfCV(_r, _f, _c, _v1, _v2)
#define CTOP_CTRL_H13_FLRd(_r, _v)						CTOP_CTRL_H13A0_FLRd(_r, _v)
#define CTOP_CTRL_H13_FLWr(_r, _v)						CTOP_CTRL_H13A0_FLWr(DE_CTOP, H13, _r, _v)
#define CTOP_CTRL_H13_FLRf(_r, _f01, _v01)				CTOP_CTRL_H13A0_FLRf(DE_CTOP, H13, _r, _f01, _v01)
#define CTOP_CTRL_H13_FLWf(_r, _f01, _v01)				CTOP_CTRL_H13A0_FLWf(DE_CTOP, H13, _r, _f01, _v01)
#define CTOP_CTRL_H13_FLCM(_r, _f, _c, _m)				CTOP_CTRL_H13A0_FLCM(DE_CTOP, H13, _r, _f, _c, _m)
#define CTOP_CTRL_H13_FLCV(_r, _f, _c, _v1, _v2)		CTOP_CTRL_H13A0_FLCV(DE_CTOP, H13, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32              *addr;
		CTOP_CTRL_REG_H13A0_T   *a0;
		CTOP_CTRL_REG_H13B0_T   *b0;
	} shdw;

	union {
		volatile UINT32                 *addr;
		volatile CTOP_CTRL_REG_H13A0_T  *a0;
		volatile CTOP_CTRL_REG_H13B0_T  *b0;
	} phys;
} CTOP_CTRL_REG_H13_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern CTOP_CTRL_REG_H13_T gCTOP_CTRL_H13;


static inline UINT32 CTOP_CTRL_H13_READ(UINT32 offset)
{
	return _REG_OfsRd(gCTOP_CTRL_H13.shdw.addr, gCTOP_CTRL_H13.phys.addr, offset);
}

static inline void CTOP_CTRL_H13_WRITE(UINT32 offset, UINT32 value)
{
	_REG_OfsWr(gCTOP_CTRL_H13.shdw.addr, gCTOP_CTRL_H13.phys.addr, offset, value);
}
#endif
/**  @} */
