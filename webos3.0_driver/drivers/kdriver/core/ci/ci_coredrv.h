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
 *  core driver interface header for ci device. ( used only within kdriver )
 *
 *  @author		Srinivasan Shanmugam	(srinivasan.shanmugam@lge.com)
 *  @author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  @author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  @author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  @version	1.0
 *  @date		2010.2.22
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef	_CI_COREDRV_H_
#define	_CI_COREDRV_H_

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

#include "ci_kapi.h"
#include "ci_regdefs.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	PCMCIA_BUS_SPEED_MIN = 0x0,
	PCMCIA_BUS_SPEED_LOW,
	PCMCIA_BUS_SPEED_HIGH,
	PCMCIA_BUS_SPEED_MAX
} CI_BUS_SPEED_T;

typedef enum
{
	ACCESS_1BYTE_MODE = 0,
	ACCESS_2BYTE_MODE = 1,
	ACCESS_4BYTE_MODE = 2
}CI_ACCESS_MODE_T;

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define INIT_VAL			0x0000
#define CLK_128				0x0080
#define CLK_511				0X01FF
#define CLK_639 			0X027F
#define	CD1_CD2_HIGH		0x03
#define CD_POL				0x80

#define DA_STATUS			0x80

/* Control and Status1 */
#define VCC_SW0				0x0004
#define EN_PCCARD			0x0020
#define CARD_RESET			0x0080

/* Control and Status3 */
#define IO_INT_MODE			0x0004
#define POD_MODE			0x0010

/* PC Card Control1 */
#define CHIP_MODE_IO		0X001D
#define CHIP_MODE_ATTR		0X001B

/* for CI plus */
#define RESET_PHY_INT		0x10
#define IIR_STATUS			0x10

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
extern void CI_InitSMC(void);
extern void CI_UnmapSMC(void);
extern void CI_DefaultInit(void);

extern SINT32 CI_Check_CD_Pol(UINT32 opt, const char *func);

extern SINT32 CI_Initialize( void );

extern SINT32 CI_UnInitialize( void );

extern SINT32 CI_ResetCI( void );

extern SINT32 CI_CAMInit( void );

extern SINT32 CI_CAMPowerOff( void );

extern SINT32 CI_CAMPowerOnCompleted( void );

extern SINT32 CI_CheckCIS( void );

extern SINT32 CI_WriteCOR( void );

extern SINT32 CI_CAMDetectStatus( UINT32 *o_puwIsCardDetected );

extern SINT32 CI_ReadData ( UINT8 *o_pbyData, UINT16 *io_pwDataBufSize, int to_user );

extern SINT32 CI_NegoBuff( UINT32 *o_puwBufSize );

extern SINT32 CI_ReadDAStatus( UINT32 *o_puwIsDataAvailable, int log_sel );

extern SINT32 CI_WriteData ( UINT8 *i_pbyData, UINT32 i_wDataBufSize, int from_user );

extern SINT32 CI_ResetPhysicalIntrf( void );

extern SINT32 CI_SetRS(void );

extern SINT32 CI_ReadIIRStatus( UINT32 *o_puwIIRStatus, int log_sel );

extern SINT32 CI_CheckCAMType( SINT8 *o_pRtnValue, UINT8 *o_puwCheckCAMType );

extern SINT32 CI_GetCIPlusSupportVersion( SINT8 *o_pRtnValue, UINT32 *o_puwVersion );

extern SINT32 CI_GetCIPlusOperatorProfile( SINT8 *o_pRtnValue, UINT32 *o_puwProfile);

extern SINT32 CI_RegPrint( void );

extern SINT32 CI_RegWrite( UINT32 ui32Offset, UINT32 ui32Value );

extern SINT32 CI_CAMSetDelay( CI_DELAY_TYPE_T eDelayType, UINT32 uiDelayValue );

extern SINT32 CI_CAMPrintDelayValues( void );

extern SINT32 CI_SetPCCardBusTiming( CI_BUS_SPEED_T speed );

extern SINT32 CI_ChangeAccessMode( CI_ACCESS_MODE_T mode );

extern SINT32 CI_DataTest(MOD_ID mId);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_COREDRV_H_ */

/** @} */
