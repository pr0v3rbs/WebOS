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
 *  main configuration file for clock device
 *
 *  author		gaius.lee
 *  version		1.0
 *  date		2010.03.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_clock
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
/**
 * if clock provider ( vdec or sdec ) can't support 64 bit solution, clock should do itself */
#define INTERNAL_WRAP_AROUND		0
#define MAX_GSTC_TIME				0x1FFFFFFF

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
//#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "clk_drv.h"
#include "clk_cfg.h"
#include "clk_core.h"

#include "sdec_reg.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define CLK_GSTC_SRC_0	0

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	function prototypes
 *----------------------------------------------------------------------------------------*/
extern int SDEC_HAL_GSTC(UINT8 core, UINT32 *pStcc_bs_32_1, UINT32 *pStcc_bs_0);

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Global variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	Driver functions for CLOCK IOCTL calls
 *----------------------------------------------------------------------------------------*/

/**
 * Get Internal Time. For internal use
 * Get gstc time (90 Khz clock) from kernel drv and return.
 *
 * @def CLK_IOR_GET_CURRENTGSTC
 */
static ClockTime CLK_GetGstc ( void )
{
	UINT32			gstc_sdec[2] = { 0, 0 };
	ClockTime 		gstc = 0;

	SDEC_HAL_GSTC(CLK_GSTC_SRC_0, &gstc_sdec[0], &gstc_sdec[1]);

	/* sdec은 42 bit의 gstcc를 쓰는데, 상위 33bit가 90 khz 단위의 gstc임.
	 * 그러므로 상위 33bit를 가져다가 쓰기 위해서 41 ~ 10 을 1bit shift한 후에,
	 * 9 ~ 0의 MSB 1bit을 가져다가 쓴다.
	 */
	gstc = ((ClockTime)gstc_sdec[0] << 1) | ( ((ClockTime)gstc_sdec[1] ) & 0x1);

//	CLK_KDRV_LOG( CLK_INFO, "gstc_vdec = %u gstc = %llu\n", gstc_vdec, gstc);

	return gstc;
}


/**
 * CLK_DD_Init
 * note:
 * @def CLK_IOW_INIT
 * Initialize clk module.
 * Set clock to default status.
 */

int	CLK_DD_Init ( void )
{
	return RET_OK;
}

/**
 * CLK_DD_Terminate
 * note:
 * @def CLK_IOW_TERMINATE
 * Terminate clock module.
 */

int	CLK_DD_Terminate ( void )
{
	/**/
	return RET_OK;
}

/**
 * Get Internal Time.
 * Get gstc time (90 Khz clock) from kernel drv and return.
 *
 * @def CLK_IOR_GET_CURRENTGSTC
 */
int CLK_DD_GetGstc ( UINT32 arg )
{
	int 			ret 	= RET_ERROR;
	UINT32			gstc_vdec	= 0, gstc_sdec[2] = { 0, 0 };
	ClockTime 		gstc = 0;
	LX_CLK_GET_GSTC_T clk_gstc;
	/*extern UINT32	VDEC_REG_GetGSTC(void);*/

	if (copy_from_user(&clk_gstc, (LX_CLK_GET_GSTC_T *)arg, sizeof(LX_CLK_GET_GSTC_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		ret = -EFAULT;
		goto exit;
	}

	/*gstc_vdec    = VDEC_REG_GetGSTC();	*/	// 20110403 seokjoo.lee
	SDEC_HAL_GSTC(CLK_GSTC_SRC_0, &gstc_sdec[0], &gstc_sdec[1]);

	/* sdec은 42 bit의 gstcc를 쓰는데, 상위 33bit가 90 khz 단위의 gstc임.
	 * 그러므로 상위 33bit를 가져다가 쓰기 위해서 41 ~ 10 을 1bit shift한 후에,
	 * 9 ~ 0의 MSB 1bit을 가져다가 쓴다.
	 */
	gstc = ((ClockTime)gstc_sdec[0] << 1) | ( ((ClockTime)gstc_sdec[1] ) & 0x1);

	clk_gstc.gstc_32_1 	= gstc_sdec[0];
	clk_gstc.gstc_0 	= gstc_sdec[1];

	CLK_KDRV_LOG( CLK_INFO, "gstc_vdec = %u gstc = %llu\n", gstc_vdec, gstc);

	if (copy_to_user( (LX_CLK_GET_GSTC_T *)arg, &clk_gstc, sizeof(LX_CLK_GET_GSTC_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		ret = -EFAULT;
		goto exit;
	}

exit:
	return ret;
}


#include "clk_math.c"

/**
 * CLK_DD_Open
 * note:
 * @def CLK_IOW_OPEN
 * Open and init clk object.
 * Set clock to default status.
 */

int	CLK_DD_Open ( UINT32 arg )
{
	int 			ret 	= RET_ERROR;
	ClockTime		gstc 	= 0;
	LX_CLK_OBJ_T	*clock, clockObj;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		ret = -EFAULT;
		goto exit;
	}

	/* initial seed & last internal time to current internal time */
	gstc		 			= CLK_GetGstc();
	clock->seed_itime 		= gstc;
	clock->last_itime 		= gstc;

	clock->seed_etime 		= 0;
	clock->last_etime		= 0;

	/* initial rate shall be 0. user must set rate (1,1) when want to start */
	clock->rate_numer 		= 0;
	clock->rate_denom		= 1;
	clock->time_offset		= 0;

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		ret = -EFAULT;
		goto exit;
	}

exit:
	return ret;
}


/**
 * Convert time.
 * Using gstc (90 Khz clock) and time offset and rate, calculating Time Stamp. ( 32 bit, 1 ns resolution )
 * Formula : ( gstc@Current - gstc@last ) * CONVERSION_COEF * RATE + TIMESTAMP@last + offset
 *
 * @def CLK_IOR_GET_TIMESTAMP
 */
static ClockTime CLK_DD_ConvertTime ( LX_CLK_OBJ_T *clock, ClockTime internal)
{
	ClockTime 		ret, time_offset, cinternal, lexternal, cnum, cdenom;

	CLK_CHECK_CODE( clock == NULL, return 0, "clock is null\n");
	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	cnum		= clock->rate_numer;
	cdenom		= clock->rate_denom;
	lexternal	= clock->last_etime;
	time_offset = clock->time_offset;

	cinternal	= internal- clock->last_itime;

	/* this part will be removed if vdec kernel driver support 32-bit gstc scheme. */
#ifdef INTERNAL_WRAP_AROUND
	if( internal < clock->last_itime )
	{
		CLK_KDRV_LOG( CLK_WARNING, "there is overwrapped clock!!! so calculate again!!\n");
		/* calculate difference like circular(?) ring gstc clock */
		cinternal =  MAX_GSTC_TIME - clock->last_itime;
		cinternal += internal;

		clock->last_itime = internal;
	}
#endif

	CLK_KDRV_LOG( CLK_INFO, "internal = %llu, last_itime = %llu, cinternal = %llu\n", internal, clock->last_itime, cinternal);

	/* converting domain from 90 Khz to 1 ns resolution. */
	cinternal = CLK_MATH_Uint64_Scale (cinternal, CONVERT_NUM, CONVERT_DENOM, 0);

	CLK_KDRV_LOG( CLK_INFO, "*= CLOCKTIME_COEF = %llu\n", cinternal);

	/* applying rate */
	ret = CLK_MATH_Uint64_Scale (cinternal, cnum, cdenom, 0);

	/* add last Timestamp and offset*/
	ret += lexternal;
	ret += time_offset;

	CLK_KDRV_LOG( CLK_INFO, "ret = %llu lexternal= %llu\n", ret, lexternal);

	/* make sure the time is increasing */
	clock->last_etime = MAX (ret, clock->last_etime);
	clock->last_itime = MAX (internal, clock->last_itime);

	return clock->last_etime;
}

/**
 * Get time stamp from internal time.
 * Calculating time stamp with internal time, offset and rate. result is 1ns resolution time stamp.
 *
 * @def CLK_IOR_GET_TIMESTAMP
 */

int	CLK_DD_GetTimestamp ( UINT32 arg )
{
	LX_CLK_OBJ_T	*clock, clockObj;
	ClockTime 		adjustTime, internalTime = 0;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		return -EFAULT;
	}

	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	/* [TODO] getting internal time */

	internalTime = CLK_GetGstc();

	adjustTime = CLK_DD_ConvertTime(clock, internalTime);

	/* save return value */
	clock->ret_value = adjustTime;


	CLK_KDRV_LOG( CLK_INFO, "ret_value = %llu\n", clock->ret_value);

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		return -EFAULT;
	}

	return RET_OK;
}

/**
 * Revert time.
 * Using Time Stamp and time offset and rate, calculating gstc (90 Khz clock).
 *
 * @def CLK_IOR_GET_GSTC
 */

int	CLK_DD_RevertTime ( UINT32 arg )
{
	LX_CLK_OBJ_T	*clock, clockObj;
	ClockTime 		ret, time_offset, cexternal, linternal, cnum, cdenom, external;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		return -EFAULT;
	}

	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	/* save parameter from user space */
	external    = clock->param;

	cnum		= clock->rate_numer;
	cdenom		= clock->rate_denom;
	linternal	= clock->last_itime;
	time_offset = clock->time_offset;
	cexternal	= external- clock->last_etime;

	CLK_KDRV_LOG( CLK_INFO, "external = %llu, last_itime = %llu, cexternal = %llu\n", external, clock->last_itime, cexternal);

	/* converting domain from 1 ns resolution to 90 Khz. */
	cexternal = CLK_MATH_Uint64_Scale (cexternal, CONVERT_DENOM, CONVERT_NUM, 0);

	/* applying rate */
	ret = CLK_MATH_Uint64_Scale (cexternal, cdenom, cnum, 0);

	/* add last gstc and offset*/
	ret += linternal;
	ret += time_offset;

	/* save return value */
	clock->ret_value = ret;

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		return -EFAULT;
	}

	return RET_OK;
}



/**
 * slam clock of given clock.
 * Set time stamp with given @slam_time.
 * If clock is slamed, base time is set and calculating begins.
 *
 * @def CLK_IOW_SLAMTIME
 */

int	CLK_DD_SlamTime ( UINT32 arg )
{
	LX_CLK_OBJ_T	*clock, clockObj;
	ClockTime 		slam_time;
	UINT32			internalTime = 0;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		return -EFAULT;
	}

	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	/* save parameter from user space */
	slam_time    = clock->param;

	CLK_KDRV_LOG( CLK_INFO, "slam_time = %llu\n", slam_time);

	internalTime = CLK_GetGstc();

	clock->seed_itime	= internalTime;
	clock->last_itime	= internalTime;
	clock->seed_etime	= slam_time;
	clock->last_etime	= slam_time;

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		return -EFAULT;
	}

	return RET_OK;
}



/**
 * Set clock rate of given clock.
 *
 * @def CLK_IOW_SETRATE
 */

int	CLK_DD_SetRate ( UINT32 arg )
{
	LX_CLK_OBJ_T	*clock, clockObj;
	ClockTime 		param;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		return -EFAULT;
	}

	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	/* save parameter from user space */
	param    			= clock->param;

	CLK_DD_ConvertTime(clock, CLK_GetGstc());

	clock->rate_numer 	= ( param >> 32 ) & 0xffffffff;
	clock->rate_denom	= ( param >> 00 ) & 0xffffffff;

	//CLK_DD_ConvertTime(clock, CLK_GetGstc());

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		return -EFAULT;
	}

	return RET_OK;
}



/**
 * Get clock rate of given clock.
 *
 * @def CLK_IOR_GETRATE
 */
int	CLK_DD_GetRate ( UINT32 arg )
{
	LX_CLK_OBJ_T	*clock, clockObj;
	ClockTime 		ret;

	clock = &clockObj;

	if (copy_from_user(clock, (LX_CLK_OBJ_T *)arg, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_from_user error !!! \n");
		return -EFAULT;
	}

	CLK_CHECK_CODE( clock->opened == FALSE, return 0, "clk is not opend\n");

	ret =  clock->rate_numer << 32;
	ret += clock->rate_denom;

	/* save return value */
	clock->ret_value = ret;

	if (copy_to_user( (LX_CLK_OBJ_T *)arg, clock, sizeof(LX_CLK_OBJ_T)))
	{
		CLK_KDRV_LOG( CLK_ERROR, "ioctl: copy_to_user error !!! \n");
		return -EFAULT;
	}

	return RET_OK;
}

