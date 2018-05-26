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

/** @file fbdev_kapi.h
 *
 *  application interface header for osd device
 *
 *  @author      daeyoung lim(raxis.lim@lge.com)
 *  @version		1.0
 *  @date		2010.02.01
 *
 *  @addtogroup lg1150_fbdev
 *	@{
 */

#ifndef	_FBDEV_KAPI_H_
#define	_FBDEV_KAPI_H_

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
#define	FBDEV_IOC_MAGIC		'F'
/**
@name FBDEV IOCTL List
IOCTL list for osd device.

 @{
 @def FBDEV_IOR_GET_VSCREEN
	see FBIOGET_VSCREENINFO in linux/fb.h.

 @def FBDEV_IOW_SET_VSCREEN
	see FBIOGET_VSCREENINFO in linux/fb.h.

 @def FBDEV_IOR_GET_FSCREEN
	see FBIOGET_FSCREENINFO in linux/fb.h.

 @def FBDEV_IOR_CHIP_REV_INFO
 	Reads chip revision information.
 	Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.
	@param LX_CHIP_REV_INFO_T [OUT] chip version data
	@return RET_OK

 @def FBDEV_IOW_SET_PALETTE
	@param pointer to LX_FBDEV_PALETTE_T
 	@return RET_OK for success, none zero for otherwise

 @def FBDEV_IOR_GET_CONFIG
 	get device configuration ( this IOCTL overrides FBIOGET_VSCREENINFO )
	@param [OUT] pointer to LX_FBDEV_CONFIG_T
  	@return RET_OK for success, none zero for otherwise

  	@see FBDEV_IOR_GET_VSCREEN

 @def FBDEV_IOW_SET_CONFIG
 	update device configuration ( this IOCTL overrides FBIOPUT_VSCREENINFO )
 	@param [IN] pointer to LX_FBDEV_CONFIG_T
 	@return RET_OK for success, none zero for otherwise

 	This IOCTL update the whole values in FB device.
 	Before using this IOCTL, user should get the latest configuration by FBDEV_IOR_GET_CONFIG.\n
 	DO NOT manage LX_FBDEV_CONFIG_T as your local variable.

 	@see FBDEV_IOW_SET_VSCREEN

 @def FBDEV_IOR_GET_POSITION
	@brief get current position information of FB device.
	@param LX_POSITION_T [OUT] position information of FB device
 	@return RET_OK for success, none zero for otherwise

 @def FBDEV_IOW_SET_POSITION
 	@brief set new position information of FB device.
	@param LX_POSITION_T [in] position information of FB device
 	get new position information of FB device.

 	@return RET_OK for success, none zero for otherwise

 	This IOCTRL is only effective when device type is CURSOR.\n
 	This IOCTRL causes screen update.

 @def FBDEV_IOR_GET_PHY_MEMORY
 	@brief get the physical memory information of the FB device.
 	@param LX_FBDEV_PHY_MEMORY_T [out] memory information

 	@return RET_OK for success, none zero for otherwise

	@note Front and back screen is changed every pan_display call.
		  So you should call this ioctl when you need to access OSD.

 @def FBDEV_IOW_SET_VISIBLE
	@brief change the visibility of each OSD layer
	@param UINT32 [IN] TRUE if visiable, FALSE if hidden
 	@return RET_OK for success, none zero for otherwise

 @def FBDEV_IOR_GET_VISIBLE
	@brief get the visibility of each OSD layer
	@param UINT32 [OUT] status of visibility
 	@return RET_OK for success, none zero for otherwise

 @def FBDEV_IOW_SET_FREEZE_CTRL
	@brief if set, OSD update by pan_display is stopped. this should be used very carefully

 @def FBDEV_IOR_GET_VIEW_CTRL
	@brief get current view status of OSDs

 @def FBDEV_IOW_SET_VIEW_CTRL
	@brief set view status of OSDs all together

 @def FBDEV_IOW_FLIP_FB_SCREEN
	@brieif show user defiend framebuffer. framebuffer is fully managed by application.

 @def FBdEV_IOW_EXEC_BATCH_RUN
	@brief	execute the multiple job within the same vsync interval.
*/

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define FBDEV_IOR_GET_VSCREEN			FBIOGET_VSCREENINFO
#define FBDEV_IOW_SET_VSCREEN			FBIOPUT_VSCREENINFO
#define FBDEV_IOR_GET_FSCREEN			FBIOGET_FSCREENINFO

#define	FBDEV_IOC_IDX(i)				(0x20+(i))

#define FBDEV_IOR_CHIP_REV_INFO			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 0), LX_CHIP_REV_INFO_T )
#define	FBDEV_IOR_GET_CFG				_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 1), LX_FBDEV_CFG_T )

#define FBDEV_IOW_SET_PALETTE			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 2), LX_FBDEV_PALETTE_T )
#define FBDEV_IOR_GET_CONFIG			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 3), LX_FBDEV_CONFIG_T )
#define FBDEV_IOW_SET_CONFIG			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 4), LX_FBDEV_CONFIG_T )
#define FBDEV_IOR_GET_POSITION			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 5), LX_POSITION_T )
#define FBDEV_IOW_SET_POSITION			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 6), LX_POSITION_T )
#define FBDEV_IOW_SET_CURSOR_POSITION	_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX( 7), LX_FBDEV_CURSOR_POSITION_T )

#define FBDEV_IOR_GET_VIEW_CTRL			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(10), LX_FBDEV_VIEW_CTRL_T )
#define FBDEV_IOW_SET_VIEW_CTRL			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(11), LX_FBDEV_VIEW_CTRL_T )

#define FBDEV_IOW_SET_VSYNC_EVENT		_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(12), UINT32 )

#define FBDEV_IOR_GET_PHY_MEMORY		_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(20), LX_FBDEV_PHY_MEMORY_T )
#define FBDEV_IOR_GET_VISIBLE			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(21), UINT32 )
#define FBDEV_IOW_SET_VISIBLE			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(22), UINT32 )
#define FBDEV_IO_WAITFORVSYNC			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(23), UINT32 )
#define FBDEV_IORW_GET_ZLIST			_IOWR(FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(24), LX_FBDEV_ZLIST_CTRL_T )
#define FBDEV_IOW_SET_ZLIST				_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(25), LX_FBDEV_ZLIST_CTRL_T )
#define FBDEV_IOW_WAITFORVSYNC_ON_FLIP	_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(26), int )
#define FBDEV_IOW_SET_VIDEO_MIXER_CTRL	_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(27), LX_FBDEV_VIDEO_MIXER_CTRL_T )

#define FBDEV_IOW_FLIP_FB_IMAGE			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(29), LX_FBDEV_FB_IMAGE_T )
#define FBDEV_IOW_UPDATE_FB_SCREEN		_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(30), LX_FBDEV_FB_SCREEN_T )
#define FBDEV_IOR_GET_FB_STATUS			_IOR( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(32), LX_FBDEV_FB_STATUS_T )

#define FBDEV_IOW_SET_TRID_CTRL			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(42), LX_FBDEV_TRID_CTRL_T )
#define FBDEV_IOW_SET_CONN_CTRL			_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(43), LX_FBDEV_CONN_CTRL_T )
#define FBDEV_IOW_SET_FREEZE_CTRL		_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(45), LX_FBDEV_FREEZE_CTRL_T )
#define FBDEV_IOW_SET_PIVOT_CTRL		_IOW( FBDEV_IOC_MAGIC, FBDEV_IOC_IDX(46), LX_FBDEV_PIVOT_CTRL_T )

#define FBDEV_IOC_MAXNR					256
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	LX_FBDEV_SCREEN_PER_DEVICE	2		/**< L8 S/W configured to double buffering mode */

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/** OSD ID definition.
 *
 *	OSD H/W support 4 physical OSD layer.
 *
 * @see FBDEV_MAX_DEVICE
*/
typedef enum
{
	LX_FBDEV_ID_OSD0	= 0,	/**< general purpose OSD */
	LX_FBDEV_ID_OSD1	= 1,	/**< general purpose OSD */
	LX_FBDEV_ID_OSD2	= 2,	/**< general purpose OSD ( may not be supported by H/W ) */
	LX_FBDEV_ID_OSD3	= 3,	/**< cursor only */
	LX_FBDEV_ID_CSR0	= 3,	/**< cursor only */
	LX_FBDEV_ID_MAX,
}
LX_FBDEV_ID_T;

#define LX_FBDEV_IS_VALID_ID(fb_dev_id)		((fb_dev_id)>=LX_FBDEV_ID_OSD0 && (fb_dev_id)<LX_FBDEV_ID_MAX)

/** OSD path definition
 *
 *	OSD H/W can route its image to either LEFT path or RIGHT path or both path.
 *  path route is each SOC specific, so you should contact to OSD H/W engineer.
 *
 *	L9 spec)
 *	Left  path is routed to the blender with video ( it's called normally as "blend mode" )
 *	Right path is routed to the direct FRC input   ( it's called normally as "separated mode" )
 *
 *
 *	H13 spec)
 *	Not available now
 *
 */
typedef enum
{
	LX_FBDEV_PATH_LEFT		= 0x1,	/**< left path id.  path definition is specific for each soc model */
	LX_FBDEV_PATH_RIGHT 	= 0x2,	/**< right path id. path definition is specific for each soc model */
	LX_FBDEV_PATH_ALL		= LX_FBDEV_PATH_LEFT|LX_FBDEV_PATH_RIGHT,	/**< both left/right path */
}
LX_FBDEV_PATH_T;

/** OSD driver configuration
 *
 * @see FBDEV_IOR_GET_CFG
 */
typedef struct
{
    UINT32          b_use_irq:1,            /**< OSD ISR enabled ? */
                    b_gpu_clock_enable:1,   /**< 3D GPU clock enabled ? */
								:2,			/**< ... */
                    cursor_layer:2,         /**< which OSD is used as cursor  */
					uhd_disp_mode:1,		/**< UHD OSD display is enabled ? */
                   	:25;

	UINT16			display_width;			/**< display panel width */
	UINT16			display_height;			/**< display panel height */

    struct
    {
        UINT32 		b_little_endian		:1,	/**< OSD data is assumed to little endian ? */
                    b_show_on_startup	:1,	/**< OSD is shown at startup ? */
                    b_use_vsync			:1,	/**< OSD updated at vsync */
                    b_use_global_alpha	:1,	/**< OSD uses global alpha (ignore pixel alpha) ? */
                   	b_active			:1,	/**< OSD is active ? */
                    max_buf_num			:3,	/**< OSD uses double buffering */
                    bits_per_pixel		:8,	/**< bits per pixel */
					pixel_format		:8, /**< pixel format 	*/
					b_gfx_lock_req		:1, /**< OSD is using GFX block for some H/W workaround. so application should consider GFX lock when requesting FB operation */
					b_use_trid_hw		:1,
					b_use_builtin_fbmem :1,	/**< OSD contains dedicated FB memory ? */
                    rsvd:5;

        UINT16      max_xres;               /**< maximum width */
        UINT16      max_yres;               /**< maximum height */
        UINT32      max_size;               /**< maximum buffer size. it's calculated */
    }
    fb[LX_FBDEV_ID_MAX];					/**< increase max item to 8 to hook the future feature */

	UINT32			fb_mem_base;			/**< FBDEV memory base address. set during system initialization */
	UINT32			fb_mem_size;			/**< FBDEV memory size. set during system initialization */
}
LX_FBDEV_CFG_T;

/** OSD pixel type
 *
 *
 *	@note we've tested only RGB mode since modern graphics doesn't use YCbCr mode.
 */
typedef enum
{
	LX_FBDEV_PIXEL_FORMAT_YCBCR_655		= 0,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_2644	= 1,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_4833	= 2,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_6433	= 3,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_888		= 4,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_8655	= 5,
	LX_FBDEV_PIXEL_FORMAT_YCBCR_8888	= 6,

	LX_FBDEV_PIXEL_FORMAT_RGB_565		= 8,
	LX_FBDEV_PIXEL_FORMAT_RGB_1555		= 9,
	LX_FBDEV_PIXEL_FORMAT_RGB_4444		= 0xa,
	LX_FBDEV_PIXEL_FORMAT_RGB_6343		= 0xb,
	LX_FBDEV_PIXEL_FORMAT_RGB_888		= 0xc,
	LX_FBDEV_PIXEL_FORMAT_RGB_8565		= 0xd,
	LX_FBDEV_PIXEL_FORMAT_RGB_8888		= 0xe,
}
LX_FBDEV_PIXEL_FORMAT_T;

/** device type
*/
typedef enum
{
	LX_FBDEV_TYPE_OSD		= 0,				/**< layer type is for normal "OSD" */
	LX_FBDEV_TYPE_CURSOR	= 1,				/**< layer type is for cursor */
}
LX_FBDEV_TYPE_T;

/* for old code */
#define LX_FBDEV_POSITION_T     LX_POSITION_T
#define LX_FBDEV_DIMENSION_T    LX_DIMENSION_T

typedef struct
{
	LX_DIMENSION_T	size;						/* cursor size. should be less than 128 */
	LX_POSITION_T	hotspot;					/* hotspot position */
	LX_POSITION_T	pos;						/* cursor position */
}
LX_FBDEV_CURSOR_POSITION_T;

/** physical memory information of FB device.
 *	This structure is used to pass physical memory information to GFX engine.
 *	[note] since L8 supports double buffering mode by default, application should split memory region into two parts.
 *
 */
typedef struct
{
	UINT32		memptr;							/**< physical memory address */
	UINT32		length;							/**< memory length in byte */
	UINT32		stride;							/**< stride information */
}
LX_FBDEV_PHY_MEMORY_T;

/** device configuration

*/
typedef struct
{
	/** FB device screen info
	 *  xres, yres, xres_virtual, yres_virtual, bits_per_pixel is provided for referencing current configuration.
	 *  they are read-only variables, you can't update these variables with FBDEV_IOW_SET_CONFIG.
	 */
	struct
	{
		UINT16					xres;
		UINT16					yres;
		UINT16					xres_virtual;
		UINT16					yres_virtual;
		UINT32					bits_per_pixel;
	}
	fbinfo;

	/** input window information
	 *	valid range is from 0 to area of input canvas
	 */
	LX_RECT_T				input_win;

	/** output window information
	 *	value range is from 0 to 1920x1080 sized display area.
	 *  [note] output_win should be larger than input_win.
	 */
	LX_RECT_T				output_win;

	UINT32					color_key_enable:1,		///< enable/disable color key.

	/** enable/disable global alpha
	 * When enabled, device driver should use global_alpha to control layer trasparency.
	 * All alpha value in each pixel is replaced by global_alpha value.
	 */
							global_alpha_enable:1,

	/** enable/disable fade effect
	 * When enabled, device driver should use fade_alpha to modulate each OSD pixel and geneates
	 * each pixel is blended (modulated) with blend_alpha value.
	 */
							blend_alpha_enable:1,

	/** enable/disable vsync wait
	 * When enabled, device driver should wait vsync for every flip operation.
	 */
							vsync_enable:1,

	/** enable/disable reverse enable
	 * When ebaled, OSD screen will be shown up-side down
	 */
							reverse_screen_enable:1,
							:27;

	UINT8					global_alpha;			//< layer global alpha when global_alpha_enable is TRUE
													//	 ( 0: full transparent ~ 0xff: opaque - default )\n
	UINT8					blend_alpha;			//< blend alpha when alpha_channel_enable is TRUE
													//   ( 0 ~ 0xff )\n
	UINT8					rsvd_x[6];

	UINT32					color_key;				///< color key when color_key_enable is TRUE.\n
													///	 <b> 2010/02/23 </b>
													///	 L8 replaces alpha field of each pixel with global_alpha
													///	 when global_alpha_enable is TRUE.
													///	 So you cannot apply color_key exactly when both global_alpha
													///	 and color_key are enabled since alpha field is modified
													///	 by system.
													///	 So L8 device driver ignores alpha field of color_key data
													///	 when global alpha is enabled. ( This is a simple wrapround )
	UINT8					rsvd_y[3];
}
LX_FBDEV_CONFIG_T;

/** FBDEV status
 *
 *
 */
typedef struct
{
	UINT32		view_fb_phys_addr;					///	physical address of current view screen
	UINT32		b_visible:1,						/// current POSD is visible or not
				:31;

	UINT32		rsvd[2];
}
LX_FBDEV_FB_STATUS_T;

/** structure for CLUT8 palette.

 palette is used only when FB device is configured to indexed colors.
 for 256 indexed colors, normally you should pass 256 palette data.
 But it is possible for you to use less than 256 colors.\n
 pixel format in palette should be the same as pixel format of FB device.\n
 (note) Let's discuss later if we support translation of pixel format when some other pixel format is provided.
*/
typedef struct
{
	int						palette_num;				///< total number of palette entry
	UINT32 USER_PTR 		*palette_data;				///< pointer to palette data
}
LX_FBDEV_PALETTE_T;

/** zorder defintion for FBDEV
 *
 * order[n] is layerd to top position than order[n+1]
 *
 * @note cursor layer is not change its order since it is always topmost layer.
 * @note be careful when application wants to raise zorder of video layer over osd layer.
 *		 in that case, application should also configure video mixer parameter using FBDEV_IOW_SET_VIDEO_MIXER_CTRL
 *
 * @see  FBDEV_IOW_SET_VIDEO_MIXER_CTRL
 *
 */
typedef struct
{
#define	LX_FBDEV_ZLIST_OSD0		0
#define	LX_FBDEV_ZLIST_OSD1		1
#define	LX_FBDEV_ZLIST_OSD2		2		/**< POSD2. POSD2 is removed. DO NOT use this */
#define	LX_FBDEV_ZLIST_VIDEO	3
#define LX_FBDEV_ZLIST_NONE		0xff
#define	LX_FBDEV_ZLIST_MAX		3		/**< POSD0, POSD1, VIDEO can be controlled */
	LX_FBDEV_PATH_T				path;
	LX_FBDEV_ID_T				order[LX_FBDEV_ZLIST_MAX];
}
LX_FBDEV_ZLIST_T, LX_FBDEV_ZLIST_CTRL_T;

/** OSD connection (output) ctrl
 *
 */
typedef struct
{
#define	LX_FBDEV_CONN_STD						0x0000		/* STD resolution */
#define	LX_FBDEV_CONN_UD						0x0001		/* UD resolution */
#define LX_FBDEV_CONN_LVDS						0x0010		/* LVDS */
#define LX_FBDEV_CONN_FRCIII					0x0020		/* FRCIII */
#define LX_FBDEV_CONN_OSD_VIDEO_SEPARATE		0x0100		/* OSD and VIDEO is separated to the different path */
#define	LX_FBDEV_CONN_OSD_VIDEO_BLEND			0x0200		/* OSD and VIDEO is bleneded and feeded to the same path */

#define	LX_FBDEV_CONN_STD_LVDS					(LX_FBDEV_CONN_STD|LX_FBDEV_CONN_LVDS  |LX_FBDEV_CONN_OSD_VIDEO_BLEND)
#define LX_FBDEV_CONN_STD_FRCIII_SEPARATE		(LX_FBDEV_CONN_STD|LX_FBDEV_CONN_FRCIII|LX_FBDEV_CONN_OSD_VIDEO_SEPARATE)
#define LX_FBDEV_CONN_STD_FRCIII_BLEND			(LX_FBDEV_CONN_STD|LX_FBDEV_CONN_FRCIII|LX_FBDEV_CONN_OSD_VIDEO_BLEND)
#define LX_FBDEV_CONN_UD_FRCIII_BLEND			(LX_FBDEV_CONN_UD |LX_FBDEV_CONN_FRCIII|LX_FBDEV_CONN_OSD_VIDEO_BLEND)
	UINT32				conn_type;
}
LX_FBDEV_CONN_CTRL_T;

/** 3D control structure
 *
 *
 *
 */
typedef struct
{
#define	LX_FBDEV_TRID_MODE_NONE			0x00		/** 3D mode off */
#define	LX_FBDEV_TRID_MODE_2D			0x00		/** 3D mode none */
#define	LX_FBDEV_TRID_MODE_TB			0x10		/** 3D mode T/B */
#define	LX_FBDEV_TRID_MODE_SS			0x20		/** 3D mode S/S */
#define LX_FBDEV_TRID_MODE_2D_3D		0x40		/** 3D mdoe (2Dto3D) */

#define LX_FBDEV_TRID_MODE_EXT_UP		0x00		/** Up   effect (default) */
#define LX_FBDEV_TRID_MODE_EXT_DOWN		0x01		/** Down effect */
	UINT16				mode;
	UINT16				disparity;					/** 0 ~ 0x40(64)	*/
}
LX_FBDEV_TRID_CTRL_T;

typedef struct
{
#define	LX_FBDEV_FREEZE_MASK_NONE			0x0000
#define	LX_FBDEV_FREEZE_MASK_PAN_DISPLAY	0x0001
	UINT16	opmask;
	UINT8	onoff;
}
LX_FBDEV_FREEZE_CTRL_T;

typedef struct
{
#define LX_FBDEV_PIVOT_MODE_NONE	0
#define LX_FBDEV_PIVOT_MODE_90_CW	1
#define LX_FBDEV_PIVOT_MODE_180_CW	2
#define LX_FBDEV_PIVOT_MODE_270_CW	3
	UINT32	mode;
}
LX_FBDEV_PIVOT_CTRL_T;

typedef struct
{
	LX_FBDEV_PATH_T		path;			/**< which path (Left, Right, All) */
	UINT8				b_enable;		/**< BOOLEAN. enable video mixer or not */
#define	LX_FBDEV_VIDEO_MIXER_ID_MAX	2	/**< LG soc supports the max 3 independent video mixer. but it's enough to support only two region */
	UINT8				mix_id;			/**< mixer window id */
	LX_RECT_T			mix_win;		/**< rectangular information for video mixer area. its valid region is (0,0,1920,1080) */
}
LX_FBDEV_VIDEO_MIXER_CTRL_T;

typedef struct
{
	UINT8				view_status[LX_FBDEV_ID_MAX];	/***< each slot means FB index. slot value means on/off status.
															FALSE: off, TRUE: on, 0xff(other value): hold current status */
}
LX_FBDEV_VIEW_CTRL_T;

typedef struct
{
	UINT32				phys_addr;		/**< physical address of frame buffer */
	UINT32				stride;			/**< stride info of frame buffer */
	UINT8				bits_per_pixel;	/**< bits per pixel */
	UINT32				height;			/**< height -- not used */

//	UINT8				pixel_format; 	/**< pixel format - not used. ARGB only */
}
LX_FBDEV_FB_IMAGE_T;

/** non FB screen update description
 *
 *
 */
typedef struct
{
#define	LX_FBDEV_SCREEN_OPMASK_NONE			0x00
#define	LX_FBDEV_SCREEN_OPMASK_IMG_CTRL		0x01	/* enable screen update */
#define	LX_FBDEV_SCREEN_OPMASK_LAYER_CTRL	0x02	/* enable layer update */
#define	LX_FBDEV_SCREEN_OPMASK_TRID_CTRL	0x04	/* enable 3d update */
	UINT16					opmask;

	LX_FBDEV_FB_IMAGE_T		img_ctrl;
	LX_FBDEV_CONFIG_T		layer_ctrl;
	LX_FBDEV_TRID_CTRL_T	trid_ctrl;
}
LX_FBDEV_FB_SCREEN_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_DRV_H_ */

/** @} */
