#ifndef _HDMI_RX_LINK_REGISTER_MAP_DW_LINK1_H_
#define _HDMI_RX_LINK_REGISTER_MAP_DW_LINK1_H_
/*-----------------------------------------------------------------------------
		0x400 link1_phy_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_i2c_rstn                       : 1 ;  //     0     
	UINT32 phy_pdb                            : 1 ;  //     1     
	UINT32 phy_i2c_enable                     : 1 ;  //     2     
	UINT32 hpd_out                            : 1 ;  //     3     
	UINT32 phy_eq_odt_sel                     : 2 ;  //  5: 4     
	UINT32 phy_pdb_man_mode                   : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_man_pp_value                   : 1 ;  //     8     
	UINT32 reg_man_pp_mode                    : 1 ;  //     9     
	UINT32 reg_man_mhl_value                  : 1 ;  //    10     
	UINT32 reg_man_mhl_mode                   : 1 ;  //    11     
	UINT32 phy_pp_mode                        : 1 ;  //    12     
	UINT32 phy_mhl_mode                       : 1 ;  //    13     
	UINT32 reg_odt_pdb_man_value              : 1 ;  //    14     
	UINT32 reg_odt_pdb_man_mode               : 1 ;  //    15     
	UINT32 phy_arc_pdb                        : 1 ;  //    16     
	UINT32 phy_arc_odt_sel                    : 3 ;  // 19:17     
	UINT32 hpd_in                             : 1 ;  //    20     
	UINT32 hpd_out_oen                        : 1 ;  //    21     
	UINT32 hpd_1k_en                          : 1 ;  //    22     
	UINT32 hpd_100k_en                        : 1 ;  //    23     
} H15A0_LINK1_PHY_00_T;

/*-----------------------------------------------------------------------------
		0x404 link1_phy_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phy_bert_num_err_0                 : 8 ;  //  7: 0     
	UINT32 phy_bert_num_err_1                 : 8 ;  // 15: 8     
	UINT32 phy_bert_test_res                  : 2 ;  // 17:16     
	UINT32 phy_bert_sync_done                 : 1 ;  //    18     
	UINT32 phy_bert_run_ch2                   : 1 ;  //    19     
	UINT32 phy_bert_run_ch1                   : 1 ;  //    20     
	UINT32 phy_bert_run_ch0                   : 1 ;  //    21     
	UINT32 phy_bert_ch_sel                    : 2 ;  // 23:22     
	UINT32 phy_cbus_hpd_out                   : 1 ;  //    24     
	UINT32 phy_cbus_hpd_out_oen               : 1 ;  //    25     
	UINT32 phy_cbus_hpd_100k_en               : 1 ;  //    26     
	UINT32 phy_cbus_hpd_1k_en                 : 1 ;  //    27     
	UINT32 phy_cbus_hpd_in                    : 1 ;  //    28     
} H15A0_LINK1_PHY_01_T;

/*-----------------------------------------------------------------------------
		0x408 link1_interrupt_enable_00 ''
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
} H15A0_LINK1_INTERRUPT_ENABLE_00_T;

/*-----------------------------------------------------------------------------
		0x40c link1_interrupt_enable_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_cmd_msc_err_enable       : 1 ;  //     0     
	UINT32 intr_plug_out_enable               : 1 ;  //     1     
	UINT32 intr_plug_in_enable                : 1 ;  //     2     
	UINT32 intr_hdmi5v_fall_enable            : 1 ;  //     3     
	UINT32 intr_hdmi5v_rise_enable            : 1 ;  //     4     
	UINT32 intr_scdt_fall_enable              : 1 ;  //     5     
	UINT32 intr_scdt_rise_enable              : 1 ;  //     6     
	UINT32 intr_hdmi_mode_enable              : 1 ;  //     7     
	UINT32 intr_set_mute_enable               : 1 ;  //     8     
	UINT32 intr_clr_mute_enable               : 1 ;  //     9     
	UINT32 intr_vsync_enable                  : 1 ;  //    10     
	UINT32 intr_hdmi5v_fedge_enable           : 1 ;  //    11     
	UINT32 intr_hdmi5v_redge_enable           : 1 ;  //    12     
	UINT32 intr_scdt_fedge_enable             : 1 ;  //    13     
	UINT32 intr_scdt_redge_enable             : 1 ;  //    14     
	UINT32 intr_cd_sense_fedge_enable         : 1 ;  //    15     
	UINT32 intr_cd_sense_redge_enable         : 1 ;  //    16     
	UINT32 intr_auth_init_enable              : 1 ;  //    17     
	UINT32 intr_auth_done_enable              : 1 ;  //    18     
	UINT32 intr_hdcp_err_enable               : 1 ;  //    19     
	UINT32 intr_ecc_err_enable                : 1 ;  //    20     
	UINT32 intr_terc4_err_enable              : 1 ;  //    21     
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
} H15A0_LINK1_INTERRUPT_ENABLE_01_T;

/*-----------------------------------------------------------------------------
		0x410 link1_interrupt_enable_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_chg_24mode_enable        : 1 ;  //     0     
	UINT32 intr_cbus_pathen_set_enable        : 1 ;  //     1     
	UINT32 intr_cbus_new_dscr_enable          : 1 ;  //     2     
	UINT32 intr_cbus_new_intstat_enable       : 1 ;  //     3     
	UINT32 intr_cbus_new_dcap_enable          : 1 ;  //     4     
	UINT32 intr_cbus_new_ucp_enable           : 1 ;  //     5     
	UINT32 intr_cbus_cmd_ucp_enable           : 1 ;  //     6     
	UINT32 intr_cbus_pathen_clr_enable        : 1 ;  //     7     
	UINT32 intr_cbus_mute_set_enable          : 1 ;  //     8     
	UINT32 intr_cbus_mute_clr_enable          : 1 ;  //     9     
	UINT32 intr_cbus_new_mscerr_enable        : 1 ;  //    10     
	UINT32 intr_cbus_new_rcp_enable           : 1 ;  //    11     
	UINT32 intr_cbus_cmd_set_hpd_enable       : 1 ;  //    12     
	UINT32 intr_cbus_cmd_get_state_enable     : 1 ;  //    13     
	UINT32 intr_cbus_cmd_get_vdid_enable      : 1 ;  //    14     
	UINT32 intr_cbus_cmd_rd_devcap_enable     : 1 ;  //    15     
	UINT32 intr_cbus_cmd_rd_devintr_enable    : 1 ;  //    16     
	UINT32 intr_cbus_cmd_pathen_set_enable    : 1 ;  //    17     
	UINT32 intr_cbus_cmd_pathen_clr_enable    : 1 ;  //    18     
	UINT32 intr_cbus_cmd_dcaprd_set_enable    : 1 ;  //    19     
	UINT32 intr_cbus_cmd_3dreq_set_enable     : 1 ;  //    20     
	UINT32 intr_cbus_cmd_grtwrt_set_enable    : 1 ;  //    21     
	UINT32 intr_cbus_cmd_reqwrt_set_enable    : 1 ;  //    22     
	UINT32 intr_cbus_cmd_wrt_burst_enable     : 1 ;  //    23     
	UINT32 intr_cbus_cmd_dscrchg_set_enable   : 1 ;  //    24     
	UINT32 intr_cbus_cmd_dcapchg_set_enable   : 1 ;  //    25     
	UINT32 intr_cbus_cmd_edidchg_set_enable   : 1 ;  //    26     
	UINT32 intr_cbus_cmd_clr_hpd_enable       : 1 ;  //    27     
	UINT32 intr_cbus_cmd_rap_poll_enable      : 1 ;  //    28     
	UINT32 intr_cbus_cmd_rap_on_enable        : 1 ;  //    29     
	UINT32 intr_cbus_cmd_rap_off_enable       : 1 ;  //    30     
	UINT32 intr_cbus_cmd_rcp_enable           : 1 ;  //    31     
} H15A0_LINK1_INTERRUPT_ENABLE_02_T;

/*-----------------------------------------------------------------------------
		0x414 link1_interrupt_enable_03 ''
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
	UINT32 intr_cbus_new_rap_enable           : 1 ;  //    11     
	UINT32 intr_chg_avi_to_ycc420_enable      : 1 ;  //    12	: H15B0 Added     
} H15A0_LINK1_INTERRUPT_ENABLE_03_T;

/*-----------------------------------------------------------------------------
		0x418 link1_interrupt_00 ''
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
} H15A0_LINK1_INTERRUPT_00_T;

/*-----------------------------------------------------------------------------
		0x41c link1_interrupt_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_cmd_msc_err              : 1 ;  //     0     
	UINT32 intr_plug_out                      : 1 ;  //     1     
	UINT32 intr_plug_in                       : 1 ;  //     2     
	UINT32 intr_hdmi5v_fall                   : 1 ;  //     3     
	UINT32 intr_hdmi5v_rise                   : 1 ;  //     4     
	UINT32 intr_scdt_fall                     : 1 ;  //     5     
	UINT32 intr_scdt_rise                     : 1 ;  //     6     
	UINT32 intr_hdmi_mode                     : 1 ;  //     7     
	UINT32 intr_set_mute                      : 1 ;  //     8     
	UINT32 intr_clr_mute                      : 1 ;  //     9     
	UINT32 intr_vsync                         : 1 ;  //    10     
	UINT32 intr_hdmi5v_fedge                  : 1 ;  //    11     
	UINT32 intr_hdmi5v_redge                  : 1 ;  //    12     
	UINT32 intr_scdt_fedge                    : 1 ;  //    13     
	UINT32 intr_scdt_redge                    : 1 ;  //    14     
	UINT32 intr_cd_sense_fedge                : 1 ;  //    15     
	UINT32 intr_cd_sense_redge                : 1 ;  //    16     
	UINT32 intr_auth_init                     : 1 ;  //    17     
	UINT32 intr_auth_done                     : 1 ;  //    18     
	UINT32 intr_hdcp_err                      : 1 ;  //    19     
	UINT32 intr_ecc_err                       : 1 ;  //    20     
	UINT32 intr_terc4_err                     : 1 ;  //    21     
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
} H15A0_LINK1_INTERRUPT_01_T;

/*-----------------------------------------------------------------------------
		0x420 link1_interrupt_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_cbus_chg_24mode               : 1 ;  //     0     
	UINT32 intr_cbus_pathen_set               : 1 ;  //     1     
	UINT32 intr_cbus_new_dscr                 : 1 ;  //     2     
	UINT32 intr_cbus_new_intstat              : 1 ;  //     3     
	UINT32 intr_cbus_new_dcap                 : 1 ;  //     4     
	UINT32 intr_cbus_new_ucp                  : 1 ;  //     5     
	UINT32 intr_cbus_cmd_ucp                  : 1 ;  //     6     
	UINT32 intr_cbus_pathen_clr               : 1 ;  //     7     
	UINT32 intr_cbus_mute_set                 : 1 ;  //     8     
	UINT32 intr_cbus_mute_clr                 : 1 ;  //     9     
	UINT32 intr_cbus_new_mscerr               : 1 ;  //    10     
	UINT32 intr_cbus_new_rcp                  : 1 ;  //    11     
	UINT32 intr_cbus_cmd_set_hpd              : 1 ;  //    12     
	UINT32 intr_cbus_cmd_get_state            : 1 ;  //    13     
	UINT32 intr_cbus_cmd_get_vdid             : 1 ;  //    14     
	UINT32 intr_cbus_cmd_rd_devcap            : 1 ;  //    15     
	UINT32 intr_cbus_cmd_rd_devintr           : 1 ;  //    16     
	UINT32 intr_cbus_cmd_pathen_set           : 1 ;  //    17     
	UINT32 intr_cbus_cmd_pathen_clr           : 1 ;  //    18     
	UINT32 intr_cbus_cmd_dcaprd_set           : 1 ;  //    19     
	UINT32 intr_cbus_cmd_3dreq_set            : 1 ;  //    20     
	UINT32 intr_cbus_cmd_grtwrt_set           : 1 ;  //    21     
	UINT32 intr_cbus_cmd_reqwrt_set           : 1 ;  //    22     
	UINT32 intr_cbus_cmd_wrt_burst            : 1 ;  //    23     
	UINT32 intr_cbus_cmd_dscrchg_set          : 1 ;  //    24     
	UINT32 intr_cbus_cmd_dcapchg_set          : 1 ;  //    25     
	UINT32 intr_cbus_cmd_edidchg_set          : 1 ;  //    26     
	UINT32 intr_cbus_cmd_clr_hpd              : 1 ;  //    27     
	UINT32 intr_cbus_cmd_rap_poll             : 1 ;  //    28     
	UINT32 intr_cbus_cmd_rap_on               : 1 ;  //    29     
	UINT32 intr_cbus_cmd_rap_off              : 1 ;  //    30     
	UINT32 intr_cbus_cmd_rcp                  : 1 ;  //    31     
} H15A0_LINK1_INTERRUPT_02_T;

/*-----------------------------------------------------------------------------
		0x24 link1_interrupt_03 ''
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
	UINT32 intr_cbus_new_rap                  : 1 ;  //    11     
	UINT32 intr_chg_avi_to_ycc420		      : 1 ;  //    12	: H15B0 Added     
} H15A0_LINK1_INTERRUPT_03_T;

/*-----------------------------------------------------------------------------
		0x428 link1_system_control_00 ''
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
	UINT32 reg_preamble_cnt                   : 4 ;  // 12: 9     
	UINT32 reg_hdmi_mode_sel                 : 1 ;  //    13     
	UINT32 reg_hdmi_mode                      : 1 ;  //    14     
	UINT32 reg_cd_sense                       : 1 ;  //    15     
	UINT32 reg_cd_sense_org                   : 1 ;  //    16     
	UINT32 reg_conn                           : 1 ;  //    17     
	UINT32 reg_scdt                           : 1 ;  //    18     
	UINT32 reg_hdmi5v                         : 1 ;  //    19     
	UINT32 reg_cbusclk_present                : 1 ;  //    20     
	UINT32 reg_audclk_present                 : 1 ;  //    21     
	UINT32 reg_pixclk_present                 : 1 ;  //    22     
	UINT32 reg_tmdsclk_present                : 1 ;  //    23     
	UINT32 reg_arc_src                        : 1 ;  //    24     
	UINT32 reg_edid_rd_done                   : 1 ;  //    25     
	UINT32 reg_edid_rd_cnt                    : 4 ;  // 29:26     
} H15A0_LINK1_SYSTEM_CONTROL_00_T;

/*-----------------------------------------------------------------------------
		0x42c link1_system_control_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_afifo_ctrl_thr                 : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vfifo_ctrl_thr                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_afifo_pdiff                    : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_vfifo_pdiff                    : 6 ;  // 29:24     
	UINT32 reg_scr_mode                       : 1 ;  //    30     
} H15A0_LINK1_SYSTEM_CONTROL_01_T;

/*-----------------------------------------------------------------------------
		0x430 link1_hdcp_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_0                      : 8 ;  //  7: 0     
	UINT32 reg_hdcp_unauth_mode_chg           : 1 ;  //     8     
	UINT32 reg_hdcp_unauth_nosync             : 1 ;  //     9     
	UINT32 reg_hdcp_dbg_bksv_ld               : 1 ;  //    10     
	UINT32 reg_hdcp_authed                    : 1 ;  //    11     
	UINT32 reg_hdcp_bypass_xor                : 1 ;  //    12     
	UINT32 reg_hdcp_disable                   : 1 ;  //    13     
	UINT32 reg_hdcp_enc_en                    : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bist_err                       : 1 ;  //    16     
	UINT32 reg_bist_done                      : 1 ;  //    17     
	UINT32 reg_bist_init                      : 1 ;  //    18     
	UINT32 reg_hdcp_mode                      : 2 ;  // 20:19     
	UINT32 reg_hdcp_aksv_en                   : 1 ;  //    21     
	UINT32 reg_vsync_pole                     : 2 ;  // 23:22     
	UINT32 reg_vsync_pole_man_mode            : 1 ;  //    24     
	UINT32 reg_vsync_pole_man                 : 1 ;  //    25     
	UINT32 reg_con_sel_man_mode               : 1 ;  //    26     
	UINT32 reg_con_sel_man                    : 1 ;  //    27     
} H15A0_LINK1_HDCP_00_T;

/*-----------------------------------------------------------------------------
		0x434 link1_hdcp_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1                      : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4                      : 8 ;  // 31:24     
} H15A0_LINK1_HDCP_01_T;

/*-----------------------------------------------------------------------------
		0x438 link1_hdcp_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7                      : 8 ;  // 31:24     
} H15A0_LINK1_HDCP_02_T;

/*-----------------------------------------------------------------------------
		0x43c link1_hdcp_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4                    : 8 ;  // 31:24     
} H15A0_LINK1_HDCP_03_T;

/*-----------------------------------------------------------------------------
		0x440 link1_hdcp_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3                    : 8 ;  // 31:24     
} H15A0_LINK1_HDCP_04_T;

/*-----------------------------------------------------------------------------
		0x444 link1_hdcp_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps                     : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4                    : 8 ;  // 31:24     
} H15A0_LINK1_HDCP_05_T;

/*-----------------------------------------------------------------------------
		0x448 link1_packet_00 ''
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
} H15A0_LINK1_PACKET_00_T;

/*-----------------------------------------------------------------------------
		0x44c link1_packet_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_01_T;

/*-----------------------------------------------------------------------------
		0x450 link1_packet_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_02_T;

/*-----------------------------------------------------------------------------
		0x454 link1_packet_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_03_T;

/*-----------------------------------------------------------------------------
		0x458 link1_packet_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_04_T;

/*-----------------------------------------------------------------------------
		0x45c link1_packet_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_05_T;

/*-----------------------------------------------------------------------------
		0x460 link1_packet_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_06_T;

/*-----------------------------------------------------------------------------
		0x464 link1_packet_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_07_T;

/*-----------------------------------------------------------------------------
		0x468 link1_packet_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_08_T;

/*-----------------------------------------------------------------------------
		0x46c link1_packet_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_09_T;

/*-----------------------------------------------------------------------------
		0x470 link1_packet_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_10_T;

/*-----------------------------------------------------------------------------
		0x474 link1_packet_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_11_T;

/*-----------------------------------------------------------------------------
		0x478 link1_packet_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_12_T;

/*-----------------------------------------------------------------------------
		0x47c link1_packet_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_13_T;

/*-----------------------------------------------------------------------------
		0x480 link1_packet_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_14_T;

/*-----------------------------------------------------------------------------
		0x484 link1_packet_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_15_T;

/*-----------------------------------------------------------------------------
		0x488 link1_packet_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_16_T;

/*-----------------------------------------------------------------------------
		0x48c link1_packet_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_asp_hdr_0                  : 8 ;  // 23:16     
	UINT32 reg_pkt_asp_hdr_1                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_17_T;

/*-----------------------------------------------------------------------------
		0x490 link1_packet_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_18_T;

/*-----------------------------------------------------------------------------
		0x494 link1_packet_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_19_T;

/*-----------------------------------------------------------------------------
		0x498 link1_packet_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_20_T;

/*-----------------------------------------------------------------------------
		0x49c link1_packet_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_21_T;

/*-----------------------------------------------------------------------------
		0x4a0 link1_packet_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_22_T;

/*-----------------------------------------------------------------------------
		0x4a4 link1_packet_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_23_T;

/*-----------------------------------------------------------------------------
		0x4a8 link1_packet_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_24_T;

/*-----------------------------------------------------------------------------
		0x4ac link1_packet_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_25_T;

/*-----------------------------------------------------------------------------
		0x4b0 link1_packet_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_26_T;

/*-----------------------------------------------------------------------------
		0x4b4 link1_packet_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_27_T;

/*-----------------------------------------------------------------------------
		0x4b8 link1_packet_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_28_T;

/*-----------------------------------------------------------------------------
		0x4bc link1_packet_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_29_T;

/*-----------------------------------------------------------------------------
		0x4c0 link1_packet_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_30_T;

/*-----------------------------------------------------------------------------
		0x4c4 link1_packet_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_31_T;

/*-----------------------------------------------------------------------------
		0x4c8 link1_packet_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_32_T;

/*-----------------------------------------------------------------------------
		0x4cc link1_packet_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_hbr_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_hbr_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gcp_smute                  : 1 ;  //    16     
	UINT32                                    : 8 ;  // 24:17     reserved
	UINT32 reg_pkt_gcp_cmute                  : 1 ;  //    25     
} H15A0_LINK1_PACKET_33_T;

/*-----------------------------------------------------------------------------
		0x4d0 link1_packet_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_hdr_1                : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_34_T;

/*-----------------------------------------------------------------------------
		0x4d4 link1_packet_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_3                : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_35_T;

/*-----------------------------------------------------------------------------
		0x4d8 link1_packet_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_7                : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_36_T;

/*-----------------------------------------------------------------------------
		0x4dc link1_packet_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_11               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_37_T;

/*-----------------------------------------------------------------------------
		0x4e0 link1_packet_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_15               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_38_T;

/*-----------------------------------------------------------------------------
		0x4e4 link1_packet_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_19               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_39_T;

/*-----------------------------------------------------------------------------
		0x4e8 link1_packet_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_23               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_40_T;

/*-----------------------------------------------------------------------------
		0x4ec link1_packet_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_27               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_41_T;

/*-----------------------------------------------------------------------------
		0x4f0 link1_packet_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_hdr_1                : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_42_T;

/*-----------------------------------------------------------------------------
		0x4f4 link1_packet_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_3                : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_43_T;

/*-----------------------------------------------------------------------------
		0x4f8 link1_packet_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_7                : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_44_T;

/*-----------------------------------------------------------------------------
		0x4fc link1_packet_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_11               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_45_T;

/*-----------------------------------------------------------------------------
		0x500 link1_packet_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_15               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_46_T;

/*-----------------------------------------------------------------------------
		0x504 link1_packet_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_19               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_47_T;

/*-----------------------------------------------------------------------------
		0x508 link1_packet_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_23               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_48_T;

/*-----------------------------------------------------------------------------
		0x50c link1_packet_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_27               : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_49_T;

/*-----------------------------------------------------------------------------
		0x510 link1_packet_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_50_T;

/*-----------------------------------------------------------------------------
		0x514 link1_packet_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_51_T;

/*-----------------------------------------------------------------------------
		0x518 link1_packet_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_52_T;

/*-----------------------------------------------------------------------------
		0x51c link1_packet_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_53_T;

/*-----------------------------------------------------------------------------
		0x520 link1_packet_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_54_T;

/*-----------------------------------------------------------------------------
		0x524 link1_packet_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_55_T;

/*-----------------------------------------------------------------------------
		0x528 link1_packet_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_56_T;

/*-----------------------------------------------------------------------------
		0x52c link1_packet_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_57_T;

/*-----------------------------------------------------------------------------
		0x530 link1_packet_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_58_T;

/*-----------------------------------------------------------------------------
		0x534 link1_packet_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_59_T;

/*-----------------------------------------------------------------------------
		0x538 link1_packet_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_60_T;

/*-----------------------------------------------------------------------------
		0x53c link1_packet_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_61_T;

/*-----------------------------------------------------------------------------
		0x540 link1_packet_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_62_T;

/*-----------------------------------------------------------------------------
		0x544 link1_packet_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_63_T;

/*-----------------------------------------------------------------------------
		0x548 link1_packet_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_64_T;

/*-----------------------------------------------------------------------------
		0x54c link1_packet_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_65_T;

/*-----------------------------------------------------------------------------
		0x550 link1_packet_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_0                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_1                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_2                      : 8 ;  // 23:16     
} H15A0_LINK1_PACKET_66_T;

/*-----------------------------------------------------------------------------
		0x554 link1_packet_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_3                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_4                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_5                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_6                      : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_67_T;

/*-----------------------------------------------------------------------------
		0x558 link1_packet_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_7                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_8                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_9                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_10                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_68_T;

/*-----------------------------------------------------------------------------
		0x55c link1_packet_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_11                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_12                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_13                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_14                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_69_T;

/*-----------------------------------------------------------------------------
		0x560 link1_packet_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_15                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_16                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_17                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_18                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_70_T;

/*-----------------------------------------------------------------------------
		0x564 link1_packet_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_19                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_20                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_21                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_22                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_71_T;

/*-----------------------------------------------------------------------------
		0x568 link1_packet_72 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_23                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_24                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_25                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_26                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_72_T;

/*-----------------------------------------------------------------------------
		0x56c link1_packet_73 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_27                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_28                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_29                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_30                     : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_73_T;

/*-----------------------------------------------------------------------------
		0x570 link1_packet_74 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK1_PACKET_74_T;

/*-----------------------------------------------------------------------------
		0x574 link1_packet_75 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_75_T;

/*-----------------------------------------------------------------------------
		0x578 link1_packet_76 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_76_T;

/*-----------------------------------------------------------------------------
		0x57c link1_packet_77 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_77_T;

/*-----------------------------------------------------------------------------
		0x580 link1_packet_78 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_78_T;

/*-----------------------------------------------------------------------------
		0x584 link1_packet_79 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_79_T;

/*-----------------------------------------------------------------------------
		0x588 link1_packet_80 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_80_T;

/*-----------------------------------------------------------------------------
		0x58c link1_packet_81 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK1_PACKET_81_T;

/*-----------------------------------------------------------------------------
		0x590 link1_packet_82 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_vsi_hdmi_vic               : 8 ;  //  7: 0     
	UINT32 reg_dbg_vsi_3d_struct              : 4 ;  // 11: 8     
	UINT32 reg_dbg_vsi_3d_meta_en             : 1 ;  //    12     
	UINT32 reg_dbg_vsi_3d                     : 1 ;  //    13     
	UINT32 reg_dbg_vsi_4k2k                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_pkt_clr_spd_cnt                : 4 ;  // 19:16     
} H15A0_LINK1_PACKET_82_T;

/*-----------------------------------------------------------------------------
		0x594 link1_video_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pr_tx                          : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_pr_rx                          : 4 ;  // 11: 8     
	UINT32 reg_pr_rx_en                       : 1 ;  //    12     
	UINT32 reg_pr_cmu_sync                    : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_pr_cbcr                        : 4 ;  // 19:16     
} H15A0_LINK1_VIDEO_00_T;

/*-----------------------------------------------------------------------------
		0x598 link1_video_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_align422_tx                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_align422_rx                    : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_align422_rx_en                 : 1 ;  //    12     
	UINT32 reg_align422_swap_rb               : 1 ;  //    13     
} H15A0_LINK1_VIDEO_01_T;

/*-----------------------------------------------------------------------------
		0x59c link1_video_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cmode_tx                       : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cmode_rx                       : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cmode_rx_en                    : 1 ;  //    16     
	UINT32 reg_cmode_ppll_sync                : 1 ;  //    17     
} H15A0_LINK1_VIDEO_02_T;

/*-----------------------------------------------------------------------------
		0x5a0 link1_video_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_neg_pol_en                     : 1 ;  //     0     
	UINT32 reg_inv_hs                         : 1 ;  //     1     
	UINT32 reg_inv_vs                         : 1 ;  //     2     
	UINT32 reg_inv_eo                         : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vf_clr                         : 1 ;  //     8     
	UINT32 reg_vf_clr_nosync                  : 1 ;  //     9     
} H15A0_LINK1_VIDEO_03_T;

/*-----------------------------------------------------------------------------
		0x5a4 link1_video_04 ''		// register modified 
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_tot                        : 13 ;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_tot                        : 12 ;  // 27:16     
} H15A0_LINK1_VIDEO_04_T;

/*-----------------------------------------------------------------------------
		0x5a8 link1_video_05 ''		// register modified
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_av                         : 13 ;  //  12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_av                         : 12 ;  // 27:16     
} H15A0_LINK1_VIDEO_05_T;

/*-----------------------------------------------------------------------------
		0x5ac link1_video_06 ''		// register modified
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_fp                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_v_fp                           : 6 ;  // 21:16     
} H15A0_LINK1_VIDEO_06_T;

/*-----------------------------------------------------------------------------
		0x5b0 link1_video_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_bp                           : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_v_bp                           : 7 ;  // 22:16     
} H15A0_LINK1_VIDEO_07_T;

/*-----------------------------------------------------------------------------
		0x5b4 link1_video_08 ''		// register modified
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_sync                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_v_sync                         : 8 ;  // 23:16     
} H15A0_LINK1_VIDEO_08_T;

/*-----------------------------------------------------------------------------
		0x5b8 link1_video_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vid_blank_g                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_vid_blank_r                    : 12;  // 27:16     
} H15A0_LINK1_VIDEO_09_T;

/*-----------------------------------------------------------------------------
		0x5bc link1_video_10 ''
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
} H15A0_LINK1_VIDEO_10_T;

/*-----------------------------------------------------------------------------
		0x5c0 link1_audio_00 ''
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
} H15A0_LINK1_AUDIO_00_T;

/*-----------------------------------------------------------------------------
		0x5c4 link1_audio_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_tx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_tx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_tx_2                     : 4 ;  // 19:16     
} H15A0_LINK1_AUDIO_01_T;

/*-----------------------------------------------------------------------------
		0x5c8 link1_audio_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_rx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_rx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_rx_2                     : 4 ;  // 19:16     
} H15A0_LINK1_AUDIO_02_T;

/*-----------------------------------------------------------------------------
		0x5cc link1_audio_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_tx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_tx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_tx_2                   : 4 ;  // 19:16     
} H15A0_LINK1_AUDIO_03_T;

/*-----------------------------------------------------------------------------
		0x5d0 link1_audio_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_rx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_rx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_rx_2                   : 4 ;  // 19:16     
} H15A0_LINK1_AUDIO_04_T;

/*-----------------------------------------------------------------------------
		0x5d4 link1_audio_05 ''
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
} H15A0_LINK1_AUDIO_05_T;

/*-----------------------------------------------------------------------------
		0x5d8 link1_audio_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sp_paerr                       : 1 ;  //     0     
	UINT32 reg_sp_pserr                       : 1 ;  //     1     
	UINT32 reg_sp_gnd_mode                    : 1 ;  //     2     
	UINT32 reg_sp_en                          : 1 ;  //     3     
} H15A0_LINK1_AUDIO_06_T;

/*-----------------------------------------------------------------------------
		0x5dc link1_audio_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_ow_byte1                 : 8 ;  //  7: 0     
	UINT32 reg_achst_ow_bit2                  : 1 ;  //     8     
	UINT32 reg_achst_ow_en                    : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_achst_fs                       : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_achst_byte0                    : 8 ;  // 31:24     
} H15A0_LINK1_AUDIO_07_T;

/*-----------------------------------------------------------------------------
		0x5e0 link1_audio_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_byte1                    : 8 ;  //  7: 0     
	UINT32 reg_achst_byte2                    : 8 ;  // 15: 8     
	UINT32 reg_achst_byte3                    : 8 ;  // 23:16     
	UINT32 reg_achst_byte4                    : 8 ;  // 31:24     
} H15A0_LINK1_AUDIO_08_T;

/*-----------------------------------------------------------------------------
		0x5e4 link1_audio_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_pc_0                     : 8 ;  //  7: 0     
	UINT32 reg_burst_pc_1                     : 8 ;  // 15: 8     
	UINT32 reg_burst_pd_0                     : 8 ;  // 23:16     
	UINT32 reg_burst_pd_1                     : 8 ;  // 31:24     
} H15A0_LINK1_AUDIO_09_T;

/*-----------------------------------------------------------------------------
		0x5e8 link1_audio_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_i2s_ch_mute                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_i2s_len_ovrr                   : 4 ;  // 11: 8     
	UINT32 reg_i2s_len_ovrr_en                : 1 ;  //    12     
	UINT32 reg_burst_auto_clr                 : 1 ;  //    13     
} H15A0_LINK1_AUDIO_10_T;

/*-----------------------------------------------------------------------------
		0x5f0 link1_cbus_00 ''
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
	UINT32 reg_cbus_disconn_man               : 1 ;  //    15     
	UINT32 reg_cbus_disconn_en                : 1 ;  //    16     
	UINT32 reg_cbus_out_inv                   : 1 ;  //    17     
	UINT32 reg_cbus_out_en_sel                : 1 ;  //    18     
	UINT32 reg_phy_sink_mhl_en                : 1 ;  //    19     
	UINT32 reg_phy_sink_vbus_en               : 1 ;  //    20     
	UINT32 reg_phy_sink_ztmds                 : 1 ;  //    21     
	UINT32 reg_phy_sink_zterm                 : 1 ;  //    22     
	UINT32 reg_phy_sink_cbus_zon              : 1 ;  //    23     
	UINT32 reg_phy_sink_cbus_zdis             : 1 ;  //    24     
} H15A0_LINK1_CBUS_00_T;

/*-----------------------------------------------------------------------------
		0x5f4 link1_cbus_01 ''
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
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_cbus_disconn_cnt               : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_01_T;

/*-----------------------------------------------------------------------------
		0x5f8 link1_cbus_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_thres             : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_n_retry                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_wait_arb_0             : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_wait_arb_1             : 2 ;  // 25:24     
} H15A0_LINK1_CBUS_02_T;

/*-----------------------------------------------------------------------------
		0x5fc link1_cbus_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_cnt               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_src_bit_length         : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_st                     : 4 ;  // 19:16     
	UINT32 reg_cbus_rcv_msg_timeout_0         : 4 ;  // 23:20     
	UINT32 reg_cbus_rcv_msg_timeout_1         : 6 ;  // 29:24     
} H15A0_LINK1_CBUS_03_T;

/*-----------------------------------------------------------------------------
		0x600 link1_cbus_04 ''
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
} H15A0_LINK1_CBUS_04_T;

/*-----------------------------------------------------------------------------
		0x604 link1_cbus_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_hpd             : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_seq_st                         : 8 ;  // 15: 8     
	UINT32 reg_cmd_st                         : 8 ;  // 23:16     
	UINT32 reg_man_msc_wrt_stat_pathen_clr    : 1 ;  //    24     
} H15A0_LINK1_CBUS_05_T;

/*-----------------------------------------------------------------------------
		0x608 link1_cbus_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rd_devcap           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_vdid            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_get_state           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_clr_hpd             : 5 ;  // 28:24     
} H15A0_LINK1_CBUS_06_T;

/*-----------------------------------------------------------------------------
		0x60c link1_cbus_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_3dreq_set  : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_msc_err         : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_burst           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_rd_dev_intr_chg     : 5 ;  // 28:24     
} H15A0_LINK1_CBUS_07_T;

/*-----------------------------------------------------------------------------
		0x610 link1_cbus_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_dcapchgset : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_set_intr_dscrchgset : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_set_intr_reqwrt_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_grtwrt_set : 5 ;  // 28:24     
} H15A0_LINK1_CBUS_08_T;

/*-----------------------------------------------------------------------------
		0x614 link1_cbus_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_wrt_stat_pathen_clr : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_wrt_stat_pathen_set : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_stat_dcaprd_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_edidchgset : 5 ;  // 28:24     
} H15A0_LINK1_CBUS_09_T;

/*-----------------------------------------------------------------------------
		0x618 link1_cbus_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_rcp_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rapk_ecode                  : 8 ;  // 15: 8     
	UINT32 reg_rx_rapk_ecode                  : 8 ;  // 23:16     
	UINT32 reg_man_msg_rap_content_off        : 1 ;  //    24     
	UINT32 reg_man_msg_rap_content_on         : 1 ;  //    25     
	UINT32 reg_man_msg_rap_poll               : 1 ;  //    26     
} H15A0_LINK1_CBUS_10_T;

/*-----------------------------------------------------------------------------
		0x61c link1_cbus_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_rcpk_rtn_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rcpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_rcpk_rtn_kcode              : 8 ;  // 23:16     
	UINT32 reg_man_msg_rcp_cmd                : 1 ;  //    24     
} H15A0_LINK1_CBUS_11_T;

/*-----------------------------------------------------------------------------
		0x620 link1_cbus_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_src_support_rcp            : 1 ;  //     0     
	UINT32 reg_man_src_support_rap            : 1 ;  //     1     
} H15A0_LINK1_CBUS_12_T;

/*-----------------------------------------------------------------------------
		0x624 link1_cbus_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rcp                 : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_state_sink_rap_cnt_off         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_state_sink_rap_cnt_on          : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_state_sink_rap_poll            : 7 ;  // 30:24     
} H15A0_LINK1_CBUS_13_T;

/*-----------------------------------------------------------------------------
		0x628 link1_cbus_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_msc_errcode                 : 8 ;  //  7: 0     
	UINT32 reg_tx_ddc_errcode                 : 8 ;  // 15: 8     
	UINT32 reg_tx_msc_errcode                 : 8 ;  // 23:16     
	UINT32 reg_get_msc_errcode_enable         : 1 ;  //    24     
} H15A0_LINK1_CBUS_14_T;

/*-----------------------------------------------------------------------------
		0x62c link1_cbus_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_adopter_id_h          : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_dev_cat               : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_mhl_version           : 8 ;  // 23:16     
	UINT32 reg_sink_cap_dev_state             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_15_T;

/*-----------------------------------------------------------------------------
		0x630 link1_cbus_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_video_type            : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_aud_link_mode         : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_vid_link_mode         : 8 ;  // 23:16     
	UINT32 reg_sink_cap_adopter_id_l          : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_16_T;

/*-----------------------------------------------------------------------------
		0x634 link1_cbus_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_device_id_h           : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_feature_flag          : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_bandwidth             : 8 ;  // 23:16     
	UINT32 reg_sink_cap_log_dev_map           : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_17_T;

/*-----------------------------------------------------------------------------
		0x638 link1_cbus_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_intr_stat_size        : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_scratchpad_size       : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_device_id_l           : 8 ;  // 23:16     
} H15A0_LINK1_CBUS_18_T;

/*-----------------------------------------------------------------------------
		0x63c link1_cbus_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_intr_reserved_23          : 8 ;  //  7: 0     
	UINT32 reg_sink_intr_reserved_22          : 8 ;  // 15: 8     
	UINT32 reg_sink_intr_dchg_int             : 8 ;  // 23:16     
	UINT32 reg_sink_intr_rchg_int             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_19_T;

/*-----------------------------------------------------------------------------
		0x640 link1_cbus_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_stat_reserved_33          : 8 ;  //  7: 0     
	UINT32 reg_sink_stat_reserved_32          : 8 ;  // 15: 8     
	UINT32 reg_sink_stat_link_mode            : 8 ;  // 23:16     
	UINT32 reg_sink_stat_conn_rdy             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_20_T;

/*-----------------------------------------------------------------------------
		0x644 link1_cbus_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_vendor_id_c1               : 8 ;  //  7: 0     
	UINT32 reg_src_dev_state_c0               : 8 ;  // 15: 8     
	UINT32 reg_sink_vendor_id_80              : 8 ;  // 23:16     
} H15A0_LINK1_CBUS_21_T;

/*-----------------------------------------------------------------------------
		0x648 link1_cbus_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_43             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_42             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_41             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_40             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_22_T;

/*-----------------------------------------------------------------------------
		0x64c link1_cbus_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_47             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_46             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_45             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_44             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_23_T;

/*-----------------------------------------------------------------------------
		0x650 link1_cbus_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4b             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4a             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_49             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_48             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_24_T;

/*-----------------------------------------------------------------------------
		0x654 link1_cbus_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4f             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4e             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_4d             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_4c             : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_25_T;

/*-----------------------------------------------------------------------------
		0x658 link1_cbus_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_adopter_id_l           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_adopter_id_h           : 8 ;  // 15: 8     
	UINT32 reg_src_cap_dev_cat                : 8 ;  // 23:16     
	UINT32 reg_src_cap_mhl_version            : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_26_T;

/*-----------------------------------------------------------------------------
		0x65c link1_cbus_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_feature_flag           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_log_dev_map            : 8 ;  // 15: 8     
	UINT32 reg_src_cap_aud_link_mode          : 8 ;  // 23:16     
	UINT32 reg_src_cap_vid_link_mode          : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_27_T;

/*-----------------------------------------------------------------------------
		0x660 link1_cbus_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_intr_stat_size         : 8 ;  //  7: 0     
	UINT32 reg_src_cap_scratchpad_size        : 8 ;  // 15: 8     
	UINT32 reg_src_cap_device_id_l            : 8 ;  // 23:16     
	UINT32 reg_src_cap_device_id_h            : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_28_T;

/*-----------------------------------------------------------------------------
		0x664 link1_cbus_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_65          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_64          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_63          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_62          : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_29_T;

/*-----------------------------------------------------------------------------
		0x668 link1_cbus_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_69          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_68          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_67          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_66          : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_30_T;

/*-----------------------------------------------------------------------------
		0x66c link1_cbus_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6d          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6c          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_6b          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_6a          : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_31_T;

/*-----------------------------------------------------------------------------
		0x670 link1_cbus_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6f          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6e          : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_next_2              : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_next_1              : 3 ;  // 26:24     
} H15A0_LINK1_CBUS_32_T;

/*-----------------------------------------------------------------------------
		0x674 link1_cbus_33 ''
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
} H15A0_LINK1_CBUS_33_T;

/*-----------------------------------------------------------------------------
		0x678 link1_cbus_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_pulse_width_2_max    : 8 ;  //  7: 0     
	UINT32 reg_cbus_sink_pulse_width_min      : 8 ;  // 15: 8     
	UINT32 reg_cbus_sink_pulse_width_max      : 8 ;  // 23:16     
	UINT32 reg_seq_msc_wrt_burst              : 4 ;  // 27:24     
	UINT32 reg_man_msc_wrt_burst              : 1 ;  //    28     
	UINT32 reg_man_msc_set_intr_reqwrt_set    : 1 ;  //    29     
	UINT32 reg_man_msc_set_intr_dscrch_set    : 1 ;  //    30     
} H15A0_LINK1_CBUS_34_T;

/*-----------------------------------------------------------------------------
		0x67c link1_cbus_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_to_discover_min_2    : 8 ;  //  7: 0     
	UINT32 reg_cbus_wake_to_discover_min_1    : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_cbus_wake_to_discover_max_2    : 8 ;  // 23:16     
	UINT32 reg_cbus_wake_to_discover_max_1    : 2 ;  // 25:24     
} H15A0_LINK1_CBUS_35_T;

/*-----------------------------------------------------------------------------
		0x680 link1_cbus_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_ack_str                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_ack_min                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_ack_max                : 8 ;  // 23:16     
} H15A0_LINK1_CBUS_36_T;

/*-----------------------------------------------------------------------------
		0x684 link1_cbus_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_arb_min                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_arb_max                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_min                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_max                : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_37_T;

/*-----------------------------------------------------------------------------
		0x688 link1_cbus_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_syn0_min               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_syn0_max               : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_syn1_min               : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_syn1_max               : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_38_T;

/*-----------------------------------------------------------------------------
		0x68c link1_cbus_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_bit_bn1                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_bit_bn2                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_bn3                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_bn4                : 8 ;  // 31:24     
} H15A0_LINK1_CBUS_39_T;

/*-----------------------------------------------------------------------------
		0x690 link1_cbus_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_ucpk_rtn_ccode              : 8 ;  //  7: 0     
	UINT32 reg_tx_ucpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_ucpk_rtn_ccode              : 8 ;  // 23:16     
	UINT32 reg_state_sink_ucp                 : 7 ;  // 30:24     
} H15A0_LINK1_CBUS_40_T;

/*-----------------------------------------------------------------------------
		0x694 link1_cbus_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_ucp_cmd                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_man_src_support_ucp            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_man_msg_ucp_ccode              : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_err_2               : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_err_1               : 3 ;  // 26:24     
} H15A0_LINK1_CBUS_41_T;

/*-----------------------------------------------------------------------------
		0x698 link1_cbus_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_61          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_60          : 8 ;  // 15: 8     
	UINT32 reg_src_scratchpad_num             : 4 ;  // 19:16     
	UINT32 reg_src_scratchpad_id_sel          : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_src_mrc_unsupport              : 1 ;  //    24     
	UINT32 reg_src_mrc_support                : 1 ;  //    25     
} H15A0_LINK1_CBUS_42_T;

/*-----------------------------------------------------------------------------
		0x69c link1_edid_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_255                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_254                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_253                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_252                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_00_T;

/*-----------------------------------------------------------------------------
		0x6a0 link1_edid_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_251                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_250                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_249                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_248                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_01_T;

/*-----------------------------------------------------------------------------
		0x6a4 link1_edid_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_247                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_246                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_245                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_244                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_02_T;

/*-----------------------------------------------------------------------------
		0x6a8 link1_edid_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_243                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_242                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_241                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_240                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_03_T;

/*-----------------------------------------------------------------------------
		0x6ac link1_edid_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_239                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_238                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_237                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_236                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_04_T;

/*-----------------------------------------------------------------------------
		0x6b0 link1_edid_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_235                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_234                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_233                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_232                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_05_T;

/*-----------------------------------------------------------------------------
		0x6b4 link1_edid_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_231                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_230                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_229                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_228                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_06_T;

/*-----------------------------------------------------------------------------
		0x6b8 link1_edid_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_227                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_226                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_225                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_224                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_07_T;

/*-----------------------------------------------------------------------------
		0x6bc link1_edid_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_223                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_222                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_221                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_220                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_08_T;

/*-----------------------------------------------------------------------------
		0x6c0 link1_edid_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_219                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_218                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_217                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_216                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_09_T;

/*-----------------------------------------------------------------------------
		0x6c4 link1_edid_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_215                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_214                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_213                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_212                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_10_T;

/*-----------------------------------------------------------------------------
		0x6c8 link1_edid_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_211                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_210                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_209                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_208                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_11_T;

/*-----------------------------------------------------------------------------
		0x6cc link1_edid_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_207                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_206                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_205                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_204                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_12_T;

/*-----------------------------------------------------------------------------
		0x6d0 link1_edid_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_203                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_202                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_201                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_200                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_13_T;

/*-----------------------------------------------------------------------------
		0x6d4 link1_edid_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_199                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_198                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_197                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_196                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_14_T;

/*-----------------------------------------------------------------------------
		0x6d8 link1_edid_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_195                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_194                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_193                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_192                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_15_T;

/*-----------------------------------------------------------------------------
		0x6dc link1_edid_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_191                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_190                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_189                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_188                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_16_T;

/*-----------------------------------------------------------------------------
		0x6e0 link1_edid_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_187                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_186                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_185                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_184                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_17_T;

/*-----------------------------------------------------------------------------
		0x6e4 link1_edid_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_183                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_182                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_181                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_180                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_18_T;

/*-----------------------------------------------------------------------------
		0x6e8 link1_edid_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_179                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_178                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_177                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_176                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_19_T;

/*-----------------------------------------------------------------------------
		0x6ec link1_edid_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_175                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_174                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_173                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_172                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_20_T;

/*-----------------------------------------------------------------------------
		0x6f0 link1_edid_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_171                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_170                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_169                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_168                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_21_T;

/*-----------------------------------------------------------------------------
		0x6f4 link1_edid_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_167                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_166                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_165                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_164                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_22_T;

/*-----------------------------------------------------------------------------
		0x6f8 link1_edid_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_163                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_162                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_161                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_160                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_23_T;

/*-----------------------------------------------------------------------------
		0x6fc link1_edid_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_159                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_158                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_157                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_156                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_24_T;

/*-----------------------------------------------------------------------------
		0x700 link1_edid_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_155                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_154                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_153                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_152                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_25_T;

/*-----------------------------------------------------------------------------
		0x704 link1_edid_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_151                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_150                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_149                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_148                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_26_T;

/*-----------------------------------------------------------------------------
		0x708 link1_edid_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_147                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_146                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_145                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_144                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_27_T;

/*-----------------------------------------------------------------------------
		0x70c link1_edid_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_143                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_142                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_141                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_140                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_28_T;

/*-----------------------------------------------------------------------------
		0x710 link1_edid_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_139                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_138                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_137                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_136                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_29_T;

/*-----------------------------------------------------------------------------
		0x714 link1_edid_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_135                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_134                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_133                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_132                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_30_T;

/*-----------------------------------------------------------------------------
		0x718 link1_edid_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_131                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_130                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_129                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_128                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_31_T;

/*-----------------------------------------------------------------------------
		0x71c link1_edid_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_127                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_126                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_125                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_124                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_32_T;

/*-----------------------------------------------------------------------------
		0x720 link1_edid_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_123                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_122                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_121                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_120                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_33_T;

/*-----------------------------------------------------------------------------
		0x724 link1_edid_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_119                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_118                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_117                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_116                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_34_T;

/*-----------------------------------------------------------------------------
		0x728 link1_edid_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_115                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_114                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_113                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_112                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_35_T;

/*-----------------------------------------------------------------------------
		0x72c link1_edid_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_111                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_110                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_109                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_108                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_36_T;

/*-----------------------------------------------------------------------------
		0x730 link1_edid_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_107                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_106                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_105                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_104                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_37_T;

/*-----------------------------------------------------------------------------
		0x734 link1_edid_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_103                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_102                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_101                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_100                   : 8 ;  // 31:24     
} H15A0_LINK1_EDID_38_T;

/*-----------------------------------------------------------------------------
		0x738 link1_edid_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_99                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_98                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_97                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_96                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_39_T;

/*-----------------------------------------------------------------------------
		0x73c link1_edid_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_95                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_94                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_93                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_92                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_40_T;

/*-----------------------------------------------------------------------------
		0x740 link1_edid_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_91                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_90                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_89                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_88                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_41_T;

/*-----------------------------------------------------------------------------
		0x744 link1_edid_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_87                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_86                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_85                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_84                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_42_T;

/*-----------------------------------------------------------------------------
		0x748 link1_edid_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_83                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_82                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_81                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_80                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_43_T;

/*-----------------------------------------------------------------------------
		0x74c link1_edid_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_79                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_78                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_77                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_76                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_44_T;

/*-----------------------------------------------------------------------------
		0x750 link1_edid_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_75                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_74                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_73                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_72                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_45_T;

/*-----------------------------------------------------------------------------
		0x754 link1_edid_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_71                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_70                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_69                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_68                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_46_T;

/*-----------------------------------------------------------------------------
		0x758 link1_edid_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_67                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_66                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_65                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_64                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_47_T;

/*-----------------------------------------------------------------------------
		0x75c link1_edid_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_63                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_62                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_61                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_60                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_48_T;

/*-----------------------------------------------------------------------------
		0x760 link1_edid_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_59                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_58                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_57                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_56                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_49_T;

/*-----------------------------------------------------------------------------
		0x764 link1_edid_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_55                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_54                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_53                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_52                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_50_T;

/*-----------------------------------------------------------------------------
		0x768 link1_edid_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_51                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_50                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_49                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_48                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_51_T;

/*-----------------------------------------------------------------------------
		0x76c link1_edid_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_47                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_46                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_45                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_44                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_52_T;

/*-----------------------------------------------------------------------------
		0x770 link1_edid_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_43                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_42                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_41                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_40                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_53_T;

/*-----------------------------------------------------------------------------
		0x774 link1_edid_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_39                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_38                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_37                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_36                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_54_T;

/*-----------------------------------------------------------------------------
		0x778 link1_edid_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_35                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_34                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_33                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_32                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_55_T;

/*-----------------------------------------------------------------------------
		0x77c link1_edid_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_31                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_30                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_29                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_28                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_56_T;

/*-----------------------------------------------------------------------------
		0x780 link1_edid_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_27                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_26                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_25                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_24                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_57_T;

/*-----------------------------------------------------------------------------
		0x784 link1_edid_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_23                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_22                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_21                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_20                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_58_T;

/*-----------------------------------------------------------------------------
		0x788 link1_edid_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_19                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_18                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_17                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_16                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_59_T;

/*-----------------------------------------------------------------------------
		0x78c link1_edid_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_15                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_14                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_13                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_12                    : 8 ;  // 31:24     
} H15A0_LINK1_EDID_60_T;

/*-----------------------------------------------------------------------------
		0x790 link1_edid_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_11                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_10                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_9                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_8                     : 8 ;  // 31:24     
} H15A0_LINK1_EDID_61_T;

/*-----------------------------------------------------------------------------
		0x794 link1_edid_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_7                     : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_6                     : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_5                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_4                     : 8 ;  // 31:24     
} H15A0_LINK1_EDID_62_T;

/*-----------------------------------------------------------------------------
		0x798 link1_edid_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_3                     : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_2                     : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_1                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_0                     : 8 ;  // 31:24     
} H15A0_LINK1_EDID_63_T;
typedef struct {
	H15A0_LINK1_PHY_00_T               	phy_00;                 //0x400
	H15A0_LINK1_PHY_01_T               	phy_01;                 //0x404
	H15A0_LINK1_INTERRUPT_ENABLE_00_T  	interrupt_enable_00;    //0x408
	H15A0_LINK1_INTERRUPT_ENABLE_01_T  	interrupt_enable_01;    //0x40c
	H15A0_LINK1_INTERRUPT_ENABLE_02_T  	interrupt_enable_02;    //0x410
	H15A0_LINK1_INTERRUPT_ENABLE_03_T  	interrupt_enable_03;    //0x414
	H15A0_LINK1_INTERRUPT_00_T         	interrupt_00;           //0x418
	H15A0_LINK1_INTERRUPT_01_T         	interrupt_01;           //0x41c
	H15A0_LINK1_INTERRUPT_02_T         	interrupt_02;           //0x420
	H15A0_LINK1_INTERRUPT_03_T         	interrupt_03;           //0x424~0x24
	H15A0_LINK1_SYSTEM_CONTROL_00_T    	system_control_00;      //0x428
	H15A0_LINK1_SYSTEM_CONTROL_01_T    	system_control_01;      //0x42c
	H15A0_LINK1_HDCP_00_T              	hdcp_00;                //0x430
	H15A0_LINK1_HDCP_01_T              	hdcp_01;                //0x434
	H15A0_LINK1_HDCP_02_T              	hdcp_02;                //0x438
	H15A0_LINK1_HDCP_03_T              	hdcp_03;                //0x43c
	H15A0_LINK1_HDCP_04_T              	hdcp_04;                //0x440
	H15A0_LINK1_HDCP_05_T              	hdcp_05;                //0x444
	H15A0_LINK1_PACKET_00_T            	packet_00;              //0x448
	H15A0_LINK1_PACKET_01_T            	packet_01;              //0x44c
	H15A0_LINK1_PACKET_02_T            	packet_02;              //0x450
	H15A0_LINK1_PACKET_03_T            	packet_03;              //0x454
	H15A0_LINK1_PACKET_04_T            	packet_04;              //0x458
	H15A0_LINK1_PACKET_05_T            	packet_05;              //0x45c
	H15A0_LINK1_PACKET_06_T            	packet_06;              //0x460
	H15A0_LINK1_PACKET_07_T            	packet_07;              //0x464
	H15A0_LINK1_PACKET_08_T            	packet_08;              //0x468
	H15A0_LINK1_PACKET_09_T            	packet_09;              //0x46c
	H15A0_LINK1_PACKET_10_T            	packet_10;              //0x470
	H15A0_LINK1_PACKET_11_T            	packet_11;              //0x474
	H15A0_LINK1_PACKET_12_T            	packet_12;              //0x478
	H15A0_LINK1_PACKET_13_T            	packet_13;              //0x47c
	H15A0_LINK1_PACKET_14_T            	packet_14;              //0x480
	H15A0_LINK1_PACKET_15_T            	packet_15;              //0x484
	H15A0_LINK1_PACKET_16_T            	packet_16;              //0x488
	H15A0_LINK1_PACKET_17_T            	packet_17;              //0x48c
	H15A0_LINK1_PACKET_18_T            	packet_18;              //0x490
	H15A0_LINK1_PACKET_19_T            	packet_19;              //0x494
	H15A0_LINK1_PACKET_20_T            	packet_20;              //0x498
	H15A0_LINK1_PACKET_21_T            	packet_21;              //0x49c
	H15A0_LINK1_PACKET_22_T            	packet_22;              //0x4a0
	H15A0_LINK1_PACKET_23_T            	packet_23;              //0x4a4
	H15A0_LINK1_PACKET_24_T            	packet_24;              //0x4a8
	H15A0_LINK1_PACKET_25_T            	packet_25;              //0x4ac
	H15A0_LINK1_PACKET_26_T            	packet_26;              //0x4b0
	H15A0_LINK1_PACKET_27_T            	packet_27;              //0x4b4
	H15A0_LINK1_PACKET_28_T            	packet_28;              //0x4b8
	H15A0_LINK1_PACKET_29_T            	packet_29;              //0x4bc
	H15A0_LINK1_PACKET_30_T            	packet_30;              //0x4c0
	H15A0_LINK1_PACKET_31_T            	packet_31;              //0x4c4
	H15A0_LINK1_PACKET_32_T            	packet_32;              //0x4c8
	H15A0_LINK1_PACKET_33_T            	packet_33;              //0x4cc
	H15A0_LINK1_PACKET_34_T            	packet_34;              //0x4d0
	H15A0_LINK1_PACKET_35_T            	packet_35;              //0x4d4
	H15A0_LINK1_PACKET_36_T            	packet_36;              //0x4d8
	H15A0_LINK1_PACKET_37_T            	packet_37;              //0x4dc
	H15A0_LINK1_PACKET_38_T            	packet_38;              //0x4e0
	H15A0_LINK1_PACKET_39_T            	packet_39;              //0x4e4
	H15A0_LINK1_PACKET_40_T            	packet_40;              //0x4e8
	H15A0_LINK1_PACKET_41_T            	packet_41;              //0x4ec
	H15A0_LINK1_PACKET_42_T            	packet_42;              //0x4f0
	H15A0_LINK1_PACKET_43_T            	packet_43;              //0x4f4
	H15A0_LINK1_PACKET_44_T            	packet_44;              //0x4f8
	H15A0_LINK1_PACKET_45_T            	packet_45;              //0x4fc
	H15A0_LINK1_PACKET_46_T            	packet_46;              //0x500
	H15A0_LINK1_PACKET_47_T            	packet_47;              //0x504
	H15A0_LINK1_PACKET_48_T            	packet_48;              //0x508
	H15A0_LINK1_PACKET_49_T            	packet_49;              //0x50c
	H15A0_LINK1_PACKET_50_T            	packet_50;              //0x510
	H15A0_LINK1_PACKET_51_T            	packet_51;              //0x514
	H15A0_LINK1_PACKET_52_T            	packet_52;              //0x518
	H15A0_LINK1_PACKET_53_T            	packet_53;              //0x51c
	H15A0_LINK1_PACKET_54_T            	packet_54;              //0x520
	H15A0_LINK1_PACKET_55_T            	packet_55;              //0x524
	H15A0_LINK1_PACKET_56_T            	packet_56;              //0x528
	H15A0_LINK1_PACKET_57_T            	packet_57;              //0x52c
	H15A0_LINK1_PACKET_58_T            	packet_58;              //0x530
	H15A0_LINK1_PACKET_59_T            	packet_59;              //0x534
	H15A0_LINK1_PACKET_60_T            	packet_60;              //0x538
	H15A0_LINK1_PACKET_61_T            	packet_61;              //0x53c
	H15A0_LINK1_PACKET_62_T            	packet_62;              //0x540
	H15A0_LINK1_PACKET_63_T            	packet_63;              //0x544
	H15A0_LINK1_PACKET_64_T            	packet_64;              //0x548
	H15A0_LINK1_PACKET_65_T            	packet_65;              //0x54c
	H15A0_LINK1_PACKET_66_T            	packet_66;              //0x550
	H15A0_LINK1_PACKET_67_T            	packet_67;              //0x554
	H15A0_LINK1_PACKET_68_T            	packet_68;              //0x558
	H15A0_LINK1_PACKET_69_T            	packet_69;              //0x55c
	H15A0_LINK1_PACKET_70_T            	packet_70;              //0x560
	H15A0_LINK1_PACKET_71_T            	packet_71;              //0x564
	H15A0_LINK1_PACKET_72_T            	packet_72;              //0x568
	H15A0_LINK1_PACKET_73_T            	packet_73;              //0x56c
	H15A0_LINK1_PACKET_74_T            	packet_74;              //0x570
	H15A0_LINK1_PACKET_75_T            	packet_75;              //0x574
	H15A0_LINK1_PACKET_76_T            	packet_76;              //0x578
	H15A0_LINK1_PACKET_77_T            	packet_77;              //0x57c
	H15A0_LINK1_PACKET_78_T            	packet_78;              //0x580
	H15A0_LINK1_PACKET_79_T            	packet_79;              //0x584
	H15A0_LINK1_PACKET_80_T            	packet_80;              //0x588
	H15A0_LINK1_PACKET_81_T            	packet_81;              //0x58c
	H15A0_LINK1_PACKET_82_T            	packet_82;              //0x590
	H15A0_LINK1_VIDEO_00_T             	video_00;               //0x594
	H15A0_LINK1_VIDEO_01_T             	video_01;               //0x598
	H15A0_LINK1_VIDEO_02_T             	video_02;               //0x59c
	H15A0_LINK1_VIDEO_03_T             	video_03;               //0x5a0
	H15A0_LINK1_VIDEO_04_T             	video_04;               //0x5a4
	H15A0_LINK1_VIDEO_05_T             	video_05;               //0x5a8
	H15A0_LINK1_VIDEO_06_T             	video_06;               //0x5ac
	H15A0_LINK1_VIDEO_07_T             	video_07;               //0x5b0
	H15A0_LINK1_VIDEO_08_T             	video_08;               //0x5b4
	H15A0_LINK1_VIDEO_09_T             	video_09;               //0x5b8
	H15A0_LINK1_VIDEO_10_T             	video_10;               //0x5bc
	H15A0_LINK1_AUDIO_00_T             	audio_00;               //0x5c0
	H15A0_LINK1_AUDIO_01_T             	audio_01;               //0x5c4
	H15A0_LINK1_AUDIO_02_T             	audio_02;               //0x5c8
	H15A0_LINK1_AUDIO_03_T             	audio_03;               //0x5cc
	H15A0_LINK1_AUDIO_04_T             	audio_04;               //0x5d0
	H15A0_LINK1_AUDIO_05_T             	audio_05;               //0x5d4
	H15A0_LINK1_AUDIO_06_T             	audio_06;               //0x5d8
	H15A0_LINK1_AUDIO_07_T             	audio_07;               //0x5dc
	H15A0_LINK1_AUDIO_08_T             	audio_08;               //0x5e0
	H15A0_LINK1_AUDIO_09_T             	audio_09;               //0x5e4
	H15A0_LINK1_AUDIO_10_T             	audio_10;               //0x5e8
	UINT32                             	reserved02;                   //0x5ec
	H15A0_LINK1_CBUS_00_T              	cbus_00;                //0x5f0
	H15A0_LINK1_CBUS_01_T              	cbus_01;                //0x5f4
	H15A0_LINK1_CBUS_02_T              	cbus_02;                //0x5f8
	H15A0_LINK1_CBUS_03_T              	cbus_03;                //0x5fc
	H15A0_LINK1_CBUS_04_T              	cbus_04;                //0x600
	H15A0_LINK1_CBUS_05_T              	cbus_05;                //0x604
	H15A0_LINK1_CBUS_06_T              	cbus_06;                //0x608
	H15A0_LINK1_CBUS_07_T              	cbus_07;                //0x60c
	H15A0_LINK1_CBUS_08_T              	cbus_08;                //0x610
	H15A0_LINK1_CBUS_09_T              	cbus_09;                //0x614
	H15A0_LINK1_CBUS_10_T              	cbus_10;                //0x618
	H15A0_LINK1_CBUS_11_T              	cbus_11;                //0x61c
	H15A0_LINK1_CBUS_12_T              	cbus_12;                //0x620
	H15A0_LINK1_CBUS_13_T              	cbus_13;                //0x624
	H15A0_LINK1_CBUS_14_T              	cbus_14;                //0x628
	H15A0_LINK1_CBUS_15_T              	cbus_15;                //0x62c
	H15A0_LINK1_CBUS_16_T              	cbus_16;                //0x630
	H15A0_LINK1_CBUS_17_T              	cbus_17;                //0x634
	H15A0_LINK1_CBUS_18_T              	cbus_18;                //0x638
	H15A0_LINK1_CBUS_19_T              	cbus_19;                //0x63c
	H15A0_LINK1_CBUS_20_T              	cbus_20;                //0x640
	H15A0_LINK1_CBUS_21_T              	cbus_21;                //0x644
	H15A0_LINK1_CBUS_22_T              	cbus_22;                //0x648
	H15A0_LINK1_CBUS_23_T              	cbus_23;                //0x64c
	H15A0_LINK1_CBUS_24_T              	cbus_24;                //0x650
	H15A0_LINK1_CBUS_25_T              	cbus_25;                //0x654
	H15A0_LINK1_CBUS_26_T              	cbus_26;                //0x658
	H15A0_LINK1_CBUS_27_T              	cbus_27;                //0x65c
	H15A0_LINK1_CBUS_28_T              	cbus_28;                //0x660
	H15A0_LINK1_CBUS_29_T              	cbus_29;                //0x664
	H15A0_LINK1_CBUS_30_T              	cbus_30;                //0x668
	H15A0_LINK1_CBUS_31_T              	cbus_31;                //0x66c
	H15A0_LINK1_CBUS_32_T              	cbus_32;                //0x670
	H15A0_LINK1_CBUS_33_T              	cbus_33;                //0x674
	H15A0_LINK1_CBUS_34_T              	cbus_34;                //0x678
	H15A0_LINK1_CBUS_35_T              	cbus_35;                //0x67c
	H15A0_LINK1_CBUS_36_T              	cbus_36;                //0x680
	H15A0_LINK1_CBUS_37_T              	cbus_37;                //0x684
	H15A0_LINK1_CBUS_38_T              	cbus_38;                //0x688
	H15A0_LINK1_CBUS_39_T              	cbus_39;                //0x68c
	H15A0_LINK1_CBUS_40_T              	cbus_40;                //0x690
	H15A0_LINK1_CBUS_41_T              	cbus_41;                //0x694
	H15A0_LINK1_CBUS_42_T              	cbus_42;                //0x698
	H15A0_LINK1_EDID_00_T              	edid_00;                //0x69c
	H15A0_LINK1_EDID_01_T              	edid_01;                //0x6a0
	H15A0_LINK1_EDID_02_T              	edid_02;                //0x6a4
	H15A0_LINK1_EDID_03_T              	edid_03;                //0x6a8
	H15A0_LINK1_EDID_04_T              	edid_04;                //0x6ac
	H15A0_LINK1_EDID_05_T              	edid_05;                //0x6b0
	H15A0_LINK1_EDID_06_T              	edid_06;                //0x6b4
	H15A0_LINK1_EDID_07_T              	edid_07;                //0x6b8
	H15A0_LINK1_EDID_08_T              	edid_08;                //0x6bc
	H15A0_LINK1_EDID_09_T              	edid_09;                //0x6c0
	H15A0_LINK1_EDID_10_T              	edid_10;                //0x6c4
	H15A0_LINK1_EDID_11_T              	edid_11;                //0x6c8
	H15A0_LINK1_EDID_12_T              	edid_12;                //0x6cc
	H15A0_LINK1_EDID_13_T              	edid_13;                //0x6d0
	H15A0_LINK1_EDID_14_T              	edid_14;                //0x6d4
	H15A0_LINK1_EDID_15_T              	edid_15;                //0x6d8
	H15A0_LINK1_EDID_16_T              	edid_16;                //0x6dc
	H15A0_LINK1_EDID_17_T              	edid_17;                //0x6e0
	H15A0_LINK1_EDID_18_T              	edid_18;                //0x6e4
	H15A0_LINK1_EDID_19_T              	edid_19;                //0x6e8
	H15A0_LINK1_EDID_20_T              	edid_20;                //0x6ec
	H15A0_LINK1_EDID_21_T              	edid_21;                //0x6f0
	H15A0_LINK1_EDID_22_T              	edid_22;                //0x6f4
	H15A0_LINK1_EDID_23_T              	edid_23;                //0x6f8
	H15A0_LINK1_EDID_24_T              	edid_24;                //0x6fc
	H15A0_LINK1_EDID_25_T              	edid_25;                //0x700
	H15A0_LINK1_EDID_26_T              	edid_26;                //0x704
	H15A0_LINK1_EDID_27_T              	edid_27;                //0x708
	H15A0_LINK1_EDID_28_T              	edid_28;                //0x70c
	H15A0_LINK1_EDID_29_T              	edid_29;                //0x710
	H15A0_LINK1_EDID_30_T              	edid_30;                //0x714
	H15A0_LINK1_EDID_31_T              	edid_31;                //0x718
	H15A0_LINK1_EDID_32_T              	edid_32;                //0x71c
	H15A0_LINK1_EDID_33_T              	edid_33;                //0x720
	H15A0_LINK1_EDID_34_T              	edid_34;                //0x724
	H15A0_LINK1_EDID_35_T              	edid_35;                //0x728
	H15A0_LINK1_EDID_36_T              	edid_36;                //0x72c
	H15A0_LINK1_EDID_37_T              	edid_37;                //0x730
	H15A0_LINK1_EDID_38_T              	edid_38;                //0x734
	H15A0_LINK1_EDID_39_T              	edid_39;                //0x738
	H15A0_LINK1_EDID_40_T              	edid_40;                //0x73c
	H15A0_LINK1_EDID_41_T              	edid_41;                //0x740
	H15A0_LINK1_EDID_42_T              	edid_42;                //0x744
	H15A0_LINK1_EDID_43_T              	edid_43;                //0x748
	H15A0_LINK1_EDID_44_T              	edid_44;                //0x74c
	H15A0_LINK1_EDID_45_T              	edid_45;                //0x750
	H15A0_LINK1_EDID_46_T              	edid_46;                //0x754
	H15A0_LINK1_EDID_47_T              	edid_47;                //0x758
	H15A0_LINK1_EDID_48_T              	edid_48;                //0x75c
	H15A0_LINK1_EDID_49_T              	edid_49;                //0x760
	H15A0_LINK1_EDID_50_T              	edid_50;                //0x764
	H15A0_LINK1_EDID_51_T              	edid_51;                //0x768
	H15A0_LINK1_EDID_52_T              	edid_52;                //0x76c
	H15A0_LINK1_EDID_53_T              	edid_53;                //0x770
	H15A0_LINK1_EDID_54_T              	edid_54;                //0x774
	H15A0_LINK1_EDID_55_T              	edid_55;                //0x778
	H15A0_LINK1_EDID_56_T              	edid_56;                //0x77c
	H15A0_LINK1_EDID_57_T              	edid_57;                //0x780
	H15A0_LINK1_EDID_58_T              	edid_58;                //0x784
	H15A0_LINK1_EDID_59_T              	edid_59;                //0x788
	H15A0_LINK1_EDID_60_T              	edid_60;                //0x78c
	H15A0_LINK1_EDID_61_T              	edid_61;                //0x790
	H15A0_LINK1_EDID_62_T              	edid_62;                //0x794
	H15A0_LINK1_EDID_63_T              	edid_63;                //0x798
}HDMI_LINK1_REG_H15A0_T;
#endif
