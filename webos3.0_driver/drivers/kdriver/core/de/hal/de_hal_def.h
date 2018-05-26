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
 * date       2010.05.03
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef  DE_HAL_DEF_INC
#define  DE_HAL_DEF_INC

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
	DCLK_61_875   ,
	DCLK_65		  ,
	DCLK_66_462   ,
	DCLK_66_528   ,
	DCLK_36_818   ,
	DCLK_36_855   ,
	DCLK_54		  ,
	DCLK_54_054   ,
	DCLK_74_1758  ,
	DCLK_74_25	  ,
	DCLK_80_109   ,
	DCLK_80_190   ,
	DCLK_81		  ,
	DCLK_27		  ,
	DCLK_13_5	  ,
	DCLK_27_027   ,
	DCLK_13_5135  ,
	DCLK_148_3516 ,
	DCLK_148_5    ,
	DCLK_85_86    ,
} DE_DPLL_CLK_T;

typedef struct {
	UINT16 dpllM;
	UINT16 dpllN;
	UINT16 dpllOd;
} DE_DPLL_DIV_T;

typedef struct {
 	DE_DPLL_CLK_T clk;
 	DE_DPLL_DIV_T div;
} DE_DPLL_SET_T;

typedef struct {
    UINT16 tW;
    UINT16 tH;
} DE_PANEL_SIZE_T;

typedef struct {
    LX_DE_PANEL_TYPE_T panelType;
    DE_DPLL_CLK_T      *pDpllClk;
} DE_PANEL_SET_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_HAL_DEF_INC  ----- */
/**  @} */
