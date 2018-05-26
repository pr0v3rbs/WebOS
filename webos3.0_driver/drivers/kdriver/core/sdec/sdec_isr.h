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


#ifndef _SDEC_ISR_h
#define _SDEC_ISR_h

#ifdef __cplusplus
extern "C" {
#endif

//intr source
#define PCR 0x00000001
#define TB_DCOUNT 0x00000002
#define BDRC_0 0x00000010
#define BDRC_1 0x00000020
#define BDRC_2 0x00000040
#define BDRC_3 0x00000080
#define ERR_RPT 0x00001000
#define GPB_DATA_CHA_GPL 0x00010000
#define GPB_DATA_CHA_GPH 0x00020000
#define GPB_DATA_CHB_GPL 0x00040000
#define GPB_DATA_CHB_GPH 0x00080000
#define GPB_FULL_CHA_GPL 0x00100000
#define GPB_FULL_CHA_GPH 0x00200000
#define GPB_FULL_CHB_GPL 0x00400000
#define GPB_FULL_CHB_GPH 0x00800000
#define TP_INFO_CHA 0x01000000
#define TP_INFO_CHB 0x02000000
#define SEC_ERR_CHA 0x04000000
#define SEC_ERR_CHB 0x08000000


//TP Info
#define AFE 0x00000001
#define TPDF 0x00000002
#define SPF 0x00000004
#define ESPI 0x00000008
#define RAI 0x00000010
#define DI 0x00000020
#define TSC 0x000000C0
#define TPRI 0x00000100
#define PUSI 0x00000200
#define TEI 0x00000400
#define TSC_CHG 0x00001000
#define AUTO_SC_CHK 0x00002000
//#define PIDF_IDX 0x00001000


// TP Info Interrupt Buffer Size
#define TPI_ITR_BUF_SIZE 0x4
#define SEC_ERR_ITR_BUF_SIZE 	0x4

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
   enum
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
   structre
----------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------
   API
----------------------------------------------------------------------------------------*/
//void SDEC_ISR_Handler(S_SDEC_PARAM_T *stpSdecParam);
//void SDEC_CORE1_ISR_Handler(S_SDEC_PARAM_T *stpSdecParam);
//DTV_STATUS_T SDEC_ErrIntrCheck(UINT8 core, S_SDEC_PARAM_T *stpSdecParam);
//DTV_STATUS_T SDEC_TPInfoIntrCheck(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch);

void SDEC_PollThread(void *param);

int		SDEC_ISR_Init(S_SDEC_PARAM_T *stpSdecParam);
void	SDEC_ISR_Cleanup(S_SDEC_PARAM_T *stpSdecParam);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

