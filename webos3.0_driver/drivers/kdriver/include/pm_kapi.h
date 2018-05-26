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


#ifndef	_PM_KAPI_H_
#define	_PM_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PM_IOC_MAGIC               'g'

#define PMINFO_HEADER						0x504d4657
#define PMINFO_VERSION_1					1

#define PMINFO_SIZE							0x4000
#define PMINFO_OFFSET						(0x40000 - PMINFO_SIZE)

#define PM_CM3_RECOVERY
#define PMS_PARTITION_FW_SIZE	0x10000		// 64KB
#define PMS_HEADER_SIZE			0x200
#define PMS_FW_SHADOW_MAGIC_1 	0x706d6677
#define PMS_FW_SHADOW_MAGIC_2 	0x73686472

#define PMS_CMD_CPU_FREQ					1
#define PMS_CMD_CPU_VOL						2
#define PMS_CMD_GPU_FREQ					4
#define PMS_CMD_GPU_VOL						5
#define PMS_CMD_CHECK_INIT						6

#define PMS_OFF								0
#define PMS_ON								1

#define	TEST_FULL							0x0
#define	TEST_DVFS							0x1
#define	TEST_HOTPLUG						0x2
#define	TEST_FIXED							0x3

#define	PM_THERMAL_LOG_OFF			0x0
#define	PM_THERMAL_LOG_ON			0x1
#define	IPC_PACKET_BASE (0xF7083E00)  /* Limit ~0x2000_3FFF 256 bytes CA15:0xF7083E00~ */
#define	IPC_PACKET_SIZE (4*8)       /* 4Byte unit */
#define 	IPC_STATUS_BASE         ((IPC_PACKET_BASE) + (IPC_PACKET_SIZE)*(7) )    /* 0x20003EE0 */
#define	IPC_TS_BASE            ((IPC_STATUS_BASE) + (0x1C))    /* offset: 0x1c */
#define	IPC_TS_VAL(NUM)        (*( int *) (IPC_TS_BASE + 4*NUM)) /* NUM: 0~3  range : -40 ~ +125 Celcius */
#define	IPC_TS_ADDR(NUM)        ( (IPC_TS_BASE) + 4*(NUM)) /* NUM: 0~3  range : -40 ~ +125 Celcius */
#define	TS_CELCIUS_MAX		(125)
#define	TS_CELCIUS_MIN		(-40)
#define	TS_NUM	3
#define	TS_HISTORY_NUM		10	/* should be even number */

#define	STATUS_TEMPERATURE_STABLE			0
#define	STATUS_TEMPERATURE_INCREASING		1
#define	STATUS_TEMPERATURE_DECREASING		2
#define	STATUS_TEMPERATURE_LOW_WARNING		3
#define	STATUS_TEMPERATURE_HIGH_WARNING		4
#define	STATUS_TEMPERATURE_LOW_CRITICAL		5
#define	STATUS_TEMPERATURE_HIGH_CRITICAL		6

#define	TEMPERATURE_LOW_WARNING		(0)
#define	TEMPERATURE_HIGH_WARNING		(100)
#define	TEMPERATURE_LOW_CRITICAL		(-10)
#define	TEMPERATURE_HIGH_CRITICAL		(120)




/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	unsigned int	module;
	unsigned int 	sub;
	unsigned int 	state;
}LX_PM_CLOCKGATING_T;

typedef struct
{
	unsigned int	cpu_freq;
	unsigned int 	cpu_vol;
	unsigned int 	cpu_on;
	unsigned int 	gpu_freq;
	unsigned int 	gpu_vol;
	unsigned int 	gpu_on;
	unsigned int 	sel;	// 0: no change,  1:cpu freq change, 2:cpu vol change, 4:gpu freq change, 5:gpu vol change,
	unsigned int 	thernal_0;
	unsigned int 	thernal_1;
	unsigned int 	thernal_2;
	unsigned int 	pm_inited;
}LX_PM_STATUS_T;

typedef struct
{
	unsigned int	interval; // autodown interval
	unsigned int 	testmode; // 0 : notest, 1: cpu, 2:gpu
}LX_PM_TEST_FACTOR_T;


typedef struct
{
	unsigned char *	src;
	unsigned int	fwsize;
}LX_PM_CM3_INFO_T;

#define PM_CLK_ON	0
#define PM_CLK_OFF	1

#define PM_NULL		0
#define PM_SUB_ALL	0xFFFFFFFF
#define PM_SUB_MAX	5

typedef  enum
{
	PM_VENC = 1,
 	PM_VDEC = 2,
	PM_VIP =3,
	PM_MAX = 3,
} CLOCK_GATING_MODULE;

typedef  enum
{
	PM_VENC_0 = 0x1,
	PM_VENC_1 = 0x2,
	PM_VENC_MAX = 2,
} CLOCK_GATING_VENC_SUB;

typedef  enum
{
	PM_VDEC0_G2 = 0x1,
	PM_VDEC1_HEVC = 0x2,
	PM_VDEC1_G1 = 0x4,
	PM_VDEC2_HEVC = 0x8,
	PM_VDEC2_G1 = 0x10,
	PM_VDEC_MAX = 5,
} CLOCK_GATING_VDEC_SUB;

typedef  enum
{
	PM_VIP_0 = 0x1,
	PM_VIP_MAX = 1
} CLOCK_GATING_VIP_SUB;

#define	PM_DEV_MAJOR     	177

#define	PM_CPU_GOV_WORK			1
#define	PM_GET_CPU_GOV_STATUS	2
#define	PM_GET_STATUS			3
#define	PM_SET_STATUS			4
#define	PM_GET_CLOCK_GATING		5
#define	PM_SET_CLOCK_GATING		6
#define	PM_GET_TEST_FACTOR		7
#define	PM_SET_TEST_FACTOR		8
#define	PM_GO_TEST				9
#define	PM_STOP_TEST			10
#define	PM_UART_TO_CM3			11
#define	PM_GET_DEBUG_PRINT		12
#define	PM_SET_DEBUG_PRINT		13
#define	PM_GET_SAMPLING_RATE	14
#define	PM_SET_SAMPLING_RATE	15
#define	PM_RECOVERY_CM3			16
#define	PM_SET_CM3_STOP			17
#define	PM_SET_CM3_START		18
#define	PM_STORE_CM3_FW			19
#define	PM_RECOVERY_CM3_TEST	20



#define	CPU_VOL_DOWN		1
#define	GPU_VOL_DOWN		2
#define	CPU_FREQ_UP			3
#define	GPU_FREQ_UP			4
#define	CPU_FULL_RANDOM		5
#define	CPU_HOTPLUG_STRESS	6
#define	CPU_DVFS_RANDOM		7
#define	CPU_FREQ_RANDOM		8
#define	CPU_VOL_RANDOM		9
#define	GPU_DVFS_RANDOM		10
#define	GPU_FREQ_RANDOM		11
#define	GPU_VOL_RANDOM		12

#define CPU_FREQ_OD			1404
#define CPU_FREQ_ND			1080
#define CPU_FREQ_UD			624
#define GPU_FREQ_OD			672
#define GPU_FREQ_ND			552

#define CPU_VOL_OD			1050
#define CPU_VOL_ND			940
#define CPU_VOL_UD			800
#define GPU_VOL_OD			1000
#define GPU_VOL_ND			940
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PM_DRV_H_ */

/** @} */
