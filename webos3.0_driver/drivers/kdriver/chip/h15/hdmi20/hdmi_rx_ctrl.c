/**
 * @file hdmi_rx_ctrl.c
 * @brief HDMI RX controller driver
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */
#include "hdmi_rx_ctrl.h"
#include <linux/string.h>
#include <linux/delay.h>
/*
 * Controller Version
 */

/** Controller version. If defined version is 1.30a or above, if not defined the version is 1.20a */
#define CTRL_13xa

/*
 * Miscellaneous
 */
/** Configuration clock minimum [kHz] */
#define CFG_CLK_MIN				(10000UL)
/** Configuration clock maximum [kHz] */
//#define CFG_CLK_MAX				(160000UL)
#define CFG_CLK_MAX				(200000UL)
/** Mode detection clock minimum [kHz] */
#define MD_CLK_MIN				(10000UL)
/** Mode detection clock maximum [kHz] */
#define MD_CLK_MAX				(50000UL)
/** TMDS clock minimum [kHz] */
#define TMDS_CLK_MIN			(25000UL)
/** TMDS clock maximum [kHz] */
//#define TMDS_CLK_MAX			(340000UL)
#define TMDS_CLK_MAX			(600000UL)
/** TMDS clock delta [kHz] */
#define TMDS_CLK_DELTA			(125)
/** Pixel clock minimum [kHz] */
#define PIXEL_CLK_MIN			0
/** Pixel clock maximum [kHz] */
#define PIXEL_CLK_MAX			TMDS_CLK_MAX
/** Horizontal resolution minimum */
#define HRESOLUTION_MIN			(320)
/** Horizontal resolution maximum */
#define HRESOLUTION_MAX			(4096)
/** Vertical resolution minimum */
#define VRESOLUTION_MIN			(240)
/** Vertical resolution maximum */
#define VRESOLUTION_MAX			(4455)
/** Refresh rate minimum [1/100 Hz] */
#define REFRESH_RATE_MIN		(100)
/** Refresh rate maximum [1/100 Hz] */
#define REFRESH_RATE_MAX		(25000)
/** I2C operation max polling wait */
#define MAX_IIC_WAIT			(500000)
/** File name */
static const char *file = "hdmi_rx_ctrl";

/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#define MSK(m, n)		(((1 << (m)) - 1) << (n))
/**
 * Bit mask
 * @param n shift
 */
#ifndef BIT
#define BIT(n)			MSK(1, (n))
#endif
/*
 * HDMI registers
 */
/** Register address: setup control */
#define RA_HDMI_SETUP_CTRL      (0x000UL)
/** Hot plug detect signaled */
#define 	HOT_PLUG_DETECT			BIT(0)
/** Register address: override control */
#define RA_HDMI_OVR_CTRL        (0x004UL)
/** Register address: timer control */
#define RA_HDMI_TIMERA_CTRL     (0x008UL)
/** Register address: resistor override */
#define RA_HDMI_RES_OVR         (0x010UL)
/** Register address: resistor status */
#define RA_HDMI_RES_STS         (0x014UL)
/** Register address: TMDS PLL control */
#define RA_HDMI_PLL_CTRL        (0x018UL)
/** Register address: TMDS PLL frequency range */
#define RA_HDMI_PLL_FRQSET1     (0x01CUL)
/** Register address: TMDS PLL frequency range */
#define RA_HDMI_PLL_FRQSET2     (0x020UL)
/** Register address: TMDS PLL PCP and ICP range */
#define RA_HDMI_PLL_PAR1        (0x024UL)
/** Register address: TMDS PLL PCP and ICP range */
#define RA_HDMI_PLL_PAR2        (0x028UL)
/** Register address: TMDS PLL KOSC and CCOLF range */
#define RA_HDMI_PLL_PAR3        (0x02CUL)
/** Register address: PLL post lock filter */
#define RA_HDMI_PLL_LCK_STS     (0x030UL)
/** Register address: PLL clock control */
#define RA_HDMI_CLK_CTRL        (0x034UL)
/** Register address: PCB diversity control */
#define RA_HDMI_PCB_CTRL        (0x038UL)
/** Input selector */
#define		INPUT_SELECT			BIT(16)
/** Register address: phase control */
#define RA_HDMI_PHS_CTRL        (0x040UL)
/** Register address: used phases */
#define RA_HDMI_PHS_USD         (0x044UL)
/** Register address: miscellaneous operations control */
#define RA_HDMI_MISC_CTRL       (0x048UL)
/** Register address: EQ offset calibration */
#define RA_HDMI_EQOFF_CTRL      (0x04CUL)
/** Register address: EQ gain control */
#define RA_HDMI_EQGAIN_CTRL     (0x050UL)
/** Register address: EQ status */
#define RA_HDMI_EQCAL_STS       (0x054UL)
/** Register address: EQ results */
#define RA_HDMI_EQRESULT        (0x058UL)
/** Register address: EQ measurement control */
#define RA_HDMI_EQ_MEAS_CTRL    (0x05CUL)
/** Register address: HDMI mode recover */
#define RA_HDMI_MODE_RECOVER    (0x080UL)
/** Register address: HDMI error protection */
#define RA_HDMI_ERRORA_PROTECT  (0x084UL)
/** Register address: validation and production test */
#define RA_HDMI_ERD_STS         (0x088UL)
/** Register address: video output sync signals control */
#define RA_HDMI_SYNC_CTRL       (0x090UL)
/** VS polarity adjustment */
#define		VS_POL_ADJ_MODE			MSK(2, 3)
/** HS polarity adjustment automatic */
#define		VS_POL_ADJ_AUTO			(2)
/** HS polarity adjustment */
#define		HS_POL_ADJ_MODE			MSK(2, 1)
/** HS polarity adjustment automatic inversion */
#define		HS_POL_ADJ_AUTO			(2)
/** Register address: clock measurement */
#define RA_HDMI_CKM_EVLTM       (0x094UL)
/** Evaluation period */
#define		EVAL_TIME				MSK(12, 4)
/** active wait period for TMDS stabilisation */
#define		TMDS_STABLE_TIMEOUT			(30)
/** Register address: legal clock count */
#define RA_HDMI_CKM_F           (0x098UL)
/** Maximum count for legal count */
#define 	MAXFREQ					MSK(16, 16)
/** Minimum count for legal count */
#define 	MINFREQ					MSK(16, 0)
/** Register address: measured clock results */
#define RA_HDMI_CKM_RESULT      (0x09CUL)
/** Measured clock is stable */
#define 	CLOCK_IN_RANGE			BIT(17)
/** Measured clock rate in bits */
#define 	CLKRATE					MSK(16, 0)
/** Register address: sub-sampling control */
#define RA_HDMI_RESMPL_CTRL     (0x0A4UL)
/** Register address: deep color mode control */
#define RA_HDMI_DCM_CTRL        (0x0A8UL)
/** Register address: video output mute configuration */
#define RA_HDMI_VM_CFG_CH_0_1   (0x0B0UL)
/** Register address: video output mute configuration */
#define RA_HDMI_VM_CFG_CH2      (0x0B4UL)
/** Register address: spare */
#define RA_HDMI_SPARE           (0x0B8UL)
/** Register address: HDMI status */
#define RA_HDMI_STS             (0x0BCUL)
/** Current deep color mode */
#define		DCM_CURRENT_MODE		MSK(4, 28)
/** Deep color mode, 24 bit */
#define		DCM_CURRENT_MODE_24b	(4)
/** Deep color mode, 30 bit */
#define		DCM_CURRENT_MODE_30b	(5)
/** Deep color mode, 36 bit */
#define		DCM_CURRENT_MODE_36b	(6)
/** Deep color mode, 48 bit */
#define		DCM_CURRENT_MODE_48b	(7)
/*
 * Video Mode registers
 */
/** Register address: video mode control */
#define RA_MD_HCTRL1            (0x140UL)
/** Register address: video mode control */
#define RA_MD_HCTRL2            (0x144UL)
/** Register address: horizontal sync */
#define RA_MD_HT0               (0x148UL)
/** Register address: horizontal offset */
#define RA_MD_HT1               (0x14CUL)
/** Horizontal total length */
#define 	HTOT_PIX				MSK(16, 16)
/** Horizontal offset length */
#define 	HOFS_PIX				MSK(16, 0)
/** Register address: horizontal active length */
#define RA_MD_HACT_PX           (0x150UL)
/** Horizontal active length */
#define 	HACT_PIX				MSK(16, 0)
/** Register address: horizontal active time */
#define RA_MD_HACT_PXA          (0x154UL)
/** Register address: vertical control */
#define RA_MD_VCTRL             (0x158UL)
/** Register address: vertical timing - sync pulse duration */
#define RA_MD_VSC               (0x15CUL)
/** Register address: vertical timing - frame duration */
#define RA_MD_VTC               (0x160UL)
/** Frame duration */
#define		VTOT_CLK				(~0)
/** Register address: vertical offset length */
#define RA_MD_VOL               (0x164UL)
/** Vertical offset length */
#define 	VOFS_LIN				MSK(16, 0)
/** Register address: vertical active length */
#define RA_MD_VAL               (0x168UL)
/** Vertical active length */
#define 	VACT_LIN				MSK(16, 0)
/** Register address: vertical timing */
#define RA_MD_VTH               (0x16CUL)
/** Register address: vertical total length */
#define RA_MD_VTL               (0x170UL)
/** Vertical total length */
#define 	VTOT_LIN				MSK(16, 0)
/** Register address: skew measurement trigger */
#define RA_MD_IL_CTRL           (0x174UL)
/** Register address: VS and HS skew */
#define RA_MD_IL_SKEW           (0x178UL)
/** Register address: V&H skew and filed detection */
#define RA_MD_IL_POL            (0x17CUL)
/** Register address: video mode status */
#define RA_MD_STS               (0x180UL)
/** Interlace active status */
#define 	ILACE					BIT(3)
/*
 * Audio registers
 */
/** Register address: audio mode control */
#define RA_AUD_CTRL             (0x200UL)
/** Register address: audio PLL control */
#define RA_AUD_PLL_CTRL         (0x208UL)
/** Register address: audio PLL lock */
#define RA_AUD_PLL_LOCK         (0x20CUL)
/** Register address: DDS audio clock control */
#define RA_AUD_PLL_RESET        (0x210UL)
/** Register address: audio clock control */
#define RA_AUD_CLK_CTRL         (0x214UL)
/** Register address: ASP sync intervals */
#define RA_AUD_CLK_MASP         (0x218UL)
/** Register address: audio sync interval */
#define RA_AUD_CLK_MAUD         (0x21CUL)
/** Register address: sync interval reset */
#define RA_AUD_FILT_CTRL1       (0x220UL)
/** Register address: phase filter control */
#define RA_AUD_FILT_CTRL2       (0x224UL)
/** Register address: manual CTS control */
#define RA_AUD_CTS_MAN          (0x228UL)
/** Register address: manual N control */
#define RA_AUD_N_MAN            (0x22CUL)
/** Register address: audio clock status */
#define RA_AUD_CLK_STS          (0x23CUL)
/** Register address: audio FIFO control */
#define RA_AUD_FIFO_CTRL        (0x240UL)
/** Audio FIFO reset */
#define 	AFIF_INIT				BIT(0)
/** Register address: audio FIFO threshold */
#define RA_AUD_FIFO_TH          (0x244UL)
/** Register address: audio FIFO fill */
#define RA_AUD_FIFO_FILL_S      (0x248UL)
/** Register address: audio FIFO fill minimum/maximum */
#define RA_AUD_FIFO_CLRA_MM     (0x24CUL)
/** Register address: audio FIFO fill status */
#define RA_AUD_FIFO_FILLSTS     (0x250UL)
/** Register address: audio output interface configuration */
#define RA_AUD_CHEXTRA_CTRL     (0x254UL)
/** Register address: audio mute control */
#define RA_AUD_MUTE_CTRL        (0x258UL)
/** Manual/automatic audio mute control */
#define		AUD_MUTE_SEL			MSK(2, 5)
/** Force unmute (overrules all) */
#define		AUD_MUTE_FORCE_UN		(0)
/** Automatic mute when FIFO thresholds are exceeded */
#define		AUD_MUTE_FIFO_TH		(1)
/** Automatic mute when FIFO over/underflows */
#define		AUD_MUTE_FIFO_FL		(2)
/** Force mute (overrules all) */
#define		AUD_MUTE_FORCE			(3)
/** Register address: serial audio output control */
#define RA_AUD_SAO_CTRL         (0x260UL)
/** Register address: parallel audio output control */
#define RA_AUD_PAO_CTRL         (0x264UL)
/** Register address: audio FIFO status */
#define RA_AUD_FIFO_STS         (0x27CUL)
/** Register address: lock detector threshold */
#define RA_CI_PLLAUDIO_5        (0x28CUL)
/** Register address: test mode selection */
#define RA_CI_PLLAUDIO_4        (0x290UL)
/** Register address: bypass divider control */
#define RA_CI_PLLAUDIO_3        (0x294UL)
/** Register address: monitoring */
#define RA_CI_PLLAUDIO_2        (0x298UL)
/** Register address: control */
#define RA_CI_PLLAUDIO_1        (0x29CUL)
/** Register address: SNPS PHY GEN3 control - starting version 1.30a */
#define RA_SNPS_PHYG3_CTRL	(0x2C0UL)
/** Register address:  I2C Master: slave address - starting version 1.30a */
#define RA_I2CM_PHYG3_SLAVE 		(0x2C4UL)
/** Register address: I2C Master: register address - starting version 1.30a */
#define RA_I2CM_PHYG3_ADDRESS 		(0x2C8UL)
/** Register address: I2C Master: data to write to slave - starting version 1.30a */
#define RA_I2CM_PHYG3_DATAO  		(0x2CCUL)
/** Register address: I2C Master: data read from slave - starting version 1.30a */
#define RA_I2CM_PHYG3_DATAI  		(0x2D0UL)
/** Register address: I2C Master: operation RD/WR - starting version 1.30a */
#define RA_I2CM_PHYG3_OPERATION		(0x2D4UL)
/** Register address: I2C Master: SS/HS mode - starting version 1.30a */
#define RA_I2CM_PHYG3_MODE			(0x2D8UL)
/** Register address: I2C Master: soft reset - starting version 1.30a */
#define RA_I2CM_PHYG3_SOFTRST		(0x2DCUL)
/** Register address: I2C Master: ss mode counters  - starting version 1.30a */
#define RA_I2CM_PHYG3_SS_CNTS		(0x2E0UL)
/** Register address:I2C Master:  hs mode counters  - starting version 1.30a */
#define RA_I2CM_PHYG3_FS_HCNT		(0x2E4UL)
/*
 * Packet Decoder and FIFO Control registers
 */
/** Register address: packet decoder and FIFO control */
#define RA_PDEC_CTRL            (0x300UL)
/** Packet FIFO store filter enable */
#define		PFIFO_STORE_FILTER_EN	BIT(31)
/** Packet FIFO store packet */
#define		PFIFO_STORE_PACKET		BIT(16)
/** Packet FIFO clear min/max information */
#define		PD_FIFO_FILL_INFO_CLR	BIT(8)
/** Packet FIFO skip one packet */
#define		PD_FIFO_SKIP			BIT(6)
/** Packet FIFO clear */
#define		PD_FIFO_CLR				BIT(5)
/** Packet FIFO write enable */
#define		PD_FIFO_WE				BIT(4)
/** Packet error detection/correction enable */
#define		PDEC_BCH_EN				BIT(0)
/** Register address: packet decoder and FIFO configuration */
#define RA_PDEC_FIFO_CFG        (0x304UL)
/** Register address: packet decoder and FIFO status */
#define RA_PDEC_FIFO_STS        (0x308UL)
/** Register address: packet decoder and FIFO byte data */
#define RA_PDEC_FIFO_DATA       (0x30CUL)
/** Register address: packet decoder and FIFO debug control */
#define RA_PDEC_DBG_CTRL        (0x310UL)
/** Register address: packet decoder and FIFO measured timing gap */
#define RA_PDEC_DBG_TMAX        (0x314UL)
/** Register address: CTS loop */
#define RA_PDEC_DBG_CTS         (0x318UL)
/** Register address: ACP frequency count */
#define RA_PDEC_DBG_ACP         (0x31CUL)
/** Register address: signal errors in data island packet */
#define RA_PDEC_DBG_ERRA_CORR   (0x320UL)
/** Register address: CTS reset measurement control */
#define RA_PDEC_ACRM_CTRL       (0x330UL)
/** Register address: maximum CTS div N value */
#define RA_PDEC_ACRM_MAX        (0x334UL)
/** Register address: minimum CTS div N value */
#define RA_PDEC_ACRM_MIN        (0x338UL)
/** Register BCH/Checksum Error Filter Handling Register */
#define RA_PDEC_ERR_FILTER        (0x33CUL)
/** Register address: audio sub packet control */
#define RA_PDEC_ASP_CTRL        (0x340UL)
/** Automatic mute all video channels */
#define		AUTO_VMUTE				BIT(6)
/** Automatic mute audio sub packets */
#define		AUTO_SPFLAT_MUTE		MSK(4, 2)
/** Register address: audio sub packet errors */
#define RA_PDEC_ASP_ERR         (0x344UL)
/** Register address: packet decoder status, see packet interrupts */
#define RA_PDEC_STS             (0x360UL)
/** Register address: general control packet AV mute */
#define RA_PDEC_GCP_AVMUTE      (0x380UL)
/** Register address: audio clock regeneration */
#define RA_PDEC_ACR_CTS        (0x390UL)
/** Audio clock regeneration, CTS parameter */
#define		CTS_DECODED				MSK(20, 0)
/** Register address: audio clock regeneration */
#define RA_PDEC_ACR_N         	(0x394UL)
/** Audio clock regeneration, N parameter */
#define		N_DECODED				MSK(20, 0)
/** Register address: auxiliary video information info frame */
#define RA_PDEC_AVI_HB         	(0x3A0UL)
/** PR3-0, pixel repetition factor */
#define 	PIX_REP_FACTOR			MSK(4, 24)
/** Register address: auxiliary video information info frame */
#define RA_PDEC_AVI_PB         	(0x3A4UL)
/** VIC6-0, video mode identification code */
#define 	VID_IDENT_CODE			MSK(7, 24)
/** ITC, IT content */
#define		IT_CONTENT				BIT(23)
/** EC2-0, extended colorimetry */
#define		EXT_COLORIMETRY			MSK(3, 20)
/** Q1-0, RGB quantization range */
#define		RGB_QUANT_RANGE			MSK(2, 18)
/** SC1-0, non-uniform scaling information */
#define		NON_UNIF_SCALE			MSK(2, 16)
/** C1-0, colorimetry information */
#define		COLORIMETRY				MSK(2, 14)
/** M1-0, picture aspect ratio */
#define		PIC_ASPECT_RATIO		MSK(2, 12)
/** R3-0, active format aspect ratio */
#define		ACT_ASPECT_RATIO		MSK(4, 8)
/** Y1-0, video format */
#define		VIDEO_FORMAT			MSK(2, 5)
/** A0, active format information present */
#define		ACT_INFO_PRESENT		BIT(4)
/** B1-0, bar valid information */
#define		BAR_INFO_VALID			MSK(2, 2)
/** S1-0, scan information from packet extraction */
#define		SCAN_INFO				MSK(2, 0)
/** Register address: auxiliary video information info frame */
#define RA_PDEC_AVI_TBB        	(0x3A8UL)
/** Line number to start of bottom bar */
#define		LIN_ST_BOT_BAR			MSK(16, 16)
/** Line number to end of top bar */
#define		LIN_END_TOP_BAR			MSK(16, 0)
/** Register address: auxiliary video information info frame */
#define RA_PDEC_AVI_LRB        	(0x3ACUL)
/** Pixel number of start right bar */
#define		PIX_ST_RIG_BAR			MSK(16, 16)
/** Pixel number of end left bar */
#define		PIX_END_LEF_BAR			MSK(16, 0)
/** Register address: special audio layout control for multi-channel audio */
#define RA_PDEC_AIF_CTRL       	(0x3C0UL)
/** Register address: audio info frame */
#define RA_PDEC_AIF_HB         	(0x3C4UL)
/** Register address: audio info frame */
#define RA_PDEC_AIF_PB0        	(0x3C8UL)
/** CA7-0, channel/speaker allocation */
#define 	CH_SPEAK_ALLOC			MSK(8, 24)
/** CTX, coding extension */
#define 	AIF_DATA_BYTE_3			MSK(8, 16)
/** SF2-0, sample frequency */
#define 	SAMPLE_FREQ				MSK(3, 10)
/** SS1-0, sample size */
#define 	SAMPLE_SIZE				MSK(2, 8)
/** CT3-0, coding type */
#define 	CODING_TYPE				MSK(4, 4)
/** CC2-0, channel count */
#define 	CHANNEL_COUNT			MSK(3, 0)
/** Register address: audio info frame */
#define RA_PDEC_AIF_PB1       	(0x3CCUL)
/** DM_INH, down-mix inhibit */
#define 	DWNMIX_INHIBIT			BIT(7)
/** LSV3-0, level shift value */
#define 	LEVEL_SHIFT_VAL			MSK(4, 3)
/** Register address: gamut sequence number */
#define RA_PDEC_GMD_HB         	(0x3D0UL)
/** Register address: gamut meta data */
#define RA_PDEC_GMD_PB         	(0x3D4UL)
/*
 * DTL Interface registers
 */
/** Register address: dummy register for testing */
#define RA_DUMMY_IP_REG        	(0xF00UL)
/*
 * Packet Decoder Interrupt registers
 */
/** Register address: packet decoder interrupt clear enable */
#define RA_PDEC_IEN_CLR        	(0xF78UL)
/** Register address: packet decoder interrupt set enable */
#define RA_PDEC_IEN_SET        	(0xF7CUL)
/** Register address: packet decoder interrupt status */
#define RA_PDEC_ISTS           	(0xF80UL)
/** Register address: packet decoder interrupt enable */
#define RA_PDEC_IEN            	(0xF84UL)
/** Register address: packet decoder interrupt clear status */
#define RA_PDEC_ICLR           	(0xF88UL)
/** Register address: packet decoder interrupt set status */
#define RA_PDEC_ISET           	(0xF8CUL)
/** DVI detection status */
#define		DVIDET					BIT(28)
/** AIF checksum changed */
#define		AIF_CKS_CHG				BIT(25)
/** AVI checksum changed */
#define		AVI_CKS_CHG				BIT(24)
/** Packet FIFO new entry */
#define		PD_FIFO_NEW_ENTRY		BIT(8)
/** Packet FIFO overflow */
#define		PD_FIFO_OVERFL			BIT(4)
/** Packet FIFO underflow */
#define		PD_FIFO_UNDERFL			BIT(3)
/*
 * Audio Clock Interrupt registers
 */
/** Register address: audio clock interrupt clear enable */
#define RA_AUD_CLK_IEN_CLR     	(0xF90UL)
/** Register address: audio clock interrupt set enable */
#define RA_AUD_CLK_IEN_SET     	(0xF94UL)
/** Register address: audio clock interrupt status */
#define RA_AUD_CLK_ISTS        	(0xF98UL)
/** Register address: audio clock interrupt enable */
#define RA_AUD_CLK_IEN         	(0xF9CUL)
/** Register address: audio clock interrupt clear status */
#define RA_AUD_CLK_ICLR        	(0xFA0UL)
/** Register address: audio clock interrupt set status */
#define RA_AUD_CLK_ISET        	(0xFA4UL)
/*
 * Audio FIFO Interrupt registers
 */
/** Register address: audio FIFO interrupt clear enable */
#define RA_AUD_FIFO_IEN_CLR    	(0xFA8UL)
/** Register address: audio FIFO interrupt set enable */
#define RA_AUD_FIFO_IEN_SET    	(0xFACUL)
/** Register address: audio FIFO interrupt status */
#define RA_AUD_FIFO_ISTS       	(0xFB0UL)
/** Register address: audio FIFO interrupt enable */
#define RA_AUD_FIFO_IEN        	(0xFB4UL)
/** Register address: audio FIFO interrupt clear status */
#define RA_AUD_FIFO_ICLR       	(0xFB8UL)
/** Register address: audio FIFO interrupt set status */
#define RA_AUD_FIFO_ISET       	(0xFBCUL)
/** Audio FIFO overflow interrupt */
#define		AFIF_OVERFL				BIT(4)
/** Audio FIFO underflow interrupt */
#define		AFIF_UNDERFL			BIT(3)
/*
 * Mode Detection Interrupt registers
 */
#define RA_HDCP22_IEN_CLR		(0xF60)
#define RA_HDCP22_IEN_SET		(0xF64)
#define RA_HDCP22_IEN_ISTS		(0xF68)
#define RA_HDCP22_IEN			(0xF6C)
#define RA_HDCP22_ICLR			(0xF70)
#define RA_HDCP22_ISET			(0xF74)

#define HDCP22_CAP				BIT(0)
#define HDCP22_NCAP				BIT(1)
#define HDCP22_AUTH_LOST		BIT(2)
#define HDCP22_AUTH_ISTS		BIT(3)
#define HDCP22_AUTH_FAIL		BIT(4)
#define HDCP22_DECR_CHANGE		BIT(5)


/** Register address: mode detection interrupt clear enable */
#define RA_MD_IEN_CLR          	(0xFC0UL)
/** Register address: mode detection interrupt set enable */
#define RA_MD_IEN_SET          	(0xFC4UL)
/** Register address: mode detection interrupt status */
#define RA_MD_ISTS             	(0xFC8UL)
/** Register address: mode detection interrupt enable */
#define RA_MD_IEN              	(0xFCCUL)
/** Register address: mode detection interrupt clear status */
#define RA_MD_ICLR             	(0xFD0UL)
/** Register address: mode detection interrupt set status */
#define RA_MD_ISET             	(0xFD4UL)
/** Video mode interrupts */
#define		VIDEO_MODE				(MSK(3,9) | MSK(1,6) | BIT(3))
/*
 * HDMI Interrupt registers
 */
/** Register address: HDMI interrupt clear enable */
#define RA_HDMI_IEN_CLR        	(0xFD8UL)
/** Register address: HDMI interrupt set enable */
#define RA_HDMI_IEN_SET        	(0xFDCUL)
/** Register address: HDMI interrupt status */
#define RA_HDMI_ISTS           	(0xFE0UL)
/** Register address: HDMI interrupt enable */
#define RA_HDMI_IEN            	(0xFE4UL)
/** Register address: HDMI interrupt clear status */
#define RA_HDMI_ICLR           	(0xFE8UL)
/** Register address: HDMI interrupt set status */
#define RA_HDMI_ISET			(0xFECUL)
/** AKSV receive interrupt */
#define AKSV_RCV				BIT(25)
/** Deep color mode change interrupt */
#define 	DCM_CURRENT_MODE_CHG	BIT(16)
/** Clock change interrupt */
#define 	CLK_CHANGE				MSK(2,5)/*BIT(6)*/

/* Won.Hur */
#define		PLL_LOCK_STATUS			BIT(5)
#define 	PHY_I2C_DONE			BIT(28)
#define		PHY_I2C_NACK			BIT(29)

#define		SCDC_0					BIT(19)
#define		SCDC_1					BIT(18)


/*
 * DMI registers
 */
/** Register address: DMI software reset */
#define RA_DMI_SW_RST          (0xFF0UL)
/** Register address: DMI disable interface */
#define RA_DMI_DISABLE_IF      (0xFF4UL)
/** Register address: DMI module ID */
#define RA_DMI_MODULE_ID       (0xFFCUL)


extern int synopsys_phy_disable;
extern int gVideoStableCheckCnt;
/* ***************************************************************************
 * Data Manipulation and Access
 * ***************************************************************************/
/**
 * Find first (least significant) bit set
 * @param[in] data word to search
 * @return bit position or 32 if none is set
 */
static unsigned first_bit_set(uint32_t data)
{
	unsigned n = 32;

	if (data != 0) {
		for (n = 0; (data & 1) == 0; n++) {
			data >>= 1;
		}
	}
	return n;
}

/**
 * Get bit field
 * @param[in] data raw data
 * @param[in] mask bit field mask
 * @return bit field value
 */
static uint32_t get(uint32_t data, uint32_t mask)
{
	return (data & mask) >> first_bit_set(mask);
}

/**
 * Set bit field
 * @param[in] data raw data
 * @param[in] mask bit field mask
 * @param[in] value new value
 * @return new raw data
 */
static uint32_t set(uint32_t data, uint32_t mask, uint32_t value)
{
	return ((value << first_bit_set(mask)) & mask) | (data & ~mask);
}

/**
 * Read bit field from device
 * @param[in,out] ctx context information
 * @param[in] addr bus address
 * @param[in] mask bitfield mask
 * @return zero if error or bit field value
 */
static uint32_t io_read(struct hdmi_rx_ctrl *ctx, uint16_t addr, uint32_t mask)
{
	return (ctx == 0)? 0 : get(ctx->bsp_read(addr), mask);
}

/**
 * Write bit field into device
 * @param[in,out] ctx context information
 * @param[in] addr bus address
 * @param[in] mask bit field mask
 * @param[in] value new value
 * @return error code
 */
static int io_write(struct hdmi_rx_ctrl *ctx, uint16_t addr, uint32_t mask, uint32_t value)
{
	int error = 0;
	unsigned long flags = 0;
	if (ctx == 0) {
		return -EINVAL;
	}
	error |= ctx->spin_lock_irq_save(&ctx->spin, &flags);
	if (error == 0) {
		ctx->bsp_write(addr, set(ctx->bsp_read(addr), mask, value));
		error |= ctx->spin_unlock_irq_restore(&ctx->spin, flags);
	}
	return error;
}

/* ***************************************************************************
 * Auxiliary
 * ***************************************************************************/
/**
 * Configure audio after reset (default settings)
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_audio_cfg(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	error |= io_write(ctx, RA_AUD_MUTE_CTRL, AUD_MUTE_SEL, AUD_MUTE_FORCE_UN);
	/* enable all outputs and select 32-bit for I2S */
	ctx->bsp_write(RA_AUD_SAO_CTRL, 1);
	return error;
}

/**
 * Reset audio FIFO
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_audio_fifo_rst(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}

	//ctx->log_info("Port[%d] : Audio FIFO reset", ctx->device_id);	

	error |= io_write(ctx, RA_AUD_FIFO_CTRL, AFIF_INIT, 1);
	error |= io_write(ctx, RA_AUD_FIFO_CTRL, AFIF_INIT, 0);
	ctx->debug_audio_fifo_rst++;
	return error;
}

/**
 * Configure legal clock range
 * @param[in,out] ctx context information
 * @param[in] min TMDS clock minimum
 * @param[in] max TMDS clock maximum
 * @return error code
 */
static int control_clk_range(struct hdmi_rx_ctrl *ctx, unsigned long min, unsigned long max)
{
	int error = 0;
	unsigned evaltime = 0;
	unsigned long ref_clk;
	if (ctx == 0 || min > max) {
		return -EINVAL;
	}
#ifdef CTRL_13xa  /* RX Controller Release 1.30a and above */
	ref_clk = ctx->md_clk;
#else	/* RX Controller Release 1.20a */
	ref_clk = ctx->cfg_clk;
#endif
	evaltime = (ref_clk * 4095) / ref_clk;
	min = (min * evaltime) / ref_clk;
	max = (max * evaltime) / ref_clk;
	error |= io_write(ctx, RA_HDMI_CKM_F, MINFREQ, min);
	error |= io_write(ctx, RA_HDMI_CKM_F, MAXFREQ, max);
	return error;
}

/**
 * Configure control after reset (default settings)
 * @param[in,out] ctx context information
 * @param[in] port input port 0 (A) or 1 (B)
 * @return error code
 */
static int control_cfg(struct hdmi_rx_ctrl *ctx, unsigned port)
{
	int error = 0;
	unsigned evaltime = 0;
	if (ctx == 0) {
		return -EINVAL;
	}
#ifdef CTRL_13xa /* RX Controller Release 1.30a and above */
	evaltime = (ctx->md_clk * 4095) / ctx->md_clk;
#else	/* RX Controller Release 1.20a */
	evaltime = (ctx->cfg_clk * 4095) / 158000;
#endif
	ctx->bsp_write(RA_HDMI_OVR_CTRL, ~0);	/* enable all */
	error |= io_write(ctx, RA_HDMI_SYNC_CTRL, VS_POL_ADJ_MODE, VS_POL_ADJ_AUTO);
	error |= io_write(ctx, RA_HDMI_SYNC_CTRL, HS_POL_ADJ_MODE, HS_POL_ADJ_AUTO);
	error |= io_write(ctx, RA_HDMI_CKM_EVLTM, EVAL_TIME, evaltime);
	error |= control_clk_range(ctx, TMDS_CLK_MIN, TMDS_CLK_MAX);
	/* bit field shared between phy and controller */
	error |= io_write(ctx, RA_HDMI_PCB_CTRL, INPUT_SELECT, port);
	error |= io_write(ctx, RA_SNPS_PHYG3_CTRL, ((1 << 2) - 1) << 2, port);
	return error;
}

/**
 * Enable/disable all interrupts
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable interrupts
 * @return error code
 */
static int interrupts_cfg(struct hdmi_rx_ctrl *ctx, bool enable)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (enable) {
		/* set enable */
		ctx->bsp_write(RA_PDEC_IEN_SET, ~0);
		ctx->bsp_write(RA_AUD_CLK_IEN_SET, ~0);
		ctx->bsp_write(RA_AUD_FIFO_IEN_SET, ~0);
		ctx->bsp_write(RA_MD_IEN_SET, ~0);
		ctx->bsp_write(RA_HDMI_IEN_SET, ~0);
		ctx->bsp_write(RA_HDCP22_IEN_SET, ~0);
	} else {
		/* clear enable */
		ctx->bsp_write(RA_PDEC_IEN_CLR, ~0);
		ctx->bsp_write(RA_AUD_CLK_IEN_CLR, ~0);
		ctx->bsp_write(RA_AUD_FIFO_IEN_CLR, ~0);
		ctx->bsp_write(RA_MD_IEN_CLR, ~0);
		ctx->bsp_write(RA_HDMI_IEN_CLR, ~0);
		ctx->bsp_write(RA_HDCP22_IEN_CLR, ~0);
		/* clear status */
		ctx->bsp_write(RA_PDEC_ICLR, ~0);
		ctx->bsp_write(RA_AUD_CLK_ICLR, ~0);
		ctx->bsp_write(RA_AUD_FIFO_ICLR, ~0);
		ctx->bsp_write(RA_MD_ICLR, ~0);
		ctx->bsp_write(RA_HDMI_ICLR, ~0);
		ctx->bsp_write(RA_HDCP22_ICLR, ~0);
	}
	return error;
}

/**
 * Enable/disable interrupts after HPD change
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable interrupts
 * @return error code
 */
static int interrupts_hpd(struct hdmi_rx_ctrl *ctx, bool enable)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (enable) {
		/* set enable */
		ctx->bsp_write(RA_PDEC_IEN_SET, DVIDET|AIF_CKS_CHG|AVI_CKS_CHG|PD_FIFO_NEW_ENTRY|PD_FIFO_OVERFL);
		ctx->bsp_write(RA_AUD_FIFO_IEN_SET, AFIF_OVERFL|AFIF_UNDERFL);
		ctx->bsp_write(RA_MD_IEN_SET, VIDEO_MODE);
		ctx->bsp_write(RA_HDCP22_IEN_SET, HDCP22_CAP|HDCP22_NCAP|HDCP22_AUTH_LOST|HDCP22_AUTH_ISTS|HDCP22_AUTH_FAIL|HDCP22_DECR_CHANGE);
		ctx->bsp_write(RA_HDMI_IEN_SET, AKSV_RCV|DCM_CURRENT_MODE_CHG|PLL_LOCK_STATUS|PHY_I2C_DONE|PHY_I2C_NACK|SCDC_0|SCDC_1 );
	} else {
		/* clear enable */
		ctx->bsp_write(RA_PDEC_IEN_CLR, DVIDET|AIF_CKS_CHG|AVI_CKS_CHG|PD_FIFO_NEW_ENTRY|PD_FIFO_OVERFL);
		ctx->bsp_write(RA_AUD_FIFO_IEN_CLR, AFIF_OVERFL|AFIF_UNDERFL);
		ctx->bsp_write(RA_MD_IEN_CLR, VIDEO_MODE);
		ctx->bsp_write(RA_HDMI_IEN_CLR, AKSV_RCV|DCM_CURRENT_MODE_CHG|PLL_LOCK_STATUS|PHY_I2C_DONE|PHY_I2C_NACK |SCDC_0|SCDC_1);
		ctx->bsp_write(RA_HDCP22_IEN_CLR, HDCP22_CAP|HDCP22_NCAP|HDCP22_AUTH_LOST|HDCP22_AUTH_ISTS|HDCP22_AUTH_FAIL|HDCP22_DECR_CHANGE);

		/* clear status */
		ctx->bsp_write(RA_PDEC_ICLR, DVIDET|AIF_CKS_CHG|AVI_CKS_CHG|PD_FIFO_NEW_ENTRY|PD_FIFO_OVERFL);
		ctx->bsp_write(RA_AUD_FIFO_ICLR, AFIF_OVERFL|AFIF_UNDERFL);
		ctx->bsp_write(RA_MD_ICLR, VIDEO_MODE);
		ctx->bsp_write(RA_HDMI_ICLR, AKSV_RCV|DCM_CURRENT_MODE_CHG|PLL_LOCK_STATUS|PHY_I2C_DONE|PHY_I2C_NACK |SCDC_0|SCDC_1);
		ctx->bsp_write(RA_HDCP22_ICLR, HDCP22_CAP|HDCP22_NCAP|HDCP22_AUTH_LOST|HDCP22_AUTH_ISTS|HDCP22_AUTH_FAIL|HDCP22_DECR_CHANGE);
	}
	return error;
}

/**
 * Configure packet decoder after reset (default settings)
 * @param[in,out] ctx context information
 * @return error code
 */
static int packet_cfg(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	ctx->bsp_write(RA_PDEC_CTRL, PFIFO_STORE_FILTER_EN|PD_FIFO_WE|PDEC_BCH_EN);
	ctx->bsp_write(RA_PDEC_ASP_CTRL, AUTO_VMUTE|AUTO_SPFLAT_MUTE);
	return error;
}

/**
 * Reset packet decoder FIFO
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_packet_fifo_rst(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	ctx->log_info("Port[%d] : Packet FIFO reset", ctx->device_id);

	error |= io_write(ctx, RA_PDEC_CTRL, PD_FIFO_FILL_INFO_CLR|PD_FIFO_CLR, ~0);
	error |= io_write(ctx, RA_PDEC_CTRL, PD_FIFO_FILL_INFO_CLR|PD_FIFO_CLR,  0);
	ctx->debug_packet_fifo_rst++;
	return error;
}

/**
 * Read packet AIF parameters
 * @param[in,out] ctx context information
 * @param[out] params audio parameters
 * @return error code
 */
static int packet_get_aif(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_audio *params)
{
	int error = 0;

	if (ctx == 0 || params == 0) {
		return -EINVAL;
	}
	params->coding_type = io_read(ctx, RA_PDEC_AIF_PB0, CODING_TYPE);
	params->channel_count = io_read(ctx, RA_PDEC_AIF_PB0, CHANNEL_COUNT);
	params->sample_frequency = io_read(ctx, RA_PDEC_AIF_PB0, SAMPLE_FREQ);
	params->sample_size = io_read(ctx, RA_PDEC_AIF_PB0, SAMPLE_SIZE);
	params->coding_extension = io_read(ctx, RA_PDEC_AIF_PB0, AIF_DATA_BYTE_3);
	params->channel_allocation = io_read(ctx, RA_PDEC_AIF_PB0, CH_SPEAK_ALLOC);
	params->down_mix_inhibit = io_read(ctx, RA_PDEC_AIF_PB1, DWNMIX_INHIBIT);
	params->level_shift_value = io_read(ctx, RA_PDEC_AIF_PB1, LEVEL_SHIFT_VAL);
	/** @note HW does not support AIF LFEPBL1-0, LFE play back level */
	return error;
}

/**
 * Read packet AVI parameters
 * @param[in,out] ctx context information
 * @param[out] params video parameters
 * @return error code
 */
static int packet_get_avi(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_video *params)
{
	int error = 0;

	if (ctx == 0 || params == 0) {
		return -EINVAL;
	}
	params->video_format = io_read(ctx, RA_PDEC_AVI_PB, VIDEO_FORMAT);
	params->active_valid = io_read(ctx, RA_PDEC_AVI_PB, ACT_INFO_PRESENT);
	params->bar_valid = io_read(ctx, RA_PDEC_AVI_PB, BAR_INFO_VALID);
	params->scan_info = io_read(ctx, RA_PDEC_AVI_PB, SCAN_INFO);
	params->colorimetry = io_read(ctx, RA_PDEC_AVI_PB, COLORIMETRY);
	params->picture_ratio = io_read(ctx, RA_PDEC_AVI_PB, PIC_ASPECT_RATIO);
	params->active_ratio = io_read(ctx, RA_PDEC_AVI_PB, ACT_ASPECT_RATIO);
	params->it_content = io_read(ctx, RA_PDEC_AVI_PB, IT_CONTENT);
	params->ext_colorimetry = io_read(ctx, RA_PDEC_AVI_PB, EXT_COLORIMETRY);
	params->rgb_quant_range = io_read(ctx, RA_PDEC_AVI_PB, RGB_QUANT_RANGE);
	params->n_uniform_scale = io_read(ctx, RA_PDEC_AVI_PB, NON_UNIF_SCALE);
	params->video_mode = io_read(ctx, RA_PDEC_AVI_PB, VID_IDENT_CODE);
	params->pixel_repetition = io_read(ctx, RA_PDEC_AVI_HB, PIX_REP_FACTOR);
	/** @note HW does not support AVI YQ1-0, YCC quantization range */
	/** @note HW does not support AVI CN1-0, IT content type */
	params->bar_end_top = io_read(ctx, RA_PDEC_AVI_TBB, LIN_END_TOP_BAR);
	params->bar_start_bottom = io_read(ctx, RA_PDEC_AVI_TBB, LIN_ST_BOT_BAR);
	params->bar_end_left = io_read(ctx, RA_PDEC_AVI_LRB, PIX_END_LEF_BAR);
	params->bar_start_right = io_read(ctx, RA_PDEC_AVI_LRB, PIX_ST_RIG_BAR);
	return error;
}

/* ***************************************************************************
 * Interface
 * ***************************************************************************/
/**
 * Open device validating context information
 * @param[in,out] ctx context information
 * @return error code
 *
 * @note Call sequence: open => [configure] => close
 */
int hdmi_rx_ctrl_open(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;
	unsigned i = 0;
	if (ctx == 0
			|| ctx->log_error == 0 || ctx->log_info == 0
			|| ctx->bsp_reset == 0 || ctx->bsp_reset_hdmi == 0
			|| ctx->bsp_read == 0 || ctx->bsp_write == 0
			|| ctx->spin_init == 0 || ctx->spin_destroy == 0
			|| ctx->spin_lock_irq_save == 0 || ctx->spin_unlock_irq_restore == 0
			|| ctx->cfg_clk < CFG_CLK_MIN || ctx->cfg_clk > CFG_CLK_MAX
			|| ctx->md_clk < MD_CLK_MIN || ctx->md_clk > MD_CLK_MAX) {
		return -EINVAL;
	}
	if (ctx->status != 0) {		/* is not closed? */
		return -EPERM;
	}
	error |= interrupts_cfg(ctx, false);
	for (i = 0; i < hdmi_rx_ctrl_event_cnt; i++) {
		ctx->event_handler[i] = 0;
	}
	ctx->debug_audio_fifo_rst = 0;
	ctx->debug_packet_fifo_rst = 0;
	ctx->debug_irq_handling = 0;
	ctx->debug_irq_packet_decoder = 0;
	ctx->debug_irq_audio_clock = 0;
	ctx->debug_irq_audio_fifo = 0;
	ctx->debug_irq_video_mode = 0;
	ctx->debug_irq_hdmi = 0;
	ctx->spin = 0;
	error |= ctx->spin_init(&ctx->spin);
	if (error == 0) {
		ctx->status = -1;
	}
	ctx->tmds_clk = 0;
	ctx->bsp_reset(true);
	ctx->bsp_reset(false);

	/* Added by LGE SIC won.hur */
	memset((void *)(&(ctx->previous_video)), 0, sizeof(struct hdmi_rx_ctrl_video));

	return error;
}

/**
 * Configure device
 * @param[in,out] ctx context information
 * @param[in] port input port 0 (A) or 1 (B)
 * @return error code
 *
 * @note Call sequence: open => [configure] => close
 */
int hdmi_rx_ctrl_config(struct hdmi_rx_ctrl *ctx, unsigned port)
{
	int error = 0;
	unsigned i = 0;

	if (ctx == 0 || port > 3) {
		return -EINVAL;
	}
	if (ctx->status == 0) {		/* is closed? */
		return -EPERM;
	}
	error |= interrupts_cfg(ctx, false);
	ctx->status = -1;
	ctx->tmds_clk = 0;
	if (ctx->bsp_edid_update != 0) {
		error |= ctx->bsp_edid_update();
	}
	error |= control_cfg(ctx, port);
	error |= hdmi_rx_ctrl_audio_cfg(ctx);
	error |= packet_cfg(ctx);
	error |= hdmi_rx_ctrl_audio_fifo_rst(ctx);
	error |= hdmi_rx_ctrl_packet_fifo_rst(ctx);
	for (i = 0; i < hdmi_rx_ctrl_event_cnt; i++) {
		ctx->event_handler[i] = 0;
	}
	//ctx->bsp_write(RA_HDMI_MODE_RECOVER, 0x2100);
	//ctx->bsp_write(RA_MD_VTH, 0xfff);
	if (error == 0) {
		/* HPD workaround */
		ctx->bsp_write(RA_HDMI_IEN_SET, CLK_CHANGE);
		ctx->status = 1;
	}
	/* Filter all packets with errors */
	ctx->bsp_write(RA_PDEC_ERR_FILTER,0x1FF);
	return error;
}

/**
 * Interrupt handler
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_irq_handler(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;
	unsigned i = 0;
	uint32_t intr = 0;
	uint32_t data = 0;
	unsigned long tclk = 0;
	unsigned long ref_clk;
	unsigned evaltime = 0;
	bool execute[hdmi_rx_ctrl_event_cnt];

	//ctx->log_info("Rx Interrupt triggered.");

#ifdef CTRL_13xa  /* RX Controller Release 1.30a and above */
	ref_clk = ctx->md_clk;
#else	/* RX Controller Release 1.20a */
	ref_clk = ctx->cfg_clk;
#endif
	if (ctx == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	for (i = 0; i < hdmi_rx_ctrl_event_cnt; i++) {
		execute[i] = false;
	}
	intr = ctx->bsp_read(RA_HDMI_ISTS) & ctx->bsp_read(RA_HDMI_IEN);
	//ctx->log_info("1.RX INTERRUPT TRIGGERED: 0x%x", intr);
	//ctx->log_info("HDMI Rx Demo: HDMI related event 0x%x", intr);
	if (intr != 0) {
		ctx->bsp_write(RA_HDMI_ICLR, intr);
		if (get(intr, CLK_CHANGE) != 0) {
			execute[hdmi_rx_ctrl_event_clock_change] = true;
			//ctx->log_info("HDMI Rx Demo: clock change related event 0x%x", intr);
			evaltime = (ref_clk * 4095) / ref_clk;
			data = ctx->bsp_read(RA_HDMI_CKM_RESULT);
			tclk = ((ref_clk * get(data, CLKRATE)) / evaltime);
#if 1
			if (tclk == 0) {
				error |= interrupts_hpd(ctx, false);
				error |= control_clk_range(ctx, TMDS_CLK_MIN, TMDS_CLK_MAX);
			}
			else {
				for (i = 0; i < TMDS_STABLE_TIMEOUT; i++) { /* time for TMDS to stabilise */
					;
				}
				tclk = ((ref_clk * get(data, CLKRATE)) / evaltime);
				//	error |= control_clk_range(ctx, tclk - TMDS_CLK_DELTA, tclk + TMDS_CLK_DELTA);
				error |= interrupts_hpd(ctx, true);
			}
#endif
			ctx->tmds_clk = tclk;
			//ctx->log_info("----------------------------------- tmds_clk %d", ctx->tmds_clk);
		}
		if (get(intr, DCM_CURRENT_MODE_CHG) != 0) {
#ifdef CTRL_13xa  /* RX Controller Release 1.30a and above */

			if(ctx->bsp_read(RA_PDEC_GCP_AVMUTE) & 0x00000002){
				ctx->log_info("--%d--------------------------------- GCP AVMUTE 0x%08x",ctx->device_id, ctx->bsp_read(RA_PDEC_GCP_AVMUTE));
				ctx->isMute = 1;
				ctx->event_handler[hdmi_rx_ctrl_event_avmute_change](ctx);
				execute[hdmi_rx_ctrl_event_avmute_change] = false;
			}
			else if(ctx->bsp_read(RA_PDEC_GCP_AVMUTE) & 0x00000001){
				ctx->log_info("--%d--------------------------------- GCP AVMUTE 0x%08x", ctx->device_id, ctx->bsp_read(RA_PDEC_GCP_AVMUTE));
				ctx->isMute = 0;
				ctx->event_handler[hdmi_rx_ctrl_event_avmute_change](ctx);
				execute[hdmi_rx_ctrl_event_avmute_change] = false;
			}
#if 0
			if (((ctx->bsp_read(RA_PDEC_GCP_AVMUTE) >> 4) & 0xf) > ((ctx->bsp_read(RA_HDMI_STS) >> 28) & 0xf))
			{	/* WA - 24-bit DCM fall back */
				ctx->bsp_write(RA_DMI_SW_RST, ((1 << 3) - 1) << 1);
			}
#endif
			execute[hdmi_rx_ctrl_event_dcm_change] = true;


#endif
			
			//ctx->log_info("HDMI Rx Demo: video DCM related event 0x%x", intr);
		}
		if (get(intr, AKSV_RCV) != 0) {
			execute[hdmi_rx_ctrl_event_aksv_reception] = true;
			//ctx->log_info("HDMI Rx Demo: aksv related event 0x%x", intr);
		}
		if (get(intr, PLL_LOCK_STATUS) != 0) {
			execute[hdmi_rx_ctrl_event_pll_lock_change] = true;
		}
		if (get(intr, PHY_I2C_DONE) != 0){
			execute[hdmi_rx_ctrl_event_phy_i2c_done] = true;
		}
		if (get(intr, PHY_I2C_NACK) != 0){
			execute[hdmi_rx_ctrl_event_phy_i2c_nack] = true;
		}
		if ((get(intr, SCDC_0) !=0) || (get(intr, SCDC_1))){
			execute[hdmi_rx_ctrl_event_scdc] = true;
		}

		ctx->debug_irq_hdmi++;
	}
	intr = ctx->bsp_read(RA_MD_ISTS) & ctx->bsp_read(RA_MD_IEN);
	//ctx->log_info("2.RX INTERRUPT TRIGGERED: 0x%x", intr);
	if (intr != 0) {
		ctx->bsp_write(RA_MD_ICLR, intr);
		if (get(intr, VIDEO_MODE) != 0) {
			execute[hdmi_rx_ctrl_event_video_status] = true;
			//ctx->log_info("HDMI Rx Demo: video RA_MD_ISTS related event 0x%x", intr);
		}
		ctx->debug_irq_video_mode++;
	}
	intr = ctx->bsp_read(RA_PDEC_ISTS) & ctx->bsp_read(RA_PDEC_IEN);
	//ctx->log_info("3.RX INTERRUPT TRIGGERED: 0x%x", intr);
	if (intr != 0) {
		ctx->bsp_write(RA_PDEC_ICLR, intr);
		if (get(intr, DVIDET|AVI_CKS_CHG) != 0) {
			execute[hdmi_rx_ctrl_event_video_status] = true;
			//ctx->log_info("HDMI Rx Demo: video related event 0x%x", intr);
		}
		if (get(intr, AIF_CKS_CHG) != 0) {
			execute[hdmi_rx_ctrl_event_audio_status] = true;
			//ctx->log_info("HDMI Rx Demo: audio AIF_CKS_CHG related event 0x%x", intr);
		}
		if (get(intr, PD_FIFO_NEW_ENTRY) != 0) {
			execute[hdmi_rx_ctrl_event_packet_reception] = true;
			//ctx->log_info("HDMI Rx Demo: packet reception related event 0x%x", intr);
		}
		if (get(intr, PD_FIFO_OVERFL) != 0) {
			error |= hdmi_rx_ctrl_packet_fifo_rst(ctx);
		}
		ctx->debug_irq_packet_decoder++;
	}
	intr = ctx->bsp_read(RA_AUD_CLK_ISTS) & ctx->bsp_read(RA_AUD_CLK_IEN);
	//ctx->log_info("HDMI Rx Demo: Audio related Event 0x%x", ctx->bsp_read(RA_AUD_CLK_ISTS));
	//ctx->log_info("RA_AUD_CLK_IEN 0x%x", ctx->bsp_read(RA_AUD_CLK_IEN));
	//ctx->log_info("4.RX INTERRUPT TRIGGERED: 0x%x", intr);
	if (intr != 0) {
		ctx->bsp_write(RA_AUD_CLK_ICLR, intr);
		ctx->debug_irq_audio_clock++;
	}
	intr = ctx->bsp_read(RA_AUD_FIFO_ISTS) & ctx->bsp_read(RA_AUD_FIFO_IEN);
	//ctx->log_info("HDMI Rx Demo: Audio Fifo related event 0x%x", intr);
	if (intr != 0) {
		if (get(intr, AFIF_OVERFL|AFIF_UNDERFL) != 0) {
			error |= hdmi_rx_ctrl_audio_fifo_rst(ctx);
		}
		ctx->bsp_write(RA_AUD_FIFO_ICLR, intr);
		ctx->debug_irq_audio_fifo++;
	}
	intr = ctx->bsp_read(RA_HDCP22_IEN_ISTS) & ctx->bsp_read(RA_HDCP22_IEN);
	//ctx->log_info("HDMI Rx Demo: Audio Fifo related event 0x%x", intr);
	if (intr != 0) {
		if (get(intr, HDCP22_CAP) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_cap] = true;
		}
		if (get(intr, HDCP22_NCAP) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_ncap] = true;
		}
		if (get(intr, HDCP22_AUTH_LOST) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_auth_lost] = true;
		}
		if (get(intr, HDCP22_AUTH_ISTS) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_auth_ists] = true;
		}
		if (get(intr, HDCP22_AUTH_FAIL) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_auth_fail] = true;
		}
		if (get(intr, HDCP22_DECR_CHANGE) != 0) {
			execute[hdmi_rx_ctrl_event_hdcp22_auth_change] = true;
		}

		/* Clear Interrupt */
		ctx->bsp_write(RA_HDCP22_ICLR, intr);
	}

	ctx->debug_irq_handling++;
	for (i = 0; i < hdmi_rx_ctrl_event_cnt; i++) {
		if (execute[i] && ctx->event_handler[i] != 0) {
			ctx->event_handler[i](ctx);
			//ctx->log_info("Rx event number: %d", i);
		}
	}
	return error;
}

/**
 * Enable/disable hot plug detect output
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable output
 * @return error code
 */
int hdmi_rx_ctrl_hpd(struct hdmi_rx_ctrl *ctx, bool enable)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	error |= io_write(ctx, RA_HDMI_SETUP_CTRL, HOT_PLUG_DETECT, enable? 1 : 0);
	return error;
}

/**
 * Subscribe event notification. Override previous subscription.
 * @param[in,out] ctx context information
 * @param[in] type event type
 * @param[in] handler event handler, 0 to unsubscribe
 * @return error code
 */
int hdmi_rx_ctrl_subscribe(struct hdmi_rx_ctrl *ctx, enum hdmi_rx_ctrl_event_t type, int (*handler)(struct hdmi_rx_ctrl *))
{
	int error = 0;
	unsigned long flags = 0;
	if (ctx == 0 || type >= hdmi_rx_ctrl_event_cnt) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	error |= ctx->spin_lock_irq_save(&ctx->spin, &flags);
	/* Behavior
	 * 1) Enabling:  nothing to declare
	 * 2) Disabling: an event can occur till store be disabled but
	 *    nothing will be done because interrupt checks null handler
	 * 3) Replacing: handler may be executed sooner
	 */
	if (error == 0) {
		ctx->event_handler[type] = handler;
		error |= ctx->spin_unlock_irq_restore(&ctx->spin, flags);
	}
	return error;
}

/**
 * Configure packet storage on Ctrl FIFO
 * @param[in,out] ctx context information
 * @param[in] type packet type
 * @param[in] enable enable/disable packet store
 * @return error code
 *
 * @note Subscribe packet reception event first
 */
int hdmi_rx_ctrl_store(struct hdmi_rx_ctrl *ctx, enum hdmi_rx_ctrl_packet_t type, bool enable)
{
	int error = 0;

	if (ctx == 0 || type >= hdmi_rx_ctrl_packet_cnt) {
		return -EINVAL;
	}
	if (ctx->status <= 0		/* is not configured? */
			&& ctx->event_handler[hdmi_rx_ctrl_event_packet_reception] == 0) {
		return -EPERM;
	}

	error |= io_write(ctx, RA_PDEC_CTRL, PFIFO_STORE_PACKET << type, enable? 1 : 0);
	return error;
}

/**
 * Get TDMS clock frequency [kHz]
 * @param[in,out] ctx context information
 * @return error code or value
 */
long hdmi_rx_ctrl_get_tmds_clk(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	return (error != 0)? error : ctx->tmds_clk;
}

/**
 * Read input audio parameters
 * @param[in,out] ctx context information
 * @param[out] params audio parameters
 * @return error code
 *
 * @note If EGAIN is returned,
 * @note clear interrupts before call it again
 * @note or let interrupt handler run in between
 */
int hdmi_rx_ctrl_get_audio(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_audio *params)
{
	int error = 0;

	if (ctx == 0 || params == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	error |= packet_get_aif(ctx, params);
	if (error != 0) {
		goto exit;
	}
	if (io_read(ctx, RA_PDEC_ISTS, AIF_CKS_CHG) != 0) {
		error = -EAGAIN;
		goto exit;
	}
	exit:
	return error;
}

/**
 * Get packet latest read from FIFO
 * @param[in,out] ctx context information
 * @param[out] data buffer for packet data
 * @param[in] size expected packet size
 * @return error code
 *
 * @note Info frames checksum should be verified
 */
int hdmi_rx_ctrl_get_packet(struct hdmi_rx_ctrl *ctx, uint8_t *data, size_t size)
{
	int error = 0;
	const unsigned max = 32;
	uint32_t tmp = 0;
	unsigned i = 0;

	if (ctx == 0 || size > max) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	if (io_read(ctx, RA_PDEC_STS, PD_FIFO_NEW_ENTRY) == 0) {
		error |= io_write(ctx, RA_PDEC_CTRL, PD_FIFO_SKIP, 1);	/* re-align */
		error |= io_write(ctx, RA_PDEC_CTRL, PD_FIFO_SKIP, 0);
	}

	/* Check FIFO, and continue only if new data has arrived */
	if (io_read(ctx, RA_PDEC_STS, PD_FIFO_NEW_ENTRY) == 0) {
		error = -ENODATA;
		goto exit;
	}
	for (i = 0; i < max; i++) {
		if ((i % 4) == 0) {
			/*RA_PDEC_FIFO_DATA register data is auto increament after read */
			tmp = ctx->bsp_read(RA_PDEC_FIFO_DATA);
		}
		if (io_read(ctx, RA_PDEC_STS, PD_FIFO_UNDERFL) != 0) {
			error = -ENODATA;
			goto exit;
		}
		if (i < size) {
			data[i] = (uint8_t)tmp;
		}
		tmp >>= 8;
	}
	exit:
	return error;
}

/**
 * Read input video parameters
 * @param[in,out] ctx context information
 * @param[out] params video parameters
 * @return error code
 *
 * @note If EGAIN is returned,
 * @note clear interrupts before call it again
 * @note or let interrupt handler run in between
 */
int hdmi_rx_ctrl_get_video(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_video *params, uint8_t verbose)
{
	int error = 0;
	const unsigned factor = 100;
	unsigned divider = 0;
	uint32_t tmp = 0;

	if (ctx == 0 || params == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is not configured? */
		return -EPERM;
	}
	/* DVI mode */
	params->dvi = io_read(ctx, RA_PDEC_STS, DVIDET) != 0;
	/* AVI parameters */
	error |= packet_get_avi(ctx, params);
	if (error != 0) {
		goto exit;
	}
	/* pixel clock */
	params->pixel_clk = ctx->tmds_clk;
	/* image parameters */
	params->interlaced = io_read(ctx, RA_MD_STS, ILACE) != 0;
	params->voffset = io_read(ctx, RA_MD_VOL, VOFS_LIN);
	params->vactive = io_read(ctx, RA_MD_VAL, VACT_LIN);
	params->vtotal = io_read(ctx, RA_MD_VTL, VTOT_LIN);
	/* refresh rate */
	tmp = io_read(ctx, RA_MD_VTC, VTOT_CLK);
	if (params->interlaced)	{
		params->voffset <<= 1;
		params->vactive <<= 1;
		params->vtotal <<= 1;
		tmp <<= 1;
	}
	params->refresh_rate = (tmp == 0)? 0: (ctx->md_clk * 100000) / tmp;
	if(params->video_format == 3) /* YCC420 */
	{
		params->refresh_rate = (unsigned long)(params->refresh_rate)*2;
	}
	params->hoffset = io_read(ctx, RA_MD_HT1, HOFS_PIX);
	params->hactive = io_read(ctx, RA_MD_HACT_PX, HACT_PIX);
	params->htotal = io_read(ctx, RA_MD_HT1, HTOT_PIX);
	/* deep color mode */
	tmp = io_read(ctx, RA_HDMI_STS, DCM_CURRENT_MODE);

	switch (tmp) {
	case DCM_CURRENT_MODE_48b:
		params->deep_color_mode = 48;
		divider = 2.00 * factor;	/* divide by 2 */
		break;
	case DCM_CURRENT_MODE_36b:
		params->deep_color_mode = 36;
		divider = 1.50 * factor;	/* divide by 1.5 */
		break;
	case DCM_CURRENT_MODE_30b:
		params->deep_color_mode = 30;
		divider = 1.25 * factor;	/* divide by 1.25 */
		break;
	default:
		params->deep_color_mode = 24;
		divider = 1.00 * factor;
		break;
	}
	if(params->video_format == 3) /* YCC420 */
	{
		params->pixel_clk = params->pixel_clk*2;
		params->hoffset = params->hoffset*2;
		params->hactive	= params->hactive*2;
		params->htotal = params->htotal*2;
	}
	params->pixel_clk = (params->pixel_clk * factor) / divider;
	params->hoffset = (params->hoffset * factor) / divider;
	params->hactive	= (params->hactive * factor) / divider;
	params->htotal = (params->htotal  * factor) / divider;
	if (params->pixel_clk < PIXEL_CLK_MIN || params->pixel_clk > PIXEL_CLK_MAX
			|| params->hactive < HRESOLUTION_MIN
			|| params->hactive > HRESOLUTION_MAX
			|| params->htotal < (params->hactive + params->hoffset)
			|| params->vactive < VRESOLUTION_MIN
			|| params->vactive > VRESOLUTION_MAX
			|| params->vtotal < (params->vactive + params->voffset)
			|| params->refresh_rate < REFRESH_RATE_MIN
			|| params->refresh_rate > REFRESH_RATE_MAX) {

		if(verbose == 4)ctx->log_info("Bad Video! Port[%d] PixelClk[%d], Hactive[%d], Htotal[%d], Vactive[%d], Vtotal[%d], RefreshRate[%d]\n",\
				ctx->device_id,params->pixel_clk, params->hactive, params->htotal, params->vactive, params->vtotal, params->refresh_rate);
		error = -ERANGE;
		goto exit;
	}
	else{
		if((ctx->stable_cnt <= gVideoStableCheckCnt)&&(verbose > 0))
			if(verbose != 1)ctx->log_info("Good Video![v=%d] Port[%d] PixelClk[%d], Hactive[%d], Htotal[%d], Vactive[%d], Vtotal[%d], RefreshRate[%d]\n",\
				verbose,ctx->device_id,params->pixel_clk, params->hactive, params->htotal, params->vactive, params->vtotal, params->refresh_rate);
	}
	exit:
	return error;
}

/**
 * Debug device
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_ctrl_debug(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;
	struct hdmi_rx_ctrl_video v;
	struct hdmi_rx_ctrl_audio a;

	if (ctx == 0) {
		return -EINVAL;
	}
	ctx->log_info("%s", file);
	ctx->log_info("  Config clock: %3u.%03uMHz",
			ctx->cfg_clk / 1000, ctx->cfg_clk % 1000);
	ctx->log_info("  TMDS clock:   %3u.%03uMHz",
			ctx->tmds_clk / 1000, ctx->tmds_clk % 1000);
	error |= hdmi_rx_ctrl_get_video(ctx, &v, 1);
	ctx->log_info("  Video: %ux%u%c@%u.%02uHz: %s, DCM %ub", v.hactive, v.vactive, v.interlaced? 'i' : 'p',	v.refresh_rate / 100, v.refresh_rate % 100,	v.dvi? "DVI" : "HDMI", v.deep_color_mode);
	switch (v.video_format)
	{
	case 0: /*RGB*/
		ctx->log_info("  Encoding: RGB");
		break;
	case 1: /*YCC444 */
		ctx->log_info("  Encoding: YCC444");
		break;
	case 2: /*YCC422*/
		ctx->log_info("  Encoding: YCC422");
		break;
	case 3: /*YCC420*/
		ctx->log_info("  Encoding: YCC420");
		break;
	}
	error |= hdmi_rx_ctrl_get_audio(ctx, &a);
	if (error == 0) {
		ctx->log_info("  Audio: CT=%u CC=%u SF=%u SS=%u CA=%u",
				a.coding_type, a.channel_count, a.sample_frequency,
				a.sample_size, a.channel_allocation);
	} else {
		ctx->log_info("  Audio");
	}
	ctx->log_info("  - N parameter:     %u", io_read(ctx, RA_PDEC_ACR_N, N_DECODED));
	ctx->log_info("  - CTS parameter:   %u", io_read(ctx, RA_PDEC_ACR_CTS, CTS_DECODED));
	ctx->log_info("  Audio  FIFO reset: %u", ctx->debug_audio_fifo_rst);
	ctx->log_info("  Packet FIFO reset: %u", ctx->debug_packet_fifo_rst);
	ctx->log_info("  IRQ handling:      %u", ctx->debug_irq_handling);
	ctx->log_info("  - packet decoder:  %u", ctx->debug_irq_packet_decoder);
	ctx->log_info("  - audio clock:     %u", ctx->debug_irq_audio_clock);
	ctx->log_info("  - audio FIFO:      %u", ctx->debug_irq_audio_fifo);
	ctx->log_info("  - video mode:      %u", ctx->debug_irq_video_mode);
	ctx->log_info("  - HDMI:            %u", ctx->debug_irq_hdmi);
	return error;
}

/**
 * Close device
 * @param[in,out] ctx context information
 * @return error code
 *
 * @note Call sequence: open => [configure] => close
 */
int hdmi_rx_ctrl_close(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (ctx->status == 0) {		/* is closed? */
		return -EPERM;
	}
	error |= interrupts_cfg(ctx, false);
	error |= ctx->spin_destroy(&ctx->spin);
	ctx->status = 0;
	ctx->tmds_clk = 0;
	ctx->bsp_reset(true);
	return error;
}

/**
 * PHY  GEN 3 PDDQ
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable pddq signal
 * @return error code
 */
int hdmi_rx_ctrl_pddq(struct hdmi_rx_ctrl *ctx, int enable)
{
	int error = 0;
	if (ctx == 0)
	{
		error = -EINVAL;
	}
	else
	{
		error = io_write(ctx, RA_SNPS_PHYG3_CTRL, MSK(1,1), enable);
	}
	return error;
}

/**
 * PHY GEN 3 SVS MODE
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable physvsretmodez
 * @return error code
 */
int hdmi_rx_ctrl_svsmode(struct hdmi_rx_ctrl *ctx, int enable)
{
	int error = 0;
	if (ctx == 0)
	{
		error = -EINVAL;
	}
	else
	{
		error = io_write(ctx, RA_SNPS_PHYG3_CTRL, MSK(1,6), enable);
	}
	return error;
}

/**
 * PHY GEN 3 reset signal
 * @param[in,out] ctx context information
 * @param[in] enable enable/disable phy reset
 * @return error code
 */
int hdmi_rx_ctrl_phy_reset(struct hdmi_rx_ctrl *ctx, int enable)
{
	int error = 0;
	if (ctx == 0)
	{
		error = -EINVAL;
	}
	else
	{
		error = io_write(ctx, RA_SNPS_PHYG3_CTRL, MSK(1,0), enable);
	}
	return error;
}

/**
 * PHY GEN 3 read function through internal I2C Master controller
 * @param[in,out] ctx context information
 * @param [in] slave_address of peripheral/phy
 * @param [in] reg_address to read from
 * @return 16-bit read data or error
 */
uint16_t hdmi_rx_ctrl_phy_read(struct hdmi_rx_ctrl *ctx, uint8_t slave_address, uint8_t reg_address)
{
	unsigned i = 0;
	int error = -EIO;
	if (ctx == 0)
	{
		return -EINVAL;
	}
	ctx->bsp_write(RA_I2CM_PHYG3_SLAVE, slave_address);
	ctx->bsp_write(RA_I2CM_PHYG3_ADDRESS, reg_address);
	ctx->bsp_write(RA_I2CM_PHYG3_OPERATION, 0x02); /* read op */
	for (i = 0; i < MAX_IIC_WAIT; i++)
	{	/* poll for i2c interrupts */
		if (i % 1000)
		{	/* read i2c interrupt status - whether done or error */
			if ((ctx->bsp_read(RA_HDMI_ISTS) & MSK(3, 28)) > 0)
			{	/* clear i2c interrupts */
				ctx->bsp_write(RA_HDMI_ICLR, set(ctx->bsp_read(RA_HDMI_ISTS), MSK(3, 28), 0x7));
				error = 0;
				break;
			}
		}
	}
	if (error != 0)
	{
		return error;
	}
	return (uint16_t)(ctx->bsp_read(RA_I2CM_PHYG3_DATAI));
}

/**
 * PHY GEN 3 write function through internal I2C Master controller
 * @param[in,out] ctx context information
 * @param [in] slave_address of peripheral/phy
 * @param [in] reg_address to write to
 * @param [in] data 16-bit data to be written to peripheral
 * @return error
 */
int hdmi_rx_ctrl_phy_write(struct hdmi_rx_ctrl *ctx, uint8_t slave_address, uint8_t reg_address, uint16_t data)
{
	int error = -EIO;
	unsigned i = 0;

	if(synopsys_phy_disable){
		return 0;
	}

	if (ctx == 0)
	{
		return -EINVAL;
	}
	ctx->bsp_write(RA_HDMI_ICLR, set(ctx->bsp_read(RA_HDMI_ISTS), MSK(3, 28), 0x7));
	ctx->bsp_write(RA_I2CM_PHYG3_SLAVE, slave_address);
	ctx->bsp_write(RA_I2CM_PHYG3_ADDRESS, reg_address);
	ctx->bsp_write(RA_I2CM_PHYG3_DATAO, data);
	ctx->bsp_write(RA_I2CM_PHYG3_OPERATION, 0x01); /* write op */
	for (i = 0; i < MAX_IIC_WAIT; i++)
	{	/* poll for i2c interrupts */
		if (i % 1000)
		{	/* read i2c interrupt status - whether done or error */
			//if ((ctx->bsp_read(RA_HDMI_ISTS) & MSK(3, 28)) > 0)
			if ((ctx->bsp_read(RA_HDMI_ISTS) & 0x10000000) > 0)
			{	/* clear i2c interrupts */
				ctx->bsp_write(RA_HDMI_ICLR, set(ctx->bsp_read(RA_HDMI_ISTS), MSK(3, 28), 0x7));
				error = 0;
				break;
			}
		}
	}
	return error;
}
/* Silicon Creation PLL */
unsigned hdmi_rx_refresh_audio_clk(struct hdmi_rx_ctrl *ctx)
{
	static unsigned long audpll_sc_sts1,audpll_sc_sts2;
	unsigned long sc_pll_ctrl1,sc_pll_ctrl2,sc_pll_ctrl3;
	unsigned char i;

	if (ctx == 0) {
		return -EINVAL;
	}
	if (ctx->status <= 0) {		/* is configured? */
		return -EPERM;
	}

	//read registers
	audpll_sc_sts2 = ctx->bsp_read(AUDPLL_SC_STS2);
	audpll_sc_sts1 = ctx->bsp_read(AUDPLL_SC_STS1);

	if ((audpll_sc_sts2 & ST_CONFRDY) == 0) //if values aren't ready return
	{
		//log_notice("ERROR: SC_PLL VALUE NOT READY");
		return -1;
	}

	//CTRL1
	sc_pll_ctrl1  = shift_and_mask(audpll_sc_sts1,0x3FC0,18,LEFT);
	sc_pll_ctrl1 |= shift_and_mask(audpll_sc_sts2,0x1FE0000,1,RIGHT);
	sc_pll_ctrl1 |= shift_and_mask(audpll_sc_sts2,0x1FE00,1,RIGHT);
	sc_pll_ctrl1 |= shift_and_mask(audpll_sc_sts2,0x1FE,1,RIGHT);

	//CTRL2
	sc_pll_ctrl2  = shift_and_mask(audpll_sc_sts1,0x3F,24,LEFT);
	sc_pll_ctrl2 |= shift_and_mask(audpll_sc_sts1,0x3C000,14,RIGHT);

	///////////////////////////////////////////////////////////////////////
	////Note: Adding or removing this section 							///
	///	doesn't change the system behavior								///
	sc_pll_ctrl2 |= (10<<DIVVAL);	//Cheng								///
	sc_pll_ctrl2 |= (1<<DISABLESCG);//Cheng								///
	sc_pll_ctrl2 |= (1<<DISABLEVCO);//Cheng								///
	///////////////////////////////////////////////////////////////////////

	//CTRL3
	sc_pll_ctrl3  = shift_and_mask(audpll_sc_sts1,0xE00000,17,RIGHT);
	sc_pll_ctrl3 |= shift_and_mask(audpll_sc_sts1,0x1C0000,18,RIGHT);

	sc_pll_ctrl3 |= LOCK<<MUX1;
	sc_pll_ctrl3 |= FOUT4<<MUX2;

	//Write Regs to north and south exits
	/* : By won.hur, don't know the meaning of this setting...
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL1,sc_pll_ctrl1);
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL5,sc_pll_ctrl1);

	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL2,sc_pll_ctrl2);
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL6,sc_pll_ctrl2);

	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL3,sc_pll_ctrl3);
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_CTRL7,sc_pll_ctrl3);
	*/
	////////////////////////////////////////////////////////

	ctx->bsp_write(AUDPLL_SC_CTRL,0x01);//Power down

	/*: By won.hur, don't know the meaning of this setting...
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_SPI_START,1);//Pulse SPI
	io_bus_write32(SC_PLL_BASE_ADDR,SC_PLL_SPI_START,0);
	*/


	////////////////////////////////////////////////////////
	//Note: System delay functions are too long
	//      solution was to implement register reads
	for ( i=0;i<4;i++)
	{
		ctx->bsp_read(AUDPLL_SC_CTRL);
	}

	ctx->bsp_write(AUDPLL_SC_CTRL,0x00);//Power up

	ctx->log_info("SC PLL Configured");

	return 0;
}

void hdmi_rx_scdc_initConfig(struct hdmi_rx_ctrl *ctx)
{
	/*enable on hdmi20_control register the SCDC communication*/
	ctx->bsp_write(0x0800, 0x00000010);
	/*SCDC i2c configuration register - spike suppression and timeout enable*/
	ctx->bsp_write(0x0804, 0x00180000);
	/*SCDC configuration - enable +5v provided by source and hpd voltage low more than 100ms*/
	ctx->bsp_write(0x0808,  0x00000002);
	/*SDCD manufacturer specific register - Manufacturer_OUI and version 1*/
	ctx->bsp_write(0x0860, ((0xABCD << 8) | 0x01));
	/*SCDC Channel lock config register - keep default values for now, period of 27Mhz*/
	ctx->bsp_write(0x080C, 0xC15C);
	/*SCDC enable read request*/
	ctx->bsp_write(0x0824, 0x00000001);
}

void hdmi_rx_scdc_setPowerProvided(struct hdmi_rx_ctrl *ctx, uint8_t enable)
{
	ctx->bsp_write(0x808, (enable & 0x01));
}

void hdmi_rx_scdc_setHpdLow(struct hdmi_rx_ctrl *ctx, uint8_t enable)
{
	ctx->bsp_write(0x808, ((enable << 1) & 0x10));
}

void hdmi_rx_scdc_setSinkVersion(struct hdmi_rx_ctrl *ctx, uint8_t version)
{
	ctx->bsp_write(0x860, (version & 0xFF));
}

void hdmi_rx_scdc_setManOUI(struct hdmi_rx_ctrl *ctx, uint32_t man_oui)
{
	ctx->bsp_write(0x860, ((man_oui<<8) & 0xFFFFFF00)); // NN
}

void hdmi_rx_scdc_getStatusRegisters(struct hdmi_rx_ctrl *ctx)
{
	uint32_t scdc_reg0 = 0;
	uint32_t scdc_reg1 = 0;
	uint32_t scdc_reg2 = 0;
	uint32_t scdc_reg3 = 0;

	scdc_reg0 = ctx->bsp_read(0x820);
	scdc_reg1 = ctx->bsp_read(0x824);
	scdc_reg2 = ctx->bsp_read(0x828);
	scdc_reg3 = ctx->bsp_read(0x82C);

	ctx->log_info("Rx controller Register 0x%x:", 0x820);
	ctx->log_info("SCDC Status register - Scrambling_Status: 0x%x", ((scdc_reg0 & 0x01000000) >> 24));
	ctx->log_info("SCDC TMDS Configuration register - TMDS_Bit_Clock_ratio: 0x%x", ((scdc_reg0 & 0x00020000) >> 17));
	ctx->log_info("SCDC TMDS Configuration register - Scrambling_Enable: 0x%x", ((scdc_reg0 & 0x00010000) >> 16));
	ctx->log_info("SCDC Update Flags register - RR_Test: 0x%x", ((scdc_reg0 & 0x00000400) >> 10));
	ctx->log_info("SCDC Update Flags register - CED_Update: 0x%x", ((scdc_reg0 & 0x00000200) >> 9));
	ctx->log_info("SCDC Update Flags register - Status_Update: 0x%x", ((scdc_reg0 & 0x00000100) >> 8));
	ctx->log_info("SCDC Source Version/Revision register - Source Version: 0x%x", (scdc_reg0 & 0x00000FF));
	ctx->log_info("");
	ctx->log_info("Rx Controller Register 0x%x:", 0x824);
	ctx->log_info("SCDC CED registers - Channel 0 Character Error Valid: 0x%x", ((scdc_reg1 & 0x80000000) >> 31));
	ctx->log_info("SCDC CED registers - Channel 0 Character Error Count: 0x%x", ((scdc_reg1 & 0x7FFF0000) >> 16));
	ctx->log_info("SCDC Status Flags register - Ch2_Locked: 0x%x", ((scdc_reg1 & 0x00000800) >> 11));
	ctx->log_info("SCDC Status Flags register - Ch1_Locked: 0x%x", ((scdc_reg1 & 0x00000400) >> 10));
	ctx->log_info("SCDC Status Flags register - Ch0_Locked: 0x%x", ((scdc_reg1 & 0x00000200) >> 9));
	ctx->log_info("SCDC Status Flags register - Clock_Detected: 0x%x", ((scdc_reg1 & 0x00000100) >> 8));
	ctx->log_info("SCDC Configuration register - RR_Enable: 0x%x", (scdc_reg1 & 0x00000001));
	ctx->log_info("");
	ctx->log_info("Rx Controller Register 0x%x:", 0x828);
	ctx->log_info("SCDC CED registers - Channel 2 Character Error Valid: 0x%x", ((scdc_reg2 & 0x80000000) >> 31));
	ctx->log_info("SCDC CED registers - Channel 2 Character Error Count: 0x%x", ((scdc_reg2 & 0x7FFF0000) >> 16));
	ctx->log_info("SCDC CED registers - Channel 1 Character Error Valid: 0x%x", ((scdc_reg2 & 0x00008000) >> 15));
	ctx->log_info("SCDC CED registers - Channel 1 Character Error Count: 0x%x", (scdc_reg2 & 0x00007FFF));
	ctx->log_info("");
	ctx->log_info("Rx Controller Register 0x%x:", 0x82C);
	ctx->log_info("SCDC Test Configuration register - TestReadRequest: 0x%x", ((scdc_reg3 & 0x0000080) >> 7));
	ctx->log_info("SCDC Test Configuration register - TestREadRequestDelay: 0x%x",(scdc_reg3 & 0x000007F));
}

void hdmi_rx_scdc_scrambling(struct hdmi_rx_ctrl *ctx, uint8_t enable) // Always 1
{
	/*automatic scrambling enabled*/
	if(enable == 1)
	{
		ctx->log_info("Automatic Scrambling enabled.");
		ctx->bsp_write(0x0800, (ctx->bsp_read(0x0800) | 0x0002));
	}
	else
	{
		if(enable == 2)
		{
			ctx->log_info("Forced scrambling enabled.");
			ctx->bsp_write(0x0800, ((ctx->bsp_read(0x0800) & 0xfffffffd) | 0x00003));
		}
		else
			if(enable == 0)
			{
				ctx->log_info("Scrambling disabled.");
				ctx->bsp_write(0x0800, (ctx->bsp_read(0x0800) & 0xfffffffd));
			}
	}
}
/** Get error count from previously enabled counters **/
error_cnt_t hdmi_rx_scdc_errorCnt(struct hdmi_rx_ctrl *ctx, uint8_t verbose, uint8_t check_valid)
{
	uint32_t value;
	error_cnt_t error_cnt;
	value = ctx->bsp_read(0x0824);
	error_cnt.errorCh0 = ((value >> 16) & 0x7fff);
	value = 0;
	value = ctx->bsp_read(0x0828);
	error_cnt.errorCh1 = ((value) & 0x7fff);
	error_cnt.errorCh2 = ((value >> 16) & 0x7fff);
	if(verbose)
	{
		ctx->log_info("Ch0: 0x%x",error_cnt.errorCh0);
		ctx->log_info("Ch1: 0x%x",error_cnt.errorCh1);
		ctx->log_info("Ch2: 0x%x",error_cnt.errorCh2);
	}
	if(check_valid == 1)
	{
		if((ctx->bsp_read(0x0030UL) & 0x0001UL) == 0 )
		{
			ctx->log_info("TMDS Valid = 0");
		}
		else
		{
			ctx->log_info("TMDS Valid = 1");
		}
	}
	return error_cnt;
}
void hdmi_rx_scdc_errorCntConfig(struct hdmi_rx_ctrl *ctx, uint8_t enable, uint8_t verbose)
{
	(enable)? ((verbose)?ctx->log_info(" SCDC Error counting: enabled all counters"):0) : ((verbose)? ctx->log_info(" SCDC Error counting: disabled all counters"): 0);
	(enable)? ctx->bsp_write(0x0800, 0x1010) : ctx->bsp_write(0x0800,  0x0000);
	(enable)? ctx->bsp_write(0x0808, 0x0001) : ctx->bsp_write(0x0808,  0x0000);/* Video Data Period check enable */
	(enable)? ctx->bsp_write(0x0ff0, 0x0004) : 0;
	ctx->bsp_write(0x080c, 0x61a8); /* Sampling clk 25 Mhz*/
}

void hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(struct hdmi_rx_ctrl *ctx ,uint8_t enable, uint8_t counters, uint8_t verbose)
{
	(enable)? ctx->bsp_write(0x0800,  (0x0010 | ((0x1f & counters) << 8))) : ctx->bsp_write(0x0800, 0x0000);
	(enable)? ctx->bsp_write(0x0808,  0x0001) : ctx->bsp_write(0x0808, 0x0000);/* Video Data Period check enable */
	(enable)? ctx->bsp_write(0x0ff0, 0x0004) : 0;
	ctx->bsp_write(0x080c, 0x61a8); /* Sampling clk 25 Mhz*/
}

unsigned long shift_and_mask(unsigned long value, unsigned long mask, unsigned long shift, unsigned char dir)
{
	value &= mask;

	if (dir == LEFT)
	{
		value <<= shift;
	}
	else if (dir == RIGHT)
	{
		value >>= shift;
	}

	return value;
}

