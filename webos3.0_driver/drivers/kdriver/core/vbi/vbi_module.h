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
 * TTX module
 *
 *  author      won.hur (won.hur@lge.com)
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg115x_ttx
 *  @{
 */

#ifndef _VBI_MODULE_H_
#define _VBI_MODULE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/interrupt.h>

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define H13A0_VBI_INTERRUPT_MASK	0x00000004
#define H14A0_VBI_INTERRUPT_MASK	0x00000004
#define M14A0_VBI_INTERRUPT_MASK	0x00000004
#define M14B0_VBI_INTERRUPT_MASK	0x00000004

typedef enum{
	LX_VBI_INIT_NONE = 0,
	LX_VBI_INIT_NTSC,
	LX_VBI_INIT_PAL,
	LX_VBI_INIT_MAXNUM
} LX_VBI_INIT_MODE_T;


typedef	enum{
	eDisable		= 0,
	eClosedCaptionUS	= 1,
	eClosedCaptionEURO	= 2,
	eVPS			= 3,
	eReserved		= 4,
	eTTX625A		= 5,
	eTTX625B		= 6,
	//eWST625			= 6,
	eTTX625C		= 7,
	eTTX625D		= 8,
	eTTX525B		= 9,
	//eWST525			= 9,
	eTTX525C		= 10,
	eTTX525D		= 11,
	eWSS625			= 12,
	eWSSJ			= 13
} eVBIDataType;


#define CC_MAX_DATA			= 2
#define CC_MAX_PACKET_NUM	= 6 

#define	CURRENT_TTX	6

#if CURRENT_TTX == 5	//eTTX625A
	#define	TTX_MAX_DATA	37
#elif  	CURRENT_TTX == 6	//eTTX625B
	#define	TTX_MAX_DATA	42
#elif  	CURRENT_TTX == 7	//eTTX625C
	#define	TTX_MAX_DATA	33
#elif  	CURRENT_TTX == 8	//eTTX625D
	#define	TTX_MAX_DATA	34
#elif  	CURRENT_TTX == 9	//eTTX525B
	#define	TTX_MAX_DATA	34
#elif  	CURRENT_TTX == 10	//eTTX525C
	#define	TTX_MAX_DATA	33
#elif  	CURRENT_TTX == 11	//eTTX525D
	#define	TTX_MAX_DATA	34
#endif

//111025 by dws : enlarge MAX_PACKET_NUM not to throw away large packets due to VBI Slicing on video lines.
#define	MAX_PACKET_NUM		43

#define VALID_LOWER_RANGE_OF_CC_LINE 15     // Used for line detection algorithm for L9Bx series.


#define	CC_VBI_PREAMBLE	0xffff00	//2011.12.06 : changed due endian change.
									//2011.12.29 : This is used only for below L9A0.

#define CC_VBI_PREAMBLE_0 0x00		//2011.12.29 : These value are used for L9B0.
#define CC_VBI_PREAMBLE_1 0xFF
#define CC_VBI_PREAMBLE_2 0XFF
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;
}sHeaderPacket;


typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;


	unsigned char	data[TTX_MAX_DATA];
}sTTXPacket;

typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;

	unsigned char	data[7];
	unsigned char	resv;
}sVPSPacket;

typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;

	unsigned int	resv 		: 24;
	unsigned int	data1 		: 8;
} sWSSPacket;

typedef struct{
	unsigned int	type;
	unsigned int	start;

	unsigned int	end;
	unsigned int	count;

	unsigned long	addr;

	unsigned int	data_start[MAX_PACKET_NUM];
	unsigned int	data_end[MAX_PACKET_NUM];

	unsigned int	data_num;
} sHeaderData;

#if 0  ///< data structure for little-endian
typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;

	unsigned int	resv 		: 24;
	unsigned int	data1 		: 8;

	unsigned int 	resv1;
	unsigned int	resv2;

} sCloseCaption;
#else
typedef struct{
	unsigned int	Preamble 	: 24;
	unsigned int	Status 		: 8;

	unsigned int	DataCount 	: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	data0		: 8;

	unsigned int	data1 		: 8;
	unsigned int	resv 		: 24;

	unsigned int 	resv1;
	unsigned int	resv2;

} sCloseCaption;
#endif

typedef struct{
	unsigned int	Preamble 	: 24;
	unsigned int	Status 		: 8;

	unsigned int	DataCount 	: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	data0		: 8;

	unsigned int	data1 		: 8;
	unsigned int	data2		: 8;
	unsigned int	resv 		: 16;
} sCGMS;

typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;

	unsigned int	resv 		: 24;
	unsigned int	data2		: 8;
	unsigned int	data1 		: 8;
} sWSS625;


typedef struct{
	unsigned int	Status 		: 8;
	unsigned int	Preamble 	: 24;

	unsigned int	data0		: 8;
	unsigned int	LineCount 	: 16;
	unsigned int	DataCount 	: 8;

	unsigned int	resv 		: 16;
	unsigned int	data2 		: 8;
	unsigned int	data1 		: 8;
} sWSSJ;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int VBI_TOP_UnInitialize(void);

int VBI_TTX_InterruptControl(int onoff);
int VBI_TTX_Initialize(void);
int VBI_CC_Initialize(void);
int VBI_CC_InterruptControl(int onoff);

int VBI_CC_SetPrintResultOfLineDetect(int onoff);

int VBI_Empty_Function_void(void);
int VBI_Empty_Function_int(int empty);
int VBI_Empty_Function_bool(BOOLEAN empty);


int VBI_Module_Resume(LX_VBI_INIT_MODE_T mode, int isrOnOff);
int VBI_SetLogmPrint(unsigned long arg);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_MODULE_H_ */

/** @} */

