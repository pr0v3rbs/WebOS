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

/** @file pe_shp_hw_h15.h
 *
 *  driver header for picture enhance sharpness. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_SHP_HW_H15_H_
#define	_PE_SHP_HW_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H15A_PBL_SAT_STATUS_TH	(192)
#define PE_SHP_HW_H15A_PRE_BLUR_SET_DFLT	(0x1)
#define PE_SHP_HW_H15A_PRE_BLUR_SET_OFF		(0x0)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define PE_SHP_HW_H15_SCL_MENU_TO_STR(_x)		(\
	((_x)==PE_SHP_HW_H15_RD_SCL_DISP  )? "DISP  ":\
	((_x)==PE_SHP_HW_H15_RD_SCL_INFO  )? "INFO  ":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1Y_H1)? "M1Y_H1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1Y_H2)? "M1Y_H2":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1Y_V1)? "M1Y_V1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1Y_V2)? "M1Y_V2":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2Y_H1)? "M2Y_H1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2Y_H2)? "M2Y_H2":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2Y_V1)? "M2Y_V1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2Y_V2)? "M2Y_V2":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1C_H1)? "M1C_H1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M1C_V1)? "M1C_V1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2C_H1)? "M2C_H1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_M2C_V1)? "M2C_V1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_P1Y_H1)? "P1Y_H1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_P1Y_H2)? "P1Y_H2":\
	((_x)==PE_SHP_HW_H15_RD_SCL_P1Y_V1)? "P1Y_V1":\
	((_x)==PE_SHP_HW_H15_RD_SCL_P1Y_V2)? "P1Y_V2":\
	"NUM")

#define PE_SHP_HW_H15A_RES_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_RES_ATV)? \
	"PE_SHP_H15A_RES_ATV":\
	((_x)==PE_SHP_H15A_RES_AV_480)? \
	"PE_SHP_H15A_RES_AV_480":\
	((_x)==PE_SHP_H15A_RES_AV_576)? \
	"PE_SHP_H15A_RES_AV_576":\
	((_x)==PE_SHP_H15A_RES_DTV_SD)? \
	"PE_SHP_H15A_RES_DTV_SD":\
	((_x)==PE_SHP_H15A_RES_SD)? \
	"PE_SHP_H15A_RES_SD":\
	((_x)==PE_SHP_H15A_RES_HD)? \
	"PE_SHP_H15A_RES_HD":\
	((_x)==PE_SHP_H15A_RES_HDMI_HD)? \
	"PE_SHP_H15A_RES_HDMI_HD":\
	((_x)==PE_SHP_H15A_RES_UHD)? \
	"PE_SHP_H15A_RES_UHD":\
	((_x)==PE_SHP_H15A_RES_PC)? \
	"PE_SHP_H15A_RES_PC":\
	"PE_SHP_H15A_RES_NUM")

#define PE_SHP_HW_H15A_SRS_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_SRS_ATV)? \
	"PE_SHP_H15A_SRS_ATV":\
	((_x)==PE_SHP_H15A_SRS_AV_480)? \
	"PE_SHP_H15A_SRS_AV_480":\
	((_x)==PE_SHP_H15A_SRS_AV_576)? \
	"PE_SHP_H15A_SRS_AV_576":\
	((_x)==PE_SHP_H15A_SRS_DTV_SD)? \
	"PE_SHP_H15A_SRS_DTV_SD":\
	((_x)==PE_SHP_H15A_SRS_SD)? \
	"PE_SHP_H15A_SRS_SD":\
	((_x)==PE_SHP_H15A_SRS_HD)? \
	"PE_SHP_H15A_SRS_HD":\
	((_x)==PE_SHP_H15A_SRS_HDMI_HD)? \
	"PE_SHP_H15A_SRS_HDMI_HD":\
	((_x)==PE_SHP_H15A_SRS_UHD)? \
	"PE_SHP_H15A_SRS_UHD":\
	((_x)==PE_SHP_H15A_SRS_PC)? \
	"PE_SHP_H15A_SRS_PC":\
	"PE_SHP_H15A_SRS_NUM")

#define PE_SHP_HW_H15A_SRE_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_SRE_HD)? \
	"PE_SHP_H15A_SRE_HD":\
	((_x)==PE_SHP_H15A_SRE_SD)? \
	"PE_SHP_H15A_SRE_SD":\
	((_x)==PE_SHP_H15A_SRE_COMP_HD)? \
	"PE_SHP_H15A_SRE_COMP_HD":\
	((_x)==PE_SHP_H15A_SRE_UHD)? \
	"PE_SHP_H15A_SRE_UHD":\
	"PE_SHP_H15A_SRE_NUM")

#define PE_SHP_HW_H15A_SRL_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_SRL_HD)? \
	"PE_SHP_H15A_SRL_HD":\
	((_x)==PE_SHP_H15A_SRL_SD)? \
	"PE_SHP_H15A_SRL_SD":\
	"PE_SHP_H15A_SRE_NUM")

#define PE_SHP_HW_H15A_PBL_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_PBL_ATV_NT)? \
	"PE_SHP_H15A_PBL_ATV_NT":\
	((_x)==PE_SHP_H15A_PBL_ATV_PAL)? \
	"PE_SHP_H15A_PBL_ATV_PAL":\
	((_x)==PE_SHP_H15A_PBL_AV)? \
	"PE_SHP_H15A_PBL_AV":\
	((_x)==PE_SHP_H15A_PBL_COMP_SD)? \
	"PE_SHP_H15A_PBL_COMP_SD":\
	((_x)==PE_SHP_H15A_PBL_COMP_SD)? \
	"PE_SHP_H15A_PBL_COMP_SD":\
	((_x)==PE_SHP_H15A_PBL_DTV_SD)? \
	"PE_SHP_H15A_PBL_DTV_SD":\
	((_x)==PE_SHP_H15A_PBL_HDMI_SD)? \
	"PE_SHP_H15A_PBL_HDMI_SD":\
	((_x)==PE_SHP_H15A_PBL_HD)? \
	"PE_SHP_H15A_PBL_HD":\
	((_x)==PE_SHP_H15A_PBL_PC)? \
	"PE_SHP_H15A_PBL_PC":\
	"PE_SHP_H15A_PBL_NUM")

#define PE_SHP_HW_H15A_CTI_FMT_TO_STR(_x)		(\
	((_x)==PE_SHP_H15A_CTI_ATV_NT)? \
	"PE_SHP_H15A_CTI_ATV_NT":\
	((_x)==PE_SHP_H15A_CTI_ATV_PAL)? \
	"PE_SHP_H15A_CTI_ATV_PAL":\
	((_x)==PE_SHP_H15A_CTI_HD)? \
	"PE_SHP_H15A_CTI_HD":\
	((_x)==PE_SHP_H15A_CTI_SD)? \
	"PE_SHP_H15A_CTI_SD":\
	((_x)==PE_SHP_H15A_CTI_PC)? \
	"PE_SHP_H15A_CTI_PC":\
	"PE_SHP_H15A_CTI_NUM")

#define PE_SHP_HW_H15A_TBL_IDX_TO_STR(_x)		(\
	((_x)==PE_SHP_HW_H15A_TBL_IDX_RES)? "RES":\
	((_x)==PE_SHP_HW_H15A_TBL_IDX_CTI)? "CTI":\
	((_x)==PE_SHP_HW_H15A_TBL_IDX_PBL)? "PBL":\
	((_x)==PE_SHP_HW_H15A_TBL_IDX_SRE)? "SRE":\
	((_x)==PE_SHP_HW_H15A_TBL_IDX_SRS)? "SRS":\
	"NUM")

/**
 *	pe table index enumeration(for H15AX)
 */
typedef enum
{
	PE_SHP_HW_H15A_TBL_IDX_RES = 0,    ///< shp
	PE_SHP_HW_H15A_TBL_IDX_CTI,        ///< cti
	PE_SHP_HW_H15A_TBL_IDX_PBL,        ///< pre blur
	PE_SHP_HW_H15A_TBL_IDX_SRE,        ///< sre
	PE_SHP_HW_H15A_TBL_IDX_SRS,        ///< sre shp(srs)
	PE_SHP_HW_H15A_TBL_IDX_NUM         ///< max num
}
PE_SHP_HW_H15A_TBL_IDX;

/**
 *	pe read default reg menu enumeration(for H15AX)
 */
typedef enum
{
	PE_SHP_HW_H15A_RD_MENU_DISP = 0,	///< 00 menu display
	PE_SHP_HW_H15A_RD_RES1_DISP,		///< 01 res1 display
	PE_SHP_HW_H15A_RD_RES2_DISP,		///< 02 res2 display
	PE_SHP_HW_H15A_RD_RES1_DFLT,		///< 03 res1 default
	PE_SHP_HW_H15A_RD_RES1_USER,		///< 04 res1 user
	PE_SHP_HW_H15A_RD_RES1_MASK,		///< 05 res1 mask
	PE_SHP_HW_H15A_RD_RES1_DATA,		///< 06 res1 data
	PE_SHP_HW_H15A_RD_CTI1_DISP,		///< 07 cti1 display
	PE_SHP_HW_H15A_RD_CTI2_DISP,		///< 08 cti2 display
	PE_SHP_HW_H15A_RD_CTI1_DFLT,		///< 09 cti1 default
	PE_SHP_HW_H15A_RD_CTI1_USER,		///< 0a cti1 user
	PE_SHP_HW_H15A_RD_CTI1_MASK,		///< 0b cti1 mask
	PE_SHP_HW_H15A_RD_CTI1_DATA,		///< 0c cti1 data
	PE_SHP_HW_H15A_RD_PBL1_DISP,		///< 0d pbl1 display
	PE_SHP_HW_H15A_RD_PBL2_DISP,		///< 0e pbl2 display
	PE_SHP_HW_H15A_RD_PBL1_DFLT,		///< 0f pbl1 default
	PE_SHP_HW_H15A_RD_PBL1_USER,		///< 10 pbl1 user
	PE_SHP_HW_H15A_RD_PBL1_MASK,		///< 11 pbl1 mask
	PE_SHP_HW_H15A_RD_PBL1_DATA,		///< 12 pbl1 data
	PE_SHP_HW_H15A_RD_SRS1_DISP,		///< 13 srs1 display
	PE_SHP_HW_H15A_RD_SRS1_DFLT,		///< 14 srs1 default
	PE_SHP_HW_H15A_RD_SRS1_USER,		///< 15 srs1 user
	PE_SHP_HW_H15A_RD_SRS1_MASK,		///< 16 srs1 mask
	PE_SHP_HW_H15A_RD_SRS1_DATA,		///< 17 srs1 data
	PE_SHP_HW_H15A_RD_MENU_MAX			///< 18 max num
}
PE_SHP_HW_H15A_RD_DFLT_MENU;

/**
 *	pe read scl filter coeff reg menu enumeration
 *	- msl(r):main scaler L(R), ssc:sub scaler
 */
typedef enum
{
	PE_SHP_HW_H15_RD_SCL_DISP = 0,		///< menu display
	PE_SHP_HW_H15_RD_SCL_INFO,			///< INFO
	PE_SHP_HW_H15_RD_SCL_DRV_ON,		///< scl drv on
	PE_SHP_HW_H15_RD_SCL_DRV_OFF,		///< scl drv off
	PE_SHP_HW_H15_RD_RDY_RD_M1Y,		///< ready rd m1y
	PE_SHP_HW_H15_RD_RDY_RD_M2Y,		///< ready rd m2y
	PE_SHP_HW_H15_RD_RDY_RD_M1C,		///< ready rd m1c
	PE_SHP_HW_H15_RD_RDY_RD_M2C,		///< ready rd m2c
	PE_SHP_HW_H15_RD_RDY_RD_P1Y,		///< ready rd p1y
	PE_SHP_HW_H15_RD_SCL_M1Y_H1,		///< M1Y_H1
	PE_SHP_HW_H15_RD_SCL_M1Y_H2,		///< M1Y_H2
	PE_SHP_HW_H15_RD_SCL_M1Y_V1,		///< M1Y_V1
	PE_SHP_HW_H15_RD_SCL_M1Y_V2,		///< M1Y_V2
	PE_SHP_HW_H15_RD_SCL_M2Y_H1,		///< M2Y_H1
	PE_SHP_HW_H15_RD_SCL_M2Y_H2,		///< M2Y_H2
	PE_SHP_HW_H15_RD_SCL_M2Y_V1,		///< M2Y_V1
	PE_SHP_HW_H15_RD_SCL_M2Y_V2,		///< M2Y_V2
	PE_SHP_HW_H15_RD_SCL_M1C_H1,		///< M1C_H1
	PE_SHP_HW_H15_RD_SCL_M1C_V1,		///< M1C_V1
	PE_SHP_HW_H15_RD_SCL_M2C_H1,		///< M1C_H1
	PE_SHP_HW_H15_RD_SCL_M2C_V1,		///< M1C_V1
	PE_SHP_HW_H15_RD_SCL_P1Y_H1,		///< P1Y_H1
	PE_SHP_HW_H15_RD_SCL_P1Y_H2,		///< P1Y_H2
	PE_SHP_HW_H15_RD_SCL_P1Y_V1,		///< P1Y_V1
	PE_SHP_HW_H15_RD_SCL_P1Y_V2,		///< P1Y_V2
	PE_SHP_HW_H15_RD_SCL_MAX			///< max num
}
PE_SHP_HW_H15_RD_SCL_MENU;

/**
 *	pe shp resolution enhance fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_RES_ATV = 0,	    ///< atv 
	PE_SHP_H15A_RES_AV_480,      	///< atv 480
	PE_SHP_H15A_RES_AV_576,		    ///< atv 480
	PE_SHP_H15A_RES_DTV_SD,			///< hd
	PE_SHP_H15A_RES_SD,				///< sd
	PE_SHP_H15A_RES_HD,				///< hd
	PE_SHP_H15A_RES_HDMI_HD,		///< hdmi hd
	PE_SHP_H15A_RES_UHD,			///< uhd
	PE_SHP_H15A_RES_PC,				///< pc
	PE_SHP_H15A_RES_NUM				///< max num
}
PE_SHP_HW_H15A_RES_FMT;

/**
 *	pe sre shp(srs) resolution enhance fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_SRS_ATV = 0,	    ///< atv
	PE_SHP_H15A_SRS_AV_480,     	///< atv 480
	PE_SHP_H15A_SRS_AV_576,		    ///< atv 480
	PE_SHP_H15A_SRS_DTV_SD,			///< hd
	PE_SHP_H15A_SRS_SD,				///< sd
	PE_SHP_H15A_SRS_HD,				///< hd
	PE_SHP_H15A_SRS_HDMI_HD,		///< hdmi hd
	PE_SHP_H15A_SRS_UHD,			///< uhd
	PE_SHP_H15A_SRS_PC,				///< pc
	PE_SHP_H15A_SRS_NUM				///< max num
}
PE_SHP_HW_H15A_SRS_FMT;

/**
 *	pe shp sre fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_SRE_HD = 0,			///< hdmi hd
	PE_SHP_H15A_SRE_SD,				///< hdmi sd
	PE_SHP_H15A_SRE_COMP_HD,		///< comp hd
	PE_SHP_H15A_SRE_UHD,			///< uhd
	PE_SHP_H15A_SRE_NUM				///< max num
}
PE_SHP_HW_H15A_SRE_FMT;

/**
 *	pe shp sre lut fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_SRL_HD = 0,			///< hd
	PE_SHP_H15A_SRL_SD,				///< sd
	PE_SHP_H15A_SRL_NUM				///< max num
}
PE_SHP_HW_H15A_SRL_FMT;

/**
 *	pe pre blur(dnr) fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_PBL_ATV_NT = 0,		///< atv nt
	PE_SHP_H15A_PBL_ATV_PAL,		///< atv pal
	PE_SHP_H15A_PBL_AV,				///< av
	PE_SHP_H15A_PBL_SECAM,			///< secam
	PE_SHP_H15A_PBL_COMP_SD,		///< comp sd
	PE_SHP_H15A_PBL_DTV_SD,			///< dtv sd
	PE_SHP_H15A_PBL_HDMI_SD,		///< hdmi sd
	PE_SHP_H15A_PBL_HD,				///< hd
	PE_SHP_H15A_PBL_PC,				///< pc
	PE_SHP_H15A_PBL_NUM				///< max num
}
PE_SHP_HW_H15A_PBL_FMT;

/**
 *	pe cti(p0d) fmt(for H15AX)
 */
typedef enum
{
	PE_SHP_H15A_CTI_ATV_NT = 0,		///< atv nt
	PE_SHP_H15A_CTI_ATV_PAL,		///< atv pal
	PE_SHP_H15A_CTI_HD,				///< hd
	PE_SHP_H15A_CTI_SD,				///< sd
	PE_SHP_H15A_CTI_PC,				///< pc
	PE_SHP_H15A_CTI_NUM				///< max num
}
PE_SHP_HW_H15A_CTI_FMT;

#ifdef PE_HW_H15_FOR_BRINGUP
/**
 *	pe scl index type
 */
typedef struct
{
	UINT32 h_y_indx;		///< h_y_index
	UINT32 v_y_indx;		///< v_y_index
	UINT32 h_c_indx;		///< h_c_index
	UINT32 v_c_indx;		///< v_c_index
}
PE_SHP_HW_H15_SCL_INDX_T;
/**
 *	pe scl ctrl type
 */
typedef struct
{
	UINT32 h_y_indx;		///< info : h_y_index
	UINT32 v_y_indx;		///< info : v_y_index
	UINT32 h_c_indx;		///< info : h_c_index
	UINT32 v_c_indx;		///< info : v_c_index
	UINT32 h_s_indx;		///< info : h_sub_index
	UINT32 v_s_indx;		///< info : v_sub_index
	UINT32 h_i_size;		///< info : h_i_size
	UINT32 v_i_size;		///< info : v_i_size
	UINT32 h_active;		///< info : h_active
	UINT32 v_active;		///< info : v_active
	UINT32 h_o_size;		///< info : h_o_size
	UINT32 v_o_size;		///< info : v_o_size
	UINT32 in_c_fmt;		///< info : input color format(nd), 0x6:444,0x5:422, else 420
	UINT32 pixel_rp;		///< info : pixel replace
	UINT32 acti_fmt;		///< info : active fmt(1:seamless mode)
	UINT32 vscl_mode;		///< ctrl : v sc_mode
	UINT32 adaptv_on;		///< ctrl : adaptive mode
	UINT32 blnear_on;		///< ctrl : bilinear mode on
	UINT32 h1_co_num;		///< ctrl : h1 coeff number
	UINT32 h2_co_num;		///< ctrl : h2 coeff number
	UINT32 v1_co_num;		///< ctrl : v1 coeff number
	UINT32 v2_co_num;		///< ctrl : v2 coeff number
}
PE_SHP_HW_H15_SCL_CTRL_T;
/**
 *	pe scl info type
 */
typedef struct
{
	PE_SHP_HW_H15_SCL_CTRL_T m1_y;		///< m1_y, msc
	PE_SHP_HW_H15_SCL_CTRL_T m2_y;		///< m2_y, msc
	PE_SHP_HW_H15_SCL_CTRL_T m1_c;		///< m1_c, msc
	PE_SHP_HW_H15_SCL_CTRL_T m2_c;		///< m2_c, msc
	PE_SHP_HW_H15_SCL_CTRL_T mp_y;		///< mp_y, post sc
}
PE_SHP_HW_H15_SCL_INFO_T;
#else
/**
 *	scaler fir filter type enumeration
 */
typedef enum
{
	PE_SHP_HW_H15_FIR_BILINEAR = 0,	///< bilinear filter
	PE_SHP_HW_H15_FIR_POLYPHASE,	///< polyphase filter
	PE_SHP_HW_H15_FIR_NUM			///< max num
}
PE_SHP_HW_H15_FIR;

/**
 *	v scaler tap mode type enumeration
 */
typedef enum
{
	PE_SHP_HW_H15_SCL_V12T_NORM = 0,	///< v12tap mode
	PE_SHP_HW_H15_SCL_V12T_DN14,		///< v12tap mode, h50%,v14% down scaling case
	PE_SHP_HW_H15_SCL_V06T_NORM,		///< v06tap mode, normal case
	PE_SHP_HW_H15_SCL_V06T_HALF,		///< v06tap mode, half case
	PE_SHP_HW_H15_SCL_V06T_DN75,		///< v06tap mode, 75% down scaling case
	PE_SHP_HW_H15_SCL_V06T_DN50,		///< v06tap mode, 50% down scaling case
	PE_SHP_HW_H15_SCL_V06T_DN25,		///< v06tap mode, 25% down scaling case
	PE_SHP_HW_H15_SCL_NUM				///< max num
}
PE_SHP_HW_H15_SCL;
#endif

/**
 *	pe sharpness module settings type
 */
typedef struct
{
	#ifdef PE_HW_H15_FOR_BRINGUP
	PE_SHP_HW_H15_SCL_INDX_T user_gsc_idx;				///< user gsc index
	#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
	PE_SHP_HW_H15_SCL_INDX_T user_psc_idx;				///< user psc index
	#endif
	PE_SHP_HW_H15_SCL_INDX_T user_sub_idx;				///< user sub index
	PE_SHP_HW_H15_SCL_INFO_T cur_info;					///< current info
	PE_SHP_HW_H15_SCL_INFO_T pre_info;					///< previous info
	PE_SHP_HW_H15A_RES_FMT res0a_fmt;					///< previous pe1 shp res0a fmt
	PE_SHP_HW_H15A_CTI_FMT cti0a_fmt;					///< previous dnr cti0a fmt
	PE_SHP_HW_H15A_PBL_FMT pbl0a_fmt;					///< previous dnr pbl0a(pre blur) fmt
	PE_SHP_HW_H15A_SRE_FMT sre0a_fmt;					///< previous sre0a fmt
	PE_SHP_HW_H15A_SRS_FMT srs0a_fmt;					///< previous srs0a fmt
	#else
	LX_PE_SHP_SCLFILTER_T cur_scl_idx[LX_PE_WIN_NUM];	///< current scaler filter index
	LX_PE_SHP_SCLFILTER_T pre_scl_idx[LX_PE_WIN_NUM];	///< previous scaler filter index
	PE_SHP_HW_H15_SCL h_y_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal luma tap type
	PE_SHP_HW_H15_SCL h_c_tap[LX_PE_WIN_NUM];			///< scaler filter horizontal chroma tap type
	PE_SHP_HW_H15_SCL v_y_tap[LX_PE_WIN_NUM];			///< scaler filter vertical luma tap type
	PE_SHP_HW_H15_SCL v_c_tap[LX_PE_WIN_NUM];			///< scaler filter vertical chroma tap type
	PE_SHP_HW_H15_FIR y_fir[LX_PE_WIN_NUM];				///< fir type, luma
	PE_SHP_HW_H15_FIR c_fir[LX_PE_WIN_NUM];				///< fir type, chroma
	UINT32 pre_res_en[LX_PE_WIN_NUM];					///< previous resolution enable
	PE_SHP_HW_H15A_RES_FMT res0b_fmt;					///< previous res0b fmt
	PE_SHP_HW_H15A_SRE_FMT sre0b_fmt;					///< previous sre0b fmt
	PE_SHP_HW_H15A_SRL_FMT srl0b_fmt;					///< previous srl0b fmt
	PE_SHP_HW_H15A_PBL_FMT vfc0b_fmt;					///< previous msc vfc0b fmt
	PE_SHP_HW_H15A_CTI_FMT cti0b_fmt;					///< previous p0d cti0b fmt
	UINT32 dft_msc_chroma_tear_ctrl;					///< default msc_chroma_tear_ctrl for h15a
	UINT32 cur_msc_chroma_tear_ctrl;					///< current msc_chroma_tear_ctrl for h15a
	#endif
}
PE_SHP_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_SHP_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_SHP_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_SHP_HW_H15_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int PE_SHP_HW_H15_SetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_H15_GetScalerFilterCoeff(LX_PE_SHP_SCLFILTER_T *pstParams);
int PE_SHP_HW_H15_SetScalerFilterCtrl(PE_TSK_H15_CFG_T *pCfg);
int PE_SHP_HW_H15_GetCurShpSettings(PE_SHP_HW_H15_SETTINGS_T *pstParams);
int PE_SHP_HW_H15_RdSclFltCoeff(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_H15_RdDefault(LX_PE_ETC_TBL_T *pstParams);
int PE_SHP_HW_H15_SetReCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_GetReCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_SetReHorCtrl(void *pstParams);
int PE_SHP_HW_H15_GetReHorCtrl(void *pstParams);
int PE_SHP_HW_H15_SetReVerCtrl(void *pstParams);
int PE_SHP_HW_H15_GetReVerCtrl(void *pstParams);
int PE_SHP_HW_H15_SetReMiscCtrl(void *pstParams);
int PE_SHP_HW_H15_GetReMiscCtrl(void *pstParams);
int PE_SHP_HW_H15_SetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_GetEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_SetDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_GetDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreCtrl(void *pstParams);
#ifdef PE_SHP_PARAM_CHANGE_FOR_H15
int PE_SHP_HW_H15_SetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
int PE_SHP_HW_H15_GetSreScalerFilterCoeff(LX_PE_SHP_SR_SCLFILTER_T *pstParams);
int PE_SHP_HW_H15_SetSreCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreSclFlt(void *pstParams);
int PE_SHP_HW_H15_GetSreSclFlt(void *pstParams);
int PE_SHP_HW_H15_SetSreReCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreReCmnCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreReHorCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreReHorCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreReVerCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreReVerCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreReMiscCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreReMiscCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreEdgeEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_SetSreDetailEnhanceCtrl(void *pstParams);
int PE_SHP_HW_H15_GetSreDetailEnhanceCtrl(void *pstParams);
#endif
int PE_SHP_HW_H15A_SetPblApply(PE_TSK_H15_CFG_T *pCfg);
int PE_SHP_HW_H15_DownloadSclDb(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SHP_HW_H15_H_ */
