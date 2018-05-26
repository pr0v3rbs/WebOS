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
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "os_util.h"
#include "base_device.h"
#include "logm_kapi.h"
#include "logm_drv.h"
#include "logm_cfg.h"

/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#define LOG_FORMAT_PREFIX	"%-20.20s:%4d ] "

//setup initial value of log level, force error and warning level
#define LOGM_FORCE_ERR_WARN 		1

#define LOGM_MAKE_PTID(pid,tid)  ((((pid)&0xffff)<<16)|((tid)&0xffff))
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
  global Variables
  ----------------------------------------------------------------------------------------*/
const char	*g_logm_clrStrings[] =
{   /* color    : index: Value      */
	"\x1b[0m",				/* reset attribute */

	"\x1b[0;30m",			/* fg black 	*/
	"\x1b[1;31m",			/* fg red 		*/
	"\x1b[1;32m",			/* fg green 	*/
	"\x1b[1;33m",			/* fg yellow 	*/
	"\x1b[1;34m",			/* fg blue 		*/
	"\x1b[1;35m",			/* fg purple 	*/
	"\x1b[1;36m",			/* fg cyan 		*/
	"\x1b[1;37m",			/* fg black 	*/

	"\x1b[4;30m",			/* fg black - underline */
	"\x1b[4;31m",			/* fg red 	- underline */
	"\x1b[4;32m",			/* fg green - underline */
	"\x1b[4;33m",			/* fg yellow - underline */
	"\x1b[4;34m",			/* fg blue 	- underline */
	"\x1b[4;35m",			/* fg purple - underline */
	"\x1b[4;36m",			/* fg cyan 	- underline */
	"\x1b[4;37m",			/* fg black - underline */

	"\x1b[1;37;40m",		/* bg black 	*/
	"\x1b[1;37;41m",		/* bg red 		*/
	"\x1b[0;30;42m",		/* bg green 	*/
	"\x1b[0;30;43m",		/* bg yellow 	*/
	"\x1b[1;37;44m",		/* bg blue 		*/
	"\x1b[1;37;45m",		/* bg purple 	*/
	"\x1b[1;30;46m",		/* bg cyan 		*/
	"\x1b[1;30;47m",		/* bg black 	*/
};

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
static int LOGM_PutBuffer(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, LX_LOGM_LOGTYPE_T type,
		const void *bin_data, int bin_size,
		const char *str, int len, struct logm_buffer_ctrl *bc);
static int LOGM_DropLine(struct logm_buffer_ctrl *bc);
static int LOGM_GetTailHeader(struct logm_header **h
		, unsigned char **data, struct logm_buffer_ctrl *bc);
static int _LOGM_PutLogPrintk(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, const char *str, int len);
static void _LOGM_BufLogCnt(struct logm_buffer_ctrl *bc, 
		LX_LOGM_LOGLEVEL_T level);
static void _LOGM_ObjLogCnt(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level);
static int _LOGM_CheckObjName(const char *name);
/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/*========================================================================================
  Implementation Group
  ========================================================================================*/

#define debug(fmt, arg...)	do{if(g_logm_debug_level&0x01) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define trace(fmt, arg...)	do{if(g_logm_debug_level&0x02) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define error(fmt, arg...)	do{printk (KERN_ERR "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)

#define LOGM_CHECK_ERROR(__checker,__if_action,fmt,args...)   \
	__CHECK_IF_ERROR(__checker, error, __if_action , fmt, ##args )

/**
 * register logm obj in obj map
 * @param *name [in]
 * @return int : return index of obj map to reference the obj
 * WARN : this function is called occasionally in IRQ
 * ------obj map-----
 * |     obj 0      |
 * ------------------
 * |     obj 1      |
 * ------------------
 * |     obj 2      |
 * ------------------
 * |     obj 3      |
 * ------------------
 * |     obj ...    |
 * ------------------
 */ 
int LOGM_ObjRegister(const char *name)
{
	SINT32 i;
	unsigned long flag;
	unsigned int create_proc = 0;
	LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;

	LOGM_CHECK_ERROR(name == NULL, return RET_ERROR,
			"param null\n");

	//TODO: check length of name

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
	{
		//found logm obj
		//name array must have a null byte tail of array
		if(!strncmp(map[i].name, name, LOGM_MODULE_NAME_MAX - 1))
		{
			if(map[i].ref_cnt == 0 && map[i].has_proc == OFF)
				create_proc = 1;

			map[i].ref_cnt++;
			break;
		}
		if(map[i].name[0] == '\0')
		{
			if(_LOGM_CheckObjName(name) != RET_OK)
			{
				error("obj name must use [a-z|A-Z|-|_|0-9]\n");
				i = LOGM_LOGOBJ_MAX;
				break;
			}
			//register new obj
			strncpy(map[i].name, name, LOGM_MODULE_NAME_MAX);
			map[i].name[LOGM_MODULE_NAME_MAX - 1] = '\0';
#if LOGM_FORCE_ERR_WARN
			map[i].mask = ((1<<LX_LOGM_LEVEL_ERROR)
							| (1<<LX_LOGM_LEVEL_WARNING));
#else
			map[i].mask = 0;
#endif
			map[i].level = 0;
			map[i].ref_cnt = 1;
			create_proc = 1;
			break;
		}
	}
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);
	
	if(likely(i >= 0 && i < LOGM_LOGOBJ_MAX))
		debug("register obj : %s,%d\n", name, i);
	else
	{
		error("fail register obj : %s\n", name);
		return RET_ERROR;
	}
		
	if(create_proc)
		LOGM_CreateObjProc(name, &map[i]);

	return i;
}
EXPORT_SYMBOL(LOGM_ObjRegister);

/**
  * check objname
  */
static int _LOGM_CheckObjName(const char *name)
{
	int j;
	
	for(j = 0; j < LOGM_MODULE_NAME_MAX; j++)
	{
		char c = name[j];
		//0 ~ 9 : 0x30 ~ 0x39
		//A ~ Z : 0x41 ~ 0x5a
		//a ~ z : 0x61 ~ 0x7a
		//_ , - : 0x5f, 0x2d
		if(c >= 0x30 && c <= 0x39)
			;
		else if(c >= 0x41 && c <= 0x51)
			;
		else if(c >= 0x61 && c <= 0x7a)
			;
		else if(c == 0x5F || c == 0x2D)
			;
		else if(c == '\0')
			break;
		else
			return RET_ERROR;
	}

	return RET_OK;
}

/**
 * deregister logm obj in obj map
 * @param *name [in]
 * @return int
 */
int LOGM_ObjDeregister(const char *name)
{
	SINT32 i;
	SINT32 ret = RET_ERROR;
	unsigned long flag;
	LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;

	LOGM_CHECK_ERROR(name == NULL, return RET_ERROR,
			"param null\n");

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
	{
		if(!strncmp(map[i].name, name, LOGM_MODULE_NAME_MAX - 1))
		{
			map[i].ref_cnt--;
			ret = RET_OK;
			goto func_exit;
		}
	}
	error("not found obj : %s\n", name);
func_exit:
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return ret;
}
EXPORT_SYMBOL(LOGM_ObjDeregister);

/**
 * set mask of logm obj
 * if bit '1' then enable print of the bit 
 * @param fd
 * @return int
 */
int LOGM_ObjMaskSet(SINT32 fd, UINT32 mask)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].mask = mask;
#if LOGM_FORCE_ERR_WARN
	//always error, warning
	g_logm_obj_map[fd].mask |= ((1<<LX_LOGM_LEVEL_ERROR)
								| (1<<LX_LOGM_LEVEL_WARNING));
#endif
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjMaskSet);

/**
 * get mask of logm obj
 * @param fd
 * @return int
 */
int LOGM_ObjMaskGet(SINT32 fd, UINT32 *mask)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	*mask = g_logm_obj_map[fd].mask;
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjMaskGet);

/**
 * enable a bit mask of logm obj
 * if bit '1' then enable print of the bit 
 * @param fd
 * @param bit
 */
int LOGM_ObjBitMaskEnable(SINT32 fd, SINT32 bit)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	if(bit > 31 || bit < 0)
	{
		error("out of range. bit = %d\n", bit);
		return -1;
	}

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].mask |= (1<<bit);
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	debug("enable bit %d, %s\n", bit, g_logm_obj_map[fd].name);
	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjBitMaskEnable);

/**
 * disable a bit mask of logm obj
 * if bit '0' then disable print of the bit 
 * @param fd
 * @param bit
 */
int LOGM_ObjBitMaskDisable(SINT32 fd, SINT32 bit)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	if(bit > 31 || bit < 0)
	{
		error("out of range. bit = %d\n", bit);
		return -1;
	}

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].mask &= ~(1<<bit);
#if LOGM_FORCE_ERR_WARN
	//always error, warning
	g_logm_obj_map[fd].mask |= ((1<<LX_LOGM_LEVEL_ERROR)
								| (1<<LX_LOGM_LEVEL_WARNING));
#endif
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	debug("disable bit %d, %s\n", bit, g_logm_obj_map[fd].name);
	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjBitMaskDisable);

/**
 * set user_mask of logm obj
 * It is used to just debug
 * if bit '1' then disable print of the bit 
 * the bit used to mask
 * @param fd
 * @return int
 */
int LOGM_ObjUserMaskSet(SINT32 fd, UINT32 mask)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].user_mask = mask;
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjUserMaskSet);

/**
 * get user_mask of logm obj
 * It is used to just debug
 * @param fd
 * @return int
 */
int LOGM_ObjUserMaskGet(SINT32 fd, UINT32 *mask)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	*mask = g_logm_obj_map[fd].user_mask;
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjUserMaskGet);

/**
 * enable a bit user_mask of logm obj
 * It is used to just debug
 * if bit '0' then enable print of the bit 
 * the bit used to mask
 * @param fd
 * @param bit
 */
int LOGM_ObjBitUserMaskEnable(SINT32 fd, SINT32 bit)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	if(bit > 31 || bit < 0)
	{
		error("out of range. bit = %d\n", bit);
		return -1;
	}

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].user_mask &= ~(1<<bit);
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	debug("enable user bit %d, %s\n", bit, g_logm_obj_map[fd].name);
	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjBitUserMaskEnable);

/**
 * disable a bit user_mask of logm obj
 * It is used to just debug
 * if bit '1' then disable print of the bit 
 * the bit used to mask
 * @param fd
 * @param bit
 */
int LOGM_ObjBitUserMaskDisable(SINT32 fd, SINT32 bit)
{
	unsigned long flag;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	if(bit > 31 || bit < 0)
	{
		error("out of range. bit = %d\n", bit);
		return -1;
	}

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	g_logm_obj_map[fd].user_mask |= (1<<bit);
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	debug("disable user bit %d, %s\n", bit, g_logm_obj_map[fd].name);
	return RET_OK;
}
EXPORT_SYMBOL(LOGM_ObjBitUserMaskDisable);

/**
 * get fd of logm obj
 * @return fd
 */
int LOGM_ObjFDGet(const char *name)
{
	SINT32 i;
	unsigned long flag;
	LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;

	LOGM_CHECK_ERROR(name == NULL, return RET_ERROR,
			"param null\n");

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
	{
		//found logm obj
		if(!strncmp(map[i].name, name, LOGM_MODULE_NAME_MAX - 1))
			goto func_exit;//return index of the obj
		if(map[i].name[0] == '\0')
			break;
	}
	i = -1;//fail find obj
func_exit:

	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);
	return i;
}

/**
 * write a log in logm buffer
 * this functions is called in interrupt, so you task care of using the functions.
 *
 * @param *obj  [in]
 * @param level [in]
 * @param *fn   [in] funcation name
 * @param fl    [in] code line
 * @return int
 *
 */
int LOGM_WriteLog(int *fd, char *name,  LX_LOGM_LOGLEVEL_T level,
		const char *fn, int fl, const char *fmt, ...)
{
	va_list     		args;
	char        		*buf;
	LX_LOGM_LOGOBJ_T 	*obj;
	SINT32      		len = 256;
	SINT32      		n = 0;
	SINT32      		i = 0;

	//don't need to check whether fd is null or not.
	//this function is call by MACRO.

	LOGM_CHECK_ERROR((*fd < 0 || *fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR
			, "invalid fd = %d [called %s:%d]\n", *fd, fn, fl);
	
	obj = &(g_logm_obj_map[*fd]);

	LOGM_CHECK_ERROR((obj->name[0] == 0 || obj->ref_cnt == 0)
			, return RET_ERROR 
			, "uninitialized fd = %d [called %s:%d]\n", *fd, fn, fl);

	if(!((obj->mask & ~(obj->user_mask)) & (1<<(level))))
		return 0;

	debug ("write, name %s\n", obj->name);

	while (1)
	{
		debug ("try buffer size %d\n", len);
		buf = kmalloc (len, GFP_ATOMIC);
		if (buf == NULL)
		{
			error ("no memory for %d sized log write\n", len);
			n = -ENOMEM;
			break;
		}

		if(!g_logm_short_format)
			i = snprintf(buf, len, LOG_FORMAT_PREFIX, fn, fl); 
		else
			i = snprintf(buf, len, "] "); 

		va_start (args, fmt);
		n = vsnprintf (buf + i, len - i, fmt, args);
		va_end (args);
		n += i;

		if (n < len)
		{

			debug ("got %d size string\n", n);
			if (buf[n-1] == '\n')
			{
				debug ("remove tailing newline\n");
				n--;
			}

			LOGM_PutLine(obj, level, LX_LOGM_LOGTYPE_0, NULL, 0,
					buf, n);

			break;
		}

		kfree (buf);
		len *= 2;
	}

	kfree(buf);

	return n;
}
EXPORT_SYMBOL (LOGM_WriteLog);

/**
 * put a log in logm buffer
 *
 * @param *obj  [in]
 * @param level [in]
 * @param type  [in] log type
 * @param *bin_data [in] binary log : when binary mode
 * @param bin_size  [in] binary size : when binary mode
 * @param *str      [in] log
 * @param len       [in] length of log
 * @return int
 *
 * TODO: when several buffers, how to control?
 */
int LOGM_PutLine (LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, LX_LOGM_LOGTYPE_T type,
		const void *bin_data, int bin_size,
		const char *str, int len)
{
	unsigned long flag;
	int ret;
	void *data = NULL;
	struct logm_buffer_ctrl *bc = &(g_logm_buffer_ctrl[0]);

	//if true, all logs be printed through "printk"
	//not null, not \n
	if(g_logm_path_ctrl)
		_LOGM_PutLogPrintk(obj, level, str, len);

	//put log at logm buffer
	spin_lock_irqsave (&(bc->buffer_lock), flag);
	ret = LOGM_PutBuffer(obj, level, type,
			data, bin_size,
			str, len, bc);

	if(g_logm_global_ctrl_buffer && 
			!g_logm_path_ctrl)
		wake_up(&(bc->buffer_wait));
	else
		LOGM_DropLine(bc);//because, Do not print logs.

	wake_up(&(bc->buffer_lmdump_wait));

	//count log level of buffer
	if(likely(ret == RET_OK))
		_LOGM_BufLogCnt(bc, level);

	spin_unlock_irqrestore (&(bc->buffer_lock), flag);

	//count log level of obj
	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	if(likely(ret == RET_OK))
		_LOGM_ObjLogCnt(obj, level);
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	return ret;
}

static int _LOGM_PutLogPrintk(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, const char *str, int len)
{
	const char 		*col_str = NULL;
	const char 		*nocol_str = NULL;
	unsigned int 	col_len = 0;
	unsigned int 	nocol_len = 0;
	char 			*pk_buf;
	unsigned int 	pk_size;

	if(g_logm_log_color && level == LX_LOGM_LEVEL_ERROR)
		col_str = g_logm_clrStrings[DBG_COLOR_RED];
	else if(g_logm_log_color &&level == LX_LOGM_LEVEL_WARNING)
		col_str = g_logm_clrStrings[DBG_COLOR_PURPLE];

	if(col_str)
	{
		col_len = strlen(col_str);
		nocol_str = g_logm_clrStrings[DBG_COLOR_NONE];
		nocol_len = strlen(nocol_str);
	}

	pk_size = col_len + len + nocol_len + 1;//add null
	pk_buf = (char*)kmalloc(pk_size, GFP_ATOMIC);
	LOGM_CHECK_ERROR(pk_buf == NULL, return RET_ERROR,
			"fail kmalloc\n");

	if(col_str)
	{
		memcpy((void*)pk_buf, (void*)col_str, col_len);
		memcpy((void*)(&pk_buf[col_len+len])
				, (void*)nocol_str , nocol_len);
	}
	memcpy((void*)(&pk_buf[col_len]), (void*)str, len);
	pk_buf[pk_size - 1] = '\0';

	if(!g_logm_short_format)
	{
		printk (KERN_ERR LOGM_NAME_LEVEL_FORMAT " (%05d) %s\n",
				obj->name,
				LOGM_LevelName(level),
				//current->comm,
				task_tgid_nr(current),
				pk_buf);
	}
	else
	{
		printk (KERN_ERR LOGM_NAME_LEVEL_FORMAT " %s\n",
				obj->name,
				LOGM_LevelName(level),
				pk_buf);
	}

	kfree(pk_buf);

	return RET_OK;
}

/**
  * counting log number per level of buffer
  */
static void _LOGM_BufLogCnt(struct logm_buffer_ctrl *bc, 
		LX_LOGM_LOGLEVEL_T level)
{
	if(level <= LX_LOGM_LEVEL_TRACE)
		bc->log_level_cnt[level]++;
	else
		bc->log_level_cnt[LX_LOGM_LEVEL_TRACE+1]++;

}

/**
 * counting log number per level of obj 
 */
static void _LOGM_ObjLogCnt(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level)
{
	if(level <= LX_LOGM_LEVEL_TRACE)
		obj->log_level_cnt[level]++;
	else
		obj->log_level_cnt[LX_LOGM_LEVEL_TRACE+1]++;
}


/**
 * "put a log" implementation of buffer control
 * TODO: call in lock
 * TODO : remove contant value
 *
 * @param *obj  [in]
 * @param level [in]
 * @param type  [in] log type
 * @param *bin_data [in] binary log : when binary mode
 * @param bin_size  [in] binary size : when binary mode
 * @param *str      [in] log
 * @param len       [in] length of log
 * @param *bc       [in/out] variables of buffer control
 * @return int
 *
 */
static int LOGM_PutBuffer(LX_LOGM_LOGOBJ_T *obj,
		LX_LOGM_LOGLEVEL_T level, LX_LOGM_LOGTYPE_T type,
		const void *bin_data, int bin_size,
		const char *str, int len, struct logm_buffer_ctrl *bc)
{
	int buffer_head_tmp;
	struct logm_header *header;

	/* too big size, fail */
	if (LOGM_BUFFER_SIZE < ALIGN8(sizeof(struct logm_header)*2+len))
	{
		debug ("too big len. %d\n", len);
		return -1;
	}

	/* check the buffer space.
	 * if there is not enough space, drop a line at tail.
	 */
	/* case1 */
	if (bc->buffer_tail <= bc->buffer_head)
	{
case1:
		if (LOGM_BUFFER_SIZE < ALIGN8(bc->buffer_head+sizeof(struct logm_header)*2+len))
		{
			header = (struct logm_header*)(bc->logm_buf+bc->buffer_head);
			header->size = -1;

			if (bc->buffer_tail == 0)
				LOGM_DropLine(bc);

			/* now, it becomes case2 */
			bc->buffer_head = 0;
			bc->buffer_wrap_around = 1;
			bc->buffer_lmsg_tail = 0;
		}
	}

	/* case2 */
	if (bc->buffer_head < bc->buffer_tail)
	{
		while (bc->buffer_tail - bc->buffer_head <= ALIGN8(sizeof(struct logm_header)+len))
		{
			debug ("head %4d\n", bc->buffer_head);
			LOGM_DropLine(bc);

			if (bc->buffer_tail < bc->buffer_head)
				goto case1;

			if (bc->buffer_tail == bc->buffer_head)
			{
				bc->buffer_tail = bc->buffer_head = 0;
				bc->buffer_lmsg_tail = 0;//TODO:what case?
				bc->buffer_wrap_around = 0;
				trace("ignore case\n");
				break;
			}
		}
	}

	/* move lmsg_tail */
	if(bc->buffer_wrap_around)
	{
		SINT32 				b_tail;
		SINT32  			next_b_head;
		char 				*b_buf;
		struct 	logm_header *_h;

		b_buf = bc->logm_buf;
		b_tail = bc->buffer_lmsg_tail;
		next_b_head = bc->buffer_head + ALIGN8(sizeof(struct logm_header) + len);

		while(b_tail < next_b_head)
		{
			_h = (struct logm_header *)(b_buf + b_tail);
			if(_h->size < 0)
			{
				b_tail = 0;
				bc->buffer_wrap_around = 0;
				//trace("wrap lsmg_tail\n");
				break;
			}
			b_tail += ALIGN8(sizeof(struct logm_header) + _h->size);

			if(unlikely(b_tail >= LOGM_BUFFER_SIZE))
			{
				b_tail = 0;
				bc->buffer_wrap_around = 0;
				error("fail parse tail, tail -> 0\n");
				break;
			}
		}
		bc->buffer_lmsg_tail = b_tail;

		//trace("wrap head %x tail %x cnt %d\n", next_b_head, b_tail, i);
	}

	/* now, its safe to store the data.
	 * lets copy the data.
	 */
	header = (struct logm_header*)(bc->logm_buf+bc->buffer_head);
	header->clock = local_clock();
	header->obj = obj;
	header->level = level;
	header->type = type;
	header->size = len;
	header->bin_data = bin_data;
	header->bin_size = bin_size;
	header->ptid = LOGM_MAKE_PTID(task_tgid_nr(current), task_pid_nr(current));
	header->count = ++(bc->buffer_write_counter);
	memcpy (bc->logm_buf+bc->buffer_head+sizeof(struct logm_header), str, len);

	buffer_head_tmp = bc->buffer_head;
	bc->buffer_head += ALIGN8(sizeof(struct logm_header)+len);
	debug ("move head %4d -> %4d\n", buffer_head_tmp, bc->buffer_head);

	return 0;
}

/**
 * remove a log in buffer
 * TODO: call in lock
 * TODO : remove contant value
 * @param *bc       [in/out] variables of buffer control
 * @return int
 *
 */
static int LOGM_DropLine(struct logm_buffer_ctrl *bc)
{
	int buffer_tail_tmp;
	struct logm_header *h;

	/* read size */
	if (LOGM_GetTailHeader(&h, NULL, bc) < 0)
		return -1;

	/* move tail */
	if (h->size < 0)
		bc->buffer_tail = 0;
	else
	{
		buffer_tail_tmp = bc->buffer_tail;
		bc->buffer_tail += ALIGN8(sizeof(struct logm_header)+h->size);
		debug ("move tail %4d -> %4d\n", buffer_tail_tmp, bc->buffer_tail);

		//if (h->bin_data)
		//vfree (h->bin_data);
	}

	/* wrap if its the end */
	if ((LOGM_GetTailHeader(&h, NULL, bc) == 0) && (h->size == -1))
		bc->buffer_tail = 0;

	bc->buffer_dropped++;
	debug ("drop a line. %d line dropped\n", bc->buffer_dropped);

	return 0;
}

static int LOGM_GetTailHeader(struct logm_header **h
		, unsigned char **data, struct logm_buffer_ctrl *bc)
{
	if (bc->buffer_tail == bc->buffer_head)
	{
		debug ("Ooops. no data.\n");
		return -1;
	}

	/* read */
	if(data)
		*data = bc->logm_buf+bc->buffer_tail+sizeof(struct logm_header);

	if(h)
		*h = (struct logm_header*)(bc->logm_buf+bc->buffer_tail);

	return 0;
}

/**
 * read a log in logm buffer
 * TODO: call in lock
 * @param *bc       [in/out] variables of buffer control
 * @param *data     [out] buffer to store a log
 * @param readed    [in]
 * @param size      [in]
 * @return int
 *
 */
int LOGM_ReadLog(struct logm_buffer_ctrl *bc, 
		char *data, int readed, int size)
{
	int			ret;
	int32_t		buffer_tail_tmp;
	char		*buf = data+readed;
	int32_t		len = size-readed;
	struct		logm_header *h;

	if (LOGM_GetTailHeader(&h, NULL, bc) < 0)
		return -EIO;//no data

	debug ("length %d\n", h->size);

	//buffer wrap-around
	if (h->size < 0)
	{
		bc->buffer_tail = 0;
		return -EAGAIN;
	}

	if (h->type == LX_LOGM_LOGTYPE_0)
	{
		ret = LOGM_ParseLog(h, buf, len);
		if(ret < 0)
			goto func_exit;
	}
	else
	{
		error ("Oops\n");
		ret = 0;
	}

	buffer_tail_tmp = bc->buffer_tail;
	bc->buffer_tail += ALIGN8(sizeof(struct logm_header) + h->size);
	debug ("move tail %4d -> %4d\n", buffer_tail_tmp, bc->buffer_tail);

	//diff number of log, checking drop of log
	if ((h->count-1) != bc->buffer_last_readed_counter)
		error ("buffer dropped. %d\n",
				h->count - bc->buffer_last_readed_counter - 1);
	bc->buffer_last_readed_counter = h->count;

func_exit:
	return ret;
}

/**
 * parse log
 */
int LOGM_ParseLog(struct logm_header *h, char *dest, int32_t dest_len)
{
	const char 	*col_str = NULL;
	const char 	*nocol_str = NULL;
	uint32_t 	col_len = 0;
	uint32_t 	nocol_len = 0;
	uint32_t 	hlen;
	uint32_t 	length = h->size;
	uint32_t 	log_len = 0;
	uint32_t 	nanosec_rem;
	uint64_t 	t;
	char 		*data;

	if(!g_logm_short_format)//normal log
		hlen = 5+6+13+12;//string length of header : "[%5lu.%06lu] " LOGM_NAME_LEVEL_FORMAT " (%05d) "
	else
		hlen = 5+6+12+5;

	if (hlen+length+1 > dest_len)
	{
		/* not enough buffer */
		debug ("not enough buffer\n");
		return -ENOMEM;
	}

	t = h->clock;
	nanosec_rem = do_div(t, 1000000000);
	if(!g_logm_short_format)
	{
		hlen = snprintf (dest, dest_len,
				"[%5lu.%06u] " LOGM_NAME_LEVEL_FORMAT " (%05d) ",
				(unsigned long)t, nanosec_rem/1000,
				h->obj->name,
				LOGM_LevelName(h->level),
				(h->ptid >> 16) & 0xffff);
	}
	else
	{
		hlen = snprintf (dest, dest_len,
				"[%5lu.%06u] " LOGM_NAME_LEVEL_FORMAT " ",
				(unsigned long)t, nanosec_rem/1000,
				h->obj->name,
				LOGM_LevelName(h->level));
	}
	if (dest_len == hlen)
	{
		debug ("need more buffer\n");
		return -ENOMEM;
	}

	if(g_logm_log_color && h->level == LX_LOGM_LEVEL_ERROR)
		col_str = g_logm_clrStrings[DBG_COLOR_RED];
	else if(g_logm_log_color && h->level == LX_LOGM_LEVEL_WARNING)
		col_str = g_logm_clrStrings[DBG_COLOR_PURPLE];

	if(col_str)
	{
		col_len = strlen(col_str);
		nocol_str = g_logm_clrStrings[DBG_COLOR_NONE];
		nocol_len = strlen(nocol_str);
	}

	log_len = hlen + length + col_len + nocol_len + 1;//add '\n'

	if ( log_len > dest_len)
	{
		/* not enough buffer */
		debug ("not enough buffer\n");
		return -ENOMEM;
	}

	if(col_str)
	{
		memcpy((void*)(&dest[hlen]), (void*)col_str, col_len);
		memcpy((void*)(&dest[hlen + col_len + length]), (void*)nocol_str, nocol_len);
	}

	data = (void *)h + sizeof(struct logm_header);
	memcpy((void*)(&dest[hlen + col_len]), data, length);

	/* put tailing new line */
	dest[log_len - 1] = '\n';
	return log_len;
}

/**
 * return level name
 * @param l     [in] log level
 * @return char *
 *
 */
const char *LOGM_LevelName(LX_LOGM_LOGLEVEL_T l)
{
	static char buf[32];

	switch (l)
	{
		case LX_LOGM_LEVEL_ERROR:
			return "ERR";
		case LX_LOGM_LEVEL_WARNING:
			return "WAR";
		case LX_LOGM_LEVEL_NOTI:
			return "NOT";
		case LX_LOGM_LEVEL_INFO:
			return "INF";
		case LX_LOGM_LEVEL_DEBUG:
			return "DEB";
		case LX_LOGM_LEVEL_TRACE:
			return "TRA";

		default:
			sprintf (buf, "B%02d", l);
			return buf;
	}
}

/**
 * return printk level
 * @param l     [in] log level
 * @return int
 *
 */
int LOGM_PrintkLevel(LX_LOGM_LOGLEVEL_T l)
{
	return 3;//force ERROR, printk
#if 0
	switch (l)
	{
		case LX_LOGM_LEVEL_ERROR://KERN_ERR
			return 3;
		case LX_LOGM_LEVEL_WARNING:
			return 4;
		case LX_LOGM_LEVEL_NOTI:
			return 5;
		case LX_LOGM_LEVEL_INFO:
			return 6;

		default:
		case LX_LOGM_LEVEL_DEBUG:
		case LX_LOGM_LEVEL_TRACE:
			return 7;
	}
#endif
}


/** @} */

