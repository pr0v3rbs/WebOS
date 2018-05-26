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
 *	hardware abstraction layer for ipc.
 *
 *	author		
 *	version		1.0
 *	date		
 *	note		
 *
 *	@addtogroup 
 *	@{
 */

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/
#define ENABLE_FW_IPC_LOG

/*------------------------------------------------------------------------------
	File Inclusions
------------------------------------------------------------------------------*/
#include <linux/slab.h>
#include "os_util.h"
#include "base_device.h"

#include "de_ver_def.h"
#include "de_def.h"
#include "ipc_hal.h"
#include "ipc_phys_hal.h"

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define CHECK_REG_NULL(fp) {if(fp == NULL || g_chip_id < 0)\
							{DE_ERROR("%s is null\n",#fp); \
							return RET_ERROR;}}
#define GET_ID() g_chip_id

#define IPC_TRACE(format, args...)\
	if(g_ipc_trace==1) DE_PRINT(format, ##args);\
	else if(g_ipc_trace==2) printk(format, ##args);

#define IPC_DUMP(format, args...)\
	if(g_ipc_trace==2) printk(format, ##args)

#define MCU_ID(a) ((a >> 16) & 0xF)
#define STR_IPC_DST(d, s)\
	switch(d)\
	{\
		case IPC_DST_DE:\
			s = "DE";break;\
		case IPC_DST_FMT3D:\
			s = "FMT3D";break;\
		case IPC_DST_DENC:\
			s = "DENC";break;\
		case IPC_DST_PQL:\
			s = "PQL";break;\
		case IPC_DST_TOP:\
			s = "TOP";break;\
		case IPC_DST_H3D:\
			s = "H3D";break;\
		case IPC_DST_FRC:\
			s = "FRC";break;\
		case IPC_DST_SRE:\
			s = "SRE";break;\
		case IPC_DST_DPPA:\
			s = "DPPA";break;\
		case IPC_DST_DPPB:\
			s = "DPPB";break;\
		case IPC_DST_PWM:\
			s = "PWM";break;\
		case IPC_DST_DBG:\
			s = "DBG";break;\
		default:\
			s = "NULL";break;\
	}

/*------------------------------------------------------------------------------
	Type Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Function Prototype Declarations
------------------------------------------------------------------------------*/
extern int DE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
extern int DE_KIPC_GetData(UINT32 mcu_id, void *pMsg, UINT32 msgCnt);
extern int DE_KIPC_SetMem(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
#ifdef INCLUDE_KDRV_BE
extern int BE_MCU_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
extern int BE_MCU_GetData(UINT32 mcu_id, void *pMsg, UINT32 msgCnt);
#endif
/*------------------------------------------------------------------------------
	External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Functions  
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Variables
------------------------------------------------------------------------------*/
struct ipc_client g_ipc_client[MCU_ID_MAX] = 
{
	{ // MCU_ID_0
		DE_KIPC_SetData,
		DE_KIPC_GetData,
		DE_KIPC_SetMem,
	},
	{ // MCU_ID_1
		NULL, NULL, NULL
	},
	{ // MCU_ID_2
		NULL, NULL, NULL
	},
	{ // MCU_ID_3
		NULL, NULL, NULL
	},
	{ // MCU_ID_EXT_0
		NULL, NULL, NULL
	},
	{ // MCU_ID_EXT_1
		NULL, NULL, NULL
	},

};

static struct ipc_handle *gp_last_ipc_handle = NULL;
static char g_ipc_ready[MCU_ID_MAX];
static struct ipc_log g_ipc_log;
static UINT32 g_ipc_trace = 0;

/*------------------------------------------------------------------------------
	Static Function Prototypes Declarations
------------------------------------------------------------------------------*/
static int ipc_hal_write(UINT32 cmd, void *msg, UINT32 cnt);
static int ipc_hal_read(UINT32 mcu_id, void *msg, UINT32 cnt);

/*------------------------------------------------------------------------------
	Static Variables
------------------------------------------------------------------------------*/
OS_SEM_T	_g_ipc_hal_sem;

/*==============================================================================
	Implementation Group
==============================================================================*/

int ipc_hal_init(void)
{
	int ret = RET_OK;

	OS_InitMutex(&_g_ipc_hal_sem, OS_SEM_ATTR_DEFAULT);

	ret = ipc_phys_hal_init();
	if(ret) return RET_ERROR;

	memset(g_ipc_client, 0, sizeof(struct ipc_client) * MCU_ID_MAX);
	memset(g_ipc_ready,  0, sizeof(char) * MCU_ID_MAX);
	memset(g_ipc_log.cmd, 0, sizeof(UINT32) * MAX_IPC_LOG);
	memset(g_ipc_log.buf, 0, sizeof(char*) * MAX_IPC_LOG);
	g_ipc_log.idx = 0;

#ifdef USE_KDRV_CODES_FOR_H15
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) {
		g_ipc_client[MCU_ID_0].write = ipc_hal_write;
		g_ipc_client[MCU_ID_0].read  = ipc_hal_read;
		g_ipc_client[MCU_ID_0].wrmem = DE_KIPC_SetMem;

		g_ipc_client[MCU_ID_1].write = ipc_hal_write;
		g_ipc_client[MCU_ID_1].read  = ipc_hal_read;		
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) {
		g_ipc_client[MCU_ID_0].write = ipc_hal_write;
		g_ipc_client[MCU_ID_0].read  = ipc_hal_read;
			
#ifdef INCLUDE_KDRV_BE
		g_ipc_client[MCU_ID_1].write = BE_MCU_SetData;
		g_ipc_client[MCU_ID_1].read  = BE_MCU_GetData;
#endif
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(M14,B0) ) 
		{
			g_ipc_client[MCU_ID_0].write = ipc_hal_write;
			g_ipc_client[MCU_ID_0].read  = ipc_hal_read;
			g_ipc_client[MCU_ID_0].wrmem = DE_KIPC_SetMem;

			g_ipc_client[MCU_ID_1].write = ipc_hal_write;
			g_ipc_client[MCU_ID_1].read  = ipc_hal_read;
		} else
		{
			g_ipc_client[MCU_ID_0].write = ipc_hal_write;
			g_ipc_client[MCU_ID_0].read  = ipc_hal_read;

#ifdef INCLUDE_KDRV_BE
			g_ipc_client[MCU_ID_1].write = BE_MCU_SetData;
			g_ipc_client[MCU_ID_1].read  = BE_MCU_GetData;
#endif
		}
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) {
		g_ipc_client[MCU_ID_0].write = ipc_hal_write;
		g_ipc_client[MCU_ID_0].read  = ipc_hal_read;
			
#ifdef INCLUDE_KDRV_BE
		g_ipc_client[MCU_ID_1].write = BE_MCU_SetData;
		g_ipc_client[MCU_ID_1].read  = BE_MCU_GetData;
#endif
	} else
#endif
	{
		DE_ERROR("undefined chip\n");
		ret = RET_ERROR;
	}

	return ret;
}

int ipc_hal_cleanup(void)
{
	int ret = RET_OK;
	int i;

	for(i=0;i<MAX_IPC_LOG;i++)
	{
		if(g_ipc_log.buf[i] != NULL)
			kfree(g_ipc_log.buf[i]);
	}

	DE_PRINT("done.\n");

	return ret;
}

struct ipc_handle *ipc_hal_open(char *name, MCU_ID_TYPE mcu_id)
{
	struct ipc_handle *ipc_handle = (struct ipc_handle *)kmalloc( \
										sizeof(struct ipc_handle),\
										GFP_KERNEL);
	int i;

	if(ipc_handle == NULL)
	{
		DE_ERROR("ipc_handle kmalloc fail\n");
		return NULL;
	}

	if(	g_ipc_client[mcu_id].write != NULL &&\
		g_ipc_client[mcu_id].read  != NULL )
	{
		ipc_handle->m_ipc_client = &g_ipc_client[mcu_id];
		memset(ipc_handle->dev_name, 0, MAX_DEV_NAME);
		for(i=0;i<(MAX_DEV_NAME-1);i++)
		{
			ipc_handle->dev_name[i] = name[i];
			if(name[i] == 0) break;
		}
	}
	else
	{
		DE_ERROR("client function is null  (write:0x%08x, read:0x%08x\n",\
				(UINT32)g_ipc_client[mcu_id].write,\
				(UINT32)g_ipc_client[mcu_id].read);
		kfree(ipc_handle);
		return NULL;
	}

	ipc_handle->p_ready = &g_ipc_ready[mcu_id];
	ipc_handle->mcu_id = mcu_id;
	ipc_handle->prev   = (void *)gp_last_ipc_handle;
	gp_last_ipc_handle = ipc_handle;

	DE_PRINT("ipc_hal_open is done for %s , mcu_id %d\n", name, mcu_id);

	return ipc_handle;
}

int ipc_hal_close(struct ipc_handle *ipc_handle)
{
	if(!ipc_handle)
		return RET_ERROR;

	kfree(ipc_handle);

	DE_PRINT("ipc_close done.\n");
	
	return RET_OK;
}

int ipc_hal_list(void)
{
	struct ipc_handle *h = gp_last_ipc_handle;

	DE_PRINT("List of mcu ipc handler  | fw ready[%d/%d]\n", \
									g_ipc_ready[0], g_ipc_ready[1]);

	if(h == NULL) 
	{
		DE_PRINT("No list for mcu ipc\n");
		return RET_OK;
	}

	do {
		DE_PRINT("\t%s\t:  mcu %d\th->0x%08x\n", h->dev_name,\
                                                h->mcu_id,  \
                                                (UINT32)h);
		if(h->prev == NULL) break;
		h = h->prev;
	} while(1);

	return RET_OK;
}

int ipc_hal_start(UINT32 mcu_id, char p)
{
	g_ipc_ready[mcu_id] = p;
	return RET_OK;
}

int ipc_hal_trace(UINT32 param)
{
	int i = 0, j = 0, k = 0, m = 0;
	char *str;

	switch(param)
	{
		case 0:
		case 1:
		case 2:
			g_ipc_trace = param;
			break;
		case 4:
			DE_PRINT("Destination of mcu ipc\n");
			for(i=IPC_DST_DE;i<IPC_DST_MAX;i++)
			{
				STR_IPC_DST(i, str);
				DE_PRINT("  %s	[%d]\n", str, i);
			}
			break;
		case 3:
			{
				int ipc_total_size = 0;
				printk("\n\n DE IPC HAL log (max %d, The last row is the latest ipc.)\n\n", MAX_IPC_LOG);
				for(i=0;i<MAX_IPC_LOG;i++)
				{
					k = i + g_ipc_log.idx;
					if(k >= MAX_IPC_LOG)
						k -= MAX_IPC_LOG;
					if( g_ipc_log.cmd[k] == 0 || \
						g_ipc_log.buf[k] == NULL) continue;
					STR_IPC_DST((g_ipc_log.cmd[k] >> DST_OFFSET) & 0xF, str);
					printk("\x1b[1;32m %03d ) %s MCU[%d] CMD[0x%08X] SIZE[%d] \x1b[0m \n",\
							k, str, MCU_ID(g_ipc_log.cmd[k]), \
							g_ipc_log.cmd[k], g_ipc_log.cnt[k]);
					printk("     ");
					for(j=0;j<g_ipc_log.cnt[k];j++)
					{
						printk("%02X ", g_ipc_log.buf[k][j]);
						if(m++ == 3) {printk(" ");m=0;}
					}
					m = 0;
					printk("\n");
					ipc_total_size += sizeof(UINT32);  /* cmd */
					ipc_total_size += g_ipc_log.cnt[k];/* cnt */
					ipc_total_size += sizeof(char*) ;  /* param-pointer */
				}
				ipc_total_size += sizeof(UINT32)    ;  /* index */
				printk("Total size of ipc_log  %d  Bytes.\n", ipc_total_size);

			}
			break;
		default:
			break;
	}
	return RET_OK;
}
 
#ifdef ENABLE_FW_IPC_LOG
static int ipc_hal_log(UINT32 cmd, void *msg, UINT32 cnt)
{
	int ret = RET_OK;
	UINT32 *p_idx = &g_ipc_log.idx;
	char *buf;

	do {
		ret = OS_LockMutex(&_g_ipc_hal_sem);
		if(ret) break;
		//OS_GetCurrentTicks(&t.sec, &t.msec, &t.usec);
		if(g_ipc_log.buf[*p_idx] != NULL) kfree(g_ipc_log.buf[*p_idx]);
		buf = (char *)kmalloc(cnt, GFP_KERNEL);
		memcpy(buf, (char*)msg, cnt);
		g_ipc_log.cmd[*p_idx] = cmd;
		g_ipc_log.cnt[*p_idx] = cnt;
		g_ipc_log.buf[*p_idx] = buf;
		if(++*p_idx >= MAX_IPC_LOG) *p_idx = 0;
		OS_UnlockMutex(&_g_ipc_hal_sem);
	} while(0);

	return ret;
}
#endif
static int ipc_hal_write(UINT32 cmd, void *msg, UINT32 cnt)
{
	int i, m=0;
	char *str;
	STR_IPC_DST((cmd >> DST_OFFSET) & 0xF, str);
	IPC_TRACE("WR: mcu [%d]  %s 	cmd[0x%08X] size[%d]\n", \
			MCU_ID(cmd), str, cmd, cnt);
	if(g_ipc_trace == 2)
	{
		char *pBuf = (char *)msg;
		IPC_DUMP("packet]  ");
		for(i=0;i<cnt;i++)
		{
			IPC_DUMP("%02X ", pBuf[i]);
			if(m++ == 3) {IPC_DUMP(" ");m=0;}
		}
		m = 0;
		IPC_DUMP("\n\n");
	}
#ifdef ENABLE_FW_IPC_LOG
	ipc_hal_log(cmd, msg, cnt);
#endif
	return DE_KIPC_SetData(cmd, msg, cnt);
}

static int ipc_hal_read(UINT32 mcu_id, void *msg, UINT32 cnt)
{
	int i, m=0;
	IPC_TRACE("RD: mcu [%d] size[%d]\n", mcu_id, cnt);
	if(g_ipc_trace == 2)
	{
		IPC_DUMP("packet]  ");
		for(i=0;i<cnt;i++)
		{
			IPC_DUMP("%02X ", *(char *)msg);
			if(m++ == 3) {IPC_DUMP(" ");m=0;}
		}
		m = 0;
		IPC_DUMP("\n\n");
	}
	return DE_KIPC_GetData(mcu_id, msg, cnt);
}

