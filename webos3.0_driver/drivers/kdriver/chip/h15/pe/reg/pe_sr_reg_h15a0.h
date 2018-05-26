#ifndef _PE_SR_REG_H15A0_H_
#define _PE_SR_REG_H15A0_H_

#undef PE_SR_REG_H15A0_NEED_TO_REMAP

/*-----------------------------------------------------------------------------
		0xc8008000L ft_grad_sm ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_grad_sm                         : 2 ;  //  1: 0     
} PE_H15A0_FT_GRAD_SM_T;

/*-----------------------------------------------------------------------------
		0xc8008004L ft_localmm_value_scale ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_localmm_scale                   : 10;  //  9: 0     
} PE_H15A0_FT_LOCALMM_VALUE_SCALE_T;

/*-----------------------------------------------------------------------------
		0xc8008008L ft_localmm_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_localmin_th                     : 10;  //  9: 0     
} PE_H15A0_FT_LOCALMM_TH_T;

/*-----------------------------------------------------------------------------
		0xc800800cL ft_localmm_sh ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_localmm_sh33                    : 4 ;  //  3: 0     
} PE_H15A0_FT_LOCALMM_SH_T;

/*-----------------------------------------------------------------------------
		0xc8008010L ft_ltv_sh ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_ltv_sh                          : 4 ;  //  3: 0     
} PE_H15A0_FT_LTV_SH_T;

/*-----------------------------------------------------------------------------
		0xc8008014L ft_ltv_th_diff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_ltv_th_low_diff                 : 10;  //  9: 0     
} PE_H15A0_FT_LTV_TH_DIFF_T;

/*-----------------------------------------------------------------------------
		0xc8008018L ft_sharp_sm ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_sharp_sm                        : 1 ;  //     0     
} PE_H15A0_FT_SHARP_SM_T;

/*-----------------------------------------------------------------------------
		0xc800801cL ft_sharp_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_sharp_th                        : 10;  //  9: 0     
} PE_H15A0_FT_SHARP_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008020L ft_sharp_scale_sh ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_sharp_scale_sh                  : 4 ;  //  3: 0     
} PE_H15A0_FT_SHARP_SCALE_SH_T;

/*-----------------------------------------------------------------------------
		0xc8008024L ft_sharp_ctrl_we ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_sharp_ctrl_we                   : 3 ;  //  2: 0     
} PE_H15A0_FT_SHARP_CTRL_WE_T;

/*-----------------------------------------------------------------------------
		0xc8008080L core1_iu_en_ltv_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_ltv_th1                  : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_lue_ltv_th0                  : 10;  // 25:16     
} PE_H15A0_CORE1_IU_EN_LTV_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008084L core1_iu_en_ltv_grdavg_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_ltv_grdavg_th            : 10;  //  9: 0     
} PE_H15A0_CORE1_IU_EN_LTV_GRDAVG_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008088L core1_iu_en_gni_th0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_en_gni_th0                : 10;  //  9: 0     
} PE_H15A0_CORE1_IU_EN_GNI_TH0_T;

/*-----------------------------------------------------------------------------
		0xc800808cL core1_iu_en_gni_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_en_gni_th2                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_en_gni_th1                : 10;  // 25:16     
} PE_H15A0_CORE1_IU_EN_GNI_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008090L core1_iu_en_gni_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_en_gni_th4                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_en_gni_th3                : 10;  // 25:16     
} PE_H15A0_CORE1_IU_EN_GNI_TH2_T;

/*-----------------------------------------------------------------------------
		0xc8008094L core1_iu_en_gni_th3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_en_gni_th6                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_en_gni_th5                : 10;  // 25:16     
} PE_H15A0_CORE1_IU_EN_GNI_TH3_T;

/*-----------------------------------------------------------------------------
		0xc8008098L core1_iu_en_grd_mul ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_grd_mul                  : 3 ;  //  2: 0     
} PE_H15A0_CORE1_IU_EN_GRD_MUL_T;

/*-----------------------------------------------------------------------------
		0xc8008100L core1_lut_id_um_grdavg0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_id_um_grdavg3            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_id_um_grdavg2            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_id_um_grdavg1            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_id_um_grdavg0            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_ID_UM_GRDAVG0_T;

/*-----------------------------------------------------------------------------
		0xc8008104L core1_lut_id_um_grdavg1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_id_um_grdavg7            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_id_um_grdavg6            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_id_um_grdavg5            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_id_um_grdavg4            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_ID_UM_GRDAVG1_T;

/*-----------------------------------------------------------------------------
		0xc8008108L core1_lut_id_um_grdavg2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_id_um_grdavg11           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_id_um_grdavg10           : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_id_um_grdavg9            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_id_um_grdavg8            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_ID_UM_GRDAVG2_T;

/*-----------------------------------------------------------------------------
		0xc800810cL core1_lut_id_um_grdavg3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_id_um_grdavg15           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_id_um_grdavg14           : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_id_um_grdavg13           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_id_um_grdavg12           : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_ID_UM_GRDAVG3_T;

/*-----------------------------------------------------------------------------
		0xc8008110L core1_we_dh_minmax ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_range1                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 core1_we_dh_range0                 : 9 ;  // 24:16     
} PE_H15A0_CORE1_WE_DH_MINMAX_T;

/*-----------------------------------------------------------------------------
		0xc8008114L core1_we_dh_grd_cost_wei ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_grd_cost_wei           : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_DH_GRD_COST_WEI_T;

/*-----------------------------------------------------------------------------
		0xc8008118L core1_we_dh_noise_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_noise_th_1             : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_DH_NOISE_TH1_T;

/*-----------------------------------------------------------------------------
		0xc800811cL core1_we_dh_noise_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_noise_th_3             : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_dh_noise_th_2             : 10;  // 25:16     
} PE_H15A0_CORE1_WE_DH_NOISE_TH2_T;

/*-----------------------------------------------------------------------------
		0xc8008120L core1_we_dh_noise_str ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_noise_str_3            : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 core1_we_dh_noise_str_2            : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 core1_we_dh_noise_str_1            : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_DH_NOISE_STR_T;

/*-----------------------------------------------------------------------------
		0xc8008124L core1_we_dh_ltv_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_ltv_th2                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_dh_ltv_th1                : 10;  // 25:16     
} PE_H15A0_CORE1_WE_DH_LTV_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008128L core1_we_dh_ltv_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_ltv_hgrd_th            : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_dh_ltv_lgrd_th            : 10;  // 25:16     
} PE_H15A0_CORE1_WE_DH_LTV_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc800812cL core1_we_dh_ltv_grd_str ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_dh_ltv_hgrd_str           : 5 ;  //  4: 0     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 core1_we_dh_ltv_lgrd_str           : 5 ;  // 20:16     
} PE_H15A0_CORE1_WE_DH_LTV_GRD_STR_T;

/*-----------------------------------------------------------------------------
		0xc8008140L core1_iu_um_clip_ltv_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_um_clip_ltv_th2           : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_um_clip_ltv_th1           : 10;  // 25:16     
} PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008144L core1_iu_um_clip_ltv_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_um_clip_ltv_th4           : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_um_clip_ltv_th3           : 10;  // 25:16     
} PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH2_T;

/*-----------------------------------------------------------------------------
		0xc8008148L core1_lut_iu_um_grdavg0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_iu_um_grdavg3            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_iu_um_grdavg2            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_iu_um_grdavg1            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_iu_um_grdavg0            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_IU_UM_GRDAVG0_T;

/*-----------------------------------------------------------------------------
		0xc800814cL core1_lut_iu_um_grdavg1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_iu_um_grdavg7            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_iu_um_grdavg6            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_iu_um_grdavg5            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_iu_um_grdavg4            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_IU_UM_GRDAVG1_T;

/*-----------------------------------------------------------------------------
		0xc8008150L core1_lut_iu_um_grdavg2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_iu_um_grdavg11           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_iu_um_grdavg10           : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_iu_um_grdavg9            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_iu_um_grdavg8            : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_IU_UM_GRDAVG2_T;

/*-----------------------------------------------------------------------------
		0xc8008154L core1_lut_iu_um_grdavg3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lut_iu_um_grdavg15           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lut_iu_um_grdavg14           : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lut_iu_um_grdavg13           : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lut_iu_um_grdavg12           : 5 ;  // 28:24     
} PE_H15A0_CORE1_LUT_IU_UM_GRDAVG3_T;

/*-----------------------------------------------------------------------------
		0xc8008158L core1_we_uh_minmax ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_range1                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 core1_we_uh_range0                 : 9 ;  // 24:16     
} PE_H15A0_CORE1_WE_UH_MINMAX_T;

/*-----------------------------------------------------------------------------
		0xc800815cL core1_we_uh_grd_cost_wei ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_grd_cost_wei           : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_UH_GRD_COST_WEI_T;

/*-----------------------------------------------------------------------------
		0xc8008160L core1_we_uh_noise_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_noise_th_1             : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_UH_NOISE_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008164L core1_we_uh_noise_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_noise_th_3             : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_uh_noise_th_2             : 10;  // 25:16     
} PE_H15A0_CORE1_WE_UH_NOISE_TH2_T;

/*-----------------------------------------------------------------------------
		0xc8008168L core1_we_uh_noise_str ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_noise_str_3            : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 core1_we_uh_noise_str_2            : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 core1_we_uh_noise_str_1            : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_UH_NOISE_STR_T;

/*-----------------------------------------------------------------------------
		0xc800816cL core1_we_uh_ltv_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_ltv_th2                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_uh_ltv_th1                : 10;  // 25:16     
} PE_H15A0_CORE1_WE_UH_LTV_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008170L core1_we_uh_ltv_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_ltv_hgrd_th            : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_uh_ltv_lgrd_th            : 10;  // 25:16     
} PE_H15A0_CORE1_WE_UH_LTV_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008174L core1_we_uh_ltv_grd_str ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_uh_ltv_hgrd_str           : 5 ;  //  4: 0     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 core1_we_uh_ltv_lgrd_str           : 5 ;  // 20:16     
} PE_H15A0_CORE1_WE_UH_LTV_GRD_STR_T;

/*-----------------------------------------------------------------------------
		0xc80081a0L core1_we_jone_base_mul ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_base_mul             : 4 ;  //  3: 0     
} PE_H15A0_CORE1_WE_JONE_BASE_MUL_T;

/*-----------------------------------------------------------------------------
		0xc80081a4L core1_we_jone_ss_shift ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_ss_sh                : 3 ;  //  2: 0     
} PE_H15A0_CORE1_WE_JONE_SS_SHIFT_T;

/*-----------------------------------------------------------------------------
		0xc80081a8L core1_we_jone_ss_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_ss_r                 : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_JONE_SS_R_T;

/*-----------------------------------------------------------------------------
		0xc80081acL core1_we_jone_mm_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_mm3_th1              : 5 ;  //  4: 0     
} PE_H15A0_CORE1_WE_JONE_MM_TH_T;

/*-----------------------------------------------------------------------------
		0xc80081b0L core1_we_jone_minmax ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_max                  : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_we_jone_min                  : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_JONE_MINMAX_T;

/*-----------------------------------------------------------------------------
		0xc80081b4L core1_we_jone_base ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_base                 : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_JONE_BASE_T;

/*-----------------------------------------------------------------------------
		0xc80081b8L core1_we_jone_grdavg_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_grdavg_th2           : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_jone_grdavg_th1           : 10;  // 25:16     
} PE_H15A0_CORE1_WE_JONE_GRDAVG_TH1_T;

/*-----------------------------------------------------------------------------
		0xc80081bcL core1_we_jone_grdavg_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_grdavg_th4           : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_jone_grdavg_th3           : 10;  // 25:16     
} PE_H15A0_CORE1_WE_JONE_GRDAVG_TH2_T;

/*-----------------------------------------------------------------------------
		0xc80081c0L core1_we_jone_d_jo_grdavg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_d_jo_grdavg4         : 8 ;  //  7: 0     
	UINT32 core1_we_jone_d_jo_grdavg3         : 8 ;  // 15: 8     
	UINT32 core1_we_jone_d_jo_grdavg2         : 8 ;  // 23:16     
	UINT32 core1_we_jone_d_jo_grdavg1         : 8 ;  // 31:24     
} PE_H15A0_CORE1_WE_JONE_D_JO_GRDAVG_T;

/*-----------------------------------------------------------------------------
		0xc80081c4L core1_we_jone_r_grd_mul1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_r_grd_mul2           : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 core1_we_jone_r_grd_mul1           : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_JONE_R_GRD_MUL1_T;

/*-----------------------------------------------------------------------------
		0xc80081c8L core1_we_jone_r_grd_mul2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_r_grd_mul5           : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 core1_we_jone_r_grd_mul4           : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 core1_we_jone_r_grd_mul3           : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_JONE_R_GRD_MUL2_T;

/*-----------------------------------------------------------------------------
		0xc80081ccL core1_we_jone_r_grd_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_r_grd_offset         : 4 ;  //  3: 0     
} PE_H15A0_CORE1_WE_JONE_R_GRD_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc80081d0L core1_we_jone_d_rgr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_d_rgr2               : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_we_jone_d_rgr1               : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_JONE_D_RGR1_T;

/*-----------------------------------------------------------------------------
		0xc80081d4L core1_we_jone_d_rgr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_jone_d_rgr5               : 8 ;  //  7: 0     
	UINT32 core1_we_jone_d_rgr4               : 8 ;  // 15: 8     
	UINT32 core1_we_jone_d_rgr3               : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_JONE_D_RGR2_T;

/*-----------------------------------------------------------------------------
		0xc8008200L core1_we_id_lq_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_lq_grd_th              : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_LQ_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008204L core1_we_id_lq_grd_ratio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_lq_grd_r5              : 4 ;  //  3: 0     
	UINT32 core1_we_id_lq_grd_r4              : 4 ;  //  7: 4     
	UINT32 core1_we_id_lq_grd_r3              : 4 ;  // 11: 8     
	UINT32 core1_we_id_lq_grd_r2              : 4 ;  // 15:12     
	UINT32 core1_we_id_lq_grd_r1              : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_ID_LQ_GRD_RATIO_T;

/*-----------------------------------------------------------------------------
		0xc8008208L core1_we_id_lq_ltv_min_max ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_lq_ltv_max             : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_lq_ltv_min             : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_LQ_LTV_MIN_MAX_T;

/*-----------------------------------------------------------------------------
		0xc800820cL core1_we_id_lq_ltv_mul ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_lq_ltv_mul             : 6 ;  //  5: 0     
} PE_H15A0_CORE1_WE_ID_LQ_LTV_MUL_T;

/*-----------------------------------------------------------------------------
		0xc8008210L core1_we_id_lq_grdavg_clip ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_lq_grdavg_clip         : 3 ;  //  2: 0     
} PE_H15A0_CORE1_WE_ID_LQ_GRDAVG_CLIP_T;

/*-----------------------------------------------------------------------------
		0xc8008214L core1_we_id_whq_base ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_whq_base               : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_WHQ_BASE_T;

/*-----------------------------------------------------------------------------
		0xc8008218L core1_we_id_hq_grdavg_mul0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_grdavg_mul0         : 4 ;  //  3: 0     
} PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL0_T;

/*-----------------------------------------------------------------------------
		0xc800821cL core1_we_id_hq_ltv_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_th1             : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_ltv_th0             : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008220L core1_we_id_hq_ltv_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_th3             : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_ltv_th2             : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_TH2_T;

/*-----------------------------------------------------------------------------
		0xc8008224L core1_we_id_hq_ltv_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_grd_th2         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_ltv_grd_th1         : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008228L core1_we_id_m_whq_ltv1_grd1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ltv1_grd1        : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD1_T;

/*-----------------------------------------------------------------------------
		0xc800822cL core1_we_id_m_whq_ltv1_grd2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ltv1_grd3        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_we_id_m_whq_ltv1_grd2        : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD2_T;

/*-----------------------------------------------------------------------------
		0xc8008230L core1_we_id_m_whq_ltv2_grd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ltv2_grd3        : 8 ;  //  7: 0     
	UINT32 core1_we_id_m_whq_ltv2_grd2        : 8 ;  // 15: 8     
	UINT32 core1_we_id_m_whq_ltv2_grd1        : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LTV2_GRD_T;

/*-----------------------------------------------------------------------------
		0xc8008234L core1_we_id_m_whq_ltv3_grd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ltv3_grd3        : 8 ;  //  7: 0     
	UINT32 core1_we_id_m_whq_ltv3_grd2        : 8 ;  // 15: 8     
	UINT32 core1_we_id_m_whq_ltv3_grd1        : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LTV3_GRD_T;

/*-----------------------------------------------------------------------------
		0xc8008238L core1_we_id_hq_ltv_th_high1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_th_high1        : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH1_T;

/*-----------------------------------------------------------------------------
		0xc800823cL core1_we_id_hq_ltv_th_high2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_th_high3        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_ltv_th_high2        : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH2_T;

/*-----------------------------------------------------------------------------
		0xc8008240L core1_we_id_m_whq_ltv_high ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ltv_high3        : 8 ;  //  7: 0     
	UINT32 core1_we_id_m_whq_ltv_high2        : 8 ;  // 15: 8     
	UINT32 core1_we_id_m_whq_ltv_high1        : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LTV_HIGH_T;

/*-----------------------------------------------------------------------------
		0xc8008244L core1_we_id_hq_grdavg_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_grdavg_th2          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_grdavg_th1          : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008248L core1_we_id_hq_grdavg_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_grdavg_th4          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_grdavg_th3          : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH2_T;

/*-----------------------------------------------------------------------------
		0xc800824cL core1_we_id_m_whq_grdavg1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_grdavg2          : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_we_id_m_whq_grdavg1          : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG1_T;

/*-----------------------------------------------------------------------------
		0xc8008250L core1_we_id_m_whq_grdavg2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_grdavg4          : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_we_id_m_whq_grdavg3          : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG2_T;

/*-----------------------------------------------------------------------------
		0xc8008254L core1_we_id_hq_ss_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ss_th1              : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_SS_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008258L core1_we_id_hq_ss_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ss_th3              : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_ss_th2              : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_SS_TH2_T;

/*-----------------------------------------------------------------------------
		0xc800825cL core1_we_id_m_whq_ss1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_ss3              : 8 ;  //  7: 0     
	UINT32 core1_we_id_m_whq_ss2              : 8 ;  // 15: 8     
	UINT32 core1_we_id_m_whq_ss1              : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_SS1_T;

/*-----------------------------------------------------------------------------
		0xc8008260L core1_we_id_hq_grdavg_mul1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_grdavg_mul1         : 4 ;  //  3: 0     
} PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL1_T;

/*-----------------------------------------------------------------------------
		0xc8008264L core1_we_id_ratio_hq ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_ratio_hq               : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_RATIO_HQ_T;

/*-----------------------------------------------------------------------------
		0xc8008268L core1_we_id_ltv_1b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_ltv_1b                 : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_LTV_1B_T;

/*-----------------------------------------------------------------------------
		0xc800826cL core1_we_id_hq_ltv_lth ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_lth             : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_LTH_T;

/*-----------------------------------------------------------------------------
		0xc8008270L core1_we_id_hq_ltv_hth ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_ltv_hth             : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LTV_HTH_T;

/*-----------------------------------------------------------------------------
		0xc8008274L core1_we_id_hq_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_grd_th              : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008278L core1_we_id_hq_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_r5                  : 4 ;  //  3: 0     
	UINT32 core1_we_id_hq_r4                  : 4 ;  //  7: 4     
	UINT32 core1_we_id_hq_r3                  : 4 ;  // 11: 8     
	UINT32 core1_we_id_hq_r2                  : 4 ;  // 15:12     
	UINT32 core1_we_id_hq_r1                  : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_ID_HQ_R_T;

/*-----------------------------------------------------------------------------
		0xc800827cL core1_we_id_hq_lmm_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_lmm_th              : 5 ;  //  4: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LMM_TH_T;

/*-----------------------------------------------------------------------------
		0xc8008280L core1_we_id_m_whq_lmm ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_lmm              : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LMM_T;

/*-----------------------------------------------------------------------------
		0xc8008284L core1_we_id_hq_low_grdavg_th1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_low_grdavg_th1      : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH1_T;

/*-----------------------------------------------------------------------------
		0xc8008288L core1_we_id_hq_low_grdavg_th2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_low_grdavg_th3      : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_id_hq_low_grdavg_th2      : 10;  // 25:16     
} PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH2_T;

/*-----------------------------------------------------------------------------
		0xc800828cL core1_we_id_hq_low_rgrd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_low_rgrd3           : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 core1_we_id_hq_low_rgrd2           : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 core1_we_id_hq_low_rgrd1           : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_T;

/*-----------------------------------------------------------------------------
		0xc8008290L core1_we_id_hq_low_rgrd_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_hq_low_rgrd_offset     : 4 ;  //  3: 0     
} PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8008294L core1_we_id_m_whq_low_rgrd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_m_whq_low_rgrd3        : 8 ;  //  7: 0     
	UINT32 core1_we_id_m_whq_low_rgrd2        : 8 ;  // 15: 8     
	UINT32 core1_we_id_m_whq_low_rgrd1        : 8 ;  // 23:16     
} PE_H15A0_CORE1_WE_ID_M_WHQ_LOW_RGRD_T;

/*-----------------------------------------------------------------------------
		0xc8008298L core1_we_mat_fs_ss1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat_th_ss1                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_mat_th_ss0                : 10;  // 25:16     
} PE_H15A0_CORE1_WE_MAT_FS_SS1_T;

/*-----------------------------------------------------------------------------
		0xc800829cL core1_we_mat_fs_ss2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat_th_ss2                : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_MAT_FS_SS2_T;

/*-----------------------------------------------------------------------------
		0xc80082a0L core1_we_mat_fs_ltv1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat_th_ltv1               : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_we_mat_th_ltv0               : 10;  // 25:16     
} PE_H15A0_CORE1_WE_MAT_FS_LTV1_T;

/*-----------------------------------------------------------------------------
		0xc80082a4L core1_we_mat_fs_ltv2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat_th_ltv2               : 10;  //  9: 0     
} PE_H15A0_CORE1_WE_MAT_FS_LTV2_T;

/*-----------------------------------------------------------------------------
		0xc80082a8L core1_we_mat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat                       : 5 ;  //  4: 0     
} PE_H15A0_CORE1_WE_MAT_T;

/*-----------------------------------------------------------------------------
		0xc80082acL core1_we_mat_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_mat_c4                    : 4 ;  //  3: 0     
	UINT32 core1_we_mat_c3                    : 4 ;  //  7: 4     
	UINT32 core1_we_mat_c2                    : 4 ;  // 11: 8     
	UINT32 core1_we_mat_c1                    : 4 ;  // 15:12     
	UINT32 core1_we_mat_c0                    : 4 ;  // 19:16     
} PE_H15A0_CORE1_WE_MAT_C_T;

/*-----------------------------------------------------------------------------
		0xc8008300L pq_class_thresh_minmax1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_minmax_1                  : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_minmax_0                  : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_MINMAX1_T;

/*-----------------------------------------------------------------------------
		0xc8008304L pq_class_thresh_minmax2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_minmax_3                  : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_minmax_2                  : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_MINMAX2_T;

/*-----------------------------------------------------------------------------
		0xc8008308L pq_class_thresh_blur1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_blur_1                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_blur_0                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_BLUR1_T;

/*-----------------------------------------------------------------------------
		0xc800830cL pq_class_thresh_blur2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_blur_3                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_blur_2                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_BLUR2_T;

/*-----------------------------------------------------------------------------
		0xc8008310L pq_class_thresh_blur3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_blur_5                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_blur_4                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_BLUR3_T;

/*-----------------------------------------------------------------------------
		0xc8008314L pq_class_thresh_blur4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_blur_7                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_blur_6                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_BLUR4_T;

/*-----------------------------------------------------------------------------
		0xc8008318L pq_class_thresh_grad1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_grad_1                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_grad_0                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_GRAD1_T;

/*-----------------------------------------------------------------------------
		0xc800831cL pq_class_thresh_grad2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_grad_3                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_grad_2                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_GRAD2_T;

/*-----------------------------------------------------------------------------
		0xc8008320L pq_class_thresh_grad3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_grad_5                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_grad_4                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_GRAD3_T;

/*-----------------------------------------------------------------------------
		0xc8008324L pq_class_thresh_grad4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_grad_7                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_grad_6                    : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_GRAD4_T;

/*-----------------------------------------------------------------------------
		0xc8008328L pq_class_thresh_diffgrad1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_diffgrad_1                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_diffgrad_0                : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD1_T;

/*-----------------------------------------------------------------------------
		0xc800832cL pq_class_thresh_diffgrad2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_diffgrad_3                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_diffgrad_2                : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD2_T;

/*-----------------------------------------------------------------------------
		0xc8008330L pq_class_thresh_diffgrad3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_diffgrad_5                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_diffgrad_4                : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD3_T;

/*-----------------------------------------------------------------------------
		0xc8008334L pq_class_thresh_diffgrad4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_diffgrad_7                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_diffgrad_6                : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD4_T;

/*-----------------------------------------------------------------------------
		0xc8008338L pq_class_thresh_ss0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_ss_1                      : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_ss_0                      : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_SS0_T;

/*-----------------------------------------------------------------------------
		0xc800833cL pq_class_thresh_ss1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_ss_3                      : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_ss_2                      : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_SS1_T;

/*-----------------------------------------------------------------------------
		0xc8008340L pq_class_thresh_ss2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_ss_5                      : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_ss_4                      : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_SS2_T;

/*-----------------------------------------------------------------------------
		0xc8008344L pq_class_thresh_ss3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 class_th_ss_7                      : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 class_th_ss_6                      : 10;  // 25:16     
} PE_H15A0_PQ_CLASS_THRESH_SS3_T;

/*-----------------------------------------------------------------------------
		0xc8008348L pq_minmax_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_minmax_cnt_0                    : 22;  // 21: 0     
} PE_H15A0_PQ_MINMAX_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc800834cL pq_minmax_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_minmax_cnt_1                    : 22;  // 21: 0     
} PE_H15A0_PQ_MINMAX_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc8008350L pq_minmax_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_minmax_cnt_2                    : 22;  // 21: 0     
} PE_H15A0_PQ_MINMAX_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc8008354L pq_minmax_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_minmax_cnt_3                    : 22;  // 21: 0     
} PE_H15A0_PQ_MINMAX_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc8008358L pq_sharp_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_0                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc800835cL pq_sharp_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_1                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc8008360L pq_sharp_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_2                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc8008364L pq_sharp_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_3                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc8008368L pq_sharp_cnt_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_4                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_4_T;

/*-----------------------------------------------------------------------------
		0xc800836cL pq_sharp_cnt_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_5                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_5_T;

/*-----------------------------------------------------------------------------
		0xc8008370L pq_sharp_cnt_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_6                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_6_T;

/*-----------------------------------------------------------------------------
		0xc8008374L pq_sharp_cnt_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_sharp_cnt_7                     : 22;  // 21: 0     
} PE_H15A0_PQ_SHARP_CNT_7_T;

/*-----------------------------------------------------------------------------
		0xc8008378L pq_grad_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_0                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc800837cL pq_grad_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_1                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc8008380L pq_grad_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_2                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc8008384L pq_grad_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_3                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc8008388L pq_grad_cnt_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_4                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_4_T;

/*-----------------------------------------------------------------------------
		0xc800838cL pq_grad_cnt_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_5                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_5_T;

/*-----------------------------------------------------------------------------
		0xc8008390L pq_grad_cnt_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_6                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_6_T;

/*-----------------------------------------------------------------------------
		0xc8008394L pq_grad_cnt_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_grad_cnt_7                      : 22;  // 21: 0     
} PE_H15A0_PQ_GRAD_CNT_7_T;

/*-----------------------------------------------------------------------------
		0xc8008398L pq_av_grad_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_0                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc800839cL pq_av_grad_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_1                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc80083a0L pq_av_grad_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_2                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc80083a4L pq_av_grad_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_3                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc80083a8L pq_av_grad_cnt_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_4                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_4_T;

/*-----------------------------------------------------------------------------
		0xc80083acL pq_av_grad_cnt_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_5                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_5_T;

/*-----------------------------------------------------------------------------
		0xc80083b0L pq_av_grad_cnt_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_6                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_6_T;

/*-----------------------------------------------------------------------------
		0xc80083b4L pq_av_grad_cnt_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_av_grad_cnt_7                   : 22;  // 21: 0     
} PE_H15A0_PQ_AV_GRAD_CNT_7_T;

/*-----------------------------------------------------------------------------
		0xc80083b8L pq_diff_grad_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_0                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc80083bcL pq_diff_grad_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_1                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc80083c0L pq_diff_grad_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_2                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc80083c4L pq_diff_grad_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_3                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc80083c8L pq_diff_grad_cnt_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_4                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_4_T;

/*-----------------------------------------------------------------------------
		0xc80083ccL pq_diff_grad_cnt_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_5                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_5_T;

/*-----------------------------------------------------------------------------
		0xc80083d0L pq_diff_grad_cnt_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_6                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_6_T;

/*-----------------------------------------------------------------------------
		0xc80083d4L pq_diff_grad_cnt_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_diff_grad_cnt_7                 : 22;  // 21: 0     
} PE_H15A0_PQ_DIFF_GRAD_CNT_7_T;

/*-----------------------------------------------------------------------------
		0xc80083d8L pq_blur_cnt_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_0                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_0_T;

/*-----------------------------------------------------------------------------
		0xc80083dcL pq_blur_cnt_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_1                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_1_T;

/*-----------------------------------------------------------------------------
		0xc80083e0L pq_blur_cnt_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_2                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_2_T;

/*-----------------------------------------------------------------------------
		0xc80083e4L pq_blur_cnt_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_3                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_3_T;

/*-----------------------------------------------------------------------------
		0xc80083e8L pq_blur_cnt_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_4                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_4_T;

/*-----------------------------------------------------------------------------
		0xc80083ecL pq_blur_cnt_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_5                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_5_T;

/*-----------------------------------------------------------------------------
		0xc80083f0L pq_blur_cnt_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_6                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_6_T;

/*-----------------------------------------------------------------------------
		0xc80083f4L pq_blur_cnt_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_blur_cnt_7                      : 22;  // 21: 0     
} PE_H15A0_PQ_BLUR_CNT_7_T;

/*-----------------------------------------------------------------------------
		0xc80083f8L pq_total_minmax_on ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_total_minmax_on                 : 22;  // 21: 0     
} PE_H15A0_PQ_TOTAL_MINMAX_ON_T;

/*-----------------------------------------------------------------------------
		0xc80083fcL pq_total_minmax_off ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pq_total_minmax_off                : 22;  // 21: 0     
} PE_H15A0_PQ_TOTAL_MINMAX_OFF_T;

/*-----------------------------------------------------------------------------
		0xc8008500L ft_up_grd_bl23_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_grd_bl23_coeff0_1            : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_grd_bl23_coeff0_0            : 6 ;  // 21:16     
} PE_H15A0_FT_UP_GRD_BL23_0_T;

/*-----------------------------------------------------------------------------
		0xc8008504L ft_up_grd_bl23_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_grd_bl23_coeff1_1            : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_grd_bl23_coeff1_0            : 6 ;  // 21:16     
} PE_H15A0_FT_UP_GRD_BL23_1_T;

/*-----------------------------------------------------------------------------
		0xc8008508L ft_up_grdavg_bl23_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_grdavg_bl23_coeff0_1         : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_grdavg_bl23_coeff0_0         : 6 ;  // 21:16     
} PE_H15A0_FT_UP_GRDAVG_BL23_0_T;

/*-----------------------------------------------------------------------------
		0xc800850cL ft_up_grdavg_bl23_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_grdavg_bl23_coeff1_1         : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_grdavg_bl23_coeff1_0         : 6 ;  // 21:16     
} PE_H15A0_FT_UP_GRDAVG_BL23_1_T;

/*-----------------------------------------------------------------------------
		0xc8008510L ft_up_ss_bl23_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_ss_bl23_coeff0_1             : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_ss_bl23_coeff0_0             : 6 ;  // 21:16     
} PE_H15A0_FT_UP_SS_BL23_0_T;

/*-----------------------------------------------------------------------------
		0xc8008514L ft_up_ss_bl23_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_ss_bl23_coeff1_1             : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_ss_bl23_coeff1_0             : 6 ;  // 21:16     
} PE_H15A0_FT_UP_SS_BL23_1_T;

/*-----------------------------------------------------------------------------
		0xc8008518L ft_up_lmm3_bl23_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_lmm3_bl23_coeff0_1           : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_lmm3_bl23_coeff0_0           : 6 ;  // 21:16     
} PE_H15A0_FT_UP_LMM3_BL23_0_T;

/*-----------------------------------------------------------------------------
		0xc800851cL ft_up_lmm3_bl23_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_lmm3_bl23_coeff1_1           : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_lmm3_bl23_coeff1_0           : 6 ;  // 21:16     
} PE_H15A0_FT_UP_LMM3_BL23_1_T;

/*-----------------------------------------------------------------------------
		0xc8008520L ft_up_ltv_bl23_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_ltv_bl23_coeff0_1            : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_ltv_bl23_coeff0_0            : 6 ;  // 21:16     
} PE_H15A0_FT_UP_LTV_BL23_0_T;

/*-----------------------------------------------------------------------------
		0xc8008524L ft_up_ltv_bl23_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_up_ltv_bl23_coeff1_1            : 6 ;  //  5: 0     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 ft_up_ltv_bl23_coeff1_0            : 6 ;  // 21:16     
} PE_H15A0_FT_UP_LTV_BL23_1_T;

#ifdef PE_SR_REG_H15A0_NEED_TO_REMAP
/*-----------------------------------------------------------------------------
		0xc8008800L ft_ext_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_height_inp                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 ft_width_inp                       : 13;  // 28:16     
} PE_H15A0_FT_EXT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8008804L ft_done_prd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ft_hdone_prd                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 f1_ymem_reserved0                  : 16;  // 31:16     
} PE_H15A0_FT_DONE_PRD_T;

/*-----------------------------------------------------------------------------
		0xc8008900L f1_ymem_sw_rst ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_sw_rst                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 f1_ymem_reserved3                  : 28;  // 31: 4     
} PE_H15A0_F1_YMEM_SW_RST_T;

/*-----------------------------------------------------------------------------
		0xc8008904L f1_ymem_vsyn_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_vsync_delay                : 20;  // 19: 0     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 f1_ymem_vsync_width                : 4 ;  // 27:24     
	UINT32 f1_ymem_vsync_delay_pol            : 1 ;  //    28     
} PE_H15A0_F1_YMEM_VSYN_DELAY_T;

/*-----------------------------------------------------------------------------
		0xc8008908L f1_ymem_line_duration ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_duration              : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 f1_ymem_reserved1                  : 16;  // 31:16     
} PE_H15A0_F1_YMEM_LINE_DURATION_T;

/*-----------------------------------------------------------------------------
		0xc800890cL f1_ymem_line_duration2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_duration2             : 15;  // 14: 0     
} PE_H15A0_F1_YMEM_LINE_DURATION2_T;

/*-----------------------------------------------------------------------------
		0xc8008910L f1_ymem_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_offset_d              : 4 ;  //  3: 0     
	UINT32 f1_ymem_line_offset_c              : 4 ;  //  7: 4     
	UINT32 f1_ymem_line_offset_b              : 4 ;  // 11: 8     
	UINT32 f1_ymem_line_offset_a              : 4 ;  // 15:12     
	UINT32 f1_ymem_line_offset_fw             : 4 ;  // 19:16     
	UINT32 f1_fw_hdone_delay_ctrl             : 6 ;  // 25:20     
} PE_H15A0_F1_YMEM_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8008914L f1_ymem_line_count0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_cnt_fw                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 f1_ymem_reserved2                  : 16;  // 31:16     
} PE_H15A0_F1_YMEM_LINE_COUNT0_T;

/*-----------------------------------------------------------------------------
		0xc8008918L f1_ymem_line_count1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_cnt_b                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 f1_ymem_line_cnt_a                 : 13;  // 28:16     
} PE_H15A0_F1_YMEM_LINE_COUNT1_T;

/*-----------------------------------------------------------------------------
		0xc800891cL f1_ymem_line_count2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_line_cnt_d                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 f1_ymem_line_cnt_c                 : 13;  // 28:16     
} PE_H15A0_F1_YMEM_LINE_COUNT2_T;

/*-----------------------------------------------------------------------------
		0xc8008920L f1_ymem_sr1_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_ymem_sr1_delay                  : 20;  // 19: 0     
} PE_H15A0_F1_YMEM_SR1_DELAY_T;

/*-----------------------------------------------------------------------------
		0xc8008924L f1_engine_rev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 f1_engine_rev_num                  : 32;  // 31: 0     
} PE_H15A0_F1_ENGINE_REV_T;
#endif

/*-----------------------------------------------------------------------------
		0xc8009000L core1_scaler_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_scale_coeff0_1            : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 core1_iu_scale_coeff0_0            : 11;  // 26:16     
} PE_H15A0_CORE1_SCALER_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc8009004L core1_scaler_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_scale_coeff0_3            : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 core1_iu_scale_coeff0_2            : 11;  // 26:16     
} PE_H15A0_CORE1_SCALER_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc8009008L core1_scaler_coef2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_scale_coeff1_1            : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 core1_iu_scale_coeff1_0            : 11;  // 26:16     
} PE_H15A0_CORE1_SCALER_COEF2_T;

/*-----------------------------------------------------------------------------
		0xc800900cL core1_scaler_coef3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_scale_coeff1_3            : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 core1_iu_scale_coeff1_2            : 11;  // 26:16     
} PE_H15A0_CORE1_SCALER_COEF3_T;

/*-----------------------------------------------------------------------------
		0xc8009010L core1_iu_blur_hcoeff1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_blur_gaussian_hcoeff1     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_blur_gaussian_hcoeff0     : 10;  // 25:16     
} PE_H15A0_CORE1_IU_BLUR_HCOEFF1_T;

/*-----------------------------------------------------------------------------
		0xc8009014L core1_iu_blur_hcoeff2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_blur_gaussian_hcoeff3     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_blur_gaussian_hcoeff2     : 10;  // 25:16     
} PE_H15A0_CORE1_IU_BLUR_HCOEFF2_T;

/*-----------------------------------------------------------------------------
		0xc8009018L core1_iu_blur_hcoeff3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_blur_gaussian_hcoeff5     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_blur_gaussian_hcoeff4     : 10;  // 25:16     
} PE_H15A0_CORE1_IU_BLUR_HCOEFF3_T;

/*-----------------------------------------------------------------------------
		0xc800901cL core1_iu_blur_vcoeff1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_blur_gaussian_vcoeff0     : 10;  //  9: 0     
} PE_H15A0_CORE1_IU_BLUR_VCOEFF1_T;

/*-----------------------------------------------------------------------------
		0xc8009020L core1_iu_blur_vcoeff2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_blur_gaussian_vcoeff2     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_iu_blur_gaussian_vcoeff1     : 10;  // 25:16     
} PE_H15A0_CORE1_IU_BLUR_VCOEFF2_T;

/*-----------------------------------------------------------------------------
		0xc8009024L core1_iu_um_clip_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_um_clip_en                : 1 ;  //     0     
} PE_H15A0_CORE1_IU_UM_CLIP_EN_T;

/*-----------------------------------------------------------------------------
		0xc8009028L core1_iu_um_r_clip_lv1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_um_r_clip_lv3             : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_iu_um_r_clip_lv2             : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_iu_um_r_clip_lv1             : 5 ;  // 20:16     
} PE_H15A0_CORE1_IU_UM_R_CLIP_LV1_T;

/*-----------------------------------------------------------------------------
		0xc800902cL core1_iu_um_r_clip_lv2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_um_r_clip_lv5             : 5 ;  //  4: 0     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 core1_iu_um_r_clip_lv4             : 5 ;  // 20:16     
} PE_H15A0_CORE1_IU_UM_R_CLIP_LV2_T;

/*-----------------------------------------------------------------------------
		0xc8009030L core1_id_blur_hcoeff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_id_blur_gaussian_hcoeff1     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_id_blur_gaussian_hcoeff0     : 10;  // 25:16     
} PE_H15A0_CORE1_ID_BLUR_HCOEFF_T;

/*-----------------------------------------------------------------------------
		0xc8009034L core1_id_blur_vcoeff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_id_blur_gaussian_vcoeff1     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_id_blur_gaussian_vcoeff0     : 10;  // 25:16     
} PE_H15A0_CORE1_ID_BLUR_VCOEFF_T;

/*-----------------------------------------------------------------------------
		0xc8009038L core1_iu_delta_cross0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_delta_c3                 : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lue_delta_c2                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lue_delta_c1                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lue_delta_c0                 : 5 ;  // 28:24     
} PE_H15A0_CORE1_IU_DELTA_CROSS0_T;

/*-----------------------------------------------------------------------------
		0xc800903cL core1_iu_delta_cross1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_delta_c7                 : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lue_delta_c6                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lue_delta_c5                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lue_delta_c4                 : 5 ;  // 28:24     
} PE_H15A0_CORE1_IU_DELTA_CROSS1_T;

/*-----------------------------------------------------------------------------
		0xc8009040L core1_iu_delta_dial0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_delta_d3                 : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lue_delta_d2                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lue_delta_d1                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lue_delta_d0                 : 5 ;  // 28:24     
} PE_H15A0_CORE1_IU_DELTA_DIAL0_T;

/*-----------------------------------------------------------------------------
		0xc8009044L core1_iu_delta_dial1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_delta_d7                 : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_lue_delta_d6                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_lue_delta_d5                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_lue_delta_d4                 : 5 ;  // 28:24     
} PE_H15A0_CORE1_IU_DELTA_DIAL1_T;

/*-----------------------------------------------------------------------------
		0xc8009048L core1_lue_avg_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_lue_avg_nf                   : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_lue_avg_th                   : 12;  // 27:16     
} PE_H15A0_CORE1_LUE_AVG_TH_T;

/*-----------------------------------------------------------------------------
		0xc800904cL core1_use_iu_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_use_iu_en                    : 1 ;  //     0     
} PE_H15A0_CORE1_USE_IU_EN_T;

/*-----------------------------------------------------------------------------
		0xc8009050L core1_iu_en_4mm_min_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_iu_en_4mm_max_th             : 5 ;  //  4: 0     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 core1_iu_en_4mm_min_th             : 5 ;  // 20:16     
} PE_H15A0_CORE1_IU_EN_4MM_MIN_TH_T;

/*-----------------------------------------------------------------------------
		0xc8009054L core1_we_id_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_range0                 : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_0_T;

/*-----------------------------------------------------------------------------
		0xc8009058L core1_we_id_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_we_id_range1                 : 8 ;  //  7: 0     
} PE_H15A0_CORE1_WE_ID_1_T;

/*-----------------------------------------------------------------------------
		0xc800905cL core1_fuse_grd_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_grd_th_2                : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_fuse_grd_th_1                : 10;  // 25:16     
} PE_H15A0_CORE1_FUSE_GRD_TH_T;

/*-----------------------------------------------------------------------------
		0xc8009060L core1_fuse_grd_r1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_grd_r_4                 : 8 ;  //  7: 0     
	UINT32 core1_fuse_grd_r_3                 : 8 ;  // 15: 8     
	UINT32 core1_fuse_grd_r_2                 : 8 ;  // 23:16     
	UINT32 core1_fuse_grd_r_1                 : 8 ;  // 31:24     
} PE_H15A0_CORE1_FUSE_GRD_R1_T;

/*-----------------------------------------------------------------------------
		0xc8009064L core1_fuse_grd_r2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_grd_r_6                 : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_fuse_grd_r_5                 : 8 ;  // 23:16     
} PE_H15A0_CORE1_FUSE_GRD_R2_T;

/*-----------------------------------------------------------------------------
		0xc8009068L core1_fuse_we_sc1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_we_sc_4                 : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_fuse_we_sc_3                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_fuse_we_sc_2                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 core1_fuse_we_sc_1                 : 5 ;  // 28:24     
} PE_H15A0_CORE1_FUSE_WE_SC1_T;

/*-----------------------------------------------------------------------------
		0xc800906cL core1_fuse_we_sc2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_we_sc_6                 : 5 ;  //  4: 0     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 core1_fuse_we_sc_5                 : 5 ;  // 20:16     
} PE_H15A0_CORE1_FUSE_WE_SC2_T;

/*-----------------------------------------------------------------------------
		0xc8009070L core1_fuse_r_grd_iuid ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_r_grd_iuid2             : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 core1_fuse_r_grd_iuid1             : 8 ;  // 23:16     
} PE_H15A0_CORE1_FUSE_R_GRD_IUID_T;

/*-----------------------------------------------------------------------------
		0xc8009074L core1_fuse_diff_iu_id_um_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_diff_iu_id_um_th2       : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 core1_fuse_diff_iu_id_um_th1       : 10;  // 25:16     
} PE_H15A0_CORE1_FUSE_DIFF_IU_ID_UM_TH_T;

/*-----------------------------------------------------------------------------
		0xc8009078L core1_fuse_we_diff_iuid_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_we_diff_iuid_3          : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_fuse_we_diff_iuid_2          : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_fuse_we_diff_iuid_1          : 5 ;  // 20:16     
} PE_H15A0_CORE1_FUSE_WE_DIFF_IUID_1_T;

/*-----------------------------------------------------------------------------
		0xc800907cL core1_fuse_we_diff_iuid_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_fuse_we_diff_iuid_6          : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 core1_fuse_we_diff_iuid_5          : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 core1_fuse_we_diff_iuid_4          : 5 ;  // 20:16     
} PE_H15A0_CORE1_FUSE_WE_DIFF_IUID_2_T;

/*-----------------------------------------------------------------------------
		0xc8009080L core1_use_jtwo ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_use_jtwo                     : 1 ;  //     0     
} PE_H15A0_CORE1_USE_JTWO_T;

/*-----------------------------------------------------------------------------
		0xc8009084L core1_j2w_cost_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_j2w_cost_th1                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 core1_j2w_cost_th0                 : 12;  // 27:16     
} PE_H15A0_CORE1_J2W_COST_TH_T;

/*-----------------------------------------------------------------------------
		0xc8009088L core1_j2w_cost_sub_ratio ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 core1_j2w_cost_sub_ratio1          : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 core1_j2w_cost_sub_ratio0          : 3 ;  // 18:16     
} PE_H15A0_CORE1_J2W_COST_SUB_RATIO_T;

/*-----------------------------------------------------------------------------
		0xc800908cL core1_3d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mode_3d_tb                         : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 mode_3d_ss                         : 1 ;  //    16     
} PE_H15A0_CORE1_3D_T;

/*-----------------------------------------------------------------------------
		0xc8009090L debug_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_mode                         : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 debug_en                           : 1 ;  //    16     
} PE_H15A0_DEBUG_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8009094L s1_demo_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_demo_mode                       : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 s1_demo_en                         : 1 ;  //    16     
} PE_H15A0_S1_DEMO_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8009098L s1_demo_pos_x ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_demo_end_x                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 s1_demo_start_x                    : 12;  // 27:16     
} PE_H15A0_S1_DEMO_POS_X_T;

#ifdef PE_SR_REG_H15A0_NEED_TO_REMAP
/*-----------------------------------------------------------------------------
		0xc8009200L core1_scaler_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scaler_input_height                : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 scaler_input_width                 : 13;  // 28:16     
} PE_H15A0_CORE1_SCALER_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8009204L core1_emp_enh5x5_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 emp_enh5x5_height                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 emp_enh5x5_width                   : 13;  // 28:16     
} PE_H15A0_CORE1_EMP_ENH5X5_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8009208L core1_enh3x3_lm11_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enh3x3_lm11_height                 : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 enh3x3_lm11_width                  : 13;  // 28:16     
} PE_H15A0_CORE1_ENH3X3_LM11_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc800920cL core1_out_lm23_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_lm23_height                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 out_lm23_width                     : 13;  // 28:16     
} PE_H15A0_CORE1_OUT_LM23_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8009210L core1_md_proc_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 md_width                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 md_height                          : 11;  // 26:16     
} PE_H15A0_CORE1_MD_PROC_BLK_T;

/*-----------------------------------------------------------------------------
		0xc8009214L core1_mu_proc_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mu_width                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 mu_height                          : 11;  // 26:16     
} PE_H15A0_CORE1_MU_PROC_BLK_T;

/*-----------------------------------------------------------------------------
		0xc8009218L core1_mf_proc_blk ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mf_width                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 mf_height                          : 11;  // 26:16     
} PE_H15A0_CORE1_MF_PROC_BLK_T;

/*-----------------------------------------------------------------------------
		0xc8009400L dvom_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dvom_disp_ysize                    : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 dvom_disp_xsize                    : 13;  // 28:16     
} PE_H15A0_DVOM_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8009404L dvo_pg_pat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lattice_pat_en                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 ramp_pat_en                        : 2 ;  //  5: 4     
	UINT32 mpg_o_color_sel                    : 2 ;  //  7: 6     
	UINT32 border_sel                         : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 box7_en                            : 1 ;  //    16     
	UINT32 box6_en                            : 1 ;  //    17     
	UINT32 box5_en                            : 1 ;  //    18     
	UINT32 box4_en                            : 1 ;  //    19     
	UINT32 box3_en                            : 1 ;  //    20     
	UINT32 box2_en                            : 1 ;  //    21     
	UINT32 box1_en                            : 1 ;  //    22     
	UINT32 box0_en                            : 1 ;  //    23     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 pg_in_sel                          : 1 ;  //    30     
	UINT32 pg_out_sel                         : 1 ;  //    31     
} PE_H15A0_DVO_PG_PAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8009408L dvo_pg_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_start_hpos                    : 13;  // 12: 0     
	UINT32                                    : 10;  // 22:13     reserved
	UINT32 disp_pulse_sel                     : 1 ;  //    23     
	UINT32 sync_o_inv_sel                     : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 force_update_src_sel               : 2 ;  // 27:26     
	UINT32 sg_reset                           : 1 ;  //    28     
	UINT32                                    : 1 ;  //    29     reserved
	UINT32 force_update_auto_en               : 1 ;  //    30     
	UINT32 force_update                       : 1 ;  //    31     
} PE_H15A0_DVO_PG_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800940cL dvom_sync0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dvom_vsync                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 dvom_vfp                           : 13;  // 28:16     
} PE_H15A0_DVOM_SYNC0_T;

/*-----------------------------------------------------------------------------
		0xc8009410L dvom_sync1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dvom_va                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 dvom_vbp                           : 13;  // 28:16     
} PE_H15A0_DVOM_SYNC1_T;

/*-----------------------------------------------------------------------------
		0xc8009414L dvom_sync2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dvom_hsync                         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 dvom_hfp                           : 13;  // 28:16     
} PE_H15A0_DVOM_SYNC2_T;

/*-----------------------------------------------------------------------------
		0xc8009418L dvom_sync3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dvom_ha                            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 dvom_hbp                           : 13;  // 28:16     
} PE_H15A0_DVOM_SYNC3_T;

/*-----------------------------------------------------------------------------
		0xc8009500L s1_ymem_sw_rst ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_sw_rst                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 s1_ymem_reserved3                  : 28;  // 31: 4     
} PE_H15A0_S1_YMEM_SW_RST_T;

/*-----------------------------------------------------------------------------
		0xc8009504L s1_ymem_pinit_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_pinit_delay                : 20;  // 19: 0     
} PE_H15A0_S1_YMEM_PINIT_DELAY_T;

/*-----------------------------------------------------------------------------
		0xc8009508L s1_ymem_vsyn_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_vsync_delay                : 20;  // 19: 0     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 s1_ymem_vsync_width                : 4 ;  // 27:24     
	UINT32 s1_ymem_vsync_delay_pol            : 1 ;  //    28     
} PE_H15A0_S1_YMEM_VSYN_DELAY_T;

/*-----------------------------------------------------------------------------
		0xc800950cL s1_ymem_line_duration ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_duration              : 15;  // 14: 0     
} PE_H15A0_S1_YMEM_LINE_DURATION_T;

/*-----------------------------------------------------------------------------
		0xc8009510L s1_ymem_line_duration2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_duration2             : 15;  // 14: 0     
} PE_H15A0_S1_YMEM_LINE_DURATION2_T;

/*-----------------------------------------------------------------------------
		0xc8009514L s1_ymem_offset0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_offset_enh50          : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 s1_ymem_line_offset_lm1            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_offset_scaler1        : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 s1_ymem_line_offset_blur1          : 5 ;  // 28:24     
} PE_H15A0_S1_YMEM_OFFSET0_T;

/*-----------------------------------------------------------------------------
		0xc8009518L s1_ymem_offset1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_offset_enh3           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 s1_ymem_line_offset_enh51          : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_offset_lm2            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 s1_ymem_line_offset_emp1           : 5 ;  // 28:24     
} PE_H15A0_S1_YMEM_OFFSET1_T;

/*-----------------------------------------------------------------------------
		0xc800951cL s1_ymem_offset2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_offset_fuse           : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 s1_ymem_line_offset_avg            : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_offset_grd            : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 s1_ymem_line_offset_co1            : 5 ;  // 28:24     
} PE_H15A0_S1_YMEM_OFFSET2_T;

/*-----------------------------------------------------------------------------
		0xc8009520L s1_ymem_offset3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_offset_mfwr           : 5 ;  //  4: 0     
} PE_H15A0_S1_YMEM_OFFSET3_T;

/*-----------------------------------------------------------------------------
		0xc8009524L s1_ymem_line_count0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_scaler1           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_blur1             : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT0_T;

/*-----------------------------------------------------------------------------
		0xc8009528L s1_ymem_line_count1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_enh50             : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_lm1               : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT1_T;

/*-----------------------------------------------------------------------------
		0xc800952cL s1_ymem_line_count2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_lm2               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_emp1              : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT2_T;

/*-----------------------------------------------------------------------------
		0xc8009530L s1_ymem_line_count3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_enh3              : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_enh51             : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT3_T;

/*-----------------------------------------------------------------------------
		0xc8009534L s1_ymem_line_count4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_grd               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_co1               : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT4_T;

/*-----------------------------------------------------------------------------
		0xc8009538L s1_ymem_line_count5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_fuse              : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_ymem_line_cnt_avg               : 13;  // 28:16     
} PE_H15A0_S1_YMEM_LINE_COUNT5_T;

/*-----------------------------------------------------------------------------
		0xc800953cL s1_ymem_line_count6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_line_cnt_mfwr              : 13;  // 12: 0     
} PE_H15A0_S1_YMEM_LINE_COUNT6_T;

/*-----------------------------------------------------------------------------
		0xc8009540L s1_ymem_reserved0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_reserved0                  : 32;  // 31: 0     
} PE_H15A0_S1_YMEM_RESERVED0_T;

/*-----------------------------------------------------------------------------
		0xc8009544L s1_ymem_reserved1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_reserved1                  : 32;  // 31: 0     
} PE_H15A0_S1_YMEM_RESERVED1_T;

/*-----------------------------------------------------------------------------
		0xc8009548L s1_ymem_reserved2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_ymem_reserved2                  : 32;  // 31: 0     
} PE_H15A0_S1_YMEM_RESERVED2_T;

/*-----------------------------------------------------------------------------
		0xc800954cL s1_fb_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_fp_linecnt                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 s1_fp_clkcnt                       : 16;  // 31:16     
} PE_H15A0_S1_FB_INFO_T;

/*-----------------------------------------------------------------------------
		0xc8009550L s1_vs_rise_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_vs_rise_linecnt                 : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 s1_vs_rise_clkcnt                  : 16;  // 31:16     
} PE_H15A0_S1_VS_RISE_INFO_T;

/*-----------------------------------------------------------------------------
		0xc8009554L s1_dbg_de_width ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_in_de_width                     : 11;  // 10: 0     
} PE_H15A0_S1_DBG_DE_WIDTH_T;

/*-----------------------------------------------------------------------------
		0xc8009558L s1_dbg_linecnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_in_line_cnt                     : 11;  // 10: 0     
} PE_H15A0_S1_DBG_LINECNT_T;

/*-----------------------------------------------------------------------------
		0xc800955cL s1_dbg_line_total_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_in_line_total_cnt               : 11;  // 10: 0     
} PE_H15A0_S1_DBG_LINE_TOTAL_CNT_T;

/*-----------------------------------------------------------------------------
		0xc8009560L s1_intr_in0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_set_intr0_y                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_set_intr0_x                     : 13;  // 28:16     
} PE_H15A0_S1_INTR_IN0_T;

/*-----------------------------------------------------------------------------
		0xc8009564L s1_intr_in1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_set_intr1_y                     : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 s1_set_intr1_x                     : 13;  // 28:16     
} PE_H15A0_S1_INTR_IN1_T;

/*-----------------------------------------------------------------------------
		0xc8009568L s1_proc_line_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_proc_line_number                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 s1_input_line_number               : 12;  // 27:16     
} PE_H15A0_S1_PROC_LINE_INFO_T;

/*-----------------------------------------------------------------------------
		0xc800956cL s1_mon_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_mon_sel                         : 3 ;  //  2: 0     
} PE_H15A0_S1_MON_SEL_T;

/*-----------------------------------------------------------------------------
		0xc8009570L s1_engine_rev ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s1_engine_rev_num                  : 32;  // 31: 0     
} PE_H15A0_S1_ENGINE_REV_T;
#endif

/*-----------------------------------------------------------------------------
		0xc800a000L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_H15A0_SR_SHP_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a004L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16     
} PE_H15A0_SR_SHP_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a008L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 pe1_output_active_sel              : 1 ;  //     1     
	UINT32 pe1_detour                         : 1 ;  //     2     
	UINT32                                    : 4 ;  //  6: 3     reserved
	UINT32 mode_3d                            : 1 ;  //     7     
	UINT32 vdpulse_pos                        : 12;  // 19: 8     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 master_en                          : 1 ;  //    24     
} PE_H15A0_SR_SHP_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a00cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0     
	UINT32 va_pe1_in                          : 1 ;  //    12     
	UINT32 va_pe1_out                         : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16     
} PE_H15A0_SR_SHP_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a010L pe1_top_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_to_cnt                           : 12;  // 11: 0     
} PE_H15A0_SR_SHP_TOP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a020L pe1_win1_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SR_SHP_WIN1_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a024L pe1_win1_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN1_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a028L pe1_win1_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN1_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a02cL pe1_win1_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN1_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a030L pe1_win1_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN1_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a040L pe1_fsw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_FSW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a044L pe1_fsw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_FSW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a048L pe1_fsw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_FSW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a04cL pe1_fsw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_FSW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a060L disp_sync_dly_addr00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_vsync                     : 16;  // 15: 0     
	UINT32 dly_sync_vfp                       : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR00_T;

/*-----------------------------------------------------------------------------
		0xc800a064L disp_sync_dly_addr01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_va                        : 16;  // 15: 0     
	UINT32 dly_sync_vbp                       : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR01_T;

/*-----------------------------------------------------------------------------
		0xc800a068L disp_sync_dly_addr02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_hsync                     : 16;  // 15: 0     
	UINT32 dly_sync_hfp                       : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR02_T;

/*-----------------------------------------------------------------------------
		0xc800a06cL disp_sync_dly_addr03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_ha                        : 16;  // 15: 0     
	UINT32 dly_sync_hbp                       : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR03_T;

/*-----------------------------------------------------------------------------
		0xc800a070L disp_sync_dly_addr04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_line_start                : 16;  // 15: 0     
	UINT32 dly_sync_actv_sel_in_h_src         : 1 ;  //    16     
	UINT32 dly_sync_actv_sel_in_v_src         : 1 ;  //    17     
	UINT32 dly_sync_actv_sel_in_h_inv         : 1 ;  //    18     
	UINT32 dly_sync_actv_sel_in_v_inv         : 1 ;  //    19     
	UINT32 dly_sync_actv_sel_out_ha           : 1 ;  //    20     
	UINT32 dly_sync_actv_sel_out_va           : 1 ;  //    21     
	UINT32 dly_sync_actv_sel_out_hs           : 1 ;  //    22     
	UINT32 dly_sync_actv_sel_out_vs           : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dly_sync_force_update              : 1 ;  //    31     
} PE_H15A0_DISP_SYNC_DLY_ADDR04_T;

/*-----------------------------------------------------------------------------
		0xc800a074L disp_sync_dly_addr05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_h_dly                     : 16;  // 15: 0     
	UINT32 dly_sync_v_dly                     : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR05_T;

/*-----------------------------------------------------------------------------
		0xc800a078L disp_sync_dly_addr06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_value                  : 16;  // 15: 0     
	UINT32 cnt_v_reset_value                  : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR06_T;

/*-----------------------------------------------------------------------------
		0xc800a07cL disp_sync_dly_addr07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_actv_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_actv_v_dly                : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR07_T;

/*-----------------------------------------------------------------------------
		0xc800a080L disp_sync_dly_addr08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_sync_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_sync_v_dly                : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR08_T;

/*-----------------------------------------------------------------------------
		0xc800a084L disp_sync_dly_addr09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_normal_number                  : 16;  // 15: 0     
} PE_H15A0_DISP_SYNC_DLY_ADDR09_T;

/*-----------------------------------------------------------------------------
		0xc800a088L disp_sync_dly_addr0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_position               : 16;  // 15: 0     
	UINT32 cnt_v_reset_position               : 16;  // 31:16     
} PE_H15A0_DISP_SYNC_DLY_ADDR0A_T;

/*-----------------------------------------------------------------------------
		0xc800a090L pe1_win3_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SR_SHP_WIN3_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a094L pe1_win3_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN3_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a098L pe1_win3_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN3_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a09cL pe1_win3_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN3_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a0a0L pe1_win3_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SR_SHP_WIN3_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a0b0L pe1_derh_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsum_mode                          : 2 ;  //  1: 0     
	UINT32 hsum_mode                          : 2 ;  //  3: 2     
	UINT32 vsum121_en                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 vertical_min_max                   : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 a_flt_mux                          : 2 ;  // 17:16     
	UINT32 a_e_mux                            : 2 ;  // 19:18     
	UINT32 t_mux                              : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_gen_width                        : 4 ;  // 27:24     
} PE_H15A0_SR_SHP_DERH_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a0b4L pe1_derh_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_gen_of_y                         : 8 ;  //  7: 0     
	UINT32 a_gen_of_c                         : 8 ;  // 15: 8     
	UINT32 avg_flt_mode                       : 3 ;  // 18:16     
	UINT32 avg_flt_en                         : 1 ;  //    19     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 max_flt_mode                       : 3 ;  // 26:24     
} PE_H15A0_SR_SHP_DERH_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a0b8L pe1_derh_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_exp_gain_ctrl_y0                 : 4 ;  //  3: 0     
	UINT32 a_exp2_gain_ctrl_x0                : 4 ;  //  7: 4     
	UINT32 a_exp2_gain_ctrl_y1                : 4 ;  // 11: 8     
	UINT32 a_exp2_gain_ctrl_x1                : 4 ;  // 15:12     
	UINT32 a_avg2_flt_wid                     : 4 ;  // 19:16     
	UINT32 a_avg2_flt_en                      : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_exp_max2_en                      : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 y_sum_mode                         : 2 ;  // 27:26     
} PE_H15A0_SR_SHP_DERH_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a0bcL pe1_derh_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 5 ;  //  4: 0     reserved
	UINT32 a_mux_for_edge_gaintable           : 3 ;  //  7: 5     
	UINT32                                    : 9 ;  // 16: 8     reserved
	UINT32 a_mux_for_detail_filter            : 3 ;  // 19:17     
	UINT32                                    : 1 ;  //    20     reserved
	UINT32 a_mux_for_edge_filter              : 3 ;  // 23:21     
	UINT32 a_scaling_flat_filter_             : 4 ;  // 27:24     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 a_mux_for_flat_filter              : 3 ;  // 31:29     
} PE_H15A0_SR_SHP_DERH_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a0c0L pe1_derh_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 reg_dbg_scale                      : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_mux                            : 4 ;  //  7: 4     
	UINT32 reg_enh_en_cc                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_enh_en_yy                      : 3 ;  // 14:12     
} PE_H15A0_SR_SHP_DERH_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a0c4L pe1_derh_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmm_param                      : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_csft_gain                      : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_th_gain_edge                   : 6 ;  // 21:16     
	UINT32 reg_th_gain_flat                   : 2 ;  // 23:22     
	UINT32 reg_th_manual_th                   : 7 ;  // 30:24     
	UINT32 reg_th_manual_en                   : 1 ;  //    31     
} PE_H15A0_SR_SHP_DERH_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800a0c8L pe1_derh_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 edge_filter_white_gain             : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 edge_filter_black_gain             : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_amean_en                       : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 edge_filter_v_tap                  : 2 ;  // 27:26     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 edge_c_filter_en                   : 1 ;  //    30     
	UINT32 edge_y_filter_en                   : 1 ;  //    31     
} PE_H15A0_SR_SHP_DERH_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800a0ccL pe1_derh_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_filter_tap                     : 2 ;  //  1: 0     
	UINT32                                    : 22;  // 23: 2     reserved
	UINT32 flat_filter_gain                   : 6 ;  // 29:24     
	UINT32 flat_filter_type                   : 1 ;  //    30     
	UINT32 flat_filter_en                     : 1 ;  //    31     
} PE_H15A0_SR_SHP_DERH_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800a0d0L pe1_derh_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl0_y0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl0_x0                      : 8 ;  // 15: 8     
	UINT32 amod_ctrl0_y1                      : 8 ;  // 23:16     
	UINT32 amod_ctrl0_x1                      : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_DERH_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800a0d4L pe1_derh_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl1_x0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl1_x1                      : 8 ;  // 15: 8     
	UINT32 y_c_mux_control                    : 3 ;  // 18:16     
	UINT32 chroma_weight                      : 5 ;  // 23:19     
} PE_H15A0_SR_SHP_DERH_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800a0d8L pe1_derh_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a2g_mode_e                         : 1 ;  //     0     
	UINT32 a2g_mode_f                         : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 a2g_mgain_e                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 a2g_mgain_f                        : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 debug_mode                         : 4 ;  // 27:24     
} PE_H15A0_SR_SHP_DERH_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800a0dcL pe1_derh_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_th1                         : 8 ;  //  7: 0     
	UINT32 e_gain_th2                         : 8 ;  // 15: 8     
	UINT32 f_gain_th1                         : 8 ;  // 23:16     
	UINT32 f_gain_th2                         : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_DERH_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800a0e0L pe1_derh_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_max                         : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 f_gain_max                         : 6 ;  // 13: 8     
} PE_H15A0_SR_SHP_DERH_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800a0e4L pe1_coring_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y2                                 : 8 ;  //  7: 0     
	UINT32 x2                                 : 8 ;  // 15: 8     
	UINT32 y3                                 : 8 ;  // 23:16     
	UINT32 x3                                 : 8 ;  // 31:24     
}  PE_H15A0_SR_SHP_PE1_CORING_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800a0e4L pe1_derh_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_DERH_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc800a0f0L pe1_derv_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_v_en                           : 1 ;  //     0     
	UINT32 der_gain_mapping                   : 2 ;  //  2: 1     
	UINT32 bif_en                             : 1 ;  //     3     
	UINT32 output_mux                         : 4 ;  //  7: 4     
	UINT32 bif_manual_th                      : 8 ;  // 15: 8     
	UINT32 th_gain                            : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 th_mode                            : 1 ;  //    24     
} PE_H15A0_SR_SHP_DERV_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800a0f4L pe1_derv_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csft_gain                          : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 csft_mode                          : 1 ;  //     8     
} PE_H15A0_SR_SHP_DERV_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800a0f8L pe1_derv_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mmd_sel                            : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 max_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 avg_sel                            : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 gain_th1                           : 8 ;  // 23:16     
	UINT32 gain_th2                           : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_DERV_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800a0fcL pe1_derv_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_b                             : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gain_w                             : 7 ;  // 14: 8     
} PE_H15A0_SR_SHP_DERV_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800a110L pe1_dp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_display                      : 3 ;  //  2: 0     
} PE_H15A0_SR_SHP_DP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a114L pe1_dp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SR_SHP_DP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a118L pe1_dp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ptic_gain_e_b                  : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_ptic_gain_e_w                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_ptic_gain_t_b                  : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_ptic_gain_t_w                  : 7 ;  // 30:24     
} PE_H15A0_SR_SHP_DP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a11cL pe1_dp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_DP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a130L pe1_sp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SR_SHP_SP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a134L pe1_sp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_SP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a138L pe1_sp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_SP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a13cL pe1_sp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_SP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a140L pe1_sp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_SP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a144L pe1_sp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_SP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800a148L pe1_sp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 1 ;  //     4     
} PE_H15A0_SR_SHP_SP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800a14cL pe1_sp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_SP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800a150L pe1_sp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_SP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800a154L pe1_sp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_SP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800a158L pe1_sp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_SP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800a15cL pe1_sp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_SP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800a160L pe1_sp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SR_SHP_SP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800a170L pe1_mp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SR_SHP_MP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a174L pe1_mp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_MP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a178L pe1_mp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_MP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a17cL pe1_mp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_MP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a180L pe1_mp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_MP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a184L pe1_mp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_MP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800a188L pe1_mp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 3 ;  //  6: 4     
} PE_H15A0_SR_SHP_MP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800a18cL pe1_mp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_MP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800a190L pe1_mp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_MP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800a194L pe1_mp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_MP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800a198L pe1_mp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_MP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800a19cL pe1_mp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_MP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800a1a0L pe1_mp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SR_SHP_MP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800a1b0L pe1_coring_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_map_h_size                   : 2 ;  //  1: 0     
	UINT32 reg_a_nomalize_en                  : 1 ;  //     2     
	UINT32 reg_t_nomalize_en                  : 1 ;  //     3     
	UINT32 reg_t_cut_resolution               : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 reg_cross_th_sc                    : 8 ;  // 23:16     
	UINT32 reg_cross_th                       : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a1b4L pe1_coring_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_beta_mul                : 8 ;  //  7: 0     
	UINT32 reg_detail_alpha_mul               : 8 ;  // 15: 8     
	UINT32 reg_edge_beta_mul                  : 8 ;  // 23:16     
	UINT32 reg_edge_alpha_mul                 : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a1b8L pe1_coring_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_a_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_a_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_a_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a1bcL pe1_coring_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_a_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_a_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_a_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a1c0L pe1_coring_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_t_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_t_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_t_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a1c4L pe1_coring_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_t_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_t_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_t_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800a1c8L pe1_coring_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_edge_minmax_y0                 : 8 ;  //  7: 0     
	UINT32 reg_edge_minmax_x0                 : 8 ;  // 15: 8     
	UINT32 reg_edge_minmax_y1                 : 8 ;  // 23:16     
	UINT32 reg_edge_minmax_x1                 : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800a1ccL pe1_coring_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_minmax_y0               : 8 ;  //  7: 0     
	UINT32 reg_detail_minmax_x0               : 8 ;  // 15: 8     
	UINT32 reg_detail_minmax_y1               : 8 ;  // 23:16
	UINT32 reg_detail_minmax_x1               : 8 ;  // 31:24
} PE_H15A0_SR_SHP_CORING_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800a1d0L pe1_coring_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mp_coring_mode                     : 2 ;  //  1: 0     
	UINT32 sp_coring_mode                     : 2 ;  //  3: 2     
	UINT32 mp_coring_en                       : 1 ;  //     4     
	UINT32 sp_coring_en                       : 1 ;  //     5     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 reg_mul_base_t                     : 8 ;  // 23:16     
	UINT32 reg_mul_base_e                     : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_CORING_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800a1d4L pe1_coring_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SR_SHP_CORING_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800a1e0L pe1_coring_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tm_h                               : 16;  // 15: 0     
	UINT32 tm_v                               : 16;  // 31:16     
} PE_H15A0_SR_SHP_CORING_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800a1e4L pe1_coring_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tm_h                               : 16;  // 15: 0     
} PE_H15A0_SR_SHP_CORING_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc800a1e8L pe1_coring_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_CORING_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800a1ecL pe1_coring_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_CORING_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc800a200L pe1_pti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SR_SHP_PTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a204L pe1_pti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SR_SHP_PTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a208L pe1_sti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
	UINT32 reg_texture_gain                   : 8 ;  // 23:16     
	UINT32 reg_edge_gain                      : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_STI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a20cL pe1_sti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SR_SHP_STI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a220L pe1_dj_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edf_en                             : 1 ;  //     0     
	UINT32 hv_filter_en                       : 1 ;  //     1     
	UINT32 g0_feature_norm_en                 : 1 ;  //     2     
	UINT32 g0_f2g_mapping_en                  : 1 ;  //     3     
	UINT32 line_variation_mode                : 1 ;  //     4     
	UINT32 l_type_protection                  : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 direction_type_mode                : 1 ;  //     7     
	UINT32 count_diff_th                      : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 output_mux                         : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 center_blur_mode                   : 2 ;  // 21:20     
	UINT32 n_avg_mode                         : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 line_variation_diff_threshold      : 8 ;  // 31:24     
} PE_H15A0_SR_SHP_DJ_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a224L pe1_dj_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 level_th                           : 8 ;  //  7: 0     
	UINT32 protect_th                         : 8 ;  // 15: 8     
	UINT32 n_avg_gain                         : 8 ;  // 23:16     
} PE_H15A0_SR_SHP_DJ_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a228L pe1_dj_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g0_cnt_min                     : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_g0_mul                         : 5 ;  // 12: 8     
} PE_H15A0_SR_SHP_DJ_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a22cL pe1_dj_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g1_protect_min                 : 8 ;  //  7: 0     
	UINT32 reg_g1_mul                         : 7 ;  // 14: 8     
} PE_H15A0_SR_SHP_DJ_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a2a0L pe1_tgen_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tgen_on_off                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 tgen_initial_seed_mode             : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 tgen_table_sel                     : 4 ;  // 19:16     
} PE_H15A0_SR_SHP_TGEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800a2a4L pe1_tgen_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_delta_h_bound                   : 5 ;  //  4: 0     
	UINT32                                    : 4 ;  //  8: 5     reserved
	UINT32 tg_delta_l_bound                   : 4 ;  // 12: 9     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_delta_max                       : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 tg_delta_scale                     : 3 ;  // 26:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800a2a8L pe1_tgen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_rnd_th                          : 8 ;  //  7: 0     
	UINT32 tg_delta_wcurr                     : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 tg_lc_ldr_th                       : 6 ;  // 21:16     
} PE_H15A0_SR_SHP_TGEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800a2acL pe1_tgen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rand_x0                            : 32;  // 31: 0     
} PE_H15A0_SR_SHP_TGEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800a2b0L pe1_tgen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_dr_ratio                        : 3 ;  //  2: 0     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 tg_dr_th                           : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 tg_mm_offset                       : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_mm_sel_mode                     : 2 ;  // 25:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800a2b4L pe1_tgen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_lavg_th                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 tg_gs_rnd                          : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_gs_mm                           : 5 ;  // 28:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800a2b8L pe1_tgen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_x2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_x1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_x0                    : 6 ;  // 29:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800a2bcL pe1_tgen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_y2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_y1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_y0                    : 6 ;  // 29:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800a2c0L pe1_tgen_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_map_edge_gain                   : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 tg_map_detail_gain                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_map_offset                      : 8 ;  // 23:16     
	UINT32 tg_gs_noise                        : 6 ;  // 29:24     
} PE_H15A0_SR_SHP_TGEN_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800a2c4L pe1_tgen_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_gain7                           : 4 ;  //  3: 0     
	UINT32 tg_gain6                           : 4 ;  //  7: 4     
	UINT32 tg_gain5                           : 4 ;  // 11: 8     
	UINT32 tg_gain4                           : 4 ;  // 15:12     
	UINT32 tg_gain3                           : 4 ;  // 19:16     
	UINT32 tg_gain2                           : 4 ;  // 23:20     
	UINT32 tg_gain1                           : 4 ;  // 27:24     
	UINT32 tg_gain0                           : 4 ;  // 31:28     
} PE_H15A0_SR_SHP_TGEN_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800a800L sr_gsc_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_source_sel                     : 1 ;  //     0     
	UINT32                                    : 2 ;  //  2: 1     reserved
	UINT32 reg_vsc_mode_m1                    : 1 ;  //     3     
	UINT32 reg_adaptive_on_m1                 : 1 ;  //     4     
	UINT32 reg_bilinear_on_m1                 : 1 ;  //     5     
	UINT32 reg_th                             : 4 ;  //  9: 6     
	UINT32 blending_range                     : 2 ;  // 11:10     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 reg_min_max_on_m1                  : 1 ;  //    14     
	UINT32 reg_min_max_th_m1                  : 10;  // 24:15     
	UINT32                                    : 6 ;  // 30:25     reserved
	UINT32 reg_sampling_mode_m1               : 1 ;  //    31     
} PE_H15A0_SR_GSC_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800a808L sr_gsc_y_outsize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hosize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vosize_m1                      : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_OUTSIZE_T;

/*-----------------------------------------------------------------------------
		0xc800a818L sr_gsc_y_insize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hisize_m1                      : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_visize_m1                      : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_INSIZE_T;

/*-----------------------------------------------------------------------------
		0xc800a820L sr_gsc_y_phase_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_phase_off_v_m1                 : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_phase_off_h_m1                 : 9 ;  // 24:16     
} PE_H15A0_SR_GSC_Y_PHASE_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc800a824L sr_gsc_y_sampling_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_h_m1               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_numerator_h_m1                 : 13;  // 28:16     
} PE_H15A0_SR_GSC_Y_SAMPLING_H_T;

/*-----------------------------------------------------------------------------
		0xc800a828L sr_gsc_y_sampling_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denominator_v_m1               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_numerator_v_m1                 : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_SAMPLING_V_T;

/*-----------------------------------------------------------------------------
		0xc800a844L sr_gsc_y_v1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_SR_GSC_Y_V1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800a848L sr_gsc_y_v1_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800a84cL sr_gsc_y_v1_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800a850L sr_gsc_y_v1_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc800a854L sr_gsc_y_v1_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc800a858L sr_gsc_y_v1_y_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800a85cL sr_gsc_y_v1_y_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc800a860L sr_gsc_y_v2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_SR_GSC_Y_V2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800a864L sr_gsc_y_v2_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800a868L sr_gsc_y_v2_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800a86cL sr_gsc_y_v2_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc800a870L sr_gsc_y_v2_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc800a874L sr_gsc_y_h1_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_SR_GSC_Y_H1_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800a878L sr_gsc_y_h1_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800a87cL sr_gsc_y_h1_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800a880L sr_gsc_y_h1_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc800a884L sr_gsc_y_h1_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc800a888L sr_gsc_y_h1_y_fir_coef_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc800a88cL sr_gsc_y_h1_y_fir_coef_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc800a890L sr_gsc_y_h2_fir_coef_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update                             : 1 ;  //     0     
	UINT32 rnw                                : 1 ;  //     1     
	UINT32 addr                               : 8 ;  //  9: 2     
} PE_H15A0_SR_GSC_Y_H2_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800a894L sr_gsc_y_h2_y_fir_coef_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc800a898L sr_gsc_y_h2_y_fir_coef_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc800a89cL sr_gsc_y_h2_y_fir_coef_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc800a8a0L sr_gsc_y_h2_y_fir_coef_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA3_T;

typedef struct {
	PE_H15A0_FT_GRAD_SM_T                 	ft_grad_sm;                   //0xc8008000L
	PE_H15A0_FT_LOCALMM_VALUE_SCALE_T     	ft_localmm_value_scale;       //0xc8008004L
	PE_H15A0_FT_LOCALMM_TH_T              	ft_localmm_th;                //0xc8008008L
	PE_H15A0_FT_LOCALMM_SH_T              	ft_localmm_sh;                //0xc800800cL
	PE_H15A0_FT_LTV_SH_T                  	ft_ltv_sh;                    //0xc8008010L
	PE_H15A0_FT_LTV_TH_DIFF_T             	ft_ltv_th_diff;               //0xc8008014L
	PE_H15A0_FT_SHARP_SM_T                	ft_sharp_sm;                  //0xc8008018L
	PE_H15A0_FT_SHARP_TH_T                	ft_sharp_th;                  //0xc800801cL
	PE_H15A0_FT_SHARP_SCALE_SH_T          	ft_sharp_scale_sh;            //0xc8008020L
	PE_H15A0_FT_SHARP_CTRL_WE_T           	ft_sharp_ctrl_we;             //0xc8008024L
	UINT32                             	    reserved00[22];               //0xc8008028L~0xc800807cL
	PE_H15A0_CORE1_IU_EN_LTV_TH_T         	core1_iu_en_ltv_th;           //0xc8008080L
	PE_H15A0_CORE1_IU_EN_LTV_GRDAVG_TH_T  	core1_iu_en_ltv_grdavg_th;    //0xc8008084L
	PE_H15A0_CORE1_IU_EN_GNI_TH0_T        	core1_iu_en_gni_th0;          //0xc8008088L
	PE_H15A0_CORE1_IU_EN_GNI_TH1_T        	core1_iu_en_gni_th1;          //0xc800808cL
	PE_H15A0_CORE1_IU_EN_GNI_TH2_T        	core1_iu_en_gni_th2;          //0xc8008090L
	PE_H15A0_CORE1_IU_EN_GNI_TH3_T        	core1_iu_en_gni_th3;          //0xc8008094L
	PE_H15A0_CORE1_IU_EN_GRD_MUL_T        	core1_iu_en_grd_mul;          //0xc8008098L
	UINT32                             	    reserved01[25];               //0xc800809cL~0xc80080fcL
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG0_T    	core1_lut_id_um_grdavg0;      //0xc8008100L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG1_T    	core1_lut_id_um_grdavg1;      //0xc8008104L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG2_T    	core1_lut_id_um_grdavg2;      //0xc8008108L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG3_T    	core1_lut_id_um_grdavg3;      //0xc800810cL
	PE_H15A0_CORE1_WE_DH_MINMAX_T         	core1_we_dh_minmax;           //0xc8008110L
	PE_H15A0_CORE1_WE_DH_GRD_COST_WEI_T   	core1_we_dh_grd_cost_wei;     //0xc8008114L
	PE_H15A0_CORE1_WE_DH_NOISE_TH1_T      	core1_we_dh_noise_th1;        //0xc8008118L
	PE_H15A0_CORE1_WE_DH_NOISE_TH2_T      	core1_we_dh_noise_th2;        //0xc800811cL
	PE_H15A0_CORE1_WE_DH_NOISE_STR_T      	core1_we_dh_noise_str;        //0xc8008120L
	PE_H15A0_CORE1_WE_DH_LTV_TH_T         	core1_we_dh_ltv_th;           //0xc8008124L
	PE_H15A0_CORE1_WE_DH_LTV_GRD_TH_T     	core1_we_dh_ltv_grd_th;       //0xc8008128L
	PE_H15A0_CORE1_WE_DH_LTV_GRD_STR_T    	core1_we_dh_ltv_grd_str;      //0xc800812cL
	UINT32                             	    reserved02[4];                //0xc8008130L~0xc800813cL
	PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH1_T   	core1_iu_um_clip_ltv_th1;     //0xc8008140L
	PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH2_T   	core1_iu_um_clip_ltv_th2;     //0xc8008144L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG0_T    	core1_lut_iu_um_grdavg0;      //0xc8008148L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG1_T    	core1_lut_iu_um_grdavg1;      //0xc800814cL
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG2_T    	core1_lut_iu_um_grdavg2;      //0xc8008150L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG3_T    	core1_lut_iu_um_grdavg3;      //0xc8008154L
	PE_H15A0_CORE1_WE_UH_MINMAX_T         	core1_we_uh_minmax;           //0xc8008158L
	PE_H15A0_CORE1_WE_UH_GRD_COST_WEI_T   	core1_we_uh_grd_cost_wei;     //0xc800815cL
	PE_H15A0_CORE1_WE_UH_NOISE_TH1_T      	core1_we_uh_noise_th1;        //0xc8008160L
	PE_H15A0_CORE1_WE_UH_NOISE_TH2_T      	core1_we_uh_noise_th2;        //0xc8008164L
	PE_H15A0_CORE1_WE_UH_NOISE_STR_T      	core1_we_uh_noise_str;        //0xc8008168L
	PE_H15A0_CORE1_WE_UH_LTV_TH_T         	core1_we_uh_ltv_th;           //0xc800816cL
	PE_H15A0_CORE1_WE_UH_LTV_GRD_TH_T     	core1_we_uh_ltv_grd_th;       //0xc8008170L
	PE_H15A0_CORE1_WE_UH_LTV_GRD_STR_T    	core1_we_uh_ltv_grd_str;      //0xc8008174L
	UINT32                             	    reserved03[10];               //0xc8008178L~0xc800819cL
	PE_H15A0_CORE1_WE_JONE_BASE_MUL_T     	core1_we_jone_base_mul;       //0xc80081a0L
	PE_H15A0_CORE1_WE_JONE_SS_SHIFT_T     	core1_we_jone_ss_shift;       //0xc80081a4L
	PE_H15A0_CORE1_WE_JONE_SS_R_T         	core1_we_jone_ss_r;           //0xc80081a8L
	PE_H15A0_CORE1_WE_JONE_MM_TH_T        	core1_we_jone_mm_th;          //0xc80081acL
	PE_H15A0_CORE1_WE_JONE_MINMAX_T       	core1_we_jone_minmax;         //0xc80081b0L
	PE_H15A0_CORE1_WE_JONE_BASE_T         	core1_we_jone_base;           //0xc80081b4L
	PE_H15A0_CORE1_WE_JONE_GRDAVG_TH1_T   	core1_we_jone_grdavg_th1;     //0xc80081b8L
	PE_H15A0_CORE1_WE_JONE_GRDAVG_TH2_T   	core1_we_jone_grdavg_th2;     //0xc80081bcL
	PE_H15A0_CORE1_WE_JONE_D_JO_GRDAVG_T  	core1_we_jone_d_jo_grdavg;    //0xc80081c0L
	PE_H15A0_CORE1_WE_JONE_R_GRD_MUL1_T   	core1_we_jone_r_grd_mul1;     //0xc80081c4L
	PE_H15A0_CORE1_WE_JONE_R_GRD_MUL2_T   	core1_we_jone_r_grd_mul2;     //0xc80081c8L
	PE_H15A0_CORE1_WE_JONE_R_GRD_OFFSET_T 	core1_we_jone_r_grd_offset;   //0xc80081ccL
	PE_H15A0_CORE1_WE_JONE_D_RGR1_T       	core1_we_jone_d_rgr1;         //0xc80081d0L
	PE_H15A0_CORE1_WE_JONE_D_RGR2_T       	core1_we_jone_d_rgr2;         //0xc80081d4L
	UINT32                             	    reserved04[10];               //0xc80081d8L~0xc80081fcL
	PE_H15A0_CORE1_WE_ID_LQ_GRD_TH_T      	core1_we_id_lq_grd_th;        //0xc8008200L
	PE_H15A0_CORE1_WE_ID_LQ_GRD_RATIO_T   	core1_we_id_lq_grd_ratio;     //0xc8008204L
	PE_H15A0_CORE1_WE_ID_LQ_LTV_MIN_MAX_T 	core1_we_id_lq_ltv_min_max;   //0xc8008208L
	PE_H15A0_CORE1_WE_ID_LQ_LTV_MUL_T     	core1_we_id_lq_ltv_mul;       //0xc800820cL
	PE_H15A0_CORE1_WE_ID_LQ_GRDAVG_CLIP_T 	core1_we_id_lq_grdavg_clip;   //0xc8008210L
	PE_H15A0_CORE1_WE_ID_WHQ_BASE_T       	core1_we_id_whq_base;         //0xc8008214L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL0_T 	core1_we_id_hq_grdavg_mul0;   //0xc8008218L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH1_T     	core1_we_id_hq_ltv_th1;       //0xc800821cL
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH2_T     	core1_we_id_hq_ltv_th2;       //0xc8008220L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_GRD_TH_T  	core1_we_id_hq_ltv_grd_th;    //0xc8008224L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD1_T	core1_we_id_m_whq_ltv1_grd1;  //0xc8008228L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD2_T	core1_we_id_m_whq_ltv1_grd2;  //0xc800822cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV2_GRD_T 	core1_we_id_m_whq_ltv2_grd;   //0xc8008230L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV3_GRD_T 	core1_we_id_m_whq_ltv3_grd;   //0xc8008234L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH1_T	core1_we_id_hq_ltv_th_high1;  //0xc8008238L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH2_T	core1_we_id_hq_ltv_th_high2;  //0xc800823cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV_HIGH_T 	core1_we_id_m_whq_ltv_high;   //0xc8008240L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH1_T  	core1_we_id_hq_grdavg_th1;    //0xc8008244L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH2_T  	core1_we_id_hq_grdavg_th2;    //0xc8008248L
	PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG1_T  	core1_we_id_m_whq_grdavg1;    //0xc800824cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG2_T  	core1_we_id_m_whq_grdavg2;    //0xc8008250L
	PE_H15A0_CORE1_WE_ID_HQ_SS_TH1_T      	core1_we_id_hq_ss_th1;        //0xc8008254L
	PE_H15A0_CORE1_WE_ID_HQ_SS_TH2_T      	core1_we_id_hq_ss_th2;        //0xc8008258L
	PE_H15A0_CORE1_WE_ID_M_WHQ_SS1_T      	core1_we_id_m_whq_ss1;        //0xc800825cL
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL1_T 	core1_we_id_hq_grdavg_mul1;   //0xc8008260L
	PE_H15A0_CORE1_WE_ID_RATIO_HQ_T       	core1_we_id_ratio_hq;         //0xc8008264L
	PE_H15A0_CORE1_WE_ID_LTV_1B_T         	core1_we_id_ltv_1b;           //0xc8008268L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_LTH_T     	core1_we_id_hq_ltv_lth;       //0xc800826cL
	PE_H15A0_CORE1_WE_ID_HQ_LTV_HTH_T     	core1_we_id_hq_ltv_hth;       //0xc8008270L
	PE_H15A0_CORE1_WE_ID_HQ_GRD_TH_T      	core1_we_id_hq_grd_th;        //0xc8008274L
	PE_H15A0_CORE1_WE_ID_HQ_R_T           	core1_we_id_hq_r;             //0xc8008278L
	PE_H15A0_CORE1_WE_ID_HQ_LMM_TH_T      	core1_we_id_hq_lmm_th;        //0xc800827cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LMM_T      	core1_we_id_m_whq_lmm;        //0xc8008280L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH1_T	core1_we_id_hq_low_grdavg_th1;//0xc8008284L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH2_T	core1_we_id_hq_low_grdavg_th2;//0xc8008288L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_T    	core1_we_id_hq_low_rgrd;      //0xc800828cL
	PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_OFFSET_T	core1_we_id_hq_low_rgrd_offset;//0xc8008290L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LOW_RGRD_T 	core1_we_id_m_whq_low_rgrd;   //0xc8008294L
	PE_H15A0_CORE1_WE_MAT_FS_SS1_T        	core1_we_mat_fs_ss1;          //0xc8008298L
	PE_H15A0_CORE1_WE_MAT_FS_SS2_T        	core1_we_mat_fs_ss2;          //0xc800829cL
	PE_H15A0_CORE1_WE_MAT_FS_LTV1_T       	core1_we_mat_fs_ltv1;         //0xc80082a0L
	PE_H15A0_CORE1_WE_MAT_FS_LTV2_T       	core1_we_mat_fs_ltv2;         //0xc80082a4L
	PE_H15A0_CORE1_WE_MAT_T               	core1_we_mat;                 //0xc80082a8L
	PE_H15A0_CORE1_WE_MAT_C_T             	core1_we_mat_c;               //0xc80082acL
	UINT32                             	    reserved05[20];               //0xc80082b0L~0xc80082fcL
	PE_H15A0_PQ_CLASS_THRESH_MINMAX1_T    	pq_class_thresh_minmax1;      //0xc8008300L
	PE_H15A0_PQ_CLASS_THRESH_MINMAX2_T    	pq_class_thresh_minmax2;      //0xc8008304L
	PE_H15A0_PQ_CLASS_THRESH_BLUR1_T      	pq_class_thresh_blur1;        //0xc8008308L
	PE_H15A0_PQ_CLASS_THRESH_BLUR2_T      	pq_class_thresh_blur2;        //0xc800830cL
	PE_H15A0_PQ_CLASS_THRESH_BLUR3_T      	pq_class_thresh_blur3;        //0xc8008310L
	PE_H15A0_PQ_CLASS_THRESH_BLUR4_T      	pq_class_thresh_blur4;        //0xc8008314L
	PE_H15A0_PQ_CLASS_THRESH_GRAD1_T      	pq_class_thresh_grad1;        //0xc8008318L
	PE_H15A0_PQ_CLASS_THRESH_GRAD2_T      	pq_class_thresh_grad2;        //0xc800831cL
	PE_H15A0_PQ_CLASS_THRESH_GRAD3_T      	pq_class_thresh_grad3;        //0xc8008320L
	PE_H15A0_PQ_CLASS_THRESH_GRAD4_T      	pq_class_thresh_grad4;        //0xc8008324L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD1_T  	pq_class_thresh_diffgrad1;    //0xc8008328L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD2_T  	pq_class_thresh_diffgrad2;    //0xc800832cL
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD3_T  	pq_class_thresh_diffgrad3;    //0xc8008330L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD4_T  	pq_class_thresh_diffgrad4;    //0xc8008334L
	PE_H15A0_PQ_CLASS_THRESH_SS0_T        	pq_class_thresh_ss0;          //0xc8008338L
	PE_H15A0_PQ_CLASS_THRESH_SS1_T        	pq_class_thresh_ss1;          //0xc800833cL
	PE_H15A0_PQ_CLASS_THRESH_SS2_T        	pq_class_thresh_ss2;          //0xc8008340L
	PE_H15A0_PQ_CLASS_THRESH_SS3_T        	pq_class_thresh_ss3;          //0xc8008344L
	PE_H15A0_PQ_MINMAX_CNT_0_T            	pq_minmax_cnt_0;              //0xc8008348L
	PE_H15A0_PQ_MINMAX_CNT_1_T            	pq_minmax_cnt_1;              //0xc800834cL
	PE_H15A0_PQ_MINMAX_CNT_2_T            	pq_minmax_cnt_2;              //0xc8008350L
	PE_H15A0_PQ_MINMAX_CNT_3_T            	pq_minmax_cnt_3;              //0xc8008354L
	PE_H15A0_PQ_SHARP_CNT_0_T             	pq_sharp_cnt_0;               //0xc8008358L
	PE_H15A0_PQ_SHARP_CNT_1_T             	pq_sharp_cnt_1;               //0xc800835cL
	PE_H15A0_PQ_SHARP_CNT_2_T             	pq_sharp_cnt_2;               //0xc8008360L
	PE_H15A0_PQ_SHARP_CNT_3_T             	pq_sharp_cnt_3;               //0xc8008364L
	PE_H15A0_PQ_SHARP_CNT_4_T             	pq_sharp_cnt_4;               //0xc8008368L
	PE_H15A0_PQ_SHARP_CNT_5_T             	pq_sharp_cnt_5;               //0xc800836cL
	PE_H15A0_PQ_SHARP_CNT_6_T             	pq_sharp_cnt_6;               //0xc8008370L
	PE_H15A0_PQ_SHARP_CNT_7_T             	pq_sharp_cnt_7;               //0xc8008374L
	PE_H15A0_PQ_GRAD_CNT_0_T              	pq_grad_cnt_0;                //0xc8008378L
	PE_H15A0_PQ_GRAD_CNT_1_T              	pq_grad_cnt_1;                //0xc800837cL
	PE_H15A0_PQ_GRAD_CNT_2_T              	pq_grad_cnt_2;                //0xc8008380L
	PE_H15A0_PQ_GRAD_CNT_3_T              	pq_grad_cnt_3;                //0xc8008384L
	PE_H15A0_PQ_GRAD_CNT_4_T              	pq_grad_cnt_4;                //0xc8008388L
	PE_H15A0_PQ_GRAD_CNT_5_T              	pq_grad_cnt_5;                //0xc800838cL
	PE_H15A0_PQ_GRAD_CNT_6_T              	pq_grad_cnt_6;                //0xc8008390L
	PE_H15A0_PQ_GRAD_CNT_7_T              	pq_grad_cnt_7;                //0xc8008394L
	PE_H15A0_PQ_AV_GRAD_CNT_0_T           	pq_av_grad_cnt_0;             //0xc8008398L
	PE_H15A0_PQ_AV_GRAD_CNT_1_T           	pq_av_grad_cnt_1;             //0xc800839cL
	PE_H15A0_PQ_AV_GRAD_CNT_2_T           	pq_av_grad_cnt_2;             //0xc80083a0L
	PE_H15A0_PQ_AV_GRAD_CNT_3_T           	pq_av_grad_cnt_3;             //0xc80083a4L
	PE_H15A0_PQ_AV_GRAD_CNT_4_T           	pq_av_grad_cnt_4;             //0xc80083a8L
	PE_H15A0_PQ_AV_GRAD_CNT_5_T           	pq_av_grad_cnt_5;             //0xc80083acL
	PE_H15A0_PQ_AV_GRAD_CNT_6_T           	pq_av_grad_cnt_6;             //0xc80083b0L
	PE_H15A0_PQ_AV_GRAD_CNT_7_T           	pq_av_grad_cnt_7;             //0xc80083b4L
	PE_H15A0_PQ_DIFF_GRAD_CNT_0_T         	pq_diff_grad_cnt_0;           //0xc80083b8L
	PE_H15A0_PQ_DIFF_GRAD_CNT_1_T         	pq_diff_grad_cnt_1;           //0xc80083bcL
	PE_H15A0_PQ_DIFF_GRAD_CNT_2_T         	pq_diff_grad_cnt_2;           //0xc80083c0L
	PE_H15A0_PQ_DIFF_GRAD_CNT_3_T         	pq_diff_grad_cnt_3;           //0xc80083c4L
	PE_H15A0_PQ_DIFF_GRAD_CNT_4_T         	pq_diff_grad_cnt_4;           //0xc80083c8L
	PE_H15A0_PQ_DIFF_GRAD_CNT_5_T         	pq_diff_grad_cnt_5;           //0xc80083ccL
	PE_H15A0_PQ_DIFF_GRAD_CNT_6_T         	pq_diff_grad_cnt_6;           //0xc80083d0L
	PE_H15A0_PQ_DIFF_GRAD_CNT_7_T         	pq_diff_grad_cnt_7;           //0xc80083d4L
	PE_H15A0_PQ_BLUR_CNT_0_T              	pq_blur_cnt_0;                //0xc80083d8L
	PE_H15A0_PQ_BLUR_CNT_1_T              	pq_blur_cnt_1;                //0xc80083dcL
	PE_H15A0_PQ_BLUR_CNT_2_T              	pq_blur_cnt_2;                //0xc80083e0L
	PE_H15A0_PQ_BLUR_CNT_3_T              	pq_blur_cnt_3;                //0xc80083e4L
	PE_H15A0_PQ_BLUR_CNT_4_T              	pq_blur_cnt_4;                //0xc80083e8L
	PE_H15A0_PQ_BLUR_CNT_5_T              	pq_blur_cnt_5;                //0xc80083ecL
	PE_H15A0_PQ_BLUR_CNT_6_T              	pq_blur_cnt_6;                //0xc80083f0L
	PE_H15A0_PQ_BLUR_CNT_7_T              	pq_blur_cnt_7;                //0xc80083f4L
	PE_H15A0_PQ_TOTAL_MINMAX_ON_T         	pq_total_minmax_on;           //0xc80083f8L
	PE_H15A0_PQ_TOTAL_MINMAX_OFF_T        	pq_total_minmax_off;          //0xc80083fcL
	UINT32                             	    reserved06[64];               //0xc8008400L~0xc80084fcL
	PE_H15A0_FT_UP_GRD_BL23_0_T           	ft_up_grd_bl23_0;             //0xc8008500L
	PE_H15A0_FT_UP_GRD_BL23_1_T           	ft_up_grd_bl23_1;             //0xc8008504L
	PE_H15A0_FT_UP_GRDAVG_BL23_0_T        	ft_up_grdavg_bl23_0;          //0xc8008508L
	PE_H15A0_FT_UP_GRDAVG_BL23_1_T        	ft_up_grdavg_bl23_1;          //0xc800850cL
	PE_H15A0_FT_UP_SS_BL23_0_T            	ft_up_ss_bl23_0;              //0xc8008510L
	PE_H15A0_FT_UP_SS_BL23_1_T            	ft_up_ss_bl23_1;              //0xc8008514L
	PE_H15A0_FT_UP_LMM3_BL23_0_T          	ft_up_lmm3_bl23_0;            //0xc8008518L
	PE_H15A0_FT_UP_LMM3_BL23_1_T          	ft_up_lmm3_bl23_1;            //0xc800851cL
	PE_H15A0_FT_UP_LTV_BL23_0_T           	ft_up_ltv_bl23_0;             //0xc8008520L
	PE_H15A0_FT_UP_LTV_BL23_1_T           	ft_up_ltv_bl23_1;             //0xc8008524L
	#ifdef PE_SR_REG_H15A0_NEED_TO_REMAP
	UINT32                             	    reserved07[182];              //0xc8008528L~0xc80087fcL
	PE_H15A0_FT_EXT_SIZE_T                	ft_ext_size;                  //0xc8008800L
	PE_H15A0_FT_DONE_PRD_T                	ft_done_prd;                  //0xc8008804L
	UINT32                             	    reserved08[62];               //0xc8008808L~0xc80088fcL
	PE_H15A0_F1_YMEM_SW_RST_T             	f1_ymem_sw_rst;               //0xc8008900L
	PE_H15A0_F1_YMEM_VSYN_DELAY_T         	f1_ymem_vsyn_delay;           //0xc8008904L
	PE_H15A0_F1_YMEM_LINE_DURATION_T      	f1_ymem_line_duration;        //0xc8008908L
	PE_H15A0_F1_YMEM_LINE_DURATION2_T     	f1_ymem_line_duration2;       //0xc800890cL
	PE_H15A0_F1_YMEM_OFFSET_T             	f1_ymem_offset;               //0xc8008910L
	PE_H15A0_F1_YMEM_LINE_COUNT0_T        	f1_ymem_line_count0;          //0xc8008914L
	PE_H15A0_F1_YMEM_LINE_COUNT1_T        	f1_ymem_line_count1;          //0xc8008918L
	PE_H15A0_F1_YMEM_LINE_COUNT2_T        	f1_ymem_line_count2;          //0xc800891cL
	PE_H15A0_F1_YMEM_SR1_DELAY_T          	f1_ymem_sr1_delay;            //0xc8008920L
	PE_H15A0_F1_ENGINE_REV_T              	f1_engine_rev;                //0xc8008924L
	#endif
}PE_SR_F1_REG_H15A0_T;

typedef struct {
	PE_H15A0_CORE1_SCALER_COEF0_T         	core1_scaler_coef0;           //0xc8009000L
	PE_H15A0_CORE1_SCALER_COEF1_T         	core1_scaler_coef1;           //0xc8009004L
	PE_H15A0_CORE1_SCALER_COEF2_T         	core1_scaler_coef2;           //0xc8009008L
	PE_H15A0_CORE1_SCALER_COEF3_T         	core1_scaler_coef3;           //0xc800900cL
	PE_H15A0_CORE1_IU_BLUR_HCOEFF1_T      	core1_iu_blur_hcoeff1;        //0xc8009010L
	PE_H15A0_CORE1_IU_BLUR_HCOEFF2_T      	core1_iu_blur_hcoeff2;        //0xc8009014L
	PE_H15A0_CORE1_IU_BLUR_HCOEFF3_T      	core1_iu_blur_hcoeff3;        //0xc8009018L
	PE_H15A0_CORE1_IU_BLUR_VCOEFF1_T      	core1_iu_blur_vcoeff1;        //0xc800901cL
	PE_H15A0_CORE1_IU_BLUR_VCOEFF2_T      	core1_iu_blur_vcoeff2;        //0xc8009020L
	PE_H15A0_CORE1_IU_UM_CLIP_EN_T        	core1_iu_um_clip_en;          //0xc8009024L
	PE_H15A0_CORE1_IU_UM_R_CLIP_LV1_T     	core1_iu_um_r_clip_lv1;       //0xc8009028L
	PE_H15A0_CORE1_IU_UM_R_CLIP_LV2_T     	core1_iu_um_r_clip_lv2;       //0xc800902cL
	PE_H15A0_CORE1_ID_BLUR_HCOEFF_T       	core1_id_blur_hcoeff;         //0xc8009030L
	PE_H15A0_CORE1_ID_BLUR_VCOEFF_T       	core1_id_blur_vcoeff;         //0xc8009034L
	PE_H15A0_CORE1_IU_DELTA_CROSS0_T      	core1_iu_delta_cross0;        //0xc8009038L
	PE_H15A0_CORE1_IU_DELTA_CROSS1_T      	core1_iu_delta_cross1;        //0xc800903cL
	PE_H15A0_CORE1_IU_DELTA_DIAL0_T       	core1_iu_delta_dial0;         //0xc8009040L
	PE_H15A0_CORE1_IU_DELTA_DIAL1_T       	core1_iu_delta_dial1;         //0xc8009044L
	PE_H15A0_CORE1_LUE_AVG_TH_T           	core1_lue_avg_th;             //0xc8009048L
	PE_H15A0_CORE1_USE_IU_EN_T            	core1_use_iu_en;              //0xc800904cL
	PE_H15A0_CORE1_IU_EN_4MM_MIN_TH_T     	core1_iu_en_4mm_min_th;       //0xc8009050L
	PE_H15A0_CORE1_WE_ID_0_T              	core1_we_id_0;                //0xc8009054L
	PE_H15A0_CORE1_WE_ID_1_T              	core1_we_id_1;                //0xc8009058L
	PE_H15A0_CORE1_FUSE_GRD_TH_T          	core1_fuse_grd_th;            //0xc800905cL
	PE_H15A0_CORE1_FUSE_GRD_R1_T          	core1_fuse_grd_r1;            //0xc8009060L
	PE_H15A0_CORE1_FUSE_GRD_R2_T          	core1_fuse_grd_r2;            //0xc8009064L
	PE_H15A0_CORE1_FUSE_WE_SC1_T          	core1_fuse_we_sc1;            //0xc8009068L
	PE_H15A0_CORE1_FUSE_WE_SC2_T          	core1_fuse_we_sc2;            //0xc800906cL
	PE_H15A0_CORE1_FUSE_R_GRD_IUID_T      	core1_fuse_r_grd_iuid;        //0xc8009070L
	PE_H15A0_CORE1_FUSE_DIFF_IU_ID_UM_TH_T	core1_fuse_diff_iu_id_um_th;  //0xc8009074L
	PE_H15A0_CORE1_FUSE_WE_DIFF_IUID_1_T  	core1_fuse_we_diff_iuid_1;    //0xc8009078L
	PE_H15A0_CORE1_FUSE_WE_DIFF_IUID_2_T  	core1_fuse_we_diff_iuid_2;    //0xc800907cL
	PE_H15A0_CORE1_USE_JTWO_T             	core1_use_jtwo;               //0xc8009080L
	PE_H15A0_CORE1_J2W_COST_TH_T          	core1_j2w_cost_th;            //0xc8009084L
	PE_H15A0_CORE1_J2W_COST_SUB_RATIO_T   	core1_j2w_cost_sub_ratio;     //0xc8009088L
	PE_H15A0_CORE1_3D_T                   	core1_3d;                     //0xc800908cL
	PE_H15A0_DEBUG_CTRL_T                 	debug_ctrl;                   //0xc8009090L
	PE_H15A0_S1_DEMO_CTRL_T               	s1_demo_ctrl;                 //0xc8009094L
	PE_H15A0_S1_DEMO_POS_X_T              	s1_demo_pos_x;                //0xc8009098L
	#ifdef PE_SR_REG_H15A0_NEED_TO_REMAP
	UINT32                             	    reserved00[89];               //0xc800909cL~0xc80091fcL
	PE_H15A0_CORE1_SCALER_SIZE_T          	core1_scaler_size;            //0xc8009200L
	PE_H15A0_CORE1_EMP_ENH5X5_SIZE_T      	core1_emp_enh5x5_size;        //0xc8009204L
	PE_H15A0_CORE1_ENH3X3_LM11_SIZE_T     	core1_enh3x3_lm11_size;       //0xc8009208L
	PE_H15A0_CORE1_OUT_LM23_SIZE_T        	core1_out_lm23_size;          //0xc800920cL
	PE_H15A0_CORE1_MD_PROC_BLK_T          	core1_md_proc_blk;            //0xc8009210L
	PE_H15A0_CORE1_MU_PROC_BLK_T          	core1_mu_proc_blk;            //0xc8009214L
	PE_H15A0_CORE1_MF_PROC_BLK_T          	core1_mf_proc_blk;            //0xc8009218L
	UINT32                             	    reserved01[121];              //0xc800921cL~0xc80093fcL
	PE_H15A0_DVOM_DISP_SIZE_T             	dvom_disp_size;               //0xc8009400L
	PE_H15A0_DVO_PG_PAT_CTRL_T            	dvo_pg_pat_ctrl;              //0xc8009404L
	PE_H15A0_DVO_PG_SYNC_CTRL0_T          	dvo_pg_sync_ctrl0;            //0xc8009408L
	PE_H15A0_DVOM_SYNC0_T                 	dvom_sync0;                   //0xc800940cL
	PE_H15A0_DVOM_SYNC1_T                 	dvom_sync1;                   //0xc8009410L
	PE_H15A0_DVOM_SYNC2_T                 	dvom_sync2;                   //0xc8009414L
	PE_H15A0_DVOM_SYNC3_T                 	dvom_sync3;                   //0xc8009418L
	UINT32                             	    reserved02[57];               //0xc800941cL~0xc80094fcL
	PE_H15A0_S1_YMEM_SW_RST_T             	s1_ymem_sw_rst;               //0xc8009500L
	PE_H15A0_S1_YMEM_PINIT_DELAY_T        	s1_ymem_pinit_delay;          //0xc8009504L
	PE_H15A0_S1_YMEM_VSYN_DELAY_T         	s1_ymem_vsyn_delay;           //0xc8009508L
	PE_H15A0_S1_YMEM_LINE_DURATION_T      	s1_ymem_line_duration;        //0xc800950cL
	PE_H15A0_S1_YMEM_LINE_DURATION2_T     	s1_ymem_line_duration2;       //0xc8009510L
	PE_H15A0_S1_YMEM_OFFSET0_T            	s1_ymem_offset0;              //0xc8009514L
	PE_H15A0_S1_YMEM_OFFSET1_T            	s1_ymem_offset1;              //0xc8009518L
	PE_H15A0_S1_YMEM_OFFSET2_T            	s1_ymem_offset2;              //0xc800951cL
	PE_H15A0_S1_YMEM_OFFSET3_T            	s1_ymem_offset3;              //0xc8009520L
	PE_H15A0_S1_YMEM_LINE_COUNT0_T        	s1_ymem_line_count0;          //0xc8009524L
	PE_H15A0_S1_YMEM_LINE_COUNT1_T        	s1_ymem_line_count1;          //0xc8009528L
	PE_H15A0_S1_YMEM_LINE_COUNT2_T        	s1_ymem_line_count2;          //0xc800952cL
	PE_H15A0_S1_YMEM_LINE_COUNT3_T        	s1_ymem_line_count3;          //0xc8009530L
	PE_H15A0_S1_YMEM_LINE_COUNT4_T        	s1_ymem_line_count4;          //0xc8009534L
	PE_H15A0_S1_YMEM_LINE_COUNT5_T        	s1_ymem_line_count5;          //0xc8009538L
	PE_H15A0_S1_YMEM_LINE_COUNT6_T        	s1_ymem_line_count6;          //0xc800953cL
	PE_H15A0_S1_YMEM_RESERVED0_T          	s1_ymem_reserved0;            //0xc8009540L
	PE_H15A0_S1_YMEM_RESERVED1_T          	s1_ymem_reserved1;            //0xc8009544L
	PE_H15A0_S1_YMEM_RESERVED2_T          	s1_ymem_reserved2;            //0xc8009548L
	PE_H15A0_S1_FB_INFO_T                 	s1_fb_info;                   //0xc800954cL
	PE_H15A0_S1_VS_RISE_INFO_T            	s1_vs_rise_info;              //0xc8009550L
	PE_H15A0_S1_DBG_DE_WIDTH_T            	s1_dbg_de_width;              //0xc8009554L
	PE_H15A0_S1_DBG_LINECNT_T             	s1_dbg_linecnt;               //0xc8009558L
	PE_H15A0_S1_DBG_LINE_TOTAL_CNT_T      	s1_dbg_line_total_cnt;        //0xc800955cL
	PE_H15A0_S1_INTR_IN0_T                	s1_intr_in0;                  //0xc8009560L
	PE_H15A0_S1_INTR_IN1_T                	s1_intr_in1;                  //0xc8009564L
	PE_H15A0_S1_PROC_LINE_INFO_T          	s1_proc_line_info;            //0xc8009568L
	PE_H15A0_S1_MON_SEL_T                 	s1_mon_sel;                   //0xc800956cL
	PE_H15A0_S1_ENGINE_REV_T              	s1_engine_rev;                //0xc8009570L
	#endif
}PE_SR_S1_REG_H15A0_T;

typedef struct {
	PE_H15A0_SR_SHP_TOP_CTRL_00_T           pe1_top_ctrl_00;              //0xc800a000L
	PE_H15A0_SR_SHP_TOP_CTRL_01_T           pe1_top_ctrl_01;              //0xc800a004L
	PE_H15A0_SR_SHP_TOP_CTRL_02_T           pe1_top_ctrl_02;              //0xc800a008L
	PE_H15A0_SR_SHP_TOP_CTRL_03_T           pe1_top_ctrl_03;              //0xc800a00cL
	PE_H15A0_SR_SHP_TOP_CTRL_04_T           pe1_top_ctrl_04;              //0xc800a010L
	UINT32                             	    reserved00[3];                //0xc800a014L~0xc800a01cL
	PE_H15A0_SR_SHP_WIN1_CTRL_00_T          pe1_win1_ctrl_00;             //0xc800a020L
	PE_H15A0_SR_SHP_WIN1_CTRL_01_T          pe1_win1_ctrl_01;             //0xc800a024L
	PE_H15A0_SR_SHP_WIN1_CTRL_02_T          pe1_win1_ctrl_02;             //0xc800a028L
	PE_H15A0_SR_SHP_WIN1_CTRL_03_T          pe1_win1_ctrl_03;             //0xc800a02cL
	PE_H15A0_SR_SHP_WIN1_CTRL_04_T          pe1_win1_ctrl_04;             //0xc800a030L
	UINT32                             	    reserved01[3];                //0xc800a034L~0xc800a03cL
	PE_H15A0_SR_SHP_FSW_CTRL_00_T           pe1_fsw_ctrl_00;              //0xc800a040L
	PE_H15A0_SR_SHP_FSW_CTRL_01_T           pe1_fsw_ctrl_01;              //0xc800a044L
	PE_H15A0_SR_SHP_FSW_CTRL_02_T           pe1_fsw_ctrl_02;              //0xc800a048L
	PE_H15A0_SR_SHP_FSW_CTRL_03_T           pe1_fsw_ctrl_03;              //0xc800a04cL
	UINT32                             	    reserved02[4];                //0xc800a050L~0xc800a05cL
	PE_H15A0_DISP_SYNC_DLY_ADDR00_T       	disp_sync_dly_addr00;         //0xc800a060L
	PE_H15A0_DISP_SYNC_DLY_ADDR01_T       	disp_sync_dly_addr01;         //0xc800a064L
	PE_H15A0_DISP_SYNC_DLY_ADDR02_T       	disp_sync_dly_addr02;         //0xc800a068L
	PE_H15A0_DISP_SYNC_DLY_ADDR03_T       	disp_sync_dly_addr03;         //0xc800a06cL
	PE_H15A0_DISP_SYNC_DLY_ADDR04_T       	disp_sync_dly_addr04;         //0xc800a070L
	PE_H15A0_DISP_SYNC_DLY_ADDR05_T       	disp_sync_dly_addr05;         //0xc800a074L
	PE_H15A0_DISP_SYNC_DLY_ADDR06_T       	disp_sync_dly_addr06;         //0xc800a078L
	PE_H15A0_DISP_SYNC_DLY_ADDR07_T       	disp_sync_dly_addr07;         //0xc800a07cL
	PE_H15A0_DISP_SYNC_DLY_ADDR08_T       	disp_sync_dly_addr08;         //0xc800a080L
	PE_H15A0_DISP_SYNC_DLY_ADDR09_T       	disp_sync_dly_addr09;         //0xc800a084L
	PE_H15A0_DISP_SYNC_DLY_ADDR0A_T       	disp_sync_dly_addr0a;         //0xc800a088L
	UINT32                             	    reserved03;                   //0xc800a08cL
	PE_H15A0_SR_SHP_WIN3_CTRL_00_T          pe1_win3_ctrl_00;             //0xc800a090L
	PE_H15A0_SR_SHP_WIN3_CTRL_01_T          pe1_win3_ctrl_01;             //0xc800a094L
	PE_H15A0_SR_SHP_WIN3_CTRL_02_T          pe1_win3_ctrl_02;             //0xc800a098L
	PE_H15A0_SR_SHP_WIN3_CTRL_03_T          pe1_win3_ctrl_03;             //0xc800a09cL
	PE_H15A0_SR_SHP_WIN3_CTRL_04_T          pe1_win3_ctrl_04;             //0xc800a0a0L
	UINT32                             	    reserved04[3];                //0xc800a0a4L~0xc800a0acL
	PE_H15A0_SR_SHP_DERH_CTRL_00_T          pe1_derh_ctrl_00;             //0xc800a0b0L
	PE_H15A0_SR_SHP_DERH_CTRL_01_T          pe1_derh_ctrl_01;             //0xc800a0b4L
	PE_H15A0_SR_SHP_DERH_CTRL_02_T          pe1_derh_ctrl_02;             //0xc800a0b8L
	PE_H15A0_SR_SHP_DERH_CTRL_03_T          pe1_derh_ctrl_03;             //0xc800a0bcL
	PE_H15A0_SR_SHP_DERH_CTRL_04_T          pe1_derh_ctrl_04;             //0xc800a0c0L
	PE_H15A0_SR_SHP_DERH_CTRL_05_T          pe1_derh_ctrl_05;             //0xc800a0c4L
	PE_H15A0_SR_SHP_DERH_CTRL_06_T          pe1_derh_ctrl_06;             //0xc800a0c8L
	PE_H15A0_SR_SHP_DERH_CTRL_07_T          pe1_derh_ctrl_07;             //0xc800a0ccL
	PE_H15A0_SR_SHP_DERH_CTRL_08_T          pe1_derh_ctrl_08;             //0xc800a0d0L
	PE_H15A0_SR_SHP_DERH_CTRL_09_T          pe1_derh_ctrl_09;             //0xc800a0d4L
	PE_H15A0_SR_SHP_DERH_CTRL_0A_T          pe1_derh_ctrl_0a;             //0xc800a0d8L
	PE_H15A0_SR_SHP_DERH_CTRL_0B_T          pe1_derh_ctrl_0b;             //0xc800a0dcL
	PE_H15A0_SR_SHP_DERH_CTRL_0C_T          pe1_derh_ctrl_0c;             //0xc800a0e0L
	PE_H15A0_SR_SHP_PE1_CORING_CTRL_10_T    pe1_coring_ctrl_10;           //0xc800a0e4L
	UINT32                             	    reserved05[2];                //0xc800a0e8L~0xc800a0ecL
	PE_H15A0_SR_SHP_DERV_CTRL_0_T           pe1_derv_ctrl_0;              //0xc800a0f0L
	PE_H15A0_SR_SHP_DERV_CTRL_1_T           pe1_derv_ctrl_1;              //0xc800a0f4L
	PE_H15A0_SR_SHP_DERV_CTRL_2_T           pe1_derv_ctrl_2;              //0xc800a0f8L
	PE_H15A0_SR_SHP_DERV_CTRL_3_T           pe1_derv_ctrl_3;              //0xc800a0fcL
	UINT32                             	    reserved06[4];                //0xc800a100L~0xc800a10cL
	PE_H15A0_SR_SHP_DP_CTRL_00_T            pe1_dp_ctrl_00;               //0xc800a110L
	PE_H15A0_SR_SHP_DP_CTRL_01_T            pe1_dp_ctrl_01;               //0xc800a114L
	PE_H15A0_SR_SHP_DP_CTRL_02_T            pe1_dp_ctrl_02;               //0xc800a118L
	PE_H15A0_SR_SHP_DP_CTRL_03_T            pe1_dp_ctrl_03;               //0xc800a11cL
	UINT32                             	    reserved07[4];                //0xc800a120L~0xc800a12cL
	PE_H15A0_SR_SHP_SP_CTRL_00_T            pe1_sp_ctrl_00;               //0xc800a130L
	PE_H15A0_SR_SHP_SP_CTRL_01_T            pe1_sp_ctrl_01;               //0xc800a134L
	PE_H15A0_SR_SHP_SP_CTRL_02_T            pe1_sp_ctrl_02;               //0xc800a138L
	PE_H15A0_SR_SHP_SP_CTRL_03_T            pe1_sp_ctrl_03;               //0xc800a13cL
	PE_H15A0_SR_SHP_SP_CTRL_04_T            pe1_sp_ctrl_04;               //0xc800a140L
	PE_H15A0_SR_SHP_SP_CTRL_05_T            pe1_sp_ctrl_05;               //0xc800a144L
	PE_H15A0_SR_SHP_SP_CTRL_06_T            pe1_sp_ctrl_06;               //0xc800a148L
	PE_H15A0_SR_SHP_SP_CTRL_07_T            pe1_sp_ctrl_07;               //0xc800a14cL
	PE_H15A0_SR_SHP_SP_CTRL_08_T            pe1_sp_ctrl_08;               //0xc800a150L
	PE_H15A0_SR_SHP_SP_CTRL_09_T            pe1_sp_ctrl_09;               //0xc800a154L
	PE_H15A0_SR_SHP_SP_CTRL_0A_T            pe1_sp_ctrl_0a;               //0xc800a158L
	PE_H15A0_SR_SHP_SP_CTRL_0B_T            pe1_sp_ctrl_0b;               //0xc800a15cL
	PE_H15A0_SR_SHP_SP_CTRL_0C_T            pe1_sp_ctrl_0c;               //0xc800a160L
	UINT32                             	    reserved08[3];                //0xc800a164L~0xc800a16cL
	PE_H15A0_SR_SHP_MP_CTRL_00_T            pe1_mp_ctrl_00;               //0xc800a170L
	PE_H15A0_SR_SHP_MP_CTRL_01_T            pe1_mp_ctrl_01;               //0xc800a174L
	PE_H15A0_SR_SHP_MP_CTRL_02_T            pe1_mp_ctrl_02;               //0xc800a178L
	PE_H15A0_SR_SHP_MP_CTRL_03_T            pe1_mp_ctrl_03;               //0xc800a17cL
	PE_H15A0_SR_SHP_MP_CTRL_04_T            pe1_mp_ctrl_04;               //0xc800a180L
	PE_H15A0_SR_SHP_MP_CTRL_05_T            pe1_mp_ctrl_05;               //0xc800a184L
	PE_H15A0_SR_SHP_MP_CTRL_06_T            pe1_mp_ctrl_06;               //0xc800a188L
	PE_H15A0_SR_SHP_MP_CTRL_07_T            pe1_mp_ctrl_07;               //0xc800a18cL
	PE_H15A0_SR_SHP_MP_CTRL_08_T            pe1_mp_ctrl_08;               //0xc800a190L
	PE_H15A0_SR_SHP_MP_CTRL_09_T            pe1_mp_ctrl_09;               //0xc800a194L
	PE_H15A0_SR_SHP_MP_CTRL_0A_T            pe1_mp_ctrl_0a;               //0xc800a198L
	PE_H15A0_SR_SHP_MP_CTRL_0B_T            pe1_mp_ctrl_0b;               //0xc800a19cL
	PE_H15A0_SR_SHP_MP_CTRL_0C_T            pe1_mp_ctrl_0c;               //0xc800a1a0L
	UINT32                             	    reserved09[3];                //0xc800a1a4L~0xc800a1acL
	PE_H15A0_SR_SHP_CORING_CTRL_00_T        pe1_coring_ctrl_00;           //0xc800a1b0L
	PE_H15A0_SR_SHP_CORING_CTRL_01_T        pe1_coring_ctrl_01;           //0xc800a1b4L
	PE_H15A0_SR_SHP_CORING_CTRL_02_T        pe1_coring_ctrl_02;           //0xc800a1b8L
	PE_H15A0_SR_SHP_CORING_CTRL_03_T        pe1_coring_ctrl_03;           //0xc800a1bcL
	PE_H15A0_SR_SHP_CORING_CTRL_04_T        pe1_coring_ctrl_04;           //0xc800a1c0L
	PE_H15A0_SR_SHP_CORING_CTRL_05_T        pe1_coring_ctrl_05;           //0xc800a1c4L
	PE_H15A0_SR_SHP_CORING_CTRL_06_T        pe1_coring_ctrl_06;           //0xc800a1c8L
	PE_H15A0_SR_SHP_CORING_CTRL_07_T        pe1_coring_ctrl_07;           //0xc800a1ccL
	PE_H15A0_SR_SHP_CORING_CTRL_08_T        pe1_coring_ctrl_08;           //0xc800a1d0L
	PE_H15A0_SR_SHP_CORING_CTRL_09_T        pe1_coring_ctrl_09;           //0xc800a1d4L
	UINT32                             	    reserved10[2];                //0xc800a1d8L~0xc800a1dcL
	PE_H15A0_SR_SHP_CORING_STAT_00_T        pe1_coring_stat_00;           //0xc800a1e0L
	PE_H15A0_SR_SHP_CORING_STAT_01_T        pe1_coring_stat_01;           //0xc800a1e4L
	PE_H15A0_SR_SHP_CORING_STAT_02_T        pe1_coring_stat_02;           //0xc800a1e8L
	PE_H15A0_SR_SHP_CORING_STAT_03_T        pe1_coring_stat_03;           //0xc800a1ecL
	UINT32                             	    reserved11[4];                //0xc800a1f0L~0xc800a1fcL
	PE_H15A0_SR_SHP_PTI_CTRL_00_T           pe1_pti_ctrl_00;              //0xc800a200L
	PE_H15A0_SR_SHP_PTI_CTRL_01_T           pe1_pti_ctrl_01;              //0xc800a204L
	PE_H15A0_SR_SHP_STI_CTRL_00_T           pe1_sti_ctrl_00;              //0xc800a208L
	PE_H15A0_SR_SHP_STI_CTRL_01_T           pe1_sti_ctrl_01;              //0xc800a20cL
	UINT32                             	    reserved12[4];                //0xc800a210L~0xc800a21cL
	PE_H15A0_SR_SHP_DJ_CTRL_00_T            pe1_dj_ctrl_00;               //0xc800a220L
	PE_H15A0_SR_SHP_DJ_CTRL_01_T            pe1_dj_ctrl_01;               //0xc800a224L
	PE_H15A0_SR_SHP_DJ_CTRL_02_T            pe1_dj_ctrl_02;               //0xc800a228L
	PE_H15A0_SR_SHP_DJ_CTRL_03_T            pe1_dj_ctrl_03;               //0xc800a22cL
	UINT32                             	    reserved13[28];               //0xc800a230L~0xc800a29cL
	PE_H15A0_SR_SHP_TGEN_CTRL_00_T          pe1_tgen_ctrl_00;             //0xc800a2a0L
	PE_H15A0_SR_SHP_TGEN_CTRL_01_T          pe1_tgen_ctrl_01;             //0xc800a2a4L
	PE_H15A0_SR_SHP_TGEN_CTRL_02_T          pe1_tgen_ctrl_02;             //0xc800a2a8L
	PE_H15A0_SR_SHP_TGEN_CTRL_03_T          pe1_tgen_ctrl_03;             //0xc800a2acL
	PE_H15A0_SR_SHP_TGEN_CTRL_04_T          pe1_tgen_ctrl_04;             //0xc800a2b0L
	PE_H15A0_SR_SHP_TGEN_CTRL_05_T          pe1_tgen_ctrl_05;             //0xc800a2b4L
	PE_H15A0_SR_SHP_TGEN_CTRL_06_T          pe1_tgen_ctrl_06;             //0xc800a2b8L
	PE_H15A0_SR_SHP_TGEN_CTRL_07_T          pe1_tgen_ctrl_07;             //0xc800a2bcL
	PE_H15A0_SR_SHP_TGEN_CTRL_08_T          pe1_tgen_ctrl_08;             //0xc800a2c0L
	PE_H15A0_SR_SHP_TGEN_CTRL_09_T          pe1_tgen_ctrl_09;             //0xc800a2c4L
}PE_SR_SHP_REG_H15A0_T;

typedef struct {
	PE_H15A0_SR_GSC_Y_CTRL_T              	sr_gsc_y_ctrl;                //0xc800a800L
	UINT32                             	    reserved00;                   //0xc800a804L
	PE_H15A0_SR_GSC_Y_OUTSIZE_T           	sr_gsc_y_outsize;             //0xc800a808L
	UINT32                             	    reserved01[3];                //0xc800a80cL~0xc800a814L
	PE_H15A0_SR_GSC_Y_INSIZE_T            	sr_gsc_y_insize;              //0xc800a818L
	UINT32                             	    reserved02;                   //0xc800a81cL
	PE_H15A0_SR_GSC_Y_PHASE_OFFSET_T      	sr_gsc_y_phase_offset;        //0xc800a820L
	PE_H15A0_SR_GSC_Y_SAMPLING_H_T        	sr_gsc_y_sampling_h;          //0xc800a824L
	PE_H15A0_SR_GSC_Y_SAMPLING_V_T        	sr_gsc_y_sampling_v;          //0xc800a828L
	UINT32                             	    reserved03[6];                //0xc800a82cL~0xc800a840L
	PE_H15A0_SR_GSC_Y_V1_FIR_COEF_CTRL_T  	sr_gsc_y_v1_fir_coef_ctrl;    //0xc800a844L
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA0_T	sr_gsc_y_v1_y_fir_coef_data0; //0xc800a848L
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA1_T	sr_gsc_y_v1_y_fir_coef_data1; //0xc800a84cL
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA2_T	sr_gsc_y_v1_y_fir_coef_data2; //0xc800a850L
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA3_T	sr_gsc_y_v1_y_fir_coef_data3; //0xc800a854L
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA4_T	sr_gsc_y_v1_y_fir_coef_data4; //0xc800a858L
	PE_H15A0_SR_GSC_Y_V1_Y_FIR_COEF_DATA5_T	sr_gsc_y_v1_y_fir_coef_data5; //0xc800a85cL
	PE_H15A0_SR_GSC_Y_V2_FIR_COEF_CTRL_T  	sr_gsc_y_v2_fir_coef_ctrl;    //0xc800a860L
	PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA0_T	sr_gsc_y_v2_y_fir_coef_data0; //0xc800a864L
	PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA1_T	sr_gsc_y_v2_y_fir_coef_data1; //0xc800a868L
	PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA2_T	sr_gsc_y_v2_y_fir_coef_data2; //0xc800a86cL
	PE_H15A0_SR_GSC_Y_V2_Y_FIR_COEF_DATA3_T	sr_gsc_y_v2_y_fir_coef_data3; //0xc800a870L
	PE_H15A0_SR_GSC_Y_H1_FIR_COEF_CTRL_T  	sr_gsc_y_h1_fir_coef_ctrl;    //0xc800a874L
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA0_T	sr_gsc_y_h1_y_fir_coef_data0; //0xc800a878L
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA1_T	sr_gsc_y_h1_y_fir_coef_data1; //0xc800a87cL
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA2_T	sr_gsc_y_h1_y_fir_coef_data2; //0xc800a880L
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA3_T	sr_gsc_y_h1_y_fir_coef_data3; //0xc800a884L
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA4_T	sr_gsc_y_h1_y_fir_coef_data4; //0xc800a888L
	PE_H15A0_SR_GSC_Y_H1_Y_FIR_COEF_DATA5_T	sr_gsc_y_h1_y_fir_coef_data5; //0xc800a88cL
	PE_H15A0_SR_GSC_Y_H2_FIR_COEF_CTRL_T  	sr_gsc_y_h2_fir_coef_ctrl;    //0xc800a890L
	PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA0_T	sr_gsc_y_h2_y_fir_coef_data0; //0xc800a894L
	PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA1_T	sr_gsc_y_h2_y_fir_coef_data1; //0xc800a898L
	PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA2_T	sr_gsc_y_h2_y_fir_coef_data2; //0xc800a89cL
	PE_H15A0_SR_GSC_Y_H2_Y_FIR_COEF_DATA3_T	sr_gsc_y_h2_y_fir_coef_data3; //0xc800a8a0L
}PE_SR_GSC_REG_H15A0_T;

#endif
