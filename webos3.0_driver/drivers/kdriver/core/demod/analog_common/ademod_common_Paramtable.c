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



/*****************************************************************************
**
**  Name: ademod_common_Paramtable.c
**
**  Description: Parameter table
**
**  Functions
**  Implemented:    
**
**  References:     
**
**  Exports:    
**
**  Dependencies:   ademod_common_demod.h 
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_Paramtable.h"
LX_ADEMOD_PARMENTRY ADEMOD_PARAMS[]=
{
	{0x14,	"LX_ADEMOD_PARM_AGC_OPTIONS",	PARM_RO,	1,	16,	"agc_options " },
	{0x24,	"LX_ADEMOD_PARM_CONFIG_OPTIONS",	PARM_RO,	1,	16,	"config_options " },
	{0x61,	"LX_ADEMOD_PARM_AUD_OPTIONS",	PARM_RO,	1,	16,	"aud_options " },
	{0x66,	"LX_ADEMOD_PARM_DAC_OPTIONS",	PARM_RO,	1,	16,	"dac_options " },
	{0x6D,	"LX_ADEMOD_PARM_SYNC_OPTIONS",	PARM_RO,	1,	16,	"sync_options " },
	{0x86,	"LX_ADEMOD_PARM_SMPL_RATE_DIV",	PARM_RW,	1,	10,	"sample_rate_div " },
	{0x88,	"LX_ADEMOD_PARM_I2C_WRITE_COUNT",	PARM_RW,	1,	10,	"i2c_write_counter " },
	{0x89,	"LX_ADEMOD_PARM_I2C_WRITE_OPTIM",	PARM_RW,	1,	10,	"i2c_write_optimization " },
	{0x07,	"LX_ADEMOD_PARM_SPCTR_INV",	PARM_RW,	1,	10,	"spectrum_invert " },
	{0x08,	"LX_ADEMOD_PARM_VID_BNDW",	PARM_RW,	1,	10,	"bandwidth " },
	{0x09,	"LX_ADEMOD_PARM_VID_GAIN",	PARM_RW,	1,	10,	"gain " },
	{0x0A,	"LX_ADEMOD_PARM_VID_OFFS",	PARM_RW,	1,	10,	"offset " },
	{0x1D,	"LX_ADEMOD_PARM_GRP_DEL",	PARM_RW,	80,	16,	"group_delay_coeff_0 group_delay_coeff_1 group_delay_coeff_2 group_delay_coeff_3 group_delay_coeff_4 group_delay_coeff_5 group_delay_coeff_6 group_delay_coeff_7 group_delay_coeff_8 group_delay_coeff_9 group_delay_coeff_10 group_delay_coeff_11 group_delay_coeff_12 group_delay_coeff_13 group_delay_coeff_14 group_delay_coeff_15 group_delay_coeff_16 group_delay_coeff_17 group_delay_coeff_18 group_delay_coeff_19 group_delay_coeff_20 group_delay_coeff_21 group_delay_coeff_22 group_delay_coeff_23 group_delay_coeff_24 group_delay_coeff_25 group_delay_coeff_26 group_delay_coeff_27 group_delay_coeff_28 group_delay_coeff_29 group_delay_coeff_30 group_delay_coeff_31 group_delay_coeff_32 group_delay_coeff_33 group_delay_coeff_34 group_delay_coeff_35 group_delay_coeff_36 group_delay_coeff_37 group_delay_coeff_38 group_delay_coeff_39 group_delay_coeff_40 group_delay_coeff_41 group_delay_coeff_42 group_delay_coeff_43 group_delay_coeff_44 group_delay_coeff_45 group_delay_coeff_46 group_delay_coeff_47 group_delay_coeff_48 group_delay_coeff_49 group_delay_coeff_50 group_delay_coeff_51 group_delay_coeff_52 group_delay_coeff_53 group_delay_coeff_54 group_delay_coeff_55 group_delay_coeff_56 group_delay_coeff_57 group_delay_coeff_58 group_delay_coeff_59 group_delay_coeff_60 group_delay_coeff_61 group_delay_coeff_62 group_delay_coeff_63 group_delay_coeff_64 group_delay_coeff_65 group_delay_coeff_66 group_delay_coeff_67 group_delay_coeff_68 group_delay_coeff_69 group_delay_coeff_70 group_delay_coeff_71 group_delay_coeff_72 group_delay_coeff_73 group_delay_coeff_74 group_delay_coeff_75 group_delay_coeff_76 group_delay_coeff_77 group_delay_coeff_78 group_delay_coeff_79 " },
	{0x26,	"LX_ADEMOD_PARM_VSB_ENERGY",	PARM_RW,	1,	10,	"vsb_bandwidth " },
	{0x2E,	"LX_ADEMOD_PARM_VIDCR_RANGE",	PARM_RW,	3,	10,	"option cr_range_min cr_range_max " },
	{0x5F,	"LX_ADEMOD_PARM_VIDBW_VIRTUALFS",	PARM_RW,	2,	16,	"bw_virtualfs_lsb bw_virtualfs_msb " },
	{0x60,	"LX_ADEMOD_PARM_VIDBW_TINC",	PARM_RW,	3,	10,	"bw_tinc_ph1 bw_tinc_ph2 bw_tinc_ph2_inv " },
	{0x7E,	"LX_ADEMOD_PARM_VIDOFFS_TWEAK",	PARM_RW,	1,	10,	"offs_tweak " },
	{0x01,	"LX_ADEMOD_PARM_AUD_MODE",	PARM_RW,	1,	10,	"mode " },
	{0x03,	"LX_ADEMOD_PARM_AUD_FRQ",	PARM_RW,	6,	10,	"aud_freq_sc1 aud_freq_sc1_min aud_freq_sc1_max reserved reserved reserved " },
	{0x0B,	"LX_ADEMOD_PARM_AUD_GAIN",	PARM_RW,	2,	10,	"dac_aud_gain option " },
	{0x0C,	"LX_ADEMOD_PARM_AUD_CLIP",	PARM_RW,	1,	10,	"dac_aud_clip " },
	{0x0D,	"LX_ADEMOD_PARM_AUD_OFFS",	PARM_RW,	1,	10,	"dac_aud_offset " },
	{0x2F,	"LX_ADEMOD_PARM_AUD_SIF_BW",	PARM_RW,	1,	10,	"sif_bw " },
	{0x30,	"LX_ADEMOD_PARM_AUD_MIX",	PARM_RW,	2,	10,	"mixer_down mixer_up " },
	{0x62,	"LX_ADEMOD_PARM_AUD_SIF_FRQ",	PARM_RW,	2,	10,	"sif_freq_low sif_freq_high " },
	{0x63,	"LX_ADEMOD_PARM_AUD_AGC_TARG",	PARM_RW,	2,	10,	"agc_target agc_bw_target " },
	{0x64,	"LX_ADEMOD_PARM_AUD_AGC_GAIN_THR",	PARM_RW,	2,	10,	"agc_gain_thresh_min agc_gain_thresh_max " },
	{0x65,	"LX_ADEMOD_PARM_AUD_AGC_GAIN_PRESET",	PARM_RW,	1,	10,	"agc_gain_preset " },
	{0x6F,	"LX_ADEMOD_PARM_AUD_AUTOMUTE",	PARM_RW,	1,	16,	"auto_mute " },
	{0x70,	"LX_ADEMOD_PARM_AUD_MUTESPEED",	PARM_RW,	3,	10,	"option mute_speed_on mute_speed_off " },
	{0x73,	"LX_ADEMOD_PARM_AUD_MONO_FRQ",	PARM_RW,	2,	10,	"mono_freq_low mono_freq_high " },
	{0x74,	"LX_ADEMOD_PARM_AUD_DEVIATION",	PARM_RW,	1,	10,	"mono_deviation " },
	{0x75,	"LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE",	PARM_RW,	1,	10,	"mono_bbsamplerate " },
	{0x76,	"LX_ADEMOD_PARM_AUD_DEEMPHASIS",	PARM_RW,	1,	10,	"mono_deemphasis " },
	{0x77,	"LX_ADEMOD_PARM_AUD_BSBNDEXTRAGAIN",	PARM_RW,	1,	10,	"mono_bbextragain " },
	{0x78,	"LX_ADEMOD_PARM_AUD_SIF_OUT",	PARM_RW,	2,	10,	"sif_out sif_mono " },
	{0x7B,	"LX_ADEMOD_PARM_AUD_MONO_GAIN",	PARM_RW,	5,	10,	"mono_gain_1 mono_gain_2 mono_gain_shift1 mono_gain_shift2 mono_gain_shift3 " },
	{0x7C,	"LX_ADEMOD_PARM_AUD_BUZZDELAY",	PARM_RW,	2,	10,	"buzz_delay_offs buzz_delay_fudge " },
	{0x31,	"LX_ADEMOD_PARM_AGC_GAIN_THR",	PARM_RW,	4,	10,	"selector option rf_gain_thre_min rf_gain_thre_max " },
	{0x32,	"LX_ADEMOD_PARM_AGC_GAIN_PRESET",	PARM_RW,	3,	10,	"selector option rf_gain_pres " },
	{0x33,	"LX_ADEMOD_PARM_AGC_RFIIR_BW",	PARM_RW,	2,	10,	"option rf_iir_bandw " },
	{0x34,	"LX_ADEMOD_PARM_AGC_LPD_ATTACK",	PARM_RW,	2,	10,	"option rf_lpd_attack " },
	{0x35,	"LX_ADEMOD_PARM_AGC_LPD_DECAY",	PARM_RW,	2,	10,	"option rf_lpd_decay " },
	{0x36,	"LX_ADEMOD_PARM_AGC_LPD_FASTDECAY",	PARM_RW,	2,	10,	"option rf_fast_decay " },
	{0x37,	"LX_ADEMOD_PARM_AGC_LPD_DECAYWIN",	PARM_RW,	2,	10,	"option rf_decay_win " },
	{0x38,	"LX_ADEMOD_PARM_AGC_LPD_DECAYTHR",	PARM_RW,	2,	10,	"option rf_decay_thr " },
	{0x39,	"LX_ADEMOD_PARM_AGC_DECFACTOR",	PARM_RW,	2,	10,	"option rf_dec_factor " },
	{0x3A,	"LX_ADEMOD_PARM_AGC_INSTAB_GAIN_ADJ",	PARM_RW,	2,	10,	"option rf_instab_gain " },
	{0x3B,	"LX_ADEMOD_PARM_AGC_BNDW",	PARM_RW,	2,	10,	"option rf_lpf_bandw " },
	{0x3C,	"LX_ADEMOD_PARM_AGC_SPEED",	PARM_RW,	5,	10,	"option rf_lpf_speed_poserr_rf rf_lpf_speed_negerr_rf rf_lpf_speed_poserr_if rf_lpf_speed_negerr_if " },
	{0x3D,	"LX_ADEMOD_PARM_AGC_BACKOFF",	PARM_RW,	2,	10,	"option backoff " },
	{0x80,	"LX_ADEMOD_PARM_AGC_FASTMODE",	PARM_RW,	1,	10,	"fast_mode " },
	{0x81,	"LX_ADEMOD_PARM_AGC_OUTMODE",	PARM_RW,	1,	10,	"out_mode " },
	{0x82,	"LX_ADEMOD_PARM_AGC_WANDMODE",	PARM_RW,	1,	10,	"wand_mode " },
	{0x87,	"LX_ADEMOD_PARM_AGC_PHASE",	PARM_RW,	1,	10,	"phase " },
	{0x3E,	"LX_ADEMOD_PARM_DIGAGC_GAIN_THR",	PARM_RW,	6,	10,	"selector option dig_gain_thre_min gain_lsb dig_gain_thre_min gain_msb dig_gain_thre_max gain_lsb dig_gain_thre_max gain_msb " },
	{0x3F,	"LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET",	PARM_RW,	4,	10,	"selector option dig_gain_pres_lsb dig_gain_pres_msb " },
	{0x40,	"LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE",	PARM_RW,	1,	10,	"dig_coh_mode " },
	{0x41,	"LX_ADEMOD_PARM_DIGAGC_LPD_ATTACK",	PARM_RW,	2,	10,	"option dig_lpd_attack " },
	{0x42,	"LX_ADEMOD_PARM_DIGAGC_LPD_DECAY",	PARM_RW,	2,	10,	"option dig_lpd_decay " },
	{0x43,	"LX_ADEMOD_PARM_DIGAGC_LPD_FASTDECAY",	PARM_RW,	2,	10,	"option dig_fast_decay " },
	{0x44,	"LX_ADEMOD_PARM_DIGAGC_LPD_DECAYWIN",	PARM_RW,	2,	10,	"option dig_decay_win " },
	{0x45,	"LX_ADEMOD_PARM_DIGAGC_LPD_DECAYTHR",	PARM_RW,	2,	10,	"option dig_decay_thr " },
	{0x46,	"LX_ADEMOD_PARM_DIGAGC_BNDW",	PARM_RW,	2,	10,	"option dig_lpf_bandw " },
	{0x47,	"LX_ADEMOD_PARM_DIGAGC_SPEED",	PARM_RW,	7,	10,	"option resp_time_poserr_rf resp_time_negerr_rf resp_time_poserr_if resp_time_negerr_if resp_time_poserr_dig resp_time_negerr_dig " },
	{0x48,	"LX_ADEMOD_PARM_DIGAGC_BACKOFF",	PARM_RW,	2,	10,	"option backoff " },
	{0x49,	"LX_ADEMOD_PARM_DIGAGC_BNDW_COH",	PARM_RW,	2,	10,	"option bandwidth_coherent_dig " },
	{0x4A,	"LX_ADEMOD_PARM_DIGAGC_SPEED_COH",	PARM_RW,	7,	10,	"option dig_lpf_speed_coh_poserr_rf dig_lpf_speed_coh_negerr_rf dig_lpf_speed_coh_poserr_if dig_lpf_speed_coh_negerr_if dig_lpf_speed_coh_poserr_dig dig_lpf_speed_coh_negerr_dig " },
	{0x4B,	"LX_ADEMOD_PARM_DIGAGC_BACKOFF_COH",	PARM_RW,	2,	10,	"option backoff_coherent " },
	{0x71,	"LX_ADEMOD_PARM_DIGAGC_NBND_THR",	PARM_RW,	3,	10,	"option dig_lpf_narbnd_thr1 dig_lpf_narbnd_thr2 " },
	{0x72,	"LX_ADEMOD_PARM_DIGAGC_NBND_OFFS",	PARM_RW,	4,	10,	"option dig_lpf_narbnd_offs1 dig_lpf_narbnd_offs2 dig_lpf_narbnd_offs3 " },
	{0x7A,	"LX_ADEMOD_PARM_DIGAGC_NBND_MODE",	PARM_RW,	1,	10,	"dig_lpf_narbnd_mode " },
	{0x8A,	"LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET",	PARM_RW,	2,	10,	"option dig_target " },
	{0x8B,	"LX_ADEMOD_PARM_DIGAGC_DECIMATION_RATIO",	PARM_RW,	2,	10,	"option dig_dec_ratio " },
	{0x8C,	"LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR",	PARM_RW,	7,	10,	"option dig_lpf_speed_hyb_poserr_rf dig_lpf_speed_hyb_negerr_rf dig_lpf_speed_hyb_poserr_if dig_lpf_speed_hyb_negerr_if dig_lpf_speed_hyb_poserr_dig dig_lpf_speed_hyb_negerr_dig " },
	{0x02,	"LX_ADEMOD_PARM_VID_FRQ",	PARM_RW,	1,	10,	"int_frq " },
	{0x04,	"LX_ADEMOD_PARM_SMPL_RATE",	PARM_RW,	1,	10,	"sample_rate " },
	{0x20,	"LX_ADEMOD_PARM_VIDCR_OVERMOD",	PARM_RW,	11,	10,	"mode cr_overmod_noise_thre_lock cr_overmod_noise_thre_acq cr_overmod_thre_lock cr_overmod_thre_acq cr_overmod_thre_2_lock cr_overmod_thre_2_acq cr_overmod_noise_w_acq cr_overmod_noise_w_lock cr_overmod_noise_2_w_acq cr_overmod_noise_2_w_lock " },
	{0x4C,	"LX_ADEMOD_PARM_VIDCR_FRLOOP_SPEED",	PARM_RW,	2,	10,	"option cr_frloopspeed " },
	{0x4D,	"LX_ADEMOD_PARM_VIDCR_FRLOOP_GAIN",	PARM_RW,	2,	10,	"option cr_frloopgain " },
	{0x4E,	"LX_ADEMOD_PARM_VIDCR_PHLOOP_SPEED",	PARM_RW,	2,	10,	"option cr_phloopspeed " },
	{0x4F,	"LX_ADEMOD_PARM_VIDCR_PHLOOP_GAIN",	PARM_RW,	2,	10,	"option cr_phloopspeed " },
	{0x51,	"LX_ADEMOD_PARM_VIDCR_LOCKDETERR",	PARM_RW,	2,	10,	"option cr_lockdet " },
	{0x52,	"LX_ADEMOD_PARM_VIDCR_LOCKDETLINES",	PARM_RW,	2,	10,	"option cr_lockdetlines " },
	{0x54,	"LX_ADEMOD_PARM_VIDCR_UNLOCKDETERR",	PARM_RW,	2,	10,	"option cr_unlockdeterr " },
	{0x55,	"LX_ADEMOD_PARM_VIDCR_UNLOCKDETLINES",	PARM_RW,	2,	10,	"option cr_unlockdetlines " },
	{0x57,	"LX_ADEMOD_PARM_VIDCR_MUTELOCKDETERR",	PARM_RW,	2,	10,	"option cr_mutelockdet " },
	{0x58,	"LX_ADEMOD_PARM_VIDCR_MUTELOCKDETLINES",	PARM_RW,	2,	10,	"option cr_mutelockdetlines " },
	{0x5A,	"LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETERR",	PARM_RW,	2,	10,	"option cr_muteunlockdet " },
	{0x5B,	"LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETLINES",	PARM_RW,	2,	10,	"option cr_muteunlockdetlines " },
	{0x5C,	"LX_ADEMOD_PARM_VIDCR_OVERMOD_WEIGHTING",	PARM_RW,	1,	10,	"cr_overmod_weight " },
	{0x5D,	"LX_ADEMOD_PARM_VIDCR_OVERMOD_PHASEREV",	PARM_RW,	3,	10,	"option cr_overmod_perc cr_overmod_phaserev_angle " },
	{0x5E,	"LX_ADEMOD_PARM_VIDCR_RESET",	PARM_RW,	4,	10,	"option reset_fr1_thresh reset_crr_bound_tc reset_crr_lock_tc " },
	{0x67,	"LX_ADEMOD_PARM_VID_CLIP",	PARM_RW,	1,	10,	"dac_vid_clip " },
	{0x68,	"LX_ADEMOD_PARM_DAC_SRCSEL",	PARM_RW,	4,	10,	"dac_src_sel_aud dac_src_sel_vid dac_src_sel_if dac_src_sel_rf " },
	{0x69,	"LX_ADEMOD_PARM_DAC_DITHER",	PARM_RW,	4,	10,	"selector dac_dither dac_dither_lfsr dac_dither_init " },
	{0x6A,	"LX_ADEMOD_PARM_VID_INSTAB",	PARM_RW,	1,	10,	"dac_instabthr_vid " },
	{0x6B,	"LX_ADEMOD_PARM_AUD_INSTAB",	PARM_RW,	1,	10,	"dac_instabthr_aud_downmix dac_instabthr_aud_upmix " },
	{0x6C,	"LX_ADEMOD_PARM_AGC_INSTAB",	PARM_RW,	2,	10,	"selector dac_instabthr " },
	{0x6E,	"LX_ADEMOD_PARM_VIDCR_OUT_CF",	PARM_RW,	1,	10,	"lif_out_cf " },
	{0x7F,	"LX_ADEMOD_PARM_LOWIF_BNDW",	PARM_RW,	1,	10,	"lowif_bndw " },
	{0x0E,	"LX_ADEMOD_PARM_LOCK_STAT",	PARM_RO,	1,	10,	"lock_status " },
	{0x21,	"LX_ADEMOD_PARM_ISALIVE_STAT",	PARM_RO,	1,	10,	"comm_status " },
	{0x23,	"LX_ADEMOD_PARM_SYNC_STAT",	PARM_RO,	1,	16,	"sync_status " },
	{0x28,	"LX_ADEMOD_PARM_CARR_OFFS",	PARM_RO,	1,	10,	"carrier_offset " },
	{0x29,	"LX_ADEMOD_PARM_PLL_STAT",	PARM_RO,	1,	10,	"pll_status " },
	{0x2A,	"LX_ADEMOD_PARM_API_VER",	PARM_RO,	1,	10,	"api_version " },
	{0x7D,	"LX_ADEMOD_PARM_IF_LEVEL_STAT",	PARM_RO,	2,	10,	"if_level digital_gain " },
	{0x83,	"LX_ADEMOD_PARM_RF_LEVEL_STAT",	PARM_RO,	4,	10,	"top_mode rf_gain digital_rf_gain digital_gain " },
	{0x93,	"LX_ADEMOD_PARM_VID_GAIN_RC",	PARM_RW,	2,	10, "gain_rc " },
	{0x94,	"LX_ADEMOD_PARM_VID_OFFS_RC",	PARM_RW,	2,	10, "offset_rc " },
	{0x95,	"LX_ADEMOD_PARM_VID_CLIP_RC",	PARM_RW,	2,	10, "dac_vid_clip_rc " },
	{0x96,	"LX_ADEMOD_PARM_AUD_GAIN_RC",	PARM_RW,	1,	10, "dac_aud_gain option_rc " },
	{0x97,	"LX_ADEMOD_PARM_AUD_OFFS_RC",	PARM_RW,	1,	10, "dac_aud_offset_rc " },
	{0x98,	"LX_ADEMOD_PARM_AUD_CLIP_RC",	PARM_RW,	1,	10, "dac_aud_clip_rc " },





	END_OF_PARAM_LIST
};
