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
 * main driver implementation for HDMI device.
 * HDMI device will teach you how to make device driver with new platform.
 *
 * author     sunghyun.myoung (sh.myoung@lge.com)
 * version    1.0
 * date       2010.02.19
 * note       Additional information.
 *
 * @addtogroup lg115x_hdmi
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/workqueue.h>/**< For working queue */
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/div64.h> 		//do_div
#include <linux/kthread.h>
#include <linux/wait.h>		/**< Wait queue */

#include "os_util.h"
#include "hdmi_hw_h14.h"
#include "hdmi_reg_h14.h"
#include "sys_regs.h"


/******************************************************************************
 *				DEFINES
 *****************************************************************************/
#define u16CHK_RANGE(X,Y,offset)		((UINT16) ((X) + (offset) - (Y)) <= (offset*2))
#define TBL_NUM(X)						(sizeof(X) /sizeof((X)[0]))

#define CHK_UHD_BD()	(lx_chip_plt() != LX_CHIP_PLT_FHD)
#define CHK_FHD_BD()	(lx_chip_plt() == LX_CHIP_PLT_FHD)

/**
 *	Global variables of the driver
 */
extern LX_HDMI_REG_T *pH14HdmiRegCfg;
extern HDMI_DATA_T *gH14BootData;

static HDMI_PORT_CONNECTION_TYPE_T _gH14HDMIConnectState = HDMI_PORT_NOT_CONNECTED;
static LX_HDMI_TIMING_INFO_T _gH14PrevTiming = {0,};
static UINT32 _gH14TimingReadCnt = 0;
static UINT32 _gH14HDMIState = HDMI_STATE_DISABLE;

static LX_HDMI_AVI_COLORSPACE_T	_gH14PrevPixelEncoding = LX_HDMI_AVI_COLORSPACE_RGB;
static LX_HDMI_INFO_PACKET_T _gH14PrevAVIPacket = {0, };
static LX_HDMI_INFO_PACKET_T _gH14PrevVSIPacket = {0, };
static BOOLEAN _gH14AVIReadState 	  = FALSE;
static BOOLEAN _gH14VSIState 	  	  = FALSE;
static BOOLEAN _gH14AVIChangeState 	  = FALSE;

//audio
static BOOLEAN _gH14AudioMuteState 	  = FALSE;
static BOOLEAN _gH14AudioArcStatus 	  = FALSE;	//This variable is used ARC Enabled Status to recover ARC mode after PHY reset.
static UINT32  _gH14IntrAudioState 	  = HDMI_AUDIO_INIT_STATE;
static UINT32  _gH14HdmiPortStableCount  = 0;	//This count is used to detect if HDMI Switch port is changed.
static UINT32  _gH14HdmiFreqErrorCount   = 0;	//This count is used to detect if HDMI TMDS Frequency and Frequency is different.
static UINT32  _gH14HdmiAudioThreadCount = 0;	//_gH14HdmiAudioThreadCount is increased in _HDMI_H14_Periodic_Audio_Task function
static UINT64  _gH14IntrBurstInfoCount	 = 0;	//This count is used to detect if burst info. interrupt is asserted count.
static UINT64  _gH14TaskBurstInfoCount	 = 0;	//This count is used to save _gH14IntrBurstInfoCount in hdmi task.
static UINT64  _gH14BurstInfoPrevJiffies = 0;	//This Jiffies is used to save previous burst info. interrupt jiffies.
static LX_HDMI_AUDIO_INFO_T _gH14HdmiAudioInfo = {LX_HDMI_AUDIO_DEFAULT, LX_HDMI_SAMPLING_FREQ_NONE};

//interrupt variables
static UINT32 _gH14Intr_vf_stable = HDMI_VIDEO_INIT_STATE;
static UINT32 _gH14Intr_avi = HDMI_AVI_INIT_STATE;
static UINT32 _gH14Intr_src_mute = HDMI_SOURCE_MUTE_CLEAR_STATE;
static UINT32 _gH14Intr_vsi = HDMI_VSI_INIT_STATE;

static UINT32 _gH14Force_thread_sleep  = 0;	//enable  = 2, disable  =0
static UINT32 _gH14HDMI_thread_running = 1;	//enable  = 1, disable  =0
static UINT32 _gH14HDMI_interrupt_registered = 0;

static UINT32 _gH14ChipPlatform = 0;

//wait_queue_head_t	WaitQueue_HDMI;
DECLARE_WAIT_QUEUE_HEAD(WaitQueue_HDMI_H14);

DEFINE_SPINLOCK(_gIntrHdmiH14VideoLock);
DEFINE_SPINLOCK(_gHdmiH14RegReadLock);
//HDMI Audio Variables
DEFINE_SPINLOCK(_gIntrHdmiH14AudioLock);

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

	{	3840,	2160,	1,		3840, 	2160, 	1,		LX_HDMI_EXT_4K_2K},			//3840x2160
	{	4096,	2160,	1,		4096, 	2160, 	1,		LX_HDMI_EXT_4K_2K},			//4096x2160
};

static const sO_ENUM EXT_3D_TYPE[ ] =
{	/// LX_HDMI_VSI_3D_STRUCTURE_T 						LX_HDMI_EXT_FORMAT_INFO_T
	{	LX_HDMI_VSI_3D_STRUCTURE_FRAME_PACKING,			LX_HDMI_EXT_3D_FRAMEPACK},
	{	LX_HDMI_VSI_3D_STRUCTURE_FIELD_ALTERNATIVE,		LX_HDMI_EXT_3D_FIELD_ALTERNATIVE},
	{	LX_HDMI_VSI_3D_STRUCTURE_LINE_ALTERNATIVE,		LX_HDMI_EXT_3D_LINE_ALTERNATIVE},
	{	LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_FULL,		LX_HDMI_EXT_3D_SBSFULL},
	{	LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH,				LX_HDMI_EXT_3D_L_DEPTH},
	{	LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH_GRAPHICS,		LX_HDMI_EXT_3D_L_GRAPHICS},
	{	LX_HDMI_VSI_3D_STRUCTURE_TOP_BOTTOM,			LX_HDMI_EXT_3D_TNB},
	{	LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_HALF,		LX_HDMI_EXT_3D_SBS},
	{	LX_HDMI_VSI_3D_STRUCTURE_MAX,					LX_HDMI_EXT_2D_FORMAT},
};

/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */
static struct task_struct	*stHDMI_Thread;

PHY_REG_CTRL_H14_T  gPHY_REG_CTRL_H14;
static volatile HDMI_LINK_REG_H14_T *pstLinkReg;
static volatile HDMI_LINK_REG_H14_T *pstLinkShadowReg;


/******************************************************************************
 *				Local function
 *****************************************************************************/
static int _HDMI_H14_GetExtendTimingInfo(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock);
static int _HDMI_H14_ClearSWResetAll(void);			//H14D Chip - Ctop control
static int _HDMI_H14_PhyRunReset(void);
static int _HDMI_H14_RunReset(void);
static int _HDMI_H14_GetPortConnection(void);
static int _HDMI_H14_SetInternalMute(LX_HDMI_MUTE_CTRL_T interMute);
static int _HDMI_H14_SetVideoBlank(LX_HDMI_AVI_COLORSPACE_T space);
static int _HDMI_H14_EnableInterrupt(void);
static int _HDMI_H14_Thread(void *data);
irqreturn_t _HDMI_H14_IRQHandler(int irq, void *dev);
static void _HDMI_H14_Periodic_Task(void);
#ifdef NOT_USED_NOW
static void _HDMI_H14_ModifyUDFormat(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock);
#endif	//#ifdef NOT_USED_NOW
static void _HDMI_H14_CheckUDExtInfo(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock);

//audio related.
static int _HDMI_H14_SetAudio(void);
static int _HDMI_H14_GetAudioInfo(void);
static int _HDMI_H14_GetAudioTypeAndFreq(LX_HDMI_AUDIO_TYPE_T *audioType, LX_HDMI_SAMPLING_FREQ_T *samplingFreq);
static int _HDMI_H14_GetAudioFreqFromTmdsClock(LX_HDMI_SAMPLING_FREQ_T *samplingFreqFromTmds);
static int _HDMI_H14_DebugAudioInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo);

int HDMI_H14_HWInitial(void)
{
	gPHY_REG_CTRL_H14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(PHY_REG_H14_T));

#ifndef	USE_QEMU_SYSTEM
	pstLinkReg =(HDMI_LINK_REG_H14_T *)ioremap(pH14HdmiRegCfg->link_reg_base_addr, pH14HdmiRegCfg->link_reg_size);
#else
	pstLinkReg =(HDMI_LINK_REG_H14_T *)ioremap(pH14HdmiRegCfg->link_qemu_base_addr, pH14HdmiRegCfg->link_reg_size);
#endif

	HDMI_PRINT("HDMI LINK REG CFG \n");
	HDMI_PRINT("LINK REG Base Address	0x%08x\n", pH14HdmiRegCfg->link_reg_base_addr);
	HDMI_PRINT("LINK REG Size 			0x%08x\n", pH14HdmiRegCfg->link_reg_size);
	HDMI_PRINT("LINK REG Qumu Address 	0x%08x\n", pH14HdmiRegCfg->link_qemu_base_addr);

	if (pstLinkReg == NULL)
	{
		HDMI_ERROR("ERROR : can't allocate for register\n");
		return RET_ERROR;
	}

	pstLinkShadowReg = (HDMI_LINK_REG_H14_T *)kmalloc(sizeof(HDMI_LINK_REG_H14_T), GFP_KERNEL);
	if (pstLinkShadowReg == NULL)
	{
		HDMI_ERROR("ERROR : can't allocate for shadow register\n");
		return RET_ERROR;
	}

	_gH14ChipPlatform = lx_chip_plt();

	//*  2012-06-15 : HDMI Initail 주의 사항
	//*  Ctop reset 이후 phy pdb & restn 동작 해야 함(Ctop 에서 phy pdb만 하는 것 같음.
	//*  After phy pdb, phy register initi value 가 오염됨 -> pdb 후에는 rstn reset해주어야 함.
	//*  Phy pdb & restn 하면 phy register initi value로 변경 됨,  다시 phy register setting 해야 함.
	//*
	_HDMI_H14_ClearSWResetAll();			//H14D Chip - Ctop control

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_enable, 0x1);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
	LINK_H14_Wr01(phy_00, phy_rstn, 0x0);			///< Phy Enable
	LINK_H14_Wr01(phy_00, phy_arc_pdb, 0x0);			///< Phy Enable
	LINK_H14_WrFL(phy_00);

	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x0);
	LINK_H14_WrFL(phy_00);

	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
	LINK_H14_WrFL(phy_00);
	/* PHY_PDB=1, PHY_RSTN=1 을 동시에 인가 register들이 제대로 pre-set되지 않는 현상 발생함. */
	/* PHY_PDB=1  -> 10us  -> PHY_RSTN=1  */
	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_rstn, 0x1);			///< Phy Enable
	LINK_H14_WrFL(phy_00);

	_HDMI_H14_PhyRunReset();	// init Phy register
	_HDMI_H14_RunReset();		// init Link register

	//audio setting
	_HDMI_H14_SetAudio();

	_HDMI_H14_EnableInterrupt();

	stHDMI_Thread = kthread_create(_HDMI_H14_Thread, (void*)NULL, "hdmi_thread");
	if (stHDMI_Thread)
	{
		wake_up_process(stHDMI_Thread);
		HDMI_DEBUG("HDMI Thread [%d]\n", stHDMI_Thread->pid);
	}
	else
	{
		HDMI_ERROR("HDMI Thread Already Created\n");
	}

	HDMI_H14_SetPowerControl(0);

	HDMI_TRACE("H14 HDMI Initialize \n" );
	return RET_OK;
}

int HDMI_H14_SetPort(UINT32 *port)
{
	return RET_ERROR;
}

int HDMI_H14_Thread_Control(int sleep_enable)
{
	if (sleep_enable)
	{
		_gH14Force_thread_sleep = 2;
		_gH14HDMIState = HDMI_STATE_DISABLE;
		HDMI_DEBUG("H14 sleeping HDMI Thread \n");
	}
	else
	{
		_gH14HDMIState = HDMI_STATE_IDLE;
		_gH14Force_thread_sleep = 0;
		wake_up_interruptible(&WaitQueue_HDMI_H14);
		_gH14HDMI_thread_running = 1;
		HDMI_DEBUG("H14 Starting HDMI Thread\n");
	}

	return RET_OK;
}

int HDMI_H14_GetMode(LX_HDMI_MODE_T *mode)
{
	mode->bHDMI =  gH14BootData->mode;

	HDMI_PRINT("[%d] %s : Mode = [%s] \n", __L__, __F__,  mode->bHDMI? "HDMI":"DVI");
	return RET_OK;
}

int HDMI_H14_GetAspectRatio(LX_HDMI_ASPECTRATIO_T *ratio)
{
	ratio->eAspectRatio = (((_gH14PrevAVIPacket.dataBytes[0] &0xff0000)>>16) &0x30)>>4;		// M1M0

	HDMI_PRINT("[%d] %s : [0] NoData, [1]4:3, [2]16:9 [3] Future  ==> [%d] \n", __L__, __F__, ratio->eAspectRatio);
	return RET_OK;
}

int HDMI_H14_GetColorDomain(LX_HDMI_COLOR_DOMAIN_T *color)
{
	UINT32 temp = 0;
	memset((void *)color , 0 , sizeof(LX_HDMI_COLOR_DOMAIN_T));

	if (_gH14Intr_vf_stable == HDMI_VIDEO_INIT_STATE || _gH14Intr_avi == HDMI_AVI_INIT_STATE)
	{
		return RET_OK;
	}

	if (_gH14Intr_avi > HDMI_AVI_INIT_STATE)
	{
		color->bHdmiMode =  gH14BootData->mode;

		if (color->bHdmiMode)
		{
			if (_gH14AVIReadState)
				temp = _gH14PrevAVIPacket.dataBytes[0];
			else
			{
				LINK_H14_RdFL(packet_19);
				temp = LINK_H14_Rd(packet_19);
			}

			color->ePixelEncoding = (((temp &0xff00)>>8) &0x60)>>5;				// Y1Y0
			color->eColorimetry = (((temp &0xff0000)>>16) &0xc0)>>6;			// C1C0
			color->eExtColorimetry = (((temp &0xff000000)>>24) &0x70)>>4;		// EC2EC1EC0
			color->eITContent = (((temp &0xff000000)>>24) &0x80)>>7;			// ITC
			color->eRGBQuantizationRange = (((temp &0xff000000)>>24) &0x0c)>>2;	// Q1Q0
		}
	}

	return RET_OK;
}

int HDMI_H14_GetTimingInfo(LX_HDMI_TIMING_INFO_T *info)
{
	UINT32 tmp32 = 0;
	UINT32 tmdsClock = 0,	tmpClock = 0;
	UINT32 up_freq = 0,	down_freq = 0;
	UINT32 pixelRepet = 0, colorDepth = 0;

	LX_HDMI_TIMING_INFO_T 	bufTiming;

	memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));

	do{
		if (_gH14HDMI_thread_running == 1 && _gH14HDMIState < HDMI_STATE_READ)
			break;

		if (_gH14HDMI_thread_running == 1 && _gH14HDMIState == HDMI_STATE_STABLE)
			goto func_exit;

		LINK_H14_RdFL(video_04);
		LINK_H14_Rd01(video_04, reg_h_tot, bufTiming.hTotal);		///< Horizontal total pixels
		LINK_H14_Rd01(video_04, reg_v_tot, bufTiming.vTotal);		///< Vertical total lines

		LINK_H14_RdFL(video_05);
		LINK_H14_Rd01(video_05, reg_h_av, bufTiming.hActive);		///< Horizontal active pixel
		LINK_H14_Rd01(video_05, reg_v_av, bufTiming.vActive);		///< Vertical active lines
#if 0
		LINK_H14_RdFL(video_06);
		LINK_H14_Rd01(video_06, reg_h_fp, bufTiming.hStart);		///< Horizontal start pixel (Front Porch)
		LINK_H14_Rd01(video_06, reg_v_fp, bufTiming.vStart);		///< Vertical start lines (Front Porch)
#endif
		LINK_H14_RdFL(video_07);
		LINK_H14_Rd01(video_07, reg_h_bp, bufTiming.hStart);	///< Horizontal start pixel (Back Porch)
		LINK_H14_Rd01(video_07, reg_v_bp, bufTiming.vStart);	///< Vertical start lines (Back Porch)

		LINK_H14_RdFL(video_10);
		LINK_H14_Rd01(video_10, reg_intrl, bufTiming.scanType);
		bufTiming.scanType^=1; 							///< Scan type (0 : interlace, 1 : progressive) 	info->scanType ^= 1;

		bufTiming.extInfo = 0; 	///< Full 3D Timing

		///* pixel clock *///
		PHY_REG_H14_RdFL(tmds_freq_1);
		PHY_REG_H14_RdFL(tmds_freq_2);

		PHY_REG_H14_Rd01(tmds_freq_1, tmds_freq, up_freq);
		PHY_REG_H14_Rd01(tmds_freq_2, tmds_freq, down_freq);
		tmdsClock = ((up_freq << 8) + down_freq); 	// XXX.XX KHz
#if 0 // Check UD format detection Issue
		if (CHK_UHD_BD())
			_HDMI_H14_ModifyUDFormat(&bufTiming, tmdsClock);		// modify 297MHz format
#endif
		_HDMI_H14_GetExtendTimingInfo(&bufTiming, tmdsClock);		// get extend information

		LINK_H14_RdFL(video_02);
		LINK_H14_Rd01(video_02, reg_cmode_tx, colorDepth);
		colorDepth = (colorDepth &0x60)>>5;

		if ( (bufTiming.extInfo == LX_HDMI_EXT_4K_2K) && (colorDepth > 0))
		{
			HDMI_VIDEO("[%d] %s : not support 4Kx2K color bit over 8bit[%d]\n", __L__, __F__,colorDepth);
			memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_cnt;
		}

		// InValid Format Check
		if ( (bufTiming.hActive > 4100) || (bufTiming.vActive > 2415) ||
			(bufTiming.hActive < 320) || (bufTiming.vActive < 240) )
		{
			//memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
			HDMI_VIDEO("[%d] %s : InValid Format for Active Size,  hActive[%d] vActive [%d]\n", __L__, __F__, bufTiming.hActive, bufTiming.vActive);
			goto func_exit;
		}

		//* Support 2D 1280x1024i@86  by 20120202
		//* Not support Master #333 - because MASTER timing issue
		if (bufTiming.hActive == 1280 && bufTiming.vActive == 512 && bufTiming.scanType == 1)
		{
			HDMI_VIDEO("[%d] %s : not support 2D 1280x1024!@86 of Master #333\n", __L__, __F__);
			memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_cnt;
		}

		//_HDMI_H14_GetExtendTimingInfo(&bufTiming, tmdsClock);		// get extend information

		if (bufTiming.vActive == 0 || bufTiming.hActive == 0 )
		{
			HDMI_PRINT("[%d] %s : _HDMI_H14_GetExtendTimingInfo hActive, vActive  = 0\n", __L__, __F__);
			//memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_exit;
		}

		//* Support 2D 1920x1080i@50 of EIA-861D  for any PC Card  by 20111010
		//* Not support Master #840 - because MASTER timing issue
		if (bufTiming.hTotal == 2304)// && bufTiming.hActive == 1920 && bufTiming.scanType == 1)
		{
			if (bufTiming.vTotal == 1250)
			{
				HDMI_VIDEO("[%d] %s : Support 2D 1920x1080!@50 of EIA-861D \n", __L__, __F__);
			}
			else
			{
				HDMI_VIDEO("[%d] %s : not support 2D 1920x1080!@50 of EIA-861D Master #840\n", __L__, __F__);
				memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
				goto func_cnt;
			}
		}

		if (bufTiming.hActive %2 == 1)
		{
			HDMI_VIDEO("[%d] %s : hActive modification [%d]\n", __L__, __F__, bufTiming.hActive);
			if ( u16CHK_RANGE(bufTiming.hActive, 4096, 2) )		bufTiming.hActive = 4096;
			else if ( u16CHK_RANGE(bufTiming.hActive, 3840, 2) )	bufTiming.hActive = 3840;
			else if ( u16CHK_RANGE(bufTiming.hActive, 1920, 2) )	bufTiming.hActive = 1920;
			else if ( u16CHK_RANGE(bufTiming.hActive, 1440, 2) )	bufTiming.hActive = 1440;
			else if ( u16CHK_RANGE(bufTiming.hActive, 1280, 2) )	bufTiming.hActive = 1280;
			else if ( u16CHK_RANGE(bufTiming.hActive, 720, 2) )		bufTiming.hActive = 720;
			else if ( u16CHK_RANGE(bufTiming.hActive, 640, 2) )		bufTiming.hActive = 640;
		}

		if (bufTiming.vActive %2 == 1)
		{
			HDMI_VIDEO("[%d] %s : vActive modification [%d]\n", __L__, __F__, bufTiming.vActive);
			if ( u16CHK_RANGE(bufTiming.vActive, 2160, 2) )		bufTiming.vActive = 2160;
			else if ( u16CHK_RANGE(bufTiming.vActive, 1080, 2) )	bufTiming.vActive = 1080;
			else if ( u16CHK_RANGE(bufTiming.vActive, 720, 2) )		bufTiming.vActive = 720;
			else if ( u16CHK_RANGE(bufTiming.vActive, 576, 2) )		bufTiming.vActive = 576;
			else if ( u16CHK_RANGE(bufTiming.vActive, 480, 2) )		bufTiming.vActive = 480;
			else if ( u16CHK_RANGE(bufTiming.vActive, 288, 2) )		bufTiming.vActive = 288;
			else if ( u16CHK_RANGE(bufTiming.vActive, 240, 2) )		bufTiming.vActive = 240;
		}

		///* Start  Horizontal & Vertical frequency *///
#if 0
		PHY_REG_H14_RdFL(tmds_freq_1);
		PHY_REG_H14_RdFL(tmds_freq_2);

		PHY_REG_H14_Rd01(tmds_freq_1, tmds_freq, up_freq);
		PHY_REG_H14_Rd01(tmds_freq_2, tmds_freq, down_freq);
		tmdsClock = ((up_freq << 8) + down_freq); 	// XXX.XX KHz
#endif
		LINK_H14_RdFL(video_00);
		LINK_H14_Rd01(video_00, reg_pr_tx, pixelRepet);

		tmpClock = tmdsClock * 1000;

		if (tmdsClock != 0 && bufTiming.vTotal > 0 && bufTiming.hTotal > 0 )		// XXX.XX KHz
		{
			//LINK_H14_RdFL(video_02);
			//LINK_H14_Rd01(video_02, reg_cmode_tx, colorDepth);
			//colorDepth = (colorDepth &0x60)>>5;

			if (colorDepth)
			{
				tmdsClock= tmdsClock * 100;
				if (colorDepth == 1) tmdsClock = tmdsClock / 125;					// colorDepth = 10bit
				else				tmdsClock = tmdsClock / 150;					// colorDepth = 12bit
			}
			tmdsClock = tmdsClock * 1000;			// XX,XXX KHz
			//bufTiming.hFreq = tmdsClock / bufTiming.hTotal;		///< Horizontal frequency(100 Hz unit) = Dot Freq / hTotal

			bufTiming.vFreq = tmdsClock / bufTiming.vTotal;		//XX.X KHz
			tmpClock = bufTiming.vFreq * 100;					//X,XX0 KHz
			bufTiming.vFreq = tmpClock / bufTiming.hTotal;		//XX.X Hz	///< Veritical frequency(1/10 Hz unit) = hFreq / vTotal

			if ( bufTiming.scanType == 0)		// 0 : interlace
				bufTiming.vFreq = bufTiming.vFreq + bufTiming.vFreq;

			if (bufTiming.extInfo == LX_HDMI_EXT_3D_FIELD_ALTERNATIVE || pixelRepet > 0)
			{
				bufTiming.vFreq = bufTiming.vFreq >> 1;
			}

			if (bufTiming.vFreq > 1000)							bufTiming.vFreq = 1000;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 240, 5) )		bufTiming.vFreq = 240;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 250, 5) )		bufTiming.vFreq = 250;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 300, 5) )		bufTiming.vFreq = 300;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 500, 5) )		bufTiming.vFreq = 500;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 600, 5) )		bufTiming.vFreq = 600;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 660, 5) )		bufTiming.vFreq = 660;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 700, 5) )		bufTiming.vFreq = 700;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 720, 5) )		bufTiming.vFreq = 720;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 750, 5) )		bufTiming.vFreq = 750;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 760, 5) )		bufTiming.vFreq = 760;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 800, 5) )		bufTiming.vFreq = 800;
			else if ( u16CHK_RANGE(bufTiming.vFreq, 850, 5) )		bufTiming.vFreq = 850;
			else
			{
				tmp32 = bufTiming.vFreq % 10;
				if ( tmp32 != 0 )
				{
					bufTiming.vFreq = bufTiming.vFreq / 10;
					if (tmp32 < 5)
						bufTiming.vFreq = bufTiming.vFreq * 10;
					else
						bufTiming.vFreq = (bufTiming.vFreq + 1) * 10;
				}
			}
		}
		///* End Horizontal & Vertical frequency *///

		if ((_gH14HDMIState > HDMI_STATE_NO_SCDT && _gH14HDMIState <= HDMI_STATE_READ) ||_gH14HDMI_thread_running == 0)
		{
			if (_gH14Intr_vf_stable > HDMI_VIDEO_INIT_STATE)
			{
				CTOP_CTRL_H14_RdFL(ctr26);
				CTOP_CTRL_H14_Rd01(ctr26, phy_pll_sel, tmp32);

				if (pixelRepet > 0)
				{
					if (tmp32 == 0)
					{
						CTOP_CTRL_H14_Wr01(ctr26, phy_pll_sel, 0x2);
						CTOP_CTRL_H14_WrFL(ctr26);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/%d]  for pixel repet \n", __L__, __F__, pixelRepet);
					}
				}
				else if (bufTiming.extInfo == LX_HDMI_EXT_4K_2K)
				{
					if (tmp32 == 0)
					{
						CTOP_CTRL_H14_Wr01(ctr26, phy_pll_sel, 0x2);
						CTOP_CTRL_H14_WrFL(ctr26);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/2]  for UD\n", __L__, __F__);
					}
				}
				else
				{
					if (tmp32 != 0)
					{
						CTOP_CTRL_H14_Wr01(ctr26, phy_pll_sel, 0x0);
						CTOP_CTRL_H14_WrFL(ctr26);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/1]  for default\n", __L__, __F__);
					}
				}
			}

			if (pixelRepet > 1)
			{
				memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
				HDMI_ERROR("[%d] %s : not support Pixel Repetation [%d]\n", __L__, __F__, pixelRepet);
				break;
			}
		}

func_cnt:

		if ( _gH14TimingReadCnt > 100) 	goto func_exit;

		if ( memcmp(&_gH14PrevTiming, &bufTiming, sizeof(LX_HDMI_TIMING_INFO_T)) == 0 )
		{
			_gH14TimingReadCnt++;

			if (_gH14HDMIState == HDMI_STATE_READ && _gH14TimingReadCnt < 9)
				HDMI_VIDEO("[%d] %s : hActive[%d] vActive [%d] ReadCnt[%d] \n", __L__, __F__, bufTiming.hActive, bufTiming.vActive,_gH14TimingReadCnt);
		}

		break;

func_exit:

		memcpy(&bufTiming, &_gH14PrevTiming, sizeof(LX_HDMI_TIMING_INFO_T));
	} while(0);

	info->hFreq 		= _gH14PrevTiming.hFreq 	= bufTiming.hFreq; 			///< Horizontal frequency(100 Hz unit) = Dot Freq / hTotal
	info->vFreq 		= _gH14PrevTiming.vFreq 	= bufTiming.vFreq; 			///< Veritical frequency(1/10 Hz unit) = hFreq / vTotal
	info->hTotal		= _gH14PrevTiming.hTotal 	= bufTiming.hTotal; 		///< Horizontal total pixels
	info->vTotal 		= _gH14PrevTiming.vTotal		= bufTiming.vTotal; 		///< Vertical total lines
	info->hStart		= _gH14PrevTiming.hStart		= bufTiming.hStart; 		///< Horizontal start pixel (Back Porch)
	info->vStart 		= _gH14PrevTiming.vStart		= bufTiming.vStart;			///< Vertical start lines (Back Porch)
	info->hActive 	= _gH14PrevTiming.hActive	= bufTiming.hActive;		///< Horizontal active pixel
	info->vActive 	= _gH14PrevTiming.vActive	= bufTiming.vActive; 		///< Vertical active lines
	info->scanType 	= _gH14PrevTiming.scanType	= bufTiming.scanType; 		///< Scan type (0 : interlace, 1 : progressive) 	info->scanType ^= 1;
	info->extInfo		= _gH14PrevTiming.extInfo	= bufTiming.extInfo; 	///< Full 3D Timing
	info->state 		= _gH14HDMIState;

	HDMI_PRINT("[%d] %s  \n", __L__, __F__);
	return RET_OK;
}

int HDMI_H14_GetAviPacket(LX_HDMI_INFO_PACKET_T *packet)
{
	ULONG flags = 0;
	UINT32 header;
	UINT32 data[8];
	static UINT32 gAVIReadCnt = 0;
	LX_HDMI_AVI_COLORSPACE_T	csc = LX_HDMI_AVI_COLORSPACE_RGB;

	memset((void *)packet , 0 , sizeof(LX_HDMI_INFO_PACKET_T));
	//packet->InfoFrameType = LX_HDMI_INFO_AVI;

	if (_gH14Intr_vf_stable == HDMI_VIDEO_INIT_STATE || _gH14Intr_avi == HDMI_AVI_INIT_STATE)
	{
		gAVIReadCnt = 0;
		memset(&_gH14PrevAVIPacket, 0 , sizeof(LX_HDMI_INFO_PACKET_T));
		//_gH14PrevAVIPacket.InfoFrameType = LX_HDMI_INFO_AVI;
		return RET_OK;
	}

	if ( _gH14HDMI_thread_running == 1 && _gH14Intr_avi == HDMI_AVI_STABLE_STATE)
		goto func_exit;

	if (_gH14Intr_avi == HDMI_AVI_CHANGE_STATE)
	{
		//gAVIReadCnt = 0;
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
		_gH14AVIReadState = FALSE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
	}

	if (gAVIReadCnt > 300)
	{
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_avi = HDMI_AVI_STABLE_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
	}

	LINK_H14_RdFL(packet_18);
	header = LINK_H14_Rd(packet_18);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
	packet->header = (header &0xffff);

	LINK_H14_RdFL(packet_19);
	LINK_H14_RdFL(packet_20);
	LINK_H14_RdFL(packet_21);
	LINK_H14_RdFL(packet_22);
	LINK_H14_RdFL(packet_23);
	LINK_H14_RdFL(packet_24);
	LINK_H14_RdFL(packet_25);

	data[0] = LINK_H14_Rd(packet_19);
	data[1] = LINK_H14_Rd(packet_20);
	data[2] = LINK_H14_Rd(packet_21);
	data[3] = LINK_H14_Rd(packet_22);
	data[4] = LINK_H14_Rd(packet_23);
	data[5] = LINK_H14_Rd(packet_24);
	data[6] = LINK_H14_Rd(packet_25);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];

	if ( _gH14HDMI_thread_running == 1)
		packet->dataBytes[7] = gH14BootData->mode;		//HDMI Mode
	else
	{
		LINK_H14_RdFL(system_control_00);
		LINK_H14_Rd01(system_control_00, reg_hdmi_mode, data[7]);
		packet->dataBytes[7] = gH14BootData->mode = data[7];		//HDMI Mode
	}

	csc = (((data[0] &0xff00)>>8) &0x60)>>5;

	if ((csc == 3) && ((data[0]>>8)  == 0x60))		//data[0] 0xff ==> Cheaksum, data[0] 0x6000 ==> Y!Y0
	{
		csc = 0;
		packet->dataBytes[0] = data[0] = data[0] &0xff;
	}

	if (_gH14PrevPixelEncoding != csc)
	{
		_HDMI_H14_SetVideoBlank(csc);
		_gH14PrevPixelEncoding = csc;
	}

	_gH14AVIReadState = TRUE;

	if ( memcmp(&_gH14PrevAVIPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		gAVIReadCnt = 0;
		HDMI_VIDEO("[%d] %s : changed AVI Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, _gH14PrevAVIPacket.InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("AVI header	gPre[%d] /packet[%d] \n", _gH14PrevAVIPacket.header, packet->header);
		HDMI_VIDEO("AVI data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", _gH14PrevAVIPacket.dataBytes[0], _gH14PrevAVIPacket.dataBytes[1], _gH14PrevAVIPacket.dataBytes[2], \
			_gH14PrevAVIPacket.dataBytes[3], _gH14PrevAVIPacket.dataBytes[4], _gH14PrevAVIPacket.dataBytes[5], _gH14PrevAVIPacket.dataBytes[6], _gH14PrevAVIPacket.dataBytes[7]);
		HDMI_VIDEO("AVI data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&_gH14PrevAVIPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T));
		return RET_OK;
	}
	else
	{
		if (gAVIReadCnt < 350) 		gAVIReadCnt++;
		return RET_OK;
	}

func_exit:
	memcpy(packet, &_gH14PrevAVIPacket,  sizeof(LX_HDMI_INFO_PACKET_T));
	HDMI_PRINT("[%d] %s \n", __L__, __F__);

	return RET_OK;
}

int HDMI_H14_GetVsiPacket(LX_HDMI_INFO_PACKET_T *packet)
{
	ULONG flags = 0;
	UINT32 header;
	UINT32 data[9];
	static UINT32 gVSIReadCnt = 0;

	memset((void *)packet , 0 , sizeof(LX_HDMI_INFO_PACKET_T));
	packet->InfoFrameType = LX_HDMI_INFO_VSI;

	//HDMI_DEBUG("%s, %d _gH14Intr_vf_stable= %d,_gH14Intr_vsi = %d, \n\n",__F__, __L__, _gH14Intr_vf_stable, _gH14Intr_vsi);

	if (_gH14Intr_vf_stable == HDMI_VIDEO_INIT_STATE ||_gH14Intr_vsi < HDMI_VSI_INTERRUPT_STATE)
	{
		gVSIReadCnt = 0;
		memset(&_gH14PrevVSIPacket, 0 , sizeof(LX_HDMI_INFO_PACKET_T));
		_gH14PrevVSIPacket.InfoFrameType = LX_HDMI_INFO_VSI;
		return RET_OK;
	}

	if (_gH14VSIState == TRUE)
	{
		LINK_H14_RdFL(interrupt_enable_00);
		LINK_H14_Wr01(interrupt_enable_00, intr_no_vsi_enable, 0x1);			///< 20 intr_no_vsi_enable
		LINK_H14_WrFL(interrupt_enable_00);
		_gH14VSIState = FALSE;
		HDMI_DEBUG("[%d] %s : No VSI intra enable  \n", __LINE__, __func__);
	}

	if ( _gH14HDMI_thread_running == 1 && _gH14Intr_vsi == HDMI_VSI_STABLE_STATE)
		goto func_exit;

	if (gVSIReadCnt > 200)
	{
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_vsi = HDMI_VSI_STABLE_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
	}

	LINK_H14_RdFL(packet_75);
	header = LINK_H14_Rd(packet_75);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
	packet->header = (header &0xffff);

	LINK_H14_RdFL(packet_76);
	LINK_H14_RdFL(packet_77);
	LINK_H14_RdFL(packet_78);
	LINK_H14_RdFL(packet_79);
	LINK_H14_RdFL(packet_80);
	LINK_H14_RdFL(packet_81);
	LINK_H14_RdFL(packet_82);

	data[0] = LINK_H14_Rd(packet_76);
	data[1] = LINK_H14_Rd(packet_77);
	data[2] = LINK_H14_Rd(packet_78);
	data[3] = LINK_H14_Rd(packet_79);
	data[4] = LINK_H14_Rd(packet_80);
	data[5] = LINK_H14_Rd(packet_81);
	data[6] = LINK_H14_Rd(packet_82);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];
	packet->dataBytes[7] = 0;

	if ( memcmp(&_gH14PrevVSIPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		gVSIReadCnt = 0;
		HDMI_VIDEO("[%d] %s : changed VSI Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, _gH14PrevVSIPacket.InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("VSI header	gPre[%d] /packet[%d] \n", _gH14PrevVSIPacket.header, packet->header);
		HDMI_VIDEO("VSI data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", _gH14PrevVSIPacket.dataBytes[0], _gH14PrevVSIPacket.dataBytes[1], _gH14PrevVSIPacket.dataBytes[2], \
			_gH14PrevVSIPacket.dataBytes[3], _gH14PrevVSIPacket.dataBytes[4], _gH14PrevVSIPacket.dataBytes[5], _gH14PrevVSIPacket.dataBytes[6], _gH14PrevVSIPacket.dataBytes[7]);
		HDMI_VIDEO("VSI data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&_gH14PrevVSIPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T));
		return RET_OK;
	}
	else
	{
		if (gVSIReadCnt < 250) 		gVSIReadCnt++;
		return RET_OK;
	}

func_exit:
	memcpy(packet, &_gH14PrevVSIPacket, sizeof(LX_HDMI_INFO_PACKET_T));
	HDMI_PRINT("[%d] %s \n", __L__, __F__);

	return RET_OK;
}

int HDMI_H14_GetSpdPacket(LX_HDMI_INFO_PACKET_T *packet)
{
	UINT32 header;
	UINT32 data[9];
	static LX_HDMI_INFO_PACKET_T gPrevSPDPacket = {0, };

	memset((void *)packet , 0 , sizeof(LX_HDMI_INFO_PACKET_T));
	packet->InfoFrameType = LX_HDMI_INFO_SPD;

	if (_gH14Intr_vf_stable == HDMI_VIDEO_INIT_STATE)
	{
		memset(&gPrevSPDPacket, 0 , sizeof(LX_HDMI_INFO_PACKET_T));
		gPrevSPDPacket.InfoFrameType = LX_HDMI_INFO_SPD;
		return RET_OK;
	}

	LINK_H14_RdFL(packet_59);
	header = LINK_H14_Rd(packet_59);
	packet->header = (header &0xffff);

	LINK_H14_RdFL(packet_60);
	LINK_H14_RdFL(packet_61);
	LINK_H14_RdFL(packet_62);
	LINK_H14_RdFL(packet_63);
	LINK_H14_RdFL(packet_64);
	LINK_H14_RdFL(packet_65);
	LINK_H14_RdFL(packet_66);

	data[0] = LINK_H14_Rd(packet_60);
	data[1] = LINK_H14_Rd(packet_61);
	data[2] = LINK_H14_Rd(packet_62);
	data[3] = LINK_H14_Rd(packet_63);
	data[4] = LINK_H14_Rd(packet_64);
	data[5] = LINK_H14_Rd(packet_65);
	data[6] = LINK_H14_Rd(packet_66);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];
	packet->dataBytes[7] = 0;

	if ( memcmp(&gPrevSPDPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		HDMI_VIDEO("[%d] %s : changed SPD Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, gPrevSPDPacket.InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("SPD header	gPre[%d] /packet[%d] \n", gPrevSPDPacket.header, packet->header);
		HDMI_VIDEO("SPD data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", gPrevSPDPacket.dataBytes[0], gPrevSPDPacket.dataBytes[1], gPrevSPDPacket.dataBytes[2], \
			gPrevSPDPacket.dataBytes[3], gPrevSPDPacket.dataBytes[4], gPrevSPDPacket.dataBytes[5], gPrevSPDPacket.dataBytes[6], gPrevSPDPacket.dataBytes[7]);
		HDMI_VIDEO("SPD data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&gPrevSPDPacket, packet, sizeof(LX_HDMI_INFO_PACKET_T));
	}

	return RET_OK;
}

int HDMI_H14_GetInfoPacket(LX_HDMI_INFO_PACKET_T *packet)
{
	//not implemented now!!!
	memset((void *)packet, 0, sizeof(LX_HDMI_INFO_PACKET_T));

	if (_gH14Intr_vf_stable == HDMI_VIDEO_INIT_STATE)
		return RET_OK;

	return RET_OK;
}

int HDMI_H14_SetPowerControl(UINT32 power)
{
	if (power)
	{
		LINK_H14_RdFL(phy_00);
		LINK_H14_Wr01(phy_00, phy_enable, 0x1);
		LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
		LINK_H14_Wr01(phy_00, phy_rstn, 0x0);			///< Phy Enable
		LINK_H14_Wr01(phy_00, phy_arc_pdb, 0x0);			///< Phy Enable
		LINK_H14_WrFL(phy_00);

		/* PHY_PDB=1, PHY_RSTN=1 을 동시에 인가 register들이 제대로 pre-set되지 않는 현상 발생함. */
		/* PHY_PDB=1  -> 10us  -> PHY_RSTN=1  */
		OS_MsecSleep(2);	// ms delay

		LINK_H14_RdFL(phy_00);
		LINK_H14_Wr01(phy_00, phy_pdb, 0x0);
		LINK_H14_WrFL(phy_00);

		OS_MsecSleep(2);	// ms delay

		LINK_H14_RdFL(phy_00);
		LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
		LINK_H14_WrFL(phy_00);
		/* PHY_PDB=1, PHY_RSTN=1 을 동시에 인가 register들이 제대로 pre-set되지 않는 현상 발생함. */
		/* PHY_PDB=1  -> 10us  -> PHY_RSTN=1  */
		OS_MsecSleep(2);	// ms delay

		LINK_H14_RdFL(phy_00);
		LINK_H14_Wr01(phy_00, phy_rstn, 0x1);			///< Phy Enable
		LINK_H14_WrFL(phy_00);

		OS_MsecSleep(2);	// ms delay

		PHY_REG_H14_RdFL(pdb_d0_man_sel);
		PHY_REG_H14_Wr01(pdb_d0_man_sel, pdb_d0_man_sel, 0x1);
		PHY_REG_H14_Wr01(pdb_d0_man_sel, pdb_d0_man, 0x0);
		PHY_REG_H14_WrFL(pdb_d0_man_sel);

		PHY_REG_H14_RdFL(pdb_dck_man_sel);
		PHY_REG_H14_Wr01(pdb_dck_man_sel, pdb_dck_man_sel, 0x1);
		PHY_REG_H14_Wr01(pdb_dck_man_sel, pdb_dck_man, 0x0);
		PHY_REG_H14_WrFL(pdb_dck_man_sel);

		PHY_REG_H14_RdFL(pdb_d0_man_sel);
		PHY_REG_H14_Wr01(pdb_d0_man_sel, pdb_d0_man_sel, 0x0);
		PHY_REG_H14_Wr01(pdb_d0_man_sel, pdb_d0_man, 0x1);
		PHY_REG_H14_WrFL(pdb_d0_man_sel);

		PHY_REG_H14_RdFL(pdb_dck_man_sel);
		PHY_REG_H14_Wr01(pdb_dck_man_sel, pdb_dck_man_sel, 0x0);
		PHY_REG_H14_Wr01(pdb_dck_man_sel, pdb_dck_man, 0x1);
		PHY_REG_H14_WrFL(pdb_dck_man_sel);

		_HDMI_H14_PhyRunReset();	//init Phy register
		_HDMI_H14_RunReset();		//init Link register

		PHY_REG_H14_RdFL(cr_mode_sel_resetb);
		PHY_REG_H14_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb, 0x0);
		PHY_REG_H14_WrFL(cr_mode_sel_resetb);

		PHY_REG_H14_RdFL(cr_mode_sel_resetb);
		PHY_REG_H14_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb, 0x1);
		PHY_REG_H14_WrFL(cr_mode_sel_resetb);
	}
	else
	{
		LINK_H14_RdFL(phy_00);
		LINK_H14_Wr01(phy_00, phy_pdb, 0x0);
		LINK_H14_Wr01(phy_00, phy_rstn, 0x0);		///< Phy Disable
		LINK_H14_WrFL(phy_00);
	}

	HDMI_DEBUG("[%d] %s :  %s \n", __L__, __F__, (power ? "On" : "Off"));
	return RET_OK;
}

int HDMI_H14_SetHPD(LX_HDMI_HPD_T *hpd)
{
	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, hpd0_out, hpd->bEnable);
	LINK_H14_WrFL(phy_00);

	HDMI_PRINT("[%d] %s :  %s \n", __L__, __F__, (hpd->bEnable ? "On" : "Off"));
	return RET_OK;
}


int HDMI_H14_GetStatus(LX_HDMI_STATUS_T *status)
{
	UINT32 tmdsClock = 0;
	UINT32 up_freq = 0,	down_freq = 0;
	LX_HDMI_AVI_COLORSPACE_T csc = 0;		    ///< PixelEncoding

	memset((void *)status , 0 , sizeof(LX_HDMI_STATUS_T));

	do {
		if (_gH14HDMIState < HDMI_STATE_SCDT)
			break;

		status->bHdmiMode = gH14BootData->mode;

		status->eHotPlug = _gH14HDMIConnectState;

		LINK_H14_RdFL(video_02);
		LINK_H14_Rd01(video_02, reg_cmode_tx, status->eColorDepth);

		status->eColorDepth = (status->eColorDepth &0x60)>>5;

		LINK_H14_RdFL(video_00);
		LINK_H14_Rd01(video_00, reg_pr_tx, status->pixelRepet);

		status->csc = _gH14PrevPixelEncoding;
		csc = (((_gH14PrevAVIPacket.dataBytes[0] &0xff00)>>8) &0x60)>>5;
		if (_gH14PrevPixelEncoding == csc)		HDMI_DEBUG(" _gH14PrevPixelEncoding = %d , csc = %d \n", _gH14PrevPixelEncoding, csc);

		PHY_REG_H14_RdFL(tmds_freq_1);
		PHY_REG_H14_RdFL(tmds_freq_2);

		PHY_REG_H14_Rd01(tmds_freq_1, tmds_freq, up_freq);
		PHY_REG_H14_Rd01(tmds_freq_2, tmds_freq, down_freq);
		tmdsClock = ((up_freq << 8) + down_freq); 	// XXX.XX KHz

		HDMI_VIDEO("HDMI_H14_GetStatus  depth = 0x%x tmds =%d\n", status->eColorDepth, tmdsClock);
		HDMI_VIDEO("[HDMI] %d HDMI  State  = %d _gH14Force_thread_sleep= %d  _gH14HDMI_thread_running =%d \n",\
			__L__, _gH14HDMIState,_gH14Force_thread_sleep, _gH14HDMI_thread_running);
		HDMI_VIDEO("[HDMI]  AVI State[%d] VSI State[%d]  SRC Mute State[%d] \n",  _gH14Intr_avi, _gH14Intr_vsi, _gH14Intr_src_mute );
	} while (0);

	return RET_OK;
}

/**
 *  HDMI_H14_GetAudioInfo
 *
 *  @parm LX_HDMI_AUDIO_INFO_T *
 *  @return int
*/
int HDMI_H14_GetAudioInfo(LX_HDMI_AUDIO_INFO_T *pAudioInfo)
{
	int ret = RET_OK;

	//Return a previous global value.
	pAudioInfo->audioType	 = _gH14HdmiAudioInfo.audioType;
	pAudioInfo->samplingFreq = _gH14HdmiAudioInfo.samplingFreq;

	//Debug print
	if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_1S) == 0)
	{
		HDMI_AUDIO("HDMI_GetAudioInfo : type = %d, freq = %d, state = %d, mute = %d\n",	\
					pAudioInfo->audioType, pAudioInfo->samplingFreq, _gH14IntrAudioState, _gH14AudioMuteState);
	}
	else
	{
		HDMI_PRINT("HDMI_GetAudioInfo : type = %d, freq = %d, state = %d, mute = %d\n",	\
					pAudioInfo->audioType, pAudioInfo->samplingFreq, _gH14IntrAudioState, _gH14AudioMuteState);
	}

	return ret;
}

/**
 *  HDMI_H14_GetAudioCopyInfo
 *
 *  @parm LX_HDMI_AUDIO_COPY_T *
 *  @return int
*/
int HDMI_H14_GetAudioCopyInfo(LX_HDMI_AUDIO_COPY_INFO_T *pCopyInfo)
{
	int ret = RET_OK;

	UINT32	reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1;
	UINT32	CpBit, LBit;

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	LINK_H14_RdFL(system_control_00);
	LINK_H14_Rd01(system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	//Get a hdmi audio copyright information.
	if (reg_hdmi_mode == 1)	//HDMI Mode
	{
		//Read  reg_achst_byte0 reg.
		LINK_H14_RdFL(audio_07);
		LINK_H14_Rd01(audio_07, reg_achst_byte0, reg_achst_byte0);

		//Set a CpBit
		if (reg_achst_byte0 & HDMI_AUDIO_CP_BIT_MASK)
			CpBit = 1;
		else
			CpBit = 0;

		//Read  reg_achst_byte1reg.
		LINK_H14_RdFL(audio_08);
		LINK_H14_Rd01(audio_08, reg_achst_byte1, reg_achst_byte1);

		//Set a LBit
		if (reg_achst_byte1 & HDMI_AUDIO_L_BIT_MASK)
			LBit = 1;
		else
			LBit = 0;

		//Set a Copyright Info. by CpBit and LBit
		if (CpBit == 0 && LBit == 0)
			pCopyInfo->copyInfo = LX_HDMI_AUDIO_COPY_ONCE;
		else if (CpBit == 0 && LBit == 1)
			pCopyInfo->copyInfo = LX_HDMI_AUDIO_COPY_NO_MORE;	//same with LX_HDMI_AUDIO_COPY_NEVER
		else
			pCopyInfo->copyInfo = LX_HDMI_AUDIO_COPY_FREE;

		//Debug print
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_1S) == 0)
		{
			HDMI_AUDIO("HDMI_H14_GetAudioCopyInfo : CopyInfo = %d(mode = %d)(0x%X, 0x%X)\n", pCopyInfo->copyInfo, reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1);
		}
		else
		{
			HDMI_PRINT("HDMI_H14_GetAudioCopyInfo : CopyInfo = %d(mode = %d)(0x%X, 0x%X)\n", pCopyInfo->copyInfo, reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1);
		}
	}
	else	//DVI Mode
	{
		pCopyInfo->copyInfo = LX_HDMI_AUDIO_COPY_FREE;
	}

	HDMI_PRINT("HDMI_H14_GetAudioCopyInfo : CopyInfo = %d(mode = %d)\n", pCopyInfo->copyInfo, reg_hdmi_mode);
	return ret;
}

/**
 *  HDMI_H14_SetArc
 *
 *  @parm LX_HDMI_ARC_CTRL_T *
 *  @return int
*/
int HDMI_H14_SetArc(LX_HDMI_ARC_CTRL_T *pArcCtrl)
{
	//ARC source
	LINK_H14_RdFL(heac_00);
	LINK_H14_Wr01(heac_00, reg_arc_src, 0x0);
	LINK_H14_WrFL(heac_00);

	//Enable or Disable ARC port
	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_arc_pdb, (UINT32)pArcCtrl->bARCEnable);
	LINK_H14_WrFL(phy_00);

	//Update _gAudioArcStatus
	_gH14AudioArcStatus = pArcCtrl->bARCEnable;

	HDMI_DEBUG("[%d] %s :  %s \n", __L__, __F__, pArcCtrl->bARCEnable? "On":"Off");
	return RET_OK;
}

/**
 *  HDMI_H14_SetMute
 *
 *  @parm LX_HDMI_MUTE_CTRL_T *
 *  @return int
*/
int HDMI_H14_SetMute(LX_HDMI_MUTE_CTRL_T *pMuteCtrl)
{
	BOOLEAN 	videoMuteState;

	LINK_H14_RdFL(system_control_02);
	LINK_H14_Rd01(system_control_02, reg_mute_vid, videoMuteState);

	//video related.
	if (pMuteCtrl->eMode == LX_HDMI_VIDEO_MUTE || pMuteCtrl->eMode == LX_HDMI_AV_MUTE)
	{
		if (pMuteCtrl->bVideoMute != videoMuteState)
		{
			LINK_H14_Wr01(system_control_02, reg_mute_vid, pMuteCtrl->bVideoMute);
			LINK_H14_WrFL(system_control_02);

			HDMI_VIDEO("[%d] %s : bVideoMute = %s \n", __L__, __F__, (pMuteCtrl->bVideoMute ? "On" : "Off"));
		}
	}

	//audio related.
	if (pMuteCtrl->eMode == LX_HDMI_AUDIO_MUTE || pMuteCtrl->eMode == LX_HDMI_AV_MUTE)
	{
		//Check a previous state
		if (_gH14AudioMuteState != pMuteCtrl->bAudioMute)
		{
			if (pMuteCtrl->bAudioMute == TRUE)
			{
				//Mute audio data
				LINK_H14_RdFL(audio_05);
				LINK_H14_Wr01(audio_05, reg_i2s_sd_en, 0x0);	//I2S SD Output Disable(4 Channel)
				LINK_H14_WrFL(audio_05);
			}
			else
			{
				//Un-mute audio data
				LINK_H14_RdFL(audio_05);
				LINK_H14_Wr01(audio_05, reg_i2s_sd_en, 0xF);	//I2S SD Output Enable(4 Channel)
				LINK_H14_WrFL(audio_05);
			}

			//Update _gH14AudioMuteState
			_gH14AudioMuteState = pMuteCtrl->bAudioMute;

			//For debug print
			if (pMuteCtrl->bAudioMute == FALSE)
			{
				HDMI_AUDIO("[%d] %s : bAudioMute = %s \n", __L__, __F__, (pMuteCtrl->bAudioMute ? "On" : "Off"));
			}
			else
			{
				HDMI_DEBUG("[%d] %s : bAudioMute = %s \n", __L__, __F__, (pMuteCtrl->bAudioMute ? "On" : "Off"));
			}
		}

		//For debug print
		if (pMuteCtrl->bAudioMute == FALSE)
		{
			HDMI_DEBUG("[%d]SetMute : type = %d, freq = %d, mute = %d, PSC = %d\n", \
						__L__, _gH14HdmiAudioInfo.audioType, _gH14HdmiAudioInfo.samplingFreq, _gH14AudioMuteState, _gH14HdmiPortStableCount);
		}
	}

	return RET_OK;
}

/**
 *  HDMI_H14_GetAudioDebugInfo
 *
 *  @parm LX_HDMI_AUDIO_INFO_T *
 *  @return int
*/
int HDMI_H14_GetAudioDebugInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo)
{
	int ret = RET_OK;

	//Copy and Print HDMI Audio Debug Info.
	(void)_HDMI_H14_DebugAudioInfo(pAudioDebugInfo);

	HDMI_DEBUG("HDMI_H14_GetAudioDebugInfo : type = %d, freq = %d, state = %d, mute = %d, PSC = %d, video = %d\n", \
				_gH14HdmiAudioInfo.audioType, _gH14HdmiAudioInfo.samplingFreq, _gH14IntrAudioState, _gH14AudioMuteState, _gH14HdmiPortStableCount, _gH14HDMIState);

	return ret;
}

#ifdef NOT_USED_NOW
static void _HDMI_H14_ModifyUDFormat(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock)
{
	do {
		if (tmdsClock < 29000) break;
//// H total
		if (u16CHK_RANGE(info->hTotal, 4400, 2) ) //@30Hz
		{
			if (info->hActive > 4000)	// 4096x2160@30Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal =  4400;
				info->vTotal = 2250;
				info->hActive = 4096;
				info->vActive = 2160;
				info->hStart = 297;
				info->vStart = 72;
			}
			else // 3840x2160@30Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal = 4400;
				info->vTotal = 2250;
				info->hActive = 3840;
				info->vActive = 2160;
				info->hStart = 296;
				info->vStart = 72;
			}
		}
		else if (u16CHK_RANGE(info->hTotal, 5280, 2) ) //@25Hz
		{
			if (info->hActive > 4000)	// 4096x2160@25Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal = 5280;
				info->vTotal = 2250;
				info->hActive = 4096;
				info->vActive = 2160;
				info->hStart = 296;
				info->vStart = 72;
			}
			else // 3840x2160@25Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal = 5280;
				info->vTotal = 2250;
				info->hActive = 3840;
				info->vActive = 2160;
				info->hStart = 296;
				info->vStart = 72;
			}
		}
		else if (u16CHK_RANGE(info->hTotal, 5500, 2) ) //24Hz
		{
			if (info->hActive > 4000)	// 4096x2160@24Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal =  5500;
				info->vTotal = 2250;
				info->hActive = 4096;
				info->vActive = 2160;
				info->hStart = 296;
				info->vStart = 72;
			}
			else // 3840x2160@24Hz
			{
				HDMI_VIDEO("[%d] %s : UD H[%d] format modification \n", __L__, __F__, info->hTotal);

				info->hTotal = 5500;
				info->vTotal = 2250;
				info->hActive = 3840;
				info->vActive = 2160;
				info->hStart = 296;
				info->vStart = 72;
			}
		}
		else if (u16CHK_RANGE(info->hTotal, 2200, 2) ) // @60Hz
		{
			if (info->hActive > 2000)	// 2048x2160@60Hz
			{
				HDMI_VIDEO("[%d] %s : 2048x2160@60Hz format modification \n", __L__, __F__);

				info->hTotal = 2200;
				info->vTotal = 2250;
				info->hActive = 2048;
				info->vActive = 2160;
				info->hStart = 148;
				info->vStart = 72;
			}
			else // 1920x2160@60Hz
			{
				HDMI_VIDEO("[%d] %s : 1920x2160@60Hz format modification \n", __L__, __F__);

				info->hTotal = 2200;
				info->vTotal = 2250;
				info->hActive = 1920;
				info->vActive = 2160;
				info->hStart = 148;
				info->vStart = 72;
			}
		}
		else if (u16CHK_RANGE(info->hTotal, 2640, 2) ) // @50Hz
		{
			if (info->hActive > 2000)	// 2048x2160@50Hz
			{
				HDMI_VIDEO("[%d] %s : 2048x2160@50Hz format modification \n", __L__, __F__);

				info->hTotal = 2640;
				info->vTotal = 2250;
				info->hActive = 2048;
				info->vActive = 2160;
				info->hStart = 148;
				info->vStart = 72;
			}
			else // 1920x2160@50Hz
			{
				HDMI_VIDEO("[%d] %s : 1920x2160@50Hz format modification \n", __L__, __F__);

				info->hTotal = 2640;
				info->vTotal = 2250;
				info->hActive = 1920;
				info->vActive = 2160;
				info->hStart = 148;
				info->vStart = 72;
			}
		}
////// V total
		else if (u16CHK_RANGE(info->vTotal, 2250, 5) )
		{
			if (u16CHK_RANGE(info->hTotal, 4400, 2) ) //@30Hz
			{
				if (info->hActive > 4000)	// 4096x2160@30Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal =  4400;
					info->vTotal = 2250;
					info->hActive = 4096;
					info->vActive = 2160;
					info->hStart = 297;
					info->vStart = 72;
				}
				else // 3840x2160@30Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal = 4400;
					info->vTotal = 2250;
					info->hActive = 3840;
					info->vActive = 2160;
					info->hStart = 296;
					info->vStart = 72;
				}
			}
			else if (u16CHK_RANGE(info->hTotal, 5280, 2) ) //@25Hz
			{
				if (info->hActive > 4000)	// 4096x2160@25Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal = 5280;
					info->vTotal = 2250;
					info->hActive = 4096;
					info->vActive = 2160;
					info->hStart = 296;
					info->vStart = 72;
				}
				else // 3840x2160@25Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal = 5280;
					info->vTotal = 2250;
					info->hActive = 3840;
					info->vActive = 2160;
					info->hStart = 296;
					info->vStart = 72;
				}
			}
			else if (u16CHK_RANGE(info->hTotal, 5500, 2) ) //@24Hz
			{
				if (info->hActive > 4000)	// 4096x2160@24Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal =  5500;
					info->vTotal = 2250;
					info->hActive = 4096;
					info->vActive = 2160;
					info->hStart = 296;
					info->vStart = 72;
				}
				else // 3840x2160@24Hz
				{
					HDMI_VIDEO("[%d] %s : UD V[%d] format modification \n", __L__, __F__, info->vTotal);

					info->hTotal = 5500;
					info->vTotal = 2250;
					info->hActive = 3840;
					info->vActive = 2160;
					info->hStart = 296;
					info->vStart = 72;
				}
			}
			else if (u16CHK_RANGE(info->hTotal, 2200, 2) ) // @60Hz
			{
				if (info->hActive > 2000)	// 2048x2160@60Hz
				{
					HDMI_VIDEO("[%d] %s : 2048x2160@60Hz format modification \n", __L__, __F__);

					info->hTotal = 2200;
					info->vTotal = 2250;
					info->hActive = 2048;
					info->vActive = 2160;
					info->hStart = 148;
					info->vStart = 72;
				}
				else // 1920x2160@60Hz
				{
					HDMI_VIDEO("[%d] %s : 1920x2160@60Hz format modification \n", __L__, __F__);

					info->hTotal = 2200;
					info->vTotal = 2250;
					info->hActive = 1920;
					info->vActive = 2160;
					info->hStart = 148;
					info->vStart = 72;
				}

			}
			else if (u16CHK_RANGE(info->hTotal, 2640, 2) ) // @50Hz
			{
				if (info->hActive > 2000)	// 2048x2160@50Hz
				{
					HDMI_VIDEO("[%d] %s : 2048x2160@50Hz format modification \n", __L__, __F__);

					info->hTotal = 2640;
					info->vTotal = 2250;
					info->hActive = 2048;
					info->vActive = 2160;
					info->hStart = 148;
					info->vStart = 72;
				}
				else // 1920x2160@50Hz
				{
					HDMI_VIDEO("[%d] %s : 1920x2160@50Hz format modification \n", __L__, __F__);

					info->hTotal = 2640;
					info->vTotal = 2250;
					info->hActive = 1920;
					info->vActive = 2160;
					info->hStart = 148;
					info->vStart = 72;
				}
			}
			else if (u16CHK_RANGE(info->hTotal, 2750, 2) ) // 1920x2160@48Hz
			{
				HDMI_VIDEO("[%d] %s : 1920x2160@48Hz format modification \n", __L__, __F__);

				info->hTotal = 2750;
				info->vTotal = 2250;
				info->hActive = 1920;
				info->vActive = 2160;
				info->hStart = 148;
				info->vStart = 72;
			}
		}
	} while (0);
}
#endif	//#ifdef NOT_USED_NOW

static void _HDMI_H14_CheckUDExtInfo(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock)
{
	UINT16 vic = 0;

	do {
		if (tmdsClock < 29000) break;

		if (_gH14Intr_avi > HDMI_AVI_INIT_STATE)
		{
			vic = ((_gH14PrevAVIPacket.dataBytes[1] &0xff) &0x7f);	// VIC6~VIC0
			HDMI_VIDEO("VIC of AVI Packet = [%d]  if VIC is over 96, this format is UD format\n", vic);
		}

		if (info->extInfo == LX_HDMI_EXT_2D_FORMAT && info->vActive == 2160)
		{
			info->extInfo = LX_HDMI_EXT_4K_2K;
		}
		else if (info->extInfo == LX_HDMI_EXT_3D_LINE_ALTERNATIVE && info->vActive == 1080)
		{
			info->vActive = 2160;
			info->extInfo = LX_HDMI_EXT_4K_2K;
		}
	} while (0);
}

static int _HDMI_H14_GetExtendTimingInfo(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock)
{
	UINT8 	tmp8 =0;
	UINT32	tmp32 = 0, i = 0;
	//UINT32 	videoIdCode = 0;
	LX_HDMI_TIMING_INFO_T 	bufTiming = {0,};
	LX_HDMI_VSI_VIDEO_FORMAT_T eVideoFormat = 0;	/**< HDMI VSI info */
	LX_HDMI_VSI_3D_STRUCTURE_T e3DStructure = 0;	/**< HDMI VSI info */
	const sEXT_TIMING_ENUM *pTbl;
	const sO_ENUM *p3DTbl;

	do {
		memcpy(&bufTiming , info , sizeof(LX_HDMI_TIMING_INFO_T));
		bufTiming.extInfo = LX_HDMI_EXT_2D_FORMAT;		///< 2D format

		if (_gH14Intr_vsi < HDMI_VSI_INTERRUPT_STATE)
		{
			_HDMI_H14_CheckUDExtInfo(&bufTiming, tmdsClock);		// check UD format
			if (bufTiming.extInfo == LX_HDMI_EXT_4K_2K)		break;

			pTbl = &TBL_EXT_INFO[0];
			for (i = 0; i < TBL_NUM(TBL_EXT_INFO) ; i++, pTbl++)
			{
				if ((info->hActive == pTbl->hAct_info) && (info->vActive == pTbl->vAct_info) && (info->scanType== pTbl->scan_info) )
				{
					bufTiming.hActive 		= pTbl->hAct_buf;
					bufTiming.vActive 		= pTbl->vAct_buf;
					bufTiming.scanType	= pTbl->scan_buf;
					bufTiming.extInfo		= pTbl->extInfo_buf;
					HDMI_VIDEO(" check Extend TimingInfo for NO VSI \n");
					break;
				}
			}
			break;
		}

		/**< HDMI VSI info */
		LINK_H14_RdFL(packet_77);
		tmp32 = LINK_H14_Rd(packet_77);
		eVideoFormat = ((tmp32 &0xff))>> 5;

		if (eVideoFormat == LX_FORMAT_EXTENDED_RESOLUTION_FORMAT)		// 4Kx2K
		{
			bufTiming.extInfo = LX_HDMI_EXT_4K_2K;				///< 4kx2K format
		}
		else if (eVideoFormat == LX_FORMAT_3D_FORMAT)
		{
			e3DStructure = ((tmp32 &0xff00)>>8)>> 4;

			p3DTbl = &EXT_3D_TYPE[0];
			for (i = 0; i < TBL_NUM(EXT_3D_TYPE) ; i++, p3DTbl++)
			{
				if (e3DStructure == p3DTbl->PreEnum)
				{
					bufTiming.extInfo = p3DTbl->PostEnum;
					break;
				}
				bufTiming.extInfo = p3DTbl->PostEnum;
			}
		}
		else //if (eVideoFormat == LX_FORMAT_NO_ADDITIONAL_FORMAT)
		{
			_HDMI_H14_CheckUDExtInfo(&bufTiming, tmdsClock);		// check UD format
			break;
		}

		switch(bufTiming.extInfo)		//if (eVideoFormat == LX_FORMAT_3D_FORMAT)
		{
			case LX_HDMI_EXT_3D_FRAMEPACK:
			{
				// Russia  STB Issue
				// VSI Info is 3D F/P and Source is 2D
				if ( (bufTiming.hActive == 640 && bufTiming.vActive == 480) \
					|| (bufTiming.hActive == 720 && bufTiming.vActive == 480) \
					|| (bufTiming.hActive == 720 && bufTiming.vActive == 576) \
					|| (bufTiming.hActive == 1280 && bufTiming.vActive == 720) \
					|| (bufTiming.hActive == 1920 && bufTiming.vActive == 1080) )
				{
					bufTiming.extInfo = LX_HDMI_EXT_2D_FORMAT;		///< 2D format
					HDMI_VIDEO("[%d] %s : 3D info is F/P, but Timing Info is 2D format for Russia STB Issue \n",  __L__, __F__);
					break;
				}
#if 1
				if ( (bufTiming.vActive == 2228) ||(bufTiming.vActive == 1028) )		//1080i & 480i
				{
					bufTiming.scanType = 0;
					tmp8 = 68; //23+22+23
					bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
				}
				else if (bufTiming.vActive == 1226)		//576i
				{
					bufTiming.scanType = 0;
					tmp8 = 74; // 25+24+25
					bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
				}
				else if (bufTiming.vActive == 1103)		// interace
				{
					bufTiming.scanType = 0;
					tmp8 = 23; //23
					bufTiming.vActive = bufTiming.vActive - tmp8;
				}
				else			//progressive
				{
					tmp8 = (bufTiming.vTotal - bufTiming.vActive);
					bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
				}

#else
				/// detected progressive for Frame Packing Interlace format, check scan type.
				LINK_H14_RdFL(packet_20);		//AVI
				tmp32 = LINK_H14_Rd(packet_20);
				videoIdCode = (((tmp32 &0xff)) &0x7f);				// VIC6~VIC0

				if (videoIdCode == 5 || videoIdCode == 6 ||videoIdCode == 7 ||videoIdCode == 10 || videoIdCode == 11 ||
					videoIdCode == 20 || videoIdCode == 21 ||videoIdCode == 22 || videoIdCode == 25 || videoIdCode == 26 ||
					videoIdCode == 39 || videoIdCode == 40 || videoIdCode == 44 || videoIdCode == 45)		// 1080!@60Hz & 1080!@50Hz
					bufTiming.scanType = 0;

				HDMI_VIDEO(" VIC  = %d \n", videoIdCode);

				if (videoIdCode == 0)
				{
					if ( (bufTiming.vActive == 2228) ||(bufTiming.vActive == 1028) )		//1080i & 480i
					{
						bufTiming.scanType = 0;
						tmp8 = 68; //23+22+23
						bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
					}
					else if (bufTiming.vActive == 1226)	//576i
					{
						bufTiming.scanType = 0;
						tmp8 = 74; //23+22+23
						bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
					}
				}
				else
				{
					if (bufTiming.scanType)		//progressive
					{
						tmp8 = (bufTiming.vTotal - bufTiming.vActive);
						bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
					}
					else					//interlace
					{
						if ( (bufTiming.vActive == 2228) ||(bufTiming.vActive == 1028) )		//1080i & 480i
						{
							tmp8 = 68; //23+22+23
							bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
						}
						else if (bufTiming.vActive == 1226)		//576i
						{
							tmp8 = 74; // 25+24+25
							bufTiming.vActive = (bufTiming.vActive - tmp8) >> 1;
						}
						else if (bufTiming.vActive == 1103)
						{
							tmp8 = 23; //23
							bufTiming.vActive = bufTiming.vActive - tmp8;
						}
					}
				}
#endif
			} break;

			case LX_HDMI_EXT_3D_SBSFULL:
				bufTiming.hActive = bufTiming.hActive >> 1;
				break;

			case LX_HDMI_EXT_3D_FIELD_ALTERNATIVE:
			{
				bufTiming.scanType = 0;

				if (bufTiming.vActive == 1103)		tmp8 = 23; //1080! - 22.5
				else if (bufTiming.vActive == 601) 	tmp8 = 25; //576! - 24.5
				else if (bufTiming.vActive == 503)	tmp8 = 23; //480! - 22.5
				else 								tmp8 = 0;

				bufTiming.vActive = bufTiming.vActive - tmp8;

			} break;

			case LX_HDMI_EXT_3D_LINE_ALTERNATIVE:
				bufTiming.vActive = bufTiming.vActive >> 1;
				break;

			case LX_HDMI_EXT_4K_2K:
				if ( !((bufTiming.hActive == 3840 && bufTiming.vActive == 2160) || (bufTiming.hActive == 4096 && bufTiming.vActive == 2160) ||\
					(bufTiming.hActive == 1920 && bufTiming.vActive == 2160 && tmdsClock > 29000) || \
					(bufTiming.hActive == 2048 && bufTiming.vActive == 2160 && tmdsClock > 29000) ) )
				{
					bufTiming.extInfo = LX_HDMI_EXT_2D_FORMAT;
					HDMI_VIDEO("[%d] %s : VSI is UD, but Timing Info is 2D format \n",  __L__, __F__);
				}
				break;

			case LX_HDMI_EXT_2D_FORMAT:
			case LX_HDMI_EXT_3D_L_DEPTH:
			case LX_HDMI_EXT_3D_L_GRAPHICS:
			case LX_HDMI_EXT_3D_TNB:
			case LX_HDMI_EXT_3D_SBS:
			case LX_HDMI_EXT_MAX:
			default:
				break;
		}

		/* Not support 3D 1920x1080!@50 of EIA-861D */
		if (eVideoFormat == LX_FORMAT_3D_FORMAT && (bufTiming.hTotal == 2304 ||bufTiming.hTotal == 4608))
		{
			memset(&bufTiming , 0 , sizeof(LX_HDMI_TIMING_INFO_T));
			HDMI_VIDEO("[%d] %s : Not support 3D 1920x1080i@50(1250)  \n",  __L__, __F__);
		}
	} while(0);

	if ( ( _gH14HDMI_thread_running == 0 &&  _gH14TimingReadCnt < 23 && _gH14TimingReadCnt > 20) ||
		(_gH14HDMIState >= HDMI_STATE_READ && _gH14TimingReadCnt < 6 && _gH14TimingReadCnt > 3) )
	{
		//HDMI_VIDEO(" VIC  = %d [gpreAVI - %d] \n", videoIdCode, __gH14PrevAVIPacket.eAviPacket.VideoIdCode);
		HDMI_VIDEO(" VideoFormat[%d]  3D Structure[%d] \n", eVideoFormat, e3DStructure);
		HDMI_VIDEO(" Timinginfo   buf hActive = %d // 	hActive = %d \n", bufTiming.hActive, info->hActive);
		HDMI_VIDEO(" Timinginfo   buf vActive = %d // 	vActive = %d \n", bufTiming.vActive, info->vActive);
		HDMI_VIDEO(" Timinginfo   buf scanType = %d //  scanType = %d \n", bufTiming.scanType, info->scanType);
		HDMI_VIDEO(" Timinginfo   buf full_3d = %d //  full_3d = %d \n", bufTiming.extInfo, info->extInfo);
		HDMI_VIDEO(" Timinginfo   Extend info([0]Normal[1]FP[2]FA[3]LA[4]SSF[5]LD[6]LDG[7]TNB[8]SBS[9]4K = %d \n", bufTiming.extInfo);
	}

	info->hTotal			= bufTiming.hTotal; 		///< Horizontal total pixels
	info->vTotal 			= bufTiming.vTotal; 		///< Vertical total lines
	info->hStart			= bufTiming.hStart; 		///< Horizontal start pixel (Back Porch)
	info->vStart 			= bufTiming.vStart;		///< Vertical start lines (Back Porch)
	info->hActive 		= bufTiming.hActive;		///< Horizontal active pixel
	info->vActive 		= bufTiming.vActive; 		///< Vertical active lines
	info->scanType 		= bufTiming.scanType; 		///< Scan type (0 : interlace, 1 : progressive) 	info->scanType ^= 1;
	info->extInfo			= bufTiming.extInfo; 		///< Ext format info

	HDMI_PRINT("[%d] %s : VideoFormat[%d]  3D Structure[%d] \n",  __L__, __F__, eVideoFormat, e3DStructure);

	return RET_OK;
}

static int _HDMI_H14_SetInternalMute(LX_HDMI_MUTE_CTRL_T interMute)
{
	BOOLEAN 	videoMuteState;

	LINK_H14_RdFL(system_control_02);
	LINK_H14_Rd01(system_control_02, reg_mute_vid, videoMuteState);

	//video related.
	if (interMute.eMode == LX_HDMI_VIDEO_MUTE || interMute.eMode == LX_HDMI_AV_MUTE)
	{
		if (interMute.bVideoMute != videoMuteState)
		{
			LINK_H14_Wr01(system_control_02, reg_mute_vid, interMute.bVideoMute);
			LINK_H14_WrFL(system_control_02);
			HDMI_VIDEO("[%d] %s : bVideoMute = %s \n", __L__, __F__, (interMute.bVideoMute ? "On" : "Off"));
		}
		else
			HDMI_VIDEO("[%d] %s : skip bVideoMute = %s \n", __L__, __F__, (interMute.bVideoMute ? "On" : "Off"));
	}

	//audio related.
	if (interMute.eMode == LX_HDMI_AUDIO_MUTE || interMute.eMode == LX_HDMI_AV_MUTE)
	{
		//Check a previous state
		if (_gH14AudioMuteState != interMute.bAudioMute)
		{
			if (interMute.bAudioMute == TRUE)
			{
				//Mute audio data
				LINK_H14_RdFL(audio_05);
				LINK_H14_Wr01(audio_05, reg_i2s_sd_en, 0x0);	//I2S SD Output Disable(4 Channel)
				LINK_H14_WrFL(audio_05);
			}
			else
			{
				//Un-mute audio data
				LINK_H14_RdFL(audio_05);
				LINK_H14_Wr01(audio_05, reg_i2s_sd_en, 0xF);	//I2S SD Output Enable(4 Channel)
				LINK_H14_WrFL(audio_05);
			}

			//Update _gH14AudioMuteState
			_gH14AudioMuteState = interMute.bAudioMute;

			HDMI_AUDIO("[%d] %s : bAudioMute = %s \n", __L__, __F__, (interMute.bAudioMute ? "On" : "Off"));
		}
	}

	return RET_OK;
}

static int _HDMI_H14_SetVideoBlank(LX_HDMI_AVI_COLORSPACE_T space)
{
	UINT32 blank_r = 0x0;
	UINT32 blank_b = 0x0;

	switch(space)
	{
		case LX_HDMI_AVI_COLORSPACE_YCBCR422:
		case LX_HDMI_AVI_COLORSPACE_YCBCR444:
		{
			blank_r = 0x800;
			blank_b = 0x800;
		}	break;

		case LX_HDMI_AVI_COLORSPACE_RGB:
		default:
		{
			blank_r = 0x0;
			blank_b = 0x0;
		}	break;
	}

	/* Blank Red */
	LINK_H14_RdFL(video_09);
	LINK_H14_Wr01(video_09, reg_vid_blank_r, blank_r);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_H14_WrFL(video_09);

	/* Blank Blue */
	LINK_H14_RdFL(video_10);
	LINK_H14_Wr01(video_10, reg_vid_blank_b, blank_b);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_H14_WrFL(video_10);

	HDMI_VIDEO("[%d] %s : changed video blank color space [%d]  \n",  __L__, __F__, space);
	return RET_OK;
}

static int _HDMI_H14_ClearSWResetAll(void)
{
	CTOP_CTRL_H14_RdFL(ctr00);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_hdmi_dto, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_sys, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_hdcp, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_tmds, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_vfifo_w, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_afifo_w, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_pix_pip, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_vfifo_r, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_aud, 0x1);
	CTOP_CTRL_H14_Wr01(ctr00, reg_afifo_r, 0x1);
	CTOP_CTRL_H14_WrFL(ctr00);

	OS_MsecSleep(1);	// ms delay

	CTOP_CTRL_H14_RdFL(ctr00);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_hdmi_dto, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_sys, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, swrst_hdcp, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_tmds, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_vfifo_w, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_afifo_w, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_pix_pip, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_vfifo_r, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_aud, 0x0);
	CTOP_CTRL_H14_Wr01(ctr00, reg_afifo_r, 0x0);
	CTOP_CTRL_H14_WrFL(ctr00);

	OS_MsecSleep(1);	// ms delay

	/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
	CTOP_CTRL_H14A0_RdFL(ctr01);
	CTOP_CTRL_H14A0_Wr01(ctr01,phy_i2c_clk_sel,1);
	CTOP_CTRL_H14A0_WrFL(ctr01);

	return RET_OK;
}

static int _HDMI_H14_GetPortConnection(void)
{
	static UINT32 portCnt = 0;
	UINT32 scdt = 0;
	UINT32 ctrl_repeat = 0;
	UINT32 noVsi_Intra = 0;
	BOOLEAN mode = 0;
	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_VIDEO_MUTE};

	LINK_H14_RdFL(system_control_00);
	LINK_H14_Rd01(system_control_00, reg_scdt, scdt);
	LINK_H14_Rd01(system_control_00, reg_hdmi_mode, mode);

	if (scdt == HDMI_PORT_CONNECTED)
	{
		if (portCnt != 0)		HDMI_DEBUG("[%d] %s : Connection \n", __LINE__, __func__);
			portCnt = 0;
	}
	else
	{
		mode = 0;
		if (portCnt < 3)
		{
			spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
			_gH14Intr_vf_stable = HDMI_VIDEO_INIT_STATE;
			_gH14Intr_avi = HDMI_AVI_INIT_STATE;
			_gH14Intr_vsi = HDMI_VSI_INIT_STATE;
			_gH14Intr_src_mute = HDMI_SOURCE_MUTE_CLEAR_STATE;
			_gH14TimingReadCnt = 0;
			_gH14AVIReadState 	  = FALSE;
			_gH14VSIState = FALSE;
			_gH14AVIChangeState = FALSE;
			spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

			_HDMI_H14_SetInternalMute(muteCtrl);	// Video All Clear

			///* Reset phy pll for pixel repetition  *///
			//CTOP_CTRL_H14A0_RdFL(ctr26);
			//CTOP_CTRL_H14A0_Rd01(ctr26, phy_ppll_sel, ctrl_repeat);

			if (ctrl_repeat)
			{
				//CTOP_CTRL_H14A0_Wr01(ctr26, phy_ppll_sel, 0x0);
				//CTOP_CTRL_H14A0_WrFL(ctr26);
			}

			LINK_H14_RdFL(interrupt_enable_00);
			LINK_H14_Rd01(interrupt_enable_00, intr_no_vsi_enable, noVsi_Intra);			///< 20 intr_no_vsi_enable

			if (noVsi_Intra != 0)
			{
				LINK_H14_Wr01(interrupt_enable_00, intr_no_vsi_enable, 0x0);			///< 20 intr_no_vsi_enable
				LINK_H14_WrFL(interrupt_enable_00);
				HDMI_DEBUG("[%d] %s : No VSI intra disable \n", __LINE__, __func__);
			}

			HDMI_DEBUG("[%d] %s : Dis Connection  [%d] \n", __LINE__, __func__,   portCnt);
			portCnt++;
		}
	}

	spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
	_gH14HDMIConnectState = scdt;
	gH14BootData->mode = mode;
	spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

	return RET_OK;
}

static int _HDMI_H14_PhyRunReset(void)
{
	/*
	PHY_REG_H14_RdFL(tmds_clk_inv);
	PHY_REG_H14_Wr01(tmds_clk_inv, tmds_clk_inv, 0x1);
	PHY_REG_H14_WrFL(tmds_clk_inv);
	*/

	// set eq at 2012-11-17
	//PHY_REG_H14_RdFL(dr_n1);
	//PHY_REG_H14_Wr01(dr_n1, dr_n1, 0x7);
	//PHY_REG_H14_WrFL(dr_n1);

	// HDMI_ADAPTIVE_LOOP_Manual
	PHY_REG_H14_RdFL(eq_cs_rs_sel);
	PHY_REG_H14_Wr01(eq_cs_rs_sel, eq_rs_man_sel, 0x1);
	PHY_REG_H14_Wr01(eq_cs_rs_sel, eq_cs_man_sel, 0x3);
	PHY_REG_H14_WrFL(eq_cs_rs_sel);

	//PHY_REG_H14_RdFL(eq_rs);
	//PHY_REG_H14_Wr01(eq_rs,eq_rs,0x5);		//2012-11-17 change 3 -> 5
	//PHY_REG_H14_WrFL(eq_rs);

	PHY_REG_H14_RdFL(cr_kvco_offset);
	PHY_REG_H14_Wr01(cr_kvco_offset, cr_kvco_offset, 0x1);		//2012-11-22 change 2 -> 1
	PHY_REG_H14_WrFL(cr_kvco_offset);

/*	// HDCP Problem to the Master for L9 -- need cheak  : default value 0x1
	PHY_REG_H14_RdFL(eq_af_en_avg_width);
	PHY_REG_H14_Wr01(eq_af_en_avg_width, eq_avg_width, 0x0);
	PHY_REG_H14_WrFL(eq_af_en_avg_width);
*/

	PHY_REG_H14_RdFL(idr_adj);
	PHY_REG_H14_Wr01(idr_adj, idr_adj, 0x1);
	PHY_REG_H14_WrFL(idr_adj);

	return RET_OK;
}

static int _HDMI_H14_RunReset(void)
{
	// Clock Divide for Pixel Repetition format
	LINK_H14_RdFL(video_00);
	LINK_H14_Wr01(video_00, reg_pr_cmu_sync, 0x1);
	LINK_H14_WrFL(video_00);

	// Negative polarity for H14 UD and 3D_SS_Full format
	// Support positive and negative polarity for H14 DE HDMI data bridge
	LINK_H14_RdFL(video_03);
	LINK_H14_Wr01(video_03, reg_neg_pol_en, 0x1);
	LINK_H14_WrFL(video_03);

	// HPD PIN Control Enable
	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, hpd0_oen, 0x0);
	LINK_H14_WrFL(phy_00);

/*	// set internal test hdcp key
	LINK_H14_RdFL((UINT32*)&glink_818_bx);
	LINK_H14_Wr01.reg_dbg_hdcp_key_bak_en,0x0)
	LINK_H14_WrFL((UINT32*)&glink_818_bx);
*/

	//ARC source
	//glink_0_bx.reg_arc_src_prt0 = 0;
	LINK_H14_RdFL(heac_00);
	LINK_H14_Wr01(heac_00, reg_arc_src, 0x0);
	LINK_H14_WrFL(heac_00);

	//Enable or Disable ARC port
	//ghpd_out_control_1_bx.phy0_phy_arc_pdb = arc->ARCEnable;
	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_arc_pdb, (UINT32)_gH14AudioArcStatus);
	LINK_H14_WrFL(phy_00);

	HDMI_DEBUG("[%d] %s :  ARC = %s \n", __L__, __F__, _gH14AudioArcStatus? "On":"Off");
	return RET_OK;
}

/**
* _HDMI_H14_SetAudio
*
* @parm void
* @return int
*/
static int _HDMI_H14_SetAudio(void)
{
	//ARC source
	LINK_H14_RdFL(heac_00);
	LINK_H14_Wr01(heac_00, reg_arc_src, 0x1);
	LINK_H14_WrFL(heac_00);

	//Channel 0
	LINK_H14_RdFL(system_control_00);
	LINK_H14_Wr01(system_control_00, reg_aac_en, 0x1);	//Auto Audio Path Configuration Enable(N, CTS value is auto configured.)
	LINK_H14_WrFL(system_control_00);

	LINK_H14_RdFL(audio_00);
	LINK_H14_Wr01(audio_00, reg_acr_en, 		 0x0);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_H14_Wr01(audio_00, reg_acr_n_fs, 		 0x1);	//0 : 128Fs, 1 : 256 Fs, 2 : 512Fs(default : 256Fs)
	LINK_H14_Wr01(audio_00, reg_acr_clk_aud_div, 0x1);	//0 : 128Fs, 1 : 256 Fs, 2 : 512Fs(default : 256Fs)
	LINK_H14_Wr01(audio_00, reg_acr_ncts_rx_en,  0x0);	//Use N, CTS value for audio clock generation
	LINK_H14_Wr01(audio_00, reg_acr_adj_thr, 	 0x3);	//Threshold Value for Clock Frequency Auto Adjustment for proper FIFO running, not required
	LINK_H14_Wr01(audio_00, reg_acr_adj_en, 	 0x1);	//Enable Clock Frequency Auto Adjustment for proper FIFO running
	LINK_H14_WrFL(audio_00);

	LINK_H14_RdFL(audio_05);
	LINK_H14_Wr01(audio_05, reg_i2s_sck_edge, 1);	//I2S Format for falling edge
	LINK_H14_Wr01(audio_05, reg_i2s_sd_en,  0xF);	//I2S SD Output Enable(4 Channel)
	LINK_H14_Wr01(audio_05, reg_i2s_out_en,   1);	//I2S Output Enable
	LINK_H14_Wr01(audio_05, reg_i2s_sd0_map,  0);	//I2S SD 0 Output Channel Mappings
	LINK_H14_Wr01(audio_05, reg_i2s_sd1_map,  1);	//I2S SD 1 Output Channel Mappings
	LINK_H14_Wr01(audio_05, reg_i2s_sd2_map,  2);	//I2S SD 2 Output Channel Mappings
	LINK_H14_Wr01(audio_05, reg_i2s_sd3_map,  3);	//I2S SD 3 Output Channel Mappings
	LINK_H14_WrFL(audio_05);

	//Set audio mute state
	_gH14AudioMuteState = FALSE;

	HDMI_AUDIO("[%d] %s\n", __L__, __F__);
	return RET_OK;
}


/**
 *  _HDMI_H14_GetAudioTypeAndFreq
 *
 *  @return int
*/static int _HDMI_H14_GetAudioInfo(void)
{
	int 	ret = RET_OK;

	ULONG	flags = 0;
	BOOLEAN bDebugEnabled = FALSE;	//< HDMI Debug Print Enabled

	UINT32	reg_scdt, reg_hdmi_mode, reg_burst_pc_0;

	UINT16	ui16VActive;		//< HDMI Vertical Active Size
	UINT16	ui16HActive;		//< HDMI horizontal Active Size
	UINT32	intrVideoState	= HDMI_STATE_DISABLE;
	UINT32	intrAudioState	= HDMI_AUDIO_INIT_STATE;
	UINT32	audioRecheckTime 	= HDMI_AUDIO_RECHECK_TIME_500MS;

	LX_HDMI_AUDIO_TYPE_T		audioType		= LX_HDMI_AUDIO_NO_AUDIO;		///< HDMI Audio Type.
	LX_HDMI_SAMPLING_FREQ_T 	samplingFreq	= LX_HDMI_SAMPLING_FREQ_NONE; 	///< Sampling Frequency

	LX_HDMI_SAMPLING_FREQ_T 	samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_NONE;	//< HDMI sampling frequency from TMDS clock
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_AUDIO_MUTE};

	//Increase _gH14HdmiAudioThreadCount;
	_gH14HdmiAudioThreadCount++;

	//Check HDMI port connection
	LINK_H14_RdFL(system_control_00);
	LINK_H14_Rd01(system_control_00, reg_scdt, reg_scdt);

	if (reg_scdt == 0)	//Check HDMI, DVI Sync Detect
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);

		//Clear _gH14IntrAudioState
		_gH14IntrAudioState = HDMI_AUDIO_INIT_STATE;

		//Clear _gH14HdmiFreqErrorCount
		_gH14HdmiFreqErrorCount = 0;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI / DVI Not Connected(state = %d)!!!\n", __L__, _gH14IntrAudioState);

		goto func_exit;
	}

	//spin lock for protection : lock
	spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);

	//copy video interrupt state
	intrVideoState = _gH14HDMIState;
	//intrVideoState = HDMI_STATE_READ;	//temp for test

	//copy audio interrupt state
	intrAudioState = _gH14IntrAudioState;

	//spin lock for protection : unlock
	spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

	//Check _gH14IntrAudioState is stably changed.
	if ( (intrAudioState == HDMI_AUDIO_STABLE_STATE && intrVideoState >= HDMI_STATE_READ)
	  ||(intrAudioState == HDMI_AUDIO_STABLE_STATE && _gH14HdmiPortStableCount > HDMI_AUDIO_PORT_STABLE_TIME_5S) )	//To protect a abnormal or transient video and audio info drop state.
	{
		if (_gH14HdmiAudioInfo.audioType == LX_HDMI_AUDIO_AC3_IEC60958)
			audioRecheckTime = HDMI_AUDIO_RECHECK_TIME_1S;
		else
			audioRecheckTime = HDMI_AUDIO_RECHECK_TIME_500MS;

		//Recheck for HDMI Audio Format and sampling frequency
		if ((_gH14HdmiAudioThreadCount % audioRecheckTime) == 0)	// 0.5 or 1 seconds, Thread calls a every 20 ms.
		{
			//Check a current mute status for workaround
			if (_gH14AudioMuteState == TRUE)
			{
				//Unmute audio data for abnormal state
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = FALSE;
				_HDMI_H14_SetInternalMute(muteCtrl);

				//Set a debug print status
				bDebugEnabled = TRUE;
			}

			//Get HDMI Audio Type and Sampling Frequency
			(void)_HDMI_H14_GetAudioTypeAndFreq(&audioType, &samplingFreq);

			//Get a sampling frequency from TMDS clock
			(void)_HDMI_H14_GetAudioFreqFromTmdsClock(&samplingFreqFromTmds);

			//H14 A0 IP Bug : non-PCM interrupt is not triggerred if non-PCM(AC-3) is changed to non-PCM(DTS).
			if ( (audioType			  != _gH14HdmiAudioInfo.audioType)
			  ||(samplingFreqFromTmds != _gH14HdmiAudioInfo.samplingFreq) )
			{
				HDMI_DEBUG("[%d]HDMI_GetAudioInfo : type = %d<-%d, tmds freq = %d<-%d, freq = %d, state = %d, mute = %d, PSC = %d(%d)\n", \
							__L__, audioType, _gH14HdmiAudioInfo.audioType, samplingFreqFromTmds, _gH14HdmiAudioInfo.samplingFreq,	\
							samplingFreq, _gH14IntrAudioState, _gH14AudioMuteState, _gH14HdmiPortStableCount, audioRecheckTime);

				//spin lock for protection for audio
				spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
				_gH14IntrAudioState = HDMI_AUDIO_INTERRUPT_STATE;
				_gH14HdmiFreqErrorCount = 0;
				spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

				//Mute audio data
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = TRUE;
				_HDMI_H14_SetInternalMute(muteCtrl);

				//Reset _gH14IntrBurstInfoCount
				_gH14IntrBurstInfoCount = 0;

				goto func_exit;
			}
		}

		//Increase _gH14HdmiPortStableCount(If HDMI port is changed, this count is cleared.)
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
		_gH14HdmiPortStableCount++;
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		//Debug print
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
		{
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : type = %d, freq = %d, state = %d, PSC = %d. mute = %d\n",	\
						__L__, _gH14HdmiAudioInfo.audioType, _gH14HdmiAudioInfo.samplingFreq,	\
						_gH14IntrAudioState, _gH14HdmiPortStableCount, _gH14AudioMuteState);
		}

		return ret;
	}
	else if (intrAudioState == HDMI_AUDIO_STABLE_STATE && intrVideoState < HDMI_STATE_READ)
	{
		HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_VIDEO_INTERRUPT_STATE(state A = %d, V = %d)!!!\n", __L__, _gH14IntrAudioState, _gH14Intr_vf_stable);

		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
		_gH14IntrAudioState = HDMI_AUDIO_INTERRUPT_STATE;
		_gH14HdmiFreqErrorCount = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);
	}
	else if (intrAudioState == HDMI_AUDIO_GET_INFO_STATE && intrVideoState >= HDMI_STATE_READ)
	{
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_AUDIO_GET_INFO_STATE(state A = %d, V = %d)!!!\n", __L__, _gH14IntrAudioState, _gH14Intr_vf_stable);
	}
	else if (intrAudioState < HDMI_AUDIO_GET_INFO_STATE && intrVideoState >= HDMI_STATE_READ)
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);

		//increase _gH14IntrAudioState
		_gH14IntrAudioState++;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_AUDIO_UNSTABLE_STATE(state A = %d, V = %d)!!!\n", __L__, _gH14IntrAudioState, _gH14Intr_vf_stable);

		goto func_exit;
	}
	else
	{
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI / DVI Not Connected(state = %d)!!!\n", __L__, _gH14IntrAudioState);

		goto func_exit;
	}

	//Get video vertical & horizontal active size
	LINK_H14_RdFL(video_05);
	LINK_H14_Rd01(video_05, reg_h_av, ui16HActive);
	LINK_H14_Rd01(video_05, reg_v_av, ui16VActive);

	//Check video active size
	if ( (ui16VActive <	240) || (ui16HActive <	320)	\
	  ||(ui16VActive > 2300) || (ui16HActive > 4096) )
	{
		HDMI_AUDIO("[%d]HDMI_GetAudioInfo : Video Active Size Error(v = %d, h = %d)!!!\n", \
					__L__, ui16VActive, ui16HActive);

		goto func_exit;
	}

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	LINK_H14_RdFL(system_control_00);
	LINK_H14_Rd01(system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	if (reg_hdmi_mode)
	{
		//Get HDMI Audio Type and Sampling Frequency
		(void)_HDMI_H14_GetAudioTypeAndFreq(&audioType, &samplingFreq);

		//Get a sampling frequency from TMDS clock
		(void)_HDMI_H14_GetAudioFreqFromTmdsClock(&samplingFreqFromTmds);

		//Get HDMI Audio Sampling Frequency from TMDS clock when audio sample is PCM
		if (audioType == LX_HDMI_AUDIO_PCM)
		{
			//Check a sampling frequency from status byte to TMDS clock
			//Channel status byte 0 value is 44.2Khz normal case or abnormal case.
			if ( (samplingFreq == LX_HDMI_SAMPLING_FREQ_44_1KHZ)
			  &&(samplingFreqFromTmds != LX_HDMI_SAMPLING_FREQ_44_1KHZ) )
			{
				//Check a HDMI Port Stable Count
				if (_gH14HdmiFreqErrorCount < HDMI_AUDIO_FREQ_ERROR_TIME_500MS)
				{
					//Increase _gH14HdmiFreqErrorCount.
					spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
					_gH14HdmiFreqErrorCount++;
					spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

					//Set a no audio mode
					audioType = LX_HDMI_AUDIO_NO_AUDIO;

					if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					{
						HDMI_ERROR("[%d]HDMI_GetAudioInfo : Freq. Error(freq = %d, tmds freq = %d)!!!\n",	\
									__L__, samplingFreq, samplingFreqFromTmds);
					}

					goto func_exit;
				}
				else
				{
					if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					{
						HDMI_ERROR("[%d]HDMI_GetAudioInfo : Freq. Error => Force 48KHz(freq = %d, tmds freq = %d)!!!\n",	\
									__L__, samplingFreq, samplingFreqFromTmds);
					}
				}
			}

			//Set a audio output mute when TMDS Fs is zero or not support.
			if ( (samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_NONE)
			   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_22_05KHZ)
			   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_24_KHZ)
			   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_768_KHZ))
			{
				//Set a no audio mode
				audioType = LX_HDMI_AUDIO_NO_AUDIO;

				if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					HDMI_ERROR("[%d]HDMI_GetAudioInfo : TMDS Fs Error(%d)!!!\n", __L__, samplingFreqFromTmds);

				goto func_exit;
			}
		}

		//Set a sampling frequency from TMDS clock
		samplingFreq = samplingFreqFromTmds;

		//Set a audio output mute when TMDS Fs is zero or not support.
		if ( (samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_NONE)
		   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_22_05KHZ)
		   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_24_KHZ)
		   ||(samplingFreqFromTmds == LX_HDMI_SAMPLING_FREQ_768_KHZ))
		{
			//Set a no audio mode
			audioType = LX_HDMI_AUDIO_NO_AUDIO;

			if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
				HDMI_ERROR("[%d]HDMI_GetAudioInfo : TMDS Fs Error(%d)!!!\n", __L__, samplingFreqFromTmds);

			goto func_exit;
		}

		//For debug print
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
		{
			HDMI_AUDIO("[%d]GetAudioInfo : type = %d, freq = %d, mute = %d, PSC = %d\n", \
						__L__, audioType, samplingFreq, _gH14AudioMuteState, _gH14HdmiPortStableCount);
		}
	}
	else
	{
		//Set DVI mode
		audioType	 = LX_HDMI_AUDIO_DVI;
		samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;
	}

	//Check a audio and video stable status
	if ( (audioType	  == _gH14HdmiAudioInfo.audioType)
	  &&(samplingFreq == _gH14HdmiAudioInfo.samplingFreq)
	  /* &&(_gH14HdmiAudioInfo.audioType >= LX_HDMI_AUDIO_PCM) */
	  &&(intrVideoState >= HDMI_STATE_READ) )
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);

		//Set a _gH14IntrAudioState
		_gH14IntrAudioState = HDMI_AUDIO_STABLE_STATE;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		//Set a debug print status
		bDebugEnabled = TRUE;
	}

	//For debug print
	if ( (audioType != _gH14HdmiAudioInfo.audioType) || (samplingFreq != _gH14HdmiAudioInfo.samplingFreq) )
	{
		if ( ((audioType == LX_HDMI_AUDIO_DVI) || (audioType == LX_HDMI_AUDIO_NO_AUDIO))	\
		  &&((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0) )
		{
			//Set a debug print status
			bDebugEnabled = TRUE;
		}
	}

	//Check a debug print status
	if (bDebugEnabled == TRUE)
	{
		//Get a vaild Pc data for payload
		LINK_H14_RdFL(audio_09);
		LINK_H14_Rd01(audio_09, reg_burst_pc_0, reg_burst_pc_0);

		HDMI_AUDIO("[%d]GetAudioInfo : type = %d(%d), freq = %d, mute = %d, PSC = %d\n", \
					__L__, audioType, reg_burst_pc_0 & 0x1F, samplingFreq, _gH14AudioMuteState, _gH14HdmiPortStableCount);
	}

func_exit:
	if (audioType == LX_HDMI_AUDIO_NO_AUDIO)
	{
		//Mute audio data
		muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
		muteCtrl.bAudioMute = TRUE;
		_HDMI_H14_SetInternalMute(muteCtrl);

		//Clears a global value for audio info.
		_gH14HdmiAudioInfo.audioType	= LX_HDMI_AUDIO_NO_AUDIO;
		_gH14HdmiAudioInfo.samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;

		//Clear _gH14HdmiPortStableCount
		_gH14HdmiPortStableCount = 0;

		//Debug print
		if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI Audio Mute On !!!\n", __L__);
	}
	else
	{
		//Update a global value when audio info. is stable.
		_gH14HdmiAudioInfo.audioType	= audioType;
		_gH14HdmiAudioInfo.samplingFreq = samplingFreq;
	}

	HDMI_ATASK("_H14_GetAudioInfo : type = %d, freq = %d(A = %d, V = %d)\n", audioType, samplingFreq, intrAudioState, intrVideoState);
	return RET_OK;
}

/**
 *  _HDMI_H14_GetAudioTypeAndFreq
 *
 *  @return int
*/
static int _HDMI_H14_GetAudioTypeAndFreq(LX_HDMI_AUDIO_TYPE_T *audioType, LX_HDMI_SAMPLING_FREQ_T *samplingFreq)
{
	int ret = RET_OK;

	UINT32	reg_hdmi_mode;
	UINT32	reg_achst_byte0, reg_achst_byte3;
	UINT32	reg_burst_pc_0;

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	LINK_H14_RdFL(system_control_00);
	LINK_H14_Rd01(system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	if (reg_hdmi_mode)
	{
		//Check if audio sample word used for other purposes than liner PCM.
		LINK_H14_RdFL(audio_07);
		LINK_H14_Rd01(audio_07, reg_achst_byte0, reg_achst_byte0);

		if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)	//bit 1, 0 : PCM, 1 : non-PCM
		{
			//Check a vaild Pc data for payload
			LINK_H14_RdFL(audio_09);
			LINK_H14_Rd01(audio_09, reg_burst_pc_0, reg_burst_pc_0);

			if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
			{
				//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
				switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK)	//bit 4 ~ 0
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
						*audioType = _gH14HdmiAudioInfo.audioType;
						if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
							HDMI_DEBUG("_HDMI_H14_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(audioType= %d)\n", *audioType);

						break;

					default:
						*audioType = LX_HDMI_AUDIO_NO_AUDIO;
						break;
				}

				//Debug print
				if ((reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) == BURST_INFO_AUDIO_TYPE_PAUSE)
				{
					if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
						HDMI_DEBUG("_HDMI_H14_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_PAUSE(Pc = %d)!!!\n", reg_burst_pc_0);
				}
			}	//if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0)
			else
			{
				*audioType = LX_HDMI_AUDIO_PCM;

				HDMI_ERROR("_HDMI_H14_GetAudioTypeAndFreq : Burst Info Error = %d\n", (reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK));
			}
		}	//if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)
		else
		{
			*audioType = LX_HDMI_AUDIO_PCM;

			/* Workaround Code for Skylife and DMT HDMI Repeater(Digital Stream, HDMI 210) Issue.(2013-11-02) */
			if (_gH14IntrBurstInfoCount != _gH14TaskBurstInfoCount)
			{
				//Update _gH14TaskBurstInfoCount value.
				_gH14TaskBurstInfoCount = _gH14IntrBurstInfoCount;

				/* Check a audio type in IEC61937 Burst Info. Packet. */
				//Check a vaild Pc data for payload
				LINK_H14_RdFL(audio_09);
				LINK_H14_Rd01(audio_09, reg_burst_pc_0, reg_burst_pc_0);

				if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0) //bit 7, 0 : No Error, 1 : Error
				{
					//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
					switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK)	//bit 4 ~ 0
					{
						case BURST_INFO_AUDIO_TYPE_AC3:
						case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
							*audioType = LX_HDMI_AUDIO_AC3_IEC60958;
							break;

						case BURST_INFO_AUDIO_TYPE_NULL:
							*audioType = _gH14HdmiAudioInfo.audioType;
							if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
								HDMI_DEBUG("_HDMI_H14_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(audioType= %d)\n", *audioType);
							break;

						default:
							*audioType = _gH14HdmiAudioInfo.audioType;
							break;
					}
				}

				//Check a audio type if AC-3 ES exist.
				if (*audioType == LX_HDMI_AUDIO_AC3_IEC60958)
				{
					//Check a audio type if first audio info get state for debug print.
					if (_gH14IntrAudioState == HDMI_AUDIO_GET_INFO_STATE)
					{
						HDMI_DEBUG("_HDMI_H14_GetAudioTypeAndFreq : Forced AC-3.(byte0 = 0x%X, Pc = 0x%X)\n", reg_achst_byte0, reg_burst_pc_0);
					}
				}
			}
			else
			{
				//Reset _gH14IntrBurstInfoCount
				_gH14IntrBurstInfoCount = 0;
			}
		}

		//Set Sampling frequency from IEC60958 Channel Status Byte 3
		LINK_H14_RdFL(audio_08);
		LINK_H14_Rd01(audio_08, reg_achst_byte3, reg_achst_byte3);

		switch(reg_achst_byte3 & HDMI_AUDIO_SAMPLE_BIT_MASK) 	//bit 0 ~ 3
		{
			case HDMI_AUDIO_SAMPLE_22_05KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_22_05KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_24_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_24_KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_32_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_32_KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_44_1KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_44_1KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_48_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_48_KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_88_2KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_88_2KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_96_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_96_KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_176_4KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_176_4KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_192_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_192_KHZ;
				break;

			case HDMI_AUDIO_SAMPLE_768_KHZ:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_768_KHZ;
				break;

			default:
				*samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;

				HDMI_AUDIO("_HDMI_H14_GetAudioTypeAndFreq(Channel Status) : samplingFreq = %d\n", *samplingFreq);
				break;
		}
	}
	else
	{
		//Set DVI mode
		*audioType	  = LX_HDMI_AUDIO_DVI;
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;
	}

	//Check a No Audio Mode
	if (*audioType == LX_HDMI_AUDIO_NO_AUDIO)
	{
		*samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;
	}

	HDMI_ATASK("_H14_GetAudioTypeAndFreq : type = %d, freq = %d\n", *audioType, *samplingFreq);
	return ret;
}

/**
 *  _HDMI_H14_GetAudioFreqFromTmdsClock
 *
 *  @return int
*/
static int _HDMI_H14_GetAudioFreqFromTmdsClock(LX_HDMI_SAMPLING_FREQ_T *samplingFreqFromTmds)
{
	int ret = RET_OK;

	UINT8		ui8TmdsClockHigh = 0;		//< HDMI measured clock value of TMDS clock for upper 8 bit
	UINT8		ui8TmdsClockLow = 0;		//< HDMI measured clock value of TMDS clock for low 8 bit

	UINT64		ui64TmdsClock = 0;			//< HDMI measured clock value of TMDS clock
	UINT64		ui64AcrN = 0;				//< HDMI ACR N value
	UINT64		ui64AcrCts = 0;				//< HDMI ACR CTS value
	UINT64		ui64TmdsSamplingFreq = 0;	//< HDMI sampling frequency in source device from TMDS clock

	//Get measured frequency value of TMDS clock.
	PHY_REG_H14_RdFL(tmds_freq_1);
	PHY_REG_H14_RdFL(tmds_freq_2);

	PHY_REG_H14_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
	PHY_REG_H14_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);

	ui64TmdsClock = (UINT64)((ui8TmdsClockHigh << 8) | ui8TmdsClockLow) * 10000;
	HDMI_AUDIO("Get TDMS Clock : ui64TmdsClock = %llu, ui8TmdsClockHigh = 0x%X, ui8TmdsClockLow = %d\n", ui64TmdsClock, ui8TmdsClockHigh, ui8TmdsClockLow);

	//Get ACR N H/W value.
	LINK_H14_RdFL(audio_01);
	LINK_H14_Rd01(audio_01, reg_acr_n_tx, ui64AcrN);

	//Get ACR CTS H/W value.
	LINK_H14_RdFL(audio_03);
	LINK_H14_Rd01(audio_03, reg_acr_cts_tx, ui64AcrCts);
	HDMI_AUDIO("Get TDMS ACR  : ui64AcrN = %llu, ui64AcrCts = %llu\n", ui64AcrN, ui64AcrCts);

	//Compute a sampling frequency from TMDS clock
	ui64TmdsSamplingFreq = ui64AcrN * ui64TmdsClock;

	//Check divide by zero value.
	if ( (ui64TmdsSamplingFreq > 0) && (ui64AcrCts > 0) )
	{
		do_div(ui64TmdsSamplingFreq, ui64AcrCts * 128);
	}

	//Mapping a sampling frequency from measuring from TMDS clock and ACR N & CTS H/W value
	if (ui64TmdsSamplingFreq == 0)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_NONE;
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_NONE\n");
	}
	else if (ui64TmdsSamplingFreq < 22983)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_22_05KHZ;	//  22.05 kHz(not supported)
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_22_05KHZ(not supported)\n");
	}
	else if (ui64TmdsSamplingFreq < 30000)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_24_KHZ;	//  24 kHz(not supported)
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_24_KHZ(not supported)\n");
	}
	else if (ui64TmdsSamplingFreq < 33800)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_32_KHZ;	//  32 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_32KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 45965)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_44_1KHZ;	//  44.1 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_44_1KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 67000)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_48_KHZ;	//  48 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_48_KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 91935)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_88_2KHZ;	//  88.2 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_88_2KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 135000)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_96_KHZ;	//  96 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_96_KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 183870)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_176_4KHZ;	//  176.4 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_176_4KHZ\n");
	}
	else if (ui64TmdsSamplingFreq < 210000)
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_192_KHZ;	//  192 kHz
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_192_KHZ\n");
	}
	else
	{
		*samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_768_KHZ;	//  768 kHz(not supported)
		HDMI_PRINT("Get Fs from TDMS clock => LX_HDMI_SAMPLING_FREQ_768_KHZ(not supported)\n");
	}

	HDMI_ATASK("Get Fs from TDMS clock : %llu => %d\n", ui64TmdsSamplingFreq, *samplingFreqFromTmds);
//	HDMI_ERROR("Get Fs from TDMS clock : %llu => %d\n", ui64TmdsSamplingFreq, *samplingFreqFromTmds);

	return ret;
}

/**
 *  _HDMI_H14_DebugAudioInfo
 *
 *  @return int
*/
static int _HDMI_H14_DebugAudioInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo)
{
	int ret = RET_OK;

	UINT8		ui8TmdsClockHigh = 0;		//< HDMI measured clock value of TMDS clock for upper 8 bit
	UINT8		ui8TmdsClockLow = 0;		//< HDMI measured clock value of TMDS clock for low 8 bit

	UINT64		ui64TmdsClock = 0;			//< HDMI measured clock value of TMDS clock
	UINT64		ui64AcrN = 0;				//< HDMI ACR N value
	UINT64		ui64AcrCts = 0;				//< HDMI ACR CTS value
	UINT64		ui64TmdsSamplingFreq = 0;	//< HDMI sampling frequency in source device from TMDS clock

	UINT32		reg_achst_byte0, reg_achst_byte1, reg_achst_byte2, reg_achst_byte3, reg_achst_byte4;
	UINT32		reg_burst_pc_0, reg_burst_pc_1, reg_burst_pd_0, reg_burst_pd_1;

	UINT32		sampling_freq, ext_sampling_freq;
	UINT16		burst_info_pc = 0, burst_info_pd = 0;

	//Get measured frequency value of TMDS clock.
	PHY_REG_H14_RdFL(tmds_freq_1);
	PHY_REG_H14_RdFL(tmds_freq_2);

	PHY_REG_H14_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
	PHY_REG_H14_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	ui64TmdsClock = (UINT64)((ui8TmdsClockHigh << 8) | ui8TmdsClockLow) * 10000;
	HDMI_DEBUG("Get TDMS Clock : ui64TmdsClock = %llu, ui8TmdsClockHigh = 0x%X, ui8TmdsClockLow = %d\n", ui64TmdsClock, ui8TmdsClockHigh, ui8TmdsClockLow);

	//Get ACR N H/W value.
	LINK_H14_RdFL(audio_01);
	LINK_H14_Rd01(audio_01, reg_acr_n_tx, ui64AcrN);

	//Get ACR CTS H/W value.
	LINK_H14_RdFL(audio_03);
	LINK_H14_Rd01(audio_03, reg_acr_cts_tx, ui64AcrCts);
	HDMI_DEBUG("Get TDMS ACR  : ui64AcrN = %llu, ui64AcrCts = %llu\n", ui64AcrN, ui64AcrCts);

	//Compute a sampling frequency from TMDS clock
	ui64TmdsSamplingFreq = ui64AcrN * ui64TmdsClock;

	//Check divide by zero value.
	if ( (ui64TmdsSamplingFreq > 0) && (ui64AcrCts > 0) )
	{
		do_div(ui64TmdsSamplingFreq, ui64AcrCts * 128);
	}

	HDMI_DEBUG("TMDS Clock = %llu, ACR N = %llu, ACR CTS = %llu, Tmds Fs = %llu\n",	\
				ui64TmdsSamplingFreq, ui64AcrN, ui64AcrCts, ui64TmdsSamplingFreq);

	//Read  reg_achst_byte0 reg.
	LINK_H14_RdFL(audio_07);
	LINK_H14_Rd01(audio_07, reg_achst_byte0, reg_achst_byte0);

	//Check IEC60958 Channel Status Byte0
	HDMI_DEBUG("IEC60958 Channel Status Byte0 = 0x%X(0x%X)\n", reg_achst_byte0, LINK_H14_Rd(audio_07));

	//Read  reg_achst_byte1 ~ 4 reg.
	LINK_H14_RdFL(audio_08);
	LINK_H14_Rd01(audio_08, reg_achst_byte1, reg_achst_byte1);
	LINK_H14_Rd01(audio_08, reg_achst_byte2, reg_achst_byte2);
	LINK_H14_Rd01(audio_08, reg_achst_byte3, reg_achst_byte3);
	LINK_H14_Rd01(audio_08, reg_achst_byte4, reg_achst_byte4);

	//Check IEC60958 Channel Status Byte1 ~ 4
	HDMI_DEBUG("IEC60958 Channel Status Byte1 = 0x%X, Byte2 = 0x%X, Byte3 = 0x%X, Byte4 = 0x%X(0x%X)\n",	\
				reg_achst_byte1, reg_achst_byte2, reg_achst_byte3, reg_achst_byte4, LINK_H14_Rd(audio_08));


	if (reg_achst_byte0 & 0x1)
		HDMI_DEBUG("Consumer use of channel status block is error!!!\n");
	else
		HDMI_DEBUG("Consumer use of channel status block.\n");


	if (reg_achst_byte0 & 0x2)
		HDMI_DEBUG("Audio sample word used for other purposes than liner PCM.\n");
	else
		HDMI_DEBUG("Audio sample word used for liner PCM.\n");


	if (reg_achst_byte0 & 0x4)
		HDMI_DEBUG("Software for which no copyright is asserted.\n");
	else
		HDMI_DEBUG("Software for which copyright is asserted.\n");


	HDMI_DEBUG("Category code  = 0x%X\n", reg_achst_byte1);

	HDMI_DEBUG("Source number  = %d\n", reg_achst_byte2 & 0x0F);


	HDMI_DEBUG("Channel number = %d\n", reg_achst_byte2 & 0xF0);
	if ((reg_achst_byte2 & 0xF0) == 0x10)
		HDMI_DEBUG("Left channel for stereo channel format.\n");

	if ((reg_achst_byte2 & 0xF0) == 0x20)
		HDMI_DEBUG("Right channel for stereo channel format.\n");

	//Set Sampling frequency from IEC60958 Channel Status Byte 3
	sampling_freq = reg_achst_byte3 & HDMI_AUDIO_SAMPLE_BIT_MASK; 	//bit 0 ~ 3
	HDMI_DEBUG("Sampling frequency = %d\n", sampling_freq);

	//Get a extension sampling frequency from IEC60958 Channel Status Byte 3
	ext_sampling_freq = (reg_achst_byte3 >> 6) | (reg_achst_byte3 & HDMI_AUDIO_SAMPLE_BIT_MASK);
	HDMI_DEBUG("Extension sampling frequency = %d\n", ext_sampling_freq & HDMI_AUDIO_EXT_SAMPLE_BIT_MASK);

	switch(ext_sampling_freq & HDMI_AUDIO_EXT_SAMPLE_BIT_MASK) 	//bit 0 ~ 3, bit 6 ~7
	{
		case HDMI_AUDIO_EXT_SAMPLE_384_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_384_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_1536_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_1536_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_1024_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_1024_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_3528_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_3528_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_7056_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_7056_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_14112_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_14112_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_64_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_64_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_128_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_128_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_256_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_256_KHZ\n");
			break;

		case HDMI_AUDIO_EXT_SAMPLE_512_KHZ:
			HDMI_DEBUG("HDMI_AUDIO_EXT_SAMPLE_512_KHZ\n");
			break;

		default:
			HDMI_DEBUG("ext_sampling_freq = %d\n", ext_sampling_freq);
			break;
	}


	if ((reg_achst_byte3 & 0x0F) == 0)
		HDMI_DEBUG("Symbol frequency = 64 X 44.1KHz = 2.8224 MHz.\n");

	if ((reg_achst_byte3 & 0x0F) == 2)
		HDMI_DEBUG("Symbol frequency = 64 X 48KHz = 3.072 MHz.\n");

	if ((reg_achst_byte3 & 0x0F) == 3)
		HDMI_DEBUG("Symbol frequency = 64 X 32KHz = 2.048 MHz.\n");


	if ((reg_achst_byte3 & 0x30) == 0x00)
		HDMI_DEBUG("Clock accuracy is Level II.\n");

	if ((reg_achst_byte3 & 0x30) == 0x10)
		HDMI_DEBUG("Clock accuracy is Level I.\n");

	if ((reg_achst_byte3 & 0x30) == 0x20)
		HDMI_DEBUG("Clock accuracy is Level III.\n");

	if ((reg_achst_byte3 & 0x30) == 0x30)
		HDMI_DEBUG("Interface frame rate not matched to sampling frequency.\n");


	if (reg_achst_byte4 & 0x01)
		HDMI_DEBUG("Maximum audio sample word length is 24 bits.\n");
	else
		HDMI_DEBUG("Maximum audio sample word length is 20 bits.\n");


	if ((reg_achst_byte4 & 0x01) == 0)
		HDMI_DEBUG("Sample word lenth is not indicated.\n");

	if ((reg_achst_byte4 & 0x01) == 1)
		HDMI_DEBUG("Sample word lenth is 20 or 16 bits.\n");

	if ((reg_achst_byte4 & 0x0E) == 2)
		HDMI_DEBUG("Sample word lenth is 22 or 18 bits.\n");

	if ((reg_achst_byte4 & 0x0E) == 4)
		HDMI_DEBUG("Sample word lenth is 23 or 19 bits.\n");

	if ((reg_achst_byte4 & 0x0E) == 10)
		HDMI_DEBUG("Sample word lenth is 24 or 20 bits.\n");

	if ((reg_achst_byte4 & 0x0E) == 12)
		HDMI_DEBUG("Sample word lenth is 21 or 17 bits.\n");


	if ((reg_achst_byte4 >> 4) == 16)
		HDMI_DEBUG("Original sampling frequency is 44.1 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 7)
		HDMI_DEBUG("Original sampling frequency is 88.2 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 11)
		HDMI_DEBUG("Original sampling frequency is 22.05 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 3)
		HDMI_DEBUG("Original sampling frequency is 176.4 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 13)
		HDMI_DEBUG("Original sampling frequency is 48 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 5)
		HDMI_DEBUG("Original sampling frequency is 96 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 9)
		HDMI_DEBUG("Original sampling frequency is 24 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 1)
		HDMI_DEBUG("Original sampling frequency is 192 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 14)
		HDMI_DEBUG("Original sampling frequency is 128 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 6)
		HDMI_DEBUG("Original sampling frequency is 8 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 10)
		HDMI_DEBUG("Original sampling frequency is 11.025 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 2)
		HDMI_DEBUG("Original sampling frequency is 12 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 12)
		HDMI_DEBUG("Original sampling frequency is 32 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 4)
		HDMI_DEBUG("Original sampling frequency is 64 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 8)
		HDMI_DEBUG("Original sampling frequency is 16 kHz.\n");

	if ((reg_achst_byte4 >> 4) == 0)
		HDMI_DEBUG("Original sampling frequency is not indicated.\n");


	//Check IEC61937 Burst Info
	if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)
	{
		//Get a vaild Pc, Pd data for payload
		LINK_H14_RdFL(audio_09);
		LINK_H14_Rd01(audio_09, reg_burst_pc_0, reg_burst_pc_0);
		LINK_H14_Rd01(audio_09, reg_burst_pc_1, reg_burst_pc_1);
		LINK_H14_Rd01(audio_09, reg_burst_pd_0, reg_burst_pd_0);
		LINK_H14_Rd01(audio_09, reg_burst_pd_1, reg_burst_pd_1);

		burst_info_pc = (UINT16)((reg_burst_pc_1 << 8)|reg_burst_pc_0);
		burst_info_pd = (UINT16)((reg_burst_pd_1 << 8)|reg_burst_pd_0);

		HDMI_DEBUG("IEC61937 Burst Info Pc = 0x%X, Pd = 0x%X\n", burst_info_pc, burst_info_pd);

		if (burst_info_pc & BURST_INFO_PAYLOAD_ERROR_BIT_MASK)
			HDMI_DEBUG("Error-flag indicationg that the burst-payload may contain errors.\n");
		else
			HDMI_DEBUG("Error-flag indicationg a vaild burst-payload.\n");


		//Set Audio Data-Types according to IEC61937-2 Burst Info Preamble C
		switch(burst_info_pc & BURST_INFO_AUDIO_TYPE_BIT_MASK)	//bit 4 ~ 0
		{
			case BURST_INFO_AUDIO_TYPE_AC3:
			case BURST_INFO_AUDIO_TYPE_AC3_ENHANCED:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_AC3_X(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_DTS_I:
			case BURST_INFO_AUDIO_TYPE_DTS_II:
			case BURST_INFO_AUDIO_TYPE_DTS_III:
			case BURST_INFO_AUDIO_TYPE_DTS_IV:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_DTS_X(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_MPEG1_L1:
			case BURST_INFO_AUDIO_TYPE_MPEG1_L23:
			case BURST_INFO_AUDIO_TYPE_MPEG2_EXT:
			case BURST_INFO_AUDIO_TYPE_MPEG2_L1:
			case BURST_INFO_AUDIO_TYPE_MPEG2_L2:
			case BURST_INFO_AUDIO_TYPE_MPEG2_L3:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_MPEG1_XX(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_MPEG2_AAC:
			case BURST_INFO_AUDIO_TYPE_MPEG2_AAC_LOW:
			case BURST_INFO_AUDIO_TYPE_MPEG4_AAC:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_MPEG2_AAC_XX(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_ATRAC:
			case BURST_INFO_AUDIO_TYPE_ATRAC_23:
			case BURST_INFO_AUDIO_TYPE_ATRAC_X:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_ATRAC_XX(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_WMA_I_IV:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_WMA_I_IV(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_MAT:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_MAT(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_NULL:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_NULL(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_SMPTE_338M:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_SMPTE_338M(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			case BURST_INFO_AUDIO_TYPE_PAUSE:
				HDMI_DEBUG("BURST_INFO_AUDIO_TYPE_PAUSE(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;

			default:
				HDMI_DEBUG("LX_HDMI_DEBUG_NO_AUDIO(Audio type in Pc = %d)\n", burst_info_pc & 0x001F);
				break;
		}

		HDMI_DEBUG("Data-type-dependent info. = %d\n", burst_info_pc & BURST_INFO_DEPENDENT_INFO_BIT_MASK);
		HDMI_DEBUG("Bitstream number = %d\n", burst_info_pc & BURST_INFO_STREAM_NUMBER_BIT_MASK);


		HDMI_DEBUG("Length-code : Number of bits = %d\n", burst_info_pd);
	}

	//Copy a HMDI audio debug information.
	pAudioDebugInfo->tmdsClock	 	  = ui64TmdsClock;
	pAudioDebugInfo->tmdsSamplingFreq = ui64TmdsSamplingFreq;

	//Copy a HMDI audio channel status byte debug information.
	pAudioDebugInfo->chanStatusByte0  = (UINT8)reg_achst_byte0;
	pAudioDebugInfo->chanStatusByte1  = (UINT8)reg_achst_byte1;
	pAudioDebugInfo->chanStatusByte2  = (UINT8)reg_achst_byte2;
	pAudioDebugInfo->chanStatusByte3  = (UINT8)reg_achst_byte3;
	pAudioDebugInfo->chanStatusByte4  = (UINT8)reg_achst_byte4;

	//Copy a HMDI audio burst info. debug information.
	pAudioDebugInfo->burstInfoPc	  = burst_info_pc;
	pAudioDebugInfo->burstInfoPd	  = burst_info_pd;

	return ret;
}


static int _HDMI_H14_Thread(void *data)
{
	while(1)
	{
		_HDMI_H14_GetPortConnection();	//check connection
		_HDMI_H14_Periodic_Task();	//Video Task
		_HDMI_H14_GetAudioInfo();	//Audio Task

		OS_MsecSleep(HDMI_THREAD_TIMEOUT);

		if (_gH14Force_thread_sleep > 1)
		{
			HDMI_PRINT("User force sleep HDMI Thread\n");
			//_gH14Force_thread_sleep  = 1;
			_gH14HDMI_thread_running = 0;
			//interruptible_sleep_on(&WaitQueue_HDMI_H14);
			wait_event_interruptible(WaitQueue_HDMI_H14, _gH14Force_thread_sleep == 0);
		}
	}
	return RET_OK;
}

static int _HDMI_H14_EnableInterrupt(void)
{
	LINK_H14_RdFL(interrupt_enable_00);
	LINK_H14_Wr01(interrupt_enable_00, intr_new_aud_enable, 0x0);		///< 0 intr_new_aud_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_new_spd_enable, 0x0);		///< 1 intr_new_spd_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_new_unr_enable, 0x0);		///< 2 intr_new_unr_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_cts_chg_enable, 0x0);			///< 3 intr_cts_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_n_chg_enable, 0x0);			///< 4 intr_n_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_fs_chg_enable, 0x1);			///< 5 intr_fs_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_2pcm_chg_enable, 0x1);		///< 6 intr_2pcm_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_2npcm_chg_enable, 0x1);		///< 7 intr_2npcm_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_spdif_err_enable, 0x0);		///< 8 intr_spdif_err_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_afifo_undr_enable, 0x0);		///< 9 intr_afifo_undr_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_afifo_ovrr_enable, 0x0);		///< 10 intr_afifo_ovrr_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_burst_info_enable, 0x1);		///< 11 intr_burst_info_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_vf_stable_enable, 0x1);		///< 12 intr_vf_stable_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_vid_chg_enable, 0x1);			///< 13 intr_vid_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_vr_chg_enable, 0x1);			///< 14 intr_vr_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_hr_chg_enable, 0x1);			///< 15 intr_hr_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_po_chg_enable, 0x0);			///< 16 intr_po_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_il_chg_enable, 0x0);			///< 17 intr_il_chg_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_vfifo_undr_enable, 0x0);		///< 18 intr_vfifo_undr_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_vfifo_ovrr_enable, 0x0);		///< 19 intr_vfifo_ovrr_enable
	LINK_H14_Wr01(interrupt_enable_00, intr_no_vsi_enable, 0x0);			///< 20 intr_no_vsi_enable
	//intr_achst_5b_chg_int_enable is not working, so disabled.
	LINK_H14_Wr01(interrupt_enable_00, intr_achst_5b_chg_int_enable, 0x0);	///< 21 intr_achst_5b_chg_int_enable
	LINK_H14_WrFL(interrupt_enable_00);

	LINK_H14_RdFL(interrupt_enable_01);
	LINK_H14_Wr01(interrupt_enable_01, intr_plug_in_enable, 0x0);			///< 0 intr_plug_in_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_plug_out_enable, 0x0);		///< 1 intr_plug_out_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_scdt_rise_enable, 0x0);		///< 2 intr_scdt_rise_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_scdt_fall_enable, 0x0);			///< 3 intr_scdt_fall_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_vsync_enable, 0x0);			///< 4 intr_vsync_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_hdmi_mode_enable, 0x0);		///< 5 intr_hdmi_mode_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_set_mute_enable, 0x1);		///< 6 intr_set_mute_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_clr_mute_enable, 0x0);		///< 7 intr_clr_mute_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_auth_init_enable, 0x0);		///< 8 intr_auth_init_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_auth_done_enable, 0x0);		///< 9 intr_auth_done_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_hdcp_err_enable, 0x0);		///< 10 intr_hdcp_err_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_ecc_err_enable, 0x0);			///< 11 intr_ecc_err_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_terc4_err_enable, 0x0);		///< 12 intr_terc4_err_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_acr_err_enable, 0x0);			///< 13 intr_acr_err_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_asp_err_enable, 0x0);			///< 14 intr_asp_err_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_no_avi_enable, 0x0);			///< 15 intr_no_avi_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_no_gcp_enable, 0x0);			///< 16 intr_no_gcp_enable

	LINK_H14_Wr01(interrupt_enable_01, intr_chg_avi_b12345_enable, 0x1);		///< 17 intr_chg_avi_b12345_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_chg_avi_to_rgb_enable, 0x1);		///< 18 intr_chg_avi_to_rgb_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_chg_avi_to_ycc444_enable, 0x1);	///< 19 intr_chg_avi_to_ycc444_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_chg_avi_to_ycc422_enable, 0x1);	///< 20 intr_chg_avi_to_ycc422_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_avi_enable, 0x0);				///< 21 intr_new_avi_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_vsi_enable, 0x0);				///< 22 intr_new_vsi_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_chg_vsi_vformat_enable, 0x1);		///< 23 intr_chg_vsi_vformat_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_gcp_enable, 0x0);			///< 24 intr_new_gcp_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_asp_enable, 0x0);			///< 25 intr_new_asp_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_acr_enable, 0x1);				///< 26 intr_new_acr_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_gbd_enable, 0x0);			///< 27 intr_new_gbd_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_acp_enable, 0x0);			///< 28 intr_new_acp_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_isrc1_enable, 0x0);			///< 29 intr_new_isrc1_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_isrc2_enable, 0x0);			///< 30 intr_new_isrc2_enable
	LINK_H14_Wr01(interrupt_enable_01, intr_new_mpg_enable, 0x0);			///< 31 intr_new_mpg_enable
	LINK_H14_WrFL(interrupt_enable_01);

	if (_gH14HDMI_interrupt_registered < 1)
	{
		if (request_irq(H14_IRQ_HDMI, _HDMI_H14_IRQHandler, 0, "hdmi", NULL))
		{
			HDMI_ERROR("request_irq is failed\n");
		}
		else
			_gH14HDMI_interrupt_registered = 1;
	}

	HDMI_DEBUG("HDMI_H14_EnableInterrupt\n");
	return RET_OK;
}

int HDMI_H14_DisableInterrupt(void)
{
	LINK_H14_RdFL(interrupt_enable_00);
	LINK_H14_Wr(interrupt_enable_00, 0x0);
	LINK_H14_WrFL(interrupt_enable_00);

	LINK_H14_RdFL(interrupt_enable_01);
	LINK_H14_Wr(interrupt_enable_01, 0x0);
	LINK_H14_WrFL(interrupt_enable_01);

	free_irq(H14_IRQ_HDMI, NULL);
	HDMI_DEBUG("HDMI_H14_DisableInterrupt\n");

	return RET_OK;
}

/**
 *
 * HDMI_L9Bx_irq_handler irq handler
 *
 * @param	irq , device id , regs
 * @return	0 : OK , -1 : NOK
 *
*/
irqreturn_t _HDMI_H14_IRQHandler(int irq, void *dev)
{
	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_MUTE_NONE};
	LX_HDMI_AVI_COLORSPACE_T	csc = LX_HDMI_AVI_COLORSPACE_RGB;

	UINT32 intra0 = 0;
	UINT32 intra1 = 0;
	UINT16 hTotal = 0, vTotal = 0, hActive = 0, vActive = 0;

	UINT64	elapsedTime = 0;

	LINK_H14_RdFL(interrupt_00);
	intra0 = LINK_H14_Rd(interrupt_00);

	LINK_H14_RdFL(interrupt_01);
	intra1 = LINK_H14_Rd(interrupt_01);

	HDMI_INTR("----- intra0 = 0x%x ---- intra1 = 0x%x  -----\n",intra0, intra1);

	//if ((intra0 &0x100000)>>20)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vf_stable))
	{
		HDMI_DEBUG("intr_vf_stable\n");

		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_vf_stable = HDMI_VIDEO_INTERRUPT_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
		_gH14IntrAudioState = HDMI_AUDIO_INTERRUPT_STATE;
		_gH14HdmiFreqErrorCount = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		if (_gH14HDMIState >= HDMI_STATE_READ)
		{
			HDMI_TASK("[%d] %s : -Changed -  _gH14Intr_vf_stable [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] / PSC[%d]\n", __L__, __F__, _gH14HDMIState, _gH14HdmiPortStableCount );
			spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
			_gH14Intr_vf_stable = HDMI_VIDEO_READ_STATE;
			//_gH14HDMIState = HDMI_STATE_READ;
			_gH14TimingReadCnt = 0;
			_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
			_gH14AVIReadState = FALSE;
			spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

			//intr_vf_stable is abnormally toggled from UP player source in UD Model for abnormal chip only.(2013-06-25)
			//Check a HDMI Port Stable Count
			if ( (CHK_UHD_BD() && _gH14HdmiPortStableCount > HDMI_AUDIO_RECHECK_TIME_500MS)	\
			  ||(CHK_FHD_BD() && _gH14HdmiPortStableCount > HDMI_AUDIO_PORT_STABLE_TIME_5S) )
			{
				//spin lock for protection for audio
				spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
				_gH14IntrAudioState = HDMI_AUDIO_STABLE_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);
			}
			else
			{
				//Mute audio data
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = TRUE;
				_HDMI_H14_SetInternalMute(muteCtrl);
			}
		}
		else
		{
			//Mute audio data
			muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
			muteCtrl.bAudioMute = TRUE;
			_HDMI_H14_SetInternalMute(muteCtrl);
		}
	}

	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_b12345))
	{
		HDMI_DEBUG("intr_chg_avi_b12345\n");
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
		_gH14AVIReadState = FALSE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

		if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_rgb))
		{
			HDMI_INTR("intr_chg_avi_to_rgb\n");
			csc = LX_HDMI_AVI_COLORSPACE_RGB;
		}

		if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_ycc444))
		{
			HDMI_INTR("intr_chg_avi_to_ycc444\n");
			csc = LX_HDMI_AVI_COLORSPACE_YCBCR444;
		}

		if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_ycc422))
		{
			HDMI_INTR("intr_chg_avi_to_ycc422\n");
			csc = LX_HDMI_AVI_COLORSPACE_YCBCR422;
		}

		HDMI_TASK("[%d][HDMI_AVI] state [%d]  preCSS[%d] CSC = %d \n", __L__, _gH14HDMIState, _gH14PrevPixelEncoding, csc);

		if (_gH14HDMIState >= HDMI_STATE_READ)//(_gH14Intr_vf_stable == HDMI_VIDEO_STABLE_STATE) 			// for PS3 CSC
		{
			if (_gH14PrevPixelEncoding != csc)
			{
				//spin lock for protection
				spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
				_gH14Intr_avi = HDMI_AVI_CHANGE_STATE;
				_gH14TimingReadCnt = 0;
				_gH14AVIReadState = FALSE;
				_gH14AVIChangeState = TRUE;
				spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

				HDMI_TASK("[%d] %s : -Changed  - _gH14Intr_avi [STABLE_STATE] => [CHANGE_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__,_gH14HDMIState );
				muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
				muteCtrl.bVideoMute = TRUE;
				_HDMI_H14_SetInternalMute(muteCtrl);
			}
		}
	}

	if (LINK_H14_Rd_fld(interrupt_01,intr_set_mute))
	{
		HDMI_INTR("intr_set_mute\n");

		//if (_gH14Intr_vf_stable > HDMI_VIDEO_INIT_STATE)
		{
			if (_gH14Intr_src_mute != HDMI_SOURCE_MUTE_STATE)
			{
				muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
				muteCtrl.bVideoMute = TRUE;
				_HDMI_H14_SetInternalMute(muteCtrl);

				spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
				_gH14Intr_src_mute = HDMI_SOURCE_MUTE_STATE;
				_gH14TimingReadCnt = 0;
				spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

				HDMI_DEBUG("[%d] intr_set_mute : HDMI_SOURCE MUTE Enable[%d] \n", __L__, _gH14Intr_src_mute );
			}
		}
	}

	if (LINK_H14_Rd_fld(interrupt_01,intr_no_vsi))
	{
		HDMI_TASK("intr_no_vsi\n");
		HDMI_TASK("[%d] %s : -Changed  - _gH14Intr_vsi [STABLE_STATE] => [NO_DATA_STAT]     /   HDMI_STABLE[%d] \n", __L__, __F__,_gH14HDMIState );
		HDMI_TASK("[%d] %s : -Changed  - _gH14Intr_vf_stable [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__,_gH14HDMIState );
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_vsi = HDMI_VSI_NO_DATA_STATE;
		_gH14Intr_vf_stable = HDMI_VIDEO_READ_STATE;
		_gH14TimingReadCnt = 0;
		_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
		_gH14AVIReadState = FALSE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

		LINK_H14_RdFL(interrupt_enable_01);
		LINK_H14_Wr01(interrupt_enable_00, intr_no_vsi_enable, 0x0);			///< 20 intr_no_vsi_enable
		LINK_H14_WrFL(interrupt_enable_00);

		HDMI_DEBUG("[%d] %s : No VSI intra disable  \n", __LINE__, __func__);
	}

	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_vsi_vformat))
	{
		HDMI_DEBUG("intr_chg_vsi_vformat\n");
		spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
		_gH14Intr_vsi = HDMI_VSI_INTERRUPT_STATE;
		_gH14VSIState = TRUE;
		spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

		// when  don't change Sync and change VSI on 3D for OPTIMUS Phone ( 3D / LTE /MHL )
		if (_gH14HDMIState > HDMI_STATE_READ)
		{
			HDMI_TASK("[%d] %s : -Changed  - _gH14Intr_vf_stable [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__,_gH14HDMIState );
			spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
			_gH14Intr_vf_stable = HDMI_VIDEO_READ_STATE;
			_gH14TimingReadCnt = 0;
			_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
			_gH14AVIReadState = FALSE;
			spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
		}
	}

//audio related. => start
	if (LINK_H14_Rd_fld(interrupt_00, intr_new_acr))
	{
		HDMI_INTR("intr_new_acr\n");

		/* interrupt_enable_01 reg. is not read because of H14 A0 chip bug, so use global variable. */
		LINK_H14_RdFL(interrupt_enable_01);
		LINK_H14_Wr01(interrupt_enable_01, intr_scdt_fall_enable, 0x1);		///< 3 intr_scdt_fall_enable
		LINK_H14_Wr01(interrupt_enable_01, intr_new_acr_enable, 0x0);		///< 26 intr_new_acr_enable
		LINK_H14_WrFL(interrupt_enable_01);

		//To protect EMI issue, ACR is disabled in DVI mode because of avoiding abnormal clock generation.
		LINK_H14_RdFL(audio_00);
		LINK_H14_Wr01(audio_00, reg_acr_en, 0x1);	//ACR Enable(Audio Clock Generation Function Activation)
		LINK_H14_WrFL(audio_00);

		HDMI_INTR("[%d]IRQHandler : ACR ON.\n", __L__);
	}

	if (LINK_H14_Rd_fld(interrupt_01, intr_scdt_fall))
	{
		HDMI_INTR("intr_scdt_fall\n");

		/* interrupt_enable_01 reg. is not read because of H14 A0 chip bug, so use global variable. */
		LINK_H14_RdFL(interrupt_enable_01);
		LINK_H14_Wr01(interrupt_enable_01, intr_scdt_fall_enable, 0x0);		///< 3 intr_scdt_fall_enable
		LINK_H14_Wr01(interrupt_enable_01, intr_new_acr_enable, 0x1);		///< 26 intr_new_acr_enable
		LINK_H14_WrFL(interrupt_enable_01);

		//To protect EMI issue, ACR is disabled in DVI mode because of avoiding abnormal clock generation.
		LINK_H14_RdFL(audio_00);
		LINK_H14_Wr01(audio_00, reg_acr_en, 0x0);	//ACR Disable(Audio Clock Generation Function Activation)
		LINK_H14_WrFL(audio_00);

		HDMI_INTR("[%d]IRQHandler : ACR OFF.\n", __L__);
	}

	//intr_fs_chg is normal toggled in FHD Model.
	if (CHK_FHD_BD() && (intra0 & HDMI_AUDIO_INTERRUPT_BIT_MASK))
	{
		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
		_gH14IntrAudioState = HDMI_AUDIO_INTERRUPT_STATE;
		_gH14HdmiFreqErrorCount = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

		//Mute audio data
		muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
		muteCtrl.bAudioMute = TRUE;
		_HDMI_H14_SetInternalMute(muteCtrl);

		if (LINK_H14_Rd_fld(interrupt_00, intr_fs_chg))
			HDMI_INTR("intr_fs_chg\n");

		if (LINK_H14_Rd_fld(interrupt_00, intr_2pcm_chg))
			HDMI_INTR("intr_2pcm_chg\n");

		if (LINK_H14_Rd_fld(interrupt_00, intr_2npcm_chg))
			HDMI_INTR("intr_2npcm_chg\n");
	}

	//intr_fs_chg is abnormally toggled from UD player source in UD Model.
	if (CHK_UHD_BD() && (intra0 & HDMI_AUDIO_INTERRUPT_BIT_MASK))
	{
		if (LINK_H14_Rd_fld(interrupt_00, intr_fs_chg))
			HDMI_INTR("intr_fs_chg\n");

		if (LINK_H14_Rd_fld(interrupt_00, intr_2pcm_chg))
			HDMI_INTR("intr_2pcm_chg\n");

		if (LINK_H14_Rd_fld(interrupt_00, intr_2npcm_chg))
			HDMI_INTR("intr_2npcm_chg\n");
	}

	if (LINK_H14_Rd_fld(interrupt_00,intr_burst_info))
	{
		//Get a elapsed mili-second time.
		elapsedTime = jiffies_to_msecs(jiffies - _gH14BurstInfoPrevJiffies);

		//Save _gH14BurstInfoPrevJiffies
		_gH14BurstInfoPrevJiffies = jiffies;

		if (_gH14IntrBurstInfoCount == 0 && elapsedTime > HDMI_AUDIO_BURST_INFO_RECHECK_80MS)
		{
			//spin lock for protection for audio
			spin_lock_irqsave(&_gIntrHdmiH14AudioLock, flags);
			_gH14IntrAudioState = HDMI_AUDIO_INTERRUPT_STATE;
			_gH14HdmiFreqErrorCount = 0;
			spin_unlock_irqrestore(&_gIntrHdmiH14AudioLock, flags);

			//Mute audio data
			muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
			muteCtrl.bAudioMute = TRUE;
			_HDMI_H14_SetInternalMute(muteCtrl);

			if (_gH14HdmiPortStableCount > HDMI_AUDIO_PORT_STABLE_TIME_5S)
			{
				//Check a vaild Pc data for payload
				LINK_H14_RdFL(audio_09);
				HDMI_DEBUG("_gH14IntrBurstInfoCount(%llu), Pd_Pc = 0x%X, elapsedTime = %llu\n",	\
							_gH14IntrBurstInfoCount, LINK_H14_Rd(audio_09), elapsedTime);
			}
		}

		//Increase _gH14IntrBurstInfoCount
		_gH14IntrBurstInfoCount++;

		HDMI_INTR("intr_burst_info(%llu)\n", _gH14IntrBurstInfoCount);

		//if ((_gH14HdmiAudioThreadCount % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
			//HDMI_DEBUG("intr_burst_info(%llu), elapsedTime = %llu\n", _gH14IntrBurstInfoCount, elapsedTime);
	}
	else
	{
		//_gH14IntrBurstInfoCount is normal toggled in FHD Model.
		if (CHK_FHD_BD())
		{
			//Reset _gH14IntrBurstInfoCount
			_gH14IntrBurstInfoCount = 0;
		}
		else
		{
			//Check a vaild Pc data for payload
			LINK_H14_RdFL(audio_09);
			HDMI_INTR("_gH14IntrBurstInfoCount(%llu), Pd_Pc = 0x%X\n", _gH14IntrBurstInfoCount, LINK_H14_Rd(audio_09));
		}
	}
//audio related. => end.

	if (LINK_H14_Rd_fld(interrupt_00,intr_vid_chg) || LINK_H14_Rd_fld(interrupt_00,intr_vr_chg) || LINK_H14_Rd_fld(interrupt_00,intr_hr_chg) )
	{
		if (LINK_H14_Rd_fld(interrupt_00,intr_vid_chg))		HDMI_INTR("intr_vid_chg\n");
		if (LINK_H14_Rd_fld(interrupt_00,intr_vr_chg))		HDMI_INTR("intr_vr_chg\n");
		if (LINK_H14_Rd_fld(interrupt_00,intr_hr_chg))		HDMI_INTR("intr_hr_chg\n");

		LINK_H14_RdFL(video_04);
		LINK_H14_Rd01(video_04, reg_h_tot, hTotal);
		LINK_H14_Rd01(video_04, reg_v_tot, vTotal);

		LINK_H14_RdFL(video_05);
		LINK_H14_Rd01(video_05, reg_h_av, hActive);
		LINK_H14_Rd01(video_05, reg_v_av, vActive);

		HDMI_INTR("hTotal = %d, vTotal = %d, hActive = %d, vActive = %d\n", hTotal, vTotal, hActive, vActive);
	}

#if 0	//Not used interrupt is disabled.
	//if ((intra0 &0x1)>>0) 	//	if (pstLinkReg->interrupt_00.intr_new_gcp)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_gcp))
	{
		HDMI_INTR("intr_new_gcp\n");
	}

	//if ((intra0 &0x2)>>1) 	//	if (pstLinkReg->interrupt_00.intr_new_asp)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_asp))
	{
		HDMI_INTR("intr_new_asp\n");
	}

#if 0
	//if ((intra0 &0x4)>>2) 	//	if (pstLinkReg->interrupt_00.intr_new_acr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_acr))
	{
		HDMI_INTR("intr_new_acr\n");
	}
#endif

	//if ((intra0 &0x8)>>3) 	//	if (pstLinkReg->interrupt_00.intr_new_gbd)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_gbd))
	{
		HDMI_INTR("intr_new_gbd\n");
	}

	//if ((intra0 &0x10)>>4) 	//	if (pstLinkReg->interrupt_00.intr_new_acp)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_acp))
	{
		HDMI_INTR("intr_new_acp\n");
	}

	//if ((intra0 &0x20)>>5)	//	if (pstLinkReg->interrupt_00.intr_new_isrc1)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_isrc1))
	{
		HDMI_INTR("intr_new_isrc1\n");
	}

	//if ((intra0 &0x40)>>6)	//	if (pstLinkReg->interrupt_00.intr_new_isrc2)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_isrc2))
	{
		HDMI_INTR("intr_new_isrc2\n");
	}

	//if ((intra0 &0x80)>>7)	//	if (pstLinkReg->interrupt_00.intr_new_mpg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_mpg))
	{
		HDMI_INTR("intr_new_mpg\n");
	}

	//if ((intra0 &0x100)>>8)	//	if (pstLinkReg->interrupt_00.intr_new_aud)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_aud))
	{
		HDMI_INTR("intr_new_aud\n");
	}

	//if ((intra0 &0x200)>>9)	//	if (pstLinkReg->interrupt_00.intr_new_spd)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_spd))
	{
		HDMI_INTR("intr_new_spd\n");
	}

	//if ((intra0 &0x400)>>10)	//	if (pstLinkReg->interrupt_00.intr_new_unr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_new_unr))
	{
		HDMI_INTR("intr_new_unr\n");
	}

	//if ((intra0 &0x800)>>11)	//	if (pstLinkReg->interrupt_00.intr_cts_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_cts_chg))
	{
		HDMI_INTR("intr_cts_chg\n");
	}

	//if ((intra0 &0x1000)>>12)	//	if (pstLinkReg->interrupt_00.intr_n_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_n_chg))
	{
		HDMI_INTR("intr_n_chg\n");
	}

#if 0
	//if ((intra0 &0x2000)>>13)	//	if (pstLinkReg->interrupt_00.intr_fs_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_fs_chg))
	{
		HDMI_PRINT("intr_fs_chg\n");
	}

	//if ((intra0 &0x4000)>>14)	//	if (pstLinkReg->interrupt_00.intr_2pcm_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_2pcm_chg))
	{
		HDMI_PRINT("intr_2pcm_chg\n");
	}

	//if ((intra0 &0x8000)>>15)	//	if (pstLinkReg->interrupt_00.intr_2npcm_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_2npcm_chg))
	{
		HDMI_PRINT("intr_2npcm_chg\n");
	}
#endif

	//if ((intra0 &0x10000)>>16)	//	if (pstLinkReg->interrupt_00.intr_spdif_err)
	if (LINK_H14_Rd_fld(interrupt_00,intr_spdif_err))
	{
		HDMI_INTR("intr_spdif_err\n");
	}

	//if ((intra0 &0x20000)>>17)	//	if (pstLinkReg->interrupt_00.intr_afifo_undr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_afifo_undr))
	{
		HDMI_INTR("intr_afifo_undr\n");
	}

	//if ((intra0 &0x40000)>>18)	//	if (pstLinkReg->interrupt_00.intr_afifo_ovrr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_afifo_ovrr))
	{
		HDMI_INTR("intr_afifo_ovrr\n");
	}

	//if ((intra0 &0x80000)>>19)	//	if (pstLinkReg->interrupt_00.intr_burst_info)
	if (LINK_H14_Rd_fld(interrupt_00,intr_burst_info))
	{
		HDMI_INTR("intr_burst_info\n");
	}

#if 0
	//if ((intra0 &0x100000)>>20) 	//	if (pstLinkReg->interrupt_00.intr_vf_stable)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vf_stable))
	{
		HDMI_DEBUG("intr_vf_stable\n");
	}
#endif

	//if ((intra0 &0x200000)>>21)	//	if (pstLinkReg->interrupt_00.intr_vid_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vid_chg))
	{
		HDMI_INTR("intr_vid_chg\n");
	}

	//if ((intra0 &0x400000)>>22)	//	if (pstLinkReg->interrupt_00.intr_vr_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vr_chg))
	{
		HDMI_INTR("intr_vr_chg\n");
	}

	//if ((intra0 &0x800000)>>23)	//	if (pstLinkReg->interrupt_00.intr_hr_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_hr_chg))
	{
		HDMI_INTR("intr_hr_chg\n");
	}

	//((intra0 &0x1000000)>>24)	//	if (pstLinkReg->interrupt_00.intr_po_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_po_chg))
	{
		HDMI_INTR("intr_po_chg\n");
	}

	//((intra0 &0x2000000)>>25)	//	if (pstLinkReg->interrupt_00.intr_il_chg)
	if (LINK_H14_Rd_fld(interrupt_00,intr_il_chg))
	{
		HDMI_INTR("intr_il_chg\n");
	}

	//((intra0 &0x4000000)>>26)	//	if (pstLinkReg->interrupt_00.intr_vfifo_undr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vfifo_undr))
	{
		HDMI_INTR("intr_vfifo_undr\n");
	}

	//((intra0 &0x8000000)>>27)	//	if (pstLinkReg->interrupt_00.intr_vfifo_ovrr)
	if (LINK_H14_Rd_fld(interrupt_00,intr_vfifo_ovrr))
	{
		HDMI_INTR("intr_vfifo_ovrr\n");
	}

	//((intra0 &0x10000000)>>28)	//	if (pstLinkReg->interrupt_00.intr_achst_5b_chg_int)
	if (LINK_H14_Rd_fld(interrupt_00,intr_achst_5b_chg_int))
	{
		HDMI_PRINT("intr_achst_5b_chg_int\n");
	}

///////////////////////////////////////////////////////////////////////////////
	//if ((intra1 &0x1)>>0) 	//	if (pstLinkReg->interrupt_01.intr_plug_in)
	if (LINK_H14_Rd_fld(interrupt_01,intr_plug_in))
	{
		HDMI_INTR("intr_plug_in\n");
	}

	//if ((intra1 &0x2)>>1) 	//	if (pstLinkReg->interrupt_01.intr_plug_in)
	if (LINK_H14_Rd_fld(interrupt_01,intr_plug_out))
	{
		HDMI_INTR("intr_plug_out\n");
	}

	//if ((intra1 &0x4)>>2) 	//	if (pstLinkReg->interrupt_01.intr_scdt_rise)
	if (LINK_H14_Rd_fld(interrupt_01,intr_scdt_rise))
	{
		HDMI_INTR("intr_scdt_rise\n");
	}

#if 0
	//if ((intra1 &0x8)>>3) 	//	if (pstLinkReg->interrupt_01.intr_scdt_fall)
	if (LINK_H14_Rd_fld(interrupt_01,intr_scdt_fall))
	{
		HDMI_INTR("intr_scdt_fall\n");
	}
#endif

	//if ((intra1 &0x10)>>4) 	//	if (pstLinkReg->interrupt_01.intr_vsync)
	if (LINK_H14_Rd_fld(interrupt_01,intr_vsync))
	{
		HDMI_INTR("intr_vsync\n");
	}

	//if ((intra1 &0x20)>>5) 	//	if (pstLinkReg->interrupt_01.intr_hdmi_mode)
	if (LINK_H14_Rd_fld(interrupt_01,intr_hdmi_mode))
	{
		HDMI_INTR("intr_hdmi_mode\n");
	}

#if 0
	//if ((intra1 &0x40)>>6) 	//	if (pstLinkReg->interrupt_01.intr_set_mute)
	if (LINK_H14_Rd_fld(interrupt_01,intr_set_mute))
	{
		HDMI_PRINT("intr_set_mute\n");
	}
#endif

	//if ((intra1 &0x80)>>7) 	//	if (pstLinkReg->interrupt_01.intr_clr_mute)
	if (LINK_H14_Rd_fld(interrupt_01,intr_clr_mute))
	{
		HDMI_INTR("intr_clr_mute\n");
	}

	//if ((intra1 &0x100)>>8) 	//	if (pstLinkReg->interrupt_01.intr_auth_init)
	if (LINK_H14_Rd_fld(interrupt_01,intr_auth_init))
	{
		HDMI_INTR("intr_auth_init\n");
	}

	//if ((intra1 &0x200)>>9) 	//	if (pstLinkReg->interrupt_01.intr_auth_done)
	if (LINK_H14_Rd_fld(interrupt_01,intr_auth_done))
	{
		HDMI_INTR("intr_auth_done\n");
	}

	//if ((intra1 &0x400)>>10) 	//	if (pstLinkReg->interrupt_01.intr_hdcp_err)
	if (LINK_H14_Rd_fld(interrupt_01,intr_hdcp_err))
	{
		HDMI_INTR("intr_hdcp_err\n");
	}

	//if ((intra1 &0x800)>>11) 	//	if (pstLinkReg->interrupt_01.intr_ecc_err)
	if (LINK_H14_Rd_fld(interrupt_01,intr_ecc_err))
	{
		HDMI_INTR("intr_ecc_err\n");
	}

	//if ((intra1 &0x1000)>>12) 	//	if (pstLinkReg->interrupt_01.intr_terc4_err)
	if (LINK_H14_Rd_fld(interrupt_01,intr_terc4_err))
	{
		HDMI_INTR("intr_terc4_err\n");
	}

	//if ((intra1 &0x2000)>>13) 	//	if (pstLinkReg->interrupt_01.intr_acr_err)
	if (LINK_H14_Rd_fld(interrupt_01,intr_acr_err))
	{
		HDMI_INTR("intr_acr_err\n");
	}

	//if ((intra1 &0x4000)>>14) 	//	if (pstLinkReg->interrupt_01.intr_asp_err)
	if (LINK_H14_Rd_fld(interrupt_01,intr_asp_err))
	{
		HDMI_INTR("intr_asp_err\n");
	}

	//if ((intra1 &0x8000)>>15) 	//	if (pstLinkReg->interrupt_01.intr_no_avi)
	if (LINK_H14_Rd_fld(interrupt_01,intr_no_avi))
	{
		HDMI_INTR("intr_no_avi\n");
	}

	//if ((intra1 &0x10000)>>16) 	//	if (pstLinkReg->interrupt_01.intr_no_gcp)
	if (LINK_H14_Rd_fld(interrupt_01,intr_no_gcp))
	{
		HDMI_INTR("intr_no_gcp\n");
	}

#if 0
	//if ((intra1 &0x20000)>>17) 	//	if (pstLinkReg->interrupt_01.intr_chg_avi_b12345)
	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_b12345))
	{
		HDMI_DEBUG("intr_chg_avi_b12345\n");
	}

	//if ((intra1 &0x40000)>>18) 	//	if (pstLinkReg->interrupt_01.intr_chg_avi_to_rgb)
	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_rgb))
	{
		HDMI_DEBUG("intr_chg_avi_to_rgb\n");
	}

	//if ((intra1 &0x80000)>>19) 	//	if (pstLinkReg->interrupt_01.intr_chg_avi_to_ycc444)
	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_ycc444))
	{
		HDMI_DEBUG("intr_chg_avi_to_ycc444\n");
	}

	//if ((intra1 &0x100000)>>20) 	//	if (pstLinkReg->interrupt_01.intr_chg_avi_to_ycc422)
	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_avi_to_ycc422))
	{
		HDMI_DEBUG("intr_chg_avi_to_ycc422\n");
	}
#endif

	//if ((intra1 &0x200000)>>21) 	//	if (pstLinkReg->interrupt_01.intr_new_avi)
	if (LINK_H14_Rd_fld(interrupt_01,intr_new_avi))
	{
		HDMI_INTR("intr_new_avi\n");
	}

	//if ((intra1 &0x400000)>>22) 	//	if (pstLinkReg->interrupt_01.intr_new_vsi)
	if (LINK_H14_Rd_fld(interrupt_01,intr_new_vsi))
	{
		HDMI_INTR("intr_new_vsi\n");
	}

#if 0
	//if ((intra1 &0x800000)>>23) 	//	if (pstLinkReg->interrupt_01.intr_chg_vsi_vformat)
	if (LINK_H14_Rd_fld(interrupt_01,intr_chg_vsi_vformat))
	{
		HDMI_DEBUG("intr_chg_vsi_vformat\n");
	}

	//if ((intra1 &0x1000000)>>24) 	//	if (pstLinkReg->interrupt_01.intr_no_vsi)
	if (LINK_H14_Rd_fld(interrupt_01,intr_no_vsi))
	{
		HDMI_PRINT("intr_no_vsi\n");
	}
#endif

#endif	//#if 0	//Not used interrupt is disabled.

	if (intra0)
	{
		LINK_H14_Wr(interrupt_00, intra0);
		LINK_H14_WrFL(interrupt_00);
	}

	if (intra1)
	{
		LINK_H14_Wr(interrupt_01, intra1);
		LINK_H14_WrFL(interrupt_01);
	}

	return IRQ_HANDLED;
}

static void _HDMI_H14_Periodic_Task(void)
{
	static UINT32 gPeriodicState = HDMI_STATE_IDLE;
	static UINT32 gCheakCnt = 0;		// check count after vf stable interrupt
	static UINT32 gMuteCnt = 0;			// check count for  mute
	static UINT32 gAviCnt = 0;			// check count for checking AVI Data	in the HDMI mode
	static UINT32 gAviChgCnt = 0;		// check count after changing CSC in stable status
	static UINT32 gScdtCnt = 0;		// check count after connected

	UINT32 muteClear = 0;
	UINT32 aviHeader = 0;
	UINT32 temp = 0;
	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 	muteCtrl = {FALSE, FALSE, LX_HDMI_MUTE_NONE};

	do {
		if (_gH14HDMIState == HDMI_STATE_IDLE && _gH14HDMIState == HDMI_STATE_DISABLE)
			gPeriodicState = _gH14HDMIState;

		if (_gH14HDMIConnectState == HDMI_PORT_NOT_CONNECTED)			// change check connection in the thread function by 20121206
		{
			gCheakCnt = gMuteCnt = gAviCnt = gAviChgCnt = gScdtCnt = 0;
			gPeriodicState = HDMI_STATE_NO_SCDT;
		}
		else
		{
			if (_gH14HDMIState <= HDMI_STATE_NO_SCDT)
			{
				gPeriodicState = HDMI_STATE_SCDT;
				HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_DISABLE  => HDMI_STATE_SCDT[%d] \n", __L__, __F__, gPeriodicState);
			}

			///* start Source Mute Control  *///
			if (_gH14Intr_src_mute == HDMI_SOURCE_MUTE_STATE)
			{
				LINK_H14_RdFL(packet_34);
				LINK_H14_Rd01(packet_34, reg_pkt_gcp_cmute, muteClear);
				LINK_H14_Rd01(packet_34, reg_pkt_gcp_smute, temp);

				gMuteCnt++;
				if (muteClear == 1  || gMuteCnt >= 150)
				{
					spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
					_gH14Intr_src_mute = HDMI_SOURCE_MUTE_CLEAR_STATE;
					spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

					gPeriodicState = HDMI_STATE_CHECK_MODE;

					muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
					muteCtrl.bVideoMute = FALSE;
					_HDMI_H14_SetInternalMute(muteCtrl);

					HDMI_DEBUG("[%d] %s : -source mute clear  cnt [%d] state[%d]  \n", __L__, __F__, gMuteCnt, gPeriodicState);
					gCheakCnt =  gMuteCnt = 0;
				}
				else
				{
					if (gPeriodicState != HDMI_STATE_CHECK_SOURCE_MUTE)
					{
						HDMI_TASK("[%d] %s : -Changed- HDMI_STATE [%d]  => HDMI_STATE_CHECK_SOURCE_MUTE \n", __L__, __F__, gPeriodicState);
						gPeriodicState = HDMI_STATE_CHECK_SOURCE_MUTE;
					}
				}
			}
			///* end Source Mute Control  *///
			if (_gH14AVIChangeState == TRUE)
			{
				HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_[%d]  => HDMI_STATE_CHECK_AVI_CHG \n", __L__, __F__, gPeriodicState);
				spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
				_gH14AVIChangeState = FALSE;
				//_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
				gPeriodicState = HDMI_STATE_CHECK_AVI_CHG;
				gAviChgCnt = 0;
			}

			if (_gH14Intr_vf_stable == HDMI_VIDEO_READ_STATE)
			{
				HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_[%d]  => HDMI_STATE_READ && _gH14Intr_vf_stable[READ_STATE => INTERRUPT_STATE] \n", __L__, __F__, gPeriodicState);
				gPeriodicState = HDMI_STATE_READ;
				spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
				_gH14Intr_vf_stable = HDMI_VIDEO_INTERRUPT_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
			}
		}

		switch(gPeriodicState)
		{
			case HDMI_STATE_NO_SCDT:
				break;

			case HDMI_STATE_SCDT:		// if (HDMI_SCDT_COUNT == 4)  5 call
				{
					if (gScdtCnt < HDMI_SCDT_COUNT)	gScdtCnt++;
					else
					{
						gScdtCnt++;
						if (_gH14Intr_vf_stable == HDMI_VIDEO_INTERRUPT_STATE)
						{
							gPeriodicState = HDMI_STATE_CHECK_MODE;
							HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_SCDT  => HDMI_STATE_CHECK_MODE with HDMI_VIDEO_INTERRUPT_STATE \n", __L__, __F__);
						}

						if (gScdtCnt > 50)			// when no vf_stable Interrupt
						{
							gPeriodicState = HDMI_STATE_CHECK_MODE;
							spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
							_gH14Intr_vf_stable = HDMI_VIDEO_INTERRUPT_STATE;
							if (_gH14Intr_avi == HDMI_AVI_INIT_STATE)		_gH14Intr_avi = HDMI_AVI_INTERRUPT_STATE;
							if (_gH14Intr_vsi == HDMI_VSI_INIT_STATE)		_gH14Intr_vsi = HDMI_VSI_INTERRUPT_STATE;
							spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);

							HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_SCDT  => HDMI_STATE_CHECK_MODE with No vf_stable Interrupt \n", __L__, __F__);
						}
					}
					HDMI_TASK("[%d] %s : HDMI_STATE_SCDT scdtCnt = [%d] \n", __L__, __F__, gScdtCnt);
				}
				break;

			case HDMI_STATE_INTE_CHECK:
			case HDMI_STATE_CHECK_MODE:		// all interrupt cheak
			case HDMI_STATE_CHECK_SOURCE_MUTE:
			case HDMI_STATE_CHECK_AVI_NO:
				{
					///*  HDMI_STATE_CHECK_SOURCE_MUTE *///
					if (_gH14Intr_src_mute == HDMI_SOURCE_MUTE_STATE)
					{
						HDMI_TASK("[%d] %s : HDMI_STATE [HDMI_STATE_CHECK_SOURCE_MUTE(%d)] \n", __L__, __F__, gPeriodicState);
						break;
					}

					///*  HDMI_STATE_CHECK_NO_AVI   //If HDMI mode and AVI no packet, timing info is all '0'*///
					if (gH14BootData->mode == 1)
					{
						gAviCnt ++;
						if (_gH14Intr_avi <= HDMI_AVI_INTERRUPT_STATE &&  gAviCnt < 50)		// No read AVI
						{
							LINK_H14_RdFL(packet_18);
							aviHeader = LINK_H14_Rd(packet_18);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
							aviHeader = (aviHeader &0xffff);

							if (aviHeader == 0)
							{
								gPeriodicState = HDMI_STATE_CHECK_AVI_NO;
								HDMI_TASK("*[%d] %s : No AVI Packet - before AVI Init state\n", __L__, __F__);
								break;
							}
						}
					}
					///*  HDMI_STATE_CHECK_MODE && HDMI_STATE_CHECK_AVI_CHG *///
					if (gCheakCnt > HDMI_CHEAK_COUNT)
					{
						gPeriodicState = HDMI_STATE_READ;
						HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_INTE_CHECK[%d] => HDMI_STATE_READ \n", __L__, __F__, gPeriodicState);
					}
					gCheakCnt++;

					HDMI_TASK("[%d] %s : HDMI State = [%d]  cheakCnt = %d\n", __L__, __F__, gPeriodicState, gCheakCnt);
				}
				break;

			case HDMI_STATE_CHECK_AVI_CHG:
				{
					if (gAviChgCnt > 4)
					{
						muteCtrl.eMode	= LX_HDMI_VIDEO_MUTE;
						muteCtrl.bVideoMute = FALSE;			// mute Clear
						_HDMI_H14_SetInternalMute(muteCtrl);

						gPeriodicState = HDMI_STATE_CHECK_MODE;
						gAviChgCnt = gCheakCnt =  0;
						HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_CHECK_AVI_CHG  => HDMI_STATE_CHECK_MODE[%d] \n", __L__, __F__, gPeriodicState);
					}
					gAviChgCnt++;
				}
				break;

			case HDMI_STATE_READ:
				{
					if (_gH14TimingReadCnt < 2)
					{
						muteCtrl.eMode	= LX_HDMI_VIDEO_MUTE;
						muteCtrl.bVideoMute = FALSE;			// mute Clear
						_HDMI_H14_SetInternalMute(muteCtrl);
						gAviChgCnt = gCheakCnt =  0;
					}
					else if (_gH14TimingReadCnt > HDMI_READ_COUNT)
						gPeriodicState = HDMI_STATE_STABLE;

					HDMI_TASK("[%d] %s : HDMI State = [%d]  _gH14TimingReadCnt = %d \n", __L__, __F__, gPeriodicState, _gH14TimingReadCnt);
				}
				break;

			case HDMI_STATE_STABLE:
				{
					spin_lock_irqsave(&_gIntrHdmiH14VideoLock, flags);
					_gH14Intr_vf_stable = HDMI_VIDEO_STABLE_STATE;
					spin_unlock_irqrestore(&_gIntrHdmiH14VideoLock, flags);
				}
				break;

			case HDMI_STATE_IDLE:
			case HDMI_STATE_DISABLE:
			case HDMI_STATE_MAX:
			default :
				break;
		}
		//if (ret) break;

	} while(0);

	_gH14HDMIState = gPeriodicState;

	if (_gH14HDMIState < HDMI_STATE_STABLE)
		HDMI_PRINT("[%d]  %s :  State  = %d  \n\n\n", __L__, __F__, _gH14HDMIState);
}


int HDMI_H14_GetRegister(UINT32 addr , UINT32 *value)
{
	unsigned long ulAddr;
	UINT8 slave =0;
	UINT8 reg =0;
	UINT8 data =0;

	if (addr < 0x320)
	{
		if (pstLinkReg == NULL)	return RET_ERROR;

		ulAddr = (unsigned long) pstLinkReg;
		*value = *((volatile UINT32*)(ulAddr + addr));
		*value = *((volatile UINT32*)(ulAddr + addr));
		//HDMI_DEBUG("[HDMI]  %s : %d  Addr  = 0x%x , Value = 0x%x  \n", __F__, __L__, addr, *value);
	}
	else if (addr >= 0x3800 && addr <=0x38CD)
	{
		slave = ((addr &0xff00)>>8)<<1;
		reg = (addr &0xff);

		REG_ReadI2C(PHY_REG_H14_I2C_IDX, slave, reg, &data);
		*value = (UINT32)data;
		//HDMI_DEBUG("[HDMI]  %s : %d  slave = 0x%x reg = 0x%x, Value = 0x%x \n", __F__, __L__, slave, reg,data);
	}

	return RET_OK;
}

int HDMI_H14_SetRegister(UINT32 addr , UINT32 value)
{
	unsigned long ulAddr;
	UINT8 slave =0;
	UINT8 reg =0;
	UINT8 data =0;

	if (addr < 0x320)
	{
		if (pstLinkReg == NULL)	return RET_ERROR;

		ulAddr = (unsigned long) pstLinkReg;
		*((volatile UINT32*)(ulAddr + addr)) = value ;
		//HDMI_DEBUG("[HDMI]  %s : %d  Addr  = 0x%x , Value = 0x%x  \n", __F__, __L__, addr, value);
	}
	else if (addr >= 0x3800 && addr <=0x38CD)
	{
		slave = ((addr &0xff00)>>8)<<1;
		reg = (addr &0xff);
		data = (UINT8)(value &0xff);

		REG_WriteI2C(PHY_REG_H14_I2C_IDX, slave, reg, data);
		//HDMI_DEBUG("[HDMI]  %s : %d  slave = 0x%x reg = 0x%x, data = 0x%x \n", __F__, __L__, slave, reg, data);
	}

	return RET_OK;
}


#ifdef	KDRV_CONFIG_PM
int HDMI_H14_RunSuspend(void)
{
	int ret = RET_OK;
	return ret;
}

int HDMI_H14_RunResume(void)
{
	int ret = RET_OK;

	_HDMI_H14_ClearSWResetAll();			//H14D Chip - Ctop control

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_enable, 0x1);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
	LINK_H14_Wr01(phy_00, phy_rstn, 0x0);			///< Phy Enable
	LINK_H14_Wr01(phy_00, phy_arc_pdb, 0x0);			///< Phy Enable
	LINK_H14_WrFL(phy_00);

	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x0);
	LINK_H14_WrFL(phy_00);

	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_pdb, 0x1);
	LINK_H14_WrFL(phy_00);
	/* PHY_PDB=1, PHY_RSTN=1 을 동시에 인가 register들이 제대로 pre-set되지 않는 현상 발생함. */
	/* PHY_PDB=1  -> 10us  -> PHY_RSTN=1  */
	OS_MsecSleep(2);	// ms delay

	LINK_H14_RdFL(phy_00);
	LINK_H14_Wr01(phy_00, phy_rstn, 0x1);			///< Phy Enable
	LINK_H14_WrFL(phy_00);

	_HDMI_H14_PhyRunReset();	// init Phy register
	_HDMI_H14_RunReset();		// init Link register

	//audio setting
	_HDMI_H14_SetAudio();

	_HDMI_H14_EnableInterrupt();

	HDMI_H14_SetPowerControl(0);
	return ret;
}
#endif


