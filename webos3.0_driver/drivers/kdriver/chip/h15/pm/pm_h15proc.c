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
 *  main driver implementation for pm device.
 *	pm device will teach you how to make device driver with new platform.
 *
 *  author		hankyung.yu (hankyung.yu@lge.com)
 *  version		1.0
 *  date			2014.07.18
 *  note			Additional information.
 *
 *  @addtogroup lg1210_pm
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include "os_util.h"
#include "base_device.h"
#include "pm_drv.h"
#include "proc_util.h"
#include "debug_util.h"

#include "pm_h15common.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_AFFECTED_CPUS,		// 1
	PROC_ID_SET_FREQ,			// 2
	PROC_ID_CUR_FREQ,			// 3
	PROC_ID_MAX_FREQ,			// 4
	PROC_ID_MIN_FREQ,			// 5
	PROC_ID_CUR_VOL,			// 6
	PROC_ID_TRANSITION_LATENCY,	// 7
	PROC_ID_AVAILABLE_FREQ,		// 8
	PROC_ID_HARD_UP_THRESHOLD,	// 9
	PROC_ID_EASY_UP_THRESHOLD,	// 10
	PROC_ID_EASY_COUNT,			// 11
	PROC_ID_SAMPLING_RATE,		// 12
	PROC_ID_SAMPLING_RATE_MIN,	// 13
	PROC_ID_CURRENT_CPUS,		// 14
	PROC_ID_GOVERNOR,			// 15
	PROC_ID_OD_FREQDOWN_COUNT,	// 16
	PROC_ID_ND_FREQDOWN_COUNT,	// 17
	PROC_ID_MPD_UD_COUNT,		// 18
	PROC_ID_MPD_ND_COUNT,		// 19
	PROC_ID_MPD_OD_COUNT,		// 20
	PROC_ID_RESET_LOADMON,		// 21
	PROC_ID_GET_CURRENT_LOAD,	// 22
	PROC_ID_TO_SUSPEND,			// 23
	PROC_ID_MP_ENABLE,			// 24
	PROC_ID_MAX,
};

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
static pms_h15gov_t* pm_h15gov = NULL;
static pms_h15func_t* pm_h15func = NULL;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_pm_h15_device_proc_table[] =
{
	{ "author",					PROC_ID_AUTHOR  			, OS_PROC_FLAG_READ },
	{ "affected_cpus",			PROC_ID_AFFECTED_CPUS		, OS_PROC_FLAG_READ },
	{ "set_frequency",			PROC_ID_SET_FREQ			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "current_frequency",		PROC_ID_CUR_FREQ			, OS_PROC_FLAG_READ },
	{ "max_frequency",			PROC_ID_MAX_FREQ			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "min_frequency",			PROC_ID_MIN_FREQ			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "current_voltage",		PROC_ID_CUR_VOL				, OS_PROC_FLAG_READ },
	{ "transition_latency",		PROC_ID_TRANSITION_LATENCY  , OS_PROC_FLAG_READ },
	{ "available_frequency",	PROC_ID_AVAILABLE_FREQ		, OS_PROC_FLAG_READ },
	{ "up_threshold_hard",		PROC_ID_HARD_UP_THRESHOLD	, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "up_threshold_easy",		PROC_ID_EASY_UP_THRESHOLD	, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "count_easy",				PROC_ID_EASY_COUNT			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "sampling_rate",			PROC_ID_SAMPLING_RATE		, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "sampling_rate_min",		PROC_ID_SAMPLING_RATE_MIN	, OS_PROC_FLAG_READ },
	{ "online_cpus",			PROC_ID_CURRENT_CPUS		, OS_PROC_FLAG_READ },
	{ "governor",				PROC_ID_GOVERNOR			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "freqdown_od_count",		PROC_ID_OD_FREQDOWN_COUNT	, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "freqdown_nd_count",		PROC_ID_ND_FREQDOWN_COUNT	, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "mp_enable",				PROC_ID_MP_ENABLE			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "mpd_ud_count",			PROC_ID_MPD_UD_COUNT		, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "mpd_nd_count",			PROC_ID_MPD_ND_COUNT		, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "mpd_od_count",			PROC_ID_MPD_OD_COUNT		, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "reset_loadmon",			PROC_ID_RESET_LOADMON		, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ "current_load",			PROC_ID_GET_CURRENT_LOAD	, OS_PROC_FLAG_READ},
	{ "to_suspend",				PROC_ID_TO_SUSPEND			, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE  },
	{ NULL, 					PROC_ID_MAX					, 0 }
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * read_proc implementation of pm device
 *
*/
static int	h15_read_proc(	UINT32 procId, char* buffer )
{
	unsigned int cpus[4];
	unsigned int i;
	unsigned int temp;
	int	ret = 0 ;

	/* TODO: add your proc_write implementation */
	if ( lx_chip_rev() >= LX_CHIP_REV(H15,A0) )
	{
		switch( procId )
		{
			case PROC_ID_AUTHOR:
				ret = sprintf( buffer, "%s\n", "hankyung.yu, (hankyung.yu@lge.com)\njun.kong, (jun.kong@lge.com)" );
				break;
			case PROC_ID_AFFECTED_CPUS:
				ret = sprintf( buffer, "%s\n", "cpu0 cpu1 cpu2 cpu3" );
				break;
			case PROC_ID_SET_FREQ:
				ret = sprintf( buffer, "%u\n",pm_h15func->getfreq() * 1000);
				break;
			case PROC_ID_CUR_FREQ:
				ret = sprintf( buffer, "%u\n",pm_h15func->getfreq() * 1000);
				break;
			case PROC_ID_MAX_FREQ:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_max_freq() * 1000);
				break;
			case PROC_ID_MIN_FREQ:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_min_freq() * 1000);
				break;
			case PROC_ID_CUR_VOL:
				ret = sprintf( buffer, "%u\n",pm_h15func->getvol() * 1000);
				break;
			case PROC_ID_TRANSITION_LATENCY:
				ret = sprintf( buffer, "%s\n", "100000" );
				break;
			case PROC_ID_AVAILABLE_FREQ:
				ret = sprintf( buffer, "%s\n", "624000, 1080000, 1404000" );
				break;
			case PROC_ID_HARD_UP_THRESHOLD:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_hard_up_threshold() );
				break;
			case PROC_ID_EASY_UP_THRESHOLD:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_easy_up_threshold() );
				break;
			case PROC_ID_EASY_COUNT:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_easy_count());
				break;
			case PROC_ID_SAMPLING_RATE:
				ret = sprintf( buffer, "%u\n", pm_h15func->get_sampling() * 1000 * (1000 / HZ));
				break;
			case PROC_ID_SAMPLING_RATE_MIN:
				ret = sprintf( buffer, "%s\n", "10000" );
				break;
			case PROC_ID_CURRENT_CPUS:
				temp = pm_h15func->get_cpu_onoff();
				for (i=0; i<4; i++)
					cpus[i] = (temp & (1 << i)) >> i;
				ret = sprintf( buffer, "%u, %u, %u, %u\n",cpus[0],cpus[1],cpus[2],cpus[3]);
				break;
			case PROC_ID_GOVERNOR:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_gov());
				break;
			case PROC_ID_OD_FREQDOWN_COUNT:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_od_freqdown_count());
				break;
			case PROC_ID_ND_FREQDOWN_COUNT:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_nd_freqdown_count());
				break;
			case PROC_ID_MPD_UD_COUNT:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_mpd_count() & 0xFF);
				break;
			case PROC_ID_MPD_ND_COUNT:
				ret = sprintf( buffer, "%u\n", (pm_h15gov->get_mpd_count() >>  8) & 0xFF);
				break;
			case PROC_ID_MPD_OD_COUNT:
				ret = sprintf( buffer, "%u\n", (pm_h15gov->get_mpd_count() >>  16) & 0xFF);
				break;
			case PROC_ID_TO_SUSPEND:
			case PROC_ID_RESET_LOADMON:
				break;
			case PROC_ID_GET_CURRENT_LOAD:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_load(4));
				break;
			case PROC_ID_MP_ENABLE:
				ret = sprintf( buffer, "%u\n", pm_h15gov->get_mp_enable());
				break;
			default:
				ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
				break;
		}
	}
	return ret;
}

/**
 * write_proc implementation of pm device
 *
*/
static int h15_write_proc( UINT32 procId, char* command )
{
	unsigned int input_value;
	unsigned int input_n;
	unsigned int temp;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_HARD_UP_THRESHOLD:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15gov->set_hard_up_threshold(input_value);
			break;
		case PROC_ID_EASY_UP_THRESHOLD:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15gov->set_easy_up_threshold(input_value);
			break;
		case PROC_ID_EASY_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15gov->set_easy_count(input_value);
			break;
		case PROC_ID_SAMPLING_RATE:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15func->set_sampling(input_value/1000);
			break;
		case PROC_ID_TO_SUSPEND:
			input_n = sscanf(command,"%u\n", &input_value);
			if (input_value == 1)
			{
				temp = pm_h15gov->get_gov() | PMS_GOV_TOSUSPEND_FLAG;
				pm_h15gov->set_gov(temp);
				pm_h15gov->set_startup_timer(0);
				pm_h15func->stop_timer();
				pm_h15func->set_max_performance();
			}
			break;
		case PROC_ID_GOVERNOR:
			input_n = sscanf(command,"%u\n", &input_value);
			if (input_value <= 3)
			{
				pm_h15gov->set_gov(input_value);
				pm_h15gov->set_startup_timer(0);
				if (input_value)
					pm_h15func->start_timer();
				else
				{
					pm_h15func->stop_timer();
					pm_h15func->set_max_performance();
				}
			}
			break;
		case PROC_ID_MAX_FREQ:
			input_n = sscanf(command,"%u\n", &input_value);
			if( input_value < pm_h15gov->get_min_freq())
			{
				printk("input frequency is lower then min frequency\n");
				break;
			}
			pm_h15gov->set_max_freq(input_value / 1000);
			break;
		case PROC_ID_MIN_FREQ:
			input_n = sscanf(command,"%u\n", &input_value);
			if( input_value < pm_h15gov->get_max_freq())
			{
				printk("input frequency is higher then max frequency\n");
				break;
			}
			pm_h15gov->set_min_freq(input_value / 1000);
			break;
		case PROC_ID_SET_FREQ:
			input_n = sscanf(command,"%u\n", &input_value);
			if (input_value > (pm_h15func->getfreq() * 1000))
			{
				pm_h15func->frequp(input_value/1000);
				pm_h15func->run();
			}
			else if (input_value < (pm_h15func->getfreq() * 1000))
			{
				pm_h15func->freqdown(input_value/1000);
				pm_h15func->run();
			}
			break;
		case PROC_ID_OD_FREQDOWN_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15gov->set_od_freqdown_count(input_value);
			break;
		case PROC_ID_ND_FREQDOWN_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			pm_h15gov->set_nd_freqdown_count(input_value);
			break;
		case PROC_ID_MPD_UD_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			temp = pm_h15gov->get_mpd_count() & 0xFFFFFF00;
			if (input_value > 255)
				input_value = 255;
			if (input_value == 0)
				input_value = 1;
			temp = temp | input_value;
			pm_h15gov->set_mpd_count(temp);
			break;
		case PROC_ID_MPD_ND_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			temp = pm_h15gov->get_mpd_count() & 0xFFFF00FF;
			if (input_value > 255)
				input_value = 255;
			if (input_value == 0)
				input_value = 1;
			temp = temp | (input_value << 8);
			pm_h15gov->set_mpd_count(temp);
			break;
		case PROC_ID_MPD_OD_COUNT:
			input_n = sscanf(command,"%u\n", &input_value);
			temp = pm_h15gov->get_mpd_count() & 0xFF00FFFF;
			if (input_value > 255)
				input_value = 255;
			if (input_value == 0)
				input_value = 1;
			temp = temp | (input_value << 16);
			pm_h15gov->set_mpd_count(temp);
			break;
		case PROC_ID_RESET_LOADMON:
			pm_h15gov->reset_load_monitor();
			break;
		case PROC_ID_MP_ENABLE:
			input_n = sscanf(command,"%u\n", &input_value);
			if (input_value != 0)
				input_value = 1;
			pm_h15gov->set_mp_enable(input_value);
		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for pm device
 *
 * @see PM_Init
*/
void h15_proc_init (void)
{
	pm_h15gov  = get_h15pm_gov();
	pm_h15func  = get_h15pm_func();

	OS_PROC_CreateEntryEx ( PM_MODULE, _g_pm_h15_device_proc_table,
										h15_read_proc,
										h15_write_proc );
}

/**
 * cleanup proc utility for pm device
 *
 * @see PM_Cleanup
*/
void h15_proc_cleanup (void)
{
	OS_PROC_RemoveEntry( PM_MODULE );
}

static pms_proc_t h15_proc_func =
{
	.init = h15_proc_init,
	.cleanup = h15_proc_cleanup,
};

pms_proc_t* get_h15pm_proc(void)
{
	return &h15_proc_func;
}
/** @} */

