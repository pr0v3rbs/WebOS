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
 *  CIS extraction layer
 *
 *  author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version	0.6
 *  date		2010.02.22
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */



/*=============================================================================+
| Global Symbol Definition                                                     |
+=============================================================================*/

#ifndef	_CI_CIS_H_
#define	_CI_CIS_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "ci_regdefs.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------------------*/
/* Max count for nonvalid tuples */
#define MAX_TUPLE_COUNT_ERR		300



/* tuple code */
#define CISTPL_VERS_1			0x15
#define CISTPL_CONFIG			0x1A
#define CISTPL_CFTABLE_ENTRY	0x1B
#define CISTPL_DEVICE_OC		0x1C
#define CISTPL_DEVICE_OA		0x1D
#define CISTPL_MANFID			0x20
#define CISTPL_DEVICE			0x01
#define CISTPL_DEVICE_A			0x17
#define CISTPL_DEVICE_TEMP		0x24

#define CISTPL_NO_LINK			0x14
#define CISTPL_END				0xFF
#define CISTPL_LINK_END			0xFF

/* sub-tuple code */
#define CFGST_CIF				0xC0
#define CCST_TERMINATE			0xFF
#define STCE_EV					0xC0
#define STCE_PD					0xC1

/* DVB-CI compliant information */

#define	TPLLV1_MAJOR			0x05	// in CISTPL_VERS_1
#define	TPLLV1_MINOR			0x00	// in CISTPL_VERS_1
#define	TPCC_RADR_MAX			0x0FFE	// in CISTPL_CONFIG
#define	DVB_CI_COMPLIANT_IFN	0x0241	// in CISTPL_CONFIG-CCST_CIF
#define TPCE_IF					0x04	// in CISTPL_CFTABLE_ENTRY
#define TPCE_IO					0x22	// in CISTPL_CFTABLE_ENTRY

/* return value */
#define	CIS_OK					0
#define	CIS_FAIL				(-1)
#define	CIS_PARSETIMEOUT		(-2)

/* IO register access macro */
#if	0//!defined(INCLUDE_H15_CHIP_KDRV)
#define BYTE(addr)				*((volatile unsigned char *) (addr))
#define WORD(addr)				*((volatile unsigned short *) (addr))			/* Burst mode */
#define DWORD(addr)				*((volatile unsigned int *) (addr))				/* Burst mode */
#define CONFIG_REG(addr)		*((volatile unsigned char *) (addr))
#endif

/* CIS tuple data bit definition */
#define	EXT_BIT					0x80
/* CISTPL_CONFIG */
#define	TPCC_RASZ_MASK			0x03
#define	TPCC_RMSZ_MASK			0x3c
#define	TPCC_RFSZ_MASK			0xc0
#define CONFIG_OPTION_BIT		0x01
#define	CONFIG_STATUS_BIT		0x02
#define	PIN_REPLACE_BIT			0x04
#define SOCKET_COPY_BIT			0x08
#define	EXT_STATUS_BIT			0x10
#define	IO_BASE0_BIT			0x20
#define	IO_BASE1_BIT			0x40
#define	IO_BASE2_BIT			0x80
#define	IO_BASE3_BIT			0x01
#define	IO_LIMIT_BIT			0x02
#define PWR_MANSUP_BIT			0x04
#define	TPCE_FS_MISC_BIT		0x80
#define TPCE_FS_MEMSP_BIT		0x60
#define	TPCE_FS_IRQ_BIT			0x10
/* CISTPL_CFTABLE_ENTRY */
#define	TPCE_INDX_IF_BIT		0x80
#define	TPCE_INDX_DEF_BIT		0x40
#define	TPCE_FS_IOSP_BIT		0x08
#define TPCE_FS_TIME_BIT		0x04
#define	TPCE_FS_POWER_BIT		0x03
/* POWER description structure */
#define	PWR_PDWNI_BIT			0x40
#define	PWR_PEAKI_BIT			0x20
#define	PWR_AVRGI_BIT			0x10
#define	PWR_STATI_BIT			0x08
#define	PWR_MAXV_BIT			0x04
#define	PWR_MINV_BIT			0x02
#define PWR_NOMV_BIT			0x01
/* CISTPL_DEVICE_OA & OC */
#define	VCC_5V			0
#define	VCC_3V3			1
#define VCC_xVx			2
#define	VCC_CB			3
#define	DTYPE_NULL		0x0
#define	DTYPE_ROM		0x1
#define	DTYPE_OTPROM	0x2
#define	DTYPE_EPROM		0x3
#define	DTYPE_EEPROM	0x4
#define	DTYPE_FLASH		0x5
#define	DTYPE_SRAM		0x6
#define	DTYPE_DRAM		0x7
#define	DTYPE_FUNCSPEC	0xd
#define	DTYPE_EXTEND	0xe
#define	DSPEED_NULL		0x0
#define	DSPEED_250NS	0x1
#define DSPEED_200NS	0x2
#define	DSPEED_150NS	0x3
#define	DSPEED_100NS	0x4
#define	DSPEED_EXT		0x7
#define	SP_MAN1_0		0x1
#define	SP_MAN1_2		0x2
#define	SP_MAN1_3		0x3
#define	SP_MAN1_5		0x4
#define	SP_MAN2_0		0x5
#define	SP_MAN2_5		0x6
#define	SP_MAN3_0		0x7
#define	SP_MAN3_5		0x8
#define	SP_MAN4_0		0x9
#define	SP_MAN4_5		0xa
#define	SP_MAN5_0		0xb
#define	SP_MAN5_5		0xc
#define	SP_MAN6_0		0xd
#define	SP_MAN7_0		0xe
#define	SP_MAN8_0		0xf
#define	SP_1ns			0x0
#define	SP_10ns			0x1
#define	SP_100ns		0x2
#define	SP_1us			0x3
#define	SP_10us			0x4
#define	SP_100us		0x5
#define	SP_1ms			0x6
#define	SP_10ms			0x7
#define _512B			0x0
#define _2KB			0x1
#define _8KB			0x2
#define _32KB			0x3
#define _128KB			0x4
#define _512KB			0x5
#define _2MB			0x6
/* CISTPL_VERS_1 */
#define	PCSTAND_FEB95	0x0500
#define	PCMCIA2_x		0x0401
#define	PCMCIA1_0		0x0400
/* CISTPL_CFTABLE_ENTRY */
#define	MEMORY			0x0
#define	IO_MEMORY		0x1
#define	CUSTOM_IF0		0x4
#define	CUSTOM_IF1		0x5
#define	CUSTOM_IF2		0x6
#define	CUSTOM_IF3		0x7
#define	VCC_ONLY		0x01
#define	VCC_VPP			0x02
#define	VCC_VPP_12		0x03
#define	SGL2			0x01
#define	LNG2CADDR2		0x02
#define	MEMSP_DES		0x03
#define	PWR_MAN1_0		0x0
#define	PWR_MAN1_2		0x1
#define	PWR_MAN1_3		0x2
#define	PWR_MAN1_5		0x3
#define	PWR_MAN2_0		0x4
#define	PWR_MAN2_5		0x5
#define	PWR_MAN3_0		0x6
#define	PWR_MAN3_5		0x7
#define	PWR_MAN4_0		0x8
#define	PWR_MAN4_5		0x9
#define	PWR_MAN5_0		0xa
#define	PWR_MAN5_5		0xb
#define	PWR_MAN6_0		0xc
#define	PWR_MAN7_0		0xd
#define	PWR_MAN8_0		0xe
#define	PWR_MAN9_0		0xf
#define	PWR_100nA		0x0
#define	PWR_1uA			0x1
#define	PWR_10uA		0x2
#define	PWR_100uA		0x3
#define	PWR_1mA			0x4
#define	PWR_10mA		0x5
#define	PWR_100mA		0x6
#define	PWR_1A			0x7
#define	PWR_10uV		0x0
#define	PWR_100uV		0x1
#define	PWR_1mV			0x2
#define	PWR_10mV		0x3
#define	PWR_100mV		0x4
#define	PWR_1V			0x5
#define	PWR_10V			0x6
#define	PWR_100V		0x7
#define	IO_8BIT			0x1
#define	IO_16BIT		0x2
#define	IO_BOTH			0x3
#define DEVICE_OAOC_STORAGE_MAX		1

#define VERS_1_INFO_LEN	40
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/* tuple status	vaue */
typedef enum TPL_STATUS_ENUM {
	DVB_CI_COMPLIANT		= 0,
	DVB_CI_NOT_COMPLIANT	= 1,
	DATA_AVAILABLE			= 2,
	DATA_NOT_AVAILABLE		= 3
}
TPL_STATUS;


/**
 *	tuple status structure
 */
typedef struct {
	TPL_STATUS	vers_1;
	TPL_STATUS	config;
	TPL_STATUS	cftable_entry;
#if 0 	 //not used
	TPL_STATUS	no_link;
	TPL_STATUS	device;
	TPL_STATUS	device_a;
	TPL_STATUS	device_oa;
	TPL_STATUS	device_oc;
	TPL_STATUS	manfid;
	TPL_STATUS	dummy[3];
#endif
}
CISTPL_STATUS;


/**
 *	tuple(DEVICE_OAOC) information structure
 */
typedef struct {
	UINT8	otherInfo;
	UINT8	id;
	UINT8	xspeed;
	UINT8	xdevice;
	UINT8	deviceSize;
	UINT8	dummy[3];
}
DEVICE_OAOC;


/**
 *	tuple(DEVICE_AC) information structure
 */
typedef struct {
	UINT8	id;
	UINT8	xspeed;
	UINT8	xdevice;
	UINT8	deviceSize;
}
DEVICE_AC;



/**
 *	tuple(VERS_1) information structure
 */
typedef struct {
	UINT8	major;
	UINT8	minor;
	UINT8	manufact[ VERS_1_INFO_LEN ];
	UINT8	product[ VERS_1_INFO_LEN ];
	UINT8	add1[ VERS_1_INFO_LEN ];
	UINT8	add2[ VERS_1_INFO_LEN ];
	UINT8	ui8IsItCIPlus;
	UINT8	dummy[1];
	UINT32	version;
	UINT32	profile;
}
VERS_1;


/**
 *	tuple(CONFIG) information structure
 */
typedef struct {
	UINT8	size;
	UINT8	lastIndex;
	UINT16	mask;
	UINT32	baseAddr;
}
CONFIG;




#define CCST_CIF_STRING_LENGTH		20

/**
 *	tuple(CCST_CIF) information structure
 */
typedef struct {
	UINT8	ifIdNumBase;
	UINT8	ifIdNum;
	UINT8	str[CCST_CIF_STRING_LENGTH]; // 20 bytes
	UINT8	dummy[2];
}
CCST_CIF;


/**
 *	tuple(PWR_DESCRIPT) information structure
 */
typedef struct {
	UINT8		pwr_sel;
	UINT8		nom_v;
	UINT8		nom_v_x;
	UINT8		min_v;
	UINT8		min_v_x;
	UINT8		max_v;
	UINT8		max_v_x;
	UINT8		stat_i;
	UINT8		stat_i_x;
	UINT8		avrg_i;
	UINT8		avrg_i_x;
	UINT8		peak_i;
	UINT8		peak_i_x;
	UINT8		pdwn_i;
	UINT8		pdwn_i_x;
	UINT8		dummy;
}
PWR_DESCRIPT;


/**
 *	tuple(CFTABLE_ENTRY) information structure
 */
typedef struct {
	UINT8	cfgTableIdx;
	UINT8	interface;
	UINT8	featureSel;
	UINT8	dummy;
	PWR_DESCRIPT	pwrInfoField[3];
}
CFTABLE_ENTRY;

/**
 *	tuple(MANFID) information structure
 */
typedef struct {
	UINT16	manf;
	UINT16	card;
}
MANFID;

/**
 *	tuple(STCE_EVPD) information structure
 */
typedef struct {
	UINT8	environment[10];
	UINT8	physicalDev[20];
	UINT8	dummy[2];
}
STCE_EVPD;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
SINT32 CIS_WhatCAM( UINT8 mId, UINT8 *o_pui8IsItCIPlusCAM );
SINT32 CIS_GetCIPlusCAMSupportVersion( UINT8 mId, UINT32 *o_pui32Version );
SINT32 CIS_GetCIPlusCAMOperatorProfile( UINT8 mId, UINT32 *o_pui32Profile );
SINT32 CIS_Config(UINT8 mId);

SINT32 CIS_WriteCOR(UINT8 mId);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_CIS_H_ */
