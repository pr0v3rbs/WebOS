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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "proc_util.h"
#include "debug_util.h"

#include "sdec_io.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define STR_BLOCK_LIST		"top,io,core_a,core_b"

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum
{
	PROC_ID_AUTHOR	= 0,
	PROC_ID_DELAY,
	PROC_ID_REG,
	PROC_ID_REG_DEV,
	PROC_ID_REG_BLOCK,
	PROC_ID_REG_OFFSET,
	PROC_ID_REG_VALUE,
	PROC_ID_REG_DUMP,
	PROC_ID_MAX,
};

enum
{
	PROC_REGDUMP_TOP		= 0x0001,

	PROC_REGDUMP_IO			= 0x0010,
	PROC_REGDUMP_CORE_A		= 0x0100,
	PROC_REGDUMP_CORE_B		= 0x0200,
	PROC_REGDUMP_CORE		= 0x0F00,
	PROC_REGDUMP_BLOCK_ALL	= 0x0FFF,

	PROC_REGDUMP_DEV_0		= 0x1000,
	PROC_REGDUMP_DEV_1		= 0x2000,
	PROC_REGDUMP_DEV_ALL	= 0xF000,

	PROC_REGDUMP_ALL		= 0xFFFF,
};

typedef enum
{
	PROC_REG_BLOCK_NONE,
	PROC_REG_BLOCK_TOP,
	PROC_REG_BLOCK_IO,
	PROC_REG_BLOCK_CORE_A,
	PROC_REG_BLOCK_CORE_B,
} SDEC_PROC_REG_BLOCK_T;


typedef struct
{
	UINT32					dev;
	SDEC_PROC_REG_BLOCK_T	block;
	UINT32					offset;
} SDEC_PROC_REG_CTRL_T;

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
static int regdump_open(struct inode *inode, struct file *file);
static int regdump_release(struct inode *inode, struct file *file);

static void reg_write_command(const char* command);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct file_operations proc_regdump_fops =
{
	.open		= regdump_open,
	.read		= seq_read,
	.llseek 	= seq_lseek,
	.release	= regdump_release,
};

static OS_PROC_DESC_TABLE_T	_g_sdec_device_proc_table[] =
{
	{ "author",			PROC_ID_AUTHOR ,	OS_PROC_FLAG_READ },
	{ "delay",			PROC_ID_DELAY,		OS_PROC_FLAG_WRITE },
	{ "reg",			PROC_ID_REG,		OS_PROC_FLAG_WRITE},
	{ "reg_dev",		PROC_ID_REG_DEV,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_block",		PROC_ID_REG_BLOCK,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_offset",		PROC_ID_REG_OFFSET,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "reg_value",		PROC_ID_REG_VALUE,	OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE},
	{ "regdump",		PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_BLOCK_ALL},
	{ "regdump_top",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_TOP},
	{ "regdump_io",		PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_IO},
	{ "regdump_core_a",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_CORE_A},
	{ "regdump_core_b",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD, &proc_regdump_fops, (void*)PROC_REGDUMP_CORE_B},
	{ "regdump_core",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_CORE},
	{ "regdump_all",	PROC_ID_REG_DUMP,	OS_PROC_FLAG_SEQRD,	&proc_regdump_fops, (void*)PROC_REGDUMP_ALL},
	{ NULL, 			PROC_ID_MAX		,	0 }
};

static struct
{
	const char 				*name;
	SDEC_PROC_REG_BLOCK_T	block;
} block_list[] =
{
	{"top",		PROC_REG_BLOCK_TOP},
	{"io",		PROC_REG_BLOCK_IO},
	{"core_a",	PROC_REG_BLOCK_CORE_A},
	{"core_b",	PROC_REG_BLOCK_CORE_B},

	{NULL,		PROC_REG_BLOCK_NONE},
};

static SDEC_PROC_REG_CTRL_T proc_reg_ctrl;

/*========================================================================================
	Implementation Group
========================================================================================*/

static SDEC_PROC_REG_BLOCK_T str_to_regblock(const char* name)
{
	int i;

	for(i=0; block_list[i].name != NULL; i++)
	{
		if(!strcmp(name, block_list[i].name))
			return block_list[i].block;
	}

	return PROC_REG_BLOCK_NONE;
}

static const char* regblock_to_str(SDEC_PROC_REG_BLOCK_T block)
{
	int i;

	for(i=0; block_list[i].name != NULL; i++)
	{
		if(block == block_list[i].block)
			return block_list[i].name;
	}
	return "none";
}

static LX_SDEC_BLOCK_T get_sdec_block(SDEC_PROC_REG_BLOCK_T reg_block)
{
	int num = proc_reg_ctrl.dev;

	switch(reg_block)
	{
		case PROC_REG_BLOCK_TOP:	return LX_SDEC_BLOCK_TOP;
		case PROC_REG_BLOCK_IO:		return (num == 1) ? LX_SDEC_BLOCK_CORE1_IO : LX_SDEC_BLOCK_IO;
		case PROC_REG_BLOCK_CORE_A:	return (num == 1) ? LX_SDEC_BLOCK_CORE1_CORE_A : LX_SDEC_BLOCK_CORE_A;
		case PROC_REG_BLOCK_CORE_B:	return (num == 1) ? LX_SDEC_BLOCK_CORE1_CORE_B : LX_SDEC_BLOCK_CORE_B;
		default:
			return LX_SDEC_BLOCK_NONE;
	}
}

static int reg_write(SDEC_PROC_REG_BLOCK_T reg_block, UINT32 offset, UINT32 value)
{
	ULONG base_addr;
	UINT32 max_size;
	LX_SDEC_BLOCK_T block;

	block = get_sdec_block(reg_block);
	if(block == LX_SDEC_BLOCK_NONE)
		return -1;

	base_addr = (ULONG)SDEC_IO_GetRegBaseAddr(block);
	max_size = SDEC_IO_GetRegSize(block);

	if(base_addr != 0 &&
		offset < max_size && (offset%4) == 0)
	{
		SDEC_WRITE32(base_addr + offset, value);
		return 0;
	}

	return -1;
}

static int reg_read(SDEC_PROC_REG_BLOCK_T reg_block, UINT32 offset, UINT32 *value)
{
	ULONG base_addr;
	UINT32 max_size;
	LX_SDEC_BLOCK_T block;

	block = get_sdec_block(reg_block);
	if(block == LX_SDEC_BLOCK_NONE)
		return -1;

	base_addr = (ULONG)SDEC_IO_GetRegBaseAddr(block);
	max_size = SDEC_IO_GetRegSize(block);

	if(base_addr != 0 &&
		offset < max_size && (offset%4) == 0)
	{
		*value = SDEC_READ32(base_addr + offset);
		return 0;
	}

	return -1;
}


/*
 * read_proc implementation of sdec device
 *
*/
static int	_SDEC_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "jinhwan.bae (jinhwan.bae@lge.com)" );
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
 * write_proc implementation of sdec device
 *
*/
static int _SDEC_WriteProcFunction( UINT32 procId, char* command )
{
	LX_SDEC_CFG_T *cfg;
	UINT32 val;
	int rc;

	switch( procId )
	{
		case PROC_ID_DELAY:
		{
			val = simple_strtoul( command, (char**)NULL, 0);
			OS_MsecSleep(val);
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

			cfg = SDEC_CFG_GetConfig();
			if(val < cfg->nCore)
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
			SDEC_PROC_REG_BLOCK_T block = PROC_REG_BLOCK_NONE;

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
 * initialize proc utility for sdec device
 *
 * @see SDEC_Init
*/
void	SDEC_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( SDEC_MODULE, _g_sdec_device_proc_table,
											_SDEC_ReadProcFunction,
											_SDEC_WriteProcFunction );
}

/**
 * cleanup proc utility for sdec device
 *
 * @see SDEC_Cleanup
*/
void	SDEC_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( SDEC_MODULE );
}




static void reg_write_command(const char* command)
{
	int rc;
	char name[32];
	UINT32 offset, value;
	SDEC_PROC_REG_BLOCK_T block;

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




/** REG DUMP **/
static void regdump_display(const char *title, struct seq_file *m, LX_SDEC_BLOCK_T block)
{
	UINT32 max_size;
	ULONG base_addr;
	UINT32 offset, value;

	base_addr = (ULONG)SDEC_IO_GetRegBaseAddr(block);
	max_size = SDEC_IO_GetRegSize(block);

	seq_printf(m, "%s", title);
	for(offset=0; offset<max_size; offset+=4)
	{
		value = SDEC_READ32(base_addr + offset);
		if((offset % 16) == 0)	seq_printf(m, "\n%04x : ", offset);
		seq_printf(m, "%08x ", value);
	}
	seq_printf(m, "\n\n");
}

static void regdump_show_n(struct seq_file *m, int n, unsigned long flag)
{
	char name[16];

	if(flag&PROC_REGDUMP_IO)
	{
		sprintf(name, "SDEC%d SDIO", n);
		regdump_display(name, m, (n==0) ? LX_SDEC_BLOCK_IO : LX_SDEC_BLOCK_CORE1_IO);
	}

	if(flag&PROC_REGDUMP_CORE)
	{
		if(flag&PROC_REGDUMP_CORE_A)
		{
			sprintf(name, "SDEC%d CORE A", n);
			regdump_display(name, m, (n==0) ? LX_SDEC_BLOCK_CORE_A : LX_SDEC_BLOCK_CORE1_CORE_A);
		}

		if(flag&PROC_REGDUMP_CORE_B)
		{
			sprintf(name, "SDEC%d CORE B", n);
			regdump_display(name, m, (n==0) ? LX_SDEC_BLOCK_CORE_B : LX_SDEC_BLOCK_CORE1_CORE_B);
		}
	}
}

static int regdump_show (struct seq_file *m, void *data)
{
	unsigned long flag = (unsigned long)m->private;
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();

	flag |= ((proc_reg_ctrl.dev == 0) ? PROC_REGDUMP_DEV_0 : PROC_REGDUMP_DEV_1);

	if(flag&PROC_REGDUMP_TOP)
	{
		regdump_display("TOP", m, LX_SDEC_BLOCK_TOP);
	}

	if((flag&PROC_REGDUMP_DEV_0) && cfg->nCore > 0)
	{
		regdump_show_n(m, 0, flag);
	}

	if((flag&PROC_REGDUMP_DEV_1) && cfg->nCore > 1)
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

