/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  sdec driver
 *
 *  @author	Kyungseok Hyun ( ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef _SDEC_REG_CTRL_H_
#define _SDEC_REG_CTRL_H_


#include "sys_reg_base.h"
#include "sdec_cfg.h"

#include "sdec_reg.h"
#include "sdec_reg_h13a0.h"
#include "sdec_reg_m14c0.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	union {
		UINT32				*addr;
		SDTOP_REG_T			*common;
		SDTOP_REG_H13A0_T	*H13A0;
	} shdw;

	union {
		volatile UINT32				*addr;
		volatile SDTOP_REG_T		*common;
		volatile SDTOP_REG_H13A0_T	*H13A0;
	} phys;

	UINT32	size;
} SDTOP_REG_CTRL_T;


typedef struct
{
	union {
		UINT32				*addr;
		SDIO_REG_T			*common;
		SDIO_REG_M14C0_T	*M14C0;
	} shdw;

	union {
		volatile UINT32				*addr;
		volatile SDIO_REG_T			*common;
		volatile SDIO_REG_M14C0_T	*M14C0;
	} phys;

	UINT32	size;
} SDIO_REG_CTRL_T;

typedef struct
{
	union {
		UINT32		*addr;
		MPG_REG_T	*reg;
	} shdw[2];

	union {
		volatile UINT32		*addr;
		volatile MPG_REG_T	*reg;
	} phys[2];

	UINT32	size;

	UINT32 pidf_data[2][64];
	UINT32 secf_data[2][64][7];
} MPG_REG_CTRL_T;


#define SDEC_REG_WRITE(addr, val)		__raw_writel(val , (void*)(addr))
#define SDEC_REG_READ(addr)				__raw_readl((void*)(addr))


/* SDEC TOP Reg Access */
#define SDTOP_REG_SHDW(_c)				(g_sdec_sdtop_reg_ctrl->shdw._c)
#define SDTOP_REG_PHYS(_c)				(g_sdec_sdtop_reg_ctrl->phys._c)

#define SDTOP_RdPHYofs(_ofs)			SDEC_REG_READ(_REG_Addr(SDTOP_REG_PHYS(addr),_ofs))
#define SDTOP_WrPHYofs(_ofs, _v)		SDEC_REG_WRITE(_REG_Addr(SDTOP_REG_PHYS(addr),_ofs), _v)

#define SDTOP_RdFLOfs(_ofs)				_REG_RdFLOfs(SDTOP_REG_SHDW(addr), SDTOP_REG_PHYS(addr),_ofs)
#define SDTOP_WrFLOfs(_ofs)				_REG_WrFLOfs(SDTOP_REG_SHDW(addr), SDTOP_REG_PHYS(addr),_ofs)

#define SDTOP_RdFL_EX(_c,_r)			_REG_RdFL(SDTOP_REG_SHDW(_c), SDTOP_REG_PHYS(_c), _r)
#define SDTOP_WrFL_EX(_c,_r)			_REG_WrFL(SDTOP_REG_SHDW(_c), SDTOP_REG_PHYS(_c), _r)

#define SDTOP_Rd_EX(_c,_r,_v)			do{ _v = _REG_Rd(SDTOP_REG_SHDW(_c),_r); } while(0)
#define SDTOP_Wr_EX(_c,_r,_v)			_REG_Wr(SDTOP_REG_SHDW(_c),_r,_v)

#define SDTOP_Rd01_EX(_c,_r,_f01,_v01)	_REG_Rd01(SDTOP_REG_SHDW(_c),_r,_f01,_v01)
#define SDTOP_Wr01_EX(_c,_r,_f01,_v01)	_REG_Wr01(SDTOP_REG_SHDW(_c),_r,_f01,_v01)


#define SDTOP_RdFL(_r)					SDTOP_RdFL_EX(common,_r)
#define SDTOP_WrFL(_r)					SDTOP_WrFL_EX(common,_r)

#define SDTOP_Rd(_r,_v)					SDTOP_Rd_EX(common,_r,_v)
#define SDTOP_Wr(_r,_v)					SDTOP_Wr_EX(common,_r,_v)

#define SDTOP_Rd01(_r,_f01,_v01)		SDTOP_Rd01_EX(common,_r,_f01,_v01)
#define SDTOP_Wr01(_r,_f01,_v01)		SDTOP_Wr01_EX(common,_r,_f01,_v01)



/* SDEC IO Reg Access */
#define SDIO_REG_SHDW(_c,_n)				(g_sdec_sdio_reg_ctrl[_n].shdw._c)
#define SDIO_REG_PHYS(_c,_n)				(g_sdec_sdio_reg_ctrl[_n].phys._c)

#define SDIO_RdPHYofs(core,_ofs)			SDEC_REG_READ(_REG_Addr(SDIO_REG_PHYS(addr,core),_ofs))
#define SDIO_WrPHYofs(core,_ofs,_v)			SDEC_REG_WRITE(_REG_Addr(SDIO_REG_PHYS(addr,core),_ofs), _v)

#define SDIO_RdFLOfs(_n,_ofs)				_REG_RdFLOfs(SDIO_REG_SHDW(addr,_n), SDIO_REG_PHYS(addr,_n),_ofs)
#define SDIO_WrFLOfs(_n,_ofs)				_REG_WrFLOfs(SDIO_REG_SHDW(addr,_n), SDIO_REG_PHYS(addr,_n),_ofs)

#define SDIO_RdFL_EX(_c,_n,_r)				_REG_RdFL(SDIO_REG_SHDW(_c,_n), SDIO_REG_PHYS(_c,_n), _r)
#define SDIO_WrFL_EX(_c,_n,_r)				_REG_WrFL(SDIO_REG_SHDW(_c,_n), SDIO_REG_PHYS(_c,_n), _r)

#define SDIO_Rd_EX(_c,_n,_r,_v)				do{ _v = _REG_Rd(SDIO_REG_SHDW(_c,_n),_r); } while(0)
#define SDIO_Wr_EX(_c,_n,_r,_v)				_REG_Wr(SDIO_REG_SHDW(_c,_n),_r,_v)

#define SDIO_Rd01_EX(_c,_n,_r,_f01,_v01)	_REG_Rd01(SDIO_REG_SHDW(_c,_n),_r,_f01,_v01)
#define SDIO_Wr01_EX(_c,_n,_r,_f01,_v01)	_REG_Wr01(SDIO_REG_SHDW(_c,_n),_r,_f01,_v01)

#define SDIO_RdFL(_n,_r)					SDIO_RdFL_EX(common,_n,_r)
#define SDIO_WrFL(_n,_r)					SDIO_WrFL_EX(common,_n,_r)

#define SDIO_Rd(_n,_r,_v)					SDIO_Rd_EX(common,_n,_r,_v)
#define SDIO_Wr(_n,_r,_v)					SDIO_Wr_EX(common,_n,_r,_v)

#define SDIO_Rd01(_n,_r,_f01,_v01)			SDIO_Rd01_EX(common,_n,_r,_f01,_v01)
#define SDIO_Wr01(_n,_r,_f01,_v01)			SDIO_Wr01_EX(common,_n,_r,_f01,_v01)



/* SDEC Core Reg Access */
#define MPG_REG_SHDW(core,ch)			(g_sdec_mpg_reg_ctrl[core].shdw[ch].reg)
#define MPG_REG_PHYS(core,ch)			(g_sdec_mpg_reg_ctrl[core].phys[ch].reg)
#define MPG_REG_PIDF_SHDW(core,ch,i)	(g_sdec_mpg_reg_ctrl[core].pidf_data[ch][i])
#define MPG_REG_SECF_SHDW(core,ch,i,j)	(g_sdec_mpg_reg_ctrl[core].secf_data[ch][i][j])

#define MPG_RdFLOfs(core,ch,_ofs)		_REG_RdFLOfs(MPG_REG_SHDW(core,ch), MPG_REG_PHYS(core,ch),_ofs)
#define MPG_WrFLOfs(core,ch,_ofs)		_REG_WrFLOfs(MPG_REG_SHDW(core,ch), MPG_REG_PHYS(core,ch),_ofs)

#define MPG_RdPHYofs(core,ch,_ofs)		SDEC_REG_READ(_REG_Addr(MPG_REG_PHYS(core,ch),_ofs))
#define MPG_WrPHYofs(core,ch,_ofs,_v)	SDEC_REG_WRITE(_REG_Addr(MPG_REG_PHYS(core,ch),_ofs),_v)


#define MPG_RdFL(_i, _r)		\
	do{							\
		int __core = _i >> 2;	\
		int __n = _i & 0x1;		\
		_REG_RdFL(MPG_REG_SHDW(__core, __n), MPG_REG_PHYS(__core, __n), _r);	\
	} while(0)

#define MPG_WrFL(_i,_r)			\
	do{							\
		int __core = _i >> 2;	\
		int __n = _i & 0x1;		\
		_REG_WrFL(MPG_REG_SHDW(__core, __n), MPG_REG_PHYS(__core, __n), _r);	\
	} while(0)

#define MPG_Rd(_i, _r, _v)		\
	do{							\
		int __core = _i >> 2;	\
		int __n = _i & 0x1;		\
		_v = _REG_Rd(MPG_REG_SHDW(__core,__n),_r);	\
	} while(0)

#define MPG_Wr(_i, _r,_v)		\
	do{							\
		int __core = _i >> 2;	\
		int __n = _i & 0x1;		\
		_REG_Wr(MPG_REG_SHDW(__core,__n),_r,_v);	\
	} while(0)

#define MPG_Rd01(_i, _r,_f01,_v01)	\
	do{								\
		int __core = _i >> 2;		\
		int __n = _i & 0x1;			\
		_REG_Rd01(MPG_REG_SHDW(__core,__n),_r,_f01,_v01);	\
	} while(0)

#define MPG_Wr01(_i, _r,_f01,_v01)	\
	do{								\
		int __core = _i >> 2;		\
		int __n = _i & 0x1;			\
		_REG_Wr01(MPG_REG_SHDW(__core,__n),_r,_f01,_v01);	\
	} while(0)


// read data from shashadow to shadow's pidf_data
#define MPG_RdFL_SShadow(_i, _r)	\
	do{								\
		int __core = _i >> 2;		\
		int __n = _i & 0x1;			\
		_REG_Wr01(MPG_REG_SHDW(__core,__n),pidf_data,pidf_data,MPG_REG_PIDF_SHDW(__core,__n,_r));	\
	} while(0)

// write data from shadow's pidf_data to shashadow
#define MPG_WrFL_SShadow(_i, _r)	\
	do{								\
		int __core = _i >> 2;		\
		int __n = _i & 0x1;			\
		_REG_Rd01(MPG_REG_SHDW(__core,__n),pidf_data,pidf_data,MPG_REG_PIDF_SHDW(__core,__n,_r));	\
	} while(0)



// read data from shashadow to shadow's secf_data
#define MPG_RdFL_Secf_SShadow(_c, _r, _i)	\
	do{ 									\
		int __core = _c >> 2;				\
		int __n = _c & 0x1;					\
		_REG_Wr01(MPG_REG_SHDW(__core,__n),secf_data,secf_data,MPG_REG_SECF_SHDW(__core,__n,_r,_i));	\
	} while(0)


// write data from shadow's secf_data to shashadow
#define MPG_WrFL_Secf_SShadow(_c, _r, _i)	\
	do{ 									\
		int __core = _c >> 2;				\
		int __n = _c & 0x1;					\
		_REG_Rd01(MPG_REG_SHDW(__core,__n),secf_data,secf_data,MPG_REG_SECF_SHDW(__core,__n,_r,_i));	\
	} while(0)




extern SDTOP_REG_CTRL_T *g_sdec_sdtop_reg_ctrl;
extern SDIO_REG_CTRL_T	*g_sdec_sdio_reg_ctrl;
extern MPG_REG_CTRL_T	*g_sdec_mpg_reg_ctrl;


#ifdef __cplusplus
}
#endif

#endif

