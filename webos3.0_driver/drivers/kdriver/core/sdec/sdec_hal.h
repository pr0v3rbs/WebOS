/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef _SDEC_HAL_H
#define _SDEC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sdec_kapi.h"
#include "sdec_io.h"

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define LX_SDEC_MAX_NUM_OF_CORE	2

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

#define LX_SDEC_HAL_CHECK_CH(cfg, ch) \
	SDEC_CHECK_ERROR(ch >= cfg->nChannel, goto exit , "Over channel range[ch:%d]", ch)

#define LX_SDEC_HAL_CHECK_PARAM(x) \
	SDEC_CHECK_ERROR(x == NULL, goto exit , "parameter is NULL!\n")

#define LX_SDEC_HAL_CHECK_RET(x) \
	SDEC_CHECK_ERROR(x != 0, goto exit, "Error! [E = %d]\n", x)


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

enum
{
	SDEC_HAL_DISABLE = 0,
	SDEC_HAL_ENABLE = 1,
};

typedef enum{
	 SDEC_HAL_CDIOP_SERIAL_0		= 0x0,
	 SDEC_HAL_CDIOP_SERIAL_1		= 0x1
 }SDEC_HAL_CDIOP_PCONF_T;

typedef enum{
	 SDEC_HAL_CDIP_SERIAL_0		= 0x0,
	 SDEC_HAL_CDIP_SERIAL_1		= 0x1,
	 SDEC_HAL_CDIP_PARALLEL_0	= 0x2,
	 SDEC_HAL_CDIP_PARALLEL_1	= 0x3
 }SDEC_HAL_CDIP_PCONF_T;

typedef enum{
	 SDEC_HAL_CDIP_47DETECTION	= 0x0,
	 SDEC_HAL_CDIP_NEG_SOP		= 0x1,
	 SDEC_HAL_CDIP_POS_SOP		= 0x2,
	 SDEC_HAL_CDIP_BOTH_SOP		= 0x3
 }SDEC_HAL_CDIP_SYNC_TYPE_T;

typedef enum{
	SDEC_HAL_CDIP_MPEG2TS		= 0x0,
	SDEC_HAL_CDIP_VIDEOSTREAM	= 0xA,
	SDEC_HAL_CDIP_MUXEDSTREAM	= 0xB
 }SDEC_HAL_CDIP_DTYPE_T;

enum {
	SDEC_HAL_CDIP_SERIAL = 0,
	SDEC_HAL_CDIP_PARALLEL = 1,
};

enum {
	SDEC_HAL_CDIP_CLK_ACT_HIGH 	= 0,
	SDEC_HAL_CDIP_CLK_ACT_LOW 	= 1,
};

enum {
	SDEC_HAL_CDIP_VAL_DISABLE 	= 0,
	SDEC_HAL_CDIP_VAL_ENABLE		= 1,
};

enum {
	SDEC_HAL_CDIOP_IN = 0,
	SDEC_HAL_CDIOP_OUT = 1,
};

enum {
	SDEC_HAL_CDIP_FIRST = 0,
	SDEC_HAL_CDIP_FOLLOWER = 1,
};

enum {
	SDEC_HAL_TS_INPUT_EXTERNAL = 0,
	SDEC_HAL_TS_INPUT_INTERNAL,
	SDEC_HAL_TS_INPUT_NUM
};

/** TS_SRC_SEL(demod_sel) **/
enum {
	SDEC_HAL_IN_EXT_DEMOD 		= 0,
	SDEC_HAL_IN_EXT_PARALLEL0	= 1,
	SDEC_HAL_IN_EXT_PARALLEL1	= 2,
	SDEC_HAL_IN_EXT_SERIAL		= 3,
	SDEC_HAL_IN_EXT_CI_OUTPUT	= 4,	/* TPI. input from CICAM */
	SDEC_HAL_IN_EXT_CI_INPUT	= 5,	/* TPIO. inout to CICAM */
	SDEC_HAL_IN_EXT_NUM
};

enum {
	SDEC_HAL_IN_INT_XXX = 0,

	SDEC_HAL_IN_INT_NUM =4
};


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *  structre
 *----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *  API
 *----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

int SDEC_HAL_TOPInit(void);
int SDEC_HAL_IOInit(void);
int SDEC_HAL_MPGInit(void);
/* HAL for TOP register */
int SDEC_HAL_SetVideoOut(UINT8 idx, UINT8 sel);
int SDEC_HAL_SetPVRSrc(UINT8 idx, UINT8 sel);
int SDEC_HAL_SetAudioOut(UINT8 idx, UINT8 sel);
int SDEC_HAL_SetUploadSrc(UINT8 idx, UINT8 sel);
int SDEC_HAL_SetOutputSrcCore(UINT8 idx, UINT8 sel);
int SDEC_HAL_SetTsSelector(UINT8 core, UINT8 port, UINT8 main_src, UINT8 sub_src);
int SDEC_HAL_GetTsSelector(UINT8 core, UINT8 port, UINT8 *main_src, UINT8 *sub_src);



/* HAL for IO register */
int SDEC_HAL_SetGPBBaseAddr(UINT8 core, UINT8 gpb_base);
UINT32 SDEC_HAL_GetGPBBaseAddr(UINT8 core);
UINT32 SDEC_HAL_GetSTCCErrorCtrl(UINT8 core, UINT8 idx);
int SDEC_HAL_SetSTCCErrorCtrl(UINT8 core, UINT8 idx, UINT32 err_max, UINT8 en);
int SDEC_HAL_SDMWCLastBValidMode(UINT8 core, UINT8 en);
int SDEC_HAL_EnableVideoReady(UINT8 core, UINT8 idx, UINT8 en);
int SDEC_HAL_EnableAudioReady(UINT8 core, UINT8 idx, UINT8 en);
int SDEC_HAL_EnableAutoIncr(UINT8 core, UINT8 en);
int SDEC_HAL_SetTs2PesPid(UINT8 core, UINT16 pid);
UINT16 SDEC_HAL_GetTs2PesPid(UINT8 core);
UINT32 SDEC_HAL_IntrStatReadAndClear(UINT8 core);
UINT32 SDEC_HAL_GetErrorInterruptStat(UINT8 core);
int SDEC_HAL_EnableInterrupt(UINT8 core, E_SDEC_INTR_T eSdecIntrSrc, UINT8 en);
int SDEC_HAL_EnableErrorInterrupt(UINT8 core, E_SDEC_ERRINTR_T eSdecErrIntr, UINT8 val);
int SDEC_HAL_SDMWCReset(UINT8 core, UINT32 val);
UINT32 SDEC_HAL_SDMWCGet(UINT8 core);
UINT32 SDEC_HAL_SDMWCGetStatus(UINT8 core);
UINT32 SDEC_HAL_STCCGetASG(UINT8 core);
UINT8 SDEC_HAL_STCCGetMain(UINT8 core);
int SDEC_HAL_STCCSetMain(UINT8 core, UINT8 ch);
int SDEC_HAL_STCCSetVideoAssign(UINT8 core, UINT8 idx, UINT8 ch);
int SDEC_HAL_STCCSetAudioAssign(UINT8 core, UINT8 idx, UINT8 ch);
UINT32 SDEC_HAL_STCCGetReg(UINT8 core, UINT8 ch);
int SDEC_HAL_STCCSetReg(UINT8 core, UINT8 ch, UINT32 val);
int SDEC_HAL_STCCSetSTC(UINT8 core, UINT8 ch, UINT32 val);

int SDEC_HAL_STCCGetLiveSTC(UINT8 core, UINT8 ch, UINT32 *stcc_41_10, UINT32 *stcc_9_0);
int SDEC_HAL_STCCGetLatchedSTC(UINT8 core, UINT8 ch, UINT32 *stcc_41_10, UINT32 *stcc_9_0);
int SDEC_HAL_STCCGetPCR(UINT8 core, UINT8 ch, UINT32 *pcr_41_10, UINT32 *pcr_9_0);

int SDEC_HAL_STCCEnable(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_STCCReset(UINT8 core, UINT8 ch);
int SDEC_HAL_STCCSetCh(UINT8 core, UINT8 ch, UINT8 chan);
int SDEC_HAL_STCCEnableCopy(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_STCCEnableLatch(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_AVSTCReset(UINT8 core, UINT8 ch);
int SDEC_HAL_STCCSetPCRPid(UINT8 core, UINT8 ch, UINT16 pid);
int SDEC_HAL_STCCSetSubStccRate(UINT8 core, UINT32 val);
UINT32 SDEC_HAL_STCCGetSubStccRate(UINT8 core);
int SDEC_HAL_GSTC(UINT8 core, UINT32 *pStcc_41_10, UINT32 *pStcc_9_0);
int SDEC_HAL_GSTC1(UINT8 core, UINT32 *pStcc_bs_32_1, UINT32 *pStcc_bs_0);
int SDEC_HAL_GSTCReset(UINT8 core);
int SDEC_HAL_GSTCSetValue(UINT8 core, UINT8 index, UINT32 val);
int SDEC_HAL_DSCSetCasType(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_DSCSetBlkMode(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_DSCSetResMode(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_DSCSetKeySize(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_DSCEnablePESCramblingCtrl(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_DSCSetEvenMode(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_DSCSetOddMode(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_SetParallelInput(UINT8 core, UINT8 isExternal);
int SDEC_HAL_CDIPEnable(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_CIDCReset(UINT8 core, UINT8 ch);
int SDEC_HAL_CIDCMinSyncByteDetection(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_CIDCMaxSyncByteDrop(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_CIDCSetSrc(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_CIDCRead(UINT8 core, UINT8 ch, UINT32 *pVal);
int SDEC_HAL_CIDCGetStatus(UINT8 core, UINT8 ch, CDIC *pCdic);
UINT8 SDEC_HAL_CIDCGetCdif_OverFlow(UINT8 core, UINT8 ch);
UINT8 SDEC_HAL_CIDCGetCdif_Full(UINT8 core, UINT8 ch);
UINT32 SDEC_HAL_CIDCGet(UINT8 core, UINT8 ch);
int SDEC_HAL_CIDC3DlConf(UINT8 core, UINT8 ch, UINT8 val);
int SDEC_HAL_CDIP(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg);
int SDEC_HAL_CDIOP(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg);
int SDEC_HAL_CDIOPEnable(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_CDIPAEnable(UINT8 core, UINT8 ch, UINT8 en);
int SDEC_HAL_CDIPA(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg);

/* ~ H13A0 CDIC2 PID Filter Related */
int SDEC_HAL_EnableCDIC2PIDFWrite(UINT8 core, UINT8 en);
int SDEC_HAL_CDIC2PIDFSetPidfData(UINT8 core, UINT8 idx, UINT32 val);
UINT32 SDEC_HAL_CDIC2PIDFGetPidfData(UINT8 core, UINT8 idx);
int SDEC_HAL_CDIC2PIDFEnablePidFilter(UINT8 core, UINT8 idx, UINT8 en);
UINT8 SDEC_HAL_CDIC2GetPIDFEnable(UINT8 core, UINT8 idx);
int SDEC_HAL_CDIC2DlExtConf(UINT8 core, UINT8 ch, UINT8 val);

/* CDIPn_2nd Configuration for using external original clock */
int SDEC_HAL_CDIPn_2ND(UINT8 core, UINT8 port, LX_SDEC_CFG_INPUT_T *cfg_input);

/* HAL for MPEG register */

TPI_ISTAT SDEC_HAL_TPIGetIntrStat(UINT8 ch);
SE_ISTAT SDEC_HAL_SEGetIntrStat(UINT8 ch);
int SDEC_HAL_ConfSetPESReadyCheck(UINT8 ch, UINT8 chk_ch, UINT8 val);
CHAN_STAT SDEC_HAL_GetChannelStatus(UINT8 ch);
UINT32 SDEC_HAL_GetChannelStatus2(UINT8 ch);
UINT8 SDEC_HAL_ChanStatGetMWFOverFlow(UINT8 ch);
UINT32 SDEC_HAL_CCCheckEnableGet(UINT8 ch, UINT8 idx);
int SDEC_HAL_CCCheckEnableSet(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_ExtConfSECIDcont(UINT8 ch, UINT8 val);
int SDEC_HAL_ExtConfSECICCError(UINT8 ch, UINT8 val);
int SDEC_HAL_ExtConfVideoDupPacket(UINT8 ch, UINT8 val);
int SDEC_HAL_ExtConfDcontDupPacket(UINT8 ch, UINT8 val);
int SDEC_HAL_ExtConfGPBOverWrite(UINT8 ch, UINT8 en);
int SDEC_HAL_ExtConfGPBFullLevel(UINT8 ch, UINT32 val);
int SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(UINT8 ch, UINT8 val);
int SDEC_HAL_TPISetIntrAutoScCheck(UINT8 ch, UINT8 val);
int SDEC_HAL_GPBSetFullIntr(UINT8 ch, UINT8 loc);
int SDEC_HAL_GPBClearFullIntr(UINT8 ch, UINT8 loc);
UINT32 SDEC_HAL_GPBGetDataIntrStat(UINT8 ch, UINT8 isHigh);
UINT32 SDEC_HAL_GPBGetFullIntrStat(UINT8 ch, UINT8 idx);
UINT32 SDEC_HAL_GPBGetWritePtr(UINT8 ch, UINT8 idx);
UINT32 SDEC_HAL_GPBGetReadPtr(UINT8 ch, UINT8 idx);
int SDEC_HAL_GPBSetReadPtr(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_GPBSetWritePtr(UINT8 ch, UINT8 idx, UINT32 val);
UINT32 SDEC_HAL_GPBGetLowerBnd(UINT8 ch, UINT8 idx);
UINT32 SDEC_HAL_GPBGetUpperBnd(UINT8 ch, UINT8 idx);
int SDEC_HAL_GPBSetBnd(UINT8 ch, UINT8 idx, UINT32 l_bnd, UINT32 u_bnd);

UINT32 SDEC_HAL_SECFGetLinkedMap(UINT8 ch, UINT8 idx);
UINT32 SDEC_HAL_SECFGetMap(UINT8 ch, UINT8 idx);
int SDEC_HAL_SECFSetMap(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_SECFSetMapBit(UINT8 ch, UINT8 idx, UINT8 loc);
int SDEC_HAL_SECFClearMapBit(UINT8 ch, UINT8 idx, UINT8 loc);
UINT32 SDEC_HAL_SECFGetEnable(UINT8 ch, UINT8 idx);
UINT8 SDEC_HAL_SECFGetEnableBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFSetEnable(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_SECFSetEnableBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFClearEnableBit(UINT8 ch, UINT8 loc);
UINT32 SDEC_HAL_SECFGetBufValid(UINT8 ch, UINT8 idx);
int SDEC_HAL_SECFSetBufValid(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_SECFSetBufValidBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFClearBufValidBit(UINT8 ch, UINT8 loc);
UINT32 SDEC_HAL_SECFGetMapType(UINT8 ch, UINT8 loc);
UINT8 SDEC_HAL_SECFGetMapTypeBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFSetMapType(UINT8 ch, UINT8 idx, UINT32 val);
int SDEC_HAL_SECFSetMapTypeBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFClearMapTypeBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFSetCRCBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_SECFClearCRCBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_PIDFSetCRCBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_PIDFClearCRCBit(UINT8 ch, UINT8 loc);
int SDEC_HAL_PIDFSetPidfData(UINT8 ch, UINT8 idx, UINT32 val);
UINT32 SDEC_HAL_PIDFGetPidfData(UINT8 ch, UINT8 idx);
int SDEC_HAL_PIDFEnable(UINT8 ch, UINT8 idx, UINT8 en);
int SDEC_HAL_PIDFScrambleCheck(UINT8 ch, UINT8 idx, UINT8 en);
int SDEC_HAL_PIDFDescEnable(UINT8 ch, UINT8 idx, UINT8 en);
int SDEC_HAL_PIDFDownloadEnable(UINT8 ch, UINT8 idx, UINT8 en);
int SDEC_HAL_SECFSetSecfData(UINT8 ch, UINT8 secf_idx, UINT8 word_idx, UINT32 val);
UINT32 SDEC_HAL_SECFGetSecfData(UINT8 ch, UINT8 secf_idx, UINT8 word_idx);
int SDEC_HAL_KMEMSet(UINT8 ch, UINT8 key_type, UINT8 pid_idx, UINT8 odd_key, UINT8 word_idx, UINT32 val);
UINT32 SDEC_HAL_KMEMGet(UINT8 ch, UINT8 key_type, UINT8 pid_idx, UINT8 odd_key, UINT8 word_idx);



/* TS Out Stub */
UINT32 SDEC_HAL_CDIOP_GetStatus(UINT8 core, UINT8 idx);
int SDEC_HAL_BDRC_Enable(UINT8 core, UINT8 idx, UINT8 en);
int SDEC_HAL_BDRC_Reset(UINT8 core, UINT8 idx);
int SDEC_HAL_BDRC_SetWptrUpdate(UINT8 core, UINT8 idx);
int SDEC_HAL_BDRC_EnableWptrAutoUpdate(UINT8 core, UINT8 idx, UINT8 en);
int SDEC_HAL_BDRC_SetGPBChannel(UINT8 core, UINT8 idx, UINT8 gpb_chan);
int SDEC_HAL_BDRC_SetGPBIndex(UINT8 core, UINT8 idx, UINT8 gpb_idx);
int SDEC_HAL_BDRC_SetQlen(UINT8 core, UINT8 idx, UINT8 q_len);
int SDEC_HAL_BDRC_SetDtype(UINT8 core, UINT8 idx, UINT8 dtype);
int SDEC_HAL_BDRC_SetBufferEmptyLevel(UINT8 core, UINT8 idx, UINT32 buf_e_lev);
int SDEC_HAL_CDOC_Reset(UINT8 core, UINT8 idx);
int SDEC_HAL_CDOC_SetSrc(UINT8 core, UINT8 idx, UINT8 src);
int SDEC_HAL_CDOC_SetTsoSrc(UINT8 core, UINT8 idx, UINT8 tso_src);
int SDEC_HAL_CDOC_SetIDMask(UINT8 core, UINT8 idx, UINT8 id_mask);
int SDEC_HAL_CDOC_SetIDMsb(UINT8 core, UINT8 idx, UINT8 id_msb);
int SDEC_HAL_CDOC_SetIDValue(UINT8 core, UINT8 idx, UINT8 id_value);
int SDEC_HAL_CDIOP_SetOut(UINT8 core, UINT8 idx,	LX_SDEC_CFG_OUTPUT_T *pCfg);
/* End of TS Out Stub */


int SDEC_HAL_SetGSTCClkSource(UINT8 core, UINT8 gstc_index, UINT8 fixed_sysclk);
int SDEC_HAL_SetTimeStampClkSource(UINT8 core, UINT8 fixed_sysclk);



#ifdef __cplusplus
}
#endif

#endif /* _SDEC_HAL_H */

