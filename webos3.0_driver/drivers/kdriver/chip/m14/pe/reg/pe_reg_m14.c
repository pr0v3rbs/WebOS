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

/** @file pe_reg_m14.c
 *
 *  driver for picture enhance register read, write functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.06.11
 *	@see
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "be_cfg.h"
#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_inf_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_REG_M14_CHECK_CODE(_checker,_action,fmt,args...)		\
	if (_checker)\
	{\
		PE_PRINT_ERROR(fmt,##args);\
		_action;\
	}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
PE_CVA_REG_M14A_T gPE_CVA_M14A;
PE_CVB_REG_M14A_T gPE_CVB_M14A;
PE_P0L_REG_M14A_T gPE_P0L_M14A;
PE_P0R_REG_M14A_T gPE_P0R_M14A;
PE_MSL_REG_M14A_T gPE_MSL_M14A;
PE_MSR_REG_M14A_T gPE_MSR_M14A;
PE_SSC_REG_M14A_T gPE_SSC_M14A;
PE_OVL_REG_M14A_T gPE_OVL_M14A;
PE_OVR_REG_M14A_T gPE_OVR_M14A;
PE_P1L_REG_M14A_T gPE_P1L_M14A;
PE_CVA_REG_M14B_T gPE_CVA_M14B;
PE_CVB_REG_M14B_T gPE_CVB_M14B;
PE_P0M_REG_M14B_T gPE_P0M_M14B;
PE_P0D_REG_M14B_T gPE_P0D_M14B;
PE_P0S_REG_M14B_T gPE_P0S_M14B;
PE_MSC_REG_M14B_T gPE_MSC_M14B;
PE_SSC_REG_M14B_T gPE_SSC_M14B;
PE_PE1_REG_M14B_T gPE_PE1_M14B;
PE_LED_REG_M14B_T gPE_LED_M14B;
#ifdef PE_HW_M14_USE_M14B_WCP_REG
PE_OVL_REG_M14B_T gPE_OVL_M14B;
PE_OVR_REG_M14B_T gPE_OVR_M14B;
#endif
PE_BFW_INFO_REG_M14B_T gPE_BFW_INFO_M14B;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init phy to virtual addr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_M14_InitPhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_M14BX)
	{
		/*	gpBeMem->fw[5].fw_name, gpBeMem->fw[5].fw_base, sizeof(BE_FW_INFO_T));*/
		/* causion : should check base address */
		gPE_CVA_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVA_REG_M14B0_T));
		gPE_CVB_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVB_REG_M14B0_T));
		gPE_P0M_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0M_REG_M14B0_T));
		gPE_P0D_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0D_REG_M14B0_T));
		gPE_P0S_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0S_REG_M14B0_T));
		gPE_MSC_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSC_REG_M14B0_T));
		gPE_SSC_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_SSC_REG_M14B0_T));
		gPE_PE1_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_PE1_REG_M14B0_T));
		gPE_LED_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_LED_REG_M14B0_T));
		#ifdef PE_HW_M14_USE_M14B_WCP_REG
		gPE_OVL_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVL_REG_M14B0_T));
		gPE_OVR_M14B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVR_REG_M14B0_T));
		#endif
		gPE_CVA_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_CVA_REG_M14B_BASE, sizeof(PE_CVA_REG_M14B0_T));
		gPE_CVB_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_CVB_REG_M14B_BASE, sizeof(PE_CVB_REG_M14B0_T));
		gPE_P0M_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_P0M_REG_M14B_BASE, sizeof(PE_P0M_REG_M14B0_T));
		gPE_P0D_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_P0D_REG_M14B_BASE, sizeof(PE_P0D_REG_M14B0_T));
		gPE_P0S_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_P0S_REG_M14B_BASE, sizeof(PE_P0S_REG_M14B0_T));
		gPE_MSC_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_MSC_REG_M14B_BASE, sizeof(PE_MSC_REG_M14B0_T));
		gPE_SSC_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_SSC_REG_M14B_BASE, sizeof(PE_SSC_REG_M14B0_T));
		gPE_PE1_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_PE1_REG_M14B_BASE, sizeof(PE_PE1_REG_M14B0_T));
		gPE_LED_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_LED_REG_M14B_BASE, sizeof(PE_LED_REG_M14B0_T));
		#ifdef PE_HW_M14_USE_M14B_WCP_REG
		gPE_OVL_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_OVL_REG_M14B_BASE, sizeof(PE_OVL_REG_M14B0_T));
		gPE_OVR_M14B.phys.addr  = (volatile UINT32 *)ioremap(PE_OVR_REG_M14B_BASE, sizeof(PE_OVR_REG_M14B0_T));
		#endif
		gPE_BFW_INFO_M14B.phys.addr = (volatile UINT32 *)ioremap(gMemCfgBe[1].fw[5].fw_base, sizeof(BE_FW_INFO_T));
	}
	else if(PE_KDRV_VER_M14AX)
	{
		gPE_CVA_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVA_REG_M14A0_T));
		gPE_CVB_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVB_REG_M14A0_T));
		gPE_P0L_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0L_REG_M14A0_T));
		gPE_P0R_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0R_REG_M14A0_T));
		gPE_MSL_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSL_REG_M14A0_T));
		gPE_MSR_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSR_REG_M14A0_T));
		gPE_SSC_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_SSC_REG_M14A0_T));
		gPE_OVL_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVL_REG_M14A0_T));
		gPE_OVR_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVR_REG_M14A0_T));
		gPE_P1L_M14A.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P1L_REG_M14A0_T));
		gPE_CVA_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_CVA_REG_M14_BASE, sizeof(PE_CVA_REG_M14A0_T));
		gPE_CVB_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_CVB_REG_M14_BASE, sizeof(PE_CVB_REG_M14A0_T));
		gPE_P0L_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_P0L_REG_M14_BASE, sizeof(PE_P0L_REG_M14A0_T));
		gPE_P0R_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_P0R_REG_M14_BASE, sizeof(PE_P0R_REG_M14A0_T));
		gPE_MSL_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_MSL_REG_M14_BASE, sizeof(PE_MSL_REG_M14A0_T));
		gPE_MSR_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_MSR_REG_M14_BASE, sizeof(PE_MSR_REG_M14A0_T));
		gPE_SSC_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_SSC_REG_M14_BASE, sizeof(PE_SSC_REG_M14A0_T));
		gPE_OVL_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_OVL_REG_M14_BASE, sizeof(PE_OVL_REG_M14A0_T));
		gPE_OVR_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_OVR_REG_M14_BASE, sizeof(PE_OVR_REG_M14A0_T));
		gPE_P1L_M14A.phys.addr  = (volatile UINT32 *)ioremap(PE_P1L_REG_M14_BASE, sizeof(PE_P1L_REG_M14A0_T));
	}
	else
	{
		PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
	}
	return ret;
}
/**
 * free phy to virtual addr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_M14_FreePhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_M14BX)
	{
		OS_Free((void *)gPE_CVA_M14B.shdw.addr);
		OS_Free((void *)gPE_CVB_M14B.shdw.addr);
		OS_Free((void *)gPE_P0M_M14B.shdw.addr);
		OS_Free((void *)gPE_P0D_M14B.shdw.addr);
		OS_Free((void *)gPE_P0S_M14B.shdw.addr);
		OS_Free((void *)gPE_MSC_M14B.shdw.addr);
		OS_Free((void *)gPE_SSC_M14B.shdw.addr);
		OS_Free((void *)gPE_PE1_M14B.shdw.addr);
		OS_Free((void *)gPE_LED_M14B.shdw.addr);
		#ifdef PE_HW_M14_USE_M14B_WCP_REG
		OS_Free((void *)gPE_OVL_M14B.shdw.addr);
		OS_Free((void *)gPE_OVR_M14B.shdw.addr);
		#endif
		if (gPE_CVA_M14B.phys.addr) iounmap((void *)gPE_CVA_M14B.phys.addr);
		if (gPE_CVB_M14B.phys.addr) iounmap((void *)gPE_CVB_M14B.phys.addr);
		if (gPE_P0M_M14B.phys.addr) iounmap((void *)gPE_P0M_M14B.phys.addr);
		if (gPE_P0D_M14B.phys.addr) iounmap((void *)gPE_P0D_M14B.phys.addr);
		if (gPE_P0S_M14B.phys.addr) iounmap((void *)gPE_P0S_M14B.phys.addr);
		if (gPE_MSC_M14B.phys.addr) iounmap((void *)gPE_MSC_M14B.phys.addr);
		if (gPE_SSC_M14B.phys.addr) iounmap((void *)gPE_SSC_M14B.phys.addr);
		if (gPE_PE1_M14B.phys.addr) iounmap((void *)gPE_PE1_M14B.phys.addr);
		if (gPE_LED_M14B.phys.addr) iounmap((void *)gPE_LED_M14B.phys.addr);
		#ifdef PE_HW_M14_USE_M14B_WCP_REG
		if (gPE_OVL_M14B.phys.addr) iounmap((void *)gPE_OVL_M14B.phys.addr);
		if (gPE_OVR_M14B.phys.addr) iounmap((void *)gPE_OVR_M14B.phys.addr);
		#endif
		if (gPE_BFW_INFO_M14B.phys.addr) iounmap((void *)gPE_BFW_INFO_M14B.phys.addr);
	}
	else if(PE_KDRV_VER_M14AX)
	{
		OS_Free((void *)gPE_CVA_M14A.shdw.addr);
		OS_Free((void *)gPE_CVB_M14A.shdw.addr);
		OS_Free((void *)gPE_P0L_M14A.shdw.addr);
		OS_Free((void *)gPE_P0R_M14A.shdw.addr);
		OS_Free((void *)gPE_MSL_M14A.shdw.addr);
		OS_Free((void *)gPE_MSR_M14A.shdw.addr);
		OS_Free((void *)gPE_SSC_M14A.shdw.addr);
		OS_Free((void *)gPE_OVL_M14A.shdw.addr);
		OS_Free((void *)gPE_OVR_M14A.shdw.addr);
		OS_Free((void *)gPE_P1L_M14A.shdw.addr);
		if (gPE_CVA_M14A.phys.addr) iounmap((void *)gPE_CVA_M14A.phys.addr);
		if (gPE_CVB_M14A.phys.addr) iounmap((void *)gPE_CVB_M14A.phys.addr);
		if (gPE_P0L_M14A.phys.addr) iounmap((void *)gPE_P0L_M14A.phys.addr);
		if (gPE_P0R_M14A.phys.addr) iounmap((void *)gPE_P0R_M14A.phys.addr);
		if (gPE_MSL_M14A.phys.addr) iounmap((void *)gPE_MSL_M14A.phys.addr);
		if (gPE_MSR_M14A.phys.addr) iounmap((void *)gPE_MSR_M14A.phys.addr);
		if (gPE_SSC_M14A.phys.addr) iounmap((void *)gPE_SSC_M14A.phys.addr);
		if (gPE_OVL_M14A.phys.addr) iounmap((void *)gPE_OVL_M14A.phys.addr);
		if (gPE_OVR_M14A.phys.addr) iounmap((void *)gPE_OVR_M14A.phys.addr);
		if (gPE_P1L_M14A.phys.addr) iounmap((void *)gPE_P1L_M14A.phys.addr);
	}
	else
	{
		PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
	}
	return ret;
}
/**
 * write address data
 *	- use address instantly allocated
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_M14_WD(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_M14)
		{
			recvAddr = addr;
			if(PE_KDRV_VER_M14BX)
			{
				if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_M14B_BASE;
				else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_M14B_BASE;
			}
			else
			{
				if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_M14_BASE;
				else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_M14_BASE;
			}
			currAddr = recvAddr;
			currAddr >>= 2;
			currAddr <<= 2;

			virtAddr = (UINT32)ioremap(currAddr, 0x8);
			if (currAddr == recvAddr) {
				REG_WD(virtAddr, value);
				break;
			}
			currValue = REG_RD(virtAddr);
			nextValue = REG_RD((virtAddr+4));

			dBit = (recvAddr - currAddr)<<3;
			wBit = (32 - dBit);

			currValue  = GET_BITS(currValue ,0	  ,dBit);
			currValue += GET_PVAL(value		,dBit ,wBit);

			nextValue  = GET_PVAL(nextValue ,dBit ,wBit);
			nextValue += GET_BITS(value		,0	  ,dBit);
			REG_WD(virtAddr		,currValue);
			REG_WD((virtAddr+4) ,nextValue);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	if (virtAddr) iounmap((void *)virtAddr);
	return ret;
}

/**
 * write address data
 *	- use address already allocated
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_M14_WrAllocatedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_M14BX)
		{
			if((addr)>=PE_P0M_REG_M14B_OFFSET && (addr)<(PE_P0M_REG_M14B_OFFSET+sizeof(PE_P0M_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_P0M_M14B.phys.addr+((addr-PE_P0M_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0D_REG_M14B_OFFSET && (addr)<(PE_P0D_REG_M14B_OFFSET+sizeof(PE_P0D_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_P0D_M14B.phys.addr+((addr-PE_P0D_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0S_REG_M14B_OFFSET && (addr)<(PE_P0S_REG_M14B_OFFSET+sizeof(PE_P0S_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_P0S_M14B.phys.addr+((addr-PE_P0S_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSC_REG_M14B_OFFSET && (addr)<(PE_MSC_REG_M14B_OFFSET+sizeof(PE_MSC_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_MSC_M14B.phys.addr+((addr-PE_MSC_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_SSC_REG_M14B_OFFSET && (addr)<(PE_SSC_REG_M14B_OFFSET+sizeof(PE_SSC_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_SSC_M14B.phys.addr+((addr-PE_SSC_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_PE1_REG_M14B_OFFSET && (addr)<(PE_PE1_REG_M14B_OFFSET+sizeof(PE_PE1_REG_M14B0_T)))
			{
				*((UINT32 *)(gPE_PE1_M14B.phys.addr+((addr-PE_PE1_REG_M14B_OFFSET)/sizeof(UINT32)))) = value;
			}
			else
			{
				PE_PRINT_WARN("[%x][%s,%d]wrong addr,data (0x%04x, 0x%08x)\n",\
					PE_CHIP_VER,__F__,__L__,(addr),(value));
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if((addr)>=PE_CVA_REG_M14_OFFSET && (addr)<(PE_CVA_REG_M14_OFFSET+sizeof(PE_CVA_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_CVA_M14A.phys.addr+((addr-PE_CVA_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_CVB_REG_M14_OFFSET && (addr)<(PE_CVB_REG_M14_OFFSET+sizeof(PE_CVB_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_CVB_M14A.phys.addr+((addr-PE_CVA_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0L_REG_M14_OFFSET && (addr)<(PE_P0L_REG_M14_OFFSET+sizeof(PE_P0L_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_P0L_M14A.phys.addr+((addr-PE_P0L_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0R_REG_M14_OFFSET && (addr)<(PE_P0R_REG_M14_OFFSET+sizeof(PE_P0R_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_P0R_M14A.phys.addr+((addr-PE_P0R_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSL_REG_M14_OFFSET && (addr)<(PE_MSL_REG_M14_OFFSET+sizeof(PE_MSL_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_MSL_M14A.phys.addr+((addr-PE_MSL_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSR_REG_M14_OFFSET && (addr)<(PE_MSR_REG_M14_OFFSET+sizeof(PE_MSR_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_MSR_M14A.phys.addr+((addr-PE_MSR_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P1L_REG_M14_OFFSET && (addr)<(PE_P1L_REG_M14_OFFSET+sizeof(PE_P1L_REG_M14A0_T)))
			{
				*((UINT32 *)(gPE_P1L_M14A.phys.addr+((addr-PE_P1L_REG_M14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else
			{
				PE_PRINT_WARN("[%x][%s,%d]wrong addr,data (0x%04x, 0x%08x)\n",\
					PE_CHIP_VER,__F__,__L__,(addr),(value));
			}
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * read address data
 *
 * @param   addr [in] UINT32, accessing for register
 * @return  value of register
 * @see
 * @author
 */
UINT32 PE_REG_M14_RD(UINT32 addr)
{
	UINT32 value = 0x0;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_M14)
		{
			recvAddr = addr;
			if(PE_KDRV_VER_M14BX)
			{
				if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_M14B_BASE;
				else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_M14B_BASE;
			}
			else
			{
				if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_M14_BASE;
				else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_M14_BASE;
			}

			currAddr = recvAddr;
			currAddr >>= 2;
			currAddr <<= 2;
			virtAddr = (UINT32)ioremap(currAddr, 0x8);

			value = REG_RD(virtAddr);
			if (currAddr == recvAddr) break;

			nextValue = REG_RD(virtAddr+4);
			dBit = (recvAddr - currAddr)<<3;
			wBit = (32 - dBit);
			value  = GET_BITS(value, dBit, wBit);
			value += GET_PVAL(nextValue, wBit, dBit);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	if (virtAddr) iounmap((void *)virtAddr);
	return value;
}

/**
 * write address data for debug
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see
 * @author
 */
int PE_REG_M14_WrReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr,data;
	do {
		CHECK_KNULL(pstParams);
		PE_REG_M14_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_M14)
		{
			addr = pstParams->data[0];
			data = pstParams->data[1];
			ret = PE_REG_M14_WD(addr,data);
			PE_REG_M14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_M14_WD() error.\n",__F__,__L__);
			PE_PRINT_DEBUG("set : addr:0x%x, data:0x%x\n",addr,data);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	} while(0);
	return ret;
}
/**
 * read address data for debug
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see
 * @author
 */
int PE_REG_M14_RdReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr;
	do{
		CHECK_KNULL(pstParams);
		PE_REG_M14_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_M14)
		{
			addr = pstParams->data[0];
			pstParams->data[1] = PE_REG_M14_RD(addr);
			PE_PRINT_DEBUG("get : addr:0x%x, data:0x%x\n",addr,pstParams->data[1]);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	return RET_OK;
}

/**
 * set trace table
 *
 * @param   *fn [in] const char, func name
 * @param   line [in] UINT32
 * @param   *tbl [in] char, table name
 * @param   ofst [in] UINT32, offset
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */ 
int PE_REG_M14_SetTraceTable(const char* fn,
							UINT32 line,
							char *tbl,
							UINT32 ofst)
{
    int ret = RET_OK;
	char str[PE_TRACE_STR_SIZE];
	do{
		CHECK_KNULL(tbl);
		sprintf(str, "[%s,%d]%s(+ 0x%x)", fn, line, tbl, ofst);
		ret = PE_INF_M14_SetTraceDataBuf(str);
		PE_REG_M14_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_M14_SetTraceDataBuf() error.\n",__F__,__L__);
	}while (0);
	return ret;
}

