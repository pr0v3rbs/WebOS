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

#ifndef  __CPU_TOP_REG_H13_H__
#define  __CPU_TOP_REG_H13_H__

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sys_reg_base.h"

#include "cpu_top_reg_h13a0.h"
//#include "cpu_top_reg_h13b0.h"


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define CPU_TOP_H13A0_SHDW		(gCPU_TOP_REG_H13.shdw.a0)
#define CPU_TOP_H13B0_SHDW		(gCPU_TOP_REG_H13.shdw.b0)

#define CPU_TOP_H13A0_PHYS		(gCPU_TOP_REG_H13.phys.a0)
#define CPU_TOP_H13B0_PHYS		(gCPU_TOP_REG_H13.phys.b0)



/* H13 COMMON */
#define CPU_TOP_REG_H13_RdFL(_chip,_r)		_REG_RdFL(CPU_TOP_##_chip##_SHDW,CPU_TOP_##_chip##_PHYS,_r)
#define CPU_TOP_REG_H13_WrFL(_chip,_r)		_REG_WrFL(CPU_TOP_##_chip##_SHDW,CPU_TOP_##_chip##_PHYS,_r)

#define CPU_TOP_REG_H13_Rd(_chip,_r)		_REG_Rd(CPU_TOP_##_chip##_SHDW,_r)
#define CPU_TOP_REG_H13_Wr(_chip,_r,_v)		_REG_Wr(CPU_TOP_##_chip##_SHDW,_r,_v)


#define CPU_TOP_REG_H13_RdFd(_chip,_r,_f01) \
	_REG_RdFd(CPU_TOP_##_chip##_SHDW,_r,_f01)

#define CPU_TOP_REG_H13_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01)

#define CPU_TOP_REG_H13_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CPU_TOP_REG_H13_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define CPU_TOP_REG_H13_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01)

#define CPU_TOP_REG_H13_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define CPU_TOP_REG_H13_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(CPU_TOP_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* H13A0 */
#define CPU_TOP_H13A0_RdFL(_r)		CPU_TOP_REG_H13_RdFL(H13A0,_r)
#define CPU_TOP_H13A0_WrFL(_r)		CPU_TOP_REG_H13_WrFL(H13A0,_r)

#define CPU_TOP_H13A0_RdFd(_r,_f01)					CPU_TOP_REG_H13_RdFd(H13A0,_r,_f01)

#define CPU_TOP_H13A0_Rd01(_r,_f01,_v01)				CPU_TOP_REG_H13_Rd01(H13A0,_r,_f01,_v01)
#define CPU_TOP_H13A0_Rd02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_REG_H13_Rd02(H13A0,_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_REG_H13_Rd03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CPU_TOP_H13A0_Wr01(_r,_f01,_v01)				CPU_TOP_REG_H13_Wr01(H13A0,_r,_f01,_v01)
#define CPU_TOP_H13A0_Wr02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_REG_H13_Wr02(H13A0,_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_REG_H13_Wr03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


/* H13B0 */
#if 0
#define CPU_TOP_H13B0_RdFL(_r)		CPU_TOP_REG_H13_RdFL(H13B0,_r)
#define CPU_TOP_H13B0_WrFL(_r)		CPU_TOP_REG_H13_WrFL(H13B0,_r)

#define CPU_TOP_H13B0_RdFd(_r,_f01)					CPU_TOP_REG_H13_RdFd(H13B0,_r,_f01)

#define CPU_TOP_H13B0_Rd01(_r,_f01,_v01)			CPU_TOP_REG_H13_Rd01(H13B0,_r,_f01,_v01)
#define CPU_TOP_H13B0_Rd02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_REG_H13_Rd02(H13B0,_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_REG_H13_Rd03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CPU_TOP_H13B0_Wr01(_r,_f01,_v01)			CPU_TOP_REG_H13_Wr01(H13B0,_r,_f01,_v01)
#define CPU_TOP_H13B0_Wr02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_REG_H13_Wr02(H13B0,_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_REG_H13_Wr03(H13B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)
#else
#define CPU_TOP_H13B0_RdFL(_r)			CPU_TOP_H13A0_RdFL(_r)
#define CPU_TOP_H13B0_WrFL(_r)			CPU_TOP_H13A0_WrFL(_r)

#define CPU_TOP_H13B0_RdFd(_r,_f01)		CPU_TOP_H13A0_RdFd(_r,_f01)

#define CPU_TOP_H13B0_Rd01(_r,_f01,_v01)	CPU_TOP_H13A0_Rd01(_r,_f01,_v01)
#define CPU_TOP_H13B0_Rd02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_H13A0_Rd02(_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_H13A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define CPU_TOP_H13B0_Wr01(_r,_f01,_v01)			CPU_TOP_H13A0_Wr01(_r,_f01,_v01)
#define CPU_TOP_H13B0_Wr02(_r,_f01,_v01,_f02,_v02)	CPU_TOP_H13A0_Wr02(_r,_f01,_v01,_f02,_v02)
#define CPU_TOP_H13B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	CPU_TOP_H13A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)
#endif

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
    union {
        UINT32              *addr;
        CPU_TOP_REG_H13A0_T   *a0;
//		CPU_TOP_REG_H13B0_T   *b0;
    } shdw;

    union {
        volatile UINT32                 *addr;
        volatile CPU_TOP_REG_H13A0_T  *a0;
//        volatile CPU_TOP_REG_H13B0_T	*b0;
    } phys;
} CPU_TOP_REG_H13_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern CPU_TOP_REG_H13_T gCPU_TOP_REG_H13;


static inline UINT32 CPU_TOP_H13_READ(UINT32 offset)
{
	return _REG_OfsRd(gCPU_TOP_REG_H13.shdw.addr, gCPU_TOP_REG_H13.phys.addr, offset);
}

static inline void CPU_TOP_H13_WRITE(UINT32 offset, UINT32 value)
{
	_REG_OfsWr(gCPU_TOP_REG_H13.shdw.addr, gCPU_TOP_REG_H13.phys.addr, offset, value);
}
#endif
/**  @} */
