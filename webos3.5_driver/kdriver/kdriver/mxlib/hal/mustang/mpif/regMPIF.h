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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regMPIF.h
/// @brief  MPIF Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_MPIF_H_
#define _REG_MPIF_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#define MAX_TIMEOUT								0xFFFFFFFF//0xA0000


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// BASEADDR & BK
#define BASEADDR_RIU                    0xBF000000  

#define BK_CHIPTOP                       0x3C00
#define BK_MPIF                          0x20800
#define BK_CLKGEN0                       0x1600

//----- Chip Top -------------------------
#define REG_MPIF_DRV					0x0A
#define REG_MPIF_RPUPCI_ENZ				0x0B
#define REG_ALLPAD_IN                   0x50
#define REG_CKG_MPIF                    0x68
#define REG_MPIF_MODE					0x6F

// Register offsets of MPIF
#define REG_MPIF_LC1A_SETTINGS							0x00
#define REG_MPIF_LC2A_REG_CONTROL						0x01
#define REG_MPIF_LC2A_REG_ADDRESS						0x02
#define REG_MPIF_LC2A_REG_DATA							0x03
#define REG_MPIF_LC2B_REG_CONTROL						0x04
#define REG_MPIF_LC2B_REG_ADDRESS						0x05
#define REG_MPIF_LC2B_REG_DATA							0x06
#define REG_MPIF_LC3A_PACKET_CONTROL					0x07
#define REG_MPIF_LC3A_PACKET_LENGTH						0x08
#define REG_MPIF_LC3A_PACKET_DATA						0x09	// 0x09-0x10
#define REG_MPIF_LC3A_PACKET_MIU_BASE_ADDRESS			0x11	// 0x11-0x12
#define REG_MPIF_LC3A_PACKET_MIU_BASE_ADDRESS_L			0x11	// 0x11[0:15], Base address is at 0x11-0x12
#define REG_MPIF_LC3A_PACKET_MIU_BASE_ADDRESS_H			0x12	// 0x12[16:31]
#define REG_MPIF_LC3B_PACKET_CONTROL					0x13
#define REG_MPIF_LC3B_PACKET_LENGTH						0x14
#define REG_MPIF_LC3B_PACKET_DATA						0x15	// 0x15-0x1C
#define REG_MPIF_LC3B_PACKET_MIU_BASE_ADDRESS			0x1D	// 0x1D-0x1E
#define REG_MPIF_LC3B_PACKET_MIU_BASE_ADDRESS_L			0x1D	// 0x1D[0:15], Base address is at 0x1D
#define REG_MPIF_LC3B_PACKET_MIU_BASE_ADDRESS_H			0x1E	// 0x1E[16:31]
#define REG_MPIF_LC4A_STREAM_CONTROL					0x1F
#define REG_MPIF_LC4A_STREAM_LENGTH						0x20
#define REG_MPIF_LC4A_STREAM_MIU_BASE_ADDRESS			0x21	// 0x21-0x22
#define REG_MPIF_LC4A_STREAM_MIU_BASE_ADDRESS_L			0x21	// 0x21[0:15], Base address is at 0x21
#define REG_MPIF_LC4A_STREAM_MIU_BASE_ADDRESS_H			0x22	// 0x22[16:31]
#define REG_MPIF_4WIRE_SPI_CONTROL						0x23
#define REG_MPIF_3WIRE_SPI_CONTROL						0x24
#define REG_MPIF_SPI_CONTROL							0x25
#define REG_MPIF_SPI_COMMAND_VALUE_L					0x26	// 0x26[0:15], Command value is at 0x26-0x27
#define REG_MPIF_SPI_COMMAND_VALUE_H					0x27	// 0x27[0:15]
#define REG_MPIF_SPI_DATA_LENGTH						0x28
#define REG_MPIF_SPI_MIU_BASE_ADDRESS_L					0x29	// 0x29[0:15], Base address is at 0x29-0x2A
#define REG_MPIF_SPI_MIU_BASE_ADDRESS_H					0x2A	// 0x2A[0:9]
	#define MPIF_SPI_MIU_BASEADDR_H_SHIFT				16
	#define MPIF_SPI_MIU_BASEADDR_H_MASK				0xFFFF
#define REG_MPIF_CHANNEL_BUSY_STATUS					0x2B
#define REG_MPIF_INTERRUPT_EVENT_MASK					0x2C
#define REG_MPIF_INTERRYPT_EVENT_STATUS					0x2D
#define REG_MPIF_BUSY_TIMEOUT_COUNTER					0x2E
#define REG_MPIF_BUSY_TIMEOUT_COMMAND_ID				0x2F
#define REG_MPIF_MISC1									0x30
#define REG_MPIF_MISC2									0x31
#define REG_MPIF_MISC3          						0x32
#define REG_MPIF_SYNC_CONTROL   						0x33
#define REG_MPIF_SPI_UCPLT_LENGTH						0x34
#define REG_MPIF_MIU_WPROTECT_CONTROL					0x35
#define REG_MPIF_MIU_ADDR_LOW_BOUND                     0x36
#define REG_MPIF_MIU_ADDR_LOW_BOUND_L                   0x36   // 0x36[0:15], Command value is at 0x36-0x37
#define REG_MPIF_MIU_ADDR_LOW_BOUND_H                   0x37   // 0x37[0:9]
#define REG_MPIF_MIU_ADDR_UPPER_BOUND                   0x38
#define REG_MPIF_MIU_ADDR_UPPER_BOUND_L                 0x38   // 0x38[0:15], Command value is at 0x38-0x39
#define REG_MPIF_MIU_ADDR_UPPER_BOUND_H                 0x39   // 0x39[0:9]
#define REG_MPIF_THROTTLE_CFG							0x3B
#define REG_MPIF_DEBUG_SELECTION						0x40
#define REG_MPIF_DEBUG_CHECKSUM							0x41   // 0x41-0x42
#define REG_MPIF_DEBUG_STS_IF_CTRL0						0x43
#define REG_MPIF_DEBUG_STS_IF_CTRL1						0x44
#define REG_MPIF_DEBUG_STS_PKT3A_CNT					0x45
#define REG_MPIF_DEBUG_STS_PKT3B_CNT					0x46
#define REG_MPIF_DEBUG_STS_STM0_CNT						0x47
#define REG_MPIF_DEBUG_STS_LC2X_CNT						0x48
#define REG_MPIF_DEBUG_STS_LC3X_CNT						0x49
#define REG_MPIF_DEBUG_STS_LC4X_CNT						0x4A
#define REG_MPIF_DEBUG_M2I_I2M_BISTFAIL					0x4B
#define REG_MPIF_DEBUG_MIU_MASK_RDY 					0x4C
#define REG_MPIF_DEBUG_MPIF2_SEL     					0x4D

//---------- SPIF REG Definition ---------------------------
#define REG_SPIF_LC3A_PACKET_CONTROL                    0x30
#define REG_SPIF_LC3A_CHKSUM                            0x31
#define REG_SPIF_LC3A_PACKET_LEN                        0x32
#define REG_SPIF_LC3A_MADR_L                            0x33
#define REG_SPIF_LC3A_MADR_H                            0x34

#define REG_SPIF_LC3B_PACKET_CONTROL                    0x40
#define REG_SPIF_LC3B_CHKSUM                            0x41
#define REG_SPIF_LC3B_PACKET_LEN                        0x42
#define REG_SPIF_LC3B_MADR_L                            0x43
#define REG_SPIF_LC3B_MADR_H                            0x44

#define REG_SPIF_LC4A_CONTROL                           0x50
#define REG_SPIF_LC4A_CHKSUM_L                          0x51
#define REG_SPIF_LC4A_CHKSUM_H                          0x52
#define REG_SPIF_LC4A_STREAM_LEN                        0x53
#define REG_SPIF_LC4A_MADR_L                            0x54
#define REG_SPIF_LC4A_MADR_H                            0x55

#define REG_SPIF_INT_MASK                               0x60
#define REG_SPIF_INT_CLR                                0x61
#define REG_SPIF_INT_RAW_STS                            0x63
#define REG_SPIF_INT_FINAL_STS                          0x64
#define REG_SPIF_CHANNEL_BUSY_STATUS                    0x65
#define REG_SPIF_MISC1                                  0x71
#define REG_SPIF_MISC2                                  0x72

#define SPIF_LC1A_BUSY                                  0x01
#define SPIF_LC2A_BUSY                                  0x02
#define SPIF_LC2B_BUSY                                  0x04
#define SPIF_LC3A_BUSY                                  0x08
#define SPIF_LC3B_BUSY                                  0x10
#define SPIF_LC4A_BUSY                                  0x20

#define SPIF_LC1A_TXDONE                                0x01
#define SPIF_LC2A_TXDONE                                0x02
#define SPIF_LC2B_TXDONE                                0x04
#define SPIF_LC3A_TXDONE                                0x08
#define SPIF_LC3B_TXDONE                                0x10
#define SPIF_LC4A_TXDONE                                0x20
#define SPIF_MIU_READDONE                               0x40
#define SPIF_MIU_WRITEDONE                              0x80
#define SPIF_LC2A_CKSUM_ERR                             0x0200
#define SPIF_LC2B_CKSUM_ERR                             0x0400
#define SPIF_LC3A_CKSUM_ERR                             0x0800
#define SPIF_LC3B_CKSUM_ERR                             0x1000
#define SPIF_LC4A_CKSUM_ERR                             0x2000

#define SPIF_CLK_DLY_BUFDLY_NUM							0 //0~15

#define SPIF_CLK_INV_DISABLE							0
#define SPIF_CLK_INV_ENABLE								1

#define SPIF_CLK_INV_DLY								0 //0~15

#define SPIF_CLK_INV_INV_DISABLE						0
#define SPIF_CLK_INV_INV_ENABLE							1

#define SPIF_EVENT_NULL									0x00
#define SPIF_EVENT_TX_DONE								0x01
#define SPIF_EVENT_TX_ERROR								0x02
//----------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_MPIF_H_

