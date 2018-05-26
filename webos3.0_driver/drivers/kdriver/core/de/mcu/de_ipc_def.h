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


/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.02.18
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#ifndef _DE_IPC_TYPE_h
#define _DE_IPC_TYPE_h

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define DTVSOC_IPC_TOTAL_SIZE		 0x100
#define VIDEO_IPC_BUFF_NUM			 100
#define VIDEO_MSG_STR_MAX			 2048

#define DTVSOC_IPC_FROM_CPU_SIZE	 (DTVSOC_IPC_TOTAL_SIZE/2)
#define DTVSOC_IPC_FROM_MCU_SIZE	 (DTVSOC_IPC_TOTAL_SIZE/2)
#define VIDEO_IPC_STR_MAX			 DTVSOC_IPC_FROM_CPU_SIZE

#define IPC_1_TOTAL_SIZE               0x20
#define IPC_1_FROM_DE_SIZE             0x8
#define IPC_1_FROM_CPU_SIZE            ((IPC_1_TOTAL_SIZE-IPC_1_FROM_DE_SIZE)/2)
#define IPC_1_FROM_MCU_SIZE            ((IPC_1_TOTAL_SIZE-IPC_1_FROM_DE_SIZE)/2)


#define	VIDEO_WDG_INTERRUPT_ARM_BIT  31
#define	VIDEO_IPC_INTERRUPT_ARM_BIT  29
#define	VIDEO_DMA_INTERRUPT_ARM_BIT  28
#define	VIDEO_JPG_INTERRUPT_ARM_BIT  27
#define	VIDEO_USB_INTERRUPT_ARM_BIT  26
#define	VIDEO_WEL_INTERRUPT_ARM_BIT  25
#define	VIDEO_WER_INTERRUPT_ARM_BIT  24
#define	VIDEO_AMZ_INTERRUPT_ARM_BIT  23


#define	VIDEO_WDG_INTERRUPT_ARM_MASK (1 << VIDEO_WDG_INTERRUPT_ARM_BIT)
#define	VIDEO_IPC_INTERRUPT_ARM_MASK (1 << VIDEO_IPC_INTERRUPT_ARM_BIT)
#define	VIDEO_DMA_INTERRUPT_ARM_MASK (1 << VIDEO_DMA_INTERRUPT_ARM_BIT)
#define	VIDEO_JPG_INTERRUPT_ARM_MASK (1 << VIDEO_JPG_INTERRUPT_ARM_BIT)
#define	VIDEO_USB_INTERRUPT_ARM_MASK (1 << VIDEO_USB_INTERRUPT_ARM_BIT)
#define	VIDEO_WEL_INTERRUPT_ARM_MASK (1 << VIDEO_WEL_INTERRUPT_ARM_BIT)
#define	VIDEO_WER_INTERRUPT_ARM_MASK (1 << VIDEO_WER_INTERRUPT_ARM_BIT)
#define	VIDEO_AMZ_INTERRUPT_ARM_MASK (1 << VIDEO_AMZ_INTERRUPT_ARM_BIT)

#define	VIDEO_MSB_INTERRUPT_ARM_BIT  VIDEO_IPC_INTERRUPT_ARM_BIT
#define	VIDEO_LSB_INTERRUPT_ARM_BIT  VIDEO_WER_INTERRUPT_ARM_BIT
#define	VIDEO_WTH_INTERRUPT_ARM_BIT  (VIDEO_MSB_INTERRUPT_ARM_BIT - VIDEO_LSB_INTERRUPT_ARM_BIT + 1)

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
/**
 * @brief IPC command list
 */
typedef enum {
	DE_IPC_DE_INIT	= 0x00000001		/* 0x00000001 */, ///< Initialize de module
	DE_IPC_GET_FIRMWARE_INFO			/* 0x00000002 */, ///< get information of Firmware
	DE_IPC_GET_INPUT_WIN				/* 0x00000003 */, ///< get scaler input window size and position.
	DE_IPC_GET_OUT_WIN					/* 0x00000004 */, ///< set scaler output window size and position.
	DE_IPC_SET_INPUT_WIN				/* 0x00000005 */, ///< set scaler input window size and position.
	DE_IPC_SET_OUT_WIN					/* 0x00000006 */, ///< get scaler input window size and position.
	DE_IPC_SET_FREEZE					/* 0x00000007 */, ///< set freeze of selected window
	DE_IPC_SET_INPUT_SRC				/* 0x00000008 */, ///< set source type comming into selected window
	DE_IPC_SET_DIS_OUT					/* 0x00000009 */, ///< set display size and position.
	DE_IPC_SET_DIS_FMT					/* 0x0000000a */, ///< set display pannel format
	DE_IPC_SET_MULTI_WIN     			/* 0x0000000b */, ///< select type of muliwindow
	DE_IPC_MULTI_WIN_ENALBE				/* 0x0000000c */, ///< determine selected multi window to turn On or Off
	DE_IPC_SET_CVI_SRC_TYPE				/* 0x0000000d */, ///< set source type comming into selected CVI
	DE_IPC_SET_CVI_CSC					/* 0x0000000e */, ///< send color space conversion matrix and offset for each external source information.
	DE_IPC_SET_POST_CSC					/* 0x0000000f */, ///< send color space conversion matrix for post processing block
	DE_IPC_GET_MVI_COLORIMETRY			/* 0x00000010 */, ///< get MVI source colorimetry information.
	DE_IPC_SET_ZLIST					/* 0x00000011 */, ///< set display size and position.
	DE_IPC_GRAB_PIXEL					/* 0x00000012 */, ///< grab video pixels of certain block size and position.
	DE_IPC_GET_MVI_TIMING_INFO			/* 0x00000013 */, ///< set source type comming into selected MVI
	DE_IPC_SET_DEINTERLACE				/* 0x00000014 */, ///< set deinterlace on/off control
	//DE_IPC_SET_3D_SRC_MUX				, ///< control pre-3D source path and enable/disable pre-3D block in DE top level
	DE_IPC_SET_PRE3D_IN_MODE			/* 0x00000015 */, ///< select pre-3D input mode  (0: normal mode, 1:frame by frame mode)
	//DE_IPC_SET_PRE3D_OUT_MODE			, ///< select pre-3D ouput mode (00:T/B, 01:S/S, 10:FR/FR, 11:reserved)
	//DE_IPC_SET_PRE3D_IN_CH			, ///< select pre-3D input channel(0: A, 1:B)
	//DE_IPC_SET_PRE3D_ENABLE			, ///< control pre-3D block on/off (0: disable, 1: enable)
	DE_IPC_SET_3D_OPR_MODE				/* 0x00000016 */, ///< control pre-3D block on/off (0: disable, 1: enable)
	DE_IPC_SET_PWM						/* 0x00000017 */, ///< set de PWN parameter. only used for 60Hz LCD panel.
	DE_IPC_SET_CVE						/* 0x00000018 */, ///< set de CVE parameter.
	DE_IPC_SET_VCS						/* 0x00000019 */, ///< set de VCS parameter.
	DE_IPC_GET_CAPTURE_WIN				/* 0x0000001a */, ///< get de captured video size and offset
	DE_IPC_SET_CAPTURE_WIN				/* 0x0000001b */, ///< set de captured video size and offset
#ifdef USE_SUPPORT_EUROPE
	DE_IPC_SET_FR_RATE					/* 0x0000001c */, ///< set display frame rate
	DE_IPC_GET_OUT_FR_RATE				/* 0x0000001d */, ///< get display frame rate
#endif
	DE_IPC_SET_BG_COLOR					/* 0x0000001e */, ///< set back ground color of display
	DE_IPC_SET_WIN_BLANK				/* 0x0000001f */, ///< set blank of selected window and blank color
	DE_IPC_SET_IRE_PATTERN				/* 0x00000020 */, ///< set color and IRE levels of full pattern.
	DE_IPC_SET_SPREAD					/* 0x00000021 */, ///< set LVDS spread spectrum for 60Hz mode (PDP).
	DE_IPC_REG_RD						/* 0x00000022 */, ///< read Register
	DE_IPC_REG_WD						/* 0x00000023 */, ///< write Register
	DE_IPC_OIF_INIT						/* 0x00000024 */, ///< Initialize OIF module.
	DE_IPC_OIF_SET_LVDS_OUTPUT			/* 0x00000025 */, ///< Set LVDS OUTPUT Enable.
	DE_IPC_OIF_SET_LVDS_POWER			/* 0x00000026 */, ///< Set LVDS power on.
	DE_IPC_OIF_SET_DARK_SCREEN			/* 0x00000027 */, ///< Set LVDS dark screen.
	//DE_IPC_OIF_SET_LVDS_INFO			, ///< Set LVDS Information.
	//DE_IPC_OIF_PWM_CONTROL			, ///< set OIF PWM CONTROL parameter.
	//DE_IPC_OIF_LVDS_IF_CONTROL		, ///< set OIF LVDS IF Ctrl parameter.
	//DE_IPC_OIF_LVDS_TX_CONTROL		, ///< set OIF LVDS TX Ctrl parameter.
	//DE_IPC_OIF_DELAY_DATA				, ///< set OIF DELAY DATA parameter.
	//DE_IPC_OIF_LVDS_FORCE_DATA			, ///< set OIF LVDS FORCE DATA parameter.
	DE_IPC_OIF_SET_VESA_JEIDA			/* 0x00000028 */, ///< Select LVDS type(VESA/JEIDA).
	DE_IPC_OIF_SET_PANEL_IF				/* 0x00000029 */, ///< select LVDS inteface type(LVDS/mini-LVDS).
	DE_IPC_OIF_SET_PIXEL				/* 0x0000002a */, ///< set LVDS pixel type( single/dual/quad).
	DE_IPC_OIF_SET_BIT_DEPTH			/* 0x0000002b */, ///< set LVDS bit resolution(10bit/8bit).
	DE_IPC_OIF_SELECT_OUT_PATH			/* 0x0000002c */, ///< set LVDS signal path from DE/FRC/LED/ODC
	DE_IPC_OIF_SELECT_LVDS_PATH			/* 0x0000002d */, ///< set LVDS signal path from DE/FRC/LED/ODC
	DE_IPC_OIF_SELECT_OUT_CH			/* 0x0000002e */, ///< select output chanel number 1CH, 2CH, 4CH according to resolution and frame rate
	DE_IPC_OIF_SELECT_LED_PATH			/* 0x0000002f */, ///< select LED input path from DE/FRC.
	DE_IPC_OIF_SELECT_PWM_PATH			/* 0x00000030 */, ///< select PWM signal path from DE/FRC.
	DE_IPC_OIF_SELECT_LR_PATH			/* 0x00000031 */, ///< select LR signal path from DE/FRC.
	DE_IPC_OIF_GET_INFO					/* 0x00000032 */, ///< get OIF setting information.
	DE_IPC_OIF_GET_LVDS_IF_CTRL			/* 0x00000033 */, ///< get OIF setting information.
	DE_IPC_OIF_GET_LVDS_TX_CTRL			/* 0x00000034 */, ///< Control LVDS TX following TCON enable.
	DE_IPC_SET_CVI_FIR					/* 0x00000035 */, ///< send captured video fir coef for double/quad sampling case.
	DE_IPC_SET_CVI_TPG					/* 0x00000036 */, ///< set captured video test pattern generator to full mono color.
	DE_IPC_SET_CVI_CSAMPLE_MODE			/* 0x00000037 */, ///< set captured video color sampling mode(sub sampling or 3 tap filtering).
	DE_IPC_SET_CVI_SYNC_RESHAPE			/* 0x00000038 */, ///< reshape sync of captured video for PC input.
	DE_IPC_SET_PE_TEST_PATTERN_GEN		/* 0x00000039 */, ///< generate TPG in PE region.
	DE_IPC_SET_PE_BBD_CTRL				/* 0x0000003a */, ///< set PE0 black boundary detection control.
	DE_IPC_GET_PE_BBD_STATUS			/* 0x0000003b */, ///< get PE0 black boundary detection status.
	DE_IPC_SET_EDGE_CROP				/* 0x0000003c */, ///< generate TPG in PE region.
	DE_IPC_SET_DVR_FMT_CTRL				/* 0x0000003d */, ///< set DVR format control
	DE_IPC_FC_SET_WIN_INFO				/* 0x0000003e */, ///< set geomitery information of picture to be caputured.
	DE_IPC_FC_GET_FRAME_INFO			/* 0x0000003f */, ///< get address of picture to be caputured
	DE_IPC_SET_SUB_SC_FREEZE			/* 0x00000040 */, ///< set frame caputuring mode On or Off.
	DE_IPC_SET_3D_FULL_MODE				/* 0x00000041 */, ///< set UD & 3D mode.
	DE_IPC_SET_CVI_VIDEO_FRAME_BUFFER	/* 0x00000042 */, ///< copy frame buffer of certain block size and position.
	DE_IPC_GET_CVI_INFO					/* 0x00000043 */, ///< get information of cvi input.
	DE_IPC_SET_CVI_FREEZE				/* 0x00000044 */, ///< de cvi video frame buffer freeze
	DE_IPC_SET_LVDS_PATH_INFO			/* 0x00000045 */, ///< set lvds dual out mode for display sync control
	DE_IPC_SELECT_MULTI_WIN_SRC			/* 0x00000046 */, ///< de multi window source for cvi input
	DE_IPC_SET_UD_MODE					/* 0x00000047 */, ///< set UD mode.
	DE_IPC_SET_CVI_DELAY				/* 0x00000048 */, ///< set delay of gbr signal on cvi
	DE_IPC_SET_LOW_DELAY			    /* 0x00000049 */, ///< set low delay mode for frame delay
	DE_IPC_GET_PAR_INFO					/* 0x0000004a */, ///< get information of pixel aspect ratio
	DE_IPC_SET_PLATFORM_VERSION			/* 0x0000004b */, ///< set information of chip version
	DE_IPC_SET_MULTI_VISION				/* 0x0000004c */, ///< set information of multi vision
	DE_IPC_GET_FRAME_FOR_WEBOS			/* 0x0000004d */, ///< get frame address for WebOs to be caputured
	DE_IPC_SET_IF_CONFIG				/* 0x0000004e */, ///< set configure for DE
	DE_IPC_GET_LOW_DELAY			    /* 0x0000004f */, ///< get low delay mode for frame delay
	DE_IPC_GET_SCALER_INFO				/* 0x00000050 */, ///< get scaler info
	DE_IPC_SET_INNER_PATTERN			/* 0x00000051 */, ///< get scaler info
	DE_IPC_SET_WIN_PORT_SRC             /* 0x00000052 */, ///< set window port source
	DE_IPC_SET_WIN_FREEZE               /* 0x00000053 */, ///< set freeze of selected window
	DE_IPC_SET_CAPTURE                  /* 0x00000054 */, ///< set capture control
	DE_IPC_SET_CVI_PIXEL_CAPTURE        /* 0x00000055 */, ///< set capture control for cvi input pixels (read/write)
	DE_IPC_SET_SRC_MIRROR               /* 0x00000056 */, ///< set source mirror to denc (scart-out path)
	DE_IPC_SET_ACT_FMT_DETECT           /* 0x00000057 */, ///< set actual format detect (for seamless switch)
	DE_IPC_SET_PUSH_MODE                /* 0x00000058 */, ///< set push mode(for updating pic before dvo), UINT32 wid
//#ifdef INCLUDE_KDRV_FMT3D
	FMT3D_IPC_INIT	= 0x10000000		/* 0x10000000 */, ///< initialize 3D formatter module.
	FMT3D_IPC_SET_PROCESS_MODE			/* 0x10000001 */, ///< set 3D formatter processing mode
	FMT3D_IPC_GET_PROCESS_MODE			/* 0x10000002 */, ///< get 3D formatter processing mode
	FMT3D_IPC_SET_IN_IMG_INFO			/* 0x10000003 */, ///< set 3D formatter input image setting
	FMT3D_IPC_GET_IN_IMG_INFO			/* 0x10000004 */, ///< get 3D formatter input image setting
	FMT3D_IPC_GET_OUT_IMG_INFO			/* 0x10000005 */, ///< get 3D formatter output image info.
	FMT3D_IPC_SET_RUNMODE				/* 0x10000006 */, ///< set 3D formatter running mode.
	FMT3D_IPC_TOGGLE_LR_IN_ORDER		/* 0x10000007 */, ///< set 3D formatter input image left/right order.
	FMT3D_IPC_TOGGLE_LR_OUT_ORDER		/* 0x10000008 */, ///< set 3D formatter output left/right first order.
	FMT3D_IPC_SET_IN_IMG_FMT			/* 0x10000009 */, ///< set 3D formatter input image format.
	FMT3D_IPC_SET_DIS_FMT				/* 0x1000000a */, ///< set 3D formatter display format(line interleaved or frame sequential).
	FMT3D_IPC_SET_IN_FR_RATE			/* 0x1000000b */, ///< set 3D formatter input image frame rate.
	FMT3D_IPC_SET_OUT_FR_RATE			/* 0x1000000c */, ///< set 3D formatter output frame rate (120Hz or 240Hz)
	FMT3D_IPC_SET_CONVERGENCE			/* 0x1000000d */, ///< set 3D convergence
	FMT3D_IPC_SET_IMG_DOUBLE_PIC		/* 0x1000000e */, ///< set 3D formatter input image frame rate.
	FMT3D_IPC_SET_SAMPLE_MODE		    /* 0x1000000f */, ///< set 3D formatter input image sample mode.
	FMT3D_IPC_SET_SRC_MUX		        /* 0x10000010 */, ///< select 3D formatter input source path.
	FMT3D_IPC_SET_IMG_SCAN_TYPE		    /* 0x10000011 */, ///< set 3D formatter input image scan type
	FMT3D_IPC_SET_FRC_OUT_TYPE		    /* 0x10000012 */, ///< set 3D formatter output type for FRC MCU operation
	FMT3D_IPC_SET_FRC_OUT_LR_ORDER		/* 0x10000013 */, ///< set 3D formatter output LR order for FRC MCU operation
	FMT3D_T3_IPC_SET_CONVERGENCE		/* 0x10000014 */, ///< set T3 convergence
	FMT3D_T3_IPC_SET_DEPTH_CTRL			/* 0x10000015 */, ///< set T3 depth control
	FMT3D_IPC_SET_FPR_FHD				/* 0x10000016 */, ///< set 3D formatter FPR FHD mode
	FMT3D_IPC_SET_INOUT_CTRL			/* 0x10000017 */, ///< set 3D formatter inout/output control
//#endif
#ifdef INCLUDE_KDRV_TCON
	TCON_IPC_INIT = 0x20000000			/* 0x20000000 */, ///< Initialize TCON module.
	TCON_IPC_SET_ODC_TABLE				/* 0x20000001 */, ///< Set OD(over drive control) Table [for improving LCD panel response time].
	TCON_IPC_CONTROL					/* 0x20000002 */, ///< set control parameter
	TCON_IPC_SET_DGA					/* 0x20000003 */, ///< Set DGA(Digital Gamma Algorithm) Table and Enable.
#endif
#ifdef INCLUDE_KDRV_DENC
	DENC_IPC_INIT = 0x30000000			/* 0x30000000 */, ///< Initialize DENC module.
	DENC_IPC_ONOFF						/* 0x30000001 */, ///< NTSC/PAL/SECAM encoder enable(TRUE : enable, FALSE : disable)
	DENC_IPC_SET_OUTPUT_VIDEO_STD		/* 0x30000002 */, ///< Select the color standard of CVE.
	DENC_IPC_SET_SOURCE					/* 0x30000003 */, ///< Select the input source of CVE.
	DENC_IPC_TTX_ENABLE					/* 0x30000004 */, ///< Teletext insertion enable (TRUE : enable, FALSE : disable)
	DENC_IPC_WSS_ENABLE					/* 0x30000005 */, ///< WSS insertion enable (TRUE : enable, FALSE : disable)
	DENC_IPC_VPS_ENABLE					/* 0x30000006 */, ///< VPS insertion enable (TRUE : enable, FALSE : disable)
	DENC_IPC_SET_TTX_DATA				/* 0x30000007 */, ///< Inserts Teletext data to VBI lines.
	DENC_IPC_SET_WSS_DATA				/* 0x30000008 */, ///< Inserts WSS data to VBI line.
	DENC_IPC_SET_VPS_DATA				/* 0x30000009 */, ///< Inserts VPS data to VBI lines.
	DENC_IPC_COLORBAR_ENABLE			/* 0x3000000a */, ///< Enable/disable internal colorbar pattern (TRUE : enable, FALSE : disable)
	DENC_IPC_MUTE_CONTROL				/* 0x3000000b */, ///< Set dark of DENC Block (0:off, 1:on)
	DENC_IPC_STRIP_CAPTURE_CONTROL		/* 0x3000000c */, ///< turn on/off sub-scaler for 3D auto detectiong by capturing video.
#endif
#ifdef INCLUDE_KDRV_DIMM
	DIMM_IPC_INIT = 0x40000000			/* 0x40000000 */, ///< Initialize DIMM module.
	DIMM_IPC_LOCALDIMMING_CONTROL		/* 0x40000001 */, ///< Control local-dimming-on.
	DIMM_IPC_STORE_DEMO				    /* 0x40000002 */, ///< Control Dimming Demo Mode.
	DIMM_IPC_LINE_DEMO				    /* 0x40000003 */, ///< Control Dimming Line Demo Mode.
	DIMM_IPC_CONTROL_COMPENSATION		/* 0x40000004 */, ///< Control Pixel Compensation.
	DIMM_IPC_LUT_CONTROL				/* 0x40000005 */, ///< Set Look up Table.
	DIMM_IPC_STORE_CONTROL				/* 0x40000006 */, ///< Local Dimming Control for Store Mode.
	DIMM_IPC_REG_RD						/* 0x40000007 */, ///< Read Register
	DIMM_IPC_REG_WR						/* 0x40000008 */, ///< Write Register
//#ifdef USE_DIMM_TEMPORAL_DEFINITION
	DIMM_IPC_GET_VERSION				/* 0x40000009 */, ///< Get DIMM INFO.
	DIMM_IPC_DBG_CONTROL				/* 0x4000000a */, ///< For DIMM debugging.
	DIMM_IPC_BLU_CONTROL				/* 0x4000000b */, ///< BLU(Back Light Uint) Control of DIMM.
//#endif
	DIMM_IPC_WBCOLOR_CTRL				/* 0x4000000c */, ///< Local Dimming Control White Balance.
#endif
#ifdef INCLUDE_KDRV_PE
	PQL_IPC_INIT = 0x50000000			/* 0x50000000 */, ///< Initialize PQL module.
	PQL_IPC_SET_YUV2RGB					/* 0x50000001 */, ///< change YUV color space to RGB color domain.
	PQL_IPC_SET_RGB2YUV					/* 0x50000002 */, ///< change RGB color space to YUV color domain.
	PQL_IPC_SET_CONTRAST				/* 0x50000003 */, ///< control contrast of luminance.
	PQL_IPC_SET_BRIGHTNESS				/* 0x50000004 */, ///< control brightness of luminance.
	PQL_IPC_SET_SATURATION				/* 0x50000005 */, ///< control saturation of chrominance.
	PQL_IPC_SET_HUE						/* 0x50000006 */, ///< control hue of chrominance.
	PQL_IPC_SET_SHARPNESS				/* 0x50000007 */, ///< control sharpness of picture.
	PQL_IPC_SET_COLORTEMP				/* 0x50000008 */, ///< control color temperature of picture.
	PQL_IPC_SET_PARAMTABLE				/* 0x50000009 */, ///< set PQ parameter table.
	PQL_IPC_SET_TPG						/* 0x5000000a */, ///< generating internal test patterns.
	PQL_IPC_SET_TNR						/* 0x5000000b */, ///< control temporal noise reduction.
	PQL_IPC_SET_IPC						/* 0x5000000c */, ///< control deinterlacer.
	PQL_IPC_SET_DNR						/* 0x5000000d */, ///< control dynamic noise reduction.
	PQL_IPC_SET_TNR_ENABLE				/* 0x5000000e */, ///< set TNR Enable
	PQL_IPC_SET_FILMMODE				/* 0x5000000f */, ///< set Film Mode
	PQL_IPC_SET_SHPENABLE				/* 0x50000010 */, ///< enable sharpness of picture.
	PQL_IPC_SET_PC_ENABLE				/* 0x50000011 */, ///< enable picture control.
	PQL_IPC_SET_GAMMALUT				/* 0x50000012 */, ///< set gamma lut
	PQL_IPC_GET_GAMMALUT				/* 0x50000013 */, ///< get gamma lut
	PQL_IPC_SET_COLORFILTER				/* 0x50000014 */, ///< set color filter
	PQL_IPC_SET_EDGEENHANCE				/* 0x50000015 */, ///< control edge enhance of picture.
	PQL_IPC_SET_XVYCC					/* 0x50000016 */, ///< control xvYCC.
	PQL_IPC_SET_TABLE					/* 0x50000017 */, ///< set pql table
	PQL_IPC_LOAD_DB_DATA				/* 0x50000018 */, ///< load db data
#endif
	DE_IPC_CMD_MAX						, ///< Maximum number for IPC communication between MCU and CPU
} DE_IPC_CMD_T;

typedef enum {
	// mcu 0
	VIDEO_IPC_MCU,
	VIDEO_IPC_CPU,
	VIDEO_DMA_MCU,
	VIDEO_DMA_CPU,
	VIDEO_JPG_MCU,
	VIDEO_JPG_CPU,
	VIDEO_USB_MCU,
	VIDEO_USB_CPU,
	VIDEO_WEL_MCU,
	VIDEO_WEL_CPU,
	VIDEO_WER_MCU,
	VIDEO_WER_CPU,
	VIDEO_AMZ_MCU,
	VIDEO_AMZ_CPU,

	// mcu 1
	VIDEO_INT_HWI,
	VIDEO_INT_SWI,

	VIDEO_CPU_MCU_ACK,
	VIDEO_CPU_MCU_IPC,
	VIDEO_MCU_CPU_ACK,
	VIDEO_MCU_CPU_IPC,
	VIDEO_DE_MCU_ACK,
	VIDEO_DE_MCU_IPC,


	VIDEO_IPC_MAX,
} VIDEO_IPC_TYPE_T;

typedef enum {
	VIDEO_IPC_SIZE ,
	VIDEO_IPC_CONT ,
	VIDEO_IPC_FROM ,
	VIDEO_IPC_TYPE ,
	VIDEO_IPC_DATA ,
} VIDEO_IPC_CMD_T;

typedef enum {
	VIDEO_IPC_FROM_UART = 1 ,
	VIDEO_IPC_FROM_HOST = 2 ,
} VIDEO_IPC_FROM_T;

typedef enum {
	VIDEO_IPC_CMD_CLI = 1 ,
	VIDEO_IPC_CMD_VAL = 2 ,
	VIDEO_IPC_CMD_PRT = 3 ,
	VIDEO_IPC_CMD_SYNC = 5
} VIDEO_IPC_DATA_TYPE_T;

typedef enum {
	VIDEO_IPC_OK,
	VIDEO_IPC_BUSY,
	VIDEO_IPC_EMPTY,
	VIDEO_IPC_LAST,
} VIDEO_IPC_STATUS_T;

typedef enum {
	MCU1_SWI_IPC = 0x00000001,	
	MCU1_SWI_ACK = 0x00000002,

	MCU1_SWI_MAX
} MCU1_SWI_TYPE_T;

typedef enum {
	MCU1_MCU_INT_WDG = 0,
	MCU1_MCU_INT_GPIO,
	MCU1_MCU_INT_DMA,
	MCU1_MCU_INT_SWI,
	MCU1_MCU_INT_MAX = 32
} MCU1_MCU_INT_TYPE_T;  

typedef enum {
	MCU1_CPU_INT_WDG = 0,
	MCU1_CPU_INT_DMA,
	MCU1_CPU_INT_LED,
	MCU1_CPU_INT_DPPA,
	MCU1_CPU_INT_SRE,
	MCU1_CPU_INT_LRX,
	MCU1_CPU_INT_DVO,
	MCU1_CPU_INT_DVI,
	MCU1_CPU_INT_H3D0,
	MCU1_CPU_INT_H3D1,
	MCU1_CPU_INT_SWI = 15, // 15 ~ 31
	MCU1_CPU_INT_MAX = 32
} MCU1_CPU_INT_TYPE_T;

typedef struct {
	UINT16 u16From;
	UINT16 u16Type;
	UINT32 u32Cmd;
	UINT32 u32Data;
} MCU_1_IPC_MSG_T;

typedef struct {
	UINT32 wInx;
	UINT32 rInx;
	UINT32 queMax;
	UINT32 queOverflow;
	UINT8  str[VIDEO_IPC_BUFF_NUM][VIDEO_IPC_STR_MAX];
} MCU_VIDEO_EXCHANGE_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif
/**  @} */
