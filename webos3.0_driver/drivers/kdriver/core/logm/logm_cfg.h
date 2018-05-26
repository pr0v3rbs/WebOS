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
 *  main configuration file for logm device
 *	logm device will teach you how to make device driver with new platform.
 *
 *  author		hwang.hayun (hwang.hayun@lge.com)
 *  version		1.0 
 *  date		2013.10.29
 *  note		Additional information. 
 *
 *  @addtogroup lg115x_logm 
 *	@{
 */

#ifndef	_LOGM_CFG_H_
#define	_LOGM_CFG_H_

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
#define	LOGM_MODULE			"logm"
#define LOGM_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

int LOGM_ObjFDGet(const char *name);
int LOGM_ReadLog(struct logm_buffer_ctrl *bc, 
                    char *data, int readed, int size);
const char *LOGM_LevelName(LX_LOGM_LOGLEVEL_T l);
int LOGM_PrintkLevel(LX_LOGM_LOGLEVEL_T l);
int LOGM_ParseLog(struct logm_header *h, char *dest, int32_t dest_len);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern const char	*g_logm_clrStrings[];

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LOGM_CFG_H_ */

/** @} */

