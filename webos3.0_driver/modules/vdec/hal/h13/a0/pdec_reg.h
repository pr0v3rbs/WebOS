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
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     sooya.joo (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _PDEC_REG_H_
#define _PDEC_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0add reg_name ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0000 reg_pdec_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_scd_en                      : 1,	//     0
	reg_pdec_en                     : 1,	//     1
	reg_aub_intr_chk_en             : 1,	//     2
	                                : 1,	//     3 reserved
	reg_video_standard              : 4,	//  4: 7
	reg_pdec_endian                 : 2,	//  8: 9
	                                :18,	// 10:27 reserved
	reg_srst                        : 4;	// 28:31
} REG_PDEC_CONF;

/*-----------------------------------------------------------------------------
	0x0004 reg_trick_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_trick_mode                  ;   	// 31: 0
} REG_TRICK_MODE;

/*-----------------------------------------------------------------------------
	0x0008 reg_aux_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aux_mode                    ;   	// 31: 0
} REG_AUX_MODE;

/*-----------------------------------------------------------------------------
	0x000c reg_seq_err_code ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_seq_error_code              ;   	// 31: 0
} REG_SEQ_ERR_CODE;

/*-----------------------------------------------------------------------------
	0x0010 reg_cpb_base ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_base                    ;   	// 31: 0
} REG_CPB_BASE;

/*-----------------------------------------------------------------------------
	0x0014 reg_cpb_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_size                    ;   	// 31: 0
} REG_CPB_SIZE;

/*-----------------------------------------------------------------------------
	0x0018 reg_cpb_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_wptr                    ;   	// 31: 0
} REG_CPB_WPTR;

/*-----------------------------------------------------------------------------
	0x001c reg_cpb_rptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_rptr                    ;   	// 31: 0
} REG_CPB_RPTR;

/*-----------------------------------------------------------------------------
	0x0020 reg_cpb_afull_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_afull_level             ;   	// 31: 0
} REG_CPB_AFULL_LEVEL;

/*-----------------------------------------------------------------------------
	0x0024 reg_cpb_aempty_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpb_aempty_level            ;   	// 31: 0
} REG_CPB_AEMPTY_LEVEL;

/*-----------------------------------------------------------------------------
	0x0028 reg_ies_afull_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ies_afull_level             : 8;	//  0: 7
} REG_IES_AFULL_LEVEL;

/*-----------------------------------------------------------------------------
	0x002c reg_aub_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_aempty_level            : 8,	//  0: 7
	reg_aub_afull_level             : 8;	//  8:15
} REG_AUB_LEVEL;

/*-----------------------------------------------------------------------------
	0x0030 reg_aub_base ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_base                    ;   	// 31: 0
} REG_AUB_BASE;

/*-----------------------------------------------------------------------------
	0x0034 reg_aub_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_size                    ;   	// 31: 0
} REG_AUB_SIZE;

/*-----------------------------------------------------------------------------
	0x0038 reg_aub_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_wptr                    ;   	// 31: 0
} REG_AUB_WPTR;

/*-----------------------------------------------------------------------------
	0x003c reg_aub_rptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_rptr                    ;   	// 31: 0
} REG_AUB_RPTR;

/*-----------------------------------------------------------------------------
	0x0040 status_cpb_buf_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	status_cpb_buffer_level         ;   	// 31: 0
} STATUS_CPB_BUF_LEVEL;

/*-----------------------------------------------------------------------------
	0x0044 status_ies_buf_level ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	status_ies_buffer_level         : 8;	//  0: 7
} STATUS_IES_BUF_LEVEL;

/*-----------------------------------------------------------------------------
	0x0048 status_buf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	status_pesdec                   : 8,	//  0: 7
	/*status_aub_almost_full*/          : 1,	//     8
	status_cpb_almost_empty         : 1,	//     9
	status_cpb_almosot_full         : 1,	//    10
	status_aub_almost_empty         : 1,	//    11
	status_aub_almost_full          : 1,	//    12
	status_ies_almost_full          : 1,	//    13
	                                : 2,	// 14:15 reserved
	status_bdrc_almost_empty        : 1,	//    16
	status_bdrc_almost_full         : 1;	//    17
} STATUS_BUF;

/*-----------------------------------------------------------------------------
	0x004c status_aub ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_cnt                     :20,	//  0:19
	                                : 4,	// 20:23 reserved
	reg_aub_status                  : 8;	// 24:31
} STATUS_AUB;

/*-----------------------------------------------------------------------------
	0x0050 reg_udef_scd0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd0                   ;   	// 31: 0
} REG_UDEF_SCD0;

/*-----------------------------------------------------------------------------
	0x0054 reg_udef_scd1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd1                   ;   	// 31: 0
} REG_UDEF_SCD1;

/*-----------------------------------------------------------------------------
	0x0058 reg_udef_scd2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd2                   ;   	// 31: 0
} REG_UDEF_SCD2;

/*-----------------------------------------------------------------------------
	0x005c reg_udef_scd3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd3                   ;   	// 31: 0
} REG_UDEF_SCD3;

/*-----------------------------------------------------------------------------
	0x0060 reg_udef_scd4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd4                   ;   	// 31: 0
} REG_UDEF_SCD4;

/*-----------------------------------------------------------------------------
	0x0064 reg_udef_scd5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd5                   ;   	// 31: 0
} REG_UDEF_SCD5;

/*-----------------------------------------------------------------------------
	0x0068 reg_udef_scd6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_udef_scd6                   ;   	// 31: 0
} REG_UDEF_SCD6;

/*-----------------------------------------------------------------------------
	0x006c reg_udef_scd_num ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	reg_udef_scd6                   : 4,	//  4: 7
	reg_udef_scd5                   : 4,	//  8:11
	reg_udef_scd4                   : 4,	// 12:15
	reg_udef_scd3                   : 4,	// 16:19
	reg_udef_scd2                   : 4,	// 20:23
	reg_udef_scd1                   : 4,	// 24:27
	reg_udef_scd0                   : 4;	// 28:31
} REG_UDEF_SCD_NUM;

/*-----------------------------------------------------------------------------
	0x0070 reg_bdrc_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_dtype                  : 4,	//  0: 3
	reg_bdrc_q_depth                : 4,	//  4: 7
	reg_bdrc_endian                 : 1,	//     8
	                                : 7,	//  9:15 reserved
	reg_wait_cycle                  : 8,	// 16:23
	reg_wptr_upd                    : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_bdrc_rst                    : 1,	//    28
	                                : 2,	// 29:30 reserved
	reg_bdrc_en                     : 1;	//    31
} REG_BDRC_CONF;

/*-----------------------------------------------------------------------------
	0x0074 reg_bdrc_buf_base ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_base                   ;   	// 31: 0
} REG_BDRC_BUF_BASE;

/*-----------------------------------------------------------------------------
	0x0078 reg_bdrc_buf_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_size                   ;   	// 31: 0
} REG_BDRC_BUF_SIZE;

/*-----------------------------------------------------------------------------
	0x007c reg_bdrc_buf_wptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_wptr                   ;   	// 31: 0
} REG_BDRC_BUF_WPTR;

/*-----------------------------------------------------------------------------
	0x0080 reg_bdrc_buf_rptr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_rptr                   ;   	// 31: 0
} REG_BDRC_BUF_RPTR;

/*-----------------------------------------------------------------------------
	0x0084 reg_bdrc_buf_elev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_buf_elev               ;   	// 31: 0
} REG_BDRC_BUF_ELEV;

/*-----------------------------------------------------------------------------
	0x0088 reg_bdrc_buf_flev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_bdrc_buf_flev               ;   	// 31: 0
} REG_BDRC_BUF_FLEV;

/*-----------------------------------------------------------------------------
	0x008c reg_gmau_txid ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_cmd_txid               ;   	// 31: 0
} REG_GMAU_TXID;

/*-----------------------------------------------------------------------------
	0x0090 reg_pes_header_flag ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :21,	//  0:20 reserved
	pes_extension_flag_2            : 1,	//    21
	p_std_buffer_flag               : 1,	//    22
	program_packet_sequence_counter_: 1,	//    23
	pack_header_field_flag          : 1,	//    24
	pes_private_data_flag           : 1,	//    25
	pes_extension_flag              : 1,	//    26
	pes_crc_flag                    : 1,	//    27
	additional_copy_info_flag       : 1,	//    28
	dsm_trick_mode_flag             : 1,	//    29
	es_rate_flag                    : 1,	//    30
	escr_flag                       : 1;	//    31
} REG_PES_HEADER_FLAG;

/*-----------------------------------------------------------------------------
	0x0094 reg_escr_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_escr_base_32to1             ;   	// 31: 0
} REG_ESCR_1;

/*-----------------------------------------------------------------------------
	0x0098 reg_escr_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_excr_base_0                 : 1,	//     0
	                                : 7,	//  1: 7 reserved
	reg_escr_extension              : 9;	//  8:16
} REG_ESCR_2;

/*-----------------------------------------------------------------------------
	0x009c reg_es_rate ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_es_rate                     :22;	//  0:21
} REG_ES_RATE;

/*-----------------------------------------------------------------------------
	0x00a0 reg_dsm_trick ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	reg_field_id                    : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_rep_cntrl                   : 5,	// 12:16
	                                : 3,	// 17:19 reserved
	reg_frequency_truncation        : 2,	// 20:21
	                                : 2,	// 22:23 reserved
	reg_intra_slice_refresh         : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_trick_mode_contol           : 2;	// 28:29
} REG_DSM_TRICK;

/*-----------------------------------------------------------------------------
	0x00a4 reg_add_copy_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_additional_copy_info        : 7;	//  0: 6
} REG_ADD_COPY_INFO;

/*-----------------------------------------------------------------------------
	0x00a8 reg_prv_pes_pack_crc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_previous_pes_packet_crc     :16;	//  0:15
} REG_PRV_PES_PACK_CRC;

/*-----------------------------------------------------------------------------
	0x00b0 reg_pes_pri_data_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pes_private_data_127_96     ;   	// 31: 0
} REG_PES_PRI_DATA_3;

/*-----------------------------------------------------------------------------
	0x00b4 reg_pes_pri_data_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pes_private_data_95_64      ;   	// 31: 0
} REG_PES_PRI_DATA_2;

/*-----------------------------------------------------------------------------
	0x00b8 reg_pes_pri_data_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pes_private_data_64_32      ;   	// 31: 0
} REG_PES_PRI_DATA_1;

/*-----------------------------------------------------------------------------
	0x00bc reg_pes_pri_data_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pes_private_data_31_0       ;   	// 31: 0
} REG_PES_PRI_DATA_0;

/*-----------------------------------------------------------------------------
	0x00c0 reg_pack_header ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pack_field_length           : 8;	//  0: 7
} REG_PACK_HEADER;

/*-----------------------------------------------------------------------------
	0x00c4 reg_prg_pack_seq_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_original_stuff_length       : 6,	//  0: 5
	reg_mpeg1_mpeg2_identifier      : 1,	//     6
	                                : 1,	//     7 reserved
	reg_program_packet_sequence_coun: 7;	//  8:14
} REG_PRG_PACK_SEQ_CNT;

/*-----------------------------------------------------------------------------
	0x00c8 reg_p_std_buffer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_p_std_buffer_size           :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_p_std_buffer_scale          : 1;	//    16
} REG_P_STD_BUFFER;

/*-----------------------------------------------------------------------------
	0x00cc reg_pes_ext_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pes_extension_field_length  : 7;	//  0: 6
} REG_PES_EXT_2;

/*-----------------------------------------------------------------------------
	0x00d0 reg_pdec_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pdec_status                 ;   	// 31: 0
} REG_PDEC_STATUS;

/*-----------------------------------------------------------------------------
	0x00d4 reg_pdec_gmau_tr_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_transfer_cnt           ;   	// 31: 0
} REG_PDEC_GMAU_TR_CNT;

/*-----------------------------------------------------------------------------
	0x00d8 reg_pdec_st_es_wmif ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_es_wmif_status              ;   	// 31: 0
} REG_PDEC_ST_ES_WMIF;

/*-----------------------------------------------------------------------------
	0x00dc reg_pdec_st_aub_wmif ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aub_wr_status               ;   	// 31: 0
} REG_PDEC_ST_AUB_WMIF;

/*-----------------------------------------------------------------------------
	0x00fc reg_last_mod_date ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	last_modification_date          ;   	// 31: 0
} REG_LAST_MOD_DATE;

typedef struct {
	REG_PDEC_CONF                   	reg_pdec_conf                   ;	// 0x0000 : ''
	UINT32                  			reg_trick_mode                  ;	// 0x0004 : ''
	UINT32                    			reg_aux_mode                    ;	// 0x0008 : ''
	UINT32                				reg_seq_err_code                ;	// 0x000c : ''
	UINT32                    			reg_cpb_base                    ;	// 0x0010 : ''
	UINT32                    			reg_cpb_size                    ;	// 0x0014 : ''
	UINT32                    			reg_cpb_wptr                    ;	// 0x0018 : ''
	UINT32                    			reg_cpb_rptr                    ;	// 0x001c : ''
	UINT32             					reg_cpb_afull_level             ;	// 0x0020 : ''
	UINT32            					reg_cpb_aempty_level            ;	// 0x0024 : ''
	REG_IES_AFULL_LEVEL             	reg_ies_afull_level             ;	// 0x0028 : ''
	REG_AUB_LEVEL                   	reg_aub_level                   ;	// 0x002c : ''
	UINT32                    			reg_aub_base                    ;	// 0x0030 : ''
	UINT32                    			reg_aub_size                    ;	// 0x0034 : ''
	UINT32                    			reg_aub_wptr                    ;	// 0x0038 : ''
	UINT32                    			reg_aub_rptr                    ;	// 0x003c : ''
	UINT32            					status_cpb_buf_level            ;	// 0x0040 : ''
	STATUS_IES_BUF_LEVEL            	status_ies_buf_level            ;	// 0x0044 : ''
	STATUS_BUF                      	status_buf                      ;	// 0x0048 : ''
	STATUS_AUB                      	status_aub                      ;	// 0x004c : ''
	REG_UDEF_SCD0                   	reg_udef_scd0                   ;	// 0x0050 : ''
	REG_UDEF_SCD1                   	reg_udef_scd1                   ;	// 0x0054 : ''
	REG_UDEF_SCD2                   	reg_udef_scd2                   ;	// 0x0058 : ''
	REG_UDEF_SCD3                   	reg_udef_scd3                   ;	// 0x005c : ''
	REG_UDEF_SCD4                   	reg_udef_scd4                   ;	// 0x0060 : ''
	REG_UDEF_SCD5                   	reg_udef_scd5                   ;	// 0x0064 : ''
	REG_UDEF_SCD6                   	reg_udef_scd6                   ;	// 0x0068 : ''
	REG_UDEF_SCD_NUM                	reg_udef_scd_num                ;	// 0x006c : ''
	REG_BDRC_CONF                   	reg_bdrc_conf                   ;	// 0x0070 : ''
	REG_BDRC_BUF_BASE               	reg_bdrc_buf_base               ;	// 0x0074 : ''
	REG_BDRC_BUF_SIZE               	reg_bdrc_buf_size               ;	// 0x0078 : ''
	REG_BDRC_BUF_WPTR               	reg_bdrc_buf_wptr               ;	// 0x007c : ''
	REG_BDRC_BUF_RPTR               	reg_bdrc_buf_rptr               ;	// 0x0080 : ''
	REG_BDRC_BUF_ELEV               	reg_bdrc_buf_elev               ;	// 0x0084 : ''
	REG_BDRC_BUF_FLEV               	reg_bdrc_buf_flev               ;	// 0x0088 : ''
	REG_GMAU_TXID                   	reg_gmau_txid                   ;	// 0x008c : ''
	REG_PES_HEADER_FLAG             	reg_pes_header_flag             ;	// 0x0090 : ''
	REG_ESCR_1                      	reg_escr_1                      ;	// 0x0094 : ''
	REG_ESCR_2                      	reg_escr_2                      ;	// 0x0098 : ''
	REG_ES_RATE                     	reg_es_rate                     ;	// 0x009c : ''
	REG_DSM_TRICK                   	reg_dsm_trick                   ;	// 0x00a0 : ''
	REG_ADD_COPY_INFO               	reg_add_copy_info               ;	// 0x00a4 : ''
	REG_PRV_PES_PACK_CRC            	reg_prv_pes_pack_crc            ;	// 0x00a8 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x00ac
	REG_PES_PRI_DATA_3              	reg_pes_pri_data_3              ;	// 0x00b0 : ''
	REG_PES_PRI_DATA_2              	reg_pes_pri_data_2              ;	// 0x00b4 : ''
	REG_PES_PRI_DATA_1              	reg_pes_pri_data_1              ;	// 0x00b8 : ''
	REG_PES_PRI_DATA_0              	reg_pes_pri_data_0              ;	// 0x00bc : ''
	REG_PACK_HEADER                 	reg_pack_header                 ;	// 0x00c0 : ''
	REG_PRG_PACK_SEQ_CNT            	reg_prg_pack_seq_cnt            ;	// 0x00c4 : ''
	REG_P_STD_BUFFER                	reg_p_std_buffer                ;	// 0x00c8 : ''
	REG_PES_EXT_2                   	reg_pes_ext_2                   ;	// 0x00cc : ''
	REG_PDEC_STATUS                 	reg_pdec_status                 ;	// 0x00d0 : ''
	REG_PDEC_GMAU_TR_CNT            	reg_pdec_gmau_tr_cnt            ;	// 0x00d4 : ''
	REG_PDEC_ST_ES_WMIF             	reg_pdec_st_es_wmif             ;	// 0x00d8 : ''
	REG_PDEC_ST_AUB_WMIF            	reg_pdec_st_aub_wmif            ;	// 0x00dc : ''
	UINT32                          	                 __rsvd_01[   7];	// 0x00e0 ~ 0x00f8
	REG_LAST_MOD_DATE               	reg_last_mod_date               ;	// 0x00fc : ''
} PDEC_REG_T;
/* 56 regs, 56 types */

/* 57 regs, 57 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegPDEC : real register of PDEC.
 * gpRegPDEC     : shadow register.
 *
 * @def PDEC_RdFL: Read  FLushing : Shadow <- Real.
 * @def PDEC_WrFL: Write FLushing : Shadow -> Real.
 * @def PDEC_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def PDEC_Wr  : Write whole register(UINT32) from Shadow register.
 * @def PDEC_Rd01 ~ PDEC_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def PDEC_Wr01 ~ PDEC_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define PDEC_RdFL(_r)			((gpRegPDEC->_r)=(gpRealRegPDEC->_r))
#define PDEC_WrFL(_r)			((gpRealRegPDEC->_r)=(gpRegPDEC->_r))

#define PDEC_Rd(_r)			*((UINT32*)(&(gpRegPDEC->_r)))
#define PDEC_Wr(_r,_v)			((PDEC_Rd(_r))=((UINT32)(_v)))

#define PDEC_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
								} while(0)

#define PDEC_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
								} while(0)

#define PDEC_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
								} while(0)

#define PDEC_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
								} while(0)

#define PDEC_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
								} while(0)

#define PDEC_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
								} while(0)

#define PDEC_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
								} while(0)

#define PDEC_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
								} while(0)

#define PDEC_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
								} while(0)

#define PDEC_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
								} while(0)

#define PDEC_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
								} while(0)

#define PDEC_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
									(_v12) = (gpRegPDEC->_r._f12);				\
								} while(0)

#define PDEC_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
									(_v12) = (gpRegPDEC->_r._f12);				\
									(_v13) = (gpRegPDEC->_r._f13);				\
								} while(0)

#define PDEC_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
									(_v12) = (gpRegPDEC->_r._f12);				\
									(_v13) = (gpRegPDEC->_r._f13);				\
									(_v14) = (gpRegPDEC->_r._f14);				\
								} while(0)

#define PDEC_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
									(_v12) = (gpRegPDEC->_r._f12);				\
									(_v13) = (gpRegPDEC->_r._f13);				\
									(_v14) = (gpRegPDEC->_r._f14);				\
									(_v15) = (gpRegPDEC->_r._f15);				\
								} while(0)

#define PDEC_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegPDEC->_r._f01);				\
									(_v02) = (gpRegPDEC->_r._f02);				\
									(_v03) = (gpRegPDEC->_r._f03);				\
									(_v04) = (gpRegPDEC->_r._f04);				\
									(_v05) = (gpRegPDEC->_r._f05);				\
									(_v06) = (gpRegPDEC->_r._f06);				\
									(_v07) = (gpRegPDEC->_r._f07);				\
									(_v08) = (gpRegPDEC->_r._f08);				\
									(_v09) = (gpRegPDEC->_r._f09);				\
									(_v10) = (gpRegPDEC->_r._f10);				\
									(_v11) = (gpRegPDEC->_r._f11);				\
									(_v12) = (gpRegPDEC->_r._f12);				\
									(_v13) = (gpRegPDEC->_r._f13);				\
									(_v14) = (gpRegPDEC->_r._f14);				\
									(_v15) = (gpRegPDEC->_r._f15);				\
									(_v16) = (gpRegPDEC->_r._f16);				\
								} while(0)


#define PDEC_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
								} while(0)

#define PDEC_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
								} while(0)

#define PDEC_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
								} while(0)

#define PDEC_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
								} while(0)

#define PDEC_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
								} while(0)

#define PDEC_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
								} while(0)

#define PDEC_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
								} while(0)

#define PDEC_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
								} while(0)

#define PDEC_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
								} while(0)

#define PDEC_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
								} while(0)

#define PDEC_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
								} while(0)

#define PDEC_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
									(gpRegPDEC->_r._f12) = (_v12);				\
								} while(0)

#define PDEC_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
									(gpRegPDEC->_r._f12) = (_v12);				\
									(gpRegPDEC->_r._f13) = (_v13);				\
								} while(0)

#define PDEC_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
									(gpRegPDEC->_r._f12) = (_v12);				\
									(gpRegPDEC->_r._f13) = (_v13);				\
									(gpRegPDEC->_r._f14) = (_v14);				\
								} while(0)

#define PDEC_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
									(gpRegPDEC->_r._f12) = (_v12);				\
									(gpRegPDEC->_r._f13) = (_v13);				\
									(gpRegPDEC->_r._f14) = (_v14);				\
									(gpRegPDEC->_r._f15) = (_v15);				\
								} while(0)

#define PDEC_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegPDEC->_r._f01) = (_v01);				\
									(gpRegPDEC->_r._f02) = (_v02);				\
									(gpRegPDEC->_r._f03) = (_v03);				\
									(gpRegPDEC->_r._f04) = (_v04);				\
									(gpRegPDEC->_r._f05) = (_v05);				\
									(gpRegPDEC->_r._f06) = (_v06);				\
									(gpRegPDEC->_r._f07) = (_v07);				\
									(gpRegPDEC->_r._f08) = (_v08);				\
									(gpRegPDEC->_r._f09) = (_v09);				\
									(gpRegPDEC->_r._f10) = (_v10);				\
									(gpRegPDEC->_r._f11) = (_v11);				\
									(gpRegPDEC->_r._f12) = (_v12);				\
									(gpRegPDEC->_r._f13) = (_v13);				\
									(gpRegPDEC->_r._f14) = (_v14);				\
									(gpRegPDEC->_r._f15) = (_v15);				\
									(gpRegPDEC->_r._f16) = (_v16);				\
								} while(0)

/* Indexed Register Access.
 *
 * There is in-direct field specified by 'index' field within a register.
 * Normally a register has only one meaning for a 'field_name', but indexed register
 * can hold several data for a 'field_name' specifed by 'index' field of indexed register.
 * When writing an 3rd data for given 'field_name' register, you need to set 'rw' = 0, 'index' = 2,
 * and 'load' = 0.
 *
 * ASSUMPTION
 * For Writing indexed register load bit
 *
 * parameter list
 * _r     : name of register
 * _lname : name of load  bit field	: shall be 0 after macro executed.
 * _rwname: name of rw    bit field : shall be 0 after PDEC_Wind(), 1 for PDEC_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * PDEC_Rind : General indexed register Read.(
 * PDEC_Wind : General indexed register Read.
 *
 * PDEC_Ridx : For 'index', 'rw', 'load' field name
 * PDEC_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define PDEC_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								PDEC_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								PDEC_WrFL(_r);									\
								PDEC_RdFL(_r);									\
								PDEC_Rd01(_r,_fname,_fval);						\
							} while (0)

#define PDEC_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				PDEC_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define PDEC_Ridx(_r, _ival, _fname, _fval)	PDEC_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define PDEC_Widx(_r, _ival, _fname, _fval)	PDEC_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#define gpRealRegPDEC		stpPDEC_Reg
#define gpRegPDEC			stpPDEC_RegShadow

#ifdef __cplusplus
}
#endif

#endif	/* _PDEC_REG_H_ */

/* from 'PDEC.csv' 20110322 14:02:19   대한민국 표준시 by getregs v2.7 */
