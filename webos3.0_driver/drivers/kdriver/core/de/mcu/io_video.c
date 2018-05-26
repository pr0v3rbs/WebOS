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

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#ifdef USE_XTENSA
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <xtensa/hal.h>

#include <BSP/UART/stdio.h>

#include <XDE/CFG_VIDEO.h>
#include <Common/DDI_DTV.h>
#include <Common/MEM_DTV.h>

#include <XDE/VIDEO_TYPE.h>
#include <XDE/IPC_VIDEO.h>
#include <XDE/REG_DLR.h>
#include <XDE/MEM_VIDEO.h>
#include <XDE/MCU_VIDEO.h>
#include <XDE/de_kapi.h>
#include <XDE/IO_VIDEO.h>
#include <XDE/DD_VIDEO_Param.h>
#include <XDE/REG_VIDEO_MCU.h>
#include <TOP/version.h>

#ifdef INCLUDE_KDRV_FRC
#include "FRC/io_frc.h"
#endif
#ifdef INCLUDE_KDRV_FMT3D
#include "XDE/fmt3d_kapi.h"
#include "XDE/io_fmt3d.h"
#endif
#ifdef INCLUDE_KDRV_TCON
#include "XDE/io_tcon.h"
#endif
#ifdef INCLUDE_KDRV_DENC
#include "XDE/io_denc.h"
#endif
#ifdef INCLUDE_KDRV_DIMM
#include "XDE/io_dimm.h"
#endif
#ifdef INCLUDE_KDRV_PQL
#include "XDE/io_pql.h"
#endif

#else
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "de_drv.h"

#include "base_types.h"

#include "de_model.h"
#include "de_def.h"
#include "de_ipc_def.h"
#include "de_int_def.h"
#include "hal/de_hal_def.h"
#include "de_ipc.h"
#include "mcu/mcu_test.h"
#include "mcu/io_video.h"

#ifdef INCLUDE_KDRV_FRC
#include "../frc/mcu/io_frc.h"
#endif
#ifdef INCLUDE_KDRV_FMT3D
#include "mcu/io_fmt3d.h"
#endif
#ifdef INCLUDE_KDRV_TCON
#include "mcu/io_tcon.h"
#endif
#ifdef INCLUDE_KDRV_DENC
#include "mcu/io_denc.h"
#endif
#ifdef INCLUDE_KDRV_DIMM
#include "mcu/io_dimm.h"
#endif
#ifdef INCLUDE_KDRV_PQL
#include "mcu/io_pql.h"
#endif
#endif

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define PI 3.14159265
#define DE_POST_CSC_FLOAT_BIT 12

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#ifdef USE_LINUX_KERNEL
#define Str2I(pStr) simple_strtoul(pStr ,NULL ,0)
#else
#define Str2I(pStr) strtoul(pStr ,NULL ,0)
#endif

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    UINT16 indx;
    UINT16 size;
    UINT32 arry[9+6];
} VIDEO_POST_CSC_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern LX_DE_CVI_SYNC_STABILITY_T gVideoVSyncStability;
#ifdef USE_XTENSA
extern UINT32 jiffies;
extern BOOLEAN gVideoDtvIsGood[2];
#endif
extern char prtBuff[100];

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/
int DE_LoadPostCsc(int *pCsctable, int *pContrast, int *pSaturation, int *pBrightness, int *pTheta) DE_CODEDDR;

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
char gaSendBuffOfIPC[VIDEO_MSG_STR_MAX];
#endif

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
#ifdef USE_KDRV_CODES_FOR_CSC
static int DE_SetMatrixFromArray(double Dx[3][3], int Ax[9]) DE_CODEDDR;
static int DE_MultiplyMatrix(double Dx[3][3], double Ax[3][3], double Bx[3][3]) DE_CODEDDR;
static int DE_ClippingMatrix(double Ax[3][3]) DE_CODEDDR;
static int DE_SetArrayFromMatrix(int Ax[9], double Dx[3][3]) DE_CODEDDR;
#endif

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
#ifdef USE_XTENSA
static LX_DE_CVE_PARAM_T sDePathOfCve = {LX_DE_CVE_WITH_OSD, LX_DE_CVE_NTSC};
//static LX_DE_GRAB_PIXEL_T sVideoGrapPixel DE_DATADDR;
static BOOLEAN gVideoIsGrabCnt = 0;
#endif
#ifdef USE_VIDEO_TEST_MCU_IN_CPU
static LX_DE_DIS_FMT_T sVideoDisFormat;
#ifdef USE_SUPPORT_EUROPE
static LX_DE_FR_RATE_T sVideoIoFrRate;
#endif
#endif
#ifdef USE_KDRV_CODES_FOR_CSC
static VIDEO_POST_CSC_T svVideoPostCsc[1] DE_DATADDR = {
	{ 0x00 ,(9+6)  ,{ 0x00000000, } }
};
static int sVideoPostCscTable[9 + 6] DE_DATADDR = {0};
static double smdCoef[3][3] DE_DATADDR;
static double smdCntr[3][3] DE_DATADDR = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
};
static double smdSatr[3][3] DE_DATADDR = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
};
static double smdtHue[3][3] DE_DATADDR = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
};
static UINT32 sBrigth DE_DATADDR = 0;
#endif

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
#ifdef USE_XTENSA
int MCU_VIDEO_IPC_PRINT(const char *file, const int line, const char *fn, char *cliBuff, UINT32 strLen)
{
	DTV_STATUS_T ret = OK;
	int i = 0;

	do {
		if (!VIDEO_GetParameter(VIDEO_CTRL_IPC_PRINT_BET_CPU)) break;
		do {
			printq("%s:%d:%s, length(%d)", file,line,fn,strLen);
		if (!strLen) break;
			printq(" data = [%02x", cliBuff[i++]&0xff);
			while (i<strLen) {
				printq(" %02x", cliBuff[i++]&0xff);
				if (i >= strLen) break;
				if (!(i%4)) printq(",");
		}
			printq("]");
		} while (0);
		printq(" at %d\n", jiffies);
	} while (0);

	return ret;
}
#endif

#ifdef USE_VIDEO_TEST_MCU_IN_CPU
DTV_STATUS_T MCU_VIDEO_IPC_IOCTL(char *cliBuff, UINT32 strLen)
{
	DTV_STATUS_T ret = NOT_OK;
	UINT32 ipcCmd;
	UINT32 rjtCmd;
	int cmdSize = sizeof(ipcCmd);
	char  *strBuff = cliBuff;

	do {
		CHECK_KNULL(strBuff);
		memcpy(&ipcCmd, strBuff, cmdSize);
		do {
			if (GET_BITS(ipcCmd, 24, 8) == VIDEO_GetParameter(VIDEO_CTRL_IPC_PRT_MASK)) break;
			if (0x1 == (0x1 & VIDEO_GetParameter(VIDEO_CTRL_IPC_PRT_MESSAGE))) ffprintq("IPC (0x%08x) at %d\n", ipcCmd, jiffies);
			if (0x2 == (0x2 & VIDEO_GetParameter(VIDEO_CTRL_IPC_PRT_MESSAGE))) ffprintk("IPC (0x%08x) at %d\n", ipcCmd, jiffies);
#ifdef USE_DE_TRACE_IPC_MSG
			MCU_VIDEO_IPC_PRINT(__FILE__,__LINE__, __FUNCTION__, strBuff, strLen);
#endif
		} while (0);
		if ((rjtCmd = VIDEO_GetParameter(VIDEO_CTRL_IPC_RJT_MESSAGE)) > 0) {
			if (0xFFFFFFFF == rjtCmd) break;
			if (rjtCmd == ipcCmd) break;
		}
		strBuff += cmdSize;
		switch (ipcCmd) {
			case DE_IPC_DE_INIT :
				ret = MCU_DE_IO_Init(strBuff);
				break;
			case DE_IPC_GET_FIRMWARE_INFO :
				ret = MCU_DE_IO_GetFirmwareInfo(strBuff);
				break;
			case DE_IPC_GET_INPUT_WIN :
				ret = MCU_DE_IO_GetInputWin(strBuff);
				break;
			case DE_IPC_GET_OUT_WIN :
				ret = MCU_DE_IO_GetOutWin(strBuff);
				break;
			case DE_IPC_SET_INPUT_WIN :
				ret = MCU_DE_IO_SetInputWin(strBuff);
				break;
			case DE_IPC_SET_OUT_WIN :
				ret = MCU_DE_IO_SetOutWin(strBuff);
				break;
			case DE_IPC_SET_FREEZE :
				ret = MCU_DE_IO_SetFreeze(strBuff);
				break;
			case DE_IPC_SET_INPUT_SRC :
				ret = MCU_DE_IO_SetInputSrc(strBuff);
				break;
			case DE_IPC_SET_DIS_OUT :
				ret = MCU_DE_IO_SetDisOut(strBuff);
				break;
			case DE_IPC_SET_DIS_FMT :
				ret = MCU_DE_IO_SetDisFmt(strBuff);
				break;
			case DE_IPC_SET_MULTI_WIN :
				ret = MCU_DE_IO_SetMultiWin(strBuff);
				break;
			case DE_IPC_MULTI_WIN_ENALBE :
				ret = MCU_DE_IO_MultiWinEnable(strBuff);
				break;
			case DE_IPC_SET_CVI_SRC_TYPE :
				ret = MCU_DE_IO_SetCviSrcType(strBuff);
				break;
			case DE_IPC_SET_CVI_CSC :
				ret = MCU_DE_IO_SetCviCsc(strBuff);
				break;
			case DE_IPC_SET_POST_CSC :
				ret = MCU_DE_IO_SetPostCsc(strBuff);
				break;
			case DE_IPC_GET_MVI_COLORIMETRY :
				ret = MCU_DE_IO_GetMviColorimetry(strBuff);
				break;
			case DE_IPC_SET_ZLIST :
				ret = MCU_DE_IO_SetZList(strBuff);
				break;
			case DE_IPC_GRAB_PIXEL				:
				ret = MCU_DE_IO_GrabPixel(strBuff);
				break;
			case DE_IPC_GET_MVI_TIMING_INFO		:
				ret = MCU_DE_IO_GetMviTimingInfo(strBuff);
				break;
			case DE_IPC_SET_DEINTERLACE		:
				ret = MCU_DE_IO_SetDeinterlace(strBuff);
				break;
			case DE_IPC_GET_CAPTURE_WIN		:
				ret = MCU_DE_IO_GetCaptureWin(strBuff);
				break;
			case DE_IPC_SET_CAPTURE_WIN		:
				ret = MCU_DE_IO_SetCaptureWin(strBuff);
				break;
			//case DE_IPC_SET_3D_SRC_MUX		:
			//    ret = MCU_DE_IO_Set3DSrcMux(strBuff);
			//    break;
			case DE_IPC_SET_PRE3D_IN_MODE		:
				ret = MCU_DE_IO_SetPre3DInMode(strBuff);
				break;
			//case DE_IPC_SET_PRE3D_OUT_MODE		:
			//    ret = MCU_DE_IO_SetPre3DOutMode(strBuff);
			//    break;
			//case DE_IPC_SET_PRE3D_IN_CH		:
			//    ret = MCU_DE_IO_SetPre3DInCH(strBuff);
			//    break;
			//case DE_IPC_SET_PRE3D_ENABLE		:
			//    ret = MCU_DE_IO_SetPre3DEnable(strBuff);
			//    break;
			case DE_IPC_SET_3D_OPR_MODE		:
				ret = MCU_DE_IO_Set3DOprMode(strBuff);
				break;
			case DE_IPC_SET_PWM		:
				ret = MCU_DE_IO_SetPwm(strBuff);
				break;
			case DE_IPC_SET_CVE		:
				ret = MCU_DE_IO_SetCve(strBuff);
				break;
			case DE_IPC_SET_VCS		:
				ret = MCU_DE_IO_SetVcs(strBuff);
				break;
			case DE_IPC_SET_EDGE_CROP		:
				ret = MCU_DE_IO_SetEdgeCrop(strBuff);
				break;
#ifdef USE_SUPPORT_EUROPE
			case DE_IPC_GET_OUT_FR_RATE :
				ret = MCU_DE_IO_GetOutFrRate(strBuff);
				break;
			case DE_IPC_SET_FR_RATE :
				ret = MCU_DE_IO_SetFrRate(strBuff);
				break;
#endif
#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
			case DE_IPC_SET_BG_COLOR :
				ret = MCU_DE_IO_SetBgColor(strBuff);
				break;
			case DE_IPC_SET_WIN_BLANK :
				ret = MCU_DE_IO_SetWinBlank(strBuff);
				break;
			case DE_IPC_SET_IRE_PATTERN			:
			case DE_IPC_SET_SPREAD				:
			case DE_IPC_REG_RD					:
			case DE_IPC_REG_WD					:
			case DE_IPC_OIF_INIT				:
			case DE_IPC_OIF_SET_LVDS_OUTPUT		:
			case DE_IPC_CMD_MAX					:
				break;
#endif
#ifdef INCLUDE_KDRV_FMT3D
			case FMT3D_IPC_INIT :
				ret = MCU_FMT3D_IO_Init(strBuff);
				break;
			case FMT3D_IPC_SET_PROCESS_MODE :
				ret = MCU_FMT3D_IO_SetProcessMode(strBuff);
				break;
			case FMT3D_IPC_GET_PROCESS_MODE :
				ret = MCU_FMT3D_IO_GetProcessMode(strBuff);
				break;
			case FMT3D_IPC_SET_IN_IMG_INFO :
				ret = MCU_FMT3D_IO_SetInImgInfo(strBuff);
				break;
			case FMT3D_IPC_GET_IN_IMG_INFO :
				ret = MCU_FMT3D_IO_GetInImgInfo(strBuff);
				break;
			case FMT3D_IPC_GET_OUT_IMG_INFO :
				ret = MCU_FMT3D_IO_GetOutImgInfo(strBuff);
				break;
			case FMT3D_IPC_SET_RUNMODE :
				ret = MCU_FMT3D_IO_SetRunmode(strBuff);
				break;
			case FMT3D_IPC_TOGGLE_LR_IN_ORDER :
				ret = MCU_FMT3D_IO_ToggleLrInOrder(strBuff);
				break;
			case FMT3D_IPC_TOGGLE_LR_OUT_ORDER :
				ret = MCU_FMT3D_IO_ToggleLrOutOrder(strBuff);
				break;
			case FMT3D_IPC_SET_IN_IMG_FMT :
				ret = MCU_FMT3D_IO_SetInImgFmt(strBuff);
				break;
			case FMT3D_IPC_SET_DIS_FMT :
				ret = MCU_FMT3D_IO_SetDisFmt(strBuff);
				break;
			case FMT3D_IPC_SET_IN_FR_RATE :
				ret = MCU_FMT3D_IO_SetInFrRate(strBuff);
				break;
			case FMT3D_IPC_SET_OUT_FR_RATE :
				ret = MCU_FMT3D_IO_SetOutFrRate(strBuff);
				break;
			case FMT3D_IPC_SET_CONVERGENCE :
				ret = MCU_FMT3D_IO_SetConvergence(strBuff);
				break;
			case FMT3D_IPC_SET_IMG_DOUBLE_PIC :
				ret = MCU_FMT3D_IO_SetImgDoublePic(strBuff);
				break;
			case FMT3D_IPC_SET_SAMPLE_MODE :
				ret = MCU_FMT3D_IO_SetSampleMode(strBuff);
				break;
			case FMT3D_IPC_SET_SRC_MUX :
				ret = MCU_FMT3D_IO_SetSrcMux(strBuff);
				break;
			case FMT3D_IPC_SET_IMG_SCAN_TYPE :
				ret = MCU_FMT3D_IO_SetImgScanType(strBuff);
				break;
			case FMT3D_IPC_SET_FRC_OUT_TYPE :
				ret = MCU_FMT3D_IO_SetFrcOutType(strBuff);
				break;
			case FMT3D_IPC_SET_FRC_OUT_LR_ORDER :
				ret = MCU_FMT3D_IO_SetFrcOutOrder(strBuff);
				break;
			case FMT3D_T3_IPC_SET_CONVERGENCE :
				ret = MCU_FMT3D_T3_IO_SetConvergence(strBuff);
				break;
			case FMT3D_T3_IPC_SET_DEPTH_CTRL :
				ret = MCU_FMT3D_T3_IO_SetDepthCtrl(strBuff);
				break;
			case FMT3D_IPC_SET_FPR_FHD :
				ret = MCU_FMT3D_IO_SetFprFhd(strBuff);
				break;
#endif
#ifdef INCLUDE_KDRV_TCON
			case TCON_IPC_INIT :
				ret = MCU_TCON_IO_Init(strBuff);
				break;
			case TCON_IPC_SET_ODC_TABLE :
				ret = MCU_TCON_IO_SetOdcTable(strBuff);
				break;
			case TCON_IPC_CONTROL :
				ret = MCU_TCON_IO_Control(strBuff);
				break;
#endif
#ifdef INCLUDE_KDRV_DENC
			case DENC_IPC_INIT :
				ret = MCU_DENC_IO_Init(strBuff);
				break;
			case DENC_IPC_ONOFF :
				ret = MCU_DENC_IO_OnOff(strBuff);
				break;
			case DENC_IPC_SET_OUTPUT_VIDEO_STD :
				ret = MCU_DENC_IO_SetOutPutVideoStd(strBuff);
				break;
			case DENC_IPC_SET_SOURCE :
				ret = MCU_DENC_IO_SetSource(strBuff);
				break;
			case DENC_IPC_TTX_ENABLE :
				ret = MCU_DENC_IO_TtxEnable(strBuff);
				break;
			case DENC_IPC_WSS_ENABLE :
				ret = MCU_DENC_IO_WssEnable(strBuff);
				break;
			case DENC_IPC_VPS_ENABLE :
				ret = MCU_DENC_IO_VpsEnable(strBuff);
				break;
			case DENC_IPC_SET_TTX_DATA :
				ret = MCU_DENC_IO_SetTtxData(strBuff);
				break;
			case DENC_IPC_SET_WSS_DATA :
				ret = MCU_DENC_IO_SetWssData(strBuff);
				break;
			case DENC_IPC_SET_VPS_DATA :
				ret = MCU_DENC_IO_SetVpsData(strBuff);
				break;
			case DENC_IPC_COLORBAR_ENABLE :
				ret = MCU_DENC_IO_ColorBarEnable(strBuff);
				break;
#endif
#ifdef INCLUDE_KDRV_DIMM
			case DIMM_IPC_INIT :
				ret = MCU_DIMM_IO_Init(strBuff);
				break;
			case DIMM_IPC_LOCALDIMMING_CONTROL :
				ret = MCU_DIMM_IO_LocaldimmingControl(strBuff);
				break;
			case DIMM_IPC_STORE_DEMO :
				ret = MCU_DIMM_IO_StoreDemo(strBuff);
				break;
			case DIMM_IPC_LINE_DEMO :
				ret = MCU_DIMM_IO_LineDemo(strBuff);
				break;
			case DIMM_IPC_CONTROL_COMPENSATION :
				ret = MCU_DIMM_IO_ControlCompensation(strBuff);
				break;
			case DIMM_IPC_LUT_CONTROL :
				ret = MCU_DIMM_IO_LutControl(strBuff);
				break;
			case DIMM_IPC_STORE_CONTROL :
				ret = MCU_DIMM_IO_StoreControl(strBuff);
				break;
			case DIMM_IPC_REG_RD :
			case DIMM_IPC_REG_WR :
				break;
#ifdef USE_DIMM_TEMPORAL_DEFINITION
			case DIMM_IPC_GET_VERSION :
				ret = MCU_DIMM_IO_GetVersion(strBuff);
				break;
			case DIMM_IPC_DBG_CONTROL :
				ret = MCU_DIMM_IO_DbgControl(strBuff);
				break;
			case DIMM_IPC_BLU_CONTROL :
				ret = MCU_DIMM_IO_BluControl(strBuff);
				break;
#endif
			case DIMM_IPC_WBCOLOR_CTRL :
				break;
#endif
#ifdef INCLUDE_KDRV_PQL
			case PQL_IPC_INIT :
				ret = MCU_PQL_IO_Init(strBuff);
				break;
			case PQL_IPC_SET_YUV2RGB :
				ret = MCU_PQL_IO_SetYuv2Rgb(strBuff);
				break;
			case PQL_IPC_SET_RGB2YUV :
				ret = MCU_PQL_IO_SetRgb2Yuv(strBuff);
				break;
			case PQL_IPC_SET_CONTRAST :
				ret = MCU_PQL_IO_SetContrast(strBuff);
				break;
			case PQL_IPC_SET_BRIGHTNESS :
				ret = MCU_PQL_IO_SetBrightness(strBuff);
				break;
			case PQL_IPC_SET_SATURATION :
				ret = MCU_PQL_IO_SetSaturation(strBuff);
				break;
			case PQL_IPC_SET_HUE :
				ret = MCU_PQL_IO_SetHue(strBuff);
				break;
			case PQL_IPC_SET_SHARPNESS :
				ret = MCU_PQL_IO_SetSharpness(strBuff);
				break;
			case PQL_IPC_SET_COLORTEMP :
			case PQL_IPC_SET_PARAMTABLE :
				break;
			case PQL_IPC_SET_TPG :
				ret = MCU_PQL_IO_SetTpg(strBuff);
				break;
			case PQL_IPC_SET_TNR :
			case PQL_IPC_SET_IPC :
			case PQL_IPC_SET_DNR :
				break;
			case PQL_IPC_SET_TNR_ENABLE :
				ret = MCU_PQL_IO_SetTnrEnable(strBuff);
				break;
			case PQL_IPC_SET_FILMMODE    :
			case PQL_IPC_SET_SHPENABLE	 :
			case PQL_IPC_SET_PC_ENABLE	 :
			case PQL_IPC_SET_GAMMALUT	 :
			case PQL_IPC_GET_GAMMALUT	 :
			case PQL_IPC_SET_COLORFILTER :
			case PQL_IPC_SET_EDGEENHANCE :
			case PQL_IPC_SET_XVYCC		 :
				break;
#endif
			default :
#ifdef INCLUDE_KDRV_FRC
				MCU_FRC_IPC_IOCTL(cliBuff, strLen);
#endif
				break;
		}
	} while (0);

	return ret;
}

DTV_STATUS_T MCU_DE_IO_Init(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	VIDEO_SetParameter(VIDEO_CTRL_DSP_ZOOM_FROM_MVI, TRUE);
	VIDEO_SetParameter(VIDEO_CTRL_DSP_ZOOM_FROM_CPU, TRUE);
#if 0
    LX_DE_PANEL_TYPE_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		ret = MCU_SetPanelType(&stParams);
		if (ret) break;
		ret = VIDEO_SetParameter(VIDEO_CTRL_BLUE_BAR_IF_NO_SIGNAL, FALSE);
#ifdef USE_DE_BLUE_BAR
		if (ret) break;
		ret = VIDEO_SetBluBar(FALSE);
#endif
	} while (0);
#endif
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetFirmwareInfo(char *strBuff)
{
	DTV_STATUS_T ret;
    LX_DE_FIRMWARE_INFO_T stParams;
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char date[] = __DATE__;
#ifdef USE_LINUX_KERNEL
	char **argv = NULL;
#else
	char *argv[20];
#endif
	int argc;
	int i;

	do {
		stParams.version = 0x100;

		VIDEO_GetVersion(date);
		//ffprintk("%s\n", date);
#ifdef USE_DE_TRACE_IPC_MSG
		MCU_VIDEO_IPC_PRINT(__FILE__,__LINE__, __FUNCTION__, date, strlen(date));
#endif
		memset(&stParams.date, 0, sizeof(stParams.date));
#ifdef USE_LINUX_KERNEL
		argv = MCU_VIDEO_GetArgFromStr(date, &argc);
#else
		argc = MCU_VIDEO_ParseFromStr(date, argv);
#endif
		if (argc != 3) {
			ret = NOT_OK;
			break;
		}
		stParams.date.year = Str2I(argv[2]);
		stParams.date.day  = Str2I(argv[1]);
		for (i = 0; i < 12; i++) {
			if (!strcmp(argv[0], months[i])) {
				stParams.date.month = i + 1;
				break;
			}
		}
		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#ifdef USE_LINUX_KERNEL
	if (argv) argv_free(argv);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetInputSrc(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA

#if 0
	LX_DE_SET_INPUT_T stParams;
	VIDEO_WIN_TYPE_T winType;
	VIDEO_WIN_PATH_TYPE_T winPath;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.win_id) {
			case LX_DE_WIN_MAIN :
				winType = VIDEO_WIN_DSP;
				break;
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
		switch (stParams.inputSrc) {
			case LX_CVI_A :
				winPath = VIDEO_WIN_PATH_CAPA;
				break;
			case LX_CVI_B :
				winPath = VIDEO_WIN_PATH_CAPB;
				break;
			case LX_MVI_VDEC :
				winPath = VIDEO_WIN_PATH_MVI;
				break;
			case LX_MVI_CPU :
				winPath = VIDEO_WIN_PATH_VCS;
				break;
			default :
				BREAK_WRONG(stParams.inputSrc);
		}
		if (ret) break;
		ret = VIDEO_UpdateWinPath(winType, winPath);
	} while (0);
#endif

#endif
	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetCviSrcType(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_CVI_SRC_TYPE_T stParams;

	VIDEO_WIN_PATH_TYPE_T winPath = VIDEO_WIN_PATH_CAPA;
	VIDEO_FORMAT_TYPE_T videoFormat;

	VIDEO_WIN_TYPE_T winType;
	VIDEO_WIN_PATH_TYPE_T cviPath;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.cvi_channel) {
			case LX_DE_CVI_CH_B :
				winPath = VIDEO_WIN_PATH_CAPB;
			case LX_DE_CVI_CH_A :
				ret = VIDEO_GetFormatWithPath(winPath, &videoFormat);
				if (ret) break;
				videoFormat.sampling		 = stParams.sampling;
				videoFormat.size.hSize	   = stParams.size_offset.hsize;
#ifdef USE_CVI_SAMPLE_MODE
				switch (videoFormat.sampling) {
					case LX_DE_CVI_QUAD_SAMPLING :
						videoFormat.size.hSize		 >>= 2;
						break;
					case LX_DE_CVI_DOUBLE_SAMPLING :
						videoFormat.size.hSize		 >>= 1;
						break;
					case LX_DE_CVI_NORMAL_SAMPLING :
					default :
						break;
				}
#endif
				videoFormat.size.vSize	   = stParams.size_offset.vsize;
				videoFormat.offset.hOffset = stParams.size_offset.hoffset;
				videoFormat.offset.vOffset = stParams.size_offset.voffset;
				videoFormat.aspect		   = stParams.aspect;
				videoFormat.scan		   = stParams.scan;
				videoFormat.size.bpp		 = (LX_DE_CVI_10BIT == stParams.bit_res)?10:8;
				videoFormat.sample.cvi		 = (LX_DE_CVI_444 == stParams.c_sample)?VIDEO_CHROM_444:VIDEO_CHROM_422;
				do {
					if (VIDEO_SCAN_PROGRESSIVE == stParams.scan) break;
					if (LX_DE_SCALER_444 != stParams.sc_c_sample) break;
					stParams.sc_c_sample = LX_DE_SCALER_422;
				}while (0);
				videoFormat.sample.smux		 = stParams.sc_c_sample;
				videoFormat.sample.mscr		 = stParams.sc_c_sample;
				videoFormat.sample.c3df	     = VIDEO_CHROM_422;
				videoFormat.sample.m3df	     = VIDEO_CHROM_422;

				videoFormat.de_mode			 = stParams.de_mode;
				ret = VIDEO_SetFormatWithPath(winPath, &videoFormat);
#ifdef USE_DE_BIT_8
				if (ret) break;
				ret = VIDEO_SetBitsOfMSCR(VIDEO_BITS_MAX, 8);
#else
				if (ret) break;
				ret = VIDEO_SetBitsOfMSCR(VIDEO_BITS_MAX, videoFormat.size.bpp);
#endif
				break;
			default :
				BREAK_WRONG(stParams.cvi_channel);
		}
		if (ret) break;
		gVideoVSyncStability = stParams.stability;
		for (winType=VIDEO_WIN_DSP; winType<VIDEO_WIN_MAX; winType++) {
			if (ret) break;
			ret = VIDEO_GetWinPathWithType(winType, &cviPath);
			if (cviPath != winPath) continue;
			if (ret) break;
			ret = VIDEO_UpdateWinPath(winType, winPath);
		}
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetCviCsc(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_CVI_CSC_T stParams;
	VIDEO_CVI_PATH_T cviPath;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.cvi_channel) {
			case LX_DE_CVI_CH_A :
				cviPath = VIDEO_CVI_PATH_A;
				break;
			case LX_DE_CVI_CH_B :
				cviPath = VIDEO_CVI_PATH_B;
				break;
			default :
				BREAK_WRONG(stParams.cvi_channel);
		}
		if (ret) break;
		ret = VIDEO_SetCscBypassOfCVI(cviPath, stParams.isEnable);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetPostCsc(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
#ifdef USE_KDRV_CODES_FOR_CSC
	LX_DE_POST_CSC_T stParams;
	int cscTable[9+6];
	int i, inx = 0;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));

		for (i=0;i<9;i++) cscTable[inx++] = GET_SEXT(stParams.CSC_matrix[i], 0, 16);
		for (i=0;i<6;i++) cscTable[inx++] = GET_SEXT(stParams.CSC_offset[i], 0, 16);

		ret = DE_LoadPostCsc(cscTable, NULL, NULL, NULL, NULL);
	} while (0);
#endif
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetMviColorimetry(char *strBuff)
{
	DTV_STATUS_T ret = OK;
	LX_DE_MVI_COLORIMETRY_T stParams;
#ifdef USE_XTENSA
	VIDEO_WIN_TYPE_T winType;
	VIDEO_WIN_PATH_TYPE_T winPath;
	VIDEO_RECT_T vIn;

	memcpy(&stParams.win_id, strBuff, sizeof(LX_DE_WIN_ID_T));
	do {
		switch (stParams.win_id) {
			case LX_DE_WIN_MAIN :
				winType = VIDEO_WIN_DSP;
				break;
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
		if (ret) break;
		ret = VIDEO_GetWinPathWithType(winType, &winPath);
		if (ret) break;
        switch (winPath) {
            case VIDEO_WIN_PATH_MVI :
            case VIDEO_WIN_PATH_USB :
            case VIDEO_WIN_PATH_CPU :
            case VIDEO_WIN_PATH_VCS :
				ret = VIDEO_GetInfoOfWCP(winType, &vIn, NULL, NULL, NULL);
				if (ret) break;
				stParams.colorimetry = (720 <= vIn.h) ? LX_DE_COLOR_709 : LX_DE_COLOR_601;
				break;
			default :
				BREAK_WRONG(winPath);
		}
		if (ret) break;
		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetInputWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_WIN_DIMENSION_T stParams;

	VIDEO_WIN_TYPE_T winType = VIDEO_WIN_DSP;
	VIDEO_RECT_T wIn;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.win_id) {
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
			case LX_DE_WIN_MAIN :
				memcpy(&wIn, &stParams.rect, sizeof(wIn));
				ret = VIDEO_SetInfoOfWCP(winType, NULL, &wIn, NULL, NULL);
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetInputWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
	LX_DE_WIN_DIMENSION_T stParams;
#ifdef USE_XTENSA

	VIDEO_WIN_TYPE_T winType = VIDEO_WIN_DSP;
	VIDEO_RECT_T wIn;

	do {
		memcpy(&stParams.win_id, strBuff, sizeof(LX_DE_WIN_ID_T));
		switch (stParams.win_id) {
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
			case LX_DE_WIN_MAIN :
				ret = VIDEO_GetInfoOfWCP(winType, NULL, &wIn, NULL, NULL);
				if (ret) break;
				memcpy(&stParams.rect, &wIn, sizeof(wIn));
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
		if (ret) break;
		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetOutWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_WIN_DIMENSION_T stParams;

	VIDEO_WIN_TYPE_T winType = VIDEO_WIN_DSP;
	VIDEO_RECT_T wOut;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.win_id) {
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
			case LX_DE_WIN_MAIN :
				memcpy(&wOut, &stParams.rect, sizeof(wOut));
				ret = VIDEO_SetInfoOfWCP(winType, NULL, NULL, &wOut, NULL);
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetOutWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
	LX_DE_WIN_DIMENSION_T stParams;
#ifdef USE_XTENSA

	VIDEO_WIN_TYPE_T winType = VIDEO_WIN_DSP;
	VIDEO_RECT_T wOut;

	do {
		memcpy(&stParams.win_id, strBuff, sizeof(LX_DE_WIN_ID_T));
		switch (stParams.win_id) {
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
			case LX_DE_WIN_MAIN :
				ret = VIDEO_GetInfoOfWCP(winType, NULL, NULL, &wOut, NULL);
				if (ret) break;
				memcpy(&stParams.rect, &wOut, sizeof(wOut));
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
		if (ret) break;
		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetDisOut(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	VIDEO_RECT_T wOut;
	VIDEO_WIN_TYPE_T winType = VIDEO_WIN_DSP;
	static VIDEO_RECT_T backupWinOut;
	static BOOLEAN bDispOut = TRUE;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		if (bDispOut == stParams) break;
		bDispOut = stParams;
		if (!stParams) {
			ret = VIDEO_GetInfoOfWCP(winType, NULL, NULL, &backupWinOut, NULL);
			if (ret) break;
			memset(&wOut, 0, sizeof(wOut));
			ret = VIDEO_SetInfoOfWCP(winType, NULL, NULL, &wOut, NULL);
		} else {
			ret = VIDEO_SetInfoOfWCP(winType, NULL, NULL, &backupWinOut, NULL);
		}
	} while (0);
#endif

	return ret;
}

#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
DTV_STATUS_T MCU_DE_IO_SetBgColor(char *strBuff)
{
	DTV_STATUS_T ret = NOT_OK;
#ifdef USE_XTENSA
	LX_DE_COLOR_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}
#endif

DTV_STATUS_T MCU_DE_IO_SetDisFmt(char *strBuff)
{
	DTV_STATUS_T ret = OK;

	memcpy(&sVideoDisFormat, strBuff, sizeof(sVideoDisFormat));
	ret = MCU_SetPanelType(&sVideoDisFormat);

	return ret;
}

#ifdef USE_SUPPORT_EUROPE
DTV_STATUS_T MCU_DE_IO_SetFrRate(char *strBuff)
{
	DTV_STATUS_T ret = OK;

	do {
		memcpy(&sVideoIoFrRate, strBuff, sizeof(sVideoIoFrRate));
		sVideoDisFormat.fr_rate = sVideoIoFrRate.fr_rate;
		ret = VIDEO_SetParameter(VIDEO_CTRL_TRACE_DCO_FCW, (sVideoIoFrRate.isForceFreeRun?FALSE:TRUE));
		if (ret) break;
		ret = MCU_SetPanelType(&sVideoDisFormat);
	} while (0);

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetOutFrRate(char *strBuff)
{
	sVideoIoFrRate.fr_rate = sVideoDisFormat.fr_rate;
	memcpy(gaSendBuffOfIPC, &sVideoIoFrRate, sizeof(sVideoIoFrRate));

	return VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(sVideoIoFrRate));
}
#endif

DTV_STATUS_T MCU_DE_IO_SetFreeze(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA

#if 0
	LX_DE_SET_FREEZE_T stParams;
	VIDEO_FRAME_TYPE_T frameType = VIDEO_FRAME_MW_SCR;

	memcpy(&stParams.win_id, strBuff, sizeof(stParams));
	switch (stParams.win_id) {
		case LX_DE_WIN_SUB :
			frameType = VIDEO_FRAME_SW_PIP;
		case LX_DE_WIN_MAIN :
			ret = VIDEO_SetFreezeFrame(frameType, stParams.bEnable);
			break;
		default :
			BREAK_WRONG(stParams.win_id);
	}
#endif

#endif
	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetZList(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_ZLIST_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetMultiWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_MULTI_WIN_TYPE_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_MultiWinEnable(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	VIDEO_WCP_TYPE_T lVideoWCP;
	static VIDEO_WCP_TYPE_T lVideoWCP_Old;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
        ret = VIDEO_GetWinOprMode(&lVideoWCP);
		lVideoWCP_Old = lVideoWCP;
		if (ret) break;
        switch (lVideoWCP) {
			case VIDEO_OPR_DSP_NOP :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_PIP;
				break;
			case VIDEO_OPR_DSP_VCR :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_PIP_VCR;
				break;
			case VIDEO_OPR_DSP_DVR :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_PIP_DVR;
				break;
			case VIDEO_OPR_DSP_PIP :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_NOP;
				break;
			case VIDEO_OPR_PIP_VCR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_VCR;
				break;
			case VIDEO_OPR_PIP_DVR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_DVR;
				break;
			default :
				break;
		}
		if (lVideoWCP_Old == lVideoWCP) break;
		ret = VIDEO_SetWinOprMode(lVideoWCP);

        switch (lVideoWCP) {
			case VIDEO_OPR_DSP_PIP :
			case VIDEO_OPR_PIP_VCR :
			case VIDEO_OPR_PIP_DVR :
				if (VIDEO_PIPSYNC_OFF != VIDEO_GetSyncOfPIP(TRUE)) break;
				VIDEO_SetSyncOfPIP(VIDEO_GetSyncOfPIP(FALSE));
			default :
				break;
		}
	} while (0);
#endif

	return ret;
}

#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
DTV_STATUS_T MCU_DE_IO_SetWinBlank(char *strBuff)
{
	DTV_STATUS_T ret = NOT_OK;
#ifdef USE_XTENSA
	LX_DE_SET_WIN_BLANK_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}
#endif

DTV_STATUS_T MCU_DE_IO_GrabPixel(char *strBuff)
{
#ifdef USE_XTENSA
	memcpy(&sVideoGrapPixel, strBuff, sizeof(sVideoGrapPixel));
	gVideoIsGrabCnt = 1;
#endif
	return OK;
}

DTV_STATUS_T DE_DDR_GrabPixelOfCVI(void)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
    VIDEO_FRAME_TYPE_T frameType;
	UINT32 rowY;
	UINT32 rowC;

	switch (gVideoIsGrabCnt) {
		case 0 : break;
		case 1 :
			gVideoIsGrabCnt++;
			ret = VIDEO_SetBypassCAP(FALSE);
			if (ret) break;
			ret = VIDEO_SetIndexOfFrame(frameType, VIDEO_FRAME_INX_MAX, 0, 0);
			break;
		case 2 :
		case 3 :
			gVideoIsGrabCnt++;
			break;
		case 4 :
			gVideoIsGrabCnt++;
			switch (sVideoGrapPixel.region.win_id) {
			case LX_DE_WIN_MAIN :
			case LX_DE_WIN_SUB :
				frameType = VIDEO_FRAME_MW_CAP;
				break;
			default :
					BREAK_WRONG(sVideoGrapPixel.region.win_id);
		}
		if (ret) break;
        ret = VIDEO_GetGrabAddr(frameType, &rowY, &rowC);
		if (ret) break;
			sVideoGrapPixel.region.realPixelGrabW = rowY;
			sVideoGrapPixel.region.realPixelGrabH = rowC;
			//ret = DE_DDR_GrabPixel(&sVideoGrapPixel);
        if (ret) break;
        ret = VIDEO_SetIndexOfFrame(frameType, VIDEO_FRAME_INX_MAX, 9, 9);
        if (ret) break;
		ret = VIDEO_SetBypassCAP(TRUE);
        if (ret) break;
			memcpy(gaSendBuffOfIPC, &sVideoGrapPixel, sizeof(sVideoGrapPixel));
		default :
			gVideoIsGrabCnt = 0;
			ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(sVideoGrapPixel));
			break;
	}
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetMviTimingInfo(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#if 0
	LX_DE_MVI_TIMING_INFO_T stParams;
#ifdef USE_XTENSA
	VIDEO_WIN_TYPE_T winType;
	VIDEO_WIN_PATH_TYPE_T winPath;
	VIDEO_FORMAT_TYPE_T lVideoFormat;
	BOOLEAN isGoodVideo;

	do {
		memcpy(&stParams.win_id, strBuff, sizeof(LX_DE_WIN_ID_T));
		switch (stParams.win_id) {
			case LX_DE_WIN_MAIN :
				winType = VIDEO_WIN_DSP;
				break;
			case LX_DE_WIN_SUB :
				winType = VIDEO_WIN_PIP;
				break;
			default :
				BREAK_WRONG(stParams.win_id);
		}
		if (ret) break;
		isGoodVideo = gVideoDtvIsGood[winType];
		ret = VIDEO_GetWinPathWithType(winType, &winPath);
		if (ret) break;
        switch (winPath) {
            case VIDEO_WIN_PATH_MVI :
            case VIDEO_WIN_PATH_USB :
            case VIDEO_WIN_PATH_CPU :
            case VIDEO_WIN_PATH_VCS :
				break;
			default :
				BREAK_WRONG(winPath);
		}
		if (ret) break;
		ret = VIDEO_GetWinFormt(winType, &lVideoFormat);
		if (ret) break;
		stParams.mpeg_time.h_size  = (!isGoodVideo)?0:lVideoFormat.size.hSize;
		stParams.mpeg_time.v_size  = (!isGoodVideo)?0:lVideoFormat.size.vSize;
		stParams.mpeg_time.h_start = (!isGoodVideo)?0:lVideoFormat.offset.hOffset;
		stParams.mpeg_time.v_start = (!isGoodVideo)?0:lVideoFormat.offset.vOffset;
		stParams.mpeg_time.isProg  = (VIDEO_SCAN_PROGRESSIVE == lVideoFormat.scan)?LX_DE_PROGRESSIVE:LX_DE_INTERLACED;
		stParams.mpeg_time.v_freq  = lVideoFormat.frate;

		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));
#endif
	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetDeinterlace(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		ret = VIDEO_SetIPCOn(stParams);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_GetCaptureWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
	LX_DE_CVI_CAPTURE_WIN_T stParams;
#ifdef USE_XTENSA
	VIDEO_WIN_PATH_TYPE_T   winPath;
	VIDEO_FORMAT_TYPE_T     videoFormat;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.cvi_channel) {
			case LX_DE_CVI_CH_A :
				winPath = VIDEO_WIN_PATH_CAPA;
				break;
			case LX_DE_CVI_CH_B :
				winPath = VIDEO_WIN_PATH_CAPB;
				break;
			default :
				BREAK_WRONG(stParams.cvi_channel);
		}
		if (ret) break;
		ret = VIDEO_GetFormatWithPath(winPath, &videoFormat);
		if (ret) break;

		stParams.size_offset.hsize   = videoFormat.size.hSize;
		stParams.size_offset.vsize   = videoFormat.size.vSize;
		stParams.size_offset.hoffset = videoFormat.offset.hOffset;
		stParams.size_offset.voffset = videoFormat.offset.vOffset;

		memcpy(gaSendBuffOfIPC, &stParams, sizeof(stParams));
	} while (0);
#endif
	ret = VIDEO_MCU_PutData(gaSendBuffOfIPC, sizeof(stParams));

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetCaptureWin(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_CVI_CAPTURE_WIN_T stParams;
	VIDEO_WIN_PATH_TYPE_T   winPath;
	VIDEO_WIN_PATH_TYPE_T   cviPath;
	VIDEO_WIN_TYPE_T 		winType;
	VIDEO_FORMAT_TYPE_T     videoFormat;
	VIDEO_RECT_T 			vIn;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams.cvi_channel) {
			case LX_DE_CVI_CH_A :
				winPath = VIDEO_WIN_PATH_CAPA;
				break;
			case LX_DE_CVI_CH_B :
				winPath = VIDEO_WIN_PATH_CAPB;
				break;
			default :
				BREAK_WRONG(stParams.cvi_channel);
		}
		if (ret) break;
		ret = VIDEO_GetFormatWithPath(winPath, &videoFormat);
		if (ret) break;
		videoFormat.size.hSize	   = stParams.size_offset.hsize;
		videoFormat.size.vSize	   = stParams.size_offset.vsize;
		videoFormat.offset.hOffset = stParams.size_offset.hoffset;
		videoFormat.offset.vOffset = stParams.size_offset.voffset;
		ret = VIDEO_SetFormatWithPath(winPath, &videoFormat);
		if (ret) break;
		memcpy(&vIn, &stParams.size_offset, sizeof(vIn));
		for (winType=VIDEO_WIN_DSP; winType<VIDEO_WIN_MAX; winType++) {
			if (ret) break;
			ret = VIDEO_GetWinPathWithType(winType, &cviPath);
			if (cviPath != winPath) continue;
			if (ret) break;
			ret = VIDEO_SetInfoOfWCP(winType, &vIn, NULL, NULL, NULL);
		}
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetPre3DInMode(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_PRE3D_IN_MODE_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_Set3DOprMode(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_3D_OPR_MODE_T stParams;
	VIDEO_3DF_MODE_T mode3DF;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams) {
			case LX_3D_OFF :
				mode3DF = VIDEO_3DF_MODE_OFF;
				break;
			case LX_3D_ON :
				mode3DF = VIDEO_3DF_MODE_NORMAL;
				break;
			case LX_3D_BYPASS :
				mode3DF = VIDEO_3DF_MODE_BYPASS;
				break;
			case LX_3D_2DTO3D :
				mode3DF = VIDEO_3DF_MODE_2DTO3D;
				break;
			case LX_3D_TB :
				mode3DF = VIDEO_3DF_MODE_3DBYT3;
				break;
			default :
				BREAK_WRONG(stParams);
		}
		if (ret) break;
		VIDEO_SetMode3DF(mode3DF);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetPwm(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_PWM_PARAM_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetCve(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	VIDEO_VCRSYNC_TYPE_T dType;

	do {
		memcpy(&sDePathOfCve, strBuff, sizeof(sDePathOfCve));
		switch (sDePathOfCve.std) {
			case LX_DE_CVE_NTSC :
				dType = VIDEO_VCRSYNC_720X480I;
				break;
			case LX_DE_CVE_PAL :
			case LX_DE_CVE_SECAM :
				dType = VIDEO_VCRSYNC_720X576I;
				break;
			default :
				BREAK_WRONG(sDePathOfCve.std);
		}
		if (ret) break;
		ret = VIDEO_MCU_SetSyncOfVCR(dType);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetVcs(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DE_VCS_IPC_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		ret = VIDEO_SetIPC4CPU(stParams.inx, stParams.data);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DE_IO_SetEdgeCrop(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		ret = VIDEO_SetParameter(VIDEO_CTRL_ZOOM_CUT_WIN_OVER, stParams);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T DE_GetPathOfCVE(LX_DE_CVE_PARAM_T *pDePathOfCve)
{
#ifdef USE_XTENSA
	*pDePathOfCve = sDePathOfCve;
#endif

	return OK;
}

DTV_STATUS_T MCU_SetPanelType(LX_DE_DIS_FMT_T *pstParams)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	VIDEO_DSYNC_TYPE_T dType = VIDEO_DSYNC_1920x1080_60P;
	BOOLEAN bDividedFreq = FALSE;
	BOOLEAN isSamePannel;
	static LX_DE_PANEL_TYPE_T panel_Old = LX_PANEL_TYPE_MAX;

	switch (pstParams->panel_type) {
		case LX_PANEL_TYPE_1366 :
			dType = VIDEO_DSYNC_1366x_768_60P;
			break;
		case LX_PANEL_TYPE_1920 :
			switch (pstParams->fr_rate) {
				case 47 :
					bDividedFreq = TRUE;
				case 48 :
					dType = VIDEO_DSYNC_1920x1080_48P;
					break;
				case 50 :
					dType = VIDEO_DSYNC_1920x1080_50P;
					break;
				case 59 :
					bDividedFreq = TRUE;
				case 60 :
					dType = VIDEO_DSYNC_1920x1080_60P;
					break;
				default :
					break;
			}
			break;
		default :
			BREAK_WRONG(pstParams->panel_type);
	}
	isSamePannel = (panel_Old == pstParams->panel_type);
	panel_Old    = pstParams->panel_type;

	do {
		if (ret) break;
		ret = VIDEO_SetDividedFreq(bDividedFreq);
		if (ret) break;
		ret = (isSamePannel)?VIDEO_SetSyncOfDisplay(dType):VIDEO_MCU_SetSyncOfDisplay(dType);
	} while (0);
#endif

	return ret;
}

#if 0
DTV_STATUS_T DE_DDR_GrabPixel(LX_DE_GRAB_PIXEL_T *pstParams)
{
	DTV_STATUS_T ret = OK;
	VIDEO_FORMAT_TYPE_T lVideoFormat;

	UINT8 *pFrameAddrY8 = NULL;
	UINT8 *pFrameAddrC8 = NULL;
	UINT8 *pFrameAddrY2 = NULL;
    UINT8 *pFrameAddrC2 = NULL;

	UINT32 grabX;
	UINT32 grabY;
	UINT32 grabW;
	UINT32 grabH;
	UINT32 rowAddrY;
	UINT32 rowAddrC;
	UINT32 frameW;
	UINT32 frameH;
	UINT32 pixelG8;
	UINT32 pixelB8;
	UINT32 pixelR8;
	UINT32 pixelG2;
	UINT32 pixelB2;
	UINT32 pixelR2;
	UINT8  pixelY2;
	UINT8  pixelC2;
	UINT32 x;
	UINT32 y;
	UINT32 posY;
	UINT32 posC;
	UINT32 divY;
	UINT32 remY;
	UINT32 divC;
	UINT32 remC;
	UINT32 framSize;
	UINT32 grabSize;

	do {
        CHECK_KNULL(pstParams);
		ret = VIDEO_GetWinFormt(VIDEO_WIN_DSP, &lVideoFormat);
		if (ret) break;
		rowAddrY = pstParams->region.realPixelGrabW;
		rowAddrC = pstParams->region.realPixelGrabH;
		frameW = 2048;
		frameH = 1200;

		grabX = pstParams->region.pixelGrabX;
		grabY = pstParams->region.pixelGrabY;
		grabW = pstParams->region.pixelGrabW;
		grabH = pstParams->region.pixelGrabH;

		framSize  = frameW  * frameH;
		grabSize  = grabW  * grabH;
        if (!grabSize) break;

		pFrameAddrY8 = (UINT8 *)CONV_ROW2PHY(GET_BITS(rowAddrY, 16, 16));
        pFrameAddrC8 = (UINT8 *)CONV_ROW2PHY(GET_BITS(rowAddrC, 16, 16));
		pFrameAddrY2 = (UINT8 *)CONV_ROW2PHY(GET_BITS(rowAddrY,  0, 16));
        pFrameAddrC2 = (UINT8 *)CONV_ROW2PHY(GET_BITS(rowAddrC,  0, 16));
		CHECK_KNULL(pFrameAddrY8);
		CHECK_KNULL(pFrameAddrC8);
		CHECK_KNULL(pFrameAddrY2);
		CHECK_KNULL(pFrameAddrC2);

		xthal_dcache_region_invalidate((char *)pFrameAddrY8, framSize);
		xthal_dcache_region_invalidate((char *)pFrameAddrC8, framSize);
		xthal_dcache_region_invalidate((char *)pFrameAddrY2, framSize/4);
		xthal_dcache_region_invalidate((char *)pFrameAddrC2, framSize/4);
			pixelG8 = 0;
			pixelB8 = 0;
			pixelR8 = 0;
		pixelG2 = 0;
		pixelB2 = 0;
		pixelR2 = 0;
		do {
			for (y=0;y<grabH;y++) {
				for (x=0;x<pstParams->region.pixelGrabW;x++) {
					posY = (frameW * (y+grabY) + (x+grabX));
					posC = GET_SVAL(posY, 1, 16, (VIDEO_CHROM_444==lVideoFormat.sample.smux)?2:1);
					pixelG8 += pFrameAddrY8[posY];
					pixelB8 += pFrameAddrC8[posC];
					pixelR8 += pFrameAddrC8[posC+1];
				}
			}
			if (10 != pstParams->region.colorDepth) break;

			for (y=0;y<grabH;y++) {
				for (x=0;x<pstParams->region.pixelGrabW;x++) {
					posY = (frameW * (y+grabY) + (x+grabX));
					divY = posY/4;
					remY = posY%4;
					posC = GET_SVAL(posY, 1, 16, (VIDEO_CHROM_444==lVideoFormat.sample.smux)?2:1);
					divC = posC/4;
					remC = posC%4;
					pixelY2  = pFrameAddrY2[divY];
					pixelC2  = pFrameAddrC2[divC];
					pixelG2 += GET_BITS(pixelY2, remY*2, 2);
					pixelB2 += GET_BITS(pixelC2, (remC+0)*2, 2);
					pixelR2 += GET_BITS(pixelC2, (remC+1)*2, 2);
				}
			}
		} while(0);
		pixelG8 /= grabSize;
		pixelB8 /= grabSize;
		pixelR8 /= grabSize;
		pixelG2 /= grabSize;
		pixelB2 /= grabSize;
		pixelR2 /= grabSize;
		pixelG8 <<= 2;
		pixelB8 <<= 2;
		pixelR8 <<= 2;

		pstParams->color.pixelGrabY  = pixelG8 + pixelG2;
		pstParams->color.pixelGrabCb = pixelB8 + pixelB2;
		pstParams->color.pixelGrabCr = pixelR8 + pixelR2;
	} while (0);

	return ret;
}
#endif

#endif

#ifdef USE_KDRV_CODES_FOR_CSC
static int DE_SetMatrixFromArray(double Dx[3][3], int Ax[9])
{
	int k;
	double Bx[9];

	for (k=0;k<9;k++) {
		Bx[k] = (double)Ax[k];
		Dx[k/3][k%3] = Bx[k] / (1<<29);
	}

	return OK;
}

static int DE_MultiplyMatrix(double Dx[3][3], double Ax[3][3], double Bx[3][3])
{
	int r, c, k;

	for (r=0;r<3;r++) for (c=0;c<3;c++) {
		Dx[r][c] = 0;
		for (k=0;k<3;k++) Dx[r][c] += Ax[r][k] * Bx[k][c];
	}

	return OK;
}

static int DE_ClippingMatrix(double Ax[3][3])
{
	int k;
	double dValue;

	for (k=0;k<9;k++) {
		dValue = Ax[k/3][k%3];
		if      (dValue >  2.0) dValue =  2.0;
		else if (dValue < -2.0) dValue = -2.0;
		Ax[k/3][k%3] = dValue;
	}

	return OK;
}

static int DE_SetArrayFromMatrix(int Ax[9], double Dx[3][3])
{
	int k;
	double Bx[9];

	for (k=0;k<9;k++) {
		Bx[k] = Dx[k/3][k%3] * (1<<29);
		Ax[k] = (int)Bx[k];
	}

	return OK;
}

int DE_LoadPostCsc(int *pCsctable, int *pContrast, int *pBrightness, int *pSaturation, int *pTheta)
{
	int ret = OK;
	int i;
	double coefA[3][3];
	double coefB[3][3];
	double coefC[3][3];
	int arryA[9];
	UINT32 index;
	UINT32 writeSize;
	UINT32 *pTable;
	double contVal;
	double satuVal;
	double thetaVal;
	double hueVal;
	double cosVal;
	double sinVal;

	do {
		if (pCsctable){
			for (i=0;i<(9+6);i++) sVideoPostCscTable[i] = pCsctable[i];
			DE_SetMatrixFromArray(smdCoef, sVideoPostCscTable);
		}

		if (pContrast) {
			contVal  = (double)(*pContrast);
			contVal /= (1<<DE_POST_CSC_FLOAT_BIT);
			for (i=0;i<9;i++) smdCntr[i/3][i%3] = ((0 == i) || (4 == i) || (8 == i))?contVal:0.0;
		}

		if (pSaturation) {
			satuVal  = (double)(*pSaturation);
			satuVal /= (1<<DE_POST_CSC_FLOAT_BIT);
			for (i=0;i<9;i++) smdSatr[i/3][i%3] = ((4 == i) || (8 == i))?satuVal:(0 == i)?1.0:0.0;
		}

		if (pTheta) {
			thetaVal = (double)*pTheta;
			thetaVal *= PI;
			thetaVal /= 1800.0;
			cosVal = (double)cos((double)(thetaVal));
			sinVal = (double)sin((double)(thetaVal));
			for (i=0;i<9;i++) {
				hueVal = 0.0;
				if (0 == i) {
					hueVal = 1.0;
				} else if ( (4 == i) || (8 == i) ) {
					hueVal  = cosVal;
				} else if (5 == i) {
					hueVal -= sinVal;
				} else if (7 == i) {
					hueVal  = sinVal;
				}
				smdtHue[i/3][i%3] = hueVal;
			}
		}
		if (pBrightness) sBrigth = *pBrightness;

		DE_MultiplyMatrix(coefA, smdCoef, smdtHue);
		DE_MultiplyMatrix(coefB, smdSatr, smdCntr);
		DE_MultiplyMatrix(coefC, coefA,   coefB);
		DE_ClippingMatrix(coefC);
		DE_SetArrayFromMatrix(arryA, coefC);

		index     = svVideoPostCsc->indx;
		writeSize = svVideoPostCsc->size;
		pTable    = svVideoPostCsc->arry;
		for (i=0;i<9;i++) pTable[i] = (UINT32)arryA[i];
		for (i=0;i<6;i++) {
			pTable[i+9] = (UINT32)sVideoPostCscTable[i+9];
			if (i < 3) continue;
			pTable[i+9] += sBrigth;
		}
		ret = VIDEO_LoadTable(VIDEO_POST_CSC_COEF, VIDEO_PARAM_WRITE, index, pTable, writeSize);
	} while (0);

	return ret;
}
#endif
