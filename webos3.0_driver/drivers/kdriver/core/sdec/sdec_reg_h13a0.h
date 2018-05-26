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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author	jinhwan.bae
 *  @version	1.0
 *  @date		2012-05-25
 *  @note		Additional information.
 */


#ifndef _SDEC_REG_H13A0_H_
#define _SDEC_REG_H13A0_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "sdec_reg.h"

typedef struct {
	VID_OUT_SEL                     	vid_out_sel                     ;	// 0x0000 : ''
	DL_SEL                          	dl_sel                          ;	// 0x0004 : ''
/* added at L9 A0
 * Start */
	UINT32                          	                 __rsvd_04[   2];	// 0x0008
	UINT32                          	addr_sw0                        ;	// 0x0010 : ''
	UINT32                          	addr_sw1                        ;	// 0x0014 : ''
	UINT32                          	addr_sw2                        ;	// 0x0018 : ''
	UINT32                              te_version                      ;	// 0x0020 : ''
/* added at L9 A0
 * End */
} SDTOP_REG_H13A0_T;
/* 2 regs, 2 types */

#ifdef __cplusplus
}
#endif

#endif	/* _SDEC_REG_H13A0_H_ */


