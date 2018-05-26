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
 *  driver debug output utility
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.18
 *
 *  @addtogroup lg1150_base
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
#include "os_util.h"
#include "proc_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define ISR_BUFFER_SIZE			8192
#define	DBG_BUFSZ			 	160

#define NONE_MASK				0x00
#define CRNL_MASK           	0x01        // Append newline at the end of line
#define HEAD_MASK				0x02        // Prepend task name header
#define TIME_MASK				0x04        // Prepend time information header
#define AUTO_MASK				0x08        // Automatic appending task/tim header
#define ISRP_MASK				0x10        // Append Isr Buffer
#define OSDP_MASK				0x20        // OSD Print Flag

#define DBG_CRNL            	"\n"

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	char		module_name[LX_MAX_DEVICE_NAME];

	UINT32		is_opened:1,	// module debug is opened or not
				is_enabled:1,	// module debug is enabled or not
				rsvd:30;

	UINT32		print_mask;		// N indepdendent print mask for module
	UINT8		color[LX_MAX_MODULE_DEBUG_NUM];	// color attribute for print mask
}
DBG_MANAGE_TABLE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int			g_global_debug_fd;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#if 0//default LOGM
static int _OS_DEBUG_UartOutput(const char* module_name, const DBG_COLOR_T color, const int option, const char* headtext, const char *format, va_list arg );
extern char *simple_strtok(char *s, const char *delim, char **save_ptr);
#endif

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#if 0//default LOGM
static OS_SEM_T					_g_debug_sem;
static OS_SEM_T					_g_error_sem;
static DBG_OUTPUT_FUNCPTR		_g_uart_func			= NULL;
static DBG_MANAGE_TABLE_T*		_g_debug_manage_table	= NULL;
static DBG_LEVEL_T				_g_debug_global_level	= DBG_LEVEL_OFF;
static BOOLEAN 					_g_debug_suppress_error = FALSE;
static char*					_g_debug_level_string[]	=
{
	"Off", "ERR", "WRN", "Log"
};

static char	*clrStrings[] =
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

static const char _g_sep_text[] = "===================================================================================================";
#endif//default LOGM

/*========================================================================================
	Implementation Group
========================================================================================*/

/** @name Function Definition for Debug Utility
 *	function list
 *
 *	@{
*/

/**
 * Initialize debug utility system
 *
 * @see base_device_init
 */
#if 1//default LOGM
void OS_DEBUG_Init(void)
{
	g_global_debug_fd = DBG_OPEN( "kdrvcore" );
	if ( g_global_debug_fd >= 0 )
	{
		LOGM_ObjBitMaskEnable(g_global_debug_fd, LX_LOGM_LEVEL_ERROR);
		LOGM_ObjBitMaskEnable(g_global_debug_fd, LX_LOGM_LEVEL_WARNING);
		LOGM_ObjBitMaskEnable(g_global_debug_fd, LX_LOGM_LEVEL_NOTI);
	}
}

void	OS_DEBUG_Cleanup ( void )
{
	DBG_CLOSE(g_global_debug_fd);
}

#else

void	OS_DEBUG_Init	( void )
{
	_g_debug_manage_table = NULL;

	OS_InitMutex(&_g_debug_sem, OS_SEM_ATTR_DEFAULT);
	OS_InitMutex(&_g_error_sem, OS_SEM_ATTR_DEFAULT);

	/* create debug management structure */
	_g_debug_manage_table = (DBG_MANAGE_TABLE_T*)kmalloc( sizeof(DBG_MANAGE_TABLE_T)* LX_MAX_DEVICE_NUM, GFP_KERNEL );
	memset( _g_debug_manage_table, 0x0, sizeof(DBG_MANAGE_TABLE_T)*LX_MAX_DEVICE_NUM );

	/* Initialize global level
	 * TODO: use some initial value from user
	 */
	_g_debug_global_level = DBG_LEVEL_DBG;
	_g_uart_func		  = printk;

	/* make the default debug output ( index 0 )
	 * default debug output has empty module name ( this makes output more cutty ? )
	 * TODO: use some initial value from user.
	 */
	g_global_debug_fd = OS_DEBUG_OpenModule ( "kdrvcore" );
	if ( g_global_debug_fd >= 0 )
	{
		OS_DEBUG_EnableModule ( g_global_debug_fd );
		OS_DEBUG_EnableModuleByIndex ( g_global_debug_fd, DEBUG_IDX_COMMON_PRINT, DBG_COLOR_NONE );
		OS_DEBUG_DisableModuleByIndex( g_global_debug_fd, DEBUG_IDX_OSUTIL_PRINT );
//		OS_DEBUG_EnableModuleByIndex ( g_global_debug_fd, OS_DEBUG_OS_PRINT_IDX, DBG_COLOR_YELLOW );

#ifdef  KDRV_OS_DEBUG
 	   OS_DEBUG_EnableModuleByIndex( g_global_debug_fd, DEBUG_IDX_OSUTIL_PRINT, DBG_COLOR_GREEN );
#endif

	}
	else
	{
//		printk("can't open kdrvcore debug object !!\n" );
	}
}

/**
 * cleanup debug utility system
 *
 * @see base_device_cleanup
 */
void	OS_DEBUG_Cleanup ( void )
{
	kfree ( _g_debug_manage_table );
}

/**
 * create debug id for module
 * module should call this function before calling any debug output function.
 *
 * by default, debug output is disabled at open time. so you should enable debug after opening
 * module debug.
 *
 * @see OS_DEBUG_EnableModule
 * @see OS_DEBUG_DisableModule
 *
 */
int			OS_DEBUG_OpenModule( const char* module_name )
{
	int		i;
	int		ret;
	int		fd = -1;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't open debug for (%s) \n", module_name);
		fd = -1; goto function_exit;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	for ( i=0; i< LX_MAX_DEVICE_NUM; i++ )
	{
		if ( FALSE == _g_debug_manage_table[i].is_opened )
		{
			fd = i; break;
		}
	}

	if ( fd < 0 )
	{
		DBG_PRINT_ERROR("debug slot exhausted. can't open debug for (%s) \n", module_name);
		goto function_exit;
	}

	/* make default sctructure for module debug
	 *
	 * by default, module debug is disabled at open time. so you should enable debug
	 * after opening module debug.
	 */
	snprintf( _g_debug_manage_table[fd].module_name, LX_MAX_DEVICE_NAME, module_name );
	_g_debug_manage_table[fd].is_opened  = TRUE;
	_g_debug_manage_table[fd].is_enabled = FALSE;
	_g_debug_manage_table[fd].print_mask = 0x00;

	for (i=0; i< LX_MAX_MODULE_DEBUG_NUM; i++ )
		_g_debug_manage_table[fd].color[i]	= DBG_COLOR_NONE;

function_exit:
	OS_UnlockMutex(&_g_debug_sem);
	return fd;
}
EXPORT_SYMBOL(OS_DEBUG_OpenModule);

/**
 * close debug id
 *
 */
void			OS_DEBUG_CloseModule( int module_fd )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't close debug for (%d) \n", module_fd );
		return;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		memset( &_g_debug_manage_table[module_fd], 0x0, sizeof(DBG_MANAGE_TABLE_T));
	}

	OS_UnlockMutex(&_g_debug_sem);
}

/**
 * enable debug output
 */
void			OS_DEBUG_EnableModule( int module_fd )
{
	int	ret;

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		_g_debug_manage_table[module_fd].is_enabled = TRUE;
	}

	OS_UnlockMutex(&_g_debug_sem);
}
EXPORT_SYMBOL(OS_DEBUG_EnableModule);

/**
 * disable debug output
 */
void			OS_DEBUG_DisableModule( int module_fd )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		_g_debug_manage_table[module_fd].is_enabled = FALSE;
	}

	OS_UnlockMutex(&_g_debug_sem);
}

/**
 * set module print mask
 *
 */
void			OS_DEBUG_SetModuleMask( int module_fd, UINT32 mask )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		_g_debug_manage_table[module_fd].print_mask = mask;
	}

	OS_UnlockMutex(&_g_debug_sem);
}

/**
 *  get module print mask
 *
 */
UINT32		OS_DEBUG_GetModuleMask( int module_fd )
{
	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return 0x0;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return 0x0;
	}

	return 	_g_debug_manage_table[module_fd].print_mask;
}

/**
 * enable module debug output by index
 *
 */
void		OS_DEBUG_EnableModuleByIndex( int module_fd, int index, DBG_COLOR_T color )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return;
	}

	if ( index >= LX_MAX_MODULE_DEBUG_NUM )
	{
		DBG_PRINT_WARNING("out of range. index=%d\n", index);
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		_g_debug_manage_table[module_fd].print_mask 	|= (1<<index);
		_g_debug_manage_table[module_fd].color[index] 	 = (UINT8)color;
	}

	OS_UnlockMutex(&_g_debug_sem);
}
EXPORT_SYMBOL(OS_DEBUG_EnableModuleByIndex);

/**
 * disable module debug output by index
 *
 */
void		OS_DEBUG_DisableModuleByIndex( int module_fd, int index )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't control module (%d) \n", module_fd );
		return;
	}

	if ( module_fd < 0 || module_fd >= LX_MAX_DEVICE_NUM )
	{
		DBG_PRINT_WARNING("out of range. fd=%d\n", module_fd);
		return;
	}

	if ( index >= LX_MAX_MODULE_DEBUG_NUM )
	{
		DBG_PRINT_WARNING("out of range. index=%d\n", index);
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	if ( _g_debug_manage_table[module_fd].is_opened )
	{
		_g_debug_manage_table[module_fd].print_mask 	&= ~(1<<index);
		_g_debug_manage_table[module_fd].color[index] 	 = DBG_COLOR_NONE;
	}

	OS_UnlockMutex(&_g_debug_sem);
}

/**
 * set new global debug level
 */
void			OS_DEBUG_SetGlobalLevel ( DBG_LEVEL_T level )
{
	int	ret;

	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't set level (%d) \n", level );
		return;
	}

	if ( level < DBG_LEVEL_OFF || level > DBG_LEVEL_ALL )
	{
		DBG_PRINT_WARNING("out of range. level=%d\n", level );
		return;
	}

	ret = OS_LockMutex(&_g_debug_sem);

	_g_debug_global_level = level;

	OS_UnlockMutex(&_g_debug_sem);
}

/**
 * get current global debug level
 *
 */
DBG_LEVEL_T 	OS_DEBUG_GetGlobalLevel ( void )
{
	if ( !_g_debug_manage_table )
	{
		DBG_PRINT_ERROR("debug system shutdowned. can't get level\n");
		return DBG_LEVEL_OFF;
	}

	return 	_g_debug_global_level;
}

/**
 * debug output function for module
 *
 */
void			OS_DEBUG_Print_Tag( const char* szFunc, const int nLine, const int module_fd, int index, const char *format, ...)
{
    #define BUFF_SZ     64
    char            	headtext[BUFF_SZ];
    char*				textptr;
    va_list         	arg;

	/* if debug system is shutdown, abort print */
	if(	!_g_debug_manage_table ) return;

	/* if global debug level is not allowed, abort print */
	if( _g_debug_global_level < DBG_LEVEL_DBG )	return;

	/* if module is not allocated nor enabled, abort print */
	if( !_g_debug_manage_table[module_fd].is_opened )   return;
	if( !_g_debug_manage_table[module_fd].is_enabled ) return;

	/* if index is out of range, abort print */
	if( index >= LX_MAX_MODULE_DEBUG_NUM ) return;

	/* if index is set not to allow print, abort print */
	if( 0 == ( _g_debug_manage_table[module_fd].print_mask & (1<<index) ) ) return;

	if ( szFunc )
	{
		snprintf( headtext, BUFF_SZ, "%16s():%-4d] ", szFunc, nLine );
		textptr = headtext;
	}
	else
	{
		textptr = NULL;
	}

	/* call debug out */
    va_start (arg, format);
    _OS_DEBUG_UartOutput( _g_debug_manage_table[module_fd].module_name, _g_debug_manage_table[module_fd].color[index], NONE_MASK, textptr, format, arg);
    va_end(arg);
}
EXPORT_SYMBOL(OS_DEBUG_Print_Tag);

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * core print function for debug output
 *
 *
 */
static int _OS_DEBUG_UartOutput(const char* module_name, const DBG_COLOR_T color, const int option, const char* headtext, const char *format, va_list arg )
{
	char	dbgbuf[DBG_BUFSZ];
	int 	dbgcnt;
	UINT32	sec, msec;

#if defined(CONFIG_PRINTK_TIME)
	char	strbuf[DBG_BUFSZ];
	char	*line;
	int 	dbgcnt2;

	if (_g_debug_global_level == DBG_LEVEL_OFF) return 0;

	vsnprintf( strbuf, DBG_BUFSZ, format, arg );

	dbgcnt = 0;
	line = strbuf;
	while(*line != '\0')
	{
		dbgcnt2 = 0;

		/* if color option is enabled, add ansi color code */
		if ( color != DBG_COLOR_NONE )
		{
			dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%s", clrStrings[(int)color] );
		}

		/* if time option is enabled, add timing information */
		if ( option & TIME_MASK )
		{
			OS_GetCurrentTicks( &sec, &msec, NULL );
			dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%3d.%03d|", sec, msec );
		}

		/* check module name is avaiable */
		if ( module_name && module_name[0] )
		{
			dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%-7s] ", module_name );
		}
		else
		{
			dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%-7s] ", (char*)"");
		}

		/* add main output body */
		if (headtext)
		{
			dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%-20s", headtext );
		}

		while(1)
		{
			char c = *line;
			if(c == '\0' || c == '\n')
			{
				/* add ansi color reset */
				if ( color != DBG_COLOR_NONE )
					dbgcnt2 += snprintf( dbgbuf+dbgcnt2, DBG_BUFSZ-dbgcnt2, "%s", clrStrings[DBG_COLOR_NONE] );

				if(c == '\0') break;
			}
			dbgbuf[dbgcnt2++] = c;
			line++;

			if(c == '\n' || dbgcnt2 == (DBG_BUFSZ-1)) break;
		}
		dbgbuf[dbgcnt2] = '\0';

		// TODO: LOCK UART ????

		_g_uart_func( "%s", dbgbuf );

		// TODO: UNLOCK UART ????

		dbgcnt += dbgcnt2;
	}
#else
	if (_g_debug_global_level == DBG_LEVEL_OFF) return 0;

	dbgcnt = 0;

	/* if color option is enabled, add ansi color code */
	if ( color != DBG_COLOR_NONE )
	{
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%s", clrStrings[(int)color] );
	}

	/* if time option is enabled, add timing information */
	if ( option & TIME_MASK )
	{
	 OS_GetCurrentTicks( &sec, &msec, NULL );
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%3d.%03d|", sec, msec );
	}

	/* check module name is avaiable */
	if ( module_name && module_name[0] )
	{
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%-7s] ", module_name );
	}
	else
	{
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%-7s] ", (char*)"");
	}

	/* add main output body */
	if (headtext)
	{
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%-20s", headtext );
	}

	/* add main output body */
	dbgcnt += vsnprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, format, arg );

	/* add ansi color reset */
	if ( color != DBG_COLOR_NONE )
	{
	 dbgcnt += snprintf( dbgbuf+dbgcnt, DBG_BUFSZ-dbgcnt, "%s", clrStrings[DBG_COLOR_NONE] );
	}

	// TODO: LOCK UART ????

	_g_uart_func( "%s", dbgbuf );

	// TODO: UNLOCK UART ????
#endif

	return dbgcnt;
}


//  Null debug print utility
int _OS_DEBUG_UartNull(const char *format , ... )
{
    return(0);
}

/** print debug message for Critial Error, Warning, Assertion Error
 *
 *  @author bk1472
 *  @param szFile [IN] use __FILE__
 *  @param szFunction [IN] use __FUNCTION__
 *  @param nLine [IN] use __LINE__
 *  @param format [IN] format control.
 *  @return void.
 */
void			OS_DEBUG_PrintError_Tag(const DBG_LEVEL_T level, const char *szFile, const char *szFunc, const int nLine, const char *format, ...)
{
	va_list		arg;
    char    	dbgbuf[DBG_BUFSZ];

    if(_g_debug_global_level == DBG_LEVEL_OFF)	return;
    if(_g_debug_suppress_error == TRUE)			return;

    _g_debug_suppress_error = TRUE;

    va_start(arg, format);
    vsnprintf(dbgbuf, DBG_BUFSZ, format,arg);
    va_end(arg);

	/* make beautiful output for warning/error */
	_g_uart_func("%s", clrStrings[DBG_COLOR_RED] );
	_g_uart_func("[%s] %s@%s:%d - ", _g_debug_level_string[level], szFunc, OS_ParseFileName(szFile), nLine );
	_g_uart_func("%s", dbgbuf );
	_g_uart_func("%s\n", clrStrings[DBG_COLOR_NONE] );

    _g_debug_suppress_error = FALSE;
}
EXPORT_SYMBOL(OS_DEBUG_PrintError_Tag);

/*========================================================================================
	Implementation Group ( Proc Utility )
========================================================================================*/
static char delim_line[]  = ";\t\n";
static char delim_field[] = " :\t\n";

int	_OS_DEBUG_DumpPrintMask ( char* buffer )
{
	int	i,j;
	int	len = 0;

	len += sprintf( buffer+len, "-------------------------------------------------------------------------------\n");
	len += sprintf( buffer+len, " GLOBAL DEBUG LEVEL = %s (%d)\n", _g_debug_level_string[_g_debug_global_level], _g_debug_global_level );
	len += sprintf( buffer+len, "-------------------------------------------------------------------------------\n");
	len += sprintf( buffer+len, " ID    MODULE   FLAG    SUB DEBUG INFO\n");
	len += sprintf( buffer+len, "-------------------------------------------------------------------------------\n");

	for ( i=0; i<LX_MAX_DEVICE_NUM; i++ )
	{
		if ( _g_debug_manage_table[i].is_opened )
		{
			len += sprintf( buffer+len, "[%2d] %10s (%-3s) | ", i, _g_debug_manage_table[i].module_name, (_g_debug_manage_table[i].is_enabled)? "ON":"OFF" );

			for ( j=0; j<LX_MAX_MODULE_DEBUG_NUM;j++ )
			{
				len += sprintf( buffer+len, "[%d] %-3s %d ", j, (_g_debug_manage_table[i].print_mask & (1<<j))? "ON":"OFF",
																_g_debug_manage_table[i].color[j] );
			}
			len += sprintf( buffer+len, "\n");
		}
	}

	return len;
}

int		_OS_DEBUG_SetPrintMask	( char* config_data )
{
	int		i,j;
    char* line_str;
    char* line_tok;
    char* line_save_ptr;
    char* fld_tok;
    char* fld_sav_ptr;

    char  module_field_printmask;
    int	  module_field_printcolor;

	/* start parsing.. */
    for ( line_str = config_data;  ; line_str = NULL )
    {
		/* parsing line */
        line_tok = simple_strtok( line_str, delim_line, &line_save_ptr);

		/* check end of parsing */
        if ( line_tok == NULL) break;
        /* skip comment line */
        if ( line_tok[0] == '#' || strlen(line_tok) == 0 ) continue;

		/* gfx0:Y:Y-0:Y-1:Y-2:Y-3; */

		fld_tok = simple_strtok( line_tok, delim_field, &fld_sav_ptr);

		/* if module name is global ? */
		if (!strcmp( fld_tok, "global") )
		{
			fld_tok = simple_strtok( NULL, delim_field, &fld_sav_ptr);

			for ( i=0; i<= DBG_LEVEL_ALL; i++ )
			{
				if ( !strcmp( fld_tok, _g_debug_level_string[i] ) )
				{
					OS_DEBUG_SetGlobalLevel(i); break;
				}
			}

			continue;
		}

		for ( i=0; i<LX_MAX_DEVICE_NUM && _g_debug_manage_table[i].is_opened ; i++ )
		{
			if (strcmp( fld_tok, _g_debug_manage_table[i].module_name )) continue;

			/* get moduel print mask */
			fld_tok = simple_strtok( NULL, delim_field, &fld_sav_ptr);

			if ( !strcmp( fld_tok, "Y" ) )
			{
				OS_DEBUG_EnableModule( i );
			}
			else /* NO */
			{
				OS_DEBUG_DisableModule( i );
			}

			for ( j=0; j< LX_MAX_MODULE_DEBUG_NUM ; j++ )
			{
				/* get field print mask */
				fld_tok = simple_strtok( NULL, delim_field, &fld_sav_ptr);

				if (!fld_tok) break;

				sscanf( fld_tok, " %c-%d", &module_field_printmask, &module_field_printcolor );

				OS_DEBUG_EnableModuleByIndex( i, j, module_field_printcolor );

				if ( module_field_printmask == 'N' )
				{
					OS_DEBUG_DisableModuleByIndex( i, j );
				}
			}

			/* module is processed successfully. break inner loop */
			break;
		}
    }

	return RET_OK;
}

/** load config file and update print mask
 *
 *
 */
int		_OS_DEBUG_LoadPrintMask			( char* file_name )
{
	char*		config_data;
	int			file_size;
	OS_FILE_T	config_file;

	/* open config file and read */
	if ( RET_OK != OS_OpenFile( &config_file, file_name, O_CREAT | O_RDWR | O_LARGEFILE, 0666 ) )
	{
		printk("<error> can't open config_file (%s)\n", file_name );
		return RET_ERROR;
	}

	if ( ( file_size = OS_SizeOfFile( &config_file ) ) < 0 )
	{
		printk("<error> can't get file size (%s)\n", file_name );
		return RET_ERROR;
	}
	config_data = OS_KMalloc( file_size );
	OS_ReadFile (&config_file, config_data, file_size );
	OS_CloseFile( &config_file );

	(void)_OS_DEBUG_SetPrintMask( config_data );
	OS_KFree( config_data );

	return RET_OK;
}

/** save current print mask to config file
 *
 *
 */
int		_OS_DEBUG_SavePrintMask			( char* file_name )
{
	int			i,j;
	int			len;
	char*		buffer;
	OS_FILE_T	config_file;

	/* open config file */
	if ( RET_OK != OS_OpenFile( &config_file, file_name, O_CREAT | O_RDWR | O_LARGEFILE, 0666 ) )
	{
		printk("<error> can't open config_file (%s)\n", file_name );
		return RET_ERROR;
	}

	buffer = (char*)OS_KMalloc(1024);

	/* write global debug level */
	len = 0;
	len += sprintf( buffer+len, "#-------------------------------------------------------------------------------\n");
	len += sprintf( buffer+len, "# global debug mode\n");
	len += sprintf( buffer+len, "#-------------------------------------------------------------------------------\n");
	len += sprintf( buffer+len, "global:%s\n", _g_debug_level_string[_g_debug_global_level] );
	len += sprintf( buffer+len, "#-------------------------------------------------------------------------------\n");
	len += sprintf( buffer+len, "# module debug mode\n");
	len += sprintf( buffer+len, "#-------------------------------------------------------------------------------\n");
	OS_WriteFile( &config_file, buffer, len );

	/* write each debug level */

	for ( i=0; i<LX_MAX_DEVICE_NUM; i++ )
	{
		if ( _g_debug_manage_table[i].is_opened )
		{
			len = 0;
			len+= sprintf( buffer+len, "%s", _g_debug_manage_table[i].module_name );
			len+= sprintf( buffer+len, ":%c", (_g_debug_manage_table[i].is_enabled)? 'Y':'N' );

			for ( j=0; j<LX_MAX_MODULE_DEBUG_NUM;j++ )
			{
				len += sprintf( buffer+len, ":%c-%d", (_g_debug_manage_table[i].print_mask & (1<<j))? 'Y':'N',
														_g_debug_manage_table[i].color[j] );
			}
			len += sprintf( buffer+len, "\n");

			OS_WriteFile( &config_file, buffer, len );
		}
	}

	OS_CloseFile( &config_file );

	OS_KFree(buffer);

	return RET_OK;
}
#endif//default LOGM


/** @} */
/** @} */

