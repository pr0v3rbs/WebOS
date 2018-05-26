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
#include <asm/io.h>


#include "os_util.h"

#include "sdec_impl.h"
#include "sdec_hal.h"
#include "sdec_reg_ctrl.h"

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
SDTOP_REG_CTRL_T *g_sdec_sdtop_reg_ctrl;

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

int SDEC_HAL_TOPInit(void)
{
	LX_SDEC_CFG_T *cfg;
	UINT32	reg_size;

	cfg = SDEC_CFG_GetConfig();
	g_sdec_sdtop_reg_ctrl = (SDTOP_REG_CTRL_T*)OS_KMalloc(sizeof(SDTOP_REG_CTRL_T));
	SDEC_CHECK_ERROR(g_sdec_sdtop_reg_ctrl == NULL, return RET_ERROR, "kmalloc fail");

	if (lx_chip_rev() < LX_CHIP_REV(M14, B0))
		reg_size = sizeof(SDTOP_REG_H13A0_T);
	else
		reg_size = sizeof(SDTOP_REG_T);

	g_sdec_sdtop_reg_ctrl->size = reg_size;

	g_sdec_sdtop_reg_ctrl->phys.addr = (volatile UINT32*)ioremap(cfg->sdtopRegBase, reg_size);
	SDEC_CHECK_ERROR(g_sdec_sdtop_reg_ctrl->phys.addr == NULL, return RET_ERROR, "ioremap fail");

	g_sdec_sdtop_reg_ctrl->shdw.addr = (UINT32*)OS_KMalloc(reg_size);
	SDEC_CHECK_ERROR(g_sdec_sdtop_reg_ctrl->shdw.addr == NULL, return RET_ERROR, "kmalloc fail");

	return RET_OK;
}

int SDEC_HAL_SetVideoOut(UINT8 idx, UINT8 sel)
{
	int ret = RET_ERROR;

	/* idx : VDEC Number, sel : From Which? */
	/* vid0,1 -> 0: SDEC0, 1:SDEC1
	    vid2 -> 0: SDEC0 CDIC2 CHB, 1: PVR Port0, 2: PVR Port1, 3:SDEC1 CDIC2 CHB */

	SDTOP_RdFL(vid_out_sel);

	switch(idx)
	{
		if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			case 0 : SDTOP_Wr01(vid_out_sel, vid0_sel, sel); break;
			case 1 : SDTOP_Wr01(vid_out_sel, vid1_sel, sel); break;
		}
		case 2 : SDTOP_Wr01(vid_out_sel, vid2_sel, sel); break;
		default : goto exit;
	}

	SDTOP_WrFL(vid_out_sel);
	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_SetPVRSrc(UINT8 idx, UINT8 sel)
{
	int ret = RET_ERROR;

	/* idx : DL Port Number, sel : From Which? 0: SDEC0, 1: SENC, 2: SDEC1*/

	SDTOP_RdFL(dl_sel);

	switch(idx)
	{
		case 0 : SDTOP_Wr01(dl_sel, dl0_sel, sel); break;
		case 1 : SDTOP_Wr01(dl_sel, dl1_sel, sel); break;
		default : goto exit;
	}

	SDTOP_WrFL(dl_sel);
	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_SetAudioOut(UINT8 idx, UINT8 sel)
{
	int ret = RET_ERROR;

	/* idx : ADEC Number, sel : From Which? 0: SDEC0, 1: SDEC1*/

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		SDTOP_RdFL(aud_out_sel);

		switch(idx)
		{
			case 0 : SDTOP_Wr01(aud_out_sel, aud0_sel, sel); break;
			case 1 : SDTOP_Wr01(aud_out_sel, aud1_sel, sel); break;
			default : goto exit;
		}

		SDTOP_WrFL(aud_out_sel);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_SetUploadSrc(UINT8 idx, UINT8 sel)
{
	int ret = RET_ERROR;

	/* idx : Upload Port Number, sel : To where? 0: SDEC0, 1: SDEC1*/

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		SDTOP_RdFL(up_sel);

		switch(idx)
		{
			case 0 : SDTOP_Wr01(up_sel, up0_sel, sel); break;
			case 1 : SDTOP_Wr01(up_sel, up1_sel, sel); break;
			default : goto exit;
		}

		SDTOP_WrFL(up_sel);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_SetOutputSrcCore(UINT8 idx, UINT8 sel)
{
	int ret = RET_ERROR;

	/* idx : should be 0, sel : From Which CDOC? 0: SDEC0, 1: SDEC1*/

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		SDTOP_RdFL(stpo_sel);
		SDTOP_Wr01(stpo_sel, stpo_sel, sel);
		SDTOP_WrFL(stpo_sel);
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_SetTsSelector(UINT8 core, UINT8 port, UINT8 main_src, UINT8 sub_src)
{
	UINT8 idx;
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();

	if(cfg->inTsSelector == 0)
	{
		SDEC_PRINT_WARNING("No TS Selector");
		return RET_ERROR;
	}

#if 0	// Is it necessary ?
	if(core >= 2 || port >= 4 || main_src >= SDEC_HAL_TS_INPUT_NUM)
		return RET_ERROR;

	if(main_src == SDEC_HAL_TS_INPUT_EXTERNAL && sub_src >= SDEC_HAL_IN_EXT_NUM)
		return RET_ERROR;

	if(main_src == SDEC_HAL_TS_INPUT_INTERNAL && sub_src >= SDEC_HAL_IN_INT_NUM)
		return RET_ERROR;
#endif

	if(lx_chip() == LX_CHIP_H15)
	{
		if(port <= 2) idx = core*2 + port;
		else idx = 4 + core*2 + (port - 3);
	}
	else
	{
		idx = core*4 + port;
	}

	SDTOP_RdFL(ts_src_sel[idx]);
	SDTOP_Wr01(ts_src_sel[idx], src_sel, main_src);
	if(main_src == SDEC_HAL_TS_INPUT_EXTERNAL)
		SDTOP_Wr01(ts_src_sel[idx], demod_sel, sub_src);
	else
		SDTOP_Wr01(ts_src_sel[idx], ci_sel, sub_src);
	SDTOP_WrFL(ts_src_sel[idx]);

	return RET_OK;
}

int SDEC_HAL_GetTsSelector(UINT8 core, UINT8 port, UINT8 *main_src, UINT8 *sub_src)
{
	UINT8 idx;
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();

	if(cfg->inTsSelector == 0)
		return RET_ERROR;

	if(lx_chip() == LX_CHIP_H15)
	{
		if(port <= 2) idx = core*2 + port;
		else idx = 4 + core*2 + (port - 3);
	}
	else
	{
		idx = core*4 + port;
	}

	SDTOP_RdFL(ts_src_sel[idx]);
	SDTOP_Rd01(ts_src_sel[idx], src_sel, *main_src);
	if(*main_src == SDEC_HAL_TS_INPUT_EXTERNAL)
		SDTOP_Rd01(ts_src_sel[idx], demod_sel, *sub_src);
	else
		SDTOP_Rd01(ts_src_sel[idx], ci_sel, *sub_src);

	return RET_OK;
}

