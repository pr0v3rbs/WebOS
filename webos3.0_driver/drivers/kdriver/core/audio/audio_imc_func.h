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
 * adec functions through IMC.
 * Communicate adec firmware through IMC instead registers.
 *
 * author     wonchang.shin (wonchang.shinu@lge.com)
 * version    1.0
 * date       2010.06.19
 *
 * @addtogroup lg1152_adec
 * @{
 */

#ifndef	_ADEC_IMC_FUNC_H_
#define	_ADEC_IMC_FUNC_H_

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "imc/adec_imc.h"
#include "imc/adec_imc_cmd.h"
#include "imc/cmd/module_cmd_adc.h"
#include "imc/cmd/module_cmd_connection.h"
#include "imc/cmd/module_cmd_debug.h"
#include "imc/cmd/module_cmd_ddc.h"
#include "imc/cmd/module_cmd_ddt.h"
#include "imc/cmd/module_cmd_dts.h"
#include "imc/cmd/module_cmd_ra8.h"
#include "imc/cmd/module_cmd_wma.h"
#include "imc/cmd/module_cmd_aacenc.h"
#include "imc/cmd/module_cmd_ac3enc.h"
#include "imc/cmd/module_cmd_dtsenc.h"
#include "imc/cmd/module_cmd_hdmi.h"
#include "imc/cmd/module_cmd_lipsync.h"
#include "imc/cmd/module_cmd_mixer.h"
#include "imc/cmd/module_cmd_outctrl.h"
#include "imc/cmd/module_cmd_se.h"
#include "imc/cmd/module_cmd_sif.h"
#include "imc/cmd/module_cmd_spdifout.h"
#include "imc/cmd/module_cmd_pcmcvt.h"
#include "imc/cmd/module_cmd_tp.h"
#include "imc/cmd/module_cmd_esoutctrl.h"
#include "imc/cmd/module_cmd_flac.h"
#include "imc/cmd/module_cmd_pcmdec.h"
#include "imc/cmd/module_cmd_dtoctrl.h"
#include "imc/cmd/module_cmd_ctrlsrc.h"
#include "imc/cmd/module_cmd_pcmout.h"
#include "imc/cmd/module_cmd_bypass.h"
#include "imc/cmd/module_cmd_rts.h"
#include "imc/cmd/module_cmd_sola.h"
#include "imc/cmd/module_cmd_inputctrl.h"
#include "imc/cmd/module_cmd_i2s.h"
#include "imc/cmd/module_cmd_reform.h"
#include "imc/cmd/module_cmd_dtsm6.h"
#include "imc/cmd/module_cmd_mp3.h"

#include "imc/evt/module_evt_dec.h"
#include "imc/evt/module_evt_spdifout.h"
#include "imc/evt/module_evt_lipsync.h"
#include "imc/evt/module_evt_tp.h"
#include "imc/evt/module_evt_dtoctrl.h"
#include "imc/evt/module_evt_pcmcvt.h"
#include "imc/evt/module_evt_mixer.h"
#include "imc/evt/module_evt_sif.h"
#include "imc/evt/module_evt_se.h"
#include "imc/evt/module_evt_inputctrl.h"
#include "imc/evt/module_evt_pcmout.h"
#include "imc/evt/module_evt_outctrl.h"
#include "imc/evt/module_evt_hdmi.h"

#include "module/adec_module.h"
#include "module/adec_module_list.h"
#include "module/adec_module_se_param_map.h"


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
SINT32 AUDIO_IMC_Init(void);
void AUDIO_IMC_Finalize(void);
void AUDIO_IMC_Process(void);

SINT32 AUDIO_IMC_SendCmd(int cmd, ADEC_MODULE_ID targetModule);
SINT32 AUDIO_IMC_SendCmdEx(int cmd, ADEC_MODULE_ID targetModule, PFN_ImcResponse cb, void *cbParam);
SINT32 AUDIO_IMC_SendCmdParam(int cmd, ADEC_MODULE_ID targetModule, unsigned int bodySize, void *pParam);
SINT32 AUDIO_IMC_SendCmdRsp(int cmd, ADEC_MODULE_ID targetModule, unsigned int bodySize, void *pParam,
							PFN_ImcResponse rspCb, void *pCbParam);
UINT32 AUDIO_IMC_RegisterEvent(void* pDevInfo, PFN_ImcNoti	pFuncImcNoti, ADEC_MODULE_ID modID, UINT32 event,
						IMC_ACTION_REPEAT_TYPE repeatType, SINT32 notiLevel);
void AUDIO_IMC_CancelEvent(UINT32 event , ADEC_MODULE_ID moduleID, UINT32 actionID);


SINT32 AUDIO_IPC_DBG_Init(void);
void AUDIO_IPC_DBG_Finalize(void);
void AUDIO_IPC_ProcessDebug(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* _ADEC_IMC_FUNC_H_ */
