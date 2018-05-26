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

#ifndef  __CTOP_CTRL_REG_M14_H__
#define  __CTOP_CTRL_REG_M14_H__

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sys_reg_base.h"

#include "ctop_ctrl_reg_m14a0.h"
#include "ctop_ctrl_reg_m14b0.h"


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define CTOP_M14A0_SHDW		(gCTOP_CTRL_M14.shdw.a0)
#define CTOP_M14A0_PHYS		(gCTOP_CTRL_M14.phys.a0)


/* M14 COMMON */
#define CTOP_REG_M14xx_RdFL(_chip,_r)		_REG_RdFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)
#define CTOP_REG_M14xx_WrFL(_chip,_r)		_REG_WrFL(CTOP_##_chip##_SHDW,CTOP_##_chip##_PHYS,_r)

#define CTOP_REG_M14xx_Rd(_chip,_r)			_REG_Rd(CTOP_##_chip##_SHDW,_r)
#define CTOP_REG_M14xx_Wr(_chip,_r,_v)		_REG_Wr(CTOP_##_chip##_SHDW,_r,_v)


#define CTOP_REG_M14xx_RdFd(_chip,_r,_f01) \
	_REG_RdFd(CTOP_##_chip##_SHDW,_r,_f01)

#define CTOP_REG_M14xx_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_M14xx_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_M14xx_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CTOP_REG_M14xx_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(CTOP_##_chip##_SHDW,_r,_f01,_v01)

#define CTOP_REG_M14xx_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CTOP_REG_M14xx_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(CTOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* M14A0 */
#define CTOP_CTRL_M14A0_RdFL(_r)		CTOP_REG_M14xx_RdFL(M14A0,_r)
#define CTOP_CTRL_M14A0_WrFL(_r)		CTOP_REG_M14xx_WrFL(M14A0,_r)

#define CTOP_CTRL_M14A0_Rd(_r)			CTOP_REG_M14xx_Rd(M14A0,_r)
#define CTOP_CTRL_M14A0_Wr(_r,_v)		CTOP_REG_M14xx_Wr(M14A0,_r,_v)


#define CTOP_CTRL_M14A0_RdFd(_r,_f01)					CTOP_REG_M14xx_RdFd(M14A0,_r,_f01)

#define CTOP_CTRL_M14A0_Rd01(_r,_f01,_v01)				CTOP_REG_M14xx_Rd01(M14A0,_r,_f01,_v01)
#define CTOP_CTRL_M14A0_Rd02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_M14xx_Rd02(M14A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_M14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_M14xx_Rd03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_M14A0_Wr01(_r,_f01,_v01)				CTOP_REG_M14xx_Wr01(M14A0,_r,_f01,_v01)
#define CTOP_CTRL_M14A0_Wr02(_r,_f01,_v01,_f02,_v02)	CTOP_REG_M14xx_Wr02(M14A0,_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_M14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CTOP_REG_M14xx_Wr03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* M14B0 */

#define CTOP_M14B0_SHDW(_m)					(gCTOP_CTRL_M14B0.shdw._m)
#define CTOP_M14B0_PHYS(_m)					(gCTOP_CTRL_M14B0.phys._m)

#define CTOP_CTRL_M14B0_RdFL(_m,_r)			_REG_RdFL(CTOP_M14B0_SHDW(_m),CTOP_M14B0_PHYS(_m),_r)
#define CTOP_CTRL_M14B0_WrFL(_m,_r)			_REG_WrFL(CTOP_M14B0_SHDW(_m),CTOP_M14B0_PHYS(_m),_r)

#define CTOP_CTRL_M14B0_Rd(_m,_r)				_REG_Rd(CTOP_M14B0_SHDW(_m),_r)
#define CTOP_CTRL_M14B0_Wr(_m,_r,_v)			_REG_Wr(CTOP_M14B0_SHDW(_m),_r,_v)


#define CTOP_CTRL_M14B0_RdFd(_m,_r,_f01)					_REG_RdFd(CTOP_M14B0_SHDW(_m),_r,_f01)
#define CTOP_CTRL_M14B0_Rd01(_m,_r,_f01,_v01)				_REG_Rd01(CTOP_M14B0_SHDW(_m),_r,_f01,_v01)
#define CTOP_CTRL_M14B0_Rd02(_m,_r,_f01,_v01,_f02,_v02)		_REG_Rd02(CTOP_M14B0_SHDW(_m),_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_M14B0_Rd03(_m,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CTOP_M14B0_SHDW(_m),_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_M14B0_Wr01(_m,_r,_f01,_v01)				_REG_Wr01(CTOP_M14B0_SHDW(_m),_r,_f01,_v01)
#define CTOP_CTRL_M14B0_Wr02(_m,_r,_f01,_v01,_f02,_v02)		_REG_Wr02(CTOP_M14B0_SHDW(_m),_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_M14B0_Wr03(_m,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Wr03(CTOP_M14B0_SHDW(_m),_r,_f01,_v01,_f02,_v02,_f03,_v03)


/*---------------------------------------------------------------------------------------*/

#define CTOP_CTRL_M14_RdFL(_r)							CTOP_CTRL_M14A0_RdFL(_r)
#define CTOP_CTRL_M14_WrFL(_r)							CTOP_CTRL_M14A0_WrFL(_r)
#define CTOP_CTRL_M14_Rd(_r)							CTOP_CTRL_M14A0_Rd(_r)
#define CTOP_CTRL_M14_Wr(_r, _v)						CTOP_CTRL_M14A0_Wr(_r, _v)
#define CTOP_CTRL_M14_Rd01(_r, _f01, _v01)				CTOP_CTRL_M14A0_Rd01(_r, _f01, _v01)
#define CTOP_CTRL_M14_Wr01(_r, _f01, _v01)				CTOP_CTRL_M14A0_Wr01(_r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32              *addr;
		CTOP_CTRL_REG_M14A0_T   *a0;
//		CTOP_CTRL_REG_M14B0_T   *b0;
	} shdw;

	union {
		volatile UINT32                 *addr;
		volatile CTOP_CTRL_REG_M14A0_T  *a0;
//		volatile CTOP_CTRL_REG_M14B0_T  *b0;
	} phys;
} CTOP_CTRL_REG_M14_T;


typedef struct
{
	CTOP_REG_M14B0_T	shdw;
	CTOP_REG_M14B0_T	phys;
} CTOP_CTRL_REG_M14B0_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern CTOP_CTRL_REG_M14_T		gCTOP_CTRL_M14;
extern CTOP_CTRL_REG_M14B0_T	gCTOP_CTRL_M14B0;


static inline UINT32 CTOP_CTRL_M14_READ(UINT32 offset)
{
	return _REG_OfsRd(gCTOP_CTRL_M14.shdw.addr, gCTOP_CTRL_M14.phys.addr, offset);
}

static inline void CTOP_CTRL_M14_WRITE(UINT32 offset, UINT32 value)
{
	_REG_OfsWr(gCTOP_CTRL_M14.shdw.addr, gCTOP_CTRL_M14.phys.addr, offset, value);
}


#define CTOP_CTRL_M14B0_READ(m,offset)	\
({ \
	UINT32 v = 0; \
	if(offset < sizeof(M14_B0_CTOP_##m##_TYPE)) { \
		v = _REG_OfsRd(gCTOP_CTRL_M14B0.shdw.m, gCTOP_CTRL_M14B0.phys.m, offset); \
	} \
	v; \
})

#define CTOP_CTRL_M14B0_WRITE(m,offset,value)	\
do { \
	if(offset < sizeof(M14_B0_CTOP_##m##_TYPE)) { \
		_REG_OfsWr(gCTOP_CTRL_M14B0.shdw.m, gCTOP_CTRL_M14B0.phys.m, offset, value); \
	} \
} while(0)

#endif
/**  @} */
