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
 *  main configuration file for afe device
 *	afe device will teach you how to make device driver with new platform.
 *
 *  author		wonsik.do (wonsik.do@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_afe
 *	@{
 */

#ifndef	_AFE_CFG_H_
#define	_AFE_CFG_H_

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
#define	AFE_MODULE			"afe"
#define AFE_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
Vport Register configuration.
*/
typedef struct
{
	char*		chip_name;					// Chip Name
	UINT32		vport_reg_base_addr;		//	vport Register Base Address for Real Chip.
	UINT32		vport_reg_size;				//	vport Register Size  for Real Chip.
	UINT32		vport_qemu_base_addr;		//	vport Qemu Register Base Address.
	UINT32		chbcvd_reg_base_addr;		//	CHBCVD Register Base Address for Real Chip.
	UINT32		chbcvd_reg_size;			//	CHBCVD Register Size for Real Chip.
} LX_CVD_REG_T;

typedef struct
{
     char*       memory_name;
     unsigned int        memory_base;
     unsigned int        memory_size;
} LX_CVD_MEM_CFG_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_CVD_MEM_CFG_T gMemCfgCvd[];
extern LX_CVD_MEM_CFG_T gCvdMemCfg;
extern LX_CVD_REG_T *pVportRegCfg, gRegCfgVport[];

extern LX_CVD_MEM_CFG_T*	gpCvdMemCfg;
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AFE_CFG_H_ */

/** @} */

