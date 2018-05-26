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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author		won.hur (won.hur@lge.com)
 *  @version	1.0
 *  @date		2014-05-12
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/semaphore.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/div64.h>
#include <linux/kthread.h>

#include "hdmi20_hal_h15ax.h"
#include "os_util.h"


#include "hdmi20_kapi.h"
#include "hdmi_kapi.h"
#include "hdmi20_drv.h"
#include "hdmi20_rx_port0.h"
#include "hdmi20_rx_port1.h"
#include "hdmi20_tx_port.h"

#include "hdmi20_dbg_regpresets.h"
#include "sys_regs.h"

#include "gpio_core.h"

/* Added 2014/07/02 taejun.lee */
#include <../../core/hdmi/hdmi_module.h>
/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/
#ifndef ON
#define ON	1
#endif

#ifndef OFF
#define OFF 0
#endif

#define ENABLE_VFREQ_MONITOR 1
#define PHY_LOCK_TRY_COUNT 1
#define PHY_RESET_TRY_COUNT	20

#define PHY_DBG_HIST_MAX	30

#define SW_WA_HDMI20_FIELDALTERNATIVE_MANUAL_OUTPUT_SYNCMODE 1

#define HDMI20_USE_DCM_DETECTOR	0
/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#define HDMI20_REG_WR(address,data)   \
	gMapped_address=(UINT32 *)ioremap(address, sizeof(UINT32));\
do{\
	if(gMapped_address == NULL) break;\
	*gMapped_address = data;\
	iounmap((void *)gMapped_address);\
}while(0);\

#define HDMI20_REG_RD(address,data)   \
	gMapped_address = (UINT32 *)ioremap(address, sizeof(UINT32));\
do{\
	if(gMapped_address == NULL) {data = 0; break;}\
	data = (unsigned int)*gMapped_address;\
	iounmap((void *)gMapped_address);\
}while(0);\

#define TBL_NUM(X)						(sizeof(X) /sizeof((X)[0]))
#define ENUM_TO_STR(x) #x

#define u32CHK_RANGE(X,Y,offset)		((((int)X - (int)Y) <= (int)offset) && (((int)X - (int)Y) >= -(int)offset))


/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/
typedef enum
{
	HDMI20_RX_SW_EMPTY = 0,
	HDMI20_RX_SW_FIELD_ALTERNATIVE_HV_MISSMATCH,
	HDMI20_RX_SW_SONY_EXPERIA_Z2,
	HDMI20_RX_SW_MAXNUM
} LX_HDMI20_SW_WORKAROUND_T;

typedef struct
{
	UINT32 cnt;
	UINT32 slave_addr[PHY_DBG_HIST_MAX];
	UINT32 reg_addr[PHY_DBG_HIST_MAX];
	UINT32 data[PHY_DBG_HIST_MAX];
} LX_HDMI20_DBG_PHY_HISTORY_T;

typedef enum
{
	HDMI20_MANUAL_2D_3G_3840_2160_24 = 0,
	HDMI20_MANUAL_2D_3G_3840_2160_25,
	HDMI20_MANUAL_2D_3G_3840_2160_30,
	HDMI20_MANUAL_2D_3G_3840_2160_420_50,
	HDMI20_MANUAL_2D_3G_3840_2160_420_60,
	HDMI20_MANUAL_2D_3G_4096_2160_24,
	HDMI20_MANUAL_2D_3G_4096_2160_25,
	HDMI20_MANUAL_2D_3G_4096_2160_30,
	HDMI20_MANUAL_2D_6G_3840_2160_50,
	HDMI20_MANUAL_2D_6G_3840_2160_60,
	HDMI20_MANUAL_2D_6G_4096_2160_50,
	HDMI20_MANUAL_2D_6G_4096_2160_60,
	HDMI20_MANUAL_2D_4K_SYNC_PARAM_MAXNUM
} HDMI20_MANUAL_2D_4K_SYNC_PARAM_T;

typedef enum
{
	HDMI20_MANUAL_3D_3G_1920_1080_50_FA = 0,
	HDMI20_MANUAL_3D_3G_1440_576_50_FA,
	HDMI20_MANUAL_3D_3G_1920_1080_60_FA,
	HDMI20_MANUAL_3D_3G_1440_480_60_FA,
	HDMI20_MANUAL_3D_3G_2880_480_60_FA,
	HDMI20_MANUAL_3D_3G_2880_600_50_FA,
	HDMI20_MANUAL_3D_3G_1440_600_50_FA,
	HDMI20_MANUAL_3D_SYNC_PARAM_FA_MAXNUM
} HDMI20_MANUAL_3D_SYNC_PARAM_FA_T;

typedef enum
{
	HDMI20_MANUAL_SYNC_VIC22 = 0,
	HDMI20_MANUAL_SYNC_VIC18,
	HDMI20_MANUAL_SYNC_VIC7,
	HDMI20_MANUAL_SYNC_VIC14,
	HDMI20_MANUAL_2D_2K_SYNC_PARAM_MAXNUM
} HDMI20_MANUAL_2D_2K_SYNC_PARAM_T;

typedef enum
{
	HDMI20_MANUAL_3D_3G_2880_576_50_SBSFULL = 0,	// Master 683
	HDMI20_MANUAL_3D_SYNC_PARAM_SBSFULL_MAXNUM
} HDMI20_MANUAL_3D_SYNC_PARAM_SBSFULL_T;


typedef enum
{
	ACR = 0,
	GCP,
	ACP,
	ISRC1,
	ISRC2,
	GAMUT,
	VSI,
	AVI,
	SPD,
	AIF,
	MPEGS,
	PACKET_MAXNUM
} HDMI20_H15AX_PACKET_NAME_TYPE_T;

typedef struct
{
	UINT32 phyInitiatedMode;	
	UINT32 hactive;
	UINT32 vactive;
	UINT32 vfreq;

	UINT32 hBlankHactive;	//0x408
	UINT32 hFrontHsync;		//0x40C
	UINT32 vFrontVsync;		//0x414
	UINT32 vBlankVactive;	//0x410
} HDMI20_MANUAL_SYNC_PARAM_VALUE_T;


typedef struct
{
	UINT32 hactive;
	UINT32 vactive;
	UINT32 vfreq;
	UINT32 dcmmode;
	UINT32 isInterlaced;
	UINT32 okPhyValue;
	UINT32 ngPhyValue;
} HDMI20_PHY_AUDIO_SW_WA_DATASTRUCT_T;
/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
typedef enum
{
	HDMI20_PHY_AUDIO_1920_1080_50_P_DCM10 = 0,
	HDMI20_PHY_AUDIO_1920_1080_60_P_DCM10,
	HDMI20_PHY_AUDIO_SW_TIMING_MAXNUM
} HDMI20_PHY_AUDIO_SW_WA_TIMING_LIST_t;

HDMI20_PHY_AUDIO_SW_WA_DATASTRUCT_T gAudioPhySWWAList[HDMI20_PHY_AUDIO_SW_TIMING_MAXNUM] = 
{
	{1920, 1080, 500, 0x5, 0, 0x0, 0x1},
	{1920, 1080, 600, 0x5, 0, 0x0, 0x1},
	{1280, 720,  600, 0x5, 0, 0x1, 0x10},
	{1280, 720,  500, 0x5, 0, 0x1, 0x10},
	{1920, 1080, 500, 0x5, 1, 0x1, 0x10},
	{1920, 1080, 600, 0x5, 1, 0x1, 0x10},
	{2560, 1080, 500, 0x0, 0, 0x0, 0x1},
	{1920, 1080, 600, 0x5, 1, 0x1, 0x10},
};

/* For 3G 4K video display enable signal HW bug, SW W/A */
HDMI20_MANUAL_SYNC_PARAM_VALUE_T g2DManualSyncLUT[HDMI20_MANUAL_2D_4K_SYNC_PARAM_MAXNUM] =
{
	{0, 3840, 2160, 240, 0x67C0F00, 0x4FC0058, 0x8000A, 0x5A0870},
	{0, 3840, 2160, 250, 0x5A00F00, 0x4200058, 0x8000A, 0x5A0870},
	{0, 3840, 2160, 300, 0x2300F00, 0xB00058,  0x8000A, 0x5A0870},
	{0, 3840, 2160, 500, 0x02D00780, 0x210002C, 0x8000A, 0x5A0870},
	{0, 3840, 2160, 600, 0x1180780, 0x0058002C, 0x0008000A, 0x005A0870},
	{0, 4096, 2160, 240, 0x57C1000, 0x3FC0058, 0x8000A, 0x5A0870},
	{0, 4096, 2160, 250, 0x4A01000, 0x3C80058, 0x8000A, 0x5A0870},
	{0, 4096, 2160, 300, 0x1301000, 0x580058,  0x8000A, 0x5A0870},
	{1, 3840, 2160, 500, 0x5A00F00, 0x4200058, 0x8000A, 0x5A0870},
	{1, 3840, 2160, 600, 0x2300F00, 0xB00058,  0x8000A, 0x5A0870},
	{1, 4096, 2160, 500, 0x4A01000, 0x3C80058, 0x8000A, 0x5A0870},
	{1, 4096, 2160, 600, 0x1301000, 0x580058,  0x8000A, 0x5A0870}
};

HDMI20_MANUAL_SYNC_PARAM_VALUE_T g3D_FA_ManualSyncLUT[HDMI20_MANUAL_3D_SYNC_PARAM_FA_MAXNUM] =
{
	{0, 1920, 1080, 500, 0x02d00780, 0x0210002c, 0x00020005, 0x0016021c},	// Master 508
	{0, 1440, 576,  500, 0x012005a0, 0x0018007c, 0x00010003, 0x00180120},	// Master 518
	{0, 1920, 1080, 600, 0x01180780, 0x0058002c, 0x00020005, 0x0016021c},	// Master 605
	{0, 1440, 480,  600, 0x011405a0, 0x0026007b, 0x00020003, 0x001600f0},	// Master 655
	{0, 2880, 480,  600, 0x02280B40, 0x004C00F8, 0x00040003, 0x001600F0},	// Master 596
	{0, 2880, 600,  500, 0x02400B40, 0x003000FC, 0x00020003, 0x00180120},	// Master 803, 856
	{0, 1440, 600,  500, 0x012005A0, 0x0018007E, 0x00020003, 0x00180120}	// Master 518 
};

HDMI20_MANUAL_SYNC_PARAM_VALUE_T g2D_2K_ManualSyncLUT[HDMI20_MANUAL_2D_2K_SYNC_PARAM_MAXNUM] = 
{
	{0, 1440, 576,  500, 0x12005A0, 0x18007E,  0x20003,	0x180120},
	{0, 720,  576,  500, 0x9002D0,  0xC0040,   0x50005, 0x310240},
	{0, 1440, 480,  600, 0x11405A0, 0x26007C,  0x40003, 0x1600F0},
	{0, 1440, 480,  600, 0x11405A0, 0x20007C,  0x90006, 0x2D01E0}		
};


/* Phy DBG */
LX_HDMI20_DBG_PHY_HISTORY_T phyDBGHist;

/* Audo EQ DBG */
LX_HDMI20_DBG_PHY_HISTORY_T phyAddHist[HDMI20_HAL_NUMBER_OF_LINK];

/* Device handlers : Needs each for each link */
HDMI20_HAL_RX_DEVICE_DATA gHDMI20RxHandler[HDMI20_HAL_NUMBER_OF_LINK];
HDMI20_HAL_TX_DEVICE_DATA gHDMI20TxHandler;


/* Added 2014/07/02 taejun.lee */
HDMI20_HAL_AUDIO_DATA *pgHDMI20AudioData = NULL;
static LX_HDMI_AUDIO_INFO_T _gH15AxHdmiAudioInfo[HDMI_LINK_MAX] = { \
	{LX_HDMI_LINK0, LX_HDMI_AUDIO_DEFAULT, LX_HDMI_SAMPLING_FREQ_NONE}, \
	{LX_HDMI_LINK0, LX_HDMI_AUDIO_DEFAULT, LX_HDMI_SAMPLING_FREQ_NONE} };


/* TX will be added */

/* DBG : Register presets for DBG */
LX_HDMI20_REG_SETTING_T	_gHDMI20RegSettings_H15Ax[] =
{
	{"HDMI20_H15A0_TX_SET_PHY_TO_UD_OUTPUT", hdmi20_h15a0_tx_set_phy_on, sizeof(hdmi20_h15a0_tx_set_phy_on)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI20_H15A0_TX_SET_SCRAMLBING_ON", hdmi20_h15a0_tx_set_scrambling_on, sizeof(hdmi20_h15a0_tx_set_scrambling_on)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI20_H15A0_TX_SET_2CH_SYNC_SETTINGS", hdmi20_h15a0_tx_set_sync_to_ud, sizeof(hdmi20_h15a0_tx_set_sync_to_ud)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_TX_TEST_PATTERN_WITH_MAINVIDEO_OUTPUT_RGB444", hdmi20_h15a0_tx_test_pattern_out_rgb444, sizeof(hdmi20_h15a0_tx_test_pattern_out_rgb444)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_CHANGE_RGB444_TO_YUV422_ON_TEST_PATTERN_MODE", hdmi20_h15a0_tx_test_pattern_out_yuv422, sizeof(hdmi20_h15a0_tx_test_pattern_out_yuv422)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DISPLAY_PLL_ON", hdmi20_dbg_display_pll_on, sizeof(hdmi20_dbg_display_pll_on)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_VX1_8LANE", hdmi20_dbg_vx1_8lane, sizeof(hdmi20_dbg_vx1_8lane)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_TPG_4K", hdmi20_dbg_vx1_tpg_4k, sizeof(hdmi20_dbg_vx1_tpg_4k)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_CCO_SETTING", hdmi20_dbg_cco_setting, sizeof(hdmi20_dbg_cco_setting)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_TPG_ON", 	hdmi20_dbg_additional_tpg, sizeof(hdmi20_dbg_additional_tpg)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_VX1_8LANE_REAL_FINAL_STEP0", hdmi20_dbg_vx1_8lane_real_final_step0, sizeof(hdmi20_dbg_vx1_8lane_real_final_step0)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_VX1_8LANE_REAL_FINAL_STEP1", hdmi20_dbg_vx1_8lane_real_final_step1, sizeof(hdmi20_dbg_vx1_8lane_real_final_step1)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_VX1_8LANE_REAL_FINAL_STEP2", hdmi20_dbg_vx1_8lane_real_final_step2, sizeof(hdmi20_dbg_vx1_8lane_real_final_step2)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_SET_PHY_SETTING_ON", hdmi20_dbg_phy_setting, sizeof(hdmi20_dbg_phy_setting)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_SET_PHY_4K60_SETTING_ON", hdmi20_dbg_4k60_phy_setting, sizeof(hdmi20_dbg_4k60_phy_setting)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_SET_TX_4K60_OUTPUT", hdmi20_dbg_tx_4k60_output, sizeof(hdmi20_dbg_tx_4k60_output)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_SET_TX_4K30_OUTPUT", hdmi20_dbg_tx_4k30_output, sizeof(hdmi20_dbg_tx_4k30_output)/sizeof(LX_HDMI20_REG_T)},
	{"HDMI2.0_DBG_SET_TX_4K30_OUTPUT", hdmi20_dbg_tx_4k30_output_pe_sync, sizeof(hdmi20_dbg_tx_4k30_output_pe_sync)/sizeof(LX_HDMI20_REG_T)},
};

LX_HDMI20_REG_SETS_T _gHDMI20RegSettings =
{
	.pRegSettings = _gHDMI20RegSettings_H15Ax,
	.number_of_modes =sizeof(_gHDMI20RegSettings_H15Ax)/sizeof(LX_HDMI20_REG_SETTING_T),
};

/* Variables for Main Thread */
struct task_struct* gMainthread = NULL;
volatile int gMainThreadAlive = 1;
volatile int gMainThreadStamp = 0;
int gMainThreadMsleep = HDMI20_HAL_H15AX_INITIAL_THREAD_SLEEP_MS;

/* Variables for TX Thread */
struct task_struct* gTxthread = NULL;
volatile int gTxThreadAlive = 1;
volatile int gTxThreadStamp = 0;
int gTxThreadMsleep = HDMI20_HAL_H15AX_TX_THREAD_SLEEP_MS;

/* Variables for Video&Audio stablility */
int gVideoStableCntInitial = 8;
int gVideoStableCntNormalRun = HDMI20_HAL_H15AX_VIDEO_CHECK_CNT;

int gVideoStableCheckCnt	= 8;
int gAudioNoaudioCnt[2]		= {HDMI20_HAL_H15AX_AUDIO_CHECK_CNT, HDMI20_HAL_H15AX_AUDIO_CHECK_CNT}; /* Added 2014/08/04 taejun.lee */

/* DBG : Global Variables for activate/de-activate Working modes */
UINT32 gPrevCrcData[2] = {0,};
UINT32 gPrevSyncData[2][8] = {0,};
UINT32 gPrevCVISyncData[2][2] = {0,};
UINT8  gCrcMonitorRunMode = 0;
UINT8  gSyncMonitorRunMode = 0;
UINT8  gCVISyncMonitorRunMode = 0;
int gUseHDMI20Tx = 0;
int gWasTXInitiated = 0;
int gPhyLinkResetMode = 1;
int gPhyResetOnAbnormalCnt = 1;

unsigned int gPhyAutoEqStatusAddr = 0x09;
unsigned int gPhyAutoEqStatusdata = 0x00000200;
unsigned int gPhyAutoEqStatusMask = 0x00000600;

unsigned int gHPDInverseMode = 1;
unsigned int gDDCMode[2] = {0,};

volatile UINT32 *pDDCControlAddr = NULL;

LX_GPIO_VALUE_T	gGpio15Value = LX_GPIO_VALUE_INVALID;

UINT32 gVideoSurgeDetectThreshold = 0x40000;
int gVideoFujiFilmNoResetAccum[2] = {0,};
int gVideoFujiFilmThresHold = 3;
int gVideoUseGCPPacketForDCM = 0;
int gVideoPhyLostThreshold = 10; 
int gVideoPhyWriteMode = 0;
int gVideoClearAVMuteByTimer = 1;
int gDisableAllDCMSetting = 0;
UINT32 gHDMI20AVMuteHoldTime = 1000;
UINT32 gHDMI20DdcGpioHoldTime = 50;
UINT32 gHDMI20LocalMuteHoldTime = 0;
UINT32 gHDMI20LocalMuteHoldInitTime = 0;
UINT32 gHDMI20LocalMuteHoldOriginTime = 2500;

UINT32 gVideoVfreqMinValue = 15;
UINT32 gVideoVfreqMaxValue = 80;

UINT32 gAudioIsGalaxyTab10[2] = {0,};
UINT32 gVideoSurgeMonitor[2][10] = {0,};
UINT32 gVideoSurgeMonitor2[2][10] = {0,};

int gVideoResetSyncOnSurge = 0;
int gVideoDebugAmountOfHDCP14Reset = 0;
int gVideoDisableDCMChange = 0;
int gVideoResetManualOnVfreqChange = 0;
int gVideoUseManualSyncOnFHD = 0;
int gVideo2DManualSyncWaitTime = 0;
int gVideoResetESMOnResolutionChange = 0;
int gVideoDelayPhyInit = 0;
int gVideoUseSCDCIsr = 1;
int gVideoCheckHtotalForManualSync = 1;
UINT32 gVideoManualSyncHtotalCheckThres = 20;
int gLinkResetModeDetectionMode = 1;
UINT32 gVideoVfreqMonitorThreshhold = 3;
int gVideoEnableManualSync = 1;
int gVideoBlackoutonTimingInfoZero = 0;
int gAbnormalPhyLostClearTime = 2;
int gUseVideoChangeFlagOnMainThread = 1;
int gVideoTMDSTolerence = HDMI20_RX_H15AX_TMDS_TOLERANCE;
int gVideoDCMUpdateOnVideoChange = 1;
int gVideoDCMUpdateOnStableHDCP14 = 1;
int gVideoDCMDetectModeWithGCP = 0;
int gVideoLinkResetMode = 1;
int gVideoLinkResetDelayTime = 50;
int gVideoEnableUpdateVideoDatapacket = 1;
int gVideoUpdateCounterForDatapacket = 10;
int gVideoDCMDecisionCnt = 8;
int gDecreaseStableCntOnSigChange = 0;
int gVideoSyncResetDelay = 300;
int gVerboseOnAbnormalVideo = 0;
int gForcedHDMIMode = 0;
int gReinitHDCP14OnAksvISR = 0;
int synopsys_disable = 0;
int synopsys_phy_disable = 0;
int gManualSyncDelayTime = 400;
int gPhyI2CResetWaitTime = 100;
int gPhyI2CResetMAXcount = 10;
int gPhyResetWaitTime = 40;
int gHPDRequestWaitThreshold = 30;
int gLinkResetWaitTime = 100;
UINT32 gLinkResetValue = 0xFFFF;
int gAbnormalVideoResetTime = 10;
int gAbnormalPhyChangeTime = 4;
int gAbnormalPhyLostTime = 20;
int gReinitPhyOnLockLossByCnt = 1;
int gVideoPhyResetThreshold = PHY_RESET_TRY_COUNT;
int gVideoDebugLevel = 0;
int gAudioDebugLevel = 0;
//unsigned int gVideoPhyFHDDBGMode = 0;
unsigned int gVideoPhyFHDDBGMode = 2; 	// 2: Fixed PLL
unsigned int gVideoPhyUHDDBGMode = 0;
//unsigned int gVideoPhyFHDValue = 0x1A0; // 20140724 : If equalizer is not fixed value, master 6100 won't work
unsigned int gVideoPhyAlterFHDValue = 0x2C0;
unsigned int gVideoPhyAlterFHDMode = 0x0;
unsigned int gVideoPhyFHDValue = 6;
unsigned int gVideoPhyUHDValue = 0x1A0;
unsigned int gFixedScdcValue = 0;
unsigned int gFixedPLLMode = 0;
unsigned int gNumOfRunningHDMI = 0;
unsigned int gLGEPhyAPISleepTime = 10;
unsigned int gHDMIDcmCtrl = 0x40514;
unsigned int gHDCPBcapsCtrlValue = 0x12374;

/* SW W/A for webos popup issue for SONY EXPERIA */
unsigned int gHDMI205VPopUpSigneHoldTime = 0; // msec
unsigned int gHDMI20stableHDCP14HoldTime = 500; // msec 

unsigned int gHDMIModeRecover = 0x30010100;
unsigned int gHDMIVerticalTiming = 0x00000812 ;

unsigned int gHDMI20CapabilityAddr  = HDMI20_HAL_SRC_CAPABILITY_ADDR;
unsigned int gHDMI20CapabilityCheck = HDMI20_HAL_SCDC_MAGIC_WORD;

unsigned int gCntOfResume = 0;
/* SW W/A for H15Ax : CVI Bug */
UINT32 *pHDMI_MUX_CTRL_B = 0x0;
UINT32 *pHDMI_MUX_CTRL_C = 0x0;
UINT32 *pHDMI_MUX_CTRL_D = 0x0;


/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
/* Function needed inside for HDMI20 HAL */
static void __HDMI20_HAL_H15Ax_Rx_Init_Dev_Handler(UINT8 isResume);

/* IRQ handler for LINK */
static irqreturn_t __HDMI20_HAL_RX_Link_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs);

/* Thread & Handler update(subscribe) function */
static int __HDMI20_HAL_H15Ax_Refresh_TMDS(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_AVMute(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_Video(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_Audio(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_DataPacket(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_AKSV(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_PLLLock(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_PhyI2CDone(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_PhyI2CNack(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_DCM(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Packet_Store(struct hdmi_rx_ctrl *ctx, UINT8 type, bool enable);
static int __HDMI20_HAL_H15Ax_Refresh_SCDC(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_CAP(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_NCAP(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_LOST(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_ISTS(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_FAIL(struct hdmi_rx_ctrl *ctx);
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_CHANGE(struct hdmi_rx_ctrl *ctx);


/* System Function wrapper */
static int __HDMI20_H15Ax_Spin_Init(void *handler);
static int __HDMI20_H15Ax_Spin_IRQ_Lock(void *handler, unsigned long *flags);
static int __HDMI20_H15Ax_Spin_IRQ_UnLock(void *handler, unsigned long flags);
static int __HDMI20_Destory_Spinlock(void *handler);


/* Function for IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Initialize(LX_HDMI20_INIT_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_UnInitialize(void);
static int __HDMI20_HAL_H15Ax_Tx_Initialize(void);
static int __HDMI20_HAL_H15Ax_Tx_UnInitialize(void);
static int __HDMI20_HAL_H15Ax_Rx_Get_TimingInfo(UINT8 port, LX_HDMI_TIMING_INFO_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_AVIInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_SPDInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_VSIInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_COLORInfo(UINT8 port, LX_HDMI_COLOR_DOMAIN_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_AKSVData	(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_PHYStatus(LX_HDMI_PHY_INFORM_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_AspectRatio(UINT8 port, LX_HDMI_ASPECTRATIO_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_SrcDeviceInfo(UINT8 port, LX_HDMI20_SRCINFO_T *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Set_ResetTimingInfo(void);
static int __HDMI20_HAL_H15Ax_Rx_Set_EDID(UINT8 port, UINT8 *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_EDID(UINT8 port, UINT8 *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Set_HDCP14(UINT8 port, UINT32 *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Get_HDCP14(UINT8 port, UINT32 *pData, int size);
static int __HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(UINT8 port ,UINT8 onOff, UINT8 force);
static int __HDMI20_HAL_H15Ax_Rx_Set_EnableEDID(UINT8 onOff);
static int __HDMI20_HAL_H15Ax_Rx_DBG_Tool(int type, UINT8 port, int data);
static int __HDMI20_HAL_H15Ax_Rx_GetAudioInfo(UINT8 port, LX_HDMI_AUDIO_INFO_T *pHdmiAudioInfo, int size);
static int __HDMI20_HAL_H15Ax_Rx_GetAudioCopyInfo(UINT8 port, LX_HDMI_AUDIO_COPY_T *pHdmiCopyInfo, int size);
static int __HDMI20_HAL_H15Ax_Rx_SetHDMIARC(UINT8 port, BOOLEAN bOnOff);
static int __HDMI20_HAL_H15Ax_Rx_SetMute(UINT8 port, LX_HDMI_MUTE_CTRL_T *pMuteCtrl, int size);
static int __HDMI20_HAL_H15Ax_Rx_GetHDMIState(UINT8 port, LX_HDMI_STATUS_T *pHdmiStatus, int size);
static int __HDMI20_HAL_H15Ax_Rx_GetDebugAudioInfo(UINT8 port, LX_HDMI_DEBUG_AUDIO_INFO_T *pDebugAudioInfo, int size);
static void __HDMI20_HAL_H15Ax_Tx_Init_Dev_Handler(void);
static int __HDMI20_HAL_H15Ax_Rx_Set_HDCP22Info(UINT8 port, int isInitiated);

/* DBG */
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintHandler(HDMI20_HAL_RX_DEVICE_DATA *handler);
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintVideo(HDMI20_HAL_RX_DEVICE_DATA *handler);
static int __HDMI20_HW_H15_SetCombination(UINT32 param);
static void __HDMI20_HAL_DBG_PrintPixelCRCMonitor(UINT8 port, UINT8 diffmode);
static void __HDMI20_HAL_DBG_PrintECCMonitor(UINT8 port);
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintSavedEDID(UINT8 port);

/* Main */
static void __HDMI20_HAL_H15Ax_MainThread(void);
static char *uint8ToBinary(UINT8 i);
static void __HDMI20_HAL_H15Ax_TxThread(void);


/* New Added by LGSIC won.hur */
static int __HDMI20_HAL_H15Ax_Rx_Get_AudioFreqFromTMDSClock(UINT8 port, unsigned int *samplingFreq);
static void __HDMI20_HAL_H15Ax_Rx_Audio_Configure(UINT8 port, int isUD);
static int __HDMI20_HAL_H15Ax_UpdateTimingInfo(struct hdmi_rx_ctrl *ctx, int clear);
static int __HDMI20_HAL_H15Ax_IsSCDCvalueUD(UINT8 port);
static int __HDMI20_HAL_H15Ax_IsHPD_Up(UINT8 port);
static void __HDMI20_HAL_H15Ax_DBG_PrintSyncMonitor(UINT8 port, UINT8 diffmode);
static void __HDMI20_HAL_H15Ax_UpdateSCDC(UINT8 port);
static void __HDMI20_HAL_H15Ax_UpdatePhyLockFlag(UINT8 port);
static void __HDMI20_HAL_H15Ax_ClearChangeFlags(UINT8 port);
static void __HDMI20_HAL_H15Ax_Adjust_VideoParam(struct hdmi_rx_ctrl_video *v);
static void __HDMI20_HAL_H15Ax_SetVideoColorDomain(UINT8 port);
static void __HDMI20_HAL_H15Ax_TestPattern(UINT8 port, int isUD, int isOn);
static int __HDMI20_HAL_H15Ax_UpdateDCMMode(UINT8 port, int clear, int force);
static void __HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(UINT8 port);
static void __HDMI20_HAL_H15Ax_Rx_Set4k60(UINT8 port);
static void __HDMI20_HAL_H15Ax_Rx_CheckSCDC(UINT8 port);
static UINT32 __HDMI20_HAL_H15Ax_Calculate_PLL_Value(UINT32 tmds_clk);
static void __HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(int port, int clear);
static int __HDMI20_HAL_H15Ax_Rx_ListOfNotSupportingVideo(UINT8 port);
static void __HDMI20_HAL_H15Ax_Rx_SetForcedHDMIMode(UINT8 port);
static void __HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(UINT8 port, int clear);
static int __HDMI_HAL_H15Ax_Rx_AccessPhy(UINT8 port, LX_HDMI20_PHY_ACCESS_T *pPhyCtrl, int size);
static int __HDMI20_HAL_H15Ax_WritePhy(struct hdmi_rx_ctrl *ctx, UINT8 slave_address, UINT8 reg_address, UINT16 data);
static int __HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(UINT8 port, UINT8 mode);
static int __HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(UINT8 port);
static int __HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(UINT8 port, int onOff, int useTimer, UINT32 caller_id);
static int __HDMI20_HAL_H15Ax_Rx_SetLocalVideoMuteColor(UINT8 port, UINT32 isYCbCr);
static void __HDMI20_HAL_H15Ax_CheckVideoColorDomain(UINT8 port);

/* TX */
static void __HDMI20_HAL_H15Ax_Tx_TOP_Write(UINT32 addr, UINT32 data);
static UINT32 __HDMI20_HAL_H15Ax_Tx_TOP_Read(UINT32 addr);
static UINT32 __HDMI20_HAL_H15Ax_Tx_Read_Link(UINT32 addr);
static void __HDMI20_HAL_H15Ax_Tx_Write_Link(UINT32 addr, UINT32 data);
static void __HDMI20_HAL_H15Ax_Tx_SetMode4K60Hz(void);
static void __HDMI20_HAL_H15Ax_Tx_SetMode4K30Hz(void);

/* Added 2014/07/02 taejun.lee */
static void __HDMI20_HAL_H15Ax_TX_AUD_I2S_SETTING(void);
static void __HDMI20_HAL_H15Ax_TX_AUD_FC(void);

static int __HDMI20_HAL_H15Ax_GetAudioType(UINT8 port, LX_HDMI_AUDIO_TYPE_T *audioType);
static int __HDMI20_HAL_H15Bx_GetAudioType(UINT8 port, LX_HDMI_AUDIO_TYPE_T *audioType);

static void __HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(UINT8 port, int isCablePlug);

/* Added 2014/08/04 taejun.lee */
static int __HDMI20_HAL_H15Ax_GetAudioInfo(UINT8 port);
static int __HDMI20_HAL_H15Ax_AudioReset(UINT8 port);

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/
static unsigned int *gMapped_address; // For DBG

static char sPacketName[hdmi_rx_ctrl_packet_cnt][PACKET_MAXNUM] =
{
	{"ACR"},
	{"GCP"},
	{"ACP"},
	{"ISRC1"},
	{"ISRC2"},
	{"GAMUT"},
	{"VSI"},
	{"AVI"},
	{"SPD"},
	{"AIF"},
	{"MPEGS"}
};


int dataPacketNum[PACKET_MAXNUM] = 
{
	0x01,
	0x03,
	0x04,
	0x05,
	0x06,
	0x0A,
	0x81,
	0x82,
	0x83,
	0x84,
	0x85
};


/* For device supporitng 3D without VSI */
static const sEXT_TIMING_ENUM TBL_EXT_INFO[ ] =
{	/// hAct_info	vAct_info	scan_info	hAct_buf	vAct_buf	scan_buf	extInfo_buf
	{	1280,	1470,	1,		1280, 	720, 	1,		LX_HDMI_EXT_3D_FRAMEPACK }, 		//720p FP
	{	1920,	2205,	1,		1920, 	1080, 	1,		LX_HDMI_EXT_3D_FRAMEPACK },		//1080p FP
	{	1920,	2228,	1,		1920, 	1080, 	0,		LX_HDMI_EXT_3D_FRAMEPACK },		//1080i FP

	{	2560,	720,	1,		1280, 	720, 	1,		LX_HDMI_EXT_3D_SBSFULL }, 			//720p SSF
	{	3840,	1080,	1,		1920, 	1080, 	1,		LX_HDMI_EXT_3D_SBSFULL },			//1080p SSF
	{	3840,	1080,	0,		1920, 	1080, 	0,		LX_HDMI_EXT_3D_SBSFULL },			//1080i SSF

	{	1280,	1440,	1,		1280, 	720, 	1,		LX_HDMI_EXT_3D_LINE_ALTERNATIVE }, 	//720p LA
	{	1920,	2160,	1,		1920, 	1080, 	1,		LX_HDMI_EXT_3D_LINE_ALTERNATIVE },	//1080p LA

	{	1920,	1103,	1,		1920, 	1080, 	0,		LX_HDMI_EXT_3D_FIELD_ALTERNATIVE },	//1080i FA
};

/*========================================================================================
  Implementation Group
  ========================================================================================*/

/*========================================================================================
  Implementation Group
  ========================================================================================*/
int HDMI20_H15A0_Open(void)
{
	/* Function that will be called before initialization */
	/* Important */
	int i, ret;

	/* EDID */
	for(i=0;i<HDMI20_HAL_NUMBER_OF_LINK;i++){
		HDMI20_INFO("Initiate Internal EDID Data for port[%d]\n", i);
		gHDMI20RxHandler[i].wasEdidWritten = 0;
		memset((void *)&gHDMI20RxHandler[i].edidData, 0, HDMI20_HAL_SIZE_OF_EDID);
	}

	if(lx_chip_rev() >= LX_CHIP_REV(H15, B0)){
		GPIO_DevExGetValue(15, &gGpio15Value);
		HDMI20_INFO("---PCB HPD GPIO : HW OPT. BIT10 = [%d]---\n", gGpio15Value);

		if(gGpio15Value == LX_GPIO_VALUE_LOW){
			gHPDInverseMode = 0;
		}
		else{
			gHPDInverseMode = 1;
		}
	}

	pDDCControlAddr = (volatile UINT32 *)ioremap(0xC8600088, sizeof(UINT32));

	ret = RET_OK;

	return ret;
}

void HDMI20_H15A0_ResetDataPacketBuffer(int type ,UINT8 port)
{
	int i;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return;
	}

	HDMI20_PACKET("Reset DataPacket Buffer Type[%d]/LINK[%d]\n", type, port);
	if(type >= 	hdmi_rx_ctrl_packet_cnt){
		for(i=0;i<hdmi_rx_ctrl_packet_cnt; i++){
			memset((void *)gHDMI20RxHandler[port].dataPacket[i], 0,(sizeof(UINT8)*MAX_DATA_PACKET_LENGTH));
		}
	}
	else{
		memset((void *)gHDMI20RxHandler[port].dataPacket[type], 0,(sizeof(UINT8)*MAX_DATA_PACKET_LENGTH));
	}
}

void HDMI20_H15A0_PrintDataPacketBuffer(int type ,UINT8 port)
{
	int i,y;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return;
	}

	if(type < 	hdmi_rx_ctrl_packet_cnt){
		HDMI20_INFO("Data Packet Type  =>  %s  \n", sPacketName[type]);
		for(i=0;i<3;i++){
			HDMI20_INFO("[HB%02d] - Binary[  %s  ]  -  Hex[  0x%02x  ]\n", i, uint8ToBinary((UINT8)gHDMI20RxHandler[port].dataPacket[type][i]), (UINT8)gHDMI20RxHandler[port].dataPacket[type][i]);
		}

		for(i=3;i<hdmi_rx_ctrl_packet_cnt; i++){
			HDMI20_INFO("[ B%02d] - Binary[  %s  ]  -  Hex[  0x%02x  ]\n", i, uint8ToBinary((UINT8)gHDMI20RxHandler[port].dataPacket[type][i]) ,(UINT8)gHDMI20RxHandler[port].dataPacket[type][i]);
		}
	}
	else {
		for(y=0; y<hdmi_rx_ctrl_packet_cnt; y++){
			HDMI20_INFO("Data Packet y  =>  %s  \n", sPacketName[y]);
			for(i=0;i<3;i++){
				HDMI20_INFO("[HB%02d] - Binary[  %s  ]  -  Hex[  0x%02x  ]\n", i, uint8ToBinary((UINT8)gHDMI20RxHandler[port].dataPacket[y][i]) ,(UINT8)gHDMI20RxHandler[port].dataPacket[y][i]);
			}

			for(i=3;i<hdmi_rx_ctrl_packet_cnt; i++){
				HDMI20_INFO("[ B%02d] - Binary[  %s  ]  -  Hex[  0x%02x  ]\n", i, uint8ToBinary( (UINT8)gHDMI20RxHandler[port].dataPacket[y][i] ) ,(UINT8)gHDMI20RxHandler[port].dataPacket[y][i]);
			}
		}
	}

}

int HDMI20_H15A0_Suspend(void)
{
	int ret = RET_ERROR;
	int i;
	HDMI20_INFO("HDMI2.0 Kernel Driver Suspend Start\n");
	int cnt = 50;	

	gMainThreadAlive = 0;

	while(1)
	{
		if(gMainThreadStamp == 0){
			HDMI20_INFO("Success closing HDMI2.0 Main thread\n");
			ret = RET_OK;
			break;
		}

		if(cnt <= 0)
		{
			HDMI20_INFO("Failed to exit HDMI2.0 Main thread\n");
			break;
		}
		cnt--;
		msleep(gMainThreadMsleep);
	}

	for(i=0;i<HDMI20_NUM_OF_RX_PORTS;i++){
		/* Audio Mute */
		//gHDMI20RxHandler[i].audioMuteEableCnt = 0;
		//gHDMI20RxHandler[i].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
		__HDMI20_HAL_H15Ax_AudioReset(i);
		
		/* HPD control */
		HDMI20_INFO(" Pull down HPD for HDMI2.0 port[%d]\n", i);
		__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(i, 0, 0);

		/* webos 5V popup sign control */
		gHDMI20RxHandler[i].connected5Vtime = 0;
		gHDMI20RxHandler[i].stablehdcp14time = 0;
	}

	return ret;
}

int HDMI20_H15A0_Resume(void)
{
	int i,y;
	UINT32 addr;
	UINT32 gpio_value = 0;

	HDMI20_INFO("HDMI2.0 Kernel Driver Resume Start\n");
	
	/* SW WA : PS4 cable connection long time */
	gReinitPhyOnLockLossByCnt = 1;
	
	__HDMI20_HAL_H15Ax_Rx_Init_Dev_Handler(1);

	/* Step. Set GPIO Pull Up */
	if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
		HDMI20_REG_RD(0xC898E494, gpio_value);
		gpio_value |= 0x30000000;
		HDMI20_REG_WR(0xC898E494, gpio_value);
	}
	else{
		HDMI20_REG_RD(0xC898E488, gpio_value);
		gpio_value &= 0xFFFFFFFC;
		HDMI20_REG_WR(0xC898E488, gpio_value);
	}



	/* Change PHY mode from fixed->Auto equalizer from H15Bx */
	if(lx_chip_rev( ) >= LX_CHIP_REV(H15, B0)){
		gVideoPhyFHDDBGMode = 2;
		gVideoPhyFHDValue = 0x6;
		gPhyLinkResetMode = 1;
	
		gVideoPhyUHDValue  = 0x200;
	}
	else{
		gVideoPhyFHDDBGMode = 2;
		gVideoPhyFHDValue = 0x6;
		gPhyLinkResetMode = 1;

		gVideoPhyUHDValue  = 0x1A0;
	}

	/* Reset HDCP22 initiated info */
	for(i = 0; i<HDMI20_HAL_NUMBER_OF_LINK; i++)
	{
		gHDMI20RxHandler[i].gWasHDCP22Written = 0;
		/* Step : Set GPIO Setting for HPD */
		if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
			/* HOT PLUG GPIO : Set Normal/GPIO Mode : Port 120, 121 */
			GPIO_DevSetPinMux(gHDMI20RxHandler[i].gpio_hpd_real, 1);
			GPIO_DevSetMode(gHDMI20RxHandler[i].gpio_hpd_real, LX_GPIO_MODE_OUTPUT);
			GPIO_DevSetPinMux(gHDMI20RxHandler[i].gpio_hpd_temp, 1);
			GPIO_DevSetMode(gHDMI20RxHandler[i].gpio_hpd_temp, LX_GPIO_MODE_OUTPUT);
		}
	}

	/* Enable 5V IRQ */
	HDMI20_HAL_H15Ax_RX0_Enable_5V_IRQ(1, 1);
	HDMI20_HAL_H15Ax_RX0_Enable_5V_IRQ(0, 1);
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(1, 1);
	HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(0, 1);

	/* Check 5V status for both port due to unknown state */
	HDMI20_HAL_H15Ax_RX0_Reset_TOP_SyncInfo();
	HDMI20_HAL_H15Ax_RX1_Reset_TOP_SyncInfo();

	gMainThreadAlive = 1;

	gMainthread = kthread_create( (void *)__HDMI20_HAL_H15Ax_MainThread, 0, "hdmi20_rx_video_stable_checker");
	if(gMainthread){
		HDMI20_INFO("Initialing VIDEO Stable checker thread\n");
		wake_up_process(gMainthread);
	}else HDMI20_ERROR("Cannot initiate Video stablizor thread\n");

	/* HDCP2.2 Write PUK*/
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_1, HDMI20_H15AX_PUK_1);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_2, HDMI20_H15AX_PUK_2);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_3, HDMI20_H15AX_PUK_3);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_4, HDMI20_H15AX_PUK_4);

	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_1, HDMI20_H15AX_PUK_1);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_2, HDMI20_H15AX_PUK_2);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_3, HDMI20_H15AX_PUK_3);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_4, HDMI20_H15AX_PUK_4);

	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_1, HDMI20_RX_H15AX_DUK_1);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_2, HDMI20_RX_H15AX_DUK_2);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_3, HDMI20_RX_H15AX_DUK_3);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_4, HDMI20_RX_H15AX_DUK_4);

	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_1, HDMI20_RX_H15AX_DUK_1);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_2, HDMI20_RX_H15AX_DUK_2);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_3, HDMI20_RX_H15AX_DUK_3);
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_4, HDMI20_RX_H15AX_DUK_4);

	/* SW Workaround for ACR pll mode */
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_RX_ACR_PLL_MODE, 0x10001 );
	__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_RX_ACR_PLL_MODE, 0x10001 );

	gCntOfResume++;
	HDMI20_INFO("HDMI2.0 Kernel Driver Resume End. Accumulated Resume cnt[%d]\n", gCntOfResume);
	return RET_OK;
}

int HDMI20_H15A0_Process_Cmd(LX_HDMI20_CMD_T* cmd)
{
	int ret = RET_OK;
	int cmd_result = HDMI20_CMD_STATUS_CMD_ERROR;

	do{
		/* Step 1. pharse cmd */
		switch(cmd->command)
		{
			case HDMI20_CMD_RX_INITIALIZE:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Initialize((LX_HDMI20_INIT_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_UNINITIALIZE	:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_UnInitialize();
				}
				break;

			case HDMI20_CMD_TX_INITIALIZE:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Tx_Initialize();
				}
				break;

			case HDMI20_CMD_TX_UNINITIALIZE	:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Tx_UnInitialize();
				}
				break;

			case HDMI20_CMD_RX_GET_TIMINGINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_TimingInfo((UINT8)cmd->attributes, (LX_HDMI_TIMING_INFO_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_AVIINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_AVIInfo((UINT8)cmd->attributes, (LX_HDMI_INFO_PACKET_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_SPDINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_SPDInfo((UINT8)cmd->attributes, (LX_HDMI_INFO_PACKET_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_VSIINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_VSIInfo((UINT8)cmd->attributes, (LX_HDMI_INFO_PACKET_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_COLORINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_COLORInfo((UINT8)cmd->attributes, (LX_HDMI_COLOR_DOMAIN_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_AKSVDATA	:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_AKSVData((UINT8)cmd->attributes, (LX_HDMI_INFO_PACKET_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_PHYSTATUS:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_PHYStatus((LX_HDMI_PHY_INFORM_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_SET_RESETTIMINGINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_ResetTimingInfo();
				}
				break;

			case HDMI20_CMD_RX_SET_EDID	:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_EDID((UINT8)cmd->attributes, (UINT8 *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_EDID	:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_EDID((UINT8)cmd->attributes, (UINT8 *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_SET_HDCP14:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_HDCP14((UINT8)cmd->attributes, (UINT32 *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_GET_HDCP14:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_HDCP14((UINT8)cmd->attributes, (UINT32 *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_RX_SET_ENABLEHPD:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_EnableHPD((UINT8)cmd->attributes, (UINT8)cmd->size, 0);
				}
				break;

			case HDMI20_CMD_RX_SET_ENABLEEDID:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_EnableEDID((UINT8)cmd->attributes);
				}
				break;

			case HDMI20_CMD_RX_DBG_TOOL:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_DBG_Tool((int)cmd->handle, (UINT8)cmd->attributes, (int)cmd->size);
				}
				break;
			case  HDMI20_CMD_RX_GET_AUDIOINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_GetAudioInfo((UINT8)cmd->attributes, (LX_HDMI_AUDIO_INFO_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case  HDMI20_CMD_RX_GET_AUDIOCOPYINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_GetAudioCopyInfo((UINT8)cmd->attributes, (LX_HDMI_AUDIO_COPY_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case  HDMI20_CMD_RX_SET_HDMIARC:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_SetHDMIARC((UINT8)cmd->attributes, (BOOLEAN)cmd->handle);
				}
				break;
			case  HDMI20_CMD_RX_SET_MUTE:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_SetMute((UINT8)cmd->attributes, (LX_HDMI_MUTE_CTRL_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case  HDMI20_CMD_RX_GET_AUDIODBGINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_GetDebugAudioInfo((UINT8)cmd->attributes, (LX_HDMI_DEBUG_AUDIO_INFO_T *)cmd->handle, (int)cmd->size);
				}
				break;

			case HDMI20_CMD_TX_SET_INPUTMUX:
				{
					cmd_result = HDMI20_HAL_H15Ax_Tx_InputMux(0, (LX_HDMI20_TX_INPUTMUX_TYPE_T*)cmd->handle);
				}
				break;

			case HDMI20_CMD_TX_GET_INPUTMUX:
				{
					cmd_result = HDMI20_HAL_H15Ax_Tx_InputMux(1, (LX_HDMI20_TX_INPUTMUX_TYPE_T *)cmd->handle);
				}
				break;

			case HDMI20_CMD_DBG_SET_PRESETS:
				{
					cmd_result = __HDMI20_HW_H15_SetCombination((UINT32)cmd->attributes);
				}
				break;
			case HDMI20_CMD_RX_GET_ASPECTRATIO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_AspectRatio((UINT8)cmd->attributes, (LX_HDMI_ASPECTRATIO_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case HDMI20_CMD_RX_GET_HDMISTATE:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_GetHDMIState((UINT8)cmd->attributes, (LX_HDMI_STATUS_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case HDMI20_CMD_RX_SET_HDCP22INFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Set_HDCP22Info((UINT8)cmd->attributes, (int)cmd->size);
				}
				break;
			case HDMI20_CMD_RX_GET_SRCINFO:
				{
					cmd_result = __HDMI20_HAL_H15Ax_Rx_Get_SrcDeviceInfo((UINT8)cmd->attributes, (LX_HDMI20_SRCINFO_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case HDMI20_CMD_RX_SET_PHY_ACCESS:
				{
					cmd_result = __HDMI_HAL_H15Ax_Rx_AccessPhy((UINT8)cmd->attributes, (LX_HDMI20_PHY_ACCESS_T *)cmd->handle, (int)cmd->size);
				}
				break;
			case HDMI20_CMD_RX_OPEN:
				{
					cmd_result = HDMI20_H15A0_Open();
				}
				break;
			case HDMI20_CMD_RX_SET_ENABLE_OPERATION:
				{
					gNumOfRunningHDMI = 2;	
					cmd_result = RET_OK;
				}
				break;
			default :
				{
					HDMI20_ERROR("Unknown IOCTL CMD[%d]\n", cmd->command);
					cmd_result = HDMI20_CMD_STATUS_IOCTL_ERROR;
					break;
				}
		}

		/* Last Step : Update CMD status*/
		cmd->status = cmd_result;

	} while(0);

	return ret;
}


/* -------------------------------------------------------------------------------
 * Static Function below this line
 * ------------------------------------------------------------------------------- */
static void __HDMI20_HAL_H15Ax_Rx_ClearI2CFlag(UINT8 port)
{
	gHDMI20RxHandler[port].i2cDoneSignal = 0;
	gHDMI20RxHandler[port].i2cNackSignal = 0;
}

static void __HDMI20_HAL_H15Ax_ClearChangeFlags(UINT8 port)
{
	gHDMI20RxHandler[port].scdc_change = 0;
	gHDMI20RxHandler[port].video_change = 0;
	gHDMI20RxHandler[port].dcm_change = 0;
	gHDMI20RxHandler[port].audio_change = 0;
	gHDMI20RxHandler[port].tmds_change = 0;
	gHDMI20RxHandler[port].hpd_request = 0;
	gHDMI20RxHandler[port].aksv_change = 0;
}


static int __HDMI20_HAL_H15Ax_UpdateDCMMode(UINT8 port, int clear, int force)
{
	int i;
	int ret = 0;
	int currentDCM = 0;
	int currentDCMStable = 0;


	if(clear){
		HDMI20_VIDEO("Notice [%d] : Clear DCM History \n", port);
		gHDMI20RxHandler[port].currentDCMMode = 0x0;

		if(force >0 )hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x200);

		for(i=0;i<HDMI20_HAL_H15AX_DCM_DECISION_MAX_NUM;i++){
			gHDMI20RxHandler[port].previousDCMMode[i] = 4;
		}
	}
	else if(force > 0){

		if(gVideoDisableDCMChange > 0) return 0;

		switch(force)
		{
			case 24:
				currentDCM = 0x0;
				break;
			case 30:
				currentDCM = 0x5;
				break;
			case 36:
				currentDCM = 0x6;
				break;
			case 48:
				currentDCM = 0x7;
				break;
			default:
				currentDCM = force;
				break;
		}
	

		if(gHDMI20RxHandler[port].currentDCMMode == currentDCM){
			HDMI20_VIDEO("Notice [%d] : Force DCM update but, same as previous[0x%x]\n", port, currentDCM);
			return 0;	
		}

		if(gDisableAllDCMSetting == 0){
			if(currentDCM == 0x5){
				HDMI20_VIDEO("Notice [%d] : DCM Mode Changed from [0x%x] to 10 bit mode\n", port, gHDMI20RxHandler[port].currentDCMMode);
				hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x220);
				ret = RET_OK;
			}
			else if(currentDCM == 0x6){
				HDMI20_VIDEO("Notice [%d] : DCM Mode Changed from [0x%x] to 12 bit mode\n", port, gHDMI20RxHandler[port].currentDCMMode);
				hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x240);
				ret = RET_OK;
			}
			else if(currentDCM == 0x7){
				HDMI20_VIDEO("Notice [%d] : DCM Mode Changed  from [0x%x] to 16 bit mode\n", port, gHDMI20RxHandler[port].currentDCMMode);
				hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x260);
				ret = RET_OK;
			}
			else if((currentDCM == 0x4)||(currentDCM == 0x0)){
				currentDCM = 0;
				HDMI20_VIDEO("Notice [%d] : DCM Mode Changed from [0x%x] to 8 Bit mode\n", port, gHDMI20RxHandler[port].currentDCMMode);
				hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x200);

				ret = RET_OK;
			}
			else{
				HDMI20_VIDEO("Error [%d] : Unknown DCM value[0x%08x]\n", port, currentDCM);
				ret = RET_ERROR;
			}
		}
		if(ret == RET_OK){
			gHDMI20RxHandler[port].currentDCMMode = currentDCM;
		}
	}
	else{
		if(gVideoDisableDCMChange > 0) return 0;

		if(gVideoDCMDetectModeWithGCP){
			currentDCM = (UINT32)gHDMI20RxHandler[port].dataPacket[GCP][5];
			currentDCM &= (0x0000000f);
		}
		else{
			currentDCM = gHDMI20RxHandler[port].ctrl.bsp_read(HDMI20_HAL_H15AX_RX_STATUS_REG);
			currentDCM &= HDMI20_HAL_H15AX_RX_CURRENT_DCM_MASK;
			currentDCM = currentDCM >> 28;
		}

		for(i=0;i<gVideoDCMDecisionCnt;i++){
			gHDMI20RxHandler[port].previousDCMMode[i] = gHDMI20RxHandler[port].previousDCMMode[i+1];
		}


		gHDMI20RxHandler[port].previousDCMMode[gVideoDCMDecisionCnt] = currentDCM;

		for(i=0;i<gVideoDCMDecisionCnt;i++){
			if(gHDMI20RxHandler[port].previousDCMMode[i] != gHDMI20RxHandler[port].previousDCMMode[i+1]){
				currentDCMStable++;
			}
		}

		if((currentDCM == 0x4)||(currentDCM == 0x0)){
			currentDCM = 0;
		}

		if((currentDCMStable == 0)&&(currentDCM != gHDMI20RxHandler[port].currentDCMMode)){
			if(gDisableAllDCMSetting == 0){
				if(currentDCM == 0x5){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Packet to 10 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x220);
					ret = RET_OK;
				}
				else if(currentDCM == 0x6){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Packet to 12 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x240);
					ret = RET_OK;
				}
				else if(currentDCM == 0x7){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Packet to 16 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x260);
					ret = RET_OK;
				}
				else if((currentDCM == 0x4)||(currentDCM == 0x0)){
					currentDCM = 0;
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed to 8 Bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x200);

					ret = RET_OK;
				}
				else{
					HDMI20_VIDEO("Error [%d] : Unknown DCM value[0x%08x]\n", port, currentDCM);
					ret = RET_ERROR;
				}

				if(ret == RET_OK){
					gHDMI20RxHandler[port].currentDCMMode = currentDCM;
				}
			}
			/*
			if(gHDMI20RxHandler[port].currentDCMMode == 0){
				currentDCM = gHDMI20RxHandler[port].ctrl.bsp_read(HDMI20_HAL_H15AX_RX_STATUS_REG);
				currentDCM &= HDMI20_HAL_H15AX_RX_CURRENT_DCM_MASK;

				if(currentDCM == 0x50000000){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Register to 10 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x220);
					currentDCM = 5;
				}
				else if(currentDCM == 0x60000000){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Register to 12 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x240);
					currentDCM = 6;
				}
				else if(currentDCM == 0x70000000){
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Register to 16 bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x260);
					currentDCM = 7;
				}
				else{
					HDMI20_VIDEO("Notice [%d] : DCM Mode Changed by Register to 8 Bit mode\n", port);
					hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x200);
					currentDCM = 0;
				}

				gHDMI20RxHandler[port].currentDCMMode = currentDCM;
			}
*/
		}
		else{
			if(currentDCMStable > 0){
				HDMI20_VIDEO("Warning %d : DCM is unstable[%d]. StableCnt Reset from[%d]\n", port, currentDCMStable, gHDMI20RxHandler[port].ctrl.stable_cnt);

				gHDMI20RxHandler[port].ctrl.stable_cnt = 0;
				if(gVideoDebugLevel == 935){
					for(i=0;i<gVideoDCMDecisionCnt;i++){
						HDMI20_VIDEO("Port %d : DCM[%d]<=[0x%08x]\n", port, i, gHDMI20RxHandler[port].previousDCMMode[i]);
					}
				}
			}
			ret = 1;
		}
	}
	
	return ret;
}



static void __HDMI20_HAL_H15Ax_UpdatePhyLockFlag(UINT8 port)
{
	if(gHDMI20RxHandler[port].ctrl.bsp_read(0x30) & 0x00000001){
		gHDMI20RxHandler[port].isPhyLocked = true;

	}
	else{
		gHDMI20RxHandler[port].isPhyLocked = false;
        //gHDMI20RxHandler[port].audioMuteEableCnt = 0;
        //gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
        __HDMI20_HAL_H15Ax_AudioReset(port);
	}
}

static int __HDMI_HAL_H15Ax_Rx_AccessPhy(UINT8 port, LX_HDMI20_PHY_ACCESS_T *pPhyCtrl, int size)
{
	int ret = RET_ERROR;
	LX_HDMI20_PHY_ACCESS_T userdata;

	do{
		if(pPhyCtrl == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI20_PHY_ACCESS_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI20_PHY_ACCESS_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI20_PHY_ACCESS_T));

		/* Step 1-2. Get User data to kernel*/
		if( copy_from_user(&userdata, (void __user *)pPhyCtrl, sizeof(LX_HDMI20_PHY_ACCESS_T))){
			HDMI20_ERROR("Cannot copy HDMI20 RX Phy I2C ctrl structure from user\n");
			break;
		}

		if(userdata.isWrite){
			ret = hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, userdata.slaveAddr , userdata.regAddr, (UINT16)userdata.data);
			if(phyDBGHist.cnt < PHY_DBG_HIST_MAX){
				phyDBGHist.slave_addr[phyDBGHist.cnt] = userdata.slaveAddr;
				phyDBGHist.reg_addr[phyDBGHist.cnt] = userdata.regAddr;
				phyDBGHist.data[phyDBGHist.cnt] = userdata.data;
				phyDBGHist.cnt++;
			}

		}
		else{

			userdata.data = (UINT32)hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, userdata.slaveAddr, userdata.regAddr);
		}

		ret = copy_to_user((LX_HDMI20_PHY_ACCESS_T *)pPhyCtrl, &userdata, sizeof(LX_HDMI20_PHY_ACCESS_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "PHY I2C Access", port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

static void __HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(UINT8 port, int clear)
{
	UINT32 scdc_value = 0x0;

	if(clear){
		gHDMI20RxHandler[port].isHDMI20Device = false;
	}
	else{
		scdc_value = gHDMI20RxHandler[port].ctrl.bsp_read(gHDMI20CapabilityAddr);

		if((scdc_value & gHDMI20CapabilityCheck) == 0x00000000){
			gHDMI20RxHandler[port].isHDMI20Device = false;
		}
		else{
			gHDMI20RxHandler[port].isHDMI20Device = true;
		}
	}
}


static void __HDMI20_HAL_H15Ax_UpdateSCDC(UINT8 port)
{
	if(gFixedScdcValue > 0){
		if(gVideoDebugLevel == 3939){
			HDMI20_VIDEO(" Disable Update SCDC on Port[%d]\n", port);
		}
	}

	if(gHDMI20RxHandler[port].phyInitiatedMode){
		if(__HDMI20_HAL_H15Ax_IsSCDCvalueUD(port) == 0){
			gHDMI20RxHandler[port].scdc_change = true;
		}
	}
	else{
		if(__HDMI20_HAL_H15Ax_IsSCDCvalueUD(port)){
			gHDMI20RxHandler[port].scdc_change = true;
		}
	}
}

static int __HDMI20_HAL_H15Ax_IsSCDCvalueUD(UINT8 port)
{
	int ret = 0;
	UINT32 scdc_value;

	if(gHDMI20RxHandler[port].ctrl.bsp_read == NULL)
	{
		printk("[%s:%d] Warning! HDMI20 device handler is NULL\n", __F__, __L__);
		scdc_value = 0;
	}
	else
	{
		if(gVideoUseSCDCIsr > 0){
			scdc_value = gHDMI20RxHandler[port].isScdcUD;
		}
		else{
			scdc_value = gHDMI20RxHandler[port].ctrl.bsp_read(0x820);
			scdc_value &= 0x00020000;
		}
	}

	if(scdc_value) ret = 1;
	else ret = 0;

	if(gVideoDebugLevel == 3939){
		HDMI20_VIDEO(" Current SCDC return is [%d]. Scdc was [0x%08x]\n", ret, scdc_value);
	}

	return ret;
}

static void __HDMI20_HAL_H15Ax_Rx_CheckSCDC(UINT8 port)
{
	int ret = 0;
	int i;

	if(gFixedScdcValue == 0){
		if(gHDMI20RxHandler[port].phyInitiatedMode != __HDMI20_HAL_H15Ax_IsSCDCvalueUD(port))
		{
			HDMI20_VIDEO("Notice [%d] : ReInit Phy due to SCDC change from [%d] to SCDC[%d] \n", port,gHDMI20RxHandler[port].phyInitiatedMode,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(port));

			if(gHDMI20RxHandler[port].phyInitiatedMode){
				HDMI20_VIDEO("Notice [%d] : Reset Link and PHY Due to Manual SYnc to Auto Sync\n", port);
				/* Clear Timing Info */
				__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[port].ctrl, 1);
				
				gHDMI20RxHandler[port].locking_time = 0;
				gHDMI20RxHandler[port].phyLostCnt = 0;
				gHDMI20RxHandler[port].videoAbnormalCnt = 0;
				gHDMI20RxHandler[port].videoAbResetCnt = 0;
				gHDMI20RxHandler[port].phy_rst_request = 1;
				gHDMI20RxHandler[port].isManualSyncMode = 1;
				gHDMI20RxHandler[port].ctrl.stable_cnt = 0;
				HDMI20_HAL_H15Ax_Rx_LinkVideoReset(port, 0);


				ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(port,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(port));
				if(ret<0){
					HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", port, ret);
				}
				__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(port, 0);

			}
			else{
				if(gHDMI20RxHandler[port].isHDCP22){
					HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, port);
				}
			
				for(i=0;i<=gVideoDebugAmountOfHDCP14Reset;i++){
					hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
					hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
				}

				gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, 0xFFFE);
				msleep(50);
				gHDMI20RxHandler[port].ctrl.bsp_write(0xFF4, 0xFFFF);

				ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(port, __HDMI20_HAL_H15Ax_IsSCDCvalueUD(port));
				if(ret<0){
					HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", port, ret);
				}

				gHDMI20RxHandler[port].ctrl.stable_cnt = 0;
				gHDMI20RxHandler[port].videoAbnormalCnt = 0;
				gHDMI20RxHandler[port].videoAbResetCnt = 0;
				gHDMI20RxHandler[port].phyLostCnt = 0;
				gHDMI20RxHandler[port].phy_rst_request = 1;
				gHDMI20RxHandler[port].locking_time = 0;
			}
		}

		gHDMI20RxHandler[port].scdc_change  = false;

	}
	else{
		gHDMI20RxHandler[port].scdc_change  = false;

	}
}

static int __HDMI20_HAL_H15Ax_IsHPD_Up(UINT8 port)
{
	UINT32 hpd;

	hpd = gHDMI20RxHandler[port].ctrl.bsp_read(0x0) & 0x00000001;

	//if(lx_chip_rev( ) >= LX_CHIP_REV(H15, A1)){
	if(gHPDInverseMode){
		if(hpd) hpd = 0;
		else hpd = 1;
	}
	else{
		if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
			if(hpd) hpd = 1;
			else hpd = 0;
		}
		else{
			if(port == 1){
				if(hpd) hpd = 0;
				else hpd = 1;
			}
			else{
				if(hpd) hpd = 1;
				else hpd = 0;
			}
		}
	}

	return hpd;
}

static void __HDMI20_HAL_DBG_PrintECCMonitor(UINT8 port)
{
	UINT32 data0;
	UINT32 data1;

	UINT32 ch0 = 0;
	UINT32 ch1 = 0 ;
	UINT32 ch2 = 0;

	data0 = gHDMI20RxHandler[port].ctrl.bsp_read(0x824);
	data1 = gHDMI20RxHandler[port].ctrl.bsp_read(0x828);

	if(data0 && 0x80000000 != 0){ ch0 = (data0 & 0x7FFF0000)>>16; }
	
	if(data1 && 0x00008000 != 0){ ch1 = (data1 & 0x00007FFF); }
	
	if(data1 && 0x80000000 != 0){ ch2 = (data1 & 0x7FFF0000)>>16; }

	HDMI20_VIDEO("DBG %d : CRC CH0[%d] CH1[%d] CH2[%d] | Data0[0x%08x] Data1[0x%08x]\n", port ,ch0, ch1, ch2, data0, data1);

}

static void __HDMI20_HAL_H15Ax_DBG_PrintSyncMonitor(UINT8 port, UINT8 diffmode)
{
	int i;
	UINT32 currData[2][8] = {0,};
	int diff = 0;

	if(port == 0){
		if(diffmode >=1 ){
			currData[port][0] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x208);
			currData[port][1] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C);
			currData[port][2] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x210);
			currData[port][3] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x214);
			currData[port][4] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x408);
			currData[port][5] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x40C);
			currData[port][6] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x410);
			currData[port][7] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x414);

			for(i=0; i<=7; i++){
				if(currData[port][i] != gPrevSyncData[port][i]){
					diff++;	
				}
			}

			if(diff >= 1){
				HDMI20_VIDEO(" Rx[%d] Sync Changed[%d]. From  [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",\
						port, diff, gPrevSyncData[port][0], gPrevSyncData[port][1], gPrevSyncData[port][2], gPrevSyncData[port][3], gPrevSyncData[port][4], gPrevSyncData[port][5], \
						gPrevSyncData[port][6], gPrevSyncData[port][7]); 
				HDMI20_VIDEO(" Rx[%d] Sync Changed[%d]. To  [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",\
						port, diff , currData[port][0], currData[port][1], currData[port][2], currData[port][3], currData[port][4], currData[port][5], \
						currData[port][6], currData[port][7]); 
				
			}

			for(i=0; i<=7; i++){
				gPrevSyncData[port][i] = currData[port][i];
			}
		}
		else{
			HDMI20_VIDEO(" Rx[%d] => [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",port,\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x208),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x210),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x214),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x408),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x40C), __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x410),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x414));
		}		
		/* Reset Sync monitors */
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x20C, 0);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x210, 0);
	}
	else if(port == 1){
		if(diffmode >=1 ){
			currData[port][0] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x208);
			currData[port][1] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C);
			currData[port][2] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x210);
			currData[port][3] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x214);
			currData[port][4] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x408);
			currData[port][5] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x40C);
			currData[port][6] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x410);
			currData[port][7] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x414);

			for(i=0; i<=7; i++){
				if(currData[port][i] != gPrevSyncData[port][i]){
					diff++;	
				}
			}

			if(diff >= 1){
				HDMI20_VIDEO(" Rx[%d] Sync Changed[%d]. From  [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",\
						port, diff, gPrevSyncData[port][0], gPrevSyncData[port][1], gPrevSyncData[port][2], gPrevSyncData[port][3], gPrevSyncData[port][4], gPrevSyncData[port][5], \
						gPrevSyncData[port][6], gPrevSyncData[port][7]); 
				HDMI20_VIDEO(" Rx[%d] Sync Changed[%d]. To  [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",\
						port, diff , currData[port][0], currData[port][1], currData[port][2], currData[port][3], currData[port][4], currData[port][5], \
						currData[port][6], currData[port][7]); 
				
			}

			for(i=0; i<=7; i++){
				gPrevSyncData[port][i] = currData[port][i];
			}
		}
		else{
			HDMI20_VIDEO(" Rx[%d] => [0x%08x] / HS[0x%08x] / H[0x%08x] / [0x%08x] / [0x%08x] /  [0x%08x] / [0x%08x] / [0x%08x]\n",port,\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x208),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x210),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x214),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x408),  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x40C), __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x410),\
					__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x414));
		}		
		/* Reset Sync monitors */
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x20C, 0);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x210, 0);

	}
}

static void __HDMI20_HAL_DBG_PrintPixelCRCMonitor(UINT8 port, UINT8 diffmode)
{
	UINT32 currData = 0;

	if(port == 0){
		if(diffmode == 0){
			HDMI20_VIDEO(" Rx[%d] CRC => [0x%08x]\n", port,  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x1840));
		}
		else{
			currData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x1840);
			if(currData != gPrevCrcData[port]){
				HDMI20_VIDEO(" Rx[%d] CRC Monitor changed from [0x%08x] => [0x%08x]\n", port,  gPrevCrcData[port], currData);
				gPrevCrcData[port] = currData;
			}
		}
	}
	else if(port == 1){
		if(diffmode == 0){
			HDMI20_VIDEO(" Rx[%d] CRC => [0x%08x]\n", port,  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x1840));
		}
		else{
			currData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x1840);
			if(currData != gPrevCrcData[port]){
				HDMI20_VIDEO(" Rx[%d] CRC changed from [0x%08x] => [0x%08x]\n", port,  gPrevCrcData[port], currData);
				gPrevCrcData[port] = currData;
			}
		}
	}
}

static void __HDMI20_HAL_DBG_PrintCVIMonitor(UINT8 port, UINT8 diffmode)
{
	UINT32 currData[2] = {0,};	

	if(port == 0){
		HDMI20_REG_RD(0xC8000258, currData[0]);
		HDMI20_REG_RD(0xC80002D0, currData[1]);
	}
	else if(port == 1){
		HDMI20_REG_RD(0xC8000358, currData[0]);
		HDMI20_REG_RD(0xC80003D0, currData[1]);
	}

	currData[1] &= (0x0FFF0000);

	if(diffmode == 0){
		HDMI20_VIDEO(" Rx[%d] CVI Offset[0x%08x], diff_V_cnt[0x%08x]\n", port, currData[0], currData[1]);
	}
	else{
		if((currData[0] != gPrevCVISyncData[port][0]) || ( currData[1] != gPrevCVISyncData[port][1])){
			HDMI20_VIDEO(" Rx[%d] CVI Offset[0x%08x]/[0x%08x], diff_V_cnt[0x%08x]/[0x%08x]\n", port, currData[0], gPrevCVISyncData[port][0],  currData[1], gPrevCVISyncData[port][1]);
		}

		gPrevCVISyncData[port][0] = currData[0];
		gPrevCVISyncData[port][1] = currData[1];
	}
}

static void __HDMI20_HAL_H15Ax_TxThread(void)
{
	UINT32 tx_hpd_value = 0x0;

	while(1){
		if(gTxThreadAlive <= 0) { HDMI20_INFO("Kill Thread\n"); break; }

		if((gWasTXInitiated == 0)||(gUseHDMI20Tx == 0)){
			HDMI20_INFO("Tx was not initiated or, TX use flag is zero\n");
			break;
		}

		tx_hpd_value = __HDMI20_HAL_H15Ax_Tx_Read_Link(0xc010);

		if(tx_hpd_value == 0x000000F3){
			if(gVideoDebugLevel == 4201) HDMI20_VIDEO("TX : HPD is UP \n");
			/////////// SCRAMBLING  ///////////
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f800, 0x00000054);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f804, 0x00000020);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f808, 0x00000003);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f810, 0x00000010);
			msleep(100);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f800, 0x00000054);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f804, 0x00000010);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f808, 0x00000001);
			__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f810, 0x00000010);
			msleep(500);
		}
		else{
			if(gVideoDebugLevel == 4201) HDMI20_VIDEO("TX : HPD is down[0x%08x]\n", tx_hpd_value);
		
			__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2000, 0x00000006);
			__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2100, 0x00000006);
			
			__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x0004, 0x00000012);
		}

		gTxThreadStamp = 1;
		msleep(gTxThreadMsleep);
	}
	
	gTxThreadStamp = 0;
	gTxThreadAlive = 0;
}

static void __HDMI20_HAL_H15Ax_MainThread(void)
{
	int i,y,surge_cnt, surge_result,ret;
	int tx5Vconnected;
	UINT32 tx_hpd_value = 0x0;
	UINT32 register_value = 0x0;
	UINT32 hpdWaitCnt[2] = {0,};
	UINT32 datapacketUpdateTimer[2] = {0,};
	UINT32 product_name = 0;
	UINT32 data;
	UINT32 phyValue0, phyValue1 = 0;
	UINT32 divider = 0;
	UINT32 hvtotal;
	UINT32 dbg_vfreq;
	UINT32 vtotal;
	UINT32 video_format;
	unsigned int pEdidAddress = 0;

	UINT32 phy02value = 0;
	UINT32 addr;
	UINT32 currentJiffiesMsec = 0;

	HDMI20_INFO("Thread Start \n");
	surge_cnt = 0;
	gMainThreadStamp = 0;
	/* Important to sleep at least for 600msec with HPD down */
	for(i=0;i<HDMI20_NUM_OF_RX_PORTS;i++){
		HDMI20_INFO("Force HPD down for 600msec on HDMI2.0 port [%d] \n", i);
		__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(i ,0, 1);
	}
	

	for(i=0;i<HDMI20_NUM_OF_RX_PORTS;i++){
		pEdidAddress = (unsigned int)gHDMI20RxHandler[i].topmem.pSwAddr + (unsigned int)HDMI20_HAL_H15AX_RX_INTERNAL_EDID_OFFSET;
		if(pEdidAddress != NULL){
			if(gHDMI20RxHandler[i].wasEdidWritten == 1){
				HDMI20_INFO("NOTI %d : Internal EDID was written before MainThread start\n", i); 
				ret = memcmp((void *)&gHDMI20RxHandler[i].edidData, (void*)pEdidAddress, HDMI20_HAL_SIZE_OF_EDID);
				if(ret == 0){
					HDMI20_INFO("NOTI %d : Current internal EDID is same as the written EDID\n", i);
				}
				else{
					HDMI20_INFO("NOTI %d : Current internal EDID is different as the written EDID[%d]\n", i, ret);
					addr = 	(UINT32)HDMI20_HAL_H15AX_RX_INTERNAL_EDID_OFFSET;
					for(y=0; y<(HDMI20_HAL_SIZE_OF_EDID >> 2); y++){
						if(i == 0){
							__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(addr, gHDMI20RxHandler[i].edidData[y]);
						}
						else if(i ==1){
							__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(addr, gHDMI20RxHandler[i].edidData[y]);
						}
						addr += 4;
					}
					HDMI20_INFO("NOTI %d : Update Internal EDID Data from the MW complete\n", i);
				}
			}
			else{
				HDMI20_INFO("NOTI %d : No EDID data was delivered to driver before Main Thread Start\n", i);
				memset((void *)pEdidAddress, 0, HDMI20_HAL_SIZE_OF_EDID);
			}
		}
		else{
			HDMI20_INFO("WARN %d :SKIP EDID verification due to NULL address pointer\n",i);
		}
	
		gHDMI20RxHandler[i].connected5Vtime = 0;
		gHDMI20RxHandler[i].stablehdcp14time = 0;
	
	}
	msleep(600);


	while(1)
	{
		/* Force Kill Thread */
		if(gMainThreadAlive <= 0){ HDMI20_INFO("Kill Thread\n"); break; }

		for(i=0;i<gNumOfRunningHDMI;i++){
			/* Step 1. Check Required Information */
			HDMI20_HAL_Check5VLevelInfo(i, &tx5Vconnected); // Check 5V

			if(tx5Vconnected){
				/* DBG : View sync status */
				if((gVideoDebugLevel == 1919)||(gVideoDebugLevel == 1921)){ __HDMI20_HAL_H15Ax_DBG_PrintSyncMonitor(i, gSyncMonitorRunMode);}
				if((gVideoDebugLevel == 1920)||(gVideoDebugLevel == 1921)){__HDMI20_HAL_DBG_PrintPixelCRCMonitor(i, gCrcMonitorRunMode);}
				if((gVideoDebugLevel == 1922)||(gVideoDebugLevel == 1921)){__HDMI20_HAL_DBG_PrintCVIMonitor(i, gCVISyncMonitorRunMode);}
				if(gVideoDebugLevel == 2002){HDMI20_INFO(" DBG %d : Current 5V stable time=>%d msec\n",  gHDMI20RxHandler[i].connected5Vtime);}
				if(gVideoDebugLevel == 1132){HDMI20_INFO(" DBG %d : 8FC[0x%08x] / 0xFC[0x%08x] / 0xE0[0x%08x] / 0x600[0x%08x] /stable[%d]\n",\
						i, gHDMI20RxHandler[i].ctrl.bsp_read(0x8fc), gHDMI20RxHandler[i].ctrl.bsp_read(0xfc), gHDMI20RxHandler[i].ctrl.bsp_read(0xe0), gHDMI20RxHandler[i].ctrl.bsp_read(0x600),gHDMI20RxHandler[i].stablehdcp14time);
				}
			
				/* Surge Monitor */
				surge_result = 0;
				if(i == 0){
					gVideoSurgeMonitor[i][surge_cnt%10] =  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C);
					gVideoSurgeMonitor2[i][surge_cnt%10] =  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x214);
				}
				else {
					gVideoSurgeMonitor[i][surge_cnt%10] =  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C);
					gVideoSurgeMonitor2[i][surge_cnt%10] =  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x214);
				}
				surge_cnt += 1;
				for(y=0;y<10;y=y+2){
					if(gVideoSurgeMonitor[i][y] >= gVideoSurgeMonitor[i][y+1]){
						if(gVideoSurgeMonitor[i][y] - gVideoSurgeMonitor[i][y+1] >=  gVideoSurgeDetectThreshold){
							if(gVideoDebugLevel == 1225){
								HDMI20_VIDEO("DBG %d : Surge tick [y]=[0x%08x], [y+1]=[0x%08x]\n", i, gVideoSurgeMonitor[i][y], gVideoSurgeMonitor[i][y+1]);
							}
							surge_result++;
						}
					}
					else{
						if(gVideoSurgeMonitor[i][y+1] - gVideoSurgeMonitor[i][y] >=  gVideoSurgeDetectThreshold){
							if(gVideoDebugLevel == 1225){
								HDMI20_VIDEO("DBG %d : Surge tick [y]=[0x%08x], [y+1]=[0x%08x]\n", i, gVideoSurgeMonitor[i][y], gVideoSurgeMonitor[i][y+1]);
							}
							surge_result++;
						}
					}
				}
				for(y=0;y<10;y=y+2){
					if(gVideoSurgeMonitor2[i][y] >= gVideoSurgeMonitor2[i][y+1]){
						if(gVideoSurgeMonitor2[i][y] - gVideoSurgeMonitor2[i][y+1] >=  gVideoSurgeDetectThreshold){
							if(gVideoDebugLevel == 1225){
								HDMI20_VIDEO("DBG %d : Surge tick2 [y]=[0x%08x], [y+1]=[0x%08x]\n", i, gVideoSurgeMonitor2[i][y], gVideoSurgeMonitor2[i][y+1]);
							}
							surge_result++;
						}
					}
					else{
						if(gVideoSurgeMonitor2[i][y+1] - gVideoSurgeMonitor2[i][y] >=  gVideoSurgeDetectThreshold){
							if(gVideoDebugLevel == 1225){
								HDMI20_VIDEO("DBG %d : Surge tick2 [y]=[0x%08x], [y+1]=[0x%08x]\n", i, gVideoSurgeMonitor2[i][y], gVideoSurgeMonitor2[i][y+1]);
							}
							surge_result++;
						}
					}
				}
				/* End of Surge Monitor */

				/* Local Mute Color Detection */
				video_format = gHDMI20RxHandler[i].ctrl.bsp_read(0x3A4);
				video_format = (video_format & 0x60); video_format = video_format>>5;
				video_format = video_format & 0x3;

				/* Auto AVmute color select */
				if(video_format == 0){
					__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMuteColor(i,  (UINT32)((gHDMI20RxHandler[i].dataPacket[AVI][5]&0x60)>>5));
				}
				else{
					__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMuteColor(i,  video_format);
				}


				if(gVideoDebugLevel == 1211){
					HDMI20_VIDEO("DBG %d : Video Format register[0x%x], Datapacket[0x%x]\n", i, video_format, (UINT32)((gHDMI20RxHandler[i].dataPacket[AVI][5]&0x60)>>5));
				}

				if(gVideoDebugLevel == 1112){
					HDMI20_VIDEO(" DBG %d : DCM mode [0x%08x] \n", i, (HDMI20_HAL_H15AX_RX_CURRENT_DCM_MASK & gHDMI20RxHandler[i].ctrl.bsp_read(HDMI20_HAL_H15AX_RX_STATUS_REG)));
				}
				if(gVideoDebugLevel == 1923){ __HDMI20_HAL_DBG_PrintECCMonitor(i);}

				/* Keep in track of Htotal value incase of interlaced */
				if(i == 0){
					vtotal = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C) & 0xFFFF0000)>>16;
				}
				else{
					vtotal = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C) & 0xFFFF0000)>>16;
				}

				if(gHDMI20RxHandler[i].vtotalbuffer >= vtotal){
					gHDMI20RxHandler[i].vtotalbuffer = vtotal;
				}
				else{
					if(gHDMI20RxHandler[i].vtotalbuffer == 0){
						gHDMI20RxHandler[i].vtotalbuffer = vtotal;
					}
					else if(vtotal - gHDMI20RxHandler[i].vtotalbuffer >= 2){
						gHDMI20RxHandler[i].vtotalbuffer = vtotal;
					}
					else{
						/* DO NOT UPDATE */
					}
				}

				
				if(gHDMI20RxHandler[i].is5Vconnected == 0){
					HDMI20_VIDEO("Notice [%d] : 5V was up without connection function\n", i);
					HDMI20_HAL_ConnectCable(i, ON);
				}
				else{
					__HDMI20_HAL_H15Ax_UpdateSCDC(i);			/* Update SCDC always */
					__HDMI20_HAL_H15Ax_UpdatePhyLockFlag(i);	/* Update Phy Lock Flag */

					/* SW W/A for CVI bug.. Only work for Chips Under H15A1 */
					if((lx_chip_rev() <= LX_CHIP_REV(H15,A1)||(gVideoDebugLevel == 914))){
						if(i == 0){
							//HDMI20_VIDEO("SW WA for CVI Bug only for H15Ax\n");
							if(pHDMI_MUX_CTRL_B != 0x0){
								*pHDMI_MUX_CTRL_B &= 0xFFFFFF00;
								*pHDMI_MUX_CTRL_B |= 0x00000011;
							}
							if(pHDMI_MUX_CTRL_C != 0x0){
								*pHDMI_MUX_CTRL_C &= 0xFFFFFF00;
								*pHDMI_MUX_CTRL_C |= 0x00000022;
							}
							if(pHDMI_MUX_CTRL_D != 0x0){
								*pHDMI_MUX_CTRL_D &= 0xFFFFFF00;
								*pHDMI_MUX_CTRL_D |= 0x00000033;
							}
						}
					}

					if(gDDCMode[i] == 0){
						currentJiffiesMsec = jiffies_to_msecs(jiffies);
						if(currentJiffiesMsec - gHDMI20RxHandler[i].cable_connect_time >= gHDMI20DdcGpioHoldTime){
							if(gVideoDebugLevel == 803){ __HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(i, 1);}
						}
						else{
							HDMI20_VIDEO("NOTICE [%d] : DDC Hold [0x%x]-[0x%x] < [0x%x]\n",i,currentJiffiesMsec, gHDMI20RxHandler[i].cable_connect_time, gHDMI20DdcGpioHoldTime);
						}


					}


					/* AV Mute */
					if(gVideoDebugLevel == 1052){
						HDMI20_VIDEO("DBG %d : GCP AVMute[0x%x]\n", i, gHDMI20RxHandler[i].dataPacket[GCP][4]);
					}

					/* Calculate Real TMDS, PIXEL CLK */
					gHDMI20RxHandler[i].real_tmds_clk = (gHDMI20RxHandler[i].ctrl.bsp_read(0x9C) & 0x0000FFFF);
					if(gHDMI20RxHandler[i].real_tmds_clk != 0){
						gHDMI20RxHandler[i].real_tmds_clk = ((gHDMI20RxHandler[i].ctrl.bsp_read(0x9C) & 0x0000FFFF) *gHDMI20RxHandler[i].ctrl.md_clk)/4095;

						if(gHDMI20RxHandler[i].currentDCMMode== 0x5){
							divider = 125;			
						}
						else if(gHDMI20RxHandler[i].currentDCMMode== 0x6){
							divider = 150;	
						}
						else if(gHDMI20RxHandler[i].currentDCMMode== 0x7){
							divider = 200;
						}
						else{
							divider =  100;
						}

						gHDMI20RxHandler[i].real_pixel_clk = (gHDMI20RxHandler[i].real_tmds_clk * 100) / divider;

					}
					else{
						gHDMI20RxHandler[i].real_tmds_clk = 0;
						gHDMI20RxHandler[i].real_pixel_clk = 0;
					}

					if(gVideoDebugLevel == 1747){
						hvtotal = (gHDMI20RxHandler[i].timing_info.vtotal * gHDMI20RxHandler[i].timing_info.htotal);

						if(hvtotal != 0){
							dbg_vfreq = ((gHDMI20RxHandler[i].real_pixel_clk * 1000) / hvtotal);
						}
						else{
							dbg_vfreq = 0;
						}

						HDMI20_VIDEO("DBG %d : Polling TMDS CLK[%d]KHz, PIXEL CLK[%d]KHz, VFREQ[%d]/INFO[%d]\n", i, gHDMI20RxHandler[i].real_tmds_clk, gHDMI20RxHandler[i].real_pixel_clk, dbg_vfreq, gHDMI20RxHandler[i].real_vfreq);
					}

					hvtotal = (gHDMI20RxHandler[i].timing_info.vtotal * gHDMI20RxHandler[i].timing_info.htotal);

					#if ENABLE_VFREQ_MONITOR
					hvtotal = (gHDMI20RxHandler[i].timing_info.vtotal * gHDMI20RxHandler[i].timing_info.htotal);
					
					if(hvtotal != 0){
						dbg_vfreq = ((gHDMI20RxHandler[i].real_pixel_clk * 1000) / hvtotal);

						if(dbg_vfreq >= gHDMI20RxHandler[i].real_vfreq){
							if((dbg_vfreq - gHDMI20RxHandler[i].real_vfreq) >=gVideoVfreqMonitorThreshhold){
								HDMI20_VIDEO("Warning %d : Vfreq diff is over [%d]. Change Vfreq to [%d]\n", i, (dbg_vfreq - gHDMI20RxHandler[i].real_vfreq), dbg_vfreq);
								gHDMI20RxHandler[i].real_vfreq = dbg_vfreq;
								if((gHDMI20RxHandler[i].ctrl.bsp_read(0x400) != 0)&&(gVideoResetManualOnVfreqChange > 0)){
									gHDMI20RxHandler[i].ctrl.bsp_write(0x400, 0x0);
									gHDMI20RxHandler[i].ctrl.bsp_write(0x404, 0x0);
								}
							}
						}
						else{
							if((gHDMI20RxHandler[i].real_vfreq - dbg_vfreq) >=gVideoVfreqMonitorThreshhold){
								HDMI20_VIDEO("Warning %d : Vfreq diff is over [%d]. Change Vfreq to [%d]\n", i, (dbg_vfreq - gHDMI20RxHandler[i].real_vfreq), dbg_vfreq);
								gHDMI20RxHandler[i].real_vfreq = dbg_vfreq;
								if((gHDMI20RxHandler[i].ctrl.bsp_read(0x400) != 0) &&(gVideoResetManualOnVfreqChange > 0)){
									gHDMI20RxHandler[i].ctrl.bsp_write(0x400, 0x0);
									gHDMI20RxHandler[i].ctrl.bsp_write(0x404, 0x0);
								}
							}
						}

						if(gHDMI20RxHandler[i].real_vfreq != dbg_vfreq){

							if( (gHDMI20RxHandler[i].real_vfreq <= gVideoVfreqMinValue )||(gHDMI20RxHandler[i].real_vfreq >=   gVideoVfreqMaxValue)){
								HDMI20_VIDEO("Notice %d : Update Vfreq even diff threshold in under[%d] due to min[%d]/max[%d] condition\n", i, gVideoVfreqMonitorThreshhold, gVideoVfreqMinValue,gVideoVfreqMaxValue);
								gHDMI20RxHandler[i].real_vfreq = dbg_vfreq;
							}
							else{
								if(gVideoDebugLevel == 153){
									HDMI20_VIDEO("Warning %d : Real Vfreq and Signed Vfreq is different! TimingInfo[%d] != Calculated[%d]\n", i, gHDMI20RxHandler[i].real_vfreq, dbg_vfreq);
								}
							}
						}
					}
					else{
						dbg_vfreq = 0;
					}
					#endif

					/* AV MUTE HOLD TIME */
					if(gVideoClearAVMuteByTimer >0){
						if(gHDMI20RxHandler[i].ctrl.isMute != 0){
							currentJiffiesMsec = jiffies_to_msecs(jiffies);
							if(currentJiffiesMsec - gHDMI20RxHandler[i].avmute_on_time >= gHDMI20AVMuteHoldTime){
								//if(gHDMI20RxHandler[i].ctrl.bsp_read(0x30) & 0x00000001)
								//{
									HDMI20_VIDEO("NOTICE [%d] : AV Mute released by hold time! [0x%x]-[0x%x] >= [0x%x]\n",i,currentJiffiesMsec, gHDMI20RxHandler[i].avmute_on_time, gHDMI20AVMuteHoldTime);
									gHDMI20RxHandler[i].ctrl.isMute = 0;
									//__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 0);
									__HDMI20_HAL_H15Ax_SetVideoColorDomain(i);
									__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(i);
									hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
								//}
								//else{
									//HDMI20_VIDEO("NOTICE [%d] : AV Mute released by hold time but PHY is DOWN! [0x%x]-[0x%x] >= [0x%x]\n",i,currentJiffiesMsec, gHDMI20RxHandler[i].avmute_on_time, gHDMI20AVMuteHoldTime);
								//}
							}
							else{
								HDMI20_VIDEO("NOTICE [%d] : AVMute Hold [0x%x]-[0x%x] < [0x%x]\n",i,currentJiffiesMsec, gHDMI20RxHandler[i].avmute_on_time, gHDMI20AVMuteHoldTime);
								if(gVideoDebugLevel !=  741) gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
							}
						}
					}

					if(gHDMI20RxHandler[i].connect_request){
						if(gHDMI20RxHandler[i].hpd_request == 0){
							if(__HDMI20_HAL_H15Ax_IsHPD_Up(i) == 0){
								HDMI20_VIDEO("Notice [%d] : Executing Connection Request\n", i);
								gHDMI20RxHandler[i].hpd_request = 1;
								gHDMI20RxHandler[i].locking_time = 0;
								gHDMI20RxHandler[i].phy_rst_request = 1;
								HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);

								if(gVideoDelayPhyInit>0){
									HDMI20_VIDEO("DBG %d : Debugger Activated : Delay Phy init for [%d] msec\n",gVideoDelayPhyInit);
									msleep(gVideoDelayPhyInit);
								}

								ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
								if(ret<0){
									HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
								}
						

								if(gHDMI20RxHandler[i].isHDCP14 == 0) HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_REQUEST_HPD ,i);
								else{
									__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(i, 1, 1);
								}	
							}
							else{
								if(gHDMI20RxHandler[i].isPhyLocked){
									gHDMI20RxHandler[i].locking_time++;
									HDMI20_VIDEO("Notice [%d] : Phy Lock success(Connection Steps) : Time[%d]msec\n", i,\
											gHDMI20RxHandler[i].locking_time * gMainThreadMsleep);

									__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(i, 1, 0, __L__);
								
									if( gHDMI20RxHandler[i].isLongPhyRecovery> 0){
										HDMI20_VIDEO("Notice [%d] Main Reset Due to long clock recovery\n", i);
										gHDMI20RxHandler[i].isLongPhyRecovery = 0; 

										if(gHDMI20RxHandler[i].isHDCP22){
											HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
										}

										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}

										gHDMI20RxHandler[i].isManualSyncMode = 0;
										HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);
										gHDMI20RxHandler[i].locking_time = 0;
										gHDMI20RxHandler[i].phy_rst_request = 1;
										ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
										if(ret<0){
											HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
										}
										__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
									}
									else{
										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}

										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xFFFE);
										msleep(50);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF4, 0xFFFF);
										gHDMI20RxHandler[i].isManualSyncMode = 0;

										/* Important to clear ManualSYnc value */
										data = gHDMI20RxHandler[i].ctrl.bsp_read(0x400);
										data &= (~0x0000000F);
										gHDMI20RxHandler[i].ctrl.bsp_write(0x400, data);
										gHDMI20RxHandler[i].ctrl.bsp_write(0x404, 0x0);

										/* Audio Output Enable, Audio FIFO reset */
										hdmi_rx_ctrl_audio_cfg(&gHDMI20RxHandler[i].ctrl);
										hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										/* Packet Fifo reset */
										hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										/* Inform HDCP2.2 that video is stable */
										if(gHDMI20RxHandler[i].isHDCP14 == 0) HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_GOOD_VIDEO, i);
										/* Update Video*/
										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
										gHDMI20RxHandler[i].videoAbnormalCnt = 0;
										gHDMI20RxHandler[i].videoAbResetCnt = 0;
										gHDMI20RxHandler[i].phyLostCnt = 0;
										__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i, 0);
										/* Added 2014/08/04 taejun.lee */
										__HDMI20_HAL_H15Ax_GetAudioInfo(i);

										/* Clear Connect Step flag */
										gHDMI20RxHandler[i].connect_request = 0;
										gHDMI20RxHandler[i].phy_rst_request = 0;
									}
								}
								else{
									HDMI20_VIDEO("Notice [%d] : Waiting Phy Lock(Connection Steps)\n", i);
									gHDMI20RxHandler[i].locking_time++;

									if(gHDMI20RxHandler[i].scdc_change != 0){
										__HDMI20_HAL_H15Ax_Rx_CheckSCDC(i);
									}
									else if((gHDMI20RxHandler[i].phyInitiatedMode == 0)&& (gPhyLinkResetMode >0) &&(gHDMI20RxHandler[i].locking_time >= gPhyResetWaitTime)&&(gVideoDebugLevel != 4949)&&(gHDMI20RxHandler[i].isAudioARCMode == 0)){

										phyValue0 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[i].ctrl, 0x69, 0x9);
										phyValue1 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[i].ctrl, 0x69, 0xa);

										HDMI20_VIDEO("Warning [%d] : Phy was not locked with in [%d] msec(connect). PhyReset. Phy0x9[%x]/0xA[0x%x]\n", i, (gHDMI20RxHandler[i].locking_time * gMainThreadMsleep), phyValue0, phyValue1);

										gHDMI20RxHandler[i].isLongPhyRecovery = 1; 
										/*
										 * Camera :  
										 * Phy0x9[0x368d]/0xA[0x26c0]
										 * Phy0x9[308d]/0xA[0x26c0]
										 * Phy0x9[98d]/0xA[0x26c1]
										 * HX906 :   
										 * 
										 * Phy0x9[368d]/0xA[0x26c0]
										 * Phy0x9[328d]/0xA[0x26c0]
										 * 
										 *
										 */

										if((((phyValue0 == 0x368D)&&(phyValue1 == 0x26C0))|| ((phyValue0 == 0x308D)&&(phyValue1 == 0x26C0)) || ((phyValue0 == 0x98D)&&(phyValue1 == 0x26C1)))&&(gVideoFujiFilmNoResetAccum[i] < gVideoFujiFilmThresHold)){
											HDMI20_VIDEO("Notice %d : Will not reset phy due to FUJIFILM camera(connect)\n", i);	
											gVideoFujiFilmNoResetAccum[i]++;
											gHDMI20RxHandler[i].locking_time = 0;
										}
										else{
											if(gVideoFujiFilmNoResetAccum[i] >= gVideoFujiFilmThresHold){
												HDMI20_VIDEO("Notice %d : FujiFilm no phy reset time out[%d]\n", i ,gVideoFujiFilmNoResetAccum[i]);
												gVideoFujiFilmNoResetAccum[i] = 0;
											}

											if(gHDMI20RxHandler[i].isPhyAlterMode == 0){
												gHDMI20RxHandler[i].isPhyAlterMode = 1;
											}
											else{
												gHDMI20RxHandler[i].isPhyAlterMode = 0;
											}

											if(gLinkResetModeDetectionMode > 0){
												/* Add HDMI Link mode-detection reset */
												if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
												gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
												msleep(100);
											}

											ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
											if(ret<0){
												HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
											}
											gHDMI20RxHandler[i].locking_time = 0;
										}
									}
									
									/* 20140925 won.hur : Added to reset nCTS value */
									__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i, 1);
								}
							}
						}
						else{
							if(__HDMI20_HAL_H15Ax_IsHPD_Up(i)){
								HDMI20_VIDEO("Notice [%d] : Got HPD Up from connection(Connections Steps)\n", i);
								gHDMI20RxHandler[i].hpd_request = 0;
							}
							else{
								HDMI20_VIDEO("Notice [%d] : Waiting HPD UP(Connection Steps)\n", i);
								hpdWaitCnt[i]++;
								if(hpdWaitCnt[i] >= gHPDRequestWaitThreshold){
									HDMI20_VIDEO("Notice [%d] : Call HPD request one more time.[%d]/[%d]\n", i, hpdWaitCnt[i], 	gHPDRequestWaitThreshold);
									HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_REQUEST_HPD ,i);

									hpdWaitCnt[i] = 0;
								}
							}
						}
					}
					else{
						if(gHDMI20RxHandler[i].phy_rst_request)
						{
							if(gHDMI20RxHandler[i].isPhyLocked){
								if(gHDMI20RxHandler[i].isManualSyncMode == 1){
									HDMI20_VIDEO("Notice [%d] : Recover from 3D manual Sync mode. Reset Link TMDS\n", i);

									if(gVideoDebugLevel == 1219){
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										gHDMI20RxHandler[i].isManualSyncMode = 0;
									}
									else {
										
										if(gHDMI20RxHandler[i].isHDCP22){
											HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
										}
										
										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xFFFF);
										msleep(50);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF4, 0xFFFF);
										gHDMI20RxHandler[i].isManualSyncMode = 0;
									
										gHDMI20RxHandler[i].locking_time = 0;
										gHDMI20RxHandler[i].phy_rst_request = 1;
										ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
										if(ret<0){
											HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
										}
									}
								}
								else{
									gHDMI20RxHandler[i].locking_time++;
									HDMI20_VIDEO("Notice [%d] : Phy Lock success(Normal Run) : Time[%d]msec\n", i,\
											gHDMI20RxHandler[i].locking_time * gMainThreadMsleep);
									if( gHDMI20RxHandler[i].isLongPhyRecovery> 0){
										HDMI20_VIDEO("Notice [%d] Main Reset Due to long clock recovery\n", i);
										gHDMI20RxHandler[i].isLongPhyRecovery = 0; 

										if(gHDMI20RxHandler[i].isHDCP22){
											HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
										}

										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}
										data = gHDMI20RxHandler[i].ctrl.bsp_read(0x0);
										HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);
										gHDMI20RxHandler[i].isManualSyncMode = 0;

										gHDMI20RxHandler[i].locking_time = 0;
										gHDMI20RxHandler[i].phy_rst_request = 1;
										ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
										if(ret<0){
											HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
										}
										__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
									}
									else if(gHDMI20RxHandler[i].isResetAfterPhyLock == 1){
										HDMI20_VIDEO("Notice [%d] Main Reset Due to clock recovery\n", i);
										gHDMI20RxHandler[i].isResetAfterPhyLock = 0; 

										if(gHDMI20RxHandler[i].isHDCP22){
											HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
										}

										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}
										HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);

										gHDMI20RxHandler[i].isManualSyncMode = 0;

										gHDMI20RxHandler[i].locking_time = 0;
										gHDMI20RxHandler[i].phy_rst_request = 1;
										ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
										if(ret<0){
											HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
										}
										__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);

									}
									else {
										if(gHDMI20RxHandler[i].isHDCP22){
											//HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
											//msleep(50);
											HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_GOOD_VIDEO, i);
										}

										__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(i, 1, 0, __L__);

										/* Clear Timing Info */
										__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 1);
										/*
										if(gVideoDebugLevel != 2525){
											for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
												hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
												hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											}
										}*/
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xFFFE);
										msleep(50);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF4, 0xFFFF);
										gHDMI20RxHandler[i].isManualSyncMode = 0;
										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;

										/* Important to clear ManualSYnc value */
										data = gHDMI20RxHandler[i].ctrl.bsp_read(0x400);
										data &= (~0x0000000F);
										gHDMI20RxHandler[i].ctrl.bsp_write(0x400, data);
										gHDMI20RxHandler[i].ctrl.bsp_write(0x404, 0x0);

										/* Audio Output Enable, Audio FIFO reset */
										hdmi_rx_ctrl_audio_cfg(&gHDMI20RxHandler[i].ctrl);
										hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										/* Packet Fifo reset */
										hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										/* Inform HDCP2.2 that video is stable */
										/* Do not know where HDCP2.2 have normal run phy init case..
										 * Leave this until MASTER device supports HDCP2.2 */
										if(gHDMI20RxHandler[i].isHDCP14 == 0)HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_GOOD_VIDEO, i);

										/* Update Video*/
										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
										gHDMI20RxHandler[i].videoAbnormalCnt = 0;
										//gHDMI20RxHandler[i].videoAbResetCnt = 0;
										gHDMI20RxHandler[i].phyLostCnt = 0;
										__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i,0);

										/* Added 2014/08/04 taejun.lee */
										__HDMI20_HAL_H15Ax_GetAudioInfo(i);
										gHDMI20RxHandler[i].phy_rst_request = 0;
									}
								}
							}
							else{
								HDMI20_VIDEO("Notice [%d] : Waiting Phy Lock(Normal Run) \n", i);
								gHDMI20RxHandler[i].locking_time++;

								if(gHDMI20RxHandler[i].scdc_change != 0){
									__HDMI20_HAL_H15Ax_Rx_CheckSCDC(i);
								}
								else if( (gHDMI20RxHandler[i].phyInitiatedMode == 0)&&(gPhyLinkResetMode >0) &&(gHDMI20RxHandler[i].locking_time >= gPhyResetWaitTime)&&(gVideoDebugLevel != 4949)&&(gHDMI20RxHandler[i].isAudioARCMode == 0)){
									
									phyValue0 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[i].ctrl, 0x69, 0x9);
									phyValue1 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[i].ctrl, 0x69, 0xa);

									HDMI20_VIDEO("Warning [%d] : Phy was not locked with in [%d] msec(normal). PhyReset.Phy0x9[%x]/0xA[0x%x] \n", i, (gHDMI20RxHandler[i].locking_time * gMainThreadMsleep), phyValue0, phyValue1);

									gHDMI20RxHandler[i].isLongPhyRecovery = 1;
									
									if((phyValue0 == 0x368D)&&(phyValue1 == 0x26C0)){
										HDMI20_VIDEO("Notice %d : Will not reset phy due to FUJIFILM camera(normal)\n", i);	
										gHDMI20RxHandler[i].locking_time = 0;
									}
									else{
										gHDMI20RxHandler[i].isPhyAlterMode = 0;

										if(gLinkResetModeDetectionMode > 0){			
											/* Add HDMI Link mode-detection reset */
											if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
											gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
											msleep(100);
										}

										ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
										if(ret<0){
											HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
										}
										gHDMI20RxHandler[i].locking_time = 0;
									}
								}

								/* 20140925 won.hur : Added to reset nCTS value */
								__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i, 1);

							}
						}
						else
						{
							if(__HDMI20_HAL_H15Ax_IsHPD_Up(i)&&( gHDMI20RxHandler[i].isPhyLocked)){
								if(gHDMI20RxHandler[i].phyLostCnt > 0){
									HDMI20_VIDEO("Notice %d : Recover from phy lock loss[%d]\n", i, gHDMI20RxHandler[i].phyLostCnt); 
									gHDMI20RxHandler[i].phyLostCnt = 0;
							
									if(gLinkResetModeDetectionMode > 0){			
										/* Add HDMI Link mode-detection reset */
										if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										msleep(100);
									}

									if(gHDMI20RxHandler[i].isHDCP22){
										//HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
										//msleep(50);
										HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_GOOD_VIDEO, i);
									}
								}

								hpdWaitCnt[i] = 0;

								if((gHDMI20RxHandler[i].scdc_change != 0) || (gHDMI20RxHandler[i].video_change != 0) ||\
										(gHDMI20RxHandler[i].audio_change != 0) || (gHDMI20RxHandler[i].tmds_change != 0))
								{
									//gHDMI20RxHandler[i].audioMuteEableCnt = 0;
									__HDMI20_HAL_H15Ax_AudioReset(i);
									gHDMI20RxHandler[i].audio_change = 1;
									HDMI20_VIDEO("Notice [%d] : Flag change => SCDC[%d] / VC[%d] / AC[%d] / TC[%d] / AKSV[%d]\n", i,\
											gHDMI20RxHandler[i].scdc_change, gHDMI20RxHandler[i].video_change,\
											gHDMI20RxHandler[i].audio_change, gHDMI20RxHandler[i].tmds_change, gHDMI20RxHandler[i].aksv_change);
								}

								if((gHDMI20RxHandler[i].scdc_change != 0) || (gHDMI20RxHandler[i].video_change != 0) ||\
										(gHDMI20RxHandler[i].tmds_change != 0) || (gHDMI20RxHandler[i].aksv_change != 0))
								{
									if(gHDMI20RxHandler[i].scdc_change != 0){
										__HDMI20_HAL_H15Ax_Rx_CheckSCDC(i);
									}
									else if(gHDMI20RxHandler[i].tmds_change != 0){
										HDMI20_VIDEO("Notice [%d] : TMDS Change detected. Stable cnt[%d]\n", i, gHDMI20RxHandler[i].ctrl.stable_cnt);
										gHDMI20RxHandler[i].video_change = 0; // Disable VideoChange Sequence on TMDS&VIDEO both change

										if((gHDMI20RxHandler[i].ctrl.stable_cnt >= gVideoStableCheckCnt)||(gHDMI20RxHandler[i].phyLostCnt > gVideoPhyLostThreshold)){
											HDMI20_VIDEO("Notice [%d] : TMDS Change on stable Video Cnt. Reset Phy.\n", i);
											gHDMI20RxHandler[i].locking_time = 0;
											gHDMI20RxHandler[i].phy_rst_request = 1;

											/* Clear Timing Info */
											__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 1);
											gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
											/* Inform HDCP2.2 to reset */
											if(gHDMI20RxHandler[i].isHDCP22){
												HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
											}

											if(gLinkResetModeDetectionMode > 0){			
												/* Add HDMI Link mode-detection reset */
												if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
												gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
												msleep(100);
											}
											
											gHDMI20RxHandler[i].isResetAfterPhyLock = 1;
											
											ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
											if(ret<0){
												HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
											}
											//__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);									
										
											/* Reset Audio Clock & FIFO */
											gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0x10);
											hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										}
										else{
											gHDMI20RxHandler[i].tmds_change = false;
											/* Initiate Data Packet Buffer */
											//__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
											/* Get New Data Packet */
											hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[i].ctrl);
											for(y=0;y<PACKET_MAXNUM;y++){
												/* Even though this was opened, it will eventually be closed on Data packet ISR */
												if(y != GCP) __HDMI20_HAL_H15Ax_Packet_Store(&gHDMI20RxHandler[i].ctrl, dataPacketNum[y], 1);
											}

											/* Reset Audio Clock & FIFO */
											gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0x10);
											hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);

										}
									}
									else if(gHDMI20RxHandler[i].video_change != 0){
										HDMI20_VIDEO("Notice [%d] : VIDEO Change detected. stable_cnt[%d]\n", i, gHDMI20RxHandler[i].ctrl.stable_cnt);
										/* Very Important to put video_change on front of Update Timing Info 
										 * video_change flag might get UP on msleep inside of reset_sync_position*/
										gHDMI20RxHandler[i].video_change = false;

										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
										HDMI20_H15A0_ResetDataPacketBuffer(VSI, i);
									}

									if(gHDMI20RxHandler[i].aksv_change != 0){
										HDMI20_VIDEO("Notice [%d] : MainThread AKSV Change detected\n", i);
										gHDMI20RxHandler[i].aksv_change = 0;

										/* Reset Audio Clock & FIFO */
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0x10);
										hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										
										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
									}

									/* DVI to HDMI cable, audio noise issue */
									hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
								}

								if(gHDMI20RxHandler[i].audio_change != 0){
									if(gHDMI20RxHandler[i].ctrl.bsp_read(0x208) & 0x80000000){
										HDMI20_AUDIO("Notice [%d] : Audio PLL Lock Ok\n", i);

										gHDMI20RxHandler[i].ctrl.stable_cnt = 0;

										__HDMI20_HAL_H15Ax_Rx_Audio_Configure(i, 0);
										/* Re-initialize Audio FIFO */
										hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
										gHDMI20RxHandler[i].audio_change = 0;

										__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i, 0);

										/* Added 2014/08/04 taejun.lee */
										__HDMI20_HAL_H15Ax_GetAudioInfo(i);
									}
									else{
										HDMI20_AUDIO("Notice [%d] : Waiting for AUDIO PLL Lock\n", i);
									}
								}

								if(gHDMI20RxHandler[i].ctrl.stable_cnt < gVideoStableCheckCnt)
								{
									gHDMI20RxHandler[i].ctrl.stable_cnt++;
								}
								else if(gHDMI20RxHandler[i].ctrl.stable_cnt == gVideoStableCheckCnt)
								{
									HDMI20_VIDEO("Notice [%d] : Video was stable for [%d] msec.\n", i, gHDMI20RxHandler[i].ctrl.stable_cnt * gMainThreadMsleep);
									if(gVideoStableCheckCnt ==  gVideoStableCntInitial){
										HDMI20_VIDEO("Notice [%d] : Video stable for the first connection. Change Stable Thres\n", i);
										gVideoStableCheckCnt = gVideoStableCntNormalRun;
										gHDMI20RxHandler[i].ctrl.stable_cnt = gVideoStableCheckCnt;
									}
									
									gHDMI20RxHandler[i].ctrl.stable_cnt++;
									__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 0);
								}
								else{
									/* This part will run always on steady state
									 * Insert code as wishing that it would always be called when on steady video */
									__HDMI20_HAL_H15Ax_CheckVideoColorDomain(i);

									/* Galaxy Tab detection */
									//if(gAudioIsGalaxyTab10[i] == 0){
										if((gHDMI20RxHandler[i].dataPacket[SPD][4] == 0x94 )\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][5] == 0x0)\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][6] == 0x53)\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][7] == 0x41)\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][8] == 0x4d)\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][9] == 0x53)\
												&&(gHDMI20RxHandler[i].dataPacket[SPD][10] == 0x55)){

											if((gHDMI20RxHandler[i].dataPacket[AIF][5] == 0x05)&&(gHDMI20RxHandler[i].dataPacket[AIF][8] == 0x0b) && (gHDMI20RxHandler[i].ctrl.bsp_read(0x254) == 0xFC)){
												HDMI20_VIDEO("Notice %d : Detected Galaxy Tab 10.1\n", i);
												gAudioIsGalaxyTab10[i] = 1;
												gHDMI20RxHandler[i].ctrl.bsp_write(0x254, 0x0);
												hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[i].ctrl);
											}
										}
									//}

									if((surge_result != 0)&&( gVideoResetSyncOnSurge >0)){
										HDMI20_VIDEO("Warning %d : Surge insertion detected!\n");
										__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(i);
									}

									if(gVideoDebugLevel == 1227){
										HDMI20_VIDEO(" DBG %d Result[%d] : Surge 1 [0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x]\n",\
												i, surge_result,\
												gVideoSurgeMonitor[i][0],gVideoSurgeMonitor[i][1],gVideoSurgeMonitor[i][2],gVideoSurgeMonitor[i][3],gVideoSurgeMonitor[i][4],\
												gVideoSurgeMonitor[i][5],gVideoSurgeMonitor[i][6],gVideoSurgeMonitor[i][7],gVideoSurgeMonitor[i][8],gVideoSurgeMonitor[i][9]);
										HDMI20_VIDEO(" DBG %d Result[%d] : Surge 2 [0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x][0x%08x]\n",\
												i, surge_result,\
												gVideoSurgeMonitor2[i][0],gVideoSurgeMonitor2[i][1],gVideoSurgeMonitor2[i][2],gVideoSurgeMonitor2[i][3],gVideoSurgeMonitor2[i][4],\
												gVideoSurgeMonitor2[i][5],gVideoSurgeMonitor2[i][6],gVideoSurgeMonitor2[i][7],gVideoSurgeMonitor2[i][8],gVideoSurgeMonitor2[i][9]);
									}

									if(gHDMI20RxHandler[i].isHDCP22Ncap != 0){
										HDMI20_VIDEO("Warning [%d] : Detected HDCP22 NCAP on stable video\n", i);
										gHDMI20RxHandler[i].isHDCP22Ncap = 0;
										__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(i);
									}

									/* Reset Audio FIFO on AUdio FIFO abnormal case */
									if(gHDMI20RxHandler[i].ctrl.bsp_read(0x27C) != 0x4 || gHDMI20RxHandler[i].ctrl.bsp_read(0xFB0) != 0x4){
										if(gHDMI20RxHandler[i].ctrl.bsp_read(0x27C) != 0) HDMI20_VIDEO("Notice %d : Audio FIFO full. Reset Audio FIFO\n", i);	
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFB8, 0x1F);
										hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[i].ctrl);
									}

									/* Update Audio */
									__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(i, 0);


									/* Added 2014/10/19 : Update data packets every gVideoUpdateCounterForDatapacket * sleep msecs*/
									if(gVideoEnableUpdateVideoDatapacket > 0){	
										if(datapacketUpdateTimer[i] >  gVideoUpdateCounterForDatapacket){
											datapacketUpdateTimer[i] = 0;
											for(y=0;y<PACKET_MAXNUM;y++){
												/* Even though this was opened, it will eventually be closed on Data packet ISR */
												if(y != GCP) __HDMI20_HAL_H15Ax_Packet_Store(&gHDMI20RxHandler[i].ctrl, dataPacketNum[y], 1);
											}
										}
										else{
											datapacketUpdateTimer[i]++;
										}
									}
									
									/* Added 2014/08/04 taejun.lee */
									__HDMI20_HAL_H15Ax_GetAudioInfo(i);

									/* Check Source HDMI2.0 capability */
									__HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(i, 0);



								}

								if( (gPhyResetOnAbnormalCnt >0) && (gHDMI20RxHandler[i].videoAbnormalCnt >=gAbnormalVideoResetTime)){
									HDMI20_VIDEO("Warning [%d] : Continous Abnormal Video for [%d] msec. Reset! Acuum[%d]\n",i, gHDMI20RxHandler[i].videoAbnormalCnt * gMainThreadMsleep, gHDMI20RxHandler[i].videoAbResetCnt);

									gHDMI20RxHandler[i].locking_time = 0;
									gHDMI20RxHandler[i].phy_rst_request = 1;
									gHDMI20RxHandler[i].videoAbnormalCnt = 0;
									gHDMI20RxHandler[i].videoAbResetCnt++;

									if(gLinkResetModeDetectionMode > 0){
										/* Add HDMI Link mode-detection reset */
										if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										msleep(100);
									}

									ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
									if(ret<0){
										HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
									}
									__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
								}
							}
							else if(__HDMI20_HAL_H15Ax_IsHPD_Up(i)&&( gHDMI20RxHandler[i].isPhyLocked == 0)&&(gHDMI20RxHandler[i].ctrl.isMute == 0)){
								/* Check  PhyLock when on good video */
								HDMI20_VIDEO("Warning [%d] : Lost Phylock on stable[%d] video. Lost Cnt[%d]\n",i, gHDMI20RxHandler[i].ctrl.stable_cnt, gHDMI20RxHandler[i].phyLostCnt);
								gHDMI20RxHandler[i].phyLostCnt++;

								__HDMI20_HAL_H15Ax_Rx_CheckSCDC(i);

								if(gHDMI20RxHandler[i].isManualSyncMode == 2){
									/* Clear Timing Info */
									__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 1);
									HDMI20_VIDEO("Notice [%d] : Recover from 3D manual Sync mode. Reset Phy\n", i);

									gHDMI20RxHandler[i].locking_time = 0;
									gHDMI20RxHandler[i].phy_rst_request = 1;

									gHDMI20RxHandler[i].isManualSyncMode = 1;
									gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
									HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);

									ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
									if(ret<0){
										HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
									}

									gHDMI20RxHandler[i].isResetAfterPhyLock = 1;

									__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
								}
								else if((gHDMI20RxHandler[i].ctrl.stable_cnt >= gVideoStableCntNormalRun)&&(gHDMI20RxHandler[i].phyInitiatedMode == 0)){
									HDMI20_VIDEO("Notice [%d] : Reset Link and PHY on phy lock ONLY for 3G signals\n", i);

									/* Clear Timing Info */
									__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[i].ctrl, 1);

									gHDMI20RxHandler[i].locking_time = 0;
									gHDMI20RxHandler[i].phy_rst_request = 1;

									if(gVideoDebugLevel != 2525){
										for(y=0;y<=gVideoDebugAmountOfHDCP14Reset;y++){
											hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
											hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);
										}
									}	
									//gHDMI20RxHandler[i].isManualSyncMode = 1;
									gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
									HDMI20_HAL_H15Ax_Rx_LinkVideoReset(i, 0);

									ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
									if(ret<0){
										HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
									}
									
									gHDMI20RxHandler[i].isResetAfterPhyLock = 1;
									
									__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
								}
								else if(gHDMI20RxHandler[i].ctrl.stable_cnt >= gVideoStableCntNormalRun){
									/* Important to clear ManualSYnc value */
									data = gHDMI20RxHandler[i].ctrl.bsp_read(0x400);
									data &= (~0x000000FF);
									gHDMI20RxHandler[i].ctrl.bsp_write(0x400, data);
									gHDMI20RxHandler[i].ctrl.bsp_write(0x404, 0x0);
									
									if(gLinkResetModeDetectionMode > 0){
										if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										msleep(100);
									}

								}
								else if((gHDMI20RxHandler[i].phyLostCnt >= gAbnormalPhyLostTime)&&(gReinitPhyOnLockLossByCnt >= 1)){
									HDMI20_VIDEO("NOTICE [%d] : Reset Phy due to lock signal loss. Cnt[%d]/Thres[%d]\n", i, gHDMI20RxHandler[i].phyLostCnt, gAbnormalPhyLostTime);

									if(gLinkResetModeDetectionMode > 0){
										if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										msleep(100);
									}

									//gReinitPhyOnLockLossByCnt = 0;

									gHDMI20RxHandler[i].locking_time = 0;
									gHDMI20RxHandler[i].phy_rst_request = 1;

									/* Inform HDCP2.2 to reset */
									if(gHDMI20RxHandler[i].isHDCP22){
										HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_RESET_VIDEO, i);
									}

									ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
									if(ret<0){
										HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
									}
									__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
								}

								/* Important to Reset Video stable count */
								gHDMI20RxHandler[i].ctrl.stable_cnt = 0;
							}
							else if(__HDMI20_HAL_H15Ax_IsHPD_Up(i)&&( gHDMI20RxHandler[i].isPhyLocked == 0)&&(gHDMI20RxHandler[i].ctrl.isMute != 0)){
								HDMI20_VIDEO("Warning [%d] : Lost Phylock, But AVMUTE. stableCnt[%d] Lost Cnt[%d]\n",i, gHDMI20RxHandler[i].ctrl.stable_cnt, gHDMI20RxHandler[i].phyLostCnt);
								gHDMI20RxHandler[i].phyLostCnt++;
								
								if((gHDMI20RxHandler[i].phyLostCnt >= gAbnormalPhyLostTime)&&(gReinitPhyOnLockLossByCnt >= 1)){
									HDMI20_VIDEO("NOTICE [%d] : Reset Phy due to lock signal loss(First time). Cnt[%d]/Thres[%d]\n", i, gHDMI20RxHandler[i].phyLostCnt, gAbnormalPhyLostTime);

									//gReinitPhyOnLockLossByCnt = 0;

									gHDMI20RxHandler[i].locking_time = 0;
									gHDMI20RxHandler[i].phy_rst_request = 1;

									/* Add HDMI Link mode-detection reset */
									if(gLinkResetModeDetectionMode > 0){
										if(gVideoDebugLevel == 851)HDMI20_VIDEO("Notice %d : HDMI Link Mode-detection reset.\n", i);
										gHDMI20RxHandler[i].ctrl.bsp_write(0xFF0, 0xE);
										msleep(100);
									}

									ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(i,  __HDMI20_HAL_H15Ax_IsSCDCvalueUD(i));
									if(ret<0){
										HDMI20_ERROR("Cannot Initiate Phy[%d]. Return error from Phy write[%d]\n", i, ret);
									}
									__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(i, 0);
								}
							}
							else{
								if(gHDMI20RxHandler[i].hpd_request == 1){
									HDMI20_VIDEO("Notice [%d] : Waiting for request response of HPD up[%d]\n", i, hpdWaitCnt[i]);
									hpdWaitCnt[i]++;
									if(hpdWaitCnt[i] >= gHPDRequestWaitThreshold){
										HDMI20_VIDEO("Notice [%d] : Call HPD request one more time.[%d]/[%d]\n", i, hpdWaitCnt[i], 	gHPDRequestWaitThreshold);
										HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_REQUEST_HPD ,i);
										hpdWaitCnt[i] = 0;
									}
								}
								else{
									HDMI20_VIDEO("Notice [%d] : HPD down even without connect_request\n", i);
									if(__HDMI20_HAL_H15Ax_IsSCDCvalueUD(i)){
										HDMI20_VIDEO("Notice [%d] Just HPD up due to SCDC is high\n", i);
										__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(i, 1, 1);
									}
									else{
										HDMI20_HAL_ConnectCable(i, ON);
									}
								}
							} // End of if(__HDMI20_HAL_H15Ax_IsHPD_Up

						}
					}

					if(gHDMI20RxHandler[i].connected5Vtime < gHDMI205VPopUpSigneHoldTime){
						gHDMI20RxHandler[i].connected5Vtime += gMainThreadMsleep;
					}

					/* HDCP1.4 stable check */
					if(( ((gHDMI20RxHandler[i].ctrl.bsp_read(0xe0) & 0x003F0000)>>16) >= 0x06)|| gHDMI20RxHandler[i].ctrl.bsp_read(0xFC) == 0x101) {
						if(gHDMI20RxHandler[i].stablehdcp14time <= gHDMI20stableHDCP14HoldTime){
							gHDMI20RxHandler[i].stablehdcp14time += gMainThreadMsleep;
						}
					}
					else{
						gHDMI20RxHandler[i].stablehdcp14time = 0;
					}

				} // End of if(gHDMI20RxHandler[i].is5Vconnected == 0)

			} // End of if(tx5Vconnected)
			else{
				/* 5V is not connected */
				gHDMI20RxHandler[i].connected5Vtime = 0;
			
				if(__HDMI20_HAL_H15Ax_IsHPD_Up(i) != 0){
					HDMI20_VIDEO("Notice [%d] : HPD was UP even though 5V is Low. Set HPD LOW\n", i);
					__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(i, 0, 1);
				}
			}
		} // End of for(i=0;i<1;i++)

		gMainThreadStamp = 1;
		msleep(gMainThreadMsleep);
	} // End of While(1)

	gMainThreadStamp = 0;
	gMainThreadAlive = 0;
}

static int __HDMI20_HAL_H15Ax_Rx_GetHDMIState(UINT8 port, LX_HDMI_STATUS_T *pHdmiStatus, int size)
{
	int ret = RET_ERROR;
	UINT32 video_format;
	LX_HDMI_STATUS_T userdata;

	do{
		if(pHdmiStatus == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_STATUS_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_STATUS_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_STATUS_T));

		/* Step 2. Copy data */
		if((UINT16)gHDMI20RxHandler[port].timing_info.dvi){
			userdata.bHdmiMode = 0;
		}
		else{
			userdata.bHdmiMode = 1;
		}

		video_format = gHDMI20RxHandler[port].ctrl.bsp_read(0x3A4);
		video_format = (video_format & 0x60); video_format = video_format>>5;
		video_format = video_format & 0x3;

		if(video_format == 0){
			video_format = (UINT32)((gHDMI20RxHandler[port].dataPacket[AVI][5]&0x60)>>5);
		}

		userdata.eHotPlug = gHDMI20RxHandler[port].isHPD;
		userdata.eColorDepth = 0;
		/* For COlor HW bug on link, inform DE only to do WA on 13.5 pixel repetition with 4:2:2 format */
		if(gHDMI20RxHandler[port].timing_info.pixel_repetition && ( gHDMI20RxHandler[port].timing_info.video_format == 1)){
			if(gVideoDebugLevel == 226){
				HDMI20_VIDEO("DBG %d : DE SW WA for 13.5Mhz 4:2:2\n", port);
			}
			userdata.pixelRepet = gHDMI20RxHandler[port].timing_info.pixel_repetition;
		}
		else{
			if(gVideoDebugLevel == 226){
				HDMI20_VIDEO("DBG %d : No SW WA for 13.5 4:2:2\n", port);
			}
			userdata.pixelRepet = 0;
		}

		userdata.csc =(UINT16)video_format;

		ret = copy_to_user((LX_HDMI_STATUS_T *)pHdmiStatus, &userdata, sizeof(LX_HDMI_STATUS_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "HDMI_STATE", port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

static void __HDMI20_HAL_H15Ax_Tx_Init_Dev_Handler(void)
{

	/* Initiate handler */
	/* Step 1. Init hdmi20 port device data */
	gHDMI20TxHandler.initiated = 1;
	gHDMI20TxHandler.isHPDInserted = 0;
	gHDMI20TxHandler.topmem.hwAddr = HDMI20_HAL_H15AX_TX0_TOP_ADDR;
	gHDMI20TxHandler.topmem.size = HDMI20_HAL_H15AX_TX0_TOP_SIZE;
	gHDMI20TxHandler.topmem.pSwAddr = NULL;
	gHDMI20TxHandler.linkmem.hwAddr = HDMI20_HAL_H15AX_TX0_CTRL_ADDR;
	gHDMI20TxHandler.linkmem.size = HDMI20_HAL_H15AX_TX0_CTRL_SIZE;
	gHDMI20TxHandler.linkmem.pSwAddr = NULL;
}

static int __HDMI20_HAL_H15Ax_Rx_Set_HDCP22Info(UINT8 port, int isInitiated)
{
	int ret = RET_ERROR;

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(isInitiated){
			gHDMI20RxHandler[port].gWasHDCP22Written = 1;
		}
		else{
			gHDMI20RxHandler[port].gWasHDCP22Written = 0;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

static void __HDMI20_HAL_H15Ax_Rx_Init_Dev_Handler(UINT8 isResume)
{
	/* Initiate handler */
	if(lx_chip_rev( ) >= LX_CHIP_REV(H15, A1)){
		gHDMI20RxHandler[0].port5vGPIOnum = HDMI20_RX_H15A1_5V_LV_PORT0;
		gHDMI20RxHandler[1].port5vGPIOnum = HDMI20_RX_H15A1_5V_LV_PORT1;
	}
	else {
		gHDMI20RxHandler[0].port5vGPIOnum = HDMI20_RX_H15A0_5V_LV_PORT0;
		gHDMI20RxHandler[1].port5vGPIOnum = HDMI20_RX_H15A0_5V_LV_PORT1;
	}

	gHDMI20RxHandler[0].gpio_hpd_real = HDMI20_RX_H15A1_HPD_GPIO_PORT0_REAL;
	gHDMI20RxHandler[0].gpio_hpd_temp = HDMI20_RX_H15A1_HPD_GPIO_PORT0_TEMP;
	
	gHDMI20RxHandler[1].gpio_hpd_real = HDMI20_RX_H15A1_HPD_GPIO_PORT1_REAL;
	gHDMI20RxHandler[1].gpio_hpd_temp = HDMI20_RX_H15A1_HPD_GPIO_PORT1_TEMP;


	/* Step 1. Init hdmi20 port[0] device data */
	if(isResume == 0){
		gHDMI20RxHandler[0].port = (HDMI20_HAL_NUMBER_OF_PHY_FOR_LINK - 1 );
		gHDMI20RxHandler[0].initiated = 1;
		gHDMI20RxHandler[0].irq = HDMI20_RX_IRQ_LINK_PORT0;
		gHDMI20RxHandler[0].top_irq = HDMI20_RX_IRQ_TOP_PORT0;
		gHDMI20RxHandler[0].linkmem.hwAddr = HDMI20_HAL_H15AX_RX0_CTRL_ADDR;
		gHDMI20RxHandler[0].linkmem.size = HDMI20_HAL_H15AX_RX0_CTRL_SIZE;
		gHDMI20RxHandler[0].linkmem.pSwAddr = NULL;
		gHDMI20RxHandler[0].topmem.hwAddr = HDMI20_HAL_H15AX_RX0_TOP_ADDR;
		gHDMI20RxHandler[0].topmem.size = HDMI20_HAL_H15AX_RX0_TOP_SIZE;
		gHDMI20RxHandler[0].topmem.pSwAddr = NULL;
		gHDMI20RxHandler[0].ipcmem.hwAddr = 0;
		gHDMI20RxHandler[0].ipcmem.size = 0;
		gHDMI20RxHandler[0].ipcmem.pSwAddr = NULL;
		gHDMI20RxHandler[0].locking_time  = 0;
		gHDMI20RxHandler[0].isHPD = 0;
		gHDMI20RxHandler[0].phyInitiatedMode = 0;
		gHDMI20RxHandler[0].scdc_change = 0;
		gHDMI20RxHandler[0].video_change = 0;
		gHDMI20RxHandler[0].real_tmds_clk = 0;
		gHDMI20RxHandler[0].real_pixel_clk = 0;
		gHDMI20RxHandler[0].dcm_change = 0;
		gHDMI20RxHandler[0].aksv_change = 0;
		gHDMI20RxHandler[0].audio_change = 0;
		gHDMI20RxHandler[0].tmds_change = 0;
		gHDMI20RxHandler[0].isScdcUD = 0;
		gHDMI20RxHandler[0].videoAbnormalCnt = 0;
		gHDMI20RxHandler[0].phyLostCnt = 0;
		gHDMI20RxHandler[0].isTimingInvalid = 0;
		gHDMI20RxHandler[0].isHDMI20Device = 0;
		/* Step 1-1. Init Link handler settings */
		gHDMI20RxHandler[0].ctrl.tmds_clock_old = 0;
		gHDMI20RxHandler[0].ctrl.status = 0;
		gHDMI20RxHandler[0].vtotalbuffer = 0;
		gHDMI20RxHandler[0].updateTimingTime = 0;
		gHDMI20RxHandler[0].isHDCP22 = 0;
		gHDMI20RxHandler[0].isHDCP22Ncap = 0;
		gHDMI20RxHandler[0].isHDCP14 = 0;
		gHDMI20RxHandler[0].isResetAfterPhyLock = 0;
		gHDMI20RxHandler[0].isLongPhyRecovery = 0;
		gAudioIsGalaxyTab10[0] = 0;
	}
	gHDMI20RxHandler[0].localmutetimer = 0;
	gHDMI20RxHandler[0].isAudioARCMode = 0;
	gHDMI20RxHandler[0].videoAbResetCnt = 0;
	gHDMI20RxHandler[0].isPhyLocked = 0;
	gHDMI20RxHandler[0].hpd_request = 0;
	gHDMI20RxHandler[0].connect_request = 0;
	gHDMI20RxHandler[0].phy_rst_request = 0;
	gHDMI20RxHandler[0].is5Vconnected  = 0;
	gHDMI20RxHandler[0].audioMuteEableCnt = 0;
	gHDMI20RxHandler[0].ctrl.isMute = 0;
	gHDMI20RxHandler[0].isAudioMute = 1;
	gHDMI20RxHandler[0].isPhyAlterMode = 0;
	gHDMI20RxHandler[0].currentDCMMode = 0;
	gHDMI20RxHandler[0].connected5Vtime = 0;
	gHDMI20RxHandler[0].ctrl.device_id = HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT0;
	gHDMI20RxHandler[0].ctrl.log_error = __HDMI20_HAL_H15Ax_Log_Error_Port0;
	gHDMI20RxHandler[0].ctrl.log_info = __HDMI20_HAL_H15Ax_Log_Info_Port0;
	gHDMI20RxHandler[0].ctrl.bsp_reset = __HDMI20_HAL_H15Ax_Device_Reset_Port0;
	gHDMI20RxHandler[0].ctrl.bsp_reset_hdmi = __HDMI20_HAL_H15Ax_Device_Reset_HDMI_Port0;
	gHDMI20RxHandler[0].ctrl.bsp_edid_update = __HDMI20_HAL_H15Ax_Update_EDID_Port0;
	gHDMI20RxHandler[0].ctrl.bsp_read = __HDMI20_HAL_H15Ax_BSP_Read_Port0;
	gHDMI20RxHandler[0].ctrl.bsp_write = __HDMI20_HAL_H15Ax_BSP_Write_Port0;
	gHDMI20RxHandler[0].ctrl.spin_init = __HDMI20_H15Ax_Spin_Init;
	gHDMI20RxHandler[0].ctrl.spin_destroy =  __HDMI20_Destory_Spinlock;
	gHDMI20RxHandler[0].ctrl.spin_lock_irq_save = __HDMI20_H15Ax_Spin_IRQ_Lock;
	gHDMI20RxHandler[0].ctrl.spin_unlock_irq_restore = __HDMI20_H15Ax_Spin_IRQ_UnLock;
	gHDMI20RxHandler[0].ctrl.cfg_clk = HDMI20_RX_CLOCK_CONFIG_KHZ;
	gHDMI20RxHandler[0].ctrl.md_clk = HDMI20_RX_CLOCK_MODE_DETECTION_KHZ;
	gHDMI20RxHandler[0].ctrl.stable_cnt = 0;

	/* Step 1-2. Init Phy handler settings */
	gHDMI20RxHandler[0].phy.version = HDMI20_RX_PHY_VERSION;
	gHDMI20RxHandler[0].phy.generation = HDMI20_RX_PHY_GENERATION;
	gHDMI20RxHandler[0].phy.status = 0;
	gHDMI20RxHandler[0].phy.cfg_clk = gHDMI20RxHandler[0].ctrl.md_clk;
	gHDMI20RxHandler[0].phy.log_error = __HDMI20_HAL_H15Ax_Log_Error_Port0;
	gHDMI20RxHandler[0].phy.log_info =__HDMI20_HAL_H15Ax_Log_Info_Port0;
	gHDMI20RxHandler[0].phy.bsp_reset = __HDMI20_HAL_H15Ax_Rx_Phy_Reset_Port0;
	gHDMI20RxHandler[0].phy.bsp_pddq = __HDMI20_HAL_H15Ax_Rx_Phy_Pddq_Port0;
	gHDMI20RxHandler[0].phy.bsp_svsmode = __HDMI20_HAL_H15Ax_Rx_Phy_Svsretmode_Port0;
	gHDMI20RxHandler[0].phy.bsp_zcal_reset = __HDMI20_HAL_H15Ax_Rx_Phy_ZCAL_Reset_Port0;
	gHDMI20RxHandler[0].phy.bsp_zcal_done = __HDMI20_HAL_H15Ax_Rx_Phy_Zcal_Done_Port0;
	//gHDMI20RxHandler[0].phy.bsp_config =
	gHDMI20RxHandler[0].phy.bsp_read = __HDMI20_HAL_H15Ax_Rx_Phy_Read_Port0;
	gHDMI20RxHandler[0].phy.bsp_write = __HDMI20_HAL_H15Ax_Rx_Phy_Write_Port0;
	gHDMI20RxHandler[0].check19201080Dcm = 0;
	gHDMI20RxHandler[0].is19201080Dcm = 0;

	/* Step 2. Init hdmi20 port1 device data */
	if(isResume == 0){
		gHDMI20RxHandler[1].port = (HDMI20_HAL_NUMBER_OF_PHY_FOR_LINK - 1 );
		gHDMI20RxHandler[1].initiated = 1;
		gHDMI20RxHandler[1].irq = HDMI20_RX_IRQ_LINK_PORT1;
		gHDMI20RxHandler[1].top_irq = HDMI20_RX_IRQ_TOP_PORT1;
		gHDMI20RxHandler[1].linkmem.hwAddr = HDMI20_HAL_H15AX_RX1_CTRL_ADDR;
		gHDMI20RxHandler[1].linkmem.size = HDMI20_HAL_H15AX_RX1_CTRL_SIZE;
		gHDMI20RxHandler[1].linkmem.pSwAddr = NULL;
		gHDMI20RxHandler[1].topmem.hwAddr = HDMI20_HAL_H15AX_RX1_TOP_ADDR;
		gHDMI20RxHandler[1].topmem.size = HDMI20_HAL_H15AX_RX1_TOP_SIZE;
		gHDMI20RxHandler[1].topmem.pSwAddr = NULL;
		gHDMI20RxHandler[1].ipcmem.hwAddr = 0;
		gHDMI20RxHandler[1].ipcmem.size = 0;
		gHDMI20RxHandler[1].ipcmem.pSwAddr = NULL;
		gHDMI20RxHandler[1].locking_time  = 0;
		gHDMI20RxHandler[1].isHPD = 0;
		gHDMI20RxHandler[1].real_tmds_clk = 0;
		gHDMI20RxHandler[1].real_pixel_clk = 0;
		gHDMI20RxHandler[1].phyInitiatedMode = 0;
		gHDMI20RxHandler[1].scdc_change = 0;
		gHDMI20RxHandler[1].video_change = 0;
		gHDMI20RxHandler[1].dcm_change = 0;
		gHDMI20RxHandler[1].aksv_change = 0;
		gHDMI20RxHandler[1].audio_change = 0;
		gHDMI20RxHandler[1].tmds_change = 0;
		gHDMI20RxHandler[1].isScdcUD = 0;
		gHDMI20RxHandler[1].videoAbnormalCnt = 0;
		gHDMI20RxHandler[1].phyLostCnt = 0;
		gHDMI20RxHandler[1].isTimingInvalid = 0;
		gHDMI20RxHandler[1].isHDMI20Device = 0;

		/* Step 2-1. Init Link handler settings */
		gHDMI20RxHandler[1].ctrl.tmds_clock_old = 0;
		gHDMI20RxHandler[1].ctrl.status = 0;
		gHDMI20RxHandler[1].vtotalbuffer = 0;
		gHDMI20RxHandler[1].updateTimingTime = 0;
		gHDMI20RxHandler[1].isHDCP22 = 0;
		gHDMI20RxHandler[1].isHDCP22Ncap = 0;
		gHDMI20RxHandler[1].isHDCP14 = 0;
		gHDMI20RxHandler[1].isResetAfterPhyLock = 0;
		gHDMI20RxHandler[1].isLongPhyRecovery = 0;
		gAudioIsGalaxyTab10[1] = 0;
	}
	gHDMI20RxHandler[1].localmutetimer = 0;
	gHDMI20RxHandler[1].isAudioARCMode = 0;
	gHDMI20RxHandler[1].videoAbResetCnt = 0;
	gHDMI20RxHandler[1].isPhyLocked = 0;
	gHDMI20RxHandler[1].hpd_request = 0;
	gHDMI20RxHandler[1].connect_request = 0;
	gHDMI20RxHandler[1].phy_rst_request = 0;
	gHDMI20RxHandler[1].is5Vconnected  = 0;
	gHDMI20RxHandler[1].audioMuteEableCnt = 0;
	gHDMI20RxHandler[1].ctrl.isMute = 0;
	gHDMI20RxHandler[1].isAudioMute = 1;
	gHDMI20RxHandler[1].currentDCMMode = 0;
	gHDMI20RxHandler[1].isPhyAlterMode = 0;
	gHDMI20RxHandler[1].connected5Vtime = 0;
	gHDMI20RxHandler[1].ctrl.device_id = HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1;
	gHDMI20RxHandler[1].ctrl.log_error = __HDMI20_HAL_H15Ax_Log_Error_Port1;
	gHDMI20RxHandler[1].ctrl.log_info = __HDMI20_HAL_H15Ax_Log_Info_Port1;
	gHDMI20RxHandler[1].ctrl.bsp_reset = __HDMI20_HAL_H15Ax_Device_Reset_Port1;
	gHDMI20RxHandler[1].ctrl.bsp_reset_hdmi = __HDMI20_HAL_H15Ax_Device_Reset_HDMI_Port1;
	gHDMI20RxHandler[1].ctrl.bsp_edid_update = __HDMI20_HAL_H15Ax_Update_EDID_Port1;
	gHDMI20RxHandler[1].ctrl.bsp_read = __HDMI20_HAL_H15Ax_BSP_Read_Port1;
	gHDMI20RxHandler[1].ctrl.bsp_write = __HDMI20_HAL_H15Ax_BSP_Write_Port1;
	gHDMI20RxHandler[1].ctrl.spin_init = __HDMI20_H15Ax_Spin_Init;
	gHDMI20RxHandler[1].ctrl.spin_destroy = __HDMI20_Destory_Spinlock;
	gHDMI20RxHandler[1].ctrl.spin_lock_irq_save =  __HDMI20_H15Ax_Spin_IRQ_Lock;
	gHDMI20RxHandler[1].ctrl.spin_unlock_irq_restore = __HDMI20_H15Ax_Spin_IRQ_UnLock;
	gHDMI20RxHandler[1].ctrl.cfg_clk = HDMI20_RX_CLOCK_CONFIG_KHZ;
	gHDMI20RxHandler[1].ctrl.md_clk = HDMI20_RX_CLOCK_MODE_DETECTION_KHZ;
	gHDMI20RxHandler[1].ctrl.stable_cnt = 0;
	gHDMI20RxHandler[1].check19201080Dcm = 0;
	gHDMI20RxHandler[1].is19201080Dcm = 0;

	/* Step 2-2. Init Phy handler settings */
	gHDMI20RxHandler[1].phy.version = HDMI20_RX_PHY_VERSION;
	gHDMI20RxHandler[1].phy.generation = HDMI20_RX_PHY_GENERATION;
	gHDMI20RxHandler[1].phy.status = 0;
	gHDMI20RxHandler[1].phy.cfg_clk = gHDMI20RxHandler[1].ctrl.md_clk;
	gHDMI20RxHandler[1].phy.log_error = __HDMI20_HAL_H15Ax_Log_Error_Port1;
	gHDMI20RxHandler[1].phy.log_info =__HDMI20_HAL_H15Ax_Log_Info_Port1;
	gHDMI20RxHandler[1].phy.bsp_reset = __HDMI20_HAL_H15Ax_Rx_Phy_Reset_Port1;
	gHDMI20RxHandler[1].phy.bsp_pddq = __HDMI20_HAL_H15Ax_Rx_Phy_Pddq_Port1;
	gHDMI20RxHandler[1].phy.bsp_svsmode = __HDMI20_HAL_H15Ax_Rx_Phy_Svsretmode_Port1;
	gHDMI20RxHandler[1].phy.bsp_zcal_reset = __HDMI20_HAL_H15Ax_Rx_Phy_ZCAL_Reset_Port1;
	gHDMI20RxHandler[1].phy.bsp_zcal_done = __HDMI20_HAL_H15Ax_Rx_Phy_Zcal_Done_Port1;
	//gHDMI20RxHandler[1].phy.bsp_config =
	gHDMI20RxHandler[1].phy.bsp_read = __HDMI20_HAL_H15Ax_Rx_Phy_Read_Port1;
	gHDMI20RxHandler[1].phy.bsp_write = __HDMI20_HAL_H15Ax_Rx_Phy_Write_Port1;

	HDMI20_AUDIO("Init Dev : port(%0) ARCmode(%d)", gHDMI20RxHandler[0].isAudioARCMode);

}


static irqreturn_t __HDMI20_HAL_RX_Link_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs)
{
	int ret = 0;

	if ( dev_id == 0){
		HDMI20_INTR("ISR : RX[%d] NULL parmeter\n", (int)dev_id );
		return -1;
	}

	ret = hdmi_rx_ctrl_irq_handler(&gHDMI20RxHandler[(int)*((int *)((void *)dev_id))].ctrl);
	if( ret < 0) {
		if (ret != -EPERM)
		{
			HDMI20_INTR("ISR : RX[%d] Link driver ISR error\n", (int)*((int *)((void *)dev_id)));
		}
	}

	return IRQ_HANDLED;
}


static int __HDMI20_Destory_Spinlock(void *handler)
{
	HDMI20_DBG0("Function called [0x%08x]\n", (UINT32)handler);

	return 0;
}


static int __HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(UINT8 port, int onOff, int useTimer, UINT32 caller_id)
{
	int ret = RET_ERROR;
	UINT32 data;
	UINT32 currentJiffiesMsec = 0;


	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(useTimer) currentJiffiesMsec = jiffies_to_msecs(jiffies);

		if(onOff){
			__HDMI20_HAL_H15Ax_SetVideoColorDomain(port);

			HDMI20_VIDEO("Notice [%d] : Set Local Video Mute => [%s]. Mode => [%s]. Caller =>[%d]\n", port, onOff?"On":"Off", useTimer?"Timer":"Force", caller_id);

			if(useTimer) { 
				gHDMI20RxHandler[port].localmutetimer = currentJiffiesMsec;
			}

			data = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
			data |= (0x00010000);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xb4, data);

		}
		else{
			if(useTimer){
				if(currentJiffiesMsec - gHDMI20RxHandler[port].localmutetimer >= gHDMI20LocalMuteHoldTime){
					HDMI20_VIDEO("Notice [%d] : LocalMute release by hold time pass. Caller ID[%d]\n", port, caller_id);
					if(gHDMI20LocalMuteHoldTime == gHDMI20LocalMuteHoldInitTime){
						gHDMI20LocalMuteHoldTime = gHDMI20LocalMuteHoldOriginTime;
						HDMI20_VIDEO("Notice [%d] : Video stable for the first connection. Change Local Mute hold Thres\n", port);
						gHDMI20RxHandler[port].localmutetimer  = 0;
					}
					data = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
					data &= (~0x00010000);
					gHDMI20RxHandler[port].ctrl.bsp_write(0xb4, data);
				}
				else{
					if(gVideoDebugLevel == 208) HDMI20_VIDEO("NOTICE [%d] : LocalMute Hold [0x%x]-[0x%x] < [0x%x]. Caller ID[%d]\n",port,currentJiffiesMsec, gHDMI20RxHandler[port].localmutetimer, gHDMI20LocalMuteHoldTime, caller_id);
				}
			}
			else{
				HDMI20_VIDEO("Notice [%d] : LocalMute release by force. Caller_Id[%d]\n", port, caller_id);
				data = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
				data &= (~0x00010000);
				gHDMI20RxHandler[port].ctrl.bsp_write(0xb4, data);
			}
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(UINT8 port)
{
	int ret = 0;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return 0;
	}

	if( gHDMI20RxHandler[port].ctrl.bsp_read(0xb4) & 0x00010000){
		ret = 1;
	}
	else{
		ret = 0;
	}

	return ret;
}

static int __HDMI20_HAL_H15Ax_Rx_SetLocalVideoMuteColor(UINT8 port, UINT32 isYCbCr)
{
	int ret = RET_ERROR;
	UINT32 data;

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(isYCbCr){
			data  = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
			data &= (0x00010000); data |= (0x00008000);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xB0, 0x8000);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xB4, data);
		}
		else{
			data  = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
			data &= (0x00010000);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xB0, 0x0);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xB4, data);
		}

		ret = RET_OK;
	} while(0);

	return ret;
}



static int __HDMI20_HAL_H15Ax_Rx_Initialize(LX_HDMI20_INIT_T *pData, int size)
{
	int i;
	int ret = RET_ERROR;
	UINT32 gpio_value = 0;
	UINT32 tempValue = 0;
	LX_HDMI20_INIT_T	userData;
	LX_GPIO_VALUE_T gpioValue = LX_GPIO_VALUE_INVALID;

	HDMI20_INFO("Function called \n");

	do{
		/* Step 0. Init DBG */
		_gHDMI20RegSettings.pRegSettings = _gHDMI20RegSettings_H15Ax;
		_gHDMI20RegSettings.number_of_modes = sizeof(_gHDMI20RegSettings_H15Ax)/sizeof(LX_HDMI20_REG_SETTING_T);

		memset((void *)&phyDBGHist, 0, sizeof(LX_HDMI20_DBG_PHY_HISTORY_T));
		memset((void *)&phyAddHist[0], 0, sizeof(LX_HDMI20_DBG_PHY_HISTORY_T));
		memset((void *)&phyAddHist[1], 0, sizeof(LX_HDMI20_DBG_PHY_HISTORY_T));


		/* Step 1. Init Rx Device handler */
		HDMI20_INFO("Initiating HDMI20 device handler \n");
		__HDMI20_HAL_H15Ax_Rx_Init_Dev_Handler(0);

		/* Step 1-1. Check given args to proceed */
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI20_INIT_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_TIMING_INFO_T), size);
			break;
		}

		/* Step 1-2. Get User data to kernel*/
		if( copy_from_user(&userData, (void __user *)pData, sizeof(LX_HDMI20_INIT_T))){
			HDMI20_ERROR("Cannot copy HDMI20 Initd ata structure from user\n");
			break;
		}

		/* Step 2. Ioremap HW to memory */
		HDMI20_INFO("Remapping HDMI20 device HW to memory \n");
		/* Step 2-1. Remap Rx0 */
		for(i = 0; i<HDMI20_HAL_NUMBER_OF_LINK; i++)
		{
			if(gHDMI20RxHandler[i].linkmem.pSwAddr != NULL) {
				gHDMI20RxHandler[i].linkmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].linkmem.hwAddr, gHDMI20RxHandler[i].linkmem.size);
			}
			else {
				iounmap((void *)gHDMI20RxHandler[i].linkmem.pSwAddr);
				gHDMI20RxHandler[i].linkmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].linkmem.hwAddr, gHDMI20RxHandler[i].linkmem.size);
			}

			if(gHDMI20RxHandler[i].topmem.pSwAddr != NULL) {
				gHDMI20RxHandler[i].topmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].topmem.hwAddr, gHDMI20RxHandler[i].topmem.size);
			}
			else {
				iounmap((void *)gHDMI20RxHandler[i].topmem.pSwAddr);
				gHDMI20RxHandler[i].topmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].topmem.hwAddr, gHDMI20RxHandler[i].topmem.size);
			}
		
			gHDMI20RxHandler[i].ipcmem.hwAddr = userData.ipc_address[i];
			gHDMI20RxHandler[i].ipcmem.size = sizeof(UINT32);

			if(gHDMI20RxHandler[i].ipcmem.pSwAddr != NULL) {
				gHDMI20RxHandler[i].ipcmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].ipcmem.hwAddr, gHDMI20RxHandler[i].ipcmem.size);
			}
			else {
				iounmap((void *)gHDMI20RxHandler[i].ipcmem.pSwAddr);
				gHDMI20RxHandler[i].ipcmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20RxHandler[i].ipcmem.hwAddr, gHDMI20RxHandler[i].ipcmem.size);
			}

			HDMI20_INFO("MAPPED RX[%d] : CTRL[0x%08x] / TOP[0x%08x]  \n", i, (unsigned int)gHDMI20RxHandler[i].linkmem.pSwAddr,  (unsigned int)gHDMI20RxHandler[i].topmem.pSwAddr);

		}



		if(lx_chip_rev () >= LX_CHIP_REV(H15,A0)){
			pHDMI_MUX_CTRL_B = (volatile UINT32 *)ioremap(0xC8000140, sizeof(UINT32));
			pHDMI_MUX_CTRL_C = (volatile UINT32 *)ioremap(0xC8000180, sizeof(UINT32));
			pHDMI_MUX_CTRL_D = (volatile UINT32 *)ioremap(0xC80001C0, sizeof(UINT32));

			HDMI20_INFO("SW W/A Mapped Addr : [0xC8000140]=>[0x%08x],[0xC8000180]=>[0x%08x],[0xC80001C0]=>[0x%08x]\n", pHDMI_MUX_CTRL_B, pHDMI_MUX_CTRL_C, pHDMI_MUX_CTRL_D);

			/* SW W/A for CVI bug.. Only work for Chips Under H15A1 */
			if(lx_chip_rev() >= LX_CHIP_REV(H15,B0)){
				//HDMI20_VIDEO("SW WA for CVI Bug only for H15Ax\n");
				if(pHDMI_MUX_CTRL_B != 0x0){
					*pHDMI_MUX_CTRL_B &= 0xFFFFFF00;
					*pHDMI_MUX_CTRL_B |= 0x00000011;
				}
				if(pHDMI_MUX_CTRL_C != 0x0){
					*pHDMI_MUX_CTRL_C &= 0xFFFFFF00;
					*pHDMI_MUX_CTRL_C |= 0x00000022;
				}
				if(pHDMI_MUX_CTRL_D != 0x0){
					*pHDMI_MUX_CTRL_D &= 0xFFFFFF00;
					*pHDMI_MUX_CTRL_D |= 0x00000033;
				}
			}

		}


		/* Step 2-2. Set GPIO Pull Up */
		if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
			HDMI20_REG_RD(0xC898E494, gpio_value);
			gpio_value |= 0x30000000;
			HDMI20_REG_WR(0xC898E494, gpio_value);
		}
		else{
			HDMI20_REG_RD(0xC898E488, gpio_value);
			gpio_value &= 0xFFFFFFFC;
			HDMI20_REG_WR(0xC898E488, gpio_value);
		}

		/* Step 3. Open Rx */
		for(i = 0; i<HDMI20_HAL_NUMBER_OF_LINK; i++)
		{
			/* Step : Set GPIO Setting for HPD */
			if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
				/* HOT PLUG GPIO : Set Normal/GPIO Mode : Port 120, 121 */
				GPIO_DevSetPinMux(gHDMI20RxHandler[i].gpio_hpd_real, 1);
				GPIO_DevSetMode(gHDMI20RxHandler[i].gpio_hpd_real, LX_GPIO_MODE_OUTPUT);
				GPIO_DevSetPinMux(gHDMI20RxHandler[i].gpio_hpd_temp, 1);
				GPIO_DevSetMode(gHDMI20RxHandler[i].gpio_hpd_temp, LX_GPIO_MODE_OUTPUT);
			}

			/* Step 3-1. Open Rx port [i] */
			HDMI20_INFO("Opening Synopsys Rx[%d] device\n", i);
			/* Link */
			ret = hdmi_rx_ctrl_open(&gHDMI20RxHandler[i].ctrl);
			if( ret <0 ) { HDMI20_ERROR("Error opening HDMI20 port[%d] ctrl[%d]\n", i, ret); }
			/* HDCP */
			ret = hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[i].ctrl, &gHDMI20RxHandler[i].hdcp);

			if( ret <0 ) { HDMI20_ERROR("Error opening HDMI20 port[%d] hdcp 1.4[%d]\n", i, ret); }
			/* Config Link Settings */
			ret = hdmi_rx_ctrl_config(&gHDMI20RxHandler[i].ctrl, gHDMI20RxHandler[i].port);
			if( ret <0 ) { HDMI20_ERROR("Error on HDMI20 port[%d] initial configuration[%d]\n",i, ret); }
			/* HDMI MODE Recover */
			gHDMI20RxHandler[i].ctrl.bsp_write(0x080, gHDMIModeRecover);
			gHDMI20RxHandler[i].ctrl.bsp_write(0x16C, gHDMIVerticalTiming);
			/* DBG : Fixed HDMI mode */
			if(gForcedHDMIMode >0){ __HDMI20_HAL_H15Ax_Rx_SetForcedHDMIMode(i); }

			gHDMI20RxHandler[i].ctrl.bsp_write(0xff0, 0x7F);
			ret = hdmi_rx_phy_open(&gHDMI20RxHandler[i].phy);
			if( ret <0 ) { HDMI20_ERROR("Error on HDMI20 port[%d] initial phy opening[%d]\n",i, ret); }
			/* Register Link IRQ */
			ret = request_irq(gHDMI20RxHandler[i].irq, (irq_handler_t)__HDMI20_HAL_RX_Link_IRQ_Handler, 0, "HDMI20", (void *)&gHDMI20RxHandler[i].ctrl.device_id);
			if( ret <0 ) { HDMI20_ERROR("Failed to request HDMI20 port0 interrupt[%d]\n", ret); }

			/* List up to subscribe */
			/* TMDS refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_clock_change, __HDMI20_HAL_H15Ax_Refresh_TMDS);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] TMDS Refresher\n", i);}
			/* VIDEO refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_video_status, __HDMI20_HAL_H15Ax_Refresh_Video);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] VIDEO Refresher\n", i);}
			/* Audio Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_audio_status, __HDMI20_HAL_H15Ax_Refresh_Audio);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AUDIO Refresher\n", i);}
			/* Data Packet Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_packet_reception, __HDMI20_HAL_H15Ax_Refresh_DataPacket);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Data Packet Refresher\n", i);}
			/* AKSV Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_aksv_reception, __HDMI20_HAL_H15Ax_Refresh_AKSV);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AKSV Refresher\n",  i);}
			/* PLL Lock Change Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_pll_lock_change, __HDMI20_HAL_H15Ax_Refresh_PLLLock);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] PLL Lock Change Refresher\n",  i);}
			/* Phy i2c done  Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_phy_i2c_done, __HDMI20_HAL_H15Ax_Refresh_PhyI2CDone);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Phy i2c done Refresher\n",  i);}
			/* Phy i2c nack Refresher to IRQ */
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_phy_i2c_nack, __HDMI20_HAL_H15Ax_Refresh_PhyI2CNack);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Phy i2c nack Refresher\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_dcm_change, __HDMI20_HAL_H15Ax_Refresh_DCM);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] DCM Refresher\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_avmute_change, __HDMI20_HAL_H15Ax_Refresh_AVMute);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AVMute Refresher\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_scdc, __HDMI20_HAL_H15Ax_Refresh_SCDC);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] SCDC Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_cap, __HDMI20_HAL_H15Ax_Refresh_HDCP22_CAP);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 CAP Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_ncap, __HDMI20_HAL_H15Ax_Refresh_HDCP22_NCAP);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 NCAP Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_lost, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_LOST);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_LOST Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_ists, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_ISTS);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_ISTS Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_fail, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_FAIL);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_FAIL Refresh\n",  i);}
			ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[i].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_change, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_CHANGE);
			if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_CHANGE Refresh\n",  i);}

			/* Initiate Audio data */
			memset((void *)&gHDMI20RxHandler[i].audio, 0, sizeof(struct hdmi_rx_ctrl_audio));
			memset((void *)&gHDMI20RxHandler[i].timing_info, 0, sizeof(struct hdmi_rx_ctrl_video));
			memset((void *)&gHDMI20RxHandler[i].sync_info, 0, sizeof(HDMI20_HAL_DBG_SYNC_INFO_T));

			/* Auto detect VS, HS polarity : Enable Auto sync detect */
			gHDMI20RxHandler[i].ctrl.bsp_write(0x90, 0x14);

			/* Test mode : Bypass(HDCP2.2) to  HDCP1.4 core */
			//HDMI20_HAL_H15Ax_Rx_OverRideHDCP(i, 0, 1);

			/* Test mode : Bypass HDMI1.4 */
			if(gHDMI20RxHandler[i].gWasHDCP22Written >= 1){
				if(gHDMI20RxHandler[i].gWasHDCP14Written >= 1){
					gHDMI20RxHandler[i].ctrl.bsp_write(0xc0, 0x01000001);
				}
				else{
					gHDMI20RxHandler[i].ctrl.bsp_write(0xc0, 0x01000000);
				}
				gHDMI20RxHandler[i].ctrl.bsp_write(0x81c, 0x00410000);
			}
			else if(gHDMI20RxHandler[i].gWasHDCP14Written >= 1){
				gHDMI20RxHandler[i].ctrl.bsp_write(0xc0, 0x01000001);
				gHDMI20RxHandler[i].ctrl.bsp_write(0x81c, 0x00410002);

			}
			else{
				// Test mode
				gHDMI20RxHandler[i].ctrl.bsp_write(0xc0, 0x01000000);
				gHDMI20RxHandler[i].ctrl.bsp_write(0x81c, 0x2);
			}

			/* HDCP1.4 Set DDC to 100Khz */
			gHDMI20RxHandler[i].ctrl.bsp_write(0xc4, gHDCPBcapsCtrlValue);

			/* Initiate Data Packet Buffer */
			HDMI20_H15A0_ResetDataPacketBuffer(hdmi_rx_ctrl_packet_cnt, i);


			/* Init SCDC */
			//hdmi_rx_scdc_initConfig(&gHDMI20RxHandler[i].ctrl);
			//hdmi_rx_scdc_scrambling(&gHDMI20RxHandler[i].ctrl, 1);
			gHDMI20RxHandler[i].ctrl.bsp_write(0x800, 0x1F11);
			gHDMI20RxHandler[i].ctrl.bsp_write(0x860, 0xABCDEF01);
			gHDMI20RxHandler[i].ctrl.bsp_write(0x808, 0x1);
			gHDMI20RxHandler[i].ctrl.bsp_write(0x80C, 0x3FF0C15C);
		}

		/* Port 0 : Register 5V IRQ */
		ret = request_irq( gHDMI20RxHandler[0].top_irq, (irq_handler_t)__HDMI20_HAL_RX0_5V_IRQ_Handler, 0, "HDMI20", (void *)&gHDMI20RxHandler[0]);
		if( ret <0 ) { HDMI20_ERROR("Failed to request 5V port0 interrupt[%d]\n", ret); }

		/* Port 1 : Register 5V IRQ */
		ret = request_irq( gHDMI20RxHandler[1].top_irq, (irq_handler_t)__HDMI20_HAL_RX1_5V_IRQ_Handler, 0, "HDMI20", (void *)&gHDMI20RxHandler[1]);
		if( ret <0 ) { HDMI20_ERROR("Failed to request 5V port1 interrupt[%d]\n", ret); }

		/* Change PHY mode from fixed->Auto equalizer from H15Bx */
		if(lx_chip_rev( ) >= LX_CHIP_REV(H15, B0)){
			gVideoPhyFHDDBGMode = 2;
			gVideoPhyFHDValue = 0x6;
			gPhyLinkResetMode = 1;

			gVideoPhyUHDValue = 0x200;
		}
		else{
			gVideoPhyFHDDBGMode = 2;
			gVideoPhyFHDValue = 0x6;
			gPhyLinkResetMode = 1;

			gVideoPhyUHDValue = 0x1A0;
		}

		/* Enable 5V IRQ */
		HDMI20_HAL_H15Ax_RX0_Enable_5V_IRQ(1, 1);
		HDMI20_HAL_H15Ax_RX0_Enable_5V_IRQ(0, 1);
		HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(1, 1);
		HDMI20_HAL_H15Ax_RX1_Enable_5V_IRQ(0, 1);

		/* Check 5V status for both port due to unknown state */
		HDMI20_HAL_H15Ax_RX0_Reset_TOP_SyncInfo();
		HDMI20_HAL_H15Ax_RX1_Reset_TOP_SyncInfo();


		/* Start HDMI Main thread(Checking video information on video transtion state) */
		gMainThreadAlive = 1;
		gMainthread = kthread_create( (void *)__HDMI20_HAL_H15Ax_MainThread, 0, "hdmi20_rx_video_stable_checker");
		if(gMainthread){
			HDMI20_INFO("Initialing VIDEO Stable checker thread\n");
			wake_up_process(gMainthread);
		}else HDMI20_ERROR("Cannot initiate Video stablizor thread\n");

		/* Initiate PHY port 0*/
		ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(0, 0);
		if(ret<0){
			HDMI20_ERROR("Cannot Initiate Phy[0]. No lock signal from PHY\n");
		}

		/* Initiate PHY port 1*/
		ret = HDMI20_HAL_H15Ax_Rx_Initiate_Phy(1, 0);
		if(ret<0){
			HDMI20_ERROR("Cannot Initiate Phy[1]. No lock signal from PHY\n");
		}

		/* Reset Link Port 0*/
		HDMI20_HAL_H15Ax_Rx_LinkVideoReset(0, 1);

		/* Reset Link Port 1*/
		HDMI20_HAL_H15Ax_Rx_LinkVideoReset(1, 1);

		if(gHPDInverseMode){
			hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[0].ctrl, 1);
			hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[1].ctrl, 1);
		
			gpioValue = LX_GPIO_VALUE_HIGH;
			ret  = _GPIO_SetValue(gHDMI20RxHandler[0].gpio_hpd_real , gpioValue);
			ret |= _GPIO_SetValue(gHDMI20RxHandler[0].gpio_hpd_temp , gpioValue);
			ret |= _GPIO_SetValue(gHDMI20RxHandler[1].gpio_hpd_real , gpioValue);
			ret |= _GPIO_SetValue(gHDMI20RxHandler[1].gpio_hpd_temp , gpioValue);
			
			if(ret < 0){
				HDMI20_ERROR("ERROR : Failed to control GPIO HPD\n");
			}

		}
		else{
			if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[0].ctrl, 0);
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[1].ctrl, 0);

				gpioValue = LX_GPIO_VALUE_LOW;
				ret  = _GPIO_SetValue(gHDMI20RxHandler[0].gpio_hpd_real , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[0].gpio_hpd_temp , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[1].gpio_hpd_real , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[1].gpio_hpd_temp , gpioValue);

				if(ret < 0){
					HDMI20_ERROR("ERROR : Failed to control GPIO HPD\n");
				}

			}
			else{
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[0].ctrl, 0);
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[1].ctrl, 1);
			}
		}

		gHDMI20RxHandler[0].isHPD = 0;
		gHDMI20RxHandler[1].isHPD = 0;

		/* HDCP2.2 Write PUK*/
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_1, HDMI20_H15AX_PUK_1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_2, HDMI20_H15AX_PUK_2);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_3, HDMI20_H15AX_PUK_3);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_PUK_ADDR_4, HDMI20_H15AX_PUK_4);

		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_1, HDMI20_H15AX_PUK_1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_2, HDMI20_H15AX_PUK_2);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_3, HDMI20_H15AX_PUK_3);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_PUK_ADDR_4, HDMI20_H15AX_PUK_4);

		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_1, HDMI20_RX_H15AX_DUK_1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_2, HDMI20_RX_H15AX_DUK_2);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_3, HDMI20_RX_H15AX_DUK_3);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_H15AX_DUK_ADDR_4, HDMI20_RX_H15AX_DUK_4);

		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_1, HDMI20_RX_H15AX_DUK_1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_2, HDMI20_RX_H15AX_DUK_2);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_3, HDMI20_RX_H15AX_DUK_3);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_H15AX_DUK_ADDR_4, HDMI20_RX_H15AX_DUK_4);

		/* SW Workaround for ACR pll mode */
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(HDMI20_RX_ACR_PLL_MODE, 0x10001 );
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(HDMI20_RX_ACR_PLL_MODE, 0x10001 );

		/* Added 2014/07/02 taejun.lee */
		if(lx_chip_rev( ) >= LX_CHIP_REV(H15, B0)){
			;
		}else{
			if (NULL == pgHDMI20AudioData)
			{
				pgHDMI20AudioData = (HDMI20_HAL_AUDIO_DATA *) ioremap(H15_AUD_BASE + 0x06d8,0x10);
				if (NULL == pgHDMI20AudioData)
				{
					HDMI20_ERROR("Audio Reg address ERR!");
					return RET_ERROR;
				}
			}
			HDMI20_INFO("Audio Reg address [0x%08X]\n", (UINT32) H15_AUD_BASE + 0x06d8);
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* This function is call on IRQ only when Deep Color Mode information has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_AVMute(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		if(gHDMI20RxHandler[ctx->device_id].ctrl.isMute == 1){
			if(gVideoDebugLevel == 628) HDMI20_VIDEO("--NOTICE [%d] : Set AV Mute--\n", ctx->device_id);
			gHDMI20RxHandler[ctx->device_id].avmute_on_time = jiffies_to_msecs(jiffies);
		}
		else{
			if(gVideoDebugLevel == 628)	HDMI20_VIDEO("--NOTICE [%d] : Clear AV Mute--\n", ctx->device_id);
			//gHDMI20RxHandler[ctx->device_id].avmute_on_time = jiffies_to_msecs(jiffies);
			if(gVideoDebugLevel != 734) gHDMI20RxHandler[ctx->device_id].ctrl.isMute = 0;
		}

	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_CAP(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);
	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_NCAP(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		gHDMI20RxHandler[ctx->device_id].isHDCP22Ncap = 1;
		gHDMI20RxHandler[ctx->device_id].isHDCP14 = 1;

		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);
	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_LOST(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);

	} while(0);

	return ret;
}
static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_ISTS(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		gHDMI20RxHandler[ctx->device_id].isHDCP22 = true;
		
		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);
	} while(0);

	return ret;

}

static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_FAIL(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);
	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_CHANGE(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		HDMI20_VIDEO("[%s:%d] called \n", __F__, __L__);
	} while(0);

	return ret;
}



static int __HDMI20_HAL_H15Ax_Refresh_SCDC(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_OK;
	UINT32 scdc_value;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		scdc_value = gHDMI20RxHandler[ctx->device_id].ctrl.bsp_read(0x820);
		scdc_value &= 0x00020000;

		HDMI20_VIDEO("SCDC ISR [%d] : SCDC[0x820]=[0x%08x]\n", ctx->device_id, scdc_value);


		if(scdc_value) {
			gHDMI20RxHandler[ctx->device_id].isScdcUD = 1;
		}
		else {
			if(gHDMI20RxHandler[ctx->device_id].isManualSyncMode == 1){
				HDMI20_VIDEO("SCDC ISR [%d] : Interrupt but on link reset[%d]\n", ctx->device_id, gHDMI20RxHandler[ctx->device_id].isManualSyncMode);
				break;
			}
			gHDMI20RxHandler[ctx->device_id].isScdcUD = 0;
		}

	} while(0);

	return ret;


}

/* This function is call on IRQ only when Deep Color Mode information has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_DCM(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_ERROR;
	struct hdmi_rx_ctrl_video v;
	UINT32 gcpdata = 0;
	UINT32 gcpdata1 = 0;

	do{
		if(gVideoDebugLevel == 911) break;

		if(gVideoDebugLevel == 913){
			if(gHDMI20RxHandler[ctx->device_id].aksv_change !=0){
				HDMI20_VIDEO("DBG %d : Disable DCM change on AKSV interrupt\n", ctx->device_id);
				break;
			}
		}

		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		/* Step 2. Wait for stablization */
		if(gHDMI20RxHandler[ctx->device_id].tmds_change != 0){
			if(gVideoDebugLevel == 628) HDMI20_VIDEO("DCM ISR [%d] : ISR but, on phy reset\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		if(gHDMI20RxHandler[ctx->device_id].ctrl.isMute != 0){
			if(gVideoDebugLevel == 628) HDMI20_VIDEO("DCM ISR [%d] : ISR but, on AVMute\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		/* Step 3. Get Previous TMDS Clock*/
		ret = hdmi_rx_ctrl_get_video(ctx, &v, 3);

		if(ret < 0) {
			if(ret != -ERANGE)HDMI20_VIDEO("Error when attempting to achieve video data[%d]\n",  ret);
			ret = RET_ERROR;
			break;
		}

		if(gVideoDebugLevel == 650){
			HDMI20_VIDEO("DCM ISR [%d] : ISR but disabled by debugger\n", ctx->device_id);
			break;
		}

		if(gVideoUseGCPPacketForDCM >0){
			gcpdata = (gHDMI20RxHandler[ctx->device_id].ctrl.bsp_read(0x380) >> 4) & 0x0000000F;

			if(gVideoDebugLevel == 917){
				HDMI20_VIDEO("DBG %d : deep color mode by 0x380. DCM[0x%x]\n", ctx->device_id, gcpdata);
			}


			switch(gcpdata)
			{
				case 7:
					gcpdata1= 48;
					break;
				case 6:
					gcpdata1= 36;
					break;
				case 5:
					gcpdata1= 30;
					break;
				default :
					gcpdata1= 24;
					break;
			}
		
			if(gcpdata1 != v.deep_color_mode){
				HDMI20_VIDEO("DBG %d : DCM ISR but, gcp packet[%d] != dedicated[%d]\n", ctx->device_id, gcpdata1, v.deep_color_mode);
				break;
			}
		}


		if (ctx->previous_video.deep_color_mode != v.deep_color_mode) {
			if(v.deep_color_mode == 24 || v.deep_color_mode == 30 || v.deep_color_mode == 36 || v.deep_color_mode == 48)
			{
				HDMI20_VIDEO("DCM ISR [%d] : Deep Color Mode Change ISR[%d]->[%d]\n",  ctx->device_id, ctx->previous_video.deep_color_mode, v.deep_color_mode);
				ctx->previous_video.deep_color_mode = v.deep_color_mode;

				gHDMI20RxHandler[ctx->device_id].dcm_change = true;

				if(gVideoDCMUpdateOnVideoChange == 1){
					__HDMI20_HAL_H15Ax_UpdateDCMMode(ctx->device_id, 0, v.deep_color_mode);
					gHDMI20RxHandler[ctx->device_id].dcm_change = false;
				}
			}
		}
		else{
			HDMI20_VIDEO("DCM ISR [%d] : Deep Color Mode ISR but same as previous[%d]->[%d]\n",  ctx->device_id, ctx->previous_video.deep_color_mode, v.deep_color_mode);
		}

	} while(0);

	return ret;


}


/* This function is call on IRQ only when TMDS CLK information has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_TMDS(struct hdmi_rx_ctrl *ctx)
{
	int ret = RET_ERROR;
	UINT32 tmds_clk = 0;
	struct hdmi_rx_ctrl_video v;

	do{
		/* Step 1. Check pointer */
		if(ctx == 0){
			break;
		}

		/* Step 2. Get TMDS Clock */
		tmds_clk = hdmi_rx_ctrl_get_tmds_clk(ctx);


		//HDMI20_CLOCK("Port[%d] IRQ on TMDS Clk. TCLK is %d\n", ctx->device_id, tmds_clk);

		if(tmds_clk == 0){
			/* Insert code when TMDS Clock is 0 */
			break;
		}

		if(gHDMI20RxHandler[ctx->device_id].ctrl.isMute != 0){
			if(gVideoDebugLevel == 628) HDMI20_VIDEO("TMDS ISR [%d] : ISR but, on AVMute\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		/* Step 4. Get Previous TMDS Clock*/
		ret = hdmi_rx_ctrl_get_video(ctx, &v, 0);

		/* Step 5. Change PHY setting for picture-repeatation only when TMDS CLK changed */
		if(tmds_clk != 0){
			if (((ctx->tmds_clock_old + gVideoTMDSTolerence) > tmds_clk) && \
					((ctx->tmds_clock_old - gVideoTMDSTolerence) < tmds_clk)) {
				ret = RET_OK;
				break;
			}
			else{
				//HDMI20_CLOCK("Port[%d] TMDS CLK changed from [%3u.%03uMHz] -> [%3u.%03uMHz]\n", ctx->device_id ,(unsigned int)(ctx->tmds_clock_old / 1000), (unsigned int)(ctx->tmds_clock_old %1000), (unsigned int)(tmds_clk/ 1000), (unsigned int)(tmds_clk % 1000));
				if(gVideoDebugLevel == 1213 ){
					HDMI20_VIDEO("DBG [%d] : TMDS changed over tolerence [%d] -> [%d]\n", ctx->device_id, ctx->tmds_clock_old, tmds_clk);	
				}

				gHDMI20RxHandler[ctx->device_id].tmds_change = true;

				gHDMI20RxHandler[ctx->device_id].stablehdcp14time = 0;
				///* Main thread TMDS_Change detect is slow */
				//gHDMI20RxHandler[ctx->device_id].ctrl.stable_cnt = 0;
				gHDMI20RxHandler[ctx->device_id].check19201080Dcm = 0;
				gHDMI20RxHandler[ctx->device_id].is19201080Dcm = 0;
			}
			/* Update TMDS Clock information for DEVICE Handler */
			//ret = hdmi_rx_ctrl_get_video(ctx, &v, 0);
			ctx->tmds_clock_old = tmds_clk;

		}

	} while(0);

	return ret;
}


static int __HDMI20_HAL_H15Ax_UpdateTimingInfo(struct hdmi_rx_ctrl *ctx, int clear)
{
	int ret = RET_ERROR;
	struct hdmi_rx_ctrl_video v = {0};
	UINT32 hvtotal;
	UINT32 vactive;
	UINT32 vtotal;
	UINT32 hactive;
	UINT32 htotal;
	UINT32 hblank;
	UINT32 vblank;
	UINT32 hfront;
	UINT32 vfront;
	UINT32 hsync;
	UINT32 vsync;
	UINT32 data;

	UINT32 hBlankHactive;
	UINT32 hFrontHsync;
	UINT32 vFrontVsync;
	UINT32 vBlankVactive;

	UINT32 divider;

	if(clear){
		HDMI20_VIDEO("Port[%d] : Clear Video timing info\n", ctx->device_id);
		memset((void *)&gHDMI20RxHandler[ctx->device_id].timing_info, 0, sizeof(struct hdmi_rx_ctrl_video));
		memset((void *)&gHDMI20RxHandler[ctx->device_id].sync_info, 0, sizeof(HDMI20_HAL_DBG_SYNC_INFO_T));

		gHDMI20RxHandler[ctx->device_id].stablehdcp14time = 0;
		
		gHDMI20RxHandler[ctx->device_id].real_vfreq = 0;

		ret = RET_OK;
	}
	else{
		if(gVerboseOnAbnormalVideo >= 1){
			ret = hdmi_rx_ctrl_get_video(ctx, &v, 4);
		}
		else{
			ret = hdmi_rx_ctrl_get_video(ctx, &v, 2);
		}

		if(gVideoDebugLevel == 5959){
			HDMI20_VIDEO("Port[%d] Warning : Forced Update Video Timing Info\n", ctx->device_id);
			memcpy((void *)&gHDMI20RxHandler[ctx->device_id].timing_info, (void *)&v, sizeof(struct hdmi_rx_ctrl_video));
			ret = RET_OK;
		}
		else if(ret < 0) {
			HDMI20_VIDEO("Port[%d] : Can not update Video timing info. Video Bad\n", ctx->device_id);
			memset((void *)&gHDMI20RxHandler[ctx->device_id].timing_info, 0, sizeof(struct hdmi_rx_ctrl_video));
			gHDMI20RxHandler[ctx->device_id].isTimingInvalid = 1;

			gHDMI20RxHandler[ctx->device_id].ctrl.stable_cnt = 0;

			ret = RET_ERROR;

		}
		else{
			HDMI20_VIDEO("Port[%d] Notice : Update Video Timing Info. [%ux%u%c: %s, DCM %ub, VIC:%u]\n", ctx->device_id,\
					v.hactive, v.vactive, v.interlaced? 'i' : 'p', v.dvi? "DVI" : "HDMI", v.deep_color_mode, v.video_mode);

			gHDMI20RxHandler[ctx->device_id].isTimingInvalid = 0;
			gHDMI20RxHandler[ctx->device_id].real_vfreq = 0;

			__HDMI20_HAL_H15Ax_UpdateDCMMode(ctx->device_id, 0, v.deep_color_mode);
			__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(ctx->device_id);
		
			hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[ctx->device_id].ctrl);
			__HDMI20_HAL_H15Ax_SetVideoColorDomain(ctx->device_id);

			/* Update Manual Sync*/
			if(ctx->device_id == 0){
				hactive = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x210) & 0x0000FFFF;
				htotal  = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x210) & 0xFFFF0000)>>16;
				vactive = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C) & 0x0000FFFF;
				vtotal  = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x20C) & 0xFFFF0000)>>16;
			}
			else if(ctx->device_id == 1){
				hactive = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x210) & 0x0000FFFF;
				htotal  = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x210) & 0xFFFF0000)>>16;
				vactive = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C) & 0x0000FFFF;
				vtotal  = (__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x20C) & 0xFFFF0000)>>16;
			}



			if(gHDMI20RxHandler[ctx->device_id].currentDCMMode== 0x5){
				divider = 125;	
			}
			else if(gHDMI20RxHandler[ctx->device_id].currentDCMMode== 0x6){
				divider = 150;	
			}
			else if(gHDMI20RxHandler[ctx->device_id].currentDCMMode== 0x7){
				divider = 200;
			}
			else{
				divider =  100;
			}

			if(v.interlaced){
				vtotal = gHDMI20RxHandler[ctx->device_id].vtotalbuffer;
			}


			/*
			if(vactive%2 != 0){
				if(vactive%10 == 1) vactive -= 1;
				else if(vactive%10 == 9) vactive += 1;
				else if(vactive%8 == 1) vactive -= 1;
				else if(vactive%8 == 7) vactive += 1;
				else if(vactive%4 == 1) vactive -= 1;
				else if(vactive%4 == 3) vactive += 1;
			}
			*/
			if(vtotal >= 600){
				if(vtotal%2 != 0){
					if(vtotal%5 != 0){
						if(vtotal%10 == 1) vactive -= 1;
						else if(vtotal%10 == 9) vactive += 1;
						else if(vtotal%8 == 1) vactive -= 1;
						else if(vtotal%8 == 7) vactive += 1;
						else if(vtotal%4 == 1) vtotal -= 1;
						else if(vtotal%4 == 3) vtotal += 1;
					}
				}
			}

			hactive = (hactive * 100)/divider;
			htotal = (htotal * 100)/divider;

			hblank = htotal - hactive;
			vblank = vtotal - vactive;

			if(hblank%2 != 0){
				if(hblank%10 == 1) hblank -= 1;
				else if(hblank%10 == 9) hblank += 1;
				else if(hblank%8 == 1) hblank -= 1;
				else if(hblank%8 == 7) hblank += 1;
				else if(hblank%4 == 1) hblank -= 1;
				else if(hblank%4 == 3) hblank += 1;
			}

			if(hactive%2 != 0){
				if(hactive%10 == 1) hactive -= 1;
				else if(hactive%10 == 9) hactive += 1;
				else if(hactive%8 == 1) hactive -= 1;
				else if(hactive%8 == 7) hactive += 1;
				else if(hactive%4 == 1) hactive -= 1;
				else if(hactive%4 == 3) hactive += 1;
			}
			if(htotal%2 != 0){
				if(htotal%10 == 1) htotal -= 1;
				else if(htotal%10 == 9) htotal += 1;
				else if(htotal%8 == 1) htotal -= 1;
				else if(htotal%8 == 7) htotal += 1;
				else if(htotal%4 == 1) htotal -= 1;
				else if(htotal%4 == 3) htotal += 1;
			}

			if(hblank){
				//hfront = (hblank * 30)/100;
				//hsync = (hblank * 15)/100;
				hfront = 2;
				hsync = 2;
			}
			else {
				hfront = 0;
				hsync = 0;
			}

			if(vblank){
				//vfront = (vblank * 30)/100;
				//vsync = (vblank * 15)/100;
				vfront = 2;
				vsync = 2;
			}
			else{
				vfront = 0;
				vsync = 0;
			}

			if((hactive == 0)||(htotal == 0)||(vactive == 0)||(vtotal == 0)||(vblank == 0)||(hblank == 0)){
				hBlankHactive = 0;
				hFrontHsync = 0;
				vFrontVsync = 0;
				vBlankVactive = 0;
				HDMI20_VIDEO("Notice %d : Manual Sync zero. [%d][%d][%d][%d][%d][%d]\n", ctx->device_id,\
						hactive, htotal, vactive, vtotal, vblank, hblank);
			}
			else{
				hBlankHactive = hblank; hBlankHactive = hBlankHactive << 16; hBlankHactive |= hactive;
				hFrontHsync = hfront; hFrontHsync = hFrontHsync << 16; hFrontHsync |= hsync;
				vFrontVsync = vfront; vFrontVsync = vFrontVsync << 16; vFrontVsync |= vsync;
				vBlankVactive = vblank; vBlankVactive = vBlankVactive << 16; vBlankVactive |= vactive;
			
				if(gVideoUseManualSyncOnFHD>0) HDMI20_VIDEO("Manual Sync value ready.. hBlankHactive[0x%08x], hFrontHsync[0x%08x] vFrontVsync[0x%08x] vBlankVactive[0x%08x]\n", hBlankHactive, hFrontHsync, vFrontVsync, vBlankVactive);
			}

			gHDMI20RxHandler[ctx->device_id].ctrl.hBlankHactive = hBlankHactive;
			gHDMI20RxHandler[ctx->device_id].ctrl.hFrontHsync = hFrontHsync;
			gHDMI20RxHandler[ctx->device_id].ctrl.vFrontVsync = vFrontVsync;
			gHDMI20RxHandler[ctx->device_id].ctrl.vBlankVactive = vBlankVactive;

			gHDMI20RxHandler[ctx->device_id].updateTimingTime = jiffies_to_msecs(jiffies);
			
			memcpy((void *)&gHDMI20RxHandler[ctx->device_id].timing_info, (void *)&v, sizeof(struct hdmi_rx_ctrl_video));
			
			hvtotal = (gHDMI20RxHandler[ctx->device_id].timing_info.vtotal * gHDMI20RxHandler[ctx->device_id].timing_info.htotal);

			if(hvtotal != 0){
				gHDMI20RxHandler[ctx->device_id].real_vfreq = ((gHDMI20RxHandler[ctx->device_id].real_pixel_clk * 1000) / hvtotal);
			}
			else{
				gHDMI20RxHandler[ctx->device_id].real_vfreq = 0;
			}

			ret = RET_OK;
		}
	}

	return ret;
}

static void __HDMI20_HAL_H15Ax_CheckVideoColorDomain(UINT8 port)
{
	UINT32 video_format = 0;
	UINT32 value_of_reg14 = 0;
	UINT32 value_of_reg18 = 0;
	UINT32 invalid = 0;

	#if 0
	if(video_format == 0){
		video_format = (UINT32)((gHDMI20RxHandler[port].dataPacket[AVI][5]&0x60)>>5);
	}
	#endif
	video_format = gHDMI20RxHandler[port].ctrl.bsp_read(0x3A4);
	video_format = (video_format & 0x60); video_format = video_format>>5;
	video_format = video_format & 0x3;

	if(port == 0){
		value_of_reg14 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x14);
		value_of_reg18 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x18);
	}
	else if(port == 1){
		value_of_reg14 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x14);
		value_of_reg18 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x18);
	}

	if(gVideoDebugLevel == 659){
		HDMI20_VIDEO("DBG %d : Color Domain 0x14[0x%08x]/0x18[0x%08x]\n", port, value_of_reg14, value_of_reg18);
	}

	if(video_format == 0){ /* RGB 444*/
		if(value_of_reg14 != 0x10006) invalid++;
		if(value_of_reg18 != 0x0) invalid++;
	}
	if(video_format == 1){ /* YUV 422*/
		if(value_of_reg14 != 0x21) invalid++;
		if(value_of_reg18 != 0x1) invalid++;

	}
	else if(video_format == 2){ /*  YUV 444 */
		if(value_of_reg14 != 0x21) invalid++;
		if(value_of_reg18 != 0x0) invalid++;

	}
	else if(video_format == 3) { /* YUV 420 */
		if(value_of_reg14 != 0x24) invalid++;
		if(value_of_reg18 != 0x0) invalid++;
	}

	if(invalid >0){
		HDMI20_VIDEO("Warning [%d] : Found wrong CSC setting for [%d] domain. Invalid[%d]\n", port, video_format, invalid);
		__HDMI20_HAL_H15Ax_SetVideoColorDomain(port);
	}
}

static void __HDMI20_HAL_H15Ax_SetVideoColorDomain(UINT8 port)
{
	UINT32 video_format = 0;
	
	video_format = gHDMI20RxHandler[port].ctrl.bsp_read(0x3A4);
	video_format = (video_format & 0x60); video_format = video_format>>5;
	video_format = video_format & 0x3;

	#if 0
	if(video_format == 0){
		video_format = (UINT32)((gHDMI20RxHandler[port].dataPacket[AVI][5]&0x60)>>5);
	}
	#endif

	if(video_format == 0){ /* RGB 444*/
		if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x14, 0x10006);
		}
		else if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x14, 0x10006);
		}
	}
	if(video_format == 1){ /* YUV 422*/
		if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x18, 0x1);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x14, 0x21);
		}
		else if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x18, 0x1);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x14, 0x21);
		}
	}
	else if(video_format == 2){ /*  YUV 444 */
		if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x14, 0x21);
		}
		else if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x14, 0x21);
		}
	}
	else if(video_format == 3) { /* YUV 420 */
		if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x14, 0x24);
		}
		else if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x18, 0x0);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x14, 0x24);
		}
	}
}

/* This function is only called by IRQ when there is a change in video signal */
static int __HDMI20_HAL_H15Ax_Refresh_Video(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;
	int vfreq = 0;
	int prev_vfreq=0;
	struct hdmi_rx_ctrl_video v = {0};
	UINT32 isPhyLocked = 0;
	UINT32 data;

	do{
		/* Step 1. Check Pointer */
		if (ctx == 0)
		{
			ret = RET_ERROR;
			break;
		}

		/* Step 2. Wait for stablization */
		if(gHDMI20RxHandler[ctx->device_id].tmds_change != 0){
			if(gVideoDebugLevel == 628)HDMI20_VIDEO("VIDEO ISR [%d] : ISR but, on phy reset\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		if(gHDMI20RxHandler[ctx->device_id].ctrl.isMute != 0){
			if(gVideoDebugLevel == 628)HDMI20_VIDEO("VIDEO ISR [%d] : ISR but, on AVMute\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		/* Step 3. Get Video sync param */
		ret = hdmi_rx_ctrl_get_video(ctx, &v, 1);
		if(ret < 0) {
			if(ret != -ERANGE)HDMI20_VIDEO("Error when attempting to achieve video data[%d]\n",  ret);
			ret = RET_ERROR;
			break;
		}

		/* Step 4. If video change, save status & set PHY */
		if ((ret == 0) &&
				(((ctx->previous_video.hactive + 10) < (v.hactive)) ||
				 ((ctx->previous_video.hactive != 0) && ((ctx->previous_video.hactive - 10) > (v.hactive))) ||
				 ((ctx->previous_video.vactive + 5) < (v.vactive)) ||
				 ((ctx->previous_video.vactive != 0) && ((ctx->previous_video.vactive - 5) > (v.vactive))) ||
				 (ctx->previous_video.pixel_repetition != v.pixel_repetition) ||
				 //(ctx->previous_video.deep_color_mode != v.deep_color_mode) ||
				 ((ctx->previous_video.refresh_rate + 3) < (v.refresh_rate)) ||
				 ((ctx->previous_video.refresh_rate != 0) && ((ctx->previous_video.refresh_rate - 3) > (v.refresh_rate))) ||
				 (ctx->previous_video.dvi != v.dvi) ||
				 (ctx->previous_video.video_mode != v.video_mode) ||
				 (ctx->previous_video.interlaced != v.interlaced)
				)
		   )
		{
			__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(ctx->device_id, 1, 0, __L__);

			if((ctx->previous_video.pixel_clk == 0)||(ctx->previous_video.vtotal == 0)||(ctx->previous_video.htotal)) {
				prev_vfreq = 0;
			}
			else{
				prev_vfreq = (ctx->previous_video.pixel_clk *1000) /( ctx->previous_video.vtotal* ctx->previous_video.htotal);
			}

			if((v.pixel_clk == 0)||(v.vtotal == 0)||(v.htotal == 0)){
				vfreq = 0;
			}
			else{
				vfreq = (v.pixel_clk * 1000 )/(v.vtotal * v.htotal);
			}

			/* Debug print */
			HDMI20_VIDEO("Video Change.. [%ux%u%c@%uHz: %s, DCM %ub, VIC:%u] -> [%ux%u%c@%uHz: %s, DCM %ub, VIC:%u]. \n",\
					ctx->previous_video.hactive, ctx->previous_video.vactive, \
					ctx->previous_video.interlaced? 'i' : 'p',prev_vfreq,\
					ctx->previous_video.dvi? "DVI" : "HDMI",\
					ctx->previous_video.deep_color_mode, ctx->previous_video.video_mode,\
					v.hactive, v.vactive, v.interlaced? 'i' : 'p', vfreq ,\
					v.dvi? "DVI" : "HDMI", v.deep_color_mode, v.video_mode);

			/* Important to clear ManualSYnc value */
			data = gHDMI20RxHandler[ctx->device_id].ctrl.bsp_read(0x400);
			data &= (~0x0000000F);
			gHDMI20RxHandler[ctx->device_id].ctrl.bsp_write(0x400, data);
			gHDMI20RxHandler[ctx->device_id].ctrl.bsp_write(0x404, 0x0);

			if( gUseVideoChangeFlagOnMainThread>0) gHDMI20RxHandler[ctx->device_id].video_change = true;
		
			if (ctx->previous_video.deep_color_mode != v.deep_color_mode) {
				if(v.deep_color_mode == 24 || v.deep_color_mode == 30 || v.deep_color_mode == 36 || v.deep_color_mode == 48)
				{
					HDMI20_VIDEO("Video ISR [%d] : Deep Color Mode Change ISR[%d]->[%d]\n",  ctx->device_id, ctx->previous_video.deep_color_mode, v.deep_color_mode);
					ctx->previous_video.deep_color_mode = v.deep_color_mode;

					gHDMI20RxHandler[ctx->device_id].dcm_change = true;

					if(gVideoDCMUpdateOnVideoChange == 1){
						__HDMI20_HAL_H15Ax_UpdateDCMMode(ctx->device_id, 0, v.deep_color_mode);
						gHDMI20RxHandler[ctx->device_id].dcm_change = false;
					}
				}
			}


			gHDMI20RxHandler[ctx->device_id].stablehdcp14time = 0;
			gHDMI20RxHandler[ctx->device_id].check19201080Dcm = 0;
			gHDMI20RxHandler[ctx->device_id].is19201080Dcm = 0;

			/* Update Device Handler video information */
			ctx->previous_video.dvi = v.dvi;
			ctx->previous_video.pixel_clk = v.pixel_clk;
			ctx->previous_video.refresh_rate = v.refresh_rate;
			ctx->previous_video.interlaced = v.interlaced;
			ctx->previous_video.video_mode = v.video_mode;
			ctx->previous_video.htotal = v.htotal;
			ctx->previous_video.vtotal = v.vtotal;
			ctx->previous_video.hactive = v.hactive;
			ctx->previous_video.vactive  = v.vactive;
			ctx->previous_video.pixel_repetition = v.pixel_repetition;
		}
		else{
			if(gVideoDebugLevel == 1105)
			{
				HDMI20_VIDEO("ISR : No video change or change within torrerence\n");
			}
		}
		ret = RET_OK;
	}while(0);

	return ret;
}


/* This function is only called by IRQ when AUDIO information has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_Audio(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;
	//struct hdmi_rx_ctrl_audio a;
	unsigned int samplingFreq = 0;

	HDMI20_AUDIO("__HDMI20_HAL_H15Ax_Refresh_Audio (audio pll:0x%x)\n", gHDMI20RxHandler[ctx->device_id].ctrl.bsp_read(0x208));

	do{
		/* Step 1. Check pointer */
		if (ctx == 0){
			ret = RET_ERROR;
			break;
		}
		
		#if 0
		/* Step 2. Wait for stablization */
		if(gHDMI20RxHandler[ctx->device_id].tmds_change != 0){
			if(gVideoDebugLevel == 628)HDMI20_VIDEO("Audio ISR [%d] : ISR but, on phy reset\n", ctx->device_id);
			ret = RET_OK;
			break;
		}

		if(gHDMI20RxHandler[ctx->device_id].ctrl.isMute != 0){
			if(gVideoDebugLevel == 628)HDMI20_VIDEO("Audio ISR [%d] : ISR but, on AVMute\n", ctx->device_id);
			ret = RET_OK;
			break;
		}
		#endif

		/* Step 2. Get Audio info */
		ret = hdmi_rx_ctrl_get_audio(ctx, &gHDMI20RxHandler[ctx->device_id].audio);

		if( ret < 0) {
			HDMI20_AUDIO("Error when attemting to achieve AUDIO data[%d]\n", ret);
			ret = RET_ERROR;
			break;
		}

		gHDMI20RxHandler[ctx->device_id].audio_change = 1;
		gHDMI20RxHandler[ctx->device_id].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
		/* Soft reset for iaudclk domain(dmi_sw_rst.aud_swreset) */
		gHDMI20RxHandler[ctx->device_id].ctrl.bsp_write(0xff0, 0x00000010);

		/* Step 3. */
		/* Reprocess Audio Sampling freq */
		ret = __HDMI20_HAL_H15Ax_Rx_Get_AudioFreqFromTMDSClock((UINT8)ctx->device_id, &samplingFreq);
		if( ret <0 ){
			HDMI20_AUDIO("Cannot reproduce AudioFreq from TMDS Clock[%d]\n", ret);
			ret = RET_ERROR;
			break;
		}
		gHDMI20RxHandler[ctx->device_id].audio.sample_frequency = samplingFreq;

		ret = RET_OK;
	}while(0);

	return ret;
}

static void __HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(int port, int clear)
{

	/* Block this update to prevent unsteady state of N/CTS value */
	/* SW workaround for AUDIO */
	gHDMI20RxHandler[port].ctrl.n_param = (0x000FFFFF & gHDMI20RxHandler[port].ctrl.bsp_read(0x394));
	gHDMI20RxHandler[port].ctrl.cts_param = (0x000FFFFF & gHDMI20RxHandler[port].ctrl.bsp_read(0x390));

	if((gVideoDebugLevel == 690)||(gVideoDebugLevel == 6969)){
		HDMI20_VIDEO("DBG [%d] : N/CTS value => [0x%08x][0x%08x]\n", port, gHDMI20RxHandler[port].ctrl.n_param, gHDMI20RxHandler[port].ctrl.cts_param);
	}

	if(gVideoDebugLevel == 6969){return;}


	if(clear){
		gHDMI20RxHandler[port].ctrl.n_param = 0;
		gHDMI20RxHandler[port].ctrl.cts_param = 0;
	}

	if((gHDMI20RxHandler[port].is19201080Dcm > 0)&&(gHDMI20RxHandler[port].check19201080Dcm >0))
	{
		if(gVideoDebugLevel == 1114){HDMI20_VIDEO("DBG [%d] : SW WA for 1920x1080 50/60 DCM10\n", port);}
		
		if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x38, 0x2);
		}
		else if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x38, 0x2);
		}
	}
	else{
		if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x38, 0x1);
		}
		else if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x38, 0x1);
		}
	}

	/* Sw workaround */
	if(port == 0){
		if(gHDMI20RxHandler[port].phyInitiatedMode){ // If UD input
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0( HDMI20_RX_ACR_CTS, (gHDMI20RxHandler[port].ctrl.cts_param>>2));
		}
		else{ // if FHD input
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0( HDMI20_RX_ACR_CTS, gHDMI20RxHandler[port].ctrl.cts_param);
		}

		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0( HDMI20_RX_ACR_N, gHDMI20RxHandler[port].ctrl.n_param);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0( HDMI20_RX_ACR_PLL_V, __HDMI20_HAL_H15Ax_Calculate_PLL_Value( gHDMI20RxHandler[port].real_tmds_clk));
	}
	else{
		if(gHDMI20RxHandler[port].phyInitiatedMode){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1( HDMI20_RX_ACR_CTS,  (gHDMI20RxHandler[port].ctrl.cts_param>>2));
		}
		else{
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1( HDMI20_RX_ACR_CTS,  gHDMI20RxHandler[port].ctrl.cts_param);
		}

		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1( HDMI20_RX_ACR_N,  gHDMI20RxHandler[port].ctrl.n_param);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1( HDMI20_RX_ACR_PLL_V, __HDMI20_HAL_H15Ax_Calculate_PLL_Value(  gHDMI20RxHandler[port].real_tmds_clk));

	}
}

static UINT32 __HDMI20_HAL_H15Ax_Calculate_PLL_Value(UINT32 tmds_clk)
{
	/* For DBG purpose */
	if(gFixedPLLMode != 0){
		if(gFixedPLLMode == 8282){
			return 0;
		}
		else{
			return gFixedPLLMode;
		}
	}

	if((tmds_clk >= 0)&&(tmds_clk <= 46250)){
		return 0;
	}
	else if((tmds_clk > 46250)&&(tmds_clk <= 92500)){
		return 1;
	}
	else if((tmds_clk > 92500)&&(tmds_clk <= 185000)){
		return 2;
	}
	else if((tmds_clk > 185000)&&(tmds_clk <= 340000)){
		return 3;
	}
	else if((tmds_clk > 340000)){
		return 3; // 2014/07/08 temperary
	}
	else return 0;
}



/* This function is only called by IRQ when DATAPACKET has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_DataPacket(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;
	UINT8 data[32] = {0};
	int i;
	int enumtype;

	do {
		/* Step 1. Check pointer */
		if (ctx == 0){
			ret = RET_ERROR;
			break;
		}

		/* Step 2. Get Packet Info */
		ret = hdmi_rx_ctrl_get_packet(ctx, data, sizeof(data));
		if ( ret < 0 ) {
			if(ret != -ENODATA) HDMI20_PACKET("Error attempting to achieve Data Packet[%d]\n", ret);
			ret = RET_ERROR;
			break;
		}

		if( ret == 0) {
			/* Disable duplicate incoming packets */
			ret = __HDMI20_HAL_H15Ax_Packet_Store(ctx, data[0], false);
		}

		/* Added by won.hur : Save data packets to buffer */
		switch (data[0])
		{
			case 0x01:
				enumtype =  hdmi_rx_ctrl_packet_ACR;
				break;
			case 0x03:
				enumtype =  hdmi_rx_ctrl_packet_GCP;
				break;
			case 0x04:
				enumtype =  hdmi_rx_ctrl_packet_ACP;
				break;
			case 0x05:
				enumtype =  hdmi_rx_ctrl_packet_ISRC1;
				break;
			case 0x06:
				enumtype =  hdmi_rx_ctrl_packet_ISRC2;
				break;
			case 0x0A:
				enumtype =  hdmi_rx_ctrl_packet_GAMUT;
				break;
			case 0x81:
				enumtype =  hdmi_rx_ctrl_packet_VSI;
				break;
			case 0x82:
				enumtype =  hdmi_rx_ctrl_packet_AVI;
				break;
			case 0x83:
				enumtype =  hdmi_rx_ctrl_packet_SPD;
				break;
			case 0x84:
				enumtype =  hdmi_rx_ctrl_packet_AIF;
				break;
			case 0x85:
				enumtype =  hdmi_rx_ctrl_packet_MPEGS;
				break;
			default:
				enumtype = hdmi_rx_ctrl_packet_cnt;
				break;
		}



		if(enumtype < hdmi_rx_ctrl_packet_cnt){
			//HDMI20_VIDEO("Recieved DataPacket[0x%x] for Link[%d]\n", data[0], ctx->device_id);
			for(i=0;i<MAX_DATA_PACKET_LENGTH;i++){
				gHDMI20RxHandler[ctx->device_id].dataPacket[enumtype][i] = data[i];
				//HDMI20_INFO(" Buffer[%d] <= 0x%x (0x%x)\n", i, gHDMI20RxHandler[ctx->device_id].dataPacket[type][i], data[i]);
			}
		}
		ret = RET_OK;
	} while(0);

	return ret;
}

/* This function is only called by IRQ when AKSV data has been changed */
static int __HDMI20_HAL_H15Ax_Refresh_AKSV(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;

	__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(ctx->device_id, 1, 1, __L__);

	if(gReinitHDCP14OnAksvISR){
		HDMI20_AKSV("AKSV update [%d] : Request HDCP1.4 init\n", ctx->device_id);
		/* HDCP reinit*/
		hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[ctx->device_id].ctrl, &gHDMI20RxHandler[ctx->device_id].hdcp);
	}
	else{
		HDMI20_AKSV("AKSV ISR [%d] : But not requesting HDCP 1.4 Init\n", ctx->device_id);
	}


	//__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(ctx->device_id);
	gHDMI20RxHandler[ctx->device_id].aksv_change = 1;
	

	ret = __HDMI20_HAL_H15Ax_Refresh_Video(ctx);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_PLLLock(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;


	if(gHDMI20RxHandler[ctx->device_id].is5Vconnected == 1){
		if(gHDMI20RxHandler[ctx->device_id].ctrl.bsp_read(0x30) & 0x00000001){
			if(gVideoDebugLevel == 1123) HDMI20_VIDEO("PLL ISR [%d] => PLL Lock UP\n",ctx->device_id);
		}
		else{
			if(gVideoDebugLevel == 1123) HDMI20_VIDEO("PLL ISR [%d] => PLL Lock Down\n",ctx->device_id);
			if(__HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(ctx->device_id) == 0) {
				__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(ctx->device_id, 1, 0, __L__);
			}

			
			//__HDMI20_HAL_H15Ax_UpdateTimingInfo(ctx, 1);
		}
	}
	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_PhyI2CDone(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;

	gHDMI20RxHandler[ctx->device_id].i2cDoneSignal++;
	HDMI20_VIDEO("ISR [%d] => Receieved Phy I2C Done[%d]\n", ctx->device_id, gHDMI20RxHandler[ctx->device_id].i2cDoneSignal);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Refresh_PhyI2CNack(struct hdmi_rx_ctrl *ctx)
{
	int ret = 0;

	gHDMI20RxHandler[ctx->device_id].i2cNackSignal++;
	HDMI20_VIDEO("ISR [%d] => Receieved Phy I2C Nack[%d]\n", ctx->device_id,gHDMI20RxHandler[ctx->device_id].i2cNackSignal);

	return ret;
}


static int __HDMI20_HAL_H15Ax_Packet_Store(struct hdmi_rx_ctrl *ctx, UINT8 type, bool enable)
{
	int error = 0;

	if (ctx == 0)
	{
		return -EINVAL;
	}
	switch (type)
	{
		case 0x01:
			//HDMI20_PACKET("Port %d : Packet Type ACR[0x%x]  En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ACR, enable);
			break;
		case 0x03:
			//HDMI20_PACKET("Port %d : Packet Type GCP[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_GCP, enable);
			break;
		case 0x04:
			//HDMI20_PACKET("Port %d : Packet Type ACP[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ACP, enable);
			break;
		case 0x05:
			//HDMI20_PACKET("Port %d : Packet Type ISRC1[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ISRC1, enable);
			break;
		case 0x06:
			//HDMI20_PACKET("Port %d :Packet Type ISRC2[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ISRC2, enable);
			break;
		case 0x0A:
			//HDMI20_PACKET("Port %d : Packet Type GAMUT[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_GAMUT, enable);
			break;
		case 0x81:
			//HDMI20_PACKET("Port %d : Packet Type VSI[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_VSI, enable);
			break;
		case 0x82:
			//HDMI20_PACKET("Port %d : Packet Type AVI[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_AVI, enable);
			break;
		case 0x83:
			//HDMI20_PACKET("Port %d : Packet Type SPD[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_SPD, enable);
			break;
		case 0x84:
			//HDMI20_PACKET("Port %d : Packet Type AIF[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_AIF, enable);
			break;
		case 0x85:
			//HDMI20_PACKET("Port %d : Packet Type MPEGS[0x%x] En[%d]\n", ctx->device_id, type, enable);
			error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_MPEGS, enable);
			break;
		default:
			error = -EINVAL;
			break;
	}
	return error;
}



static int __HDMI20_HAL_H15Ax_Rx_UnInitialize(void)
{
	int ret = RET_OK;

	do{

	} while(0);

	return ret;

}

static void __HDMI20_HAL_H15Ax_TestPattern(UINT8 port, int isUD, int isOn)
{

	if(port == 0){
		if(isOn){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x4, 0x6);
			if(isUD){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0508, 0x08CA1130); //Vtotal = 2250, Htotal = 4400
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x050C, 0x087C0886); //Vs_srt = 2172, Vs_end = 2182
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0510, 0x00040874); //Va_srt =    4, Va_end = 2164
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0514, 0x005800B0); //Hs_srt =   88, Hs_end =  176
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0518, 0x01D810D8); //Ha_srt =  472, Ha_end = 4312

			}
			else{
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0508, 0x04650898); // Vtotal = 1125 Htotal = 2200
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x050C, 0x04400445); // VS SRT = 1088 VS END = 1093
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0510, 0x0004043C);// VA SRT =    4 VA END = 1084
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0514, 0x002C0058); // HS SRT =   44 HS END =   88
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0518, 0x00EC086C); // HA SRT =  236 HA END = 2156

			}
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0600, 0x00000011); // PATTERN EN
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0500, 0x00000020); // Sync gen Load Setting
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0500, 0x00000120); //
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0500, 0x00000080); //
		}
		else{
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x4, 5);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x600, 0);
		}
	}
	else if(port == 1){
		if(isOn){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x4, 0x6);
			if(isUD){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0508, 0x08CA1130); //Vtotal = 2250, Htotal = 4400
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x050C, 0x087C0886); //Vs_srt = 2172, Vs_end = 2182
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0510, 0x00040874); //Va_srt =    4, Va_end = 2164
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0514, 0x005800B0); //Hs_srt =   88, Hs_end =  176
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0518, 0x01D810D8); //Ha_srt =  472, Ha_end = 4312

			}
			else{
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0508, 0x04650898); // Vtotal = 1125 Htotal = 2200
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x050C, 0x04400445); // VS SRT = 1088 VS END = 1093
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0510, 0x0004043C);// VA SRT =    4 VA END = 1084
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0514, 0x002C0058); // HS SRT =   44 HS END =   88
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0518, 0x00EC086C); // HA SRT =  236 HA END = 2156

			}
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0600, 0x00000011); // PATTERN EN
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0500, 0x00000020); // Sync gen Load Setting
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0500, 0x00000120); //
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0500, 0x00000080); //
		}
		else{
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x4, 5);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x600, 0);
		}
	}
}

static int __HDMI20_HAL_H15Ax_Tx_Initialize(void)
{
	int ret = RET_OK;

	HDMI20_INFO("Function called \n");

	do{
		/* Step 1. Init Tx Device Handler */
		HDMI20_INFO("Initiating HDMI20 Tx device handler \n");
		__HDMI20_HAL_H15Ax_Tx_Init_Dev_Handler();

		/* Step 2. Ioremap HW to memory */
		if(gHDMI20TxHandler.topmem.pSwAddr != NULL) {
			gHDMI20TxHandler.topmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20TxHandler.topmem.hwAddr, gHDMI20TxHandler.topmem.size);
		}
		else {
			iounmap((void *)gHDMI20TxHandler.topmem.pSwAddr);
			gHDMI20TxHandler.topmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20TxHandler.topmem.hwAddr, gHDMI20TxHandler.topmem.size);
		}

		if(gHDMI20TxHandler.linkmem.pSwAddr != NULL) {
			gHDMI20TxHandler.linkmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20TxHandler.linkmem.hwAddr, gHDMI20TxHandler.linkmem.size);
		}
		else {
			iounmap((void *)gHDMI20TxHandler.linkmem.pSwAddr);
			gHDMI20TxHandler.linkmem.pSwAddr = (volatile UINT32 *)ioremap(gHDMI20TxHandler.linkmem.hwAddr, gHDMI20TxHandler.linkmem.size);
		}

		HDMI20_INFO("MAPPED Tx : TOP[0x%08x] / CTRL[0x%08x]  \n",  (unsigned int)gHDMI20TxHandler.topmem.pSwAddr, (unsigned int)gHDMI20TxHandler.linkmem.pSwAddr);

		/* Step 3. Initially set settings to 4K 60Hz */
		gWasTXInitiated = 1;
		__HDMI20_HAL_H15Ax_Tx_SetMode4K60Hz();
		gUseHDMI20Tx = 1;
		

		/* Set PUK/DUK : HDCP2.2 */
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_PUK_ADDR_1, HDMI20_H15AX_PUK_1);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_PUK_ADDR_2, HDMI20_H15AX_PUK_2);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_PUK_ADDR_3, HDMI20_H15AX_PUK_3);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_PUK_ADDR_4, HDMI20_H15AX_PUK_4);

		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_DUK_ADDR_1, HDMI20_TX_H15AX_DUK_1);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_DUK_ADDR_2, HDMI20_TX_H15AX_DUK_2);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_DUK_ADDR_3, HDMI20_TX_H15AX_DUK_3);
		__HDMI20_HAL_H15Ax_Tx_TOP_Write(HDMI20_H15AX_DUK_ADDR_4, HDMI20_TX_H15AX_DUK_4);

		gTxThreadAlive = 1;
		gTxthread = kthread_create( (void *)__HDMI20_HAL_H15Ax_TxThread, 0, "hdmi20_tx_video_stable_checker");
		if(gTxthread){
			HDMI20_INFO("Initialing HDMI20 TX Stable checker thread\n");
			wake_up_process(gTxthread);
		}else HDMI20_ERROR("Cannot initiate Video stablizor thread\n");


	} while(0);

	return ret;

}

static int __HDMI20_HAL_H15Ax_Tx_UnInitialize(void)
{
	int ret = RET_OK;

	do{

	} while(0);

	return ret;
}

static int __HDMI20_HAL_H15Ax_Rx_ListOfNotSupportingVideo(UINT8 port)
{
	int ret = RET_ERROR;

	do{
		/* No support on Pixel repition over 1 */
		if(gHDMI20RxHandler[port].timing_info.pixel_repetition >1){
			//HDMI20_VIDEO("ERROR %d : Does not support pixel repetition more than 1. PR[%d]\n", port, gHDMI20RxHandler[port].timing_info.pixel_repetition);
			break;
		}

		/* Invalid Format Check */
		if((gHDMI20RxHandler[port].timing_info.hactive < 320) || (gHDMI20RxHandler[port].timing_info.vactive < 240) )
		{
			HDMI20_VIDEO("ERROR %d : InValid Format for Active Size,  hActive[%d] vActive [%d]\n", port,\
					gHDMI20RxHandler[port].timing_info.hactive, gHDMI20RxHandler[port].timing_info.vactive);
			break;
		}

		/* Support 2D 1280x1024i@86  by 20120202
		 * Not support Master #333 - because MASTER timing issue */
		if (gHDMI20RxHandler[port].timing_info.hactive == 1280 && \
				gHDMI20RxHandler[port].timing_info.vactive == 512 &&\
				gHDMI20RxHandler[port].timing_info.interlaced == 0)
		{
			HDMI20_VIDEO("ERROR %d : not support 2D 1280x1024!@86 of Master #333\n", port);
			break;
		}

		//* Support 2D 1920x1080i@50 of EIA-861D  for any PC Card  by 20111010
		//* Not support Master #840 - because MASTER timing issue
		if (gHDMI20RxHandler[port].timing_info.htotal == 2304)// && gHDMI20RxHandler[port].timing_info.hActive == 1920 && gHDMI20RxHandler[port].timing_info.scanType == 1)
		{
			if (gHDMI20RxHandler[port].timing_info.vtotal == 1250)
			{
				HDMI20_VIDEO("NOTI [%d] : Support 2D 1920x1080!@50 of EIA-861D \n",port);

			}
			else
			{
				HDMI20_VIDEO("ERROR %d : not support 2D 1920x1080!@50 of EIA-861D Master #840\n", port);
				break;
			}
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_TimingInfo(UINT8 port, LX_HDMI_TIMING_INFO_T *pData, int size)
{
	int ret = RET_ERROR;
	int i;
	LX_HDMI_TIMING_INFO_T userdata;
	LX_HDMI_VSI_3D_STRUCTURE_T	vsi3DData = LX_HDMI_VSI_3D_STRUCTURE_MAX;
	LX_HDMI_EXT_FORMAT_INFO_T	lg3DData = LX_HDMI_EXT_2D_FORMAT;
	int vfreq,hfreq;
	UINT32 data, data1;
	UINT32 vsyncTime = 0;
	UINT32 hsyncTime = 0;
	int hvtotal = 0;
	UINT32 fix_offset, ctop_hdmi_clk_div;
	const sEXT_TIMING_ENUM *pTbl;
	const sO_ENUM *p3DTbl;
	int device3DvideoWithoutVSI = 0;
	int detected_error = 0;
	UINT32 phyInitiatedMode = 0;
	UINT32 valueOfPhyClk = 0;
	UINT32 currentTime = 0;

	do{
		/* Step 0. Memset */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_TIMING_INFO_T));

		/* Step 0-1. Break if not connected */
		if(gHDMI20RxHandler[port].is5Vconnected == 0){
			ret = RET_OK;
			break;
		}

		/* Step 0-2. Break if HPD is down */
		if(__HDMI20_HAL_H15Ax_IsHPD_Up(port) == 0){
			ret = RET_OK;
			break;
		}

		/* Step 1. Check given pointer */
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		/* Step 2. Check given data size */
		if(size != sizeof(LX_HDMI_TIMING_INFO_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_TIMING_INFO_T), size);
			break;
		}

		/* Step 3. Check port validity */
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if((gHDMI20RxHandler[port].ctrl.isMute == 0) &&(__HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(port)!=0)){
			__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(port, 0, 1, __L__);
		}


		/* Step 3-0. Block timing info on non video stable cnt */
		if(gHDMI20RxHandler[port].ctrl.stable_cnt <= gVideoStableCheckCnt){
			ret = RET_OK;
			break;
		}

		if((gHDMI20RxHandler[port].scdc_change != 0) || (gHDMI20RxHandler[port].video_change != 0) ||\
				(gHDMI20RxHandler[port].tmds_change != 0) ){
			ret = RET_OK;
			break;	
		}

		/* Step 3-1. Block operation on Phy Lock & non-steady state */
		if((gHDMI20RxHandler[port].timing_info.vactive == 0) ||(gHDMI20RxHandler[port].timing_info.hactive == 0))
		{
			ret = copy_to_user((LX_HDMI_TIMING_INFO_T *)pData, &userdata, sizeof(LX_HDMI_TIMING_INFO_T));
			if(ret) {
				HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "TIMING" ,port);
			}
			break;
		}

		/* Step 3-2. Block Not-supported videos */
		if( __HDMI20_HAL_H15Ax_Rx_ListOfNotSupportingVideo(port) < 0){
			//HDMI20_VIDEO("Error %d : Timing info to Zero due to Not supporting Timing spec\n", port);
			ret = copy_to_user((LX_HDMI_TIMING_INFO_T *)pData, &userdata, sizeof(LX_HDMI_TIMING_INFO_T));
			if(ret) {
				HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "TIMING" ,port);
			}

			gHDMI20RxHandler[port].ctrl.stable_cnt = 0;
			ret = RET_OK;
			break;
		}


		/* Step 4. Calculate Frequency to fit LGE MW */
		if((gHDMI20RxHandler[port].real_pixel_clk == 0)||(gHDMI20RxHandler[port].timing_info.htotal == 0)){
			hfreq = 0;
		}
		else{
			hfreq = ((gHDMI20RxHandler[port].real_pixel_clk * 1000) / gHDMI20RxHandler[port].timing_info.htotal); 					///< Horizontal frequency(100 Hz unit)
		}



		/* Step 5. Check Range : pixel Clk, video sync data from HW is not accurate.. So i need to calucate the approperiate value for MW */
		if((gHDMI20RxHandler[port].real_pixel_clk == 0)||(gHDMI20RxHandler[port].timing_info.vtotal == 0)||(gHDMI20RxHandler[port].timing_info.htotal == 0)){
			vfreq = 0;
		}
		else{
			/* Check vTotal Range : Update this case statement through development process */
			if(gHDMI20RxHandler[port].timing_info.vtotal > 820){
				if(gHDMI20RxHandler[port].timing_info.vtotal > 1120){			
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 2250, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 2250;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1500, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1500;				
				    else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1235, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1235;//1235,1240
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1125, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1125;				
				}
				else{ 
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1100, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1100;//1099,1100				
				    else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1090, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1090;//1089,1090				
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 1000, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 1000;//996,1000			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 925, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 925;//926			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 825, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 825;//823
				}
			}
			else{
				if(gHDMI20RxHandler[port].timing_info.vtotal > 660){
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 805, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 805;//806,804,805
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 800, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 800;//798,802				
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 790, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 790;//791,790,795				
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 750, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 750;//755,748			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 666, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 666;//666,667
				}
				else{
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 625, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 625;//625
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 530, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 530;//529
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 525, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 525;//526,524,525
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 505, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 505;//504
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 495, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 495;//494			
				}
			}
				//else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vtotal, 900, 2)) gHDMI20RxHandler[port].timing_info.vtotal = 900;//905,

			//hvtotal = (gHDMI20RxHandler[port].timing_info.vtotal * gHDMI20RxHandler[port].timing_info.htotal);
			//vfreq = (UINT16)((gHDMI20RxHandler[port].real_pixel_clk * 1000) / hvtotal);
			vfreq = gHDMI20RxHandler[port].real_vfreq;

			if(gVideoDebugLevel == 1144){
				HDMI20_VIDEO("DBG %d : current Vfreq [%d]\n", port, vfreq);
			}

			/* SW WA */
			if(vfreq == 28) vfreq = 30;
			else if(vfreq == 14) vfreq = 15;

			if(vfreq%2){
				/* Always give oppertunity for originally odd vfreqs.. 23.97, 25, 59,97 */
				if(vfreq == 23) vfreq = 24;
				else if(vfreq == 25) vfreq = 25;
				else if(vfreq == 59) vfreq = 60;
				else if(u32CHK_RANGE(vfreq, 60, 2)) vfreq = 60;
				else if(u32CHK_RANGE(vfreq, 50, 2)) vfreq = 50;
				else if(u32CHK_RANGE(vfreq, 30, 2)) vfreq = 30;
				//else if(u32CHK_RANGE(vfreq, 25, 1)) vfreq = 25;
				else if(u32CHK_RANGE(vfreq, 24, 1)) vfreq = 24;
			}

			if(gHDMI20RxHandler[port].timing_info.video_format == 0x3){
				vfreq = vfreq << 1;
			}

			//check hActive Range : need to update the below case
			if(gHDMI20RxHandler[port].timing_info.hactive > 1360){
				if(gHDMI20RxHandler[port].timing_info.hactive > 1900){
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 4096, 5)) gHDMI20RxHandler[port].timing_info.hactive= 4096;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 3840, 5)) gHDMI20RxHandler[port].timing_info.hactive= 3840;			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 1920, 5)) gHDMI20RxHandler[port].timing_info.hactive= 1920;
				}
				else{							
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 1440, 2)) gHDMI20RxHandler[port].timing_info.hactive= 1440;			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 1366, 2)) gHDMI20RxHandler[port].timing_info.hactive= 1366;
				}
			}
			else{
				if(gHDMI20RxHandler[port].timing_info.hactive > 1020){
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 1280, 2)) gHDMI20RxHandler[port].timing_info.hactive= 1280;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 1024, 2)) gHDMI20RxHandler[port].timing_info.hactive= 1024;
				}
				else{					
				 	if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 720, 2)) gHDMI20RxHandler[port].timing_info.hactive= 720;
				 	else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.hactive, 640, 2)) gHDMI20RxHandler[port].timing_info.hactive= 640;
				}
			}	
		    if(gHDMI20RxHandler[port].timing_info.vactive > 700){	
				if(gHDMI20RxHandler[port].timing_info.vactive > 1000){
					if(gHDMI20RxHandler[port].timing_info.vactive == 1201) gHDMI20RxHandler[port].timing_info.vactive = 1201;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 2160, 5)) gHDMI20RxHandler[port].timing_info.vactive = 2160;						
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 1200, 2)) gHDMI20RxHandler[port].timing_info.vactive = 1200; 
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 1080, 2)) gHDMI20RxHandler[port].timing_info.vactive = 1080;			
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 1050, 2)) gHDMI20RxHandler[port].timing_info.vactive = 1050; 
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 1035, 2)) gHDMI20RxHandler[port].timing_info.vactive = 1035;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 1024, 2)) gHDMI20RxHandler[port].timing_info.vactive = 1024; 
					
				}
				else{		
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 960, 2)) gHDMI20RxHandler[port].timing_info.vactive = 960;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 900, 2)) gHDMI20RxHandler[port].timing_info.vactive = 900;				
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 870, 2)) gHDMI20RxHandler[port].timing_info.vactive = 870;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 864, 2)) gHDMI20RxHandler[port].timing_info.vactive = 864;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 800, 2)) gHDMI20RxHandler[port].timing_info.vactive = 800;				
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 768, 2)) gHDMI20RxHandler[port].timing_info.vactive= 768;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 720, 2)) gHDMI20RxHandler[port].timing_info.vactive = 720;	
		    	}
		    }
			else{
				if(gHDMI20RxHandler[port].timing_info.vactive > 490){					
				 	if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 624, 2)) gHDMI20RxHandler[port].timing_info.vactive = 624;
				 	else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 600, 2)) gHDMI20RxHandler[port].timing_info.vactive = 600;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 584, 2)) gHDMI20RxHandler[port].timing_info.vactive = 584;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 576, 2)) gHDMI20RxHandler[port].timing_info.vactive = 576;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 496, 2)) gHDMI20RxHandler[port].timing_info.vactive = 496;
				}
				else{
					if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 480, 2)) gHDMI20RxHandler[port].timing_info.vactive = 480;
			    	else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 483, 2)) gHDMI20RxHandler[port].timing_info.vactive = 483;					
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 400, 2)) gHDMI20RxHandler[port].timing_info.vactive = 400;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 350, 2)) gHDMI20RxHandler[port].timing_info.vactive = 350;
			   		else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 288, 2)) gHDMI20RxHandler[port].timing_info.vactive = 288;
					else if(u32CHK_RANGE(gHDMI20RxHandler[port].timing_info.vactive, 240, 2)) gHDMI20RxHandler[port].timing_info.vactive = 240;
		    	}
			}
		}

		if(gVideoDebugLevel == 570){
			HDMI20_VIDEO("DBG : PCLK[%d] / hv[%d] = vfreq[%d] --> vfreq[%d]\n",  gHDMI20RxHandler[port].real_pixel_clk, hvtotal, \
					(UINT16)((gHDMI20RxHandler[port].real_pixel_clk * 1000) / hvtotal), vfreq);
		}

		/* Step 5-1. Convert Vfreq to a scale that the MW & DE FW wants(Multiply by 10) */
		vfreq = vfreq * 10;

		/* Step 6. Update VSI info to Timing Info operation..
		 * Weird that VSI information is sent through this path...*/
		/* Step 6-1. Check VSI info frame version */
		if((gHDMI20RxHandler[port].dataPacket[VSI][5] == 0x03) &&\
				(gHDMI20RxHandler[port].dataPacket[VSI][6] == 0x0C)&&\
				(gHDMI20RxHandler[port].dataPacket[VSI][7] == 0x00))
		{
			if((gHDMI20RxHandler[port].dataPacket[VSI][8] & 0xF0) == 0x40){
				vsi3DData = (0x0F & ((gHDMI20RxHandler[port].dataPacket[VSI][9] & 0xF0) >> 4));
			}


		}
		else{
			vsi3DData = LX_HDMI_VSI_3D_STRUCTURE_MAX;
		}

		/* Defence Code. Block operation on LINK TOP output zero sync */
		
		vsyncTime = gHDMI20RxHandler[port].ctrl.bsp_read(0x15c); vsyncTime &= 0x0000FFFF;
		hsyncTime = gHDMI20RxHandler[port].ctrl.bsp_read(0x148); hsyncTime &= 0x0000FFFF;
		if(vsi3DData == LX_HDMI_VSI_3D_STRUCTURE_FIELD_ALTERNATIVE){
			if((vsyncTime == 0) || (hsyncTime == 0) || (gHDMI20RxHandler[port].real_pixel_clk == 0)\
					||(gHDMI20RxHandler[port].timing_info.voffset == 0)){

				HDMI20_VIDEO("Error %d : Timing info cannot perform as to prevent zero division\n", port);
				HDMI20_VIDEO(" Value : VS[0x%x] HS[0x%x] PC[0x%x] VO[0x%x]\n", vsyncTime, hsyncTime, gHDMI20RxHandler[port].real_pixel_clk, gHDMI20RxHandler[port].timing_info.voffset);

				ret = copy_to_user((LX_HDMI_TIMING_INFO_T *)pData, &userdata, sizeof(LX_HDMI_TIMING_INFO_T));
				if(ret) {
					HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "TIMING" ,port);
				}
				break;
			}
		}

		/* Decide lge3DData with vsi3DData */
		switch(vsi3DData)
		{
			case LX_HDMI_VSI_3D_STRUCTURE_FRAME_PACKING:
				lg3DData = LX_HDMI_EXT_3D_FRAMEPACK;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_FIELD_ALTERNATIVE:
				lg3DData = LX_HDMI_EXT_3D_FIELD_ALTERNATIVE;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_LINE_ALTERNATIVE:
				lg3DData = LX_HDMI_EXT_3D_LINE_ALTERNATIVE;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_FULL:
				lg3DData = LX_HDMI_EXT_3D_SBSFULL;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_TOP_BOTTOM:
				lg3DData = LX_HDMI_EXT_3D_TNB;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_HALF:
				lg3DData = LX_HDMI_EXT_3D_SBS;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH:
				lg3DData = LX_HDMI_EXT_3D_L_DEPTH;
				break;
			case LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH_GRAPHICS:
				lg3DData = 	LX_HDMI_EXT_3D_L_GRAPHICS;
				break;
			default :
				if((gHDMI20RxHandler[port].dataPacket[VSI][8] & 0xF0) == 0x20){
					lg3DData = LX_HDMI_EXT_4K_2K;
				}
				else{
					lg3DData = LX_HDMI_EXT_2D_FORMAT;
				}
				break;
		}

		/* Step 7. Insert Timing Info to data structure*/
		//userdata.hFreq = (UINT16)hfreq;
		userdata.hFreq = 0;
		userdata.vFreq = (UINT16)vfreq; 					///< Veritical frequency(1/10 Hz unit)
		userdata.hTotal = (UINT16)gHDMI20RxHandler[port].timing_info.htotal; 			///< Horizontal total pixels
		userdata.vTotal = (UINT16)gHDMI20RxHandler[port].timing_info.vtotal; 			///< Vertical total lines
		userdata.hStart = (UINT16)gHDMI20RxHandler[port].timing_info.hoffset; 		///< Horizontal start pixel
		userdata.vStart = (UINT16)gHDMI20RxHandler[port].timing_info.voffset;			///< Vertical start lines
		userdata.hActive = (UINT16)gHDMI20RxHandler[port].timing_info.hactive;		///< Horizontal active pixel
		userdata.vActive = (UINT16)gHDMI20RxHandler[port].timing_info.vactive; 		///< Vertical active lines

		if((UINT16)gHDMI20RxHandler[port].timing_info.interlaced){
			userdata.scanType = 0;
		}
		else{
			userdata.scanType = 1;
		}

		//userdata.HdmiLock;				///< Sampling phase
		/* Update extInfo*/
		userdata.extInfo = lg3DData;
		/* Do not know what "STATE" does... */
		userdata.state = 0;

		/* Step 8. Check 3D video without VSI information */
		if(gVideoDebugLevel == 3401){
			HDMI20_VIDEO("DBG %d : switch(%d)/pixel_repetition(%d)\n", port, lg3DData, gHDMI20RxHandler[port].timing_info.pixel_repetition);
		}

		if(lg3DData == LX_HDMI_EXT_2D_FORMAT)
		{
			pTbl = &TBL_EXT_INFO[0];
			for (i = 0; i < TBL_NUM(TBL_EXT_INFO) ; i++, pTbl++)
			{
				if ((userdata.hActive == pTbl->hAct_info) && (userdata.vActive == pTbl->vAct_info) && (userdata.scanType== pTbl->scan_info) )
				{
					if(gVideoDebugLevel == 3401){
						HDMI20_VIDEO("DBG %d : 3D video without VSI info [%d]x[%d] [%d] => [%d]x[%d] [%d][%d]\n",\
								userdata.hActive, userdata.vActive, userdata.scanType, pTbl->hAct_info, pTbl->vAct_buf,\
								pTbl->scan_buf, pTbl->extInfo_buf);
					}

					userdata.hActive 		= pTbl->hAct_buf;
					userdata.vActive 		= pTbl->vAct_buf;
					userdata.scanType	= pTbl->scan_buf;
					userdata.extInfo		= pTbl->extInfo_buf;

					lg3DData = userdata.extInfo;
					device3DvideoWithoutVSI = 1;
					break;
				}
			}

			/* Step 8-1. If 3D without VSI condition */
			if(device3DvideoWithoutVSI >0){
				ret = copy_to_user((LX_HDMI_TIMING_INFO_T *)pData, &userdata, sizeof(LX_HDMI_TIMING_INFO_T));
				if(ret) {
					HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "TIMING" ,port);
					break;
				}
			}
		} 



		if((lg3DData == LX_HDMI_EXT_4K_2K)||(lg3DData == LX_HDMI_EXT_2D_FORMAT)){
			/* SW WA for 3G 4K video DisplayEnable signal bug */
			if(gVideoDebugLevel == 3840){
				HDMI20_VIDEO(" SW WA DBG :phy[%d]  0x400[0x%08x] %dx%d@%d\n", gHDMI20RxHandler[port].phyInitiatedMode, gHDMI20RxHandler[port].ctrl.bsp_read(0x400), userdata.hActive, userdata.vActive, userdata.vFreq);
			}
		
			phyInitiatedMode = __HDMI20_HAL_H15Ax_IsSCDCvalueUD(port);

			if((userdata.vActive >= 2160) &&(userdata.hActive >=3840)){
				if((phyInitiatedMode > 0)&&(gHDMI20RxHandler[port].timing_info.video_format == 0x3)){
					// Do not SW WA for U+ 4K 60 4:2:0
					if((userdata.vFreq == 500)||(userdata.vFreq == 600)){
						if(gVideoDebugLevel == 454) HDMI20_VIDEO(" Notice %d : 50Hz or 60Hz detected [%d]. HTotal[%d]\n", port, userdata.vFreq, userdata.hTotal);

						if((userdata.hTotal >= (5280 - gVideoManualSyncHtotalCheckThres)) && (userdata.hTotal <= (5280 + gVideoManualSyncHtotalCheckThres))){
							userdata.vFreq = 500;	
						}
						else if((userdata.hTotal >= (4400 - gVideoManualSyncHtotalCheckThres)) && (userdata.hTotal <= (4400 + gVideoManualSyncHtotalCheckThres))){
							userdata.vFreq = 600;	
						}

						if(gVideoDebugLevel == 454) HDMI20_VIDEO(" Notice %d : Result vfreq[%d] / Htotal[%d]\n", port, userdata.vFreq, userdata.hTotal);
					}
				}
				else{ 
					if(gVideoCheckHtotalForManualSync > 0){
						if((userdata.vFreq == 240)||(userdata.vFreq == 250)){
							if(gVideoDebugLevel == 454) HDMI20_VIDEO(" Notice %d : 24Hz or 25Hz detected on 4K manual sync [%d]. HTotal[%d]\n", port, userdata.vFreq, userdata.hTotal);

							if((userdata.hTotal >= (5280 - gVideoManualSyncHtotalCheckThres)) && (userdata.hTotal <= (5280 + gVideoManualSyncHtotalCheckThres))){
								userdata.vFreq = 250;	
							}
							else if((userdata.hTotal >= (5500 - gVideoManualSyncHtotalCheckThres)) && (userdata.hTotal <= (5500 + gVideoManualSyncHtotalCheckThres))){
								userdata.vFreq = 240;	
							}
							else if((userdata.hTotal >= (4400 - gVideoManualSyncHtotalCheckThres)) && (userdata.hTotal <= (4400 + gVideoManualSyncHtotalCheckThres))){
								userdata.vFreq = 300;	
							}

							if(gVideoDebugLevel == 454) HDMI20_VIDEO(" Notice %d : Result vfreq[%d] / Htotal[%d]\n", port, userdata.vFreq, userdata.hTotal);
						}
					}

					if((gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0)&&(gVideoEnableManualSync > 0)&&(gHDMI20RxHandler[port].ctrl.isMute == 0)){
					

						for(i=0;i<HDMI20_MANUAL_2D_4K_SYNC_PARAM_MAXNUM;i++){
							if((userdata.vActive == g2DManualSyncLUT[i].vactive)&&(userdata.hActive == g2DManualSyncLUT[i].hactive)\
									&&(userdata.vFreq == g2DManualSyncLUT[i].vfreq)&&(phyInitiatedMode == g2DManualSyncLUT[i].phyInitiatedMode)){
								HDMI20_VIDEO(" DBG %d : Software Workaround for 4K signal\n", port);
								HDMI20_VIDEO(" DBG %d : SW WA for 4K signal %dx%d@%d 3G/6G=%d. Color[0x%x]\n", port, userdata.hActive, userdata.vActive, userdata.vFreq, g2DManualSyncLUT[i].phyInitiatedMode\
										, gHDMI20RxHandler[port].timing_info.video_format);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x408, g2DManualSyncLUT[i].hBlankHactive);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, g2DManualSyncLUT[i].hFrontHsync);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x414, g2DManualSyncLUT[i].vFrontVsync);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x410, g2DManualSyncLUT[i].vBlankVactive);
								//gHDMI20RxHandler[port].isManualSyncMode = 2;

								gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0xF);

								__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(port, 0, 0, __L__);
								__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
								
								break;
							}
							else if(i == (HDMI20_MANUAL_2D_4K_SYNC_PARAM_MAXNUM -1)){
								if(gVideoDebugLevel == 6555) HDMI20_VIDEO(" WARN %d : Could not find table for 4K %dx%d@%d 3G/6G=%d\n", port,userdata.hActive,  userdata.vActive, userdata.vFreq, phyInitiatedMode);
							}
						}
					}
				}
			}
		}
		else
		{
			/* Step 9. Need to modifiy Video sync params for MW, because some values must be divided by 2 for 3D videos and pixel Repetition*/
			switch(lg3DData)
			{
				case LX_HDMI_EXT_3D_FRAMEPACK:
					/* From Old issues from HDMI1.4
					 * Russia  STB Issue :VSI Info is 3D F/P and Source is 2D*/
					if ( (userdata.hActive == 640 && userdata.vActive == 480) \
							|| (userdata.hActive == 720 && userdata.vActive == 480) \
							|| (userdata.hActive == 720 && userdata.vActive == 576) \
							|| (userdata.hActive == 1280 && userdata.vActive == 720) \
							|| (userdata.hActive == 1920 && userdata.vActive == 1080) )
					{
						userdata.extInfo = LX_HDMI_EXT_2D_FORMAT;
						if(gVideoDebugLevel == 4224) HDMI20_VIDEO("Notice %d : 3D VSI info is F/P, but Timing Info is 2D format for Russia STB Issue \n",  port);
					}
					else if ( (userdata.vActive == 2228) ||(userdata.vActive == 1028) )		//1080i & 480i
					{
						/* To support Master Device.. Old methods came from HDMI1.4 the famous M.S.H */
						userdata.scanType = 0;
						userdata.vActive = (userdata.vActive - 68) >> 1;	//68 : 23+22+23
					}
					else if (userdata.vActive == 1226)		//576i
					{
						/* To support Master Device.. Old methods came from HDMI1.4 the famous M.S.H */
						userdata.scanType = 0;
						userdata.vActive = (userdata.vActive - 74) >> 1;	//74 : 25+24+25
					}
					else if (userdata.vActive == 1103)		// interace
					{
						/* To support Master Device.. Old methods came from HDMI1.4 the famous M.S.H */
						userdata.scanType = 0;
						userdata.vActive = userdata.vActive - 23;
					}
					else			//progressive
					{
						/* To support Master Device.. Old methods came from HDMI1.4 the famous M.S.H */
						userdata.vActive = (userdata.vActive - userdata.vTotal + userdata.vActive) >> 1;
					}
					break;
				case LX_HDMI_EXT_3D_FIELD_ALTERNATIVE:
					/* To support Master Device.. Old methods came from HDMI1.4 the famous M.S.H */
					userdata.scanType = 0;

					if (userdata.vActive == 1103)		fix_offset = 23; //1080! - 22.5
					else if (userdata.vActive == 601) 	fix_offset = 25; //576! - 24.5
					else if (userdata.vActive == 503)	fix_offset = 23; //480! - 22.5
					else 								fix_offset = 0;
					userdata.vActive = userdata.vActive - fix_offset;
					userdata.vFreq = userdata.vFreq >> 1;

					/* SW Workaround for LINK OUTPUT sync manual mode */
					#if SW_WA_HDMI20_FIELDALTERNATIVE_MANUAL_OUTPUT_SYNCMODE
					if(	(gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0)&&(gVideoEnableManualSync > 0) &&(gHDMI20RxHandler[port].ctrl.isMute == 0)){
						HDMI20_VIDEO(" DBG %d : Software Workaround for 3D FA signal\n", port);
						
						gHDMI20RxHandler[port].isManualSyncMode = 2;

						gHDMI20RxHandler[port].ctrl.bsp_write(0x17c, 0x00000004);

						for(i=0;i<HDMI20_MANUAL_3D_SYNC_PARAM_FA_MAXNUM;i++){
							if((userdata.vActive == g3D_FA_ManualSyncLUT[i].vactive)&&(userdata.hActive == g3D_FA_ManualSyncLUT[i].hactive)\
									&&(userdata.vFreq == (g3D_FA_ManualSyncLUT[i].vfreq>>1))&&(phyInitiatedMode == g3D_FA_ManualSyncLUT[i].phyInitiatedMode)){
								HDMI20_VIDEO(" DBG %d : SW WA for 3D FA signal %dx%d@%d 3G/6G=%d\n", port, userdata.hActive, userdata.vActive, userdata.vFreq, g3D_FA_ManualSyncLUT[i].phyInitiatedMode);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x408, g3D_FA_ManualSyncLUT[i].hBlankHactive);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, g3D_FA_ManualSyncLUT[i].hFrontHsync);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x414, g3D_FA_ManualSyncLUT[i].vFrontVsync);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x410, g3D_FA_ManualSyncLUT[i].vBlankVactive);


								gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x00000003);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0000003F);
								__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
						
								break;	
							}
							else if(i == (HDMI20_MANUAL_3D_SYNC_PARAM_FA_MAXNUM -1)){
								HDMI20_VIDEO(" WARN %d : Could not find table for 3D FA %dx%d@%d 3G/6G=%d\n", port,userdata.hActive,  userdata.vActive, (userdata.vFreq<<1), phyInitiatedMode);
							}
						}
					}
					#endif
					break;
				case LX_HDMI_EXT_3D_LINE_ALTERNATIVE:
					userdata.vTotal = userdata.vTotal >> 1;
					userdata.vActive = userdata.vActive >> 1;
					userdata.vStart = userdata.vStart >> 1;
					break;
				case LX_HDMI_EXT_3D_SBSFULL:
					userdata.hTotal = userdata.hTotal >> 1;
					userdata.hActive = userdata.hActive >> 1;
					userdata.hStart = userdata.hStart >> 1;
					break;
				case LX_HDMI_EXT_3D_TNB:
					break;
				case LX_HDMI_EXT_3D_SBS:
					break;
				case LX_HDMI_EXT_3D_L_DEPTH:
				case LX_HDMI_EXT_3D_L_GRAPHICS:
				case LX_HDMI_EXT_2D_FORMAT:
				default :
					break;
			}
		}

		/* Step 10. Reduce Horizontal into half on pixel repetition */
		if(gHDMI20RxHandler[port].timing_info.pixel_repetition > 0)
		{
			if(gVideoDebugLevel == 129){
				HDMI20_VIDEO("DBG %d : Pixel Repitition mode[%d] Htotal[%d] HActive[%d] HStart[%d]\n", port, gHDMI20RxHandler[port].timing_info.pixel_repetition,userdata.hTotal, userdata.hActive, userdata.hStart);
			}
			
			if(gHDMI20RxHandler[port].timing_info.video_format == 1)userdata.PixelRepeat = gHDMI20RxHandler[port].timing_info.pixel_repetition;

			userdata.hTotal = userdata.hTotal >> 1;
			userdata.hActive = userdata.hActive >> 1;
			userdata.hStart = userdata.hStart >> 1;

			if((gHDMI20RxHandler[port].timing_info.video_format != 1)&&(gVideoDebugLevel != 227)){	
				if(port == 0){
					HDMI20_REG_RD(0xC8600000, ctop_hdmi_clk_div);
					ctop_hdmi_clk_div |= 0x00000010;
					HDMI20_REG_WR(0xC8600000, ctop_hdmi_clk_div);
				}
				else if(port == 1){
					HDMI20_REG_RD(0xC8600004, ctop_hdmi_clk_div);
					ctop_hdmi_clk_div |= 0x10000000;
					HDMI20_REG_WR(0xC8600004, ctop_hdmi_clk_div);
				}
			}
		}
		else if(gHDMI20RxHandler[port].timing_info.pixel_repetition == 0)
		{
			if(gVideoDebugLevel == 129){
				HDMI20_VIDEO("DBG %d : Pixel Repitition mode[%d] Htotal[%d] HActive[%d] HStart[%d]\n", port, gHDMI20RxHandler[port].timing_info.pixel_repetition,userdata.hTotal, userdata.hActive, userdata.hStart);
			}

			if((gHDMI20RxHandler[port].timing_info.hactive >= (3840 - 20)) &&\
					(gHDMI20RxHandler[port].timing_info.vactive >= (2160 - 20)))
			{
				/* Do nothing */
			}
			else{
				if(port == 0){
					HDMI20_REG_RD(0xC8600000, ctop_hdmi_clk_div);
					ctop_hdmi_clk_div &= (~0x00000010);
					HDMI20_REG_WR(0xC8600000, ctop_hdmi_clk_div);
				}
				else if(port == 1){
					HDMI20_REG_RD(0xC8600004, ctop_hdmi_clk_div);
					ctop_hdmi_clk_div &= (~0x10000000);
					HDMI20_REG_WR(0xC8600004, ctop_hdmi_clk_div);
				}
			}
		}

		/* Step 11. Double Vfreq for Interlaced signals */
		if(userdata.scanType == 0){
			userdata.vFreq = userdata.vFreq * 2;
		}

		/* Step 11-1. SW WA for vfreq on VIC code : 50Hz */
		if((gHDMI20RxHandler[port].timing_info.video_mode >= 20)&&(gHDMI20RxHandler[port].timing_info.video_mode<=31)){
			if(userdata.vFreq != 500){
				if(gVideoDebugLevel == 848){
					HDMI20_VIDEO("Warning %d : VIC code[%d] frame rate is not [%d]. (VIC : 500)\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.vFreq);
				}
				if(userdata.vFreq >= 500){
					if(userdata.vFreq - 500 < 30){
						userdata.vFreq = 500;
						if((userdata.vActive >= 2160) &&(userdata.hActive >=3840)&&(gVideoEnableManualSync > 0)){
							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);
						}
					}
				}
				else{
					if(500 - userdata.vFreq < 30){
						userdata.vFreq = 500;
						if((userdata.vActive >= 2160) &&(userdata.hActive >=3840)&&(gVideoEnableManualSync > 0)){
							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);
						}
					}
				}
				if(userdata.vFreq != 500){
					if(gVideoDebugLevel == 848){
						HDMI20_VIDEO("Warning %d : VIC code[%d] frame rate and Vfreq is not same.[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.vFreq);
					}
				}
			}
		}

		/* Step 11-2. SW WA for vfreq on VIC code : 60Hz */
		if((gHDMI20RxHandler[port].timing_info.video_mode >= 1)&&(gHDMI20RxHandler[port].timing_info.video_mode<=16)){
			if(userdata.vFreq != 600){
				if(gVideoDebugLevel == 848){
					HDMI20_VIDEO("Warning %d : VIC code[%d] frame rate is not [%d]. (VIC : 600)\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.vFreq);
				}
				if(userdata.vFreq >= 600){
					if(userdata.vFreq - 600 < 30){
						userdata.vFreq = 600;
						if((userdata.vActive >= 2160) &&(userdata.hActive >=3840)&&(gVideoEnableManualSync > 0)){
							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);
						}
					}
				}
				else{
					if(600 - userdata.vFreq < 30){
						userdata.vFreq = 600;
						if((userdata.vActive >= 2160) &&(userdata.hActive >=3840)&&(gVideoEnableManualSync > 0)){
							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);
						}
					}
				}
				if(userdata.vFreq != 600){
					if(gVideoDebugLevel == 848){
						HDMI20_VIDEO("Warning %d : VIC code[%d] frame rate and Vfreq is not same.[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.vFreq);
					}
				}		
			}
		}

		/* SW WA for VIC 22,18, 7 : Manual Sync */
		if((lg3DData != LX_HDMI_EXT_3D_FIELD_ALTERNATIVE)&&(lg3DData != LX_HDMI_EXT_3D_SBSFULL)&&(gVideoEnableManualSync > 0)){
			if((gHDMI20RxHandler[port].timing_info.video_mode == 22)||(gHDMI20RxHandler[port].timing_info.video_mode == 18)|| (gHDMI20RxHandler[port].timing_info.video_mode == 7)\
					|| (gHDMI20RxHandler[port].timing_info.video_mode == 14)){
				if((gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0)&&(gVideoEnableManualSync > 0) &&(gHDMI20RxHandler[port].ctrl.isMute == 0)){
					if(gHDMI20RxHandler[port].timing_info.video_mode == 22) data1 = HDMI20_MANUAL_SYNC_VIC22;
					else if(gHDMI20RxHandler[port].timing_info.video_mode == 18) data1 = HDMI20_MANUAL_SYNC_VIC18;
					else if(gHDMI20RxHandler[port].timing_info.video_mode == 7) data1 = HDMI20_MANUAL_SYNC_VIC7;
					else if(gHDMI20RxHandler[port].timing_info.video_mode == 14) data1 = HDMI20_MANUAL_SYNC_VIC14;


					if((userdata.hActive == g2D_2K_ManualSyncLUT[data1].hactive)&&(userdata.vActive == g2D_2K_ManualSyncLUT[data1].vactive)){
						gHDMI20RxHandler[port].ctrl.bsp_write(0x408, g2D_2K_ManualSyncLUT[data1].hBlankHactive);
						gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, g2D_2K_ManualSyncLUT[data1].hFrontHsync);
						gHDMI20RxHandler[port].ctrl.bsp_write(0x414, g2D_2K_ManualSyncLUT[data1].vFrontVsync);
						gHDMI20RxHandler[port].ctrl.bsp_write(0x410, g2D_2K_ManualSyncLUT[data1].vBlankVactive);
						gHDMI20RxHandler[port].isManualSyncMode = 2;

						data = 0xF;
						if(gHDMI20RxHandler[port].timing_info.interlaced) data |= 0x00000030;

						data1 = 0;
						if((lg3DData != LX_HDMI_EXT_4K_2K)&&(lg3DData != LX_HDMI_EXT_2D_FORMAT)){
							data1 = vsi3DData; data1 = data1 << 1;
							data1 |= 0x00000001;
						}

						gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
						gHDMI20RxHandler[port].ctrl.bsp_write(0x404, data1);
						__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
					}

					if( gHDMI20RxHandler[port].ctrl.bsp_read(0x400) != 0x0){
						if(gHDMI20RxHandler[port].ctrl.bsp_read(0x404) != 0x0){
							HDMI20_VIDEO(" DBG %d : Software Workaround for 3D VIC[%d] signal. 3D[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, vsi3DData );
						}
						else{
							HDMI20_VIDEO(" DBG %d : Software Workaround for 2D VIC[%d] signal. 3D[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, vsi3DData );
						}
					}
					else{
						if(gVideoDebugLevel == 1147){
							HDMI20_VIDEO(" DBG %d : SW WA for VIC[%d] but hActive[%d]/vActive[%d] is not same as VIC\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.hActive, userdata.vActive);
						}
					}
				}
				else{
					if(gHDMI20RxHandler[port].ctrl.bsp_read(0x404) == 0x0){
						if((lg3DData != LX_HDMI_EXT_4K_2K)&&(lg3DData != LX_HDMI_EXT_2D_FORMAT)){
							HDMI20_VIDEO("Notice %d : Founded 3D after Manual Sync. 3D[%d]\n", port, vsi3DData);

							data1 = vsi3DData; data1 = data1 << 1;
							data1 |= 0x00000001;

							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, data1);
							__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
						}
					}
					else{
						if(((gHDMI20RxHandler[port].ctrl.bsp_read(0x404)>>1)& 0xF ) != vsi3DData){
							HDMI20_VIDEO("Notice %d : Founded wrong 3D after Manual Sync. 3D[0x%x]/Set[0x%x]\n", port, vsi3DData, gHDMI20RxHandler[port].ctrl.bsp_read(0x404));

							data1 = vsi3DData; data1 = data1 << 1;
							data1 |= 0x00000001;

							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, data1);
							__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
						}
					}
				}
			}
			else{
				currentTime = jiffies_to_msecs(jiffies);
				if(currentTime - gHDMI20RxHandler[port].updateTimingTime >= gVideo2DManualSyncWaitTime){
					if((gVideoEnableManualSync > 0) &&(gHDMI20RxHandler[port].ctrl.isMute == 0)&&(lg3DData == LX_HDMI_EXT_2D_FORMAT)&&(gVideoUseManualSyncOnFHD >0)){
						if((userdata.vActive < 2160) && (userdata.hActive < 3840)){

							if((gHDMI20RxHandler[port].ctrl.hBlankHactive != 0) &&(gHDMI20RxHandler[port].ctrl.vFrontVsync != 0) &&\
									(gHDMI20RxHandler[port].ctrl.vFrontVsync != 0) && (gHDMI20RxHandler[port].ctrl.vBlankVactive != 0)){
								if(gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0){
									data = 0xF;
									if(gHDMI20RxHandler[port].timing_info.interlaced){
										data |= 0x00000030;
									}
							
									HDMI20_VIDEO("Notice %d : 2D Manual sync mode. 3D Flag[%d] \n", port, lg3DData);

									gHDMI20RxHandler[port].ctrl.bsp_write(0x408, gHDMI20RxHandler[port].ctrl.hBlankHactive);
									gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, gHDMI20RxHandler[port].ctrl.hFrontHsync);
									gHDMI20RxHandler[port].ctrl.bsp_write(0x414, gHDMI20RxHandler[port].ctrl.vFrontVsync);
									gHDMI20RxHandler[port].ctrl.bsp_write(0x410, gHDMI20RxHandler[port].ctrl.vBlankVactive);

									gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);

									if(gVideoDebugLevel == 334){
										if(gHDMI20RxHandler[port].timing_info.interlaced){
											gHDMI20RxHandler[port].ctrl.bsp_write(0x158, 0x3);
										}
										else{
											gHDMI20RxHandler[port].ctrl.bsp_write(0x158, 0x2);
										}
									}
									__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
								}
							}
						}
					}
					else{
						if((gHDMI20RxHandler[port].timing_info.video_mode == 5)&&(gHDMI20RxHandler[port].ctrl.isMute == 0)&&(lg3DData == LX_HDMI_EXT_2D_FORMAT) ){
							if(gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0){
								HDMI20_VIDEO("Notice %d : 2D Manual sync mode for VIC5(Master 394). 3D Flag[%d] \n", port, lg3DData);
			
								data = 0x37;

								gHDMI20RxHandler[port].ctrl.bsp_write(0x408, 0x01180780);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, 0x0058002C);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x414, 0x00020005);
								gHDMI20RxHandler[port].ctrl.bsp_write(0x410, 0x0016021C);

								gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);

								__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
							}
						}
					}
				}
				else{
					HDMI20_VIDEO("DBG %d : Waiting hold time for manual sync entry\n", port);	
				}
			}
		}
		else if(lg3DData == LX_HDMI_EXT_3D_SBSFULL){
			if((gHDMI20RxHandler[port].timing_info.video_mode == 22)&& (gHDMI20RxHandler[port].ctrl.bsp_read(0x400) == 0x0) &&(gVideoEnableManualSync > 0) &&(gHDMI20RxHandler[port].ctrl.isMute == 0)){
				gHDMI20RxHandler[port].ctrl.bsp_write(0x408, 0x02400B40);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x40C, 0x003000FC);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x414, 0x00020003);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x410, 0x00180120);
				gHDMI20RxHandler[port].isManualSyncMode = 2;

				data = 0xF;
				if(gHDMI20RxHandler[port].timing_info.interlaced) data |= 0x00000030;

				data1 = 0x9;

				gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x404, data1);
				__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x158, 3);

				if( gHDMI20RxHandler[port].ctrl.bsp_read(0x400) != 0x0){
					if(gHDMI20RxHandler[port].ctrl.bsp_read(0x404) != 0x0){
						HDMI20_VIDEO(" DBG %d : Software Workaround for 3D VIC[%d] signal. 3D[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, vsi3DData );
					}
					else{
						HDMI20_VIDEO(" DBG %d : Software Workaround for 2D VIC[%d] signal. 3D[%d]\n", port, gHDMI20RxHandler[port].timing_info.video_mode, vsi3DData );
					}
				}
				else{
					if(gVideoDebugLevel == 1147){
						HDMI20_VIDEO(" DBG %d : SW WA for VIC[%d] but hActive[%d]/vActive[%d] is not same as VIC\n", port, gHDMI20RxHandler[port].timing_info.video_mode, userdata.hActive, userdata.vActive);
					}
				}
			}
		}

		/* Detect late 3D on 2D manual sync */
		if(((gHDMI20RxHandler[port].ctrl.bsp_read(0x400) != 0x0) && (gHDMI20RxHandler[port].ctrl.bsp_read(0x400) & 0x80000000) == 0x0) &&(gVideoUseManualSyncOnFHD >0)){
			if(((gHDMI20RxHandler[port].ctrl.bsp_read(0x404)>>1) & 0x0000000F ) != vsi3DData){
				if((userdata.vActive < 2160) && (userdata.hActive < 3840)){
					if((vsi3DData == 0)||(vsi3DData == 1)||(vsi3DData == 2)||(vsi3DData == 3)||(vsi3DData == 6)||(vsi3DData == 8)){
						if((vsi3DData != LX_HDMI_EXT_2D_FORMAT)&&(lg3DData != LX_HDMI_EXT_3D_FIELD_ALTERNATIVE)&&(lg3DData != LX_HDMI_EXT_3D_SBSFULL)&&(vsi3DData != LX_HDMI_EXT_3D_TNB)){
							HDMI20_VIDEO("Notice %d : Founded different 3D after 2D Manual Sync. vsi[%d] !=reg[%d]\n", port, vsi3DData, ((gHDMI20RxHandler[port].ctrl.bsp_read(0x404)>>1) & 0x0000000F));

							data1 = vsi3DData; data1 = data1 << 1;
							data1 |= 0x00000001;

							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, data1);
							__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
						}
						else if(vsi3DData == LX_HDMI_EXT_3D_TNB){
							HDMI20_VIDEO("Notice %d : Founded TNB 3D on 2D Manual Sync. Return to AutoSync. vsi[%d]\n", port, vsi3DData);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);
							gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);
							__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
						}
					}
				}
			}
		}

		/* SW WA */
		if((gHDMI20RxHandler[port].isManualSyncMode == 2)){
			if((userdata.hActive == 2880)&&(userdata.vActive == 600)){
				userdata.hActive = 720;
				userdata.vActive = 576;
			}
		}

		/* SW WA */
		if(( gHDMI20RxHandler[port].timing_info.video_mode == 22)&&(gHDMI20RxHandler[port].timing_info.pixel_repetition == 0)){
			userdata.vActive = 576; 
			userdata.hActive = 1440;
			if(lg3DData == LX_HDMI_EXT_3D_SBSFULL){
				userdata.hTotal = 3456;
			}
		}

		/* Step 12. Copy Data to user */
		if(gHDMI20RxHandler[port].ctrl.isMute != 0){
			/* Mute */
			if(gVideoDebugLevel == 230) {
				memset((void *)&userdata, 0, sizeof(LX_HDMI_TIMING_INFO_T));
			}
			else{
				__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(port, 1, 0, __L__);
			}

			//gHDMI20RxHandler[port].audioMuteEableCnt = 0;
			//gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
			__HDMI20_HAL_H15Ax_AudioReset(port);
		}
		else{
			if(gHDMI20RxHandler[port].audioMuteEableCnt == HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME){
				//HDMI20_AUDIO("Audio Enable!!!!!!!!!!!!\n");
			}else if (gHDMI20RxHandler[port].audioMuteEableCnt < HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME) {
				HDMI20_AUDIO("audio mute cnt(port:%d) : %d\n", port, gHDMI20RxHandler[port].audioMuteEableCnt);			
				gHDMI20RxHandler[port].audioMuteEableCnt++; 
			}
		}

		/* To block HDCP14 transition noise */
		if((gHDMI20RxHandler[port].ctrl.bsp_read(0x8FC) == 0x4000)&&(gHDMI20RxHandler[port].ctrl.bsp_read(0xFC) != 0x201)){
			if((gHDMI20RxHandler[port].ctrl.bsp_read(0xFC) == 0x101)||(gHDMI20RxHandler[port].ctrl.bsp_read(0xFC) == 0x301)){
				if(gHDMI20RxHandler[port].stablehdcp14time < gHDMI20stableHDCP14HoldTime)	{
					/* Mute */
					memset((void *)&userdata, 0, sizeof(LX_HDMI_TIMING_INFO_T));
					if(gVideoDebugLevel == 1133){
						HDMI20_INFO("DBG %d : MUTE due to TMDS unstable. [%d]\n", port, gHDMI20RxHandler[port].stablehdcp14time);
					}
				}
			}
		}

		if((userdata.vActive == 0) || (userdata.hActive == 0) || (userdata.vFreq == 0)){
			//gHDMI20RxHandler[port].audioMuteEableCnt = 0;
			//gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
			__HDMI20_HAL_H15Ax_AudioReset(port);
		}


		if((gVideoDCMUpdateOnVideoChange != 0)&&(gVideoDCMUpdateOnVideoChange != 2)){
			if(gHDMI20RxHandler[port].dcm_change != 0){
				HDMI20_VIDEO("Warning [%d] : Do not update timing info on DCM change\n", port);
				userdata.hActive = 0;
				userdata.vActive = 0;
				userdata.vFreq = 0;
			}
		}

		/* SW WA for audio Phy issue */
		if(gHDMI20RxHandler[port].check19201080Dcm == 0){
			for(i=0; i<	HDMI20_PHY_AUDIO_SW_TIMING_MAXNUM; i++){
				if((userdata.hActive == gAudioPhySWWAList[i].hactive) && (userdata.vActive == gAudioPhySWWAList[i].vactive)\
						&&(userdata.vFreq == gAudioPhySWWAList[i].vfreq)\
						&& (gHDMI20RxHandler[port].currentDCMMode == gAudioPhySWWAList[i].dcmmode) &&(gHDMI20RxHandler[port].timing_info.interlaced == gAudioPhySWWAList[i].isInterlaced)){
					
					valueOfPhyClk = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, 0x69, 0x9);
					valueOfPhyClk &= (0x00000600); valueOfPhyClk = valueOfPhyClk>>9; valueOfPhyClk &= 0x00000003;

					if(valueOfPhyClk == gAudioPhySWWAList[i].okPhyValue){
						HDMI20_VIDEO("SW W/A %d : Found OK Phy value for audio\n", port);
						gHDMI20RxHandler[port].check19201080Dcm = 1;
						gHDMI20RxHandler[port].is19201080Dcm = 0;
					}
					else if(valueOfPhyClk == gAudioPhySWWAList[i].ngPhyValue){
						HDMI20_VIDEO("SW W/A %d : Found NG Phy value for audio\n", port);
						gHDMI20RxHandler[port].check19201080Dcm = 1;
						gHDMI20RxHandler[port].is19201080Dcm = 1;
					}
					else{
						HDMI20_VIDEO("WARNING [%d] : SW WA warning.. Phy value is not in range. =>[0x%x]\n", port, valueOfPhyClk);
					}
				}
				else {
					if(gVideoDebugLevel == 1201){
						HDMI20_VIDEO("WARN %d : Cannot find [%d][%d][%d][%d][%d]\n", port, userdata.hActive, userdata.vActive, userdata.vFreq , gHDMI20RxHandler[port].currentDCMMode, gHDMI20RxHandler[port].timing_info.interlaced);
					}
				}
			}
		}

		if(gHDMI20RxHandler[port].ctrl.bsp_read(0x30) & 0x00000001){
			// PLL OK!	

		}
		else{
			userdata.hActive = 0;
			userdata.vActive = 0;
			userdata.vFreq = 0;
		}


		ret = copy_to_user((LX_HDMI_TIMING_INFO_T *)pData, &userdata, sizeof(LX_HDMI_TIMING_INFO_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "TIMING" ,port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_AVIInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_INFO_PACKET_T userdata;
	UINT32 video_format;
	UINT8 diff_data = 0;
	UINT8 origin_data;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_INFO_PACKET_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_INFO_PACKET_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_INFO_PACKET_T));

		/* Step 1-1. Preset */
		video_format = gHDMI20RxHandler[port].ctrl.bsp_read(0x3A4);
		video_format = (video_format & 0x60); 
		diff_data = (UINT8)video_format;

		if(gVideoDebugLevel == 1221){
			HDMI20_VIDEO("DBG %d : video_format[0x%x]diffdata[0x%x]\n", port, video_format, diff_data);
		}

		if(diff_data != 0){
			origin_data = gHDMI20RxHandler[port].dataPacket[AVI][5];
			origin_data &= (~0x60); 
			origin_data |= diff_data;

			gHDMI20RxHandler[port].dataPacket[AVI][5] = origin_data;

			if(gVideoDebugLevel == 1221){
				HDMI20_VIDEO("DBG %d : origin_data[0x%x], packet[5d][0x%x]\n", port, origin_data, gHDMI20RxHandler[port].dataPacket[AVI][5]);
			}		
		}

		/* Step 2. Copy data */
		userdata.InfoFrameType = 0x82;
		memcpy((void *)&userdata.header, (void *)&gHDMI20RxHandler[port].dataPacket[AVI][1], 2);
		memcpy((void *)&userdata.dataBytes, (void *)&gHDMI20RxHandler[port].dataPacket[AVI][4], MAX_DATA_PACKET_LENGTH - 4);

		/* Notice : WEBOS & Pioneer gets DVI/HDMI information through this 7th bytes of AVI info frame...
		 *          This is not a standard in the specification.
		 *          Only LGE way... Do not know the history of this.. But i think the one who first did this
		 *          was a lazy guy and did not want a new IOCTL command for this..*/
		if((UINT16)gHDMI20RxHandler[port].timing_info.dvi){
			userdata.dataBytes[7] &= 0xFE;
		}
		else{
			userdata.dataBytes[7] |= 0x01;
		}




		ret = copy_to_user((LX_HDMI_INFO_PACKET_T *)pData, &userdata, sizeof(LX_HDMI_INFO_PACKET_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", sPacketName[AVI], port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_SPDInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_INFO_PACKET_T userdata;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_INFO_PACKET_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_INFO_PACKET_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_INFO_PACKET_T));

		/* Step 2. Copy data */
		userdata.InfoFrameType = 0x83;
		memcpy((void *)&userdata.header, (void *)&gHDMI20RxHandler[port].dataPacket[SPD][1], 2);
		memcpy((void *)&userdata.dataBytes, (void *)&gHDMI20RxHandler[port].dataPacket[SPD][4], MAX_DATA_PACKET_LENGTH - 4);

		ret = copy_to_user((LX_HDMI_INFO_PACKET_T *)pData, &userdata, sizeof(LX_HDMI_INFO_PACKET_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", sPacketName[SPD], port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_VSIInfo(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_INFO_PACKET_T userdata;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_INFO_PACKET_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_INFO_PACKET_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_INFO_PACKET_T));

		/* Step 2. Copy data */
		userdata.InfoFrameType = 0x81;
		memcpy((void *)&userdata.header, (void *)&gHDMI20RxHandler[port].dataPacket[VSI][1], 2);
		memcpy((void *)&userdata.dataBytes, (void *)&gHDMI20RxHandler[port].dataPacket[VSI][4], MAX_DATA_PACKET_LENGTH - 4);

		ret = copy_to_user((LX_HDMI_INFO_PACKET_T *)pData, &userdata, sizeof(LX_HDMI_INFO_PACKET_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", sPacketName[VSI], port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_COLORInfo(UINT8 port, LX_HDMI_COLOR_DOMAIN_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_COLOR_DOMAIN_T data;
	UINT32 video_format;

	do{
		/* Not used */
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_COLOR_DOMAIN_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_COLOR_DOMAIN_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Reset memory */
		memset((void *)&data , 0 , sizeof(LX_HDMI_COLOR_DOMAIN_T));

		if((UINT16)gHDMI20RxHandler[port].timing_info.dvi){
			data.bHdmiMode = 0;
		}
		else{
			data.bHdmiMode = 1;
		}
		
		video_format = gHDMI20RxHandler[port].ctrl.bsp_read(0x3A4);
		video_format = (video_format & 0x60); video_format = video_format>>5;
		video_format = video_format & 0x3;

		if(video_format == 0){
			data.ePixelEncoding = (UINT16)((gHDMI20RxHandler[port].dataPacket[AVI][5]&0x60)>>5);
		}
		else{
			data.ePixelEncoding = (UINT16)(video_format);
		}

		data.eColorimetry = (UINT16)gHDMI20RxHandler[port].timing_info.colorimetry;
		data.eExtColorimetry = (UINT16)gHDMI20RxHandler[port].timing_info.ext_colorimetry;
		data.eITContent = (UINT16)gHDMI20RxHandler[port].timing_info.it_content;
		data.eRGBQuantizationRange = (UINT16)gHDMI20RxHandler[port].timing_info.rgb_quant_range;

		ret = copy_to_user((LX_HDMI_COLOR_DOMAIN_T *)pData, &data, sizeof(LX_HDMI_COLOR_DOMAIN_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", __F__, port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_SrcDeviceInfo(UINT8 port, LX_HDMI20_SRCINFO_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI20_SRCINFO_T data;


	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI20_SRCINFO_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI20_SRCINFO_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Reset memory */
		memset((void *)&data , 0 , sizeof(LX_HDMI20_SRCINFO_T));

		/* Fill in data */
		data.isHDMI20Device = gHDMI20RxHandler[port].isHDMI20Device;

		ret = copy_to_user((LX_HDMI20_SRCINFO_T *)pData, &data, sizeof(LX_HDMI20_SRCINFO_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", __F__, port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_AspectRatio(UINT8 port, LX_HDMI_ASPECTRATIO_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_ASPECTRATIO_T data;


	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_ASPECTRATIO_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_ASPECTRATIO_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		/* Reset memory */
		memset((void *)&data , 0 , sizeof(LX_HDMI_ASPECTRATIO_T));

		data.eAspectRatio = (UINT16)gHDMI20RxHandler[port].timing_info.picture_ratio;

		ret = copy_to_user((LX_HDMI_ASPECTRATIO_T *)pData, &data, sizeof(LX_HDMI_ASPECTRATIO_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", __F__, port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}


/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_AKSVData(UINT8 port, LX_HDMI_INFO_PACKET_T *pData, int size)
{
	int ret = RET_OK;

	do{
		/* Not Used */
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_PHYStatus(LX_HDMI_PHY_INFORM_T *pData, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_PHY_INFORM_T userdata;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_PHY_INFORM_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_PHY_INFORM_T), size);
			break;
		}


		/* Step 1. Initiate Data */
		memset((void *)&userdata, 0, sizeof(LX_HDMI_PHY_INFORM_T));

		/* Step 2. Insert data */
		userdata.hdmi_mode[0] = gHDMI20RxHandler[0].timing_info.dvi;		// < hdmi/dvi mode
		userdata.hdmi_mode[1] = gHDMI20RxHandler[1].timing_info.dvi;		// < hdmi/dvi mode

		/* SW W/A for webos Popup sign for SONY EXPERIA Z2*/
		if(gHDMI205VPopUpSigneHoldTime == 0){
			userdata.hdmi5v[0] = gHDMI20RxHandler[0].is5Vconnected;
			userdata.hdmi5v[1] = gHDMI20RxHandler[1].is5Vconnected;
		}
		else{
			if(gHDMI20RxHandler[0].connected5Vtime >= gHDMI205VPopUpSigneHoldTime){
				userdata.hdmi5v[0] = 1;		//	< HDMI 5V detection on each ports
			}
			else{
				userdata.hdmi5v[0] = 0;
			}

			if(gHDMI20RxHandler[1].connected5Vtime >= gHDMI205VPopUpSigneHoldTime){
				userdata.hdmi5v[1] = 1;		//	< HDMI 5V detection on each ports
			}
			else{
				userdata.hdmi5v[1] = 0;
			}
		}

	
		userdata.hpd_out[0] = gHDMI20RxHandler[0].isHPD;		//	< HPD out status on each ports
		userdata.hpd_out[1] = gHDMI20RxHandler[1].isHPD;		//	< HPD out status on each ports

		//int 	hpd_pol[4];		//	< HPD polarity on eash ports
		//int 	hpd_in[4];		//	< HPD in status on each ports
		//int		phy_pdb[4];		//  < PHY PDB inform on each ports
		//int		phy_enable[4];		//  < PHY Enable inform on each ports
		//int		phy_rstn[4];		//  < PHY Reset inform on each ports
		//int 	edid_rd_done[4];		//  < EDID RD Done inform on each ports
		//int 	scdt[4];				//  < SDCT inform on each ports
		//UINT32		hdcp_authed[4];		//  < HDCP Authentication Done inform on each ports
		//UINT32		hdcp_enc_en[4];		//  < HDCP Authentication Start inform on each ports
		//int		cd_sense;				// < MHL CD Sence Detection flag(only in port 3)
		//int		cbus_conn_done;				// < MHL CBUS Connection Done
		//int		cbus_warb_done;				// < MHL CBUS Warb Done
		//int		cbus_disconn;			// < MHL CBUS Disconnected
		//int		phy_sink_cbus_zdis;			// < MHL Phy Sink CBUS Zdis
		//int		cbus_st;					// < MHL CBUS Status
		//UINT32	cbus_00;					// < MHL CBUS Register 00 Status
		//UINT32	cbus_01;					// < MHL CBUS Register 01 Status
		//LX_HDMI_PHY_STATUS_T	phy_status;	// current phy status
		userdata.tmds_clock[0] = gHDMI20RxHandler[0].real_tmds_clk;				// Phy TMDS Clock
		userdata.tmds_clock[1] = gHDMI20RxHandler[1].real_tmds_clk;				// Phy TMDS Clock
		//UINT8	state_sink_rcp;				// MHL sink RCP State
		//UINT8	module_init;					// HDMI module initialize/uninitialize
		//UINT8	module_open;					// HDMI module open/close
		//UINT8	module_conn;					// HDMI module connect/disconnect
		//UINT8	hpd_enable;						// HPD out Enabled
		//int		rstn_edid[4];				// sw reset status of edid on each port
		//int		tcs_done[4];				// tcs_done status of each port


		/* Step 3. Copy data to user layer */
		ret = copy_to_user((LX_HDMI_PHY_INFORM_T *)pData, &userdata, sizeof(LX_HDMI_PHY_INFORM_T));
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [PHY STATUS] data.\n");
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Set_ResetTimingInfo(void)
{
	int ret = RET_OK;

	do{
		/* Not used */
	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Set_EDID(UINT8 port, UINT8 *pData, int size)
{
	int ret = RET_ERROR;
	UINT32 edidData[(HDMI20_HAL_SIZE_OF_EDID >> 2)];
	int i;
	UINT32 addr;
	UINT32 dbgData;

	do{
		HDMI20_INFO("NOTI %d : Set Internal EDID from MW\n", port);

		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != HDMI20_HAL_SIZE_OF_EDID){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", HDMI20_HAL_SIZE_OF_EDID, size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		memset((void *)&edidData, 0, HDMI20_HAL_SIZE_OF_EDID);

		if( copy_from_user(&edidData, (void __user *)pData, HDMI20_HAL_SIZE_OF_EDID)){
			HDMI20_ERROR("Cannot copy EDID data structure from user\n");
			break;
		}

		/* Store EDID data from above */
		memcpy((void *)&gHDMI20RxHandler[port].edidData, (void *)edidData, HDMI20_HAL_SIZE_OF_EDID);
		
		gHDMI20RxHandler[port].wasEdidWritten = true;
		
		addr = 	(UINT32)HDMI20_HAL_H15AX_RX_INTERNAL_EDID_OFFSET;
		
		for(i=0; i<(HDMI20_HAL_SIZE_OF_EDID >> 2); i++){
			if(port == 0){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(addr, edidData[i]);
				dbgData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(addr);
			}
			else if(port ==1){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(addr, edidData[i]);
				dbgData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(addr);
			}

			if(gVideoDebugLevel == 1016){
				HDMI20_INFO("Internal EDID[%d] : BYTE [%d] =  [0x%x]<=[0x%08x] (0x%08x)\n", port, i, addr, edidData[i],dbgData);
			}
			addr += 4;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_EDID(UINT8 port, UINT8 *pData, int size)
{
	int i;
	int ret = RET_ERROR;
	UINT32 edidData[(HDMI20_HAL_SIZE_OF_EDID >> 2)];
	UINT32 addr;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != HDMI20_HAL_SIZE_OF_EDID){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", HDMI20_HAL_SIZE_OF_EDID, size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		addr = 	(UINT32)HDMI20_HAL_H15AX_RX_INTERNAL_EDID_OFFSET;
		
		for(i=0; i<(HDMI20_HAL_SIZE_OF_EDID >>2); i++){
			if(port == 0){
				edidData[i] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(addr);
			}else if(port == 1){
				edidData[i] = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(addr);
			}

			if(gVideoDebugLevel == 1016){
				HDMI20_INFO("Internal EDID[%d] : BYTE [%d] -- [0x%x]<=[0x%08x]\n", port, i, addr, edidData[i]);
			}
			addr += 4;
		}

		ret = copy_to_user((UINT8 *)pData, (UINT8 *)&edidData, HDMI20_HAL_SIZE_OF_EDID);
		if(ret) {
			HDMI20_ERROR("Copy Error to USER space for [%s] data on Port[%d].\n", "Internal EDID", port);
			break;
		}

		ret = RET_OK;
	} while(0);

	return ret;
}


/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Set_HDCP14(UINT8 port, UINT32 *pData, int size)
{
	int ret = RET_ERROR;
	UINT32 data = 0;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(struct hdmi_rx_ctrl_hdcp)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_TIMING_INFO_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if( copy_from_user(&gHDMI20RxHandler[port].hdcp, (void __user *)pData, sizeof(struct hdmi_rx_ctrl_hdcp))){
			HDMI20_ERROR("Cannot copy HDCP1.4 data structure from user\n");
			break;
		}

		/* Turn off HDCP1.4 bypass mode (double check) */
		gHDMI20RxHandler[port].gWasHDCP14Written = 1;

		/* SET GPIO Mux to DDC line */
		HDMI20_REG_RD(0xC8600088, data);
		data &= (~0x000003C0);
		HDMI20_REG_WR(0xC8600088, data);

		gDDCMode[0] = 1;
		gDDCMode[1] = 1;


		HDMI20_INFO("Writing HDCP1.4 key to HDMI 2.0 Port[%d]\n", port);
		ret = RET_OK;
	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_HDCP14(UINT8 port, UINT32 *pData, int size)
{
	int ret = RET_ERROR;

	do{
		if(pData == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(struct hdmi_rx_ctrl_hdcp)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_TIMING_INFO_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		ret = copy_to_user((void __user *)pData, &gHDMI20RxHandler[port].hdcp, sizeof(struct hdmi_rx_ctrl_hdcp));
		if(ret) {
			HDMI20_ERROR("Error on attempting to copy HDCP1.4 data to USER for Port[%d]\n", port);
			break;
		}
		else{
			HDMI20_INFO("Success copy HDCP1.4 data to USER for Port[%d]\n", port);
			ret = RET_OK;
		}

		ret = RET_OK;

	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(UINT8 port ,UINT8 onOff, UINT8 force)
{
	int ret = RET_ERROR;
	LX_GPIO_VALUE_T gpioValue = LX_GPIO_VALUE_INVALID;

													
	if(force == 0){
		HDMI20_INFO("Enable HPD Request from KADP. Port[%d], Onoff[%d]\n", port, onOff);
	}
	else{
		HDMI20_INFO("Force HPD control. Port[%d], Onoff[%d]\n", port, onOff);
	}

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(gHPDInverseMode){
			if(onOff){
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
				gHDMI20RxHandler[port].isHPD = 1;

				gpioValue = LX_GPIO_VALUE_LOW;

				ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

				if(ret < 0){
					HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
				}
			}
			else{
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
				gHDMI20RxHandler[port].isHPD = 0;

				gpioValue = LX_GPIO_VALUE_HIGH;

				ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

				if(ret < 0){
					HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
				}
			}
		}
		else {
			if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
				if(onOff){
					hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
					gHDMI20RxHandler[port].isHPD = 1;

					gpioValue = LX_GPIO_VALUE_HIGH;

					ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
					ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

					if(ret < 0){
						HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
					}
				}
				else{
					hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
					gHDMI20RxHandler[port].isHPD = 0;

					gpioValue = LX_GPIO_VALUE_LOW;

					ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
					ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

					if(ret < 0){
						HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
					}
				}
			}
			else{
				if(onOff){
					if(port == 1){
						hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
						gHDMI20RxHandler[port].isHPD = 1;
					}
					else if(port == 0){
						hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
						gHDMI20RxHandler[port].isHPD = 1;
					}

					gHDMI20RxHandler[port].hpd_request = 0;
				}
				else{
					if(port == 1){
						hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
						gHDMI20RxHandler[port].isHPD = 0;
					}
					else if(port == 0){
						hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
						gHDMI20RxHandler[port].isHPD = 0;
					}

					gHDMI20RxHandler[port].hpd_request = 0;
				}
			}
		}
		//hdmi_rx_scdc_setHpdLow(&gHDMI20RxHandler[port].ctrl, onOff);

		ret = RET_OK;

	} while(0);

	return ret;
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Set_EnableEDID(UINT8 onOff)
{
	int ret = RET_OK;

	do{

	} while(0);

	return ret;

}


/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_GetAudioInfo(UINT8 port, LX_HDMI_AUDIO_INFO_T *pHdmiAudioInfo, int size)
{
	int ret = RET_ERROR;
	LX_HDMI_AUDIO_INFO_T	data;


	do{
		if(pHdmiAudioInfo == NULL){
			HDMI20_ERROR("Error! Null pointer recieved from USER!\n");
			break;
		}

		if(size != sizeof(LX_HDMI_AUDIO_INFO_T)){
			HDMI20_ERROR("Error! Abnormal data size!([%d] != [%d]) \n", sizeof(LX_HDMI_AUDIO_INFO_T), size);
			break;
		}

		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}
		
		/* Added 2014/08/04 taejun.lee */
		if((__HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(port) > 0) ||  gHDMI20RxHandler[port].audio_change == 1 || gHDMI20RxHandler[port].ctrl.isMute == 1 || gHDMI20RxHandler[port].audioMuteEableCnt < HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME || gAudioNoaudioCnt[port] > 0) {
			data.audioType  = LX_HDMI_AUDIO_NO_AUDIO;
		} else {	
			data.audioType  = _gH15AxHdmiAudioInfo[port].audioType;
			
		}
		data.samplingFreq =  _gH15AxHdmiAudioInfo[port].samplingFreq;
		//HDMI20_AUDIO("audiotype : %d, samplingFreq : %d\n", data.audioType, data.samplingFreq);

		ret = copy_to_user((LX_HDMI_AUDIO_INFO_T *)pHdmiAudioInfo, &data, sizeof(LX_HDMI_AUDIO_INFO_T));
		if(ret) {
			HDMI20_ERROR("Error on attempting to copy to USER for Port[%d] Synopsys LINK.\n", port);
			break;
		}

		ret = RET_OK;

	} while(0);

	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_Get_AudioFreqFromTMDSClock(UINT8 port, unsigned int *samplingFreq)
{
	int ret = RET_OK;

	UINT64		ui64TmdsClock = 0;			//< HDMI measured clock value of TMDS clock
	UINT64		ui64AcrN = 0;				//< HDMI ACR N value
	UINT64		ui64AcrCts = 0;				//< HDMI ACR CTS value
	UINT64		ui64TmdsSamplingFreq = 0;	//< HDMI sampling frequency in source device from TMDS clock

	//Get measured frequency value of TMDS clock.
	ui64TmdsClock = (UINT64)(gHDMI20RxHandler[port].real_tmds_clk) * 1000;
	//HDMI20_INFO( "TMDS CLK : %llu\n", ui64TmdsClock);

	//Get ACR N H/W value.
	ui64AcrN = gHDMI20RxHandler[port].ctrl.n_param;		//20 bits


	//Get ACR CTS H/W value.
	ui64AcrCts = gHDMI20RxHandler[port].ctrl.cts_param;	//20 bits
	//HDMI20_INFO("Get TDMS ACR  : ui64AcrN = %llu, ui64AcrCts = %llu\n", ui64AcrN, ui64AcrCts);

	//Compute a sampling frequency from TMDS clock
	ui64TmdsSamplingFreq = ui64AcrN * ui64TmdsClock;

	//Check divide by zero value.
	if ( (ui64TmdsSamplingFreq > 0) && (ui64AcrCts > 0) )
	{
		do_div(ui64TmdsSamplingFreq, ui64AcrCts * 128);
	}

	//HDMI20_INFO(" do_dvi = %llu\n", ui64TmdsSamplingFreq);

	//Mapping a sampling frequency from measuring from TMDS clock and ACR N & CTS H/W value
	if (ui64TmdsSamplingFreq == 0)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_NONE\n");
	}
	else if (ui64TmdsSamplingFreq < 22983)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_22_05KHZ;	//  22.05 kHz(not supported)
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_22_05KHZ(not supported)\n");
	}
	else if (ui64TmdsSamplingFreq < 30000)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_24_KHZ;	//  24 kHz(not supported)
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_24_KHZ(not supported)\n");
	}
	else if (ui64TmdsSamplingFreq < 33800)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_32_KHZ;	//  32 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_32KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 45965)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_44_1KHZ;	//  44.1 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_44_1KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 67000)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_48_KHZ;	//  48 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_48_KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 91935)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_88_2KHZ;	//  88.2 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_88_2KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 135000)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_96_KHZ;	//  96 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_96_KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 183870)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_176_4KHZ;	//  176.4 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_176_4KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 210000)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_192_KHZ;	//  192 kHz
		//HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_192_KHZ\n");
	}
	else
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_768_KHZ;	//  768 kHz(not supported)
		////HDMI20_INFO("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_768_KHZ(not supported)\n");
	}

	//HDMI_ATASK("Get Fs from TDMS clock : %llu => %d\n", ui64TmdsSamplingFreq, *samplingFreq);
	//HDMI20_INFO("Get Fs from TDMS clock : %llu => %d\n", ui64TmdsSamplingFreq, *samplingFreq);

	return ret;
}


/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_GetAudioCopyInfo(UINT8 port, LX_HDMI_AUDIO_COPY_T *pHdmiCopyInfo, int size)
{
	if(lx_chip_rev( ) >= LX_CHIP_REV(H15, B0)){

		int ret = RET_OK;

		UINT32	reg_achst_byte0, reg_achst_byte1;
		UINT32	CpBit, LBit;

		//Check a HDMI Link Number
		if (port >= HDMI_LINK_MAX)
		{
			HDMI20_ERROR("[%d] %s : port = %d is error!!!\n", __L__, __F__, port);
			return RET_ERROR;
		}

		//Get a hdmi audio copyright information.
		if ((UINT16)gHDMI20RxHandler[port].timing_info.dvi == 1)	//HDMI Mode
		{
			//Read  reg_achst_byte0 reg.
			if(port == 0){
				reg_achst_byte0 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x70);
			} else {
				reg_achst_byte0 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x70);
			}

			//Set a CpBit
			if (reg_achst_byte0 & HDMI_AUDIO_CP_BIT_MASK)
				CpBit = 1;
			else
				CpBit = 0;

			//Read  reg_achst_byte1reg.
			if(port == 0){
				reg_achst_byte1 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x74);
			} else {
				reg_achst_byte1 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x74);
			}

			//Set a LBit
			if (reg_achst_byte1 & HDMI_AUDIO_L_BIT_MASK)
				LBit = 1;
			else
				LBit = 0;

			//Set a Copyright Info. by CpBit and LBit
			if (CpBit == 0 && LBit == 0)
				*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_ONCE;
			else if (CpBit == 0 && LBit == 1)
				*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_NO_MORE;	//same with LX_HDMI_AUDIO_COPY_NEVER
			else
				*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_FREE;

		}
		else	//DVI Mode
		{
			*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_FREE;
		}

		HDMI20_AUDIO("HDMI_H15A0_GetAudioCopyInfo : port = %d, CopyInfo = %d\n", port, *pHdmiCopyInfo);
		return ret;
	}else {
		int ret = RET_OK;

		//Read	reg_achst_byte0 reg.
		if(port == 0){
			*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_ONCE;
		} else {
			*pHdmiCopyInfo = LX_HDMI_AUDIO_COPY_NO_MORE;
		}

		HDMI20_AUDIO("__HDMI20_HAL_H15Ax_Rx_GetAudioCopyInfo is not supported(under H15Bx)\n");
		do{

	} while(0);
		return ret;
	}
}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_SetHDMIARC(UINT8 port, BOOLEAN bOnOff)
{
	int ret = RET_OK;

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		gHDMI20RxHandler[port].isAudioARCMode = bOnOff;

		/* Added 2014/07/02 taejun.lee */
		if(port == 0){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x0C, (UINT32)bOnOff);
		}
		else if(port == 1){
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x0C, (UINT32)bOnOff);
		}

		HDMI20_AUDIO("SetHDMIARC : port(%d) ARCmode(%d)", port, gHDMI20RxHandler[port].isAudioARCMode);

	} while(0);

	return ret;

}

static int __HDMI20_HAL_H15Ax_Rx_SetMute(UINT8 port, LX_HDMI_MUTE_CTRL_T *pMuteCtrl, int size)
{
	int ret = RET_OK;

	HDMI20_AUDIO("__HDMI20_HAL_H15Ax_Rx_SetMute mutecnt(%d)\n", gHDMI20RxHandler[port].audioMuteEableCnt);

	do{	
		BOOLEAN 	videoMuteState, audioMuteState;
	
		//Check a HDMI Link Number
		if (port >= HDMI_LINK_MAX)
		{
			HDMI20_ERROR("[%d] %s : port = %d is error!!!\n", __L__, __F__, port);
			return RET_ERROR;
		}
	
		//OS_LockMutex(&g_HDMI_Sema);

#if 0
		//Bring video mute state from register
		LINK_NUM_H15_RdFL(link_num, system_control_00);
		LINK_NUM_H15_Rd01(link_num, system_control_00, reg_mute_vid, videoMuteState);
	
		//video related.
		if (pMuteCtrl->eMode == LX_HDMI_VIDEO_MUTE || pMuteCtrl->eMode == LX_HDMI_AV_MUTE)
		{
			if (pMuteCtrl->bVideoMute != videoMuteState)
			{
				LINK_NUM_H15_Wr01(link_num, system_control_00, reg_mute_vid, pMuteCtrl->bVideoMute);
				LINK_NUM_H15_WrFL(link_num, system_control_00);
	
				HDMI_VIDEO("[%d] %s : link_num = %d, bVideoMute = %s \n", __L__, __F__, link_num, (pMuteCtrl->bVideoMute ? "On" : "Off"));
			}
		}
#endif

		//Bring audio mute state from register
		audioMuteState = !(gHDMI20RxHandler[port].ctrl.bsp_read(0x260) & 0x00000001);

		//audio related.
		if (pMuteCtrl->eMode == LX_HDMI_AUDIO_MUTE || pMuteCtrl->eMode == LX_HDMI_AV_MUTE)
		{
			//Check a previous state
			if (pMuteCtrl->bAudioMute != audioMuteState)
			{
				if (pMuteCtrl->bAudioMute == TRUE)
				{
					//Mute audio data
					gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
					HDMI20_AUDIO("Mute enable!\n");
					gHDMI20RxHandler[port].isAudioMute = 1;
				}
				else
				{
					//Un-mute audio data
					if(gHDMI20RxHandler[port].audio_change == 0 && gHDMI20RxHandler[port].ctrl.isMute == 0 && gHDMI20RxHandler[port].audioMuteEableCnt == HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME || gAudioNoaudioCnt[port] == 0) {
						gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000001);// AUD_OUTPUT_MUTE_CTRL unmute
						HDMI20_AUDIO("Mute disable!\n");
						gHDMI20RxHandler[port].isAudioMute = 0;
					}
				}
	
				//For debug print
				HDMI20_AUDIO("[%d] %s : port = %d, bAudioMute = %s \n",	\
							__L__, __F__, port, (pMuteCtrl->bAudioMute ? "On" : "Off"));
			}
	
			//For debug print
			if (pMuteCtrl->bAudioMute == FALSE)
			{
				HDMI20_AUDIO("[%d]SetMute : port = %d, type = %d, freq = %d, mute = %d\n", \
							__L__, port, _gH15AxHdmiAudioInfo[port].audioType, _gH15AxHdmiAudioInfo[port].samplingFreq, \
							audioMuteState);
			}
		}
		//OS_UnlockMutex(&g_HDMI_Sema);
	} while(0);



	return ret;

}

/* IOCTL */
static int __HDMI20_HAL_H15Ax_Rx_GetDebugAudioInfo(UINT8 port, LX_HDMI_DEBUG_AUDIO_INFO_T *pDebugAudioInfo, int size)
{
	int ret = RET_OK;

	do{

	} while(0);

	return ret;

}

static void __HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(UINT8 port, int isCablePlug)
{
	UINT32 previous_dcm = 0;	

	previous_dcm = gHDMI20RxHandler[port].ctrl.previous_video.deep_color_mode;
	/* Initiate handler's video data */
	memset((void *)&gHDMI20RxHandler[port].ctrl.previous_video, 0 , sizeof(struct hdmi_rx_ctrl_video));
	gHDMI20RxHandler[port].ctrl.previous_video.deep_color_mode = previous_dcm;

	/* Due to the fact that, the TMDS_CLK will only be updated when there is a ISR..
	 * Dangerous to clear the TMDS_CLK info */
	if(gVideoDebugLevel == 1107){	
		if(isCablePlug){
			gHDMI20RxHandler[port].ctrl.tmds_clk = 0;
			gHDMI20RxHandler[port].ctrl.tmds_clock_old = 0;
		}
	}

	gHDMI20RxHandler[port].real_tmds_clk = 0;
	gHDMI20RxHandler[port].real_pixel_clk = 0;

	//gHDMI20RxHandler[port].isManualSyncMode = 0;
	//gHDMI20RxHandler[port].phyLostCnt = 0;
	/* Initiate handlers' audio data */
	memset((void *)&gHDMI20RxHandler[port].audio, 0, sizeof(struct hdmi_rx_ctrl_audio));

	/* Reset Data Packet Buffer */
	HDMI20_H15A0_ResetDataPacketBuffer(hdmi_rx_ctrl_packet_cnt, port);

	__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[port].ctrl, 1);
}

/* DBG function */
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintSavedEDID(UINT8 port)
{
	int i = 0;

	UINT8 edid_data[HDMI20_HAL_SIZE_OF_EDID] = {0,};

	memcpy((void *)&edid_data, (void *)gHDMI20RxHandler[port].edidData, HDMI20_HAL_SIZE_OF_EDID);

	printk("=========================================================================================================================\n");
	printk("[0x00] |  [0x00] [0x01] [0x02] [0x03] [0x04] [0x05] [0x06] [0x07] [0x08] [0x09] [0x0A] [0x0B] [0x0C] [0x0D] [0x0E] [0x0F]\n");
	printk("=========================================================================================================================\n");

	for (i = 0; i < 16; i++) {
		if((i==0)||(i==8)){
			printk("-------------------------------------------------------------------------------------------------------------------------\n");
			printk("EDID Block %d, Bytes %d-%d [%02xH-%02xH]\n",  i/8, (128*(i/8)), (128*((i/8)+1) -1), (128*(i/8)), (128*((i/8)+1) -1));
			printk("-------------------------------------------------------------------------------------------------------------------------\n");
		}
		printk("[0x%02x] | [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x] [0x%02x]\n",\
				i, edid_data[(i*16) + 0], edid_data[(i*16) + 1], edid_data[(i*16) + 2], edid_data[(i*16) + 3], edid_data[(i*16) + 4],\
				edid_data[(i*16) + 5], edid_data[(i*16) + 6], edid_data[(i*16) + 7], edid_data[(i*16) + 8],\
				edid_data[(i*16) + 9], edid_data[(i*16) + 10],edid_data[(i*16) + 11], edid_data[(i*16) + 12],\
				edid_data[(i*16) + 13], edid_data[(i*16) + 14],edid_data[(i*16) + 15]);
	}
}

/* DBG function */
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintHandler(HDMI20_HAL_RX_DEVICE_DATA *handler)
{
	int scdc_value;

	if(handler == 0) {
		HDMI20_ERROR("NULL pointer \n");
		return;
	}

	if(handler->initiated == 0){
		HDMI20_ERROR("Device handler was not initiated.\n");
		return;
	}

	scdc_value = gHDMI20RxHandler[handler->ctrl.device_id].ctrl.bsp_read(0x820);
	scdc_value &= 0x00020000;

	HDMI20_INFO("HDMI 2.0 Port [%d] GPIO[%d], On-Phy-Initiating[%d] , DIL_HPD_ENABLE[%d]: \n", handler->ctrl.device_id, handler->port5vGPIOnum ,handler->phy_rst_request, gNumOfRunningHDMI);
	HDMI20_INFO("HDMI DCM Mode : [0x%08x], SRC Device HDMI2.0 Capability : [%d] , ctrl.isMute[%d]\n", handler->currentDCMMode, handler->isHDMI20Device, handler->ctrl.isMute);
	HDMI20_INFO("HDMI I2C flag : Done[%d], NACK[%d]\n", handler->i2cDoneSignal, handler->i2cNackSignal);
	HDMI20_INFO("SW W/A Mapped Addr : [0xC8000140]=>[0x%08x],[0xC8000180]=>[0x%08x],[0xC80001C0]=>[0x%08x]\n", pHDMI_MUX_CTRL_B, pHDMI_MUX_CTRL_C, pHDMI_MUX_CTRL_D);
	HDMI20_INFO(" Got IPC address of [0x%08x]\n", handler->ipcmem.hwAddr);
	HDMI20_INFO("5V Status : [%d], HPD[%d] ,Video Status : [%d], locking time[%d], connect Requst[%d]\n", handler->is5Vconnected, handler->isHPD, handler->ctrl.stable_cnt, (handler->locking_time * gMainThreadMsleep), handler->connect_request);
	HDMI20_INFO(" PCB GPIO [%d]. HPD Mode [%d]\n",gGpio15Value,gHPDInverseMode);
	HDMI20_INFO("SCDC Status : [%d], Phy Locked scdc Value[%d]\n", scdc_value, handler->phyInitiatedMode);
	HDMI20_INFO("  TOP Mem : PhyAddr[0x%08x] / SWAddr[0x%08x] / size[%d]\n", \
			(unsigned int)handler->topmem.hwAddr, (unsigned int)handler->topmem.pSwAddr, handler->topmem.size);
	HDMI20_INFO("  Link Mem : PhyAddr[0x%08x] / SWAddr[0x%08x] / size[%d]\n", \
			(unsigned int)handler->linkmem.hwAddr, (unsigned int)handler->linkmem.pSwAddr, handler->linkmem.size);
	HDMI20_INFO("   PhyLocked[%d] Amount of Port[%d] / Link IRQ[%d] / 5V IRQ[%d]\n",\
			handler->isPhyLocked, handler->port, handler->irq, handler->top_irq);
	HDMI20_VIDEO(" Change Flag : scdc[%d]/video[%d]/audio[%d]/tmds[%d]/hpdRequest[%d]\n", \
			handler->scdc_change, handler->video_change,\
			handler->audio_change, handler->tmds_change, handler->hpd_request);
	HDMI20_INFO("  Link Ctrl Handler 5VholdonTime[%d]:\n", (unsigned int)handler->connected5Vtime);
	HDMI20_INFO("  cfg_clk[%d] / md_clk[%d] / tmds_clk[%d], WasInternalEdidWritten[%d]\n",\
			(unsigned int)handler->ctrl.cfg_clk, (unsigned int)handler->ctrl.md_clk, (unsigned int)handler->ctrl.tmds_clk, (unsigned int)handler->wasEdidWritten);
	HDMI20_INFO("  Audio Fifo reset CNT[%d] / Packet Fifo reset CNT[%d] / IRQ Handling CNT[%d]\n", \
			(unsigned int)handler->ctrl.debug_audio_fifo_rst, (unsigned int)handler->ctrl.debug_packet_fifo_rst, (unsigned int)handler->ctrl.debug_irq_handling);
	HDMI20_INFO("  Packet Decoder CNT[%d] / Audio Clock CNT[%d] / Audio Fifo CNT[%d]\n", \
			handler->ctrl.debug_irq_packet_decoder, handler->ctrl.debug_irq_audio_clock, handler->ctrl.debug_irq_audio_fifo);
	HDMI20_INFO("  Video IRQ CNT[%d] / HDMI IRQ CNT[%d]\n", handler->ctrl.debug_irq_video_mode, handler->ctrl.debug_irq_hdmi);
	HDMI20_INFO("  AUDIO N PARAM[%d], AUDIO CTS PARAM[%d], gUseHDMI20Tx[%d]\n", handler->ctrl.n_param, handler->ctrl.cts_param, gUseHDMI20Tx);
	HDMI20_INFO("  gCntOfResume[%d] gMainThreadStamp[%d], TX Driver initiate at pll lock : %d \n", gCntOfResume,gMainThreadStamp,gWasTXInitiated);
	HDMI20_INFO(" Phy Alter mode : [%d], REAL TMDS[%d], REAL PIXEL[%d]\n", handler->isPhyAlterMode, handler->real_tmds_clk, handler->real_pixel_clk);
	HDMI20_INFO("  hBlank/hActive => [0x%08x], hFront/hSync=>[0x%08x], vFront/vSync=>[0x%08x], vBlank/vActive=>[0x%08x]n",\
			handler->sync_info.hBlankHactive, handler->sync_info.hFrontHsync, handler->sync_info.vFrontVsync, handler->sync_info.vBlankVactive);
	HDMI20_INFO(" check19201080Dcm[%d], is19201080Dcm[%d]. IS_SCDC_UD[%d]\n", handler->check19201080Dcm, handler->is19201080Dcm, handler->isScdcUD); 
	HDMI20_INFO(" IsAudioARCMode[%d], Lost Phy Lock[%d], Local Mute[%d]\n", handler->isAudioARCMode, handler->phyLostCnt, __HDMI20_HAL_H15Ax_Rx_IsLocalMuteOn(handler->ctrl.device_id));
	HDMI20_INFO("isHDCP22 = %d, isHDCP22Ncap = %d, isHDCP14 = %d\n", handler->isHDCP22, handler->isHDCP22Ncap, handler->isHDCP14);
	HDMI20_INFO(" isGalaxyTab = [%d]\n", gAudioIsGalaxyTab10[handler->ctrl.device_id]); 
}


/* DBG function */
static void __HDMI20_HAL_H15Ax_Rx_DBG_PrintVideo(HDMI20_HAL_RX_DEVICE_DATA *handler)
{
	int ret;
	int vfreq = 0;
	int hfreq = 0;
	struct hdmi_rx_ctrl_video v = {0};
	struct hdmi_rx_ctrl_audio a = {0};

	if(handler == 0) {
		HDMI20_ERROR("NULL pointer \n");
		return;
	}

	if(handler->initiated == 0){
		HDMI20_ERROR("Device handler was not initiated.\n");
		return;
	}

	ret = hdmi_rx_ctrl_get_video(&handler->ctrl, &v, 3);
	if(ret < 0) {
		if( ret != -ERANGE) {
			HDMI20_ERROR("Error when attempting to achieve video data[%d]\n",  ret);
			return;
		}
		else{
			HDMI20_ERROR("Bad video or no signal\n");
		}
	}

	ret = hdmi_rx_ctrl_get_audio(&handler->ctrl, &a);
	if( ret < 0) {
		HDMI20_AUDIO("Error when attemting to achieve AUDIO data[%d]\n", ret);
		return;
	}


	if((v.pixel_clk == 0)||(v.vtotal == 0)||(v.htotal == 0)){
		vfreq = 0;
	}
	else{
		vfreq = (v.pixel_clk * 1000)/(v.vtotal * v.htotal);
	}


	if((v.pixel_clk == 0)||(v.htotal == 0)){
		hfreq = 0;
	}
	else{
		hfreq = ((v.pixel_clk * 1000) / v.htotal); 					///< Horizontal frequency(100 Hz unit)
	}

	HDMI20_INFO("HDMI 2.0 Port [%d] VIDEO Status [%d] Timing Invalid[%d] ManualSync Mode[%d]\n", handler->ctrl.device_id, handler->ctrl.stable_cnt,handler->isTimingInvalid, handler->isManualSyncMode);
	/** DVI detection status: DVI (true) or HDMI (false) */
	HDMI20_INFO("  DVI(1) or HDMI(0)       => [%d]\n",v.dvi);
	/** Deep color mode: 24, 30, 36 or 48 [bits per pixel] */
	HDMI20_INFO("  Deep Color Mode         => [%d]\n",v.deep_color_mode);

	HDMI20_INFO("[Video Status]\n");
	/** Pixel clock frequency [kHz] */
	HDMI20_INFO("  Vertical Freq [Hz]      => [%d]\n",vfreq);
	HDMI20_INFO("  Horizontal Freq [Hz]    => [%d]\n",hfreq);
	HDMI20_INFO("  Pixel Clock Freq [kHz]  => [%d]\n",(unsigned int)v.pixel_clk);
	/** Refresh rate [0.01Hz] */
	HDMI20_INFO("  Refresh Rate [0.01Hz]   => [%d]\n",(unsigned int)v.refresh_rate);
	/** Interlaced */
	HDMI20_INFO("  Is Interlaced           => [%d]\n",v.interlaced);
	/** Vertical offset */
	HDMI20_INFO("  Vertical offset         => [%d]\n",v.voffset);
	/** Vertical active */
	HDMI20_INFO("  Vertical Active         => [%d]\n",v.vactive);
	/** Vertical total */
	HDMI20_INFO("  Vertical Total          => [%d]\n",v.vtotal);
	/** Horizontal offset */
	HDMI20_INFO("  Horizontal Offset       => [%d]\n",v.hoffset);
	/** Horizontal active */
	HDMI20_INFO("  Horizontal Active       => [%d]\n",v.hactive);
	/** Horizontal total */
	HDMI20_INFO("  Horizontal Total        => [%d]\n",v.htotal);
	HDMI20_INFO("[Audio Status]\n");
	HDMI20_INFO("  Coding Type             => [0x%08x]\n", a.coding_type);
	HDMI20_INFO("  Channel Count           => [0x%08x]\n", a.channel_count);
	HDMI20_INFO("  Sample Frequency        => [0x%08x]\n", a.sample_frequency);
	HDMI20_INFO("  Sample Size             => [0x%08x]\n", a.sample_size);
	HDMI20_INFO("  Coding Extension        => [0x%08x]\n", a.coding_extension);
	HDMI20_INFO("  Channel Allocation      => [0x%08x]\n", a.channel_allocation);
	HDMI20_INFO("  Down Mix Inhibit        => [0x%08x]\n", a.down_mix_inhibit);
	HDMI20_INFO("  Level Shift Value       => [0x%08x]\n", a.level_shift_value);

	HDMI20_INFO("[AVI InfoFrame Status]\n");
	/** AVI Y1-0, video format */
	HDMI20_INFO("  [Y1-0] Video format                      => [0x%08x]\n",v.video_format);
	/** AVI A0, active format information present */
	HDMI20_INFO("  [A0] Active Format Information present   => [0x%08x]\n ",v.active_valid);
	/** AVI B1-0, bar valid information */
	HDMI20_INFO("  [B1-0] Bar Valid                         => [0x%08x]\n",v.bar_valid);
	/** AVI S1-0, scan information */
	HDMI20_INFO("  [S1-0] Scan Information                  => [0x%08x]\n ",v.scan_info);
	/** AVI C1-0, colorimetry information */
	HDMI20_INFO("  [C1-0] Colorimetry Information           => [0x%08x]\n ",v.colorimetry);
	/** AVI M1-0, picture aspect ratio */
	HDMI20_INFO("  [M1-0] Picture Aspect ratio              => [0x%08x]\n ",v.picture_ratio);
	/** AVI R3-0, active format aspect ratio */
	HDMI20_INFO("  [R3-0] Active format aspect ratio        => [0x%08x]\n ",v.active_ratio);
	/** AVI ITC, IT content */
	HDMI20_INFO("  [ITC] IT Content                         => [0x%08x]\n",v.it_content);
	/** AVI EC2-0, extended colorimetry */
	HDMI20_INFO("  [EC2-0] Extended Colorimetry             => [0x%08x]\n ",v.ext_colorimetry);
	/** AVI Q1-0, RGB quantization range */
	HDMI20_INFO("  [Q1-0] RGA Quantization Range            => [0x%08x]\n ",v.rgb_quant_range);
	/** AVI SC1-0, non-uniform scaling information */
	HDMI20_INFO("  [SC1-0] Non-uniform scaling information  => [0x%08x]\n ",v.n_uniform_scale);
	/** AVI VIC6-0, video mode identification code */
	HDMI20_INFO("  [VIC6-0] Video mode identification code  => [0x%08x]\n ",v.video_mode);
	/** AVI PR3-0, pixel repetition factor */
	HDMI20_INFO("  [PR3-0] Picel Repetition Factor          => [0x%08x]\n ",v.pixel_repetition);
	/** AVI, line number of end of top bar */
	HDMI20_INFO("  [AVI] Line number of end of top bar      => [0x%08x]\n",v.bar_end_top);
	/** AVI, line number of start of bottom bar */
	HDMI20_INFO("  [AVI] Line number of start of bottom bar => [0x%08x]\n ",v.bar_start_bottom);
	/** AVI, pixel number of end of left bar */
	HDMI20_INFO("  [AVI] Pixel number of end of left bar    => [0x%08x]\n",v.bar_end_left);
	/** AVI, pixel number of start of right bar */
	HDMI20_INFO("  [AVI] Pixel number of start of right bar => [0x%08x]\n ",v.bar_start_right);

}

static int __HDMI20_H15Ax_Spin_Init(void *handler)
{
	//spin_lock_init((spinlock_t *)handler);
	return 0;
}

static int __HDMI20_H15Ax_Spin_IRQ_Lock(void *handler, unsigned long *flags)
{
	//spin_lock_irqsave((spinlock_t *)handler, flags);
	return 0;
}

static int __HDMI20_H15Ax_Spin_IRQ_UnLock(void *handler, unsigned long flags)
{
	//spin_unlock_irqrestore((spinlock_t *)handler, flags);
	return 0;
}


static void __HDMI20_HAL_H15Ax_Tx_SetMode4K30Hz(void)
{
	HDMI20_REG_WR(0xC898C020, 0x08000000); // TXCLKDRV PDB
	HDMI20_REG_WR(0xC898C0A0, 0x000000F9); // TXCLKDRV PDB
	HDMI20_REG_WR(0xC86040AC, 0x56ACFA00); // RXCLKDRV PDB

	/////// HDMI20TX_4K30p_PHY ////////
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);

	// Power down
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x10014, 0x00000001);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x00000036);
	msleep(1);

	// PHY configuration
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x10014, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x00000036);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1C000, 0x00000026);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000006);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000040);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000010);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000015);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000003);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);

	//SNPS Recommanded
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000019); // Termination
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000009);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);  // Slopeboosting & Pre-emp
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000039);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x0000000E);  // Voltage Level Control
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000001);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x0000008C);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x0000001C);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000004);  // Clock Alignment
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(100);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000017); ; PLL BYPASS MODE
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000004);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	//msleep( 100);

	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000005);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	//__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	//msleep( 100);

	// Power up
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x0000002E);
	msleep(500);
	///////////////////////////////////


	/////// HDMI20TX_UD_30p_PG ////////
	HDMI20_REG_WR(0xc8604000, 0x50000000);

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1E410, 0x00000002); // HDCP 1.4 <-> HDCP 2.2
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4000, 0x00000070); // DVI:0x70 HDMI:0x18

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x0004, 0x00000012);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x0008, 0x00000111); // 2CH -> 1CH

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x14000, 0x00000033); // HDCP bypass
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x14004, 0x00000000); //

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2000, 0x00000006); // UD(2ch)                       --> FHD
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x203c, 0x08CA0898); // Vtotal = 2250, Htotal = 2200  --> 1125/2200 : 04650898
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2040, 0x087C0886); // Vs_srt = 2172, Vs_end = 2182  --> 1088/1093 : 04400445
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2044, 0x00040874); // Va_srt =    4, Va_end = 2164  -->    4/1084 : 0004043C
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2048, 0x002C0058); // Hs_srt =   44, Hs_end =   88  -->   44/  88 : 002C0058
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x204c, 0x00EC086C); // Ha_srt =  236, Ha_end = 2156  -->  236/2156 : 00EC086C
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2054, 0x00000100);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2100, 0x00000006);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x213c, 0x08CA0898);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2140, 0x087C0886);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2144, 0x00040874);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2148, 0x002C0058);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x214c, 0x00EC086C);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2154, 0x00000100);

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x00200020);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x01200120);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x00800080);
}


void static __HDMI20_HAL_H15Ax_Tx_SetMode4K60Hz(void)
{
	/////// HDMI20TX_UD_PHY /////////
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);

	// Power down
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x10014, 0x00000001);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x00000036);
	msleep(1);

	// PHY configuration
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x10014, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x00000036);
	msleep(1);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1C000, 0x00000026);
	msleep(50);

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000006);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x0000001A);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x0000007C);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000019);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000009);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000039);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x0000000E);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000001);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x0000004A);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000005);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000010);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000080);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000015);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000003);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x0000001C);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x00000000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x00000004);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC080, 0x00000071);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC084, 0x00000004);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC088, 0x000000BF);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC08C, 0x000000FF);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC098, 0x00000010);
	msleep(50);
	// Power up
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0xC000, 0x0000002E);
	msleep(50);
	///////////////////////////////////


	/////////// SCRAMBLING  ///////////
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f800, 0x00000054);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f804, 0x00000020);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f808, 0x00000003);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f810, 0x00000010);
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f800, 0x00000054);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f804, 0x00000010);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f808, 0x00000001);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1f810, 0x00000010);
	msleep(500);
	///////////////////////////////////

	///////// HDMI20TX_UD_PG //////////
	HDMI20_REG_WR(0xc8604000, 0x00000000);

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x1E410, 0x00000002); // HDCP 1.4 <-> HDCP 2.2
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4000, 0x00000018); // DVI:0x70 HDMI:0x18

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x0004, 0x00000012);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x0008, 0x00000111); // 2CH -> 1CH

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x14000, 0x00000033); // HDCP bypass
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x14004, 0x00000000); //

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2000, 0x00000006); // UD(2ch)                       --> FHD
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x203c, 0x08CA0898); // Vtotal = 2250, Htotal = 2200  --> 1125/2200 : 04650898
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2040, 0x087C0886); // Vs_srt = 2172, Vs_end = 2182  --> 1088/1093 : 04400445
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2044, 0x00040874); // Va_srt =    4, Va_end = 2164  -->    4/1084 : 0004043C
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2048, 0x002C0058); // Hs_srt =   44, Hs_end =   88  -->   44/  88 : 002C0058
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x204c, 0x00EC086C); // Ha_srt =  236, Ha_end = 2156  -->  236/2156 : 00EC086C

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2054, 0x00000100);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2100, 0x00000006);

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x213c, 0x08CA0898);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2140, 0x087C0886);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2144, 0x00040874);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2148, 0x002C0058);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x214c, 0x00EC086C);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2154, 0x00000100);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x00200020);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x01200120);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2034, 0x00800080);

	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x207c, 0x00000063);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2080, 0x00000063);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2054, 0x00000100);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x217c, 0x00000063);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2180, 0x00000063);
	__HDMI20_HAL_H15Ax_Tx_TOP_Write(0x2154, 0x00000100);

	///////////////////////////////////);
	msleep(500);

	/// HDMI20TX_UD_frame_composer ////
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4384, 0x00000011); // data scramble

	///////////// RGB444 //////////////
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4000, 0x000000F8); //  (FC_INVIDCONF     )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4004, 0x00000000);//  (FC_INHACTV0      )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4008, 0x0000000F);//  (FC_INHACTV1      )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x400C, 0x00000030);//  (FC_INHBLANK0     )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4010, 0x00000002);//  (FC_INHBLANK1     )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4014, 0x00000070);//  (FC_INVACTV0      )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4018, 0x00000008);//  (FC_INVACTV1      )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x401C, 0x0000005A);//  (FC_INVBLANK      )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4020, 0x000000B0);//  (FC_HSYNCINDELAY0 )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4024, 0x00000000);//  (FC_HSYNCINDELAY1 )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4028, 0x00000058);//  (FC_HSYNCINWIDTH0 )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x402C, 0x00000000);//  (FC_HSYNCINWIDTH1 )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4030, 0x00000008);//  (FC_VSYNCINDELAY  )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4034, 0x0000000A);//  (FC_VSYNCINWIDTH  )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4038, 0x00000060);//  (FC_INFREQ0       )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x403C, 0x000000EA);//  (FC_INFREQ1       )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4040, 0x00000000);//  (FC_INFREQ2       )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4044, 0x0000000C);//  (FC_CTRLDUR       )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4048, 0x00000020);//  (FC_EXCTRLDUR     )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x404C, 0x00000007);//  (FC_EXCTRLSPAC    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4050, 0x00000055);//  (FC_EXCTRLSPAC    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4054, 0x00000000);//  (FC_EXCTRLSPAC    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4058, 0x00000015);//  (FC_EXCTRLSPAC    )

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4064, 0x00000060);//  (FC_AVICONF0    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4068, 0x000000EB);//  (FC_AVICONF1    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x406C, 0x0000008B);//  (FC_AVICONF2    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x405C, 0x00000007);//  (FC_AVICONF3    )
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4070, 0x0000006B);//  (FC_AVIID       )
	////////////////////////////////////

	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x10008, 0x01);		// Tx reg(0x4002) - mc_swrstzeq, Main Controller S/W Register
	msleep(100);
	__HDMI20_HAL_H15Ax_Tx_Write_Link(0x4034, 0x0A);	// Tx reg(0x100D) - fc_vsyncinwidrh, Frame Composer Input Video Vsync Width REgister
}

static UINT32 __HDMI20_HAL_H15Ax_Tx_Read_Link(UINT32 addr)
{
	UINT32 ret;

	if(gHDMI20TxHandler.linkmem.pSwAddr != NULL){
		ret = (UINT32)(*((UINT32 *)((UINT32)gHDMI20TxHandler.linkmem.pSwAddr + (UINT32)addr)));
	}
	else{
		HDMI20_VIDEO("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
		ret = 0;
	}

	return ret;
}

static void __HDMI20_HAL_H15Ax_Tx_Write_Link(UINT32 addr, UINT32 data)
{
	UINT32* dest;

	if(gHDMI20TxHandler.linkmem.pSwAddr != NULL){
		dest = 	((UINT32 *)((UINT32)gHDMI20TxHandler.linkmem.pSwAddr + (UINT32)addr));
		*dest = data;
	}
	else{
		HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n",__F__, __L__);
	}
}

static UINT32 __HDMI20_HAL_H15Ax_Tx_TOP_Read(UINT32 addr)
{
	UINT32 ret;

	if(gHDMI20TxHandler.topmem.pSwAddr != NULL){
		ret = (UINT32)(*((UINT32 *)((UINT32)gHDMI20TxHandler.topmem.pSwAddr + (UINT32)addr)));
	}
	else{
		HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
		ret = 0;
	}

	return ret;
}

static void   __HDMI20_HAL_H15Ax_Tx_TOP_Write(UINT32 addr, UINT32 data)
{
	UINT32* dest;

	if(gHDMI20TxHandler.topmem.pSwAddr != NULL){
		dest = 	((UINT32 *)((UINT32)gHDMI20TxHandler.topmem.pSwAddr + (UINT32)addr));
		*dest = data;
	}
	else{
		HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n",__F__ , __L__);
	}
}

static int __HDMI20_HAL_H15Ax_WritePhy(struct hdmi_rx_ctrl *ctx, UINT8 slave_address, UINT8 reg_address, UINT16 data)
{
	int ret = RET_OK;
	int retryCnt = 10;


	if(gVideoPhyWriteMode == 0){
		ret = hdmi_rx_ctrl_phy_write(ctx, slave_address, reg_address, data);
	}
	else{
		while(1)
		{
			ret = hdmi_rx_ctrl_phy_write(ctx, slave_address, reg_address, data);
			if(ret<0){
				if(retryCnt <0){
					ret = RET_ERROR;
					break;
				}
				retryCnt -= 1;
				HDMI20_ERROR("Error [%d] : Error Writing Phy. SA[0x%x]/[0x%x]<=[0x%x]\n", ctx->device_id, slave_address, reg_address, data);
				msleep(1);
			}
			else{
				break;
			}
		}
	}
	return ret;
}



int HDMI20_HAL_H15Ax_Rx_Initiate_Phy(int port, int isUD)
{
	int ret;
	int i;
	UINT32 regData = 0;
	UINT32 phyEQsetValue = 0;
	UINT32 phyEQsetMode = 2;
	UINT32 data;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return RET_ERROR;
	}

	/* Important to clear ManualSYnc value */
	data = gHDMI20RxHandler[port].ctrl.bsp_read(0x400);
	data &= (~0x0000000F);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x0);

	if(gFixedScdcValue > 0){
		isUD = 1;
	}

	if(gHDMI20RxHandler[port].isPhyAlterMode == true){
		phyEQsetValue = gVideoPhyAlterFHDValue;
		phyEQsetMode = gVideoPhyAlterFHDMode;
	}
	else{
		phyEQsetValue = gVideoPhyFHDValue;
		phyEQsetMode = gVideoPhyFHDDBGMode;
	}


	gHDMI20RxHandler[port].phyInitiatedMode = isUD;

	gHDMI20RxHandler[port].ctrl.previous_video.deep_color_mode = 24;
	gHDMI20RxHandler[port].dcm_change = false;

	HDMI20_VIDEO(" Initate Phy called with [%d](0:FHD/1:UHD). Auto/Manual:[%d]/[%d], Value[0x%x]/[0x%x]\n",\
			isUD,phyEQsetMode,gVideoPhyUHDDBGMode, phyEQsetValue, gVideoPhyUHDValue);
	
	/* Set local mute on PHy initiate */
	__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(port, 1, 0, __L__);

	gHDMI20RxHandler[port].ctrl.bsp_write(0x02C0, 0x00000063);
	msleep(gLGEPhyAPISleepTime);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x02C0, 0x00000062);
	msleep(gLGEPhyAPISleepTime);


	if(isUD){
		if(gVideoPhyUHDDBGMode == 0x2){
			ret  = __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x2, 0x22c8);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0xE, 0x108);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x25, 0x6);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x8, 0x40);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3e, gVideoPhyUHDValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x5e, gVideoPhyUHDValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x7e, gVideoPhyUHDValue);
		}
		else{
			ret  = __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x2, 0x22c8);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0xE, 0x108);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x25, 0x6);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x41, gVideoPhyUHDValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x61, gVideoPhyUHDValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x81, gVideoPhyUHDValue);
		}
	}
	else{
		ret  = __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x2, 0x22c8);
		ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0xe, 0x8);
		ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x25, 0x4);

		if( phyEQsetMode== 0x2){
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x8, 0x40);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3e, phyEQsetValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x5e, phyEQsetValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x7e, phyEQsetValue);
		}
		else {
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x41, phyEQsetValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x61, phyEQsetValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x81, phyEQsetValue);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x5, 0x1809);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x5, 0x1819);
			ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0x5, 0x1809);
		}

	}
	
	if(gHDMI20RxHandler[port].currentDCMMode == 0x5){
		hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x220);
	}
	else if(gHDMI20RxHandler[port].currentDCMMode == 0x6){
		hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x240);
	}
	else if(gHDMI20RxHandler[port].currentDCMMode == 0x7){
		hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x260);
	}


	if(gVideoDebugLevel != 5544){
		ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, 0x69, 0xD, 0xA);
	}

	for(i=0;i<phyAddHist[port].cnt; i++){
		printk("AUTO EQ DBG[%d] : Slave[0x%x]/Reg[0x%x] <= [0x%x]\n", port, phyAddHist[port].slave_addr[i], phyAddHist[port].reg_addr[i], phyAddHist[port].data[i]);
		ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, phyAddHist[port].slave_addr[i], phyAddHist[port].reg_addr[i], phyAddHist[port].data[i]);
	}

	for(i=0;i<phyDBGHist.cnt; i++){
		printk("PHY DBG[%d] : Slave[0x%x]/Reg[0x%x] <= [0x%x]\n", port, phyDBGHist.slave_addr[i], phyDBGHist.reg_addr[i], phyDBGHist.data[i]);
		ret |= __HDMI20_HAL_H15Ax_WritePhy(&gHDMI20RxHandler[port].ctrl, phyDBGHist.slave_addr[i], phyDBGHist.reg_addr[i], phyDBGHist.data[i]);
	}
	


	gHDMI20RxHandler[port].tmds_change = false;
	gHDMI20RxHandler[port].ctrl.bsp_write(0x02C0, 0x00000060);

	if(gHDMI20RxHandler[port].isAudioARCMode){
		if(port == 0){
			regData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0xC);
			regData |= 0x00000001;

			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0xC, regData);
		}
		else if(port == 1){
			regData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0xC);
			regData |= 0x00000001;

			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0xC, regData);
		}
	}
	else{
		if(port == 0){
			regData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0xC);
			regData &= (~0x00000001);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0xC, regData);
		}
		else if(port == 1){
			regData = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0xC);
			regData &= (~0x00000001);
			__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0xC, regData);
		}
	}

	HDMI20_AUDIO("Init Phy : port(%d) ARCmode(%d)", port, gHDMI20RxHandler[port].isAudioARCMode);

	//__HDMI20_HAL_H15Ax_Rx_Audio_Configure(port, isUD);

	return ret;
}

int HDMI20_HAL_Check5VLevelInfo(int port, int *pValue)
{
	int ret = 0;
	UINT32 gpio_num;
	UINT32 reg5Vvalue = 0;

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(gVideoDebugLevel == 756){
			*pValue = 1;
			ret = RET_OK;
			break;
		}

		if(gVideoDebugLevel != 2014){
			gpio_num = gHDMI20RxHandler[port].port5vGPIOnum;

			ret = _GPIO_GetValue(gpio_num, (LX_GPIO_VALUE_T *)pValue);
			if((ret >= 0)&&(*pValue != 0xFF)) {
				ret = RET_OK;
			}

		}
		else{
			reg5Vvalue= gHDMI20RxHandler[port].ctrl.bsp_read(HDMI20_RX_H15B0_5V_ADDR);
			reg5Vvalue &= HDMI20_RX_H15B0_5V_BIT;

			*pValue = reg5Vvalue;
		}

	} while(0);

	return ret;
}

int HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_T type, int port)
{
	int ret = RET_ERROR;
	UINT32 data;

	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if((unsigned int)gHDMI20RxHandler[port].ipcmem.hwAddr == 0){
			HDMI20_ERROR("User memory not initiated yet\n");
			break;
		}

		if(gHDMI20RxHandler[port].ipcmem.pSwAddr == NULL){
			HDMI20_ERROR("[%s:%d] Error! Device Driver access without initialization.\n", __F__, __L__);
			return;
		}


		if(type >= HDMI20_HAL_IPC_MAXNUM){
			HDMI20_ERROR("Unknown IPC type [x%08x] \n", type);
		}

		/* Check User Data */
		data = (UINT32)(*((UINT32 *)((UINT32)gHDMI20RxHandler[port].ipcmem.pSwAddr)));	

		if((data & type) != type){
			// Initiate ESM

			data |= type;
			*((UINT32 *)((UINT32)gHDMI20RxHandler[port].ipcmem.pSwAddr)) = data;
		}

		ret = RET_OK;
	}while(0);

	return ret;
}


int HDMI20_HAL_ConnectCable(int port, int bOnOff)
{
	int ret = RET_ERROR;
	LX_GPIO_VALUE_T gpioValue = LX_GPIO_VALUE_INVALID;
	UINT32 data;
	UINT32 hpd_data;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return RET_ERROR;
	}

	if(gMainThreadStamp == 0){
		HDMI20_INFO("WARN %d : Connect Cable was called without MAIN THREAD operation\n", port);
		return RET_ERROR;
	}
	gHDMI20RxHandler[port].isPhyAlterMode = 0;

	gAudioIsGalaxyTab10[port] = 0;

	__HDMI20_HAL_H15Ax_UpdateDCMMode(port, 1, 1);

	//gHDMI20RxHandler[port].isAudioARCMode = 0;
	HDMI20_AUDIO("Cnct cable : port(%d) ARCmode(%d)", port, gHDMI20RxHandler[port].isAudioARCMode);
	gHDMI20RxHandler[port].check19201080Dcm = 0;
	gHDMI20RxHandler[port].is19201080Dcm = 0;

	if(bOnOff){
		HDMI20_INFO("HDMI2.0 Port[%d] Connect sequence. \n", port);

		gHDMI20RxHandler[port].cable_connect_time = jiffies_to_msecs(jiffies);

		/* SW WA : PS4 cable connection long time */
		gReinitPhyOnLockLossByCnt = 1;

		/* Audio Mute */
		//gHDMI20RxHandler[port].audioMuteEableCnt = 0;
		//gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
		__HDMI20_HAL_H15Ax_AudioReset(port);
		
		//__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(port, 1);
		__HDMI20_HAL_H15Ax_ClearChangeFlags(port);
		gHDMI20RxHandler[port].tmds_change = 1;

		gHDMI20RxHandler[port].is5Vconnected = 1;
		gHDMI20RxHandler[port].connect_request = 1;
		gHDMI20RxHandler[port].phyLostCnt = 0;
		gHDMI20RxHandler[port].isTimingInvalid = 0;
		
		__HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(port, 1);

		gHDMI20RxHandler[port].isAudioMute = 1;
		
		if(gVideoDebugLevel == 803){ __HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(port, 0);}
	}
	else{
		HDMI20_INFO("HDMI2.0 Port[%d] Dis-connect sequence. \n", port);

		gHDMI20RxHandler[port].ctrl.isMute = 0;
		gHDMI20RxHandler[port].isHDCP14 = 0;
		gHDMI20RxHandler[port].isHDCP22 = 0;
		gHDMI20RxHandler[port].isHDCP22Ncap = 0;

		gHDMI20RxHandler[port].isScdcUD = 0;

		data = gHDMI20RxHandler[port].ctrl.bsp_read(0xb4);
		data |= (0x00010000);
		gHDMI20RxHandler[port].ctrl.bsp_write(0xb4, data);

		gVideoStableCheckCnt = gVideoStableCntInitial;
		gHDMI20LocalMuteHoldTime = gHDMI20LocalMuteHoldInitTime;	

		if(gVideoDebugLevel == 803){ __HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(port, 0);}

		/* Audio Mute */
		//gHDMI20RxHandler[port].audioMuteEableCnt = 0;
		//gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
		__HDMI20_HAL_H15Ax_AudioReset(port);
		
		if(gHPDInverseMode){
			hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
			gHDMI20RxHandler[port].isHPD = 0;
			
			gpioValue = LX_GPIO_VALUE_HIGH;

			ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
			ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

			if(ret < 0){
				HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
			}
		}
		else{
			if(lx_chip_rev( ) >= LX_CHIP_REV(H15,A1)){
				hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
				gHDMI20RxHandler[port].isHPD = 0;

				gpioValue = LX_GPIO_VALUE_LOW;

				ret  = _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_real , gpioValue);
				ret |= _GPIO_SetValue(gHDMI20RxHandler[port].gpio_hpd_temp , gpioValue);

				if(ret < 0){
					HDMI20_ERROR("ERROR %d : Failed to control GPIO HPD\n", port);
				}
			}
			else{
				if(port == 1)
				{
					hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 1);
					gHDMI20RxHandler[port].isHPD = 0;
				}
				else{
					hdmi_rx_ctrl_hpd(&gHDMI20RxHandler[port].ctrl, 0);
					gHDMI20RxHandler[port].isHPD = 0;
				}
			}
		}

		/* Reset HDCP */
		gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, 0x7F);
		__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(port ,0, 1);

		gHDMI20RxHandler[port].ctrl.bsp_write(0x808, 0x0);

		if (hdmi_rx_phy_close(&gHDMI20RxHandler[port].phy) < 0) {
			gHDMI20RxHandler[port].ctrl.log_info("First time, or error when closing RX[%d] PHY", port);
		}

		gHDMI20RxHandler[port].ctrl.bsp_write(0xff0, ((1 << 3) - 1) << 1);
		if (hdmi_rx_phy_open(&gHDMI20RxHandler[port].phy) < 0) {
			gHDMI20RxHandler[port].ctrl.log_info("Error when opening RX[%d] PHY", port);
		}
		__HDMI20_HAL_H15Ax_Rx_InitiateHDMI20Data(port, 1);
		__HDMI20_HAL_H15Ax_ClearChangeFlags(port);

		gHDMI20RxHandler[port].ctrl.bsp_write(0x800, 0x1F11);
		gHDMI20RxHandler[port].ctrl.bsp_write(0x860, 0xABCDEF01);
		gHDMI20RxHandler[port].ctrl.bsp_write(0x808, 0x1);
		gHDMI20RxHandler[port].ctrl.bsp_write(0x80C, 0x3FF0C15C);


		gHDMI20RxHandler[port].is5Vconnected = 0;
		gHDMI20RxHandler[port].ctrl.stable_cnt = 0;
		gHDMI20RxHandler[port].phyLostCnt = 0;
		gHDMI20RxHandler[port].isTimingInvalid = 0;
		__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[port].ctrl, 1);
		__HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(port, 1);
		
	}

	return RET_OK;
}

static void __HDMI20_HAL_H15Ax_Rx_SetForcedHDMIMode(UINT8 port)
{
	UINT32 data;

	data = gHDMI20RxHandler[port].ctrl.bsp_read(0x080);
	data |= 0x000000C0;
	gHDMI20RxHandler[port].ctrl.bsp_write(0x080, data);

}

int HDMI20_HAL_H15Ax_Rx_DiffVideoData(struct hdmi_rx_ctrl_video *v0, struct hdmi_rx_ctrl_video *v1)
{
	int changed_item = 0;

	if(v0->dvi != v1->dvi) {
		HDMI20_INFO("Changed [DVI] : [%d]/[%d]\n",v0->dvi, v1->dvi);
		changed_item++;
	}

	if(v0->deep_color_mode != v1->deep_color_mode) {
		HDMI20_INFO("Changed [DCM] : [%d]/[%d]\n", v0->deep_color_mode, v1->deep_color_mode);
		changed_item++;
	}

	if(v0->pixel_clk != v1->pixel_clk) {
		HDMI20_INFO("Changed [Pixel Clock] : [%d]/[%d]\n", (unsigned int)v0->pixel_clk, (unsigned int)v1->pixel_clk);
		changed_item++;
	}

	if(v0->refresh_rate != v1->refresh_rate){
		HDMI20_INFO("Changed [Refresh Rate] : [%d]/[%d]\n", (unsigned int)v0->refresh_rate, (unsigned int)v1->refresh_rate);
		changed_item++;
	}

	if(v0->interlaced != v1->interlaced){
		HDMI20_INFO("Changed [Interlaced] : [%d]/[%d]\n", v0->interlaced, v1->interlaced);
		changed_item++;
	}

	if(v0->voffset != v1->voffset){
		HDMI20_INFO("Changed [vOffset] : [%d]/[%d]\n", v0->voffset, v1->voffset);
		changed_item++;
	}

	if(v0->vactive != v1->vactive){
		HDMI20_INFO("Changed [vActive] : [%d]/[%d]\n", v0->vactive, v1->vactive);
		changed_item++;
	}

	if(v0->vtotal != v1->vtotal){
		HDMI20_INFO("Changed [vTotal] : [%d]/[%d]\n", v0->vtotal, v1->vtotal);
		changed_item++;
	}

	if(v0->hoffset != v1->hoffset) {
		HDMI20_INFO("Changed [hoffset] : [%d]/[%d]\n", v0->hoffset, v1->hoffset);
		changed_item++;
	}

	if(v0->hactive != v1->hactive){
		HDMI20_INFO("Changed [hactive] : [%d]/[%d]\n", v0->hactive, v1->hactive);
		changed_item++;
	}

	if(v0->htotal != v1->htotal) {
		HDMI20_INFO("Changed [htotal] : [%d]/[%d]\n", v0->htotal, v1->htotal);
		changed_item++;
	}

	if(v0->video_format != v1->video_format) {
		HDMI20_INFO("Changed [video_format] : [%d]/[%d]\n", v0->video_format, v1->video_format);
		changed_item++;
	}

	if(v0->active_valid != v1->active_valid){
		HDMI20_INFO("Changed [active_valid] : [%d]/[%d]\n", v0->active_valid, v1->active_valid);
		changed_item++;
	}

	if(v0->bar_valid != v1->bar_valid){
		HDMI20_INFO("Changed [bar_valid] : [%d]/[%d]\n", v0->bar_valid, v1->bar_valid);
		changed_item++;
	}

	if(v0->scan_info != v1->scan_info) {
		HDMI20_INFO("Changed [scan_info] : [%d]/[%d]\n", v0->scan_info, v1->scan_info);
		changed_item++;
	}

	if(v0->colorimetry != v1->colorimetry){
		HDMI20_INFO("Changed [colorimetry] : [%d]/[%d]\n", v0->colorimetry, v1->colorimetry);
		changed_item++;
	}

	if(v0->picture_ratio != v1->picture_ratio){
		HDMI20_INFO("Changed [Picture Ratio] : [%d]/[%d]\n", v0->picture_ratio, v1->picture_ratio);
		changed_item++;
	}

	if(v0->active_ratio != v1->active_ratio){
		HDMI20_INFO("Changed [Active Ratio] : [%d]/[%d]\n", v0->active_ratio, v1->active_ratio);
		changed_item++;
	}

	if(v0->it_content != v1->it_content){
		HDMI20_INFO("Changed [IT Content] : [%d]/[%d]\n", v0->it_content, v1->it_content);
		changed_item++;
	}

	if(v0->ext_colorimetry != v1->ext_colorimetry){
		HDMI20_INFO("Changed [Ext Colorimetry] : [%d]/[%d]\n", v0->ext_colorimetry, v1->ext_colorimetry);
		changed_item++;
	}

	if(v0->rgb_quant_range != v1->rgb_quant_range){
		HDMI20_INFO("Changed [RGB QUANT RANGE] : [%d]/[%d]\n", v0->rgb_quant_range, v1->rgb_quant_range);
		changed_item++;
	}

	if(v0->n_uniform_scale != v1->n_uniform_scale){
		HDMI20_INFO("Changed [Uniform Scale] : [%d]/[%d]\n", v0->n_uniform_scale, v1->n_uniform_scale);
		changed_item++;
	}


	if(v0->video_mode != v1->video_mode){
		HDMI20_INFO("Changed [Video Mode] : [%d]/[%d]\n", v0->video_mode, v1->video_mode);
		changed_item++;
	}

	if(v0->pixel_repetition != v1->pixel_repetition){
		HDMI20_INFO("Changed [Pixel Repetition] : [%d]/[%d]\n", v0->pixel_repetition, v1->pixel_repetition);
		changed_item++;
	}

	if(v0->bar_end_top != v1->bar_end_top){
		HDMI20_INFO("Changed [Bar End Top] : [%d]/[%d]\n", v0->bar_end_top, v1->bar_end_top);
		changed_item++;
	}

	if(v0->bar_start_bottom != v1->bar_start_bottom){
		HDMI20_INFO("Changed [Bar Start Bottom] : [%d]/[%d]\n", v0->bar_start_bottom, v1->bar_start_bottom);
		changed_item++;
	}

	if(v0->bar_end_left != v1->bar_end_left){
		HDMI20_INFO("Changed [BAR END LEFT] : [%d]/[%d]\n", v0->bar_end_left, v1->bar_end_left);
		changed_item++;
	}

	if(v0->bar_start_right != v1->bar_start_right){
		HDMI20_INFO("Changed [Bar Start Right] : [%d]/[%d]\n", v0->bar_start_right, v1->bar_start_right);
		changed_item++;
	}

	HDMI20_INFO(" Total Changed : [%d]\n", changed_item);

	return 0;
}


int HDMI20_HAL_H15Ax_Rx_OverRideHDCP(int port, int isHDCP22, int enable)
{
	UINT32 data = 0;

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
		return RET_ERROR;
	}

	data = 	gHDMI20RxHandler[port].ctrl.bsp_read(0x81C);

	if ( enable ) { data |= HDMI20_RX_H15AX_HDCP_OVR_ENABLE; }
	else { data &= (~HDMI20_RX_H15AX_HDCP_OVR_ENABLE); }

	if(isHDCP22) { data |= HDMI20_RX_H15AX_HDCP_OVR_SELECT;}
	else { data &= (~HDMI20_RX_H15AX_HDCP_OVR_SELECT);}

	gHDMI20RxHandler[port].ctrl.bsp_write(0x81C, data);

	return RET_OK;
}


void HDMI20_HAL_H15Ax_Rx_LinkVideoReset(int port, int mode)
{
	int ret;
	UINT32 hpd_value = 0;
	UINT32 data = 0;

	HDMI20_VIDEO("Link Video Reset on port[%d]\n", port);
	
	__HDMI20_HAL_H15Ax_Rx_SetLocalVideoMute(port, 1, 0, __L__);

	/* Audio Mute */
	//gHDMI20RxHandler[port].audioMuteEableCnt = 0;
	//gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000000);// AUD_OUTPUT_MUTE_CTRL mute
	__HDMI20_HAL_H15Ax_AudioReset(port);
	
	hpd_value = gHDMI20RxHandler[port].ctrl.bsp_read(0x0);

	if((gVideoLinkResetMode > 0)||(mode >0)){

		//msleep(gVideoLinkResetDelayTime);
		gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, gLinkResetValue);
		msleep(gVideoLinkResetDelayTime);
		gHDMI20RxHandler[port].ctrl.bsp_write(0xFF4, gLinkResetValue);
		msleep(gVideoLinkResetDelayTime);
	}
	else{
		if(mode){
			gHDMI20RxHandler[port].ctrl.bsp_write(0xFF4, gLinkResetValue);
			msleep(gVideoLinkResetDelayTime);
			gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, gLinkResetValue);
			msleep(gVideoLinkResetDelayTime);
		}
		else{
			/* Avoid Side effect of Manual Sync */
			gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0);

			/* Audio reset on VideoLinkReset */
			gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, 0x10);
			msleep(gVideoLinkResetDelayTime);
		}
	}
	//ret = hdmi_rx_ctrl_open(&gHDMI20RxHandler[port].ctrl);
	//if( ret <0 ) { HDMI20_ERROR("Error opening HDMI20 port[%d] ctrl[%d]\n", port, ret); }

	/* HDCP */
	ret = hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
	if( ret <0 ) { HDMI20_ERROR("Error opening HDMI20 port[%d] hdcp 1.4[%d]\n", port, ret); }

	/* HDCP1.4 Set DDC to 100Khz */
	gHDMI20RxHandler[port].ctrl.bsp_write(0xc4, gHDCPBcapsCtrlValue);


	ret = hdmi_rx_ctrl_config(&gHDMI20RxHandler[port].ctrl, gHDMI20RxHandler[port].port);
	if( ret <0 ) { HDMI20_ERROR("Error on HDMI20 port[%d] initial configuration[%d]\n",port, ret); }

	gHDMI20RxHandler[port].ctrl.bsp_write(0x080, gHDMIModeRecover);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x16C, gHDMIVerticalTiming);
	if(gForcedHDMIMode >0){ __HDMI20_HAL_H15Ax_Rx_SetForcedHDMIMode(port); }

	/* Auto detect VS, HS polarity : Enable Auto sync detect */
	gHDMI20RxHandler[port].ctrl.bsp_write(0x90, 0x14);

	if(gHDMI20RxHandler[port].gWasHDCP22Written >= 1){
		HDMI20_VIDEO("Link VIdeo reset with [HDCP2.2 enable][%d]/[%d]\n", gHDMI20RxHandler[port].gWasHDCP14Written, gHDMI20RxHandler[port].gWasHDCP22Written);

		if(gHDMI20RxHandler[port].gWasHDCP14Written >= 1){
			gHDMI20RxHandler[port].ctrl.bsp_write(0xc0, 0x01000001);
		}
		else{
			gHDMI20RxHandler[port].ctrl.bsp_write(0xc0, 0x01000000);
		}
		gHDMI20RxHandler[port].ctrl.bsp_write(0x81c, 0x00410000);
	}
	else if(gHDMI20RxHandler[port].gWasHDCP14Written >= 1){
		HDMI20_VIDEO("Link Video reset with [HDCP1.4 enable][%d]/[%d]\n", gHDMI20RxHandler[port].gWasHDCP14Written, gHDMI20RxHandler[port].gWasHDCP22Written);
		gHDMI20RxHandler[port].ctrl.bsp_write(0xc0, 0x01000001);
		/* Make sure to change this value to 0x0 when HDCP2.2 is ready */
		gHDMI20RxHandler[port].ctrl.bsp_write(0x81c, 0x00410002);
	}
	else{
		// Test mode
		gHDMI20RxHandler[port].ctrl.bsp_write(0xc0, 0x01000000);
		gHDMI20RxHandler[port].ctrl.bsp_write(0x81c, 0x2);
	}

	/* List up to subscribe */
	/* TMDS refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_clock_change, __HDMI20_HAL_H15Ax_Refresh_TMDS);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] TMDS Refresher\n", port);}
	/* VIDEO refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_video_status, __HDMI20_HAL_H15Ax_Refresh_Video);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] VIDEO Refresher\n", port);}
	/* Audio Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_audio_status, __HDMI20_HAL_H15Ax_Refresh_Audio);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AUDIO Refresher\n", port);}
	/* Data Packet Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_packet_reception, __HDMI20_HAL_H15Ax_Refresh_DataPacket);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Data Packet Refresher\n", port);}
	/* AKSV Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_aksv_reception, __HDMI20_HAL_H15Ax_Refresh_AKSV);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AKSV Refresher\n",  port);}
	/* PLL Lock Change Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_pll_lock_change, __HDMI20_HAL_H15Ax_Refresh_PLLLock);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] PLL Lock Change Refresher\n",  port);}
	/* Phy i2c done  Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_phy_i2c_done, __HDMI20_HAL_H15Ax_Refresh_PhyI2CDone);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Phy i2c done Refresher\n",  port);}
	/* Phy i2c nack Refresher to IRQ */
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_phy_i2c_nack, __HDMI20_HAL_H15Ax_Refresh_PhyI2CNack);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] Phy i2c nack Refresher\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_dcm_change, __HDMI20_HAL_H15Ax_Refresh_DCM);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] DCM Refresher\n",  port);}

	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_avmute_change, __HDMI20_HAL_H15Ax_Refresh_AVMute);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] AVMute Refresher\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_scdc, __HDMI20_HAL_H15Ax_Refresh_SCDC);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] SCDC refresh\n",  port);}
	
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_cap, __HDMI20_HAL_H15Ax_Refresh_HDCP22_CAP);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 CAP Refresh\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_ncap, __HDMI20_HAL_H15Ax_Refresh_HDCP22_NCAP);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 NCAP Refresh\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_lost, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_LOST);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_LOST Refresh\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_ists, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_ISTS);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_ISTS Refresh\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_fail, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_FAIL);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_FAIL Refresh\n",  port);}
	ret = hdmi_rx_ctrl_subscribe(&gHDMI20RxHandler[port].ctrl, hdmi_rx_ctrl_event_hdcp22_auth_change, __HDMI20_HAL_H15Ax_Refresh_HDCP22_AUTH_CHANGE);
	if( ret <0 ) { HDMI20_ERROR("Error listing port[%d] HDCP22 AUTH_CHANGE Refresh\n",  port);}

	/* Auto detect VS, HS polarity : Enable Auto sync detect */
	gHDMI20RxHandler[port].ctrl.bsp_write(0x90, 0x14);


	/* Initiate Data Packet Buffer */
	HDMI20_H15A0_ResetDataPacketBuffer(hdmi_rx_ctrl_packet_cnt, port);



	/* Set DCM Auto Detect */
	gHDMI20RxHandler[port].ctrl.bsp_write(0x0A8, gHDMIDcmCtrl);

	/* Restore HPD*/
	gHDMI20RxHandler[port].ctrl.bsp_write(0x0, hpd_value);

	/* Enable AVMute */
	data = gHDMI20RxHandler[port].ctrl.bsp_read(0x300);
	data = data | 0x00008000;
	gHDMI20RxHandler[port].ctrl.bsp_write(0x300, data);

	/* Init SCDC */
	//hdmi_rx_scdc_initConfig(&gHDMI20RxHandler[port].ctrl);
	//hdmi_rx_scdc_scrambling(&gHDMI20RxHandler[port].ctrl, 1);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x800, 0x1F11);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x860, 0xABCDEF01);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x808, 0x1);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x80C, 0x3FF0C15C);

	/*Clear Sync */

	/* Initiate checker value for SOURCE HDMI2.0 capability */
	__HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(port, 1);
	msleep(gLinkResetWaitTime);
}

static void __HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(UINT8 port)
{
	UINT32 data;

	if(port >= HDMI20_NUM_OF_RX_PORTS){
		HDMI20_VIDEO("ERR %d : Prevent Error. Non existing port[%d]\n", port, port);
		return;
	}

	HDMI20_VIDEO("Notice [%d] : Reset Sync position. Delay is [%d] msec\n", port, gVideoSyncResetDelay);

	if(port >= HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_VIDEO("ERR %d : Port [%d] number exceeded [%d].\n", port, port, HDMI20_HAL_NUMBER_OF_LINK);
		return;
	}

	data = gHDMI20RxHandler[port].ctrl.bsp_read(0x400);
	data |= 0x80000000;
	gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);
	msleep(gVideoSyncResetDelay);
	data &= (~0x80000000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x400, data);

	if(gVideoDCMUpdateOnVideoChange == 2)__HDMI20_HAL_H15Ax_UpdateDCMMode(port, 0, gHDMI20RxHandler[port].ctrl.previous_video.deep_color_mode);
}

/* DBG function */
static int __HDMI20_HW_H15_SetCombination(UINT32 param)
{
	int ret = RET_ERROR;
	int i,count;
	volatile LX_HDMI20_REG_T	*regs;
	char	*index;
	unsigned int read_data, same;

	do {
		/* Step 1. Check whether paramater is valid */
		if(param == 119) {	// Magic keyword to print available output modes...
			printk("\n[HDMI20] Display all register-set-presets ...\n");
			for(i=0; i<_gHDMI20RegSettings.number_of_modes;i++)
			{
				printk("  Index[%d] : %s\n", i,_gHDMI20RegSettings.pRegSettings[i].index);
			}
			ret = RET_OK;
			break;
		}
		else if(param >= 10000)
		{
			param = param - 10000;
			printk("\n[HDMI20] Compare register values regarding to register-set index[%d]", param);

			if(param > _gHDMI20RegSettings.number_of_modes) {
				HDMI20_ERROR("Unknown HDMI20 index. Index was [%d]", param);
				break;
			}

			regs  = _gHDMI20RegSettings.pRegSettings[param].regs;
			count = _gHDMI20RegSettings.pRegSettings[param].count;
			index = _gHDMI20RegSettings.pRegSettings[param].index;

			printk("\n[HDMI20] Register Index[%d] => %s",param, (char *)index);
			for(i=0; i<count; i++){
				HDMI20_REG_RD(regs[i].addr, read_data);
				if(read_data == regs[i].value) same = 1;
				else same = 0;

				printk("\n[%03d] Current value of [0x%08x] => [0x%08x] / index value [0x%08x] (%s)", i, regs[i].addr, read_data, regs[i].value, same ? "SAME":"DIFFER");
			}
			printk("\n[HDMI20] Register read complete!\n");

			ret = RET_OK;
			break;
		}
		else if(param > _gHDMI20RegSettings.number_of_modes) {
			HDMI20_ERROR("\nUnknown HDMI20 mode. Index was [%d]\n", param);
			break;
		}

		/* Step 2. Set mode */
		regs  = _gHDMI20RegSettings.pRegSettings[param].regs;
		count = _gHDMI20RegSettings.pRegSettings[param].count;
		index = _gHDMI20RegSettings.pRegSettings[param].index;

		printk("\n[HDMI20] Setting to registers to register-set-index => %s", (char *)index);
		for(i=0; i<count; i++){
			printk("\n[%03d] Writing... [0x%08x] <= [0x%08x]", i, regs[i].addr, regs[i].value);
			HDMI20_REG_WR(regs[i].addr, regs[i].value);
			msleep(1);
		}
		printk("\n[HDMI20] Setting preset complete!\n");

		ret = RET_OK;
	} while(0);

	return ret;
}



static void __HDMI20_HAL_H15Ax_Rx_Audio_Configure(UINT8 port, int isUD)
{
	HDMI20_AUDIO("__HDMI20_HAL_H15Ax_Rx_Audio_Configure\n");
	if(port == 0){
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x38, 0x1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x34, 0x1);
	}
	else if(port == 1){
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x38, 0x1);
		__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x34, 0x1);
	}

	//gHDMI20RxHandler[port].ctrl.bsp_write(0x284, 0x00001800);// AUDPLL_GEN_N
	gHDMI20RxHandler[port].ctrl.bsp_write(0x200, 0x00000060);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x214, 0x00000003);// AUD_CLK_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x208, 0x00000000);// AUD_PLL_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x094, 0x0016FFF0);// HDMI_CKM_EVLTM
	gHDMI20RxHandler[port].ctrl.bsp_write(0x098, 0xF98A0190);// HDMI_CKM_F
	gHDMI20RxHandler[port].ctrl.bsp_write(0x244, 0x01401008);// AUD_FIFO_TH
	//gHDMI20RxHandler[port].ctrl.bsp_write(0x080, 0x00000021);// HDMI_MODE_RECOVER
	gHDMI20RxHandler[port].ctrl.bsp_write(0x340, 0x0000007C);// PDEC_ASP_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x240, 0x00010000);// AUD_FIFO_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x0A4, 0x00000001);// HDMI_RESMPL_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x258, 0x007004A8);// AUD_MUTE_CTRL
	//gHDMI20RxHandler[port].ctrl.bsp_write(0x258, 0x807004A8);// AUD_MUTE_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x264, 0x00006000);// AUD_PAO_CTRL
	//gHDMI20RxHandler[port].ctrl.bsp_write(0x254, 0x0000007F);// AUD_CHEXTR_CTRL
	gHDMI20RxHandler[port].ctrl.bsp_write(0x3C0, 0x00000100);// PDEC_AIF_CTRL

	gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
	gHDMI20RxHandler[port].ctrl.bsp_write(0x254, 0x000000FC);// AUD_CHEXTR_CTRL

	gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_CHECK_CNT;
	gHDMI20RxHandler[port].isAudioMute = 1;
}

static char *uint8ToBinary(UINT8 i)
{
	static char s[ 8 + 1 ] = {'0', };
	int count = 8;

	do{
		s[--count] = '0' + (char)( i & 1 );
		i = i >> 1;
	} while(count);

	return s;
}

/* Added 2014/07/02 taejun.lee */
static void __HDMI20_HAL_H15Ax_TX_AUD_I2S_SETTING(void)
{
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3100<<2), 0x002F);
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3101<<2), 0x0018);
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3103<<2), 0x0000);
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3102<<2), 0x0003);

}

/* Added 2014/07/02 taejun.lee */
static void __HDMI20_HAL_H15Ax_TX_AUD_FC(void)
{
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3200<<2), 0x0000); //Write Audio Clock Regenerator N Value Register 1
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3201<<2), 0x0018); //Write Audio Clock Regenerator N Value Register 2
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3202<<2), 0x0000); //Write Audio Clock Regenerator N Value Register 3
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3203<<2), 0x0014); //Write Audio Clock Regenerator CTS Value Register 1
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3204<<2), 0x0044); //Write Audio Clock Regenerator CTS Value Register 2
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x3205<<2), 0x0012); //Write Audio Clock Regenerator CTS Value Register 3

	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1025<<2), 0x0071); //FC_AUDICONF0	0x1025 DataWr = 10 @ 1797785
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1026<<2), 0x0033); //FC_AUDICONF1	0x1026 DataWr = 00 @ 1798045
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1027<<2), 0x001F); //FC_AUDICONF2	0x1027 DataWr = 00 @ 1798305
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1028<<2), 0x0000); //FC_AUDICONF3	0x1028 DataWr = 20 @ 1798605
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1063<<2), 0x0001); //FC_AUDSCONF	 0x1063 DataWr = a0 @ 1798895 //jkang for 8ch
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1065<<2), 0x0000); //FC_AUDSV	 0x1065 DataWr = 00 @ 1799155
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1066<<2), 0x0000); //FC_AUDSU	 0x1066 DataWr = ff @ 1799415
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1067<<2), 0x0000); //FC_AUDSCHNLS0 0x1067 DataWr = 11 @ 1799675
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1068<<2), 0x0000); //FC_AUDSCHNLS1 0x1068 DataWr = aa @ 1799975
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x1069<<2), 0x0000); //FC_AUDSCHNLS2 0x1069 DataWr = 14 @ 1800265
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106A<<2), 0x0000); //FC_AUDSCHNLS3 0x106a DataWr = 22 @ 1800525
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106B<<2), 0x0000); //FC_AUDSCHNLS4 0x106b DataWr = 22 @ 1800785
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106C<<2), 0x0000); //FC_AUDSCHNLS5 0x106c DataWr = 12 @ 1801045
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106D<<2), 0x0000); //FC_AUDSCHNLS6 0x106d DataWr = 20 @ 1801305
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106E<<2), 0x0002); //FC_AUDSCHNLS7 0x106e DataWr = e @ 1801605
	__HDMI20_HAL_H15Ax_Tx_Write_Link((0x106F<<2), 0x0000); //FC_AUDSCHNLS8 0x106f DataWr = 6c @ 1801895

}

/* Added 2014/07/02 taejun.lee */
static int __HDMI20_HAL_H15Ax_GetAudioType(UINT8 port, LX_HDMI_AUDIO_TYPE_T *audioType)
{
	int ret = RET_OK;

	UINT32	reg_hdmi_mode = 0;
	UINT32	reg_achst_byte0;
	UINT32	reg_burst_pc_0;

	if (pgHDMI20AudioData == NULL) {
		return RET_ERROR;
	}

	if(port == 0) {
		reg_achst_byte0 = pgHDMI20AudioData->aud_hdmi0_fmt;
		reg_burst_pc_0  = pgHDMI20AudioData->aud_hdmi0_pcpd & 0xFFFF;
	} else if(port ==1 ) {
		reg_achst_byte0 = pgHDMI20AudioData->aud_hdmi1_fmt;
		reg_burst_pc_0	= pgHDMI20AudioData->aud_hdmi1_pcpd & 0xFFFF;
	}else {
		return RET_ERROR;
	}

	/* Added 2014/07/03 taejun.lee */
	//reg_burst_pc_0 = ((reg_burst_pc_0 >> 8) | (reg_burst_pc_0 << 8)) & 0xFFFF;

	if((UINT16)gHDMI20RxHandler[port].timing_info.dvi){
		reg_hdmi_mode = 0;
	}
	else{
		reg_hdmi_mode = 1;
	}

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	if (reg_hdmi_mode && reg_achst_byte0 > 0)
	{
		//Check if audio sample word used for other purposes than liner PCM.

		if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)	//bit 1, 0 : PCM, 1 : non-PCM
		{
			//Check a vaild Pc data for payload
			if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
			{
				//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
				switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) //bit 4 ~ 0
				{
					case BURST_INFO_AUDIO_TYPE_AC3:
					case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
						*audioType = LX_HDMI_AUDIO_AC3;
						break;

					case BURST_INFO_AUDIO_TYPE_DTS_I:
					case BURST_INFO_AUDIO_TYPE_DTS_II:
					case BURST_INFO_AUDIO_TYPE_DTS_III:
					case BURST_INFO_AUDIO_TYPE_DTS_IV:
						*audioType = LX_HDMI_AUDIO_DTS;
						break;

					case BURST_INFO_AUDIO_TYPE_MPEG2_AAC:
					case BURST_INFO_AUDIO_TYPE_MPEG2_AAC_LOW:
					case BURST_INFO_AUDIO_TYPE_MPEG4_AAC:
						*audioType = LX_HDMI_AUDIO_AAC;
						break;

					case BURST_INFO_AUDIO_TYPE_MPEG1_L1:
					case BURST_INFO_AUDIO_TYPE_MPEG1_L23:
					case BURST_INFO_AUDIO_TYPE_MPEG2_EXT:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L1:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L2:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L3:
						*audioType = LX_HDMI_AUDIO_MPEG;
						break;

					case BURST_INFO_AUDIO_TYPE_NULL:
						*audioType = _gH15AxHdmiAudioInfo[port].audioType;
						//if ((_gH15A0HdmiAudioThreadCount[port] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
						//	HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(link_num = %d, audioType= %d)\n", link_num,*audioType);
						break;

					default:
						*audioType = LX_HDMI_AUDIO_NO_AUDIO;
						break;
				}

				//Debug print
				/*if ((reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) == BURST_INFO_AUDIO_TYPE_PAUSE)
				  {
				  if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
				  HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_PAUSE(link_num = %d, Pc = %d)!!!\n", link_num,reg_burst_pc_0);
				  }
				  */
			}	//if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0)
			else
			{
				*audioType = LX_HDMI_AUDIO_PCM;

				//HDMI_ERROR("_HDMI_H15A0_GetAudioTypeAndFreq : Burst Info Error = %d(link_num = %d)\n", (reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK), link_num);
			}
		}	//if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)
		else
		{
			*audioType = LX_HDMI_AUDIO_PCM;

#if 0
			/* Workaround Code for Skylife and DMT HDMI Repeater(Digital Stream, HDMI 210) Issue.(2013-11-02) */
			if (_gH15A0IntrBurstInfoCount[link_num] != _gH15A0TaskBurstInfoCount[link_num])
			{
				//Update _gH15A0TaskBurstInfoCount[link_num] value.
				_gH15A0TaskBurstInfoCount[link_num] = _gH15A0IntrBurstInfoCount[link_num];

				/* Check a audio type in IEC61937 Burst Info. Packet. */
				//Check a vaild Pc data for payload
				LINK_NUM_H15_RdFL(link_num, audio_09);
				LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_0, reg_burst_pc_0);

				if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
				{
					//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
					switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) //bit 4 ~ 0
					{
						case BURST_INFO_AUDIO_TYPE_AC3:
						case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
							*audioType = LX_HDMI_AUDIO_AC3_IEC60958;
							break;

						case BURST_INFO_AUDIO_TYPE_NULL:
							*audioType = _gH15A0HdmiAudioInfo[link_num].audioType;
							if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
								HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(link_num = %d, audioType= %d)\n", link_num,*audioType);
							break;

						default:
							*audioType = _gH15A0HdmiAudioInfo[link_num].audioType;
							break;
					}
				}

				//Check a audio type if AC-3 ES exist.
				if (*audioType == LX_HDMI_AUDIO_AC3_IEC60958)
				{
					//Check a audio type if first audio info get state for debug print.
					if (_gH15A0IntrAudioState[link_num] == HDMI_AUDIO_GET_INFO_STATE)
					{
						HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : Forced AC-3.(link_num = %d, byte0 = 0x%X, Pc = 0x%X)\n", link_num,reg_achst_byte0, reg_burst_pc_0);
					}
				}
			}
			else
			{
				//Reset _gH15A0IntrBurstInfoCount[link_num]
				_gH15A0IntrBurstInfoCount[link_num] = 0;
			}
#endif
		}
	}
	else
	{
		//Set DVI mode
		*audioType	  = LX_HDMI_AUDIO_DVI;
		if (reg_achst_byte0 == 0) {
			*audioType = LX_HDMI_AUDIO_AC3;
		}
	}

	//HDMI_ATASK("_H15A0_GetAudioTypeAndFreq : link_num = %d, type = %d, freq = %d\n", link_num, *audioType, *samplingFreq);
	return ret;
}

static int __HDMI20_HAL_H15Bx_GetAudioType(UINT8 port, LX_HDMI_AUDIO_TYPE_T *audioType)
{
	int ret = RET_OK;

	UINT32	reg_hdmi_mode = 0;
	UINT32	reg_achst_byte0;
	UINT32	reg_burst_pc_0;

	if(port == 0) {
		reg_achst_byte0 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x70);
		reg_burst_pc_0  = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(0x88);
	} else if(port ==1 ) {
		reg_achst_byte0 = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x70);
		reg_burst_pc_0  = __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(0x88);
	}else {
		return RET_ERROR;
	}

	if((UINT16)gHDMI20RxHandler[port].timing_info.dvi){
		reg_hdmi_mode = 0;
	}
	else{
		reg_hdmi_mode = 1;
	}

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	if (reg_hdmi_mode > 0)
	{
		//Check if audio sample word used for other purposes than liner PCM.
		if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)	//bit 1, 0 : PCM, 1 : non-PCM
		{
			//Check a vaild Pc data for payload
			if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
			{
				//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
				switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) //bit 4 ~ 0
				{
					case BURST_INFO_AUDIO_TYPE_AC3:
					case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
						*audioType = LX_HDMI_AUDIO_AC3;
						break;

					case BURST_INFO_AUDIO_TYPE_DTS_I:
					case BURST_INFO_AUDIO_TYPE_DTS_II:
					case BURST_INFO_AUDIO_TYPE_DTS_III:
					case BURST_INFO_AUDIO_TYPE_DTS_IV:
						*audioType = LX_HDMI_AUDIO_DTS;
						break;

					case BURST_INFO_AUDIO_TYPE_MPEG2_AAC:
					case BURST_INFO_AUDIO_TYPE_MPEG2_AAC_LOW:
					case BURST_INFO_AUDIO_TYPE_MPEG4_AAC:
						*audioType = LX_HDMI_AUDIO_AAC;
						break;

					case BURST_INFO_AUDIO_TYPE_MPEG1_L1:
					case BURST_INFO_AUDIO_TYPE_MPEG1_L23:
					case BURST_INFO_AUDIO_TYPE_MPEG2_EXT:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L1:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L2:
					case BURST_INFO_AUDIO_TYPE_MPEG2_L3:
						*audioType = LX_HDMI_AUDIO_MPEG;
						break;

					case BURST_INFO_AUDIO_TYPE_NULL:
						*audioType = _gH15AxHdmiAudioInfo[port].audioType;
						//if ((_gH15A0HdmiAudioThreadCount[port] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
						//	HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(link_num = %d, audioType= %d)\n", link_num,*audioType);
						break;

					default:
						*audioType = LX_HDMI_AUDIO_NO_AUDIO;
						break;
				}

				//Debug print
				/*if ((reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) == BURST_INFO_AUDIO_TYPE_PAUSE)
				  {
				  if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
				  HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_PAUSE(link_num = %d, Pc = %d)!!!\n", link_num,reg_burst_pc_0);
				  }
				  */
			}	//if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0)
			else
			{
				*audioType = LX_HDMI_AUDIO_PCM;

				//HDMI_ERROR("_HDMI_H15A0_GetAudioTypeAndFreq : Burst Info Error = %d(link_num = %d)\n", (reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK), link_num);
			}
		}	//if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)
		else
		{
			*audioType = LX_HDMI_AUDIO_PCM;

#if 0
			/* Workaround Code for Skylife and DMT HDMI Repeater(Digital Stream, HDMI 210) Issue.(2013-11-02) */
			if (_gH15A0IntrBurstInfoCount[link_num] != _gH15A0TaskBurstInfoCount[link_num])
			{
				//Update _gH15A0TaskBurstInfoCount[link_num] value.
				_gH15A0TaskBurstInfoCount[link_num] = _gH15A0IntrBurstInfoCount[link_num];

				/* Check a audio type in IEC61937 Burst Info. Packet. */
				//Check a vaild Pc data for payload
				LINK_NUM_H15_RdFL(link_num, audio_09);
				LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_0, reg_burst_pc_0);

				if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
				{
					//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
					switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) //bit 4 ~ 0
					{
						case BURST_INFO_AUDIO_TYPE_AC3:
						case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
							*audioType = LX_HDMI_AUDIO_AC3_IEC60958;
							break;

						case BURST_INFO_AUDIO_TYPE_NULL:
							*audioType = _gH15A0HdmiAudioInfo[link_num].audioType;
							if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
								HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(link_num = %d, audioType= %d)\n", link_num,*audioType);
							break;

						default:
							*audioType = _gH15A0HdmiAudioInfo[link_num].audioType;
							break;
					}
				}

				//Check a audio type if AC-3 ES exist.
				if (*audioType == LX_HDMI_AUDIO_AC3_IEC60958)
				{
					//Check a audio type if first audio info get state for debug print.
					if (_gH15A0IntrAudioState[link_num] == HDMI_AUDIO_GET_INFO_STATE)
					{
						HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : Forced AC-3.(link_num = %d, byte0 = 0x%X, Pc = 0x%X)\n", link_num,reg_achst_byte0, reg_burst_pc_0);
					}
				}
			}
			else
			{
				//Reset _gH15A0IntrBurstInfoCount[link_num]
				_gH15A0IntrBurstInfoCount[link_num] = 0;
			}
#endif
		}
	}
	else
	{
		//Set DVI mode
		*audioType	  = LX_HDMI_AUDIO_DVI;
	}

	//HDMI_ATASK("_H15A0_GetAudioTypeAndFreq : link_num = %d, type = %d, freq = %d\n", link_num, *audioType, *samplingFreq);
	return ret;
}


/* Added 2014/08/05 taejun.lee */
static int __HDMI20_HAL_H15Ax_GetAudioInfo(UINT8 port)
{
	int ret = RET_ERROR;
	int muteInfo;
	LX_HDMI_AUDIO_INFO_T	data;
	
	do{
		if(port >= HDMI20_HAL_NUMBER_OF_LINK){
			HDMI20_ERROR("Error! Port exceeded max port size(%d)]n", port);
			break;
		}

		if(gHDMI20RxHandler[port].audioMuteEableCnt > 0) {
		
			/* Give Data */
			if(gAudioNoaudioCnt[port] <= HDMI20_HAL_H15AX_AUDIO_CHECK_CNT){
				if(lx_chip_rev( ) >= LX_CHIP_REV(H15, B0)){
					__HDMI20_HAL_H15Bx_GetAudioType(port, &data.audioType);
				}else{
					__HDMI20_HAL_H15Ax_GetAudioType(port, &data.audioType);
				}

				if(gHDMI20RxHandler[port].ctrl.bsp_read(0x27C) != 4) {
					//data.audioType = _gH15AxHdmiAudioInfo[port].audioType;
				}

				__HDMI20_HAL_H15Ax_Rx_Get_AudioFreqFromTMDSClock(port, &data.samplingFreq);
			}

			if(gAudioNoaudioCnt[port] > 0){
				/* Fifo status check*/
				if(gHDMI20RxHandler[port].ctrl.bsp_read(0x27C) > 4 || gAudioNoaudioCnt[port] == 1)hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[port].ctrl);
				gAudioNoaudioCnt[port]--;
				HDMI20_AUDIO("cnt:%d  type:%d freq:%d fifo:%d\n", gAudioNoaudioCnt[port], _gH15AxHdmiAudioInfo[port].audioType, _gH15AxHdmiAudioInfo[port].samplingFreq, gHDMI20RxHandler[port].ctrl.bsp_read(0x27C));
			}

			if(data.audioType == LX_HDMI_AUDIO_NO_AUDIO) {
				gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_CHECK_CNT;
			}else if(data.audioType != _gH15AxHdmiAudioInfo[port].audioType && _gH15AxHdmiAudioInfo[port].audioType != LX_HDMI_AUDIO_NO_AUDIO){
				gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_CHECK_CNT;
				gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
				gHDMI20RxHandler[port].isAudioMute = 1;
				HDMI20_AUDIO("Audio check cnt initialize - type change(%d -> %d) \n", _gH15AxHdmiAudioInfo[port].audioType, data.audioType);
			}
			_gH15AxHdmiAudioInfo[port].audioType = data.audioType;

			/* Added 2014/07/21 taejun.lee */
			if (_gH15AxHdmiAudioInfo[port].samplingFreq != data.samplingFreq && _gH15AxHdmiAudioInfo[port].audioType == LX_HDMI_AUDIO_PCM)
			{
				gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_CHECK_CNT;
				gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
				gHDMI20RxHandler[port].isAudioMute = 1;
				HDMI20_AUDIO("Audio check cnt initialize - fs change(%d -> %d) \n", _gH15AxHdmiAudioInfo[port].samplingFreq, data.samplingFreq);
				_gH15AxHdmiAudioInfo[port].samplingFreq = data.samplingFreq;
			}

			/* Audio Mute Control*/
			int mute_status;
			mute_status = gHDMI20RxHandler[port].ctrl.bsp_read(0x260);
			if(gAudioNoaudioCnt[port] >= HDMI20_HAL_H15AX_AUDIO_CHECK_CNT && mute_status == 1) {
				gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
				gHDMI20RxHandler[port].isAudioMute = 1;
				HDMI20_AUDIO("Port(%d) Cnt(%d) : Mute enalbe!\n", port, gAudioNoaudioCnt[port]);
			} else if(gAudioNoaudioCnt[port] == 0 && mute_status == 0 && gHDMI20RxHandler[port].isAudioMute == 0 && gHDMI20RxHandler[port].audioMuteEableCnt == HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME){
				gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x00000001);// AUD_OUTPUT_MUTE_CTRL unmute 
				HDMI20_AUDIO("Port(%d) Cnt(%d) : Mute disable!\n", port, gAudioNoaudioCnt[port]);
			}
		}

		//Audio debug
		if(gAudioDebugLevel == 1 || gAudioDebugLevel == 2){
			HDMI20_AUDIO("Port:%d MuteCnt:%d StableCnt:%d MuteSts", port, gHDMI20RxHandler[port].audioMuteEableCnt,gAudioNoaudioCnt[port], gHDMI20RxHandler[port].isAudioMute);
			HDMI20_AUDIO("AudioType:%d AudioSampfreq:%d FifoSts:%d\n", _gH15AxHdmiAudioInfo[port].audioType, _gH15AxHdmiAudioInfo[port].samplingFreq, gHDMI20RxHandler[port].ctrl.bsp_read(0x27C));
			if(gAudioDebugLevel == 1) gAudioDebugLevel = 0;
		}

		if(gVideoDebugLevel == 75001)HDMI20_AUDIO("Get info : port(%d) ARCmode(%d)", port, gHDMI20RxHandler[port].isAudioARCMode);

		// i2S mute
		if(gHDMI20RxHandler[port].ctrl.bsp_read(0x27C) != 4) {
			if(_gH15AxHdmiAudioInfo[port].audioType == LX_HDMI_AUDIO_PCM) {
				muteInfo = gHDMI20RxHandler[port].ctrl.bsp_read(0x260);
				muteInfo |= (0x000001E0);
				gHDMI20RxHandler[port].ctrl.bsp_write(0x260, muteInfo);
				gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_I2S_MUTE_CNT;
			}
		}else if(gAudioNoaudioCnt[port] == 0 || _gH15AxHdmiAudioInfo[port].audioType != LX_HDMI_AUDIO_PCM ){
			muteInfo = gHDMI20RxHandler[port].ctrl.bsp_read(0x260);
			muteInfo &= (0x00000001);
			gHDMI20RxHandler[port].ctrl.bsp_write(0x260, muteInfo);
		}
		//printk("fifo sts: %d mute sts : %x\n",gHDMI20RxHandler[port].ctrl.bsp_read(0x27C), gHDMI20RxHandler[port].ctrl.bsp_read(0x260));
		
		ret = RET_OK;

	} while(0);

	return ret;

}

static int __HDMI20_HAL_H15Ax_AudioReset(UINT8 port){
	gHDMI20RxHandler[port].audioMuteEableCnt = 0;
	gHDMI20RxHandler[port].ctrl.bsp_write(0x260, 0x000001E0);// AUD_OUTPUT_MUTE_CTRL mute
	gHDMI20RxHandler[port].isAudioMute = 1;
	//gHDMI20RxHandler[port].audio_change= 1;
	gAudioNoaudioCnt[port] = HDMI20_HAL_H15AX_AUDIO_CHECK_CNT;
	hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[port].ctrl);
}



static void __HDMI20_HAL_H15Ax_Rx_Set4k60(UINT8 port)
{
	// HDMI20RX_P0_4K_60P
	gHDMI20RxHandler[port].ctrl.bsp_write(0xFF4, 0x0000007F);// Enable Clock domain Syncronous
	msleep(1);
	gHDMI20RxHandler[port].ctrl.bsp_write(0xFF0, 0x00000042);// Software Reset
	msleep(1);

	hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);


	//; PHY SETTING
	//; Power down
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C0, 0x00000063);
	msleep(1);

	//; PHY configuration
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C0, 0x00000062);
	msleep(1);

	//;clock Measurement Unit Configuration
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x00000002);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x000022C8);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	//;CDR Control
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x0000000E);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x00000108);// ; Data rate >= 3.4Gbps
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	//;MPLL Driver Control
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x00000025);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x00000006);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	//;workaround
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x00000041);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x000001D0); //Data rate >= 3.4Gbps
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x00000061);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x000001D0);// Data rate >= 3.4Gbps
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C4, 0x00000069);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C8, 0x00000081);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2CC, 0x000001D0); //Data rate >= 3.4Gbps
	gHDMI20RxHandler[port].ctrl.bsp_write(0x2D4, 0x00000001);
	msleep(100);

	gHDMI20RxHandler[port].ctrl.bsp_write(0x2C0, 0x00000060); //PHY Power UP
	gHDMI20RxHandler[port].ctrl.bsp_write(0x000, 0x0F000C00); //HPD PORT0 ENABLE


	gHDMI20RxHandler[port].ctrl.bsp_write(0x800, 0x00000011); //; SCDC On, Auto Descrambling
	//;gHDMI20RxHandler[port].ctrl.bsp_write(0x0C0, 0x01000000 ; HDCP 1.4 bypass
	gHDMI20RxHandler[port].ctrl.bsp_write(0x81C, 0x00000002); //HDCP 2.2 Not used
	gHDMI20RxHandler[port].ctrl.bsp_write(0x090, 0x00000014);
	msleep(5000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x80000000); //Controller Sysc Reset On
	msleep(1000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x00000000); //Controller Sysc Reset Off
	msleep(1000);

	msleep(1000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x860, 0xABCDEF01); //SCDC Write Data 0 Register
	msleep(1000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x808, 0x00000001);// SCDC Configuration Register
	msleep(1000);
	gHDMI20RxHandler[port].ctrl.bsp_write(0x80C, 0x00A0c15c);
	msleep(1000);
}

static int __HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(UINT8 port, UINT8 mode)
{
	UINT32 data;
	UINT32 portValue[2] = {0x300, 0xC0};


	if(pDDCControlAddr == NULL){
		HDMI20_INFO("Err %d : %s called without initialize\n", port, __F__);
		return 0;
	}


	if(mode){
		HDMI20_INFO("Notice %d : Set GPIO Mode slave\n", port);

		data = (UINT32)(*((UINT32 *)((UINT32)pDDCControlAddr)));
		data &= (~portValue[port]);
		*pDDCControlAddr = data;
		gDDCMode[port] = 1;

	}
	else{
		HDMI20_INFO("Notice %d : Set GPIO Mode Master\n", port);
		data = (UINT32)(*((UINT32 *)((UINT32)pDDCControlAddr)));
		data &= (~portValue[port]);
		data |= (portValue[port]);
		*pDDCControlAddr = data;
		gDDCMode[port] = 0;
	}
}

/* DBG function */
static int __HDMI20_HAL_H15Ax_Rx_DBG_Tool(int type, UINT8 port, int data)
{
	int i ;
	UINT32 value,value0, value1, value2;
	struct hdmi_rx_ctrl_video v = {0};
	
	if(port> HDMI20_HAL_NUMBER_OF_LINK){
		HDMI20_ERROR("Wrong Port selected[%d]\n", port);
		return RET_ERROR;
	}

	switch(type)
	{
		case 0:
			__HDMI20_HAL_H15Ax_Rx_DBG_PrintHandler(&gHDMI20RxHandler[port]);
			break;
		case 1:
			__HDMI20_HAL_H15Ax_Rx_DBG_PrintVideo(&gHDMI20RxHandler[port]);
			break;
		case 2:
			printk(" SYNOPSYS CTRL DBG : %d\n", hdmi_rx_ctrl_debug(&gHDMI20RxHandler[port].ctrl));
			break;
		case 3:
			printk(" HDCP DEBUG \%d\n", hdmi_rx_ctrl_hdcp_debug(&gHDMI20RxHandler[port].ctrl));
			break;
		case 4:
			printk("hdmi_rx_phy_debug = %d\n", hdmi_rx_phy_debug(&gHDMI20RxHandler[port].phy));
			break;
		case 5:
			printk("HS Total : [0x%08x]\n",  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(HDMI20_RX_H15AX_AD_TMDS_HS_TOTAL));
			printk("H  Total : [0x%08x]\n",	__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port0(HDMI20_RX_H15AX_AD_TMDS_H_TOTAL));
			break;
		case 6:
			printk("HS Total : [0x%08x]\n",  __HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(HDMI20_RX_H15AX_AD_TMDS_HS_TOTAL));
			printk("H  Total : [0x%08x]\n",	__HDMI20_HAL_H15Ax_Rx_TOP_Read_Port1(HDMI20_RX_H15AX_AD_TMDS_H_TOTAL));
			break;
		case 7:
			hdmi_rx_scdc_getStatusRegisters(&gHDMI20RxHandler[port].ctrl);
			break;
		case 8:
			i = hdmi_rx_ctrl_get_video(&gHDMI20RxHandler[port].ctrl, &v, 3);
			HDMI20_HAL_H15Ax_Rx_DiffVideoData(&gHDMI20RxHandler[port].timing_info, &v);
			break;
		case 9:
			value0 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, 0x69,0x34);
			value1 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, 0x69,0x54);
			value2 = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, 0x69,0x74);

			printk(" Port [%d] : Ch0[0x%x] / Ch1[0x%x] / Ch2[0x%x]\n", port, value0, value1, value2);

			break;
		case 10:
			{
				__HDMI20_HAL_H15Ax_Rx_DBG_PrintSavedEDID(port);
			}
			break;
		case 100:
			__HDMI20_HAL_H15Ax_Rx_Init_Dev_Handler(data);
			break;
		case 101:
			//__HDMI20_HAL_H15Ax_Rx_Initialize();
			break;
		case 102:
			printk("Refresh Port[%d] TMDS = %d \n", port ,__HDMI20_HAL_H15Ax_Refresh_TMDS(&gHDMI20RxHandler[port].ctrl));
			break;
		case 103:
			printk("Refresh Port[%d] Video = %d \n", port ,__HDMI20_HAL_H15Ax_Refresh_Video(&gHDMI20RxHandler[port].ctrl));
			break;
		case 104:
			printk("Refresh Port[%d] Audio = %d \n", port ,__HDMI20_HAL_H15Ax_Refresh_Audio(&gHDMI20RxHandler[port].ctrl));
			break;
		case 105:
			printk("Refresh Port[%d] DataPacket = %d \n", port ,__HDMI20_HAL_H15Ax_Refresh_DataPacket(&gHDMI20RxHandler[port].ctrl));
			break;
		case 106 :
			printk("Refresh Port[%d] AKSV = %d \n", port ,__HDMI20_HAL_H15Ax_Refresh_AKSV(&gHDMI20RxHandler[port].ctrl));
			break;
		case 107 :
			__HDMI20_HAL_H15Ax_Rx_UnInitialize();
			break;
		case 108:
			__HDMI20_HAL_H15Ax_Tx_Initialize();
			break;
		case 811:
			gMainThreadMsleep = data;
			break;
		case 812:
			printk("Current Main thread sleep time is [%d]\n", gMainThreadMsleep);
			break;
		case 813:
			printk("Current Main thread status is [%d]\n", gMainThreadAlive);
			break;
		case 814:
			gMainThreadAlive = data;
			break;
		case 815:
			printk("gDDC[%d] = %d\n",port, gDDCMode[port]);
			break;

		case 816 :
			gVideoStableCheckCnt = data;
			gAudioNoaudioCnt[0] = data;
			gAudioNoaudioCnt[1] = data;
			break;
		case 817 :
			printk("Current  gVideoStableCheckCnt = %d\n", gVideoStableCheckCnt);
			break;
		case 820:
			printk("Current hist buffer stack cnt : %d\n", phyDBGHist.cnt);
			for(i=0; i<phyDBGHist.cnt; i++){
				printk(" PHY DBG Stack[%d] : [0x%08x][0x%08x] <= [0x%08x]\n", i, phyDBGHist.slave_addr[i], phyDBGHist.reg_addr[i], phyDBGHist.data[i]);
			}
			break;
		case 821:
			printk("Current hist buffer stack cnt : %d\n", phyAddHist[port].cnt);
			for(i=0; i<phyAddHist[port].cnt; i++){
				printk(" PHY DBG Stack[%d] : [0x%08x][0x%08x] <= [0x%08x]\n", i, phyAddHist[port].slave_addr[i], phyAddHist[port].reg_addr[i], phyAddHist[port].data[i]);
			}
			break;
		case 826:
			memset((void *)&phyDBGHist, 0, sizeof(LX_HDMI20_DBG_PHY_HISTORY_T));
			break;
		case 900:
			printk("gVideoStableCntInitial = %d\n", gVideoStableCntInitial);
			break;
		case 901:
			gVideoStableCntInitial = data;
			break;
		case 902:
			printk("gVideoStableCntNormalRun = %d\n", gVideoStableCntNormalRun);
			break;
		case 903:
			gVideoStableCntNormalRun = data;
			break;
		case 996:
			synopsys_phy_disable=0;
			break;
		case 997:
			synopsys_phy_disable=1;
			break;
		case 998:
			synopsys_disable = 0;
			break;
		case 999:
			synopsys_disable = 1;
			break;
		case 1000:
			printk(" Current Debug Level is %d\n", gVideoDebugLevel);
			break;
		case 1001:
			gVideoDebugLevel = data;
			break;
		case 1002:
			gAudioDebugLevel = data;
		break;	
		case 1100:
			printk("gHDMI20LocalMuteHoldInitTime = %d\n", gHDMI20LocalMuteHoldInitTime);
			break;
		case 1101:
			gHDMI20LocalMuteHoldInitTime = data;
			break;
		case 1102:
			printk("gHDMI20LocalMuteHoldOriginTime = %d\n", gHDMI20LocalMuteHoldOriginTime);
			break;
		case 1103:
			gHDMI20LocalMuteHoldOriginTime = data;
			break;
		case 1500:
			hdmi_rx_ctrl_packet_fifo_rst(&gHDMI20RxHandler[port].ctrl);
			break;
		case 2000:
			printk(" Current Phy reset count is %d\n", gVideoPhyResetThreshold);
			break;
		case 2001:
			gVideoPhyResetThreshold = data;
			break;
		case 2500:
			if(port == 0){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x1804, 0x1);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x1804, 0x0);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x1800, 0x1);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port0(0x1808, data);
			}
			else if(port == 1){
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x1804, 0x1);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x1804, 0x0);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x1800, 0x1);
				msleep(10);
				__HDMI20_HAL_H15Ax_Rx_TOP_Write_Port1(0x1808, data);
			}
			break;
		case 2501:
			gCrcMonitorRunMode = data;
			break;
		case 2502:
			printk("gCrcMonitorRunMode = %d\n", gCrcMonitorRunMode);
			break;
		case 2503:
			gSyncMonitorRunMode = data;
			break;
		case 2504:
			printk("gSyncMonitorRunMode = %d\n", gSyncMonitorRunMode);
			break;
		case 2505:
			gCVISyncMonitorRunMode = data;
			break;
		case 2506:
			printk("gCVISyncMonitorRunMode = %d\n", gCVISyncMonitorRunMode);
			break;
		case 3000:
			printk(" Was HDCP 1.4 port[%d] written? [%d]\n",port, gHDMI20RxHandler[port].gWasHDCP14Written);
			break;
		case 3001:
			printk(" Was HDCP 2.2 port[%d] written? [%d]\n",port, gHDMI20RxHandler[port].gWasHDCP22Written);
			break;
		case 3002:
			gHDMI20RxHandler[port].gWasHDCP14Written = data;
			break;
		case 3003:
			gHDMI20RxHandler[port].gWasHDCP22Written = data;
			break;
		case 3500:
			printk("gVideoLinkResetDelayTime = %d\n", gVideoLinkResetDelayTime);
			break;
		case 3501:
			gVideoLinkResetDelayTime = data;
			break;
		case 4000:
			__HDMI20_HAL_H15Ax_Tx_SetMode4K30Hz();
			break;
		case 4001:
			__HDMI20_HAL_H15Ax_Tx_SetMode4K60Hz();
			break;
		case 4500:
			printk("gHPDInverseMode = %d\n", gHPDInverseMode);
			break;
		case 4501:
			gHPDInverseMode = data;	
			break;
		case 5000:
			__HDMI20_HAL_H15Ax_Packet_Store(&gHDMI20RxHandler[port].ctrl, data, 1);
			break;
		case 5001:
			__HDMI20_HAL_H15Ax_Packet_Store(&gHDMI20RxHandler[port].ctrl, data, 0);
			break;
		case 5500:
			printk("gDisableAllDCMSetting = %d\n", gDisableAllDCMSetting);
			break;
		case 5501:
			gDisableAllDCMSetting = data;
			break;
		case 6000:
			hdmi_rx_scdc_setManOUI(&gHDMI20RxHandler[port].ctrl, data);
			break;
		case 6001:
			hdmi_rx_scdc_scrambling(&gHDMI20RxHandler[port].ctrl, data);
			break;
		case 6200:
			printk("gVideoPhyWriteMode = %d\n", gVideoPhyWriteMode);
			break;
		case 6201:
			gVideoPhyWriteMode = data;
			break;
		case 6500:
			__HDMI20_HAL_H15Ax_Rx_Set_EnableHPD(port ,data, 0);
			break;
		case 6666:
			HDMI20_H15A0_PrintDataPacketBuffer(data, port);
			break;
		case 6667:
			HDMI20_H15A0_ResetDataPacketBuffer(data, port);
			break;
		case 6668:
			for(i=0;i<=MPEGS;i++){
				printk(" Data Type[%d] = %s\n", i, sPacketName[i]);
			}
			break;
		case 7000:
			gPhyI2CResetWaitTime = data;
			break;
		case 7001:
			printk("Phy i2c reset time is[%d]\n", gPhyI2CResetWaitTime);
			break;
		case 7002:
			gPhyResetWaitTime = data;
			break;
		case 7003:
			printk("Phy reset time is[%d]\n", gPhyResetWaitTime);
			break;
		case 7004 :
			gLinkResetWaitTime = data;
			break;
		case 7005:
			printk("Link Video Reset Time is %d \n", gLinkResetWaitTime);
			break;
		case 7100:
			printk("gVideoPhyLostThreshold = %d\n", gVideoPhyLostThreshold);
			break;
		case 7101:
			gVideoPhyLostThreshold = data;
			break;
		case 7200:
			printk("gVideoUpdateCounterForDatapacket = %d\n", gVideoUpdateCounterForDatapacket);
			break;
		case 7201:
			gVideoUpdateCounterForDatapacket = data;
			break;
		case 7202:
			printk("gVideoEnableUpdateVideoDatapacket = %d\n", gVideoEnableUpdateVideoDatapacket);
			break;
		case 7203:
			gVideoEnableUpdateVideoDatapacket = data;
			break;
		case 7250:
			printk("gVideoVfreqMonitorThreshhold = %d\n", gVideoVfreqMonitorThreshhold);
			break;
		case 7251:
			gVideoVfreqMonitorThreshhold = data;
			break;
		case 7252:
			printk("gVideoVfreqMinValue = %d\n", gVideoVfreqMinValue);
			break;
		case 7253:
			gVideoVfreqMinValue = data;
			break;
		case 7254:
			printk("gVideoVfreqMaxValue = %d\n", gVideoVfreqMaxValue);
			break;
		case 7255:
			gVideoVfreqMaxValue = data;
			break;
		case 7300:
			printk("gHDMI20AVMuteHoldTime = %d\n", gHDMI20AVMuteHoldTime);
			break;
		case 7301:
			gHDMI20AVMuteHoldTime = data;
			break;
		case 7350:
			printk("gVideoDelayPhyInit = %d\n", gVideoDelayPhyInit);
			break;
		case 7351:
			gVideoDelayPhyInit = data;
			break;
		case 7400:
			printk("gVideoDCMDecisionCnt : %d\n", gVideoDCMDecisionCnt);
			break;
		case 7401:
			if(data >= HDMI20_HAL_H15AX_DCM_DECISION_MAX_NUM)
			{
				printk("Cannot update this value more than %d\n", HDMI20_HAL_H15AX_DCM_DECISION_MAX_NUM);
			}
			else
			{
				gVideoDCMDecisionCnt = data;
			}
			break;
		case 7500:
			printk("gHDMIDcmCtrl value is 0x%08x\n", gHDMIDcmCtrl);
			break;
		case 7501:
			gHDMIDcmCtrl = data;
			break;
		case 7550:
			printk("gVideoPhyAlterFHDValue = %d\n", gVideoPhyAlterFHDValue);
			break;
		case 7551:
			gVideoPhyAlterFHDValue = data;
			break;
		case 7600:
			gReinitHDCP14OnAksvISR = data;
			break;
		case 7601:
			printk(" Re init on ASKV ISR : %d\n", gReinitHDCP14OnAksvISR);
			break;
		case 7700:
			printk("gVideoDCMDetectModeWithGCP = %d\n", gVideoDCMDetectModeWithGCP);
			break;
		case 7701:
			gVideoDCMDetectModeWithGCP = data;
			break;
		case 7777:
			HDMI20_HAL_H15Ax_Rx_OverRideHDCP(port, 0, data);
			break;
		case 7778:
			HDMI20_HAL_H15Ax_Rx_OverRideHDCP(port, 1, data);
			break;
		case 7800:
			gPhyI2CResetMAXcount = data;
			break;
		case 7801:
			printk("Phy I2C Reset max cnt : %d\n", gPhyI2CResetMAXcount);
			break;
		case 7850:
			printk("gVideoPhyAlterFHDMode = %d \n", gVideoPhyAlterFHDMode);
			break;
		case 7851:
			gVideoPhyAlterFHDMode = data;
			break;
		case 7900:
			printk("gVideoSyncResetDelay = %d\n", gVideoSyncResetDelay);
			break;
		case 7901:
			gVideoSyncResetDelay = data;
			break;
		case 7950:
			printk("gDecreaseStableCntOnSigChange = %d\n", gDecreaseStableCntOnSigChange);
			break;
		case 7951:
			gDecreaseStableCntOnSigChange = data;
			break;
		case 8000:
			printk("gAbnormalPhyLostTime : %d\n", gAbnormalPhyLostTime);
			break;
		case 8001:
			gAbnormalPhyLostTime = data;
			break;
		case 8002:
			printk("gReinitPhyOnLockLossByCnt : %d\n", gReinitPhyOnLockLossByCnt);
			break;
		case 8003:
			gReinitPhyOnLockLossByCnt = data;
			break;
		case 8004:
			printk("gPhyResetOnAbnormalCnt : %d\n", gPhyResetOnAbnormalCnt);	
			break;
		case 8005:
			gPhyResetOnAbnormalCnt = data;
			break;
		case 8006:
			printk("gManualSyncDelayTime : %d\n", gManualSyncDelayTime);
			break;
		case 8007:
			gManualSyncDelayTime = data;
			break;
		case 8030:
			printk("gHDMI20LocalMuteHoldTime = %d\n", gHDMI20LocalMuteHoldTime);
			break;
		case 8031:
			gHDMI20LocalMuteHoldTime = data;
			break;
		case 8050:
			printk("gHDMI20DdcGpioHoldTime = %d\n", gHDMI20DdcGpioHoldTime);
			break;
		case 8051:
			gHDMI20DdcGpioHoldTime =data;
			break;
		case 8100:
			printk("gVerboseOnAbnormalVideo : %d\n", gVerboseOnAbnormalVideo);
			break;
		case 8101:
			gVerboseOnAbnormalVideo = data;
			break;
		case 8150:
			printk("gVideo2DManualSyncWaitTime : %d\n", gVideo2DManualSyncWaitTime);
			break;
		case 8151:
			gVideo2DManualSyncWaitTime = data;
			break;
		case 8280:
			__HDMI20_HAL_H15Ax_BSP_Write_Port1(0x0FF4, 0x0000007F);
			msleep(1);
			__HDMI20_HAL_H15Ax_BSP_Write_Port1(0x0FF0, 0x0000007F);
			msleep(1);
			__HDMI20_HAL_H15Ax_BSP_Write_Port1(0x090, 0x00000014);
			break;
		case 8281:
			__HDMI20_HAL_H15Ax_BSP_Write_Port0(0x0FF4, 0x0000007F);
			msleep(1);
			__HDMI20_HAL_H15Ax_BSP_Write_Port0(0x0FF0, 0x0000007F);
			msleep(1);
			__HDMI20_HAL_H15Ax_BSP_Write_Port0(0x090, 0x00000014);
			msleep(1);
			break;
		case 8200:
			printk("Phy FHD value = 0x%08x\n", gVideoPhyFHDValue);
			break;
		case 8201:
			gVideoPhyFHDValue = data;
			break;
		case 8202:
			printk("Phy FHD DBG mode value = 0x%08x\n", gVideoPhyFHDDBGMode);
			break;
		case 8203:
			gVideoPhyFHDDBGMode = data;
			break;
		case 8204:
			printk("UHD Phy DBG mode value = 0x%08x\n", gVideoPhyUHDValue);
			break;
		case 8205:
			gVideoPhyUHDValue = data;
			break;
		case 8206:
			printk("Phy UHD DBG mode value = 0x%08x\n", gVideoPhyUHDDBGMode);
			break;
		case 8207:
			gVideoPhyUHDDBGMode = data;
			break;
		case 8250:
			printk("Phy BCAPS control = 0x%x\n", gHDCPBcapsCtrlValue);
			break;
		case 8251:
			gHDCPBcapsCtrlValue = data;
			break;
		case 8260:
			printk("gVideoLinkResetMode = %d\n", gVideoLinkResetMode);
			break;
		case 8261:
			gVideoLinkResetMode = data;
			break;
		case 8282:
			__HDMI20_HAL_H15Ax_Rx_ResetSyncPosition(port);
			break;
		case 8300:
			gAbnormalVideoResetTime = data;
			break;
		case 8301:
			printk("gAbnormal Video Reset time is :%d\n", gAbnormalVideoResetTime);
			break;
		case 8450:
			printk("gVideoUseGCPPacketForDCM = %d\n", gVideoUseGCPPacketForDCM);
			break;
		case 8451:
			gVideoUseGCPPacketForDCM = data;
			break;
		case 8500:
			__HDMI20_HAL_H15Ax_UpdateDCMMode(port, data, 1);
			break;
		case 8800:
			printk("gPhyAutoEqStatusAddr = 0x%x\n", gPhyAutoEqStatusAddr);
			break;
		case 8801:
			gPhyAutoEqStatusAddr = data;
			break;
		case 8802:
			printk("gPhyAutoEqStatusdata = 0x%x\n", gPhyAutoEqStatusdata);
			break;
		case 8803:
			gPhyAutoEqStatusdata = data;
			break;
		case 8804:
			printk("gPhyAutoEqStatusMask = 0x%x\n", gPhyAutoEqStatusMask);
			break;
		case 8805:
			gPhyAutoEqStatusMask = data;
			break;
		case 8900:
			printk("gHDMI20stableHDCP14HoldTime = %d\n", gHDMI20stableHDCP14HoldTime);
			break;
		case 8901:
			gHDMI20stableHDCP14HoldTime = data;
			break;
		case 8950:
			printk("gVideoUseSCDCIsr = %d\n", gVideoUseSCDCIsr);
			break;
		case 8951:
			gVideoUseSCDCIsr = data;
			break;
		case 9000:
			printk("gForcedHDMIMode is : %d\n", gForcedHDMIMode);
			break;
		case 9001:
			gForcedHDMIMode	= data;
			break;
		case 9020:
			printk("gVideoResetESMOnResolutionChange = %d\n", gVideoResetESMOnResolutionChange);
			break;
		case 9021:
			gVideoResetESMOnResolutionChange = data;
			break;
		case 9050:
			printk("gLinkResetModeDetectionMode = %d\n", gLinkResetModeDetectionMode);
			break;
		case 9051:
			gLinkResetModeDetectionMode = data;
			break;
		case 9100:
			printk("gVideoClearAVMuteByTimer = %d\n", gVideoClearAVMuteByTimer);
			break;
		case 9101:
			gVideoClearAVMuteByTimer = data;
			break;
		case 9200:
			printk("gVideoDCMUpdateOnVideoChange = %d\n", gVideoDCMUpdateOnVideoChange);
			break;
		case 9201:
			gVideoDCMUpdateOnVideoChange = data;
			break;
		case 9250:
			printk("gVideoResetManualOnVfreqChange = %d\n", gVideoResetManualOnVfreqChange);
			break;
		case 9251:
			gVideoResetManualOnVfreqChange = data;
			break;
		case 9300:
			printk("gVideoTMDSTolerence = %d\n", gVideoTMDSTolerence);
			break;
		case 9301:
			gVideoTMDSTolerence = data;
			break;
		case 9320:
			printk("gVideoDebugAmountOfHDCP14Reset = %d\n", gVideoDebugAmountOfHDCP14Reset);
			break;
		case 9321:
			gVideoDebugAmountOfHDCP14Reset = data;
			break;
		case 9350:
			printk("gVideoUsdeManualSyncOnFHD = %d\n", gVideoUseManualSyncOnFHD);
			break;
		case 9351:
			gVideoUseManualSyncOnFHD = data;
			break;
		case 9400:
			printk("gUseVideoChangeFlagOnMainThread = %d\n", gUseVideoChangeFlagOnMainThread);
			break;
		case 9401:
			gUseVideoChangeFlagOnMainThread = data; 
			break;
		case 9450:
			printk("gVideoFujiFilmThresHold = %d\n", gVideoFujiFilmThresHold);
			break;
		case 9451:
			gVideoFujiFilmThresHold = data;
			break;
		case 9500:
			printk("gVideoBlackoutonTimingInfoZero = %d\n", gVideoBlackoutonTimingInfoZero);
			break;
		case 9501:
			gVideoBlackoutonTimingInfoZero = data;
			break;
		case 9530:
			printk("gVideoResetSyncOnSurge = %d\n", gVideoResetSyncOnSurge);
			break;
		case 9531:
			gVideoResetSyncOnSurge = data;
			break;
		case 9550:
			__HDMI20_HAL_h15Ax_Rx_SetDDCGPIOMode(port, data);
			break;
		case 9600:
			printk("gAbnormalPhyLostClearTime = %d\n", gAbnormalPhyLostClearTime);
			break;
		case 9601:
			gAbnormalPhyLostClearTime = data;
			break;
		case 9630:
			printk("gVideoSurgeDetectThreshold = %d\n", gVideoSurgeDetectThreshold);
			break;
		case 9631:
			gVideoSurgeDetectThreshold = data;
			break;
		case 9650:
			printk("gVideoEnableManualSync = %d\n", gVideoEnableManualSync);
			break;
		case 9651:
			gVideoEnableManualSync = data;
			break;
		case 9700:
			printk("gVideoDisableDCMChange = %d\n", gVideoDisableDCMChange);
			break;
		case 9701:
			gVideoDisableDCMChange = data;
			break;
		case 9992:
			printk("HDMI1.4 PWR down and UP\n");
			hdmi_rx_ctrl_hdcp_reset(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
			hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
			break;
		case 9993:
			printk(" Fixed Pll set mode is : %d\n", gFixedPLLMode );
			break;
		case 9994:
			gFixedPLLMode = data;
			break;
		case 9995:
			printk("Fixed SCDC value to [0x%08x]\n", gFixedScdcValue);
			break;
		case 9996:
			gFixedScdcValue = data;
			break;
		case 9997:
			hdmi_rx_ctrl_hdcp_config(&gHDMI20RxHandler[port].ctrl, &gHDMI20RxHandler[port].hdcp);
			break;
		case 9998:
			printk("Initiate Phy = %d\n", HDMI20_HAL_H15Ax_Rx_Initiate_Phy(port, data));
			break;
		case 9999:
			printk("Reset LinkVideo\n");
			HDMI20_HAL_H15Ax_Rx_LinkVideoReset(port, data);
			break;
		case 14000:
			printk("Current SRC HDMI2.0 Capability is [%d]\n", gHDMI20RxHandler[port].isHDMI20Device);

			break;
		case 14001:
			printk("Trigger checking SRC HDMI2.0 Capability \n");
			 __HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(port, 0);
			break;
		case 14002:
			printk("Reset SRC HDMI2.0 Capability \n");
			__HDMI20_HAL_H15Ax_Rx_CheckSRCHDMI20Capability(port, 1);
			break;
		case 14003:
			printk("Current SRC Capability check addr is 0x%08x\n", gHDMI20CapabilityAddr);
			break;
		case 14004:
			gHDMI20CapabilityAddr = data;
			break;
		case 14005:
			printk("Current SRC Capability check value is 0x%08x\n", gHDMI20CapabilityCheck);
			break;
		case 14006:
			gHDMI20CapabilityCheck = data;
			break;
		case 15000:
			printk("gHDMIMode recover value is = 0x%08x \n",  gHDMIModeRecover);
			break;
		case 15001:
			gHDMIModeRecover = data;
			break;
		case 15002:
			printk("gHDMI Vertical Timing is = 0x%08x\n", gHDMIVerticalTiming);
			break;
		case 15003:
			gHDMIVerticalTiming = data;
			break;
		case 16000:
			HDMI20_REG_WR(0xC86040A4, 0x08500000);
			HDMI20_REG_WR(0xC8604084, 0x0C004820);
			break;
			/* Added 2014/07/02 taejun.lee */
		case 16001:
			HDMI20_REG_WR(0xC86040A4, 0x08700000);
			HDMI20_REG_WR(0xC8604084, 0x0C008820);
			break;
		case 16111:
			printk("Tx Audio setting\n");
			__HDMI20_HAL_H15Ax_TX_AUD_I2S_SETTING();
			__HDMI20_HAL_H15Ax_TX_AUD_FC();
			break;
		case 20000:
			__HDMI20_HAL_H15Ax_UpdateTimingInfo(&gHDMI20RxHandler[port].ctrl, data);
			if(data == 0)__HDMI20_HAL_H15Ax_SetVideoColorDomain(i);

			break;
		case 20001:
			__HDMI20_HAL_H15Ax_UpdateAudioCtsNparam(port, data);
			break;
		case 30000:
			hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, data);
			break;
		case 30008:
			hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x200);
			break;
		case 30010:
			hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x220);
			break;
		case 30012:
			hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x240);
			break;
		case 30016:
			hdmi_rx_ctrl_phy_write(&gHDMI20RxHandler[port].ctrl, 0x69, 0x3, 0x260);
			break;
		case 40000:
			gNumOfRunningHDMI = data;
			break;
		case 40001:
			printk("Num of running HDMI : %d\n", gNumOfRunningHDMI);
			break;
		case 42424:
			gLinkResetValue = data;
			break;
		case 42425:
			printk("gLinkResetValue is 0x%08x\n",gLinkResetValue);
			break;
		case 50000:
			gLGEPhyAPISleepTime = data;
			break;
		case 50001:
			printk("LGE Phy API Sleep time : %d\n", gLGEPhyAPISleepTime);
			break;
		case 66666:
			printk("Force Suspend on HDMI2.0\n");
			HDMI20_H15A0_Suspend();
			break;
		case 66667:
			printk("Force Resume on HDMI2.0\n");
			HDMI20_H15A0_Resume();
			break;
		case 68000:
			printk("gPhyLinkResetMode = %d\n", gPhyLinkResetMode);
			break;
		case 68001:
			gPhyLinkResetMode = data;	
			break;
		case 70000:
			gHDMI20RxHandler[port].ctrl.bsp_write(0x17C, 0x4); // Enable
			gHDMI20RxHandler[port].ctrl.bsp_write(0x404, 0x3); // Mode
			gHDMI20RxHandler[port].ctrl.bsp_write(0x408, 0x01180780); // Hsync fp, active
			gHDMI20RxHandler[port].ctrl.bsp_write(0x40c, 0x0058002c); // Hsync width, delay
			gHDMI20RxHandler[port].ctrl.bsp_write(0x414, 0x00020005); // Vsync width delay,
			gHDMI20RxHandler[port].ctrl.bsp_write(0x410, 0x0016021c); // Vsync fp, active

			gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x8000003d);
			msleep(1);
			gHDMI20RxHandler[port].ctrl.bsp_write(0x400, 0x0000003d);

			break;
		case 75000:
			gHDMI20RxHandler[port].isAudioARCMode = data;	
			break;
		case 75001:
			printk("gEnableARCControl[%d] = %d\n" ,port, gHDMI20RxHandler[port].isAudioARCMode);
			break;
		case 77777:
			__HDMI20_HAL_H15Ax_TestPattern(port, 0, data);
			break;
		case 77778:
			__HDMI20_HAL_H15Ax_TestPattern(port, 1, data);
			break;
		case 79998:
			__HDMI20_HAL_H15Ax_Rx_Audio_Configure(port, data);
			break;

		case 79999:
			hdmi_rx_ctrl_audio_fifo_rst(&gHDMI20RxHandler[port].ctrl);
			break;
		case 80000:
			/* Test : UART2 to HDCP22 */
			printk("Switched UART2 to HDCP22 port 0\n");
			HDMI20_REG_WR(0xC8601084, 0x22908000);
			break;
		case 82820:
			value = hdmi_rx_ctrl_phy_read(&gHDMI20RxHandler[port].ctrl, 0x69,(UINT8)data);
			printk("Phy read for slave[0x69] RegAddr[0x%x] => [0x%08x]\n", data,  value);
			break;
		case 88888:
			__HDMI20_HAL_H15Ax_Rx_Set4k60(port);
			break;
		case 99999:
			__HDMI20_HAL_H15Ax_ClearChangeFlags(port);
			break;

		default :
			break;
	}

	return RET_OK;
}


