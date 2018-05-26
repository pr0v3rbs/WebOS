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
 *	resource manager implementation for de device.
 *
 *	author		
 *	version		1.0
 *	date		
 *	note		
 *
 *	@addtogroup 
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define TEST_IPC_LOGICAL_LAYER

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/slab.h>
#include "os_util.h"
#include "base_device.h"
#include "resource_mgr.h"
#include "de_def.h"
#include "de_model.h"
#include "de_io.h"
#include "de_cfg.h"
#include "de_sync.h"
#include "de_mem.h"
#include "hal/ipc_hal.h"
#include "mcu/de_dbi.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define COPY_FROM_USER(dst,size) { \
	ret = copy_from_user(&dst, (void __user *)arg, sizeof(size));}
#define COPY_TO_USER(src,size)   { \
	ret = copy_to_user(  (void __user *)arg, &src, sizeof(size));\
	if(ret) break;}
#define CHECK_PTR(ptr)          \
	if(ptr==NULL){\
		DE_ERROR("===)>   ptr is NULL. at %s line[%d]\n", __F__, __L__); \
		ret = RET_ERROR;break;}
#define EXIT_ERROR() { \
	DE_ERROR("%s [%d] : exit by error\n", __F__, __L__);return RET_ERROR;}
#define CALL_IO(func) { \
	if(1)func(arg); \
	ret = __push_ioc_buf(cmd, #func, arg);\
	__pop_ioc_buf(0, cmd); }
#define RESUME_IO(func, p) { \
	arg = (unsigned long)p;  \
	if(1){	ret = func(arg); \
			if(ret)break;}\
	}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions  
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _init_manager(void);
static int _uninit_manager(void);
static int _create_device(  int     index,        \
							char    *name,        \
							BOOLEAN active,       \
							LX_DE_INPUT_SRC_T src);
static int _destroy_device(int index);

static int _ioctl_system(unsigned int cmd, unsigned long arg);
static int _ioctl_source(unsigned int cmd, unsigned long arg);
static int _ioctl_window(unsigned int cmd, unsigned long arg);
static int _ioctl_operation(unsigned int cmd, unsigned long arg);
static int _ioctl_control(unsigned int cmd, unsigned long arg);
static int _ioctl_etc(unsigned int cmd, unsigned long arg);
static int _init_ioc_buf(BOOLEAN sel);
static int _thread_handle(UINT32 *timeout);

static int __operation(		DISP_DEV_TYPE  dev_id,   \
							OPERATION_TYPE opr_type, \
							EXECUTE_CMD    cmd,      \
							void           *param);
static int __control(		DISP_DEV_TYPE    dev_id, \
							DISP_DEV_CONTROL ctrl,   \
							EXECUTE_CMD      cmd,    \
							void             *param);
static int __operation_map( OPERATION_TYPE *p_opr,   \
							OPERATION_TYPE  cur_opr, \
							EXECUTE_CMD     cmd);
static int __source_win_port_map(LX_DE_INPUT_SOURCE_T param);
static int __source_property(LX_DE_CVI_SRC_TYPE_T param);
static int __source_mirror(LX_DE_SRC_MIRROR_T param);
static int __view_device(void);
static int __view_ioctl(void);
static int __view_misc(void);
static int __push_ioc_buf(unsigned int cmd, char *c, unsigned long arg);
static int __pop_ioc_buf(BOOLEAN dump_all, unsigned int cmd);
static int __test_ipc(UINT32 reset);
static int _test_execute4mcu(void);
static int __test_cmd2mcu(int argc, char* cmd, char* param0,   \
		char* param1, char* param2, char* param3, char* param4,\
		char* param5, char* param6, char* param7);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DISPLAY_DEV_T *g_dev[DISP_DEV_MAX];
static SOURCE_INFO_T *g_src[DISP_DEV_MAX];
static SYSTEM_INFO_T *g_sys;
static WINDOW_INFO_T *g_win;
static CMD_PARAM_BUF_T ioc_buf[DE_IOC_MAXNR + 1];

/*========================================================================================
	Implementation Group
========================================================================================*/
int resource_mgr_init(void)
{
	int ret = RET_OK;

	de_mem_init();
	DE_IO_IPC_Init(0);

	_init_manager();
	_init_ioc_buf(TRUE);

	_create_device(DISP_DEV_MAIN,  "main",    TRUE,  LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_SUB_1, "sub1",    FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_SUB_2, "sub2",    FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_SUB_3, "sub3",    FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_VTS,   "vcr",     FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_VTV,   "dvr",     FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_VTM,   "vtg",     FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_VTC,   "capture", FALSE, LX_DE_IN_SRC_NONE);
	_create_device(DISP_DEV_GRAB,  "grab",    FALSE, LX_DE_IN_SRC_NONE);

	return ret;

}

int resource_mgr_cleanup(void)
{
	int ret = RET_OK;

	de_sync_free();
	DE_IO_IPC_Cleanup(0);

	_destroy_device(DISP_DEV_MAX);
	_uninit_manager();
	_init_ioc_buf(FALSE); //uninit

	return ret;
}

DISPLAY_DEV_T *susp_dev[DISP_DEV_MAX];
SOURCE_INFO_T *susp_src[DISP_DEV_MAX];
SYSTEM_INFO_T *susp_sys;
WINDOW_INFO_T *susp_win;

int resource_mgr_suspend(void)
{
	int ret = RET_OK;
	int i;

	do {
		// kmalloc area is stored to NVMEM. 
		// So, data backup is not needed.
		// (step1/step2 :  susp_dev, susp_src, susp_sys, susp_win)
		//
		// step 1. alloc memory for suspending last state of each device
		susp_sys = (SYSTEM_INFO_T *)kmalloc(sizeof(SYSTEM_INFO_T),\
										GFP_KERNEL);
		susp_win = (WINDOW_INFO_T *)kmalloc(sizeof(WINDOW_INFO_T),\
										GFP_KERNEL);
		memset(susp_sys, 0, sizeof(SYSTEM_INFO_T));
		memset(susp_win, 0, sizeof(WINDOW_INFO_T));

		for(i=0;i<DISP_DEV_MAX;i++)
		{
			susp_src[i] = (SOURCE_INFO_T *)kmalloc(sizeof(SOURCE_INFO_T),\
										GFP_KERNEL);
			memset(susp_src[i], 0, sizeof(SOURCE_INFO_T));

			susp_dev[i] = (DISPLAY_DEV_T *)kmalloc(sizeof(DISPLAY_DEV_T),\
										GFP_KERNEL);
			memset(susp_dev[i], 0, sizeof(DISPLAY_DEV_T));
			susp_dev[i]->src = susp_src[i];
			susp_dev[i]->sys = susp_sys;
			susp_dev[i]->win = susp_win;
		}

		// step 2. backup all data
		memcpy(susp_sys, g_sys, sizeof(SYSTEM_INFO_T));
		memcpy(susp_win, g_win, sizeof(WINDOW_INFO_T));
		for(i=0;i<DISP_DEV_MAX;i++)
		{
			memcpy(susp_src[i], g_src[i], sizeof(SOURCE_INFO_T));
			memcpy(susp_dev[i], g_dev[i], sizeof(DISPLAY_DEV_T));
		}
	} while(0);

	return ret;
}

int resource_mgr_resume(void)
{
	int ret = RET_OK;
	int i;
	unsigned long arg;
	unsigned int flag;
	DISPLAY_DEV_T *dev = NULL;

	do {
		dev = g_dev[0];
		DE_IO_Resume(TRUE);
		
//		__test_cmd2mcu(2, "reset", "1",0,0,0,0,0,0,0);

		// init
		RESUME_IO(DE_IO_Init, &dev->sys->p_type);
		DE_PRINT("p_type = %d\n", dev->sys->p_type);

		// system state
		RESUME_IO(DE_IO_SetFrRate, &dev->sys->rate);
		RESUME_IO(DE_IO_SetInterfaceConfig, &dev->sys->if_cfg);
		de_sync_vsync_enable(2);

		// dev state
		for(i=0;i<DISP_DEV_MAX;i++)
		{
			dev = g_dev[i];
			if(!dev->active) continue;

			DE_PRINT("dev name : %s\n", dev->name);

			switch(i)
			{
				case DISP_DEV_SUB_1:
				case DISP_DEV_SUB_2:
				case DISP_DEV_SUB_3:
					{
						// pip enable
						flag = TRUE;
						RESUME_IO(DE_IO_MultiWinEnable, &flag);
					}
				case DISP_DEV_MAIN:
					{
						LX_DE_INPUT_SOURCE_T param;
						LX_DE_CVI_SRC_TYPE_T cvi_param;

						RESUME_IO(DE_IO_SetWinBlank, &dev->win->blank[i]);

						// source
						param.srcId = i;
						param.srcType = dev->src->map;
						param.srcPort = dev->src->port;
						RESUME_IO(DE_IO_SetWinPortSrc, &param);
						memcpy((char*)&cvi_param, \
								(char*)&dev->src->type, \
								sizeof(LX_DE_CVI_SRC_TYPE_T));
						RESUME_IO(DE_IO_SetCviSrcType, &cvi_param);

						// window
						RESUME_IO(DE_IO_SetInputWin, &dev->win->in_win[i]);
						RESUME_IO(DE_IO_SetOutWin,   &dev->win->out_win[i]);
					}
					break;
				case DISP_DEV_VTS:
					{
						LX_DE_CVE_PARAM_T param;
						LX_DE_SRC_MIRROR_T mirror_param;
						// vcr on
						param.fb  = dev->sys->cve_param.fb;
						param.std = dev->sys->cve_param.std;
						RESUME_IO(DE_IO_SetCve, &param);
						// source mirror
						mirror_param.srcPort  = dev->src->mirror.srcPort;
						mirror_param.destPort = dev->src->mirror.destPort;
						mirror_param.enable   = dev->src->mirror.enable;
						RESUME_IO(DE_IO_SetSrcMirror, &mirror_param); 
					}
					break;
				case DISP_DEV_VTV:
					// dvr on
					break;
				case DISP_DEV_VTM:
					// vtm on
					break;
				default:
					break;
			}
		}
		DE_IO_Resume(FALSE);
	} while(0);

	if(susp_sys) kfree(susp_sys);
	if(susp_win) kfree(susp_win);
	for(i=0;i<DISP_DEV_MAX;i++)
	{
		if(susp_src[i]) kfree(susp_src[i]);
		if(susp_dev[i]) kfree(susp_dev[i]);
	}

	if(ret)
		DE_ERROR("resuming is fail.\n");

	return ret;
}

int resource_mgr_probe(void)
{
	int ret = RET_OK;

	return ret;
}

int resource_mgr_remove(void)
{
	int ret = RET_OK;

	return ret;
}

int resource_mgr_ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch(cmd)
	{
		case DE_IOW_DE_INIT:
		case DE_IOW_FW_DOWNLOAD:
		case DE_IOR_GET_FIRMWARE_INFO:
		case DE_IOW_SET_IF_CONFIG:
		case DE_IOW_SET_OUTPUT_MODE :
		case DE_IOW_SET_DIS_FMT:
		case DE_IOW_SET_FR_RATE:
		case DE_IOR_GET_OUT_FR_RATE:
		case DE_IOW_SET_DIS_OUT:
		case DE_IOR_GET_SYS_STATUS:
		case DE_IOR_GET_CAPACITY:			
			{
				ret = _ioctl_system(cmd, arg);
			}
			break;
		case DE_IOW_SET_INPUT_WIN:
		case DE_IOW_SET_OUT_WIN:
		case DE_IOR_GET_INPUT_WIN:
		case DE_IOR_GET_OUT_WIN:
		case DE_IOW_SET_WIN_BLANK:
		case DE_IOW_SET_BG_COLOR:
		case DE_IOW_SET_ZLIST:
		case DE_IOW_SET_MULTI_VISION:   // for multi-vision demo
		case DE_IOW_SET_INNER_PATTERN:
		case DE_IOW_SET_CVI_TPG:
			{
				ret = _ioctl_window(cmd, arg);
			}
			break;
		case DE_IOW_SET_CVI_SRC_TYPE:
		case DE_IOW_SET_CVI_FIR:
		case DE_IOW_SET_CVI_CSAMPLE_MODE:
		case DE_IOW_SET_WIN_PORT_SRC:
		case DE_IOR_GET_SRC_STATUS:
		case DE_IOW_SET_CVI_HDMIPORT:
		case DE_IOW_SET_SRC_MIRROR:
			{
				ret = _ioctl_source(cmd, arg);
			}
			break;
		case DE_IOW_SET_3D_INOUT_CTRL:  // 3D
		case DE_IOW_SET_UD_MODE:        // UD
		case DE_IOW_MULTI_WIN_ENABLE:   // PIP
		case DE_IOW_SET_CVE:
		case DE_IOW_SET_DVR_FMT_CTRL:
		case DE_IOW_SET_VTM:
		case DE_IOR_GET_VTM:
		case DE_IOR_GET_SCALER_INFO:    // for emp csc converter(YC->ARGB)
		case DE_IOW_SET_CVI_FREEZE:
		case DE_IOWR_SET_CVI_VIDEO_FRAME_BUFFER:
		case DE_IOW_SET_CAPTURE:
		case DE_IOW_SET_VCS:
		case DE_IOW_SET_OPERATION :
		case DE_IOW_SET_SUB_OPERATION :
			{
				ret = _ioctl_operation(cmd, arg);
			}
			break;
		case DE_IOW_SET_LOW_DELAY:
		case DE_IOR_GET_LOW_DELAY:
		case DE_IOW_SET_WIN_FREEZE:
		case DE_IOW_SET_ACT_FMT_DETECT:
			{
				ret = _ioctl_control(cmd, arg);
			}
			break;
		default:
			{
				ret = _ioctl_etc(cmd, arg);
			}
			break;
	}

	return ret;
}

DISPLAY_DEV_T* resource_mgr_get_handle(DISP_DEV_TYPE dev)
{
	return g_dev[dev];
}

void resource_mgr_thread(void)
{
	static UINT32 timeout = 3000;

	do {
		if(timeout) OS_MsecSleep(timeout);
		_thread_handle(&timeout);
	} while(0);
}

static int _thread_handle(UINT32 *timeout)
{
	int ret = RET_OK;
	SYSTEM_INFO_T *p = g_sys;
	static UINT32 reset = 0;

	do {
		switch(p->thread_handle.type)
		{
			case THREAD_TYPE_MONITOR :
				*timeout = 30;
				reset = 1;
				break;
			case THREAD_TYPE_IPC_TEST :
				__test_ipc(reset);
				*timeout = 0;
				reset = 0;
				break;
			default:
				break;
		}
	} while(0);

	return ret;
}

static int _init_manager(void)
{
	int ret = RET_OK;
	int i;
	
	// create global source & system & window manager
	for(i=0;i<DISP_DEV_MAX;i++)
	{
		g_src[i] = (SOURCE_INFO_T *)OS_Malloc(sizeof(SOURCE_INFO_T));
		memset(g_src[i], 0, sizeof(SOURCE_INFO_T));
	}
	g_sys = (SYSTEM_INFO_T *)OS_Malloc(sizeof(SYSTEM_INFO_T));
	g_win = (WINDOW_INFO_T *)OS_Malloc(sizeof(WINDOW_INFO_T));
	memset(g_sys, 0, sizeof(SYSTEM_INFO_T));
	memset(g_win, 0, sizeof(WINDOW_INFO_T));

	g_win->order.enable = FALSE; // hw default order (win0-bot,win1-top)
	g_sys->out_en       = TRUE;
	g_sys->initialized  = FALSE;
	
	return ret;
}

static int _uninit_manager(void)
{
	int ret = RET_OK;
	int i;

	// free system & window manager 
	OS_Free(g_sys);
	OS_Free(g_win);
	// free source manager 
	for(i=0;i<DISP_DEV_MAX;i++)	OS_Free(g_src[i]);

	return ret;
}

static int _create_device(  int     index,        \
							char    *name,        \
							BOOLEAN active,       \
							LX_DE_INPUT_SRC_T src)
{
	int ret = RET_OK;
	DISPLAY_DEV_T *dev = NULL;

	do {
		dev = (DISPLAY_DEV_T *)OS_Malloc(sizeof(DISPLAY_DEV_T));
		memset(dev, 0, sizeof(DISPLAY_DEV_T));

		dev->src = g_src[index];
		dev->sys = g_sys;
		dev->win = g_win;

		memcpy(dev->name, name, DEV_NAME_MAX);
		DE_TRACE("@@@ %s   dev[0x%08x]       @@@\n", \
				dev->name, (unsigned int)dev);
		dev->active   = active;
		dev->src->map = src;
		dev->opr      = OPER_MODE_2D;
		
		g_dev[index]  = dev;
	}
	while(0);

	return ret;
}

static int _destroy_device(int index)
{
	int ret = RET_OK;
	int i;

	switch(index)
	{
		case DISP_DEV_MAX:
			for(i=0;i<DISP_DEV_MAX;i++)	OS_Free(g_dev[i]);
			break;
		default :
			if(index >= DISP_DEV_MAX) break;
			OS_Free(g_dev[index]);
			break;
	}

	return ret;
}

static int _ioctl_system(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;
	SYSTEM_INFO_T *p = g_sys;
	
	DE_TRACE("@@ ##  %s \n", __F__);

	switch(cmd)
	{
		case DE_IOW_DE_INIT:
			if(!p->initialized)
			{
				COPY_FROM_USER(p->p_type, LX_DE_PANEL_TYPE_T);
				ret = de_sync_init();
				ret = de_sync_vsync_enable(1);
				CALL_IO(DE_IO_Init);
				p->initialized = TRUE;
			}
			break;
		case DE_IOW_FW_DOWNLOAD:
			{
				COPY_FROM_USER(p->fw_dwld, LX_DE_FW_DWLD_T);
				CALL_IO(DE_IO_FW_Download);
			}
			break;
		case DE_IOR_GET_FIRMWARE_INFO:
			{
				CALL_IO(DE_IO_GetFirmwareInfo);
				COPY_FROM_USER(p->fw_info, LX_DE_FIRMWARE_INFO_T);
			}
			break;
		case DE_IOW_SET_IF_CONFIG:
			{
				COPY_FROM_USER(p->if_cfg, LX_DE_IF_CONFIG_T);
				CALL_IO(DE_IO_SetInterfaceConfig);
			}
			break;
		case DE_IOW_SET_OUTPUT_MODE :
			{
				COPY_FROM_USER(p->out_mode, LX_DE_OUTPUT_MODE_T);
				//CALL_IO(DE_IO_SetOutmode);
			}
			break;
		case DE_IOW_SET_DIS_FMT:
			{
				COPY_FROM_USER(p->fmt, LX_DE_DIS_FMT_T);
				CALL_IO(DE_IO_SetDisFmt);
			}
			break;
		case DE_IOW_SET_FR_RATE:
			{
				COPY_FROM_USER(p->rate, LX_DE_FR_RATE_T);
				CALL_IO(DE_IO_SetFrRate);
			}
			break;
		case DE_IOR_GET_OUT_FR_RATE:
			{
				CALL_IO(DE_IO_GetOutFrRate);
				COPY_FROM_USER(p->out_rate, LX_DE_FR_RATE_T);
			}
			break;
		case DE_IOW_SET_DIS_OUT:
			{
				COPY_FROM_USER(p->out_en, BOOLEAN);
				CALL_IO(DE_IO_SetDisOut);
			}
			break;
		case DE_IOR_GET_SYS_STATUS:
			{
				CALL_IO(DE_IO_GetSystemStatus);
			}
			break;
		case DE_IOR_GET_CAPACITY:
			{
				CALL_IO(DE_IO_GetCapacity);
			}
			break;
		default:
			break;
	}

	return ret;
}

static int _ioctl_window(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;
	WINDOW_INFO_T *p = g_win;

//	DE_TRACE("@@ ##  %s \n", __F__);

	switch(cmd)
	{
		case DE_IOW_SET_INPUT_WIN:
			{
				LX_DE_WIN_DIMENSION_T param;
				COPY_FROM_USER(param, LX_DE_WIN_DIMENSION_T);
				memcpy(&p->in_win[param.win_id], \
						&param,                  \
						sizeof(LX_DE_WIN_DIMENSION_T));
				CALL_IO(DE_IO_SetInputWin);
			}
			break;
		case DE_IOW_SET_OUT_WIN:
			{
				LX_DE_WIN_DIMENSION_T param;
				COPY_FROM_USER(param, LX_DE_WIN_DIMENSION_T);
				memcpy(&p->out_win[param.win_id], \
						&param,                   \
						sizeof(LX_DE_WIN_DIMENSION_T));
				CALL_IO(DE_IO_SetOutWin);
			}
			break;
		case DE_IOR_GET_INPUT_WIN:
			{
				LX_DE_WIN_DIMENSION_T param;
				COPY_FROM_USER(param, LX_DE_WIN_DIMENSION_T);
				CALL_IO(DE_IO_GetInputWin);
				COPY_FROM_USER(p->in_win[param.win_id], \
							LX_DE_WIN_DIMENSION_T);
			}
			break;
		case DE_IOR_GET_OUT_WIN:
			{
				LX_DE_WIN_DIMENSION_T param;
				COPY_FROM_USER(param, LX_DE_WIN_DIMENSION_T);
				CALL_IO(DE_IO_GetOutWin);
				COPY_FROM_USER(p->out_win[param.win_id], \
							LX_DE_WIN_DIMENSION_T);
			}
			break;
		case DE_IOW_SET_WIN_BLANK:
			{
				LX_DE_SET_WIN_BLANK_T param;
				COPY_FROM_USER(param, LX_DE_SET_WIN_BLANK_T);
				memcpy(&p->blank[param.win_id], \
						&param,                 \
						sizeof(LX_DE_SET_WIN_BLANK_T));
				CALL_IO(DE_IO_SetWinBlank);
			}
			break;
		case DE_IOW_SET_ZLIST:
			{
				LX_DE_ZLIST_T param;
				COPY_FROM_USER(param, LX_DE_ZLIST_T);
				memcpy(&p->order, \
						&param,   \
						sizeof(LX_DE_ZLIST_T));
				CALL_IO(DE_IO_SetZList);
			}
			break;
		case DE_IOW_SET_MULTI_VISION: 
			{
				LX_DE_RECT_T param;
				COPY_FROM_USER(param, LX_DE_RECT_T);
				memcpy(&p->multi_vision, \
						&param,          \
						sizeof(LX_DE_RECT_T));
				CALL_IO(DE_IO_SetMultiVision);
			}
			break;
		case DE_IOW_SET_BG_COLOR:
			{
				LX_DE_COLOR_T param;
				COPY_FROM_USER(param, LX_DE_COLOR_T);
				memcpy(&p->bg_color, \
						&param,      \
						sizeof(LX_DE_COLOR_T));
				CALL_IO(DE_IO_SetBgColor);
			}
			break;
		case DE_IOW_SET_INNER_PATTERN:
			{
				LX_DE_INNER_PATTERN_T param;
				COPY_FROM_USER(param, LX_DE_INNER_PATTERN_T);
				memcpy(&p->inner_pattern, \
						&param,      \
						sizeof(LX_DE_INNER_PATTERN_T));
				CALL_IO(DE_IO_SetInnerPattern);
			}
			break;
		case DE_IOW_SET_CVI_TPG:
			{
				LX_DE_CVI_TPG_T param;
				COPY_FROM_USER(param, LX_DE_CVI_TPG_T);
				memcpy(&p->cvi_tpg,  \
						&param,      \
						sizeof(LX_DE_CVI_TPG_T));
				CALL_IO(DE_IO_SetCviTpg);
			}
			break;
		case DE_IOW_SET_SUB_WIN:
			{
				COPY_FROM_USER(p->sub_win, LX_DE_SET_SUB_WIN_T);
				CALL_IO(DE_IO_SetSubWindow);
			}
			break;
		default:
			break;
	}

	return ret;
}

static int _ioctl_source(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;
	
	DE_TRACE("@@ ##  %s \n", __F__);

	switch(cmd)
	{
		case DE_IOW_SET_CVI_SRC_TYPE:
			{
				LX_DE_CVI_SRC_TYPE_T param;
				COPY_FROM_USER(param, LX_DE_CVI_SRC_TYPE_T);
				ret = __source_property(param);
				CALL_IO(DE_IO_SetCviSrcType);
			}
			break;
		case DE_IOW_SET_CVI_FIR:
			{
				LX_DE_CVI_FIR_T param;
				COPY_FROM_USER(param, LX_DE_CVI_FIR_T);
				//ret = __source_property(param);
				CALL_IO(DE_IO_SetCviFir);
			}
			break;
		case DE_IOW_SET_CVI_CSAMPLE_MODE:
			{
				LX_DE_CSAMPLE_MODE_T param;
				COPY_FROM_USER(param, LX_DE_CSAMPLE_MODE_T);
				//ret = __source_property(param);
				CALL_IO(DE_IO_SetCviCsampleMode);
			}
			break;
		case DE_IOW_SET_WIN_PORT_SRC:
			{
				LX_DE_INPUT_SOURCE_T param;
				COPY_FROM_USER(param, LX_DE_INPUT_SOURCE_T);
				ret = __source_win_port_map(param);
				CALL_IO(DE_IO_SetWinPortSrc);
			}
			break;
		case DE_IOR_GET_SRC_STATUS:
			{
				CALL_IO(DE_IO_GetSourceStatus);
			}
			break;			
		case DE_IOW_SET_CVI_HDMIPORT:
			{
				LX_DE_HDMIPORT_T param;
				COPY_FROM_USER(param, LX_DE_HDMIPORT_T);
				CALL_IO(DE_IO_SetHDMIPort);
			}
			break;
		case DE_IOW_SET_SRC_MIRROR:
			{
				LX_DE_SRC_MIRROR_T param;
				COPY_FROM_USER(param, LX_DE_SRC_MIRROR_T);
				ret = __source_mirror(param);
				CALL_IO(DE_IO_SetSrcMirror); 
			}
			break;
		default:
			break;
	}

	return ret;
}

static int _ioctl_control(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;

	switch(cmd)
	{
		case DE_IOW_SET_LOW_DELAY:
			{
				UINT32 param;
				COPY_FROM_USER(param, UINT32);
				ret = __control(DISP_DEV_MAIN,           \
									DEV_CONTROL_LOW_DLY, \
									EXEC_CMD_RUN,        \
									(void *)&param);
				CALL_IO(DE_IO_SetLowDelay);
				break;
			}
		case DE_IOR_GET_LOW_DELAY:
			{
				// DE_IO_GetLowDelay(arg);
				CALL_IO(DE_IO_GetLowDelay);
			}
			break;
		case DE_IOW_SET_WIN_FREEZE:
			{
				LX_DE_WIN_FREEZE_T param;
				COPY_FROM_USER(param, LX_DE_WIN_FREEZE_T);
				ret = __control((DISP_DEV_TYPE)param.win_id,    \
									DEV_CONTROL_FRZ,            \
									(param.bEnable == FALSE) ?  \
									EXEC_CMD_OFF : EXEC_CMD_RUN,\
									(void *)&param);
				CALL_IO(DE_IO_SetWinFreeze);
			}
			break;
		case DE_IOW_SET_ACT_FMT_DETECT:
			{
				LX_DE_ACT_FORMAT_T param;
				COPY_FROM_USER(param, LX_DE_ACT_FORMAT_T);
				ret = __control(DISP_DEV_MAIN,                  \
									DEV_CONTROL_ACT_FMT,        \
									(param.enable == FALSE) ?   \
									EXEC_CMD_OFF : EXEC_CMD_RUN,\
									(void *)&param);
				CALL_IO(DE_IO_SetActFmtDetect);
			}
			break;
		default:
			break;
	}

	return ret;
}

static int _ioctl_operation(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;
	
	DE_TRACE("@@ ##  %s  cmd = %d\n", __F__, cmd & 0xFF);

	switch(cmd)
	{
		case DE_IOW_SET_3D_INOUT_CTRL:  // 3D
			{
				LX_DE_3D_INOUT_CTRL_T param;
				COPY_FROM_USER(param, LX_DE_3D_INOUT_CTRL_T);
				ret = __operation(DISP_DEV_MAIN,                      \
							OPER_MODE_3D,                             \
							(param.run_mode == LX_DE_3D_RUNMODE_OFF) ?\
							EXEC_CMD_OFF :	EXEC_CMD_RUN,             \
							(void *)&param);
				CALL_IO(DE_IO_Set3DInOutCtrl);
			}
			break;
		case DE_IOW_SET_UD_MODE:        // UD
			{
				BOOLEAN param;
				COPY_FROM_USER(param, BOOLEAN);
				ret = __operation(DISP_DEV_MAIN,                      \
									OPER_MODE_UD,                     \
									(param == FALSE) ?                \
									EXEC_CMD_OFF : EXEC_CMD_RUN,      \
									(void *)NULL);
				CALL_IO(DE_IO_SetUDMode);
			}
			break;
		case DE_IOW_MULTI_WIN_ENABLE:   // PIP
			{
				BOOLEAN param;
				COPY_FROM_USER(param, BOOLEAN);
				ret = __operation(DISP_DEV_SUB_1,                     \
									OPER_MODE_PIP,                    \
									(param == FALSE) ?                \
									EXEC_CMD_OFF : EXEC_CMD_RUN,      \
									(void *)NULL);
				CALL_IO(DE_IO_MultiWinEnable);
			}
			break;
		case DE_IOW_SET_CVE:
			{
				LX_DE_CVE_PARAM_T param;
				COPY_FROM_USER(param, LX_DE_CVE_PARAM_T);
				ret = __operation(DISP_DEV_VTS,                       \
									OPER_MODE_MEM,                    \
									EXEC_CMD_RUN,                     \
									(void *)&param);
				CALL_IO(DE_IO_SetCve);
			}
			break;
		case DE_IOW_SET_DVR_FMT_CTRL:
			{
				LX_DE_DVR_DISFMT_T param;
				COPY_FROM_USER(param, LX_DE_DVR_DISFMT_T);
				ret = __operation(DISP_DEV_VTV,                       \
									OPER_MODE_MEM,                    \
									(param.bOnOff == FALSE) ?         \
									EXEC_CMD_OFF : EXEC_CMD_RUN,      \
									(void *)&param);
				CALL_IO(DE_IO_SetDvrFmtCtrl);
			}
			break;
		case DE_IOW_SET_VTM:
			{
				LX_DE_VTM_FRAME_INFO_T param;
				COPY_FROM_USER(param, LX_DE_VTM_FRAME_INFO_T);
				ret = __operation(DISP_DEV_VTM,                       \
									OPER_MODE_MEM,                    \
									(param.enable == FALSE) ?         \
									EXEC_CMD_OFF : EXEC_CMD_RUN,      \
									(void *)&param);
				CALL_IO(DE_IO_SetVTM);
				COPY_FROM_USER(param, LX_DE_VTM_FRAME_INFO_T);
				param.mem_fd = de_mem_fd();
				COPY_TO_USER(param, LX_DE_VTM_FRAME_INFO_T);
			}
			break;
		case DE_IOW_SET_CVI_FREEZE:
			{
				LX_DE_CVI_CAPTURE_FREEZE_T param;
				COPY_FROM_USER(param, LX_DE_CVI_CAPTURE_FREEZE_T);
				CALL_IO(DE_IO_SetCviFreeze);
			}
			break;
		case DE_IOWR_SET_CVI_VIDEO_FRAME_BUFFER:
			{
				LX_DE_CVI_RW_VIDEO_FRAME_T param;
				COPY_FROM_USER(param, LX_DE_CVI_RW_VIDEO_FRAME_T);
				ret = __operation(DISP_DEV_GRAB,                      \
									OPER_MODE_MEM,                    \
									EXEC_CMD_ONESHOT,                 \
									(void *)&param);
				CALL_IO(DE_IO_SetCviVideoFrameBuffer);
			}
			break;
		case DE_IOR_GET_VTM:
			{
				LX_DE_VTM_FRAME_INFO_T param;
				COPY_FROM_USER(param, LX_DE_VTM_FRAME_INFO_T);
				CALL_IO(DE_IO_GetVTM);
				if(param.win_id == 0)
					de_sync_wait(SYNC_WAIT_WEL);
				else if(param.win_id == 1)
					de_sync_wait(SYNC_WAIT_WER);
				else{}
			}
			break;
		case DE_IOR_GET_SCALER_INFO:// for emp csc converter(YC->ARGB)
				CALL_IO(DE_IO_GetScalerInfo);
			break;
		case DE_IOW_SET_CAPTURE:
			{
				LX_DE_CAPTURE_CTRL_T param;
				COPY_FROM_USER(param, LX_DE_CAPTURE_CTRL_T);
				ret = __operation(DISP_DEV_VTC,                 \
									OPER_MODE_MEM,              \
									EXEC_CMD_ONESHOT,           \
									(void *)&param);
				CALL_IO(DE_IO_SetCaptureControl);
			}
			break;
		case DE_IOW_SET_VCS:
			{
				LX_DE_VCS_IPC_T param;
				COPY_FROM_USER(param, LX_DE_VCS_IPC_T);
				CALL_IO(DE_IO_SetVcs); 
			} break;
		case DE_IOW_SET_OPERATION :
			{
				CALL_IO(DE_IO_SetOperation); 
			} break;
		case DE_IOW_SET_SUB_OPERATION :
			{
				CALL_IO(DE_IO_SetSubOperation); 
			} break;
		
		default:
			break;
	}

	return ret;
}

static int _ioctl_etc(unsigned int cmd, unsigned long arg)
{
	int ret = RET_OK;
	
	DE_TRACE("@@ ##  %s \n", __F__);

	switch(cmd)
	{
		case DE_IOW_REG_WR:
			{
				CALL_IO(DE_IO_RegWr);
			}
			break;
		case DE_IOR_REG_RD:
			{
				CALL_IO(DE_IO_RegRd);
			}
			break;
		case DE_IOW_SET_UART_FOR_MCU:
			{
				CALL_IO(DE_IO_SetUart4MCU);
			}
			break;
		case DE_IOW_SET_EVAL_DBG :
			{
				LX_DE_SET_EVAL_DBG_T param;
				COPY_FROM_USER(param, LX_DE_SET_EVAL_DBG_T);
				switch (param.cmd)
				{
					case LX_DE_EVAL_CMD_DBI :
					{
						ret = DBI_VIDEO_Execute(param.arg.argc,\
												param.arg.argv);
					}
					break;
					default :
						break;
				}
			}
			break;
		case DE_IOW_SET_DBG:
			{
				LX_DE_SET_DBG_T param;
				COPY_FROM_USER(param, LX_DE_SET_DBG_T);
				
				if(param.type == LX_DE_DBG_DEVICE_STATUS)
				{
					__view_device();
				}
				else if(param.type == LX_DE_DBG_TRACE_IOCTL)
				{
					__view_ioctl();
				}
				else if(param.type == LX_DE_DBG_MISC_DATA)
				{
#ifdef TEST_IPC_LOGICAL_LAYER
					struct ipc_handle *ipc_handle;
					LX_DE_WIN_FREEZE_T stParams;
					LX_DE_FIRMWARE_INFO_T fw_info;
					UINT32 ipcCmd = 0x53;

					DE_PRINT("ipc logical layer test\n");

					ipc_handle = ipc_hal_open("de", 0); // MCU_ID = 0
					DE_PRINT("mcu ipc is open. handle = 0x%08x\n", \
														(UINT32)ipc_handle);
					stParams.win_id = 0;
					stParams.bEnable = 1;
					stParams.location = 0;
					ret = ipc_handle->m_ipc_client->write(ipcCmd,   \
							&stParams,\
							sizeof(LX_DE_WIN_FREEZE_T));
					ipcCmd = 0x2;
					ret = ipc_handle->m_ipc_client->write(ipcCmd,   \
							&fw_info,\
							0);
					ret = ipc_handle->m_ipc_client->read(&fw_info,\
							sizeof(LX_DE_FIRMWARE_INFO_T));
					DE_PRINT("Fw. info.  : ver %d , y/m/d %04d/%02d/%02d\n",\
							fw_info.version,   \
							fw_info.date.year, \
							fw_info.date.month,\
							fw_info.date.day);
					ipc_hal_close(ipc_handle);
#endif
					__view_misc();
					ipc_hal_list();
				}
				else if(param.type == LX_DE_DBG_IPC_TEST)
				{
					SYSTEM_INFO_T *p = g_sys;
					p->thread_handle.type   = (param.bParam==TRUE) ? \
											  THREAD_TYPE_IPC_TEST : \
											  THREAD_TYPE_MONITOR;
					p->thread_handle.enable = param.bParam;
					p->thread_handle.param  = param.u32Param;
					DE_PRINT("IPC_TEST : enable [%d] , count [%d]\n",\
							param.bParam, param.u32Param);
				}
				else if(param.type == LX_DE_DBG_VERIFY_FW)
				{
					DE_IO_VerifyFirmware(arg);
				}
				else if(param.type == LX_DE_DBG_TRACE_IPC)
				{
					ipc_hal_trace(param.bParam);
				}
				else if(param.type == LX_DE_DBG_RESUME_TEST)
				{
					resource_mgr_suspend();
					resource_mgr_resume();
				}
				else if(param.type == LX_DE_DBG_UHD_SETUP)
				{
					_test_execute4mcu();
				}
				else
				{
					CALL_IO(DE_IO_SetDebug);
				}
			}
			break;
		case DE_IOW_SET_SCART_RGB_BYPASS_MODE: // sw work-around
		default :
			break;
	}

	return ret;
}

static int __operation(	DISP_DEV_TYPE  dev_id,  \
						OPERATION_TYPE opr_type,\
						EXECUTE_CMD    cmd,     \
						void          *param)
{
	int ret = RET_OK;
	DISPLAY_DEV_T *dev = g_dev[dev_id];
	SOURCE_INFO_T *src = dev->src;
	WINDOW_INFO_T *win = dev->win;
	SYSTEM_INFO_T *sys = dev->sys;

	do {
		CHECK_PTR(dev);

		//OPR_MAP(dev->opr, opr_type, cmd);
		__operation_map(&dev->opr, opr_type, cmd);
		
		switch(dev_id)
		{
			case DISP_DEV_MAIN :
				{
		//			dev->active = (cmd == EXEC_CMD_RUN) ? TRUE : FALSE;
				}
				break;
			case DISP_DEV_SUB_1:
				{
					dev->active = (cmd == EXEC_CMD_RUN) ? TRUE : FALSE;
				}
				break;
			case DISP_DEV_VTS :
				{
					LX_DE_CVE_PARAM_T *p = (LX_DE_CVE_PARAM_T *)param;
					dev->active = TRUE;
					src->map = LX_DE_IN_SRC_FB;
					win->scan[dev_id] = LX_DE_INTERLACED;
					win->out_win[dev_id].rect.w = 720;
					win->out_win[dev_id].rect.h = (p->std == LX_DE_CVE_NTSC) ?\
												  480 : 576;
					win->rate[dev_id] = (p->std == LX_DE_CVE_NTSC) ? 60 : 50;
					win->base_addr[dev_id] = p->fb; //(0:NTSC,1:PAL,2:SECAM)
					sys->cve_param.fb  = p->fb;
					sys->cve_param.std = p->std;
				}
				break;
			case DISP_DEV_VTV  :
				{
					LX_DE_DVR_DISFMT_T *p = (LX_DE_DVR_DISFMT_T *)param;
					dev->active = (cmd == EXEC_CMD_OFF) ? FALSE : TRUE;
					src->map = ((p->bOnOff & 0xF) == 1) ?       \
							   g_dev[DISP_DEV_MAIN]->src->map : \
							   ((p->bOnOff & 0xF) > 1)  ? 0 : 1;
					win->scan[dev_id] = p->scan;
					win->out_win[dev_id].rect.w = p->h_size;
					win->out_win[dev_id].rect.h = p->v_size;
					win->rate[dev_id] = p->fr_rate;
					win->base_addr[dev_id] = p->baseAddr;
				}	
				break;
			case DISP_DEV_VTM  :
				{
					LX_DE_VTM_FRAME_INFO_T *p = (LX_DE_VTM_FRAME_INFO_T *)param;
					dev->active = (cmd == EXEC_CMD_OFF) ? FALSE : TRUE;
					if((DISP_DEV_TYPE)p->win_id >= DISP_DEV_MAX) break;
					src->map = g_dev[p->win_id]->src->map;
					win->scan[dev_id] = LX_DE_PROGRESSIVE;
					win->out_win[dev_id].rect.w = win->out_win[p->win_id].rect.w;
					win->out_win[dev_id].rect.h = win->out_win[p->win_id].rect.h;
					win->rate[dev_id] = sys->rate.fr_rate;
					win->base_addr[dev_id] = p->y_frame[0];
				}
				break;
			case DISP_DEV_VTC  :
				{
					LX_DE_CAPTURE_CTRL_T *p = (LX_DE_CAPTURE_CTRL_T *)param;
					dev->active = (cmd == EXEC_CMD_OFF) ? FALSE : TRUE;
					src->map = g_dev[p->win_id]->src->map;
					win->scan[dev_id] = LX_DE_PROGRESSIVE;
					win->out_win[dev_id].rect.w = p->wOutSize;
					win->out_win[dev_id].rect.h = p->hOutSize;
					win->rate[dev_id] = sys->rate.fr_rate;
					win->base_addr[dev_id] = (UINT32)p->p_y_frame;
				}
				break;
			case DISP_DEV_GRAB :
				{
					//LX_DE_CVI_RW_VIDEO_FRAME_T *p = (LX_DE_CVI_RW_VIDEO_FRAME_T *)param;
					dev->active = (cmd == EXEC_CMD_OFF) ? FALSE : TRUE;
					src->map = g_dev[DISP_DEV_MAIN]->src->map;
				}
				break;
			case DISP_DEV_SUB_2:
			case DISP_DEV_SUB_3:
			case DISP_DEV_MAX  :
			default:
				DE_WARN("%s : un-defined dev[%d]\n", __F__, dev_id);
				break;
		}
	} while(0);

	return RET_OK;
}

static int __control(	DISP_DEV_TYPE    dev_id,\
						DISP_DEV_CONTROL ctrl,  \
						EXECUTE_CMD      cmd,   \
						void             *param)
{
	int ret = RET_OK;
	DISPLAY_DEV_T *dev = g_dev[dev_id];

	switch(ctrl)
	{
		case DEV_CONTROL_LOW_DLY :
			dev->low_delay = *(UINT32 *)param;
			break;
		case DEV_CONTROL_FRZ :
			dev->freeze    = (cmd == TRUE) ? TRUE : FALSE;
			break;
		case DEV_CONTROL_ACT_FMT :
			dev->act_fmt_detect = (cmd == TRUE) ? TRUE : FALSE;
			break;
		default :
			break;
	}

	return ret;
}

static int __operation_map( OPERATION_TYPE *p_opr,   \
							OPERATION_TYPE  cur_opr, \
							EXECUTE_CMD     cmd)
{
	int ret = RET_OK;

	if(cmd == EXEC_CMD_OFF)
	{
		*p_opr &= ~cur_opr;
	}
	else if((cmd == EXEC_CMD_RUN) || (cmd == EXEC_CMD_ONESHOT))
	{
		*p_opr |= cur_opr;
	}

	return ret;
}

static int __source_win_port_map(LX_DE_INPUT_SOURCE_T param)
{
	int ret = RET_OK;
	DISP_DEV_TYPE dev_id = (DISP_DEV_TYPE)param.srcId;
	DISPLAY_DEV_T *dev   = g_dev[dev_id];
	SOURCE_INFO_T *src   = dev->src;

	do {
		CHECK_PTR(dev);
		src->map  = (LX_DE_IN_SRC_T)param.srcType;
		src->port = param.srcPort;
	} while(0);

	return ret;
}

static int __source_mirror(LX_DE_SRC_MIRROR_T param)
{
	int ret = RET_OK;
	DISP_DEV_TYPE dev_id = DISP_DEV_VTS;
	DISPLAY_DEV_T *dev   = g_dev[dev_id];
	SOURCE_INFO_T *src   = dev->src;

	do {
		CHECK_PTR(dev);
		src->mirror.srcPort  = param.srcPort;
		src->mirror.destPort = param.destPort;
		src->mirror.enable	 = param.enable;
	} while(0);

	return ret;
}

static int __source_property(LX_DE_CVI_SRC_TYPE_T param)
{
	int ret = RET_OK;
	DISP_DEV_TYPE dev_id = (DISP_DEV_TYPE)param.cvi_channel;
	DISPLAY_DEV_T *dev   = g_dev[dev_id];
	SOURCE_INFO_T *src   = dev->src;

	do {
		CHECK_PTR(dev);
		memcpy(&src->type, &param, sizeof(LX_DE_CVI_SRC_TYPE_T));
	} while(0);

	return ret;
}

static int __view_device(void)
{
	int ret = RET_OK;
	DISPLAY_DEV_T *dev = NULL;
	int i = 0;
	char *str_src, *str_oper;

	do {
		DE_PRINT("\n@@  System Info.\n");
		DE_PRINT("Panel size : %d (0:FHD,1:WXGA(HD),2:1024x768, ...)\n",\
															g_sys->p_type);
		DE_PRINT("Fw. info.  : ver %d , y/m/d %04d/%02d/%02d\n",\
											g_sys->fw_info.version,   \
											g_sys->fw_info.date.year, \
											g_sys->fw_info.date.month,\
											g_sys->fw_info.date.day);
		DE_PRINT("Output en  : %s\n", g_sys->out_en?"TRUE":"FALSE");
		DE_PRINT("h/v/ha/va  : %d/%d/%d/%d\n", \
											g_sys->fmt.h_total, \
											g_sys->fmt.v_total, \
											g_sys->fmt.h_active,\
											g_sys->fmt.v_active);
		DE_PRINT("Panel type : %d (0:LCD,1:PDP,2:OLED,3:BOX,4:GTV_STB)\n",\
											g_sys->fmt.dev_type);
		DE_PRINT("Fr rate    : %d\n", g_sys->rate.fr_rate);
		DE_PRINT("act_fmt_detect : %d\n", g_dev[0]->act_fmt_detect);
		DE_PRINT("(oper bit 0:2D,1:3D,2:UD,3:PIP,4:MEM)\n");

		for(i=DISP_DEV_MAIN;i<DISP_DEV_MAX;i++)
		{
			dev = g_dev[i];
			CHECK_PTR(dev);
			SRC_STR(dev->src->map, str_src);
			OPR_STR(dev->opr,      str_oper);
			DE_PRINT("=== dev[%d] =====================================================\n", i);
			/*DE_PRINT("handle  : dev->0x%08x src->0x%08x opr->0x%08x\n",\
			 		 	i,                     \
						(unsigned int)dev,     \
						(unsigned int)dev->src,\
						(unsigned int)dev->opr);*/
			DE_PRINT("name    : %s\n", dev->name);
			DE_PRINT("active  : %s\n", (dev->active==FALSE)?"FALSE":"TRUE");
			//if(dev->active == TRUE)
			{
				LX_DE_CVI_SRC_TYPE_T p;
				DE_PRINT("opmode  : 0x%X \n", dev->opr);
				DE_PRINT("source  : %s\n", str_src);
				if(i < DISP_DEV_VTS){
					if(dev->win->order.enable == FALSE) {
						DE_PRINT("order   : hw default zorder (win0-bot/win1-top)\n");
					}
					else{
						DE_PRINT("order   : zorder win%d/win%d , alpha : %d/%d\n",\
								dev->win->order.z_list[0],\
								dev->win->order.z_list[1],\
								dev->win->order.alpha[0],\
								dev->win->order.alpha[1]);
					}

					DE_PRINT("bit res : %d", dev->bit_res);
					DE_PRINT(" , low delay : 0x%x , freeze : %d\n",\
							dev->low_delay,\
							dev->freeze);
				}
				DE_PRINT("in_win  : x(%d)/y(%d)/w(%d)/h(%d)\n", \
						dev->win->in_win[i].rect.x,\
						dev->win->in_win[i].rect.y,\
						dev->win->in_win[i].rect.w,\
						dev->win->in_win[i].rect.h);
				DE_PRINT("out_win : x(%d)/y(%d)/w(%d)/h(%d)\n", \
						dev->win->out_win[i].rect.x,\
						dev->win->out_win[i].rect.y,\
						dev->win->out_win[i].rect.w,\
						dev->win->out_win[i].rect.h);
				if(dev->win->inner_pattern.enable)
				{
					DE_PRINT("inner pattern : en(%d) type(%d)(0:full-gray,1:white-box) level(%d)\n",\
							dev->win->inner_pattern.enable, \
							dev->win->inner_pattern.type,   \
							dev->win->inner_pattern.level);
				}
				else
					DE_PRINT("inner pattern disabled");
				if(	(dev->src->map >  LX_DE_IN_SRC_HDMI) || \
					(dev->src->map == LX_DE_IN_SRC_NONE)) continue;
				memcpy(&p, &dev->src->type, sizeof(LX_DE_CVI_SRC_TYPE_T));
				DE_PRINT("  src size h/v/ho/vo %d/%d/%d/%d\n",\
							p.size_offset.hsize,p.size_offset.vsize,\
							p.size_offset.hoffset,p.size_offset.voffset);
				DE_PRINT("  color [%d] (0:601SD,1:709HD,2:RGB,3:RGBEX,\n",p.color);
				DE_PRINT("             4:XVYCC601,5:XVYCC709,6:601CVD)\n");				
				DE_PRINT("  aspect   [%d] (0:4|3,1:16|9)\n",p.aspect);
				DE_PRINT("  scan     [%d] (0:I,1:P)\n",p.scan);
				DE_PRINT("  bit      [%d] (0:10bit,1:8bit)\n",p.bit_res);
				DE_PRINT("  c_sample [%d] (0:422,1:444)\n",p.c_sample);
				DE_PRINT("  de_mode  [%d] (0:normal sync,1:hdmi sync)\n",p.de_mode);
				DE_PRINT("  sampling [%d] (0:normal,1:double,2:quad)\n",p.sampling);
				DE_PRINT("  scaler c sample [%d] (0:yuv420,1:422,2:444)\n",p.sc_c_sample);
				DE_PRINT("  sync stability [%d] (0:std,1:nonstd)\n",p.stability);
				DE_PRINT("  field polarity [%d] (0:org,1:reverse)\n",p.field_polarity);
				DE_PRINT("  yc_delay    [%d] (0:bypass,1:atv,2:others)\n",p.yc_delay);
				DE_PRINT("  cvi port    [%d] (0:ext_a,1:ext_b,2:cvd,3:adc)\n",p.cvi_port);
				DE_PRINT("  3d full fmt [%d] (0:nor,1:FP,2:SSF,3:FIA,\n",p.trid_full_format);
				DE_PRINT("                   4:row-inter5:col-inter,6:4k2k)\n");
				DE_PRINT("  cvi src [%d] (0:VGA,1:YPbPr,2:ATV,3:CVBS,\n",p.cvi_input_src);
				DE_PRINT("               4:SVIDEO,5:SCART,6:HDMI)\n");
				DE_PRINT("  color std [%d] (0:default,1:NTSC_M,2:NTSC_M_RF,3:PAL,4:PAL_RF,\n",\
							p.cvd_color_std);
				DE_PRINT("                 5:SECAM,6:SECAM_RF,7:NTSC_443,8:NTSC_443_RF,\n");
				DE_PRINT("                 9:PAL_60,10:PAL_M_RF,11:PAL_M,12:PAL_M_RF,\n");
				DE_PRINT("                 13:PAL_NC,14:PAL_NC_RF,15:UNKNOWN)\n");
			}
		}
		DE_PRINT("===============================================================\n\n");
	} while(0);

	return ret;
}

static int __view_misc(void)
{
	/* memory configuration */
	de_cfg_disply();

	/* list of callback function  */
	de_sync_display();

	return RET_OK;
}


/* ioctl trace (called history) */

static int __view_ioctl(void)
{
	/* history of ioctl call  */
	__pop_ioc_buf(TRUE, 1);

	return RET_OK;
}

static int _init_ioc_buf(BOOLEAN sel)
{
	int ret = RET_OK;
	int i;

	if(sel == TRUE) // init
	{
		for(i=0;i<DE_IOC_MAXNR;i++)
		{
			memset(ioc_buf[i].s, 0, PARAM_NAME_MAX);
			ioc_buf[i].t.sec  = 0;
			ioc_buf[i].t.msec = 0;
			ioc_buf[i].t.usec = 0;
			ioc_buf[i].p      = NULL;
		}
	}
	else	// cleanup ioc buffer
	{
		for(i=0;i<DE_IOC_MAXNR;i++)
		{
			if(ioc_buf[i].p != NULL) OS_Free(ioc_buf[i].p);
		}
	}

	return ret;
}

static int __push_ioc_buf(unsigned int cmd, char *c, unsigned long arg)
{
	int ret = RET_OK;
	void *param = (void __user *)arg;
	TIME_STAMP_T t;
	UINT32 idx  = (UINT32)((cmd >> _IOC_NRSHIFT)   & _IOC_NRMASK);
	UINT32 size = (UINT32)((cmd >> _IOC_SIZESHIFT) & _IOC_SIZEMASK);
	UINT32 i = 0;

	if(idx > DE_IOC_MAXNR) EXIT_ERROR(); 
	if(ioc_buf[idx].p == NULL)
		ioc_buf[idx].p = (UINT8 *)OS_Malloc(size);

	ioc_buf[idx].c = cmd;
	ret = copy_from_user(ioc_buf[idx].p, (void __user *)param, size);
	if(ret) DE_ERROR("copy error\n");
	
	memset(ioc_buf[idx].s, 0, PARAM_NAME_MAX);
	for(i=0;i<PARAM_NAME_MAX;i++)
	{
		ioc_buf[idx].s[i] = c[i];
		if(c[i] == 0) break;
	}
	
	OS_GetCurrentTicks(&t.sec, &t.msec, &t.usec);
	memcpy(&ioc_buf[idx].t, (char*)&t, sizeof(TIME_STAMP_T));
	
	return RET_OK;
}

static int __pop_ioc_buf(BOOLEAN dump_all, unsigned int c)
{
	int i = 0;
	unsigned int cmd = c;
	UINT32 idx  = (UINT32)((c >> _IOC_NRSHIFT)   & _IOC_NRMASK);

	if(cmd == 0) return RET_OK;

print_start :
	if(dump_all == FALSE)
	{
		i   = idx;
		cmd = c;
	}
	else
	{
		cmd = ioc_buf[i].c;
	}

	if(ioc_buf[i].p == NULL) goto print_end;
	//if( cmd != DE_IOR_GET_INPUT_WIN &&
	//	cmd != DE_IOR_GET_OUT_WIN)
	{
		DE_TRACE("%s : cmd = %d , \t%06d.%06d \n", \
							ioc_buf[i].s, i,  \
							ioc_buf[i].t.sec, \
							ioc_buf[i].t.msec*1000 + ioc_buf[i].t.usec);
	}

	switch(cmd)
	{
		case DE_IOW_DE_INIT:
			{LX_DE_PANEL_TYPE_T *p = (LX_DE_PANEL_TYPE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tpanel type [%d] ",*p);
			DE_TRACE("\t\t(0:1920/1366/1024/1365/3840/1280/720/640/576)\n");
			}break;
		case DE_IOR_GET_FIRMWARE_INFO:
			{LX_DE_FIRMWARE_INFO_T *p = (LX_DE_FIRMWARE_INFO_T *)ioc_buf[i].p;
			DE_TRACE("\t\tversion [%d] y/m/d  %d/%d/%d\n",  \
			p->version,p->date.year,p->date.month,p->date.day);}break;
		case DE_IOW_FW_DOWNLOAD:
			{LX_DE_FW_DWLD_T *p = (LX_DE_FW_DWLD_T *)ioc_buf[i].p;
			DE_TRACE("\t\tfw inx[%d] size[%d]\n",p->inx,p->size);}break;
		case DE_IOW_SET_IF_CONFIG:
			{LX_DE_IF_CONFIG_T *p = (LX_DE_IF_CONFIG_T *)ioc_buf[i].p;
			DE_TRACE("\t\tconfig_type   [%d] ",p->config_type);
			DE_TRACE("\t\t(0:all,1:display_device,2:mirror,3:frc,4:3d)\n");
			DE_TRACE("\t\tdisplay_type  [%d] ",p->display_type);
			DE_TRACE("\t\t(0:LCD/PDP/OLED/CP/STB)\n");
			DE_TRACE("\t\tdisplay_mirror[%d] ",p->display_mirror);
			DE_TRACE("\t\t(0:off/x/y/x_y)\n");
			DE_TRACE("\t\tfrc_type      [%d] ",p->frc_type);
			DE_TRACE("\t\t(0:internal/external)\n");
			DE_TRACE("\t\ttrid_type [%d] (0:off/on)\n",p->trid_type);}break;
		case DE_IOW_SET_DIS_FMT:
			{LX_DE_DIS_FMT_T *p = (LX_DE_DIS_FMT_T *)ioc_buf[i].p;
			DE_TRACE("\t\tpanel_type [%d]\n",p->panel_type);
			DE_TRACE("\t\tfr_rate    [%d]\n",p->fr_rate);
			DE_TRACE("\t\th_total    [%d]\n",p->h_total);
			DE_TRACE("\t\tv_total    [%d]\n",p->v_total);
			DE_TRACE("\t\th_active   [%d]\n",p->h_active);
			DE_TRACE("\t\tv_active   [%d]\n",p->v_active);
			DE_TRACE("\t\tdev_type   [%d] (0:LCD/PDP/OLED/BOX/GTV_STB)\n",\
			p->dev_type);}break;
		case DE_IOW_SET_FR_RATE:
			{LX_DE_FR_RATE_T *p = (LX_DE_FR_RATE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tfree run   [%d]\n",p->isForceFreeRun);
			DE_TRACE("\t\tframe rate [%d]\n",p->fr_rate);}break;
		case DE_IOR_GET_OUT_FR_RATE:
			{LX_DE_FR_RATE_T *p = (LX_DE_FR_RATE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tfree run   [%d]\n",p->isForceFreeRun);
			DE_TRACE("\t\tframe rate [%d]\n",p->fr_rate);}break;
		case DE_IOW_SET_DIS_OUT:
			{BOOLEAN *p = (BOOLEAN *)ioc_buf[i].p;
			DE_TRACE("\t\tdisplay out [%d]\n", *p);}break;
		case DE_IOW_SET_INPUT_WIN:
			{LX_DE_WIN_DIMENSION_T *p = (LX_DE_WIN_DIMENSION_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin [%d]\n",p->win_id);
			DE_TRACE("\t\trect w/h/x/y  %d/%d/%d/%d\n",\
			p->rect.w,p->rect.h,p->rect.x,p->rect.y);}break;
		case DE_IOW_SET_OUT_WIN:
			{LX_DE_WIN_DIMENSION_T *p = (LX_DE_WIN_DIMENSION_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin [%d]\n",p->win_id);
			DE_TRACE("\t\trect w/h/x/y  %d/%d/%d/%d\n",\
			p->rect.w,p->rect.h,p->rect.x,p->rect.y);}break;
		case DE_IOW_SET_WIN_BLANK:
			{LX_DE_SET_WIN_BLANK_T *p = (LX_DE_SET_WIN_BLANK_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin    [%d]\n",p->win_id);
			DE_TRACE("\t\tenable [%d]  isRGB [%d]\n",p->bEnable,p->isRGB);
			DE_TRACE("\t\tcolor r/g/b  %d/%d/%d\n",\
			p->win_color.r,p->win_color.g,p->win_color.b);}break;
		case DE_IOW_SET_BG_COLOR:
			{LX_DE_COLOR_T *p = (LX_DE_COLOR_T *)ioc_buf[i].p;
			DE_TRACE("\t\tr/g/b  %d/%d/%d\n",p->r,p->g,p->b);}break;
		case DE_IOW_SET_ZLIST:
			{LX_DE_ZLIST_T *p = (LX_DE_ZLIST_T *)ioc_buf[i].p;
			DE_TRACE("\t\tenable [%d]\n",p->enable);
			DE_TRACE("\t\torder %d/%d (top/bot)\n",p->z_list[0],p->z_list[1]);
			DE_TRACE("\t\talpha %d/%d (top/bot)\n",p->alpha[0],p->alpha[1]);}break;
		case DE_IOW_SET_MULTI_VISION:   // for multi-vision demo
			{LX_DE_RECT_T *p = (LX_DE_RECT_T *)ioc_buf[i].p;
			DE_TRACE("\t\tw/h/x/y %d/%d/%d/%d\n",p->w,p->h,p->x,p->y);}break;
		case DE_IOW_SET_CVI_SRC_TYPE:
			{LX_DE_CVI_SRC_TYPE_T *p = (LX_DE_CVI_SRC_TYPE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tcvi ch   [%d]\n",p->cvi_channel);
			DE_TRACE("\t\tsrc size h/v/ho/vo %d/%d/%d/%d\n",\
			p->size_offset.hsize,p->size_offset.vsize,\
			p->size_offset.hoffset,p->size_offset.voffset);
			DE_TRACE("\t\tcolor [%d] (0:601SD,1:709HD,2:RGB,3:RGBEX,\n",p->color);
			DE_TRACE("\t\t           4:XVYCC601,5:XVYCC709,6:601CVD)\n");				
			DE_TRACE("\t\taspect   [%d] (0:4|3,1:16|9)\n",p->aspect);
			DE_TRACE("\t\tscan     [%d] (0:I,1:P)\n",p->scan);
			DE_TRACE("\t\tbit      [%d] (0:10bit,1:8bit)\n",p->bit_res);
			DE_TRACE("\t\tc_sample [%d] (0:422,1:444)\n",p->c_sample);
			DE_TRACE("\t\tde_mode  [%d] (0:normal sync,1:hdmi sync)\n",p->de_mode);
			DE_TRACE("\t\tsampling [%d] (0:normal,1:double,2:quad)\n",p->sampling);
			DE_TRACE("\t\tscaler c sample [%d] (0:yuv420,1:422,2:444)\n",p->sc_c_sample);
			DE_TRACE("\t\tsync stability [%d] (0:std,1:nonstd)\n",p->stability);
			DE_TRACE("\t\tfield polarity [%d] (0:org,1:reverse)\n",p->field_polarity);
			DE_TRACE("\t\tyc_delay    [%d] (0:bypass,1:atv,2:others)\n",p->yc_delay);
			DE_TRACE("\t\tcvi port    [%d] (0:ext_a,1:ext_b,2:cvd,3:adc)\n",p->cvi_port);
			DE_TRACE("\t\t3d full fmt [%d] (0:nor,1:FP,2:SSF,3:FIA,\n",p->trid_full_format);
			DE_TRACE("\t\t                 4:row-inter5:col-inter,6:4k2k)\n");
			DE_TRACE("\t\tcvi src [%d] (0:VGA,1:YPbPr,2:ATV,3:CVBS,\n",p->cvi_input_src);
			DE_TRACE("\t\t             4:SVIDEO,5:SCART,6:HDMI)\n");
			DE_TRACE("\t\tcolor std [%d] (0:default,1:NTSC_M,2:NTSC_M_RF,3:PAL,4:PAL_RF,\n",\
			p->cvd_color_std);
			DE_TRACE("\t\t               5:SECAM,6:SECAM_RF,7:NTSC_443,8:NTSC_443_RF,\n");
			DE_TRACE("\t\t               9:PAL_60,10:PAL_M_RF,11:PAL_M,12:PAL_M_RF,\n");
			DE_TRACE("\t\t               13:PAL_NC,14:PAL_NC_RF,15:UNKNOWN)\n");}break;
		case DE_IOW_SET_3D_INOUT_CTRL:  // 3D
			{LX_DE_3D_INOUT_CTRL_T *p = (LX_DE_3D_INOUT_CTRL_T *)ioc_buf[i].p;
			DE_TRACE("\t\trun mode     [%d] ",p->run_mode);
			DE_TRACE("\t\t(0:off,1:on,2:3dTo2d,3:2dTo3d\n");
			DE_TRACE("\t\tin_fmt       [%d] ",p->in_img_fmt);
			DE_TRACE("\t\t(0:TB,1:SS,2:QNC,3:CB,4:FP,5:FRI,6:FIA,7:FS,8:LIA,");
			DE_TRACE("\t\t9:SSF,10:DHD,11:COA,12:LIAH)\n");
			DE_TRACE("\t\tin_lr_order  [%d] ",p->in_lr_order);
			DE_TRACE("\t\t(0:r first,1:l first)\n");
			DE_TRACE("\t\tout fmt      [%d] ",p->out_img_fmt);
			DE_TRACE("\t\t(0:2D,1:TB,2:SS,3:FS,4:FU,5:DTB,6:DSS)\n");
			DE_TRACE("\t\tout_lr_order [%d] ",p->out_lr_order);
			DE_TRACE("\t\t(0:r first,1:l first)\n");}break;
		case DE_IOW_SET_UD_MODE:        // UD
			{BOOLEAN *p = (BOOLEAN *)ioc_buf[i].p;
			DE_TRACE("\t\tud mode [%d]\n",*p);}break;
		case DE_IOW_MULTI_WIN_ENABLE:   // PIP
			{BOOLEAN *p = (BOOLEAN *)ioc_buf[i].p;
			DE_TRACE("\t\tpip en [%d]\n",*p);}break;
		case DE_IOW_SET_CVE:
			{LX_DE_CVE_PARAM_T *p = (LX_DE_CVE_PARAM_T *)ioc_buf[i].p;
			DE_TRACE("\t\tpath [%d] (0:w/osd,1:wo/osd,2:src)\n",p->fb);
			DE_TRACE("\t\ttime [%d] (0:NTSC,1:PAL,2:SECAM)\n",p->std);}break;
		case DE_IOW_SET_DVR_FMT_CTRL:
			{LX_DE_DVR_DISFMT_T *p = (LX_DE_DVR_DISFMT_T *)ioc_buf[i].p;
			DE_TRACE("\t\tonoff [%d]\n",p->bOnOff);
			DE_TRACE("\t\tscan  [%d] (0:i,1:p)\n",p->scan);
			DE_TRACE("\t\th/v   %d/%d\n", p->h_size,p->v_size);
			DE_TRACE("\t\tfr rate    %d\n",p->fr_rate);
			DE_TRACE("\t\tbase addr  0x%08x\n",p->baseAddr);}break;
		case DE_IOW_SET_VTM:
			{LX_DE_VTM_FRAME_INFO_T *p = (LX_DE_VTM_FRAME_INFO_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin    [%d]\n",p->win_id);
			DE_TRACE("\t\taddr   0x%08x\n",p->address);
			DE_TRACE("\t\tfd     [%d]\n",p->mem_fd);
			DE_TRACE("\t\tenable [%d]\n",p->enable);
			DE_TRACE("\t\tstride %d\n",p->stride);
			DE_TRACE("\t\tsampling [%d] (0:420,1:422,2:444)\n",p->sampling);}break;
		case DE_IOR_GET_VTM:
			{LX_DE_VTM_FRAME_INFO_T *p = (LX_DE_VTM_FRAME_INFO_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin    [%d]\n",p->win_id);}break;
		case DE_IOR_GET_SCALER_INFO:    // for emp csc converter(YC->ARGB)
			{LX_DE_SCALER_INFO_T *p = (LX_DE_SCALER_INFO_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin [%d]\n",p->win_id);
			DE_TRACE("\t\tw/h/s %d/%d/%d\n",p->width,p->height,p->stride);
			DE_TRACE("\t\tsampling [%d] (0:420/422/444)\n",p->sampling);
			DE_TRACE("\t\ty/c addr 0x%08x/0x%08x\n",\
			(UINT32)p->p_y_frame,(UINT32)p->p_cbcr_frame);}break;
		case DE_IOW_SET_LOW_DELAY:
			{UINT32 *p = (UINT32 *)ioc_buf[i].p;
			DE_TRACE("\t\tparam  0x%08x\n",*p);}break;
		case DE_IOW_SET_CAPTURE:
			{LX_DE_CAPTURE_CTRL_T *p = (LX_DE_CAPTURE_CTRL_T *)ioc_buf[i].p;
			DE_TRACE("\t\tlocation [%d] (0:source,1:display)\n",p->location);
			DE_TRACE("\t\tin win w/h/x/y %d/%d/%d/%d\n",\
			p->inWin.w,p->inWin.h,p->inWin.x,p->inWin.y);
			DE_TRACE("\t\tout win w/h %d/%d\n",p->wOutSize,p->hOutSize);
			DE_TRACE("\t\tpixel fmt [%d] (0:RGB10/RGB8/YC444_10/444_8/",p->pixelFormat);
			DE_TRACE("\t\t422_10/422_8/420_10/420_8)\n");
			DE_TRACE("\t\ty/c addr 0x%08x/0x%08x, stride[%d]\n",\
			(UINT32)p->p_y_frame,(UINT32)p->p_cbcr_frame, p->stride);}break;
		case DE_IOW_SET_CVI_FREEZE:
			{LX_DE_CVI_CAPTURE_FREEZE_T *p = (LX_DE_CVI_CAPTURE_FREEZE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tOn/Off capture [%d] cvifreeze [%d]\n",\
					p->bCaptureOnOff, p->bCviFreezeOnOff);}break;
		case DE_IOWR_SET_CVI_VIDEO_FRAME_BUFFER:
			{LX_DE_CVI_RW_VIDEO_FRAME_T *p = (LX_DE_CVI_RW_VIDEO_FRAME_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin [%d]\n",p->region.win_id);
			DE_TRACE("\t\tw/h/x/y %d/%d/%d/%d\n",\
			p->region.pixelGrabW,p->region.pixelGrabH,\
			p->region.pixelGrabX,p->region.pixelGrabY);
			DE_TRACE("\t\treal w/h in memory  %d/%d\n",\
			p->region.realPixelGrabW,p->region.realPixelGrabH);
			DE_TRACE("\t\tcolorspace [%d] (0:yuv,1:rgb)\n",p->region.colorSpace);
			DE_TRACE("\t\tcolorDepth [%d] (0:8bit,1:10bit)\n",p->region.colorDepth);
			DE_TRACE("\t\tphy Y8/Y2/C8/C2 [0x%08x/0x%08x/0x%08x/0x%08x] , stride [%d]",\
			p->frameAddr.phyFrameAddrY8,p->frameAddr.phyFrameAddrC8,\
			p->frameAddr.phyFrameAddrY2,p->frameAddr.phyFrameAddrC2,\
			p->frameAddr.stride);}break;
		case DE_IOW_SET_INNER_PATTERN:
			{LX_DE_INNER_PATTERN_T *p = (LX_DE_INNER_PATTERN_T *)ioc_buf[i].p;
			DE_TRACE("\t\tinner pattern  en[%d] type[%d](0:full-gray,1:white-box) level[%d]\n",
			p->enable,p->type,p->level);}break;
		case DE_IOW_SET_WIN_FREEZE:
			{LX_DE_WIN_FREEZE_T *p = (LX_DE_WIN_FREEZE_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin[%d] enable[%d] loc[%d] (0:disp,1:input,2:sub)\n",\
			p->win_id, p->bEnable,p->location);}break;
		case DE_IOW_SET_WIN_PORT_SRC:
			{LX_DE_INPUT_SOURCE_T *p = (LX_DE_INPUT_SOURCE_T *)ioc_buf[i].p;
			DE_TRACE("\t\twin[%d] type[%d] port[%d] \n" ,p->srcId, p->srcType,p->srcPort);
			DE_TRACE("\t\t type=(0:none,1:vga,2:ypbpr,3:atv,4:cvbs,5:scart,");
			DE_TRACE("\t\t       6:hdmi,7:mvi,8:cpu,9:fb)\n");}break;
		case DE_IOW_SET_CVI_TPG:
			{LX_DE_CVI_TPG_T *p = (LX_DE_CVI_TPG_T *)ioc_buf[i].p;
			DE_TRACE("\t\tcvi channel[%d] ptn[%d] frz[%d] gbr[%d]\n",\
			p->cvi_channel,p->isPtnOn,p->isFrzOn,p->isGBR);
			DE_TRACE("\t\tpattern color[%d]\n",p->ptnColor);
			DE_TRACE("\t\t 0:white,1:yellow,2:cyan,3:green,4:magenta,5:red,6:blue,7:black\n");
			DE_TRACE("\t\t 8:gradation-h,9:grad-v,10:grad-c,11:grad-yc,12:black/white\n");
			}break;
		case DE_IOW_SET_SRC_MIRROR:
			{LX_DE_SRC_MIRROR_T *p = (LX_DE_SRC_MIRROR_T *)ioc_buf[i].p;
			DE_TRACE("\t\tsrcPort[%d] dstPort[%d] enb[%d]\n",\
			p->srcPort,p->destPort,p->enable);}break;
		case DE_IOW_SET_ACT_FMT_DETECT:
			{LX_DE_ACT_FORMAT_T *p = (LX_DE_ACT_FORMAT_T *)ioc_buf[i].p;
			DE_TRACE("\t\tenable[%d] chgFlag[%d]\n",p->enable,p->chgFlag);}break;
		case DE_IOW_SET_CVI_FIR:
			{LX_DE_CVI_FIR_T *p = (LX_DE_CVI_FIR_T *)ioc_buf[i].p;
			DE_TRACE("\t\tcvi_ch[%d] enable[%d]\n",p->cvi_channel,p->isEnable);
			DE_TRACE("\t\ty[%02x %02x %02x %02x %02x %02x %02x %02x]\n",\
			p->fir_coef[0], p->fir_coef[1], p->fir_coef[2], p->fir_coef[3],\
			p->fir_coef[4], p->fir_coef[5], p->fir_coef[6], p->fir_coef[7]);
			DE_TRACE("\t\tc[%02x %02x %02x %02x %02x %02x]\n",\
			p->fir_coef_CbCr[0], p->fir_coef_CbCr[1], p->fir_coef_CbCr[2],\
			p->fir_coef_CbCr[3], p->fir_coef_CbCr[4], p->fir_coef_CbCr[5]);}break;
		case DE_IOW_SET_CVI_CSAMPLE_MODE:
			{LX_DE_CSAMPLE_MODE_T *p = (LX_DE_CSAMPLE_MODE_T *)ioc_buf[i].p;
			DE_TRACE("\t\tcvi_ch[%d] is3tap[%d]\n",p->cvi_channel,p->is3tap);}break;
		case DE_IOW_SET_SUB_WIN:
			{LX_DE_SET_SUB_WIN_T *p = (LX_DE_SET_SUB_WIN_T *)ioc_buf[i].p;
			DE_TRACE("\t\tsub_win[%d]\n",p->win_id);}break;			
		default :
			break;
	}
print_end :
	if(++i >= DE_IOC_MAXNR) return RET_OK;
	if(dump_all) goto print_start;

	return RET_OK;
}

static int __test_ipc(UINT32 reset)
{
	int ret = RET_OK;
	SYSTEM_INFO_T *p = g_sys;
	static UINT32 send_count = 0;
	static UINT32 discont_count = 0;
	LX_DE_FIRMWARE_INFO_T send_info;
	unsigned long arg = (unsigned long)&send_info;
	static TIME_STAMP_T t[2];

	do {
		if(reset)
		{
			send_count = 0;
			discont_count = 0;
			OS_GetCurrentTicks(&t[0].sec, &t[0].msec, &t[0].usec);
		}
		send_info.ipc_count = send_count;
		DE_IO_GetFirmwareInfo(arg);
		COPY_FROM_USER(send_info, LX_DE_FIRMWARE_INFO_T);
		if((send_count + 1) != send_info.ipc_count)
		{
			discont_count++;
			DE_PRINT("Discontinuty : received count[%d] , sent count [%d]\n", \
					send_info.ipc_count, send_count);
		}
		send_count++;
		if(send_count >= p->thread_handle.param)
		{
			OS_GetCurrentTicks(&t[1].sec, &t[1].msec, &t[1].usec);

			DE_PRINT("Self-Ipc-Test\n");
			DE_PRINT("#########################################\n");
			DE_PRINT("Done. Total iteration [%d]\n", send_count);
			DE_PRINT("Started  at %d.%06d  sec\n", t[0].sec, t[0].msec * 1000 + t[0].usec);
			DE_PRINT("Finished at %d.%06d  sec\n", t[1].sec, t[1].msec * 1000 + t[1].usec);
			DE_PRINT("Discontinuity is occured [ %d ] times\n", discont_count);
			DE_PRINT("#########################################\n\n");

			p->thread_handle.type = THREAD_TYPE_MONITOR;
			p->thread_handle.enable = TRUE;
			send_count = 0;
			discont_count = 0;
		}
	} while(0);

	return ret;
}

static int _test_execute4mcu(void)
{
	__test_cmd2mcu(3, "WR", "0xC428", "0xFFFFFFFF",0,0,0,0,0,0);
	__test_cmd2mcu(4, "MVVDO", "0", "0x10","0xF0",0,0,0,0,0);
	__test_cmd2mcu(2, "MVDF", "4", 0,0,0,0,0,0,0);
	__test_cmd2mcu(3, "WR", "0xC428", "0",0,0,0,0,0,0);
	__test_cmd2mcu(2, "MVDF", "4", 0,0,0,0,0,0,0);

	printk("%s : done...\n", __F__);
	return 0;
}

static int __test_cmd2mcu(int argc, char* cmd, char* param0,   \
		char* param1, char* param2, char* param3, char* param4,\
		char* param5, char* param6, char* param7)
{
	char *cmd_buf[9];
	int i;
	int ret = 0;

	if(argc < 1) return ret;

	cmd_buf[0] = cmd;
	cmd_buf[1] = param0;
	cmd_buf[2] = param1;
	cmd_buf[3] = param2;
	cmd_buf[4] = param3;
	cmd_buf[5] = param4;
	cmd_buf[6] = param5;
	cmd_buf[7] = param6;
	cmd_buf[8] = param7;

	for(i=0;i<8;i++)
	{
		if(cmd_buf[i] != 0)	printk("%s ", cmd_buf[i]);
	}
	printk("\n");

	ret = DBI_VIDEO_Execute(argc, (char**)cmd_buf);

	if (ret) printk("cmd error\n");
	OS_MsecSleep(30);
	return ret;
}
