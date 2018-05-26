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
 *  main configuration file for gfx device
 *	gfx device will teach you how to make device driver with new platform.
 *
 *  author		raxis.lim (raxis.lim@lge.com)
 *  version		1.0 
 *  date		2009.12.30
 *  note		Additional information. 
 *
 *  @addtogroup lg1150_gfx 
 *	@{
 */

#ifndef	_GFX_CFG_H_
#define	_GFX_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define	GFX_USE_NOISY_ERR_DEBUG		/** print nosiy ouput when GFX error detected */

#define	GFX_MAX_RGN_NUM		2

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	GFX_MODULE			"gfx"
#define GFX_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	LX_MEMCFG_T	surface[GFX_MAX_RGN_NUM];
}
LX_GFX_MEM_CFG_T;

typedef struct
{
	UINT32		chip;
	char*		chip_name;
	UINT32		reg_base_addr;
	UINT32		reg_size;
	UINT32		irq_num;
}
LX_GFX_REG_CFG_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern LX_GFX_MEM_CFG_T		gMemCfgGfx;
extern LX_GFX_REG_CFG_T*	gpGfxRegCfg;

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_CFG_H_ */

/** @} */

