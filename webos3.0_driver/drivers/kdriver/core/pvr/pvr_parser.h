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
 *  main configuration file for pvr device
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		kyungbin.pak
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef	_PVR_PARSER_H_
#define	_PVR_PARSER_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	PVR_DOWN_DATA_BUFFER_ALIGN		(12)

#define	PVR_DOWN_DATA_BUFFER_MAX_THRESHOLD	(PVR_DOWN_DATA_BUFFER_THRESHOLD * 4)
#define	PVR_DOWN_DATA_BUFFER_MIN_THRESHOLD	(PVR_DOWN_DATA_BUFFER_THRESHOLD / 64)

		//#define	PVR_DOWN_DATA_AU_BUFFER_THRESHOLD	(PVR_DOWN_DATA_BUFFER_THRESHOLD >> 3)
		//#define	PVR_DOWN_DATA_AU_BUFFER_MAX_THRESHOLD	(PVR_DOWN_DATA_AU_BUFFER_THRESHOLD * 4)
		//#define	PVR_DOWN_DATA_AU_BUFFER_MIN_THRESHOLD	(PVR_DOWN_DATA_AU_BUFFER_THRESHOLD / 32)

#define PVR_DOWN_DATA_BUFFER_ADJUST		(256*3) // change the data "128" to "256*3" at BCM35230 by kihun.lee
#define PVR_DOWN_DATA_BUFFER_SIZE		(PVR_DOWN_DATA_BUFFER_THRESHOLD * 24 + PVR_DOWN_DATA_BUFFER_ADJUST)

		//#define PVR_DOWN_DATA_AU_BUFFER_SIZE	(PVR_DOWN_DATA_AU_BUFFER_THRESHOLD * 24 + PVR_DOWN_DATA_BUFFER_ADJUST)

#define PVR_DOWN_INDEX_BUFFER_ADJUST	(192)
#define PVR_DOWN_INDEX_BUFFER_SIZE		((6*4*16)*96*2)
#define	PVR_DOWN_INDEX_BUFFER_THRESHOLD	(512)

#define	PVR_DOWN_MIN_DATASIZE_PER_SEC	(262144)  /*< For 2 Mbs stream */

#define	PVR_DOWN_MAX_INDEX_PER_SEC		(60)
#define	PVR_DOWN_MAX_INDEX_NUMBER		((PVR_DOWN_DATA_BUFFER_SIZE - PVR_DOWN_DATA_BUFFER_ADJUST) / PVR_DOWN_MIN_DATASIZE_PER_SEC * PVR_DOWN_MAX_INDEX_PER_SEC)

		/* If this count is too small, we can lose some index data.
		 * If this count is too large, we can consume too much time
		 * to get index data. When we consume too much time for this,
		 * the possibility of data buffer overflow will be increased.*/
#define	PVR_MAX_INDEX_RETRY				(4)

#define	PVR_TP_SYNC_BYTE				(0x47)

#define	NO_ADAPTATION_FIELD_RSRV		(0)
#define	NO_ADAPTATION_FIELD				(1)
#define	ADAPTATION_FIELD_ONLY			(2)
#define	ADAPTATION_FIELD_PAYLOAD		(3)

#define	TRANSPORT_ERROR_MASK			(0x80)
#define	PAYLOAD_UNIT_START_MASK			(0x40)
#define	TRANSPORT_PRIORITY_MASK			(0x20)
#define	PID_HIGH_BYTE_MASK				(0x1F)
#define	PID_LOW_BYTE_MASK				(0xFF)
#define	SCRAMBLING_CONTROL_MASK			(0xC0)
#define	ADAPTATION_FIELD_CONTROL_MASK	(0x30)
#define	CONTINUITY_COUNTER_MASK			(0x0F)

#define	TRANSPORT_ERROR_SHIFT			(7)
#define	PAYLOAD_UNIT_START_SHIFT		(6)
#define	TRANSPORT_PRIORITY_SHIFT		(5)
#define	PID_HIGH_BYTE_SHIFT				(8)
#define	PID_LOW_BYTE_SHIFT				(0)
#define	SCRAMBLING_CONTROL_SHIFT		(6)
#define	ADAPTATION_FIELD_CONTROL_SHIFT	(4)
#define	CONTINUITY_COUNTER_SHIFT		(0)

#define	GET_TRANSPORT_ERROR(pByte)			((*(char *)pByte & TRANSPORT_ERROR_MASK) >> TRANSPORT_ERROR_SHIFT)
#define	GET_PAYLOAD_UNIT_START(pByte)   	((*(char *)pByte & PAYLOAD_UNIT_START_MASK) >> PAYLOAD_UNIT_START_SHIFT)
#define	GET_TRANSPORT_PRIORITY(pByte)		((*(char *)pByte & TRANSPORT_PRIORITY_MASK) >> TRANSPORT_PRIORITY_SHIFT)
#define	GET_SCRAMBLING_CONTROL(pByte)		((*(char *)pByte & SCRAMBLING_CONTROL_MASK) >> SCRAMBLING_CONTROL_SHIFT)
#define	GET_ADAPTATION_FIELD_CONTROL(pByte)	((*(char *)pByte & ADAPTATION_FIELD_CONTROL_MASK) >> ADAPTATION_FIELD_CONTROL_SHIFT)
#define	GET_CONTINUITY_COUNTER(pByte)		((*(char *)pByte & CONTINUITY_COUNTER_MASK) >> CONTINUITY_COUNTER_SHIFT)

#define	PVR_INDEX_MPGE2_SEQ_START		0xB3
#define	PVR_INDEX_MPGE2_PIC_START		0x00

#define	PVR_INDEX_MPGE2_PIC_MASK		0x07
#define	PVR_INDEX_MPGE2_I_PIC			0x01
#define	PVR_INDEX_MPGE2_P_PIC			0x02
#define	PVR_INDEX_MPGE2_B_PIC			0x03

#define	PVR_INDEX_READY_WAIT_TIME		(10)
#define	PVR_INDEX_MAX_READY_WAIT_TIME	(280)

#define PVR_BYTE_SIZE					(8)

#define	PVR_H264_NAL_AUD				(9)
#define	PVR_H264_NAL_SPS				(7)
#define	PVR_H264_NAL_PPS				(8)

#define	PVR_H264_NAL_IDR				(5)
#define	PVR_H264_NAL_CS					(1)
#define	PVR_H264_NAL_CSA				(2)
#define	PVR_H264_NAL_CSB				(3)
#define	PVR_H264_NAL_CSC				(4)

#define	PVR_H264_NAL_EOS				(10)
#define	PVR_H264_NAL_EST				(11)

#define	PVR_H264_NAL_SEI				(6)
#define	PVR_H264_SEI_BP					(0)
#define	PVR_H264_SEI_PT					(1)
#define	PVR_H264_SEI_PSR				(2)
#define	PVR_H264_SEI_FP					(3)
#define	PVR_H264_SEI_UDR				(4)
#define	PVR_H264_SEI_UDU				(5)
#define	PVR_H264_SEI_RP					(6)
#define	PVR_H264_SEI_DRP				(7)
#define	PVR_H264_SEI_SP					(8)
#define	PVR_H264_SEI_SI					(9)
#define	PVR_H264_SEI_SSI				(10)
#define	PVR_H264_SEI_SSLC				(11)
#define	PVR_H264_SEI_SSC				(12)
#define	PVR_H264_SEI_FFF				(13)
#define	PVR_H264_SEI_FFFR				(14)
#define	PVR_H264_SEI_FFS				(15)
#define	PVR_H264_SEI_PRSS				(16)
#define	PVR_H264_SEI_PRSE				(17)
#define	PVR_H264_SEI_MCS				(18)
#define	PVR_H264_SEI_INVALID			(0xFF)

#define	PVR_DOWN_TS_PACKET_SIZE			(188)	/* Standard TS packet size */
#define	PVR_DOWN_MIN_TS_HEADER_SIZE		(5)		/* 1 sync. byte + 4 bytes fixed heade */
#define	PVR_DOWN_PACKET_SIZE			(192)	/* 4 bytes time stamp + standard TS packet */
#define PVR_DOWN_MPEG2_SEQ_INFOSIZE		(11)

#define	PVR_H264_ALLOW_FR_CHANGE		(TRUE)

#define	PVR_TIME_STAMP_CLOCK			(27000000) //UNIT is Hz ==> 1 sec
#define	PVR_TIME_STAMP_MAX_DIFF			(29700000) //UNIT is Hz ==> 1.1 sec

#define PVR_DOWN_DATA_SEMA_NAME		"DNDEVDS"
#define PVR_DOWN_INDEX_SEMA_NAME	"DNDEVIS"

#ifdef CHECK_I_INTERVAL
#define	PVR_CHECK_IDR_TIME()		do { UINT64 miliSec; SC_TIME_GetElapsedMS(&miliSec); printf("==> IDR found [%d ms]\n", (UINT32)miliSec);} while(0)
#define	PVR_CHECK_I_TIME()			do { UINT64 miliSec; SC_TIME_GetElapsedMS(&miliSec); printf("==> I found [%d ms]\n", (UINT32)miliSec);} while(0)
#else
#define	PVR_CHECK_IDR_TIME()		do {} while(0)
#define	PVR_CHECK_I_TIME()			do {} while(0)
#endif


#define PVR_UPDATE_BITSTREAM_BUFFER(pData, startBitPos, bitSize, dataSize)	\
		do	\
		{	\
			pData = (pData) + (((bitSize) + PVR_BYTE_SIZE - (startBitPos) - 1)/ PVR_BYTE_SIZE);	\
			dataSize = (dataSize) -(((bitSize) + PVR_BYTE_SIZE - (startBitPos) - 1) / PVR_BYTE_SIZE);	\
			startBitPos = PVR_BYTE_SIZE - (((bitSize) + PVR_BYTE_SIZE - (startBitPos) - 1) % PVR_BYTE_SIZE) -1 ;	\
		} while(0)	\

#define PVR_GETWORD(pData, var)	\
		do	\
		{	\
			var = (*pData << 24);	\
			var |= (*(pData + 1) << 16);	\
			var |= (*(pData + 2) << 8);	\
			var |= (*(pData + 3));		\
		} while (0)

#define	PVR_READ_N_BITS(word, bitPos, n) ((word << (PVR_BYTE_SIZE*4 - bitPos -1)) >> (PVR_BYTE_SIZE*4 - n))

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

typedef enum _PVR_H264_PIC_TYPE_T
{
	H264_I = 0,
	H264_I_P,
	H264_I_P_B,
	H264_SI,
	H264_SI_SP,
	H264_I_SI,
	H264_I_SI_P_SP,
	H264_I_SI_P_SP_B
} PVR_H264_PIC_TYPE_T;

typedef enum _PVR_H264_PIC_STRUCT_T
{
	H264_FRAME = 0, 		/* (progressive) frame */
	H264_TOP,				/* top field */
	H264_BOTTOM,			/* bottom field */
	H264_TOP_BOT,			/* to field, bottom field, in that order */
	H264_BOT_TOP,			/* bottom field, to field, in that order */
	H264_TOP_BOT_TR,		/* to field, bottom field, top field repeated, in that order */
	H264_BOT_TOP_BR,		/* bottom field, to field, bottom field repeated, in that order */
	H264_FRAME_DOUBLE,		/* frame doubling */
	H264_FRAME_TRIPLE,		/* frame tripling */
	H264_RESERVED
} PVR_H264_PIC_STRUCT_T;

typedef enum _PVR_PICMODE_T
{
	PVR_PICMODE_UNSET = 0,			/* NOT SET YET */
	PVR_PICMODE_FRAME,				/* FRAME MODE (include top/bottom field within a picture) */
	PVR_PICMODE_FIELD				/* FIELD MODE ( a pictuer = field )*/
} PVR_PICMODE_T;

typedef enum _PVR_EXP_GOLOMB_MODE_T
{
	EXP_GOLOMB_UE = 0,
	EXP_GOLOMB_SE,
	EXP_GOLOMB_ME,
	EXP_GOLOMB_TE
} PVR_EXP_GOLOMB_MODE_T;

typedef struct _PVR_BIT_STREAM_T
{
	UINT8	*pData; 		/* data */

	UINT32	startBitPos;	/* Start bit position. 7 to 0. 7 for left first bit. */
	SINT32	dataSize;		/* data size */

	UINT32	bitSize;		/* parsed bit size */
} PVR_BIT_STREAM_T;

#define	PVR_MAX_PAY_LOADSIZE	((PVR_DOWN_TS_PACKET_SIZE - PVR_DOWN_MIN_TS_HEADER_SIZE) * 2)
typedef struct _PVR_SEI_HEADER_T
{
	UINT32	payloadType;
	UINT32	payloadSize;

	UINT8	payload[PVR_MAX_PAY_LOADSIZE];
} PVR_SEI_HEADER_T;

typedef struct _PVR_PIC_TIMING_T
{
	UINT32	cddpFlag;		/* cpb_dpb_delay_present_flag */
	UINT32	crdlm1; 		/* cpb_removal_delay_length_minus1 */
	UINT32	dodlm1; 		/* dpb_output_delay_length_minus1 */
	UINT32	crd;			/* cpb_removal_delay */
	UINT32	dod;			/* dpb_output_delay */

	UINT32	pspf;			/* pic_struct_present_flag */
	UINT32	ps; 			/* pic_struct */
	UINT32	numClockTS; 	/* number of clock TS */
	UINT32	ctFlag[3];		/* clock_timestamp_flag */
	UINT32	ct; 			/* ct_type */
	UINT32	nfbFlag;		/* nuit_field_based_flag */
	UINT32	cntt;			/* counting_type */
	UINT32	ftFlag; 		/* full_timestamp_flag */
	UINT32	dFlag;			/* discontinuity_flag */
	UINT32	cdFlag; 		/* cnt_dropped_flag */
	UINT32	nf; 			/* n_frames */
	UINT32	sv; 			/* seconds_value */
	UINT32	mv; 			/* minutes_value */
	UINT32	hv; 			/* hours_value */

	UINT32	sFlag;			/* seconds_flag */
	UINT32	sv1;			/* seconds_value */
	UINT32	mFlag;			/* minutes_flag */
	UINT32	mv1;			/* minutes_value */
	UINT32	hFlag;			/* hours_flag */
	UINT32	hv1;			/* hours_value */

	UINT32	tol;			/* time_offset_length */
	UINT32	to; 			/* time_offset */
} PVR_PIC_TIMING_T;

typedef struct _PVR_HRD_T
{
	UINT32	ccm1;			/* cpb_cnt_minus1 */
	UINT8	brs : 4;		/* bit_rate_scale */
	UINT8	css : 4;		/* cpb_size_scale */

	UINT32	brvm1[32];		/* bit_rate_value_minus1 */
	UINT32	csvm1[32];		/* cpb_size_value_minus1 */
	UINT8	cFlag[32];		/* cbr_flag */

	UINT32	icrdlm1 : 5;	/* initial_cpb_removal_delay_length_minus1 */
	UINT32	crdlm1 : 5; 	/* cpb_removal_delay_length_minus1 */
	UINT32	dodlm1 : 5; 	/* dpb_output_delay_length_minus1 */
	UINT32	tol : 5;		/* time_offset_length */
	UINT32	rsv : 12;
} PVR_HRD_T;

typedef struct _PVR_RECOVERY_POINT_T
{
	UINT32	rfc;			/* recovery_frame_cnt */
	UINT32	emFlag : 1; 	/* bit_rate_scale */
	UINT32	blFlag : 1; 	/* broken_link_flag */
	UINT32	csgi : 1;		/* changing_slice_group_idc */
} PVR_RECOVERY_POINT_T;

typedef struct _PVR_VUI_T
{
	UINT8	aripf;			/* aspect_ratio_info_present_flag */
	UINT8	arIdc;			/* aspect_ratio_idc */
	UINT16	sw; 			/* sar_width */
	UINT16	sh; 			/* sar_height */

	UINT8	oipFlag : 1;	/* overscan_info_present_flag */
	UINT8	oaFlag : 1; 	/* overscan_appropriate_flag */
	UINT8	vstpFlag : 1;	/* video_signal_type_present_flag */
	UINT8	vf : 3; 		/* video_format */
	UINT8	vfrFlag : 1;	/* video_full_range_flag */
	UINT8	cdpFlag : 1;	/* colour_description_present_flag */

	UINT8	cp; 			/* colour_primaries */
	UINT8	tc; 			/* transfer_characteristics */
	UINT8	mc; 			/* matrix_coefficients */

	UINT8	clipFlag;		/* chroma_loc_info_present_flag */
	UINT32	cslttf; 		/* chroma_sample_loc_type_top_field */
	UINT32	vdltbf; 		/* chroma_sample_loc_type_bottom_field */

	UINT8	tipFlag;		/* timing_info_present_flag */
	UINT32	nuit;			/* num_units_in_tick */
	UINT32	ts; 			/* time_scale */
	UINT8	ffrFlag;		/* fixed_frame_rate_flag */

	UINT8	nhppFlag;		/* nal_hrd_parameters_present_flag */
	PVR_HRD_T nhrdp;		/* nal hrd_parameters */

	UINT8	vhppFlag;		/* vcl_hrd_parameters_present_flag */
	PVR_HRD_T vhrdp;		/* vcl hrd_parameters */

	UINT8	ldhFlag;		/* low_delay_hrd_flag */
	UINT8	pspFlag;		/* pic_struct_present_flag */
	UINT8	brFlag; 		/* bitstream_restriction_flag */

	UINT8	mvopbFlag;		/* motion_vectors_over_pic_bondaries_flag */
	UINT32	mbppd;			/* max_bytes_per_pic_denom */
	UINT32	mbpmd;			/* max_bits_per_mb_denom */
	UINT32	lmmlh;			/* log2_max_mv_length_horizontal */
	UINT32	lmmlv;			/* log2_max_mv_length_vertical */
	UINT32	nrf;			/* num_reorder_frames */
	UINT32	mdfb;			/* max_dec_frame_buffering */
} PVR_VUI_T;

typedef struct _PVR_SPS_T
{
	UINT8	profileIdc;

	UINT8	cs0flag : 1;	/* constraint_set0_flag */
	UINT8	cs1flag : 1;	/* constraint_set1_flag */
	UINT8	cs2flag : 1;	/* constraint_set2_flag */
	UINT8	cs3flag : 1;	/* constraint_set3_flag */
	UINT8	cs4flag : 1;	/* constraint_set4_flag */
	UINT8	rsv1 : 3;

	UINT8	levelIdc;
	UINT32	spsID;			/* seq_parameter_set_id */
	UINT32	cfIDC;			/* chroma_format_idc */
	UINT32	scpFlag;		/* separate_colour_plane_flag */
	UINT32	bdlm8;			/* bit_depth_luma_minus8 */
	UINT32	bdcm8;			/* bit_depth_chroma_minus8 */

	UINT8	qyztbFlag;		/* qpprime_y_zero_transform_bypass_flag */
	UINT8	ssmpFlag;		/* seq_scaling_matrix_present_flag */

	UINT32	sslpFlag[12];	/* seq_scaling_list_present_flag */
	SINT32	sclist4x4[6][16];	/* ScalingList4x4 */
	SINT32	sclist8x8[6][64];	/* ScalingList8x8 */

	UINT32	lmfnm4; 		/* log2_max_frame_num_minus4 */
	UINT32	poct;			/* pic_order_cnt_type */
	UINT32	lmpoclm4;		/* log2_max_pic_order_cnt_lsb_minus4 */
	UINT32	dpoazFlag;		/* delta_pic_order_always_zero_flag */
	UINT32	ofnrp;			/* offset_for_non_ref_pic */
	UINT32	ofttbf; 		/* offset_for_top_to_bottom_field */
	UINT32	nrfipocc;		/* num_ref_frames_in_pic_order_cnt_cycle */
	UINT16	ofrf[255];		/* offset_for_ref_frame */

	UINT32	mnrf;			/* max_num_ref_frames */
	UINT32	gifnvaFlag; 	/* gaps_in_frame_num_value_allowed_flag */
	UINT32	pwimm1; 		/* pic_width_in_mbs_minus1 */
	UINT32	phimum1;		/* pic_height_in_map_units_minus1 */
	UINT8	fmoFlag : 1;	/* frame_mbs_only_flag */
	UINT8	maffFlag : 1;	/* mb_adaptive_frame_field_flag */
	UINT8	d8iFlag : 1;	/* direct_8x8_inference_flag */
	UINT8	fcFlag : 1; 	/* frame_cropping_flag */
	UINT8	rsv2 : 4;

	UINT32	fclo;			/* frame_crop_left_offset */
	UINT32	fcro;			/* frame_crop_right_offset */
	UINT32	fcto;			/* frame_crop_top_offset */
	UINT32	fcbo;			/* frame_crop_bottom_offset */

	UINT32	vuippFlag;		/* vui_parameters_present_flag */

	PVR_VUI_T	vuip;		/* VUI parameter */
} PVR_SPS_T;

typedef struct _PVR_TP_HEADER_INFO_T {
	UINT32	sync_byte : 8;
	UINT32	transport_error : 1;
	UINT32	payload_unit_start : 1;
	UINT32	transport_priority : 1;
	UINT32	pid : 13;
	UINT32	scrambling_control : 2;
	UINT32	adaptation_field_control : 2;
	UINT32	continuity_counter : 4;

	UINT32	byteOffsetPayload;
} PVR_TP_HEADER_INFO_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
UINT32	PVR_DD_GetTPHeader(UINT8 *pSingleTP, PVR_TP_HEADER_INFO_T *pTPHeader);
void PVR_DD_GetSPS(UINT8 *tpstream);
PVR_H264_PIC_STRUCT_T PVR_DD_GetPicStruct(UINT8 *tpstream);
UINT32 PVR_DD_UpdateH264StreamInfo(BOOLEAN bFoundSPS, UINT8 *tpstream, UINT32 packetOffset);
UINT32 PVR_DD_UpdateMPEG2StreamInfo(UINT8 *tpstream);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_PARSER_H_ */

/** @} */

