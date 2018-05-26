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

#ifndef	_FBDEV_IMPL_H_
#define	_FBDEV_IMPL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fb.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "fbdev_kapi.h"
#include "fbdev_cfg.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define MODULE_NAME         	"lg115x_fb"
#define OSD0_FBNAME				"osd0_fb"
#define OSD1_FBNAME				"osd1_fb"
#define OSD2_FBNAME				"osd2_fb"
#define OSD3_FBNAME				"crsr_fb"
#define OSD_CURSOR_FBNAME   	OSD3_FBNAME

#define is_win(name, x) 		((strcmp(name, x ## _FBNAME) == 0) ? 1 : 0)

#define	BYTES_PER_WORD			8		/* 1 bitmap word =  8 byte */
#define BITS_PER_WORD			64		/* 1 bitmap word = 64 bit  */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define FBDEV_ERROR(format,args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_ERROR, format ,##args)
#define FBDEV_WARN(format, args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_WARNING, format ,##args)
#define FBDEV_NOTI(format, args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_NOTI, format ,##args)
#define FBDEV_INFO(format, args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_INFO, format ,##args)
#define FBDEV_DEBUG(format,args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_DEBUG, format, ##args)
#define FBDEV_PRINT(format,args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_DEBUG, format, ##args)
#define FBDEV_TRACE(format,args...)		DBG_PRINT( g_fbdev_debug_fd, LX_LOGM_LEVEL_TRACE, format, ##args)

#define FBDEV_HW_DEBUG(format,args...)	DBG_PRINT( g_fbdev_debug_fd, 10 /* bit[10] */, format, ##args)
#define FBDEV_ISR_DEBUG(format,args...)	DBG_PRINT( g_fbdev_debug_fd, 11 /* bit[11] */, format, ##args)

#define FBDEV_TRACE_BEGIN()
#define FBDEV_TRACE_END()
#define FBDEV_TRACE_MARK()

#define FBDEV_CHECK_DEBUG(__checker,__if_action,fmt,args...)		\
             __CHECK_IF_ERROR(__checker, FBDEV_DEBUG, __if_action , fmt, ##args )

#define FBDEV_CHECK_CODE(__checker,__if_action,fmt,args...)		\
             __CHECK_IF_ERROR(__checker, FBDEV_WARN, __if_action , fmt, ##args )

#define FBDEV_CHECK_ERROR(__checker,__if_action,fmt,args...)	\
             __CHECK_IF_ERROR(__checker, FBDEV_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct fb_var_screeninfo    FBDEV_VAR_SCREEN_T;
typedef struct fb_fix_screeninfo    FBDEV_FIX_SCREEN_T;

typedef struct _FBDEV_CTX_T			FBDEV_CTX_T;
typedef struct _FBDEV_WIN_CTX_T		FBDEV_WIN_CTX_T;

struct scanvas
{
	UINT8			fb_dev_id;

	ULONG			osd_hdr_addr;		/* ioremapped (virtual) address of OSD header */

	ULONG			osd_plte_paddr;		/* physical address of palette data.
										   WORD(64 bit) Pointer to Palette allocated in SDRAM  */
	ULONG			osd_bm_paddr;		/* physical address of current bitmap data.
										   WORD(64 bit) Pointer to Image (Y Component) allocated in SDRAM  */

	UINT8			bits_per_pixel;		/* pixel depth, 1,2,8,16,32... */
	UINT32			stride;				/* byte size of single line. it is humna reable value of usWordsPerLine */

	LX_RECT_T		input_win;			/* input window is used to determine area within OSD bitmap data.
										 * when updating screen, driver finds the start address of real base
										 * address to be displayed with input_win.
										 * input_win shoule be area inside of the real OSD bitmap data.
										 * for example, if OSD bitmap data is 1366x768, input_win area should
										 * be located within (0,0,1366,768).
										 * kdriver also uses xoffset and yoffset for pan_display requet.
										 * simple equation to calucate the base address is :
										 *
										 * base_address = ( bitmap_addr + (input_win.y + yoffset) * bitmap_stride
										 *				    + ( input_win.x + xoffset )
										 */

	LX_RECT_T		output_win;			/* output window is used to determine area within display region.
										 * output window is mapped to the real display ( normally 1920x1080 )
										 * by customizing both input_win and output_win, we can display any area
										 * of bitmap data onto any area of display region.
										 *
										 * output_win data is direactly written to the L8 OSD register.
										 */

	UINT8			color_key_en;		/* Color Key Enable */
	UINT8			pixel_order_en;		/* Pixel order Enable */
	UINT8			global_alpha_en; 	/* Global Alpha for 0 and 4 Pixel format */
	UINT8			blend_alpha_en;		/* each pixel is blended with blend_alpha */
	UINT8			reserver_screen_en;	/* Reverse display mode enabled ? */

	UINT32*			palette;			/* palette is allocated at initialiation time */
	UINT32			color_key;			/* 32bit colorkey */
	UINT8 	 		global_alpha;		/* 8bit global alpha */
	UINT8			blend_alpha;		/* 8Bit blend alpha  */

	LX_DIMENSION_T	csr_size;			/* valid only for cursor layer. user defined cursor size */
	LX_POSITION_T	csr_hotspot;		/* valid only for cursor layer. user defined hotspot position */

	FBDEV_WIN_CTX_T* dm;				/* link to image memory info (not used) */
};

/** context strucutre for each FBDEV(OSD) device
 *	this strut contains HW side OSD parameters.
 *
 */
struct _FBDEV_WIN_CTX_T
{
	UINT8			fb_dev_id;				/* FB Id */
	UINT8			linux_fb_mode;			/* if set, linux fb interface will work well */
	SINT16 			users; 					/* number of requests to open fb device */

	struct fb_info	fbinfo;					/* linux fb info */
	struct scanvas	pixel;					/* image description info */

	/* framebuffer area */
  	ULONG			fb_base_phys;			/* physical address of OSD bitmap data */
	ULONG			fb_base;				/* N/A */
  	UINT32			fb_size;				/* byte size of OSD bitmap data */

	/*osd header info*/
	ULONG			osd_hdr_base_phys;		/* physical address of indirect OSD header data */
	ULONG			osd_hdr_base;			/* ioremapped address */
	UINT32			osd_hdr_size;			/* byte size of OSD header (register) */

	/*palette header info*/
	ULONG			osd_plte_base_phys;		/* physical address of palette data */
	ULONG			osd_plte_base;			/* ioremapped address. Not used */
	UINT32			osd_plte_size;			/* number of color in palette, fixed ad 256 */

  	UINT32			pseudo_palette[17];		/* linux fb: fake palette of 16 colors. not used ? */

	OS_SEM_T		fb_dev_mtx;				/* mutex for each FB device */

	struct
	{
		UINT8		b_use_vsync;			/* current FB should wait for vsync for every flip operaration or not */
		UINT8		b_osd_hdr_write;		/* enable osd header handler or not. if not enabled, OSD header handler is not called */
		UINT8		b_osd_hdr_flush;		/* enable osd header flush or not. if not enabled, OSD header regs are not updated */
	}
	status;

	int				vsync_cnt_on_flip;		/* vsync count that will be used to wait on flipping. <0 will not care the vsync. */
	int				vsync_cnt;

	UINT16			freeze_ctrl_mask;		/* if freeze mask is non-zero value, fb kdriver should block some operation accoring to freeze_mask */
	FBDEV_CTX_T*	dm; 					/* pointer to parent context */
};

/** main context structure for FBDEV controller
 *
 *
 */
struct _FBDEV_CTX_T
{
	FBDEV_WIN_CTX_T*	wctx[FBDEV_MAX_DEVICE];	/* each layer context */

	ULONG				mmio_base_phys;			/* memory mapped reg addr */
	ULONG				mmio_size;				/* total byte size of OSD reg */

	struct
	{
		UINT16			pivot_mode;				/* pivot angle. refoer LX_FBDEV_PIVOT_MODE_XXX  */
		UINT8			b_freeze;				/* global osd freeze */

	} global;

	struct device		*dev; 					/* handle for the linux platform device */
};

/* FBDEV regiseter configuration
 *
 *
 */
typedef struct
{
    char*       chip_name;
    UINT32      reg_base_addr;
    UINT32      reg_size;
    UINT32      irq_num;

	UINT32		slkrd_prefix_mask;
	UINT32		slkrd_mmap_mask;

#define	FBDEV_HDR_OP_MMIO	0
#define FBDEV_HDR_OP_REG	1
	UINT32		hdr_op_method:1,		/* heaer operation method */
				:31;

	UINT32		frame_base_addr;		/* predefined FB device memory adress. value 0x0 means dynamic allocation */
	UINT32		frame_base_size;		/* FB device memory size */
}
LX_FBDEV_REG_IO_CTX_T;

typedef enum
{
	/* issued when FB pixel format changing
	 * data[0] = fb_dev_id, data[1] = bpp
	 */
	FBDEV_HW_EVENT_PXL_FMT_CHANGE			= 0,

	/* screen reverse changed
	 * data = struct reverse_screen_change
	 */
	FBDEV_HW_EVENT_REVERSE_SCREEN_CHANGE,

	/* vsync vent arrived.
	 * data = struct vsync
	 */
	FBDEV_HW_EVENT_VSYNC,

	/* FB(POSD) is opened/closed
	 * data[0] = fb_dev_id
	 */
	FBDEV_HW_EVENT_POSD_OPEN,
	FBDEV_HW_EVENT_POSD_CLOSE,

	/* FB(POSD) freeze should stopped */
	FBDEV_HW_EVENT_FREEZE_OFF,
}
FBDEV_HW_EVENT_T;

typedef union
{
	/* general data */
	struct
	{
		UINT32	arg[4];
	}
	gen;

	/* FBDEV_HW_EVENT_PXL_FMT_CHANGE */
	struct
	{
		UINT32	fb_dev_id;
		UINT32	bits_per_pixel;;
	}
	pxl_fmt_change;

	struct
	{
		UINT32	fb_dev_id;
		UINT32	enable;
	}
	reverse_screen_change;

	/* FBDEV_HW_EVENT_VSYNC */
	struct
	{
		UINT32	sync_counter;
	}
	vsync;

	/* FBDEV_HW_EVENT_POSD_OPEN, FBDEV_HW_EVENT_POSD_CLOSE */
	struct
	{
		UINT32	fb_dev_id;
	}
	osd;
}
FBDEV_HW_EVENT_DATA_T;

typedef struct
{
	bool			b_suspend;

	struct
	{
		UINT32		visible_status;
	}
	hybernation;
}
FBDEV_DRVDATA_T;

/** data struct to support OSD vsync event.
 *	OSD vsync event is supported based on Linux uevent method.
 */
typedef struct
{
	int					vsync_irq_enabled;
	int					wait_vsync_cnt;

	struct device* 		dev;
	ktime_t				vsync_time;

	struct completion 	vsync_completion;
	spinlock_t 			spinlock;
}
FBDEV_VSYNC_UEVENT_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
struct fb_var_screeninfo* FBDEV_GetVarScreenObject(int idx);

void			FBDEV_InitCfg(void);
void			FBDEV_InitHWCtx(void);

void			FBDEV_GetDefaultPalData(UINT32* pPal);
void			FBDEV_InitCanvas(FBDEV_WIN_CTX_T *w);
int				FBDEV_CleanupLayer(FBDEV_WIN_CTX_T *w);

int				FBDEV_GetConfig(FBDEV_WIN_CTX_T *w, LX_FBDEV_CONFIG_T *config);
int				FBDEV_SetConfig(FBDEV_WIN_CTX_T *w, LX_FBDEV_CONFIG_T *config);

void			FBDEV_SetPalette(FBDEV_WIN_CTX_T *w, UINT32* palette, int num_pal);
void			FBDEV_SetOSDImageEndian(int layer, int bpp);

int  			FBDEV_FlipFBImage( int fb_dev_id, LX_FBDEV_FB_IMAGE_T* fb_screen, BOOLEAN b_free_run );
int 			FBDEV_UpdateFBScreen( int fb_dev_id, LX_FBDEV_FB_SCREEN_T* fb_screen );

void			FBDEV_BackupFBList(void);
void			FBDEV_RestoreFBList(void);

void			FBDEV_MON_Init			( void );
void			FBDEV_MON_SetFPSMon		( BOOLEAN onoff );
void			FBDEV_MON_NotiRefresh	( int fb_dev_id );
void			FBDEV_MON_ISRTick		( void );

/** execute FBDEV internal event
 *
 */
void			FBDEV_ExecFBHWHandler(FBDEV_HW_EVENT_T ev, FBDEV_HW_EVENT_DATA_T* data );

/** get FB main context
 *
 * @return pointer to FB device driver
 */
FBDEV_CTX_T*	FBDEV_GetCtx(void);

/** get windows context based on OSD layer
 *
 *
 * @return pointer to dm_win_info if valid layer, NULL otherwise
 */
FBDEV_WIN_CTX_T* FBDEV_GetWinCtx(int layer);

/** get window canvas based on OSD layer
 *
 *
 * @return pointer to scanvas if valid layer, NULL otherwise
 */
struct scanvas*	FBDEV_GetWinCanvas( int layer );


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int						g_fbdev_debug_fd;		/** debug id */

extern	int						g_fbdev_osd_freeze;
extern  LX_FBDEV_CFG_T*			g_fbdev_cfg;			/** FBDEV config */
extern  FBDEV_CTX_T*			g_dm_ctx;				/** FBDEV main context */
extern	LX_FBDEV_REG_IO_CTX_T*	g_reg_io_ctx;			/** FBDEV register I/O context */

extern	UINT32					g_fbdev_vsync_cnt;		/** current VSYNC count status */

/* FBDEV config access macro */
#define LX_FBDEV_CFG(_fld)  g_fbdev_cfg->_fld

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_DRV_H_ */

/** @} */
