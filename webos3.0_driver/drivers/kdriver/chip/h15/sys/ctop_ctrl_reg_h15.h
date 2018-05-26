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

#ifndef  __CTOP_CTRL_REG_H15_H__
#define  __CTOP_CTRL_REG_H15_H__

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sys_reg_base.h"


#include "ctop_ctrl_reg_h15a0.h"
//#include "ctop_ctrl_reg_h15b0.h"


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/* H15A0 */

#define CTOP_H15A0_SHDW(_m)					(gCTOP_CTRL_H15.shdw.a0._m)
#define CTOP_H15A0_PHYS(_m)					(gCTOP_CTRL_H15.phys.a0._m)

#define CTOP_CTRL_H15A0_RdFL(_m,_r)			_REG_RdFL(CTOP_H15A0_SHDW(_m),CTOP_H15A0_PHYS(_m),_r)
#define CTOP_CTRL_H15A0_WrFL(_m,_r)			_REG_WrFL(CTOP_H15A0_SHDW(_m),CTOP_H15A0_PHYS(_m),_r)

#define CTOP_CTRL_H15A0_Rd(_m,_r)			_REG_Rd(CTOP_H15A0_SHDW(_m),_r)
#define CTOP_CTRL_H15A0_Wr(_m,_r,_v)		_REG_Wr(CTOP_H15A0_SHDW(_m),_r,_v)


#define CTOP_CTRL_H15A0_RdFd(_m,_r,_f01)					_REG_RdFd(CTOP_H15A0_SHDW(_m),_r,_f01)
#define CTOP_CTRL_H15A0_Rd01(_m,_r,_f01,_v01)				_REG_Rd01(CTOP_H15A0_SHDW(_m),_r,_f01,_v01)
#define CTOP_CTRL_H15A0_Rd02(_m,_r,_f01,_v01,_f02,_v02)		_REG_Rd02(CTOP_H15A0_SHDW(_m),_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H15A0_Rd03(_m,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CTOP_H15A0_SHDW(_m),_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CTOP_CTRL_H15A0_Wr01(_m,_r,_f01,_v01)				_REG_Wr01(CTOP_H15A0_SHDW(_m),_r,_f01,_v01)
#define CTOP_CTRL_H15A0_Wr02(_m,_r,_f01,_v01,_f02,_v02)		_REG_Wr02(CTOP_H15A0_SHDW(_m),_r,_f01,_v01,_f02,_v02)
#define CTOP_CTRL_H15A0_Wr03(_m,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Wr03(CTOP_H15A0_SHDW(_m),_r,_f01,_v01,_f02,_v02,_f03,_v03)



/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		CTOP_REG_H15A0_T	a0;
//		CTOP_REG_H15B0_T	b0;
	} shdw;

	union {
		CTOP_REG_H15A0_T	a0;
//		CTOP_REG_H15B0_T	b0;
	} phys;
} CTOP_CTRL_REG_H15_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern CTOP_CTRL_REG_H15_T		gCTOP_CTRL_H15;



#define CTOP_CTRL_H15A0_READ(m,offset)	\
({ \
	UINT32 v = 0; \
	if(offset < sizeof(H15_A0_CTOP_##m##_TYPE)) { \
		v = _REG_OfsRd(gCTOP_CTRL_H15.shdw.a0.m, gCTOP_CTRL_H15.phys.a0.m, offset); \
	} \
	v; \
})

#define CTOP_CTRL_H15A0_WRITE(m,offset,value)	\
do { \
	if(offset < sizeof(H15_A0_CTOP_##m##_TYPE)) { \
		_REG_OfsWr(gCTOP_CTRL_H15.shdw.a0.m, gCTOP_CTRL_H15.phys.a0.m, offset, value); \
	} \
} while(0)

#endif
/**  @} */
