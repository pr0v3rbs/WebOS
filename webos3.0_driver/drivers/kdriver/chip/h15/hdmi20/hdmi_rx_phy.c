/**
 * @file hdmi_rx_phy.c
 * @brief HDMI RX PHY driver
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */
#include "hdmi_rx_phy.h"

/** File name */
static const char *file = "hdmi_rx_phy";
/*
 * Miscellaneous
 */
/** Configuration clock minimum [kHz] */
#define CFG_CLK_MIN				(10000UL)
/** Configuration clock maximum [kHz] */
#define CFG_CLK_MAX				(160000UL)
/** TMDS clock minimum [kHz] */
#define TMDS_CLK_MIN			(25000UL)
/** TMDS clock maximum [kHz] */
#define TMDS_CLK_MAX			(340000UL)
/** PLL setting when less than 40500kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_40500kHz	(0x0400UL)
/** PLL setting when less than 74500kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_74500kHz	(0x0410UL)
/** PLL setting when less than 111375kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_111375kHz	(0x0A00UL)
/** PLL setting when less than 148500kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_148500kHz	(0x0E00UL)
/** PLL setting when less than 222750kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_222750kHz	(0x0E40UL)
/** PLL setting when less than 297000kHz */
#define GEN2_TCHIP_PLL_LESS_THAN_297000kHz	(0x1660UL)
/** PLL setting when more than 297000kHz */
#define GEN2_TCHIP_PLL_MORE_THAN_297000kHz	(0x2200UL)
/*
 * PHY testchip registers
 */
/** Register address: peaking configuration */
#define GEN2_TCHIP_RA_PEAKING				(0x04UL)
/** Peaking, HDMIRX_PHY_TSMC_65LP_2_5V E203 default configuration */
#define 	GEN2_TCHIP_PEAKING_CFG_E203		(0x3B88UL)
/** Peaking, HDMIRX_PHY_TSMC_65GP_2_5V - E201 default configuration */
#define 	GEN2_TCHIP_PEAKING_CFG_E201		(0x2228UL)
/** Register address: PHY PLL configuration */
#define GEN2_TCHIP_RA_PHY_PLL				(0x10UL)
/** Register address: clock good flag */
#define GEN2_TCHIP_RA_CLK_GOOD				(0x1CUL)
/** Clock good flag, default configuration */
#define GEN2_TCHIP_CLK_GOOD_CFG			(0xB0D8UL)
/** Register address: internal timers configuration */
#define GEN2_TCHIP_RA_TIMERS				(0x1DUL)
/** Register address: input port configuration */
#define GEN2_TCHIP_RA_INPUT_PORT			(0x1FUL)
/** Register address: latch enable flag */
#define GEN2_TCHIP_RA_LATCH_EN				(0xFFUL)

//#else // w/out TCHIP
/** PLL range when less than 40500kHz */
#define GEN2_PLL_LESS_THAN_40500kHz	(40500UL)
/** PLL range when less than 74500kHz */
#define GEN2_PLL_LESS_THAN_74500kHz	(74500UL)
/** PLL range when less than 111375kHz */
#define GEN2_PLL_LESS_THAN_111375kHz	(111375UL)
/** PLL range when less than 148500kHz */
#define GEN2_PLL_LESS_THAN_148500kHz	(148500UL)
/** PLL range when less than 222750kHz */
#define GEN2_PLL_LESS_THAN_222750kHz	(222750UL)
/** PLL range when less than 297000kHz */
#define GEN2_PLL_LESS_THAN_297000kHz	(297000UL)
/** PLL range when more than 297000kHz */
#define GEN2_PLL_MORE_THAN_297000kHz	(340000UL)

/*
 * PHY registers within the controller
 */
/** HPD and PHY target state control */
#define GEN2_REG_HDMI_SETUP_CTRL		(0x000UL)
/** PHY override configurations */
#define GEN2_REG_HDMI_OVR_CTRL		(0x004UL)
/** PHY register to store the number of CFG_CLK cycles
 * that occur within a 4-µs period. */
#define GEN2_REG_HDMI_TIMER_CTRL		(0x008UL)
/** PHY configurations override */
#define GEN2_REG_HDMI_RES_OVR		(0x010UL)
/** PHY PLL general configurations */
#define GEN2_REG_HDMI_PLL_CTRL		(0x018UL)
/** PLL sub ranges 1 */
#define GEN2_REG_HDMI_PLL_FRQSET1 	(0x01CUL)
/** PLL sub ranges 2 */
#define GEN2_REG_HDMI_PLL_FRQSET2	(0x020UL)
/** PLL coefficients (Integral & Proportional Charge Pumps) 1 */
#define GEN2_REG_HDMI_PLL_PAR1		(0x024UL)
/** PLL coefficients (Integral & Proportional Charge Pumps) 2 */
#define GEN2_REG_HDMI_PLL_PAR2		(0x028UL)
/** PLL coefficients (Oscillator gains and CCO Low Pass filter) */
#define GEN2_REG_HDMI_PLL_PAR3		(0x02CUL)
/** hysteresis of clock input and active edge selection of PLL clock
 * to clock out data to DWC_hdmi_rx */
#define GEN2_REG_HDMI_CLK_CTRL		(0x034UL)
/** clock inversions and input selection */
#define GEN2_REG_HDMI_PCB_CTRL		(0x038UL)
/** Equalizer offset register */
#define GEN2_REG_HDMI_EQOFF_CTRL		(0x04CUL)
/** Equalizer gain control */
#define GEN2_REG_HDMI_EQGAIN_CTRL	(0x050UL)
/**  HDMI frequency change detection time period */
#define GEN2_REG_HDMI_CKM_EVLTM 		(0x094UL)
/** default values according to PHY macro */
/*HDMIRX_PHY_TSMC_65GP_2_5V*/ /* E201 */
#define E201_EQ_RSV_DEFAULT			(0x11UL)
#define E201_INV_ALIGNCLOCK_DEFAULT	(0x1UL)


/*HDMIRX_PHY_TSMC_40LP_2_5V*/ /* E202 */
#define E202_EQ_RSV_DEFAULT			(0x31UL)
#define E202_INV_ALIGNCLOCK_DEFAULT	(0x3UL)

/*HDMIRX_PHY_TSMC_65LP_2_5V*/ /* E203 */
#define E203_EQ_RSV_DEFAULT			(0x1CUL)
#define E203_INV_ALIGNCLOCK_DEFAULT	(0x1UL)

/* HDMIRX_PHY_TSMC_55LP_2_5V*/ /* E206 */
#define E206_EQ_RSV_DEFAULT			(0x11UL)
#define E206_INV_ALIGNCLOCK_DEFAULT	(0x1UL)

/* HDMIRX_PHY_TSMC_55GP_2_5V*/ /* E207 */
#define E207_EQ_RSV_DEFAULT			(0x1CUL)
#define E207_INV_ALIGNCLOCK_DEFAULT	(0x1UL)

/* PLL configuration values */
/* HDMIRX_PHY_TSMC_55GP_2_5V*/ /* E207 */
#define E207_HDMI_PLL_PAR1_DEFAULT		(0xFCFBFF69UL)
#define E207_HDMI_PLL_PAR2_DEFAULT		(0x6F6DAFBFUL)
#define E207_HDMI_PLL_PAR3_DEFAULT		(0x7EDCA998UL)
/*all other SNPS PHYs - E201, E202, E203 and E206 */
#define E201_E206_HDMI_PLL_PAR1_DEFAULT		(0xF8F8FBC6UL)
#define E201_E206_HDMI_PLL_PAR2_DEFAULT		(0x6A6AABBBUL)
#define E201_E206_HDMI_PLL_PAR3_DEFAULT		(0x76DCA998UL)

/* common default values for PHYs */
#define GEN2_HDMI_PLL_FRQSET1_DEFAULT 	(0x72523121UL)
#define GEN2_HDMI_PLL_FRQSET2_DEFAULT	(0x40D48A93UL)
#define GEN2_HDMI_SETUP_CTRL_DEFAULT		(0x03000C1BUL)
#define GEN2_HDMI_CLK_CTRL_DEFAULT		(0x10UL)

/** register address: PHY Gen3 clock measurement unit configuration */
#define GEN3_REG_HDMI_PHY_CMU_CONFIG			(0x02UL)
/** register address: PHY Gen3 system configuration */
#define GEN3_REG_HDMI_PHY_SYSTEM_CONFIG		(0x03UL)
/** register address: PHY Gen3 main FSM status 1 */
#define GEN3_REG_HDMI_PHY_MAINFSM_STATUS1	(0x09UL)
/** register address: PHY Gen3 DAC control */
#define GEN3_REG_HDMI_PHY_DAC_CTRL 			(0x12UL)
/** register address: PHY Gen3 Clock Channel control */
#define GEN3_REG_HDMI_PHY_CLK_CH_CTRL 		(0x20UL)
/** register address: PHY Gen3 Analog Test Bus Selection */
#define GEN3_REG_HDMI_PHY_SUP_ATB_SELECT 	(0x21UL)
/** register address: PHY Gen3 Analog Test Bus Selection and
 * Slicer/Driver Voltage Threshold */
#define GEN3_REG_HDMI_PHY_REC_VOLTAGE_LEVEL 	(0x22UL)
/** MPLL Transfer Function Parameters 1 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS1	(0x26UL)
/** MPLL Transfer Function Parameters 2 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS2	(0x27UL)
/** MPLL Transfer Function Parameters 3 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS3	(0x28UL)
/** MPLL Transfer Function Parameters 4 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS4	(0x29UL)
/** MPLL Transfer Function Parameters 5 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS5	(0x2AUL)
/** MPLL Transfer Function Parameters 6 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS6	(0x2BUL)
/** MPLL Transfer Function Parameters 7 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS7	(0x2CUL)
/** MPLL Transfer Function Parameters 8 */
#define GEN3_REG_HDMI_PHY_MPLL_PARAMETERS8	(0x2DUL)
/** register address: PHY Gen3 Channel 0 Analog Test Bus Control 1 */
#define GEN3_REG_HDMI_PHY_CH0_ATB_CTRL1 		(0x3AUL)
/** register address: PHY Gen3 Channel 1 Analog Test Bus Control 1 */
#define GEN3_REG_HDMI_PHY_CH1_ATB_CTRL1 		(0x5AUL)
/** register address: PHY Gen3 Channel 2 Analog Test Bus Control 1 */
#define GEN3_REG_HDMI_PHY_CH2_ATB_CTRL1 		(0x7AUL)
/** register overload protections **/
#define GEN3_REG_HDMI_PHY_OVL_PROT_CTRL      (0x0DUL)
/** PHY Gen3 Clock measurement lock threshold - default 8*/
#define GEN3_LOCK_THRES						(0x08UL)
/* active write wait to the zcal done */
#define MIN_WRITE_WAIT					(50000)

/* ***************************************************************************
 * Interface and configuration
 * ***************************************************************************/
/**
 * Open device validating context information
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_phy_open(struct hdmi_rx_phy *ctx)
{
	int error = 0;
	unsigned tmp = 0;
	//ctx->log_info("RX Phy: %d", ctx->version);
	if (ctx == 0 || ctx->bsp_reset == 0
			|| ctx->log_error == 0	|| ctx->log_info == 0
			|| ctx->bsp_read == 0 || ctx->bsp_write == 0
			|| ctx->cfg_clk < CFG_CLK_MIN || ctx->cfg_clk > CFG_CLK_MAX)
	{
		ctx->log_info("PHY OPEN: error -EINVAL");
		return -EINVAL;
	}
	if (ctx->status != 0)
	{	/* is not closed? */
		ctx->log_info("PHY OPEN: error -EPERM already open.");
		return -EPERM;
	}
	ctx->status = -1;
	ctx->bsp_reset(true);
	ctx->bsp_pddq(1);
	ctx->bsp_svsmode(1);
	ctx->bsp_zcal_reset(1);
	/* Check zcal done assertion */
	for (tmp = 0; (tmp < MIN_WRITE_WAIT) && (ctx->bsp_zcal_done() != 0); tmp++)
	{
		;
	}
	if (ctx->bsp_zcal_done() == 0)
	{
		ctx->bsp_zcal_reset(0);
		ctx->bsp_reset(false);
		/* write timebase override and override enable */
		ctx->bsp_write(GEN3_REG_HDMI_PHY_CMU_CONFIG, (GEN3_LOCK_THRES << 10) | (1 << 9) | (((1 << 9) - 1) & ((ctx->cfg_clk * 4) / 1000)));
		if(ctx->version != 213 || ctx->version != 204)
		{
			/* set internal DAC to generate internal reference voltages */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_SUP_ATB_SELECT, 0x2000);
			/* GENETARED VOLTAGE 1.25V */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_DAC_CTRL, 0x19FB);
			/* Enables the bias generator for the loopback drivers */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_CLK_CH_CTRL, 0x1);
			/* Disable equalizer calibration for TMDS frequencies between 25MHz - 46.25MHz*/
			ctx->bsp_write(GEN3_REG_HDMI_PHY_SYSTEM_CONFIG, 0x00);
		}
		if(ctx->version == 204)
		{
			/* configure the MPLL parameters (proportional and integral charge pumps) */
			/* values obtained from characterization */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS2, 0x1294);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS3, 0x3712);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS4, 0x26E2);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS5, 0x34DC);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS6, 0x44E4);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS7, 0x1960);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS8, 0x476C);
		}
		if(ctx->version == 213)
		{
			/* HDMIRX_PHY_TSMC_28HPM_1_8V*/ /*E213*/
			/* configure the MPLL parameters (proportional and integral charge pumps) */
			/* values obtained from characterization */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS2, 0x0094);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS3, 0x0000);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS4, 0x0000);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS5, 0x2000);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS6, 0x240A);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS7, 0x1281);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS8, 0x0048);
		}
		if(ctx->version == 211 || ctx->version == 205)
		{
			/* HDMIRX_PHY_SMIC_40LL_2_5V*/ /*E211*/
			/* configure the MPLL parameters (proportional and integral charge pumps) */
			/* values obtained from characterization */
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS2, 0x1394);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS3, 0x24CA);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS4, 0x1291);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS5, 0x324A);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS6, 0x0002);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS7, 0x0100);
			ctx->bsp_write(GEN3_REG_HDMI_PHY_MPLL_PARAMETERS8, 0x0000);
		}
		if(ctx->generation == 3)
		{
			if((ctx->version != 213) || (ctx->version != 204))
			{
				ctx->bsp_write(GEN3_REG_HDMI_PHY_OVL_PROT_CTRL,0x2);
			}
			/* power up */
			ctx->bsp_pddq(0);
			if((ctx->version != 213) || (ctx->version != 204))
			{
				ctx->bsp_write(GEN3_REG_HDMI_PHY_OVL_PROT_CTRL,0x0);
			}
		}
		//ctx->log_info("PHY CFG");
	}
	else
	{
		error = -EHOSTDOWN;
	}
	return error;
}

/**
 * Configure device setting internal timers and PLL
 * @param[in,out] ctx context information
 * @param[in] tclk TMDS clock frequency [kHz]
 * @param[in] dcm deep color mode (bits per pixel)
 * @return error code
 */
int hdmi_rx_phy_config(struct hdmi_rx_phy *ctx, unsigned long tclk, unsigned dcm)
{
	int error = 0;

	ctx->log_info("RX Phy Cfg: %d", ctx->version);
	if (ctx == 0 || tclk < TMDS_CLK_MIN || tclk > TMDS_CLK_MAX)
	{
		return -EINVAL;
	}
	if (ctx->status == 0) 
	{	/* is closed? */
		return -EPERM;
	}
	ctx->status = -1;
	//ctx->bsp_pddq(1);
	switch (dcm)
	{
	case 24:
		dcm = 0;
		break;
	case 30:
		dcm = 1;
		break;
	case 36:
		dcm = 2;
		break;
	case 48:
		dcm = 3;
		break;
	default:
		dcm = 0;
		break;
	}
	/* force port select, Disable Equalization calibration at lowest rate */
	//	ctx->bsp_write(GEN3_REG_HDMI_PHY_SYSTEM_CONFIG, (dcm << 5));
	//	error = -ctx->bsp_config(tclk);
	//	ctx->bsp_pddq(0);

	ctx->status = 1;
	return error;
}

int hdmi_rx_phy_config_and_reset(struct hdmi_rx_phy *ctx, unsigned port, unsigned long tclk, unsigned dcm)
{
	int error = 0;
	uint32_t pll = 0;
	uint32_t evaltime = 0;
	ctx->log_info("RX Phy Cfg and Reset to Tclk[%d],DCM[%d]",tclk, dcm  );

	if (ctx == 0 || tclk < TMDS_CLK_MIN || tclk > TMDS_CLK_MAX)
	{
		return -EINVAL;
	}
	if (ctx->status == 0)
	{	/* is closed? */
		return -EPERM;
	}
	ctx->status = -1;
	if(ctx->generation < 3)
	{
		/* PHY_GEN3 */
		/* for cfg_clk at 158MHz, evaltime is 4095. It has a linear relationship */
		//evaltime = (ctx->cfg_clk * 4095) / 158000;
		evaltime = (ctx->cfg_clk * 4095) / ctx->cfg_clk;
		pll = tclk * evaltime / ctx->cfg_clk;
		/* configuration clock does not change */
		/* reconfigure only if PLL ranges have changed */
		if (ctx->pll_cfg != pll)
		{
			ctx->pll_cfg = pll;
#ifdef TESTCHIP
			ctx->bsp_reset(true);
			/* write number of sys_clks in 4us  */
			/* refer to test chip documentation */
			ctx->bsp_write(GEN2_TCHIP_RA_TIMERS, (((4 * ctx->cfg_clk) / 1000) << 1));
			ctx->bsp_write(GEN2_TCHIP_RA_LATCH_EN, 0);					/* disarm */
			ctx->bsp_write(GEN2_TCHIP_RA_PHY_PLL, pll);
			ctx->bsp_write(GEN2_TCHIP_RA_INPUT_PORT, port);
			ctx->bsp_write(GEN2_TCHIP_RA_CLK_GOOD, GEN2_TCHIP_CLK_GOOD_CFG);		/* assert */
			ctx->bsp_write(GEN2_TCHIP_RA_PEAKING, ctx->peaking);
			ctx->bsp_write(GEN2_TCHIP_RA_LATCH_EN, 1);					/* re-arm */
			ctx->bsp_reset(false);
#else
			ctx->peaking = 0;
			ctx->pll_cfg = 0;
			ctx->status = -1;
#endif
		}
	} // if !GEN3 Phy
	else
	{
		ctx->bsp_svsmode(1);
		ctx->bsp_reset(false);
		ctx->bsp_pddq(1);
		switch (dcm)
		{
		case 8:
			dcm = 0;
			break;
		case 10:
			dcm = 1;
			break;
		case 12:
			dcm = 2;
			break;
		case 16:
			dcm = 3;
			break;
		default:
			dcm = 0;
			break;
		}
		if( ctx->generation == 3)
		{
			/* force port select, Disable Equalization calibration at lowest rate */
			ctx->log_info( "PHY CONFIG FOR NEW DCM");
			ctx->bsp_write(GEN3_REG_HDMI_PHY_SYSTEM_CONFIG, (dcm << 5));
			//ctx->bsp_write(REG_HDMI_PHY_SYSTEM_CONFIG, 0x110);
			error = -ctx->bsp_config(tclk);
			ctx->bsp_svsmode(1);
			ctx->bsp_reset(false);
			ctx->bsp_pddq(0);
		}
	}
	ctx->status = 1;
	return error;
}

/**
 * Debug device
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_phy_debug(struct hdmi_rx_phy *ctx)
{
	int error = 0;
	ctx->log_info("Test debug PHY");
	if (ctx == 0) 
	{
		return -EINVAL;
	}
	if (ctx->status <= 0) 
	{	/* is not configured? */
		return -EPERM;
	}
	if(ctx->generation == 3)
	{
		ctx->pll_cfg = ctx->bsp_read(GEN3_REG_HDMI_PHY_MAINFSM_STATUS1);
	}
	ctx->log_info("%s", file);
	ctx->log_info("  Config clock: %lukHz", ctx->cfg_clk);
	ctx->log_info("  PLL configuration: %04X", ctx->pll_cfg);
	return error;
}

/**
 * Fast switching control
 * @param[in,out] ctx context information
 * @param[in] enable or disable fast switching
 */
void hdmi_rx_phy_fast_switching(struct hdmi_rx_phy *ctx, int enable)
{
	ctx->bsp_write(GEN3_REG_HDMI_PHY_SYSTEM_CONFIG, ctx->bsp_read(GEN3_REG_HDMI_PHY_SYSTEM_CONFIG) | ((enable & 1) << 11));
}

/**
 * Close device
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_phy_close(struct hdmi_rx_phy *ctx)
{
	int error = 0;

	if (ctx == 0) 
	{
		return -EINVAL;
	}
	if (ctx->status == 0) 
	{		/* is closed? */
		return -EPERM;
	}
	ctx->status = 0;
	ctx->bsp_reset(true);
	if(ctx->generation == 3)
	{
		ctx->bsp_pddq(1);
	}
	return error;
}
/**
 * Close device
 * @param[in,out] ctx context information
 * @return error code
 */
int hdmi_rx_phy_try_force_close(struct hdmi_rx_phy *ctx)
{
	int error = 0;

	if (ctx == 0)
	{
		return -EINVAL;
	}
	ctx->status = 0;
	ctx->bsp_reset(true);
	if(ctx->generation == 3)
	{
		ctx->bsp_pddq(1);
	}
	return error;
}
