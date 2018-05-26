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
#ifndef __HDMI_REG_H15_H__
#define __HDMI_REG_H15_H__


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "link_top_reg_h15a0.h"
#include "link_0_reg_h15a0.h"
#include "link_1_reg_h15a0.h"
#include "phy_reg_h15a0.h"
#include "phy_reg_h15b0.h"
#include "hdcp22_prt0_reg_h15b0.h"
#include "hdcp22_prt1_reg_h15b0.h"

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PHY_REG_H15_I2C_IDX			(1)

#define PHY_REG_H15A0_SHDW		(gPHY_REG_CTRL_H15A0.shdw.H15)
#define PHY_REG_H15B0_SHDW		(gPHY_REG_CTRL_H15B0.shdw.H15)


/* H15 COMMON */
#define PHY_REG_RdFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_RdFL(PHY_REG_H15_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  PHY_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define PHY_REG_WrFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_WrFL(PHY_REG_H15_I2C_IDX,				\
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



/* H15 */
#define PHY_REG_H15A0_RdFL(_r)		PHY_REG_RdFL(H15A0,_r)
#define PHY_REG_H15A0_WrFL(_r)		PHY_REG_WrFL(H15A0,_r)

#define PHY_REG_H15A0_RdFd(_r,_f01)					PHY_REG_RdFd(H15A0,_r,_f01)

#define PHY_REG_H15A0_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(H15A0,_r,_f01,_v01)
#define PHY_REG_H15A0_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(H15A0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H15A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(H15A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_H15A0_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(H15A0,_r,_f01,_v01)
#define PHY_REG_H15A0_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(H15A0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H15A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(H15A0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_H15B0_RdFL(_r)		PHY_REG_RdFL(H15B0,_r)
#define PHY_REG_H15B0_WrFL(_r)		PHY_REG_WrFL(H15B0,_r)

#define PHY_REG_H15B0_RdFd(_r,_f01)					PHY_REG_RdFd(H15B0,_r,_f01)

#define PHY_REG_H15B0_Rd01(_r,_f01,_v01)			PHY_REG_Rd01(H15B0,_r,_f01,_v01)
#define PHY_REG_H15B0_Rd02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Rd02(H15B0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H15B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Rd03(H15B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define PHY_REG_H15B0_Wr01(_r,_f01,_v01)			PHY_REG_Wr01(H15B0,_r,_f01,_v01)
#define PHY_REG_H15B0_Wr02(_r,_f01,_v01,_f02,_v02)	PHY_REG_Wr02(H15B0,_r,_f01,_v01,_f02,_v02)
#define PHY_REG_H15B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	PHY_REG_Wr03(H15B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

/* LINK0, LINK1 */
#define LINK_NUM_H15_RdFL(_m,_r)								\
	do { 														\
		if(_m == 0) 											\
			((pst_LINK0_ShadowReg->_r)=(pst_LINK0_Reg->_r)); 	\
		else													\
			((pst_LINK1_ShadowReg->_r)=(pst_LINK1_Reg->_r)); 	\
	} while (0)

#define LINK_NUM_H15_WrFL(_m,_r)								\
	do { 														\
		if(_m == 0) 											\
			((pst_LINK0_Reg->_r)=(pst_LINK0_ShadowReg->_r));	\
		else													\
			((pst_LINK1_Reg->_r)=(pst_LINK1_ShadowReg->_r));	\
	} while (0)


/*
#define LINK_NUM_H15_Rd(_m,_r,_v)	\
	do { 														\
		if(_m == 0) 											\
			(_v) = *((UINT32*)(&(pst_LINK0_ShadowReg->_r))); \
		else													\
			(_v) = *((UINT32*)(&(pst_LINK1_ShadowReg->_r))); \
	} while (0)
	*/
#define LINK_NUM_H15_Rd(_m,_r)	\
	( { 														\
	  UINT32 v = 0; \
		if(_m == 0) 											\
			v = *((UINT32*)(&(pst_LINK0_ShadowReg->_r))); \
		else													\
			v = *((UINT32*)(&(pst_LINK1_ShadowReg->_r))); \
	  v;\
	} )

#define LINK_NUM_H15_Wr(_m,_r,_v)	\
	do { 														\
		if(_m == 0) 											\
			((LINK_H15_Rd(0 ,_r))=((UINT32)(_v))); \
		else													\
			((LINK_H15_Rd(1 ,_r))=((UINT32)(_v))); \
	} while (0)
		

#define LINK_NUM_H15_Rd_fld(_m,_r,_f01)							\
	( { 														\
	  UINT32 v = 0; \
		if(_m == 0) 											\
			v = (pst_LINK0_ShadowReg->_r._f01); 				\
		else													\
			v = (pst_LINK1_ShadowReg->_r._f01); 				\
	  v;\
	} )
								
#define LINK_NUM_H15_Rd01(_m,_r,_f01,_v01)												\
	do { 											\
		if(_m == 0) 											\
			(_v01) = (pst_LINK0_ShadowReg->_r._f01);		\
		else													\
			(_v01) = (pst_LINK1_ShadowReg->_r._f01);		\
	} while(0)

#define LINK_NUM_H15_Rd02(_m,_r,_f01,_v01,_f02,_v02)									\
	do { 											\
		if(_m == 0) 											\
		{	\
			(_v01) = (pst_LINK0_ShadowReg->_r._f01);		\
			(_v02) = (pst_LINK0_ShadowReg->_r._f02);		\
		}\
		else													\
		{	\
			(_v01) = (pst_LINK1_ShadowReg->_r._f01);		\
			(_v02) = (pst_LINK1_ShadowReg->_r._f02);		\
		}\
	} while(0)

#define LINK_NUM_H15_Wr01(_m,_r,_f01,_v01)												\
	do { 											\
		if(_m == 0) 											\
		(pst_LINK0_ShadowReg->_r._f01) = (_v01);		\
		else													\
		(pst_LINK1_ShadowReg->_r._f01) = (_v01);		\
	} while(0)

#define LINK_NUM_H15_Wr02(_m,_r,_f01,_v01,_f02,_v02)									\
	do { 											\
		if(_m == 0) 											\
		{	\
			(pst_LINK0_ShadowReg->_r._f01) = (_v01);		\
			(pst_LINK0_ShadowReg->_r._f02) = (_v02);		\
		}\
		else													\
		{	\
			(pst_LINK1_ShadowReg->_r._f01) = (_v01);		\
			(pst_LINK1_ShadowReg->_r._f02) = (_v02);		\
		}\
	} while(0)

/* LINK0, LINK1 */
#define LINK_H15_RdFL(_m,_r)		((pst_LINK##_m##_ShadowReg->_r)=(pst_LINK##_m##_Reg->_r))

#define LINK_H15_WrFL(_m,_r)			((pst_LINK##_m##_Reg->_r)=(pst_LINK##_m##_ShadowReg->_r))

#define LINK_H15_Rd(_m,_r)				*((UINT32*)(&(pst_LINK##_m##_ShadowReg->_r)))
#define LINK_H15_Wr(_m,_r,_v)			((LINK_H15_Rd(_m,_r))=((UINT32)(_v)))

#define LINK_H15_Rd_fld(_m,_r,_f01)	(pst_LINK##_m##_ShadowReg->_r._f01)

								
#define LINK_H15_Rd01(_m,_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_LINK##_m##_ShadowReg->_r._f01);		\
								} while(0)

#define LINK_H15_Rd02(_m,_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_LINK##_m##_ShadowReg->_r._f01);		\
									(_v02) = (pst_LINK##_m##_ShadowReg->_r._f02);		\
								} while(0)

#define LINK_H15_Wr01(_m,_r,_f01,_v01)												\
								do { 											\
									(pst_LINK##_m##_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK_H15_Wr02(_m,_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_LINK##_m##_ShadowReg->_r._f01) = (_v01);		\
									(pst_LINK##_m##_ShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define LINK0_H15_RdFL(_r)			((pst_LINK0_ShadowReg->_r)=(pst_LINK0_Reg->_r))

#define LINK0_H15_WrFL(_r)			((pst_LINK0_Reg->_r)=(pst_LINK0_ShadowReg->_r))

#define LINK0_H15_Rd(_r)				*((UINT32*)(&(pst_LINK0_ShadowReg->_r)))
#define LINK0_H15_Wr(_r,_v)			((LINK0_H15_Rd(_r))=((UINT32)(_v)))

#define LINK0_H15_Rd_fld(_r,_f01)	(pst_LINK0_ShadowReg->_r._f01)

								
#define LINK0_H15_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_LINK0_ShadowReg->_r._f01);		\
								} while(0)

#define LINK0_H15_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_LINK0_ShadowReg->_r._f01);		\
									(_v02) = (pst_LINK0_ShadowReg->_r._f02);		\
								} while(0)

#define LINK0_H15_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pst_LINK0_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK0_H15_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_LINK0_ShadowReg->_r._f01) = (_v01);		\
									(pst_LINK0_ShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define LINK1_H15_RdFL(_r)			((pst_LINK1_ShadowReg->_r)=(pst_LINK1_Reg->_r))

#define LINK1_H15_WrFL(_r)			((pst_LINK1_Reg->_r)=(pst_LINK1_ShadowReg->_r))

#define LINK1_H15_Rd(_r)				*((UINT32*)(&(pst_LINK1_ShadowReg->_r)))
#define LINK1_H15_Wr(_r,_v)			((LINK1_H15_Rd(_r))=((UINT32)(_v)))

#define LINK1_H15_Rd_fld(_r,_f01)	(pst_LINK1_ShadowReg->_r._f01)

								
#define LINK1_H15_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_LINK1_ShadowReg->_r._f01);		\
								} while(0)

#define LINK1_H15_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_LINK1_ShadowReg->_r._f01);		\
									(_v02) = (pst_LINK1_ShadowReg->_r._f02);		\
								} while(0)

#define LINK1_H15_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pst_LINK1_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK1_H15_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_LINK1_ShadowReg->_r._f01) = (_v01);		\
									(pst_LINK1_ShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define LINK_TOP_H15_RdFL(_r)			((pst_LINK_TOP_ShadowReg->_r)=(pst_LINK_TOP_Reg->_r))

#define LINK_TOP_H15_WrFL(_r)			((pst_LINK_TOP_Reg->_r)=(pst_LINK_TOP_ShadowReg->_r))

#define LINK_TOP_H15_Rd(_r)				*((UINT32*)(&(pst_LINK_TOP_ShadowReg->_r)))
#define LINK_TOP_H15_Wr(_r,_v)			((LINK_TOP_H15_Rd(_r))=((UINT32)(_v)))

#define LINK_TOP_H15_Rd_fld(_r,_f01)	(pst_LINK_TOP_ShadowReg->_r._f01)

								
#define LINK_TOP_H15_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_LINK_TOP_ShadowReg->_r._f01);		\
								} while(0)

#define LINK_TOP_H15_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_LINK_TOP_ShadowReg->_r._f01);		\
									(_v02) = (pst_LINK_TOP_ShadowReg->_r._f02);		\
								} while(0)

#define LINK_TOP_H15_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pst_LINK_TOP_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define LINK_TOP_H15_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_LINK_TOP_ShadowReg->_r._f01) = (_v01);		\
									(pst_LINK_TOP_ShadowReg->_r._f02) = (_v02);		\
								} while(0)


/* HDCP22_Prt0, HDCP22_Prt1 */
#define HDCP22_NUM_H15_RdFL(_m,_r)								\
	do { 														\
		if(_m == 0) 											\
			((pst_HDCP22_Prt0_ShadowReg->_r)=(pst_HDCP22_Prt0_Reg->_r)); 	\
		else													\
			((pst_HDCP22_Prt1_ShadowReg->_r)=(pst_HDCP22_Prt1_Reg->_r)); 	\
	} while (0)

#define HDCP22_NUM_H15_WrFL(_m,_r)								\
	do { 														\
		if(_m == 0) 											\
			((pst_HDCP22_Prt0_Reg->_r)=(pst_HDCP22_Prt0_ShadowReg->_r));	\
		else													\
			((pst_HDCP22_Prt1_Reg->_r)=(pst_HDCP22_Prt1_ShadowReg->_r));	\
	} while (0)


/*
#define HDCP22_NUM_H15_Rd(_m,_r,_v)	\
	do { 														\
		if(_m == 0) 											\
			(_v) = *((UINT32*)(&(pst_HDCP22_Prt0_ShadowReg->_r))); \
		else													\
			(_v) = *((UINT32*)(&(pst_HDCP22_Prt1_ShadowReg->_r))); \
	} while (0)
	*/
#define HDCP22_NUM_H15_Rd(_m,_r)	\
	( { 														\
	  UINT32 v = 0; \
		if(_m == 0) 											\
			v = *((UINT32*)(&(pst_HDCP22_Prt0_ShadowReg->_r))); \
		else													\
			v = *((UINT32*)(&(pst_HDCP22_Prt1_ShadowReg->_r))); \
	  v;\
	} )

#define HDCP22_NUM_H15_Wr(_m,_r,_v)	\
	do { 														\
		if(_m == 0) 											\
			((HDCP22_H15_Rd(0 ,_r))=((UINT32)(_v))); \
		else													\
			((HDCP22_H15_Rd(1 ,_r))=((UINT32)(_v))); \
	} while (0)
		

#define HDCP22_NUM_H15_Rd_fld(_m,_r,_f01)							\
	( { 														\
	  UINT32 v = 0; \
		if(_m == 0) 											\
			v = (pst_HDCP22_Prt0_ShadowReg->_r._f01); 				\
		else													\
			v = (pst_HDCP22_Prt1_ShadowReg->_r._f01); 				\
	  v;\
	} )
								
#define HDCP22_NUM_H15_Rd01(_m,_r,_f01,_v01)												\
	do { 											\
		if(_m == 0) 											\
			(_v01) = (pst_HDCP22_Prt0_ShadowReg->_r._f01);		\
		else													\
			(_v01) = (pst_HDCP22_Prt1_ShadowReg->_r._f01);		\
	} while(0)

#define HDCP22_NUM_H15_Rd02(_m,_r,_f01,_v01,_f02,_v02)									\
	do { 											\
		if(_m == 0) 											\
		{	\
			(_v01) = (pst_HDCP22_Prt0_ShadowReg->_r._f01);		\
			(_v02) = (pst_HDCP22_Prt0_ShadowReg->_r._f02);		\
		}\
		else													\
		{	\
			(_v01) = (pst_HDCP22_Prt1_ShadowReg->_r._f01);		\
			(_v02) = (pst_HDCP22_Prt1_ShadowReg->_r._f02);		\
		}\
	} while(0)

#define HDCP22_NUM_H15_Wr01(_m,_r,_f01,_v01)												\
	do { 											\
		if(_m == 0) 											\
		(pst_HDCP22_Prt0_ShadowReg->_r._f01) = (_v01);		\
		else													\
		(pst_HDCP22_Prt1_ShadowReg->_r._f01) = (_v01);		\
	} while(0)

#define HDCP22_NUM_H15_Wr02(_m,_r,_f01,_v01,_f02,_v02)									\
	do { 											\
		if(_m == 0) 											\
		{	\
			(pst_HDCP22_Prt0_ShadowReg->_r._f01) = (_v01);		\
			(pst_HDCP22_Prt0_ShadowReg->_r._f02) = (_v02);		\
		}\
		else													\
		{	\
			(pst_HDCP22_Prt1_ShadowReg->_r._f01) = (_v01);		\
			(pst_HDCP22_Prt1_ShadowReg->_r._f02) = (_v02);		\
		}\
	} while(0)

/* HDCP22_Prt0, HDCP22_Prt */
#define HDCP22_H15_RdFL(_m,_r)		((pst_HDCP22_Prt##_m##_ShadowReg->_r)=(pst_HDCP22_Prt##_m##_Reg->_r))

#define HDCP22_H15_WrFL(_m,_r)			((pst_HDCP22_Prt##_m##_Reg->_r)=(pst_HDCP22_Prt##_m##_ShadowReg->_r))

#define HDCP22_H15_Rd(_m,_r)				*((UINT32*)(&(pst_HDCP22_Prt##_m##_ShadowReg->_r)))
#define HDCP22_H15_Wr(_m,_r,_v)			((HDCP22_H15_Rd(_m,_r))=((UINT32)(_v)))

#define HDCP22_H15_Rd_fld(_m,_r,_f01)	(pst_HDCP22_Prt##_m##_ShadowReg->_r._f01)

								
#define HDCP22_H15_Rd01(_m,_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_HDCP22_Prt##_m##_ShadowReg->_r._f01);		\
								} while(0)

#define HDCP22_H15_Rd02(_m,_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_HDCP22_Prt##_m##_ShadowReg->_r._f01);		\
									(_v02) = (pst_HDCP22_Prt##_m##_ShadowReg->_r._f02);		\
								} while(0)

#define HDCP22_H15_Wr01(_m,_r,_f01,_v01)												\
								do { 											\
									(pst_HDCP22_Prt##_m##_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define HDCP22_H15_Wr02(_m,_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_HDCP22_Prt##_m##_ShadowReg->_r._f01) = (_v01);		\
									(pst_HDCP22_Prt##_m##_ShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define HDCP22_Prt0_H15_RdFL(_r)			((pst_HDCP22_Prt0_ShadowReg->_r)=(pst_HDCP22_Prt0_Reg->_r))

#define HDCP22_Prt0_H15_WrFL(_r)			((pst_HDCP22_Prt0_Reg->_r)=(pst_HDCP22_Prt0_ShadowReg->_r))

#define HDCP22_Prt0_H15_Rd(_r)				*((UINT32*)(&(pst_HDCP22_Prt0_ShadowReg->_r)))
#define HDCP22_Prt0_H15_Wr(_r,_v)			((HDCP22_Prt0_H15_Rd(_r))=((UINT32)(_v)))

#define HDCP22_Prt0_H15_Rd_fld(_r,_f01)	(pst_HDCP22_Prt0_ShadowReg->_r._f01)

								
#define HDCP22_Prt0_H15_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_HDCP22_Prt0_ShadowReg->_r._f01);		\
								} while(0)

#define HDCP22_Prt0_H15_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_HDCP22_Prt0_ShadowReg->_r._f01);		\
									(_v02) = (pst_HDCP22_Prt0_ShadowReg->_r._f02);		\
								} while(0)

#define HDCP22_Prt0_H15_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pst_HDCP22_Prt0_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define HDCP22_Prt0_H15_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_HDCP22_Prt0_ShadowReg->_r._f01) = (_v01);		\
									(pst_HDCP22_Prt0_ShadowReg->_r._f02) = (_v02);		\
								} while(0)

#define HDCP22_Prt1_H15_RdFL(_r)			((pst_HDCP22_Prt1_ShadowReg->_r)=(pst_HDCP22_Prt1_Reg->_r))

#define HDCP22_Prt1_H15_WrFL(_r)			((pst_HDCP22_Prt1_Reg->_r)=(pst_HDCP22_Prt1_ShadowReg->_r))

#define HDCP22_Prt1_H15_Rd(_r)				*((UINT32*)(&(pst_HDCP22_Prt1_ShadowReg->_r)))
#define HDCP22_Prt1_H15_Wr(_r,_v)			((HDCP22_Prt1_H15_Rd(_r))=((UINT32)(_v)))

#define HDCP22_Prt1_H15_Rd_fld(_r,_f01)	(pst_HDCP22_Prt1_ShadowReg->_r._f01)

								
#define HDCP22_Prt1_H15_Rd01(_r,_f01,_v01)												\
								do { 											\
									(_v01) = (pst_HDCP22_Prt1_ShadowReg->_r._f01);		\
								} while(0)

#define HDCP22_Prt1_H15_Rd02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(_v01) = (pst_HDCP22_Prt1_ShadowReg->_r._f01);		\
									(_v02) = (pst_HDCP22_Prt1_ShadowReg->_r._f02);		\
								} while(0)

#define HDCP22_Prt1_H15_Wr01(_r,_f01,_v01)												\
								do { 											\
									(pst_HDCP22_Prt1_ShadowReg->_r._f01) = (_v01);		\
								} while(0)

#define HDCP22_Prt1_H15_Wr02(_r,_f01,_v01,_f02,_v02)									\
								do { 											\
									(pst_HDCP22_Prt1_ShadowReg->_r._f01) = (_v01);		\
									(pst_HDCP22_Prt1_ShadowReg->_r._f02) = (_v02);		\
								} while(0)
/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	union {
		UINT32          *addr;
		PHY_REG_H15A0_T	*H15;
//		PHY_REG_H14Bx_T	*H14bx;
	} shdw;
} PHY_REG_CTRL_H15A0_T;

typedef struct
{
	union {
		UINT32          *addr;
		PHY_REG_H15B0_T	*H15;
//		PHY_REG_H14Bx_T	*H14bx;
	} shdw;
} PHY_REG_CTRL_H15B0_T;

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PHY_REG_CTRL_H15A0_T	gPHY_REG_CTRL_H15A0;
extern PHY_REG_CTRL_H15B0_T	gPHY_REG_CTRL_H15B0;


#endif
