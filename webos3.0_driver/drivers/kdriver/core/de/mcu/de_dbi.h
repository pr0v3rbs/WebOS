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
 * verification code for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.02.23
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef _DE_DBI_h
#define _DE_DBI_h

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum {
	OK					= 0,
	NOT_OK				= -1,
	INVALID_PARAMS		= -2,
	NOT_ENOUGH_RESOURCE = -3,
	NOT_SUPPORTED		= -4,
	NOT_PERMITTED		= -5,
	TIMEOUT				= -6,
	NO_DATA_RECEIVED	= -7,
	DN_BUT_OVERFLOW		= -8
} DTV_STATUS_T;

typedef struct {
	char * CmdStr;
	unsigned int cmdId;
	int (* func) (int argc, char **argv);
} DBI_CMD_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int DBI_VIDEO_Execute(int argc, char **argv);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif

/**  @} */

