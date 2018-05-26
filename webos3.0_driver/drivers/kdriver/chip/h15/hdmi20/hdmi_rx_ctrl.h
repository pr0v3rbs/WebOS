/**
 * @file hdmi_rx_ctrl.h
 * @brief HDMI RX controller driver
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */
#ifndef HDMI_RX_CTRL_H_
#define HDMI_RX_CTRL_H_

#include <linux/errno.h>
#include <linux/types.h>
//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>

//Shift directions
enum shift_dirs{
 RIGHT	= 1,
 LEFT
};

/* Silicon Creations PLL */
//ST CONF RDY Bit
#define ST_CONFRDY 		0x0001

//RX Address
#define AUDPLL_SC_STS2 	0x2BCUL
#define AUDPLL_SC_STS1 	0x2B8UL
#define AUDPLL_SC_CTRL	0x2B4UL


//SC PLL Address
#define SC_PLL_L_BASE_ADDR 	0x3000UL
#define SC_PLL_H_BASE_ADDR 	0xC4000000
#define SC_PLL_BASE_ADDR	SC_PLL_L_BASE_ADDR+SC_PLL_H_BASE_ADDR

#define SC_PLL_CTRL1 	0x00UL
#define SC_PLL_CTRL2	0x01UL
#define SC_PLL_CTRL3	0x02UL
#define SC_PLL_CTRL4	0x03UL
#define SC_PLL_CTRL5	0x04UL
#define SC_PLL_CTRL6	0x05UL
#define SC_PLL_CTRL7	0x06UL
#define SC_PLL_CTRL8	0x07UL
#define SC_PLL_SPI_START	0x08UL


#define MUX1	12		//MUX1 Position
#define MUX2	8		//MUX2 Position
#define DIVVAL 12		//DIVVAL Position
#define DISABLEVCO 18	//DISABLEVCO Position
#define DISABLESCG 22	//DISABLESCG Position


enum MUX_PLL_VALUES{
	FOUT4,
	FOUT3,
	FOUT2,
	FOUT1PH270,
	FOUT1PH180,
	FOUT1PH90,
	FOUT1PH0,
	FOUT1POSTDIV,
	FOUTVCO,
	PD ,
	LOCK ,
	FREF ,
	NOISECLOCK,
	CLKSSG,
	GROUND_PLL,
	VDD_PLL
};


/**
 * @short HDMI RX controller events
 */
enum hdmi_rx_ctrl_event_t
{
	/** TMDS clock change event */
	hdmi_rx_ctrl_event_clock_change = 0,
	/** TMDS clock change event */
	hdmi_rx_ctrl_event_avmute_change,
	/** Packet reception event */
	hdmi_rx_ctrl_event_packet_reception,
	hdmi_rx_ctrl_event_scdc,
	/** Deep Color Change reception event */
	hdmi_rx_ctrl_event_dcm_change,
	/** Video status change event */
	hdmi_rx_ctrl_event_video_status,
	/** Audio status change event */
	hdmi_rx_ctrl_event_audio_status,
	/** AKSV reception event */
	hdmi_rx_ctrl_event_aksv_reception,
	/** PLL Lock Change reception event */
	hdmi_rx_ctrl_event_pll_lock_change,
	/** Phy I2C reception event */
	hdmi_rx_ctrl_event_phy_i2c_done,
	/** Phy I2C reception event */
	hdmi_rx_ctrl_event_phy_i2c_nack,
	/* Added for HDCP22 */
	hdmi_rx_ctrl_event_hdcp22_cap,
	hdmi_rx_ctrl_event_hdcp22_ncap,
	hdmi_rx_ctrl_event_hdcp22_auth_lost,
	hdmi_rx_ctrl_event_hdcp22_auth_ists,
	hdmi_rx_ctrl_event_hdcp22_auth_fail,
	hdmi_rx_ctrl_event_hdcp22_auth_change,
	/** Sentinel value */
	hdmi_rx_ctrl_event_cnt
};

/**
 * @short HDMI RX controller packet reception
 */
enum hdmi_rx_ctrl_packet_t
{
	/** Audio clock regeneration packet */
	hdmi_rx_ctrl_packet_ACR = 0,
	/** General control packet */
	hdmi_rx_ctrl_packet_GCP,
	/** Audio content protection packet */
	hdmi_rx_ctrl_packet_ACP,
	/** International standard recording code 1 packet */
	hdmi_rx_ctrl_packet_ISRC1,
	/** International standard recording code 2 packet */
	hdmi_rx_ctrl_packet_ISRC2,
	/** Gamut metadata packet */
	hdmi_rx_ctrl_packet_GAMUT,
	/** Vendor specific info frame packet */
	hdmi_rx_ctrl_packet_VSI,
	/** Auxiliary video info frame packet */
	hdmi_rx_ctrl_packet_AVI,
	/** Source product descriptor info frame packet */
	hdmi_rx_ctrl_packet_SPD,
	/** Audio info frame packet */
	hdmi_rx_ctrl_packet_AIF,
	/** MPEG source info frame packet */
	hdmi_rx_ctrl_packet_MPEGS,
	/** Sentinel value */
	hdmi_rx_ctrl_packet_cnt
};

/**
 * @short HDMI RX controller audio parameters
 *
 * For Audio InfoFrame (AIF) details see HDMI 1.4a section 8.2.2
 */
struct hdmi_rx_ctrl_audio
{

	/** AIF CT3-0, coding type */
	unsigned coding_type;
	/** AIF CC2-0, channel count */
	unsigned channel_count;
	/** AIF SF2-0, sample frequency */
	unsigned sample_frequency;
	/** AIF SS1-0, sample size */
	unsigned sample_size;
	/** AIF CTX, coding extension */
	unsigned coding_extension;
	/** AIF CA7-0, channel/speaker allocation */
	unsigned channel_allocation;
	/** AIF DM_INH, down-mix inhibit */
	unsigned down_mix_inhibit;
	/** AIF LSV3-0, level shift value */
	unsigned level_shift_value;
};

/**
 * @short HDMI RX controller video parameters
 *
 * For Auxiliary Video InfoFrame (AVI) details see HDMI 1.4a section 8.2.2
 */
struct hdmi_rx_ctrl_video
{
	/** DVI detection status: DVI (true) or HDMI (false) */
	bool dvi;
	/** Deep color mode: 24, 30, 36 or 48 [bits per pixel] */
	unsigned deep_color_mode;

	/** Pixel clock frequency [kHz] */
	unsigned long pixel_clk;
	/** Refresh rate [0.01Hz] */
	unsigned long refresh_rate;
	/** Interlaced */
    bool interlaced;
    /** Vertical offset */
    unsigned voffset;
    /** Vertical active */
    unsigned vactive;
    /** Vertical total */
    unsigned vtotal;
    /** Horizontal offset */
    unsigned hoffset;
    /** Horizontal active */
    unsigned hactive;
    /** Horizontal total */
    unsigned htotal;

	/** AVI Y1-0, video format */
	unsigned video_format;
	/** AVI A0, active format information present */
	unsigned active_valid;
	/** AVI B1-0, bar valid information */
	unsigned bar_valid;
	/** AVI S1-0, scan information */
	unsigned scan_info;
	/** AVI C1-0, colorimetry information */
	unsigned colorimetry;
	/** AVI M1-0, picture aspect ratio */
	unsigned picture_ratio;
	/** AVI R3-0, active format aspect ratio */
	unsigned active_ratio;
	/** AVI ITC, IT content */
	unsigned it_content;
	/** AVI EC2-0, extended colorimetry */
	unsigned ext_colorimetry;
	/** AVI Q1-0, RGB quantization range */
	unsigned rgb_quant_range;
	/** AVI SC1-0, non-uniform scaling information */
	unsigned n_uniform_scale;
	/** AVI VIC6-0, video mode identification code */
	unsigned video_mode;
	/** AVI PR3-0, pixel repetition factor */
	unsigned pixel_repetition;
	/** AVI, line number of end of top bar */
	unsigned bar_end_top;
	/** AVI, line number of start of bottom bar */
	unsigned bar_start_bottom;
	/** AVI, pixel number of end of left bar */
	unsigned bar_end_left;
	/** AVI, pixel number of start of right bar */
	unsigned bar_start_right;

};

/**
 * @short HDMI RX controller context information
 *
 * Initialize @b user fields (set status to zero).
 * After opening this data is for internal use only.
 */
struct hdmi_rx_ctrl
{
	/* Added by LGE SIC won.hur */
	int device_id;		/* UNIQUE ID for each context information */
	bool isMute;
	/* Added by LGE SIC won.hur */
	struct hdmi_rx_ctrl_video	previous_video;
	/** (@b user) Context status: closed (0), opened (<0) and configured (>0) */
	int status;
	/** (@b user) Log errors */
	void (*log_error)(const char *file, int line, const char *fmt, ...);
	/** (@b user) Log information */
	void (*log_info)(const char *fmt, ...);
	/** (@b user) Board support package, reset device */
	void (*bsp_reset)(bool enable);
	/** (@b user) Board support package, reset HDMI clock domain */
	void (*bsp_reset_hdmi)(bool enable);
	/** (@b user) Board support package, EDID update - optional if not provided no action is performed */
	int (*bsp_edid_update)(void);
	/** (@b user) Board support package, register read access */
	uint32_t (*bsp_read)(uint16_t addr);
	/** (@b user) Board support package, register write access */
	void (*bsp_write)(uint16_t addr, uint32_t data);
	/** Mutual exclusion */
	void *spin;
	/** (@b user) Mutual exclusion initialization */
	int (*spin_init)(void *handler);
	/** (@b user) Mutual exclusion destruction */
	int (*spin_destroy)(void *handler);
	/** (@b user) Mutual exclusion lock */
	int (*spin_lock_irq_save)(void *handler, unsigned long *flags);
	/** (@b user) Mutual exclusion unlock */
	int (*spin_unlock_irq_restore)(void *handler, unsigned long flags);
	/** (@b user) Configuration clock frequency [kHz], valid range 10MHz to 160MHz */
	unsigned long cfg_clk;
	/** (@b user) Mode detection clock frequency [kHz], valid range 10MHz to 50MHz */
	unsigned long md_clk;
	/** TDMS clock frequency [kHz] */
	unsigned long tmds_clock_old;
	unsigned long tmds_clk;
	unsigned int stable_cnt;
	unsigned int n_param;
	unsigned int cts_param;
	/** Event handler */
	int (*event_handler[hdmi_rx_ctrl_event_cnt])(struct hdmi_rx_ctrl *);
	/** Debug status, audio FIFO reset count */
	unsigned debug_audio_fifo_rst;
	/** Debug status, packet FIFO reset count */
	unsigned debug_packet_fifo_rst;
	/** Debug status, IRQ handling count */
	unsigned debug_irq_handling;
	/** Debug status, IRQ packet decoder count */
	unsigned debug_irq_packet_decoder;
	/** Debug status, IRQ audio clock count */
	unsigned debug_irq_audio_clock;
	/** Debug status, IRQ audio FIFO count */
	unsigned debug_irq_audio_fifo;
	/** Debug status, IRQ video mode count */
	unsigned debug_irq_video_mode;
	/** Debug status, IRQ HDMI count */
	unsigned debug_irq_hdmi;
	unsigned int hBlankHactive;
	unsigned int hFrontHsync;
	unsigned int vFrontVsync;
	unsigned int vBlankVactive;
};

//int (*event_handler[hdmi_rx_ctrl_event_cnt])(struct hdmi_rx_ctrl *);

int hdmi_rx_ctrl_open(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_config(struct hdmi_rx_ctrl *ctx, unsigned input);
int hdmi_rx_ctrl_irq_handler(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_hpd(struct hdmi_rx_ctrl *ctx, bool enable);
int hdmi_rx_ctrl_subscribe(struct hdmi_rx_ctrl *ctx, enum hdmi_rx_ctrl_event_t type, int (*handler)(struct hdmi_rx_ctrl *));
int hdmi_rx_ctrl_store(struct hdmi_rx_ctrl *ctx, enum hdmi_rx_ctrl_packet_t type, bool enable);
long hdmi_rx_ctrl_get_tmds_clk(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_get_audio(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_audio *params);
int hdmi_rx_ctrl_get_packet(struct hdmi_rx_ctrl *ctx, uint8_t *data, size_t size);
int hdmi_rx_ctrl_get_video(struct hdmi_rx_ctrl *ctx, struct hdmi_rx_ctrl_video *params, uint8_t verbose);
int hdmi_rx_ctrl_debug(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_close(struct hdmi_rx_ctrl *ctx);

/* Modified Won hur */
int hdmi_rx_ctrl_audio_cfg(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_packet_fifo_rst(struct hdmi_rx_ctrl *ctx);
int hdmi_rx_ctrl_audio_fifo_rst(struct hdmi_rx_ctrl *ctx);


/* PHY GEN3 functions */
int hdmi_rx_ctrl_pddq(struct hdmi_rx_ctrl *ctx, int enable);
int hdmi_rx_ctrl_svsmode(struct hdmi_rx_ctrl *ctx, int enable);
int hdmi_rx_ctrl_phy_reset(struct hdmi_rx_ctrl *ctx, int enable);
uint16_t hdmi_rx_ctrl_phy_read(struct hdmi_rx_ctrl *ctx, uint8_t slave_address, uint8_t reg_address);
int hdmi_rx_ctrl_phy_write(struct hdmi_rx_ctrl *ctx, uint8_t slave_address, uint8_t reg_address, uint16_t data);

unsigned hdmi_rx_refresh_audio_clk(struct hdmi_rx_ctrl *ctx);
unsigned long shift_and_mask(unsigned long value, unsigned long mask, unsigned long shift, unsigned char dir);
unsigned hdmi_rx_refresh_audio_clk(struct hdmi_rx_ctrl *ctx);

/* HDMI 2.0 features - error counting */
typedef struct
{
	uint32_t errorCh0;
	uint32_t errorCh1;
	uint32_t errorCh2;
	uint8_t validCnt;
} error_cnt_t;

/* error counting and equalizer settings */
typedef struct {
	uint32_t error_ch_count;
	uint8_t ch_bestEqSetting;
} eqResult_t;

/* HDMI 2.0 features - SCDC config*/
void hdmi_rx_scdc_initConfig(struct hdmi_rx_ctrl *ctx);
/* HDMI 2.0 features - Scrambling enable*/
void hdmi_rx_scdc_setPowerProvided(struct hdmi_rx_ctrl *ctx, uint8_t enable);
void hdmi_rx_scdc_setHpdLow(struct hdmi_rx_ctrl *ctx, uint8_t enable);
void hdmi_rx_scdc_setSinkVersion(struct hdmi_rx_ctrl *ctx, uint8_t version);
void hdmi_rx_scdc_setManOUI(struct hdmi_rx_ctrl *ctx, uint32_t man_oui);
void hdmi_rx_scdc_getStatusRegisters(struct hdmi_rx_ctrl *ctx);
void hdmi_rx_scdc_scrambling(struct hdmi_rx_ctrl *ctx, uint8_t enable);

error_cnt_t hdmi_rx_scdc_errorCnt(struct hdmi_rx_ctrl *ctx, uint8_t verbose, uint8_t check_valid);

void hdmi_rx_scdc_errorCntConfig(struct hdmi_rx_ctrl *ctx, uint8_t enable, uint8_t verbose);

void hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(struct hdmi_rx_ctrl *ctx, uint8_t enable, uint8_t counters, uint8_t verbose);

#endif /* HDMI_RX_CTRL_H_ */
