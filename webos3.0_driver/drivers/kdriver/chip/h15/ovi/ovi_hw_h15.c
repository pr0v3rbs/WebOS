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
 *	OVI module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include "os_util.h"

#include "ovi_dbg.h"
#include "ovi_hw_h15.h"
#include "ovi_reg_def_h15.h"
#include "ovi_reg_h15.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

#include "ovi_output_format_h15a0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define OVI_HW_H15_GAMMA_TABLE_SIZENUM 1024
#define OVI_HW_H15_WB_GAIN_STEP	26	// 0.26% step per w/b gain 1
#define OVI_HW_H15_WB_OFFSET_STEP 8	// 8 step per w/b offset 1
#define OVI_HW_H15_WB_DEFAULT_GAIN 16384

#define OVI_REG_WR(address,data)   \
	gMapped_address=(volatile UINT32 *)ioremap(address, sizeof(UINT32));\
	do{\
		if(gMapped_address == NULL) break;\
		*gMapped_address = data;\
		iounmap((void *)gMapped_address);\
	}while(0);\

#define OVI_REG_RD(address,data)   \
	gMapped_address = (volatile UINT32 *)ioremap(address, sizeof(UINT32));\
	do{\
		if(gMapped_address == NULL) {data = 0; break;}\
		data = (unsigned int)*gMapped_address;\
		iounmap((void *)gMapped_address);\
	}while(0);\

#define OVI_HW_H15_CH_POWER_REG_MASK	0x00FFF000

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chPowerOnRegValue;
} OVI_HW_H15_INFO_T;

typedef struct{
	LX_OVI_OUTPUT_MODE_T	*pOVIOutputModes;
	UINT32 number_of_modes;
} OVI_HW_H15_OUTPUT_MODES_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern LX_MEMCFG_T *gpOviTconMem;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
OVI_OIF_REG_H15_T gOVI_OIF_H15;
OVI_LED_REG_H15_T gOVI_LED_H15;

static unsigned int *gMapped_address;

LX_OVI_OUTPUT_MODE_T _gOVIOutputModes_H15Ax[] =
{
	{"Display settings for H15 bringup\n",  ovi_h15a0_debug_mode_h15_bringup, sizeof(ovi_h15a0_debug_mode_h15_bringup)/sizeof(LX_OVI_REG_T)}
};

OVI_HW_H15_OUTPUT_MODES_T _gOVIOutputModes_H15 =
{
	.pOVIOutputModes 	 = NULL,
	.number_of_modes	 = 0,
};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static UINT32 _OVI_H15_GetGain(UINT32 gain);
static SINT16 _OVI_H15_GetOffset(UINT32 offset);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _gPrevRedGain = OVI_HW_H15_WB_DEFAULT_GAIN;
static UINT32 _gPrevGreenGain = OVI_HW_H15_WB_DEFAULT_GAIN;
static UINT32 _gPrevBlueGain = OVI_HW_H15_WB_DEFAULT_GAIN;

static SINT16 _gPrevRedOffset = 0;
static SINT16 _gPrevGreenOffset = 0;
static SINT16 _gPrevBlueOffset = 0;

static OVI_HW_H15_INFO_T _gOviHwH15Info;

/*========================================================================================
	Implementation Group
========================================================================================*/
int OVI_HW_H15_Initialize(void)
{
	int ret = RET_OK;

	do {
		OVI_PRINT("OVI chip revision is set to H15 A0\n");
		/* Set configuration of Output mode presets, for mode change on run time */
		_gOVIOutputModes_H15.pOVIOutputModes = _gOVIOutputModes_H15Ax;
		_gOVIOutputModes_H15.number_of_modes	 = sizeof(_gOVIOutputModes_H15Ax)/sizeof(LX_OVI_OUTPUT_MODE_T);

		gOVI_OIF_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_OIF_REG_H15A0_T));
		gOVI_LED_H15.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_LED_REG_H15A0_T));

		gOVI_OIF_H15.phys.addr = (volatile UINT32 *)ioremap(OVI_OIF_REG_H15A0_BASE, sizeof(OVI_OIF_REG_H15A0_T));
		gOVI_LED_H15.phys.addr = (volatile UINT32 *)ioremap(OVI_LED_REG_H15A0_BASE, sizeof(OVI_LED_REG_H15A0_T));

		/* for copying value used in channel power on/off function */
		CTOP_CTRL_H15A0_RdFL(DPE, ctr39);
		_gOviHwH15Info.chPowerOnRegValue = CTOP_CTRL_H15A0_Rd(DPE, ctr39);
	} while(0);

	return ret;
}

int OVI_HW_H15_Close(void)
{
	int ret = RET_OK;

	if(gOVI_OIF_H15.shdw.addr)
	{
		OS_Free((void *)gOVI_OIF_H15.shdw.addr);
		gOVI_OIF_H15.shdw.addr = NULL;
	}
	if(gOVI_LED_H15.shdw.addr)
	{
		OS_Free((void *)gOVI_LED_H15.shdw.addr);
		gOVI_LED_H15.shdw.addr = NULL;
	}

	if (gOVI_OIF_H15.phys.addr)
	{
		iounmap((void *)gOVI_OIF_H15.phys.addr);
		gOVI_OIF_H15.phys.addr = NULL;
	}
	if (gOVI_LED_H15.phys.addr)
	{
		iounmap((void *)gOVI_LED_H15.phys.addr);
		gOVI_LED_H15.phys.addr = NULL;
	}

	return ret;
}

int OVI_HW_H15_Resume(void)
{
	int ret = RET_OK;

	do {

	} while(0);

	return ret;
}

int OVI_HW_H15_SetCombination(UINT32 param)
{
	int ret = RET_ERROR;
	int i,count;
	volatile LX_OVI_REG_T	*regs;
	char	*index;
	unsigned int read_data, same;

	do {
		/* Step 1. Check whether paramater is valid */
		if(param == 119) {	// Magic keyword to print available output modes...
			printk("\n[OVI] Display all register-set-presets ...\n");
			for(i=0; i<_gOVIOutputModes_H15.number_of_modes;i++)
			{
				printk("  Index[%d] : %s", i,_gOVIOutputModes_H15.pOVIOutputModes[i].index);
			}
			ret = RET_OK;
			break;
		}
		else if(param >= 10000)
		{
			param = param - 10000;
			printk("\n[OVI] Compare register values regarding to register-set index[%d]", param);

			if(param > _gOVIOutputModes_H15.number_of_modes) {
				OVI_ERROR("Unknown OVI index. Index was [%d]", param);
				break;
			}

			regs  = _gOVIOutputModes_H15.pOVIOutputModes[param].regs;
			count = _gOVIOutputModes_H15.pOVIOutputModes[param].count;
			index = _gOVIOutputModes_H15.pOVIOutputModes[param].index;

			printk("\n[OVI] Register Index[%d] => %s",param, (char *)index);
			for(i=0; i<count; i++){
				if (regs[i].addr == 0x00000001) continue;

				OVI_REG_RD(regs[i].addr, read_data);
				if(read_data == regs[i].value) same = 1;
				else same = 0;

				printk("\n[%03d] Current value of [0x%08x] => [0x%08x] / index value [0x%08x] (%s)", i, regs[i].addr, read_data, regs[i].value, same ? "SAME":"DIFFER");
			}
			printk("\n[OVI] Register read complete!\n");

			ret = RET_OK;
			break;
		}
		else if(param > _gOVIOutputModes_H15.number_of_modes) {
			OVI_ERROR("\nUnknown OVI mode. Index was [%d]\n", param);
			break;
		}

		/* Step 2. Set mode */
		regs  = _gOVIOutputModes_H15.pOVIOutputModes[param].regs;
		count = _gOVIOutputModes_H15.pOVIOutputModes[param].count;
		index = _gOVIOutputModes_H15.pOVIOutputModes[param].index;

		printk("\n[OVI] Setting to registers to register-set-index => %s", (char *)index);
		for(i=0; i<count; i++){
			if (regs[i].addr == 0x00000001)
			{
				printk("\n[%03d] Delay... %dms", i, regs[i].value/1000);
				OS_UsecDelay(regs[i].value);
			}
			printk("\n[%03d] Writing... [0x%08x] <= [0x%08x]", i, regs[i].addr, regs[i].value);
			OVI_REG_WR(regs[i].addr, regs[i].value);
		}
		printk("\n[OVI] Setting preset complete!\n");

		ret = RET_OK;
	} while(0);

	return ret;
}

int OVI_HW_H15_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams)
{
	int ret = RET_OK;

#if 0 // temp
	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		//select chip type
		CTOP_CTRL_H15A0_RdFL(LEFT, ctr93);

		if(pstParams->bEnable)
		{
			if(pstParams->u16Period == LX_OVI_SPREAD_SPECTRUM_DEFAULT_PERIOD)
			{
				switch(pstParams->eSpreadRatio)
				{
					case LX_OVI_SPREAD_RATIO_1 :
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x5);
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x8);
					break;
					case LX_OVI_SPREAD_RATIO_2 :
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x6);
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x6);
					break;
					case LX_OVI_SPREAD_RATIO_4 :
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x9);
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x4);
					break;
					case LX_OVI_SPREAD_RATIO_3 :
					default :
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x8);
						CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x5);
					break;
				}
			}
			else
			{
				CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x8);
				CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x5);
			}
		}
		else
		{
			CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x0);
			CTOP_CTRL_H15A0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x0);
		}

		CTOP_CTRL_H15A0_WrFL(LEFT, ctr93);
	} while(0);
#endif

	return ret;
}

int OVI_HW_H15_SetColorTempEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_LED_H15_RdFL(led_wb_en);
		if(bParam)
		{
			OVI_LED_H15_Wr01(led_wb_en, wb_en0, 0x1);
			OVI_LED_H15_Wr01(led_wb_en, wb_mode, 0x1);
		}
		else
		{
			OVI_LED_H15_Wr01(led_wb_en, wb_en0, 0x0);
		}
		OVI_LED_H15_WrFL(led_wb_en);
	} while (0);

	return ret;
}

int OVI_HW_H15_SetGammaEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_LED_H15_RdFL(led_dpg_en);
		if(bParam)
			OVI_LED_H15_Wr01(led_dpg_en, dpg_en, 0x1);
		else
			OVI_LED_H15_Wr01(led_dpg_en, dpg_en, 0x0);
		OVI_LED_H15_WrFL(led_dpg_en);
	} while (0);

	return ret;
}

static UINT32 _OVI_H15_GetGain(UINT32 gain)
{
	UINT32 ret_gain;
	UINT32 temp;

	temp = 10000 + (gain - 192) * OVI_HW_H15_WB_GAIN_STEP;
	ret_gain = (temp * OVI_HW_H15_WB_DEFAULT_GAIN) / 10000;

	return ret_gain;
}

static SINT16 _OVI_H15_GetOffset(UINT32 offset)
{
	SINT16 ret_offset;

	ret_offset = ((SINT32)offset - 64) * OVI_HW_H15_WB_OFFSET_STEP;
	return ret_offset;
}

int OVI_HW_H15_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_gain, g_gain, b_gain;
	SINT16 rOffset, gOffset, bOffset;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		// W/B enable
		OVI_LED_H15_RdFL(led_wb_en);
		OVI_LED_H15_Wr01(led_wb_en, wb_en0, 0x1);
		OVI_LED_H15_Wr01(led_wb_en, wb_mode, 0x1);
		OVI_LED_H15_WrFL(led_wb_en);

		r_gain = _OVI_H15_GetGain(pstParams->r_gain);
		g_gain = _OVI_H15_GetGain(pstParams->g_gain);
		b_gain = _OVI_H15_GetGain(pstParams->b_gain);

		rOffset = _OVI_H15_GetOffset(pstParams->r_offset);
		gOffset = _OVI_H15_GetOffset(pstParams->g_offset);
		bOffset = _OVI_H15_GetOffset(pstParams->b_offset);

		if((_gPrevRedGain == r_gain) && (_gPrevGreenGain == g_gain) && (_gPrevBlueGain == b_gain)
			&& (_gPrevRedOffset == rOffset) && (_gPrevGreenOffset == gOffset) && (_gPrevBlueOffset == bOffset))
		{
			return RET_OK;
		}

		// Set red gain
		OVI_LED_H15_RdFL(led_wb_r0_r1);
		OVI_LED_H15_RdFL(led_wb_r2_r3);
		OVI_LED_H15_RdFL(led_wb_r4_r5);
		OVI_LED_H15_RdFL(led_wb_r6_r7);
		OVI_LED_H15_RdFL(led_wb_r8_r9);
		OVI_LED_H15_Wr(led_wb_r0_r1, (r_gain << 16) | r_gain);
		OVI_LED_H15_Wr(led_wb_r2_r3, (r_gain << 16) | r_gain);
		OVI_LED_H15_Wr(led_wb_r4_r5, (r_gain << 16) | r_gain);
		OVI_LED_H15_Wr(led_wb_r6_r7, (r_gain << 16) | r_gain);
		OVI_LED_H15_Wr(led_wb_r8_r9, (r_gain << 16) | r_gain);
		OVI_LED_H15_WrFL(led_wb_r0_r1);
		OVI_LED_H15_WrFL(led_wb_r2_r3);
		OVI_LED_H15_WrFL(led_wb_r4_r5);
		OVI_LED_H15_WrFL(led_wb_r6_r7);
		OVI_LED_H15_WrFL(led_wb_r8_r9);

		// Set red offset
		OVI_LED_H15_RdFL(led_wb_r_offset);
		OVI_LED_H15_Wr01(led_wb_r_offset, r_offset, rOffset);
		OVI_LED_H15_WrFL(led_wb_r_offset);

		// Set green gain
		OVI_LED_H15_RdFL(led_wb_g0_g1);
		OVI_LED_H15_RdFL(led_wb_g2_g3);
		OVI_LED_H15_RdFL(led_wb_g4_g5);
		OVI_LED_H15_RdFL(led_wb_g6_g7);
		OVI_LED_H15_RdFL(led_wb_g8_g9);
		OVI_LED_H15_Wr(led_wb_g0_g1, (g_gain << 16) | g_gain);
		OVI_LED_H15_Wr(led_wb_g2_g3, (g_gain << 16) | g_gain);
		OVI_LED_H15_Wr(led_wb_g4_g5, (g_gain << 16) | g_gain);
		OVI_LED_H15_Wr(led_wb_g6_g7, (g_gain << 16) | g_gain);
		OVI_LED_H15_Wr(led_wb_g8_g9, (g_gain << 16) | g_gain);
		OVI_LED_H15_WrFL(led_wb_g0_g1);
		OVI_LED_H15_WrFL(led_wb_g2_g3);
		OVI_LED_H15_WrFL(led_wb_g4_g5);
		OVI_LED_H15_WrFL(led_wb_g6_g7);
		OVI_LED_H15_WrFL(led_wb_g8_g9);

		// Set green offset
		OVI_LED_H15_RdFL(led_wb_g_offset);
		OVI_LED_H15_Wr01(led_wb_g_offset, g_offset, gOffset);
		OVI_LED_H15_WrFL(led_wb_g_offset);

		// Set blue gain
		OVI_LED_H15_RdFL(led_wb_b0_b1);
		OVI_LED_H15_RdFL(led_wb_b2_b3);
		OVI_LED_H15_RdFL(led_wb_b4_b5);
		OVI_LED_H15_RdFL(led_wb_b6_b7);
		OVI_LED_H15_RdFL(led_wb_b8_b9);
		OVI_LED_H15_Wr(led_wb_b0_b1, (b_gain << 16) | b_gain);
		OVI_LED_H15_Wr(led_wb_b2_b3, (b_gain << 16) | b_gain);
		OVI_LED_H15_Wr(led_wb_b4_b5, (b_gain << 16) | b_gain);
		OVI_LED_H15_Wr(led_wb_b6_b7, (b_gain << 16) | b_gain);
		OVI_LED_H15_Wr(led_wb_b8_b9, (b_gain << 16) | b_gain);
		OVI_LED_H15_WrFL(led_wb_b0_b1);
		OVI_LED_H15_WrFL(led_wb_b2_b3);
		OVI_LED_H15_WrFL(led_wb_b4_b5);
		OVI_LED_H15_WrFL(led_wb_b6_b7);
		OVI_LED_H15_WrFL(led_wb_b8_b9);

		// Set blue offset
		OVI_LED_H15_RdFL(led_wb_b_offset);
		OVI_LED_H15_Wr01(led_wb_b_offset, b_offset, bOffset);
		OVI_LED_H15_WrFL(led_wb_b_offset);

		_gPrevRedGain = r_gain;
		_gPrevGreenGain = g_gain;
		_gPrevBlueGain = b_gain;
		_gPrevRedOffset = rOffset;
		_gPrevGreenOffset = gOffset;
		_gPrevBlueOffset = bOffset;

	} while(0);

	return ret;
}

int OVI_HW_H15_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams)
{
	int ret = RET_OK;
	UINT32 *rGammaTable = NULL;
	UINT32 *gGammaTable = NULL;
	UINT32 *bGammaTable = NULL;
	int i;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		if((pstParams->rGammaLut.sizeNum != OVI_HW_H15_GAMMA_TABLE_SIZENUM)
		|| (pstParams->gGammaLut.sizeNum != OVI_HW_H15_GAMMA_TABLE_SIZENUM)
		|| (pstParams->bGammaLut.sizeNum != OVI_HW_H15_GAMMA_TABLE_SIZENUM))
		{
			OVI_ERROR("[OVI] Gamma table size is not correct! \n");
			ret = RET_ERROR;
			break;
		}

		rGammaTable = OS_Malloc(OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		gGammaTable = OS_Malloc(OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		bGammaTable = OS_Malloc(OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		if(rGammaTable == NULL || gGammaTable == NULL || bGammaTable == NULL)
		{
			OVI_ERROR("[OVI] memory allocate failed! \n");
			ret = RET_ERROR;
			break;
		}

		ret = copy_from_user(rGammaTable, (void __user *)pstParams->rGammaLut.pData, OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		ret |= copy_from_user(gGammaTable, (void __user *)pstParams->gGammaLut.pData, OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		ret |= copy_from_user(bGammaTable, (void __user *)pstParams->bGammaLut.pData, OVI_HW_H15_GAMMA_TABLE_SIZENUM*4);
		if(ret)
		{
			OVI_ERROR("[OVI] Error in copying from user! \n");
			ret = RET_ERROR;
			break;
		}

		// Gamma Table download
		OVI_LED_H15_RdFL(led_dpg_direct_acc);
		OVI_LED_H15_Wr01(led_dpg_direct_acc, dpg_direct_acc, 0x1);
		OVI_LED_H15_WrFL(led_dpg_direct_acc);

		OVI_LED_H15_RdFL(led_dpg_lut_wdata);

		/* Setting Red Gamma Table */
		for(i=0; i<OVI_HW_H15_GAMMA_TABLE_SIZENUM; i++)
		{
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_we, 0x4);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_waddr, i);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_wdata, (rGammaTable[i] << 2) & 0xFFF);
			OVI_LED_H15_WrFL(led_dpg_lut_wdata);
		}
		/* Setting Green Gamma Table */
		for(i=0; i<OVI_HW_H15_GAMMA_TABLE_SIZENUM; i++)
		{
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_we, 0x2);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_waddr, i);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_wdata, (gGammaTable[i] << 2) & 0xFFF);
			OVI_LED_H15_WrFL(led_dpg_lut_wdata);
		}
		/* Setting Blue Gamma Table */
		for(i=0; i<OVI_HW_H15_GAMMA_TABLE_SIZENUM; i++)
		{
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_we, 0x1);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_waddr, i);
			OVI_LED_H15_Wr01(led_dpg_lut_wdata, dpg_wdata, (bGammaTable[i] << 2) & 0xFFF);
			OVI_LED_H15_WrFL(led_dpg_lut_wdata);
		}

		OVI_LED_H15_RdFL(led_dpg_lut_wr_done);
		OVI_LED_H15_Wr01(led_dpg_lut_wr_done, dpg_lut_wr_done, 0x1);
		OVI_LED_H15_WrFL(led_dpg_lut_wr_done);

		// Gamma Enable
		OVI_LED_H15_RdFL(led_dpg_en);
		OVI_LED_H15_Wr01(led_dpg_en, dpg_en, 0x1);
		OVI_LED_H15_WrFL(led_dpg_en);
	} while(0);

	if(rGammaTable != NULL)
		OS_Free(rGammaTable);
	if(gGammaTable != NULL)
		OS_Free(gGammaTable);
	if(bGammaTable != NULL)
		OS_Free(bGammaTable);

	return ret;
}

int OVI_HW_H15_SetLvdsReverse(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H15_RdFL(oif_vch_config);
		if(bParam) {
			OVI_OIF_H15_Wr01(oif_vch_config, vch_h_reverse, 0x1);
			OVI_OIF_H15_Wr01(oif_vch_config, vch_line_delay, 0x1);
		}
		else {
			OVI_OIF_H15_Wr01(oif_vch_config, vch_h_reverse, 0x0);
			OVI_OIF_H15_Wr01(oif_vch_config, vch_line_delay, 0x0);
		}
		OVI_OIF_H15_WrFL(oif_vch_config);
	} while(0);

	return ret;
}

int OVI_HW_H15_SetChannelPower(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		CTOP_CTRL_H15A0_RdFL(DPE, ctr39);
		if(!bParam && ((CTOP_CTRL_H15A0_Rd(DPE, ctr39) & OVI_HW_H15_CH_POWER_REG_MASK) != 0x0)) // display output off
		{
			CTOP_CTRL_H15A0_Wr(DPE, ctr39, _gOviHwH15Info.chPowerOnRegValue & (~OVI_HW_H15_CH_POWER_REG_MASK));
			OVI_DEBUG("[OVI] display is changed to OFF status!\n");
		}
		else if(bParam && ((CTOP_CTRL_H15A0_Rd(DPE, ctr39) & OVI_HW_H15_CH_POWER_REG_MASK) == 0x0))// display output on
		{
			CTOP_CTRL_H15A0_Wr(DPE, ctr39, _gOviHwH15Info.chPowerOnRegValue);
			OVI_DEBUG("[OVI] display is changed to ON status!\n");
		}
		else
		{
			OVI_DEBUG("[OVI] channel power status is not changed - same as current status!\n");
		}
		CTOP_CTRL_H15A0_WrFL(DPE, ctr39);
	} while(0);

	return ret;
}

int OVI_HW_H15_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H15_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_LVDS_OUT_JEIDA:
				OVI_OIF_H15_Wr01(oif_lvds_config0, lvds_lsb_first, 0x0);
				break;
			case LX_OVI_LVDS_OUT_VESA:
				OVI_OIF_H15_Wr01(oif_lvds_config0, lvds_lsb_first, 0x1);
				break;
			default:
				break;
		}
		OVI_OIF_H15_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_H15_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H15_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_8BIT:
				OVI_OIF_H15_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x1);
				break;
			case LX_OVI_10BIT:
				OVI_OIF_H15_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x0);
				break;
			default:
				break;
		}
		OVI_OIF_H15_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_H15_SetLvdsBlack(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H15_RdFL(oif_lvds_config1);
		OVI_OIF_H15_RdFL(oif_lvds_config2);
		OVI_OIF_H15_RdFL(oif_vx1_config3);
		OVI_OIF_H15_RdFL(oif_vx1_config4);

		if(!bParam) // original data out
		{
			OVI_OIF_H15_Wr01(oif_lvds_config1, lvds_force_even_data, 0x0);
			OVI_OIF_H15_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x0);

			OVI_OIF_H15_Wr01(oif_vx1_config3, vx1_force_even_data, 0x0);
			OVI_OIF_H15_Wr01(oif_vx1_config4, vx1_force_odd_data, 0x0);
		}
		else // black data out
		{
			OVI_OIF_H15_Wr01(oif_lvds_config1, lvds_force_even_data, 0x40000000);
			OVI_OIF_H15_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x40000000);

			OVI_OIF_H15_Wr01(oif_vx1_config3, vx1_force_even_data, 0x40000000);
			OVI_OIF_H15_Wr01(oif_vx1_config4, vx1_force_odd_data, 0x40000000);
		}
		OVI_OIF_H15_WrFL(oif_lvds_config1);
		OVI_OIF_H15_WrFL(oif_lvds_config2);
		OVI_OIF_H15_WrFL(oif_vx1_config3);
		OVI_OIF_H15_WrFL(oif_vx1_config4);
	} while(0);

	return ret;
}

int OVI_HW_H15_DisplayResume(LX_OVI_DISPLAY_INFO_T *pstParams)
{
	int ret = RET_OK;
	int i;
	int count;
	const LX_OVI_REG_T *regs;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( H15, A1))
		{
			regs = ovi_h15a1_debug_mode_h15_bringup;
			count = sizeof(ovi_h15a1_debug_mode_h15_bringup)/sizeof(LX_OVI_REG_T);
		}
		else
		{
			regs = ovi_h15a0_debug_mode_h15_bringup;
			count = sizeof(ovi_h15a0_debug_mode_h15_bringup)/sizeof(LX_OVI_REG_T);
		}

		for(i=0; i<count; i++)
		{
			if (regs[i].addr == 0x00000001)
			{
				OS_UsecDelay(regs[i].value);
			}
			else
			{
				OVI_REG_WR(regs[i].addr, regs[i].value);
			}
		}
	} while(0);

	return ret;
}

/** @} */
