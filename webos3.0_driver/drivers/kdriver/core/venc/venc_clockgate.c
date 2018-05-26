/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 2013 by LG Electronics Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

/** @file
 *
 *  clock gating implementation for venc device
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2013.06.27
 *  note		Additional information.
 *
 *  @addtogroup lg115x_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#define VENC_ENABLE_CLOCK_GATE

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>

#include "os_util.h"
#include "base_types.h"
#include "venc_drv.h"

#ifdef VENC_ENABLE_CLOCK_GATE
#include "sys_regs.h"

#ifdef INCLUDE_KDRV_PM
#include "pm_kapi.h"
#endif
#endif

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/
#ifdef INCLUDE_KDRV_PM
extern int PM_SetClockGating(u32 Module, u32 SubModule, u32 state);
#endif

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  Static Variables
-----------------------------------------------------------------------------*/

/*========================================================================================
  Implementation Group
========================================================================================*/

#ifdef VENC_ENABLE_CLOCK_GATE
static inline void VENC_FlushEnable( BOOLEAN en )
{
	if (0)
	{

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	// H14
	else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		CPU_TOP_H14A0_RdFL( flush_req );
		CPU_TOP_H14A0_Wr01( flush_req, venc_flush_en, en ? 1 : 0 );
		CPU_TOP_H14A0_WrFL( flush_req );
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	// M14
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		CPU_TOP_M14A0_RdFL( flush_req );
		CPU_TOP_M14A0_Wr01( flush_req, venc_flush_en, en ? 1 : 0 );
		CPU_TOP_M14A0_WrFL( flush_req );
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	// H13
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		CPU_TOP_H13B0_RdFL( flush_req );
		CPU_TOP_H13B0_Wr01( flush_req, venc_flush_en, en ? 1 : 0 );
		CPU_TOP_H13B0_WrFL( flush_req );
	}
#endif
	else
	{
		VENC_ERROR("Not supported chip 0x%X\n", lx_chip_rev());
	}

#if 0
	UINT32 flush_req;
	UINT32 venc_flush_done = 0;

	// 2. check flush done
	while( TRUE )
	{
		CPU_TOP_RdFL( flush_done_status );
		CPU_TOP_Rd01( flush_done_status, venc_flush_done, venc_flush_done );

		if ( venc_flush_done )
		{
			printk("Check venc_flush_done\n");
			break;
		}
	}
#endif

	return;
}

static inline void VENC_SoftwareResetEnable( BOOLEAN en )
{
	if (0)
	{

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	// H14
	else if      ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		CTOP_CTRL_H14A0_RdFL( ctr04 );
		CTOP_CTRL_H14A0_Wr01( ctr04, swrst_ve, en ? 1 : 0 );		// CTR04[31]
		CTOP_CTRL_H14A0_Wr01( ctr04, swrst_te_ve, en ? 1 : 0 );	// CTR04[29]
		CTOP_CTRL_H14A0_Wr01( ctr04, reg_swrst_se, en ? 1 : 0 );	// CTR04[15]
		CTOP_CTRL_H14A0_WrFL( ctr04 );

	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	// M14
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) )
		{
			CTOP_CTRL_M14B0_RdFL( VENC, ctr01 );
			CTOP_CTRL_M14B0_Wr01( VENC, ctr01, swrst_ve, en ? 1 : 0 );	// CTR04[31]
			CTOP_CTRL_M14B0_Wr01( VENC, ctr01, swrst_te, en ? 1 : 0 );	// CTR04[29]
			CTOP_CTRL_M14B0_Wr01( VENC, ctr01, swrst_se, en ? 1 : 0 );	// CTR04[15]
			CTOP_CTRL_M14B0_WrFL( VENC, ctr01 );
		}
		else //if ( lx_chip_rev() >= LX_CHIP_REV(M14,A0) )
		{
			CTOP_CTRL_M14A0_RdFL( ctr04 );
			CTOP_CTRL_M14A0_Wr01( ctr04, swrst_ve, en ? 1 : 0 );		// CTR04[31]
			CTOP_CTRL_M14A0_Wr01( ctr04, swrst_te_ve, en ? 1 : 0 );	// CTR04[29]
			CTOP_CTRL_M14A0_Wr01( ctr04, reg_swrst_se, en ? 1 : 0 );	// CTR04[15]
			CTOP_CTRL_M14A0_WrFL( ctr04 );
		}
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	// H13
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		CTOP_CTRL_H13B0_RdFL( ctr04 );
		CTOP_CTRL_H13B0_Wr01( ctr04, swrst_ve, en ? 1 : 0 );		// CTR04[31]
		CTOP_CTRL_H13B0_Wr01( ctr04, swrst_te_ve, en ? 1 : 0 );	// CTR04[29]
		CTOP_CTRL_H13B0_Wr01( ctr04, reg_swrst_se, en ? 1 : 0 );	// CTR04[15]
		CTOP_CTRL_H13B0_WrFL( ctr04 );
	}
#endif
	// Not supported chip.
	else
	{
		VENC_ERROR("Not supported chip 0x%X\n", lx_chip_rev());
	}

	return;
}

static inline void VENC_ClockEnable( BOOLEAN en )
{
	UINT8 clock_sel = ( en ) ? 0x0: 0x3 ;

	if (0)
	{

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	// H14
	else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		CTOP_CTRL_H14A0_RdFL( ctr06 );
		CTOP_CTRL_H14A0_Wr01( ctr06, veclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
		CTOP_CTRL_H14A0_Wr01( ctr06, te_teclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
		CTOP_CTRL_H14A0_WrFL( ctr06 );
	}
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
	// M14
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) )
		{
			CTOP_CTRL_M14B0_RdFL( VENC, ctr00 );
			CTOP_CTRL_M14B0_Wr01( VENC, ctr00, veclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
			CTOP_CTRL_M14B0_Wr01( VENC, ctr00, teclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
			CTOP_CTRL_M14B0_WrFL( VENC, ctr00 );
		}
		else //if ( lx_chip_rev() >= LX_CHIP_REV(M14,A0) )
		{
			CTOP_CTRL_M14A0_RdFL( ctr06 );
			CTOP_CTRL_M14A0_Wr01( ctr06, veclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
			CTOP_CTRL_M14A0_Wr01( ctr06, te_teclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
			CTOP_CTRL_M14A0_WrFL( ctr06 );
		}
	}
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
	// H13
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		CTOP_CTRL_H13B0_RdFL( ctr06 );
		CTOP_CTRL_H13B0_Wr01( ctr06, veclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
		CTOP_CTRL_H13B0_Wr01( ctr06, te_teclk_sel, clock_sel );	// 0:original, 1:half, 2:quater, 4/5/6/7:disable
		CTOP_CTRL_H13B0_WrFL( ctr06 );
	}
#endif
	// Not supported chip.
	else
	{
		VENC_ERROR("Not supported chip 0x%X\n", lx_chip_rev());
	}

	return;
}
#endif

void ClockGateOff(int id)
{
	if ( id > gpstVencConfig->num_device )
	{
		return;
	}

#ifdef VENC_ENABLE_CLOCK_GATE
	if ( !(gpstVencConfig->cfg_flag & LX_VENC_CFG_FLAG_PM) )
	{
		return;
	}

	if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) )
	{
#ifdef INCLUDE_KDRV_PM
		if ( id == 0 )
		{
			PM_SetClockGating(PM_VENC, PM_VENC_0, PM_CLK_OFF);
		}
		else if ( id == 1)
		{
			PM_SetClockGating(PM_VENC, PM_VENC_1, PM_CLK_OFF);
		}
		else
		{
			VENC_WARN("Unkown channel id\n");
		}
#else
		VENC_ERROR("Not support PM kdriver\n");
#endif
	}
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ||
			  !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ||
			  !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		VENC_FlushEnable( TRUE );
		VENC_SoftwareResetEnable( TRUE );
		VENC_ClockEnable( FALSE );
	}
	else
	{
		VENC_ERROR("Not supported chip 0x%X\n", lx_chip_rev());
	}

	VENC_NOTI("Clock gating: OFF\n");
#endif

	return;
}

void ClockGateOn(int id)
{
	if ( id > gpstVencConfig->num_device )
	{
		return;
	}

#ifdef	VENC_ENABLE_CLOCK_GATE
	if ( !(gpstVencConfig->cfg_flag & LX_VENC_CFG_FLAG_PM) )
	{
		return;
	}

	if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) )
	{
#ifdef INCLUDE_KDRV_PM
		if ( id == 0 )
		{
			PM_SetClockGating(PM_VENC, PM_VENC_0, PM_CLK_ON);
		}
		else if ( id == 1)
		{
			PM_SetClockGating(PM_VENC, PM_VENC_1, PM_CLK_ON);
		}
		else
		{
			VENC_WARN("Unkown channel id\n");
		}
#else
		VENC_ERROR("Not support PM kdriver\n");
#endif
	}
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ||
			  !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ||
			  !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		VENC_FlushEnable( FALSE );
		VENC_SoftwareResetEnable( FALSE );
		VENC_ClockEnable( TRUE );
	}
	else
	{
		VENC_ERROR("Not supported chip 0x%X\n", lx_chip_rev());
	}

	VENC_NOTI("Clock gating: ON\n");
#endif

	return;
}

