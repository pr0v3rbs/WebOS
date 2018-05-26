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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2010.03.08
 * note		  Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------
 *	 Control Constants
 *--------------------------------------------------------------------------*/
#undef USE_KDRV_CODES_FOR_L9

/*----------------------------------------------------------------------------
 *	 File Inclusions
 *--------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "base_types.h"
#include "base_device.h"
#include "os_util.h"

#include "de_model.h"
#include "de_kapi.h"
#include "de_ver_def.h"

#include "de_def.h"
#include "de_hal_def.h"
#include "de_hal.h"
#include "mcu/de_int_def.h"
#include "mcu/de_ipc_def.h"
#ifdef USE_KDRV_CODES_FOR_L9
#include "l9/de/de_fw_l9.h"
#include "l9/de/de_reg_l9.h"
#include "l9/de/de_ipc_l9.h"
#include "l9/de/vp_reg_l9.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13
#include "h13/de/de_fw_h13.h"
#include "h13/de/de_reg_h13.h"
#include "h13/de/de_ipc_h13.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H14
#include "h14/de/de_fw_h14.h"
#include "h14/de/de_reg_h14.h"
#include "h14/de/de_ipc_h14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14
#include "m14/de/de_fw_m14.h"
#include "m14/de/de_reg_m14.h"
#include "m14/de/de_ipc_m14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H15
#include "h15/de/de_fw_h15.h"
#include "h15/de/de_reg_h15.h"
#include "h15/de/de_ipc_h15.h"
#endif

/*----------------------------------------------------------------------------
 *	 Constant Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 Macro Definitions
 *--------------------------------------------------------------------------*/
#define CHECK_REG_NULL(fp) {if(fp == NULL || g_chip_id < 0)\
							{DE_ERROR("%s is not supported in this chip.\n",#fp); \
							return RET_ERROR;}}
#define GET_ID() g_chip_id

/*----------------------------------------------------------------------------
 *	 Type Definitions
 *--------------------------------------------------------------------------*/
typedef struct {
	int (*init)(LX_DE_PANEL_TYPE_T *pstParams);
	int (*set_frame_rate)(DE_DPLL_CLK_T dclk);
	int (*set_bg_color)(LX_DE_COLOR_T *pBackColor);
	int (*set_win_blank)(LX_DE_SET_WIN_BLANK_T *pWinBlank);
	int (*get_mvi_colorimetry)(LX_DE_MVI_COLORIMETRY_T *pstParams);
	int (*set_cvi_src_type)(LX_DE_CVI_SRC_TYPE_T *pstParams);
	int (*set_uart4mcu)(int bUart4MCU);
	int (*download_fw)(LX_DE_FW_DWLD_T *pstParams);
	int (*download_fw_bin)(void);
	int (*download_fw_head)(void);
	int (*set_cvi_fir)(LX_DE_CVI_FIR_T *pstParams);
	int (*set_cvi_tpg)(LX_DE_CVI_TPG_T *pstParams);
	int (*set_cvi_csample)(LX_DE_CSAMPLE_MODE_T *pstParams);
	int (*set_vcs)(LX_DE_VCS_IPC_T *pstParams);
	int (*reset_de)(BOOLEAN bReset);
	int (*init_phy2virt)(void);
	int (*free_phy2virt)(void);
	int (*init_gpio)(void);
	int (*init_hdmi)(void);
	int (*init_lvds)(void);
	int (*init_misc)(void);
	int (*init_osd)(void);
	BOOLEAN (*is_alive_ipc)(void);
	int (*write_reg)(UINT32 addr, UINT32 value);
	UINT32 (*read_reg)(UINT32 addr);
	int (*set_cvi_buffer)(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams);
	int (*set_cvi_rw)(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams,\
						LX_DE_CVI_RW_PREW_FRAME_T *prewInfo);
	int (*sel_multiwinsrc)(LX_DE_MULTI_WIN_SRC_T *pstParams);
	int (*set_scart_rbg_bypass)(int enable);
	int (*set_ud_mode)(BOOLEAN *pstParams);
	int (*set_debug)(LX_DE_SET_DBG_T *pstParams);
	int (*get_debug)(LX_DE_SET_DBG_T *pstParams);
	int (*set_win_port_src)(LX_DE_INPUT_CFG_T *pstParams);
	int (*set_if_config)(LX_DE_IF_CONFIG_T *pstParams);
	int (*get_sys_status)(LX_DE_SYS_STATUS_T *pstParams);
	int (*get_src_status)(LX_DE_SRC_STATUS_T *pstParams);
	int (*set_operation)(LX_DE_OPERATION_CTRL_T *pstParams);
	int (*set_operation_sub)(LX_DE_SUB_OPERATION_CTRL_T *pstParams);
	int (*set_capture)(LX_DE_CAPTURE_CTRL_T *pstParams);
	BOOLEAN (*check_irq4vsync)(void);
	int (*switch_uart0)(int whichUart);
	int (*set_ttx)(VIDEO_TTX_PACK_T *pTtxPack);
	int (*set_vtm)(LX_DE_VTM_FRAME_INFO_T *pstParams);
	int (*get_vtm)(LX_DE_VTM_FRAME_INFO_T *pstParams);
	int (*get_fir)(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir); 
	int (*verify_fw)(void);
	int (*set_seamless)(LX_DE_RECT_T *pstParams);
	int (*set_hdmi_port)(LX_DE_HDMIPORT_T *pstParams);
	int (*get_capa)(LX_DE_CAPACITY_T *pstParams);
} DE_REG_CB_T;

DE_REG_CB_T gfn_all[5];
static int g_chip_id = 0;

/*----------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 External Variables
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 global Functions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 global Variables
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 Static Function Prototypes Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	 Static Variables
 *--------------------------------------------------------------------------*/
static DE_DPLL_CLK_T sDeDpllClk_1920[] = {
	DCLK_148_5	  ,
	DCLK_148_3516 ,
};

static DE_DPLL_CLK_T sDeDpllClk_1366[] = {
	DCLK_81 ,
	DCLK_81 ,
};

static DE_DPLL_CLK_T sDeDpllClk_1024[] = {
	DCLK_65 ,
	DCLK_65 ,
};

static DE_DPLL_CLK_T sDeDpllClk_3840[] = {
	DCLK_148_5	  ,
	DCLK_148_3516 ,
};

static DE_DPLL_CLK_T sDeDpllClk_480[] = {
	DCLK_54 ,
	DCLK_54 ,
};

static DE_PANEL_SET_T sDePanelSet[] = {
	{ LX_PANEL_TYPE_1920 , sDeDpllClk_1920 },
	{ LX_PANEL_TYPE_1366 , sDeDpllClk_1366 },
	{ LX_PANEL_TYPE_1024 , sDeDpllClk_1024 },
	{ LX_PANEL_TYPE_1365 , sDeDpllClk_1366 },
	{ LX_PANEL_TYPE_3840 , sDeDpllClk_3840 },
	{ LX_PANEL_TYPE_1280 , sDeDpllClk_1920 },
	{ LX_PANEL_TYPE_720  , sDeDpllClk_480  },
	{ LX_PANEL_TYPE_576  , sDeDpllClk_480  }
};

static LX_DE_FR_RATE_T sDeFrRate = { FALSE, 60 };
static LX_DE_PANEL_TYPE_T sDePanelType = LX_PANEL_TYPE_1920;

/*============================================================================
 *	 Implementation Group
 *==========================================================================*/
/**
 * @callgraph
 * @callergraph
 *
 * @brief select chip for call-back function 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_Select_Chip(void)
{
#define REG_FN gfn_all[g_chip_id]

	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		g_chip_id = 4;
		REG_FN.init                  = DE_REG_H15_Init;
		REG_FN.set_frame_rate        = DE_REG_H15_SetFrRate;
		REG_FN.set_bg_color          = DE_REG_H15_SetBgColor;
		REG_FN.set_win_blank         = DE_REG_H15_SetWinBlank;
		REG_FN.get_mvi_colorimetry   = NULL;
		REG_FN.set_cvi_src_type      = DE_REG_H15_SetCviSrcType;
		REG_FN.set_uart4mcu          = DE_REG_H15_Uart0_Switch;
		REG_FN.download_fw           = DE_REG_H15_FW_Download;
		REG_FN.download_fw_bin       = DE_FW_H15_DownloadFromBin;
		REG_FN.download_fw_head      = DE_FW_H15_DownloadFromHead;
		REG_FN.set_cvi_fir           = DE_REG_H15_SetCviFir;
		REG_FN.set_cvi_tpg           = DE_REG_H15_SetCviTpg;
		REG_FN.set_cvi_csample       = DE_REG_H15_SetCviCsampleMode;
		REG_FN.set_vcs               = DE_REG_H15_SetVcs;
		REG_FN.reset_de              = DE_REG_H15_ResetDE;
		REG_FN.init_phy2virt         = DE_REG_H15_InitPHY2VIRT;
		REG_FN.free_phy2virt         = DE_REG_H15_FreePHY2VIRT;
		REG_FN.init_gpio             = DE_REG_H15_GPIO_Init;
		REG_FN.init_hdmi             = DE_REG_H15_HDMI_Init;
		REG_FN.init_lvds             = DE_REG_H15_LVDS_Init;
		REG_FN.init_misc             = DE_REG_H15_MISC_Init;
		REG_FN.init_osd              = DE_REG_H15_OSD_Init;
		REG_FN.is_alive_ipc          = DE_REG_H15_IPCisAlive;
		REG_FN.write_reg             = DE_REG_H15_WD;
		REG_FN.read_reg              = DE_REG_H15_RD;
		REG_FN.set_cvi_buffer        = DE_REG_H15_SetCviVideoFrameBuffer;
		REG_FN.set_cvi_rw            = DE_REG_H15_SetCviRWFrameBuffer;
		REG_FN.sel_multiwinsrc       = DE_REG_H15_SelectMultiWinSrc;
		REG_FN.set_scart_rbg_bypass  = NULL;
		REG_FN.set_ud_mode           = DE_REG_H15_SetUdMode;
		REG_FN.set_debug             = DE_REG_H15_SetDebug;
		REG_FN.get_debug             = NULL;
		REG_FN.set_win_port_src        = DE_REG_H15_SetWinPortSrc;
		REG_FN.set_if_config         = DE_REG_H15_SetInterfaceConfig;
		REG_FN.get_sys_status        = DE_REG_H15_GetSystemStatus;
		REG_FN.get_src_status        = DE_REG_H15_GetSourceStatus;
		REG_FN.set_operation         = DE_REG_H15_SetOperation;
		REG_FN.set_operation_sub     = DE_REG_H15_SetSubOperation;
		REG_FN.set_capture           = NULL;
		REG_FN.switch_uart0          = DE_REG_H15_Uart0_Switch;
		REG_FN.set_ttx               = NULL;
		REG_FN.set_vtm               = DE_REG_H15_SetVTM;
		REG_FN.get_vtm               = DE_REG_H15_GetVTM;
		REG_FN.get_fir               = DE_REG_H15_GetFIR;
		REG_FN.verify_fw             = DE_FW_H15_Verify;
		REG_FN.set_hdmi_port         = DE_REG_H15_SetHdmiPort;
		REG_FN.get_capa              = DE_REG_H15_GetCapacity;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		g_chip_id = 3;
		REG_FN.init                  = DE_REG_H14_Init;
		REG_FN.set_frame_rate        = DE_REG_H14_SetFrRate;
		REG_FN.set_bg_color          = DE_REG_H14_SetBgColor;
		REG_FN.set_win_blank         = DE_REG_H14_SetWinBlank;
		REG_FN.get_mvi_colorimetry   = NULL;
		REG_FN.set_cvi_src_type      = DE_REG_H14_SetCviSrcType;
		REG_FN.set_uart4mcu          = DE_REG_H14_Uart0_Switch;
		REG_FN.download_fw           = DE_REG_H14_FW_Download;
		REG_FN.download_fw_bin       = DE_FW_H14_DownloadFromBin;
		REG_FN.download_fw_head      = DE_FW_H14_DownloadFromHead;
		REG_FN.set_cvi_fir           = DE_REG_H14_SetCviFir;
		REG_FN.set_cvi_tpg           = DE_REG_H14_SetCviTpg;
		REG_FN.set_cvi_csample       = DE_REG_H14_SetCviCsampleMode;
		REG_FN.set_vcs               = DE_REG_H14_SetVcs;
		REG_FN.reset_de              = DE_REG_H14_ResetDE;
		REG_FN.init_phy2virt         = DE_REG_H14_InitPHY2VIRT;
		REG_FN.free_phy2virt         = DE_REG_H14_FreePHY2VIRT;
		REG_FN.init_gpio             = DE_REG_H14_GPIO_Init;
		REG_FN.init_hdmi             = DE_REG_H14_HDMI_Init;
		REG_FN.init_lvds             = DE_REG_H14_LVDS_Init;
		REG_FN.init_misc             = DE_REG_H14_MISC_Init;
		REG_FN.init_osd              = DE_REG_H14_OSD_Init;
		REG_FN.is_alive_ipc          = DE_REG_H14_IPCisAlive;
		REG_FN.write_reg             = DE_REG_H14_WD;
		REG_FN.read_reg              = DE_REG_H14_RD;
		REG_FN.set_cvi_buffer        = DE_REG_H14_SetCviVideoFrameBuffer;
		REG_FN.set_cvi_rw            = NULL;
		REG_FN.sel_multiwinsrc       = DE_REG_H14_SelectMultiWinSrc;
		REG_FN.set_scart_rbg_bypass  = NULL;
		REG_FN.set_ud_mode           = DE_REG_H14_SetUdMode;
		REG_FN.set_debug             = DE_REG_H14_SetDebug;
		REG_FN.get_debug             = NULL;
		REG_FN.set_win_port_src        = DE_REG_H14_SetWinPortSrc;
		REG_FN.set_if_config         = DE_REG_H14_SetInterfaceConfig;
		REG_FN.get_sys_status        = DE_REG_H14_GetSystemStatus;
		REG_FN.get_src_status        = DE_REG_H14_GetSourceStatus;
		REG_FN.set_operation         = DE_REG_H14_SetOperation;
		REG_FN.set_operation_sub     = DE_REG_H14_SetSubOperation;
		REG_FN.set_capture           = NULL;
		REG_FN.switch_uart0          = DE_REG_H14_Uart0_Switch;
		REG_FN.set_ttx               = NULL;
		REG_FN.set_vtm               = DE_REG_H14_SetVTM;
		REG_FN.get_vtm               = DE_REG_H14_GetVTM;
		REG_FN.get_fir               = DE_REG_H14_GetFIR;
		REG_FN.verify_fw             = DE_FW_H14_Verify;
		REG_FN.set_hdmi_port         = DE_REG_H14_SetHdmiPort;	
		REG_FN.get_capa              = DE_REG_H14_GetCapacity;		
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		g_chip_id = 2;
		REG_FN.init                  = DE_REG_M14_Init;
		REG_FN.set_frame_rate        = DE_REG_M14_SetFrRate;
		REG_FN.set_bg_color          = DE_REG_M14_SetBgColor;
		REG_FN.set_win_blank         = DE_REG_M14_SetWinBlank;
		REG_FN.get_mvi_colorimetry   = NULL;
		REG_FN.set_cvi_src_type      = DE_REG_M14_SetCviSrcType;
		REG_FN.set_uart4mcu          = DE_REG_M14_Uart0_Switch;
		REG_FN.download_fw           = DE_REG_M14_FW_Download;
		REG_FN.download_fw_bin       = DE_FW_M14_DownloadFromBin;
		REG_FN.download_fw_head      = DE_FW_M14_DownloadFromHead;
		REG_FN.set_cvi_fir           = DE_REG_M14_SetCviFir;
		REG_FN.set_cvi_tpg           = DE_REG_M14_SetCviTpg;
		REG_FN.set_cvi_csample       = DE_REG_M14_SetCviCsampleMode;
		REG_FN.set_vcs               = DE_REG_M14_SetVcs;
		REG_FN.reset_de              = DE_REG_M14_ResetDE;
		REG_FN.init_phy2virt         = DE_REG_M14_InitPHY2VIRT;
		REG_FN.free_phy2virt         = DE_REG_M14_FreePHY2VIRT;
		REG_FN.init_gpio             = DE_REG_M14_GPIO_Init;
		REG_FN.init_hdmi             = DE_REG_M14_HDMI_Init;
		REG_FN.init_lvds             = DE_REG_M14_LVDS_Init;
		REG_FN.init_misc             = DE_REG_M14_MISC_Init;
		REG_FN.init_osd              = DE_REG_M14_OSD_Init;
		REG_FN.is_alive_ipc          = DE_REG_M14_IPCisAlive;
		REG_FN.write_reg             = DE_REG_M14_WD;
		REG_FN.read_reg              = DE_REG_M14_RD;
		REG_FN.set_cvi_buffer        = DE_REG_M14_SetCviVideoFrameBuffer;
		REG_FN.set_cvi_rw            = DE_REG_M14_SetCviRWFrameBuffer;
		REG_FN.sel_multiwinsrc       = DE_REG_M14_SelectMultiWinSrc;
		REG_FN.set_scart_rbg_bypass  = NULL;
		REG_FN.set_ud_mode           = DE_REG_M14_SetUdMode;
		REG_FN.set_debug             = DE_REG_M14_SetDebug;
		REG_FN.get_debug             = NULL;
		REG_FN.set_win_port_src        = DE_REG_M14_SetWinPortSrc;
		REG_FN.set_if_config         = DE_REG_M14_SetInterfaceConfig;
		REG_FN.get_sys_status        = DE_REG_M14_GetSystemStatus;
		REG_FN.get_src_status        = DE_REG_M14_GetSourceStatus;
		REG_FN.set_operation         = DE_REG_M14_SetOperation;
		REG_FN.set_operation_sub     = DE_REG_M14_SetSubOperation;
		REG_FN.set_capture           = NULL;
		REG_FN.switch_uart0          = DE_REG_M14_Uart0_Switch;
		REG_FN.set_ttx               = NULL;
		REG_FN.set_vtm               = DE_REG_M14_SetVTM;
		REG_FN.get_vtm               = DE_REG_M14_GetVTM;
		REG_FN.get_fir               = DE_REG_M14_GetFIR;
		REG_FN.verify_fw             = DE_FW_M14_Verify;
		REG_FN.set_hdmi_port         = DE_REG_M14_SetHdmiPort;
		REG_FN.get_capa              = DE_REG_M14_GetCapacity;		
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		g_chip_id = 1;
		REG_FN.init                  = DE_REG_H13_Init;
		REG_FN.set_frame_rate        = DE_REG_H13_SetFrRate;
		REG_FN.set_bg_color          = DE_REG_H13_SetBgColor;
		REG_FN.set_win_blank         = DE_REG_H13_SetWinBlank;
		REG_FN.get_mvi_colorimetry   = NULL;
		REG_FN.set_cvi_src_type      = DE_REG_H13_SetCviSrcType;
		REG_FN.set_uart4mcu          = DE_REG_H13_Uart0_Switch;
		REG_FN.download_fw           = DE_REG_H13_FW_Download;
		REG_FN.download_fw_bin       = DE_FW_H13_DownloadFromBin;
		REG_FN.download_fw_head      = DE_FW_H13_DownloadFromHead;
		REG_FN.set_cvi_fir           = DE_REG_H13_SetCviFir;
		REG_FN.set_cvi_tpg           = DE_REG_H13_SetCviTpg;
		REG_FN.set_cvi_csample       = DE_REG_H13_SetCviCsampleMode;
		REG_FN.set_vcs               = DE_REG_H13_SetVcs;
		REG_FN.reset_de              = DE_REG_H13_ResetDE;
		REG_FN.init_phy2virt         = DE_REG_H13_InitPHY2VIRT;
		REG_FN.free_phy2virt         = DE_REG_H13_FreePHY2VIRT;
		REG_FN.init_gpio             = DE_REG_H13_GPIO_Init;
		REG_FN.init_hdmi             = DE_REG_H13_HDMI_Init;
		REG_FN.init_lvds             = DE_REG_H13_LVDS_Init;
		REG_FN.init_misc             = DE_REG_H13_MISC_Init;
		REG_FN.init_osd              = DE_REG_H13_OSD_Init;
		REG_FN.is_alive_ipc          = DE_REG_H13_IPCisAlive;
		REG_FN.write_reg             = DE_REG_H13_WD;
		REG_FN.read_reg              = DE_REG_H13_RD;
		REG_FN.set_cvi_buffer        = DE_REG_H13_SetCviVideoFrameBuffer;
		REG_FN.set_cvi_rw            = NULL;
		REG_FN.sel_multiwinsrc       = DE_REG_H13_SelectMultiWinSrc;
		REG_FN.set_scart_rbg_bypass  = NULL;
		REG_FN.set_ud_mode           = DE_REG_H13_SetUdMode;
		REG_FN.set_debug             = DE_REG_H13_SetDebug;
		REG_FN.get_debug             = NULL;
		REG_FN.set_win_port_src         = DE_REG_H13_SetWinPortSrc;
		REG_FN.set_if_config         = DE_REG_H13_SetInterfaceConfig;
		REG_FN.get_sys_status        = DE_REG_H13_GetSystemStatus;
		REG_FN.get_src_status        = DE_REG_H13_GetSourceStatus;
		REG_FN.set_operation         = DE_REG_H13_SetOperation;
		REG_FN.set_operation_sub     = DE_REG_H13_SetSubOperation;
		REG_FN.set_capture           = NULL;
		REG_FN.switch_uart0          = DE_REG_H13_Uart0_Switch;
		REG_FN.set_ttx               = NULL;
		REG_FN.set_vtm               = DE_REG_H13_SetVTM;
		REG_FN.get_vtm               = DE_REG_H13_GetVTM;
		REG_FN.get_fir               = DE_REG_H13_GetFIR;
		REG_FN.verify_fw             = DE_FW_H13_Verify;
		REG_FN.set_seamless          = DE_REG_H13_SetSeamlessInfo;
		REG_FN.set_hdmi_port         = DE_REG_H13_SetHdmiPort;	
		REG_FN.get_capa              = DE_REG_H13_GetCapacity;		
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_L9) ) {
		g_chip_id = 0;
		REG_FN.init                  = DE_REG_L9_Init;
		REG_FN.set_frame_rate        = DE_REG_L9_SetFrRate;
		REG_FN.set_bg_color          = DE_REG_L9_SetBgColor;
		REG_FN.set_win_blank         = DE_REG_L9_SetWinBlank;
		REG_FN.get_mvi_colorimetry   = DE_REG_L9_GetMviColorimetry;
		REG_FN.set_cvi_src_type      = DE_REG_L9_SetCviSrcType;
		REG_FN.set_uart4mcu          = DE_REG_L9_Uart0_Switch;
		REG_FN.download_fw           = DE_REG_L9_FW_Download;
		REG_FN.download_fw_bin       = DE_FW_L9_DownloadFromBin;
		REG_FN.download_fw_head      = DE_FW_L9_DownloadFromHead;
		REG_FN.set_cvi_fir           = DE_REG_L9_SetCviFir;
		REG_FN.set_cvi_tpg           = DE_REG_L9_SetCviTpg;
		REG_FN.set_cvi_csample       = DE_REG_L9_SetCviCsampleMode;
		REG_FN.set_vcs               = DE_REG_L9_SetVcs;
		REG_FN.reset_de              = DE_REG_L9_ResetDE;
		REG_FN.init_phy2virt         = DE_REG_L9_InitPHY2VIRT;
		REG_FN.free_phy2virt         = DE_REG_L9_FreePHY2VIRT;
		REG_FN.init_gpio             = DE_REG_L9_GPIO_Init;
		REG_FN.init_hdmi             = DE_REG_L9_HDMI_Init;
		REG_FN.init_lvds             = DE_REG_L9_LVDS_Init;
		REG_FN.init_misc             = DE_REG_L9_MISC_Init;
		REG_FN.init_osd              = DE_REG_L9_OSD_Init;
		REG_FN.is_alive_ipc          = DE_REG_L9_IPCisAlive;
		REG_FN.write_reg             = DE_REG_L9_WD;
		REG_FN.read_reg              = DE_REG_L9_RD;
		REG_FN.set_cvi_buffer        = DE_REG_L9_SetCviVideoFrameBuffer;
		REG_FN.set_cvi_rw            = NULL;
		REG_FN.sel_multiwinsrc       = DE_REG_L9_SelectMultiWinSrc;
		REG_FN.set_scart_rbg_bypass  = DE_REG_L9_Set_SCART_RGB_Bypass_Mode;
		REG_FN.set_ud_mode           = DE_REG_L9_SetUdMode;
		REG_FN.set_debug             = DE_REG_L9_SetDebug;
		REG_FN.get_debug             = NULL;
		REG_FN.set_win_port_src	= DE_REG_L9_SettWinPortSrc;
		REG_FN.set_if_config         = NULL;
		REG_FN.get_sys_status        = NULL;
		REG_FN.get_src_status        = NULL;
		REG_FN.set_operation         = NULL;
		REG_FN.set_operation_sub     = NULL;
		REG_FN.set_capture           = NULL;
		REG_FN.switch_uart0          = DE_REG_L9_Uart0_Switch;
		REG_FN.set_ttx               = DE_REG_L9_SetTtx;
		REG_FN.set_vtm               = DE_REG_L9_SetVTM;
		REG_FN.get_vtm               = NULL
		REG_FN.get_fir               = NULL;
		REG_FN.verify_fw             = NULL;
		REG_FN.set_hdmi_port         = NULL;
		REG_FN.get_capa              = NULL;
	} else
#endif
	{
		g_chip_id = -1;
		DE_ERROR("not defined chip rev.\n");
		ret = RET_ERROR;
	}

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set Frame rate of Display
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_Init(LX_DE_PANEL_TYPE_T *pstParams)
{
	int ret = RET_OK;

	sDePanelType = *pstParams;
	CHECK_REG_NULL(gfn_all[GET_ID()].init);
	ret = gfn_all[GET_ID()].init(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get irq number
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_InitRegister(void)
{
	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set Frame rate of Display
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetFrRate(LX_DE_FR_RATE_T *pstParams)
{
	int ret = RET_OK;
	DE_DPLL_CLK_T dclk;
	DE_DPLL_CLK_T *pDisplayPll = NULL;
	int pllIndex;
	int i;

	do {
		CHECK_KNULL(pstParams);
		sDeFrRate = *pstParams;
		pllIndex = 0;

		for (i=0;i<ARRAY_SIZE(sDePanelSet);i++) {
			if (sDePanelSet[i].panelType != sDePanelType) continue;
			pDisplayPll = sDePanelSet[i].pDpllClk;
			break;
		}
		CHECK_KNULL(pDisplayPll);
		dclk = pDisplayPll[pllIndex];

		CHECK_REG_NULL(gfn_all[GET_ID()].set_frame_rate);
		gfn_all[GET_ID()].set_frame_rate(dclk);
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get Frame rate of Display
 *
 * @param pFrRate [OUT] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetFrRate(LX_DE_FR_RATE_T *pstParams)
{
	*pstParams = sDeFrRate;
	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief send color space conversion matrix and offset for each external source information.
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetBgColor(LX_DE_COLOR_T *pBackColor)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_bg_color);
	ret = gfn_all[GET_ID()].set_bg_color(pBackColor);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set blank color of selected window
 *
 * @param pWinBlank [IN] structure pointer to carry information about window Id, whether Turn On or Off and blank color
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetWinBlank(LX_DE_SET_WIN_BLANK_T *pWinBlank)
{
	int ret = RET_OK;
	
	CHECK_REG_NULL(gfn_all[GET_ID()].set_win_blank);
	ret = gfn_all[GET_ID()].set_win_blank(pWinBlank);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief copy frame buffer of certain block size and position.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviVideoFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams)
{
	int ret = RET_OK;
	
	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_buffer);
	ret = gfn_all[GET_ID()].set_cvi_buffer(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief copy frame buffer of certain block size and position.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviRWFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams,\
								LX_DE_CVI_RW_PREW_FRAME_T *prewInfo)
{
	int ret = RET_OK;
	
	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_rw);
	ret = gfn_all[GET_ID()].set_cvi_rw(pstParams, prewInfo);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set information which is comming from CVI port
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviSrcType(LX_DE_CVI_SRC_TYPE_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_src_type);
	ret = gfn_all[GET_ID()].set_cvi_src_type(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get MVI source colorimetry information.
 *
 * @param pstParams [OUT] value of MVI colorimetry
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetMviColorimetry(LX_DE_MVI_COLORIMETRY_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_mvi_colorimetry);
	ret = gfn_all[GET_ID()].get_mvi_colorimetry(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set uart for MCU or CPU
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetUart4MCU(BOOLEAN bUart4MCU)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_uart4mcu);
	ret = gfn_all[GET_ID()].set_uart4mcu(bUart4MCU);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set de VCS parameter.
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetVcs(LX_DE_VCS_IPC_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_vcs);
	ret = gfn_all[GET_ID()].set_vcs(pstParams);

	return ret;
}

#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_HAL_FW_Download(LX_DE_FW_DWLD_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].download_fw);
	ret = gfn_all[GET_ID()].download_fw(pstParams);

	return ret;
}
#endif

/**
 * @callgraph
 * @callergraph
 *
 * @brief send captured video fir coef for double/quad sampling case.
 *
 * @param cvi_channel, fir coef, and enable [IN] value of capture video
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviFir(LX_DE_CVI_FIR_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_fir);
	ret = gfn_all[GET_ID()].set_cvi_fir(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief send set captured video test pattern generator to black.
 *
 * @param cvi_channel, pattern enable, freeze enable, pattern color space [IN]] value of capture video
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviTpg(LX_DE_CVI_TPG_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_tpg);
	ret = gfn_all[GET_ID()].set_cvi_tpg(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set captured video color sampling mode(sub sampling or 3 tap filtering).
 *
 * @param cvi_channel, color sampling mode(sub_sample, 3-tap-filter) [IN] value of capture video
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCviCsampleMode(LX_DE_CSAMPLE_MODE_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_cvi_csample);
	ret = gfn_all[GET_ID()].set_cvi_csample(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief reset Register of DE HW Block
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_ResetDE(BOOLEAN bReset)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].reset_de);
	ret = gfn_all[GET_ID()].reset_de(bReset);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init phys -> virt for register access
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_InitPHY2VIRT(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_phy2virt);
	ret = gfn_all[GET_ID()].init_phy2virt();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief free phys -> virt for register access
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_FreePHY2VIRT(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].free_phy2virt);
	ret = gfn_all[GET_ID()].free_phy2virt();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init gpio
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GPIO_Init(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_gpio);
	ret = gfn_all[GET_ID()].init_gpio();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init hdmi 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_HDMI_Init(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_hdmi);
	ret = gfn_all[GET_ID()].init_hdmi();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init lvds 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_LVDS_Init(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_lvds);
	ret = gfn_all[GET_ID()].init_lvds();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init misc 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_MISC_Init(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_misc);
	ret = gfn_all[GET_ID()].init_misc();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init osd 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_OSD_Init(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].init_osd);
	ret = gfn_all[GET_ID()].init_osd();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief download firmware from binary file
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_FW_DownloadFromBin(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].download_fw_bin);
	ret = gfn_all[GET_ID()].download_fw_bin();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief download firmware from header 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_FW_DownloadFromHead(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].download_fw_head);
	ret = gfn_all[GET_ID()].download_fw_head();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief check ipc ready
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_IPCisAlive(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].is_alive_ipc);
	ret = gfn_all[GET_ID()].is_alive_ipc();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief write register 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_WriteReg(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].write_reg);
	ret = gfn_all[GET_ID()].write_reg(addr, value);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief read register
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_ReadReg(UINT32 addr, UINT32 *value)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].read_reg);
	ret = gfn_all[GET_ID()].read_reg(addr);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief control ud mode 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetUdMode(BOOLEAN *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_ud_mode);
	ret = gfn_all[GET_ID()].set_ud_mode(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief control vtm 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_vtm);
	ret = gfn_all[GET_ID()].set_vtm(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get address for vtm operation
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_vtm);
	ret = gfn_all[GET_ID()].get_vtm(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set pip source
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SelectMultiWinSrc(LX_DE_MULTI_WIN_SRC_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].sel_multiwinsrc);
	ret = gfn_all[GET_ID()].sel_multiwinsrc(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief sw work-around 
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetSCARTRGBBypassMode(int *pEnable)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_scart_rbg_bypass);
	ret = gfn_all[GET_ID()].set_scart_rbg_bypass(*pEnable);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief  
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetDebug(LX_DE_SET_DBG_T *pstParams)
{
	int ret = RET_OK;
	
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief  
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetDebug(LX_DE_SET_DBG_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_debug);
	ret = gfn_all[GET_ID()].set_debug(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetWinPortSrc(LX_DE_INPUT_CFG_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_win_port_src);
	ret = gfn_all[GET_ID()].set_win_port_src(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetInterfaceConfig(LX_DE_IF_CONFIG_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_if_config);
	ret = gfn_all[GET_ID()].set_if_config(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetSystemStatus(LX_DE_SYS_STATUS_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_sys_status);
	ret = gfn_all[GET_ID()].get_sys_status(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_src_status);
	ret = gfn_all[GET_ID()].get_src_status(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set window configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetOperation(LX_DE_OPERATION_CTRL_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_operation);
	ret = gfn_all[GET_ID()].set_operation(pstParams);

	return ret;
}


/**
 * @callgraph
 * @callergraph
 *
 * @brief Set window configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetSubOperation(LX_DE_SUB_OPERATION_CTRL_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_operation_sub);
	ret = gfn_all[GET_ID()].set_operation_sub(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set capture control
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetCaptureControl(LX_DE_CAPTURE_CTRL_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_capture);
	ret = gfn_all[GET_ID()].set_capture(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make IPC interrupt of MCU be happen
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SwitchUART0(int whichUart)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].switch_uart0);
	ret = gfn_all[GET_ID()].switch_uart0(whichUart);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set ttx data
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetTtx(VIDEO_TTX_PACK_T *pTtxPack)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_ttx);
	ret = gfn_all[GET_ID()].set_ttx(pTtxPack);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get cvi fir coefficient
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetFIR(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_fir);
	ret = gfn_all[GET_ID()].get_fir(pstParams, fir);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief verify firmware binary
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_VerifyFirmware(void)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].verify_fw);
	ret = gfn_all[GET_ID()].verify_fw();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set hdmi port
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetHDMIPort(LX_DE_HDMIPORT_T *pstParams)
{
	int ret = RET_OK;
#ifdef USE_KDRV_CODES_FOR_H15
	CHECK_REG_NULL(gfn_all[GET_ID()].set_hdmi_port);
	ret = gfn_all[GET_ID()].set_hdmi_port(pstParams);
#endif
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get capacity of chip
 *
 * @param pstParams [OUT] capa info
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_GetCapacity(LX_DE_CAPACITY_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].get_capa);
	ret = gfn_all[GET_ID()].get_capa(pstParams);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set seamless info to mcu
 *
 * @param bReset [IN] BOOLEAN
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_HAL_SetSeamlessInfo(LX_DE_RECT_T *pstParams)
{
	int ret = RET_OK;

	CHECK_REG_NULL(gfn_all[GET_ID()].set_seamless);
	ret = gfn_all[GET_ID()].set_seamless(pstParams);

	return ret;
}

/**  @} */
