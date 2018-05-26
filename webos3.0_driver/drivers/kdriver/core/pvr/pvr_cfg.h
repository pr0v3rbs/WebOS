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
 *  main configuration file for pvr device
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		kyungbin.pak
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef	_PVR_CFG_H_
#define	_PVR_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "pvr_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	PVR_MODULE			"pvr"
#define PVR_MAX_DEVICE		2

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define H13_DVR_REG_BASE		(H13_TE_BASE + 0x1400)
#define M14_A0_DVR_REG_BASE		(M14_TE_BASE + 0x1400)
#define M14_B0_DVR_REG_BASE		(M14_B0_TE_BASE + 0x1400)
#define H14_DVR_REG_BASE		(H14_TE_BASE + 0x1400)
#define H15_DVR_REG_BASE		(H15_TE_BASE + 0x1400)



/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

typedef struct
{
	int 			irqNum;
	UINT32			regBase;

	UINT8			num_dndev;		// number of upload device
	UINT8			num_updev;		// number of download device
	UINT8			mem_shared;		// shared channel A mem in M14Bx (memory limitation)

	UINT8			fixup_timestamp;	// H15 timestamp bug
} LX_PVR_CFG_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void PVR_CFG_PreInit(void);
LX_PVR_CFG_T* PVR_CFG_GetConfig(void);
LX_MEMCFG_T* PVR_CFG_GetMemConfig(LX_PVR_CH_T ch);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_MEMCFG_T	gMemCfgPvr[PVR_MAX_DEVICE][LX_PVR_MEM_NUM];


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_CFG_H_ */

/** @} */

