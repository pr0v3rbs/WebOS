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

/** @file base_types.h
 *
 *	common header file for lg115x driver
 *
 *  @author		daeyoung lim (raxis.lim@lge.com)
 *  @version	1.0
 *
 *  @addtogroup lg1150_base
 */

#ifndef	_BASE_TYPES_H_
#define	_BASE_TYPES_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define		LX_MAX_DEVICE_NUM		64
#define		LX_MAX_DEVICE_NAME		12

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Base Type Definitions
----------------------------------------------------------------------------------------*/
#ifndef UINT8
typedef	unsigned char			__UINT8;
#define UINT8 __UINT8
#endif

#ifndef UINT08
typedef	unsigned char			__UINT08;
#define UINT08 __UINT08
#endif

#ifndef SINT8
typedef	signed char				__SINT8;
#define SINT8 __SINT8
#endif

#ifndef SINT08
typedef	signed char				__SINT08;
#define SINT08 __SINT08
#endif

#ifndef CHAR
typedef	char					__CHAR;
#define CHAR __CHAR
#endif

#ifndef UINT16
typedef	unsigned short			__UINT16;
#define UINT16 __UINT16
#endif

#ifndef SINT16
typedef	signed short			__SINT16;
#define SINT16 __SINT16
#endif

#ifndef UINT32
typedef	unsigned int			__UINT32;
#define UINT32 __UINT32
#endif

#ifndef SINT32
typedef signed int				__SINT32;
#define SINT32 __SINT32
#endif

#ifndef BOOLEAN
#ifndef _EMUL_WIN
typedef	unsigned int			__BOOLEAN;
#define BOOLEAN __BOOLEAN
#else
typedef	unsigned char			__BOOLEAN;
#define BOOLEAN __BOOLEAN
#endif
#endif

#ifndef ULONG
typedef unsigned long			__ULONG;
#define ULONG __ULONG
#endif

#ifndef SLONG
typedef signed long				__SLONG;
#define SLONG __SLONG
#endif

#ifndef UINT64
#ifndef _EMUL_WIN
typedef	unsigned long long		__UINT64;
#else
typedef	unsigned _int64			__UINT64;
#endif
#define UINT64 __UINT64
#endif

#ifndef SINT64
#ifndef _EMUL_WIN
typedef	signed long long		__SINT64;
#else
typedef	signed _int64			__SINT64;
#endif
#define SINT64 __SINT64
#endif

#ifndef TRUE
#define TRUE					(1)
#endif

#ifndef FALSE
#define FALSE					(0)
#endif

/*
#ifndef ON_STATE
#define ON_STATE				(1)
#endif

#ifndef OFF_STATE
#define OFF_STATE				(0)
#endif
*/

#ifndef ON
#define ON						(1)
#endif

#ifndef OFF
#define OFF						(0)
#endif

#ifndef NULL
#define NULL					((void *)0)
#endif

#ifndef _STR
#define _STR(_x)	_VAL(_x)
#define _VAL(_x)	#_x
#endif

/** simple form of __func__, __LINE__, __FILE__ */
#ifndef	__F__
#define	__F__		__func__
#endif
#ifndef	__L__
#define	__L__		__LINE__
#endif
#ifndef	__FL__
#define	__FL__		__FILE__
#endif

/*----------------------------------------------------------------------------------------
    Base Chip Configuration

 [note] please use lx_chip_rev() to get current chip revision
 [note] lx_chip_rev() enable us to write multi chip device driver based on runtime branch.
----------------------------------------------------------------------------------------*/
#define LX_CHIP_L8            		  	0x08	/* L8 = LG1150 -- not maintained any more*/
#define LX_CHIP_L9          		    0x09	/* L9 = LG1152 -- not maintained any more*/
#define LX_CHIP_H13						0x13	/* H13= LG1154 */
#define LX_CHIP_M14						0x14	/* M14= LG1331 */
#define LX_CHIP_H14						0x15	/* H14= LG1156 */
#define LX_CHIP_H15						0x17	/* H15= LG1211 */
#define LX_CHIP_M16						0x18	/* M16= LG???? */
#define	LX_CHIP_UNKNOWN					0xff

#define	MAKE_CHIP_REV(chp,rev)			(((chp<<8)&0xff00) | ((rev)&0x00ff))
#define LX_CHIP_REV(chp,rev)			MAKE_CHIP_REV( LX_CHIP_##chp, 0x##rev )
#define LX_ACE_REV(chp,rev)				LX_CHIP_REV(chp,rev)

#define	LX_COMP_CHIP(val,chp)			( (int)((val)&0xff00) - (int)((chp<<8) & 0xff00) )	/* compare if current value is chip (L8,L9,H13 etc). return 0 if same chip */
#define	LX_COMP_CHIP_REV(val,chprev)	( (int)((val)&0xffff) - (int)((chprev) & 0xffff) )	/* compare if current value is chip (L9A0,H13B0 etc). return 0 if same chip */

#define	LX_CHIP_REV_L9_A0				LX_CHIP_REV(L9,A0)		/* shortcut chip rev = L9 A0 */
#define	LX_CHIP_REV_L9_A1				LX_CHIP_REV(L9,A1)		/* shortcut chip rev = L9 A1 */
#define	LX_CHIP_REV_L9_B0				LX_CHIP_REV(L9,B0)		/* shortcut chip rev = L9 B0 */
#define	LX_CHIP_REV_L9_B1				LX_CHIP_REV(L9,B1)		/* shortcut chip rev = L9 B1 */
#define LX_CHIP_REV_H13_A0				LX_CHIP_REV(H13,A0)		/* shortcut chip rev = H13 A0*/
#define LX_CHIP_REV_H13_B0				LX_CHIP_REV(H13,B0)		/* shortcut chip rev = H13 B0*/
#define LX_CHIP_REV_M14_A0				LX_CHIP_REV(M14,A0)		/* shortcut chip rev = M14 A0*/
#define LX_CHIP_REV_M14_B0				LX_CHIP_REV(M14,B0)		/* shortcut chip rev = M14 B0*/
#define LX_CHIP_REV_H14_A0				LX_CHIP_REV(H14,A0)		/* shortcut chip rev = H14 A0*/
#define LX_CHIP_REV_H15_A0				LX_CHIP_REV(H15,A0)		/* shortcut chip rev = H15 A0*/

/*----------------------------------------------------------------------------------------
    Target Board (Platform) Configuration

 [note] please use lx_chip_plt() to get current target board
 [note] lx_chip_plt() enable us to write multi platform device driver based on runtime branch.

----------------------------------------------------------------------------------------*/
#define LX_CHIP_PLT_FHD			0x1000		/* FHD platform = 1920x1080@60Hz / 1920x1080@120Hz */
#define LX_CHIP_PLT_UHD			0x2000		/* UHD platform = 2560x1080 / 3840x210 / 5120x2160 */

/*----------------------------------------------------------------------------------------
	Standard Resolution Definition
----------------------------------------------------------------------------------------*/
#define LX_MAKE_RES(w,h)		(((w)<<16)|(h))
#define LX_RES_XGA				LX_MAKE_RES(1024,768)
#define LX_RES_WXGA				LX_MAKE_RES(1366,768)
#define LX_RES_HD				LX_MAKE_RES(1280,720)
#define LX_RES_FHD				LX_MAKE_RES(1920,1080)
#define LX_RES_WFHD				LX_MAKE_RES(2560,1080)
#define LX_RES_UHD				LX_MAKE_RES(3840,2160)
#define LX_RES_WUHD				LX_MAKE_RES(5120,2160)

#define LX_RES_GET_WIDTH(r)		(((r)>>16) & 0xffff)
#define LX_RES_GET_HEIGHT(r)	(((r))     & 0xffff)

/*----------------------------------------------------------------------------------------
	Error Code Definitions
----------------------------------------------------------------------------------------*/
#define	RET_OK					0			///< success
#define RET_ERROR				-EIO		///< general error
#define RET_INVALID_PARAMS		-EINVAL		///< invalid paramter
#define RET_INVALID_IOCTL		-ENOTTY		///< invalid ioctl request
#define	RET_OUT_OF_MEMORY		-ENOMEM		///< out ot memory
#define RET_NODATA              -ENODATA    ///< No data available
#define RET_TIMEOUT				-ETIME		///< timeout
#define RET_TRY_AGAIN			-EAGAIN		///< try again
#define RET_INTR_CALL			-EINTR		///< interrupted system call
#define	RET_NOT_SUPPORTED		-0xffff		///< not supported function (LG extention)

/*----------------------------------------------------------------------------------------
	Linux PM(Power Management) Definition
	@see linux/pm.h
----------------------------------------------------------------------------------------*/
typedef enum
{
	LX_PM_TYPE_SUSPEND,						///< Instant suspend
	LX_PM_TYPE_RESUME,						///< Instant boot (resume)

	LX_PM_TYPE_FREEZE,						///< SNAPSHOT suspend
	LX_PM_TYPE_THAW,						///< SNAPSHOT resume right after image making
	LX_PM_TYPE_RESTORE,						///< SNAPSHOT boot

	LX_PM_TYPE_PREPARE,
	LX_PM_TYPE_COMPLETE,
	LX_PM_TYPE_POWEROFF,
	LX_PM_TYPE_SUSPEND_LATE,
	LX_PM_TYPE_RESUME_EARLY,
	LX_PM_TYPE_FREEZE_LATE,
	LX_PM_TYPE_THAW_EARLY,
	LX_PM_TYPE_POWEROFF_LATE,
	LX_PM_TYPE_RESTORE_EARLY,
	LX_PM_TYPE_SUSPEND_NOIRQ,
	LX_PM_TYPE_RESUME_NOIRQ,
	LX_PM_TYPE_FREEZE_NOIRQ,
	LX_PM_TYPE_THAW_NOIRQ,
	LX_PM_TYPE_POWEROFF_NOIRQ,
	LX_PM_TYPE_RESTORE_NOIRQ,
	LX_PM_TYPE_RUNTIME_SUSPEND,
	LX_PM_TYPE_RUNTIME_RESUME,
	LX_PM_TYPE_RUNTIME_IDLE,
}
LX_PM_TYPE_T;

/*----------------------------------------------------------------------------------------
	Signal Definition
----------------------------------------------------------------------------------------*/
#define LX_SIGNUM(mod,idx)  ((mod[0]<<24) | (mod[1]<<16)| (idx&0xffff))

typedef struct
{
    UINT32  signum;
    UINT32  arg[3];
}
LX_SIG_INFO_T;

typedef void(*LX_SIG_HANDLER_T)(LX_SIG_INFO_T* siginfo);

/*----------------------------------------------------------------------------------------
	Macro Function Definitions
----------------------------------------------------------------------------------------*/
#define LX_MASK_OP_SET(val,msk)			do { (val) |= (msk);  } while(0)
#define LX_MASK_OP_CLR(val,msk)			do { (val) &= ~(msk); } while(0)
#define LX_MASK_OP_CHK(val,msk)			(((val)&(msk))? TRUE:FALSE)
#define LX_MASK_OP_NOT(val)				(~(val))
#define LX_MASK_OP_AND(val,msk)			((val)&(msk))
#define LX_MASK_OP_XOR(val,mask)		((val)^(msk))

#ifndef OFFSET
#define OFFSET(structure, member)		/* byte offset of member in structure*/\
		((int) &(((structure *) 0) -> member))
#endif

#ifndef MEMBER_SIZE
#define MEMBER_SIZE(structure, member)	/* size of a member of a structure */\
		(sizeof (((structure *) 0) -> member))
#endif

#ifndef NELEMENTS
#define NELEMENTS(array)				/* number of elements in an array */ \
		(sizeof (array) / sizeof ((array) [0]))
#endif

/**
 *	@def LX_CALC_ALIGNED_VALUE
 *
 *	very simple macro to get N order byte aligned integer ( return 2**N byte aligned value )
 *
 *	order = 0 --> return value itself ( non aligned )
 *	order = 1 --> return  2 byte aligned value
 *	order = 2 --> return  4 byte aligned value
 *	order = 3 --> return  8 byte aligned value
 *	order = 4 --> return 16 byte aligned value
 *	order = 5 --> return 32 byte aligned value
 *	...
 *	order = 12 --> return 4k byte aligned value
 *	...
 */
#define LX_CALC_ALIGNED_VALUE(val,order)	(((val)+(1<<(order))-1) & ~((1<<(order))-1))

/**
 *	@def PARAM_UNUSED(param)
 *
 *	gcc produces warning message when it detects unused parameters or variables.
 * 	The best workaround for the warning is to remove unused parameters.
 *	But you can remove warning by marking the unused parameters to "unused".
 *	If you hesitate to remove the unused parameters, use PARAM_UNUSED macro instead.
 */
#ifndef	PARAM_UNUSED
#define	PARAM_UNUSED(param)  ((void)(param))
#endif

/**
 * @def likely(x), unlikely(x)
 *
 * linux kernel supports likely, unlikely macroes to optmize if statement
 *
 */
#ifndef	__KERNEL__
	#define	likely(x)	__builtin_expect((x),1)
	#define	unlikely(x)	__builtin_expect((x),0)
#endif

/**
 *	@def __CHECK_IF_ERROR
 *
 *	General purpose error check routine.
 *	This macro will help you to write error check code with one line.
 */
#ifndef	__CHECK_IF_ERROR
#define	__CHECK_IF_ERROR(__checker,__if_output,__if_action,fmt,args...) \
{ \
	if (unlikely(__checker)) { \
		__if_output(fmt,##args); \
		__if_action; \
	} \
}
#endif

#ifndef	__CHECK_IF_ELSE_ERROR
#define	__CHECK_IF_ELSE_ERROR(__checker,__if_output,__if_action,__else_action,fmt,args...) \
{ \
	if (unlikely(__checker)) { \
		__if_output(fmt,##args); \
		__if_action; \
	} \
	else { \
		__else_action; \
	} \
}
#endif

#ifndef __CHECK_IF_ERROR2
#define __CHECK_IF_ERROR2(__checker,__if_output,__if_action,fmt,args...) \
{ \
	if (unlikely(__checker)) { \
		__if_output( "("_STR(__checker)") failed. " fmt,##args); \
		__if_action; \
	} \
}
#endif


/**
@note This snippet utilize "Designated Initializer" style of ISO C99. Find related article for more information.\n

@def LX_STRING_ENTRY
macro for build string list
@param _enum_def [IN] define name or enumeration to be made into string entry of list.
@see LX_ENUM_NAME LX_D_ENUM_NAME

@def LX_D_STRING_ENTRY
@param _enum_def [IN] define name or enumeration name to be made into string entry of list.
@param _enum_min [IN] define name or enumeration name which has smallest valid value.
@see LX_ENUM_NAME LX_D_ENUM_NAME

@def LX_IOCMD_STRING_ENTRY
macro for building string list.
@param _iocmd_def	[IN] define name of IO Control command.

@def LX_IOCMD_STRING_HOLDER
macro for building string list. for overriding default name (especially usefull when shortened name)
@param _iocmd_def	[IN] define name of IO Control command.
@see LX_IOCMD_NAME
*/
#define LX_STRING_ENTRY(_enum_def)				[ _enum_def ]			  = #_enum_def
#define LX_D_STRING_ENTRY(_enum_def, _enum_min)	[ _enum_def - _enum_min ] = #_enum_def
#define LX_IOCMD_STRING_ENTRY(_iocmd_def)		[_IOC_NR(_iocmd_def)]	  = #_iocmd_def
#define LX_IOCMD_STRING_HOLDER(_iocmd_def)		[_IOC_NR(_iocmd_def)]

/**
General macro to get canonical name(string) of IO control definition.

-# Assumption & Policy
	- Enumeration is non-negative type.
-# Syntax
@note This snippet utilize "Designated Initializer" style of ISO C99. Find related article for more information.\n
[@b IMPORTANT] Define ..._MAXNR properly and the Last element of _string_list should be [...MAXNR] = "unknown".

@param _iocmd 		: UINT32   : Io Control command value to query its name.
@param _string_list : (char*[]): should be prepared before use this macro like below code snippet.
@param _iocmd_maxnr : UINT32   : define name of module's IO_MAXNR
	usually defined as 1 + maximum value of _IOC_NR( all module's IO command )
@param _skip_bytes  : size_t   : skip bytes from the begining of name string for @p _iocmd.
	- define as ( sizeof("PREFIX") -1 ) to skip for printing "PREFIX". (remember : sizeof("") == 1)
	- define as ( 0 ) for full name with prefix.(@b NOT @b RECOMMENDED)

-# How to use
	-# define Io Control command value ( usually [module]_kapi.h)
	-# define @c LX_[MODULE]_IO_CMD_PREFIX_SIZE
		-# as "(sizeof("IOCMD_PREFIX_NAME_")-1) to print short name(without prfix)
	-# define pre-module macro to get its name.
		-# as @c LX_[MODULE]_IOCMD_NAME
	-# build string table. just declare proper variable name in <tt>const char*[]</tt>,
		-# fill it using @c LX_IOCMD_STRING_ENTRY macro,
		@note Name string is properly arranged by its definition value. we can focus on adding/deleting entry and forget its order.
.

- EXAMPLE: "EXAMPLE" module has following definition.
@code
	// in example_kapi.h
	// Step 0 : IO control cmd definition block.
	#define LX_EXAMPLE_IO_CMD_1		_IO(  'A',1,UINT32)
	#define LX_EXAMPLE_IO_CMD_2		_IOWR('A',2,UINT32)
	#define LX_EXAMPLE_IO_CMD_4		_IOR( 'A',4,UINT32)

	#define LX_EXAMPLE_IO_CMD_MAXNR	5	// 1 + maximum value of _IOC_NR(IO control cmd)

	// Step 1 : define prefix(shall be skiped in printing)
	#define LX_EXAMPLE_IO_CMD_PREFIX_SIZE			( sizeof("LX_EXAMPLE_IO_") -1 )
	//#define LX_EXAMPLE_IO_CMD_PREFIX_SIZE			0	// for full name

	// step 2 : define pre-module macro to get its name.
	#define LX_EXAMPLE_GET_IOCMD_NAME(_iocmd)		\
			LX_IOCMD_NAME(_iocmd, gstrExampleIoList, LX_EXAMPLE_IO_CMD_MAXNR, LX_EXAMPLE_IO_CMD_PREFIX_SIZE)

	...

	// in example_drv.c
	// Step 3 : build string table for io control command .
	const char *gstrExampleIoList[] =
	{
														// actual memory allocation
														//                      +-> automatically extracted by
														//                      |   *IO_CMD definition.
														// filled by compiler: [0] = NULL,
		LX_IOCMD_STRING_ENTRY( LX_EXAMPLE_IO_CMD_1 ),	// shall be expanded : [1] = "LX_EXAMPLE_IO_CMD_1",
		LX_IOCMD_STRING_ENTRY( LX_EXAMPLE_IO_CMD_2 ),	// shall be expanded : [2] = "LX_EXAMPLE_IO_CMD_2",
		   												// filled by compiler: [3] = NULL,
		LX_IOCMD_STRING_ENTRY( LX_EXAMPLE_IO_CMD_4 ),	// shall be expanded : [4] = "LX_EXAMPLE_IO_CMD_4",
		[LX_EXAMPLE_IO_CMD_MAXNR] = "unknown"			// shall be expanded : [5] = "unknown"
	}
	...

	// in other source files.
	// Ready to Use
	void Example_Io_Control(int fd, UINT32 cmd, void *param)
	{
		cmd = LX_EXAMPLE_IO_CMD_1;
		printf("cmd is :%s\n", LX_EXAMPLE_GET_IOCMD_NAME(cmd));	// "cmd is :CMD_1"

		#define LX_EXAMPLE_IO_CMD_BAD		_IOR( 'A',18,UINT32)	// test bad command
		cmd = LX_EXAMPLE_IO_CMD_BAD;
		printf("cmd is :%s\n", LX_EXAMPLE_GET_IOCMD_NAME(cmd));	// "cmd is :unknown"
	}

@endcode
*/
#define LX_IOCMD_NAME(_iocmd,_string_list,_iocmd_maxnr,_skip_bytes)								\
											( ( _IOC_NR(_iocmd) < _iocmd_maxnr )				\
											? (	 (NULL!=_string_list[_IOC_NR(_iocmd)]) 			\
												? _string_list[_IOC_NR(_iocmd)]+_skip_bytes		\
												: _string_list[_iocmd_maxnr]					\
											  )													\
											: _string_list[_iocmd_maxnr] )


/**
@def LX_ENUM_NAME
	get name of single bounded enumeration.

	@pre @i _string_list should be properly listed see @ref LX_IOCMD_NAME.

	@param _enum 		[IN] enumeration value to get its name
	@param _string_list [IN] global string list of given enumeration
	@param _enum_max	[IN] maximum valid enumeration value.
	@param _skip_bytes	[IN] bytes to skip for prefix of enumeration name.
	@return name string in _string_list (e.g. _string_list[_enum]+skip_bytes or _string_list[_enum_max+1] for unknown value.
	@see LX_D_ENUM_NAME


@def LX_D_ENUM_NAME
	get name of double bounded enumeration.\n
	returns its name of given @italic _enum.\n
	[@b IMPORTANT] DO NOT use @italic _enum_min is zero, USE single bounded enumeration @ref LX_ENUM_NAME.\n
	Because if @italic _enum_min = 0, it can generate a compiler warning.\n
	[NOTE] if the iocmd is not defined in coniguous, there can be 4 byte(char*) storage lost per enumeration hole.\n
	[NOTE] Define valid MIN & MAX value properly or cause try to print null stirng.\n

	@param _enum 		[IN] enumeration value to get its name
	@param _string_list [IN] global string list of given enumeration
	@param _enum_min	[IN] minimal valid enumeration value.(NON ZERO)
	@param _enum_max	[IN] maximum valid enumeration value.
	@param _skip_bytes	[IN] bytes to skip for prefix of enumeration name.
	@return name string in _string_list (e.g. _string_list[_enum]+_skip_bytes or _string_list[_enum_max+1] for unknown value.


-# Benefit & Cost
	-# Benefit
		-# no need to re-arrange switch-case, just add string entry into list, when add/insert new enumeration value inbetween case statement.
		-# no need to search or switch-case.
		-# code size : 2 substraction/comparison + 2 decision(jump) + 1~2 indirect access of string pointer.
		  compared to heavy switch-case. please calculate ;-)
		-# string data is collected to maximize data cache hit ratio.
		-# the access macro for collected string table helps instruction cache hit ratio and instruction prefetch/pre-decoding.
		-# integrated error handling. (except _string_list is null : ;-)
	-# Cost
		-# if enumeration is not defined as contiguous, the string table 4 byte(e.g:char*) loss per gap of enumeration value.
	.
-# Single/Double bounded Enumeration.
	-# Single bounded Enumeration.
	when the valid range of enumeration begins from ZERO.
	-# Double bounded Enumeration
	when the valid range of enumeartion begins from non-zero.
.

- EXAMPLE : below shows how to use these macros.
@code
	// example_single bounded.h
	#include "base_type.h"			// LX_?_ENUM_NAME() macro.

	// single bounded enumeration : begins from zero.
	typedef enum {
		LX_EXAMPLE_S_ENUM_0 	= 0,
		LX_EXAMPLE_S_ENUM_1 	= 1,
		LX_EXAMPLE_S_ENUM_4 	= 4,
		LX_EXAMPLE_S_ENUM_MAX	= LX_EXAMPLE_S_ENUM_4		// maximum valid value.
	} LX_EXAMPLE_S_ENUM_TYPE_T;

	#define LX_EXAMPLE_S_ENUM_PREFIX_SIZE	(sizeof("LX_EXAMPLE_")-1)

	#define LX_EXAMPLE_S_ENUM_NAME(_e) LX_ENUM_NAME(_e, gstExample_S_Enum, LX_EXAMPLE_S_ENUM_MAX, LX_EXAMPLE_S_ENUM_PREFIX_SIZE)

	extern const char *gstExample_S_Enum[];

	// example_double_bounded.h
	// double bounded enumeration : begins from non-zero.
	#include "base_type.h"			// LX_?_ENUM_NAME() macro.

	// double bounded enumeration : begin with non-zero.
	typedef enum {
		LX_EXAMPLE_D_ENUM_10 = 10,
		LX_EXAMPLE_D_ENUM_11 = 11,
		LX_EXAMPLE_D_ENUM_14 = 14,

		LX_EXAMPLE_D_ENUM_MIN = LX_EXAMPLE_D_ENUM_10,
		LX_EXAMPLE_D_ENUM_MAX = LX_EXAMPLE_D_ENUM_14
	} LX_EXAMPLE_D_ENUM_TYPE_T;

	#define LX_EXAMPLE_D_ENUM_PREFIX_SIZE	(sizeof("LX_EXAMPLE_")-1)

	#define LX_EXAMPLE_D_ENUM_NAME(_e) LX_D_ENUM_NAME(_e, gstrExample_D_Enum, LX_EXAMPLE_D_ENUM_MIN, LX_EXAMPLE_D_ENUM_MAX, LX_EXAMPLE_D_ENUM_PREFIX_SIZE)

	extern const char *gstrExample_D_Enum[];
	...

	...

	// example_single_bounded.c
	const char *gstrExample_S_Enum[] = {
		LX_STRING_ENTRY(LX_EXAMPLE_S_ENUM_0),	// shall be expanded :     [0] = "LX_EXAMPLE_S_ENUM_0",
		LX_STRING_ENTRY(LX_EXAMPLE_S_ENUM_1),	// shall be expanded :     [1] = "LX_EXAMPLE_S_ENUM_1",
												// filled by compiler:     [2] = NULL,
												// filled by compiler:     [3] = NULL,
												// -> loss of 4 * 2 bytes between [2~3]
		LX_STRING_ENTRY(LX_EXAMPLE_S_ENUM_4),	// shall be expanded :     [4] = "LX_EXAMPLE_S_ENUM_5",
		[LX_EXAMPLE_ENUM_MAX+1]	= "Unknown"		//                   : [4+1=5] = "Unkonwn"
	};
	...

	// example_double_bounded.c
	const char *gstrExample_D_Enum[] = {
		LX_D_STRING_ENTRY(LX_EXAMPLE_D_ENUM_10, LX_EXAMPLE_D_ENUM_MIN),	// shall be expanded : [10-10  =0] = "LX_EXAMPLE_D_ENUM_10",
		LX_D_STRING_ENTRY(LX_EXAMPLE_D_ENUM_11, LX_EXAMPLE_D_ENUM_MIN),	// shall be expanded : [11-10  =1] = "LX_EXAMPLE_D_ENUM_11",
																		// filled by compiler: [2] = NULL,
																		// filled by compiler: [3] = NULL,
																		// -> loss of 4bytes(char*) * 2 (2~3)
		LX_D_STRING_ENTRY(LX_EXAMPLE_D_ENUM_14),						// shall be expanded : [14-10  =4] = "LX_EXAMPLE_D_ENUM_14",
		[LX_EXAMPLE_D_ENUM_MAX+1,LX_EXAMPLE_D_ENUM_MIN] = "Unknown"		//                   : [14-10+1=5] = "Unknown"
	};
	...

	// caller.c
	#include "example_single_bounded.h"
	#include "example_double_bounded.h"

	Function_using_enumeration_name(LX_EXAMPLE_S_ENUM_T single_enum, LX_EXAMPLE_D_ENUM_T double_enum)
	{
		printf("single bounded enum name is '%s'\n", LX_EXAMPLE_S_ENUM_NAME(single_enum));	// "S_ENUM_#" or "Unknown"
		printf("double bounded enum name is '%s'\n", LX_EXAMPLE_D_ENUM_NAME(double_enum));	// "D_ENUM_#" or "Unknown"
	}
@endcode
*/
#define LX_ENUM_NAME(_enum,_string_list,_enum_max,_skip_bytes)							\
											( ( (_enum) <= (_enum_max) )				\
											? (	 (NULL!=(_string_list)[(_enum)]) 		\
												? (_string_list)[(_enum)]+(_skip_bytes)	\
												: (_string_list)[(_enum_max)+1]			\
											  )											\
											: (_string_list)[(_enum_max)+1] )

#define LX_D_ENUM_NAME(_enum,_string_list,_enum_min, _enum_max,_skip_bytes)								\
											( ( ( (_enum_min)<=(_enum) ) && ( (_enum)<=(_enum_max) ) )	\
											? (	 (NULL!=(_string_list)[(_enum)]) 						\
												? (_string_list)[(_enum)]+(_skip_bytes)					\
												: (_string_list)[(_enum_max)+1]							\
											  )															\
											: (_string_list)[(_enum_max)+1] )

/*----------------------------------------------------------------------------------------
    Common Type Definitions (Basic)
----------------------------------------------------------------------------------------*/
/**
 * very common endian type information.
 * data endian is always not the same as system endian.
 * some data endian should be converted to system endian or some appropridate endian type.
 * LX_ENDIANT_T may be used to identify data endian type.
 */
typedef enum
{
	LX_ENDIAN_LITTLE = 0,
	LX_ENDIAN_BIG    = 1,
}
LX_ENDIAN_T;

/**
 * chip revision information.
 *
 */
typedef	struct
{
	UINT32		version;		///< 4byte version data
	UINT8		date[4];		///< data[0] : year, date[1] : month, date[2] : day
}
CHIP_REV_INFO_T, LX_CHIP_REV_INFO_T;

/**
 * structure for register access.
 * this structure is used for application to access to register directly.
 *
 */
typedef struct
{
	UINT32		addr;			///< register address
	UINT32		value;			///< register value
}
LX_REGISTER_T;

/** general position
 *
 */
typedef struct
{
	SINT16		x;			///< x (offset)
	SINT16		y;			///< y (offset)
}
LX_POSITION_T;

/** general dimension
 *
 */
typedef struct
{
	UINT16		w;			///< width
	UINT16		h;			///< height
}
LX_DIMENSION_T;

/** general rectangle
 *
 */
typedef struct
{
	UINT16		x;			///< x ( offset )
	UINT16		y;			///< y ( offset )
	UINT16		w;			///< width
	UINT16		h;			///< height
}
LX_RECTANGLE_T /* long form */, LX_RECT_T /* short form */ ;

static inline LX_RECT_T LX_MAKE_RECT(UINT16 x, UINT16 y, UINT16 w, UINT16 h )
{
	LX_RECT_T rt = { x, y, w, h }; return rt;
}

/** memory type description
 *	there is many memory definitions in device driver implementation.
 *	in kernel driver, almost memory type is physical memory type, but there is also many virtual memory including user memory.
 *	if memory type is important when defining new some memory interface, please use the below memory type.
 *	this will increase code readabilities and decrease runtime error
 */
typedef enum
{
	LX_MEM_TYPE_PHYS	= 0,	/* physical memory area */
	LX_MEM_TYPE_VIRT	= 1,	/* virtual (user) memory area */
}
LX_MEM_TYPE_T;

/** general memory buffer
 *
 *
 */
typedef struct
{
	UINT32		addr;		///< address of buffer ( physical or logical )
	UINT32		length;		///< length of buffer
}
LX_MEM_BUF_T;

/** general memory buffer extention
 *
 *
 */
typedef struct
{
	UINT32		phys_addr;	///< physical address
	UINT32		virt_addr;	///< virtual address
	UINT32		length;		///< length of buffer
}
LX_MEM_BUF_EX_T;

/** general memory buffer stream
 *
 */
typedef struct
{
	LX_MEM_BUF_T	mem_buf;		///< memory buffer
	UINT32			curr_offset;	///< current offset from mem_buf.addr
}
LX_MEM_STREAM_T;

/**
 @def USER_PTR
 If you have structure parameter passed to IOCTRL and that structure has pointer value,
 pointer value should be marked as __user when it is used in kernel driver.
 "__user" indidcator means that pointer is from user space not kernel space.

 Let's define a structure contains pointer value:

 @code
 typedef struct
 {
    int   x, y, w, h;
	char USER_PTR *img;
 }
 IMAGE_DATA_T;
 @endcode

 IMAGE_DATA_T in user space will be expressed as follows:
 @code
 typedef struct
 {
    int   x, y, w, h;
	char *img;
 }
 IMAGE_DATA_T;
 @endcode

 IMAGE_DATA_T in kernel space will be expressed as follows:
 @code
 typedef struct
 {
   int   x, y, w, h;
   char* __user *img;
 }
 IMAGE_DATA_T;
 @endcode

*/
#ifndef	USER_PTR
#undef	USER_PTR
#endif

#ifdef  __KERNEL__	/* kernel space */
#define		USER_PTR	__user
#else				/* user space */
#define		USER_PTR
#endif

/*----------------------------------------------------------------------------------------
    Common Type Definitions (MemCfg)
----------------------------------------------------------------------------------------*/
/**
 * Common Memory Entry for each module/sub-module.
 * if base is zero then, it shall be calculated automatically from previous end address.
 */
typedef struct
{
	char	*name;		///< name of memory chunk : for debugging & module param(TODO)
	UINT32	base;		///< physical base address in BYTE!!! of media ip
	UINT32	size;		///< size in BYTE!!!
} LX_MEMCFG_T;

/**
 * Total memory status in CFG
 */
typedef struct
{
	UINT32	size;		///< total size in BYTE!!!
	UINT32	alloc;		///< total alloc space in BYTE!!!
} LX_MEMCFG_STAT_T;


/**
 * ENTRY : actual allocation shall be made ( base shall be modified.)
 * HOLDER: Just a Allocation Begin or total size marker.
 */
#define LX_MEMCFG_ENTRY_MARK_ENTRY		0
#define LX_MEMCFG_ENTRY_MARK_HOLDER		1
/**
 * structure for collecting LX_MEMCFG_T array for modules.
 */
typedef struct
{
	char		*name;
	LX_MEMCFG_T	*pM;
	int			nM;
	UINT32		flag;
} LX_MEMCFG_ENTRY_T;

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _KDRV_TYPES_DRV_H_ */

