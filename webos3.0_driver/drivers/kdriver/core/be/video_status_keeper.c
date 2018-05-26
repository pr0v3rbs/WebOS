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
 *  Main file for Status Keeper 
 *
 *  author		won.hur(won.hur@lge.com)	
 *  version		1.0
 *  date		2013.04.05
 *  note		Additional information.
 *
 *  @addtogroup VIDEO
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/sched.h>

#include "base_types.h"
#include "video_status_keeper.h"
#include "be_kapi.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define ST_KEEPER_PRINT(fmt, args...)		{if(_gSKeeperDebugPrintFlag >= 1) printk(fmt, ##args);}
#define ST_KEEPER_LOG_PRINT(fmt, args...)	{if(_gSKeeperDebugPrintFlag >= 2) printk(fmt, ##args);}

#define ST_KEEPER_CHECK_DATABASE()	{if(gSKeeperDataBase == NULL){\
											ret=RET_ERROR;\
											ST_KEEPER_PRINT("[%s:%d] ST Manager DB not found!\n", __F__, __L__);\
											break;}}

#define ST_KEEPER_CHECK_STATUS()		{if(gSKeeperStatus <= LX_VP_ST_KEEPER_STATUS_UNINITIALIZED){\
											ret=RET_ERROR;\
											ST_KEEPER_PRINT("[%s:%d] ST Manager DB was not initiated!\n", __F__, __L__);\
											break;\
											}\
										else if(gSKeeperStatus >= LX_VP_ST_KEEPER_STATUS_MAXNUM){\
											ret=RET_ERROR;\
											ST_KEEPER_PRINT("[%s:%d] Fatal Error! Memory was contaminated!\n", __F__, __L__);\
											break;}}

#define MAKE_PTID(pid,tid)  ((((pid)&0xffff)<<16)|((tid)&0xffff))
/* How to use above macro...
 *  MAKE_PTID((int)task_tgid_nr(current), (int)task_pid_nr(current))
 */


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct{
	unsigned int header;
	unsigned int pid;
	unsigned int time;
	unsigned int ioctl;
	unsigned int size;
	unsigned int tail;
} LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
int __VIDEO_SKeeper_DBG_CheckStatus(void);
int __VIDEO_SKeeper_DBG_PrintModuleList(void);
int __VIDEO_SKeeper_DBG_PrintIndexLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module);
int __VIDEO_SKeeper_DBG_PrintHistoryLog( LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type);
int __VIDEO_SKeeper_DBG_DrawHistoryLog(unsigned char *log, unsigned int size);
int __VIDEO_SKeeper_DBG_Set_TimeStampShiftLength(int shiftAmount);
int __VIDEO_SKeeper_DBG_Debugger(BE_SKEEPER_DBG_T param);
int	__VIDEO_SKeeper_DBG_EnableDBGPrint(BOOLEAN bOnOff);
int	__VIDEO_SKeeper_DBG_EnableDBGLogPrint(BOOLEAN bOnOff);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
LX_VP_ST_KEEPER_T *gSKeeperDataBase = NULL;
static LX_VP_ST_KEEPER_STATUS_T	gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_UNINITIALIZED;
static int _gSKeeperTimeStampShiftUnit	= 4;
static int _gSKeeperDebugPrintFlag	= 0;


LX_VP_ST_KEEPER_MODULE_NAME_T gSKeeperModuleName[] = 
{
	{"BE MODULE\n"},
	{"VBI MODULE\n"},
	{"OVI MODULE\n"},
};

/*========================================================================================
	Implementation Group
========================================================================================*/
int VIDEO_SKeeperInit(void)
{
	int ret = RET_ERROR;

	do{
		/* Step 1. Check multiple-initialization */
		if(gSKeeperDataBase != NULL){
			ST_KEEPER_PRINT("[%s:%d] ST Manager was already initialized.\n",__F__, __L__);
			ret = RET_OK;
			break;
		}

		/* Step 2. Allocate & Check memory for Global ST Manager Data Base */
		gSKeeperDataBase = (LX_VP_ST_KEEPER_T *)kmalloc(sizeof(LX_VP_ST_KEEPER_T), GFP_KERNEL);
		if(gSKeeperDataBase == NULL){
			ST_KEEPER_PRINT("[%s:%d] Failed to allocate memory for RSR DataBase\n", __F__, __L__);
			gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_ERROR;
			break;
		}

		/* Step 3. Initiate Global ST Manager Data Base */
		memset(gSKeeperDataBase, 0, sizeof(LX_VP_ST_KEEPER_T));
		
		/* Step 4. Set Status for Global ST Manager */
		gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_INITIALIZED;

		ST_KEEPER_PRINT("[%s:%d] ST Manager initialization complete.\n",__F__, __L__);
		ret = RET_OK;
	}while(0);

	return ret;
}


int VIDEO_SKeeperUnInit(void)
{
	int ret = RET_ERROR;
	int i;

	do{
		ST_KEEPER_CHECK_DATABASE()

		/* Step 1. Check for un-allocated memory & Free them*/
		/* Step 1-1. Check ST_KEEPER_TABLE_DATA for listed modules */
		for(i=0; i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM; i++){
			if(gSKeeperDataBase->table[i].pRsrIndex != NULL) kfree((void *)gSKeeperDataBase->table[i].pRsrIndex);
		}

		/* Step 1-2. Check IO HANDLER HISTORY DATA for listed modules */
		for(i=0; i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM; i++){
			if(gSKeeperDataBase->io_log[i].pStartAddress  != NULL) kfree((void *)gSKeeperDataBase->io_log[i].pStartAddress);
			gSKeeperDataBase->io_log[i].pStartAddress	= NULL;
			gSKeeperDataBase->io_log[i].pEndAddress		= NULL;
		}
		
		/* Step 1-3. Check DISPATCHER HISTORY DATA for listed modules */
		for(i=0; i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM; i++){
			if(gSKeeperDataBase->dispatch_log[i].pStartAddress != NULL) kfree((void *)gSKeeperDataBase->dispatch_log[i].pStartAddress);
			gSKeeperDataBase->dispatch_log[i].pStartAddress	= NULL;
			gSKeeperDataBase->dispatch_log[i].pEndAddress	= NULL;

		}

		/* Step 2. Free Data base memory */
		if(gSKeeperDataBase!= NULL) kfree((void*)gSKeeperDataBase);
 		
		/* Step 3. Reset DB pointer */
		gSKeeperDataBase= NULL;

		/* Step 4. Update Global status for Resource Manager */
		gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_UNINITIALIZED;
	
		ret = RET_OK;
	}while(0);

	return ret;
}


int VIDEO_SKeeperListUpIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_INDEX_T *rsrIndex, int size)
{
	int ret = RET_ERROR;

	/* ETC : Turn On DBG print */
	__VIDEO_SKeeper_DBG_EnableDBGPrint(1);
	
	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/* Step 0. Check module type argument */
		if(module >= LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM) {
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown module type[%d] \n", __F__, __L__, module);
			break;
		}

		/* Step 1. Check resource manager status for called module */
		if(gSKeeperDataBase->status[module] >= LX_VP_ST_KEEPER_MODULE_STATUS_ON){
			ST_KEEPER_PRINT("[%s:%d] Error! Can't list up index while operating... \n", __F__, __L__);
			break;
		}

		/* Step 2. Check whether RESOURCE LIST BUFFER WAS NOT CLEAR */
		if(gSKeeperDataBase->table[module].pRsrIndex != NULL) {
			kfree((void *)gSKeeperDataBase->table[module].pRsrIndex);
			gSKeeperDataBase->table[module].pRsrIndex = NULL;
		}

		/* Step 3. Check size to prevent buffer overflow */
		if(size > LX_VP_ST_KEEPER_TABLE_ITEM_MAX){
			ST_KEEPER_PRINT("[%s:%d] Error! Can't list up index while operating... \n", __F__, __L__);
			gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ERROR;
			break;
		}

		/* Step 4. Check resource pointer for segmentation fault */
		/* Change this part to access_ok() for better usage...
		 * Or may just use this for performance cause...*/
		if(rsrIndex == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! Index list pointer was NULL... \n", __F__, __L__);
			gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ERROR;
			break;
		}

		/* Step 5. Allocate memory for resource item in ST Manager Buffer */
		gSKeeperDataBase->table[module].pRsrIndex = (LX_VP_ST_KEEPER_INDEX_T *)kmalloc((sizeof(LX_VP_ST_KEEPER_INDEX_T) * size), GFP_KERNEL);
		
		/* Step 6. Check result of memory allocation */
		if(gSKeeperDataBase->table[module].pRsrIndex == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! Memory allocation failed while requesting for index buffer \n", __F__, __L__);
			gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ERROR;
			break;
		}

		/* Step 7. Copy resource contents to ST Manager Buffer */
		memcpy((void *)gSKeeperDataBase->table[module].pRsrIndex, (void *)rsrIndex, (sizeof(LX_VP_ST_KEEPER_INDEX_T) * size));

		/* Step 8. Update ST Manager BUFFER TABLE X Count */
		gSKeeperDataBase->table[module].rsrIndexCount = size;

		/* Step 9. Update status & Exit */
		gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ON;
		ret = RET_OK;
	}while(0);

	/* ETC : Turn off enabled DBG print */
	__VIDEO_SKeeper_DBG_EnableDBGPrint(0);
	
	return ret;
}

int VIDEO_SKeeperDeleteIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module)
{
	int ret = RET_ERROR;
	int loaded_module_count = 0;
	int i;

	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/* Step 0. Check module type argument */
		if(module >= LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM) {
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown module type[%d] \n", __F__, __L__, module);
			break;
		}

		/* Step 1. Check resource manager status for called module */
		if(gSKeeperDataBase->status[module] >= LX_VP_ST_KEEPER_MODULE_STATUS_OFF){
			ST_KEEPER_PRINT("[%s:%d] Error! Can't delete index while operating... \n", __F__, __L__);
			ret = RET_OK;
			break;
		}

		/* Step 2. Check allocated memory & unallocate them all */
		if(gSKeeperDataBase->table[module].pRsrIndex != NULL){
			kfree(gSKeeperDataBase->table[module].pRsrIndex);
			gSKeeperDataBase->table[module].pRsrIndex = NULL;
		}

		/* Step 3. Reset Table index counters */
		gSKeeperDataBase->table[module].rsrIndexCount = 0;

		/* Step 4. Update ST Manager module Status */
		gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_UN_REGISTERED;

		/* Step 5. Check ST Manager global status */
		for(i=0; i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM; i++){
			if(gSKeeperDataBase->status[module] >= LX_VP_ST_KEEPER_MODULE_STATUS_OFF){
				loaded_module_count++;
			}
		}
		
		/* Step 6. Update ST Manager global status */
		if(loaded_module_count >0) gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_MODULE_LOADED;
		else gSKeeperStatus = LX_VP_ST_KEEPER_STATUS_NO_MODULE_LOADED;

		/* Step 8. Exit */
		ret = RET_OK;

	}while(0);

	return ret;
}


int VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_TYPE_T module, int index, int status)
{
	int ret = RET_ERROR;
	LX_VP_ST_KEEPER_INDEX_T *statusData;
		
	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/* Step 0. Check module type argument */
		if(module >= LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM) {
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown module type[%d] \n", __F__, __L__, module);
			break;
		}

		/* Step 1. Check resource manager status for called module */
		if(gSKeeperDataBase->status[module] <= LX_VP_ST_KEEPER_MODULE_STATUS_OFF){
			ST_KEEPER_PRINT("[%s:%d] Warning! Will not update Index. Status if OFF. \n", __F__, __L__);
			break;
		}

		/* Step 2. Check whether Index Buffer pointer */
		if(gSKeeperDataBase->table[module].pRsrIndex == NULL) {
			ST_KEEPER_PRINT("[%s:%d] ERROR! Index buffer pointer is NULL. \n", __F__, __L__);
			break;
	
		}

		/* Step 3. Check size to prevent buffer overflow */
		if((index >= gSKeeperDataBase->table[module].rsrIndexCount)||(index < 0)){
			ST_KEEPER_PRINT("[%s:%d] Error! Inserted index number does not exist. Index[%d]/Max[%d] \n", __F__, __L__, index, gSKeeperDataBase->table[module].rsrIndexCount);
			gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ERROR;
			break;
		}

		/* Step 4. Update status & Exit */
#if 0
		statusData =  &gSKeeperDataBase->table[module].pRsrIndex;
		statusData[index]->status = status;
		statusData[index]->last_modified_time = (unsigned int)(jiffies>>_gSKeeperTimeStampShiftUnit);
		statusData[index]->modifier = MAKE_PTID((int)task_tgid_nr(current), (int)task_pid_nr(current));
#endif
		gSKeeperDataBase->table[module].pRsrIndex[index].status = status;
		gSKeeperDataBase->table[module].pRsrIndex[index].last_modified_time = (unsigned int)(jiffies>>_gSKeeperTimeStampShiftUnit);
		gSKeeperDataBase->table[module].pRsrIndex[index].modifier = MAKE_PTID((int)task_tgid_nr(current), (int)task_pid_nr(current));

		ret = RET_OK;
	}while(0);

	return ret;
}


int VIDEO_SKeeper_InitHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type)
{
	int ret = RET_ERROR;
	unsigned char *pStartAddress = NULL;


	do{
		/* Step 1. Check Status Keeper Database */
		if(gSKeeperDataBase == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! ST Manager was not initialized.\n",__F__, __L__);
			break;
		}

		/* Step 2. Check type whether it is valid or not */
		if((type <= LX_VP_ST_KEEPER_HISTORY_NONE) || (type >= LX_VP_ST_KEEPER_HISTORY_MAXNUM)){
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown history log type[%d].\n",__F__, __L__, type);
			break;
		}

		/* Step 3. Check whether this is not the first time, free previous memory space */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			pStartAddress = gSKeeperDataBase->io_log[module].pStartAddress;
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			pStartAddress = gSKeeperDataBase->dispatch_log[module].pStartAddress;
		}

		if(pStartAddress != NULL){
			kfree(pStartAddress);
		}

		/* Step 4. Allocate memory space for history log */
		pStartAddress = (unsigned char *)kmalloc((sizeof(unsigned char) * LX_VP_ST_KEEPER_CMD_HISTORY_BUFFER_SIZE), GFP_KERNEL);
		
		/* Step 5. Check result of memory allocation */
		if(pStartAddress == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! Memory allocation failed while requesting for history buffer \n", __F__, __L__);
			break;
		}

		/* Step 6. Initiate Table data */
		memset((void *)pStartAddress, 0, (sizeof(unsigned char) * LX_VP_ST_KEEPER_CMD_HISTORY_BUFFER_SIZE));

		/* Step 7. Update status & Exit */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			gSKeeperDataBase->io_log[module].pStartAddress = pStartAddress;
			gSKeeperDataBase->io_log[module].pEndAddress = (unsigned char *)((unsigned int)pStartAddress + (sizeof(unsigned char) * LX_VP_ST_KEEPER_CMD_HISTORY_BUFFER_SIZE));
			gSKeeperDataBase->io_log[module].status = LX_VP_ST_KEEPER_HISTORY_STATUS_OFF;

		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			gSKeeperDataBase->dispatch_log[module].pStartAddress = pStartAddress;
			gSKeeperDataBase->dispatch_log[module].pEndAddress = (unsigned char *)((unsigned int)pStartAddress + (sizeof(unsigned char) * LX_VP_ST_KEEPER_CMD_HISTORY_BUFFER_SIZE));
			gSKeeperDataBase->dispatch_log[module].status = LX_VP_ST_KEEPER_HISTORY_STATUS_OFF;	
		}

		ret = RET_OK;

	}while(0);

	return ret;
}

int VIDEO_SKeeper_UnInitHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type)
{
	int ret = RET_ERROR;
	unsigned char *pStartAddress = NULL;


	do{
		/* Step 1. Check Status Keeper Database */
		if(gSKeeperDataBase == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! ST Manager was not initialized.\n",__F__, __L__);
			break;
		}

		/* Step 2. Check type whether it is valid or not */
		if((type <= LX_VP_ST_KEEPER_HISTORY_NONE) || (type >= LX_VP_ST_KEEPER_HISTORY_MAXNUM)){
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown history log type[%d].\n",__F__, __L__, type);
			break;
		}

		/* Step 3. Check whether this is not the first time, free previous memory space */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			pStartAddress = gSKeeperDataBase->io_log[module].pStartAddress;
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			pStartAddress = gSKeeperDataBase->dispatch_log[module].pStartAddress;
		}

		if(pStartAddress != NULL){
			kfree(pStartAddress);
		}

		/* Step 4. Update status & Exit */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			gSKeeperDataBase->io_log[module].pStartAddress = NULL;
			gSKeeperDataBase->io_log[module].pEndAddress = NULL;	
			gSKeeperDataBase->io_log[module].Head = 0;	
			gSKeeperDataBase->io_log[module].status = LX_VP_ST_KEEPER_HISTORY_STATUS_UNINITIALIZED;
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			gSKeeperDataBase->dispatch_log[module].pStartAddress = NULL;
			gSKeeperDataBase->dispatch_log[module].pEndAddress = NULL;
			gSKeeperDataBase->dispatch_log[module].Head = 0;	
			gSKeeperDataBase->dispatch_log[module].status = LX_VP_ST_KEEPER_HISTORY_STATUS_UNINITIALIZED;
		}

		ret = RET_OK;

	}while(0);

	return ret;
}

int VIDEO_SKeeper_PushHistoryLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type, LX_VP_ST_KEEPER_HISTORY_INDEX_T *data)
{
	int ret = RET_ERROR;
	unsigned char *pWriteAddress = NULL;
	unsigned int  totalWriteSize = 0;
	unsigned int  leftMemorySize = 0;
	LX_VP_ST_KEEPER_HISTORY_T historyDatabase;
	LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T writeData;

	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/* Step 1. Check type whether it is valid or not */
		if((type <= LX_VP_ST_KEEPER_HISTORY_NONE) || (type >= LX_VP_ST_KEEPER_HISTORY_MAXNUM)){
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown history log type[%d].\n",__F__, __L__, type);
			break;
		}

		/* Step 2. Get History DataBase */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			memcpy((void *)&historyDatabase, (void *)&gSKeeperDataBase->io_log[module], sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			memcpy((void *)&historyDatabase, (void *)&gSKeeperDataBase->dispatch_log[module], sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}

		/* Step 3. Check status */
		if(historyDatabase.status <= LX_VP_ST_KEEPER_HISTORY_STATUS_OFF)
		{
			//ST_KEEPER_PRINT("[%s:%d] Logging OFF! Log status off for module[%d], type[%d]\n",__F__, __L__, module, type);
			ret = RET_OK;
			break;
		}

		/* Step 4. Check History Index pointers.. */
		if((historyDatabase.pStartAddress == NULL)||(historyDatabase.pEndAddress == NULL)){
			ST_KEEPER_PRINT("[%s:%d] Null pointer! One of the pointers was NULL. S[0x%x]/E[0x%x]\n",__F__, __L__, (unsigned int)historyDatabase.pStartAddress, (unsigned int)historyDatabase.pEndAddress);
			break;
		}

		/* Step 5. Check pointer of pushing data */
		if(data == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! Data pointer is NULL[0x%x] on module[%d], type[%d]\n",__F__, __L__, (unsigned int)data, module, type);
			break;
		}
			
		/* Step 6. Copy data to buffer */
		/* 6-0) Ready Data */
		writeData.header = LX_VP_ST_KEEPER_HISTORY_START_CODE;
		writeData.tail   = LX_VP_ST_KEEPER_HISTORY_END_CODE;
		writeData.pid 	 = MAKE_PTID((int)task_tgid_nr(current), (int)task_pid_nr(current));
		writeData.ioctl	 = data->ioctl;
		writeData.size	 = data->size;
		writeData.time 	 = (unsigned int)(jiffies>>_gSKeeperTimeStampShiftUnit);
		/* 6-1) Get total write size */
		totalWriteSize = sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) + data->size;
		
		/* 6-2) Get left memory space */
		leftMemorySize = (unsigned int)((unsigned int)historyDatabase.pEndAddress - (unsigned int)historyDatabase.pStartAddress) - (unsigned int)historyDatabase.Head;
		
		/* 6-3) Get write position */
		pWriteAddress = (unsigned char *)((unsigned int)historyDatabase.pStartAddress + (unsigned int)historyDatabase.Head);
		
		/* Debugging */
		ST_KEEPER_LOG_PRINT("\n[%s:%d] Begin writing.. HEAD[%d] StartAddr[0x%08x] EndAddr[0x%08x] Status[%d]\
						\n\t\tPID[0x%08x] TIME[0x%08x] IOCTL[0x%08x] SIZE[%d] ADDR[0x%08x]\n \
						\n\t\tleftMem[%d] pWriteAddr[0x%08x]\n",\
				__F__, __L__, historyDatabase.Head, (unsigned int)historyDatabase.pStartAddress, (unsigned int)historyDatabase.pEndAddress, historyDatabase.status, \
				writeData.pid, writeData.time, (unsigned int)data->ioctl, data->size, (unsigned int)data->pData, leftMemorySize, (unsigned int)pWriteAddress);\

		/* 6-3) Write to buffer */
		/* 6-3-1) Write Data header */
		if(leftMemorySize >= (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int))) {
			memcpy((void *)pWriteAddress, (void *)&writeData, (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)));
			ST_KEEPER_LOG_PRINT("S1 : pWriteAddress[0x%08x], T_size[0x%x] - U16size[0x%x] = [0x%x]\n", (unsigned int)pWriteAddress, sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T), sizeof(unsigned int), (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)));
			pWriteAddress = (unsigned char *)((unsigned int)pWriteAddress + (unsigned int)(sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)));
			ST_KEEPER_LOG_PRINT("S2 : pWriteAddress[0x%08x]\n", (unsigned int)pWriteAddress );
			leftMemorySize =  leftMemorySize - (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int));
		} 
		else {
			memcpy((void *)pWriteAddress, (void *)&writeData, leftMemorySize);
			pWriteAddress = historyDatabase.pStartAddress;

			memcpy((void *)pWriteAddress, (void *)((unsigned int)&writeData + (unsigned int)(leftMemorySize)) ,  (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)) - leftMemorySize);
			pWriteAddress = (unsigned char *)((unsigned int) pWriteAddress + (sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)) - leftMemorySize);

			leftMemorySize =  (unsigned int)((unsigned int)historyDatabase.pEndAddress - (unsigned int)historyDatabase.pStartAddress - ((sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int)) - leftMemorySize));
		}
		
		/* 6-3-2) Write Data */
		if(leftMemorySize >= data->size) {
			ST_KEEPER_LOG_PRINT("S3 : Read *pWriteAddress[0x%08x] pWriteAddress[0x%08x], data->size[0x%x]\n", (unsigned int)*pWriteAddress, (unsigned int)pWriteAddress, data->size); 
			memcpy((void *)pWriteAddress, (void *)data->pData, data->size);
			pWriteAddress = (unsigned char *)((unsigned int)pWriteAddress + data->size);
			ST_KEEPER_LOG_PRINT("S4 : Read *pWriteAddress[0x%08x] pWriteAddress[0x%08x]\n", (unsigned int)*pWriteAddress, (unsigned int)pWriteAddress); 
			leftMemorySize =  leftMemorySize - data->size;
		} 
		else {
			memcpy((void *)pWriteAddress, (void *)data->pData, leftMemorySize);
			pWriteAddress = historyDatabase.pStartAddress;

			memcpy((void *)pWriteAddress, (void *)((unsigned int)data->pData + (unsigned int)(leftMemorySize)) ,  data->size - leftMemorySize);
			pWriteAddress = (unsigned char *)((unsigned int)pWriteAddress +  data->size - leftMemorySize);

			leftMemorySize =  (unsigned int)((unsigned int)historyDatabase.pEndAddress - (unsigned int)historyDatabase.pStartAddress - data->size - leftMemorySize);
		}

		/* 6-3-3) Write Tail */
		if(leftMemorySize >= sizeof(unsigned int)) {
			ST_KEEPER_LOG_PRINT("S5 : Read *pWriteAddress[0x%08x] pWriteAddress[0x%08x], sizeof(unsigned int)[0x%x]\n", (unsigned int)*pWriteAddress, (unsigned int)pWriteAddress, sizeof(unsigned int)); 
			memcpy((void *)pWriteAddress, (void *)&writeData.tail, sizeof(unsigned int));
			pWriteAddress = (unsigned char *)((unsigned int)pWriteAddress + sizeof(unsigned int) + 1);
			ST_KEEPER_LOG_PRINT("S6 : Read *pWriteAddress[0x%08x] pWriteAddress[0x%08x]\n", (unsigned int)*pWriteAddress, (unsigned int)pWriteAddress); 
			leftMemorySize =  leftMemorySize - sizeof(unsigned int);
		} 
		else {
			memcpy((void *)pWriteAddress, (void *)&writeData.tail, leftMemorySize);
			pWriteAddress = historyDatabase.pStartAddress;

			memcpy((void *)pWriteAddress, (void *)((unsigned int)&writeData.tail + (unsigned int)(leftMemorySize)) ,  sizeof(unsigned int) - leftMemorySize);
			pWriteAddress = (unsigned char *)((unsigned int)pWriteAddress +  sizeof(unsigned int) - leftMemorySize + 1);

			leftMemorySize =  (unsigned int)((unsigned int)historyDatabase.pEndAddress - (unsigned int)historyDatabase.pStartAddress - sizeof(unsigned ) - leftMemorySize);
		}	

		
		/* 7) Update Global Header */
		historyDatabase.Head = (unsigned int)pWriteAddress - (unsigned int)historyDatabase.pStartAddress;

		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			memcpy((void *)&gSKeeperDataBase->io_log[module], (void *)&historyDatabase, sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			memcpy((void *)&gSKeeperDataBase->dispatch_log[module], (void *)&historyDatabase, sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}
		
		ret = RET_OK;
	}while(0);

	return ret;
}

/*
int VIDEO_SKeeper_IO_Handler()
{
	return RET_OK;
}

int VIDEO_SKeeper_Dispatcher()
{
	return RET_OK;
}
*/


int VIDEO_SKeeper_DBG_Ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_ERROR;
	BE_SKEEPER_DBG_T param;
	
	switch(cmd)
	{
		case BE_IOW_STATUS_KEEPER_DBG:
		{
			ret = copy_from_user(&param, (BE_SKEEPER_DBG_T *)arg, sizeof(BE_SKEEPER_DBG_T));
			if(ret) {
				ST_KEEPER_PRINT("[%s:%d] Return Error when copying data from usr. ret [%d].\n", __F__, __L__, ret);
				break;
			}
			ret = __VIDEO_SKeeper_DBG_Debugger(param);
		}
		break;
		
		default:
			break;
	}

	return ret;
}

/* -----------------------------------------------------------------------------------------------------
 * Static Functions Below this line .....
 * -----------------------------------------------------------------------------------------------------*/
int	__VIDEO_SKeeper_DBG_EnableDBGPrint(BOOLEAN bOnOff)
{
	int ret = RET_OK;

	if(bOnOff) _gSKeeperDebugPrintFlag = 1;
	else _gSKeeperDebugPrintFlag = 0;

	return ret;
}

int	__VIDEO_SKeeper_DBG_EnableDBGLogPrint(BOOLEAN bOnOff)
{
	int ret = RET_OK;

	if(bOnOff) _gSKeeperDebugPrintFlag = 2;
	else _gSKeeperDebugPrintFlag = 0;

	return ret;
}




int __VIDEO_SKeeper_DBG_Debugger(BE_SKEEPER_DBG_T param)
{
	int ret = RET_ERROR;

	do{
		/* Step 1. Check if cmd is in valid range. */
		if(((int)param.cmd_type<0)||(param.cmd_type>=BE_SKEEPER_DBG_CMD_MAXNUM)){
			ST_KEEPER_PRINT("[%s:%d] Unknown CMD Type. cmd [%d].\n", __F__, __L__, param.cmd_type);
			break;
		}

		/* Step 2. Execute cmd */
		switch((int)param.cmd_type)
		{
			case BE_SKEEPER_DBG_CMD_INIT: 
			{
				ret = VIDEO_SKeeperInit();
			}
			break;

			case BE_SKEEPER_DBG_CMD_UNINIT: 
			{
				ret = VIDEO_SKeeperUnInit();
			}
			break;
	
			case BE_SKEEPER_DBG_CMD_INIT_HISTORY:
			{
				ret = VIDEO_SKeeper_InitHistoryLog((LX_VP_ST_KEEPER_MODULE_TYPE_T)param.module_type, (LX_VP_ST_KEEPER_HISTORY_TYPE_T)param.history_type);
			}
			break;

			case BE_SKEEPER_DBG_CMD_UNINIT_HISTORY:
			{
				ret = VIDEO_SKeeper_InitHistoryLog((LX_VP_ST_KEEPER_MODULE_TYPE_T)param.module_type, (LX_VP_ST_KEEPER_HISTORY_TYPE_T)param.history_type);
			}
			break;
			
			case BE_SKEEPER_DBG_CMD_CHECK_STATUS: 
			{
				ret = __VIDEO_SKeeper_DBG_CheckStatus();
			}
			break;
			
			case BE_SKEEPER_DBG_CMD_PRINT_MODULE_LIST: 
			{
				ret = __VIDEO_SKeeper_DBG_PrintModuleList();
			}
			break;

			case BE_SKEEPER_DBG_CMD_PRINT_STATUSINDEX:
			{
				ret = __VIDEO_SKeeper_DBG_PrintIndexLog((LX_VP_ST_KEEPER_MODULE_TYPE_T)param.module_type);
			}
			break;

			case BE_SKEEPER_DBG_CMD_PRINTLOG: 
			{
				ret = __VIDEO_SKeeper_DBG_PrintHistoryLog((LX_VP_ST_KEEPER_MODULE_TYPE_T)param.module_type, (LX_VP_ST_KEEPER_HISTORY_TYPE_T)param.history_type);
			}
			break;

			case BE_SKEEPER_DBG_CMD_CHANGE_HISTORY_TIME_UNIT:
			{
				ST_KEEPER_PRINT("[%s:%d] Current time shift length [%d], Requested to [%d]  \n",__F__, __L__, _gSKeeperTimeStampShiftUnit, param.etcData);

				ret = __VIDEO_SKeeper_DBG_Set_TimeStampShiftLength((int)param.etcData);
				
				ST_KEEPER_PRINT("[%s:%d] Time shift length changed to [%d] \n",__F__, __L__, _gSKeeperTimeStampShiftUnit);
			}
			break;

			case BE_SKEEPER_DBG_CMD_CHANGE_HISTORY_LOG_STATUS:
			{
				/* Step 1. Check DB for segementation fault */
				ST_KEEPER_CHECK_DATABASE();
				
				/* Step 2. Check input range */
				if(((int)param.module_type < 0) || ((int)param.module_type >=  LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM)){
					ST_KEEPER_PRINT("[%s:%d] Module type out of range [%d] \n",__F__, __L__, (int)param.module_type);
					break;
				}

				if(((int)param.history_type <= LX_VP_ST_KEEPER_HISTORY_NONE) || ((int)param.history_type >= LX_VP_ST_KEEPER_HISTORY_MAXNUM)){
					ST_KEEPER_PRINT("[%s:%d] History type out of range [%d] \n",__F__, __L__, (int)param.history_type);
					break;
				}
				
				/* Step 3. Read Before Set */
				if((int)param.history_type == LX_VP_ST_KEEPER_HISTORY_IO){
					ST_KEEPER_PRINT("[%s:%d] Current status for Module[%d] HistType[%d] is [%d].\n",\
							__F__, __L__, (int)param.module_type, (int)param.history_type,\
							gSKeeperDataBase->io_log[(int)param.module_type].status);
					
					gSKeeperDataBase->io_log[(int)param.module_type].status = (int)param.etcData;

					ST_KEEPER_PRINT("[%s:%d] Status changed for Module[%d] HistType[%d] to  [%d].\n",\
							__F__, __L__, (int)param.module_type, (int)param.history_type,\
							gSKeeperDataBase->io_log[(int)param.module_type].status);
					ret = RET_OK;
				}
				else if((int)param.history_type == LX_VP_ST_KEEPER_HISTORY_DISPATCH){
					ST_KEEPER_PRINT("[%s:%d] Current status for Module[%d] HistType[%d] is [%d].\n",\
							__F__, __L__, (int)param.module_type, (int)param.history_type,\
							gSKeeperDataBase->dispatch_log[(int)param.module_type].status);
					
					gSKeeperDataBase->dispatch_log[(int)param.module_type].status = (int)param.etcData;

					ST_KEEPER_PRINT("[%s:%d] Status changed for Module[%d] HistType[%d] to  [%d].\n",\
							__F__, __L__, (int)param.module_type, (int)param.history_type,\
							gSKeeperDataBase->dispatch_log[(int)param.module_type].status);
					ret = RET_OK;
				}
				else{
					ST_KEEPER_PRINT("[%s:%d] Debugger for History Type[%d] is missing...\n",__F__, __L__, (int)param.history_type); 
				}
			}
			break;

			case BE_SKEEPER_DBG_CMD_CHANGE_MODULE_STATUS:
			{
				/* Step 1. Check DB for segementation fault */
				ST_KEEPER_CHECK_DATABASE();
				
				/* Step 2. Check input range */
				if(((int)param.module_type < 0) || ((int)param.module_type >=  LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM)){
					ST_KEEPER_PRINT("[%s:%d] Module type out of range [%d] \n",__F__, __L__, (int)param.module_type);
					break;
				}
		
				/* Step 3. Read & set */
				ST_KEEPER_PRINT("[%s:%d] Current status for Module[%d] is [%d].\n",\
						__F__, __L__, (int)param.module_type,\
						gSKeeperDataBase->status[(int)param.module_type]);
					
				gSKeeperDataBase->status[(int)param.module_type] = (int)param.etcData;

				ST_KEEPER_PRINT("[%s:%d] Status changed for Module[%d] to [%d].\n",\
						__F__, __L__, (int)param.module_type,\
						gSKeeperDataBase->status[(int)param.module_type]);

				ret = RET_OK;
			}
			break;

			case BE_SKEEPER_DBG_CMD_ENABLE_DBG_PRINT:
			{
				if(param.etcData >= 2) __VIDEO_SKeeper_DBG_EnableDBGLogPrint(1);
				if(param.etcData >= 1) __VIDEO_SKeeper_DBG_EnableDBGPrint(1);
				else __VIDEO_SKeeper_DBG_EnableDBGPrint(0);

				ret = RET_OK;
			}
			break;
			default:
			{
				ST_KEEPER_PRINT("[%s:%d] Within CMD range but Unknown CMD Type. cmd [%d].\n", __F__, __L__, param.cmd_type);
			}
			break;
		}

	}while(0);

	return ret;
}


int __VIDEO_SKeeper_DBG_CheckStatus(void)
{
	int ret = RET_OK;
	int i;

	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/*               12345678901234567890123456789012345678901234567890 */
		ST_KEEPER_PRINT("\n");	
		ST_KEEPER_PRINT("--------------------------------------------------\n");
		ST_KEEPER_PRINT("            [VIDEO STATUS KEEPER STATUS]          \n");
		ST_KEEPER_PRINT("--------------------------------------------------\n");
		for(i=0;i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM;i++) {
		ST_KEEPER_PRINT("\n");	
		ST_KEEPER_PRINT("  Current values for Module[%d] : %s\n", i, gSKeeperModuleName[i].index);
		ST_KEEPER_PRINT("    -> Status[%d]                                 \n", gSKeeperDataBase->status[i]);
		ST_KEEPER_PRINT("    -> TABLE : Index count[%d]/pRsrTable[0x%08x]\n",\
						 				gSKeeperDataBase->table[i].rsrIndexCount,\
										(unsigned int)gSKeeperDataBase->table[i].pRsrIndex);
		/*               12345678901234567890123456789012345678901234567890 */
		ST_KEEPER_PRINT("    -> IO_LOG :pStartAddr[0x%08x]/pEndAddr[0x%08x]\n",\
										(unsigned int)gSKeeperDataBase->io_log[i].pStartAddress,\
										(unsigned int)gSKeeperDataBase->io_log[i].pEndAddress);
		ST_KEEPER_PRINT("               Head[0x%x]/status[%d]\n",\
										gSKeeperDataBase->io_log[i].Head,\
										gSKeeperDataBase->io_log[i].status);
		ST_KEEPER_PRINT("    -> DP_LOG :pStartAddr[0x%08x]/pEndAddr[0x%08x]\n",\
										(unsigned int)gSKeeperDataBase->dispatch_log[i].pStartAddress,\
										(unsigned int)gSKeeperDataBase->dispatch_log[i].pEndAddress);
		ST_KEEPER_PRINT("               Head[0x%x]/status[%d]\n",\
										(unsigned int)gSKeeperDataBase->dispatch_log[i].Head,\
										gSKeeperDataBase->dispatch_log[i].status);	
		}
		ST_KEEPER_PRINT("--------------------------------------------------\n");
	} while(0);

	return ret;
}

int __VIDEO_SKeeper_DBG_PrintModuleList(void)
{
	int ret = RET_OK;
	int i;


	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();
		ST_KEEPER_PRINT("\n");	
		ST_KEEPER_PRINT("--------------------------------------------------\n");
		ST_KEEPER_PRINT("           [VIDEO STATUS KEEPER MODULES]          \n");
		ST_KEEPER_PRINT("--------------------------------------------------\n");
		for(i=0;i<LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM;i++){
			ST_KEEPER_PRINT(" Module ID[%d] : %s \n", i, gSKeeperModuleName[i].index);
		}
		ST_KEEPER_PRINT("--------------------------------------------------\n");
	}while(0);

	return ret;
}

int __VIDEO_SKeeper_DBG_PrintIndexLog(LX_VP_ST_KEEPER_MODULE_TYPE_T module)
{
	int ret = RET_ERROR;
	int i;
	LX_VP_ST_KEEPER_INDEX_T *pIndex;

	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();

		/* Step 0. Check module type argument */
		if(module >= LX_VP_ST_KEEPER_MODULE_TYPE_MAXNUM) {
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown module type[%d] \n", __F__, __L__, module);
			break;
		}

		/* Step 2. Check resource manager status for called module */
		if(gSKeeperDataBase->status[module] <= LX_VP_ST_KEEPER_MODULE_STATUS_UN_REGISTERED){
			ST_KEEPER_PRINT("[%s:%d] Can't access Index data due to status[%d]! \n", __F__, __L__, gSKeeperDataBase->status[module]);
			break;
		}

		/* Step 3. Get Pointer */
		pIndex = gSKeeperDataBase->table[module].pRsrIndex;

		/* Step 4. Check resource pointer for segmentation fault */
		if( pIndex == NULL){
			ST_KEEPER_PRINT("[%s:%d] Error! Index list pointer is NULL\n", __F__, __L__);
			gSKeeperDataBase->status[module] = LX_VP_ST_KEEPER_MODULE_STATUS_ERROR;
			break;
		}
	
		/* Step 5. Print Index */
		ST_KEEPER_PRINT("\n");	
		ST_KEEPER_PRINT("----------------------------------------------------------\n");
		ST_KEEPER_PRINT("[VIDEO STATUS KEEPER : STATUS INDEX] Current Time : 0x%08x\n", (unsigned int)(jiffies>>_gSKeeperTimeStampShiftUnit));
		ST_KEEPER_PRINT("----------------------------------------------------------\n");
		for(i=0;i<gSKeeperDataBase->table[module].rsrIndexCount;i++){
			ST_KEEPER_PRINT("[#%2d]. [%35s] => [0x%08x]. Modifed by [0x%08x] at [0x%08x] \n",\
								i, pIndex[i].index, pIndex[i].status, pIndex[i].modifier, \
								pIndex[i].last_modified_time);
		}
		ST_KEEPER_PRINT("---------------------------------------------------------\n");

		ret = RET_OK;
	}while(0);

	return ret;
}

int __VIDEO_SKeeper_DBG_PrintHistoryLog( LX_VP_ST_KEEPER_MODULE_TYPE_T module, LX_VP_ST_KEEPER_HISTORY_TYPE_T type)
{
	int ret = RET_ERROR;
	unsigned int draw_size;
	LX_VP_ST_KEEPER_HISTORY_T historyDatabase;


	do{
		ST_KEEPER_CHECK_DATABASE();
		ST_KEEPER_CHECK_STATUS();
		ST_KEEPER_PRINT("\n");	
		ST_KEEPER_PRINT("--------------------------------------------------\n");
		ST_KEEPER_PRINT("  [VIDEO STATUS MODULE(%d) HISTORY LOG(type:%d)]  \n", module, type);
		ST_KEEPER_PRINT("--------------------------------------------------\n");
	
		/* Step 1. Check type whether it is valid or not */
		if((type <= LX_VP_ST_KEEPER_HISTORY_NONE) || (type >= LX_VP_ST_KEEPER_HISTORY_MAXNUM)){
			ST_KEEPER_PRINT("[%s:%d] Error! Unknown history log type[%d].\n",__F__, __L__, type);
			break;
		}

		/* Step 2. Get History DataBase */
		if(type == LX_VP_ST_KEEPER_HISTORY_IO)
		{
			memcpy((void *)&historyDatabase, (void *)&gSKeeperDataBase->io_log[module], sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}
		else if(type == LX_VP_ST_KEEPER_HISTORY_DISPATCH)
		{
			memcpy((void *)&historyDatabase, (void *)&gSKeeperDataBase->dispatch_log[module], sizeof(LX_VP_ST_KEEPER_HISTORY_T));
		}


		/* Step 3. Check pointer of history log */
		if((historyDatabase.pStartAddress == NULL)||(historyDatabase.pEndAddress == NULL)){
			ST_KEEPER_PRINT("[%s:%d] Error! One of hisotry pointer was NULL. pS[0x%x], pE[0x%x]\n",\
								__F__, __L__, (unsigned int)historyDatabase.pStartAddress, (unsigned int)historyDatabase.pEndAddress);
			break;
		}

		/* Step 4. Draw History Log */
		ST_KEEPER_PRINT("\t[   TIME   ] | [   PID    ] [ IOCTL NR  ] [   SIZE   ] => [    PARAMETER   ]\n");
		draw_size = (unsigned int)((unsigned int)historyDatabase.pEndAddress - (unsigned int)historyDatabase.pStartAddress);
		ret = __VIDEO_SKeeper_DBG_DrawHistoryLog(historyDatabase.pStartAddress, draw_size);
		if(ret<0) ST_KEEPER_PRINT("[%s:%d] Error! Failed to draw History log table. Ret[%d]\n", __F__, __L__, ret);
		ST_KEEPER_PRINT("\n--------------------------------------------------\n");
	}while(0);

	return ret;
}


int __VIDEO_SKeeper_DBG_DrawHistoryLog(unsigned char *log, unsigned int size)
{
	int ret = RET_ERROR;
	int i,y,tail_location, data_location;
	LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T history_data;
	unsigned int found_valid_data;
	unsigned int first_header_data;

	int same = 0;

	do{
		/* Step 1. Find valid data strip */
		found_valid_data  = 0;
		first_header_data = 0xFFFFFFFF;

		for(i=0;i<(size-4);i++){
			if(log[i] == LX_VP_ST_KEEPER_HISTORY_START_CODE_0) same = 1;
			else same = 0;
			ST_KEEPER_LOG_PRINT(" log[%d][0x%x] differ to LX_VP_ST_KEEPER_HISTORY_START_CODE_0[0x%x] = %s\n", i, log[i], LX_VP_ST_KEEPER_HISTORY_START_CODE, same ? "SAME":"DIFFER");
			if((log[i] == LX_VP_ST_KEEPER_HISTORY_START_CODE_0)&&(log[i+1] == LX_VP_ST_KEEPER_HISTORY_START_CODE_1)\
					&& (log[i+2] == LX_VP_ST_KEEPER_HISTORY_START_CODE_2)&&(log[i+3] == LX_VP_ST_KEEPER_HISTORY_START_CODE_3)){
				if(first_header_data == 0xFFFFFFFF){
					first_header_data = i;
				}

				if( (i + sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T)) < size){
					memcpy((void *)&history_data, (void *)(&log[i]), sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int));
					
					if(( i + sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) + history_data.size ) <= size){
						tail_location = i + sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) + history_data.size - sizeof(unsigned int);
						data_location = 0;

						if((log[tail_location] == LX_VP_ST_KEEPER_HISTORY_END_CODE_0)&&(log[tail_location+1] == LX_VP_ST_KEEPER_HISTORY_END_CODE_1)\
								&&(log[tail_location+2] == LX_VP_ST_KEEPER_HISTORY_END_CODE_2)&&(log[tail_location+3] == LX_VP_ST_KEEPER_HISTORY_END_CODE_3)){
							found_valid_data++;
							ST_KEEPER_PRINT("\t[0x%08x] | [0x%08x] [ 0x%08x] [0x%08x] =>  ", \
									history_data.time, history_data.pid,  _IOC_NR(history_data.ioctl) ,history_data.size);
							data_location = i + sizeof(LX_VP_ST_KEEPER_HISTORY_DATA_TYPE_T) - sizeof(unsigned int);
							for(y=0;y<history_data.size;y++) ST_KEEPER_PRINT("%0x ", log[data_location + y]);					
							ST_KEEPER_PRINT("\n");
							i = data_location + history_data.size + sizeof(unsigned int); 
						}
						else{
							ST_KEEPER_LOG_PRINT("[%s:%d] Warning! There was no TAIL. location[%d] [0x%x][0x%x][0x%x][0x%x]\n", __F__, __L__,\
									tail_location, log[tail_location], log[tail_location+1], log[tail_location+2], log[tail_location+3]);
								ST_KEEPER_LOG_PRINT("[%s:%d] Answer to TAIL. [0x%x][0x%x][0x%x][0x%x]\n", __F__, __L__,\
									LX_VP_ST_KEEPER_HISTORY_END_CODE_0, LX_VP_ST_KEEPER_HISTORY_END_CODE_1,LX_VP_ST_KEEPER_HISTORY_END_CODE_2,LX_VP_ST_KEEPER_HISTORY_END_CODE_3 );
						}
					}
					else{
						ST_KEEPER_LOG_PRINT("[%s:%d] Tail area reached over size limit. size[%d] i[%d] his_size[%d]\n", __F__, __L__, size, i, history_data.size);
						for(y=i;y<(size-4);y++) ST_KEEPER_PRINT("0x%02x ", log[y]);
						ST_KEEPER_LOG_PRINT("[%s:%d] Wrap around ...\n", __F__, __L__);
						for(y=0;y<first_header_data;y++) ST_KEEPER_PRINT("0x%02x ", log[y]);
						i = size - 4; // Get out of for-loop
					}
				}
				else{
					ST_KEEPER_LOG_PRINT("[%s:%d] Searching area reached over size limit. size[%d] i[%d]\n", __F__, __L__, size, i);
					for(y=i;y<(size-4);y++) ST_KEEPER_PRINT("0x%02x ", log[y]);
					i = size - 4; // Get out of for-loop
				}
			}
			else{
				ST_KEEPER_LOG_PRINT("[%s:%d] No Header on i[%d] = [0x%x].\n", __F__, __L__, i, log[i]);
			}
		}
		ret = RET_OK;
	} while(0);

	return ret;
}

int __VIDEO_SKeeper_DBG_Set_TimeStampShiftLength(int shiftAmount)
{
	int ret = RET_ERROR;

	do{
		ST_KEEPER_CHECK_DATABASE();
	
		/* Step 1. Check boundary */
		if((shiftAmount<0)||(shiftAmount>31)){
			ST_KEEPER_PRINT("[%s:%d] Error! ShiftAmount value error. ([%d]<0)||([%d]>31) == 1.\n", __F__, __L__, shiftAmount, shiftAmount);
			break;
		}

		_gSKeeperTimeStampShiftUnit	 = shiftAmount;

		ret = RET_OK;
	}while(0);

	return ret;
}

