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


#ifndef	_PM_H15COMMON_H_
#define	_PM_H15COMMON_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "debug_util.h"
#include "pm_cfg.h"
#include "pm_kapi.h"
#include "pm_drv.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PMS_GOV_MASK			0xF
#define PMS_GOV_TOSUSPEND_FLAG	0X10

#define PMS_GOV_DISABLE			0x0
#define PMS_GOV_ENABLE			0x1
#define PMS_GOV_MONITOR			0x2
#define PMS_GOV_EN_N_MON		0x3


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	unsigned int	(*getfreq)		(void);
	unsigned int	(*getvol)		(void);
	int				(*frequp)		(unsigned int freq);
	int				(*freqdown)		(unsigned int freq);
	void			(*set_cpu_onoff)(unsigned int core_n, unsigned int onoff);
	unsigned int	(*get_cpu_onoff)(void);
	void			(*run) 			(void);
	unsigned int	(*get_sampling)	(void);
	int				(*set_sampling)	(unsigned int ms);
	void			(*start_timer)	(void);
	void			(*stop_timer)	(void);
	void			(*set_max_performance)	(void);
} pms_h15func_t;

typedef struct
{
	void  			(*init)(void);
	void  			(*check_cpu)(void);
	unsigned int	(*get_gov)(void);
	int				(*set_gov)(unsigned int status);
	unsigned int	(*get_hard_up_threshold)(void);
	int				(*set_hard_up_threshold)(unsigned int threshold);
	unsigned int	(*get_easy_up_threshold)(void);
	int				(*set_easy_up_threshold)(unsigned int threshold);
	unsigned int	(*get_easy_count)(void);
	int				(*set_easy_count)(unsigned int count);
	unsigned int 	(*get_startup_timer)(void);
	int 			(*set_startup_timer)(unsigned int startup_time);
	void 			(*reset_load_monitor)(void);
	unsigned int 	(*get_od_freqdown_count)(void);
	int 			(*set_od_freqdown_count)(unsigned int value);
	unsigned int 	(*get_nd_freqdown_count)(void);
	int 			(*set_nd_freqdown_count)(unsigned int value);
	unsigned int 	(*get_mpd_count)(void);
	int 			(*set_mpd_count)(unsigned int value);
	unsigned int 	(*get_min_freq)(void);
	int 			(*set_min_freq)(unsigned int value);
	unsigned int 	(*get_max_freq)(void);
	int				(*set_max_freq)(unsigned int value);
	unsigned int 	(*get_mp_enable)(void);
	int				(*set_mp_enable)(unsigned int value);
	unsigned int	(*get_load)(unsigned int cpu);
} pms_h15gov_t;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern pms_driver_t* get_h15pm_driver(void);
extern pms_proc_t* get_h15pm_proc(void);
extern pms_h15func_t* get_h15pm_func(void);
extern pms_h15gov_t* get_h15pm_gov(void);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PM_DRV_H_ */

/** @} */
