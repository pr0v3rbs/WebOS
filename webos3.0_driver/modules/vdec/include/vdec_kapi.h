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

/**
 * @file
 *
 * application interface header for vdec device
 */

#ifndef	_VDEC_KAPI_H_
#define	_VDEC_KAPI_H_


#include <linux/ioctl.h>

/**
 * Video Codec Type.
 * decoding formats supported by the Video decoder.
 */
typedef enum
{
	LX_VDEC_CODEC_INVALID = -1,

	LX_VDEC_CODEC_H264_HP,
	LX_VDEC_CODEC_H264_MVC,
	LX_VDEC_CODEC_H263_SORENSON,
	LX_VDEC_CODEC_VC1_RCV_V1,
	LX_VDEC_CODEC_VC1_RCV_V2,
	LX_VDEC_CODEC_VC1_ES,
	LX_VDEC_CODEC_MPEG2_HP,
	LX_VDEC_CODEC_MPEG4_ASP,
	LX_VDEC_CODEC_XVID,
	LX_VDEC_CODEC_DIVX3,
	LX_VDEC_CODEC_DIVX4,
	LX_VDEC_CODEC_DIVX5,
	LX_VDEC_CODEC_RVX,
	LX_VDEC_CODEC_AVS,
	LX_VDEC_CODEC_VP8,
	LX_VDEC_CODEC_THEORA,
	LX_VDEC_CODEC_HEVC,
        LX_VDEC_CODEC_VP9,
} LX_VDEC_CODEC_T;

typedef enum
{
	/** Video decoder input can be selected from system decoder 0. */
	LX_VDEC_SRC_SDEC0,

	/** Video decoder input can be selected from system decoder 1. */
	LX_VDEC_SRC_SDEC1,

	/** Video decoder input can be selected from system decoder 1. */
	LX_VDEC_SRC_SDEC2,

	/** Video decoder input is manually controlled by caller. */
	LX_VDEC_SRC_BUFF,

	/** Video decoder input is manually controlled by caller. */
	LX_VDEC_SRC_BUFFTVP,
} LX_VDEC_SRC_T;


typedef enum
{
	/**
	 * Video decoder output shall be directed to Main Window of DE(Display
	 * Engine) */
	LX_VDEC_DST_DE0,

	/**
	 * Video decoder output shall be directed to Sub Window of DE(Display
	 * Engine) */
	LX_VDEC_DST_DE1,

	LX_VDEC_DST_DE2,
	LX_VDEC_DST_DE3,
	LX_VDEC_DST_DE4,
	LX_VDEC_DST_DE5,

	/**
	 * Video decoder output shall be remains to DPB (Decoded Picture
	 * Buffer) not automatically passed to DE.  use this for thumbnail
	 * output. */
	LX_VDEC_DST_BUFF,
} LX_VDEC_DST_T;




typedef enum
{
	/**
	 * Video decoder will be operated on the broadcast/DVR/file play
	 * configuration
	 * */
	LX_VDEC_OPMOD_NORMAL = 0,

	/**
	 * Video decoder will be operated on the channel browser , thumbnail
	 * and drip decoding configuration */
	LX_VDEC_OPMOD_ONE_FRAME,
} LX_VDEC_OPMODE_T;



typedef enum
{
	LX_VDEC_MEMORY_FORMAT_RASTER,
	LX_VDEC_MEMORY_FORMAT_TILED,
} LX_VDEC_MEMORY_FORMAT_T;

typedef struct
{
	unsigned int base_stc;
	unsigned int base_pts;
} LX_VDEC_BASETIME_T;

typedef struct
{
	unsigned int stc;
	unsigned int mask;
} LX_VDEC_STC_T;

typedef struct
{
	unsigned long addr;
	int size;
	int read_offset;
	int write_offset;
} LX_VDEC_BUFFER_INFO_T;

typedef LX_VDEC_BUFFER_INFO_T LX_VDEC_CPB_INFO_T;

typedef struct
{
	unsigned long addr;
	int size;
} LX_VDEC_BUFFER_T;


/**
 * Buffer status for VDEC input buffer ( CPB )
 * used LX_VDEC_IO_GET_BUFFER_STATUS
 */
typedef struct
{
	unsigned int	cpb_depth;
	unsigned int	cpb_size;
	unsigned int	auib_depth;
	unsigned int	auib_size;
} LX_VDEC_IO_BUFFER_STATUS_T;

/**
 * Video Decoder Access Unit Type.
 * For update buffer write pointer for which type of this chunk of memory contains.
 * for LX_VDEC_IO_UPDATE_BUFFER_T::au_type
 */
typedef enum
{
	LX_VAU_SEQH = 1,	///< Sequence Header
	LX_VAU_SEQE,		///< Sequence End.
	LX_VAU_DATA,		///< Picture Data.
} LX_VAU_T;

/**
 * For update buffer write pointer for which type of this chunk of memory contains.
 * [NOTE] au_size should be 512 bytes unit.
 */
typedef struct
{
	unsigned long UId;		///< Unique ID
	LX_VAU_T au_type;		///< access unit type of this chunk memory.
	const unsigned char *au_ptr;	///< access unit pointer. in physical address.
	unsigned int au_size;		///< writing size should be multiple of 512 bytes.
#define	VDEC_UNKNOWN_TIMESTAMP		0xFFFFFFFFFFFFFFFFLL
	unsigned long long timestamp;	///< time stamp 1 ns.
} LX_VDEC_IO_UPDATE_BUFFER_T;

typedef struct
{
	unsigned int size;

	unsigned int buf1_offset;
	unsigned int buf1_size;

	/* for cpb wraparound */
	unsigned int buf2_offset;
	unsigned int buf2_size;

	LX_VAU_T au_type;
	unsigned long long timestamp;
	unsigned long uid;
} LX_VDEC_ES_DATA_T;


typedef enum
{
	lx_vdec_flags_pollerr	= (1<<0),

	/* vdc should copy DPB where user or I supplies. lx_vdec_flags_user_dpb
	 * also needed */
	lx_vdec_flags_copy_dpb	= (1<<1),

	/* user(middleware) will supply the DPB memory. no unref the displayed
	 * frame will be called */
	lx_vdec_flags_user_dpb	= (1<<2),

	/* UHD mode. video ES 가 UHD 스트림을 포함하고 있다. 혹은 UHD seamless
	 * 스트림이다.
	 */
	lx_vdec_flags_uhd_seemless = (1<<3),
	lx_vdec_flags_uhd = (1<<3),

	/* adec master mode on local clock mode. */
	lx_vdec_flags_adec_master = (1<<4),

	/* vdec master mode on local clock mode. */
	lx_vdec_flags_vdec_master = (1<<5),

	lx_vdec_flags_low_latency = (1<<6),
} lx_vdec_flags_t;

typedef struct
{
	/* memory from ion */
	int ion_y;
	unsigned long priv;

	/* memory from specific physical address */
	unsigned long y, cb, cr;

	/* color format */
	enum
	{
		fb_color_format_420,
	} color_format;
	enum
	{
		/* planner Y and following interleaved Cb and Cr */
		fb_buffer_format_y_ic,
	} buffer_format;

	/* buffer size */
	int width, height, stride;
} LX_VDEC_FB_T;

typedef struct
{
	/* buffer y address */
	unsigned long y;

	/* user buffer that the buffer will stored */
	unsigned char *result;
} LX_VDEC_CONVERT_FRAME_T;


typedef enum {
	VDEC_MAP_LINEAR		= 0,
	VDEC_MAP_TILED		= 1,
	VDEC_MAP_TILED_MIXED_V	= 1,
	VDEC_MAP_TILED_FRAME_V	= 2,
	VDEC_MAP_TILED_HEVC	= 3,
	VDEC_MAP_TILED_HEVC10	= 4,
	VDEC_MAP_TILED_8X4	= 5, /* g1 */
	VDEC_MAP_TILED_4X4	= 6, /* g2 */

	VDEC_MAP_MIN		= VDEC_MAP_LINEAR,
	VDEC_MAX		= VDEC_MAP_TILED_4X4,
	VDEC_INVALID
} VDEC_MAP_T;

typedef enum {
	VDEC_MAP_BALANCE_OFF	= 0,
	VDEC_MAP_BALANCE_1_1	= 1,
	VDEC_MAP_BALANCE_1_3	= 2,
	VDEC_MAP_BALANCE_3_1	= 3,

	VDEC_MAP_BALANCE_MIN	= VDEC_MAP_BALANCE_OFF,
	VDEC_MAP_BALANCE_MAX	= VDEC_MAP_BALANCE_3_1,
	VDEC_MAP_BALANCE_INVALID
} VDEC_MAP_BALANCE_T;

typedef struct {
	VDEC_MAP_T		eFrameMapType;
	unsigned int		ui32TiledBase;
	VDEC_MAP_BALANCE_T	eBalanceMode;
	unsigned short		ui16M0Offset;
	unsigned short		ui16M1Offset;
	unsigned char		ui8FrameIndex;
} VDEC_MAP_INFO_T;

typedef struct
{
	unsigned long long priv;
	VDEC_MAP_INFO_T map_info;
} LX_VDEC_FRAME_PRIV_T;






/**
 * scan mode of video decoder.
 * only specified picture type of input data, others shall be skipped.\n
 */
typedef enum
{
	LX_VDEC_PIC_SCAN_ALL,			///< decode IPB frame.
	LX_VDEC_PIC_SCAN_I,			///< I picture only (PB skip)
	LX_VDEC_PIC_SCAN_IP,			///< IP picture only (B skip only)

	/* dish CP use top only frame on trick mode */
	LX_VDEC_PIC_SCAN_I_BRAINFART_DISH_TRICK,
} LX_VDEC_PIC_SCAN_T;

typedef enum
{
	LX_VDEC_DISPLAY_FREEZE_UNSET,		///< freeze unset.
	LX_VDEC_DISPLAY_FREEZE_SET,		///< freeze set.
} LX_VDEC_DISPLAY_FREEZE_T;

typedef enum
{
	LX_VDEC_DISPLAY_SYNC_MATCH,
	LX_VDEC_DISPLAY_SYNC_FREERUN,
} LX_VDEC_DISPLAY_SYNC_T;

typedef enum
{
	LX_VDEC_D14_UHDSEAMLESS_NONE,


	/* First implementation of UHD seamless */
	LX_VDEC_D14_UHDSEAMLESS_ZZAMBBONG,	// for netflix


	/* Only supported for switching between 1920x1080 and 3840x2160 size.
	 * This mode use dual HDMI output to support 60Hz framerate.
	 *
	 * Dual HDMI output is sent to U14. 1920x1080 is scaled up to UHD at
	 * ULSA.
	 */
	LX_VDEC_D14_UHDSEAMLESS_EXTERNAL,	// for BBC


	/* Any frame size from SD to 3840x2160 can be. But, framerate should be
	 * less than or equal with 30Hz.
	 *
	 * All video output is sent to H13. H13 will scale the video to *x2160,
	 * vertically. And, ULSA will scale the video to 3840x2160,
	 * horizontally.
	 */
	LX_VDEC_D14_UHDSEAMLESS_INTERNAL,	// for Amazon
} LX_VDEC_D14_UHDSEAMLESS;





#define	LX_VDEC_IOC_MAGIC		'v'

#define LX_VDEC_IO_FLUSH			_IO  (LX_VDEC_IOC_MAGIC, 5)

#define LX_VDEC_IO_GET_BUFFER_STATUS		_IOR (LX_VDEC_IOC_MAGIC, 7,  LX_VDEC_IO_BUFFER_STATUS_T)
#define LX_VDEC_IO_UPDATE_BUFFER		_IOW (LX_VDEC_IOC_MAGIC, 8,  LX_VDEC_IO_UPDATE_BUFFER_T)
#define LX_VDEC_GET_ES_BUFFER			_IOWR(LX_VDEC_IOC_MAGIC, 9,  LX_VDEC_ES_DATA_T)
#define LX_VDEC_PUT_ES_BUFFER			_IOW (LX_VDEC_IOC_MAGIC, 10, LX_VDEC_ES_DATA_T)
#define LX_VDEC_GET_NOTI_INFO			_IOR (LX_VDEC_IOC_MAGIC, 11, LX_VDEC_BUFFER_INFO_T)
#define LX_VDEC_GET_NOTI_INFO_WAIT		_IOR (LX_VDEC_IOC_MAGIC, 12, LX_VDEC_BUFFER_INFO_T)
#define LX_VDEC_SET_NOTI_TAIL			_IO  (LX_VDEC_IOC_MAGIC, 13)
#define LX_VDEC_SET_PREFERRED_DEC		_IO  (LX_VDEC_IOC_MAGIC, 14)


#define LX_VDEC_UNREF_DECODEBUFFER		_IOW (LX_VDEC_IOC_MAGIC, 17, unsigned long)
#define LX_VDEC_SET_DECODING_QUEUE_SIZE		_IOW (LX_VDEC_IOC_MAGIC, 18, int)
#define LX_VDEC_GET_DECODED_QUEUE_SIZE		_IOR (LX_VDEC_IOC_MAGIC, 19, int)
#define LX_VDEC_SET_OUTPUT_MEMORY_FORMAT	_IOW (LX_VDEC_IOC_MAGIC, 20, LX_VDEC_MEMORY_FORMAT_T)
#define LX_VDEC_SET_SCAN_PICTURE		_IO  (LX_VDEC_IOC_MAGIC, 21)
#define LX_VDEC_SET_CODEC			_IO  (LX_VDEC_IOC_MAGIC, 23)
#define LX_VDEC_SET_DISPLAY_OFFSET		_IO  (LX_VDEC_IOC_MAGIC, 24)
#define LX_VDEC_SET_INPUT_DEVICE		_IO  (LX_VDEC_IOC_MAGIC, 25)
#define LX_VDEC_SET_OUTPUT_DEVICE		_IO  (LX_VDEC_IOC_MAGIC, 26)
#define LX_VDEC_SET_BASETIME			_IOW (LX_VDEC_IOC_MAGIC, 27, LX_VDEC_BASETIME_T)
#define LX_VDEC_GET_GLOBAL_STC			_IOR (LX_VDEC_IOC_MAGIC, 28, LX_VDEC_STC_T)
#define LX_VDEC_SET_ID				_IO  (LX_VDEC_IOC_MAGIC, 29)
#define LX_VDEC_STEAL_USERDATA			_IO  (LX_VDEC_IOC_MAGIC, 30)
#define LX_VDEC_SET_LOW_LATENCY			_IO  (LX_VDEC_IOC_MAGIC, 31)
#define LX_VDEC_SET_3D_TYPE			_IO  (LX_VDEC_IOC_MAGIC, 32)
#define LX_VDEC_GET_CPB_INFO			_IOR (LX_VDEC_IOC_MAGIC, 33, LX_VDEC_BUFFER_INFO_T)
#define LX_VDEC_SET_CPB				_IOW (LX_VDEC_IOC_MAGIC, 34, LX_VDEC_BUFFER_T)
#define LX_VDEC_SET_REQUEST_PICTURES		_IO  (LX_VDEC_IOC_MAGIC, 35)
#define LX_VDEC_RESET				_IO  (LX_VDEC_IOC_MAGIC, 36)
#define LX_VDEC_USE_GSTC			_IO  (LX_VDEC_IOC_MAGIC, 37)
#define LX_VDEC_SET_FLAGS			_IO  (LX_VDEC_IOC_MAGIC, 38)
#define LX_VDEC_DISPLAY_FREEZE			_IO  (LX_VDEC_IOC_MAGIC, 39)
#define LX_VDEC_DISPLAY_SYNC			_IO  (LX_VDEC_IOC_MAGIC, 40)
#define LX_VDEC_SET_SPEED			_IO  (LX_VDEC_IOC_MAGIC, 41)
#define LX_VDEC_SET_FRAMERATE			_IO  (LX_VDEC_IOC_MAGIC, 42)
#define LX_VDEC_SET_FRAMEBUFFER			_IOW (LX_VDEC_IOC_MAGIC, 43, LX_VDEC_FB_T)
#define LX_VDEC_SET_STEP			_IO  (LX_VDEC_IOC_MAGIC, 44)
#define LX_VDEC_INIT				_IO  (LX_VDEC_IOC_MAGIC, 45)
#define LX_VDEC_CONVERT_FRAME			_IOW (LX_VDEC_IOC_MAGIC, 46, LX_VDEC_CONVERT_FRAME_T)
#define LX_VDEC_EOS				_IO  (LX_VDEC_IOC_MAGIC, 47)
#define LX_VDEC_SET_ADEC_CHANNEL		_IO  (LX_VDEC_IOC_MAGIC, 48)
#define LX_VDEC_ADD_FLAGS			_IO  (LX_VDEC_IOC_MAGIC, 49)
#define LX_VDEC_DEL_FLAGS			_IO  (LX_VDEC_IOC_MAGIC, 50)
#define LX_VDEC_DESTROY				_IO  (LX_VDEC_IOC_MAGIC, 51)
#define LX_VDEC_SET_CHANNEL_NUMBER		_IO  (LX_VDEC_IOC_MAGIC, 52)
#define LX_VDEC_GET_STC				_IOR (LX_VDEC_IOC_MAGIC, 53, LX_VDEC_STC_T)
#define LX_VDEC_SET_D14_UHDSEAMLESS		_IO  (LX_VDEC_IOC_MAGIC, 54)
#define LX_VDEC_GET_BUFFER_PRIV			_IOWR(LX_VDEC_IOC_MAGIC, 55, LX_VDEC_FRAME_PRIV_T)




/*
 * D14 control ioctl parameters. Used at H13 to access D14 through SPI channel.
 */
struct d14_firmware
{
	void *data;
	int size;
};
struct d14_register
{
	unsigned int addr;
	unsigned int data;
};
struct d14_memory
{
	unsigned int addr;
	void *data;
	int size;
};

#define	LX_D14_IOC_MAGIC			'd'
#define D14_RESET				_IO  (LX_D14_IOC_MAGIC,  1)
#define D14_SET_SPI_SPEED			_IOWR(LX_D14_IOC_MAGIC,  2, int)
#define D14_SET_FIRMWARE			_IOW (LX_D14_IOC_MAGIC,  3, struct d14_firmware)
#define D14_GET_IPC				_IOWR(LX_D14_IOC_MAGIC,  4, struct d14_register)
#define D14_SET_IPC_				_IOW (LX_D14_IOC_MAGIC,  5, struct d14_register)
#define D14_SET_IPC				_IOW (LX_D14_IOC_MAGIC,  6, struct d14_register)
#define D14_GET_REG				_IOWR(LX_D14_IOC_MAGIC,  7, struct d14_register)
#define D14_SET_REG				_IOW (LX_D14_IOC_MAGIC,  8, struct d14_register)
#define D14_GET_MEM				_IOW (LX_D14_IOC_MAGIC,  9, struct d14_memory)
#define D14_SET_MEM				_IOW (LX_D14_IOC_MAGIC, 10, struct d14_memory)


#ifdef __KERNEL__
union ioctl_args
{
	LX_VDEC_IO_BUFFER_STATUS_T buffer_status;
	LX_VDEC_IO_UPDATE_BUFFER_T update_buffer;
	LX_VDEC_ES_DATA_T es_data;
	unsigned long unref_decodebuffer;
	unsigned int enable_log;
	int queue_size;
	LX_VDEC_MEMORY_FORMAT_T memory_format;
	LX_VDEC_BASETIME_T basetime;
	LX_VDEC_STC_T stc;
	LX_VDEC_BUFFER_INFO_T buffer_info;
	LX_VDEC_BUFFER_T buffer;
	LX_VDEC_FB_T fb;
	LX_VDEC_CONVERT_FRAME_T convert_frame;
	LX_VDEC_FRAME_PRIV_T frame_priv;


	/* d14 ioctls */
	int d14_spi_speed;
	struct d14_firmware d14_firmware;
	struct d14_register d14_register;
	struct d14_memory d14_memory;
};
#endif







#define LX_VDEC_FOURCC(s)		(s[0]<<0|s[1]<<8|s[2]<<16|s[3]<<24)
#define LX_VDEC_FOURCC2(s0,s1,s2,s3)	(s0<<0|s1<<8|s2<<16|s3<<24)

#define LX_VDEC_READ_MAGIC(s)		(s[0]<<0|s[1]<<8|s[2]<<16|s[3]<<24)
#define LX_VDEC_READ_MAGIC2(s0,s1,s2,s3) (s0<<0|s1<<8|s2<<16|s3<<24)

typedef struct decoded_buffer decoded_buffer_t;
struct decoded_buffer
{
	unsigned long addr_y;
	unsigned long addr_cb, addr_cr;
	//unsigned long addr_y_bot;	// bottom
	//unsigned long addr_cb_bot, addr_cr_bot;

	int buffer_index;
	unsigned long user_dpb_priv;

	/* 0 for linear
	 * other for tiled. */
	int vdo_map_type __attribute__((deprecated));	/* deprecated */

	int error_blocks;

	/**
	 * @DECODED_REPORT_LOW_DELAY: there is low_delay option on sequence
	 * header.
	 *
	 * @DECODED_REPORT_HW_RESET: require hardware reset. not used.
	 *
	 * @DECODED_REPORT_EOS: EOS action has been completed after
	 * ioctl(LX_VDEC_EOS)
	 *
	 * @DECODED_REPORT_REQUEST_DPB: require DPB memory after sequence init.
	 * only available with lx_vdec_flags_user_dpb flag set.
	 *
	 * @DECODED_REPORT_SEQUENCE_INIT_FAILED: sequence init failed. broken
	 * sequence data or no sequence data.  cannot proceed further decoding.
	 *
	 * @DECODED_REPORT_DECODE_FAILED: frame decoding failed.  but, decoding
	 * can be proceeding.
	 *
	 * @DECODED_REPORT_NOT_SUPPORTED_STREAM: this hardware cannot decode
	 * this video ES. out of hardware spec.
	 *
	 * @DECODED_REPORT_RESOURCE_BUSY: hardware resource is insufficient to
	 * decode the video ES. after closing some other decoder instance,
	 * decoder may decode the ES.
	 *
	 * @DISPLAY_REPORT_DROPPED: this frame has been dropped and did not
	 * displayed.
	 */
	enum
	{
		DECODED_REPORT_LOW_DELAY		= (1<<0),
		DECODED_REPORT_HW_RESET			= (1<<1),
		DECODED_REPORT_EOS			= (1<<2),
		DECODED_REPORT_REQUEST_DPB		= (1<<3),

		/* error case, bit8 ~ bit15 */
#define DECODED_REPORT_ERROR_MASK	0xff00
		DECODED_REPORT_SEQUENCE_INIT_FAILED	= (1<<8),
		DECODED_REPORT_DECODE_FAILED		= (1<<9),
		DECODED_REPORT_NOT_SUPPORTED_STREAM	= (1<<10),
		DECODED_REPORT_RESOURCE_BUSY		= (1<<11),

		DISPLAY_REPORT_DROPPED			= (1<<16),
	} report;

	int framerate_num;
	int framerate_den;

	int crop_left, crop_right;
	int crop_top, crop_bottom;

	int stride;
	int width;
	int height;

	int display_width;
	int display_height;

	enum
	{
		decoded_buffer_interlace_top_first,
		decoded_buffer_interlace_bottom_first,
		decoded_buffer_interlace_none,		// progressive

		/* DISH CP only */
		decoded_buffer_interlace_top_only,
		decoded_buffer_interlace_bottom_only,
	} interlace;

	enum
	{
		decoded_buffer_picture_type_i,
		decoded_buffer_picture_type_p,
		decoded_buffer_picture_type_b,
		decoded_buffer_picture_type_bi,
		decoded_buffer_picture_type_d,
		decoded_buffer_picture_type_s,
		decoded_buffer_picture_type_pskip,
	} picture_type;
	int display_period;

	/*
	 * AFD in MPEG video. See
	 * - http://en.wikipedia.org/wiki/Active_Format_Description
	 * - http://webapp.etsi.org/workprogram/Report_WorkItem.asp?WKI_ID=21480
	 */
	unsigned char active_format;

	int ui8Mpeg2Dar;
	int par_w, par_h;

	/*
	 * FPA(Frame Packing Arrangement) in MPEG4 video. See
	 * - http://en.wikipedia.org/wiki/H.264/MPEG-4_AVC
	 */
	int frame_packing_arrangement;

	unsigned int stc_discontinuity;
	unsigned int dts;
	unsigned int pts;
	long long timestamp;

	enum
	{
		decoded_buffer_multi_picture_left,
		decoded_buffer_multi_picture_right,
		decoded_buffer_multi_picture_none,
	} multi_picture;

	unsigned long uid;

	void *user_data_addr;
	int user_data_size;
	int top_field_first;
	int repeat_first_field;

	int num_of_buffer_required;

	void *decoder_priv;
};






#endif /* _VDEC_DRV_H_ */

/* vim:set ts=8: */
