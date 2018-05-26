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


/** @file png_kapi.h
 *
 *  application interface header for png device
 *
 *  @author		bongrae.cho (bongrae.cho@lge.com)
 *  @version		1.0 
 *  @date		2013.07.07
 *
 *  @addtogroup lg_png 
 *	@{
 */

#ifndef	_PNG_KAPI_H_
#define	_PNG_KAPI_H_

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
#define	PNG_IOC_MAGIC		'P'
/**
@name PNG IOCTL List
ioctl list for png device.
*/

/*
	------------------------------------------------------
	          bit per pixel	       |   Bits per channel
	------------------------------------------------------
	    Color option   |  Channels |   1   2   4   8   16
	------------------------------------------------------
	Indexed            |     1     |   1   2   4   8   X
	Grayscale          |     1     |   1   2   4   8   16
	Grayscale & alpha  |     2     |   X   X   X   16  32
	Truecolor          |     3     |   X   X   X   24  48
	Truecolor & alpha  |     4     |   X   X   X   32  64
*/


/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define PNG_IO_RESET				_IO(PNG_IOC_MAGIC, 	0)
#define PNG_IOR_GET_MEM_INFO		_IOR(PNG_IOC_MAGIC, 1, LX_PNG_MEM_STAT_T)
#define PNG_IOW_SET_MEM_INFO		_IOW(PNG_IOC_MAGIC, 2, LX_PNG_MEM_STAT_T)
#define PNG_IOR_GET_CONFIG			_IOR(PNG_IOC_MAGIC, 3, LX_PNG_CFG_T)
#define PNG_IOW_SET_CONFIG			_IOW(PNG_IOC_MAGIC, 4, LX_PNG_CFG_T)
//#define	PNG_IOR_GET_STATUS		    _IOR(PNG_IOC_MAGIC, 3, UINT32 )
#define	PNG_IOWR_DECODE			    _IOWR(PNG_IOC_MAGIC, 5, LX_PNG_DECODE_PARAM_T)
#define	PNG_IOR_GET_DECODE_RESULT	_IOR(PNG_IOC_MAGIC, 6, LX_PNG_DECODE_PARAM_T )

#define PNG_IOC_MAXNR				7
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum
{
	LX_PNG_COLOR_GRAYSCALE					= 0,
	LX_PNG_COLOR_TRUECOLOR					= 2,
	LX_PNG_COLOR_INDEXED					= 3,
	LX_PNG_COLOR_GRAYSCALE_ALPHA			= 4,
	LX_PNG_COLOR_TRUECOLOR_ALPHA			= 6,
} LX_PNG_COLOR_TYPE;

typedef enum
{
	LX_PNG_DECODE_MODE_FULL,
	LX_PNG_DECODE_MODE_PARTIAL,
} LX_PNG_DECODE_MODE;

typedef struct
{
    UINT16      width;
    UINT16      height;
    LX_PNG_DECODE_MODE       decode_mode;
    LX_PNG_COLOR_TYPE       color_type;
    UINT8       bit_depth;
    UINT8       output_format;
} LX_PNG_CFG_T;

typedef struct
{
    UINT32 mem_addr;
    UINT32 mem_size;
    /*
    UINT32 input_addr;
    UINT32 input_size;
    
    UINT32 output_addr;
    UINT32 output_size;*/
} LX_PNG_MEM_STAT_T;


/*
** PNG decoder parameter 
*/
typedef struct
{
    /*
    UINT32 chunk_type;
    UINT32 chunk_size;

    UINT8 num_chunk;

    UINT32 total_size;
*/
    UINT32 input_addr;
    UINT32 input_length;

    UINT32 output_addr;
    UINT32 output_length;

    UINT32 mode;
} LX_PNG_DECODE_PARAM_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PNG_DRV_H_ */

/** @} */
