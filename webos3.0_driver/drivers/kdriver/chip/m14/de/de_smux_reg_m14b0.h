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

#ifndef _REG_SMUX_M14B0_H_
#define _REG_SMUX_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020300L smux_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 o_c_fmt                            : 3 ;  //  2: 0     
	UINT32 prew_rd_422                        : 1 ;  //     3     
	UINT32 i_c_fmt                            : 3 ;  //  6: 4     
	UINT32                                    : 5 ;  // 11: 7     reserved
	UINT32 prew_mux_sel                       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cvi_mux_sel                        : 3 ;  // 18:16     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 wr_disalbe                         : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_size                           : 1 ;  //    28     
	UINT32 set_mvi_val                        : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 pr_sq                              : 1 ;  //    31     
} M14B0_SMUX_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0020304L smux_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mifr_mode                          : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 hsc_enable                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 gos_enable                         : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 line_diff_en                       : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 line_diff                          : 12;  // 31:20     
} M14B0_SMUX_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0020308L smux_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_req_pend_val                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_req_pend_val                     : 12;  // 27:16     
} M14B0_SMUX_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc002030cL smux_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lr_switch_on                       : 1 ;  //     0     
	UINT32 lr_switch_mode                     : 1 ;  //     1     
	UINT32 lr_switch_auto                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 fifo_split                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 lr_write_sel                       : 1 ;  //     8     
	UINT32 lr_write_mode                      : 1 ;  //     9     
	UINT32 lr_write_auto                      : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 dec3d_mux_sel                      : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 auto_3d_mode                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 smux_front_buffer_detour           : 1 ;  //    20     
} M14B0_SMUX_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0020310L smux_ctrlr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_o_c_fmt                          : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 r_i_c_fmt                          : 3 ;  //  6: 4     
	UINT32                                    : 24;  // 30: 7     reserved
	UINT32 r_pr_sq                            : 1 ;  //    31     
} M14B0_SMUX_CTRLR_T;

/*-----------------------------------------------------------------------------
		0xc0020314L smux_hsc_y_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_y                     : 6 ;  //  5: 0     
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 aal_coef_0_y                       : 10;  // 21:12     
} M14B0_SMUX_HSC_Y_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc0020318L smux_hsc_y_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_2_y                       : 10;  // 21:12     
} M14B0_SMUX_HSC_Y_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc002031cL smux_hsc_y_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_4_y                       : 10;  // 21:12     
} M14B0_SMUX_HSC_Y_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc0020320L smux_hsc_y_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_5_y                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_6_y                       : 10;  // 21:12     
} M14B0_SMUX_HSC_Y_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc0020324L smux_hsc_c_set_val_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_offset_c                     : 6 ;  //  5: 0     
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 aal_coef_0_c                       : 10;  // 21:12     
} M14B0_SMUX_HSC_C_SET_VAL_0_T;

/*-----------------------------------------------------------------------------
		0xc0020328L smux_hsc_c_set_val_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_1_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_2_c                       : 10;  // 21:12     
} M14B0_SMUX_HSC_C_SET_VAL_1_T;

/*-----------------------------------------------------------------------------
		0xc002032cL smux_hsc_c_set_val_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_3_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_4_c                       : 10;  // 21:12     
} M14B0_SMUX_HSC_C_SET_VAL_2_T;

/*-----------------------------------------------------------------------------
		0xc0020330L smux_hsc_c_set_val_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aal_coef_5_c                       : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 aal_coef_6_c                       : 10;  // 21:12     
} M14B0_SMUX_HSC_C_SET_VAL_3_T;

/*-----------------------------------------------------------------------------
		0xc0020334L smux_size_i ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_i                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize_i                            : 12;  // 27:16     
} M14B0_SMUX_SIZE_I_T;

/*-----------------------------------------------------------------------------
		0xc0020338L smux_size_o ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16     
} M14B0_SMUX_SIZE_O_T;

/*-----------------------------------------------------------------------------
		0xc002033cL smux_size_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize_r                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize_r                            : 12;  // 27:16     
} M14B0_SMUX_SIZE_R_T;

/*-----------------------------------------------------------------------------
		0xc0020340L smux_hsc_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 voffset                            : 12;  // 27:16     
} M14B0_SMUX_HSC_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0020344L smux_hsc_active ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hactive                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vactive                            : 12;  // 27:16     
} M14B0_SMUX_HSC_ACTIVE_T;

/*-----------------------------------------------------------------------------
		0xc0020348L smux_pixel_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_data                            : 10;  //  9: 0     
	UINT32 cb_data                            : 10;  // 19:10     
	UINT32 y_data                             : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_pixel_data_en                  : 1 ;  //    31     
} M14B0_SMUX_PIXEL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc002034cL smux_hsc_input_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsc_i_hsize                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hsc_i_vsize                        : 12;  // 27:16     
} M14B0_SMUX_HSC_INPUT_INFO_T;

/*-----------------------------------------------------------------------------
		0xc0020350L smux_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 20;  // 19: 0     reserved
	UINT32 smux_pic_init_r_count              : 4 ;  // 23:20     
	UINT32 smux_pic_init_w_count              : 4 ;  // 19:16     
	UINT32 smux_ro_lcnt                       : 12;  // 11: 0     
} M14B0_SMUX_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0020354L smux_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 smux_ro_w_lcnt                     : 12;  // 11: 0     
} M14B0_SMUX_STATUS1_T;

typedef struct {
	M14B0_SMUX_CTRL0_T                 	smux_ctrl0;                   //0xc0020300L
	M14B0_SMUX_CTRL1_T                 	smux_ctrl1;                   //0xc0020304L
	M14B0_SMUX_CTRL2_T                 	smux_ctrl2;                   //0xc0020308L
	M14B0_SMUX_CTRL3_T                 	smux_ctrl3;                   //0xc002030cL
	M14B0_SMUX_CTRLR_T                 	smux_ctrlr;                   //0xc0020310L
	M14B0_SMUX_HSC_Y_SET_VAL_0_T       	smux_hsc_y_set_val_0;         //0xc0020314L
	M14B0_SMUX_HSC_Y_SET_VAL_1_T       	smux_hsc_y_set_val_1;         //0xc0020318L
	M14B0_SMUX_HSC_Y_SET_VAL_2_T       	smux_hsc_y_set_val_2;         //0xc002031cL
	M14B0_SMUX_HSC_Y_SET_VAL_3_T       	smux_hsc_y_set_val_3;         //0xc0020320L
	M14B0_SMUX_HSC_C_SET_VAL_0_T       	smux_hsc_c_set_val_0;         //0xc0020324L
	M14B0_SMUX_HSC_C_SET_VAL_1_T       	smux_hsc_c_set_val_1;         //0xc0020328L
	M14B0_SMUX_HSC_C_SET_VAL_2_T       	smux_hsc_c_set_val_2;         //0xc002032cL
	M14B0_SMUX_HSC_C_SET_VAL_3_T       	smux_hsc_c_set_val_3;         //0xc0020330L
	M14B0_SMUX_SIZE_I_T                	smux_size_i;                  //0xc0020334L
	M14B0_SMUX_SIZE_O_T                	smux_size_o;                  //0xc0020338L
	M14B0_SMUX_SIZE_R_T                	smux_size_r;                  //0xc002033cL
	M14B0_SMUX_HSC_OFFSET_T            	smux_hsc_offset;              //0xc0020340L
	M14B0_SMUX_HSC_ACTIVE_T            	smux_hsc_active;              //0xc0020344L
	M14B0_SMUX_PIXEL_CTRL_T            	smux_pixel_ctrl;              //0xc0020348L
	M14B0_SMUX_HSC_INPUT_INFO_T        	smux_hsc_input_info;          //0xc002034cL
	M14B0_SMUX_STATUS_T                	smux_status;                  //0xc0020350L
	M14B0_SMUX_STATUS1_T               	smux_status1;                 //0xc0020354L
} DE_SMM_REG_M14B0_T;

typedef struct {
	M14B0_SMUX_CTRL0_T                 	smux_ctrl0;                   //0xc0020600L
	M14B0_SMUX_CTRL1_T                 	smux_ctrl1;                   //0xc0020604L
	M14B0_SMUX_CTRL2_T                 	smux_ctrl2;                   //0xc0020608L
	M14B0_SMUX_CTRL3_T                 	smux_ctrl3;                   //0xc002060cL
	M14B0_SMUX_CTRLR_T                 	smux_ctrlr;                   //0xc0020610L
	M14B0_SMUX_HSC_Y_SET_VAL_0_T       	smux_hsc_y_set_val_0;         //0xc0020614L
	M14B0_SMUX_HSC_Y_SET_VAL_1_T       	smux_hsc_y_set_val_1;         //0xc0020618L
	M14B0_SMUX_HSC_Y_SET_VAL_2_T       	smux_hsc_y_set_val_2;         //0xc002061cL
	M14B0_SMUX_HSC_Y_SET_VAL_3_T       	smux_hsc_y_set_val_3;         //0xc0020620L
	M14B0_SMUX_HSC_C_SET_VAL_0_T       	smux_hsc_c_set_val_0;         //0xc0020624L
	M14B0_SMUX_HSC_C_SET_VAL_1_T       	smux_hsc_c_set_val_1;         //0xc0020628L
	M14B0_SMUX_HSC_C_SET_VAL_2_T       	smux_hsc_c_set_val_2;         //0xc002062cL
	M14B0_SMUX_HSC_C_SET_VAL_3_T       	smux_hsc_c_set_val_3;         //0xc0020630L
	M14B0_SMUX_SIZE_I_T                	smux_size_i;                  //0xc0020634L
	M14B0_SMUX_SIZE_O_T                	smux_size_o;                  //0xc0020638L
	M14B0_SMUX_SIZE_R_T                	smux_size_r;                  //0xc002063cL
	M14B0_SMUX_HSC_OFFSET_T            	smux_hsc_offset;              //0xc0020640L
	M14B0_SMUX_HSC_ACTIVE_T            	smux_hsc_active;              //0xc0020644L
	M14B0_SMUX_PIXEL_CTRL_T            	smux_pixel_ctrl;              //0xc0020648L
	M14B0_SMUX_HSC_INPUT_INFO_T        	smux_hsc_input_info;          //0xc002064cL
	M14B0_SMUX_STATUS_T                	smux_status;                  //0xc0020650L
	M14B0_SMUX_STATUS1_T               	smux_status1;                 //0xc0020654L
} DE_SMS_REG_M14B0_T;

typedef struct {
	M14B0_SMUX_CTRL0_T                 	smux_ctrl0;                   //0xc0021e00L
	M14B0_SMUX_CTRL1_T                 	smux_ctrl1;                   //0xc0021e04L
	UINT32                   		reserved00;                   //0xc0021e08L
	M14B0_SMUX_CTRL3_T                 	smux_ctrl3;                   //0xc0021e0cL
	UINT32  	                   	reserved01;                   //0xc0021e10L
	M14B0_SMUX_HSC_Y_SET_VAL_0_T       	smux_hsc_y_set_val_0;         //0xc0021e14L
	M14B0_SMUX_HSC_Y_SET_VAL_1_T       	smux_hsc_y_set_val_1;         //0xc0021e18L
	M14B0_SMUX_HSC_Y_SET_VAL_2_T       	smux_hsc_y_set_val_2;         //0xc0021e1cL
	M14B0_SMUX_HSC_Y_SET_VAL_3_T       	smux_hsc_y_set_val_3;         //0xc0021e20L
	M14B0_SMUX_HSC_C_SET_VAL_0_T       	smux_hsc_c_set_val_0;         //0xc0021e24L
	M14B0_SMUX_HSC_C_SET_VAL_1_T       	smux_hsc_c_set_val_1;         //0xc0021e28L
	M14B0_SMUX_HSC_C_SET_VAL_2_T       	smux_hsc_c_set_val_2;         //0xc0021e2cL
	M14B0_SMUX_HSC_C_SET_VAL_3_T       	smux_hsc_c_set_val_3;         //0xc0021e30L
	M14B0_SMUX_SIZE_I_T                	smux_size_i;                  //0xc0021e34L
	M14B0_SMUX_SIZE_O_T                	smux_size_o;                  //0xc0021e38L
	UINT32  	                   	reserved02;                   //0xc0021e3cL
	M14B0_SMUX_HSC_OFFSET_T            	smux_hsc_offset;              //0xc0021e40L
	M14B0_SMUX_HSC_ACTIVE_T            	smux_hsc_active;              //0xc0021e44L
	M14B0_SMUX_PIXEL_CTRL_T            	smux_pixel_ctrl;              //0xc0021e48L
	M14B0_SMUX_HSC_INPUT_INFO_T        	smux_hsc_input_info;          //0xc0021e4cL
	M14B0_SMUX_STATUS_T                	smux_status;                  //0xc0021e50L
	M14B0_SMUX_STATUS1_T               	smux_status1;                 //0xc0021e54L
} DE_SMW_REG_M14B0_T;

#endif
