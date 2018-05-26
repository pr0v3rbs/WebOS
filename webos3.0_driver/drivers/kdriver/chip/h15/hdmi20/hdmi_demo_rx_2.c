/*
 * hdmi_demo_rx_2.c
 *
 *  Created on: Oct 17, 2012
 */
#define INTR
//#define SC_PLL
#define INCLUDE_HDMI_TX
#define RX_COMPLIANCE

#include "hdcp22_polling.h"
#include "hdmi_demo_rx_2.h"
#include "hdmi_rx_phy.h"
#include "hdmi_rx_ctrl.h"
#include "hdmi_rx_ctrl_hdcp.h"
#include "platform.h"
#ifdef INCLUDE_HDMI_TX
#include "api.h"
#include "demo.h"
#endif
#include "video_if.h"
#include "../mipi_demo_csi2host/mipi_cameras.h"
#include "platform.h"

/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#define MSK(m, n)				(((1 << (m)) - 1) << (n))


#define TMDS_TOLERANCE  (20)
#define V_MODE_STABLE	(500000)
/* Video DRP PLL controller 32-bit address */
//static const uint32_t VIDEO_PLL_BASE_ADDRESS = 0xc9807400;
int error_counter = 0;
static const char *file = "hdmi_demo_rx_2";
static unsigned long tmds_clock_old = 0;
static struct hdmi_rx_ctrl_video video_params = {0};
static struct hdmi_rx_ctrl_audio audio_params = {0};
static int (*hdmi_rx_callback)(int((*handler)(void *, void *, void *)), void * param1, void * param2, void * param3);
//static int (*hdmi_tx_callback)(int((*handler)(void *, void *, void *)), void * param1, void * param2, void * param3);

/*****E205 equalizer wa **********/
static error_cnt_t errorCnt[8];
static eqResult_t eqResult[3];
static int cnt_start = 0;
static	int cnt_stop = 0;
static	int cnt_tmp = 0;
/*********************************/
static int globalphase = 0;
char hdmi_rx_buffer[3][TOKEN_SIZE] = {{'\0'}};
int hdmi_rx_buffer_no = 0;
unsigned dcm = 8;
videoParams_t *Rx_pVideo = NULL;
audioParams_t * Rx_pAudio = NULL;
hdcpParams_t *Rx_pHdcp = NULL;
productParams_t *Rx_pProduct;
extern unsigned repeater_enabled;
extern unsigned demo_rx_enabled;
extern unsigned update_demo;
unsigned hdmitx_phy = 108;
static struct rx {
	/** HDMI RX received signal changed */
	bool change;
	/** HDMI RX input port 0 (A) or 1 (B) (or 2(C) or 3 (D)) */
	unsigned port;
	/** HDMI RX IRQ number */
	const unsigned irq;
	/** HDMI RX PHY context */
	struct hdmi_rx_phy phy;
	/** HDMI RX controller context */
	struct hdmi_rx_ctrl ctrl;
	/** HDMI RX controller HDCP configuration */
	const struct hdmi_rx_ctrl_hdcp hdcp;
} rx = {false, 3, HDMI_RX_INT_1, {0}, {0},

		{
#if 1
				/**
				 * @warning These HDCP encrypted keys are production keys.
				 * @warning Do NOT send them to customers!
				 */
				false, /*populate w/ your keys here*/,
				{
						/*populate w/ your keys here*/, /*populate w/ your keys here*/,
				},
				{
				/*populate w/ your keys here*/
				}
		}
};

/* ***************************************************************************
 * Data handling
 * ***************************************************************************/
/**
 * Find first (least significant) bit set
 * @param[in] data word to search
 * @return bit position or 32 if none is set
 */
static unsigned hdmi_rx_demo_first_bit_set(uint32_t data)
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
static uint32_t hdmi_rx_demo_get_bit(uint32_t data, uint32_t mask)
{
	return (data & mask) >> hdmi_rx_demo_first_bit_set(mask);
}
#if 0
/**
 * Set bit field
 * @param[in] data raw data
 * @param[in] mask bit field mask
 * @param[in] value new value
 * @return new raw data
 */
static uint32_t hdmi_rx_demo_set_bit(uint32_t data, uint32_t mask, uint32_t value)
{
	return ((value << hdmi_rx_demo_first_bit_set(mask)) & mask) | (data & ~mask);
}
#endif
int hdmi_demo_rx_register_callback (int((*top_app_callback)(int((*handler)(void *, void *, void *)), void * param1, void * param2, void * param3)))
{
	if (top_app_callback != NULL)
	{
		hdmi_rx_callback = top_app_callback;
		return 0;
	}
	return 1;
}

int hdmi_demo_rx_unregister_callback()
{
	hdmi_rx_callback = NULL;
	return 0;
}

/**
 * Read data from HDMI RX CTRL
 * @param[in] addr register address
 * @return data read value
 */
static uint32_t hdmi_rx_ctrl_read(uint16_t addr)
{
	io_bus_read32(HDMI_RX_CTRL_BASE, addr); // rx bus setup controller related workaround
	return io_bus_read32(HDMI_RX_CTRL_BASE, addr);
}

/**
 * Write data to HDMI RX CTRL
 * @param[in] addr register address
 * @param[in] data new register value
 */
void hdmi_rx_ctrl_write(uint16_t addr, uint32_t data)
{
	/* log_info("%04X:%08X", addr, data); */
	io_bus_write32(HDMI_RX_CTRL_BASE, addr, data);
}
/**
 * Reset HDMI RX controller
 * @param[in] enable reset enable
 */
static void hdmi_rx_ctrl_reset(bool enable)
{
	board_reset(HDMI_RX);
}

static void hdmi_rx_ctrl_reset_hdmi(bool enable)
{
	uint32_t temp = hdmi_rx_ctrl_read(0xff0);
	temp &= ~(1 << 2);
	temp |= (enable & 1) << 2;
	hdmi_rx_ctrl_write(0xff0, temp);
}

/** PHY GEN 3 I2C slave address (Testchip E205) */
#define PHY_GEN3_I2C_SLAVE_ADDR			(0x39UL)
#define PHY_GEN3_GLUE_I2C_SLAVE_ADDR	(0x48UL)
/**
 * PHY Wrapper
 * wrap functions needed by phy
 * used for phy gen 3 only
 */
void phy_wrapper_zcal_reset(int enable)
{
	board_reset(ZCAL);
}

static uint16_t phy_wrapper_read(uint8_t reg_address)
{
	return hdmi_rx_ctrl_phy_read(&rx.ctrl, PHY_GEN3_I2C_SLAVE_ADDR, reg_address);
}

static void phy_wrapper_write(uint8_t reg_address, uint16_t data)
{
	hdmi_rx_ctrl_phy_write(&rx.ctrl, PHY_GEN3_I2C_SLAVE_ADDR, reg_address, data);
}

static void phy_wrapper_svsretmode(int enable)
{
	hdmi_rx_ctrl_svsmode(&rx.ctrl, enable);
}

static void phy_wrapper_phy_reset(bool enable)
{
	hdmi_rx_ctrl_phy_reset(&rx.ctrl, enable? 1: 0);
}

static void phy_wrapper_pddq(int enable)
{
	hdmi_rx_ctrl_pddq(&rx.ctrl, enable);
}

static uint16_t phy_wrapper_zcal_done()
{
	/* Check if bit 14 of I2C Glue Logic register´s 3 is at 0.  (If 0 ok, else failed) */
	return (hdmi_rx_ctrl_phy_read(&rx.ctrl, PHY_GEN3_GLUE_I2C_SLAVE_ADDR, 0x3) >> 14 & 1);
}

static int phy_wrapper_setup_bsp(unsigned long tmds)
{
	int error = 0;
	/* configuration particular to testchip E205 */
	if (tmds > 200000)
	{   /* DRP HDMI RX PLL 90deg */
		/* log_info("DRP 90deg"); */
		log_notice( " > 200000");
		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd0c4);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0081);
		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
	}
	else
	{
		if ((tmds > 100000) && (tmds < 120000))
		{ 	/* DRP HDMI RX PLL 90deg */
			/* log_info("DRP 90deg"); */
			log_notice( " > 100000 and < 120000");
			drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd0c4);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0081);
			drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
		}
		else
		{   /* DRP HDMI RX PLL 0deg */
			/* log_info("DRP 0deg"); */
			log_notice( " else");
			drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x10c4);
			//drp_reset(RX_PLL_BASE_ADDRESS);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0080);
			drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
		}
	}
	return error;
}
#if 0
/**
 * Read an EDID byte
 * @param[in] addr register address
 * @param[in] port A(1) or B(2)
 * @return read value
 */
static uint8_t edid_read(uint8_t port, uint8_t addr)
{
	if (port != 1)
	{
		io_read8(EDID_B_RAM_BASE + addr);	/* HW WA: read 2x */
		return io_read8(EDID_B_RAM_BASE + addr);
	}
	else
	{
		io_read8(EDID_A_RAM_BASE + addr);	/* HW WA: read 2x */
		return io_read8(EDID_A_RAM_BASE + addr);
	}
}
#endif
/**
 * Write an EDID byte
 * @param[in] port A(1) or B(2) or C(3) or D(4) or all (0xff)
 * @param[in] addr register address
 * @param[in] data new register value
 */
static void edid_write(uint8_t port, uint8_t addr, uint8_t data)
{
	int i = 0;
	i = ((port & (1 << 0)) > 0)? io_write8(EDID_A_RAM_BASE + addr, data): 0;
#ifdef EDID_B_RAM_BASE
	i = ((port & (1 << 1)) > 0)? io_write8(EDID_B_RAM_BASE + addr, data): 0;
#endif
#ifdef EDID_C_RAM_BASE
	i = ((port & (1 << 1)) > 0)? io_write8(EDID_C_RAM_BASE + addr, data): 0;
#endif
#ifdef EDID_D_RAM_BASE
	i = ((port & (1 << 1)) > 0)? io_write8(EDID_D_RAM_BASE + addr, data): 0;
#endif
}

/**
 * Update EDID
 * @return error code
 */
int hdmi_demo_rx_ctrl_edid_update()
{
	const uint8_t edid[] =
	{
			// 4k
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
			0x44, 0x89, 0xD4, 0x03, 0x15, 0xCD, 0x5B, 0x07,
			0x15, 0x15, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78,
			0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
			0x12, 0x48, 0x4C, 0x20, 0x00, 0x00, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,
			0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
			0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E,
			0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
			0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00,
			0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
			0x44, 0x4D, 0x49, 0x20, 0x41, 0x6E, 0x61, 0x6C,
			0x79, 0x7A, 0x65, 0x72, 0x00, 0x00, 0x00, 0xFD,
			0x00, 0x17, 0xF1, 0x08, 0x8C, 0x1E, 0x00, 0x0A,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xAB,

			0x02, 0x03, 0x77, 0x71, 0x5F, 0x90, 0x1F, 0x22,
			0x20, 0x05, 0x14, 0x04, 0x13, 0x3E, 0x3C, 0x11,
			0x02, 0x03, 0x15, 0x06, 0x01, 0x07, 0x08, 0x09,
			0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x12, 0x16,
			0x17, 0x18, 0x19, 0x1A, 0x5F, 0x1B, 0x1C, 0x1D,
			0x1E, 0x21, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
			0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
			0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
			0x39, 0x3A, 0x3B, 0x3D, 0x42, 0x3F, 0x40, 0x32,
			0x0F, 0x7F, 0x07, 0x17, 0x7F, 0x50, 0x3F, 0x7F,
			0xC0, 0x57, 0x7F, 0x00, 0x5F, 0x7F, 0x01, 0x67,
			0x7F, 0x00, 0x83, 0x4F, 0x00, 0x00, 0x78, 0x03,
			0x0C, 0x00, 0x10, 0x00, 0x38, 0x3C, 0x20, 0xA0,
			0x8A, 0x01, 0x02, 0x03, 0x04, 0x81, 0x40, 0x20,
			0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB

			/*//EDID, limited to 200Mhz
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
			0x4F, 0x2E, 0xE2, 0x01, 0x00, 0x00, 0x00, 0x00,
			0x06, 0x17, 0x01, 0x03, 0x80, 0x46, 0x28, 0x78,
			0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
			0x12, 0x48, 0x4C, 0x20, 0x00, 0x00, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,
			0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
			0x45, 0x00, 0xDF, 0xA4, 0x21, 0x00, 0x00, 0x1E,
			0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
			0x58, 0x2C, 0x45, 0x00, 0xDF, 0xA4, 0x21, 0x00,
			0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
			0x44, 0x4D, 0x49, 0x20, 0x4C, 0x4C, 0x43, 0x0A,
			0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
			0x00, 0x3A, 0x3E, 0x0F, 0x46, 0x0F, 0x00, 0x0A,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x3B,
			0x02, 0x03, 0x22, 0x71, 0x83, 0x01, 0x00, 0x00,

			0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xF0, 0x28,
			0x4D, 0x01, 0x02, 0x03, 0x04, 0x05, 0x90, 0x11,
			0x12, 0x13, 0x14, 0x1F, 0x20, 0x21, 0x23, 0x09,
			0x07, 0x07, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0,
			0x1E, 0x20, 0x6E, 0x28, 0x55, 0x00, 0xDF, 0xA4,
			0x21, 0x00, 0x00, 0x1E, 0x8C, 0x0A, 0xD0, 0x8A,
			0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
			0xDF, 0xA4, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A,
			0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E,
			0x96, 0x00, 0x30, 0xA4, 0x21, 0x00, 0x00, 0x18,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF
*/
	};

	unsigned int i = 0;
	for (i = 0; i < sizeof(edid); i++)
	{	/* write to all EDID RAMs */
		edid_write(0xff, i, edid[i]);
	}
	log_notice("RX Demo: Default EDID write to EDID RAM. Total %d bytes, %d blocks", sizeof(edid), sizeof(edid)/128);

	return 0;
}

void hdmi_demo_rx_new_edid_update()
{
	unsigned int i = 0;
	for (i = 0; i < ((int)(rx_edidNrBlocks_top))*128; i++)
	{	/* write to all EDID RAMs */
		edid_write(0xff, i, rx_rawEdid_top[i]);
	}
	log_notice("RX Demo: New EDID write to EDID RAM. Total %d bytes, %d blocks", ((int)(rx_edidNrBlocks_top))*128, (int)(rx_edidNrBlocks_top));
}



/**
 * Update port selection
 * @param[out] rx HDMI RX parameters
 * @param[in] delta delta {-1, 0, 1}
 * @return port selected
 */
int hdmi_demo_rx_menu_main_cb(void * param1, void * param2, void * param3)
{
	log_notice("--->>>hdmi_demo_rx_menu_main_cb");
	return hdmi_demo_rx_menu_main(((char(*)[TOKEN_SIZE])(param1)), *(int*)(param2), param3);
}

/**
 * Interrupt handler
 * @param[in] params handler parameters
 */
void irq_handler(void *params)
{
	log_notice("IRQ HANDLER CALLED 0.");
	int error = 0;
	interrupts_acknowledgeSpecificDemo(INT_MASK_HDMI_RX, 2);
	//	disable_interrupt(9);
	//	interrupts_disableSpecificDemo(INT_MASK_HDMI_RX, 2);
	error = hdmi_rx_ctrl_irq_handler(&rx.ctrl);
	//	interrupts_enableSpecificDemo(INT_MASK_HDMI_RX, 2);
	//	enable_interrupt(9);
	//log_notice("IRQ HANDLER CALLED hdmi_rx_ctrl_irq_handler.");
	if (error < 0)
	{
		if (error != -EPERM)
		{
			log_trace(file, __LINE__, "RX IRQ handler %d", error);
		}
	}
	//	error = os_irq_acknowledge(((struct rx *)params)->irq);
	//	log_notice("IRQ HANDLER CALLED os_irq_acknowledge.");
	//	if (error < 0)
	//	{
	//		log_trace(file, __LINE__, "RX IRQ acknowledge %d", error);
	//	}
}
#if 0 /* old irq handler */
void irq_handler(void *params)
{
	disable_interrupt(9);
	int error = 0;
	log_notice("IRQ HANDLER CALLED 0.");
	interrupts_acknowledgeSpecificDemo(INT_MASK_HDMI_RX, 2);
	interrupts_disableSpecificDemo(INT_MASK_HDMI_RX, 2);
	error = hdmi_rx_ctrl_irq_handler(&rx.ctrl);
	interrupts_enableSpecificDemo(INT_MASK_HDMI_RX, 2);
	enable_interrupt(9);
	//log_notice("IRQ HANDLER CALLED hdmi_rx_ctrl_irq_handler.");
	if (error < 0)
	{
		if (error != -EPERM)
		{
			log_trace(file, __LINE__, "RX IRQ handler %d", error);
		}
	}
	//	error = os_irq_acknowledge(((struct rx *)params)->irq);
	//	log_notice("IRQ HANDLER CALLED os_irq_acknowledge.");
	//	if (error < 0)
	//	{
	//		log_trace(file, __LINE__, "RX IRQ acknowledge %d", error);
	//	}
}
#endif
/**
 * Clock event handler
 * @param[in,out] ctx context information
 * @return error code
 */
//static int clock_handler(void * param1, void * param2, void* param3)
static int clock_handler(struct hdmi_rx_ctrl *ctx)
{
	log_notice("HDMI Rx Demo: clock handler called");
	int factor_m = 8;
	int error = 0;
	long tclk_temp = 0;
	unsigned long tclk = 0;
	struct hdmi_rx_ctrl_video v;
	if (ctx == 0)
	{
		return -EINVAL;
	}
	tclk = hdmi_rx_ctrl_get_tmds_clk(ctx);
	if(tclk == 0)
	{
		log_notice(" ----------------------------Tclk == 0");
		io_bus_write32(VIDEO_IF_BASE_ADDRESS + 0x2003, 0, 0x02);
	}
	else
	{
#if 1 // ancosta workaround
		/* If no TMDSvalid, restart RX PHY macro */
		if ((hdmi_rx_ctrl_read(0x30) & 0x01) == 0)
		{
			log_notice("Workaround applied");
			rx.change = true;
			if (hdmi_rx_phy_close(&rx.phy) < 0)
			{
				log_error("RX PHY close");
			}
			hdmi_rx_ctrl_write(0xff0, ((1 << 3) - 1) << 1);
			if (hdmi_rx_phy_open(&rx.phy) < 0)
			{
				log_error("RX PHY open");
			}
			/* workaround for E213 daugther cards */
			//hdmi_rx_ctrl_write(0x2c0, 0x40 + (unsigned) (0 << 2));
			//ctx.phy.bsp_write(0x03, rx.phy.bsp_read(0x03) | 0x0004);
			phy_wrapper_write(0x03, phy_wrapper_read(0x03) | 0x0004);
			return 0;
		}
#endif
		io_bus_write32(VIDEO_IF_BASE_ADDRESS + 0x2003, 0, 0x03);
	}
	error |= hdmi_rx_ctrl_get_video(ctx, &v);
	if (((tmds_clock_old + TMDS_TOLERANCE) > tclk) &&
			((tmds_clock_old - TMDS_TOLERANCE) < tclk))
	{
		return 0;
	}
	else
	{
		switch (v.deep_color_mode) {
		case 48:
			//dcm = 16 bits;
			factor_m = 2;
			break;
		case 36:
			//dcm = 12 bits;
			factor_m = 1.5;
			break;
		case 30:
			//dcm = 10 bits;
			factor_m = 1.25;
			break;
		default:
			//dcm = 8 bits;
			factor_m = 1.00;
			break;
		}
		tclk_temp = hdmi_rx_ctrl_get_tmds_clk(ctx);
		hdmi_rx_demo_pll((unsigned long)(tclk_temp*factor_m));
		error |= hdmi_rx_phy_config(&rx.phy, tclk, v.deep_color_mode);
	}
	error |= hdmi_rx_ctrl_get_video(ctx, &v);
	tmds_clock_old = ctx->tmds_clk;
#if 1
	ctx->log_info("TMDS clock: %3u.%03uMHz",
			ctx->tmds_clk / 1000, ctx->tmds_clk % 1000);
#endif
	return error;
}

/**
 * Video event handler
 * @param[in,out] ctx context information
 * @return error code
 */
static int video_handler(struct hdmi_rx_ctrl *ctx)
{
	//struct hdmi_rx_ctrl *ctx = (struct hdmi_rx_ctrl (*))(param3);
	int factor_m = 0;
	int tclk_temp = 0;
	int error = 0;
	unsigned i = 0;
	struct hdmi_rx_ctrl_video v = {0};

	if (ctx == 0)
	{
		return -EINVAL;
	}
	log_notice("HDMI Rx Demo: video handler called");
	/* wait for the video mode is stable */
	for (i = 0; i < V_MODE_STABLE; i++)
	{
		;
	}
	error |= hdmi_rx_ctrl_get_video(ctx, &v);
	log_notice("status = %d", error);
	if ((error == 0) &&
			(((video_params.hactive + 5) < (v.hactive)) ||
					((video_params.hactive != 0) && ((video_params.hactive - 5) > (v.hactive))) ||
					((video_params.vactive + 5) < (v.vactive)) ||
					((video_params.vactive != 0) && ((video_params.vactive - 5) > (v.vactive))) ||
					(video_params.pixel_repetition != v.pixel_repetition) ||
					(video_params.deep_color_mode != v.deep_color_mode) ||
					((video_params.refresh_rate + 3) < (v.refresh_rate)) ||
					((video_params.refresh_rate != 0) && ((video_params.refresh_rate - 3) > (v.refresh_rate))) ||
					(video_params.dvi != v.dvi) ||
					(video_params.video_mode != v.video_mode) ||
					(video_params.interlaced != v.interlaced)
			)
	)
	{
#if 1
		ctx->log_info("Video: %ux%u%c@%u.%02uHz: %s, DCM %ub, VIC:%u",
				v.hactive, v.vactive, v.interlaced? 'i' : 'p',
						v.refresh_rate / 100, v.refresh_rate % 100,
						v.dvi? "DVI" : "HDMI", v.deep_color_mode, v.video_mode);
#endif

		//video_if_adjust_zoom_autocenter();
		rx.change = true;
		video_params.dvi = v.dvi;
		video_params.pixel_clk = v.pixel_clk;
		video_params.refresh_rate = v.refresh_rate;
		video_params.interlaced = v.interlaced;
		video_params.video_mode = v.video_mode;
		video_params.deep_color_mode = v.deep_color_mode;
		video_params.htotal = v.htotal;
		video_params.vtotal = v.vtotal;
		video_params.hactive = v.hactive;
		video_params.vactive  = v.vactive;
		video_params.pixel_repetition = v.pixel_repetition;
		if (video_params.deep_color_mode != v.deep_color_mode)
		{
			if(v.deep_color_mode == 24 || v.deep_color_mode == 30 || v.deep_color_mode == 36 || v.deep_color_mode == 48)
			{
				log_notice("video handler: deep color mode changed = %d", v.deep_color_mode);
				video_params.deep_color_mode = v.deep_color_mode;
			}
		}
#if 1
		switch (video_params.deep_color_mode) {
		case 48:
			//dcm = 16 bits;
			factor_m = 2;
			break;
		case 36:
			//dcm = 12 bits;
			factor_m = 1.5;
			break;
		case 30:
			//dcm = 10 bits;
			factor_m = 1.25;
			break;
		default:
			//dcm = 8 bits;
			factor_m = 1.00;
			break;
		}
		tclk_temp = hdmi_rx_ctrl_get_tmds_clk(ctx);
		hdmi_rx_demo_pll((unsigned long)(tclk_temp*factor_m));
#endif
		error |= hdmi_rx_phy_config(&rx.phy, v.pixel_clk, v.deep_color_mode);
		error_counter = 0;
	}
	else
	{
		error_counter++;
		if (error_counter > 20)
		{
			error |= hdmi_rx_phy_config(&rx.phy,1, 36);
			hdmi_rx_demo_pll((unsigned long)(1450001));/*TODO*/
			error_counter = 0;
		}
	}
	video_if_adjust_zoom_autocenter();
	return error;
}

/**
 * Audio event handler
 * @param[in,out] ctx context information
 * @return error code
 */
static int audio_handler(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;
	struct hdmi_rx_ctrl_audio a;
	//struct hdmi_rx_ctrl *ctx = (struct hdmi_rx_ctrl (*))(param3);
	log_notice("HDMI Rx Demo: audio handler called");
	if (ctx == 0)
	{
		return -EINVAL;
	}
	error |= hdmi_rx_ctrl_get_audio(ctx, &a);
	if (error == 0)
	{
		/*ctx->log_info("Audio: CT=%u CC=%u SF=%u SS=%u CA=%u",
				a.coding_type, a.channel_count, a.sample_frequency,
				a.sample_size, a.channel_allocation);*/
	}
	video_if_adjust_zoom_autocenter();
	return error;
}

/**
 * Packet store configuration
 * @param[in,out] ctx context information
 * @param[in] type packet type to store
 * @param[in] enable enable/disable store
 * @return error code
 */
static int packet_store(struct hdmi_rx_ctrl *ctx, uint8_t type, bool enable)
{
	int error = 0;

	if (ctx == 0)
	{
		return -EINVAL;
	}
	switch (type)
	{
	case 0x01:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ACR, enable);
		break;
	case 0x03:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_GCP, enable);
		break;
	case 0x04:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ACP, enable);
		break;
	case 0x05:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ISRC1, enable);
		break;
	case 0x06:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_ISRC2, enable);
		break;
	case 0x0A:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_GAMUT, enable);
		break;
	case 0x81:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_VSI, enable);
		break;
	case 0x82:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_AVI, enable);
		break;
	case 0x83:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_SPD, enable);
		break;
	case 0x84:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_AIF, enable);
		break;
	case 0x85:
		error |= hdmi_rx_ctrl_store(ctx, hdmi_rx_ctrl_packet_MPEGS, enable);
		break;
	default:
		if (enable)
		{
			ctx->log_info("Packet Types");
			ctx->log_info("  0x01 - ACR");
			ctx->log_info("  0x03 - GCP");
			ctx->log_info("  0x04 - ACP");
			ctx->log_info("  0x05 - ISR1");
			ctx->log_info("  0x06 - ISR2");
			ctx->log_info("  0x0A - GAMUT");
			ctx->log_info("  0x81 - VSI");
			ctx->log_info("  0x82 - AVI");
			ctx->log_info("  0x83 - SPD");
			ctx->log_info("  0x84 - AIF");
			ctx->log_info("  0x85 - MPEGS");
		}
		error = -EINVAL;
		break;
	}
	return error;
}

/**
 * Packet event handler
 * @param[in,out] ctx context information
 * @return error code
 */
static int packet_handler(struct hdmi_rx_ctrl *ctx)
{
	int error = 0;
	uint8_t data[32] = {0};
	unsigned i = 0;

	if (ctx == 0)
	{
		return -EINVAL;
	}
	error |= hdmi_rx_ctrl_get_packet(ctx, data, sizeof(data));
	if (error == 0)
	{
		error |= packet_store(ctx, data[0], false);		/* disable */
		ctx->log_info("-> %02X", data[0]);
		for (i = 0; i < sizeof(data); i += 8)
		{
			ctx->log_info("   %02X %02X %02X %02X %02X %02X %02X %02X",
					data[i+0], data[i+1], data[i+2], data[i+3],
					data[i+4], data[i+5], data[i+6], data[i+7]);
		}
	}
	return error;
}



static int clock_handler_cb(struct hdmi_rx_ctrl *ctx)
{
	log_notice("----------------------------------------clock handler cb");
	return clock_handler(ctx);
}

static int video_handler_cb(struct hdmi_rx_ctrl *ctx)
{
	log_notice("----------------------------------------video handler cb");
	return video_handler(ctx);


}

static int audio_handler_cb(struct hdmi_rx_ctrl *ctx)
{
	log_notice("----------------------------------------audio handler cb");
	sprintf(hdmi_rx_buffer[0], "hdmirx");
	sprintf(hdmi_rx_buffer[1], "audio");
	sprintf(hdmi_rx_buffer[2], "change");
	hdmi_rx_buffer_no = 3;
	return audio_handler(ctx);

}

/**
 * AKSV event handler
 * @param[in,out] ctx context information
 * @return error code
 */
static int aksv_handler(struct hdmi_rx_ctrl *ctx)
{
	io_bus_write32(HDMI_RX_CTRL_BASE + 0x00b4, 0, 0x10000);
	log_notice("HDMI Rx Demo: aksv handler called.");
	int error = 0;
	long int wait_t = 0;
	if (ctx == 0)
	{
		return -EINVAL;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_video_status, video_handler_cb);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe video status");
	}
	while(wait_t < 1000000)
	{
		wait_t ++;
	}
	wait_t = 0;
	while(wait_t < 1000000)
	{
		wait_t ++;
	}
	wait_t = 0;
	while(wait_t < 1000000)
	{
		wait_t ++;
	}
	video_if_adjust_zoom_autocenter();
	return error;
}
/**
 * Configure HDMI RX controller
 * @param[in,out] ctx context information
 * @param[in] port input port 0 (A) or 1 (B)
 * @param[in] hdcp HDCP configuration
 * @return error code
 */
static int rx_ctrl_config(struct hdmi_rx_ctrl *ctx, unsigned port, const struct hdmi_rx_ctrl_hdcp *hdcp)
{
	int error = 0;
	if (ctx == 0 || hdcp == 0)
	{
		return -EINVAL;
	}
	error |= hdmi_rx_ctrl_config(ctx, port);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX controller configure %d", error);
		goto exit;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_clock_change, clock_handler_cb);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe clock change");
		goto exit;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_video_status, video_handler_cb);
	//error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_video_status, video_handler);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe video status");
		goto exit;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_audio_status, audio_handler_cb);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe audio status");
		goto exit;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_packet_reception, packet_handler);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe packet reception");
		goto exit;
	}
	error |= hdmi_rx_ctrl_subscribe(ctx, hdmi_rx_ctrl_event_aksv_reception, aksv_handler);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX subscribe AKSV reception");
		goto exit;
	}
	error |= hdmi_rx_ctrl_hpd(ctx, true);
	if (error != 0)
	{
		ctx->log_error(file, __LINE__, "RX hot plug detect output");
		goto exit;
	}
	exit:
	return error;
}

int hdmi_demo_rx_menu_main(char buf[][TOKEN_SIZE], int buffers_no, void * param)
{
	unsigned new_port;

	log_notice("hdmi rx MENU");
	/* parse user's options */
	if (buffers_no != 0)
	{
		if (strcmp(buf[0], "rxhdcppol") == 0)
		{
			hdcp22_rx_polling();
		}
		if (strcmp(buf[0], "txhdcppol") == 0)
		{
			hdcp22_tx_polling();
		}
		if (strcmp(buf[0], "rxhdcppol14") == 0)
		{
			hdcp14_rx_polling();
		}
		if (strcmp(buf[0], "txhdcppol14") == 0)
		{
			hdcp14_tx_polling();
		}
		/*
		if (strcmp(buf[0], "equalizer3") == 0)
		{
			log_notice(" stage 3 result: %d", hdmi_rx_phy_equalizerSw_Stage3v2(100,0));
		}
		if (strcmp(buf[0], "equalizer2") == 0)
		{
			log_notice(" stage 2 result: %d", hdmi_rx_phy_equalizerSw_Stage2v2(100,0));
		}
		if (strcmp(buf[0], "equalizer1") == 0)
		{
			log_notice(" stage 1 result: %d", hdmi_rx_phy_equalizerSw_Stage1(100,0));
		}
		 */
		if (strcmp(buf[0], "equalizer") == 0)
		{
			if (buffers_no >= 2)
			{
				if (strcmp(buf[1], "auto") == 0)
				{
					hdmi_rx_phy_equalizerManualSet(0, 0, 0, 1, rx.phy.version);
					log_notice("HDMI Rx Phy Equalization set to Automatic.");
				}
				else
				{
					if (buffers_no == 4)
					{
						hdmi_rx_phy_equalizerManualSet(strtoul(buf[1],NULL,0), strtoul(buf[2],NULL,0), strtoul(buf[3],NULL,0), 0, rx.phy.version);
						log_notice(" HDMI Rx Phy Equalization set to manual mode, value applied ch0: 0x%x", strtoul(buf[1],NULL,0));
						log_notice(" HDMI Rx Phy Equalization set to manual mode, value applied ch1: 0x%x", strtoul(buf[2],NULL,0));
						log_notice(" HDMI Rx Phy Equalization set to manual mode, value applied ch2: 0x%x", strtoul(buf[3],NULL,0));
					}
					else
					{
						/*same setting for the 3 channels*/
						hdmi_rx_phy_equalizerManualSet(strtoul(buf[1],NULL,0), strtoul(buf[1],NULL,0), strtoul(buf[1],NULL,0), 0, rx.phy.version);
						log_notice(" HDMI Rx Phy Equalization set to manual mode, value applied: 0x%x", strtoul(buf[1],NULL,0));
					}
				}
			}
			else
			{
				log_error("Incorrect parameters.");
			}
		}
		if (strcmp(buf[0], "scanequalizer") == 0)
		{
			if (buffers_no == 2)
			{
				hdmi_rx_phy_equalizerSwAutoset(strtoul(buf[1],NULL,0), rx.phy.version);
			}
			else
			{
				hdmi_rx_phy_equalizerSwAutoset(0, rx.phy.version);
			}
		}
		if (strcmp(buf[0], "equalizerwa") == 0)
		{
			if (buffers_no == 11)
			{
				int eq = 0;
				eq = (strtoul(buf[3],NULL,0) << 7) | (strtoul(buf[4],NULL,0) << 6) | (strtoul(buf[5],NULL,0) << 5)
																								| (strtoul(buf[6],NULL,0) << 4) | (strtoul(buf[7],NULL,0) << 3) | (strtoul(buf[8],NULL,0) << 2)
																								| (strtoul(buf[9],NULL,0) << 1) | (strtoul(buf[10],NULL,0) << 0);
				log_notice("eq = 0x%x", eq);
				log_notice("error counters enabled sum = 0x%x", strtoul(buf[2],NULL,0));
				hdmi_rx_phy_equalizer_SwWA(strtoul(buf[1],NULL,0), eq, strtoul(buf[2],NULL,0), rx.phy.version);
			}
			else
			{
				if (buffers_no == 2)
				{
					hdmi_rx_phy_equalizer_SwWA(strtoul(buf[1],NULL,0), 0, strtoul(buf[2],NULL,0), rx.phy.version);
				}
				else
				{
					log_notice("Incorrect number of arguments.");
					log_notice("Usage: equalizerwa <waiting time p/error count> <counters to be enabled> <eq_sett7>...<eq_sett0>");
					log_notice("If only the waiting time is defined, the algorithm will run thru all the settings.\n");
					log_notice("Character Error Detection - Video Data period check enable	0x10");
					log_notice("Character Error Detection - Data Island Period check enable	0x08");
					log_notice("Character Error Detection - Guard Band Period check enable	0x04");
					log_notice("Character Error Detection - Preamble Period check enable	0x02");
					log_notice("Character Error Detection - Control Period check enable	0x01");
				}
			}
		}
		if (strcmp(buf[0], "scdcstat") == 0)
		{
			hdmi_rx_scdc_getStatusRegisters();
		}
		if (strcmp(buf[0], "scdcsetsinkver") == 0)
		{
			hdmi_rx_scdc_setSinkVersion(0x01);
			log_notice("Done.");
		}
		if (strcmp(buf[0], "scdcpower") == 0)
		{
			hdmi_rx_scdc_setPowerProvided(strtoul(buf[1],NULL,0));
			log_notice("SCDC PowerProvided set to %d", strtoul(buf[1],NULL,0));
		}
		else if (strcmp(buf[0], "scrambling") == 0)
		{
			hdmi_rx_scdc_scrambling(strtoul(buf[1], NULL, 0));
		}
		else if (strcmp(buf[0], "errorcnt") == 0) /* SCDC Error counting */
		{
			if (buffers_no > 1)
			{
				if(buffers_no >= 3)
				{
					/*	Register 0x800
					Bits 		Name 			R/W 	Description
					31:13 								Reserved and read as zero
					12 			viddatachecken 	R/W 	Character Error Detection - Video Data period check enable
					11 			dataischecken 	R/W 	Character Error Detection - Data Island Period check enable
					10 			gbchecken 		R/W 	Character Error Detection - Guard Band Period check enable
					9 			preambchecken 	R/W 	Character Error Detection - Preamble Period check enable
					8 			ctrlchecken 	R/W 	Character Error Detection - Control Period check enable
					7:5 								Reserved and read as zero
					4 			scdc_enable 	R/W 	Status and Control Data Channel enable
														0 - SCDC disable
														1 - SCDC enable"
					3:2 								Reserved and read as zero
					1:0 								scramben_sel R/W Scrambling enable selection:
														0: SCDC control
														1: auto -> enable when detected scrambled data
														2: auto -> enable when detected scrambled data
														3: forces descrambling
					 */
					log_notice("------------Enable specific error counter");
					hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(strtoul(buf[1], NULL, 0), strtoul(buf[2], NULL, 0), 0);
				}
				else
				{
					/* Enable/Disable error counting */
					hdmi_rx_scdc_errorCntConfig(strtoul(buf[1], NULL, 0), 0);
				}
			}
			else
			{
				log_notice("------------------error count read");
				hdmi_rx_scdc_errorCnt(1,1);
			}
		}
		if (strcmp(buf[0], "phyport") == 0)
		{
			if (buffers_no == 2)
			{
				new_port = (unsigned) strtoul(buf[1],NULL,0) -1 ;
				hdmi_rx_ctrl_fast_switching(new_port);
			}
		}

		else if (strcmp(buf[0], "dcmbrg") == 0)
		{
			if (buffers_no == 2)
			{
				dcm = strtoul(buf[1], NULL, 0);
				hdmi_rx_change_dcm (dcm);
			}
			else
			{
				log_notice("RX: Error changing DCM, incorrect number of arguments.");
			}

		}
		else if (strcmp(buf[0], "repeater") == 0)
		{
			hdmi_demo_repeater_cmd();
		}
		else if (strcmp(buf[0], "plldrp1") == 0)
		{
			phy_wrapper_setup_bsp(200000);
		}
		else if (strcmp(buf[0], "plldrp2") == 0)
		{
			phy_wrapper_setup_bsp(110000);
		}
		else if (strcmp(buf[0], "plldrp3") == 0)
		{
			phy_wrapper_setup_bsp(300000);
		}
		else if (strcmp(buf[0], "demo") == 0)
		{
			demo_rx_enabled = 1;
			hdmi_rx_demo();
			log_notice("Hdmi Rx Demo enabled.");
		}
		else if (strcmp(buf[0], "globalphase") == 0)
		{
			globalphase = strtoul(buf[1], NULL, 0);
			log_notice("globalphase = %d", globalphase);
		}
		else if (strcmp(buf[0], "apply") == 0)
		{
			if(buffers_no == 2)
			{
				log_notice("globalphase = %d", globalphase);
				hdmi_rx_demo_pll((unsigned long)(buf[1]));
			}
			else
			{
				log_notice("Error: missing clock value.");
			}
		}
		if (strcmp(buf[0], "pllrx") == 0)

		{
			if (buffers_no == 3)
			{
				hdmi_rx_board_pixel_clock(0x00, strtoul(buf[1], NULL, 0), strtoul(buf[2], NULL, 0));
				log_notice("Phase applied.");
			}
			else
			{
				log_notice("Not enought parameteres. Command not applied");
				log_notice("Usage: pllrx <frequency> <dcm mode (8-12)>");
			}
		}
		if (strcmp(buf[0], "phaserx1212") == 0)
		{
			if (buffers_no == 2)
			{
				if (strcmp(buf[1], "0") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 0 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase0 applied.");
				}
				if (strcmp(buf[1], "22") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 22,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase22 applied.");
				}
				if (strcmp(buf[1], "45") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 45 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase45 applied.");

				}
				if (strcmp(buf[1], "67") == 0)
				{

					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 67,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase67 applied.");

				}
				if (strcmp(buf[1], "90") == 0)
				{

					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 90 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase90 applied.");

				}
				if (strcmp(buf[1], "112") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 112,5 deg */
					/* tclk N = 12, M = 12, Fase = 112,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase112 applied.");

				}
				if (strcmp(buf[1], "135") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 135 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "157") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 157,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0005);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0005);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0005);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase157 applied.");

				}
				if (strcmp(buf[1], "180") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 180 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");

				}
				if (strcmp(buf[1], "202") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 202,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase202 applied.");

				}
				if (strcmp(buf[1], "225") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 225 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase225 applied.");

				}
				if (strcmp(buf[1], "247") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 247,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0008);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0008);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0008);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase247 applied.");
				}
				if (strcmp(buf[1], "270") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 270 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase270 applied.");
				}
				if (strcmp(buf[1], "292") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 292,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase292 applied.");
				}
				if (strcmp(buf[1], "315") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 315 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");
				}
				if (strcmp(buf[1], "337") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 12, M = 12, Fase = 337,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000b);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000b);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5186);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000b);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase337 applied.");
				}

			}
		}
		if (strcmp(buf[0], "phaserx44") == 0)
		{
			if (buffers_no == 2)
			{
				if (strcmp(buf[1], "0") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 0 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase0 applied.");
				}
				if (strcmp(buf[1], "22") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 22,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase22 applied.");
				}
				if (strcmp(buf[1], "45") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 45 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase45 applied.");

				}
				if (strcmp(buf[1], "66") == 0)
				{
					//fase 135 M=7 N=7
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 67,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					////drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "90") == 0)
				{

					//fase 180, M= 7, N= 7
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 90 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);
					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");

				}
				if (strcmp(buf[1], "112") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 112,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase112 applied.");

				}
				if (strcmp(buf[1], "135") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 135 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "157") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 157,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase157 applied.");

				}
				if (strcmp(buf[1], "180") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 180 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");

				}
				if (strcmp(buf[1], "202") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 202,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase202 applied.");

				}
				if (strcmp(buf[1], "225") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 225 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase225 applied.");

				}
				if (strcmp(buf[1], "247") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 247,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase247 applied.");
				}
				if (strcmp(buf[1], "270") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 270 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase270 applied.");
				}
				if (strcmp(buf[1], "292") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 292,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x5082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase292 applied.");
				}
				if (strcmp(buf[1], "315") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 315 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x9082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");
				}
				if (strcmp(buf[1], "337") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 4, M = 4, Fase = 337,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd082);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd082);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase337 applied.");
				}

			}
		}
		if (strcmp(buf[0], "phaserx2424") == 0)
		{
			if (buffers_no == 2)
			{

				if (strcmp(buf[1], "0") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 0 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase0 applied.");
				}
				if (strcmp(buf[1], "22") == 0)
				{
					//fase 45 M=3, N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 22,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase22 applied.");
				}
				if (strcmp(buf[1], "45") == 0)
				{

					//fase 90 M=3, N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 45 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase45 applied.");

				}
				if (strcmp(buf[1], "67") == 0)
				{
					//fase 135 M=3 N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 67,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase67 applied.");

				}
				if (strcmp(buf[1], "90") == 0)
				{

					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 90 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0006);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase90 applied.");

				}
				if (strcmp(buf[1], "112") == 0)
				{

					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 112,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0007);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase112 applied.");

				}
				if (strcmp(buf[1], "135") == 0)
				{

					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 135 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0009);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "157") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 157,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000a);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");

				}
				if (strcmp(buf[1], "180") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 180 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");
				}
				if (strcmp(buf[1], "202") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 202,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000d);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000d);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000d);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase202 applied.");
				}
				if (strcmp(buf[1], "225") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 225 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x000f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x000f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x000f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase225 applied.");
				}
				if (strcmp(buf[1], "247") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 247,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0010);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0010);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0010);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase247 applied.");
				}
				if (strcmp(buf[1], "270") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 270 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0012);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0012);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0012);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase270 applied.");
				}
				if (strcmp(buf[1], "292") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 292,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0013);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0013);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0013);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase292 applied.");
				}
				if (strcmp(buf[1], "315") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 315 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0015);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0015);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0015);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");
				}
				if (strcmp(buf[1], "337") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 24, M = 24, Fase = 337,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0016);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0016);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x930c);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0016);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase337 applied.");
				}
			}
		}
		if (strcmp(buf[0], "phaserx66") == 0)
		{
			if (buffers_no == 2)
			{
				if (strcmp(buf[1], "0") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 0 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase0 applied.");
				}
				if (strcmp(buf[1], "22") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 22,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x70c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x70c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x70c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase22 applied.");
				}
				if (strcmp(buf[1], "45") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 45 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase45 applied.");

				}
				if (strcmp(buf[1], "67") == 0)
				{
					//fase 135 M=7 N=7
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 67,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x30c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x30c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x30c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase67 applied.");

				}
				if (strcmp(buf[1], "90") == 0)
				{

					//fase 180, M= 7, N= 7
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 90 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x90c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x90c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x90c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase90 applied.");

				}
				if (strcmp(buf[1], "112") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 112,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xf0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xf0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0001);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xf0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase112 applied.");

				}
				if (strcmp(buf[1], "135") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 135 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x50c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x50c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x50c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "157") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 157,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xb0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xb0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0002);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xb0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0002);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase157 applied.");

				}
				if (strcmp(buf[1], "180") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 180 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x10c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x10c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");

				}
				if (strcmp(buf[1], "202") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 202,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x70c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x70c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x70c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase202 applied.");

				}
				if (strcmp(buf[1], "225") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 225 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xd0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xd0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase225 applied.");

				}
				if (strcmp(buf[1], "247") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 247,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x30c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x30c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x30c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase247 applied.");
				}
				if (strcmp(buf[1], "270") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 270 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x90c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x90c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x90c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase270 applied.");
				}
				if (strcmp(buf[1], "292") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 292,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xf0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xf0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0004);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xf0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0004);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase292 applied.");
				}
				if (strcmp(buf[1], "315") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 315 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x50c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0005);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x50c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0005);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x50c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0005);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");
				}
				if (strcmp(buf[1], "337") == 0)
				{
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					/* tclk N = 6, M = 6, Fase = 337,5 deg */
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xb0c3);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0005);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0xb0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0005);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xb0c3);
					//					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0005);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase337 applied.");
				}

			}
		}

		if (strcmp(buf[0], "phaserx2") == 0)
		{
			if (buffers_no == 2)
			{
				if (strcmp(buf[1], "0") == 0)
				{
					//fase 0 M=3, N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x0142);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x0198);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase0 applied.");
				}
				if (strcmp(buf[1], "45") == 0)
				{
					//fase 45 M=3, N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);


					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x6042);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x0198);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x7042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x6042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase45 applied.");
				}
				if (strcmp(buf[1], "90") == 0)
				{

					//fase 90 M=3, N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x6042);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x0198);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xd042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x6042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);
					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase90 applied.");

				}
				if (strcmp(buf[1], "135") == 0)
				{
					//fase 135 M=3 N=3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x6042);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0081);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x1058);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x3042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x6042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0081);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase135 applied.");

				}
				if (strcmp(buf[1], "180") == 0)
				{

					//fase 180, M= 3, N= 3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x9042);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0081);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x1058);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9042);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase180 applied.");

				}
				if (strcmp(buf[1], "225") == 0)
				{

					//fase 225, M= 3, N= 3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//	drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//	drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xf104);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0081);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x1058);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xf104);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0081);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase225 applied.");

				}
				if (strcmp(buf[1], "270") == 0)
				{

					//fase 225, M= 3, N= 3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x5104);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x1058);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x5104);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0082);

					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase270 applied.");

				}
				if (strcmp(buf[1], "315") == 0)
				{

					//fase 225, M= 3, N= 3
					drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

					//		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1042);
					//		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0xb104);
					//						drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0082);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x1058);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x21d1);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x3001);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x9042);
					//				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0xb104);
					drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0082);


					drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
					log_notice("phase315 applied.");

				}
			}
		}

		else if (strcmp(buf[0], "repeaterinit") == 0)
		{
			hdmi_demo_repeater_init();
		}
		else if (strcmp(buf[0], "pkt") == 0)
		{
			packet_store(&rx.ctrl, strtoul(buf[1], 0, 0), true);
		}
		else if (strcmp(buf[0], "dbg") == 0)
		{	/* hidden */
			hdmi_rx_phy_debug(&rx.phy);
			hdmi_rx_ctrl_debug(&rx.ctrl);
			hdmi_rx_ctrl_hdcp_debug(&rx.ctrl);
		}
		else if (strcmp(buf[0], "c") == 0)
		{
			if (buffers_no == 3)
			{
				hdmi_rx_ctrl_write(strtoul(buf[1], NULL, 0), strtoul(buf[2], NULL, 0));
			}
			else if (buffers_no == 2)
			{
				log_notice("hdmi rx read 0x%X:0x%X", strtoul(buf[1], NULL, 0), hdmi_rx_ctrl_read(strtoul(buf[1], NULL, 0)));
			}
		}
		else if (strcmp(buf[0], "phyrw") == 0) {

			if (buffers_no == 3)
			{
				phy_wrapper_write(strtoul(buf[1], NULL, 0),strtoul(buf[2], NULL, 0));
			}
			else if (buffers_no == 2)
			{
				log_notice("0x%02X", phy_wrapper_read(strtoul(buf[1], NULL, 0)));
				log_notice("0x%02X", phy_wrapper_read(strtoul(buf[1], NULL, 0)));
			}

		}
		else if (strcmp(buf[0], "repeaterman") == 0)
		{
			hdmi_demo_repeater_manual_update();
		}
		else if (strcmp(buf[0], "phyclose") == 0) {


			if (hdmi_rx_phy_close(&rx.phy) < 0) {
				log_notice("RX PHY closing failed");
			}else
			{
				log_notice("RX PHY: successfully closed");
			}
		}
		else if (strcmp(buf[0], "phyopen") == 0) {


			if (hdmi_rx_phy_open(&rx.phy) < 0) {
				log_notice("RX PHY opening failed");
			}else
			{
				log_notice("RX PHY: successfully opened");
			}
		}
		else if (strcmp(buf[0], "phycfg") == 0) {


			if (hdmi_rx_phy_config(&rx.phy, rx.phy.cfg_clk, 8) < 0) {
				log_notice("RX PHY opening failed");
			}else
			{
				log_notice("RX PHY: successfully opened");
			}
		}


		else if (strcmp(buf[0], "starttx") == 0)
		{
			const u8 vName[] = "Synopsys";
			const u8 pName[] = "HDMI Tx";
			dtd_t tmp_dtd;
			LOG_MENUSTR("HDMI Tx demo: ", __DATE__);
			demo_Start();

			Rx_pProduct = (productParams_t*)malloc(sizeof(productParams_t));
			Rx_pAudio = (audioParams_t*)malloc(sizeof(audioParams_t));
			Rx_pVideo = (videoParams_t*)malloc(sizeof(videoParams_t));
			//Rx_pHdcp = (hdcpParams_t*)malloc(sizeof(hdcpParams_t));
			productParams_Reset(Rx_pProduct);
			productParams_SetVendorName(Rx_pProduct, vName, sizeof(vName) - 1);
			productParams_SetProductName(Rx_pProduct, pName, sizeof(pName) - 1);
			productParams_SetSourceType(Rx_pProduct, 0x0A);
			audioParams_Reset(Rx_pAudio);
			audioParams_SetInterfaceType(Rx_pAudio, I2S);
			audioParams_SetClockFsFactor(Rx_pAudio, 128);
			videoParams_Reset(Rx_pVideo);
			videoParams_SetHdmi(Rx_pVideo,1);
			dtd_Fill(&tmp_dtd, 1, 60000);
			videoParams_SetDtd(Rx_pVideo, &tmp_dtd);
			//hdcpParams_Reset(Rx_pHdcp);
			log_notice("STARTTX0");
			api_Configure(Rx_pVideo, Rx_pAudio, Rx_pProduct, Rx_pHdcp, hdmitx_phy); /* E108 */

		}
		else if (strcmp(buf[0], "int") == 0)
		{
			interrupts_enableSpecificDemo(INT_MASK_HDMI_RX, 2);
		}
		else if (strcmp(buf[0], "intoff") == 0)
		{
			interrupts_disableSpecificDemo(INT_MASK_HDMI_RX, 2);
		}
		else if (strcmp(buf[0], "pddq") == 0)
		{
			if (buffers_no == 2)
			{
				phy_wrapper_pddq(strtoul(buf[1], NULL, 0));
			}
		}
		else if (strcmp(buf[0], "svs") == 0)
		{
			if (buffers_no == 2)
			{
				phy_wrapper_svsretmode(strtoul(buf[1], NULL, 0));
			}
		}
		else if (strcmp(buf[0], "phyres") == 0)
		{
			if (buffers_no == 2)
			{
				phy_wrapper_phy_reset(strtoul(buf[1], NULL, 0));
			}
		}
		else if (strcmp(buf[0], "ceares") == 0)
		{
			hdmi_rx_ctrl_write (0x400,0x80000000);
			hdmi_rx_ctrl_write (0x400,0x0);
			log_notice("Rx Ctrl CEA Output Reseted");
		}
		else if (strcmp(buf[0], "printedid") == 0)
		{
			hdmi_demo_rx_print_edid();
		}
		else if (strcmp(buf[0], "ceaman") == 0)
		{
			if (buffers_no == 2)
			{
				switch(strtoul(buf[1],NULL,0))
				{
				case 1:
					log_notice("Rx Ctrl CEA Output Manual Mode VM=1");
					// Force CEA registers to manual mode and CEA VM = 1
					hdmi_rx_ctrl_write (0x408,0x00a00280);
					hdmi_rx_ctrl_write (0x40C,0x00100060);
					hdmi_rx_ctrl_write (0x410,0x002d01e0);
					hdmi_rx_ctrl_write (0x414,0x000a0002);
					hdmi_rx_ctrl_write (0x400,0x80000000);
					hdmi_rx_ctrl_write (0x400,0x3);
					break;

				case 2:
				case 3:
					log_notice("Rx Ctrl CEA Output Manual Mode VM=2/3");
					// Force CEA registers to manual mode and CEA VM = 2/3
					hdmi_rx_ctrl_write (0x408,0x008a02d0);
					hdmi_rx_ctrl_write (0x40C,0x0010003e);
					hdmi_rx_ctrl_write (0x410,0x002d01e0);
					hdmi_rx_ctrl_write (0x414,0x00090006);
					hdmi_rx_ctrl_write (0x400,0x80000000);
					hdmi_rx_ctrl_write (0x400,0x3);
					break;

				case 4:
					log_notice("Rx Ctrl CEA Output Manual Mode VM=4");
					// Force CEA registers to manual mode and CEA VM = 4
					hdmi_rx_ctrl_write (0x408,0x01720500);
					hdmi_rx_ctrl_write (0x40C,0x006e0028);
					hdmi_rx_ctrl_write (0x410,0x001e02d0);
					hdmi_rx_ctrl_write (0x414,0x00050005);
					hdmi_rx_ctrl_write (0x400,0x80000000);
					hdmi_rx_ctrl_write (0x400,0xf);
					break;

				}
			}
		}
		else if (strcmp(buf[0], "pllrec") == 0)
		{
			// same cfg as 7500
			drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1145);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT1_REG_1,0x9145);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT1_REG_2,0x0002);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0118);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21D1);
			drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
			drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
			//log_notice("config test");

			// tclk pll config < 7425
			drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1145);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT1_REG_1,0x9145);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT1_REG_2,0x0002);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0118);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21D1);
			drp_write(RX_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
			drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
			log_notice("PLL reconfigured 1");
		}
		else if (strcmp(buf[0], "bridge") == 0)
		{
			hdmi_rx_bridge_cmd();
		}
		else if (strcmp(buf[0], "clk") == 0)
		{
			log_notice("tmds_clk_rx: 0x%x", hdmi_rx_ctrl_read(0x09C));
			//log_notice("Rx Tclk Pll lock: 0x%x",io_bus_read32((RX_PLL_BASE_ADDRESS + 0x08UL), 0x00));
			log_notice("Rx VALID: 0x%x", hdmi_rx_ctrl_read(0x030));
			log_notice("Video Pll lock: 0x%x",io_bus_read32((VIDEO_PLL_BASE_ADDRESS + 0x08UL), 0x00));
			log_notice("Tx Tclk Pll lock: 0x%x",io_read8(RX_PLL_BASE_ADDRESS + 0x7508));
			log_notice("Tx PHY Pll lock:0x%x",io_bus_read32(((HDMI_TX_CTRL_BASE + 0x3004) & 0x1), 0x00));
			log_notice("Audio Pll lock: 0x%x",io_bus_read32(AUDIO_PLL_BASE_ADDRESS + 0x7308, 0x00));
			log_notice("RX Status Interrupt Reg (0xfe0):0x%x", hdmi_rx_ctrl_read(0xfe0));
			log_notice("RX Interrupt Enable Reg (0xfe4):0x%x", hdmi_rx_ctrl_read(0xfe4));
			log_notice("RX Interrupt Clear Reg (0xfe8):0x%x", hdmi_rx_ctrl_read(0xfe8));
			log_notice("md_hact_px - dcm dependent (0x150):%d", hdmi_rx_ctrl_read(0x6150)); //md_hact_px 0x150 // md_val 0x168
			log_notice("md_val (0x168):%d", hdmi_rx_ctrl_read(0x6168));
		}
		else if (strcmp(buf[0], "audiowr") == 0)
		{
			Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x1010, 0x02, 0x48);
			Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x0A0A, 0x08, 0x4B);

		}
		else if (strcmp(buf[0], "audiord") == 0)
		{
			Snps_I2cReadRequest(AUDIO_IIC_BASE_ADDRESS, 0x08);
			os_sleep(200);
			log_notice("Audio DAC i2c result 0x08: %d",Snps_I2cRead(AUDIO_IIC_BASE_ADDRESS, 0x4B));
		}
		else if (strcmp(buf[0], "audiord1") == 0)
		{
			Snps_I2cRead(AUDIO_IIC_BASE_ADDRESS, 0x48);
			Snps_I2cReadRequest(AUDIO_IIC_BASE_ADDRESS, 0x02);
			os_sleep(200);
			log_notice("Audio DAC i2c result 0x02: %d",Snps_I2cRead(AUDIO_IIC_BASE_ADDRESS, 0x48));
		}
		else if (strcmp(buf[0], "audiocfg") == 0)
		{
			if (buffers_no == 1)
			{
				Snps_I2cReadRequest(AUDIO_IIC_BASE_ADDRESS, 0x02);
				os_sleep(200);
				log_notice("Audio DAC i2c result: %d",Snps_I2cRead(AUDIO_IIC_BASE_ADDRESS, 0x48));
				//log_notice("Audio DAC i2c result: %d",Snps_I2cRead(AUDIO_IIC_BASE_ADDRESS, 0x4B));
				//log_notice("Audio DAC i2c result: %d",xilinx_i2c_write(0x81620000, 0x48, 0x02, 0x1010));
				//log_notice("Audio PLL i2c result: %d",xilinx_i2c_write(0x81620000, 0x4B, 0x08, 0x0A0A));

				// commute audio on tx, 64FS
				io_write8(0xc9803206,0x4);

				// fifo reset
				hdmi_rx_ctrl_write(0x240, 0x10001);
				hdmi_rx_ctrl_write(0x240, 0x10000);
			}
			else if (buffers_no == 3)
			{
				/*TODO*/ /* Do porting for Snps I2c controler*/
				//log_notice("i2c write: %d",xilinx_i2c_write(0x81620000, strtoul(buf[1], NULL, 0), strtoul(buf[1], NULL, 0), strtoul(buf[1], NULL, 0)));
			}
		}
		else if (strcmp(buf[0], "fifores") == 0)
		{
			//hdmi_rx_ctrl_write (0x258,0x0c0f8);
			hdmi_rx_ctrl_write (0x260,0x021);
			sleep(200);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			//hdmi_rx_ctrl_write (0x258,0x0c0d8);
			sleep(200);
			hdmi_rx_ctrl_write (0x260,0x001);
		}
		else if (strcmp(buf[0], "phystat") == 0)
		{
			log_notice("PHY STATUS : [-1 : Not connected | 0 : Closed | 1 : Opened | 2: Ready] :%d",hdmi_demo_rx_check_phy_status());
		}
		else if (strcmp(buf[0], "checkrx") == 0)
		{
			log_notice("Check if Audio or Video Mode has changed");
			hdmi_demo_rx_check_audio_video_mode();
		}
		else if (strcmp(buf[0], "cfgtx") == 0)
		{
			log_notice("Apply current configurations to HDMI TX");

			// DCM Rx
			/* TODO Should not be necessary to call it each time. Improve this */
			hdmi_rx_change_dcm(Rx_pVideo->mColorResolution);

			// Apply CEA mode change to HDMI TX
			Rx_pAudio->mInterfaceType = 0; // commute to i2s input
			api_Configure(Rx_pVideo, Rx_pAudio, Rx_pProduct, NULL, hdmitx_phy);

			//Rx Ctrl - CEA output reset
			hdmi_rx_ctrl_write (0x400,0x80000000);
			hdmi_rx_ctrl_write (0x400,0x0);

			// Audio fifo reset
			//hdmi_rx_ctrl_write (0x258,0x0c0f8);
			hdmi_rx_ctrl_write (0x260,0x021);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			//hdmi_rx_ctrl_write (0x258,0x0c0d8);
			hdmi_rx_ctrl_write (0x260,0x001);
		}
		else if (strcmp(buf[0], "hpd") == 0)
		{
			log_notice("Hot Plug Detect Pulse");
			/* Turn on HPD - all the ports*/
			hdmi_rx_ctrl_write(0x0,0x0000C01); /* TODO: replace this write by a function which will set the HPD of the active port */
			os_sleep(200);
			hdmi_rx_ctrl_write(0x0,0xF000C01); /* TODO: replace this write by a function which will set the HPD of the active port */
		}
#ifdef SC_PLL
		else if (strcmp(buf[0], "refresh") == 0)
		{
			hdmi_rx_refresh_audio_clk(&rx.ctrl);
			//Reset Audio FIFO after change in sample frequency
			//////////////////////////////
			hdmi_rx_ctrl_write (0x260,0x021);
			sleep(200);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			sleep(200);
			hdmi_rx_ctrl_write (0x260,0x001);
		}
#endif
		else if (strcmp(buf[0], "help") == 0)
		{
			log_notice("************************************************************");
			log_notice("* REPEATER Commands                                        *");
			log_notice("************************************************************");
			log_notice("* repeaterinit     - Initialize repeater                   *");
			log_notice("* repeater         - TURN-ON repeater demo                 *");
			log_notice("* checkrx          - Check what Rx controller is receiving *");
			log_notice("* cfgtx            - Reconfigure Tx with info Rx received  *");
			log_notice("*                                                          *");
			log_notice("************************************************************");
			log_notice("* Debug Commands                                           *");
			log_notice("************************************************************");
			log_notice("* phystat          - Check phy status and wake-up          *");
			log_notice("* fifores          - Reset Audio FIFOs                     *");
			log_notice("* clk              - Check clocks in the system (Rx and Tx)*");
			log_notice("*                                                          *");
			log_notice("************************************************************");
		}
		else if (strcmp(buf[0], "exit") == 0)
		{
			return 0;
		}

	}
	return 1;
}

/**
 * Main thread. Configure HDMI RX. User interface.
 * @param[in] param thread parameter
 * @return error code
 */
void* hdmi_demo_rx_entry_point(int *phy)
{
	int param = 0;
	/* HDMI RX PHY */
	if(*phy > 0)
	{
		rx.phy.version = *phy;
		log_notice("Rx PHY %d", rx.phy.version);
		if(*phy >= 205)
		{
			rx.phy.generation = 3;
			log_notice("Rx PHY GEN 3");
		}
		else
		{
			rx.phy.generation = 2;
			log_notice("Rx PHY GEN 2");
		}
	}
	else
	{
		rx.phy.version = 205;
		rx.phy.generation = 3;
		log_notice("Demo RX: Invalid Phy value received. Default Phy defined: E%d", rx.phy.version);
	}
	rx.phy.log_error = log_trace;
	rx.phy.log_info = log_notice;
	rx.phy.bsp_reset = phy_wrapper_phy_reset;
	rx.phy.bsp_pddq = phy_wrapper_pddq;
	rx.phy.bsp_svsmode = phy_wrapper_svsretmode;
	rx.phy.bsp_zcal_reset = phy_wrapper_zcal_reset;
	rx.phy.bsp_zcal_done = phy_wrapper_zcal_done;
	rx.phy.bsp_config = phy_wrapper_setup_bsp;
	rx.phy.bsp_read = phy_wrapper_read;
	rx.phy.bsp_write = phy_wrapper_write;
	/* HDMI RX controller */
	rx.ctrl.log_error = log_trace;
	rx.ctrl.log_info = log_notice;
	rx.ctrl.bsp_reset = hdmi_rx_ctrl_reset;
	rx.ctrl.bsp_reset_hdmi = hdmi_rx_ctrl_reset_hdmi;
	rx.ctrl.bsp_edid_update = hdmi_demo_rx_ctrl_edid_update;
	rx.ctrl.bsp_read = hdmi_rx_ctrl_read;
	rx.ctrl.bsp_write = hdmi_rx_ctrl_write;
	rx.ctrl.spin_init = os_spin_init;
	rx.ctrl.spin_destroy = os_spin_destroy;
	rx.ctrl.spin_lock_irq_save = os_spin_lock_irq_save;
	rx.ctrl.spin_unlock_irq_restore = os_spin_unlock_irq_restore;
	rx.ctrl.cfg_clk = 62500;
	rx.ctrl.md_clk = 25000;
	log_notice("RX: 0x%x", hdmi_rx_ctrl_read(0xff8));
	log_notice("RX: 0x%x", hdmi_rx_ctrl_read(0xffc));
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x1010, 0x02, 0x48);
	sleep(50);
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x0A0A, 0x08, 0x4B);
	sleep(50);
	/* Set Audio DAC Volume to half scale */
	//Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x7f7f, 0x05, 0x48);
	hdmi_rx_set_volume(70);
	sleep(50);
	//Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x7f7f, 0x06, 0x48);
	hdmi_rx_set_volume(70);
	sleep(50);
	//xilinx_i2c_write(0x81620000, 0x48, 0x05, 0x7f7f);
	//xilinx_i2c_write(0x81620000, 0x48, 0x06, 0x7f7f);
	/* HDMI RX PHY */
	rx.phy.cfg_clk = rx.ctrl.md_clk;
	if (param != 0)
	{
		log_notice("input parameter will be ignored");
	}
	else if (hdmi_rx_ctrl_open(&rx.ctrl) < 0) {
		log_error("RX controller open");
		goto exit_phy;
	}
	else if (hdmi_rx_ctrl_hdcp_config(&rx.ctrl, &rx.hdcp))
	{
		log_error("RX HDCP configure");
		goto exit;
	}
#ifdef INTR
	//interrupts_enableSpecificDemo(INT_MASK_HDMI_RX, 2);
	/*else if (os_irq_request(rx.irq, irq_handler, &rx) < 0)
	{
		log_error("RX IRQ request");
		goto exit_ctrl;

	}*/
#endif
	if (rx_ctrl_config(&rx.ctrl, rx.port, &rx.hdcp) < 0)
	{
		goto exit_irq;
	}
	/* Turn on HPD - Hot PlugDetect output,but turn off specific HPD output */
	hdmi_rx_ctrl_write(0x0,0x0f000c01); /* Hot Plug Detect off all ports */
	//log_notice("entry8");
	if (hdmi_rx_ctrl_hpd(&rx.ctrl,true) <0) { /* Enable HPD (bit 0), but keep HPD of all ports unchanged*/
		goto exit;
	}
	if (hdmi_rx_phy_open(&rx.phy) < 0)
	{
		log_error("RX PHY open");
		goto exit;
		/* For Fast Switching all HDMI RX Interrupts must be disabled */
	}
	hdmi_rx_phy_config(&rx.phy,27000,8); //--
	hdmi_rx_phy_fast_switching(&rx.phy, 1);
	if(rx.phy.version == 213)
	{
		/* workaround for E213 daugther cards */
		//hdmi_rx_ctrl_write(0x2c0, 0x40 + (unsigned) (0 << 2));
		rx.phy.bsp_write(0x03, rx.phy.bsp_read(0x03) | 0x0004);
	}
	//hdmi_demo_rx_check_phy_status();
	//log_notice("check phy status done");
	interrupts_enableSpecificDemo(INT_MASK_HDMI_RX, 2);
	log_notice("HDMI RX demo: " __TIME__ " " __DATE__);
	return 0;
	exit_irq:
	if (os_irq_free(rx.irq) < 0) {
		log_trace(file, __LINE__, "RX IRQ free");
	}
#ifdef INTR
	exit_ctrl:
#endif
	if (hdmi_rx_ctrl_close(&rx.ctrl) < 0) {
		log_trace(file, __LINE__, "RX controller close");
	}
	exit_phy:
	if (hdmi_rx_phy_close(&rx.phy) < 0) {
		log_trace(file, __LINE__, "RX PHY close");
	}
	exit:
	return 0;
}

/**
 * HDMI RX Check PHY STATUS
 * @return phy status (-1=Not connected, 0=Closed, 1=Opened, 2=PHY Receiving data)
 */

int hdmi_demo_rx_check_phy_status()
{

	static int phy_status = 0;
#if 1
	/* If no TMDSvalid, and no TMDS clock then restart RX PHY macro */
	if ((((hdmi_rx_ctrl_read(0x30)& (uint32_t)0x01)==0 ) || hdmi_rx_ctrl_read(0x09C)== (uint32_t)0x30000 )&& (phy_status > 0) ){
		//	if ((hdmi_rx_ctrl_read(0x09C)==(uint32_t)0x10000 )&& (phy_status > 0) ){
		os_sleep(1000); /* Wait 500ms to check if PHY recovers */
		if ((((hdmi_rx_ctrl_read(0x30)& (uint32_t)0x01)==0 ) || ((hdmi_rx_ctrl_read(0x09C))==(uint32_t)0x10000)) && (phy_status > 0)) { /* if RX PHY still doesn't recover let's reset it ! */

			if (hdmi_rx_phy_close(&rx.phy) < 0) {
				log_notice("RX PHY closing failed. Retrying...");
				if (hdmi_rx_phy_try_force_close(&rx.phy) >= 0)
				{
					log_notice("RX PHY: successfully closed");
					phy_status = 0;
				}
				hdmi_rx_ctrl_write(0xff0, ((1 << 3) - 1) << 1);
			}else
			{
				log_notice("RX PHY: successfully closed");
				phy_status = 0;
			}
			/* Turn off HPD - Hot PlugDetect output */
			//hdmi_rx_ctrl_write (0x0,0x0000C01);    /* TODO: replace this write by a function which will reset the HPD of the active port */
		}
	}
#endif
#if 1
	/* Check if rx is not "opened" and +5V is detected*/
	if (phy_status <= 0){
		os_sleep(100); /* Wait some time for the Hardware to reflect the +5 sensing */

		if (io_bus_read32((SETUP_APB_SFR_BASE + 0x07700), 0x00) & (1<<(rx.port))){ /* TODO: replace this io_read8 by a function which will sense the +5V */
			/* Turn on HPD - Hot PlugDetect outputs*/
			hdmi_rx_ctrl_write(0x0,0xF000C01);     /* TODO: replace this write by a function which will set the HPD of the active port */
			if (hdmi_rx_phy_open(&rx.phy) < 0) {
				log_notice("RX PHY: failed to open");
			}else
			{
				log_notice("RX PHY opened Successfully");
				hdmi_rx_phy_config(&rx.phy,27000,8);
				phy_status = 1;
			}
			//hdmi_rx_phy_fast_switching(&rx.phy, 1);
			//log_notice("Fast Switching enabled");
		}
		else
		{
			phy_status = -1; /* Not connected */
			/* reset controller HDMI clock domains only*/
			hdmi_rx_ctrl_write(0xff0,0x06);
		}
	}
#endif
	if (((hdmi_rx_ctrl_read(0x30)& (uint32_t)0x01)== 1) && (hdmi_rx_ctrl_read(0x09C)!= (uint32_t)0x10000))
	{
		phy_status = 2; /* PHY OK, receiving data*/
	}
	return phy_status;
}

int hdmi_demo_rx_check_video_mode(int report_change, unsigned *cea,	unsigned *dcm) {

	struct hdmi_rx_ctrl *ctx = &rx.ctrl;
	int error = 0;
	struct hdmi_rx_ctrl_video v = { 0 };

	error |= hdmi_rx_ctrl_get_video(ctx, &v);
	*cea = v.video_mode;
	*dcm = v.deep_color_mode;

	if (((video_params.hactive + 5) < (v.hactive)) || ((video_params.hactive
			!= 0) && ((video_params.hactive - 5) > (v.hactive)))
			|| ((video_params.vactive + 5) < (v.vactive))
			|| ((video_params.vactive != 0) && ((video_params.vactive - 5)
					> (v.vactive))) || (video_params.pixel_repetition
							!= v.pixel_repetition) || (video_params.deep_color_mode
									!= v.deep_color_mode) || ((video_params.refresh_rate + 3)
											< (v.refresh_rate)) || ((video_params.refresh_rate != 0)
													&& ((video_params.refresh_rate - 3) > (v.refresh_rate)))
													|| (video_params.dvi != v.dvi) || (video_params.video_mode
															!= v.video_mode) || (video_params.interlaced != v.interlaced))

	{
		error = 1;

		if (video_params.video_mode != v.video_mode) {
			//
			//			log_notice("buffer CEA. struct value: %d",v.video_mode);
			if (v.video_mode != 0) {
				error = 1; // cea mode changed
			} else {
				error = -1;
			}
		}
		if (video_params.deep_color_mode != v.deep_color_mode) {
			//			log_notice("buffer dcm: %d\n",v.deep_color_mode);
			if (v.deep_color_mode != 0) {
				error = 2; // dcm changed
			} else {
				error = -2;
			}

			printf("2.CEA FROM FCTN: %d\n", *cea);
			printf("2.DCM FROM FCTN: %d\n", *dcm);
			printf("2.VIDEO MODE: %d\n", v.video_mode);
			printf("2.DCM: %d\n", v.deep_color_mode);
		}
	}
	if (report_change == 1 && error > 0) {
		ctx->log_info("Video: %ux%u%c@%u.%02uHz: %s, DCM %ub, VIC:%u",
				v.hactive, v.vactive, v.interlaced ? 'i' : 'p', v.refresh_rate
						/ 100, v.refresh_rate % 100, v.dvi ? "DVI" : "HDMI",
								v.deep_color_mode, v.video_mode);

		//			rx.change = true;
		video_params.dvi = v.dvi;
		video_params.pixel_clk = v.pixel_clk;
		video_params.refresh_rate = v.refresh_rate;
		video_params.interlaced = v.interlaced;
		video_params.video_mode = v.video_mode;
		video_params.deep_color_mode = v.deep_color_mode;
		video_params.htotal = v.htotal;
		video_params.vtotal = v.vtotal;
		video_params.hactive = v.hactive;
		video_params.vactive = v.vactive;
		video_params.pixel_repetition = v.pixel_repetition;
	}

	*cea = v.video_mode;
	*dcm = v.deep_color_mode;

	return error;
}

void hdmi_rx_change_dcm(unsigned dcm)
{
	switch (dcm)
	{
	case 8:
		// configure RX PHY PLL
		phy_wrapper_write(0x03,0x900);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
		// video pll config 1:1 M=10 / N = 10
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1145);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x9145);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0002);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0118);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21D1);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x00);

		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0004);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0158);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21DF);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
		break;
	case 10:
		// configure RX PHY PLL
		phy_wrapper_write(0x03,0x920);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
		// video pll config 1:1.25 M=16 / N =20
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1208);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x128a);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0000);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x1148);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x2111);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);

		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0004);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0158);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21DF);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
		break;
	case 12:
		// configure RX PHY PLL
		phy_wrapper_write(0x03,0x940);
		drp_reset(0xc9807400, 0x01);
		// video pll config 1:1.5 M = 2 / N =3
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1041);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x1042);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0080);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0398);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x2111);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x00);

		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0004);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0158);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21DF);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
		break;
	case 16:
		// configure RX PHY PLL
		phy_wrapper_write(0x03,0x960);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
		// video pll config 1:2 M = 5 / N = 10
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1083);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0080);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x11C8);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0080);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0318);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x2111);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x00);

		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x1104);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0004);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0158);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21DF);
		drp_write(RX_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);

		break;
	default:
		// configure RX PHY PLL
		phy_wrapper_write(0x03,0x900);
		drp_reset(VIDEO_PLL_BASE_ADDRESS, 0x01);
		// video pll config 1:1
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_1,0x1145);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKFBOUT_REG_2,0x0000);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_1,0x9145);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_CLKOUT0_REG_2,0x0002);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_1,0x0118);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIGFILT_REG_2,0x21D1);
		drp_write(VIDEO_PLL_BASE_ADDRESS,DRP_DIVLOCK,0x3001);
		drp_reset(0xc9807400, 0x00);
		break;
	}
}

void hdmi_rx_bridge_cmd()
{
	//commute vg to bridge
	io_bus_write32(VIDEO_MUX_ADDRESS,0x00, 0x0);

	// commute from bridge output to cea
	io_bus_write32(0xc4000000,0x200f,0x2);

	// commute audio on tx, 64FS
	io_bus_write32(HDMI_TX_CTRL_BASE + 0x3206, 0x00,0x4);
}

void hdmi_rx_set_volume(unsigned char volume)
{
	int volume_data;
	volume_data = volume*256+volume;

	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, (uint16_t) volume_data, 0x05, 0x48);
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, (uint16_t) volume_data, 0x06, 0x48);
	//xilinx_i2c_write(0x81620000, 0x48, 0x05, (uint16_t) volume_data);
	//xilinx_i2c_write(0x81620000, 0x48, 0x06, (uint16_t) volume_data);
}

void hdmi_demo_rx_new_edid_fill( unsigned char nrBlocks, unsigned char *newEdid)
{
	rx_edidNrBlocks_top = nrBlocks;
	int byteCounter;
	for(byteCounter = 0; byteCounter < ((int)(nrBlocks))*128; byteCounter++)
	{
		rx_rawEdid_top[byteCounter] = newEdid[byteCounter];
	}
}

void hdmi_demo_rx_print_edid()
{
	unsigned int i = 0;

	for( i = 0; i < 128*rx_edidNrBlocks_top; i++ )
	{
		printf(" | 0x%02x", rx_rawEdid_top[i]);
	}
}

/* TODO Migrate this fast switching function to the HDMI RX API */
void hdmi_rx_ctrl_fast_switching(unsigned new_port) {
	unsigned rx_phy_status;

	log_notice("PHY PORT Selected [1=a | 2=b | 3=c |4=d] : %d", new_port + 1);
	if (new_port != rx.port) {
		/******* start fast switching ****/
		//--		rx_phy_status = hdmi_demo_rx_check_phy_status();
		//--		rx_phy_status = 2;
		/* If the RX PHY is receiving video, let's mute the video output */
		//--		if (rx_phy_status == 2) {
		//--			api_AvMute(TRUE);
		//--			os_sleep(500); /* wait for Avmute to make effect */
		//--		}
		/* Disable HDCP */
		//--		hdmi_rx_ctrl_write(0xc0, 0x2);

		/* reset controller HDMI clock domain only*/
		//--		hdmi_rx_ctrl_write(0xff0, 0x06);

		/* Change HDMI RX Input Port */
		rx.port = new_port;
		hdmi_rx_ctrl_write(0x2c0, 0x40 + (unsigned) (rx.port << 2));
		/* Turn off HPD - all ports not selected, this ensures HDCP I2C of transmitters will restart*/
		hdmi_rx_ctrl_write(0x0, (0x0000C01 + (0x1000000 << (rx.port)))); /* TODO: replace this write by a function which will set the HPD of the active port */

		/* TODO use open and close functions instead. PHY register will be cleaned ...*/
		/*hdmi_rx_ctrl_phy_reset(&rx.ctrl, 1);*/
		/*hdmi_rx_ctrl_phy_reset(&rx.ctrl, 0);*/
		//--		os_sleep(350); /* wait for 7 frames at 24Hz*/

		/* Turn on HPD - all the ports*/
		hdmi_rx_ctrl_write(0x0, 0xF000C01); /* TODO: replace this write by a function which will set the HPD of the active port */

		//--	rx_phy_status = hdmi_demo_rx_check_phy_status();
		//--		rx_phy_status = 2;

		/* If the RX PHY is still connected, let's enable HDCP */
		//--		if (rx_phy_status != -1) {
		//			/* Enable HDCP */
		//			hdmi_rx_ctrl_write(0xc0, 0x3);
		//			/* Reset HDMI RX CEA video output */
		//			hdmi_rx_ctrl_write(0x400, 0x80000000);
		//			hdmi_rx_ctrl_write(0x400, 0x0);
		//			api_AvMute(FALSE);
		//		}
	}
	//--	os_sleep(500); /* wait for system to get stable, very important for fast switching */
}
#if 0
void hdmi_demo_repeater() {
	int rx_phy_status;
	static unsigned cea_mode=1;
	static unsigned dcm_mode=8;
	unsigned change_detected = 0;
	unsigned tclk_pll_not_clocked = 0;
	unsigned dviMode = 0;
	unsigned bio_button=0;
	static unsigned char volume = 0;

	/****************** BIO BUTTON **********************/
	bio_button = bio_read_buttons();
	if (bio_button != 0) {
		switch (bio_button) {
		case 1: /* S1 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_A);
			break;

		case 2: /* S2 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_B);
			break;

		case 4: /* S3 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_C);
			break;

		case 8: /* S4 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_D);
			break;

		case 16: /* S5 button */
			if (volume <255) volume = volume + 1;
			hdmi_rx_set_volume(volume);
			break;
		case 32: /* S6 button */
			if (volume>0) volume = volume - 1;
			hdmi_rx_set_volume(volume);
			break;
		case 64: /* S7 button */
			hdmi_demo_repeater2();
			break;
		case 128: /* S8 button */
			hdmi_demo_repeater();
			break;
		default:
			log_notice("no function allocated to this button");
			break;
		}
	}
	/****************** BIO BUTTON **********************/

	/* Check if PHY is still working properly and reset it if required */
	rx_phy_status = hdmi_demo_rx_check_phy_status();
	if (rx_phy_status == 2) /* If RX PHY is receiving data and operating correctly */
	{
		api_AvMute(0);
		/* If Video PLL is locked, but Tx TCLK PLL is not locked, then force reconfigure HDMI TX*/
		if (io_read8(0xc9807408) & 0x1) {
			tclk_pll_not_clocked = ((io_read8(0xc9807508) == (uint8_t) 0x0));
		} else {
			tclk_pll_not_clocked = 0;
		}

		/* Check if there is a change of video mode, or there is a need to repgram the HDMI TX */
		if ((hdmi_demo_rx_check_video_mode(0, &cea_mode, &dcm_mode) > 0
				|| tclk_pll_not_clocked)) {
			os_sleep(350); // >7frames*24Hz. 4 for the mode recover block and 3 for CEA output block
			change_detected = hdmi_demo_rx_check_video_mode(1, &cea_mode, &dcm_mode);

			if (dviMode == 1) {
				dcm_mode = 24;
			}

			if (change_detected >= 1 || tclk_pll_not_clocked) {
				api_AvMute(1);

				// Apply CEA mode change to HDMI TX
				demo_SetParamsReconfigure(cea_mode, dviMode);

				// Call HDMI RX Bridge command to change clock routing for Video and Audio
				hdmi_rx_bridge_cmd();

				// DCM Rx
				log_notice("change rx dcm");
				hdmi_rx_change_dcm(dcm_mode / 3);
			}
		}
	}
}
#endif
#if 1
void hdmi_rx_demo()
{
	/* Configure Audio DAC and Audio PLL */
#ifdef SC_PLL
	hdmi_rx_set_volume(0);
	hdmi_rx_refresh_audio_clk(&rx.ctrl);
	//hdmi_rx_set_volume(191);
	//hdmi_rx_set_volume(52);
#else
	hdmi_rx_set_volume(0);
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x1010, 0x02, 0x48);
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x0A0A, 0x08, 0x4B);
	//xilinx_i2c_write(0x81620000, 0x48, 0x02, 0x1010);
	//xilinx_i2c_write(0x81620000, 0x4B, 0x08, 0x0A0A);
	hdmi_rx_set_volume(191);
#endif
	// Audio fifo reset
	//hdmi_rx_ctrl_write (0x258,0x0c0f8);
	hdmi_rx_ctrl_write (0x260,0x021);
	hdmi_rx_ctrl_write(0x240, 0x10001);
	hdmi_rx_ctrl_write(0x240, 0x10000);
	//hdmi_rx_ctrl_write (0x258,0x0c0d8);
	hdmi_rx_ctrl_write (0x260,0x001);
	hdmi_rx_set_volume(52);
}
void hdmi_rx_demo_fifores()
{
#ifdef SC_PLL
	hdmi_rx_set_volume(191);
	hdmi_rx_refresh_audio_clk(&rx.ctrl);
	sleep(200);
	//Reset Audio FIFO after change in sample frequency
	//////////////////////////////
	hdmi_rx_ctrl_write (0x260,0x021);
	sleep(200);
	hdmi_rx_ctrl_write(0x240, 0x10001);
	hdmi_rx_ctrl_write(0x240, 0x10000);
	sleep(200);
	hdmi_rx_ctrl_write (0x260,0x001);
	hdmi_rx_set_volume(52);
#else


	//#ifdef SC_PLL
	//	hdmi_rx_refresh_audio_clk(&rx.ctrl);
	//#endif
	// Audio fifo reset
	//hdmi_rx_ctrl_write (0x258,0x0c0f8);
	hdmi_rx_ctrl_write (0x260,0x021);
	hdmi_rx_ctrl_write(0x240, 0x10001);
	hdmi_rx_ctrl_write(0x240, 0x10000);
	//hdmi_rx_ctrl_write (0x258,0x0c0d8);
	hdmi_rx_ctrl_write (0x260,0x001);
#endif
}
void hdmi_demo_repeater_init()
{
	const u8 vName[] = "Synopsys";
	const u8 pName[] = "HDMI Tx";
	dtd_t tmp_dtd;
	LOG_MENUSTR("HDMI Tx demo: ", __DATE__);
	demo_Start();

	Rx_pProduct = (productParams_t*)malloc(sizeof(productParams_t));
	Rx_pAudio = (audioParams_t*)malloc(sizeof(audioParams_t));
	Rx_pVideo = (videoParams_t*)malloc(sizeof(videoParams_t));
	//Rx_pHdcp = (hdcpParams_t*)malloc(sizeof(hdcpParams_t));
	productParams_Reset(Rx_pProduct);
	productParams_SetVendorName(Rx_pProduct, vName, sizeof(vName) - 1);
	productParams_SetProductName(Rx_pProduct, pName, sizeof(pName) - 1);
	productParams_SetSourceType(Rx_pProduct, 0x0A);
	audioParams_Reset(Rx_pAudio);
	audioParams_SetInterfaceType(Rx_pAudio, I2S);
	audioParams_SetClockFsFactor(Rx_pAudio, 128);
	videoParams_Reset(Rx_pVideo);
	videoParams_SetHdmi(Rx_pVideo,1);
	dtd_Fill(&tmp_dtd, 1, 60000);
	videoParams_SetDtd(Rx_pVideo, &tmp_dtd);
	//hdcpParams_Reset(Rx_pHdcp);
	sleep(200);
	api_Configure(Rx_pVideo, Rx_pAudio, Rx_pProduct, Rx_pHdcp, hdmitx_phy);

	/* Configure Audio DAC and Audio PLL */
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x1010, 0x02, 0x48);
	Snps_I2cWrite(AUDIO_IIC_BASE_ADDRESS, 0x0A0A, 0x08, 0x4B);
	//xilinx_i2c_write(0x81620000, 0x48, 0x02, 0x1010);
	//xilinx_i2c_write(0x81620000, 0x4B, 0x08, 0x0A0A);
	hdmi_rx_set_volume(127);
	// Audio fifo reset
	//hdmi_rx_ctrl_write (0x258,0x0c0f8);
	hdmi_rx_ctrl_write (0x260,0x021);
	hdmi_rx_ctrl_write(0x240, 0x10001);
	hdmi_rx_ctrl_write(0x240, 0x10000);
	//hdmi_rx_ctrl_write (0x258,0x0c0d8);
	hdmi_rx_ctrl_write (0x260,0x001);

	/* Commute all muxes from Tx-only mode to Repeater mode */
	hdmi_rx_bridge_cmd();
	repeater_enabled = 1;
}
void hdmi_demo_repeater_manual_update()
{
	if(repeater_enabled == 1)
	{
		repeater_enabled = 2;
	}
	else
	{
		if(repeater_enabled == 2)
		{
			repeater_enabled = 1;
		}
	}
}
void hdmi_demo_repeater_cmd()
{
	// Initialize HDMI RX Controller and HDMI RX PHY
	hdmi_demo_repeater_init();
	sprintf(hdmi_rx_buffer[0], "hdmirx");
	sprintf(hdmi_rx_buffer[1], "clock");
	sprintf(hdmi_rx_buffer[2], "tmdsok");
	hdmi_rx_buffer_no = 3;
	hdmi_rx_callback(0, (void*) hdmi_rx_buffer, (void*) &hdmi_rx_buffer_no, (void*)&rx.ctrl);
	log_notice("Repeater source configured.");
}
void hdmi_rx_buttons_compliance()
{
	unsigned bio_button = 0;
	static unsigned char volume = 52;

	bio_button = bio_read_buttons();
	if (bio_button != 0) {
		switch (bio_button) {
		case 1: /* S1 button */
			log_notice("zoom");
			video_if_update_zoom(1);
			video_if_update_position();
			break;

		case 2: /* S2 button */
			log_notice("zoom");
			video_if_update_zoom(0);
			video_if_update_position();
			break;

		case 4: /* S3 button */
			log_notice("panx");
			video_if_update_panx(1);
			video_if_update_position();
			break;

		case 8: /* S4 button */
			log_notice("panx");
			video_if_update_panx(0);
			video_if_update_position();
			break;

		case 16: /* S5 button */
			if(volume <= 249)
			{
				volume = volume + 5;
				hdmi_rx_set_volume(volume);
				log_notice("Volume - (%d)", volume);
			}
			break;
		case 32: /* S6 button */
			if(volume > 5)
			{
				volume = volume - 5;
				hdmi_rx_set_volume(volume);
				log_notice("Volume + (%d)", volume);
			}
			break;
		case 64: /* S7 button */
			log_notice("pany");
			video_if_update_pany(1);
			video_if_update_position();
			break;
		case 128: /* S8 button */
			log_notice("pany");
			video_if_update_pany(0);
			video_if_update_position();
			break;
		default:
			log_notice("no function allocated to this button");
			break;
		}
	}
}
void hdmi_rx_buttons_ro()
{
	unsigned bio_button = 0;
	static unsigned char volume = 52;
	static unsigned char hdmi_input_fast_sw = HDMI_INPUT_PORT_A;
	static unsigned brightness_value = 0;

	/****************** BIO BUTTON **********************/
	bio_button = bio_read_buttons();
	if (bio_button != 0) {
		switch (bio_button) {
		case 1: /* S1 button */
#ifdef RX_COMPLIANCE
			update_demo = 1;
			log_notice("update demo enabled.");
#else
			log_notice("Bio buttons: brigthness +");
			//hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_A);
			if(brightness_value>=MAX_BRIGHT)
			{
				brightness_value=MIN_BRIGHT;
			}
			else
			{
				brightness_value+=2;
			}
			cam_aptina_change_brightness(brightness_value);
#endif
			break;

		case 2: /* S2 button */
			//hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_B);
#ifdef RX_COMPLIANCE
			update_demo = 0;
			log_notice("update demo disabled.");
#else

			log_notice("Bio buttons: brigthness -");
			if(brightness_value<=MIN_BRIGHT)
			{
				brightness_value=MAX_BRIGHT;
			}
			else
			{
				brightness_value-=2;
			}
			cam_aptina_change_brightness(brightness_value);
#endif
			break;

		case 4: /* S3 button */
			log_notice("Bio buttons: fast switching +");
			if(hdmi_input_fast_sw>=HDMI_INPUT_PORT_D)
			{
				hdmi_input_fast_sw=HDMI_INPUT_PORT_A;
			}
			else
			{
				hdmi_input_fast_sw++;
			}
			hdmi_rx_ctrl_fast_switching(hdmi_input_fast_sw);
			//hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_C);
			break;

		case 8: /* S4 button */
			log_notice("Bio buttons: fast switching -");
			if(hdmi_input_fast_sw<=HDMI_INPUT_PORT_A)
			{
				hdmi_input_fast_sw=HDMI_INPUT_PORT_D;
			}
			else
			{
				hdmi_input_fast_sw--;
			}
			hdmi_rx_ctrl_fast_switching(hdmi_input_fast_sw);
			//hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_D);
			break;

		case 16: /* S5 button */
			log_notice("Interrupts on");
			disable_interrupt(9);
			break;
		case 32: /* S6 button */
			log_notice("Interrupts off");
			disable_interrupt(9);
			break;
		case 64: /* S7 button */
			log_notice("Bio buttons: CEA path reset");
			//ceares
			hdmi_rx_ctrl_write (0x400,0x80000000);
			hdmi_rx_ctrl_write (0x400,0x0);
			log_notice("Rx Ctrl CEA Output Reseted");
			//fifores
			hdmi_rx_ctrl_write (0x260,0x021);
			sleep(200);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			sleep(200);
			hdmi_rx_ctrl_write (0x260,0x001);
			break;
		case 128: /* S8 button */
			log_notice("Bio buttons: HDMI Repeater mode enabled");
			repeater_enabled = 1;
			sleep(5);
			break;
		default:
			log_notice("no function allocated to this button");
			break;
		}
	}
}
void hdmi_rx_buttons()
{
	unsigned bio_button = 0;
	static unsigned char volume = 127;

	/****************** BIO BUTTON **********************/
	bio_button = bio_read_buttons();
	if (bio_button != 0) {
		switch (bio_button) {
		case 1: /* S1 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_A);
			break;

		case 2: /* S2 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_B);
			break;

		case 4: /* S3 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_C);
			break;

		case 8: /* S4 button */
			hdmi_rx_ctrl_fast_switching(HDMI_INPUT_PORT_D);
			break;

		case 16: /* S5 button */
			if(volume <= 249)
			{
				volume = volume + 5;
				hdmi_rx_set_volume(volume);
			}
			break;
		case 32: /* S6 button */
			if(volume > 5)
			{
				volume = volume - 5;
				hdmi_rx_set_volume(volume);
			}
			break;
		case 64: /* S7 button */
			//ceares
			hdmi_rx_ctrl_write (0x400,0x80000000);
			hdmi_rx_ctrl_write (0x400,0x0);
			log_notice("Rx Ctrl CEA Output Reseted");
			//fifores
			hdmi_rx_ctrl_write (0x260,0x021);
			sleep(200);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			sleep(200);
			hdmi_rx_ctrl_write (0x260,0x001);
			break;
		case 128: /* S8 button */
			if(repeater_enabled == 0)
			{
				repeater_enabled = 1;
				demo_rx_enabled = 0;
				log_notice("HDMI Repeater enabled.");
			}
			if(repeater_enabled == 2)
			{
				hdmi_demo_repeater2();
				log_notice("HDMI Repeater Manual Update mode: DATA UPDATED.");
			}
			sleep(5);
			break;
		default:
			log_notice("no function allocated to this button");
			break;
		}
	}
	/****************** BIO BUTTON **********************/
}

void hdmi_demo_repeater2() {
	int rx_phy_status;
	unsigned tclk_pll_not_locked = 0;
	//	unsigned bio_button = 0;
	//	static unsigned char volume = 127;
	//static int count_no_locks = 0;

	hdmi_rx_buttons_ro();
	/* Check if PHY is still working properly and reset it if required */
	rx_phy_status = hdmi_demo_rx_check_phy_status();
	if (rx_phy_status == 2) /* If RX PHY is receiving data and operating correctly */
	{
		/* TODO*/ // correction of behaviour when rx->tx ok ( avoid count_no_lock = 6 )
		api_AvMute(FALSE);
		/* If Video PLL is locked, but Tx TCLK PLL is not locked, then force reconfigure HDMI TX*/
		if (io_read8(0xc9807408) & 0x1) {
			tclk_pll_not_locked = ((io_read8(0xc9807508) == (uint8_t) 0x0));
		} else {
			tclk_pll_not_locked = 0;
		}

		/* Check if there is a change of video or audio mode, or if there is a need to reprogram the HDMI TX */
		if ((hdmi_demo_rx_check_audio_video_mode() != 0 || tclk_pll_not_locked)) {

			api_AvMute(TRUE);
			os_sleep(350); // >7frames*24Hz. 4 for the mode recover block and 3 for CEA output block
			hdmi_demo_rx_check_audio_video_mode(); /* Update Rx configs with stable values */
			// Apply CEA mode change to HDMI TX
			Rx_pAudio->mInterfaceType = 0; // commute to i2s input
			api_Configure(Rx_pVideo, Rx_pAudio, Rx_pProduct, NULL, hdmitx_phy);
			sleep(150);
			// DCM Rx
			log_notice("Rx_pVideo->mColorResolution 0x%x", Rx_pVideo->mColorResolution);
			hdmi_rx_change_dcm(Rx_pVideo->mColorResolution);


			//Rx Ctrl - CEA output reset
			hdmi_rx_ctrl_write (0x400,0x80000000);
			hdmi_rx_ctrl_write (0x400,0x0);

			// Reconfigure Bridge muxes
			// TODO Check where muxes are overwritten, this bridge function should not be necessary
			hdmi_rx_bridge_cmd();
			// if Rx2DSI path is active, adjust zoom to fit screen
			//video_if_adjust_zoom();
			if((io_bus_read32(0xc4002003, 0) & 0x01) == 1)
			{
				video_if_adjust_zoom_autocenter();
			}
#ifdef SC_PLL
			hdmi_rx_refresh_audio_clk(&rx.ctrl);
			log_notice("SC_PLL refresh cmd applied.");
#endif
			// Audio fifo reset
			//hdmi_rx_ctrl_write (0x258,0x0c0f8);
			hdmi_rx_ctrl_write (0x260,0x021);
			sleep(200);
			hdmi_rx_ctrl_write(0x240, 0x10001);
			hdmi_rx_ctrl_write(0x240, 0x10000);
			//hdmi_rx_ctrl_write (0x258,0x0c0d8);
			hdmi_rx_ctrl_write (0x260,0x001);
			sleep(500);
		}

	}
}


int hdmi_demo_rx_check_audio_video_mode()
{

	struct hdmi_rx_ctrl *ctx = &rx.ctrl;
	int changes_detected = 0;
	struct hdmi_rx_ctrl_video v = { 0 };
	struct hdmi_rx_ctrl_audio a = { 0 };
	dtd_t tmp_dtd;
	unsigned ref_clk = 0;
	/*unsigned eval_time = 0;*/
	unsigned data = 0;
	unsigned Srate = 0;
	static unsigned N = 0;
	unsigned cts = 0;
	unsigned evaltime = 0;
	unsigned tclk = 0;

	hdmi_rx_ctrl_get_video(ctx, &v);
	hdmi_rx_ctrl_get_audio(&rx.ctrl, &a);

	if (((video_params.hactive + 5) < (v.hactive)) || ((video_params.hactive
			!= 0) && ((video_params.hactive - 5) > (v.hactive)))
			|| ((video_params.vactive + 5) < (v.vactive))
			|| ((video_params.vactive != 0) && ((video_params.vactive - 5)
					> (v.vactive))) || (video_params.pixel_repetition
							!= v.pixel_repetition) || (video_params.deep_color_mode
									!= v.deep_color_mode) || ((video_params.refresh_rate + 3)
											< (v.refresh_rate)) || ((video_params.refresh_rate != 0)
													&& ((video_params.refresh_rate - 3) > (v.refresh_rate)))
													|| (video_params.dvi != v.dvi) || (video_params.video_mode
															!= v.video_mode) || (video_params.interlaced != v.interlaced))
	{
		log_notice("Video Mode change detected");
		changes_detected = 1;
		hdmi_rx_ctrl_debug(ctx);

		video_params.dvi = v.dvi;
		video_params.pixel_clk = v.pixel_clk;
		video_params.refresh_rate = v.refresh_rate;
		video_params.interlaced = v.interlaced;
		video_params.video_mode = v.video_mode;
		video_params.deep_color_mode = v.deep_color_mode;
		video_params.htotal = v.htotal;
		video_params.vtotal = v.vtotal;
		video_params.hactive = v.hactive;
		video_params.vactive = v.vactive;
		video_params.pixel_repetition = v.pixel_repetition;
		video_params.video_format = v.video_format;
		dtd_Fill(&tmp_dtd, video_params.video_mode, board_SupportedRefreshRate(video_params.video_mode));
		videoParams_SetDtd(Rx_pVideo, &tmp_dtd);

		if (video_params.dvi == 1) {Rx_pVideo->mHdmi = 0;} else {Rx_pVideo->mHdmi = 1;}

		if (video_params.video_format == 0) // RGB
		{
			Rx_pVideo->mEncodingOut = RGB;
			Rx_pVideo->mEncodingIn = RGB;
			log_notice ("Encoding Input RGB");
		}
		else if (video_params.video_format == 1) // YCC422
		{
			Rx_pVideo->mEncodingOut = RGB;
			Rx_pVideo->mEncodingIn = RGB;
			log_notice ("Encoding Input YCC422");
		}
		else if (video_params.video_format == 2) // YCC444
		{
			Rx_pVideo->mEncodingOut = RGB;
			Rx_pVideo->mEncodingIn = RGB;
			log_notice ("Encoding Input YCC444");
		}
		else log_notice ("Encoding Input Unknown");

		Rx_pVideo->mColorResolution = (u8)video_params.deep_color_mode/3;
		Rx_pVideo->mPixelRepetitionFactor = video_params.pixel_repetition;
	}

	if(N != hdmi_rx_demo_get_bit(ctx->bsp_read(0x394), MSK(20, 0)))
	{
		changes_detected = 1;
		audio_params.channel_allocation = a.channel_allocation;
		audio_params.channel_count = a.channel_count;
		audio_params.coding_extension = a.coding_extension;
		audio_params.coding_type = a.coding_type;
		audio_params.down_mix_inhibit = a.down_mix_inhibit;
		audio_params.level_shift_value = a.level_shift_value;
		audio_params.sample_frequency = a.sample_frequency;
		audio_params.sample_size = a.sample_size;
		ref_clk = ctx->md_clk;
		evaltime = (ref_clk * 4095) / 158000;
		data = ctx->bsp_read(0x09C);
		tclk = ((ref_clk * hdmi_rx_demo_get_bit(data, MSK(16, 0))) / evaltime);
		log_notice("\ncalculated tclk: %d", tclk);
		N = hdmi_rx_demo_get_bit(ctx->bsp_read(0x394), MSK(20, 0));
		cts = hdmi_rx_demo_get_bit(ctx->bsp_read(0x390), MSK(20, 0));
		log_notice("  - N parameter:     %u", N);
		log_notice("  - CTS parameter:   %u", cts);
		Srate = 1000*((tclk * N)/(128 * cts));
		log_notice("calculated SAMPLING RATE: %d", Srate);
		if( Srate > 46000)
		{
			Srate = 48000;
		}
		else
		{
			if( Srate < 38000)
			{
				Srate = 32000;
			}
			else
			{
				Srate = 44100;
			}
		}
		audioParams_SetSamplingFrequency(Rx_pAudio, Srate);
		Rx_pAudio->mChannelAllocation = a.channel_allocation;
		Rx_pAudio->mSampleSize = a.sample_size;

		log_notice("new SAMPLING RATE applied: %d", Srate);
	}
	return changes_detected;
}
#if 1
int hdmi_rx_demo_pll(unsigned long tmds)
{
	int error = 0;
	log_notice("tmds w/ dcm = %d", tmds);
	if (tmds < 50000)
	{
		drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

		/* H62 M = 24, N = 24, phase 45 */
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x130c);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x130c);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0003);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x130c);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0003);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x130c);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0003);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9a98);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
		drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

		drp_reset(RX_PLL_BASE_ADDRESS, 0x00);
	}
	else
	{
		if(tmds < 100000)
		{
			drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

			/* H62 M = 12, N = 12, phase 0 */
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1186);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1186);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1186);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1186);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b98);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
			drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

			drp_reset(RX_PLL_BASE_ADDRESS, 0x00);

		}
		else
		{
			if(tmds < 200000)
			{
				drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

				/* H62 M = 6, N = 6, phase 0 */
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x10c3);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0000);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x10c3);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0000);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x10c3);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0000);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x10c3);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0000);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9b58);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

				drp_reset(RX_PLL_BASE_ADDRESS, 0x00);

			}
			else
			{
				drp_reset(RX_PLL_BASE_ADDRESS, 0x01);

				/* H62 M = 4, N = 4, phase 0 */
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_1, 0x1082);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKFBOUT_REG_2, 0x0080);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_1, 0x1082);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT0_REG_2, 0x0080);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_1, 0x1082);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT1_REG_2, 0x0080);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_1, 0x1082);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_CLKOUT2_REG_2, 0x0080);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_1, 0x9bd8);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIGFILT_REG_2, 0x180f);
				drp_write(RX_PLL_BASE_ADDRESS, DRP_DIVLOCK, 0x1041);

				drp_reset(RX_PLL_BASE_ADDRESS, 0x00);

			}

		}
	}
	return error;
}
#endif
int hdmi_demo_rx_only_check_audio_video_mode()
{

	struct hdmi_rx_ctrl *ctx = &rx.ctrl;
	int changes_detected = 0;
	struct hdmi_rx_ctrl_video v = { 0 };
	struct hdmi_rx_ctrl_audio a = { 0 };
	dtd_t tmp_dtd;
	unsigned ref_clk = 0;
	/*unsigned eval_time = 0;*/
	unsigned data = 0;
	unsigned Srate = 0;
	static unsigned N = 0;
	unsigned cts = 0;
	unsigned evaltime = 0;
	unsigned tclk = 0;

	hdmi_rx_ctrl_get_video(ctx, &v);
	hdmi_rx_ctrl_get_audio(&rx.ctrl, &a);

	if (((video_params.hactive + 5) < (v.hactive)) || ((video_params.hactive
			!= 0) && ((video_params.hactive - 5) > (v.hactive)))
			|| ((video_params.vactive + 5) < (v.vactive))
			|| ((video_params.vactive != 0) && ((video_params.vactive - 5)
					> (v.vactive))) || (video_params.pixel_repetition
							!= v.pixel_repetition) || (video_params.deep_color_mode
									!= v.deep_color_mode) || ((video_params.refresh_rate + 3)
											< (v.refresh_rate)) || ((video_params.refresh_rate != 0)
													&& ((video_params.refresh_rate - 3) > (v.refresh_rate)))
													|| (video_params.dvi != v.dvi) || (video_params.video_mode
															!= v.video_mode) || (video_params.interlaced != v.interlaced))
	{
		log_notice("Video Mode change detected");
		changes_detected = 1;
		hdmi_rx_ctrl_debug(ctx);

		video_params.dvi = v.dvi;
		video_params.pixel_clk = v.pixel_clk;
		video_params.refresh_rate = v.refresh_rate;
		video_params.interlaced = v.interlaced;
		video_params.video_mode = v.video_mode;
		video_params.deep_color_mode = v.deep_color_mode;
		video_params.htotal = v.htotal;
		video_params.vtotal = v.vtotal;
		video_params.hactive = v.hactive;
		video_params.vactive = v.vactive;
		video_params.pixel_repetition = v.pixel_repetition;
		video_params.video_format = v.video_format;
	}
	if(N != hdmi_rx_demo_get_bit(ctx->bsp_read(0x394), MSK(20, 0)))
	{
		changes_detected = 1;
		N = hdmi_rx_demo_get_bit(ctx->bsp_read(0x394), MSK(20, 0));
		//		cts = hdmi_rx_demo_get_bit(ctx->bsp_read(0x390), MSK(20, 0));
		//		log_notice("  - N parameter:     %u", N);
		//		log_notice("  - CTS parameter:   %u", cts);
		//		Srate = 1000*((tclk * N)/(128 * cts));
		//		log_notice("calculated SAMPLING RATE: %d", Srate);
		//		if( Srate > 46000)
		//		{
		//			Srate = 48000;
		//		}
		//		else
		//		{
		//			if( Srate < 38000)
		//			{
		//				Srate = 32000;
		//			}
		//			else
		//			{
		//				Srate = 44100;
		//			}
		//		}
		//		audioParams_SetSamplingFrequency(Rx_pAudio, Srate);
		//		Rx_pAudio->mChannelAllocation = a.channel_allocation;
		//		Rx_pAudio->mSampleSize = a.sample_size;
		//
		//		log_notice("new SAMPLING RATE applied: %d", Srate);
	}
	return changes_detected;
}

int hdmi_rx_demo_check_audio_video_change()
{
	static int N = 0;
	/*static int cts = 0;*/
	static int hactive = 0;

	if(hactive != hdmi_rx_ctrl_read(0x150))
	{
		hactive = hdmi_rx_ctrl_read(0x150);
		log_notice("HDMI RX Demo: video data change detected.");
		return 1;
	}
	if(N != hdmi_rx_demo_get_bit(hdmi_rx_ctrl_read(0x394), MSK(20, 0)))
	{
		N = hdmi_rx_demo_get_bit(hdmi_rx_ctrl_read(0x394), MSK(20, 0));
		//cts = hdmi_rx_demo_get_bit(ctx->bsp_read(0x390), MSK(20, 0));
		log_notice("HDMI RX Demo: audio data change detected.");
		return 1;
	}
	return 0;
}
void hdmi_rx_phy_equalizerManualSet(uint8_t value_ch0, uint8_t value_ch1, uint8_t value_ch2, uint8_t auto_equalize, uint32_t phy_version)
{
	/*	c 0x2c0 0x4d //pddq on
	phyrw 0x08 0x20 // enable manual equalizer
	phyrw 0x32 0x4447 // ch0
	phyrw 0x52 0x4447 // ch1
	phyrw 0x72 0x4447 // ch2
	c 0x2c0 0x4c */ // pddq 0ff

	io_bus_write32(HDMI_RX_CTRL_BASE + 0x2C0UL, 0x00, 0x004E);
	uint16_t tmp = 0;

	if(auto_equalize)
	{
		phy_wrapper_write(0x08, 0x00);
	}
	else
	{
		if(phy_version == 205)
		{
			log_notice("Equalizer Manual Override enabled for E205: 0x20");
			phy_wrapper_write(0x08, 0x20);
		}
		else
		{
			log_notice("Equalizer Manual Override enabled for E213 / other: 0x40");
			phy_wrapper_write(0x08, 0x40);
		}
		// for phy E205
		//phy_wrapper_write(0x08, 0x20);
		// for phy E213
		//phy_wrapper_write(0x08, 0x40);
		tmp = phy_wrapper_read(0x32);
		//log_notice(" read 0x32 = 0x%x", tmp);
		//log_notice(" wrote 0x32 = 0x%x", (tmp & 0xfff8) | (value & 0x7));
		phy_wrapper_write(0x32,(tmp & 0xfff8) | (value_ch0 & 0x7));
		tmp = phy_wrapper_read(0x52);
		//log_notice(" read 0x52 = 0x%x", tmp);
		//log_notice(" wrote 0x52 = 0x%x", (tmp & 0xfff8) | (value & 0x7));
		phy_wrapper_write(0x52,(tmp & 0xfff8) | (value_ch1 & 0x7));
		tmp = phy_wrapper_read(0x72);
		//log_notice(" read 0x52 = 0x%x", tmp);
		//log_notice(" wrote 0x72 = 0x%x", (tmp & 0xfff8) | (value & 0x7));
		phy_wrapper_write(0x72,(tmp & 0xfff8) | (value_ch2 & 0x7));
	}
	io_bus_write32(HDMI_RX_CTRL_BASE + 0x2C0UL, 0x00, 0x004C);
}
uint8_t hdmi_rx_phy_equalizerSwAutoset(uint32_t setTime, uint32_t phy_version)
{
	error_cnt_t errorCnt[8];
	eqResult_t eqResult[3];

	eqResult[0].error_ch_count = 0xffffffff;
	eqResult[1].error_ch_count = 0xffffffff;
	eqResult[3].error_ch_count = 0xffffffff;
	unsigned long int wait_time = 0;
	uint8_t cnt_tmp = 0;
	uint8_t cnt_time = 0;
	/* Get error counting for the 3 channels */
	for(cnt_tmp = 0; cnt_tmp < 8; cnt_tmp++)
	{
		log_notice("equalizerSwAutoSet: equalizer setting %d", cnt_tmp);
		hdmi_rx_scdc_errorCntConfig(0, 0);
		hdmi_rx_phy_equalizerManualSet(cnt_tmp, cnt_tmp, cnt_tmp, 0, phy_version);
		hdmi_rx_scdc_errorCntConfig(1, 0);
		wait_time = 0;
		for(cnt_time = 0; (cnt_time + 1) < setTime; )
		{
			while(wait_time < 1000000)
			{
				wait_time++;
			}
			cnt_time = cnt_time + 1;
		}
		errorCnt[cnt_tmp] = hdmi_rx_scdc_errorCnt(1,0);
	}
	/* Choose the lowest count for each channel */
	for(cnt_tmp = 0; cnt_tmp < 8; cnt_tmp++)
	{
		if(errorCnt[cnt_tmp].errorCh0 < eqResult[0].error_ch_count)
		{
			eqResult[0].error_ch_count = errorCnt[cnt_tmp].errorCh0;
			eqResult[0].ch_bestEqSetting = cnt_tmp;
		}
	}
	for(cnt_tmp = 0; cnt_tmp < 8; cnt_tmp++)
	{
		if(errorCnt[cnt_tmp].errorCh1 < eqResult[1].error_ch_count)
		{
			eqResult[1].error_ch_count = errorCnt[cnt_tmp].errorCh1;
			eqResult[1].ch_bestEqSetting = cnt_tmp;
		}
	}
	for(cnt_tmp = 0; cnt_tmp < 8; cnt_tmp++)
	{
		if(errorCnt[cnt_tmp].errorCh2 < eqResult[2].error_ch_count)
		{
			eqResult[2].error_ch_count = errorCnt[cnt_tmp].errorCh2;
			eqResult[2].ch_bestEqSetting = cnt_tmp;
		}
	}
	log_notice("For ch0 the best setting is %d", eqResult[0].ch_bestEqSetting);
	log_notice("For ch0 the error counting is %d", eqResult[0].error_ch_count);
	log_notice("For ch1 the best setting is %d", eqResult[1].ch_bestEqSetting);
	log_notice("For ch1 the error counting is %d", eqResult[1].error_ch_count);
	log_notice("For ch2 the best setting is %d", eqResult[2].ch_bestEqSetting);
	log_notice("For ch2 the error counting is %d", eqResult[2].error_ch_count);
	/* Voting for the best setting - for now using the lowest error count result of the 3 channel*/
	if((eqResult[0].ch_bestEqSetting == eqResult[1].ch_bestEqSetting) & (eqResult[1].ch_bestEqSetting == eqResult[2].ch_bestEqSetting)) // 3 channels with the same value
	{
		hdmi_rx_phy_equalizerManualSet(eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, 0, phy_version);
		log_notice("Equalizer setting applied: %d", eqResult[0].ch_bestEqSetting);
		return 1;
	}
	else
	{
		if(eqResult[0].ch_bestEqSetting == eqResult[1].ch_bestEqSetting)
		{
			hdmi_rx_phy_equalizerManualSet(eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, 0, phy_version);
			log_notice("Equalizer setting applied: %d", eqResult[0].ch_bestEqSetting);
			return 1;
		}
		else
		{
			if(eqResult[0].ch_bestEqSetting == eqResult[2].ch_bestEqSetting)
			{
				hdmi_rx_phy_equalizerManualSet(eqResult[2].ch_bestEqSetting, eqResult[2].ch_bestEqSetting, eqResult[2].ch_bestEqSetting, 0, phy_version);
				log_notice("Equalizer setting applied: %d", eqResult[2].ch_bestEqSetting);
				return 1;
			}
			else
			{
				if(eqResult[1].ch_bestEqSetting == eqResult[2].ch_bestEqSetting)
				{
					hdmi_rx_phy_equalizerManualSet(eqResult[1].ch_bestEqSetting, eqResult[1].ch_bestEqSetting, eqResult[1].ch_bestEqSetting, 0, phy_version);
					log_notice("Equalizer setting applied: %d", eqResult[1].ch_bestEqSetting);
					return 1;
				}
				else
				{
					log_notice("Voting failed, all channels have different optimal equalizer settings.");
					log_notice("Applying equalizer setting chosen for channel 0 = %d.", eqResult[0].ch_bestEqSetting);
					hdmi_rx_phy_equalizerManualSet(eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, eqResult[0].ch_bestEqSetting, 0, phy_version);
					return -1;
				}

			}
		}
	}

}
uint8_t hdmi_rx_phy_equalizerSw_Stage1(uint32_t setTime, uint32_t errorLimit, uint32_t errorCntEnabled, uint32_t phy_version)
{
	error_cnt_t errorCnt_tmp;
	log_notice("Stage 1:");
	/* Check if for auto equalization the setting found is error-free */
	//hdmi_rx_scdc_errorCntConfig(0, 0);
	hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(0, 0, 0);
	//hdmi_rx_scdc_errorCntConfig(1, 0);
	hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(1, errorCntEnabled, 0);
	os_sleep(setTime);
	errorCnt_tmp = hdmi_rx_scdc_errorCnt(1,0);

	if((io_bus_read32(HDMI_RX_CTRL_BASE + 0x0030UL, 0x00) & 0x0001UL) == 0 )
	{
		log_notice("TMDS Valid = 0");
		return 0;
	}
	else
	{
		log_notice("TMDS Valid = 1");
	}

	//	log_notice("STAGE 1: Error cnt ch0: %d", errorCnt.errorCh0);
	//	log_notice("STAGE 1: Error cnt ch1: %d", errorCnt.errorCh1);
	//	log_notice("STAGE 1: Error cnt ch2: %d", errorCnt.errorCh2);
	return (errorCnt_tmp.errorCh0 <= errorLimit)? ((errorCnt_tmp.errorCh1 <= errorLimit)? ((errorCnt_tmp.errorCh2 <= errorLimit)? 1 : 0) : 0) : 0;
}
uint8_t hdmi_rx_phy_equalizerSw_Stage2v2(uint32_t setTime, uint16_t useEqSettings, uint32_t errorCntEnabled, uint32_t phy_version)
{
	eqResult[0].error_ch_count = 0xffffffff;
	eqResult[1].error_ch_count = 0xffffffff;
	eqResult[2].error_ch_count = 0xffffffff;
	eqResult[0].ch_bestEqSetting = 0;
	eqResult[1].ch_bestEqSetting = 0;
	eqResult[2].ch_bestEqSetting = 0;
	if(useEqSettings == 0)
	{
		useEqSettings = 0xffff;
	}

	for(cnt_tmp = 7; cnt_tmp >= 0; cnt_tmp--)
	{
		if((useEqSettings >> cnt_tmp & 0x0001) == 1)
		{
			log_notice("STAGE2: equalizer setting %d", cnt_tmp);
			//hdmi_rx_scdc_errorCntConfig(0, 0);
			hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(0, 0, 0);
			hdmi_rx_phy_equalizerManualSet(cnt_tmp, cnt_tmp, cnt_tmp, 0, phy_version);
			//hdmi_rx_scdc_errorCntConfig(1, 0);
			hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(1, errorCntEnabled, 0);
			os_sleep(setTime);
			errorCnt[cnt_tmp] = hdmi_rx_scdc_errorCnt(0,0);
			if((io_bus_read32(HDMI_RX_CTRL_BASE + 0x0030UL, 0x00) & 0x0001UL) == 0 )
			{
				log_notice("TMDS Valid = 0");
				errorCnt[cnt_tmp].errorCh0 = 0xffffffff;
				errorCnt[cnt_tmp].errorCh1 = 0xffffffff;
				errorCnt[cnt_tmp].errorCh2 = 0xffffffff;
				log_notice("Ch0: 0x%x",errorCnt[cnt_tmp].errorCh0);
				log_notice("Ch1: 0x%x",errorCnt[cnt_tmp].errorCh1);
				log_notice("Ch2: 0x%x",errorCnt[cnt_tmp].errorCh2);
			}
			else
			{
				log_notice("TMDS Valid = 1");
				log_notice("Ch0: 0x%x",errorCnt[cnt_tmp].errorCh0);
				log_notice("Ch1: 0x%x",errorCnt[cnt_tmp].errorCh1);
				log_notice("Ch2: 0x%x",errorCnt[cnt_tmp].errorCh2);
				if(errorCnt[cnt_tmp].errorCh0 < eqResult[0].error_ch_count)
				{
					eqResult[0].error_ch_count = errorCnt[cnt_tmp].errorCh0;
					eqResult[0].ch_bestEqSetting = cnt_tmp;
				}
				if(errorCnt[cnt_tmp].errorCh1 < eqResult[1].error_ch_count)
				{
					eqResult[1].error_ch_count = errorCnt[cnt_tmp].errorCh1;
					eqResult[1].ch_bestEqSetting = cnt_tmp;
				}
				if(errorCnt[cnt_tmp].errorCh2 < eqResult[2].error_ch_count)
				{
					eqResult[2].error_ch_count = errorCnt[cnt_tmp].errorCh2;
					eqResult[2].ch_bestEqSetting = cnt_tmp;
				}
			}
			if(errorCnt[cnt_tmp].errorCh0 + errorCnt[cnt_tmp].errorCh1 + errorCnt[cnt_tmp].errorCh2 == 0)
			{
				hdmi_rx_phy_equalizerManualSet(cnt_tmp, cnt_tmp, cnt_tmp, 0, phy_version);
				log_notice(" STAGE2: Zero errors condition found for setting %d. Setting applied. Exiting...", cnt_tmp);
				//hdmi_rx_scdc_errorCntConfig(0, 0);
				hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(0, 0, 0);
				//cnt_stop = cnt_tmp;
				return 1;
			}
		}
	}
	log_notice(" STAGE2: Unable to find error-free setting for all channels.");
	log_notice(" STAGE2: Best setting for ch0 is %d", eqResult[0].ch_bestEqSetting);
	log_notice(" STAGE2: Best setting for ch1 is %d", eqResult[1].ch_bestEqSetting);
	log_notice(" STAGE2: Best setting for ch2 is %d", eqResult[2].ch_bestEqSetting);
	hdmi_rx_phy_equalizerManualSet(eqResult[0].ch_bestEqSetting, eqResult[1].ch_bestEqSetting, eqResult[2].ch_bestEqSetting, 0, phy_version);
	log_notice(" STAGE2: Settings applied. Exiting...");
	//hdmi_rx_scdc_errorCntConfig(0, 0);
	hdmi_rx_scdc_errorCntConfig_EnableSpecificCounter(0, 0, 0);
	return 0;
}

uint8_t hdmi_rx_phy_equalizerSw_Stage3v2(uint32_t setTime, uint32_t errorLimit, uint32_t phy_version)
{
	//	error_cnt_t errorCnt[8];
	error_cnt_t errorCntTmp[8];
	//	eqResult_t eqResult[3];

	eqResult[0].error_ch_count = 0xffffffff;
	eqResult[1].error_ch_count = 0xffffffff;
	eqResult[2].error_ch_count = 0xffffffff;
	/* Choose the lowest count for each channel */
	for(cnt_tmp = 7; cnt_tmp > 0; cnt_tmp--)
	{
		//	log_notice("cnt_tmp %d", cnt_tmp);
		if(errorCnt[cnt_tmp].errorCh0 < eqResult[0].error_ch_count)
		{
			eqResult[0].error_ch_count = errorCnt[cnt_tmp].errorCh0;
			eqResult[0].ch_bestEqSetting = cnt_tmp;
		}
	}
	//log_notice("cnt_tmp %d", cnt_tmp);
	for(cnt_tmp = 7; cnt_tmp > 0; cnt_tmp--)
	{
		//log_notice("cnt_tmp %d", cnt_tmp);
		if(errorCnt[cnt_tmp].errorCh1 < eqResult[1].error_ch_count)
		{
			eqResult[1].error_ch_count = errorCnt[cnt_tmp].errorCh1;
			eqResult[1].ch_bestEqSetting = cnt_tmp;
		}
	}
	//log_notice("cnt_tmp %d", cnt_tmp);
	for(cnt_tmp = 7; cnt_tmp > 0; cnt_tmp--)
	{
		//log_notice("cnt_tmp %d", cnt_tmp);
		if(errorCnt[cnt_tmp].errorCh2 < eqResult[2].error_ch_count)
		{
			eqResult[2].error_ch_count = errorCnt[cnt_tmp].errorCh2;
			eqResult[2].ch_bestEqSetting = cnt_tmp;
		}
	}
	hdmi_rx_scdc_errorCntConfig(0, 0);
	if(eqResult[0].error_ch_count > errorLimit)
	{
		log_notice("STAGE3: best error setting for ch0 contains error values above limit.");
	}
	if(eqResult[1].error_ch_count > errorLimit)
	{
		log_notice("STAGE3: best error setting for ch1 contains error values above limit.");
	}
	if(eqResult[2].error_ch_count > errorLimit)
	{
		log_notice("STAGE3: best error setting for ch2 contains error values above limit.");
	}

	log_notice("STAGE3 concluded.");
	log_notice("STAGE3: best setting for ch0 is %d", eqResult[0].ch_bestEqSetting);
	log_notice("STAGE3: best setting for ch1 is %d", eqResult[1].ch_bestEqSetting);
	log_notice("STAGE3: best setting for ch2 is %d", eqResult[2].ch_bestEqSetting);

	log_notice("For ch0 the error counting is %d", eqResult[0].error_ch_count);
	log_notice("For ch1 the error counting is %d", eqResult[1].error_ch_count);
	log_notice("For ch2 the error counting is %d", eqResult[2].error_ch_count);

	hdmi_rx_phy_equalizerManualSet(eqResult[0].ch_bestEqSetting, eqResult[1].ch_bestEqSetting, eqResult[2].ch_bestEqSetting, 0, phy_version);
	return 1;
}
uint8_t hdmi_rx_phy_equalizer_SwWA(uint32_t setTime, uint32_t useEqSettings, uint32_t errorCountersEnabled, uint32_t phy_version)
{
	if(hdmi_rx_phy_equalizerSw_Stage1(setTime, 0, errorCountersEnabled, phy_version) == 1)
	{
		log_notice("STAGE1: No errors found.");
		return 1;
	}
	else
	{
		log_notice("STAGE1: Errors found or TMDS valid = 0. Entering STAGE2.");
		if(hdmi_rx_phy_equalizerSw_Stage2v2(setTime, useEqSettings, errorCountersEnabled, phy_version) == 1)
		{
			//log_notice("STAGE2: Best setting found and applied. Exiting...");
			return 1;
		}
		return 0;
	}
}

#endif
