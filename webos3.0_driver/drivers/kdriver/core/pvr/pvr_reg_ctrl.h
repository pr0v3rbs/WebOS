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
 *  register access interface for the LG1150 hardware pvr
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2014.05.13
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef _PVR_REG_CTRL_H_
#define _PVR_REG_CTRL_H_

#include "sys_reg_base.h"
#include "pvr_cfg.h"
#include "pvr_reg_db.h"


typedef enum
{
	PVR_REG_BLOCK_TOP	= 0x0,
	PVR_REG_BLOCK_PIE,
	PVR_REG_BLOCK_DN,
	PVR_REG_BLOCK_UP,
	PVR_REG_BLOCK_MAX,
} PVR_REG_BLOCK_T;


typedef struct {

	struct {
		UINT32			*base;
		DVR_TOP_REG_T	*top;
		DVR_PIE_REG_T	*pie[2];
		DVR_DN_REG_T	*dn[2];
		DVR_UP_REG_T	*up[2];
	} shdw;

	struct {
		volatile UINT32 		*base;
		volatile DVR_TOP_REG_T	*top;
		volatile DVR_PIE_REG_T	*pie[2];
		volatile DVR_DN_REG_T	*dn[2];
		volatile DVR_UP_REG_T	*up[2];
	} phys;

	UINT32 size[PVR_REG_BLOCK_MAX];
} DVR_REG_CTRL_T;



/* PVR TOP Reg Access */
#define PVR_TOP_REG_SHDW				(g_dvr_reg_ctrl->shdw.top)
#define PVR_TOP_REG_PHYS				(g_dvr_reg_ctrl->phys.top)
#define PVR_TOP_REG_SIZE				(g_dvr_reg_ctrl->size[PVR_REG_BLOCK_TOP])

#define PVR_TOP_REG_READ(_r)			(PVR_TOP_REG_PHYS->_r)
#define PVR_TOP_REG_WRITE(_r,_v)		((PVR_TOP_REG_PHYS->_r) = _v)

#define PVR_TOP_REG_READ32(_r)			(*((UINT32*)(&(PVR_TOP_REG_PHYS->_r))))
#define PVR_TOP_REG_WRITE32(_r,_v)		(*((UINT32*)(&(PVR_TOP_REG_PHYS->_r))) = _v)


#define PVR_TOP_RdFL(_r)				_REG_RdFL(PVR_TOP_REG_SHDW, PVR_TOP_REG_PHYS, _r)
#define PVR_TOP_WrFL(_r)				_REG_WrFL(PVR_TOP_REG_SHDW, PVR_TOP_REG_PHYS, _r)

#define PVR_TOP_Rd(_r,_v)				do{ _v = _REG_Rd(PVR_TOP_REG_SHDW,_r); } while(0)
#define PVR_TOP_Wr(_r,_v)				_REG_Wr(PVR_TOP_REG_SHDW,_r,_v)

#define PVR_TOP_Rd01(_r,_f01,_v01)		_REG_Rd01(PVR_TOP_REG_SHDW,_r,_f01,_v01)
#define PVR_TOP_Wr01(_r,_f01,_v01)		_REG_Wr01(PVR_TOP_REG_SHDW,_r,_f01,_v01)

/* PVR PIE Reg Access */
#define PVR_PIE_REG_SHDW(_c)			(g_dvr_reg_ctrl->shdw.pie[_c])
#define PVR_PIE_REG_PHYS(_c)			(g_dvr_reg_ctrl->phys.pie[_c])
#define PVR_PIE_REG_SIZE				(g_dvr_reg_ctrl->size[PVR_REG_BLOCK_PIE])

#define PVR_PIE_REG_READ(_c,_r)			(PVR_PIE_REG_PHYS(_c)->_r)
#define PVR_PIE_REG_WRITE(_c,_r,_v)		((PVR_PIE_REG_PHYS(_c)->_r) = _v)

#define PVR_PIE_REG_READ32(_c,_r)		(*((UINT32*)(&(PVR_PIE_REG_PHYS(_c)->_r))))
#define PVR_PIE_REG_WRITE32(_c,_r,_v)	(*((UINT32*)(&(PVR_PIE_REG_PHYS(_c)->_r))) = _v)

#define PVR_PIE_RdFL(_c,_r)				_REG_RdFL(PVR_PIE_REG_SHDW(_c), PVR_PIE_REG_PHYS(_c), _r)
#define PVR_PIE_WrFL(_c,_r)				_REG_WrFL(PVR_PIE_REG_SHDW(_c), PVR_PIE_REG_PHYS(_c), _r)

#define PVR_PIE_Rd(_c,_r,_v)			do{ _v = _REG_Rd(PVR_PIE_REG_SHDW(_c),_r); } while(0)
#define PVR_PIE_Wr(_c,_r,_v)			_REG_Wr(PVR_PIE_REG_SHDW(_c),_r,_v)

#define PVR_PIE_Rd01(_c,_r,_f01,_v01)	_REG_Rd01(PVR_PIE_REG_SHDW(_c),_r,_f01,_v01)
#define PVR_PIE_Wr01(_c,_r,_f01,_v01)	_REG_Wr01(PVR_PIE_REG_SHDW(_c),_r,_f01,_v01)

/* PVR DN Reg Access */
#define PVR_DN_REG_SHDW(_c)				(g_dvr_reg_ctrl->shdw.dn[_c])
#define PVR_DN_REG_PHYS(_c)				(g_dvr_reg_ctrl->phys.dn[_c])
#define PVR_DN_REG_SIZE					(g_dvr_reg_ctrl->size[PVR_REG_BLOCK_DN])

#define PVR_DN_RdFL(_c,_r)				_REG_RdFL(PVR_DN_REG_SHDW(_c), PVR_DN_REG_PHYS(_c), _r)
#define PVR_DN_WrFL(_c,_r)				_REG_WrFL(PVR_DN_REG_SHDW(_c), PVR_DN_REG_PHYS(_c), _r)

#define PVR_DN_Rd(_c,_r,_v)				do{ _v = _REG_Rd(PVR_DN_REG_SHDW(_c),_r); } while(0)
#define PVR_DN_Wr(_c,_r,_v)				_REG_Wr(PVR_DN_REG_SHDW(_c),_r,_v)

#define PVR_DN_Rd01(_c,_r,_f01,_v01)	_REG_Rd01(PVR_DN_REG_SHDW(_c),_r,_f01,_v01)
#define PVR_DN_Wr01(_c,_r,_f01,_v01)	_REG_Wr01(PVR_DN_REG_SHDW(_c),_r,_f01,_v01)

/* PVR UP Reg Access */
#define PVR_UP_REG_SHDW(_c)				(g_dvr_reg_ctrl->shdw.up[_c])
#define PVR_UP_REG_PHYS(_c)				(g_dvr_reg_ctrl->phys.up[_c])
#define PVR_UP_REG_SIZE					(g_dvr_reg_ctrl->size[PVR_REG_BLOCK_UP])

#define PVR_UP_REG_READ(_c,_r)			(PVR_UP_REG_PHYS(_c)->_r)
#define PVR_UP_REG_WRITE(_c,_r,_v)		((PVR_UP_REG_PHYS(_c)->_r) = _v)


#define PVR_UP_RdFL(_c,_r)				_REG_RdFL(PVR_UP_REG_SHDW(_c), PVR_UP_REG_PHYS(_c), _r)
#define PVR_UP_WrFL(_c,_r)				_REG_WrFL(PVR_UP_REG_SHDW(_c), PVR_UP_REG_PHYS(_c), _r)

#define PVR_UP_Rd(_c,_r,_v)				do{ _v = _REG_Rd(PVR_UP_REG_SHDW(_c),_r); } while(0)
#define PVR_UP_Wr(_c,_r,_v)				_REG_Wr(PVR_UP_REG_SHDW(_c),_r,_v)

#define PVR_UP_Rd01(_c,_r,_f01,_v01)	_REG_Rd01(PVR_UP_REG_SHDW(_c),_r,_f01,_v01)
#define PVR_UP_Wr01(_c,_r,_f01,_v01)	_REG_Wr01(PVR_UP_REG_SHDW(_c),_r,_f01,_v01)




#define PVR_REG_WRITE(addr, val)	__raw_writel(val , (void*)(addr))
#define PVR_REG_READ(addr)			__raw_readl((void*)(addr))


extern DVR_REG_CTRL_T	*g_dvr_reg_ctrl;



#endif /* _PVR_REG_CTRL_H_ */
