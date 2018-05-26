#ifndef _HDMI_RX_LINK_REGISTER_MAP_DW_LINK0_H_
#define _HDMI_RX_LINK_REGISTER_MAP_DW_LINK0_H_
/*-----------------------------------------------------------------------------
		0x0 link0_phy_00 ''
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
} H15A0_LINK0_PHY_00_T;

/*-----------------------------------------------------------------------------
		0x4 link0_phy_01 ''
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
} H15A0_LINK0_PHY_01_T;

/*-----------------------------------------------------------------------------
		0x8 link0_interrupt_enable_00 ''
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
} H15A0_LINK0_INTERRUPT_ENABLE_00_T;

/*-----------------------------------------------------------------------------
		0xc link0_interrupt_enable_01 ''
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
} H15A0_LINK0_INTERRUPT_ENABLE_01_T;

/*-----------------------------------------------------------------------------
		0x10 link0_interrupt_enable_02 ''
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
} H15A0_LINK0_INTERRUPT_ENABLE_02_T;

/*-----------------------------------------------------------------------------
		0x14 link0_interrupt_enable_03 ''
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
} H15A0_LINK0_INTERRUPT_ENABLE_03_T;

/*-----------------------------------------------------------------------------
		0x18 link0_interrupt_00 ''
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
} H15A0_LINK0_INTERRUPT_00_T;

/*-----------------------------------------------------------------------------
		0x1c link0_interrupt_01 ''
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
} H15A0_LINK0_INTERRUPT_01_T;

/*-----------------------------------------------------------------------------
		0x20 link0_interrupt_02 ''
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
} H15A0_LINK0_INTERRUPT_02_T;

/*-----------------------------------------------------------------------------
		0x24 link0_interrupt_03 ''
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
} H15A0_LINK0_INTERRUPT_03_T;

/*-----------------------------------------------------------------------------
		0x28 link0_system_control_00 ''
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
} H15A0_LINK0_SYSTEM_CONTROL_00_T;

/*-----------------------------------------------------------------------------
		0x2c link0_system_control_01 ''
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
} H15A0_LINK0_SYSTEM_CONTROL_01_T;

/*-----------------------------------------------------------------------------
		0x30 link0_hdcp_00 ''
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
} H15A0_LINK0_HDCP_00_T;

/*-----------------------------------------------------------------------------
		0x34 link0_hdcp_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_an_1                      : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_2                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_3                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_4                      : 8 ;  // 31:24     
} H15A0_LINK0_HDCP_01_T;

/*-----------------------------------------------------------------------------
		0x38 link0_hdcp_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_an_5                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_an_6                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_an_7                      : 8 ;  // 31:24     
} H15A0_LINK0_HDCP_02_T;

/*-----------------------------------------------------------------------------
		0x3c link0_hdcp_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_aksv_1                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_aksv_2                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_aksv_3                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_aksv_4                    : 8 ;  // 31:24     
} H15A0_LINK0_HDCP_03_T;

/*-----------------------------------------------------------------------------
		0x40 link0_hdcp_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bksv_0                    : 8 ;  //  7: 0     
	UINT32 reg_hdcp_bksv_1                    : 8 ;  // 15: 8     
	UINT32 reg_hdcp_bksv_2                    : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_3                    : 8 ;  // 31:24     
} H15A0_LINK0_HDCP_04_T;

/*-----------------------------------------------------------------------------
		0x44 link0_hdcp_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdcp_bcaps                     : 8 ;  //  7: 0     
	UINT32 reg_hdcp_ri_0                      : 8 ;  // 15: 8     
	UINT32 reg_hdcp_ri_1                      : 8 ;  // 23:16     
	UINT32 reg_hdcp_bksv_4                    : 8 ;  // 31:24     
} H15A0_LINK0_HDCP_05_T;

/*-----------------------------------------------------------------------------
		0x48 link0_packet_00 ''
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
} H15A0_LINK0_PACKET_00_T;

/*-----------------------------------------------------------------------------
		0x4c link0_packet_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_01_T;

/*-----------------------------------------------------------------------------
		0x50 link0_packet_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_02_T;

/*-----------------------------------------------------------------------------
		0x54 link0_packet_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_03_T;

/*-----------------------------------------------------------------------------
		0x58 link0_packet_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_04_T;

/*-----------------------------------------------------------------------------
		0x5c link0_packet_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_05_T;

/*-----------------------------------------------------------------------------
		0x60 link0_packet_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_06_T;

/*-----------------------------------------------------------------------------
		0x64 link0_packet_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_07_T;

/*-----------------------------------------------------------------------------
		0x68 link0_packet_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_acp_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_acp_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_acp_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_acp_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_08_T;

/*-----------------------------------------------------------------------------
		0x6c link0_packet_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_09_T;

/*-----------------------------------------------------------------------------
		0x70 link0_packet_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_10_T;

/*-----------------------------------------------------------------------------
		0x74 link0_packet_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_11_T;

/*-----------------------------------------------------------------------------
		0x78 link0_packet_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_12_T;

/*-----------------------------------------------------------------------------
		0x7c link0_packet_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_13_T;

/*-----------------------------------------------------------------------------
		0x80 link0_packet_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_14_T;

/*-----------------------------------------------------------------------------
		0x84 link0_packet_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_15_T;

/*-----------------------------------------------------------------------------
		0x88 link0_packet_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_aif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_aif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_aif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_aif_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_16_T;

/*-----------------------------------------------------------------------------
		0x8c link0_packet_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_asp_hdr_0                  : 8 ;  // 23:16     
	UINT32 reg_pkt_asp_hdr_1                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_17_T;

/*-----------------------------------------------------------------------------
		0x90 link0_packet_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_18_T;

/*-----------------------------------------------------------------------------
		0x94 link0_packet_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_19_T;

/*-----------------------------------------------------------------------------
		0x98 link0_packet_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_20_T;

/*-----------------------------------------------------------------------------
		0x9c link0_packet_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_21_T;

/*-----------------------------------------------------------------------------
		0xa0 link0_packet_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_22_T;

/*-----------------------------------------------------------------------------
		0xa4 link0_packet_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_23_T;

/*-----------------------------------------------------------------------------
		0xa8 link0_packet_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_avi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_avi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_avi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_avi_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_24_T;

/*-----------------------------------------------------------------------------
		0xac link0_packet_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_25_T;

/*-----------------------------------------------------------------------------
		0xb0 link0_packet_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_26_T;

/*-----------------------------------------------------------------------------
		0xb4 link0_packet_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_27_T;

/*-----------------------------------------------------------------------------
		0xb8 link0_packet_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_28_T;

/*-----------------------------------------------------------------------------
		0xbc link0_packet_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_29_T;

/*-----------------------------------------------------------------------------
		0xc0 link0_packet_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_30_T;

/*-----------------------------------------------------------------------------
		0xc4 link0_packet_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_31_T;

/*-----------------------------------------------------------------------------
		0xc8 link0_packet_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_gbd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_gbd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_gbd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_gbd_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_32_T;

/*-----------------------------------------------------------------------------
		0xcc link0_packet_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_hbr_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_hbr_hdr_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_gcp_smute                  : 1 ;  //    16     
	UINT32                                    : 8 ;  // 24:17     reserved
	UINT32 reg_pkt_gcp_cmute                  : 1 ;  //    25     
} H15A0_LINK0_PACKET_33_T;

/*-----------------------------------------------------------------------------
		0xd0 link0_packet_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_hdr_1                : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_34_T;

/*-----------------------------------------------------------------------------
		0xd4 link0_packet_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_3                : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_35_T;

/*-----------------------------------------------------------------------------
		0xd8 link0_packet_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_7                : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_36_T;

/*-----------------------------------------------------------------------------
		0xdc link0_packet_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_11               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_37_T;

/*-----------------------------------------------------------------------------
		0xe0 link0_packet_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_15               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_38_T;

/*-----------------------------------------------------------------------------
		0xe4 link0_packet_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_19               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_39_T;

/*-----------------------------------------------------------------------------
		0xe8 link0_packet_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_23               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_40_T;

/*-----------------------------------------------------------------------------
		0xec link0_packet_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc1_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc1_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc1_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc1_dat_27               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_41_T;

/*-----------------------------------------------------------------------------
		0xf0 link0_packet_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_hdr_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_hdr_1                : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_42_T;

/*-----------------------------------------------------------------------------
		0xf4 link0_packet_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_0                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_1                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_2                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_3                : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_43_T;

/*-----------------------------------------------------------------------------
		0xf8 link0_packet_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_4                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_5                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_6                : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_7                : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_44_T;

/*-----------------------------------------------------------------------------
		0xfc link0_packet_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_8                : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_9                : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_10               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_11               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_45_T;

/*-----------------------------------------------------------------------------
		0x100 link0_packet_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_12               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_13               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_14               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_15               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_46_T;

/*-----------------------------------------------------------------------------
		0x104 link0_packet_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_16               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_17               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_18               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_19               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_47_T;

/*-----------------------------------------------------------------------------
		0x108 link0_packet_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_20               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_21               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_22               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_23               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_48_T;

/*-----------------------------------------------------------------------------
		0x10c link0_packet_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_isrc2_dat_24               : 8 ;  //  7: 0     
	UINT32 reg_pkt_isrc2_dat_25               : 8 ;  // 15: 8     
	UINT32 reg_pkt_isrc2_dat_26               : 8 ;  // 23:16     
	UINT32 reg_pkt_isrc2_dat_27               : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_49_T;

/*-----------------------------------------------------------------------------
		0x110 link0_packet_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_50_T;

/*-----------------------------------------------------------------------------
		0x114 link0_packet_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_51_T;

/*-----------------------------------------------------------------------------
		0x118 link0_packet_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_52_T;

/*-----------------------------------------------------------------------------
		0x11c link0_packet_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_53_T;

/*-----------------------------------------------------------------------------
		0x120 link0_packet_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_54_T;

/*-----------------------------------------------------------------------------
		0x124 link0_packet_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_55_T;

/*-----------------------------------------------------------------------------
		0x128 link0_packet_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_56_T;

/*-----------------------------------------------------------------------------
		0x12c link0_packet_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_mif_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_mif_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_mif_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_mif_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_57_T;

/*-----------------------------------------------------------------------------
		0x130 link0_packet_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_58_T;

/*-----------------------------------------------------------------------------
		0x134 link0_packet_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_59_T;

/*-----------------------------------------------------------------------------
		0x138 link0_packet_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_60_T;

/*-----------------------------------------------------------------------------
		0x13c link0_packet_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_61_T;

/*-----------------------------------------------------------------------------
		0x140 link0_packet_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_62_T;

/*-----------------------------------------------------------------------------
		0x144 link0_packet_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_63_T;

/*-----------------------------------------------------------------------------
		0x148 link0_packet_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_64_T;

/*-----------------------------------------------------------------------------
		0x14c link0_packet_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_spd_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_spd_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_spd_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_spd_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_65_T;

/*-----------------------------------------------------------------------------
		0x150 link0_packet_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_0                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_1                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_2                      : 8 ;  // 23:16     
} H15A0_LINK0_PACKET_66_T;

/*-----------------------------------------------------------------------------
		0x154 link0_packet_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_3                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_4                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_5                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_6                      : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_67_T;

/*-----------------------------------------------------------------------------
		0x158 link0_packet_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_7                      : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_8                      : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_9                      : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_10                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_68_T;

/*-----------------------------------------------------------------------------
		0x15c link0_packet_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_11                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_12                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_13                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_14                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_69_T;

/*-----------------------------------------------------------------------------
		0x160 link0_packet_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_15                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_16                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_17                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_18                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_70_T;

/*-----------------------------------------------------------------------------
		0x164 link0_packet_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_19                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_20                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_21                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_22                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_71_T;

/*-----------------------------------------------------------------------------
		0x168 link0_packet_72 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_23                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_24                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_25                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_26                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_72_T;

/*-----------------------------------------------------------------------------
		0x16c link0_packet_73 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_unr_27                     : 8 ;  //  7: 0     
	UINT32 reg_pkt_unr_28                     : 8 ;  // 15: 8     
	UINT32 reg_pkt_unr_29                     : 8 ;  // 23:16     
	UINT32 reg_pkt_unr_30                     : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_73_T;

/*-----------------------------------------------------------------------------
		0x170 link0_packet_74 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_hdr_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_hdr_1                  : 8 ;  // 15: 8     
} H15A0_LINK0_PACKET_74_T;

/*-----------------------------------------------------------------------------
		0x174 link0_packet_75 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_0                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_1                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_2                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_3                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_75_T;

/*-----------------------------------------------------------------------------
		0x178 link0_packet_76 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_4                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_5                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_6                  : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_7                  : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_76_T;

/*-----------------------------------------------------------------------------
		0x17c link0_packet_77 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_8                  : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_9                  : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_10                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_11                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_77_T;

/*-----------------------------------------------------------------------------
		0x180 link0_packet_78 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_12                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_13                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_14                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_15                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_78_T;

/*-----------------------------------------------------------------------------
		0x184 link0_packet_79 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_16                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_17                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_18                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_19                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_79_T;

/*-----------------------------------------------------------------------------
		0x188 link0_packet_80 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_20                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_21                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_22                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_23                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_80_T;

/*-----------------------------------------------------------------------------
		0x18c link0_packet_81 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pkt_vsi_dat_24                 : 8 ;  //  7: 0     
	UINT32 reg_pkt_vsi_dat_25                 : 8 ;  // 15: 8     
	UINT32 reg_pkt_vsi_dat_26                 : 8 ;  // 23:16     
	UINT32 reg_pkt_vsi_dat_27                 : 8 ;  // 31:24     
} H15A0_LINK0_PACKET_81_T;

/*-----------------------------------------------------------------------------
		0x190 link0_packet_82 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_vsi_hdmi_vic               : 8 ;  //  7: 0     
	UINT32 reg_dbg_vsi_3d_struct              : 4 ;  // 11: 8     
	UINT32 reg_dbg_vsi_3d_meta_en             : 1 ;  //    12     
	UINT32 reg_dbg_vsi_3d                     : 1 ;  //    13     
	UINT32 reg_dbg_vsi_4k2k                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_pkt_clr_spd_cnt                : 4 ;  // 19:16     
} H15A0_LINK0_PACKET_82_T;

/*-----------------------------------------------------------------------------
		0x194 link0_video_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pr_tx                          : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_pr_rx                          : 4 ;  // 11: 8     
	UINT32 reg_pr_rx_en                       : 1 ;  //    12     
	UINT32 reg_pr_cmu_sync                    : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_pr_cbcr                        : 4 ;  // 19:16     
} H15A0_LINK0_VIDEO_00_T;

/*-----------------------------------------------------------------------------
		0x198 link0_video_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_align422_tx                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_align422_rx                    : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_align422_rx_en                 : 1 ;  //    12     
	UINT32 reg_align422_swap_rb               : 1 ;  //    13     
} H15A0_LINK0_VIDEO_01_T;

/*-----------------------------------------------------------------------------
		0x19c link0_video_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cmode_tx                       : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cmode_rx                       : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cmode_rx_en                    : 1 ;  //    16     
	UINT32 reg_cmode_ppll_sync                : 1 ;  //    17     
} H15A0_LINK0_VIDEO_02_T;

/*-----------------------------------------------------------------------------
		0x1a0 link0_video_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_neg_pol_en                     : 1 ;  //     0     
	UINT32 reg_inv_hs                         : 1 ;  //     1     
	UINT32 reg_inv_vs                         : 1 ;  //     2     
	UINT32 reg_inv_eo                         : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_vf_clr                         : 1 ;  //     8     
	UINT32 reg_vf_clr_nosync                  : 1 ;  //     9     
} H15A0_LINK0_VIDEO_03_T;

/*-----------------------------------------------------------------------------
		0x1a4 link0_video_04 ''		// register modified 
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_tot                        : 13 ;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_tot                        : 12 ;  // 27:16     
} H15A0_LINK0_VIDEO_04_T;

/*-----------------------------------------------------------------------------
		0x1a8 link0_video_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_av                         : 13 ;  //  12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_av                         : 12 ;  // 27:16     
} H15A0_LINK0_VIDEO_05_T;

/*-----------------------------------------------------------------------------
		0x1ac link0_video_06 ''			// register modified
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_fp                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_v_fp                           : 6 ;  // 21:16     
} H15A0_LINK0_VIDEO_06_T;

/*-----------------------------------------------------------------------------
		0x1b0 link0_video_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_bp                           : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_v_bp                           : 7 ;  // 22:16     
} H15A0_LINK0_VIDEO_07_T;

/*-----------------------------------------------------------------------------
		0x1b4 link0_video_08 ''		// register modified
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_h_sync                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_v_sync                         : 8 ;  // 23:16     
} H15A0_LINK0_VIDEO_08_T;

/*-----------------------------------------------------------------------------
		0x1b8 link0_video_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vid_blank_g                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_vid_blank_r                    : 12;  // 27:16     
} H15A0_LINK0_VIDEO_09_T;

/*-----------------------------------------------------------------------------
		0x1bc link0_video_10 ''
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
} H15A0_LINK0_VIDEO_10_T;

/*-----------------------------------------------------------------------------
		0x1c0 link0_audio_00 ''
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
} H15A0_LINK0_AUDIO_00_T;

/*-----------------------------------------------------------------------------
		0x1c4 link0_audio_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_tx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_tx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_tx_2                     : 4 ;  // 19:16     
} H15A0_LINK0_AUDIO_01_T;

/*-----------------------------------------------------------------------------
		0x1c8 link0_audio_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_n_rx_0                     : 8 ;  //  7: 0     
	UINT32 reg_acr_n_rx_1                     : 8 ;  // 15: 8     
	UINT32 reg_acr_n_rx_2                     : 4 ;  // 19:16     
} H15A0_LINK0_AUDIO_02_T;

/*-----------------------------------------------------------------------------
		0x1cc link0_audio_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_tx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_tx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_tx_2                   : 4 ;  // 19:16     
} H15A0_LINK0_AUDIO_03_T;

/*-----------------------------------------------------------------------------
		0x1d0 link0_audio_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_acr_cts_rx_0                   : 8 ;  //  7: 0     
	UINT32 reg_acr_cts_rx_1                   : 8 ;  // 15: 8     
	UINT32 reg_acr_cts_rx_2                   : 4 ;  // 19:16     
} H15A0_LINK0_AUDIO_04_T;

/*-----------------------------------------------------------------------------
		0x1d4 link0_audio_05 ''
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
} H15A0_LINK0_AUDIO_05_T;

/*-----------------------------------------------------------------------------
		0x1d8 link0_audio_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sp_paerr                       : 1 ;  //     0     
	UINT32 reg_sp_pserr                       : 1 ;  //     1     
	UINT32 reg_sp_gnd_mode                    : 1 ;  //     2     
	UINT32 reg_sp_en                          : 1 ;  //     3     
} H15A0_LINK0_AUDIO_06_T;

/*-----------------------------------------------------------------------------
		0x1dc link0_audio_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_ow_byte1                 : 8 ;  //  7: 0     
	UINT32 reg_achst_ow_bit2                  : 1 ;  //     8     
	UINT32 reg_achst_ow_en                    : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_achst_fs                       : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 reg_achst_byte0                    : 8 ;  // 31:24     
} H15A0_LINK0_AUDIO_07_T;

/*-----------------------------------------------------------------------------
		0x1e0 link0_audio_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_achst_byte1                    : 8 ;  //  7: 0     
	UINT32 reg_achst_byte2                    : 8 ;  // 15: 8     
	UINT32 reg_achst_byte3                    : 8 ;  // 23:16     
	UINT32 reg_achst_byte4                    : 8 ;  // 31:24     
} H15A0_LINK0_AUDIO_08_T;

/*-----------------------------------------------------------------------------
		0x1e4 link0_audio_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_pc_0                     : 8 ;  //  7: 0     
	UINT32 reg_burst_pc_1                     : 8 ;  // 15: 8     
	UINT32 reg_burst_pd_0                     : 8 ;  // 23:16     
	UINT32 reg_burst_pd_1                     : 8 ;  // 31:24     
} H15A0_LINK0_AUDIO_09_T;

/*-----------------------------------------------------------------------------
		0x1e8 link0_audio_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_i2s_ch_mute                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 reg_i2s_len_ovrr                   : 4 ;  // 11: 8     
	UINT32 reg_i2s_len_ovrr_en                : 1 ;  //    12     
	UINT32 reg_burst_auto_clr                 : 1 ;  //    13     
} H15A0_LINK0_AUDIO_10_T;

/*-----------------------------------------------------------------------------
		0x1f0 link0_cbus_00 ''
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
} H15A0_LINK0_CBUS_00_T;

/*-----------------------------------------------------------------------------
		0x1f4 link0_cbus_01 ''
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
} H15A0_LINK0_CBUS_01_T;

/*-----------------------------------------------------------------------------
		0x1f8 link0_cbus_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_thres             : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_n_retry                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_wait_arb_0             : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_wait_arb_1             : 2 ;  // 25:24     
} H15A0_LINK0_CBUS_02_T;

/*-----------------------------------------------------------------------------
		0x1fc link0_cbus_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_nack_cnt               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_src_bit_length         : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_st                     : 4 ;  // 19:16     
	UINT32 reg_cbus_rcv_msg_timeout_0         : 4 ;  // 23:20     
	UINT32 reg_cbus_rcv_msg_timeout_1         : 6 ;  // 29:24     
} H15A0_LINK0_CBUS_03_T;

/*-----------------------------------------------------------------------------
		0x200 link0_cbus_04 ''
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
} H15A0_LINK0_CBUS_04_T;

/*-----------------------------------------------------------------------------
		0x204 link0_cbus_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_hpd             : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_seq_st                         : 8 ;  // 15: 8     
	UINT32 reg_cmd_st                         : 8 ;  // 23:16     
	UINT32 reg_man_msc_wrt_stat_pathen_clr    : 1 ;  //    24     
} H15A0_LINK0_CBUS_05_T;

/*-----------------------------------------------------------------------------
		0x208 link0_cbus_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rd_devcap           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_vdid            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_get_state           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_clr_hpd             : 5 ;  // 28:24     
} H15A0_LINK0_CBUS_06_T;

/*-----------------------------------------------------------------------------
		0x20c link0_cbus_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_3dreq_set  : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_get_msc_err         : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_burst           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_rd_dev_intr_chg     : 5 ;  // 28:24     
} H15A0_LINK0_CBUS_07_T;

/*-----------------------------------------------------------------------------
		0x210 link0_cbus_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_set_intr_dcapchgset : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_set_intr_dscrchgset : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_set_intr_reqwrt_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_grtwrt_set : 5 ;  // 28:24     
} H15A0_LINK0_CBUS_08_T;

/*-----------------------------------------------------------------------------
		0x214 link0_cbus_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_wrt_stat_pathen_clr : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_state_sink_wrt_stat_pathen_set : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_state_sink_wrt_stat_dcaprd_set : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_state_sink_set_intr_edidchgset : 5 ;  // 28:24     
} H15A0_LINK0_CBUS_09_T;

/*-----------------------------------------------------------------------------
		0x218 link0_cbus_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_rcp_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rapk_ecode                  : 8 ;  // 15: 8     
	UINT32 reg_rx_rapk_ecode                  : 8 ;  // 23:16     
	UINT32 reg_man_msg_rap_content_off        : 1 ;  //    24     
	UINT32 reg_man_msg_rap_content_on         : 1 ;  //    25     
	UINT32 reg_man_msg_rap_poll               : 1 ;  //    26     
} H15A0_LINK0_CBUS_10_T;

/*-----------------------------------------------------------------------------
		0x21c link0_cbus_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_rcpk_rtn_kcode              : 8 ;  //  7: 0     
	UINT32 reg_tx_rcpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_rcpk_rtn_kcode              : 8 ;  // 23:16     
	UINT32 reg_man_msg_rcp_cmd                : 1 ;  //    24     
} H15A0_LINK0_CBUS_11_T;

/*-----------------------------------------------------------------------------
		0x220 link0_cbus_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_src_support_rcp            : 1 ;  //     0     
	UINT32 reg_man_src_support_rap            : 1 ;  //     1     
} H15A0_LINK0_CBUS_12_T;

/*-----------------------------------------------------------------------------
		0x224 link0_cbus_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_state_sink_rcp                 : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_state_sink_rap_cnt_off         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_state_sink_rap_cnt_on          : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_state_sink_rap_poll            : 7 ;  // 30:24     
} H15A0_LINK0_CBUS_13_T;

/*-----------------------------------------------------------------------------
		0x228 link0_cbus_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_msc_errcode                 : 8 ;  //  7: 0     
	UINT32 reg_tx_ddc_errcode                 : 8 ;  // 15: 8     
	UINT32 reg_tx_msc_errcode                 : 8 ;  // 23:16     
	UINT32 reg_get_msc_errcode_enable         : 1 ;  //    24     
} H15A0_LINK0_CBUS_14_T;

/*-----------------------------------------------------------------------------
		0x22c link0_cbus_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_adopter_id_h          : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_dev_cat               : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_mhl_version           : 8 ;  // 23:16     
	UINT32 reg_sink_cap_dev_state             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_15_T;

/*-----------------------------------------------------------------------------
		0x230 link0_cbus_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_video_type            : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_aud_link_mode         : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_vid_link_mode         : 8 ;  // 23:16     
	UINT32 reg_sink_cap_adopter_id_l          : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_16_T;

/*-----------------------------------------------------------------------------
		0x234 link0_cbus_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_device_id_h           : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_feature_flag          : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_bandwidth             : 8 ;  // 23:16     
	UINT32 reg_sink_cap_log_dev_map           : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_17_T;

/*-----------------------------------------------------------------------------
		0x238 link0_cbus_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_cap_intr_stat_size        : 8 ;  //  7: 0     
	UINT32 reg_sink_cap_scratchpad_size       : 8 ;  // 15: 8     
	UINT32 reg_sink_cap_device_id_l           : 8 ;  // 23:16     
} H15A0_LINK0_CBUS_18_T;

/*-----------------------------------------------------------------------------
		0x23c link0_cbus_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_intr_reserved_23          : 8 ;  //  7: 0     
	UINT32 reg_sink_intr_reserved_22          : 8 ;  // 15: 8     
	UINT32 reg_sink_intr_dchg_int             : 8 ;  // 23:16     
	UINT32 reg_sink_intr_rchg_int             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_19_T;

/*-----------------------------------------------------------------------------
		0x240 link0_cbus_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_stat_reserved_33          : 8 ;  //  7: 0     
	UINT32 reg_sink_stat_reserved_32          : 8 ;  // 15: 8     
	UINT32 reg_sink_stat_link_mode            : 8 ;  // 23:16     
	UINT32 reg_sink_stat_conn_rdy             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_20_T;

/*-----------------------------------------------------------------------------
		0x244 link0_cbus_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_vendor_id_c1               : 8 ;  //  7: 0     
	UINT32 reg_src_dev_state_c0               : 8 ;  // 15: 8     
	UINT32 reg_sink_vendor_id_80              : 8 ;  // 23:16     
} H15A0_LINK0_CBUS_21_T;

/*-----------------------------------------------------------------------------
		0x248 link0_cbus_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_43             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_42             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_41             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_40             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_22_T;

/*-----------------------------------------------------------------------------
		0x24c link0_cbus_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_47             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_46             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_45             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_44             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_23_T;

/*-----------------------------------------------------------------------------
		0x250 link0_cbus_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4b             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4a             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_49             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_48             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_24_T;

/*-----------------------------------------------------------------------------
		0x254 link0_cbus_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sink_scratchpad_4f             : 8 ;  //  7: 0     
	UINT32 reg_sink_scratchpad_4e             : 8 ;  // 15: 8     
	UINT32 reg_sink_scratchpad_4d             : 8 ;  // 23:16     
	UINT32 reg_sink_scratchpad_4c             : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_25_T;

/*-----------------------------------------------------------------------------
		0x258 link0_cbus_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_adopter_id_l           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_adopter_id_h           : 8 ;  // 15: 8     
	UINT32 reg_src_cap_dev_cat                : 8 ;  // 23:16     
	UINT32 reg_src_cap_mhl_version            : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_26_T;

/*-----------------------------------------------------------------------------
		0x25c link0_cbus_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_feature_flag           : 8 ;  //  7: 0     
	UINT32 reg_src_cap_log_dev_map            : 8 ;  // 15: 8     
	UINT32 reg_src_cap_aud_link_mode          : 8 ;  // 23:16     
	UINT32 reg_src_cap_vid_link_mode          : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_27_T;

/*-----------------------------------------------------------------------------
		0x260 link0_cbus_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_cap_intr_stat_size         : 8 ;  //  7: 0     
	UINT32 reg_src_cap_scratchpad_size        : 8 ;  // 15: 8     
	UINT32 reg_src_cap_device_id_l            : 8 ;  // 23:16     
	UINT32 reg_src_cap_device_id_h            : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_28_T;

/*-----------------------------------------------------------------------------
		0x264 link0_cbus_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_65          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_64          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_63          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_62          : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_29_T;

/*-----------------------------------------------------------------------------
		0x268 link0_cbus_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_69          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_68          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_67          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_66          : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_30_T;

/*-----------------------------------------------------------------------------
		0x26c link0_cbus_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6d          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6c          : 8 ;  // 15: 8     
	UINT32 reg_src_wrt_scratchpad_6b          : 8 ;  // 23:16     
	UINT32 reg_src_wrt_scratchpad_6a          : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_31_T;

/*-----------------------------------------------------------------------------
		0x270 link0_cbus_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_6f          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_6e          : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_next_2              : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_next_1              : 3 ;  // 26:24     
} H15A0_LINK0_CBUS_32_T;

/*-----------------------------------------------------------------------------
		0x274 link0_cbus_33 ''
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
} H15A0_LINK0_CBUS_33_T;

/*-----------------------------------------------------------------------------
		0x278 link0_cbus_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_pulse_width_2_max    : 8 ;  //  7: 0     
	UINT32 reg_cbus_sink_pulse_width_min      : 8 ;  // 15: 8     
	UINT32 reg_cbus_sink_pulse_width_max      : 8 ;  // 23:16     
	UINT32 reg_seq_msc_wrt_burst              : 4 ;  // 27:24     
	UINT32 reg_man_msc_wrt_burst              : 1 ;  //    28     
	UINT32 reg_man_msc_set_intr_reqwrt_set    : 1 ;  //    29     
	UINT32 reg_man_msc_set_intr_dscrch_set    : 1 ;  //    30     
} H15A0_LINK0_CBUS_34_T;

/*-----------------------------------------------------------------------------
		0x27c link0_cbus_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_wake_to_discover_min_2    : 8 ;  //  7: 0     
	UINT32 reg_cbus_wake_to_discover_min_1    : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_cbus_wake_to_discover_max_2    : 8 ;  // 23:16     
	UINT32 reg_cbus_wake_to_discover_max_1    : 2 ;  // 25:24     
} H15A0_LINK0_CBUS_35_T;

/*-----------------------------------------------------------------------------
		0x280 link0_cbus_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_ack_str                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_ack_min                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_ack_max                : 8 ;  // 23:16     
} H15A0_LINK0_CBUS_36_T;

/*-----------------------------------------------------------------------------
		0x284 link0_cbus_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_arb_min                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_arb_max                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_min                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_max                : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_37_T;

/*-----------------------------------------------------------------------------
		0x288 link0_cbus_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_syn0_min               : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_syn0_max               : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_syn1_min               : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_syn1_max               : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_38_T;

/*-----------------------------------------------------------------------------
		0x28c link0_cbus_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cbus_bi_bit_bn1                : 8 ;  //  7: 0     
	UINT32 reg_cbus_bi_bit_bn2                : 8 ;  // 15: 8     
	UINT32 reg_cbus_bi_bit_bn3                : 8 ;  // 23:16     
	UINT32 reg_cbus_bi_bit_bn4                : 8 ;  // 31:24     
} H15A0_LINK0_CBUS_39_T;

/*-----------------------------------------------------------------------------
		0x290 link0_cbus_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_rx_ucpk_rtn_ccode              : 8 ;  //  7: 0     
	UINT32 reg_tx_ucpe_rtn_ecode              : 8 ;  // 15: 8     
	UINT32 reg_tx_ucpk_rtn_ccode              : 8 ;  // 23:16     
	UINT32 reg_state_sink_ucp                 : 7 ;  // 30:24     
} H15A0_LINK0_CBUS_40_T;

/*-----------------------------------------------------------------------------
		0x294 link0_cbus_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_man_msg_ucp_cmd                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_man_src_support_ucp            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_man_msg_ucp_ccode              : 8 ;  // 15: 8     
	UINT32 reg_cbus_abort_err_2               : 8 ;  // 23:16     
	UINT32 reg_cbus_abort_err_1               : 3 ;  // 26:24     
} H15A0_LINK0_CBUS_41_T;

/*-----------------------------------------------------------------------------
		0x298 link0_cbus_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_wrt_scratchpad_61          : 8 ;  //  7: 0     
	UINT32 reg_src_wrt_scratchpad_60          : 8 ;  // 15: 8     
	UINT32 reg_src_scratchpad_num             : 4 ;  // 19:16     
	UINT32 reg_src_scratchpad_id_sel          : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_src_mrc_unsupport              : 1 ;  //    24     
	UINT32 reg_src_mrc_support                : 1 ;  //    25     
} H15A0_LINK0_CBUS_42_T;

/*-----------------------------------------------------------------------------
		0x29c link0_edid_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_255                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_254                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_253                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_252                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_00_T;

/*-----------------------------------------------------------------------------
		0x2a0 link0_edid_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_251                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_250                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_249                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_248                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_01_T;

/*-----------------------------------------------------------------------------
		0x2a4 link0_edid_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_247                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_246                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_245                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_244                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_02_T;

/*-----------------------------------------------------------------------------
		0x2a8 link0_edid_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_243                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_242                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_241                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_240                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_03_T;

/*-----------------------------------------------------------------------------
		0x2ac link0_edid_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_239                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_238                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_237                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_236                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_04_T;

/*-----------------------------------------------------------------------------
		0x2b0 link0_edid_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_235                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_234                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_233                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_232                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_05_T;

/*-----------------------------------------------------------------------------
		0x2b4 link0_edid_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_231                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_230                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_229                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_228                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_06_T;

/*-----------------------------------------------------------------------------
		0x2b8 link0_edid_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_227                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_226                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_225                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_224                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_07_T;

/*-----------------------------------------------------------------------------
		0x2bc link0_edid_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_223                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_222                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_221                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_220                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_08_T;

/*-----------------------------------------------------------------------------
		0x2c0 link0_edid_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_219                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_218                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_217                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_216                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_09_T;

/*-----------------------------------------------------------------------------
		0x2c4 link0_edid_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_215                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_214                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_213                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_212                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_10_T;

/*-----------------------------------------------------------------------------
		0x2c8 link0_edid_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_211                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_210                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_209                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_208                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_11_T;

/*-----------------------------------------------------------------------------
		0x2cc link0_edid_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_207                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_206                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_205                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_204                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_12_T;

/*-----------------------------------------------------------------------------
		0x2d0 link0_edid_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_203                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_202                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_201                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_200                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_13_T;

/*-----------------------------------------------------------------------------
		0x2d4 link0_edid_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_199                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_198                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_197                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_196                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_14_T;

/*-----------------------------------------------------------------------------
		0x2d8 link0_edid_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_195                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_194                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_193                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_192                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_15_T;

/*-----------------------------------------------------------------------------
		0x2dc link0_edid_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_191                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_190                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_189                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_188                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_16_T;

/*-----------------------------------------------------------------------------
		0x2e0 link0_edid_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_187                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_186                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_185                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_184                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_17_T;

/*-----------------------------------------------------------------------------
		0x2e4 link0_edid_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_183                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_182                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_181                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_180                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_18_T;

/*-----------------------------------------------------------------------------
		0x2e8 link0_edid_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_179                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_178                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_177                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_176                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_19_T;

/*-----------------------------------------------------------------------------
		0x2ec link0_edid_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_175                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_174                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_173                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_172                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_20_T;

/*-----------------------------------------------------------------------------
		0x2f0 link0_edid_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_171                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_170                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_169                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_168                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_21_T;

/*-----------------------------------------------------------------------------
		0x2f4 link0_edid_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_167                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_166                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_165                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_164                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_22_T;

/*-----------------------------------------------------------------------------
		0x2f8 link0_edid_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_163                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_162                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_161                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_160                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_23_T;

/*-----------------------------------------------------------------------------
		0x2fc link0_edid_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_159                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_158                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_157                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_156                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_24_T;

/*-----------------------------------------------------------------------------
		0x300 link0_edid_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_155                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_154                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_153                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_152                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_25_T;

/*-----------------------------------------------------------------------------
		0x304 link0_edid_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_151                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_150                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_149                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_148                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_26_T;

/*-----------------------------------------------------------------------------
		0x308 link0_edid_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_147                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_146                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_145                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_144                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_27_T;

/*-----------------------------------------------------------------------------
		0x30c link0_edid_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_143                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_142                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_141                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_140                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_28_T;

/*-----------------------------------------------------------------------------
		0x310 link0_edid_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_139                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_138                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_137                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_136                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_29_T;

/*-----------------------------------------------------------------------------
		0x314 link0_edid_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_135                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_134                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_133                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_132                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_30_T;

/*-----------------------------------------------------------------------------
		0x318 link0_edid_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_131                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_130                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_129                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_128                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_31_T;

/*-----------------------------------------------------------------------------
		0x31c link0_edid_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_127                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_126                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_125                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_124                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_32_T;

/*-----------------------------------------------------------------------------
		0x320 link0_edid_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_123                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_122                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_121                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_120                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_33_T;

/*-----------------------------------------------------------------------------
		0x324 link0_edid_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_119                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_118                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_117                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_116                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_34_T;

/*-----------------------------------------------------------------------------
		0x328 link0_edid_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_115                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_114                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_113                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_112                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_35_T;

/*-----------------------------------------------------------------------------
		0x32c link0_edid_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_111                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_110                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_109                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_108                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_36_T;

/*-----------------------------------------------------------------------------
		0x330 link0_edid_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_107                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_106                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_105                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_104                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_37_T;

/*-----------------------------------------------------------------------------
		0x334 link0_edid_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_103                   : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_102                   : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_101                   : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_100                   : 8 ;  // 31:24     
} H15A0_LINK0_EDID_38_T;

/*-----------------------------------------------------------------------------
		0x338 link0_edid_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_99                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_98                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_97                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_96                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_39_T;

/*-----------------------------------------------------------------------------
		0x33c link0_edid_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_95                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_94                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_93                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_92                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_40_T;

/*-----------------------------------------------------------------------------
		0x340 link0_edid_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_91                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_90                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_89                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_88                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_41_T;

/*-----------------------------------------------------------------------------
		0x344 link0_edid_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_87                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_86                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_85                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_84                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_42_T;

/*-----------------------------------------------------------------------------
		0x348 link0_edid_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_83                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_82                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_81                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_80                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_43_T;

/*-----------------------------------------------------------------------------
		0x34c link0_edid_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_79                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_78                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_77                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_76                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_44_T;

/*-----------------------------------------------------------------------------
		0x350 link0_edid_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_75                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_74                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_73                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_72                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_45_T;

/*-----------------------------------------------------------------------------
		0x354 link0_edid_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_71                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_70                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_69                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_68                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_46_T;

/*-----------------------------------------------------------------------------
		0x358 link0_edid_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_67                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_66                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_65                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_64                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_47_T;

/*-----------------------------------------------------------------------------
		0x35c link0_edid_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_63                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_62                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_61                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_60                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_48_T;

/*-----------------------------------------------------------------------------
		0x360 link0_edid_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_59                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_58                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_57                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_56                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_49_T;

/*-----------------------------------------------------------------------------
		0x364 link0_edid_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_55                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_54                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_53                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_52                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_50_T;

/*-----------------------------------------------------------------------------
		0x368 link0_edid_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_51                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_50                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_49                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_48                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_51_T;

/*-----------------------------------------------------------------------------
		0x36c link0_edid_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_47                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_46                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_45                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_44                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_52_T;

/*-----------------------------------------------------------------------------
		0x370 link0_edid_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_43                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_42                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_41                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_40                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_53_T;

/*-----------------------------------------------------------------------------
		0x374 link0_edid_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_39                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_38                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_37                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_36                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_54_T;

/*-----------------------------------------------------------------------------
		0x378 link0_edid_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_35                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_34                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_33                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_32                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_55_T;

/*-----------------------------------------------------------------------------
		0x37c link0_edid_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_31                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_30                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_29                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_28                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_56_T;

/*-----------------------------------------------------------------------------
		0x380 link0_edid_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_27                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_26                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_25                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_24                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_57_T;

/*-----------------------------------------------------------------------------
		0x384 link0_edid_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_23                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_22                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_21                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_20                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_58_T;

/*-----------------------------------------------------------------------------
		0x388 link0_edid_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_19                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_18                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_17                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_16                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_59_T;

/*-----------------------------------------------------------------------------
		0x38c link0_edid_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_15                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_14                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_13                    : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_12                    : 8 ;  // 31:24     
} H15A0_LINK0_EDID_60_T;

/*-----------------------------------------------------------------------------
		0x390 link0_edid_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_11                    : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_10                    : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_9                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_8                     : 8 ;  // 31:24     
} H15A0_LINK0_EDID_61_T;

/*-----------------------------------------------------------------------------
		0x394 link0_edid_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_7                     : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_6                     : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_5                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_4                     : 8 ;  // 31:24     
} H15A0_LINK0_EDID_62_T;

/*-----------------------------------------------------------------------------
		0x398 link0_edid_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_edid_3                     : 8 ;  //  7: 0     
	UINT32 reg_dbg_edid_2                     : 8 ;  // 15: 8     
	UINT32 reg_dbg_edid_1                     : 8 ;  // 23:16     
	UINT32 reg_dbg_edid_0                     : 8 ;  // 31:24     
} H15A0_LINK0_EDID_63_T;
typedef struct {
	H15A0_LINK0_PHY_00_T               	phy_00;                 //0x0
	H15A0_LINK0_PHY_01_T               	phy_01;                 //0x4
	H15A0_LINK0_INTERRUPT_ENABLE_00_T  	interrupt_enable_00;    //0x8
	H15A0_LINK0_INTERRUPT_ENABLE_01_T  	interrupt_enable_01;    //0xc
	H15A0_LINK0_INTERRUPT_ENABLE_02_T  	interrupt_enable_02;    //0x10
	H15A0_LINK0_INTERRUPT_ENABLE_03_T  	interrupt_enable_03;    //0x14
	H15A0_LINK0_INTERRUPT_00_T         	interrupt_00;           //0x18
	H15A0_LINK0_INTERRUPT_01_T         	interrupt_01;           //0x1c
	H15A0_LINK0_INTERRUPT_02_T         	interrupt_02;           //0x20
	H15A0_LINK0_INTERRUPT_03_T         	interrupt_03;           //0x24
	H15A0_LINK0_SYSTEM_CONTROL_00_T    	system_control_00;      //0x28
	H15A0_LINK0_SYSTEM_CONTROL_01_T    	system_control_01;      //0x2c
	H15A0_LINK0_HDCP_00_T              	hdcp_00;                //0x30
	H15A0_LINK0_HDCP_01_T              	hdcp_01;                //0x34
	H15A0_LINK0_HDCP_02_T              	hdcp_02;                //0x38
	H15A0_LINK0_HDCP_03_T              	hdcp_03;                //0x3c
	H15A0_LINK0_HDCP_04_T              	hdcp_04;                //0x40
	H15A0_LINK0_HDCP_05_T              	hdcp_05;                //0x44
	H15A0_LINK0_PACKET_00_T            	packet_00;              //0x48
	H15A0_LINK0_PACKET_01_T            	packet_01;              //0x4c
	H15A0_LINK0_PACKET_02_T            	packet_02;              //0x50
	H15A0_LINK0_PACKET_03_T            	packet_03;              //0x54
	H15A0_LINK0_PACKET_04_T            	packet_04;              //0x58
	H15A0_LINK0_PACKET_05_T            	packet_05;              //0x5c
	H15A0_LINK0_PACKET_06_T            	packet_06;              //0x60
	H15A0_LINK0_PACKET_07_T            	packet_07;              //0x64
	H15A0_LINK0_PACKET_08_T            	packet_08;              //0x68
	H15A0_LINK0_PACKET_09_T            	packet_09;              //0x6c
	H15A0_LINK0_PACKET_10_T            	packet_10;              //0x70
	H15A0_LINK0_PACKET_11_T            	packet_11;              //0x74
	H15A0_LINK0_PACKET_12_T            	packet_12;              //0x78
	H15A0_LINK0_PACKET_13_T            	packet_13;              //0x7c
	H15A0_LINK0_PACKET_14_T            	packet_14;              //0x80
	H15A0_LINK0_PACKET_15_T            	packet_15;              //0x84
	H15A0_LINK0_PACKET_16_T            	packet_16;              //0x88
	H15A0_LINK0_PACKET_17_T            	packet_17;              //0x8c
	H15A0_LINK0_PACKET_18_T            	packet_18;              //0x90
	H15A0_LINK0_PACKET_19_T            	packet_19;              //0x94
	H15A0_LINK0_PACKET_20_T            	packet_20;              //0x98
	H15A0_LINK0_PACKET_21_T            	packet_21;              //0x9c
	H15A0_LINK0_PACKET_22_T            	packet_22;              //0xa0
	H15A0_LINK0_PACKET_23_T            	packet_23;              //0xa4
	H15A0_LINK0_PACKET_24_T            	packet_24;              //0xa8
	H15A0_LINK0_PACKET_25_T            	packet_25;              //0xac
	H15A0_LINK0_PACKET_26_T            	packet_26;              //0xb0
	H15A0_LINK0_PACKET_27_T            	packet_27;              //0xb4
	H15A0_LINK0_PACKET_28_T            	packet_28;              //0xb8
	H15A0_LINK0_PACKET_29_T            	packet_29;              //0xbc
	H15A0_LINK0_PACKET_30_T            	packet_30;              //0xc0
	H15A0_LINK0_PACKET_31_T            	packet_31;              //0xc4
	H15A0_LINK0_PACKET_32_T            	packet_32;              //0xc8
	H15A0_LINK0_PACKET_33_T            	packet_33;              //0xcc
	H15A0_LINK0_PACKET_34_T            	packet_34;              //0xd0
	H15A0_LINK0_PACKET_35_T            	packet_35;              //0xd4
	H15A0_LINK0_PACKET_36_T            	packet_36;              //0xd8
	H15A0_LINK0_PACKET_37_T            	packet_37;              //0xdc
	H15A0_LINK0_PACKET_38_T            	packet_38;              //0xe0
	H15A0_LINK0_PACKET_39_T            	packet_39;              //0xe4
	H15A0_LINK0_PACKET_40_T            	packet_40;              //0xe8
	H15A0_LINK0_PACKET_41_T            	packet_41;              //0xec
	H15A0_LINK0_PACKET_42_T            	packet_42;              //0xf0
	H15A0_LINK0_PACKET_43_T            	packet_43;              //0xf4
	H15A0_LINK0_PACKET_44_T            	packet_44;              //0xf8
	H15A0_LINK0_PACKET_45_T            	packet_45;              //0xfc
	H15A0_LINK0_PACKET_46_T            	packet_46;              //0x100
	H15A0_LINK0_PACKET_47_T            	packet_47;              //0x104
	H15A0_LINK0_PACKET_48_T            	packet_48;              //0x108
	H15A0_LINK0_PACKET_49_T            	packet_49;              //0x10c
	H15A0_LINK0_PACKET_50_T            	packet_50;              //0x110
	H15A0_LINK0_PACKET_51_T            	packet_51;              //0x114
	H15A0_LINK0_PACKET_52_T            	packet_52;              //0x118
	H15A0_LINK0_PACKET_53_T            	packet_53;              //0x11c
	H15A0_LINK0_PACKET_54_T            	packet_54;              //0x120
	H15A0_LINK0_PACKET_55_T            	packet_55;              //0x124
	H15A0_LINK0_PACKET_56_T            	packet_56;              //0x128
	H15A0_LINK0_PACKET_57_T            	packet_57;              //0x12c
	H15A0_LINK0_PACKET_58_T            	packet_58;              //0x130
	H15A0_LINK0_PACKET_59_T            	packet_59;              //0x134
	H15A0_LINK0_PACKET_60_T            	packet_60;              //0x138
	H15A0_LINK0_PACKET_61_T            	packet_61;              //0x13c
	H15A0_LINK0_PACKET_62_T            	packet_62;              //0x140
	H15A0_LINK0_PACKET_63_T            	packet_63;              //0x144
	H15A0_LINK0_PACKET_64_T            	packet_64;              //0x148
	H15A0_LINK0_PACKET_65_T            	packet_65;              //0x14c
	H15A0_LINK0_PACKET_66_T            	packet_66;              //0x150
	H15A0_LINK0_PACKET_67_T            	packet_67;              //0x154
	H15A0_LINK0_PACKET_68_T            	packet_68;              //0x158
	H15A0_LINK0_PACKET_69_T            	packet_69;              //0x15c
	H15A0_LINK0_PACKET_70_T            	packet_70;              //0x160
	H15A0_LINK0_PACKET_71_T            	packet_71;              //0x164
	H15A0_LINK0_PACKET_72_T            	packet_72;              //0x168
	H15A0_LINK0_PACKET_73_T            	packet_73;              //0x16c
	H15A0_LINK0_PACKET_74_T            	packet_74;              //0x170
	H15A0_LINK0_PACKET_75_T            	packet_75;              //0x174
	H15A0_LINK0_PACKET_76_T            	packet_76;              //0x178
	H15A0_LINK0_PACKET_77_T            	packet_77;              //0x17c
	H15A0_LINK0_PACKET_78_T            	packet_78;              //0x180
	H15A0_LINK0_PACKET_79_T            	packet_79;              //0x184
	H15A0_LINK0_PACKET_80_T            	packet_80;              //0x188
	H15A0_LINK0_PACKET_81_T            	packet_81;              //0x18c
	H15A0_LINK0_PACKET_82_T            	packet_82;              //0x190
	H15A0_LINK0_VIDEO_00_T             	video_00;               //0x194
	H15A0_LINK0_VIDEO_01_T             	video_01;               //0x198
	H15A0_LINK0_VIDEO_02_T             	video_02;               //0x19c
	H15A0_LINK0_VIDEO_03_T             	video_03;               //0x1a0
	H15A0_LINK0_VIDEO_04_T             	video_04;               //0x1a4
	H15A0_LINK0_VIDEO_05_T             	video_05;               //0x1a8
	H15A0_LINK0_VIDEO_06_T             	video_06;               //0x1ac
	H15A0_LINK0_VIDEO_07_T             	video_07;               //0x1b0
	H15A0_LINK0_VIDEO_08_T             	video_08;               //0x1b4
	H15A0_LINK0_VIDEO_09_T             	video_09;               //0x1b8
	H15A0_LINK0_VIDEO_10_T             	video_10;               //0x1bc
	H15A0_LINK0_AUDIO_00_T             	audio_00;               //0x1c0
	H15A0_LINK0_AUDIO_01_T             	audio_01;               //0x1c4
	H15A0_LINK0_AUDIO_02_T             	audio_02;               //0x1c8
	H15A0_LINK0_AUDIO_03_T             	audio_03;               //0x1cc
	H15A0_LINK0_AUDIO_04_T             	audio_04;               //0x1d0
	H15A0_LINK0_AUDIO_05_T             	audio_05;               //0x1d4
	H15A0_LINK0_AUDIO_06_T             	audio_06;               //0x1d8
	H15A0_LINK0_AUDIO_07_T             	audio_07;               //0x1dc
	H15A0_LINK0_AUDIO_08_T             	audio_08;               //0x1e0
	H15A0_LINK0_AUDIO_09_T             	audio_09;               //0x1e4
	H15A0_LINK0_AUDIO_10_T             	audio_10;               //0x1e8
	UINT32                             	reserved00;                   //0x1ec
	H15A0_LINK0_CBUS_00_T              	cbus_00;                //0x1f0
	H15A0_LINK0_CBUS_01_T              	cbus_01;                //0x1f4
	H15A0_LINK0_CBUS_02_T              	cbus_02;                //0x1f8
	H15A0_LINK0_CBUS_03_T              	cbus_03;                //0x1fc
	H15A0_LINK0_CBUS_04_T              	cbus_04;                //0x200
	H15A0_LINK0_CBUS_05_T              	cbus_05;                //0x204
	H15A0_LINK0_CBUS_06_T              	cbus_06;                //0x208
	H15A0_LINK0_CBUS_07_T              	cbus_07;                //0x20c
	H15A0_LINK0_CBUS_08_T              	cbus_08;                //0x210
	H15A0_LINK0_CBUS_09_T              	cbus_09;                //0x214
	H15A0_LINK0_CBUS_10_T              	cbus_10;                //0x218
	H15A0_LINK0_CBUS_11_T              	cbus_11;                //0x21c
	H15A0_LINK0_CBUS_12_T              	cbus_12;                //0x220
	H15A0_LINK0_CBUS_13_T              	cbus_13;                //0x224
	H15A0_LINK0_CBUS_14_T              	cbus_14;                //0x228
	H15A0_LINK0_CBUS_15_T              	cbus_15;                //0x22c
	H15A0_LINK0_CBUS_16_T              	cbus_16;                //0x230
	H15A0_LINK0_CBUS_17_T              	cbus_17;                //0x234
	H15A0_LINK0_CBUS_18_T              	cbus_18;                //0x238
	H15A0_LINK0_CBUS_19_T              	cbus_19;                //0x23c
	H15A0_LINK0_CBUS_20_T              	cbus_20;                //0x240
	H15A0_LINK0_CBUS_21_T              	cbus_21;                //0x244
	H15A0_LINK0_CBUS_22_T              	cbus_22;                //0x248
	H15A0_LINK0_CBUS_23_T              	cbus_23;                //0x24c
	H15A0_LINK0_CBUS_24_T              	cbus_24;                //0x250
	H15A0_LINK0_CBUS_25_T              	cbus_25;                //0x254
	H15A0_LINK0_CBUS_26_T              	cbus_26;                //0x258
	H15A0_LINK0_CBUS_27_T              	cbus_27;                //0x25c
	H15A0_LINK0_CBUS_28_T              	cbus_28;                //0x260
	H15A0_LINK0_CBUS_29_T              	cbus_29;                //0x264
	H15A0_LINK0_CBUS_30_T              	cbus_30;                //0x268
	H15A0_LINK0_CBUS_31_T              	cbus_31;                //0x26c
	H15A0_LINK0_CBUS_32_T              	cbus_32;                //0x270
	H15A0_LINK0_CBUS_33_T              	cbus_33;                //0x274
	H15A0_LINK0_CBUS_34_T              	cbus_34;                //0x278
	H15A0_LINK0_CBUS_35_T              	cbus_35;                //0x27c
	H15A0_LINK0_CBUS_36_T              	cbus_36;                //0x280
	H15A0_LINK0_CBUS_37_T              	cbus_37;                //0x284
	H15A0_LINK0_CBUS_38_T              	cbus_38;                //0x288
	H15A0_LINK0_CBUS_39_T              	cbus_39;                //0x28c
	H15A0_LINK0_CBUS_40_T              	cbus_40;                //0x290
	H15A0_LINK0_CBUS_41_T              	cbus_41;                //0x294
	H15A0_LINK0_CBUS_42_T              	cbus_42;                //0x298
	H15A0_LINK0_EDID_00_T              	edid_00;                //0x29c
	H15A0_LINK0_EDID_01_T              	edid_01;                //0x2a0
	H15A0_LINK0_EDID_02_T              	edid_02;                //0x2a4
	H15A0_LINK0_EDID_03_T              	edid_03;                //0x2a8
	H15A0_LINK0_EDID_04_T              	edid_04;                //0x2ac
	H15A0_LINK0_EDID_05_T              	edid_05;                //0x2b0
	H15A0_LINK0_EDID_06_T              	edid_06;                //0x2b4
	H15A0_LINK0_EDID_07_T              	edid_07;                //0x2b8
	H15A0_LINK0_EDID_08_T              	edid_08;                //0x2bc
	H15A0_LINK0_EDID_09_T              	edid_09;                //0x2c0
	H15A0_LINK0_EDID_10_T              	edid_10;                //0x2c4
	H15A0_LINK0_EDID_11_T              	edid_11;                //0x2c8
	H15A0_LINK0_EDID_12_T              	edid_12;                //0x2cc
	H15A0_LINK0_EDID_13_T              	edid_13;                //0x2d0
	H15A0_LINK0_EDID_14_T              	edid_14;                //0x2d4
	H15A0_LINK0_EDID_15_T              	edid_15;                //0x2d8
	H15A0_LINK0_EDID_16_T              	edid_16;                //0x2dc
	H15A0_LINK0_EDID_17_T              	edid_17;                //0x2e0
	H15A0_LINK0_EDID_18_T              	edid_18;                //0x2e4
	H15A0_LINK0_EDID_19_T              	edid_19;                //0x2e8
	H15A0_LINK0_EDID_20_T              	edid_20;                //0x2ec
	H15A0_LINK0_EDID_21_T              	edid_21;                //0x2f0
	H15A0_LINK0_EDID_22_T              	edid_22;                //0x2f4
	H15A0_LINK0_EDID_23_T              	edid_23;                //0x2f8
	H15A0_LINK0_EDID_24_T              	edid_24;                //0x2fc
	H15A0_LINK0_EDID_25_T              	edid_25;                //0x300
	H15A0_LINK0_EDID_26_T              	edid_26;                //0x304
	H15A0_LINK0_EDID_27_T              	edid_27;                //0x308
	H15A0_LINK0_EDID_28_T              	edid_28;                //0x30c
	H15A0_LINK0_EDID_29_T              	edid_29;                //0x310
	H15A0_LINK0_EDID_30_T              	edid_30;                //0x314
	H15A0_LINK0_EDID_31_T              	edid_31;                //0x318
	H15A0_LINK0_EDID_32_T              	edid_32;                //0x31c
	H15A0_LINK0_EDID_33_T              	edid_33;                //0x320
	H15A0_LINK0_EDID_34_T              	edid_34;                //0x324
	H15A0_LINK0_EDID_35_T              	edid_35;                //0x328
	H15A0_LINK0_EDID_36_T              	edid_36;                //0x32c
	H15A0_LINK0_EDID_37_T              	edid_37;                //0x330
	H15A0_LINK0_EDID_38_T              	edid_38;                //0x334
	H15A0_LINK0_EDID_39_T              	edid_39;                //0x338
	H15A0_LINK0_EDID_40_T              	edid_40;                //0x33c
	H15A0_LINK0_EDID_41_T              	edid_41;                //0x340
	H15A0_LINK0_EDID_42_T              	edid_42;                //0x344
	H15A0_LINK0_EDID_43_T              	edid_43;                //0x348
	H15A0_LINK0_EDID_44_T              	edid_44;                //0x34c
	H15A0_LINK0_EDID_45_T              	edid_45;                //0x350
	H15A0_LINK0_EDID_46_T              	edid_46;                //0x354
	H15A0_LINK0_EDID_47_T              	edid_47;                //0x358
	H15A0_LINK0_EDID_48_T              	edid_48;                //0x35c
	H15A0_LINK0_EDID_49_T              	edid_49;                //0x360
	H15A0_LINK0_EDID_50_T              	edid_50;                //0x364
	H15A0_LINK0_EDID_51_T              	edid_51;                //0x368
	H15A0_LINK0_EDID_52_T              	edid_52;                //0x36c
	H15A0_LINK0_EDID_53_T              	edid_53;                //0x370
	H15A0_LINK0_EDID_54_T              	edid_54;                //0x374
	H15A0_LINK0_EDID_55_T              	edid_55;                //0x378
	H15A0_LINK0_EDID_56_T              	edid_56;                //0x37c
	H15A0_LINK0_EDID_57_T              	edid_57;                //0x380
	H15A0_LINK0_EDID_58_T              	edid_58;                //0x384
	H15A0_LINK0_EDID_59_T              	edid_59;                //0x388
	H15A0_LINK0_EDID_60_T              	edid_60;                //0x38c
	H15A0_LINK0_EDID_61_T              	edid_61;                //0x390
	H15A0_LINK0_EDID_62_T              	edid_62;                //0x394
	H15A0_LINK0_EDID_63_T              	edid_63;                //0x398
} HDMI_LINK0_REG_H15A0_T;
#endif
