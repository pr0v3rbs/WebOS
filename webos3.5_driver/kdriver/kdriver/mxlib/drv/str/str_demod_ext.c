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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   str_demod_ext.c
/// @brief  demod_ext Suspend-to-RAM (STR) related code
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/kernel.h>
#include <linux/delay.h>
#include "mst_str.h"
#include "drvMSPI.h"

#define DBG_DEMOD_EXT_STR_ENTRY(_f)   //(_f)
#define DBG_DEMOD_EXT_STR_EXIT(_f)    //(_f)
#define DBG_DEMOD_EXT_STR(_f)         //(_f)

#define DEMOD_IIC_PORD_ID      3
#define DEMOD_IIC_SLAVE_ADDR   0xF2
#define DEMOD_IIC_RETRY        1
#define DEMOD_IIC_SPEED        100
#define DEMOD_IIC_SLAVE_CFG    ((DEMOD_IIC_PORD_ID << 8) | DEMOD_IIC_SLAVE_ADDR)
#define DEMOD_IIC_CHANNEL_ID   0

#define TS_CLK_INV 1
#define TS_DATA_SWAP 0
#define TS_DATA_SERIAL 0

#define SDRAM_DATA_CHECK                 0
#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define TIMING_VERIFICATION              0
#define SDRAM_BASE                       0x5000
#define SPI_DEVICE_BUFFER_SIZE           256

//--------------------------------------------------------------------- for DVB-C
#define     C_AUTO_SCAN_SYM_RATE    1
#define     C_AUTO_SCAN_QAM	        1
#define     C_IF_INV_PWM_OUT_EN     0
#define     C_ZIF                   0
#define     C_FC_L                  0x88
#define     C_FC_H                  0x13
#define     C_FS_L                  0xC0
#define     C_FS_H                  0x5D
#define     C_BW_L                  0xDB
#define     C_BW_H                  0x1A
#define     C_BW1_L                 0xF4
#define     C_BW1_H                 0x1A
#define     C_BW2_L                 0xDB
#define     C_BW2_H                 0x1A
#define     C_BW3_L                 0x58
#define     C_BW3_H                 0x1B
#define     C_QAM                   2
#define     C_CCI                   0
#define     C_TS_SERIAL             TS_DATA_SERIAL
#define     C_TS_CLK_RATE           6
#define     C_TS_OUT_INV            TS_CLK_INV
#define     C_TS_DATA_SWAP          TS_DATA_SWAP
#define     C_IQ_SWAP               0
#define     C_TS_SPREAD_SPAN          0 //Unit: kHz (0: disable; default: 40 kHz)
#define     C_TS_SPREAD_STEP_SIZE    0.0 //Unit: percent(%) (0: disable; default: 3.0 %)
//--------------------------------------------------------------------- for DVB-SS2

//SSPI COMMAND
#define RIU_W_CMD      0x1A
#define RIU_W1_CMD     0x1D
#define RIU_R_CMD       0x18
#define RIU_R1_CMD      0x1C
#define RIU_RT_CMD      0x11
#define RIU_R1T_CMD      0x15
#define MIU_W_CMD      0x25
#define MIU_R_CMD   0x20
#define MIU_ST_CMD       0x21
#define CFG_W_CMD       0x05
#define CFG_R_CMD       0x00

#define MSB1240_BOOT_START_ADDR     0x00000
#define MSB1240_BOOT_END_ADDR       0x00FFF
#define MSB1240_DVBT2_P1_START_ADDR 0x01000
#define MSB1240_DVBT2_P1_END_ADDR   0x08FFF
#define MSB1240_DVBT2_P2_START_ADDR 0x09000
#define MSB1240_DVBT2_P2_END_ADDR   0x0FFFF
#define MSB1240_DVBT_START_ADDR     0x10000
#define MSB1240_DVBT_END_ADDR       0x17FFF
#define MSB1240_DVBC_START_ADDR     0x18000
#define MSB1240_DVBC_END_ADDR       0x1FFFF
#define MSB1240_DVBS_START_ADDR     0x20000
#define MSB1240_DVBS_END_ADDR       0x28FFF

#define MSB1240_WINDOWS_BASE                0x100   // unit: 4K  // 0x100*4k base = 0x100000(1M)
#define MSB1240_BOOT_WINDOWS_OFFSET         MSB1240_WINDOWS_BASE    // 1M
#define MSB1240_DVBT2_P2_WINDOWS_OFFSET    (MSB1240_WINDOWS_BASE + 0x08)   // 1M+32k
#define MSB1240_DVBT2_P1_WINDOWS_OFFSET    (MSB1240_DVBT2_P2_WINDOWS_OFFSET + 0x08)   // 1M+64k
#define MSB1240_DVBT_WINDOWS_OFFSET        (MSB1240_DVBT2_P1_WINDOWS_OFFSET + 0x08)   // 1M+96k
#define MSB1240_DVBS_WINDOWS_OFFSET        (MSB1240_DVBT_WINDOWS_OFFSET + 0x08)   // 1M+128k
#define MSB1240_DVBC_WINDOWS_OFFSET        (MSB1240_DVBS_WINDOWS_OFFSET + 0x10)   // 1M+128k+64k

#define REG_MB_CNTL     0x0C80
#define REG_MB_ADDR_L   0x0C84
#define REG_MB_ADDR_H   0x0C82
#define REG_MB_DATA     0x0C86
#define REG_FSM_EN       0x0CB8
#define MSB1240_MB_CNT_TH 0x7FF

#define MSB1240_BOOT  0x01
#define MSB1240_DVBT2 0x02
#define MSB1240_DVBT  0x04
#define MSB1240_DVBC  0x08
#define MSB1240_DVBS  0x10
#define MSB1240_ALL   0x0F
#define MSB1240_disconnect 0x80

#define MSB1240_DVBS_ADCPLL_IQ_SWAP          1//0 //1 is ADCPLL IQ swap enable, for customer issue.

typedef enum
{
    ///< QAM 16
    mapi_demodulator_datatype_E_CAB_QAM16 = 0,
    ///< QAM 32
    mapi_demodulator_datatype_E_CAB_QAM32 = 1,
    ///< QAM 64
    mapi_demodulator_datatype_E_CAB_QAM64 = 2,
    ///< QAM 128
    mapi_demodulator_datatype_E_CAB_QAM128 = 3,
    ///< QAM 256
    mapi_demodulator_datatype_E_CAB_QAM256 = 4,
    ///< Invalid value
    mapi_demodulator_datatype_E_CAB_INVALID
} mapi_demodulator_datatype_EN_CAB_CONSTEL_TYPE;

typedef enum
{
    /// 6 MHz
    E_RF_CH_BAND_6MHz = 0x01,
    /// 7 MHz
    E_RF_CH_BAND_7MHz = 0x02,
    /// 8 MHz
    E_RF_CH_BAND_8MHz = 0x03,
    /// others...
    E_RF_CH_BAND_1MHz,
    E_RF_CH_BAND_1p7MHz,
    E_RF_CH_BAND_2MHz,
    E_RF_CH_BAND_3MHz,
    E_RF_CH_BAND_4MHz,
    E_RF_CH_BAND_5MHz,
    E_RF_CH_BAND_9MHz,
    E_RF_CH_BAND_10MHz,    
    /// invalid
    E_RF_CH_BAND_INVALID
} RF_CHANNEL_BANDWIDTH;

typedef enum                            // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
{
    E_DEMOD_BW_17M    = 0x0,
    E_DEMOD_BW_5M      = 0x1,
    E_DEMOD_BW_6M      = 0x2,
    E_DEMOD_BW_7M      = 0x3,
    E_DEMOD_BW_8M      = 0x4,
    E_DEMOD_BW_10M    = 0x5,
}E_DEMOD_CHANNEL_BANDWIDTH;

typedef enum
{
    demod_ext_datatype_E_DEVICE_DEMOD_ATV = 0,
    demod_ext_datatype_E_DEVICE_DEMOD_DVB_T,
    demod_ext_datatype_E_DEVICE_DEMOD_DVB_C,
    demod_ext_datatype_E_DEVICE_DEMOD_DVB_S,
    demod_ext_datatype_E_DEVICE_DEMOD_DTMB,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC_VSB,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC_QPSK,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC_16QAM,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC_64QAM,
    demod_ext_datatype_E_DEVICE_DEMOD_ATSC_256QAM,
    demod_ext_datatype_E_DEVICE_DEMOD_DVB_T2,
    demod_ext_datatype_E_DEVICE_DEMOD_ISDB,
    demod_ext_datatype_E_DEVICE_DEMOD_MAX,
    demod_ext_datatype_E_DEVICE_DEMOD_NULL = demod_ext_datatype_E_DEVICE_DEMOD_MAX,
} demod_ext_datatype_EN_DEVICE_DEMOD_TYPE;

typedef enum
{
    // operation mode settings
    T_OPMODE_RFAGC_EN  = 0x20,   // 0x20
    T_OPMODE_HUMDET_EN,
    T_OPMODE_DCR_EN,
    T_OPMODE_IIS_EN,
    T_OPMODE_CCI_EN,
    T_OPMODE_ACI_EN,
    T_OPMODE_IQB_EN,
    T_OPMODE_AUTO_IQ,
    T_OPMODE_AUTO_RFMAX,    // 0x28
    T_OPMODE_AUTO_ACI,
    T_OPMODE_FIX_MODE_CP,
    T_OPMODE_FIX_TPS,
    T_OPMODE_AUTO_SCAN,
    T_OPMODE_RSV_0X2D,
    T_OPMODE_RSV_0X2E,
    T_OPMODE_RSV_0X2F,

    // channel config param
    T_CONFIG_RSSI,    // 0x30
    T_CONFIG_ZIF,
    T_CONFIG_FREQ,
    T_CONFIG_FC_L,
    T_CONFIG_FC_H,
    T_CONFIG_FS_L,
    T_CONFIG_FS_H,
    T_CONFIG_BW,
    T_CONFIG_MODE,    // 0x38
    T_CONFIG_CP,
    T_CONFIG_LP_SEL,
    T_CONFIG_CSTL,
    T_CONFIG_HIER,
    T_CONFIG_HPCR,
    T_CONFIG_LPCR,
    T_CONFIG_IQ_SWAP,
    T_CONFIG_RFMAX,    // 0x40
    T_CONFIG_CCI,
    T_CONFIG_ICFO_RANGE,
    T_CONFIG_RFAGC_REF,
    T_CONFIG_IFAGC_REF_2K,
    T_CONFIG_IFAGC_REF_8K,
    T_CONFIG_IFAGC_REF_ACI,
    T_CONFIG_IFAGC_REF_IIS_2K,
    T_CONFIG_IFAGC_REF_IIS_8K,    // 0x48
    T_CONFIG_ACI_DET_TH_L,
    T_CONFIG_ACI_DET_TH_H,
    T_CONFIG_TS_SERIAL,
    T_CONFIG_TS_CLK_RATE,
    T_CONFIG_TS_OUT_INV,
    T_CONFIG_TS_DATA_SWAP,
    T_CONFIG_2K_SFO_H,
    T_CONFIG_2K_SFO_L,    // 0x50
    T_CONFIG_8K_SFO_H,
    T_CONFIG_8K_SFO_L,
    T_CONFIG_CHECK_CHANNEL,
    T_CONFIG_SLICER_SNR_POS,
    T_CONFIG_TDP_CCI_KP,
    T_CONFIG_CCI_FSWEEP,
    T_CONFIG_TS_CLK_RATE_AUTO,
    T_CONFIG_IF_INV_PWM_OUT_EN,

    /**********************
     * crc =
     *    ~(T_OPMODE_RFAGC_EN^T_OPMODE_HUMDET_EN^....^T_CONFIG_TS_CLK_RATE_AUTO)
     ************************/
    // T_PARAM_CHECK_SUM,

    // dvbt lock history
    T_DVBT_LOCK_HIS   = 0xF0,
    T_DVBT2_NOCHAN_Flag = 0xF1, 
    T_DVBT_NOCHAN_Flag = 0xF2,
    T_DETECT_DONE_FLAG = 0xF3,
    T_SNR100_L,             // 0xF3
    T_SNR100_H,             // 0xF4
    T_CFO10K_L,             // 0xF5
    T_CFO10K_H,             // 0xF6
    T_CONFIG_SPREAD_SPAN,   //0xF7
    T_CONFIG_SPREAD_STEP,   //0xF8

    DVBT_PARAM_LEN,
} DVBT_Param;

typedef enum
{
    // fw version, check sum
    E_T2_CHECK_SUM_L      = 0x00,
    E_T2_CHECK_SUM_H,
    E_T2_FW_VER_0,
    E_T2_FW_VER_1,
    E_T2_FW_VER_2,

    // operation mode
    E_T2_ZIF_EN           = 0x20,
    E_T2_RF_AGC_EN,
    E_T2_HUM_DET_EN,
    E_T2_DCR_EN,
    E_T2_IQB_EN,
    E_T2_IIS_EN,
    E_T2_CCI_EN,
    E_T2_LOW_PWR_DET_EN,
    E_T2_ACI_DET_EN,
    E_T2_TD_MOTION_EN,
    E_T2_FD_MOTION_EN,

    // channel tuning param
    E_T2_BW               = 0x40,
    E_T2_FC_L             = 0x41,
    E_T2_FC_H             = 0x42,
    E_T2_FS_L,
    E_T2_FS_H,
    E_T2_ZIF,
    E_T2_GI,
    E_T2_ACI_DET_TYPE,
    E_T2_AGC_REF,         //0x48
    E_T2_RSSI_REF,
    E_T2_SNR_TIME_L,
    E_T2_SNR_TIME_H,
    E_T2_BER_CMP_TIME_L,
    E_T2_BER_CMP_TIME_H,
    E_T2_SFO_CFO_NUM,
    E_T2_CCI,
    E_T2_ACI_DET_TH_L,    //0x50
    E_T2_ACI_DET_TH_H,
    E_T2_TS_SERIAL     = 0x52,
    E_T2_TS_CLK_RATE   = 0x53,
    E_T2_TS_OUT_INV    = 0x54,
    E_T2_TS_DATA_SWAP  = 0x55,
    E_T2_TDP_CCI_KP,
    E_T2_CCI_FSWEEP,      //0x57
    E_T2_TS_ERR_POL,      //0x58
    E_T2_IF_AGC_INV_PWM_EN, // 0x59

    E_T2_TOTAL_CFO_0      = 0x85,
    E_T2_TOTAL_CFO_1,
    
    // dvbt2 lock history
    E_T2_DVBT2_LOCK_HIS   = 0xF0,
    E_T2_FEF_DET_IND,
    E_T2_MPLP_NO_COMMON_IND,
    E_T2_SNR_L,             // 0xf3
    E_T2_SNR_H,             // 0xf4    
    E_T2_DOPPLER_DET_FLAG,  // 0xf5
    E_T2_DOPPLER_DET_TH_L,  // 0xf6
    E_T2_DOPPLER_DET_TH_H,  // 0xf7
    E_T2_SPREAD_SPAN,       //0xf8
    E_T2_SPREAD_STEP,       //0xf9
    E_T2_FEF_DET_IND_S2,    //0xfa
    E_T2_PKT_PER_SEC_L,       //0xfb
    E_T2_PKT_PER_SEC_H,       //0xfc

    // splp, mplp releted
    E_T2_PLP_ID_ARR       = 0x100,
    
    E_T2_PKT_ACCU_0       =0x108,
		E_T2_PKT_ACCU_1,   //0x109
    E_T2_PKT_ACCU_2,   //0x10a
    E_T2_PKT_ACCU_3,   //0x10b
    E_T2_PKT_READ_CTRL,  //0x10c    
    
    E_T2_L1_FLAG          = 0x120,
    E_T2_PLP_ID,
    E_T2_GROUP_ID,
    E_T2_PARAM_NUM,
} E_DVBT2_PARAM;

typedef enum
{
    // fw version, check sum,Address Start at 0x00
    C_CHECK_SUM_L = 0x00,
    C_CHECK_SUM_H,
    C_FW_VER_0,
    C_FW_VER_1,
    C_FW_VER_2,
	
    // Operation Mode Settings,Address Start at 0x20
    C_opmode_auto_scan_sym_rate= 0x20,
    C_opmode_auto_scan_qam,
    C_if_inv_pwm_out_en,

    // Config Params
    C_config_zif= 0x23,
    C_config_fc_l,
    C_config_fc_h,
    C_config_fs_l,
    C_config_fs_h,
    C_config_bw_l,    // 0x28
    C_config_bw_h,
    C_config_bw1_l,
    C_config_bw1_h,
    C_config_bw2_l,
    C_config_bw2_h,
    C_config_bw3_l,
    C_config_bw3_h,
    C_config_qam,     // 0x30
    C_config_cci,
    C_config_ts_serial,
    C_config_ts_clk_rate,
    C_config_ts_out_inv,
    C_config_ts_data_swap,
    C_config_iq_swap,   

     //not always changed
     C_opmode_rfagc_en,
     C_opmode_humdet_en,   // 0x38 
     C_opmode_dcr_en,   
     C_opmode_iqb_en,   
     C_opmode_auto_iq,
     C_opmode_auto_rfmax,

     C_opmode_atv_detector_en,

     C_config_rssi,  
     C_config_rfmax,
     
     C_lock_indicator, //0x40, [0] = TR lock HIS , [7] = FEC lock

     C_CFO10_L,
     C_CFO10_H,

     C_SNR100_L,
     C_SNR100_H,
     C_config_spread_span,
     C_config_spread_step,

    DVBC_PARAM_LEN,
} DVBC_Param;

typedef enum
{
    E_S2_ZIF_EN = 0x00,
    E_S2_RF_AGC_EN,
    E_S2_DCR_EN,
    E_S2_IQB_EN,
    E_S2_IIS_EN, 
    E_S2_CCI_EN,
    E_S2_FORCE_ACI_SELECT,
    E_S2_IQ_SWAP,           //For DVBS2
    E_S2_AGC_REF_EXT_0,
    E_S2_AGC_REF_EXT_1,
    E_S2_AGC_K,
    E_S2_ADCI_GAIN,
    E_S2_ADCQ_GAIN,
    E_S2_SRD_SIG_SRCH_RNG,
    E_S2_SRD_DC_EXC_RNG,
    E_S2_FORCE_CFO_0,       //0FH
    E_S2_FORCE_CFO_1,
    E_S2_DECIMATION_NUM,
    E_S2_PSD_SMTH_TAP,
    E_S2_CCI_FREQN_0_L,
    E_S2_CCI_FREQN_0_H,
    E_S2_CCI_FREQN_1_L,
    E_S2_CCI_FREQN_1_H,
    E_S2_CCI_FREQN_2_L,
    E_S2_CCI_FREQN_2_H,
    E_S2_TR_LOPF_KP,
    E_S2_TR_LOPF_KI,
    E_S2_FINEFE_KI_SWITCH_0,
    E_S2_FINEFE_KI_SWITCH_1,
    E_S2_FINEFE_KI_SWITCH_2,
    E_S2_FINEFE_KI_SWITCH_3,
    E_S2_FINEFE_KI_SWITCH_4,    //1FH
    E_S2_PR_KP_SWITCH_0,
    E_S2_PR_KP_SWITCH_1,
    E_S2_PR_KP_SWITCH_2,
    E_S2_PR_KP_SWITCH_3,
    E_S2_PR_KP_SWITCH_4,
    E_S2_FS_GAMMA,
    E_S2_FS_ALPHA0,
    E_S2_FS_ALPHA1,
    E_S2_FS_ALPHA2,
    E_S2_FS_ALPHA3,
    E_S2_FS_H_MODE_SEL,
    E_S2_FS_OBSWIN,
    E_S2_FS_PEAK_DET_TH_L,
    E_S2_FS_PEAK_DET_TH_H,
    E_S2_FS_CONFIRM_NUM,
    E_S2_EQ_MU_FFE_DA,      //2FH
    E_S2_EQ_MU_FFE_DD,
    E_S2_EQ_ALPHA_SNR_DA,
    E_S2_EQ_ALPHA_SNR_DD,
    E_S2_FEC_ALFA,                  //For DVBS2
    E_S2_FEC_BETA,                  //For DVBS2
    E_S2_FEC_SCALING_LLR,           //For DVBS2
    E_S2_TS_SERIAL,
    E_S2_TS_CLK_RATE,
    E_S2_TS_OUT_INV, 
    E_S2_TS_DATA_SWAP,
    //------------------------------------------
    E_S2_FW_VERSION_L,              //0x3A
    E_S2_FW_VERSION_H,              //0x3B
    E_S2_CHIP_VERSION,
    E_S2_FS_L,                      //Frequency
    E_S2_FS_H,                      //Frequency
    E_S2_MANUAL_TUNE_SYMBOLRATE_L,  //0x3F
    E_S2_MANUAL_TUNE_SYMBOLRATE_H,  //
    E_S2_SYSTEM_TYPE,               //DVBS/S2
    E_S2_MODULATION_TYPE,           //QPSK/8PSK
    E_S2_CODERATE,                  //1/2

    E_S2_UNCRT_PKT_NUM_7_0,
    E_S2_UNCRT_PKT_NUM_8_15,
    E_S2_STATE_FLAG,                //H
    E_S2_SUBSTATE_FLAG,             //L
    E_S2_HUM_DETECT_FLAG,
    E_S2_CCI_DETECT_FLAG,
    E_S2_IIS_DETECT_FLAG,
    E_S2_OPEN_HUM_VLD_IRQ_FLAG,
    E_S2_SRD_COARSE_DONE_FLAG,
    E_S2_SRD_FINE_DONE_FLAG,
    E_S2_FINEFE_DONE_FLAG,
    E_S2_REV_FRAME_FLAG,            //0x4F
    E_S2_DUMMY_FRAME_FLAG,
    E_S2_PLSC_DONE_FLAG,
    E_S2_GET_INFO_FROM_FRAME_LENGTH_DONE_FLAG,
    E_S2_IQ_SWAP_DETECT_FLAG,
    E_S2_FRAME_ACQUISITION_DONE_FLAG,
    E_S2_OLCFE_DONE_FLAG,
    E_S2_FSYNC_FOUND_FLAG,
    E_S2_FSYNC_FAIL_SEARCH_FLAG,
    E_S2_FALSE_ALARM_FLAG,
    E_S2_VITERBI_IN_SYNC_FLAG,
    E_S2_INT_CODE_RATE_SEARCH_FAIL_FLAG,
    E_S2_VITERBI_INT_PRE_FLAG,
    E_S2_BER_WINDOW_END_FLAG,
    E_S2_PASS_WRONG_INT_FLAG,
    E_S2_CLK_CNT_OVER_FLAG,
    E_S2_UNCRT_OVER_FLAG,           //0x5F
    E_S2_DISEQC_RX_LENGTH,
    E_S2_DISEQC_INTERRUPT_FLAG,
    E_S2_DISEQC_RX_FLAG,
    E_S2_DISEQC_INTERRUPT_STATUS,
    E_S2_DISEQC_STATUS_FLAG,
    E_S2_ACI_FIR_SELECTED,          //0x65
    //LOCK    
    E_S2_AGC_LOCK_FLAG,
    E_S2_DCR_LOCK_FLAG,
    E_S2_DAGC0_LOCK_FLAG,
    E_S2_DAGC1_LOCK_FLAG,
    E_S2_DAGC2_LOCK_FLAG,
    E_S2_DAGC3_LOCK_FLAG,
    E_S2_TR_LOCK_FLAG,
    E_S2_CLCFE_LOCK_FLAG,
    E_S2_EQ_LOCK_FLAG,
    E_S2_PR_LOCK_FLAG,              //0x6F
    E_S2_FSYNC_LOCK_FLAG,
    E_S2_FSYNC_FAIL_LOCK_FLAG,

    E_S2_MB_SWUSE12L,               //0x72
    E_S2_MB_SWUSE12H,
    E_S2_MB_SWUSE13L,
    E_S2_MB_SWUSE13H,
    E_S2_MB_SWUSE14L,
    E_S2_MB_SWUSE14H,
    E_S2_MB_SWUSE15L,
    E_S2_MB_SWUSE15H,
    E_S2_MB_SWUSE16L,
    E_S2_MB_SWUSE16H,
    E_S2_MB_SWUSE17L,
    E_S2_MB_SWUSE17H,
    E_S2_MB_SWUSE18L,
    E_S2_MB_SWUSE18H,               //0x7F
    E_S2_MB_SWUSE19L,
    E_S2_MB_SWUSE19H,
    E_S2_MB_SWUSE1AL,
    E_S2_MB_SWUSE1AH,
    E_S2_MB_SWUSE1BL,
    E_S2_MB_SWUSE1BH,
    E_S2_MB_SWUSE1CL,
    E_S2_MB_SWUSE1CH,
    E_S2_MB_SWUSE1DL,
    E_S2_MB_SWUSE1DH,
    E_S2_MB_SWUSE1EL,
    E_S2_MB_SWUSE1EH,
    E_S2_MB_SWUSE1FL,
    E_S2_MB_SWUSE1FH,               //0x8D

    E_S2_MB_DMDTOP_DBG_0,
    E_S2_MB_DMDTOP_DBG_1,           //0x8F
    E_S2_MB_DMDTOP_DBG_2,
    E_S2_MB_DMDTOP_DBG_3,
    E_S2_MB_DMDTOP_DBG_4,
    E_S2_MB_DMDTOP_DBG_5,
    E_S2_MB_DMDTOP_DBG_6,
    E_S2_MB_DMDTOP_DBG_7,
    E_S2_MB_DMDTOP_DBG_8,
    E_S2_MB_DMDTOP_DBG_9,
    E_S2_MB_DMDTOP_DBG_A,
    E_S2_MB_DMDTOP_DBG_B,

    E_S2_MB_DMDTOP_SWUSE00L,
    E_S2_MB_DMDTOP_SWUSE00H,
    E_S2_MB_DMDTOP_SWUSE01L,
    E_S2_MB_DMDTOP_SWUSE01H,
    E_S2_MB_DMDTOP_SWUSE02L,
    E_S2_MB_DMDTOP_SWUSE02H,        //0x9F
    E_S2_MB_DMDTOP_SWUSE03L,
    E_S2_MB_DMDTOP_SWUSE03H,
    E_S2_MB_DMDTOP_SWUSE04L,
    E_S2_MB_DMDTOP_SWUSE04H,
    E_S2_MB_DMDTOP_SWUSE05L,
    E_S2_MB_DMDTOP_SWUSE05H,
    E_S2_MB_DMDTOP_SWUSE06L,
    E_S2_MB_DMDTOP_SWUSE06H,
    E_S2_MB_DMDTOP_SWUSE07L,
    E_S2_MB_DMDTOP_SWUSE07H,

    E_S2_MB_TOP_WR_DBG_90,          //0xAA
    E_S2_MB_TOP_WR_DBG_91,
    E_S2_MB_TOP_WR_DBG_92,
    E_S2_MB_TOP_WR_DBG_93,
    E_S2_MB_TOP_WR_DBG_94,
    E_S2_MB_TOP_WR_DBG_95,          //0xAF
    E_S2_MB_TOP_WR_DBG_96,
    E_S2_MB_TOP_WR_DBG_97,
    E_S2_MB_TOP_WR_DBG_98,
    E_S2_MB_TOP_WR_DBG_99,

    E_S2_MB_DUMMY_REG_0,
    E_S2_MB_DUMMY_REG_1,
    E_S2_MB_DUMMY_REG_2,
    E_S2_MB_DUMMY_REG_3,
    E_S2_MB_DUMMY_REG_4,
    E_S2_MB_DUMMY_REG_5,
    E_S2_MB_DUMMY_REG_6,
    E_S2_MB_DUMMY_REG_7,
    E_S2_MB_DUMMY_REG_8,
    E_S2_MB_DUMMY_REG_9,
    E_S2_MB_DUMMY_REG_A,
    E_S2_MB_DUMMY_REG_B,            //0xBF
    E_S2_MB_DUMMY_REG_C,
    E_S2_MB_DUMMY_REG_D,
    E_S2_MB_DUMMY_REG_E,
    E_S2_MB_DUMMY_REG_F,
    E_S2_MB_DUMMY_REG_10,
    E_S2_MB_DUMMY_REG_11,

    E_S2_MB_DMDTOP_INFO_01,
    E_S2_MB_DMDTOP_INFO_02,
    E_S2_MB_DMDTOP_INFO_03,
    E_S2_MB_DMDTOP_INFO_04,
    E_S2_MB_DMDTOP_INFO_05,         //0xCA
    E_S2_MB_DMDTOP_INFO_06,
    E_S2_MB_DMDTOP_INFO_07,
    E_S2_MB_DMDTOP_INFO_08,

    E_S2_IDLE_STATE_UPDATED,
    E_S2_LOG_FLAG,                  //0xCF
    E_S2_LOG_SKIP_INDEX,
    E_S2_LOCK_COUNT,
    E_S2_NARROW_STEP_FLAG,
    E_S2_UNCORRECT_PKT_COUNT,
    E_S2_DISEQC_INIT_MODE,
    E_S2_DECIMATE_FORCED,           //0xD5
    E_S2_SRD_MAX_SRG_FLAG,
    E_S2_DVBS_OUTER_RETRY,

    E_S2_FORCED_DECIMATE_FLAG,
    E_S2_NO_SIGNAL_FLAG,
    E_S2_SPECTRUM_TRACK_FLAG,
    E_S2_SRD_LOCAL_SEARCH_FLAG,
    E_S2_NO_SIGNAL_RATIO_CHECK_FLAG,
    E_S2_LOW_SR_ACI_FLAG,
    E_S2_SPECTRUM_TRACKER_TIMEOUT,
    E_S2_TR_TIMEOUT,                //0xDF
    E_S2_BALANCE_TRACK,
    E_S2_GAIN_TILT_FLAG,            //0xE1
    E_S2_IFAGC_POLARITY,            //0xE2
    E_S2_SPREAD_SPAN,               //0xE3
    E_S2_SPREAD_STEP,               //0xE4
    E_S2_TR_Indicator_ff_A,         //0xE5
    E_S2_TR_Indicator_ff_B,         //0xE6
    E_S2_TS_serial_mode,            //0xE7
    E_S2_10SNR_L,                   //0xE8
    E_S2_10SNR_H,                   //0xE9
    E_S2_TUNER_CFO_L,              //0xEA
    E_S2_TUNER_CFO_H,              //0xEB
    E_S2_PACKET_ERROR_PER_SEC_L,    //0xEC
    E_S2_PACKET_ERROR_PER_SEC_H,    //0xED
    
    DVBS2_PARAM_LEN                 //0xEE
}E_DVBS2_PARAM;

const static MS_U8 _demod_ext_fw_dat[] =
{
   #include "msb124x_dvbt2.dat"
};

#define     S_TS_SPREAD_SPAN          0 //Unit: kHz (0: disable; default: 40 kHz)
#define     S_TS_SPREAD_STEP_SIZE    0.0 //Unit: percent(%) (0: disable; default: 3.0 %)
static MS_U8 S_TS_Output_VAL = TS_DATA_SERIAL;
static MS_BOOL S_TS_DataSwap_VAL = TS_DATA_SWAP;
static MS_BOOL S_TS_clk_inv_VAL = TS_CLK_INV;
static MS_U8 g_msb1240_plp_id   = 0xff;

//configure
//--------------------------------------------------------------------- for DVB-T2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04

// FC: FC = FS = 5000 = 0x1388     (5.0MHz IF)
#define T2_FC_L_VAL            0x88    // 5.0M
#define T2_FC_H_VAL            0x13
#define T2_TS_SERIAL_VAL        TS_DATA_SERIAL
#define T2_TS_CLK_RATE_VAL      0x06
#define T2_TS_OUT_INV_VAL       TS_CLK_INV
#define T2_TS_DATA_SWAP_VAL     TS_DATA_SWAP
#define T2_TS_ERR_POL_VAL       0x00
#define T2_IF_AGC_INV_PWM_EN_VAL 0x00
#define T2_TS_SPREAD_SPAN          0 //Unit: kHz (0: disable; default: 40 kHz)
#define T2_TS_SPREAD_STEP_SIZE    0.0 //Unit: percent(%) (0: disable; default: 3.0 %)
static MS_U8   T2_TS_SERIAL = T2_TS_SERIAL_VAL;
static MS_U8   T2_TS_CLK_INV = T2_TS_OUT_INV_VAL;

//--------------------------------------------------------------------- for DVB-T
//operation

//configure
#define T_FC_L                    0xc0
#define T_FC_H                    0x12
#define T_FS_L                    0x80
#define T_FS_H                    0x70
#define T_BW                      0x03
#define T_IQ_SWAP                 0x00
#define T_SERIAL_TS               TS_DATA_SERIAL
#define T_TS_CLK_SEL              0x06
#define T_TS_OUT_INV              TS_CLK_INV
#define T_TS_DATA_SWAP            TS_DATA_SWAP
#define T_IF_INV_PWM_OUT_EN  0x00
#define T_TS_SPREAD_SPAN          0 //Unit: kHz (0: disable; default: 40 kHz)
#define T_TS_SPREAD_STEP_SIZE    0.0 //Unit: percent(%) (0: disable; default: 3.0 %)
static MS_U8   T_SERIAL_TS_VAL = T_SERIAL_TS;

MS_U8 MSB1240_DVBT_DSPREG_TABLE[] =
{
    T_BW,  T_FC_L,  T_FC_H,  T_SERIAL_TS,  T_TS_CLK_SEL,  T_TS_OUT_INV,  T_TS_DATA_SWAP,
    T_IQ_SWAP,  T_IF_INV_PWM_OUT_EN
};

MS_U8 demod_ext_DVBC_DSPREG_TABLE[] =
{
    C_AUTO_SCAN_SYM_RATE,  //0x20
    C_AUTO_SCAN_QAM,
    C_IF_INV_PWM_OUT_EN,
    C_ZIF,
    C_FC_L,
    C_FC_H,
    C_FS_L,
    C_FS_H,

    C_BW_L,             //0x28
    C_BW_H,
    C_BW1_L,
    C_BW1_H,
    C_BW2_L,
    C_BW2_H,
    C_BW3_L,
    C_BW3_H,

    C_QAM,               //0x30
    C_CCI,
    C_TS_SERIAL,
    C_TS_CLK_RATE,
    C_TS_OUT_INV,
    C_TS_DATA_SWAP,
    C_IQ_SWAP,
};

extern MS_S32 MDrv_SW_IIC_Write(MS_U8 u8ChIIC, MS_U8 u8SlaveID, MS_U8 u8AddrCnt, MS_U8* pu8Addr, MS_U32 u32BufLen, MS_U8* pu8Buf);
extern MS_S32 MDrv_SW_IIC_Read(MS_U8 u8ChIIC, MS_U8 u8SlaveID, MS_U8 u8AddrCnt, MS_U8* pu8Addr, MS_U32 u32BufLen, MS_U8* pu8Buf);

extern void MDrv_GPIO_Init(void);
extern void MDrv_GPIO_Set_High(MS_U8 u8IndexGPIO);
extern void MDrv_GPIO_Set_Low(MS_U8 u8IndexGPIO);
extern void MDrv_IIC_Init(void);
extern void MDrv_SW_IIC_Enable(MS_U8 u8ChIIC, MS_U16 bEnable);
extern void MDrv_SW_IIC_SetSpeed(MS_U8 u8ChIIC, MS_U8 u8Speed);

extern int MDrv_Demod_DVBEXT_GetState(MS_U16 *dvb_ext_state);
extern int MDrv_Demod_DVBEXT_SetState(MS_U16 dvb_ext_state);

static demod_ext_datatype_EN_DEVICE_DEMOD_TYPE demod_ext_CurrentDemodulator_Type = demod_ext_datatype_E_DEVICE_DEMOD_DVB_T;
static MS_BOOL sg_IsSPIEnabled = FALSE;

static MS_BOOL demod_ext_WriteReg_I2C(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbData);

    return TRUE;
}

static MS_BOOL demod_ext_ReadReg_I2C(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_U8 u8MsbData[5];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 5, u8MsbData);
    MDrv_SW_IIC_Read(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbData);

    return TRUE;
}

static MS_BOOL demod_ext_I2C_CH_Reset(MS_U8 ch_num)
{
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    data[0] = 0x53;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 5, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, data);

    return TRUE;
}

static MS_BOOL demod_ext_Turn_Off_ALL_Pad_In(MS_BOOL b_en)
{
    MS_BOOL bRet = true;
    MS_U8   u8_reg = 0;

    // Turn off all-pad-in function
    // [8:8] 1: all pad turn off except i2c.
    demod_ext_ReadReg_I2C(0x0900+(0x28)*2+1, &u8_reg);

    if (b_en == TRUE)
        u8_reg &= (0xff-0x01);
    else
        u8_reg |= 0x01;

    demod_ext_WriteReg_I2C(0x0900+(0x28)*2+1, u8_reg);

    return bRet;
}

static MS_BOOL demod_ext_EnableSPIPad(MS_BOOL en)
{
    MS_U16 u16tmp = 0;
    MS_BOOL status = TRUE;

    if (en)
    {
        // TS2_PAD for MSPI >>>>>
        u16tmp = MDrv_STR_Read2Byte(0x101e00, 0x54);
        MDrv_STR_Write2Byte(0x101e00, 0x54, (u16tmp|0x30));
        // <<<<< TS2_PAD for MSPI

        demod_ext_WriteReg_I2C(0x0976, 0x02); // TS2_PAD for SSPI
        sg_IsSPIEnabled = TRUE;
    }
    else
    {
        u16tmp = MDrv_STR_Read2Byte(0x101e00, 0x54);
        MDrv_STR_Write2Byte(0x101e00, 0x54, ((u16tmp&~0x30)|0x10));

        demod_ext_WriteReg_I2C(0x0976, 0x01);
        sg_IsSPIEnabled = FALSE;
    }

    return status;
}

static MS_BOOL demod_ext_SS_RIU_Write16(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL ret = TRUE;
    MS_U8 Wdata[5];

    Wdata[0] = RIU_W_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = (MS_U8)(u16Data&0x00ff);
    Wdata[4] = (MS_U8)((u16Data>>8)&0x00ff);

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);

    // send write address & data
    MDrv_MSPI_Write(Wdata,5);

    MDrv_MSPI_SlaveEnable(FALSE);

    return ret;
}

static MS_BOOL demod_ext_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL ret = true;

    if (sg_IsSPIEnabled)
    {
        ret &= demod_ext_SS_RIU_Write16(u16Addr, u16Data);
    }
    else
    {
        ret &= demod_ext_WriteReg_I2C(u16Addr, (MS_U8)u16Data&0x00ff);
        ret &= demod_ext_WriteReg_I2C(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);
    }
    return ret;
}

static MS_BOOL demod_ext_SS_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata)
{
    MS_BOOL bRet = true;
    MS_U8 Rdata[5];

    Rdata[0] = RIU_R1T_CMD;
    Rdata[1] = u16Addr & 0xFF;
    Rdata[2] = (u16Addr >> 8) & 0xFF;
    Rdata[3] = 0x00;

    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(Rdata,4);
    // read operation
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}

static MS_BOOL demod_ext_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8MsbData[5];

    if (sg_IsSPIEnabled)
    {
        bRet = demod_ext_SS_RIU_Read8(u16Addr, pu8Data);
    }
    else
    {
        demod_ext_ReadReg_I2C(u16Addr, pu8Data);
    }

    return bRet;
}

static MS_BOOL demod_ext_DTV_DVB_HW_init(void)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;

    // ASIC INIT for Windermere DVB-T2
    {
    // -------------------------------------------------------------------
    // Initialize DMD_ANA_MISC
    // -------------------------------------------------------------------
    // [0]reg_tst_ldo25i
    // [1]reg_tst_ldo25q
    // [5:4]reg_tst_ldo25i_selfb
    // [7:6]reg_tst_ldo25q_selfb
    // [8]reg_pd_dm2p5ldoi = 1'b0
    // [9]reg_pd_dm2p5ldoq = 1'b0
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x4f)*2, 0x0000);

    // [0]reg_tst_ldo11_clk
    // [1]reg_tst_ldo26
    // [2]reg_tst_ldo11_cmp
    // [3]reg_pd_dm1p1ldo_clk = 1'b0
    // [4]reg_pd_dm1p1ldo_cmp = 1'b0
    // [6]reg_tst_ldo26_selfb
    // [7]reg_pd_dm2p6ldo = 1'b0
    // [9:8]reg_tst_ldo11_cmp_selfb
    // [11:10]reg_tst_ldo11_clk_selfb
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x4e)*2, 0x0000);

    // [1:0]reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
    // [15:8]reg_mpll_loop_div_second      feedback divider, div by binary data number
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x33)*2, 0x1201);

    // [2:0]reg_mpll_ictrl    charge pump current control
    // [3]reg_mpll_in_sel    1.8V or 3.3V reference clock domain select (1'b0=0==>3.3 V reference clock domain)
    // [4]reg_mpll_xtal2adc_sel    select the XTAL clock bypass to MPLL_ADC_CLK
    // [5]reg_mpll_xtal2next_pll_sel  crystal clock bypass to next PLL select
    // [6]reg_mpll_vco_offset	    set VCO initial offset frequency
    // [7]reg_mpll_pd    gated reference clock and power down PLL analog_3v: 1=power down
    // [8]reg_xtal_en    XTAL enable register; 1: enable
    // [10:9]reg_xtal_sel    XTAL driven strength select.
    // [11]reg_mpll_porst    MPLL input  power on reset, connect to reg as MPLL_RESET
    // [12]reg_mpll_reset    PLL software reset; 1:reset
    // [13]reg_pd_dmpll_clk    XTAL to MPLL clock reference power down
    // [14]reg_pd_3p3_1    XTAL to CLK_24M_3P3_1 power down
    // [15]reg_pd_3p3_2    XTAL to CLK_24M_3P3_2 power down
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x35)*2, 0x1803);
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x35)*2, 0x0003);


    // [0]reg_mpll_clk_dp_pddummy
    // [1]reg_adc_clk_pdADC output clock power down
    // [2]reg_mpll_div2_pdMPLL_DIV2 power down
    // [3]reg_mpll_div3_pdMPLL_DIV3 power down
    // [4]reg_mpll_div4_pdMPLL_DIV4 power down
    // [5]reg_mpll_div8_pdMPLL_DIV8 power down
    // [6]reg_mpll_div10_pdMPLL_DIV10 power down
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h2400);  // divide ADC clock to 24Mhz = 24*36/36
          bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x30)*2, 0x2400);

    // $display("--------------------------------------");
    // $display("Initialize ADC I/Q");
    // $display("--------------------------------------");

    // [0]Q channel ADC power down
    // [1]I channel ADC power down
    // [2]Q channel clamp enable. 0:enable, 1:disable
    // [3]I channel clamp enable. 0:enable, 1:disable
    // [6:4]    I channel input mux control;
    //3'b000=I channel ADC calibration mode input
    //3'b001=VIF signal from VIFPGA
    //3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //all the other combination are only for test mode, don't use without understanding.
    // [10:8]   Q channel input mux control;
    //3'b000=Q channel ADC calibration mode input
    //3'b001=VIF signal from VIFPGA 3'b010 = SSIF signal from PAD_SIFP(M)
    //3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //all the other combination are only for test mode, don't use without understanding.
    // [12]ADC I,Q swap enable; 1: swap
    // [13]ADC clock out select; 1: ADC_CLKQ
    // [14]ADC linear calibration bypass enable; 1:enable
    // [15]ADC internal 1.2v regulator control always 0 in T3
        bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x01)*2, 0x0440);

    // [2:0]reg_imuxs_s
    // [6:4]reg_qmuxs_s
    // [9:8]reg_iclpstr_s
    // [13:12]reg_qclpstr_s
        bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x45)*2, 0x0000);

    // [0]Channel I ADC power down: 1=power dwon
    // [1]Channel Q ADC power down: 1=power dwon
    // [2]power down clamp buffer for test mode
    // [3]change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
        bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x0c)*2, 0x0002);

    // [0]reg_linear_cal_start_q
    // [1]reg_linear_cal_mode_q
    // [2]reg_linear_cal_en_q
    // [3]reg_linear_cal_code0_oren_q
    // [6:4]reg_linear_cal_status_sel_q
    // [7]reg_pwdn_vcalbuf
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x0f)*2, 0x0000);

    // [3:0]clamp voltage control
    //          3'b000 = 0.7v
    //          3'b001 = 0.75v
    //          3'b010 = 0.5v
    //          3'b011 = 0.4v
    //          3'b100 = 0.8v
    //          3'b101 = 0.9v
    //          3'b110 = 0.65v
    //          3'b111 = 0.60v
    // [4]REFERENCE power down
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x20)*2, 0x0000);

    // Set ADC gain is 1
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x0b)*2, 0x0909);

    // Disable ADC Sign bit
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x2e)*2, 0x0000);

    // ADC I channel offset
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x2a)*2, 0x0c00);

    // ADC Q channel offset
      bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x2b)*2, 0x0c00);

    // [2:0]reg_acl_ref
    // [5:4]reg_acl_isel
    // [8]reg_xtal_pm_isel
    // [9]reg_bond_mode
    // [10]reg_clk_bond_mode
    // [11]reg_clk_usb_3p3_en
    // [13:12]reg_iq_ctrl	= 2'd1
    bRet &= demod_ext_WriteReg2bytes(0x0A00+(0x19)*2, 0x1e00);

    // [ 4:0]reg_ckg_bist[4:0]
    // [11:8]reg_ckg_adcd_d2[3:0]
    bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1c)*2, 0x0000);

    // [ 4:0]reg_ckg_dvbtm_sram_t11x_t22x[4:0]
    // [12:8]reg_ckg_dvbtm_sram_t11x_t24x[4:0]
    bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1e)*2, 0x0000);


    // -------------------------------------------------------------------
    // Release clock gating
    // -------------------------------------------------------------------

    // [0]reg_xtal_en
    // [9:8]reg_clk_pd_iic
    // [10]reg_clk_pd_all
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x09)*2, 0x0101);

    // [3:0]reg_ckg_adcd
    // [7:4]reg_ckg_sadc
    // [11:8]reg_ckg_iicm
    // [13:12]reg_ckg_sbus
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x0a)*2, 0x0000);

    // [5:0]reg_ckg_mcu
    // [6]reg_ckg_live
    // [11:8]reg_ckg_inner
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x0b)*2, 0x0030);
    //set MCU ckg to 108MHz by jason
    // reg_ckg_mcu[4:2] = 0x0
    // reg_ckg_mcu[5] = 0x0

    // @0x0910
    // [3:0]reg_ckg_dvbtm_adc
    // [6:4]reg_ckg_dvbt_inner1x
    // [10:8]reg_ckg_dvbt_inner2x
    // [14:12]reg_ckg_dvbt_inner4x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x10)*2, 0x1110);

    // @0x0911
    // [2:0]reg_ckg_dvbt_outer1x
    // [6:4]reg_ckg_dvbt_outer2x
    // [11:8]reg_ckg_dvbtc_outer2x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x11)*2, 0x0111);

    // @0x0912
    // [3:0]reg_ckg_dvbtm_ts
    // [4]reg_dvbtm_ts_out_mode
    // [5]reg_dvbtm_ts_clk_pol
    // [15:8]reg_dvbtm_ts_clk_divnum
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x12)*2, 0x1418);

    // @0x0913
    // [5:0]reg_ckg_spi
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x13)*2, 0x0020);

    // @0x0914
    // [12:8]reg_ckg_dvbtm_sram_t1o2x_t22x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x14)*2, 0x0000);

    // @0x0915
    // [3:0]reg_ckg_dvbc_inner
    // [6:4]reg_ckg_dvbc_eq
    // [10:8]reg_ckg_dvbc_eq8x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x15)*2, 0x0111);

    // @0x0916
    // [8:4]reg_ckg_dvbtm_sram_adc_t22x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x16)*2, 0x0001);

    // @0x0917
    // [4:0]reg_ckg_dvbtm_sram_t12x_t22x
    // [12:8]reg_ckg_dvbtm_sram_t12x_t24x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x17)*2, 0x0000);

    // @0x0918
    // [4:0]reg_ckg_dvbtm_sram_t14x_t24x
    // [12:8]reg_ckg_dvbtm_ts_in
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x18)*2, 0x0400);

    // @0x0919
    // [2:0]reg_ckg_tdp_jl_inner1x
    // [6:4]reg_ckg_tdp_jl_inner4x
    // [15:8]reg_ckg_miu
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x19)*2, 0x3c00);

    // @0x091a
    // [6:4]reg_ckg_dvbt2_inner1x
    // [10:8]reg_ckg_dvbt2_inner2x
    // [14:12]reg_ckg_dvbt2_inner4x
     //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1a)*2, 0x0000);

    // @0x090e
    // [3:0]reg_ckg_dvbs2_ldpc_inner_sram
    // [7:4]reg_ckg_dvbs_viterbi_sram
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x0e)*2, 0x0000);

    // @0x091b
    // [2:0]reg_ckg_dvbt2_outer1x
    // [6:4]reg_ckg_dvbt2_outer2x
    // [10:8]reg_ckg_syn_miu
    // [14:12]reg_ckg_syn_ts
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1b)*2, 0x0000);

    // @0x091c
    // [4:0]reg_ckg_bist
    // [11:8]reg_ckg_adcd_d2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1c)*2, 0x0000);

    // @0x091d
    // [3:0]reg_ckg_dvbtm_adc_eq_1x
    // [7:4]reg_ckg_dvbtm_adc_eq_0p5x
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x1d)*2, 0x0000);

    // @0x0921
    // [2:0]reg_ckg_tdp_teq_inner1x
    // [14:12]reg_ckg_tdp_teq_inner2x
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x21)*2, 0x0000);

    // @0x0922
    // [3:0]reg_ckg_dvbt_t2_inner0p5x_dvbc_eq1x
    // [7:4]reg_ckg_dvbt_t2_inner2x_dvbc_eq4x
    // [11:8]reg_ckg_dvbt_t2_inner1x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x22)*2, 0x0000);

    // [1:0]reg_iicm_pad_sel
    // [4]reg_i2c_sbpm_en
    // [12:8]reg_i2c_sbpm_idle_num
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x08)*2, 0x0a01);

    // [8]reg_turn_off_pad
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x28)*2, 0x0000);

    // @0x0970
    // [3:0]reg_ckg_dvbt_inner2x_srd0p5x
    // [7:4]reg_ckg_dvbt2_inner2x_srd0p5x
    // [12:8]reg_ckg_dvbtm_sram_t1outer1x_t24x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x70)*2, 0x0000);

    // @0x0971
    // [4:0]reg_ckg_dvbtm_sram_t12x_t24x_srd1x
    // [12:8]reg_ckg_dvbtm_sram_t14x_t24x_srd1x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x71)*2, 0x0000);

    // @0x0972
    // [6:0]reg_ckg_dvbt2_s2_bch_out
    // [12:8]reg_ckg_dvbt2_outer2x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x72)*2, 0x0000);

    // @0x0973
    // [3:0]reg_ckg_dvbt2_inner4x_s2_inner
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x73)*2, 0x0000);

    // @0x0974
    // [4:0]reg_ckg_dvbtm_sram_t12x_t24x_s2inner
    // [12:8]reg_ckg_dvbtm_sram_t14x_t24x_s2inner
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x74)*2, 0x0000);

    // @0x0975
    // [4:0]reg_ckg_dvbtc_rs
    // [11:8]reg_ckg_dvbs_outer2x_dvbt_outer2x
    // [15:12]reg_ckg_dvbs_outer2x_dvbt_outer2x_miu
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x75)*2, 0xc101);

    // @0x0976
    // [4:0]reg_ckg_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x76)*2, 0x000c);

    // @0x0977
    // [3:0]reg_ckg_dvbt2_inner4x_dvbtc_rs
    // [8:4]reg_ckg_dvbtm_sram_adc_t22x_dvbtc_rs
    //DVBT2
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x77)*2, 0x0000);

//    $display("--------------------------------------");
//    $display("Initialize Transport Stream synthesizer and APLL");
//    $display("--------------------------------------");

    // [15:0]reg_synth_set[15: 0]
    // [ 7:0]reg_synth_set[23:16]
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x51)*2, 0x0000);
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x52)*2, 0x0040);


    // [0]reg_synth_reset
    // [1]reg_synth_ssc_en
    // [2]reg_synth_ssc_mode
    // [4]reg_synth_sld
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x50)*2, 0x0010);

    // [1:0]reg_apll_loop_div_first
    // [15:8]reg_apll_loop_div_second
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x57)*2, 0x0000);

    // [0]reg_apll_pd
    // [1]reg_apll_reset
    // [2]reg_apll_porst
    // [3]reg_apll_vco_offset
    // [4]reg_apll_en_ts
    // [5]reg_apll_endcc
    // [6]reg_apll_clkin_sel
    // [8]reg_apll_ts_mode
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x55)*2, 0x0100);
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x55)*2, 0x0110);

    // [16:0]reg_apll_test
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x59)*2, 0x0000);

    // 0x0920
    // [3:0]reg_ckg_ts_apll_div[2:0]
      bRet &= demod_ext_WriteReg2bytes(0x0900+(0x20)*2, 0x0004);


    // -------------------------------------------------------------------
    // initialize MIU
    // -------------------------------------------------------------------
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);


// set frequence 133MHz
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x11)*2, 0x60cc);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x11)*2, 0x00cc);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x11)*2, 0x004c);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x10)*2, 0x33f8);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x12)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x13)*2, 0x0000);
    // yihao 20130925 for new apll model
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x16)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x1c)*2, 0x00b0);

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x01)*2, 0x8100);
    // cke: [0]
    // reg_self_refresh: [1]
    // reg_dynamic_cke: [2]
    // reg_dynamic_ck_odt: [3]
    // reg_dram_bus: [5:4] 00: 16b, 01: 32b, 10: 64b
    // reg_dram_type: [7:6] 00: sdr, 01: ddr, 10: ddr2
    // reg_4ba: [8]    0: 2bk,  1: 4bk
    // reg_col_size: [10:9]
    // reg_cke_oenz: [12]
    // reg_dq_oenz: [13]
    // reg_adr_oenz: [14]
    // reg_cs_z: [15]
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x01)*2, 0xe100);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x01)*2, 0x8100);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x02)*2, 0x0371);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x03)*2, 0x0030);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x04)*2, 0x33c9);
    // reg_tRAS                      : [3:0]        9
    // reg_tRC                       : [7:4]        c
    // reg_tRCD                      : [11:8]       3
    // reg_tRP                       : [15:12]      3
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x05)*2, 0x4232);
    // reg_tRRD                      : [3:0]         2
    // tWR                           : [7:4]         3
    // reg_tMRD                      : [11:8]        2
    // reg_tRTP                       : [15:12]    4
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x06)*2, 0x5532);
    // reg_w2r_dly(tWTR)             : [3:0]         2
    // reg_w2r_oen_dly               : [7:4]         3
    // reg_r2w_dly(tRTW)             : [11:8]        5
    // reg_r2w_oen_dly               : [15:12]       5
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x07)*2, 0x400c);
    // tRFC                          : [5:0]         c
    // reg_tRAS[4]                   : [6]           0
    // reg_tRC[4]                    : [7]           0
    // reg_write_latency             : [10:8]        0
    // reg_tCCD                      : [15:14]       1
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0e)*2, 0x1800);

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x23)*2, 0x7ffe);

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x20)*2, 0xc001);

    //delay 1
    udelay(1000);

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x0f)*2, 0x0c00);

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x01)*2, 0x010d);


// $display("--------Initial DRAM start here!!!");
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x00)*2, 0x0001);


    // wait(test_chip_top.dut.i_dig_top.miu_inst.miu_reg_0.init_done);
    // while( (getValue(0x1200) & 0x8000)!= 0x8000);
    u8_timeout = 0;
    bRet &= demod_ext_ReadReg(0x1201, &u8_tmp);

    while( (u8_tmp&0x80) != 0x80)
    {
        if(u8_timeout++>200)
        {
            return FALSE;
        }

        // 10us delay
        udelay(10);
        bRet &= demod_ext_ReadReg(0x1201, &u8_tmp);
    }

    // $display("--------Initial Done");
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x08)*2, 0x0001);


    // $display("-------------------------");
    // $display("-- miu self test start --");
    // $display("-------------------------");
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x70)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x71)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x72)*2, 0x0010);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x74)*2, 0x5aa5);
    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x70)*2, 0x0001);

    // #10000;
    //delay 1
    udelay(1000);

    //wait test_finish
    u8_timeout = 0;
    bRet &= demod_ext_ReadReg(0x12E1, &u8_tmp);
    while( (u8_tmp&0x80) != 0x80)
    {
        if(u8_timeout++>200)
        {
            return FALSE;
        }
        // 10us delay
        udelay(10);
        bRet &= demod_ext_ReadReg(0x12E1, &u8_tmp);
    }

     // #10000;
     //delay 1
     udelay(1000);

    // MIU self test FAIL let program stuck in this while loop
    u8_timeout = 0;
    bRet &= demod_ext_ReadReg(0x12E1, &u8_tmp);
    while( (u8_tmp&0x40) != 0x00)
    {
        if(u8_timeout++>200)
        {
            return FALSE;
        }
        // 10us delay
        udelay(10);
        bRet &= demod_ext_ReadReg(0x12E1, &u8_tmp);
    }

    bRet &= demod_ext_WriteReg2bytes(0x1200+(0x23)*2, 0x0000);

    // -------------------------------------------------------------------
    // initialize MIU  finish
    // -------------------------------------------------------------------

    // -------------------------------------------------------------------
    //  Turn on pads
    // -------------------------------------------------------------------

    // ------Turn off all pad in
    // [0] reg_set_pad_low
    // [1] reg_set_pad_high
    // [2] reg_set_i2cs_pad_low
    // [3] reg_set_i2cs_pad_high
    // [8] reg_turn_off_pad
     bRet &= demod_ext_WriteReg2bytes(0x0900+(0x28)*2, 0x0000);

    // ------I2CM pad on
    // [1:0]    reg_iicm_pad_sel[1:0]1:iicm enable 2:UART enable
    // [4]	    reg_i2c_sbpm_en		1: enable I2CS bypass to I2CM function
    // [12:8]   reg_i2c_sbpm_idle_num[4:0]a: default
     bRet &= demod_ext_WriteReg2bytes(0x0900+(0x08)*2, 0x0a01);

    // ------Transport Stream pad on (except TS ERR pad)
    // [15:0]   reg_en_ts_pad[15:0]	0x00ff:normal TS location 0xff00:reverse TS location
     bRet &= demod_ext_WriteReg2bytes(0x0900+(0x2d)*2, 0x00ff);

    // ------Transport Stream pad on (TS ERR pad)
    // [0]	    reg_en_ts_err_pad	1: enable
    // [4]	    reg_ts_err_pol	1: inverse 0:normal
     bRet &= demod_ext_WriteReg2bytes(0x0900+(0x2e)*2, 0x0001);

    // ------AGC pad on
    // [0] reg_ifagc_t_enable
    // [1] reg_ifagc_t_odmode
    // [2] reg_ifagc_t_data_sel
    // [4] reg_ifagc_t_force_enable
    // [5] reg_ifagc_t_force_value
    // [8] reg_ifagc_s_enable
    // [9] reg_ifagc_s_odmode
    // [10] reg_ifagc_s_data_sel
    // [12] reg_ifagc_s_force_enable
    // [13] reg_ifagc_s_force_value
    bRet &= demod_ext_WriteReg2bytes(0x0a00+(0x18)*2, 0x0001);
    }

    return bRet;
}

static MS_BOOL demod_ext_SS_RIU_Write8(MS_U16 u16Addr, MS_U8 data)
{
    MS_BOOL bRet = true;
    MS_U8 Wdata[4];

    Wdata[0] = RIU_W1_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = data;

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);

    // send write address & data
    MDrv_MSPI_Write(Wdata,4);

    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}

static MS_BOOL demod_ext_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8MsbData[6];

    if (sg_IsSPIEnabled)
    {
        bRet = demod_ext_SS_RIU_Write8(u16Addr, u8Data);
    }
    else
    {
        demod_ext_WriteReg_I2C(u16Addr, u8Data);
    }

    return bRet;
}

static MS_BOOL demod_ext_SS_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = true;
    MS_U8 Wdata[5];

    Wdata[0] = MIU_W_CMD;
    Wdata[1] = u32Addr & 0xFF;
    Wdata[2] = (u32Addr >> 8) & 0xFF;
    Wdata[3] = (u32Addr >> 16)& 0xFF;
    Wdata[4] = (u32Addr >> 24);

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    // send data
    MDrv_MSPI_Write(pdata,u16Size);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}

static MS_BOOL demod_ext_SS_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = true;
    MS_U8 Rdata[SPI_DEVICE_BUFFER_SIZE];
    MS_U16 dataLen, i, j=0;

    do
    {
        dataLen = (u16Size>16?16:u16Size);//(len>24?24:len);

        Rdata[0] = MIU_R_CMD;
        Rdata[1] = u32Addr & 0xFF;
        Rdata[2] = (u32Addr >> 8) & 0xFF;
        Rdata[3] = (u32Addr >> 16)& 0xFF;
        Rdata[4] = (u32Addr >> 24);
        Rdata[5] = dataLen+1;

        // send read command to read data
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,6);
        MDrv_MSPI_SlaveEnable(FALSE);

        // read operation
        Rdata[0] = MIU_ST_CMD;
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,1);

        MDrv_MSPI_Read(Rdata, dataLen+1);
        MDrv_MSPI_SlaveEnable(FALSE);

        if(Rdata[0] != 0x0A)
        {
            printk("MDrv_SS_MIU_Reads fail, status=0x%x\n", Rdata[0]);
            return false;
        }

        for (i=1; i<dataLen+1; i++, j++)
        {
            pdata[j] = Rdata[i];
        }

        u16Size -= dataLen;
        u32Addr += dataLen;

    }while(u16Size);

    return bRet;
}

static MS_BOOL demod_ext_WriteRegs_I2C(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;

    u8MsbDataValue[0] = 0x10;
    u8MsbDataValue[1] = 0x00;
    u8MsbDataValue[2] = 0x00;
    u8MsbDataValue[3] = (u16Addr >> 8) &0xff;
    u8MsbDataValue[4] = u16Addr &0xff;

    for(idx = 0; idx < data_size ; idx++)
    {
        u8MsbDataValue[5+idx] = u8pData[idx];
    }

    u8MsbDataValue[0] = 0x35;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
    MDrv_SW_IIC_Write(DEMOD_IIC_CHANNEL_ID, DEMOD_IIC_SLAVE_ADDR, 0, NULL, 1, u8MsbDataValue);

    return bRet;
}

static MS_BOOL demod_ext_Load2Sdram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sdram_win_offset_base)
{
    MS_BOOL bRet = true;

    // mask miu access of fdp, tdi, djb
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2 + 1, 0x0f);
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2, 0xf0);

    // 10us delay
    udelay(10);

    // Disable MCU
    bRet &= demod_ext_WriteReg(0x0b00+(0x19)*2, 0x03);

    if (sg_IsSPIEnabled)
    {
        MS_U32 u32Addr = 0;

        u32Addr = (MS_U32)sdram_win_offset_base * 0x1000; // unit: 4K
        bRet &= demod_ext_SS_MIU_Writes(u32Addr, u8_ptr, data_length);

#if (SDRAM_DATA_CHECK == 1)
        MS_U16 i = 0, j = 0, index = 0;
        MS_U8 buf[SPI_DEVICE_BUFFER_SIZE]={0};

        if((data_length % SPI_DEVICE_BUFFER_SIZE) == 0)
            index = data_length / SPI_DEVICE_BUFFER_SIZE;
        else
            index = data_length / SPI_DEVICE_BUFFER_SIZE + 1;

        for (i=0; i<index; i++)
        {
            memset(buf, 0x00, SPI_DEVICE_BUFFER_SIZE);

            bRet &= demod_ext_SS_MIU_Reads(u32Addr+SPI_DEVICE_BUFFER_SIZE*i, buf, SPI_DEVICE_BUFFER_SIZE);

            for (j=0; j<SPI_DEVICE_BUFFER_SIZE; j++)
            {
                if (buf[j] != u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j])
                {
                    printk("spi miu write&read fail! idx=%x, y=0x%x, x=0x%x\n", (SPI_DEVICE_BUFFER_SIZE*i+j), buf[i], u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j]);
                }
            }
        }
        printk("[msb1240]SDRAM data check SPI %x...Done\n", sdram_win_offset_base);
#endif
    }
    else
    {
        MS_U16  sdram_win_offset = sdram_win_offset_base;
        MS_U16  x_data_offset = 0;
        MS_U16  y_cir_addr = 0;

        if (demod_ext_I2C_CH_Reset(0) == FALSE)
        {
            printk(">>>MSB1240 CH0 Reset:Fail\n");
            return FALSE;
        }
        else
        {
            // set xData map upper and low bound for 64k DRAM window
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x63)*2, 0x3F24);

            // set xData map offset for 64k DRAM window, 64kbytes alignment
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x64)*2, 0x0000);

            // set xData map upper and low bound for 4k DRAM window
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x65)*2, 0x2014);

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            // enable xData map for DRAM
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x62)*2, 0x0007);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                if (y_cir_addr == 0x6000)
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                // max 0x200, error above.....

                if((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
                {
                    bRet &= demod_ext_WriteRegs_I2C(y_cir_addr, (u8_ptr + x_data_offset),LOAD_CODE_I2C_BLOCK_NUM);
                    y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                    x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
                }
                else
                {
                    bRet &= demod_ext_WriteRegs_I2C(y_cir_addr, (u8_ptr + x_data_offset),data_length - x_data_offset);
                    y_cir_addr += (data_length - x_data_offset);
                    x_data_offset += (data_length - x_data_offset);
                }
            }

#if (SDRAM_DATA_CHECK == 1)
            // beg data check.
            sdram_win_offset = sdram_win_offset_base;

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                MS_U8 u8_tmp;
                if (y_cir_addr == 0x6000)
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                bRet &= demod_ext_ReadReg(y_cir_addr++, &u8_tmp);
                if(u8_tmp != *(u8_ptr + x_data_offset++))
                {
                    printk("I2C miu write&read fail! idx=0x%x, y=0x%x, x=0x%x\n",y_cir_addr-1, u8_tmp, *(u8_ptr + x_data_offset-1));
                }
            }

            printk("[msb1240]SDRAM data check I2C %x...Done\n", sdram_win_offset_base);
            // end data check
#endif

            //  Release xData map for SDRAM
            bRet &= demod_ext_WriteReg2bytes(0x2B00+(0x62)*2, 0x0000);

            // Channel changed from CH 0x00 to CH 0x03
            if (demod_ext_I2C_CH_Reset(3) == FALSE)
            {
                printk(">>>MSB1240 CH3 Reset:Fail\n");
                return FALSE;
            }
        }
    }

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAM_Boot(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    MS_U32  u32_code_size = 0;

    // boot code
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_BOOT_START_ADDR;
    u32_code_size = MSB1240_BOOT_END_ADDR - MSB1240_BOOT_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_BOOT_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAM_dvbt2(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    MS_U32  u32_code_size = 0;

    // dvbt2_p2
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_DVBT2_P2_START_ADDR;
    u32_code_size = MSB1240_DVBT2_P2_END_ADDR - MSB1240_DVBT2_P2_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_DVBT2_P2_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    // dvbt2_p1
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_DVBT2_P1_START_ADDR;
    u32_code_size = MSB1240_DVBT2_P1_END_ADDR - MSB1240_DVBT2_P1_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_DVBT2_P1_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAM_dvbt(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    MS_U32  u32_code_size = 0;

    // dvbt code
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_DVBT_START_ADDR;
    u32_code_size = MSB1240_DVBT_END_ADDR - MSB1240_DVBT_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_DVBT_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAM_dvbs(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    MS_U32  u32_code_size = 0;

    // dvbs code
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_DVBS_START_ADDR;
    u32_code_size = MSB1240_DVBS_END_ADDR - MSB1240_DVBS_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_DVBS_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAM_dvbc(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    MS_U32  u32_code_size = 0;

    // dvbc code
    data_ptr = (MS_U8 *)_demod_ext_fw_dat;
    data_ptr = data_ptr + MSB1240_DVBC_START_ADDR;
    u32_code_size = MSB1240_DVBC_END_ADDR - MSB1240_DVBC_START_ADDR + 1;
    code_size = (MS_U16)u32_code_size;
    win_offset = MSB1240_DVBC_WINDOWS_OFFSET;
    bRet &= demod_ext_Load2Sdram(data_ptr,code_size,win_offset);

    return bRet;
}

static MS_BOOL demod_ext_LoadDspCodeToSDRAMAll(void)
{
    MS_BOOL bRet = true;

    // boot code
    bRet &= demod_ext_LoadDspCodeToSDRAM_Boot();

    // dvbt2
    bRet &= demod_ext_LoadDspCodeToSDRAM_dvbt2();

    // dvbt
    bRet &= demod_ext_LoadDspCodeToSDRAM_dvbt();

    // dvbs
    bRet &= demod_ext_LoadDspCodeToSDRAM_dvbs();

    // dvbtc
    bRet &= demod_ext_LoadDspCodeToSDRAM_dvbc();

    return bRet;
}

static MS_BOOL demod_ext_MSB1240_MEM_switch(MS_U8 mem_type)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U16 timeout = 0;
    MS_U8 u8Data = 0;

    switch(demod_ext_CurrentDemodulator_Type)
    {
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T2:
            u8Data=1;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T:
            u8Data=2;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_C:
            u8Data=3;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_S:
            u8Data=4;
            break;
        default:
            u8Data=2;
            return FALSE;
    }

    if(mem_type == 1)
    {
        // Enable SRAM+SDRAM memory map

        // SRAM_START_ADDR 0x0000
        bRet &= demod_ext_WriteReg2bytes(0x1000,0x0000);
        bRet &= demod_ext_WriteReg2bytes(0x1004,0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= demod_ext_WriteReg2bytes(0x1002,0x0000);
        bRet &= demod_ext_WriteReg2bytes(0x1006,0x7FFF);
        if (u8Data == 4)
        {
            bRet &= demod_ext_WriteReg(0x2B80,0x12);
            // DRAM_START_ADDR 1M+0x2*0x1000+0x8000
            bRet &= demod_ext_WriteReg2bytes(0x1008,0x0000);
            bRet &= demod_ext_WriteReg2bytes(0x100C,0x8000);

            // DRAM_END_ADDR   1M+0x2*0x1000+0xFFFF
            bRet &= demod_ext_WriteReg2bytes(0x100A,0x0000);
            bRet &= demod_ext_WriteReg2bytes(0x100E,0xFFFF);
        }
        else
        {
            bRet &= demod_ext_WriteReg(0x2B80,0x10);

            // DRAM_START_ADDR 0x8000
            bRet &= demod_ext_WriteReg2bytes(0x1008,0x0000);
            bRet &= demod_ext_WriteReg2bytes(0x100C,0x8000);


            // DRAM_END_ADDR    0xFFFF
            bRet &= demod_ext_WriteReg2bytes(0x100A,0x0000);
            bRet &= demod_ext_WriteReg2bytes(0x100E,0xFFFF);
        }
        // Enable SRAM&SDRAM memory map
        bRet &= demod_ext_WriteReg(0x1018,0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= demod_ext_ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printk("@msb1240, D+S memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x05);
    }
    else if(mem_type == 0)
    {
        // Enable SDRAM memory map
        // Run bootloader from SDRAM 1M

        bRet &= demod_ext_WriteReg(0x2B80,0x10);

        // DRAM_START_ADDR 0x0000
        bRet &= demod_ext_WriteReg2bytes(0x1008,0x0000);
        bRet &= demod_ext_WriteReg2bytes(0x100C,0x0000);

        // DRAM_END_ADDR    0x7FFF
        bRet &= demod_ext_WriteReg2bytes(0x100A,0x0000);
        bRet &= demod_ext_WriteReg2bytes(0x100E,0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= demod_ext_WriteReg(0x1018,0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= demod_ext_ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printk("@msb1240, D memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x04);
    }
    else
    {
       printk("@msb1240, invalid mem type mapping.\n");
       return FALSE;
    }

    return bRet;
}

static MS_BOOL demod_ext_LoadSdram2Sram(MS_U8 CodeNum)
{
    MS_BOOL bRet = true;
    MS_U8   u8_data = 0;
    MS_U8   u8_timeout = 0xFF;

    // mask miu access of fdp, tdi, djb
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2,0xf0);

    // 10us delay
    udelay(10);

    // Disable MCU
    bRet &= demod_ext_WriteReg(0x0b00+(0x19)*2, 0x03);

    // Use GDMA move code from SDRAM to SRAM
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x00)*2, 0x4254);     // rst
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x00)*2, 0x4257);    // cfg & trig

   switch(CodeNum)
   {
        case 0x02: //DVB-T2 @001000h ~ 008FFFh
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x02)*2, 0x0011); // start from 1M+64k
            break;

        case 0x04: //DVB-T @010000h ~ 017FFFh
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x01)*2, 0x8000); // Set src_addr
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x02)*2, 0x0011); // start from 1M+96k
            break;

        case 0x08:
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x02)*2, 0x0013); // start from 1M+192k
            break;

        case 0x10:
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= demod_ext_WriteReg2bytes(0x0300+(0x02)*2, 0x0012); // start from 1M+128k
            break;
        default:
            bRet &= FALSE;
            break;
   }

    // Set dst_addr
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x03)*2, 0x0000);
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x04)*2, 0x0000);

    // Set data_size
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x05)*2, 0x8000);
    bRet &= demod_ext_WriteReg2bytes(0x0300+(0x06)*2, 0x0000);

    bRet &= demod_ext_WriteReg(0x0300+(0x07)*2, 0x01); //start GDMA

    // Wait for GDMA
    do
    {
      udelay(10);
      bRet &= demod_ext_ReadReg(0x0300+(0x08)*2, &u8_data);
      u8_timeout--;
    }while(((u8_data&0x01) != 0x01) && (u8_timeout != 0x00));

    if(u8_data != 0x01)
    {
        printk("[msb1240]LoadSdram2Sram, GDMA move code fail!!\n");
        return false;
    }

    bRet &= demod_ext_MSB1240_MEM_switch(1);

    return bRet;
}

static MS_BOOL demod_ext_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8     status = true;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;

    if (sg_IsSPIEnabled)
    {
        status &= demod_ext_SS_RIU_Write8(REG_MB_DATA, u8Data);
        status &= demod_ext_SS_RIU_Write8(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
        status &= demod_ext_SS_RIU_Write8(REG_MB_ADDR_L,  (MS_U8)(u16Addr));
        status &= demod_ext_SS_RIU_Write8(REG_MB_CNTL, 0x04);

        do
        {
            status &= demod_ext_SS_RIU_Read8(REG_MB_CNTL, &cntl);
            if (cntr++ > MSB1240_MB_CNT_TH)
            {
                return FALSE;
            }
        }
        while(cntl != 0xff);
    }
    else
    {
        status &= demod_ext_WriteReg(REG_MB_DATA, u8Data);
        status &= demod_ext_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
        status &= demod_ext_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
        status &= demod_ext_WriteReg(REG_MB_CNTL, 0x04);

        do
        {
            status &= demod_ext_ReadReg(REG_MB_CNTL, &cntl);
            if (cntr++ > MSB1240_MB_CNT_TH)
            {
                return FALSE;
            }
        }
        while(cntl != 0xff);
    }

    return status;
}

static MS_BOOL demod_ext_ReadDspReg(MS_U16 u16Addr, MS_U8* pData)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;

    if (sg_IsSPIEnabled)
    {
        status &= demod_ext_SS_RIU_Write8(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
        status &= demod_ext_SS_RIU_Write8(REG_MB_ADDR_L, (MS_U8)(u16Addr));
        status &= demod_ext_SS_RIU_Write8(REG_MB_CNTL, 0x03);

        do
        {
            status &= demod_ext_SS_RIU_Read8(REG_MB_CNTL, &cntl);
            if (cntr++ > MSB1240_MB_CNT_TH)
            {
                printk("MSB1240_MB_READ_FAILURE\n");
                return FALSE;
            }
        }
        while(cntl != 0xff);

        status &= demod_ext_SS_RIU_Read8(REG_MB_DATA, pData);
    }
    else
    {
        status &= demod_ext_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
        status &= demod_ext_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
        status &= demod_ext_WriteReg(REG_MB_CNTL, 0x03);

        do
        {
            status &= demod_ext_ReadReg(REG_MB_CNTL, &cntl);
            if (cntr++ > MSB1240_MB_CNT_TH)
            {
                printk("MSB1240_MB_READ_FAILURE\n");
                return FALSE;
            }
        }
        while(cntl != 0xff);
    }

    status &= demod_ext_ReadReg(REG_MB_DATA, pData);
    return status;
}

static MS_BOOL demod_ext_DTV_DVBT2_DSPReg_Init(void)
{

    if( demod_ext_WriteDspReg((MS_U8)E_T2_BW, T2_BW_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_FC_L, T2_FC_L_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_FC_H, T2_FC_H_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_TS_SERIAL, T2_TS_SERIAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_TS_OUT_INV, T2_TS_CLK_INV) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)E_T2_TS_ERR_POL, T2_TS_ERR_POL_VAL) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U16)E_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL) != TRUE)
    {
      printk("T2 dsp reg init NG\n"); return FALSE;
    }

    if( demod_ext_WriteDspReg((MS_U16)E_T2_SPREAD_SPAN, (MS_U16) T2_TS_SPREAD_SPAN) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }

    if( demod_ext_WriteDspReg((MS_U16)E_T2_SPREAD_STEP, (MS_U16) T2_TS_SPREAD_STEP_SIZE) != TRUE)
    {
        printk("T2 dsp reg init NG\n"); return FALSE;
    }

    printk("T2 dsp reg init ok\n");

    return TRUE;
}

static MS_BOOL demod_ext_DTV_DVBT_DSPReg_Init(void)
{
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_BW, T_BW) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_FC_L, T_FC_L) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_FC_H, T_FC_H) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_IQ_SWAP, T_IQ_SWAP) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_TS_SERIAL, T_SERIAL_TS_VAL) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_TS_CLK_RATE, T_TS_CLK_SEL) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_TS_OUT_INV, T_TS_OUT_INV) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U8)T_CONFIG_TS_DATA_SWAP, T_TS_DATA_SWAP) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }
    if( demod_ext_WriteDspReg((MS_U16)T_CONFIG_IF_INV_PWM_OUT_EN, T_IF_INV_PWM_OUT_EN) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }

    if( demod_ext_WriteDspReg((MS_U16)T_CONFIG_SPREAD_SPAN, (MS_U16) T_TS_SPREAD_SPAN) != TRUE)
    {
        printk("T dsp reg init NG\n"); return FALSE;
    }

    if( demod_ext_WriteDspReg((MS_U16)T_CONFIG_SPREAD_STEP, (MS_U16) T_TS_SPREAD_STEP_SIZE) != TRUE)
    {
         printk("T dsp reg init NG\n"); return FALSE;
    }

    printk("dvbt dsp reg init ok\n");

    return TRUE;
}

static MS_BOOL demod_ext_DTV_DVBC_DSPReg_Init(void)
{
    MS_U8    idx = 0;

    for (idx = 0; idx<sizeof(demod_ext_DVBC_DSPREG_TABLE); idx++)
    {
        if(demod_ext_WriteDspReg(idx + 0x20, demod_ext_DVBC_DSPREG_TABLE[idx])!=TRUE)
        {
            printk("dvbc dsp reg init NG\n");
            return FALSE;
        }
    }

    if( demod_ext_WriteDspReg((MS_U16)C_config_spread_span, (MS_U16) C_TS_SPREAD_SPAN) != TRUE)
    {
        printk("C dsp reg init NG\n"); return FALSE;
    }

    if( demod_ext_WriteDspReg((MS_U16)C_config_spread_step, (MS_U16) C_TS_SPREAD_STEP_SIZE) != TRUE)
    {
        printk("C dsp reg init NG\n"); return FALSE;
    }

    return TRUE;
}

static MS_BOOL demod_ext_LoadDSPCode(void)
{
    MS_U8        FWversionH = 0xFF;
    MS_U8        FWversionL = 0xFF;

    MS_U8         u8Data = 0;
    MS_BOOL       bRet = true;
    MS_U8 u8FirmwareType = MSB1240_DVBT;

    switch(demod_ext_CurrentDemodulator_Type)
    {
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T2:
            u8Data=1;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T:
            u8Data=2;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_C:
            u8Data=3;
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_S:
            u8Data=4;
            break;
        default:
            u8Data=2;
            return FALSE;
    }

    // mask miu access for all and mcu
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2 + 1,0x7f);
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2,0xfe);
    // 10us delay
    udelay(10);
    // Disable MCU
    bRet &= demod_ext_WriteReg(0x0b00+(0x19)*2, 0x03);

    switch (u8Data)
    {
        case 1:
            u8FirmwareType = MSB1240_DVBT2;
            break;
        case 2:
        default:
            u8FirmwareType = MSB1240_DVBT;
            break;
        case 3:
            u8FirmwareType = MSB1240_DVBC;
            break;
        case 4:
            u8FirmwareType = MSB1240_DVBS;
            break;
    }

    bRet &= demod_ext_LoadSdram2Sram(u8FirmwareType);

    // enable miu access of mcu gdma
    bRet &= demod_ext_WriteReg(0x1200+(0x23)*2,0xf0);
    // 10us delay
    udelay(10);

    // Enable MCU
    bRet &= demod_ext_WriteReg(0x0b00+(0x19)*2, 0x00);

    switch(demod_ext_CurrentDemodulator_Type)
    {
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T2:
            demod_ext_DTV_DVBT2_DSPReg_Init();
            break;
        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_T:
            demod_ext_DTV_DVBT_DSPReg_Init();
            break;

        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_C:
            demod_ext_DTV_DVBC_DSPReg_Init();
            break;

        case demod_ext_datatype_E_DEVICE_DEMOD_DVB_S:
            demod_ext_WriteDspReg((MS_U16)E_S2_TS_serial_mode, S_TS_Output_VAL);

            //Configure AGC IFAGC_polarity	0:positive	1:negative
            demod_ext_WriteDspReg((MS_U16)E_S2_IFAGC_POLARITY, 0x01);
            //setting TS SSC (Spread Function)
            demod_ext_WriteDspReg((MS_U16)E_S2_SPREAD_SPAN, S_TS_SPREAD_SPAN);
            demod_ext_WriteDspReg((MS_U16)E_S2_SPREAD_STEP, S_TS_SPREAD_STEP_SIZE);

            demod_ext_WriteDspReg((MS_U16)E_S2_TUNER_CFO_L, 0);
            demod_ext_WriteDspReg((MS_U16)E_S2_TUNER_CFO_H, 0);

            //Configure TS output clk inv
            bRet&=demod_ext_ReadReg(0x0924, &u8Data);
            if (S_TS_clk_inv_VAL == 1)
            {
                u8Data|=0x20;
            }
            else
            {
                u8Data&=(0xff-0x20);
            }
            bRet&=demod_ext_WriteReg(0x0924,u8Data);//ts clk inv

            //Configure TS output data swap mode
            bRet&=demod_ext_ReadReg(0x2A40, &u8Data);

            if(S_TS_DataSwap_VAL) //enable
              u8Data|=0x20;
            else                  //disable
              u8Data&=~(0x20);
            bRet&=demod_ext_WriteReg(0x2A40,u8Data);

            bRet &=demod_ext_ReadReg(0x0900+(0x44)*2, &FWversionL);
            bRet &=demod_ext_ReadReg(0x0900+(0x44)*2+0x0001, &FWversionH);
            break;

        default:
            return FALSE;
    }

    return bRet;
}

static void demod_ext_Reset(void)
{
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;

    // mask miu access for all and mcu
    demod_ext_WriteReg(0x1200+(0x23)*2 + 1,0x0f);
    demod_ext_WriteReg(0x1200+(0x23)*2,0xf0);
    // 10us delay
    udelay(10);

    demod_ext_WriteReg(0x0B00 + (0x19) * 2, 0x03);
    demod_ext_WriteReg(0x0B00 + (0x10) * 2, 0x01);
    demod_ext_WriteReg(REG_MB_CNTL, 0x00);             //clear MB_CNTL
    udelay(5 * 100);

    // enable miu access of mcu gdma
    demod_ext_WriteReg(0x1200+(0x23)*2 + 1,0x00);
    demod_ext_WriteReg(0x1200+(0x23)*2,0x00);
    // 10us delay
    udelay(10);

    demod_ext_WriteReg(0x0B00 + (0x10) * 2, 0x00);
    demod_ext_WriteReg(0x0B00 + (0x19) * 2, 0x00);
    mdelay(5);
    demod_ext_ReadReg(REG_MB_CNTL, &u8Data);           //check MB_CNTL ready
    while(u8Data != 0xff)
    {
        mdelay(50);
        demod_ext_ReadReg(REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
        if (u32Retry++ > 200)
        {
            printk(">>>MSB1240: Reset Fail!\n");
            break;
        }
    }
}

static MS_BOOL demod_ext_DTV_SetFrequency(MS_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth, MS_BOOL bPalBG, MS_BOOL bLPsel)
{
    MS_U32 u32DMD_IfFreq = 5000;
    MS_U8  u8_if_agc_mode = 0;

    if(demod_ext_CurrentDemodulator_Type == demod_ext_datatype_E_DEVICE_DEMOD_DVB_T2)
    {
        MS_U8 bw = E_DEMOD_BW_8M;
        switch (eBandWidth)
        {
            case E_RF_CH_BAND_6MHz:
                bw = E_DEMOD_BW_6M;
                break;
            case E_RF_CH_BAND_7MHz:
                bw = E_DEMOD_BW_7M;
                break;
            case E_RF_CH_BAND_8MHz:
                bw = E_DEMOD_BW_8M;
                break;
            default:
                bw = E_DEMOD_BW_8M;
                break;
        }

        demod_ext_Reset();
        udelay(500);
        demod_ext_WriteDspReg((MS_U16)E_T2_BW, bw);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M

        demod_ext_WriteDspReg((MS_U16)E_T2_PLP_ID, g_msb1240_plp_id);
        demod_ext_WriteDspReg((MS_U16)E_T2_FC_L, (MS_U8)u32DMD_IfFreq);
        demod_ext_WriteDspReg((MS_U16)E_T2_FC_H, (MS_U8)(u32DMD_IfFreq>>8));
        demod_ext_WriteDspReg((MS_U16)E_T2_IF_AGC_INV_PWM_EN, u8_if_agc_mode);

        demod_ext_WriteReg(REG_FSM_EN, 0x01); // FSM_EN
    }
    else if (demod_ext_CurrentDemodulator_Type == demod_ext_datatype_E_DEVICE_DEMOD_DVB_T)
    {
        demod_ext_Reset();
        udelay(500);

        demod_ext_WriteDspReg((MS_U16)T_CONFIG_BW, eBandWidth);      // BW: 1->6M, 2->7M, 3->8M
        demod_ext_WriteDspReg((MS_U16)T_CONFIG_FC_L, (MS_U8)u32DMD_IfFreq);
        demod_ext_WriteDspReg((MS_U16)T_CONFIG_FC_H, (MS_U8)(u32DMD_IfFreq>>8));
        demod_ext_WriteDspReg((MS_U16)T_CONFIG_IF_INV_PWM_OUT_EN, u8_if_agc_mode);

        // Hierarchy mode
        demod_ext_WriteDspReg((MS_U8)T_CONFIG_LP_SEL, bLPsel? 0x01:0x00);

        demod_ext_WriteReg(REG_FSM_EN, 0x01); // FSM_EN//Active(1);
    }

    return TRUE;
}

static MS_BOOL demod_ext_DTV_DVB_C_SetFrequency(MS_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth,MS_U32 u32SymRate, mapi_demodulator_datatype_EN_CAB_CONSTEL_TYPE eQAM)
{
    MS_U8             reg_symrate_l = 0;
    MS_U8             reg_symrate_h = 0;

    MS_U32            u32DMD_IfFreq = 0;
    MS_U8             u8_if_agc_mode = 0;

    MS_BOOL           status = TRUE;

    eBandWidth = eBandWidth;

    reg_symrate_l = (MS_U8) (u32SymRate & 0xff);
    reg_symrate_h = (MS_U8) (u32SymRate >> 8);

    u32DMD_IfFreq = 5000;
    u8_if_agc_mode = 0;

    //// Reset Demod ///////////////////
    demod_ext_Reset();
    udelay(500);
    //// DSP Register Overwrite ///////////////////
    status &= demod_ext_WriteDspReg((MS_U16)C_config_fc_l, (MS_U8)u32DMD_IfFreq);
    status &= demod_ext_WriteDspReg((MS_U16)C_config_fc_h, (MS_U8)(u32DMD_IfFreq>>8));
    status &= demod_ext_WriteDspReg((MS_U16)C_if_inv_pwm_out_en, u8_if_agc_mode);
    if (u32SymRate == 0)//Auto SR QAM Mode
    {
        // DTV_DVB_C_Set_Config_dvbc_auto(MS_TRUE);
        status &= demod_ext_WriteDspReg((MS_U16)C_opmode_auto_scan_sym_rate, 1);
        status &= demod_ext_WriteDspReg((MS_U16)C_opmode_auto_scan_qam, 1);
        status &= demod_ext_WriteDspReg((MS_U16)C_config_bw_l, C_BW_L);
        status &= demod_ext_WriteDspReg((MS_U16)C_config_bw_h, C_BW_H);
    }
    else //Manual Mode
    {
        status &= demod_ext_WriteDspReg((MS_U16)C_opmode_auto_scan_sym_rate, 0);
        status &= demod_ext_WriteDspReg((MS_U16)C_opmode_auto_scan_qam, 0);
        status &= demod_ext_WriteDspReg((MS_U16)C_config_bw_l, reg_symrate_l);
        status &= demod_ext_WriteDspReg((MS_U16)C_config_bw_h, reg_symrate_h);
        status &= demod_ext_WriteDspReg((MS_U16)C_config_qam, (MS_U8)eQAM);
    }

    status &= demod_ext_WriteReg(REG_FSM_EN, 0x01);

    return status;
}

static MS_BOOL demod_ext_MSB1240_DVBS_Demod_Restart(MS_U32 u32Frequency, MS_U32 u32SymbolBitrate)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16Address =0;
    MS_U8   u8Data =0;
    MS_U16  u16SymbolRate =0;
    MS_U16  __attribute__ ((unused)) u16CenterFreq =0;
    MS_U8   u8counter = 0;
    MS_U16  u16_fwVER = 0;

    u16SymbolRate=u32SymbolBitrate;
    u16CenterFreq=(MS_U16)u32Frequency;

    u16Address=0x0990;
    bRet&=demod_ext_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=demod_ext_WriteReg(u16Address,u8Data);
    MsOS_DelayTask(50);
    u16Address=0x0B52;
    u8Data=(u16SymbolRate&0xFF);
    bRet&=demod_ext_WriteReg(u16Address,u8Data);
    u16Address=0x0B53;
    u8Data=((u16SymbolRate>>8)&0xFF);
    bRet&=demod_ext_WriteReg(u16Address,u8Data);

    //ADCPLL IQ swap
    if(MSB1240_DVBS_ADCPLL_IQ_SWAP==1)
    {
        u16Address=0x0A03;
        bRet&=demod_ext_ReadReg(u16Address, &u8Data);
        u8Data|=(0x10);
        bRet&=demod_ext_WriteReg(u16Address, u8Data);
    }

    u16Address=0x0990;
    bRet&=demod_ext_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    u8Data|=0x01;
    bRet&=demod_ext_WriteReg(u16Address,u8Data);

    u8counter = 6;
    bRet&=demod_ext_ReadReg(u16Address,&u8Data);
    while( ((u8Data&0x01) == 0x00) && (u8counter != 0) )
    {
        printk("0x0990=0x%x, bRet=%d, u8counter=%d\n",u8Data,bRet,u8counter);
        u8Data|=0x01;
        bRet&=demod_ext_WriteReg(u16Address,u8Data);
        bRet&=demod_ext_ReadReg(u16Address,&u8Data);
        u8counter--;
    }

    if((u8Data&0x01)==0x00)
    {
        bRet = FALSE;
    }

    bRet &=demod_ext_ReadReg(0x0900+(0x44)*2+1, &u8Data);
    u16_fwVER = u8Data;
    bRet &=demod_ext_ReadReg(0x0900+(0x44)*2, &u8Data);
    u16_fwVER = (u16_fwVER<<8)|u8Data;
    printk("MSB1240_S MDrv_Demod_Restart-, FW_VER=0x%x\n",u16_fwVER);

    return bRet;
}

static MS_U8 saveT_eBandWidth = E_RF_CH_BAND_8MHz;
static MS_U8 saveT_bLPsel = FALSE;
static MS_U16 dvb_ext_state = 0;

extern int suspend_demod_ext(void)
{
    DBG_DEMOD_EXT_STR_ENTRY(printk("[DEMOD_EXT STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    MDrv_Demod_DVBEXT_GetState(&dvb_ext_state);
    if (dvb_ext_state == 0)
    {
        DBG_DEMOD_EXT_STR(printk("suspend_demod_ext: no external demod\n"));
        return  0;
    }

    MDrv_MSPI_SetPowerState(E_POWER_SUSPEND);
    if (dvb_ext_state != MSB1240_disconnect)
    {
        MDrv_GPIO_Init();
        MDrv_IIC_Init();
        MDrv_SW_IIC_SetSpeed(DEMOD_IIC_CHANNEL_ID, 1);
        MDrv_SW_IIC_Enable(DEMOD_IIC_CHANNEL_ID, ENABLE);

        MDrv_GPIO_Set_Low(62+1); // gpio reset external demod
    }

    DBG_DEMOD_EXT_STR_EXIT(printk("[DEMOD_EXT STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    return 0;
}

extern int resume_demod_ext(void)
{
#if 0
    MS_BOOL status = TRUE;
#endif
    MS_U8 u8tmp;

    DBG_DEMOD_EXT_STR_ENTRY(printk("[DEMOD_EXT STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if (dvb_ext_state == 0)
    {
        DBG_DEMOD_EXT_STR(printk("resume_demod_ext: no external demod\n"));
        return  0;
    }
    DBG_DEMOD_EXT_STR(printk("resume_demod_ext: resume...\n"));

    MDrv_MSPI_SetPowerState(E_POWER_RESUME);
    if (dvb_ext_state != MSB1240_disconnect)
    {
#if 0
        MDrv_GPIO_Init();
        MDrv_IIC_Init();
        MDrv_SW_IIC_Enable(DEMOD_IIC_CHANNEL_ID, ENABLE);
#endif
        MDrv_GPIO_Set_High(62+1); // gpio release external demod
        demod_ext_I2C_CH_Reset(3);
#if 0
        // check if MCP exist >>>>>
        status &= demod_ext_ReadReg_I2C(0x0900+(0x4b)*2, &u8tmp);
        status &= (u8tmp == 0xaa);
        status &= demod_ext_ReadReg_I2C(0x0900+(0x4b)*2+1, &u8tmp);
        status &= (u8tmp == 0x55);
        if (!status)
        {
            printk("no external demod\n");
            return  0;
        }
        printk("resume external demod\n");
        // <<<<< check if MCP exist
#endif
        demod_ext_Turn_Off_ALL_Pad_In(TRUE);  // turn off all pad in on slave side

        demod_ext_EnableSPIPad(TRUE);
        demod_ext_DTV_DVB_HW_init();
#if 0
        demod_ext_LoadDspCodeToSDRAMAll();
        demod_ext_LoadDSPCode();
#endif
        demod_ext_EnableSPIPad(FALSE);

        MDrv_Demod_DVBEXT_SetState(MSB1240_disconnect);
    }

    DBG_DEMOD_EXT_STR_EXIT(printk("[DEMOD_EXT STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    return 0;
}


