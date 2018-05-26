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
 *  Linux proc interface for logm device.
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
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/ctype.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/workqueue.h>

#include "os_util.h"
#include "logm_drv.h"
#include "logm_cfg.h"
#include "proc_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

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
enum logm_set_mask_type
{
	logm_set_mask,
	logm_add_mask,
	logm_del_mask,
};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _LOGM_ProcStatusOpen (struct inode *inode
                                    , struct file *file);
static int _LOGM_ProcStatusShow (struct seq_file *s, void *data);
static ssize_t _LOGM_ProcStatusWrite (struct file *file
        , const char __user *buf, size_t size, loff_t *off);
static int _LOGM_ObjMaskOpen (struct inode *inode
        , struct file *file);
static ssize_t _LOGM_ObjMaskWrite (struct file *file,
		const char __user *buf, size_t size, loff_t *off);
static int _LOGM_ObjMaskRelease (struct inode *inode
        , struct file *file);
static int _LOGM_ObjMaskShow (struct seq_file *s, void *data);
static int __obj_mask_write (LX_LOGM_LOGOBJ_T *obj
        , const char *buf, int size);
static int _LOGM_ProcLMsgOpen(struct inode *inode
										, struct file *file);
static int _LOGM_ProcLMsgShow(struct seq_file *s, void *data);
static ssize_t _LOGM_ProcLMsgWrite(struct file *file,
		const char __user *buf, size_t size, loff_t *off);
static void _LOGM_CreateObjProcBH (struct work_struct *work);
static void __obj_mask_set (LX_LOGM_LOGOBJ_T *obj
        , enum logm_set_mask_type type, unsigned int mask);
static int _LOGM_LmdumpOpen(struct inode *inode
										, struct file *file);
static int _LOGM_LmdumpClose(struct inode *inode
										, struct file *file);
static ssize_t _LOGM_LmdumpRead(struct file *file,
		char __user *data, size_t size, loff_t *off);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct proc_dir_entry *proc_entry;

static struct file_operations proc_status_fops =
{
	.open = _LOGM_ProcStatusOpen,
	.read = seq_read,
	.write = _LOGM_ProcStatusWrite,
	.llseek = seq_lseek,
	.release = single_release,
};

static struct file_operations proc_fops =
{
	.open = _LOGM_ObjMaskOpen,
	.write = _LOGM_ObjMaskWrite,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = _LOGM_ObjMaskRelease,
};

static struct file_operations proc_lmsg_fops = 
{
	.open = _LOGM_ProcLMsgOpen,
	.write = _LOGM_ProcLMsgWrite,
	.read = seq_read,
	.release = single_release,
};

static struct file_operations proc_lmdump_fops = 
{
	.open = _LOGM_LmdumpOpen,
	.release = _LOGM_LmdumpClose,
	.read = _LOGM_LmdumpRead,
};

struct _LOGM_ObjProcs
{
	char *name;
	LX_LOGM_LOGOBJ_T *obj;

	struct _LOGM_ObjProcs *next;
};

//create obj proc file
static DEFINE_SPINLOCK (_LOGM_ObjProcsRootLock);
static struct _LOGM_ObjProcs _LOGM_ObjProcsRoot;
static DECLARE_WORK (_LOGM_ProcWork, _LOGM_CreateObjProcBH);

//print log
static UINT32 _g_logm_latest_num = 0;


typedef struct
{
	char *name[LX_LOGM_LEVEL_MAX+1];
}LX_LOGM_BIT_NAME_T;

//module mask bit custom name list
static LX_LOGM_BIT_NAME_T *_g_logm_bit_name_list;

/*========================================================================================
	Implementation Group
========================================================================================*/
#define debug(fmt, arg...)	do{if(g_logm_debug_level&0x01) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define trace(fmt, arg...)	do{if(g_logm_debug_level&0x02) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define error(fmt, arg...)	do{printk (KERN_ERR "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)

#define LOGM_CHECK_ERROR(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, error, __if_action , fmt, ##args )

static ssize_t _LOGM_ProcStatusWrite (struct file *file
        , const char __user *buf, size_t size, loff_t *off)
{
	char *str;
	char *name;
	char *sep, *p;
	int cnt;
	int ret = size;

	if (*off != 0)
	{
		error ("nonzero offset\n");
		return -EINVAL;
	}

    str = vmalloc (size);
    if(str == NULL)
    {
        error("fail vmalloc\n");
        return -EFAULT;
    }

    if(copy_from_user(str, buf, size))
    {
        error ("copy_from_user failed\n");
        ret = -EFAULT;
		goto func_exit;
    }

    str[size-1] = 0;

    cnt = 0;
	name = str;
	p = str;
	while (cnt<size && *p != ':')
	{
		p ++;
		cnt ++;
	}

	if (cnt == size || *p != ':')
	{
		error ("no name seperator. "
				"write something like \"vdec:debug\".\n");
		ret = -EINVAL;
		goto func_exit;
	}

	*p = 0;
	sep = p+1;

	while (
			*name == ' ' ||
			*name == '\t' ||
			*name == '\n'
	      )
		name ++;

	while (str < p && (
				*(p-1) == ' ' ||
				*(p-1) == '\t' ||
				*(p-1) == '\n'
			  ))
	{
		p --;
		*p = 0;
	}

	debug ("obj name \"%s\", mask \"%s\"\n",
			name, sep);

    //set g_logm_global_ctrl_buffer
    if(!strncmp(name, "global", strlen("global")))
    {
        unsigned long i;

		i = simple_strtoul(sep, NULL, 10);
		//on => on : nothing
		if(i && g_logm_global_ctrl_buffer)
			;
		//off => on : enable print
		else if(i && !g_logm_global_ctrl_buffer)
		{
			g_logm_global_ctrl_buffer = 1;
			debug("Enable logm display...!\n");
		}
		//off => off : nothing
		else if(!i && !g_logm_global_ctrl_buffer)
			;
		//on => off : disable print
		else if(!i && g_logm_global_ctrl_buffer)
		{
			g_logm_global_ctrl_buffer = 0;
			debug("Disable logm display...!\n");
		}
    }
    //path ctrl
	else if(!strncmp(name, "printk", strlen("printk")))
	{
        unsigned long i;

		i = simple_strtoul(sep, NULL, 10);

		if(i)
			g_logm_path_ctrl = 1;
		else
			g_logm_path_ctrl = 0;
	}
	else if(!strncmp(name, "color", strlen("color")))
	{
		unsigned long i;

		i = simple_strtoul(sep, NULL, 10);

		if(i)
			g_logm_log_color = 1;
		else
			g_logm_log_color = 0;
	}
	else
	{
		int fd;
		uint32_t i;
		int sep_size = size - (sep - str);

		ret = RET_ERROR;
		LOGM_CHECK_ERROR(sep_size <= 0, goto func_exit, "fail get index\n");

		fd = LOGM_ObjFDGet(name);
		if(fd < 0)
		{
			fd = LOGM_ObjRegister(name);
			LOGM_CHECK_ERROR(fd < 0, goto func_exit, "fail fd_get\n");
		}

		for(i = 0; i < sep_size; i++)
			sep[i] = toupper(sep[i]);

		__obj_mask_write (&g_logm_obj_map[fd], sep, sep_size);

		ret = size;
	}

func_exit:
	vfree (str);
	return ret;
}

static int _LOGM_BitNameListInit(void)
{
	unsigned int size = sizeof(LX_LOGM_BIT_NAME_T) * LOGM_LOGOBJ_MAX;

	_g_logm_bit_name_list = (LX_LOGM_BIT_NAME_T *)vmalloc(size);
	LOGM_CHECK_ERROR(_g_logm_bit_name_list == NULL, return RET_ERROR,
			"fail vmalloc\n");
	memset(_g_logm_bit_name_list, 0, size);

	return 0;
}

static int _LOGM_BitNameListCleanup(void)
{
	int i,j;

	for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
		for(j = 0; j < LX_LOGM_LEVEL_MAX+1; j++)
			vfree(_g_logm_bit_name_list[i].name[j]);

	vfree(_g_logm_bit_name_list);
	return 0;
}

int LOGM_RegBitName(int fd, unsigned int bit, char *name)
{
	char *bit_name;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);

	if(bit > 31 || bit <= LX_LOGM_LEVEL_TRACE)
	{
		error("out of range. bit = %d\n", bit);
		return -1;
	}

	LOGM_CHECK_ERROR(name == NULL, return RET_ERROR,
			"name is NULL\n");

	bit_name = (char*)vmalloc(strlen(name) + 1);
	LOGM_CHECK_ERROR(bit_name == NULL, return RET_ERROR,
			"fail vmalloc\n");

	strncpy(bit_name, name, strlen(name) + 1);

	if(_g_logm_bit_name_list[fd].name[bit])
		vfree(_g_logm_bit_name_list[fd].name[bit]);

	_g_logm_bit_name_list[fd].name[bit] = bit_name;

	//TODO : where vfree bit_name ??

	return 0;
}
EXPORT_SYMBOL(LOGM_RegBitName);

int LOGM_GetBitName(int fd, struct logm_obj_bit_name *bit_name)
{
	int i;

	LOGM_CHECK_ERROR((fd < 0 || fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", fd);
	LOGM_CHECK_ERROR(bit_name == NULL, return RET_ERROR,
			"bit name is NULL\n");

	for(i = 0; i < LX_LOGM_LEVEL_MAX+1; i++)
	{
		if(_g_logm_bit_name_list[fd].name[i])
		{
			strncpy(bit_name->name[i], _g_logm_bit_name_list[fd].name[i], LOGM_MODULE_NAME_MAX);
			bit_name->name[i][LOGM_MODULE_NAME_MAX-1] = 0;
		}
		else
			memset(bit_name->name[i], 0, LOGM_MODULE_NAME_MAX);
	}

	return 0;
}
EXPORT_SYMBOL(LOGM_GetBitName);

/**
 * initialize proc utility for logm device
 *
 * @see LOGM_Init
*/
void	LOGM_PROC_Init (void)
{
    //make proc to control log module
	proc_entry = proc_mkdir ("logm", NULL);
	proc_create ("status", 0440, proc_entry,
			&proc_status_fops);
	proc_create ("lmsg", 0440, proc_entry,
			&proc_lmsg_fops);
	proc_create ("lmdump", 0440, proc_entry,
			&proc_lmdump_fops);

	_LOGM_BitNameListInit();
}

/**
 * cleanup proc utility for logm device
 *
 * @see LOGM_Cleanup
*/
void	LOGM_PROC_Cleanup (void)
{
    //delete proc_entry
	remove_proc_entry ("lmdump", proc_entry);
	remove_proc_entry ("lmsg", proc_entry);
	remove_proc_entry ("status", proc_entry);
	remove_proc_entry ("logm", NULL);

	_LOGM_BitNameListCleanup();
}

static void _LOGM_CreateObjProcBH (struct work_struct *work)
{
	unsigned long flag;
	struct _LOGM_ObjProcs *now;

	spin_lock_irqsave (&_LOGM_ObjProcsRootLock, flag);

	while (_LOGM_ObjProcsRoot.next)
	{
		now = _LOGM_ObjProcsRoot.next;

		_LOGM_ObjProcsRoot.next = now->next;
		spin_unlock_irqrestore (&_LOGM_ObjProcsRootLock, flag);

		debug ("make proc for %s\n", now->name);

		proc_create_data(now->name, 0640, proc_entry,
				&proc_fops, now->obj);
		
		//TODO: need lock? (g_logm_obj_map_lock)
		now->obj->has_proc = ON;
		kfree (now->name);
		kfree (now);


		spin_lock_irqsave (&_LOGM_ObjProcsRootLock, flag);
	}

	spin_unlock_irqrestore (&_LOGM_ObjProcsRootLock, flag);
}


/**
 * create proc file to control mask of obj
 */
void LOGM_CreateObjProc(const char *name, LX_LOGM_LOGOBJ_T *obj)
{
	if(!in_interrupt())
	{
		proc_create_data(name, 0640, proc_entry,
				&proc_fops, obj);

		//TODO: need lock? (g_logm_obj_map_lock)
		obj->has_proc = ON;
	}
	else
	{
		unsigned long flag;
		struct _LOGM_ObjProcs *new;

		debug ("queue work for proc create in irq, %s\n", name);

		new = kmalloc (sizeof (struct _LOGM_ObjProcs), GFP_ATOMIC);
		if (!new)
		{
			error ("no memory for proc, %s\n", name);
			return;
		}
		
		//TODO: static size name ?, 
		new->name = kmalloc (strlen (name)+1, GFP_ATOMIC);
		if (!new->name)
		{
			error ("no memory for name, %s\n", name);
			kfree (new);
			return;
		}
		strcpy (new->name, name);;
		new->obj = obj;

		spin_lock_irqsave (&_LOGM_ObjProcsRootLock, flag);
		new->next = _LOGM_ObjProcsRoot.next;
		_LOGM_ObjProcsRoot.next = new;
		spin_unlock_irqrestore (&_LOGM_ObjProcsRootLock, flag);

		queue_work (system_wq, &_LOGM_ProcWork);
	}
}

static int _LOGM_ProcStatusShow (struct seq_file *s, void *data)
{
    SINT32 i,j;
    unsigned long flag;
    SINT32  filled;
    SINT32  buffer_head;
    SINT32  buffer_tail;
    SINT32  buffer_write_counter;
    SINT32  buffer_dropped;
	UINT32  log_level_cnt[LX_LOGM_LEVEL_TRACE+2];
	struct logm_buffer_ctrl *bc;


    for(i = 0; i < LOGM_BUFFER_NUMBER; i++)
    {
		bc = &g_logm_buffer_ctrl[i];

        spin_lock_irqsave(&bc->buffer_lock, flag);
        buffer_head = bc->buffer_head;
        buffer_tail = bc->buffer_tail;
        buffer_write_counter = bc->buffer_write_counter;
        buffer_dropped = bc->buffer_dropped;

		for(j = 0; j < ARRAY_SIZE(log_level_cnt); j++)
			log_level_cnt[j] = bc->log_level_cnt[j];
        spin_unlock_irqrestore(&bc->buffer_lock, flag);

        filled = buffer_head - buffer_tail;
        if (filled < 0)
            filled += LOGM_BUFFER_SIZE;

#define seq_print_status(v,f)	seq_printf (s, #v"="f"\n", v)
        seq_printf(s, "---------------[buf %d]---------------\n", i);
        seq_printf(s, "=====buf ctrl=====\n");
        seq_printf(s, "global=%d\n", g_logm_global_ctrl_buffer);
        seq_printf(s, "printk=%d\n", g_logm_path_ctrl);
		seq_printf(s, "color=%d\n", g_logm_log_color);
        seq_printf(s, "\n");
        seq_printf(s, "=====buffer status=====\n");
        seq_print_status (buffer_head, "%d");
        seq_print_status (buffer_tail, "%d");
        seq_print_status (LOGM_BUFFER_SIZE, "%d");
        seq_print_status (filled, "%d");
        seq_print_status (buffer_write_counter, "%d");
        seq_print_status (buffer_dropped, "%d");
        seq_printf(s, "\n");
        seq_printf(s, "=====log count=====\n");
		seq_printf(s, "error log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_ERROR]);
		seq_printf(s, "warn  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_WARNING]);
		seq_printf(s, "noti  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_NOTI]);
		seq_printf(s, "info  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_INFO]);
		seq_printf(s, "debug log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_DEBUG]);
		seq_printf(s, "trace log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_TRACE]);
		seq_printf(s, "other log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_TRACE+1]);
        seq_printf(s, "\n");
#undef seq_print_status
    }

	if(_g_logm_latest_num != 0)
	{
        seq_printf(s, "==lmsg ctrl==\n");
		seq_printf(s, "log latest num=%d\n", _g_logm_latest_num);
	}

	return 0;
}

static int _LOGM_ProcStatusOpen (struct inode *inode
                                        , struct file *file)
{
	return single_open (file, _LOGM_ProcStatusShow, NULL);
}

/**
 * read lmdump
 */
static ssize_t _LOGM_LmdumpRead(struct file *file,
		char __user *data, size_t size, loff_t *off)
{
	return 0;
}

/**
 * open lmdump
 */
static int _LOGM_LmdumpOpen(struct inode *inode
										, struct file *file)
{
    unsigned long flag;
	struct logm_buffer_ctrl *bc = &(g_logm_buffer_ctrl[0]);

	spin_lock_irqsave (&(bc->buffer_lock), flag);
	bc->buffer_lmdump_tail = bc->buffer_head;
	spin_unlock_irqrestore (&(bc->buffer_lock), flag);

	trace("open\n");

	return 0;
}

/**
 * close lmdump
 */
static int _LOGM_LmdumpClose(struct inode *inode
										, struct file *file)
{
	trace("close\n");

	return 0;
}

/**
 * write count to show log
 */
static ssize_t _LOGM_ProcLMsgWrite(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int ret = RET_ERROR;
	char *kbuf;
	
	kbuf = (char *)kmalloc(size, GFP_KERNEL);
	LOGM_CHECK_ERROR(kbuf == NULL, return -1, "fail kmalloc\n");

	if(copy_from_user(kbuf, buf, size))
	{
		error ("copy_from_user failed\n");
		ret = -EFAULT;
		goto func_exit;
	}
	kbuf[size-1] = '\0';

	_g_logm_latest_num = simple_strtoul(kbuf, NULL, 10);
	ret = size;
func_exit:
	kfree(kbuf);
	return ret;
}

/**
 * show logm buffer(similar dmesg)
 */
static int _LOGM_ProcLMsgShow(struct seq_file *s, void *data)
{
	int ret = RET_ERROR;
	unsigned long flag;
	SINT32 	b_head;
	SINT32 	b_tail;
	SINT32  b_lmsg_tail;
	SINT32 	b_wrap;
	UINT32  b_wr_cnt;
	char 	*b_buf;
	struct 	logm_buffer_ctrl *bc = &(g_logm_buffer_ctrl[0]);

	b_buf = (char *)vmalloc(LOGM_BUFFER_SIZE);
	LOGM_CHECK_ERROR(b_buf == NULL, return -1, "fail vmalloc\n");

	//get info logm buffer
	spin_lock_irqsave (&(bc->buffer_lock), flag);
	b_wrap = bc->buffer_wrap_around;
	b_head = bc->buffer_head;
	b_lmsg_tail = bc->buffer_lmsg_tail;
	b_wr_cnt = bc->buffer_write_counter;
	memcpy(b_buf, bc->logm_buf, LOGM_BUFFER_SIZE);
	spin_unlock_irqrestore (&(bc->buffer_lock), flag);

	if(b_wrap == 0)//if not wrap-around, read from 0 to head
	{
		if(b_head)//not empty
			b_tail = 0;
		else
		{
			trace("logm buffer empty!\n");
			ret = RET_OK;
			goto func_exit;
		}
	}
	else//wrap-around
	{
		b_tail = b_lmsg_tail;

		if(b_tail < b_head)
		{
			error("invalid lmsg_tail tail %x head %x\n"
									,b_tail, b_head);
			ret = RET_ERROR;
			goto func_exit;
		}
	}

	while(TRUE)
	{
		char 			*data;
		char 			*b_str = NULL;
		int32_t 		length;
		uint32_t 		nanosec_rem;
		uint64_t 		t;
		struct 			logm_header *_h;

		_h = (struct logm_header*)(b_buf + b_tail);
		length = _h->size;

		if(length > 0)
		{
			//print tail
			if(_g_logm_latest_num)
			{
				if((b_wr_cnt - _h->count) >= _g_logm_latest_num)
					goto no_print;
			}
			
			//print header
			t = _h->clock;
			nanosec_rem = do_div(t, 1000000000);
			seq_printf(s, "[%5lu.%06u] " LOGM_NAME_LEVEL_FORMAT " (%05d:%05d) "
					, (unsigned long)t, nanosec_rem/1000
					, _h->obj->name
					, LOGM_LevelName(_h->level)
					, (_h->ptid >> 16) & 0xffff
					, _h->ptid & 0xffff);

			//print string	
			b_str = (char *)vmalloc(length+1);
			LOGM_CHECK_ERROR(b_str == NULL, ret = RET_ERROR; goto func_exit;
					, "fail kmalloc\n");
			data = (void *)_h + sizeof(struct logm_header);
			memcpy(b_str, data, length);
			b_str[length] = '\0';

			if(g_logm_log_color && _h->level == LX_LOGM_LEVEL_ERROR)
				seq_printf(s, "%s", g_logm_clrStrings[DBG_COLOR_RED]);
			else if(g_logm_log_color &&_h->level == LX_LOGM_LEVEL_WARNING)
				seq_printf(s, "%s", g_logm_clrStrings[DBG_COLOR_PURPLE]);

			seq_printf(s, "%s", b_str);

			if(g_logm_log_color && (_h->level == LX_LOGM_LEVEL_ERROR
				|| _h->level == LX_LOGM_LEVEL_WARNING))
				seq_printf(s, "%s\n", g_logm_clrStrings[DBG_COLOR_NONE]);
			else
				seq_printf(s, "\n");

			vfree(b_str);
			b_str = NULL;
no_print:
			b_tail += ALIGN8(sizeof(struct logm_header) + length);
		}
		else if(b_wrap)// && length < 0//wrap-arround
		{
			b_tail = 0;
			b_wrap = 0;
			goto loop1;
		}
		else//error case
		{
			error("logm buffer error\n");
			ret = RET_ERROR;
			goto func_exit;
		}
		
loop1:
		if(!b_wrap && (b_tail >= b_head))
			break;

		if(unlikely(b_tail >= LOGM_BUFFER_SIZE))//error case
		{
			error("fail parse tail\n");
			ret = RET_ERROR;
			goto func_exit;
		}
	}
	ret = RET_OK;

func_exit:
	vfree(b_buf);
	return ret;
}

static int _LOGM_ProcLMsgOpen(struct inode *inode
										, struct file *file)
{
	return single_open(file, _LOGM_ProcLMsgShow, NULL);
}

static int _LOGM_ObjMaskOpen (struct inode *inode
        , struct file *file)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);

	return single_open (file, _LOGM_ObjMaskShow, dp->data);
#else
	return single_open (file, _LOGM_ObjMaskShow, PDE_DATA(inode));
#endif
}

static int _LOGM_ObjMaskShow (struct seq_file *s, void *data)
{
	LX_LOGM_LOGOBJ_T *obj = (LX_LOGM_LOGOBJ_T*)s->private;
	int a;
	char *sep = "";
	int i;
	UINT32 mask;
	UINT32 log_level_cnt[LX_LOGM_LEVEL_TRACE+2];
	unsigned long flag;
	int fd;

	spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	mask = obj->mask;
	for(i = 0; i < ARRAY_SIZE(log_level_cnt); i++)
		log_level_cnt[i] = obj->log_level_cnt[i];
	spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);

	seq_printf (s, "%s: ", obj->name);

	for (a=0; a<32; a++)
	{
		if (mask & (1<<a))
		{
			seq_printf (s, "%s%s", sep, LOGM_LevelName(a));
			sep = ", ";
		}
	}
	seq_printf (s, "\n");

	fd = LOGM_ObjFDGet(obj->name);
	seq_printf(s, "==== %s custom bit name ====\n", obj->name);
	for(i = 0; i < 32; i++)
	{
		if(_g_logm_bit_name_list[fd].name[i] != NULL)
		{
			seq_printf(s, "B%02d : %s\n", i, _g_logm_bit_name_list[fd].name[i]);
		}
	}

	
    seq_printf(s, "=====log count=====\n");
	seq_printf(s, "error log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_ERROR]);
	seq_printf(s, "warn  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_WARNING]);
	seq_printf(s, "noti  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_NOTI]);
	seq_printf(s, "info  log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_INFO]);
	seq_printf(s, "debug log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_DEBUG]);
	seq_printf(s, "trace log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_TRACE]);
	seq_printf(s, "other log count=%u\n", log_level_cnt[LX_LOGM_LEVEL_TRACE+1]);

	seq_printf(s, "\n");

	return 0;
}

static ssize_t _LOGM_ObjMaskWrite (struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	LX_LOGM_LOGOBJ_T *obj;
	int ret;
	char *kbuf;
	SINT32 i;

	obj = ((struct seq_file *)file->private_data)->private;

	kbuf = (char *)kmalloc(size, GFP_KERNEL);
	LOGM_CHECK_ERROR(kbuf == NULL, return -1, "fail kmalloc\n");

	if(copy_from_user(kbuf, buf, size))
	{
		error ("copy_from_user failed\n");
		ret = -EFAULT;
		goto func_exit;
	}
	kbuf[size-1] = '\0';

	for(i = 0; i < size; i++)
		kbuf[i] = toupper(kbuf[i]);

	ret = __obj_mask_write(obj, kbuf, size);
func_exit:
	kfree(kbuf);
	return ret;
}

/**
 * TODO: rewrite this function. easy to read
 */
static int __obj_mask_write (LX_LOGM_LOGOBJ_T *obj
        , const char *buf, int size)
{
	int cnt = 0;
	const char *p;
	unsigned int mask;
	int sep;
	enum logm_set_mask_type set = logm_set_mask;

	p = buf;

	mask = 0;
	sep = 1;

	while (cnt < size)
	{
		if (*p == '+')
		{
			__obj_mask_set (obj, set, mask);
			mask = 0;
			set = logm_add_mask;
			sep = 1;

			p ++;
			cnt ++;
		}
		else if (*p == '-')
		{
			__obj_mask_set (obj, set, mask);
			mask = 0;
			set = logm_del_mask;
			sep = 1;

			p ++;
			cnt ++;
		}
		else if (
				*p == ',' ||
				*p == ' ' ||
				*p == '\t' ||
				*p == '\n' ||
				*p == 0
			)
		{
			sep = 1;

			p ++;
			cnt ++;
		}
		else if (sep)
		{
			struct
			{
				const char *name;
				int len;
				LX_LOGM_LOGLEVEL_T level;
			} level_table[] =
			{
#define lt(s,n)	{#s, sizeof(#s), LX_LOGM_LEVEL_##n, }
				lt(ERROR, ERROR),
				lt(WARN, WARNING),
				lt(NOTI, NOTI),
				lt(INFO, INFO),
				lt(DEBUG, DEBUG),
				lt(TRACE, TRACE),

				/* simple form */
				lt(E, ERROR),
				lt(W, WARNING),
				lt(N, NOTI),
				lt(I, INFO),
				lt(D, DEBUG),
				lt(T, TRACE),
#undef lt
			};

			if (*p == 'B')
			{
				unsigned long bit;

				p++;
				cnt++;

				bit = simple_strtoul(p, NULL, 10);

				if(bit <= 9)
				{   
					p++;
					cnt++;
					mask |= 1 << bit;
					debug ("got it\n");
				}
				else if(bit <= 31)
				{   
					p += 2;
					cnt += 2;
					mask |= 1 << bit;
					debug ("got it\n");
				}
				else
				{   
					error ("unknown format.\n");
					return -1;
				}
			}
			else
			{
				int a;

				for (a=0; a<ARRAY_SIZE (level_table); a++)
				{
					int len;

					debug ("name %s, len %d\n",
							level_table[a].name,
							level_table[a].len);

					len = level_table[a].len-1;
					if (size-cnt < len)
					{
						debug ("no data\n");
						continue;
					}

					if (!strncmp (p,level_table[a].name, len))
					{
						debug ("got it\n");
						mask |= 1<<level_table[a].level;

						p += len;
						cnt += len;
						break;
					}
				}
				if (a == ARRAY_SIZE(level_table))
				{
					error ("unknown format. %02x, %c\n", *p, *p);
					return -EINVAL;
				}
			}

			sep = 0;
		}
		else
		{
			error ("unknown format. %02x, %c\n", *p, *p);
			return -EINVAL;
		}
	}

	__obj_mask_set (obj, set, mask);

	return size;
}

/**
 * TODO: rewrite this function. easy to read
 */
static void __obj_mask_set (LX_LOGM_LOGOBJ_T *obj
        , enum logm_set_mask_type type, unsigned int mask)
{
	unsigned long flag;
	int a, l;

	if (!mask)
		return;

    spin_lock_irqsave(&g_logm_obj_map_lock, flag);
	if (type == logm_add_mask)
	{
		debug ("%s: add %08x to %08x\n",
				obj->name, mask, obj->mask);
		obj->mask |= mask;
		goto func_exit;
	}

	if (type == logm_del_mask)
	{
		debug ("%s: del %08x from %08x\n",
				obj->name, mask, obj->mask);
		obj->mask &= ~mask;
		goto func_exit;
	}

	l = 0;
	for (a=0; a<32; a++)
		if (mask & (1<<a))
			l = a;

	debug ("%s: level %d\n", obj->name, l);

	for (a=31; a>l; a--)
		obj->mask &= ~(1<<a);

	for (a=l; a>=0; a--)
		obj->mask |= 1<<a;

	debug ("%s: mask %08x\n", obj->name, obj->mask);

func_exit:
    spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);
}

static int _LOGM_ObjMaskRelease (struct inode *inode
        , struct file *file)
{
	return single_release (inode, file);
}


/** @} */


