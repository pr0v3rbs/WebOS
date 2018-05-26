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


//#ifndef _REG_HDMI_LINK_M14B0_h
//#define _REG_HDMI_LINK_M14B0_h
/*-----------------------------------------------------------------------------
		0x0 phy_link_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_rstn_prt0                     : 1 ;  //     0     
	UINT32 phy_pdb_prt0                      : 1 ;  //     1     
	UINT32 phy_enable_prt0                   : 1 ;  //     2     
	UINT32 hpd_out_prt0                      : 1 ;  //     3     
	UINT32 phy_pp_mode_prt0                  : 1 ;  //     4     
	UINT32 phy_mhl_mode_prt0                 : 1 ;  //     5     
	UINT32 phy_eq_odt_sel_prt0               : 2 ;  //  7: 6     
	UINT32 phy_arc_pdb_prt0                  : 1 ;  //     8     
	UINT32 phy_arc_odt_sel_prt0              : 2 ;  // 10: 9     
	UINT32 hpd_in_prt0                       : 1 ;  //    11     
	UINT32 hpd_out_oen_prt0                  : 1 ;  //    12     
	UINT32 hpd_1k_en_prt0                    : 1 ;  //    13     
	UINT32 hpd_100k_en_prt0                  : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 link_sw_rstn_tmds_prt0            : 1 ;  //    16     
	UINT32 link_sw_rstn_hdcp_prt0            : 1 ;  //    17     
	UINT32 link_sw_rstn_edid_prt0            : 1 ;  //    18     
} M14B0_PHY_LINK_00_T;

/*-----------------------------------------------------------------------------
		0x4 phy_link_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0_prt0           : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1_prt0           : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res_prt0            : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done_prt0           : 1 ;  //    18     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 phy_bert_run_ch2_prt0             : 1 ;  //    24     
	UINT32 phy_bert_run_ch1_prt0             : 1 ;  //    25     
	UINT32 phy_bert_run_ch0_prt0             : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 phy_bert_ch_sel_prt0              : 2 ;  // 29:28     
} M14B0_PHY_LINK_01_T;

/*-----------------------------------------------------------------------------
		0x8 phy_link_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_rstn_prt1                     : 1 ;  //     0     
	UINT32 phy_pdb_prt1                      : 1 ;  //     1     
	UINT32 phy_enable_prt1                   : 1 ;  //     2     
	UINT32 hpd_out_prt1                      : 1 ;  //     3     
	UINT32 phy_pp_mode_prt1                  : 1 ;  //     4     
	UINT32 phy_mhl_mode_prt1                 : 1 ;  //     5     
	UINT32 phy_eq_odt_sel_prt1               : 2 ;  //  7: 6     
	UINT32 hpd_in_prt1                       : 1 ;  //     8     
	UINT32 hpd_out_oen_prt1                  : 1 ;  //     9     
	UINT32 hpd_1k_en_prt1                    : 1 ;  //    10     
	UINT32 hpd_100k_en_prt1                  : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 link_sw_rstn_tmds_prt1            : 1 ;  //    16     
	UINT32 link_sw_rstn_hdcp_prt1            : 1 ;  //    17     
	UINT32 link_sw_rstn_edid_prt1            : 1 ;  //    18     
} M14B0_PHY_LINK_02_T;

/*-----------------------------------------------------------------------------
		0xc phy_link_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0_prt1           : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1_prt1           : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res_prt1            : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done_prt1           : 1 ;  //    18     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 phy_bert_run_ch2_prt1             : 1 ;  //    24     
	UINT32 phy_bert_run_ch1_prt1             : 1 ;  //    25     
	UINT32 phy_bert_run_ch0_prt1             : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 phy_bert_ch_sel_prt1              : 2 ;  // 29:28     
} M14B0_PHY_LINK_03_T;

/*-----------------------------------------------------------------------------
		0x10 phy_link_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_rstn_prt2                     : 1 ;  //     0     
	UINT32 phy_pdb_prt2                      : 1 ;  //     1     
	UINT32 phy_enable_prt2                   : 1 ;  //     2     
	UINT32 hpd_out_prt2                      : 1 ;  //     3     
	UINT32 phy_pp_mode_prt2                  : 1 ;  //     4     
	UINT32 phy_mhl_mode_prt2                 : 1 ;  //     5     
	UINT32 phy_eq_odt_sel_prt2               : 2 ;  //  7: 6     
	UINT32 hpd_in_prt2                       : 1 ;  //     8     
	UINT32 hpd_out_oen_prt2                  : 1 ;  //     9     
	UINT32 hpd_1k_en_prt2                    : 1 ;  //    10     
	UINT32 hpd_100k_en_prt2                  : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 link_sw_rstn_tmds_prt2            : 1 ;  //    16     
	UINT32 link_sw_rstn_hdcp_prt2            : 1 ;  //    17     
	UINT32 link_sw_rstn_edid_prt2            : 1 ;  //    18     
} M14B0_PHY_LINK_04_T;

/*-----------------------------------------------------------------------------
		0x14 phy_link_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0_prt2           : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1_prt2           : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res_prt2            : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done_prt2           : 1 ;  //    18     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 phy_bert_run_ch2_prt2             : 1 ;  //    24     
	UINT32 phy_bert_run_ch1_prt2             : 1 ;  //    25     
	UINT32 phy_bert_run_ch0_prt2             : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 phy_bert_ch_sel_prt2              : 2 ;  // 29:28     
} M14B0_PHY_LINK_05_T;

/*-----------------------------------------------------------------------------
		0x18 phy_link_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_rstn_prt3                     : 1 ;  //     0     
	UINT32 phy_pdb_prt3                      : 1 ;  //     1     
	UINT32 phy_enable_prt3                   : 1 ;  //     2     
	UINT32 hpd_out_prt3                      : 1 ;  //     3     
	UINT32 hpd_out_en_prt3                   : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 phy_eq_odt_sel_prt3               : 2 ;  //  7: 6     
	UINT32 reg_man_pp_value                   : 1 ;  //     8     
	UINT32 reg_man_pp_mode                    : 1 ;  //     9     
	UINT32 reg_man_mhl_value                  : 1 ;  //    10     
	UINT32 reg_man_mhl_mode                   : 1 ;  //    11     
	UINT32 hpd_in_prt3                       : 1 ;  //    12     
	UINT32 hpd_1k_en_prt3                    : 1 ;  //    13     
	UINT32 hpd_100k_en_prt3                  : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 link_sw_rstn_tmds_prt3            : 1 ;  //    16     
	UINT32 link_sw_rstn_hdcp_prt3            : 1 ;  //    17     
	UINT32 link_sw_rstn_edid_prt3            : 1 ;  //    18     
	UINT32 link_sw_rstn_cbus_prt3            : 1 ;  //    19     
} M14B0_PHY_LINK_06_T;

/*-----------------------------------------------------------------------------
		0x1c phy_link_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0_prt3           : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1_prt3           : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res_prt3            : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done_prt3           : 1 ;  //    18     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 phy_bert_run_ch2_prt3             : 1 ;  //    24     
	UINT32 phy_bert_run_ch1_prt3             : 1 ;  //    25     
	UINT32 phy_bert_run_ch0_prt3             : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 phy_bert_ch_sel_prt3              : 2 ;  // 29:28     
} M14B0_PHY_LINK_07_T;

/*-----------------------------------------------------------------------------
		0x20 interrupt_enable_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_new_avi_enable                : 1 ;  //     0     
	UINT32 intr_new_vsi_enable                : 1 ;  //     1     
	UINT32 intr_chg_vsi_vformat_enable        : 1 ;  //     2     
	UINT32 intr_new_gcp_enable                : 1 ;  //     3     
	UINT32 intr_new_asp_enable                : 1 ;  //     4     
	UINT32 intr_new_acr_enable                : 1 ;  //     5     
	UINT32 intr_new_gbd_enable                : 1 ;  //     6     
	UINT32 intr_new_acp_enable                : 1 ;  //     7     
	UINT32 intr_new_isrc1_enable              : 1 ;  //     8     
	UINT32 intr_new_isrc2_enable              : 1 ;  //     9     
	UINT32 intr_new_mpg_enable                : 1 ;  //    10     
	UINT32 intr_new_aud_enable                : 1 ;  //    11     
	UINT32 intr_new_spd_enable                : 1 ;  //    12     
	UINT32 intr_new_unr_enable                : 1 ;  //    13     
	UINT32 intr_cts_chg_enable                : 1 ;  //    14     
	UINT32 intr_n_chg_enable                  : 1 ;  //    15     
	UINT32 intr_fs_chg_enable                 : 1 ;  //    16     
	UINT32 intr_2pcm_chg_enable               : 1 ;  //    17     
	UINT32 intr_2npcm_chg_enable              : 1 ;  //    18     
	UINT32 intr_aud_type_chg_enable           : 1 ;  //    19     
	UINT32 intr_spdif_err_enable              : 1 ;  //    20     
	UINT32 intr_afifo_undr_enable             : 1 ;  //    21     
	UINT32 intr_afifo_ovrr_enable             : 1 ;  //    22     
	UINT32 intr_burst_info_enable             : 1 ;  //    23     
	UINT32 intr_vf_stable_enable              : 1 ;  //    24     
	UINT32 intr_vid_chg_enable                : 1 ;  //    25     
	UINT32 intr_vr_chg_enable                 : 1 ;  //    26     
	UINT32 intr_hr_chg_enable                 : 1 ;  //    27     
	UINT32 intr_po_chg_enable                 : 1 ;  //    28     
	UINT32 intr_il_chg_enable                 : 1 ;  //    29     
	UINT32 intr_vfifo_undr_enable             : 1 ;  //    30     
	UINT32 intr_vfifo_ovrr_enable             : 1 ;  //    31     
} M14B0_INTERRUPT_ENABLE_00_T;

/*-----------------------------------------------------------------------------
		0x24 interrupt_enable_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_hdmi5v_redge_prt3_enable     : 1 ;  //     0     
	UINT32 intr_hdmi5v_redge_prt2_enable     : 1 ;  //     1     
	UINT32 intr_hdmi5v_redge_prt1_enable     : 1 ;  //     2     
	UINT32 intr_hdmi5v_redge_prt0_enable     : 1 ;  //     3     
	UINT32 intr_scdt_fedge_prt3_enable       : 1 ;  //     4     
	UINT32 intr_scdt_fedge_prt2_enable       : 1 ;  //     5     
	UINT32 intr_scdt_fedge_prt1_enable       : 1 ;  //     6     
	UINT32 intr_scdt_fedge_prt0_enable       : 1 ;  //     7     
	UINT32 intr_scdt_redge_prt3_enable       : 1 ;  //     8     
	UINT32 intr_scdt_redge_prt2_enable       : 1 ;  //     9     
	UINT32 intr_scdt_redge_prt1_enable       : 1 ;  //    10     
	UINT32 intr_scdt_redge_prt0_enable       : 1 ;  //    11     
	UINT32 intr_cd_sense_fedge_prt3_enable   : 1 ;  //    12     
	UINT32 intr_cd_sense_redge_prt3_enable   : 1 ;  //    13     
	UINT32 intr_auth_init_prt0_enable        : 1 ;  //    14     
	UINT32 intr_auth_done_prt0_enable        : 1 ;  //    15     
	UINT32 intr_hdcp_err_enable               : 1 ;  //    16     
	UINT32 intr_ecc_err_enable                : 1 ;  //    17     
	UINT32 intr_terc4_err_prt3_enable        : 1 ;  //    18     
	UINT32 intr_terc4_err_prt2_enable        : 1 ;  //    19     
	UINT32 intr_terc4_err_prt1_enable        : 1 ;  //    20     
	UINT32 intr_terc4_err_prt0_enable        : 1 ;  //    21     
	UINT32 intr_acr_err_enable                : 1 ;  //    22     
	UINT32 intr_asp_err_enable                : 1 ;  //    23     
	UINT32 intr_no_avi_enable                 : 1 ;  //    24     
	UINT32 intr_no_vsi_enable                 : 1 ;  //    25     
	UINT32 intr_no_gcp_enable                 : 1 ;  //    26     
	UINT32 intr_no_spd_enable                 : 1 ;  //    27     
	UINT32 intr_chg_avi_b12345_enable         : 1 ;  //    28     
	UINT32 intr_chg_avi_to_rgb_enable         : 1 ;  //    29     
	UINT32 intr_chg_avi_to_ycc444_enable      : 1 ;  //    30     
	UINT32 intr_chg_avi_to_ycc422_enable      : 1 ;  //    31     
} M14B0_INTERRUPT_ENABLE_01_T;

/*-----------------------------------------------------------------------------
		0x28 interrupt_enable_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_plug_in_prt1_enable          : 1 ;  //     0     
	UINT32 intr_plug_in_prt0_enable          : 1 ;  //     1     
	UINT32 int_hdmi5v_fall_prt3_enable       : 1 ;  //     2     
	UINT32 int_hdmi5v_fall_prt2_enable       : 1 ;  //     3     
	UINT32 int_hdmi5v_fall_prt1_enable       : 1 ;  //     4     
	UINT32 int_hdmi5v_fall_prt0_enable       : 1 ;  //     5     
	UINT32 int_hdmi5v_rise_prt3_enable       : 1 ;  //     6     
	UINT32 int_hdmi5v_rise_prt2_enable       : 1 ;  //     7     
	UINT32 int_hdmi5v_rise_prt1_enable       : 1 ;  //     8     
	UINT32 int_hdmi5v_rise_prt0_enable       : 1 ;  //     9     
	UINT32 intr_scdt_fall_prt3_enable        : 1 ;  //    10     
	UINT32 intr_scdt_fall_prt2_enable        : 1 ;  //    11     
	UINT32 intr_scdt_fall_prt1_enable        : 1 ;  //    12     
	UINT32 intr_scdt_fall_prt0_enable        : 1 ;  //    13     
	UINT32 intr_scdt_rise_prt3_enable        : 1 ;  //    14     
	UINT32 intr_scdt_rise_prt2_enable        : 1 ;  //    15     
	UINT32 intr_scdt_rise_prt1_enable        : 1 ;  //    16     
	UINT32 intr_scdt_rise_prt0_enable        : 1 ;  //    17     
	UINT32 intr_hdmi_mode_prt3_enable        : 1 ;  //    18     
	UINT32 intr_hdmi_mode_prt2_enable        : 1 ;  //    19     
	UINT32 intr_hdmi_mode_prt1_enable        : 1 ;  //    20     
	UINT32 intr_hdmi_mode_prt0_enable        : 1 ;  //    21     
	UINT32 intr_set_mute_enable               : 1 ;  //    22     
	UINT32 intr_clr_mute_enable               : 1 ;  //    23     
	UINT32 intr_vsync_prt3_enable            : 1 ;  //    24     
	UINT32 intr_vsync_prt2_enable            : 1 ;  //    25     
	UINT32 intr_vsync_prt1_enable            : 1 ;  //    26     
	UINT32 intr_vsync_prt0_enable            : 1 ;  //    27     
	UINT32 intr_hdmi5v_fedge_prt3_enable     : 1 ;  //    28     
	UINT32 intr_hdmi5v_fedge_prt2_enable     : 1 ;  //    29     
	UINT32 intr_hdmi5v_fedge_prt1_enable     : 1 ;  //    30     
	UINT32 intr_hdmi5v_fedge_prt0_enable     : 1 ;  //    31     
} M14B0_INTERRUPT_ENABLE_02_T;

/*-----------------------------------------------------------------------------
		0x2c interrupt_enable_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_pathen_clr_enable        : 1 ;  //     0     
	UINT32 intr_cbus_mute_set_enable          : 1 ;  //     1     
	UINT32 intr_cbus_mute_clr_enable          : 1 ;  //     2     
	UINT32 intr_cbus_new_mscerr_enable        : 1 ;  //     3     
	UINT32 intr_cbus_new_rcp_enable           : 1 ;  //     4     
	UINT32 intr_cbus_cmd_set_hpd_enable       : 1 ;  //     5     
	UINT32 intr_cbus_cmd_get_state_enable     : 1 ;  //     6     
	UINT32 intr_cbus_cmd_get_vdid_enable      : 1 ;  //     7     
	UINT32 intr_cbus_cmd_rd_devcap_enable     : 1 ;  //     8     
	UINT32 intr_cbus_cmd_rd_devintr_enable    : 1 ;  //     9     
	UINT32 intr_cbus_cmd_pathen_set_enable    : 1 ;  //    10     
	UINT32 intr_cbus_cmd_pathen_clr_enable    : 1 ;  //    11     
	UINT32 intr_cbus_cmd_dcaprd_set_enable    : 1 ;  //    12     
	UINT32 intr_cbus_cmd_3dreq_set_enable     : 1 ;  //    13     
	UINT32 intr_cbus_cmd_grtwrt_set_enable    : 1 ;  //    14     
	UINT32 intr_cbus_cmd_reqwrt_set_enable    : 1 ;  //    15     
	UINT32 intr_cbus_cmd_wrt_burst_enable     : 1 ;  //    16     
	UINT32 intr_cbus_cmd_dscrchg_set_enable   : 1 ;  //    17     
	UINT32 intr_cbus_cmd_dcapchg_set_enable   : 1 ;  //    18     
	UINT32 intr_cbus_cmd_edidchg_set_enable   : 1 ;  //    19     
	UINT32 intr_cbus_cmd_clr_hpd_enable       : 1 ;  //    20     
	UINT32 intr_cbus_cmd_rap_poll_enable      : 1 ;  //    21     
	UINT32 intr_cbus_cmd_rap_on_enable        : 1 ;  //    22     
	UINT32 intr_cbus_cmd_rap_off_enable       : 1 ;  //    23     
	UINT32 intr_cbus_cmd_rcp_enable           : 1 ;  //    24     
	UINT32 intr_cbus_cmd_msc_err_enable       : 1 ;  //    25     
	UINT32 intr_plug_out_prt3_enable         : 1 ;  //    26     
	UINT32 intr_plug_out_prt2_enable         : 1 ;  //    27     
	UINT32 intr_plug_out_prt1_enable         : 1 ;  //    28     
	UINT32 intr_plug_out_prt0_enable         : 1 ;  //    29     
	UINT32 intr_plug_in_prt3_enable          : 1 ;  //    30     
	UINT32 intr_plug_in_prt2_enable          : 1 ;  //    31     
} M14B0_INTERRUPT_ENABLE_03_T;

/*-----------------------------------------------------------------------------
		0x30 interrupt_enable_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_ready_enable             : 1 ;  //     0     
	UINT32 intr_cbus_disconn_enable           : 1 ;  //     1     
	UINT32 intr_cbus_max_nack_enable          : 1 ;  //     2     
	UINT32 intr_cbus_max_retx_enable          : 1 ;  //     3     
	UINT32 intr_cbus_max_rerx_enable          : 1 ;  //     4     
	UINT32 intr_cbus_new_devstate_enable      : 1 ;  //     5     
	UINT32 intr_cbus_new_vdid_enable          : 1 ;  //     6     
	UINT32 intr_cbus_new_devcap_enable        : 1 ;  //     7     
	UINT32 intr_cbus_chg_dscr_src_enable      : 1 ;  //     8     
	UINT32 intr_cbus_chg_dscr_sink_enable     : 1 ;  //     9     
	UINT32 intr_cbus_chg_ppmode_enable        : 1 ;  //    10     
	UINT32 intr_cbus_chg_24mode_enable        : 1 ;  //    11     
	UINT32 intr_cbus_pathen_set_enable        : 1 ;  //    12     
	UINT32 intr_cbus_new_dscr_enable          : 1 ;  //    13     
	UINT32 intr_cbus_new_intstat_enable       : 1 ;  //    14     
	UINT32 intr_cbus_new_dcap_enable          : 1 ;  //    15     
	UINT32 intr_cbus_new_ucp_enable           : 1 ;  //    16     
	UINT32 intr_cbus_cmd_ucp_enable           : 1 ;  //    17     
	UINT32 intr_auth_init_prt3_enable        : 1 ;  //    18     
	UINT32 intr_auth_init_prt2_enable        : 1 ;  //    19     
	UINT32 intr_auth_init_prt1_enable        : 1 ;  //    20     
	UINT32 intr_auth_done_prt3_enable        : 1 ;  //    21     
	UINT32 intr_auth_done_prt2_enable        : 1 ;  //    22     
	UINT32 intr_auth_done_prt1_enable        : 1 ;  //    23     
} M14B0_INTERRUPT_ENABLE_04_T;

/*-----------------------------------------------------------------------------
		0x34 interrupt_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_new_avi                       : 1 ;  //     0     
	UINT32 intr_new_vsi                       : 1 ;  //     1     
	UINT32 intr_chg_vsi_vformat               : 1 ;  //     2     
	UINT32 intr_new_gcp                       : 1 ;  //     3     
	UINT32 intr_new_asp                       : 1 ;  //     4     
	UINT32 intr_new_acr                       : 1 ;  //     5     
	UINT32 intr_new_gbd                       : 1 ;  //     6     
	UINT32 intr_new_acp                       : 1 ;  //     7     
	UINT32 intr_new_isrc1                     : 1 ;  //     8     
	UINT32 intr_new_isrc2                     : 1 ;  //     9     
	UINT32 intr_new_mpg                       : 1 ;  //    10     
	UINT32 intr_new_aud                       : 1 ;  //    11     
	UINT32 intr_new_spd                       : 1 ;  //    12     
	UINT32 intr_new_unr                       : 1 ;  //    13     
	UINT32 intr_cts_chg                       : 1 ;  //    14     
	UINT32 intr_n_chg                         : 1 ;  //    15     
	UINT32 intr_fs_chg                        : 1 ;  //    16     
	UINT32 intr_2pcm_chg                      : 1 ;  //    17     
	UINT32 intr_2npcm_chg                     : 1 ;  //    18     
	UINT32 intr_aud_type_chg                  : 1 ;  //    19     
	UINT32 intr_spdif_err                     : 1 ;  //    20     
	UINT32 intr_afifo_undr                    : 1 ;  //    21     
	UINT32 intr_afifo_ovrr                    : 1 ;  //    22     
	UINT32 intr_burst_info                    : 1 ;  //    23     
	UINT32 intr_vf_stable                     : 1 ;  //    24     
	UINT32 intr_vid_chg                       : 1 ;  //    25     
	UINT32 intr_vr_chg                        : 1 ;  //    26     
	UINT32 intr_hr_chg                        : 1 ;  //    27     
	UINT32 intr_po_chg                        : 1 ;  //    28     
	UINT32 intr_il_chg                        : 1 ;  //    29     
	UINT32 intr_vfifo_undr                    : 1 ;  //    30     
	UINT32 intr_vfifo_ovrr                    : 1 ;  //    31     
} M14B0_INTERRUPT_00_T;

/*-----------------------------------------------------------------------------
		0x38 interrupt_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_hdmi5v_redge_prt3            : 1 ;  //     0     
	UINT32 intr_hdmi5v_redge_prt2            : 1 ;  //     1     
	UINT32 intr_hdmi5v_redge_prt1            : 1 ;  //     2     
	UINT32 intr_hdmi5v_redge_prt0            : 1 ;  //     3     
	UINT32 intr_scdt_fedge_prt3              : 1 ;  //     4     
	UINT32 intr_scdt_fedge_prt2              : 1 ;  //     5     
	UINT32 intr_scdt_fedge_prt1              : 1 ;  //     6     
	UINT32 intr_scdt_fedge_prt0              : 1 ;  //     7     
	UINT32 intr_scdt_redge_prt3              : 1 ;  //     8     
	UINT32 intr_scdt_redge_prt2              : 1 ;  //     9     
	UINT32 intr_scdt_redge_prt1              : 1 ;  //    10     
	UINT32 intr_scdt_redge_prt0              : 1 ;  //    11     
	UINT32 intr_cd_sense_fedge_prt3          : 1 ;  //    12     
	UINT32 intr_cd_sense_redge_prt3          : 1 ;  //    13     
	UINT32 intr_auth_init_prt0               : 1 ;  //    14     
	UINT32 intr_auth_done_prt0               : 1 ;  //    15     
	UINT32 intr_hdcp_err                      : 1 ;  //    16     
	UINT32 intr_ecc_err                       : 1 ;  //    17     
	UINT32 intr_terc4_err_prt3               : 1 ;  //    18     
	UINT32 intr_terc4_err_prt2               : 1 ;  //    19     
	UINT32 intr_terc4_err_prt1               : 1 ;  //    20     
	UINT32 intr_terc4_err_prt0               : 1 ;  //    21     
	UINT32 intr_acr_err                       : 1 ;  //    22     
	UINT32 intr_asp_err                       : 1 ;  //    23     
	UINT32 intr_no_avi                        : 1 ;  //    24     
	UINT32 intr_no_vsi                        : 1 ;  //    25     
	UINT32 intr_no_gcp                        : 1 ;  //    26     
	UINT32 intr_no_spd                        : 1 ;  //    27     
	UINT32 intr_chg_avi_b12345                : 1 ;  //    28     
	UINT32 intr_chg_avi_to_rgb                : 1 ;  //    29     
	UINT32 intr_chg_avi_to_ycc444             : 1 ;  //    30     
	UINT32 intr_chg_avi_to_ycc422             : 1 ;  //    31     
} M14B0_INTERRUPT_01_T;

/*-----------------------------------------------------------------------------
		0x3c interrupt_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_plug_in_prt1                 : 1 ;  //     0     
	UINT32 intr_plug_in_prt0                 : 1 ;  //     1     
	UINT32 int_hdmi5v_fall_prt3              : 1 ;  //     2     
	UINT32 int_hdmi5v_fall_prt2              : 1 ;  //     3     
	UINT32 int_hdmi5v_fall_prt1              : 1 ;  //     4     
	UINT32 int_hdmi5v_fall_prt0              : 1 ;  //     5     
	UINT32 int_hdmi5v_rise_prt3              : 1 ;  //     6     
	UINT32 int_hdmi5v_rise_prt2              : 1 ;  //     7     
	UINT32 int_hdmi5v_rise_prt1              : 1 ;  //     8     
	UINT32 int_hdmi5v_rise_prt0              : 1 ;  //     9     
	UINT32 intr_scdt_fall_prt3               : 1 ;  //    10     
	UINT32 intr_scdt_fall_prt2               : 1 ;  //    11     
	UINT32 intr_scdt_fall_prt1               : 1 ;  //    12     
	UINT32 intr_scdt_fall_prt0               : 1 ;  //    13     
	UINT32 intr_scdt_rise_prt3               : 1 ;  //    14     
	UINT32 intr_scdt_rise_prt2               : 1 ;  //    15     
	UINT32 intr_scdt_rise_prt1               : 1 ;  //    16     
	UINT32 intr_scdt_rise_prt0               : 1 ;  //    17     
	UINT32 intr_hdmi_mode_prt3               : 1 ;  //    18     
	UINT32 intr_hdmi_mode_prt2               : 1 ;  //    19     
	UINT32 intr_hdmi_mode_prt1               : 1 ;  //    20     
	UINT32 intr_hdmi_mode_prt0               : 1 ;  //    21     
	UINT32 intr_set_mute                      : 1 ;  //    22     
	UINT32 intr_clr_mute                      : 1 ;  //    23     
	UINT32 intr_vsync_prt3                   : 1 ;  //    24     
	UINT32 intr_vsync_prt2                   : 1 ;  //    25     
	UINT32 intr_vsync_prt1                   : 1 ;  //    26     
	UINT32 intr_vsync_prt0                   : 1 ;  //    27     
	UINT32 intr_hdmi5v_fedge_prt3            : 1 ;  //    28     
	UINT32 intr_hdmi5v_fedge_prt2            : 1 ;  //    29     
	UINT32 intr_hdmi5v_fedge_prt1            : 1 ;  //    30     
	UINT32 intr_hdmi5v_fedge_prt0            : 1 ;  //    31     
} M14B0_INTERRUPT_02_T;

/*-----------------------------------------------------------------------------
		0x40 interrupt_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_pathen_clr               : 1 ;  //     0     
	UINT32 intr_cbus_mute_set                 : 1 ;  //     1     
	UINT32 intr_cbus_mute_clr                 : 1 ;  //     2     
	UINT32 intr_cbus_new_mscerr               : 1 ;  //     3     
	UINT32 intr_cbus_new_rcp                  : 1 ;  //     4     
	UINT32 intr_cbus_cmd_set_hpd              : 1 ;  //     5     
	UINT32 intr_cbus_cmd_get_state            : 1 ;  //     6     
	UINT32 intr_cbus_cmd_get_vdid             : 1 ;  //     7     
	UINT32 intr_cbus_cmd_rd_devcap            : 1 ;  //     8     
	UINT32 intr_cbus_cmd_rd_devintr           : 1 ;  //     9     
	UINT32 intr_cbus_cmd_pathen_set           : 1 ;  //    10     
	UINT32 intr_cbus_cmd_pathen_clr           : 1 ;  //    11     
	UINT32 intr_cbus_cmd_dcaprd_set           : 1 ;  //    12     
	UINT32 intr_cbus_cmd_3dreq_set            : 1 ;  //    13     
	UINT32 intr_cbus_cmd_grtwrt_set           : 1 ;  //    14     
	UINT32 intr_cbus_cmd_reqwrt_set           : 1 ;  //    15     
	UINT32 intr_cbus_cmd_wrt_burst            : 1 ;  //    16     
	UINT32 intr_cbus_cmd_dscrchg_set          : 1 ;  //    17     
	UINT32 intr_cbus_cmd_dcapchg_set          : 1 ;  //    18     
	UINT32 intr_cbus_cmd_edidchg_set          : 1 ;  //    19     
	UINT32 intr_cbus_cmd_clr_hpd              : 1 ;  //    20     
	UINT32 intr_cbus_cmd_rap_poll             : 1 ;  //    21     
	UINT32 intr_cbus_cmd_rap_on               : 1 ;  //    22     
	UINT32 intr_cbus_cmd_rap_off              : 1 ;  //    23     
	UINT32 intr_cbus_cmd_rcp                  : 1 ;  //    24     
	UINT32 intr_cbus_cmd_msc_err              : 1 ;  //    25     
	UINT32 intr_plug_out_prt3                : 1 ;  //    26     
	UINT32 intr_plug_out_prt2                : 1 ;  //    27     
	UINT32 intr_plug_out_prt1                : 1 ;  //    28     
	UINT32 intr_plug_out_prt0                : 1 ;  //    29     
	UINT32 intr_plug_in_prt3                 : 1 ;  //    30     
	UINT32 intr_plug_in_prt2                 : 1 ;  //    31     
} M14B0_INTERRUPT_03_T;

/*-----------------------------------------------------------------------------
		0x44 interrupt_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_ready                    : 1 ;  //     0     
	UINT32 intr_cbus_disconn                  : 1 ;  //     1     
	UINT32 intr_cbus_max_nack                 : 1 ;  //     2     
	UINT32 intr_cbus_max_retx                 : 1 ;  //     3     
	UINT32 intr_cbus_max_rerx                 : 1 ;  //     4     
	UINT32 intr_cbus_new_devstate             : 1 ;  //     5     
	UINT32 intr_cbus_new_vdid                 : 1 ;  //     6     
	UINT32 intr_cbus_new_devcap               : 1 ;  //     7     
	UINT32 intr_cbus_chg_dscr_src             : 1 ;  //     8     
	UINT32 intr_cbus_chg_dscr_sink            : 1 ;  //     9     
	UINT32 intr_cbus_chg_ppmode               : 1 ;  //    10     
	UINT32 intr_cbus_chg_24mode               : 1 ;  //    11     
	UINT32 intr_cbus_pathen_set               : 1 ;  //    12     
	UINT32 intr_cbus_new_dscr                 : 1 ;  //    13     
	UINT32 intr_cbus_new_intstat              : 1 ;  //    14     
	UINT32 intr_cbus_new_dcap                 : 1 ;  //    15     
	UINT32 intr_cbus_new_ucp                  : 1 ;  //    16     
	UINT32 intr_cbus_cmd_ucp                  : 1 ;  //    17     
	UINT32 intr_auth_init_prt3               : 1 ;  //    18     
	UINT32 intr_auth_init_prt2               : 1 ;  //    19     
	UINT32 intr_auth_init_prt1               : 1 ;  //    20     
	UINT32 intr_auth_done_prt3               : 1 ;  //    21     
	UINT32 intr_auth_done_prt2               : 1 ;  //    22     
	UINT32 intr_auth_done_prt1               : 1 ;  //    23     
} M14B0_INTERRUPT_04_T;

/*-----------------------------------------------------------------------------
		0x48 system_control_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdmi_mode_sel                 : 1 ;  //     0     
	UINT32 reg_hdmi_mode_prt3                : 1 ;  //     1     
	UINT32 reg_hdmi_mode_prt2                : 1 ;  //     2     
	UINT32 reg_hdmi_mode_prt1                : 1 ;  //     3     
	UINT32 reg_hdmi_mode_prt0                : 1 ;  //     4     
	UINT32 reg_cd_sense_prt3                  : 1 ;  //     5     
	UINT32 reg_cd_sense_org_prt3              : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_conn_prt3                     : 1 ;  //     8     
	UINT32 reg_conn_prt2                     : 1 ;  //     9     
	UINT32 reg_conn_prt1                     : 1 ;  //    10     
	UINT32 reg_conn_prt0                     : 1 ;  //    11     
	UINT32 reg_scdt_prt3                     : 1 ;  //    12     
	UINT32 reg_scdt_prt2                     : 1 ;  //    13     
	UINT32 reg_scdt_prt1                     : 1 ;  //    14     
	UINT32 reg_scdt_prt0                     : 1 ;  //    15     
	UINT32 reg_hdmi5v_prt3                   : 1 ;  //    16     
	UINT32 reg_hdmi5v_prt2                   : 1 ;  //    17     
	UINT32 reg_hdmi5v_prt1                   : 1 ;  //    18     
	UINT32 reg_hdmi5v_prt0                   : 1 ;  //    19     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_prt_sel                        : 2 ;  // 25:24     
} M14B0_SYSTEM_CONTROL_00_T;

/*-----------------------------------------------------------------------------
		0x4c system_control_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_avc_en                         : 1 ;  //     0     
	UINT32 reg_aac_en                         : 1 ;  //     1     
	UINT32 reg_mute_vid_stat                  : 1 ;  //     2     
	UINT32 reg_mute_vid                       : 1 ;  //     3     
	UINT32 reg_mute_aud_stat                  : 1 ;  //     4     
	UINT32 reg_mute_aud                       : 1 ;  //     5     
	UINT32 reg_mute_aud_nosync                : 1 ;  //     6     
	UINT32 reg_mute_aud_dvi                   : 1 ;  //     7     
	UINT32 reg_mute_aud_pol                   : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_preamble_cnt_prt3             : 4 ;  // 19:16     
	UINT32 reg_preamble_cnt_prt2             : 4 ;  // 23:20     
	UINT32 reg_preamble_cnt_prt1             : 4 ;  // 27:24     
	UINT32 reg_preamble_cnt_prt0             : 4 ;  // 31:28     
} M14B0_SYSTEM_CONTROL_01_T;

/*-----------------------------------------------------------------------------
		0x50 system_control_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_afifo_ctrl_thr                 : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vfifo_ctrl_thr                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_afifo_pdiff                    : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_vfifo_pdiff                    : 6 ;  // 29:24     
} M14B0_SYSTEM_CONTROL_02_T;

/*-----------------------------------------------------------------------------
		0x54 hdcp_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0_prt0                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_mode_chg_prt0     : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_nosync_prt0       : 1 ;  //     9     
	UINT32 reg_hdcp_dbg_bksv_ld_prt0         : 1 ;  //    10     
	UINT32 reg_hdcp_authed_prt0              : 1 ;  //    11     
	UINT32 reg_hdcp_bypass_xor_prt0          : 1 ;  //    12     
	UINT32 reg_hdcp_disable_prt0             : 1 ;  //    13     
	UINT32 reg_hdcp_enc_en_prt0              : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bist_err_prt0                 : 1 ;  //    16     
	UINT32 reg_bist_done_prt0                : 1 ;  //    17     
	UINT32 reg_bist_init_prt0                : 1 ;  //    18     
} M14B0_HDCP_00_T;

/*-----------------------------------------------------------------------------
		0x58 hdcp_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1_prt0                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2_prt0                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3_prt0                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4_prt0                : 8 ;  // 31:24     
} M14B0_HDCP_01_T;

/*-----------------------------------------------------------------------------
		0x5c hdcp_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0_prt0              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5_prt0                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6_prt0                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7_prt0                : 8 ;  // 31:24     
} M14B0_HDCP_02_T;

/*-----------------------------------------------------------------------------
		0x60 hdcp_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1_prt0              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2_prt0              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3_prt0              : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4_prt0              : 8 ;  // 31:24     
} M14B0_HDCP_03_T;

/*-----------------------------------------------------------------------------
		0x64 hdcp_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0_prt0              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1_prt0              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2_prt0              : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3_prt0              : 8 ;  // 31:24     
} M14B0_HDCP_04_T;

/*-----------------------------------------------------------------------------
		0x68 hdcp_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps_prt0               : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0_prt0                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1_prt0                : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4_prt0              : 8 ;  // 31:24     
} M14B0_HDCP_05_T;

/*-----------------------------------------------------------------------------
		0x6c hdcp_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0_prt1                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_mode_chg_prt1     : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_nosync_prt1       : 1 ;  //     9     
	UINT32 reg_hdcp_dbg_bksv_ld_prt1         : 1 ;  //    10     
	UINT32 reg_hdcp_authed_prt1              : 1 ;  //    11     
	UINT32 reg_hdcp_bypass_xor_prt1          : 1 ;  //    12     
	UINT32 reg_hdcp_disable_prt1             : 1 ;  //    13     
	UINT32 reg_hdcp_enc_en_prt1              : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bist_err_prt1                 : 1 ;  //    16     
	UINT32 reg_bist_done_prt1                : 1 ;  //    17     
	UINT32 reg_bist_init_prt1                : 1 ;  //    18     
} M14B0_HDCP_06_T;

/*-----------------------------------------------------------------------------
		0x70 hdcp_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1_prt1                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2_prt1                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3_prt1                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4_prt1                : 8 ;  // 31:24     
} M14B0_HDCP_07_T;

/*-----------------------------------------------------------------------------
		0x74 hdcp_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0_prt1              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5_prt1                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6_prt1                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7_prt1                : 8 ;  // 31:24     
} M14B0_HDCP_08_T;

/*-----------------------------------------------------------------------------
		0x78 hdcp_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1_prt1              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2_prt1              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3_prt1              : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4_prt1              : 8 ;  // 31:24     
} M14B0_HDCP_09_T;

/*-----------------------------------------------------------------------------
		0x7c hdcp_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0_prt1              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1_prt1              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2_prt1              : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3_prt1              : 8 ;  // 31:24     
} M14B0_HDCP_0A_T;

/*-----------------------------------------------------------------------------
		0x80 hdcp_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps_prt1               : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0_prt1                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1_prt1                : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4_prt1              : 8 ;  // 31:24     
} M14B0_HDCP_0B_T;

/*-----------------------------------------------------------------------------
		0x84 hdcp_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0_prt2                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_mode_chg_prt2     : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_nosync_prt2       : 1 ;  //     9     
	UINT32 reg_hdcp_dbg_bksv_ld_prt2         : 1 ;  //    10     
	UINT32 reg_hdcp_authed_prt2              : 1 ;  //    11     
	UINT32 reg_hdcp_bypass_xor_prt2          : 1 ;  //    12     
	UINT32 reg_hdcp_disable_prt2             : 1 ;  //    13     
	UINT32 reg_hdcp_enc_en_prt2              : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bist_err_prt2                 : 1 ;  //    16     
	UINT32 reg_bist_done_prt2                : 1 ;  //    17     
	UINT32 reg_bist_init_prt2                : 1 ;  //    18     
} M14B0_HDCP_0C_T;

/*-----------------------------------------------------------------------------
		0x88 hdcp_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1_prt2                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2_prt2                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3_prt2                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4_prt2                : 8 ;  // 31:24     
} M14B0_HDCP_0D_T;

/*-----------------------------------------------------------------------------
		0x8c hdcp_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0_prt2              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5_prt2                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6_prt2                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7_prt2                : 8 ;  // 31:24     
} M14B0_HDCP_0E_T;

/*-----------------------------------------------------------------------------
		0x90 hdcp_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1_prt2              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2_prt2              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3_prt2              : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4_prt2              : 8 ;  // 31:24     
} M14B0_HDCP_0F_T;

/*-----------------------------------------------------------------------------
		0x94 hdcp_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0_prt2              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1_prt2              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2_prt2              : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3_prt2              : 8 ;  // 31:24     
} M14B0_HDCP_10_T;

/*-----------------------------------------------------------------------------
		0x98 hdcp_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps_prt2               : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0_prt2                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1_prt2                : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4_prt2              : 8 ;  // 31:24     
} M14B0_HDCP_11_T;

/*-----------------------------------------------------------------------------
		0x9c hdcp_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0_prt3                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_mode_chg_prt3     : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_nosync_prt3       : 1 ;  //     9     
	UINT32 reg_hdcp_dbg_bksv_ld_prt3         : 1 ;  //    10     
	UINT32 reg_hdcp_authed_prt3              : 1 ;  //    11     
	UINT32 reg_hdcp_bypass_xor_prt3          : 1 ;  //    12     
	UINT32 reg_hdcp_disable_prt3             : 1 ;  //    13     
	UINT32 reg_hdcp_enc_en_prt3              : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bist_err_prt3                 : 1 ;  //    16     
	UINT32 reg_bist_done_prt3                : 1 ;  //    17     
	UINT32 reg_bist_init_prt3                : 1 ;  //    18     
} M14B0_HDCP_12_T;

/*-----------------------------------------------------------------------------
		0xa0 hdcp_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1_prt3                : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2_prt3                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3_prt3                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4_prt3                : 8 ;  // 31:24     
} M14B0_HDCP_13_T;

/*-----------------------------------------------------------------------------
		0xa4 hdcp_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0_prt3              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5_prt3                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6_prt3                : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7_prt3                : 8 ;  // 31:24     
} M14B0_HDCP_14_T;

/*-----------------------------------------------------------------------------
		0xa8 hdcp_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1_prt3              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2_prt3              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3_prt3              : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4_prt3              : 8 ;  // 31:24     
} M14B0_HDCP_15_T;

/*-----------------------------------------------------------------------------
		0xac hdcp_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0_prt3              : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1_prt3              : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2_prt3              : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3_prt3              : 8 ;  // 31:24     
} M14B0_HDCP_16_T;

/*-----------------------------------------------------------------------------
		0xb0 hdcp_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps_prt3               : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0_prt3                : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1_prt3                : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4_prt3              : 8 ;  // 31:24     
} M14B0_HDCP_17_T;

/*-----------------------------------------------------------------------------
		0xb4 packet_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_dec                        : 8 ;  //  7: 0     
	UINT32 reg_spd_dec                        : 8 ;  // 15: 8     
	UINT32 reg_pkt_clr                        : 1 ;  //    16     
	UINT32 reg_pkt_clr_dvi                    : 1 ;  //    17     
	UINT32 reg_pkt_clr_nosync                 : 1 ;  //    18     
	UINT32 reg_pkt_clr_no_packet              : 1 ;  //    19     
	UINT32 reg_pkt_clr_avi_cnt                : 4 ;  // 23:20     
	UINT32 reg_pkt_clr_gcp_cnt                : 4 ;  // 27:24     
	UINT32 reg_pkt_clr_vsi_cnt                : 4 ;  // 31:28     
} M14B0_PACKET_00_T;

/*-----------------------------------------------------------------------------
		0xb8 packet_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_01_T;

/*-----------------------------------------------------------------------------
		0xbc packet_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_02_T;

/*-----------------------------------------------------------------------------
		0xc0 packet_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_03_T;

/*-----------------------------------------------------------------------------
		0xc4 packet_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_04_T;

/*-----------------------------------------------------------------------------
		0xc8 packet_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_05_T;

/*-----------------------------------------------------------------------------
		0xcc packet_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_06_T;

/*-----------------------------------------------------------------------------
		0xd0 packet_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_07_T;

/*-----------------------------------------------------------------------------
		0xd4 packet_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_08_T;

/*-----------------------------------------------------------------------------
		0xd8 packet_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_09_T;

/*-----------------------------------------------------------------------------
		0xdc packet_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_0A_T;

/*-----------------------------------------------------------------------------
		0xe0 packet_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_0B_T;

/*-----------------------------------------------------------------------------
		0xe4 packet_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_0C_T;

/*-----------------------------------------------------------------------------
		0xe8 packet_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_0D_T;

/*-----------------------------------------------------------------------------
		0xec packet_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_0E_T;

/*-----------------------------------------------------------------------------
		0xf0 packet_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_0F_T;

/*-----------------------------------------------------------------------------
		0xf4 packet_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_10_T;

/*-----------------------------------------------------------------------------
		0xf8 packet_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_asp_hdr_0                  : 8 ;  // 23:16     
	UINT32 reg_pkt_asp_hdr_1                  : 8 ;  // 31:24     
} M14B0_PACKET_11_T;

/*-----------------------------------------------------------------------------
		0xfc packet_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_12_T;

/*-----------------------------------------------------------------------------
		0x100 packet_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_13_T;

/*-----------------------------------------------------------------------------
		0x104 packet_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_14_T;

/*-----------------------------------------------------------------------------
		0x108 packet_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_15_T;

/*-----------------------------------------------------------------------------
		0x10c packet_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_16_T;

/*-----------------------------------------------------------------------------
		0x110 packet_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_17_T;

/*-----------------------------------------------------------------------------
		0x114 packet_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_18_T;

/*-----------------------------------------------------------------------------
		0x118 packet_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_19_T;

/*-----------------------------------------------------------------------------
		0x11c packet_1a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_1A_T;

/*-----------------------------------------------------------------------------
		0x120 packet_1b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_1B_T;

/*-----------------------------------------------------------------------------
		0x124 packet_1c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_1C_T;

/*-----------------------------------------------------------------------------
		0x128 packet_1d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_1D_T;

/*-----------------------------------------------------------------------------
		0x12c packet_1e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_1E_T;

/*-----------------------------------------------------------------------------
		0x130 packet_1f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_1F_T;

/*-----------------------------------------------------------------------------
		0x134 packet_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_20_T;

/*-----------------------------------------------------------------------------
		0x138 packet_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_hbr_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_hbr_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gcp_smute                  : 1 ;  //    16     
	UINT32 reg_pkt_gcp_cmute                  : 1 ;  //    17     
} M14B0_PACKET_21_T;

/*-----------------------------------------------------------------------------
		0x13c packet_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_hdr_1                : 8 ;  // 15: 8     
} M14B0_PACKET_22_T;

/*-----------------------------------------------------------------------------
		0x140 packet_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_3                : 8 ;  // 31:24     
} M14B0_PACKET_23_T;

/*-----------------------------------------------------------------------------
		0x144 packet_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_7                : 8 ;  // 31:24     
} M14B0_PACKET_24_T;

/*-----------------------------------------------------------------------------
		0x148 packet_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_11               : 8 ;  // 31:24     
} M14B0_PACKET_25_T;

/*-----------------------------------------------------------------------------
		0x14c packet_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_15               : 8 ;  // 31:24     
} M14B0_PACKET_26_T;

/*-----------------------------------------------------------------------------
		0x150 packet_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_19               : 8 ;  // 31:24     
} M14B0_PACKET_27_T;

/*-----------------------------------------------------------------------------
		0x154 packet_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_23               : 8 ;  // 31:24     
} M14B0_PACKET_28_T;

/*-----------------------------------------------------------------------------
		0x158 packet_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_27               : 8 ;  // 31:24     
} M14B0_PACKET_29_T;

/*-----------------------------------------------------------------------------
		0x15c packet_2a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_hdr_1                : 8 ;  // 15: 8     
} M14B0_PACKET_2A_T;

/*-----------------------------------------------------------------------------
		0x160 packet_2b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_3                : 8 ;  // 31:24     
} M14B0_PACKET_2B_T;

/*-----------------------------------------------------------------------------
		0x164 packet_2c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_7                : 8 ;  // 31:24     
} M14B0_PACKET_2C_T;

/*-----------------------------------------------------------------------------
		0x168 packet_2d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_11               : 8 ;  // 31:24     
} M14B0_PACKET_2D_T;

/*-----------------------------------------------------------------------------
		0x16c packet_2e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_15               : 8 ;  // 31:24     
} M14B0_PACKET_2E_T;

/*-----------------------------------------------------------------------------
		0x170 packet_2f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_19               : 8 ;  // 31:24     
} M14B0_PACKET_2F_T;

/*-----------------------------------------------------------------------------
		0x174 packet_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_23               : 8 ;  // 31:24     
} M14B0_PACKET_30_T;

/*-----------------------------------------------------------------------------
		0x178 packet_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_27               : 8 ;  // 31:24     
} M14B0_PACKET_31_T;

/*-----------------------------------------------------------------------------
		0x17c packet_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_32_T;

/*-----------------------------------------------------------------------------
		0x180 packet_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_33_T;

/*-----------------------------------------------------------------------------
		0x184 packet_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_34_T;

/*-----------------------------------------------------------------------------
		0x188 packet_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_35_T;

/*-----------------------------------------------------------------------------
		0x18c packet_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_36_T;

/*-----------------------------------------------------------------------------
		0x190 packet_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_37_T;

/*-----------------------------------------------------------------------------
		0x194 packet_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_38_T;

/*-----------------------------------------------------------------------------
		0x198 packet_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_39_T;

/*-----------------------------------------------------------------------------
		0x19c packet_3a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_3A_T;

/*-----------------------------------------------------------------------------
		0x1a0 packet_3b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_3B_T;

/*-----------------------------------------------------------------------------
		0x1a4 packet_3c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_3C_T;

/*-----------------------------------------------------------------------------
		0x1a8 packet_3d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_3D_T;

/*-----------------------------------------------------------------------------
		0x1ac packet_3e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_3E_T;

/*-----------------------------------------------------------------------------
		0x1b0 packet_3f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_3F_T;

/*-----------------------------------------------------------------------------
		0x1b4 packet_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_40_T;

/*-----------------------------------------------------------------------------
		0x1b8 packet_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_41_T;

/*-----------------------------------------------------------------------------
		0x1bc packet_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_0                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_1                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_2                      : 8 ;  // 23:16     
} M14B0_PACKET_42_T;

/*-----------------------------------------------------------------------------
		0x1c0 packet_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_3                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_4                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_5                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_6                      : 8 ;  // 31:24     
} M14B0_PACKET_43_T;

/*-----------------------------------------------------------------------------
		0x1c4 packet_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_7                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_8                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_9                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_10                     : 8 ;  // 31:24     
} M14B0_PACKET_44_T;

/*-----------------------------------------------------------------------------
		0x1c8 packet_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_11                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_12                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_13                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_14                     : 8 ;  // 31:24     
} M14B0_PACKET_45_T;

/*-----------------------------------------------------------------------------
		0x1cc packet_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_15                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_16                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_17                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_18                     : 8 ;  // 31:24     
} M14B0_PACKET_46_T;

/*-----------------------------------------------------------------------------
		0x1d0 packet_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_19                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_20                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_21                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_22                     : 8 ;  // 31:24     
} M14B0_PACKET_47_T;

/*-----------------------------------------------------------------------------
		0x1d4 packet_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_23                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_24                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_25                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_26                     : 8 ;  // 31:24     
} M14B0_PACKET_48_T;

/*-----------------------------------------------------------------------------
		0x1d8 packet_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_27                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_28                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_29                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_30                     : 8 ;  // 31:24     
} M14B0_PACKET_49_T;

/*-----------------------------------------------------------------------------
		0x1dc packet_4a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_hdr_1                  : 8 ;  // 15: 8     
} M14B0_PACKET_4A_T;

/*-----------------------------------------------------------------------------
		0x1e0 packet_4b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_3                  : 8 ;  // 31:24     
} M14B0_PACKET_4B_T;

/*-----------------------------------------------------------------------------
		0x1e4 packet_4c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_7                  : 8 ;  // 31:24     
} M14B0_PACKET_4C_T;

/*-----------------------------------------------------------------------------
		0x1e8 packet_4d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_11                 : 8 ;  // 31:24     
} M14B0_PACKET_4D_T;

/*-----------------------------------------------------------------------------
		0x1ec packet_4e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_15                 : 8 ;  // 31:24     
} M14B0_PACKET_4E_T;

/*-----------------------------------------------------------------------------
		0x1f0 packet_4f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_19                 : 8 ;  // 31:24     
} M14B0_PACKET_4F_T;

/*-----------------------------------------------------------------------------
		0x1f4 packet_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_23                 : 8 ;  // 31:24     
} M14B0_PACKET_50_T;

/*-----------------------------------------------------------------------------
		0x1f8 packet_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_27                 : 8 ;  // 31:24     
} M14B0_PACKET_51_T;

/*-----------------------------------------------------------------------------
		0x1fc packet_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_vsi_hdmi_vic               : 8 ;  //  7: 0     
	UINT32 reg_dbg_vsi_3d_struct              : 4 ;  // 11: 8     
	UINT32 reg_dbg_vsi_3d_meta_en             : 1 ;  //    12     
	UINT32 reg_dbg_vsi_3d                     : 1 ;  //    13     
	UINT32 reg_dbg_vsi_4k2k                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_pkt_clr_spd_cnt                : 4 ;  // 19:16     
} M14B0_PACKET_52_T;

/*-----------------------------------------------------------------------------
		0x200 video_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pr_tx                          : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_pr_rx                          : 4 ;  // 11: 8     
	UINT32 reg_pr_rx_en                       : 1 ;  //    12     
	UINT32 reg_pr_cmu_sync                    : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_pr_cbcr                        : 4 ;  // 19:16     
} M14B0_VIDEO_00_T;

/*-----------------------------------------------------------------------------
		0x204 video_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_align422_tx                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_align422_rx                    : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_align422_rx_en                 : 1 ;  //    12     
	UINT32 reg_align422_swap_rb               : 1 ;  //    13     
} M14B0_VIDEO_01_T;

/*-----------------------------------------------------------------------------
		0x208 video_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cmode_tx                       : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cmode_rx                       : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cmode_rx_en                    : 1 ;  //    16     
	UINT32 reg_cmode_ppll_sync                : 1 ;  //    17     
} M14B0_VIDEO_02_T;

/*-----------------------------------------------------------------------------
		0x20c video_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_neg_pol_en                     : 1 ;  //     0     
	UINT32 reg_inv_hs                         : 1 ;  //     1     
	UINT32 reg_inv_vs                         : 1 ;  //     2     
	UINT32 reg_inv_eo                         : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vf_clr                         : 1 ;  //     8     
	UINT32 reg_vf_clr_nosync                  : 1 ;  //     9     
} M14B0_VIDEO_03_T;

/*-----------------------------------------------------------------------------
		0x210 video_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_tot                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_tot                          : 12;  // 27:16     
} M14B0_VIDEO_04_T;

/*-----------------------------------------------------------------------------
		0x214 video_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_av                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_av                           : 12;  // 27:16     
} M14B0_VIDEO_05_T;

/*-----------------------------------------------------------------------------
		0x218 video_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_fp                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_v_fp                           : 6 ;  // 21:16     
} M14B0_VIDEO_06_T;

/*-----------------------------------------------------------------------------
		0x21c video_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_bp                           : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_v_bp                           : 7 ;  // 22:16     
} M14B0_VIDEO_07_T;

/*-----------------------------------------------------------------------------
		0x220 video_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_sync                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_v_sync                         : 8 ;  // 23:16     
} M14B0_VIDEO_08_T;

/*-----------------------------------------------------------------------------
		0x224 video_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vid_blank_g                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_vid_blank_r                    : 12;  // 27:16     
} M14B0_VIDEO_09_T;

/*-----------------------------------------------------------------------------
		0x228 video_0a ''
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
} M14B0_VIDEO_0A_T;

/*-----------------------------------------------------------------------------
		0x22c audio_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_clk_aud_div                : 2 ;  //  1: 0     
	UINT32 reg_acr_n_fs                       : 2 ;  //  3: 2     
	UINT32 reg_acr_en                         : 1 ;  //     4     
	UINT32 reg_acr_ncts_rx_en                 : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_acr_adj_thr                    : 4 ;  // 11: 8     
	UINT32 reg_acr_adj_en                     : 1 ;  //    12     
	UINT32 reg_acr_en_man                     : 1 ;  //    13     
} M14B0_AUDIO_00_T;

/*-----------------------------------------------------------------------------
		0x230 audio_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_tx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_tx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_tx_2                     : 4 ;  // 19:16     
} M14B0_AUDIO_01_T;

/*-----------------------------------------------------------------------------
		0x234 audio_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_rx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_rx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_rx_2                     : 4 ;  // 19:16     
} M14B0_AUDIO_02_T;

/*-----------------------------------------------------------------------------
		0x238 audio_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_tx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_tx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_tx_2                   : 4 ;  // 19:16     
} M14B0_AUDIO_03_T;

/*-----------------------------------------------------------------------------
		0x23c audio_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_rx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_rx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_rx_2                   : 4 ;  // 19:16     
} M14B0_AUDIO_04_T;

/*-----------------------------------------------------------------------------
		0x240 audio_05 ''
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
} M14B0_AUDIO_05_T;

/*-----------------------------------------------------------------------------
		0x244 audio_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sp_paerr                       : 1 ;  //     0     
	UINT32 reg_sp_pserr                       : 1 ;  //     1     
	UINT32 reg_sp_gnd_mode                    : 1 ;  //     2     
	UINT32 reg_sp_en                          : 1 ;  //     3     
} M14B0_AUDIO_06_T;

/*-----------------------------------------------------------------------------
		0x248 audio_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_ow_byte1                 : 8 ;  //  7: 0     
	UINT32 reg_achst_ow_bit2                  : 1 ;  //     8     
	UINT32 reg_achst_ow_en                    : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_achst_fs                       : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_achst_byte0                    : 8 ;  // 31:24     
} M14B0_AUDIO_07_T;

/*-----------------------------------------------------------------------------
		0x24c audio_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_byte1                    : 8 ;  //  7: 0     
	UINT32 reg_achst_byte2                    : 8 ;  // 15: 8     
	UINT32 reg_achst_byte3                    : 8 ;  // 23:16     
	UINT32 reg_achst_byte4                    : 8 ;  // 31:24     
} M14B0_AUDIO_08_T;

/*-----------------------------------------------------------------------------
		0x250 audio_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_pc_0                     : 8 ;  //  7: 0     
	UINT32 reg_burst_pc_1                     : 8 ;  // 15: 8     
	UINT32 reg_burst_pd_0                     : 8 ;  // 23:16     
	UINT32 reg_burst_pd_1                     : 8 ;  // 31:24     
} M14B0_AUDIO_09_T;

/*-----------------------------------------------------------------------------
		0x254 audio_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_i2s_ch_mute                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_i2s_len_ovrr                   : 4 ;  // 11: 8     
	UINT32 reg_i2s_len_ovrr_en                : 1 ;  //    12     
} M14B0_AUDIO_0A_T;

/*-----------------------------------------------------------------------------
		0x258 edid_heac_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_arc_src                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_edid_rd_done_prt0             : 1 ;  //     4     
	UINT32 reg_edid_rd_done_prt1             : 1 ;  //     5     
	UINT32 reg_edid_rd_done_prt2             : 1 ;  //     6     
	UINT32 reg_edid_rd_done_prt3             : 1 ;  //     7     
	UINT32 reg_edid_rd_cnt_prt0              : 4 ;  // 11: 8     
	UINT32 reg_edid_rd_cnt_prt1              : 4 ;  // 15:12     
	UINT32 reg_edid_rd_cnt_prt2              : 4 ;  // 19:16     
	UINT32 reg_edid_rd_cnt_prt3              : 4 ;  // 23:20     
} M14B0_EDID_HEAC_00_T;

/*-----------------------------------------------------------------------------
		0x25c cbus_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_st                        : 3 ;  //  2: 0     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_cbus_intr_dscrchg              : 1 ;  //     8     
	UINT32 reg_cbus_pp_mode                   : 1 ;  //     9     
	UINT32 reg_cbus_path_en                   : 1 ;  //    10     
	UINT32 reg_cbus_rap_content_on            : 1 ;  //    11     
	UINT32 reg_cbus_in                        : 1 ;  //    12     
	UINT32 reg_cbus_intr_dscrchg_clr          : 1 ;  //    13     
	UINT32 reg_cbus_ctrl_inv                  : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cbus_disconn_en                : 1 ;  //    16     
	UINT32 reg_cbus_out_inv                   : 1 ;  //    17     
	UINT32 reg_cbus_out_en_sel                : 1 ;  //    18     
	UINT32 reg_phy_sink_mhl_en                : 1 ;  //    19     
	UINT32 reg_phy_sink_vbus_en               : 1 ;  //    20     
	UINT32 reg_phy_sink_ztmds                 : 1 ;  //    21     
	UINT32 reg_phy_sink_zterm                 : 1 ;  //    22     
	UINT32 reg_phy_sink_cbus_zon              : 1 ;  //    23     
	UINT32 reg_phy_sink_cbus_zdis             : 1 ;  //    24     
} M14B0_CBUS_00_T;

/*-----------------------------------------------------------------------------
		0x260 cbus_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_wait_con               : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cbus_rtx_st                    : 8 ;  // 15: 8     
	UINT32 reg_cbus_disconn                   : 1 ;  //    16     
	UINT32 reg_cbus_warb_done                 : 1 ;  //    17     
	UINT32 reg_cbus_conn_done                 : 1 ;  //    18     
	UINT32 reg_cbus_pull_up                   : 1 ;  //    19     
	UINT32 reg_cbus_disc_dt                   : 1 ;  //    20     
	UINT32 reg_cbus_wake_dt                   : 1 ;  //    21     
} M14B0_CBUS_01_T;

/*-----------------------------------------------------------------------------
		0x264 cbus_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_thres             : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_n_retry                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_wait_arb_0             : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_wait_arb_1             : 2 ;  // 25:24     
} M14B0_CBUS_02_T;

/*-----------------------------------------------------------------------------
		0x268 cbus_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_cnt               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_src_bit_length         : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_st                     : 4 ;  // 19:16     
} M14B0_CBUS_03_T;

/*-----------------------------------------------------------------------------
		0x26c cbus_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msc_wrt_stat_pathen_set    : 1 ;  //     0     
	UINT32 reg_man_msc_wrt_stat_dcaprd_set    : 1 ;  //     1     
	UINT32 reg_man_msc_set_intr_3dreq_set     : 1 ;  //     2     
	UINT32 reg_man_msc_rd_devcap              : 1 ;  //     3     
	UINT32 reg_man_msc_get_vdid               : 1 ;  //     4     
	UINT32 reg_man_msc_get_state              : 1 ;  //     5     
	UINT32 reg_man_msc_clr_hpd                : 1 ;  //     6     
	UINT32 reg_man_msc_set_hpd                : 1 ;  //     7     
	UINT32 reg_seq_msc_wrt_stat_dcaprd_set    : 4 ;  // 11: 8     
	UINT32 reg_seq_msc_wrt_stat_pathen_set    : 4 ;  // 15:12     
	UINT32 reg_seq_msc_rd_devcap              : 4 ;  // 19:16     
	UINT32 reg_seq_msc_get_vdid               : 4 ;  // 23:20     
	UINT32 reg_seq_msc_get_state              : 4 ;  // 27:24     
	UINT32 reg_seq_msc_set_hpd                : 4 ;  // 31:28     
} M14B0_CBUS_04_T;

/*-----------------------------------------------------------------------------
		0x270 cbus_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_hpd             : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_seq_st                         : 8 ;  // 15: 8     
	UINT32 reg_cmd_st                         : 8 ;  // 23:16     
	UINT32 reg_man_msc_wrt_stat_pathen_clr    : 1 ;  //    24     
} M14B0_CBUS_05_T;

/*-----------------------------------------------------------------------------
		0x274 cbus_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rd_devcap           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_vdid            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_get_state           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_clr_hpd             : 5 ;  // 28:24     
} M14B0_CBUS_06_T;

/*-----------------------------------------------------------------------------
		0x278 cbus_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_3dreq_set  : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_msc_err         : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_burst           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_rd_dev_intr_chg     : 5 ;  // 28:24     
} M14B0_CBUS_07_T;

/*-----------------------------------------------------------------------------
		0x27c cbus_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_dcapchgset : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_set_intr_dscrchgset : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_set_intr_reqwrt_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_grtwrt_set : 5 ;  // 28:24     
} M14B0_CBUS_08_T;

/*-----------------------------------------------------------------------------
		0x280 cbus_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_wrt_stat_pathen_clr : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_wrt_stat_pathen_set : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_stat_dcaprd_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_edidchgset : 5 ;  // 28:24     
} M14B0_CBUS_09_T;

/*-----------------------------------------------------------------------------
		0x284 cbus_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_rcp_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rapk_ecode                  : 8 ;  // 15: 8     
	UINT32 reg_rx_rapk_ecode                  : 8 ;  // 23:16     
	UINT32 reg_man_msg_rap_content_off        : 1 ;  //    24     
	UINT32 reg_man_msg_rap_content_on         : 1 ;  //    25     
	UINT32 reg_man_msg_rap_poll               : 1 ;  //    26     
} M14B0_CBUS_0A_T;

/*-----------------------------------------------------------------------------
		0x288 cbus_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_rcpk_rtn_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rcpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_rcpk_rtn_kcode              : 8 ;  // 23:16     
	UINT32 reg_man_msg_rcp_cmd                : 1 ;  //    24     
} M14B0_CBUS_0B_T;

/*-----------------------------------------------------------------------------
		0x28c cbus_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_src_support_rcp            : 1 ;  //     0     
	UINT32 reg_man_src_support_rap            : 1 ;  //     1     
} M14B0_CBUS_0C_T;

/*-----------------------------------------------------------------------------
		0x290 cbus_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rcp                 : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_state_sink_rap_cnt_off         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_state_sink_rap_cnt_on          : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_state_sink_rap_poll            : 7 ;  // 30:24     
} M14B0_CBUS_0D_T;

/*-----------------------------------------------------------------------------
		0x294 cbus_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_msc_errcode                 : 8 ;  //  7: 0     
	UINT32 reg_tx_ddc_errcode                 : 8 ;  // 15: 8     
	UINT32 reg_tx_msc_errcode                 : 8 ;  // 23:16     
	UINT32 reg_get_msc_errcode_enable         : 1 ;  //    24     
} M14B0_CBUS_0E_T;

/*-----------------------------------------------------------------------------
		0x298 cbus_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_adopter_id_h          : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_dev_cat               : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_mhl_version           : 8 ;  // 23:16     
	UINT32 reg_sink_cap_dev_state             : 8 ;  // 31:24     
} M14B0_CBUS_0F_T;

/*-----------------------------------------------------------------------------
		0x29c cbus_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_video_type            : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_aud_link_mode         : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_vid_link_mode         : 8 ;  // 23:16     
	UINT32 reg_sink_cap_adopter_id_l          : 8 ;  // 31:24     
} M14B0_CBUS_10_T;

/*-----------------------------------------------------------------------------
		0x2a0 cbus_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_device_id_h           : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_feature_flag          : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_bandwidth             : 8 ;  // 23:16     
	UINT32 reg_sink_cap_log_dev_map           : 8 ;  // 31:24     
} M14B0_CBUS_11_T;

/*-----------------------------------------------------------------------------
		0x2a4 cbus_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_intr_stat_size        : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_scratchpad_size       : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_device_id_l           : 8 ;  // 23:16     
} M14B0_CBUS_12_T;

/*-----------------------------------------------------------------------------
		0x2a8 cbus_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_intr_reserved_23          : 8 ;  //  7: 0     
	UINT32 reg_sink_intr_reserved_22          : 8 ;  // 15: 8     
	UINT32 reg_sink_intr_dchg_int             : 8 ;  // 23:16     
	UINT32 reg_sink_intr_rchg_int             : 8 ;  // 31:24     
} M14B0_CBUS_13_T;

/*-----------------------------------------------------------------------------
		0x2ac cbus_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_stat_reserved_33          : 8 ;  //  7: 0     
	UINT32 reg_sink_stat_reserved_32          : 8 ;  // 15: 8     
	UINT32 reg_sink_stat_link_mode            : 8 ;  // 23:16     
	UINT32 reg_sink_stat_conn_rdy             : 8 ;  // 31:24     
} M14B0_CBUS_14_T;

/*-----------------------------------------------------------------------------
		0x2b0 cbus_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_vendor_id_c1               : 8 ;  //  7: 0     
	UINT32 reg_src_dev_state_c0               : 8 ;  // 15: 8     
	UINT32 reg_sink_vendor_id_80              : 8 ;  // 23:16     
} M14B0_CBUS_15_T;

/*-----------------------------------------------------------------------------
		0x2b4 cbus_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_43             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_42             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_41             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_40             : 8 ;  // 31:24     
} M14B0_CBUS_16_T;

/*-----------------------------------------------------------------------------
		0x2b8 cbus_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_47             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_46             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_45             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_44             : 8 ;  // 31:24     
} M14B0_CBUS_17_T;

/*-----------------------------------------------------------------------------
		0x2bc cbus_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4b             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4a             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_49             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_48             : 8 ;  // 31:24     
} M14B0_CBUS_18_T;

/*-----------------------------------------------------------------------------
		0x2c0 cbus_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4f             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4e             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_4d             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_4c             : 8 ;  // 31:24     
} M14B0_CBUS_19_T;

/*-----------------------------------------------------------------------------
		0x2c4 cbus_1a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_adopter_id_l           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_adopter_id_h           : 8 ;  // 15: 8     
	UINT32 reg_src_cap_dev_cat                : 8 ;  // 23:16     
	UINT32 reg_src_cap_mhl_version            : 8 ;  // 31:24     
} M14B0_CBUS_1A_T;

/*-----------------------------------------------------------------------------
		0x2c8 cbus_1b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_feature_flag           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_log_dev_map            : 8 ;  // 15: 8     
	UINT32 reg_src_cap_aud_link_mode          : 8 ;  // 23:16     
	UINT32 reg_src_cap_vid_link_mode          : 8 ;  // 31:24     
} M14B0_CBUS_1B_T;

/*-----------------------------------------------------------------------------
		0x2cc cbus_1c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_intr_stat_size         : 8 ;  //  7: 0     
	UINT32 reg_src_cap_scratchpad_size        : 8 ;  // 15: 8     
	UINT32 reg_src_cap_device_id_l            : 8 ;  // 23:16     
	UINT32 reg_src_cap_device_id_h            : 8 ;  // 31:24     
} M14B0_CBUS_1C_T;

/*-----------------------------------------------------------------------------
		0x2d0 cbus_1d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_65          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_64          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_63          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_62          : 8 ;  // 31:24     
} M14B0_CBUS_1D_T;

/*-----------------------------------------------------------------------------
		0x2d4 cbus_1e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_69          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_68          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_67          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_66          : 8 ;  // 31:24     
} M14B0_CBUS_1E_T;

/*-----------------------------------------------------------------------------
		0x2d8 cbus_1f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6d          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6c          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_6b          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_6a          : 8 ;  // 31:24     
} M14B0_CBUS_1F_T;

/*-----------------------------------------------------------------------------
		0x2dc cbus_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6f          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6e          : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_next_2              : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_next_1              : 3 ;  // 26:24     
} M14B0_CBUS_20_T;

/*-----------------------------------------------------------------------------
		0x2e0 cbus_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_disc_cnt                  : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cbus_3d_wb_man                 : 1 ;  //     4     
	UINT32 reg_cbus_force_long_arb            : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_cbus_wake_pulse_width_1_min    : 8 ;  // 15: 8     
	UINT32 reg_cbus_wake_pulse_width_1_max    : 8 ;  // 23:16     
	UINT32 reg_cbus_wake_pulse_width_2_min    : 8 ;  // 31:24     
} M14B0_CBUS_21_T;

/*-----------------------------------------------------------------------------
		0x2e4 cbus_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_pulse_width_2_max    : 8 ;  //  7: 0     
	UINT32 reg_cbus_sink_pulse_width_min      : 8 ;  // 15: 8     
	UINT32 reg_cbus_sink_pulse_width_max      : 8 ;  // 23:16     
	UINT32 reg_seq_msc_wrt_burst              : 4 ;  // 27:24     
	UINT32 reg_man_msc_wrt_burst              : 1 ;  //    28     
	UINT32 reg_man_msc_set_intr_reqwrt_set    : 1 ;  //    29     
	UINT32 reg_man_msc_set_intr_dscrch_set    : 1 ;  //    30     
} M14B0_CBUS_22_T;

/*-----------------------------------------------------------------------------
		0x2e8 cbus_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_to_discover_min_2    : 8 ;  //  7: 0     
	UINT32 reg_cbus_wake_to_discover_min_1    : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_cbus_wake_to_discover_max_2    : 8 ;  // 23:16     
	UINT32 reg_cbus_wake_to_discover_max_1    : 2 ;  // 25:24     
} M14B0_CBUS_23_T;

/*-----------------------------------------------------------------------------
		0x2ec cbus_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_ack_str                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_ack_min                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_ack_max                : 8 ;  // 23:16     
} M14B0_CBUS_24_T;

/*-----------------------------------------------------------------------------
		0x2f0 cbus_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_arb_min                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_arb_max                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_min                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_max                : 8 ;  // 31:24     
} M14B0_CBUS_25_T;

/*-----------------------------------------------------------------------------
		0x2f4 cbus_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_syn0_min               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_syn0_max               : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_syn1_min               : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_syn1_max               : 8 ;  // 31:24     
} M14B0_CBUS_26_T;

/*-----------------------------------------------------------------------------
		0x2f8 cbus_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_bit_bn1                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_bit_bn2                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_bn3                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_bn4                : 8 ;  // 31:24     
} M14B0_CBUS_27_T;

/*-----------------------------------------------------------------------------
		0x2fc cbus_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_ucpk_rtn_ccode              : 8 ;  //  7: 0     
	UINT32 reg_tx_ucpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_ucpk_rtn_ccode              : 8 ;  // 23:16     
	UINT32 reg_state_sink_ucp                 : 7 ;  // 30:24     
} M14B0_CBUS_28_T;

/*-----------------------------------------------------------------------------
		0x300 cbus_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_ucp_cmd                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_man_src_support_ucp            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_man_msg_ucp_ccode              : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_err_2               : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_err_1               : 3 ;  // 26:24     
} M14B0_CBUS_29_T;

/*-----------------------------------------------------------------------------
		0x304 cbus_2a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_61          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_60          : 8 ;  // 15: 8     
	UINT32 reg_src_scratchpad_num             : 4 ;  // 19:16     
	UINT32 reg_src_scratchpad_id_sel          : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_src_mrc_unsupport              : 1 ;  //    24     
	UINT32 reg_src_mrc_support                : 1 ;  //    25     
} M14B0_CBUS_2A_T;

/*-----------------------------------------------------------------------------
		0x308 edid_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_0_prt0               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_1_prt0               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_2_prt0               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_3_prt0               : 8 ;  // 31:24     
} M14B0_EDID_00_T;

/*-----------------------------------------------------------------------------
		0x30c edid_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_4_prt0               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_5_prt0               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_6_prt0               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_7_prt0               : 8 ;  // 31:24     
} M14B0_EDID_01_T;

/*-----------------------------------------------------------------------------
		0x310 edid_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_8_prt0               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_9_prt0               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_10_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_11_prt0              : 8 ;  // 31:24     
} M14B0_EDID_02_T;

/*-----------------------------------------------------------------------------
		0x314 edid_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_12_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_13_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_14_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_15_prt0              : 8 ;  // 31:24     
} M14B0_EDID_03_T;

/*-----------------------------------------------------------------------------
		0x318 edid_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_16_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_17_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_18_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_19_prt0              : 8 ;  // 31:24     
} M14B0_EDID_04_T;

/*-----------------------------------------------------------------------------
		0x31c edid_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_20_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_21_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_22_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_23_prt0              : 8 ;  // 31:24     
} M14B0_EDID_05_T;

/*-----------------------------------------------------------------------------
		0x320 edid_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_24_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_25_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_26_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_27_prt0              : 8 ;  // 31:24     
} M14B0_EDID_06_T;

/*-----------------------------------------------------------------------------
		0x324 edid_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_28_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_29_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_30_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_31_prt0              : 8 ;  // 31:24     
} M14B0_EDID_07_T;

/*-----------------------------------------------------------------------------
		0x328 edid_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_32_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_33_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_34_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_35_prt0              : 8 ;  // 31:24     
} M14B0_EDID_08_T;

/*-----------------------------------------------------------------------------
		0x32c edid_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_36_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_37_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_38_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_39_prt0              : 8 ;  // 31:24     
} M14B0_EDID_09_T;

/*-----------------------------------------------------------------------------
		0x330 edid_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_40_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_41_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_42_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_43_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0A_T;

/*-----------------------------------------------------------------------------
		0x334 edid_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_44_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_45_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_46_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_47_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0B_T;

/*-----------------------------------------------------------------------------
		0x338 edid_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_48_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_49_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_50_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_51_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0C_T;

/*-----------------------------------------------------------------------------
		0x33c edid_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_52_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_53_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_54_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_55_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0D_T;

/*-----------------------------------------------------------------------------
		0x340 edid_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_56_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_57_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_58_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_59_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0E_T;

/*-----------------------------------------------------------------------------
		0x344 edid_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_60_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_61_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_62_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_63_prt0              : 8 ;  // 31:24     
} M14B0_EDID_0F_T;

/*-----------------------------------------------------------------------------
		0x348 edid_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_64_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_65_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_66_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_67_prt0              : 8 ;  // 31:24     
} M14B0_EDID_10_T;

/*-----------------------------------------------------------------------------
		0x34c edid_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_68_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_69_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_70_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_71_prt0              : 8 ;  // 31:24     
} M14B0_EDID_11_T;

/*-----------------------------------------------------------------------------
		0x350 edid_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_72_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_73_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_74_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_75_prt0              : 8 ;  // 31:24     
} M14B0_EDID_12_T;

/*-----------------------------------------------------------------------------
		0x354 edid_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_76_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_77_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_78_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_79_prt0              : 8 ;  // 31:24     
} M14B0_EDID_13_T;

/*-----------------------------------------------------------------------------
		0x358 edid_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_80_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_81_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_82_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_83_prt0              : 8 ;  // 31:24     
} M14B0_EDID_14_T;

/*-----------------------------------------------------------------------------
		0x35c edid_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_84_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_85_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_86_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_87_prt0              : 8 ;  // 31:24     
} M14B0_EDID_15_T;

/*-----------------------------------------------------------------------------
		0x360 edid_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_88_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_89_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_90_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_91_prt0              : 8 ;  // 31:24     
} M14B0_EDID_16_T;

/*-----------------------------------------------------------------------------
		0x364 edid_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_92_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_93_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_94_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_95_prt0              : 8 ;  // 31:24     
} M14B0_EDID_17_T;

/*-----------------------------------------------------------------------------
		0x368 edid_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_96_prt0              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_97_prt0              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_98_prt0              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_99_prt0              : 8 ;  // 31:24     
} M14B0_EDID_18_T;

/*-----------------------------------------------------------------------------
		0x36c edid_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_100_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_101_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_102_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_103_prt0             : 8 ;  // 31:24     
} M14B0_EDID_19_T;

/*-----------------------------------------------------------------------------
		0x370 edid_1a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_104_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_105_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_106_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_107_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1A_T;

/*-----------------------------------------------------------------------------
		0x374 edid_1b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_108_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_109_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_110_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_111_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1B_T;

/*-----------------------------------------------------------------------------
		0x378 edid_1c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_112_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_113_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_114_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_115_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1C_T;

/*-----------------------------------------------------------------------------
		0x37c edid_1d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_116_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_117_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_118_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_119_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1D_T;

/*-----------------------------------------------------------------------------
		0x380 edid_1e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_120_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_121_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_122_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_123_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1E_T;

/*-----------------------------------------------------------------------------
		0x384 edid_1f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_124_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_125_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_126_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_127_prt0             : 8 ;  // 31:24     
} M14B0_EDID_1F_T;

/*-----------------------------------------------------------------------------
		0x388 edid_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_128_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_129_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_130_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_131_prt0             : 8 ;  // 31:24     
} M14B0_EDID_20_T;

/*-----------------------------------------------------------------------------
		0x38c edid_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_132_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_133_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_134_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_135_prt0             : 8 ;  // 31:24     
} M14B0_EDID_21_T;

/*-----------------------------------------------------------------------------
		0x390 edid_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_136_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_137_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_138_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_139_prt0             : 8 ;  // 31:24     
} M14B0_EDID_22_T;

/*-----------------------------------------------------------------------------
		0x394 edid_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_140_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_141_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_142_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_143_prt0             : 8 ;  // 31:24     
} M14B0_EDID_23_T;

/*-----------------------------------------------------------------------------
		0x398 edid_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_144_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_145_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_146_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_147_prt0             : 8 ;  // 31:24     
} M14B0_EDID_24_T;

/*-----------------------------------------------------------------------------
		0x39c edid_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_148_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_149_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_150_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_151_prt0             : 8 ;  // 31:24     
} M14B0_EDID_25_T;

/*-----------------------------------------------------------------------------
		0x3a0 edid_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_152_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_153_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_154_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_155_prt0             : 8 ;  // 31:24     
} M14B0_EDID_26_T;

/*-----------------------------------------------------------------------------
		0x3a4 edid_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_156_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_157_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_158_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_159_prt0             : 8 ;  // 31:24     
} M14B0_EDID_27_T;

/*-----------------------------------------------------------------------------
		0x3a8 edid_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_160_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_161_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_162_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_163_prt0             : 8 ;  // 31:24     
} M14B0_EDID_28_T;

/*-----------------------------------------------------------------------------
		0x3ac edid_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_164_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_165_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_166_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_167_prt0             : 8 ;  // 31:24     
} M14B0_EDID_29_T;

/*-----------------------------------------------------------------------------
		0x3b0 edid_2a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_168_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_169_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_170_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_171_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2A_T;

/*-----------------------------------------------------------------------------
		0x3b4 edid_2b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_172_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_173_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_174_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_175_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2B_T;

/*-----------------------------------------------------------------------------
		0x3b8 edid_2c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_176_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_177_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_178_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_179_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2C_T;

/*-----------------------------------------------------------------------------
		0x3bc edid_2d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_180_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_181_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_182_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_183_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2D_T;

/*-----------------------------------------------------------------------------
		0x3c0 edid_2e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_184_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_185_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_186_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_187_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2E_T;

/*-----------------------------------------------------------------------------
		0x3c4 edid_2f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_188_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_189_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_190_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_191_prt0             : 8 ;  // 31:24     
} M14B0_EDID_2F_T;

/*-----------------------------------------------------------------------------
		0x3c8 edid_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_192_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_193_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_194_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_195_prt0             : 8 ;  // 31:24     
} M14B0_EDID_30_T;

/*-----------------------------------------------------------------------------
		0x3cc edid_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_196_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_197_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_198_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_199_prt0             : 8 ;  // 31:24     
} M14B0_EDID_31_T;

/*-----------------------------------------------------------------------------
		0x3d0 edid_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_200_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_201_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_202_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_203_prt0             : 8 ;  // 31:24     
} M14B0_EDID_32_T;

/*-----------------------------------------------------------------------------
		0x3d4 edid_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_204_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_205_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_206_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_207_prt0             : 8 ;  // 31:24     
} M14B0_EDID_33_T;

/*-----------------------------------------------------------------------------
		0x3d8 edid_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_208_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_209_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_210_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_211_prt0             : 8 ;  // 31:24     
} M14B0_EDID_34_T;

/*-----------------------------------------------------------------------------
		0x3dc edid_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_212_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_213_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_214_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_215_prt0             : 8 ;  // 31:24     
} M14B0_EDID_35_T;

/*-----------------------------------------------------------------------------
		0x3e0 edid_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_216_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_217_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_218_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_219_prt0             : 8 ;  // 31:24     
} M14B0_EDID_36_T;

/*-----------------------------------------------------------------------------
		0x3e4 edid_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_220_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_221_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_222_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_223_prt0             : 8 ;  // 31:24     
} M14B0_EDID_37_T;

/*-----------------------------------------------------------------------------
		0x3e8 edid_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_224_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_225_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_226_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_227_prt0             : 8 ;  // 31:24     
} M14B0_EDID_38_T;

/*-----------------------------------------------------------------------------
		0x3ec edid_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_228_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_229_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_230_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_231_prt0             : 8 ;  // 31:24     
} M14B0_EDID_39_T;

/*-----------------------------------------------------------------------------
		0x3f0 edid_3a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_232_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_233_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_234_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_235_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3A_T;

/*-----------------------------------------------------------------------------
		0x3f4 edid_3b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_236_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_237_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_238_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_239_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3B_T;

/*-----------------------------------------------------------------------------
		0x3f8 edid_3c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_240_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_241_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_242_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_243_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3C_T;

/*-----------------------------------------------------------------------------
		0x3fc edid_3d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_244_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_245_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_246_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_247_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3D_T;

/*-----------------------------------------------------------------------------
		0x400 edid_3e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_248_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_249_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_250_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_251_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3E_T;

/*-----------------------------------------------------------------------------
		0x404 edid_3f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_252_prt0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_253_prt0             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_254_prt0             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_255_prt0             : 8 ;  // 31:24     
} M14B0_EDID_3F_T;

/*-----------------------------------------------------------------------------
		0x408 edid_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_0_prt1               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_1_prt1               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_2_prt1               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_3_prt1               : 8 ;  // 31:24     
} M14B0_EDID_40_T;

/*-----------------------------------------------------------------------------
		0x40c edid_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_4_prt1               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_5_prt1               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_6_prt1               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_7_prt1               : 8 ;  // 31:24     
} M14B0_EDID_41_T;

/*-----------------------------------------------------------------------------
		0x410 edid_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_8_prt1               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_9_prt1               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_10_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_11_prt1              : 8 ;  // 31:24     
} M14B0_EDID_42_T;

/*-----------------------------------------------------------------------------
		0x414 edid_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_12_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_13_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_14_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_15_prt1              : 8 ;  // 31:24     
} M14B0_EDID_43_T;

/*-----------------------------------------------------------------------------
		0x418 edid_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_16_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_17_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_18_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_19_prt1              : 8 ;  // 31:24     
} M14B0_EDID_44_T;

/*-----------------------------------------------------------------------------
		0x41c edid_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_20_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_21_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_22_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_23_prt1              : 8 ;  // 31:24     
} M14B0_EDID_45_T;

/*-----------------------------------------------------------------------------
		0x420 edid_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_24_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_25_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_26_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_27_prt1              : 8 ;  // 31:24     
} M14B0_EDID_46_T;

/*-----------------------------------------------------------------------------
		0x424 edid_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_28_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_29_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_30_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_31_prt1              : 8 ;  // 31:24     
} M14B0_EDID_47_T;

/*-----------------------------------------------------------------------------
		0x428 edid_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_32_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_33_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_34_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_35_prt1              : 8 ;  // 31:24     
} M14B0_EDID_48_T;

/*-----------------------------------------------------------------------------
		0x42c edid_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_36_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_37_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_38_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_39_prt1              : 8 ;  // 31:24     
} M14B0_EDID_49_T;

/*-----------------------------------------------------------------------------
		0x430 edid_4a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_40_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_41_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_42_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_43_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4A_T;

/*-----------------------------------------------------------------------------
		0x434 edid_4b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_44_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_45_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_46_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_47_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4B_T;

/*-----------------------------------------------------------------------------
		0x438 edid_4c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_48_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_49_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_50_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_51_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4C_T;

/*-----------------------------------------------------------------------------
		0x43c edid_4d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_52_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_53_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_54_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_55_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4D_T;

/*-----------------------------------------------------------------------------
		0x440 edid_4e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_56_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_57_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_58_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_59_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4E_T;

/*-----------------------------------------------------------------------------
		0x444 edid_4f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_60_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_61_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_62_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_63_prt1              : 8 ;  // 31:24     
} M14B0_EDID_4F_T;

/*-----------------------------------------------------------------------------
		0x448 edid_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_64_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_65_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_66_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_67_prt1              : 8 ;  // 31:24     
} M14B0_EDID_50_T;

/*-----------------------------------------------------------------------------
		0x44c edid_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_68_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_69_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_70_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_71_prt1              : 8 ;  // 31:24     
} M14B0_EDID_51_T;

/*-----------------------------------------------------------------------------
		0x450 edid_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_72_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_73_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_74_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_75_prt1              : 8 ;  // 31:24     
} M14B0_EDID_52_T;

/*-----------------------------------------------------------------------------
		0x454 edid_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_76_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_77_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_78_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_79_prt1              : 8 ;  // 31:24     
} M14B0_EDID_53_T;

/*-----------------------------------------------------------------------------
		0x458 edid_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_80_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_81_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_82_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_83_prt1              : 8 ;  // 31:24     
} M14B0_EDID_54_T;

/*-----------------------------------------------------------------------------
		0x45c edid_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_84_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_85_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_86_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_87_prt1              : 8 ;  // 31:24     
} M14B0_EDID_55_T;

/*-----------------------------------------------------------------------------
		0x460 edid_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_88_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_89_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_90_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_91_prt1              : 8 ;  // 31:24     
} M14B0_EDID_56_T;

/*-----------------------------------------------------------------------------
		0x464 edid_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_92_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_93_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_94_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_95_prt1              : 8 ;  // 31:24     
} M14B0_EDID_57_T;

/*-----------------------------------------------------------------------------
		0x468 edid_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_96_prt1              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_97_prt1              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_98_prt1              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_99_prt1              : 8 ;  // 31:24     
} M14B0_EDID_58_T;

/*-----------------------------------------------------------------------------
		0x46c edid_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_100_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_101_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_102_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_103_prt1             : 8 ;  // 31:24     
} M14B0_EDID_59_T;

/*-----------------------------------------------------------------------------
		0x470 edid_5a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_104_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_105_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_106_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_107_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5A_T;

/*-----------------------------------------------------------------------------
		0x474 edid_5b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_108_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_109_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_110_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_111_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5B_T;

/*-----------------------------------------------------------------------------
		0x478 edid_5c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_112_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_113_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_114_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_115_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5C_T;

/*-----------------------------------------------------------------------------
		0x47c edid_5d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_116_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_117_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_118_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_119_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5D_T;

/*-----------------------------------------------------------------------------
		0x480 edid_5e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_120_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_121_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_122_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_123_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5E_T;

/*-----------------------------------------------------------------------------
		0x484 edid_5f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_124_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_125_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_126_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_127_prt1             : 8 ;  // 31:24     
} M14B0_EDID_5F_T;

/*-----------------------------------------------------------------------------
		0x488 edid_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_128_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_129_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_130_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_131_prt1             : 8 ;  // 31:24     
} M14B0_EDID_60_T;

/*-----------------------------------------------------------------------------
		0x48c edid_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_132_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_133_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_134_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_135_prt1             : 8 ;  // 31:24     
} M14B0_EDID_61_T;

/*-----------------------------------------------------------------------------
		0x490 edid_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_136_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_137_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_138_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_139_prt1             : 8 ;  // 31:24     
} M14B0_EDID_62_T;

/*-----------------------------------------------------------------------------
		0x494 edid_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_140_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_141_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_142_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_143_prt1             : 8 ;  // 31:24     
} M14B0_EDID_63_T;

/*-----------------------------------------------------------------------------
		0x498 edid_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_144_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_145_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_146_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_147_prt1             : 8 ;  // 31:24     
} M14B0_EDID_64_T;

/*-----------------------------------------------------------------------------
		0x49c edid_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_148_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_149_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_150_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_151_prt1             : 8 ;  // 31:24     
} M14B0_EDID_65_T;

/*-----------------------------------------------------------------------------
		0x4a0 edid_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_152_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_153_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_154_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_155_prt1             : 8 ;  // 31:24     
} M14B0_EDID_66_T;

/*-----------------------------------------------------------------------------
		0x4a4 edid_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_156_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_157_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_158_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_159_prt1             : 8 ;  // 31:24     
} M14B0_EDID_67_T;

/*-----------------------------------------------------------------------------
		0x4a8 edid_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_160_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_161_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_162_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_163_prt1             : 8 ;  // 31:24     
} M14B0_EDID_68_T;

/*-----------------------------------------------------------------------------
		0x4ac edid_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_164_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_165_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_166_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_167_prt1             : 8 ;  // 31:24     
} M14B0_EDID_69_T;

/*-----------------------------------------------------------------------------
		0x4b0 edid_6a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_168_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_169_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_170_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_171_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6A_T;

/*-----------------------------------------------------------------------------
		0x4b4 edid_6b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_172_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_173_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_174_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_175_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6B_T;

/*-----------------------------------------------------------------------------
		0x4b8 edid_6c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_176_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_177_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_178_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_179_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6C_T;

/*-----------------------------------------------------------------------------
		0x4bc edid_6d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_180_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_181_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_182_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_183_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6D_T;

/*-----------------------------------------------------------------------------
		0x4c0 edid_6e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_184_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_185_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_186_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_187_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6E_T;

/*-----------------------------------------------------------------------------
		0x4c4 edid_6f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_188_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_189_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_190_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_191_prt1             : 8 ;  // 31:24     
} M14B0_EDID_6F_T;

/*-----------------------------------------------------------------------------
		0x4c8 edid_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_192_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_193_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_194_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_195_prt1             : 8 ;  // 31:24     
} M14B0_EDID_70_T;

/*-----------------------------------------------------------------------------
		0x4cc edid_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_196_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_197_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_198_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_199_prt1             : 8 ;  // 31:24     
} M14B0_EDID_71_T;

/*-----------------------------------------------------------------------------
		0x4d0 edid_72 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_200_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_201_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_202_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_203_prt1             : 8 ;  // 31:24     
} M14B0_EDID_72_T;

/*-----------------------------------------------------------------------------
		0x4d4 edid_73 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_204_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_205_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_206_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_207_prt1             : 8 ;  // 31:24     
} M14B0_EDID_73_T;

/*-----------------------------------------------------------------------------
		0x4d8 edid_74 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_208_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_209_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_210_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_211_prt1             : 8 ;  // 31:24     
} M14B0_EDID_74_T;

/*-----------------------------------------------------------------------------
		0x4dc edid_75 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_212_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_213_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_214_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_215_prt1             : 8 ;  // 31:24     
} M14B0_EDID_75_T;

/*-----------------------------------------------------------------------------
		0x4e0 edid_76 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_216_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_217_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_218_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_219_prt1             : 8 ;  // 31:24     
} M14B0_EDID_76_T;

/*-----------------------------------------------------------------------------
		0x4e4 edid_77 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_220_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_221_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_222_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_223_prt1             : 8 ;  // 31:24     
} M14B0_EDID_77_T;

/*-----------------------------------------------------------------------------
		0x4e8 edid_78 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_224_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_225_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_226_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_227_prt1             : 8 ;  // 31:24     
} M14B0_EDID_78_T;

/*-----------------------------------------------------------------------------
		0x4ec edid_79 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_228_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_229_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_230_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_231_prt1             : 8 ;  // 31:24     
} M14B0_EDID_79_T;

/*-----------------------------------------------------------------------------
		0x4f0 edid_7a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_232_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_233_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_234_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_235_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7A_T;

/*-----------------------------------------------------------------------------
		0x4f4 edid_7b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_236_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_237_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_238_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_239_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7B_T;

/*-----------------------------------------------------------------------------
		0x4f8 edid_7c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_240_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_241_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_242_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_243_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7C_T;

/*-----------------------------------------------------------------------------
		0x4fc edid_7d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_244_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_245_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_246_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_247_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7D_T;

/*-----------------------------------------------------------------------------
		0x500 edid_7e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_248_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_249_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_250_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_251_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7E_T;

/*-----------------------------------------------------------------------------
		0x504 edid_7f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_252_prt1             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_253_prt1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_254_prt1             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_255_prt1             : 8 ;  // 31:24     
} M14B0_EDID_7F_T;

/*-----------------------------------------------------------------------------
		0x508 edid_80 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_0_prt2               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_1_prt2               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_2_prt2               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_3_prt2               : 8 ;  // 31:24     
} M14B0_EDID_80_T;

/*-----------------------------------------------------------------------------
		0x50c edid_81 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_4_prt2               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_5_prt2               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_6_prt2               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_7_prt2               : 8 ;  // 31:24     
} M14B0_EDID_81_T;

/*-----------------------------------------------------------------------------
		0x510 edid_82 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_8_prt2               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_9_prt2               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_10_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_11_prt2              : 8 ;  // 31:24     
} M14B0_EDID_82_T;

/*-----------------------------------------------------------------------------
		0x514 edid_83 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_12_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_13_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_14_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_15_prt2              : 8 ;  // 31:24     
} M14B0_EDID_83_T;

/*-----------------------------------------------------------------------------
		0x518 edid_84 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_16_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_17_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_18_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_19_prt2              : 8 ;  // 31:24     
} M14B0_EDID_84_T;

/*-----------------------------------------------------------------------------
		0x51c edid_85 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_20_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_21_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_22_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_23_prt2              : 8 ;  // 31:24     
} M14B0_EDID_85_T;

/*-----------------------------------------------------------------------------
		0x520 edid_86 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_24_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_25_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_26_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_27_prt2              : 8 ;  // 31:24     
} M14B0_EDID_86_T;

/*-----------------------------------------------------------------------------
		0x524 edid_87 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_28_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_29_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_30_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_31_prt2              : 8 ;  // 31:24     
} M14B0_EDID_87_T;

/*-----------------------------------------------------------------------------
		0x528 edid_88 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_32_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_33_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_34_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_35_prt2              : 8 ;  // 31:24     
} M14B0_EDID_88_T;

/*-----------------------------------------------------------------------------
		0x52c edid_89 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_36_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_37_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_38_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_39_prt2              : 8 ;  // 31:24     
} M14B0_EDID_89_T;

/*-----------------------------------------------------------------------------
		0x530 edid_8a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_40_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_41_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_42_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_43_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8A_T;

/*-----------------------------------------------------------------------------
		0x534 edid_8b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_44_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_45_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_46_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_47_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8B_T;

/*-----------------------------------------------------------------------------
		0x538 edid_8c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_48_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_49_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_50_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_51_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8C_T;

/*-----------------------------------------------------------------------------
		0x53c edid_8d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_52_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_53_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_54_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_55_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8D_T;

/*-----------------------------------------------------------------------------
		0x540 edid_8e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_56_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_57_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_58_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_59_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8E_T;

/*-----------------------------------------------------------------------------
		0x544 edid_8f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_60_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_61_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_62_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_63_prt2              : 8 ;  // 31:24     
} M14B0_EDID_8F_T;

/*-----------------------------------------------------------------------------
		0x548 edid_90 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_64_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_65_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_66_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_67_prt2              : 8 ;  // 31:24     
} M14B0_EDID_90_T;

/*-----------------------------------------------------------------------------
		0x54c edid_91 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_68_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_69_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_70_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_71_prt2              : 8 ;  // 31:24     
} M14B0_EDID_91_T;

/*-----------------------------------------------------------------------------
		0x550 edid_92 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_72_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_73_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_74_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_75_prt2              : 8 ;  // 31:24     
} M14B0_EDID_92_T;

/*-----------------------------------------------------------------------------
		0x554 edid_93 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_76_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_77_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_78_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_79_prt2              : 8 ;  // 31:24     
} M14B0_EDID_93_T;

/*-----------------------------------------------------------------------------
		0x558 edid_94 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_80_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_81_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_82_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_83_prt2              : 8 ;  // 31:24     
} M14B0_EDID_94_T;

/*-----------------------------------------------------------------------------
		0x55c edid_95 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_84_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_85_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_86_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_87_prt2              : 8 ;  // 31:24     
} M14B0_EDID_95_T;

/*-----------------------------------------------------------------------------
		0x560 edid_96 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_88_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_89_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_90_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_91_prt2              : 8 ;  // 31:24     
} M14B0_EDID_96_T;

/*-----------------------------------------------------------------------------
		0x564 edid_97 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_92_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_93_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_94_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_95_prt2              : 8 ;  // 31:24     
} M14B0_EDID_97_T;

/*-----------------------------------------------------------------------------
		0x568 edid_98 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_96_prt2              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_97_prt2              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_98_prt2              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_99_prt2              : 8 ;  // 31:24     
} M14B0_EDID_98_T;

/*-----------------------------------------------------------------------------
		0x56c edid_99 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_100_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_101_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_102_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_103_prt2             : 8 ;  // 31:24     
} M14B0_EDID_99_T;

/*-----------------------------------------------------------------------------
		0x570 edid_9a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_104_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_105_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_106_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_107_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9A_T;

/*-----------------------------------------------------------------------------
		0x574 edid_9b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_108_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_109_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_110_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_111_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9B_T;

/*-----------------------------------------------------------------------------
		0x578 edid_9c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_112_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_113_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_114_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_115_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9C_T;

/*-----------------------------------------------------------------------------
		0x57c edid_9d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_116_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_117_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_118_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_119_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9D_T;

/*-----------------------------------------------------------------------------
		0x580 edid_9e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_120_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_121_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_122_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_123_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9E_T;

/*-----------------------------------------------------------------------------
		0x584 edid_9f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_124_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_125_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_126_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_127_prt2             : 8 ;  // 31:24     
} M14B0_EDID_9F_T;

/*-----------------------------------------------------------------------------
		0x588 edid_100 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_128_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_129_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_130_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_131_prt2             : 8 ;  // 31:24     
} M14B0_EDID_100_T;

/*-----------------------------------------------------------------------------
		0x58c edid_101 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_132_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_133_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_134_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_135_prt2             : 8 ;  // 31:24     
} M14B0_EDID_101_T;

/*-----------------------------------------------------------------------------
		0x590 edid_102 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_136_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_137_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_138_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_139_prt2             : 8 ;  // 31:24     
} M14B0_EDID_102_T;

/*-----------------------------------------------------------------------------
		0x594 edid_103 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_140_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_141_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_142_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_143_prt2             : 8 ;  // 31:24     
} M14B0_EDID_103_T;

/*-----------------------------------------------------------------------------
		0x598 edid_104 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_144_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_145_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_146_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_147_prt2             : 8 ;  // 31:24     
} M14B0_EDID_104_T;

/*-----------------------------------------------------------------------------
		0x59c edid_105 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_148_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_149_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_150_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_151_prt2             : 8 ;  // 31:24     
} M14B0_EDID_105_T;

/*-----------------------------------------------------------------------------
		0x5a0 edid_106 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_152_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_153_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_154_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_155_prt2             : 8 ;  // 31:24     
} M14B0_EDID_106_T;

/*-----------------------------------------------------------------------------
		0x5a4 edid_107 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_156_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_157_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_158_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_159_prt2             : 8 ;  // 31:24     
} M14B0_EDID_107_T;

/*-----------------------------------------------------------------------------
		0x5a8 edid_108 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_160_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_161_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_162_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_163_prt2             : 8 ;  // 31:24     
} M14B0_EDID_108_T;

/*-----------------------------------------------------------------------------
		0x5ac edid_109 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_164_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_165_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_166_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_167_prt2             : 8 ;  // 31:24     
} M14B0_EDID_109_T;

/*-----------------------------------------------------------------------------
		0x5b0 edid_10a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_168_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_169_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_170_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_171_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10A_T;

/*-----------------------------------------------------------------------------
		0x5b4 edid_10b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_172_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_173_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_174_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_175_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10B_T;

/*-----------------------------------------------------------------------------
		0x5b8 edid_10c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_176_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_177_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_178_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_179_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10C_T;

/*-----------------------------------------------------------------------------
		0x5bc edid_10d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_180_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_181_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_182_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_183_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10D_T;

/*-----------------------------------------------------------------------------
		0x5c0 edid_10e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_184_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_185_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_186_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_187_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10E_T;

/*-----------------------------------------------------------------------------
		0x5c4 edid_10f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_188_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_189_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_190_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_191_prt2             : 8 ;  // 31:24     
} M14B0_EDID_10F_T;

/*-----------------------------------------------------------------------------
		0x5c8 edid_110 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_192_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_193_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_194_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_195_prt2             : 8 ;  // 31:24     
} M14B0_EDID_110_T;

/*-----------------------------------------------------------------------------
		0x5cc edid_111 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_196_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_197_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_198_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_199_prt2             : 8 ;  // 31:24     
} M14B0_EDID_111_T;

/*-----------------------------------------------------------------------------
		0x5d0 edid_112 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_200_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_201_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_202_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_203_prt2             : 8 ;  // 31:24     
} M14B0_EDID_112_T;

/*-----------------------------------------------------------------------------
		0x5d4 edid_113 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_204_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_205_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_206_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_207_prt2             : 8 ;  // 31:24     
} M14B0_EDID_113_T;

/*-----------------------------------------------------------------------------
		0x5d8 edid_114 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_208_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_209_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_210_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_211_prt2             : 8 ;  // 31:24     
} M14B0_EDID_114_T;

/*-----------------------------------------------------------------------------
		0x5dc edid_115 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_212_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_213_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_214_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_215_prt2             : 8 ;  // 31:24     
} M14B0_EDID_115_T;

/*-----------------------------------------------------------------------------
		0x5e0 edid_116 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_216_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_217_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_218_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_219_prt2             : 8 ;  // 31:24     
} M14B0_EDID_116_T;

/*-----------------------------------------------------------------------------
		0x5e4 edid_117 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_220_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_221_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_222_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_223_prt2             : 8 ;  // 31:24     
} M14B0_EDID_117_T;

/*-----------------------------------------------------------------------------
		0x5e8 edid_118 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_224_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_225_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_226_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_227_prt2             : 8 ;  // 31:24     
} M14B0_EDID_118_T;

/*-----------------------------------------------------------------------------
		0x5ec edid_119 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_228_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_229_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_230_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_231_prt2             : 8 ;  // 31:24     
} M14B0_EDID_119_T;

/*-----------------------------------------------------------------------------
		0x5f0 edid_11a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_232_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_233_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_234_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_235_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11A_T;

/*-----------------------------------------------------------------------------
		0x5f4 edid_11b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_236_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_237_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_238_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_239_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11B_T;

/*-----------------------------------------------------------------------------
		0x5f8 edid_11c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_240_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_241_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_242_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_243_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11C_T;

/*-----------------------------------------------------------------------------
		0x5fc edid_11d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_244_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_245_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_246_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_247_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11D_T;

/*-----------------------------------------------------------------------------
		0x600 edid_11e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_248_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_249_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_250_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_251_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11E_T;

/*-----------------------------------------------------------------------------
		0x604 edid_11f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_252_prt2             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_253_prt2             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_254_prt2             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_255_prt2             : 8 ;  // 31:24     
} M14B0_EDID_11F_T;

/*-----------------------------------------------------------------------------
		0x608 edid_120 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_0_prt3               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_1_prt3               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_2_prt3               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_3_prt3               : 8 ;  // 31:24     
} M14B0_EDID_120_T;

/*-----------------------------------------------------------------------------
		0x60c edid_121 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_4_prt3               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_5_prt3               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_6_prt3               : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_7_prt3               : 8 ;  // 31:24     
} M14B0_EDID_121_T;

/*-----------------------------------------------------------------------------
		0x610 edid_122 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_8_prt3               : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_9_prt3               : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_10_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_11_prt3              : 8 ;  // 31:24     
} M14B0_EDID_122_T;

/*-----------------------------------------------------------------------------
		0x614 edid_123 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_12_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_13_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_14_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_15_prt3              : 8 ;  // 31:24     
} M14B0_EDID_123_T;

/*-----------------------------------------------------------------------------
		0x618 edid_124 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_16_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_17_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_18_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_19_prt3              : 8 ;  // 31:24     
} M14B0_EDID_124_T;

/*-----------------------------------------------------------------------------
		0x61c edid_125 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_20_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_21_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_22_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_23_prt3              : 8 ;  // 31:24     
} M14B0_EDID_125_T;

/*-----------------------------------------------------------------------------
		0x620 edid_126 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_24_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_25_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_26_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_27_prt3              : 8 ;  // 31:24     
} M14B0_EDID_126_T;

/*-----------------------------------------------------------------------------
		0x624 edid_127 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_28_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_29_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_30_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_31_prt3              : 8 ;  // 31:24     
} M14B0_EDID_127_T;

/*-----------------------------------------------------------------------------
		0x628 edid_128 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_32_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_33_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_34_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_35_prt3              : 8 ;  // 31:24     
} M14B0_EDID_128_T;

/*-----------------------------------------------------------------------------
		0x62c edid_129 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_36_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_37_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_38_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_39_prt3              : 8 ;  // 31:24     
} M14B0_EDID_129_T;

/*-----------------------------------------------------------------------------
		0x630 edid_12a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_40_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_41_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_42_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_43_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12A_T;

/*-----------------------------------------------------------------------------
		0x634 edid_12b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_44_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_45_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_46_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_47_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12B_T;

/*-----------------------------------------------------------------------------
		0x638 edid_12c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_48_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_49_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_50_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_51_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12C_T;

/*-----------------------------------------------------------------------------
		0x63c edid_12d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_52_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_53_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_54_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_55_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12D_T;

/*-----------------------------------------------------------------------------
		0x640 edid_12e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_56_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_57_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_58_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_59_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12E_T;

/*-----------------------------------------------------------------------------
		0x644 edid_12f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_60_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_61_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_62_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_63_prt3              : 8 ;  // 31:24     
} M14B0_EDID_12F_T;

/*-----------------------------------------------------------------------------
		0x648 edid_130 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_64_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_65_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_66_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_67_prt3              : 8 ;  // 31:24     
} M14B0_EDID_130_T;

/*-----------------------------------------------------------------------------
		0x64c edid_131 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_68_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_69_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_70_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_71_prt3              : 8 ;  // 31:24     
} M14B0_EDID_131_T;

/*-----------------------------------------------------------------------------
		0x650 edid_132 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_72_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_73_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_74_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_75_prt3              : 8 ;  // 31:24     
} M14B0_EDID_132_T;

/*-----------------------------------------------------------------------------
		0x654 edid_133 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_76_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_77_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_78_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_79_prt3              : 8 ;  // 31:24     
} M14B0_EDID_133_T;

/*-----------------------------------------------------------------------------
		0x658 edid_134 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_80_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_81_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_82_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_83_prt3              : 8 ;  // 31:24     
} M14B0_EDID_134_T;

/*-----------------------------------------------------------------------------
		0x65c edid_135 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_84_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_85_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_86_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_87_prt3              : 8 ;  // 31:24     
} M14B0_EDID_135_T;

/*-----------------------------------------------------------------------------
		0x660 edid_136 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_88_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_89_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_90_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_91_prt3              : 8 ;  // 31:24     
} M14B0_EDID_136_T;

/*-----------------------------------------------------------------------------
		0x664 edid_137 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_92_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_93_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_94_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_95_prt3              : 8 ;  // 31:24     
} M14B0_EDID_137_T;

/*-----------------------------------------------------------------------------
		0x668 edid_138 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_96_prt3              : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_97_prt3              : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_98_prt3              : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_99_prt3              : 8 ;  // 31:24     
} M14B0_EDID_138_T;

/*-----------------------------------------------------------------------------
		0x66c edid_139 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_100_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_101_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_102_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_103_prt3             : 8 ;  // 31:24     
} M14B0_EDID_139_T;

/*-----------------------------------------------------------------------------
		0x670 edid_13a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_104_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_105_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_106_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_107_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13A_T;

/*-----------------------------------------------------------------------------
		0x674 edid_13b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_108_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_109_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_110_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_111_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13B_T;

/*-----------------------------------------------------------------------------
		0x678 edid_13c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_112_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_113_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_114_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_115_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13C_T;

/*-----------------------------------------------------------------------------
		0x67c edid_13d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_116_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_117_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_118_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_119_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13D_T;

/*-----------------------------------------------------------------------------
		0x680 edid_13e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_120_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_121_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_122_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_123_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13E_T;

/*-----------------------------------------------------------------------------
		0x684 edid_13f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_124_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_125_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_126_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_127_prt3             : 8 ;  // 31:24     
} M14B0_EDID_13F_T;

/*-----------------------------------------------------------------------------
		0x688 edid_140 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_128_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_129_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_130_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_131_prt3             : 8 ;  // 31:24     
} M14B0_EDID_140_T;

/*-----------------------------------------------------------------------------
		0x68c edid_141 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_132_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_133_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_134_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_135_prt3             : 8 ;  // 31:24     
} M14B0_EDID_141_T;

/*-----------------------------------------------------------------------------
		0x690 edid_142 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_136_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_137_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_138_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_139_prt3             : 8 ;  // 31:24     
} M14B0_EDID_142_T;

/*-----------------------------------------------------------------------------
		0x694 edid_143 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_140_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_141_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_142_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_143_prt3             : 8 ;  // 31:24     
} M14B0_EDID_143_T;

/*-----------------------------------------------------------------------------
		0x698 edid_144 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_144_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_145_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_146_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_147_prt3             : 8 ;  // 31:24     
} M14B0_EDID_144_T;

/*-----------------------------------------------------------------------------
		0x69c edid_145 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_148_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_149_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_150_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_151_prt3             : 8 ;  // 31:24     
} M14B0_EDID_145_T;

/*-----------------------------------------------------------------------------
		0x6a0 edid_146 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_152_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_153_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_154_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_155_prt3             : 8 ;  // 31:24     
} M14B0_EDID_146_T;

/*-----------------------------------------------------------------------------
		0x6a4 edid_147 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_156_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_157_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_158_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_159_prt3             : 8 ;  // 31:24     
} M14B0_EDID_147_T;

/*-----------------------------------------------------------------------------
		0x6a8 edid_148 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_160_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_161_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_162_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_163_prt3             : 8 ;  // 31:24     
} M14B0_EDID_148_T;

/*-----------------------------------------------------------------------------
		0x6ac edid_149 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_164_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_165_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_166_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_167_prt3             : 8 ;  // 31:24     
} M14B0_EDID_149_T;

/*-----------------------------------------------------------------------------
		0x6b0 edid_14a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_168_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_169_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_170_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_171_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14A_T;

/*-----------------------------------------------------------------------------
		0x6b4 edid_14b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_172_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_173_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_174_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_175_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14B_T;

/*-----------------------------------------------------------------------------
		0x6b8 edid_14c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_176_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_177_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_178_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_179_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14C_T;

/*-----------------------------------------------------------------------------
		0x6bc edid_14d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_180_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_181_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_182_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_183_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14D_T;

/*-----------------------------------------------------------------------------
		0x6c0 edid_14e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_184_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_185_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_186_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_187_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14E_T;

/*-----------------------------------------------------------------------------
		0x6c4 edid_14f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_188_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_189_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_190_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_191_prt3             : 8 ;  // 31:24     
} M14B0_EDID_14F_T;

/*-----------------------------------------------------------------------------
		0x6c8 edid_150 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_192_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_193_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_194_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_195_prt3             : 8 ;  // 31:24     
} M14B0_EDID_150_T;

/*-----------------------------------------------------------------------------
		0x6cc edid_151 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_196_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_197_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_198_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_199_prt3             : 8 ;  // 31:24     
} M14B0_EDID_151_T;

/*-----------------------------------------------------------------------------
		0x6d0 edid_152 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_200_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_201_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_202_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_203_prt3             : 8 ;  // 31:24     
} M14B0_EDID_152_T;

/*-----------------------------------------------------------------------------
		0x6d4 edid_153 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_204_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_205_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_206_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_207_prt3             : 8 ;  // 31:24     
} M14B0_EDID_153_T;

/*-----------------------------------------------------------------------------
		0x6d8 edid_154 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_208_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_209_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_210_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_211_prt3             : 8 ;  // 31:24     
} M14B0_EDID_154_T;

/*-----------------------------------------------------------------------------
		0x6dc edid_155 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_212_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_213_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_214_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_215_prt3             : 8 ;  // 31:24     
} M14B0_EDID_155_T;

/*-----------------------------------------------------------------------------
		0x6e0 edid_156 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_216_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_217_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_218_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_219_prt3             : 8 ;  // 31:24     
} M14B0_EDID_156_T;

/*-----------------------------------------------------------------------------
		0x6e4 edid_157 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_220_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_221_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_222_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_223_prt3             : 8 ;  // 31:24     
} M14B0_EDID_157_T;

/*-----------------------------------------------------------------------------
		0x6e8 edid_158 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_224_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_225_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_226_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_227_prt3             : 8 ;  // 31:24     
} M14B0_EDID_158_T;

/*-----------------------------------------------------------------------------
		0x6ec edid_159 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_228_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_229_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_230_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_231_prt3             : 8 ;  // 31:24     
} M14B0_EDID_159_T;

/*-----------------------------------------------------------------------------
		0x6f0 edid_15a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_232_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_233_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_234_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_235_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15A_T;

/*-----------------------------------------------------------------------------
		0x6f4 edid_15b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_236_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_237_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_238_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_239_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15B_T;

/*-----------------------------------------------------------------------------
		0x6f8 edid_15c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_240_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_241_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_242_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_243_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15C_T;

/*-----------------------------------------------------------------------------
		0x6fc edid_15d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_244_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_245_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_246_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_247_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15D_T;

/*-----------------------------------------------------------------------------
		0x700 edid_15e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_248_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_249_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_250_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_251_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15E_T;

/*-----------------------------------------------------------------------------
		0x704 edid_15f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_252_prt3             : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_253_prt3             : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_254_prt3             : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_255_prt3             : 8 ;  // 31:24     
} M14B0_EDID_15F_T;

/*-----------------------------------------------------------------------------
		0x708 hdcp_key_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_2             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_3             : 8 ;  // 31:24     
} M14B0_HDCP_KEY_00_T;

/*-----------------------------------------------------------------------------
		0x70c hdcp_key_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_4             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_5             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_6             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_7             : 8 ;  // 31:24     
} M14B0_HDCP_KEY_01_T;

/*-----------------------------------------------------------------------------
		0x710 hdcp_key_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_8             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_9             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_10            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_11            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_02_T;

/*-----------------------------------------------------------------------------
		0x714 hdcp_key_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_12            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_13            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_14            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_15            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_03_T;

/*-----------------------------------------------------------------------------
		0x718 hdcp_key_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_16            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_17            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_18            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_19            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_04_T;

/*-----------------------------------------------------------------------------
		0x71c hdcp_key_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_20            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_21            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_22            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_23            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_05_T;

/*-----------------------------------------------------------------------------
		0x720 hdcp_key_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_24            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_25            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_26            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_27            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_06_T;

/*-----------------------------------------------------------------------------
		0x724 hdcp_key_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_28            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_29            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_30            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_31            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_07_T;

/*-----------------------------------------------------------------------------
		0x728 hdcp_key_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_32            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_33            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_34            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_35            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_08_T;

/*-----------------------------------------------------------------------------
		0x72c hdcp_key_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_36            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_37            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_38            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_39            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_09_T;

/*-----------------------------------------------------------------------------
		0x730 hdcp_key_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_40            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_41            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_42            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_43            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_0A_T;

/*-----------------------------------------------------------------------------
		0x734 hdcp_key_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_44            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_45            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_46            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_47            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_0B_T;

/*-----------------------------------------------------------------------------
		0x738 hdcp_key_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_48            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_49            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_50            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_51            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_0C_T;

/*-----------------------------------------------------------------------------
		0x73c hdcp_key_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_52            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_53            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_54            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_55            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_0D_T;

/*-----------------------------------------------------------------------------
		0x740 hdcp_key_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_56            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_57            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_58            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_59            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_0F_T;

/*-----------------------------------------------------------------------------
		0x744 hdcp_key_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_60            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_61            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_62            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_63            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_10_T;

/*-----------------------------------------------------------------------------
		0x748 hdcp_key_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_64            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_65            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_66            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_67            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_11_T;

/*-----------------------------------------------------------------------------
		0x74c hdcp_key_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_68            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_69            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_70            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_71            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_12_T;

/*-----------------------------------------------------------------------------
		0x750 hdcp_key_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_72            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_73            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_74            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_75            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_13_T;

/*-----------------------------------------------------------------------------
		0x754 hdcp_key_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_76            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_77            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_78            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_79            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_14_T;

/*-----------------------------------------------------------------------------
		0x758 hdcp_key_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_80            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_81            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_82            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_83            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_15_T;

/*-----------------------------------------------------------------------------
		0x75c hdcp_key_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_84            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_85            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_86            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_87            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_16_T;

/*-----------------------------------------------------------------------------
		0x760 hdcp_key_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_88            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_89            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_90            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_91            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_17_T;

/*-----------------------------------------------------------------------------
		0x764 hdcp_key_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_92            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_93            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_94            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_95            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_18_T;

/*-----------------------------------------------------------------------------
		0x768 hdcp_key_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_96            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_97            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_98            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_99            : 8 ;  // 31:24     
} M14B0_HDCP_KEY_19_T;

/*-----------------------------------------------------------------------------
		0x76c hdcp_key_1a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_100           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_101           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_102           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_103           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1A_T;

/*-----------------------------------------------------------------------------
		0x770 hdcp_key_1b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_104           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_105           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_106           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_107           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1B_T;

/*-----------------------------------------------------------------------------
		0x774 hdcp_key_1c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_108           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_109           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_110           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_111           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1C_T;

/*-----------------------------------------------------------------------------
		0x778 hdcp_key_1d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_112           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_113           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_114           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_115           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1D_T;

/*-----------------------------------------------------------------------------
		0x77c hdcp_key_1e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_116           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_117           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_118           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_119           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1E_T;

/*-----------------------------------------------------------------------------
		0x780 hdcp_key_1f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_120           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_121           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_122           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_123           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_1F_T;

/*-----------------------------------------------------------------------------
		0x784 hdcp_key_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_124           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_125           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_126           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_127           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_20_T;

/*-----------------------------------------------------------------------------
		0x788 hdcp_key_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_128           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_129           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_130           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_131           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_21_T;

/*-----------------------------------------------------------------------------
		0x78c hdcp_key_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_132           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_133           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_134           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_135           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_22_T;

/*-----------------------------------------------------------------------------
		0x790 hdcp_key_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_136           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_137           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_138           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_139           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_23_T;

/*-----------------------------------------------------------------------------
		0x794 hdcp_key_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_140           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_141           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_142           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_143           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_24_T;

/*-----------------------------------------------------------------------------
		0x798 hdcp_key_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_144           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_145           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_146           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_147           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_25_T;

/*-----------------------------------------------------------------------------
		0x79c hdcp_key_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_148           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_149           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_150           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_151           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_26_T;

/*-----------------------------------------------------------------------------
		0x7a0 hdcp_key_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_152           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_153           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_154           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_155           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_27_T;

/*-----------------------------------------------------------------------------
		0x7a4 hdcp_key_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_156           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_157           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_158           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_159           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_28_T;

/*-----------------------------------------------------------------------------
		0x7a8 hdcp_key_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_160           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_161           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_162           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_163           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_29_T;

/*-----------------------------------------------------------------------------
		0x7ac hdcp_key_2a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_164           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_165           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_166           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_167           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2A_T;

/*-----------------------------------------------------------------------------
		0x7b0 hdcp_key_2b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_168           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_169           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_170           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_171           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2B_T;

/*-----------------------------------------------------------------------------
		0x7b4 hdcp_key_2c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_172           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_173           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_174           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_175           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2C_T;

/*-----------------------------------------------------------------------------
		0x7b8 hdcp_key_2d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_176           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_177           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_178           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_179           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2D_T;

/*-----------------------------------------------------------------------------
		0x7bc hdcp_key_2e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_180           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_181           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_182           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_183           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2E_T;

/*-----------------------------------------------------------------------------
		0x7c0 hdcp_key_2f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_184           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_185           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_186           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_187           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_2F_T;

/*-----------------------------------------------------------------------------
		0x7c4 hdcp_key_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_188           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_189           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_190           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_191           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_30_T;

/*-----------------------------------------------------------------------------
		0x7c8 hdcp_key_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_192           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_193           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_194           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_195           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_31_T;

/*-----------------------------------------------------------------------------
		0x7cc hdcp_key_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_196           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_197           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_198           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_199           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_32_T;

/*-----------------------------------------------------------------------------
		0x7d0 hdcp_key_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_200           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_201           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_202           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_203           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_33_T;

/*-----------------------------------------------------------------------------
		0x7d4 hdcp_key_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_204           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_205           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_206           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_207           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_34_T;

/*-----------------------------------------------------------------------------
		0x7d8 hdcp_key_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_208           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_209           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_210           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_211           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_35_T;

/*-----------------------------------------------------------------------------
		0x7dc hdcp_key_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_212           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_213           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_214           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_215           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_36_T;

/*-----------------------------------------------------------------------------
		0x7e0 hdcp_key_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_216           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_217           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_218           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_219           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_37_T;

/*-----------------------------------------------------------------------------
		0x7e4 hdcp_key_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_220           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_221           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_222           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_223           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_38_T;

/*-----------------------------------------------------------------------------
		0x7e8 hdcp_key_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_224           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_225           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_226           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_227           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_39_T;

/*-----------------------------------------------------------------------------
		0x7ec hdcp_key_3a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_228           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_229           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_230           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_231           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3A_T;

/*-----------------------------------------------------------------------------
		0x7f0 hdcp_key_3b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_232           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_233           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_234           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_235           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3B_T;

/*-----------------------------------------------------------------------------
		0x7f4 hdcp_key_3c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_236           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_237           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_238           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_239           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3C_T;

/*-----------------------------------------------------------------------------
		0x7f8 hdcp_key_3d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_240           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_241           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_242           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_243           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3D_T;

/*-----------------------------------------------------------------------------
		0x7fc hdcp_key_3e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_244           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_245           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_246           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_247           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3E_T;

/*-----------------------------------------------------------------------------
		0x800 hdcp_key_3f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_248           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_249           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_250           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_251           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_3F_T;

/*-----------------------------------------------------------------------------
		0x804 hdcp_key_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_252           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_253           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_254           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_255           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_40_T;

/*-----------------------------------------------------------------------------
		0x808 hdcp_key_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_256           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_257           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_258           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_259           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_41_T;

/*-----------------------------------------------------------------------------
		0x80c hdcp_key_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_260           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_261           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_262           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_263           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_42_T;

/*-----------------------------------------------------------------------------
		0x810 hdcp_key_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_264           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_265           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_266           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_267           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_43_T;

/*-----------------------------------------------------------------------------
		0x814 hdcp_key_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_268           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_269           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_270           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_271           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_44_T;

/*-----------------------------------------------------------------------------
		0x818 hdcp_key_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_272           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_273           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_274           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_275           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_45_T;

/*-----------------------------------------------------------------------------
		0x81c hdcp_key_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_276           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_277           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_278           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_279           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_46_T;

/*-----------------------------------------------------------------------------
		0x820 hdcp_key_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_280           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_281           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_282           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_283           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_47_T;

/*-----------------------------------------------------------------------------
		0x824 hdcp_key_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_284           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_285           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_286           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_287           : 8 ;  // 31:24     
} M14B0_HDCP_KEY_48_T;
typedef struct {
	M14B0_PHY_LINK_00_T                	phy_link_00;                  //0x0
	M14B0_PHY_LINK_01_T                	phy_link_01;                  //0x4
	M14B0_PHY_LINK_02_T                	phy_link_02;                  //0x8
	M14B0_PHY_LINK_03_T                	phy_link_03;                  //0xc
	M14B0_PHY_LINK_04_T                	phy_link_04;                  //0x10
	M14B0_PHY_LINK_05_T                	phy_link_05;                  //0x14
	M14B0_PHY_LINK_06_T                	phy_link_06;                  //0x18
	M14B0_PHY_LINK_07_T                	phy_link_07;                  //0x1c
	M14B0_INTERRUPT_ENABLE_00_T        	interrupt_enable_00;          //0x20
	M14B0_INTERRUPT_ENABLE_01_T        	interrupt_enable_01;          //0x24
	M14B0_INTERRUPT_ENABLE_02_T        	interrupt_enable_02;          //0x28
	M14B0_INTERRUPT_ENABLE_03_T        	interrupt_enable_03;          //0x2c
	M14B0_INTERRUPT_ENABLE_04_T        	interrupt_enable_04;          //0x30
	M14B0_INTERRUPT_00_T               	interrupt_00;                 //0x34
	M14B0_INTERRUPT_01_T               	interrupt_01;                 //0x38
	M14B0_INTERRUPT_02_T               	interrupt_02;                 //0x3c
	M14B0_INTERRUPT_03_T               	interrupt_03;                 //0x40
	M14B0_INTERRUPT_04_T               	interrupt_04;                 //0x44
	M14B0_SYSTEM_CONTROL_00_T          	system_control_00;            //0x48
	M14B0_SYSTEM_CONTROL_01_T          	system_control_01;            //0x4c
	M14B0_SYSTEM_CONTROL_02_T          	system_control_02;            //0x50
	M14B0_HDCP_00_T                    	hdcp_00;                      //0x54
	M14B0_HDCP_01_T                    	hdcp_01;                      //0x58
	M14B0_HDCP_02_T                    	hdcp_02;                      //0x5c
	M14B0_HDCP_03_T                    	hdcp_03;                      //0x60
	M14B0_HDCP_04_T                    	hdcp_04;                      //0x64
	M14B0_HDCP_05_T                    	hdcp_05;                      //0x68
	M14B0_HDCP_06_T                    	hdcp_06;                      //0x6c
	M14B0_HDCP_07_T                    	hdcp_07;                      //0x70
	M14B0_HDCP_08_T                    	hdcp_08;                      //0x74
	M14B0_HDCP_09_T                    	hdcp_09;                      //0x78
	M14B0_HDCP_0A_T                    	hdcp_10;                      //0x7c
	M14B0_HDCP_0B_T                    	hdcp_11;                      //0x80
	M14B0_HDCP_0C_T                    	hdcp_12;                      //0x84
	M14B0_HDCP_0D_T                    	hdcp_13;                      //0x88
	M14B0_HDCP_0E_T                    	hdcp_14;                      //0x8c
	M14B0_HDCP_0F_T                    	hdcp_15;                      //0x90
	M14B0_HDCP_10_T                    	hdcp_16;                      //0x94
	M14B0_HDCP_11_T                    	hdcp_17;                      //0x98
	M14B0_HDCP_12_T                    	hdcp_18;                      //0x9c
	M14B0_HDCP_13_T                    	hdcp_19;                      //0xa0
	M14B0_HDCP_14_T                    	hdcp_20;                      //0xa4
	M14B0_HDCP_15_T                    	hdcp_21;                      //0xa8
	M14B0_HDCP_16_T                    	hdcp_22;                      //0xac
	M14B0_HDCP_17_T                    	hdcp_23;                      //0xb0
	M14B0_PACKET_00_T                  	packet_00;                    //0xb4
	M14B0_PACKET_01_T                  	packet_01;                    //0xb8
	M14B0_PACKET_02_T                  	packet_02;                    //0xbc
	M14B0_PACKET_03_T                  	packet_03;                    //0xc0
	M14B0_PACKET_04_T                  	packet_04;                    //0xc4
	M14B0_PACKET_05_T                  	packet_05;                    //0xc8
	M14B0_PACKET_06_T                  	packet_06;                    //0xcc
	M14B0_PACKET_07_T                  	packet_07;                    //0xd0
	M14B0_PACKET_08_T                  	packet_08;                    //0xd4
	M14B0_PACKET_09_T                  	packet_09;                    //0xd8
	M14B0_PACKET_0A_T                  	packet_10;                    //0xdc
	M14B0_PACKET_0B_T                  	packet_11;                    //0xe0
	M14B0_PACKET_0C_T                  	packet_12;                    //0xe4
	M14B0_PACKET_0D_T                  	packet_13;                    //0xe8
	M14B0_PACKET_0E_T                  	packet_14;                    //0xec
	M14B0_PACKET_0F_T                  	packet_15;                    //0xf0
	M14B0_PACKET_10_T                  	packet_16;                    //0xf4
	M14B0_PACKET_11_T                  	packet_17;                    //0xf8
	M14B0_PACKET_12_T                  	packet_18;                    //0xfc
	M14B0_PACKET_13_T                  	packet_19;                    //0x100
	M14B0_PACKET_14_T                  	packet_20;                    //0x104
	M14B0_PACKET_15_T                  	packet_21;                    //0x108
	M14B0_PACKET_16_T                  	packet_22;                    //0x10c
	M14B0_PACKET_17_T                  	packet_23;                    //0x110
	M14B0_PACKET_18_T                  	packet_24;                    //0x114
	M14B0_PACKET_19_T                  	packet_25;                    //0x118
	M14B0_PACKET_1A_T                  	packet_26;                    //0x11c
	M14B0_PACKET_1B_T                  	packet_27;                    //0x120
	M14B0_PACKET_1C_T                  	packet_28;                    //0x124
	M14B0_PACKET_1D_T                  	packet_29;                    //0x128
	M14B0_PACKET_1E_T                  	packet_30;                    //0x12c
	M14B0_PACKET_1F_T                  	packet_31;                    //0x130
	M14B0_PACKET_20_T                  	packet_32;                    //0x134
	M14B0_PACKET_21_T                  	packet_33;                    //0x138
	M14B0_PACKET_22_T                  	packet_34;                    //0x13c
	M14B0_PACKET_23_T                  	packet_35;                    //0x140
	M14B0_PACKET_24_T                  	packet_36;                    //0x144
	M14B0_PACKET_25_T                  	packet_37;                    //0x148
	M14B0_PACKET_26_T                  	packet_38;                    //0x14c
	M14B0_PACKET_27_T                  	packet_39;                    //0x150
	M14B0_PACKET_28_T                  	packet_40;                    //0x154
	M14B0_PACKET_29_T                  	packet_41;                    //0x158
	M14B0_PACKET_2A_T                  	packet_42;                    //0x15c
	M14B0_PACKET_2B_T                  	packet_43;                    //0x160
	M14B0_PACKET_2C_T                  	packet_44;                    //0x164
	M14B0_PACKET_2D_T                  	packet_45;                    //0x168
	M14B0_PACKET_2E_T                  	packet_46;                    //0x16c
	M14B0_PACKET_2F_T                  	packet_47;                    //0x170
	M14B0_PACKET_30_T                  	packet_48;                    //0x174
	M14B0_PACKET_31_T                  	packet_49;                    //0x178
	M14B0_PACKET_32_T                  	packet_50;                    //0x17c
	M14B0_PACKET_33_T                  	packet_51;                    //0x180
	M14B0_PACKET_34_T                  	packet_52;                    //0x184
	M14B0_PACKET_35_T                  	packet_53;                    //0x188
	M14B0_PACKET_36_T                  	packet_54;                    //0x18c
	M14B0_PACKET_37_T                  	packet_55;                    //0x190
	M14B0_PACKET_38_T                  	packet_56;                    //0x194
	M14B0_PACKET_39_T                  	packet_57;                    //0x198
	M14B0_PACKET_3A_T                  	packet_58;                    //0x19c
	M14B0_PACKET_3B_T                  	packet_59;                    //0x1a0
	M14B0_PACKET_3C_T                  	packet_60;                    //0x1a4
	M14B0_PACKET_3D_T                  	packet_61;                    //0x1a8
	M14B0_PACKET_3E_T                  	packet_62;                    //0x1ac
	M14B0_PACKET_3F_T                  	packet_63;                    //0x1b0
	M14B0_PACKET_40_T                  	packet_64;                    //0x1b4
	M14B0_PACKET_41_T                  	packet_65;                    //0x1b8
	M14B0_PACKET_42_T                  	packet_66;                    //0x1bc
	M14B0_PACKET_43_T                  	packet_67;                    //0x1c0
	M14B0_PACKET_44_T                  	packet_68;                    //0x1c4
	M14B0_PACKET_45_T                  	packet_69;                    //0x1c8
	M14B0_PACKET_46_T                  	packet_70;                    //0x1cc
	M14B0_PACKET_47_T                  	packet_71;                    //0x1d0
	M14B0_PACKET_48_T                  	packet_72;                    //0x1d4
	M14B0_PACKET_49_T                  	packet_73;                    //0x1d8
	M14B0_PACKET_4A_T                  	packet_74;                    //0x1dc
	M14B0_PACKET_4B_T                  	packet_75;                    //0x1e0
	M14B0_PACKET_4C_T                  	packet_76;                    //0x1e4
	M14B0_PACKET_4D_T                  	packet_77;                    //0x1e8
	M14B0_PACKET_4E_T                  	packet_78;                    //0x1ec
	M14B0_PACKET_4F_T                  	packet_79;                    //0x1f0
	M14B0_PACKET_50_T                  	packet_80;                    //0x1f4
	M14B0_PACKET_51_T                  	packet_81;                    //0x1f8
	M14B0_PACKET_52_T                  	packet_82;                    //0x1fc
	M14B0_VIDEO_00_T                   	video_00;                     //0x200
	M14B0_VIDEO_01_T                   	video_01;                     //0x204
	M14B0_VIDEO_02_T                   	video_02;                     //0x208
	M14B0_VIDEO_03_T                   	video_03;                     //0x20c
	M14B0_VIDEO_04_T                   	video_04;                     //0x210
	M14B0_VIDEO_05_T                   	video_05;                     //0x214
	M14B0_VIDEO_06_T                   	video_06;                     //0x218
	M14B0_VIDEO_07_T                   	video_07;                     //0x21c
	M14B0_VIDEO_08_T                   	video_08;                     //0x220
	M14B0_VIDEO_09_T                   	video_09;                     //0x224
	M14B0_VIDEO_0A_T                   	video_10;                     //0x228
	M14B0_AUDIO_00_T                   	audio_00;                     //0x22c
	M14B0_AUDIO_01_T                   	audio_01;                     //0x230
	M14B0_AUDIO_02_T                   	audio_02;                     //0x234
	M14B0_AUDIO_03_T                   	audio_03;                     //0x238
	M14B0_AUDIO_04_T                   	audio_04;                     //0x23c
	M14B0_AUDIO_05_T                   	audio_05;                     //0x240
	M14B0_AUDIO_06_T                   	audio_06;                     //0x244
	M14B0_AUDIO_07_T                   	audio_07;                     //0x248
	M14B0_AUDIO_08_T                   	audio_08;                     //0x24c
	M14B0_AUDIO_09_T                   	audio_09;                     //0x250
	M14B0_AUDIO_0A_T                   	audio_10;                     //0x254
	M14B0_EDID_HEAC_00_T               	edid_heac_00;                 //0x258
	M14B0_CBUS_00_T                    	cbus_00;                      //0x25c
	M14B0_CBUS_01_T                    	cbus_01;                      //0x260
	M14B0_CBUS_02_T                    	cbus_02;                      //0x264
	M14B0_CBUS_03_T                    	cbus_03;                      //0x268
	M14B0_CBUS_04_T                    	cbus_04;                      //0x26c
	M14B0_CBUS_05_T                    	cbus_05;                      //0x270
	M14B0_CBUS_06_T                    	cbus_06;                      //0x274
	M14B0_CBUS_07_T                    	cbus_07;                      //0x278
	M14B0_CBUS_08_T                    	cbus_08;                      //0x27c
	M14B0_CBUS_09_T                    	cbus_09;                      //0x280
	M14B0_CBUS_0A_T                    	cbus_10;                      //0x284
	M14B0_CBUS_0B_T                    	cbus_11;                      //0x288
	M14B0_CBUS_0C_T                    	cbus_12;                      //0x28c
	M14B0_CBUS_0D_T                    	cbus_13;                      //0x290
	M14B0_CBUS_0E_T                    	cbus_14;                      //0x294
	M14B0_CBUS_0F_T                    	cbus_15;                      //0x298
	M14B0_CBUS_10_T                    	cbus_16;                      //0x29c
	M14B0_CBUS_11_T                    	cbus_17;                      //0x2a0
	M14B0_CBUS_12_T                    	cbus_18;                      //0x2a4
	M14B0_CBUS_13_T                    	cbus_19;                      //0x2a8
	M14B0_CBUS_14_T                    	cbus_20;                      //0x2ac
	M14B0_CBUS_15_T                    	cbus_21;                      //0x2b0
	M14B0_CBUS_16_T                    	cbus_22;                      //0x2b4
	M14B0_CBUS_17_T                    	cbus_23;                      //0x2b8
	M14B0_CBUS_18_T                    	cbus_24;                      //0x2bc
	M14B0_CBUS_19_T                    	cbus_25;                      //0x2c0
	M14B0_CBUS_1A_T                    	cbus_26;                      //0x2c4
	M14B0_CBUS_1B_T                    	cbus_27;                      //0x2c8
	M14B0_CBUS_1C_T                    	cbus_28;                      //0x2cc
	M14B0_CBUS_1D_T                    	cbus_29;                      //0x2d0
	M14B0_CBUS_1E_T                    	cbus_30;                      //0x2d4
	M14B0_CBUS_1F_T                    	cbus_31;                      //0x2d8
	M14B0_CBUS_20_T                    	cbus_32;                      //0x2dc
	M14B0_CBUS_21_T                    	cbus_33;                      //0x2e0
	M14B0_CBUS_22_T                    	cbus_34;                      //0x2e4
	M14B0_CBUS_23_T                    	cbus_35;                      //0x2e8
	M14B0_CBUS_24_T                    	cbus_36;                      //0x2ec
	M14B0_CBUS_25_T                    	cbus_37;                      //0x2f0
	M14B0_CBUS_26_T                    	cbus_38;                      //0x2f4
	M14B0_CBUS_27_T                    	cbus_39;                      //0x2f8
	M14B0_CBUS_28_T                    	cbus_40;                      //0x2fc
	M14B0_CBUS_29_T                    	cbus_41;                      //0x300
	M14B0_CBUS_2A_T                    	cbus_42;                      //0x304
	M14B0_EDID_00_T                    	edid_00;                      //0x308
	M14B0_EDID_01_T                    	edid_01;                      //0x30c
	M14B0_EDID_02_T                    	edid_02;                      //0x310
	M14B0_EDID_03_T                    	edid_03;                      //0x314
	M14B0_EDID_04_T                    	edid_04;                      //0x318
	M14B0_EDID_05_T                    	edid_05;                      //0x31c
	M14B0_EDID_06_T                    	edid_06;                      //0x320
	M14B0_EDID_07_T                    	edid_07;                      //0x324
	M14B0_EDID_08_T                    	edid_08;                      //0x328
	M14B0_EDID_09_T                    	edid_09;                      //0x32c
	M14B0_EDID_0A_T                    	edid_10;                      //0x330
	M14B0_EDID_0B_T                    	edid_11;                      //0x334
	M14B0_EDID_0C_T                    	edid_12;                      //0x338
	M14B0_EDID_0D_T                    	edid_13;                      //0x33c
	M14B0_EDID_0E_T                    	edid_14;                      //0x340
	M14B0_EDID_0F_T                    	edid_15;                      //0x344
	M14B0_EDID_10_T                    	edid_16;                      //0x348
	M14B0_EDID_11_T                    	edid_17;                      //0x34c
	M14B0_EDID_12_T                    	edid_18;                      //0x350
	M14B0_EDID_13_T                    	edid_19;                      //0x354
	M14B0_EDID_14_T                    	edid_20;                      //0x358
	M14B0_EDID_15_T                    	edid_21;                      //0x35c
	M14B0_EDID_16_T                    	edid_22;                      //0x360
	M14B0_EDID_17_T                    	edid_23;                      //0x364
	M14B0_EDID_18_T                    	edid_24;                      //0x368
	M14B0_EDID_19_T                    	edid_25;                      //0x36c
	M14B0_EDID_1A_T                    	edid_26;                      //0x370
	M14B0_EDID_1B_T                    	edid_27;                      //0x374
	M14B0_EDID_1C_T                    	edid_28;                      //0x378
	M14B0_EDID_1D_T                    	edid_29;                      //0x37c
	M14B0_EDID_1E_T                    	edid_30;                      //0x380
	M14B0_EDID_1F_T                    	edid_31;                      //0x384
	M14B0_EDID_20_T                    	edid_32;                      //0x388
	M14B0_EDID_21_T                    	edid_33;                      //0x38c
	M14B0_EDID_22_T                    	edid_34;                      //0x390
	M14B0_EDID_23_T                    	edid_35;                      //0x394
	M14B0_EDID_24_T                    	edid_36;                      //0x398
	M14B0_EDID_25_T                    	edid_37;                      //0x39c
	M14B0_EDID_26_T                    	edid_38;                      //0x3a0
	M14B0_EDID_27_T                    	edid_39;                      //0x3a4
	M14B0_EDID_28_T                    	edid_40;                      //0x3a8
	M14B0_EDID_29_T                    	edid_41;                      //0x3ac
	M14B0_EDID_2A_T                    	edid_42;                      //0x3b0
	M14B0_EDID_2B_T                    	edid_43;                      //0x3b4
	M14B0_EDID_2C_T                    	edid_44;                      //0x3b8
	M14B0_EDID_2D_T                    	edid_45;                      //0x3bc
	M14B0_EDID_2E_T                    	edid_46;                      //0x3c0
	M14B0_EDID_2F_T                    	edid_47;                      //0x3c4
	M14B0_EDID_30_T                    	edid_48;                      //0x3c8
	M14B0_EDID_31_T                    	edid_49;                      //0x3cc
	M14B0_EDID_32_T                    	edid_50;                      //0x3d0
	M14B0_EDID_33_T                    	edid_51;                      //0x3d4
	M14B0_EDID_34_T                    	edid_52;                      //0x3d8
	M14B0_EDID_35_T                    	edid_53;                      //0x3dc
	M14B0_EDID_36_T                    	edid_54;                      //0x3e0
	M14B0_EDID_37_T                    	edid_55;                      //0x3e4
	M14B0_EDID_38_T                    	edid_56;                      //0x3e8
	M14B0_EDID_39_T                    	edid_57;                      //0x3ec
	M14B0_EDID_3A_T                    	edid_58;                      //0x3f0
	M14B0_EDID_3B_T                    	edid_59;                      //0x3f4
	M14B0_EDID_3C_T                    	edid_60;                      //0x3f8
	M14B0_EDID_3D_T                    	edid_61;                      //0x3fc
	M14B0_EDID_3E_T                    	edid_62;                      //0x400
	M14B0_EDID_3F_T                    	edid_63;                      //0x404
	M14B0_EDID_40_T                    	edid_64;                      //0x408
	M14B0_EDID_41_T                    	edid_65;                      //0x40c
	M14B0_EDID_42_T                    	edid_66;                      //0x410
	M14B0_EDID_43_T                    	edid_67;                      //0x414
	M14B0_EDID_44_T                    	edid_68;                      //0x418
	M14B0_EDID_45_T                    	edid_69;                      //0x41c
	M14B0_EDID_46_T                    	edid_70;                      //0x420
	M14B0_EDID_47_T                    	edid_71;                      //0x424
	M14B0_EDID_48_T                    	edid_72;                      //0x428
	M14B0_EDID_49_T                    	edid_73;                      //0x42c
	M14B0_EDID_4A_T                    	edid_74;                      //0x430
	M14B0_EDID_4B_T                    	edid_75;                      //0x434
	M14B0_EDID_4C_T                    	edid_76;                      //0x438
	M14B0_EDID_4D_T                    	edid_77;                      //0x43c
	M14B0_EDID_4E_T                    	edid_78;                      //0x440
	M14B0_EDID_4F_T                    	edid_79;                      //0x444
	M14B0_EDID_50_T                    	edid_80;                      //0x448
	M14B0_EDID_51_T                    	edid_81;                      //0x44c
	M14B0_EDID_52_T                    	edid_82;                      //0x450
	M14B0_EDID_53_T                    	edid_83;                      //0x454
	M14B0_EDID_54_T                    	edid_84;                      //0x458
	M14B0_EDID_55_T                    	edid_85;                      //0x45c
	M14B0_EDID_56_T                    	edid_86;                      //0x460
	M14B0_EDID_57_T                    	edid_87;                      //0x464
	M14B0_EDID_58_T                    	edid_88;                      //0x468
	M14B0_EDID_59_T                    	edid_89;                      //0x46c
	M14B0_EDID_5A_T                    	edid_90;                      //0x470
	M14B0_EDID_5B_T                    	edid_91;                      //0x474
	M14B0_EDID_5C_T                    	edid_92;                      //0x478
	M14B0_EDID_5D_T                    	edid_93;                      //0x47c
	M14B0_EDID_5E_T                    	edid_94;                      //0x480
	M14B0_EDID_5F_T                    	edid_95;                      //0x484
	M14B0_EDID_60_T                    	edid_96;                      //0x488
	M14B0_EDID_61_T                    	edid_97;                      //0x48c
	M14B0_EDID_62_T                    	edid_98;                      //0x490
	M14B0_EDID_63_T                    	edid_99;                      //0x494
	M14B0_EDID_64_T                    	edid_100;                     //0x498
	M14B0_EDID_65_T                    	edid_101;                     //0x49c
	M14B0_EDID_66_T                    	edid_102;                     //0x4a0
	M14B0_EDID_67_T                    	edid_103;                     //0x4a4
	M14B0_EDID_68_T                    	edid_104;                     //0x4a8
	M14B0_EDID_69_T                    	edid_105;                     //0x4ac
	M14B0_EDID_6A_T                    	edid_106;                     //0x4b0
	M14B0_EDID_6B_T                    	edid_107;                     //0x4b4
	M14B0_EDID_6C_T                    	edid_108;                     //0x4b8
	M14B0_EDID_6D_T                    	edid_109;                     //0x4bc
	M14B0_EDID_6E_T                    	edid_110;                     //0x4c0
	M14B0_EDID_6F_T                    	edid_111;                     //0x4c4
	M14B0_EDID_70_T                    	edid_112;                     //0x4c8
	M14B0_EDID_71_T                    	edid_113;                     //0x4cc
	M14B0_EDID_72_T                    	edid_114;                     //0x4d0
	M14B0_EDID_73_T                    	edid_115;                     //0x4d4
	M14B0_EDID_74_T                    	edid_116;                     //0x4d8
	M14B0_EDID_75_T                    	edid_117;                     //0x4dc
	M14B0_EDID_76_T                    	edid_118;                     //0x4e0
	M14B0_EDID_77_T                    	edid_119;                     //0x4e4
	M14B0_EDID_78_T                    	edid_120;                     //0x4e8
	M14B0_EDID_79_T                    	edid_121;                     //0x4ec
	M14B0_EDID_7A_T                    	edid_122;                     //0x4f0
	M14B0_EDID_7B_T                    	edid_123;                     //0x4f4
	M14B0_EDID_7C_T                    	edid_124;                     //0x4f8
	M14B0_EDID_7D_T                    	edid_125;                     //0x4fc
	M14B0_EDID_7E_T                    	edid_126;                     //0x500
	M14B0_EDID_7F_T                    	edid_127;                     //0x504
	M14B0_EDID_80_T                    	edid_128;                     //0x508
	M14B0_EDID_81_T                    	edid_129;                     //0x50c
	M14B0_EDID_82_T                    	edid_130;                     //0x510
	M14B0_EDID_83_T                    	edid_131;                     //0x514
	M14B0_EDID_84_T                    	edid_132;                     //0x518
	M14B0_EDID_85_T                    	edid_133;                     //0x51c
	M14B0_EDID_86_T                    	edid_134;                     //0x520
	M14B0_EDID_87_T                    	edid_135;                     //0x524
	M14B0_EDID_88_T                    	edid_136;                     //0x528
	M14B0_EDID_89_T                    	edid_137;                     //0x52c
	M14B0_EDID_8A_T                    	edid_138;                     //0x530
	M14B0_EDID_8B_T                    	edid_139;                     //0x534
	M14B0_EDID_8C_T                    	edid_140;                     //0x538
	M14B0_EDID_8D_T                    	edid_141;                     //0x53c
	M14B0_EDID_8E_T                    	edid_142;                     //0x540
	M14B0_EDID_8F_T                    	edid_143;                     //0x544
	M14B0_EDID_90_T                    	edid_144;                     //0x548
	M14B0_EDID_91_T                    	edid_145;                     //0x54c
	M14B0_EDID_92_T                    	edid_146;                     //0x550
	M14B0_EDID_93_T                    	edid_147;                     //0x554
	M14B0_EDID_94_T                    	edid_148;                     //0x558
	M14B0_EDID_95_T                    	edid_149;                     //0x55c
	M14B0_EDID_96_T                    	edid_150;                     //0x560
	M14B0_EDID_97_T                    	edid_151;                     //0x564
	M14B0_EDID_98_T                    	edid_152;                     //0x568
	M14B0_EDID_99_T                    	edid_153;                     //0x56c
	M14B0_EDID_9A_T                    	edid_154;                     //0x570
	M14B0_EDID_9B_T                    	edid_155;                     //0x574
	M14B0_EDID_9C_T                    	edid_156;                     //0x578
	M14B0_EDID_9D_T                    	edid_157;                     //0x57c
	M14B0_EDID_9E_T                    	edid_158;                     //0x580
	M14B0_EDID_9F_T                    	edid_159;                     //0x584
	M14B0_EDID_100_T                   	edid_160;                     //0x588
	M14B0_EDID_101_T                   	edid_161;                     //0x58c
	M14B0_EDID_102_T                   	edid_162;                     //0x590
	M14B0_EDID_103_T                   	edid_163;                     //0x594
	M14B0_EDID_104_T                   	edid_164;                     //0x598
	M14B0_EDID_105_T                   	edid_165;                     //0x59c
	M14B0_EDID_106_T                   	edid_166;                     //0x5a0
	M14B0_EDID_107_T                   	edid_167;                     //0x5a4
	M14B0_EDID_108_T                   	edid_168;                     //0x5a8
	M14B0_EDID_109_T                   	edid_169;                     //0x5ac
	M14B0_EDID_10A_T                   	edid_170;                     //0x5b0
	M14B0_EDID_10B_T                   	edid_171;                     //0x5b4
	M14B0_EDID_10C_T                   	edid_172;                     //0x5b8
	M14B0_EDID_10D_T                   	edid_173;                     //0x5bc
	M14B0_EDID_10E_T                   	edid_174;                     //0x5c0
	M14B0_EDID_10F_T                   	edid_175;                     //0x5c4
	M14B0_EDID_110_T                   	edid_176;                     //0x5c8
	M14B0_EDID_111_T                   	edid_177;                     //0x5cc
	M14B0_EDID_112_T                   	edid_178;                     //0x5d0
	M14B0_EDID_113_T                   	edid_179;                     //0x5d4
	M14B0_EDID_114_T                   	edid_180;                     //0x5d8
	M14B0_EDID_115_T                   	edid_181;                     //0x5dc
	M14B0_EDID_116_T                   	edid_182;                     //0x5e0
	M14B0_EDID_117_T                   	edid_183;                     //0x5e4
	M14B0_EDID_118_T                   	edid_184;                     //0x5e8
	M14B0_EDID_119_T                   	edid_185;                     //0x5ec
	M14B0_EDID_11A_T                   	edid_186;                     //0x5f0
	M14B0_EDID_11B_T                   	edid_187;                     //0x5f4
	M14B0_EDID_11C_T                   	edid_188;                     //0x5f8
	M14B0_EDID_11D_T                   	edid_189;                     //0x5fc
	M14B0_EDID_11E_T                   	edid_190;                     //0x600
	M14B0_EDID_11F_T                   	edid_191;                     //0x604
	M14B0_EDID_120_T                   	edid_192;                     //0x608
	M14B0_EDID_121_T                   	edid_193;                     //0x60c
	M14B0_EDID_122_T                   	edid_194;                     //0x610
	M14B0_EDID_123_T                   	edid_195;                     //0x614
	M14B0_EDID_124_T                   	edid_196;                     //0x618
	M14B0_EDID_125_T                   	edid_197;                     //0x61c
	M14B0_EDID_126_T                   	edid_198;                     //0x620
	M14B0_EDID_127_T                   	edid_199;                     //0x624
	M14B0_EDID_128_T                   	edid_200;                     //0x628
	M14B0_EDID_129_T                   	edid_201;                     //0x62c
	M14B0_EDID_12A_T                   	edid_202;                     //0x630
	M14B0_EDID_12B_T                   	edid_203;                     //0x634
	M14B0_EDID_12C_T                   	edid_204;                     //0x638
	M14B0_EDID_12D_T                   	edid_205;                     //0x63c
	M14B0_EDID_12E_T                   	edid_206;                     //0x640
	M14B0_EDID_12F_T                   	edid_207;                     //0x644
	M14B0_EDID_130_T                   	edid_208;                     //0x648
	M14B0_EDID_131_T                   	edid_209;                     //0x64c
	M14B0_EDID_132_T                   	edid_210;                     //0x650
	M14B0_EDID_133_T                   	edid_211;                     //0x654
	M14B0_EDID_134_T                   	edid_212;                     //0x658
	M14B0_EDID_135_T                   	edid_213;                     //0x65c
	M14B0_EDID_136_T                   	edid_214;                     //0x660
	M14B0_EDID_137_T                   	edid_215;                     //0x664
	M14B0_EDID_138_T                   	edid_216;                     //0x668
	M14B0_EDID_139_T                   	edid_217;                     //0x66c
	M14B0_EDID_13A_T                   	edid_218;                     //0x670
	M14B0_EDID_13B_T                   	edid_219;                     //0x674
	M14B0_EDID_13C_T                   	edid_220;                     //0x678
	M14B0_EDID_13D_T                   	edid_221;                     //0x67c
	M14B0_EDID_13E_T                   	edid_222;                     //0x680
	M14B0_EDID_13F_T                   	edid_223;                     //0x684
	M14B0_EDID_140_T                   	edid_224;                     //0x688
	M14B0_EDID_141_T                   	edid_225;                     //0x68c
	M14B0_EDID_142_T                   	edid_226;                     //0x690
	M14B0_EDID_143_T                   	edid_227;                     //0x694
	M14B0_EDID_144_T                   	edid_228;                     //0x698
	M14B0_EDID_145_T                   	edid_229;                     //0x69c
	M14B0_EDID_146_T                   	edid_230;                     //0x6a0
	M14B0_EDID_147_T                   	edid_231;                     //0x6a4
	M14B0_EDID_148_T                   	edid_232;                     //0x6a8
	M14B0_EDID_149_T                   	edid_233;                     //0x6ac
	M14B0_EDID_14A_T                   	edid_234;                     //0x6b0
	M14B0_EDID_14B_T                   	edid_235;                     //0x6b4
	M14B0_EDID_14C_T                   	edid_236;                     //0x6b8
	M14B0_EDID_14D_T                   	edid_237;                     //0x6bc
	M14B0_EDID_14E_T                   	edid_238;                     //0x6c0
	M14B0_EDID_14F_T                   	edid_239;                     //0x6c4
	M14B0_EDID_150_T                   	edid_240;                     //0x6c8
	M14B0_EDID_151_T                   	edid_241;                     //0x6cc
	M14B0_EDID_152_T                   	edid_242;                     //0x6d0
	M14B0_EDID_153_T                   	edid_243;                     //0x6d4
	M14B0_EDID_154_T                   	edid_244;                     //0x6d8
	M14B0_EDID_155_T                   	edid_245;                     //0x6dc
	M14B0_EDID_156_T                   	edid_246;                     //0x6e0
	M14B0_EDID_157_T                   	edid_247;                     //0x6e4
	M14B0_EDID_158_T                   	edid_248;                     //0x6e8
	M14B0_EDID_159_T                   	edid_249;                     //0x6ec
	M14B0_EDID_15A_T                   	edid_250;                     //0x6f0
	M14B0_EDID_15B_T                   	edid_251;                     //0x6f4
	M14B0_EDID_15C_T                   	edid_252;                     //0x6f8
	M14B0_EDID_15D_T                   	edid_253;                     //0x6fc
	M14B0_EDID_15E_T                   	edid_254;                     //0x700
	M14B0_EDID_15F_T                   	edid_255;                     //0x704
	M14B0_HDCP_KEY_00_T                	hdcp_key_00;                  //0x708
	M14B0_HDCP_KEY_01_T                	hdcp_key_01;                  //0x70c
	M14B0_HDCP_KEY_02_T                	hdcp_key_02;                  //0x710
	M14B0_HDCP_KEY_03_T                	hdcp_key_03;                  //0x714
	M14B0_HDCP_KEY_04_T                	hdcp_key_04;                  //0x718
	M14B0_HDCP_KEY_05_T                	hdcp_key_05;                  //0x71c
	M14B0_HDCP_KEY_06_T                	hdcp_key_06;                  //0x720
	M14B0_HDCP_KEY_07_T                	hdcp_key_07;                  //0x724
	M14B0_HDCP_KEY_08_T                	hdcp_key_08;                  //0x728
	M14B0_HDCP_KEY_09_T                	hdcp_key_09;                  //0x72c
	M14B0_HDCP_KEY_0A_T                	hdcp_key_0a;                  //0x730
	M14B0_HDCP_KEY_0B_T                	hdcp_key_0b;                  //0x734
	M14B0_HDCP_KEY_0C_T                	hdcp_key_0c;                  //0x738
	M14B0_HDCP_KEY_0D_T                	hdcp_key_0d;                  //0x73c
	M14B0_HDCP_KEY_0F_T                	hdcp_key_0f;                  //0x740
	M14B0_HDCP_KEY_10_T                	hdcp_key_10;                  //0x744
	M14B0_HDCP_KEY_11_T                	hdcp_key_11;                  //0x748
	M14B0_HDCP_KEY_12_T                	hdcp_key_12;                  //0x74c
	M14B0_HDCP_KEY_13_T                	hdcp_key_13;                  //0x750
	M14B0_HDCP_KEY_14_T                	hdcp_key_14;                  //0x754
	M14B0_HDCP_KEY_15_T                	hdcp_key_15;                  //0x758
	M14B0_HDCP_KEY_16_T                	hdcp_key_16;                  //0x75c
	M14B0_HDCP_KEY_17_T                	hdcp_key_17;                  //0x760
	M14B0_HDCP_KEY_18_T                	hdcp_key_18;                  //0x764
	M14B0_HDCP_KEY_19_T                	hdcp_key_19;                  //0x768
	M14B0_HDCP_KEY_1A_T                	hdcp_key_1a;                  //0x76c
	M14B0_HDCP_KEY_1B_T                	hdcp_key_1b;                  //0x770
	M14B0_HDCP_KEY_1C_T                	hdcp_key_1c;                  //0x774
	M14B0_HDCP_KEY_1D_T                	hdcp_key_1d;                  //0x778
	M14B0_HDCP_KEY_1E_T                	hdcp_key_1e;                  //0x77c
	M14B0_HDCP_KEY_1F_T                	hdcp_key_1f;                  //0x780
	M14B0_HDCP_KEY_20_T                	hdcp_key_20;                  //0x784
	M14B0_HDCP_KEY_21_T                	hdcp_key_21;                  //0x788
	M14B0_HDCP_KEY_22_T                	hdcp_key_22;                  //0x78c
	M14B0_HDCP_KEY_23_T                	hdcp_key_23;                  //0x790
	M14B0_HDCP_KEY_24_T                	hdcp_key_24;                  //0x794
	M14B0_HDCP_KEY_25_T                	hdcp_key_25;                  //0x798
	M14B0_HDCP_KEY_26_T                	hdcp_key_26;                  //0x79c
	M14B0_HDCP_KEY_27_T                	hdcp_key_27;                  //0x7a0
	M14B0_HDCP_KEY_28_T                	hdcp_key_28;                  //0x7a4
	M14B0_HDCP_KEY_29_T                	hdcp_key_29;                  //0x7a8
	M14B0_HDCP_KEY_2A_T                	hdcp_key_2a;                  //0x7ac
	M14B0_HDCP_KEY_2B_T                	hdcp_key_2b;                  //0x7b0
	M14B0_HDCP_KEY_2C_T                	hdcp_key_2c;                  //0x7b4
	M14B0_HDCP_KEY_2D_T                	hdcp_key_2d;                  //0x7b8
	M14B0_HDCP_KEY_2E_T                	hdcp_key_2e;                  //0x7bc
	M14B0_HDCP_KEY_2F_T                	hdcp_key_2f;                  //0x7c0
	M14B0_HDCP_KEY_30_T                	hdcp_key_30;                  //0x7c4
	M14B0_HDCP_KEY_31_T                	hdcp_key_31;                  //0x7c8
	M14B0_HDCP_KEY_32_T                	hdcp_key_32;                  //0x7cc
	M14B0_HDCP_KEY_33_T                	hdcp_key_33;                  //0x7d0
	M14B0_HDCP_KEY_34_T                	hdcp_key_34;                  //0x7d4
	M14B0_HDCP_KEY_35_T                	hdcp_key_35;                  //0x7d8
	M14B0_HDCP_KEY_36_T                	hdcp_key_36;                  //0x7dc
	M14B0_HDCP_KEY_37_T                	hdcp_key_37;                  //0x7e0
	M14B0_HDCP_KEY_38_T                	hdcp_key_38;                  //0x7e4
	M14B0_HDCP_KEY_39_T                	hdcp_key_39;                  //0x7e8
	M14B0_HDCP_KEY_3A_T                	hdcp_key_3a;                  //0x7ec
	M14B0_HDCP_KEY_3B_T                	hdcp_key_3b;                  //0x7f0
	M14B0_HDCP_KEY_3C_T                	hdcp_key_3c;                  //0x7f4
	M14B0_HDCP_KEY_3D_T                	hdcp_key_3d;                  //0x7f8
	M14B0_HDCP_KEY_3E_T                	hdcp_key_3e;                  //0x7fc
	M14B0_HDCP_KEY_3F_T                	hdcp_key_3f;                  //0x800
	M14B0_HDCP_KEY_40_T                	hdcp_key_40;                  //0x804
	M14B0_HDCP_KEY_41_T                	hdcp_key_41;                  //0x808
	M14B0_HDCP_KEY_42_T                	hdcp_key_42;                  //0x80c
	M14B0_HDCP_KEY_43_T                	hdcp_key_43;                  //0x810
	M14B0_HDCP_KEY_44_T                	hdcp_key_44;                  //0x814
	M14B0_HDCP_KEY_45_T                	hdcp_key_45;                  //0x818
	M14B0_HDCP_KEY_46_T                	hdcp_key_46;                  //0x81c
	M14B0_HDCP_KEY_47_T                	hdcp_key_47;                  //0x820
	M14B0_HDCP_KEY_48_T                	hdcp_key_48;                  //0x824
}HDMI_LINK_REG_M14B0_T;
//#endif
