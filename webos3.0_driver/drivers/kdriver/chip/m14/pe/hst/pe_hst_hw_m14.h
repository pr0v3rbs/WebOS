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

/** @file pe_hst_hw_m14.h
 *
 *  driver header for picture enhance dynamic contrast module functions. ( used only within kdriver )
 *	- histogram information
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.07.04
 *	@see		
 */

#ifndef	_PE_HST_HW_M14_H_
#define	_PE_HST_HW_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_tsk_m14.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_HST_HW_M14_HST_BIN_NUM	32

/* [0]tb0,[1]ss0,[2]tb1,[3]ss1 */
#define PE_HST_HW_M14_LRC_INF_NUM	4

/* [0,1]y tb,ss,[2,3]cb,[4,5]cr, [6,7]g,[8,9]b,[10,11]r, [12,13]h,[14,15]s,[16,17]v */
#define PE_HST_HW_M14_SRC_INF_NUM	18

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe global histogram menu enumeration
 */
typedef enum
{
	PE_HST_HW_M14_MENU_DISPLAY = 0,			///< menu display
	PE_HST_HW_M14_MENU_SRC_APL_INF,			///< src apl info
	PE_HST_HW_M14_MENU_SRC_HST_INF,			///< src histo info
	PE_HST_HW_M14_MENU_LRC_HST_INF,			///< lrcr histo info
	PE_HST_HW_M14_MENU_CMN_HST_INF,			///< histo cmn info
	PE_HST_HW_M14_MENU_SRC_HST_Y_0,			///< src histo y0
	PE_HST_HW_M14_MENU_SRC_HST_Y_1,			///< src histo y1
	PE_HST_HW_M14_MENU_SRC_HST_CB0,			///< src histo cb0
	PE_HST_HW_M14_MENU_SRC_HST_CB1,			///< src histo cb1
	PE_HST_HW_M14_MENU_SRC_HST_CR0,			///< src histo cr0
	PE_HST_HW_M14_MENU_SRC_HST_CR1,			///< src histo cr1
	PE_HST_HW_M14_MENU_SRC_HST_G_0,			///< src histo g0
	PE_HST_HW_M14_MENU_SRC_HST_G_1,			///< src histo g1
	PE_HST_HW_M14_MENU_SRC_HST_B_0,			///< src histo b0
	PE_HST_HW_M14_MENU_SRC_HST_B_1,			///< src histo b1
	PE_HST_HW_M14_MENU_SRC_HST_R_0,			///< src histo r0
	PE_HST_HW_M14_MENU_SRC_HST_R_1,			///< src histo r1
	PE_HST_HW_M14_MENU_SRC_HST_H_0,			///< src histo h0
	PE_HST_HW_M14_MENU_SRC_HST_H_1,			///< src histo h1
	PE_HST_HW_M14_MENU_SRC_HST_S_0,			///< src histo s0
	PE_HST_HW_M14_MENU_SRC_HST_S_1,			///< src histo s1
	PE_HST_HW_M14_MENU_SRC_HST_V_0,			///< src histo v0
	PE_HST_HW_M14_MENU_SRC_HST_V_1,			///< src histo v1
	PE_HST_HW_M14_MENU_LRC_TB0_T_G,			///< lrcr histo tb0 t g
	PE_HST_HW_M14_MENU_LRC_TB0_B_G,			///< lrcr histo tb0 b g
	PE_HST_HW_M14_MENU_LRC_TB0_T_B,			///< lrcr histo tb0 t b
	PE_HST_HW_M14_MENU_LRC_TB0_B_B,			///< lrcr histo tb0 b b
	PE_HST_HW_M14_MENU_LRC_TB0_T_R,			///< lrcr histo tb0 t r
	PE_HST_HW_M14_MENU_LRC_TB0_B_R,			///< lrcr histo tb0 b r
	PE_HST_HW_M14_MENU_LRC_SS0_L_G,			///< lrcr histo ss0 l g
	PE_HST_HW_M14_MENU_LRC_SS0_R_G,			///< lrcr histo ss0 r g
	PE_HST_HW_M14_MENU_LRC_SS0_L_B,			///< lrcr histo ss0 l b
	PE_HST_HW_M14_MENU_LRC_SS0_R_B,			///< lrcr histo ss0 r b
	PE_HST_HW_M14_MENU_LRC_SS0_L_R,			///< lrcr histo ss0 l r
	PE_HST_HW_M14_MENU_LRC_SS0_R_R,			///< lrcr histo ss0 r r
	PE_HST_HW_M14_MENU_LRC_TB1_T_G,			///< lrcr histo tb1 t g
	PE_HST_HW_M14_MENU_LRC_TB1_B_G,			///< lrcr histo tb1 b g
	PE_HST_HW_M14_MENU_LRC_TB1_T_B,			///< lrcr histo tb1 t b
	PE_HST_HW_M14_MENU_LRC_TB1_B_B,			///< lrcr histo tb1 b b
	PE_HST_HW_M14_MENU_LRC_TB1_T_R,			///< lrcr histo tb1 t r
	PE_HST_HW_M14_MENU_LRC_TB1_B_R,			///< lrcr histo tb1 b r
	PE_HST_HW_M14_MENU_LRC_SS1_L_G,			///< lrcr histo ss1 l g
	PE_HST_HW_M14_MENU_LRC_SS1_R_G,			///< lrcr histo ss1 r g
	PE_HST_HW_M14_MENU_LRC_SS1_L_B,			///< lrcr histo ss1 l b
	PE_HST_HW_M14_MENU_LRC_SS1_R_B,			///< lrcr histo ss1 r b
	PE_HST_HW_M14_MENU_LRC_SS1_L_R,			///< lrcr histo ss1 l r
	PE_HST_HW_M14_MENU_LRC_SS1_R_R,			///< lrcr histo ss1 r r
	PE_HST_HW_M14_MENU_MAX					///< max num
}
PE_HST_HW_M14_GBL_HST_MENU;

/**
 *	pe histo src apl state machine enumeration
 */
typedef enum
{
	PE_HST_HW_M14_SRC_APL_STM_ALL_INIT = 0,		///< state init

	PE_HST_HW_M14_SRC_APL_STM_YUV_RDY0,			///< state ycbcr ready
	PE_HST_HW_M14_SRC_APL_STM_YUV_GET0,			///< state ycbcr get
	PE_HST_HW_M14_SRC_APL_STM_YUV_RDY1,			///< state ycbcr ready
	PE_HST_HW_M14_SRC_APL_STM_YUV_GET1,			///< state ycbcr get
	PE_HST_HW_M14_SRC_APL_STM_YUV_RDY2,			///< state ycbcr ready
	PE_HST_HW_M14_SRC_APL_STM_YUV_GET2,			///< state ycbcr get

	PE_HST_HW_M14_SRC_APL_STM_HSV_RDY0,			///< state hsv ready
	PE_HST_HW_M14_SRC_APL_STM_HSV_GET0,			///< state hsv get
	PE_HST_HW_M14_SRC_APL_STM_HSV_RDY1,			///< state hsv ready
	PE_HST_HW_M14_SRC_APL_STM_HSV_GET1,			///< state hsv get
	PE_HST_HW_M14_SRC_APL_STM_HSV_RDY2,			///< state hsv ready
	PE_HST_HW_M14_SRC_APL_STM_HSV_GET2,			///< state hsv get

	PE_HST_HW_M14_SRC_APL_STM_GBR_RDY0,			///< state gbr ready
	PE_HST_HW_M14_SRC_APL_STM_GBR_GET0,			///< state gbr get
	PE_HST_HW_M14_SRC_APL_STM_GBR_RDY1,			///< state gbr ready
	PE_HST_HW_M14_SRC_APL_STM_GBR_GET1,			///< state gbr get
	PE_HST_HW_M14_SRC_APL_STM_GBR_RDY2,			///< state gbr ready
	PE_HST_HW_M14_SRC_APL_STM_GBR_GET2,			///< state gbr get

	PE_HST_HW_M14_SRC_APL_STM_BBD_RDY0,			///< state bbd ready
	PE_HST_HW_M14_SRC_APL_STM_BBD_GET0,			///< state bbd get

	PE_HST_HW_M14_SRC_APL_STM_MAX				///< max num
}
PE_HST_HW_M14_SRC_APL_STM;

/**
 *	pe histo src apl selection enumeration
 */
typedef enum
{
	PE_HST_HW_M14_SRC_APL_SEL_INIT = 0,		///< init
	PE_HST_HW_M14_SRC_APL_SEL_YUV0,			///< ycbcr
	PE_HST_HW_M14_SRC_APL_SEL_HSV0,			///< hsv
	PE_HST_HW_M14_SRC_APL_SEL_GBR0,			///< gbr
	PE_HST_HW_M14_SRC_APL_SEL_GBR1,			///< gbr
	PE_HST_HW_M14_SRC_APL_SEL_GBR2,			///< gbr
	PE_HST_HW_M14_SRC_APL_SEL_BBD0,			///< bbd
	PE_HST_HW_M14_SRC_APL_SEL_MAX			///< max num
}
PE_HST_HW_M14_SRC_APL_SEL;

/**
 *	pe src histo state machine enumeration
 */
typedef enum
{
	PE_HST_HW_M14_SRC_HST_STM_START = 0,		///< state start
	PE_HST_HW_M14_SRC_HST_STM_RDY_01ST_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_01ST_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_01ST,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_01ST,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_02ND_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_02ND_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_02ND,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_02ND,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_03RD_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_03RD_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_03RD,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_03RD,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_04TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_04TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_04TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_04TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_05TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_05TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_05TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_05TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_06TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_06TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_06TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_06TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_07TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_07TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_07TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_07TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_08TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_08TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_08TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_08TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_09TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_09TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_09TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_09TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_10TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_10TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_10TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_10TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_11TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_11TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_11TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_11TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_12TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_12TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_12TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_12TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_13TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_13TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_13TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_13TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_14TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_14TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_14TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_14TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_15TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_15TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_15TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_15TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_16TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_16TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_16TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_16TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_17TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_17TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_17TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_17TH,			///< state release
	PE_HST_HW_M14_SRC_HST_STM_RDY_18TH_0,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_RDY_18TH_1,		///< state ready
	PE_HST_HW_M14_SRC_HST_STM_GET_18TH,			///< state get
	PE_HST_HW_M14_SRC_HST_STM_RLS_18TH,			///< state release	
	PE_HST_HW_M14_SRC_HST_STM_END,				///< state end
	PE_HST_HW_M14_SRC_HST_STM_MAX				///< max num
}
PE_HST_HW_M14_SRC_HST_STM;

/**
 *	pe src histo selection enumeration
 */
typedef enum
{
	PE_HST_HW_M14_SRC_HST_SEL_Y = 0,		///< ycbcr y
	PE_HST_HW_M14_SRC_HST_SEL_CB,			///< ycbcr cb
	PE_HST_HW_M14_SRC_HST_SEL_CR,			///< ycbcr cr
	PE_HST_HW_M14_SRC_HST_SEL_G,			///< gbr g
	PE_HST_HW_M14_SRC_HST_SEL_B,			///< gbr b
	PE_HST_HW_M14_SRC_HST_SEL_R,			///< gbr r
	PE_HST_HW_M14_SRC_HST_SEL_H,			///< hsv h
	PE_HST_HW_M14_SRC_HST_SEL_S,			///< hsv s
	PE_HST_HW_M14_SRC_HST_SEL_V,			///< hsv v
	PE_HST_HW_M14_SRC_HST_SEL_MAX			///< max num
}
PE_HST_HW_M14_SRC_HST_SEL;

/**
 *	pe src histo mode selection enumeration
 */
typedef enum
{
	PE_HST_HW_M14_SRC_HST_MOD_SEL_FULL = 0,		///< full win, not 2d case
	PE_HST_HW_M14_SRC_HST_MOD_SEL_TB,			///< tb win, for 2d & auto fmt detect
	PE_HST_HW_M14_SRC_HST_MOD_SEL_SS,			///< ss win, for 2d & auto fmt detect
	PE_HST_HW_M14_SRC_HST_MOD_SEL_MAX			///< max num
}
PE_HST_HW_M14_SRC_HST_MOD_SEL;

/**
 *	pe lrcr histo selection enumeration
 */
typedef enum
{
	PE_HST_HW_M14_LRC_HST_WIN_MOD_2D = 0,		///< window mode 2d
	PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0,			///< window mode tb
	PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0,			///< window mode ss
	PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1,			///< window mode tb
	PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1,			///< window mode ss
	PE_HST_HW_M14_LRC_HST_WIN_MOD_MAX			///< max num
}
PE_HST_HW_M14_LRC_HST_WIN_MOD;

/**
 *	pe lrcr histo state machine enumeration
 */
typedef enum
{
	PE_HST_HW_M14_LRC_HST_STM_INIT = 0,		///< state init
	PE_HST_HW_M14_LRC_HST_STM_RDY0_0,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_RDY0_1,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_GET0,			///< state get
	PE_HST_HW_M14_LRC_HST_STM_RLS0,			///< state release
	PE_HST_HW_M14_LRC_HST_STM_RDY1_0,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_RDY1_1,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_GET1,			///< state get
	PE_HST_HW_M14_LRC_HST_STM_RLS1,			///< state release
	PE_HST_HW_M14_LRC_HST_STM_RDY2_0,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_RDY2_1,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_GET2,			///< state get
	PE_HST_HW_M14_LRC_HST_STM_RLS2,			///< state release
	PE_HST_HW_M14_LRC_HST_STM_RDY3_0,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_RDY3_1,		///< state ready
	PE_HST_HW_M14_LRC_HST_STM_GET3,			///< state get
	PE_HST_HW_M14_LRC_HST_STM_RLS3,			///< state release
	PE_HST_HW_M14_LRC_HST_STM_MAX			///< max num
}
PE_HST_HW_M14_LRC_HST_STM;

/**
 *	pe lrcr histo selection enumeration
 */
typedef enum
{
	PE_HST_HW_M14_LRC_HST_SEL_TB0_GRN = 0,	///< tb0 top green
	PE_HST_HW_M14_LRC_HST_SEL_TB0_BLU,		///< tb0 top blue
	PE_HST_HW_M14_LRC_HST_SEL_TB0_RED,		///< tb0 top red
	PE_HST_HW_M14_LRC_HST_SEL_SS0_GRN,		///< ss0 left green
	PE_HST_HW_M14_LRC_HST_SEL_SS0_BLU,		///< ss0 left blue
	PE_HST_HW_M14_LRC_HST_SEL_SS0_RED,		///< ss0 left red
	PE_HST_HW_M14_LRC_HST_SEL_TB1_GRN,		///< tb1 top green
	PE_HST_HW_M14_LRC_HST_SEL_TB1_BLU,		///< tb1 top blue
	PE_HST_HW_M14_LRC_HST_SEL_TB1_RED,		///< tb1 top red
	PE_HST_HW_M14_LRC_HST_SEL_SS1_GRN,		///< ss1 left green
	PE_HST_HW_M14_LRC_HST_SEL_SS1_BLU,		///< ss1 left blue
	PE_HST_HW_M14_LRC_HST_SEL_SS1_RED,		///< ss1 left red
	PE_HST_HW_M14_LRC_HST_SEL_RD_DONE,		///< read done
	PE_HST_HW_M14_LRC_HST_SEL_MAX			///< max num
}
PE_HST_HW_M14_LRC_HST_SEL;

/**
 *	pe detected 3d format type enumeration
 */
typedef enum {
	PE_HST_HW_M14_DET_3D_FMT_2D_2D = 0,		///< 2d
	PE_HST_HW_M14_DET_3D_FMT_3D_CB,			///< 3d cb
	PE_HST_HW_M14_DET_3D_FMT_3D_CC,			///< 3d cc
	PE_HST_HW_M14_DET_3D_FMT_3D_LL,			///< 3d ll
	PE_HST_HW_M14_DET_3D_FMT_3D_SS,			///< 3d ss
	PE_HST_HW_M14_DET_3D_FMT_3D_TB,			///< 3d tb
	PE_HST_HW_M14_DET_3D_FMT_UNKNOWN,		///< unknown
	PE_HST_HW_M14_DET_3D_FMT_MAX			///< max num
}
PE_HST_HW_M14_DET_3D_FMT;

/**
 *	pe state type
 */
typedef struct
{
	UINT32 curr;		///< current state
	UINT32 next;		///< next state
}
PE_HST_HW_M14_STATE_T;

/**
 *	pe apl control type
 */
typedef struct
{
	UINT32 apl_sel;					///< apl_selection, see PE_HST_HW_M14_SRC_APL_SEL
	UINT32 h_size[LX_PE_WIN_NUM];	///< h size
	UINT32 v_size[LX_PE_WIN_NUM];	///< v size
	UINT32 mux_sel;					///< mux selection
}
PE_HST_HW_M14_SRC_APL_CTRL_T;

/**
 *	pe apl data type
 */
typedef struct
{
	UINT32 yuv[LX_PE_WIN_NUM];		///< yuv [9:0]:y, [19:10]:cb, [29:20]:cr
	UINT32 hsv[LX_PE_WIN_NUM];		///< hsv [9:0]:h, [19:10]:s,  [29:20]:v
	UINT32 gbr[LX_PE_WIN_NUM];		///< gbr [9:0]:g, [19:10]:b,  [29:20]:r
	UINT32 bb0[LX_PE_WIN_NUM];		///< bbd state0 [11:0]:x0, [27:16]:y0
	UINT32 bb1[LX_PE_WIN_NUM];		///< bbd state1 [11:0]:x1, [27:16]:y1
}
PE_HST_HW_M14_SRC_APL_DATA_T;

/**
 *	pe src apl type
 */
typedef struct
{
	PE_HST_HW_M14_STATE_T state;			///< apl state, see PE_HST_HW_M14_SRC_APL_STM
	PE_HST_HW_M14_SRC_APL_CTRL_T ctrl;		///< apl control
	PE_HST_HW_M14_SRC_APL_DATA_T data;		///< apl data
}
PE_HST_HW_M14_SRC_APL_T;

/**
 *	pe histo control type
 */
typedef struct
{
	UINT32 out_type;					///< out type, see LX_PE_OUT_TYPE
	UINT32 in_f_rate;					///< input frame rate(x10 hz)
	UINT32 mod_sel;						///< mode selection, see PE_HST_HW_M14_SRC_HST_MOD_SEL
	UINT32 mux_sel[LX_PE_WIN_NUM][3];	///< mux selection, see PE_HST_HW_M14_SRC_HST_SEL
	UINT32 win_sel[LX_PE_WIN_NUM][3];	///< window selection, if 0,win_a, if 1,win_b
	UINT32 x_ofst[LX_PE_WIN_NUM];		///< x offset
	UINT32 y_ofst[LX_PE_WIN_NUM];		///< y offset
	UINT32 h_size[LX_PE_WIN_NUM];		///< h size
	UINT32 v_size[LX_PE_WIN_NUM];		///< v size
	UINT32 x0_pos[LX_PE_WIN_NUM][2];	///< x0 pos
	UINT32 y0_pos[LX_PE_WIN_NUM][2];	///< y0 pos
	UINT32 x1_pos[LX_PE_WIN_NUM][2];	///< x1 pos
	UINT32 y1_pos[LX_PE_WIN_NUM][2];	///< y1 pos
}
PE_HST_HW_M14_SRC_HST_CTRL_T;

/**
 *	pe histo data type
 */
typedef struct
{
	UINT32 y0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< y0, last one is histo valid
	UINT32 y1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< y1, last one is histo valid
	UINT32 cb0[PE_HST_HW_M14_HST_BIN_NUM+1];	///< cb0, last one is histo valid
	UINT32 cb1[PE_HST_HW_M14_HST_BIN_NUM+1];	///< cb1, last one is histo valid
	UINT32 cr0[PE_HST_HW_M14_HST_BIN_NUM+1];	///< cr0, last one is histo valid
	UINT32 cr1[PE_HST_HW_M14_HST_BIN_NUM+1];	///< cr1, last one is histo valid
	UINT32 g0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< g0, last one is histo valid
	UINT32 g1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< g1, last one is histo valid
	UINT32 b0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< b0, last one is histo valid
	UINT32 b1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< b1, last one is histo valid
	UINT32 r0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< r0, last one is histo valid
	UINT32 r1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< r1, last one is histo valid
	UINT32 h0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< h0, last one is histo valid
	UINT32 h1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< h1, last one is histo valid
	UINT32 s0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< s0, last one is histo valid
	UINT32 s1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< s1, last one is histo valid
	UINT32 v0[PE_HST_HW_M14_HST_BIN_NUM+1];		///< v0, last one is histo valid
	UINT32 v1[PE_HST_HW_M14_HST_BIN_NUM+1];		///< v1, last one is histo valid
	UINT32 pxl_cnt[LX_PE_WIN_NUM];				///< pixel count
	UINT32 s_state[LX_PE_WIN_NUM];				///< s_state from histo
	UINT32 pre_h_bin[LX_PE_WIN_NUM];			///< pre hue bin 6~9 for green detection
	UINT32 cur_h_bin[LX_PE_WIN_NUM];			///< cur hue bin 6~9 for green detection
}
PE_HST_HW_M14_SRC_HST_DATA_T;

/**
 *	pe src histo type
 */
typedef struct
{
	PE_HST_HW_M14_STATE_T state;		///< state, see PE_HST_HW_M14_SRC_HST_STM
	PE_HST_HW_M14_SRC_HST_CTRL_T ctrl;	///< histo control
	PE_HST_HW_M14_SRC_HST_DATA_T data;	///< histo data
}
PE_HST_HW_M14_SRC_HST_T;

/**
 *	pe lrcr histo control type
 */
typedef struct
{
	UINT32 out_type;		///< out type, see LX_PE_OUT_TYPE
	UINT32 hst_sel;			///< histo selection, see PE_HST_HW_M14_LRC_HST_SEL
	UINT32 win_mod;			///< window mode, see PE_HST_HW_M14_LRC_HST_WIN_MOD
	UINT32 tb0_t_pos[4];	///< tb0 t position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb0_b_pos[4];	///< tb0 b position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss0_l_pos[4];	///< ss0 l position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss0_r_pos[4];	///< ss0 r position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb1_t_pos[4];	///< tb1 t position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 tb1_b_pos[4];	///< tb1 b position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss1_l_pos[4];	///< ss1 l position, [0]x0,[1]y0,[2]x1,[3]y1
	UINT32 ss1_r_pos[4];	///< ss1 r position, [0]x0,[1]y0,[2]x1,[3]y1
}
PE_HST_HW_M14_LRC_HST_CTRL_T;

/**
 *	pe lrcr histo data type
 */
typedef struct
{
	UINT32 tb0_t_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 top green
	UINT32 tb0_b_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 bottom green
	UINT32 tb0_t_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 top blue
	UINT32 tb0_b_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 bottom blue
	UINT32 tb0_t_red[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 top red
	UINT32 tb0_b_red[PE_HST_HW_M14_HST_BIN_NUM];	///< tb0 bottom red
	UINT32 ss0_l_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 left green
	UINT32 ss0_r_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 right green
	UINT32 ss0_l_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 left blue
	UINT32 ss0_r_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 right blue
	UINT32 ss0_l_red[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 left red
	UINT32 ss0_r_red[PE_HST_HW_M14_HST_BIN_NUM];	///< ss0 right red
	UINT32 tb1_t_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 top green
	UINT32 tb1_b_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 bottom green
	UINT32 tb1_t_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 top blue
	UINT32 tb1_b_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 bottom blue
	UINT32 tb1_t_red[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 top red
	UINT32 tb1_b_red[PE_HST_HW_M14_HST_BIN_NUM];	///< tb1 bottom red
	UINT32 ss1_l_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 left green
	UINT32 ss1_r_grn[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 right green
	UINT32 ss1_l_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 left blue
	UINT32 ss1_r_blu[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 right blue
	UINT32 ss1_l_red[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 left red
	UINT32 ss1_r_red[PE_HST_HW_M14_HST_BIN_NUM];	///< ss1 right red
}
PE_HST_HW_M14_LRC_HST_DATA_T;

/**
 *	pe lrcr histo type
 */
typedef struct
{
	PE_HST_HW_M14_STATE_T state;		///< histo state, see PE_HST_HW_M14_LRC_HST_STM
	PE_HST_HW_M14_LRC_HST_CTRL_T ctrl;	///< histo control
	PE_HST_HW_M14_LRC_HST_DATA_T data;	///< histo data
}
PE_HST_HW_M14_LRC_HST_T;

/**
 *	pe display hst info type
 */
typedef struct
{
	LX_PE_SRC_TYPE src_type;	///< pe source type
	LX_PE_OUT_TYPE out_type;	///< pe out type
}
PE_HST_HW_M14_DIS_INFO_T;

/**
 *	pe lrcr hst info type
 */
typedef struct
{
	UINT32 dif_max[PE_HST_HW_M14_LRC_INF_NUM];	///< diff. max, see PE_HST_HW_M14_LRC_INF_NUM
	UINT32 pxl_cnt[PE_HST_HW_M14_LRC_INF_NUM];	///< pixel count, see PE_HST_HW_M14_LRC_INF_NUM
	UINT32 set_cnt[PE_HST_HW_M14_LRC_INF_NUM];	///< setting count, see PE_HST_HW_M14_LRC_INF_NUM
}
PE_HST_HW_M14_LRC_INFO_T;

/**
 *	pe src hst info type
 */
typedef struct
{
	UINT32 s_state[LX_PE_WIN_NUM];				///< s_state from histo
	UINT32 dif_cnt[PE_HST_HW_M14_SRC_INF_NUM];	///< diff. count, see PE_HST_HW_M14_SRC_INF_NUM
	UINT32 pxl_cnt[PE_HST_HW_M14_SRC_INF_NUM];	///< pixel count, see PE_HST_HW_M14_SRC_INF_NUM
	UINT32 set_cnt[PE_HST_HW_M14_SRC_INF_NUM];	///< setting count, see PE_HST_HW_M14_SRC_INF_NUM
}
PE_HST_HW_M14_SRC_INFO_T;

/**
 *	pe hst info type
 */
typedef struct
{
	PE_HST_HW_M14_DET_3D_FMT det_3df;		///< detected 3d format
	PE_HST_HW_M14_DIS_INFO_T dis_inf;		///< display info.
	PE_HST_HW_M14_LRC_INFO_T lrc_inf;		///< lrcr histo info.
	PE_HST_HW_M14_SRC_INFO_T src_inf;		///< src histo info.
}
PE_HST_HW_M14_INFO_T;

/**
 *	pe hst hw settings type
 */
typedef struct
{
	PE_HST_HW_M14_SRC_APL_T src_apl;			///< src apl
	PE_HST_HW_M14_SRC_HST_T src_hist;			///< src histogram
	PE_HST_HW_M14_LRC_HST_T lrc_hist;			///< lrcr histogram
	PE_HST_HW_M14_INFO_T hist_inf;				///< histo info
	LX_PE_HST_HISTO_CFG_T cfg[LX_PE_WIN_NUM];	///< histo config
}
PE_HST_HW_M14_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void PE_HST_HW_M14_SetDbgPrint(UINT32 on_off);
int PE_HST_HW_M14_Init(PE_CFG_CTRL_T *pstParams);
int PE_HST_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_HST_HW_M14_GetGlobalHistInfo(LX_PE_ETC_TBL_T *pstParams);
int PE_HST_HW_M14_CheckGlobalHistoInfo(PE_TSK_M14_CFG_T *pCfg);
int PE_HST_HW_M14_SetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams);
int PE_HST_HW_M14_GetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams);
int PE_HST_HW_M14_GetHistoInfo(PE_HST_HW_M14_INFO_T *pstParams);
int PE_HST_HW_M14_GetDetected3dFmt(UINT32 *pstParams);
int PE_HST_HW_M14_GetSaturationState(UINT32 win_id, UINT32 *pstParams);
int PE_HST_HW_M14_GetSrcHistoBin(UINT32 *pstParams, PE_HST_HW_M14_SRC_HST_SEL hst_sel);
int PE_HST_HW_M14_GetHistogramInfo(LX_PE_HST_HISTO_INFO_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_HST_HW_M14_H_ */
