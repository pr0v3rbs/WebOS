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
 * VBI module
 *
 *  author      won.hur (won.hur@lge.com)
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg115x_vbi
 *  @{
 */

#ifndef _VBI_DATA_H_
#define _VBI_DATA_H_

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
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	TTX_DATA_SIZE		48

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct{
	unsigned int header;
	unsigned int tailer;
	unsigned int count;
	unsigned int CGMS_offset;
	unsigned int VPS_offset;
	unsigned int WSS_offset;
	unsigned int status;
	unsigned int ttx_data_count;
} sBufInform;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int VBI_Data_Suspend(void);
int VBI_Data_Resume(void);

int VBI_TTX_InitDataMemory(void);
int VBI_TTX_UninitDataMemory(void);
int VBI_TTX_GetMemorySize(void);
int VBI_TTX_GetDataAddr(sHeaderData *data);
unsigned int VBI_TTX_GetVBIDataLineCount(unsigned int *data);
unsigned int VBI_CC_CheckDataParity(unsigned int data);

int VBI_TTX_ClearTTXMemory(void);
int VBI_TTX_ClearVPSMemory(void);
int VBI_TTX_ClearWSSMemory(void);
int VBI_TTX_ClearAllMemory(void);

int VBI_CC_PutVBIData(unsigned int *data , unsigned int size);
int VBI_CC_GetVBIData(LX_CC_DATA_T *cc_data , unsigned int size);
int VBI_CC_InitDataMemory(void);
int VBI_CC_UninitDataMemory(void);
int VBI_CC_GetVBIBufferInform(void);
int VBI_CC_SetVBIBufferClear(void);


int VBI_PutCGMSData(sCGMS *data);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VBI_DATA_H_ */

/** @} */

