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
 *  driver interface header for logm device. ( used only within kdriver )
 *	logm device will teach you how to make device driver with new platform.
 *
 *  @author		hwang.hayun (hwang.hayun@lge.com)
 *  @version	1.0 
 *  @date		2013.10.29
 *
 *  @addtogroup lg115x_logm
 *	@{
 */

#ifndef	_LOGM_DRV_H_
#define	_LOGM_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "logm_kapi.h"

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
//logm buffer
#define LOGM_BUFFER_NUMBER      1
#define LOGM_BUFFER_SIZE        (1024*1024)
#define LOGM_NAME_LEVEL_FORMAT 	"%-8.8s %-3.3s"

#define ALIGN8(x) 		(((x) + 7) & ~7)
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
struct logm_buffer_ctrl
{
    SINT32      buffer_head;
    SINT32      buffer_tail;
    SINT32      buffer_write_counter;
    SINT32      buffer_dropped;
    SINT32      buffer_last_readed_counter;
	SINT32 		buffer_wrap_around;
	SINT32 		buffer_lmsg_tail;
	SINT32 		buffer_lmdump_tail;
	UINT32 		log_level_cnt[LX_LOGM_LEVEL_TRACE+2];
    spinlock_t  buffer_lock;
    wait_queue_head_t buffer_wait;
    wait_queue_head_t buffer_lmdump_wait;
    char        *logm_buf;
};

struct logm_header
{
	UINT64 clock;
	LX_LOGM_LOGOBJ_T *obj;
	LX_LOGM_LOGLEVEL_T level;

	/* data type and size that fallowing this header */
	LX_LOGM_LOGTYPE_T type;
	SINT32 size;

	/* for binary */
	const void *bin_data;
	SINT32 bin_size;

	/* continuty counter to check buffer drop */
	SINT32 count;

	UINT32 ptid;

};

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	int      LOGM_Init(void);
extern	void     LOGM_Cleanup(void);
extern	void     LOGM_PreInit(void);

//lgom_proc.c
void LOGM_CreateObjProc(const char *name, LX_LOGM_LOGOBJ_T *obj);

//logm_drv.c
int LOGM_PutLine (LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, LX_LOGM_LOGTYPE_T type,
		const void *bin_data, int bin_size,
		const char *str, int len);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
//logm buffer
extern  struct logm_buffer_ctrl g_logm_buffer_ctrl[LOGM_BUFFER_NUMBER];
extern spinlock_t g_logm_obj_map_lock;
extern struct proc_dir_entry *g_logm_proc_entry;
extern int g_logm_debug_level;
extern int g_logm_global_ctrl_buffer;
extern unsigned int g_logm_path_ctrl;
extern unsigned int g_logm_short_format;
extern unsigned int g_logm_log_color;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LOGM_DRV_H_ */

/** @} */
