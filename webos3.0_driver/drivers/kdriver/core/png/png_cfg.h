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
 *  main configuration file for png device
 *	png device will teach you how to make device driver with new platform.
 *
 *  @author		juhee
 *  version		1.0
 *  date		2011.12.02
 *  note		Additional information.
 *
 *  @addtogroup lg_png
 *	@{
 */

#ifndef	_PNG_CFG_H_
#define	_PNG_CFG_H_

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
#define	PNG_MODULE			"png"

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
    char*       memory_name;
    UINT32      memory_base;
    UINT32      memory_size;
} LX_PNG_MEM_CFG_T;

typedef struct	
{
	UINT32			chip;
	UINT32			png_reg_base;
    UINT32          png_reg_size;
	UINT32			icod_reg_base;
    UINT32          icod_reg_size;
    UINT32          platform;
	int             devno;
    UINT32          irq_num;
    UINT32          num_device;
    LX_PNG_MEM_CFG_T mem_cfg;
} PNG_CFG_TBL_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_PNG_MEM_CFG_T 		gMemCfgPng[];

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
int PNG_Config(PNG_CFG_TBL_T *pPNG);



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PNG_CFG_H_ */

/** @} */

