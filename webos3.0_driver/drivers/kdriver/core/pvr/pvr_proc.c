/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  Linux proc interface for pvr device.
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		kyungbin.pak
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
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
#include <asm/uaccess.h>

#include "pvr_impl.h"
#include "pvr_core.h"
#include "pvr_reg.h"
#include "pvr_reg_ctrl.h"
#include "proc_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define STR_BLOCK_LIST		"top,pie,dn,up"

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_DELAY,
	PROC_ID_MEM_INFO,
	PROC_ID_STATUS,
	PROC_ID_UPSTATUS,
	PROC_ID_DNSTATUS,
	PROC_ID_REG,
	PROC_ID_REG_DEV,
	PROC_ID_REG_BLOCK,
	PROC_ID_REG_OFFSET,
	PROC_ID_REG_VALUE,
	PROC_ID_REG_DUMP,
	PROC_ID_MAX,
};

enum {
	PROC_STATUS_UP	= 0x1,
	PROC_STATUS_DN	= 0x2,
	PROC_STATUS_ALL	= 0x3,
};


enum
{
	PROC_REGDUMP_TOP		= 0x00001,

	PROC_REGDUMP_PIE		= 0x00010,
	PROC_REGDUMP_DN			= 0x00100,
	PROC_REGDUMP_UP			= 0x01000,
	PROC_REGDUMP_BLOCK_ALL	= 0x0FFFF,

	PROC_REGDUMP_DEV_0		= 0x10000,
	PROC_REGDUMP_DEV_1		= 0x20000,
	PROC_REGDUMP_DEV_ALL	= 0xF0000,

	PROC_REGDUMP_ALL		= 0xFFFFF,
};

typedef enum
{
	PROC_REG_BLOCK_NONE,
	PROC_REG_BLOCK_TOP,
	PROC_REG_BLOCK_PIE,
	PROC_REG_BLOCK_DN,
	PROC_REG_BLOCK_UP,
} PVR_PROC_REG_BLOCK_T;

typedef struct
{
	UINT32					dev;
	PVR_PROC_REG_BLOCK_T	block;
	UINT32					offset;
} PVR_PROC_REG_CTRL_T;


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
static int status_open(struct inode *inode, struct file *file);
static int status_release(struct inode *inode, struct file *file);

static int regdump_open(struct inode *inode, struct file *file);
static int regdump_release(struct inode *inode, struct file *file);

static void reg_write_command(const char* command);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct file_operations proc_status_fops =
{
	.open		= status_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= status_release,
};

static struct file_operations proc_regdump_fops =
{
	.open		= regdump_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= regdump_release,
};


static OS_PROC_DESC_TABLE_T	_g_pvr_device_proc_table[] =
{
	{ "author",			PROC_ID_AUTHOR  ,	OS_PROC_FLAG_READ },
	{ "delay",			PROC_ID_DELAY,		OS_PROC_FLAG_WRITE },
	{ "mem",			PROC_ID_MEM_INFO,	OS_PROC_FLAG_READ },
	{ "status",			PROC_ID_STATUS,		OS_PROC_FLAG_SEQRD,	&proc_status_fops, (void*)PROC_STATUS_ALL},
	{ "upstatus",		PROC_ID_UPSTATUS,	OS_PROC_FLAG_SEQRD,	&proc_status_fops, (void*)PROC_STATUS_UP},
	{ "dnstatus",		PROC_ID_DNSTATUS,	OS_PROC_FLAG_SEQRD,	&proc_status_fops, (void*)PROC_STATUS_DN},
	{ "reg",			PROC_ID_REG,		OS_PROC_FLAG_WRITE},
	{ "reg_dev",		PROC_ID_REG_DEV,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_block",		PROC_ID_REG_BLOCK,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_offset", 	PROC_ID_REG_OFFSET, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_value",		PROC_ID_REG_VALUE,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "regdump",		PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_BLOCK_ALL},
	{ "regdump_top",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_TOP},
	{ "regdump_pie",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_PIE},
	{ "regdump_pie_all",PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)(PROC_REGDUMP_PIE|PROC_REGDUMP_DEV_ALL)},
	{ "regdump_dn",		PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_DN},
	{ "regdump_dn_all",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)(PROC_REGDUMP_DN|PROC_REGDUMP_DEV_ALL)},
	{ "regdump_up",		PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_UP},
	{ "regdump_up_all", PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD, &proc_regdump_fops, (void*)(PROC_REGDUMP_UP|PROC_REGDUMP_DEV_ALL)},
	{ "regdump_all",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_ALL},
	{ NULL, 		PROC_ID_MAX,		0 }
};

static struct
{
	const char 				*name;
	PVR_PROC_REG_BLOCK_T	block;
} block_list[] =
{
	{"top",		PROC_REG_BLOCK_TOP},
	{"pie",		PROC_REG_BLOCK_PIE},
	{"dn",		PROC_REG_BLOCK_DN},
	{"up",		PROC_REG_BLOCK_UP},

	{NULL,		PROC_REG_BLOCK_NONE},
};


static PVR_PROC_REG_CTRL_T proc_reg_ctrl;


/*========================================================================================
	Implementation Group
========================================================================================*/
static PVR_PROC_REG_BLOCK_T str_to_regblock(const char* name)
{
	int i;

	for(i=0; block_list[i].name != NULL; i++)
	{
		if(!strcmp(name, block_list[i].name))
			return block_list[i].block;
	}

	return PROC_REG_BLOCK_NONE;
}

static const char* regblock_to_str(PVR_PROC_REG_BLOCK_T block)
{
	int i;

	for(i=0; block_list[i].name != NULL; i++)
	{
		if(block == block_list[i].block)
			return block_list[i].name;
	}
	return "none";
}

static LX_PVR_BLOCK_T get_pvr_block(PVR_PROC_REG_BLOCK_T reg_block)
{
	int num = proc_reg_ctrl.dev;

	switch(reg_block)
	{
		case PROC_REG_BLOCK_TOP:	return LX_PVR_BLOCK_TOP;
		case PROC_REG_BLOCK_PIE:	return (num == 1) ? LX_PVR_BLOCK_PIE_B : LX_PVR_BLOCK_PIE_A;
		case PROC_REG_BLOCK_DN:		return (num == 1) ? LX_PVR_BLOCK_DN_B : LX_PVR_BLOCK_DN_A;
		case PROC_REG_BLOCK_UP:		return (num == 1) ? LX_PVR_BLOCK_UP_B : LX_PVR_BLOCK_UP_A;
		default:
			return LX_PVR_BLOCK_NONE;
	}
}

static int reg_write(PVR_PROC_REG_BLOCK_T reg_block, UINT32 offset, UINT32 value)
{
	ULONG base_addr;
	UINT32 max_size;
	LX_PVR_BLOCK_T block;

	block = get_pvr_block(reg_block);
	if(block == LX_PVR_BLOCK_NONE)
		return -1;

	base_addr = (ULONG)PVR_GetRegBaseAddr(block);
	max_size = PVR_GetRegSize(block);

	if(base_addr != 0 &&
		offset < max_size && (offset%4) == 0)
	{
		PVR_REG_WRITE(base_addr + offset, value);
		return 0;
	}

	return -1;
}

static int reg_read(PVR_PROC_REG_BLOCK_T reg_block, UINT32 offset, UINT32 *value)
{
	ULONG base_addr;
	UINT32 max_size;
	LX_PVR_BLOCK_T block;

	block = get_pvr_block(reg_block);
	if(block == LX_PVR_BLOCK_NONE)
		return -1;

	base_addr = (ULONG)PVR_GetRegBaseAddr(block);
	max_size = PVR_GetRegSize(block);

	if(base_addr != 0 &&
		offset < max_size && (offset%4) == 0)
	{
		*value = PVR_REG_READ(base_addr + offset);
		return 0;
	}

	return -1;
}



/*
 * read_proc implementation of pvr device
 *
*/
static int	_PVR_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;
	int 	ch, n;

	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "kyungbin.pak" );
		}
		break;

		case PROC_ID_MEM_INFO:
		{
			LX_MEMCFG_T *mem_cfg;
			int len = 0;

			for(ch=0; ch<LX_PVR_CH_NUM; ch++)
			{
				mem_cfg = PVR_CFG_GetMemConfig(ch);
				len += sprintf(buffer + len, "CH_%c\n", 'A' + ch);
				for(n=0; n<LX_PVR_MEM_NUM; n++)
				{
					len += sprintf(buffer + len, "%12s : 0x%08x ++ 0x%08x\n",
						mem_cfg[n].name, mem_cfg[n].base, mem_cfg[n].size);
				}
			}
			ret = len;
		}
		break;

		case PROC_ID_REG_DEV:
		{
			ret = sprintf( buffer, "device:%d\n", proc_reg_ctrl.dev);
		}
		break;

		case PROC_ID_REG_BLOCK:
		{
			ret = sprintf(buffer, "block:%s\n", regblock_to_str(proc_reg_ctrl.block));
		}
		break;

		case PROC_ID_REG_OFFSET:
		{
			ret = sprintf(buffer, "offset:0x%x\n", proc_reg_ctrl.offset);
		}
		break;

		case PROC_ID_REG_VALUE:
		{
			UINT32 value = 0;
			reg_read(proc_reg_ctrl.block, proc_reg_ctrl.offset, &value);
			ret = sprintf(buffer, "0x%x\n", value);
		}
		break;


		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}

	return ret;
}



/*
 * write_proc implementation of pvr device
 *
*/
static int _PVR_WriteProcFunction( UINT32 procId, char* command )
{
	LX_PVR_CFG_T *cfg;
	UINT32 val;
	int rc;

	switch( procId )
	{
		case PROC_ID_DELAY:
		{
			unsigned long delay;
			delay = simple_strtoul( command, (char**)NULL, 0);
			OS_MsecSleep(delay);
		}
		break;

		case PROC_ID_REG:
		{
			reg_write_command(command);
		}
		break;

		case PROC_ID_REG_DEV:
		{
			val = (UINT32)simple_strtoul(command, (char**)NULL, 0);

			cfg = PVR_CFG_GetConfig();
			if(val < cfg->num_updev || val < cfg->num_dndev)
			{
				proc_reg_ctrl.dev = val;
			}
			else
			{
				printk(KERN_ERR "invalid num(%d)", val);
			}
		}
		break;

		case PROC_ID_REG_BLOCK:
		{
			char name[32];
			PVR_PROC_REG_BLOCK_T block = PROC_REG_BLOCK_NONE;

			rc = sscanf(command, "%s", name);
			if(rc == 1)
			{
				block = str_to_regblock(name);
			}

			if(block != PROC_REG_BLOCK_NONE)
			{
				proc_reg_ctrl.block = block;
			}
			else
			{
				printk(KERN_ERR "invalid name(%s). avail("STR_BLOCK_LIST")", name);
			}
		}
		break;

		case PROC_ID_REG_OFFSET:
		{
			val = simple_strtoul(command, (char**)NULL, 0);
			proc_reg_ctrl.offset = val;
		}
		break;

		case PROC_ID_REG_VALUE:
		{
			val = simple_strtoul(command, (char**)NULL, 16);	/* base 16 */
			reg_write(proc_reg_ctrl.block, proc_reg_ctrl.offset, val);
		}
		break;

		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}


/**
 * initialize proc utility for pvr device
 *
 * @see PVR_Init
*/
void	PVR_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( PVR_MODULE, _g_pvr_device_proc_table,
											_PVR_ReadProcFunction,
											_PVR_WriteProcFunction );
}

/**
 * cleanup proc utility for pvr device
 *
 * @see PVR_Cleanup
*/
void	PVR_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( PVR_MODULE );
}





/** PVR STATUS **/
static int status_show (struct seq_file *m, void *data)
{
	PVR_CTRL_T *ctrl;
	LX_PVR_CH_T lxch;
	UINT32 total, used;
	UINT32 wptr, rptr;
	unsigned long flag = (unsigned long)m->private;
	LX_PVR_CFG_T *cfg = PVR_CFG_GetConfig();

#define dn_status_string(x)	(x==LX_PVR_DN_STATE_RECORD) ? "record" : \
							(x==LX_PVR_DN_STATE_OVERFLOW) ? "overflow" : "idle"

#define up_status_string(x)	(x==LX_PVR_UP_STATE_PLAY) ? "play" : \
							(x==LX_PVR_UP_STATE_PAUSE) ? "pause" : "idle"


	seq_printf(m, "=========================\n");
	if(flag&PROC_STATUS_DN)
	{
		seq_printf(m, "DOWNLOAD\n");
		for(lxch=LX_PVR_CH_A; lxch<cfg->num_dndev; lxch++)
		{
			ctrl = PVR_GetCtrl(lxch);
			if(ctrl == NULL) continue;

			seq_printf(m, " ch_%c state : %s\n",'a'+lxch, dn_status_string(ctrl->state.eDnState));
			if(ctrl->state.eDnState != LX_PVR_DN_STATE_IDLE)
			{
				wptr = ctrl->buf.dn.wptr;
				rptr =  ctrl->buf.dn.rptr;

				total = ctrl->buf.dn.end - ctrl->buf.dn.base;
				used = (total + wptr - rptr) % total;

				seq_printf(m, "   range(0x%08x--0x%08x)\n", ctrl->buf.dn.base, ctrl->buf.dn.end);
				seq_printf(m, "   wptr:0x%08x, rptr:0x%08x\n", wptr, rptr);
				seq_printf(m, "   used:%d, total:%d\n", used, total);
			}
		}
		seq_printf(m, "\n");
	}

	if(flag&PROC_STATUS_UP)
	{
		seq_printf(m, "UPLOAD\n");
		for(lxch=LX_PVR_CH_A; lxch<cfg->num_updev; lxch++)
		{
			ctrl = PVR_GetCtrl(lxch);
			if(ctrl == NULL) continue;

			seq_printf(m, " ch_%c state : %s\n",'a'+lxch, up_status_string(ctrl->state.eUpState));
			if(ctrl->state.eUpState != LX_PVR_UP_STATE_IDLE)
			{
				DVR_UP_GetPointersReg(ctrl->ch, &wptr, &rptr);

				total = ctrl->buf.up.end - ctrl->buf.up.base;
				used = (total + wptr - rptr) % total;
				seq_printf(m, "   range(0x%08x--0x%08x)\n", ctrl->buf.up.base, ctrl->buf.up.end);
				seq_printf(m, "   wptr:0x%08x, rptr:0x%08x\n", wptr, rptr);
				seq_printf(m, "   used:%d, total:%d\n", used, total);
			}
		}
		seq_printf(m, "\n");
	}

	return 0;
}

static int status_open (struct inode *inode, struct file *file)
{
	unsigned long flag;
	int ret;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	flag = dp->data;
#else
	flag = (unsigned long)PDE_DATA(inode);
#endif

	ret = single_open (file, status_show, (void*)flag);

	return ret;
}

static int status_release (struct inode *inode, struct file *file)
{
	return single_release (inode, file);
}


/** REG DUMP **/
static void reg_write_command(const char* command)
{
	int rc;
	char name[32];
	UINT32 offset, value;
	PVR_PROC_REG_BLOCK_T block;

	rc = sscanf( command, "%s %x %x", name, &offset, &value);
	if(rc != 3) goto usage;

	block = str_to_regblock(name);
	if(block == PROC_REG_BLOCK_NONE) goto usage;

	reg_write(block, offset, value);

	return;

usage:
	printk(KERN_ERR "\n");
	printk(KERN_ERR "usage> block offset value\n");
	printk(KERN_ERR "block:"STR_BLOCK_LIST"\n");
}


static void regdump_display(const char *title, struct seq_file *m, LX_PVR_BLOCK_T block)
{
	UINT32 max_size;
	ULONG base_addr;
	UINT32 offset, value;

	base_addr = (ULONG)PVR_GetRegBaseAddr(block);
	max_size = PVR_GetRegSize(block);

	seq_printf(m, "%s", title);
	for(offset=0; offset<max_size; offset+=4)
	{
		value = PVR_REG_READ(base_addr + offset);
		if((offset % 16) == 0)	seq_printf(m, "\n%04x : ", offset);
		seq_printf(m, "%08x ", value);
	}
	seq_printf(m, "\n\n");
}

static void regdump_show_n(struct seq_file *m, int n, unsigned long flag)
{
	char name[16];
	LX_PVR_CFG_T *cfg = PVR_CFG_GetConfig();

	if(flag&PROC_REGDUMP_PIE)
	{
		sprintf(name, "PIE %c", (n==1) ? 'B' : 'A');
		regdump_display(name, m, (n==1) ? LX_PVR_BLOCK_PIE_B : LX_PVR_BLOCK_PIE_A);
	}

	if((flag&PROC_REGDUMP_DN) && n < cfg->num_dndev)
	{
		sprintf(name, "DN %c", (n==1) ? 'B' : 'A');
		regdump_display(name, m, (n==1) ? LX_PVR_BLOCK_DN_B : LX_PVR_BLOCK_DN_A);
	}

	if((flag&PROC_REGDUMP_UP) && n < cfg->num_updev)
	{
		sprintf(name, "UP %c", (n==1) ? 'B' : 'A');
		regdump_display(name, m, (n==1) ? LX_PVR_BLOCK_UP_B : LX_PVR_BLOCK_UP_A);
	}
}

static int regdump_show (struct seq_file *m, void *data)
{
	unsigned long flag = (unsigned long)m->private;

	flag |= ((proc_reg_ctrl.dev == 0) ? PROC_REGDUMP_DEV_0 : PROC_REGDUMP_DEV_1);

	if(flag&PROC_REGDUMP_TOP)
	{
		regdump_display("TOP", m, LX_PVR_BLOCK_TOP);
	}

	if(flag&PROC_REGDUMP_DEV_0)
	{
		regdump_show_n(m, 0, flag);
	}

	if(flag&PROC_REGDUMP_DEV_1)
	{
		regdump_show_n(m, 1, flag);
	}

	return 0;
}

static int regdump_open (struct inode *inode, struct file *file)
{
	unsigned long flag;
	int ret;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	flag = dp->data;
#else
	flag = (unsigned long)PDE_DATA(inode);
#endif

	ret = single_open (file, regdump_show, (void*)flag);

	return ret;
}

static int regdump_release (struct inode *inode, struct file *file)
{
	return single_release (inode, file);
}





/** @} */


