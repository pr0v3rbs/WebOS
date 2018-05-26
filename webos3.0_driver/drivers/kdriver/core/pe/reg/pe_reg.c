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

/** @file pe_reg.c
 *
 *  driver for picture enhance register read, write functions.
 *  ( used only within kdriver )
 *
 *	@author		Seung-Jun, Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.06.11
 *	@see
 */

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_cfg.h"
#include "pe_def.h"
#include "pe_reg.h"
#include "pe_reg_hw.h"

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------*/
typedef struct
{
	int (*init_phy_to_virt)(void);
	int (*free_phy_to_virt)(void);
	int (*wr_reg)(UINT32 addr, UINT32 value);
	int (*wr_alloc_reg)(UINT32 addr, UINT32 value);
	UINT32 (*rd_reg)(UINT32 addr);
	int (*set_trace_tbl)(const char* f, UINT32 l, char *t, UINT32 o);
}
PE_REG_CB_FUNC_T;

/*----------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------*/
static int PE_REG_WrAllocatedAddr(UINT32 addr, UINT32 value);
static UINT32 PE_REG_RD(UINT32 addr);

/*----------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------*/
static PE_REG_CB_FUNC_T _g_pe_reg_cb_fn;

/*============================================================================
	Implementation Group
============================================================================*/
/**
 * register call back functions
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_RegisterCbFunc(void)
{
	int ret = RET_OK;

	memset(&_g_pe_reg_cb_fn, 0, sizeof(PE_REG_CB_FUNC_T));
	do{
		#ifdef USE_PE_KDRV_CODES_FOR_H15
		if (PE_KDRV_VER_H15)
		{
			_g_pe_reg_cb_fn.init_phy_to_virt	= PE_REG_H15_InitPhyToVirt;
			_g_pe_reg_cb_fn.free_phy_to_virt	= PE_REG_H15_FreePhyToVirt;
			_g_pe_reg_cb_fn.wr_reg				= PE_REG_H15_WD;
			_g_pe_reg_cb_fn.wr_alloc_reg		= PE_REG_H15_WrAllocatedAddr;
			_g_pe_reg_cb_fn.rd_reg				= PE_REG_H15_RD;
			_g_pe_reg_cb_fn.set_trace_tbl		= PE_REG_H15_SetTraceTable;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_M14
		if (PE_KDRV_VER_M14)
		{
			_g_pe_reg_cb_fn.init_phy_to_virt	= PE_REG_M14_InitPhyToVirt;
			_g_pe_reg_cb_fn.free_phy_to_virt	= PE_REG_M14_FreePhyToVirt;
			_g_pe_reg_cb_fn.wr_reg				= PE_REG_M14_WD;
			_g_pe_reg_cb_fn.wr_alloc_reg		= PE_REG_M14_WrAllocatedAddr;
			_g_pe_reg_cb_fn.rd_reg				= PE_REG_M14_RD;
			_g_pe_reg_cb_fn.set_trace_tbl		= PE_REG_M14_SetTraceTable;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H14
		if (PE_KDRV_VER_H14)
		{
			_g_pe_reg_cb_fn.init_phy_to_virt	= PE_REG_H14_InitPhyToVirt;
			_g_pe_reg_cb_fn.free_phy_to_virt	= PE_REG_H14_FreePhyToVirt;
			_g_pe_reg_cb_fn.wr_reg				= PE_REG_H14_WD;
			_g_pe_reg_cb_fn.wr_alloc_reg		= PE_REG_H14_WrAllocatedAddr;
			_g_pe_reg_cb_fn.rd_reg				= PE_REG_H14_RD;
			_g_pe_reg_cb_fn.set_trace_tbl		= NULL;
		}
		else
		#endif
		#ifdef USE_PE_KDRV_CODES_FOR_H13
		if (PE_KDRV_VER_H13)
		{
			_g_pe_reg_cb_fn.init_phy_to_virt	= PE_REG_H13_InitPhyToVirt;
			_g_pe_reg_cb_fn.free_phy_to_virt	= PE_REG_H13_FreePhyToVirt;
			_g_pe_reg_cb_fn.wr_reg				= PE_REG_H13_WD;
			_g_pe_reg_cb_fn.wr_alloc_reg		= PE_REG_H13_WrAllocatedAddr;
			_g_pe_reg_cb_fn.rd_reg				= PE_REG_H13_RD;
			_g_pe_reg_cb_fn.set_trace_tbl		= NULL;
		}
		else
		#endif
		{
			PE_PRINT_ERROR("[%x,%x] kdrv chip ver is wrong.\n", \
				PE_CHIP_VER, g_pe_kdrv_ver_mask);
			ret = RET_ERROR;
		}
	}while (0);
	return ret;
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
int PE_REG_SetTraceTable(const char* fn,
						UINT32 line,
						char *tbl,
						UINT32 ofst)
{
    int ret = RET_OK;
	do{
		if(_g_pe_reg_cb_fn.set_trace_tbl)
		{
			ret = _g_pe_reg_cb_fn.set_trace_tbl(fn,	line, tbl, ofst);
			PE_CHECK_CODE(ret, break, "set_trace_tbl() error.\n");
		}
	}while (0);
	return ret;
}
/**
 * write address data set
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_SetTable(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_ERROR;
	UINT32 table_size;
	int i;
	do {
		PE_CHECK_CODE(!table, break, "table is null.\n");
		PE_CHECK_CODE(!size, break, "size is zero.\n");
		table_size = size/sizeof(PE_REG_PARAM_T);
		for (i=0;i<table_size;i++)
			PE_REG_WrAllocatedAddr((table[i].addr+offset), table[i].data);
		ret = RET_OK;
	}while (0);
	return ret;
}
/**
 * read address data set
 *
 * @param   *table [in/out] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_GetTable(PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_ERROR;
	UINT32 table_size;
	int i;
	do {
		PE_CHECK_CODE(!table, break, "table is null.\n");
		PE_CHECK_CODE(!size, break, "size is zero.\n");
		table_size = size/sizeof(PE_REG_PARAM_T);
		for (i=0;i<table_size;i++)
			table[i].data=PE_REG_RD(table[i].addr+offset);
		ret = RET_OK;
	}while (0);
	return ret;
}

/**
 * init phy to virtual addr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_InitPhyToVirt(void)
{
	int ret = RET_OK;
	do {
		if(_g_pe_reg_cb_fn.init_phy_to_virt)
		{
			ret = _g_pe_reg_cb_fn.init_phy_to_virt();
			PE_CHECK_CODE(ret, break, "init_phy_to_virt() error.\n");
		}
	}while (0);
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
int PE_REG_FreePhyToVirt(void)
{
	int ret = RET_OK;
	do {
		if(_g_pe_reg_cb_fn.free_phy_to_virt)
		{
			ret = _g_pe_reg_cb_fn.free_phy_to_virt();
			PE_CHECK_CODE(ret, break, "free_phy_to_virt() error.\n");
		}
	}while (0);
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
int PE_REG_WD(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(_g_pe_reg_cb_fn.wr_reg)
		{
			ret = _g_pe_reg_cb_fn.wr_reg(addr, value);
			PE_CHECK_CODE(ret, break, "wr_reg() error.\n");
		}
	}while (0);
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
static int PE_REG_WrAllocatedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(_g_pe_reg_cb_fn.wr_alloc_reg)
		{
			ret = _g_pe_reg_cb_fn.wr_alloc_reg(addr, value);
			PE_CHECK_CODE(ret, break, "wr_alloc_reg() error.\n");
		}
	}while (0);
	return ret;
}

/**
 * read address data
 *	- use register allocated instantly
 *
 * @param   addr [in] UINT32, accessing for register
 * @return  value of register
 * @see
 * @author
 */
static UINT32 PE_REG_RD(UINT32 addr)
{
	UINT32 value = 0x0;
	do{
		if(_g_pe_reg_cb_fn.rd_reg)
		{
			value = _g_pe_reg_cb_fn.rd_reg(addr);
		}
	}while (0);
	return value;
}

