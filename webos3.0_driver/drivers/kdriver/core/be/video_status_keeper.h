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
 *  header file for Status Keeper 
 *	
 *  author		won.hur(won.hur@lge.com)	
 *  version		1.0
 *  date		2013.04.50
 *  note		Additional information.
 *
 *  @addtogroup VIDEO 
 *	@{
 */

#ifndef _VIDEO_STATUS_KEEPER_H_
#define _VIDEO_STATUS_KEEPER_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef  __cplusplus
extern "C"
{
#endif /* __cplusplus */


#include <linux/string.h>	

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define LX_VP_ST_KEEPER_CMD_HISTORY_BUFFER_SIZE		50 * 20 // History for each module 

#define LX_VP_ST_KEEPER_TABLE_INDEX_LENGTH	50
#define LX_VP_ST_KEEPER_TABLE_ITEM_MAX 	30		// RSR TABLE Y LABEL

#define LX_VP_ST_KEEPER_HISTORY_START_CODE	0x0DD0FECA
#define LX_VP_ST_KEEPER_HISTORY_END_CODE	0xEFBEADDE

#define LX_VP_ST_KEEPER_HISTORY_START_CODE_0	0xCA	
#define LX_VP_ST_KEEPER_HISTORY_START_CODE_1	0xFE
#define LX_VP_ST_KEEPER_HISTORY_START_CODE_2	0xD0
#define LX_VP_ST_KEEPER_HISTORY_START_CODE_3	0x0D

#define LX_VP_ST_KEEPER_HISTORY_END_CODE_0		0xDE
#define LX_VP_ST_KEEPER_HISTORY_END_CODE_1		0xAD
#define LX_VP_ST_KEEPER_HISTORY_END_CODE_2		0xBE
#define LX_VP_ST_KEEPER_HISTORY_END_CODE_3		0xEF



#define LX_VP_ST_KEEPER_MODULE_NAME_MAX_CHAR 20	
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SKEEPER_CONVERT_ENUM_TO_STRING(x) #x	
#define SKEEPER_INSERT_DATA(pdata, name)  {strcpy(pdata[name].index ,SKEEPER_CONVERT_ENUM_TO_STRING(name));}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/* Add module for management */	
typedef enum {
	LX_VP_ST_KEEPER_MODULE_BE = 0,
	LX_VP_ST_KEEPER_MODULE_VBI,
	LX_VP_ST_KEEPER_MODULE_OVI,
	LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM
} LX_VP_ST_KEEPER_MODULE_TYPE_T;
	
typedef struct{
	char index[LX_VP_ST_KEEPER_MODULE_NAME_MAX_CHAR];
} LX_VP_ST_KEEPER_MODULE_NAME_T;

/* ---------------------------------------------------------------------- */
/* Do Not modifiy below.... */
/* -----------------------------------------------------------------------*/
typedef struct{
	char index[LX_VP_ST_KEEPER_TABLE_INDEX_LENGTH];

} LX_VP_ST_KEEPER_META_DATA_T;	

typedef enum {
	LX_VP_ST_KEEPER_HISTORY_NONE = 0,
	LX_VP_ST_KEEPER_HISTORY_IO,
	LX_VP_ST_KEEPER_HISTORY_DISPATCH,
	LX_VP_ST_KEEPER_HISTORY_MAXNUM
} LX_VP_ST_KEEPER_HISTORY_TYPE_T;

typedef enum {
	LX_VP_ST_KEEPER_STATUS_ERROR = -1,
	LX_VP_ST_KEEPER_STATUS_UNINITIALIZED = 0,
	LX_VP_ST_KEEPER_STATUS_INITIALIZED,
	LX_VP_ST_KEEPER_STATUS_NO_MODULE_LOADED,
	LX_VP_ST_KEEPER_STATUS_MODULE_LOADED,
	LX_VP_ST_KEEPER_STATUS_MAXNUM
} LX_VP_ST_KEEPER_STATUS_T;

typedef enum{
	LX_VP_ST_KEEPER_MODULE_STATUS_ERROR = -1,
	LX_VP_ST_KEEPER_MODULE_STATUS_UN_REGISTERED,
	LX_VP_ST_KEEPER_MODULE_STATUS_OFF,
	LX_VP_ST_KEEPER_MODULE_STATUS_ON,
	LX_VP_ST_KEEPER_MODULE_STATUS_MAXNUM
} LX_VP_ST_KEEPER_MODULE_STATUS_T;

typedef enum{
	LX_VP_ST_KEEPER_HISTORY_STATUS_ERROR = -1,
	LX_VP_ST_KEEPER_HISTORY_STATUS_UNINITIALIZED = 0,
	LX_VP_ST_KEEPER_HISTORY_STATUS_OFF,
	LX_VP_ST_KEEPER_HISTORY_STATUS_INITIALIZED,
	LX_VP_ST_KEEPER_HISTORY_STATUS_ON,
	LX_VP_ST_KEEPER_HISTORY_STATUS_MAXNUM
} LX_VP_ST_KEEPER_HISTORY_STATUS_T;

typedef struct {
	unsigned int status;
	char index[LX_VP_ST_KEEPER_TABLE_INDEX_LENGTH];
	unsigned int last_modified_time;
	unsigned int modifier;
} LX_VP_ST_KEEPER_INDEX_T;

typedef struct {
	int			   rsrIndexCount;
	LX_VP_ST_KEEPER_INDEX_T *pRsrIndex;
} LX_VP_ST_KEEPER_TABLE_DATA_T;


typedef struct{
	unsigned int ioctl;
	unsigned int size;
	unsigned int *pData;
} LX_VP_ST_KEEPER_HISTORY_INDEX_T;

typedef struct {
	unsigned char *pStartAddress;
	unsigned char *pEndAddress;
	unsigned int Head;
	//unsigned int Tail;	// No Tail needed.. We will show the whole buffer...
	LX_VP_ST_KEEPER_HISTORY_STATUS_T status;
} LX_VP_ST_KEEPER_HISTORY_T;

typedef struct {
	LX_VP_ST_KEEPER_MODULE_STATUS_T   status[LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM];
	LX_VP_ST_KEEPER_TABLE_DATA_T	  table[LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM];
	LX_VP_ST_KEEPER_HISTORY_T		  io_log[LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM];
	LX_VP_ST_KEEPER_HISTORY_T		  dispatch_log[LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM];
} LX_VP_ST_KEEPER_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int VIDEO_SKeeperInit(void);
int VIDEO_SKeeperUnInit(void);

int VIDEO_SKeeperListUpIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_INDEX_T *rsrIndex, int size);
int VIDEO_SKeeperDeleteIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module);
int VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module, int index, int status);

//int VIDEO_SKeeper_IO_Handler(LX_VP_ST_KEEPER_MODULE_TYPE_T module, unsigned int ioctl, unsigned int arg );
//int VIDEO_SKeeper_Dispatcher(LX_VP_ST_KEEPER_MODULE_TYPE_T module, unsigned int ioctl, unsigned int arg );

int VIDEO_SKeeper_InitHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type);
int VIDEO_SKeeper_UnInitHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type);

int VIDEO_SKeeper_PushHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type, LX_VP_ST_KEEPER_HISTORY_INDEX_T *data);

int VIDEO_SKeeper_DBG_Ioctl(unsigned int cmd, unsigned long arg);


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/



#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _VIDEO_STATUS_KEEPER_H_ */

/** @} */


