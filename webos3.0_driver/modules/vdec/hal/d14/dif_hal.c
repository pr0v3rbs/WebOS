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
 * main driver implementation for de device.
 *
 * author     @lge.com
 * version    1.0
 * date       20.04.09
 * note       Additional information.
 *
 * @addtogroup lx_vdec
 * @{
 */

/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define LOG_NAME	dif_hal
#define LOG_LEVEL	log_level_error
//#define BEF_CHIPREV	//before revision chip
/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include "hal/de_ipc_hal_api.h"
#include "hal/de_vdo_hal_api.h"
#include "hal/dif_hal.h"
#include "log.h"

// VCP
#include "../../../dxipc/dxipc_drv.h"
#include "../../../dxipc/uvd_reg.h"
#include "vcp_reg.h"

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/
#define DEIF_NUMOF_CH			4
#define DEIF_NUMOF_VDO_CH		2
#define DE_Q_FRAME_NUM			2


/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/
//#define assert(e)
#define assert(e)		\
	do{ if (!e) printk("assert! %s:%d\n",__FILE__, __LINE__); } while(0)
/*
//Box Test
#define TEST_WRITE_DPB(a,b)	 (*(volatile UINT32 *)(a+b))
UINT32 offsetDPB[4] = {0,(128*1024),(1024*1024),((128*1024)+(1024*1024))};
/////////
*/
/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef enum
{
	DEIF_HAL_IDLE_CH,
	DEIF_HAL_ACTIVE_CH,
	DEIF_HAL_MIRROR_CH
} DEIF_CH_STATE_E;

typedef struct
{
	BOOLEAN			valid;
	S_DE_IPC_T		ipc;
	UINT16			scan_type;
	UINT16			disp_period;
} DEIF_FRM_T;

typedef struct
{
	S_DE_IPC_T		sDeIpc;
	DEIF_FRM_T		frm[2];
	DEIF_FRM_T		*cur_frm;
	DEIF_FRM_T		*next_frm;

	UINT8			ch;
	UINT8			u8VsyncCh;
	enum
	{
		DE_IF_CLOSE,
		DE_IF_OPEN
	} eState;

	UINT32			ui32FieldCount;

	BOOLEAN			bForcedPush;
	UINT8			u8NoneForcedPushCnt;
	UINT8			u8nFrameRepeat;

	UINT32			u32PicWidth;
	UINT32			u32PicHeight;

	UINT32			u32FrameRateRes;
	UINT32			u32FrameRateDiv;
	UINT32			u32DpbMapType;
	SINT32			i32FramePackingArrange;
	UINT32			u32VOffset;
} DEIF_HAL_T;

/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   External Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Variables
 *----------------------------------------------------------------------------*/
static UINT8		au8PortUse[DEIF_NUMOF_CH];
static UINT8		g_mirror_out = 0xFF;
static UINT8		g_mirror_src = 0xFF;
static DEIF_HAL_T	deifs[DEIF_NUMOF_CH];
static UINT8	ui8SeamlessMode = 0;
static UINT8		u8PtsChInfo = 0;
/******************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
*******************************************************************************
*/
UINT32 DIF_HAL_GetChannelNum(void)
{
	return DEIF_NUMOF_CH;
}

void DIF_HAL_Reset(UINT8 ch)
{
	S_DE_IPC_T		*psDeIpc = NULL;
	DEIF_HAL_T		*deif;

	deif = &deifs[ch];

	deif->cur_frm = &deif->frm[0];
	deif->next_frm = &deif->frm[1];

	deif->cur_frm->valid = FALSE;
	deif->next_frm->valid = FALSE;

	psDeIpc = (S_DE_IPC_T *)&deif->sDeIpc;

	memset((void *)psDeIpc, 0x0, sizeof(S_DE_IPC_T));
	psDeIpc->ui32FrameIdx = 0xFF;
	psDeIpc->ui32DisplayMode = 0x3;
	psDeIpc->ui32LR_Order = 2;	// None
	psDeIpc->ui32DpbMapType = 0xFF;
	psDeIpc->ui32FramePackArrange = (UINT32)-2;

	deif->ch = ch;

	deif->bForcedPush = TRUE;
	deif->u8NoneForcedPushCnt = 0;

	deif->ui32FieldCount = 0xFFFFFFFF;

	deif->u32PicWidth = 0;
	deif->u32PicHeight = 0;

	deif->u32FrameRateRes = 0;
	deif->u32FrameRateDiv = 0;
	deif->u32VOffset = 0;

	deif->u32DpbMapType = 5;//DPB_MAX

}

void DIF_HAL_Init(void)
{
	UINT32	i;

	DE_IPC_HAL_Init();
	DE_VDO_HAL_Init();

	for (i=0; i<DEIF_NUMOF_CH; i++)
	{
		au8PortUse[i] = DEIF_HAL_IDLE_CH;
		DIF_HAL_Reset(i);
	}
}

void DIF_SetOutChannelMode(UINT32 mode)
{
	log_noti("Set uhd out mode : %d\n", mode);

	HDMI_FC_VSDPAYLOAD4(0) = mode;
	HDMI_FC_VSDPAYLOAD4(1) = mode;
	HDMI_FC_VSDPAYLOAD1(0) = mode;
	HDMI_FC_VSDPAYLOAD1(1) = mode;
}

void DIF_SetFrameInfoDxIpc(UINT32 ui32Resolution, UINT32 ui32DualOutMode, UINT32 ui32FrameRate, UINT32 ui32V_Offset)
{
	log_noti("Set first frame info to DxIPC : Resol %d DualoutMode %d FrmRate %d V_Offset %x\n"
			, ui32Resolution, ui32DualOutMode, ui32FrameRate, ui32V_Offset);

	DX_IPC_HAL_SetReg(DX_REG_MCU0_RESOLUTION, ui32Resolution);
	DX_IPC_HAL_SetReg(DX_REG_MCU0_FRAME_RATE, ui32FrameRate);
	DX_IPC_HAL_SetReg(DX_REG_MCU0_DUALOUT_MODE, ui32DualOutMode);
	DX_IPC_HAL_SetReg(DX_REG_VERTICALOFFSET, ui32V_Offset);
	DX_IPC_Send_to_MCU0(DX_REG_MCU0_RESOLUTION);

}

UINT32 DIF_GetClockSrc(void)
{
	UINT32 ui32ClockSource;
	ui32ClockSource = DX_IPC_HAL_GetReg(DX_REG_CLOCKSOURCE);
	
	return ui32ClockSource; 
}

inline void DE_IF_SetVcp(UINT32 ch, DISPQ_FRM_T *frm_info)
{
	UINT32			u32Temp = 0;
	UINT32			u32Temp2 = 0;
	BOOLEAN			bChanged = FALSE;
	DEIF_HAL_T		*dif;
	static SINT8 		PrevCh = -1;
	static UINT8		u8DelayCnt = 0;
	UINT32			u32VOffset = 0;

	dif = &deifs[ch];

	if (ui8SeamlessMode == 1)
	{
		if(PrevCh != ch)
		{
			u8DelayCnt++;
			if((u8DelayCnt == 1) && (PrevCh == 1)) //3-frame delay
			{
				if(ch == 0)
				{
//					HDMI_FC_VSDPAYLOAD4(0) = 0x00000001;
//					HDMI_FC_VSDPAYLOAD4(1) = 0x00000001;
				}
				log_noti("Change channel(Seamless) : ch%d -> ch%d\n",PrevCh,ch);
				PrevCh = ch;
				bChanged = TRUE;
				u8DelayCnt = 0;
				u8PtsChInfo = 0;
			}
			if((u8DelayCnt == 1) && (PrevCh == 0))
			{
				if(ch == 1)
				{
//					HDMI_FC_VSDPAYLOAD4(0) = 0x00000002;
//					HDMI_FC_VSDPAYLOAD4(1) = 0x00000002;
				}
				log_noti("Change channel(Seamless) : ch%d -> ch%d\n",PrevCh,ch);
				PrevCh = ch;
				bChanged = TRUE;
				u8DelayCnt = 0;
				u8PtsChInfo = 1;
			}

			if (PrevCh == -1) //for initial
			{
				if(ch == 0)
				{
//					HDMI_FC_VSDPAYLOAD4(0) = 0x00000001;
//					HDMI_FC_VSDPAYLOAD4(1) = 0x00000001;
					log_noti("Change channel(Seamless) : ch%d -> ch%d\n",PrevCh,ch);
					PrevCh = ch;
					u8DelayCnt = 0;
					u8PtsChInfo = 0;
				}
				else if(ch == 1)
				{
//					HDMI_FC_VSDPAYLOAD4(0) = 0x00000002;
//					HDMI_FC_VSDPAYLOAD4(1) = 0x00000002;
					log_noti("Change channel(Seamless) : ch%d -> ch%d\n",PrevCh,ch);
					PrevCh = ch;
					u8DelayCnt = 0;
					u8PtsChInfo = 1;
				}
			}
		}
	}
	else
	{
//		HDMI_FC_VSDPAYLOAD4(0) = 0x00000000;
//		HDMI_FC_VSDPAYLOAD4(1) = 0x00000000;
		PrevCh = -1;
		u8DelayCnt = 0;
	}
	

	//resolution change check
	if (dif->u32PicWidth != frm_info->ui32PicWidth ||
			dif->u32PicHeight != frm_info->ui32PicHeight)
	{
		log_noti("Change resol %d x %d -> %d x %d\n",
			dif->u32PicWidth, dif->u32PicHeight,
			frm_info->ui32PicWidth, frm_info->ui32PicHeight);

		dif->u32PicWidth = frm_info->ui32PicWidth;
		dif->u32PicHeight = frm_info->ui32PicHeight;

		bChanged = TRUE;
	}

	//framerate change check
	if (dif->u32FrameRateRes != frm_info->ui32FrameRateRes ||
			dif->u32FrameRateDiv != frm_info->ui32FrameRateDiv)
	{
		log_noti("Change rate %d/%d -> %d/%d\n",
				dif->u32FrameRateRes, dif->u32FrameRateDiv,
				frm_info->ui32FrameRateRes, frm_info->ui32FrameRateDiv);

		dif->u32FrameRateRes = frm_info->ui32FrameRateRes;
		dif->u32FrameRateDiv = frm_info->ui32FrameRateDiv;

		bChanged = TRUE;
	}

	//DPB Map Type change check -> 8bit/10bit
	if( dif->u32DpbMapType != frm_info->ui32DpbMapMode)
	{
		log_noti("Change map type %d -> %d\n",
				dif->u32DpbMapType, frm_info->ui32DpbMapMode);
		dif->u32DpbMapType = frm_info->ui32DpbMapMode;

		bChanged = TRUE;
	}

	//FramePacking Arrangement information check
	if( dif->i32FramePackingArrange != frm_info->i32FramePackArrange)
	{
		log_noti("3D Frame Arrange type %d -> %d\n",
				dif->i32FramePackingArrange, frm_info->i32FramePackArrange);
		dif->i32FramePackingArrange = frm_info->i32FramePackArrange;

		bChanged = TRUE;
	}
	
	//Vertical Offset information check
	if( dif->u32VOffset != frm_info->ui32V_Offset)
	{
		log_noti("Frame Vertical offset %x -> %x\n",
				dif->u32VOffset, frm_info->ui32V_Offset);
		dif->u32VOffset = frm_info->ui32V_Offset;

		bChanged = TRUE;
	}

	if (bChanged)
	{
		UINT32 u32Residual;
		UINT32 ui32CropW;
		UINT32 ui32CropH;
		DX_TYPE_3D e3DType;
		UINT32 ui32ClkSrc;

		// Crop picture size calculate
		if((frm_info->ui32DpbMapMode == 3) || (frm_info->ui32DpbMapMode == 4)) //HEVC crop(left, right, bottom)
		{
			ui32CropW = (frm_info->ui32PicWidth -( (frm_info->ui32H_Offset >>16)&0xFFFF) ) - (frm_info->ui32H_Offset&0xFFFF);
//			ui32CropH = (frm_info->ui32PicHeight & 0xFFFF) - (frm_info->ui32V_Offset & 0xFFFF);
//			top and bottom crop set to DX_IPC
			ui32CropH = (frm_info->ui32PicHeight & 0xFFFF) - ( (frm_info->ui32V_Offset >>16)&0xFFFF) - (frm_info->ui32V_Offset & 0xFFFF);

			u32Temp = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
		}
		else //H.264 crop(Right, bottom)
		{
			ui32CropW = frm_info->ui32PicWidth - (frm_info->ui32H_Offset&0xFFFF);
			ui32CropH = (frm_info->ui32PicHeight & 0xFFFF) - (frm_info->ui32V_Offset & 0xFFFF);
			u32Temp = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
		}
		DX_IPC_HAL_SetReg(DX_REG_RESOLUTION, u32Temp);

		u32Temp = frm_info->ui32FrameRateRes/frm_info->ui32FrameRateDiv;
		u32Temp *= 1000;
		u32Residual = frm_info->ui32FrameRateRes%frm_info->ui32FrameRateDiv;
		u32Temp += (u32Residual*1000)/frm_info->ui32FrameRateDiv;
		//when frame rate is smaller than 30Hz, even/odd mode setting
		//when frame rate is from 30Hz to 60Hz, Left/Right mode setting

		ui32ClkSrc = DX_IPC_HAL_GetReg(DX_REG_CLOCKSOURCE);

		if(ui32ClkSrc == 1)//live
		{

			if( u32Temp <= 23976 )
			{
				u32Temp = 24;			// 23.976 Hz
			}
			else if( u32Temp <= 24000 )
			{
				u32Temp = 24;			// 24 Hz
			}
			else if( u32Temp <= 25000 )
			{
				u32Temp = 25;	// 25 Hz
			}
			else if( u32Temp <= 29970 )
			{
				u32Temp = 30;	// 29.97 Hz
			}
			else if( u32Temp <= 30000 )
			{
				u32Temp = 30;	// 30 Hz
			}
			else if( u32Temp <= 50000 )
			{
				u32Temp = 50;	// 50 Hz
			}
			else if( u32Temp <= 59940 )
			{
				u32Temp = 60;	// 59.94 Hz
			}
			else if( u32Temp <= 60000 )
			{
				u32Temp = 60;	// 60 Hz
			}
			else
			{
				log_error("not suported frame rate for VCP, Res %d, Div%d\n",
						frm_info->ui32FrameRateRes, frm_info->ui32FrameRateDiv);
				u32Temp = 30;
			}
		}
		else if(ui32ClkSrc == 0) //not live
		{

			if( u32Temp <= 23988 )
			{
				u32Temp = 23;			// 23.976 Hz
			}
			else if( u32Temp <= 24500 )
			{
				u32Temp = 24;			// 24 Hz
			}
			else if( u32Temp <= 27485 )
			{
				u32Temp = 25;	// 25 Hz
			}
			else if( u32Temp <= 29985 )
			{
				u32Temp = 29;	// 29.97 Hz
			}
			else if( u32Temp <= 40000 )
			{
				u32Temp = 30;	// 30 Hz
			}
			else if( u32Temp <= 54970 )
			{
				u32Temp = 50;	// 50 Hz
			}
			else if( u32Temp <= 59970 )
			{
				u32Temp = 59;	// 59.94 Hz
			}
			else if( u32Temp <= 60000 )
			{
				u32Temp = 60;	// 60 Hz
			}
			else
			{
				log_error("not suported frame rate for VCP, Res %d, Div%d\n",
						frm_info->ui32FrameRateRes, frm_info->ui32FrameRateDiv);
				u32Temp = 30;
			}
		}
#ifdef BEF_CHIPREV
		if(u32Temp > 30) //60p -> left/right mode
		{
			u32Temp2 = (1 << 16);
		}
#endif
		if((frm_info->ui32DpbMapMode == 1) || (frm_info->ui32DpbMapMode == 2) || (frm_info->ui32DpbMapMode == 3))
		{
			//8bit
			u32Temp2 |= (0);
		}
		else if(frm_info->ui32DpbMapMode == 4)
		{
			//10bit
			u32Temp2 |= (1);
		}

		switch (frm_info->i32FramePackArrange)
		{
		default:
		case DISPQ_FPA_NONE:
			e3DType = DX_TYPE_3D_NONE;
			break;

		case DISPQ_FPA_CHECKERBOARD:
			e3DType = DX_TYPE_3D_FPA_CHECKERBOARD;
			break;

		case DISPQ_FPA_COLUMN_ALTERNATION:
			e3DType = DX_TYPE_3D_FPA_COLUMN_ALTERNATION;
			break;

		case DISPQ_FPA_ROW_ALTERNATION:
			e3DType = DX_TYPE_3D_FPA_ROW_ALTERNATION;
			break;

		case DISPQ_FPA_SIDE_BY_SIDE:
			e3DType = DX_TYPE_3D_FPA_SIDE_BY_SIDE;
			break;

		case DISPQ_FPA_TOP_BOTTOM:
			e3DType = DX_TYPE_3D_FPA_TOP_BOTTOM;
			break;

		case DISPQ_FPA_FRAME_ALTERNATION:
			e3DType = DX_TYPE_3D_FPA_FRAME_ALTERNATION;
			break;
		}

		u32VOffset = frm_info->ui32V_Offset;

		DX_IPC_HAL_SetReg(DX_REG_PAR, (frm_info->ui16ParW<<16) | frm_info->ui16ParH);
		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 1);	//frame decoding done
		DX_IPC_HAL_SetReg(DX_REG_DUALOUT_MODE, u32Temp2);	// dual out mode // 31~16 : 0 [even/odd] , 1 [Left/Right] 15~0 : 0 [ 8bit], 1 [10 bit]

		DX_IPC_HAL_SetReg(DX_REG_FRAME_RATE, u32Temp);
		DX_IPC_HAL_SetReg(DX_REG_3D_TYPE, e3DType);
		DX_IPC_HAL_SetReg(DX_REG_VERTICALOFFSET,u32VOffset);
		DX_IPC_Send_to_MCU0(DX_REG_FRAME_RATE);

	}
}

void DIF_HAL_SetMirrorOut(UINT8 src, UINT8 mirror_out)
{
	log_noti("DIF_HAL %d: set mirror port %d - > %d\n",
			src, g_mirror_out, mirror_out);

	if (g_mirror_out == mirror_out)
		return;

	if (src >= DEIF_NUMOF_CH && src != 0xFF)
	{
		log_error("dif %d: wrong src ch num\n", src);
		return;
	}

	if (mirror_out >= DEIF_NUMOF_CH && mirror_out != 0xFF)
	{
		log_error("dif %d: wrong out ch num\n", mirror_out);
		return;
	}

	if (g_mirror_out != 0xFF)
	{
		// already have mirror out port
		if (au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
			au8PortUse[g_mirror_out] = DEIF_HAL_IDLE_CH;
	}

	g_mirror_out = mirror_out;
	g_mirror_src = src;

	if (au8PortUse[g_mirror_out] == DEIF_HAL_IDLE_CH)
		au8PortUse[g_mirror_out] = DEIF_HAL_MIRROR_CH;
}

void DIF_HAL_SelVsync(UINT8 ch, UINT32 ui32VSyncSrc)
{
	/*
	if (ch >= DE_IF_NUM)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return;
	}*/

#ifdef ARM_USING_MMCU
	_mpipe_sel_vsync(u8DeIfCh, ui32VSyncSrc);
#endif

	DE_VDO_HAL_SetVSyncSel(ch, ui32VSyncSrc);
}

void DIF_HAL_Open(UINT8 ch)
{
	if (au8PortUse[ch] == DEIF_HAL_ACTIVE_CH)
		log_warning("DEIF_HAL %d: already used channel\n", ch);

	au8PortUse[ch] = DEIF_HAL_ACTIVE_CH;
}

void DIF_HAL_Close(UINT8 ch)
{
	if (ch == g_mirror_out)
		au8PortUse[ch] = DEIF_HAL_MIRROR_CH;
	else
		au8PortUse[ch] = DEIF_HAL_IDLE_CH;

 	DIF_HAL_Reset(ch);
}

void DIF_HAL_SetNewFrame(UINT8 ch, DISPQ_FRM_T *frm_info)
{
	S_DE_IPC_T		*ipc;

	ipc = &deifs[ch].sDeIpc;

	ipc->ui32Tiled_FrameBaseAddr = frm_info->ui32Tiled_FrameBaseAddr;
	ipc->ui32DpbMapType = frm_info->ui32DpbMapMode;
	ipc->ui32Y_FrameBaseAddr = frm_info->ui32Y_FrameBaseAddr;
	ipc->ui32C_FrameBaseAddr = frm_info->ui32C_FrameBaseAddr;
	ipc->ui32Stride = frm_info->ui32Stride;
	ipc->ui32ColorFormat = frm_info->FrameFormat;

	ipc->ui32FrameIdx = frm_info->ui32FrameIdx;
	ipc->ui16FrameRateRes = frm_info->ui32FrameRateRes;
	ipc->ui16FrameRateDiv = frm_info->ui32FrameRateDiv;
	ipc->ui32AspectRatio = frm_info->ui32AspectRatio;
	ipc->ui32H_Offset = frm_info->ui32H_Offset;
	ipc->ui32V_Offset = frm_info->ui32V_Offset;
	ipc->ui16PicWidth = frm_info->ui32PicWidth;
	ipc->ui16PicHeight = frm_info->ui32PicHeight;
	ipc->ui32FramePackArrange = frm_info->i32FramePackArrange;
	ipc->ui32LR_Order = frm_info->ui32LR_Order;
	ipc->ui16ParW = frm_info->ui16ParW;
	ipc->ui16ParH = frm_info->ui16ParH;
	ipc->ui32PTS = frm_info->ui32PTS;
	ipc->ui32UpdateInfo = 0x1;

	DE_IF_SetVcp(ch, frm_info);
}

BOOLEAN DIF_HAL_UpdateDisplay(UINT8 ch, DIF_FRM_T *frame)
{
	S_DE_IPC_T		*ipc;
	UINT32			ui32PTS;
	ipc = &deifs[ch].sDeIpc;

	ipc->ui32UpdateInfo = frame->update_info;
	ipc->ui32VdecPause = frame->freeze;
	ipc->ui32DisplayMode = frame->scan_type;

	if (ch<DEIF_NUMOF_VDO_CH)
	{
		if (ui8SeamlessMode == 1)
		{
			if (ch == u8PtsChInfo)
			{
				ui32PTS = frame->frm_info.ui32PTS;
				DX_IPC_HAL_SetReg(DX_REG_PTS, ui32PTS);
				log_debug("Ch%d PTS Value(%d)\n",ch,ui32PTS);
			}
		}
		else
		{
			ui32PTS = frame->frm_info.ui32PTS;
			DX_IPC_HAL_SetReg(DX_REG_PTS, ui32PTS);
			log_debug("Ch%d PTS Value(%d)\n",ch,ui32PTS);
		}

		// send vsync noti to MCU0
		DX_IPC_Send_to_MCU0_Noti(DX_REG_VCP_NOTI);

		DE_VDO_HAL_Update(ch, ipc, 0);
		if (ch == g_mirror_src && g_mirror_out != 0xFF &&
				au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
			DE_VDO_HAL_Update(g_mirror_out, ipc, 0);
	}

	ipc->ui32UpdateInfo = 0;

	//deif->ui32FieldCount++;

	return TRUE;
}

void DIF_HAL_Suspend(void)
{
	DE_VDO_HAL_Suspend();
	//DE_IPC_HAL_Suspend();
}

void DIF_HAL_Resume(void)
{
	//DE_IPC_HAL_Resume();
	DE_VDO_HAL_Resume();
}

void DIF_HAL_SetSeamless(UINT8 ui8Seamless)
{
	ui8SeamlessMode = ui8Seamless;

	DE_VDO_HAL_SetSeamless(ui8Seamless);
}
