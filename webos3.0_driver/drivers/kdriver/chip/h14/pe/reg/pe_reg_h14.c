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

/** @file pe_reg_h14.c
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

#include "pe_hw_h14.h"
#include "pe_reg_h14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_REG_H14_CHECK_CODE(_checker,_action,fmt,args...)		\
	if (_checker)\
	{\
		PE_PRINT_ERROR(fmt,##args);\
		_action;\
	}

#define PE_REG_H14_PRINT_WRONG(_a,_d)	\
	PE_PRINT_WARN("[%x][%s,%d]wrong addr,data (0x%04x, 0x%08x)\n",PE_CHIP_VER,__F__,__L__,(_a),(_d))

#define PE_REG_H14_BLOCK_MASK		0xFF00
#define PE_REG_H14_DEFAULT_CASE(_a,_d)		default:PE_REG_H14_PRINT_WRONG(_a,_d);break
#define PE_REG_H14_RANGE_CHECK(_a,_m)		(((_a)&PE_REG_H14_BLOCK_MASK)==(_m))
#define PE_REG_H14_WR_P0L(_a,_f,_d)			case (_a):PE_P0L_H14_Wr(_f,_d);PE_P0L_H14_WrFL(_f);break
#define PE_REG_H14_WR_P0R(_a,_f,_d)			case (_a):PE_P0R_H14_Wr(_f,_d);PE_P0R_H14_WrFL(_f);break
#define PE_REG_H14_WR_P1L(_a,_f,_d)			case (_a):PE_P1L_H14_Wr(_f,_d);PE_P1L_H14_WrFL(_f);break
#define PE_REG_H14_WR_CVA(_a,_f,_d)			case (_a):PE_CVA_H14_Wr(_f,_d);PE_CVA_H14_WrFL(_f);break
#define PE_REG_H14_WR_CVB(_a,_f,_d)			case (_a):PE_CVB_H14_Wr(_f,_d);PE_CVB_H14_WrFL(_f);break
#define PE_REG_H14_WR_MSL(_a,_f,_d)			case (_a):PE_MSL_H14_Wr(_f,_d);PE_MSL_H14_WrFL(_f);break
#define PE_REG_H14_WR_MSR(_a,_f,_d)			case (_a):PE_MSR_H14_Wr(_f,_d);PE_MSR_H14_WrFL(_f);break

/* define for use the function, write address data using remmaped addr */
#undef PE_REG_H14_WR_ADDR_DATA_USING_REMAPED_ADDR

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
PE_CVA_REG_H14_T gPE_CVA_H14;
PE_CVB_REG_H14_T gPE_CVB_H14;
PE_P0L_REG_H14_T gPE_P0L_H14;
PE_P0R_REG_H14_T gPE_P0R_H14;
PE_MSL_REG_H14_T gPE_MSL_H14;
PE_MSR_REG_H14_T gPE_MSR_H14;
PE_SSC_REG_H14_T gPE_SSC_H14;
PE_OVL_REG_H14_T gPE_OVL_H14;
PE_OVR_REG_H14_T gPE_OVR_H14;
PE_P1L_REG_H14_T gPE_P1L_H14;
//PE_BVE_REG_H14_T gPE_BVE_H14;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#ifdef PE_REG_H14_WR_ADDR_DATA_USING_REMAPED_ADDR
static int PE_REG_H14_WriteRemappedAddr(UINT32 addr, UINT32 value);
#endif

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
int PE_REG_H14_InitPhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H14)
	{
		gPE_CVA_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVA_REG_H14A0_T));
		gPE_CVB_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_CVB_REG_H14A0_T));
		gPE_P0L_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0L_REG_H14A0_T));
		gPE_P0R_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P0R_REG_H14A0_T));
		gPE_MSL_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSL_REG_H14A0_T));
		gPE_MSR_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_MSR_REG_H14A0_T));
		gPE_SSC_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_SSC_REG_H14A0_T));
		gPE_OVL_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVL_REG_H14A0_T));
		gPE_OVR_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_OVR_REG_H14A0_T));
		gPE_P1L_H14.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_P1L_REG_H14A0_T));
//		gPE_BVE_H14.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_BVE_REG_H14X_T));
		gPE_CVA_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_CVA_REG_H14_BASE, sizeof(PE_CVA_REG_H14A0_T));
		gPE_CVB_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_CVB_REG_H14_BASE, sizeof(PE_CVB_REG_H14A0_T));
		gPE_P0L_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_P0L_REG_H14_BASE, sizeof(PE_P0L_REG_H14A0_T));
		gPE_P0R_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_P0R_REG_H14_BASE, sizeof(PE_P0R_REG_H14A0_T));
		gPE_MSL_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_MSL_REG_H14_BASE, sizeof(PE_MSL_REG_H14A0_T));
		gPE_MSR_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_MSR_REG_H14_BASE, sizeof(PE_MSR_REG_H14A0_T));
		gPE_SSC_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_SSC_REG_H14_BASE, sizeof(PE_SSC_REG_H14A0_T));
		gPE_OVL_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_OVL_REG_H14_BASE, sizeof(PE_OVL_REG_H14A0_T));
		gPE_OVR_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_OVR_REG_H14_BASE, sizeof(PE_OVR_REG_H14A0_T));
		gPE_P1L_H14.phys.addr  = (volatile UINT32 *)ioremap(PE_P1L_REG_H14_BASE, sizeof(PE_P1L_REG_H14A0_T));
//		gPE_BVE_H14.phys.addr   = (volatile UINT32 *)ioremap(PE_BVE_REG_H14_BASE, sizeof(PE_BVE_REG_H14X_T));
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
int PE_REG_H14_FreePhyToVirt(void)
{
	OS_Free((void *)gPE_CVA_H14.shdw.addr);
	OS_Free((void *)gPE_CVB_H14.shdw.addr);
	OS_Free((void *)gPE_P0L_H14.shdw.addr);
	OS_Free((void *)gPE_P0R_H14.shdw.addr);
	OS_Free((void *)gPE_MSL_H14.shdw.addr);
	OS_Free((void *)gPE_MSR_H14.shdw.addr);
	OS_Free((void *)gPE_SSC_H14.shdw.addr);
	OS_Free((void *)gPE_OVL_H14.shdw.addr);
	OS_Free((void *)gPE_OVR_H14.shdw.addr);
	OS_Free((void *)gPE_P1L_H14.shdw.addr);
//	OS_Free((void *)gPE_BVE_H14.shdw.addr);
	if (gPE_CVA_H14.phys.addr) iounmap((void *)gPE_CVA_H14.phys.addr);
	if (gPE_CVB_H14.phys.addr) iounmap((void *)gPE_CVB_H14.phys.addr);
	if (gPE_P0L_H14.phys.addr) iounmap((void *)gPE_P0L_H14.phys.addr);
	if (gPE_P0R_H14.phys.addr) iounmap((void *)gPE_P0R_H14.phys.addr);
	if (gPE_MSL_H14.phys.addr) iounmap((void *)gPE_MSL_H14.phys.addr);
	if (gPE_MSR_H14.phys.addr) iounmap((void *)gPE_MSR_H14.phys.addr);
	if (gPE_SSC_H14.phys.addr) iounmap((void *)gPE_SSC_H14.phys.addr);
	if (gPE_OVL_H14.phys.addr) iounmap((void *)gPE_OVL_H14.phys.addr);
	if (gPE_OVR_H14.phys.addr) iounmap((void *)gPE_OVR_H14.phys.addr);
	if (gPE_P1L_H14.phys.addr) iounmap((void *)gPE_P1L_H14.phys.addr);
//	if (gPE_BVE_H14.phys.addr) iounmap((void *)gPE_BVE_H14.phys.addr);
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
int PE_REG_H14_WD(UINT32 addr, UINT32 value)
{
	#ifdef PE_REG_H14_WR_ADDR_DATA_USING_REMAPED_ADDR
	return PE_REG_H14_WriteRemappedAddr(addr,value);
	#else
	int ret = RET_OK;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_H14)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_H14_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_H14_BASE;
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
	#endif
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
int PE_REG_H14_WrAllocatedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H14AX)
		{
			if((addr)>=PE_CVA_REG_H14_OFFSET && (addr)<(PE_CVA_REG_H14_OFFSET+sizeof(PE_CVA_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_CVA_H14.phys.addr+((addr-PE_CVA_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_CVB_REG_H14_OFFSET && (addr)<(PE_CVB_REG_H14_OFFSET+sizeof(PE_CVB_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_CVB_H14.phys.addr+((addr-PE_CVA_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0L_REG_H14_OFFSET && (addr)<(PE_P0L_REG_H14_OFFSET+sizeof(PE_P0L_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_P0L_H14.phys.addr+((addr-PE_P0L_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P0R_REG_H14_OFFSET && (addr)<(PE_P0R_REG_H14_OFFSET+sizeof(PE_P0R_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_P0R_H14.phys.addr+((addr-PE_P0R_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSL_REG_H14_OFFSET && (addr)<(PE_MSL_REG_H14_OFFSET+sizeof(PE_MSL_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_MSL_H14.phys.addr+((addr-PE_MSL_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_MSR_REG_H14_OFFSET && (addr)<(PE_MSR_REG_H14_OFFSET+sizeof(PE_MSR_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_MSR_H14.phys.addr+((addr-PE_MSR_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_P1L_REG_H14_OFFSET && (addr)<(PE_P1L_REG_H14_OFFSET+sizeof(PE_P1L_REG_H14A0_T)))
			{
				*((UINT32 *)(gPE_P1L_H14.phys.addr+((addr-PE_P1L_REG_H14_OFFSET)/sizeof(UINT32)))) = value;
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
UINT32 PE_REG_H14_RD(UINT32 addr)
{
	UINT32 value = 0x0;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_H14)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_DTVSOC_DE_H14_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_DTVSOC_DE_H14_BASE;

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
int PE_REG_H14_WrReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr,data;
	do {
		CHECK_KNULL(pstParams);
		PE_REG_H14_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H14)
		{
			addr = pstParams->data[0];
			data = pstParams->data[1];
			ret = PE_REG_H14_WD(addr,data);
			PE_REG_H14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_H14_WD() error.\n",__F__,__L__);
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
int PE_REG_H14_RdReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr;
	do{
		CHECK_KNULL(pstParams);
		PE_REG_H14_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H14)
		{
			addr = pstParams->data[0];
			pstParams->data[1] = PE_REG_H14_RD(addr);
			PE_PRINT_DEBUG("get : addr:0x%x, data:0x%x\n",addr,pstParams->data[1]);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	return RET_OK;
}

#ifdef PE_REG_H14_WR_ADDR_DATA_USING_REMAPED_ADDR
/**
 * write address data using remmaped addr
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_REG_H14_WriteRemappedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	if(PE_REG_H14_RANGE_CHECK(addr,0x0400))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0L(0x440,p0l_tnr_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x444,p0l_tnr_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x448,p0l_tnr_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x44c,p0l_tnr_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x450,p0l_tnr_ctrl_04,value);
			PE_REG_H14_WR_P0L(0x454,p0l_tnr_ctrl_05,value);
			PE_REG_H14_WR_P0L(0x458,p0l_tnr_ctrl_06,value);
			PE_REG_H14_WR_P0L(0x45c,p0l_tnr_ctrl_07,value);
			PE_REG_H14_WR_P0L(0x460,p0l_tnr_ctrl_08,value);
			PE_REG_H14_WR_P0L(0x464,p0l_tnr_ctrl_09,value);
			PE_REG_H14_WR_P0L(0x468,p0l_tnr_ctrl_10,value);
			PE_REG_H14_WR_P0L(0x46c,p0l_tnr_ctrl_11,value);
			PE_REG_H14_WR_P0L(0x470,p0l_tnr_ctrl_12,value);
			PE_REG_H14_WR_P0L(0x474,p0l_tnr_ctrl_13,value);
			PE_REG_H14_WR_P0L(0x478,p0l_tnr_ctrl_14,value);
			PE_REG_H14_WR_P0L(0x47c,p0l_tnr_ctrl_15,value);
			PE_REG_H14_WR_P0L(0x480,p0l_tnr_ctrl_16,value);
			PE_REG_H14_WR_P0L(0x484,p0l_tnr_ctrl_17,value);
			PE_REG_H14_WR_P0L(0x488,p0l_tnr_ctrl_18,value);
			PE_REG_H14_WR_P0L(0x48c,p0l_tnr_ctrl_19,value);
			PE_REG_H14_WR_P0L(0x490,p0l_tnr_ctrl_20,value);
			PE_REG_H14_WR_P0L(0x494,p0l_tnr_ctrl_21,value);
			PE_REG_H14_WR_P0L(0x498,p0l_tnr_ctrl_22,value);
			PE_REG_H14_WR_P0L(0x49c,p0l_tnr_ctrl_23,value);
			PE_REG_H14_WR_P0L(0x4a0,p0l_tnr_ctrl_24,value);
			PE_REG_H14_WR_P0L(0x4a4,p0l_tnr_ctrl_25,value);
			PE_REG_H14_WR_P0L(0x4a8,p0l_tnr_ctrl_26,value);
			PE_REG_H14_WR_P0L(0x4ac,p0l_tnr_ctrl_27,value);
			PE_REG_H14_WR_P0L(0x4b0,p0l_tnr_ctrl_28,value);
			PE_REG_H14_WR_P0L(0x4b4,p0l_tnr_ctrl_29,value);
			PE_REG_H14_WR_P0L(0x4b8,p0l_tnr_ctrl_30,value);
			PE_REG_H14_WR_P0L(0x4bc,p0l_tnr_ctrl_31,value);
			PE_REG_H14_WR_P0L(0x4d0,p0l_tnr_main_lut_00,value);
			PE_REG_H14_WR_P0L(0x4d4,p0l_tnr_main_lut_01,value);
			PE_REG_H14_WR_P0L(0x4d8,p0l_tnr_main_lut_02,value);
			PE_REG_H14_WR_P0L(0x4e0,p0l_tnr_ctrl_32,value);
			PE_REG_H14_WR_P0L(0x4e4,p0l_tnr_ctrl_33,value);
			PE_REG_H14_WR_P0L(0x4e8,p0l_tnr_ctrl_34,value);
			PE_REG_H14_WR_P0L(0x4ec,p0l_tnr_ctrl_35,value);
			PE_REG_H14_WR_P0L(0x4f0,p0l_tnr_ctrl_36,value);
			PE_REG_H14_WR_P0L(0x4f4,p0l_tnr_ctrl_37,value);
			PE_REG_H14_WR_P0L(0x4f8,p0l_tnr_ctrl_38,value);
			PE_REG_H14_WR_P0L(0x4fc,p0l_tnr_ctrl_39,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x0500))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0L(0x500,p0l_ipc_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x504,p0l_ipc_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x508,p0l_ipc_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x50c,p0l_ipc_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x510,p0l_ipc_ctrl_04,value);
			PE_REG_H14_WR_P0L(0x514,p0l_ipc_ctrl_05,value);
			PE_REG_H14_WR_P0L(0x518,p0l_ipc_ctrl_06,value);
			PE_REG_H14_WR_P0L(0x51c,p0l_ipc_ctrl_07,value);
			PE_REG_H14_WR_P0L(0x520,p0l_ipc_ctrl_08,value);
			PE_REG_H14_WR_P0L(0x524,p0l_ipc_ctrl_09,value);
			PE_REG_H14_WR_P0L(0x528,p0l_ipc_ctrl_10,value);
			PE_REG_H14_WR_P0L(0x52c,p0l_ipc_ctrl_11,value);
			PE_REG_H14_WR_P0L(0x530,p0l_ipc_ctrl_12,value);
			PE_REG_H14_WR_P0L(0x534,p0l_ipc_ctrl_13,value);
			PE_REG_H14_WR_P0L(0x538,p0l_ipc_ctrl_14,value);
			PE_REG_H14_WR_P0L(0x53c,p0l_ipc_ctrl_15,value);
			PE_REG_H14_WR_P0L(0x540,p0l_ipc_ctrl_16,value);
			PE_REG_H14_WR_P0L(0x544,p0l_ipc_ctrl_17,value);
			PE_REG_H14_WR_P0L(0x548,p0l_ipc_ctrl_18,value);
			PE_REG_H14_WR_P0L(0x54c,p0l_ipc_ctrl_19,value);
			PE_REG_H14_WR_P0L(0x560,p0l_clc_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x564,p0l_clc_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x568,p0l_clc_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x56c,p0l_clc_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x570,p0l_clc_ctrl_04,value);
			PE_REG_H14_WR_P0L(0x574,p0l_clc_ctrl_05,value);
			PE_REG_H14_WR_P0L(0x578,p0l_clc_ctrl_06,value);
			PE_REG_H14_WR_P0L(0x57c,p0l_clc_stat_00,value);
			PE_REG_H14_WR_P0L(0x580,p0l_vflt_ctrl,value);
			PE_REG_H14_WR_P0L(0x584,p0l_cti_ctrl_1,value);
			PE_REG_H14_WR_P0L(0x588,p0l_mnr_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x58c,p0l_mnr_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x590,p0l_mnr_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x594,p0l_mnr_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x598,p0l_mnr_ctrl_04,value);
			PE_REG_H14_WR_P0L(0x59c,p0l_mnr_ctrl_05,value);
			PE_REG_H14_WR_P0L(0x5a0,p0l_bnr_detail_ctrl,value);
			PE_REG_H14_WR_P0L(0x5a4,p0l_vfilter_02,value);
			PE_REG_H14_WR_P0L(0x5a8,p0l_vfilter_01,value);
			PE_REG_H14_WR_P0L(0x5ac,p0l_bnr_dc_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x5b0,p0l_bnr_dc_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x5b4,p0l_bnr_ac_ctrl_07,value);
			PE_REG_H14_WR_P0L(0x5b8,p0l_bnr_ac_ctrl_08,value);
			PE_REG_H14_WR_P0L(0x5bc,p0l_bnr_ac_ctrl_06,value);
			PE_REG_H14_WR_P0L(0x5c0,p0l_bnr_ac_ctrl_00,value);
			PE_REG_H14_WR_P0L(0x5c4,p0l_bnr_ac_ctrl_01,value);
			PE_REG_H14_WR_P0L(0x5c8,p0l_bnr_ac_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x5cc,p0l_bnr_ac_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x5d0,p0l_bnr_ac_ctrl_04,value);
			PE_REG_H14_WR_P0L(0x5d4,p0l_bnr_ac_ctrl_05,value);
			PE_REG_H14_WR_P0L(0x5d8,p0l_dnr_max_ctrl,value);
			PE_REG_H14_WR_P0L(0x5dc,p0l_dnr_dbar_ctrl,value);
			PE_REG_H14_WR_P0L(0x5e4,p0l_cti_ctrl_0,value);
			PE_REG_H14_WR_P0L(0x5e8,p0l_reg_c420_ctrl,value);
			PE_REG_H14_WR_P0L(0x5f0,p0l_ipc_ctrl_20,value);
			PE_REG_H14_WR_P0L(0x5f4,p0l_ipc_ctrl_21,value);
			PE_REG_H14_WR_P0L(0x5f8,p0l_ipc_ctrl_22,value);
			PE_REG_H14_WR_P0L(0x5fc,p0l_ipc_ctrl_23,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x0600))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0L(0x600,p0l_ipc_ctrl_24,value);
			PE_REG_H14_WR_P0L(0x604,p0l_ipc_ctrl_25,value);
			PE_REG_H14_WR_P0L(0x608,p0l_ipc_ctrl_26,value);
			PE_REG_H14_WR_P0L(0x60c,p0l_ipc_ctrl_27,value);
			PE_REG_H14_WR_P0L(0x610,p0l_ipc_ctrl_28,value);
			PE_REG_H14_WR_P0L(0x614,p0l_ipc_ctrl_29,value);
			PE_REG_H14_WR_P0L(0x618,p0l_ipc_ctrl_30,value);
			PE_REG_H14_WR_P0L(0x61c,p0l_ipc_ctrl_31,value);
			PE_REG_H14_WR_P0L(0x66c,p0l_clc_ctrl_07,value);
			PE_REG_H14_WR_P0L(0x688,p0l_bnr_dc_ctrl_02,value);
			PE_REG_H14_WR_P0L(0x68c,p0l_bnr_dc_ctrl_03,value);
			PE_REG_H14_WR_P0L(0x69c,p0l_bnr_ac_ctrl_09,value);
			PE_REG_H14_WR_P0L(0x6d0,p0l_tnr_ctrl_40,value);
			PE_REG_H14_WR_P0L(0x6d4,p0l_tnr_ctrl_41,value);
			PE_REG_H14_WR_P0L(0x6d8,p0l_tnr_ctrl_42,value);
			PE_REG_H14_WR_P0L(0x6dc,p0l_tnr_ctrl_43,value);
			PE_REG_H14_WR_P0L(0x6e0,p0l_tnr_ctrl_44,value);
			PE_REG_H14_WR_P0L(0x6e4,p0l_tnr_ctrl_45,value);
			PE_REG_H14_WR_P0L(0x6e8,p0l_tnr_ctrl_46,value);
			PE_REG_H14_WR_P0L(0x6ec,p0l_tnr_ctrl_47,value);
			PE_REG_H14_WR_P0L(0x6f0,p0l_tnr_ctrl_48,value);
			PE_REG_H14_WR_P0L(0x6f4,p0l_tnr_ctrl_49,value);
			PE_REG_H14_WR_P0L(0x6f8,p0l_tnr_ctrl_50,value);
			PE_REG_H14_WR_P0L(0x6fc,p0l_tnr_ctrl_51,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x2400))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0R(0x2440,p0r_tnr_ctrl_00,value);
			PE_REG_H14_WR_P0R(0x2444,p0r_tnr_ctrl_01,value);
			PE_REG_H14_WR_P0R(0x2448,p0r_tnr_ctrl_02,value);
			PE_REG_H14_WR_P0R(0x244c,p0r_tnr_ctrl_03,value);
			PE_REG_H14_WR_P0R(0x2450,p0r_tnr_ctrl_04,value);
			PE_REG_H14_WR_P0R(0x2454,p0r_tnr_ctrl_05,value);
			PE_REG_H14_WR_P0R(0x2458,p0r_tnr_ctrl_06,value);
			PE_REG_H14_WR_P0R(0x245c,p0r_tnr_ctrl_07,value);
			PE_REG_H14_WR_P0R(0x2460,p0r_tnr_ctrl_08,value);
			PE_REG_H14_WR_P0R(0x2464,p0r_tnr_ctrl_09,value);
			PE_REG_H14_WR_P0R(0x2468,p0r_tnr_ctrl_10,value);
			PE_REG_H14_WR_P0R(0x246c,p0r_tnr_ctrl_11,value);
			PE_REG_H14_WR_P0R(0x2470,p0r_tnr_ctrl_12,value);
			PE_REG_H14_WR_P0R(0x2474,p0r_tnr_ctrl_13,value);
			PE_REG_H14_WR_P0R(0x2478,p0r_tnr_ctrl_14,value);
			PE_REG_H14_WR_P0R(0x247c,p0r_tnr_ctrl_15,value);
			PE_REG_H14_WR_P0R(0x2480,p0r_tnr_ctrl_16,value);
			PE_REG_H14_WR_P0R(0x2484,p0r_tnr_ctrl_17,value);
			PE_REG_H14_WR_P0R(0x2488,p0r_tnr_ctrl_18,value);
			PE_REG_H14_WR_P0R(0x248c,p0r_tnr_ctrl_19,value);
			PE_REG_H14_WR_P0R(0x2490,p0r_tnr_ctrl_20,value);
			PE_REG_H14_WR_P0R(0x2494,p0r_tnr_ctrl_21,value);
			PE_REG_H14_WR_P0R(0x2498,p0r_tnr_ctrl_22,value);
			PE_REG_H14_WR_P0R(0x249c,p0r_tnr_ctrl_23,value);
			PE_REG_H14_WR_P0R(0x24a0,p0r_tnr_ctrl_24,value);
			PE_REG_H14_WR_P0R(0x24a4,p0r_tnr_ctrl_25,value);
			PE_REG_H14_WR_P0R(0x24a8,p0r_tnr_ctrl_26,value);
			PE_REG_H14_WR_P0R(0x24ac,p0r_tnr_ctrl_27,value);
			PE_REG_H14_WR_P0R(0x24b0,p0r_tnr_ctrl_28,value);
			PE_REG_H14_WR_P0R(0x24b4,p0r_tnr_ctrl_29,value);
			PE_REG_H14_WR_P0R(0x24b8,p0r_tnr_ctrl_30,value);
			PE_REG_H14_WR_P0R(0x24bc,p0r_tnr_ctrl_31,value);
			PE_REG_H14_WR_P0R(0x24d0,p0r_tnr_main_lut_00,value);
			PE_REG_H14_WR_P0R(0x24d4,p0r_tnr_main_lut_01,value);
			PE_REG_H14_WR_P0R(0x24d8,p0r_tnr_main_lut_02,value);
			PE_REG_H14_WR_P0R(0x24dc,p0r_tnr_status_04,value);
			PE_REG_H14_WR_P0R(0x24e0,p0r_tnr_ctrl_32,value);
			PE_REG_H14_WR_P0R(0x24e4,p0r_tnr_ctrl_33,value);
			PE_REG_H14_WR_P0R(0x24e8,p0r_tnr_ctrl_34,value);
			PE_REG_H14_WR_P0R(0x24ec,p0r_tnr_ctrl_35,value);
			PE_REG_H14_WR_P0R(0x24f0,p0r_tnr_ctrl_36,value);
			PE_REG_H14_WR_P0R(0x24f4,p0r_tnr_ctrl_37,value);
			PE_REG_H14_WR_P0R(0x24f8,p0r_tnr_ctrl_38,value);
			PE_REG_H14_WR_P0R(0x24fc,p0r_tnr_ctrl_39,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x2500))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0R(0x2500,p0r_ipc_ctrl_00,value);
			PE_REG_H14_WR_P0R(0x2504,p0r_ipc_ctrl_01,value);
			PE_REG_H14_WR_P0R(0x2508,p0r_ipc_ctrl_02,value);
			PE_REG_H14_WR_P0R(0x250c,p0r_ipc_ctrl_03,value);
			PE_REG_H14_WR_P0R(0x2510,p0r_ipc_ctrl_04,value);
			PE_REG_H14_WR_P0R(0x2514,p0r_ipc_ctrl_05,value);
			PE_REG_H14_WR_P0R(0x2518,p0r_ipc_ctrl_06,value);
			PE_REG_H14_WR_P0R(0x251c,p0r_ipc_ctrl_07,value);
			PE_REG_H14_WR_P0R(0x2520,p0r_ipc_ctrl_08,value);
			PE_REG_H14_WR_P0R(0x2524,p0r_ipc_ctrl_09,value);
			PE_REG_H14_WR_P0R(0x2528,p0r_ipc_ctrl_10,value);
			PE_REG_H14_WR_P0R(0x252c,p0r_ipc_ctrl_11,value);
			PE_REG_H14_WR_P0R(0x2530,p0r_ipc_ctrl_12,value);
			PE_REG_H14_WR_P0R(0x2534,p0r_ipc_ctrl_13,value);
			PE_REG_H14_WR_P0R(0x2538,p0r_ipc_ctrl_14,value);
			PE_REG_H14_WR_P0R(0x253c,p0r_ipc_ctrl_15,value);
			PE_REG_H14_WR_P0R(0x2540,p0r_ipc_ctrl_16,value);
			PE_REG_H14_WR_P0R(0x2544,p0r_ipc_ctrl_17,value);
			PE_REG_H14_WR_P0R(0x2548,p0r_ipc_ctrl_18,value);
			PE_REG_H14_WR_P0R(0x254c,p0r_ipc_ctrl_19,value);
			PE_REG_H14_WR_P0R(0x2580,p0r_vfilter_00,value);
			PE_REG_H14_WR_P0R(0x2584,p0r_cti_ctrl_1,value);
			PE_REG_H14_WR_P0R(0x2588,p0r_mnr_ctrl_00,value);
			PE_REG_H14_WR_P0R(0x258c,p0r_mnr_ctrl_01,value);
			PE_REG_H14_WR_P0R(0x2590,p0r_mnr_ctrl_02,value);
			PE_REG_H14_WR_P0R(0x2594,p0r_mnr_ctrl_03,value);
			PE_REG_H14_WR_P0R(0x2598,p0r_mnr_ctrl_04,value);
			PE_REG_H14_WR_P0R(0x259c,p0r_mnr_ctrl_05,value);
			PE_REG_H14_WR_P0R(0x25a0,p0r_bnr_detail_ctrl,value);
			PE_REG_H14_WR_P0R(0x25a4,p0r_vfilter_02,value);
			PE_REG_H14_WR_P0R(0x25a8,p0r_vfilter_01,value);
			PE_REG_H14_WR_P0R(0x25ac,p0r_bnr_dc_ctrl_00,value);
			PE_REG_H14_WR_P0R(0x25b0,p0r_bnr_dc_ctrl_01,value);
			PE_REG_H14_WR_P0R(0x25b4,p0r_bnr_ac_ctrl_07,value);
			PE_REG_H14_WR_P0R(0x25b8,p0r_bnr_ac_ctrl_08,value);
			PE_REG_H14_WR_P0R(0x25bc,p0r_bnr_ac_ctrl_06,value);
			PE_REG_H14_WR_P0R(0x25c0,p0r_bnr_ac_ctrl_00,value);
			PE_REG_H14_WR_P0R(0x25c4,p0r_bnr_ac_ctrl_01,value);
			PE_REG_H14_WR_P0R(0x25c8,p0r_bnr_ac_ctrl_02,value);
			PE_REG_H14_WR_P0R(0x25cc,p0r_bnr_ac_ctrl_03,value);
			PE_REG_H14_WR_P0R(0x25d0,p0r_bnr_ac_ctrl_04,value);
			PE_REG_H14_WR_P0R(0x25d4,p0r_bnr_ac_ctrl_05,value);
			PE_REG_H14_WR_P0R(0x25d8,p0r_dnr_max_ctrl,value);
			PE_REG_H14_WR_P0R(0x25dc,p0r_dnr_dbar_ctrl,value);
			PE_REG_H14_WR_P0R(0x25e4,p0r_cti_ctrl_0,value);
			PE_REG_H14_WR_P0R(0x25e8,p0r_reg_c420_ctrl,value);
			PE_REG_H14_WR_P0R(0x25f0,p0r_ipc_ctrl_20,value);
			PE_REG_H14_WR_P0R(0x25f4,p0r_ipc_ctrl_21,value);
			PE_REG_H14_WR_P0R(0x25f8,p0r_ipc_ctrl_22,value);
			PE_REG_H14_WR_P0R(0x25fc,p0r_ipc_ctrl_23,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x2600))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P0R(0x2600,p0r_ipc_ctrl_24,value);
			PE_REG_H14_WR_P0R(0x2604,p0r_ipc_ctrl_25,value);
			PE_REG_H14_WR_P0R(0x2608,p0r_ipc_ctrl_26,value);
			PE_REG_H14_WR_P0R(0x260c,p0r_ipc_ctrl_27,value);
			PE_REG_H14_WR_P0R(0x2610,p0r_ipc_ctrl_28,value);
			PE_REG_H14_WR_P0R(0x2614,p0r_ipc_ctrl_29,value);
			PE_REG_H14_WR_P0R(0x2618,p0r_ipc_ctrl_30,value);
			PE_REG_H14_WR_P0R(0x261c,p0r_ipc_ctrl_31,value);
			PE_REG_H14_WR_P0R(0x2688,p0r_bnr_dc_ctrl_02,value);
			PE_REG_H14_WR_P0R(0x268c,p0r_bnr_dc_ctrl_03,value);
			PE_REG_H14_WR_P0R(0x269c,p0r_bnr_ac_ctrl_09,value);
			PE_REG_H14_WR_P0R(0x26d0,p0r_tnr_ctrl_40,value);
			PE_REG_H14_WR_P0R(0x26d4,p0r_tnr_ctrl_41,value);
			PE_REG_H14_WR_P0R(0x26d8,p0r_tnr_ctrl_42,value);
			PE_REG_H14_WR_P0R(0x26dc,p0r_tnr_ctrl_43,value);
			PE_REG_H14_WR_P0R(0x26e0,p0r_tnr_ctrl_44,value);
			PE_REG_H14_WR_P0R(0x26e4,p0r_tnr_ctrl_45,value);
			PE_REG_H14_WR_P0R(0x26e8,p0r_tnr_ctrl_46,value);
			PE_REG_H14_WR_P0R(0x26ec,p0r_tnr_ctrl_47,value);
			PE_REG_H14_WR_P0R(0x26f0,p0r_tnr_ctrl_48,value);
			PE_REG_H14_WR_P0R(0x26f4,p0r_tnr_ctrl_49,value);
			PE_REG_H14_WR_P0R(0x26f8,p0r_tnr_ctrl_50,value);
			PE_REG_H14_WR_P0R(0x26fc,p0r_tnr_ctrl_51,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x5300))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P1L(0x53d0,pe1_derh_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x53d4,pe1_derh_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x53d8,pe1_derh_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x53dc,pe1_derh_ctrl_03,value);
			PE_REG_H14_WR_P1L(0x53e0,pe1_derh_ctrl_04,value);
			PE_REG_H14_WR_P1L(0x53e4,pe1_derh_ctrl_05,value);
			PE_REG_H14_WR_P1L(0x53e8,pe1_derh_ctrl_06,value);
			PE_REG_H14_WR_P1L(0x53ec,pe1_derh_ctrl_07,value);
			PE_REG_H14_WR_P1L(0x53f0,pe1_derh_ctrl_08,value);
			PE_REG_H14_WR_P1L(0x53f4,pe1_derh_ctrl_09,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x5400))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P1L(0x5400,pe1_derv_ctrl_0,value);
			PE_REG_H14_WR_P1L(0x5404,pe1_derv_ctrl_1,value);
			PE_REG_H14_WR_P1L(0x5408,pe1_derv_ctrl_2,value);
			PE_REG_H14_WR_P1L(0x540c,pe1_derv_ctrl_3,value);
			PE_REG_H14_WR_P1L(0x5410,pe1_dp_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5414,pe1_dp_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x5420,pe1_coring_stat_00,value);
			PE_REG_H14_WR_P1L(0x5424,pe1_coring_stat_01,value);
			PE_REG_H14_WR_P1L(0x5470,pe1_derh_ctrl_0a,value);
			PE_REG_H14_WR_P1L(0x5474,pe1_derh_ctrl_0b,value);
			PE_REG_H14_WR_P1L(0x5478,pe1_derh_ctrl_0c,value);
			PE_REG_H14_WR_P1L(0x547c,pe1_derh_ctrl_0d,value);
			PE_REG_H14_WR_P1L(0x5490,pe1_sp_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5494,pe1_sp_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x5498,pe1_sp_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x549c,pe1_sp_ctrl_03,value);
			PE_REG_H14_WR_P1L(0x54a0,pe1_sp_ctrl_04,value);
			PE_REG_H14_WR_P1L(0x54a8,pe1_sp_ctrl_05,value);
			PE_REG_H14_WR_P1L(0x54ac,pe1_sp_ctrl_06,value);
			PE_REG_H14_WR_P1L(0x54b0,pe1_sp_ctrl_07,value);
			PE_REG_H14_WR_P1L(0x54b4,pe1_sp_ctrl_08,value);
			PE_REG_H14_WR_P1L(0x54b8,pe1_sp_ctrl_09,value);
			PE_REG_H14_WR_P1L(0x54bc,pe1_sp_ctrl_0a,value);
			PE_REG_H14_WR_P1L(0x54c0,pe1_sp_ctrl_0b,value);
			PE_REG_H14_WR_P1L(0x54c4,pe1_sp_ctrl_0c,value);
			PE_REG_H14_WR_P1L(0x54d0,pe1_debug_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x54d4,pe1_debug_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x54d8,pe1_debug_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x54f0,pe1_mp_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x54f4,pe1_mp_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x54f8,pe1_mp_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x54fc,pe1_mp_ctrl_03,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x5500))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P1L(0x5500,pe1_mp_ctrl_04,value);
			PE_REG_H14_WR_P1L(0x5508,pe1_mp_ctrl_05,value);
			PE_REG_H14_WR_P1L(0x550c,pe1_mp_ctrl_06,value);
			PE_REG_H14_WR_P1L(0x5510,pe1_mp_ctrl_07,value);
			PE_REG_H14_WR_P1L(0x5514,pe1_mp_ctrl_08,value);
			PE_REG_H14_WR_P1L(0x5518,pe1_mp_ctrl_09,value);
			PE_REG_H14_WR_P1L(0x551c,pe1_mp_ctrl_0a,value);
			PE_REG_H14_WR_P1L(0x5520,pe1_mp_ctrl_0b,value);
			PE_REG_H14_WR_P1L(0x5524,pe1_mp_ctrl_0c,value);
			PE_REG_H14_WR_P1L(0x5530,pe1_cti_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5534,pe1_cti_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x5538,pe1_cti_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x5540,pe1_ti_ctrl_0,value);
			PE_REG_H14_WR_P1L(0x5544,pe1_ti_ctrl_1,value);
			PE_REG_H14_WR_P1L(0x5548,pe1_ti_ctrl_2,value);
			PE_REG_H14_WR_P1L(0x5550,pe1_blur_ctrl,value);
			PE_REG_H14_WR_P1L(0x5554,pe1_dce_blur_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5558,pe1_dce_blur_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x555c,pe1_dce_blur_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x5560,pe1_coring_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5564,pe1_coring_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x5568,pe1_coring_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x556c,pe1_coring_ctrl_03,value);
			PE_REG_H14_WR_P1L(0x5570,pe1_coring_ctrl_04,value);
			PE_REG_H14_WR_P1L(0x5574,pe1_coring_ctrl_05,value);
			PE_REG_H14_WR_P1L(0x5578,pe1_coring_ctrl_06,value);
			PE_REG_H14_WR_P1L(0x557c,pe1_coring_ctrl_07,value);
			PE_REG_H14_WR_P1L(0x5580,pe1_coring_ctrl_08,value);
			PE_REG_H14_WR_P1L(0x5584,pe1_coring_ctrl_09,value);
			PE_REG_H14_WR_P1L(0x5590,pe1_dj_ctrl_00,value);
			PE_REG_H14_WR_P1L(0x5594,pe1_dj_ctrl_01,value);
			PE_REG_H14_WR_P1L(0x5598,pe1_dj_ctrl_02,value);
			PE_REG_H14_WR_P1L(0x559c,pe1_dj_ctrl_03,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x5600))
	{
		switch(addr)
		{
			PE_REG_H14_WR_P1L(0x5614,pe1_cen_ia_ctrl,value);
			PE_REG_H14_WR_P1L(0x5618,pe1_cen_ia_data,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x1100))
	{
		switch(addr)
		{
			PE_REG_H14_WR_MSL(0x1174,msc_chroma_tear_l,value);
			PE_REG_H14_WR_MSL(0x1178,msc_chroma_tear_ctrl_l,value);
			PE_REG_H14_WR_MSL(0x1180,msc_min_max_ctrl_l_y,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x3100))
	{
		switch(addr)
		{
			PE_REG_H14_WR_MSR(0x3174,msc_chroma_tear_r,value);
			PE_REG_H14_WR_MSR(0x3178,msc_chroma_tear_ctrl_r,value);
			PE_REG_H14_WR_MSR(0x3180,msc_min_max_ctrl_r_y,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x100))
	{
		switch(addr)
		{
			PE_REG_H14_WR_CVA(0x184,cva_cti_ctrl,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else if(PE_REG_H14_RANGE_CHECK(addr,0x200))
	{
		switch(addr)
		{
			PE_REG_H14_WR_CVB(0x284,cvb_cti_ctrl,value);
			PE_REG_H14_DEFAULT_CASE(addr,value);
		}
	}
	else
	{
		PE_REG_H14_PRINT_WRONG(addr,value);
	}
	return ret;
}
#endif

