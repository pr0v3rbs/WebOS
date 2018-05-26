#ifndef _PRT0_H_
#define _PRT0_H_
/*-----------------------------------------------------------------------------
		0x1000 ad_version ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_version                        : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_VERSION_T;

/*-----------------------------------------------------------------------------
		0x1010 ad_hdcp22_skp_pkf_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_pkf_1                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_1_T;

/*-----------------------------------------------------------------------------
		0x1010 ad_hdcp22_skp_pkf_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_pkf_2                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_2_T;

/*-----------------------------------------------------------------------------
		0x1018 ad_hdcp22_skp_pkf_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_pkf_3                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_3_T;

/*-----------------------------------------------------------------------------
		0x101c ad_hdcp22_skp_pkf_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_pkf_4                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_4_T;

/*-----------------------------------------------------------------------------
		0x1020 ad_hdcp22_skp_duk_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_duk_1                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_1_T;

/*-----------------------------------------------------------------------------
		0x1024 ad_hdcp22_skp_duk_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_duk_2                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_2_T;

/*-----------------------------------------------------------------------------
		0x1028 ad_hdcp22_skp_duk_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_duk_3                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_3_T;

/*-----------------------------------------------------------------------------
		0x102c ad_hdcp22_skp_duk_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_skp_duk_4                    : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_4_T;

/*-----------------------------------------------------------------------------
		0x1030 ad_hdcp22_hpd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_ihdmi_hpd                    : 1 ;  //     0     
} H15B0_HDCP22_PRT0_AD_HDCP22_HPD_CTRL_T;

/*-----------------------------------------------------------------------------
		0x1034 ad_hdcp22_sync_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_hdcp22_hsync_pol             : 1 ;  //     0     
	UINT32 o_reg_hdcp22_vsync_pol             : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 o_reg_hdcp22_sync_pol_sel          : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 i_reg_hdcp22_inhsync_pol           : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 i_reg_hdcp22_invsync_pol           : 1 ;  //    20     
} H15B0_HDCP22_PRT0_AD_HDCP22_SYNC_CTRL_T;

/*-----------------------------------------------------------------------------
		0x1038 ad_hdcp22_out_data_align ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_hdcp22_out_data_align_0     : 1 ;  //     0     
	UINT32 o_reg_hdcp22_out_data_align_1     : 1 ;  //     1     
} H15B0_HDCP22_PRT0_AD_HDCP22_OUT_DATA_ALIGN_T;

/*-----------------------------------------------------------------------------
		0x1040 ad_hdcp22_clk_enable ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_cfgen                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 o_reg_hdmien                       : 1 ;  //     4     
} H15B0_HDCP22_PRT0_AD_HDCP22_CLK_ENABLE_T;

/*-----------------------------------------------------------------------------
		0x1044 ad_hdcp22_enable ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_hdcp_sw_enable               : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 o_reg_hdcp_endislock               : 1 ;  //    16     
} H15B0_HDCP22_PRT0_AD_HDCP22_ENABLE_T;

/*-----------------------------------------------------------------------------
		0x1048 ad_hdcp22_control ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_hdcp22_switch_lck            : 1 ;  //     0     
	UINT32 o_reg_hdcp22_ovr_en                : 1 ;  //     1     
	UINT32 o_reg_hdcp22_ovr_val               : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 o_reg_hdcp22_pkt_err_ovr_en        : 1 ;  //     4     
	UINT32 o_reg_hdcp22_pkt_err_ovr_val       : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 o_reg_hdcp22_pkt_err_ovr_en_1        : 1 ;  //     8     
	UINT32 o_reg_hdcp22_pkt_err_ovr_val_1       : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 o_reg_hdcp22_cd_ovr_en             : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 o_reg_hdcp22_cd_ovr_val            : 4 ;  // 23:20     
} H15B0_HDCP22_PRT0_AD_HDCP22_CONTROL_T;

/*-----------------------------------------------------------------------------
		0x104c ad_hdcp22_reg_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32	hdcp22_reg_status                  : 32;  // 31: 0     reserved
} H15B0_HDCP22_PRT0_AD_HDCP22_REG_STATUS_T;

/*-----------------------------------------------------------------------------
		0x1050 ad_hdcp22_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hdcp22_intr_status                                    : 32;  // 31: 0     reserved
} H15B0_HDCP22_PRT0_AD_HDCP22_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x105c ad_main_axi_user ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_axi_awuser                   : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 o_reg_axi_aruser                   : 4 ;  // 19:16     
} H15B0_HDCP22_PRT0_AD_MAIN_AXI_USER_T;

/*-----------------------------------------------------------------------------
		0x1060 ad_reg_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ref_tim_clr                    : 1 ;  //     0     
} H15B0_HDCP22_PRT0_AD_REG_TIME_T;

/*-----------------------------------------------------------------------------
		0x1064 ad_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 timer_cnt_en                       : 1 ;  //     0     
	UINT32 timer_cnt_clr                      : 1 ;  //     1     
} H15B0_HDCP22_PRT0_AD_TIMER_T;

/*-----------------------------------------------------------------------------
		0x1068 ad_timer_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 timer_cnt                          : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_TIMER_CNT_T;

/*-----------------------------------------------------------------------------
		0x107c ad_timer_set ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 timer_set                          : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_TIMER_SET_T;

/*-----------------------------------------------------------------------------
		0x1080 ad_intr_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_de_intr_en               : 1 ;  //     0     
	UINT32 o_reg_out_hs_intr_en               : 1 ;  //     1     
	UINT32 o_reg_out_vs_intr_en               : 1 ;  //     2     
	UINT32 o_reg_in_de_intr_en                : 1 ;  //     3     
	UINT32 o_reg_in_hs_intr_en                : 1 ;  //     4     
	UINT32 o_reg_in_vs_intr_en                : 1 ;  //     5     
	UINT32                                    : 8 ;  // 13: 6     reserved
	UINT32 timer_intr_en                      : 1 ;  //    14     
} H15B0_HDCP22_PRT0_AD_INTR_EN_T;

/*-----------------------------------------------------------------------------
		0x1084 ad_intr_mask ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_mask                          : 15;  // 14: 0     
} H15B0_HDCP22_PRT0_AD_INTR_MASK_T;

/*-----------------------------------------------------------------------------
		0x1088 ad_intr_clr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_de_intr_clr              : 1 ;  //     0     
	UINT32 o_reg_out_hs_intr_clr              : 1 ;  //     1     
	UINT32 o_reg_out_vs_intr_clr              : 1 ;  //     2     
	UINT32 o_reg_in_de_intr_clr               : 1 ;  //     3     
	UINT32 o_reg_in_hs_intr_clr               : 1 ;  //     4     
	UINT32 o_reg_in_vs_intr_clr               : 1 ;  //     5     
	UINT32                                    : 8 ;  // 13: 6     reserved
	UINT32 timer_intr_clr                     : 1 ;  //    14     
} H15B0_HDCP22_PRT0_AD_INTR_CLR_T;

/*-----------------------------------------------------------------------------
		0x108c ad_info_clr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_info_clr                 : 1 ;  //     0     
	UINT32 o_reg_in_info_clr                  : 1 ;  //     1     
} H15B0_HDCP22_PRT0_AD_INFO_CLR_T;

/*-----------------------------------------------------------------------------
		0x1090 ad_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_de_intr                  : 1 ;  //     0     
	UINT32 o_reg_out_hs_intr                  : 1 ;  //     1     
	UINT32 o_reg_out_vs_intr                  : 1 ;  //     2     
	UINT32 o_reg_in_de_intr                   : 1 ;  //     3     
	UINT32 o_reg_in_hs_intr                   : 1 ;  //     4     
	UINT32 o_reg_in_vs_intr                   : 1 ;  //     5     
	UINT32                                    : 8 ;  // 13: 6     reserved
	UINT32 timer_intr                         : 1 ;  //    14     
} H15B0_HDCP22_PRT0_AD_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x10a0 ad_in_hs_intr_line_num_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_in_hs_intr_line_num          : 16;  // 15: 0     
	UINT32 o_reg_in_hs_intr_line_num_en       : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_HS_INTR_LINE_NUM_EN_T;

/*-----------------------------------------------------------------------------
		0x10a4 ad_in_de_intr_line_num_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_in_de_intr_line_num          : 16;  // 15: 0     
	UINT32 o_reg_in_de_intr_line_num_en       : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_DE_INTR_LINE_NUM_EN_T;

/*-----------------------------------------------------------------------------
		0x10a8 ad_in_va_srt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_va_srt                    : 16;  // 15: 0     
} H15B0_HDCP22_PRT0_AD_IN_VA_SRT_T;

/*-----------------------------------------------------------------------------
		0x10ac ad_in_hs_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_de_total                  : 16;  // 15: 0     
	UINT32 i_reg_in_hs_total                  : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_HS_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10b0 ad_in_h_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_h_active                  : 16;  // 15: 0     
	UINT32 i_reg_in_h_total                   : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_H_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10b4 ad_in_err_h_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_err_h_active              : 16;  // 15: 0     
	UINT32 i_reg_in_err_h_total               : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_ERR_H_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10b8 ad_in_vs_iif_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_vs_iif_cnt                : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_IN_VS_IIF_CNT_T;

/*-----------------------------------------------------------------------------
		0x10bc ad_in_vs_pix_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_vs_pix_cnt                : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_IN_VS_PIX_CNT_T;

/*-----------------------------------------------------------------------------
		0x10c0 ad_in_htotal_iif ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_h_total_1_iif             : 16;  // 15: 0     
	UINT32 i_reg_in_h_total_0_iif             : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_IN_HTOTAL_IIF_T;

/*-----------------------------------------------------------------------------
		0x10c4 ad_in_vs_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_vs_intr_time              : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_IN_VS_INTR_TIME_T;

/*-----------------------------------------------------------------------------
		0x10c8 ad_in_hs_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_hs_intr_time              : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_IN_HS_INTR_TIME_T;

/*-----------------------------------------------------------------------------
		0x10cc ad_in_de_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_in_de_intr_time              : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_IN_DE_INTR_TIME_T;

/*-----------------------------------------------------------------------------
		0x10d0 ad_out_hs_intr_line_num_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_hs_intr_line_num         : 16;  // 15: 0     
	UINT32 o_reg_out_hs_intr_line_num_en      : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_HS_INTR_LINE_NUM_EN_T;

/*-----------------------------------------------------------------------------
		0x10d4 ad_out_de_intr_line_num_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_reg_out_de_intr_line_num         : 16;  // 15: 0     
	UINT32 o_reg_out_de_intr_line_num_en      : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_DE_INTR_LINE_NUM_EN_T;

/*-----------------------------------------------------------------------------
		0x10d8 ad_out_va_srt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_va_srt                   : 16;  // 15: 0     
} H15B0_HDCP22_PRT0_AD_OUT_VA_SRT_T;

/*-----------------------------------------------------------------------------
		0x10dc ad_out_hs_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_de_total                 : 16;  // 15: 0     
	UINT32 i_reg_out_hs_total                 : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_HS_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10e0 ad_out_h_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_h_active                 : 16;  // 15: 0     
	UINT32 i_reg_out_h_total                  : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_H_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10e4 ad_out_err_h_total ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_err_h_active             : 16;  // 15: 0     
	UINT32 i_reg_out_err_h_total              : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_ERR_H_TOTAL_T;

/*-----------------------------------------------------------------------------
		0x10e8 ad_out_vs_iif_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_vs_iif_cnt               : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_OUT_VS_IIF_CNT_T;

/*-----------------------------------------------------------------------------
		0x10ec ad_out_vs_pix_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_vs_pix_cnt               : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_OUT_VS_PIX_CNT_T;

/*-----------------------------------------------------------------------------
		0x10f0 ad_out_htotal_iif ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_h_total_1_iif            : 16;  // 15: 0     
	UINT32 i_reg_out_h_total_0_iif            : 16;  // 31:16     
} H15B0_HDCP22_PRT0_AD_OUT_HTOTAL_IIF_T;

/*-----------------------------------------------------------------------------
		0x10f4 ad_out_vs_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_vs_intr_time             : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_OUT_VS_INTR_TIME_T;

/*-----------------------------------------------------------------------------
		0x10f8 ad_out_hs_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_hs_intr_time             : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_OUT_HS_INTR_TIME_T;

/*-----------------------------------------------------------------------------
		0x10fc ad_out_de_intr_time ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 i_reg_out_de_intr_time             : 32;  // 31: 0     
} H15B0_HDCP22_PRT0_AD_OUT_DE_INTR_TIME_T;
typedef struct {
	H15B0_HDCP22_PRT0_AD_VERSION_T                       	ad_version;                   //0x1000
	UINT32                             	reserved00[3];                //0x1004~0x100c
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_1_T              	ad_hdcp22_skp_pkf_1;          //0x1010
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_2_T              	ad_hdcp22_skp_pkf_2;          //0x1014
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_3_T              	ad_hdcp22_skp_pkf_3;          //0x1018
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_PKF_4_T              	ad_hdcp22_skp_pkf_4;          //0x101c
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_1_T              	ad_hdcp22_skp_duk_1;          //0x1020
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_2_T              	ad_hdcp22_skp_duk_2;          //0x1024
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_3_T              	ad_hdcp22_skp_duk_3;          //0x1028
	H15B0_HDCP22_PRT0_AD_HDCP22_SKP_DUK_4_T              	ad_hdcp22_skp_duk_4;          //0x102c
	H15B0_HDCP22_PRT0_AD_HDCP22_HPD_CTRL_T               	ad_hdcp22_hpd_ctrl;           //0x1030
	H15B0_HDCP22_PRT0_AD_HDCP22_SYNC_CTRL_T              	ad_hdcp22_sync_ctrl;          //0x1034
	H15B0_HDCP22_PRT0_AD_HDCP22_OUT_DATA_ALIGN_T         	ad_hdcp22_out_data_align;     //0x1038
	UINT32                             	reserved02;                   //0x103c
	H15B0_HDCP22_PRT0_AD_HDCP22_CLK_ENABLE_T             	ad_hdcp22_clk_enable;         //0x1040
	H15B0_HDCP22_PRT0_AD_HDCP22_ENABLE_T                 	ad_hdcp22_enable;             //0x1044
	H15B0_HDCP22_PRT0_AD_HDCP22_CONTROL_T                	ad_hdcp22_control;            //0x1048
	H15B0_HDCP22_PRT0_AD_HDCP22_REG_STATUS_T             	ad_hdcp22_reg_status;         //0x104c
	H15B0_HDCP22_PRT0_AD_HDCP22_INTR_STATUS_T            	ad_hdcp22_intr_status;        //0x1050
	UINT32                             	reserved03[2];                //0x1054~0x1058
	H15B0_HDCP22_PRT0_AD_MAIN_AXI_USER_T                 	ad_main_axi_user;             //0x105c
	H15B0_HDCP22_PRT0_AD_REG_TIME_T                      	ad_reg_time;                  //0x1060
	H15B0_HDCP22_PRT0_AD_TIMER_T                         	ad_timer;                     //0x1064
	H15B0_HDCP22_PRT0_AD_TIMER_CNT_T                     	ad_timer_cnt;                 //0x1068
	UINT32                             	reserved04[4];                //0x106c~0x1078
	H15B0_HDCP22_PRT0_AD_TIMER_SET_T                     	ad_timer_set;                 //0x107c
	H15B0_HDCP22_PRT0_AD_INTR_EN_T                       	ad_intr_en;                   //0x1080
	H15B0_HDCP22_PRT0_AD_INTR_MASK_T                     	ad_intr_mask;                 //0x1084
	H15B0_HDCP22_PRT0_AD_INTR_CLR_T                      	ad_intr_clr;                  //0x1088
	H15B0_HDCP22_PRT0_AD_INFO_CLR_T                      	ad_info_clr;                  //0x108c
	H15B0_HDCP22_PRT0_AD_INTR_STATUS_T                   	ad_intr_status;               //0x1090
	UINT32                             	reserved05[3];                //0x1094~0x109c
	H15B0_HDCP22_PRT0_AD_IN_HS_INTR_LINE_NUM_EN_T        	ad_in_hs_intr_line_num_en;    //0x10a0
	H15B0_HDCP22_PRT0_AD_IN_DE_INTR_LINE_NUM_EN_T        	ad_in_de_intr_line_num_en;    //0x10a4
	H15B0_HDCP22_PRT0_AD_IN_VA_SRT_T                     	ad_in_va_srt;                 //0x10a8
	H15B0_HDCP22_PRT0_AD_IN_HS_TOTAL_T                   	ad_in_hs_total;               //0x10ac
	H15B0_HDCP22_PRT0_AD_IN_H_TOTAL_T                    	ad_in_h_total;                //0x10b0
	H15B0_HDCP22_PRT0_AD_IN_ERR_H_TOTAL_T                	ad_in_err_h_total;            //0x10b4
	H15B0_HDCP22_PRT0_AD_IN_VS_IIF_CNT_T                 	ad_in_vs_iif_cnt;             //0x10b8
	H15B0_HDCP22_PRT0_AD_IN_VS_PIX_CNT_T                 	ad_in_vs_pix_cnt;             //0x10bc
	H15B0_HDCP22_PRT0_AD_IN_HTOTAL_IIF_T                 	ad_in_htotal_iif;             //0x10c0
	H15B0_HDCP22_PRT0_AD_IN_VS_INTR_TIME_T               	ad_in_vs_intr_time;           //0x10c4
	H15B0_HDCP22_PRT0_AD_IN_HS_INTR_TIME_T               	ad_in_hs_intr_time;           //0x10c8
	H15B0_HDCP22_PRT0_AD_IN_DE_INTR_TIME_T               	ad_in_de_intr_time;           //0x10cc
	H15B0_HDCP22_PRT0_AD_OUT_HS_INTR_LINE_NUM_EN_T       	ad_out_hs_intr_line_num_en;   //0x10d0
	H15B0_HDCP22_PRT0_AD_OUT_DE_INTR_LINE_NUM_EN_T       	ad_out_de_intr_line_num_en;   //0x10d4
	H15B0_HDCP22_PRT0_AD_OUT_VA_SRT_T                    	ad_out_va_srt;                //0x10d8
	H15B0_HDCP22_PRT0_AD_OUT_HS_TOTAL_T                  	ad_out_hs_total;              //0x10dc
	H15B0_HDCP22_PRT0_AD_OUT_H_TOTAL_T                   	ad_out_h_total;               //0x10e0
	H15B0_HDCP22_PRT0_AD_OUT_ERR_H_TOTAL_T               	ad_out_err_h_total;           //0x10e4
	H15B0_HDCP22_PRT0_AD_OUT_VS_IIF_CNT_T                	ad_out_vs_iif_cnt;            //0x10e8
	H15B0_HDCP22_PRT0_AD_OUT_VS_PIX_CNT_T                	ad_out_vs_pix_cnt;            //0x10ec
	H15B0_HDCP22_PRT0_AD_OUT_HTOTAL_IIF_T                	ad_out_htotal_iif;            //0x10f0
	H15B0_HDCP22_PRT0_AD_OUT_VS_INTR_TIME_T              	ad_out_vs_intr_time;          //0x10f4
	H15B0_HDCP22_PRT0_AD_OUT_HS_INTR_TIME_T              	ad_out_hs_intr_time;          //0x10f8
	H15B0_HDCP22_PRT0_AD_OUT_DE_INTR_TIME_T              	ad_out_de_intr_time;          //0x10fc
}HDCP22_PRT0_REG_H15B0_T;
#endif
