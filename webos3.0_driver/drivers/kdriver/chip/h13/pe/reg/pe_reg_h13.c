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

/** @file pe_reg_h13.c
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

#include "pe_hw_h13.h"
#include "pe_reg_h13.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_REG_H13_CHECK_CODE(_checker,_action,fmt,args...)		\
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
PE_CVA_REG_H13_T gPE_CVA_H13;
PE_CVB_REG_H13_T gPE_CVB_H13;
PE_CVA_REG_H13B_T gPE_CVA_H13B;
PE_CVB_REG_H13B_T gPE_CVB_H13B;
PE_P0L_REG_H13_T gPE_P0L_H13;
PE_P0R_REG_H13_T gPE_P0R_H13;
PE_P0L_REG_H13B_T gPE_P0L_H13B;
PE_P0R_REG_H13B_T gPE_P0R_H13B;
PE_MSL_REG_H13_T gPE_MSL_H13;
PE_MSR_REG_H13_T gPE_MSR_H13;
PE_MSL_REG_H13B_T gPE_MSL_H13B;
PE_MSR_REG_H13B_T gPE_MSR_H13B;
PE_SSC_REG_H13_T gPE_SSC_H13;
PE_OVL_REG_H13_T gPE_OVL_H13;
PE_OVR_REG_H13_T gPE_OVR_H13;
PE_OVL_REG_H13B_T gPE_OVL_H13B;
PE_OVR_REG_H13B_T gPE_OVR_H13B;
PE_DPP_REG_H13_T gPE_DPP_H13;
PE_P1L_REG_H13_T gPE_P1L_H13;
PE_P1L_REG_H13B_T gPE_P1L_H13B;
PE_BVE_REG_H13_T gPE_BVE_H13;

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
int PE_REG_H13_InitPhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H13)
	{
		gPE_CVA_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_CVA_REG_H13A0_T));
		gPE_CVB_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_CVB_REG_H13A0_T));
		gPE_CVA_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVA_REG_H13B0_T));
		gPE_CVB_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVB_REG_H13B0_T));
		gPE_P0L_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_P0L_REG_H13A0_T));
		gPE_P0R_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_P0R_REG_H13A0_T));
		gPE_P0L_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0L_REG_H13B0_T));
		gPE_P0R_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0R_REG_H13B0_T));
		gPE_MSL_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_MSL_REG_H13A0_T));
		gPE_MSR_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_MSR_REG_H13A0_T));
		gPE_MSL_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSL_REG_H13B0_T));
		gPE_MSR_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSR_REG_H13B0_T));
		gPE_SSC_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_SSC_REG_H13A0_T));
		gPE_OVL_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_OVL_REG_H13A0_T));
		gPE_OVR_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_OVR_REG_H13A0_T));
		gPE_OVL_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVL_REG_H13B0_T));
		gPE_OVR_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVR_REG_H13B0_T));
		gPE_DPP_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_DPP_REG_H13A0_T));
		gPE_P1L_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_P1L_REG_H13A0_T));
		gPE_P1L_H13B.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P1L_REG_H13B0_T));
		gPE_BVE_H13.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_BVE_REG_H13X_T));
		gPE_CVA_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_CVA_REG_H13_BASE, sizeof(PE_CVA_REG_H13A0_T));
		gPE_CVB_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_CVB_REG_H13_BASE, sizeof(PE_CVB_REG_H13A0_T));
		gPE_CVA_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_CVA_REG_H13_BASE, sizeof(PE_CVA_REG_H13B0_T));
		gPE_CVB_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_CVB_REG_H13_BASE, sizeof(PE_CVB_REG_H13B0_T));
		gPE_P0L_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_P0L_REG_H13_BASE, sizeof(PE_P0L_REG_H13A0_T));
		gPE_P0R_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_P0R_REG_H13_BASE, sizeof(PE_P0R_REG_H13A0_T));
		gPE_P0L_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_P0L_REG_H13_BASE, sizeof(PE_P0L_REG_H13B0_T));
		gPE_P0R_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_P0R_REG_H13_BASE, sizeof(PE_P0R_REG_H13B0_T));
		gPE_MSL_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_MSL_REG_H13_BASE, sizeof(PE_MSL_REG_H13A0_T));
		gPE_MSR_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_MSR_REG_H13_BASE, sizeof(PE_MSR_REG_H13A0_T));
		gPE_MSL_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_MSL_REG_H13_BASE, sizeof(PE_MSL_REG_H13B0_T));
		gPE_MSR_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_MSR_REG_H13_BASE, sizeof(PE_MSR_REG_H13B0_T));
		gPE_SSC_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_SSC_REG_H13_BASE, sizeof(PE_SSC_REG_H13A0_T));
		gPE_OVL_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_OVL_REG_H13_BASE, sizeof(PE_OVL_REG_H13A0_T));
		gPE_OVR_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_OVR_REG_H13_BASE, sizeof(PE_OVR_REG_H13A0_T));
		gPE_OVL_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_OVL_REG_H13_BASE, sizeof(PE_OVL_REG_H13B0_T));
		gPE_OVR_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_OVR_REG_H13_BASE, sizeof(PE_OVR_REG_H13B0_T));
		gPE_DPP_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_DPP_REG_H13_BASE, sizeof(PE_DPP_REG_H13A0_T));
		gPE_P1L_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_P1L_REG_H13_BASE, sizeof(PE_P1L_REG_H13A0_T));
		gPE_P1L_H13B.phys.addr  = (volatile UINT32 *)ioremap(PE_P1L_REG_H13_BASE, sizeof(PE_P1L_REG_H13B0_T));
		gPE_BVE_H13.phys.addr   = (volatile UINT32 *)ioremap(PE_BVE_REG_H13_BASE, sizeof(PE_BVE_REG_H13X_T));
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
int PE_REG_H13_FreePhyToVirt(void)
{
	OS_Free((void *)gPE_CVA_H13.shdw.addr);
	OS_Free((void *)gPE_CVB_H13.shdw.addr);
	OS_Free((void *)gPE_CVA_H13B.shdw.addr);
	OS_Free((void *)gPE_CVB_H13B.shdw.addr);
	OS_Free((void *)gPE_P0L_H13.shdw.addr);
	OS_Free((void *)gPE_P0R_H13.shdw.addr);
	OS_Free((void *)gPE_P0L_H13B.shdw.addr);
	OS_Free((void *)gPE_P0R_H13B.shdw.addr);
	OS_Free((void *)gPE_MSL_H13.shdw.addr);
	OS_Free((void *)gPE_MSR_H13.shdw.addr);
	OS_Free((void *)gPE_MSL_H13B.shdw.addr);
	OS_Free((void *)gPE_MSR_H13B.shdw.addr);
	OS_Free((void *)gPE_SSC_H13.shdw.addr);
	OS_Free((void *)gPE_OVL_H13.shdw.addr);
	OS_Free((void *)gPE_OVR_H13.shdw.addr);
	OS_Free((void *)gPE_OVL_H13B.shdw.addr);
	OS_Free((void *)gPE_OVR_H13B.shdw.addr);
	OS_Free((void *)gPE_DPP_H13.shdw.addr);
	OS_Free((void *)gPE_P1L_H13.shdw.addr);
	OS_Free((void *)gPE_P1L_H13B.shdw.addr);
	OS_Free((void *)gPE_BVE_H13.shdw.addr);
	if (gPE_CVA_H13.phys.addr) iounmap((void *)gPE_CVA_H13.phys.addr);
	if (gPE_CVB_H13.phys.addr) iounmap((void *)gPE_CVB_H13.phys.addr);	
	if (gPE_CVA_H13B.phys.addr) iounmap((void *)gPE_CVA_H13B.phys.addr);
	if (gPE_CVB_H13B.phys.addr) iounmap((void *)gPE_CVB_H13B.phys.addr);
	if (gPE_P0L_H13.phys.addr) iounmap((void *)gPE_P0L_H13.phys.addr);
	if (gPE_P0R_H13.phys.addr) iounmap((void *)gPE_P0R_H13.phys.addr);
	if (gPE_P0L_H13B.phys.addr) iounmap((void *)gPE_P0L_H13B.phys.addr);
	if (gPE_P0R_H13B.phys.addr) iounmap((void *)gPE_P0R_H13B.phys.addr);
	if (gPE_MSL_H13.phys.addr) iounmap((void *)gPE_MSL_H13.phys.addr);
	if (gPE_MSR_H13.phys.addr) iounmap((void *)gPE_MSR_H13.phys.addr);
	if (gPE_MSL_H13B.phys.addr) iounmap((void *)gPE_MSL_H13B.phys.addr);
	if (gPE_MSR_H13B.phys.addr) iounmap((void *)gPE_MSR_H13B.phys.addr);
	if (gPE_SSC_H13.phys.addr) iounmap((void *)gPE_SSC_H13.phys.addr);
	if (gPE_OVL_H13.phys.addr) iounmap((void *)gPE_OVL_H13.phys.addr);
	if (gPE_OVR_H13.phys.addr) iounmap((void *)gPE_OVR_H13.phys.addr);
	if (gPE_OVL_H13B.phys.addr) iounmap((void *)gPE_OVL_H13B.phys.addr);
	if (gPE_OVR_H13B.phys.addr) iounmap((void *)gPE_OVR_H13B.phys.addr);
	if (gPE_DPP_H13.phys.addr) iounmap((void *)gPE_DPP_H13.phys.addr);
	if (gPE_P1L_H13.phys.addr) iounmap((void *)gPE_P1L_H13.phys.addr);
	if (gPE_P1L_H13B.phys.addr) iounmap((void *)gPE_P1L_H13B.phys.addr);
	if (gPE_BVE_H13.phys.addr) iounmap((void *)gPE_BVE_H13.phys.addr);
	return RET_OK;
}
/**
 * write address data
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_H13_WD(UINT32 addr, UINT32 value)
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
		if(PE_KDRV_VER_H13)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_H13_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_H13_BASE;
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
int PE_REG_H13_WrAllocatedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H13BX)
		{
			
			if((addr)>=PE_CVA_REG_H13_OFFSET && (addr)<(PE_CVA_REG_H13_OFFSET+sizeof(PE_CVA_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_CVA_H13B.phys.addr+((addr-PE_CVA_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_CVB_REG_H13_OFFSET && (addr)<(PE_CVB_REG_H13_OFFSET+sizeof(PE_CVB_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_CVB_H13B.phys.addr+((addr-PE_CVB_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0L_REG_H13_OFFSET && (addr)<(PE_P0L_REG_H13_OFFSET+sizeof(PE_P0L_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_P0L_H13B.phys.addr+((addr-PE_P0L_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0R_REG_H13_OFFSET && (addr)<(PE_P0R_REG_H13_OFFSET+sizeof(PE_P0R_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_P0R_H13B.phys.addr+((addr-PE_P0R_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSL_REG_H13_OFFSET && (addr)<(PE_MSL_REG_H13_OFFSET+sizeof(PE_MSL_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_MSL_H13B.phys.addr+((addr-PE_MSL_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSR_REG_H13_OFFSET && (addr)<(PE_MSR_REG_H13_OFFSET+sizeof(PE_MSR_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_MSR_H13B.phys.addr+((addr-PE_MSR_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_OVL_REG_H13_OFFSET && (addr)<(PE_OVL_REG_H13_OFFSET+sizeof(PE_OVL_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_OVL_H13B.phys.addr+((addr-PE_OVL_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_OVR_REG_H13_OFFSET && (addr)<(PE_OVR_REG_H13_OFFSET+sizeof(PE_OVR_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_OVR_H13B.phys.addr+((addr-PE_OVR_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P1L_REG_H13_OFFSET && (addr)<(PE_P1L_REG_H13_OFFSET+sizeof(PE_P1L_REG_H13B0_T)))
			{
				*((UINT32 *)(gPE_P1L_H13B.phys.addr+((addr-PE_P1L_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else
			{
				PE_PRINT_WARN("[%x][%s,%d]wrong addr,data (0x%04x, 0x%08x)\n",\
					PE_CHIP_VER,__F__,__L__,(addr),(value));
			}
		}
		else if(PE_KDRV_VER_H13AX)
		{
			if((addr)>=PE_CVA_REG_H13_OFFSET && (addr)<(PE_CVA_REG_H13_OFFSET+sizeof(PE_CVA_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_CVA_H13.phys.addr+((addr-PE_CVA_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_CVB_REG_H13_OFFSET && (addr)<(PE_CVB_REG_H13_OFFSET+sizeof(PE_CVB_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_CVB_H13.phys.addr+((addr-PE_CVB_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0L_REG_H13_OFFSET && (addr)<(PE_P0L_REG_H13_OFFSET+sizeof(PE_P0L_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_P0L_H13.phys.addr+((addr-PE_P0L_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0R_REG_H13_OFFSET && (addr)<(PE_P0R_REG_H13_OFFSET+sizeof(PE_P0R_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_P0R_H13.phys.addr+((addr-PE_P0R_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSL_REG_H13_OFFSET && (addr)<(PE_MSL_REG_H13_OFFSET+sizeof(PE_MSL_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_MSL_H13.phys.addr+((addr-PE_MSL_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSR_REG_H13_OFFSET && (addr)<(PE_MSR_REG_H13_OFFSET+sizeof(PE_MSR_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_MSR_H13.phys.addr+((addr-PE_MSR_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_OVL_REG_H13_OFFSET && (addr)<(PE_OVL_REG_H13_OFFSET+sizeof(PE_OVL_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_OVL_H13.phys.addr+((addr-PE_OVL_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_OVR_REG_H13_OFFSET && (addr)<(PE_OVR_REG_H13_OFFSET+sizeof(PE_OVR_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_OVR_H13.phys.addr+((addr-PE_OVR_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P1L_REG_H13_OFFSET && (addr)<(PE_P1L_REG_H13_OFFSET+sizeof(PE_P1L_REG_H13A0_T)))
			{
				*((UINT32 *)(gPE_P1L_H13.phys.addr+((addr-PE_P1L_REG_H13_OFFSET)/sizeof(UINT32)))) = value;
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
UINT32 PE_REG_H13_RD(UINT32 addr)
{
	UINT32 value = 0x0;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_H13)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_H13_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_H13_BASE;

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
int PE_REG_H13_WrReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr,data;
	do {
		CHECK_KNULL(pstParams);
		PE_REG_H13_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H13)
		{
			addr = pstParams->data[0];
			data = pstParams->data[1];
			ret = PE_REG_H13_WD(addr,data);
			PE_REG_H13_CHECK_CODE(ret,break,"[%s,%d] PE_REG_H13_WD() error.\n",__F__,__L__);
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
int PE_REG_H13_RdReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr;
	do{
		CHECK_KNULL(pstParams);
		PE_REG_H13_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H13)
		{
			addr = pstParams->data[0];
			pstParams->data[1] = PE_REG_H13_RD(addr);
			PE_PRINT_NOTI("get : addr:0x%x, data:0x%x\n",addr,pstParams->data[1]);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	return RET_OK;
}

