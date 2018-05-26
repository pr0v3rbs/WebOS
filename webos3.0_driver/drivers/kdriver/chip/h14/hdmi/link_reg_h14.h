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


//#ifndef _REG_HDMI_LINK_H14_h
//#define _REG_HDMI_LINK_H14_h
/*-----------------------------------------------------------------------------
		0xc000c000L phy_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_arc_pdb                        : 1 ;  //     0     
	UINT32 phy_rstn                           : 1 ;  //     1     
	UINT32 phy_pdb                            : 1 ;  //     2     
	UINT32 phy_enable                         : 1 ;  //     3     
	UINT32 hpd0_oen                           : 1 ;  //     4     
	UINT32 hpd0_out                           : 1 ;  //     5     
	UINT32 ddc_write_mode                     : 2 ;  //  7: 6     
	UINT32                                    : 16;  // 23: 8     reserved
	UINT32 reg_rev_num                        : 8 ;  // 31:24     
} H14_PHY_00_T;

/*-----------------------------------------------------------------------------
		0xc000c004L phy_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0                 : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1                 : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res                  : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done                 : 1 ;  //    18     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 phy_bert_run_ch0                   : 1 ;  //    24     
	UINT32 phy_bert_run_ch1                   : 1 ;  //    25     
	UINT32 phy_bert_run_ch2                   : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 phy_bert_ch_sel                    : 2 ;  // 29:28     
	UINT32                                    : 2 ;  // 31:30   reserved
} H14_PHY_01_T;

/*-----------------------------------------------------------------------------
		0xc000c008L interrupt_enable_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_new_aud_enable                : 1 ;  //     0     
	UINT32 intr_new_spd_enable                : 1 ;  //     1     
	UINT32 intr_new_unr_enable                : 1 ;  //     2     
	UINT32 intr_cts_chg_enable                : 1 ;  //     3     
	UINT32 intr_n_chg_enable                  : 1 ;  //     4     
	UINT32 intr_fs_chg_enable                 : 1 ;  //     5     
	UINT32 intr_2pcm_chg_enable               : 1 ;  //     6     
	UINT32 intr_2npcm_chg_enable              : 1 ;  //     7     
	UINT32 intr_spdif_err_enable              : 1 ;  //     8     
	UINT32 intr_afifo_undr_enable             : 1 ;  //     9     
	UINT32 intr_afifo_ovrr_enable             : 1 ;  //    10     
	UINT32 intr_burst_info_enable             : 1 ;  //    11     
	UINT32 intr_vf_stable_enable              : 1 ;  //    12     
	UINT32 intr_vid_chg_enable                : 1 ;  //    13     
	UINT32 intr_vr_chg_enable                 : 1 ;  //    14     
	UINT32 intr_hr_chg_enable                 : 1 ;  //    15     
	UINT32 intr_po_chg_enable                 : 1 ;  //    16     
	UINT32 intr_il_chg_enable                 : 1 ;  //    17     
	UINT32 intr_vfifo_undr_enable             : 1 ;  //    18     
	UINT32 intr_vfifo_ovrr_enable             : 1 ;  //    19     
	UINT32 intr_no_vsi_enable                 : 1 ;  //    20     
	UINT32 intr_achst_5b_chg_int_enable       : 1 ;  //    21     
	UINT32                                    : 10;  // 31:22   reserved
} H14_INTERRUPT_ENABLE_00_T;

/*-----------------------------------------------------------------------------
		0xc000c00cL interrupt_enable_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_plug_in_enable                : 1 ;  //     0     
	UINT32 intr_plug_out_enable               : 1 ;  //     1     
	UINT32 intr_scdt_rise_enable              : 1 ;  //     2     
	UINT32 intr_scdt_fall_enable              : 1 ;  //     3     
	UINT32 intr_vsync_enable                  : 1 ;  //     4     
	UINT32 intr_hdmi_mode_enable              : 1 ;  //     5     
	UINT32 intr_set_mute_enable               : 1 ;  //     6     
	UINT32 intr_clr_mute_enable               : 1 ;  //     7     
	UINT32 intr_auth_init_enable              : 1 ;  //     8     
	UINT32 intr_auth_done_enable              : 1 ;  //     9     
	UINT32 intr_hdcp_err_enable               : 1 ;  //    10     
	UINT32 intr_ecc_err_enable                : 1 ;  //    11     
	UINT32 intr_terc4_err_enable              : 1 ;  //    12     
	UINT32 intr_acr_err_enable                : 1 ;  //    13     
	UINT32 intr_asp_err_enable                : 1 ;  //    14     
	UINT32 intr_no_avi_enable                 : 1 ;  //    15     
	UINT32 intr_no_gcp_enable                 : 1 ;  //    16     
	UINT32 intr_chg_avi_b12345_enable         : 1 ;  //    17     
	UINT32 intr_chg_avi_to_rgb_enable         : 1 ;  //    18     
	UINT32 intr_chg_avi_to_ycc444_enable      : 1 ;  //    19     
	UINT32 intr_chg_avi_to_ycc422_enable      : 1 ;  //    20     
	UINT32 intr_new_avi_enable                : 1 ;  //    21     
	UINT32 intr_new_vsi_enable                : 1 ;  //    22     
	UINT32 intr_chg_vsi_vformat_enable        : 1 ;  //    23     
	UINT32 intr_new_gcp_enable                : 1 ;  //    24     
	UINT32 intr_new_asp_enable                : 1 ;  //    25     
	UINT32 intr_new_acr_enable                : 1 ;  //    26     
	UINT32 intr_new_gbd_enable                : 1 ;  //    27     
	UINT32 intr_new_acp_enable                : 1 ;  //    28     
	UINT32 intr_new_isrc1_enable              : 1 ;  //    29     
	UINT32 intr_new_isrc2_enable              : 1 ;  //    30     
	UINT32 intr_new_mpg_enable                : 1 ;  //    31     
} H14_INTERRUPT_ENABLE_01_T;

/*-----------------------------------------------------------------------------
		0xc000c010L interrupt_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_new_gcp                       : 1 ;  //     0     
	UINT32 intr_new_asp                       : 1 ;  //     1     
	UINT32 intr_new_acr                       : 1 ;  //     2     
	UINT32 intr_new_gbd                       : 1 ;  //     3     
	UINT32 intr_new_acp                       : 1 ;  //     4     
	UINT32 intr_new_isrc1                     : 1 ;  //     5     
	UINT32 intr_new_isrc2                     : 1 ;  //     6     
	UINT32 intr_new_mpg                       : 1 ;  //     7     
	UINT32 intr_new_aud                       : 1 ;  //     8     
	UINT32 intr_new_spd                       : 1 ;  //     9     
	UINT32 intr_new_unr                       : 1 ;  //    10     
	UINT32 intr_cts_chg                       : 1 ;  //    11     
	UINT32 intr_n_chg                         : 1 ;  //    12     
	UINT32 intr_fs_chg                        : 1 ;  //    13     
	UINT32 intr_2pcm_chg                      : 1 ;  //    14     
	UINT32 intr_2npcm_chg                     : 1 ;  //    15     
	UINT32 intr_spdif_err                     : 1 ;  //    16     
	UINT32 intr_afifo_undr                    : 1 ;  //    17     
	UINT32 intr_afifo_ovrr                    : 1 ;  //    18     
	UINT32 intr_burst_info                    : 1 ;  //    19     
	UINT32 intr_vf_stable                     : 1 ;  //    20     
	UINT32 intr_vid_chg                       : 1 ;  //    21     
	UINT32 intr_vr_chg                        : 1 ;  //    22     
	UINT32 intr_hr_chg                        : 1 ;  //    23     
	UINT32 intr_po_chg                        : 1 ;  //    24     
	UINT32 intr_il_chg                        : 1 ;  //    25     
	UINT32 intr_vfifo_undr                    : 1 ;  //    26     
	UINT32 intr_vfifo_ovrr                    : 1 ;  //    27     
	UINT32 intr_achst_5b_chg_int              : 1 ;  //    28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H14_INTERRUPT_00_T;

/*-----------------------------------------------------------------------------
		0xc000c014L interrupt_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_plug_in                       : 1 ;  //     0     
	UINT32 intr_plug_out                      : 1 ;  //     1     
	UINT32 intr_scdt_rise                     : 1 ;  //     2     
	UINT32 intr_scdt_fall                     : 1 ;  //     3     
	UINT32 intr_vsync                         : 1 ;  //     4     
	UINT32 intr_hdmi_mode                     : 1 ;  //     5     
	UINT32 intr_set_mute                      : 1 ;  //     6     
	UINT32 intr_clr_mute                      : 1 ;  //     7     
	UINT32 intr_auth_init                     : 1 ;  //     8     
	UINT32 intr_auth_done                     : 1 ;  //     9     
	UINT32 intr_hdcp_err                      : 1 ;  //    10     
	UINT32 intr_ecc_err                       : 1 ;  //    11     
	UINT32 intr_terc4_err                     : 1 ;  //    12     
	UINT32 intr_acr_err                       : 1 ;  //    13     
	UINT32 intr_asp_err                       : 1 ;  //    14     
	UINT32 intr_no_avi                        : 1 ;  //    15     
	UINT32 intr_no_gcp                        : 1 ;  //    16     
	UINT32 intr_chg_avi_b12345                : 1 ;  //    17     
	UINT32 intr_chg_avi_to_rgb                : 1 ;  //    18     
	UINT32 intr_chg_avi_to_ycc444             : 1 ;  //    19     
	UINT32 intr_chg_avi_to_ycc422             : 1 ;  //    20     
	UINT32 intr_new_avi                       : 1 ;  //    21     
	UINT32 intr_new_vsi                       : 1 ;  //    22     
	UINT32 intr_chg_vsi_vformat               : 1 ;  //    23     
	UINT32 intr_no_vsi                        : 1 ;  //    24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H14_INTERRUPT_01_T;

/*-----------------------------------------------------------------------------
		0xc000c018L system_control_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_scdt                           : 1 ;  //     0     
	UINT32 reg_hdmi_mode                      : 1 ;  //     1     
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 reg_conn                           : 1 ;  //     3     
	UINT32 reg_preamble_cnt                   : 4 ;  //  7: 4     
	UINT32 reg_avc_en                         : 1 ;  //     8     
	UINT32 reg_aac_en                         : 1 ;  //     9     
	UINT32 reg_mhl_ppm                        : 1 ;  //    10     
	UINT32 reg_mhl_en                         : 1 ;  //    11     
	UINT32 reg_dbg_mhl_ppm                    : 1 ;  //    12     
	UINT32 reg_dbg_mhl_en                     : 1 ;  //    13     
	UINT32 reg_dbg_mhl_conf                   : 1 ;  //    14     
	UINT32                                    : 17;  // 31:15   reserved
} H14_SYSTEM_CONTROL_00_T;

/*-----------------------------------------------------------------------------
		0xc000c01cL system_control_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_afifo_pdiff                    : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vfifo_pdiff                    : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_afifo_ctrl_thr                 : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_vfifo_ctrl_thr                 : 5 ;  // 28:24     
	UINT32                                    : 3 ;  // 31:29   reserved
} H14_SYSTEM_CONTROL_01_T;

/*-----------------------------------------------------------------------------
		0xc000c020L system_control_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mute_aud_stat                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_mute_aud                       : 1 ;  //     4     
	UINT32 reg_mute_aud_nosync                : 1 ;  //     5     
	UINT32 reg_mute_aud_dvi                   : 1 ;  //     6     
	UINT32 reg_mute_aud_pol                   : 1 ;  //     7     
	UINT32 reg_mute_vid_stat                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_mute_vid                       : 1 ;  //    12     
	UINT32                                    : 11;  // 23:13     reserved
	UINT32 reg_dbg_link_bypass                : 1 ;  //    24     
	UINT32 reg_dbg_i2c_ck_inv                 : 1 ;  //    25     
	UINT32 reg_dbg_lvds_ck_inv                : 1 ;  //    26     
	UINT32 reg_dbg_lvds_ck_half               : 1 ;  //    27     
	UINT32                                    : 4 ;  // 31:28   reserved
} H14_SYSTEM_CONTROL_02_T;

/*-----------------------------------------------------------------------------
		0xc000c024L hdcp_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps                     : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_nosync             : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_mode_chg           : 1 ;  //     9     
	UINT32                                    : 22;  // 31:10   reserved
} H14_HDCP_00_T;

/*-----------------------------------------------------------------------------
		0xc000c028L hdcp_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0                      : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_1                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_2                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_3                      : 8 ;  // 31:24     
} H14_HDCP_01_T;

/*-----------------------------------------------------------------------------
		0xc000c02cL hdcp_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_4                      : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7                      : 8 ;  // 31:24     
} H14_HDCP_02_T;

/*-----------------------------------------------------------------------------
		0xc000c030L hdcp_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_1                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_2                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_3                    : 8 ;  // 31:24     
} H14_HDCP_03_T;

/*-----------------------------------------------------------------------------
		0xc000c034L hdcp_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_4                    : 8 ;  //  7: 0     
	UINT32                                    : 24;  // 31:8    reserved
} H14_HDCP_04_T;

/*-----------------------------------------------------------------------------
		0xc000c038L hdcp_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3                    : 8 ;  // 31:24     
} H14_HDCP_05_T;

/*-----------------------------------------------------------------------------
		0xc000c03cL hdcp_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_4                    : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_hdcp_ri_0                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_ri_1                      : 8 ;  // 31:24     
} H14_HDCP_06_T;

/*-----------------------------------------------------------------------------
		0xc000c040L hdcp_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_authed                    : 1 ;  //     0     
	UINT32 reg_hdcp_enc_en                    : 1 ;  //     1     
	UINT32 reg_hdcp_bypass_xor                : 1 ;  //     2     
	UINT32 reg_hdcp_disable                   : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_dbg_hdcp_bksv_ld               : 1 ;  //     8     
	UINT32 reg_dbg_hdcp_key_bak_en            : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_bist_init                      : 1 ;  //    16     
	UINT32 reg_bist_done                      : 1 ;  //    17     
	UINT32 reg_bist_err                       : 1 ;  //    18     
	UINT32                                    : 13;  // 31:19   reserved
} H14_HDCP_07_T;

/*-----------------------------------------------------------------------------
		0xc000c044L hdcp_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pgmr_addr_0                    : 8 ;  //  7: 0     
	UINT32 reg_pgmr_addr_1                    : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_pgmr_wdata                     : 8 ;  // 23:16     
	UINT32 reg_pgmr_rdata                     : 8 ;  // 31:24     
} H14_HDCP_08_T;

/*-----------------------------------------------------------------------------
		0xc000c048L hdcp_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pgmr_done                      : 1 ;  //     0     
	UINT32 reg_pgmr_rd                        : 1 ;  //     1     
	UINT32 reg_pgmr_wr                        : 1 ;  //     2     
	UINT32 reg_pgmr_en                        : 1 ;  //     3     
	UINT32                                    : 28;  // 31:4    reserved
} H14_HDCP_09_T;

/*-----------------------------------------------------------------------------
		0xc000c04cL packet_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_dec                        : 8 ;  //  7: 0     
	UINT32 reg_spd_dec                        : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_pkt_clr                        : 1 ;  //    24     
	UINT32 reg_pkt_clr_dvi                    : 1 ;  //    25     
	UINT32 reg_pkt_clr_nosync                 : 1 ;  //    26     
	UINT32                                    : 5 ;  // 31:27   reserved
} H14_PACKET_00_T;

/*-----------------------------------------------------------------------------
		0xc000c050L packet_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_01_T;

/*-----------------------------------------------------------------------------
		0xc000c054L packet_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_02_T;

/*-----------------------------------------------------------------------------
		0xc000c058L packet_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_03_T;

/*-----------------------------------------------------------------------------
		0xc000c05cL packet_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_04_T;

/*-----------------------------------------------------------------------------
		0xc000c060L packet_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_05_T;

/*-----------------------------------------------------------------------------
		0xc000c064L packet_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_06_T;

/*-----------------------------------------------------------------------------
		0xc000c068L packet_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_07_T;

/*-----------------------------------------------------------------------------
		0xc000c06cL packet_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_08_T;

/*-----------------------------------------------------------------------------
		0xc000c070L packet_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_09_T;

/*-----------------------------------------------------------------------------
		0xc000c074L packet_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_10_T;

/*-----------------------------------------------------------------------------
		0xc000c078L packet_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_11_T;

/*-----------------------------------------------------------------------------
		0xc000c07cL packet_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_12_T;

/*-----------------------------------------------------------------------------
		0xc000c080L packet_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_13_T;

/*-----------------------------------------------------------------------------
		0xc000c084L packet_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_14_T;

/*-----------------------------------------------------------------------------
		0xc000c088L packet_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_15_T;

/*-----------------------------------------------------------------------------
		0xc000c08cL packet_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_16_T;

/*-----------------------------------------------------------------------------
		0xc000c090L packet_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_asp_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_asp_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_17_T;

/*-----------------------------------------------------------------------------
		0xc000c094L packet_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_18_T;

/*-----------------------------------------------------------------------------
		0xc000c098L packet_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_19_T;

/*-----------------------------------------------------------------------------
		0xc000c09cL packet_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_20_T;

/*-----------------------------------------------------------------------------
		0xc000c0a0L packet_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_21_T;

/*-----------------------------------------------------------------------------
		0xc000c0a4L packet_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_22_T;

/*-----------------------------------------------------------------------------
		0xc000c0a8L packet_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_23_T;

/*-----------------------------------------------------------------------------
		0xc000c0acL packet_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_24_T;

/*-----------------------------------------------------------------------------
		0xc000c0b0L packet_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_25_T;

/*-----------------------------------------------------------------------------
		0xc000c0b4L packet_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_26_T;

/*-----------------------------------------------------------------------------
		0xc000c0b8L packet_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_27_T;

/*-----------------------------------------------------------------------------
		0xc000c0bcL packet_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_28_T;

/*-----------------------------------------------------------------------------
		0xc000c0c0L packet_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_29_T;

/*-----------------------------------------------------------------------------
		0xc000c0c4L packet_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_30_T;

/*-----------------------------------------------------------------------------
		0xc000c0c8L packet_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_31_T;

/*-----------------------------------------------------------------------------
		0xc000c0ccL packet_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_32_T;

/*-----------------------------------------------------------------------------
		0xc000c0d0L packet_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_33_T;

/*-----------------------------------------------------------------------------
		0xc000c0d4L packet_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_hbr_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_hbr_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_pkt_gcp_smute                  : 1 ;  //    24     
	UINT32 reg_pkt_gcp_cmute                  : 1 ;  //    25     
	UINT32                                    : 6 ;  // 31:26   reserved
} H14_PACKET_34_T;

/*-----------------------------------------------------------------------------
		0xc000c0d8L packet_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_hdr_1                : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_35_T;

/*-----------------------------------------------------------------------------
		0xc000c0dcL packet_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_3                : 8 ;  // 31:24     
} H14_PACKET_36_T;

/*-----------------------------------------------------------------------------
		0xc000c0e0L packet_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_7                : 8 ;  // 31:24     
} H14_PACKET_37_T;

/*-----------------------------------------------------------------------------
		0xc000c0e4L packet_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_11               : 8 ;  // 31:24     
} H14_PACKET_38_T;

/*-----------------------------------------------------------------------------
		0xc000c0e8L packet_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_15               : 8 ;  // 31:24     
} H14_PACKET_39_T;

/*-----------------------------------------------------------------------------
		0xc000c0ecL packet_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_19               : 8 ;  // 31:24     
} H14_PACKET_40_T;

/*-----------------------------------------------------------------------------
		0xc000c0f0L packet_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_23               : 8 ;  // 31:24     
} H14_PACKET_41_T;

/*-----------------------------------------------------------------------------
		0xc000c0f4L packet_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_27               : 8 ;  // 31:24     
} H14_PACKET_42_T;

/*-----------------------------------------------------------------------------
		0xc000c0f8L packet_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_hdr_1                : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_43_T;

/*-----------------------------------------------------------------------------
		0xc000c0fcL packet_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_3                : 8 ;  // 31:24     
} H14_PACKET_44_T;

/*-----------------------------------------------------------------------------
		0xc000c100L packet_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_7                : 8 ;  // 31:24     
} H14_PACKET_45_T;

/*-----------------------------------------------------------------------------
		0xc000c104L packet_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_11               : 8 ;  // 31:24     
} H14_PACKET_46_T;

/*-----------------------------------------------------------------------------
		0xc000c108L packet_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_15               : 8 ;  // 31:24     
} H14_PACKET_47_T;

/*-----------------------------------------------------------------------------
		0xc000c10cL packet_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_19               : 8 ;  // 31:24     
} H14_PACKET_48_T;

/*-----------------------------------------------------------------------------
		0xc000c110L packet_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_23               : 8 ;  // 31:24     
} H14_PACKET_49_T;

/*-----------------------------------------------------------------------------
		0xc000c114L packet_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_27               : 8 ;  // 31:24     
} H14_PACKET_50_T;

/*-----------------------------------------------------------------------------
		0xc000c118L packet_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_51_T;

/*-----------------------------------------------------------------------------
		0xc000c11cL packet_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_52_T;

/*-----------------------------------------------------------------------------
		0xc000c120L packet_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_53_T;

/*-----------------------------------------------------------------------------
		0xc000c124L packet_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_54_T;

/*-----------------------------------------------------------------------------
		0xc000c128L packet_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_55_T;

/*-----------------------------------------------------------------------------
		0xc000c12cL packet_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_56_T;

/*-----------------------------------------------------------------------------
		0xc000c130L packet_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_57_T;

/*-----------------------------------------------------------------------------
		0xc000c134L packet_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_58_T;

/*-----------------------------------------------------------------------------
		0xc000c138L packet_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_59_T;

/*-----------------------------------------------------------------------------
		0xc000c13cL packet_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_60_T;

/*-----------------------------------------------------------------------------
		0xc000c140L packet_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_61_T;

/*-----------------------------------------------------------------------------
		0xc000c144L packet_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_62_T;

/*-----------------------------------------------------------------------------
		0xc000c148L packet_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_63_T;

/*-----------------------------------------------------------------------------
		0xc000c14cL packet_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_64_T;

/*-----------------------------------------------------------------------------
		0xc000c150L packet_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_65_T;

/*-----------------------------------------------------------------------------
		0xc000c154L packet_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_66_T;

/*-----------------------------------------------------------------------------
		0xc000c158L packet_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_0                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_1                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_2                      : 8 ;  // 23:16     
	UINT32                                    : 8 ;  // 31:24   reserved
} H14_PACKET_67_T;

/*-----------------------------------------------------------------------------
		0xc000c15cL packet_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_3                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_4                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_5                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_6                      : 8 ;  // 31:24     
} H14_PACKET_68_T;

/*-----------------------------------------------------------------------------
		0xc000c160L packet_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_7                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_8                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_9                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_10                     : 8 ;  // 31:24     
} H14_PACKET_69_T;

/*-----------------------------------------------------------------------------
		0xc000c164L packet_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_11                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_12                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_13                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_14                     : 8 ;  // 31:24     
} H14_PACKET_70_T;

/*-----------------------------------------------------------------------------
		0xc000c168L packet_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_15                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_16                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_17                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_18                     : 8 ;  // 31:24     
} H14_PACKET_71_T;

/*-----------------------------------------------------------------------------
		0xc000c16cL packet_72 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_19                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_20                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_21                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_22                     : 8 ;  // 31:24     
} H14_PACKET_72_T;

/*-----------------------------------------------------------------------------
		0xc000c170L packet_73 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_23                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_24                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_25                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_26                     : 8 ;  // 31:24     
} H14_PACKET_73_T;

/*-----------------------------------------------------------------------------
		0xc000c174L packet_74 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_27                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_28                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_29                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_30                     : 8 ;  // 31:24     
} H14_PACKET_74_T;

/*-----------------------------------------------------------------------------
		0xc000c178L packet_75 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_hdr_1                  : 8 ;  // 15: 8     
	UINT32                                    : 16;  // 31:16   reserved
} H14_PACKET_75_T;

/*-----------------------------------------------------------------------------
		0xc000c17cL packet_76 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_3                  : 8 ;  // 31:24     
} H14_PACKET_76_T;

/*-----------------------------------------------------------------------------
		0xc000c180L packet_77 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_7                  : 8 ;  // 31:24     
} H14_PACKET_77_T;

/*-----------------------------------------------------------------------------
		0xc000c184L packet_78 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_11                 : 8 ;  // 31:24     
} H14_PACKET_78_T;

/*-----------------------------------------------------------------------------
		0xc000c188L packet_79 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_15                 : 8 ;  // 31:24     
} H14_PACKET_79_T;

/*-----------------------------------------------------------------------------
		0xc000c18cL packet_80 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_19                 : 8 ;  // 31:24     
} H14_PACKET_80_T;

/*-----------------------------------------------------------------------------
		0xc000c190L packet_81 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_23                 : 8 ;  // 31:24     
} H14_PACKET_81_T;

/*-----------------------------------------------------------------------------
		0xc000c194L packet_82 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_27                 : 8 ;  // 31:24     
} H14_PACKET_82_T;

/*-----------------------------------------------------------------------------
		0xc000c198L packet_83 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_vsi_hdmi_vic               : 8 ;  // 7:0    
	UINT32 reg_dbg_vsi_3d_struct              : 4 ;  // 11: 8     
	UINT32 reg_dbg_vsi_3d_meta_en             : 1 ;  //    12     
	UINT32 reg_dbg_vsi_3d                     : 1 ;  //    13     
	UINT32 reg_dbg_vsi_4k2k                   : 1 ;  //    14     
	UINT32                                    : 17;  // 31:15   reserved
} H14_PACKET_83_T;

/*-----------------------------------------------------------------------------
		0xc000c19cL video_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pr_tx                          : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_pr_rx                          : 4 ;  // 11: 8     
	UINT32 reg_pr_rx_en                       : 1 ;  //    12     
	UINT32 reg_pr_cmu_sync                    : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_pr_cbcr                        : 4 ;  // 19:16     
	UINT32                                    : 12;  // 31:20   reserved
} H14_VIDEO_00_T;

/*-----------------------------------------------------------------------------
		0xc000c1a0L video_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_align422_tx                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_align422_rx                    : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_align422_rx_en                 : 1 ;  //    12     
	UINT32 reg_align422_swap_rb               : 1 ;  //    13     
	UINT32                                    : 18;  // 31:14   reserved
} H14_VIDEO_01_T;

/*-----------------------------------------------------------------------------
		0xc000c1a4L video_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cmode_tx                       : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cmode_rx                       : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cmode_rx_en                    : 1 ;  //    16     
	UINT32 reg_cmode_ppll_sync                : 1 ;  //    17     
	UINT32                                    : 14;  // 31:18   reserved
} H14_VIDEO_02_T;

/*-----------------------------------------------------------------------------
		0xc000c1a8L video_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_neg_pol_en                     : 1 ;  //     0     
	UINT32 reg_inv_hs                         : 1 ;  //     1     
	UINT32 reg_inv_vs                         : 1 ;  //     2     
	UINT32 reg_inv_eo                         : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vf_clr                         : 1 ;  //     8     
	UINT32 reg_vf_clr_nosync                  : 1 ;  //     9     
	UINT32                                    : 22;  // 31:10   reserved
} H14_VIDEO_03_T;

/*-----------------------------------------------------------------------------
		0xc000c1acL video_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_tot                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_tot                          : 12;  // 27:16     
	UINT32                                    : 4 ;  // 31:28   reserved
} H14_VIDEO_04_T;

/*-----------------------------------------------------------------------------
		0xc000c1b0L video_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_av                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_av                           : 12;  // 27:16     
	UINT32                                    : 4 ;  // 31:28   reserved
} H14_VIDEO_05_T;

/*-----------------------------------------------------------------------------
		0xc000c1b4L video_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_fp                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_v_fp                           : 6 ;  // 21:16     
	UINT32                                    : 10;  // 31:22   reserved
} H14_VIDEO_06_T;

/*-----------------------------------------------------------------------------
		0xc000c1b8L video_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_bp                           : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_v_bp                           : 7 ;  // 22:16     
	UINT32                                    : 9 ;  // 31:23   reserved
} H14_VIDEO_07_T;

/*-----------------------------------------------------------------------------
		0xc000c1bcL video_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_sync                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_v_sync                         : 8 ;  // 23:16     
	UINT32                                    : 8 ;  // 31:24   reserved
} H14_VIDEO_08_T;

/*-----------------------------------------------------------------------------
		0xc000c1c0L video_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vid_blank_g                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_vid_blank_r                    : 12;  // 27:16     
	UINT32                                    : 4 ;  // 31:28   reserved
} H14_VIDEO_09_T;

/*-----------------------------------------------------------------------------
		0xc000c1c4L video_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_pol                          : 1 ;  //     0     
	UINT32 reg_v_pol                          : 1 ;  //     1     
	UINT32 reg_intrl                          : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_dbg_osd_on                     : 1 ;  //     8     
	UINT32 reg_dbg_osd_vf_clr                 : 1 ;  //     9     
	UINT32 reg_dbg_osd_af_clr                 : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_vid_blank_b                    : 12;  // 27:16     
	UINT32                                    : 4 ;  // 31:28   reserved
} H14_VIDEO_10_T;

/*-----------------------------------------------------------------------------
		0xc000c1c8L audio_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_clk_aud_div                : 2 ;  //  1: 0     
	UINT32 reg_acr_n_fs                       : 2 ;  //  3: 2     
	UINT32 reg_acr_en                         : 1 ;  //     4     
	UINT32 reg_acr_ncts_rx_en                 : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_acr_adj_thr                    : 4 ;  // 11: 8     
	UINT32 reg_acr_adj_en                     : 1 ;  //    12     
	UINT32                                    : 19;  // 31:13   reserved
} H14_AUDIO_00_T;

/*-----------------------------------------------------------------------------
		0xc000c1ccL audio_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_tx                       : 20;  // 19: 0     
	UINT32                                    : 12;  // 31:20   reserved
} H14_AUDIO_01_T;

/*-----------------------------------------------------------------------------
		0xc000c1d0L audio_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_rx                       : 20;  // 19: 0     
	UINT32                                    : 12;  // 31:20   reserved
} H14_AUDIO_02_T;

/*-----------------------------------------------------------------------------
		0xc000c1d4L audio_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_tx                     : 20;  // 19: 0     
	UINT32                                    : 12;  // 31:20   reserved
} H14_AUDIO_03_T;

/*-----------------------------------------------------------------------------
		0xc000c1d8L audio_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_rx                     : 20;  // 19: 0     
	UINT32                                    : 12;  // 31:20   reserved
} H14_AUDIO_04_T;

/*-----------------------------------------------------------------------------
		0xc000c1dcL audio_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_asp_layout                     : 1 ;  //     0     
	UINT32 reg_i2s_shift1                     : 1 ;  //     1     
	UINT32 reg_i2s_lsb_first                  : 1 ;  //     2     
	UINT32 reg_i2s_rj                         : 1 ;  //     3     
	UINT32 reg_i2s_ws_pol                     : 1 ;  //     4     
	UINT32 reg_i2s_sign_ext                   : 1 ;  //     5     
	UINT32 reg_i2s_16bit                      : 1 ;  //     6     
	UINT32 reg_i2s_sck_edge                   : 1 ;  //     7     
	UINT32 reg_i2s_pass_inval                 : 1 ;  //     8     
	UINT32 reg_i2s_pcm_only                   : 1 ;  //     9     
	UINT32 reg_i2s_vucp                       : 1 ;  //    10     
	UINT32 reg_i2s_mclk_en                    : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_i2s_sd_en                      : 4 ;  // 19:16     
	UINT32 reg_i2s_out_en                     : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_i2s_sd0_map                    : 2 ;  // 25:24     
	UINT32 reg_i2s_sd1_map                    : 2 ;  // 27:26     
	UINT32 reg_i2s_sd2_map                    : 2 ;  // 29:28     
	UINT32 reg_i2s_sd3_map                    : 2 ;  // 31:30     
} H14_AUDIO_05_T;

/*-----------------------------------------------------------------------------
		0xc000c1e0L audio_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sp_paerr                       : 1 ;  //     0     
	UINT32 reg_sp_pserr                       : 1 ;  //     1     
	UINT32 reg_sp_gnd_mode                    : 1 ;  //     2     
	UINT32 reg_sp_en                          : 1 ;  //     3     
	UINT32                                    : 28;  // 31:4    reserved
} H14_AUDIO_06_T;

/*-----------------------------------------------------------------------------
		0xc000c1e4L audio_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_ow_byte1                 : 8 ;  //  7: 0     
	UINT32 reg_achst_ow_bit2                  : 1 ;  //     8     
	UINT32 reg_achst_ow_en                    : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_achst_fs                       : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_achst_byte0                    : 8 ;  // 31:24     
} H14_AUDIO_07_T;

/*-----------------------------------------------------------------------------
		0xc000c1e8L audio_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_byte1                    : 8 ;  //  7: 0     
	UINT32 reg_achst_byte2                    : 8 ;  // 15: 8     
	UINT32 reg_achst_byte3                    : 8 ;  // 23:16     
	UINT32 reg_achst_byte4                    : 8 ;  // 31:24     
} H14_AUDIO_08_T;

/*-----------------------------------------------------------------------------
		0xc000c1ecL audio_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_pc_0                     : 8 ;  //  7: 0     
	UINT32 reg_burst_pc_1                     : 8 ;  // 15: 8     
	UINT32 reg_burst_pd_0                     : 8 ;  // 23:16     
	UINT32 reg_burst_pd_1                     : 8 ;  // 31:24     
} H14_AUDIO_09_T;

/*-----------------------------------------------------------------------------
		0xc000c1f0L audio_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_i2s_ch_mute                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_i2s_len_ovrr                   : 4 ;  // 11: 8     
	UINT32 reg_i2s_len_ovrr_en                : 1 ;  //    12     
	UINT32                                    : 19;  // 31:13   reserved
} H14_AUDIO_10_T;

/*-----------------------------------------------------------------------------
		0xc000c1f4L heac_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_arc_src                        : 1 ;  //     0     
	UINT32                                    : 31;  // 31:1    reserved
} H14_HEAC_00_T;

/*-----------------------------------------------------------------------------
		0xc000c200L hdcp_rom_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_2             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_3             : 8 ;  // 31:24     
} H14_HDCP_ROM_00_T;

/*-----------------------------------------------------------------------------
		0xc000c204L hdcp_rom_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_4             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_5             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_6             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_7             : 8 ;  // 31:24     
} H14_HDCP_ROM_01_T;

/*-----------------------------------------------------------------------------
		0xc000c208L hdcp_rom_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_8             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_9             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_10            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_11            : 8 ;  // 31:24     
} H14_HDCP_ROM_02_T;

/*-----------------------------------------------------------------------------
		0xc000c20cL hdcp_rom_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_12            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_13            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_14            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_15            : 8 ;  // 31:24     
} H14_HDCP_ROM_03_T;

/*-----------------------------------------------------------------------------
		0xc000c210L hdcp_rom_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_16            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_17            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_18            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_19            : 8 ;  // 31:24     
} H14_HDCP_ROM_04_T;

/*-----------------------------------------------------------------------------
		0xc000c214L hdcp_rom_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_20            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_21            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_22            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_23            : 8 ;  // 31:24     
} H14_HDCP_ROM_05_T;

/*-----------------------------------------------------------------------------
		0xc000c218L hdcp_rom_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_24            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_25            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_26            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_27            : 8 ;  // 31:24     
} H14_HDCP_ROM_06_T;

/*-----------------------------------------------------------------------------
		0xc000c21cL hdcp_rom_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_28            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_29            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_30            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_31            : 8 ;  // 31:24     
} H14_HDCP_ROM_07_T;

/*-----------------------------------------------------------------------------
		0xc000c220L hdcp_rom_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_32            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_33            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_34            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_35            : 8 ;  // 31:24     
} H14_HDCP_ROM_08_T;

/*-----------------------------------------------------------------------------
		0xc000c224L hdcp_rom_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_36            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_37            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_38            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_39            : 8 ;  // 31:24     
} H14_HDCP_ROM_09_T;

/*-----------------------------------------------------------------------------
		0xc000c228L hdcp_rom_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_40            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_41            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_42            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_43            : 8 ;  // 31:24     
} H14_HDCP_ROM_10_T;

/*-----------------------------------------------------------------------------
		0xc000c22cL hdcp_rom_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_44            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_45            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_46            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_47            : 8 ;  // 31:24     
} H14_HDCP_ROM_11_T;

/*-----------------------------------------------------------------------------
		0xc000c230L hdcp_rom_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_48            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_49            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_50            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_51            : 8 ;  // 31:24     
} H14_HDCP_ROM_12_T;

/*-----------------------------------------------------------------------------
		0xc000c234L hdcp_rom_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_52            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_53            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_54            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_55            : 8 ;  // 31:24     
} H14_HDCP_ROM_13_T;

/*-----------------------------------------------------------------------------
		0xc000c238L hdcp_rom_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_56            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_57            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_58            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_59            : 8 ;  // 31:24     
} H14_HDCP_ROM_14_T;

/*-----------------------------------------------------------------------------
		0xc000c23cL hdcp_rom_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_60            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_61            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_62            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_63            : 8 ;  // 31:24     
} H14_HDCP_ROM_15_T;

/*-----------------------------------------------------------------------------
		0xc000c240L hdcp_rom_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_64            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_65            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_66            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_67            : 8 ;  // 31:24     
} H14_HDCP_ROM_16_T;

/*-----------------------------------------------------------------------------
		0xc000c244L hdcp_rom_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_68            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_69            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_70            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_71            : 8 ;  // 31:24     
} H14_HDCP_ROM_17_T;

/*-----------------------------------------------------------------------------
		0xc000c248L hdcp_rom_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_72            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_73            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_74            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_75            : 8 ;  // 31:24     
} H14_HDCP_ROM_18_T;

/*-----------------------------------------------------------------------------
		0xc000c24cL hdcp_rom_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_76            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_77            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_78            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_79            : 8 ;  // 31:24     
} H14_HDCP_ROM_19_T;

/*-----------------------------------------------------------------------------
		0xc000c250L hdcp_rom_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_80            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_81            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_82            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_83            : 8 ;  // 31:24     
} H14_HDCP_ROM_20_T;

/*-----------------------------------------------------------------------------
		0xc000c254L hdcp_rom_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_84            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_85            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_86            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_87            : 8 ;  // 31:24     
} H14_HDCP_ROM_21_T;

/*-----------------------------------------------------------------------------
		0xc000c258L hdcp_rom_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_88            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_89            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_90            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_91            : 8 ;  // 31:24     
} H14_HDCP_ROM_22_T;

/*-----------------------------------------------------------------------------
		0xc000c25cL hdcp_rom_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_92            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_93            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_94            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_95            : 8 ;  // 31:24     
} H14_HDCP_ROM_23_T;

/*-----------------------------------------------------------------------------
		0xc000c260L hdcp_rom_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_96            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_97            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_98            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_99            : 8 ;  // 31:24     
} H14_HDCP_ROM_24_T;

/*-----------------------------------------------------------------------------
		0xc000c264L hdcp_rom_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_100           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_101           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_102           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_103           : 8 ;  // 31:24     
} H14_HDCP_ROM_25_T;

/*-----------------------------------------------------------------------------
		0xc000c268L hdcp_rom_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_104           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_105           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_106           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_107           : 8 ;  // 31:24     
} H14_HDCP_ROM_26_T;

/*-----------------------------------------------------------------------------
		0xc000c26cL hdcp_rom_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_108           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_109           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_110           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_111           : 8 ;  // 31:24     
} H14_HDCP_ROM_27_T;

/*-----------------------------------------------------------------------------
		0xc000c270L hdcp_rom_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_112           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_113           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_114           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_115           : 8 ;  // 31:24     
} H14_HDCP_ROM_28_T;

/*-----------------------------------------------------------------------------
		0xc000c274L hdcp_rom_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_116           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_117           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_118           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_119           : 8 ;  // 31:24     
} H14_HDCP_ROM_29_T;

/*-----------------------------------------------------------------------------
		0xc000c278L hdcp_rom_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_120           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_121           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_122           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_123           : 8 ;  // 31:24     
} H14_HDCP_ROM_30_T;

/*-----------------------------------------------------------------------------
		0xc000c27cL hdcp_rom_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_124           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_125           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_126           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_127           : 8 ;  // 31:24     
} H14_HDCP_ROM_31_T;

/*-----------------------------------------------------------------------------
		0xc000c280L hdcp_rom_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_128           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_129           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_130           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_131           : 8 ;  // 31:24     
} H14_HDCP_ROM_32_T;

/*-----------------------------------------------------------------------------
		0xc000c284L hdcp_rom_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_132           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_133           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_134           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_135           : 8 ;  // 31:24     
} H14_HDCP_ROM_33_T;

/*-----------------------------------------------------------------------------
		0xc000c288L hdcp_rom_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_136           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_137           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_138           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_139           : 8 ;  // 31:24     
} H14_HDCP_ROM_34_T;

/*-----------------------------------------------------------------------------
		0xc000c28cL hdcp_rom_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_140           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_141           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_142           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_143           : 8 ;  // 31:24     
} H14_HDCP_ROM_35_T;

/*-----------------------------------------------------------------------------
		0xc000c290L hdcp_rom_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_144           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_145           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_146           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_147           : 8 ;  // 31:24     
} H14_HDCP_ROM_36_T;

/*-----------------------------------------------------------------------------
		0xc000c294L hdcp_rom_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_148           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_149           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_150           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_151           : 8 ;  // 31:24     
} H14_HDCP_ROM_37_T;

/*-----------------------------------------------------------------------------
		0xc000c298L hdcp_rom_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_152           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_153           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_154           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_155           : 8 ;  // 31:24     
} H14_HDCP_ROM_38_T;

/*-----------------------------------------------------------------------------
		0xc000c29cL hdcp_rom_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_156           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_157           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_158           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_159           : 8 ;  // 31:24     
} H14_HDCP_ROM_39_T;

/*-----------------------------------------------------------------------------
		0xc000c2a0L hdcp_rom_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_160           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_161           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_162           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_163           : 8 ;  // 31:24     
} H14_HDCP_ROM_40_T;

/*-----------------------------------------------------------------------------
		0xc000c2a4L hdcp_rom_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_164           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_165           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_166           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_167           : 8 ;  // 31:24     
} H14_HDCP_ROM_41_T;

/*-----------------------------------------------------------------------------
		0xc000c2a8L hdcp_rom_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_168           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_169           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_170           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_171           : 8 ;  // 31:24     
} H14_HDCP_ROM_42_T;

/*-----------------------------------------------------------------------------
		0xc000c2acL hdcp_rom_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_172           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_173           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_174           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_175           : 8 ;  // 31:24     
} H14_HDCP_ROM_43_T;

/*-----------------------------------------------------------------------------
		0xc000c2b0L hdcp_rom_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_176           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_177           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_178           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_179           : 8 ;  // 31:24     
} H14_HDCP_ROM_44_T;

/*-----------------------------------------------------------------------------
		0xc000c2b4L hdcp_rom_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_180           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_181           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_182           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_183           : 8 ;  // 31:24     
} H14_HDCP_ROM_45_T;

/*-----------------------------------------------------------------------------
		0xc000c2b8L hdcp_rom_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_184           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_185           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_186           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_187           : 8 ;  // 31:24     
} H14_HDCP_ROM_46_T;

/*-----------------------------------------------------------------------------
		0xc000c2bcL hdcp_rom_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_188           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_189           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_190           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_191           : 8 ;  // 31:24     
} H14_HDCP_ROM_47_T;

/*-----------------------------------------------------------------------------
		0xc000c2c0L hdcp_rom_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_192           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_193           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_194           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_195           : 8 ;  // 31:24     
} H14_HDCP_ROM_48_T;

/*-----------------------------------------------------------------------------
		0xc000c2c4L hdcp_rom_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_196           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_197           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_198           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_199           : 8 ;  // 31:24     
} H14_HDCP_ROM_49_T;

/*-----------------------------------------------------------------------------
		0xc000c2c8L hdcp_rom_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_200           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_201           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_202           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_203           : 8 ;  // 31:24     
} H14_HDCP_ROM_50_T;

/*-----------------------------------------------------------------------------
		0xc000c2ccL hdcp_rom_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_204           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_205           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_206           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_207           : 8 ;  // 31:24     
} H14_HDCP_ROM_51_T;

/*-----------------------------------------------------------------------------
		0xc000c2d0L hdcp_rom_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_208           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_209           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_210           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_211           : 8 ;  // 31:24     
} H14_HDCP_ROM_52_T;

/*-----------------------------------------------------------------------------
		0xc000c2d4L hdcp_rom_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_212           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_213           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_214           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_215           : 8 ;  // 31:24     
} H14_HDCP_ROM_53_T;

/*-----------------------------------------------------------------------------
		0xc000c2d8L hdcp_rom_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_216           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_217           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_218           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_219           : 8 ;  // 31:24     
} H14_HDCP_ROM_54_T;

/*-----------------------------------------------------------------------------
		0xc000c2dcL hdcp_rom_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_220           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_221           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_222           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_223           : 8 ;  // 31:24     
} H14_HDCP_ROM_55_T;

/*-----------------------------------------------------------------------------
		0xc000c2e0L hdcp_rom_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_224           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_225           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_226           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_227           : 8 ;  // 31:24     
} H14_HDCP_ROM_56_T;

/*-----------------------------------------------------------------------------
		0xc000c2e4L hdcp_rom_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_228           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_229           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_230           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_231           : 8 ;  // 31:24     
} H14_HDCP_ROM_57_T;

/*-----------------------------------------------------------------------------
		0xc000c2e8L hdcp_rom_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_232           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_233           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_234           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_235           : 8 ;  // 31:24     
} H14_HDCP_ROM_58_T;

/*-----------------------------------------------------------------------------
		0xc000c2ecL hdcp_rom_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_236           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_237           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_238           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_239           : 8 ;  // 31:24     
} H14_HDCP_ROM_59_T;

/*-----------------------------------------------------------------------------
		0xc000c2f0L hdcp_rom_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_240           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_241           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_242           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_243           : 8 ;  // 31:24     
} H14_HDCP_ROM_60_T;

/*-----------------------------------------------------------------------------
		0xc000c2f4L hdcp_rom_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_244           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_245           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_246           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_247           : 8 ;  // 31:24     
} H14_HDCP_ROM_61_T;

/*-----------------------------------------------------------------------------
		0xc000c2f8L hdcp_rom_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_248           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_249           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_250           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_251           : 8 ;  // 31:24     
} H14_HDCP_ROM_62_T;

/*-----------------------------------------------------------------------------
		0xc000c2fcL hdcp_rom_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_252           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_253           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_254           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_255           : 8 ;  // 31:24     
} H14_HDCP_ROM_63_T;

/*-----------------------------------------------------------------------------
		0xc000c300L hdcp_rom_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_256           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_257           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_258           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_259           : 8 ;  // 31:24     
} H14_HDCP_ROM_64_T;

/*-----------------------------------------------------------------------------
		0xc000c304L hdcp_rom_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_260           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_261           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_262           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_263           : 8 ;  // 31:24     
} H14_HDCP_ROM_65_T;

/*-----------------------------------------------------------------------------
		0xc000c308L hdcp_rom_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_264           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_265           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_266           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_267           : 8 ;  // 31:24     
} H14_HDCP_ROM_66_T;

/*-----------------------------------------------------------------------------
		0xc000c30cL hdcp_rom_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_268           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_269           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_270           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_271           : 8 ;  // 31:24     
} H14_HDCP_ROM_67_T;

/*-----------------------------------------------------------------------------
		0xc000c310L hdcp_rom_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_272           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_273           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_274           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_275           : 8 ;  // 31:24     
} H14_HDCP_ROM_68_T;

/*-----------------------------------------------------------------------------
		0xc000c314L hdcp_rom_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_276           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_277           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_278           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_279           : 8 ;  // 31:24     
} H14_HDCP_ROM_69_T;

/*-----------------------------------------------------------------------------
		0xc000c318L hdcp_rom_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_280           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_281           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_282           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_283           : 8 ;  // 31:24     
} H14_HDCP_ROM_70_T;

/*-----------------------------------------------------------------------------
		0xc000c31cL hdcp_rom_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_284           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_285           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_286           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_287           : 8 ;  // 31:24     
} H14_HDCP_ROM_71_T;


typedef struct {
	H14_PHY_00_T                       	phy_00;                       //0xc000c000L
	H14_PHY_01_T                       	phy_01;                       //0xc000c004L
	H14_INTERRUPT_ENABLE_00_T          	interrupt_enable_00;          //0xc000c008L
	H14_INTERRUPT_ENABLE_01_T          	interrupt_enable_01;          //0xc000c00cL
	H14_INTERRUPT_00_T                 	interrupt_00;                 //0xc000c010L
	H14_INTERRUPT_01_T                 	interrupt_01;                 //0xc000c014L
	H14_SYSTEM_CONTROL_00_T            	system_control_00;            //0xc000c018L
	H14_SYSTEM_CONTROL_01_T            	system_control_01;            //0xc000c01cL
	H14_SYSTEM_CONTROL_02_T            	system_control_02;            //0xc000c020L
	H14_HDCP_00_T                      	hdcp_00;                      //0xc000c024L
	H14_HDCP_01_T                      	hdcp_01;                      //0xc000c028L
	H14_HDCP_02_T                      	hdcp_02;                      //0xc000c02cL
	H14_HDCP_03_T                      	hdcp_03;                      //0xc000c030L
	H14_HDCP_04_T                      	hdcp_04;                      //0xc000c034L
	H14_HDCP_05_T                      	hdcp_05;                      //0xc000c038L
	H14_HDCP_06_T                      	hdcp_06;                      //0xc000c03cL
	H14_HDCP_07_T                      	hdcp_07;                      //0xc000c040L
	H14_HDCP_08_T                      	hdcp_08;                      //0xc000c044L
	H14_HDCP_09_T                      	hdcp_09;                      //0xc000c048L
	H14_PACKET_00_T                    	packet_00;                    //0xc000c04cL
	H14_PACKET_01_T                    	packet_01;                    //0xc000c050L
	H14_PACKET_02_T                    	packet_02;                    //0xc000c054L
	H14_PACKET_03_T                    	packet_03;                    //0xc000c058L
	H14_PACKET_04_T                    	packet_04;                    //0xc000c05cL
	H14_PACKET_05_T                    	packet_05;                    //0xc000c060L
	H14_PACKET_06_T                    	packet_06;                    //0xc000c064L
	H14_PACKET_07_T                    	packet_07;                    //0xc000c068L
	H14_PACKET_08_T                    	packet_08;                    //0xc000c06cL
	H14_PACKET_09_T                    	packet_09;                    //0xc000c070L
	H14_PACKET_10_T                    	packet_10;                    //0xc000c074L
	H14_PACKET_11_T                    	packet_11;                    //0xc000c078L
	H14_PACKET_12_T                    	packet_12;                    //0xc000c07cL
	H14_PACKET_13_T                    	packet_13;                    //0xc000c080L
	H14_PACKET_14_T                    	packet_14;                    //0xc000c084L
	H14_PACKET_15_T                    	packet_15;                    //0xc000c088L
	H14_PACKET_16_T                    	packet_16;                    //0xc000c08cL
	H14_PACKET_17_T                    	packet_17;                    //0xc000c090L
	H14_PACKET_18_T                    	packet_18;                    //0xc000c094L
	H14_PACKET_19_T                    	packet_19;                    //0xc000c098L
	H14_PACKET_20_T                    	packet_20;                    //0xc000c09cL
	H14_PACKET_21_T                    	packet_21;                    //0xc000c0a0L
	H14_PACKET_22_T                    	packet_22;                    //0xc000c0a4L
	H14_PACKET_23_T                    	packet_23;                    //0xc000c0a8L
	H14_PACKET_24_T                    	packet_24;                    //0xc000c0acL
	H14_PACKET_25_T                    	packet_25;                    //0xc000c0b0L
	H14_PACKET_26_T                    	packet_26;                    //0xc000c0b4L
	H14_PACKET_27_T                    	packet_27;                    //0xc000c0b8L
	H14_PACKET_28_T                    	packet_28;                    //0xc000c0bcL
	H14_PACKET_29_T                    	packet_29;                    //0xc000c0c0L
	H14_PACKET_30_T                    	packet_30;                    //0xc000c0c4L
	H14_PACKET_31_T                    	packet_31;                    //0xc000c0c8L
	H14_PACKET_32_T                    	packet_32;                    //0xc000c0ccL
	H14_PACKET_33_T                    	packet_33;                    //0xc000c0d0L
	H14_PACKET_34_T                    	packet_34;                    //0xc000c0d4L
	H14_PACKET_35_T                    	packet_35;                    //0xc000c0d8L
	H14_PACKET_36_T                    	packet_36;                    //0xc000c0dcL
	H14_PACKET_37_T                    	packet_37;                    //0xc000c0e0L
	H14_PACKET_38_T                    	packet_38;                    //0xc000c0e4L
	H14_PACKET_39_T                    	packet_39;                    //0xc000c0e8L
	H14_PACKET_40_T                    	packet_40;                    //0xc000c0ecL
	H14_PACKET_41_T                    	packet_41;                    //0xc000c0f0L
	H14_PACKET_42_T                    	packet_42;                    //0xc000c0f4L
	H14_PACKET_43_T                    	packet_43;                    //0xc000c0f8L
	H14_PACKET_44_T                    	packet_44;                    //0xc000c0fcL
	H14_PACKET_45_T                    	packet_45;                    //0xc000c100L
	H14_PACKET_46_T                    	packet_46;                    //0xc000c104L
	H14_PACKET_47_T                    	packet_47;                    //0xc000c108L
	H14_PACKET_48_T                    	packet_48;                    //0xc000c10cL
	H14_PACKET_49_T                    	packet_49;                    //0xc000c110L
	H14_PACKET_50_T                    	packet_50;                    //0xc000c114L
	H14_PACKET_51_T                    	packet_51;                    //0xc000c118L
	H14_PACKET_52_T                    	packet_52;                    //0xc000c11cL
	H14_PACKET_53_T                    	packet_53;                    //0xc000c120L
	H14_PACKET_54_T                    	packet_54;                    //0xc000c124L
	H14_PACKET_55_T                    	packet_55;                    //0xc000c128L
	H14_PACKET_56_T                    	packet_56;                    //0xc000c12cL
	H14_PACKET_57_T                    	packet_57;                    //0xc000c130L
	H14_PACKET_58_T                    	packet_58;                    //0xc000c134L
	H14_PACKET_59_T                    	packet_59;                    //0xc000c138L
	H14_PACKET_60_T                    	packet_60;                    //0xc000c13cL
	H14_PACKET_61_T                    	packet_61;                    //0xc000c140L
	H14_PACKET_62_T                    	packet_62;                    //0xc000c144L
	H14_PACKET_63_T                    	packet_63;                    //0xc000c148L
	H14_PACKET_64_T                    	packet_64;                    //0xc000c14cL
	H14_PACKET_65_T                    	packet_65;                    //0xc000c150L
	H14_PACKET_66_T                    	packet_66;                    //0xc000c154L
	H14_PACKET_67_T                    	packet_67;                    //0xc000c158L
	H14_PACKET_68_T                    	packet_68;                    //0xc000c15cL
	H14_PACKET_69_T                    	packet_69;                    //0xc000c160L
	H14_PACKET_70_T                    	packet_70;                    //0xc000c164L
	H14_PACKET_71_T                    	packet_71;                    //0xc000c168L
	H14_PACKET_72_T                    	packet_72;                    //0xc000c16cL
	H14_PACKET_73_T                    	packet_73;                    //0xc000c170L
	H14_PACKET_74_T                    	packet_74;                    //0xc000c174L
	H14_PACKET_75_T                    	packet_75;                    //0xc000c178L
	H14_PACKET_76_T                    	packet_76;                    //0xc000c17cL
	H14_PACKET_77_T                    	packet_77;                    //0xc000c180L
	H14_PACKET_78_T                    	packet_78;                    //0xc000c184L
	H14_PACKET_79_T                    	packet_79;                    //0xc000c188L
	H14_PACKET_80_T                    	packet_80;                    //0xc000c18cL
	H14_PACKET_81_T                    	packet_81;                    //0xc000c190L
	H14_PACKET_82_T                    	packet_82;                    //0xc000c194L
	H14_PACKET_83_T                    	packet_83;                    //0xc000c198L
	H14_VIDEO_00_T                     	video_00;                     //0xc000c19cL
	H14_VIDEO_01_T                     	video_01;                     //0xc000c1a0L
	H14_VIDEO_02_T                     	video_02;                     //0xc000c1a4L
	H14_VIDEO_03_T                     	video_03;                     //0xc000c1a8L
	H14_VIDEO_04_T                     	video_04;                     //0xc000c1acL
	H14_VIDEO_05_T                     	video_05;                     //0xc000c1b0L
	H14_VIDEO_06_T                     	video_06;                     //0xc000c1b4L
	H14_VIDEO_07_T                     	video_07;                     //0xc000c1b8L
	H14_VIDEO_08_T                     	video_08;                     //0xc000c1bcL
	H14_VIDEO_09_T                     	video_09;                     //0xc000c1c0L
	H14_VIDEO_10_T                     	video_10;                     //0xc000c1c4L
	H14_AUDIO_00_T                     	audio_00;                     //0xc000c1c8L
	H14_AUDIO_01_T                     	audio_01;                     //0xc000c1ccL
	H14_AUDIO_02_T                     	audio_02;                     //0xc000c1d0L
	H14_AUDIO_03_T                     	audio_03;                     //0xc000c1d4L
	H14_AUDIO_04_T                     	audio_04;                     //0xc000c1d8L
	H14_AUDIO_05_T                     	audio_05;                     //0xc000c1dcL
	H14_AUDIO_06_T                     	audio_06;                     //0xc000c1e0L
	H14_AUDIO_07_T                     	audio_07;                     //0xc000c1e4L
	H14_AUDIO_08_T                     	audio_08;                     //0xc000c1e8L
	H14_AUDIO_09_T                     	audio_09;                     //0xc000c1ecL
	H14_AUDIO_10_T                     	audio_10;                     //0xc000c1f0L
	H14_HEAC_00_T                      	heac_00;                      //0xc000c1f4L
	UINT32                             	reserved00[2];                //0xc000c1f8L~0xc000c1fcL
	H14_HDCP_ROM_00_T                  	hdcp_rom_00;                  //0xc000c200L
	H14_HDCP_ROM_01_T                  	hdcp_rom_01;                  //0xc000c204L
	H14_HDCP_ROM_02_T                  	hdcp_rom_02;                  //0xc000c208L
	H14_HDCP_ROM_03_T                  	hdcp_rom_03;                  //0xc000c20cL
	H14_HDCP_ROM_04_T                  	hdcp_rom_04;                  //0xc000c210L
	H14_HDCP_ROM_05_T                  	hdcp_rom_05;                  //0xc000c214L
	H14_HDCP_ROM_06_T                  	hdcp_rom_06;                  //0xc000c218L
	H14_HDCP_ROM_07_T                  	hdcp_rom_07;                  //0xc000c21cL
	H14_HDCP_ROM_08_T                  	hdcp_rom_08;                  //0xc000c220L
	H14_HDCP_ROM_09_T                  	hdcp_rom_09;                  //0xc000c224L
	H14_HDCP_ROM_10_T                  	hdcp_rom_10;                  //0xc000c228L
	H14_HDCP_ROM_11_T                  	hdcp_rom_11;                  //0xc000c22cL
	H14_HDCP_ROM_12_T                  	hdcp_rom_12;                  //0xc000c230L
	H14_HDCP_ROM_13_T                  	hdcp_rom_13;                  //0xc000c234L
	H14_HDCP_ROM_14_T                  	hdcp_rom_14;                  //0xc000c238L
	H14_HDCP_ROM_15_T                  	hdcp_rom_15;                  //0xc000c23cL
	H14_HDCP_ROM_16_T                  	hdcp_rom_16;                  //0xc000c240L
	H14_HDCP_ROM_17_T                  	hdcp_rom_17;                  //0xc000c244L
	H14_HDCP_ROM_18_T                  	hdcp_rom_18;                  //0xc000c248L
	H14_HDCP_ROM_19_T                  	hdcp_rom_19;                  //0xc000c24cL
	H14_HDCP_ROM_20_T                  	hdcp_rom_20;                  //0xc000c250L
	H14_HDCP_ROM_21_T                  	hdcp_rom_21;                  //0xc000c254L
	H14_HDCP_ROM_22_T                  	hdcp_rom_22;                  //0xc000c258L
	H14_HDCP_ROM_23_T                  	hdcp_rom_23;                  //0xc000c25cL
	H14_HDCP_ROM_24_T                  	hdcp_rom_24;                  //0xc000c260L
	H14_HDCP_ROM_25_T                  	hdcp_rom_25;                  //0xc000c264L
	H14_HDCP_ROM_26_T                  	hdcp_rom_26;                  //0xc000c268L
	H14_HDCP_ROM_27_T                  	hdcp_rom_27;                  //0xc000c26cL
	H14_HDCP_ROM_28_T                  	hdcp_rom_28;                  //0xc000c270L
	H14_HDCP_ROM_29_T                  	hdcp_rom_29;                  //0xc000c274L
	H14_HDCP_ROM_30_T                  	hdcp_rom_30;                  //0xc000c278L
	H14_HDCP_ROM_31_T                  	hdcp_rom_31;                  //0xc000c27cL
	H14_HDCP_ROM_32_T                  	hdcp_rom_32;                  //0xc000c280L
	H14_HDCP_ROM_33_T                  	hdcp_rom_33;                  //0xc000c284L
	H14_HDCP_ROM_34_T                  	hdcp_rom_34;                  //0xc000c288L
	H14_HDCP_ROM_35_T                  	hdcp_rom_35;                  //0xc000c28cL
	H14_HDCP_ROM_36_T                  	hdcp_rom_36;                  //0xc000c290L
	H14_HDCP_ROM_37_T                  	hdcp_rom_37;                  //0xc000c294L
	H14_HDCP_ROM_38_T                  	hdcp_rom_38;                  //0xc000c298L
	H14_HDCP_ROM_39_T                  	hdcp_rom_39;                  //0xc000c29cL
	H14_HDCP_ROM_40_T                  	hdcp_rom_40;                  //0xc000c2a0L
	H14_HDCP_ROM_41_T                  	hdcp_rom_41;                  //0xc000c2a4L
	H14_HDCP_ROM_42_T                  	hdcp_rom_42;                  //0xc000c2a8L
	H14_HDCP_ROM_43_T                  	hdcp_rom_43;                  //0xc000c2acL
	H14_HDCP_ROM_44_T                  	hdcp_rom_44;                  //0xc000c2b0L
	H14_HDCP_ROM_45_T                  	hdcp_rom_45;                  //0xc000c2b4L
	H14_HDCP_ROM_46_T                  	hdcp_rom_46;                  //0xc000c2b8L
	H14_HDCP_ROM_47_T                  	hdcp_rom_47;                  //0xc000c2bcL
	H14_HDCP_ROM_48_T                  	hdcp_rom_48;                  //0xc000c2c0L
	H14_HDCP_ROM_49_T                  	hdcp_rom_49;                  //0xc000c2c4L
	H14_HDCP_ROM_50_T                  	hdcp_rom_50;                  //0xc000c2c8L
	H14_HDCP_ROM_51_T                  	hdcp_rom_51;                  //0xc000c2ccL
	H14_HDCP_ROM_52_T                  	hdcp_rom_52;                  //0xc000c2d0L
	H14_HDCP_ROM_53_T                  	hdcp_rom_53;                  //0xc000c2d4L
	H14_HDCP_ROM_54_T                  	hdcp_rom_54;                  //0xc000c2d8L
	H14_HDCP_ROM_55_T                  	hdcp_rom_55;                  //0xc000c2dcL
	H14_HDCP_ROM_56_T                  	hdcp_rom_56;                  //0xc000c2e0L
	H14_HDCP_ROM_57_T                  	hdcp_rom_57;                  //0xc000c2e4L
	H14_HDCP_ROM_58_T                  	hdcp_rom_58;                  //0xc000c2e8L
	H14_HDCP_ROM_59_T                  	hdcp_rom_59;                  //0xc000c2ecL
	H14_HDCP_ROM_60_T                  	hdcp_rom_60;                  //0xc000c2f0L
	H14_HDCP_ROM_61_T                  	hdcp_rom_61;                  //0xc000c2f4L
	H14_HDCP_ROM_62_T                  	hdcp_rom_62;                  //0xc000c2f8L
	H14_HDCP_ROM_63_T                  	hdcp_rom_63;                  //0xc000c2fcL
	H14_HDCP_ROM_64_T                  	hdcp_rom_64;                  //0xc000c300L
	H14_HDCP_ROM_65_T                  	hdcp_rom_65;                  //0xc000c304L
	H14_HDCP_ROM_66_T                  	hdcp_rom_66;                  //0xc000c308L
	H14_HDCP_ROM_67_T                  	hdcp_rom_67;                  //0xc000c30cL
	H14_HDCP_ROM_68_T                  	hdcp_rom_68;                  //0xc000c310L
	H14_HDCP_ROM_69_T                  	hdcp_rom_69;                  //0xc000c314L
	H14_HDCP_ROM_70_T                  	hdcp_rom_70;                  //0xc000c318L
	H14_HDCP_ROM_71_T                  	hdcp_rom_71;                  //0xc000c31cL
}HDMI_LINK_REG_H14_T;
//#endif
