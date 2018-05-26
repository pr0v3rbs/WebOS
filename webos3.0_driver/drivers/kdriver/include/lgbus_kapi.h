/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  application interface header for lgbus device
 *
 *  author		hwanwook.lee (hwanwook.lee@lge.com)
 *              bongrae.cho (bongrae.cho@lge.com)
 *              ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2012.07.14
 *
 *  @addtogroup lg115x_lgbus
 */

#ifndef	_LGBUS_KAPI_H_
#define	_LGBUS_KAPI_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	LGBUS_IOC_MAGIC		'L'


#define LGBUS_MAX_MNT_BLK		32
#define LGBUS_MAX_DDR_PORT		4

#define LGBUS_MAX_BLKNAME		10

#define LGBUS_MAX_BLK_REG		32
#define LGBUS_MAX_BLK_REGNAME	20

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*
; LG BUS monitoring structure.
*/

typedef enum
{
	LGBUS_TYPE_L	= 0,
	LGBUS_TYPE_LBUS = LGBUS_TYPE_L,
	LGBUS_TYPE_G,
	LGBUS_TYPE_GBUS = LGBUS_TYPE_G,
	LGBUS_TYPE_E,
	LGBUS_TYPE_EBUS = LGBUS_TYPE_E,
	LGBUS_TYPE_MAX
} LX_LGBUS_BUS_TYPE_T;

typedef enum
{
	LGBUS_WRITE = 0,
	LGBUS_READ,
	LGBUS_RW_MAX
} LX_LGBUS_RW_T;

typedef enum
{
	LGBUS_MON_MODE_MANUAL = 0,	/* Starting B/W monitoring when read function is called */
	LGBUS_MON_MODE_AUTO,
} LX_LGBUS_MON_MODE_T;

typedef struct
{
	LX_LGBUS_MON_MODE_T	mode;
	UINT32	blkmask;	/* Block mask to be monitored */
	UINT32 	freq;		/* Monitoring frequency */
} LX_LGBUS_MON_CFG_T;

typedef struct
{
	UINT32	blk_index;
	UINT32	reg_type;
	UINT32	reg_value;
} LX_LGBUS_MON_REG_T;

/* chip dependant information */
typedef struct
{
	UINT64	max_bw;
	UINT32	blk_count;
	char	name[LGBUS_MAX_MNT_BLK][LGBUS_MAX_BLKNAME];
	UINT32	clock[LGBUS_MAX_MNT_BLK];
	char	reg_name[LGBUS_MAX_BLK_REG][LGBUS_MAX_BLK_REGNAME];
	UINT32	reg_count;
} LX_LGBUS_MON_INFO_T;

/* Monitroing Data Value */

/* Raw data getting from register */
typedef struct
{
	UINT8	bus;
	UINT8	port;
	UINT8	port_other;
	UINT8	reserved;

	struct
	{
		UINT32	bw;
//		UINT32	bw_bud;
		UINT32	req_val;
		UINT32	max_ltn;
		UINT32	sum_ltn;
		UINT32	ltn_bud;
		UINT32	other;
	} rw[LGBUS_RW_MAX];
} LX_LGBUS_MON_DATA_RAW_T;

/* Calucated value */
typedef struct
{
	struct
	{
		UINT32 	avg_ltn;
		UINT32 	port_sum;
	} rw[LGBUS_RW_MAX];

} LX_LGBUS_MON_DATA_BLK_T;

typedef struct
{
	UINT64	total_bw;

	struct
	{
		UINT64	total_bw;
		UINT32	other_bw;
		UINT32	port_bw[LGBUS_MAX_DDR_PORT];
		UINT32	score;
	} rw[LGBUS_RW_MAX];

} LX_LGBUS_MON_DATA_BUS_T;

typedef struct
{
	UINT64	max_bw;
	UINT32	blk_count;
} LX_LGBUS_MON_DATA_HW_T;

typedef struct
{
	UINT32						max_bus_num;	/* 1 = L BUS, 2 = L/G BUS, 3 = L/G/E BUS */

	LX_LGBUS_MON_DATA_RAW_T		raw[LGBUS_MAX_MNT_BLK];
	LX_LGBUS_MON_DATA_BLK_T		blk[LGBUS_MAX_MNT_BLK];
	LX_LGBUS_MON_DATA_BUS_T		bus[LGBUS_TYPE_MAX];

	LX_LGBUS_MON_DATA_HW_T		hw;
	LX_LGBUS_MON_CFG_T			cfg;
} LX_LGBUS_MON_DATA_T;



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

#define LGBUS_IOW_MON_START		_IOW(LGBUS_IOC_MAGIC, 0, LX_LGBUS_MON_CFG_T)
#define LGBUS_IO_MON_STOP		_IO(LGBUS_IOC_MAGIC, 1)
#define LGBUS_IOR_MON_READ		_IOR(LGBUS_IOC_MAGIC, 2, LX_LGBUS_MON_DATA_T)
#define LGBUS_IOR_MON_GETINFO	_IOR(LGBUS_IOC_MAGIC, 3, LX_LGBUS_MON_INFO_T)
#define LGBUS_IOR_MON_READREG	_IOR(LGBUS_IOC_MAGIC, 4, LX_LGBUS_MON_REG_T)
#define LGBUS_IOW_MON_WRITEREG	_IOW(LGBUS_IOC_MAGIC, 5, LX_LGBUS_MON_REG_T)

#define LGBUS_IOC_MAXNR			10

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LGBUS_KAPI_H_ */

