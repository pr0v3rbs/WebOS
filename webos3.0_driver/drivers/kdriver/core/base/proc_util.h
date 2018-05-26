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
 *	Linux proc file system utility
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.14
 *
 *  @addtogroup lg1150_base
 *	@{
 */

#ifndef	_PROC_UTIL_H_
#define	_PROC_UTIL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include "base_types.h"

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

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * Proc attribute flag for OS_PROC_DESC_TABLE_T.
 * READ/WRITE flag can be ORed.
 *
 * @see OS_PROC_DESC_TABLE_T
 */
enum
{
	OS_PROC_FLAG_READ		= 0x0001,	///< read attribute enabled
	OS_PROC_FLAG_WRITE		= 0x0002,	///< write attribute eanbled
	OS_PROC_FLAG_SEQRD		= 0x0010,	///< OS_PROC_DESC_TABLE_T.fop shall be used and proc_create shall be used.
	OS_PROC_FLAG_SEQRW		= 0x0020,	///< OS_PROC_DESC_TABLE_T.fop shall be used and proc_create shall be used.
	OS_PROC_FLAG_FOP		= OS_PROC_FLAG_SEQRD,
};

/**
 * Proc description table.
 * device driver using proc_util should define its own proc description.
 * by convention, OS_PROC_DESC_TABLE_t is defined at [module_name]_proc.c
 *
 * @code
 * static OS_PROC_DESC_TABLE_t	_g_template_device_proc_table[] =
 * {
 * 	 { "author",	PROC_ID_AUTHOR  , OS_PROC_FLAG_READ },
 * 	 { "command",	PROC_ID_COMMAND , OS_PROC_FLAG_WRITE},
 *   { "debug",     PROC_ID_DEBUG   , OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
 *   { "big_debug",	PROC_ID_BIG_DEBUG,OS_PROC_FLAG_READ | OS_PROC_FLAG_FOP },
 *   { "big_debug2",PROC_ID_BIG_2	, OS_PROC_FLAG_READ | OS_PROC_FLAG_FOP, &module_private_data },
 * 	 { NULL, 		PROC_ID_MAX		, 0 }
 * };
 * @endcode
 *
 * @see template_proc.c
*/
typedef struct
{
	char*	name;		///< proc name
	UINT32	id:16,		///< unique id used at device driver
			flag:16;	///< read/write/seq_file(fop) flag
	void*	fop;		///< if fop should be overred per-proc_entry base.(i.e. OS_PROC_FLAG_FOP )
	void*	data;		///< if OS_PROC_FLAG_FOP, proc_entry->data shall be set by this.
}
OS_PROC_DESC_TABLE_T;

#define	OS_PROC_DESC_END	{ NULL, PROC_ID_MAX, 0 }

/**
 * user-defined read proc function
 * proc_util checks basic error conditions, so you don't need to check any error.
 * basic usage is the same as linux read proc function.
 * you should write something to buf.
 *
 * @see OS_PROC_CreateEntryEx
*/
typedef int	(*OS_PROC_READ_FUNC_T)( UINT32 procId, char* buffer );

/**
 * user-defined write proc function
 * proc_util checks basic error conditions, so you don't need to check any error.
 * basic usage is the same as linux read proc function.
 * proc_util passes command which is null terminated string and you need to parse command string.
 *
 * @see OS_PROC_CreateEntryEx
*/
typedef int (*OS_PROC_WRITE_FUNC_T)( UINT32 procId, char* command );

/**
 simple macro to help define seq print with ease

 @code

 static void osd_reg_dump(struct seq_file* m)
 {
	seq_printf( m, "dump osd regs : 0x%08x\n", 0xc0035200 );
 }

 OS_PROC_SEQRD_DEFINE(g_reg_dump_proc_fops, osd_reg_dump);

 @endcode
 */
#define OS_PROC_SEQRD_DEFINE(name,readfn)  \
static int print_##name(struct seq_file* m, void* v) \
{ \
    readfn(m); return RET_OK;\
}\
static int open_##name(struct inode *n, struct file *fl)\
{\
    return single_open(fl, print_##name, NULL);\
}\
\
const static struct file_operations name = { \
    .open       = open_##name, \
    .read       = seq_read, \
    .llseek     = seq_lseek, \
    .release    = single_release, \
};

#define OS_PROC_SEQRD_DEFINE_EX(name,readfn)  \
static void readfn (struct seq_file *m); \
OS_PROC_SEQRD_DEFINE(name,readfn)


#define OS_PROC_SEQRW_DEFINE(name,readfn,writefn)  \
static int print_##name(struct seq_file* m, void* v) \
{ \
    readfn(m); return RET_OK;\
}\
static int open_##name(struct inode *n, struct file *fl)\
{\
    return single_open(fl, print_##name, NULL);\
}\
\
const static struct file_operations name = { \
    .open       = open_##name, \
    .read       = seq_read, \
	.write		= writefn,	\
    .llseek     = seq_lseek, \
    .release    = single_release, \
};

#define OS_PROC_SEQRW_DEFINE_EX(name,readfn,writefn)  \
static void readfn (struct seq_file *m); \
static ssize_t writefn (struct file *file, const char __user *buf, size_t size, loff_t *off); \
OS_PROC_SEQRW_DEFINE(name,readfn,writefn)

/** @} */

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void		OS_PROC_Init			( void );
extern	void		OS_PROC_Cleanup			( void );
extern int			OS_PROC_CreateEntryEx	( char*							base_name,
											  OS_PROC_DESC_TABLE_T*	pTable,
											  OS_PROC_READ_FUNC_T		read_function,
											  OS_PROC_WRITE_FUNC_T	write_function );

extern	int			OS_PROC_RemoveEntry		( char*					base_name );
extern	void		OS_PROC_FlushEntry		( void );

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BASE_DRV_H_ */

/** @} */

