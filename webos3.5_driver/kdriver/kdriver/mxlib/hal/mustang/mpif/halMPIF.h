//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _HAL_MPIF_H_
#define _HAL_MPIF_H_

#include "MsCommon.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DEBUG_MPIF(debug_level, x)     do { if (_u8MPIFDbgLevel >= (debug_level)) (x); } while(0)

//mpif clock
#define MPIF_CLOCK_160MHZ                               0x10
#define MPIF_CLOCK_120MHZ                               0x04
#define MPIF_CLOCK_108MHZ                               0x08
#define MPIF_CLOCK_86MHZ                                0x0C
#define MPIF_CLOCK_72MHZ                                0x00
#define MPIF_CLOCK_54MHZ                                0x14
#define MPIF_CLOCK_27MHZ                                0x18
#define MPIF_CLOCK_13_5MHZ                              0x1C
#define MPIF_CLOCK_SEL									MPIF_CLOCK_13_5MHZ

//status event bit    
#define MPIF_EVENTBIT_4WIRESPI                          0x01
#define MPIF_EVENTBIT_3WIRESPI                          0x02
#define MPIF_EVENTBIT_1A                                0x04
#define MPIF_EVENTBIT_2A                                0x08
#define MPIF_EVENTBIT_2B                                0x10
#define MPIF_EVENTBIT_3A                                0x20
#define MPIF_EVENTBIT_3B                                0x40
#define MPIF_EVENTBIT_4A                                0x80
#define MPIF_EVENTBIT_2A_ERR                            0x0100
#define MPIF_EVENTBIT_2B_ERR                            0x0200
#define MPIF_EVENTBIT_3A_ERR                            0x0400
#define MPIF_EVENTBIT_3B_ERR                            0x0800
#define MPIF_EVENTBIT_4A_ERR                            0x1000
#define MPIF_EVENTBIT_BUSY_TIMEOUT                      0x2000
#define MPIF_EVENTBIT_SLAVE_REQUEST                     0x4000

#define MPIF_LC3X_PACKET_DATA_MAX_LENGTH				16 // 16 bytes
#define MPIF_LC3X_MAX_PACKET                            0xFFFF

//slave id
#define MPIF_SLAVE_ID_0									0x00
#define MPIF_SLAVE_ID_1									0x01
#define MPIF_SLAVE_ID_2									0x02
#define MPIF_SLAVE_ID_3									0x03

#define MPIF_CHECKSUM_DISABLE							0x00
#define MPIF_CHECKSUM_ENABLE							0x01
#define MPIF_CHECKSUM_SEL								MPIF_CHECKSUM_ENABLE

#define MPIF_RTX_INDICATOR_DISABLE						0x00
#define MPIF_RTX_INDICATOR_ENABLE						0x01

#define MPIF_2X_DEST_SPIF								0
#define MPIF_2X_DEST_XIU								1

#define MPIF_MAX_RTX_0									0x00    // 0 time
#define MPIF_MAX_RTX_1									0x01    // 1 times
#define MPIF_MAX_RTX_2									0x02    // 2 times
#define MPIF_MAX_RTX_3									0x03    // 3 times
#define MPIF_MAX_RTX_SEL								MPIF_MAX_RTX_0

#define MPIF_LC4A_CHECKPOINT_0							0x00	// 1*256 bytes
#define MPIF_LC4A_CHECKPOINT_1							0x01	// 2*256 bytes
#define MPIF_LC4A_CHECKPOINT_2							0x02	// 3*256 bytes
#define MPIF_LC4A_CHECKPOINT_3							0x03	// 4*256 bytes

#define MPIF_CLK_THROTTLE_RATE_CYL0						0						
#define MPIF_CLK_THROTTLE_RATE_CYL1						1
#define MPIF_CLK_THROTTLE_RATE_CYL2						2
#define MPIF_CLK_THROTTLE_RATE_CYL3						3

#define MPIF_CLK_THROTTLE_SIZE_4BYTES					0	
#define MPIF_CLK_THROTTLE_SIZE_8BYTES					1
#define MPIF_CLK_THROTTLE_SIZE_12BYTES					2
#define MPIF_CLK_THROTTLE_SIZE_36BYTES					3

//-------------------------------------------------------------
//event status
#define MPIF_EVENT_NULL									0x00
#define MPIF_EVENT_TX_DONE								0x01
#define MPIF_EVENT_TX_ERROR								0x02
#define MPIF_EVENT_BUSYTIMEOUT                          0x04

#define MPIF_SPI_RIU_MAXLEN								16

#define MPIF_SPI_SRC_RIU								0
#define MPIF_SPI_SRC_MIU								1

#define MPIF_SPI_READ_MODE								0
#define MPIF_SPI_WRITE_MODE								1
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    MPIF_DBGLV_NONE,    ///< disable all the debug message
    MPIF_DBGLV_INFO,    ///< information
    MPIF_DBGLV_NOTICE,  ///< normal but significant condition
    MPIF_DBGLV_WARNING, ///< warning conditions
    MPIF_DBGLV_ERR,     ///< error conditions
    MPIF_DBGLV_CRIT,    ///< critical conditions
    MPIF_DBGLV_ALERT,   ///< action must be taken immediately
    MPIF_DBGLV_EMERG,   ///< system is unusable
    MPIF_DBGLV_DEBUG,   ///< debug-level messages
}MPIF_DebugLevel;

typedef struct _HAL_MPIF_CONFIG
{			
	MS_U8 u8trc_1A2X; //MPIF read/write turn around cycle for 1A/2X channel
	MS_U8 u8trc_3X4A; //MPIF read/write turn around cycle for 3X/4A channel
	MS_U8 u8wc; //MPIF wait ACK/NAK cycle
	MS_U8 u8Slave0Dw; //Slave 0 Data Width
	MS_U8 u8Slave1Dw; //Slave 1 Data Width
	MS_U8 u8Slave2Dw; //Slave 2 Data Width
	MS_U8 u8Slave3Dw; //Slave 3 Data Width
	MS_U8 u8CmdDw; //commend data width			
}HAL_MPIF_CONFIG;

typedef struct _HAL_MPIF_3XCTL
{
	MS_BOOL bchksum_en;
	MS_BOOL bretrx_idx;
	MS_U8 u8retrx;	
	MS_U8 u8MIUSel;
	MS_BOOL bfmode;
	MS_BOOL bNotWaitMIUDone;
	MS_U8 u8Wcnt;
	MS_U8 Reserved;
}HAL_MPIF_3XCTL;

typedef struct _HAL_MPIF_4ACTL
{		
	MS_U8 u8retrx_limit;	
	MS_U8 u8MIUSel;
	MS_BOOL bNotWaitMIUDone;
	MS_U8 u8granular;
	MS_U8 u8Wcnt;
	MS_U8 Reserved0;
	MS_U8 Reserved1;
	MS_U8 Reserved2;
}HAL_MPIF_4ACTL;

#ifdef MPIF_SPI_SUPPORT
typedef struct _HAL_MPIF_SPI_CONFIG
{    
	MS_U8 u8SerialMode;  //[0]: CPHA, [1]:CPOL
	MS_U8 u8LeadingCyc;
	MS_U8 u8TrailingCyc;
	MS_U8 u8CMDLen;
	MS_BOOL bSrcMIU;	
	MS_U8 u8MIUSel;
} HAL_MPIF_SPI_CONFIG;
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern void HAL_MPIF_Config(MS_U32 u32RegBaseAddr);
extern void HAL_MPIF_Set_Clock(MS_U8 u8clk);
extern void HAL_MPIF_CLK_Disable(MS_BOOL bDisabled);
extern void HAL_MPIF_Init(HAL_MPIF_CONFIG *pmpifconf);
extern MS_BOOL HAL_MPIF_BusyWait_ChannelFree(MS_U8 u8event_bit, MS_U32 u32timeout);
extern MS_BOOL HAL_MPIF_LC1A_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8index, MS_U8 *pu8data);
extern MS_BOOL HAL_MPIF_LC2A_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16 *pu16data);
extern MS_BOOL HAL_MPIF_LC2B_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16 *pu16data);
extern MS_BOOL HAL_MPIF_LC3A_RIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8len, MS_U8 *pu8data, HAL_MPIF_3XCTL* p3xctl);
extern MS_BOOL HAL_MPIF_LC3B_RIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8len, MS_U8 *pu8data, HAL_MPIF_3XCTL* p3xctl);
extern MS_BOOL HAL_MPIF_LC3A_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U32 u32len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_3XCTL* p3xctl);
extern MS_BOOL HAL_MPIF_LC3B_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U32 u32len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_3XCTL* p3xctl);
extern MS_BOOL HAL_MPIF_LC4A_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U16 u16len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_4ACTL* p4actl);
extern void HAL_MPIF_SET_1A2XTRCycle(MS_U8 u8trc);
extern void HAL_MPIF_SET_3X4ATRCycle(MS_U8 u8trc);
extern void HAL_MPIF_SET_WaitCycle(MS_U8 u8wc);
extern void HAL_MPIF_SET_SlaveDataWidth(MS_U8 u8slaveid, MS_U8 u8dw);
extern void HAL_MPIF_CmdDataWidth(MS_U8 u8dw);
extern void HAL_MPIF_SWReset(void);
extern MS_BOOL HAL_MPIF_SetCmdDataMode(MS_U8 u8slaveid, MS_U8 u8cmdwidth, MS_U8 u8datawidth);
extern MS_BOOL HAL_MPIF_SetSlave_ClkInv_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum);
extern MS_BOOL HAL_MPIF_SetSlave_Clk_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum);

#ifdef MPIF_SPI_SUPPORT
extern MS_BOOL HAL_MPIF_SPI_Config(MS_U8 u8slaveid, HAL_MPIF_SPI_CONFIG *pspiconfig);
extern MS_BOOL HAL_MPIF_SPI_RIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_U8 *pu8data, MS_U16 u16datalen);
extern MS_BOOL HAL_MPIF_SPI_MIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_PHYADDR u32MiuAddr, MS_U16 u16datalen);
#endif

extern MS_U8 _u8MPIFDbgLevel;
extern HAL_MPIF_CONFIG _hal_mpif_cfg;

#ifdef MPIF_SPI_SUPPORT
extern HAL_MPIF_SPI_CONFIG _hal_mpif_spi_cfg;
#endif

#endif // _HAL_MPIF_H_
