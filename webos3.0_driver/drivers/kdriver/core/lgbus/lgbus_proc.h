/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  driver interface header for lgbus device. ( used only within kdriver )
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  @author		bongrae.cho (bongrae.cho@lge.com)
 *  @version	1.0
 *  @date		2012.11.22
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef	_LGBUS_PROC_H_
#define	_LGBUS_PROC_H_

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


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
enum {
    PROC_ID_READ_BW_MON = 0,
    PROC_ID_CFG_REPEAT_CNT,
    PROC_ID_CFG_REQ_BLKS,
    PROC_ID_CFG_SCR_CLR,
    PROC_ID_PRINT_BW_MON,
    PROC_ID_README,
    PROC_ID_MAX,
};

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void LGBUS_PROC_Init(void);
void LGBUS_PROC_Cleanup (void);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LGBUS_PROC_H_ */

/** @} */
