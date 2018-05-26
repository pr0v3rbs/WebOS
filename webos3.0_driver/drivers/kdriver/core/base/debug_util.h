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

#ifndef	_DEBUG_UTIL_H_
#define	_DEBUG_UTIL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
//#include <linux/proc_fs.h>
#include "base_types.h"

#include "logm_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/** @name Type Defininition for Debug Utility
 *  @brief type definitions for debug utility
 *
 * @{
 *
*/

/**
 *	Maximum number of debug output for each module.
*/
#define		LX_MAX_MODULE_DEBUG_NUM			16

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/**
 * _USE_PRINT_FORMAT_CHECK
 * 20100709 seokjoo.lee : strict format check for OS_DEGBUG Print* series.
 */
#ifndef _USE_PRINT_FORMAT_CHECK
#define _USE_PRINT_FORMAT_CHECK 1
#endif

#if (_USE_PRINT_FORMAT_CHECK > 0 )
#define __PRINTF(_a,_b) __attribute__ (( format (printf, _a, _b) ))
#else
#define __PRINTF(_a,_b)
#endif

/**
 *
 *
*/
#if 0//default LOGM

#define DBG_OPEN(module_name)								OS_DEBUG_OpenModule(module_name)
#define DBG_CLOSE(module_fd)								OS_DEBUG_CloseModule(module_fd)
#define DBG_PRINT_ERROR(format, args...)					OS_DEBUG_PrintError(format, ##args)
#define DBG_PRINT_WARNING(format, args...) 					OS_DEBUG_PrintWarning(format, ##args)

/**
 * print "time, module name and your debug text"
 *
 * Most module may want to use DBG_PRINT().
 *
 * Below is specific output of DBG_PRINT().
 *
 * \verbatim
 * 2524.020|template] template device initialized
 * \endverbatim
*/
#define	DBG_PRINT(module_fd,index,format, args...)			OS_DEBUG_Print(module_fd,index,format, ##args)

/**
 * print "time, module name, function, line and your debug text"
 *
 * since DBG_PRINTX() can print detail information about current execution, this macro can be
 * used as line-by-line trace :-)
 *
 * Below is specific output of DBG_PRINTX().
 *
 * \verbatim
 * 2524.341|template] template_device_init():211 ] template device initialized
 * \endverbatim
 *
*/
#define	DBG_PRINTX(module_fd,index,format, args...)			OS_DEBUG_PrintX(module_fd,index,format, ##args)

/**
 * DBG_MSG() is general-purpose debug output ( it doesn't need any register/open action )
 * So DBG_MSG() just prints current ticks and your debug text.
 *
 * This function can be used at simple trace or temporary tiny line-by-line debug.
 *
 * Below is specific output of DBG_MSG().
 *
 * \verbatim
 * 2524.321|        ] <!> debug  system initialized..
 * \endverbatim
 *
*/
#define	DBG_MSG(format, args...)							DBG_PRINT(0,DEBUG_IDX_COMMON_PRINT,format, ##args)
#define DBG_TRACE(format, args...)							DBG_PRINTX(0,DEBUG_IDX_COMMON_PRINT,format, ##args)

/*
 * DO NOT use in your code.
 * @see DBG_PRINT_ERROR
 */
#define OS_DEBUG_PrintError(format, args...)				OS_DEBUG_PrintError_Tag(DBG_LEVEL_ERR,__FILE__,__FUNCTION__,__LINE__,format, ##args)
/*
 * DO NOT use in your code.
 * @see DBG_PRINT_WARNING
 */
#define OS_DEBUG_PrintWarning(format, args...)		 		OS_DEBUG_PrintError_Tag(DBG_LEVEL_WRN,__FILE__,__FUNCTION__,__LINE__,format, ##args)
/*
 * DO NOT use in your code.
 * @see DBG_PRINT_PRINT
 */
#define	OS_DEBUG_Print(module_fd,index,format, args...)		OS_DEBUG_Print_Tag(NULL,0,module_fd,index,format, ##args)
/*
 * DO NOT use in your code.
 * @see DBG_PRINT_PRINTX
 */
#define	OS_DEBUG_PrintX(module_fd,index,format, args...)	OS_DEBUG_Print_Tag(__FUNCTION__,__LINE__,module_fd,index,format, ##args)

#else

#define DBG_OPEN(module_name) 		LOGM_ObjRegister(module_name)
#define DBG_CLOSE(module_fd)		LOGM_ObjRegister(g_logm_obj_map[module_fd].name)

#define DBG_PRINT_ERROR(format, args...)		LOGM_PRINT(g_global_debug_fd,LX_LOGM_LEVEL_ERROR,format, ##args)
#define DBG_PRINT_WARNING(format, args...) 		LOGM_PRINT(g_global_debug_fd,LX_LOGM_LEVEL_WARNING,format, ##args)

#define	DBG_PRINT(module_fd,index,format, args...)			LOGM_PRINT(module_fd,index,format, ##args)
#define	DBG_PRINTX(module_fd,index,format, args...)			LOGM_PRINT(module_fd,index,format, ##args)

#define	DBG_MSG(format, args...) 				LOGM_PRINT(g_global_debug_fd,LX_LOGM_LEVEL_NOTI,format, ##args)
#define DBG_TRACE(format, args...) 				LOGM_PRINT(g_global_debug_fd,LX_LOGM_LEVEL_TRACE,format, ##args)

#endif//default LOGM

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * debug output level
 *
 * @see OS_DEBUG_SetGlobalLevel
 * @see OS_DEBUG_GetGlobalLevel
 */
typedef enum
{
	DBG_LEVEL_OFF	= 0,		///< debug off
	DBG_LEVEL_ERR	= 1,		///< only error level is printed
	DBG_LEVEL_WRN	= 2,		///< only error and warning level are printed
	DBG_LEVEL_DBG	= 3,		///< all debugs are printed
	DBG_LEVEL_ALL	= DBG_LEVEL_DBG,
}
DBG_LEVEL_T;

/**
 * color definition for debug output
 *
 * @see OS_DEBUG_EnableModuleByIndex
 */
typedef enum
{
	DBG_COLOR_NONE	= 0,

	DBG_COLOR_BLACK,			///< foreground : black
	DBG_COLOR_RED,				///< foreground : red
	DBG_COLOR_GREEN,			///< foreground : green
	DBG_COLOR_YELLOW,			///< foreground : yellow
	DBG_COLOR_BLUE,				///< foreground : blue
	DBG_COLOR_PURPLE,			///< foreground : purple
	DBG_COLOR_CYAN,				///< foreground : cyan
	DBG_COLOR_GRAY,				///< foreground : gray

	DBG_COLOR_UL_BLACK,			///< foreground : black (underline attribute)
	DBG_COLOR_UL_RED,			///< foreground : red	(underline attribute)
	DBG_COLOR_UL_GREEN,			///< foreground : green (underline attribute)
	DBG_COLOR_UL_YELLOW,		///< foreground : yellow (underline attribute)
	DBG_COLOR_UL_BLUE,			///< foreground : blue	(underline attribute)
	DBG_COLOR_UL_PURPLE,		///< foreground : purple (underline attribute)
	DBG_COLOR_UL_CYAN,			///< foreground : cyan	(underline attribute)
	DBG_COLOR_UL_GRAY,			///< foreground : gray	(underline attribute)


	DBG_COLOR_BG_BLACK,			///< background : black
	DBG_COLOR_BG_RED,			///< background : red
	DBG_COLOR_BG_GREEN,			///< background : green
	DBG_COLOR_BG_YELLOW,		///< background : yellow
	DBG_COLOR_BG_BLUE,			///< background : blue
	DBG_COLOR_BG_PURPLE,		///< background : purple
	DBG_COLOR_BG_CYAN,			///< background : cyan
	DBG_COLOR_BG_GRAY,			///< background : gray
}
DBG_COLOR_T;

/**
 * pointer type definition for raw debug output function
 *
 * TODO : add the function to replace internal debug output function
 */
typedef    int    (*DBG_OUTPUT_FUNCPTR)(const char *fmt, ...);

/**
 * special debug index for common print. DO NOT use in your code.
 *
 */
enum
{
	DEBUG_IDX_COMMON_PRINT	= 0,		///< used at DBG_MSG, DBG_TRACE
	DEBUG_IDX_OSUTIL_PRINT	= 1,		///< used at os_util.c
};

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


#ifdef KDRV_DEBUG_BUILD

extern void			OS_DEBUG_Init	( void );
extern void			OS_DEBUG_Cleanup ( void );
#if 0//default LOGM
extern void			OS_DEBUG_InitProc ( void );

extern int			OS_DEBUG_OpenModule( const char* module_name);
extern void			OS_DEBUG_CloseModule( int module_fd );
extern void			OS_DEBUG_EnableModule( int module_fd );
extern void			OS_DEBUG_DisableModule( int module_fd );
extern void			OS_DEBUG_SetModuleMask( int module_fd, UINT32 mask );
extern UINT32		OS_DEBUG_GetModuleMask( int module_fd );
extern void			OS_DEBUG_EnableModuleByIndex( int module_fd, int index, DBG_COLOR_T color );
extern void			OS_DEBUG_DisableModuleByIndex( int module_fd, int index );

extern void			OS_DEBUG_SetGlobalLevel ( DBG_LEVEL_T level );
extern DBG_LEVEL_T 	OS_DEBUG_GetGlobalLevel ( void );

/*
 * DO NOT use in your code.
 */
extern void			OS_DEBUG_PrintError_Tag(const DBG_LEVEL_T level, const char *szFile, const char *szFunc, const int nLine, const char *format, ...)
																												__PRINTF(5,6);
extern void			OS_DEBUG_Print_Tag( const char* szFunc, const int nLine, const int module_fd, const int index, const char *format, ...)
																												__PRINTF(5,6);
#else

#define OS_DEBUG_EnableModule(module_fd)
#define OS_DEBUG_PrintError_Tag(l, fi, fn, ln, f, args...) 		LOGM_PRINT(g_global_debug_fd, LX_LOGM_LEVEL_ERROR, f, ##args)
#define OS_DEBUG_EnableModuleByIndex(module_fd, index,color ) 	LOGM_ObjBitMaskEnable(module_fd, index)
#define OS_DEBUG_DisableModuleByIndex(module_fd, index ) 		LOGM_ObjBitMaskDisable(module_fd, index)
#define OS_DEBUG_SetModuleMask(module_fd,mask )					LOGM_ObjMaskSet(module_fd, mask)
#endif//default LOGM

#else /* KDRV_DEBUG_BUILD */
/*
 *
 */
#define	OS_DEBUG_Init()
#define OS_DEBUG_Cleanup()
#define OS_DEBUG_InitProc()

#define	OS_DEBUG_PrintError_Tag(szFile,szFunc,nLine,format,args...)
#define OS_DEBUG_Print_Tag(szFunc,nLine,format,args...)

#define OS_DEBUG_OpenModule(module_name)			(0)
#define OS_DEBUG_CloseModule(module_fd)
#define OS_DEBUG_EnableModule(module_fd)
#define OS_DEBUG_DisableModule(module_fd)
#define OS_DEBUG_SetModuleMask(module_fd,mask )
#define OS_DEBUG_GetModuleMask(module_fd)		(0xf)
#define OS_DEBUG_EnableModuleByIndex(module_fd,index,color)
#define OS_DEBUG_DisableModuleByIndex(module_fd,index)

#define OS_DEBUG_SetGlobalLevel(level)
#define OS_DEBUG_GetGlobalLevel()					(DBG_LEVEL_OFF)

#endif /* KDRV_DEBUG_BUILD */

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern int  g_global_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BASE_DRV_H_ */

/** @} */
/** @} */

