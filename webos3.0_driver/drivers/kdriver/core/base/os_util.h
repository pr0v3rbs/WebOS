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
 *  OS(Linux) kernal API wrapper
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  @version	1.0
 *  @date		2009.11.18
 *
 *  @addtogroup lg1150_base
 *	@{
 */

#ifndef	_OS_UTIL_H_
#define	_OS_UTIL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include "base_types.h"

#include "l9/os/linux_platform.h"
#include "l9/os/linux_irqs.h"
#include "h13/os/linux_platform.h"
#include "h13/os/linux_irqs.h"
#include "m14/os/linux_platform.h"
#include "m14/os/linux_irqs.h"
#include "h14/os/linux_platform.h"
#include "h14/os/linux_irqs.h"
#include "h15/os/linux_platform.h"
#include "h15/os/linux_irqs.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/** @name Type Definition for OS Common
 *
 * @{
 *
 *	@def OS_NO_WAIT
 *	timeout option with no wait
 *
 *	function with this timeout never wait until it can acquire the mutex.
 *	try and fail if mutex is not available.
 *	this option is used in interrupt handler.
 *
 *	@see OS_LockMutexEx, OS_RecvEvent
 *
 *	@def OS_INFINITE_WAIT
 *	timeout option with infinite wait
 *
 *	function with this timeout wait forever until it can acquire the mutex.
 *	this option is used in normal task.
*/
#define OS_NO_WAIT				0
#define OS_INFINITE_WAIT		((UINT32)-1)

/**
 * @def OS_SEM_ATTR_DEFAULT
 * @brief default semaphore attribute ( not recursive )
 */
#define OS_SEM_ATTR_DEFAULT     0

/**
 * @def OS_EVENT_RECEIVE_ANY
 * option for OS_RecvEvent. wait for one of requested event.
 *
 *
 * @def OS_EVENT_RECEIVE_ALL
 * option for OS_RecvEvent. wait for all of requested event.
 *
*/
#define OS_EVENT_RECEIVE_ANY	0
#define OS_EVENT_RECEIVE_ALL	1

/**
 * @def OS_TIMER_TIMEOUT
 * timeout is issued onece (one short timer).
 *
 * @def OS_TIMER_TIMETICK
 * timetick is issued perodically.
 */
#define	OS_TIMER_TIMEOUT		(1<<0)
#define	OS_TIMER_TIMETICK		(1<<1)
/** @} */


/**
 *	type definition for semaphore structure
 */
typedef struct
{
	struct semaphore 		sem;			///< semaphore structure in Linux
	wait_queue_head_t		wq;				///< wait_queue_head structure in Linux. wait for timed wait.
	UINT32					wait_loop:1,	///< mutex lock with timeout requested
							rsvd:31;
}
OS_SEM_T;

/**
 *	type definition for event structure
 */
typedef struct
{
	spinlock_t				spinlock;
	wait_queue_head_t		wq;				///< wait_queue_head structure in Linux
	volatile UINT32			ev;				///< event value
}
OS_EVENT_T;


/**
 * type defnition for timer callback function.
 */
typedef void	(*OS_TIMER_CALLBACK_T)(ULONG);

/**
 * type definition for timer structure
 */
typedef struct
{
	spinlock_t				spinlock;
	struct timer_list		tmr;			///< timer_list structure in Linux
	UINT32					option;			///< timer option
	UINT32					data;			///< timer data
	ULONG					ticks;			///< timer tick value ( jiffies )
	OS_TIMER_CALLBACK_T		callback;		///< timer callback function
}
OS_TIMER_T;

/**
 * type definition for the file operation
 */
typedef struct
{
	struct file*			fd;				///< pointer to linux file struct
}
OS_FILE_T;

typedef enum
{
	OS_RGN_MGR_TYPE_TLSF	= 0x0,	///< memory manager by TLSF	(obsolete. DO NOT USE)
	OS_RGN_MGR_TYPE_HMA		= 0x1,	///< memory manager by HMA
	OS_RGN_MGR_TYPE_DEFAULT	= OS_RGN_MGR_TYPE_HMA,	///< memory manager by HMA
}
OS_RGN_MGR_TYPE_T;

/**
 * type definition for the memory region
 */
typedef struct
{
	OS_RGN_MGR_TYPE_T		mgr_type;		///< allocator manager

	int						block_size;		///< block size
	int						block_num;		///< total block_num
	int						mem_pool_size;	///< memory pool size for memory manager

	void*					phys_mem_addr;	///< start address of physical memory
	void*					virt_mem_addr;	///< start address of virtual memory for memory manager
	void*					mem_manager;	///< address of memory manager

	UINT32					mem_alloc_cnt;	///< allocation count
	UINT32					mem_alloc_size;	///< allocated memory size
}
OS_RGN_T;

typedef struct
{
	void*					phys_mem_addr;	///< start address of physical memory
	UINT32					length;			///< total length of physical memory
	UINT32					block_size;		///< mimimum allocation size

	UINT32					mem_alloc_cnt;	///< allocation count
	UINT32					mem_alloc_size;	///< allocated memory size
	UINT32					mem_free_size;	///< free memory size ( real memory will be less than free size  )
}
OS_RGN_INFO_T;

/**
 * type definition for the cached operation
 */
typedef struct
{
	ULONG	phys_addr;
	ULONG	virt_addr;
	UINT32	length;
}
OS_CACHE_MAP_T;

/*----------------------------------------------------------------------------------------
	MUTEX OS API
----------------------------------------------------------------------------------------*/

/**
 * @name Function Definition for OS Mutex(Semaphore)
 * os functions for mutex(semaphore)
 *
 * @{
 *
 * @def OS_InitMutex
 * initialize mutex
 *
 * @param pSem [IN] pointer to OS_SEM_T
 * @param attr [IN] attribute of mutex ( one of OS_SEM_ATTR_DEFAULT, OS_SEM_ATTR_RECURSIVE )
 *
 * @def OS_LockMutexEx
 * lock mutex.
 *
 * @param pSem [IN] pointer to OS_SEM_T
 * @param timeout [IN] timeout value (msec)
 * @return RET_OK if success, RET_TIMEOUT if timeout, none zero value for otherwise
 *
 * when timeout is OS_INFINITE_WAIT, task may be blocked until it gets the mutex.
 *
 * when timeout is OS_NO_WAIT
 *
 * As you know, You should use OS_TrylockMutex in interrupt service handler.
 * OS_TrylockMutex never sleeps (non-blocking mechanim);
 * If semaphore is available at the time it will return 0 (ok).
 * If the semaphore is not available at the time of the call, it returns immediately with
 * a nonzero return value.
 *
 * NOTE: As you know, done't call OS_MUTEX_LOCK in interrupt service handler.
 *
 * @def OS_UnlockMutex
 * unlock mutex.
 *
 * @param pSem [IN] pointer to OS_SEM_T
 * @return NONE
 */
#define OS_InitMutex(pSem,attr)			OS_InitMutex_Tag(pSem,attr)
#define OS_LockMutexEx(pSem,timeout)	OS_LockMutex_Tag(pSem,timeout, " ", __F__, __L__)
#define OS_LockMutex(pSem)				OS_LockMutex_Tag(pSem,OS_INFINITE_WAIT, " ", __F__, __L__)
#define OS_UnlockMutex(pSem)			OS_UnlockMutex_Tag(pSem, " ", __F__, __L__)

void	OS_InitMutex_Tag	( OS_SEM_T* pSem, UINT32 attr );
int		OS_LockMutex_Tag	( OS_SEM_T* pSem, UINT32 timeout, const char* szFile, const char* szFunc, const int nLine );
void	OS_UnlockMutex_Tag 	( OS_SEM_T* pSem, const char* szFile, const char* szFunc, const int nLine );

/** @} */

/*----------------------------------------------------------------------------------------
	EVENT OS API
----------------------------------------------------------------------------------------*/
/**
 * @name Function Definition for OS Event
 * os functions for event
 *
 * @{
 *
 * @def OS_InitEvent
 * initialize event structure.
 * @param pEvnt [IN] point to OS_EVENT_T
 *
 * @def Os_RecvEvent
 * receive event.
 *
 * @def OS_SendEvent
 * send event.
 *
 * @def OS_ClearEvent
 * clear event.
 */
#define OS_InitEvent(pEvnt)			OS_InitEvent_Tag( pEvnt )
#define OS_RecvEvent(pEvnt,ev,rEv,opt,tm)	OS_RecvEvent_Tag(pEvnt,ev,rEv,opt,tm," ",__F__,__L__)
#define	OS_SendEvent(pEvnt,ev)		OS_SendEvent_Tag( pEvnt, ev, " ", __F__, __L__)
#define OS_ClearEvent(pEvnt)		OS_ClearEvent_Tag( pEvnt, " ", __F__, __L__)

void	OS_InitEvent_Tag	( OS_EVENT_T* pEvnt );
int		OS_RecvEvent_Tag	( OS_EVENT_T* pEvnt, UINT32 ev, UINT32* rEv, UINT32 option, UINT32 timeout,
							const char* szFile, const char* szFunc, const int nLine );
void	OS_SendEvent_Tag	( OS_EVENT_T* pEvnt, UINT32 ev,
							const char* szFile, const char* szFunc, const int nLine );
void	OS_ClearEvent_Tag	( OS_EVENT_T* pEvnt,
							const char* szFile, const char* szFunc, const int nLine );

/** @} */

/*----------------------------------------------------------------------------------------
	TIME/TIMER OS API
----------------------------------------------------------------------------------------*/
/**
 * @name Function Definition for OS Time/Timer
 * os functions for time/timer
 *
 * @{
*/


/**
 @def OS_MsecSleep
 general sleep (mili second).
 @param msec [IN] msec time
 @return RET_OK if success, RET_INTR_CALL if sleep is cancelled by user interrupt.\n
         You should check return value if time is very crtical to operation.

 @def OS_NsecDelay
 busy wait delay (nano second).
 @param nsec [IN] nano second time
 @return NONE

 Remember that the these delay functions are busy-waiting.\n
 Pros : I think delay is very accurate than the normal sleep or schedule_timeout.\n
 Cons : Other tasks can't be run during the time interval. Thus, these functions should only
 be used when there is no practical alternative.\n

 @def OS_UsecDelay
 busy wait delay (micro second).
 @param usec [IN] micro second time
 @return NONE

 Remember that the these delay functions are busy-waiting.\n
 Pros : I think delay is very accurate than the normal sleep or schedule_timeout.\n
 Cons : Other tasks can't be run during the time interval. Thus, these functions should only
 be used when there is no practical alternative.\n

 @def OS_GetMsecTicks
 @param NONE
 @return ticks in unit of mili second (UINT64)

 get current time ticks in unit of microsecond.
 this functions uses current_kernel_time() api.

 @def OS_GetUsecTicks
 @param NONE
 @return ticks in unit of micro second (UINT64)

 current time ticks in unit of microsecond.
 function uses current_kernel_time() api and

 @def OS_GetCurrentTicks
 elapsed time tick.
 @param pSec [OUT]  seconds ( can be NULL )
 @param pMsec [OUT] mili seconds ( can be NULL )
 @param pUsec [OUT] micro seconds ( can be NULL )
 @return NONE

 functions uses current_kernel_time() api.
 function can returns elapsed tick time in unit of second or mili second or micro second.

 example, If you want to get elapsed mili second, use to following code:

	@verbatim
	UINT32 msecTime
	UINT32 usecTime;
	OS_GetCurrentTicks(NULL,&msecTime,NULL);
	printk("elapsed ticks in milisecond  = %d\n", msecTime );
	@endverbatim

*/
UINT64	OS_GetMsecTicks 	( void );
UINT64	OS_GetUsecTicks 	( void );
UINT64	OS_GetNsecTicks		( void );
void	OS_GetCurrentTicks 	( UINT32* pSec, UINT32* pMsec, UINT32* pUsec );

int		OS_MsecSleep 		( UINT32 msec );
void	OS_UsecDelay 		( UINT32 usec );
void	OS_NsecDelay 		( UINT32 nsec );

/**
 @def OS_InitTimer
 Initialize timer structure.

 @param pTimer [IN] pointer to OS_TIMER_T

 @def OS_StartTimer
 Start timeick/timeout.
 when option is OS_TIMER_TIMEOUT, timer_callback function is called once and timer is stopped automatically.\n
 when option is OS_TIMER_TIMETICK, timer_callback function is called perodically and timer should be stopped
 with OS_StopTimer.\n
 For safe operation, I recommend to stop timer with OS_StopTimer regardless of timer option.\n

 Please remember that timer_callback function is a software interrupt.
 So be careful not to call any function which makes current process to sleep.

 @param pTimer [IN] pointer to OS_TIMER_T
 @param timer_callback [IN] pointer to callback function which is called when requested timeout/timetick ocurrs
 @param option [IN] timer option. one of OS_TIMER_TIMEOUT, OS_TIMER_TIMETICK
 @param timeout [IN] timeout value in milisecond
 @param data [IN] extra data to passed to timer callback

 @def OS_StopTimer
 Stop timer/timetick.
 When timer is already stopped, function call is ignored.
 @param pTimer [IN] pointer to OS_TIMER_T
*/
void		OS_InitTimer	( OS_TIMER_T* pTimer );
void		OS_StopTimer	( OS_TIMER_T* pTimer );
void		OS_StartTimer	( OS_TIMER_T* pTimer, OS_TIMER_CALLBACK_T timer_callback, UINT32 option, UINT32 timeout, UINT32 data );

/*----------------------------------------------------------------------------------------
	MEMORY OS API
----------------------------------------------------------------------------------------*/

/**
 * @name Function Defnition for OS Memory
 * os functions for linux memory operation
 *
 * @{
 *
 * @def OS_KMalloc
 *	allocate kernel memory using kmalloc
 *	@param size [IN] byte size of requested memory
 *	@return pointer to the allocated memory
 *
 *
 * @def OS_KFree
 *	free kernel memory using kfree.
 *	@param ptr [IN] memory pointer to free
 *	@return none
 *
 * @def OS_Malloc
 *	alias for OS_KMalloc.
 *	@param size [IN] byte size of requested memory
 *
 * @def OS_Free
 *	alias for OS_KFree
 *	@param ptr [IN] memory pointer to free
 *	@return none
 *
 * @def OS_InitRegionEx
 *	initilize memory region.
 *	The device driver can allocate anonymous size of memory from the memry region.
 *	This simple region manager can allocate memory very fast.
 *
 *	@param pRgn [IN]		pointer to memory region manager
 *	@param phys_addr  [IN]	physical start address of memory region
 *	@param size 	  [IN]	memory pool size
 *	@param block_size [IN]	minimul allocation size & alignment size
 *	@return RET_OK if success, RET_ERROR otherwise.
 *
 * @def OS_CleanupRegion
 *	cleanup memory region.
 *	This function destroy the memory region.
 *	Note that this function will not free all the allocated memory.
 *	If you are not sure, DO NOT call this function.
 *
 *	@param pRgn [IN] pointer to memory region manager
 *	@return RET_OK if success, RET_ERROR otherwise.
 *
 * @def OS_MallocRegion
 *	allocate memory from the memory region.
 *	This function may be blocked by the memory condition.
 *	So DO NOT call this function from the ISR.
 *
 *	@param pRgn [IN]		pointer to memory region manager
 *	@return pointer to the allocated physical address.
 *
 * @def OS_FreeRegion
 *	free memory to the memory region.
 *	This function may be blocked by the memory condition.
 *	So DO NOT call this function from the ISR.
 *
 *	@param pRgn [IN]		pointer to memory region manager
 *	@param ptr [IN] memory pointer to free
 *	@return RET_OK if success, RET_ERROR otherwise.
 *
 * @def OS_GetRegionInfo
 * get the current status os region
 *
 *	@param pRgn [IN] pointer to memory region manager
 *	@param pRegionInfo [OUT] pointer to memory information
 *	@return RET_OK if success, RET_ERROR otherwise.
 */
#define OS_KMalloc(size)			OS_KMalloc_Tag(size, " ", __F__, __L__)
#define OS_KFree(ptr)				OS_KFree_Tag(ptr, " ", __F__, __L__)

#define OS_Malloc(size)				OS_KMalloc(size)
#define	OS_Free(ptr)				OS_KFree(ptr)

void*	OS_KMalloc_Tag( size_t size, const char* szFile, const char* szFunc, const int nLine );
void	OS_KFree_Tag( void* ptr, const char* szFile, const char* szFunc, const int nLine );


#define OS_RGN_DEFAULT_BLK_SIZE		PAGE_SIZE	/* 4096	*/

#define OS_InitRegionEx(pRgn,mgr_type,phys_addr,size,block_size) \
									OS_InitRegionEx_Tag(pRgn, mgr_type, (void*)phys_addr, size, block_size, " ", __F__, __L__ )
#define OS_InitRegion(pRgn,phys_addr,size)	\
									OS_InitRegionEx(pRgn, OS_RGN_MGR_TYPE_TLSF, (void*)phys_addr, size, OS_RGN_DEFAULT_BLK_SIZE )
#define OS_CleanupRegion(pRgn)		OS_CleanupRegion_Tag(pRgn, " ", __F__, __L__)
#define OS_MallocRegion(pRgn,size)	OS_MallocRegion_Tag(pRgn,size, " ", __F__, __L__)
#define OS_FreeRegion(pRgn,ptr)		OS_FreeRegion_Tag(pRgn,ptr, " ", __F__, __L__)
#define OS_GetRegionInfo(pRgn,pMemInfo)	OS_GetRegionInfo_Tag(pRgn, pMemInfo, " ", __F__, __L__)

int		OS_InitRegionEx_Tag	( OS_RGN_T* pRgn, OS_RGN_MGR_TYPE_T mgr_type, const void* phys_addr, const int size,  const int block_size,
								const char* szFile, const char* szFunc, const int nLine );
int		OS_CleanupRegion_Tag( OS_RGN_T* pRgn, const char* szFile,
								const char* szFunc, const int nLine );
void*	OS_MallocRegion_Tag	( OS_RGN_T* pRgn, size_t size,
								const char* szFile, const char* szFunc, const int nLine );
int		OS_FreeRegion_Tag	( OS_RGN_T* pRgn, void* ptr,
								const char* szFile, const char* szFunc, const int nLine );
int		OS_GetRegionInfo_Tag( OS_RGN_T* pRgn, OS_RGN_INFO_T* pMemInfo,
								const char* szFile, const char* szFunc, const int nLine );

/** @} */

/*----------------------------------------------------------------------------------------
	REGS OS API
----------------------------------------------------------------------------------------*/
/** write register
 *
 *	@param  reg [IN] register address
 *	@param 	val [IN] UINT32 register value
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int		OS_WrReg 	( UINT32 reg, UINT32 val );

/** read register
 *
 *	@param  reg [IN] register address
 *	@param 	val [OUT] UINT32 register value
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int		OS_RdReg	( UINT32 reg, UINT32* val );

/** write reigister with array
 *
 *	@param  regList [IN] array of register address
 *	@param 	valList [IN] array of UINT32 register value
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int		OS_WrRegList( UINT32 regList[], UINT32 valList[], int num );

/** read register with array
 *
 *	@param  regList [IN] array of register address
 *	@param 	valList [OUT] array of UINT32 register value
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int		OS_RdRegList( UINT32 regList[], UINT32 valList[], int num );

/** write UINT32 data list to physical address
 *
 *	@param  phys_addr [IN] start address of physical memory
 *	@param 	data_list [IN] data list
 *	@param	num_data  [IN] num of data
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int     OS_WrData ( UINT32 phys_addr, UINT32* data_list, int num_data );

/** read UINT32 data list to physical address
 *
 *	@param  phys_addr [IN] start address of physical memory
 *	@param 	data_list [OUT data list
 *	@param	num_data  [IN] num of data
 *	@return RET_OK if success, RET_ERROR if ioremap failed
 */
int     OS_RdData ( UINT32 phys_addr, UINT32* data_list, int num_data );

/** dump binary data including register
 *
 *	@param disp_addr [IN] start address to be displayed in output screen.
 *	@param data_addr [IN] buffer pointer to hold actual data
 *  @param data_cnt  [IN] data size
 *	@param output_buf[OUT] if NOT null, all dump contents are stored in output_buf
 */
#define	OS_HexDump(disp_addr,data_ptr,data_cnt)	(void)OS_HexDumpEx(disp_addr,data_ptr,data_cnt,NULL)
int		OS_HexDumpEx ( UINT32 disp_addr, void* data_ptr, UINT32 data_cnt, char* output_buf );

/** scan linux cmdline string and return target command option
 *	@param fmt_str [IN] target pattern
 *	@param val_str [IN] value string of target pattern
 *	@return RET_OK if success, RET_ERROR if pattern not found
 *
 */
int OS_ScanKernelCmdline( const char* fmt_str, char* val_str );

/*----------------------------------------------------------------------------------------
	ISR OS API
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	FILE OS API
----------------------------------------------------------------------------------------*/
/**
 * @name Function Defnition for OS File Access
 * os functions for file access service in device driver
 *
 * @{
*/

/**
 * @def OS_OpenFile
 * open file ( refer to open(2) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @param fname [IN] file name
 * @param flags [IN] file flags
 * @param mode  [IN] file creation mode if O_CREAT flags is used.
 * @return RET_OK if success, RET_ERROR if file not found or can't be opened.
 *
 * @def OS_CloseFile
 * close file ( refer to close(2) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @return RET_OK if success, RET_ERROR if failed
 *
 * @def OS_SeekFile
 * seek file ( refer to lseek(2) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @return file offset value if success, -1 if failed
 *
 * @def OS_SizeOfFile
 * get the current position of file ( refer to ftell(3) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @return file size value if success, -1 if failed
 *
 * @def OS_WriteFile
 * write data to file ( refer to write(2) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @param buffer [IN] write buffer to hold data
 * @param count [IN] requested bytes to write
 * @return total bytes written if success, -1 if failed
 *
 * @def OS_ReadFile
 * read data from file ( refer to read(2) in manpage ).
 * @param pFile [IN] pointer to OS_FILE_T
 * @param buffer [OUT] read buffer to hold data
 * @param count [IN] requested bytes to read
 * @return total bytes read if success, -1 if failed
 */
#define OS_OpenFile(pFile,fname,flags,mode)		OS_OpenFile_Tag(pFile, fname, flags, mode, " ", __F__, __L__)
#define OS_CloseFile(pFile)						OS_CloseFile_Tag(pFile , " ", __F__, __L__)
#define OS_SeekFile(pFile, offset, whence)		OS_SeekFile_Tag(pFile, offset, whence, " ", __F__, __L__)
#define OS_SizeOfFile(pFile)					OS_SizeOfFile_Tag(pFile,  " ", __F__, __L__)
#define OS_WriteFile(pFile,buffer,count)		OS_WriteFile_Tag(pFile, buffer, count,  " ", __F__, __L__)
#define OS_ReadFile(pFile,buffer,count)			OS_ReadFile_Tag(pFile, buffer, count,  " ", __F__, __L__)

int		OS_OpenFile_Tag			( OS_FILE_T* pFile, char* fname, int flags, mode_t mode,
											const char* szFile, const char* szFunc, const int nLine );
int		OS_SeekFile_Tag			( OS_FILE_T* pFile, off_t offset, int whence,
											const char* szFile, const char* szFunc, const int nLine );
int		OS_SizeOfFile_Tag		( OS_FILE_T* pFile,
											const char* szFile, const char* szFunc, const int nLine );
int		OS_WriteFile_Tag		( OS_FILE_T* pFile, char* buffer, int count,
											const char* szFile, const char* szFunc, const int nLine );
int		OS_ReadFile_Tag			( OS_FILE_T* pFile, char* buffer, int count,
											const char* szFile, const char* szFunc, const int nLine );
int		OS_CloseFile_Tag		( OS_FILE_T* pFile,
											const char* szFile, const char* szFunc, const int nLine );

/**
 * @name Function Defnition for OS File Access
 * os functions for file access service in device driver
 *
 * @{
 *
 * @def OS_OpenCache
 *
 * @def OS_CloseCache
 *
 * @def OS_InvalCache
 *
 * @def OS_FlushCache
 *
 * @def OS_CleanCache
 */
int		OS_OpenCacheMap	( OS_CACHE_MAP_T* pMap, ULONG phys_addr, UINT32 length );
int		OS_CloseCacheMap( OS_CACHE_MAP_T* pMap );
int		OS_InvalCacheMap( OS_CACHE_MAP_T* pMap, UINT32 off, UINT32 length );
int		OS_CleanCacheMap( OS_CACHE_MAP_T* pMap, UINT32 off, UINT32 length );

/** @} */


/*----------------------------------------------------------------------------------------
	SYSTEM OS API
----------------------------------------------------------------------------------------*/
/**
 * system failure processing
 *
 * This function is called when some system failure should be reported during runtime.
 * for debug stage, this function will show some useful information for debug.
 * for release stage, this function will reboot(?) the whole system.
 *
 * @note current implementation just report error message and do nothing.
 *
 */
#define	OS_FAILURE()				OS_FAILURE_Tag(" ", __F__, __L__)

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void	OS_CORE_Init		(void);
void	OS_CORE_InitDebug	(void);
void	OS_CORE_Cleanup 	(void);

/** return LG chip revision information
 *
 *	@see LX_CHIP_REV
 */
UINT32	lx_chip				(void);
UINT32	lx_chip_rev			(void);
UINT32	lx_ace				(void);
UINT32	lx_ace_rev			(void);

/** return platform information for customizing the kdriver
 */
UINT32	lx_chip_plt			(void);

#if defined(CONFIG_LX_GRAPHIC_RES) && (CONFIG_LX_GRAPHIC_RES>0)
    #define lx_chip_graphic_res()   CONFIG_LX_GRAPHIC_RES
#else
    extern UINT32 lx_chip_graphic_res(void);
#endif

/** helper function to making linux device class supporting udev
 *
 *	@param udev[IN] your device node allocated by cdev_add()
 *	@param fmt [IN] your device name
 */
void	OS_CreateDeviceClass	(dev_t dev, const char* fmt, ...);
void	OS_DestroyDeviceClass	(dev_t dev );

/*----------------------------------------------------------------------------------------
  DO NOT use the below function. Use OS_XXX macroes instead of these functions.
----------------------------------------------------------------------------------------*/
void	OS_FAILURE_Tag	(const char* szFile, const char* szFunc, const int nLine);
char*	simple_strtok	(char *s, const char *delim, char **save_ptr);

/*---------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _TIME_UTIL_H_ */

/** @} */

