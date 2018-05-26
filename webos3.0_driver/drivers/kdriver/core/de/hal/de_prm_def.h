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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.04.15
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef  DE_PRM_DEF_INC
#define  DE_PRM_DEF_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum {
	DE_CVM_FIR_COEF		 ,
	DE_CVS_FIR_COEF		 ,
	DE_CVM_CSC_COEF		 ,
	DE_CVS_CSC_COEF		 ,
	DE_MSCR_FIRY_COEF	 ,
	DE_MSCR_FIRC_COEF	 ,
	DE_SSCR_FIRY_COEF	 ,
	DE_MSCR_PHASE_OFFSET ,
	DE_SSCR_PHASE_OFFSET ,
	DE_CVM_HDN_COEF		 ,
	DE_MDM_HDN_COEF		 ,
	DE_DLR_QX_COEF		 ,
	DE_DLR_PHASE_COEF	 ,
	DE_MDM_CSC_COEF		 ,
	DE_POST_CSC_COEF	 ,
	DE_PARAM_TYPE_MAX	 ,
} DE_PARAM_TYPE_T;

typedef enum {
    DE_PARAM_READ   = 0,
    DE_PARAM_WRITE  = 1
} DE_PARAM_WR_T;

typedef struct {
    DE_PARAM_TYPE_T tableId;
    int (* func) (DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size);
} DE_TABLE_PARAM_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_PRM_INC  ----- */

/**  @} */
