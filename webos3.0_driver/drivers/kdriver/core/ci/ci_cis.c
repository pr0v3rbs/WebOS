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
 *  CIS extraction layer
 *
 *  author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version	0.6
 *  date		2010.02.22
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */


/*=============================================================================+
| Include                                                                      |
+=============================================================================*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/delay.h>	// for usleep_range, jinhwan.bae 20131018

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/memory.h>
#include <os_util.h>
#include "ci_cis.h"
#include "ci_coredrv.h"
#include "ci_defs.h"
#include "ci_io.h"
#include "ci_drv.h"


/*=============================================================================+
| Local Symbol Definition                                                      |
+=============================================================================*/

/* tuple buffer size */
#define	CIS_TPL_BUF_SZ	257 // maximum possible tuple length

#define MAX_VALID_TUPLE_CNT_CHK	260

/* base address & offset of module attribute memory */
extern unsigned long	CiModBaseAddr[MAX_MOD_NUM];

#define	AM_STEP			0x00000004
#define	AM_STEP16		0x00000002			//for 16bit burst mode

/* CIS_ReadTuple function return value */
#define	TUPLE_NOT_END	0
#define	TUPLE_END		1
#define	TUPLE_LAST		2
#define	TUPLE_INT		3					// for ISR

/* Delay value */
#define SLEEP_VALUE_INIT	20
#define SLEEP_VALUE_INC		200	//160
#define MAX_DELAY_TRY		4


BOOLEAN gbIsCISChecked = FALSE;
extern volatile UINT32 guwDetectCard;		// for ISR
extern volatile UINT32 guwIsPowerRestart;
extern volatile S_CI_REG_T* gpstCIReg;

extern UINT32 gCI_KDRV_Delay[CI_D_ENUM_MAX];	// jinhwan.bae for delay

extern char *simple_strtok(char *s, const char *delim, char **save_ptr);  // jinhwan.bae for strtok


/*=============================================================================+
| Local Variable Declaration                                                   |
+=============================================================================*/

/* CISTPL_VERS_1 information */
static VERS_1			CIS_infoVers1[MAX_MOD_NUM];
/* CISTPL_CONFIG information */
static CONFIG			CIS_infoConfig[MAX_MOD_NUM];
/* CISTPL_CONFIG SubTuple-CIF information */
static CCST_CIF			CIS_ccstCifInfo[MAX_MOD_NUM];
/* CISTPL_CFTABLE_ENTRY information */
static CFTABLE_ENTRY	CIS_infoCftableEntry[MAX_MOD_NUM];
/* CISTPL_CFTABLE_ENTRY SubTuple-CE_EV and CE_PD information */
static STCE_EVPD		CIS_infoStceEvpd[MAX_MOD_NUM];

#if 0 	// not used
/* CISTPL_DEVICE information */
static DEVICE_AC		CIS_infoDevice[MAX_MOD_NUM];
/* CISTPL_DEVICE_A information */
static DEVICE_AC		CIS_infoDeviceA[MAX_MOD_NUM];
/* CISTPL_DEVICE_OA information */
static DEVICE_OAOC		CIS_infoDeviceOa[MAX_MOD_NUM];
/* CISTPL_DEVICE_OC information */
static DEVICE_OAOC		CIS_infoDeviceOc[MAX_MOD_NUM];
/* CISTPL_MANFID information */
static MANFID			CIS_infoManfid[MAX_MOD_NUM];
#endif

/* CISTPL_STATUS information */
static CISTPL_STATUS	CIS_infoTupleStatus[MAX_MOD_NUM];

UINT32   CIS_ParseTuple_count = 0;

/*=============================================================================+
| Local Function Prototype                                                     |
+=============================================================================*/

static int CIS_ReadTuple(UINT32 baseAddr,
						UINT8 *pBuf,
						UINT32 *pOffset,
						SINT16 nSleepTime,
						int burst_mode);
static int CIS_ParseTuple(MOD_ID mId, UINT8 *pBuf, CISTPL_STATUS *pStatus);

static TPL_STATUS CIS_ParseVers1(MOD_ID mId, UINT8 *pBuf);
static TPL_STATUS CIS_ParseConfig(MOD_ID mId, UINT8 *pBuf);
static TPL_STATUS CIS_ParseCftableEntry(MOD_ID mId, UINT8 *pBuf);
#if 0 // not used
static TPL_STATUS CIS_ParseDevice(MOD_ID mId, UINT8 *pBuf);
static TPL_STATUS CIS_ParseDeviceO(MOD_ID mId, UINT8 *pBuf);
static TPL_STATUS CIS_ParseManfid(MOD_ID mId, UINT8 *pBuf);
#endif

#if 0 // CI+1.3
static UINT8 CIS_IsItCIPlusCAM( UINT8 *i_pui8Buf, UINT32 i_ui32Len );
#else
static UINT8 CIS_IsItCIPlusCAM( VERS_1 *pVers1, UINT8 *i_pui8Buf, UINT32 i_ui32Len );
#endif

static void CIS_ParsePowerInfoField(
	UINT8 *pBuf, PWR_DESCRIPT *pPwr, UINT8 pwrParaSel, UINT8 *len);

/*=============================================================================+
| Function Definition                                                          |
+=============================================================================*/
/**
 * Check whether CI or CI+ CAM
 *
*/
SINT32 CIS_WhatCAM( UINT8 mId, UINT8 *o_pui8IsItCIPlusCAM )
{
	SINT32 iRet = -1;

	if (gbIsCISChecked) {
		*o_pui8IsItCIPlusCAM = CIS_infoVers1[mId].ui8IsItCIPlus;
		iRet = 0;
	} else {
		*o_pui8IsItCIPlusCAM = -1;
	}

	return iRet;
}


/**
 * Get CI+ Support Version
 *
*/
SINT32 CIS_GetCIPlusCAMSupportVersion( UINT8 mId, UINT32 *o_pui32Version )
{
	SINT32 iRet = -1;

	if (gbIsCISChecked) {
		*o_pui32Version = CIS_infoVers1[mId].version;
		iRet = 0;
	} else {
		*o_pui32Version = 0;
	}

	return iRet;
}


/**
 * Get CI+ Operator Profile
 *
*/
SINT32 CIS_GetCIPlusCAMOperatorProfile( UINT8 mId, UINT32 *o_pui32Profile )
{
	SINT32 iRet = -1;

	if (gbIsCISChecked) {
		*o_pui32Profile = CIS_infoVers1[mId].profile;
		iRet = 0;
	} else {
		*o_pui32Profile = 0;
	}

	return iRet;
}


/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_Config

   DESCRIPTION:  Perform CIS configuration for PCMCIA module.

   PARAMETERS:   mId    = module id

   RETURN VALUE: CIS_OK   = success
                 CIS_FAIL = fail
                                                                              */
/*----------------------------------------------------------------------------*/
/**
 *  Perform CIS configuration for PCMCIA module
*/
SINT32 CIS_Config(UINT8 mId)
{
	UINT8			aui8Buf[CIS_TPL_BUF_SZ] = { 0 };
	CISTPL_STATUS	*pStatus;
	CONFIG			*pConfig;
	UINT32			baseAddr = CiModBaseAddr[mId];
	int				chkCnt = 0;
	UINT32			offset = 0;
	int				rdRet;
	SINT8			nNumofTry_ReadTuple = 0;
	SINT16			nSleepTime = gCI_KDRV_Delay[CI_D_CIS_END_READ_TUPLE_INITIAL]; // SLEEP_VALUE_INIT;
	int				burst_mode = !!g_ci_burst_mode;
	UINT8			ci_value;

	if( mId != MOD_A )
		return CIS_FAIL;

	if( guwIsPowerRestart )	{	// for power restart
		CI_SOC_MSG_CIS_ERR("guwIsPowerRestart detected\n");
		return CIS_FAIL;
	}

	CI_SOC_MSG_CIS_INFO("Configuring CIS of module %c \n", 'A'+mId);

	pStatus = &CIS_infoTupleStatus[mId];
	pConfig  = &CIS_infoConfig[mId];

	CI_SOC_MSG_CIS_INFO("baseAddr: 0x%x \n", baseAddr);

	/* STEP 1 : check whether the first tuple is valid -----------------------*/
	/* the basic purpose of this checking is to check whether the module is
	   really ready.
	   note that some module is not ready even after asserting RDY signal. */

	chkCnt = MAX_VALID_TUPLE_CNT_CHK + 20;

	memset((void *)&CIS_infoVers1[mId], 0x00, sizeof(VERS_1));
	memset((void *)&CIS_infoConfig[mId], 0x00, sizeof(CONFIG));
	memset((void *)&CIS_ccstCifInfo[mId], 0x00, sizeof(CCST_CIF));
	memset((void *)&CIS_infoCftableEntry[mId], 0x00, sizeof(CFTABLE_ENTRY));
	memset((void *)&CIS_infoStceEvpd[mId], 0x00, sizeof(STCE_EVPD));

	while( chkCnt-- )
	{
		if( !guwDetectCard )			// for ISR
			goto CIS_CONFIG_FAIL;

		if (CI_Check_CD_Pol(0, __func__)) {
			CI_SOC_MSG_CIS_ERR("CI Removed (1)\n");
			goto CIS_CONFIG_FAIL;
		}

		if( guwIsPowerRestart )	{	// for power restart
			CI_SOC_MSG_CIS_ERR("guwIsPowerRestart detected\n");
			goto CIS_CONFIG_FAIL;
		}

		ci_value = BYTE(baseAddr);
		CI_SOC_MSG_CIS_TUPLE_2("> ci_value = 0x%02X\n", ci_value);
		if (ci_value == CISTPL_DEVICE_OA)
		{
			if((MAX_VALID_TUPLE_CNT_CHK - 1) == chkCnt) {
				CI_HW_IO_SLEEP(CI_D_CIS_CONFIG_FIRST_TUPLE_OK);
			}

			break;
		}

		CI_HW_IO_SLEEP(CI_D_CIS_CONFIG_FIRST_TUPLE_NG);
	}

	if( chkCnt <= 0 )
	{
		CI_SOC_MSG_CIS_ERR("CIS_Config: the first tuple is invalid [0x%02X]\n", ci_value);
		goto CIS_CONFIG_FAIL;
	}

	/* STEP 2 : read & analyze CIS tuples ------------------------------------*/
CIS_READ_ANALYZE_STEP:
	// Initialize variables
	offset = 0x0;
	CIS_ParseTuple_count = 0;
	chkCnt = MAX_VALID_TUPLE_CNT_CHK;

	// initialize tuple status informations with DATA_NOT_AVAILABLE
	pStatus->vers_1        	= DATA_NOT_AVAILABLE;
	pStatus->config        	= DATA_NOT_AVAILABLE;
	pStatus->cftable_entry 	= DATA_NOT_AVAILABLE;
#if 0 	// not used
	pStatus->device        	= DATA_NOT_AVAILABLE;
	pStatus->device_a      	= DATA_NOT_AVAILABLE;
	pStatus->device_oa     	= DATA_NOT_AVAILABLE;
	pStatus->device_oc     	= DATA_NOT_AVAILABLE;
	pStatus->manfid       	= DATA_NOT_AVAILABLE;
#endif

	if ( burst_mode ) {
#ifdef _CI_ACCESS_BURST_MODE_
		CI_ChangeAccessMode(ACCESS_4BYTE_MODE);
#endif
	}

	while( chkCnt-- )
	{
		rdRet = CIS_ReadTuple(baseAddr, aui8Buf, &offset, nSleepTime, burst_mode);
		if( rdRet != TUPLE_INT )
		{
			if (CI_Check_CD_Pol(0, __func__) == 1) {
				CI_SOC_MSG_CIS_ERR("CI Removed (2)\n");
				rdRet = TUPLE_INT;
			}
		}

		if( rdRet == TUPLE_INT )
		{
			if (burst_mode) {
#ifdef _CI_ACCESS_BURST_MODE_
				CI_ChangeAccessMode(ACCESS_1BYTE_MODE);
#endif
			}

			goto CIS_CONFIG_FAIL;
		}

		if( rdRet == TUPLE_END )
			break;

	#if 0
		{
			UINT32 addr_limit = (pConfig->baseAddr)? pConfig->baseAddr : TPCC_RADR_MAX;
			if (offset > addr_limit) {
				CI_SOC_MSG_CIS_ERR("offset overflow : limit[0x%04X]\n", addr_limit);
				break;
			}
		}
	#endif

		CI_SOC_MSG_CIS_TUPLE("Tuple Code=0x%02X, Tuple Length=%3d -------------------------------\n",
																		aui8Buf[0], aui8Buf[1]+2);
		CI_SOC_MSG_CIS_TUPLE("-----------------------------------------------------------------\n");

		if( CIS_ParseTuple(mId, aui8Buf, pStatus) == CIS_PARSETIMEOUT )
		{
			CI_SOC_MSG_CIS_ERR("CIS configuration FAILED #1\n");
			break;
		}
		if( rdRet == TUPLE_LAST )
			break;

		if( guwIsPowerRestart ) {	// for power restart
			CI_SOC_MSG_CIS_ERR("guwIsPowerRestart detected\n");
			goto CIS_CONFIG_FAIL;
		}

		memset(aui8Buf, 0x00, sizeof (aui8Buf));
	}

	if (burst_mode) {
#ifdef _CI_ACCESS_BURST_MODE_
		CI_ChangeAccessMode(ACCESS_1BYTE_MODE);
#endif
	}

	if( (DVB_CI_COMPLIANT == pStatus->vers_1) &&
  		(DVB_CI_COMPLIANT == pStatus->config) &&
		(DVB_CI_COMPLIANT == pStatus->cftable_entry) )
	{
		CI_SOC_MSG_CIS_INFO("\n\nCIS configuration SUCCESS\n\n" );
		return CIS_OK;
	}
	else
	{	// if the abnormal case, the try again (max 3, delay time is changed)
		if( ++nNumofTry_ReadTuple < MAX_DELAY_TRY )
		{
			CI_SOC_MSG_CIS_ERR("Again get CIS configuration (%d try)\n", nNumofTry_ReadTuple);
			nSleepTime = SLEEP_VALUE_INC * nNumofTry_ReadTuple;
		//	nSleepTime += 100;

			goto CIS_READ_ANALYZE_STEP;
		} else {
			CI_SOC_MSG_CIS_ERR("CIS configuration FAILED #2\n" );
		}
	}


CIS_CONFIG_FAIL:
	return CIS_FAIL;

}
/*----------------------------------------------------------------------------*/


/**
 *	Write COR(Configuration Option Register) *
 */
SINT32 CIS_WriteCOR(UINT8 mId)
{
	UINT8 read_data;

#if 0
	if( lx_chip_rev() >= LX_CHIP_REV(H15, A0) )
	{
		H15_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H14, A0) )
	{
		H14_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, B0) )
	{
		M14_B0_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, A0) )
	{
		M14_A0_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H13, A0) )
	{
#if 0 // OLD KERNEL Auto IOREMAP
		H13_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( H13_CI_CAM_BASE + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
#else
		H13_CI_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
		read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
#endif
	}
#else
#if 1
	_ci_cam_writew(CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f, (volatile unsigned short *)(gRegCICAM + CIS_infoConfig[mId].baseAddr));
	read_data = _ci_cam_readw((volatile unsigned short *)(CiModBaseAddr[MOD_A] + CIS_infoConfig[mId].baseAddr));
#else
	writew(CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f, (volatile unsigned short *)(gRegCICAM + CIS_infoConfig[mId].baseAddr));
	read_data = readw((volatile unsigned short *)(CiModBaseAddr[MOD_A] + CIS_infoConfig[mId].baseAddr));
//	CI_HW_REG_WRITE((CIS_infoConfig[mId].baseAddr) , CIS_infoCftableEntry[mId].cfgTableIdx & 0x3f);
//	read_data = ( UINT16 )( * ( UINT16* )( CiModBaseAddr[MOD_A] + (CIS_infoConfig[mId].baseAddr)) );//CI_REG_READ(CIS_infoConfig[mId].baseAddr);
#endif
	CI_SOC_MSG_DBG("COR = 0x%02x\n", read_data);
#endif

	CI_HW_IO_SLEEP(CI_D_CIS_END_WRITE_COR);

	return CIS_OK;
}


/*----------------------------------------------------------------------------*/


#ifdef _CI_ACCESS_BURST_MODE_

/*----------------------------------------------------------------------------*/
/*

   NAME:         CIS_ReadTuple32

   DESCRIPTION:  Read CIS tuple from module. (32bit burst mode)

   PARAMETERS:   baseAddr = base address of module
                 pBuf     = pointer to tuple buffer
                 pOffset  = pointer to offset value of target tuple

   RETURN VALUE: TUPLE_NOT_END = one or more tuples are following
                 TUPLE_END     = no more tuple
                 TUPLE_NOT_END = current tuple is the last one
                                                                           			*/
/*----------------------------------------------------------------------------*/
static int CIS_ReadTuple32(UINT32 baseAddr, UINT8 *pBuf, UINT32 *pOffset, SINT16 nSleepTime)
{
	UINT8	*pData = NULL;
	UINT32 	offset = *pOffset;
	UINT8 	tuple_byte = 0x00;
	UINT32	tuple_value = 0x00000000;
	UINT16	tuple_value_word = 0x0000;
	UINT8	tuple_length_temp = 0, tuple_length_q = 0, tuple_length_r = 0; /* tuple length quotient, remainder */

	pData  = pBuf;

	/* 0.	If Start Address is aligned by 4, Start with 4 byte mode , set in CIS_Config, actually getting 2 byte data by 1 shot.
			else not aligned by 4, Start with 2 byte mode (actually getting 1 byte data by 1 shot) until BYTE3 and continue to 4 byte mode */
	if((baseAddr+offset)%4)
	{
		CI_SOC_MSG_CIS_INFO("tuple start address is not aligned by 4 [0x%x]\n",baseAddr+offset);

		/* Not Aligned Mode, Start with 2 byte mode until BYTE3 */
		CI_ChangeAccessMode(ACCESS_2BYTE_MODE);

		/* 1. Read First Byte and check Error */
		tuple_value_word = WORD(baseAddr+offset);
		tuple_byte = (UINT8)(tuple_value_word & 0x00FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		if( tuple_byte == CISTPL_END )
		{
			return TUPLE_END;
		}

		*pData++ = tuple_byte;
		offset += AM_STEP16;

		/* 2. Read Tuple Length */
		tuple_value_word = WORD(baseAddr+offset);
		tuple_byte = (UINT8)(tuple_value_word & 0x00FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		if( tuple_byte == CISTPL_LINK_END )
		{
			return TUPLE_LAST;
		}

		*pData++ = tuple_byte;
		offset += AM_STEP16;

		tuple_length_temp = tuple_byte;

		/* 3. Read 1 BYTE Tuple Data */
		if( !guwDetectCard )
		{
			return TUPLE_INT;
		}
		tuple_value_word = WORD(baseAddr+offset);
		tuple_byte = (UINT8)(tuple_value_word & 0x00FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		*pData++ = tuple_byte;
		offset += AM_STEP16;

		/* after 1 byte data read */
		tuple_length_temp--;

		/* 4. Read DWORD until tuple_length_q : actual length /2 (+ 5. Read Remain 1 BYTE) */
		tuple_length_q = (tuple_length_temp/2);
		tuple_length_r = (tuple_length_temp%2);

		/* End of Not Aligned Mode, convert to 4 byte mode */
		CI_ChangeAccessMode(ACCESS_4BYTE_MODE);

		CI_HW_IO_SLEEP(CI_D_CIS_DURING_READ_TUPLE);
	}
	else
	{
		/* 1. Read DWORD 32 bit, 2 valid CIS data bytes */
		tuple_value = DWORD(baseAddr+offset);

		/* 2. First 2 Valid Data Processing */
		tuple_byte = (UINT8)(tuple_value & 0x000000FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		/* Check 1st Tuple Value Error */
		if(tuple_byte == CISTPL_END)
		{
			/* mode will be changed in CIS_Config to BYTE Mode */
			return TUPLE_END;
		}

		/* Save 1st Tuple Byte */
		*pData++ =  tuple_byte;

		/* Check 2nd Tuple Value Error */
		tuple_byte = (UINT8)((tuple_value & 0x00FF0000) >> 16);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		if(tuple_byte == CISTPL_LINK_END)
		{
			return TUPLE_LAST;
		}

		/* Save 2nd Tuple Byte */
		*pData++ =  tuple_byte;

		/* 3. Jump to Next 2 Valid Data Address */
		offset += AM_STEP;

		/* 4. Read DWORD until tuple_length_q : actual length /2 (+ 5. Read Remain 1 BYTE) */
		tuple_length_q = (tuple_byte/2);
		tuple_length_r = (tuple_byte%2);
	}

	while(tuple_length_q--)
	{
		if( !guwDetectCard )
		{
			return TUPLE_INT;
		}

		tuple_value = DWORD(baseAddr+offset);
		tuple_byte = (UINT8)(tuple_value & 0x000000FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		*pData++ = tuple_byte; 			/* save 1st byte */

		tuple_byte = (UINT8)((tuple_value & 0x00FF0000) >> 16);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		*pData++ = tuple_byte; 	/* save 2nd byte */

		offset += AM_STEP;
		CI_HW_IO_SLEEP(CI_D_CIS_DURING_READ_TUPLE);
	}

	/* 5. If exist, Read BYTE remainder */
	/* if actual tuple length is odd, read remain 1 byte with 2 BYTE ACCESS MODE */
	/* 1 BYTE Access Mode has different addressing, 2 BYTE and 4 BYTE ACCESS have same addressing policy.
	    So, BURST Mode, Use 16bit - 32 bit mode, 2 BYTE - 4 BYTE Mode */
	if(tuple_length_r == 1)
	{
		CI_SOC_MSG_CIS_INFO("tuple 1 byte remained [0x%x]\n",baseAddr+offset);

		/* 2 Byte Addressing !! */
		CI_ChangeAccessMode(ACCESS_2BYTE_MODE);

		if( !guwDetectCard )
		{
			return TUPLE_INT;
		}

		tuple_value_word = WORD(baseAddr+offset);
		tuple_byte = (UINT8)(tuple_value_word & 0x00FF);
		CI_SOC_MSG_CIS_TUPLE_2("> tuple_byte = 0x%02X\n", tuple_byte);
		*pData++ = tuple_byte;

		offset += AM_STEP16;

		CI_ChangeAccessMode(ACCESS_4BYTE_MODE);
	}
	else if(tuple_length_r != 0)
	{
		/* ERROR */
		CI_SOC_MSG_ERR("tuple length remainder is not valid [%d]\n",tuple_length_r);
	}

	/* 6. Update Offset Value */
	*pOffset = offset;

	CI_HW_IO_SLEEP(CI_D_CIS_DURING_READ_TUPLE);

	return TUPLE_NOT_END;
}

#endif // #ifdef _CI_ACCESS_BURST_MODE_



/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ReadTuple

   DESCRIPTION:  Read CIS tuple from module.

   PARAMETERS:   baseAddr = base address of module
                 pBuf     = pointer to tuple buffer
                 pOffset  = pointer to offset value of target tuple

   RETURN VALUE: TUPLE_NOT_END = one or more tuples are following
                 TUPLE_END     = no more tuple
                 TUPLE_NOT_END = current tuple is the last one
                                                                              */
/*----------------------------------------------------------------------------*/
static int CIS_ReadTuple(UINT32 baseAddr,
						UINT8 *pBuf,
						UINT32 *pOffset,
						SINT16 nSleepTime,
						int burst_mode)
{
	UINT8	*pData = NULL;
	UINT32 	offset = *pOffset;
	UINT8 	tupleValue = 0;
	UINT8	tupleBody = 0;

#ifdef _CI_ACCESS_BURST_MODE_
	if (burst_mode) {
		return CIS_ReadTuple32(baseAddr, pBuf, pOffset, nSleepTime);
	}
#endif

	pData  = pBuf;

	/* read tuple code */
	tupleValue =  BYTE(baseAddr+offset);
	CI_SOC_MSG_CIS_TUPLE_2("> tupleValue = 0x%02X\n", tupleValue);
	if( tupleValue == CISTPL_END )
		return TUPLE_END;

	*pData++  =  tupleValue;
	offset    += AM_STEP;

	/* read tuple length */
	tupleValue =  BYTE(baseAddr+offset);
	CI_SOC_MSG_CIS_TUPLE_2("> tupleValue = 0x%02X\n", tupleValue);
	if( tupleValue == CISTPL_LINK_END )
		return( TUPLE_LAST );

	*pData++ =  tupleValue;
	offset   += AM_STEP;

	/* read tuple body */
	while( tupleValue-- )
	{
		if( !guwDetectCard )			// for ISR
			return TUPLE_INT;

		tupleBody = BYTE(baseAddr+offset);
		CI_SOC_MSG_CIS_TUPLE_2("> tupleBody  = 0x%02X\n", tupleBody);
		*pData++ = tupleBody;
		offset   += AM_STEP;

		CI_HW_IO_SLEEP(CI_D_CIS_DURING_READ_TUPLE);
	}

	/* update offset value */
	*pOffset = offset;

	_HW_IO_SLEEP(nSleepTime, nSleepTime);

	return TUPLE_NOT_END;
}



/*----------------------------------------------------------------------------*/

static void CIS_PrintBuf(UINT8 *buf, UINT32 len)
{
#if 0
	int i;

	for (i=0 ; i<len ; i++) {
		CI_SOC_MSG_DBG("0x%02x\n", buf[i]);
	}
#endif
}

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseTuple

   DESCRIPTION:  Parse CIS tuple that read from module.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer
                 pStatus = pointer to CIS status structure

   RETURN VALUE: CIS_OK   = success
                 CIS_FAIL = fail
                                                                              */
/*----------------------------------------------------------------------------*/
static int CIS_ParseTuple(MOD_ID mId, UINT8 *pBuf, CISTPL_STATUS *pStatus)
{
	UINT8	tupleCode;


	tupleCode = *pBuf;

	switch (tupleCode)
	{
	case CISTPL_VERS_1 :
		CI_SOC_MSG_CIS_INFO("[CISTPL_VERS_1] =================================================\n" );
		pStatus->vers_1 = CIS_ParseVers1(mId, pBuf);
		break;

	case CISTPL_CONFIG :
		CI_SOC_MSG_CIS_INFO("[CISTPL_CONFIG] =================================================\n" );
		pStatus->config = CIS_ParseConfig(mId, pBuf);
		break;

	case CISTPL_CFTABLE_ENTRY :
		CI_SOC_MSG_CIS_INFO("[CISTPL_CFTABLE_ENTRY] ==========================================\n" );
		/* parse only if DVB_CI compliant tuple is not found */
		if (pStatus->cftable_entry != DVB_CI_COMPLIANT)
		{
			pStatus->cftable_entry = CIS_ParseCftableEntry(mId, pBuf);
		}
		break;

	case CISTPL_NO_LINK :
	case CISTPL_DEVICE :
	case CISTPL_DEVICE_A :
	case CISTPL_DEVICE_OC :
	case CISTPL_DEVICE_OA:
	case CISTPL_MANFID :
		CI_SOC_MSG_CIS_INFO("[Tuple-0x%x] Skipped ============================================\n", tupleCode );
		break;


	default :
		CIS_ParseTuple_count++;
		CI_SOC_MSG_CIS_INFO("\n%d-CIS_ParseTuple: Not CI compliant tuple (TupleCode=0x%02X)\n",
							CIS_ParseTuple_count, tupleCode );

		/* M14_TBD, CIS_ParseTuple_count is not over than 300, check it and remove it or change the threshold */
		if(CIS_ParseTuple_count >= MAX_TUPLE_COUNT_ERR)
	   	{
		     CI_SOC_MSG_CIS_ERR("^e^========@ CIS_ParseTuple error =======\n" );
		     return CIS_PARSETIMEOUT;
		}

		CI_HW_IO_SLEEP(CI_D_CIS_PARSE_NON_CI_TUPLE);

		break;
	}

	CI_SOC_MSG_CIS_PARSE("=================================================================\n" );

	return CIS_OK;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseVers1

   DESCRIPTION:  Parse CISTPL_VERS_1 tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseVers1(MOD_ID	mId, UINT8 *pBuf)
{
	VERS_1		*pVers1;
	TPL_STATUS	ret;
	UINT8		*pData;
	UINT8		tupleLen;
	UINT8		strCnt;
	UINT8		tupleBody;
	UINT8		restLen;
	UINT8		*pStr[4];
	UINT8		ui8Tmp = 0;

	pVers1	 = &CIS_infoVers1[mId];
	pData	 = ++pBuf;
	tupleLen = *pData++;

	memset( pVers1, 0, sizeof( VERS_1 ) );

	/* M14_TBD, tupleLen is not over than 255, set the value from UINT8* value, Check and Delete It 2013. 07. 03, jinhwan.bae */
#if 0
	if( tupleLen > CIS_TPL_BUF_SZ - 2 )
	{
		CI_SOC_MSG_CIS_PARSE("CIS - Actual Tuple Len - %d, Source buf(Tuple) size - %d\n",
							tupleLen, CIS_TPL_BUF_SZ );

		return DATA_NOT_AVAILABLE;
	}
#endif

	/* parse version fields */
	pVers1->major = *pData++;
	pVers1->minor = *pData++;

	ret = ((pVers1->major==TPLLV1_MAJOR) && (pVers1->minor==TPLLV1_MINOR)) ?	\
			DVB_CI_COMPLIANT : DVB_CI_NOT_COMPLIANT;

	CI_SOC_MSG_CIS_PARSE("  Major Version: 0x%02X\n", pVers1->major );
	CI_SOC_MSG_CIS_PARSE("  Minor Version: 0x%02X\n", pVers1->minor );

	/* parse version info string fields */
	pStr[0] = pVers1->manufact; pStr[1] = pVers1->product;
	pStr[2] = pVers1->add1; 	pStr[3] = pVers1->add2;
	restLen = tupleLen-2;
	strCnt	= 0;

	while ((restLen--) && (strCnt < 4))
	{
		tupleBody = *pData++;

	//	CI_SOC_MSG_DBG_2("0x%02x\n", tupleBody);

		if (tupleBody == 0xFF) {
			break;
		} else if (tupleBody) {
			if (ui8Tmp < (VERS_1_INFO_LEN - 3)) {
				*(pStr[strCnt])++ = tupleBody;
				ui8Tmp++;
			}
		} else {
			*(pStr[strCnt])++ = tupleBody;
			*(pStr[strCnt]) = '\0';
			strCnt++;
			ui8Tmp = 0;

			// add1 or add2
			if((strCnt == 2) || (strCnt == 3))	{
				if (!pVers1->ui8IsItCIPlus) {
//					pVers1->ui8IsItCIPlus = CIS_IsItCIPlusCAM( pData, restLen );
					pVers1->ui8IsItCIPlus = CIS_IsItCIPlusCAM( pVers1, pData, restLen );
				}
			}
		}
	}

	gbIsCISChecked = TRUE;

	CI_SOC_MSG_CIS_PARSE("  Manufact Str  : %s\n", pVers1->manufact );
	CI_SOC_MSG_CIS_PARSE("  Product Str   : %s\n", pVers1->product );
	CI_SOC_MSG_CIS_PARSE("  Add1 Str      : %s\n", pVers1->add1 );
	CI_SOC_MSG_CIS_PARSE("  Add2 Str      : %s\n", pVers1->add2 );
	CI_SOC_MSG_CIS_PARSE("  Is it CI+ CAM : %d[%s]\n", pVers1->ui8IsItCIPlus,
						(pVers1->ui8IsItCIPlus)? "yes" : "no");

	return ret;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseVers1

   DESCRIPTION:  Parse CISTPL_CONFIG tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseConfig(MOD_ID mId, UINT8 *pBuf)
{
	CONFIG		*pConfig;
	CCST_CIF	*pCcstCif;
	UINT8		*pData;
	UINT8		tupleLen;
	UINT8		tupleBody;
	UINT8		tpccRfsz, tpccRmsz, tpccRasz;
	UINT8		shiftByte;
	UINT8		subTupleCode, subTupleLen;
	UINT8		exitFlag;
	UINT8		*str;
	UINT16		stciIfn;

	pConfig  = &CIS_infoConfig[mId];
	pData    = ++pBuf;
   	tupleLen = *pData++;

	tupleBody = *pData++;
	tpccRasz = tupleBody & TPCC_RASZ_MASK;
	/* size of configuration register base address */
	tpccRmsz = (tupleBody & TPCC_RMSZ_MASK) >> 2;
	/* size of presence mask */
	tpccRfsz = (tupleBody & TPCC_RFSZ_MASK) >> 6;
	/* byte size of RFU = it must be zero */
	tpccRasz++;
	tpccRmsz++;

	CI_SOC_MSG_CIS_PARSE("  TPCC_RASZ : %02d bytes\n", tpccRasz );
	CI_SOC_MSG_CIS_PARSE("  TPCC_RMSZ : %02d bytes\n", tpccRmsz );
	CI_SOC_MSG_CIS_PARSE("  TPCC_RFSZ : %02d bytes\n", tpccRfsz );

	if( tupleLen < (tpccRasz+tpccRmsz+tpccRfsz+2) )
	{
		CI_SOC_MSG_CIS_ERR("CIS_ParseConfig: size error \n" );
		return DVB_CI_NOT_COMPLIANT;
	}

	pConfig->size = tupleBody;

	pConfig->lastIndex = *pData;

	CI_SOC_MSG_CIS_PARSE("  Last Index Entry    : 0x%02X\n", pConfig->lastIndex );

	pConfig->baseAddr = 0x00000000;
	shiftByte = 0;

	while( tpccRasz-- )
	{
		pConfig->baseAddr |=
			((UINT32)(*(++pData))&0x000000FF) << (8*shiftByte++);
	}

	CI_SOC_MSG_CIS_PARSE("	Config Reg Base Addr: 0x%08X\n",	pConfig->baseAddr );

	if( pConfig->baseAddr > TPCC_RADR_MAX )
	{
		return DVB_CI_NOT_COMPLIANT;
	}

	pConfig->mask = *(++pData);
	if( tpccRmsz > 1 )
		pData += (tpccRmsz - 1); // skip

	subTupleCode = *(++pData);
	if( subTupleCode != CFGST_CIF )
	{
		return DVB_CI_NOT_COMPLIANT;
	}

	/* parse sub-tuple */
	pCcstCif = &CIS_ccstCifInfo[mId];
	subTupleLen = *(++pData);
	tupleBody   = *(++pData);
	if( tupleBody & 0x40 )
	{
		pCcstCif->ifIdNumBase = tupleBody;
		stciIfn = (((UINT16)tupleBody) & 0x00FF);
		tupleBody = *(++pData);
		stciIfn |= (((UINT16)tupleBody) << 8) & 0xFF00;
		pCcstCif->ifIdNum = tupleBody;

		CI_SOC_MSG_CIS_PARSE("  Interface Num       : 0x%04X\n", stciIfn );

		if( stciIfn != DVB_CI_COMPLIANT_IFN )
		{
			return DVB_CI_NOT_COMPLIANT;
		}

		subTupleLen -= 2; // remainder of data
		if (subTupleLen > CCST_CIF_STRING_LENGTH)
			subTupleLen = CCST_CIF_STRING_LENGTH;
		exitFlag = 0;
	   	str = pCcstCif->str;
		while( (subTupleLen--)					&&
				((tupleBody=*(++pData))!=0xFF)	&&
				(!exitFlag) )
		{
			*str++ = tupleBody;
			if( tupleBody == 0x00 ) exitFlag = 1;
		}

		CI_SOC_MSG_CIS_PARSE("  String Info         : %s\n", pCcstCif->str );
	}
	else
	{
		return DVB_CI_NOT_COMPLIANT;
	}

	return DVB_CI_COMPLIANT;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseCftableEntry

   DESCRIPTION:  Parse CISTPL_CFTABLE_ENTRY tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseCftableEntry(MOD_ID mId, UINT8 *pBuf)
{
	CFTABLE_ENTRY	*pEntry;
	UINT8			*pData;
	UINT8			tupleLen;
	UINT8			tupleBody;
	UINT8			feature;
	UINT8			pwrVal;
	PWR_DESCRIPT	*pPwrInfo;
	UINT8			pwrParaSel;
	int				i;
	UINT8			infoLen;
	UINT8			numOfWindow;
	UINT8			sizeOfWindow;
	UINT8			subTupleLen;
	UINT8			*pNext;
	UINT8			*pStr;
	STCE_EVPD		*pStceEvpd;

	pEntry    = &CIS_infoCftableEntry[mId];
	pData     = ++pBuf;
   	tupleLen  = *pData++;

	tupleBody = *pData;
	pEntry->cfgTableIdx = tupleBody;

	if( !(tupleBody & (TPCE_INDX_DEF_BIT|TPCE_INDX_IF_BIT)) )
	{
   		return DVB_CI_NOT_COMPLIANT;
	}

	tupleBody = *(++pData);
	if( tupleBody != TPCE_IF )
	{
		return DVB_CI_NOT_COMPLIANT;
	}

	/* parsing features */
	feature = *(++pData);
	CIS_infoCftableEntry[mId].featureSel = feature;

	/* parse power information */
	pwrVal = feature & TPCE_FS_POWER_BIT;
	while( pwrVal-- )
	{
		pPwrInfo   = pEntry->pwrInfoField;
		pwrParaSel = *(++pData);
		/* parse each power field */
		for( i=0; i<7; i++ )
		{
			CIS_ParsePowerInfoField(pData, pPwrInfo,
				(UINT8)(pwrParaSel & (UINT8)(0x01<<i)),	&infoLen);
			pData += infoLen;
		}
		pPwrInfo++;
	}

	/* skip tme information */
	if( feature & TPCE_FS_TIME_BIT )
		pData++;

	/* parse I/O space information */
	if( feature & TPCE_FS_IOSP_BIT )
	{
		tupleBody = *(++pData);
		if( tupleBody != TPCE_IO )
		{
			return DVB_CI_NOT_COMPLIANT;
		}
	}

	/* skip IRQ information */
	if( feature & TPCE_FS_IRQ_BIT )
	{
		tupleBody = *(++pData);
		if( tupleBody & 0x10 )
			pData += 2;
	}

	/* skip memory space information */
	if( feature & TPCE_FS_MEMSP_BIT )
	{
		switch( (feature & TPCE_FS_MEMSP_BIT)>>5 )
		{
			case 1 :	pData+=2;	break;
			case 2 :	pData+=4;	break;
			case 3 :
				tupleBody    = *(++pData);
				numOfWindow  = tupleBody & 0x07;
				sizeOfWindow = ((tupleBody & 0x18)>>3) + ((tupleBody & 0x60)>>5);
				if( tupleBody & 0x80 )
				 	sizeOfWindow += (tupleBody & 0x60)>>5;
			 	pData += numOfWindow * sizeOfWindow;
			break;
		}
	}

	/* skip miscellaneous informations */
	if( feature & TPCE_FS_MISC_BIT )
	{
		tupleBody = *(++pData);
		while( tupleBody & EXT_BIT )
			tupleBody = *(++pData);
	}

	tupleBody = *(++pData);

	if( tupleBody == 0xFF )
	{
		return DVB_CI_NOT_COMPLIANT;
	}

	/* parse environment sub-tuple */
	pStceEvpd = &CIS_infoStceEvpd[mId];
	if( tupleBody == STCE_EV ) {
		pStr = pStceEvpd->environment;
		subTupleLen = *(++pData);
		tupleBody   = *(++pData);
		pNext = pData + subTupleLen;
		while( (subTupleLen--) && (tupleBody!=0xFF) )
		{
			*pStr++ = tupleBody;
			if( tupleBody == 0x00 )
				break;
			else
				tupleBody = *(++pData);
		}

		CI_SOC_MSG_CIS_INFO("	Environment String    : %s\n", pStceEvpd->environment );
	}
	else
	{
			pNext = pData;
	}

	/* parse physical device sub-tuple */
	pData     = pNext;
	tupleBody = *pData;

	if( tupleBody == STCE_PD )
	{
		pStr = pStceEvpd->physicalDev;
		subTupleLen = *(++pData);
		tupleBody   = *(++pData);
		pNext = pData + subTupleLen;
		while( (subTupleLen--) && (tupleBody!=0xFF) )
		{
			*(pStr++) = tupleBody;
			if( tupleBody == 0x00 )
				break;
			else
				tupleBody = *(++pData);
		}

		CI_SOC_MSG_CIS_INFO("  Physical Device String: %s\n", pStceEvpd->physicalDev );
	}

	return DVB_CI_COMPLIANT;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParsePowerInfoField

   DESCRIPTION:  Parse power parameter information.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: void
                                                                              */
/*----------------------------------------------------------------------------*/
static void CIS_ParsePowerInfoField(UINT8 *pBuf, PWR_DESCRIPT *pPwr,
	UINT8 pwrParaSel, UINT8 *len)
{
	UINT8	*pData;
	UINT8	cnt;
	UINT8	tupleBody = 0;

	pData = pBuf;
	cnt = 0;

	if( pwrParaSel & PWR_NOMV_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->nom_v = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->nom_v_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_MINV_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->min_v = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->min_v_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_MAXV_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->max_v = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->max_v_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_STATI_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->stat_i = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->stat_i_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_AVRGI_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->avrg_i = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->avrg_i_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_PEAKI_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->peak_i = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->peak_i_x = tupleBody;
			cnt++;
		}
	}

	else if( pwrParaSel & PWR_PDWNI_BIT ) {
		tupleBody = *(++pData);
		cnt++;
		pPwr->pdwn_i = tupleBody;
		if( tupleBody & EXT_BIT ) {
			tupleBody = *(++pData);
			pPwr->pdwn_i_x = tupleBody;
			cnt++;
		}
	}

	/* skip */
	while (tupleBody&EXT_BIT) {
		tupleBody = *(pData++);
		cnt++;
	}

	*len = cnt;

	return;
}
/*----------------------------------------------------------------------------*/

#if 0  // not used
/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseDevice

   DESCRIPTION:  Parse CISTPL_DEVICE or CISTPL_DEVICE_A tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseDevice(MOD_ID mId, UINT8 *pBuf)
{
	UINT8		*pData;
	DEVICE_AC	*pDev;
	UINT8		tupleBody;
	UINT8		tupleCode;
	UINT8		tupleLen;
	UINT8		devId;
	UINT8		storageAvail;

	pData = pBuf;

	tupleCode = *pData++;
	if 		(tupleCode == CISTPL_DEVICE)   pDev = &CIS_infoDevice[mId];
	else if	(tupleCode == CISTPL_DEVICE_A) pDev = &CIS_infoDeviceA[mId];
	else return(DVB_CI_NOT_COMPLIANT);

	tupleLen = *pData++;

	devId = *pData++;
	storageAvail = DEVICE_OAOC_STORAGE_MAX;

	while ((devId != 0xFF) && (storageAvail--))
	{
		pDev->id = devId;

		/* parse extended device speed */
		if ((devId&0x07) == DSPEED_EXT) {
			tupleBody = *pData++;
			pDev->xspeed = tupleBody;
			while (tupleBody&EXT_BIT)
				tupleBody = *pData++;
		}

		/* parse extended devide type */
		if (((devId>>4)&0x0F) == DTYPE_EXTEND) {
			tupleBody = *pData++;
			pDev->xdevice = tupleBody;
			while (tupleBody&EXT_BIT)
				tupleBody = *pData++;
		}

		/* parse device size field */
		pDev->deviceSize = *pData++;

		devId = *pData++;
	}

	return(DVB_CI_COMPLIANT);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseDeviceO

   DESCRIPTION:  Parse CISTPL_DEVICE_OC or CISTPL_DEVICE_OA tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseDeviceO(MOD_ID mId, UINT8 *pBuf)
{
	UINT8		*pData;
	DEVICE_OAOC	*pDev;
	UINT8		tupleBody;
	UINT8		tupleCode;
	UINT8		tupleLen;
	UINT8		otherInfo;
	UINT8		devId;
	UINT8		storageAvail;
	UINT8		vccUsed;

	pData = pBuf;

	tupleCode = *pData++;
	if 		(tupleCode == CISTPL_DEVICE_OC) pDev = &CIS_infoDeviceOc[mId];
	else if	(tupleCode == CISTPL_DEVICE_OA) pDev = &CIS_infoDeviceOa[mId];
	else return(DVB_CI_NOT_COMPLIANT);

	tupleLen = *pData++;

	/* parse other conditions info field */
	otherInfo = *pData++;
	pDev->otherInfo = otherInfo;

	vccUsed = (otherInfo >> 1) & 0x03;

	/* parse additional bytes of other conditions info field */
	if (otherInfo & EXT_BIT) {
		tupleBody = *pData++;
		while (tupleBody&EXT_BIT)
			tupleBody = *pData++;
	}

	/* parse	device id field */
	devId = *pData++;
	storageAvail = DEVICE_OAOC_STORAGE_MAX;
	while ((devId != 0xFF) && (storageAvail--)) {
		pDev->id = devId;

		/* parse extended device speed */
		if ((devId&0x07) == DSPEED_EXT) {
			tupleBody = *pData++;
			pDev->xspeed = tupleBody;
			while (tupleBody&EXT_BIT)
				tupleBody = *pData++;
		}

		/* parse extended devide type */
		if (((devId>>4)&0x0FF) == DTYPE_EXTEND) {
			tupleBody = *pData++;
			pDev->xdevice = tupleBody;
			while (tupleBody&EXT_BIT)
				tupleBody = *pData++;
		}

		/* parse device size field */
		pDev->deviceSize = *pData++;

		devId = *pData++;
	}

	return(DVB_CI_COMPLIANT);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         CIS_ParseManfid

   DESCRIPTION:  Parse CISTPL_MANFID tuple.

   PARAMETERS:   mId     = module id
                 pBuf    = pointer to tuple buffer

   RETURN VALUE: DVB_CI_COMPLIANT     = CI compliant
                 DVB_CI_NOT_COMPLIANT = not CI compliant
                                                                              */
/*----------------------------------------------------------------------------*/
static TPL_STATUS CIS_ParseManfid(MOD_ID mId, UINT8 *pBuf)
{
	MANFID	*pManfid;
	UINT8	*pData;
	UINT8	tupleLen;

	pManfid  = &CIS_infoManfid[mId];
	pData    = ++pBuf;
	tupleLen = *pData++;

	pManfid->manf =  ( (UINT16) (*pData++) )    & 0x00FF;
	pManfid->manf |= (((UINT16) (*pData++))<<8) & 0xFF00;
	pManfid->card =  ( (UINT16) (*pData++) )    & 0x00FF;
	pManfid->card |= (((UINT16) (*pData  ))<<8) & 0xFF00;

	CI_SOC_MSG_CIS_PARSE("	Manufacturer ID         : 0x%04X\n", pManfid->manf );
	CI_SOC_MSG_CIS_PARSE("	Card ID & Revision Info : 0x%04X\n", pManfid->card );

	return(DVB_CI_COMPLIANT);
}
#endif

/*----------------------------------------------------------------------------*/
#define STR_LEN_MAX  	25

#define COMPATIBILITY 	11 /* "compatible[" */

#if 0 // CI+ 1.3
#define CIPLUS			8  /* "ciplus=1" */
#else
#define CIPLUS			7	/* "ciplus=" */
#define CIPROF			7 	/* "ciprof=" */
#endif

#if 0 //CI+1.3
static UINT8 CIS_IsItCIPlusCAM( UINT8 *i_pui8Buf, UINT32 i_ui32Len )
#else
static UINT8 CIS_IsItCIPlusCAM( VERS_1 *pVers1, UINT8 *i_pui8Buf, UINT32 i_ui32Len )
#endif
{
	UINT8 	iRet = 0;
#if 1 // CI+1.3
	UINT8 	i = 0;
	UINT32 	ui32Number = 0;
	char 	*pStr = NULL, *pSavePtrPlus = NULL, *pSavePtrProf = NULL;
#endif

	UINT8 *pui8Tmp = i_pui8Buf;
	UINT8 *pos;
	CHAR   cCompStr[ STR_LEN_MAX ];

	if ((i_pui8Buf == NULL) || !i_ui32Len) {
		return iRet;
	}

	memset( cCompStr, 0, STR_LEN_MAX );

	CIS_PrintBuf(i_pui8Buf, i_ui32Len);

	//Search '$'
#if 1
	while ('$' != *pui8Tmp++) {
		if( pui8Tmp >= (i_pui8Buf + i_ui32Len))
		{
			return iRet;
		}
	}
#else
	pos = (UINT8 *)strnchr(i_pui8Buf, i_ui32Len, '$');
	if (pos == NULL) {
		return iRet;
	}
#endif
	if ((pui8Tmp + COMPATIBILITY) >= (i_pui8Buf + i_ui32Len)) {
		return iRet;
	}

	memcpy( cCompStr, pui8Tmp, COMPATIBILITY );

	if( !strncasecmp( cCompStr, "compatible[", COMPATIBILITY ) )
	{
		pui8Tmp = pui8Tmp + COMPATIBILITY;

		memset( cCompStr, 0, STR_LEN_MAX );

		while( 1 )
		{
		#if 1
			while( ( 'c' != *pui8Tmp ) && ( 'C' != *pui8Tmp ) )
			{
				pui8Tmp++;

				if( pui8Tmp >= ( i_pui8Buf + i_ui32Len ) )
				{
					return iRet;
				}
			}
		#else
			pos = (UINT8 *)strnchr(pui8Tmp, i_ui32Len, 'c');
			if (pos == NULL) {
				pos = (UINT8 *)strnchr(pui8Tmp, i_ui32Len, 'C');
				if (pos == NULL) {
					return iRet;
				}
			}
			pui8Tmp = pos;
		#endif
#if 0 // CI+1.3
			if( ( pui8Tmp + CIPLUS ) >= ( i_pui8Buf + i_ui32Len ) ) {
				return iRet;
			}

			memcpy( cCompStr, pui8Tmp, CIPLUS );

			if (!strncasecmp( cCompStr, "ciplus=1", CIPLUS )) {
				/* it's CI plus CAM */
				iRet = 1;

				break;
			} else {
				memset( cCompStr, 0, STR_LEN_MAX );
			}
#else
			/* found 'c' or 'C" */

			/* copy 1 string until 'space' or ']'  */
			i = 0;
			while( (*pui8Tmp != ' ') && (*pui8Tmp != ']') && (pui8Tmp <= (i_pui8Buf + i_ui32Len)) )
			{
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"[%c]",*pui8Tmp);
				cCompStr[i++]=(*pui8Tmp++);
			}
			cCompStr[i]='\0';
		//	CI_SOC_MSG_CIS_CIPLUS("\n");
			CI_SOC_MSG_CIS_CIPLUS("[CI-CISTPL_VERS1] string :: %s\n",cCompStr);

			if( !strncasecmp( cCompStr, "ciplus=", CIPLUS ) )
			{
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"found ciplus=\n");

			//	CI_DTV_SOC_Message(CIS_CIPLUS,"org string :: %s\n",cCompStr);

				pStr = simple_strtok(cCompStr, "=", &pSavePtrPlus);
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"string 1 :: %s\n",pStr);

				pStr = simple_strtok(NULL, "\0", &pSavePtrPlus);
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"string 2 :: %s\n",pStr);

				if( (pStr[0] == '+') ||
					(pStr[0] == '-') ||
					(pStr[0] == '*') )
				{
					pStr = pStr + 1;
				}

				ui32Number = simple_strtoul( pStr, NULL, 10);

				/* is it correct? Check it if problem happened */
				if (ui32Number != 0) {
					/* it's CI plus CAM */
					iRet = 1;
				}

				CI_SOC_MSG_CIS_CIPLUS("[CI-CISTPL_VERS1] ciplus version = %d\n",ui32Number);

				pVers1->version = ui32Number;
			//	break;
			}
			else if (!strncasecmp( cCompStr, "ciprof=", CIPROF ))
			{
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"found ciprof=\n");
			//	CI_DTV_SOC_Message(CIS_CIPLUS,"string prof :: %s\n",pStr);

				if (!strncasecmp( cCompStr, "ciprof=0x", 9)) {
					pStr = simple_strtok(cCompStr, "x", &pSavePtrProf);
					pStr = simple_strtok(NULL, "\0", &pSavePtrProf);
					ui32Number = simple_strtoul( pStr, NULL, 16);

					CI_SOC_MSG_CIS_CIPLUS("[CI-CISTPL_VERS1] ciprof number = 0x%x\n",ui32Number);
				//	CI_SOC_MSG_CIS_CIPLUS("[CI-CISTPL_VERS1] ciprof number = %d\n",ui32Number);
				} else {
					pStr = simple_strtok(cCompStr, "=", &pSavePtrProf);
					pStr = simple_strtok(NULL, "\0", &pSavePtrProf);
					ui32Number = simple_strtoul( pStr, NULL, 10);

					CI_SOC_MSG_CIS_CIPLUS("[CI-CISTPL_VERS1] ciprof number = %d\n",ui32Number);
				}

				pVers1->profile = ui32Number;

			//	break;
			} else {
				memset( cCompStr, 0, STR_LEN_MAX );
			}

			if( (*pui8Tmp == '\0') ||
				(*pui8Tmp == ']') ||
				(pui8Tmp > (i_pui8Buf + i_ui32Len)) )
			{
				return iRet;
			}
#endif

			pui8Tmp++;
		}
	}

	return iRet;
}
