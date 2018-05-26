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
 *  driver interface header for fbdev device. ( used only within kdriver )
 *
 *  @author		raxis
 *  @version	1.0
 *  @date		2009.11.15
 *
 *  @addtogroup lg1150_fbdev
 *	@{
 */

#ifndef	_FBDEV_HW_H13_H_
#define	_FBDEV_HW_H13_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define	FBDEV_USE_HDR_RDY_REG				/* use osd header ready flag */
#define FBDEV_USE_COLD_BOOT_CLS				/* clear boot logo region if necessary */

#undef	FBDEV_USE_FRC_IF_CTRL				/* enable/diable FRC header generation for SEP.OSD with FRC3 */

#define	FBDEV_USE_UHD_S3D_AUTO_CONN_PATH	/* enable/disable auto conn path change in UHD board */

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
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
#define FBDEV_H13_CHK_2K_DISP()			(LX_RES_GET_WIDTH(lx_chip_graphic_res()) < 2048)
#define	FBDEV_H13_CHK_25K_DISP()		(g_fbdev_cfg->uhd_disp_mode && LX_RES_GET_WIDTH(lx_chip_graphic_res()) == 2560)

#define FBDEV_H13_CHK_BOSD_PATH()		(g_fbdev_h13_hw_ctx.conn_ctrl.conn_type == LX_FBDEV_CONN_OSD_VIDEO_BLEND)
#define FBDEV_H13_CHK_SOSD_PATH()		(g_fbdev_h13_hw_ctx.conn_ctrl.conn_type == LX_FBDEV_CONN_OSD_VIDEO_SEPARATE)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/** H13 OSD header structure 
 *
 *
 */
typedef struct
{
			UINT32						// OSD[0:3]_HDR0
			osd_hdr_ypos		   :12,	//  0:11
                               	   : 4,	// 12:15 reserved
			osd_hdr_xpos           :12,	// 16:27
                           		   : 3,	// 28:30 reserved
			osd_hdr_color_key_en   : 1;	//    31

			UINT32					   // OSD[0:3]_HDR1
			osd_hdr_h_mem         :12, //  0:11
           			              : 4, // 12:15 reserved
			osd_hdr_w_mem         :12; // 16:27

			UINT32					   // OSD[0:3]_HDR2
			osd_hdr_h_out         :12, //  0:11
		                          : 4, // 12:15 reserved
			osd_hdr_w_out         :12, // 16:27
                                  : 3, // 28:30 reserved
			osd_hdr_pixel_order   : 1; //    31

    UINT32								// OSD[0:3]_HDR3
    osd_hdr_wpl                    :16, //  0:15
    osd_hdr_global_alpha           : 8, // 16:23
    osd_hdr_format                 : 4, // 24:27
    osd_hdr_depth                  : 3, // 28:30
    osd_hdr_global_alpha_en        : 1; //    31

    UINT32								// OSD[0:3]_HDR4
    osd_hdr_color_key              ;    // 31: 0

    UINT32								// OSD[0:3]_HDR5
    osd_hdr_ptr_plte               ;    // 31: 0

    UINT32								// OSD[0:3]_HDR6
    osd_hdr_ptr_bmp                ;	// 31: 0
}
FBDEV_H13_OSD_HDR_T;

/** H13 FRC Protocol Interface 
 *
 *
 */
typedef struct {
	UINT32
	osd_frc_hdr_format              : 4,	//  0: 3
	osd_frc_hdr_po                  : 4,	//  4: 7
	                                : 2,	//  8: 9 reserved
	osd_frc_lr                      : 1,	//    10
	osd_frc_3d                      : 1,	//    11
	                                : 4,	// 12:15 reserved
	osd_frc_hdr_frame_number		: 4,	// 16:19
	osd_frc_hdr_prefix				: 4;	// 20:23
} 
FBDEV_H13_FRC_HDR_0;

typedef struct {
	UINT32
	osd_frc_hdr_bmp_height          :13,	//  0:12
	                                : 3,	// 13:15 reserved
	osd_frc_hdr_bmp_width           :13;	// 16:28
} 
FBDEV_H13_FRC_HDR_1;

typedef struct {
	UINT32
	osd_frc_hdr_global_alpha        : 8,	//  0: 7
	                                : 4,	//  8:11 reserved
	osd_frc_hdr_pe                  : 1,	//    12
	osd_frc_hdr_ge                  : 1,	//    13
	osd_frc_hdr_ce                  : 1,	//    14
	osd_frc_hdr_ou                  : 1;	//    15
} 
FBDEV_H13_FRC_HDR_2;

typedef struct {
	UINT32
	osd_frc_hdr_ck_b                : 8,	//  0: 7
	osd_frc_hdr_ck_g                : 8,	//  8:15
	osd_frc_hdr_ck_r                : 8;	// 16:23
} 
FBDEV_H13_FRC_HDR_3;

typedef struct {
	UINT32
	osd_frc_hdr_yoffset             :13,	//  0:12
	                                : 3,	// 13:15 reserved
	osd_frc_hdr_xoffset             :13;	// 16:28
} 
FBDEV_H13_FRC_HDR_4;

typedef struct {
	UINT32
	osd_frc_hdr_disp_height         :13,	//  0:12
	                                : 3,	// 13:15 reserved
	osd_frc_hdr_disp_width          :13;	// 16:28
} 
FBDEV_H13_FRC_HDR_5;

typedef struct
{
	UINT8							bits_per_pixel;

}
FBDEV_H13_OSD_EXHDR_T;

/** H13 OSD H/W Caps List 
 *
 *
 *
 *
 *
 */
typedef	enum
{
	FBDEV_H13_HW_CAPS_NONE,		/* none */
}
FBDEV_H13_HW_CAPS_T;

/** H13 OSD H/W Controll Handler
 *
 *
 *
 *
 */
typedef struct
{
	/* each OSD shouild be controlled if FRC sep path or not. is it is controlled by OSD main ? */

	BOOLEAN		b_hdr_ready;		 /* set to TRUE when FRC header is written */
	UINT32		hdr_data[6];         /* FRC header data */
	UINT32		hdr_data_remap[6]; 	 /* FRC header data (after remap) */
}
FBDEV_H13_FRC_IF_CTRL_T;

typedef struct
{
#define	FBDEV_H13_VOSD_OPMASK_NONE			0x0
#define	FBDEV_H13_VOSD_OPMASK_BLEND_ALPHA	0x1
#define	FBDEV_H13_VOSD_OPMASK_DOWN_SCALE	0x2
	UINT32		opmask;			/* operation mask */

	int			osd_idx;		/* current front screen idx for downscale */
	int			my_vsync;		/* vsync control */

	int			osd_surf[2];	/* surfaces to hold downscaled posd image */
	UINT32		fb_mem_ptr[2];	/* physical memory for downscaled image */

	LX_RECT_T	src_rect;		/* input rect = original OSD rect */
	LX_RECT_T	dst_rect;		/* output rect = downscaled OSD rect */
}
FBDEV_H13_VOSD_CTRL_T;

typedef struct 
{
#define	FBDEV_H13_CLUT_VOSD_WIDTH	1376	/* support UHD with 2POSD to meet 16-byte aligned base address L=688, R=688 */
#define	FBDEV_H13_CLUT_VOSD_HEIGHT	768
	int			osd_idx;		/* current front screen idx for downscale */
	int			my_vsync;		/* vsync control */

	int			osd_surf[2];	/* surfaces to hold downscaled image */
	UINT32		fb_mem_ptr[2];	/* physical memory for downscaled image */
}
FBDEV_H13_CLUT_VOSD_CTRL_T;

typedef struct
{
	BOOLEAN		b_video_mix;	/* if TRUE, OSD(DPPA) will mix video & OSD */
}
FBDEV_HW_VIDEO_MIXER_CTRL_T;

typedef struct
{
	int			my_vsync;
	int			fb_dev_id;		/* OSD Id used for merged image */
	OS_SEM_T	mtx;

	int			width;			/* osd width */
	int			height;			/* osd width */
	int			merge_surf;

	int			out_width;
	int			out_height;
#define FB_COMP_OUT_FB_NUM	2	/* 2:double buffer 3:tripple buffer */
	int			view_idx;		/* view/draw frame ctrl */
	int			out_fb_surf[FB_COMP_OUT_FB_NUM]; /* surface fd used as output frame buffer */
	UINT32		out_fb_mem[FB_COMP_OUT_FB_NUM];	/* physical addr used as ouptut frame buffer */

#define FB_COMP_OPT_NONE		0x0000
#define FB_COMP_OPT_ROTATE_90	0x0001
#define FB_COMP_OPT_ROTATE_270	0x0002
#define FB_COMP_OPT_FHD			0x0010	/* not used */
#define FB_COMP_OPT_UHD			0x0020	/* not used */
	UINT32			comp_opt;

	UINT32			osd_view_status[LX_FBDEV_ID_MAX];
}
FBDEV_H13_FB_COMPOSITOR_T;

typedef struct
{
	UINT32							intr_req_sav;					/* N/A */
	FBDEV_H13_OSD_HDR_T				osd_hdr_sav[LX_FBDEV_ID_MAX];	/* last osd header written */

	FBDEV_H13_FRC_IF_CTRL_T 		frc_if_ctrl;					/* FRC I/F control */
	FBDEV_H13_VOSD_CTRL_T			vosd_ctrl[LX_FBDEV_ID_MAX];		/* virt OSD ctrl for downscale, global alpha */
	FBDEV_H13_CLUT_VOSD_CTRL_T		clut_vosd_ctrl[LX_FBDEV_ID_MAX];/* virt OSD ctrl for clut */

	FBDEV_HW_VIDEO_MIXER_CTRL_T		video_mixer_ctrl;				/* video mixer (DPPA) control */
	FBDEV_H13_FB_COMPOSITOR_T		comp_ctrl;						/* SW OSD compositor control */

	LX_FBDEV_CONN_CTRL_T			conn_ctrl;						/* OSD connection control */

	void 	(*H13_InitHW)			(void);

	int		(*H13_SetViewCtrl)		(LX_FBDEV_VIEW_CTRL_T*);
	int 	(*H13_SetConnCtrl)		(LX_FBDEV_CONN_CTRL_T*);
	int		(*H13_SetPivotCtrl)		(LX_FBDEV_PIVOT_CTRL_T*);
	int 	(*H13_SetTridCtrl)		(int, LX_FBDEV_TRID_CTRL_T* );

	int 	(*H13_WriteOSDHeader)	(struct scanvas*);
    int		(*H13_SetOSDEnable)		(int,int);
    int		(*H13_GetOSDEnable)		(int);
	void	(*H13_ISRTickHandler)	(void);
}
FBDEV_H13_HW_CTX_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration (H13CMN)
----------------------------------------------------------------------------------------*/
extern void		FBDEV_H13CMN_WriteHdrRegs	( int fb_dev_id, FBDEV_H13_OSD_HDR_T* osd_hdr, FBDEV_H13_OSD_EXHDR_T* osd_exhdr );
extern void		FBDEV_H13CMN_WriteFRCHeader	( void );

extern int		FBDEV_H13CMN_InitOSDLayer	( int layer );

extern int		FBDEV_H13CMN_SetOSDEnable	(int layer , int enable);
extern int		FBDEV_H13CMN_GetOSDEnable	(int layer);

extern int		FBDEV_H13CMN_SetViewCtrl	( LX_FBDEV_VIEW_CTRL_T* ctrl );
extern int		FBDEV_H13CMN_SetPivotCtrl	( LX_FBDEV_PIVOT_CTRL_T* ctrl );
extern int		FBDEV_H13CMN_SetConnCtrl	(LX_FBDEV_CONN_CTRL_T* ctrl);
extern int		FBDEV_H13CMN_SetTridCtrl	( int fb_dev_id, LX_FBDEV_TRID_CTRL_T* ctrl );

extern void		FBDEV_H13CMN_HandleCLUT		( int fb_dev_id, struct scanvas* hCanvans, FBDEV_H13_OSD_HDR_T* osd_hdr );
extern void		FBDEV_H13CMN_HandleVOSD		( int fb_dev_id, struct scanvas* hCanvans, FBDEV_H13_OSD_HDR_T* osd_hdr );
extern void		FBDEV_H13CMN_Handle3DOSD	( int fb_dev_id, struct scanvas* hCanvans, FBDEV_H13_OSD_HDR_T* osd_hdr );
extern void     FBDEV_H13CMN_HandleISR 		( void );
extern void		FBDEV_H13CMN_HandleReverseOSD( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H13_OSD_HDR_T* osd_hdr );

extern void		FBDEV_H13CMN_DoOSDMerge		(int fb_dev_id, BOOLEAN b_flip );

extern int		FBDEV_H13CMN_RunSuspend		(void);
extern int		FBDEV_H13CMN_RunResume		(void);

extern void		FBDEV_H13CMN_RunBootLogoTimer( UINT32 ms, BOOLEAN bForce );

extern void		FBDEV_H13_InitOSDZOrder		(void);

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration (H13 Platform Function)
----------------------------------------------------------------------------------------*/

extern int		 FBDEV_H13_InitHW				(void);
extern int		 FBDEV_H13_ShutdownHW			(void);
extern int		 FBDEV_H13_SetOSDEnable			(int layer , int enable);
extern int		 FBDEV_H13_GetOSDEnable			(int layer);
extern int		 FBDEV_H13_InitOSDLayer			(int layer);
extern int		 FBDEV_H13_WriteOSDHeader		(struct scanvas  *pstCanvas);

extern int		 FBDEV_H13_GetZList				(LX_FBDEV_ZLIST_T* zList );
extern int		 FBDEV_H13_SetZList				(LX_FBDEV_ZLIST_T* zList );
extern int		 FBDEV_H13_SetVideoMixerCtrl	(LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl);

extern int		 FBDEV_H13_WriteOSDEndian		(int layer, ENDIAN_INFORM_T *endian);
extern int		 FBDEV_H13_WriteOSDCSC			(int layer, CSC_INFORM_T *csc);
extern int		 FBDEV_H13_GetInterruptStatus	(INTERRUPT_INFORM_T intr);
extern int		 FBDEV_H13_SetInterruptClear	(INTERRUPT_INFORM_T intr);
extern int		 FBDEV_H13_SetInterruptEnable	(INTERRUPT_INFORM_T intr , int enable);
extern int		 FBDEV_H13_DownloadPalette		(unsigned int addr , unsigned int size , void *data);

extern int		 FBDEV_H13_SetViewCtrl			(LX_FBDEV_VIEW_CTRL_T* ctrl);
extern int		 FBDEV_H13_SetPivotCtrl			(LX_FBDEV_PIVOT_CTRL_T* ctrl);
extern int		 FBDEV_H13_SetConnCtrl			(LX_FBDEV_CONN_CTRL_T* ctrl);
extern int		 FBDEV_H13_SetTridCtrl			(int layer, LX_FBDEV_TRID_CTRL_T* ctrl);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	FBDEV_H13_HW_CTX_T				g_fbdev_h13_hw_ctx;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_HW_H13_H_ */

/** @} */
