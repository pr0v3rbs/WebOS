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
 *  register access interface for the LG1150 hardware pvr
 *
 *  author		Murugan Durairaj (murugan.d@lge.com)
 *  author		modified by ki beom kim (kibeom.kim@lge.com)
 *  version		1.1
 *  date		2010.03.20
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef _PVR_REG_DB_H_
#define _PVR_REG_DB_H_


/*-----------------------------------------------------------------------------
	DVR Top
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	0x0004 intr_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	almost_b                        : 1,	//     0
	rep_b                           : 1,	//     1
	up_err_b                        : 1,	//     2
	                                : 1,	//     3 reserved
	almost_a                        : 1,	//     4
	rep_a                           : 1,	//     5
	up_err_a                        : 1,	//     6
	                                : 1,	//     7 reserved
	unit_buf_b                      : 1,	//     8
	dn_err_b                        : 1,	//     9
	sync_err_b                      : 1,	//    10
	                                : 1,	//    11 reserved
	unit_buf_a                      : 1,	//    12
	dn_err_a                        : 1,	//    13
	sync_err_a                      : 1,	//    14
	                                : 1,	//    15 reserved
	scd_b                           : 1,	//    16
	pit_err_b                       : 1,	//    17
	                                : 2,	// 18:19 reserved
	scd_a                           : 1,	//    20
	pit_err_a                       : 1;	//    21
} INTR_EN;

/*-----------------------------------------------------------------------------
	0x0008 intr_st ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	almost_b                        : 1,	//     0
	rep_b                           : 1,	//     1
	up_err_b                        : 1,	//     2
	                                : 1,	//     3 reserved
	almost_a                        : 1,	//     4
	rep_a                           : 1,	//     5
	up_err_a                        : 1,	//     6
	                                : 1,	//     7 reserved
	unit_buf_b                      : 1,	//     8
	dn_err_b                        : 1,	//     9
	sync_err_b                      : 1,	//    10
	                                : 1,	//    11 reserved
	unit_buf_a                      : 1,	//    12
	dn_err_a                        : 1,	//    13
	sync_err_a                      : 1,	//    14
	                                : 1,	//    15 reserved
	scd_b                           : 1,	//    16
	pit_err_b                       : 1,	//    17
	                                : 2,	// 18:19 reserved
	scd_a                           : 1,	//    20
	pit_err_a                       : 1;	//    21
} INTR_ST;

/*-----------------------------------------------------------------------------
	0x000c intr_cl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	almost_b                        : 1,	//     0
	rep_b                           : 1,	//     1
	up_err_b                        : 1,	//     2
	                                : 1,	//     3 reserved
	almost_a                        : 1,	//     4
	rep_a                           : 1,	//     5
	up_err_a                        : 1,	//     6
	                                : 1,	//     7 reserved
	unit_buf_b                      : 1,	//     8
	dn_err_b                        : 1,	//     9
	sync_err_b                      : 1,	//    10
	                                : 1,	//    11 reserved
	unit_buf_a                      : 1,	//    12
	dn_err_a                        : 1,	//    13
	sync_err_a                      : 1,	//    14
	                                : 1,	//    15 reserved
	scd_b                           : 1,	//    16
	pit_err_b                       : 1,	//    17
	                                : 2,	// 18:19 reserved
	scd_a                           : 1,	//    20
	pit_err_a                       : 1;	//    21
} INTR_CL;



/*-----------------------------------------------------------------------------
	DVR PIE
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	0x0000 pie_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :24,	//  0:23 reserved
	iack                            : 1,	//    24
	                                : 3,	// 25:27 reserved
	rst                             : 1;	//    28
} PIE_CONF;

/*-----------------------------------------------------------------------------
	0x0004 pie_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pie_mode                        : 2,	//  0: 1
	astc_ctr                        : 1,	//     2
	gscd_en                         : 1,	//     3
	scd0_bsel                       : 1,	//     4
	scd1_bsel                       : 1,	//     5
	scd2_bsel                       : 1,	//     6
	scd3_bsel                       : 1,	//     7
	strm_id                         : 4,	//  8:11
	strm_sel                        : 1,	//    12
	plen_chk                        : 1,	//    13
	                                : 2,	// 14:15 reserved
	scd0_en                         : 1,	//    16
	scd1_en                         : 1,	//    17
	scd2_en                         : 1,	//    18
	scd3_en                         : 1;	//    19
} PIE_MODE;

/*-----------------------------------------------------------------------------
	0x0010 pie_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pind_cnt                        : 6;	//  0: 5
} PIE_STAT;

/*-----------------------------------------------------------------------------
	0x0014 pie_fsm_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pie_state                       : 4,	//  0: 3
	trn_state                       : 4,	//  4: 7
	                                : 4,	//  8:11 reserved
	mpg1_sys                        : 1,	//    12
	                                : 3,	// 13:15 reserved
	pesdec_state                    : 4;	// 16:19
} PIE_FSM_STAT;

/*-----------------------------------------------------------------------------
	0x0018 pie_err_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pit                             : 1;	//     0
} PIE_ERR_STAT;

/*-----------------------------------------------------------------------------
	0x0020~0x002c pie_scd_value [0] ~ pie_scd_value [3] ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	scd_value                       ;   	// 31: 0
} PIE_SCD_VALUE;


/*-----------------------------------------------------------------------------
	0x0030~0x003c pie_scd_mask [0] ~ pie_scd_mask [3] ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	scd_mask                        ;   	// 31: 0
} PIE_SCD_MASK;

/*-----------------------------------------------------------------------------
	0x0080~0x00fc pie_ind [0] ~ pie_ind [31] ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pack_cnt                        :14,	//  0:13
	buf_num                         : 5,	// 14:18
	maxp                            : 1,	//    19
	scd0_bdet                       : 1,	//    20
	scd1_pdet                       : 1,	//    21
	scd2_idet                       : 1,	//    22
	scd3_sdet                       : 1,	//    23
	byte_info                       : 8;	// 24:31
} PIE_IND;




/*-----------------------------------------------------------------------------
	DVR Download Controller
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	0x0000 dn_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bypass_pie                      : 1,	//     0
	vid_type                        : 1,	//     1
	type                            : 1;	//     2
} DN_CONF;

/*-----------------------------------------------------------------------------
	0x0004 dn_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	record                          : 1,	//     0
	paus_rec                        : 1,	//     1
	l_cnt_rst                       : 1,	//     2
	                                :25,	//  3:27 reserved
	reset_sw                        : 1;	//    28
} DN_CTRL;

/*-----------------------------------------------------------------------------
	0x0008 dn_vpid ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pid                             :13;	//  0:12
} DN_VPID;

/*-----------------------------------------------------------------------------
	0x000c dn_buf_lim ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pkt_cnt_lim                     :14,	//  0:13
	                                : 2,	// 14:15 reserved
	buf_num_lim                     : 5;	// 16:20
} DN_BUF_LIM;

/*-----------------------------------------------------------------------------
	0x0010 dn_buf_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pkt_cnt                         :14,	//  0:13
	                                : 2,	// 14:15 reserved
	buf_num                         : 5;	// 16:20
} DN_BUF_INFO;

/*-----------------------------------------------------------------------------
	0x0014 dn_stc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stc_resolution                  : 7,	//  0: 6
	stc_src                         : 1;	//     7
} DN_STC_CTRL;

/*-----------------------------------------------------------------------------
	0x0018 dn_buf_sptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	dn_buf_sptr                     :20;	// 12:31
} DN_BUF_SPTR;

/*-----------------------------------------------------------------------------
	0x001c dn_buf_eptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	dn_buf_eptr                     :20;	// 12:31
} DN_BUF_EPTR;

/*-----------------------------------------------------------------------------
	0x0020 dn_buf_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	dn_buf_wptr                     :29;	//  3:31
} DN_BUF_WPTR;



/*-----------------------------------------------------------------------------
	DVR Upload Controller
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	0x0000 up_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	upen                            : 1,	//     0
	upps                            : 1,	//     1
	rep                             : 1,	//     2
	tcp                             : 1,	//     3
	                                :20,	//  4:23 reserved
	iack                            : 3,	// 24:26
	                                : 2,	// 27:28 reserved
	nm_rst                          : 1;	//    29
} UP_CONF;

/*-----------------------------------------------------------------------------
	0x0004 up_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	play_mod                        : 3,	//  0: 2
	its_mod                         : 1,	//     3
	                                : 1,	//     4 reserved
	tsc                             : 1,	//     5
	                                : 1,	//     6 reserved
	atcp                            : 1,	//     7
	up_type                         : 3,	//  8:10
	flush                           : 1,	//    11
	num_serr                        : 3,	// 12:14
	                                : 1,	//    15 reserved
	wait_cycle                      :16;	// 16:31
} UP_MODE;

/*-----------------------------------------------------------------------------
	0x0008 up_al_full_lev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	al_full_lev                     :20;	// 12:31
} UP_AL_FULL_LEV;

/*-----------------------------------------------------------------------------
	0x000c up_al_empty_lev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	al_empty_lev                    :20;	// 12:31
} UP_AL_EMPTY_LEV;

/*-----------------------------------------------------------------------------
	0x0010 up_buf_sptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	up_buf_sptr                     :20;	// 12:31
} UP_BUF_SPTR;

/*-----------------------------------------------------------------------------
	0x0014 up_buf_eptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
	up_buf_eptr                     :20;	// 12:31
} UP_BUF_EPTR;

/*-----------------------------------------------------------------------------
	0x0018 up_buf_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	up_buf_wptr                     :30;	//  2:31
} UP_BUF_WPTR;

/*-----------------------------------------------------------------------------
	0x001c up_buf_rptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	up_buf_rptr                     :30;	//  2:31
} UP_BUF_RPTR;

/*-----------------------------------------------------------------------------
	0x0020 up_buf_pptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	up_buf_pptr                     :30;	//  2:31
} UP_BUF_PPTR;

/*-----------------------------------------------------------------------------
	0x0024 up_max_jitter ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	al_jitter                       ;   	// 31: 0
} UP_MAX_JITTER;

/*-----------------------------------------------------------------------------
	0x0028 up_buf_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	al_ist                          : 2,	//  0: 1
	                                :14,	//  2:15 reserved
	al_imsk                         : 2;	// 16:17
} UP_BUF_STAT;

/*-----------------------------------------------------------------------------
	0x0034 up_tmstamp_sel ''
------------------------------------------------------------------------------*/
typedef struct  {
	UINT32
	tmstamp_sel                     : 1;	//     0
} UP_TMSTAMP_SEL;

/*-----------------------------------------------------------------------------
	0x0050 up_err_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sync_err                        : 1,	//     0
	overlap                         : 1;	//     1
} UP_ERR_STAT;

/*-----------------------------------------------------------------------------
	0x0054 up_tsc_jitter ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tsc_jitter                      ;   	// 31: 0
} UP_TSC_JITTER;

/*-----------------------------------------------------------------------------
	0x0058 up_stat0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	up_state                        : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	ecnt                            : 2,	//  8: 9
	fcnt                            : 2,	// 10:11
	mrq                             : 1,	//    12
	invp                            : 1,	//    13
	                                : 2,	// 14:15 reserved
	dtype                           : 2;	// 16:17
} UP_STAT0;

/*-----------------------------------------------------------------------------
	0x005c up_stat1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pack_byte_cnt                   : 3,	//  0: 2
	                                : 5,	//  3: 7 reserved
	pack_word_cnt                   : 5,	//  8:12
	                                : 3,	// 13:15 reserved
	trans_word_cnt                  : 9;	// 16:24
} UP_STAT1;



#define PIE_MODE_ISR_PACKET_LIMIT	0x0		/* Interrupt only when packet limit */
#define PIE_MODE_ISR_TABLE_FULL		0x1		/* Interrupt only when index table is full (32 entries) */
#define PIE_MODE_ISR_BOTH			0x2		/* Interrupt when index table full or packet limit */


#define UP_CONF_IACK_ERR_IACK		(0x1 << 2)
#define UP_CONF_IACK_REP_IACK		(0x1 << 1)
#define UP_CONF_IACK_ALMOST_IACK	(0x1 << 0)


#define UP_BUF_STAT_AL_IST_FULL		(0x1 << 1)
#define UP_BUF_STAT_AL_IST_EMPTY	(0x1 << 0)

#define UP_ERR_STAT_OVERLAP			(0x1 << 1)
#define UP_ERR_STAT_SYNC_ERR		(0x1 << 0)


typedef struct {
	UINT32                          	                 __rsvd_00[   1];	// 0x0000
	INTR_EN                         	intr_en                         ;	// 0x0004 : ''
	INTR_ST                         	intr_st                         ;	// 0x0008 : ''
	INTR_CL                         	intr_cl                         ;	// 0x000c : ''
} DVR_TOP_REG_T;

typedef struct {
	PIE_CONF                        	pie_conf                        ;	// 0x0000 : ''
	PIE_MODE                        	pie_mode                        ;	// 0x0004 : ''
	UINT32                          	                 __rsvd_00[   2];	// 0x0008 ~ 0x000c
	PIE_STAT                        	pie_stat                        ;	// 0x0010 : ''
	PIE_FSM_STAT                    	pie_fsm_stat                    ;	// 0x0014 : ''
	PIE_ERR_STAT                    	pie_err_stat                    ;	// 0x0018 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x001c
	PIE_SCD_VALUE                   	pie_scd_value[4]                ;	// 0x0020 ~ 0x002c
	PIE_SCD_MASK                    	pie_scd_mask[4]                 ;	// 0x0030 ~ 0x003c
	UINT32                          	                 __rsvd_03[  16];	// 0x0040 ~ 0x007c
	PIE_IND                         	pie_ind[32]                     ;	// 0x0080 ~ 0x00fc
} DVR_PIE_REG_T;

typedef struct {
	DN_CONF                         	dn_conf                         ;	// 0x0000 : ''
	DN_CTRL                         	dn_ctrl                         ;	// 0x0004 : ''
	DN_VPID                         	dn_vpid                         ;	// 0x0008 : ''
	DN_BUF_LIM                      	dn_buf_lim                      ;	// 0x000c : ''
	DN_BUF_INFO                     	dn_buf_info                     ;	// 0x0010 : ''
	DN_STC_CTRL                     	dn_stc_ctrl                     ;	// 0x0014 : ''
	DN_BUF_SPTR                     	dn_buf_sptr                     ;	// 0x0018 : ''
	DN_BUF_EPTR                     	dn_buf_eptr                     ;	// 0x001c : ''
	DN_BUF_WPTR                     	dn_buf_wptr                     ;	// 0x0020 : ''
} DVR_DN_REG_T;

typedef struct {
	UP_CONF                         	up_conf                         ;	// 0x0000 : ''
	UP_MODE                         	up_mode                         ;	// 0x0004 : ''
	UP_AL_FULL_LEV                  	up_al_full_lev                  ;	// 0x0008 : ''
	UP_AL_EMPTY_LEV                 	up_al_empty_lev                 ;	// 0x000c : ''
	UP_BUF_SPTR                     	up_buf_sptr                     ;	// 0x0010 : ''
	UP_BUF_EPTR                     	up_buf_eptr                     ;	// 0x0014 : ''
	UP_BUF_WPTR                     	up_buf_wptr                     ;	// 0x0018 : ''
	UP_BUF_RPTR                     	up_buf_rptr                     ;	// 0x001c : ''
	UP_BUF_PPTR                     	up_buf_pptr                     ;	// 0x0020 : ''
	UP_MAX_JITTER                   	up_max_jitter                   ;	// 0x0024 : ''
	UP_BUF_STAT                     	up_buf_stat                     ;	// 0x0028 : ''
	UINT32                          	                 __rsvd_00[   2];	// 0x002c
	UP_TMSTAMP_SEL                   	up_tmstamp_sel;                 ;	// 0x0034
	UINT32                          	                 __rsvd_01[   6];	// 0x0038 ~ 0x004c
	UP_ERR_STAT                     	up_err_stat                     ;	// 0x0050 : ''
	UP_TSC_JITTER                   	up_tsc_jitter                   ;	// 0x0054 : ''
	UP_STAT0                        	up_stat0                        ;	// 0x0058 : ''
	UP_STAT1                        	up_stat1                        ;	// 0x005c : ''
} DVR_UP_REG_T;





#define PVR_REG_SIZE				0x500

#define PVR_TOP_REG_OFFSET			0x000
#define PVR_PIE_A_REG_OFFSET		0x100
#define PVR_PIE_B_REG_OFFSET		0x200
#define PVR_DN_A_REG_OFFSET			0x300
#define PVR_DN_B_REG_OFFSET			0x380
#define PVR_UP_A_REG_OFFSET			0x400
#define PVR_UP_B_REG_OFFSET			0x480



/* define bit field */
#define PVR_INTR_ALMOST_B		(0x1 << 0)
#define PVR_INTR_REP_B			(0x1 << 1)
#define PVR_INTR_UP_ERR_B		(0x1 << 2)
#define PVR_INTR_ALMOST_A		(0x1 << 4)
#define PVR_INTR_REP_A			(0x1 << 5)
#define PVR_INTR_UP_ERR_A		(0x1 << 6)
#define PVR_INTR_UP_MASK		(0xFF << 0)

#define PVR_INTR_UNIT_BUF_B		(0x1 << 8)
#define PVR_INTR_DN_ERR_B		(0x1 << 9)
#define PVR_INTR_SYNC_ERR_B		(0x1 << 10)
#define PVR_INTR_UNIT_BUF_A		(0x1 << 12)
#define PVR_INTR_DN_ERR_A		(0x1 << 13)
#define PVR_INTR_SYNC_ERR_A		(0x1 << 14)
#define PVR_INTR_DN_MASK		(0xFF << 8)

#define PVR_INTR_SCD_B			(0x1 << 16)
#define PVR_INTR_PIT_ERR_B		(0x1 << 17)
#define PVR_INTR_SCD_A			(0x1 << 20)
#define PVR_INTR_PIT_ERR_A		(0x1 << 21)
#define PVR_INTR_PIE_MASK		(0xFF << 16)






#endif /* !_PVR_REG_DB_H_ */
