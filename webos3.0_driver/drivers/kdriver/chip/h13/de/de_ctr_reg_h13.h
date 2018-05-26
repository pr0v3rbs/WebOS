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
#ifndef  DE_CTRL_REG_H13_INC
#define  DE_CTRL_REG_H13_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H13A0
#include "de_ctr_reg_h13a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13B0
#include "de_ctr_reg_h13b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_DEA_H13_RdFL(_r)								FN_CHIP_RdFL(DE_DEA, H13, _r)
#define DE_DEA_H13_WrFL(_r)								FN_CHIP_WrFL(DE_DEA, H13, _r)
#define DE_DEA_H13_Rd(_r)								FN_CHIP_Rd32(DE_DEA, H13, _r)
#define DE_DEA_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEA, H13, _r, _v)
#define DE_DEA_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEA, H13, _r, _f01, _v01)
#define DE_DEA_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEA, H13, _r, _f01, _v01)
#define DE_DEA_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEA, H13, _r, _v)
#define DE_DEA_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEA, H13, _r, _v)
#define DE_DEA_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEA, H13, _r, _f01, _v01)
#define DE_DEA_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEA, H13, _r, _f01, _v01)

#define DE_DEB_H13_RdFL(_r)								FN_CHIP_RdFL(DE_DEB, H13, _r)
#define DE_DEB_H13_WrFL(_r)								FN_CHIP_WrFL(DE_DEB, H13, _r)
#define DE_DEB_H13_Rd(_r)								FN_CHIP_Rd32(DE_DEB, H13, _r)
#define DE_DEB_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEB, H13, _r, _v)
#define DE_DEB_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEB, H13, _r, _f01, _v01)
#define DE_DEB_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEB, H13, _r, _f01, _v01)
#define DE_DEB_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEB, H13, _r, _v)
#define DE_DEB_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEB, H13, _r, _v)
#define DE_DEB_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEB, H13, _r, _f01, _v01)
#define DE_DEB_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEB, H13, _r, _f01, _v01)

#define DE_DEC_H13_RdFL(_r)								FN_CHIP_RdFL(DE_DEC, H13, _r)
#define DE_DEC_H13_WrFL(_r)								FN_CHIP_WrFL(DE_DEC, H13, _r)
#define DE_DEC_H13_Rd(_r)								FN_CHIP_Rd32(DE_DEC, H13, _r)
#define DE_DEC_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEC, H13, _r, _v)
#define DE_DEC_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEC, H13, _r, _f01, _v01)
#define DE_DEC_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEC, H13, _r, _f01, _v01)
#define DE_DEC_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEC, H13, _r, _v)
#define DE_DEC_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEC, H13, _r, _v)
#define DE_DEC_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEC, H13, _r, _f01, _v01)
#define DE_DEC_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEC, H13, _r, _f01, _v01)

#define DE_DED_H13_RdFL(_r)								FN_CHIP_RdFL(DE_DED, H13, _r)
#define DE_DED_H13_WrFL(_r)								FN_CHIP_WrFL(DE_DED, H13, _r)
#define DE_DED_H13_Rd(_r)								FN_CHIP_Rd32(DE_DED, H13, _r)
#define DE_DED_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_DED, H13, _r, _v)
#define DE_DED_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DED, H13, _r, _f01, _v01)
#define DE_DED_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DED, H13, _r, _f01, _v01)
#define DE_DED_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DED, H13, _r, _v)
#define DE_DED_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DED, H13, _r, _v)
#define DE_DED_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DED, H13, _r, _f01, _v01)
#define DE_DED_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DED, H13, _r, _f01, _v01)

#define DE_DEE_H13_RdFL(_r)								FN_CHIP_RdFL(DE_DEE, H13, _r)
#define DE_DEE_H13_WrFL(_r)								FN_CHIP_WrFL(DE_DEE, H13, _r)
#define DE_DEE_H13_Rd(_r)								FN_CHIP_Rd32(DE_DEE, H13, _r)
#define DE_DEE_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEE, H13, _r, _v)
#define DE_DEE_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEE, H13, _r, _f01, _v01)
#define DE_DEE_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEE, H13, _r, _f01, _v01)
#define DE_DEE_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEE, H13, _r, _v)
#define DE_DEE_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEE, H13, _r, _v)
#define DE_DEE_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEE, H13, _r, _f01, _v01)
#define DE_DEE_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEE, H13, _r, _f01, _v01)

#define DE_DEA_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEA##_##H13, a0, _r)
#define DE_DEA_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEA##_##H13, a0, _r)
#define DE_DEA_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEA##_##H13, a0, _r)
#define DE_DEA_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEA##_##H13, a0, _r, _v)
#define DE_DEA_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEA##_##H13, a0, _r, _f01, _v01)
#define DE_DEA_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEA##_##H13, a0, _r, _f01, _v01)
#define DE_DEA_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEA##_##H13, a0, _r, _f01, _v01)
#define DE_DEA_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEA##_##H13, a0, _r, _f01, _v01)
#define DE_DEA_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEA##_##H13, a0, _r, _f, _c, _m)
#define DE_DEA_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEA##_##H13, a0, _r, _f, _c, _m)
#define DE_DEA_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEA##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DEA_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEA##_##H13, a0, _r, _v)
#define DE_DEA_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEA##_##H13, a0, _r, _v)
#define DE_DEA_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEA##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DEB_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEB##_##H13, a0, _r)
#define DE_DEB_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEB##_##H13, a0, _r)
#define DE_DEB_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEB##_##H13, a0, _r)
#define DE_DEB_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEB##_##H13, a0, _r, _v)
#define DE_DEB_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEB##_##H13, a0, _r, _f01, _v01)
#define DE_DEB_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEB##_##H13, a0, _r, _f01, _v01)
#define DE_DEB_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEB##_##H13, a0, _r, _f01, _v01)
#define DE_DEB_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEB##_##H13, a0, _r, _f01, _v01)
#define DE_DEB_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEB##_##H13, a0, _r, _f, _c, _m)
#define DE_DEB_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEB##_##H13, a0, _r, _f, _c, _m)
#define DE_DEB_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEB##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DEB_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEB##_##H13, a0, _r, _v)
#define DE_DEB_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEB##_##H13, a0, _r, _v)
#define DE_DEB_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEB##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DEC_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEC##_##H13, a0, _r)
#define DE_DEC_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEC##_##H13, a0, _r)
#define DE_DEC_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEC##_##H13, a0, _r)
#define DE_DEC_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEC##_##H13, a0, _r, _v)
#define DE_DEC_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEC##_##H13, a0, _r, _f01, _v01)
#define DE_DEC_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEC##_##H13, a0, _r, _f01, _v01)
#define DE_DEC_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEC##_##H13, a0, _r, _f01, _v01)
#define DE_DEC_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEC##_##H13, a0, _r, _f01, _v01)
#define DE_DEC_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEC##_##H13, a0, _r, _f, _c, _m)
#define DE_DEC_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEC##_##H13, a0, _r, _f, _c, _m)
#define DE_DEC_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEC##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DEC_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEC##_##H13, a0, _r, _v)
#define DE_DEC_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEC##_##H13, a0, _r, _v)
#define DE_DEC_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEC##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DED_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DED##_##H13, a0, _r)
#define DE_DED_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DED##_##H13, a0, _r)
#define DE_DED_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DED##_##H13, a0, _r)
#define DE_DED_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DED##_##H13, a0, _r, _v)
#define DE_DED_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DED##_##H13, a0, _r, _f01, _v01)
#define DE_DED_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DED##_##H13, a0, _r, _f01, _v01)
#define DE_DED_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DED##_##H13, a0, _r, _f01, _v01)
#define DE_DED_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DED##_##H13, a0, _r, _f01, _v01)
#define DE_DED_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DED##_##H13, a0, _r, _f, _c, _m)
#define DE_DED_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DED##_##H13, a0, _r, _f, _c, _m)
#define DE_DED_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DED##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DED_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DED##_##H13, a0, _r, _v)
#define DE_DED_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DED##_##H13, a0, _r, _v)
#define DE_DED_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DED##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DEE_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEE##_##H13, a0, _r)
#define DE_DEE_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEE##_##H13, a0, _r)
#define DE_DEE_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEE##_##H13, a0, _r)
#define DE_DEE_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEE##_##H13, a0, _r, _v)
#define DE_DEE_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEE##_##H13, a0, _r, _f01, _v01)
#define DE_DEE_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEE##_##H13, a0, _r, _f01, _v01)
#define DE_DEE_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEE##_##H13, a0, _r, _f01, _v01)
#define DE_DEE_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEE##_##H13, a0, _r, _f01, _v01)
#define DE_DEE_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEE##_##H13, a0, _r, _f, _c, _m)
#define DE_DEE_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEE##_##H13, a0, _r, _f, _c, _m)
#define DE_DEE_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEE##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DEE_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEE##_##H13, a0, _r, _v)
#define DE_DEE_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEE##_##H13, a0, _r, _v)
#define DE_DEE_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEE##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DEF_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEF##_##H13, a0, _r)
#define DE_DEF_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEF##_##H13, a0, _r)
#define DE_DEF_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEF##_##H13, a0, _r)
#define DE_DEF_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEF##_##H13, a0, _r, _v)
#define DE_DEF_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEF##_##H13, a0, _r, _f01, _v01)
#define DE_DEF_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEF##_##H13, a0, _r, _f01, _v01)
#define DE_DEF_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEF##_##H13, a0, _r, _f01, _v01)
#define DE_DEF_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEF##_##H13, a0, _r, _f01, _v01)
#define DE_DEF_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEF##_##H13, a0, _r, _f, _c, _m)
#define DE_DEF_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEF##_##H13, a0, _r, _f, _c, _m)
#define DE_DEF_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEF##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_DEF_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEF##_##H13, a0, _r, _v)
#define DE_DEF_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEF##_##H13, a0, _r, _v)
#define DE_DEF_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEF##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_DEA_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEA##_##H13, b0, _r)
#define DE_DEA_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEA##_##H13, b0, _r)
#define DE_DEA_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEA##_##H13, b0, _r)
#define DE_DEA_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEA##_##H13, b0, _r, _v)
#define DE_DEA_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEA##_##H13, b0, _r, _f01, _v01)
#define DE_DEA_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEA##_##H13, b0, _r, _f01, _v01)
#define DE_DEA_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEA##_##H13, b0, _r, _f01, _v01)
#define DE_DEA_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEA##_##H13, b0, _r, _f01, _v01)
#define DE_DEA_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEA##_##H13, b0, _r, _f, _c, _m)
#define DE_DEA_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEA##_##H13, b0, _r, _f, _c, _m)
#define DE_DEA_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEA##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DEA_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEA##_##H13, b0, _r, _v)
#define DE_DEA_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEA##_##H13, b0, _r, _v)
#define DE_DEA_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEA##_##H13, b0, _r, _f, _c, _v1, _v2)

#define DE_DEB_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEB##_##H13, b0, _r)
#define DE_DEB_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEB##_##H13, b0, _r)
#define DE_DEB_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEB##_##H13, b0, _r)
#define DE_DEB_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEB##_##H13, b0, _r, _v)
#define DE_DEB_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEB##_##H13, b0, _r, _f01, _v01)
#define DE_DEB_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEB##_##H13, b0, _r, _f01, _v01)
#define DE_DEB_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEB##_##H13, b0, _r, _f01, _v01)
#define DE_DEB_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEB##_##H13, b0, _r, _f01, _v01)
#define DE_DEB_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEB##_##H13, b0, _r, _f, _c, _m)
#define DE_DEB_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEB##_##H13, b0, _r, _f, _c, _m)
#define DE_DEB_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEB##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DEB_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEB##_##H13, b0, _r, _v)
#define DE_DEB_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEB##_##H13, b0, _r, _v)
#define DE_DEB_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEB##_##H13, b0, _r, _f, _c, _v1, _v2)

#define DE_DEC_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEC##_##H13, b0, _r)
#define DE_DEC_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEC##_##H13, b0, _r)
#define DE_DEC_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEC##_##H13, b0, _r)
#define DE_DEC_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEC##_##H13, b0, _r, _v)
#define DE_DEC_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEC##_##H13, b0, _r, _f01, _v01)
#define DE_DEC_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEC##_##H13, b0, _r, _f01, _v01)
#define DE_DEC_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEC##_##H13, b0, _r, _f01, _v01)
#define DE_DEC_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEC##_##H13, b0, _r, _f01, _v01)
#define DE_DEC_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEC##_##H13, b0, _r, _f, _c, _m)
#define DE_DEC_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEC##_##H13, b0, _r, _f, _c, _m)
#define DE_DEC_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEC##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DEC_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEC##_##H13, b0, _r, _v)
#define DE_DEC_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEC##_##H13, b0, _r, _v)
#define DE_DEC_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEC##_##H13, b0, _r, _f, _c, _v1, _v2)

#define DE_DED_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DED##_##H13, b0, _r)
#define DE_DED_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DED##_##H13, b0, _r)
#define DE_DED_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DED##_##H13, b0, _r)
#define DE_DED_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DED##_##H13, b0, _r, _v)
#define DE_DED_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DED##_##H13, b0, _r, _f01, _v01)
#define DE_DED_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DED##_##H13, b0, _r, _f01, _v01)
#define DE_DED_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DED##_##H13, b0, _r, _f01, _v01)
#define DE_DED_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DED##_##H13, b0, _r, _f01, _v01)
#define DE_DED_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DED##_##H13, b0, _r, _f, _c, _m)
#define DE_DED_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DED##_##H13, b0, _r, _f, _c, _m)
#define DE_DED_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DED##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DED_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DED##_##H13, b0, _r, _v)
#define DE_DED_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DED##_##H13, b0, _r, _v)
#define DE_DED_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DED##_##H13, b0, _r, _f, _c, _v1, _v2)

#define DE_DEE_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEE##_##H13, b0, _r)
#define DE_DEE_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEE##_##H13, b0, _r)
#define DE_DEE_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEE##_##H13, b0, _r)
#define DE_DEE_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEE##_##H13, b0, _r, _v)
#define DE_DEE_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEE##_##H13, b0, _r, _f01, _v01)
#define DE_DEE_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEE##_##H13, b0, _r, _f01, _v01)
#define DE_DEE_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEE##_##H13, b0, _r, _f01, _v01)
#define DE_DEE_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEE##_##H13, b0, _r, _f01, _v01)
#define DE_DEE_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEE##_##H13, b0, _r, _f, _c, _m)
#define DE_DEE_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEE##_##H13, b0, _r, _f, _c, _m)
#define DE_DEE_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEE##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DEE_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEE##_##H13, b0, _r, _v)
#define DE_DEE_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEE##_##H13, b0, _r, _v)
#define DE_DEE_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEE##_##H13, b0, _r, _f, _c, _v1, _v2)

#define DE_DEF_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEF##_##H13, b0, _r)
#define DE_DEF_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEF##_##H13, b0, _r)
#define DE_DEF_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEF##_##H13, b0, _r)
#define DE_DEF_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEF##_##H13, b0, _r, _v)
#define DE_DEF_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEF##_##H13, b0, _r, _f01, _v01)
#define DE_DEF_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEF##_##H13, b0, _r, _f01, _v01)
#define DE_DEF_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEF##_##H13, b0, _r, _f01, _v01)
#define DE_DEF_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEF##_##H13, b0, _r, _f01, _v01)
#define DE_DEF_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEF##_##H13, b0, _r, _f, _c, _m)
#define DE_DEF_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEF##_##H13, b0, _r, _f, _c, _m)
#define DE_DEF_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEF##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_DEF_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEF##_##H13, b0, _r, _v)
#define DE_DEF_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEF##_##H13, b0, _r, _v)
#define DE_DEF_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEF##_##H13, b0, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_DEA_REG_H13A0_T *a0;
		DE_DEA_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEA_REG_H13A0_T *a0;
		volatile DE_DEA_REG_H13B0_T *b0;
	} phys;
} DE_DEA_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEB_REG_H13A0_T *a0;
		DE_DEB_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEB_REG_H13A0_T *a0;
		volatile DE_DEB_REG_H13B0_T *b0;
	} phys;
} DE_DEB_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEC_REG_H13A0_T *a0;
		DE_DEC_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEC_REG_H13A0_T *a0;
		volatile DE_DEC_REG_H13B0_T *b0;
	} phys;
} DE_DEC_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DED_REG_H13A0_T *a0;
		DE_DED_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DED_REG_H13A0_T *a0;
		volatile DE_DED_REG_H13B0_T *b0;
	} phys;
} DE_DED_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEE_REG_H13A0_T *a0;
		DE_DEE_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEE_REG_H13A0_T *a0;
		volatile DE_DEE_REG_H13B0_T *b0;
	} phys;
} DE_DEE_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEF_REG_H13A0_T *a0;
		DE_DEF_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEF_REG_H13A0_T *a0;
		volatile DE_DEF_REG_H13B0_T *b0;
	} phys;
} DE_DEF_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_CTRL_REG_H13_INC  ----- */
/**  @} */
