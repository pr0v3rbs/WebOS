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


#ifndef _SDEC_REG_M14C0_H_
#define _SDEC_REG_M14C0_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "sdec_reg.h"

typedef struct {
	CDIP                          		cdip[ 4]                        ;	// 0x0000 ~ 0x000c
	CDOP                           		cdop 		                    ;	// 0x0010
	UINT32                          	                 __rsvd_00[   1];	// 0x0014	                                	                                 	// 0x0014
	CDIC								cdic3							;	// 0x0018
	CDIC3_DL_CONF						cdic3_dl_conf					;	// 0x001c
	CDIC                            	cdic[ 2]                        ;	// 0x0020
	                                	                                 	// 0x0024
	CDIC_DSC                        	cdic_dsc[ 2]                    ;	// 0x0028
	                                	                                 	// 0x002c
	CDOC                            	cdoc[ 2]                        ;	// 0x0030
	                                	                                 	// 0x0034
    CDIP_2ND							cdip2_2nd						;   // 0x0038
    CDIP_2ND							cdip3_2nd						;	// 0x003C
    BDRC0_3								bdrc0							;	// 0x0040
    BDRC_LEV0_3							bdrc_lev0						;	// 0x0044
    BDRC0_3								bdrc1							;	// 0x0048
    BDRC_LEV0_3							bdrc_lev1						;	// 0x004C
    BDRC0_3								bdrc2							;	// 0x0050
    BDRC_LEV0_3							bdrc_lev2						;	// 0x0054
    BDRC0_3								bdrc3							;	// 0x0058
	BDRC_LEV0_3							bdrc_lev3						;	// 0x005C
	UINT32								sdmwc_sw_reset					;	// 0x0060 TBD CHECK NEEDED!
	UINT32								sdmwc_stat						;	// 0x0064 TBD CHECK NEEDED!
	UINT32                          	                 __rsvd_03[   2];	// 0x0068
																			// 0x006c
	MRQ_PRI                         	mrq_pri                         ;	// 0x0070
	UINT32                          	                 __rsvd_04[   3];	// 0x0074
																			// 0x0078
																			// 0x007c
	UINT32                            	stcc[ 8]                        ;	// 0x0080
	                                	                                 	// 0x0084
	                                	                                 	// 0x0088
	                                	                                 	// 0x008c
	                                	                                 	// 0x0090
	                                	                                 	// 0x0094
	                                	                                 	// 0x0098
	                                	                                 	// 0x009c
	SUB_STCC_RATE                   	sub_stcc_rate                   ;	// 0x00a0
	UINT32                          	                 __rsvd_05[   1];	// 0x00a4
	STCC_ERR_CTRL                   	stcc_err_ctrl[ 2]               ;	// 0x00a8
	                                	                                 	// 0x00ac
	STCC_ASG                        	stcc_asg                        ;	// 0x00b0
	BYTE_CNT                        	byte_cnt[ 2]                    ;	// 0x00b4
	                                	                                 	// 0x00b8
	UINT32                          	                 __rsvd_06[   1];	// 0x00bc
	UINT32                         		gstc0[ 3]                       ;	// 0x00c0
																			// 0x00c4
																			// 0x00c8
	UINT32                          	                 __rsvd_07[   1];	// 0x00cc
	STCC_G_TIMER						stcc_g_timer[2]                 ;	// 0x00d0
																			// 0x00d4
																			// 0x00d8
																			// 0x00dc
	UINT32                         		gstc1[ 3]                       ;	// 0x00e0
																			// 0x00e4
																			// 0x00e8
	UINT32                          	                 __rsvd_08[   5];	// 0x00ec
																			// 0x00f0
																			// 0x00f4
																			// 0x00f8
																			// 0x00fc
	INTR_EN                         	intr_en                         ;	// 0x0100
	UINT32                       		intr_stat                       ;	// 0x0104
	INTR_RSTAT                      	intr_rstat                      ;	// 0x0108
	UINT32                          	                 __rsvd_09[   1];	// 0x010c
	ERR_INTR_EN                     	err_intr_en                     ;	// 0x0110
	ERR_INTR_STAT                   	err_intr_stat                   ;	// 0x0114
	TEST                            	test                            ;	// 0x0118
	VERSION_                         	version                         ;	// 0x011c
	GPB_BASE_ADDR                   	gpb_base_addr                   ;	// 0x0120
	CDIP0_2ND							cdip0_2nd;						;	// 0x0124
   	CDIC								cdic2;							;	// 0x0128
   	CDIC2_TS2PES						cdic2_ts2pes;					;	// 0x012c
	CDIP								cdipa[3]						;	// 0x0130
																			// 0x0134
																			// 0x0138
	CDIP_2ND							cdip1_2nd;						;	// 0x013c
	UINT32												 __rsvd_17[   6];	// 0x0140
																			// 0x0144
																			// 0x0148
																			// 0x014C
																			// 0x0150
																			// 0x0154
	SYSCLK_SEL							sysclk_sel						;	// 0x158

} SDIO_REG_M14C0_T;


#ifdef __cplusplus
}
#endif

#endif	/* _SDEC_REG_M14C0_H_ */


