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
#include <linux/semaphore.h>

#include "os_util.h"
#include "hdmi_hw_h15a0.h"
#include "hdmi_reg_h15.h"
#include "sys_regs.h"

#include "gpio_core.h"

/******************************************************************************
 *				DEFINES
 *****************************************************************************/
#define u16CHK_RANGE(X,Y,offset)		((UINT16) ((X) + (offset) - (Y)) <= (offset*2))
#define TBL_NUM(X)						(sizeof(X) /sizeof((X)[0]))

#define CHK_UHD_BD()	(lx_chip_plt() != LX_CHIP_PLT_FHD)
#define CHK_FHD_BD()	(lx_chip_plt() == LX_CHIP_PLT_FHD)

#define PRT_NUM_INVALID(X)	((X!=0)&&(X!=1))
//#define PRT_NUM_CONVERT(X)	(X = X & 0x01)
#define PRT_NUM_CONVERT(X)	X=((X==0)?2:(X==1)?3:(X==2)?0:(X==3)?1:4)

//#define H15A0_MHL_CONNECTION_BUG_WORKAROUND
#define H15_DISABLE_AUDIO_INTERRUPT

#define H15_THREAD_READ_PHY_REG_VALUE

#define FOR_EDISON_XBOX360

#define H15_CODE_FOR_MHL_CTS
#define H15_CBUS_PDB_CTRL
//#define H15_HUMAX_SETTOP_HDCP_WORKAROUND
//#define H15_HDMI_SEMI_AUTO_EQ_CONTROL
#define ODT_PDB_OFF_ON_WORKAROUND	//for Dvico player re-read EDID data ( Some Dvico player(TVIX M-6510A) reads EDID only when TMDS termination is on)
//#define HPD_OFF_WORKAROUND_FOR_EXT_EDID	// long HPD off time for LTE2 and MHL to HDMI gender
//#define HPD_ON_OFF_CONTROL_WORKAROUND	// for Indian settop box dishtv ( no AVI packet )
#define HDMI_MODE_WHEN_NO_AVI_WORKAROUND	// for Indian settop box dishtv ( no AVI packet )

#define LINK_NUM_0	0
#define	LINK_NUM_1	1

#define USE_EXT_MHL_IC
#define EXT_MHL_IC_LINK_NUM (LINK_NUM_1)

#define H15B1_4K_420_SETTING
/**
 *	Global variables of the driver
 */
extern OS_SEM_T	g_HDMI_Sema;
extern LX_HDMI_REG_T *pH15A0HdmiRegCfg;
extern HDMI_DATA_T *gH15BootData;

extern HDMI_PHY_CONTROL_T	gHDMI_Phy_Control;
//static HDMI_PORT_CONNECTION_TYPE_T _gH15A0HDMIConnectState = HDMI_PORT_NOT_CONNECTED;
//static LX_HDMI_TIMING_INFO_T _gH15A0PrevTiming = {0,};
//static UINT32 _gH15A0TimingReadCnt = 0;
//static UINT32 _gH15A0HDMIState = HDMI_STATE_DISABLE;

//static LX_HDMI_AVI_COLORSPACE_T	_gH15A0PrevPixelEncoding = LX_HDMI_AVI_COLORSPACE_RGB;
#ifdef H15_BRINGUP_TODO
static LX_HDMI_INFO_PACKET_T _gH15A0PrevAVIPacket = {0, };
static LX_HDMI_INFO_PACKET_T _gH15A0PrevVSIPacket = {0, };
#endif
//static BOOLEAN _gH15A0AVIReadState 	  = FALSE;
//static BOOLEAN _gH15A0VSIState 	  = FALSE;
//static BOOLEAN _gH15A0AVIChangeState 	  = FALSE;
//static BOOLEAN _gH15A0PortSelected 	  = FALSE;
static UINT32 _gH15A0MHLContentOff = 0;	// MHL RAP support(if content off is '1' set timing info to '0' to enable AV mute.

//audio
static BOOLEAN _gH15A0AudioDebugState[HDMI_LINK_MAX] 	  = {FALSE, FALSE};		//This variable is used Audio Debug Status.
static BOOLEAN _gH15A0AudioMuteState[HDMI_LINK_MAX] 	  = {FALSE, FALSE};		//This variable is used Audio Mute Status.
static BOOLEAN _gH15A0AudioArcStatus[HDMI_LINK_MAX] 	  = {FALSE, FALSE};		//This variable is used ARC Enabled Status to recover ARC mode after PHY reset.
static UINT32  _gH15A0IntrAudioState[HDMI_LINK_MAX] 	  = {HDMI_AUDIO_INIT_STATE, HDMI_AUDIO_INIT_STATE};
static UINT32  _gH15A0HdmiPortStableCount[HDMI_LINK_MAX]  = {0, 0};		//This count is used to detect if HDMI Switch port is changed.
static UINT32  _gH15A0HdmiFreqErrorCount[HDMI_LINK_MAX]   = {0, 0};		//This count is used to detect if HDMI TMDS Frequency and Frequency is different.
static UINT32  _gH15A0HdmiAudioThreadCount[HDMI_LINK_MAX] = {0, 0};		//_gH15A0HdmiAudioThreadCount is increased in _HDMI_H15A0_Periodic_Audio_Task function
static UINT64  _gH15A0IntrBurstInfoCount[HDMI_LINK_MAX]   = {0, 0};		//This count is used to detect if burst info. interrupt is asserted count.
static UINT64  _gH15A0TaskBurstInfoCount[HDMI_LINK_MAX]   = {0, 0};		//This count is used to save _gH15A0IntrBurstInfoCount in hdmi task.
static UINT32  _gH15A0CtsInterruptCount[HDMI_LINK_MAX]    = {0, 0};		//This count is used to debug if CTS interrupt is raised.
static UINT64  _gH15A0BurstInfoPrevJiffies[HDMI_LINK_MAX] = {0, 0};		//This Jiffies is used to save previous burst info. interrupt jiffies.
static LX_HDMI_AUDIO_INFO_T _gH15A0HdmiAudioInfo[HDMI_LINK_MAX] = {	\
							{LX_HDMI_LINK0, LX_HDMI_AUDIO_DEFAULT, LX_HDMI_SAMPLING_FREQ_NONE},	\
							{LX_HDMI_LINK0, LX_HDMI_AUDIO_DEFAULT, LX_HDMI_SAMPLING_FREQ_NONE} };

//interrupt variables
//static UINT32 _gH15A0Intr_vf_stable = HDMI_VIDEO_INIT_STATE;
//static UINT32 _gH15A0Intr_avi = HDMI_AVI_INIT_STATE;
//static UINT32 _gH15A0Intr_src_mute = HDMI_SOURCE_MUTE_CLEAR_STATE;
//static UINT32 _gH15A0Intr_vsi = HDMI_VSI_INIT_STATE;

static UINT32 _gH15A0Force_thread_sleep  = 0;	//enable  = 2, disable  =0
static UINT32 _gH15A0HDMI_thread_running = 0;	//enable  = 1, disable  =0
static UINT32 _gH15A0HDMI_interrupt_registered = 0;

static UINT32 _gH15A0ChipPlatform = 0;

//static int _gPathen_set_count = 0;
static int _gH15A0CHG_AVI_count_MHL = -1;
static int _gH15A0CHG_AVI_count_EQ = -1;
static int _gH15A0_TMDS_ERROR_EQ[2] = {-1, -1};
static int _gH15A0_TMDS_ERROR_EQ_2nd[2] = {-1, -1};
static int _gH15A0_TMDS_ERROR_intr_count[2] = {0,};
static int _gH15A0_TMDS_ERROR_count[2] = {0,};
static int _gH15A0_TMDS_ERROR_count_tcs_fail[2] = {0,};
static int _gH15A0_tcs_fail_restart_count[2] = {0,};

static int _gH15A0_TERC4_ERROR_intr_count[2] = {0,};
static int _gH15A0_HDCP_ERROR_intr_count[2] = {0,};
static int _gH15A0_HDCP_ERROR_count[2] = {0,};

static int _HDMI_H15A0_ResetPortControl(int reset_enable);
static int _HDMI_H15A0_TMDS_ResetPort(int port_num);
static int _HDMI_H15A0_HDCP_ResetPort(int port_num);
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_TMDS_HDCP_ResetPort(int port_num);
static int _HDMI_H15A0_TMDS_HDCP_ResetPort_Control(int port_num, int reset);
#endif	//#ifdef NOT_USED_NOW
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_TMDS_ResetPort_Control(int port_num, int reset);
#endif
static int _HDMI_H15A0_HDCP_ResetPort_Control(int port_num, int reset);
static int _HDMI_H15A0_EnablePort(int prt_num, int enable);
static int _HDMI_H15A0_Get_ManMHLMode(void);
static int _HDMI_H15A0_Set_ManMHLMode(int link_num, int man_mhl_mode, int man_mhl_value);
static int _HDMI_H15A0_Disable_TMDS_Error_Interrupt(int link_num);
static int _HDMI_H15A0_Enable_TMDS_Error_Interrupt(int link_num);
static int _HDMI_H15A0_Disable_TERC4_Error_Interrupt(int link_num);
static int _HDMI_H15A0_Enable_TERC4_Error_Interrupt(int link_num);
static int _HDMI_H15A0_Disable_HDCP_Error_Interrupt(int link_num);
static int _HDMI_H15A0_Enable_HDCP_Error_Interrupt(int link_num);
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Set_HDCP_Unauth(int port_num, int unauth_nosync, int unauth_mode_chg);
#endif	//#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Check_RAP_Content(int clear);
static int _HDMI_H15A0_MHL_Check_Status(int link_num, int init);
static int _HDMI_H15A0_HDCP_Check_Status(void);

#ifdef ODT_PDB_OFF_ON_WORKAROUND
static int _HDMI_H15A0_Phy_Enable_Register_Access(int port_num);
static int _HDMI_H15A0_EQ_PDB_control(int reset);
#endif
static void _HDMI_H15A0_ModifyUDFormat(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock);
static void _HDMI_H15A0_CheckUDExtInfo(int link_num, LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock);
static int _HDMI_H15A0_EDID_Control(LX_HDMI_CONTROL_EDID_TYPE_T edid_control_cmd);
static UINT32 _HDMI_H15_Get_Clock_Freq(void);
static int _HDMI_H15A0_Check_HDCP_Abnormal(int link_num);
static int _HDMI_H15A0_Check_HDCP_Mode(int link_num);

//static	UINT8 HDCP_AN_Data_Zero[8] ={0,} ;
static	UINT8 HDCP_AKSV_Data_Zero[5] ={0,} ;
static LX_HDMI_PHY_INFORM_T _gH15A0HDMIPhyInform = {0,};

static unsigned int _Port_Change_StartTime = 0;
static int _SCDT_Fall_Detected[2] = {0,};
static unsigned int _TCS_Done_Time[2] = {0,};
static LX_HDMI_RCP_RECEIVE_T _gMHL_RCP_RCV_MSG;
static unsigned int _HDMI_Check_State_StartTime = 0;
static unsigned int _HDMI_Check_State_ElaspedTime = 0;

static int _MHL_PDB_Thread_wakeup = 0;
static int _MHL_Reconnect_Thread_wakeup = 0;
static int _HDMI_Resume_Thread_wakeup = 0;
static int _TCS_Restart_Thread_wakeup = 0;

static int _HDMI_H15A0_TCS_addtional_value_init = 0xa;		//default : 0x8
static int _HDMI_H15A0_TCS_addtional_value_2nd = 0x5;			//default : 0x8
static int _HDMI_H15A0_TCS_addtional_value_3rd = 0x0f;		//default : 0x8

static int _HDMI_H15A0_TCS_mode_sel = 0x3;		// 0: TCS out, 1:CK_MODE, 2:CS manual value, 3 : TCS out channel selection

static int _HDMI_H15A0_Phy_PDB_WA = 1;		//enable phy_pdb_man_mode to '1' when HDMI init.(workaround No AV when booting with cable connected)

static int _HDMI_H15A0_TCS_result[2][3] = {{0,},};
static int _HDMI_H15A0_TCS_add[2] = {0,};
static int _g_TCS_EN_restart[2] = {0,};
static int _g_auth_init_intr[2] = {0,};
static int _g_ecc_err_authed[2] = {0,};

//wait_queue_head_t	WaitQueue_HDMI;
static DECLARE_WAIT_QUEUE_HEAD(WaitQueue_HDMI_H15A0);
static DECLARE_WAIT_QUEUE_HEAD(WaitQueue_MHL_H15A0);
static DECLARE_WAIT_QUEUE_HEAD(WaitQueue_MHL_PDB_H15A0);
static DECLARE_WAIT_QUEUE_HEAD(WaitQueue_TCS_Restart_H15A0);
static DECLARE_WAIT_QUEUE_HEAD(WaitQueue_HDMI_Resume_H15A0);

static DEFINE_SPINLOCK(_gIntrHdmiH15A0VideoLock);
#ifdef NOT_USED_NOW
static DEFINE_SPINLOCK(_gHdmiH15A0RegReadLock);
#endif	//#ifdef NOT_USED_NOW

//HDMI Audio Variables
static DEFINE_SPINLOCK(_gIntrHdmiH15A0AudioLock);

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

#ifndef USE_EXT_MHL_IC
static int	_gH15A0_HDMI_MHLAutoModeCnt = 0;
#endif
static int _gHdmi_no_connection_count = 0;
static unsigned int _g_ODT_PDB_Off_Time;
static unsigned int _g_ODT_PDB_On_Time;

static int _g_rcp_send_buffer = 2;

static int _g_abnormal_3d_format[2] = {0,};

static int _g_tcs_min_max_zero_count[2] = {0,};

static UINT32	_Prev_TMDS_Clock[2] = {0,};
static UINT32	_Curr_TMDS_Clock[2] = {0,};
static UINT32	_Stable_TMDS_Count[4] = {0,};
static UINT32	_EQ_cs_man_value[4] = {0,};

static unsigned char hdmi_data_save[288];
static unsigned char edid_data_rd_port0[LX_EDID_DATA_SIZE];
static unsigned char edid_data_rd_port1[LX_EDID_DATA_SIZE];
static UINT32	_g_Disable_EQ_Offset_Cal[4] = {0,};
static int	_g_HDMI_Device_Init_Completed =  0;

static LX_GPIO_VALUE_T	GPIO15_value = LX_GPIO_VALUE_INVALID;
static LX_GPIO_VALUE_T	GPIO6_value = LX_GPIO_VALUE_INVALID;

static int _g_hdcp_mode_monitor[2] = {0,};
static int _g_prev_hdcp_mode[2] = {0,};
/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */
static struct task_struct	*stHDMI_Thread;
static struct task_struct	*stMHL_Reconnect_Thread;
static struct task_struct	*stMHL_PDB_Thread;
static struct task_struct	*stTCS_Restart_Thread;
static struct task_struct	*stHDMI_Resume_Thread;

PHY_REG_CTRL_H15A0_T  gPHY_REG_CTRL_H15A0;
PHY_REG_CTRL_H15B0_T  gPHY_REG_CTRL_H15B0;
static volatile HDMI_LINK0_REG_H15A0_T *pst_LINK0_Reg;
static volatile HDMI_LINK0_REG_H15A0_T *pst_LINK0_ShadowReg;
static volatile HDMI_LINK1_REG_H15A0_T *pst_LINK1_Reg;
static volatile HDMI_LINK1_REG_H15A0_T *pst_LINK1_ShadowReg;
static volatile HDMI_LINK_TOP_REG_H15A0_T *pst_LINK_TOP_Reg;
static volatile HDMI_LINK_TOP_REG_H15A0_T *pst_LINK_TOP_ShadowReg;

static volatile HDCP22_PRT0_REG_H15B0_T *pst_HDCP22_Prt0_Reg;
static volatile HDCP22_PRT0_REG_H15B0_T *pst_HDCP22_Prt0_ShadowReg;
static volatile HDCP22_PRT1_REG_H15B0_T *pst_HDCP22_Prt1_Reg;
static volatile HDCP22_PRT1_REG_H15B0_T *pst_HDCP22_Prt1_ShadowReg;

static volatile unsigned char *pHDCP_Key_addr;
static volatile unsigned char *pEDID_data_addr_port0;
static volatile unsigned char *pEDID_data_addr_port1;

static LX_HDMI_DRIVER_STATUS_T sHDMI_H15A0_Status;
//static LX_HDMI_DRIVER_STATUS_T *spHDMI_H15A0_Status;


extern	int		SYS_IO_WriteRegArray(UINT32 addr, UINT32 size, const void *data);
/******************************************************************************
 *				Local function
 *****************************************************************************/
static int _HDMI_H15A0_GetExtendTimingInfo(int link_num, LX_HDMI_TIMING_INFO_T *info);
static int _HDMI_H15A0_ClearSWResetAll(void);			//H15A0D Chip - Ctop control
static int _HDMI_H15A0_PhyRunReset(void);
static int _HDMI_H15B0_PhyRunReset(void);
static int _HDMI_H15A0_RunReset(void);
static int _HDMI_H15A0_GetMHLConection(void);
static int _HDMI_H15A0_GetPortConnection(void);
static int _HDMI_H15A0_SetInternalMute(int link_num, LX_HDMI_MUTE_CTRL_T interMute);
static int _HDMI_H15A0_SetVideoBlank(int link_num, LX_HDMI_AVI_COLORSPACE_T space);
static int _HDMI_H15A0_EnableInterrupt(void);
static int _HDMI_H15A0_Thread(void *data);
irqreturn_t _HDMI_H15A0_IRQHandler(int irq, void *dev);
static void _HDMI_H15A0_Periodic_Task(int link_num);

//audio related.
static int _HDMI_H15A0_SetAudio(int link_num);
static int _HDMI_H15A0_GetAudioInfo(int link_num);
static int _HDMI_H15A0_GetAudioTypeAndFreq(int link_num, LX_HDMI_AUDIO_TYPE_T *audioType, LX_HDMI_SAMPLING_FREQ_T *samplingFreq);
static int _HDMI_H15A0_GetAudioFreqFromTmdsClock(int link_num, LX_HDMI_SAMPLING_FREQ_T *samplingFreqFromTmds);
static int _HDMI_H15A0_DebugAudioInfo(int link_num, LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo);

static int _HDMI_H15A0_PhyOn(int port_num);
static int _HDMI_H15A0_PhyOn_5V(int port_num);
static int _HDMI_H15A0_PhyOff(int port_num);
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_ResetPort(int port_num);
#endif	//#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Get_EDID_Rd(int port_num , int *edid_rd_done, int *edid_rd_cnt);
static int _HDMI_H15A0_Get_HDCP_info(int port_num , UINT32 *hdcp_authed, UINT32 *hdcp_enc_en);
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Write_EDID_Data_A4P(UINT32 *pedid_data);
#endif	//#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Get_HDMI5V_Info_A4P(int *pHDMI5V_Status_PRT0, int *pHDMI5V_Status_PRT1 );
static int _HDMI_H15A0_Get_HDMI5V_Info(int port_num, int *pHDMI5V_Status);
static int _HDMI_H15A0_Set_HPD_Out(int prt_sel, int hpd_out_value);
static int _HDMI_H15A0_Set_GPIO_HPD_Out(int prt_sel, int hpd_out_value);
static int _HDMI_H15A0_Set_HPD_Out_A4P(int HPD_Out_PRT0, int HPD_Out_PRT1);
static int _HDMI_H15A0_Get_HPD_Out_A4P(int *pHPD_Out_PRT0, int *pHPD_Out_PRT1);

//static int _HDMI_H15A0_Get_AN_Data(int port_num, UINT8 *AN_Data);
static int _HDMI_H15A0_MHL_Reconnect_Thread(void *data);
static int _HDMI_H15A0_MHL_PDB_Thread(void *data);
static int _HDMI_H15A0_Phy_Reset(int port_num);
static int _HDMI_H15A0_Check_Aksv_Exist(int port_num);
#ifdef NOT_USED_NOW
static int _HDMI_H15A0_swrst_TMDS_sel_control(int reset);
#endif
static void _HDMI_H15A0_IRQHandler_Link(int link_num);
static int _HDMI_H15A0_Delayed_Device_Init(void);
static int _HDMI_H15A0_HDMI_Resume_Thread(void *data);
static int _HDMI_H15A0_TCS_Restart_Thread(void *data);

int HDMI_H15A0_HWInitial(void)
{
	static int hw_initialized = 0;

	if(hw_initialized > 0)
	{
		HDMI_ERROR("HDMI H/W Already Initialized !!!!!\n");
		return RET_ERROR;
	}

	gPHY_REG_CTRL_H15A0.shdw.addr = (UINT32 *)OS_KMalloc((size_t)sizeof(PHY_REG_H15A0_T));
	gPHY_REG_CTRL_H15B0.shdw.addr = (UINT32 *)OS_KMalloc((size_t)sizeof(PHY_REG_H15B0_T));

	pst_LINK0_Reg =(HDMI_LINK0_REG_H15A0_T *)ioremap(pH15A0HdmiRegCfg->link_reg_base_addr, pH15A0HdmiRegCfg->link_reg_size);
	pst_LINK1_Reg =(HDMI_LINK1_REG_H15A0_T *)ioremap(pH15A0HdmiRegCfg->link1_reg_base_addr, pH15A0HdmiRegCfg->link1_reg_size);
	pst_LINK_TOP_Reg =(HDMI_LINK_TOP_REG_H15A0_T *)ioremap(pH15A0HdmiRegCfg->link_top_reg_base_addr, pH15A0HdmiRegCfg->link_top_reg_size);
	pst_HDCP22_Prt0_Reg =(HDCP22_PRT0_REG_H15B0_T *)ioremap(pH15A0HdmiRegCfg->hdcp22_prt0_reg_base_addr, pH15A0HdmiRegCfg->hdcp22_prt0_reg_size);
	pst_HDCP22_Prt1_Reg =(HDCP22_PRT1_REG_H15B0_T *)ioremap(pH15A0HdmiRegCfg->hdcp22_prt1_reg_base_addr, pH15A0HdmiRegCfg->hdcp22_prt1_reg_size);

	HDMI_DEBUG("HDMI LINK REG CFG \n");
	HDMI_DEBUG("LINK 0 REG Base Address	0x%08x\n", pH15A0HdmiRegCfg->link_reg_base_addr);
	HDMI_DEBUG("LINK 0 REG Size 			0x%08x\n", pH15A0HdmiRegCfg->link_reg_size);
	HDMI_DEBUG("LINK 1 REG Base Address	0x%08x\n", pH15A0HdmiRegCfg->link1_reg_base_addr);
	HDMI_DEBUG("LINK 1 REG Size 			0x%08x\n", pH15A0HdmiRegCfg->link1_reg_size);
	HDMI_DEBUG("LINK Top REG Base Address	0x%08x\n", pH15A0HdmiRegCfg->link_top_reg_base_addr);
	HDMI_DEBUG("LINK Top REG Size 			0x%08x\n", pH15A0HdmiRegCfg->link_top_reg_size);
	HDMI_DEBUG("HDCP22 PRT0 REG Base Address	0x%08x\n", pH15A0HdmiRegCfg->hdcp22_prt0_reg_base_addr);
	HDMI_DEBUG("HDCP22 PRT0 REG Size 			0x%08x\n", pH15A0HdmiRegCfg->hdcp22_prt0_reg_size);
	HDMI_DEBUG("HDCP22 PRT1 REG Base Address	0x%08x\n", pH15A0HdmiRegCfg->hdcp22_prt1_reg_base_addr);
	HDMI_DEBUG("HDCP22 PRT1 REG Size 			0x%08x\n", pH15A0HdmiRegCfg->hdcp22_prt1_reg_size);
	//HDMI_PRINT("LINK REG Qumu Address 	0x%08x\n", pH15A0HdmiRegCfg->link_qemu_base_addr);

	if (pst_LINK0_Reg == NULL)
	{
		HDMI_ERROR("ERROR : can't allocate for register\n");
		return RET_ERROR;
	}

	pst_LINK0_ShadowReg = (HDMI_LINK0_REG_H15A0_T *)kmalloc((size_t)sizeof(HDMI_LINK0_REG_H15A0_T), GFP_KERNEL);
	pst_LINK1_ShadowReg = (HDMI_LINK1_REG_H15A0_T *)kmalloc((size_t)sizeof(HDMI_LINK1_REG_H15A0_T), GFP_KERNEL);
	pst_LINK_TOP_ShadowReg = (HDMI_LINK_TOP_REG_H15A0_T *)kmalloc((size_t)sizeof(HDMI_LINK_TOP_REG_H15A0_T), GFP_KERNEL);

	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
	pst_HDCP22_Prt0_ShadowReg = (HDCP22_PRT0_REG_H15B0_T *)kmalloc((size_t)sizeof(HDCP22_PRT0_REG_H15B0_T), GFP_KERNEL);
	pst_HDCP22_Prt1_ShadowReg = (HDCP22_PRT1_REG_H15B0_T *)kmalloc((size_t)sizeof(HDCP22_PRT1_REG_H15B0_T), GFP_KERNEL);
	}

	if ( (pst_LINK0_ShadowReg == NULL) || (pst_LINK1_ShadowReg == NULL) || (pst_LINK_TOP_ShadowReg == NULL) )
	{
		HDMI_ERROR("ERROR : can't allocate for shadow register\n");
		return RET_ERROR;
	}

	/*
	spHDMI_H15A0_Status = (LX_HDMI_DRIVER_STATUS_T *)kmalloc(sizeof(LX_HDMI_DRIVER_STATUS_T), GFP_KERNEL);
	if ( spHDMI_H15A0_Status == NULL )
	{
		HDMI_ERROR("ERROR : can't allocate for driver status\n");
		return RET_ERROR;
	}

	memset(spHDMI_H15A0_Status , 0 , sizeof(LX_HDMI_DRIVER_STATUS_T));
	*/
	//spHDMI_H15A0_Status = (LX_HDMI_DRIVER_STATUS_T *)&sHDMI_H15A0_Status;

	sHDMI_H15A0_Status.Driver_Revision = 0x14122300;
	HDMI_NOTI("HDMI H15 Driver Revision [0x%x]\n", sHDMI_H15A0_Status.Driver_Revision);
	sHDMI_H15A0_Status.device_suspend_count = 0;
	sHDMI_H15A0_Status.device_resumed_count = 0;
	sHDMI_H15A0_Status.device_in_suspend_mode = 0;

	_gH15A0HDMIPhyInform.phy_status = HDMI_PHY_STATUS_DISCONNECTED;

	/* 140710 : H15A1 New board external EDID */
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
	{
		GPIO_DevExGetValue(15, &GPIO15_value);
		HDMI_NOTI("--- HDMI HW OPT. BIT10 = [%d] --- \n", GPIO15_value);
		GPIO_DevExGetValue(6, &GPIO6_value);
		HDMI_NOTI("--- HDMI HW OPT. BIT3 = [%d] --- \n", GPIO6_value);

		if( (GPIO15_value == LX_GPIO_VALUE_LOW) && (GPIO6_value == LX_GPIO_VALUE_LOW) )
			_gH15A0HDMIPhyInform.hpd_pol[0] = 0;
		else if (GPIO15_value == LX_GPIO_VALUE_HIGH)
			_gH15A0HDMIPhyInform.hpd_pol[0] = 0;
		else
			_gH15A0HDMIPhyInform.hpd_pol[0] = 1;	//B0 board use inverted HPD !!!

		HDMI_NOTI("--- HDMI PORT4(1.4) Pol = [%d] --- \n", _gH15A0HDMIPhyInform.hpd_pol[0]);
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H15, A1))
	{
		/*
		_gH15A0HDMIPhyInform.hpd_pol[0] = 0;	//A1 board use external EDID EEPROM
		_HDMI_H15A0_Set_HPD_Out(0, 1);	// disable HPD
		*/
		_gH15A0HDMIPhyInform.hpd_pol[0] = 1;	//A1 board use internal EDID : 140717
	}
	else
		_gH15A0HDMIPhyInform.hpd_pol[0] = 1;

	_gH15A0HDMIPhyInform.hpd_pol[1] = 1;

	 pHDCP_Key_addr = (volatile unsigned char *)&(pst_LINK_TOP_Reg->hdcp_key_00);
	 pEDID_data_addr_port0 = (volatile unsigned char *)&(pst_LINK0_Reg->edid_00);
	 pEDID_data_addr_port1 = (volatile unsigned char *)&(pst_LINK1_Reg->edid_00);

	_gH15A0ChipPlatform = lx_chip_plt();

	if (pHDCP_Key_addr == NULL)
	{
		HDMI_ERROR( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
	}
	if (pEDID_data_addr_port0 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port0 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
	}
	if (pEDID_data_addr_port1 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port1 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
	}

	HDMI_H15A0_Key_Control(HDMI_KEY_SAVE);
	HDMI_H15A0_Key_Control(HDMI_KEY_CLEAR);

	stHDMI_Thread = kthread_create(_HDMI_H15A0_Thread, (void*)NULL, "hdmi_thread");
	if (stHDMI_Thread)
	{
		wake_up_process(stHDMI_Thread);
		_gH15A0HDMI_thread_running = 1;
		HDMI_DEBUG("HDMI Thread [%d]\n", stHDMI_Thread->pid);
	}
	else
	{
		HDMI_ERROR("HDMI Thread Already Created\n");
	}

	stMHL_Reconnect_Thread = kthread_create(_HDMI_H15A0_MHL_Reconnect_Thread, (void*)NULL, "mhl_reconnect_thread");
	if (stMHL_Reconnect_Thread)
	{
		wake_up_process(stMHL_Reconnect_Thread);
		HDMI_DEBUG("MHL Reconnect Thread [%d]\n", stMHL_Reconnect_Thread->pid);
	}
	else
	{
		HDMI_ERROR("MHL Reconnect Thread Already Created\n");
	}

	stMHL_PDB_Thread = kthread_create(_HDMI_H15A0_MHL_PDB_Thread, (void*)NULL, "mhl_pdb_thread");
	if (stMHL_PDB_Thread)
	{
		wake_up_process(stMHL_PDB_Thread);
		HDMI_DEBUG("MHL PDB Thread [%d]\n", stMHL_PDB_Thread->pid);
	}
	else
	{
		HDMI_ERROR("MHL Reconnect Thread Already Created\n");
	}

	stTCS_Restart_Thread = kthread_create(_HDMI_H15A0_TCS_Restart_Thread, (void*)NULL, "tcs_restart_thread");
	if (stMHL_PDB_Thread)
	{
		wake_up_process(stTCS_Restart_Thread);
		HDMI_DEBUG("TCS Restart Thread [%d]\n", stTCS_Restart_Thread->pid);
	}
	else
	{
		HDMI_ERROR("TCS Restart Thread Already Created\n");
	}

	stHDMI_Resume_Thread = kthread_create(_HDMI_H15A0_HDMI_Resume_Thread, (void*)NULL, "hdmi_resume_thread");
	if (stHDMI_Resume_Thread)
	{
		wake_up_process(stHDMI_Resume_Thread);
		HDMI_DEBUG("HDMI Resume Thread [%d]\n", stHDMI_Resume_Thread->pid);
	}
	else
	{
		HDMI_ERROR("HDMI Resume Thread Already Created\n");
	}

//	HDMI_H15A0_SetPowerControl(0);

	HDMI_TRACE("H15A0 HDMI Initialize \n" );

	hw_initialized++;

	return RET_OK;
}

int HDMI_H15A0_SetPort(UINT32 *port)
{
	int ret = RET_OK;
	int port_selected = 2;
//	int loop, port_authed, port_enc_en;
	int hdmi5v_status = 0;
//	int prev_port_an_data_zero = -1;
	ULONG	flags = 0;

	/*
	if ( spHDMI_H15A0_Status == NULL )
	{
		HDMI_ERROR("ERROR : driver status\n");
	}
	*/
	/* H15 NO Port SEL */
	/*
	LINK_NUM_H15_RdFL(0, system_control_00);
	LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, port_selected);
	*/

	//spin lock for protection : lock
	spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);

	//Check a HDMI Link Number
	if (*port < HDMI_LINK_MAX)
	{
		//Clears a global value for audio info.
		_gH15A0HdmiAudioInfo[*port].audioType	 = LX_HDMI_AUDIO_NO_AUDIO;
		_gH15A0HdmiAudioInfo[*port].samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;

		//Clear _gH15A0IntrAudioState[link_num]
		_gH15A0IntrAudioState[*port] = HDMI_AUDIO_INIT_STATE;

		//Reset _gH15A0IntrBurstInfoCount[link_num]
		_gH15A0IntrBurstInfoCount[*port] = 0;

		//Clear _gH15A0HdmiFreqErrorCount[link_num]
		_gH15A0HdmiFreqErrorCount[*port] = 0;
	}

	//H15 Port Mapping
	HDMI_DEBUG("Port Change to [%d] !!!\n", *port);
	*port = *port & 0x1;
	HDMI_DEBUG("Port Mapping HDMI 1.4 port [%d] !!!\n", *port);

	sHDMI_H15A0_Status.HDMIState[*port] = HDMI_STATE_IDLE;

#ifdef H15_BRINGUP_TODO
	if( port_selected != *port)
	{
		_HDMI_H15A0_swrst_TMDS_sel_control(1);
		sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INIT_STATE;
		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INIT_STATE;
		sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INIT_STATE;
		sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_CLEAR_STATE;
	}
	sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
	sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
	sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
	sHDMI_H15A0_Status.AVIChangeState[link_num] = FALSE;
#endif

	//spin lock for protection : unlock
	spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

	OS_LockMutex(&g_HDMI_Sema);

	//added 131127 : force timing info to zero when port change


	do {
		if (*port > HDMI_PHY1_PORT)	{ret = RET_ERROR;	break;}

		_Port_Change_StartTime = jiffies_to_msecs(jiffies);
		//_gH15A0CHG_AVI_count_EQ = 0;
#ifdef H15_BRINGUP_TODO
		_gH15A0_TMDS_ERROR_EQ[*port] = 0;
		_gH15A0_TMDS_ERROR_EQ_2nd[*port] = -1;
#endif
//		_HDMI_H15A0_Enable_TMDS_Error_Interrupt();
		_TCS_Done_Time[*port] = jiffies_to_msecs(jiffies);


#ifdef H15_CODE_FOR_MHL_CTS
		_HDMI_H15A0_Check_RAP_Content(1);
#endif

		/* Not Used */
		if (gHDMI_Phy_Control.link_reset_control)
		{
			_HDMI_H15A0_TMDS_ResetPort(port_selected);
		}
		/* Not Used */
		if (!gHDMI_Phy_Control.all_port_pdb_enable)
		{
			if (*port != 0)
				_HDMI_H15A0_PhyOff(0);
			if (*port != 1)
				_HDMI_H15A0_PhyOff(1);
		}

#ifdef H15_CODE_FOR_MHL_CTS
		if (_gH15A0HDMIPhyInform.cd_sense)
		{
			if ( (port_selected == 1) || (*port == 1) )
			{
				LINK_NUM_H15_RdFL(1, cbus_04);
				LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_wrt_stat_pathen_set, 0x1);
				LINK_NUM_H15_WrFL(1, cbus_04);
				HDMI_DEBUG("---- MHL pathen set : 0x1 \n");
				OS_MsecSleep(5);	// ms delay
				LINK_NUM_H15_RdFL(1, cbus_04);
				LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_wrt_stat_pathen_set, 0x0);
				LINK_NUM_H15_WrFL(1, cbus_04);
				HDMI_DEBUG("---- MHL pathen set : 0x0 \n");
			}
		}
#endif

		/*
		LINK_NUM_H15_RdFL(0, system_control_00);
		LINK_NUM_H15_Wr01(0, system_control_00, reg_prt_sel, *port);
		LINK_NUM_H15_WrFL(0, system_control_00);
		*/

		sHDMI_H15A0_Status.PortSelected[*port] = TRUE;
		HDMI_DEBUG("Port Selected [%d] = [%d]!!!\n", *port, sHDMI_H15A0_Status.PortSelected[*port]);
		HDMI_NOTI("sHDMI_H15A0_Status.PortSelected [%d][%d] \n", sHDMI_H15A0_Status.PortSelected[0], sHDMI_H15A0_Status.PortSelected[1]);

#ifdef H15_BRINGUP_TODO
		// Added for CHG VSI VFORMAT interrupt to occure
		LINK_NUM_H15_RdFL(0, packet_00);
		LINK_NUM_H15_Wr01(0, packet_00, reg_pkt_clr, 1);
		LINK_NUM_H15_WrFL(0, packet_00);

		OS_MsecSleep(5);	// ms delay

		LINK_NUM_H15_RdFL(0, packet_00);
		LINK_NUM_H15_Wr01(0, packet_00, reg_pkt_clr, 0);
		LINK_NUM_H15_WrFL(0, packet_00);
#endif

		_HDMI_H15A0_Get_HDMI5V_Info(*port, &hdmi5v_status);

		{
			HDMI_DEBUG("Direct Port Change to [%d] hdmi5v[%d]\n", \
					*port, hdmi5v_status);
			if (!gHDMI_Phy_Control.all_port_pdb_enable)
				_HDMI_H15A0_PhyOn(*port);
			else
			{
				/* All Phy Disabled except port_num */
				if (*port != 0)
					_HDMI_H15A0_EnablePort(0, 0);
				if (*port != 1)
					_HDMI_H15A0_EnablePort(1, 0);

				/* Enable port_num */
				_HDMI_H15A0_EnablePort(*port, 1);
			}
			_gH15A0HDMIPhyInform.prt_sel = *port;
		}

		HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, *port);

	}while (0);

	//Check a HDMI Link Number
	if (*port < HDMI_LINK_MAX)
	{
		//Check a ARC Enabled Status for Audio Return Channel.
		if (_gH15A0AudioArcStatus[*port] == TRUE)
		{
			//ARC source
			LINK_NUM_H15_RdFL(*port, system_control_00);
			LINK_NUM_H15_Wr01(*port, system_control_00, reg_arc_src, 0x0);
			LINK_NUM_H15_WrFL(*port, system_control_00);

			//Enable or Disable ARC port
			LINK_NUM_H15_RdFL(*port, phy_00);
			LINK_NUM_H15_Wr01(*port, phy_00, phy_arc_odt_sel, 0x2); 	///< Port1 PHY ARC Temination
			LINK_NUM_H15_Wr01(*port, phy_00, phy_arc_pdb, (UINT32)_gH15A0AudioArcStatus[*port]);	///< Port1 PHY ARC PDB
			LINK_NUM_H15_WrFL(*port, phy_00);
		}
	}

	OS_UnlockMutex(&g_HDMI_Sema);

#ifdef H15_BRINGUP_TODO
	if( port_selected != *port)
		_HDMI_H15A0_swrst_TMDS_sel_control(0);
#endif

	HDMI_DEBUG("[%d] %s : port = %d, ARC = %s \n", __L__, __F__, *port, _gH15A0AudioArcStatus[*port]? "On":"Off");
	return ret;
}

int HDMI_H15A0_Thread_Control(int sleep_enable)
{
	if (sleep_enable)
	{
		_gH15A0Force_thread_sleep = 2;
		sHDMI_H15A0_Status.HDMIState[0] = HDMI_STATE_DISABLE;
		sHDMI_H15A0_Status.HDMIState[1] = HDMI_STATE_DISABLE;
		HDMI_NOTI("H15A0 sleeping HDMI Thread \n");
	}
	else
	{
		wake_up_interruptible(&WaitQueue_HDMI_H15A0);
		sHDMI_H15A0_Status.HDMIState[0] = HDMI_STATE_IDLE;
		sHDMI_H15A0_Status.HDMIState[1] = HDMI_STATE_IDLE;
		_gH15A0Force_thread_sleep = 0;
		_gH15A0HDMI_thread_running = 1;
		HDMI_NOTI("H15A0 Starting HDMI Thread\n");
	}

	return RET_OK;
}

int HDMI_H15A0_GetMode(int link_num, LX_HDMI_MODE_T *mode)
{
	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

//	mode->bHDMI =  gH15BootData->mode;
	mode->bHDMI =  sHDMI_H15A0_Status.hdmi_mode[link_num];

	HDMI_PRINT("[%d] %s : Mode = [%s] \n", __L__, __F__,  mode->bHDMI? "HDMI":"DVI");
	return RET_OK;
}

int HDMI_H15A0_GetAspectRatio(int link_num, LX_HDMI_ASPECTRATIO_T *ratio)
{
	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	ratio->eAspectRatio = (((sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[0] &0xff0000)>>16) &0x30)>>4;		// M1M0

	HDMI_PRINT("[%d] %s : [0] NoData, [1]4:3, [2]16:9 [3] Future  ==> [%d] \n", __L__, __F__, ratio->eAspectRatio);
	return RET_OK;
}
\
int HDMI_H15A0_GetColorDomain(int link_num, LX_HDMI_COLOR_DOMAIN_T *color)
{
	UINT32 temp = 0;

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)color , 0 , (size_t)sizeof(LX_HDMI_COLOR_DOMAIN_T));

	if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INIT_STATE || sHDMI_H15A0_Status.Intr_avi[link_num] == HDMI_AVI_INIT_STATE)
	{
		return RET_OK;
	}

	if (sHDMI_H15A0_Status.Intr_avi[link_num] > HDMI_AVI_INIT_STATE)
	{
		color->bHdmiMode = sHDMI_H15A0_Status.hdmi_mode[link_num];

		if (color->bHdmiMode)
		{
			if (sHDMI_H15A0_Status.AVIReadState[link_num])
				temp = sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[0];
			else
			{
				LINK_NUM_H15_RdFL(link_num, packet_18);
				temp = LINK_NUM_H15_Rd(link_num, packet_18);
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

int HDMI_H15A0_GetTimingInfo(int link_num, LX_HDMI_TIMING_INFO_T *info)
{
	UINT32 tmp32 = 0;
	UINT32 tmdsClock = 0,	tmpClock = 0;
#ifndef H15_THREAD_READ_PHY_REG_VALUE
	UINT32 up_freq = 0,	down_freq = 0;
#endif
	UINT32 pixelRepet = 0, colorDepth = 0;

	LX_HDMI_TIMING_INFO_T 	bufTiming;

	static int abnormal_FP_count[2] = {0,};

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));

	do{
		if (_gH15A0HDMI_thread_running == 1 && sHDMI_H15A0_Status.HDMIState[link_num] < HDMI_STATE_READ)
			break;

		if (sHDMI_H15A0_Status.HDMIState[link_num] < HDMI_STATE_SCDT)	// 140408 : added for condition when no connection, but prev timing exist
			break;

		if (sHDMI_H15A0_Status.PortSelected[link_num] == FALSE)
			break;

		if (_gH15A0HDMI_thread_running == 1 && sHDMI_H15A0_Status.HDMIState[link_num] == HDMI_STATE_STABLE && sHDMI_H15A0_Status.PrevTiming[link_num].vFreq > 0)
			goto func_exit;

		LINK_NUM_H15_RdFL(link_num, video_04);
		LINK_NUM_H15_Rd01(link_num, video_04, reg_h_tot, bufTiming.hTotal);		///< Horizontal total pixels
		LINK_NUM_H15_Rd01(link_num, video_04, reg_v_tot, bufTiming.vTotal);		///< Vertical total lines

		LINK_NUM_H15_RdFL(link_num, video_05);
		LINK_NUM_H15_Rd01(link_num, video_05, reg_h_av, bufTiming.hActive);		///< Horizontal active pixel
		LINK_NUM_H15_Rd01(link_num, video_05, reg_v_av, bufTiming.vActive);		///< Vertical active lines
#if 0
		LINK_NUM_H15_RdFL(link_num, video_06);
		LINK_NUM_H15_Rd01(link_num, video_06, reg_h_fp, bufTiming.hStart);		///< Horizontal start pixel (Front Porch)
		LINK_NUM_H15_Rd01(link_num, video_06, reg_v_fp, bufTiming.vStart);		///< Vertical start lines (Front Porch)
#endif
		LINK_NUM_H15_RdFL(link_num, video_07);
		LINK_NUM_H15_Rd01(link_num, video_07, reg_h_bp, bufTiming.hStart);	///< Horizontal start pixel (Back Porch)
		LINK_NUM_H15_Rd01(link_num, video_07, reg_v_bp, bufTiming.vStart);	///< Vertical start lines (Back Porch)

		LINK_NUM_H15_RdFL(link_num, video_10);
		LINK_NUM_H15_Rd01(link_num, video_10, reg_intrl, bufTiming.scanType);
		bufTiming.scanType^=1; 							///< Scan type (0 : interlace, 1 : progressive) 	info->scanType ^= 1;

		bufTiming.extInfo = 0; 	///< Full 3D Timing

		///* Start  Horizontal & Vertical frequency *///
#ifdef H15_THREAD_READ_PHY_REG_VALUE
		tmdsClock = _gH15A0HDMIPhyInform.tmds_clock[link_num]; 	// XXX.XX KHz
#else
		tmdsClock = _HDMI_H15_Get_Clock_Freq();
#endif

		_HDMI_H15A0_ModifyUDFormat(&bufTiming, tmdsClock);		// modify 297MHz format

		_HDMI_H15A0_GetExtendTimingInfo(link_num, &bufTiming);		// get extend information

		LINK_NUM_H15_RdFL(link_num, video_02);
		LINK_NUM_H15_Rd01(link_num, video_02, reg_cmode_tx, colorDepth);
		colorDepth = (colorDepth &0x60)>>5;

		if ( (bufTiming.extInfo == LX_HDMI_EXT_4K_2K) && (colorDepth > 0))
		{
			HDMI_VIDEO("[%d] %s : not support 4Kx2K color bit over 8bit[%d]\n", __L__, __F__,colorDepth);
			memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_cnt;
		}
		// InValid Format Check
		if ( (bufTiming.hActive > 4096) || (bufTiming.vActive > 2415) ||
			(bufTiming.hActive < 320) || (bufTiming.vActive < 240) )
		{
			//memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
			HDMI_VIDEO("[%d] %s : InValid Format for Active Size,  hActive[%d] vActive [%d]\n", __L__, __F__, bufTiming.hActive, bufTiming.vActive);
			goto func_exit;
		}

		//* Support 2D 1280x1024i@86  by 20120202
		//* Not support Master #333 - because MASTER timing issue
		if (bufTiming.hActive == 1280 && bufTiming.vActive == 512 && bufTiming.scanType == 1)
		{
			HDMI_VIDEO("[%d] %s : not support 2D 1280x1024!@86 of Master #333\n", __L__, __F__);
			memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_cnt;
		}

	//	_HDMI_H15A0_GetExtendTimingInfo(&bufTiming);		// get extend information

		if (bufTiming.vActive == 0 || bufTiming.hActive == 0 )
		{
			HDMI_PRINT("[%d] %s : _HDMI_H15A0_GetExtendTimingInfo hActive, vActive  = 0\n", __L__, __F__);
			//memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
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
				memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
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

#ifdef H15_BRINGUP_TODO
		if ( tmdsClock > 29600 )
		{
			HDMI_VIDEO("[%d] %s : not support 4Kx2K over 296Mhz [%d0 Khz]\n", __L__, __F__,tmdsClock);
			memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
			goto func_cnt;
		}
#endif

		LINK_NUM_H15_RdFL(link_num, video_00);
		LINK_NUM_H15_Rd01(link_num, video_00, reg_pr_tx, pixelRepet);

		tmpClock = tmdsClock * 1000;

		if (tmdsClock != 0 && bufTiming.vTotal > 0 && bufTiming.hTotal > 0 )		// XXX.XX KHz
		{
			//LINK_NUM_H15_RdFL(0, video_02);
			//LINK_NUM_H15_Rd01(0, video_02, reg_cmode_tx, colorDepth);
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

		if ((sHDMI_H15A0_Status.HDMIState[link_num] > HDMI_STATE_NO_SCDT && sHDMI_H15A0_Status.HDMIState[link_num] <= HDMI_STATE_READ) ||_gH15A0HDMI_thread_running == 0)
		{
			if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] > HDMI_VIDEO_INIT_STATE)
			{
#ifdef H15_BRINGUP_TODO
				CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
				CTOP_CTRL_H15A0_Rd01(CVI, ctr00, hdmi_half_sel, tmp32);

				if (pixelRepet > 0)
				{
					if (tmp32 == 0)
					{
						CTOP_CTRL_H15A0_Wr01(CVI, ctr00, hdmi_half_sel, 0x1);
						CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/%d]  for pixel repet \n", __L__, __F__, pixelRepet);
					}
				}
				else if (bufTiming.extInfo == LX_HDMI_EXT_4K_2K)
				{
					if (tmp32 == 0)
					{
						CTOP_CTRL_H15A0_Wr01(CVI, ctr00, hdmi_half_sel, 0x1);
						CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/2]  for UD\n", __L__, __F__);
					}
				}
				else
				{
					if (tmp32 != 0)
					{
						CTOP_CTRL_H15A0_Wr01(CVI, ctr00, hdmi_half_sel, 0x0);
						CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
						HDMI_DEBUG("[%d] %s : change CTOP Clock Divide[1/1]  for default\n", __L__, __F__);
					}
				}
#endif
			}

			if (pixelRepet > 1)
			{
				memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
				HDMI_ERROR("[%d] %s : not support Pixel Repetation [%d]\n", __L__, __F__, pixelRepet);
				break;
			}
		}

		// H15 Added for 420 format
		if( (  sHDMI_H15A0_Status.PrevPixelEncoding[link_num] == LX_HDMI_AVI_COLORSPACE_YCBCR420) && (bufTiming.hActive >= 1920) && (bufTiming.vActive == 2160) && (bufTiming.scanType == 1)) {

			HDMI_VIDEO("4K 4:2:0 !!! \n");
			bufTiming.hActive = bufTiming.hActive << 1;
		}

func_cnt:
		abnormal_FP_count[link_num] = 0;

		//if ( sHDMI_H15A0_Status.TimingReadCnt[link_num] > 100) 	goto func_exit;

		if ( memcmp(&sHDMI_H15A0_Status.PrevTiming[link_num], &bufTiming, (size_t)sizeof(LX_HDMI_TIMING_INFO_T)) == 0 )
		{
			if (sHDMI_H15A0_Status.TimingReadCnt[link_num] <= 100)
				sHDMI_H15A0_Status.TimingReadCnt[link_num]++;

			if (sHDMI_H15A0_Status.HDMIState[link_num] == HDMI_STATE_READ && sHDMI_H15A0_Status.TimingReadCnt[link_num] < 9)
				HDMI_VIDEO("[%d] %s : hActive[%d] vActive [%d] ReadCnt[%d] \n", __L__, __F__, bufTiming.hActive, bufTiming.vActive,sHDMI_H15A0_Status.TimingReadCnt[link_num]);
		}

		break;

func_exit:

		memcpy(&bufTiming, &sHDMI_H15A0_Status.PrevTiming[link_num], (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
	} while(0);

	info->hFreq 	= sHDMI_H15A0_Status.PrevTiming[link_num].hFreq 	= bufTiming.hFreq; 			///< Horizontal frequency(100 Hz unit) = Dot Freq / hTotal
	info->vFreq 	= sHDMI_H15A0_Status.PrevTiming[link_num].vFreq 	= bufTiming.vFreq; 			///< Veritical frequency(1/10 Hz unit) = hFreq / vTotal
	info->hTotal	= sHDMI_H15A0_Status.PrevTiming[link_num].hTotal 	= bufTiming.hTotal; 		///< Horizontal total pixels
	info->vTotal 	= sHDMI_H15A0_Status.PrevTiming[link_num].vTotal	= bufTiming.vTotal; 		///< Vertical total lines
	info->hStart	= sHDMI_H15A0_Status.PrevTiming[link_num].hStart	= bufTiming.hStart; 		///< Horizontal start pixel (Back Porch)
	info->vStart 	= sHDMI_H15A0_Status.PrevTiming[link_num].vStart	= bufTiming.vStart;			///< Vertical start lines (Back Porch)
	info->hActive 	= sHDMI_H15A0_Status.PrevTiming[link_num].hActive	= bufTiming.hActive;		///< Horizontal active pixel
	info->vActive 	= sHDMI_H15A0_Status.PrevTiming[link_num].vActive	= bufTiming.vActive; 		///< Vertical active lines
	info->scanType 	= sHDMI_H15A0_Status.PrevTiming[link_num].scanType	= bufTiming.scanType; 		///< Scan type (0 : interlace, 1 : progressive) 	info->scanType ^= 1;
	info->extInfo	= sHDMI_H15A0_Status.PrevTiming[link_num].extInfo	= bufTiming.extInfo; 	///< Full 3D Timing
	info->state = sHDMI_H15A0_Status.HDMIState[link_num];

	/* Workaround Code for FP vactive timiming with Non-FP 3D information */
#ifdef H15_BRINGUP_TODO
	if ( sHDMI_H15A0_Status.Intr_vsi[link_num] >= HDMI_VSI_STABLE_STATE)
	{
		int vactive_reg_value;

		LINK_NUM_H15_RdFL(link_num, video_05);
		LINK_NUM_H15_Rd01(link_num, video_05, reg_v_av, vactive_reg_value);		///< Vertical active lines

		if( ( (info->vActive > 1300) && ( info->extInfo != LX_HDMI_EXT_4K_2K) ) ||\
				( ( info->extInfo == LX_HDMI_EXT_3D_FIELD_ALTERNATIVE) && (vactive_reg_value != 1103)  && (vactive_reg_value != 601)
				  && (vactive_reg_value != 503) ) )
		{
			abnormal_FP_count[link_num] ++;

			if(abnormal_FP_count[link_num] == 3)
			{

				HDMI_NOTI("#########################################\n");
				HDMI_NOTI("#########################################\n");
				HDMI_NOTI("## 3D format and resolution not match ###\n");
				HDMI_NOTI("#### Port[%d], 3D[%d], Vactive[%d], Vactive_reg[%d] #\n", \
						_gH15A0HDMIPhyInform.prt_sel, info->extInfo, info->vActive, vactive_reg_value);
				HDMI_NOTI("#########################################\n");
				HDMI_NOTI("#########################################\n");

				_g_abnormal_3d_format[link_num] = 1;
				/*
				_HDMI_H15A0_TMDS_HDCP_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel, 0);

				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				_HDMI_H15A0_TMDS_HDCP_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel, 1);
				*/
			}
		}
		else
			abnormal_FP_count[link_num] = 0;
	}
	else
		abnormal_FP_count[link_num] = 0;

	HDMI_PRINT("[%d] %s abnormal_FP_count[%d] , link_num[%d]\n", __L__, __F__, abnormal_FP_count[link_num], link_num);
#endif

	if (_gH15A0MHLContentOff && (_gH15A0HDMIPhyInform.prt_sel == 3) && (_gH15A0HDMIPhyInform.cd_sense == 1) )
		memset(info , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));

	/*	test for abnormal source
	if (bufTiming.vFreq > 600 && sHDMI_H15A0_Status.HDMIState[link_num] == HDMI_STATE_READ)
	{
		HDMI_VIDEO("[%d] %s : VFreq [%d] ReadCnt[%d] \n", __L__, __F__, bufTiming.vFreq, sHDMI_H15A0_Status.TimingReadCnt[link_num]);
		memset(info , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
	}
	*/

	return RET_OK;
}

int HDMI_H15A0_GetAviPacket(int link_num, LX_HDMI_INFO_PACKET_T *packet)
{
	ULONG flags = 0;
	UINT32 header;
	UINT32 data[8];
	static UINT32 gAVIReadCnt = 0;
	LX_HDMI_AVI_COLORSPACE_T	csc = LX_HDMI_AVI_COLORSPACE_RGB;

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)packet , 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	//packet->InfoFrameType = LX_HDMI_INFO_AVI;

	if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INIT_STATE || sHDMI_H15A0_Status.Intr_avi[link_num] == HDMI_AVI_INIT_STATE)
	{
		gAVIReadCnt = 0;
		memset(&sHDMI_H15A0_Status.PrevAVIPacket[link_num], 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		//_gH15A0PrevAVIPacket.InfoFrameType = LX_HDMI_INFO_AVI;
		//HDMI_VIDEO("[%d] %s : sHDMI_H15A0_Status.Intr_vf_stable[link_num] AVI  / sHDMI_H15A0_Status.Intr_vf_stable[link_num] \n", __L__, __F__ );
#ifdef HDMI_MODE_WHEN_NO_AVI_WORKAROUND
		if ( _gH15A0HDMI_thread_running == 1)
			packet->dataBytes[7] = sHDMI_H15A0_Status.hdmi_mode[link_num];
		else
		{
			/*
			UINT32 prt_selected;

			LINK_NUM_H15_RdFL(0, system_control_00);

			LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, prt_selected);

			if (prt_selected == 0)
				LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt0, data[7]);
			else if (prt_selected == 1)
				LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt1, data[7]);
			else if (prt_selected == 2)
				LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt2, data[7]);
			else
				LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt3, data[7]);
			*/

			LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, data[7]);

			//LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, data[7]);
			packet->dataBytes[7] = sHDMI_H15A0_Status.hdmi_mode[link_num] = data[7];		//HDMI Mode
		}
#endif
		return RET_OK;
	}

#ifdef H15_BRINGUP_TODO
	if ( _gH15A0HDMIPhyInform.tmds_clock[link_num] > 29600 )
	{
		gAVIReadCnt = 0;
		HDMI_VIDEO("[%d] %s : not support 4Kx2K over 296Mhz [%d0 Khz]\n", __L__, __F__,_gH15A0HDMIPhyInform.tmds_clock[0]);
		memset(&sHDMI_H15A0_Status.PrevAVIPacket[link_num], 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		return RET_OK;
	}
#endif

	if ( _gH15A0HDMI_thread_running == 1 && sHDMI_H15A0_Status.Intr_avi[link_num] == HDMI_AVI_STABLE_STATE)
		goto func_exit;

	if (sHDMI_H15A0_Status.Intr_avi[link_num] == HDMI_AVI_CHANGE_STATE)
	{
		//gAVIReadCnt = 0;
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
		sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
	}

	if (gAVIReadCnt > 300)
	{
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_STABLE_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
	}

	LINK_NUM_H15_RdFL(link_num, packet_17);
	header = LINK_NUM_H15_Rd(link_num, packet_17);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
	packet->header = (header &0xffff);

	LINK_NUM_H15_RdFL(link_num, packet_18);
	LINK_NUM_H15_RdFL(link_num, packet_19);
	LINK_NUM_H15_RdFL(link_num, packet_20);
	LINK_NUM_H15_RdFL(link_num, packet_21);
	LINK_NUM_H15_RdFL(link_num, packet_22);
	LINK_NUM_H15_RdFL(link_num, packet_23);
	LINK_NUM_H15_RdFL(link_num, packet_24);

	data[0] = LINK_NUM_H15_Rd(link_num, packet_18);
	data[1] = LINK_NUM_H15_Rd(link_num, packet_19);
	data[2] = LINK_NUM_H15_Rd(link_num, packet_20);
	data[3] = LINK_NUM_H15_Rd(link_num, packet_21);
	data[4] = LINK_NUM_H15_Rd(link_num, packet_22);
	data[5] = LINK_NUM_H15_Rd(link_num, packet_23);
	data[6] = LINK_NUM_H15_Rd(link_num, packet_24);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];

	if ( _gH15A0HDMI_thread_running == 1)
		packet->dataBytes[7] = sHDMI_H15A0_Status.hdmi_mode[link_num];		//HDMI Mode
	else
	{
		/*
		UINT32 prt_selected;

		LINK_NUM_H15_RdFL(0, system_control_00);

		LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, prt_selected);

		if (prt_selected == 0)
			LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt0, data[7]);
		else if (prt_selected == 1)
			LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt1, data[7]);
		else if (prt_selected == 2)
			LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt2, data[7]);
		else
			LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode_prt3, data[7]);
		*/
		LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, data[7]);

		//LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode, data[7]);
		packet->dataBytes[7] = sHDMI_H15A0_Status.hdmi_mode[link_num] = data[7];		//HDMI Mode
	}

	csc = (((data[0] &0xff00)>>8) &0x60)>>5;

	if ((csc == 3) && ((data[0]>>8)  == 0x60))		//data[0] 0xff ==> Cheaksum, data[0] 0x6000 ==> Y!Y0
	{
		csc = 0;
		packet->dataBytes[0] = data[0] = data[0] &0xff;
	}

	if (sHDMI_H15A0_Status.PrevPixelEncoding[link_num] != csc)
	{
		HDMI_DEBUG("CSC change [%d] => [%d] : link[%d]\n", sHDMI_H15A0_Status.PrevPixelEncoding[link_num] , csc, link_num);
		_HDMI_H15A0_SetVideoBlank(link_num, csc);
		sHDMI_H15A0_Status.PrevPixelEncoding[link_num] = csc;

		// H15 Added for 420 format
		if(csc == LX_HDMI_AVI_COLORSPACE_YCBCR420)
		{
#ifdef H15B1_4K_420_SETTING
			HDMI_NOTI("COLORSPACE_YCBCR420 : setting link [%d]\n", csc);
			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				LINK_NUM_H15_RdFL(link_num, video_01);
				LINK_NUM_H15_Wr(link_num, video_01, 0xC000);	// Write Only Register
				LINK_NUM_H15_WrFL(link_num, video_01);

//				HDCP22_NUM_H15_RdFL(link_num, ad_hdcp22_out_data_align);
//				HDCP22_NUM_H15_Wr01(link_num, ad_hdcp22_out_data_align, o_reg_hdcp22_out_data_align_0, 0x1);
//				HDCP22_NUM_H15_WrFL(link_num, ad_hdcp22_out_data_align);
			}
			else {
				PHY_REG_H15A0_RdFL(ch_chg_420_mode);
				PHY_REG_H15A0_Wr01(ch_chg_420_mode,ch_chg_420,0x1);
				PHY_REG_H15A0_WrFL(ch_chg_420_mode);
			}
#else
			HDMI_NOTI("COLORSPACE_YCBCR420 : setting phy [%d]\n", csc);
			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(ch_chg_420);
				PHY_REG_H15B0_Wr01(ch_chg_420,ch_chg_420,0x1);
				PHY_REG_H15B0_WrFL(ch_chg_420);
			}
			else {
				PHY_REG_H15A0_RdFL(ch_chg_420_mode);
				PHY_REG_H15A0_Wr01(ch_chg_420_mode,ch_chg_420,0x1);
				PHY_REG_H15A0_WrFL(ch_chg_420_mode);
			}
#endif
		}
		else
		{
#ifdef H15B1_4K_420_SETTING
			HDMI_NOTI("COLORSPACE Not YCBCR420 : setting link [%d]\n", csc);
			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				LINK_NUM_H15_RdFL(link_num, video_01);
				LINK_NUM_H15_Wr(link_num, video_01, 0x0000);
				LINK_NUM_H15_WrFL(link_num, video_01);

//				HDCP22_NUM_H15_RdFL(link_num, ad_hdcp22_out_data_align);
//				HDCP22_NUM_H15_Wr01(link_num, ad_hdcp22_out_data_align, o_reg_hdcp22_out_data_align_0, 0x0);
//				HDCP22_NUM_H15_WrFL(link_num, ad_hdcp22_out_data_align);
			}
			else {
				PHY_REG_H15A0_RdFL(ch_chg_420_mode);
				PHY_REG_H15A0_Wr01(ch_chg_420_mode,ch_chg_420,0x0);
				PHY_REG_H15A0_WrFL(ch_chg_420_mode);
			}
#else
			HDMI_NOTI("COLORSPACE Not YCBCR420 : setting phy [%d]\n", csc);
			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(ch_chg_420);
				PHY_REG_H15B0_Wr01(ch_chg_420,ch_chg_420,0x0);
				PHY_REG_H15B0_WrFL(ch_chg_420);
			}
			else {
				PHY_REG_H15A0_RdFL(ch_chg_420_mode);
				PHY_REG_H15A0_Wr01(ch_chg_420_mode,ch_chg_420,0x0);
				PHY_REG_H15A0_WrFL(ch_chg_420_mode);
			}
#endif
		}
	}

	sHDMI_H15A0_Status.AVIReadState[link_num] = TRUE;

	if ( memcmp(&sHDMI_H15A0_Status.PrevAVIPacket[link_num], packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		gAVIReadCnt = 0;
		HDMI_VIDEO("[%d] %s link[%d]: changed AVI Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, link_num, sHDMI_H15A0_Status.PrevAVIPacket[link_num].InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("AVI header	gPre[%d] /packet[%d] \n", sHDMI_H15A0_Status.PrevAVIPacket[link_num].header, packet->header);
		HDMI_VIDEO("AVI data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[0], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[1], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[2], \
			sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[3], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[4], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[5], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[6], sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[7]);
		HDMI_VIDEO("AVI data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&sHDMI_H15A0_Status.PrevAVIPacket[link_num], packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		return RET_OK;
	}
	else
	{
		if (gAVIReadCnt < 350) 		gAVIReadCnt++;
		return RET_OK;
	}

func_exit:
	memcpy(packet, &sHDMI_H15A0_Status.PrevAVIPacket[link_num],  (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	HDMI_PRINT("[%d] %s \n", __L__, __F__);

	return RET_OK;
}

int HDMI_H15A0_GetVsiPacket(int link_num, LX_HDMI_INFO_PACKET_T *packet)
{
	ULONG flags = 0;
	UINT32 header;
	UINT32 data[9];
	static UINT32 gVSIReadCnt = 0;

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)packet , 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	packet->InfoFrameType = LX_HDMI_INFO_VSI;

	//HDMI_DEBUG("%s, %d sHDMI_H15A0_Status.Intr_vf_stable[link_num]= %d,sHDMI_H15A0_Status.Intr_vsi[link_num] = %d, \n\n",__F__, __L__, sHDMI_H15A0_Status.Intr_vf_stable[link_num], sHDMI_H15A0_Status.Intr_vsi[link_num]);

	if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INIT_STATE ||sHDMI_H15A0_Status.Intr_vsi[link_num] < HDMI_VSI_INTERRUPT_STATE)
	{
		//HDMI_VIDEO("%s, %d sHDMI_H15A0_Status.Intr_vf_stable[link_num]= %d,sHDMI_H15A0_Status.Intr_vsi[link_num] = %d, \n\n",__F__, __L__, sHDMI_H15A0_Status.Intr_vf_stable[link_num], sHDMI_H15A0_Status.Intr_vsi[link_num]);
		gVSIReadCnt = 0;
		memset(&sHDMI_H15A0_Status.PrevVSIPacket[link_num], 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		sHDMI_H15A0_Status.PrevVSIPacket[link_num].InfoFrameType = LX_HDMI_INFO_VSI;
		return RET_OK;
	}

#ifdef H15_BRINGUP_TODO
	if ( _gH15A0HDMIPhyInform.tmds_clock[link_num] > 29600 )
	{
		gVSIReadCnt = 0;
		HDMI_VIDEO("[%d] %s : not support 4Kx2K over 296Mhz [%d0 Khz]\n", __L__, __F__,_gH15A0HDMIPhyInform.tmds_clock[0]);
		memset(&sHDMI_H15A0_Status.PrevVSIPacket[link_num], 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		sHDMI_H15A0_Status.PrevVSIPacket[link_num].InfoFrameType = LX_HDMI_INFO_VSI;
		return RET_OK;
	}
#endif

	if (sHDMI_H15A0_Status.VSIState[link_num] == TRUE)
	{
		{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
			LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
			LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_vsi_enable, 0x1);			///< 20 intr_no_vsi_enable
			LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);
			sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
			HDMI_DEBUG("[%d] %s : No VSI intra enable  \n", __LINE__, __func__);
		}
	}

	if ( _gH15A0HDMI_thread_running == 1 && sHDMI_H15A0_Status.Intr_vsi[link_num] == HDMI_VSI_STABLE_STATE)
		goto func_exit;

	if (gVSIReadCnt > 200)
	{
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_STABLE_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
	}

	LINK_NUM_H15_RdFL(link_num, packet_74);
	header = LINK_NUM_H15_Rd(link_num, packet_74);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
	packet->header = (header &0xffff);

	LINK_NUM_H15_RdFL(link_num, packet_75);
	LINK_NUM_H15_RdFL(link_num, packet_76);
	LINK_NUM_H15_RdFL(link_num, packet_77);
	LINK_NUM_H15_RdFL(link_num, packet_78);
	LINK_NUM_H15_RdFL(link_num, packet_79);
	LINK_NUM_H15_RdFL(link_num, packet_80);
	LINK_NUM_H15_RdFL(link_num, packet_81);
	data[0] = LINK_NUM_H15_Rd(link_num, packet_75);
	data[1] = LINK_NUM_H15_Rd(link_num, packet_76);
	data[2] = LINK_NUM_H15_Rd(link_num, packet_77);
	data[3] = LINK_NUM_H15_Rd(link_num, packet_78);
	data[4] = LINK_NUM_H15_Rd(link_num, packet_79);
	data[5] = LINK_NUM_H15_Rd(link_num, packet_80);
	data[6] = LINK_NUM_H15_Rd(link_num, packet_81);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];
	packet->dataBytes[7] = 0;

	if ( memcmp(&sHDMI_H15A0_Status.PrevVSIPacket[link_num], packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		gVSIReadCnt = 0;
		HDMI_VIDEO("[%d] %s : changed VSI Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, sHDMI_H15A0_Status.PrevVSIPacket[link_num].InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("VSI header	gPre[%d] /packet[%d] \n", sHDMI_H15A0_Status.PrevVSIPacket[link_num].header, packet->header);
		HDMI_VIDEO("VSI data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[0], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[1], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[2], \
			sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[3], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[4], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[5], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[6], sHDMI_H15A0_Status.PrevVSIPacket[link_num].dataBytes[7]);
		HDMI_VIDEO("VSI data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&sHDMI_H15A0_Status.PrevVSIPacket[link_num], packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		return RET_OK;
	}
	else
	{
		if (gVSIReadCnt < 250) 		gVSIReadCnt++;
		return RET_OK;
	}

func_exit:
	memcpy(packet, &sHDMI_H15A0_Status.PrevVSIPacket[link_num], (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	HDMI_PRINT("[%d] %s \n", __L__, __F__);

	return RET_OK;
}

int HDMI_H15A0_GetSpdPacket(int link_num, LX_HDMI_INFO_PACKET_T *packet)
{
	UINT32 header;
	UINT32 data[9];
	static LX_HDMI_INFO_PACKET_T gPrevSPDPacket = {0, };

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)packet , 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	packet->InfoFrameType = LX_HDMI_INFO_SPD;

	if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INIT_STATE)
	{
		memset(&gPrevSPDPacket, 0 , (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
		gPrevSPDPacket.InfoFrameType = LX_HDMI_INFO_SPD;
		return RET_OK;
	}

	LINK_NUM_H15_RdFL(link_num, packet_58);
	header = LINK_NUM_H15_Rd(link_num, packet_58);
	packet->header = (header &0xffff);

	LINK_NUM_H15_RdFL(link_num, packet_59);
	LINK_NUM_H15_RdFL(link_num, packet_60);
	LINK_NUM_H15_RdFL(link_num, packet_61);
	LINK_NUM_H15_RdFL(link_num, packet_62);
	LINK_NUM_H15_RdFL(link_num, packet_63);
	LINK_NUM_H15_RdFL(link_num, packet_64);
	LINK_NUM_H15_RdFL(link_num, packet_65);

	data[0] = LINK_NUM_H15_Rd(link_num, packet_59);
	data[1] = LINK_NUM_H15_Rd(link_num, packet_60);
	data[2] = LINK_NUM_H15_Rd(link_num, packet_61);
	data[3] = LINK_NUM_H15_Rd(link_num, packet_62);
	data[4] = LINK_NUM_H15_Rd(link_num, packet_63);
	data[5] = LINK_NUM_H15_Rd(link_num, packet_64);
	data[6] = LINK_NUM_H15_Rd(link_num, packet_65);

	packet->dataBytes[0] = data[0];
	packet->dataBytes[1] = data[1];
	packet->dataBytes[2] = data[2];
	packet->dataBytes[3] = data[3];
	packet->dataBytes[4] = data[4];
	packet->dataBytes[5] = data[5];
	packet->dataBytes[6] = data[6];
	packet->dataBytes[7] = 0;

	if ( memcmp(&gPrevSPDPacket, packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T)) != 0 )
	{
		HDMI_VIDEO("[%d] %s : changed SPD Packet / InfoFrameType gPre[%d] /packet[%d] \n", __L__, __F__, gPrevSPDPacket.InfoFrameType, packet->InfoFrameType);
		HDMI_VIDEO("SPD header	gPre[%d] /packet[%d] \n", gPrevSPDPacket.header, packet->header);
		HDMI_VIDEO("SPD data	gPre[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", gPrevSPDPacket.dataBytes[0], gPrevSPDPacket.dataBytes[1], gPrevSPDPacket.dataBytes[2], \
			gPrevSPDPacket.dataBytes[3], gPrevSPDPacket.dataBytes[4], gPrevSPDPacket.dataBytes[5], gPrevSPDPacket.dataBytes[6], gPrevSPDPacket.dataBytes[7]);
		HDMI_VIDEO("SPD data	packet[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x] \n", 	packet->dataBytes[0], packet->dataBytes[1], packet->dataBytes[2], \
			packet->dataBytes[3], packet->dataBytes[4], packet->dataBytes[5], packet->dataBytes[6], packet->dataBytes[7]);

		memcpy(&gPrevSPDPacket, packet, (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	}

	return RET_OK;
}

int HDMI_H15A0_GetInfoPacket(int link_num, LX_HDMI_INFO_PACKET_T *packet)
{
	PRT_NUM_CONVERT(link_num);
	//not implemented now!!!
	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)packet, 0, (size_t)sizeof(LX_HDMI_INFO_PACKET_T));

	if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INIT_STATE)
		return RET_OK;

	return RET_OK;
}

int HDMI_H15A0_SetPowerControl(UINT32 power)
{
	/*
	if (power)
	{
		_gH15A0HDMIPhyInform.power_control = 1;
	}
	else if (!gHDMI_Phy_Control.all_port_pdb_enable)
	{
		_gH15A0HDMIPhyInform.power_control = 0;

		_HDMI_H15A0_PhyOff(0);
		_HDMI_H15A0_PhyOff(1);

		_HDMI_H15A0_Set_HPD_Out_A4P(0,0,0,0);
	}

	*/
	HDMI_PRINT("[%d] %s :  %s \n", __L__, __F__, (power ? "On" : "Off"));

	return RET_OK;
}

int HDMI_H15A0_SetHPD(LX_HDMI_HPD_T *hpd)
{
	HDMI_NOTI("HPD enable [%s]\n",(hpd->bEnable ? "On" : "Off"));

	OS_LockMutex(&g_HDMI_Sema);

	if (hpd->bEnable)
		_gH15A0HDMIPhyInform.hpd_enable = 1;
	else
	{
		_gH15A0HDMIPhyInform.hpd_enable = 0;

			_HDMI_H15A0_PhyOff(0);
			_HDMI_H15A0_PhyOff(1);

			_HDMI_H15A0_Set_HPD_Out_A4P(!_gH15A0HDMIPhyInform.hpd_pol[0], \
					!_gH15A0HDMIPhyInform.hpd_pol[1]);
			_HDMI_H15A0_Set_GPIO_HPD_Out(0, !_gH15A0HDMIPhyInform.hpd_pol[0]);	// disable HPD

			_gH15A0HDMIPhyInform.hpd_hdcp22_control[0] = 0;
#ifndef USE_EXT_MHL_IC
			_gH15A0HDMIPhyInform.hpd_hdcp22_control[1] = 0;
#endif
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return RET_OK;
}


int HDMI_H15A0_GetStatus(int link_num, LX_HDMI_STATUS_T *status)
{
	UINT32 tmdsClock = 0;
#ifndef H15_THREAD_READ_PHY_REG_VALUE
	UINT32 up_freq = 0,	down_freq = 0;
#endif
	LX_HDMI_AVI_COLORSPACE_T csc = 0;		    ///< PixelEncoding

	PRT_NUM_CONVERT(link_num);

	if(PRT_NUM_INVALID(link_num))
		return RET_ERROR;

	memset((void *)status , 0 , (size_t)sizeof(LX_HDMI_STATUS_T));

	do {
		if (sHDMI_H15A0_Status.HDMIState[link_num] < HDMI_STATE_SCDT)
			break;

		status->bHdmiMode = sHDMI_H15A0_Status.hdmi_mode[link_num];

		status->eHotPlug = sHDMI_H15A0_Status.HDMIConnectState[link_num];

		LINK_NUM_H15_RdFL(link_num, video_02);
		LINK_NUM_H15_Rd01(link_num, video_02, reg_cmode_tx, status->eColorDepth);

		status->eColorDepth = (status->eColorDepth &0x60)>>5;

		LINK_NUM_H15_RdFL(link_num, video_00);
		LINK_NUM_H15_Rd01(link_num, video_00, reg_pr_tx, status->pixelRepet);

		status->csc = sHDMI_H15A0_Status.PrevPixelEncoding[link_num];
		csc = (((sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[0] &0xff00)>>8) &0x60)>>5;
		if (sHDMI_H15A0_Status.PrevPixelEncoding[link_num] == csc)		HDMI_DEBUG(" sHDMI_H15A0_Status.PrevPixelEncoding[link_num] = %d , csc = %d \n", sHDMI_H15A0_Status.PrevPixelEncoding[link_num], csc);

#ifdef H15_THREAD_READ_PHY_REG_VALUE
		tmdsClock = _gH15A0HDMIPhyInform.tmds_clock[link_num]; 	// XXX.XX KHz
#else
		tmdsClock = _HDMI_H15_Get_Clock_Freq();
#endif

		HDMI_VIDEO("HDMI_H15A0_GetStatus  depth = 0x%x tmds =%d\n", status->eColorDepth, tmdsClock);
		HDMI_VIDEO("[HDMI] %d HDMI  State  = %d _gH15A0Force_thread_sleep= %d  _gH15A0HDMI_thread_running =%d \n",\
			__L__, sHDMI_H15A0_Status.HDMIState[link_num],_gH15A0Force_thread_sleep, _gH15A0HDMI_thread_running);
		HDMI_VIDEO("[HDMI]  AVI State[%d] VSI State[%d]  SRC Mute State[%d] \n",  sHDMI_H15A0_Status.Intr_avi[link_num], sHDMI_H15A0_Status.Intr_vsi[link_num], sHDMI_H15A0_Status.Intr_src_mute[link_num] );
		HDMI_VIDEO("[HDMI]  H15 : 20140916(BD-P1600) \n");
	} while (0);

	return RET_OK;
}

/**
 *  HDMI_H15A0_GetAudioInfo
 *
 *  @parm LX_HDMI_AUDIO_INFO_T *
 *  @return int
*/
int HDMI_H15A0_GetAudioInfo(LX_HDMI_AUDIO_INFO_T *pAudioInfo)
{
	int ret = RET_OK;
	int link_num = 0;

	//Set a HDMI Link Number
	link_num = pAudioInfo->linkNumber;

	//Check a HDMI Link Number
	if (link_num >= HDMI_LINK_MAX)
	{
		HDMI_DEBUG("[%d] %s : link_num = %d is error!!!\n", __L__, __F__, link_num);
		return RET_ERROR;
	}

	OS_LockMutex(&g_HDMI_Sema);

	//Return a previous global value.
	pAudioInfo->audioType	 = _gH15A0HdmiAudioInfo[link_num].audioType;
	pAudioInfo->samplingFreq = _gH15A0HdmiAudioInfo[link_num].samplingFreq;

	//Debug print
	if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_1S) == 0)
	{
		HDMI_AUDIO("HDMI_GetAudioInfo : link_num = %d, type = %d, freq = %d, state = %d, mute = %d\n",	\
					link_num, pAudioInfo->audioType, pAudioInfo->samplingFreq, _gH15A0IntrAudioState[link_num], _gH15A0AudioMuteState[link_num]);
	}
	else
	{
		HDMI_PRINT("HDMI_GetAudioInfo : link_num = %d, type = %d, freq = %d, state = %d, mute = %d\n",	\
					link_num, pAudioInfo->audioType, pAudioInfo->samplingFreq, _gH15A0IntrAudioState[link_num], _gH15A0AudioMuteState[link_num]);
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return ret;
}

/**
 *  HDMI_H15A0_GetAudioCopyInfo
 *
 *  @parm LX_HDMI_AUDIO_COPY_INFO_T *
 *  @return int
*/
int HDMI_H15A0_GetAudioCopyInfo(LX_HDMI_AUDIO_COPY_INFO_T *pCopyInfo)
{
	int ret = RET_OK;
	int link_num = 0;

	UINT32	reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1;
	UINT32	CpBit, LBit;

	//Set a HDMI Link Number
	link_num = pCopyInfo->linkNumber;

	//Check a HDMI Link Number
	if (link_num >= HDMI_LINK_MAX)
	{
		HDMI_DEBUG("[%d] %s : link_num = %d is error!!!\n", __L__, __F__, link_num);
		return RET_ERROR;
	}

	OS_LockMutex(&g_HDMI_Sema);

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	//Get a hdmi audio copyright information.
	if (reg_hdmi_mode == 1)	//HDMI Mode
	{
		//Read  reg_achst_byte0 reg.
		LINK_NUM_H15_RdFL(link_num, audio_07);
		LINK_NUM_H15_Rd01(link_num, audio_07, reg_achst_byte0, reg_achst_byte0);

		//Set a CpBit
		if (reg_achst_byte0 & HDMI_AUDIO_CP_BIT_MASK)
			CpBit = 1;
		else
			CpBit = 0;

		//Read  reg_achst_byte1reg.
		LINK_NUM_H15_RdFL(link_num, audio_08);
		LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte1, reg_achst_byte1);

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
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_1S) == 0)
		{
			HDMI_AUDIO("HDMI_H15A0_GetAudioCopyInfo : link_num = %d, CopyInfo = %d(mode = %d)(0x%X, 0x%X)\n",	\
						link_num, pCopyInfo->copyInfo, reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1);
		}
		else
		{
			HDMI_PRINT("HDMI_H15A0_GetAudioCopyInfo : link_num = %d, CopyInfo = %d(mode = %d)(0x%X, 0x%X)\n",	\
						link_num, pCopyInfo->copyInfo, reg_hdmi_mode, reg_achst_byte0, reg_achst_byte1);
		}
	}
	else	//DVI Mode
	{
		pCopyInfo->copyInfo = LX_HDMI_AUDIO_COPY_FREE;
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	HDMI_PRINT("HDMI_H15A0_GetAudioCopyInfo : link_num = %d, CopyInfo = %d(mode = %d)\n", link_num, pCopyInfo->copyInfo, reg_hdmi_mode);
	return ret;
}

/**
 *  HDMI_H15A0_SetArc
 *
 *  @parm LX_HDMI_ARC_CTRL_T *
 *  @return int
*/
int HDMI_H15A0_SetArc(LX_HDMI_ARC_CTRL_T *pArcCtrl)
{
	int link_num = 0;

	//Set a HDMI Link Number
	link_num = pArcCtrl->linkNumber;

	//Check a HDMI Link Number
	if (link_num >= HDMI_LINK_MAX)
	{
		HDMI_DEBUG("[%d] %s : link_num = %d is error!!!\n", __L__, __F__, link_num);
		return RET_ERROR;
	}

	OS_LockMutex(&g_HDMI_Sema);

	//ARC source
	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Wr01(link_num, system_control_00, reg_arc_src, 0x0);
	LINK_NUM_H15_WrFL(link_num, system_control_00);

	//Enable or Disable ARC port
	LINK_NUM_H15_RdFL(link_num, phy_00);
	LINK_NUM_H15_Wr01(link_num, phy_00, phy_arc_odt_sel, 0x2);	///< Port1 PHY ARC Temination 저항값 설정 (Default 'b10 Setting 요청!)
	LINK_NUM_H15_Wr01(link_num, phy_00, phy_arc_pdb, (UINT32)pArcCtrl->bARCEnable);		///< Port1 PHY ARC PDB
	LINK_NUM_H15_WrFL(link_num, phy_00);

	//Update _gAudioArcStatus
	_gH15A0AudioArcStatus[link_num] = pArcCtrl->bARCEnable;

	OS_UnlockMutex(&g_HDMI_Sema);

	HDMI_PRINT("[%d] %s : link_num = %d, ARC = %s \n", __L__, __F__, link_num, pArcCtrl->bARCEnable? "On":"Off");
	return RET_OK;
}

/**
 *  HDMI_H15A0_SetMute
 *
 *  @parm LX_HDMI_MUTE_CTRL_T *
 *  @return int
*/
int HDMI_H15A0_SetMute(LX_HDMI_MUTE_CTRL_T *pMuteCtrl)
{
	int link_num = 0;

	BOOLEAN 	videoMuteState;

	//Set a HDMI Link Number
	link_num = pMuteCtrl->linkNumber;

	//Check a HDMI Link Number
	if (link_num >= HDMI_LINK_MAX)
	{
		HDMI_DEBUG("[%d] %s : link_num = %d is error!!!\n", __L__, __F__, link_num);
		return RET_ERROR;
	}

	OS_LockMutex(&g_HDMI_Sema);

	//Set a HDMI Link Number
	link_num = pMuteCtrl->linkNumber;

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

	//audio related.
	if (pMuteCtrl->eMode == LX_HDMI_AUDIO_MUTE || pMuteCtrl->eMode == LX_HDMI_AV_MUTE)
	{
		//Check a previous state
		if (_gH15A0AudioMuteState[link_num] != pMuteCtrl->bAudioMute)
		{
			if (pMuteCtrl->bAudioMute == TRUE)
			{
				//Mute audio data
				LINK_NUM_H15_RdFL(link_num, audio_05);
				LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd_en, 0x0);	//I2S SD Output Disable(4 Channel)
				LINK_NUM_H15_WrFL(link_num, audio_05);
			}
			else
			{
				//Un-mute audio data
				LINK_NUM_H15_RdFL(link_num, audio_05);
				LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd_en, 0xF);	//I2S SD Output Enable(4 Channel)
				LINK_NUM_H15_WrFL(link_num, audio_05);
			}

			//Update _gH15A0AudioMuteState
			_gH15A0AudioMuteState[link_num] = pMuteCtrl->bAudioMute;

			//For debug print
			if (pMuteCtrl->bAudioMute == FALSE)
			{
				HDMI_AUDIO("[%d] %s : link_num = %d, bAudioMute = %s \n",	\
							__L__, __F__, link_num, (pMuteCtrl->bAudioMute ? "On" : "Off"));
			}
			else
			{
				HDMI_DEBUG("[%d] %s : link_num = %d, bAudioMute = %s \n",	\
							__L__, __F__, link_num, (pMuteCtrl->bAudioMute ? "On" : "Off"));
			}
		}

		//For debug print
		if (pMuteCtrl->bAudioMute == FALSE)
		{
			HDMI_DEBUG("[%d]SetMute : link_num = %d, type = %d, freq = %d, mute = %d, PSC = %d\n", \
						__L__, link_num, _gH15A0HdmiAudioInfo[link_num].audioType, _gH15A0HdmiAudioInfo[link_num].samplingFreq,	\
						_gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num]);
		}
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return RET_OK;
}

/**
 *	HDMI_M14C0_SetAudioDebugInterrupt
*/
static void HDMI_H15A0_SetAudioDebugInterrupt(int link_num, UINT8 enable)
{
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_00);
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_cts_chg_enable, enable); 		///< 14 intr_cts_chg_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_n_chg_enable, enable);		///< 15 intr_n_chg_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_fs_chg_enable, enable);		///< 16 intr_fs_chg_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2pcm_chg_enable, enable);		///< 17 intr_2pcm_chg_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2npcm_chg_enable, enable);	///< 18 intr_2npcm_chg_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_aud_type_chg_enable, enable);	///< 19 intr_spdif_err_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_spdif_err_enable, enable);	///< 19 intr_spdif_err_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_afifo_undr_enable, enable);	///< 20 intr_afifo_undr_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_afifo_ovrr_enable, enable);	///< 21 intr_afifo_ovrr_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_00);

	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_ecc_err_enable,	enable);	///< 20 intr_ecc_err_enable
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_terc4_err_enable, enable);	///< 21 intr_terc4_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	HDMI_NOTI("HDMI_H15A0_SetAudioDebugInterrupt : link_num = %d, enable = %d\n", link_num, enable);
	return;
}

/**
 *  HDMI_H15A0_GetAudioDebugInfo
 *
 *  @parm LX_HDMI_AUDIO_INFO_T *
 *  @return int
*/
int HDMI_H15A0_GetAudioDebugInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo)
{
	int ret = RET_OK;
	int link_num = 0;

	//Set a HDMI Link Number
	link_num = pAudioDebugInfo->linkNumber;

	//Check a HDMI Link Number
	if (link_num == (HDMI_LINK_0 + 10) || link_num == (HDMI_LINK_1 + 10))
	{
		//Revise link number.
		link_num = link_num - 10;

		//Set a HDMI Audio Debug Enabled Status for toggle.
		if(_gH15A0AudioDebugState[link_num] == FALSE)
		{
			_gH15A0AudioDebugState[link_num] = TRUE;
			(void)HDMI_H15A0_SetAudioDebugInterrupt(link_num, 1);
		}
		else
		{
			_gH15A0AudioDebugState[link_num] = FALSE;
			(void)HDMI_H15A0_SetAudioDebugInterrupt(link_num, 0);
		}
	}
	else
	{
		//Check a HDMI Link Number
		if (link_num >= HDMI_LINK_MAX)
		{
			HDMI_DEBUG("[%d] %s : link_num = %d is error!!!\n", __L__, __F__, link_num);
			return RET_ERROR;
		}
	}

	OS_LockMutex(&g_HDMI_Sema);

	//Copy and Print HDMI Audio Debug Info.
	(void)_HDMI_H15A0_DebugAudioInfo(link_num, pAudioDebugInfo);

	HDMI_DEBUG("HDMI_H15A0_GetAudioDebugInfo : link_num = %d, type = %d, freq = %d, state = %d, mute = %d, PSC = %d, video = %d\n", \
				link_num, _gH15A0HdmiAudioInfo[link_num].audioType, _gH15A0HdmiAudioInfo[link_num].samplingFreq, _gH15A0IntrAudioState[link_num],	\
				_gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num], sHDMI_H15A0_Status.HDMIState[link_num]);

	OS_UnlockMutex(&g_HDMI_Sema);

	return ret;
}

static void _HDMI_H15A0_ModifyUDFormat(LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock)
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

static void _HDMI_H15A0_CheckUDExtInfo(int link_num, LX_HDMI_TIMING_INFO_T *info, UINT32 tmdsClock)
{
	UINT16 vic = 0;

	do {
		if (tmdsClock < 29000) break;

		if (sHDMI_H15A0_Status.Intr_avi[link_num] > HDMI_AVI_INIT_STATE)
		{
			vic = ((sHDMI_H15A0_Status.PrevAVIPacket[link_num].dataBytes[1] &0xff) &0x7f);	// VIC6~VIC0
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
static int _HDMI_H15A0_GetExtendTimingInfo(int link_num, LX_HDMI_TIMING_INFO_T *info)
{
	UINT8 	tmp8 =0;
	UINT32	tmp32 = 0, i = 0;
	UINT32 	videoIdCode = 0;
	LX_HDMI_TIMING_INFO_T 	bufTiming = {0,};
	LX_HDMI_VSI_VIDEO_FORMAT_T eVideoFormat = 0;	/**< HDMI VSI info */
	LX_HDMI_VSI_3D_STRUCTURE_T e3DStructure = 0;	/**< HDMI VSI info */
	const sEXT_TIMING_ENUM *pTbl;
	const sO_ENUM *p3DTbl;

	do {
		memcpy(&bufTiming , info , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
		bufTiming.extInfo = LX_HDMI_EXT_2D_FORMAT;		///< 2D format

		if (sHDMI_H15A0_Status.Intr_vsi[link_num] < HDMI_VSI_INTERRUPT_STATE)
		{
			_HDMI_H15A0_CheckUDExtInfo(link_num, &bufTiming, _gH15A0HDMIPhyInform.tmds_clock[link_num]);		// check UD format
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

			if (bufTiming.extInfo == LX_HDMI_EXT_3D_LINE_ALTERNATIVE)
			{
				LINK_NUM_H15_RdFL(link_num, packet_19);
				tmp32 = LINK_NUM_H15_Rd(link_num, packet_19);
				videoIdCode = (((tmp32 &0xff)) &0x7f);				// VIC6~VIC0

				if (videoIdCode > 92)
				{
					bufTiming.hActive 		= info->hActive;
					bufTiming.vActive 		= info->vActive;
					bufTiming.scanType	= info->scanType;					;
					bufTiming.extInfo = LX_HDMI_EXT_2D_FORMAT;
					HDMI_VIDEO("[%d] %s : check Extend TimingInfo 3D_LA to 2D for NO VSI and HDMI 2.0 \n",  __L__, __F__);
				}
			}
			break;
		}

		/**< HDMI VSI info */
		LINK_NUM_H15_RdFL(link_num, packet_76);
		tmp32 = LINK_NUM_H15_Rd(link_num, packet_76);
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
			break;

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
				LINK_NUM_H15_RdFL(0, packet_20);		//AVI
				LINK_NUM_H15_Rd(0, packet_20, tmp32);
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
					(bufTiming.hActive == 1920 && bufTiming.vActive == 2160 && _gH15A0HDMIPhyInform.tmds_clock[link_num] > 29000) || \
					(bufTiming.hActive == 2048 && bufTiming.vActive == 2160 && _gH15A0HDMIPhyInform.tmds_clock[link_num] > 29000) ) )
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
			memset(&bufTiming , 0 , (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
			HDMI_VIDEO("[%d] %s : Not support 3D 1920x1080i@50(1250)  \n",  __L__, __F__);
		}
	} while(0);

	if ( ( _gH15A0HDMI_thread_running == 0 &&  sHDMI_H15A0_Status.TimingReadCnt[link_num] < 23 && sHDMI_H15A0_Status.TimingReadCnt[link_num] > 20) ||
		(sHDMI_H15A0_Status.HDMIState[link_num] >= HDMI_STATE_READ && sHDMI_H15A0_Status.TimingReadCnt[link_num] < 6 && sHDMI_H15A0_Status.TimingReadCnt[link_num] > 3) )
	{
		//HDMI_VIDEO(" VIC  = %d [gpreAVI - %d] \n", videoIdCode, _sHDMI_H15A0_Status.PrevAVIPacket[link_num].eAviPacket.VideoIdCode);
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

static int _HDMI_H15A0_SetInternalMute(int link_num, LX_HDMI_MUTE_CTRL_T interMute)
{
	BOOLEAN 	videoMuteState;

	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Rd01(link_num, system_control_00, reg_mute_vid, videoMuteState);

	//video related.
	if (interMute.eMode == LX_HDMI_VIDEO_MUTE || interMute.eMode == LX_HDMI_AV_MUTE)
	{
		if (interMute.bVideoMute != videoMuteState)
		{
			LINK_NUM_H15_Wr01(link_num, system_control_00, reg_mute_vid, interMute.bVideoMute);
			LINK_NUM_H15_WrFL(link_num, system_control_00);
			HDMI_VIDEO("[%d] %s : link_num = %d, bVideoMute = %s \n", __L__, __F__, link_num, (interMute.bVideoMute ? "On" : "Off"));
		}
		else
			HDMI_VIDEO("[%d] %s : link_num = %d, skip bVideoMute = %s \n", __L__, __F__, link_num, (interMute.bVideoMute ? "On" : "Off"));
	}

	//audio related.
	if (interMute.eMode == LX_HDMI_AUDIO_MUTE || interMute.eMode == LX_HDMI_AV_MUTE)
	{
		//Check a previous state
		if (_gH15A0AudioMuteState[link_num] != interMute.bAudioMute)
		{
			if (interMute.bAudioMute == TRUE)
			{
				//Mute audio data
				LINK_NUM_H15_RdFL(link_num, audio_05);
				LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd_en, 0x0);	//I2S SD Output Disable(4 Channel)
				LINK_NUM_H15_WrFL(link_num, audio_05);
			}
			else
			{
				//Un-mute audio data
				LINK_NUM_H15_RdFL(link_num, audio_05);
				LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd_en, 0xF);	//I2S SD Output Enable(4 Channel)
				LINK_NUM_H15_WrFL(link_num, audio_05);
			}

			//Update _gH15A0AudioMuteState
			_gH15A0AudioMuteState[link_num] = interMute.bAudioMute;

			HDMI_AUDIO("[%d] %s : link_num = %d, bAudioMute = %s \n", __L__, __F__, link_num, (interMute.bAudioMute ? "On" : "Off"));
		}
	}

	return RET_OK;
}

static int _HDMI_H15A0_SetVideoBlank(int link_num, LX_HDMI_AVI_COLORSPACE_T space)
{
	UINT32 blank_r = 0x0;
	UINT32 blank_b = 0x0;

	switch(space)
	{
		case LX_HDMI_AVI_COLORSPACE_YCBCR420:	// H15 added
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
	LINK_NUM_H15_RdFL(link_num, video_09);
	LINK_NUM_H15_Wr01(link_num, video_09, reg_vid_blank_r, blank_r);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_NUM_H15_WrFL(link_num, video_09);

	/* Blank Blue */
	LINK_NUM_H15_RdFL(link_num, video_10);
	LINK_NUM_H15_Wr01(link_num, video_10, reg_vid_blank_b, blank_b);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_NUM_H15_WrFL(link_num, video_10);

	HDMI_VIDEO("[%d] %s : changed video blank color space [%d]  \n",  __L__, __F__, space);
	return RET_OK;
}

static int _HDMI_H15A0_ClearSWResetAll(void)
{
	if (lx_chip_rev() < LX_CHIP_REV(H15, B0)) {
		//Added for H15A1 DDC
		CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_5_mux_en, 0x1);	// hdmi ddc ch 0 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_4_mux_en, 0x0);	// hdmi ddc ch 0 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_3_mux_en, 0x1);	// hdmi ddc ch 1 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_2_mux_en, 0x0);	// hdmi ddc ch 1 enable
		CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
	}
	else {
		//Added for H15A1 DDC
		CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_5_mux_en, 0x0);	// hdmi ddc ch 0 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_4_mux_en, 0x0);	// hdmi ddc ch 0 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_3_mux_en, 0x0);	// hdmi ddc ch 1 enable
		CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_2_mux_en, 0x0);	// hdmi ddc ch 1 enable
		CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
	}

	CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
	CTOP_CTRL_H15A0_Wr01(CVI, ctr00, swrst_aud_dto, 0x1);	//hdmi_dto
	CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
	CTOP_CTRL_H15A0_RdFL(CVI, ctr04);
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_sys, 0x1);	//swrst_sys
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_hdcp, 0x1);	//swrst_hdcp
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_tmds, 0x1);		//swrst_tmds
#ifdef H15_BRINGUP_TODO
	CTOP_CTRL_H15A0_Wr01(DEI, ctr02, swrst_tmds_sel, 0x1);	//
#endif
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_vfifo_w, 0x1);	//swrst_vfifo_w
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_afifo_w, 0x1);	//swrst_afifo_w
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_pix_pip, 0x1);	//swrst_pix_pip
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_vfifo_r, 0x1);	//swrst_vfifo_r
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_aud, 0x1);		//swrst_aud
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_afifo_r, 0x1);	//swrst_afifo_r
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_edid, 0x1);		//swrst_edid
	CTOP_CTRL_H15A0_WrFL(CVI, ctr04);

	OS_MsecSleep(1);	// ms delay

	CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
	CTOP_CTRL_H15A0_Wr01(CVI, ctr00, swrst_aud_dto, 0x0);	//hdmi_dto
	CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
	CTOP_CTRL_H15A0_RdFL(CVI, ctr04);
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_sys, 0x0);	//swrst_sys
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_hdcp, 0x0);	//swrst_hdcp
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_tmds, 0x0);		//swrst_tmds
#ifdef H15_BRINGUP_TODO
	CTOP_CTRL_H15A0_Wr01(DEI, ctr02, swrst_tmds_sel, 0x0);	//
#endif
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_vfifo_w, 0x0);	//swrst_vfifo_w
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_afifo_w, 0x0);	//swrst_afifo_w
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_pix_pip, 0x0);	//swrst_pix_pip
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_vfifo_r, 0x0);	//swrst_vfifo_r
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_aud, 0x0);		//swrst_aud
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_afifo_r, 0x0);	//swrst_afifo_r
	CTOP_CTRL_H15A0_Wr01(CVI, ctr04, reg_swrst_edid, 0x0);		//swrst_edid
	CTOP_CTRL_H15A0_WrFL(CVI, ctr04);

	OS_MsecSleep(1);	// ms delay

	/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
	CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
	CTOP_CTRL_H15A0_Wr01(CVI, ctr00,phy_i2c_sel,1);		//phy_i2c_clk_sel
	CTOP_CTRL_H15A0_WrFL(CVI, ctr00);

	return RET_OK;
}

static int _HDMI_H15A0_GetPortConnection(void)
{
	static UINT32 portCnt[2] = {0, };
	UINT32 scdt[2] = {0, };
//	UINT32 ctrl_repeat = 0;
	UINT32 noVsi_Intra = 0;
	BOOLEAN mode[2] = {0, };
//	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_VIDEO_MUTE};
	int	port_num;
	UINT32 temp;
	UINT32 up_freq = 0,	down_freq = 0;
	int	access_phy[4];
	UINT32	hpd_out_polarity;
	int i;
	static int tmds_clock_abnormal_count[2] = {0,};
	unsigned int elasped_time_eq[2];
	static unsigned int cs_man_changed_time_eq[2];
	unsigned int elasped_2nd_time_eq[2] = {0,};
	unsigned int current_time_eq;

	static BOOLEAN prev_hdmi_mode_prt0 = 0;
	static BOOLEAN prev_hdmi_mode_prt1 = 0;

	static int prev_cr_lock[2] = {0,};
	static int prev_cr_done[2] = {0,};
	static int cr_lock[2] = {0,};
	static int cr_done[2] = {0,};

//	int hdcp_status;
//	UINT8 aksv_data[5];
	/*
	UINT32 hdcp_ri_0, hdcp_ri_1, hdcp_ri_all;
	static UINT32 prev_hdcp_ri_all;
	UINT8 HDCP_AN_Data[8] ={0,} ;
	static UINT8 Prev_HDCP_AN_Data[8] ={0,} ;

	LINK_NUM_H15_RdFL(0, hdcp_05);
	LINK_NUM_H15_Rd01(0, hdcp_05, reg_hdcp_ri_0_prt0, hdcp_ri_0);
	LINK_NUM_H15_Rd01(0, hdcp_05, reg_hdcp_ri_1_prt0, hdcp_ri_1);

	hdcp_ri_all = hdcp_ri_1 << 8 | hdcp_ri_1;

	if (hdcp_ri_all != prev_hdcp_ri_all)
	{
		HDMI_DEBUG("---- HDMI_RI CHG [0x%x] => [0x%x]\n", prev_hdcp_ri_all, hdcp_ri_all);
		prev_hdcp_ri_all = hdcp_ri_all;
	}
	_HDMI_H15A0_Get_AN_Data(0, HDCP_AN_Data);
	if ( memcmp(Prev_HDCP_AN_Data, HDCP_AN_Data, sizeof(HDCP_AN_Data) ) )
	{
		HDMI_DEBUG("---- HDMI_CHG AN[0x%02x%02x%02x%02x%02x%02x%02x%02x]\n",\
			HDCP_AN_Data[0], HDCP_AN_Data[1], HDCP_AN_Data[2], HDCP_AN_Data[3], HDCP_AN_Data[4], \
			HDCP_AN_Data[5], HDCP_AN_Data[6], HDCP_AN_Data[7]);
		memcpy(Prev_HDCP_AN_Data, HDCP_AN_Data, sizeof(HDCP_AN_Data));
	}
	*/
	_HDMI_H15A0_Get_HPD_Out_A4P(&_gH15A0HDMIPhyInform.hpd_out[0], &_gH15A0HDMIPhyInform.hpd_out[1]);
	_HDMI_H15A0_Get_HDMI5V_Info_A4P(&_gH15A0HDMIPhyInform.hdmi5v[0], &_gH15A0HDMIPhyInform.hdmi5v[1]);

//	if ( memcmp(_gH15A0HDMIPhyInform.hpd_out, _gH15A0HDMIPhyInform.hdmi5v, sizeof(_gH15A0HDMIPhyInform.hdmi5v)) )
	if ( ( _gH15A0HDMIPhyInform.hpd_out[0] != !(_gH15A0HDMIPhyInform.hdmi5v[0] ^ _gH15A0HDMIPhyInform.hpd_pol[0]) )\
	|| ( _gH15A0HDMIPhyInform.hpd_out[1] != !(_gH15A0HDMIPhyInform.hdmi5v[1] ^ _gH15A0HDMIPhyInform.hpd_pol[1]) ) )
	{
		for (port_num = 0; port_num < 2;port_num ++)
		{

			hpd_out_polarity = !( _gH15A0HDMIPhyInform.hdmi5v[port_num] ^ _gH15A0HDMIPhyInform.hpd_pol[port_num])  ;	// hpd_out considering polarity
			if ( _gH15A0HDMIPhyInform.hpd_out[port_num] != hpd_out_polarity )
			{
				_gH15A0HDMIPhyInform.tcs_done[port_num] = 0;
				_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;
				_g_TCS_EN_restart[port_num] = 1;
				_g_ecc_err_authed[port_num] = 0;

				if (!_gH15A0HDMIPhyInform.hdmi5v[port_num])		// Cable Disconnected !!!
				{
					HDMI_NOTI("---- HDMI_PHY:DisConn port[%d] = [%d] => [%d]\n", port_num, _gH15A0HDMIPhyInform.hpd_out[port_num], hpd_out_polarity);
//					if (gHDMI_Phy_Control.link_reset_control)
//						_HDMI_H15A0_TMDS_HDCP_ResetPort(port_num);
						_HDMI_H15A0_HDCP_ResetPort(port_num);
#ifdef H15_BRINGUP_TODO
						_HDMI_H15A0_TMDS_ResetPort_Control(port_num ,0);
#endif

					if (gHDMI_Phy_Control.all_port_pdb_enable)
						_HDMI_H15A0_PhyOff(port_num);

					_HDMI_H15A0_Set_HPD_Out(port_num, hpd_out_polarity);	// disable HPD
					if(port_num == 0)
						_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, hpd_out_polarity);	// disable HPD

					sHDMI_H15A0_Status.PrevPixelEncoding[port_num] = LX_HDMI_AVI_COLORSPACE_RGB;	// this is for 420 phy register setting

					if (port_num == _gH15A0HDMIPhyInform.prt_sel)
						_gH15A0_tcs_fail_restart_count[port_num] = 0;

#ifdef USE_EXT_MHL_IC
					if(port_num != EXT_MHL_IC_LINK_NUM)
#endif
						_gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num] = 0;

					if (port_num == _gH15A0HDMIPhyInform.prt_sel) {
						_gH15A0HDMIPhyInform.ecc_error_count[port_num] = 0;
						_gH15A0HDMIPhyInform.hdcp_error_count[port_num] = 0;
					}
					_gH15A0HDMIPhyInform.terc4_error_count[port_num] = 0;
					_gH15A0HDMIPhyInform.loop_count[port_num] = 0;
					_gH15A0HDMIPhyInform.scdt_detect_count[port_num] = 0;
					_gH15A0HDMIPhyInform.scdt_fall_count[port_num] = 0;
				}
				else	// cable connected
				{
					_Port_Change_StartTime = jiffies_to_msecs(jiffies);

					//if (gHDMI_Phy_Control.all_port_pdb_enable && _gH15A0HDMIPhyInform.module_open)
					if (_gH15A0HDMIPhyInform.hpd_enable && (_gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num] == 1) && (_g_HDMI_Device_Init_Completed == 1) )
					{
						HDMI_NOTI("---- HDMI_PHY:Conn port[%d] : HPD [%d] => [%d]\n", port_num, _gH15A0HDMIPhyInform.hpd_out[port_num], hpd_out_polarity);
						/* Phy Port Enable Can Be Changed HERE */

						/* All Phy Disabled except port_num */
						if (port_num != 0)
							_HDMI_H15A0_EnablePort(0, 0);
						if (port_num != 1)
							_HDMI_H15A0_EnablePort(1, 0);

						/* Enable port_num */
						_HDMI_H15A0_EnablePort(port_num, 1);

						LINK_NUM_H15_RdFL(port_num, phy_00);
						HDMI_DEBUG("-- HDMI_PHY: Port[%d] :phy_00[0x%x]\n", port_num, LINK_NUM_H15_Rd(port_num ,phy_00));

						OS_MsecSleep(10); // delay after enable port and phy register write

						/* Phy ON & phy run-reset */
						_HDMI_H15A0_PhyOn_5V(port_num);

						_HDMI_H15A0_Set_ManMHLMode(port_num, 1, 0);
						_HDMI_H15A0_Set_ManMHLMode(port_num, 1, 1);
						_HDMI_H15A0_Set_ManMHLMode(port_num, 1, 0);

						_HDMI_H15A0_Set_HPD_Out(port_num, hpd_out_polarity);	// Enable HPD
						_HDMI_H15A0_Set_HPD_Out(port_num, !hpd_out_polarity);	// Enable HPD
						_HDMI_H15A0_Set_HPD_Out(port_num, hpd_out_polarity);	// Enable HPD
						if(port_num == 0)
							_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, hpd_out_polarity);	// Enable HPD

						/* if port_num is not current selected port , disable port */
						if (port_num != _gH15A0HDMIPhyInform.prt_sel)
							_HDMI_H15A0_EnablePort(port_num, 0);

						/* Enable selected port */
						_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

#ifndef USE_EXT_MHL_IC
						if ( ( port_num == 1) && (_HDMI_H15A0_Get_ManMHLMode() == 1) && ( _gH15A0_HDMI_MHLAutoModeCnt == 0) )// MHL port
						{
							_gH15A0_HDMI_MHLAutoModeCnt = 20; // MHL Mode to HDMI for 400msec
						}
#endif
#if 0
#ifdef H15_CODE_FOR_MHL_CTS
#ifdef H15_CBUS_PDB_CTRL
						if ( (port_num == 3) && (_gH15A0HDMIPhyInform.cd_sense == 0) )
						{
							int phy_en_prt0, phy_en_prt1, phy_en_prt2, phy_en_prt3;
							HDMI_DEBUG("---- no CD_SENSE : HDMI Mode \n");

							LINK_NUM_H15_RdFL(0, phy_00);
							LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

							LINK_NUM_H15_RdFL(0, phy_02);
							LINK_NUM_H15_Rd01(0, phy_02, phy_enable_prt1, phy_en_prt1);		//PHY Enable

							LINK_NUM_H15_RdFL(0, phy_04);
							LINK_NUM_H15_Rd01(0, phy_04, phy_enable_prt2, phy_en_prt2);		//PHY Enable

							LINK_NUM_H15_RdFL(0, phy_06);
							LINK_NUM_H15_Rd01(0, phy_06, phy_enable_prt3, phy_en_prt3);		//PHY Enable

							if (!phy_en_prt3)	// IF port3(C-BUS) is not enabled?
							{
								_HDMI_H15A0_EnablePort(0, 0);
								_HDMI_H15A0_EnablePort(1, 0);
								_HDMI_H15A0_EnablePort(2, 0);
								_HDMI_H15A0_EnablePort(3, 1);
							}

							{
								/*
								HDMI_DEBUG("---- MHL Set ODT PDB Mode to 0x00\n");
								PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
								PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
								PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x0);
								PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);
								*/
								HDMI_DEBUG("---- NO CD_SENSE for MHL,  PDB_D0_MAN_SEL :0x00, PDB_DCK_MAN_SEL :0x00\n");
								PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
								PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x0);
								PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
								PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

								PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
								PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x0);
								PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
								PHY_REG_H15A0_WrFL(pdb_dck_man_sel);

							}

							if (!phy_en_prt3)
							{
								_HDMI_H15A0_EnablePort(0, phy_en_prt0);
								_HDMI_H15A0_EnablePort(1, phy_en_prt1);
								_HDMI_H15A0_EnablePort(2, phy_en_prt2);
								_HDMI_H15A0_EnablePort(3, phy_en_prt3);
							}
						}
#endif
#endif
#endif
					}
					else
						HDMI_TASK("HDMI module is not enabled hpd_en[%d], hpd_hdcp22_control[%d]!!!\n" \
								, _gH15A0HDMIPhyInform.hpd_enable , _gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num]);
				}
//				_gH15A0HDMIPhyInform.hpd_out[port_num] = _gH15A0HDMIPhyInform.hdmi5v[port_num];
			}
		}

		//memcpy(_gH15A0HDMIPhyInform.hpd_out, _gH15A0HDMIPhyInform.hdmi5v, sizeof(_gH15A0HDMIPhyInform.hdmi5v));

		//_HDMI_H15A0_Set_HPD_Out_A4P(_gH15A0HDMIPhyInform.hpd_out[0], _gH15A0HDMIPhyInform.hpd_out[1],_gH15A0HDMIPhyInform.hpd_out[2], _gH15A0HDMIPhyInform.hpd_out[3]);
	}

#ifdef H15_BRINGUP_TODO
	LINK_NUM_H15_RdFL(0, system_control_00);
	LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, _gH15A0HDMIPhyInform.prt_sel);
#endif

	LINK_NUM_H15_RdFL(0, phy_00);
	LINK_NUM_H15_Rd01(0, phy_00, phy_pdb, _gH15A0HDMIPhyInform.phy_pdb[0]);			//PHY PDB ON
	LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, _gH15A0HDMIPhyInform.phy_enable[0]);		//PHY Enable
	LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_rstn, _gH15A0HDMIPhyInform.phy_rstn[0]);			//PHY RESET
	LINK_NUM_H15_Rd01(0, phy_00, hpd_in, _gH15A0HDMIPhyInform.hpd_in[0]);			//PHY HPD IN

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, _gH15A0HDMIPhyInform.phy_pdb[1]);			//PHY PDB ON
	LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, _gH15A0HDMIPhyInform.phy_enable[1]);		//PHY Enable
	LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_rstn, _gH15A0HDMIPhyInform.phy_rstn[1]);			//PHY RESET
	LINK_NUM_H15_Rd01(1, phy_00, hpd_in, _gH15A0HDMIPhyInform.hpd_in[1]);			//PHY HPD IN

	LINK_NUM_H15_Rd01(0, system_control_00, reg_scdt, _gH15A0HDMIPhyInform.scdt[0]);
	LINK_NUM_H15_Rd01(1, system_control_00, reg_scdt, _gH15A0HDMIPhyInform.scdt[1]);

	for (i=0;i<2;i++) {
		if(_gH15A0HDMIPhyInform.scdt[i] > 0)
			_gH15A0HDMIPhyInform.scdt_detect_count[i] ++;
	}
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		HDCP22_NUM_H15_RdFL(0, ad_hdcp22_reg_status);
		HDCP22_NUM_H15_Rd01(0, ad_hdcp22_reg_status, hdcp22_reg_status, _gH15A0HDMIPhyInform.hdcp22_status[0]);
		HDCP22_NUM_H15_RdFL(1, ad_hdcp22_reg_status);
		HDCP22_NUM_H15_Rd01(1, ad_hdcp22_reg_status, hdcp22_reg_status, _gH15A0HDMIPhyInform.hdcp22_status[1]);
	}

	LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode, _gH15A0HDMIPhyInform.hdmi_mode[0]);
	LINK_NUM_H15_Rd01(1, system_control_00, reg_hdmi_mode, _gH15A0HDMIPhyInform.hdmi_mode[1]);

	for (i=0;i<2;i++)
		access_phy[i] = _gH15A0HDMIPhyInform.phy_pdb[i] && _gH15A0HDMIPhyInform.phy_enable[i] && _gH15A0HDMIPhyInform.phy_rstn[i] ;

	_HDMI_H15A0_Get_HPD_Out_A4P(&_gH15A0HDMIPhyInform.hpd_out[0], &_gH15A0HDMIPhyInform.hpd_out[1]);
	_HDMI_H15A0_Get_HDMI5V_Info_A4P(&_gH15A0HDMIPhyInform.hdmi5v[0], &_gH15A0HDMIPhyInform.hdmi5v[1]);

	for (i=0;i<2;i++) {
		if(_gH15A0HDMIPhyInform.hdmi5v[i] > 0)
			_gH15A0HDMIPhyInform.loop_count[i] ++;
	}

	for (port_num=0;port_num<2;port_num++)
	{
		if (_gH15A0HDMIPhyInform.phy_pdb[port_num] && _gH15A0HDMIPhyInform.phy_rstn[port_num])
		{
			/* All Phy Disabled except port_num */
			if (port_num != 0)
				_HDMI_H15A0_EnablePort(0, 0);
			if (port_num != 1)
				_HDMI_H15A0_EnablePort(1, 0);

			/* Enable port_num */
			_HDMI_H15A0_EnablePort(port_num, 1);

			_gH15A0HDMIPhyInform.tmds_clock[port_num] = _HDMI_H15_Get_Clock_Freq();

			if ( _gH15A0HDMIPhyInform.hpd_enable && (_gH15A0HDMIPhyInform.tmds_clock[port_num] == 0) && (_gH15A0HDMIPhyInform.hdmi5v[port_num] ))	// hdmi 5v , not no tmds clock
			{
				int cr_done, cr_lock;

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(cr_lock_done);
					PHY_REG_H15B0_Rd01(cr_lock_done, cr_lock, cr_lock);
					PHY_REG_H15B0_Rd01(cr_lock_done, cr_done, cr_done);
				}
				else {
					PHY_REG_H15A0_RdFL(cr_lock_done);
					PHY_REG_H15A0_Rd01(cr_lock_done, cr_lock, cr_lock);
					PHY_REG_H15A0_Rd01(cr_lock_done, cr_done, cr_done);
				}

			if (cr_lock && cr_done)	// no tmds clock, but CR lock done ????
			{
				tmds_clock_abnormal_count[port_num]++;
				if(tmds_clock_abnormal_count[port_num] == 10) {

					HDMI_NOTI("@@@ Port[%d] Disable EQ Calibration Mode : abnormal count [%d]\n", \
							port_num, tmds_clock_abnormal_count[port_num]);

					/* SS BD-P1600 workaround code */
					PHY_REG_H15A0_RdFL(eq_cal_mode);
					PHY_REG_H15A0_Wr01(eq_cal_mode,eq_cal_mode,0x0);
					PHY_REG_H15A0_WrFL(eq_cal_mode);
				}
			}
			else
				tmds_clock_abnormal_count[port_num] = 0;

				if (tmds_clock_abnormal_count[port_num] > 50)	//clock abnormal for long time
				{
					HDMI_NOTI("#########################################\n");
					HDMI_NOTI("#### CR LOCK DONE, but NO TMDS CLOCK ####\n");
					HDMI_NOTI("#### HPD out off/on Port [%d] ############\n", port_num);
					HDMI_NOTI("#########################################\n");

					_HDMI_H15A0_Set_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
					if(port_num == 0)
						_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
					_HDMI_H15A0_Phy_Reset(port_num);

					_g_Disable_EQ_Offset_Cal[port_num] = 1;

#ifdef USE_EXT_MHL_IC
					if(port_num != EXT_MHL_IC_LINK_NUM)
#endif
						_gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num] = 0;

					OS_MsecSleep(100);
					/*
					   PHY_REG_H15A0_RdFL(resetb_pdb_all);
					   PHY_REG_H15A0_Wr01(resetb_pdb_all,resetb_all,0x0);		//active low reset
					   PHY_REG_H15A0_WrFL(resetb_pdb_all);

					   OS_MsecSleep(10);

					   PHY_REG_H15A0_RdFL(resetb_pdb_all);
					   PHY_REG_H15A0_Wr01(resetb_pdb_all,resetb_all,0x1);		//active low reset
					   PHY_REG_H15A0_WrFL(resetb_pdb_all);
					   */

					tmds_clock_abnormal_count[port_num]= 0;
				}
			}
			else
				tmds_clock_abnormal_count[port_num] = 0;

			if( _g_abnormal_3d_format[port_num] > 0)
			{
				HDMI_NOTI("#########################################\n");
				HDMI_NOTI("#### ABNORMAL 3D Format Detected     ####\n");
				HDMI_NOTI("#### HPD out off/on Port [%d] ############\n", port_num);
				HDMI_NOTI("#########################################\n");

				_HDMI_H15A0_Set_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
				if(port_num == 0)
					_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD

#ifdef USE_EXT_MHL_IC
					if(port_num != EXT_MHL_IC_LINK_NUM)
#endif
						_gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num] = 0;

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
					PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
					PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);
				}
				else {
					PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
					PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
					PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);
				}

				OS_MsecSleep(10);	// ms delay

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
					PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
					PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);
				}
				else {
					PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
					PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
					PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);
				}

				OS_MsecSleep(100);
				/*
				   PHY_REG_H15A0_RdFL(resetb_pdb_all);
				   PHY_REG_H15A0_Wr01(resetb_pdb_all,resetb_all,0x0);		//active low reset
				   PHY_REG_H15A0_WrFL(resetb_pdb_all);

				   OS_MsecSleep(10);

				   PHY_REG_H15A0_RdFL(resetb_pdb_all);
				   PHY_REG_H15A0_Wr01(resetb_pdb_all,resetb_all,0x1);		//active low reset
				   PHY_REG_H15A0_WrFL(resetb_pdb_all);
				 */

				_g_abnormal_3d_format[port_num] = 0;
			}


			/* if port_num is not current selected port , disable port */
			if (port_num != _gH15A0HDMIPhyInform.prt_sel)
				_HDMI_H15A0_EnablePort(port_num, 0);

			/* Enable selected port */
			_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);
		}
		else
			_gH15A0HDMIPhyInform.tmds_clock[port_num] = 0;
	}

	{
		LINK_NUM_H15_Rd01(0, system_control_00, reg_scdt, scdt[0]);
		LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode, mode[0]);
		if ( prev_hdmi_mode_prt0 != mode[0])
		{
			HDMI_DEBUG("#### HDMI_Mode : Port [%d] Mode Changed [%d]=>[%d] \n",\
					0, prev_hdmi_mode_prt0, mode[0] );
			prev_hdmi_mode_prt0 = mode[0];
		}
	}
	{
		LINK_NUM_H15_Rd01(1, system_control_00, reg_scdt, scdt[1]);
		LINK_NUM_H15_Rd01(1, system_control_00, reg_hdmi_mode, mode[1]);
		if ( prev_hdmi_mode_prt1 != mode[1])
		{
			HDMI_DEBUG("#### HDMI_Mode : Port [%d] Mode Changed [%d]=>[%d] \n",\
					1, prev_hdmi_mode_prt1, mode[1]);
			prev_hdmi_mode_prt1 = mode[1];
		}
	}
	//LINK_NUM_H15_Rd01(0, system_control_00, reg_scdt, scdt);
	//LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode, mode);

	for (port_num=0;port_num<2;port_num++)
	{
		/* Enabled SW Adaptive EQ for MHL */
		/*
		   if ( (port_num ==3) && ( _gH15A0HDMIPhyInform.cd_sense) )	// for MHL
		   break;
		 */

		if ( _SCDT_Fall_Detected[port_num] )
		{
			//_HDMI_H15A0_HDCP_ResetPort(port_num);
			_g_tcs_min_max_zero_count[port_num] = 0;
			_gH15A0HDMIPhyInform.scdt_fall_count[port_num] ++;

			/* Polling SCDT sometimes miss detection of short SDCT fall */
			if( _gH15A0HDMIPhyInform.prt_sel == port_num)
				scdt[port_num] = 0;

			_gH15A0HDMIPhyInform.tcs_done[port_num] = 0;
			_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;
			//_g_TCS_EN_restart[port_num] = 1;
			_SCDT_Fall_Detected[port_num] = 0;
			_g_ecc_err_authed[port_num] = 0;

			if (_gH15A0HDMIPhyInform.phy_pdb[port_num] && _gH15A0HDMIPhyInform.phy_rstn[port_num])
			{
				/* All Phy Disabled except port_num */
				if (port_num != 0)
					_HDMI_H15A0_EnablePort(0, 0);
				if (port_num != 1)
					_HDMI_H15A0_EnablePort(1, 0);

				/* Enable port_num */
				_HDMI_H15A0_EnablePort(port_num, 1);

		//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
		//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
		//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(tcs_boundary);
					PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
					PHY_REG_H15B0_WrFL(tcs_boundary);

					PHY_REG_H15B0_RdFL(tcs_additional_value);
					PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
					PHY_REG_H15B0_WrFL(tcs_additional_value);
				}
				else {
					PHY_REG_H15A0_RdFL(tcs_boundary);
					PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
					PHY_REG_H15A0_WrFL(tcs_boundary);

					PHY_REG_H15A0_RdFL(tcs_additional_value);
					PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
					PHY_REG_H15A0_WrFL(tcs_additional_value);
				}

				HDMI_NOTI("---- HDMI_PHY : port [%d] Sync Lost \n",port_num );

				/* if port_num is not current selected port , disable port */
				if (port_num != _gH15A0HDMIPhyInform.prt_sel)
					_HDMI_H15A0_EnablePort(port_num, 0);

				/* Enable selected port */
				_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);
			}
		}

		if ( _gH15A0HDMIPhyInform.hdmi5v[port_num] /* && (_gH15A0HDMIPhyInform.tcs_done[port_num] == 0) */\
				&& _gH15A0HDMIPhyInform.phy_pdb[port_num] && _gH15A0HDMIPhyInform.phy_rstn[port_num])		// port detected but TCS not done yet
		{
			int tcs_done_value;
			int tcs_min_value[3] = {0,};
			int tcs_max_value[3] = {0xff, 0xff, 0xff};

			//HDMI_DEBUG("---- HDMI_PHY : port [%d] TCS not done\n",port_num );

			/* All Phy Disabled except port_num */
			if (port_num != 0)
				_HDMI_H15A0_EnablePort(0, 0);
			if (port_num != 1)
				_HDMI_H15A0_EnablePort(1, 0);

			/* Enable port_num */
			_HDMI_H15A0_EnablePort(port_num, 1);

			/* Restart TCS when TMDS Frequency is changes more than 100KHz : 140409 */

			_Curr_TMDS_Clock[port_num] = _HDMI_H15_Get_Clock_Freq();

			if( abs( _Curr_TMDS_Clock[port_num] - _Prev_TMDS_Clock[port_num]) > 10 ) //100kHz
			{
				int cr_done;

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(cr_lock_done);
					PHY_REG_H15B0_Rd01(cr_lock_done, cr_done, cr_done);
				}
				else {
					PHY_REG_H15A0_RdFL(cr_lock_done);
					PHY_REG_H15A0_Rd01(cr_lock_done, cr_done, cr_done);
				}

				if(cr_done)
					HDMI_NOTI("---- HDMI_PHY port [%d] : Clock Changed [%d]=>[%d] \n", port_num, _Prev_TMDS_Clock[port_num], _Curr_TMDS_Clock[port_num] );
				else
					HDMI_DEBUG("---- HDMI_PHY port [%d] : Clock Changed [%d]=>[%d] \n", port_num, _Prev_TMDS_Clock[port_num], _Curr_TMDS_Clock[port_num] );

				_Prev_TMDS_Clock[port_num] =  _Curr_TMDS_Clock[port_num];

				_Stable_TMDS_Count[port_num] = 0;

				if (port_num == _gH15A0HDMIPhyInform.prt_sel)
					_gH15A0_tcs_fail_restart_count[port_num] = 0;
				/*
				   else if( (_Stable_TMDS_Count[port_num] == 5) && ( _Curr_TMDS_Clock[port_num] > 0) )
				   HDMI_NOTI("---- HDMI_PHY Stable TMDS Count [%d] \n", _Stable_TMDS_Count[port_num] );
				   _Stable_TMDS_Count[port_num] ++;

				//		if (_gH15A0HDMIPhyInform.tcs_done[port_num] == 1)
				 */
				if( _Curr_TMDS_Clock[port_num] > 0 && _gH15A0HDMIPhyInform.hdmi5v[port_num])
				{
					HDMI_DEBUG("---- HDMI_PHY port [%d] : Clock Changed, Restart TCS \n", port_num );
			//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
			//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
			//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

					if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
						/*
						PHY_REG_H15B0_RdFL(bert_tmds_sel);
						PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
						PHY_REG_H15B0_WrFL(bert_tmds_sel);

						PHY_REG_H15B0_RdFL(bert_tmds_sel);
						PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
						PHY_REG_H15B0_WrFL(bert_tmds_sel);
						*/
					}
					else {
						PHY_REG_H15A0_RdFL(bert_tmds_sel);
						PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x0);
						PHY_REG_H15A0_WrFL(bert_tmds_sel);

						PHY_REG_H15A0_RdFL(bert_tmds_sel);
						PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x1);
						PHY_REG_H15A0_WrFL(bert_tmds_sel);
					}

					_gH15A0HDMIPhyInform.tcs_done[port_num] = 0;
					_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;

					_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
					_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;

					_g_ecc_err_authed[port_num] = 0;
				}
			}
			else if ( _gH15A0HDMIPhyInform.tcs_done[port_num] == 0)
			{

				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
					PHY_REG_H15B0_RdFL(tcs_done);
					PHY_REG_H15B0_Rd01(tcs_done, tcs_done, tcs_done_value);
				}
				else {
					PHY_REG_H15A0_RdFL(tcs_done);
					PHY_REG_H15A0_Rd01(tcs_done, tcs_done, tcs_done_value);
				}

				if (tcs_done_value)
				{
					/* ???
					PHY_REG_H15A0_RdFL(tcs_additional_value);
					PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
					PHY_REG_H15A0_WrFL(tcs_additional_value);
					*/

					if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
						PHY_REG_H15B0_RdFL(tcs_min_ch0);
						PHY_REG_H15B0_Rd01(tcs_min_ch0, tcs_min_ch0, tcs_min_value[0]);

						PHY_REG_H15B0_RdFL(tcs_min_ch1);
						PHY_REG_H15B0_Rd01(tcs_min_ch1, tcs_min_ch1, tcs_min_value[1]);

						PHY_REG_H15B0_RdFL(tcs_min_ch2);
						PHY_REG_H15B0_Rd01(tcs_min_ch2, tcs_min_ch2, tcs_min_value[2]);

						PHY_REG_H15B0_RdFL(tcs_max_ch0);
						PHY_REG_H15B0_Rd01(tcs_max_ch0, tcs_max_ch0, tcs_max_value[0]);

						PHY_REG_H15B0_RdFL(tcs_max_ch1);
						PHY_REG_H15B0_Rd01(tcs_max_ch1, tcs_max_ch1, tcs_max_value[1]);

						PHY_REG_H15B0_RdFL(tcs_max_ch2);
						PHY_REG_H15B0_Rd01(tcs_max_ch2, tcs_max_ch2, tcs_max_value[2]);

						PHY_REG_H15B0_RdFL(tcs_ch0);
						PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[port_num][0]);

						PHY_REG_H15B0_RdFL(tcs_ch1);
						PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[port_num][1]);

						PHY_REG_H15B0_RdFL(tcs_ch2);
						PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[port_num][2]);

						PHY_REG_H15B0_RdFL(tcs_additional_value);
						PHY_REG_H15B0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
					}
					else {
						PHY_REG_H15A0_RdFL(tcs_min);
						PHY_REG_H15A0_Rd01(tcs_min, tcs_min, tcs_min_value[0]);

						PHY_REG_H15A0_RdFL(tcs_max);
						PHY_REG_H15A0_Rd01(tcs_max, tcs_max, tcs_max_value[0]);

						PHY_REG_H15A0_RdFL(tcs);
						PHY_REG_H15A0_Rd01(tcs, tcs, _HDMI_H15A0_TCS_result[port_num][0]);

						PHY_REG_H15A0_RdFL(tcs_additional_value);
						PHY_REG_H15A0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
					}

					//		if( (tcs_min_value == 0) && (tcs_max_value == 0) && (_g_tcs_min_max_zero_count[port_num] < 10 ) )
					if( ( ( abs(tcs_max_value[0] - tcs_min_value[0]) < 8 ) || ( abs(tcs_max_value[1] - tcs_min_value[1]) < 8 ) || ( abs(tcs_max_value[2] - tcs_min_value[2]) <8 ) ) && (_g_tcs_min_max_zero_count[port_num] < 10 ) )
					{
						_g_tcs_min_max_zero_count[port_num] ++;

						//HDMI_NOTI("---- HDMI_PHY : port [%d] TCS Min/Max all zero [%d] !!!!! : Mode_Sel RESET !!! \n",port_num, _g_tcs_min_max_zero_count[port_num] );
						HDMI_NOTI("---- HDMI_PHY : port [%d] TCS Min[%d/%d/%d] ~= Max[%d/%d/%d] count [%d] !!!!! : Mode_Sel RESET !!! \n"\
								,port_num, tcs_min_value[0], tcs_min_value[1], tcs_min_value[2]
								,tcs_max_value[0], tcs_max_value[1], tcs_max_value[2], _g_tcs_min_max_zero_count[port_num] );

						{
							HDMI_NOTI("---- HDMI_PHY port [%d] : TCS Failed Restart TCS \n", port_num );
					//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
					//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
					//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

							if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
								PHY_REG_H15B0_RdFL(tcs_boundary);
								PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
								PHY_REG_H15B0_WrFL(tcs_boundary);

								PHY_REG_H15B0_RdFL(tcs_additional_value);
								PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
								PHY_REG_H15B0_WrFL(tcs_additional_value);

								PHY_REG_H15B0_RdFL(bert_tmds_sel);
								PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
								PHY_REG_H15B0_WrFL(bert_tmds_sel);

								PHY_REG_H15B0_RdFL(bert_tmds_sel);
								PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
								PHY_REG_H15B0_WrFL(bert_tmds_sel);
							}
							else {
								PHY_REG_H15A0_RdFL(tcs_boundary);
								PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
								PHY_REG_H15A0_WrFL(tcs_boundary);

								PHY_REG_H15A0_RdFL(tcs_additional_value);
								PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
								PHY_REG_H15A0_WrFL(tcs_additional_value);

								PHY_REG_H15A0_RdFL(bert_tmds_sel);
								PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x0);
								PHY_REG_H15A0_WrFL(bert_tmds_sel);

								PHY_REG_H15A0_RdFL(bert_tmds_sel);
								PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x1);
								PHY_REG_H15A0_WrFL(bert_tmds_sel);

							}

							_gH15A0HDMIPhyInform.tcs_done[port_num] = 0;
							_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;

							_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
							_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;

							_g_ecc_err_authed[port_num] = 0;
						}
#if 0	//M14B0
						/* Continouse TMDS Error After eq_cs_man setting !!! : 15M Cable */
						PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
						PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
						PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

						OS_MsecSleep(10);	// ms delay

						PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
						PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
						PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);
#endif

						OS_MsecSleep(30);	// ms delay
					}
					else
					{
						_g_tcs_min_max_zero_count[port_num] = 0;

						//_gH15A0CHG_AVI_count_EQ = 0;
						if( gHDMI_Phy_Control.sw_eq_algorithm_workaround)
						{
							_gH15A0_TMDS_ERROR_EQ[port_num] = 0;
							_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;
						}

						//				PHY_REG_H15A0_RdFL(reg_tcs_man_mea_1);
						//				PHY_REG_H15A0_Wr01(reg_tcs_man_mea_1, reg_tcs_man_mea_1, 0x0);
						//				PHY_REG_H15A0_WrFL(reg_tcs_man_mea_1);

#if 0	// CS Auto Mode

						PHY_REG_H15A0_RdFL(eq_cs_man);
						if (tcs_min_value + 10 < 0x1f)		// for 15m cable
						{
							if(tcs_min_value == 0)
								PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man,tcs_min_value + 12);
							else if(tcs_min_value == 4)		// 140409 : BD370V-N 1080P60Hz noise (to set cs value to 0x10(from 0xe))
								PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man,tcs_min_value + 12);
							else
								PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man,tcs_min_value + 10);
						}
						else
							PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man, 0x1f);

						PHY_REG_H15A0_Rd01(eq_cs_man, eq_cs_man, _EQ_cs_man_value[port_num]);
						PHY_REG_H15A0_WrFL(eq_cs_man);
						PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
						PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x3);
						PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

						PHY_REG_H15A0_RdFL(reset_pdb_sel);
						PHY_REG_H15A0_Wr01(reset_pdb_sel, resetb_sel,0x1);	// Reset Manual Control
						PHY_REG_H15A0_WrFL(reset_pdb_sel);

						PHY_REG_H15A0_RdFL(bert_dtb_resetb);
						PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x0);	// Data Retimer/BERT/TMDS Decoder reset
						PHY_REG_H15A0_WrFL(bert_dtb_resetb);

						//OS_MsecSleep(5);	// ms delay

						PHY_REG_H15A0_RdFL(bert_dtb_resetb);
						PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x1);	// Data Retimer/BERT/TMDS Decoder reset
						PHY_REG_H15A0_WrFL(bert_dtb_resetb);

						PHY_REG_H15A0_RdFL(reset_pdb_sel);
						PHY_REG_H15A0_Wr01(reset_pdb_sel, resetb_sel,0x0);	// Reset Manual Control
						PHY_REG_H15A0_WrFL(reset_pdb_sel);

#endif
						//_HDMI_H15A0_Enable_TMDS_Error_Interrupt();
						_TCS_Done_Time[port_num] = jiffies_to_msecs(jiffies);

						_SCDT_Fall_Detected[port_num] = 0;	// clear SCDT interrupt

#ifdef H15_BRINGUP_TODO
						_HDMI_H15A0_TMDS_ResetPort_Control(port_num ,1 );
#endif

						HDMI_DEBUG("---- HDMI_PHY : port [%d] TCS Done [%d]=>[%d]\n",port_num, _gH15A0HDMIPhyInform.tcs_done[port_num], tcs_done_value );
						HDMI_NOTI("---- HDMI_PHY : port [%d] TCS Min/Max [0x%x/0x%x]  [0x%x/0x%x]  [0x%x/0x%x] \n",port_num,\
								tcs_min_value[0] , tcs_max_value[0], tcs_min_value[1] , tcs_max_value[1], tcs_min_value[2] , tcs_max_value[2]);

						if( ( (tcs_max_value[0] < 0x14) && (tcs_max_value[1] < 0x14) && (tcs_max_value[2] < 0x14) ) \
								&& ( (tcs_min_value[0] < 0x4) && (tcs_min_value[1] < 0x4) && (tcs_min_value[2] < 0x4) ) )
						{
							_HDMI_H15A0_TCS_add[port_num] = 0x6;
							HDMI_NOTI("---- HDMI_PHY : port [%d] May be Portable Tester ??? : Add value to [0x%x] \n",port_num, _HDMI_H15A0_TCS_add[port_num] );

							if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
							{
								PHY_REG_H15B0_RdFL(tcs_additional_value);
								PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
								PHY_REG_H15B0_WrFL(tcs_additional_value);

								PHY_REG_H15B0_RdFL(tcs_ch0);
								PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[port_num][0]);

								PHY_REG_H15B0_RdFL(tcs_ch1);
								PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[port_num][1]);

								PHY_REG_H15B0_RdFL(tcs_ch2);
								PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[port_num][2]);

							}

						}
						else if( (tcs_min_value[0] < 0x4) && (tcs_min_value[1] < 0x4) && (tcs_min_value[2] < 0x4)
								&& (_Curr_TMDS_Clock[port_num] > 20000)
								&& (gHDMI_Phy_Control.tcs_add_more_workaround > 0) )
						{
							_HDMI_H15A0_TCS_add[port_num] = 0xC;
							HDMI_NOTI("!!!! HDMI_PHY : port [%d] TCS Min Low !!! Freq[%d] : Add value to [0x%x] \n"\
									, port_num, _Curr_TMDS_Clock[port_num], _HDMI_H15A0_TCS_add[port_num] );

							if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
							{
								PHY_REG_H15B0_RdFL(tcs_additional_value);
								PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
								PHY_REG_H15B0_WrFL(tcs_additional_value);

								PHY_REG_H15B0_RdFL(tcs_ch0);
								PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[port_num][0]);

								PHY_REG_H15B0_RdFL(tcs_ch1);
								PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[port_num][1]);

								PHY_REG_H15B0_RdFL(tcs_ch2);
								PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[port_num][2]);
							}
						}

						HDMI_NOTI("---- HDMI_PHY : TCS Result [0x%x] [0x%x] [0x%x] : Add value [0x%x] \n", _HDMI_H15A0_TCS_result[port_num][0] , _HDMI_H15A0_TCS_result[port_num][1] , _HDMI_H15A0_TCS_result[port_num][2], _HDMI_H15A0_TCS_add[port_num]);
						//		HDMI_DEBUG("---- HDMI_PHY : DTB reset port [%d] \n",port_num);
						_gH15A0HDMIPhyInform.tcs_done[port_num] = tcs_done_value;
						_gH15A0HDMIPhyInform.tcs_min[port_num][0] = tcs_min_value[0];
						_gH15A0HDMIPhyInform.tcs_min[port_num][1] = tcs_min_value[1];
						_gH15A0HDMIPhyInform.tcs_min[port_num][2] = tcs_min_value[2];
						_gH15A0HDMIPhyInform.tcs_max[port_num][0] = tcs_max_value[0];
						_gH15A0HDMIPhyInform.tcs_max[port_num][1] = tcs_max_value[1];
						_gH15A0HDMIPhyInform.tcs_max[port_num][2] = tcs_max_value[2];
						_gH15A0HDMIPhyInform.tcs_add[port_num] = _HDMI_H15A0_TCS_add[port_num];
						_gH15A0HDMIPhyInform.tcs_result[port_num][0] = _HDMI_H15A0_TCS_result[port_num][0];
						_gH15A0HDMIPhyInform.tcs_result[port_num][1] = _HDMI_H15A0_TCS_result[port_num][1];
						_gH15A0HDMIPhyInform.tcs_result[port_num][2] = _HDMI_H15A0_TCS_result[port_num][2];

						_g_ecc_err_authed[port_num] = 0;
					}
				}

			}

			/*
			   else if( (_Stable_TMDS_Count[port_num] < 5) && ( _Curr_TMDS_Clock[port_num] > 0) )
			   _Stable_TMDS_Count[port_num] ++;
			 */

			if( gHDMI_Phy_Control.cr_done_tcs_en_workaround)
			{
				if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
				{
					PHY_REG_H15B0_RdFL(cr_lock_done);
					PHY_REG_H15B0_Rd01(cr_lock_done, cr_lock, cr_lock[port_num]);
					PHY_REG_H15B0_Rd01(cr_lock_done, cr_done, cr_done[port_num]);

					if( prev_cr_done[port_num] != cr_done[port_num] )
					{
						if(cr_done[port_num] == 1)
						{
							if(_HDMI_H15A0_Phy_PDB_WA > 0)
							{
								int cr_pll_mode_value;

								PHY_REG_H15B0_RdFL(cr_pll_mode_offset);
								PHY_REG_H15B0_Rd01(cr_pll_mode_offset, cr_pll_mode, cr_pll_mode_value);

								if(cr_pll_mode_value <=1)
								{
									HDMI_NOTI("@@@ cr_pll_mode[%d] @@@\n", cr_pll_mode_value);
									PHY_REG_H15B0_RdFL(cr_icp_adj);
									PHY_REG_H15B0_Wr01(cr_icp_adj,cr_icp_adj,0x4);
									PHY_REG_H15B0_WrFL(cr_icp_adj);
								}
							}

							HDMI_NOTI("@@@ HDMI PHY [%d] : CR done '1' : Restart TCS !!! @@@\n", port_num);

							//	OS_MsecSleep(10);

							PHY_REG_H15B0_RdFL(bert_tmds_sel);
							PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
							PHY_REG_H15B0_WrFL(bert_tmds_sel);

							OS_MsecSleep(gHDMI_Phy_Control.tcs_en_delay_after_cr_done);

							PHY_REG_H15B0_RdFL(bert_tmds_sel);
							PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
							PHY_REG_H15B0_WrFL(bert_tmds_sel);

							_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;
						}
						prev_cr_done[port_num] = cr_done[port_num];
					}
				}
			}

		}

		/* if port_num is not current selected port , disable port */
		if (port_num != _gH15A0HDMIPhyInform.prt_sel)
			_HDMI_H15A0_EnablePort(port_num, 0);

		/* Enable selected port */
		_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

	}

#ifndef USE_EXT_MHL_IC
	if (_gH15A0_HDMI_MHLAutoModeCnt > 0)
	{
		_gH15A0_HDMI_MHLAutoModeCnt--;
		if ( _gH15A0_HDMI_MHLAutoModeCnt == 0)
		{
			_HDMI_H15A0_Set_ManMHLMode(1, 0, 0);
			HDMI_DEBUG("---- HDMI_PHY : set MHL auto mode\n");
		}
	}
#endif

	for (port_num = 0; port_num < 2;port_num ++)
	{
		_HDMI_H15A0_Get_EDID_Rd(port_num , &_gH15A0HDMIPhyInform.edid_rd_done[port_num], &temp);
		_HDMI_H15A0_Get_HDCP_info(port_num , &_gH15A0HDMIPhyInform.hdcp_authed[port_num], &_gH15A0HDMIPhyInform.hdcp_enc_en[port_num]);
	}

	/*
	if (_gH15A0HDMIPhyInform.hdcp_authed[_gH15A0HDMIPhyInform.prt_sel] && _gH15A0HDMIPhyInform.hdcp_enc_en[_gH15A0HDMIPhyInform.prt_sel] )
		hdcp_status = HDMI_HDCP_AUTH_DONE;
	else
	{
		HDMI_H15A0_Get_Aksv_Data(_gH15A0HDMIPhyInform.prt_sel, aksv_data);
		if ( (memcmp(aksv_data, (void*){0,}, sizeof(aksv_data)) ) && _gH15A0HDMIPhyInform.hdcp_authed[_gH15A0HDMIPhyInform.prt_sel])
			memcmp
	}
	*/
	current_time_eq = jiffies_to_msecs(jiffies);

	for (port_num = 0; port_num < 2;port_num ++)
	{
		elasped_time_eq[port_num] =  current_time_eq - _TCS_Done_Time[port_num];

		if ( (elasped_time_eq[port_num] > 3000) && ( ( _gH15A0_TMDS_ERROR_EQ[port_num] != -1) || ( _gH15A0_TMDS_ERROR_EQ_2nd[port_num] != -1) ) )
		{
			//		_HDMI_H15A0_Disable_TMDS_Error_Interrupt();
			HDMI_DEBUG("---- HDMI_PHY Port[%d] Stable??? for [%d]msec : Disable ECC ERROR check\n", port_num, elasped_time_eq[port_num]);
			HDMI_DEBUG("---- 1st EQ ERROR[%d], 2nd EQ ERROR[%d]", _gH15A0_TMDS_ERROR_EQ[port_num], _gH15A0_TMDS_ERROR_EQ_2nd[port_num]);
			_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
			_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;

		}
	}

//	_HDMI_H15A0_Enable_TMDS_Error_Interrupt();
	for (port_num = 0; port_num < 2;port_num ++)
	{
	//	_HDMI_H15A0_Enable_TMDS_Error_Interrupt(port_num);
	//	_HDMI_H15A0_Enable_TERC4_Error_Interrupt(port_num);
	//	_HDMI_H15A0_Enable_HDCP_Error_Interrupt(port_num);

		if(_gH15A0_TMDS_ERROR_intr_count[port_num] > 0)
		{
			_gH15A0_TMDS_ERROR_intr_count[port_num] = 0;
			_gH15A0_TMDS_ERROR_count[port_num] ++;
			_gH15A0HDMIPhyInform.ecc_error_count[port_num] ++;

			if( (_gH15A0HDMIPhyInform.tcs_done[port_num] == 1) \
					&& ( (_HDMI_H15A0_TCS_add[port_num] == _HDMI_H15A0_TCS_addtional_value_3rd) || (gHDMI_Phy_Control.sw_eq_algorithm_workaround == 0) )\
					&& (_gH15A0_tcs_fail_restart_count[port_num] < 3) )
				_gH15A0_TMDS_ERROR_count_tcs_fail[port_num]++;
			else
				_gH15A0_TMDS_ERROR_count_tcs_fail[port_num] = 0;

			if(_gH15A0_TMDS_ERROR_count[port_num] > 3)
			{
				HDMI_NOTI("!!! ECC Errors [%d] count[%d]: force scdt to zero, Port[%d] , TCS[0x%x/0x%x/0x%x] Add[0x%x] !!!\n", _gH15A0_TMDS_ERROR_count[port_num], \
						_gH15A0_TMDS_ERROR_count_tcs_fail[port_num], port_num,\
						_HDMI_H15A0_TCS_result[port_num][0], _HDMI_H15A0_TCS_result[port_num][1], _HDMI_H15A0_TCS_result[port_num][2] ,_HDMI_H15A0_TCS_add[port_num] );
				_gH15A0_TMDS_ERROR_count[port_num] = 0;
				scdt[port_num] = 0;

				if(_gH15A0_TMDS_ERROR_count_tcs_fail[port_num] == 52) // Workaround code for 3D Camcoder in QA (TMDS error for 1-sec)
				{
					if( (gHDMI_Phy_Control.hdcp14_ovr_workaround > 0)
							&& (port_num != EXT_MHL_IC_LINK_NUM) \
							&& ( (_gH15A0HDMIPhyInform.hdcp22_status[port_num] & 0x1) == 0) \
							&& (_HDMI_H15A0_Check_Aksv_Exist(port_num) == 0 ) \
							&& (_gH15A0HDMIPhyInform.hdcp_authed[port_num] == 0) && (_gH15A0HDMIPhyInform.hdcp_enc_en[port_num] == 0) )
					{
						HDCP22_NUM_H15_RdFL(port_num, ad_hdcp22_control);
						if( ( HDCP22_NUM_H15_Rd(port_num, ad_hdcp22_control) & 0x2) != 0x2)
						{
							HDMI_NOTI("@@@@ HDMI : Port [%d] : Continouse ECC Error [%d] \n",port_num , _gH15A0_TMDS_ERROR_count_tcs_fail[port_num] );
							HDMI_NOTI("@@@@ HDMI : HDCP Not Authed/Encoded, : hdcp22_status[0x%x]  \n", _gH15A0HDMIPhyInform.hdcp22_status[port_num] );
							HDMI_NOTI("@@@@ HDMI : HDCP22 Control Override Enable to 1.4 !!!!! \n");

							HDCP22_NUM_H15_RdFL(port_num, ad_hdcp22_control);
							HDCP22_NUM_H15_Wr(port_num, ad_hdcp22_control, 0x410002);
							HDCP22_NUM_H15_WrFL(port_num, ad_hdcp22_control);
						}
					}
				}
				else if(_gH15A0_TMDS_ERROR_count_tcs_fail[port_num] > 200) // TCS Failed (TMDS error for 4-sec and tcs value == 4)
				{
					HDMI_NOTI("!!!!! TCS Failed, Port[%d] : count [%d] ,TCS[0x%x/0x%x/0x%x], restart_count[%d] !!!!!\n"
							, port_num, _gH15A0_TMDS_ERROR_count_tcs_fail[port_num]\
							, _HDMI_H15A0_TCS_result[port_num][0], _HDMI_H15A0_TCS_result[port_num][1], _HDMI_H15A0_TCS_result[port_num][2] \
							,_gH15A0_tcs_fail_restart_count[port_num]);
				//	HDMI_NOTI("!!!!! Restart TCS  !!!!!\n");

					_gH15A0_tcs_fail_restart_count[port_num]++;
					_gH15A0_TMDS_ERROR_count_tcs_fail[port_num] = 0;

					/* All Phy Disabled except port_num */
					if (port_num != 0)
						_HDMI_H15A0_EnablePort(0, 0);
					if (port_num != 1)
						_HDMI_H15A0_EnablePort(1, 0);

					/* Enable port_num */
					_HDMI_H15A0_EnablePort(port_num, 1);

					{
						HDMI_NOTI("---- HDMI_PHY port [%d] : Restart TCS \n", port_num );
						//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
						//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
						//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);
						//
						if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
							PHY_REG_H15B0_RdFL(tcs_boundary);
							PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
							PHY_REG_H15B0_WrFL(tcs_boundary);

							PHY_REG_H15B0_RdFL(tcs_additional_value);
							PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
							PHY_REG_H15B0_WrFL(tcs_additional_value);

							if( gHDMI_Phy_Control.hpd_low_workaround)
							{
								if ( (_gH15A0HDMIPhyInform.hdcp_authed[port_num] == 0) \
										&& (_HDMI_H15A0_Check_Aksv_Exist(port_num) == 0 ) )
								{
									HDMI_NOTI("---- HDMI_PHY port [%d] : Not Authed and No AKSV => HPD out disable !!! \n", port_num );

									_HDMI_H15A0_Set_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
									if(port_num == 0)
										_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD

									OS_MsecSleep(100);
								}
							}

							PHY_REG_H15B0_RdFL(bert_tmds_sel);
							PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
							PHY_REG_H15B0_WrFL(bert_tmds_sel);

							PHY_REG_H15B0_RdFL(bert_tmds_sel);
							PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
							PHY_REG_H15B0_WrFL(bert_tmds_sel);
						}
						else {
							PHY_REG_H15A0_RdFL(tcs_boundary);
							PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
							PHY_REG_H15A0_WrFL(tcs_boundary);

							PHY_REG_H15A0_RdFL(tcs_additional_value);
							PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
							PHY_REG_H15A0_WrFL(tcs_additional_value);

							PHY_REG_H15A0_RdFL(bert_tmds_sel);
							PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x0);
							PHY_REG_H15A0_WrFL(bert_tmds_sel);

							PHY_REG_H15A0_RdFL(bert_tmds_sel);
							PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x1);
							PHY_REG_H15A0_WrFL(bert_tmds_sel);
						}

						_gH15A0HDMIPhyInform.tcs_done[port_num] = 0;
						_gH15A0HDMIPhyInform.tcs_restart_count[port_num] ++;

						_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
						_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;
					}
					/* if port_num is not current selected port , disable port */
					if (port_num != _gH15A0HDMIPhyInform.prt_sel)
						_HDMI_H15A0_EnablePort(port_num, 0);

					/* Enable selected port */
					_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);
				}
			}

		}
		else
		{
			_gH15A0_TMDS_ERROR_count[port_num] = 0;
			_gH15A0_TMDS_ERROR_count_tcs_fail[port_num] = 0;
		}

		if(_gH15A0_HDCP_ERROR_intr_count[port_num] > 0)
		{
			_gH15A0_HDCP_ERROR_intr_count[port_num] = 0;
			_gH15A0_HDCP_ERROR_count[port_num] ++;
			_gH15A0HDMIPhyInform.hdcp_error_count[port_num] ++;

			if(_gH15A0_HDCP_ERROR_count[port_num] > 3)
			{
				HDMI_NOTI("!!! HDCP Errors [%d] : Port[%d] !!!\n", _gH15A0_HDCP_ERROR_count[port_num], port_num );
				_gH15A0_HDCP_ERROR_count[port_num] = 0;
			}

		}
		else
		{
			_gH15A0_HDCP_ERROR_count[port_num] = 0;
		}

		if(_gH15A0_TERC4_ERROR_intr_count[port_num] > 0) {
			_gH15A0_TERC4_ERROR_intr_count[port_num] = 0;
			_gH15A0HDMIPhyInform.terc4_error_count[port_num] ++;
		}

	}


	/*
	if ( ( elasped_time_eq < 5000 ) && (_gH15A0_TMDS_ERROR_EQ != -1) )
		_HDMI_H15A0_Enable_TMDS_Error_Interrupt();
		*/

	for (port_num = 0; port_num < 2;port_num ++)
	{
		/* more than 8 ECC Error interrrupt occured during 1.8 second,
		   change EQ CS value to 0xf */
		//if ( (_gH15A0_TMDS_ERROR_EQ > 30) && ( elasped_time_eq < 3000 ) )
		//if ( (_gH15A0_TMDS_ERROR_EQ > 8) && ( elasped_time_eq < 1800 ) )
		if ( (_gH15A0_TMDS_ERROR_EQ[port_num] > 16) && ( elasped_time_eq[port_num] < 3000 ) )	// 140403
		{
			/* All Phy Disabled except port_num */
			if (port_num != 0)
				_HDMI_H15A0_EnablePort(0, 0);
			if (port_num != 1)
				_HDMI_H15A0_EnablePort(1, 0);

			/* Enable port_num */
			_HDMI_H15A0_EnablePort(port_num, 1);

			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(tcs_boundary);
				PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_2nd);
				PHY_REG_H15B0_WrFL(tcs_boundary);

				PHY_REG_H15B0_RdFL(tcs_additional_value);
				PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_2nd);
				PHY_REG_H15B0_WrFL(tcs_additional_value);

#ifdef H15_BRINGUP_TODO
				PHY_REG_H15B0_RdFL(eq_cs_man);
				//PHY_REG_H15B0_Wr01(eq_cs_man, eq_cs_man, 0xf);
				PHY_REG_H15B0_Wr01(eq_cs_man, eq_cs_man, 0x11);		// 140403 : changed cs_man value for BP540 and 15M Cable
				PHY_REG_H15B0_Rd01(eq_cs_man, eq_cs_man, _EQ_cs_man_value[_gH15A0HDMIPhyInform.prt_sel]);
				PHY_REG_H15B0_WrFL(eq_cs_man);

				/* Continouse TMDS Error After eq_cs_man setting !!! : 15M Cable */
				PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
				PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
				PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(30);	// ms delay

				_HDMI_H15A0_TMDS_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel ,1 );
#endif
				PHY_REG_H15B0_RdFL(tcs_ch0);
				PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[port_num][0]);

				PHY_REG_H15B0_RdFL(tcs_ch1);
				PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[port_num][1]);

				PHY_REG_H15B0_RdFL(tcs_ch2);
				PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[port_num][2]);

				PHY_REG_H15B0_RdFL(tcs_additional_value);
				PHY_REG_H15B0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
			}
			else {	PHY_REG_H15A0_RdFL(tcs_boundary);
				PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_2nd);
				PHY_REG_H15A0_WrFL(tcs_boundary);

				PHY_REG_H15A0_RdFL(tcs_additional_value);
				PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_2nd);
				PHY_REG_H15A0_WrFL(tcs_additional_value);

#ifdef H15_BRINGUP_TODO
				PHY_REG_H15A0_RdFL(eq_cs_man);
				//PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man, 0xf);
				PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man, 0x11);		// 140403 : changed cs_man value for BP540 and 15M Cable
				PHY_REG_H15A0_Rd01(eq_cs_man, eq_cs_man, _EQ_cs_man_value[_gH15A0HDMIPhyInform.prt_sel]);
				PHY_REG_H15A0_WrFL(eq_cs_man);

				/* Continouse TMDS Error After eq_cs_man setting !!! : 15M Cable */
				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(30);	// ms delay

				_HDMI_H15A0_TMDS_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel ,1 );
#endif
				PHY_REG_H15A0_RdFL(tcs);
				PHY_REG_H15A0_Rd01(tcs, tcs, _HDMI_H15A0_TCS_result[port_num][0]);

				PHY_REG_H15A0_RdFL(tcs_additional_value);
				PHY_REG_H15A0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
			}

			_gH15A0HDMIPhyInform.tcs_add[port_num] = _HDMI_H15A0_TCS_add[port_num];
			_gH15A0HDMIPhyInform.tcs_result[port_num][0] = _HDMI_H15A0_TCS_result[port_num][0];
			_gH15A0HDMIPhyInform.tcs_result[port_num][1] = _HDMI_H15A0_TCS_result[port_num][1];
			_gH15A0HDMIPhyInform.tcs_result[port_num][2] = _HDMI_H15A0_TCS_result[port_num][2];

			/* if port_num is not current selected port , disable port */
			if (port_num != _gH15A0HDMIPhyInform.prt_sel)
				_HDMI_H15A0_EnablePort(port_num, 0);

			/* Enable selected port */
			_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

			_SCDT_Fall_Detected[port_num] = 0;	// clear SCDT interrupt from above mode_sel reset

			/*
			   PHY_REG_H15A0_RdFL(reset_pdb_sel);
			   PHY_REG_H15A0_Wr01(reset_pdb_sel, reset_sel,0x1);	// Reset Manual Control
			   PHY_REG_H15A0_WrFL(reset_pdb_sel);

			   PHY_REG_H15A0_RdFL(bert_dtb_resetb);
			   PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x0);	// Data Retimer/BERT/TMDS Decoder reset
			   PHY_REG_H15A0_WrFL(bert_dtb_resetb);

			//OS_MsecSleep(5);	// ms delay

			PHY_REG_H15A0_RdFL(bert_dtb_resetb);
			PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x1);	// Data Retimer/BERT/TMDS Decoder reset
			PHY_REG_H15A0_WrFL(bert_dtb_resetb);

			PHY_REG_H15A0_RdFL(reset_pdb_sel);
			PHY_REG_H15A0_Wr01(reset_pdb_sel, reset_sel,0x0);	// Reset Manual Control
			PHY_REG_H15A0_WrFL(reset_pdb_sel);
			*/
			//		_HDMI_H15A0_Disable_TMDS_Error_Interrupt();
			HDMI_NOTI("---- PHY port[%d] : ECC Err intr [%d] in [%d]msec : TCS ADD [0x%x], count[%d]\n",port_num, _gH15A0_TMDS_ERROR_EQ[port_num], elasped_time_eq[port_num], _HDMI_H15A0_TCS_addtional_value_2nd, _gH15A0_tcs_fail_restart_count[port_num]);

			if( gHDMI_Phy_Control.sw_eq_algorithm_workaround)
			{
				_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
				_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = 0;
			}
			cs_man_changed_time_eq[port_num] = jiffies_to_msecs(jiffies);

			//		OS_MsecSleep(10);	// ms delay
			//		_SCDT_Fall_Detected[_gH15A0HDMIPhyInform.prt_sel] = 0;	// clear SCDT interrupt
		}

		if(_gH15A0_TMDS_ERROR_EQ_2nd[port_num] != -1)
		{
			HDMI_DEBUG("---- HDMI 2ND EQ Check [%d]\n", _gH15A0_TMDS_ERROR_EQ_2nd[port_num]);
			current_time_eq = jiffies_to_msecs(jiffies);
			elasped_2nd_time_eq[port_num] =  current_time_eq - cs_man_changed_time_eq[port_num];
		}

		/* more than 4 ECC Error interrrupt occured during .9 second,
		   change EQ CS value to 0x6 */
		//if ( (_gH15A0_TMDS_ERROR_EQ_2nd > 4) && ( elasped_2nd_time_eq < 900 ) )
		//if ( (_gH15A0_TMDS_ERROR_EQ_2nd > 8) && ( elasped_2nd_time_eq < 1800 ) )	//140404
		if ( (_gH15A0_TMDS_ERROR_EQ_2nd[port_num] > 16) && ( elasped_2nd_time_eq[port_num] < 3000 ) )	//140829
		{
//			int cs_man_value;

			/* All Phy Disabled except port_num */
			if (port_num != 0)
				_HDMI_H15A0_EnablePort(0, 0);
			if (port_num != 1)
				_HDMI_H15A0_EnablePort(1, 0);

			/* Enable port_num */
			_HDMI_H15A0_EnablePort(port_num, 1);

			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(tcs_boundary);
				PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_3rd);
				PHY_REG_H15B0_WrFL(tcs_boundary);

				PHY_REG_H15B0_RdFL(tcs_additional_value);
				PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_3rd);
				PHY_REG_H15B0_WrFL(tcs_additional_value);

#ifdef H15_BRINGUP_TODO
				/* Continouse TMDS Error After eq_cs_man setting !!! : 15M Cable */
				PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
				PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
				PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(30);	// ms delay

				_HDMI_H15A0_TMDS_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel ,1 );
#endif
				PHY_REG_H15B0_RdFL(tcs_ch0);
				PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[port_num][0]);

				PHY_REG_H15B0_RdFL(tcs_ch1);
				PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[port_num][1]);

				PHY_REG_H15B0_RdFL(tcs_ch2);
				PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[port_num][2]);

				PHY_REG_H15B0_RdFL(tcs_additional_value);
				PHY_REG_H15B0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
			}
			else {
				PHY_REG_H15A0_RdFL(tcs_boundary);
				PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_3rd);
				PHY_REG_H15A0_WrFL(tcs_boundary);

				PHY_REG_H15A0_RdFL(tcs_additional_value);
				PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_3rd);
				PHY_REG_H15A0_WrFL(tcs_additional_value);

#ifdef H15_BRINGUP_TODO
				/* Continouse TMDS Error After eq_cs_man setting !!! : 15M Cable */
				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(10);	// ms delay

				PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
				PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
				PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

				OS_MsecSleep(30);	// ms delay

				_HDMI_H15A0_TMDS_ResetPort_Control(_gH15A0HDMIPhyInform.prt_sel ,1 );
#endif
				PHY_REG_H15A0_RdFL(tcs);
				PHY_REG_H15A0_Rd01(tcs, tcs, _HDMI_H15A0_TCS_result[port_num][0]);

				PHY_REG_H15A0_RdFL(tcs_additional_value);
				PHY_REG_H15A0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[port_num]);
			}

			_gH15A0HDMIPhyInform.tcs_add[port_num] = _HDMI_H15A0_TCS_add[port_num];
			_gH15A0HDMIPhyInform.tcs_result[port_num][0] = _HDMI_H15A0_TCS_result[port_num][0];
			_gH15A0HDMIPhyInform.tcs_result[port_num][1] = _HDMI_H15A0_TCS_result[port_num][1];
			_gH15A0HDMIPhyInform.tcs_result[port_num][2] = _HDMI_H15A0_TCS_result[port_num][2];

			/* if port_num is not current selected port , disable port */
			if (port_num != _gH15A0HDMIPhyInform.prt_sel)
				_HDMI_H15A0_EnablePort(port_num, 0);

			/* Enable selected port */
			_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

			_SCDT_Fall_Detected[port_num] = 0;	// clear SCDT interrupt from above mode_sel reset

			/*
			   PHY_REG_H15A0_RdFL(reset_pdb_sel);
			   PHY_REG_H15A0_Wr01(reset_pdb_sel, reset_sel,0x1);	// Reset Manual Control
			   PHY_REG_H15A0_WrFL(reset_pdb_sel);

			   PHY_REG_H15A0_RdFL(bert_dtb_resetb);
			   PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x0);	// Data Retimer/BERT/TMDS Decoder reset
			   PHY_REG_H15A0_WrFL(bert_dtb_resetb);

			//OS_MsecSleep(5);	// ms delay

			PHY_REG_H15A0_RdFL(bert_dtb_resetb);
			PHY_REG_H15A0_Wr01(bert_dtb_resetb, dtb_resetb,0x1);	// Data Retimer/BERT/TMDS Decoder reset
			PHY_REG_H15A0_WrFL(bert_dtb_resetb);

			PHY_REG_H15A0_RdFL(reset_pdb_sel);
			PHY_REG_H15A0_Wr01(reset_pdb_sel, reset_sel,0x0);	// Reset Manual Control
			PHY_REG_H15A0_WrFL(reset_pdb_sel);
			*/
			//		_HDMI_H15A0_Disable_TMDS_Error_Interrupt();
			HDMI_NOTI("---- PHY port[%d] : ECC Err 2ND intr [%d] in [%d]msec : TCS Add value to [0x%x] ,count[%d]\n",port_num, _gH15A0_TMDS_ERROR_EQ_2nd[port_num], elasped_2nd_time_eq[port_num], _HDMI_H15A0_TCS_addtional_value_3rd , _gH15A0_tcs_fail_restart_count[port_num]);

			_gH15A0_TMDS_ERROR_EQ[port_num] = -1;
			_gH15A0_TMDS_ERROR_EQ_2nd[port_num] = -1;

			//		OS_MsecSleep(10);	// ms delay
			//		_SCDT_Fall_Detected[_gH15A0HDMIPhyInform.prt_sel] = 0;	// clear SCDT interrupt

		}

	}

#ifdef H15_BRINGUP_TODO
	/* more than 5 avi change interrupt occured during 5second,
	   change EQ CS value to 0xf */
	if ( (_gH15A0CHG_AVI_count_EQ > 5) && ( elasped_time_eq < 5000 ) )
	{
		/* Enable port_num */
		_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

		PHY_REG_H15A0_RdFL(eq_cs_man);
		PHY_REG_H15A0_Wr01(eq_cs_man, eq_cs_man, 0xf);
		PHY_REG_H15A0_Rd01(eq_cs_man, eq_cs_man, _EQ_cs_man_value[_gH15A0HDMIPhyInform.prt_sel]);
		PHY_REG_H15A0_WrFL(eq_cs_man);

		HDMI_NOTI("---- HDMI_PHY port[%d] : AVI_CHG intr [%d] in [%d]msec : EQ_CS_MAN to [%d]\n",_gH15A0HDMIPhyInform.prt_sel, _gH15A0CHG_AVI_count_EQ, elasped_time_eq, 0xf );

		_gH15A0CHG_AVI_count_EQ = -1;
	}
#endif

	for (port_num=0;port_num<2;port_num++)
	{
		if (scdt[port_num] == HDMI_PORT_CONNECTED)
		{
			if (portCnt[port_num] != 0)		HDMI_DEBUG("[%d] %s Port[%d] : Connection \n", __LINE__, __func__, port_num);
			portCnt[port_num] = 0;
		}
		else
		{
			mode[port_num] = 0;
			if (portCnt[port_num] < 3)
			{
				/*
				   spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				   sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INIT_STATE;
				   sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INIT_STATE;
				   sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INIT_STATE;
				   sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_CLEAR_STATE;
				   sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
				   sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
				   sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
				   sHDMI_H15A0_Status.AVIChangeState[link_num] = FALSE;
				   spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
				 */

				_HDMI_H15A0_SetInternalMute(port_num, muteCtrl);	// Video All Clear

#ifdef H15_BRINGUP_TODO
				///* Reset phy pll for pixel repetition  *///
				CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
				//CTOP_CTRL_H15A0_Rd01(CVI, ctr00, ppll_clk_sel, ctrl_repeat);
				CTOP_CTRL_H15A0_Rd01(CVI, ctr00, hdmi_half_sel, ctrl_repeat);

				if (ctrl_repeat)
				{
					CTOP_CTRL_H15A0_Wr01(CVI, ctr00, hdmi_half_sel, 0x0);
					CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
				}
#endif

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
				LINK_NUM_H15_RdFL(port_num, interrupt_enable_01);
#endif
				LINK_NUM_H15_Rd01(port_num, interrupt_enable_01, intr_no_vsi_enable, noVsi_Intra);			///< 20 intr_no_vsi_enable

				if (noVsi_Intra != 0)
				{
					LINK_NUM_H15_Wr01(port_num, interrupt_enable_01, intr_no_vsi_enable, 0x0);			///< 20 intr_no_vsi_enable
					LINK_NUM_H15_WrFL(port_num, interrupt_enable_01);
					HDMI_DEBUG("[%d] %s port[%d] : No VSI intra disable \n", __LINE__, __func__, port_num);
				}

				HDMI_DEBUG("[%d] %s port[%d] : Dis Connection  [%d] \n", __LINE__, __func__, port_num,  portCnt[port_num]);
				portCnt[port_num]++;
			}
		}

		// 130926 : wonsik.do spin lock needed ???
		//	spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.HDMIConnectState[port_num] = scdt[port_num];
		//	gH15BootData->mode = mode[link_num];
		sHDMI_H15A0_Status.hdmi_mode[port_num] = mode[port_num];
		//	spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
	}
	for (port_num = 0; port_num < 2;port_num ++)
	{
		_HDMI_H15A0_Enable_TMDS_Error_Interrupt(port_num);
		_HDMI_H15A0_Enable_TERC4_Error_Interrupt(port_num);
		_HDMI_H15A0_Enable_HDCP_Error_Interrupt(port_num);
	}

	return RET_OK;
}

static int _HDMI_H15A0_PhyRunReset(void)
{
	//int prt_selected;
	int phy_enable_prt1, cd_sense;
	int phy_enabled_prt;

	HDMI_DEBUG("[%s] Entered \n",__func__);

	/* H15A0 New HDMI Phy Register Settings */
   PHY_REG_H15A0_RdFL(fifo_mode_muxed_clk_sel);
//   PHY_REG_H15A0_Wr01(fifo_mode_muxed_clk_sel,fifo_mode,0x1);
   PHY_REG_H15A0_Wr01(fifo_mode_muxed_clk_sel,fifo_mode,0x2);	// invalid timing when silicon image MHL3.0 timing 371
   PHY_REG_H15A0_WrFL(fifo_mode_muxed_clk_sel);

   PHY_REG_H15A0_RdFL(con_sel_man);
   PHY_REG_H15A0_Wr01(con_sel_man,con_sel_man_val,0x1);
   PHY_REG_H15A0_Wr01(con_sel_man,con_sel_man,0x1);
   PHY_REG_H15A0_WrFL(con_sel_man);

   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);

   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x1);
   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);

   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);

   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x1);
   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);

   //140829 : added by shyang
	PHY_REG_H15A0_RdFL(odt_ctrl);
	PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x1);
	PHY_REG_H15A0_WrFL(odt_ctrl);

	PHY_REG_H15A0_RdFL(odt_ctrl);
	PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x2);
	PHY_REG_H15A0_WrFL(odt_ctrl);

	PHY_REG_H15A0_RdFL(odt_ctrl);
	PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x1);
	PHY_REG_H15A0_WrFL(odt_ctrl);

	PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
	PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
	PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

	/* H15A0 New HDMI Phy Register Settings */

	PHY_REG_H15A0_RdFL(tcs_method_err_sel);
	PHY_REG_H15A0_Wr01(tcs_method_err_sel, tcs_divide_method, 0x2);	//default : 0x2
	PHY_REG_H15A0_WrFL(tcs_method_err_sel);

	PHY_REG_H15A0_RdFL(tcs_boundary);
	PHY_REG_H15A0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
	PHY_REG_H15A0_WrFL(tcs_boundary);

	PHY_REG_H15A0_RdFL(tcs_additional_value);
	PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
	PHY_REG_H15A0_WrFL(tcs_additional_value);

	PHY_REG_H15A0_RdFL(dr_clk_inv_ch);
	PHY_REG_H15A0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch0,0x1);
	PHY_REG_H15A0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch1,0x1);
	PHY_REG_H15A0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch2,0x1);
	PHY_REG_H15A0_WrFL(dr_clk_inv_ch);

	/* H15 New HDMI Phy Register Settings */
	PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
	PHY_REG_H15A0_Wr01(eq_cs_rs_sel,eq_rs_man_sel,0x1);
#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
	PHY_REG_H15A0_Wr01(eq_cs_rs_sel,eq_cs_man_sel,0x1);
#endif
	PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
	PHY_REG_H15A0_RdFL(ck_mode_cs_man_00);
	PHY_REG_H15A0_Wr01(ck_mode_cs_man_00,ck_mode_cs_man_00,0x6);
	PHY_REG_H15A0_WrFL(ck_mode_cs_man_00);
	PHY_REG_H15A0_RdFL(ck_mode_cs_man_01);
	PHY_REG_H15A0_Wr01(ck_mode_cs_man_01,ck_mode_cs_man_01,0x8);
	PHY_REG_H15A0_WrFL(ck_mode_cs_man_01);
	PHY_REG_H15A0_RdFL(ck_mode_cs_man_10);
	PHY_REG_H15A0_Wr01(ck_mode_cs_man_10,ck_mode_cs_man_10,0xA);
	PHY_REG_H15A0_WrFL(ck_mode_cs_man_10);
	PHY_REG_H15A0_RdFL(ck_mode_cs_man_11);
	PHY_REG_H15A0_Wr01(ck_mode_cs_man_11,ck_mode_cs_man_11,0xC);
	PHY_REG_H15A0_WrFL(ck_mode_cs_man_11);
#endif

	// For edison room XBOX360
	PHY_REG_H15A0_RdFL(idr_adj);
	PHY_REG_H15A0_Wr01(idr_adj,idr_adj,0x1); //140829 by shyang
	PHY_REG_H15A0_WrFL(idr_adj);

#ifdef FOR_EDISON_XBOX360
	PHY_REG_H15A0_RdFL(cr_vbgr_ctrl);
	PHY_REG_H15A0_Wr01(cr_vbgr_ctrl,cr_vbgr_ctrl,0x5);	//140403 : OLED 15M Cable (from 0x4)
	PHY_REG_H15A0_WrFL(cr_vbgr_ctrl);

	PHY_REG_H15A0_RdFL(cr_tmr_scale);
	PHY_REG_H15A0_Wr01(cr_tmr_scale,cr_tmr_scale,0x02);
	PHY_REG_H15A0_WrFL(cr_tmr_scale);

	PHY_REG_H15A0_RdFL(eq_eval_time_1);
	PHY_REG_H15A0_Wr01(eq_eval_time_1,eq_eval_time_1,0xff);
	PHY_REG_H15A0_WrFL(eq_eval_time_1);

	PHY_REG_H15A0_RdFL(eq_eval_time_2);
	PHY_REG_H15A0_Wr01(eq_eval_time_2,eq_eval_time_2,0xff);
	PHY_REG_H15A0_WrFL(eq_eval_time_2);

	PHY_REG_H15A0_RdFL(eq_filter);
	PHY_REG_H15A0_Wr01(eq_filter,eq_filter,0x0f);	//0x1f : MSPG-4233 HDCP fail issue //0x3f // default 0x0f
	PHY_REG_H15A0_WrFL(eq_filter);
#endif
	// End of For edison room XBOX360

	PHY_REG_H15A0_RdFL(tmds_errec_detect);
	PHY_REG_H15A0_Wr01(tmds_errec_detect,tmds_errec_detect,0x1);
	PHY_REG_H15A0_WrFL(tmds_errec_detect);

	PHY_REG_H15A0_RdFL(bert_tmds_sel);
	PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x1);
	PHY_REG_H15A0_WrFL(bert_tmds_sel);

	PHY_REG_H15A0_RdFL(tmds_clk_inv);
	PHY_REG_H15A0_Wr01(tmds_clk_inv,tmds_clk_inv,0x0);
	PHY_REG_H15A0_WrFL(tmds_clk_inv);

	PHY_REG_H15A0_RdFL(reg_tcs_man_pre);
	PHY_REG_H15A0_Wr01(reg_tcs_man_pre,reg_tcs_man_pre,0x40);
	PHY_REG_H15A0_WrFL(reg_tcs_man_pre);

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_enable_prt1);

	LINK_NUM_H15_RdFL(1, system_control_00);
#ifdef H15_BRINGUP_TODO
	LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, prt_selected);
#endif
	LINK_NUM_H15_Rd01(1, system_control_00, reg_cd_sense, cd_sense);

	if( _gH15A0HDMIPhyInform.phy_enable[0] )
		phy_enabled_prt = 0;
	else
		phy_enabled_prt = 1;

	if(_g_Disable_EQ_Offset_Cal[phy_enabled_prt] == 1) {

		HDMI_NOTI("@@@ Port[%d] Disable EQ Calibration Mode in PhyRunReset \n", phy_enabled_prt);

		/* SS BD-P1600 workaround code */
		PHY_REG_H15A0_RdFL(eq_cal_mode);
		PHY_REG_H15A0_Wr01(eq_cal_mode,eq_cal_mode,0x0);
		PHY_REG_H15A0_WrFL(eq_cal_mode);

		_g_Disable_EQ_Offset_Cal[phy_enabled_prt] = 0;
	}

	PHY_REG_H15A0_RdFL(dr_filter_ch0);
	PHY_REG_H15A0_RdFL(cr_icp_adj);
	PHY_REG_H15A0_RdFL(eq_rs_man);

	if ( ( (gHDMI_Phy_Control.all_port_pdb_enable && phy_enable_prt1) /* || (!gHDMI_Phy_Control.all_port_pdb_enable && (prt_selected==0x3) )*/ ) && cd_sense)
	{
// GetMHLConnection also execute these settings

	}
	else
	{
		HDMI_DEBUG("---- HDMI Mode dr_filter_ch0 : [%d]\n", 0x7);
		PHY_REG_H15A0_Wr01(dr_filter_ch0,dr_filter_ch0,0x7);
		HDMI_DEBUG("---- HDMI Mode  CR_PLL Charge Pump Current Adjust : [%d]\n", 0x7);
		PHY_REG_H15A0_Wr01(cr_icp_adj,cr_icp_adj,0x7);
#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
		HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
		PHY_REG_H15A0_Wr01(eq_rs_man,eq_rs_man,0x3);
#else
		HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
		/* BD570 EQ error : EQ RS value 0x7 (default 0x3) , 131114 */
		PHY_REG_H15A0_Wr01(eq_rs_man,eq_rs_man,0x3);
#endif
#ifdef H15_CODE_FOR_MHL_CTS
		HDMI_DEBUG("---- HDMI ODC CTRL  : [%d]\n", 0x1);
		PHY_REG_H15A0_RdFL(odt_ctrl);
		PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x1);
		PHY_REG_H15A0_WrFL(odt_ctrl);
#endif

		HDMI_DEBUG("---- HDMI DR_N1  : [%d]\n", 0x3);
		PHY_REG_H15A0_RdFL(dr_n1);
		//PHY_REG_H15A0_Wr01(dr_n1,dr_n1,0x3);	// for 15m cable (0x38/0x93)
		PHY_REG_H15A0_Wr01(dr_n1,dr_n1,0x7);	// for U+ 4K STB
		PHY_REG_H15A0_WrFL(dr_n1);
	}

	PHY_REG_H15A0_WrFL(dr_filter_ch0);
	PHY_REG_H15A0_WrFL(cr_icp_adj);
	PHY_REG_H15A0_WrFL(eq_rs_man);

	OS_MsecSleep(100);	// ms delay

	PHY_REG_H15A0_RdFL(cr_mode_sel_resetb);
	PHY_REG_H15A0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
	PHY_REG_H15A0_WrFL(cr_mode_sel_resetb);

	PHY_REG_H15A0_RdFL(resetb_pdb_all);
	PHY_REG_H15A0_Wr01(resetb_pdb_all,resetb_all,0x1);
	PHY_REG_H15A0_WrFL(resetb_pdb_all);

	return RET_OK;
}

static int _HDMI_H15B0_PhyRunReset(void)
{
	//int prt_selected;
	int phy_enable_prt1, cd_sense;
	int phy_enabled_prt;

	HDMI_DEBUG("[%s] Entered \n",__func__);

	/* H15B0 New HDMI Phy Register Settings */
   PHY_REG_H15B0_RdFL(fifo_mode);
//   PHY_REG_H15B0_Wr01(fifo_mode_muxed_clk_sel,fifo_mode,0x1);
   PHY_REG_H15B0_Wr01(fifo_mode,fifo_mode,0x2);	// invalid timing when silicon image MHL3.0 timing 371
   PHY_REG_H15B0_WrFL(fifo_mode);

   PHY_REG_H15B0_RdFL(cr_i2c_con_sel_mode);
   PHY_REG_H15B0_Wr01(cr_i2c_con_sel_mode, man_con_sel,0x1);
   PHY_REG_H15B0_Wr01(cr_i2c_con_sel_mode, cr_i2c_con_sel_mode,0x1);
   PHY_REG_H15B0_WrFL(cr_i2c_con_sel_mode);

   PHY_REG_H15B0_RdFL(eq_i2c_odt_pdb);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_man_odt_pdb,0x0);
   PHY_REG_H15B0_WrFL(eq_i2c_odt_pdb);

   PHY_REG_H15B0_RdFL(eq_i2c_odt_pdb);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_man_odt_pdb,0x1);
   PHY_REG_H15B0_WrFL(eq_i2c_odt_pdb);

   PHY_REG_H15B0_RdFL(eq_i2c_odt_pdb);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_man_odt_pdb,0x0);
   PHY_REG_H15B0_WrFL(eq_i2c_odt_pdb);

   PHY_REG_H15B0_RdFL(eq_i2c_odt_pdb);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_i2c_odt_pdb_mode,0x1);
   PHY_REG_H15B0_Wr01(eq_i2c_odt_pdb,eq_man_odt_pdb,0x1);
   PHY_REG_H15B0_WrFL(eq_i2c_odt_pdb);

   //140829 : added by shyang
	PHY_REG_H15B0_RdFL(eq_odt_ctrl);
	PHY_REG_H15B0_Wr01(eq_odt_ctrl,eq_odt_ctrl,0x1);
	PHY_REG_H15B0_WrFL(eq_odt_ctrl);

	PHY_REG_H15B0_RdFL(eq_odt_ctrl);
	PHY_REG_H15B0_Wr01(eq_odt_ctrl,eq_odt_ctrl,0x2);
	PHY_REG_H15B0_WrFL(eq_odt_ctrl);

	PHY_REG_H15B0_RdFL(eq_odt_ctrl);
	PHY_REG_H15B0_Wr01(eq_odt_ctrl,eq_odt_ctrl,0x1);
	PHY_REG_H15B0_WrFL(eq_odt_ctrl);

	PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
	PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x0);
	PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

	/* H15B0 New HDMI Phy Register Settings */
	PHY_REG_H15B0_RdFL(tcs_rs_mode_sel);
	PHY_REG_H15B0_Wr01(tcs_rs_mode_sel, tcs_mode_sel,_HDMI_H15A0_TCS_mode_sel);
//	PHY_REG_H15B0_Wr01(tcs_rs_mode_sel, eq_rs_mode_sel,0x1);
	PHY_REG_H15B0_WrFL(tcs_rs_mode_sel);

	if(_HDMI_H15A0_TCS_mode_sel == 0x2)
	{
		PHY_REG_H15B0_RdFL(eq_cs_man_ch0);
		PHY_REG_H15B0_Wr01(eq_cs_man_ch0, eq_cs_man_ch0,0x12);
		PHY_REG_H15B0_WrFL(eq_cs_man_ch0);

		PHY_REG_H15B0_RdFL(eq_cs_man_ch1);
		PHY_REG_H15B0_Wr01(eq_cs_man_ch1, eq_cs_man_ch1,0x12);
		PHY_REG_H15B0_WrFL(eq_cs_man_ch1);

		PHY_REG_H15B0_RdFL(eq_cs_man_ch2);
		PHY_REG_H15B0_Wr01(eq_cs_man_ch2, eq_cs_man_ch2,0x12);
		PHY_REG_H15B0_WrFL(eq_cs_man_ch2);
	}
	else if(_HDMI_H15A0_TCS_mode_sel == 0x3)
	{

		PHY_REG_H15B0_RdFL(tcs_err_sel);
		PHY_REG_H15B0_Wr01(tcs_err_sel, tcs_err_sel,0x3);
		PHY_REG_H15B0_WrFL(tcs_err_sel);

		PHY_REG_H15B0_RdFL(tcs_bcs_ch_sel);
		PHY_REG_H15B0_Wr01(tcs_bcs_ch_sel, bcs_ch_sel,0x0);
		PHY_REG_H15B0_Wr01(tcs_bcs_ch_sel, tcs_ch_sel,0x0);
		PHY_REG_H15B0_WrFL(tcs_bcs_ch_sel);
	}

	PHY_REG_H15B0_RdFL(tcs_repeat_off_divide_method);
	PHY_REG_H15B0_Wr01(tcs_repeat_off_divide_method, tcs_divide_method, 0x2);	//default : 0x2
	PHY_REG_H15B0_WrFL(tcs_repeat_off_divide_method);

	PHY_REG_H15B0_RdFL(tcs_boundary);
	PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
	PHY_REG_H15B0_WrFL(tcs_boundary);

	PHY_REG_H15B0_RdFL(tcs_additional_value);
	PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
	PHY_REG_H15B0_WrFL(tcs_additional_value);

	PHY_REG_H15B0_RdFL(dr_clk_inv_ch);
	PHY_REG_H15B0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch0,0x1);
	PHY_REG_H15B0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch1,0x1);
	PHY_REG_H15B0_Wr01(dr_clk_inv_ch,dr_clk_inv_ch2,0x1);
	PHY_REG_H15B0_WrFL(dr_clk_inv_ch);

	/* H15 New HDMI Phy Register Settings */
	PHY_REG_H15B0_RdFL(tcs_rs_mode_sel);
	PHY_REG_H15B0_Wr01(tcs_rs_mode_sel,eq_rs_mode_sel,0x1);
#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
	PHY_REG_H15B0_Wr01(tcs_rs_mode_sel,eq_cs_mode_sel,0x1);
#endif
	PHY_REG_H15B0_WrFL(tcs_rs_mode_sel);

#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
	PHY_REG_H15B0_RdFL(ck_mode_cs_man_00);
	PHY_REG_H15B0_Wr01(ck_mode_cs_man_00,ck_mode_cs_man_00,0x6);
	PHY_REG_H15B0_WrFL(ck_mode_cs_man_00);
	PHY_REG_H15B0_RdFL(ck_mode_cs_man_01);
	PHY_REG_H15B0_Wr01(ck_mode_cs_man_01,ck_mode_cs_man_01,0x8);
	PHY_REG_H15B0_WrFL(ck_mode_cs_man_01);
	PHY_REG_H15B0_RdFL(ck_mode_cs_man_10);
	PHY_REG_H15B0_Wr01(ck_mode_cs_man_10,ck_mode_cs_man_10,0xA);
	PHY_REG_H15B0_WrFL(ck_mode_cs_man_10);
	PHY_REG_H15B0_RdFL(ck_mode_cs_man_11);
	PHY_REG_H15B0_Wr01(ck_mode_cs_man_11,ck_mode_cs_man_11,0xC);
	PHY_REG_H15B0_WrFL(ck_mode_cs_man_11);
#endif

	//140930 (4K, 15M, DVB)
	PHY_REG_H15B0_RdFL(eq_freq_div_ch0);
	PHY_REG_H15B0_Wr01(eq_freq_div_ch0,eq_freq_div_ch0,0x6); //141028 :0x6 (HDCP Error:MSPG-7100) //140930 : 0xF (4K,DVB)
	PHY_REG_H15B0_WrFL(eq_freq_div_ch0);
	PHY_REG_H15B0_RdFL(eq_freq_div_ch1);
	PHY_REG_H15B0_Wr01(eq_freq_div_ch1,eq_freq_div_ch1,0x6); //141028 :0x6 (HDCP Error:MSPG-7100) //140930 : 0xF (4K,DVB)
	PHY_REG_H15B0_WrFL(eq_freq_div_ch1);
	PHY_REG_H15B0_RdFL(eq_freq_div_ch2);
	PHY_REG_H15B0_Wr01(eq_freq_div_ch2,eq_freq_div_ch2,0x6); //141028 :0x6 (HDCP Error:MSPG-7100) //140930 : 0xF (4K,DVB)
	PHY_REG_H15B0_WrFL(eq_freq_div_ch2);

	// For edison room XBOX360
	// 4K U+ STB (141112)
	PHY_REG_H15B0_RdFL(idr_adj);
	PHY_REG_H15B0_Wr01(idr_adj,idr_adj,0x3); //from 0x1
	PHY_REG_H15B0_WrFL(idr_adj);

#ifdef FOR_EDISON_XBOX360
	PHY_REG_H15B0_RdFL(cr_vbgr_ctrl);
	PHY_REG_H15B0_Wr01(cr_vbgr_ctrl,cr_vbgr_ctrl,0x5);	//140403 : OLED 15M Cable (from 0x4)
	PHY_REG_H15B0_WrFL(cr_vbgr_ctrl);

	PHY_REG_H15B0_RdFL(cr_tmr_scale);
	PHY_REG_H15B0_Wr01(cr_tmr_scale,cr_tmr_scale,0x02);
	PHY_REG_H15B0_WrFL(cr_tmr_scale);

	PHY_REG_H15B0_RdFL(eq_eval_time_1);
	PHY_REG_H15B0_Wr01(eq_eval_time_1,eq_eval_time_1,0xff);
	PHY_REG_H15B0_WrFL(eq_eval_time_1);

	PHY_REG_H15B0_RdFL(eq_eval_time_2);
	PHY_REG_H15B0_Wr01(eq_eval_time_2,eq_eval_time_2,0xff);
	PHY_REG_H15B0_WrFL(eq_eval_time_2);

	PHY_REG_H15B0_RdFL(eq_filter);
	PHY_REG_H15B0_Wr01(eq_filter,eq_filter,0x0f);	//0x1f : MSPG-4233 HDCP fail issue //0x3f // default 0x0f
	PHY_REG_H15B0_WrFL(eq_filter);
#endif
	// End of For edison room XBOX360

	PHY_REG_H15B0_RdFL(tmds_errec_detect);
	PHY_REG_H15B0_Wr01(tmds_errec_detect,tmds_errec_detect,0x1);
	PHY_REG_H15B0_WrFL(tmds_errec_detect);

	PHY_REG_H15B0_RdFL(bert_tmds_sel);
	PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
	PHY_REG_H15B0_WrFL(bert_tmds_sel);

	PHY_REG_H15B0_RdFL(tmds_sync_recover_clk_inv_bypass);
	PHY_REG_H15B0_Wr01(tmds_sync_recover_clk_inv_bypass,tmds_clk_inv,0x0);
	PHY_REG_H15B0_WrFL(tmds_sync_recover_clk_inv_bypass);

	PHY_REG_H15B0_RdFL(reg_tcs_man_pre);
	PHY_REG_H15B0_Wr01(reg_tcs_man_pre,reg_tcs_man_pre,0x40);
	PHY_REG_H15B0_WrFL(reg_tcs_man_pre);

	PHY_REG_H15B0_RdFL(reg_tcs_man_mea_1);
	PHY_REG_H15B0_Wr01(reg_tcs_man_mea_1,reg_tcs_man_mea,0x44);
	PHY_REG_H15B0_WrFL(reg_tcs_man_mea_1);

	PHY_REG_H15B0_RdFL(tcs_scdt);
	PHY_REG_H15B0_Wr01(tcs_scdt,tcs_scdt,0x1);
	PHY_REG_H15B0_WrFL(tcs_scdt);

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_enable_prt1);

	LINK_NUM_H15_RdFL(1, system_control_00);
#ifdef H15_BRINGUP_TODO
	LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, prt_selected);
#endif
	LINK_NUM_H15_Rd01(1, system_control_00, reg_cd_sense, cd_sense);

	if( _gH15A0HDMIPhyInform.phy_enable[0] )
		phy_enabled_prt = 0;
	else
		phy_enabled_prt = 1;

	if(_g_Disable_EQ_Offset_Cal[phy_enabled_prt] == 1) {

		HDMI_NOTI("@@@ Port[%d] Disable EQ Calibration Mode in PhyRunReset \n", phy_enabled_prt);

		/* SS BD-P1600 workaround code */
		PHY_REG_H15A0_RdFL(eq_cal_mode);
		PHY_REG_H15A0_Wr01(eq_cal_mode,eq_cal_mode,0x0);
		PHY_REG_H15A0_WrFL(eq_cal_mode);

		_g_Disable_EQ_Offset_Cal[phy_enabled_prt] = 0;
	}

	PHY_REG_H15B0_RdFL(dr_filter_ch0);
	PHY_REG_H15B0_RdFL(cr_icp_adj);
	PHY_REG_H15B0_RdFL(eq_rs_man);

	if ( ( (gHDMI_Phy_Control.all_port_pdb_enable && phy_enable_prt1) /* || (!gHDMI_Phy_Control.all_port_pdb_enable && (prt_selected==0x3) )*/ ) && cd_sense)
	{
// GetMHLConnection also execute these settings

	}
	else
	{
		HDMI_DEBUG("---- HDMI Mode dr_filter_ch0 : [%d]\n", 0x7);
		PHY_REG_H15B0_Wr01(dr_filter_ch0,dr_filter_ch0,0x7);
		HDMI_DEBUG("---- HDMI Mode  CR_PLL Charge Pump Current Adjust : [%d]\n", 0x7);
		PHY_REG_H15B0_Wr01(cr_icp_adj,cr_icp_adj,0x7);
#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
		HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
		PHY_REG_H15B0_Wr01(eq_rs_man,eq_rs_man,0x3);
#else
		HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
		/* BD570 EQ error : EQ RS value 0x7 (default 0x3) , 131114 */
		PHY_REG_H15B0_Wr01(eq_rs_man,eq_rs_man,0x3);
#endif
#ifdef H15_CODE_FOR_MHL_CTS
		HDMI_DEBUG("---- HDMI ODC CTRL  : [%d]\n", 0x1);
		PHY_REG_H15B0_RdFL(eq_odt_ctrl);
		PHY_REG_H15B0_Wr01(eq_odt_ctrl,eq_odt_ctrl,0x1);
		PHY_REG_H15B0_WrFL(eq_odt_ctrl);
#endif

		HDMI_DEBUG("---- HDMI DR_N1  : [%d]\n", 0x3);
		PHY_REG_H15B0_RdFL(dr_n1);
		//PHY_REG_H15B0_Wr01(dr_n1,dr_n1,0x3);	// for 15m cable (0x38/0x93)
		PHY_REG_H15B0_Wr01(dr_n1,dr_n1,0x7);	// for U+ 4K STB
		PHY_REG_H15B0_WrFL(dr_n1);

		// for HDMI : 0 for HPD out level 3.3Volt
		HDMI_DEBUG("---- HDMI CBUS_NORMAL_V1  : [%d]\n", 0x0);
		PHY_REG_H15B0_RdFL(cbus_normal_v1_pdb);
		PHY_REG_H15B0_Wr01(cbus_normal_v1_pdb,cbus_normal_v1,0x0);	// for HPD out Level
		PHY_REG_H15B0_WrFL(cbus_normal_v1_pdb);
	}

	PHY_REG_H15B0_WrFL(dr_filter_ch0);
	PHY_REG_H15B0_WrFL(cr_icp_adj);
	PHY_REG_H15B0_WrFL(eq_rs_man);

	OS_MsecSleep(10);	// ms delay	: reduce for 5V to HPD short delay

	PHY_REG_H15B0_RdFL(cr_mode_sel_resetb);
	PHY_REG_H15B0_Wr01(cr_mode_sel_resetb, cr_mode_sel_resetb,0x1);
	PHY_REG_H15B0_WrFL(cr_mode_sel_resetb);

	PHY_REG_H15B0_RdFL(resetb_pdb_all);
	PHY_REG_H15B0_Wr01(resetb_pdb_all,resetb_all,0x1);
	PHY_REG_H15B0_WrFL(resetb_pdb_all);

	return RET_OK;
}

static int _HDMI_H15A0_RunReset(void)
{
	LINK_NUM_H15_RdFL(0, hdcp_00);
	LINK_NUM_H15_Wr02(0, hdcp_00, reg_con_sel_man, 1, reg_con_sel_man_mode, 1);
	LINK_NUM_H15_WrFL(0, hdcp_00);
	LINK_NUM_H15_RdFL(1, hdcp_00);
	LINK_NUM_H15_Wr02(1, hdcp_00, reg_con_sel_man, 1, reg_con_sel_man_mode, 1);
	LINK_NUM_H15_WrFL(1, hdcp_00);

	// Clock Divide for Pixel Repetition format
	LINK_NUM_H15_RdFL(0, video_00);
	LINK_NUM_H15_Wr01(0, video_00, reg_pr_cmu_sync, 0x1);
	LINK_NUM_H15_WrFL(0, video_00);

	// Clock Divide for Pixel Repetition format
	LINK_NUM_H15_RdFL(1, video_00);
	LINK_NUM_H15_Wr01(1, video_00, reg_pr_cmu_sync, 0x1);
	LINK_NUM_H15_WrFL(1, video_00);

	// Negative polarity for M14 UD and 3D_SS_Full format
	// Support positive and negative polarity for M14 DE HDMI data bridge
	LINK_NUM_H15_RdFL(0, video_03);
	LINK_NUM_H15_Wr01(0, video_03, reg_neg_pol_en,0x1);
	LINK_NUM_H15_WrFL(0, video_03);

	// Negative polarity for M14 UD and 3D_SS_Full format
	// Support positive and negative polarity for M14 DE HDMI data bridge
	LINK_NUM_H15_RdFL(1, video_03);
	LINK_NUM_H15_Wr01(1, video_03, reg_neg_pol_en,0x1);
	LINK_NUM_H15_WrFL(1, video_03);

	/* MHL 2.0 900mA */
	LINK_NUM_H15_RdFL(1, cbus_15);
	LINK_NUM_H15_Wr01(1, cbus_15, reg_sink_cap_dev_cat, 0x31);
	LINK_NUM_H15_WrFL(1, cbus_15);

	/* HDCP 1.1 not support*/
	LINK_NUM_H15_RdFL(0, hdcp_05);
	LINK_NUM_H15_Wr01(0, hdcp_05, reg_hdcp_bcaps, 0x81);
	LINK_NUM_H15_WrFL(0, hdcp_05);

	/* HDCP 1.1 not support*/
	LINK_NUM_H15_RdFL(1, hdcp_05);
	LINK_NUM_H15_Wr01(1, hdcp_05, reg_hdcp_bcaps, 0x81);
	LINK_NUM_H15_WrFL(1, hdcp_05);

	/*
	LINK_NUM_H15_RdFL(0, hdcp_11);
	LINK_NUM_H15_Wr01(0, hdcp_11, reg_hdcp_bcaps_prt1, 0x81);
	LINK_NUM_H15_WrFL(0, hdcp_11);

	LINK_NUM_H15_RdFL(0, hdcp_17);
	LINK_NUM_H15_Wr01(0, hdcp_17, reg_hdcp_bcaps_prt2, 0x81);
	LINK_NUM_H15_WrFL(0, hdcp_17);

	LINK_NUM_H15_RdFL(0, hdcp_23);
	LINK_NUM_H15_Wr01(0, hdcp_23, reg_hdcp_bcaps_prt3, 0x81);
	LINK_NUM_H15_WrFL(0, hdcp_23);
	*/

	LINK_NUM_H15_RdFL(1, cbus_12);
	LINK_NUM_H15_Wr01(1, cbus_12, reg_man_src_support_rcp, 0x1);	//manual RCP support for Vu/LTE1 phones
	LINK_NUM_H15_WrFL(1, cbus_12);

#ifdef H15_CODE_FOR_MHL_CTS
	LINK_NUM_H15_RdFL(1, cbus_02);
	LINK_NUM_H15_Wr01(1, cbus_02, reg_cbus_bi_n_retry, 0x1F);	//0x1E
	LINK_NUM_H15_WrFL(1, cbus_02);

	//0x298
	LINK_NUM_H15_RdFL(1, cbus_15);
	LINK_NUM_H15_Wr(1, cbus_15, 0x00213102);	//mhl version 2.1
	LINK_NUM_H15_WrFL(1, cbus_15);

	//0x29c
	LINK_NUM_H15_RdFL(1, cbus_16);
	LINK_NUM_H15_Wr(1, cbus_16, 0x64370100);
	LINK_NUM_H15_WrFL(1, cbus_16);

	//0x2A0
	LINK_NUM_H15_RdFL(1, cbus_17);
	LINK_NUM_H15_Wr(1, cbus_17, 0x410F0700);
	LINK_NUM_H15_WrFL(1, cbus_17);

	LINK_NUM_H15_RdFL(1, cbus_18);
	LINK_NUM_H15_Wr(1, cbus_18, 0x00001033);
	LINK_NUM_H15_WrFL(1, cbus_18);

	LINK_NUM_H15_RdFL(1, cbus_42);
	LINK_NUM_H15_Wr(1, cbus_42, 0x001F0201);
	LINK_NUM_H15_WrFL(1, cbus_42);

	LINK_NUM_H15_RdFL(1, cbus_29);
	LINK_NUM_H15_Wr(1, cbus_29, 0x01100110);
	LINK_NUM_H15_WrFL(1, cbus_29);

	LINK_NUM_H15_RdFL(1, cbus_30);
	LINK_NUM_H15_Wr(1, cbus_30, 0x01000000);
	LINK_NUM_H15_WrFL(1, cbus_30);

	LINK_NUM_H15_RdFL(1, cbus_36);
	LINK_NUM_H15_Wr(1, cbus_36, 0x120311);
	LINK_NUM_H15_WrFL(1, cbus_36);

	LINK_NUM_H15_RdFL(1, cbus_35);
	LINK_NUM_H15_Wr01(1, cbus_35, reg_cbus_wake_to_discover_min_2, 0x50);	//0x63 : MHL CTS C-Bus Wakeup fix
	LINK_NUM_H15_WrFL(1, cbus_35);

	LINK_NUM_H15_RdFL(1, cbus_33);
	LINK_NUM_H15_Wr01(1, cbus_33, reg_cbus_wake_pulse_width_1_min, 0x0D);	//0x11 : min pulse width (17msec => 13msec) //meizu phone
	LINK_NUM_H15_Wr01(1, cbus_33, reg_cbus_wake_pulse_width_1_max, 0x21);	//0x17 : max pulse width (23msec => 33msec)	//HTC One S Phone
	LINK_NUM_H15_WrFL(1, cbus_33);

	LINK_NUM_H15_RdFL(1, cbus_34);
	LINK_NUM_H15_Wr01(1, cbus_34, reg_cbus_wake_pulse_width_2_max, 0x4D);	//0x43 : max pulse width (67msec => 77msec)	//HTC One S Phone
	LINK_NUM_H15_WrFL(1, cbus_34);
#endif
	// HPD PIN Control Enable
	//LINK_NUM_H15_RdFL(0, phy_00);
	//LINK_NUM_H15_Wr01(0, phy_00, hpd0_oen, 0x0);
	//LINK_NUM_H15_WrFL(0, phy_00);

/*	// set internal test hdcp key
	LINK_NUM_H15_RdFL(0, (UINT32*)&glink_818_bx);
	LINK_NUM_H15_Wr01.reg_dbg_hdcp_key_bak_en,0x0)
	LINK_NUM_H15_WrFL(0, (UINT32*)&glink_818_bx);
*/
	/*
	LINK_NUM_H15_RdFL(0, hdcp_00);
	LINK_NUM_H15_Wr02(0, hdcp_00, reg_hdcp_unauth_mode_chg_prt0, 0, reg_hdcp_unauth_nosync_prt0, 0);
	LINK_NUM_H15_WrFL(0, hdcp_00);
	LINK_NUM_H15_RdFL(0, hdcp_06);
	LINK_NUM_H15_Wr02(0, hdcp_06, reg_hdcp_unauth_mode_chg_prt1, 0, reg_hdcp_unauth_nosync_prt1, 0);
	LINK_NUM_H15_WrFL(0, hdcp_06);
	LINK_NUM_H15_RdFL(0, hdcp_12);
	LINK_NUM_H15_Wr02(0, hdcp_12, reg_hdcp_unauth_mode_chg_prt2, 0, reg_hdcp_unauth_nosync_prt2, 0);
	LINK_NUM_H15_WrFL(0, hdcp_12);
	LINK_NUM_H15_RdFL(0, hdcp_18);
	LINK_NUM_H15_Wr02(0, hdcp_18, reg_hdcp_unauth_mode_chg_prt3, 0, reg_hdcp_unauth_nosync_prt3, 0);
	LINK_NUM_H15_WrFL(0, hdcp_18);
	*/

	return RET_OK;
}

/**
* _HDMI_H15A0_SetAudio
*
* @parm void
* @return int
*/
static int _HDMI_H15A0_SetAudio(int link_num)
{
	//ARC source, Channel 0
	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Wr01(link_num, system_control_00, reg_arc_src, 0x1);	//ARC 1: loop-back test
	LINK_NUM_H15_Wr01(link_num, system_control_00, reg_aac_en, 	0x1);	//Auto Audio Path Configuration Enable(N, CTS value is auto configured.)
	LINK_NUM_H15_WrFL(link_num, system_control_00);

	LINK_NUM_H15_RdFL(link_num, audio_00);
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_en, 		 	0x0);	//ACR Enable(Audio Clock Generation Function Activation)
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_n_fs, 		0x1);	//0 : 128Fs, 1 : 256 Fs, 2 : 512Fs(default : 256Fs)
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_clk_aud_div,	0x1);	//0 : 128Fs, 1 : 256 Fs, 2 : 512Fs(default : 256Fs)
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_ncts_rx_en,  	0x0);	//Use N, CTS value for audio clock generation
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_adj_thr, 		0x3);	//Threshold Value for Clock Frequency Auto Adjustment for proper FIFO running, not required
	LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_adj_en, 	 	0x1);	//Enable Clock Frequency Auto Adjustment for proper FIFO running
	LINK_NUM_H15_WrFL(link_num, audio_00);

	LINK_NUM_H15_RdFL(link_num, audio_05);
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sck_edge, 1);	//I2S Format for falling edge
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd_en,  0xF);	//I2S SD Output Enable(4 Channel)
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_out_en,   1);	//I2S Output Enable
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd0_map,  0);	//I2S SD 0 Output Channel Mappings
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd1_map,  1);	//I2S SD 1 Output Channel Mappings
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd2_map,  2);	//I2S SD 2 Output Channel Mappings
	LINK_NUM_H15_Wr01(link_num, audio_05, reg_i2s_sd3_map,  3);	//I2S SD 3 Output Channel Mappings
	LINK_NUM_H15_WrFL(link_num, audio_05);

	//Set audio mute state
	_gH15A0AudioMuteState[link_num] = FALSE;

	HDMI_AUDIO("[%d] %s : link_num = %d\n", __L__, __F__, link_num);
	return RET_OK;
}


/**
 *  _HDMI_H15A0_GetAudioTypeAndFreq
 *
 *  @return int
*/
static int _HDMI_H15A0_GetAudioInfo(int link_num)
{
	int 	ret = RET_OK;

	ULONG	flags = 0;
	BOOLEAN bDebugEnabled = FALSE;	//< HDMI Debug Print Enabled

	UINT32	reg_scdt, reg_hdmi_mode, reg_burst_pc_0;

	UINT16	ui16VActive;		//< HDMI Vertical Active Size
	UINT16	ui16HActive;		//< HDMI horizontal Active Size
	UINT32	intrVideoState		= HDMI_STATE_DISABLE;
	UINT32	intrAudioState		= HDMI_AUDIO_INIT_STATE;
	UINT32	audioRecheckTime 	= HDMI_AUDIO_RECHECK_TIME_500MS;

#ifndef H15_THREAD_READ_PHY_REG_VALUE
	UINT8	ui8TmdsClockHigh = 0;		//< HDMI measured clock value of TMDS clock for upper 8 bit
	UINT8	ui8TmdsClockLow  = 0;		//< HDMI measured clock value of TMDS clock for low 8 bit
#endif
	UINT64	ui64TmdsClock 	 = 0;		//< HDMI measured clock value of TMDS clock

	LX_HDMI_AUDIO_TYPE_T		audioType		= LX_HDMI_AUDIO_NO_AUDIO;		///< HDMI Audio Type.
	LX_HDMI_SAMPLING_FREQ_T 	samplingFreq	= LX_HDMI_SAMPLING_FREQ_NONE; 	///< Sampling Frequency

	LX_HDMI_SAMPLING_FREQ_T 	samplingFreqFromTmds = LX_HDMI_SAMPLING_FREQ_NONE;	//< HDMI sampling frequency from TMDS clock
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_AUDIO_MUTE};

	//Increase _gH15A0HdmiAudioThreadCount[link_num];
	_gH15A0HdmiAudioThreadCount[link_num]++;

	//Check HDMI port connection
	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Rd01(link_num, system_control_00, reg_scdt, reg_scdt);
	LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	if (reg_scdt == 0)	//Check HDMI, DVI Sync Detect
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);

		//Clear _gH15A0IntrAudioState[link_num]
		_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INIT_STATE;

		//Clear _gH15A0HdmiFreqErrorCount[link_num]
		_gH15A0HdmiFreqErrorCount[link_num] = 0;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI / DVI Not Connected(link_num = %d, state = %d)!!!\n", __L__, link_num, _gH15A0IntrAudioState[link_num]);

		goto func_exit;
	}

	//Get measured frequency value of TMDS clock.
#ifdef H15_THREAD_READ_PHY_REG_VALUE
	ui64TmdsClock = (UINT64)(_gH15A0HDMIPhyInform.tmds_clock[link_num]) * 10000;
#else
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		PHY_REG_H15B0_RdFL(tmds_freq_1);
		PHY_REG_H15B0_RdFL(tmds_freq_2);

		PHY_REG_H15B0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15B0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}
	else {
		PHY_REG_H15A0_RdFL(tmds_freq_1);
		PHY_REG_H15A0_RdFL(tmds_freq_2);

		PHY_REG_H15A0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15A0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}

	ui64TmdsClock = (UINT64)((ui8TmdsClockHigh << 8) | ui8TmdsClockLow) * 10000;
#endif

	//Check a UD format(do not support M14 chip upper 25MHz TDMS Clock)
	if (CHK_FHD_BD() && ui64TmdsClock > 250*1000*1000)
	{
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI UD Format for No Audio(link_num = %d, ui64TmdsClock = %llu)!!!\n", __L__, link_num, ui64TmdsClock);

		goto func_exit;
	}

	//Check a MHL Contents Off Status to set no audio
	if ((link_num == 1) && (_gH15A0MHLContentOff == TRUE) && (_gH15A0HDMIPhyInform.prt_sel == 3) && (_gH15A0HDMIPhyInform.cd_sense == 1))
	{
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : MHL Contents Off(link_num = %d, _gH15A0MHLContentOff = %d)!!!\n", __L__, link_num, _gH15A0MHLContentOff);

		goto func_exit;
	}

	//spin lock for protection : lock
	spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);

	//copy video interrupt state
	intrVideoState = sHDMI_H15A0_Status.HDMIState[link_num];
	//intrVideoState = HDMI_STATE_READ;	//temp for test

	//copy audio interrupt state
	intrAudioState = _gH15A0IntrAudioState[link_num];

	//spin lock for protection : unlock
	spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

	//Check _gH15A0IntrAudioState[link_num] is stably changed.
	if ((intrAudioState == HDMI_AUDIO_STABLE_STATE && intrVideoState >= HDMI_STATE_READ)
	  ||(intrAudioState == HDMI_AUDIO_STABLE_STATE && _gH15A0HdmiPortStableCount[link_num] > HDMI_AUDIO_PORT_STABLE_TIME_5S) )	//To protect a abnormal or transient video and audio info drop state.
	{
		if (_gH15A0HdmiAudioInfo[link_num].audioType == LX_HDMI_AUDIO_AC3_IEC60958)
			audioRecheckTime = HDMI_AUDIO_RECHECK_TIME_1S;
		else
			audioRecheckTime = HDMI_AUDIO_RECHECK_TIME_500MS;

		//Recheck for HDMI Audio Format and sampling frequency
		if ((_gH15A0HdmiAudioThreadCount[link_num] % audioRecheckTime) == 0)	// 0.5 or 1 seconds, Thread calls a every 20 ms.
		{
			//Check a current mute status for workaround
			if (_gH15A0AudioMuteState[link_num] == TRUE)
			{
				//Unmute audio data for abnormal state
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = FALSE;
				_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

				//Set a debug print status
				bDebugEnabled = TRUE;

				HDMI_ERROR("[%d]Forced UnMute : type = %d, freq = %d, mute = %d, PSC = %d\n", \
							__L__, _gH15A0HdmiAudioInfo[link_num].audioType, _gH15A0HdmiAudioInfo[link_num].samplingFreq, _gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num]);
			}

			//Get HDMI Audio Type and Sampling Frequency
			(void)_HDMI_H15A0_GetAudioTypeAndFreq(link_num, &audioType, &samplingFreq);

			//Get a sampling frequency from TMDS clock
			(void)_HDMI_H15A0_GetAudioFreqFromTmdsClock(link_num, &samplingFreqFromTmds);

			//M14 A0 IP Bug : non-PCM interrupt is not triggerred if non-PCM(AC-3) is changed to non-PCM(DTS).
			if ( (audioType			  != _gH15A0HdmiAudioInfo[link_num].audioType)
			  ||(samplingFreqFromTmds != _gH15A0HdmiAudioInfo[link_num].samplingFreq) )
			{
				HDMI_DEBUG("[%d]HDMI_GetAudioInfo : link_num = %d, type = %d<-%d, tmds freq = %d<-%d, freq = %d, state = %d, mute = %d, PSC = %d(%d)\n", \
							__L__, link_num, audioType, _gH15A0HdmiAudioInfo[link_num].audioType, samplingFreqFromTmds, _gH15A0HdmiAudioInfo[link_num].samplingFreq,	\
							samplingFreq, _gH15A0IntrAudioState[link_num], _gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num], audioRecheckTime);

				//spin lock for protection for audio
				spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
				_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INTERRUPT_STATE;
				_gH15A0HdmiFreqErrorCount[link_num] = 0;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

				//Mute audio data
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = TRUE;
				_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

				//Reset _gH15A0IntrBurstInfoCount[link_num]
				_gH15A0IntrBurstInfoCount[link_num] = 0;

				goto func_exit;
			}
		}

		//Increase _gH15A0HdmiPortStableCount[link_num](If HDMI port is changed, this count is cleared.)
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
		_gH15A0HdmiPortStableCount[link_num]++;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		//Debug print
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
		{
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : link_num = %d, type = %d, freq = %d, state = %d, PSC = %d. mute = %d\n",	\
						__L__, link_num, _gH15A0HdmiAudioInfo[link_num].audioType, _gH15A0HdmiAudioInfo[link_num].samplingFreq,	\
						_gH15A0IntrAudioState[link_num], _gH15A0HdmiPortStableCount[link_num], _gH15A0AudioMuteState[link_num]);
		}

		return ret;
	}
	else if (intrAudioState == HDMI_AUDIO_STABLE_STATE && intrVideoState < HDMI_STATE_READ)
	{
		HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_VIDEO_INTERRUPT_STATE(link_num = %d, state A = %d, V = %d)!!!\n", __L__, link_num, _gH15A0IntrAudioState[link_num], sHDMI_H15A0_Status.Intr_vf_stable[link_num]);

		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
		_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INTERRUPT_STATE;
		_gH15A0HdmiFreqErrorCount[link_num] = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);
	}
	else if (intrAudioState == HDMI_AUDIO_GET_INFO_STATE && intrVideoState >= HDMI_STATE_READ)
	{
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_AUDIO_GET_INFO_STATE(link_num = %d, state A = %d, V = %d)!!!\n", __L__, link_num, _gH15A0IntrAudioState[link_num], sHDMI_H15A0_Status.Intr_vf_stable[link_num]);
	}
	else if (intrAudioState < HDMI_AUDIO_GET_INFO_STATE && intrVideoState >= HDMI_STATE_READ)
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);

		//increase _gH15A0IntrAudioState[link_num]
		_gH15A0IntrAudioState[link_num]++;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI_AUDIO_UNSTABLE_STATE(link_num = %d, state A = %d, V = %d)!!!\n", __L__, link_num, _gH15A0IntrAudioState[link_num], sHDMI_H15A0_Status.Intr_vf_stable[link_num]);

		goto func_exit;
	}
	else
	{
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI / DVI Not Connected(link_num = %d, state = %d)!!!\n", __L__, link_num, _gH15A0IntrAudioState[link_num]);

		goto func_exit;
	}

	//Get video vertical & horizontal active size
	LINK_NUM_H15_RdFL(link_num, video_05);
	LINK_NUM_H15_Rd01(link_num, video_05, reg_h_av, ui16HActive);
	LINK_NUM_H15_Rd01(link_num, video_05, reg_v_av, ui16VActive);

	//Check video active size
	if ((ui16VActive <	240) || (ui16HActive <	320)	\
	  ||(ui16VActive > 2300) || (ui16HActive > 4096) )
	{
		HDMI_AUDIO("[%d]HDMI_GetAudioInfo : Video Active Size Error(link_num = %d, v = %d, h = %d)!!!\n", \
					__L__, link_num, ui16VActive, ui16HActive);

		goto func_exit;
	}

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	if (reg_hdmi_mode)
	{
		//Get HDMI Audio Type and Sampling Frequency
		(void)_HDMI_H15A0_GetAudioTypeAndFreq(link_num, &audioType, &samplingFreq);

		//Get a sampling frequency from TMDS clock
		(void)_HDMI_H15A0_GetAudioFreqFromTmdsClock(link_num, &samplingFreqFromTmds);

		//Get HDMI Audio Sampling Frequency from TMDS clock when audio sample is PCM
		if (audioType == LX_HDMI_AUDIO_PCM)
		{
			//Check a sampling frequency from status byte to TMDS clock
			//Channel status byte 0 value is 44.2Khz normal case or abnormal case.
			if ( (samplingFreq == LX_HDMI_SAMPLING_FREQ_44_1KHZ)
			   &&(samplingFreqFromTmds != LX_HDMI_SAMPLING_FREQ_44_1KHZ) )
			{
				//Check a HDMI Port Stable Count
				if (_gH15A0HdmiFreqErrorCount[link_num] < HDMI_AUDIO_FREQ_ERROR_TIME_500MS)
				{
					//Increase _gH15A0HdmiFreqErrorCount[link_num].
					spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
					_gH15A0HdmiFreqErrorCount[link_num]++;
					spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

					//Set a no audio mode
					audioType = LX_HDMI_AUDIO_NO_AUDIO;

					if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					{
						HDMI_ERROR("[%d]HDMI_GetAudioInfo : Freq. Error(link_num = %d, freq = %d, tmds freq = %d)!!!\n",	\
									__L__, link_num, samplingFreq, samplingFreqFromTmds);
					}

					goto func_exit;
				}
				else
				{
					if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					{
						HDMI_ERROR("[%d]HDMI_GetAudioInfo : Freq. Error => Force 48KHz(link_num = %d, freq = %d, tmds freq = %d)!!!\n",	\
									__L__, link_num, samplingFreq, samplingFreqFromTmds);
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

				if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
					HDMI_ERROR("[%d]HDMI_GetAudioInfo : TMDS Fs Error(link_num = %d, freq = %d)!!!\n", __L__, link_num, samplingFreqFromTmds);

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

			if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
				HDMI_ERROR("[%d]HDMI_GetAudioInfo : TMDS Fs Error(link_num = %d, freq =%d)!!!\n", __L__, link_num, samplingFreqFromTmds);

			goto func_exit;
		}

		//For debug print
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
		{
			HDMI_AUDIO("[%d]GetAudioInfo : link_num = %d, type = %d, freq = %d, mute = %d, PSC = %d\n", \
						__L__, link_num, audioType, samplingFreq, _gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num]);
		}
	}
	else
	{
		//Set DVI mode
		audioType	 = LX_HDMI_AUDIO_DVI;
		samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;
	}

	//Check a audio and video stable status
	if ( (audioType	  == _gH15A0HdmiAudioInfo[link_num].audioType)
	  &&(samplingFreq == _gH15A0HdmiAudioInfo[link_num].samplingFreq)
	  /* &&(_gH15A0HdmiAudioInfo[link_num].audioType >= LX_HDMI_AUDIO_PCM) */
	  &&(intrVideoState >= HDMI_STATE_READ) )
	{
		//spin lock for protection : lock
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);

		//Set a _gH15A0IntrAudioState[link_num]
		_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_STABLE_STATE;

		//spin lock for protection : unlock
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		//Set a debug print status
		bDebugEnabled = TRUE;
	}

	//For debug print
	if ( (audioType != _gH15A0HdmiAudioInfo[link_num].audioType) || (samplingFreq != _gH15A0HdmiAudioInfo[link_num].samplingFreq) )
	{
		if ( ((audioType == LX_HDMI_AUDIO_DVI) || (audioType == LX_HDMI_AUDIO_NO_AUDIO))	\
		  &&((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0) )
		{
			//Set a debug print status
			bDebugEnabled = TRUE;
		}
	}

	//Check a debug print status
	if (bDebugEnabled == TRUE)
	{
		//Get a vaild Pc data for payload
		LINK_NUM_H15_RdFL(link_num, audio_09);
		LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_0, reg_burst_pc_0);

		HDMI_AUDIO("[%d]GetAudioInfo : link_num = %d,type = %d(%d), freq = %d, mute = %d, PSC = %d\n", \
					__L__, link_num,audioType, reg_burst_pc_0 & 0x1F, samplingFreq, _gH15A0AudioMuteState[link_num], _gH15A0HdmiPortStableCount[link_num]);
	}

func_exit:
	if (audioType == LX_HDMI_AUDIO_NO_AUDIO)
	{
		//Mute audio data
		muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
		muteCtrl.bAudioMute = TRUE;
		_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

		//Clears a global value for audio info.
		_gH15A0HdmiAudioInfo[link_num].audioType	  = LX_HDMI_AUDIO_NO_AUDIO;
		_gH15A0HdmiAudioInfo[link_num].samplingFreq = LX_HDMI_SAMPLING_FREQ_NONE;

		//Clear _gH15A0HdmiPortStableCount[link_num]
		_gH15A0HdmiPortStableCount[link_num] = 0;

		//Debug print
		if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
			HDMI_AUDIO("[%d]HDMI_GetAudioInfo : HDMI Audio Mute On(link_num = %d) !!!\n", __L__, link_num);
	}
	else
	{
		//Update a global value when audio info. is stable.
		_gH15A0HdmiAudioInfo[link_num].audioType	  = audioType;
		_gH15A0HdmiAudioInfo[link_num].samplingFreq = samplingFreq;
	}

	HDMI_ATASK("_H15A0_GetAudioInfo : link_num = %d, type = %d, freq = %d(A = %d, V = %d)\n", link_num,audioType, samplingFreq, intrAudioState, intrVideoState);
	return RET_OK;
}

/**
 *  _HDMI_H15A0_GetAudioTypeAndFreq
 *
 *  @return int
*/
static int _HDMI_H15A0_GetAudioTypeAndFreq(int link_num, LX_HDMI_AUDIO_TYPE_T *audioType, LX_HDMI_SAMPLING_FREQ_T *samplingFreq)
{
	int ret = RET_OK;

	UINT32	reg_hdmi_mode = 0;
	UINT32	reg_achst_byte0, reg_achst_byte3;
	UINT32	reg_burst_pc_0;

	//Check HDMI /DVI Mode, 0 : DVI, 1 : HDMI
	LINK_NUM_H15_RdFL(link_num, system_control_00);
	LINK_NUM_H15_Rd01(link_num, system_control_00, reg_hdmi_mode, reg_hdmi_mode);

	if (reg_hdmi_mode)
	{
		//Check if audio sample word used for other purposes than liner PCM.
		LINK_NUM_H15_RdFL(link_num, audio_07);
		LINK_NUM_H15_Rd01(link_num, audio_07, reg_achst_byte0, reg_achst_byte0);

		if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)	//bit 1, 0 : PCM, 1 : non-PCM
		{
			//Check a vaild Pc data for payload
			LINK_NUM_H15_RdFL(link_num, audio_09);
			LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_0, reg_burst_pc_0);

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
						*audioType = _gH15A0HdmiAudioInfo[link_num].audioType;
						if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
							HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_NULL(link_num = %d, audioType= %d)\n", link_num,*audioType);
						break;

					default:
						*audioType = LX_HDMI_AUDIO_NO_AUDIO;
						break;
				}

				//Debug print
				if ((reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK) == BURST_INFO_AUDIO_TYPE_PAUSE)
				{
					if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
						HDMI_DEBUG("_HDMI_H15A0_GetAudioTypeAndFreq : BURST_INFO_AUDIO_TYPE_PAUSE(link_num = %d, Pc = %d)!!!\n", link_num,reg_burst_pc_0);
				}
			}	//if ((reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK) == 0)
			else
			{
				*audioType = LX_HDMI_AUDIO_PCM;

				HDMI_ERROR("_HDMI_H15A0_GetAudioTypeAndFreq : Burst Info Error = %d(link_num = %d)\n", (reg_burst_pc_0 & BURST_INFO_PAYLOAD_ERROR_BIT_MASK), link_num);
			}
		}	//if (reg_achst_byte0 & HDMI_AUDIO_SAMPLE_NON_PCM_MASK)
		else
		{
			*audioType = LX_HDMI_AUDIO_PCM;

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
					switch(reg_burst_pc_0 & BURST_INFO_AUDIO_TYPE_BIT_MASK)	//bit 4 ~ 0
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
		}

		//Set Sampling frequency from IEC60958 Channel Status Byte 3
		LINK_NUM_H15_RdFL(link_num, audio_08);
		LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte3, reg_achst_byte3);

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

				HDMI_AUDIO("_HDMI_H15A0_GetAudioTypeAndFreq(Channel Status) : link_num = %d, samplingFreq = %d\n", link_num,*samplingFreq);
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

	HDMI_ATASK("_H15A0_GetAudioTypeAndFreq : link_num = %d, type = %d, freq = %d\n", link_num, *audioType, *samplingFreq);
	return ret;
}

/**
 *  _HDMI_H15A0_GetAudioFreqFromTmdsClock
 *
 *  @return int
*/
static int _HDMI_H15A0_GetAudioFreqFromTmdsClock(int link_num, LX_HDMI_SAMPLING_FREQ_T *samplingFreqFromTmds)
{
	int ret = RET_OK;

#ifndef H15_THREAD_READ_PHY_REG_VALUE
	UINT8		ui8TmdsClockHigh = 0;		//< HDMI measured clock value of TMDS clock for upper 8 bit
	UINT8		ui8TmdsClockLow  = 0;		//< HDMI measured clock value of TMDS clock for low 8 bit
#endif

	UINT16		ui16VActive;				//< HDMI Vertical Active Size
	UINT16		ui16HActive;				//< HDMI horizontal Active Size

	UINT64		ui64TmdsClock = 0;			//< HDMI measured clock value of TMDS clock
	UINT64		ui64AcrN = 0;				//< HDMI ACR N value
	UINT64		ui64AcrCts = 0;				//< HDMI ACR CTS value
	UINT64		ui64TmdsSamplingFreq = 0;	//< HDMI sampling frequency in source device from TMDS clock

	//Get measured frequency value of TMDS clock.
#ifdef H15_THREAD_READ_PHY_REG_VALUE
	ui64TmdsClock = (UINT64)(_gH15A0HDMIPhyInform.tmds_clock[link_num] ) * 10000;
#else
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		PHY_REG_H15B0_RdFL(tmds_freq_1);
		PHY_REG_H15B0_RdFL(tmds_freq_2);

		PHY_REG_H15B0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15B0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}
	else {
		PHY_REG_H15A0_RdFL(tmds_freq_1);
		PHY_REG_H15A0_RdFL(tmds_freq_2);

		PHY_REG_H15A0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15A0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}

	ui64TmdsClock = (UINT64)((ui8TmdsClockHigh << 8) | ui8TmdsClockLow) * 10000;
#endif

	//Get ACR N H/W value.
	LINK_NUM_H15_RdFL(link_num, audio_01);
	//LINK_NUM_H15_Rd01(link_num, audio_01, reg_acr_n_tx, ui64AcrN);
	ui64AcrN = LINK_NUM_H15_Rd(link_num, audio_01) & 0xFFFFF;		//20 bits

	//Get ACR CTS H/W value.
	LINK_NUM_H15_RdFL(link_num, audio_03);
	//LINK_NUM_H15_Rd01(link_num, audio_03, reg_acr_cts_tx, ui64AcrCts);
	ui64AcrCts = LINK_NUM_H15_Rd(link_num, audio_03) & 0xFFFFF;	//20 bits

	//Compute a sampling frequency from TMDS clock
	ui64TmdsSamplingFreq = ui64AcrN * ui64TmdsClock;

	//Check divide by zero value.
	if ( (ui64TmdsSamplingFreq > 0) && (ui64AcrCts > 0) )
	{
		do_div(ui64TmdsSamplingFreq, ui64AcrCts * 128);
	}

	//Check if a HDMI Clock is alive.
	if(ui64TmdsClock)
	{
		HDMI_AUDIO("Get Audio Clock : link = %d, TMDS = %10llu, N = %llu, CTS = %llu, Freq = %llu\n", \
					link_num, ui64TmdsClock, ui64AcrN, ui64AcrCts, ui64TmdsSamplingFreq);
	}

	//Check a HDMI Audio Debug Enabled Status.
	if(_gH15A0AudioDebugState[link_num] == TRUE)
	{
		//Get video vertical & horizontal active size
		LINK_NUM_H15_RdFL(link_num, video_05);
		LINK_NUM_H15_Rd01(link_num, video_05, reg_h_av, ui16HActive);
		LINK_NUM_H15_Rd01(link_num, video_05, reg_v_av, ui16VActive);

		HDMI_ERROR("[%d] %10llu N%llu C%llu F%llu H%d V%d\n", \
					link_num, ui64TmdsClock, ui64AcrN, ui64AcrCts, ui64TmdsSamplingFreq,	\
					ui16HActive, ui16HActive);
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

	HDMI_ATASK("Get Fs from TDMS clock : %llu => %d(link_num = %d)\n", ui64TmdsSamplingFreq, *samplingFreqFromTmds, link_num);
//	HDMI_ERROR("Get Fs from TDMS clock : %llu => %d\n", ui64TmdsSamplingFreq, *samplingFreqFromTmds);

	return ret;
}

/**
 *  _HDMI_H15A0_DebugAudioInfo
 *
 *  @return int
*/
static int _HDMI_H15A0_DebugAudioInfo(int link_num, LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo)
{
	int ret = RET_OK;

#ifndef H15_THREAD_READ_PHY_REG_VALUE
	UINT8		ui8TmdsClockHigh = 0;		//< HDMI measured clock value of TMDS clock for upper 8 bit
	UINT8		ui8TmdsClockLow = 0;		//< HDMI measured clock value of TMDS clock for low 8 bit
#endif

	UINT64		ui64TmdsClock = 0;			//< HDMI measured clock value of TMDS clock
	UINT64		ui64AcrN = 0;				//< HDMI ACR N value
	UINT64		ui64AcrCts = 0;				//< HDMI ACR CTS value
	UINT64		ui64TmdsSamplingFreq = 0;	//< HDMI sampling frequency in source device from TMDS clock

	UINT32		reg_achst_byte0, reg_achst_byte1, reg_achst_byte2, reg_achst_byte3, reg_achst_byte4;
	UINT32		reg_burst_pc_0, reg_burst_pc_1, reg_burst_pd_0, reg_burst_pd_1;

	UINT32		sampling_freq, ext_sampling_freq;
	UINT16		burst_info_pc = 0, burst_info_pd = 0;

	UINT8		infoFrameVersion;   ///< HDMI Audio InfoFrame Version
	UINT8		infoFrameLength;	///< HDMI Audio InfoFrame Length

	UINT8		infoFrameByte1;	  	///< HDMI Audio InfoFrame Bytes 1
	UINT8		infoFrameByte2;	  	///< HDMI Audio InfoFrame Bytes 2
	UINT8		infoFrameByte3;	  	///< HDMI Audio InfoFrame Bytes 3

	//Get measured frequency value of TMDS clock.
#ifdef H15_THREAD_READ_PHY_REG_VALUE
	ui64TmdsClock = (UINT64)(_gH15A0HDMIPhyInform.tmds_clock[link_num] ) * 10000;
#else
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		PHY_REG_H15B0_RdFL(tmds_freq_1);
		PHY_REG_H15B0_RdFL(tmds_freq_2);

		PHY_REG_H15B0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15B0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}
	else {
		PHY_REG_H15A0_RdFL(tmds_freq_1);
		PHY_REG_H15A0_RdFL(tmds_freq_2);

		PHY_REG_H15A0_Rd01(tmds_freq_1, tmds_freq, ui8TmdsClockHigh);
		PHY_REG_H15A0_Rd01(tmds_freq_2, tmds_freq, ui8TmdsClockLow);
	}
	ui64TmdsClock = (UINT64)((ui8TmdsClockHigh << 8) | ui8TmdsClockLow) * 10000;
#endif

	//Get ACR N H/W value.
	LINK_NUM_H15_RdFL(link_num, audio_01);
	//LINK_NUM_H15_Rd01(link_num, audio_01, reg_acr_n_tx, ui64AcrN);
	ui64AcrN = LINK_NUM_H15_Rd(link_num, audio_01) & 0xFFFFF;		//20 bits

	//Get ACR CTS H/W value.
	LINK_NUM_H15_RdFL(link_num, audio_03);
	//LINK_NUM_H15_Rd01(link_num, audio_03, reg_acr_cts_tx, ui64AcrCts);
	ui64AcrCts = LINK_NUM_H15_Rd(link_num, audio_03) & 0xFFFFF;	//20 bits

	//Compute a sampling frequency from TMDS clock
	ui64TmdsSamplingFreq = ui64AcrN * ui64TmdsClock;

	//Check divide by zero value.
	if ( (ui64TmdsSamplingFreq > 0) && (ui64AcrCts > 0) )
	{
		do_div(ui64TmdsSamplingFreq, ui64AcrCts * 128);
	}

	HDMI_DEBUG("Get Audio Clock : link = %d, TMDS = %10llu, N = %llu, CTS = %llu, Freq = %llu\n", \
				link_num, ui64TmdsClock, ui64AcrN, ui64AcrCts, ui64TmdsSamplingFreq);

	//Read  reg_achst_byte0 reg.
	LINK_NUM_H15_RdFL(link_num, audio_07);
	LINK_NUM_H15_Rd01(link_num, audio_07, reg_achst_byte0, reg_achst_byte0);

	//Check IEC60958 Channel Status Byte0
	HDMI_DEBUG("IEC60958 Channel Status Byte0 = 0x%X\n", reg_achst_byte0);

	//Read  reg_achst_byte1 ~ 4 reg.
	LINK_NUM_H15_RdFL(link_num, audio_08);
	LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte1, reg_achst_byte1);
	LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte2, reg_achst_byte2);
	LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte3, reg_achst_byte3);
	LINK_NUM_H15_Rd01(link_num, audio_08, reg_achst_byte4, reg_achst_byte4);

	//Check IEC60958 Channel Status Byte1 ~ 4
	HDMI_DEBUG("IEC60958 Channel Status Byte1 = 0x%X, Byte2 = 0x%X, Byte3 = 0x%X, Byte4 = 0x%X\n",	\
				reg_achst_byte1, reg_achst_byte2, reg_achst_byte3, reg_achst_byte4);


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
		LINK_NUM_H15_RdFL(link_num, audio_09);
		LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_0, reg_burst_pc_0);
		LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pc_1, reg_burst_pc_1);
		LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pd_0, reg_burst_pd_0);
		LINK_NUM_H15_Rd01(link_num, audio_09, reg_burst_pd_1, reg_burst_pd_1);

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

	//Read reg_pkt_aif_hdr_0 ~ 1, reg_pkt_aif_dat_0 ~ 3 reg.
	LINK_NUM_H15_RdFL(link_num, packet_09);
	LINK_NUM_H15_RdFL(link_num, packet_10);
	LINK_NUM_H15_Rd01(link_num, packet_09, reg_pkt_aif_hdr_0, infoFrameVersion);
	LINK_NUM_H15_Rd01(link_num, packet_09, reg_pkt_aif_hdr_1, infoFrameLength);
	LINK_NUM_H15_Rd01(link_num, packet_10, reg_pkt_aif_dat_0, infoFrameByte1);
	LINK_NUM_H15_Rd01(link_num, packet_10, reg_pkt_aif_dat_1, infoFrameByte2);
	LINK_NUM_H15_Rd01(link_num, packet_10, reg_pkt_aif_dat_2, infoFrameByte3);

	//Check HDMI Audio InfoFrame Header 0 ~ 1, Byte0 ~ 3
	HDMI_DEBUG("Audio InfoFrame Header0 = 0x%X, Header1 = 0x%X, Byte1 = 0x%X, Byte2 = 0x%X, Byte3 = 0x%X\n",	\
				infoFrameVersion, infoFrameLength, infoFrameByte1, infoFrameByte2, infoFrameByte3);

	if (infoFrameVersion == HDMI_INFO_AUDIO_VERSION_NUM)
		HDMI_DEBUG("Audio InfoFrame Version is 0x%X.\n", infoFrameVersion);
	else
		HDMI_DEBUG("Audio InfoFrame Version is not 0x%X -> 0x%X!!!\n", HDMI_INFO_AUDIO_VERSION_NUM, infoFrameVersion);

	if (infoFrameLength == HDMI_INFO_AUDIO_LENGTH)
		HDMI_DEBUG("Audio InfoFrame Length  is 0x%X.\n", infoFrameLength);
	else
		HDMI_DEBUG("Audio InfoFrame Length  is not 0x%X -> 0x%X!!!\n", HDMI_INFO_AUDIO_LENGTH, infoFrameLength);

	// HDMI Audio Coding-Types according to CEA-861-E Audio Infoframe.
	switch((infoFrameByte1 >> HDMI_INFO_AUDIO_TYPE_BIT_SHIFT) & HDMI_INFO_AUDIO_TYPE_BIT_MASK) 	//Bit 4 ~ 7, CT3 ~ CT0, Data Byte 1
	{
		case HDMI_INFO_AUDIO_TYPE_REFER_HEADER:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_REFER_HEADER\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_PCM:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_PCM\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_AC3:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_AC3\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_MPEG_1:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_MPEG_1\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_MP3:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_MP3\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_MPEG2:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_MPEG2\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_AAC_LC:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_AAC_LC\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_DTS:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_DTS\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_ATRAC:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_ATRAC\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_DSD:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_DSD\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_E_AC_3:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_E_AC_3\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_DTS_HD:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_DTS_HD\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_MLP:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_MLP\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_DST:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_DST\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_WMA_PRO:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_WMA_PRO\n");
			break;

		case HDMI_INFO_AUDIO_TYPE_REFER_EXT:
			HDMI_DEBUG("HDMI_INFO_AUDIO_TYPE_REFER_EXT\n");

			// define HDMI Audio Coding-Types according to CEA-861-E Audio Infoframe.
			switch((infoFrameByte3 >> HDMI_INFO_AUDIO_EXT_TYPE_BIT_SHIFT) & HDMI_INFO_AUDIO_EXT_TYPE_BIT_MASK)	//Bit 0 ~ 4, CXT4 ~ CXT0, Data Byte 3
			{
				case HDMI_INFO_AUDIO_EXT_TYPE_REFER_CT:
					HDMI_DEBUG("HDMI_INFO_AUDIO_EXT_TYPE_REFER_CT\n");
					break;

				case HDMI_INFO_AUDIO_EXT_TYPE_HE_AAC:
					HDMI_DEBUG("HDMI_INFO_AUDIO_EXT_TYPE_HE_AAC\n");
					break;

				case HDMI_INFO_AUDIO_EXT_TYPE_HE_AACV2:
					HDMI_DEBUG("HDMI_INFO_AUDIO_EXT_TYPE_HE_AACV2\n");
					break;

				case HDMI_INFO_AUDIO_EXT_TYPE_MPEG_SUR:
					HDMI_DEBUG("HDMI_INFO_AUDIO_EXT_TYPE_MPEG_SUR\n");
					break;

				default:
					HDMI_DEBUG("AUDIO_EXT_TYPE_BIT = %d\n", (infoFrameByte3 >> HDMI_INFO_AUDIO_EXT_TYPE_BIT_SHIFT));
					break;
			}
			break;

		default:
			HDMI_DEBUG("AUDIO_TYPE_BIT = %d\n", (infoFrameByte1 >> HDMI_INFO_AUDIO_TYPE_BIT_SHIFT));
			break;
	}

	// HDMI Audio Sample Frequency according to CEA-861-E Audio Infoframe.
	switch((infoFrameByte2 >> HDMI_INFO_AUDIO_SAMPLE_BIT_SHIFT) & HDMI_INFO_AUDIO_SAMPLE_BIT_MASK) 	//Bit 4 ~ 2, SF2 ~ SF0, Data Byte 2
	{
		case HDMI_INFO_AUDIO_SAMPLE_REFER_HEADER:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_FREQ_REFER_HEADER\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_32_KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_32_KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_44_1KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_44_1KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_48_KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_48_KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_88_2KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_88_2KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_96_KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_96_KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_176_4KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_176_4KHZ\n");
			break;

		case HDMI_INFO_AUDIO_SAMPLE_192_KHZ:
			HDMI_DEBUG("HDMI_INFO_AUDIO_SAMPLE_192_KHZ\n");
			break;

		default:
			HDMI_DEBUG("AUDIO_SAMPLE_FREQ_BIT = %d\n", (infoFrameByte2 >> HDMI_INFO_AUDIO_SAMPLE_BIT_SHIFT));
			break;
	}

	if ((infoFrameByte1 & 0x07) == 0)
		HDMI_DEBUG("Audio Channel Count is Refer to Stream Header.\n");
	else
		HDMI_DEBUG("Audio Channel Count is %d channels.\n", (infoFrameByte1 & 0x07) + 1);

	if ((infoFrameByte2 & 0x03) == 0)
		HDMI_DEBUG("Audio Sample Size is Refer to Stream Header.\n");
	else if ((infoFrameByte2 & 0x03) == 1)
		HDMI_DEBUG("Audio Sample Size is 16 bit.\n");
	else if ((infoFrameByte2 & 0x03) == 2)
		HDMI_DEBUG("Audio Sample Size is 20 bit.\n");
	else
		HDMI_DEBUG("Audio Sample Size is 24 bit.\n");

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

	//Copy a HDMI Audio InfoFrame Header 0 ~ 1, Byte0 debug information.
	pAudioDebugInfo->infoFrameVersion = infoFrameVersion;
	pAudioDebugInfo->infoFrameLength  = infoFrameLength;

	//Copy a HDMI Audio InfoFrame Byte1 ~ 3 debug information.
	pAudioDebugInfo->infoFrameByte1	  = infoFrameByte1;
	pAudioDebugInfo->infoFrameByte2	  = infoFrameByte2;
	pAudioDebugInfo->infoFrameByte2	  = infoFrameByte2;

	return ret;
}

static int _HDMI_H15A0_Delayed_Device_Init(void)
{
	int	count, port_num;
	int cd_sense;
	int hdmi5v[2];
	//*  2012-06-15 : HDMI Initail ÁÖÀÇ »çÇ×
	//*  Ctop reset ÀÌÈÄ phy pdb & restn µ¿ÀÛ ÇØ¾ß ÇÔ(Ctop ¿¡¼­ phy pdb¸¸ ÇÏ´Â °Í °°À½.
	//*  After phy pdb, phy register initi value °¡ ¿À¿°µÊ -> pdb ÈÄ¿¡´Â rstn resetÇØÁÖ¾î¾ß ÇÔ.
	//*  Phy pdb & restn ÇÏ¸é phy register initi value·Î º¯°æ µÊ,  ´Ù½Ã phy register setting ÇØ¾ß ÇÔ.
	//*
	HDMI_DEBUG("[%s] Entered \n",__func__);

	_gMHL_RCP_RCV_MSG.rcp_receive_cmd = HDMI_RCP_RECEIVE_CMD_NONE;
	_gMHL_RCP_RCV_MSG.rcp_receive_flag = FALSE;
	_gMHL_RCP_RCV_MSG.rcp_msg = 0x7F;

	_HDMI_H15A0_ClearSWResetAll();			//H15A0D Chip - Ctop control

	/* Temporal Code for HDMI 1.4 port : Not to run HDCP 2.2 */
#if 1
	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		UINT32 value = 0x2;
		HDCP22_NUM_H15_RdFL(0, ad_hdcp22_control);
		HDCP22_NUM_H15_Wr(0, ad_hdcp22_control, 0x410002);
		HDCP22_NUM_H15_WrFL(0, ad_hdcp22_control);
		HDCP22_NUM_H15_RdFL(1, ad_hdcp22_control);
		HDCP22_NUM_H15_Wr(1, ad_hdcp22_control, 0x410002);
		HDCP22_NUM_H15_WrFL(1, ad_hdcp22_control);

#ifdef USE_EXT_MHL_IC
		// Disable HDCP1.4 AND 2.2 in case of using External MHL IC
		HDCP22_NUM_H15_RdFL(EXT_MHL_IC_LINK_NUM, ad_hdcp22_enable);
		HDCP22_NUM_H15_Wr01(EXT_MHL_IC_LINK_NUM, ad_hdcp22_enable, o_reg_hdcp_sw_enable, 0x0);
		HDCP22_NUM_H15_WrFL(EXT_MHL_IC_LINK_NUM, ad_hdcp22_enable);
#endif
	}
#endif

	for (port_num = 0; port_num < 2;port_num ++) {
		_HDMI_H15A0_Set_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
		if(port_num == 0) {
			GPIO_DevSetPinMux(39, TRUE);
			GPIO_DevSetPinMux(35, TRUE);
			/* GPIO mode set before value setting. */
			GPIO_DevSetMode(39, LX_GPIO_MODE_OUTPUT);
			GPIO_DevSetMode(35, LX_GPIO_MODE_OUTPUT);
			_HDMI_H15A0_Set_GPIO_HPD_Out(port_num, !_gH15A0HDMIPhyInform.hpd_pol[port_num]);	// disable HPD
		}
#ifdef USE_EXT_MHL_IC
		if(port_num != EXT_MHL_IC_LINK_NUM)
#endif
			_gH15A0HDMIPhyInform.hpd_hdcp22_control[port_num] = 0;
	}
#ifdef USE_EXT_MHL_IC
	_gH15A0HDMIPhyInform.hpd_hdcp22_control[1] = 1;
#endif

	OS_MsecSleep(10);	// ms delay

	if(_HDMI_H15A0_Phy_PDB_WA > 0)
	{
		for (port_num = 0; port_num < 2;port_num ++) {
			LINK_NUM_H15_RdFL(port_num, phy_00);
			LINK_NUM_H15_Wr01(port_num, phy_00, phy_pdb_man_mode, 0x1);			//PHY PDB Manual Mode On
			LINK_NUM_H15_WrFL(port_num, phy_00);
		}
	}

	_HDMI_H15A0_Get_HDMI5V_Info_A4P(&hdmi5v[0], &hdmi5v[1]);

	_g_ODT_PDB_Off_Time = 0;

	for (port_num = 0; port_num < 2;port_num ++)
	{
		if(hdmi5v[port_num])
		{
#ifdef ODT_PDB_OFF_ON_WORKAROUND
			if(_HDMI_H15A0_Phy_PDB_WA == 0)
			{
				_HDMI_H15A0_Phy_Enable_Register_Access(port_num);
				_HDMI_H15A0_EQ_PDB_control(1);
			}
#endif
			_g_ODT_PDB_Off_Time = jiffies_to_msecs(jiffies);
		}
	}

	if( _g_ODT_PDB_Off_Time > 0 ) {
		for(count =0; count < 20; count ++)
		{
			_g_ODT_PDB_On_Time = jiffies_to_msecs(jiffies);
			if ( ( (_g_ODT_PDB_On_Time - _g_ODT_PDB_Off_Time) > 800 ) && (_gH15A0HDMIPhyInform.hpd_enable == 1) )
				break;
			OS_MsecSleep(100);	// ms delay
		}
	}

	for (port_num = 0; port_num < 2;port_num ++)
	{
		if(hdmi5v[port_num])
		{
#ifdef ODT_PDB_OFF_ON_WORKAROUND
			if(_HDMI_H15A0_Phy_PDB_WA == 0)
			{
				_HDMI_H15A0_Phy_Enable_Register_Access(port_num);
				_HDMI_H15A0_EQ_PDB_control(0);
			}
#endif
		}
	}

	_HDMI_H15A0_PhyOff(0);
	_HDMI_H15A0_PhyOff(1);

	LINK_NUM_H15_RdFL(1, system_control_00);
	LINK_NUM_H15_Rd01(1, system_control_00, reg_cd_sense, cd_sense);

	if (cd_sense)
		_HDMI_H15A0_Set_ManMHLMode(1, 1, 0);

	//_HDMI_H15A0_Set_ManMHLMode(1, 0);
	_HDMI_H15A0_ResetPortControl(1);	// reset phy_link

	/* PHY_PDB=1, PHY_RSTN=1 À» µ¿½Ã¿¡ ÀÎ°¡ registerµéÀÌ Á¦´ë·Î pre-setµÇÁö ¾Ê´Â Çö»ó ¹ß»ýÇÔ. */
	/* PHY_PDB=1  -> 10us  -> PHY_RSTN=1  */
	OS_MsecSleep(2);	// ms delay

	//_HDMI_H15A0_PhyRunReset();	// init Phy register
	_HDMI_H15A0_RunReset();		// init Link register

	//audio setting for port0 and port1
	_HDMI_H15A0_SetAudio(HDMI_LINK_0);
	_HDMI_H15A0_SetAudio(HDMI_LINK_1);

	_HDMI_H15A0_ResetPortControl(0);	// Release reset phy_link

	_HDMI_H15A0_EnableInterrupt();

	LINK_NUM_H15_RdFL(0, phy_00);
	if (LINK_NUM_H15_Rd_fld(0, phy_00, hpd_100k_en) || LINK_NUM_H15_Rd_fld(0, phy_00, hpd_1k_en) || LINK_NUM_H15_Rd_fld(0, phy_00, hpd_out_oen))
		HDMI_NOTI(" ########## H15 Register BUG [0] !!!! [0x%x] ##################\n", LINK_NUM_H15_Rd(0, phy_00) );
	if (LINK_NUM_H15_Rd_fld(1, phy_00, hpd_100k_en) || LINK_NUM_H15_Rd_fld(1, phy_00, hpd_1k_en) || LINK_NUM_H15_Rd_fld(1, phy_00, hpd_out_oen))
		HDMI_NOTI(" ########## H15 Register BUG [1] !!!! [0x%x] ##################\n", LINK_NUM_H15_Rd(1, phy_00) );

	/*
	if ( spHDMI_H15A0_Status == NULL )
	{
		HDMI_ERROR("ERROR : driver status\n");
	}
	*/
	/*
	_HDMI_H15A0_EnablePort(3, 1);	// Enable MHL Port

	OS_MsecSleep(10); // delay after enable port and phy register write


	_HDMI_H15A0_EnablePort(3, 0);
	*/
	_g_HDMI_Device_Init_Completed = 1;

	return RET_OK;
}

static int _HDMI_H15A0_Thread(void *data)
{

	ULONG	flags = 0;

	_HDMI_H15A0_Delayed_Device_Init();

	while(1)
	{
		if( sHDMI_H15A0_Status.device_in_suspend_mode > 0) {
			goto hdmi_thread_skip;
		}
	/*
		if ( spHDMI_H15A0_Status == NULL )
		{
			HDMI_ERROR("ERROR : driver status\n");
		}
		*/
		OS_LockMutex(&g_HDMI_Sema);

		_HDMI_H15A0_GetMHLConection();	//check MHL Connection
		_HDMI_H15A0_GetPortConnection();	//check connection
		_HDMI_H15A0_Periodic_Task(0);	//Video Task
		_HDMI_H15A0_Periodic_Task(1);	//Video Task
		_HDMI_H15A0_GetAudioInfo(HDMI_LINK_0);	//Audio Task for HDMI Link 0
		_HDMI_H15A0_GetAudioInfo(HDMI_LINK_1);	//Audio Task for HDMI Link 1
		_HDMI_H15A0_HDCP_Check_Status();	// check HDCP status

		OS_UnlockMutex(&g_HDMI_Sema);

hdmi_thread_skip:

		if ( _gH15A0HDMIPhyInform.hdmi5v[0] || _gH15A0HDMIPhyInform.hdmi5v[1] )
			OS_MsecSleep(HDMI_THREAD_TIMEOUT);
		else
		{
			_gHdmi_no_connection_count ++;
			OS_MsecSleep(HDMI_THREAD_TIMEOUT << 2);

		}

		//	HDMI_TASK("HDMI Thread\n");

		if (_gH15A0Force_thread_sleep > 1)
		{
			HDMI_NOTI("Force sleep HDMI Thread\n");
			//_gH15A0Force_thread_sleep  = 1;
			_gH15A0HDMI_thread_running = 0;
			//interruptible_sleep_on(&WaitQueue_HDMI_H15A0);
			wait_event_interruptible(WaitQueue_HDMI_H15A0 , _gH15A0Force_thread_sleep == 0);
		}

		if (_gHdmi_no_connection_count > 10)
		{
			spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
			_gHdmi_no_connection_count = 0;
			_gH15A0Force_thread_sleep  = 1;
			_gH15A0HDMI_thread_running = 0;
			sHDMI_H15A0_Status.HDMIState[0] = HDMI_STATE_DISABLE;
			sHDMI_H15A0_Status.HDMIState[1] = HDMI_STATE_DISABLE;
			spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

			HDMI_NOTI("No HDMI connection : sleep thread\n");
			/*
			   if ( _gH15A0HDMIState == HDMI_STATE_DISABLE )
			   {
			   interruptible_sleep_on(&WaitQueue_HDMI_H15A0);
			   }
			   else
			 */

			if( wait_event_interruptible(WaitQueue_HDMI_H15A0 ,\
						(sHDMI_H15A0_Status.HDMIState[0] > HDMI_STATE_DISABLE) && (sHDMI_H15A0_Status.HDMIState[1] > HDMI_STATE_DISABLE)  ) < 0)
			{
				HDMI_NOTI("HDMI thread not sleeping : wakeup by interrupt\n");
			}
		}
	}
	return RET_OK;
}

static int _HDMI_H15A0_EnableInterrupt(void)
{
	int link_num;

	for (link_num = 0; link_num < 2;link_num ++)
	{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_00);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_avi_enable, 0x0);			///< 0 intr_new_avi_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_vsi_enable, 0x0);			///< 1 intr_new_vsi_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_chg_vsi_vformat_enable, 0x1);	///< 2 intr_chg_vsi_vformat_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_gcp_enable, 0x0);			///< 3 intr_new_gcp_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_asp_enable, 0x0);			///< 4 intr_new_asp_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_acr_enable, 0x0);			///< 5 intr_new_acr_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_gbd_enable, 0x0);			///< 6 intr_new_gbd_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_acp_enable, 0x0);			///< 7 intr_new_acp_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_isrc1_enable, 0x0);			///< 8 intr_new_isrc1_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_isrc2_enable, 0x0);			///< 9 intr_new_isrc2_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_mpg_enable, 0x0);			///< 10 intr_new_mpg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_aud_enable, 0x0);			///< 11 intr_new_aud_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_spd_enable, 0x0);			///< 12 intr_new_spd_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_new_unr_enable, 0x0);			///< 13 intr_new_unr_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_cts_chg_enable, 0x0);			///< 14 intr_cts_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_n_chg_enable, 0x0);				///< 15 intr_n_chg_enable
#ifdef H15_DISABLE_AUDIO_INTERRUPT
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_fs_chg_enable, 0x0);				///< 16 intr_fs_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2pcm_chg_enable, 0x0);			///< 17 intr_2pcm_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2npcm_chg_enable, 0x0);			///< 18 intr_2npcm_chg_enable
#else
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_fs_chg_enable, 0x1);				///< 16 intr_fs_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2pcm_chg_enable, 0x1);			///< 17 intr_2pcm_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_2npcm_chg_enable, 0x1);			///< 18 intr_2npcm_chg_enable
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_aud_type_chg_enable, 0x0);			///< 19 intr_aud_type_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_spdif_err_enable, 0x0);			///< 19 intr_spdif_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_afifo_undr_enable, 0x0);			///< 20 intr_afifo_undr_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_afifo_ovrr_enable, 0x0);			///< 21 intr_afifo_ovrr_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_burst_info_enable, 0x1);			///< 22 intr_burst_info_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_vf_stable_enable, 0x1);			///< 23 intr_vf_stable_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_vid_chg_enable, 0x0);			///< 24 intr_vid_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_vr_chg_enable, 0x0);				///< 25 intr_vr_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_hr_chg_enable, 0x0);				///< 26 intr_hr_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_po_chg_enable, 0x0);				///< 27 intr_po_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_il_chg_enable, 0x0);				///< 28 intr_il_chg_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_vfifo_undr_enable, 0x0);			///< 29 intr_vfifo_undr_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_00, intr_vfifo_ovrr_enable, 0x0);			///< 30 intr_vfifo_ovrr_enable
		//intr_achst_5b_chg_int_enable is not working, so disabled.
		//no intr_achst_5b_chg_int_enable register in M14
		//LINK_NUM_H15_Wr01(0, interrupt_enable_00, intr_achst_5b_chg_int_enable, 0x0);	///< 21 intr_achst_5b_chg_int_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_00);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_cbus_cmd_msc_err_enable,	0x0);		///< 0 intr_cbus_cmd_msc_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_plug_out_enable, 		0x0);		///< 1 intr_plug_out_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_plug_in_enable, 			0x0);		///< 2 intr_plug_in_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdmi5v_fall_enable, 		0x0);		///< 3 intr_hdmi5v_fall_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdmi5v_rise_enable, 		0x0);		///< 4 intr_hdmi5v_rise_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_fall_enable, 		0x0);		///< 5 intr_scdt_fall_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_rise_enable, 		0x0);		///< 6 intr_scdt_rise_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdmi_mode_enable, 		0x0);		///< 7 intr_hdmi_mode_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_set_mute_enable, 		0x1);		///< 8 intr_set_mute_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_clr_mute_enable, 		0x0);		///< 9 intr_clr_mute_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_vsync_enable, 			0x0);		///< 10 intr_vsync_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdmi5v_fedge_enable,		0x0);		///< 11 intr_hdmi5v_fedge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdmi5v_redge_enable, 	0x1);		///< 12 intr_hdmi5v_redge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_fedge_enable, 		0x1);		///< 13 intr_scdt_fedge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_redge_enable, 		0x1);		///< 14 intr_scdt_redge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_cd_sense_fedge_enable, 	0x0);		///< 15 intr_cd_sense_fedge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_cd_sense_redge_enable, 	0x0);		///< 16 intr_cd_sense_redge_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_auth_init_enable, 		0x1);		///< 17 intr_auth_init_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_auth_done_enable, 		0x1);		///< 18 intr_auth_done_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdcp_err_enable, 		0x0);		///< 19 intr_hdcp_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_ecc_err_enable, 			0x0);		///< 20 intr_ecc_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_terc4_err_enable, 		0x0);		///< 21 intr_terc4_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_acr_err_enable, 			0x0);		///< 22 intr_acr_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_asp_err_enable, 			0x0);		///< 23 intr_asp_err_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_avi_enable, 			0x0);		///< 24 intr_no_avi_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_vsi_enable, 			0x0);		///< 25 intr_no_vsi_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_gcp_enable, 			0x0);		///< 26 intr_no_gcp_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_spd_enable, 			0x0);		///< 27 intr_no_spd_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_chg_avi_b12345_enable, 	0x1);		///< 28 intr_chg_avi_b12345_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_chg_avi_to_rgb_enable, 	0x1);		///< 29 intr_chg_avi_to_rgb_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_chg_avi_to_ycc444_enable, 0x1);		///< 30 intr_chg_avi_to_ycc444_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_chg_avi_to_ycc422_enable, 0x1);		///< 31 intr_chg_avi_to_ycc422_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	}
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_02);
#endif
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_chg_24mode_enable,		0x0);	///<  0 intr_cbus_chg_24mode_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_pathen_set_enable, 		0x0);	///<  1 intr_cbus_pathen_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_dscr_enable, 		0x0);	///<  2 intr_cbus_new_dscr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_intstat_enable, 	0x0);	///<  3 intr_cbus_new_intstat_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_dcap_enable, 		0x0);	///<  4 intr_cbus_new_dcap_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_ucp_enable, 		0x0);	///<  5 intr_cbus_new_ucp_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_ucp_enable, 		0x0);	///<  6 intr_cbus_cmd_ucp_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_pathen_clr_enable, 		0x0);	///<  7 intr_cbus_pathen_clr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_mute_set_enable, 		0x0);	///<  8 intr_cbus_mute_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_mute_clr_enable, 		0x0);	///<  9 intr_cbus_mute_clr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_mscerr_enable, 		0x0);	///< 10 intr_cbus_new_mscerr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_new_rcp_enable, 		0x1);	///< 11 intr_cbus_new_rcp_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_set_hpd_enable, 	0x0);	///< 12 intr_cbus_cmd_set_hpd_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_get_state_enable, 	0x0);	///< 13 intr_cbus_cmd_get_state_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_get_vdid_enable, 	0x0);	///< 14 intr_cbus_cmd_get_vdid_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rd_devcap_enable, 	0x0);	///< 15 intr_cbus_cmd_rd_devcap_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rd_devintr_enable, 	0x0);	///< 16 intr_cbus_cmd_rd_devintr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_pathen_set_enable, 	0x0);	///< 17 intr_cbus_cmd_pathen_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_pathen_clr_enable, 	0x0);	///< 18 intr_cbus_cmd_pathen_clr_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_dcaprd_set_enable, 	0x0);	///< 19 intr_cbus_cmd_dcaprd_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_3dreq_set_enable, 	0x0);	///< 20 intr_cbus_cmd_3dreq_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_grtwrt_set_enable, 	0x0);	///< 21 intr_cbus_cmd_grtwrt_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_reqwrt_set_enable, 	0x1);	///< 22 intr_cbus_cmd_reqwrt_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_wrt_burst_enable, 	0x0);	///< 23 intr_cbus_cmd_wrt_burst_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_dscrchg_set_enable, 0x0);	///< 24 intr_cbus_cmd_dscrchg_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_dcapchg_set_enable, 0x0);	///< 25 intr_cbus_cmd_dcapchg_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_edidchg_set_enable, 0x0);	///< 26 intr_cbus_cmd_edidchg_set_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_clr_hpd_enable, 	0x0);	///< 27 intr_cbus_cmd_clr_hpd_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rap_poll_enable, 	0x0);	///< 28 intr_cbus_cmd_rap_poll_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rap_on_enable, 		0x0);	///< 29 intr_cbus_cmd_rap_on_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rap_off_enable, 	0x0);	///< 30 intr_cbus_cmd_rap_off_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_02, intr_cbus_cmd_rcp_enable, 		0x1);	///< 31 intr_cbus_cmd_rcp_enable
	LINK_NUM_H15_WrFL(1, interrupt_enable_02);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_03);
#endif
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_ready_enable, 			0x0);	///<  0 intr_cbus_ready_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_disconn_enable, 		0x1);	///<  1 intr_cbus_disconn_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_max_nack_enable, 		0x0);	///<  2 intr_cbus_max_nack_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_max_retx_enable, 		0x0);	///<  3 intr_cbus_max_retx_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_max_rerx_enable, 		0x1);	///<  4 intr_cbus_max_rerx_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_new_devstate_enable, 	0x0);	///<  5 intr_cbus_new_devstate_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_new_vdid_enable, 		0x0);	///<  6 intr_cbus_new_vdid_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_new_devcap_enable, 		0x0);	///<  7 intr_cbus_new_devcap_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_chg_dscr_src_enable, 	0x0);	///<  8 intr_cbus_chg_dscr_src_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_chg_dscr_sink_enable, 	0x0);	///<  9 intr_cbus_chg_dscr_sink_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_chg_ppmode_enable, 		0x0);	///< 10 intr_cbus_chg_ppmode_enable
	LINK_NUM_H15_Wr01(1, interrupt_enable_03, intr_cbus_new_rap_enable, 		0x0);	///< 11 intr_cbus_new_rap_enable
	LINK_NUM_H15_WrFL(1, interrupt_enable_03);

	if (_gH15A0HDMI_interrupt_registered < 1)
	{
		if (request_irq(H15_IRQ_HDMI_LINK, _HDMI_H15A0_IRQHandler, 0, "hdmi", NULL))
			HDMI_ERROR("request_irq is failed\n");
		else
			_gH15A0HDMI_interrupt_registered = 1;
	}

	HDMI_NOTI("HDMI_H15A0_EnableInterrupt\n");
	return RET_OK;
}

int HDMI_H15A0_DisableInterrupt(void)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(0, interrupt_enable_00);
#endif
	LINK_NUM_H15_Wr(0, interrupt_enable_00, 0x0);
	LINK_NUM_H15_WrFL(0, interrupt_enable_00);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(0, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr(0, interrupt_enable_01, 0x0);
	LINK_NUM_H15_WrFL(0, interrupt_enable_01);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(0, interrupt_enable_02);
#endif
	LINK_NUM_H15_Wr(0, interrupt_enable_02, 0x0);
	LINK_NUM_H15_WrFL(0, interrupt_enable_02);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(0, interrupt_enable_03);
#endif
	LINK_NUM_H15_Wr(0, interrupt_enable_03, 0x0);
	LINK_NUM_H15_WrFL(0, interrupt_enable_03);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_00);
#endif
	LINK_NUM_H15_Wr(1, interrupt_enable_00, 0x0);
	LINK_NUM_H15_WrFL(1, interrupt_enable_00);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr(1, interrupt_enable_01, 0x0);
	LINK_NUM_H15_WrFL(1, interrupt_enable_01);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_02);
#endif
	LINK_NUM_H15_Wr(1, interrupt_enable_02, 0x0);
	LINK_NUM_H15_WrFL(1, interrupt_enable_02);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(1, interrupt_enable_03);
#endif
	LINK_NUM_H15_Wr(1, interrupt_enable_03, 0x0);
	LINK_NUM_H15_WrFL(1, interrupt_enable_03);

	//free_irq(H15_IRQ_HDMI, NULL);
	HDMI_NOTI("HDMI_H15A0_DisableInterrupt\n");

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
irqreturn_t _HDMI_H15A0_IRQHandler(int irq, void *dev)
{
	UINT32 intr_link0 = 0;	//interrupt from link 0
	UINT32 intr_link1 = 0;	//interrupt from link 1

	LINK_TOP_H15_RdFL(link_top_intr_01);
	LINK_TOP_H15_Rd02(link_top_intr_01, link_apb_intr_prt0, intr_link0, link_apb_intr_prt1, intr_link1);

	HDMI_INTR("--- LINK0 [0x%x] , LINK1 [0x%x] ---\n",intr_link0, intr_link1);

	if(intr_link0)
		_HDMI_H15A0_IRQHandler_Link(0);
	if(intr_link1)
		_HDMI_H15A0_IRQHandler_Link(1);

	return IRQ_HANDLED;
}

static void _HDMI_H15A0_IRQHandler_Link(int link_num)
{
	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 		muteCtrl = {FALSE, FALSE, LX_HDMI_MUTE_NONE};
	LX_HDMI_AVI_COLORSPACE_T	csc = LX_HDMI_AVI_COLORSPACE_RGB;

	UINT32 intra0 = 0;
	UINT32 intra1 = 0;
	UINT32 intra2 = 0;
	UINT32 intra3 = 0;

	UINT32 intr_afifo_undr;  	//    21
	UINT32 intr_afifo_ovrr;  	//    22
	UINT32 intr_spdif_err;  	//    20
	UINT32 intr_aud_type_chg;   //    19
	UINT32 intr_cts_chg = 0;  	//    14
	UINT32 intr_n_chg;  		//    15
	UINT32 intr_fs_chg;  		//    16
	UINT32 intr_2pcm_chg;  		//    17
	UINT32 intr_2npcm_chg;  	//    18

	UINT32 intr_ecc_err;  		//    20
	UINT32 intr_terc4_err;  	//    21

	UINT64	elapsedTime = 0;

	LINK_NUM_H15_RdFL(link_num, interrupt_00);
	intra0 = LINK_NUM_H15_Rd(link_num, interrupt_00);

	LINK_NUM_H15_RdFL(link_num, interrupt_01);
	intra1 = LINK_NUM_H15_Rd(link_num, interrupt_01);

	LINK_NUM_H15_RdFL(link_num, interrupt_02);
	intra2 = LINK_NUM_H15_Rd(link_num, interrupt_02);

	LINK_NUM_H15_RdFL(link_num, interrupt_03);
	intra3 = LINK_NUM_H15_Rd(link_num, interrupt_03);

	//Check a HDMI Audio Debug Enabled Status.
	if(_gH15A0AudioDebugState[link_num] == TRUE)
	{
		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_afifo_undr))
			intr_afifo_undr = 1;
		else
			intr_afifo_undr = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_afifo_ovrr))
			intr_afifo_ovrr = 1;
		else
			intr_afifo_ovrr = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_spdif_err))
			intr_spdif_err = 1;
		else
			intr_spdif_err = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_aud_type_chg))
			intr_aud_type_chg = 1;
		else
			intr_aud_type_chg = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_cts_chg))
		{
			_gH15A0CtsInterruptCount[link_num]++;

			if ((_gH15A0CtsInterruptCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_10S) == 0)
				intr_cts_chg = 1;
		}
		else
		{
			intr_cts_chg = 0;
		}

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_n_chg))
			intr_n_chg = 1;
		else
			intr_n_chg = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_fs_chg))
			intr_fs_chg = 1;
		else
			intr_fs_chg = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2pcm_chg))
			intr_2pcm_chg = 1;
		else
			intr_2pcm_chg = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2npcm_chg))
			intr_2npcm_chg = 1;
		else
			intr_2npcm_chg = 0;


		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_ecc_err))
			intr_ecc_err = 1;
		else
			intr_ecc_err = 0;

		if(LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_terc4_err))
			intr_terc4_err = 1;
		else
			intr_terc4_err = 0;

		if(intr_afifo_undr || intr_afifo_ovrr || intr_spdif_err || intr_aud_type_chg || intr_cts_chg || \
		   intr_n_chg || intr_fs_chg || intr_2pcm_chg || intr_2npcm_chg || intr_ecc_err || intr_terc4_err)
		{
			HDMI_ERROR("[%d] un%d ov%d sp%d ty%d ct%d n%d fs%d 2p%d 2n%d ec%d te%d\n", \
						link_num, intr_afifo_undr, intr_afifo_ovrr, intr_spdif_err, intr_aud_type_chg, intr_cts_chg,	\
						intr_n_chg, intr_fs_chg, intr_2pcm_chg, intr_2npcm_chg, intr_ecc_err, intr_terc4_err);
		}
	}

	if ( LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_ecc_err) )
	{
		_HDMI_H15A0_Disable_TMDS_Error_Interrupt(link_num);

		if( gHDMI_Phy_Control.link_pkt_error_workaround == 1) {
			if (_gH15A0_TMDS_ERROR_EQ[link_num] != -1)
				_gH15A0_TMDS_ERROR_EQ[link_num] ++;
			if (_gH15A0_TMDS_ERROR_EQ_2nd[link_num] != -1)
				_gH15A0_TMDS_ERROR_EQ_2nd[link_num] ++;

			_gH15A0_TMDS_ERROR_intr_count[link_num] ++;
		}

		if( _gH15A0HDMIPhyInform.hdcp_authed[link_num])
			_g_ecc_err_authed[link_num] ++;

		HDMI_INTR("intr_ecc_err \n");
	}
	if ( LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_terc4_err) )
	{
		_HDMI_H15A0_Disable_TERC4_Error_Interrupt(link_num);

		if( gHDMI_Phy_Control.link_pkt_error_workaround == 1) {
			_gH15A0_TERC4_ERROR_intr_count[link_num] ++;
		}

		HDMI_INTR("intr_terc4_err \n");
	}
	if ( LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_hdcp_err) )
	{
		_HDMI_H15A0_Disable_HDCP_Error_Interrupt(link_num);

		if( gHDMI_Phy_Control.link_pkt_error_workaround == 1) {
			_gH15A0_HDCP_ERROR_intr_count[link_num] ++;
		}

		HDMI_INTR("intr_hdcp_err \n");
	}
	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_scdt_fedge))
	{
#ifdef H15_BRINGUP_TODO
		if(_gH15A0HDMI_thread_running > 0)
			_HDMI_H15A0_TMDS_ResetPort_Control(link_num ,0);
#endif
		_SCDT_Fall_Detected[link_num]++;
		HDMI_DEBUG("intr_scdt_fedge_prt[%d]\n", link_num);
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_scdt_redge))
	{
		HDMI_DEBUG("intr_scdt_redge_prt[%d]\n", link_num);
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_auth_init))
	{
		_g_auth_init_intr[link_num] = 1;

		HDMI_DEBUG("intr_auth_init_prt[%d]\n", link_num);

		if(gHDMI_Phy_Control.tcs_restart_auth_init)
		{
			_TCS_Restart_Thread_wakeup = 1;
			wake_up_interruptible(&WaitQueue_TCS_Restart_H15A0);
		}
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_auth_done))
	{
		HDMI_DEBUG("intr_auth_done_prt[%d]\n", link_num);
	}

#if 0
	LINK_NUM_H15_RdFL(0, system_control_00);
	LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, prt_selected);

	if ( ( (prt_selected == 0) && LINK_NUM_H15_Rd_fld(0, interrupt_01, intr_scdt_fedge_prt0)) \
			|| ( (prt_selected == 1) && LINK_NUM_H15_Rd_fld(0, interrupt_01, intr_scdt_fedge_prt1)) \
			|| ( (prt_selected == 2) && LINK_NUM_H15_Rd_fld(0, interrupt_01, intr_scdt_fedge_prt2)) \
			|| ( (prt_selected == 3) && LINK_NUM_H15_Rd_fld(0, interrupt_01, intr_scdt_fedge_prt3)) )
#endif
		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_scdt_fedge))
		{
			if(_gH15A0HDMI_thread_running > 0)
			{
				spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INIT_STATE;
				sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INIT_STATE;
				sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INIT_STATE;
				sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_CLEAR_STATE;
				sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
				sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
				sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
				sHDMI_H15A0_Status.AVIChangeState[link_num] = FALSE;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
			}
		}

	if(link_num == 1)	// LINK port 1 support C-BUS
	{
		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_03, intr_cbus_disconn))
		{
			LINK_NUM_H15_RdFL(link_num, cbus_00);
			LINK_NUM_H15_Wr01(link_num, cbus_00, reg_cbus_disconn_en, 0x0);
			LINK_NUM_H15_WrFL(link_num, cbus_00);

			// set CBUS_04 to initiali state
			_HDMI_H15A0_MHL_Check_Status(link_num, 1);

#ifdef H15_CBUS_PDB_CTRL
			_MHL_PDB_Thread_wakeup = 1;
			wake_up_interruptible(&WaitQueue_MHL_PDB_H15A0);
#endif

			HDMI_DEBUG("---- MHL Clear cbus_disconn_en\n");
			HDMI_INTR("intr_cbus_disconn\n");
		}
	}

	HDMI_INTR("--- intra0 [0x%x] - intra1 [0x%x] - intra2 [0x%x] - intra3 [0x%x]-----\n",intra0, intra1, intra2, intra3);

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_vf_stable))
	{
		HDMI_DEBUG("link[%d] : intr_vf_stable\n", link_num);

		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INTERRUPT_STATE;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
		_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INTERRUPT_STATE;
		_gH15A0HdmiFreqErrorCount[link_num] = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		if (sHDMI_H15A0_Status.HDMIState[link_num] >= HDMI_STATE_READ)
		{
			HDMI_TASK("[%d] %s : -Changed -  sHDMI_H15A0_Status.Intr_vf_stable[link_num] [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] / PSC[%d]\n", __L__, __F__, sHDMI_H15A0_Status.HDMIState[link_num], _gH15A0HdmiPortStableCount[link_num] );
			spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
			sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_READ_STATE;
			//_gH15A0HDMIState = HDMI_STATE_READ;
			sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
			sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
			sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
			spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

			//intr_vf_stable is abnormally toggled from UP player source in UD Model for abnormal chip only.(2013-06-25)
			//Check a HDMI Port Stable Count
			if ( (CHK_UHD_BD() && _gH15A0HdmiPortStableCount[link_num] > HDMI_AUDIO_RECHECK_TIME_500MS)	\
					||(CHK_FHD_BD() && _gH15A0HdmiPortStableCount[link_num] > HDMI_AUDIO_PORT_STABLE_TIME_5S) )
			{
				//spin lock for protection for audio
				spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
				_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_STABLE_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);
			}
			else
			{
				//Mute audio data
				muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
				muteCtrl.bAudioMute = TRUE;
				_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);
			}
		}
		else
		{
			//Mute audio data
			muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
			muteCtrl.bAudioMute = TRUE;
			_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);
		}
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_chg_avi_b12345))
	{
		HDMI_DEBUG("intr_chg_avi_b12345 : link[%d]\n", link_num);
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
		sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
		if (_gH15A0CHG_AVI_count_MHL != -1)
			_gH15A0CHG_AVI_count_MHL ++;
		if (_gH15A0CHG_AVI_count_EQ != -1)
			_gH15A0CHG_AVI_count_EQ ++;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_chg_avi_to_rgb))
		{
			HDMI_INTR("intr_chg_avi_to_rgb : link[%d]\n", link_num);
			csc = LX_HDMI_AVI_COLORSPACE_RGB;
		}

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_chg_avi_to_ycc444))
		{
			HDMI_INTR("intr_chg_avi_to_ycc444 : link[%d]\n", link_num);
			csc = LX_HDMI_AVI_COLORSPACE_YCBCR444;
		}

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_chg_avi_to_ycc422))
		{
			HDMI_INTR("intr_chg_avi_to_ycc422 : link[%d]\n", link_num);
			csc = LX_HDMI_AVI_COLORSPACE_YCBCR422;
		}

		if ( (LINK_NUM_H15_Rd_fld(link_num, packet_18, reg_pkt_avi_dat_1) & 0x60) == 0x60 )
		{
			HDMI_INTR("in intr ycc420 mode : link[%d] \n", link_num);
			csc = LX_HDMI_AVI_COLORSPACE_YCBCR420;
		}

		HDMI_TASK("[%d][HDMI_AVI] link[%d] state [%d]  preCSS[%d] CSC = %d \n", __L__, link_num,  sHDMI_H15A0_Status.HDMIState[link_num], sHDMI_H15A0_Status.PrevPixelEncoding[link_num], csc);

		if (sHDMI_H15A0_Status.HDMIState[link_num] >= HDMI_STATE_READ)//(sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_STABLE_STATE) 			// for PS3 CSC
		{
			if (sHDMI_H15A0_Status.PrevPixelEncoding[link_num] != csc)
			{
				//spin lock for protection
				spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_CHANGE_STATE;
				sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
				sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
				sHDMI_H15A0_Status.AVIChangeState[link_num] = TRUE;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

				HDMI_TASK("[%d] %s link[%d] : -Changed  - sHDMI_H15A0_Status.Intr_avi[link_num] [STABLE_STATE] => [CHANGE_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__, link_num, sHDMI_H15A0_Status.HDMIState[link_num] );
				muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
				muteCtrl.bVideoMute = TRUE;
				_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);
			}
		}
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_set_mute))
	{
		HDMI_INTR("intr_set_mute : link[%d]\n", link_num);

		//if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] > HDMI_VIDEO_INIT_STATE)
		{
			if (sHDMI_H15A0_Status.Intr_src_mute[link_num] != HDMI_SOURCE_MUTE_STATE)
			{
				muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
				muteCtrl.bVideoMute = TRUE;
				_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

				spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_STATE;
				sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

				HDMI_DEBUG("[%d] intr_set_mute link[%d] : HDMI_SOURCE MUTE Enable[%d] \n", __L__, link_num, sHDMI_H15A0_Status.Intr_src_mute[link_num] );
			}
		}
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_no_vsi))
	{
		HDMI_TASK("intr_no_vsi : link[%d]\n", link_num);
		HDMI_TASK("[%d] %s : -Changed  - sHDMI_H15A0_Status.Intr_vsi[link_num] [STABLE_STATE] => [NO_DATA_STAT]     /   HDMI_STABLE[%d] \n", __L__, __F__,sHDMI_H15A0_Status.HDMIState[link_num] );
		HDMI_TASK("[%d] %s : -Changed  - sHDMI_H15A0_Status.Intr_vf_stable[link_num] [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__,sHDMI_H15A0_Status.HDMIState[link_num] );
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_NO_DATA_STATE;
		sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_READ_STATE;
		sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
		sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_vsi_enable, 0x0);			///< 20 intr_no_vsi_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

		HDMI_DEBUG("[%d] %s link[%d] : No VSI intra disable  \n", __LINE__, __func__, link_num);
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_chg_vsi_vformat))
	{
		HDMI_DEBUG("intr_chg_vsi_vformat : link[%d] \n", link_num);
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INTERRUPT_STATE;
		sHDMI_H15A0_Status.VSIState[link_num] = TRUE;

#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_no_vsi_enable, 0x1);			///< 20 intr_no_vsi_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);
		sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
		HDMI_DEBUG("[%d] %s : No VSI intra enable link[%d] \n", __LINE__, __func__, link_num);

		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

		// when  don't change Sync and change VSI on 3D for OPTIMUS Phone ( 3D / LTE /MHL )
		if (sHDMI_H15A0_Status.HDMIState[link_num] > HDMI_STATE_READ)
		{
			HDMI_TASK("[%d] %s link[%d] : -Changed  - _gH15A0Intr_vf_stable [STABLE_STATE] => [READ_STATE]     /   HDMI_STABLE[%d] \n", __L__, __F__,link_num, sHDMI_H15A0_Status.HDMIState[link_num] );
			spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
			sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_READ_STATE;
			sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
			sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
			sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
			spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
		}
	}

	//audio related. => start
	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_new_acr))
	{
		HDMI_INTR("intr_new_acr : link[%d]\n", link_num);

#ifndef H15_DISABLE_AUDIO_INTERRUPT
		/* Disable a intr_new_acr_enable and Enable a intr_scdt_fall_enable interrupt. */
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_fall_enable, 0x1);		///< 3 intr_scdt_fall_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_new_acr_enable,   0x0);		///< 26 intr_new_acr_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);
#endif

		//To protect EMI issue, ACR is disabled in DVI mode because of avoiding abnormal clock generation.
		LINK_NUM_H15_RdFL(link_num, audio_00);
		LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_en, 0x1);	//ACR Enable(Audio Clock Generation Function Activation)
		LINK_NUM_H15_WrFL(link_num, audio_00);

		HDMI_INTR("[%d]IRQHandler : ACR ON.\n", __L__);
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_scdt_fall))
	{
		HDMI_INTR("intr_scdt_fall : link[%d]\n", link_num);

#ifndef H15_DISABLE_AUDIO_INTERRUPT
		/* Disable a intr_scdt_fall_enable and Enable a intr_new_acr_enable interrupt. */
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
		LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_scdt_fall_enable, 0x0);		///< 3 intr_scdt_fall_enable
		LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_new_acr_enable,   0x1);		///< 26 intr_new_acr_enable
		LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);
#endif

		//To protect EMI issue, ACR is disabled in DVI mode because of avoiding abnormal clock generation.
		LINK_NUM_H15_RdFL(link_num, audio_00);
		LINK_NUM_H15_Wr01(link_num, audio_00, reg_acr_en, 0x0);	//ACR Disable(Audio Clock Generation Function Activation)
		LINK_NUM_H15_WrFL(link_num, audio_00);

		HDMI_INTR("[%d]IRQHandler : ACR OFF.\n", __L__);
	}

	//intr_fs_chg is normal toggled in FHD Model.
	if (CHK_FHD_BD() && (intra0 & HDMI_AUDIO_INTERRUPT_BIT_MASK))
	{
		//spin lock for protection for audio
		spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
		_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INTERRUPT_STATE;
		_gH15A0HdmiFreqErrorCount[link_num] = 0;
		spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

		//Mute audio data
		muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
		muteCtrl.bAudioMute = TRUE;
		_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_fs_chg))
			HDMI_INTR("intr_fs_chg\n");

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2pcm_chg))
			HDMI_INTR("intr_2pcm_chg\n");

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2npcm_chg))
			HDMI_INTR("intr_2npcm_chg\n");
	}

	//intr_fs_chg is abnormally toggled from UD player source in UD Model.
	if (CHK_UHD_BD() && (intra0 & HDMI_AUDIO_INTERRUPT_BIT_MASK))
	{
		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_fs_chg))
			HDMI_INTR("intr_fs_chg\n");

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2pcm_chg))
			HDMI_INTR("intr_2pcm_chg\n");

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_2npcm_chg))
			HDMI_INTR("intr_2npcm_chg\n");
	}

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_00, intr_burst_info))
	{
		//Get a elapsed mili-second time.
		elapsedTime = jiffies_to_msecs(jiffies - _gH15A0BurstInfoPrevJiffies[link_num]);

		//Save _gH15A0BurstInfoPrevJiffies[link_num]
		_gH15A0BurstInfoPrevJiffies[link_num] = jiffies;

		if (_gH15A0IntrBurstInfoCount[link_num] == 0 && elapsedTime > HDMI_AUDIO_BURST_INFO_RECHECK_80MS)
		{
			//spin lock for protection for audio
			spin_lock_irqsave(&_gIntrHdmiH15A0AudioLock, flags);
			_gH15A0IntrAudioState[link_num] = HDMI_AUDIO_INTERRUPT_STATE;
			_gH15A0HdmiFreqErrorCount[link_num] = 0;
			spin_unlock_irqrestore(&_gIntrHdmiH15A0AudioLock, flags);

			//Mute audio data
			muteCtrl.eMode		= LX_HDMI_AUDIO_MUTE;
			muteCtrl.bAudioMute = TRUE;
			_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

			if (_gH15A0HdmiPortStableCount[link_num] > HDMI_AUDIO_PORT_STABLE_TIME_5S)
			{
				//Check a vaild Pc data for payload
				LINK_NUM_H15_RdFL(link_num, audio_09);
				HDMI_DEBUG("intr_burst_info(%llu), Pd_Pc = 0x%X, elapsedTime = %llu\n",	\
						_gH15A0IntrBurstInfoCount[link_num], LINK_NUM_H15_Rd(link_num, audio_09), elapsedTime);
			}
		}

		//Increase _gH15A0IntrBurstInfoCount[link_num]
		_gH15A0IntrBurstInfoCount[link_num]++;

		HDMI_INTR("intr_burst_info(%llu)\n", _gH15A0IntrBurstInfoCount[link_num]);

		//if ((_gH15A0HdmiAudioThreadCount[link_num] % DEBUG_HDMI_AUDIO_MSG_PRINT_TIME_100S) == 0)
		//HDMI_DEBUG("intr_burst_info(%llu), elapsedTime(%llu)\n", _gH15A0IntrBurstInfoCount[link_num], elapsedTime);
	}
	else
	{
		//_gH15A0IntrBurstInfoCount[link_num] is normal toggled in FHD Model.
		if (CHK_FHD_BD())
		{
			//Reset _gH15A0IntrBurstInfoCount[link_num]
			_gH15A0IntrBurstInfoCount[link_num] = 0;
		}
		else
		{
			//Check a vaild Pc data for payload
			LINK_NUM_H15_RdFL(link_num, audio_09);
			HDMI_INTR("_gH15A0IntrBurstInfoCount[link_num](%llu), Pd_Pc = 0x%X\n", _gH15A0IntrBurstInfoCount[link_num], LINK_NUM_H15_Rd(link_num, audio_09));
		}
	}
	//audio related. => end.

	if (LINK_NUM_H15_Rd_fld(link_num, interrupt_01, intr_hdmi5v_redge) )
	{
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);

		_gHdmi_no_connection_count = 0;

		if ( _gH15A0HDMI_thread_running < 1)
		{
			_gH15A0Force_thread_sleep = 0;
			sHDMI_H15A0_Status.HDMIState[0] = HDMI_STATE_IDLE;
			sHDMI_H15A0_Status.HDMIState[1] = HDMI_STATE_IDLE;
			wake_up_interruptible(&WaitQueue_HDMI_H15A0);
			_gH15A0HDMI_thread_running = 1;
		}
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
		HDMI_DEBUG("HDMI 5V Detected !!! : link[%d]\n", link_num);
	}

	if(link_num == 1)
	{
		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_02, intr_cbus_new_rcp))
		{
			_gMHL_RCP_RCV_MSG.rcp_receive_flag = TRUE;
			LINK_NUM_H15_RdFL(link_num, cbus_11);
			LINK_NUM_H15_Rd01(link_num, cbus_11, reg_rx_rcpk_rtn_kcode, _gMHL_RCP_RCV_MSG.rcp_msg);
			HDMI_INTR("intr_cbus_new_rcp(0x%x)\n", _gMHL_RCP_RCV_MSG.rcp_msg);
		}

		if (LINK_NUM_H15_Rd_fld(link_num, interrupt_02, intr_cbus_cmd_rcp))
		{
			spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
			if(_g_rcp_send_buffer <2)
				_g_rcp_send_buffer ++;
			spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
			HDMI_INTR("intr_cbus_cmd_rcp:buffer[%d]\n", _g_rcp_send_buffer);
		}
	}

	if (intra0)
	{
		LINK_NUM_H15_Wr(link_num, interrupt_00, intra0);
		LINK_NUM_H15_WrFL(link_num, interrupt_00);
	}

	if (intra1)
	{
		LINK_NUM_H15_Wr(link_num, interrupt_01, intra1);
		LINK_NUM_H15_WrFL(link_num, interrupt_01);
	}

	if (intra2)
	{
		LINK_NUM_H15_Wr(link_num, interrupt_02, intra2);
		LINK_NUM_H15_WrFL(link_num, interrupt_02);
	}

	if (intra3)
	{
		LINK_NUM_H15_Wr(link_num, interrupt_03, intra3);
		LINK_NUM_H15_WrFL(link_num, interrupt_03);
	}

	return;
}

static void _HDMI_H15A0_Periodic_Task(int link_num)
{
	static UINT32 gPeriodicState[2] = {HDMI_STATE_IDLE, };
	static UINT32 gCheakCnt[2] = {0,};		// check count after vf stable interrupt
	static UINT32 gMuteCnt[2] = {0,};			// check count for  mute
	static UINT32 gAviCnt[2] = {0,};			// check count for checking AVI Data	in the HDMI mode
	static UINT32 gAviChgCnt[2] = {0,};		// check count after changing CSC in stable status
	static UINT32 gScdtCnt[2] = {0,};		// check count after connected
	static UINT32 hdcp_authed_info[2] = {0,};
	static UINT32 hdcp_enc_en_info[2] = {0,};
	static int hdcp_check_count[2] = {HDMI_CHEAK_COUNT, };

	UINT32 muteClear = 0;
	UINT32 aviHeader = 0;
	UINT32 temp = 0;
	ULONG	flags = 0;
	LX_HDMI_MUTE_CTRL_T 	muteCtrl = {FALSE, FALSE, LX_HDMI_MUTE_NONE};

	do {
		if (sHDMI_H15A0_Status.HDMIState[link_num] == HDMI_STATE_IDLE && sHDMI_H15A0_Status.HDMIState[link_num] == HDMI_STATE_DISABLE)
			gPeriodicState[link_num] = sHDMI_H15A0_Status.HDMIState[link_num];

		if (sHDMI_H15A0_Status.HDMIConnectState[link_num] == HDMI_PORT_NOT_CONNECTED)			// change check connection in the thread function by 20121206
		{
			gCheakCnt[link_num] = gMuteCnt[link_num] = gAviCnt[link_num] = gAviChgCnt[link_num] = gScdtCnt[link_num] = 0;
			gPeriodicState[link_num] = HDMI_STATE_NO_SCDT;
		}
		else
		{
			if (sHDMI_H15A0_Status.HDMIState[link_num] <= HDMI_STATE_NO_SCDT)
			{
				gPeriodicState[link_num] = HDMI_STATE_SCDT;
				HDMI_TASK("link[%d] : -Changed- HDMI_STATE_DISABLE  => HDMI_STATE_SCDT[%d] \n", link_num, gPeriodicState[link_num]);
			}

			///* start Source Mute Control  *///
			if (sHDMI_H15A0_Status.Intr_src_mute[link_num] == HDMI_SOURCE_MUTE_STATE)
			{
				LINK_NUM_H15_RdFL(link_num, packet_33);
				LINK_NUM_H15_Rd01(link_num, packet_33, reg_pkt_gcp_cmute, muteClear);
				LINK_NUM_H15_Rd01(link_num, packet_33, reg_pkt_gcp_smute, temp);

				gMuteCnt[link_num]++;
				if (muteClear == 1  || gMuteCnt[link_num] >= 150)
				{
					spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
					sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_CLEAR_STATE;
					spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

					gPeriodicState[link_num] = HDMI_STATE_CHECK_MODE;
					_HDMI_Check_State_StartTime = jiffies_to_msecs(jiffies);

					muteCtrl.eMode		= LX_HDMI_VIDEO_MUTE;
					muteCtrl.bVideoMute = FALSE;
					_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

					HDMI_DEBUG("link[%d] : -source mute clear  cnt [%d] state[%d]  \n", link_num, gMuteCnt[link_num], gPeriodicState[link_num]);
					gCheakCnt[link_num] =  gMuteCnt[link_num] = 0;
				}
				else
				{
					if (gPeriodicState[link_num] != HDMI_STATE_CHECK_SOURCE_MUTE)
					{
						HDMI_TASK("link[%d] : -Changed- HDMI_STATE [%d]  => HDMI_STATE_CHECK_SOURCE_MUTE \n", link_num, gPeriodicState[link_num]);
						gPeriodicState[link_num] = HDMI_STATE_CHECK_SOURCE_MUTE;
						_HDMI_Check_State_StartTime = jiffies_to_msecs(jiffies);
					}
				}
			}
			///* end Source Mute Control  *///
			if (sHDMI_H15A0_Status.AVIChangeState[link_num] == TRUE)
			{
				HDMI_TASK("link[%d] : -Changed- HDMI_STATE_[%d]  => HDMI_STATE_CHECK_AVI_CHG \n", link_num, gPeriodicState[link_num]);
				spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				sHDMI_H15A0_Status.AVIChangeState[link_num] = FALSE;
				//sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
				gPeriodicState[link_num] = HDMI_STATE_CHECK_AVI_CHG;
				gAviChgCnt[link_num] = 0;
			}

			//if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_READ_STATE)
			if ( (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_READ_STATE) \
					&& ( ( _gH15A0HDMIPhyInform.hdcp_authed[link_num] \
							&& _HDMI_H15A0_Check_Aksv_Exist(link_num) \
							&& (hdcp_check_count[link_num] == HDMI_CHEAK_COUNT) )
						|| (_HDMI_H15A0_Check_Aksv_Exist(link_num) == 0) )
			  )
			{
				HDMI_TASK("link[%d] : -Changed- HDMI_STATE_[%d]  => HDMI_STATE_READ && Intr_vf_stable[READ_STATE => INTERRUPT_STATE] \n", link_num, gPeriodicState[link_num]);
				gPeriodicState[link_num] = HDMI_STATE_READ;
				spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
				sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INTERRUPT_STATE;
				spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

#if 0
				if (  (gHDMI_Phy_Control.hdcp14_ovr_workaround > 0) \
					/* && (_HDMI_H15A0_Check_Aksv_Exist(link_num) == 0) \ */
					&& (link_num != EXT_MHL_IC_LINK_NUM) \
					&& ( (_gH15A0HDMIPhyInform.hdcp22_status[link_num] & 0x1) == 0) \
					&& (_gH15A0HDMIPhyInform.hdcp_authed[link_num] == 0) && (_gH15A0HDMIPhyInform.hdcp_enc_en[link_num] == 0) )
				{
					HDMI_NOTI("@@@@ HDMI : Link [%d] HDCP Not Authed/Encoded, : hdcp22_status[0x%x]  \n",link_num , _gH15A0HDMIPhyInform.hdcp22_status[link_num] );
					HDMI_NOTI("@@@@ HDMI : HDCP22 Control Override Enable to 1.4 !!!!! \n");

					HDCP22_NUM_H15_RdFL(link_num, ad_hdcp22_control);
					HDCP22_NUM_H15_Wr(link_num, ad_hdcp22_control, 0x410002);
					HDCP22_NUM_H15_WrFL(link_num, ad_hdcp22_control);
				}
#endif

				if ( (_g_ecc_err_authed[link_num] > 0) && (gHDMI_Phy_Control.tcs_add_more_workaround > 0) )
				{
					HDMI_NOTI("---- HDMI_PHY : port [%d] ECC Error Count after Authed : [%d]  \n",link_num , _g_ecc_err_authed[link_num] );

#if 0
					if (_gH15A0HDMIPhyInform.phy_pdb[link_num] && _gH15A0HDMIPhyInform.phy_rstn[link_num])
					{
						/* All Phy Disabled except link_num */
						if (link_num != 0)
							_HDMI_H15A0_EnablePort(0, 0);
						if (link_num != 1)
							_HDMI_H15A0_EnablePort(1, 0);

						/* Enable port_num */
						_HDMI_H15A0_EnablePort(link_num, 1);

						//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
						//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
						//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

						if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
							PHY_REG_H15B0_RdFL(tcs_additional_value);
							PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init + 0x4);
							PHY_REG_H15B0_WrFL(tcs_additional_value);
						}
						else {
							PHY_REG_H15A0_RdFL(tcs_additional_value);
							PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init + 0x4);
							PHY_REG_H15A0_WrFL(tcs_additional_value);
						}


						/* if link_num is not current selected port , disable port */
						if (link_num != _gH15A0HDMIPhyInform.prt_sel)
							_HDMI_H15A0_EnablePort(link_num, 0);

						/* Enable selected port */
						_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);
					}
#endif
				}
			}

			/* M14 HDCP Authentication Information */
			if (sHDMI_H15A0_Status.hdmi_mode[link_num] == 1)		// for HDMI Mode
			{
				if ( ( memcmp(_gH15A0HDMIPhyInform.hdcp_authed, hdcp_authed_info, (size_t)sizeof(hdcp_authed_info)) ) \
						|| ( memcmp(_gH15A0HDMIPhyInform.hdcp_enc_en, hdcp_enc_en_info, (size_t)sizeof(hdcp_authed_info)) ) )
				{
					if(0)		// for test only
					{
						if(hdcp_authed_info[link_num] != _gH15A0HDMIPhyInform.hdcp_authed[link_num] )
						{
							HDMI_NOTI("@@@@@ Link[%d], _g_TCS_EN_restart[%d]:[%d] : HDCP_Authed [%d] => [%d] \n"\
									, link_num, link_num, _g_TCS_EN_restart[link_num], hdcp_authed_info[link_num], _gH15A0HDMIPhyInform.hdcp_authed[link_num] );
							if ( (_gH15A0HDMIPhyInform.hdcp_authed[link_num]) && (_g_TCS_EN_restart[link_num] == 1) )
							{
								if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
									/*
									PHY_REG_H15B0_RdFL(tcs_boundary);
									PHY_REG_H15B0_Wr01(tcs_boundary, tcs_boundary, _HDMI_H15A0_TCS_addtional_value_init);
									PHY_REG_H15B0_WrFL(tcs_boundary);

									PHY_REG_H15B0_RdFL(tcs_additional_value);
									PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init);
									PHY_REG_H15B0_WrFL(tcs_additional_value);
									*/

									PHY_REG_H15B0_RdFL(bert_tmds_sel);
									PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
									PHY_REG_H15B0_WrFL(bert_tmds_sel);

									PHY_REG_H15B0_RdFL(bert_tmds_sel);
									PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
									PHY_REG_H15B0_WrFL(bert_tmds_sel);
									HDMI_NOTI("@@@@@ TCS Restarted link[%d] \n", link_num);
								}
								_g_TCS_EN_restart[link_num] = 0;
							}
						}

					}
					HDMI_TASK("HDMI HDCP Information Changed !!!\n");

					for (temp = 0; temp < 2;temp++)
						HDMI_TASK("Port[%d] : HDCP_ENC_EN[%d]=>[%d], HDCP_AUTHED[%d]=>[%d]\n", temp, hdcp_enc_en_info[temp], _gH15A0HDMIPhyInform.hdcp_enc_en[temp], \
								hdcp_authed_info[temp], _gH15A0HDMIPhyInform.hdcp_authed[temp]);

					memcpy(hdcp_authed_info, _gH15A0HDMIPhyInform.hdcp_authed, (size_t)sizeof(hdcp_authed_info));
					memcpy(hdcp_enc_en_info, _gH15A0HDMIPhyInform.hdcp_enc_en, (size_t)sizeof(hdcp_authed_info));
				}

			}
		}

		switch(gPeriodicState[link_num])
		{
			case HDMI_STATE_NO_SCDT:
				break;

			case HDMI_STATE_SCDT:		// if (HDMI_SCDT_COUNT == 4)  5 call
				{
					hdcp_check_count[link_num] = HDMI_CHEAK_COUNT;
									// x2 for H15A0 (no external switch)
					if (gScdtCnt[link_num] < (HDMI_SCDT_COUNT<<1) )	gScdtCnt[link_num]++;
					else
					{
						gScdtCnt[link_num]++;
						if (sHDMI_H15A0_Status.Intr_vf_stable[link_num] == HDMI_VIDEO_INTERRUPT_STATE)
						{
							gPeriodicState[link_num] = HDMI_STATE_CHECK_MODE;
							_HDMI_Check_State_StartTime = jiffies_to_msecs(jiffies);
							HDMI_TASK("link[%d] : -Changed- HDMI_STATE_SCDT  => HDMI_STATE_CHECK_MODE with HDMI_VIDEO_INTERRUPT_STATE \n", link_num);
						}

						if (gScdtCnt[link_num] > 50)			// when no vf_stable Interrupt
						{
							gPeriodicState[link_num] = HDMI_STATE_CHECK_MODE;
							_HDMI_Check_State_StartTime = jiffies_to_msecs(jiffies);
							spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
							sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INTERRUPT_STATE;
							if (sHDMI_H15A0_Status.Intr_avi[link_num] == HDMI_AVI_INIT_STATE)		sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INTERRUPT_STATE;
							if (sHDMI_H15A0_Status.Intr_vsi[link_num] == HDMI_VSI_INIT_STATE)		sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INTERRUPT_STATE;
							spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

							HDMI_TASK("link[%d] : -Changed- HDMI_STATE_SCDT  => HDMI_STATE_CHECK_MODE with No vf_stable Interrupt \n", link_num);
						}
					}
					HDMI_TASK("link[%d] : HDMI_STATE_SCDT scdtCnt = [%d] \n", link_num, gScdtCnt[link_num]);
				}
				break;

			case HDMI_STATE_INTE_CHECK:
			case HDMI_STATE_CHECK_MODE:		// all interrupt cheak
			case HDMI_STATE_CHECK_SOURCE_MUTE:
			case HDMI_STATE_CHECK_AVI_NO:
				{
					///*  HDMI_STATE_CHECK_SOURCE_MUTE *///
					if (sHDMI_H15A0_Status.Intr_src_mute[link_num] == HDMI_SOURCE_MUTE_STATE)
					{
						HDMI_TASK("link[%d] : HDMI_STATE [HDMI_STATE_CHECK_SOURCE_MUTE(%d)] \n", link_num, gPeriodicState[link_num]);
						break;
					}

					///*  HDMI_STATE_CHECK_NO_AVI   //If HDMI mode and AVI no packet, timing info is all '0'*///
					if (sHDMI_H15A0_Status.hdmi_mode[link_num] == 1)
					{
						gAviCnt[link_num] ++;

						if ( !_gH15A0HDMIPhyInform.hdcp_authed[_gH15A0HDMIPhyInform.prt_sel]	\
								&& (_HDMI_H15A0_Check_Aksv_Exist(_gH15A0HDMIPhyInform.prt_sel) != 0) )
						{
							hdcp_check_count[link_num] = 100; //maximum 2sec
							HDMI_TASK("link[%d] : HDCP Auth : HDMI_STATE [HDMI_STATE_CHECK_SOURCE_MUTE(%d)] \n", link_num, gPeriodicState[link_num]);
						}
						else if ( (hdcp_check_count[link_num] == 100) && _gH15A0HDMIPhyInform.hdcp_authed[_gH15A0HDMIPhyInform.prt_sel]	\
								&& (_HDMI_H15A0_Check_Aksv_Exist(_gH15A0HDMIPhyInform.prt_sel) != 0) )
						{
							hdcp_check_count[link_num] = gCheakCnt[link_num] + 10;	// 200msec added
							HDMI_TASK("HDMI_STATE [%d] : HDCP Authed set hdcp_check_cout to [%d] \n", gPeriodicState[link_num], hdcp_check_count[link_num]);
						}
						/*
						else if ( _(hdcp_check_count[link_num] == HDMI_CHEAK_COUNT) && gH15A0HDMIPhyInform.hdcp_authed[_gH15A0HDMIPhyInform.prt_sel]	\
								&& (_HDMI_H15A0_Check_Aksv_Exist(_gH15A0HDMIPhyInform.prt_sel) != 0) )
						{
							hdcp_check_count[link_num] = HDMI_CHEAK_COUNT;	// No AKSV Received : HDCP not encoded?
							HDMI_TASK("HDMI_STATE [%d] : HDCP Already Authed \n", gPeriodicState[link_num]);
						}
						*/
						else if ( _HDMI_H15A0_Check_Aksv_Exist(_gH15A0HDMIPhyInform.prt_sel) == 0)
						{
							if(_gH15A0HDMIPhyInform.tmds_clock[link_num] > 29000)
								hdcp_check_count[link_num] = HDMI_CHEAK_COUNT + 20;	// MHL 3.0 Xperia Z2 green screen issue
							else
								hdcp_check_count[link_num] = HDMI_CHEAK_COUNT + 30;	// PS4 BDP magenta screen issue (141105 added 10)
								//hdcp_check_count[link_num] = HDMI_CHEAK_COUNT;	// No AKSV Received : HDCP not encoded?
							HDMI_TASK("HDMI_STATE [%d] : HDCP no encoded ??? \n", gPeriodicState[link_num]);
						}
																					// x2 for H15A0 (no external switch)
						if (sHDMI_H15A0_Status.Intr_avi[link_num] <= HDMI_AVI_INTERRUPT_STATE &&  gAviCnt[link_num] < (50<<1) )		// No read AVI
						{
							LINK_NUM_H15_RdFL(link_num, packet_18);
							aviHeader = LINK_NUM_H15_Rd(link_num, packet_18);		// reg_pkt_avi_hdr_0 (AVI Packet Version), reg_pkt_avi_hdr_1 (AVI Packet Length)
							aviHeader = (aviHeader &0xffff);

							if (aviHeader == 0)
							{
								gPeriodicState[link_num] = HDMI_STATE_CHECK_AVI_NO;
								HDMI_TASK("*[%d] %s : No AVI Packet - before AVI Init state\n", __L__, __F__);
								break;
							}
						}
					}
					else
						hdcp_check_count[link_num] = HDMI_CHEAK_COUNT;

					///*  HDMI_STATE_CHECK_MODE && HDMI_STATE_CHECK_AVI_CHG *///
					//if (gCheakCnt[link_num] > HDMI_CHEAK_COUNT)
					_HDMI_Check_State_ElaspedTime = jiffies_to_msecs(jiffies);

					if ( ( gCheakCnt[link_num] > hdcp_check_count[link_num] ) || ( (_HDMI_Check_State_ElaspedTime - _HDMI_Check_State_StartTime) > 2500) )	//MAX 2.5 Sec
					{
						gPeriodicState[link_num] = HDMI_STATE_READ;

						HDMI_TASK("link[%d] : -Changed- HDMI_STATE_INTE_CHECK[%d] => HDMI_STATE_READ : [%d msec] )\n" \
								, link_num , gPeriodicState[link_num], (_HDMI_Check_State_ElaspedTime - _HDMI_Check_State_StartTime) );

#if 0
						if (  (gHDMI_Phy_Control.hdcp14_ovr_workaround > 0) \
							/*	&& (_HDMI_H15A0_Check_Aksv_Exist(link_num) == 0) \ */
								&& (link_num != EXT_MHL_IC_LINK_NUM) \
								&& ( (_gH15A0HDMIPhyInform.hdcp22_status[link_num] & 0x1) == 0) \
								&& (_gH15A0HDMIPhyInform.hdcp_authed[link_num] == 0) && (_gH15A0HDMIPhyInform.hdcp_enc_en[link_num] == 0) )
						{
							HDMI_NOTI("@@@@ HDMI : Link [%d] HDCP Not Authed/Encoded : hdcp22_status[0x%x]  \n",link_num , _gH15A0HDMIPhyInform.hdcp22_status[link_num] );
							HDMI_NOTI("@@@@ HDMI : HDCP22 Control Override Enable to 1.4 !!!!! \n");

							HDCP22_NUM_H15_RdFL(link_num, ad_hdcp22_control);
							HDCP22_NUM_H15_Wr(link_num, ad_hdcp22_control, 0x410002);
							HDCP22_NUM_H15_WrFL(link_num, ad_hdcp22_control);
						}
#endif

						if ( (_g_ecc_err_authed[link_num] > 0) && (gHDMI_Phy_Control.tcs_add_more_workaround > 0) )
						{
							HDMI_NOTI("@@@@ HDMI_PHY : port [%d] ECC Error Count after Authed : [%d]  \n",link_num , _g_ecc_err_authed[link_num] );

#if 0
							if( (_gH15A0HDMIPhyInform.tcs_min[link_num][0] > 0x10) || (_gH15A0HDMIPhyInform.tcs_min[link_num][1] > 0x10) || (_gH15A0HDMIPhyInform.tcs_min[link_num][2] > 0x10) )
							{
								HDMI_NOTI("!!!! HDMI_PHY : port [%d] TCS MIN high : [0x%x][0x%x][0x%x]  \n",link_num\
										, _gH15A0HDMIPhyInform.tcs_min[link_num][0] , _gH15A0HDMIPhyInform.tcs_min[link_num][1] , _gH15A0HDMIPhyInform.tcs_min[link_num][2] );
							}
							else if (_gH15A0HDMIPhyInform.phy_pdb[link_num] && _gH15A0HDMIPhyInform.phy_rstn[link_num])
							{
								/* All Phy Disabled except link_num */
								if (link_num != 0)
									_HDMI_H15A0_EnablePort(0, 0);
								if (link_num != 1)
									_HDMI_H15A0_EnablePort(1, 0);

								/* Enable port_num */
								_HDMI_H15A0_EnablePort(link_num, 1);

								//		PHY_REG_H15A0_RdFL(eq_cs_rs_sel);
								//		PHY_REG_H15A0_Wr01(eq_cs_rs_sel, eq_cs_man_sel,0x0);
								//		PHY_REG_H15A0_WrFL(eq_cs_rs_sel);

								if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
									PHY_REG_H15B0_RdFL(tcs_additional_value);
									PHY_REG_H15B0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init + 0x4);
									PHY_REG_H15B0_WrFL(tcs_additional_value);

									PHY_REG_H15B0_RdFL(tcs_ch0);
									PHY_REG_H15B0_Rd01(tcs_ch0, tcs_ch0, _HDMI_H15A0_TCS_result[link_num][0]);

									PHY_REG_H15B0_RdFL(tcs_ch1);
									PHY_REG_H15B0_Rd01(tcs_ch1, tcs_ch1, _HDMI_H15A0_TCS_result[link_num][1]);

									PHY_REG_H15B0_RdFL(tcs_ch2);
									PHY_REG_H15B0_Rd01(tcs_ch2, tcs_ch2, _HDMI_H15A0_TCS_result[link_num][2]);

									PHY_REG_H15B0_RdFL(tcs_additional_value);
									PHY_REG_H15B0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[link_num]);
								}
								else {
									PHY_REG_H15A0_RdFL(tcs_additional_value);
									PHY_REG_H15A0_Wr01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_addtional_value_init + 0x4);
									PHY_REG_H15A0_WrFL(tcs_additional_value);

									PHY_REG_H15A0_RdFL(tcs);
									PHY_REG_H15A0_Rd01(tcs, tcs, _HDMI_H15A0_TCS_result[link_num][0]);

									PHY_REG_H15A0_RdFL(tcs_additional_value);
									PHY_REG_H15A0_Rd01(tcs_additional_value, tcs_additional_value, _HDMI_H15A0_TCS_add[link_num]);

								}

								HDMI_NOTI("@@@@ HDMI_PHY : TCS Add changed => Result [0x%x] [0x%x] [0x%x] : Add value [0x%x] \n", _HDMI_H15A0_TCS_result[link_num][0] , _HDMI_H15A0_TCS_result[link_num][1] , _HDMI_H15A0_TCS_result[link_num][2], _HDMI_H15A0_TCS_add[link_num]);

								/* if link_num is not current selected port , disable port */
								if (link_num != _gH15A0HDMIPhyInform.prt_sel)
									_HDMI_H15A0_EnablePort(link_num, 0);

								/* Enable selected port */
								_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);
							}
#endif
						}
					}
					gCheakCnt[link_num]++;

					HDMI_TASK("link [%d] : HDMI State = [%d]  cheakCnt = %d\n", link_num, gPeriodicState[link_num], gCheakCnt[link_num]);
				}
				break;

			case HDMI_STATE_CHECK_AVI_CHG:
				{
					if (gAviChgCnt[link_num] > 4)
					{
						muteCtrl.eMode	= LX_HDMI_VIDEO_MUTE;
						muteCtrl.bVideoMute = FALSE;			// mute Clear
						_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);

						gPeriodicState[link_num] = HDMI_STATE_CHECK_MODE;
						gAviChgCnt[link_num] = gCheakCnt[link_num] =  0;
						_HDMI_Check_State_StartTime = jiffies_to_msecs(jiffies);
						HDMI_TASK("[%d] %s : -Changed- HDMI_STATE_CHECK_AVI_CHG  => HDMI_STATE_CHECK_MODE[%d] \n", __L__, __F__, gPeriodicState[link_num]);
					}
					gAviChgCnt[link_num]++;
				}
				break;

			case HDMI_STATE_READ:
				{
					unsigned int elasped_time;

					if (sHDMI_H15A0_Status.TimingReadCnt[link_num] < 2)
					{
						muteCtrl.eMode	= LX_HDMI_VIDEO_MUTE;
						muteCtrl.bVideoMute = FALSE;			// mute Clear
						_HDMI_H15A0_SetInternalMute(link_num, muteCtrl);
						gAviChgCnt[link_num] = gCheakCnt[link_num] =  0;
					}
					else if (sHDMI_H15A0_Status.TimingReadCnt[link_num] > HDMI_READ_COUNT)
					{
						gPeriodicState[link_num] = HDMI_STATE_STABLE;
						elasped_time = jiffies_to_msecs(jiffies) - _Port_Change_StartTime;
						HDMI_DEBUG("### Port Change to Stable Time [%d msec] ### \n", elasped_time);

					}

					HDMI_TASK("link[%d] : HDMI State = [%d]  sHDMI_H15A0_Status.TimingReadCnt[link_num] = %d \n", link_num, gPeriodicState[link_num], sHDMI_H15A0_Status.TimingReadCnt[link_num]);
				}
				break;

			case HDMI_STATE_STABLE:
				{
					spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
					sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_STABLE_STATE;
					spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
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

	//HDMI_TASK("Link[%d] :  gPeriodicState  = %d \n", link_num , gPeriodicState[link_num]);

	sHDMI_H15A0_Status.HDMIState[link_num] = gPeriodicState[link_num];

	if (sHDMI_H15A0_Status.HDMIState[link_num] < HDMI_STATE_STABLE)
		HDMI_PRINT("Link[%d] :  State  = %d \n", link_num , sHDMI_H15A0_Status.HDMIState[link_num]);

	return;
}


int HDMI_H15A0_GetRegister(UINT32 addr , UINT32 *value)
{
	unsigned long ulAddr;
	UINT8 slave =0;
	UINT8 reg =0;
	UINT8 data =0;
	UINT32 prt_selected, phy_en_prt0, phy_en_prt1;
	int phy_tmp_enabled = -1;

	if (addr <= 0xFFF)
	{
		if (pst_LINK0_Reg == NULL)	return RET_ERROR;

		if(addr <= pH15A0HdmiRegCfg->link_reg_size)
			ulAddr = (unsigned long) pst_LINK0_Reg;
		else if (addr <= (0x400 + pH15A0HdmiRegCfg->link1_reg_size))
			ulAddr = (unsigned long) pst_LINK1_Reg - 0x400;
		else if (addr <= (0xc00 + pH15A0HdmiRegCfg->link_top_reg_size))
			ulAddr = (unsigned long) pst_LINK_TOP_Reg - 0xc00;
		else
		{
			HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
			return RET_ERROR;
		}

		*value = *((volatile UINT32*)(ulAddr + addr));
	}
	else if (addr >= 0x3800 && addr <=0x3FFF)
	{
		slave = ((addr &0xff00)>>8)<<1;
		reg = (addr &0xff);

		if(slave == (0x38<<1) )
			prt_selected = 0;
		else
			prt_selected = 1;

		LINK_NUM_H15_RdFL(0, phy_00);
		LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

		if (!( (( prt_selected == 0) && phy_en_prt0) || ( ( prt_selected == 1) && phy_en_prt1)) )
		{
			HDMI_TRACE("[HDMI] %s : Link Port[%d] Not Enabled \n", __F__, prt_selected);
			HDMI_TRACE("[HDMI] Enable selected port [%d] \n", prt_selected);

			phy_tmp_enabled = prt_selected;

			if (prt_selected == 0)
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
				LINK_NUM_H15_WrFL(0, phy_00);
			}
			else if (prt_selected == 1)
			{
				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
				LINK_NUM_H15_WrFL(1, phy_00);
			}

			OS_MsecSleep(5);	// ms delay
		}

		slave = 0x38<<1;

		REG_ReadI2C(PHY_REG_H15_I2C_IDX, slave, reg, &data);
		*value = (UINT32)data;
		//HDMI_DEBUG("[HDMI]  %s : %d  slave = 0x%x reg = 0x%x, Value = 0x%x \n", __F__, __L__, slave, reg,data);

		if (phy_tmp_enabled != -1)
		{
			HDMI_TRACE("[HDMI] Disable selected port [%d] \n", prt_selected);

			if (prt_selected == 0)
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
				LINK_NUM_H15_WrFL(0, phy_00);
			}
			else if (prt_selected == 1)
			{
				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
				LINK_NUM_H15_WrFL(1, phy_00);
			}
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		if ( (addr >= 0x1100) && (addr <= (0x1100 + pH15A0HdmiRegCfg->hdcp22_prt0_reg_size) ) )
			ulAddr = (unsigned long) pst_HDCP22_Prt0_Reg - 0x1100;
		else if ( (addr >= 0x1900) && (addr <= (0x1900 + pH15A0HdmiRegCfg->hdcp22_prt1_reg_size) ) )
			ulAddr = (unsigned long) pst_HDCP22_Prt1_Reg - 0x1900;
		else
		{
			HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
			return RET_ERROR;
		}

		*value = *((volatile UINT32*)(ulAddr + addr));
	}
	else
	{
		HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
		return RET_ERROR;
	}

	return RET_OK;
}

int HDMI_H15A0_SetRegister(UINT32 addr , UINT32 value)
{
	unsigned long ulAddr;
	UINT8 slave =0;
	UINT8 reg =0;
	UINT8 data =0;
	UINT32 prt_selected, phy_en_prt0, phy_en_prt1;
	int phy_tmp_enabled = -1;

	if (addr <= 0xFFF)
	{
		if (pst_LINK0_Reg == NULL)	return RET_ERROR;

		if(addr <= pH15A0HdmiRegCfg->link_reg_size)
			ulAddr = (unsigned long) pst_LINK0_Reg;
		else if (addr <= (0x400 + pH15A0HdmiRegCfg->link1_reg_size) )
			ulAddr = (unsigned long) pst_LINK1_Reg - 0x400;
		else if (addr <= (0xc00 + pH15A0HdmiRegCfg->link_top_reg_size))
			ulAddr = (unsigned long) pst_LINK_TOP_Reg - 0xc00;
		else
		{
			HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
			return RET_ERROR;
		}

		*((volatile UINT32*)(ulAddr + addr)) = value ;
		//HDMI_DEBUG("[HDMI]  %s : %d  Addr  = 0x%x , Value = 0x%x  \n", __F__, __L__, addr, value);
	}
	else if (addr >= 0x3800 && addr <=0x39FF)
	{
		slave = ((addr &0xff00)>>8)<<1;
		reg = (addr &0xff);

		if(slave == (0x38<<1) )
			prt_selected = 0;
		else
			prt_selected = 1;

		LINK_NUM_H15_RdFL(0, phy_00);
		LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

		if (!( (( prt_selected == 0) && phy_en_prt0) || ( ( prt_selected == 1) && phy_en_prt1)) )
		{
			HDMI_TRACE("[HDMI] %s : Link Port[%d] Not Enabled \n", __F__, prt_selected);
			HDMI_TRACE("[HDMI] Enable selected port [%d] \n", prt_selected);

			phy_tmp_enabled = prt_selected;

			if (prt_selected == 0)
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
				LINK_NUM_H15_WrFL(0, phy_00);
			}
			else if (prt_selected == 1)
			{
				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
				LINK_NUM_H15_WrFL(1, phy_00);
			}

			OS_MsecSleep(5);	// ms delay
		}

		slave = 0x38<<1;
		data = (UINT8)(value &0xff);

		REG_WriteI2C(PHY_REG_H15_I2C_IDX, slave, reg, data);
		//HDMI_DEBUG("[HDMI]  %s : %d  slave = 0x%x reg = 0x%x, data = 0x%x \n", __F__, __L__, slave, reg, data);

		if (phy_tmp_enabled != -1)
		{
			HDMI_TRACE("[HDMI] Disable selected port [%d] \n", prt_selected);

			if (prt_selected == 0)
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
				LINK_NUM_H15_WrFL(0, phy_00);
			}
			else if (prt_selected == 1)
			{
				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
				LINK_NUM_H15_WrFL(1, phy_00);
			}
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		if ( (addr >= 0x1100) && (addr <= (0x1100 + pH15A0HdmiRegCfg->hdcp22_prt0_reg_size) ) )
			ulAddr = (unsigned long) pst_HDCP22_Prt0_Reg - 0x1100;
		else if ( (addr >= 0x1900) && (addr <= (0x1900 + pH15A0HdmiRegCfg->hdcp22_prt1_reg_size) ) )
			ulAddr = (unsigned long) pst_HDCP22_Prt1_Reg - 0x1900;
		else
		{
			HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
			return RET_ERROR;
		}

		*((volatile UINT32*)(ulAddr + addr)) = value ;
	}
	else
	{
		HDMI_WARN("[HDMI]  %s : %d  Addr  = 0x%x  INVALID \n", __F__, __L__, addr);
		return RET_ERROR;
	}

	return RET_OK;
}

/**
 * @brief PhyOn function when hdmi5v detected (power on with out run-reset)
 *
 * @param port_num
 *
 * @return
 */
static int _HDMI_H15A0_PhyOn_5V(int port_num)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(port_num, phy_00);
	if (LINK_NUM_H15_Rd_fld(port_num, phy_00, hpd_100k_en) || LINK_NUM_H15_Rd_fld(port_num, phy_00, hpd_1k_en) || LINK_NUM_H15_Rd_fld(port_num, phy_00, hpd_out_oen))
	{
		HDMI_NOTI(" ########## H15 Register BUG [port_num] !!!! [0x%x] ##################\n", LINK_NUM_H15_Rd(port_num, phy_00) );

		/* register bug ??? */
		LINK_NUM_H15_Wr01(port_num, phy_00, hpd_100k_en, 0x0 );
		LINK_NUM_H15_Wr01(port_num, phy_00, hpd_1k_en, 0x0 );
		LINK_NUM_H15_Wr01(port_num, phy_00, hpd_out_oen, 0x0 );
		LINK_NUM_H15_WrFL(port_num, phy_00);
	}

	/* H15 New power on sequence */
	if(_HDMI_H15A0_Phy_PDB_WA == 0)
	{
		LINK_NUM_H15_RdFL(port_num, phy_00);
		LINK_NUM_H15_Wr01(port_num, phy_00, phy_pdb_man_mode, 0x1);			//PHY PDB Manual Mode On
		LINK_NUM_H15_WrFL(port_num, phy_00);
	}
	LINK_NUM_H15_RdFL(port_num, phy_00);
	LINK_NUM_H15_Wr01(port_num, phy_00, phy_pdb, 0x1);			//PHY PDB ON
	LINK_NUM_H15_WrFL(port_num, phy_00);
	LINK_NUM_H15_RdFL(port_num, phy_00);
	LINK_NUM_H15_Wr01(port_num, phy_00, phy_pdb, 0x0);			//PHY PDB OFF
	LINK_NUM_H15_WrFL(port_num, phy_00);
	LINK_NUM_H15_RdFL(port_num, phy_00);
	LINK_NUM_H15_Wr01(port_num, phy_00, phy_pdb, 0x1);			//PHY PDB ON
	LINK_NUM_H15_WrFL(port_num, phy_00);

	OS_UsecDelay(100);

	LINK_NUM_H15_RdFL(port_num, phy_00);
	LINK_NUM_H15_Wr01(port_num, phy_00, phy_i2c_rstn, 0x1);			//PHY RESET
	LINK_NUM_H15_WrFL(port_num, phy_00);

	//	LINK_NUM_H15_Wr01(port_num, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
	LINK_NUM_H15_RdFL(port_num, phy_00);
	LINK_NUM_H15_Wr01(port_num, phy_00, phy_eq_odt_sel, 0x2);
	LINK_NUM_H15_WrFL(port_num, phy_00);

	LINK_NUM_H15_RdFL(port_num, phy_00);
	HDMI_DEBUG("-- HDMI_PHY: Port[%d] :phy_00[0x%x]\n", port_num, LINK_NUM_H15_Rd(port_num ,phy_00));

	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
		_HDMI_H15B0_PhyRunReset();	// init Phy register
	else
		_HDMI_H15A0_PhyRunReset();	// init Phy register


	if(port_num == 0)
	{
		LINK_TOP_H15_RdFL(link_top_03);
		LINK_TOP_H15_Wr01(link_top_03, link_display_on_prt0, 0x1);
		LINK_TOP_H15_WrFL(link_top_03);
	}
	else {

		LINK_TOP_H15_RdFL(link_top_03);
		LINK_TOP_H15_Wr01(link_top_03, link_display_on_prt1, 0x1);
		LINK_TOP_H15_WrFL(link_top_03);
	}

	return ret;
}

/* not used */
static int _HDMI_H15A0_PhyOn(int port_num)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:

			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
			LINK_NUM_H15_Wr01(0, phy_00, phy_eq_odt_sel, 0x2);
			LINK_NUM_H15_WrFL(0, phy_00);

			OS_UsecDelay(100);
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_rstn, 0x1);			//PHY RESET
			LINK_NUM_H15_WrFL(0, phy_00);

			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
				_HDMI_H15B0_PhyRunReset();	// init Phy register
			else
				_HDMI_H15A0_PhyRunReset();	// init Phy register
			break;

		case 1:
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
			LINK_NUM_H15_Wr01(1, phy_00, phy_eq_odt_sel, 0x2);
			LINK_NUM_H15_WrFL(1, phy_00);
			OS_UsecDelay(100);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_rstn, 0x1);			//PHY RESET
			LINK_NUM_H15_WrFL(1, phy_00);

			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0))
				_HDMI_H15B0_PhyRunReset();	// init Phy register
			else
				_HDMI_H15A0_PhyRunReset();	// init Phy register
			break;

		default :
			ret = RET_ERROR;
			break;


	}

	return ret;
}

static int _HDMI_H15A0_PhyOff(int port_num)
{
	int ret = RET_OK;
#if 1
	int cd_sense;
#endif
	HDMI_DEBUG("[%s] Entered with port[%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			//ARC source
			LINK_NUM_H15_RdFL(0, system_control_00);
			LINK_NUM_H15_Wr01(0, system_control_00, reg_arc_src, 0x0);
			LINK_NUM_H15_WrFL(0, system_control_00);

			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x0);			//PHY PDB ON
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
			LINK_NUM_H15_Wr01(0, phy_00, phy_eq_odt_sel, 0x0);
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_rstn, 0x0);			//PHY RESET
			LINK_NUM_H15_WrFL(0, phy_00);
			break;

		case 1:
			LINK_NUM_H15_RdFL(1, phy_00);
#if 1
			LINK_NUM_H15_RdFL(1, system_control_00);
			LINK_NUM_H15_Rd01(1, system_control_00, reg_cd_sense, cd_sense);
			if (!cd_sense)
				LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x0);			//PHY PDB ON
#else
			LINK_NUM_H15_Wr01(0, phy_06, phy_pdb_prt3, 0x0);			//PHY PDB ON
#endif
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
			LINK_NUM_H15_Wr01(1, phy_00, phy_eq_odt_sel, 0x0);
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_rstn, 0x0);			//PHY RESET
			LINK_NUM_H15_WrFL(1, phy_00);
			break;

		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

#ifdef ODT_PDB_OFF_ON_WORKAROUND
static int _HDMI_H15A0_Phy_Enable_Register_Access(int port_num)
{
	int ret = RET_OK;

//	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
			LINK_NUM_H15_WrFL(1, phy_00);

			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_WrFL(0, phy_00);
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x0);			//PHY PDB OFF
			LINK_NUM_H15_WrFL(0, phy_00);
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_WrFL(0, phy_00);

			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
			LINK_NUM_H15_WrFL(0, phy_00);
			OS_UsecDelay(100);
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_rstn, 0x1);			//PHY RESET
			LINK_NUM_H15_WrFL(0, phy_00);

			break;

		case 1:
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_enable, 0x0);		//PHY Enable
			LINK_NUM_H15_WrFL(0, phy_00);

			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_WrFL(1, phy_00);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x0);			//PHY PDB OFF
			LINK_NUM_H15_WrFL(1, phy_00);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x1);			//PHY PDB ON
			LINK_NUM_H15_WrFL(1, phy_00);

			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_enable, 0x1);		//PHY Enable
			LINK_NUM_H15_WrFL(1, phy_00);
			OS_UsecDelay(100);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_rstn, 0x1);			//PHY RESET
			LINK_NUM_H15_WrFL(1, phy_00);

			break;

		default :
			ret = RET_ERROR;
			break;
	}

	return ret;
}

static int _HDMI_H15A0_EQ_PDB_control(int reset)
{
	int ret = RET_OK;
	HDMI_DEBUG("[%s] Entered with [%d]\n",__func__, reset);

	if(reset)
	{
		if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
			PHY_REG_H15B0_RdFL(eq_i2c_pdb_d0);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_i2c_pdb_d0_mode,0x1);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_man_pdb_d0,0x0);
			PHY_REG_H15B0_WrFL(eq_i2c_pdb_d0);

			PHY_REG_H15B0_RdFL(eq_i2c_pdb_dck);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_i2c_pdb_dck_mode,0x1);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_man_pdb_dck,0x0);
			PHY_REG_H15B0_WrFL(eq_i2c_pdb_dck);
		}
		else {
			PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
			PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

			PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
			PHY_REG_H15A0_WrFL(pdb_dck_man_sel);
		}
	}
	else
	{
		if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
			PHY_REG_H15B0_RdFL(eq_i2c_pdb_d0);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_i2c_pdb_d0_mode,0x0);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_man_pdb_d0,0x1);
			PHY_REG_H15B0_WrFL(eq_i2c_pdb_d0);

			PHY_REG_H15B0_RdFL(eq_i2c_pdb_dck);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_i2c_pdb_dck_mode,0x0);
			PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_man_pdb_dck,0x1);
			PHY_REG_H15B0_WrFL(eq_i2c_pdb_dck);
		}
		else {
			PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x0);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x1);
			PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

			PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x0);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x1);
			PHY_REG_H15A0_WrFL(pdb_dck_man_sel);
		}
	}

	return ret;
}
#endif

static int _HDMI_H15A0_ResetPortControl(int reset_enable)
{
	int ret = RET_OK;

//	HDMI_PRINT("[%s] Entered with [%d] \n",__func__, reset_enable);

	if (reset_enable)
	{
		LINK_TOP_H15_RdFL(link_top_01);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x0);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x0);
		if(_gH15A0HDMIPhyInform.hpd_pol[1])
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt1, 0x0);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt1, 0x0);
		LINK_TOP_H15_WrFL(link_top_01);

		LINK_TOP_H15_RdFL(link_top_01);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x0);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x0);
		if(_gH15A0HDMIPhyInform.hpd_pol[0])
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt0, 0x0);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt0, 0x0);
		LINK_TOP_H15_WrFL(link_top_01);
	}
	else
	{
		LINK_TOP_H15_RdFL(link_top_01);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x1);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x1);
		if(_gH15A0HDMIPhyInform.hpd_pol[1])
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt1, 0x1);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt1, 0x1);
		LINK_TOP_H15_WrFL(link_top_01);

		LINK_TOP_H15_RdFL(link_top_01);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x1);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x1);
		if(_gH15A0HDMIPhyInform.hpd_pol[0])
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt0, 0x1);
		LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt0, 0x1);
		LINK_TOP_H15_WrFL(link_top_01);
	}

	return ret;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_ResetPort(int port_num)
{
	int ret = RET_OK;
	int cd_sense, cbus_conn_done, cbus_warb_done, hpd_in_prt3;

	HDMI_PRINT("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt0, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt0, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(10);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt0, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt0, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt1, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt1, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(10);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt1, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_cbus_prt1, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}
#endif	//#ifdef NOT_USED_NOW

static int _HDMI_H15A0_TMDS_ResetPort(int port_num)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_TMDS_ResetPort_Control(int port_num, int reset)
{
	int ret = RET_OK;

	//	HDMI_DEBUG("[%s] Entered with [%d][%d] \n",__func__, port_num, reset);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}
#endif

static int _HDMI_H15A0_HDCP_ResetPort_Control(int port_num, int reset)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d][%d] \n",__func__, port_num, reset);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_TMDS_HDCP_ResetPort_Control(int port_num, int reset)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d][%d] \n",__func__, port_num, reset);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, reset);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt1, reset);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, reset);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}
#endif	//#ifdef NOT_USED_NOW

static int _HDMI_H15A0_HDCP_ResetPort(int port_num)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt1, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_TMDS_HDCP_ResetPort(int port_num)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, port_num);

	switch(port_num)
	{
		case 0:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		case 1:
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x0);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x0);
			LINK_TOP_H15_WrFL(link_top_01);
			OS_MsecSleep(5);	// ms delay
			LINK_TOP_H15_RdFL(link_top_01);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_tmds_prt0, 0x1);
			LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_hdcp_prt0, 0x1);
			LINK_TOP_H15_WrFL(link_top_01);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}
#endif

static int _HDMI_H15A0_Get_EDID_Rd(int port_num , int *edid_rd_done, int *edid_rd_cnt)
{
	int ret = RET_OK;

	switch(port_num)
	{
		case 0:
			LINK_NUM_H15_RdFL(0, system_control_00);
			LINK_NUM_H15_Rd01(0, system_control_00, reg_edid_rd_done, *edid_rd_done);
			LINK_NUM_H15_Rd01(0, system_control_00, reg_edid_rd_cnt, *edid_rd_cnt);
			break;
		case 1:
			LINK_NUM_H15_RdFL(1, system_control_00);
			LINK_NUM_H15_Rd01(1, system_control_00, reg_edid_rd_done, *edid_rd_done);
			LINK_NUM_H15_Rd01(1, system_control_00, reg_edid_rd_cnt, *edid_rd_cnt);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

static int _HDMI_H15A0_Get_HDCP_info(int port_num , UINT32 *hdcp_authed, UINT32 *hdcp_enc_en)
{
	int ret = RET_OK;

	switch(port_num)
	{
		case 0:
			LINK_NUM_H15_RdFL(0, hdcp_00);
			LINK_NUM_H15_Rd01(0, hdcp_00, reg_hdcp_authed, *hdcp_authed);
			LINK_NUM_H15_Rd01(0, hdcp_00, reg_hdcp_enc_en, *hdcp_enc_en);
			break;
		case 1:
			LINK_NUM_H15_RdFL(1, hdcp_00);
			LINK_NUM_H15_Rd01(1, hdcp_00, reg_hdcp_authed, *hdcp_authed);
			LINK_NUM_H15_Rd01(1, hdcp_00, reg_hdcp_enc_en, *hdcp_enc_en);
			break;
		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

int HDMI_H15A0_Write_HDCP_Key(UINT8 *hdcp_key_data)
{
	int ret = RET_OK;

	unsigned char hdcp_data_wr[LX_HDCP_KEY_SIZE];
	int count;

	OS_LockMutex(&g_HDMI_Sema);

	HDMI_DEBUG("[%s] Entered \n",__func__);

	if (hdcp_key_data == NULL)
	{
		OS_UnlockMutex(&g_HDMI_Sema);

		return RET_ERROR;
	}
	if (pHDCP_Key_addr == NULL)
	{
		HDMI_ERROR( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );

		OS_UnlockMutex(&g_HDMI_Sema);

		return RET_ERROR;
	}

	/*
	for (count = 0; count < LX_HDCP_KEY_SIZE; count += 4) {
		memcpy((void *)(hdcp_data_wr+count), (void *)(hdcp_key_data+284-count), 4);
	}
	*/
		/*
	for (count = 0; count < LX_HDCP_KEY_SIZE; count ++)
		hdcp_data_wr[count] = hdcp_key_data[LX_HDCP_KEY_SIZE -1 -count];
*/
	hdcp_data_wr[0] = 0; 	// check-sum ???

	for (count = 1; count < 6; count ++)
		hdcp_data_wr[count] = hdcp_key_data[count - 1];

	for (count = 6; count < (LX_HDCP_KEY_SIZE - 2); count ++)
		hdcp_data_wr[count] = hdcp_key_data[count + 2];
	hdcp_data_wr[286] = 0;
	hdcp_data_wr[287] = 0;

	memcpy((void *)pHDCP_Key_addr, (void *)hdcp_data_wr, 288);

	// Reset HPD out to '0', for source device to restart HDCP Authentification.
	_HDMI_H15A0_HDCP_ResetPort_Control(0,0);
	_HDMI_H15A0_HDCP_ResetPort_Control(1,0);

	OS_MsecSleep(5);	// ms delay

	_HDMI_H15A0_HDCP_ResetPort_Control(0,1);
	_HDMI_H15A0_HDCP_ResetPort_Control(1,1);

	//_HDMI_H15A0_Set_HPD_Out_A4P(0,0,0,0);
	_HDMI_H15A0_Set_HPD_Out_A4P(!_gH15A0HDMIPhyInform.hpd_pol[0], \
			!_gH15A0HDMIPhyInform.hpd_pol[1]);
	_HDMI_H15A0_Set_GPIO_HPD_Out(0, !_gH15A0HDMIPhyInform.hpd_pol[0]);	// disable HPD

	_gH15A0HDMIPhyInform.hpd_hdcp22_control[0] = 0;
#ifndef USE_EXT_MHL_IC
	_gH15A0HDMIPhyInform.hpd_hdcp22_control[1] = 0;
#endif

	OS_MsecSleep(10);	// ms delay

	OS_UnlockMutex(&g_HDMI_Sema);

	return ret;
}

int HDMI_H15A0_Read_HDCP_Key(UINT8 *hdcp_key_data)
{
	int ret = RET_OK;

	unsigned char hdcp_data_rd[LX_HDCP_KEY_SIZE];
	int count;

	HDMI_DEBUG("[%s] Entered \n",__func__);

	if (hdcp_key_data == NULL)
		return RET_ERROR;

	if (pHDCP_Key_addr == NULL)
	{
		HDMI_DEBUG( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
		return RET_ERROR;
	}

	memcpy((void *)hdcp_data_rd, (void *)pHDCP_Key_addr, 288);

	/*
	for (count = 0; count < LX_HDCP_KEY_SIZE; count += 4) {
		memcpy((void *)(hdcp_key_data+count), (void *)(hdcp_data_rd+284-count), 4);
	}
	*/
	/*
	for (count = 0; count < LX_HDCP_KEY_SIZE; count ++)
		hdcp_key_data[count] = hdcp_data_rd[LX_HDCP_KEY_SIZE -1 -count];
		*/
	for (count = 0; count < 5; count ++)
		hdcp_key_data[count] = hdcp_data_rd[count + 1];
	for (count = 5; count < 8; count ++)
		hdcp_key_data[count] = 0;
	for (count = 8; count < LX_HDCP_KEY_SIZE; count ++)
		hdcp_key_data[count] = hdcp_data_rd[count - 2];

	return ret;
}

int HDMI_H15A0_Read_EDID_Data(int port_num , UINT8 *pedid_data)
{
	int ret = RET_OK;
	int count = 0;

	unsigned char edid_data_rd[LX_EDID_DATA_SIZE];

	PRT_NUM_CONVERT(port_num);

	HDMI_DEBUG("[%s] Entered with port [%d] \n",__func__, port_num);

	if (pedid_data == NULL)
		return RET_ERROR;

	if (pEDID_data_addr_port0 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port0 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
		return RET_ERROR;
	}
	if (pEDID_data_addr_port1 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port1 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
		return RET_ERROR;
	}

	if (port_num == 0)
		memcpy((void *)edid_data_rd, (void *)pEDID_data_addr_port0, 256);
	else if (port_num == 1)
		memcpy((void *)edid_data_rd, (void *)pEDID_data_addr_port1, 256);
	else
		return RET_ERROR;

	/*
	for (count = 0; count < 256; count += 4) {
		memcpy((void *)(pedid_data+count), (void *)(edid_data_rd+252-count), 4);
	}
	*/
	for (count = 0; count < LX_EDID_DATA_SIZE; count ++)
		pedid_data[count] = edid_data_rd[count];
//		pedid_data[count] = edid_data_rd[LX_EDID_DATA_SIZE - 1 - count];

	return ret;
}

int HDMI_H15A0_Write_EDID_Data(int port_num , UINT8 *pedid_data)
{
	int ret = RET_OK;
	int count = 0;
	UINT8 cs;

	unsigned char edid_data_wr[LX_EDID_DATA_SIZE];

	PRT_NUM_CONVERT(port_num);

	HDMI_DEBUG("[%s] Entered with port [%d] \n",__func__, port_num);

	if (pedid_data == NULL)
		return RET_ERROR;

	if (pEDID_data_addr_port0 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port0 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
		return RET_ERROR;
	}
	if (pEDID_data_addr_port1 == NULL)
	{
		HDMI_ERROR( "[%s][%d] pEDID_data_addr_port1 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
		return RET_ERROR;
	}

	//-----------------------------
	//First Block checksum
	//-----------------------------
	for (cs = 0, count = 0x00; count < 0x7F; count++)
		cs += pedid_data[count];
	pedid_data[0x7F] = 0x100 -cs;

	HDMI_DEBUG("First Block Check Sum : port [%d] : [0x%x]\n",port_num, pedid_data[0x7F]);

	//-----------------------------
	//Second Block checksum
	//-----------------------------
	for (cs = 0, count = 0x80; count < 0xFF; count++)
		cs += pedid_data[count];
	pedid_data[0xFF] = 0x100 -cs;

	HDMI_DEBUG("Second Block Check Sum : port [%d] : [0x%x]\n",port_num, pedid_data[0xFF]);
	/*
	for (count = 0; count < 256; count += 4)
		memcpy((void *)(edid_data_wr+count), (void *)(pedid_data+252-count), 4);
		*/
	for (count = 0; count < LX_EDID_DATA_SIZE; count ++)
		edid_data_wr[count] = pedid_data[count];
//		edid_data_wr[count] = pedid_data[LX_EDID_DATA_SIZE - 1 - count];

	if (port_num == 0)
		memcpy((void *)pEDID_data_addr_port0, (void *)edid_data_wr, 256);
	else if (port_num == 1)
		memcpy((void *)pEDID_data_addr_port1, (void *)edid_data_wr, 256);
	else
		return RET_ERROR;

	return ret;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_Write_EDID_Data_A4P(UINT32 *pedid_data)
{
	int ret = RET_OK;

	if (pedid_data == NULL)
		return RET_ERROR;

	memcpy((void *)pEDID_data_addr_port0, (void *)pedid_data, 256*4);

	return ret;
}
#endif	//#ifdef NOT_USED_NOW

static int _HDMI_H15A0_Get_HDMI5V_Info_A4P(int *pHDMI5V_Status_PRT0, int *pHDMI5V_Status_PRT1)
{
	int ret = RET_OK;

	LINK_NUM_H15_RdFL(0, system_control_00);
	LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi5v, *pHDMI5V_Status_PRT0);
	LINK_NUM_H15_RdFL(1, system_control_00);
	LINK_NUM_H15_Rd01(1, system_control_00, reg_hdmi5v, *pHDMI5V_Status_PRT1);

	return ret;
}

static int _HDMI_H15A0_Get_HDMI5V_Info(int port_num, int *pHDMI5V_Status)
{
	int ret = RET_OK;

//	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(port_num, system_control_00);
	LINK_NUM_H15_Rd01(port_num, system_control_00, reg_hdmi5v, *pHDMI5V_Status);

	return ret;
}

static int _HDMI_H15A0_Get_HPD_Out_A4P(int *pHPD_Out_PRT0, int *pHPD_Out_PRT1)
{
	int ret = RET_OK;

	LINK_NUM_H15_RdFL(0, phy_00);
	LINK_NUM_H15_Rd01(0, phy_00, hpd_out, *pHPD_Out_PRT0);

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Rd01(1, phy_00, hpd_out, *pHPD_Out_PRT1);

	return ret;
}

static int _HDMI_H15A0_Set_HPD_Out(int prt_sel, int hpd_out_value)
{
	int ret = RET_OK;

	//PRT_NUM_CONVERT(prt_sel);
	HDMI_DEBUG("[%s] Entered with port[%d] hpd_out[%d]\n",__func__, prt_sel, hpd_out_value);

	if(PRT_NUM_INVALID(prt_sel))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(prt_sel, phy_00);
	LINK_NUM_H15_Wr01(prt_sel, phy_00, hpd_out, hpd_out_value);
	LINK_NUM_H15_WrFL(prt_sel, phy_00);

	return ret;
}

static int _HDMI_H15A0_Set_GPIO_HPD_Out(int prt_sel, int hpd_out_value)
{
	int ret = RET_OK;

	//PRT_NUM_CONVERT(prt_sel);
	HDMI_NOTI("[%s] Entered with port[%d] hpd_out[%d]\n",__func__, prt_sel, hpd_out_value);

	if(PRT_NUM_INVALID(prt_sel))
		return RET_ERROR;

	if(prt_sel != 0)
		return RET_ERROR;

	GPIO_DevSetValue(39, hpd_out_value);
	GPIO_DevSetValue(35, hpd_out_value);

	return ret;
}

static int _HDMI_H15A0_Set_HPD_Out_A4P(int HPD_Out_PRT0, int HPD_Out_PRT1 )
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered with [%d][%d] \n",__func__, HPD_Out_PRT0, HPD_Out_PRT1);

	LINK_NUM_H15_RdFL(0, phy_00);
	LINK_NUM_H15_Wr01(0, phy_00, hpd_out, HPD_Out_PRT0);
	LINK_NUM_H15_WrFL(0, phy_00);

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Wr01(1, phy_00, hpd_out, HPD_Out_PRT1);
	LINK_NUM_H15_WrFL(1, phy_00);

	return ret;
}

int HDMI_H15A0_Get_Phy_Status(LX_HDMI_PHY_INFORM_T *sp_hdmi_phy_status)
{
	if (sp_hdmi_phy_status == NULL)
		return RET_ERROR;

	if (!_gH15A0HDMI_thread_running)
	{
		int port_num;
		UINT32 temp;
		int	access_phy[2];
		UINT32 up_freq = 0,	down_freq = 0;

#ifdef H15_BRINGUP_TODO
		LINK_NUM_H15_RdFL(0, system_control_00);
		LINK_NUM_H15_Rd01(0, system_control_00, reg_prt_sel, _gH15A0HDMIPhyInform.prt_sel);
#endif

		LINK_NUM_H15_RdFL(0, phy_00);
		LINK_NUM_H15_Rd01(0, phy_00, phy_pdb, _gH15A0HDMIPhyInform.phy_pdb[0]);			//PHY PDB ON
		LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, _gH15A0HDMIPhyInform.phy_enable[0]);		//PHY Enable
		LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_rstn, _gH15A0HDMIPhyInform.phy_rstn[0]);			//PHY RESET
		LINK_NUM_H15_Rd01(0, phy_00, hpd_in, _gH15A0HDMIPhyInform.hpd_in[0]);			//PHY HPD IN

		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, _gH15A0HDMIPhyInform.phy_pdb[1]);			//PHY PDB ON
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, _gH15A0HDMIPhyInform.phy_enable[1]);		//PHY Enable
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_rstn, _gH15A0HDMIPhyInform.phy_rstn[1]);			//PHY RESET
		LINK_NUM_H15_Rd01(1, phy_00, hpd_in, _gH15A0HDMIPhyInform.hpd_in[1]);			//PHY HPD IN

		LINK_NUM_H15_Rd01(0, system_control_00, reg_scdt, _gH15A0HDMIPhyInform.scdt[0]);
		LINK_NUM_H15_Rd01(1, system_control_00, reg_scdt, _gH15A0HDMIPhyInform.scdt[1]);

		LINK_NUM_H15_Rd01(0, system_control_00, reg_hdmi_mode, _gH15A0HDMIPhyInform.hdmi_mode[0]);
		LINK_NUM_H15_Rd01(1, system_control_00, reg_hdmi_mode, _gH15A0HDMIPhyInform.hdmi_mode[1]);

		_HDMI_H15A0_Get_HDMI5V_Info_A4P(&_gH15A0HDMIPhyInform.hdmi5v[0], &_gH15A0HDMIPhyInform.hdmi5v[1]);

		_HDMI_H15A0_Get_HPD_Out_A4P(&_gH15A0HDMIPhyInform.hpd_out[0], &_gH15A0HDMIPhyInform.hpd_out[1]);

		for (port_num = 0; port_num < 2;port_num ++)
		{
			_HDMI_H15A0_Get_EDID_Rd(port_num , &_gH15A0HDMIPhyInform.edid_rd_done[port_num], &temp);
			_HDMI_H15A0_Get_HDCP_info(port_num , &_gH15A0HDMIPhyInform.hdcp_authed[port_num], &_gH15A0HDMIPhyInform.hdcp_enc_en[port_num]);
		}

		LINK_NUM_H15_RdFL(0, system_control_00);
		LINK_NUM_H15_Rd01(0, system_control_00, reg_cd_sense, _gH15A0HDMIPhyInform.cd_sense);
		LINK_NUM_H15_RdFL(0, cbus_01);
		LINK_NUM_H15_Rd01(0, cbus_01, reg_cbus_conn_done, _gH15A0HDMIPhyInform.cbus_conn_done);
		LINK_NUM_H15_Rd01(0, cbus_01, reg_cbus_warb_done, _gH15A0HDMIPhyInform.cbus_warb_done);
		LINK_NUM_H15_Rd01(0, cbus_01, reg_cbus_disconn, _gH15A0HDMIPhyInform.cbus_disconn);			// CBUS DisConnected ???
		LINK_NUM_H15_RdFL(0, cbus_00);
		LINK_NUM_H15_Rd01(0, cbus_00, reg_phy_sink_cbus_zdis, _gH15A0HDMIPhyInform.phy_sink_cbus_zdis);
		LINK_NUM_H15_Rd01(0, cbus_00, reg_cbus_st, _gH15A0HDMIPhyInform.cbus_st);

		for (port_num=0;port_num<2;port_num++)
			access_phy[port_num] = _gH15A0HDMIPhyInform.phy_pdb[port_num] && _gH15A0HDMIPhyInform.phy_enable[port_num] && _gH15A0HDMIPhyInform.phy_rstn[port_num] ;

		if (access_phy[0] || access_phy[1] )
		{
			_gH15A0HDMIPhyInform.tmds_clock[0] = _HDMI_H15_Get_Clock_Freq();
		}
		else
			_gH15A0HDMIPhyInform.tmds_clock[0] = 0;

		LINK_NUM_H15_RdFL(0, cbus_13);
		LINK_NUM_H15_Rd01(0, cbus_13, reg_state_sink_rcp, _gH15A0HDMIPhyInform.state_sink_rcp);
	}

	LINK_TOP_H15_RdFL(link_top_01);
	LINK_TOP_H15_Rd01(link_top_01, link_sw_rstn_edid_prt0, _gH15A0HDMIPhyInform.rstn_edid[0]);
	LINK_TOP_H15_Rd01(link_top_01, link_sw_rstn_edid_prt1, _gH15A0HDMIPhyInform.rstn_edid[1]);

	memcpy(sp_hdmi_phy_status, &_gH15A0HDMIPhyInform, (size_t)sizeof(LX_HDMI_PHY_INFORM_T));


	return RET_OK;
}

static int _HDMI_H15A0_GetMHLConection(void)
{
	int cd_sense, /*hpd_in_prt3, prt_selected,*/ phy_en_prt0, phy_en_prt1, /* cbus_disconn, */ cbus_st;
	int cbus_warb_done;
	static int _mhl_stable_count = 0;
	static int cd_sense_changed = 0;
	static int mhl_pdb_delayed_control = 0;
	static unsigned int cdsense_detection_time = 0;
	static unsigned int cdsense_current_time = 0;
	/* Port 1 MHL Connection */
	LINK_NUM_H15_RdFL(1, system_control_00);
	LINK_NUM_H15_Rd01(1, system_control_00, reg_cd_sense, cd_sense);
#ifdef H15_BRINGUP_TODO
	LINK_NUM_H15_Rd01(1, system_control_00, reg_prt_sel, prt_selected);
#endif

	LINK_NUM_H15_RdFL(1, cbus_01);
	LINK_NUM_H15_Rd01(1, cbus_01, reg_cbus_conn_done, _gH15A0HDMIPhyInform.cbus_conn_done);
	//	LINK_NUM_H15_Rd01(0, cbus_01, reg_cbus_warb_done, _gH15A0HDMIPhyInform.cbus_warb_done);
	LINK_NUM_H15_Rd01(1, cbus_01, reg_cbus_warb_done, cbus_warb_done);
	LINK_NUM_H15_Rd01(1, cbus_01, reg_cbus_disconn, _gH15A0HDMIPhyInform.cbus_disconn);			// CBUS DisConnected ???
	LINK_NUM_H15_RdFL(1, cbus_00);
	LINK_NUM_H15_Rd01(1, cbus_00, reg_phy_sink_cbus_zdis, _gH15A0HDMIPhyInform.phy_sink_cbus_zdis);
	//LINK_NUM_H15_Rd01(0, cbus_00, reg_cbus_st, _gH15A0HDMIPhyInform.cbus_st);
	LINK_NUM_H15_Rd01(1, cbus_00, reg_cbus_st, cbus_st);

	_gH15A0HDMIPhyInform.cbus_00 = LINK_NUM_H15_Rd(1, cbus_00);
	_gH15A0HDMIPhyInform.cbus_01 = LINK_NUM_H15_Rd(1, cbus_01);

	LINK_NUM_H15_RdFL(1, cbus_13);
	LINK_NUM_H15_Rd01(1, cbus_13, reg_state_sink_rcp, _gH15A0HDMIPhyInform.state_sink_rcp);

#if 0
	if (cd_sense != _gH15A0HDMIPhyInform.cd_sense)
	{
		HDMI_NOTI("---- MHL CD SENSE : [%d] => [%d]\n", _gH15A0HDMIPhyInform.cd_sense, cd_sense);
		_gH15A0HDMIPhyInform.cd_sense = cd_sense;

		if (!gHDMI_Phy_Control.all_port_pdb_enable && _gH15A0HDMIPhyInform.cd_sense && !_gH15A0HDMIPhyInform.phy_pdb[1] )
		{
			HDMI_DEBUG("---- MHL port 1 phy pdb force ON : [%d] => [%d]\n", _gH15A0HDMIPhyInform.phy_pdb[1], 1);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 1);			//PHY PDB ON
			LINK_NUM_H15_WrFL(1, phy_00);
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, _gH15A0HDMIPhyInform.phy_pdb[1]);			//PHY PDB ON
		}

		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);

		if (!gHDMI_Phy_Control.all_port_pdb_enable)		// not used
		{
#ifdef H15_BRINGUP_TODO
			if ( (prt_selected==0x3) && phy_en_prt1)
#endif
			if (/* (prt_selected==0x3) &&*/ phy_en_prt1)
			{
				PHY_REG_H15A0_RdFL(dr_filter_ch0);
				PHY_REG_H15A0_RdFL(cr_icp_adj);
				if (cd_sense)
				{
#ifdef H15_CODE_FOR_MHL_CTS
#ifdef H15_CBUS_PDB_CTRL
					HDMI_DEBUG("---- PHY INIT for MHL,  PDB_D0_MAN_SEL :0x10, PDB_DCK_MAN_SEL :0x10\n");
					PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
					PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
					PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
					PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

					PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
					PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
					PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
					PHY_REG_H15A0_WrFL(pdb_dck_man_sel);
#endif
#endif
					HDMI_DEBUG("---- MHL port : cd sense ON [%s]\n", __func__);
					HDMI_DEBUG("---- MHL CD Sense dr_filter_ch0 : [%d]\n", 0x1);
					PHY_REG_H15A0_Wr01(dr_filter_ch0,dr_filter_ch0,0x1);
					HDMI_DEBUG("---- MHL CR_PLL Charge Pump Current Adjust : [%d]\n", 0x3);
					PHY_REG_H15A0_Wr01(cr_icp_adj,cr_icp_adj,0x3);
				}
				else
				{
					HDMI_DEBUG("---- HDMI Mode dr_filter_ch0 : [%d]\n", 0x7);
					PHY_REG_H15A0_Wr01(dr_filter_ch0,dr_filter_ch0,0x7);
					HDMI_DEBUG("---- HDMI Mode  CR_PLL Charge Pump Current Adjust : [%d]\n", 0x4);
					PHY_REG_H15A0_Wr01(cr_icp_adj,cr_icp_adj,0x4);
				}
				PHY_REG_H15A0_WrFL(dr_filter_ch0);
				PHY_REG_H15A0_WrFL(cr_icp_adj);
			}
		}
		else	// all port pdb enabled condition, if cbus change detected ?
		{
			cd_sense_changed = 1;

			if(cd_sense)
			{
			//	mhl_pdb_delayed_control = 1;	// disabled workaround for PRADA phone : PDB off just after cdsence detected.
				cdsense_detection_time = jiffies_to_msecs(jiffies);
			}
			else
			{
				mhl_pdb_delayed_control = 0;
			}
		}
	}
	if(cd_sense_changed)
	{
		int phy_pdb_prt1, phy_rstn_prt1;
		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, phy_pdb_prt1);			//PHY PDB ON
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_rstn, phy_rstn_prt1);			//PHY RESET

		if (phy_pdb_prt1 && phy_rstn_prt1)
		{

			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

			if (!phy_en_prt1)	// IF port1(C-BUS) is not enabled?
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

				_HDMI_H15A0_EnablePort(0, 0);
				_HDMI_H15A0_EnablePort(1, 1);
			}
			OS_MsecSleep(10); // delay after enable port and phy register write

			PHY_REG_H15A0_RdFL(dr_filter_ch0);
			PHY_REG_H15A0_RdFL(cr_icp_adj);
			PHY_REG_H15A0_RdFL(eq_rs_man);
			if (cd_sense)
			{
				HDMI_DEBUG("---- MHL port : cd sense ON [%s]\n", __func__);
				HDMI_DEBUG("---- MHL CD Sense dr_filter_ch0 : [%d]\n", 0x1);
				PHY_REG_H15A0_Wr01(dr_filter_ch0,dr_filter_ch0,0x1);
				HDMI_DEBUG("---- MHL CR_PLL Charge Pump Current Adjust : [%d]\n", 0x2);
				PHY_REG_H15A0_Wr01(cr_icp_adj,cr_icp_adj,0x2);	// 0x3 //140514 : MHL CT B2 and new board
				HDMI_DEBUG("---- MHL EQ_RS_MAN  : [%d]\n", 0x3);
				PHY_REG_H15A0_Wr01(eq_rs_man,eq_rs_man,0x3);	//from 0x3(default) // 0x5 //140514 : MHL CT B2 and new board


#ifdef H15_CODE_FOR_MHL_CTS
					// PDB off just after cd_sence detected : removed workaournd code for PRADA phone
					HDMI_DEBUG("---- CD_SENSE for MHL,  PDB_D0_MAN_SEL :0x10, PDB_DCK_MAN_SEL :0x10\n");
					PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
					PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
					PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
					PHY_REG_H15A0_WrFL(pdb_d0_man_sel);
					PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
					PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
					PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
					PHY_REG_H15A0_WrFL(pdb_dck_man_sel);

				HDMI_DEBUG("---- MHL ODC CTRL  : [%d]\n", 0x0);
				PHY_REG_H15A0_RdFL(odt_ctrl);
				PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x1);		// 0x0 //140514 : new board impedance 55=>50Ohm
				PHY_REG_H15A0_WrFL(odt_ctrl);
#endif
				HDMI_DEBUG("---- MHL DR_N1  : [%d]\n", 0x0);
				PHY_REG_H15A0_RdFL(dr_n1);
				PHY_REG_H15A0_Wr01(dr_n1,dr_n1,0x0);	// for 15m cable (0x38/0x93)
				PHY_REG_H15A0_WrFL(dr_n1);
			}
			else
			{
				HDMI_DEBUG("---- HDMI Mode dr_filter_ch0 : [%d]\n", 0x7);
				PHY_REG_H15A0_Wr01(dr_filter_ch0,dr_filter_ch0,0x7);
				HDMI_DEBUG("---- HDMI Mode  CR_PLL Charge Pump Current Adjust : [%d]\n", 0x4);
				PHY_REG_H15A0_Wr01(cr_icp_adj,cr_icp_adj,0x4);
#ifdef H15_HDMI_SEMI_AUTO_EQ_CONTROL
				HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
				PHY_REG_H15A0_Wr01(eq_rs_man,eq_rs_man,0x3);
#else
				HDMI_DEBUG("---- HDMI EQ_RS_MAN  : [%d]\n", 0x3);
				/* BD570 EQ error : EQ RS value 0x7 (default 0x3) , 131114 */
				PHY_REG_H15A0_Wr01(eq_rs_man,eq_rs_man,0x3);
#endif
#ifdef H15_CODE_FOR_MHL_CTS
				HDMI_DEBUG("---- HDMI ODC CTRL  : [%d]\n", 0x1);
				PHY_REG_H15A0_RdFL(odt_ctrl);
				PHY_REG_H15A0_Wr01(odt_ctrl,odt_ctrl,0x1);
				PHY_REG_H15A0_WrFL(odt_ctrl);
#endif
				HDMI_DEBUG("---- HDMI DR_N1  : [%d]\n", 0x3);
				PHY_REG_H15A0_RdFL(dr_n1);
				//PHY_REG_H15A0_Wr01(dr_n1,dr_n1,0x3);	// for 15m cable (0x38/0x93)
				PHY_REG_H15A0_Wr01(dr_n1,dr_n1,0x7);	// for U+ 4K STB
				PHY_REG_H15A0_WrFL(dr_n1);
			}
			PHY_REG_H15A0_WrFL(dr_filter_ch0);
			PHY_REG_H15A0_WrFL(cr_icp_adj);
			PHY_REG_H15A0_WrFL(eq_rs_man);

			if (!phy_en_prt1)
			{
				_HDMI_H15A0_EnablePort(0, phy_en_prt0);
				_HDMI_H15A0_EnablePort(1, phy_en_prt1);
			}
			cd_sense_changed = 0;
		}
		else
		{
			HDMI_DEBUG("---- MHL cd sense , but port is off\n");
			cd_sense_changed = 0;
		}
	}
	if(mhl_pdb_delayed_control)
	{
		int phy_pdb_prt1, phy_rstn_prt1;
		cdsense_current_time = jiffies_to_msecs(jiffies);
		if( (cdsense_current_time - cdsense_detection_time) > 2000)		// odt pdb power down after 2sec (for PRADA, optimus 3D phones)
		{
			HDMI_DEBUG("---- MHL [%d sec] after cdsense \n", (cdsense_current_time - cdsense_detection_time));
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, phy_pdb_prt1);			//PHY PDB ON
			LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_rstn, phy_rstn_prt1);			//PHY RESET
			if(phy_pdb_prt1 && phy_rstn_prt1)
			{
				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable
				if(!phy_en_prt1)	// IF port3(C-BUS) is not enabled?
				{
					LINK_NUM_H15_RdFL(0, phy_00);
					LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable
					LINK_NUM_H15_RdFL(1, phy_00);
					LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable
					_HDMI_H15A0_EnablePort(0, 0);
					_HDMI_H15A0_EnablePort(1, 1);
				}
				OS_MsecSleep(10); // delay after enable port and phy register write
#ifdef H15_CODE_FOR_MHL_CTS
#ifdef H15_CBUS_PDB_CTRL
				HDMI_DEBUG("---- CD_SENSE for MHL,  PDB_D0_MAN_SEL :0x10, PDB_DCK_MAN_SEL :0x10\n");
				PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_d0_man_sel);
				PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_dck_man_sel);
#endif
#endif
				if(!phy_en_prt1)
				{
					_HDMI_H15A0_EnablePort(0, phy_en_prt0);
					_HDMI_H15A0_EnablePort(1, phy_en_prt1);
				}
				mhl_pdb_delayed_control = 0;
			}
			else
			{
				HDMI_DEBUG("---- MHL cd sense , but port is off\n");
			}
		}
	}

	if (cbus_warb_done != _gH15A0HDMIPhyInform.cbus_warb_done)
	{
		HDMI_NOTI("---- MHL warb done : [%d] => [%d]\n", _gH15A0HDMIPhyInform.cbus_warb_done, cbus_warb_done);
		_gH15A0HDMIPhyInform.cbus_warb_done = cbus_warb_done;

#ifdef H15_CODE_FOR_MHL_CTS
#ifdef H15_CBUS_PDB_CTRL
		// Move to Interrupt
		if ( cbus_warb_done == 1)
		{
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

			if (!phy_en_prt1)	// IF port3(C-BUS) is not enabled?
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

				_HDMI_H15A0_EnablePort(0, 0);
				_HDMI_H15A0_EnablePort(1, 1);
			}

			/*
			   HDMI_DEBUG("---- MHL warb_done:1, Set ODT PDB Mode to 0x00\n");
			   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
			   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
			   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x0);
			   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);
			 */

			HDMI_DEBUG("---- MHL warb_done:1, PDB_D0_MAN_SEL :0x11, PDB_DCK_MAN_SEL :0x10\n");
			PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
			PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x1);
			PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

			PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
			PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
			PHY_REG_H15A0_WrFL(pdb_dck_man_sel);

			if (!phy_en_prt1)
			{
				_HDMI_H15A0_EnablePort(0, phy_en_prt0);
				_HDMI_H15A0_EnablePort(1, phy_en_prt1);
			}
		}
		else if (_gH15A0HDMIPhyInform.cd_sense)	// cd_sense, but not connection lost
#endif
		{
#if 0
			int phy_pdb_prt3, phy_rstn_prt3;
			LINK_NUM_H15_RdFL(0, phy_06);
			LINK_NUM_H15_Rd01(0, phy_06, phy_pdb_prt3, phy_pdb_prt3);			//PHY PDB ON
			LINK_NUM_H15_Rd01(0, phy_06, phy_rstn_prt3, phy_rstn_prt3);			//PHY RESET

			if (phy_pdb_prt3 && phy_rstn_prt3)
			{

				LINK_NUM_H15_RdFL(0, phy_06);
				LINK_NUM_H15_Rd01(0, phy_06, phy_enable_prt3, phy_en_prt3);		//PHY Enable

				if (!phy_en_prt3)	// IF port3(C-BUS) is not enabled?
				{
					LINK_NUM_H15_RdFL(0, phy_00);
					LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

					LINK_NUM_H15_RdFL(0, phy_02);
					LINK_NUM_H15_Rd01(0, phy_02, phy_enable_prt1, phy_en_prt1);		//PHY Enable

					LINK_NUM_H15_RdFL(0, phy_04);
					LINK_NUM_H15_Rd01(0, phy_04, phy_enable_prt2, phy_en_prt2);		//PHY Enable

					_HDMI_H15A0_EnablePort(0, 0);
					_HDMI_H15A0_EnablePort(1, 0);
					_HDMI_H15A0_EnablePort(2, 0);
					_HDMI_H15A0_EnablePort(3, 1);
				}

				/*
				   HDMI_DEBUG("---- MHL warb done:0 ,Set ODT PDB Mode to 0x10\n");
				   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
				   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
				   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
				   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);
				 */

				HDMI_DEBUG("---- MHL warb_done:0, PDB_D0_MAN_SEL :0x10, PDB_DCK_MAN_SEL :0x10\n");
				PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

				PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_dck_man_sel);

				if (!phy_en_prt3)
				{
					_HDMI_H15A0_EnablePort(0, phy_en_prt0);
					_HDMI_H15A0_EnablePort(1, phy_en_prt1);
					_HDMI_H15A0_EnablePort(2, phy_en_prt2);
					_HDMI_H15A0_EnablePort(3, phy_en_prt3);
				}
			}
#endif
		}
#endif
	}

	if (cbus_st != _gH15A0HDMIPhyInform.cbus_st)
	{
		HDMI_NOTI("---- MHL CBUS_ST : [%d] => [%d]\n", _gH15A0HDMIPhyInform.cbus_st, cbus_st);
		_gH15A0HDMIPhyInform.cbus_st = cbus_st;

		if (_gH15A0HDMIPhyInform.cbus_st == 3)
		{
			_gH15A0CHG_AVI_count_MHL = 0;
			_mhl_stable_count = 0;
		}
#ifdef H15_CODE_FOR_MHL_CTS
		else if (_gH15A0HDMIPhyInform.cbus_st  < 2)
			_HDMI_H15A0_Check_RAP_Content(1);
#endif
	}

	if ( _gH15A0HDMIPhyInform.cd_sense)	//MHL Mode
	{
		LINK_NUM_H15_RdFL(1, cbus_00);

		if (LINK_NUM_H15_Rd_fld(1, cbus_00, reg_cbus_disconn_en) == 0)
		{
			OS_MsecSleep(10);	// ms delay
			LINK_NUM_H15_RdFL(1, cbus_00);
			LINK_NUM_H15_Wr01(1, cbus_00, reg_cbus_disconn_en, 0x1);
			LINK_NUM_H15_WrFL(1, cbus_00);
		}

#ifdef H15_CODE_FOR_MHL_CTS
		_HDMI_H15A0_Check_RAP_Content(0);
#if 0
		{

			LINK_NUM_H15_RdFL(0, cbus_04);
			LINK_NUM_H15_RdFL(0, cbus_05);
			LINK_NUM_H15_RdFL(0, cbus_09);

			if ( LINK_NUM_H15_Rd_fld(0, cbus_05, reg_man_msc_wrt_stat_pathen_clr) && ( LINK_NUM_H15_Rd_fld(0, cbus_09, reg_state_sink_wrt_stat_pathen_clr) == 1) )
			{
				LINK_NUM_H15_Wr01(0, cbus_05, reg_man_msc_wrt_stat_pathen_clr, 0x0);
				LINK_NUM_H15_WrFL(0, cbus_05);
				HDMI_DEBUG("---- MHL pathen clr : 0x0 \n");
			}
			if ( LINK_NUM_H15_Rd_fld(0, cbus_04, reg_man_msc_wrt_stat_pathen_set) )
			{
				if ( LINK_NUM_H15_Rd_fld(0, cbus_09, reg_state_sink_wrt_stat_pathen_set) == 1)
				{
					LINK_NUM_H15_Wr01(0, cbus_04, reg_man_msc_wrt_stat_pathen_set, 0x0);
					LINK_NUM_H15_WrFL(0, cbus_04);
					HDMI_DEBUG("---- MHL pathen set : 0x0 \n");
				}
				else
				{
					_gPathen_set_count ++;
					if (_gPathen_set_count > 50)
					{
						LINK_NUM_H15_Wr01(0, cbus_04, reg_man_msc_wrt_stat_pathen_set, 0x0);
						LINK_NUM_H15_WrFL(0, cbus_04);
						HDMI_DEBUG("---- MHL pathen set by time out: 0x0 \n");
						_gPathen_set_count = 0;
					}
				}

			}
		}
#endif
#endif
		if (_gH15A0HDMIPhyInform.cbus_warb_done && _gH15A0HDMIPhyInform.cbus_conn_done && (_gH15A0HDMIPhyInform.cbus_st == 3) )
		{
			/*
			   Workaround code for RCP Send Error
			 */
			 _HDMI_H15A0_MHL_Check_Status(1, 0);
			/*
			   Workaround code for HTC beatsaudio phone
			 */
			if (_gH15A0CHG_AVI_count_MHL > 20)
			{
				HDMI_NOTI("---- MHL CHG_AVI_COUNT : [%d], mhl_stable_count : [%d] \n", _gH15A0CHG_AVI_count_MHL, _mhl_stable_count);

				_gH15A0CHG_AVI_count_MHL = -1;

				wake_up_interruptible(&WaitQueue_MHL_H15A0);
			}
			else if ( _gH15A0CHG_AVI_count_MHL != -1)
			{
				_mhl_stable_count ++;
				if (_mhl_stable_count > 3000) // for about 1-minute MHL stable
					_gH15A0CHG_AVI_count_MHL = -1;
			}

		}

#if 0
		LINK_NUM_H15_RdFL(0, cbus_01);
		LINK_NUM_H15_Rd01(0, cbus_01, reg_cbus_disconn, cbus_disconn);			// CBUS DisConnected ???
		//		LINK_NUM_H15_RdFL(0, phy_06);
		//		LINK_NUM_H15_Rd01(0, phy_06, hpd_in_prt3, hpd_in_prt3);			//PHY HPD IN

		//		if (hpd_in_prt3 != _gH15A0HDMIPhyInform.hpd_in[3])
		if (cbus_disconn)
		{
			//	HDMI_DEBUG("---- MHL HPD IN PORT3 : [%d] => [%d]\n",_gH15A0HDMIPhyInform.hpd_in[3], hpd_in_prt3);
			//	_gH15A0HDMIPhyInform.hpd_in[3] = hpd_in_prt3;

			//		if (_gH15A0HDMIPhyInform.hpd_in[3])
			if (1)
			{
				HDMI_DEBUG("---- MHL Clear cbus_disconn_en\n");
				LINK_NUM_H15_RdFL(0, cbus_00);
				LINK_NUM_H15_Wr01(0, cbus_00, reg_cbus_disconn_en, 0x0);
				LINK_NUM_H15_WrFL(0, cbus_00);
				OS_MsecSleep(10);	// ms delay
				LINK_NUM_H15_RdFL(0, cbus_00);
				LINK_NUM_H15_Wr01(0, cbus_00, reg_cbus_disconn_en, 0x1);
				LINK_NUM_H15_WrFL(0, cbus_00);
			}
		}
#endif
	}
#endif
	return RET_OK;
}

int HDMI_H15A0_Get_Aksv_Data(int port_num, UINT8 *pAksv_Data)
{
	if (pAksv_Data == NULL)
		return RET_ERROR;

	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(port_num, hdcp_02);
	LINK_NUM_H15_Rd01(port_num, hdcp_02, reg_hdcp_aksv_0, pAksv_Data[0]);
	LINK_NUM_H15_RdFL(port_num, hdcp_03);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_1, pAksv_Data[1]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_2, pAksv_Data[2]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_3, pAksv_Data[3]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_4, pAksv_Data[4]);

	return RET_OK;
}

/**
 * @brief Check if AKSV Data Received
 *
 * @param port_num
 *
 * @return 0: AKSV is Zero (Not Received), 1: AKSV is Received
 */
static int _HDMI_H15A0_Check_Aksv_Exist(int port_num)
{
	UINT8 Aksv_Data[5];

//	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(port_num, hdcp_02);
	LINK_NUM_H15_Rd01(port_num, hdcp_02, reg_hdcp_aksv_0, Aksv_Data[0]);
	LINK_NUM_H15_RdFL(port_num, hdcp_03);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_1, Aksv_Data[1]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_2, Aksv_Data[2]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_3, Aksv_Data[3]);
	LINK_NUM_H15_Rd01(port_num, hdcp_03, reg_hdcp_aksv_4, Aksv_Data[4]);

	if (memcmp(Aksv_Data, HDCP_AKSV_Data_Zero, (size_t)sizeof(HDCP_AKSV_Data_Zero) ) )
	{
		return 1;
	}
	else
	{
		return 0;	// equal means AKSV not received
	}
}

#if 0
static int _HDMI_H15A0_Get_AN_Data(int port_num, UINT8 *AN_Data)
{
	if (AN_Data == NULL)
		return RET_ERROR;

	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(port_num, hdcp_00);
	LINK_NUM_H15_Rd01(port_num, hdcp_00, reg_hdcp_an_0, AN_Data[0]);
	LINK_NUM_H15_RdFL(port_num, hdcp_01);
	LINK_NUM_H15_Rd01(port_num, hdcp_01, reg_hdcp_an_4, AN_Data[4]);
	LINK_NUM_H15_Rd01(port_num, hdcp_01, reg_hdcp_an_3, AN_Data[3]);
	LINK_NUM_H15_Rd01(port_num, hdcp_01, reg_hdcp_an_2, AN_Data[2]);
	LINK_NUM_H15_Rd01(port_num, hdcp_01, reg_hdcp_an_1, AN_Data[1]);
	LINK_NUM_H15_RdFL(port_num, hdcp_02);
	LINK_NUM_H15_Rd01(port_num, hdcp_02, reg_hdcp_an_7, AN_Data[7]);
	LINK_NUM_H15_Rd01(port_num, hdcp_02, reg_hdcp_an_6, AN_Data[6]);
	LINK_NUM_H15_Rd01(port_num, hdcp_02, reg_hdcp_an_5, AN_Data[5]);

	return RET_OK;
}
#endif

/**
 * @brief Phy Enable/Disable Selected port
 *
 * @param prt_num : port number
 * @param enable : 1:Enable, 0:Disable port
 *
 * @return
 */
static int _HDMI_H15A0_EnablePort(int prt_num, int enable)
{
	/*
	if (enable)
		HDMI_DEBUG("%s : port [%d] : Enable[%d]\n",__func__ ,prt_num, enable);
		*/
//	PRT_NUM_CONVERT(prt_num);

	if(PRT_NUM_INVALID(prt_num))
		return RET_ERROR;

	LINK_NUM_H15_RdFL(prt_num, phy_00);
	LINK_NUM_H15_Wr01(prt_num, phy_00, phy_i2c_enable, enable);		//PHY Enable
	LINK_NUM_H15_WrFL(prt_num, phy_00);

	LINK_NUM_H15_RdFL(prt_num, phy_00);
	LINK_NUM_H15_Rd01(prt_num, phy_00, phy_i2c_enable, _gH15A0HDMIPhyInform.phy_enable[prt_num]);		//PHY Enable

	return RET_OK;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_GetTMDSClock(int prt_selected, UINT32 *pTmds_clock)
{
	return RET_OK;
}
#endif	//#ifdef NOT_USED_NOW

int HDMI_H15A0_MHL_Send_RCP(UINT8 rcp_data)
{
	int buffer_size;
	int buffer_full_release_code = 0;
	int buffer_full_press_code = 0;
	int stored_rcp_code = 0;
	ULONG	flags = 0;

	HDMI_DEBUG("[%s] Entered with [0x%x] \n",__func__, rcp_data);

	spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);

	if ( (_g_rcp_send_buffer == 0) && (rcp_data & 0x80) )
	{
		buffer_full_release_code = 1;
	}
	else
	{
		if(_g_rcp_send_buffer)
			_g_rcp_send_buffer --;
		else
			buffer_full_press_code = 1;

		LINK_NUM_H15_RdFL(1, cbus_10);
		LINK_NUM_H15_Rd01(1, cbus_10, reg_man_msg_rcp_kcode, stored_rcp_code);
		LINK_NUM_H15_Wr01(1, cbus_10, reg_man_msg_rcp_kcode, rcp_data);
		LINK_NUM_H15_WrFL(1, cbus_10);

		LINK_NUM_H15_RdFL(1, cbus_11);
		LINK_NUM_H15_Wr01(1, cbus_11, reg_man_msg_rcp_cmd, 0);
		LINK_NUM_H15_WrFL(1, cbus_11);

		LINK_NUM_H15_RdFL(1, cbus_11);
		LINK_NUM_H15_Wr01(1, cbus_11, reg_man_msg_rcp_cmd, 1);	// Edge Trigger write
		LINK_NUM_H15_WrFL(1, cbus_11);
	}

	buffer_size = _g_rcp_send_buffer;

	spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);

	if(buffer_full_release_code)
		HDMI_DEBUG("!!!!! RCP : Release code NOT Sent [buffer_full]");
	else if(buffer_full_press_code)
		HDMI_DEBUG("!!!!! RCP : OVERWRITE RCP code [0x%x]=>[0x%x]\n", stored_rcp_code, rcp_data);
	else
		HDMI_DEBUG("~~~~~ RCP : Buffer Size [%d]\n", buffer_size);

	return RET_OK;
}

int HDMI_H15A0_MHL_Send_WriteBurst(LX_HDMI_MHL_WRITEBURST_DATA_T *spWriteburst_data)
{
	HDMI_DEBUG("[%s] Entered \n",__func__);

	return RET_OK;
}

int HDMI_H15A0_MHL_Read_WriteBurst(LX_HDMI_MHL_WRITEBURST_DATA_T *spWriteburst_data)
{
	HDMI_DEBUG("[%s] Entered \n",__func__);

	return RET_OK;
}

int HDMI_H15A0_Module_Call_Type(LX_HDMI_CALL_TYPE_T	hdmi_call_type)
{
	HDMI_DEBUG("[%s] Entered with [0x%x] \n",__func__, hdmi_call_type);

	OS_LockMutex(&g_HDMI_Sema);

	switch(hdmi_call_type)
	{
		case HDMI_CALL_TYPE_INIT :
			_gH15A0HDMIPhyInform.module_init = 1;
			break;
		case HDMI_CALL_TYPE_UNINIT :
			_gH15A0HDMIPhyInform.module_init = 0;
			break;
		case HDMI_CALL_TYPE_OPEN :
			_gH15A0HDMIPhyInform.module_open = 1;


			/* If hpd enable is not called until module open */
			/* Following code should be removed when HPD Enable HAL is called properly */
	//		_gH15A0HDMIPhyInform.hpd_enable = 1;

			break;
		case HDMI_CALL_TYPE_CLOSE :
			_gH15A0HDMIPhyInform.module_open = 0;

			/* For Fast Port Switching, Do Nothing when module closed */
/*
			_HDMI_H15A0_PhyOff(0);
			_HDMI_H15A0_PhyOff(1);

			_HDMI_H15A0_Set_HPD_Out_A4P(0,0,0,0);
			*/

			break;
		case HDMI_CALL_TYPE_CONN :
			_gH15A0HDMIPhyInform.module_conn = 1;
			break;
		case HDMI_CALL_TYPE_DISCONN :
			_gH15A0HDMIPhyInform.module_conn = 0;
			break;
		default :
			return RET_ERROR;
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return RET_OK;
}

/**
* @brief Disable internal EDID, in case of using external DDC EEPROM.
*
* @param port_num
* @param edid_resetn
*
* @return
*/

int HDMI_H15A0_Reset_Internal_Edid(int port_num, int edid_resetn)
{
	int ret = RET_OK;

	HDMI_NOTI("[%s] Entered with port[%d] resetn[%d]\n",__func__, port_num, edid_resetn);

	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	OS_LockMutex(&g_HDMI_Sema);

	if ( (_gH15A0HDMIPhyInform.hpd_pol[port_num] != edid_resetn) && ( GPIO15_value ==LX_GPIO_VALUE_LOW) )
		HDMI_NOTI("HPD Polarity of port[%d] Changed [%d]=>[%d] \n", port_num, _gH15A0HDMIPhyInform.hpd_pol[port_num], edid_resetn);

	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		switch(port_num)
		{
			case 0:
				LINK_TOP_H15_RdFL(link_top_01);
				LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt0, edid_resetn);
				LINK_TOP_H15_WrFL(link_top_01);
#if 0
				if ( GPIO15_value ==LX_GPIO_VALUE_LOW)
				{
					_gH15A0HDMIPhyInform.hpd_pol[0] = 0;
					_HDMI_H15A0_Set_HPD_Out(0, !_gH15A0HDMIPhyInform.hpd_pol[0]);	// disable HPD
				}
#endif
				break;
			case 1:
				LINK_TOP_H15_RdFL(link_top_01);
				LINK_TOP_H15_Wr01(link_top_01, link_sw_rstn_edid_prt1, edid_resetn);
				LINK_TOP_H15_WrFL(link_top_01);
#if 0
				if ( GPIO15_value ==LX_GPIO_VALUE_LOW)
				{
					_gH15A0HDMIPhyInform.hpd_pol[1] = 0;
					_HDMI_H15A0_Set_HPD_Out(1, !_gH15A0HDMIPhyInform.hpd_pol[1]);	// disable HPD
				}
#endif
				break;
			default :
				ret = RET_ERROR;
				break;
		}
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return ret;
}

/**
* @brief CTOP Register Setting for External DDC EDID Write
*
* @param port_num
* @param enable
*
* @return
*/
int HDMI_H15A0_Enable_External_DDC_Access(int port_num, int enable)
{
	int ret = RET_OK;
//	int ctop_tmp;

	PRT_NUM_CONVERT(port_num);

	if(PRT_NUM_INVALID(port_num))
		return RET_ERROR;

	HDMI_PRINT("[%s] Entered with port[%d] enable[%d]\n",__func__, port_num, enable);

	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		if (enable)
		{
			if(port_num == 0)
			{
				CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_5_mux_en, 0x1);	// hdmi ddc clk ch 0 disable
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_4_mux_en, 0x1);	// hdmi ddc dat ch 0 disable
				CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
			}
			else if(port_num == 1)
			{
				CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_3_mux_en, 0x1);	// hdmi ddc clk ch 0 disable
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_2_mux_en, 0x1);	// hdmi ddc dat ch 0 disable
				CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
			}
			else
				ret = RET_ERROR;
		}
		else
		{
			if(port_num == 0)
			{
				CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_5_mux_en, 0x0);	// hdmi ddc clk ch 0 enable
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_4_mux_en, 0x0);	// hdmi ddc dat ch 0 enable
				CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
			}
			else if(port_num == 1)
			{
				CTOP_CTRL_H15A0_RdFL(CVI, ctr34);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_3_mux_en, 0x0);	// hdmi ddc clk ch 0 enable
				CTOP_CTRL_H15A0_Wr01(CVI, ctr34, gp6_2_mux_en, 0x0);	// hdmi ddc dat ch 0 enable
				CTOP_CTRL_H15A0_WrFL(CVI, ctr34);
			}
			else
				ret = RET_ERROR;
		}
	}
	/*
	CTOP_CTRL_H15A0_RdFL(RIGHT, ctr130);
	CTOP_CTRL_H15A0_Wr01(RIGHT, ctr130, ddc_write_enb, enable);
	CTOP_CTRL_H15A0_WrFL(RIGHT, ctr130);

	if (enable)
	{
		if ( ( port_num >= 0 ) && ( port_num < 4 ) )
		{
			CTOP_CTRL_H15A0_RdFL(RIGHT, ctr130);
			ctop_tmp = CTOP_CTRL_H15A0_Rd(RIGHT, ctr130);
			ctop_tmp = ( ctop_tmp & 0xCFFFFFFF ) | ( port_num << 28);
			CTOP_CTRL_H15A0_Wr(RIGHT, ctr130, ctop_tmp);
			CTOP_CTRL_H15A0_WrFL(RIGHT, ctr130);
		}
		else
			ret = RET_ERROR;
	}
	*/

	return ret;
}

/**
* @brief get MHL/HDMI auto/manual mode
*
* @return man_mhl_mode
*/
static int _HDMI_H15A0_Get_ManMHLMode(void)
{
	int ret;

	LINK_NUM_H15_RdFL(1, phy_00);
	LINK_NUM_H15_Rd01(1, phy_00, reg_man_mhl_mode, ret);

	return ret;
}
/**
* @brief set MHL/HDMI mode by manual
*
* @param man_mhl_mode : 0:auto, 1:Manual
* @param man_mhl_value : 0:HDMI, 1:MHL
*
* @return
*/
static int _HDMI_H15A0_Set_ManMHLMode(int link_num, int man_mhl_mode, int man_mhl_value)
{
	int ret = RET_OK;

//	PRT_NUM_CONVERT(link_num);

	if ( PRT_NUM_INVALID(link_num) )
		return RET_ERROR;

	LINK_NUM_H15_RdFL(link_num, phy_00);
	LINK_NUM_H15_Wr01(link_num, phy_00, reg_man_mhl_mode, man_mhl_mode);
	LINK_NUM_H15_Wr01(link_num, phy_00, reg_man_mhl_value, man_mhl_value);
	LINK_NUM_H15_WrFL(link_num, phy_00);

	return ret;
}

static int _HDMI_H15A0_MHL_Reconnect_Thread(void *data)
{
	int cbus_st_count, state_sink_clr_hpd, state_sink_set_hpd;

	HDMI_DEBUG("---- MHL Reconnect Thread ---- \n");

	while(1)
	{
		_MHL_Reconnect_Thread_wakeup = 0;

		//interruptible_sleep_on(&WaitQueue_MHL_H15A0);
		wait_event_interruptible(WaitQueue_MHL_H15A0 , _MHL_Reconnect_Thread_wakeup !=0 );

		HDMI_NOTI("---- !!!!!!!!!!!! MHL TMDS Unstable !!!!!!!!!!!!!!!!!!!! \n");

		LINK_NUM_H15_RdFL(1, cbus_04);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_clr_hpd, 0x1);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_set_hpd, 0x0);
		LINK_NUM_H15_WrFL(1, cbus_04);

		for (cbus_st_count = 0; cbus_st_count < 10; cbus_st_count++)
		{
			OS_MsecSleep(3000);	// ms delay

			LINK_NUM_H15_RdFL(1, cbus_06);
			LINK_NUM_H15_Rd01(1, cbus_06, reg_state_sink_clr_hpd, state_sink_clr_hpd);

			if (state_sink_clr_hpd > 0)
				break;
		}
		HDMI_NOTI("---- MHL CBUS_ST : state_sink_clr_hpd [%d] \n", state_sink_clr_hpd);

		LINK_NUM_H15_RdFL(1, cbus_04);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_clr_hpd, 0x0);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_set_hpd, 0x1);
		LINK_NUM_H15_WrFL(1, cbus_04);

		for (cbus_st_count = 0; cbus_st_count < 10; cbus_st_count++)
		{
			OS_MsecSleep(10);	// ms delay

			LINK_NUM_H15_RdFL(1, cbus_05);
			LINK_NUM_H15_Rd01(1, cbus_05, reg_state_sink_set_hpd, state_sink_set_hpd);

			if (state_sink_set_hpd > 0)
				break;
		}
		HDMI_NOTI("---- MHL CBUS_ST : state_sink_set_hpd [%d] \n", state_sink_set_hpd);

		LINK_NUM_H15_RdFL(1, cbus_04);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_clr_hpd, 0x0);
		LINK_NUM_H15_Wr01(1, cbus_04, reg_man_msc_set_hpd, 0x0);
		LINK_NUM_H15_WrFL(1, cbus_04);


	}

	return RET_OK;
}

static int _HDMI_H15A0_MHL_PDB_Thread(void *data)
{

	int phy_en_prt0, phy_en_prt1;
	int phy_pdb_prt1, phy_rstn_prt1;

	HDMI_DEBUG("---- MHL PDB Thread ---- \n");

	while(1)
	{
		_MHL_PDB_Thread_wakeup = 0;

		//interruptible_sleep_on(&WaitQueue_MHL_PDB_H15A0);
		wait_event_interruptible(WaitQueue_MHL_PDB_H15A0 , _MHL_PDB_Thread_wakeup !=0 );

		LINK_NUM_H15_RdFL(1, phy_00);
		LINK_NUM_H15_Rd01(1, phy_00, phy_pdb, phy_pdb_prt1);			//PHY PDB ON
		LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_rstn, phy_rstn_prt1);			//PHY RESET

		if (phy_pdb_prt1 && phy_rstn_prt1)
		{

			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

			if (!phy_en_prt1)	// IF port3(C-BUS) is not enabled?
			{
				LINK_NUM_H15_RdFL(0, phy_00);
				LINK_NUM_H15_Rd01(0, phy_00, phy_i2c_enable, phy_en_prt0);		//PHY Enable

				LINK_NUM_H15_RdFL(1, phy_00);
				LINK_NUM_H15_Rd01(1, phy_00, phy_i2c_enable, phy_en_prt1);		//PHY Enable

				_HDMI_H15A0_EnablePort(0, 0);
				_HDMI_H15A0_EnablePort(1, 1);
			}

			/*
			   HDMI_DEBUG("---- MHL warb done:0 ,Set ODT PDB Mode to 0x10\n");
			   PHY_REG_H15A0_RdFL(eq_i2c_odt_pdb_mode);
			   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb,0x0);
			   PHY_REG_H15A0_Wr01(eq_i2c_odt_pdb_mode,eq_i2c_odt_pdb_mode,0x1);
			   PHY_REG_H15A0_WrFL(eq_i2c_odt_pdb_mode);
			 */
			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(eq_i2c_pdb_d0);
				PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_i2c_pdb_d0_mode,0x1);
				PHY_REG_H15B0_Wr01(eq_i2c_pdb_d0,eq_man_pdb_d0,0x0);
				PHY_REG_H15B0_WrFL(eq_i2c_pdb_d0);

				PHY_REG_H15B0_RdFL(eq_i2c_pdb_dck);
				PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_i2c_pdb_dck_mode,0x1);
				PHY_REG_H15B0_Wr01(eq_i2c_pdb_dck,eq_man_pdb_dck,0x0);
				PHY_REG_H15B0_WrFL(eq_i2c_pdb_dck);
			}
			else {

				PHY_REG_H15A0_RdFL(pdb_d0_man_sel);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_d0_man_sel,pdb_d0_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_d0_man_sel);

				PHY_REG_H15A0_RdFL(pdb_dck_man_sel);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man_sel,0x1);
				PHY_REG_H15A0_Wr01(pdb_dck_man_sel,pdb_dck_man,0x0);
				PHY_REG_H15A0_WrFL(pdb_dck_man_sel);
			}

			if (!phy_en_prt1)
			{
				_HDMI_H15A0_EnablePort(0, phy_en_prt0);
				_HDMI_H15A0_EnablePort(1, phy_en_prt1);
			}
		}

		HDMI_DEBUG("---- MHL disconn, PDB_D0_MAN_SEL :0x10, PDB_DCK_MAN_SEL :0x10\n");
	}

	return RET_OK;
}

static int _HDMI_H15A0_Disable_TMDS_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_ecc_err_enable, 0x0);				///< 17 intr_ecc_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

static int _HDMI_H15A0_Enable_TMDS_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_ecc_err_enable, 0x1);				///< 17 intr_ecc_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

static int _HDMI_H15A0_Disable_TERC4_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_terc4_err_enable, 0x0);				///< 17 intr_ecc_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

static int _HDMI_H15A0_Enable_TERC4_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_terc4_err_enable, 0x1);				///< 17 intr_ecc_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

static int _HDMI_H15A0_Disable_HDCP_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdcp_err_enable, 0x0);				///< 17 intr_ecc_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

static int _HDMI_H15A0_Enable_HDCP_Error_Interrupt(int link_num)
{
#ifdef H15_LINK_INTERRUPT_ENABLE_DEBUG
	LINK_NUM_H15_RdFL(link_num, interrupt_enable_01);
#endif
	LINK_NUM_H15_Wr01(link_num, interrupt_enable_01, intr_hdcp_err_enable, 0x1);				///< 16 intr_hdcp_err_enable
	LINK_NUM_H15_WrFL(link_num, interrupt_enable_01);

	return RET_OK;
}

int HDMI_H15A0_MHL_Receive_RCP(LX_HDMI_RCP_RECEIVE_T *sp_MHL_RCP_rcv_msg)
{
	if (sp_MHL_RCP_rcv_msg->rcp_receive_cmd == HDMI_RCP_RECEIVE_CMD_CHECK_RCP)
	{
		sp_MHL_RCP_rcv_msg->rcp_receive_flag = _gMHL_RCP_RCV_MSG.rcp_receive_flag;
	}
	else if (sp_MHL_RCP_rcv_msg->rcp_receive_cmd == HDMI_RCP_RECEIVE_CMD_CLEAR)
	{
		if (_gMHL_RCP_RCV_MSG.rcp_receive_flag)
		{
			_gMHL_RCP_RCV_MSG.rcp_receive_cmd = HDMI_RCP_RECEIVE_CMD_NONE;
			_gMHL_RCP_RCV_MSG.rcp_receive_flag = FALSE;
			_gMHL_RCP_RCV_MSG.rcp_msg = 0x7F;
		}
		else
		{
			HDMI_WARN("[%s] No RCP msg to clear !!! \n",__func__);
			_gMHL_RCP_RCV_MSG.rcp_receive_cmd = HDMI_RCP_RECEIVE_CMD_NONE;
			_gMHL_RCP_RCV_MSG.rcp_msg = 0x7F;
		}
	}
	else if (sp_MHL_RCP_rcv_msg->rcp_receive_cmd == HDMI_RCP_RECEIVE_CMD_GET_MSG)
	{
		if (_gMHL_RCP_RCV_MSG.rcp_receive_flag)
		{
			sp_MHL_RCP_rcv_msg->rcp_msg = _gMHL_RCP_RCV_MSG.rcp_msg;
			HDMI_DEBUG("[%s] received msg [0x%x] \n",__func__, sp_MHL_RCP_rcv_msg->rcp_msg);

			/* RCP Reg Auto Clear after read */
			_gMHL_RCP_RCV_MSG.rcp_receive_cmd = HDMI_RCP_RECEIVE_CMD_NONE;
			_gMHL_RCP_RCV_MSG.rcp_receive_flag = FALSE;
			_gMHL_RCP_RCV_MSG.rcp_msg = 0x7F;
		}
		else
		{
			sp_MHL_RCP_rcv_msg->rcp_msg = _gMHL_RCP_RCV_MSG.rcp_msg;
			HDMI_WARN("[%s] No New RCP msg received !!! :return  [0x%x] \n",__func__, sp_MHL_RCP_rcv_msg->rcp_msg);
		}

	}
	else
	{
		HDMI_WARN("[%s] Not a valid cmd !!! [0x%x] \n",__func__, sp_MHL_RCP_rcv_msg->rcp_receive_cmd);
		return RET_ERROR;
	}

	return RET_OK;
}

#ifdef NOT_USED_NOW
/**
* @brief set HDCP Unauth Nosync/Mode Change
*
* @param unauth_nosync
* @param unauth_mode_chg
*
* @return
*/
static int _HDMI_H15A0_Set_HDCP_Unauth(int port_num, int unauth_nosync, int unauth_mode_chg)
{
	int ret = RET_OK;

	switch(port_num)
	{
		case 0:

			LINK_NUM_H15_RdFL(0, hdcp_00);
			LINK_NUM_H15_Wr01(0, hdcp_00, reg_hdcp_unauth_nosync_prt0, unauth_nosync);
			LINK_NUM_H15_Wr01(0, hdcp_00, reg_hdcp_unauth_mode_chg_prt0, unauth_mode_chg);
			LINK_NUM_H15_WrFL(0, hdcp_00);
			break;

		case 1:
			LINK_NUM_H15_RdFL(0, hdcp_06);
			LINK_NUM_H15_Wr01(0, hdcp_06, reg_hdcp_unauth_nosync_prt1, unauth_nosync);
			LINK_NUM_H15_Wr01(0, hdcp_06, reg_hdcp_unauth_mode_chg_prt1, unauth_mode_chg);
			LINK_NUM_H15_WrFL(0, hdcp_06);
			break;

		default :
			ret = RET_ERROR;
			break;
	}
	return ret;
}
#endif	//#ifdef NOT_USED_NOW

static int _HDMI_H15A0_Phy_Reset(int port_num)
{
	int ret = RET_OK;

	switch(port_num)
	{
		case 0:
			LINK_NUM_H15_RdFL(0, phy_00);
			LINK_NUM_H15_Wr01(0, phy_00, phy_pdb, 0x0);			//PHY PDB ON
			LINK_NUM_H15_Wr01(0, phy_00, phy_i2c_rstn, 0x0);			//PHY RESET
			LINK_NUM_H15_WrFL(0, phy_00);
			break;

		case 1:
			LINK_NUM_H15_RdFL(1, phy_00);
			LINK_NUM_H15_Wr01(1, phy_00, phy_pdb, 0x0);			//PHY PDB ON
			LINK_NUM_H15_Wr01(1, phy_00, phy_i2c_rstn, 0x0);			//PHY RESET
			LINK_NUM_H15_WrFL(1, phy_00);
			break;

		default :
			ret = RET_ERROR;
			break;

	}

	return ret;
}

static int _HDMI_H15A0_Check_RAP_Content(int clear)
{
	static int prev_content_on_status = 0;
	int content_on_status;
	int ret = RET_OK;

#if 1
	LINK_NUM_H15_RdFL(0, cbus_00);
	LINK_NUM_H15_Rd01(0, cbus_00, reg_cbus_rap_content_on, content_on_status);
#else	// to test RAP Function : JUST TEST ONLY !!!!!
	LINK_NUM_H15_RdFL(0, hdcp_12);
	LINK_NUM_H15_Rd01(0, hdcp_12, reg_hdcp_unauth_mode_chg_prt2, content_on_status);
#endif

	if (clear)
	{
		prev_content_on_status = 0;
		_gH15A0MHLContentOff = 0;
	}
	else if ( (content_on_status != prev_content_on_status) && (_gH15A0HDMIPhyInform.cbus_st > 1) )
	{
		HDMI_DEBUG("---- MHL RAP Contents On CHG: [%d] => [%d] \n", prev_content_on_status, content_on_status);

		if (content_on_status)	// RAP : content ON
		{
			if (_gH15A0MHLContentOff)	// av mute state
			{
				HDMI_DEBUG("---- MHL RAP Set UnMute AV !!! \n");
				_gH15A0MHLContentOff = 0;
			}
			else	// av unmute state
			{
				HDMI_DEBUG("---- MHL RAP AV already unmuted \n");
			}
		}
		else	// RAP : content Off
		{
			if (_gH15A0MHLContentOff)	// av mute state
			{
				HDMI_DEBUG("---- MHL RAP AV already muted \n");
			}
			else	// av unmute state
			{
				HDMI_DEBUG("---- MHL RAP Set Mute AV \n");
				_gH15A0MHLContentOff = 1;
			}
		}
		prev_content_on_status = content_on_status;
	}

	return ret;
}

static int _HDMI_H15A0_MHL_Check_Status(int link_num, int init)
{
	int state_sink_wrt_stat_pathen_set, seq_st;
	int man_msc_clr_hpd, man_msc_set_hpd;
	static int mhl_status_invalid = 0;

	if (init)
	{
		HDMI_DEBUG("[%s] Entered with [%d] \n",__func__, init);
		LINK_NUM_H15_RdFL(link_num, cbus_04);
		LINK_NUM_H15_Wr(link_num, cbus_04, 0x20003100);	//initial CBUS-04 register value
		LINK_NUM_H15_WrFL(link_num, cbus_04);

		mhl_status_invalid = 0;
	}
	else
	{
		LINK_NUM_H15_RdFL(link_num, cbus_09);
		LINK_NUM_H15_Rd01(link_num, cbus_09, reg_state_sink_wrt_stat_pathen_set, state_sink_wrt_stat_pathen_set);
		LINK_NUM_H15_RdFL(link_num, cbus_05);
		LINK_NUM_H15_Rd01(link_num, cbus_05, reg_seq_st, seq_st);

		if ( (state_sink_wrt_stat_pathen_set == 1) && (seq_st == 0) )
		{
			HDMI_NOTI("MHL Connected BUT invalid status : reg_state_sink_wrt_stat_pathen_set : [0x%d], reg_seq_st : [0x%x] \n", state_sink_wrt_stat_pathen_set, seq_st);

			LINK_NUM_H15_RdFL(link_num, cbus_04);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_wrt_stat_pathen_set, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_wrt_stat_dcaprd_set, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_set_intr_3dreq_set, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_rd_devcap, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_get_vdid, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_get_state, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_wrt_stat_dcaprd_set, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_wrt_stat_pathen_set, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_rd_devcap, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_get_vdid, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_get_state, 0x0);
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_seq_msc_set_hpd, 0x0);

			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_clr_hpd, 0x1);	// 0x40
			LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_set_hpd, 0x0);

			LINK_NUM_H15_WrFL(link_num, cbus_04);

			mhl_status_invalid = 1;
		}
		else if (mhl_status_invalid)
		{
			LINK_NUM_H15_RdFL(link_num, cbus_04);
			LINK_NUM_H15_Rd01(link_num, cbus_04, reg_man_msc_clr_hpd, man_msc_clr_hpd);
			LINK_NUM_H15_Rd01(link_num, cbus_04, reg_man_msc_set_hpd, man_msc_set_hpd);

			if ( (man_msc_clr_hpd == 0x1) && (man_msc_set_hpd == 0) && !_gH15A0HDMIPhyInform.scdt[3])
			{
				HDMI_NOTI("MHL invalid status : SCDT lost\n");
				LINK_NUM_H15_RdFL(link_num, cbus_04);
				LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_clr_hpd, 0x0);	// 0x80
				LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_set_hpd, 0x1);
				LINK_NUM_H15_WrFL(link_num, cbus_04);

				OS_MsecSleep(2);	// ms delay

				LINK_NUM_H15_RdFL(link_num, cbus_04);
				LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_clr_hpd, 0x0);	// 0x00
				LINK_NUM_H15_Wr01(link_num, cbus_04, reg_man_msc_set_hpd, 0x0);
				LINK_NUM_H15_WrFL(link_num, cbus_04);

				mhl_status_invalid = 0;
			}
		}

	}


	return RET_OK;
}

int HDMI_H15A0_Get_Driver_Status(LX_HDMI_DRIVER_STATUS_T *spHDMI_Driver_Status)
{
	if (spHDMI_Driver_Status == NULL)
		return RET_ERROR;

	memcpy(&spHDMI_Driver_Status->PrevTiming[0], &sHDMI_H15A0_Status.PrevTiming[0], (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
	memcpy(&spHDMI_Driver_Status->PrevTiming[1], &sHDMI_H15A0_Status.PrevTiming[1], (size_t)sizeof(LX_HDMI_TIMING_INFO_T));
	spHDMI_Driver_Status->TimingReadCnt[0] = sHDMI_H15A0_Status.TimingReadCnt[0];
	spHDMI_Driver_Status->TimingReadCnt[1] = sHDMI_H15A0_Status.TimingReadCnt[1];
	spHDMI_Driver_Status->HDMIState[0] = sHDMI_H15A0_Status.HDMIState[0];
	spHDMI_Driver_Status->HDMIState[1] = sHDMI_H15A0_Status.HDMIState[1];
	spHDMI_Driver_Status->PrevPixelEncoding[0] = sHDMI_H15A0_Status.PrevPixelEncoding[0];
	spHDMI_Driver_Status->PrevPixelEncoding[1] = sHDMI_H15A0_Status.PrevPixelEncoding[1];
	memcpy(&spHDMI_Driver_Status->PrevAVIPacket[0], &sHDMI_H15A0_Status.PrevAVIPacket[0], (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	memcpy(&spHDMI_Driver_Status->PrevAVIPacket[1], &sHDMI_H15A0_Status.PrevAVIPacket[1], (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	memcpy(&spHDMI_Driver_Status->PrevVSIPacket[0], &sHDMI_H15A0_Status.PrevVSIPacket[0], (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	memcpy(&spHDMI_Driver_Status->PrevVSIPacket[1], &sHDMI_H15A0_Status.PrevVSIPacket[1], (size_t)sizeof(LX_HDMI_INFO_PACKET_T));
	spHDMI_Driver_Status->AVIReadState[0] = sHDMI_H15A0_Status.AVIReadState[0];
	spHDMI_Driver_Status->AVIReadState[1] = sHDMI_H15A0_Status.AVIReadState[1];
	spHDMI_Driver_Status->VSIState[0] = sHDMI_H15A0_Status.VSIState[0];
	spHDMI_Driver_Status->VSIState[1] = sHDMI_H15A0_Status.VSIState[1];
	spHDMI_Driver_Status->AVIChangeState[0] = sHDMI_H15A0_Status.AVIChangeState[0];
	spHDMI_Driver_Status->AVIChangeState[1] = sHDMI_H15A0_Status.AVIChangeState[1];
	spHDMI_Driver_Status->PortSelected[0] = sHDMI_H15A0_Status.PortSelected[0];
	spHDMI_Driver_Status->PortSelected[1] = sHDMI_H15A0_Status.PortSelected[1];
	spHDMI_Driver_Status->MHLContentOff = _gH15A0MHLContentOff;

	spHDMI_Driver_Status->AudioMuteState[0] = _gH15A0AudioMuteState[0];
	spHDMI_Driver_Status->AudioMuteState[1] = _gH15A0AudioMuteState[1];
	spHDMI_Driver_Status->AudioArcStatus[0] = _gH15A0AudioArcStatus[0];
	spHDMI_Driver_Status->AudioArcStatus[1] = _gH15A0AudioArcStatus[1];
	spHDMI_Driver_Status->IntrAudioState[0] = _gH15A0IntrAudioState[0];
	spHDMI_Driver_Status->IntrAudioState[1] = _gH15A0IntrAudioState[1];
	spHDMI_Driver_Status->HdmiPortStableCount[0] = _gH15A0HdmiPortStableCount[0];
	spHDMI_Driver_Status->HdmiPortStableCount[1] = _gH15A0HdmiPortStableCount[1];
	spHDMI_Driver_Status->HdmiFreqErrorCount[0] = _gH15A0HdmiFreqErrorCount[0];
	spHDMI_Driver_Status->HdmiFreqErrorCount[1] = _gH15A0HdmiFreqErrorCount[1];
	spHDMI_Driver_Status->HdmiAudioThreadCount[0] = _gH15A0HdmiAudioThreadCount[0];
	spHDMI_Driver_Status->HdmiAudioThreadCount[1] = _gH15A0HdmiAudioThreadCount[1];
	spHDMI_Driver_Status->IntrBurstInfoCount[0] = _gH15A0IntrBurstInfoCount[0];
	spHDMI_Driver_Status->TaskBurstInfoCount[1] = _gH15A0TaskBurstInfoCount[1];
	memcpy(&spHDMI_Driver_Status->HdmiAudioInfo[0], &_gH15A0HdmiAudioInfo, sizeof(LX_HDMI_AUDIO_INFO_T));
	memcpy(&spHDMI_Driver_Status->HdmiAudioInfo[1], &_gH15A0HdmiAudioInfo, sizeof(LX_HDMI_AUDIO_INFO_T));

	spHDMI_Driver_Status->Intr_vf_stable[0] = sHDMI_H15A0_Status.Intr_vf_stable[0];
	spHDMI_Driver_Status->Intr_vf_stable[1] = sHDMI_H15A0_Status.Intr_vf_stable[1];
	spHDMI_Driver_Status->Intr_avi[0] = sHDMI_H15A0_Status.Intr_avi[0];
	spHDMI_Driver_Status->Intr_avi[1] = sHDMI_H15A0_Status.Intr_avi[1];
	spHDMI_Driver_Status->Intr_src_mute[0] = sHDMI_H15A0_Status.Intr_src_mute[0];
	spHDMI_Driver_Status->Intr_src_mute[1] = sHDMI_H15A0_Status.Intr_src_mute[1];
	spHDMI_Driver_Status->Intr_vsi[0] = sHDMI_H15A0_Status.Intr_vsi[0];
	spHDMI_Driver_Status->Intr_vsi[1] = sHDMI_H15A0_Status.Intr_vsi[1];

	spHDMI_Driver_Status->Force_thread_sleep = _gH15A0Force_thread_sleep;
	spHDMI_Driver_Status->HDMI_thread_running = _gH15A0HDMI_thread_running;
	spHDMI_Driver_Status->HDMI_interrupt_registered = _gH15A0HDMI_interrupt_registered;

	spHDMI_Driver_Status->ChipPlatform = _gH15A0ChipPlatform;

	spHDMI_Driver_Status->CHG_AVI_count_MHL[0] = _gH15A0CHG_AVI_count_MHL;
	spHDMI_Driver_Status->CHG_AVI_count_EQ[0] = _gH15A0CHG_AVI_count_EQ;
	spHDMI_Driver_Status->TMDS_ERROR_EQ[0] = _gH15A0_TMDS_ERROR_EQ[0];
	spHDMI_Driver_Status->TMDS_ERROR_EQ[1] = _gH15A0_TMDS_ERROR_EQ[1];

	spHDMI_Driver_Status->HDMIConnectState[0] = sHDMI_H15A0_Status.HDMIConnectState[0];
	spHDMI_Driver_Status->HDMIConnectState[1] = sHDMI_H15A0_Status.HDMIConnectState[1];

	spHDMI_Driver_Status->device_suspend_count = sHDMI_H15A0_Status.device_suspend_count;
	spHDMI_Driver_Status->device_resumed_count = sHDMI_H15A0_Status.device_resumed_count;
	spHDMI_Driver_Status->device_in_suspend_mode = sHDMI_H15A0_Status.device_in_suspend_mode;
	spHDMI_Driver_Status->Driver_Revision = sHDMI_H15A0_Status.Driver_Revision;

	return RET_OK;
}

static int _HDMI_H15A0_HDCP_Check_Status(void)
{
	static int hdcp_unauthed_count[2] = {0,};
	int port_num;

	for (port_num = 0; port_num <2; port_num++)
	{
		if ( (_gH15A0HDMIPhyInform.hdcp_authed[port_num] == 0) \
				&& (_HDMI_H15A0_Check_Aksv_Exist(port_num) )
				&& (_gH15A0HDMIPhyInform.scdt[port_num] ) )
		{
			hdcp_unauthed_count[port_num]++;

			if (hdcp_unauthed_count[port_num] > 200)	//for about 4-sec
			{
#if 0	// when MHL connected and ASKV received ,but HDCP not authed...
				if( (port_num == 3) &&  _gH15A0HDMIPhyInform.cd_sense && (_gH15A0HDMIPhyInform.cbus_st == 3) && _gH15A0HDMIPhyInform.cbus_conn_done)
				{
					int cbus_st_count, state_sink_clr_hpd, state_sink_set_hpd;

					HDMI_NOTI("Port [%d] MHL HDCP unauthed for long time : hpd clear/set !!!!! \n", port_num);

					LINK_M14_RdFL(cbus_04);
					LINK_M14_Wr01(cbus_04, reg_man_msc_clr_hpd, 0x1);
					LINK_M14_Wr01(cbus_04, reg_man_msc_set_hpd, 0x0);
					LINK_M14_WrFL(cbus_04);

					for (cbus_st_count = 0; cbus_st_count < 10; cbus_st_count++)
					{
						OS_MsecSleep(3000);	// ms delay

						LINK_M14_RdFL(cbus_06);
						LINK_M14_Rd01(cbus_06, reg_state_sink_clr_hpd, state_sink_clr_hpd);

						if (state_sink_clr_hpd > 0)
							break;
					}
					HDMI_NOTI("---- MHL CBUS_ST : state_sink_clr_hpd [%d] \n", state_sink_clr_hpd);

					LINK_M14_RdFL(cbus_04);
					LINK_M14_Wr01(cbus_04, reg_man_msc_clr_hpd, 0x0);
					LINK_M14_Wr01(cbus_04, reg_man_msc_set_hpd, 0x1);
					LINK_M14_WrFL(cbus_04);

					for (cbus_st_count = 0; cbus_st_count < 10; cbus_st_count++)
					{
						OS_MsecSleep(10);	// ms delay

						LINK_M14_RdFL(cbus_05);
						LINK_M14_Rd01(cbus_05, reg_state_sink_set_hpd, state_sink_set_hpd);

						if (state_sink_set_hpd > 0)
							break;
					}
					HDMI_NOTI("---- MHL CBUS_ST : state_sink_set_hpd [%d] \n", state_sink_set_hpd);

					LINK_M14_RdFL(cbus_04);
					LINK_M14_Wr01(cbus_04, reg_man_msc_clr_hpd, 0x0);
					LINK_M14_Wr01(cbus_04, reg_man_msc_set_hpd, 0x0);
					LINK_M14_WrFL(cbus_04);
				}
#endif

				HDMI_NOTI("Port [%d] unauthed for long time : reset HDCP !!!!! \n", port_num);

				_HDMI_H15A0_HDCP_ResetPort(port_num);

				hdcp_unauthed_count[port_num] = 0;
			}
		}
		else
			hdcp_unauthed_count[port_num] = 0;

	}
	if (gHDMI_Phy_Control.hdcp_reset_hdcp_mode_workaround == 1)
	{
		for (port_num=0;port_num<2;port_num++)
		{
			_HDMI_H15A0_Check_HDCP_Abnormal(port_num);
		}
	}

	for (port_num=0;port_num<2;port_num++)
	{
		_HDMI_H15A0_Check_HDCP_Mode(port_num);
	}

	return RET_OK;
}

#ifdef NOT_USED_NOW
static int _HDMI_H15A0_swrst_TMDS_sel_control(int reset)
{
	int ret = RET_OK;
	HDMI_DEBUG("[%s] Entered with [%d]\n",__func__, reset);
#ifdef H15_BRINGUP_TODO
	CTOP_CTRL_H15A0_RdFL(DEI, ctr02);
	CTOP_CTRL_H15A0_Wr01(DEI, ctr02, swrst_tmds_sel, reset);
	CTOP_CTRL_H15A0_WrFL(DEI, ctr02);
#endif
	return ret;
}
#endif

int HDMI_H15A0_Key_Control(LX_HDMI_CONTROL_KEY_TYPE_T key_control_cmd)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered [%d]\n",__func__, key_control_cmd);

	OS_LockMutex(&g_HDMI_Sema);

	switch(key_control_cmd)
	{
		case HDMI_KEY_SAVE:
			if (pHDCP_Key_addr == NULL)
			{
				HDMI_ERROR( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );

				OS_UnlockMutex(&g_HDMI_Sema);

				return RET_ERROR;
			}
			memcpy((void *)hdmi_data_save, (void *)pHDCP_Key_addr, 288);
			break;

		case HDMI_KEY_CLEAR:
			if (pHDCP_Key_addr == NULL)
			{
				HDMI_ERROR( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );

				OS_UnlockMutex(&g_HDMI_Sema);

				return RET_ERROR;
			}
			memset((void *)pHDCP_Key_addr, 0, 288);
			break;

		case HDMI_KEY_RESTORE:
			if (pHDCP_Key_addr == NULL)
			{
				HDMI_ERROR( "[%s][%d] pHDCP_Key_addr is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );

				OS_UnlockMutex(&g_HDMI_Sema);

				return RET_ERROR;
			}
			memcpy((void *)pHDCP_Key_addr, (void *)hdmi_data_save, 288);

			_HDMI_H15A0_HDCP_ResetPort_Control(0,0);
			_HDMI_H15A0_HDCP_ResetPort_Control(1,0);

			OS_MsecSleep(5);	// ms delay

			_HDMI_H15A0_HDCP_ResetPort_Control(0,1);
			_HDMI_H15A0_HDCP_ResetPort_Control(1,1);
			break;

		case HDMI_KEY_NONE:
		default :
			ret = RET_ERROR;
			break;
	}

	OS_UnlockMutex(&g_HDMI_Sema);

	return ret;
}

static int _HDMI_H15A0_EDID_Control(LX_HDMI_CONTROL_EDID_TYPE_T edid_control_cmd)
{
	int ret = RET_OK;

	HDMI_DEBUG("[%s] Entered [%d]\n",__func__, edid_control_cmd);

	switch(edid_control_cmd)
	{
		case HDMI_EDID_SAVE:
			if (pEDID_data_addr_port0 == NULL)
			{
				HDMI_ERROR( "[%s][%d] pEDID_data_addr_port0 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
				return RET_ERROR;
			}
			if (pEDID_data_addr_port1 == NULL)
			{
				HDMI_ERROR( "[%s][%d] pEDID_data_addr_port1 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
				return RET_ERROR;
			}

			memcpy((void *)edid_data_rd_port0, (void *)pEDID_data_addr_port0, 256);
			memcpy((void *)edid_data_rd_port1, (void *)pEDID_data_addr_port1, 256);

			break;

		case HDMI_EDID_RESTORE:
			if (pEDID_data_addr_port0 == NULL)
			{
				HDMI_ERROR( "[%s][%d] pEDID_data_addr_port0 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
				return RET_ERROR;
			}
			if (pEDID_data_addr_port1 == NULL)
			{
				HDMI_ERROR( "[%s][%d] pEDID_data_addr_port1 is NULL !!!!!!!!!!!!!!!!!!!!\n", __func__, __LINE__ );
				return RET_ERROR;
			}

			memcpy((void *)pEDID_data_addr_port0, (void *)edid_data_rd_port0, 256);
			memcpy((void *)pEDID_data_addr_port1, (void *)edid_data_rd_port1, 256);

			break;

		case HDMI_EDID_CLEAR:
		case HDMI_EDID_NONE:
		default :
			ret = RET_ERROR;
			break;
	}

	return ret;
}

UINT32 _HDMI_H15_Get_Clock_Freq(void)
{
	UINT32 up_freq = 0,	down_freq = 0;
	UINT32 tmdsClock;

	if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
		PHY_REG_H15B0_RdFL(tmds_freq_1);
		PHY_REG_H15B0_RdFL(tmds_freq_2);

		PHY_REG_H15B0_Rd01(tmds_freq_1,tmds_freq,up_freq);
		PHY_REG_H15B0_Rd01(tmds_freq_2,tmds_freq,down_freq);
	}
	else {
		PHY_REG_H15A0_RdFL(tmds_freq_1);
		PHY_REG_H15A0_RdFL(tmds_freq_2);

		PHY_REG_H15A0_Rd01(tmds_freq_1,tmds_freq,up_freq);
		PHY_REG_H15A0_Rd01(tmds_freq_2,tmds_freq,down_freq);
	}
	tmdsClock = ((up_freq << 8) + down_freq); 	// XXX.XX KHz

	return tmdsClock;
}

int HDMI_H15A0_HPD_HDCP22_Control(LX_HDMI_HPD_HDCP22_CONTROL_T hpd_hdcp22_control)
{
	int ret = RET_OK;

	PRT_NUM_CONVERT(hpd_hdcp22_control.prt_num);

	if(PRT_NUM_INVALID(hpd_hdcp22_control.prt_num))
		return RET_ERROR;

	HDMI_DEBUG("[%s] Entered port[%d] control[%d]\n",__func__, hpd_hdcp22_control.prt_num, hpd_hdcp22_control.hpd_hdcp22_control);

	HDMI_NOTI("HPD HDCP22 Control port[%d] : HPD[%d] => [%d]\n",\
			hpd_hdcp22_control.prt_num, _gH15A0HDMIPhyInform.tcs_done[hpd_hdcp22_control.prt_num], hpd_hdcp22_control.hpd_hdcp22_control);
	_gH15A0HDMIPhyInform.hpd_hdcp22_control[hpd_hdcp22_control.prt_num] = hpd_hdcp22_control.hpd_hdcp22_control;

	return ret;
}

static int _HDMI_H15A0_HDMI_Resume_Thread(void *data)
{

	HDMI_NOTI("---- HDMI Resume Thread ---- \n");

	while(1)
	{
		_HDMI_Resume_Thread_wakeup = 0;

		wait_event_interruptible(WaitQueue_HDMI_Resume_H15A0 , _HDMI_Resume_Thread_wakeup !=0 );

		HDMI_NOTI("---- HDMI Resume Thread RUN ~~~~ \n");

		_HDMI_H15A0_Delayed_Device_Init();
	}

	return RET_OK;
}

int HDMI_H15A0_Driver_Debug(LX_HDMI_DRV_DEBUG_T *sp_hdmi_drv_debug)
{
	int ret = RET_OK;

	if(sp_hdmi_drv_debug->hdmi_debug_0 == 2)
	{
		if(sp_hdmi_drv_debug->hdmi_debug_1 == 1)
		{
			_HDMI_H15A0_TCS_addtional_value_init = sp_hdmi_drv_debug->hdmi_debug_2;
			HDMI_NOTI("!!!!!!! TCS Add value changed to [0x%x] !!!!!!!!\n", _HDMI_H15A0_TCS_addtional_value_init );
		}
		else if(sp_hdmi_drv_debug->hdmi_debug_1 == 2)
		{
			_HDMI_H15A0_TCS_mode_sel = sp_hdmi_drv_debug->hdmi_debug_2;
			HDMI_NOTI("!!!!!!! TCS Mode Sel changed to [0x%x] !!!!!!!!\n", _HDMI_H15A0_TCS_mode_sel );
		}
		else if(sp_hdmi_drv_debug->hdmi_debug_1 == 5)
		{
			_HDMI_H15A0_Phy_PDB_WA = sp_hdmi_drv_debug->hdmi_debug_2;
			HDMI_NOTI("!!!!!!! PHY PDB Workaround value changed to [0x%x] !!!!!!!!\n", _HDMI_H15A0_Phy_PDB_WA );
		}
	}
	else if(sp_hdmi_drv_debug->hdmi_debug_0 == 99) {
		if(sp_hdmi_drv_debug->hdmi_debug_1 == 1) {
			HDMI_NOTI("---- Wake up interruptiable HDMI Resume !!! \n");
			_HDMI_Resume_Thread_wakeup = 1;
			wake_up_interruptible(&WaitQueue_HDMI_Resume_H15A0);
		}
		else if(sp_hdmi_drv_debug->hdmi_debug_1 == 2) {
			HDMI_NOTI("---- Wake up interruptiable !!! \n");
			_MHL_PDB_Thread_wakeup = 1;
			wake_up_interruptible(&WaitQueue_MHL_PDB_H15A0);
		}
	}

	return ret;
}

static int _HDMI_H15A0_Check_HDCP_Abnormal(int link_num)
{
	int ret = RET_OK;
	UINT32 hdcp_authed, hdcp_enc_en, hdcp_mode;

	if(link_num > LINK_NUM_1)
		return RET_ERROR;

#ifdef USE_EXT_MHL_IC
	if(link_num == EXT_MHL_IC_LINK_NUM)
		return ret;
#endif

	LINK_NUM_H15_RdFL(link_num, hdcp_00);
	LINK_NUM_H15_Rd01(link_num, hdcp_00, reg_hdcp_authed, hdcp_authed);
	LINK_NUM_H15_Rd01(link_num, hdcp_00, reg_hdcp_enc_en, hdcp_enc_en);
	LINK_NUM_H15_Rd01(link_num, hdcp_00, reg_hdcp_mode, hdcp_mode);

	if( (hdcp_authed == 1) && (hdcp_enc_en == 1) && (hdcp_mode == 0x2) )
	{
		HDMI_NOTI("@@@@@ HDCP Authed/Enc_En , But ENC disabled link[%d] @@@@@\n", link_num );

		_HDMI_H15A0_HDCP_ResetPort(link_num);
	}

	return ret;
}

static int _HDMI_H15A0_TCS_Restart_Thread(void *data)
{
	int phy_en_prt0, phy_en_prt1;
	int phy_pdb_prt1, phy_rstn_prt1;
	int port_num;

	HDMI_DEBUG("---- TCS Restart Thread ---- \n");

	while(1)
	{
		_TCS_Restart_Thread_wakeup = 0;

		//interruptible_sleep_on(&WaitQueue_MHL_PDB_H15A0);
		wait_event_interruptible(WaitQueue_TCS_Restart_H15A0 , _TCS_Restart_Thread_wakeup !=0 );

		for (port_num = 0; port_num < 2;port_num ++)
		{
			if(_g_auth_init_intr[port_num] == 0)
				continue;

			/* All Phy Disabled except port_num */
			if (port_num != 0)
				_HDMI_H15A0_EnablePort(0, 0);
			if (port_num != 1)
				_HDMI_H15A0_EnablePort(1, 0);

			/* Enable port_num */
			_HDMI_H15A0_EnablePort(port_num, 1);

			HDMI_DEBUG("---- TCS En Control 0=>1 Port[%d]  ---- \n", port_num);

			if (lx_chip_rev() >= LX_CHIP_REV(H15, B0)) {
				PHY_REG_H15B0_RdFL(bert_tmds_sel);
				PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x0);
				PHY_REG_H15B0_WrFL(bert_tmds_sel);

				PHY_REG_H15B0_RdFL(bert_tmds_sel);
				PHY_REG_H15B0_Wr01(bert_tmds_sel,tcs_en,0x1);
				PHY_REG_H15B0_WrFL(bert_tmds_sel);
			}
			else {
				PHY_REG_H15A0_RdFL(bert_tmds_sel);
				PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x0);
				PHY_REG_H15A0_WrFL(bert_tmds_sel);

				PHY_REG_H15A0_RdFL(bert_tmds_sel);
				PHY_REG_H15A0_Wr01(bert_tmds_sel,tcs_en,0x1);
				PHY_REG_H15A0_WrFL(bert_tmds_sel);
			}

			/* if port_num is not current selected port , disable port */
			if (port_num != _gH15A0HDMIPhyInform.prt_sel)
				_HDMI_H15A0_EnablePort(port_num, 0);

			/* Enable selected port */
			_HDMI_H15A0_EnablePort(_gH15A0HDMIPhyInform.prt_sel, 1);

		}

	}

	return RET_OK;
}

static int _HDMI_H15A0_Check_HDCP_Mode(int link_num)
{
	int ret = RET_OK;

	if(link_num > LINK_NUM_1)
		return RET_ERROR;

#ifdef USE_EXT_MHL_IC
	if(link_num == EXT_MHL_IC_LINK_NUM)
		return ret;
#endif

	LINK_NUM_H15_RdFL(link_num, hdcp_00);
	LINK_NUM_H15_Rd01(link_num, hdcp_00, reg_hdcp_mode, _g_hdcp_mode_monitor[link_num]);

	if( _g_hdcp_mode_monitor[link_num] != _g_prev_hdcp_mode[link_num] )
	{
		HDMI_DEBUG("!!! HDCP Mode Changed [0x%x]=>[0x%x] link[%d] !!!\n", _g_prev_hdcp_mode[link_num] , _g_hdcp_mode_monitor[link_num], link_num );

		_g_prev_hdcp_mode[link_num] = _g_hdcp_mode_monitor[link_num];
	}

	return ret;
}

#ifdef	KDRV_CONFIG_PM
int HDMI_H15A0_RunSuspend(void)
{
	int ret = RET_OK;
	int link_num;
	LX_HDMI_HPD_T hpd;
	ULONG	flags = 0;
	hpd.bEnable = FALSE;

	HDMI_NOTI("%s entered \n", __func__);

	_g_HDMI_Device_Init_Completed = 0;

	if(_gH15A0HDMI_thread_running > 0)
	{
		spin_lock_irqsave(&_gIntrHdmiH15A0VideoLock, flags);
		for(link_num = 0;link_num <2;link_num ++)
		{
			sHDMI_H15A0_Status.Intr_vf_stable[link_num] = HDMI_VIDEO_INIT_STATE;
			sHDMI_H15A0_Status.Intr_avi[link_num] = HDMI_AVI_INIT_STATE;
			sHDMI_H15A0_Status.Intr_vsi[link_num] = HDMI_VSI_INIT_STATE;
			sHDMI_H15A0_Status.Intr_src_mute[link_num] = HDMI_SOURCE_MUTE_CLEAR_STATE;
			sHDMI_H15A0_Status.TimingReadCnt[link_num] = 0;
			sHDMI_H15A0_Status.AVIReadState[link_num] = FALSE;
			sHDMI_H15A0_Status.VSIState[link_num] = FALSE;
			sHDMI_H15A0_Status.AVIChangeState[link_num] = FALSE;
		}
		spin_unlock_irqrestore(&_gIntrHdmiH15A0VideoLock, flags);
	}

	sHDMI_H15A0_Status.device_suspend_count ++;
	sHDMI_H15A0_Status.device_in_suspend_mode = 1;

	/* 0: Save Current EDID */
	_HDMI_H15A0_EDID_Control(HDMI_EDID_SAVE);
	/* 1 : Disable HPD */
	ret |= HDMI_H15A0_SetHPD(&hpd);

	return ret;
}

int HDMI_H15A0_RunResume(void)
{
	int ret = RET_OK;

	HDMI_NOTI("%s entered \n", __func__);

	sHDMI_H15A0_Status.device_resumed_count ++;

	HDMI_H15A0_Key_Control(HDMI_KEY_CLEAR);
	_HDMI_H15A0_EDID_Control(HDMI_EDID_RESTORE);
	_HDMI_Resume_Thread_wakeup = 1;
	wake_up_interruptible(&WaitQueue_HDMI_Resume_H15A0);
	//_HDMI_H15A0_Delayed_Device_Init();
	/*
	_HDMI_H15A0_ClearSWResetAll();			//M14D Chip - Ctop control

	// phyoff will be called in delayed device init()
	//_HDMI_H15A0_PhyOff(0);
	//_HDMI_H15A0_PhyOff(1);

	OS_MsecSleep(2);	// ms delay

	_HDMI_H15A0_RunReset();		// init Link register

	//audio setting for port0 and port1
	_HDMI_H15A0_SetAudio(HDMI_LINK_0);
	_HDMI_H15A0_SetAudio(HDMI_LINK_1);

	_HDMI_H15A0_EnableInterrupt();

//	HDMI_H15A0_SetPowerControl(0);
	*/
	sHDMI_H15A0_Status.device_in_suspend_mode = 0;

	HDMI_NOTI("%s existing ... \n", __func__);

	return ret;
}
#endif


