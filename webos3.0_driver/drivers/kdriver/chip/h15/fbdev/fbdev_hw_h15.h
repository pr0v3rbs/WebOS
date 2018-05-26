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

#ifndef	_FBDEV_HW_H15_H_
#define	_FBDEV_HW_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define	FBDEV_H15_USE_HDR_RDY_REG	/* use osd header ready flag */
#undef	FBDEV_H15_SW_HDR_RDY_CTRL	/* EXPERIMENTAL - use software based header ready flag control. if you want to use this, please unset FBDEV_USE_HDR_RDY_REG */

#undef	FBDEV_USE_FRC_IF_CTRL		/* enable/diable FRC header generation for SEP.OSD with FRC3 */
#undef	FBDEV_USE_MIF_CTRL			/* use MIF interface */

#define FBDEV_H15_KEEP_LOGO_LAYER	/* keep the logo layer from logo timer */

#define FBDEV_H15_USE_UHD_S3D_AUTO_CONN_PATH    /* enable/disable auto conn path change in UHD board */

#undef  FBDEV_H15_USE_OSD_STATUS_MON /* monitor HW status */

#undef	FBDEV_H15_CPB_BW_TEST		/* support compressed image test */

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <asm/atomic.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	FBDEV_H15_MAX_OSD_NUM			8	/* 4 graphic, 4 cursor */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	FBDEV_H15_NOTE(reason)
#define FBDEV_H15_NOT_IMPL(reason)		0

#define FBDEV_H15_GET_HW_CTX()			(FBDEV_H15_HW_CTX_T*)g_fbdev_hw_ctx.chip_priv_ctx

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/** H15 OSD header structure
 *
 *
 */
typedef struct
{
	UINT32									// OSD[0:3]_HDR0
	osd_hdr_ypos		   			:12,	//  0:11
                       	   			: 4,	// 12:15 reserved
	osd_hdr_xpos           			:12,	// 16:27
                   		   			: 3,	// 28:30 reserved
	osd_hdr_color_key_en   			: 1;	//    31

	UINT32					   				// OSD[0:3]_HDR1
	osd_hdr_h_mem         			:12, 	//  0:11
           			      			: 4, 	// 12:15 reserved
	osd_hdr_w_mem         			:12; 	// 16:27

	UINT32					   				// OSD[0:3]_HDR2
	osd_hdr_h_out         			:12, 	//  0:11
		                  			: 4, 	// 12:15 reserved
	osd_hdr_w_out         			:12, 	// 16:27
                          			: 3, 	// 28:30 reserved
	osd_hdr_pixel_order   			: 1; 	//    31

    UINT32									// OSD[0:3]_HDR3
    osd_hdr_wpl                    :16, 	//  0:15
    osd_hdr_global_alpha           : 8, 	// 16:23
    osd_hdr_format                 : 4, 	// 24:27
    osd_hdr_depth                  : 3, 	// 28:30
    osd_hdr_global_alpha_en        : 1; 	//    31

    UINT32									// OSD[0:3]_HDR4
    osd_hdr_color_key              ;    	// 31: 0

    UINT32									// OSD[0:3]_HDR5
    osd_hdr_ptr_plte               ;    	// 31: 0

    UINT32									// OSD[0:3]_HDR6
    osd_hdr_ptr_bmp                ;		// 31: 0
}
FBDEV_H15_OSD_HDR_T;

typedef struct
{
	UINT8							bits_per_pixel;

}
FBDEV_H15_OSD_EXHDR_T;

/** H15 OSD H/W Controll Handler
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
FBDEV_H15_FRC_IF_CTRL_T;

typedef struct
{
#define	FBDEV_H15_VOSD_OPMASK_NONE			0x0
#define	FBDEV_H15_VOSD_OPMASK_BLEND_ALPHA	0x1
#define	FBDEV_H15_VOSD_OPMASK_DOWN_SCALE	0x2
	UINT32		opmask;			/* operation mask */

	int			osd_idx;		/* current front screen idx for downscale */
	int			my_vsync;		/* vsync control */

	int			osd_surf[2];	/* surfaces to hold downscaled posd image */
	UINT32		fb_mem_ptr[2];	/* physical memory for downscaled image */

	LX_RECT_T	src_rect;		/* input rect = original OSD rect */
	LX_RECT_T	dst_rect;		/* output rect = downscaled OSD rect */
}
FBDEV_H15_VOSD_CTRL_T;

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
	UINT32		comp_opt;

	UINT32		osd_view_status[LX_FBDEV_ID_MAX];
}
FBDEV_H15_FB_COMPOSITOR_T;

typedef struct
{
	int 	(*H15_InitHW)			(void);

	int		(*H15_SetViewCtrl)		(LX_FBDEV_VIEW_CTRL_T*);
	int 	(*H15_SetConnCtrl)		(LX_FBDEV_CONN_CTRL_T*);
	int		(*H15_SetPivotCtrl)		(LX_FBDEV_PIVOT_CTRL_T*);
	int 	(*H15_SetTridCtrl)		(int, LX_FBDEV_TRID_CTRL_T* );

	int 	(*H15_WriteOSDHeader)	(struct scanvas*);
    int		(*H15_SetOSDEnable)		(int,int);
    int		(*H15_GetOSDEnable)		(int);
	void	(*H15_ISRTickHandler)	(void);
}
FBDEV_H15_HW_FUNC_T;

#ifdef FBDEV_H15_SW_HDR_RDY_CTRL
typedef struct
{
	atomic_t	osd_hdr_ready;
	atomic_t	mif_hdr_ready;

	UINT32		osd_hdr[7];
	UINT32		mif_hdr[2];
}
FBDEV_H15_SW_HDR_RDY_CTRL_T;
#endif

typedef struct
{
	struct
	{
		UINT32						width;
		UINT32						height;
	}
	bosd_disp;														/* display size for B.OSD */

	struct
	{
		UINT32						width;
		UINT32						height;
	}
	sosd_disp;														/* display size for S.OSD */

	BOOLEAN							b_keep_hw_conn_type;			/* DO NOT change OSD connection automatically
																	 * if the HW conn type is set to B.OSD, it means that HW doesn't have S.OSD path.
																	 * so we should now change OSD connection when the initial path is B.OSD.
																	 */
	BOOLEAN							b_support_3dosd;				/* support 3D OSD */

	UINT32							hw_conn_type;					/* HW current OSD connection */
	UINT32							hw_disp_width;					/* HW current OSD disp width */
	UINT32							hw_disp_height;					/* HW current OSD disp height */

	UINT32							intr_req_sav;					/* N/A */
	FBDEV_H15_OSD_HDR_T				osd_hdr_sav[LX_FBDEV_ID_MAX];	/* last osd header written */

	LX_FBDEV_CONN_CTRL_T			conn_ctrl[LX_FBDEV_ID_MAX];		/* layer conn ctrl */

	FBDEV_H15_FRC_IF_CTRL_T 		frc_if_ctrl;					/* FRC I/F control */
	FBDEV_H15_VOSD_CTRL_T			vosd_ctrl[LX_FBDEV_ID_MAX];		/* virt OSD ctrl for downscale, global alpha */
	FBDEV_HW_VIDEO_MIXER_CTRL_T		video_mixer_ctrl;				/* video mixer (DPPA) control */
	FBDEV_H15_FB_COMPOSITOR_T		comp_ctrl;						/* SW OSD compositor control */

#ifdef FBDEV_H15_SW_HDR_RDY_CTRL
	FBDEV_H15_SW_HDR_RDY_CTRL_T		sw_hdr_ready_ctrl[FBDEV_H15_MAX_OSD_NUM];	/* software hdr_ready_flag control */
#endif
	FBDEV_H15_HW_FUNC_T				func;
}
FBDEV_H15_HW_CTX_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration (H15CMN)
----------------------------------------------------------------------------------------*/
/* return physical OSD block index by fb_dev_id
 *
 */

#define INVALID_XNUM(x)         ((x)<=0||(x)>2) /* if xnum <= 0 */
#define INVALID_XIDX(x)         ((x)<0)         /* if xidx = -1 */
extern int		FBDEV_H15CMN_GetHWXList	( int fb_dev_id, int xlst[2] );

extern void		FBDEV_H15CMN_WriteHdrRegs	( int fb_dev_id, FBDEV_H15_OSD_HDR_T* osd_hdr, FBDEV_H15_OSD_EXHDR_T* osd_exhdr );
extern void		FBDEV_H15CMN_WriteFRCHeader	( void );

extern int		FBDEV_H15CMN_InitHW			( void );
extern int		FBDEV_H15CMN_InitOSDLayer	( int layer );

extern int		FBDEV_H15CMN_SetOSDEnable	(int layer , int enable);
extern int		FBDEV_H15CMN_GetOSDEnable	(int layer);

extern int		FBDEV_H15CMN_SetViewCtrl	( LX_FBDEV_VIEW_CTRL_T* ctrl );
extern int		FBDEV_H15CMN_SetPivotCtrl	( LX_FBDEV_PIVOT_CTRL_T* ctrl );
extern int		FBDEV_H15CMN_SetConnCtrl	(LX_FBDEV_CONN_CTRL_T* ctrl);
extern int		FBDEV_H15CMN_SetTridCtrl	( int fb_dev_id, LX_FBDEV_TRID_CTRL_T* ctrl );

extern void		FBDEV_H15CMN_HandleVOSD		( int fb_dev_id, struct scanvas* hCanvans, FBDEV_H15_OSD_HDR_T* osd_hdr );
extern void		FBDEV_H15CMN_Handle3DOSD	( int fb_dev_id, struct scanvas* hCanvans, FBDEV_H15_OSD_HDR_T* osd_hdr );
extern void     FBDEV_H15CMN_ISRHook 		( void );
extern void		FBDEV_H15CMN_HandleReverseOSD( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H15_OSD_HDR_T* osd_hdr );

extern void		FBDEV_H15CMN_DoOSDMerge		(int fb_dev_id, BOOLEAN b_flip );

extern int		FBDEV_H15CMN_RunSuspend		(void);
extern int		FBDEV_H15CMN_RunResume		(void);

extern void		FBDEV_H15CMN_RunBootLogoTimer( UINT32 ms, BOOLEAN bForce );

extern void		FBDEV_H15_InitOSDZOrder		(void);

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration (H15 Platform Function)
----------------------------------------------------------------------------------------*/

extern int		 FBDEV_H15_InitHW				(void);
extern int		 FBDEV_H15_ShutdownHW			(void);
extern int		 FBDEV_H15_SetOSDEnable			(int layer , int enable);
extern int		 FBDEV_H15_GetOSDEnable			(int layer);
extern int		 FBDEV_H15_InitOSDLayer			(int layer);
extern int		 FBDEV_H15_WriteOSDHeader		(struct scanvas  *pstCanvas);

extern int		 FBDEV_H15_GetZList				(LX_FBDEV_ZLIST_T* zList );
extern int		 FBDEV_H15_SetZList				(LX_FBDEV_ZLIST_T* zList );
extern int		 FBDEV_H15_SetVideoMixerCtrl	(LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl);

extern int		 FBDEV_H15_WriteOSDEndian		(int layer, ENDIAN_INFORM_T *endian);
extern int		 FBDEV_H15_WriteOSDCSC			(int layer, CSC_INFORM_T *csc);
extern int		 FBDEV_H15_GetInterruptStatus	(INTERRUPT_INFORM_T intr);
extern int		 FBDEV_H15_SetInterruptClear	(INTERRUPT_INFORM_T intr);
extern int		 FBDEV_H15_SetInterruptEnable	(INTERRUPT_INFORM_T intr , int enable);
extern int		 FBDEV_H15_DownloadPalette		(unsigned int addr , unsigned int size , void *data);

extern int		 FBDEV_H15_SetViewCtrl			(LX_FBDEV_VIEW_CTRL_T* ctrl);
extern int		 FBDEV_H15_SetPivotCtrl			(LX_FBDEV_PIVOT_CTRL_T* ctrl);
extern int		 FBDEV_H15_SetConnCtrl			(LX_FBDEV_CONN_CTRL_T* ctrl);
extern int		 FBDEV_H15_SetTridCtrl			(int layer, LX_FBDEV_TRID_CTRL_T* ctrl);

#ifdef	FBDEV_H15_CPB_BW_TEST
extern void		FBDEV_H15TEST_InitCPBTest		(void);
extern void		FBDEV_H15TEST_ISRHandler		(void);
extern BOOLEAN	FBDEV_H15TEST_IsCPBTestMode		(void);
#endif

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_HW_H15_H_ */

/** @} */
