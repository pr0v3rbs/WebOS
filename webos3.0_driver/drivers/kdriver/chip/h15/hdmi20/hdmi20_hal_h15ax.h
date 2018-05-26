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
 *  @author     won.hur (won.hur@lge.com) 
 *  @version    1.0
 *  @date       2014-05-12
 *  @note       Additional information.
 */

#ifndef	_HDMI20_HAL_H15AX_H_
#define	_HDMI20_HAL_H15AX_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "hdmi_rx_ctrl.h"
#include "hdmi_rx_ctrl_hdcp.h"
#include "hdmi_rx_phy.h"
#include "hdmi20_kapi.h"
#include "hdmi_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#ifndef MSK
#define MSK(m, n)		(((1 << (m)) - 1) << (n))
#endif

/**
 * Bit mask
 * @param n shift
 */
#ifndef BIT
#define BIT(n)			MSK(1, (n))
#endif
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define HDMI20_HAL_H15AX_INITIAL_THREAD_SLEEP_MS	50
#define HDMI20_HAL_H15AX_TX_THREAD_SLEEP_MS	200

#define HDMI20_HAL_H15AX_VIDEO_CHECK_CNT	20 
#define HDMI20_HAL_H15AX_AUDIO_MUTE_CHECK_TIME 25
#define HDMI20_HAL_H15AX_AUDIO_STABLE_VALUE	1
#define HDMI20_HAL_H15AX_AUDIO_CHECK_CNT	10
#define HDMI20_HAL_H15AX_AUDIO_I2S_MUTE_CNT	15


#define HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT0	0
#define HDMI20_HAL_H15AX_LINK_DEVICE_ID_PORT1	1

#define HDMI20_HAL_H15AX_DCM_DECISION_MAX_NUM 10

#define HDMI20_HAL_NUMBER_OF_LINK			HDMI20_NUM_OF_RX_PORTS	/* H15Ax : HDMI2.0 for Port 0, 1 */
#define HDMI20_HAL_NUMBER_OF_PHY_FOR_LINK	1	/* H15Ax : Each Link has one PHY */

/* Magic word to check source device SCDC capability */
#define HDMI20_HAL_SCDC_MAGIC_WORD			0x000000FF
#define HDMI20_HAL_SRC_CAPABILITY_ADDR		0x820

#define HDMI20_HAL_H15AX_RX_INTERNAL_EDID_OFFSET	0x1000
#define HDMI20_HAL_SIZE_OF_EDID		256
/* HW mapped address */
/* RX : PORT 0 */
#define HDMI20_HAL_H15AX_RX0_CTRL_ADDR		0xC8206000
#define HDMI20_HAL_H15AX_RX0_CTRL_SIZE		0x2000

#define HDMI20_HAL_H15AX_RX0_TOP_ADDR		0xC8200000
#define HDMI20_HAL_H15AX_RX0_TOP_SIZE		0x2000

/* RX : PORT 1 */
#define HDMI20_HAL_H15AX_RX1_CTRL_ADDR		0xC820E000
#define HDMI20_HAL_H15AX_RX1_CTRL_SIZE		0x2000

#define HDMI20_HAL_H15AX_RX1_TOP_ADDR		0xC8208000
#define HDMI20_HAL_H15AX_RX1_TOP_SIZE		0x2000

/* TX : PORT 0 */
#define HDMI20_HAL_H15AX_TX0_CTRL_ADDR		0xC8340000
#define HDMI20_HAL_H15AX_TX0_CTRL_SIZE		0x20000

#define HDMI20_HAL_H15AX_TX0_TOP_ADDR		0xC8300000
#define HDMI20_HAL_H15AX_TX0_TOP_SIZE		0x10000


/* Deep Color Mode */
#define HDMI20_HAL_H15AX_RX_STATUS_REG			0x0BC
#define HDMI20_HAL_H15AX_RX_CURRENT_DCM_MASK	0xF0000000


/* I2C address */
/* RX PHY : Port 0 */
#define HDMI20_RX0_PHY_GEN3_I2C_SLAVE_ADDR	0x69
/* RX PHY : Port 1 */
#define HDMI20_RX1_PHY_GEN3_I2C_SLAVE_ADDR	0x69
/* TX PHY : Port 0 */
#define HDMI20_TX0_PHY_GEN3_I2C_SLAVE_ADDR	0x71

/* SW Workaround for ACR pll mode */
#define HDMI20_RX_ACR_PLL_MODE		0x28
#define HDMI20_RX_ACR_PLL_V		0x3C

#define HDMI20_RX_ACR_CTS		0x30
#define HDMI20_RX_ACR_N			0x2C


/* Clock */
#define HDMI20_RX_CLOCK_CONFIG_KHZ			198000	// 198MHz
#define HDMI20_RX_CLOCK_MODE_DETECTION_KHZ	49500	// 49.5MHz

#define HDMI20_RX_PHY_VERSION				213 	/* HDMIRX_PHY_TSMC_28HPM_1_8V */
#define HDMI20_RX_PHY_GENERATION			3		/* HDMI PHY version 3(used in HDMI1.4) */

/* Interrupt Number */
#define H15A0_INTERRUPT_GIC_BASE	32

#define HDMI20_RX_IRQ_TOP_PORT0		(H15A0_INTERRUPT_GIC_BASE + 80)
#define HDMI20_RX_IRQ_LINK_PORT0	(H15A0_INTERRUPT_GIC_BASE + 81)
#define HDMI20_RX_IRQ_TOP_PORT1		(H15A0_INTERRUPT_GIC_BASE + 82)
#define HDMI20_RX_IRQ_LINK_PORT1	(H15A0_INTERRUPT_GIC_BASE + 83)

#define HDMI20_TX_IRQ_LINK_PORT0	(H15A0_INTERRUPT_GIC_BASE + 84)
#define HDMI20_TX_IRQ_CEC_PORT0		(H15A0_INTERRUPT_GIC_BASE + 85) // Incase customer wants this value....


/* HDMI20 SIC Top CTRL Registers */
/* ISR Registers for 5V detection */
#define HDMI20_RX_H15AX_AD_INTR_EN				(0x110)
#define HDMI20_RX_H15AX_AD_INTR_MASK			(0x114)
#define HDMI20_RX_H15AX_AD_INTR_CLR				(0x118)
#define HDMI20_RX_H15AX_AD_INTR_DATA			(0x120)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_OUT_DE		BIT(0)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_OUT_HS		BIT(1)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_OUT_VS		BIT(2)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_CTL_DE		BIT(3)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_CTL_HS		BIT(4)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_CTL_VS		BIT(5)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_TMDS_DE	BIT(6)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_TMDS_HS	BIT(7)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_TMDS_VS	BIT(8)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_5V_FEDGE	BIT(9)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_5V_REDGE	BIT(10)
#define HDMI20_RX_H15AX_AD_INTR_FLAG_TIMER		BIT(11)

#define HDMI20_TX_H15AX_AD_MAIN_MUX_CTRL		(0x004)

#define HDMI20_RX_H15AX_AD_INFO_CLR				(0x11C)
#define HDMI20_RX_H15AX_INFO_CLR_DATA			(0x7)

#define HDMI20_RX_H15AX_AD_TMDS_HS_TOTAL		(0x20C)
#define HDMI20_RX_H15AX_AD_TMDS_H_TOTAL			(0x210)

#define HDMI20_RX_H15AX_AD_HDCP22_CONTROL		(0x81C)
#define HDMI20_RX_H15AX_HDCP_OVR_SELECT			(0x00000004)	/* 0: HDCP1.4, 1: HDCP2.2 */
#define HDMI20_RX_H15AX_HDCP_OVR_ENABLE			(0x00000002)	/* 1: bypass,  0: connect */


/* HDMI 2.0 Device Characteristic Params */
#define HDMI20_RX_H15AX_TMDS_TOLERANCE	(500)
#define	HDMI20_RX_H15AX_V_MODE_STABLE	(500000)

/* Data Packets */
#define MAX_DATA_PACKET_LENGTH	32

/* HDCP2.2 PUK, DUK */
#define HDMI20_H15AX_PUK_ADDR_1	(0xF00)
#define HDMI20_H15AX_PUK_ADDR_2	(0xF04)
#define HDMI20_H15AX_PUK_ADDR_3	(0xF08)
#define HDMI20_H15AX_PUK_ADDR_4	(0xF0C)

#define HDMI20_H15AX_DUK_ADDR_1	(0xF10)
#define HDMI20_H15AX_DUK_ADDR_2	(0xF14)
#define HDMI20_H15AX_DUK_ADDR_3	(0xF18)
#define HDMI20_H15AX_DUK_ADDR_4	(0xF1C)

#define HDMI20_H15AX_PUK_1	(0x00112233)
#define HDMI20_H15AX_PUK_2	(0x44556677)
#define HDMI20_H15AX_PUK_3	(0x8899aabb)
#define HDMI20_H15AX_PUK_4	(0xccddeeff)

#define HDMI20_RX_H15AX_DUK_1	(0x00EEDDCC)
#define HDMI20_RX_H15AX_DUK_2	(0xBBAA9988)
#define HDMI20_RX_H15AX_DUK_3	(0x77665544)
#define HDMI20_RX_H15AX_DUK_4	(0x33221100)

#define HDMI20_TX_H15AX_DUK_1	(0xFFEEDDCC)
#define HDMI20_TX_H15AX_DUK_2	(0xBBAA9988)
#define HDMI20_TX_H15AX_DUK_3	(0x77665544)
#define HDMI20_TX_H15AX_DUK_4	(0x33221100)


#define HDMI20_RX_H15B0_5V_ADDR		(0x10)	
#define HDMI20_RX_H15B0_5V_BIT		(0x00100000)

#define HDMI20_RX_H15A0_5V_LV_PORT0		(121)
#define HDMI20_RX_H15A0_5V_LV_PORT1		(120)

#define HDMI20_RX_H15A1_5V_LV_PORT0		(68)
#define HDMI20_RX_H15A1_5V_LV_PORT1		(69)

#define HDMI20_RX_H15A1_HPD_GPIO_PORT0_TEMP		(121)
#define HDMI20_RX_H15A1_HPD_GPIO_PORT1_TEMP		(120)

#define HDMI20_RX_H15A1_HPD_GPIO_PORT0_REAL		(70)
#define HDMI20_RX_H15A1_HPD_GPIO_PORT1_REAL		(67)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32 hBlankHactive;
	UINT32 hFrontHsync;
	UINT32 vFrontVsync;
	UINT32 vBlankVactive;
} HDMI20_HAL_DBG_SYNC_INFO_T;

typedef struct
{
	UINT32 hwAddr;
	volatile UINT32 * pSwAddr;
	int size;
} HDMI20_HAL_MEMORY_MAPPING_T;

typedef struct
{
	bool initiated;
	HDMI20_HAL_MEMORY_MAPPING_T topmem;	/* HDMI2.0 RX Top ctrl memory mapping */
	HDMI20_HAL_MEMORY_MAPPING_T linkmem;/* HDMI2.0 RX Link ctrl memory mapping */
	HDMI20_HAL_MEMORY_MAPPING_T ipcmem;/* HDMI2.0 RX Link ctrl memory mapping */
	int previousDCMMode[HDMI20_HAL_H15AX_DCM_DECISION_MAX_NUM];
	int currentDCMMode;
	int gWasHDCP14Written;
	int gWasHDCP22Written;
	UINT32 avmute_on_time;
	UINT32 cable_connect_time;
	int audioMuteEableCnt;
	UINT32 isManualSyncMode;
	UINT32 isResetAfterPhyLock;
	UINT32 isLongPhyRecovery;
	UINT32 real_tmds_clk;
	UINT32 real_pixel_clk;
	UINT32 real_vfreq;
	UINT32 localmutetimer;
	UINT32 vtotalbuffer;
	UINT32 updateTimingTime;
	bool isAudioARCMode;
	bool isAudioMute;
	bool isPhyLocked;
	bool scdc_change;
	bool video_change;
	bool audio_change;
	bool tmds_change;
	bool aksv_change;
	bool dcm_change;
	bool is5Vconnected;
	bool isHPD;
	bool isPhyAlterMode;
	bool isScdcUD;
	bool isHDCP22;
	bool isHDCP14;
	bool isHDCP22Ncap;
	bool isHDMI20Device;
	bool isTimingInvalid;
	bool hpd_request;
	bool phy_rst_request;
	bool connect_request;
	bool wasEdidWritten;
	bool timinginfoInvalid;
	bool check19201080Dcm;
	bool is19201080Dcm;
	int phyInitiatedMode;
	int locking_time;
	int videoAbnormalCnt;
	int videoAbResetCnt;
	int phyLostCnt;
	unsigned int gpio_hpd_real;
	unsigned int gpio_hpd_temp;
	unsigned int connected5Vtime;
	unsigned int stablehdcp14time;
	unsigned int i2cDoneSignal;
	unsigned int i2cNackSignal;
	unsigned int port5vGPIOnum;
	unsigned int port;					/* HDMI2.0 RX Number of input port */
	unsigned int irq;					/* HDMI2.0 Rx LINK IRQ */
	unsigned int top_irq;				/* HDMI2.0 Rx TOP IRQ */
	HDMI20_HAL_DBG_SYNC_INFO_T sync_info;
	struct hdmi_rx_ctrl_video timing_info;
	struct hdmi_rx_ctrl ctrl;			/* HDMI2.0 RX CTRL Data */
	struct hdmi_rx_phy phy;				/* HDMI2.0 RX PHY Data */
	struct hdmi_rx_ctrl_hdcp hdcp;		/* HDMI2.0 RX HDCP Data */
	struct hdmi_rx_ctrl_audio audio;	/* HDMI2.0 RX Audio Data */
	UINT8 dataPacket[hdmi_rx_ctrl_packet_cnt][MAX_DATA_PACKET_LENGTH];
	UINT32 edidData[HDMI20_HAL_SIZE_OF_EDID >>2];
} HDMI20_HAL_RX_DEVICE_DATA;

typedef struct
{
	bool initiated;
	HDMI20_HAL_MEMORY_MAPPING_T topmem;	/* HDMI2.0 TX Top ctrl memory mapping */
	HDMI20_HAL_MEMORY_MAPPING_T linkmem;	/* HDMI2.0 TX Top ctrl memory mapping */
	bool isHPDInserted;
} HDMI20_HAL_TX_DEVICE_DATA;

typedef struct
{
	UINT32 aud_hdmi0_fmt;
	UINT32 aud_hdmi0_pcpd;
	UINT32 aud_hdmi1_fmt;
	UINT32 aud_hdmi1_pcpd;
} HDMI20_HAL_AUDIO_DATA;

typedef enum
{
	HDMI20_RX_REFRESH_TMDS = 0,
	HDMI20_RX_REFRESH_VIDEO,
	HDMI20_RX_REFRESH_AUDIO,
	HDMI20_RX_REFRESH_DATAPACKET,
	HDMI20_RX_REFRESH_AKSV,
	HDMI20_RX_REFRESH_MAXNUM
} HDMI20_RX_REFRESH_TYPE_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int HDMI20_H15A0_Open(void);
int HDMI20_H15A0_Process_Cmd(LX_HDMI20_CMD_T* cmd);
int HDMI20_H15A0_Suspend(void);
int HDMI20_H15A0_Resume(void);

int HDMI20_HAL_H15Ax_Rx_Initiate_Phy(int port, int isUD);
void HDMI20_HAL_H15Ax_Rx_LinkVideoReset(int port, int mode);
int HDMI20_HAL_H15Ax_Rx_OverRideHDCP(int port, int isHDCP22, int enable);
int HDMI20_HAL_ConnectCable(int port, int bOnOff);

void HDMI20_H15A0_ResetDataPacketBuffer(int type ,UINT8 port);
void HDMI20_H15A0_PrintDataPacketBuffer(int type ,UINT8 port);

int HDMI20_HAL_Check5VLevelInfo(int port, int *pValue);
int HDMI20_HAL_H15Ax_Rx_DiffVideoData(struct hdmi_rx_ctrl_video *v0, struct hdmi_rx_ctrl_video *v1);
int HDMI20_HAL_H15Ax_Send_IPC(HDMI20_HAL_IPC_T type, int port);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

