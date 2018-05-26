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


#ifndef _SDEC_IO_H_
#define _SDEC_IO_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_core.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */



#define CLEAR_ALL_MODE			0xFFFFFFFF


#define DEC_EN				(1 << 31)
#define DL_EN				(1 << 30)
#define TSO_EN				(1 << 29)
#define PID(x)				(x << 16)
#define TPI_IEN				(1 << 15)
#define NO_DSC 				(1 << 14)
#define PAYLOAD_PES			(1 << 12)
#define SF_MAN_EN			(1 << 11)
#define GPB_IRQ_CONF		(1 << 10)
#define TS_DN				(1 << 9)
#define DEST				0x7F

#define AUD_DEV0	0x40
#define AUD_DEV1	0x41
#define VID_DEV0	0x44
#define VID_DEV1	0x45

#define DEST_RESERVED	0x4F

#define MAX_KEY_WORD_IDX	8

#define TVCT_PID	0x1FFB


/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum{
	PCR = 0x00000001,
	TB_DCOUNT 				= 0x00000002,
	BDRC_0 					= 0x00000010,
	BDRC_1 					= 0x00000020,
	BDRC_2 					= 0x00000040,
	BDRC_3 					= 0x00000080,
	ERR_RPT				= 0x00001000,
	GPB_DATA_CHA_GPL 		= 0x00010000,
	GPB_DATA_CHA_GPH 		= 0x00020000,
	GPB_DATA_CHB_GPL 		= 0x00040000,
	GPB_DATA_CHB_GPH 		= 0x00080000,
	GPB_FULL_CHA_GPL 		= 0x00100000,
	GPB_FULL_CHA_GPH 		= 0x00200000,
	GPB_FULL_CHB_GPL 		= 0x00400000,
	GPB_FULL_CHB_GPH 		= 0x00800000,
	TP_INFO_CHA 			= 0x01000000,
	TP_INFO_CHB 			= 0x02000000,
	SEC_ERR_CHA 			= 0x04000000,
	SEC_ERR_CHB 			= 0x08000000,
}E_SDEC_INTR_T;


typedef enum{
	MPG_SD,
	MPG_CC,
	MPG_DUP,
	MPG_TS,
	MPG_PD,
	STCC_DCONT,
	CDIF_RPAGE,
	CDIF_WPAGE,
	CDIF_OVFLOW,
	SB_DROPPED,
	SYNC_LOST,
	TEST_DCONT,
}E_SDEC_ERRINTR_T;

typedef enum{
	 /*00*/E_SDEC_GPB_SIZE_4K,
	 /*01*/E_SDEC_GPB_SIZE_8K,
	 /*02*/E_SDEC_GPB_SIZE_16K,
	 /*03*/E_SDEC_GPB_SIZE_32K,
	 /*04*/E_SDEC_GPB_SIZE_64K,
	 /*05*/E_SDEC_GPB_SIZE_128K,
	 /*06*/E_SDEC_GPB_SIZE_256K,
	 /*07*/E_SDEC_GPB_SIZE_512K,
	 /*08*/E_SDEC_GPB_SIZE_1024K
 }E_SDEC_GPB_SIZE_T;

typedef enum{
	 /*00*/E_SDEC_CHA,
	 /*01*/E_SDEC_CHB,
	 /*03*/E_SDEC_CHA_CHB
 }E_SDEC_CH_T;


//for cdic set
typedef enum{
	/*00*/E_SDEC_CDIC_SRC_CDIN0,
	/*01*/E_SDEC_CDIC_SRC_CDIN1,
	/*02*/E_SDEC_CDIC_SRC_CDIN2,
	/*03*/E_SDEC_CDIC_SRC_CDIN3,
	/*04*/E_SDEC_CDIC_SRC_CDIN4,
	/*05*/E_SDEC_CDIC_SRC_CDIN5,
	/*06*/E_SDEC_CDIC_SRC_UPLOAD0,
	/*07*/E_SDEC_CDIC_SRC_UPLOAD1,
	/*08*/E_SDEC_CDIC_SRC_GPB,
	/*10*/E_SDEC_CDIC_SRC_CDINA = 10,	/* From L9B0. Parallel1 */
 }E_SDEC_CDIC_SRC_T;

//for CDIP set
typedef enum{
	 /*00*/E_SDEC_CDIP_0,
	 /*01*/E_SDEC_CDIP_1,
	 /*02*/E_SDEC_CDIP_2,
	 /*03*/E_SDEC_CDIP_3,
 }E_SDEC_CDIP_IDX_T;

typedef enum{
	 /*00*/E_SDEC_CDIOP_0,
	 /*01*/E_SDEC_CDIOP_1
 }E_SDEC_CDIOP_IDX_T;

typedef enum{
	 /*00*/E_SDEC_CDIOP_VAL_SEL_0,
	 /*01*/E_SDEC_CDIOP_VAL_SEL_1,
	 /*02*/E_SDEC_CDIOP_VAL_SEL_2,
	 /*03*/E_SDEC_CDIOP_VAL_SEL_3
 }E_SDEC_CDIOP_VAL_SEL_T;

typedef enum{
	 /*00*/E_SDEC_CDIOP_SERIAL_0,
	 /*01*/E_SDEC_CDIOP_SERIAL_1
 }E_SDEC_CDIOP_PCONF_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_DISABLE,
	 /*01*/E_SDEC_CDIP_ENABLE
 }E_SDEC_CDIP_EN_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_TEST_DISABLE,
	 /*01*/E_SDEC_CDIP_TEST_ENABLE
 }E_SDEC_CDIP_TEST_EN_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_ERR_ACT_LOW,
	 /*01*/E_SDEC_CDIP_ERR_ACT_HIGH
 }E_SDEC_CDIP_ERR_POL_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_CLK_ACT_HIGH,
	 /*01*/E_SDEC_CDIP_CLK_ACT_LOW
 }E_SDEC_CDIP_CLK_POL_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_VAL_ACT_HIGH,
	 /*01*/E_SDEC_CDIP_VAL_ACT_LOW
 }E_SDEC_CDIP_VAL_POL_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_REQ_ACT_HIGH,
	 /*01*/E_SDEC_CDIP_REQ_ACT_LOW
 }E_SDEC_CDIP_REQ_POL_T;

typedef enum {
	 /*00*/E_SDEC_CDIP_ERR_DISABLE,
	 /*01*/E_SDEC_CDIP_ERR_ENABLE
 }E_SDEC_CDIP_ERR_EN_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_VAL_DISABLE,
	 /*01*/E_SDEC_CDIP_VAL_ENABLE
 }E_SDEC_CDIP_VAL_EN_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_REQ_DISABLE,
	 /*01*/E_SDEC_CDIP_REQ_ENABLE
 }E_SDEC_CDIP_REQ_EN_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_SERIAL_0,
	 /*01*/E_SDEC_CDIP_SERIAL_1,
	 /*02*/E_SDEC_CDIP_PARALLEL_0,
	 /*03*/E_SDEC_CDIP_PARALLEL_1,
 }E_SDEC_CDIP_PCONF_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_BA_CLK_ENABLE,
	 /*01*/E_SDEC_CDIP_BA_CLK_DISABLE
 }E_SDEC_CDIP_BA_CLK_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_BA_VAL_ENABLE,
	 /*01*/E_SDEC_CDIP_BA_VAL_DISABLE
 }E_SDEC_CDIP_BA_VAL_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_BA_SOP_ENABLE,
	 /*01*/E_SDEC_CDIP_BA_SOP_DISABLE
 }E_SDEC_CDIP_BA_SOP_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_47DETECTION,
	 /*01*/E_SDEC_CDIP_NEG_SOP,
	 /*02*/E_SDEC_CDIP_POS_SOP,
	 /*03*/E_SDEC_CDIP_BOTH_SOP
 }E_SDEC_CDIP_SYNC_TYPE_T;

typedef enum{
	 /*00*/E_SDEC_CDIP_ATSC,
	 /*01*/E_SDEC_CDIP_OC,
	 /*02*/E_SDEC_CDIP_ARIB,
	 /*03*/E_SDEC_CDIP_DVB,
	 /*10*/E_SDEC_CDIP_PES = 10,
	 /*04*/E_SDEC_CDIP_MPES
 }E_SDEC_CDIP_DTYPE_T;
//end of CDIP set




#if 0
typedef struct{
	UINT8 ui8Channel;
	UINT16 ui16StatusInfo;
	UINT8 ui8index;
}S_GPB_DATA_T;
#endif



typedef enum{
	/* SD IO */
	E_SDEC_REGBACKUP_IO_CDIP0 = 0,		/* 0x00 */
	E_SDEC_REGBACKUP_IO_CDIP1,			/* 0x04 */
	E_SDEC_REGBACKUP_IO_CDIP2, 			/* 0x08 */
	E_SDEC_REGBACKUP_IO_CDIP3, 			/* 0x0C */
	E_SDEC_REGBACKUP_IO_CDIOP0, 			/* 0x10 */
	E_SDEC_REGBACKUP_IO_CDIOP1, 			/* 0x14 */
	E_SDEC_REGBACKUP_IO_CDIC_DSC0,		/* 0x28 */
	E_SDEC_REGBACKUP_IO_CDIC_DSC1, 		/* 0x2C */
	E_SDEC_REGBACKUP_IO_INTR_EN,			/* 0x100 */
	E_SDEC_REGBACKUP_IO_ERR_INTR_EN,	/* 0x110 */
	E_SDEC_REGBACKUP_IO_GPB_BASE,		/* 0x120 */
	E_SDEC_REGBACKUP_IO_CDIC2,			/* 0x12C */

	E_SDEC_REGBACKUP_IO_NUM,
}E_SDEC_REG_IO_BACKUP_T;

typedef enum{
	/* SD CORE */
	E_SDEC_REGBACKUP_CORE_EXT_CONF,			/* 0x10 */
	E_SDEC_REGBACKUP_CORE_TPI_ICONF, 		/* 0x40 */
	E_SDEC_REGBACKUP_CORE_KMEM_ADDR, 		/* 0x98 */
	E_SDEC_REGBACKUP_CORE_KMEM_DATA,		/* 0x9C */
	E_SDEC_REGBACKUP_CORE_SECF_EN_L, 		/* 0xB0 */
	E_SDEC_REGBACKUP_CORE_SECF_EN_H,		/* 0xB4 */
	E_SDEC_REGBACKUP_CORE_SECF_MTYPE_L,		/* 0xB8 */
	E_SDEC_REGBACKUP_CORE_SECF_MTYPE_H,		/* 0xBC */
	E_SDEC_REGBACKUP_CORE_SECFB_VALID_L, 	/* 0xC0 */
	E_SDEC_REGBACKUP_CORE_SECFB_VALID_H, 	/* 0xC4 */

	E_SDEC_REGBACKUP_CORE_NUM,
}E_SDEC_REG_CORE_BACKUP_T;

typedef struct {
	BOOLEAN	bInit;
	UINT32 	regIO[E_SDEC_REGBACKUP_IO_NUM];
	UINT32 	regCORE[E_SDEC_REGBACKUP_CORE_NUM];
}S_REG_BACKUP_LIST_T;

typedef enum{
	E_SDEC_RESET_MODE_ENABLE = 0,			/* enable reset */
	E_SDEC_RESET_MODE_DISABLE,				/* disable reset */
	E_SDEC_RESET_MODE_ONETIME,				/* 1 time reset mode */
}E_SDEC_RESET_MODE_T;

typedef enum{
	E_SDEC_DN_OUT_FROM_CORE_0 = 0,			/* Download of CORE0 */
	E_SDEC_DN_OUT_FROM_SENC,				/* Download of SENC */
	E_SDEC_DN_OUT_FROM_CORE_1,				/* Download of CORE1 */
}E_SDEC_TOP_DN_SEL_T;



/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

int SDEC_IO_Init(S_SDEC_PARAM_T *stpSdecParam);

int SDEC_IO_DebugCommand(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_DBG_CMD_T *stpLXSdecDbgCmd);
int SDEC_IO_GetCurrentSTCPCR(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_STC_PCR_T *stpLXSdecGetSTCPCR);
int SDEC_IO_GetLiveSTC(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_STC_T *stc);

int SDEC_IO_GetCurrentGSTC(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_GSTC_T *stpLXSdecGetGSTC);
int SDEC_IO_GetCurrentGSTC32(S_SDEC_PARAM_T *stpSdecParam,	LX_SDEC_GET_GSTC32_T *stpLXSdecGetGSTC);
int SDEC_IO_SetCurrentGSTC(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_GSTC_T *stpLXSdecSetGSTC);
int SDEC_IO_SelInputPort(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_T *stpLXSdecSelPort);
int SDEC_IO_CfgInputPort(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CFG_INPUT_PARAM_T *stpLXSdecCfgPortParam);
int SDEC_IO_SelParInput(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_PAR_INPUT_T *stpLXSdecParInput);

int SDEC_IO_SelInputSource(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_SRC_T *inputSrc);
int SDEC_IO_GetInputSource(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_SRC_T *inputSrc);

int SDEC_IO_SelCiInput(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_CI_INPUT_T *stpLXSdecParInput);
int SDEC_IO_GetParInput(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_PAR_INPUT_T *stpLXSdecParInput);
int SDEC_IO_GetCiInput(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_CI_INPUT_T *stpLXSdecCiInput);


int SDEC_IO_GetInputPort(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_INPUT_T *stpLXSdecGetInputPort);
int SDEC_IO_SetVidOutport(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_VDEC_PORT_T *stpLXSdecSetVidOutort);
int SDEC_IO_InputPortEnable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_ENABLE_INPUT_T *stpLXSdecEnableInput);
int SDEC_IO_SelectPVRSource(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_DL_SEL_T *stpLXSdecDlSel);


int SDEC_IO_EnableLog(S_SDEC_PARAM_T *stpSdecParam, UINT32 *pulArg);
int SDEC_IO_GetRegister(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_REG_T *stpLXSdecReadRegisters);
int SDEC_IO_SetRegister(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_REG_T *stpLXSdecWriteRegisters);

UINT32* SDEC_IO_GetRegBaseAddr(LX_SDEC_BLOCK_T block);
UINT32 SDEC_IO_GetRegSize(LX_SDEC_BLOCK_T block);


//SDEC inner API
int SDEC_TPI_Set(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch);
int SDEC_ERR_Intr_Set(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch);

int SDEC_InputSet(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_T *stpLXSdecSelPort);

int SDEC_InputPortEnable(UINT8 core, LX_SDEC_INPUT_T eInputPath, UINT8 en);
int SDEC_InputPortReset(UINT8 ui8Ch);
int SDEC_SDMWCReset(UINT8 ui8Ch);
int SDEC_TEAH_Reset(UINT8 core);

void SDEC_TPIIntr(struct work_struct *work);
int SDEC_InputPortReset(UINT8 ui8Ch);


// STCC Related
int SDEC_IO_GetSTCCASGStatus(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_STCCASG_T *stpLXSdecGetSTCCASG);
int SDEC_IO_SetSTCCASGStatus(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_STCCASG_T *stpLXSdecSetSTCCASG);
int SDEC_IO_GetSTCCStatus(S_SDEC_PARAM_T *stpSdecParam,	LX_SDEC_GET_STCC_STATUS_T *stpLXSdecGetSTCCStatus);


#if 1 /* TS Out Stub */
int SDEC_IO_SerialTSOUT_SetSrc(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SERIALTSOUT_SET_SOURCE_T *stpLXSdecSTSOutSetSrc);
int SDEC_IO_BDRC_SetData(S_SDEC_PARAM_T *stpSdecParam,	LX_SDEC_BDRC_T *stpLXSdecBDRCSetData);
int SDEC_IO_BDRC_Enable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_BDRC_ENABLE_T *stpLXSdecBDRCEnable);
int SDEC_IO_SerialTSOUT_SetBufELevel(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SERIALTSOUT_SET_BUFELEV_T *stpLXSdecSTSOutSetBufELev);
int SDEC_IO_CfgOutputPort(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CFG_OUTPUT_T *stpLXSdecCfgPort);
int SDEC_IO_BDRC_SetWritePtrUpdated(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_BDRC_WPTRUPD_T *stpLXSdecBDRCSetWptrUpd);
int SDEC_IO_CfgIOPort(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CFG_IO_T *stpLXSdecCfgIOPort);
#endif /* End of TS Out Stub */

int SDEC_IO_Suspend(S_SDEC_PARAM_T *stpSdecParam);
int SDEC_IO_Resume(S_SDEC_PARAM_T *stpSdecParam);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_IO_H_ */

/** @} */
