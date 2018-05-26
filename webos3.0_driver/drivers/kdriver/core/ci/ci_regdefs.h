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
 *  CI register details. ( used only within kdriver )
 *
 *  @author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  @author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  @author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  @author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  @version	1.0
 *  @date		2010.2.22
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef __CI_REGDEFS_H__
#define __CI_REGDEFS_H__

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/version.h>
#include "../include/base_types.h"
#include "ci_reg.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#include <asm/hardware.h> // For Register base address
#endif

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
#define MAX_MOD_NUM			1

#if	!defined(INCLUDE_H15_CHIP_KDRV)
#ifndef LX_CHIP_H15
#define LX_CHIP_H15			0x17	/* H15= LG1211 */
#endif

#ifndef H15_IRQ_DVBCI
#define H15_IRQ_DVBCI			(32 + 25)
#endif
#endif

#ifdef USE_QEMU_SYSTEM
#define CI_REG_BASE 		0x02500000 // check this for QEMU
#define CI_CAM_BASE 		CI_REG_BASE
#define IRQ_CI 				1
#else

/* Local Definitions instead of Global Definition */
#ifndef H14_DVBCI_BASE
#define H14_DVBCI_BASE			(H14_CS2_BASE+0x0)
#endif
#define M14_A0_DVBCI_BASE		M14_DVBCI_BASE
#define M14_B0_DVBCI_BASE		M14_DVBCI_BASE		/* M14_TBD */
#define M14_A0_IRQ_DVBCI		M14_IRQ_DVBCI
#define M14_B0_IRQ_DVBCI		M14_IRQ_DVBCI		/* M14_TBD */

#define H13_CI_REG_BASE			( H13_DVBCI_BASE + 0x10000 )
#define H13_CI_CAM_BASE			( H13_DVBCI_BASE )
#define M14_A0_CI_REG_BASE		( M14_A0_DVBCI_BASE + 0x10000 )
#define M14_A0_CI_CAM_BASE		( M14_A0_DVBCI_BASE )
#define M14_B0_CI_REG_BASE		( M14_B0_DVBCI_BASE + 0x10000 )
#define M14_B0_CI_CAM_BASE		( M14_B0_DVBCI_BASE )
#define H14_CI_REG_BASE			( H14_DVBCI_BASE + 0x10000 )
#define H14_CI_CAM_BASE			( H14_DVBCI_BASE )

#ifndef H15_CS_BASE
#define H15_CS_BASE				(0xD0000000)
#define H15_CS0_BASE			(H15_CS_BASE+0x0000000)
#define H15_CS1_BASE			(H15_CS_BASE+0x4000000)
#define H15_CS2_BASE			(H15_CS_BASE+0x8000000)
#define H15_CS3_BASE			(H15_CS_BASE+0xc000000)
#endif

#ifndef H15_DVBCI_BASE
#define H15_DVBCI_BASE			(H15_CS2_BASE+0x0)
#endif
#define H15_CI_REG_BASE			( H15_DVBCI_BASE + 0x10000 )
#define H15_CI_CAM_BASE			( H15_DVBCI_BASE )
#endif /* USE_QEMU_SYSTEM */

/*
#define H13_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 		\
										do	{	\
										*( UINT16* )( H13_CI_CAM_BASE + _ADDR32_ ) = ( UINT16 )_DATA16_; \
										} while ( 0 ) \
*/
#define gRegCICAM		CiModBaseAddr[MOD_A]

#define _CI_HW_REG_WRITE(_ADDR32_, _DATA16_) 		\
						do	{	\
							*( UINT16* )( gRegCICAM + _ADDR32_ ) = ( UINT16 )_DATA16_; \
						} while ( 0 )

#if 0
#define H13_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 		_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#define M14_A0_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 	_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#define M14_B0_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 	_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#define H14_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 		_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#define H15_CI_REG_WRITE( _ADDR32_, _DATA16_ ) 		_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#else
#define CI_HW_REG_WRITE(_ADDR32_, _DATA16_)			_CI_HW_REG_WRITE(_ADDR32_, _DATA16_)
#endif

typedef union
{
	CTRL_STAT1 		ctrl_stat1;
	UINT16			unCntrlStat1;

} U_CI_CNTRL_STAT1_T;

typedef union
{
	CTRL_STAT2 		ctrl_stat2;
	UINT16			unCntrlStat2;

} U_CI_CNTRL_STAT2_T;

typedef union
{
	CTRL_STAT3 		ctrl_stat3;
	UINT16			unCntrlStat3;

} U_CI_CNTRL_STAT3_T;

typedef union
{
	PC_CARD_CTRL1 	pc_card_ctrl1;
	UINT16			unPCCardCntrl1;

} U_CI_PCCARD_CNTRL1_T;

typedef union
{
	PC_CARD_CTRL2 	pc_card_ctrl2;
	UINT16			unPCCardCntrl2;

} U_CI_PCCARD_CNTRL2_T;

typedef union
{
	PC_CARD_CTRL3 	pc_card_ctrl3;
	UINT16			unPCCardCntrl3;

} U_CI_PCCARD_CNTRL3_T;

typedef union
{
	BURST_CTRL 		burst_ctrl;
	UINT16			unBurstCntrl;

} U_CI_BURST_CNTRL_T;

typedef union
{
	INTR_FLAG 		intr_flag;
	UINT16			unIntrFlag;

} U_CI_INTR_FLAG_T;


typedef union
{
	INTR_FLAG 		intr_mask;
	UINT16			unIntrMask;

} U_CI_INTR_MASK_T;

typedef union
{
	INTR_FLAG 		intr_ctrl;
	UINT16			unIntrCntrl;

} U_CI_INTR_CNTRL_T;

typedef union
{
	HW_HOT_SWAP_CTRL1	hw_hot_swap_ctrl1;
	UINT16				unHWHotSwapCntrl1;

} U_CI_HW_HS_CNTRL1_T;

typedef union
{
	HW_HOT_SWAP_CTRL2	hw_hot_swap_ctrl2;
	UINT16				unHWHotSwapCntrl2;

} U_CI_HW_HS_CNTRL2_T;

typedef union
{
	HW_HOT_SWAP_CTRL3	hw_hot_swap_ctrl3;
	UINT16				unHWHotSwapCntrl3;

} U_CI_HW_HS_CNTRL3_T;

typedef union
{
	HW_HOT_SWAP_CTRL4	hw_hot_swap_ctrl4;
	UINT16				unHWHotSwapCntrl4;

} U_CI_HW_HS_CNTRL4_T;

typedef union
{
	HW_HOT_SWAP_CTRL5	hw_hot_swap_ctrl5;
	UINT16				unHWHotSwapCntrl5;

} U_CI_HW_HS_CNTRL5_T;

typedef union
{
	HW_HOT_SWAP_CTRL6	hw_hot_swap_ctrl6;
	UINT16				unHWHotSwapCntrl6;

} U_CI_HW_HS_CNTRL6_T;

typedef union
{
	HW_HOT_SWAP_CTRL7	hw_hot_swap_ctrl7;
	UINT16				unHWHotSwapCntrl7;

} U_CI_HW_HS_CNTRL7_T;

typedef union
{
	HW_HOT_SWAP_CTRL8	hw_hot_swap_ctrl8;
	UINT16				unHWHotSwapCntrl8;

} U_CI_HW_HS_CNTRL8_T;


typedef struct
{
	U_CI_CNTRL_STAT1_T 		uniCntrlStat1;		//0x1000
	U_CI_CNTRL_STAT2_T 		uniCntrlStat2;		//0x1002
	U_CI_CNTRL_STAT3_T 		uniCntrlStat3;		//0x1004

	U_CI_PCCARD_CNTRL1_T 	uniPCCardCntrl1;	//0x1006
	U_CI_PCCARD_CNTRL2_T 	uniPCCardCntrl2;	//0x1008
	U_CI_PCCARD_CNTRL3_T 	uniPCCardCntrl3;	//0x100a

	UINT16			unReserved1[2];		//0x100c - 0x100e
	U_CI_BURST_CNTRL_T 	uniBurstCntrl;	//0x1010

	UINT16 					unReserved[10];		//0x1012 - 0x1024

	U_CI_INTR_FLAG_T 		uniIntrFlag;		//0x1026
	U_CI_INTR_MASK_T 		uniIntrMask;		//0x1028
	U_CI_INTR_CNTRL_T 		uniIntrCntrl;		//0x102a

	UINT16 					unPCMCIAAddrInd;	//0x102c

	U_CI_HW_HS_CNTRL1_T 	uniHWHotSwapCntrl1;	//0x102e
	U_CI_HW_HS_CNTRL2_T 	uniHWHotSwapCntrl2;	//0x1030
	U_CI_HW_HS_CNTRL3_T 	uniHWHotSwapCntrl3;	//0x1032
	U_CI_HW_HS_CNTRL4_T 	uniHWHotSwapCntrl4;	//0x1034
	U_CI_HW_HS_CNTRL5_T 	uniHWHotSwapCntrl5;	//0x1036
	U_CI_HW_HS_CNTRL6_T 	uniHWHotSwapCntrl6;	//0x1038
	U_CI_HW_HS_CNTRL7_T 	uniHWHotSwapCntrl7;	//0x103a
	U_CI_HW_HS_CNTRL8_T 	uniHWHotSwapCntrl8;	//0x103c

} S_CI_REG_T;


#if 0
typedef struct
{
	UINT16 unCntrlStat1;		//0x1000
	UINT16 unCntrlStat2;		//0x1002
	UINT16 unCntrlStat3;		//0x1004

	UINT16 unPCCardCntrl1;		//0x1006
	UINT16 unPCCardCntrl2;		//0x1008
	UINT16 unPCCardCntrl3;		//0x100a

	UINT16			unReserved1[2];		//0x100c - 0x100e
	U_CI_BURST_CNTRL_T 	uniBurstCntrl;	//0x1010

	UINT16 					unReserved[10];		//0x1012 - 0x1024

	UINT16 unIntrFlag;			//0x1026
	UINT16 unIntrMask;			//0x1028
	UINT16 unIntrCntrl;			//0x102a

	UINT16 unPCMCIAAddrInd;		//0x102c

	UINT16 unHWHotSwapCntrl1;	//0x102e
	UINT16 unHWHotSwapCntrl2;	//0x1030
	UINT16 unHWHotSwapCntrl3;	//0x1032
	UINT16 unHWHotSwapCntrl4;	//0x1034
	UINT16 unHWHotSwapCntrl5;	//0x1036
	UINT16 unHWHotSwapCntrl6;	//0x1038
	UINT16 unHWHotSwapCntrl7;	//0x103a
	UINT16 unHWHotSwapCntrl8;	//0x103c

} S_CI_REG_T;
#endif /* #if 0 */

/* module id */
typedef enum MOD_ID_ENUM
{
	MOD_A		= 0,
   	MOD_B		= 1,
   	INVALID_MOD = 2

} MOD_ID;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* __CI_REGDEFS_H__ */

/** @} */



