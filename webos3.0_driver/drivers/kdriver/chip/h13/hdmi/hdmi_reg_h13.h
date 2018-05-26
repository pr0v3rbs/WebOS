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
#ifndef __HDMI_REG_H13_H__
#define __HDMI_REG_H13_H__


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "link_reg_h13.h"
#include "phy_reg_h13.h"

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PHY_REG_H13_I2C_IDX			(1)

#define PHY_REG_H13_SHDW		(gPHY_REG_CTRL_H13.shdw.h13)


/* H13 COMMON */
#define PHY_REG_RdFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_RdFL(PHY_REG_H13_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  PHY_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define PHY_REG_WrFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_WrFL(PHY_REG_H13_I2C_IDX,				\
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



/* H13 */
#define PHY_REG_H13_RdFL(_r)		PHY_REG_RdFL(H13,_r)
#define PHY_REG_H13_WrFL(_r)		PHY_REG_WrFL(H13,_r)

#define PHY_REG_H13_RdFd(_r,_f01)					PHY_REG_RdFd(H13,_r,_f01)

#define PHY_REG_H13_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(H13,_r,_f01,_v01)
#define PHY_REG_H13_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(H13,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H13_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(H13A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_H13_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(H13,_r,_f01,_v01)
#define PHY_REG_H13_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(H13,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H13_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(H13,_r,_f01,_v01,_f02,_v02,_f03,_v03)




#define _LINK_H13_RdFL(_r)			((pstLinkShadowReg->_r)=(pstLinkReg->_r))

#define LINK_H13_WrFL(_r)			((pstLinkReg->_r)=(pstLinkShadowReg->_r))

#define LINK_H13_Rd(_r)				*((UINT32*)(&(pstLinkShadowReg->_r)))
#define LINK_H13_Wr(_r,_v)			((LINK_H13_Rd(_r))=((UINT32)(_v)))


								
#define LINK_H13_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
								} while(0)

#define LINK_H13_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
									(_v02) = (pstLinkShadowReg->_r._f02);		\
								} while(0)

#define LINK_H13_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
								do { 											\
									(_v01) = (pstLinkShadowReg->_r._f01);		\
									(_v02) = (pstLinkShadowReg->_r._f02);		\
									(_v03) = (pstLinkShadowReg->_r._f03);		\
								} while(0)

#define LINK_H13_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pstLinkShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK_H13_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pstLinkShadowReg->_r._f01) = (_v01);		\
									(pstLinkShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define LINK_H13_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)							\
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
		PHY_REG_H13_T	*h13;
//		PHY_REG_H13Bx_T	*h13bx;
	} shdw;
} PHY_REG_CTRL_H13_T;



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PHY_REG_CTRL_H13_T	gPHY_REG_CTRL_H13;


#endif
