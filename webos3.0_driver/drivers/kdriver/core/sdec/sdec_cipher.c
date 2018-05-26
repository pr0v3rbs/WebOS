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


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_hal.h"
#include "sdec_cipher.h"

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
 *   External Variables
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static UINT32 gh13_mcu_ipc;


/**
 * Initialize SDEC CIPHER
 *
 * @param	stpSdecParam	[in	] sdec param
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 * @see
 */

int SDEC_CIPHER_Init(S_SDEC_PARAM_T *stpSdecParam)
{
	/* jinhwan.bae for mcu descrambler mode , H13 Only */
	/* ioremap ipc base */
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		gh13_mcu_ipc = (UINT32)ioremap(0xC0004800, 0x100);
	}

	return RET_OK;
}



 /**
 ********************************************************************************
 * @brief
 *	 set esa mode.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_IO_SetCipherMode
	 (S_SDEC_PARAM_T *stpSdecParam,
	 LX_SDEC_CIPHER_MODE_T *stpLXSdecCipherMode)
 {
	 int eRet = RET_ERROR;
	 UINT8	 lxch;
	 UINT8	 core, ch;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherMode == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	 OS_LockMutex(&stpSdecParam->stSdecMutex);

	 SDEC_PRINT_TRACE( "<--SDEC_IO_SetCipherMode");

	 lxch = stpLXSdecCipherMode->eCh;

	 /* argument check */
	 LX_SDEC_CHECK_CODE(SDEC_CFG_IsNormalChannel(lxch) == 0, goto exit, "invalid channel[%d]", lxch);

	 SDEC_LXCH_TO_CORE_CH(lxch, core, ch);

	 SDEC_HAL_DSCSetCasType(core, ch, stpLXSdecCipherMode->eCasType);
	 SDEC_HAL_DSCSetBlkMode(core, ch, stpLXSdecCipherMode->eBlkMode);
	 SDEC_HAL_DSCSetResMode(core, ch, stpLXSdecCipherMode->eResMode);
	 SDEC_HAL_DSCSetKeySize(core, ch, stpLXSdecCipherMode->eKeySize);
	 SDEC_HAL_DSCEnablePESCramblingCtrl(core, ch, stpLXSdecCipherMode->ePSCEn);
	 SDEC_HAL_DSCSetEvenMode(core, ch, 0x2);
	 SDEC_HAL_DSCSetOddMode(core, ch, 0x3);

	 SDEC_PRINT_DESC( "SDEC_IO_SetCipherMode : Ch[%d] Type(3:AES)[%d]", lxch, stpLXSdecCipherMode->eCasType);
	 SDEC_PRINT_TRACE( "-->SDEC_IO_SetCipherMode");

	 eRet = RET_OK;

 exit:
	 OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	 return (eRet);
 }


 /**
 ********************************************************************************
 * @brief
 *	 set Cipher key.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 static UINT32 gWRCNT = 0;
#define SDEC_MAX_KEY_LEN	32
 int SDEC_IO_SetCipherKey
	 (S_SDEC_PARAM_T *stpSdecParam,
	 LX_SDEC_CIPHERKEY_T *stpLXSdecCipherKey)
 {
	 int eRet = RET_ERROR;
	 UINT8	 ui8count = 0x0;
	 UINT8	 key_type = 0, pid_idx = 0, odd_key = 0;
	 UINT32  *ui32pAddr;
	 UINT32  ipc_addr = 0;
	 UINT32  ui32rdcnt = 0;
	 UINT32  ui32mcutimeout = 0;
	 LX_SDEC_CFG_T *pSdecConf = NULL;
	 UINT8	 key_buf[SDEC_MAX_KEY_LEN];
	 int	 err = 0;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey->puiCipherKey == NULL, return RET_INVALID_PARAMS, "Invalid argument key mem is null" );
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey->uiKeySize > MAX_KEY_WORD_IDX, return RET_INVALID_PARAMS, "Invalid argument uiKeySize [%d]", stpLXSdecCipherKey->uiKeySize);

	 /* argument check */
	 pSdecConf = SDEC_CFG_GetConfig();
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey->eCh >= (LX_SDEC_CH_T)(pSdecConf->nChannel), return RET_INVALID_PARAMS, "over channel range %d", stpLXSdecCipherKey->eCh);

	 OS_LockMutex(&stpSdecParam->stSdecMutex);

	 SDEC_PRINT_DESC( "<--SDEC_IO_SetCipherKey Ch[%d] idx[%d] eKeyMode[%d] key[0x%x] uiKeySize(word)[%d]",
					 stpLXSdecCipherKey->eCh, stpLXSdecCipherKey->uiPidFltId, stpLXSdecCipherKey->eKeyMode, (UINT32)(stpLXSdecCipherKey->puiCipherKey), stpLXSdecCipherKey->uiKeySize);

	 memset(key_buf, 0x00, SDEC_MAX_KEY_LEN);
	 err = copy_from_user(key_buf, stpLXSdecCipherKey->puiCipherKey, (stpLXSdecCipherKey->uiKeySize)*4);
	 LX_SDEC_CHECK_CODE( err != RET_OK, goto func_exit, "copy_from_user error \n");

	 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN || stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD)//Key type :even or ODD
	 {
		 key_type = 0x0;
		 pid_idx =	stpLXSdecCipherKey->uiPidFltId;
		 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN)
		 {
			 odd_key = 0x0;
		 }
		 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD)
		 {
			 odd_key = 0x1;
		 }

		 ui8count = 0x0; //by skpark
	 }
	 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD_IV || stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN_IV)//Key type :IV
	 {
		 key_type = 0x1;
		 pid_idx =	stpLXSdecCipherKey->uiPidFltId;
		 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN_IV)
		 {
			 odd_key = 0x0;
		 }
		 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD_IV)
		 {
			 odd_key = 0x1;
		 }

		 ui8count = 0x0; //by skpark
	 }
	 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_MULTI2)
	 {
		 key_type = 0x02;
		 ui8count = 0x0;			 //by skpark
	 }

	 // ui32pAddr = (UINT32 *)stpLXSdecCipherKey->puiCipherKey;
	 ui32pAddr = (UINT32 *)key_buf;

	 /* jinhwan.bae mcu test */
	 if( (stpSdecParam->ui8McuDescramblerCtrlMode == 1) && (stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN || stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD) )
	 {
		 /* Only JP Platform Process following Routine */
		 SDEC_PRINT_DESC( "SDEC_IO_SetCipherKey with IPC : Ch[%d] idx[%d] eKeyMode[%d]", stpLXSdecCipherKey->eCh, stpLXSdecCipherKey->uiPidFltId, stpLXSdecCipherKey->eKeyMode);

		 /* H13 Only */
		 if (lx_chip_rev() < LX_CHIP_REV(M14, A0))
		 {
			 ui32rdcnt = SDEC_READ32(gh13_mcu_ipc + 0x14);

			 ui32mcutimeout = msecs_to_jiffies(500) + jiffies;
			 while(ui32rdcnt != gWRCNT)
			 {
				 ui32rdcnt = SDEC_READ32(gh13_mcu_ipc + 0x14);
				 SDEC_PRINT_DESC( "Infinite Loop in Key Set RdCnt[%d] WrCnt[%d]", ui32rdcnt, gWRCNT);
				 if(jiffies > ui32mcutimeout)
				 {
					 SDEC_PRINT_DESC( "Infinite Loop Timeout in Key Set RdCnt[%d] WrCnt[%d]", ui32rdcnt, gWRCNT);
					 break;
				 }
			 }

			 {
				 /* set ipc */
				 ipc_addr = 0;
				 SDEC_WRITE32(gh13_mcu_ipc + 0x00 ,  stpLXSdecCipherKey->eCh ); 	 /* channel */
				 ipc_addr |= (pid_idx << 6);
				 ipc_addr |= (odd_key << 5);
				 SDEC_WRITE32(gh13_mcu_ipc + 0x04 ,  ipc_addr); 					 /* all IPC address */

				 SDEC_WRITE32(gh13_mcu_ipc + 0x08 ,  swab32(*ui32pAddr));				 /* data 0 */
				 ui32pAddr++;
				 SDEC_WRITE32(gh13_mcu_ipc + 0x0c ,  swab32(*ui32pAddr));				 /* data 1 */

				 gWRCNT++;
				 SDEC_WRITE32(gh13_mcu_ipc + 0x10 ,  gWRCNT);							 /* write count */
			 }
		 }
	 }
	 else
	 {
		 /* sys and iv is done by original way */
		 if( ( key_type != 0x1 ) || ( pid_idx != 0xFF ) )	 /* For BCAS IV, Only key_type == 0x1 && pid_idx == 0xFF should be set to all, others are set by original way */
		 {
			 /* Followings are Original Source Code before BCAS */
			 //by skpark
			 for( ; ui8count < stpLXSdecCipherKey->uiKeySize; ui8count++)
			 {
				 SDEC_HAL_KMEMSet( stpLXSdecCipherKey->eCh, key_type, pid_idx, odd_key, ui8count, *ui32pAddr);
				 ui32pAddr++;
			 }
		 }
		 else
		 {
			 UINT8	 ui8pidfilter	 = 0x0;
			 UINT8	 ui8PidfNum 	 = 0x0;
			 LX_SDEC_CFG_T*  pSdecConf	 = NULL;

			 /* get chip configuation */
			 pSdecConf = SDEC_CFG_GetConfig();

			 /* get pid filter number from channel info structure */
			 ui8PidfNum  = pSdecConf->chInfo[stpLXSdecCipherKey->eCh].num_pidf;

			 // IV vector set to all PID filter jinhwan.bae
			 for(ui8pidfilter = 0x0; ui8pidfilter < ui8PidfNum; ui8pidfilter++)
			 {
 // 			 ui32pAddr = (UINT32 *)stpLXSdecCipherKey->puiCipherKey;
				 ui32pAddr = (UINT32 *)key_buf;

				 for(ui8count = 0x0 ; ui8count < stpLXSdecCipherKey->uiKeySize; ui8count++)
				 {
					 SDEC_HAL_KMEMSet( stpLXSdecCipherKey->eCh, key_type, ui8pidfilter, odd_key, ui8count, *ui32pAddr);
					 ui32pAddr++;
				 }
			 }
		 }

	 }// jinhwan.bae for mcu test

	 SDEC_PRINT_DESC( "-->SDEC_IO_SetCipherKey");

	 eRet = RET_OK;

 func_exit:
	 OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	 return (eRet);
 }

 /**
 ********************************************************************************
 * @brief
 *	 set Cipher key.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_IO_GetCipherKey
	 (S_SDEC_PARAM_T *stpSdecParam,
	 LX_SDEC_GET_CIPHERKEY_T *stpLXSdecCipherKey)
 {
	 int eRet = RET_ERROR;
	 UINT8 ui8count = 0x0, key_type = 0, pid_idx = 0, odd_key = 0;
	 UINT32 *ui32pAddr;
	 LX_SDEC_CFG_T *pSdecConf = NULL;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	 /* argument check */
	 pSdecConf = SDEC_CFG_GetConfig();
	 LX_SDEC_CHECK_CODE( stpLXSdecCipherKey->eCh >= (LX_SDEC_CH_T)(pSdecConf->nChannel), return RET_INVALID_PARAMS, "over channel range %d", stpLXSdecCipherKey->eCh);

	 OS_LockMutex(&stpSdecParam->stSdecMutex);

	 SDEC_PRINT_TRACE( "<--SDEC_IO_GetCipherKey");

	 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN || stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD)//Key type :even or ODD
	 {
		 key_type = 0x0;
		 pid_idx =	stpLXSdecCipherKey->uiPidFltId;
		 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN)
		 {
			 odd_key = 0x0;
		 }
		 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD)
		 {
			 odd_key = 0x1;
		 }

		 ui8count = 0x0; //by skpark
	 }
	 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD_IV || stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN_IV)//Key type :IV
	 {
		 key_type = 0x1;
		 pid_idx =	stpLXSdecCipherKey->uiPidFltId;
		 if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_EVEN_IV)
		 {
			 odd_key = 0x0;
		 }
		 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_ODD_IV)
		 {
			 odd_key = 0x1;
		 }

		 ui8count = 0x0; //by skpark
	 }
	 else if(stpLXSdecCipherKey->eKeyMode == LX_SDEC_CIPHER_KEY_MULTI2)
	 {
		 key_type = 0x02;
		 ui8count = 0x0;			 //by skpark
	 }

	 ui32pAddr = (UINT32 *)(&stpLXSdecCipherKey->uiCipherKey[0]);

	 //by skpark
	 for( ; ui8count < stpLXSdecCipherKey->uiKeySize; ui8count++)
	 {
		 *ui32pAddr = swab32(SDEC_HAL_KMEMGet( stpLXSdecCipherKey->eCh, key_type, pid_idx, odd_key, ui8count));
		 ui32pAddr++;
	 }

	 SDEC_PRINT_DESC( "SDEC_IO_GetCipherKey : idx[%d] eKeyMode[%d]", stpLXSdecCipherKey->uiPidFltId, stpLXSdecCipherKey->eKeyMode);
	 SDEC_PRINT_TRACE( "-->SDEC_IO_GetCipherKey");

	 eRet = RET_OK;

	 OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	 return (eRet);
 }



