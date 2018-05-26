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
 *  main configuration file for memprot device
 *	memprot device will teach you how to make device driver with new platform.
 *
 *  author		sanghyeun.kim
 *  version		1.0
 *  date		2012.08.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

#ifndef	_MEMPROT_CFG_H_
#define	_MEMPROT_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <asm/io.h>
#include <linux/interrupt.h>
#include "os_util.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	MEMPROT_MODULE			"memprot"
#define MEMPROT_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
/*Memory protection base address*/
#define MEMPROT_LBUS_BASE	H13_LBUS_BASE
#define MEMPROT_GBUS_BASE	H13_GBUS_BASE

/*Interrupt status register*/
#define MEMPROT_LBUS_INT_STATUS		H13_LBUS_BASE
#define MEMPROT_GBUS_INT_STATUS		H13_GBUS_BASE + 0x1900


/*Control register offset in each monitor*/
#define MEMPROT_REG_SIZE		0xFF  /*< memory protection register size*/

#define MEMPROT_INTR_EN_OFFSET	0x10  /*< offset to interrupt register*/
#define MEMPROT_BASE_OFFSET		0xB0  /*< offset to memory protection base address*/
#define MEMPROT_END_OFFSET		0xB4  /*< offset to memory protection end address */
#define MEMPROT_STATUS0_OFFSET	0xB8  /*< offset to violated address*/
#define MEMPROT_STATUS1_OFFSET	0xBC  /*< offset to violated ID*/

/* interrupt control bit of each monitor */
#define MEMPROT_INTR_EN_BIT		0x20  /*< bit to enable  memprotection interrupt */
#define MEMPROT_INTR_STATUS_BIT 	0x2000  /*< bit to indicate  memprotection interrupt */
#define MEMPROT_INTR_CLEAR_BIT 		0x200000  /*< bit to clear  memprotection interrupt */
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/*enum for memory protection monitors*/
typedef enum
{

#if INCLUDE_KDRV_MEMPROT==0xB0
	/* LBUS */
  ENUM_MEMPROT_AUDIO=0 ,
  ENUM_MEMPROT_GPU0 ,
  ENUM_MEMPROT_GPU1 ,
  ENUM_MEMPROT_GFX ,
  ENUM_MEMPROT_ICOD ,
  ENUM_MEMPROT_TE ,
  ENUM_MEMPROT_VDEC_P ,
  ENUM_MEMPROT_VDEC_S ,
  ENUM_MEMPROT_VENC_P ,
  ENUM_MEMPROT_VENC_S ,
  ENUM_MEMPROT_DE_E_CVD ,
  ENUM_MEMPROT_DE_E_MCU ,
  ENUM_MEMPROT_CPU_M ,
  ENUM_MEMPROT_LPORT_1 ,
  ENUM_MEMPROT_LPORT_2 ,
  ENUM_MEMPROT_LPORT_3 ,


	/* GBUS*/
  ENUM_MEMPROT_DE_A ,
  ENUM_MEMPROT_DE_B ,
  ENUM_MEMPROT_DE_C ,
  ENUM_MEMPROT_DE_D ,
  ENUM_MEMPROT_DPP_A ,
  ENUM_MEMPROT_DVI ,
  ENUM_MEMPROT_DVO ,
  ENUM_MEMPROT_SRE ,
  ENUM_MEMPROT_MEP_A ,
  ENUM_MEMPROT_MEP_B,
  ENUM_MEMPROT_BVE_MCU ,
  ENUM_MEMPROT_MC ,
  ENUM_MEMPROT_TCON ,
  ENUM_MEMPROT_CPU_S ,
  ENUM_MEMPROT_GPORT_1 ,
  ENUM_MEMPROT_GPORT_2 ,
  ENUM_MEMPROT_GPORT_3 ,
#else // for A0 board
	/* LBUS */
  ENUM_MEMPROT_AUDIO=0 ,
  ENUM_MEMPROT_GPU0 ,
  ENUM_MEMPROT_GPU1 ,
  ENUM_MEMPROT_GFX ,
  ENUM_MEMPROT_ICOD ,
  ENUM_MEMPROT_TE ,
  ENUM_MEMPROT_VDEC_P ,
  ENUM_MEMPROT_VDEC_S ,
  ENUM_MEMPROT_VENC_P ,
  ENUM_MEMPROT_VENC_S ,
  ENUM_MEMPROT_CPU_M ,

	/* GBUS*/
  ENUM_MEMPROT_DE_A ,
  ENUM_MEMPROT_DE_B ,
  ENUM_MEMPROT_DE_C ,
  ENUM_MEMPROT_DE_D ,
  ENUM_MEMPROT_DE_E_CVD ,
  ENUM_MEMPROT_DE_E_MCU ,
  ENUM_MEMPROT_DPP_A ,
  ENUM_MEMPROT_DVI ,
  ENUM_MEMPROT_DVO ,
  ENUM_MEMPROT_SRE ,
  ENUM_MEMPROT_MEP_A ,
  ENUM_MEMPROT_MEP_B,
  ENUM_MEMPROT_BVE_MCU ,
  ENUM_MEMPROT_MC ,
  ENUM_MEMPROT_CPU_S ,
#endif
  ENUM_MEMPROT_MAX ,

}ENUM_MEMPROT_T ;

#define MONITOR_NAME_LEN 16

typedef struct {
	ENUM_MEMPROT_T id;     /*< monitor ID */
	char name[MONITOR_NAME_LEN];
	unsigned int reg_offset; /*< register offset*/
	unsigned int int_bit;   /*< intrupt bit */
} MONITOR_INFO_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MEMPROT_CFG_H_ */

/** @} */

