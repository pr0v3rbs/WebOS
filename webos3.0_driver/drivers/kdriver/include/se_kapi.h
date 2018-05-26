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
 *  Header for Security Engine kdriver.
 *
 *  @author     stan.kim@lge.com
 *  @version    2.0
 *  @date       2012.5
 *  @note       Additional information.
 */

#ifndef	_SE_KAPI_H_
#define	_SE_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	SE_IOC_MAGIC		'S'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
/**
@name Secure Engine IOCTL List
ioctl list for Secure Engine
@{
*/

/**
@def SE_IORW_RUN_TZ_OP
	Run TZ Operation
	@param	pTzArg	[in/out] pointer to TrustZone Arguments
	@return	RET_OK if success, RET_ERROR otherwise.
*/
#define SE_IORW_RUN_TZ_OP		_IOWR(SE_IOC_MAGIC, 0, unsigned int *)

/**
@def SE_IOR_WAIT
	Wait TZ Signal
	@param	offset	[in] signal flag offset for wait
	@return	RET_OK if success, RET_ERROR otherwise.
*/
#define SE_IOR_WAIT				_IOR(SE_IOC_MAGIC, 1, unsigned int)

/**
@def SE_IOW_CACHE_CTL
	Cache Control
	@param	pCtrl	[in] Cache Contol Info
	@return	RET_OK if success, RET_ERROR otherwise.
*/
#define SE_IOW_CACHE_CTRL		_IOR(SE_IOC_MAGIC, 0x80, void *)

/**
@def SE_IO_CHECK_RESUME
	Check Resume
	@return	RET_OK if resume, RET_ERROR otherwise.
*/
#define SE_IO_CHECK_RESUME		_IO(SE_IOC_MAGIC, 0xfe)

/**
@def SE_IOW_GET_MEM_INFO
	Get SE Memory Info
	@param	pInfo	[in] Memory Info
	@return	RET_OK if success, RET_ERROR otherwise.
*/
#define SE_IOW_GET_MEM_INFO		_IOR(SE_IOC_MAGIC, 0xff, void *)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	SE_CACHE_MAP	= 0,
	SE_CACHE_INVALIDATE,
	SE_CACHE_CLEAN,
} SE_CACHE_OP;

typedef struct {
	unsigned long paddr;	/**< Physical Address */
	void *vaddr;			/**< Virtual Address */
	size_t size;			/**< Contol Size */
	SE_CACHE_OP operation;	/**< Cache Control Operation */
} SE_CACHE_CTRL_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SE_KAPI_H_ */

/** @} */
