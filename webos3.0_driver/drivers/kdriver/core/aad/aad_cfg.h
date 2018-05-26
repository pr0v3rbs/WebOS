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
 *  main configuration file for aad device
 *	aad device will teach you how to make device driver with new platform.
 *
 *  author		jongsang.oh (jongsang.oh@lge.com)
 *  version		1.0 
 *  date		2010.01.26
 *  note		Additional information. 
 *
 *  @addtogroup lg1150_aad 
 *	@{
 */

#ifndef	_AAD_CFG_H_
#define	_AAD_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

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
#define	AAD_MODULE			"aad"
#define AAD_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
AAD Register configuration.
*/
typedef struct
{
	char*		chip_name;			// 	Chip Name
	UINT32		reg_base_addr;		//	AAD Register Base Address for Real Chip.
	UINT32		reg_size;			//	AAD Register Size for Real Chip.
	UINT32		irq_num;			//	AAD IRQ Number for Real Chip.
} LX_AAD_REG_CFG_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AAD_CFG_H_ */

/** @} */

