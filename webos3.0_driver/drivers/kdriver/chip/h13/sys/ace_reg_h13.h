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
 * Register access for ace chip
 *
 * author     ks.hyun (ks.hyun@lge.com)
 * version    1.0
 * date       2012.04.27
 * note       Additional information.
 *
 * @addtogroup lg115x_sys
 * @{
 */
#ifndef __ACE_REG_H13_H__
#define __ACE_REG_H13_H__


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "ace_reg_h13a0.h"
//#include "ace_reg_h13b0.h"

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define ACE_REG_H13_I2C_IDX			(0)

#define ACE_REG_H13A0_SHDW		(gACE_REG_CTRL_H13.shdw.a0)
#define ACE_REG_H13B0_SHDW		(gACE_REG_CTRL_H13.shdw.b0)


/* H13 COMMON */
#define ACE_REG_H13xx_RdFL(_chip, _r)					\
	do{ 												\
		_REG_I2C_RdFL(ACE_REG_H13_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  ACE_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define ACE_REG_H13xx_WrFL(_chip, _r)					\
	do{ 												\
		_REG_I2C_WrFL(ACE_REG_H13_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1,	\
					  _r##_##_chip##_REG_ADDR,			\
					  ACE_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define ACE_REG_H13xx_Rd(_chip,_r)			_REG_I2C_Rd(ACE_REG_##_chip##_SHDW,_r)
#define ACE_REG_H13xx_Wr(_chip,_r,_v)		_REG_I2C_Wr(ACE_REG_##_chip##_SHDW,_r,_v)


#define ACE_REG_H13xx_RdFd(_chip,_r,_f01) \
	_REG_RdFd(ACE_REG_##_chip##_SHDW,_r,_f01)

#define ACE_REG_H13xx_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(ACE_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ACE_REG_H13xx_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ACE_REG_H13xx_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define ACE_REG_H13xx_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(ACE_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ACE_REG_H13xx_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ACE_REG_H13xx_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* H13A0 */
#define ACE_REG_H13A0_RdFL(_r)		ACE_REG_H13xx_RdFL(H13A0,_r)
#define ACE_REG_H13A0_WrFL(_r)		ACE_REG_H13xx_WrFL(H13A0,_r)

#define ACE_REG_H13A0_Rd(_r)		ACE_REG_H13xx_Rd(H13A0,_r)
#define ACE_REG_H13A0_Wr(_r,_v)		ACE_REG_H13xx_Wr(H13A0,_r,_v)

#define ACE_REG_H13A0_RdFd(_r,_f01)					ACE_REG_H13xx_RdFd(H13A0,_r,_f01)

#define ACE_REG_H13A0_Rd01(_r,_f01,_v01)			ACE_REG_H13xx_Rd01(H13A0,_r,_f01,_v01)
#define ACE_REG_H13A0_Rd02(_r,_f01,_v01,_f02,_v02)	ACE_REG_H13xx_Rd02(H13A0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_H13A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_H13xx_Rd03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ACE_REG_H13A0_Wr01(_r,_f01,_v01)			ACE_REG_H13xx_Wr01(H13A0,_r,_f01,_v01)
#define ACE_REG_H13A0_Wr02(_r,_f01,_v01,_f02,_v02)	ACE_REG_H13xx_Wr02(H13A0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_H13A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_H13xx_Wr03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* H13B0 */
#define ACE_REG_H13B0_RdFL(_r)		ACE_REG_H13xx_RdFL(H13B0,_r)
#define ACE_REG_H13B0_WrFL(_r)		ACE_REG_H13xx_WrFL(H13B0,_r)

#define ACE_REG_H13B0_Rd(_r)		ACE_REG_H13xx_Rd(H13B0,_r)
#define ACE_REG_H13B0_Wr(_r,_v)		ACE_REG_H13xx_Wr(H13B0,_r,_v)

#define ACE_REG_H13B0_RdFd(_r,_f01)					ACE_REG_H13xx_RdFd(H13B0,_r,_f01)

#define ACE_REG_H13B0_Rd01(_r,_f01,_v01)			ACE_REG_H13xx_Rd01(H13B0,_r,_f01,_v01)
#define ACE_REG_H13B0_Rd02(_r,_f01,_v01,_f02,_v02)	ACE_REG_H13xx_Rd02(H13B0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_H13B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_H13xx_Rd03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ACE_REG_H13B0_Wr01(_r,_f01,_v01)			ACE_REG_H13xx_Wr01(H13B0,_r,_f01,_v01)
#define ACE_REG_H13B0_Wr02(_r,_f01,_v01,_f02,_v02)	ACE_REG_H13xx_Wr02(H13B0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_H13B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_H13xx_Wr03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32          *addr;
		ACE_REG_H13A0_T	*a0;
//		ACE_REG_H13B0_T	*b0;
	} shdw;
} ACE_REG_CTRL_H13_T;



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern ACE_REG_CTRL_H13_T	gACE_REG_CTRL_H13;


#endif
