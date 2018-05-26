#ifndef _PE_VCP_REG_H15A0_H_
#define _PE_VCP_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc800c000L vcp_de_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_pic_init_y_u1_de              : 1 ;  //     0     
	UINT32 mask_pic_init_c_u1_de              : 1 ;  //     1     
	UINT32 mask_pic_init_y_u2_de              : 1 ;  //     2     
	UINT32 mask_pic_init_c_u2_de              : 1 ;  //     3     
	UINT32 mask_pic_init_y_f1_de              : 1 ;  //     4     
	UINT32 mask_pic_init_c_f1_de              : 1 ;  //     5     
	UINT32 mask_pic_init_y_f2_de              : 1 ;  //     6     
	UINT32 mask_pic_init_c_f2_de              : 1 ;  //     7     
	UINT32 mask_pic_init_y_u1_sr_de           : 1 ;  //     8     
	UINT32 mask_pic_init_ht_de                : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 mask_pic_start_y_u1_de             : 1 ;  //    16     
	UINT32 mask_pic_start_c_u1_de             : 1 ;  //    17     
	UINT32 mask_pic_start_y_u2_de             : 1 ;  //    18     
	UINT32 mask_pic_start_c_u2_de             : 1 ;  //    19     
	UINT32 mask_pic_start_y_f1_de             : 1 ;  //    20     
	UINT32 mask_pic_start_c_f1_de             : 1 ;  //    21     
	UINT32 mask_pic_start_y_f2_de             : 1 ;  //    22     
	UINT32 mask_pic_start_c_f2_de             : 1 ;  //    23     
	UINT32 mask_pic_start_y_u1_sr_de          : 1 ;  //    24     
	UINT32 mask_pic_start_ht_de               : 1 ;  //    25     
} PE_H15A0_VCP_DE_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c004L vcp_de_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_pic_init_y_u1_de               : 4 ;  //  3: 0     
	UINT32 sel_pic_init_c_u1_de               : 4 ;  //  7: 4     
	UINT32 sel_pic_init_y_u2_de               : 4 ;  // 11: 8     
	UINT32 sel_pic_init_c_u2_de               : 4 ;  // 15:12     
	UINT32 sel_pic_init_y_f1_de               : 4 ;  // 19:16     
	UINT32 sel_pic_init_c_f1_de               : 4 ;  // 23:20     
	UINT32 sel_pic_init_y_f2_de               : 4 ;  // 27:24     
	UINT32 sel_pic_init_c_f2_de               : 3 ;  // 30:28     
	UINT32 sel_pic_init_y_u1_sr_de            : 1 ;  //    31     
} PE_H15A0_VCP_DE_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c008L vcp_de_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_pic_init_ht_de                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 pic_init_y_cnt_en                  : 1 ;  //     4     
	UINT32 pic_start_y_cnt_en                 : 1 ;  //     5     
	UINT32 pic_init_c_cnt_en                  : 1 ;  //     6     
	UINT32 pic_start_c_cnt_en                 : 1 ;  //     7     
} PE_H15A0_VCP_DE_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c00cL vcp_de_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_de_sreset                 : 1 ;  //     0     
	UINT32 u1_cdc_c_de_sreset                 : 1 ;  //     1     
	UINT32 u2_cdc_y_de_sreset                 : 1 ;  //     2     
	UINT32 u2_cdc_c_de_sreset                 : 1 ;  //     3     
	UINT32 f1_cdc_y_de_sreset                 : 1 ;  //     4     
	UINT32 f1_cdc_c_de_sreset                 : 1 ;  //     5     
	UINT32 f2_cdc_y_de_sreset                 : 1 ;  //     6     
	UINT32 f2_cdc_c_de_sreset                 : 1 ;  //     7     
	UINT32 u1_cdc_so_y_de_sreset              : 1 ;  //     8     
	UINT32 u1_cdc_so_c_de_sreset              : 1 ;  //     9     
	UINT32 u2_cdc_so_y_de_sreset              : 1 ;  //    10     
	UINT32 u2_cdc_so_c_de_sreset              : 1 ;  //    11     
	UINT32 f1_cdc_so_y_de_sreset              : 1 ;  //    12     
	UINT32 f1_cdc_so_c_de_sreset              : 1 ;  //    13     
	UINT32 f2_cdc_so_y_de_sreset              : 1 ;  //    14     
	UINT32 f2_cdc_so_c_de_sreset              : 1 ;  //    15     
	UINT32 ht_cdc_y_de_sreset                 : 1 ;  //    16     
	UINT32 ht_cdc_c_de_sreset                 : 1 ;  //    17     
} PE_H15A0_VCP_DE_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c010L vcp_cdc_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_rdy                       : 1 ;  //     0     
	UINT32 u1_cdc_y_val                       : 1 ;  //     1     
	UINT32 u1_cdc_c_rdy                       : 1 ;  //     2     
	UINT32 u1_cdc_c_val                       : 1 ;  //     3     
	UINT32 u2_cdc_y_rdy                       : 1 ;  //     4     
	UINT32 u2_cdc_y_val                       : 1 ;  //     5     
	UINT32 u2_cdc_c_rdy                       : 1 ;  //     6     
	UINT32 u2_cdc_c_val                       : 1 ;  //     7     
	UINT32 f1_cdc_y_rdy                       : 1 ;  //     8     
	UINT32 f1_cdc_y_val                       : 1 ;  //     9     
	UINT32 f1_cdc_c_rdy                       : 1 ;  //    10     
	UINT32 f1_cdc_c_val                       : 1 ;  //    11     
	UINT32 f2_cdc_y_rdy                       : 1 ;  //    12     
	UINT32 f2_cdc_y_val                       : 1 ;  //    13     
	UINT32 f2_cdc_c_rdy                       : 1 ;  //    14     
	UINT32 f2_cdc_c_val                       : 1 ;  //    15     
	UINT32 ht_cdc_y_rdy                       : 1 ;  //    16     
	UINT32 ht_cdc_y_val                       : 1 ;  //    17     
	UINT32 ht_cdc_c_rdy                       : 1 ;  //    18     
	UINT32 ht_cdc_c_val                       : 1 ;  //    19     
} PE_H15A0_VCP_DE_CDC_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800c014L vcp_cdc_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_rdy                       : 1 ;  //     0     
	UINT32 u1_cdc_y_val                       : 1 ;  //     1     
	UINT32 u1_cdc_c_rdy                       : 1 ;  //     2     
	UINT32 u1_cdc_c_val                       : 1 ;  //     3     
	UINT32 u2_cdc_y_rdy                       : 1 ;  //     4     
	UINT32 u2_cdc_y_val                       : 1 ;  //     5     
	UINT32 u2_cdc_c_rdy                       : 1 ;  //     6     
	UINT32 u2_cdc_c_val                       : 1 ;  //     7     
	UINT32 f1_cdc_y_rdy                       : 1 ;  //     8     
	UINT32 f1_cdc_y_val                       : 1 ;  //     9     
	UINT32 f1_cdc_c_rdy                       : 1 ;  //    10     
	UINT32 f1_cdc_c_val                       : 1 ;  //    11     
	UINT32 f2_cdc_y_rdy                       : 1 ;  //    12     
	UINT32 f2_cdc_y_val                       : 1 ;  //    13     
	UINT32 f2_cdc_c_rdy                       : 1 ;  //    14     
	UINT32 f2_cdc_c_val                       : 1 ;  //    15     
} PE_H15A0_VCP_DE_CDC_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc800c018L vcp_sg_sry_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_de_last_lcount                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 sr_de_last_pcount                  : 13;  // 28:16     
} PE_H15A0_VCP_SG_SRY_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c01cL vcp_sg_sry_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_sync_enable                     : 1 ;  //     0     
} PE_H15A0_VCP_SG_SRY_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c020L vcp_pic_init_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_y_0_f2_f4_cnt             : 2 ;  //  1: 0     
	UINT32 pic_init_y_1_f1_f3_cnt             : 2 ;  //  3: 2     
	UINT32 pic_init_y_2_u2_cnt                : 2 ;  //  5: 4     
	UINT32 pic_init_y_3_u1_cnt                : 2 ;  //  7: 6     
	UINT32 pic_init_y_4_sr_cnt                : 2 ;  //  9: 8     
	UINT32 pic_init_yc_5_hdmi_tx_cnt          : 2 ;  // 11:10     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pic_start_y_0_f2_f4_cnt            : 2 ;  // 17:16     
	UINT32 pic_start_y_1_f1_f3_cnt            : 2 ;  // 19:18     
	UINT32 pic_start_y_2_u2_cnt               : 2 ;  // 21:20     
	UINT32 pic_start_y_3_u1_cnt               : 2 ;  // 23:22     
	UINT32 pic_start_y_4_sr_cnt               : 2 ;  // 25:24     
	UINT32 pic_start_yc_5_hdmi_tx_cnt         : 2 ;  // 27:26     
} PE_H15A0_VCP_PIC_INIT_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c024L vcp_pic_init_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_init_c_0_f2_f4_cnt             : 2 ;  //  1: 0     
	UINT32 pic_init_c_1_f1_f3_cnt             : 2 ;  //  3: 2     
	UINT32 pic_init_c_2_u2_cnt                : 2 ;  //  5: 4     
	UINT32 pic_init_c_3_u1_cnt                : 2 ;  //  7: 6     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pic_start_c_0_f2_f4_cnt            : 2 ;  // 17:16     
	UINT32 pic_start_c_1_f1_f3_cnt            : 2 ;  // 19:18     
	UINT32 pic_start_c_2_u2_cnt               : 2 ;  // 21:20     
	UINT32 pic_start_c_3_u1_cnt               : 2 ;  // 23:22     
} PE_H15A0_VCP_PIC_INIT_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c028L vcp_sg_sry_ht_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_ht_de_last_lcount               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 sr_ht_de_last_pcount               : 13;  // 28:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c02cL vcp_sg_sry_ht_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_ht_sync_enable                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 sr_ht_sync_sel                     : 1 ;  //     4     
} PE_H15A0_VCP_SG_SRY_HT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c030L vcp_mifu_p2lm_status_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_u2_p2lm_pcnt_y_en              : 1 ;  //     0     
	UINT32 mon_u2_p2lm_pcnt_c_en              : 1 ;  //     1     
	UINT32 mon_u1_p2lm_pcnt_y_en              : 1 ;  //     2     
	UINT32 mon_u1_p2lm_pcnt_c_en              : 1 ;  //     3     
} PE_H15A0_VCP_MIFU_P2LM_STATUS_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c034L vcp_mifu1_p2lm_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_u1_p2lm_pcnt_c                 : 11;  // 10: 0     
} PE_H15A0_VCP_MIFU1_P2LM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800c038L vcp_mifu1_p2lm_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_u1_p2lm_pcnt_y_fsr             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 mon_u1_p2lm_pcnt_y_bsr             : 11;  // 26:16     
} PE_H15A0_VCP_MIFU1_P2LM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc800c03cL vcp_mifu2_p2lm_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_u2_p2lm_pcnt_y                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 mon_u2_p2lm_pcnt_c                 : 11;  // 26:16     
} PE_H15A0_VCP_MIFU2_P2LM_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c040L vcp_mif_val_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_u1y_mif_path_ctrl              : 2 ;  //  1: 0     
	UINT32 set_u1c_mif_path_ctrl              : 2 ;  //  3: 2     
	UINT32 set_u2y_mif_path_ctrl              : 2 ;  //  5: 4     
	UINT32 set_u2c_mif_path_ctrl              : 2 ;  //  7: 6     
	UINT32 set_f1y_mif_path_ctrl              : 2 ;  //  9: 8     
	UINT32 set_f1c_mif_path_ctrl              : 2 ;  // 11:10     
	UINT32 set_f2y_mif_path_ctrl              : 2 ;  // 13:12     
	UINT32 set_f2c_mif_path_ctrl              : 2 ;  // 15:14     
} PE_H15A0_VCP_MIF_VAL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c050L vcp_mif_sreset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 miff2_sreset_c                     : 1 ;  //     0     
	UINT32 miff2_sreset_y                     : 1 ;  //     1     
	UINT32 miff1_sreset_c                     : 1 ;  //     2     
	UINT32 miff1_sreset_y                     : 1 ;  //     3     
	UINT32 mifu2_sreset_c                     : 1 ;  //     4     
	UINT32 mifu2_sreset_y                     : 1 ;  //     5     
	UINT32 mifu1_sreset_c                     : 1 ;  //     6     
	UINT32 mifu1_sreset_bsr_y                 : 1 ;  //     7     
	UINT32 mifu1_sreset_fsr_y                 : 1 ;  //     8     
} PE_H15A0_VCP_MIF_SRESET_T;

/*-----------------------------------------------------------------------------
		0xc800c054L vcp_mifu1_fsr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_y                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_y_empty                     : 1 ;  //    15     
} PE_H15A0_VCP_MIFU1_FSR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c060L vcp_mifu1_fsr_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFU1_FSR_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c064L vcp_mifu1_fsr_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_fsr_win_i_pos_y                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_fsr_win_i_pos_x                : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_FSR_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c068L vcp_mifu1_fsr_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_fsr_win_i_size_y               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_fsr_win_i_size_x               : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_FSR_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c06cL vcp_mifu1_fsr_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_fsr_win_o_pos_y                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_fsr_win_o_pos_x                : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_FSR_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c070L vcp_mifu1_fsr_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_fsr_win_o_size_y               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_fsr_win_o_size_x               : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_FSR_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c080L vcp_mifu1_fsr_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 mif_3d_fpr_on                      : 1 ;  //    16     
	UINT32 mif_3d_fpr_parity                  : 1 ;  //    17     
	UINT32                                    : 13;  // 30:18     reserved
	UINT32 mif_rd_pat_y                       : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c084L vcp_mifu1_fsr_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_lm2p_y_mode                    : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 mif_sr_lm_mode                     : 1 ;  //     4     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c088L vcp_mifu1_fsr_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c08cL vcp_mifu1_fsr_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c090L vcp_mifu1_fsr_rhost_mif_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_y_mode                         : 1 ;  //     0     
} PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc800c094L mon_vcp_mifu1_fsr_fcnt_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFU1_FSR_FCNT_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c098L vcp_mifu1_fsr_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_y                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32                                    : 13;  // 28:16     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_FSR_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c0a4L vcp_mifu1_bsr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_y                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_y_empty                     : 1 ;  //    15     
} PE_H15A0_VCP_MIFU1_BSR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c0b0L vcp_mifu1_bsr_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFU1_BSR_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c0b4L vcp_mifu1_bsr_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_bsr_win_i_pos_y                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_bsr_win_i_pos_x                : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_BSR_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c0b8L vcp_mifu1_bsr_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_bsr_win_i_size_y               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_bsr_win_i_size_x               : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_BSR_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c0bcL vcp_mifu1_bsr_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_bsr_win_o_pos_y                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_bsr_win_o_pos_x                : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_BSR_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c0c0L vcp_mifu1_bsr_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_bsr_win_o_size_y               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_bsr_win_o_size_x               : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1_BSR_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c0d0L vcp_mifu1_bsr_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 fpr_mif_on                         : 1 ;  //    16     
	UINT32 fpr_parity_mif                     : 1 ;  //    17     
	UINT32                                    : 13;  // 30:18     reserved
	UINT32 mif_rd_pat_y                       : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c0d4L vcp_mifu1_bsr_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_lm2p_y_mode                    : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 mif_sr_lm_mode                     : 1 ;  //     4     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c0d8L vcp_mifu1_bsr_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c0dcL vcp_mifu1_bsr_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c0e0L vcp_mifu1_bsr_rhost_mif_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_y_mode                         : 1 ;  //     0     
} PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc800c0e4L mon_vcp_mifu1_bsr_fcnt_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFU1_BSR_FCNT_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c0e8L vcp_mifu1_bsr_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_y                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32                                    : 13;  // 28:16     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1_BSR_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c0f4L vcp_mifu1c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_c                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_c_empty                     : 1 ;  //    15     
} PE_H15A0_VCP_MIFU1C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c100L vcp_mifu1c_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFU1C_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c104L vcp_mifu1c_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1C_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c108L vcp_mifu1c_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1C_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c10cL vcp_mifu1c_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1C_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c110L vcp_mifu1c_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFU1C_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c120L vcp_mifu1c_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 mif_3d_fpr_on                      : 1 ;  //    16     
	UINT32 mif_3d_fpr_parity                  : 1 ;  //    17     
	UINT32                                    : 12;  // 29:18     reserved
	UINT32 mif_rd_pat_c                       : 1 ;  //    30     
} PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c124L vcp_mifu1c_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mif_lm2p_c_mode                    : 2 ;  //  3: 2     
	UINT32                                    : 8 ;  // 11: 4     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c128L vcp_mifu1c_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c12cL vcp_mifu1c_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c130L vcp_mifu1c_rhost_mif_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 mif_c_mode                         : 1 ;  //     1     
} PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc800c134L mon_vcp_mifu1c_fcnt_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFU1C_FCNT_C_T;

/*-----------------------------------------------------------------------------
		0xc800c138L vcp_mifu1c_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_c                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32                                    : 13;  // 28:16     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFU1C_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c144L vcp_mifu2_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_c                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_c_empty                     : 1 ;  //    15     
	UINT32 mon_rhost_lcnt_y                   : 11;  // 26:16     
	UINT32                                    : 4 ;  // 30:27     reserved
	UINT32 mon_lm_y_empty                     : 1 ;  //    31     
} PE_H15A0_VCP_MIFU2_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c150L vcp_mifu2_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFU2_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c154L vcp_mifu2_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFU2_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c158L vcp_mifu2_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFU2_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c15cL vcp_mifu2_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFU2_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c160L vcp_mifu2_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFU2_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c170L vcp_mifu2_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 mif_3d_fpr_on                      : 1 ;  //    16     
	UINT32 mif_3d_fpr_parity                  : 1 ;  //    17     
	UINT32                                    : 12;  // 29:18     reserved
	UINT32 mif_rd_pat_c                       : 1 ;  //    30     
	UINT32 mif_rd_pat_y                       : 1 ;  //    31     
} PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c174L vcp_mifu2_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mif_lm2p_c_mode                    : 2 ;  //  3: 2     
	UINT32                                    : 8 ;  // 11: 4     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c178L vcp_mifu2_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c17cL vcp_mifu2_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c180L vcp_mifu2_rhost_mif_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_y_mode                         : 1 ;  //     0     
	UINT32 mif_c_mode                         : 1 ;  //     1     
} PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc800c184L mon_vcp_mifu2_fcnt_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFU2_FCNT_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c188L mon_vcp_mifu2_fcnt_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFU2_FCNT_C_T;

/*-----------------------------------------------------------------------------
		0xc800c18cL vcp_mifu2_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_c                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32 lm_nfill_ext_y                     : 11;  // 26:16     
	UINT32                                    : 2 ;  // 28:27     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFU2_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c194L vcp_miff1_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_c                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_c_empty                     : 1 ;  //    15     
	UINT32 mon_rhost_lcnt_y                   : 11;  // 26:16     
	UINT32                                    : 4 ;  // 30:27     reserved
	UINT32 mon_lm_y_empty                     : 1 ;  //    31     
} PE_H15A0_VCP_MIFF1_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c1a0L vcp_miff1_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFF1_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c1a4L vcp_miff1_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFF1_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c1a8L vcp_miff1_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFF1_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c1acL vcp_miff1_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFF1_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c1b0L vcp_miff1_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFF1_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c1c0L vcp_miff1_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 mif_3d_fpr_on                      : 1 ;  //    16     
	UINT32 mif_3d_fpr_parity                  : 1 ;  //    17     
	UINT32                                    : 12;  // 29:18     reserved
	UINT32 mif_rd_pat_c                       : 1 ;  //    30     
	UINT32 mif_rd_pat_y                       : 1 ;  //    31     
} PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c1c4L vcp_miff1_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_lm2p_y_mode                    : 2 ;  //  1: 0     
	UINT32 mif_lm2p_c_mode                    : 2 ;  //  3: 2     
	UINT32                                    : 8 ;  // 11: 4     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c1c8L vcp_miff1_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c1ccL vcp_miff1_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c1d0L mon_vcp_miff1_fcnt_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFF1_FCNT_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c1d4L mon_vcp_miff1_fcnt_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFF1_FCNT_C_T;

/*-----------------------------------------------------------------------------
		0xc800c1d8L vcp_miff1_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_c                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32 lm_nfill_ext_y                     : 11;  // 26:16     
	UINT32                                    : 2 ;  // 28:27     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFF1_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c1e4L vcp_miff2_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mon_rhost_lcnt_c                   : 11;  // 10: 0     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 mon_lm_c_empty                     : 1 ;  //    15     
	UINT32 mon_rhost_lcnt_y                   : 11;  // 26:16     
	UINT32                                    : 4 ;  // 30:27     reserved
	UINT32 mon_lm_y_empty                     : 1 ;  //    31     
} PE_H15A0_VCP_MIFF2_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c1f0L vcp_miff2_io_type ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_format_type                      : 4 ;  //  3: 0     
	UINT32 i_format_type                      : 4 ;  //  7: 4     
	UINT32 i_map_type                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_MIFF2_IO_TYPE_T;

/*-----------------------------------------------------------------------------
		0xc800c1f4L vcp_miff2_win_i_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFF2_WIN_I_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c1f8L vcp_miff2_win_i_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_i_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_i_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFF2_WIN_I_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c1fcL vcp_miff2_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_MIFF2_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c200L vcp_miff2_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 mif_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_MIFF2_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c210L vcp_miff2_rhost_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_mif_c_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mif_mif_y_mode                     : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 mif_3d_fpr_on                      : 1 ;  //    16     
	UINT32 mif_3d_fpr_parity                  : 1 ;  //    17     
	UINT32                                    : 12;  // 29:18     reserved
	UINT32 mif_rd_pat_c                       : 1 ;  //    30     
	UINT32 mif_rd_pat_y                       : 1 ;  //    31     
} PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c214L vcp_miff2_rhost_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_lm2p_y_mode                    : 2 ;  //  1: 0     
	UINT32 mif_lm2p_c_mode                    : 2 ;  //  3: 2     
	UINT32                                    : 8 ;  // 11: 4     reserved
	UINT32 mif_lm2p_c_o_type                  : 2 ;  // 13:12     
	UINT32 mif_lm2p_y_o_type                  : 2 ;  // 15:14     
	UINT32 mif_last_mode_y                    : 1 ;  //    16     
	UINT32 mif_last_mode_c                    : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 mif_v_reverse_en                   : 1 ;  //    20     
} PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c218L vcp_miff2_rhost_mif_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_start                   : 12;  // 11: 0     
	UINT32 mif_line_y_inc_dec_offset          : 2 ;  // 13:12     
	UINT32 mif_line_y_inc_dec_sel             : 1 ;  //    14     
	UINT32 mif_line_y_ctrl_en                 : 1 ;  //    15     
	UINT32 mif_line_c_start                   : 12;  // 27:16     
	UINT32 mif_line_cinc_dec_offset           : 2 ;  // 29:28     
	UINT32 mif_line_c_inc_dec_sel             : 1 ;  //    30     
	UINT32 mif_line_c_ctrl_en                 : 1 ;  //    31     
} PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c21cL vcp_miff2_rhost_mif_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mif_line_y_r_start                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mif_line_y_r_ctrl_en               : 1 ;  //    15     
	UINT32 mif_line_c_r_start                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mif_line_c_r_ctrl_en               : 1 ;  //    31     
} PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c220L mon_vcp_miff2_fcnt_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFF2_FCNT_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c224L mon_vcp_miff2_fcnt_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_MON_VCP_MIFF2_FCNT_C_T;

/*-----------------------------------------------------------------------------
		0xc800c228L vcp_miff2_lm_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lm_nfill_ext_c                     : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 lm_nfill_start_line                : 4 ;  // 15:12     
	UINT32 lm_nfill_ext_y                     : 11;  // 26:16     
	UINT32                                    : 2 ;  // 28:27     reserved
	UINT32 lm_fill_cnt_type                   : 1 ;  //    29     
	UINT32 lm_status_intr_en                  : 1 ;  //    30     
	UINT32 lm_status_view_en                  : 1 ;  //    31     
} PE_H15A0_VCP_MIFF2_LM_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c230L vcp_u1_cup_flt_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_en                     : 1 ;  //     0     
} PE_H15A0_VCP_U1_CUP_FLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c234L vcp_u1_cup_flt_coef01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef1                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef0                  : 12;  // 27:16     
} PE_H15A0_VCP_U1_CUP_FLT_COEF01_T;

/*-----------------------------------------------------------------------------
		0xc800c238L vcp_u1_cup_flt_coef23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef3                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef2                  : 12;  // 27:16     
} PE_H15A0_VCP_U1_CUP_FLT_COEF23_T;

/*-----------------------------------------------------------------------------
		0xc800c23cL vcp_u1_cup_flt_coef45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef5                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef4                  : 12;  // 27:16     
} PE_H15A0_VCP_U1_CUP_FLT_COEF45_T;

/*-----------------------------------------------------------------------------
		0xc800c240L vcp_u1_cup_flt_coef67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef7                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef6                  : 12;  // 27:16     
} PE_H15A0_VCP_U1_CUP_FLT_COEF67_T;

/*-----------------------------------------------------------------------------
		0xc800c244L vcp_u1_cup_flt_coef89 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef9                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef8                  : 12;  // 27:16     
} PE_H15A0_VCP_U1_CUP_FLT_COEF89_T;

/*-----------------------------------------------------------------------------
		0xc800c250L vcp_u2_cup_flt_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_en                     : 1 ;  //     0     
} PE_H15A0_VCP_U2_CUP_FLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c254L vcp_u2_cup_flt_coef01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef1                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef0                  : 12;  // 27:16     
} PE_H15A0_VCP_U2_CUP_FLT_COEF01_T;

/*-----------------------------------------------------------------------------
		0xc800c258L vcp_u2_cup_flt_coef23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef3                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef2                  : 12;  // 27:16     
} PE_H15A0_VCP_U2_CUP_FLT_COEF23_T;

/*-----------------------------------------------------------------------------
		0xc800c25cL vcp_u2_cup_flt_coef45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef5                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef4                  : 12;  // 27:16     
} PE_H15A0_VCP_U2_CUP_FLT_COEF45_T;

/*-----------------------------------------------------------------------------
		0xc800c260L vcp_u2_cup_flt_coef67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef7                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef6                  : 12;  // 27:16     
} PE_H15A0_VCP_U2_CUP_FLT_COEF67_T;

/*-----------------------------------------------------------------------------
		0xc800c264L vcp_u2_cup_flt_coef89 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef9                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef8                  : 12;  // 27:16     
} PE_H15A0_VCP_U2_CUP_FLT_COEF89_T;

/*-----------------------------------------------------------------------------
		0xc800c270L vcp_f1_cup_flt_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_en                     : 1 ;  //     0     
} PE_H15A0_VCP_F1_CUP_FLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c274L vcp_f1_cup_flt_coef01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef1                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef0                  : 12;  // 27:16     
} PE_H15A0_VCP_F1_CUP_FLT_COEF01_T;

/*-----------------------------------------------------------------------------
		0xc800c278L vcp_f1_cup_flt_coef23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef3                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef2                  : 12;  // 27:16     
} PE_H15A0_VCP_F1_CUP_FLT_COEF23_T;

/*-----------------------------------------------------------------------------
		0xc800c27cL vcp_f1_cup_flt_coef45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef5                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef4                  : 12;  // 27:16     
} PE_H15A0_VCP_F1_CUP_FLT_COEF45_T;

/*-----------------------------------------------------------------------------
		0xc800c280L vcp_f1_cup_flt_coef67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef7                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef6                  : 12;  // 27:16     
} PE_H15A0_VCP_F1_CUP_FLT_COEF67_T;

/*-----------------------------------------------------------------------------
		0xc800c284L vcp_f1_cup_flt_coef89 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef9                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef8                  : 12;  // 27:16     
} PE_H15A0_VCP_F1_CUP_FLT_COEF89_T;

/*-----------------------------------------------------------------------------
		0xc800c290L vcp_f2_cup_flt_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_en                     : 1 ;  //     0     
} PE_H15A0_VCP_F2_CUP_FLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c294L vcp_f2_cup_flt_coef01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef1                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef0                  : 12;  // 27:16     
} PE_H15A0_VCP_F2_CUP_FLT_COEF01_T;

/*-----------------------------------------------------------------------------
		0xc800c298L vcp_f2_cup_flt_coef23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef3                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef2                  : 12;  // 27:16     
} PE_H15A0_VCP_F2_CUP_FLT_COEF23_T;

/*-----------------------------------------------------------------------------
		0xc800c29cL vcp_f2_cup_flt_coef45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef5                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef4                  : 12;  // 27:16     
} PE_H15A0_VCP_F2_CUP_FLT_COEF45_T;

/*-----------------------------------------------------------------------------
		0xc800c2a0L vcp_f2_cup_flt_coef67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef7                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef6                  : 12;  // 27:16     
} PE_H15A0_VCP_F2_CUP_FLT_COEF67_T;

/*-----------------------------------------------------------------------------
		0xc800c2a4L vcp_f2_cup_flt_coef89 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_cup_flt_coef9                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vcp_cup_flt_coef8                  : 12;  // 27:16     
} PE_H15A0_VCP_F2_CUP_FLT_COEF89_T;

/*-----------------------------------------------------------------------------
		0xc800c2d0L vcp_de_intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_de_intr0_mask                  : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 vcp_de_u1y_fsr_lm_empty_mask       : 1 ;  //     2     
	UINT32 vcp_de_u1y_bsr_lm_empty_mask       : 1 ;  //     3     
	UINT32 vcp_de_u1c_lm_empty_mask           : 1 ;  //     4     
	UINT32 vcp_de_u2y_lm_empty_mask           : 1 ;  //     5     
	UINT32 vcp_de_u2c_lm_empty_mask           : 1 ;  //     6     
	UINT32 vcp_de_f1y_lm_empty_mask           : 1 ;  //     7     
	UINT32 vcp_de_f1c_lm_empty_mask           : 1 ;  //     8     
	UINT32 vcp_de_f2y_lm_empty_mask           : 1 ;  //     9     
	UINT32 vcp_de_f2c_lm_empty_mask           : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vcp_de_u1y_intr_mask               : 1 ;  //    16     
	UINT32 vcp_de_u1c_intr_mask               : 1 ;  //    17     
	UINT32 vcp_de_u2y_intr_mask               : 1 ;  //    18     
	UINT32 vcp_de_u2c_intr_mask               : 1 ;  //    19     
	UINT32 vcp_de_f1y_intr_mask               : 1 ;  //    20     
	UINT32 vcp_de_f1c_intr_mask               : 1 ;  //    21     
	UINT32 vcp_de_f2y_intr_mask               : 1 ;  //    22     
	UINT32 vcp_de_f2c_intr_mask               : 1 ;  //    23     
} PE_H15A0_VCP_DE_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc800c2d4L vcp_de_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_de_intr0_clear                 : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 vcp_de_u1y_fsr_lm_empty_clear      : 1 ;  //     2     
	UINT32 vcp_de_u1y_bsr_lm_empty_clear      : 1 ;  //     3     
	UINT32 vcp_de_u1c_lm_empty_clear          : 1 ;  //     4     
	UINT32 vcp_de_u2y_lm_empty_clear          : 1 ;  //     5     
	UINT32 vcp_de_u2c_lm_empty_clear          : 1 ;  //     6     
	UINT32 vcp_de_f1y_lm_empty_clear          : 1 ;  //     7     
	UINT32 vcp_de_f1c_lm_empty_clear          : 1 ;  //     8     
	UINT32 vcp_de_f2y_lm_empty_clear          : 1 ;  //     9     
	UINT32 vcp_de_f2c_lm_empty_clear          : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vcp_de_u1y_intr_clear              : 1 ;  //    16     
	UINT32 vcp_de_u1c_intr_clear              : 1 ;  //    17     
	UINT32 vcp_de_u2y_intr_clear              : 1 ;  //    18     
	UINT32 vcp_de_u2c_intr_clear              : 1 ;  //    19     
	UINT32 vcp_de_f1y_intr_clear              : 1 ;  //    20     
	UINT32 vcp_de_f1c_intr_clear              : 1 ;  //    21     
	UINT32 vcp_de_f2y_intr_clear              : 1 ;  //    22     
	UINT32 vcp_de_f2c_intr_clear              : 1 ;  //    23     
} PE_H15A0_VCP_DE_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc800c2d8L vcp_de_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_de_intr0_status                : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 vcp_de_u1y_fsr_lm_empty_status     : 1 ;  //     2     
	UINT32 vcp_de_u1y_bsr_lm_empty_status     : 1 ;  //     3     
	UINT32 vcp_de_u1c_lm_empty_status         : 1 ;  //     4     
	UINT32 vcp_de_u2y_lm_empty_status         : 1 ;  //     5     
	UINT32 vcp_de_u2c_lm_empty_status         : 1 ;  //     6     
	UINT32 vcp_de_f1y_lm_empty_status         : 1 ;  //     7     
	UINT32 vcp_de_f1c_lm_empty_status         : 1 ;  //     8     
	UINT32 vcp_de_f2y_lm_empty_status         : 1 ;  //     9     
	UINT32 vcp_de_f2c_lm_empty_status         : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vcp_de_u1y_intr_status             : 1 ;  //    16     
	UINT32 vcp_de_u1c_intr_status             : 1 ;  //    17     
	UINT32 vcp_de_u2y_intr_status             : 1 ;  //    18     
	UINT32 vcp_de_u2c_intr_status             : 1 ;  //    19     
	UINT32 vcp_de_f1y_intr_status             : 1 ;  //    20     
	UINT32 vcp_de_f1c_intr_status             : 1 ;  //    21     
	UINT32 vcp_de_f2y_intr_status             : 1 ;  //    22     
	UINT32 vcp_de_f2c_intr_status             : 1 ;  //    23     
} PE_H15A0_VCP_DE_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c2dcL vcp_de_intr0_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_de_intr0_pos_x                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_de_intr0_pos_y                 : 13;  // 28:16     
} PE_H15A0_VCP_DE_INTR0_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c2e0L vcp_de_intr_src_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_intr0_v_src                    : 1 ;  //     0     
	UINT32 vcp_intr0_h_src                    : 1 ;  //     1     
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 vcp_intr0_enable                   : 1 ;  //     3     
} PE_H15A0_VCP_DE_INTR_SRC_SEL_T;

/*-----------------------------------------------------------------------------
		0xc800c400L vcp_disp_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_pic_init_y_mux_u1_cdc         : 1 ;  //     0     
	UINT32 mask_pic_init_c_mux_u1_cdc         : 1 ;  //     1     
	UINT32 mask_pic_init_y_mux_u2_cdc         : 1 ;  //     2     
	UINT32 mask_pic_init_c_mux_u2_cdc         : 1 ;  //     3     
	UINT32 mask_pic_init_y_mux_f1_cdc         : 1 ;  //     4     
	UINT32 mask_pic_init_c_mux_f1_cdc         : 1 ;  //     5     
	UINT32 mask_pic_init_y_mux_f2_cdc         : 1 ;  //     6     
	UINT32 mask_pic_init_c_mux_f2_cdc         : 1 ;  //     7     
	UINT32 mask_pic_init_y_mux_m_wcp          : 1 ;  //     8     
	UINT32 mask_pic_init_c_mux_m_wcp          : 1 ;  //     9     
	UINT32 mask_pic_init_y_mux_s_wcp          : 1 ;  //    10     
	UINT32 mask_pic_init_c_mux_s_wcp          : 1 ;  //    11     
	UINT32 mask_pic_init_y_mux_ms_wcp         : 1 ;  //    12     
	UINT32 mask_pic_init_c_mux_ms_wcp         : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 mask_pic_start_y_mux_u1_cdc        : 1 ;  //    16     
	UINT32 mask_pic_start_c_mux_u1_cdc        : 1 ;  //    17     
	UINT32 mask_pic_start_y_mux_u2_cdc        : 1 ;  //    18     
	UINT32 mask_pic_start_c_mux_u2_cdc        : 1 ;  //    19     
	UINT32 mask_pic_start_y_mux_f1_cdc        : 1 ;  //    20     
	UINT32 mask_pic_start_c_mux_f1_cdc        : 1 ;  //    21     
	UINT32 mask_pic_start_y_mux_f2_cdc        : 1 ;  //    22     
	UINT32 mask_pic_start_c_mux_f2_cdc        : 1 ;  //    23     
	UINT32 mask_pic_start_y_mux_m_wcp         : 1 ;  //    24     
	UINT32 mask_pic_start_c_mux_m_wcp         : 1 ;  //    25     
	UINT32 mask_pic_start_y_mux_s_wcp         : 1 ;  //    26     
	UINT32 mask_pic_start_c_mux_s_wcp         : 1 ;  //    27     
	UINT32 mask_pic_start_y_mux_ms_wcp        : 1 ;  //    28     
	UINT32 mask_pic_start_c_mux_ms_wcp        : 1 ;  //    29     
} PE_H15A0_VCP_DISP_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c404L vcp_disp_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_DISP_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c408L vcp_disp_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_inverse_3d_lr                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 wcp_m_mix_enable                   : 2 ;  //  5: 4     
} PE_H15A0_VCP_DISP_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c40cL vcp_disp_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_DISP_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc800c410L vcp_cdc_mv_sync_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_sync_sel                  : 2 ;  //  1: 0     
	UINT32 u1_cdc_c_sync_sel                  : 2 ;  //  3: 2     
	UINT32 u2_cdc_y_sync_sel                  : 2 ;  //  5: 4     
	UINT32 u2_cdc_c_sync_sel                  : 2 ;  //  7: 6     
	UINT32 f1_cdc_y_sync_sel                  : 2 ;  //  9: 8     
	UINT32 f1_cdc_c_sync_sel                  : 2 ;  // 11:10     
	UINT32 f2_cdc_y_sync_sel                  : 2 ;  // 13:12     
	UINT32 f2_cdc_c_sync_sel                  : 2 ;  // 15:14     
} PE_H15A0_VCP_CDC_MV_SYNC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c414L vcp_cdc_mv_rst_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_disp_sreset               : 1 ;  //     0     
	UINT32 u1_cdc_c_disp_sreset               : 1 ;  //     1     
	UINT32 u2_cdc_y_disp_sreset               : 1 ;  //     2     
	UINT32 u2_cdc_c_disp_sreset               : 1 ;  //     3     
	UINT32 f1_cdc_y_disp_sreset               : 1 ;  //     4     
	UINT32 f1_cdc_c_disp_sreset               : 1 ;  //     5     
	UINT32 f2_cdc_y_disp_sreset               : 1 ;  //     6     
	UINT32 f2_cdc_c_disp_sreset               : 1 ;  //     7     
} PE_H15A0_VCP_CDC_MV_RST_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c418L vcp_cdc_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_rdy                       : 1 ;  //     0     
	UINT32 u1_cdc_y_val                       : 1 ;  //     1     
	UINT32 u1_cdc_c_rdy                       : 1 ;  //     2     
	UINT32 u1_cdc_c_val                       : 1 ;  //     3     
	UINT32 u2_cdc_y_rdy                       : 1 ;  //     4     
	UINT32 u2_cdc_y_val                       : 1 ;  //     5     
	UINT32 u2_cdc_c_rdy                       : 1 ;  //     6     
	UINT32 u2_cdc_c_val                       : 1 ;  //     7     
	UINT32 f1_cdc_y_rdy                       : 1 ;  //     8     
	UINT32 f1_cdc_y_val                       : 1 ;  //     9     
	UINT32 f1_cdc_c_rdy                       : 1 ;  //    10     
	UINT32 f1_cdc_c_val                       : 1 ;  //    11     
	UINT32 f2_cdc_y_rdy                       : 1 ;  //    12     
	UINT32 f2_cdc_y_val                       : 1 ;  //    13     
	UINT32 f2_cdc_c_rdy                       : 1 ;  //    14     
	UINT32 f2_cdc_c_val                       : 1 ;  //    15     
} PE_H15A0_VCP_DISP_CDC_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800c41cL vcp_cdc_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_y_rdy                       : 1 ;  //     0     
	UINT32 u1_cdc_y_val                       : 1 ;  //     1     
	UINT32 u1_cdc_c_rdy                       : 1 ;  //     2     
	UINT32 u1_cdc_c_val                       : 1 ;  //     3     
	UINT32 u2_cdc_y_rdy                       : 1 ;  //     4     
	UINT32 u2_cdc_y_val                       : 1 ;  //     5     
	UINT32 u2_cdc_c_rdy                       : 1 ;  //     6     
	UINT32 u2_cdc_c_val                       : 1 ;  //     7     
	UINT32 f1_cdc_y_rdy                       : 1 ;  //     8     
	UINT32 f1_cdc_y_val                       : 1 ;  //     9     
	UINT32 f1_cdc_c_rdy                       : 1 ;  //    10     
	UINT32 f1_cdc_c_val                       : 1 ;  //    11     
	UINT32 f2_cdc_y_rdy                       : 1 ;  //    12     
	UINT32 f2_cdc_y_val                       : 1 ;  //    13     
	UINT32 f2_cdc_c_rdy                       : 1 ;  //    14     
	UINT32 f2_cdc_c_val                       : 1 ;  //    15     
} PE_H15A0_VCP_DISP_CDC_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc800c420L vcp_u1_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_U1_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c424L vcp_u1_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_U1_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c428L vcp_u1_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_U1_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c430L vcp_u1_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U1_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c434L vcp_u1_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U1_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c438L vcp_u1_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_u1_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_u1_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_U1_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c43cL vcp_u1_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U1_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c440L vcp_u1_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U1_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c444L vcp_u1_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U1_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c448L vcp_u1_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_u1_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_u1_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_U1_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c44cL vcp_u1_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U1_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c450L vcp_u2_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_U2_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c454L vcp_u2_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_U2_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c458L vcp_u2_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_U2_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c460L vcp_u2_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U2_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c464L vcp_u2_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U2_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c468L vcp_u2_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_u2_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_u2_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_U2_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c46cL vcp_u2_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U2_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c470L vcp_u2_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U2_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c474L vcp_u2_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U2_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c478L vcp_u2_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_u2_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_u2_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_U2_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c47cL vcp_u2_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U2_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c480L vcp_f1_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_F1_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c484L vcp_f1_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_F1_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c488L vcp_f1_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_F1_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c490L vcp_f1_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F1_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c494L vcp_f1_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F1_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c498L vcp_f1_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_f1_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_f1_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_F1_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c49cL vcp_f1_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F1_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c4a0L vcp_f1_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F1_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c4a4L vcp_f1_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F1_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c4a8L vcp_f1_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_f1_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_f1_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_F1_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c4acL vcp_f1_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F1_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c4b0L vcp_f2_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_F2_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800c4b4L vcp_f2_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_F2_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c4b8L vcp_f2_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_F2_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c4c0L vcp_f2_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F2_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c4c4L vcp_f2_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F2_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c4c8L vcp_f2_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_f2_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_f2_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_F2_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c4ccL vcp_f2_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F2_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c4d0L vcp_f2_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F2_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c4d4L vcp_f2_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F2_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c4d8L vcp_f2_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_f2_cdc          : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_f2_cdc         : 4 ;  //  7: 4     
} PE_H15A0_VCP_F2_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800c4dcL vcp_f2_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F2_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c4e0L vcp_sg_mvy_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c4e4L vcp_sg_mvy_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c4e8L vcp_sg_mvy_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c4ecL vcp_sg_mvy_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c4f0L vcp_sg_mvy_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_MVY_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c4f4L vcp_sg_mvy_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c4f8L vcp_sg_mvy_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c4fcL vcp_sg_mvy_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c500L vcp_sg_mvy_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c504L vcp_sg_mvy_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_MVY_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c510L vcp_sg_mvc_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c514L vcp_sg_mvc_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c518L vcp_sg_mvc_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c51cL vcp_sg_mvc_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c520L vcp_sg_mvc_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_MVC_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c524L vcp_sg_mvc_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c528L vcp_sg_mvc_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c52cL vcp_sg_mvc_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c530L vcp_sg_mvc_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c534L vcp_sg_mvc_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_MVC_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c538L vcp_sg_ms_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c53cL vcp_sg_ms_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c540L vcp_sg_ms_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c544L vcp_sg_ms_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c548L vcp_sg_ms_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_MS_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c54cL vcp_sg_ms_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c550L vcp_sg_ms_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c554L vcp_sg_ms_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c558L vcp_sg_ms_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_MS_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c55cL vcp_sg_ms_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_MS_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c560L vcp_sg_svy_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c564L vcp_sg_svy_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c568L vcp_sg_svy_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c56cL vcp_sg_svy_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c570L vcp_sg_svy_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_SVY_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c574L vcp_sg_svy_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c578L vcp_sg_svy_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c57cL vcp_sg_svy_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c580L vcp_sg_svy_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c584L vcp_sg_svy_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_SVY_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c590L vcp_sg_svc_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c594L vcp_sg_svc_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c598L vcp_sg_svc_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c59cL vcp_sg_svc_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c5a0L vcp_sg_svc_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_SVC_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c5a4L vcp_sg_svc_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c5a8L vcp_sg_svc_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c5acL vcp_sg_svc_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c5b0L vcp_sg_svc_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c5b4L vcp_sg_svc_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_SVC_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c5b8L vcp_wcp_m_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pg1ch_win_o_size_y                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pg1ch_win_o_size_x                 : 13;  // 28:16     
} PE_H15A0_VCP_WCP_M_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c5bcL vcp_wcp_m_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lattice_pat_en                     : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 gbrw_pat_en                        : 2 ;  //  3: 2     
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
	UINT32 border_sel                         : 4 ;  // 11: 8     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 status_bar_en                      : 1 ;  //    15     
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 status0_00_src_sel                 : 2 ;  // 21:20     
	UINT32 status0_01_src_sel                 : 2 ;  // 23:22     
	UINT32 status0_02_src_sel                 : 2 ;  // 25:24     
} PE_H15A0_VCP_WCP_M_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c5c0L vcp_wcp_s_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pg1ch_win_o_size_y                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pg1ch_win_o_size_x                 : 13;  // 28:16     
} PE_H15A0_VCP_WCP_S_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c5c4L vcp_wcp_s_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lattice_pat_en                     : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 gbrw_pat_en                        : 2 ;  //  3: 2     
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
	UINT32 border_sel                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_S_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c5c8L vcp_wcp_m_pixel_read_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pel_pos_y                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 rd_pel_pos_x                       : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 rd_pel_pat_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_PIXEL_READ_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c5ccL vcp_wcp_m_pixel_read_data_e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msb8_rd_pel_cr                     : 8 ;  //  7: 0     
	UINT32 msb8_rd_pel_cb                     : 8 ;  // 15: 8     
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32 lsb2_rd_pel_cr                     : 2 ;  // 25:24     
	UINT32 lsb2_rd_pel_cb                     : 2 ;  // 27:26     
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_WCP_M_PIXEL_READ_DATA_E_T;

/*-----------------------------------------------------------------------------
		0xc800c5d0L vcp_wcp_m_line_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 even_line_cr                       : 8 ;  //  7: 0     
	UINT32 even_line_cb                       : 8 ;  // 15: 8     
	UINT32 even_line_y                        : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 even_line_pat_en                   : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_LINE_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c5d4L vcp_wcp_m_line_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 odd_line_cr                        : 8 ;  //  7: 0     
	UINT32 odd_line_cb                        : 8 ;  // 15: 8     
	UINT32 odd_line_y                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 odd_line_pat_en                    : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_LINE_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c5d8L vcp_wcp_s_line_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 even_line_cr                       : 8 ;  //  7: 0     
	UINT32 even_line_cb                       : 8 ;  // 15: 8     
	UINT32 even_line_y                        : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 even_line_pat_en                   : 1 ;  //    31     
} PE_H15A0_VCP_WCP_S_LINE_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c5dcL vcp_wcp_s_line_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 odd_line_cr                        : 8 ;  //  7: 0     
	UINT32 odd_line_cb                        : 8 ;  // 15: 8     
	UINT32 odd_line_y                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 odd_line_pat_en                    : 1 ;  //    31     
} PE_H15A0_VCP_WCP_S_LINE_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c5e0L vcp_wcp_m_rec0_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC0_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c5e4L vcp_wcp_m_rec0_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC0_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c5e8L vcp_wcp_m_rec1_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC1_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c5ecL vcp_wcp_m_rec1_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC1_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c5f0L vcp_wcp_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 compare_lr_diff_sel                : 2 ;  // 25:24     
	UINT32 compare_lr_sync_sel                : 1 ;  //    26     
	UINT32 compare_lr_mark_org_nac            : 1 ;  //    27     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 compare_lr_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c5f4L vcp_wcp_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 compare_lr_th_c                    : 8 ;  //  7: 0     
	UINT32 compare_lr_th_y                    : 8 ;  // 15: 8     
} PE_H15A0_VCP_WCP_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c5f8L vcp_wcp_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 clr_nac_cr                         : 4 ;  //  3: 0     
	UINT32 clr_nac_cb                         : 4 ;  //  7: 4     
	UINT32 clr_nac_y                          : 8 ;  // 15: 8     
	UINT32 clr_bdr_cr                         : 4 ;  // 19:16     
	UINT32 clr_bdr_cb                         : 4 ;  // 23:20     
	UINT32 clr_bdr_y                          : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c5fcL vcp_wcp_m_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w1_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w1_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w1_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w0_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w0_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w0_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_M_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c600L vcp_wcp_m_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w3_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w3_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w3_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w2_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w2_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w2_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_M_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c604L vcp_wcp_m_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_imux_ctrl                   : 3 ;  //  2: 0     
	UINT32 set_w0_dark_en                     : 1 ;  //     3     
	UINT32 set_w1_imux_ctrl                   : 3 ;  //  6: 4     
	UINT32 set_w1_dark_en                     : 1 ;  //     7     
	UINT32 set_w2_imux_ctrl                   : 3 ;  // 10: 8     
	UINT32 set_w2_dark_en                     : 1 ;  //    11     
	UINT32 set_w3_imux_ctrl                   : 3 ;  // 14:12     
	UINT32 set_w3_dark_en                     : 1 ;  //    15     
	UINT32 set_lparity_w0_imux_en             : 1 ;  //    16     
	UINT32 set_lparity_w0_imux                : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 set_lparity_w1_imux_en             : 1 ;  //    20     
	UINT32 set_lparity_w1_imux                : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 set_lparity_sync_c                 : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 set_lparity_sync_y                 : 3 ;  // 30:28     
} PE_H15A0_VCP_WCP_M_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c608L vcp_wcp_m_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_a0_c_imux_ctrl                 : 3 ;  //  2: 0     
	UINT32 set_a0_c_constant_en               : 1 ;  //     3     
	UINT32 set_a1_c_imux_ctrl                 : 3 ;  //  6: 4     
	UINT32 set_a1_c_constant_en               : 1 ;  //     7     
	UINT32 set_a2_c_imux_ctrl                 : 3 ;  // 10: 8     
	UINT32 set_a2_c_constant_en               : 1 ;  //    11     
	UINT32 set_a3_c_imux_ctrl                 : 3 ;  // 14:12     
	UINT32 set_a3_c_constant_en               : 1 ;  //    15     
	UINT32 set_a0_y_imux_ctrl                 : 3 ;  // 18:16     
	UINT32 set_a0_y_constant_en               : 1 ;  //    19     
	UINT32 set_a1_y_imux_ctrl                 : 3 ;  // 22:20     
	UINT32 set_a1_y_constant_en               : 1 ;  //    23     
	UINT32 set_a2_y_imux_ctrl                 : 3 ;  // 26:24     
	UINT32 set_a2_y_constant_en               : 1 ;  //    27     
	UINT32 set_a3_y_imux_ctrl                 : 3 ;  // 30:28     
	UINT32 set_a3_y_constant_en               : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c60cL vcp_wcp_m_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_abr_en                      : 2 ;  //  1: 0     
	UINT32 set_w1_abr_en                      : 2 ;  //  3: 2     
	UINT32 set_w2_abr_en                      : 2 ;  //  5: 4     
	UINT32 set_w3_abr_en                      : 2 ;  //  7: 6     
	UINT32 set_w0_awb_cr                      : 1 ;  //     8     
	UINT32 set_w0_awb_cb                      : 1 ;  //     9     
	UINT32 set_w0_awb_y                       : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 set_w1_awb_cr                      : 1 ;  //    12     
	UINT32 set_w1_awb_cb                      : 1 ;  //    13     
	UINT32 set_w1_awb_y                       : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 set_w2_awb_cr                      : 1 ;  //    16     
	UINT32 set_w2_awb_cb                      : 1 ;  //    17     
	UINT32 set_w2_awb_y                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 set_w3_awb_cr                      : 1 ;  //    20     
	UINT32 set_w3_awb_cb                      : 1 ;  //    21     
	UINT32 set_w3_awb_y                       : 1 ;  //    22     
	UINT32 w1_clr_alpha_nac_en                : 1 ;  //    23     
	UINT32 w1_clr_alpha_nac                   : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_M_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c610L vcp_wcp_m_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_clr_alpha                       : 8 ;  //  7: 0     
	UINT32 w1_clr_alpha                       : 8 ;  // 15: 8     
	UINT32 w2_clr_alpha                       : 8 ;  // 23:16     
	UINT32 w3_clr_alpha                       : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_M_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c614L vcp_wcp_m_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_ha_o_mux_ctrl                  : 4 ;  //  3: 0     
	UINT32 set_va_o_mux_ctrl                  : 4 ;  //  7: 4     
	UINT32 set_de_o_mux_ctrl                  : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_M_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c618L vcp_wcp_m_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_m_wcp           : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_m_wcp          : 4 ;  //  7: 4     
	UINT32 set_pic_init_c_mux_m_wcp           : 4 ;  // 11: 8     
	UINT32 set_pic_start_c_mux_m_wcp          : 4 ;  // 15:12     
} PE_H15A0_VCP_WCP_M_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800c620L vcp_wcp_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 compare_lr_diff_sel                : 2 ;  // 25:24     
	UINT32 compare_lr_sync_sel                : 1 ;  //    26     
	UINT32 compare_lr_mark_org_nac            : 1 ;  //    27     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 compare_lr_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c624L vcp_wcp_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 compare_lr_th_c                    : 8 ;  //  7: 0     
	UINT32 compare_lr_th_y                    : 8 ;  // 15: 8     
} PE_H15A0_VCP_WCP_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c628L vcp_wcp_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 clr_nac_cr                         : 4 ;  //  3: 0     
	UINT32 clr_nac_cb                         : 4 ;  //  7: 4     
	UINT32 clr_nac_y                          : 8 ;  // 15: 8     
	UINT32 clr_bdr_cr                         : 4 ;  // 19:16     
	UINT32 clr_bdr_cb                         : 4 ;  // 23:20     
	UINT32 clr_bdr_y                          : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c62cL vcp_wcp_s_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w1_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w1_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w1_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w0_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w0_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w0_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_S_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c630L vcp_wcp_s_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w3_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w3_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w3_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w2_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w2_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w2_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_S_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c634L vcp_wcp_s_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_imux_ctrl                   : 3 ;  //  2: 0     
	UINT32 set_w0_dark_en                     : 1 ;  //     3     
	UINT32 set_w1_imux_ctrl                   : 3 ;  //  6: 4     
	UINT32 set_w1_dark_en                     : 1 ;  //     7     
	UINT32 set_w2_imux_ctrl                   : 3 ;  // 10: 8     
	UINT32 set_w2_dark_en                     : 1 ;  //    11     
	UINT32 set_w3_imux_ctrl                   : 3 ;  // 14:12     
	UINT32 set_w3_dark_en                     : 1 ;  //    15     
	UINT32 set_lparity_w0_imux_en             : 1 ;  //    16     
	UINT32 set_lparity_w0_imux                : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 set_lparity_w1_imux_en             : 1 ;  //    20     
	UINT32 set_lparity_w1_imux                : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 set_lparity_sync_c                 : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 set_lparity_sync_y                 : 3 ;  // 30:28     
} PE_H15A0_VCP_WCP_S_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c638L vcp_wcp_s_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_a0_c_imux_ctrl                 : 3 ;  //  2: 0     
	UINT32 set_a0_c_constant_en               : 1 ;  //     3     
	UINT32 set_a1_c_imux_ctrl                 : 3 ;  //  6: 4     
	UINT32 set_a1_c_constant_en               : 1 ;  //     7     
	UINT32 set_a2_c_imux_ctrl                 : 3 ;  // 10: 8     
	UINT32 set_a2_c_constant_en               : 1 ;  //    11     
	UINT32 set_a3_c_imux_ctrl                 : 3 ;  // 14:12     
	UINT32 set_a3_c_constant_en               : 1 ;  //    15     
	UINT32 set_a0_y_imux_ctrl                 : 3 ;  // 18:16     
	UINT32 set_a0_y_constant_en               : 1 ;  //    19     
	UINT32 set_a1_y_imux_ctrl                 : 3 ;  // 22:20     
	UINT32 set_a1_y_constant_en               : 1 ;  //    23     
	UINT32 set_a2_y_imux_ctrl                 : 3 ;  // 26:24     
	UINT32 set_a2_y_constant_en               : 1 ;  //    27     
	UINT32 set_a3_y_imux_ctrl                 : 3 ;  // 30:28     
	UINT32 set_a3_y_constant_en               : 1 ;  //    31     
} PE_H15A0_VCP_WCP_S_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c63cL vcp_wcp_s_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_abr_en                      : 2 ;  //  1: 0     
	UINT32 set_w1_abr_en                      : 2 ;  //  3: 2     
	UINT32 set_w2_abr_en                      : 2 ;  //  5: 4     
	UINT32 set_w3_abr_en                      : 2 ;  //  7: 6     
	UINT32 set_w0_awb_cr                      : 1 ;  //     8     
	UINT32 set_w0_awb_cb                      : 1 ;  //     9     
	UINT32 set_w0_awb_y                       : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 set_w1_awb_cr                      : 1 ;  //    12     
	UINT32 set_w1_awb_cb                      : 1 ;  //    13     
	UINT32 set_w1_awb_y                       : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 set_w2_awb_cr                      : 1 ;  //    16     
	UINT32 set_w2_awb_cb                      : 1 ;  //    17     
	UINT32 set_w2_awb_y                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 set_w3_awb_cr                      : 1 ;  //    20     
	UINT32 set_w3_awb_cb                      : 1 ;  //    21     
	UINT32 set_w3_awb_y                       : 1 ;  //    22     
	UINT32 w1_clr_alpha_nac_en                : 1 ;  //    23     
	UINT32 w1_clr_alpha_nac                   : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_S_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c640L vcp_wcp_s_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_clr_alpha                       : 8 ;  //  7: 0     
	UINT32 w1_clr_alpha                       : 8 ;  // 15: 8     
	UINT32 w2_clr_alpha                       : 8 ;  // 23:16     
	UINT32 w3_clr_alpha                       : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_S_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c644L vcp_wcp_s_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_ha_o_mux_ctrl                  : 4 ;  //  3: 0     
	UINT32 set_va_o_mux_ctrl                  : 4 ;  //  7: 4     
	UINT32 set_de_o_mux_ctrl                  : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_S_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c648L vcp_wcp_s_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_s_wcp           : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_s_wcp          : 4 ;  //  7: 4     
	UINT32 set_pic_init_c_mux_s_wcp           : 4 ;  // 11: 8     
	UINT32 set_pic_start_c_mux_s_wcp          : 4 ;  // 15:12     
} PE_H15A0_VCP_WCP_S_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800c650L vcp_wcp_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800c654L vcp_wcp_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800c65cL vcp_wcp_m_pixel_read_data_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msb8_rd_pel_cr                     : 8 ;  //  7: 0     
	UINT32 msb8_rd_pel_cb                     : 8 ;  // 15: 8     
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32 lsb2_rd_pel_cr                     : 2 ;  // 25:24     
	UINT32 lsb2_rd_pel_cb                     : 2 ;  // 27:26     
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_WCP_M_PIXEL_READ_DATA_O_T;

/*-----------------------------------------------------------------------------
		0xc800c660L vcp_wcp_me_lrdiff_cnt_yy_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_ME_LRDIFF_CNT_YY_O_T;

/*-----------------------------------------------------------------------------
		0xc800c664L vcp_wcp_me_lrdiff_cnt_cc_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_ME_LRDIFF_CNT_CC_O_T;

/*-----------------------------------------------------------------------------
		0xc800c668L vcp_wcp_mo_lrdiff_cnt_yy_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_MO_LRDIFF_CNT_YY_O_T;

/*-----------------------------------------------------------------------------
		0xc800c66cL vcp_wcp_mo_lrdiff_cnt_cc_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_MO_LRDIFF_CNT_CC_O_T;

/*-----------------------------------------------------------------------------
		0xc800c670L vcp_wcp_se_lrdiff_cnt_yy_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SE_LRDIFF_CNT_YY_O_T;

/*-----------------------------------------------------------------------------
		0xc800c674L vcp_wcp_se_lrdiff_cnt_cc_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SE_LRDIFF_CNT_CC_O_T;

/*-----------------------------------------------------------------------------
		0xc800c678L vcp_wcp_so_lrdiff_cnt_yy_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SO_LRDIFF_CNT_YY_O_T;

/*-----------------------------------------------------------------------------
		0xc800c67cL vcp_wcp_so_lrdiff_cnt_cc_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SO_LRDIFF_CNT_CC_O_T;

/*-----------------------------------------------------------------------------
		0xc800c680L vcp_sg_bdr_v0_e_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c684L vcp_sg_bdr_v0_e_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c688L vcp_sg_bdr_v0_e_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c690L vcp_sg_bdr_v0_o_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c694L vcp_sg_bdr_v0_o_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c698L vcp_sg_bdr_v0_o_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6a0L vcp_sg_bdr_v1_e_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6a4L vcp_sg_bdr_v1_e_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6a8L vcp_sg_bdr_v1_e_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6b0L vcp_sg_bdr_v1_o_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6b4L vcp_sg_bdr_v1_o_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6b8L vcp_sg_bdr_v1_o_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6c0L vcp_sg_bdr_v2_e_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6c4L vcp_sg_bdr_v2_e_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6c8L vcp_sg_bdr_v2_e_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6d0L vcp_sg_bdr_v2_o_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6d4L vcp_sg_bdr_v2_o_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6d8L vcp_sg_bdr_v2_o_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6e0L vcp_sg_bdr_v3_e_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6e4L vcp_sg_bdr_v3_e_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6e8L vcp_sg_bdr_v3_e_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c6f0L vcp_sg_bdr_v3_o_m_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c6f4L vcp_sg_bdr_v3_o_m_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c6f8L vcp_sg_bdr_v3_o_m_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c700L vcp_sg_bdr_v0_e_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c704L vcp_sg_bdr_v0_e_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c708L vcp_sg_bdr_v0_e_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c710L vcp_sg_bdr_v0_o_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c714L vcp_sg_bdr_v0_o_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c718L vcp_sg_bdr_v0_o_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c720L vcp_sg_bdr_v1_e_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c724L vcp_sg_bdr_v1_e_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c728L vcp_sg_bdr_v1_e_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c730L vcp_sg_bdr_v1_o_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c734L vcp_sg_bdr_v1_o_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c738L vcp_sg_bdr_v1_o_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c740L vcp_sg_bdr_v2_e_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c744L vcp_sg_bdr_v2_e_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c748L vcp_sg_bdr_v2_e_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c750L vcp_sg_bdr_v2_o_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c754L vcp_sg_bdr_v2_o_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c758L vcp_sg_bdr_v2_o_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c760L vcp_sg_bdr_v3_e_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c764L vcp_sg_bdr_v3_e_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c768L vcp_sg_bdr_v3_e_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c770L vcp_sg_bdr_v3_o_s_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c774L vcp_sg_bdr_v3_o_s_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c778L vcp_sg_bdr_v3_o_s_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c780L vcp_sg_bdr_v0_e_ms_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c784L vcp_sg_bdr_v0_e_ms_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c788L vcp_sg_bdr_v0_e_ms_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c790L vcp_sg_bdr_v0_o_ms_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c794L vcp_sg_bdr_v0_o_ms_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c798L vcp_sg_bdr_v0_o_ms_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c7a0L vcp_wcp_ms_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pg1ch_win_o_size_y                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pg1ch_win_o_size_x                 : 13;  // 28:16     
} PE_H15A0_VCP_WCP_MS_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800c7a4L vcp_wcp_ms_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lattice_pat_en                     : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 gbrw_pat_en                        : 2 ;  //  3: 2     
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
	UINT32 border_sel                         : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_MS_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c7a8L vcp_wcp_ms_pixel_read_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pel_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rd_pel_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rd_pel_pat_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_MS_PIXEL_READ_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c7acL vcp_wcp_ms_pixel_read_data_e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_WCP_MS_PIXEL_READ_DATA_E_T;

/*-----------------------------------------------------------------------------
		0xc800c7b0L vcp_wcp_ms_pixel_read_data_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_WCP_MS_PIXEL_READ_DATA_O_T;

/*-----------------------------------------------------------------------------
		0xc800c7b4L vcp_wcp_ms_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 clr_nac_y                          : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 clr_bdr_y                          : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_MS_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c7b8L vcp_wcp_ms_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 w1_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 w0_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_MS_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c7bcL vcp_wcp_ms_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_imux_ctrl                   : 3 ;  //  2: 0     
	UINT32 set_w0_dark_en                     : 1 ;  //     3     
} PE_H15A0_VCP_WCP_MS_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c7c0L vcp_wcp_ms_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_ha_o_mux_ctrl                  : 4 ;  //  3: 0     
	UINT32 set_va_o_mux_ctrl                  : 4 ;  //  7: 4     
} PE_H15A0_VCP_WCP_MS_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c7c4L vcp_wcp_ms_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_ms_wcp          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_ms_wcp         : 4 ;  //  7: 4     
	UINT32 set_pic_init_c_mux_ms_wcp          : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_MS_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800c800L vcp_wcp_alpha_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_alpha_gen_y_path               : 1 ;  //     0     
	UINT32 set_alpha_gen_c_path               : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 set_region_m_y_sync_sel            : 2 ;  //  5: 4     
	UINT32 set_region_m_c_sync_sel            : 2 ;  //  7: 6     
} PE_H15A0_VCP_WCP_ALPHA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800c804L vcp_wcp_alpha_gen_y_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_vsize                          : 16;  // 15: 0     
	UINT32 set_hsize                          : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_0_T;

/*-----------------------------------------------------------------------------
		0xc800c808L vcp_wcp_alpha_gen_y_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 region_shape                       : 2 ;  //  1: 0     
	UINT32 alpha_inversion_y                  : 1 ;  //     2     
	UINT32 eo_mode_en                         : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 alpha_1                            : 8 ;  // 23:16     
	UINT32 alpha_0                            : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_1_T;

/*-----------------------------------------------------------------------------
		0xc800c80cL vcp_wcp_alpha_gen_y_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y0_a_q                             : 16;  // 15: 0     
	UINT32 x0_p                               : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_2_T;

/*-----------------------------------------------------------------------------
		0xc800c810L vcp_wcp_alpha_gen_y_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1_y1_tx_pp                        : 32;  // 31: 0     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_3_T;

/*-----------------------------------------------------------------------------
		0xc800c814L vcp_wcp_alpha_gen_y_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b_c_qq                             : 32;  // 31: 0     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_4_T;

/*-----------------------------------------------------------------------------
		0xc800c818L vcp_wcp_alpha_gen_y_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dy                                 : 16;  // 15: 0     
	UINT32 dx                                 : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_Y_5_T;

/*-----------------------------------------------------------------------------
		0xc800c81cL vcp_wcp_alpha_gen_c_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_vsize                          : 16;  // 15: 0     
	UINT32 set_hsize                          : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_0_T;

/*-----------------------------------------------------------------------------
		0xc800c820L vcp_wcp_alpha_gen_c_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 region_shape                       : 2 ;  //  1: 0     
	UINT32 alpha_inversion_c                  : 1 ;  //     2     
	UINT32 eo_mode_en                         : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 alpha_1                            : 8 ;  // 23:16     
	UINT32 alpha_0                            : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_1_T;

/*-----------------------------------------------------------------------------
		0xc800c824L vcp_wcp_alpha_gen_c_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y0_a_q                             : 16;  // 15: 0     
	UINT32 x0_p                               : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_2_T;

/*-----------------------------------------------------------------------------
		0xc800c828L vcp_wcp_alpha_gen_c_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1_y1_tx_pp                        : 32;  // 31: 0     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_3_T;

/*-----------------------------------------------------------------------------
		0xc800c82cL vcp_wcp_alpha_gen_c_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 b_c_qq                             : 32;  // 31: 0     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_4_T;

/*-----------------------------------------------------------------------------
		0xc800c830L vcp_wcp_alpha_gen_c_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dy                                 : 16;  // 15: 0     
	UINT32 dx                                 : 16;  // 31:16     
} PE_H15A0_VCP_WCP_ALPHA_GEN_C_5_T;

/*-----------------------------------------------------------------------------
		0xc800c834L vcp_wcp_m_rec2_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec2_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec2_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec2_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC2_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c838L vcp_wcp_m_rec2_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec2_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec2_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec2_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC2_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c83cL vcp_wcp_m_rec3_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec3_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec3_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec3_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC3_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c840L vcp_wcp_m_rec3_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec3_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec3_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec3_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC3_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c844L vcp_wcp_m_rec4_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec4_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec4_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec4_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC4_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c848L vcp_wcp_m_rec4_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec4_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec4_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec4_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC4_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c84cL vcp_wcp_m_rec0_o_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC0_O_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c850L vcp_wcp_m_rec0_o_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC0_O_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c854L vcp_wcp_m_rec1_o_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC1_O_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c858L vcp_wcp_m_rec1_o_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC1_O_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c85cL vcp_wcp_m_rec2_o_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec2_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec2_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec2_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC2_O_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c860L vcp_wcp_m_rec2_o_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec2_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec2_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec2_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC2_O_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c864L vcp_wcp_m_rec3_o_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec3_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec3_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec3_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC3_O_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c868L vcp_wcp_m_rec3_o_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec3_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec3_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec3_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC3_O_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c86cL vcp_wcp_m_rec4_o_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec4_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec4_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec4_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC4_O_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800c870L vcp_wcp_m_rec4_o_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec4_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec4_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec4_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_M_REC4_O_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c874L vcp_sg_sry_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800c878L vcp_sg_sry_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800c87cL vcp_sg_sry_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800c880L vcp_sg_sry_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800c884L vcp_sg_sry_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_SRY_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800c888L vcp_sg_sry_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800c88cL vcp_sg_sry_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800c890L vcp_sg_sry_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800c894L vcp_sg_sry_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800c898L vcp_sg_sry_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_SRY_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800c89cL vcp_sg_sry_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sr_de_last_num                     : 13;  // 12: 0     
} PE_H15A0_VCP_SG_SRY_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800c8a0L vcp_sg_mvy_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVY_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800c8a4L vcp_sg_mvc_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_MVC_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800c8a8L vcp_sg_svy_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVY_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800c8acL vcp_sg_svc_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_SVC_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800c8b0L vcp_sg_sry_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800c900L vcp_disp_intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_y_disp_intr0_mask           : 1 ;  //     0     
	UINT32 vcp_mv_y_disp_intr1_mask           : 1 ;  //     1     
	UINT32 vcp_mv_y_w0_intr_mask              : 1 ;  //     2     
	UINT32 vcp_mv_y_w1_intr_mask              : 1 ;  //     3     
	UINT32 vcp_mv_y_w2_intr_mask              : 1 ;  //     4     
	UINT32 vcp_mv_y_w3_intr_mask              : 1 ;  //     5     
	UINT32 vcp_sv_y_disp_intr0_mask           : 1 ;  //     6     
	UINT32 vcp_sv_y_disp_intr1_mask           : 1 ;  //     7     
	UINT32 vcp_sv_y_w0_intr_mask              : 1 ;  //     8     
	UINT32 vcp_sv_y_w1_intr_mask              : 1 ;  //     9     
	UINT32 vcp_sv_y_w2_intr_mask              : 1 ;  //    10     
	UINT32 vcp_sv_y_w3_intr_mask              : 1 ;  //    11     
	UINT32 vcp_ms_y_disp_intr_mask            : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_c_disp_intr0_mask           : 1 ;  //    16     
	UINT32 vcp_mv_c_disp_intr1_mask           : 1 ;  //    17     
	UINT32 vcp_mv_c_w0_intr_mask              : 1 ;  //    18     
	UINT32 vcp_mv_c_w1_intr_mask              : 1 ;  //    19     
	UINT32 vcp_mv_c_w2_intr_mask              : 1 ;  //    20     
	UINT32 vcp_mv_c_w3_intr_mask              : 1 ;  //    21     
	UINT32 vcp_sv_c_disp_intr0_mask           : 1 ;  //    22     
	UINT32 vcp_sv_c_disp_intr1_mask           : 1 ;  //    23     
	UINT32 vcp_sv_c_w0_intr_mask              : 1 ;  //    24     
	UINT32 vcp_sv_c_w1_intr_mask              : 1 ;  //    25     
	UINT32 vcp_sv_c_w2_intr_mask              : 1 ;  //    26     
	UINT32 vcp_sv_c_w3_intr_mask              : 1 ;  //    27     
} PE_H15A0_VCP_DISP_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc800c904L vcp_disp_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_y_disp_intr0_clear          : 1 ;  //     0     
	UINT32 vcp_mv_y_disp_intr1_clear          : 1 ;  //     1     
	UINT32 vcp_mv_y_w0_intr_clear             : 1 ;  //     2     
	UINT32 vcp_mv_y_w1_intr_clear             : 1 ;  //     3     
	UINT32 vcp_mv_y_w2_intr_clear             : 1 ;  //     4     
	UINT32 vcp_mv_y_w3_intr_clear             : 1 ;  //     5     
	UINT32 vcp_sv_y_disp_intr0_clear          : 1 ;  //     6     
	UINT32 vcp_sv_y_disp_intr1_clear          : 1 ;  //     7     
	UINT32 vcp_sv_y_w0_intr_clear             : 1 ;  //     8     
	UINT32 vcp_sv_y_w1_intr_clear             : 1 ;  //     9     
	UINT32 vcp_sv_y_w2_intr_clear             : 1 ;  //    10     
	UINT32 vcp_sv_y_w3_intr_clear             : 1 ;  //    11     
	UINT32 vcp_ms_y_disp_intr_clear           : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_c_disp_intr0_clear          : 1 ;  //    16     
	UINT32 vcp_mv_c_disp_intr1_clear          : 1 ;  //    17     
	UINT32 vcp_mv_c_w0_intr_clear             : 1 ;  //    18     
	UINT32 vcp_mv_c_w1_intr_clear             : 1 ;  //    19     
	UINT32 vcp_mv_c_w2_intr_clear             : 1 ;  //    20     
	UINT32 vcp_mv_c_w3_intr_clear             : 1 ;  //    21     
	UINT32 vcp_sv_c_disp_intr0_clear          : 1 ;  //    22     
	UINT32 vcp_sv_c_disp_intr1_clear          : 1 ;  //    23     
	UINT32 vcp_sv_c_w0_intr_clear             : 1 ;  //    24     
	UINT32 vcp_sv_c_w1_intr_clear             : 1 ;  //    25     
	UINT32 vcp_sv_c_w2_intr_clear             : 1 ;  //    26     
	UINT32 vcp_sv_c_w3_intr_clear             : 1 ;  //    27     
} PE_H15A0_VCP_DISP_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc800c908L vcp_disp_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_y_disp_intr0_status         : 1 ;  //     0     
	UINT32 vcp_mv_y_disp_intr1_status         : 1 ;  //     1     
	UINT32 vcp_mv_y_w0_intr_status            : 1 ;  //     2     
	UINT32 vcp_mv_y_w1_intr_status            : 1 ;  //     3     
	UINT32 vcp_mv_y_w2_intr_status            : 1 ;  //     4     
	UINT32 vcp_mv_y_w3_intr_status            : 1 ;  //     5     
	UINT32 vcp_sv_y_disp_intr0_status         : 1 ;  //     6     
	UINT32 vcp_sv_y_disp_intr1_status         : 1 ;  //     7     
	UINT32 vcp_sv_y_w0_intr_status            : 1 ;  //     8     
	UINT32 vcp_sv_y_w1_intr_status            : 1 ;  //     9     
	UINT32 vcp_sv_y_w2_intr_status            : 1 ;  //    10     
	UINT32 vcp_sv_y_w3_intr_status            : 1 ;  //    11     
	UINT32 vcp_ms_y_disp_intr_status          : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_c_disp_intr0_status         : 1 ;  //    16     
	UINT32 vcp_mv_c_disp_intr1_status         : 1 ;  //    17     
	UINT32 vcp_mv_c_w0_intr_status            : 1 ;  //    18     
	UINT32 vcp_mv_c_w1_intr_status            : 1 ;  //    19     
	UINT32 vcp_mv_c_w2_intr_status            : 1 ;  //    20     
	UINT32 vcp_mv_c_w3_intr_status            : 1 ;  //    21     
	UINT32 vcp_sv_c_disp_intr0_status         : 1 ;  //    22     
	UINT32 vcp_sv_c_disp_intr1_status         : 1 ;  //    23     
	UINT32 vcp_sv_c_w0_intr_status            : 1 ;  //    24     
	UINT32 vcp_sv_c_w1_intr_status            : 1 ;  //    25     
	UINT32 vcp_sv_c_w2_intr_status            : 1 ;  //    26     
	UINT32 vcp_sv_c_w3_intr_status            : 1 ;  //    27     
} PE_H15A0_VCP_DISP_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800c90cL vcp_disp_intr0_pos_mv_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_disp_intr0_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_disp_intr0_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR0_POS_MV_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c910L vcp_disp_intr1_pos_mv_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_disp_intr1_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_disp_intr1_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR1_POS_MV_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c914L vcp_disp_intr0_pos_mv_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_disp_intr0_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_disp_intr0_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR0_POS_MV_C_T;

/*-----------------------------------------------------------------------------
		0xc800c918L vcp_disp_intr1_pos_mv_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_mv_disp_intr1_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_mv_disp_intr1_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR1_POS_MV_C_T;

/*-----------------------------------------------------------------------------
		0xc800c91cL vcp_disp_intr0_pos_sv_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_sv_disp_intr0_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_sv_disp_intr0_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR0_POS_SV_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c920L vcp_disp_intr1_pos_sv_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_sv_disp_intr1_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_sv_disp_intr1_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR1_POS_SV_Y_T;

/*-----------------------------------------------------------------------------
		0xc800c924L vcp_disp_intr0_pos_sv_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_sv_disp_intr0_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_sv_disp_intr0_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR0_POS_SV_C_T;

/*-----------------------------------------------------------------------------
		0xc800c928L vcp_disp_intr1_pos_sv_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_sv_disp_intr1_pos_x            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_sv_disp_intr1_pos_y            : 13;  // 28:16     
} PE_H15A0_VCP_DISP_INTR1_POS_SV_C_T;

/*-----------------------------------------------------------------------------
		0xc800c92cL vcp_disp_intr_src_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_intr_mv_y_v_src                : 1 ;  //     0     
	UINT32 vcp_intr_mv_y_h_src                : 1 ;  //     1     
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 vcp_intr_mv_y_enable               : 1 ;  //     3     
	UINT32 vcp_intr_mv_c_v_src                : 1 ;  //     4     
	UINT32 vcp_intr_mv_c_h_src                : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 vcp_intr_mv_c_enable               : 1 ;  //     7     
	UINT32 vcp_intr_sv_y_v_src                : 1 ;  //     8     
	UINT32 vcp_intr_sv_y_h_src                : 1 ;  //     9     
	UINT32                                    : 1 ;  //    10     reserved
	UINT32 vcp_intr_sv_y_enable               : 1 ;  //    11     
	UINT32 vcp_intr_sv_c_v_src                : 1 ;  //    12     
	UINT32 vcp_intr_sv_c_h_src                : 1 ;  //    13     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 vcp_intr_sv_c_enable               : 1 ;  //    15     
} PE_H15A0_VCP_DISP_INTR_SRC_SEL_T;

/*-----------------------------------------------------------------------------
		0xc800cc00L vcp_so_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_pic_init_y_mux_u1_so_cdc      : 1 ;  //     0     
	UINT32 mask_pic_init_c_mux_u1_so_cdc      : 1 ;  //     1     
	UINT32 mask_pic_init_y_mux_u2_so_cdc      : 1 ;  //     2     
	UINT32 mask_pic_init_c_mux_u2_so_cdc      : 1 ;  //     3     
	UINT32 mask_pic_init_y_mux_f1_so_cdc      : 1 ;  //     4     
	UINT32 mask_pic_init_c_mux_f1_so_cdc      : 1 ;  //     5     
	UINT32 mask_pic_init_y_mux_f2_so_cdc      : 1 ;  //     6     
	UINT32 mask_pic_init_c_mux_f2_so_cdc      : 1 ;  //     7     
	UINT32 mask_pic_init_y_mux_so_wcp         : 1 ;  //     8     
	UINT32 mask_pic_init_c_mux_so_wcp         : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 mask_pic_start_y_mux_u1_so_cdc     : 1 ;  //    16     
	UINT32 mask_pic_start_c_mux_u1_so_cdc     : 1 ;  //    17     
	UINT32 mask_pic_start_y_mux_u2_so_cdc     : 1 ;  //    18     
	UINT32 mask_pic_start_c_mux_u2_so_cdc     : 1 ;  //    19     
	UINT32 mask_pic_start_y_mux_f1_so_cdc     : 1 ;  //    20     
	UINT32 mask_pic_start_c_mux_f1_so_cdc     : 1 ;  //    21     
	UINT32 mask_pic_start_y_mux_f2_so_cdc     : 1 ;  //    22     
	UINT32 mask_pic_start_c_mux_f2_so_cdc     : 1 ;  //    23     
	UINT32 mask_pic_start_y_mux_so_wcp        : 1 ;  //    24     
	UINT32 mask_pic_start_c_mux_so_wcp        : 1 ;  //    25     
} PE_H15A0_VCP_SO_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc04L vcp_so_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_SO_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc08L vcp_so_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_end_so_cdc_va                  : 1 ;  //     0     
	UINT32 pic_end_so_cdc_vs                  : 1 ;  //     1     
} PE_H15A0_VCP_SO_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc0cL vcp_cdc_so_rst_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 u1_cdc_so_y_so_sreset              : 1 ;  //     0     
	UINT32 u1_cdc_so_c_so_sreset              : 1 ;  //     1     
	UINT32 u2_cdc_so_y_so_sreset              : 1 ;  //     2     
	UINT32 u2_cdc_so_c_so_sreset              : 1 ;  //     3     
	UINT32 f1_cdc_so_y_so_sreset              : 1 ;  //     4     
	UINT32 f1_cdc_so_c_so_sreset              : 1 ;  //     5     
	UINT32 f2_cdc_so_y_so_sreset              : 1 ;  //     6     
	UINT32 f2_cdc_so_c_so_sreset              : 1 ;  //     7     
} PE_H15A0_VCP_CDC_SO_RST_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cc10L vcp_u1_so_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_U1_SO_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cc14L vcp_u1_so_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_U1_SO_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800cc18L vcp_u1_so_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_U1_SO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cc20L vcp_u1_so_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U1_SO_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc24L vcp_u1_so_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U1_SO_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc28L vcp_u1_so_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_u1_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_u1_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_U1_SO_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc2cL vcp_u1_so_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U1_SO_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cc30L vcp_u1_so_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U1_SO_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc34L vcp_u1_so_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U1_SO_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc38L vcp_u1_so_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_u1_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_u1_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_U1_SO_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc3cL vcp_u1_so_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U1_SO_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cc40L vcp_u2_so_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_U2_SO_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cc44L vcp_u2_so_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_U2_SO_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800cc48L vcp_u2_so_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_U2_SO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cc50L vcp_u2_so_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U2_SO_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc54L vcp_u2_so_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U2_SO_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc58L vcp_u2_so_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_u2_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_u2_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_U2_SO_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc5cL vcp_u2_so_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U2_SO_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cc60L vcp_u2_so_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_U2_SO_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc64L vcp_u2_so_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_U2_SO_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc68L vcp_u2_so_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_u2_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_u2_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_U2_SO_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc6cL vcp_u2_so_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_U2_SO_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cc70L vcp_f1_so_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_F1_SO_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cc74L vcp_f1_so_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_F1_SO_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800cc78L vcp_f1_so_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_F1_SO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cc80L vcp_f1_so_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F1_SO_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc84L vcp_f1_so_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F1_SO_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc88L vcp_f1_so_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_f1_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_f1_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_F1_SO_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc8cL vcp_f1_so_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F1_SO_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cc90L vcp_f1_so_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F1_SO_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cc94L vcp_f1_so_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F1_SO_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cc98L vcp_f1_so_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_f1_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_f1_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_F1_SO_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800cc9cL vcp_f1_so_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F1_SO_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cca0L vcp_f2_so_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_F2_SO_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cca4L vcp_f2_so_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_F2_SO_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800cca8L vcp_f2_so_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_F2_SO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800ccb0L vcp_f2_so_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F2_SO_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800ccb4L vcp_f2_so_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F2_SO_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800ccb8L vcp_f2_so_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_f2_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_f2_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_F2_SO_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800ccbcL vcp_f2_so_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F2_SO_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800ccc0L vcp_f2_so_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_F2_SO_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800ccc4L vcp_f2_so_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_F2_SO_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800ccc8L vcp_f2_so_cdc_c_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_c_mux_f2_so_cdc       : 4 ;  //  3: 0     
	UINT32 set_pic_start_c_mux_f2_so_cdc      : 4 ;  //  7: 4     
} PE_H15A0_VCP_F2_SO_CDC_C_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800ccccL vcp_f2_so_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_F2_SO_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800ccd0L vcp_sg_sosd_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800ccd4L vcp_sg_sosd_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800ccd8L vcp_sg_sosd_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800ccdcL vcp_sg_sosd_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800cce0L vcp_sg_sosd_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_SOSD_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800cce4L vcp_sg_sosd_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800cce8L vcp_sg_sosd_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800ccecL vcp_sg_sosd_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800ccf0L vcp_sg_sosd_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800ccf4L vcp_sg_sosd_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_SOSD_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800ccf8L vcp_sg_sosd_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_SOSD_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800cd00L vcp_sg_so_bdr_v0_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800cd04L vcp_sg_so_bdr_v0_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800cd08L vcp_sg_so_bdr_v0_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800cd10L vcp_sg_so_bdr_v1_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800cd14L vcp_sg_so_bdr_v1_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800cd18L vcp_sg_so_bdr_v1_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800cd20L vcp_sg_so_bdr_v2_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800cd24L vcp_sg_so_bdr_v2_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800cd28L vcp_sg_so_bdr_v2_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800cd30L vcp_sg_so_bdr_v3_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y0                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x0                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800cd34L vcp_sg_so_bdr_v3_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_pos_y1                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 win_pos_x1                         : 13;  // 28:16     
} PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800cd38L vcp_sg_so_bdr_v3_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_ha_sub_sel                    : 1 ;  //     0     
	UINT32 sync_ha_sub_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 num_ch                             : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 win_border_width_right             : 4 ;  // 19:16     
	UINT32 win_border_width_left              : 4 ;  // 23:20     
	UINT32 win_border_width_down              : 4 ;  // 27:24     
	UINT32 win_border_width_up                : 4 ;  // 31:28     
} PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800cd40L vcp_wcp_so_pixel_read_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pel_pos_y                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 rd_pel_pos_x                       : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 rd_pel_pat_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_PIXEL_READ_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cd44L vcp_wcp_so_pixel_read_data_e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msb8_rd_pel_cr                     : 8 ;  //  7: 0     
	UINT32 msb8_rd_pel_cb                     : 8 ;  // 15: 8     
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32 lsb2_rd_pel_cr                     : 2 ;  // 25:24     
	UINT32 lsb2_rd_pel_cb                     : 2 ;  // 27:26     
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_WCP_SO_PIXEL_READ_DATA_E_T;

/*-----------------------------------------------------------------------------
		0xc800cd48L vcp_wcp_so_line_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 even_line_cr                       : 8 ;  //  7: 0     
	UINT32 even_line_cb                       : 8 ;  // 15: 8     
	UINT32 even_line_y                        : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 even_line_pat_en                   : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_LINE_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cd4cL vcp_wcp_so_line_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 odd_line_cr                        : 8 ;  //  7: 0     
	UINT32 odd_line_cb                        : 8 ;  // 15: 8     
	UINT32 odd_line_y                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 odd_line_pat_en                    : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_LINE_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800cd50L vcp_wcp_so_rec0_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_REC0_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800cd54L vcp_wcp_so_rec0_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec0_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec0_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec0_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_REC0_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800cd58L vcp_wcp_so_rec1_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_x                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_x                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_en                            : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_REC1_POS_X_T;

/*-----------------------------------------------------------------------------
		0xc800cd5cL vcp_wcp_so_rec1_pos_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rec1_e_pos_y                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 rec1_s_pos_y                       : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 rec1_fill_en                       : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_REC1_POS_Y_T;

/*-----------------------------------------------------------------------------
		0xc800cd90L vcp_wcp_so_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pg1ch_win_o_size_y                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pg1ch_win_o_size_x                 : 13;  // 28:16     
} PE_H15A0_VCP_WCP_SO_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800cd94L vcp_wcp_so_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lattice_pat_en                     : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 gbrw_pat_en                        : 2 ;  //  3: 2     
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
	UINT32 border_sel                         : 4 ;  // 11: 8     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 status_bar_en                      : 1 ;  //    15     
} PE_H15A0_VCP_WCP_SO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800cda0L vcp_wcp_so_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 compare_lr_diff_sel                : 2 ;  // 25:24     
	UINT32 compare_lr_sync_sel                : 1 ;  //    26     
	UINT32 compare_lr_mark_org_nac            : 1 ;  //    27     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 compare_lr_en                      : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800cda4L vcp_wcp_so_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 compare_lr_th_c                    : 8 ;  //  7: 0     
	UINT32 compare_lr_th_y                    : 8 ;  // 15: 8     
} PE_H15A0_VCP_WCP_SO_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800cda8L vcp_wcp_so_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 clr_nac_cr                         : 4 ;  //  3: 0     
	UINT32 clr_nac_cb                         : 4 ;  //  7: 4     
	UINT32 clr_nac_y                          : 8 ;  // 15: 8     
	UINT32 clr_bdr_cr                         : 4 ;  // 19:16     
	UINT32 clr_bdr_cb                         : 4 ;  // 23:20     
	UINT32 clr_bdr_y                          : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_SO_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800cdacL vcp_wcp_so_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w1_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w1_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w1_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w0_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w0_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w0_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_SO_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800cdb0L vcp_wcp_so_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w3_clr_dark_cr                     : 4 ;  //  3: 0     
	UINT32 w3_clr_dark_cb                     : 4 ;  //  7: 4     
	UINT32 w3_clr_dark_y                      : 8 ;  // 15: 8     
	UINT32 w2_clr_dark_cr                     : 4 ;  // 19:16     
	UINT32 w2_clr_dark_cb                     : 4 ;  // 23:20     
	UINT32 w2_clr_dark_y                      : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_SO_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800cdb4L vcp_wcp_so_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_imux_ctrl                   : 3 ;  //  2: 0     
	UINT32 set_w0_dark_en                     : 1 ;  //     3     
	UINT32 set_w1_imux_ctrl                   : 3 ;  //  6: 4     
	UINT32 set_w1_dark_en                     : 1 ;  //     7     
	UINT32 set_w2_imux_ctrl                   : 3 ;  // 10: 8     
	UINT32 set_w2_dark_en                     : 1 ;  //    11     
	UINT32 set_w3_imux_ctrl                   : 3 ;  // 14:12     
	UINT32 set_w3_dark_en                     : 1 ;  //    15     
	UINT32 set_lparity_w0_imux_en             : 1 ;  //    16     
	UINT32 set_lparity_w0_imux                : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 set_lparity_w1_imux_en             : 1 ;  //    20     
	UINT32 set_lparity_w1_imux                : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 set_lparity_sync_c                 : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 set_lparity_sync_y                 : 3 ;  // 30:28     
} PE_H15A0_VCP_WCP_SO_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800cdb8L vcp_wcp_so_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_a0_c_imux_ctrl                 : 3 ;  //  2: 0     
	UINT32 set_a0_c_constant_en               : 1 ;  //     3     
	UINT32 set_a1_c_imux_ctrl                 : 3 ;  //  6: 4     
	UINT32 set_a1_c_constant_en               : 1 ;  //     7     
	UINT32 set_a2_c_imux_ctrl                 : 3 ;  // 10: 8     
	UINT32 set_a2_c_constant_en               : 1 ;  //    11     
	UINT32 set_a3_c_imux_ctrl                 : 3 ;  // 14:12     
	UINT32 set_a3_c_constant_en               : 1 ;  //    15     
	UINT32 set_a0_y_imux_ctrl                 : 3 ;  // 18:16     
	UINT32 set_a0_y_constant_en               : 1 ;  //    19     
	UINT32 set_a1_y_imux_ctrl                 : 3 ;  // 22:20     
	UINT32 set_a1_y_constant_en               : 1 ;  //    23     
	UINT32 set_a2_y_imux_ctrl                 : 3 ;  // 26:24     
	UINT32 set_a2_y_constant_en               : 1 ;  //    27     
	UINT32 set_a3_y_imux_ctrl                 : 3 ;  // 30:28     
	UINT32 set_a3_y_constant_en               : 1 ;  //    31     
} PE_H15A0_VCP_WCP_SO_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800cdbcL vcp_wcp_so_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_w0_abr_en                      : 2 ;  //  1: 0     
	UINT32 set_w1_abr_en                      : 2 ;  //  3: 2     
	UINT32 set_w2_abr_en                      : 2 ;  //  5: 4     
	UINT32 set_w3_abr_en                      : 2 ;  //  7: 6     
	UINT32 set_w0_awb_cr                      : 1 ;  //     8     
	UINT32 set_w0_awb_cb                      : 1 ;  //     9     
	UINT32 set_w0_awb_y                       : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 set_w1_awb_cr                      : 1 ;  //    12     
	UINT32 set_w1_awb_cb                      : 1 ;  //    13     
	UINT32 set_w1_awb_y                       : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 set_w2_awb_cr                      : 1 ;  //    16     
	UINT32 set_w2_awb_cb                      : 1 ;  //    17     
	UINT32 set_w2_awb_y                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 set_w3_awb_cr                      : 1 ;  //    20     
	UINT32 set_w3_awb_cb                      : 1 ;  //    21     
	UINT32 set_w3_awb_y                       : 1 ;  //    22     
} PE_H15A0_VCP_WCP_SO_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800cdc0L vcp_wcp_so_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_clr_alpha                       : 8 ;  //  7: 0     
	UINT32 w1_clr_alpha                       : 8 ;  // 15: 8     
	UINT32 w2_clr_alpha                       : 8 ;  // 23:16     
	UINT32 w3_clr_alpha                       : 8 ;  // 31:24     
} PE_H15A0_VCP_WCP_SO_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800cdc4L vcp_wcp_so_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_ha_o_mux_ctrl                  : 4 ;  //  3: 0     
	UINT32 set_va_o_mux_ctrl                  : 4 ;  //  7: 4     
	UINT32 set_de_o_mux_ctrl                  : 4 ;  // 11: 8     
} PE_H15A0_VCP_WCP_SO_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800cdc8L vcp_wcp_so_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_y_mux_so_wcp          : 4 ;  //  3: 0     
	UINT32 set_pic_start_y_mux_so_wcp         : 4 ;  //  7: 4     
	UINT32 set_pic_init_c_mux_so_wcp          : 4 ;  // 11: 8     
	UINT32 set_pic_start_c_mux_so_wcp         : 4 ;  // 15:12     
} PE_H15A0_VCP_WCP_SO_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800cdd0L vcp_wcp_sosd_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sel_wcp_so_sync_sel_v0             : 2 ;  //  1: 0     
	UINT32 sel_wcp_so_sync_sel_v1             : 2 ;  //  3: 2     
	UINT32 sel_wcp_so_sync_sel_v2             : 2 ;  //  5: 4     
	UINT32 sel_wcp_so_sync_sel_v3             : 2 ;  //  7: 6     
} PE_H15A0_VCP_WCP_SOSD_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800cdd4L vcp_wcp_sosd_lrdiff_cnt_yy_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SOSD_LRDIFF_CNT_YY_O_T;

/*-----------------------------------------------------------------------------
		0xc800cdd8L vcp_wcp_sosd_lrdiff_cnt_cc_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_WCP_SOSD_LRDIFF_CNT_CC_O_T;

/*-----------------------------------------------------------------------------
		0xc800cddcL vcp_so_intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_so_intr0_mask                  : 1 ;  //     0     
	UINT32 vcp_so_intr1_mask                  : 1 ;  //     1     
	UINT32 vcp_so_w0_intr_mask                : 1 ;  //     2     
	UINT32 vcp_so_w1_intr_mask                : 1 ;  //     3     
	UINT32 vcp_so_w2_intr_mask                : 1 ;  //     4     
	UINT32 vcp_so_w3_intr_mask                : 1 ;  //     5     
} PE_H15A0_VCP_SO_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc800cde0L vcp_so_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_so_intr0_clear                 : 1 ;  //     0     
	UINT32 vcp_so_intr1_clear                 : 1 ;  //     1     
	UINT32 vcp_so_w0_intr_clear               : 1 ;  //     2     
	UINT32 vcp_so_w1_intr_clear               : 1 ;  //     3     
	UINT32 vcp_so_w2_intr_clear               : 1 ;  //     4     
	UINT32 vcp_so_w3_intr_clear               : 1 ;  //     5     
} PE_H15A0_VCP_SO_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc800cde4L vcp_so_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_so_intr0_status                : 1 ;  //     0     
	UINT32 vcp_so_intr1_status                : 1 ;  //     1     
	UINT32 vcp_so_w0_intr_status              : 1 ;  //     2     
	UINT32 vcp_so_w1_intr_status              : 1 ;  //     3     
	UINT32 vcp_so_w2_intr_status              : 1 ;  //     4     
	UINT32 vcp_so_w3_intr_status              : 1 ;  //     5     
} PE_H15A0_VCP_SO_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800cde8L vcp_so_intr0_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_so_intr0_pos_x                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_so_intr0_pos_y                 : 13;  // 28:16     
} PE_H15A0_VCP_SO_INTR0_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cdecL vcp_so_intr1_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_so_intr1_pos_x                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_so_intr1_pos_y                 : 13;  // 28:16     
} PE_H15A0_VCP_SO_INTR1_POS_T;

/*-----------------------------------------------------------------------------
		0xc800cdf0L vcp_so_intr_src_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_intr_v_src                     : 1 ;  //     0     
	UINT32 vcp_intr_h_src                     : 1 ;  //     1     
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 vcp_intr_enable                    : 1 ;  //     3     
} PE_H15A0_VCP_SO_INTR_SRC_SEL_T;

/*-----------------------------------------------------------------------------
		0xc800ce00L vcp_disph_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mask_pic_init_mux_ht_cdc           : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 mask_pic_start_mux_ht_cdc          : 1 ;  //    16     
} PE_H15A0_VCP_DISPH_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800ce04L vcp_cdc_ht_rst_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ht_cdc_y_disph_sreset              : 1 ;  //     0     
	UINT32 ht_cdc_c_disph_sreset              : 1 ;  //     1     
} PE_H15A0_VCP_CDC_HT_RST_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800ce08L vcp_ht_win_o_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_pos_y                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_pos_x                    : 13;  // 28:16     
} PE_H15A0_VCP_HT_WIN_O_POS_T;

/*-----------------------------------------------------------------------------
		0xc800ce0cL vcp_ht_win_o_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_win_o_size_y                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cdc_win_o_size_x                   : 13;  // 28:16     
} PE_H15A0_VCP_HT_WIN_O_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800ce10L vcp_ht_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
} PE_H15A0_VCP_HT_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800ce18L vcp_ht_cdc_y_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_HT_CDC_Y_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800ce1cL vcp_ht_cdc_y_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_HT_CDC_Y_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800ce20L vcp_ht_cdc_y_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 set_pic_init_mux_ht_cdc            : 4 ;  //  3: 0     
	UINT32 set_pic_start_mux_ht_cdc           : 4 ;  //  7: 4     
} PE_H15A0_VCP_HT_CDC_Y_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc800ce24L vcp_ht_cdc_y_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_HT_CDC_Y_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800ce28L vcp_ht_cdc_c_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cdc_voffset                        : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cdc_vmode                          : 3 ;  // 10: 8     
	UINT32 cdc_vmode_cnt_sel                  : 1 ;  //    11     
	UINT32 cdc_scan_type                      : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 cdc_hmode                          : 3 ;  // 18:16     
	UINT32 cdc_hmode_cnt_sel                  : 1 ;  //    19     
	UINT32 cdc_ch                             : 2 ;  // 21:20     
	UINT32 cdc_de_mode                        : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cdc_format                         : 4 ;  // 27:24     
	UINT32 cdc_lsb_omode                      : 4 ;  // 31:28     
} PE_H15A0_VCP_HT_CDC_C_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800ce2cL vcp_ht_cdc_c_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nactive_color_cr                   : 8 ;  //  7: 0     
	UINT32 nactive_color_cb                   : 8 ;  // 15: 8     
	UINT32 nactive_color_y                    : 8 ;  // 23:16     
	UINT32 nactive_window_end_ctrl            : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 nactive_window_start_ctrl          : 3 ;  // 30:28     
	UINT32 nactive_color_enable               : 1 ;  //    31     
} PE_H15A0_VCP_HT_CDC_C_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800ce30L vcp_ht_cdc_c_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_VCP_HT_CDC_C_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800ce34L vcp_ht_cdc_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 disp_o_size_y                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 disp_o_size_x                      : 13;  // 28:16     
} PE_H15A0_VCP_HT_CDC_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800ce38L vcp_ht_pixel_read_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pel_pos_y                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 rd_pel_pos_x                       : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 rd_pel_pat_en                      : 1 ;  //    31     
} PE_H15A0_VCP_HT_PIXEL_READ_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800ce3cL vcp_ht_pixel_read_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 msb8_rd_pel_cr                     : 8 ;  //  7: 0     
	UINT32 msb8_rd_pel_cb                     : 8 ;  // 15: 8     
	UINT32 msb8_rd_pel_y                      : 8 ;  // 23:16     
	UINT32 lsb2_rd_pel_cr                     : 2 ;  // 25:24     
	UINT32 lsb2_rd_pel_cb                     : 2 ;  // 27:26     
	UINT32 lsb2_rd_pel_y                      : 2 ;  // 29:28     
} PE_H15A0_VCP_HT_PIXEL_READ_DATA_T;

/*-----------------------------------------------------------------------------
		0xc800ce40L vcp_ht_intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_ht_intr0_mask                  : 1 ;  //     0     
	UINT32 vcp_ht_intr1_mask                  : 1 ;  //     1     
} PE_H15A0_VCP_HT_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0xc800ce44L vcp_ht_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_ht_intr0_clear                 : 1 ;  //     0     
	UINT32 vcp_ht_intr1_clear                 : 1 ;  //     1     
} PE_H15A0_VCP_HT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc800ce48L vcp_ht_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_ht_intr0_status                : 1 ;  //     0     
	UINT32 vcp_ht_intr1_status                : 1 ;  //     1     
} PE_H15A0_VCP_HT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc800ce4cL vcp_ht_intr0_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_ht_intr0_pos_x                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_ht_intr0_pos_y                 : 13;  // 28:16     
} PE_H15A0_VCP_HT_INTR0_POS_T;

/*-----------------------------------------------------------------------------
		0xc800ce50L vcp_ht_intr1_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_ht_intr1_pos_x                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 vcp_ht_intr1_pos_y                 : 13;  // 28:16     
} PE_H15A0_VCP_HT_INTR1_POS_T;

/*-----------------------------------------------------------------------------
		0xc800ce54L vcp_ht_intr_src_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcp_intr_v_src                     : 1 ;  //     0     
	UINT32 vcp_intr_h_src                     : 1 ;  //     1     
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 vcp_intr_enable                    : 1 ;  //     3     
} PE_H15A0_VCP_HT_INTR_SRC_SEL_T;

/*-----------------------------------------------------------------------------
		0xc800ce58L vcp_sg_ht_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800ce5cL vcp_sg_ht_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800ce60L vcp_sg_ht_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800ce64L vcp_sg_ht_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800ce68L vcp_sg_ht_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_HT_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800ce6cL vcp_sg_ht_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800ce70L vcp_sg_ht_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800ce74L vcp_sg_ht_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800ce78L vcp_sg_ht_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800ce7cL vcp_sg_ht_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_HT_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800ce80L vcp_sg_ht_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_HT_CTRL_A_T;

/*-----------------------------------------------------------------------------
		0xc800ce84L vcp_sg_sry_ht_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync                              : 16;  // 15: 0     
	UINT32 vfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800ce88L vcp_sg_sry_ht_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va                                 : 16;  // 15: 0     
	UINT32 vbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800ce8cL vcp_sg_sry_ht_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync                              : 16;  // 15: 0     
	UINT32 hfp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800ce90L vcp_sg_sry_ht_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha                                 : 16;  // 15: 0     
	UINT32 hbp                                : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800ce94L vcp_sg_sry_ht_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_pos                     : 16;  // 15: 0     
	UINT32 sync_i_h_sel                       : 1 ;  //    16     
	UINT32 sync_i_v_sel                       : 1 ;  //    17     
	UINT32 sync_i_inv_ha                      : 1 ;  //    18     
	UINT32 sync_i_inv_va                      : 1 ;  //    19     
	UINT32 sync_o_sel_ha                      : 1 ;  //    20     
	UINT32 sync_o_sel_va                      : 1 ;  //    21     
	UINT32 sync_o_sel_hs                      : 1 ;  //    22     
	UINT32 sync_o_sel_vs                      : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 manual_cnt_h_value_en              : 1 ;  //    28     
	UINT32 manual_cnt_v_value_en              : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 update_force                       : 1 ;  //    31     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc800ce98L vcp_sg_sry_ht_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_h_dly                         : 16;  // 15: 0     
	UINT32 sync_v_dly                         : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800ce9cL vcp_sg_sry_ht_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_cnt_h_value                 : 16;  // 15: 0     
	UINT32 manual_cnt_v_value                 : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc800cea0L vcp_sg_sry_ht_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_ha_delay                      : 16;  // 15: 0     
	UINT32 auto_va_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc800cea4L vcp_sg_sry_ht_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_hs_delay                      : 16;  // 15: 0     
	UINT32 auto_vs_delay                      : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc800cea8L vcp_sg_sry_ht_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rsv                                : 32;  // 31: 0     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800ceacL vcp_sg_sry_ht_ctrl_a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_cnt                              : 16;  // 15: 0     
	UINT32 v_cnt                              : 16;  // 31:16     
} PE_H15A0_VCP_SG_SRY_HT_CTRL_A_T;

typedef struct {
	PE_H15A0_VCP_DE_CTRL0_T               	vcp_de_ctrl0;                 //0xc800c000L
	PE_H15A0_VCP_DE_CTRL1_T               	vcp_de_ctrl1;                 //0xc800c004L
	PE_H15A0_VCP_DE_CTRL2_T               	vcp_de_ctrl2;                 //0xc800c008L
	PE_H15A0_VCP_DE_CTRL3_T               	vcp_de_ctrl3;                 //0xc800c00cL
	PE_H15A0_VCP_DE_CDC_STATUS0_T           vcp_cdc_status0;              //0xc800c010L
	PE_H15A0_VCP_DE_CDC_STATUS1_T           vcp_cdc_status1;              //0xc800c014L
	PE_H15A0_VCP_SG_SRY_CTRL_T            	vcp_sg_sry_ctrl;              //0xc800c018L
	PE_H15A0_VCP_SG_SRY_CTRL1_T           	vcp_sg_sry_ctrl1;             //0xc800c01cL
	PE_H15A0_VCP_PIC_INIT_Y_STATUS_T      	vcp_pic_init_y_status;        //0xc800c020L
	PE_H15A0_VCP_PIC_INIT_C_STATUS_T      	vcp_pic_init_c_status;        //0xc800c024L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_T         	vcp_sg_sry_ht_ctrl;           //0xc800c028L
	PE_H15A0_VCP_SG_SRY_HT_CTRL1_T        	vcp_sg_sry_ht_ctrl1;          //0xc800c02cL
	PE_H15A0_VCP_MIFU_P2LM_STATUS_CTRL_T  	vcp_mifu_p2lm_status_ctrl;    //0xc800c030L
	PE_H15A0_VCP_MIFU1_P2LM_STATUS0_T     	vcp_mifu1_p2lm_status0;       //0xc800c034L
	PE_H15A0_VCP_MIFU1_P2LM_STATUS1_T     	vcp_mifu1_p2lm_status1;       //0xc800c038L
	PE_H15A0_VCP_MIFU2_P2LM_STATUS_T      	vcp_mifu2_p2lm_status;        //0xc800c03cL
	PE_H15A0_VCP_MIF_VAL_CTRL_T           	vcp_mif_val_ctrl;             //0xc800c040L
	UINT32                             	    reserved00[3];                //0xc800c044L~0xc800c04cL
	PE_H15A0_VCP_MIF_SRESET_T             	vcp_mif_sreset;               //0xc800c050L
	PE_H15A0_VCP_MIFU1_FSR_STATUS_T       	vcp_mifu1_fsr_status;         //0xc800c054L
	UINT32                             	    reserved01[2];                //0xc800c058L~0xc800c05cL
	PE_H15A0_VCP_MIFU1_FSR_IO_TYPE_T      	vcp_mifu1_fsr_io_type;        //0xc800c060L
	PE_H15A0_VCP_MIFU1_FSR_WIN_I_POS_T    	vcp_mifu1_fsr_win_i_pos;      //0xc800c064L
	PE_H15A0_VCP_MIFU1_FSR_WIN_I_SIZE_T   	vcp_mifu1_fsr_win_i_size;     //0xc800c068L
	PE_H15A0_VCP_MIFU1_FSR_WIN_O_POS_T    	vcp_mifu1_fsr_win_o_pos;      //0xc800c06cL
	PE_H15A0_VCP_MIFU1_FSR_WIN_O_SIZE_T   	vcp_mifu1_fsr_win_o_size;     //0xc800c070L
	UINT32                             	    reserved02[3];                //0xc800c074L~0xc800c07cL
	PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL0_T	vcp_mifu1_fsr_rhost_mif_ctrl0;//0xc800c080L
	PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL1_T	vcp_mifu1_fsr_rhost_mif_ctrl1;//0xc800c084L
	PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL2_T	vcp_mifu1_fsr_rhost_mif_ctrl2;//0xc800c088L
	PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL3_T	vcp_mifu1_fsr_rhost_mif_ctrl3;//0xc800c08cL
	PE_H15A0_VCP_MIFU1_FSR_RHOST_MIF_CTRL4_T	vcp_mifu1_fsr_rhost_mif_ctrl4;//0xc800c090L
	H15A0_MON_VCP_MIFU1_FSR_FCNT_Y_T        mon_vcp_mifu1_fsr_fcnt_y;     //0xc800c094L
	PE_H15A0_VCP_MIFU1_FSR_LM_CTRL_T      	vcp_mifu1_fsr_lm_ctrl;        //0xc800c098L
	UINT32                             	    reserved03[2];                //0xc800c09cL~0xc800c0a0L
	PE_H15A0_VCP_MIFU1_BSR_STATUS_T       	vcp_mifu1_bsr_status;         //0xc800c0a4L
	UINT32                             	    reserved04[2];                //0xc800c0a8L~0xc800c0acL
	PE_H15A0_VCP_MIFU1_BSR_IO_TYPE_T      	vcp_mifu1_bsr_io_type;        //0xc800c0b0L
	PE_H15A0_VCP_MIFU1_BSR_WIN_I_POS_T    	vcp_mifu1_bsr_win_i_pos;      //0xc800c0b4L
	PE_H15A0_VCP_MIFU1_BSR_WIN_I_SIZE_T   	vcp_mifu1_bsr_win_i_size;     //0xc800c0b8L
	PE_H15A0_VCP_MIFU1_BSR_WIN_O_POS_T    	vcp_mifu1_bsr_win_o_pos;      //0xc800c0bcL
	PE_H15A0_VCP_MIFU1_BSR_WIN_O_SIZE_T   	vcp_mifu1_bsr_win_o_size;     //0xc800c0c0L
	UINT32                             	    reserved05[3];                //0xc800c0c4L~0xc800c0ccL
	PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL0_T	vcp_mifu1_bsr_rhost_mif_ctrl0;//0xc800c0d0L
	PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL1_T	vcp_mifu1_bsr_rhost_mif_ctrl1;//0xc800c0d4L
	PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL2_T	vcp_mifu1_bsr_rhost_mif_ctrl2;//0xc800c0d8L
	PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL3_T	vcp_mifu1_bsr_rhost_mif_ctrl3;//0xc800c0dcL
	PE_H15A0_VCP_MIFU1_BSR_RHOST_MIF_CTRL4_T	vcp_mifu1_bsr_rhost_mif_ctrl4;//0xc800c0e0L
	H15A0_MON_VCP_MIFU1_BSR_FCNT_Y_T   	    mon_vcp_mifu1_bsr_fcnt_y;     //0xc800c0e4L
	PE_H15A0_VCP_MIFU1_BSR_LM_CTRL_T      	vcp_mifu1_bsr_lm_ctrl;        //0xc800c0e8L
	UINT32                             	    reserved06[2];                //0xc800c0ecL~0xc800c0f0L
	PE_H15A0_VCP_MIFU1C_STATUS_T          	vcp_mifu1c_status;            //0xc800c0f4L
	UINT32                             	    reserved07[2];                //0xc800c0f8L~0xc800c0fcL
	PE_H15A0_VCP_MIFU1C_IO_TYPE_T         	vcp_mifu1c_io_type;           //0xc800c100L
	PE_H15A0_VCP_MIFU1C_WIN_I_POS_T       	vcp_mifu1c_win_i_pos;         //0xc800c104L
	PE_H15A0_VCP_MIFU1C_WIN_I_SIZE_T      	vcp_mifu1c_win_i_size;        //0xc800c108L
	PE_H15A0_VCP_MIFU1C_WIN_O_POS_T       	vcp_mifu1c_win_o_pos;         //0xc800c10cL
	PE_H15A0_VCP_MIFU1C_WIN_O_SIZE_T      	vcp_mifu1c_win_o_size;        //0xc800c110L
	UINT32                             	    reserved08[3];                //0xc800c114L~0xc800c11cL
	PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL0_T 	vcp_mifu1c_rhost_mif_ctrl0;   //0xc800c120L
	PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL1_T 	vcp_mifu1c_rhost_mif_ctrl1;   //0xc800c124L
	PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL2_T 	vcp_mifu1c_rhost_mif_ctrl2;   //0xc800c128L
	PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL3_T 	vcp_mifu1c_rhost_mif_ctrl3;   //0xc800c12cL
	PE_H15A0_VCP_MIFU1C_RHOST_MIF_CTRL4_T 	vcp_mifu1c_rhost_mif_ctrl4;   //0xc800c130L
	H15A0_MON_VCP_MIFU1C_FCNT_C_T      	    mon_vcp_mifu1c_fcnt_c;        //0xc800c134L
	PE_H15A0_VCP_MIFU1C_LM_CTRL_T         	vcp_mifu1c_lm_ctrl;           //0xc800c138L
	UINT32                             	    reserved09[2];                //0xc800c13cL~0xc800c140L
	PE_H15A0_VCP_MIFU2_STATUS_T           	vcp_mifu2_status;             //0xc800c144L
	UINT32                             	    reserved10[2];                //0xc800c148L~0xc800c14cL
	PE_H15A0_VCP_MIFU2_IO_TYPE_T          	vcp_mifu2_io_type;            //0xc800c150L
	PE_H15A0_VCP_MIFU2_WIN_I_POS_T        	vcp_mifu2_win_i_pos;          //0xc800c154L
	PE_H15A0_VCP_MIFU2_WIN_I_SIZE_T       	vcp_mifu2_win_i_size;         //0xc800c158L
	PE_H15A0_VCP_MIFU2_WIN_O_POS_T        	vcp_mifu2_win_o_pos;          //0xc800c15cL
	PE_H15A0_VCP_MIFU2_WIN_O_SIZE_T       	vcp_mifu2_win_o_size;         //0xc800c160L
	UINT32                             	    reserved11[3];                //0xc800c164L~0xc800c16cL
	PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL0_T  	vcp_mifu2_rhost_mif_ctrl0;    //0xc800c170L
	PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL1_T  	vcp_mifu2_rhost_mif_ctrl1;    //0xc800c174L
	PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL2_T  	vcp_mifu2_rhost_mif_ctrl2;    //0xc800c178L
	PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL3_T  	vcp_mifu2_rhost_mif_ctrl3;    //0xc800c17cL
	PE_H15A0_VCP_MIFU2_RHOST_MIF_CTRL4_T  	vcp_mifu2_rhost_mif_ctrl4;    //0xc800c180L
	H15A0_MON_VCP_MIFU2_FCNT_Y_T            mon_vcp_mifu2_fcnt_y;         //0xc800c184L
	H15A0_MON_VCP_MIFU2_FCNT_C_T       	    mon_vcp_mifu2_fcnt_c;         //0xc800c188L
	PE_H15A0_VCP_MIFU2_LM_CTRL_T          	vcp_mifu2_lm_ctrl;            //0xc800c18cL
	UINT32                             	    reserved12;                   //0xc800c190L
	PE_H15A0_VCP_MIFF1_STATUS_T           	vcp_miff1_status;             //0xc800c194L
	UINT32                             	    reserved13[2];                //0xc800c198L~0xc800c19cL
	PE_H15A0_VCP_MIFF1_IO_TYPE_T          	vcp_miff1_io_type;            //0xc800c1a0L
	PE_H15A0_VCP_MIFF1_WIN_I_POS_T        	vcp_miff1_win_i_pos;          //0xc800c1a4L
	PE_H15A0_VCP_MIFF1_WIN_I_SIZE_T       	vcp_miff1_win_i_size;         //0xc800c1a8L
	PE_H15A0_VCP_MIFF1_WIN_O_POS_T        	vcp_miff1_win_o_pos;          //0xc800c1acL
	PE_H15A0_VCP_MIFF1_WIN_O_SIZE_T       	vcp_miff1_win_o_size;         //0xc800c1b0L
	UINT32                             	    reserved14[3];                //0xc800c1b4L~0xc800c1bcL
	PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL0_T  	vcp_miff1_rhost_mif_ctrl0;    //0xc800c1c0L
	PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL1_T  	vcp_miff1_rhost_mif_ctrl1;    //0xc800c1c4L
	PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL2_T  	vcp_miff1_rhost_mif_ctrl2;    //0xc800c1c8L
	PE_H15A0_VCP_MIFF1_RHOST_MIF_CTRL3_T  	vcp_miff1_rhost_mif_ctrl3;    //0xc800c1ccL
	H15A0_MON_VCP_MIFF1_FCNT_Y_T       	    mon_vcp_miff1_fcnt_y;         //0xc800c1d0L
	H15A0_MON_VCP_MIFF1_FCNT_C_T       	    mon_vcp_miff1_fcnt_c;         //0xc800c1d4L
	PE_H15A0_VCP_MIFF1_LM_CTRL_T          	vcp_miff1_lm_ctrl;            //0xc800c1d8L
	UINT32                             	    reserved15[2];                //0xc800c1dcL~0xc800c1e0L
	PE_H15A0_VCP_MIFF2_STATUS_T           	vcp_miff2_status;             //0xc800c1e4L
	UINT32                             	    reserved16[2];                //0xc800c1e8L~0xc800c1ecL
	PE_H15A0_VCP_MIFF2_IO_TYPE_T          	vcp_miff2_io_type;            //0xc800c1f0L
	PE_H15A0_VCP_MIFF2_WIN_I_POS_T        	vcp_miff2_win_i_pos;          //0xc800c1f4L
	PE_H15A0_VCP_MIFF2_WIN_I_SIZE_T       	vcp_miff2_win_i_size;         //0xc800c1f8L
	PE_H15A0_VCP_MIFF2_WIN_O_POS_T        	vcp_miff2_win_o_pos;          //0xc800c1fcL
	PE_H15A0_VCP_MIFF2_WIN_O_SIZE_T       	vcp_miff2_win_o_size;         //0xc800c200L
	UINT32                             	    reserved17[3];                //0xc800c204L~0xc800c20cL
	PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL0_T  	vcp_miff2_rhost_mif_ctrl0;    //0xc800c210L
	PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL1_T  	vcp_miff2_rhost_mif_ctrl1;    //0xc800c214L
	PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL2_T  	vcp_miff2_rhost_mif_ctrl2;    //0xc800c218L
	PE_H15A0_VCP_MIFF2_RHOST_MIF_CTRL3_T  	vcp_miff2_rhost_mif_ctrl3;    //0xc800c21cL
	H15A0_MON_VCP_MIFF2_FCNT_Y_T       	    mon_vcp_miff2_fcnt_y;         //0xc800c220L
	H15A0_MON_VCP_MIFF2_FCNT_C_T       	    mon_vcp_miff2_fcnt_c;         //0xc800c224L
	PE_H15A0_VCP_MIFF2_LM_CTRL_T          	vcp_miff2_lm_ctrl;            //0xc800c228L
	UINT32                             	    reserved18;                   //0xc800c22cL
	PE_H15A0_VCP_U1_CUP_FLT_CTRL_T        	vcp_u1_cup_flt_ctrl;          //0xc800c230L
	PE_H15A0_VCP_U1_CUP_FLT_COEF01_T      	vcp_u1_cup_flt_coef01;        //0xc800c234L
	PE_H15A0_VCP_U1_CUP_FLT_COEF23_T      	vcp_u1_cup_flt_coef23;        //0xc800c238L
	PE_H15A0_VCP_U1_CUP_FLT_COEF45_T      	vcp_u1_cup_flt_coef45;        //0xc800c23cL
	PE_H15A0_VCP_U1_CUP_FLT_COEF67_T      	vcp_u1_cup_flt_coef67;        //0xc800c240L
	PE_H15A0_VCP_U1_CUP_FLT_COEF89_T      	vcp_u1_cup_flt_coef89;        //0xc800c244L
	UINT32                             	    reserved19[2];                //0xc800c248L~0xc800c24cL
	PE_H15A0_VCP_U2_CUP_FLT_CTRL_T        	vcp_u2_cup_flt_ctrl;          //0xc800c250L
	PE_H15A0_VCP_U2_CUP_FLT_COEF01_T      	vcp_u2_cup_flt_coef01;        //0xc800c254L
	PE_H15A0_VCP_U2_CUP_FLT_COEF23_T      	vcp_u2_cup_flt_coef23;        //0xc800c258L
	PE_H15A0_VCP_U2_CUP_FLT_COEF45_T      	vcp_u2_cup_flt_coef45;        //0xc800c25cL
	PE_H15A0_VCP_U2_CUP_FLT_COEF67_T      	vcp_u2_cup_flt_coef67;        //0xc800c260L
	PE_H15A0_VCP_U2_CUP_FLT_COEF89_T      	vcp_u2_cup_flt_coef89;        //0xc800c264L
	UINT32                             	    reserved20[2];                //0xc800c268L~0xc800c26cL
	PE_H15A0_VCP_F1_CUP_FLT_CTRL_T        	vcp_f1_cup_flt_ctrl;          //0xc800c270L
	PE_H15A0_VCP_F1_CUP_FLT_COEF01_T      	vcp_f1_cup_flt_coef01;        //0xc800c274L
	PE_H15A0_VCP_F1_CUP_FLT_COEF23_T      	vcp_f1_cup_flt_coef23;        //0xc800c278L
	PE_H15A0_VCP_F1_CUP_FLT_COEF45_T      	vcp_f1_cup_flt_coef45;        //0xc800c27cL
	PE_H15A0_VCP_F1_CUP_FLT_COEF67_T      	vcp_f1_cup_flt_coef67;        //0xc800c280L
	PE_H15A0_VCP_F1_CUP_FLT_COEF89_T      	vcp_f1_cup_flt_coef89;        //0xc800c284L
	UINT32                             	    reserved21[2];                //0xc800c288L~0xc800c28cL
	PE_H15A0_VCP_F2_CUP_FLT_CTRL_T        	vcp_f2_cup_flt_ctrl;          //0xc800c290L
	PE_H15A0_VCP_F2_CUP_FLT_COEF01_T      	vcp_f2_cup_flt_coef01;        //0xc800c294L
	PE_H15A0_VCP_F2_CUP_FLT_COEF23_T      	vcp_f2_cup_flt_coef23;        //0xc800c298L
	PE_H15A0_VCP_F2_CUP_FLT_COEF45_T      	vcp_f2_cup_flt_coef45;        //0xc800c29cL
	PE_H15A0_VCP_F2_CUP_FLT_COEF67_T      	vcp_f2_cup_flt_coef67;        //0xc800c2a0L
	PE_H15A0_VCP_F2_CUP_FLT_COEF89_T      	vcp_f2_cup_flt_coef89;        //0xc800c2a4L
	UINT32                             	    reserved22[10];               //0xc800c2a8L~0xc800c2ccL
	PE_H15A0_VCP_DE_INTR_MASK_T           	vcp_de_intr_mask;             //0xc800c2d0L
	PE_H15A0_VCP_DE_INTR_CLEAR_T          	vcp_de_intr_clear;            //0xc800c2d4L
	PE_H15A0_VCP_DE_INTR_STATUS_T         	vcp_de_intr_status;           //0xc800c2d8L
	PE_H15A0_VCP_DE_INTR0_POS_T           	vcp_de_intr0_pos;             //0xc800c2dcL
	PE_H15A0_VCP_DE_INTR_SRC_SEL_T        	vcp_de_intr_src_sel;          //0xc800c2e0L
}PE_VCP_DE_REG_H15A0_T;

typedef struct {
	PE_H15A0_VCP_DISP_CTRL0_T               vcp_disp_ctrl0;               //0xc800c400L
	PE_H15A0_VCP_DISP_CTRL1_T               vcp_disp_ctrl1;               //0xc800c404L
	PE_H15A0_VCP_DISP_CTRL2_T               vcp_disp_ctrl2;               //0xc800c408L
	PE_H15A0_VCP_DISP_CTRL3_T               vcp_disp_ctrl3;               //0xc800c40cL
	PE_H15A0_VCP_CDC_MV_SYNC_CTRL_T         vcp_cdc_mv_sync_ctrl;         //0xc800c410L
	PE_H15A0_VCP_CDC_MV_RST_CTRL_T          vcp_cdc_mv_rst_ctrl;          //0xc800c414L
	PE_H15A0_VCP_DISP_CDC_STATUS0_T         vcp_cdc_status0;              //0xc800c418L
	PE_H15A0_VCP_DISP_CDC_STATUS1_T         vcp_cdc_status1;              //0xc800c41cL
	PE_H15A0_VCP_U1_WIN_O_POS_T             vcp_u1_win_o_pos;             //0xc800c420L
	PE_H15A0_VCP_U1_WIN_O_SIZE_T            vcp_u1_win_o_size;            //0xc800c424L
	PE_H15A0_VCP_U1_PG_PAT_CTRL_T           vcp_u1_pg_pat_ctrl;           //0xc800c428L
	UINT32                             	    reserved01;                   //0xc800c42cL
	PE_H15A0_VCP_U1_CDC_Y_CTRL0_T           vcp_u1_cdc_y_ctrl0;           //0xc800c430L
	PE_H15A0_VCP_U1_CDC_Y_CTRL1_T           vcp_u1_cdc_y_ctrl1;           //0xc800c434L
	PE_H15A0_VCP_U1_CDC_Y_CTRL2_T           vcp_u1_cdc_y_ctrl2;           //0xc800c438L
	PE_H15A0_VCP_U1_CDC_Y_STATUS_T          vcp_u1_cdc_y_status;          //0xc800c43cL
	PE_H15A0_VCP_U1_CDC_C_CTRL0_T           vcp_u1_cdc_c_ctrl0;           //0xc800c440L
	PE_H15A0_VCP_U1_CDC_C_CTRL1_T           vcp_u1_cdc_c_ctrl1;           //0xc800c444L
	PE_H15A0_VCP_U1_CDC_C_CTRL2_T           vcp_u1_cdc_c_ctrl2;           //0xc800c448L
	PE_H15A0_VCP_U1_CDC_C_STATUS_T          vcp_u1_cdc_c_status;          //0xc800c44cL
	PE_H15A0_VCP_U2_WIN_O_POS_T             vcp_u2_win_o_pos;             //0xc800c450L
	PE_H15A0_VCP_U2_WIN_O_SIZE_T            vcp_u2_win_o_size;            //0xc800c454L
	PE_H15A0_VCP_U2_PG_PAT_CTRL_T           vcp_u2_pg_pat_ctrl;           //0xc800c458L
	UINT32                             	    reserved02;                   //0xc800c45cL
	PE_H15A0_VCP_U2_CDC_Y_CTRL0_T           vcp_u2_cdc_y_ctrl0;           //0xc800c460L
	PE_H15A0_VCP_U2_CDC_Y_CTRL1_T           vcp_u2_cdc_y_ctrl1;           //0xc800c464L
	PE_H15A0_VCP_U2_CDC_Y_CTRL2_T           vcp_u2_cdc_y_ctrl2;           //0xc800c468L
	PE_H15A0_VCP_U2_CDC_Y_STATUS_T          vcp_u2_cdc_y_status;          //0xc800c46cL
	PE_H15A0_VCP_U2_CDC_C_CTRL0_T           vcp_u2_cdc_c_ctrl0;           //0xc800c470L
	PE_H15A0_VCP_U2_CDC_C_CTRL1_T           vcp_u2_cdc_c_ctrl1;           //0xc800c474L
	PE_H15A0_VCP_U2_CDC_C_CTRL2_T           vcp_u2_cdc_c_ctrl2;           //0xc800c478L
	PE_H15A0_VCP_U2_CDC_C_STATUS_T          vcp_u2_cdc_c_status;          //0xc800c47cL
	PE_H15A0_VCP_F1_WIN_O_POS_T             vcp_f1_win_o_pos;             //0xc800c480L
	PE_H15A0_VCP_F1_WIN_O_SIZE_T            vcp_f1_win_o_size;            //0xc800c484L
	PE_H15A0_VCP_F1_PG_PAT_CTRL_T           vcp_f1_pg_pat_ctrl;           //0xc800c488L
	UINT32                             	    reserved03;                   //0xc800c48cL
	PE_H15A0_VCP_F1_CDC_Y_CTRL0_T           vcp_f1_cdc_y_ctrl0;           //0xc800c490L
	PE_H15A0_VCP_F1_CDC_Y_CTRL1_T           vcp_f1_cdc_y_ctrl1;           //0xc800c494L
	PE_H15A0_VCP_F1_CDC_Y_CTRL2_T           vcp_f1_cdc_y_ctrl2;           //0xc800c498L
	PE_H15A0_VCP_F1_CDC_Y_STATUS_T          vcp_f1_cdc_y_status;          //0xc800c49cL
	PE_H15A0_VCP_F1_CDC_C_CTRL0_T           vcp_f1_cdc_c_ctrl0;           //0xc800c4a0L
	PE_H15A0_VCP_F1_CDC_C_CTRL1_T           vcp_f1_cdc_c_ctrl1;           //0xc800c4a4L
	PE_H15A0_VCP_F1_CDC_C_CTRL2_T           vcp_f1_cdc_c_ctrl2;           //0xc800c4a8L
	PE_H15A0_VCP_F1_CDC_C_STATUS_T          vcp_f1_cdc_c_status;          //0xc800c4acL
	PE_H15A0_VCP_F2_WIN_O_POS_T             vcp_f2_win_o_pos;             //0xc800c4b0L
	PE_H15A0_VCP_F2_WIN_O_SIZE_T            vcp_f2_win_o_size;            //0xc800c4b4L
	PE_H15A0_VCP_F2_PG_PAT_CTRL_T           vcp_f2_pg_pat_ctrl;           //0xc800c4b8L
	UINT32                             	    reserved04;                   //0xc800c4bcL
	PE_H15A0_VCP_F2_CDC_Y_CTRL0_T           vcp_f2_cdc_y_ctrl0;           //0xc800c4c0L
	PE_H15A0_VCP_F2_CDC_Y_CTRL1_T           vcp_f2_cdc_y_ctrl1;           //0xc800c4c4L
	PE_H15A0_VCP_F2_CDC_Y_CTRL2_T           vcp_f2_cdc_y_ctrl2;           //0xc800c4c8L
	PE_H15A0_VCP_F2_CDC_Y_STATUS_T          vcp_f2_cdc_y_status;          //0xc800c4ccL
	PE_H15A0_VCP_F2_CDC_C_CTRL0_T           vcp_f2_cdc_c_ctrl0;           //0xc800c4d0L
	PE_H15A0_VCP_F2_CDC_C_CTRL1_T           vcp_f2_cdc_c_ctrl1;           //0xc800c4d4L
	PE_H15A0_VCP_F2_CDC_C_CTRL2_T           vcp_f2_cdc_c_ctrl2;           //0xc800c4d8L
	PE_H15A0_VCP_F2_CDC_C_STATUS_T          vcp_f2_cdc_c_status;          //0xc800c4dcL
	PE_H15A0_VCP_SG_MVY_CTRL_0_T            vcp_sg_mvy_ctrl_0;            //0xc800c4e0L
	PE_H15A0_VCP_SG_MVY_CTRL_1_T            vcp_sg_mvy_ctrl_1;            //0xc800c4e4L
	PE_H15A0_VCP_SG_MVY_CTRL_2_T            vcp_sg_mvy_ctrl_2;            //0xc800c4e8L
	PE_H15A0_VCP_SG_MVY_CTRL_3_T            vcp_sg_mvy_ctrl_3;            //0xc800c4ecL
	PE_H15A0_VCP_SG_MVY_CTRL_4_T            vcp_sg_mvy_ctrl_4;            //0xc800c4f0L
	PE_H15A0_VCP_SG_MVY_CTRL_5_T            vcp_sg_mvy_ctrl_5;            //0xc800c4f4L
	PE_H15A0_VCP_SG_MVY_CTRL_6_T            vcp_sg_mvy_ctrl_6;            //0xc800c4f8L
	PE_H15A0_VCP_SG_MVY_CTRL_7_T            vcp_sg_mvy_ctrl_7;            //0xc800c4fcL
	PE_H15A0_VCP_SG_MVY_CTRL_8_T            vcp_sg_mvy_ctrl_8;            //0xc800c500L
	PE_H15A0_VCP_SG_MVY_CTRL_9_T            vcp_sg_mvy_ctrl_9;            //0xc800c504L
	UINT32                             	    reserved05[2];                //0xc800c508L~0xc800c50cL
	PE_H15A0_VCP_SG_MVC_CTRL_0_T            vcp_sg_mvc_ctrl_0;            //0xc800c510L
	PE_H15A0_VCP_SG_MVC_CTRL_1_T            vcp_sg_mvc_ctrl_1;            //0xc800c514L
	PE_H15A0_VCP_SG_MVC_CTRL_2_T            vcp_sg_mvc_ctrl_2;            //0xc800c518L
	PE_H15A0_VCP_SG_MVC_CTRL_3_T            vcp_sg_mvc_ctrl_3;            //0xc800c51cL
	PE_H15A0_VCP_SG_MVC_CTRL_4_T            vcp_sg_mvc_ctrl_4;            //0xc800c520L
	PE_H15A0_VCP_SG_MVC_CTRL_5_T            vcp_sg_mvc_ctrl_5;            //0xc800c524L
	PE_H15A0_VCP_SG_MVC_CTRL_6_T            vcp_sg_mvc_ctrl_6;            //0xc800c528L
	PE_H15A0_VCP_SG_MVC_CTRL_7_T            vcp_sg_mvc_ctrl_7;            //0xc800c52cL
	PE_H15A0_VCP_SG_MVC_CTRL_8_T            vcp_sg_mvc_ctrl_8;            //0xc800c530L
	PE_H15A0_VCP_SG_MVC_CTRL_9_T            vcp_sg_mvc_ctrl_9;            //0xc800c534L
	PE_H15A0_VCP_SG_MS_CTRL_0_T             vcp_sg_ms_ctrl_0;             //0xc800c538L
	PE_H15A0_VCP_SG_MS_CTRL_1_T             vcp_sg_ms_ctrl_1;             //0xc800c53cL
	PE_H15A0_VCP_SG_MS_CTRL_2_T             vcp_sg_ms_ctrl_2;             //0xc800c540L
	PE_H15A0_VCP_SG_MS_CTRL_3_T             vcp_sg_ms_ctrl_3;             //0xc800c544L
	PE_H15A0_VCP_SG_MS_CTRL_4_T             vcp_sg_ms_ctrl_4;             //0xc800c548L
	PE_H15A0_VCP_SG_MS_CTRL_5_T             vcp_sg_ms_ctrl_5;             //0xc800c54cL
	PE_H15A0_VCP_SG_MS_CTRL_6_T             vcp_sg_ms_ctrl_6;             //0xc800c550L
	PE_H15A0_VCP_SG_MS_CTRL_7_T             vcp_sg_ms_ctrl_7;             //0xc800c554L
	PE_H15A0_VCP_SG_MS_CTRL_8_T             vcp_sg_ms_ctrl_8;             //0xc800c558L
	PE_H15A0_VCP_SG_MS_CTRL_9_T             vcp_sg_ms_ctrl_9;             //0xc800c55cL
	PE_H15A0_VCP_SG_SVY_CTRL_0_T            vcp_sg_svy_ctrl_0;            //0xc800c560L
	PE_H15A0_VCP_SG_SVY_CTRL_1_T            vcp_sg_svy_ctrl_1;            //0xc800c564L
	PE_H15A0_VCP_SG_SVY_CTRL_2_T            vcp_sg_svy_ctrl_2;            //0xc800c568L
	PE_H15A0_VCP_SG_SVY_CTRL_3_T            vcp_sg_svy_ctrl_3;            //0xc800c56cL
	PE_H15A0_VCP_SG_SVY_CTRL_4_T            vcp_sg_svy_ctrl_4;            //0xc800c570L
	PE_H15A0_VCP_SG_SVY_CTRL_5_T            vcp_sg_svy_ctrl_5;            //0xc800c574L
	PE_H15A0_VCP_SG_SVY_CTRL_6_T            vcp_sg_svy_ctrl_6;            //0xc800c578L
	PE_H15A0_VCP_SG_SVY_CTRL_7_T            vcp_sg_svy_ctrl_7;            //0xc800c57cL
	PE_H15A0_VCP_SG_SVY_CTRL_8_T            vcp_sg_svy_ctrl_8;            //0xc800c580L
	PE_H15A0_VCP_SG_SVY_CTRL_9_T            vcp_sg_svy_ctrl_9;            //0xc800c584L
	UINT32                             	    reserved06[2];                //0xc800c588L~0xc800c58cL
	PE_H15A0_VCP_SG_SVC_CTRL_0_T            vcp_sg_svc_ctrl_0;            //0xc800c590L
	PE_H15A0_VCP_SG_SVC_CTRL_1_T            vcp_sg_svc_ctrl_1;            //0xc800c594L
	PE_H15A0_VCP_SG_SVC_CTRL_2_T            vcp_sg_svc_ctrl_2;            //0xc800c598L
	PE_H15A0_VCP_SG_SVC_CTRL_3_T            vcp_sg_svc_ctrl_3;            //0xc800c59cL
	PE_H15A0_VCP_SG_SVC_CTRL_4_T            vcp_sg_svc_ctrl_4;            //0xc800c5a0L
	PE_H15A0_VCP_SG_SVC_CTRL_5_T            vcp_sg_svc_ctrl_5;            //0xc800c5a4L
	PE_H15A0_VCP_SG_SVC_CTRL_6_T            vcp_sg_svc_ctrl_6;            //0xc800c5a8L
	PE_H15A0_VCP_SG_SVC_CTRL_7_T            vcp_sg_svc_ctrl_7;            //0xc800c5acL
	PE_H15A0_VCP_SG_SVC_CTRL_8_T            vcp_sg_svc_ctrl_8;            //0xc800c5b0L
	PE_H15A0_VCP_SG_SVC_CTRL_9_T            vcp_sg_svc_ctrl_9;            //0xc800c5b4L
	PE_H15A0_VCP_WCP_M_DISP_SIZE_T          vcp_wcp_m_disp_size;          //0xc800c5b8L
	PE_H15A0_VCP_WCP_M_PG_PAT_CTRL_T        vcp_wcp_m_pg_pat_ctrl;        //0xc800c5bcL
	PE_H15A0_VCP_WCP_S_DISP_SIZE_T          vcp_wcp_s_disp_size;          //0xc800c5c0L
	PE_H15A0_VCP_WCP_S_PG_PAT_CTRL_T        vcp_wcp_s_pg_pat_ctrl;        //0xc800c5c4L
	PE_H15A0_VCP_WCP_M_PIXEL_READ_CTRL_T    vcp_wcp_m_pixel_read_ctrl;    //0xc800c5c8L
	PE_H15A0_VCP_WCP_M_PIXEL_READ_DATA_E_T  vcp_wcp_m_pixel_read_data_e;  //0xc800c5ccL
	PE_H15A0_VCP_WCP_M_LINE_CTRL0_T         vcp_wcp_m_line_ctrl0;         //0xc800c5d0L
	PE_H15A0_VCP_WCP_M_LINE_CTRL1_T         vcp_wcp_m_line_ctrl1;         //0xc800c5d4L
	PE_H15A0_VCP_WCP_S_LINE_CTRL0_T         vcp_wcp_s_line_ctrl0;         //0xc800c5d8L
	PE_H15A0_VCP_WCP_S_LINE_CTRL1_T         vcp_wcp_s_line_ctrl1;         //0xc800c5dcL
	PE_H15A0_VCP_WCP_M_REC0_POS_X_T         vcp_wcp_m_rec0_pos_x;         //0xc800c5e0L
	PE_H15A0_VCP_WCP_M_REC0_POS_Y_T         vcp_wcp_m_rec0_pos_y;         //0xc800c5e4L
	PE_H15A0_VCP_WCP_M_REC1_POS_X_T         vcp_wcp_m_rec1_pos_x;         //0xc800c5e8L
	PE_H15A0_VCP_WCP_M_REC1_POS_Y_T         vcp_wcp_m_rec1_pos_y;         //0xc800c5ecL
	PE_H15A0_VCP_WCP_M_CTRL_0_T             vcp_wcp_m_ctrl_0;             //0xc800c5f0L
	PE_H15A0_VCP_WCP_M_CTRL_1_T             vcp_wcp_m_ctrl_1;             //0xc800c5f4L
	PE_H15A0_VCP_WCP_M_CTRL_2_T             vcp_wcp_m_ctrl_2;             //0xc800c5f8L
	PE_H15A0_VCP_WCP_M_CTRL_3_T             vcp_wcp_m_ctrl_3;             //0xc800c5fcL
	PE_H15A0_VCP_WCP_M_CTRL_4_T             vcp_wcp_m_ctrl_4;             //0xc800c600L
	PE_H15A0_VCP_WCP_M_CTRL_5_T             vcp_wcp_m_ctrl_5;             //0xc800c604L
	PE_H15A0_VCP_WCP_M_CTRL_6_T             vcp_wcp_m_ctrl_6;             //0xc800c608L
	PE_H15A0_VCP_WCP_M_CTRL_7_T             vcp_wcp_m_ctrl_7;             //0xc800c60cL
	PE_H15A0_VCP_WCP_M_CTRL_8_T             vcp_wcp_m_ctrl_8;             //0xc800c610L
	PE_H15A0_VCP_WCP_M_CTRL_9_T             vcp_wcp_m_ctrl_9;             //0xc800c614L
	PE_H15A0_VCP_WCP_M_CTRL_10_T            vcp_wcp_m_ctrl_10;            //0xc800c618L
	UINT32                             	    reserved07;                   //0xc800c61cL
	PE_H15A0_VCP_WCP_S_CTRL_0_T             vcp_wcp_s_ctrl_0;             //0xc800c620L
	PE_H15A0_VCP_WCP_S_CTRL_1_T             vcp_wcp_s_ctrl_1;             //0xc800c624L
	PE_H15A0_VCP_WCP_S_CTRL_2_T             vcp_wcp_s_ctrl_2;             //0xc800c628L
	PE_H15A0_VCP_WCP_S_CTRL_3_T             vcp_wcp_s_ctrl_3;             //0xc800c62cL
	PE_H15A0_VCP_WCP_S_CTRL_4_T             vcp_wcp_s_ctrl_4;             //0xc800c630L
	PE_H15A0_VCP_WCP_S_CTRL_5_T             vcp_wcp_s_ctrl_5;             //0xc800c634L
	PE_H15A0_VCP_WCP_S_CTRL_6_T             vcp_wcp_s_ctrl_6;             //0xc800c638L
	PE_H15A0_VCP_WCP_S_CTRL_7_T             vcp_wcp_s_ctrl_7;             //0xc800c63cL
	PE_H15A0_VCP_WCP_S_CTRL_8_T             vcp_wcp_s_ctrl_8;             //0xc800c640L
	PE_H15A0_VCP_WCP_S_CTRL_9_T             vcp_wcp_s_ctrl_9;             //0xc800c644L
	PE_H15A0_VCP_WCP_S_CTRL_10_T            vcp_wcp_s_ctrl_10;            //0xc800c648L
	UINT32                             	    reserved08;                   //0xc800c64cL
	PE_H15A0_VCP_WCP_SYNC_CTRL0_T           vcp_wcp_sync_ctrl0;           //0xc800c650L
	PE_H15A0_VCP_WCP_SYNC_CTRL1_T           vcp_wcp_sync_ctrl1;           //0xc800c654L
	UINT32                             	    reserved09;                   //0xc800c658L
	PE_H15A0_VCP_WCP_M_PIXEL_READ_DATA_O_T  vcp_wcp_m_pixel_read_data_o;  //0xc800c65cL
	PE_H15A0_VCP_WCP_ME_LRDIFF_CNT_YY_O_T   vcp_wcp_me_lrdiff_cnt_yy_o;   //0xc800c660L
	PE_H15A0_VCP_WCP_ME_LRDIFF_CNT_CC_O_T   vcp_wcp_me_lrdiff_cnt_cc_o;   //0xc800c664L
	PE_H15A0_VCP_WCP_MO_LRDIFF_CNT_YY_O_T   vcp_wcp_mo_lrdiff_cnt_yy_o;   //0xc800c668L
	PE_H15A0_VCP_WCP_MO_LRDIFF_CNT_CC_O_T   vcp_wcp_mo_lrdiff_cnt_cc_o;   //0xc800c66cL
	PE_H15A0_VCP_WCP_SE_LRDIFF_CNT_YY_O_T   vcp_wcp_se_lrdiff_cnt_yy_o;   //0xc800c670L
	PE_H15A0_VCP_WCP_SE_LRDIFF_CNT_CC_O_T   vcp_wcp_se_lrdiff_cnt_cc_o;   //0xc800c674L
	PE_H15A0_VCP_WCP_SO_LRDIFF_CNT_YY_O_T   vcp_wcp_so_lrdiff_cnt_yy_o;   //0xc800c678L
	PE_H15A0_VCP_WCP_SO_LRDIFF_CNT_CC_O_T   vcp_wcp_so_lrdiff_cnt_cc_o;   //0xc800c67cL
	PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_0_T     vcp_sg_bdr_v0_e_m_ctrl_0;     //0xc800c680L
	PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_1_T     vcp_sg_bdr_v0_e_m_ctrl_1;     //0xc800c684L
	PE_H15A0_VCP_SG_BDR_V0_E_M_CTRL_2_T     vcp_sg_bdr_v0_e_m_ctrl_2;     //0xc800c688L
	UINT32                             	    reserved10;                   //0xc800c68cL
	PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_0_T     vcp_sg_bdr_v0_o_m_ctrl_0;     //0xc800c690L
	PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_1_T     vcp_sg_bdr_v0_o_m_ctrl_1;     //0xc800c694L
	PE_H15A0_VCP_SG_BDR_V0_O_M_CTRL_2_T     vcp_sg_bdr_v0_o_m_ctrl_2;     //0xc800c698L
	UINT32                             	    reserved11;                   //0xc800c69cL
	PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_0_T     vcp_sg_bdr_v1_e_m_ctrl_0;     //0xc800c6a0L
	PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_1_T     vcp_sg_bdr_v1_e_m_ctrl_1;     //0xc800c6a4L
	PE_H15A0_VCP_SG_BDR_V1_E_M_CTRL_2_T     vcp_sg_bdr_v1_e_m_ctrl_2;     //0xc800c6a8L
	UINT32                             	    reserved12;                   //0xc800c6acL
	PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_0_T     vcp_sg_bdr_v1_o_m_ctrl_0;     //0xc800c6b0L
	PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_1_T     vcp_sg_bdr_v1_o_m_ctrl_1;     //0xc800c6b4L
	PE_H15A0_VCP_SG_BDR_V1_O_M_CTRL_2_T     vcp_sg_bdr_v1_o_m_ctrl_2;     //0xc800c6b8L
	UINT32                             	    reserved13;                   //0xc800c6bcL
	PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_0_T     vcp_sg_bdr_v2_e_m_ctrl_0;     //0xc800c6c0L
	PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_1_T     vcp_sg_bdr_v2_e_m_ctrl_1;     //0xc800c6c4L
	PE_H15A0_VCP_SG_BDR_V2_E_M_CTRL_2_T     vcp_sg_bdr_v2_e_m_ctrl_2;     //0xc800c6c8L
	UINT32                             	    reserved14;                   //0xc800c6ccL
	PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_0_T     vcp_sg_bdr_v2_o_m_ctrl_0;     //0xc800c6d0L
	PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_1_T     vcp_sg_bdr_v2_o_m_ctrl_1;     //0xc800c6d4L
	PE_H15A0_VCP_SG_BDR_V2_O_M_CTRL_2_T     vcp_sg_bdr_v2_o_m_ctrl_2;     //0xc800c6d8L
	UINT32                             	    reserved15;                   //0xc800c6dcL
	PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_0_T     vcp_sg_bdr_v3_e_m_ctrl_0;     //0xc800c6e0L
	PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_1_T     vcp_sg_bdr_v3_e_m_ctrl_1;     //0xc800c6e4L
	PE_H15A0_VCP_SG_BDR_V3_E_M_CTRL_2_T     vcp_sg_bdr_v3_e_m_ctrl_2;     //0xc800c6e8L
	UINT32                             	    reserved16;                   //0xc800c6ecL
	PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_0_T     vcp_sg_bdr_v3_o_m_ctrl_0;     //0xc800c6f0L
	PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_1_T     vcp_sg_bdr_v3_o_m_ctrl_1;     //0xc800c6f4L
	PE_H15A0_VCP_SG_BDR_V3_O_M_CTRL_2_T     vcp_sg_bdr_v3_o_m_ctrl_2;     //0xc800c6f8L
	UINT32                             	    reserved17;                   //0xc800c6fcL
	PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_0_T     vcp_sg_bdr_v0_e_s_ctrl_0;     //0xc800c700L
	PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_1_T     vcp_sg_bdr_v0_e_s_ctrl_1;     //0xc800c704L
	PE_H15A0_VCP_SG_BDR_V0_E_S_CTRL_2_T     vcp_sg_bdr_v0_e_s_ctrl_2;     //0xc800c708L
	UINT32                             	    reserved18;                   //0xc800c70cL
	PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_0_T     vcp_sg_bdr_v0_o_s_ctrl_0;     //0xc800c710L
	PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_1_T     vcp_sg_bdr_v0_o_s_ctrl_1;     //0xc800c714L
	PE_H15A0_VCP_SG_BDR_V0_O_S_CTRL_2_T     vcp_sg_bdr_v0_o_s_ctrl_2;     //0xc800c718L
	UINT32                             	    reserved19;                   //0xc800c71cL
	PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_0_T     vcp_sg_bdr_v1_e_s_ctrl_0;     //0xc800c720L
	PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_1_T     vcp_sg_bdr_v1_e_s_ctrl_1;     //0xc800c724L
	PE_H15A0_VCP_SG_BDR_V1_E_S_CTRL_2_T     vcp_sg_bdr_v1_e_s_ctrl_2;     //0xc800c728L
	UINT32                             	    reserved20;                   //0xc800c72cL
	PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_0_T     vcp_sg_bdr_v1_o_s_ctrl_0;     //0xc800c730L
	PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_1_T     vcp_sg_bdr_v1_o_s_ctrl_1;     //0xc800c734L
	PE_H15A0_VCP_SG_BDR_V1_O_S_CTRL_2_T     vcp_sg_bdr_v1_o_s_ctrl_2;     //0xc800c738L
	UINT32                             	    reserved21;                   //0xc800c73cL
	PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_0_T     vcp_sg_bdr_v2_e_s_ctrl_0;     //0xc800c740L
	PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_1_T     vcp_sg_bdr_v2_e_s_ctrl_1;     //0xc800c744L
	PE_H15A0_VCP_SG_BDR_V2_E_S_CTRL_2_T     vcp_sg_bdr_v2_e_s_ctrl_2;     //0xc800c748L
	UINT32                             	    reserved22;                   //0xc800c74cL
	PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_0_T     vcp_sg_bdr_v2_o_s_ctrl_0;     //0xc800c750L
	PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_1_T     vcp_sg_bdr_v2_o_s_ctrl_1;     //0xc800c754L
	PE_H15A0_VCP_SG_BDR_V2_O_S_CTRL_2_T     vcp_sg_bdr_v2_o_s_ctrl_2;     //0xc800c758L
	UINT32                             	    reserved23;                   //0xc800c75cL
	PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_0_T     vcp_sg_bdr_v3_e_s_ctrl_0;     //0xc800c760L
	PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_1_T     vcp_sg_bdr_v3_e_s_ctrl_1;     //0xc800c764L
	PE_H15A0_VCP_SG_BDR_V3_E_S_CTRL_2_T     vcp_sg_bdr_v3_e_s_ctrl_2;     //0xc800c768L
	UINT32                             	    reserved24;                   //0xc800c76cL
	PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_0_T     vcp_sg_bdr_v3_o_s_ctrl_0;     //0xc800c770L
	PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_1_T     vcp_sg_bdr_v3_o_s_ctrl_1;     //0xc800c774L
	PE_H15A0_VCP_SG_BDR_V3_O_S_CTRL_2_T     vcp_sg_bdr_v3_o_s_ctrl_2;     //0xc800c778L
	UINT32                             	    reserved25;                   //0xc800c77cL
	PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_0_T    vcp_sg_bdr_v0_e_ms_ctrl_0;    //0xc800c780L
	PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_1_T    vcp_sg_bdr_v0_e_ms_ctrl_1;    //0xc800c784L
	PE_H15A0_VCP_SG_BDR_V0_E_MS_CTRL_2_T    vcp_sg_bdr_v0_e_ms_ctrl_2;    //0xc800c788L
	UINT32                             	    reserved26;                   //0xc800c78cL
	PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_0_T    vcp_sg_bdr_v0_o_ms_ctrl_0;    //0xc800c790L
	PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_1_T    vcp_sg_bdr_v0_o_ms_ctrl_1;    //0xc800c794L
	PE_H15A0_VCP_SG_BDR_V0_O_MS_CTRL_2_T    vcp_sg_bdr_v0_o_ms_ctrl_2;    //0xc800c798L
	UINT32                             	    reserved27;                   //0xc800c79cL
	PE_H15A0_VCP_WCP_MS_DISP_SIZE_T         vcp_wcp_ms_disp_size;         //0xc800c7a0L
	PE_H15A0_VCP_WCP_MS_PG_PAT_CTRL_T       vcp_wcp_ms_pg_pat_ctrl;       //0xc800c7a4L
	PE_H15A0_VCP_WCP_MS_PIXEL_READ_CTRL_T   vcp_wcp_ms_pixel_read_ctrl;   //0xc800c7a8L
	PE_H15A0_VCP_WCP_MS_PIXEL_READ_DATA_E_T vcp_wcp_ms_pixel_read_data_e; //0xc800c7acL
	PE_H15A0_VCP_WCP_MS_PIXEL_READ_DATA_O_T vcp_wcp_ms_pixel_read_data_o; //0xc800c7b0L
	PE_H15A0_VCP_WCP_MS_CTRL_2_T            vcp_wcp_ms_ctrl_2;            //0xc800c7b4L
	PE_H15A0_VCP_WCP_MS_CTRL_3_T            vcp_wcp_ms_ctrl_3;            //0xc800c7b8L
	PE_H15A0_VCP_WCP_MS_CTRL_5_T            vcp_wcp_ms_ctrl_5;            //0xc800c7bcL
	PE_H15A0_VCP_WCP_MS_CTRL_9_T            vcp_wcp_ms_ctrl_9;            //0xc800c7c0L
	PE_H15A0_VCP_WCP_MS_CTRL_10_T           vcp_wcp_ms_ctrl_10;           //0xc800c7c4L
	UINT32                             	    reserved28[14];               //0xc800c7c8L~0xc800c7fcL
	PE_H15A0_VCP_WCP_ALPHA_CTRL_T           vcp_wcp_alpha_ctrl;           //0xc800c800L
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_0_T        vcp_wcp_alpha_gen_y_0;        //0xc800c804L
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_1_T        vcp_wcp_alpha_gen_y_1;        //0xc800c808L
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_2_T        vcp_wcp_alpha_gen_y_2;        //0xc800c80cL
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_3_T        vcp_wcp_alpha_gen_y_3;        //0xc800c810L
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_4_T        vcp_wcp_alpha_gen_y_4;        //0xc800c814L
	PE_H15A0_VCP_WCP_ALPHA_GEN_Y_5_T        vcp_wcp_alpha_gen_y_5;        //0xc800c818L
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_0_T        vcp_wcp_alpha_gen_c_0;        //0xc800c81cL
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_1_T        vcp_wcp_alpha_gen_c_1;        //0xc800c820L
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_2_T        vcp_wcp_alpha_gen_c_2;        //0xc800c824L
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_3_T        vcp_wcp_alpha_gen_c_3;        //0xc800c828L
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_4_T        vcp_wcp_alpha_gen_c_4;        //0xc800c82cL
	PE_H15A0_VCP_WCP_ALPHA_GEN_C_5_T        vcp_wcp_alpha_gen_c_5;        //0xc800c830L
	PE_H15A0_VCP_WCP_M_REC2_POS_X_T         vcp_wcp_m_rec2_pos_x;         //0xc800c834L
	PE_H15A0_VCP_WCP_M_REC2_POS_Y_T         vcp_wcp_m_rec2_pos_y;         //0xc800c838L
	PE_H15A0_VCP_WCP_M_REC3_POS_X_T         vcp_wcp_m_rec3_pos_x;         //0xc800c83cL
	PE_H15A0_VCP_WCP_M_REC3_POS_Y_T         vcp_wcp_m_rec3_pos_y;         //0xc800c840L
	PE_H15A0_VCP_WCP_M_REC4_POS_X_T         vcp_wcp_m_rec4_pos_x;         //0xc800c844L
	PE_H15A0_VCP_WCP_M_REC4_POS_Y_T         vcp_wcp_m_rec4_pos_y;         //0xc800c848L
	PE_H15A0_VCP_WCP_M_REC0_O_POS_X_T       vcp_wcp_m_rec0_o_pos_x;       //0xc800c84cL
	PE_H15A0_VCP_WCP_M_REC0_O_POS_Y_T       vcp_wcp_m_rec0_o_pos_y;       //0xc800c850L
	PE_H15A0_VCP_WCP_M_REC1_O_POS_X_T       vcp_wcp_m_rec1_o_pos_x;       //0xc800c854L
	PE_H15A0_VCP_WCP_M_REC1_O_POS_Y_T       vcp_wcp_m_rec1_o_pos_y;       //0xc800c858L
	PE_H15A0_VCP_WCP_M_REC2_O_POS_X_T       vcp_wcp_m_rec2_o_pos_x;       //0xc800c85cL
	PE_H15A0_VCP_WCP_M_REC2_O_POS_Y_T       vcp_wcp_m_rec2_o_pos_y;       //0xc800c860L
	PE_H15A0_VCP_WCP_M_REC3_O_POS_X_T       vcp_wcp_m_rec3_o_pos_x;       //0xc800c864L
	PE_H15A0_VCP_WCP_M_REC3_O_POS_Y_T       vcp_wcp_m_rec3_o_pos_y;       //0xc800c868L
	PE_H15A0_VCP_WCP_M_REC4_O_POS_X_T       vcp_wcp_m_rec4_o_pos_x;       //0xc800c86cL
	PE_H15A0_VCP_WCP_M_REC4_O_POS_Y_T       vcp_wcp_m_rec4_o_pos_y;       //0xc800c870L
	PE_H15A0_VCP_SG_SRY_CTRL_0_T            vcp_sg_sry_ctrl_0;            //0xc800c874L
	PE_H15A0_VCP_SG_SRY_CTRL_1_T            vcp_sg_sry_ctrl_1;            //0xc800c878L
	PE_H15A0_VCP_SG_SRY_CTRL_2_T            vcp_sg_sry_ctrl_2;            //0xc800c87cL
	PE_H15A0_VCP_SG_SRY_CTRL_3_T            vcp_sg_sry_ctrl_3;            //0xc800c880L
	PE_H15A0_VCP_SG_SRY_CTRL_4_T            vcp_sg_sry_ctrl_4;            //0xc800c884L
	PE_H15A0_VCP_SG_SRY_CTRL_5_T            vcp_sg_sry_ctrl_5;            //0xc800c888L
	PE_H15A0_VCP_SG_SRY_CTRL_6_T            vcp_sg_sry_ctrl_6;            //0xc800c88cL
	PE_H15A0_VCP_SG_SRY_CTRL_7_T            vcp_sg_sry_ctrl_7;            //0xc800c890L
	PE_H15A0_VCP_SG_SRY_CTRL_8_T            vcp_sg_sry_ctrl_8;            //0xc800c894L
	PE_H15A0_VCP_SG_SRY_CTRL_9_T            vcp_sg_sry_ctrl_9;            //0xc800c898L
	PE_H15A0_VCP_SG_SRY_CTRL_10_T           vcp_sg_sry_ctrl_10;           //0xc800c89cL
	PE_H15A0_VCP_SG_MVY_CTRL_A_T            vcp_sg_mvy_ctrl_a;            //0xc800c8a0L
	PE_H15A0_VCP_SG_MVC_CTRL_A_T            vcp_sg_mvc_ctrl_a;            //0xc800c8a4L
	PE_H15A0_VCP_SG_SVY_CTRL_A_T            vcp_sg_svy_ctrl_a;            //0xc800c8a8L
	PE_H15A0_VCP_SG_SVC_CTRL_A_T            vcp_sg_svc_ctrl_a;            //0xc800c8acL
	PE_H15A0_VCP_SG_SRY_CTRL_A_T            vcp_sg_sry_ctrl_a;            //0xc800c8b0L
	UINT32                             	    reserved29[19];               //0xc800c8b4L~0xc800c8fcL
	PE_H15A0_VCP_DISP_INTR_MASK_T           vcp_disp_intr_mask;           //0xc800c900L
	PE_H15A0_VCP_DISP_INTR_CLEAR_T          vcp_disp_intr_clear;          //0xc800c904L
	PE_H15A0_VCP_DISP_INTR_STATUS_T         vcp_disp_intr_status;         //0xc800c908L
	PE_H15A0_VCP_DISP_INTR0_POS_MV_Y_T      vcp_disp_intr0_pos_mv_y;      //0xc800c90cL
	PE_H15A0_VCP_DISP_INTR1_POS_MV_Y_T      vcp_disp_intr1_pos_mv_y;      //0xc800c910L
	PE_H15A0_VCP_DISP_INTR0_POS_MV_C_T      vcp_disp_intr0_pos_mv_c;      //0xc800c914L
	PE_H15A0_VCP_DISP_INTR1_POS_MV_C_T      vcp_disp_intr1_pos_mv_c;      //0xc800c918L
	PE_H15A0_VCP_DISP_INTR0_POS_SV_Y_T      vcp_disp_intr0_pos_sv_y;      //0xc800c91cL
	PE_H15A0_VCP_DISP_INTR1_POS_SV_Y_T      vcp_disp_intr1_pos_sv_y;      //0xc800c920L
	PE_H15A0_VCP_DISP_INTR0_POS_SV_C_T      vcp_disp_intr0_pos_sv_c;      //0xc800c924L
	PE_H15A0_VCP_DISP_INTR1_POS_SV_C_T      vcp_disp_intr1_pos_sv_c;      //0xc800c928L
	PE_H15A0_VCP_DISP_INTR_SRC_SEL_T        vcp_disp_intr_src_sel;        //0xc800c92cL
}PE_VCP_DISP_REG_H15A0_T;

typedef struct {
	PE_H15A0_VCP_SO_CTRL0_T               	vcp_so_ctrl0;                 //0xc800cc00L
	PE_H15A0_VCP_SO_CTRL1_T               	vcp_so_ctrl1;                 //0xc800cc04L
	PE_H15A0_VCP_SO_CTRL2_T               	vcp_so_ctrl2;                 //0xc800cc08L
	PE_H15A0_VCP_CDC_SO_RST_CTRL_T        	vcp_cdc_so_rst_ctrl;          //0xc800cc0cL
	PE_H15A0_VCP_U1_SO_WIN_O_POS_T        	vcp_u1_so_win_o_pos;          //0xc800cc10L
	PE_H15A0_VCP_U1_SO_WIN_O_SIZE_T       	vcp_u1_so_win_o_size;         //0xc800cc14L
	PE_H15A0_VCP_U1_SO_PG_PAT_CTRL_T      	vcp_u1_so_pg_pat_ctrl;        //0xc800cc18L
	UINT32                             	    reserved01;                   //0xc800cc1cL
	PE_H15A0_VCP_U1_SO_CDC_Y_CTRL0_T      	vcp_u1_so_cdc_y_ctrl0;        //0xc800cc20L
	PE_H15A0_VCP_U1_SO_CDC_Y_CTRL1_T      	vcp_u1_so_cdc_y_ctrl1;        //0xc800cc24L
	PE_H15A0_VCP_U1_SO_CDC_Y_CTRL2_T      	vcp_u1_so_cdc_y_ctrl2;        //0xc800cc28L
	PE_H15A0_VCP_U1_SO_CDC_Y_STATUS_T     	vcp_u1_so_cdc_y_status;       //0xc800cc2cL
	PE_H15A0_VCP_U1_SO_CDC_C_CTRL0_T      	vcp_u1_so_cdc_c_ctrl0;        //0xc800cc30L
	PE_H15A0_VCP_U1_SO_CDC_C_CTRL1_T      	vcp_u1_so_cdc_c_ctrl1;        //0xc800cc34L
	PE_H15A0_VCP_U1_SO_CDC_C_CTRL2_T      	vcp_u1_so_cdc_c_ctrl2;        //0xc800cc38L
	PE_H15A0_VCP_U1_SO_CDC_C_STATUS_T     	vcp_u1_so_cdc_c_status;       //0xc800cc3cL
	PE_H15A0_VCP_U2_SO_WIN_O_POS_T        	vcp_u2_so_win_o_pos;          //0xc800cc40L
	PE_H15A0_VCP_U2_SO_WIN_O_SIZE_T       	vcp_u2_so_win_o_size;         //0xc800cc44L
	PE_H15A0_VCP_U2_SO_PG_PAT_CTRL_T      	vcp_u2_so_pg_pat_ctrl;        //0xc800cc48L
	UINT32                             	    reserved02;                   //0xc800cc4cL
	PE_H15A0_VCP_U2_SO_CDC_Y_CTRL0_T      	vcp_u2_so_cdc_y_ctrl0;        //0xc800cc50L
	PE_H15A0_VCP_U2_SO_CDC_Y_CTRL1_T      	vcp_u2_so_cdc_y_ctrl1;        //0xc800cc54L
	PE_H15A0_VCP_U2_SO_CDC_Y_CTRL2_T      	vcp_u2_so_cdc_y_ctrl2;        //0xc800cc58L
	PE_H15A0_VCP_U2_SO_CDC_Y_STATUS_T     	vcp_u2_so_cdc_y_status;       //0xc800cc5cL
	PE_H15A0_VCP_U2_SO_CDC_C_CTRL0_T      	vcp_u2_so_cdc_c_ctrl0;        //0xc800cc60L
	PE_H15A0_VCP_U2_SO_CDC_C_CTRL1_T      	vcp_u2_so_cdc_c_ctrl1;        //0xc800cc64L
	PE_H15A0_VCP_U2_SO_CDC_C_CTRL2_T      	vcp_u2_so_cdc_c_ctrl2;        //0xc800cc68L
	PE_H15A0_VCP_U2_SO_CDC_C_STATUS_T     	vcp_u2_so_cdc_c_status;       //0xc800cc6cL
	PE_H15A0_VCP_F1_SO_WIN_O_POS_T        	vcp_f1_so_win_o_pos;          //0xc800cc70L
	PE_H15A0_VCP_F1_SO_WIN_O_SIZE_T       	vcp_f1_so_win_o_size;         //0xc800cc74L
	PE_H15A0_VCP_F1_SO_PG_PAT_CTRL_T      	vcp_f1_so_pg_pat_ctrl;        //0xc800cc78L
	UINT32                             	    reserved03;                   //0xc800cc7cL
	PE_H15A0_VCP_F1_SO_CDC_Y_CTRL0_T      	vcp_f1_so_cdc_y_ctrl0;        //0xc800cc80L
	PE_H15A0_VCP_F1_SO_CDC_Y_CTRL1_T      	vcp_f1_so_cdc_y_ctrl1;        //0xc800cc84L
	PE_H15A0_VCP_F1_SO_CDC_Y_CTRL2_T      	vcp_f1_so_cdc_y_ctrl2;        //0xc800cc88L
	PE_H15A0_VCP_F1_SO_CDC_Y_STATUS_T     	vcp_f1_so_cdc_y_status;       //0xc800cc8cL
	PE_H15A0_VCP_F1_SO_CDC_C_CTRL0_T      	vcp_f1_so_cdc_c_ctrl0;        //0xc800cc90L
	PE_H15A0_VCP_F1_SO_CDC_C_CTRL1_T      	vcp_f1_so_cdc_c_ctrl1;        //0xc800cc94L
	PE_H15A0_VCP_F1_SO_CDC_C_CTRL2_T      	vcp_f1_so_cdc_c_ctrl2;        //0xc800cc98L
	PE_H15A0_VCP_F1_SO_CDC_C_STATUS_T     	vcp_f1_so_cdc_c_status;       //0xc800cc9cL
	PE_H15A0_VCP_F2_SO_WIN_O_POS_T        	vcp_f2_so_win_o_pos;          //0xc800cca0L
	PE_H15A0_VCP_F2_SO_WIN_O_SIZE_T       	vcp_f2_so_win_o_size;         //0xc800cca4L
	PE_H15A0_VCP_F2_SO_PG_PAT_CTRL_T      	vcp_f2_so_pg_pat_ctrl;        //0xc800cca8L
	UINT32                             	    reserved04;                   //0xc800ccacL
	PE_H15A0_VCP_F2_SO_CDC_Y_CTRL0_T      	vcp_f2_so_cdc_y_ctrl0;        //0xc800ccb0L
	PE_H15A0_VCP_F2_SO_CDC_Y_CTRL1_T      	vcp_f2_so_cdc_y_ctrl1;        //0xc800ccb4L
	PE_H15A0_VCP_F2_SO_CDC_Y_CTRL2_T      	vcp_f2_so_cdc_y_ctrl2;        //0xc800ccb8L
	PE_H15A0_VCP_F2_SO_CDC_Y_STATUS_T     	vcp_f2_so_cdc_y_status;       //0xc800ccbcL
	PE_H15A0_VCP_F2_SO_CDC_C_CTRL0_T      	vcp_f2_so_cdc_c_ctrl0;        //0xc800ccc0L
	PE_H15A0_VCP_F2_SO_CDC_C_CTRL1_T      	vcp_f2_so_cdc_c_ctrl1;        //0xc800ccc4L
	PE_H15A0_VCP_F2_SO_CDC_C_CTRL2_T      	vcp_f2_so_cdc_c_ctrl2;        //0xc800ccc8L
	PE_H15A0_VCP_F2_SO_CDC_C_STATUS_T     	vcp_f2_so_cdc_c_status;       //0xc800ccccL
	PE_H15A0_VCP_SG_SOSD_CTRL_0_T         	vcp_sg_sosd_ctrl_0;           //0xc800ccd0L
	PE_H15A0_VCP_SG_SOSD_CTRL_1_T         	vcp_sg_sosd_ctrl_1;           //0xc800ccd4L
	PE_H15A0_VCP_SG_SOSD_CTRL_2_T         	vcp_sg_sosd_ctrl_2;           //0xc800ccd8L
	PE_H15A0_VCP_SG_SOSD_CTRL_3_T         	vcp_sg_sosd_ctrl_3;           //0xc800ccdcL
	PE_H15A0_VCP_SG_SOSD_CTRL_4_T         	vcp_sg_sosd_ctrl_4;           //0xc800cce0L
	PE_H15A0_VCP_SG_SOSD_CTRL_5_T         	vcp_sg_sosd_ctrl_5;           //0xc800cce4L
	PE_H15A0_VCP_SG_SOSD_CTRL_6_T         	vcp_sg_sosd_ctrl_6;           //0xc800cce8L
	PE_H15A0_VCP_SG_SOSD_CTRL_7_T         	vcp_sg_sosd_ctrl_7;           //0xc800ccecL
	PE_H15A0_VCP_SG_SOSD_CTRL_8_T         	vcp_sg_sosd_ctrl_8;           //0xc800ccf0L
	PE_H15A0_VCP_SG_SOSD_CTRL_9_T         	vcp_sg_sosd_ctrl_9;           //0xc800ccf4L
	PE_H15A0_VCP_SG_SOSD_CTRL_A_T         	vcp_sg_sosd_ctrl_a;           //0xc800ccf8L
	UINT32                             	    reserved05;                   //0xc800ccfcL
	PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_0_T    	vcp_sg_so_bdr_v0_ctrl_0;      //0xc800cd00L
	PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_1_T    	vcp_sg_so_bdr_v0_ctrl_1;      //0xc800cd04L
	PE_H15A0_VCP_SG_SO_BDR_V0_CTRL_2_T    	vcp_sg_so_bdr_v0_ctrl_2;      //0xc800cd08L
	UINT32                             	    reserved06;                   //0xc800cd0cL
	PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_0_T    	vcp_sg_so_bdr_v1_ctrl_0;      //0xc800cd10L
	PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_1_T    	vcp_sg_so_bdr_v1_ctrl_1;      //0xc800cd14L
	PE_H15A0_VCP_SG_SO_BDR_V1_CTRL_2_T    	vcp_sg_so_bdr_v1_ctrl_2;      //0xc800cd18L
	UINT32                             	    reserved07;                   //0xc800cd1cL
	PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_0_T    	vcp_sg_so_bdr_v2_ctrl_0;      //0xc800cd20L
	PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_1_T    	vcp_sg_so_bdr_v2_ctrl_1;      //0xc800cd24L
	PE_H15A0_VCP_SG_SO_BDR_V2_CTRL_2_T    	vcp_sg_so_bdr_v2_ctrl_2;      //0xc800cd28L
	UINT32                             	    reserved08;                   //0xc800cd2cL
	PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_0_T    	vcp_sg_so_bdr_v3_ctrl_0;      //0xc800cd30L
	PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_1_T    	vcp_sg_so_bdr_v3_ctrl_1;      //0xc800cd34L
	PE_H15A0_VCP_SG_SO_BDR_V3_CTRL_2_T    	vcp_sg_so_bdr_v3_ctrl_2;      //0xc800cd38L
	UINT32                             	    reserved09;                   //0xc800cd3cL
	PE_H15A0_VCP_WCP_SO_PIXEL_READ_CTRL_T 	vcp_wcp_so_pixel_read_ctrl;   //0xc800cd40L
	PE_H15A0_VCP_WCP_SO_PIXEL_READ_DATA_E_T	vcp_wcp_so_pixel_read_data_e; //0xc800cd44L
	PE_H15A0_VCP_WCP_SO_LINE_CTRL0_T      	vcp_wcp_so_line_ctrl0;        //0xc800cd48L
	PE_H15A0_VCP_WCP_SO_LINE_CTRL1_T      	vcp_wcp_so_line_ctrl1;        //0xc800cd4cL
	PE_H15A0_VCP_WCP_SO_REC0_POS_X_T      	vcp_wcp_so_rec0_pos_x;        //0xc800cd50L
	PE_H15A0_VCP_WCP_SO_REC0_POS_Y_T      	vcp_wcp_so_rec0_pos_y;        //0xc800cd54L
	PE_H15A0_VCP_WCP_SO_REC1_POS_X_T      	vcp_wcp_so_rec1_pos_x;        //0xc800cd58L
	PE_H15A0_VCP_WCP_SO_REC1_POS_Y_T      	vcp_wcp_so_rec1_pos_y;        //0xc800cd5cL
	UINT32                             	    reserved10[12];               //0xc800cd60L~0xc800cd8cL
	PE_H15A0_VCP_WCP_SO_DISP_SIZE_T       	vcp_wcp_so_disp_size;         //0xc800cd90L
	PE_H15A0_VCP_WCP_SO_PG_PAT_CTRL_T     	vcp_wcp_so_pg_pat_ctrl;       //0xc800cd94L
	UINT32                             	    reserved11[2];                //0xc800cd98L~0xc800cd9cL
	PE_H15A0_VCP_WCP_SO_CTRL_0_T          	vcp_wcp_so_ctrl_0;            //0xc800cda0L
	PE_H15A0_VCP_WCP_SO_CTRL_1_T          	vcp_wcp_so_ctrl_1;            //0xc800cda4L
	PE_H15A0_VCP_WCP_SO_CTRL_2_T          	vcp_wcp_so_ctrl_2;            //0xc800cda8L
	PE_H15A0_VCP_WCP_SO_CTRL_3_T          	vcp_wcp_so_ctrl_3;            //0xc800cdacL
	PE_H15A0_VCP_WCP_SO_CTRL_4_T          	vcp_wcp_so_ctrl_4;            //0xc800cdb0L
	PE_H15A0_VCP_WCP_SO_CTRL_5_T          	vcp_wcp_so_ctrl_5;            //0xc800cdb4L
	PE_H15A0_VCP_WCP_SO_CTRL_6_T          	vcp_wcp_so_ctrl_6;            //0xc800cdb8L
	PE_H15A0_VCP_WCP_SO_CTRL_7_T          	vcp_wcp_so_ctrl_7;            //0xc800cdbcL
	PE_H15A0_VCP_WCP_SO_CTRL_8_T          	vcp_wcp_so_ctrl_8;            //0xc800cdc0L
	PE_H15A0_VCP_WCP_SO_CTRL_9_T          	vcp_wcp_so_ctrl_9;            //0xc800cdc4L
	PE_H15A0_VCP_WCP_SO_CTRL_10_T         	vcp_wcp_so_ctrl_10;           //0xc800cdc8L
	UINT32                             	    reserved12;                   //0xc800cdccL
	PE_H15A0_VCP_WCP_SOSD_SYNC_CTRL0_T    	vcp_wcp_sosd_sync_ctrl0;      //0xc800cdd0L
	PE_H15A0_VCP_WCP_SOSD_LRDIFF_CNT_YY_O_T	vcp_wcp_sosd_lrdiff_cnt_yy_o; //0xc800cdd4L
	PE_H15A0_VCP_WCP_SOSD_LRDIFF_CNT_CC_O_T	vcp_wcp_sosd_lrdiff_cnt_cc_o; //0xc800cdd8L
	PE_H15A0_VCP_SO_INTR_MASK_T           	vcp_so_intr_mask;             //0xc800cddcL
	PE_H15A0_VCP_SO_INTR_CLEAR_T          	vcp_so_intr_clear;            //0xc800cde0L
	PE_H15A0_VCP_SO_INTR_STATUS_T         	vcp_so_intr_status;           //0xc800cde4L
	PE_H15A0_VCP_SO_INTR0_POS_T           	vcp_so_intr0_pos;             //0xc800cde8L
	PE_H15A0_VCP_SO_INTR1_POS_T           	vcp_so_intr1_pos;             //0xc800cdecL
	PE_H15A0_VCP_SO_INTR_SRC_SEL_T        	vcp_so_intr_src_sel;          //0xc800cdf0L
}PE_VCP_SO_REG_H15A0_T;

typedef struct {
	PE_H15A0_VCP_DISPH_CTRL0_T            	vcp_disph_ctrl0;              //0xc800ce00L
	PE_H15A0_VCP_CDC_HT_RST_CTRL_T        	vcp_cdc_ht_rst_ctrl;          //0xc800ce04L
	PE_H15A0_VCP_HT_WIN_O_POS_T           	vcp_ht_win_o_pos;             //0xc800ce08L
	PE_H15A0_VCP_HT_WIN_O_SIZE_T          	vcp_ht_win_o_size;            //0xc800ce0cL
	PE_H15A0_VCP_HT_PG_PAT_CTRL_T         	vcp_ht_pg_pat_ctrl;           //0xc800ce10L
	UINT32                             	    reserved01;                   //0xc800ce14L
	PE_H15A0_VCP_HT_CDC_Y_CTRL0_T         	vcp_ht_cdc_y_ctrl0;           //0xc800ce18L
	PE_H15A0_VCP_HT_CDC_Y_CTRL1_T         	vcp_ht_cdc_y_ctrl1;           //0xc800ce1cL
	PE_H15A0_VCP_HT_CDC_Y_CTRL2_T         	vcp_ht_cdc_y_ctrl2;           //0xc800ce20L
	PE_H15A0_VCP_HT_CDC_Y_STATUS_T        	vcp_ht_cdc_y_status;          //0xc800ce24L
	PE_H15A0_VCP_HT_CDC_C_CTRL0_T         	vcp_ht_cdc_c_ctrl0;           //0xc800ce28L
	PE_H15A0_VCP_HT_CDC_C_CTRL1_T         	vcp_ht_cdc_c_ctrl1;           //0xc800ce2cL
	PE_H15A0_VCP_HT_CDC_C_STATUS_T        	vcp_ht_cdc_c_status;          //0xc800ce30L
	PE_H15A0_VCP_HT_CDC_DISP_SIZE_T       	vcp_ht_cdc_disp_size;         //0xc800ce34L
	PE_H15A0_VCP_HT_PIXEL_READ_CTRL_T     	vcp_ht_pixel_read_ctrl;       //0xc800ce38L
	PE_H15A0_VCP_HT_PIXEL_READ_DATA_T     	vcp_ht_pixel_read_data;       //0xc800ce3cL
	PE_H15A0_VCP_HT_INTR_MASK_T           	vcp_ht_intr_mask;             //0xc800ce40L
	PE_H15A0_VCP_HT_INTR_CLEAR_T          	vcp_ht_intr_clear;            //0xc800ce44L
	PE_H15A0_VCP_HT_INTR_STATUS_T         	vcp_ht_intr_status;           //0xc800ce48L
	PE_H15A0_VCP_HT_INTR0_POS_T           	vcp_ht_intr0_pos;             //0xc800ce4cL
	PE_H15A0_VCP_HT_INTR1_POS_T           	vcp_ht_intr1_pos;             //0xc800ce50L
	PE_H15A0_VCP_HT_INTR_SRC_SEL_T        	vcp_ht_intr_src_sel;          //0xc800ce54L
	PE_H15A0_VCP_SG_HT_CTRL_0_T           	vcp_sg_ht_ctrl_0;             //0xc800ce58L
	PE_H15A0_VCP_SG_HT_CTRL_1_T           	vcp_sg_ht_ctrl_1;             //0xc800ce5cL
	PE_H15A0_VCP_SG_HT_CTRL_2_T           	vcp_sg_ht_ctrl_2;             //0xc800ce60L
	PE_H15A0_VCP_SG_HT_CTRL_3_T           	vcp_sg_ht_ctrl_3;             //0xc800ce64L
	PE_H15A0_VCP_SG_HT_CTRL_4_T           	vcp_sg_ht_ctrl_4;             //0xc800ce68L
	PE_H15A0_VCP_SG_HT_CTRL_5_T           	vcp_sg_ht_ctrl_5;             //0xc800ce6cL
	PE_H15A0_VCP_SG_HT_CTRL_6_T           	vcp_sg_ht_ctrl_6;             //0xc800ce70L
	PE_H15A0_VCP_SG_HT_CTRL_7_T           	vcp_sg_ht_ctrl_7;             //0xc800ce74L
	PE_H15A0_VCP_SG_HT_CTRL_8_T           	vcp_sg_ht_ctrl_8;             //0xc800ce78L
	PE_H15A0_VCP_SG_HT_CTRL_9_T           	vcp_sg_ht_ctrl_9;             //0xc800ce7cL
	PE_H15A0_VCP_SG_HT_CTRL_A_T           	vcp_sg_ht_ctrl_a;             //0xc800ce80L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_0_T       	vcp_sg_sry_ht_ctrl_0;         //0xc800ce84L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_1_T       	vcp_sg_sry_ht_ctrl_1;         //0xc800ce88L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_2_T       	vcp_sg_sry_ht_ctrl_2;         //0xc800ce8cL
	PE_H15A0_VCP_SG_SRY_HT_CTRL_3_T       	vcp_sg_sry_ht_ctrl_3;         //0xc800ce90L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_4_T       	vcp_sg_sry_ht_ctrl_4;         //0xc800ce94L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_5_T       	vcp_sg_sry_ht_ctrl_5;         //0xc800ce98L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_6_T       	vcp_sg_sry_ht_ctrl_6;         //0xc800ce9cL
	PE_H15A0_VCP_SG_SRY_HT_CTRL_7_T       	vcp_sg_sry_ht_ctrl_7;         //0xc800cea0L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_8_T       	vcp_sg_sry_ht_ctrl_8;         //0xc800cea4L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_9_T       	vcp_sg_sry_ht_ctrl_9;         //0xc800cea8L
	PE_H15A0_VCP_SG_SRY_HT_CTRL_A_T       	vcp_sg_sry_ht_ctrl_a;         //0xc800ceacL
}PE_VCP_HT_REG_H15A0_T;
#endif
