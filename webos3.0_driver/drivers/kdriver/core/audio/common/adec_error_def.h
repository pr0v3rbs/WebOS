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


/**
 *	@file adec_error_def.h
 *
 *
 *	@author		Jung, Kyung Soon (kyungsoon.jung@leg.com)
 *	@version	0.0.1
 *	@date		2012-03-15
 *	@note
 *	@see		http://www.lge.com
 *	@addtogroup ADEC_COMMON
 *	@{
 */


/*-----------------------------------------------------------------------------
 *  Should Define ADEC_ERR_DEF(_TYPE_NAME, _ERR_STR)
 *-----------------------------------------------------------------------------*/

// Avoiding intellisense error
#ifndef ADEC_ERR_DEF
#define ADEC_ERR_DEF(...)
#endif
ADEC_ERR_DEF(ADEC_ERR_GENERAL, 			"Not sepcified, but error")
ADEC_ERR_DEF(ADEC_ERR_RANGE, 			"Value range error")
ADEC_ERR_DEF(ADEC_ERR_NULL, 			"Parameter should not be null")
ADEC_ERR_DEF(ADEC_ERR_DATA_CORRUPTED, 	"Structure or the condition seems like to be broken")
ADEC_ERR_DEF(ADEC_ERR_PERMISSION, 		"Current operation is not allowed for this object")
ADEC_ERR_DEF(ADEC_ERR_EMPTY_QUEUE,		"This queue has no item")
ADEC_ERR_DEF(ADEC_ERR_OVERFLOW, 		"Buffer overflow")
ADEC_ERR_DEF(ADEC_ERR_UNDERFLOW, 		"Buffer underflow")
ADEC_ERR_DEF(ADEC_ERR_TIMEOUT, 			"Timeout")
ADEC_ERR_DEF(ADEC_ERR_UNKNOWN,			"Unknown command, module or device")
ADEC_ERR_DEF(ADEC_ERR_WAITING,			"Should wait for it")
ADEC_ERR_DEF(ADEC_ERR_ALREADY_EXIST,	"The item, device or module is already exsist")
ADEC_ERR_DEF(ADEC_ERR_ALIGNMENT,		"Alignment broken")
ADEC_ERR_DEF(ADEC_ERR_NOT_READY, 		"Not ready")
ADEC_ERR_DEF(ADEC_ERR_NOT_SUPPORT, 		"Not supported feature")
ADEC_ERR_DEF(ADEC_ERR_NOT_ENOUGH_MEM, 	"Not enough memory")
ADEC_ERR_DEF(ADEC_ERR_NOT_IMPLEMENTED, 	"Scheduled feature, but not implemented yet")
ADEC_ERR_DEF(ADEC_ERR_NOT_DEFINED,		"Some variable or object, should be defined, not defined")
ADEC_ERR_DEF(ADEC_ERR_NOT_FOUND,		"Cannot find specific item or value")
ADEC_ERR_DEF(ADEC_ERR_OUT_OF_RESOURCE,	"No resource available to use.")
ADEC_ERR_DEF(ADEC_ERR_INVALID_LOCATION,	"Invalid location")
ADEC_ERR_DEF(ADEC_ERR_DATA_SIZE,		"Data size exceeds the maximum length")
ADEC_ERR_DEF(ADEC_ERR_STATE,			"Cannot do this action in this state")
ADEC_ERR_DEF(ADEC_ERR_CONNECTION,		"Already connected, all not connected yet")
ADEC_ERR_DEF(ADEC_ERR_WRAPAROUND,		"Wraparound Detected")
ADEC_ERR_DEF(ADEC_ERR_DUPLICATED,		"The value has already been exist before")
ADEC_ERR_DEF(ADEC_ERR_SYNC_FAIL,		"Fail to find sync")
ADEC_ERR_DEF(ADEC_ERR_SYNTAX,			"Syntax error")
ADEC_ERR_DEF(ADEC_ERR_NEED_MORE_DATA,	"Not enough data to parse the packet or frame")
ADEC_ERR_DEF(ADEC_ERR_CORRUPTED,        "Struct or data seems like to be corrupted, broken")


/** @} */


