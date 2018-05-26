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
 * resource manager header file for de device.
 *
 * author     
 * version    1.0
 * date       
 * note       
 *
 * @addtogroup 
 * @{
 */

#ifndef _RESOURCE_MGR_h
#define _RESOURCE_MGR_h
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_kapi.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define OPR_MAP(dev_opr,type,cmd) { dev_opr = (cmd==EXEC_CMD_RUN) ? \
									type :                          \
									OPER_MODE_NONE;	}

#define OPR_STR(type,str)            \
    switch(type)                     \
    {                                \
		case OPER_MODE_NONE :        \
		case OPER_MODE_2D :          \
            str = "2D";       break; \
		case OPER_MODE_3D :          \
            str = "3D";       break; \
		case OPER_MODE_UD :          \
            str = "UD";       break; \
		case OPER_MODE_PIP :         \
            str = "PIP";      break; \
		case OPER_MODE_MEM :         \
            str = "MEM";      break; \
        default :                    \
            str = "UNDEFINED";break; \
	}                                

#define DEV_STR(type,str)            \
    switch(type)                     \
    {                                \
		case DISP_DEV_MAIN :         \
            str = "MAIN";     break; \
		case DISP_DEV_SUB_1 :        \
            str = "SUB_1";    break; \
		case DISP_DEV_SUB_2 :        \
            str = "SUB_2";    break; \
		case DISP_DEV_SUB_3 :        \
            str = "SUB_3";    break; \
		case DISP_DEV_VTS :          \
            str = "VCR";      break; \
		case DISP_DEV_VTV :          \
            str = "DVR";      break; \
		case DISP_DEV_VTM :          \
            str = "VTG";      break; \
		case DISP_DEV_VTC :          \
            str = "CAPTURE";  break; \
        default :                    \
            str = "UNDEFINED";break; \
	}                                

#define SRC_STR(type,str)            \
    switch(type)                     \
    {                                \
		case LX_DE_IN_SRC_NONE :     \
            str = "NONE";     break; \
		case LX_DE_IN_SRC_VGA :      \
            str = "VGA";      break; \
		case LX_DE_IN_SRC_YPBPR :    \
            str = "YPBPR";    break; \
		case LX_DE_IN_SRC_ATV :      \
            str = "ATV";      break; \
		case LX_DE_IN_SRC_CVBS :     \
            str = "CVBS";     break; \
		case LX_DE_IN_SRC_SCART :    \
            str = "SCART";    break; \
		case LX_DE_IN_SRC_HDMI :     \
            str = "HDMI";     break; \
		case LX_DE_IN_SRC_MVI :      \
            str = "MVI";      break; \
		case LX_DE_IN_SRC_CPU :      \
            str = "CPU";      break; \
		case LX_DE_IN_SRC_FB :       \
            str = "FB";       break; \
		default :                    \
            str = "UNDEFINED";break; \
	}                                

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
#define DEV_NAME_MAX 16
#define PARAM_NAME_MAX 32

typedef enum {
	OPER_MODE_NONE         = 0x00000000,
	OPER_MODE_2D           = 0x00000001,
	OPER_MODE_3D           = 0x00000002,
	OPER_MODE_UD           = 0x00000004,
	OPER_MODE_PIP          = 0x00000008,
	OPER_MODE_MEM          = 0x00000010,
	OPER_MODE_MAX          = 0xFFFFFFFF
} OPERATION_TYPE;

typedef enum {
	EXEC_CMD_OFF,
	EXEC_CMD_RUN,
	EXEC_CMD_ONESHOT,
	EXEC_CMD_MAX
} EXECUTE_CMD;

typedef enum {
	DEV_CONTROL_NONE,
	DEV_CONTROL_LOW_DLY,
	DEV_CONTROL_FRZ,
	DEV_CONTROL_CAP,
	DEV_CONTROL_ACT_FMT,
	DEV_CONTROL_MAX
} DISP_DEV_CONTROL;

typedef enum {
	DISP_DEV_MAIN = 0,   // To Display
	DISP_DEV_SUB_1,
	DISP_DEV_SUB_2,
	DISP_DEV_SUB_3,
	DISP_DEV_VTS,        // To Scart-out
	DISP_DEV_VTV,        // To Venc
	DISP_DEV_VTM,        // To Memory for gpu
	DISP_DEV_VTC,        // To Capture frame
	DISP_DEV_GRAB,       // to grab pixels
	DISP_DEV_MAX
} DISP_DEV_TYPE;

typedef enum {
	DISP_ORDER_0 = 0,
	DISP_ORDER_1,
	DISP_ORDER_2,
	DISP_ORDER_BG_0 = 8,
	DISP_ORDER_BG_1,
	DISP_ORDER_MAX
} DISP_ORDER_TYPE;

typedef enum {
	THREAD_TYPE_MONITOR = 0,
	THREAD_TYPE_IPC_TEST,

	THREAD_TYPE_MAX
} THREAD_TYPE_T;

struct s_thread_handle {
	THREAD_TYPE_T type;
	BOOLEAN enable;
	UINT32 param;
};

typedef struct {
	BOOLEAN                  initialized;
	LX_DE_PANEL_TYPE_T       p_type;
	LX_DE_FIRMWARE_INFO_T    fw_info;
	LX_DE_FW_DWLD_T          fw_dwld;
	LX_DE_IF_CONFIG_T        if_cfg;
	BOOLEAN                  out_en;
	LX_DE_DIS_FMT_T          fmt;
	LX_DE_FR_RATE_T          rate;
	LX_DE_FR_RATE_T          out_rate;
	LX_DE_OUTPUT_MODE_T      out_mode;
	LX_DE_CVE_PARAM_T        cve_param;
	struct s_thread_handle   thread_handle;
} SYSTEM_INFO_T;

typedef struct {
	LX_DE_WIN_DIMENSION_T    in_win[DISP_DEV_MAX];
	LX_DE_WIN_DIMENSION_T    out_win[DISP_DEV_MAX];
	LX_DE_SET_WIN_BLANK_T    blank[DISP_DEV_MAX];
	LX_DE_SCAN_T             scan[DISP_DEV_MAX];
	UINT32                   rate[DISP_DEV_MAX];
	UINT32                   base_addr[DISP_DEV_MAX];
	LX_DE_COLOR_T            bg_color;
	LX_DE_ZLIST_T            order;
	LX_DE_RECT_T             multi_vision;
	LX_DE_INNER_PATTERN_T    inner_pattern;
	LX_DE_CVI_TPG_T          cvi_tpg;
	LX_DE_SET_SUB_WIN_T      sub_win;
} WINDOW_INFO_T;

typedef struct {
	LX_DE_IN_SRC_T           map;
	LX_DE_CVI_SRC_TYPE_T     type;
	UINT32                   port;
	LX_DE_SRC_MIRROR_T       mirror;
} SOURCE_INFO_T;

typedef struct {
	// device index
	char idx;

	// device name
	char name[DEV_NAME_MAX];

	DISP_DEV_TYPE type;
	OPERATION_TYPE opr;
	
	// active/non-active status (active:TRUE, non-active:FALSE)
	BOOLEAN active;

	char bit_res;

	// device control
	UINT32 low_delay;
	BOOLEAN freeze;
	BOOLEAN act_fmt_detect;


	/* device specific information */

	// source map          (source port index)
	// source property     (DTV/Ext.Input/File Play/Image Render/Feedback)
	SOURCE_INFO_T          *src;


	/* global information */
	
	SYSTEM_INFO_T          *sys; // system, display output, operation
	WINDOW_INFO_T          *win; // input/output window, blank, scan,
	                             // rate, base address info
} DISPLAY_DEV_T;


/* ioctl trace (called history) */
typedef struct {
	UINT32 sec;
	UINT32 msec;
	UINT32 usec;
} TIME_STAMP_T;

typedef struct {
	char         s[PARAM_NAME_MAX];  //
	UINT32       c;     // ioctl cmd
	TIME_STAMP_T t;     // time stamp
	void        *p;     // param
} CMD_PARAM_BUF_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
int resource_mgr_init(void);
int resource_mgr_cleanup(void);
int resource_mgr_ioctl(unsigned int cmd, unsigned long arg);
int resource_mgr_suspend(void);
int resource_mgr_resume(void);
int resource_mgr_probe(void);
int resource_mgr_remove(void);
DISPLAY_DEV_T* resource_mgr_get_handle(DISP_DEV_TYPE dev);
void resource_mgr_thread(void);

#endif
/**  @} */



