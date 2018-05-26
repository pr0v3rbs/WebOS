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
 * Register access for analog chip top
 *
 * author     ks.hyun (ks.hyun@lge.com)
 * version    1.0
 * date       2012.04.27
 * note       Additional information.
 *
 * @addtogroup lg115x_sys
 * @{
 */
#ifndef __ATOP_REG_M14_H__
#define __ATOP_REG_M14_H__


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "atop_reg_m14a0.h"
#include "atop_reg_m14b0.h"

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define ATOP_REG_M14_I2C_IDX			(0)

#define ATOP_REG_M14A0_SHDW		(gATOP_REG_CTRL_M14.shdw.a0)
#define ATOP_REG_M14B0_SHDW		(gATOP_REG_CTRL_M14.shdw.b0)


/* H13 COMMON */
#define ATOP_REG_M14xx_RdFL(_chip, _r)					\
	do{ 												\
		_REG_I2C_RdFL(ATOP_REG_M14_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  ATOP_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define ATOP_REG_M14xx_WrFL(_chip, _r)					\
	do{ 												\
		_REG_I2C_WrFL(ATOP_REG_M14_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1,	\
					  _r##_##_chip##_REG_ADDR,			\
					  ATOP_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define ATOP_REG_M14xx_Rd(_chip,_r)			_REG_I2C_Rd(ATOP_REG_##_chip##_SHDW,_r)
#define ATOP_REG_H13xx_Wr(_chip,_r,_v)		_REG_I2C_Wr(ATOP_REG_##_chip##_SHDW,_r,_v)


#define ATOP_REG_M14xx_RdFd(_chip,_r,_f01) \
	_REG_RdFd(ATOP_REG_##_chip##_SHDW,_r,_f01)

#define ATOP_REG_M14xx_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ATOP_REG_M14xx_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ATOP_REG_M14xx_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define ATOP_REG_M14xx_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ATOP_REG_M14xx_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ATOP_REG_M14xx_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(ATOP_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* M14A0 */
#define ATOP_REG_M14A0_RdFL(_r)			ATOP_REG_M14xx_RdFL(M14A0,_r)
#define ATOP_REG_M14A0_WrFL(_r)			ATOP_REG_M14xx_WrFL(M14A0,_r)

#define ATOP_REG_M14A0_Rd(_r)			ATOP_REG_M14xx_Rd(M14A0,_r)
#define ATOP_REG_M14A0_Wr(_r,_v)		ATOP_REG_H13xx_Wr(M14A0,_r,_v)

#define ATOP_REG_M14A0_RdFd(_r,_f01)	ATOP_REG_M14xx_RdFd(M14A0,_r,_f01)

#define ATOP_REG_M14A0_Rd01(_r,_f01,_v01)			ATOP_REG_M14xx_Rd01(M14A0,_r,_f01,_v01)
#define ATOP_REG_M14A0_Rd02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14xx_Rd02(M14A0,_r,_f01,_v01,_f02,_v02)
#define ATOP_REG_M14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ATOP_REG_M14xx_Rd03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ATOP_REG_M14A0_Wr01(_r,_f01,_v01)			ATOP_REG_M14xx_Wr01(M14A0,_r,_f01,_v01)
#define ATOP_REG_M14A0_Wr02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14xx_Wr02(M14A0,_r,_f01,_v01,_f02,_v02)
#define ATOP_REG_M14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ATOP_REG_M14xx_Wr03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* Deprecated MACROS. Changed the prefix 'ACE' to 'ATOP' */
#define ACE_REG_M14A0_RdFL(_r)		ATOP_REG_M14A0_RdFL(_r)
#define ACE_REG_M14A0_WrFL(_r)		ATOP_REG_M14A0_WrFL(_r)

#define ACE_REG_M14A0_Rd(_r)		ATOP_REG_M14A0_Rd(_r)
#define ACE_REG_M14A0_Wr(_r,_v)		ATOP_REG_M14A0_Wr(_r,_v)

#define ACE_REG_M14A0_RdFd(_r,_f01)		ATOP_REG_M14A0_RdFd(_r,_f01)

#define ACE_REG_M14A0_Rd01(_r,_f01,_v01)			ATOP_REG_M14A0_Rd01(_r,_f01,_v01)
#define ACE_REG_M14A0_Rd02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14A0_Rd02(_r,_f01,_v01,_f02,_v02)
#define ACE_REG_M14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		ATOP_REG_M14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ACE_REG_M14A0_Wr01(_r,_f01,_v01)	ATOP_REG_M14A0_Wr01(_r,_f01,_v01)
#define ACE_REG_M14A0_Wr02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14A0_Wr02(_r,_f01,_v01,_f02,_v02)
#define ACE_REG_M14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		ATOP_REG_M14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* M14B0 */
#define ATOP_REG_M14B0_RdFL(_r)			ATOP_REG_M14xx_RdFL(M14B0,_r)
#define ATOP_REG_M14B0_WrFL(_r)			ATOP_REG_M14xx_WrFL(M14B0,_r)

#define ATOP_REG_M14B0_Rd(_r)			ATOP_REG_M14xx_Rd(M14B0,_r)
#define ATOP_REG_M14B0_Wr(_r,_v)		ATOP_REG_H13xx_Wr(M14B0,_r,_v)

#define ATOP_REG_M14B0_RdFd(_r,_f01)	ATOP_REG_M14xx_RdFd(M14B0,_r,_f01)

#define ATOP_REG_M14B0_Rd01(_r,_f01,_v01)			ATOP_REG_M14xx_Rd01(M14B0,_r,_f01,_v01)
#define ATOP_REG_M14B0_Rd02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14xx_Rd02(M14B0,_r,_f01,_v01,_f02,_v02)
#define ATOP_REG_M14B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ATOP_REG_M14xx_Rd03(M14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ATOP_REG_M14B0_Wr01(_r,_f01,_v01)			ATOP_REG_M14xx_Wr01(M14B0,_r,_f01,_v01)
#define ATOP_REG_M14B0_Wr02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14xx_Wr02(M14B0,_r,_f01,_v01,_f02,_v02)
#define ATOP_REG_M14B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ATOP_REG_M14xx_Wr03(M14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

/* Deprecated MACROS. Changed the prefix 'ACE' to 'ATOP' */
#define ACE_REG_M14B0_RdFL(_r)		ATOP_REG_M14B0_RdFL(_r)
#define ACE_REG_M14B0_WrFL(_r)		ATOP_REG_M14B0_WrFL(_r)

#define ACE_REG_M14B0_Rd(_r)		ATOP_REG_M14B0_Rd(_r)
#define ACE_REG_M14B0_Wr(_r,_v)		ATOP_REG_M14B0_Wr(_r,_v)

#define ACE_REG_M14B0_RdFd(_r,_f01)		ATOP_REG_M14B0_RdFd(_r,_f01)

#define ACE_REG_M14B0_Rd01(_r,_f01,_v01)			ATOP_REG_M14B0_Rd01(_r,_f01,_v01)
#define ACE_REG_M14B0_Rd02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14B0_Rd02(_r,_f01,_v01,_f02,_v02)
#define ACE_REG_M14B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		ATOP_REG_M14B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ACE_REG_M14B0_Wr01(_r,_f01,_v01)	ATOP_REG_M14B0_Wr01(_r,_f01,_v01)
#define ACE_REG_M14B0_Wr02(_r,_f01,_v01,_f02,_v02)	ATOP_REG_M14B0_Wr02(_r,_f01,_v01,_f02,_v02)
#define ACE_REG_M14B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		ATOP_REG_M14B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)



/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32          	*addr;
		ATOP_REG_M14A0_T	*a0;
		ATOP_REG_M14B0_T	*b0;
	} shdw;
} ATOP_REG_CTRL_M14_T;



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern ATOP_REG_CTRL_M14_T	gATOP_REG_CTRL_M14;

#endif
