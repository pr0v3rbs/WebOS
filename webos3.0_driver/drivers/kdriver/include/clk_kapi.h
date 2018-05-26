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
 *  application interface header for clock device
 *
 *  author		Jihoon Lee (gaius.lee@lge.com)
 *  version		1.1
 *  date		2011.03.07
 *
 *  @addtogroup lg1150_clock
 *	@{
 */

#ifndef	_CLK_KAPI_H_
#define	_CLK_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	CLK_IOC_MAGIC		'C'
/**
@name CLOCK IOCTL List
ioctl list for clock

@{
@def CLK_IOW_INIT
Initialize clock kernel driver
Set Clock kernel driver to default status.

@def CLK_IOW_TERMINATE
Terminate clock kernel driver
Close and delete kernel driver

@def CLK_IOW_OPEN
Get Clock context.
belows are steps to set up context :
 1. allocate memory for context.
 2. give it to kernel driver
 3. kernel driver open and set up 1 clock object.
 4. return OK
 5. kadp driver set up function pointers into context and return it to DDI.

@def CLK_IOW_CLOSE
Close clock context.
belows are steps to close clock context :
 1. give context to clock kernel driver.
 2. kernel driver delete and free clock object.
 3. kadp driver free memory for clock context.

@def CLK_IOR_GET_TIMESTAMP
Get timestamp from kernel driver
Give clock context to kernel driver.
Inside context, there are so many information. ( for frankly, context is all about clock itself )
Clock kdrv calculate timestamp based on gstc ( from vdec ) and return it to kadp driver.
Clock kdrv save last gstc and timestamp for future calculation ( without these, clock kdrv can't do anything )

@def CLK_IOR_GET_CURRENTGSTC
Get current gstc from kernel driver. #For debugging.#
Clock kdrv return internal time from vdec.

@def CLK_IOR_GET_GSTC
Get gstc based on timestamp.
Clock kdrv calculate future gstc based on last gstc and timestamp.
#WARNING# calculating gstc based on timestamp can't handle previous timestamp or gstc. Should future gstc.
#WARNING# before call this function, user should call "CLK_IOW_SETRATE" or "CLK_IOW_SLAMTIME" or "CLK_IOW_GET_TIMESTAMP" if there is change of play rate.

@def CLK_IOW_SLAMTIME
Set base timestamp as value which user give to.

@def CLK_IOW_SETRATE
Set play rate into kernel driver

@def CLK_IOR_GETRATE
Get Play rate from kernel driver


*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
// General commands
#define	CLK_IOW_INIT								_IOWR(CLK_IOC_MAGIC,  0, UINT32)
#define	CLK_IOW_TERMINATE							_IOWR(CLK_IOC_MAGIC,  1, UINT32)
#define	CLK_IOW_OPEN								_IOWR(CLK_IOC_MAGIC,  2, LX_CLK_OBJ_T)
#define	CLK_IOW_CLOSE								_IOWR(CLK_IOC_MAGIC,  3, LX_CLK_OBJ_T)

// Time commands
#define	CLK_IOR_GET_TIMESTAMP					_IOWR(CLK_IOC_MAGIC,  10, LX_CLK_OBJ_T)
#define	CLK_IOR_GET_CURRENTGSTC					_IOWR(CLK_IOC_MAGIC,  11, LX_CLK_GET_GSTC_T)
#define	CLK_IOR_GET_GSTC							_IOWR(CLK_IOC_MAGIC,  12, LX_CLK_OBJ_T)
#define	CLK_IOW_SLAMTIME							_IOWR(CLK_IOC_MAGIC,  13, LX_CLK_OBJ_T)
#define	CLK_IOW_SETRATE							_IOWR(CLK_IOC_MAGIC,  14, LX_CLK_OBJ_T)
#define	CLK_IOR_GETRATE							_IOWR(CLK_IOC_MAGIC,  15, LX_CLK_OBJ_T)

#define CLK_IOC_MAXNR				15
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#ifndef MAX
#define MAX(a, b)		((a) < (b) ? (b) : (a))
#endif
#ifndef MIN
#define MIN(a, b)		((a) < (b) ? (a) : (b))
#endif

/* add correction with carry */
#define CORRECT(c0,c1,val)                    \
  if (val) {								  \
	if (CLK_MAXUINT64 - c0.ll < val) {			\
	  if ( c1.ll == CLK_MAXUINT64 )  \
		/* overflow */						  \
		return CLK_MAXUINT64;					\
	  c1.ll++;								  \
	}										  \
	c0.ll += val;							  \
  }

/*----------------------------------------------------------------------------------------
	Enum Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * coefficient using for conversion between 1Ghz / 90Khz.
 * Static value is calculated from 1,000,000,000 / 90,000.
 */
#define CONVERT_NUM		1000000000
#define CONVERT_DENOM		90000


#define CLK_MININT8	((INT8)  0x80)
#define CLK_MAXINT8	((INT8)  0x7f)
#define CLK_MAXUINT8	((UINT8) 0xff)

#define CLK_MININT16	((INT16)  0x8000)
#define CLK_MAXINT16	((INT16)  0x7fff)
#define CLK_MAXUINT16	((UINT16) 0xffff)

#define CLK_MININT32	((INT32)  0x80000000)
#define CLK_MAXINT32	((INT32)  0x7fffffff)
//#define CLK_MAXUINT32	((UINT32) 0xffffffff)
#define CLK_MAXUINT32		(((UINT32)1 << 31) - 1)

#define CLK_MININT64	((INT64)0x8000000000000000)
#define CLK_MAXINT64	((INT64) 0x7fffffffffffffff)
//#define CLK_MAXUINT64	((UINT64)0xffffffffffffffff)
#define CLK_MAXUINT64		(((UINT64)1 << 63) - 1)

typedef struct _LX_CLK_OBJ_T LX_CLK_OBJ_T;
typedef struct _LX_CLK_OBJ_T *P_LX_CLK_OBJ_T;
typedef struct _LX_CLK_OPS_T LX_CLK_OPS_T;

typedef UINT64	ClockTime;
typedef UINT8	ClockId;

typedef struct
{
	UINT32 					gstc_32_1;	/** gstc 32~1 bits */
	UINT32 					gstc_0;		/** gstc 0 bits */
} LX_CLK_GET_GSTC_T;


struct _LX_CLK_OPS_T
{
	LX_CLK_OBJ_T*	(*GetMaster			)( LX_CLK_OBJ_T *clock);
	int  			(*SetMaster		)( LX_CLK_OBJ_T *clock, LX_CLK_OBJ_T* master);
	int  			(*SetRate			)( LX_CLK_OBJ_T *clock, UINT32 num, UINT32 denom);
	int  			(*GetRate			)( LX_CLK_OBJ_T *clock, UINT32 *num, UINT32 *denom);
	ClockTime  		(*GetTime			)( LX_CLK_OBJ_T *clock);
	UINT32  		(*RevertTime		)( LX_CLK_OBJ_T *clock, ClockTime external);
	ClockTime  		(*GetInternalTime	)( LX_CLK_OBJ_T *clock);
	int  			(*SlamClock		)( LX_CLK_OBJ_T *clock, ClockTime slam_time);
	int  			(*Close				)( LX_CLK_OBJ_T *pCtx);
};

struct _LX_CLK_OBJ_T
{
	ClockId 				id;				/* ClockId */
	BOOLEAN 				opened;			/* Is this object opened? */
//	LX_OSA_OBJECT_T 		mutex;			/* mutex(semaphore) for CLK */
	void					*shmem;			/* shared memory for CLK */
	void			 		*mutex;			/* mutex(semaphore) for CLK */

	ClockTime				last_itime; 	/* last internal time */
	ClockTime				last_etime; 	/* last external time */

	ClockTime				seed_etime; 	/* external time when slamclock  */
	ClockTime				seed_itime; 	/* last external time */

	ClockTime				rate_numer;	/* Bunja */
	ClockTime				rate_denom;	/* Bunmo */

	ClockTime				time_offset;	/* offset of thie object */

	ClockTime				param;			/* parameter value from user space to kernel driver */
	ClockTime				ret_value;		/* return value from kernel driver */

//	LX_VDEC_CTX_T			*pVdecCtx;		/* pointer of vdec context. for getting gstc */

	LX_CLK_OBJ_T			*master;		/* pointer of master clock */
	LX_CLK_OPS_T			*op;			/* pointer of operations */
} ;


typedef union
{
  UINT64 ll;
  struct
  {
    UINT32 low, high;
  } l;
} ClkUInt64;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CLK_KAPI_H_ */

/** @} */
