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



/** @file alsa_platform.h
 *
 *  alsa platform driver.
 *
 *  @author		Won Chang Shin(wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2013.03.20
 *
 *	@{
 */

#ifndef	_lg115x_ALSA_CODEC_H_
#define	_lg115x_ALSA_CODEC_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define CODEC_NAME			"lg115x-codec"
#define CODEC_DAI_NAME		"lg115x-codec-dai"

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
int ALSA_CODEC_Init(void);
void ALSA_CODEC_Cleanup(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _lg115x_ALSA_CODEC_H_ */



