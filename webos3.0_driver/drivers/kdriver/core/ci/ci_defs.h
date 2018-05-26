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
 *  typedefs for ci device
 *
 *  author 		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version		1.0
 *  date		2009.03.23
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef	_CI_DEFS_H_
#define	_CI_DEFS_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "ci_cfg.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/* jinhwan.bae 20131018
    arm/kernel 3.x , M14/H14 Platform , HZ changed from 1000 to 100,
    msleep(1~10) makes about 15ms delay and
    msleep(10~) makes few ms variation.
    to change delay from msleep to usleep_range */
#define _CI_KDRV_DELAY_USLEEP_RANGE_	1


/* jinhwa.bae 20131026
    do R/W with 32bit BURST MODE */
#if 0
#define _CI_ACCESS_BURST_MODE_			1
#define _CI_ACCESS_BURST_MODE_IO_		1
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

#define	DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_OK 				5

/* 20131021 jinhwan.bae
    changed from 10 to 12. after changing delay function from msleep to usleep_range,
    IPP SMARDTV SmartCAM-3 CI+ Reference Module fail to Check CIS.
    After change this delay from 10 to 11, working well, but consider margin, set to 12*/
// #define	DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_NG 				10
#define	DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_NG 				12

#define	DEFAULT_CI_DELAY_CIS_END_WRITE_COR						100
#define	DEFAULT_CI_DELAY_CIS_DURING_READ_TUPLE					5
/* 20141007 hyunho747.kim
   changed from 20 to 160.
   Some CAMs require bigger delay between tuple blocks
   because they might transfer partialy wrong tuple data
   : CONAX CAM  (SN : 7060104300000025, Pairing ID : 005-8201-2102, Test 22 / SMiT)
   : STRONG CAM (SN : 7061120200001557, Pairing ID : 005-8217-5606, NO_07 / RisksTV)
*/
/* 20141013 hyunho747.kim
   changed from 160 to 200.
   Without log display, it required at least 200ms delay for this CAM
   : CONAX CAM  (SN : 7060104300000025, Pairing ID : 005-8201-2102, Test 22 / SMiT)
*/
/* 20141104 hyunho747.kim
   changed from 200 to 20 (back to the original value)
   Too long CIS time(http://hlm.lge.com/issue/browse/NEWCERT-491) issue
*/
#define	DEFAULT_CI_DELAY_CIS_END_READ_TUPLE_INITIAL				20
#define	DEFAULT_CI_DELAY_CIS_PARSE_NON_CI_TUPLE					3
#define	DEFAULT_CI_DELAY_INIT_POWER_CONTROL						5		// H13 Blocked in Routine, L9 5
#define	DEFAULT_CI_DELAY_INIT_AFTER_INTERRUPT_ENABLE			10
#define	DEFAULT_CI_DELAY_CAM_INIT_BTW_VCC_CARDRESET				300
#define	DEFAULT_CI_DELAY_CAM_INIT_BTW_CARDRESET_NOTRESET		5
#define	DEFAULT_CI_DELAY_IO_SOFT_RESET_CHECK_FR					10
#define	DEFAULT_CI_DELAY_IO_END_SOFT_RESET						0		// L9 Blocked, 0 Originally (10)
#define	DEFAULT_CI_DELAY_IO_NEGOBUF_BEFORE_SOFTRESET			0		// L9 Blocked, 0 Originally (100)
#define	DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_DA					10
#define	DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_FR					10
#define	DEFAULT_CI_DELAY_IO_NEGOBUF_AFTER_WRITE_DATA			5		// L9 Final 5, L9 Previously (10)
#define	DEFAULT_CI_DELAY_IO_READ_CHECK_DA						10
#define	DEFAULT_CI_DELAY_IO_WRITE_CHECK_DA						10
#define	DEFAULT_CI_DELAY_IO_WRITE_CHECK_FR						10
#define	DEFAULT_CI_DELAY_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE		10
#define	DEFAULT_CI_DELAY_IO_WRITE_MIDDLE_BYTE_CHECK_WE			10
#define	DEFAULT_CI_DELAY_IO_WRITE_LAST_BYTE_CHECK_WE			10


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_DEFS_H_ */

/** @} */

