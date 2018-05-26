/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.

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
 *  clock gating implementation for icod device
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2013.06.27
 *  note		Additional information.
 *
 *  @addtogroup lg115x_icod
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
//#define ICOD_ENABLE_CLOCK_GATE

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

#include "base_types.h"
#include "os_util.h"
#include "mjpeg_drv.h"

#ifdef ICOD_ENABLE_CLOCK_GATE
#include "sys_reg.h"
#endif

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#define ICOD_LOCK()						spin_lock( &icod_lock )
#define ICOD_UNLOCK()					spin_unlock( &icod_lock )

#define ICOD_NOTI( format, args... )	MJPEG_NOTI( format, ##args )

#if 1
#define ICOD_DEBUG( format, args... )	MJPEG_WARN( format, ##args )
#else
#define ICOD_DEBUG( format, args... )
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef enum {
	ICOD_CLOCK_TYPE_ORIGINAL = 0,
	ICOD_CLOCK_TYPE_HALF,
	ICOD_CLOCK_TYPE_QUATER,
	ICOD_CLOCK_TYPE_DISABLE,
} ICOD_CLOCK_TYPE_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static void ICOD_ClockOff( void );

/*-----------------------------------------------------------------------------
  Static Variables
-----------------------------------------------------------------------------*/
struct timer_list icod_timer;
UINT32 	icod_clock_status;
DEFINE_SPINLOCK(icod_lock);

/*========================================================================================
  Implementation Group
========================================================================================*/

static void ICOD_TimerHandler( unsigned long arg )
{
	if ( icod_clock_status )
	{
		ICOD_ClockOff();
	}

	return;
}

static void ICOD_AddClockOffTimer( void )
{
	init_timer( &icod_timer );

	if ( icod_timer.function == NULL )
	{
		icod_timer.function = ICOD_TimerHandler;
	}

	icod_timer.expires = jiffies + (5*HZ);	// 5 seconds

	add_timer( &icod_timer );

	return;
}

#ifdef ICOD_ENABLE_CLOCK_GATE
static inline void ICOD_FlushEnable( BOOLEAN en )
{
	ICOD_DEBUG("%s :: ENTER\n", __F__);

	if(0)
	{

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	else if      ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) )
	{
		CPU_TOP_H14A0_RdFL( flush_req );
		CPU_TOP_H14A0_Wr01( flush_req, icod_flush_en, en ? 1 : 0 );
		CPU_TOP_H14A0_WrFL( flush_req );
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) )
	{
		CPU_TOP_M14B0_RdFL( flush_req );
		CPU_TOP_M14B0_Wr01( flush_req, icod_flush_en, en ? 1 : 0 );
		CPU_TOP_M14B0_WrFL( flush_req );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,A0) )
	{
		CPU_TOP_M14A0_RdFL( flush_req );
		CPU_TOP_M14A0_Wr01( flush_req, icod_flush_en, en ? 1 : 0 );
		CPU_TOP_M14A0_WrFL( flush_req );
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev() >= LX_CHIP_REV(H13,B0) )
	{
		CPU_TOP_H13B0_RdFL( flush_req );
		CPU_TOP_H13B0_Wr01( flush_req, icod_flush_en, en ? 1 : 0 );
		CPU_TOP_H13B0_WrFL( flush_req );
	}
#endif
	else
	{
		// Unkown chip revision..
	}

#if 0
	UINT32 flush_req;
	UINT32 icod_flush_done = 0;

	// 2. check flush done
	while( TRUE )
	{
		CPU_TOP_RdFL( flush_done_status );
		CPU_TOP_Rd01( flush_done_status, icod_flush_done, icod_flush_done );

		if ( icod_flush_done )
		{
			printk("Check icod_flush_done\n");
			break;
		}
	}
#endif

	ICOD_DEBUG("%s :: EXIT\n", __F__);

	return;
}

static inline void ICOD_SoftwareResetEnable( BOOLEAN en )
{
	ICOD_DEBUG("%s :: ENTER\n", __F__);

	if      ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) )
	{
		CTOP_CTRL_H14A0_RdFL( ctr04 );
		CTOP_CTRL_H14A0_Wr01( ctr04, swrst_icod, en ? 1 : 0 );
		//CTOP_CTRL_H14A0_Wr01( ctr04, swrst_icoda, en ? 1 : 0 );
		CTOP_CTRL_H14A0_WrFL( ctr04 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) )
	{
		CTOP_CTRL_M14B0_RdFL( GFX, ctr01 );
		CTOP_CTRL_M14B0_Wr01( GFX, ctr01, swrst_icod, en ? 1 : 0 );
		//CTOP_CTRL_M14B0_Wr01( GFX, ctr01, swrst_icoda, en ? 1 : 0 );
		CTOP_CTRL_M14B0_WrFL( GFX, ctr01 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,A0) )
	{
		CTOP_CTRL_M14A0_RdFL( ctr04 );
		CTOP_CTRL_M14A0_Wr01( ctr04, swrst_icod, en ? 1 : 0 );
		CTOP_CTRL_M14A0_WrFL( ctr04 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(H13,B0) )
	{
		CTOP_CTRL_H13B0_RdFL( ctr04 );
		CTOP_CTRL_H13B0_Wr01( ctr04, swrst_icod, en ? 1 : 0 );
		CTOP_CTRL_H13B0_WrFL( ctr04 );
	}
	else
	{
		// Unkown chip revision
	}

	ICOD_DEBUG("%s :: EXIT\n", __F__);

	return;
}

static inline void ICOD_ClockEnable( ICOD_CLOCK_TYPE_T eClockType )
{
	// clock_sel (0:original, 1:half, 2:quater, 4/5/6/7:disable)
	UINT8 clock_sel = (eClockType == ICOD_CLOCK_TYPE_DISABLE) ? 0x4 : 0x0;

	ICOD_DEBUG("%s :: ENTER\n", __F__);

	if      ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) )
	{
		CTOP_CTRL_H14A0_RdFL( ctr06 );
		CTOP_CTRL_H14A0_Wr01( ctr06, icodclk_sel, clock_sel );
		CTOP_CTRL_H14A0_WrFL( ctr06 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,B0) )
	{
		CTOP_CTRL_M14B0_RdFL( GFX, ctr00 );
		CTOP_CTRL_M14B0_Wr01( GFX, ctr00, icod_clk_sel, clock_sel );
		CTOP_CTRL_M14B0_WrFL( GFX, ctr00 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(M14,A0) )
	{
		CTOP_CTRL_M14A0_RdFL( ctr06 );
		CTOP_CTRL_M14A0_Wr01( ctr06, icodclk_sel, clock_sel );
		CTOP_CTRL_M14A0_WrFL( ctr06 );
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV(H13,B0) )
	{
		CTOP_CTRL_H13B0_RdFL( ctr06 );
		CTOP_CTRL_H13B0_Wr01( ctr06, icodclk_sel, clock_sel );
		CTOP_CTRL_H13B0_WrFL( ctr06 );
	}
	else
	{
		// Unkown chip revision
	}

	ICOD_DEBUG("%s :: EXIT\n", __F__);

	return;
}
#endif

static void ICOD_ClockOff( void )
{
#ifdef ICOD_ENABLE_CLOCK_GATE
	ICOD_LOCK();

	// 1. Check idle (optional)

	// 2. Enable flush
	ICOD_FlushEnable( TRUE );

	// 3. Enable software reset
	ICOD_SoftwareResetEnable( TRUE );

	// 4. Enable Clock
	ICOD_ClockEnable( ICOD_CLOCK_TYPE_DISABLE );

	icod_clock_status = 0;

	ICOD_UNLOCK();

	ICOD_NOTI("%s :: Clock gating: OFF\n", __FUNCTION__);
#else
	// NOOP
#endif
}

void ICOD_ClockOn( void )
{
#ifdef ICOD_ENABLE_CLOCK_GATE

	if ( icod_timer.function != NULL )
	{
		del_timer( &icod_timer );
	}

	if ( !icod_clock_status )
	{
		ICOD_LOCK();

		// 1. Disable flush
		ICOD_FlushEnable( FALSE );

		// 2. Disable software reset
		ICOD_SoftwareResetEnable( FALSE );

		// 3. Enable clock
		ICOD_ClockEnable( ICOD_CLOCK_TYPE_ORIGINAL );

		icod_clock_status = 1;

		ICOD_UNLOCK();

		ICOD_NOTI("%s :: Clock gating: ON\n", __FUNCTION__);
	}

	ICOD_AddClockOffTimer();
#else
	// NOOP
#endif
}

// EOF
