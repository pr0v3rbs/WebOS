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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2011.02.16
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_CTRL_REG_H14_INC
#define  DE_CTRL_REG_H14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H14A0
#include "de_ctr_reg_h14a0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_DEA_H14_RdFL(_r)								FN_CHIP_RdFL(DE_DEA, H14, _r)
#define DE_DEA_H14_WrFL(_r)							FN_CHIP_WrFL(DE_DEA, H14, _r)
#define DE_DEA_H14_Rd(_r)								FN_CHIP_Rd32(DE_DEA, H14, _r)
#define DE_DEA_H14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEA, H14, _r, _v)
#define DE_DEA_H14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEA, H14, _r, _f01, _v01)
#define DE_DEA_H14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEA, H14, _r, _f01, _v01)
#define DE_DEA_H14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEA, H14, _r, _v)
#define DE_DEA_H14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEA, H14, _r, _v)
#define DE_DEA_H14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEA, H14, _r, _f01, _v01)
#define DE_DEA_H14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEA, H14, _r, _f01, _v01)

#define DE_DEB_H14_RdFL(_r)								FN_CHIP_RdFL(DE_DEB, H14, _r)
#define DE_DEB_H14_WrFL(_r)							FN_CHIP_WrFL(DE_DEB, H14, _r)
#define DE_DEB_H14_Rd(_r)								FN_CHIP_Rd32(DE_DEB, H14, _r)
#define DE_DEB_H14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEB, H14, _r, _v)
#define DE_DEB_H14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEB, H14, _r, _f01, _v01)
#define DE_DEB_H14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEB, H14, _r, _f01, _v01)
#define DE_DEB_H14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEB, H14, _r, _v)
#define DE_DEB_H14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEB, H14, _r, _v)
#define DE_DEB_H14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEB, H14, _r, _f01, _v01)
#define DE_DEB_H14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEB, H14, _r, _f01, _v01)

#define DE_DEC_H14_RdFL(_r)								FN_CHIP_RdFL(DE_DEC, H14, _r)
#define DE_DEC_H14_WrFL(_r)							FN_CHIP_WrFL(DE_DEC, H14, _r)
#define DE_DEC_H14_Rd(_r)								FN_CHIP_Rd32(DE_DEC, H14, _r)
#define DE_DEC_H14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEC, H14, _r, _v)g
#define DE_DEC_H14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEC, H14, _r, _f01, _v01)
#define DE_DEC_H14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEC, H14, _r, _f01, _v01)
#define DE_DEC_H14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEC, H14, _r, _v)
#define DE_DEC_H14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEC, H14, _r, _v)
#define DE_DEC_H14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEC, H14, _r, _f01, _v01)
#define DE_DEC_H14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEC, H14, _r, _f01, _v01)

#define DE_DED_H14_RdFL(_r)								FN_CHIP_RdFL(DE_DED, H14, _r)
#define DE_DED_H14_WrFL(_r)							FN_CHIP_WrFL(DE_DED, H14, _r)
#define DE_DED_H14_Rd(_r)								FN_CHIP_Rd32(DE_DED, H14, _r)
#define DE_DED_H14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DED, H14, _r, _v)
#define DE_DED_H14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DED, H14, _r, _f01, _v01)
#define DE_DED_H14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DED, H14, _r, _f01, _v01)
#define DE_DED_H14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DED, H14, _r, _v)
#define DE_DED_H14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DED, H14, _r, _v)
#define DE_DED_H14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DED, H14, _r, _f01, _v01)
#define DE_DED_H14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DED, H14, _r, _f01, _v01)

#define DE_DEE_H14_RdFL(_r)								FN_CHIP_RdFL(DE_DEE, H14, _r)
#define DE_DEE_H14_WrFL(_r)							FN_CHIP_WrFL(DE_DEE, H14, _r)
#define DE_DEE_H14_Rd(_r)								FN_CHIP_Rd32(DE_DEE, H14, _r)
#define DE_DEE_H14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEE, H14, _r, _v)
#define DE_DEE_H14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEE, H14, _r, _f01, _v01)
#define DE_DEE_H14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEE, H14, _r, _f01, _v01)
#define DE_DEE_H14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEE, H14, _r, _v)
#define DE_DEE_H14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEE, H14, _r, _v)
#define DE_DEE_H14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEE, H14, _r, _f01, _v01)
#define DE_DEE_H14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEE, H14, _r, _f01, _v01)

#define DE_DEA_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEA##_##H14, a0, _r)
#define DE_DEA_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEA##_##H14, a0, _r)
#define DE_DEA_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEA##_##H14, a0, _r)
#define DE_DEA_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEA##_##H14, a0, _r, _v)
#define DE_DEA_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEA##_##H14, a0, _r, _f01, _v01)
#define DE_DEA_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEA##_##H14, a0, _r, _f01, _v01)
#define DE_DEA_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DEA##_##H14, a0, _r, _f01, _v01)
#define DE_DEA_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEA##_##H14, a0, _r, _f01, _v01)
#define DE_DEA_H14A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEA##_##H14, a0, _r, _f, _c, _m)
#define DE_DEA_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DEA##_##H14, a0, _r, _f, _c, _m)
#define DE_DEA_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEA##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DEA_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEA##_##H14, a0, _r, _v)
#define DE_DEA_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEA##_##H14, a0, _r, _v)
#define DE_DEA_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEA##_##H14, a0, _r, _f, _c, _v1, _v2)

#define DE_DEB_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEB##_##H14, a0, _r)
#define DE_DEB_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEB##_##H14, a0, _r)
#define DE_DEB_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEB##_##H14, a0, _r)
#define DE_DEB_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEB##_##H14, a0, _r, _v)
#define DE_DEB_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEB##_##H14, a0, _r, _f01, _v01)
#define DE_DEB_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEB##_##H14, a0, _r, _f01, _v01)
#define DE_DEB_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DEB##_##H14, a0, _r, _f01, _v01)
#define DE_DEB_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEB##_##H14, a0, _r, _f01, _v01)
#define DE_DEB_H14A_WfCM(_r, _f, _c, _m)					REG_SHDW_VER_WfCM(g##DE_DEB##_##H14, a0, _r, _f, _c, _m)
#define DE_DEB_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DEB##_##H14, a0, _r, _f, _c, _m)
#define DE_DEB_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEB##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DEB_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEB##_##H14, a0, _r, _v)
#define DE_DEB_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEB##_##H14, a0, _r, _v)
#define DE_DEB_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEB##_##H14, a0, _r, _f, _c, _v1, _v2)

#define DE_DEC_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEC##_##H14, a0, _r)
#define DE_DEC_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEC##_##H14, a0, _r)
#define DE_DEC_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEC##_##H14, a0, _r)
#define DE_DEC_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEC##_##H14, a0, _r, _v)
#define DE_DEC_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEC##_##H14, a0, _r, _f01, _v01)
#define DE_DEC_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEC##_##H14, a0, _r, _f01, _v01)
#define DE_DEC_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DEC##_##H14, a0, _r, _f01, _v01)
#define DE_DEC_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEC##_##H14, a0, _r, _f01, _v01)
#define DE_DEC_H14A_WfCM(_r, _f, _c, _m)					REG_SHDW_VER_WfCM(g##DE_DEC##_##H14, a0, _r, _f, _c, _m)
#define DE_DEC_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DEC##_##H14, a0, _r, _f, _c, _m)
#define DE_DEC_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEC##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DEC_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEC##_##H14, a0, _r, _v)
#define DE_DEC_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEC##_##H14, a0, _r, _v)
#define DE_DEC_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEC##_##H14, a0, _r, _f, _c, _v1, _v2)

#define DE_DED_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DED##_##H14, a0, _r)
#define DE_DED_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DED##_##H14, a0, _r)
#define DE_DED_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DED##_##H14, a0, _r)
#define DE_DED_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DED##_##H14, a0, _r, _v)
#define DE_DED_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DED##_##H14, a0, _r, _f01, _v01)
#define DE_DED_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DED##_##H14, a0, _r, _f01, _v01)
#define DE_DED_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DED##_##H14, a0, _r, _f01, _v01)
#define DE_DED_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DED##_##H14, a0, _r, _f01, _v01)
#define DE_DED_H14A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DED##_##H14, a0, _r, _f, _c, _m)
#define DE_DED_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DED##_##H14, a0, _r, _f, _c, _m)
#define DE_DED_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DED##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DED_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DED##_##H14, a0, _r, _v)
#define DE_DED_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DED##_##H14, a0, _r, _v)
#define DE_DED_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DED##_##H14, a0, _r, _f, _c, _v1, _v2)

#define DE_DEE_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEE##_##H14, a0, _r)
#define DE_DEE_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEE##_##H14, a0, _r)
#define DE_DEE_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEE##_##H14, a0, _r)
#define DE_DEE_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEE##_##H14, a0, _r, _v)
#define DE_DEE_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEE##_##H14, a0, _r, _f01, _v01)
#define DE_DEE_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEE##_##H14, a0, _r, _f01, _v01)
#define DE_DEE_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DEE##_##H14, a0, _r, _f01, _v01)
#define DE_DEE_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEE##_##H14, a0, _r, _f01, _v01)
#define DE_DEE_H14A_WfCM(_r, _f, _c, _m)					REG_SHDW_VER_WfCM(g##DE_DEE##_##H14, a0, _r, _f, _c, _m)
#define DE_DEE_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DEE##_##H14, a0, _r, _f, _c, _m)
#define DE_DEE_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEE##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DEE_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEE##_##H14, a0, _r, _v)
#define DE_DEE_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEE##_##H14, a0, _r, _v)
#define DE_DEE_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEE##_##H14, a0, _r, _f, _c, _v1, _v2)

#define DE_DEF_H14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEF##_##H14, a0, _r)
#define DE_DEF_H14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEF##_##H14, a0, _r)
#define DE_DEF_H14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEF##_##H14, a0, _r)
#define DE_DEF_H14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEF##_##H14, a0, _r, _v)
#define DE_DEF_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEF##_##H14, a0, _r, _f01, _v01)
#define DE_DEF_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEF##_##H14, a0, _r, _f01, _v01)
#define DE_DEF_H14A_FLRf(_r, _f01, _v01)					REG_PHYS_VER_FLRf(g##DE_DEF##_##H14, a0, _r, _f01, _v01)
#define DE_DEF_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEF##_##H14, a0, _r, _f01, _v01)
#define DE_DEF_H14A_WfCM(_r, _f, _c, _m)					REG_SHDW_VER_WfCM(g##DE_DEF##_##H14, a0, _r, _f, _c, _m)
#define DE_DEF_H14A_FLCM(_r, _f, _c, _m)					REG_PHYS_VER_FLCM(g##DE_DEF##_##H14, a0, _r, _f, _c, _m)
#define DE_DEF_H14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEF##_##H14, a0, _r, _f, _c, _v1, _v2)
#define DE_DEF_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEF##_##H14, a0, _r, _v)
#define DE_DEF_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEF##_##H14, a0, _r, _v)
#define DE_DEF_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEF##_##H14, a0, _r, _f, _c, _v1, _v2)
 

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_DEA_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEA_REG_H14A0_T *a0;
	} phys;
} DE_DEA_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEB_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEB_REG_H14A0_T *a0;
	} phys;
} DE_DEB_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEC_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEC_REG_H14A0_T *a0;
	} phys;
} DE_DEC_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DED_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DED_REG_H14A0_T *a0;
	} phys;
} DE_DED_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEE_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEE_REG_H14A0_T *a0;
	} phys;
} DE_DEE_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEF_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEF_REG_H14A0_T *a0;
	} phys;
} DE_DEF_REG_H14_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_CTRL_REG_H14_INC  ----- */
/**  @} */
