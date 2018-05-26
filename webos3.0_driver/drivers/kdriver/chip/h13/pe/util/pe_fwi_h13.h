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

/** @file pe_fwi_h13.h
 *
 *  fw ipc interface header for pe device. ( used only within kdriver )
 *
 *	@author
 *	@version	0.1
 *	@note
 *	@date		2011.09.20
 *	@see
 */

#ifndef	_PE_FWI_H13_H_
#define	_PE_FWI_H13_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_tsk_h13.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_FWI_H13_BASE_OFFSET				0
#define PE_FWI_H13_TNR_CTRL_OFFSET			PE_FWI_H13_BASE_OFFSET
#define PE_FWI_H13_FW_CTRL_OFFSET			(PE_FWI_H13_TNR_CTRL_OFFSET+sizeof(PE_FWI_H13_TNR_CTRL))
#define PE_FWI_H13_HOR_CTRL_OFFSET			(PE_FWI_H13_FW_CTRL_OFFSET+sizeof(PE_FWI_H13_FW_CTRL))
#define PE_FWI_H13_MISC_CTRL_OFFSET			(PE_FWI_H13_HOR_CTRL_OFFSET+sizeof(PE_FWI_H13_HOR_CTRL))
#define PE_FWI_H13_EDGE_GAIN_OFFSET			(PE_FWI_H13_MISC_CTRL_OFFSET+sizeof(PE_FWI_H13_MISC_CTRL))
#define PE_FWI_H13_TEXTURE_GAIN_OFFSET		(PE_FWI_H13_EDGE_GAIN_OFFSET+sizeof(PE_FWI_H13_EDGE_GAIN))
#define PE_FWI_H13_SRC_INFO_OFFSET			(PE_FWI_H13_TEXTURE_GAIN_OFFSET+sizeof(PE_FWI_H13_TEXTURE_GAIN))
#define PE_FWI_H13_CEN_CTRL_OFFSET			(PE_FWI_H13_SRC_INFO_OFFSET+sizeof(PE_FWI_H13_SRC_INFO))

#define PE_FWI_H13_CEN_RGN_NUM		16

/**	20130120, sj.youm,
*	define for shp ctrl when pq fw off(reg. settings were moved from shp to fwi func.)
*	because of peaking transition artifact issue on split mode.
*/
#undef PE_FWI_H13_USE_SHP_CTRL_WHEN_PQFW_OFF

/**	20130219, sj.youm,
*	define for cen region ctrl because of noise issue on gain ctrl
*/
#define PE_FWI_H13_USE_CEN_RGN_CTRL

/**	20130220, sj.youm,
*	Define for cen region ctrl on be(frc) fw, because of noise issue on gain ctrl,
*	should be turned on only when PE_FWI_H13_USE_CEN_RGN_CTRL defined
*/
#ifdef PE_FWI_H13_USE_CEN_RGN_CTRL
#define PE_FWI_H13_USE_CEN_RGN_CTRL_BY_BE_FW
#else
#undef PE_FWI_H13_USE_CEN_RGN_CTRL_BY_BE_FW
#endif

/**	20130621, sj.youm,
*	define for use be fw histogram,
*	if ext 3dfmt,frc & 3d dual full out type
*	because of apl,histo shaking and flicker on 3d dual full
*/
#define PE_FWI_H13_USE_BE_FW_HISTO_INFO

/** 20140102, inkyung.park,
*     define not to use shp fw
*/
#define PE_FWI_H13_NOT_USE_SHP_FW


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * PE tnr core ctrl
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 tnr_en;			///< tnr core enable, 0:off, 1:on
	UINT8 m_fir_en;			///< tnr motion fir enable, 0:off, 1:on
}
PE_FWI_H13_TNR_CTRL;

/**
 * PE FW ctrl
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 enable;			///< fw control enable, 1:on, 0:off
}
PE_FWI_H13_FW_CTRL;

/**
 * PE Sharpness horizontal control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 e_gain_th1;		///< 0x5474 pe1_derh_ctrl_0b[7:0],	e_gain_th1
	UINT8 e_gain_th2;		///< 0x5474 pe1_derh_ctrl_0b[15:8],	e_gain_th2
}
PE_FWI_H13_HOR_CTRL;

/**
 * PE Sharpness misc texture-to-weight control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 gt_th0;			///< 0x5574 pe1_coring_ctrl_05[15:8],	gt_th0
	UINT8 gt_th1;			///< 0x5574 pe1_coring_ctrl_05[23:16],	gt_th1
	UINT8 gt_th0a;			///< 0x5578 pe1_coring_ctrl_06[7:0],	gt_th0a
	UINT8 gt_th0b;			///< 0x5578 pe1_coring_ctrl_06[15:8],	gt_th0b
}
PE_FWI_H13_MISC_CTRL;

/**
 * PE Sharpness edge gain control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 co09_gain_b;		///< 0x5584 pe1_coring_ctrl_09[6:0],	edge_gain_b
	UINT8 co09_gain_w;		///< 0x5584 pe1_coring_ctrl_09[14:8],	edge_gain_w
	UINT8 dp01_gain_b;		///< 0x5414 pe1_dp_ctrl_01[6:0],		edge_gain_b
	UINT8 dp01_gain_w;		///< 0x5414 pe1_dp_ctrl_01[14:8],		edge_gain_w
}
PE_FWI_H13_EDGE_GAIN;

/**
 * PE Sharpness texture gain control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 co09_gain_b;		///< 0x5584 pe1_coring_ctrl_09[22:16],	texture_gain_b
    UINT8 co09_gain_w;		///< 0x5584 pe1_coring_ctrl_09[30:24],	texture_gain_w
	UINT8 dp01_gain_b;		///< 0x5414 pe1_dp_ctrl_01[22:16],		texture_gain_b
    UINT8 dp01_gain_w;		///< 0x5414 pe1_dp_ctrl_01[30:24],		texture_gain_w
}
PE_FWI_H13_TEXTURE_GAIN;

/**
 * PE SRC Info
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 dtv_mode;	//0:normal,1:file,2:hdd,3:photo,4:tp,5:camera,6:invalid, see LX_PE_DTV_TYPE
	UINT8 hdd_mode;	//0:dtv, 1,2:atv60,50, 3,4:av60,50, 5:scartrgb, 6:invalid, see LX_PE_HDD_SRC_TYPE
}
PE_FWI_H13_SRC_INFO;

/**
 * PE cen Region control
 * PQ Firmware IPC Control
 */
typedef struct {
	UINT8 rgn_num;		///< region number, 0~15
	UINT8 delta_h;		///< region delta(offset), -128 ~ 127, h
	UINT8 delta_s;		///< region delta(offset), -128 ~ 127, s
	UINT8 delta_v;		///< region delta(offset), -128 ~ 127, v
	UINT8 delta_g;		///< region delta(offset), -128 ~ 127, g
	UINT8 delta_b;		///< region delta(offset), -128 ~ 127, b
	UINT8 delta_r;		///< region delta(offset), -128 ~ 127, r
	UINT8 ma_gain;		///< region master gain, 0~128~255
}
PE_FWI_H13_CEN_CTRL;

/**
 * PQ Firmware IPC Control
 */
typedef struct {
	PE_FWI_H13_TNR_CTRL			tnr_ctrl;		///< tnr ctrl
	PE_FWI_H13_FW_CTRL			fw_ctrl;		///< fwi ctrl
	PE_FWI_H13_HOR_CTRL			shp_h_ctrl;		///< hor ctrl
	PE_FWI_H13_MISC_CTRL		shp_m_ctrl;		///< misc ctrl
	PE_FWI_H13_EDGE_GAIN		shp_e_ctrl;		///< edge ctrl
	PE_FWI_H13_TEXTURE_GAIN		shp_t_ctrl;		///< texture ctrl
	PE_FWI_H13_SRC_INFO			src_info;		///< src info
	#ifdef PE_FWI_H13_USE_CEN_RGN_CTRL
	PE_FWI_H13_CEN_CTRL			cen_ctrl;		///< cen ctrl
	#endif
}
PE_FWI_H13_PQ_IPC_CTRL;

/**
 *	pe fwi settings type
 */
typedef struct
{
	PE_FWI_H13_TNR_CTRL		tnr_ctrl[LX_PE_WIN_NUM];		///< tnr ctrl
	PE_FWI_H13_FW_CTRL		fw_ctrl;						///< fwi ctrl
	PE_FWI_H13_HOR_CTRL		shp_h_ctrl;						///< hor ctrl
	PE_FWI_H13_MISC_CTRL	shp_m_ctrl;						///< misc ctrl
	PE_FWI_H13_EDGE_GAIN	shp_e_ctrl;						///< edge ctrl
	PE_FWI_H13_TEXTURE_GAIN	shp_t_ctrl;						///< texture ctrl
	PE_FWI_H13_SRC_INFO		src_info;						///< src info
	PE_FWI_H13_CEN_CTRL		cen_ctrl[LX_PE_CMG_REGION_NUM];	///< cen ctrl
}
PE_FWI_H13_SETTINGS_T;

#ifdef PE_FWI_H13_USE_CEN_RGN_CTRL_BY_BE_FW
/**
 *	BE fwi cen gain ctrl0 settings type
 *	- should be same with BE_PE_CEN_CTRL0_T struct
 */
typedef struct
{
	UINT32	u8regnNum		:	8,	///<	region number 0~15
			u8deltHue		:	8,	///<	delta hue, -127 ~ 127
			u8deltSat		:	8,	///<	delta saturation, -127 ~ 127
			u8deltVal		:	8;	///<	delta value, -127 ~ 127
}
PE_FWI_H13_CEN_CTRL0_T;
/**
 *	BE fwi cen gain ctrl1 settings type
 *	- should be same with BE_PE_CEN_CTRL1_T struct
 */
typedef struct
{
	UINT32	u8deltGrn		:	8,	///<	delta green, -127 ~ 127
			u8deltBlu		:	8,	///<	delta blue, -127 ~ 127
			u8deltRed		:	8,	///<	delta red, -127 ~ 127
			u8gainMst		:	8;	///<	master gain, 0 ~ 255
}
PE_FWI_H13_CEN_CTRL1_T;
#endif

/**
 *	BE fwi dce histo ctrl enumeration
 */
typedef enum
{
	PE_FWI_H13_DCE_HISTO_RD0 = 0,		///< read histo 0
	PE_FWI_H13_DCE_HISTO_RD1,			///< read histo 1
	PE_FWI_H13_DCE_HISTO_NUM			///< max num
}
PE_FWI_H13_DCE_HISTO_CTRL;
/**
 *	BE fwi histo ctrl settings type
 */
typedef struct
{
	PE_FWI_H13_DCE_HISTO_CTRL ctrl;		///< ctrl
}
PE_FWI_H13_HISTO_CTRL_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
void PE_FWI_H13_SetDbgPrint(UINT32 on_off);
UINT32 PE_FWI_H13_GetDbgPrint(void);
int PE_FWI_H13_IPC_Open(void);
int PE_FWI_H13_IPC_Close(void);
int PE_FWI_H13_Init(PE_CFG_CTRL_T *pstParams);
int PE_FWI_H13_SetFwCtrl(PE_FWI_H13_FW_CTRL *pstParams);
int PE_FWI_H13_SetFwCtrlEnable(PE_TSK_H13_CFG_T *pCfg);
int PE_FWI_H13_SetFwTnrEnable(UINT32 on_off);
int PE_FWI_H13_SetCviCsc(LX_PE_CSC_INPUT_T *pstParams);
int PE_FWI_H13_SetShpHorCtrl(PE_FWI_H13_HOR_CTRL *pstParams);
int PE_FWI_H13_SetShpMiscCtrl(PE_FWI_H13_MISC_CTRL *pstParams);
int PE_FWI_H13_SetEdgeGainCtrl(PE_FWI_H13_EDGE_GAIN *pstParams);
int PE_FWI_H13_SetTextureGainCtrl(PE_FWI_H13_TEXTURE_GAIN *pstParams);
int PE_FWI_H13_SetTnrCtrl(UINT32 win_id, PE_FWI_H13_TNR_CTRL *pstParams);
int PE_FWI_H13_SetSrcInfo(PE_FWI_H13_SRC_INFO *pstParams);
int PE_FWI_H13_SetCenCtrl(PE_FWI_H13_CEN_CTRL *pstParams);
int PE_FWI_H13_SetDceHistoReadCtrl(PE_FWI_H13_HISTO_CTRL_T *pstParams);
int PE_FWI_H13_GetCurFwiSettings(PE_FWI_H13_SETTINGS_T *pstParams);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_FWI_H13_H_ */

