/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 



/** @file
 *
 * Register access for hdmi chip
 *
 * author     ks.hyun (ks.hyun@lge.com)
 * version    1.0
 * date       2012.04.27
 * note       Additional information.
 *
 * @addtogroup lg115x_sys
 * @{
 */
#ifndef __HDMI_REG_M14_H__
#define __HDMI_REG_M14_H__


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "link_reg_m14a0.h"
#include "phy_reg_m14a0.h"

#include "link_reg_m14b0.h"
#include "phy_reg_m14b0.h"

#include "link_reg_m14c0.h"
#include "phy_reg_m14c0.h"
/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PHY_REG_M14_I2C_IDX			(1)

#define PHY_REG_M14A0_SHDW		(gPHY_REG_CTRL_M14A0.shdw.m14)
#define PHY_REG_M14B0_SHDW		(gPHY_REG_CTRL_M14B0.shdw.m14)
#define PHY_REG_M14C0_SHDW		(gPHY_REG_CTRL_M14C0.shdw.m14)

/* M14 COMMON */
#define PHY_REG_RdFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_RdFL(PHY_REG_M14_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  PHY_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define PHY_REG_WrFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_WrFL(PHY_REG_M14_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1,	\
					  _r##_##_chip##_REG_ADDR,			\
					  PHY_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define PHY_REG_Rd(_chip,_r)			_REG_Rd(PHY_REG_##_chip##_SHDW,_r)
#define PHY_REG_Wr(_chip,_r,_v)			_REG_Wr(PHY_REG_##_chip##_SHDW,_r,_v)

#define PHY_REG_RdFd(_chip,_r,_f01) \
	_REG_RdFd(PHY_REG_##_chip##_SHDW,_r,_f01)

#define PHY_REG_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(PHY_REG_##_chip##_SHDW,_r,_f01,_v01)

#define PHY_REG_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(PHY_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define PHY_REG_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(PHY_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define PHY_REG_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(PHY_REG_##_chip##_SHDW,_r,_f01,_v01)

#define PHY_REG_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(PHY_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define PHY_REG_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
		_REG_Wr03(PHY_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/* M14 */
#define PHY_REG_M14A0_RdFL(_r)		PHY_REG_RdFL(M14A0,_r)
#define PHY_REG_M14A0_WrFL(_r)		PHY_REG_WrFL(M14A0,_r)

#define PHY_REG_M14A0_RdFd(_r,_f01)					PHY_REG_RdFd(M14A0,_r,_f01)

#define PHY_REG_M14A0_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(M14A0,_r,_f01,_v01)
#define PHY_REG_M14A0_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(M14A0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_M14A0_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(M14A0,_r,_f01,_v01)
#define PHY_REG_M14A0_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(M14A0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(M14A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_M14B0_RdFL(_r)		PHY_REG_RdFL(M14B0,_r)
#define PHY_REG_M14B0_WrFL(_r)		PHY_REG_WrFL(M14B0,_r)

#define PHY_REG_M14B0_RdFd(_r,_f01)					PHY_REG_RdFd(M14B0,_r,_f01)

#define PHY_REG_M14B0_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(M14B0,_r,_f01,_v01)
#define PHY_REG_M14B0_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(M14B0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(M14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_M14B0_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(M14B0,_r,_f01,_v01)
#define PHY_REG_M14B0_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(M14B0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(M14B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_M14C0_RdFL(_r)		PHY_REG_RdFL(M14C0,_r)
#define PHY_REG_M14C0_WrFL(_r)		PHY_REG_WrFL(M14C0,_r)

#define PHY_REG_M14C0_RdFd(_r,_f01)					PHY_REG_RdFd(M14C0,_r,_f01)

#define PHY_REG_M14C0_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(M14C0,_r,_f01,_v01)
#define PHY_REG_M14C0_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(M14C0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14C0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(M14C0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_M14C0_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(M14C0,_r,_f01,_v01)
#define PHY_REG_M14C0_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(M14C0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_M14C0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(M14C0,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define LINK_M14_RdFL(_r)			((pstLinkShadowReg->_r)=(pstLinkReg->_r))

#define LINK_M14_WrFL(_r)			((pstLinkReg->_r)=(pstLinkShadowReg->_r))

#define LINK_M14_Rd(_r)				*((UINT32*)(&(pstLinkShadowReg->_r)))
#define LINK_M14_Wr(_r,_v)			((LINK_M14_Rd(_r))=((UINT32)(_v)))


#define LINK_M14_Rd_fld(_r,_f01)	(pstLinkShadowReg->_r._f01)
								
#define LINK_M14_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
								} while(0)

#define LINK_M14_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
									(_v02) = (pstLinkShadowReg->_r._f02);		\
								} while(0)

#define LINK_M14_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
									(_v02) = (pstLinkShadowReg->_r._f02);		\
									(_v03) = (pstLinkShadowReg->_r._f03);		\
								} while(0)

#define LINK_M14_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pstLinkShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK_M14_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pstLinkShadowReg->_r._f01) = (_v01);		\
									(pstLinkShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define LINK_M14_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
								do { 											\
									(pstLinkShadowReg->_r._f01) = (_v01);		\
									(pstLinkShadowReg->_r._f02) = (_v02);		\
									(pstLinkShadowReg->_r._f03) = (_v03);		\
								} while(0)



/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32          *addr;
		PHY_REG_M14A0_T	*m14;
//		PHY_REG_M14Bx_T	*m14bx;
	} shdw;
} PHY_REG_CTRL_M14A0_T;

typedef struct
{
	union {
		UINT32          *addr;
		PHY_REG_M14B0_T	*m14;
//		PHY_REG_M14Bx_T	*m14bx;
	} shdw;
} PHY_REG_CTRL_M14B0_T;

typedef struct
{
	union {
		UINT32          *addr;
		PHY_REG_M14C0_T	*m14;
//		PHY_REG_M14Bx_T	*m14bx;
	} shdw;
} PHY_REG_CTRL_M14C0_T;

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PHY_REG_CTRL_M14A0_T	gPHY_REG_CTRL_M14A0;
extern PHY_REG_CTRL_M14B0_T	gPHY_REG_CTRL_M14B0;
extern PHY_REG_CTRL_M14C0_T	gPHY_REG_CTRL_M14C0;

#endif
