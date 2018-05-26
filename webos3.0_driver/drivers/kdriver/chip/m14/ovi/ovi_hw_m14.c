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
#include "ovi_hw_m14.h"
#include "ovi_reg_def_m14.h"
#include "ovi_reg_m14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"
#include "ovi_output_format_m14a0.h"
#include "ovi_output_format_m14b0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define OVI_HW_M14_LVDS_MAX_PORT_NUM 4
#define OVI_HW_M14_VX1_CASE_A_LANE_NUM 4
#define OVI_HW_M14_VX1_CASE_B_LANE_NUM 6
#define OVI_HW_M14_VX1_RTOP_MAX_LANE_NUM 8 // RTOP에서는 Vx1이 8lane까지 지원하는 것으로 되어 있으나 CTOP에서 6lane으로 제한하여 최종 6lane 사용 가능
#define OVI_HW_M14_VX1_LANE0_PDB_EN_BIT 11
#define OVI_HW_M14_VX1_LINK11_EN_BIT 23
#define OVI_HW_M14_RGB_D0_VAL 0
#define OVI_HW_M14_OSD_D0_VAL 4

#define OVI_HW_M14A0_TCON_GAMMA_TABLE_SIZENUM 768
#define OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM 1024
#define OVI_HW_TCON_INPUT_GAMMA_SIZENUM 1024
#define OVI_HW_TCON_INPUT_OD_SIZENUM 17*17

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


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	volatile UINT32 *dppbVinCfgAddr;

	volatile UINT32 *dppaRegBaseAddr;
	volatile UINT32 *tconRegBaseAddr;
	volatile UINT32 *oifRegBaseAddr;
	volatile UINT32 *ctopRegBaseAddr;

	UINT32 chPowerOnRegValue;
} OVI_HW_M14_INFO_T;

typedef struct{
	LX_OVI_OUTPUT_MODE_T	*pOVIOutputModes;
	UINT32 number_of_modes;
} OVI_HW_M14_OUTPUT_MODES_T;

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
unsigned char _gOVIOdDetour = FALSE;

OVI_TCON_REG_M14_T gOVI_TCON_M14;
OVI_OIF_REG_M14_T gOVI_OIF_M14;

static unsigned int *gMapped_address;

LX_OVI_OUTPUT_MODE_T _gOVIOutputModes_M14Ax[] =
{
	{"TCON_EPI_DETOUR_ODC_WITH_ODC_BLACK\n", ovi_m14a0_detour_odc_with_odc_black, sizeof(ovi_m14a0_detour_odc_with_odc_black)/sizeof(LX_OVI_REG_T)},
	{"TCON_EPI_RESTORE_ODC_FROM_BLACK_DETOUR\n", ovi_m14a0_restore_odc_from_black_detour, sizeof(ovi_m14a0_restore_odc_from_black_detour)/sizeof(LX_OVI_REG_T)},
	{"TCON_EPI_HORIZONTAL_REVERSE_ON\n",  ovi_m14a0_debug_mode_epi_horizontal_reverse_on, sizeof(ovi_m14a0_debug_mode_epi_horizontal_reverse_on)/sizeof(LX_OVI_REG_T)},
	{"TCON_EPI_HORIZONTAL_REVERSE_OFF\n",  ovi_m14a0_debug_mode_epi_horizontal_reverse_off, sizeof(ovi_m14a0_debug_mode_epi_horizontal_reverse_off)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_H3D_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_h3d, sizeof(ovi_m14a0_debug_mode_detour_h3d)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_FRC_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_frc, sizeof(ovi_m14a0_debug_mode_detour_frc)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_PE1_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_pe1, sizeof(ovi_m14a0_debug_mode_detour_pe1)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_SRE_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_sre, sizeof(ovi_m14a0_debug_mode_detour_sre)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_DPPB_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_dppb, sizeof(ovi_m14a0_debug_mode_detour_dppb)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_TCON_Block_Detour_Mode\n",  ovi_m14a0_debug_mode_detour_tcon, sizeof(ovi_m14a0_debug_mode_detour_tcon)/sizeof(LX_OVI_REG_T)},
};

LX_OVI_OUTPUT_MODE_T _gOVIOutputModes_M14Bx[] =
{
	{"Detour_Debugger_H3D_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_h3d, sizeof(ovi_m14b0_debug_mode_detour_h3d)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_FRC_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_frc, sizeof(ovi_m14b0_debug_mode_detour_frc)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_PE1_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_pe1, sizeof(ovi_m14b0_debug_mode_detour_pe1)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_SRE_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_sre, sizeof(ovi_m14b0_debug_mode_detour_sre)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_DPPB_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_dppb, sizeof(ovi_m14b0_debug_mode_detour_dppb)/sizeof(LX_OVI_REG_T)},
	{"Detour_Debugger_TCON_Block_Detour_Mode\n",  ovi_m14b0_debug_mode_detour_tcon, sizeof(ovi_m14b0_debug_mode_detour_tcon)/sizeof(LX_OVI_REG_T)},
};

OVI_HW_M14_OUTPUT_MODES_T _gOVIOutputModes_M14 =
{
	.pOVIOutputModes 	 = NULL,
	.number_of_modes	 = 0,
};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OVI_HW_M14_INFO_T _gOviHwM14Info;
static UINT32 _gGainStep = 26;	// 0.26% step per w/b gain 1
static SINT16 _gOffsetStep = 8;	// 8 step per w/b offset 1

#define DEFAULT_CTGAIN 16384
static UINT32 _gPrevRedGain = DEFAULT_CTGAIN, _gPrevGreenGain = DEFAULT_CTGAIN, _gPrevBlueGain = DEFAULT_CTGAIN;
static SINT16 _gPrevRedOffset = 0, _gPrevGreenOffset = 0, _gPrevBlueOffset = 0;

/*========================================================================================
	Implementation Group
========================================================================================*/
int OVI_HW_M14_Initialize(void)
{
	int ret = RET_OK;
	UINT32 u32BaseAddr = 0;
	UINT32 u32InputRGBNum = 0;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			OVI_PRINT("OVI chip revision is set to M14 B0\n");
			/* Set configuration of Output mode presets, for mode change on run time */
			_gOVIOutputModes_M14.pOVIOutputModes = _gOVIOutputModes_M14Bx;
			_gOVIOutputModes_M14.number_of_modes	 = sizeof(_gOVIOutputModes_M14Bx)/sizeof(LX_OVI_OUTPUT_MODE_T);

			gOVI_TCON_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_TCON_REG_M14B0_T));
			gOVI_OIF_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_OIF_REG_M14B0_T));

			gOVI_TCON_M14.phys.addr = (volatile UINT32 *)ioremap(OVI_TCON_REG_M14B0_BASE, sizeof(OVI_TCON_REG_M14B0_T));
			gOVI_OIF_M14.phys.addr = (volatile UINT32 *)ioremap(OVI_OIF_REG_M14B0_BASE, sizeof(OVI_OIF_REG_M14B0_T));

			_gOviHwM14Info.tconRegBaseAddr = gOVI_TCON_M14.phys.addr;
			_gOviHwM14Info.oifRegBaseAddr = gOVI_OIF_M14.phys.addr;
			_gOviHwM14Info.ctopRegBaseAddr = gCTOP_CTRL_M14.phys.addr;

			/* for copying value used in channel power on/off function */
			CTOP_CTRL_M14B0_RdFL(LEFT, ctr91);
			_gOviHwM14Info.chPowerOnRegValue = CTOP_CTRL_M14B0_Rd(LEFT, ctr91);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14, A0))
		{
			OVI_PRINT("OVI chip revision is set to M14 A0\n");
			/* Set configuration of Output mode presets, for mode change on run time */
			_gOVIOutputModes_M14.pOVIOutputModes = _gOVIOutputModes_M14Ax;
			_gOVIOutputModes_M14.number_of_modes	 = sizeof(_gOVIOutputModes_M14Ax)/sizeof(LX_OVI_OUTPUT_MODE_T);

			gOVI_TCON_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_TCON_REG_M14A0_T));
			gOVI_OIF_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_OIF_REG_M14A0_T));

			gOVI_TCON_M14.phys.addr = (volatile UINT32 *)ioremap(OVI_TCON_REG_M14A0_BASE, sizeof(OVI_TCON_REG_M14A0_T));
			gOVI_OIF_M14.phys.addr = (volatile UINT32 *)ioremap(OVI_OIF_REG_M14A0_BASE, sizeof(OVI_OIF_REG_M14A0_T));

			_gOviHwM14Info.dppbVinCfgAddr = (volatile UINT32 *)ioremap(0xC0030010, sizeof(UINT32)); // 0xC0030010 -> AD_VIN_CONFIG of DPPB LED

			_gOviHwM14Info.dppaRegBaseAddr = (volatile UINT32 *)ioremap(0xC0030010, sizeof(UINT32));
			_gOviHwM14Info.tconRegBaseAddr = gOVI_TCON_M14.phys.addr;
			_gOviHwM14Info.oifRegBaseAddr = gOVI_OIF_M14.phys.addr;
			_gOviHwM14Info.ctopRegBaseAddr = gCTOP_CTRL_M14.phys.addr;

			/* for copying value used in channel power on/off function */
			CTOP_CTRL_M14A0_RdFL(ctr111);
			_gOviHwM14Info.chPowerOnRegValue = CTOP_CTRL_M14A0_Rd(ctr111);

			/* OD detour in M14 A0 */
			if(lx_chip_rev() == LX_CHIP_REV( M14, A0))
			{
				OVI_TCON_M14A0_RdFL(tcon_control);
				OVI_TCON_M14A0_Rd01(tcon_control, in_num, u32InputRGBNum);

				OVI_REG_WR(0xC0040018, 0x00000001);
				OVI_REG_WR(0xFD30015C, 0x00100000);
				OVI_REG_WR(0xC002B054, 0xA0000000);
				if(u32InputRGBNum == 0x1) {
					OVI_REG_WR(0xC0040004, 0x0000005F);			// enable colortemp, gamma, dither
				}
				else if(u32InputRGBNum == 0x3) {
					OVI_REG_WR(0xC0040004, 0x000000DF);			// enable colortemp, gamma, dither
				}
				OVI_REG_WR(0xC004001C, 0x00000018);
				_gOVIOdDetour = TRUE;
			}
		}

		/* Get tcon base address for Frame OD */
		u32BaseAddr = gpOviTconMem->base;
		if(u32BaseAddr)
		{
			OVI_TCON_M14_RdFL(tcon_base_address);
			OVI_TCON_M14_Wr(tcon_base_address, u32BaseAddr);
			OVI_TCON_M14_WrFL(tcon_base_address);
		}
		else
		{
			OVI_ERROR("[OVI] TCON memory is not configured!\n");
			OVI_BREAK_WRONG((UINT32)gpOviTconMem);
		}
	} while(0);

	return ret;
}

int OVI_HW_M14_Close(void)
{
	int ret = RET_OK;

	if( lx_chip_rev() >= LX_CHIP_REV( M14, A0) )
	{
		if(gOVI_TCON_M14.shdw.addr)
		{
			OS_Free((void *)gOVI_TCON_M14.shdw.addr);
			gOVI_TCON_M14.shdw.addr = NULL;
		}
		if(gOVI_OIF_M14.shdw.addr)
		{
			OS_Free((void *)gOVI_OIF_M14.shdw.addr);
			gOVI_OIF_M14.shdw.addr = NULL;
		}

		if (gOVI_TCON_M14.phys.addr)
		{
			iounmap((void *)gOVI_TCON_M14.phys.addr);
			gOVI_TCON_M14.phys.addr = NULL;
		}
		if (gOVI_OIF_M14.phys.addr)
		{
			iounmap((void *)gOVI_OIF_M14.phys.addr);
			gOVI_OIF_M14.phys.addr = NULL;
		}

		if (_gOviHwM14Info.dppbVinCfgAddr)
		{
			iounmap((void *)_gOviHwM14Info.dppbVinCfgAddr);
			_gOviHwM14Info.dppbVinCfgAddr = NULL;
		}

		if (_gOviHwM14Info.dppaRegBaseAddr)
		{
			iounmap((void *)_gOviHwM14Info.dppaRegBaseAddr);
			_gOviHwM14Info.dppaRegBaseAddr = NULL;
		}
		_gOviHwM14Info.tconRegBaseAddr = NULL;
		_gOviHwM14Info.oifRegBaseAddr = NULL;
		_gOviHwM14Info.ctopRegBaseAddr = NULL;
	}

	return ret;
}

int OVI_HW_M14_Resume(void)
{
	int ret = RET_OK;
	UINT32 u32BaseAddr = 0;
	UINT32 u32InputRGBNum = 0;

	do {
		/* OD detour in M14 A0 */
		if(lx_chip_rev() == LX_CHIP_REV( M14, A0))
		{
			OVI_TCON_M14A0_RdFL(tcon_control);
			OVI_TCON_M14A0_Rd01(tcon_control, in_num, u32InputRGBNum);

			OVI_REG_WR(0xC0040018, 0x00000001);
			OVI_REG_WR(0xFD30015C, 0x00100000);
			OVI_REG_WR(0xC002B054, 0xA0000000);
			if(u32InputRGBNum == 0x1) {
				OVI_REG_WR(0xC0040004, 0x0000005F);			// enable colortemp, gamma, dither
			}
			else if(u32InputRGBNum == 0x3) {
				OVI_REG_WR(0xC0040004, 0x000000DF);			// enable colortemp, gamma, dither
			}
			OVI_REG_WR(0xC004001C, 0x00000018);
			_gOVIOdDetour = TRUE;
		}

		/* Get tcon base address for Frame OD */
		u32BaseAddr = gpOviTconMem->base;
		if(u32BaseAddr)
		{
			OVI_TCON_M14_RdFL(tcon_base_address);
			OVI_TCON_M14_Wr(tcon_base_address, u32BaseAddr);
			OVI_TCON_M14_WrFL(tcon_base_address);
		}
		else
		{
			OVI_ERROR("[OVI] TCON memory is not configured!\n");
			OVI_BREAK_WRONG((UINT32)gpOviTconMem);
		}
	} while(0);

	return ret;
}

int OVI_HW_M14_SetCombination(UINT32 param)
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
			for(i=0; i<_gOVIOutputModes_M14.number_of_modes;i++)
			{
				printk("  Index[%d] : %s", i,_gOVIOutputModes_M14.pOVIOutputModes[i].index);
			}
			ret = RET_OK;
			break;
		}
		else if(param >= 10000)
		{
			param = param - 10000;
			printk("\n[OVI] Compare register values regarding to register-set index[%d]", param);

			if(param > _gOVIOutputModes_M14.number_of_modes) {
				OVI_ERROR("Unknown OVI index. Index was [%d]", param);
				break;
			}

			regs  = _gOVIOutputModes_M14.pOVIOutputModes[param].regs;
			count = _gOVIOutputModes_M14.pOVIOutputModes[param].count;
			index = _gOVIOutputModes_M14.pOVIOutputModes[param].index;

			printk("\n[OVI] Register Index[%d] => %s",param, (char *)index);
			for(i=0; i<count; i++){
				OVI_REG_RD(regs[i].addr, read_data);
				if(read_data == regs[i].value) same = 1;
				else same = 0;

				printk("\n[%03d] Current value of [0x%08x] => [0x%08x] / index value [0x%08x] (%s)", i, regs[i].addr, read_data, regs[i].value, same ? "SAME":"DIFFER");
			}
			printk("\n[OVI] Register read complete!\n");

			ret = RET_OK;
			break;
		}
		else if(param > _gOVIOutputModes_M14.number_of_modes) {
			OVI_ERROR("\nUnknown OVI mode. Index was [%d]\n", param);
			break;
		}

		/* Step 2. Set mode */
		regs  = _gOVIOutputModes_M14.pOVIOutputModes[param].regs;
		count = _gOVIOutputModes_M14.pOVIOutputModes[param].count;
		index = _gOVIOutputModes_M14.pOVIOutputModes[param].index;

		printk("\n[OVI] Setting to registers to register-set-index => %s", (char *)index);
		for(i=0; i<count; i++){
			printk("\n[%03d] Writing... [0x%08x] <= [0x%08x]", i, regs[i].addr, regs[i].value);
			OVI_REG_WR(regs[i].addr, regs[i].value);
		}
		printk("\n[OVI] Setting preset complete!\n");

		ret = RET_OK;
	} while(0);

	return ret;
}

int OVI_HW_M14_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams)
{
	int ret = RET_OK;

	do {
		// check structure pointer
		if (pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		//select chip type
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			CTOP_CTRL_M14B0_RdFL(LEFT, ctr93);

			if (pstParams->bEnable)
			{
				if (pstParams->u16Period == LX_OVI_SPREAD_SPECTRUM_DEFAULT_PERIOD)
				{
					switch(pstParams->eSpreadRatio)
					{
						case LX_OVI_SPREAD_RATIO_1 :
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x5);
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x8);
						break;
						case LX_OVI_SPREAD_RATIO_2 :
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x6);
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x6);
						break;
						case LX_OVI_SPREAD_RATIO_4 :
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x9);
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x4);
						break;
						case LX_OVI_SPREAD_RATIO_3 :
						default :
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x8);
							CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x5);
						break;
					}
				}
				else if (pstParams->u16Period == 60)
				{
					CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x8);
					CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x6);
				}
				else
				{
					CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x8);
					CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x5);
				}
			}
			else
			{
				CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, 0x0);
				CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, 0x0);
			}

			CTOP_CTRL_M14B0_WrFL(LEFT, ctr93);
		}
		else
		{
			CTOP_CTRL_M14A0_RdFL(ctr83);

			if(pstParams->bEnable)
			{
				if(pstParams->u16Period == LX_OVI_SPREAD_SPECTRUM_DEFAULT_PERIOD)
				{
					switch(pstParams->eSpreadRatio)
					{
						case LX_OVI_SPREAD_RATIO_1 :
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x5);
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x8);
						break;
						case LX_OVI_SPREAD_RATIO_2 :
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x6);
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x6);
						break;
						case LX_OVI_SPREAD_RATIO_4 :
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x9);
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x4);
						break;
						case LX_OVI_SPREAD_RATIO_3 :
						default :
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x8);
							CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x5);
						break;
					}
				}
				else
				{
					CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x8);
					CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x5);
				}
			}
			else
			{
				CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, 0x0);
				CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, 0x0);
			}

			CTOP_CTRL_M14A0_WrFL(ctr83);
		}
	} while(0);

	return ret;
}

int OVI_HW_M14_SetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;
	int i;
	UINT32 addrOffset=0;
	UINT8 *u8odLutData = NULL;
	UINT32 u32odLutData;

	if(_gOVIOdDetour)return RET_OK;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		// check input table size
		if(pstParams->odLut.sizeNum != OVI_HW_TCON_INPUT_OD_SIZENUM)
		{
			OVI_ERROR("[OVI] Input OD LUT size is not correct!\n");
			ret = RET_ERROR;
			break;
		}

		// select address by chip type and OD mode
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			if(pstParams->eMode == LX_OVI_TCON_FRAMEOD)
				addrOffset = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_odc_start_2d);
			else if(pstParams->eMode == LX_OVI_TCON_LINEOD)
				addrOffset = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_lodc_start_3d);
		}
		else
		{
			if(pstParams->eMode == LX_OVI_TCON_FRAMEOD)
				addrOffset = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_odc_start_2d);
			else if(pstParams->eMode == LX_OVI_TCON_LINEOD)
				addrOffset = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_lodc_start_3d);
		}

		u8odLutData = OS_Malloc(OVI_HW_TCON_INPUT_OD_SIZENUM);
		if(u8odLutData == NULL)
		{
			OVI_ERROR("[OVI] memory allocate failed! \n");
			ret = RET_ERROR;
			break;
		}

		ret = copy_from_user(u8odLutData, (void __user *)pstParams->odLut.pData, OVI_HW_TCON_INPUT_OD_SIZENUM);
		if(ret)
		{
			OVI_ERROR("[OVI] Error in copying from user! \n");
			ret = RET_ERROR;
			break;
		}

		for(i=0; i<OVI_HW_TCON_INPUT_OD_SIZENUM; i++)
		{
			u32odLutData = (UINT32)u8odLutData[i];
			OVI_REG_WR(addrOffset, u32odLutData);
			addrOffset = addrOffset + 0x4;
		}

		if(pstParams->eMode == LX_OVI_TCON_FRAMEOD)
		{
			OVI_TCON_M14_RdFL(tcon_odc_opt2);
			OVI_TCON_M14_Wr01(tcon_odc_opt2, odc_upload, 0x0);
			OVI_TCON_M14_WrFL(tcon_odc_opt2);
			OVI_TCON_M14_RdFL(tcon_odc_opt2);
			OVI_TCON_M14_Wr01(tcon_odc_opt2, odc_upload, 0x1);
			OVI_TCON_M14_WrFL(tcon_odc_opt2);
		}
		else if(pstParams->eMode == LX_OVI_TCON_LINEOD)
		{
			OVI_TCON_M14_RdFL(tcon_odc_opt2);
			OVI_TCON_M14_Wr01(tcon_odc_opt2, lodc_upload, 0x0);
			OVI_TCON_M14_WrFL(tcon_odc_opt2);
			OVI_TCON_M14_RdFL(tcon_odc_opt2);
			OVI_TCON_M14_Wr01(tcon_odc_opt2, lodc_upload, 0x1);
			OVI_TCON_M14_WrFL(tcon_odc_opt2);
		}
	} while(0);

	if(u8odLutData != NULL)
		OS_Free(u8odLutData);

	return ret;
}

int OVI_HW_M14_SetFrameOdcEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gOVIOdDetour) return RET_OK;


	do {
		OVI_TCON_M14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_M14_Wr01(tcon_control, odc_en, 0x1);
		else
			OVI_TCON_M14_Wr01(tcon_control, odc_en, 0x0);
		OVI_TCON_M14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_M14_SetColorTempEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_M14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_M14_Wr01(tcon_control, wb_en, 0x1);
		else
			OVI_TCON_M14_Wr01(tcon_control, wb_en, 0x0);
		OVI_TCON_M14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_M14_SetGammaEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_M14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_M14_Wr01(tcon_control, dga_en, 0x1);
		else
			OVI_TCON_M14_Wr01(tcon_control, dga_en, 0x0);
		OVI_TCON_M14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_M14_SetDitherEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			OVI_TCON_M14B0_RdFL(tcon_control);
			if(bParam)
			{
				OVI_TCON_M14B0_Wr01(tcon_control, post_dither_en, 0x1);
			}
			else
			{
				OVI_TCON_M14B0_Wr01(tcon_control, post_dither_en, 0x0);
			}
			OVI_TCON_M14B0_WrFL(tcon_control);
		}
		else
		{
			OVI_TCON_M14A0_RdFL(tcon_control);
			if(bParam)
				OVI_TCON_M14A0_Wr01(tcon_control, dither_en, 0x1);
			else
				OVI_TCON_M14A0_Wr01(tcon_control, dither_en, 0x0);
			OVI_TCON_M14A0_WrFL(tcon_control);
		}
	} while (0);

	return ret;
}

int OVI_HW_M14_SetLineOdcMode(LX_OVI_TCON_LINEOD_MODE_T *pstParams)
{
	int ret = RET_OK;

	if(_gOVIOdDetour) return RET_OK;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		OVI_TCON_M14_RdFL(tcon_odc_opt1);

		if(pstParams->eVersion == LX_OVI_TCON_LODC_V2_0)	//PCID version2.0
		{
			if(pstParams->eRefLastSubPix == LX_OVI_TCON_LODC_NORMAL_REF)
				OVI_TCON_M14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x0);
			else
				OVI_TCON_M14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x1);

			if(pstParams->eRefPrevLine == LX_OVI_TCON_LODC_LEFT_REF)
				OVI_TCON_M14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x0);
			else
				OVI_TCON_M14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x1);
			OVI_TCON_M14_Wr01(tcon_odc_opt1, pcid20_en, 0x1);
		}
		else if(pstParams->eVersion == LX_OVI_TCON_LODC_V1_0)	//PCID version1.0
		{
			OVI_TCON_M14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x0);
			OVI_TCON_M14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x0);

			OVI_TCON_M14_Wr01(tcon_odc_opt1, pcid20_en, 0x0);
		}

		if(pstParams->bEnable)	//PCID enable/disable
			OVI_TCON_M14_Wr01(tcon_odc_opt1, lodc_en_3d, 0x1);
		else
			OVI_TCON_M14_Wr01(tcon_odc_opt1, lodc_en_3d, 0x0);

		OVI_TCON_M14_WrFL(tcon_odc_opt1);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetDitherMode(LX_OVI_TCON_DITHER_T enParam)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			OVI_TCON_M14B0_RdFL(tcon_dither_carry);
			switch(enParam)
			{
				case LX_OVI_TCON_DITHER_TRUNC:
					OVI_TCON_M14B0_Wr01(tcon_dither_carry, post_dither_sel, 0x0);
					break;
				case LX_OVI_TCON_DITHER_ROUND:
					OVI_TCON_M14B0_Wr01(tcon_dither_carry, post_dither_sel, 0x1);
					break;
				case LX_OVI_TCON_DITHER_RANDOM:
				default :
					OVI_TCON_M14B0_Wr01(tcon_dither_carry, post_dither_sel, 0x2);
					break;
			}
			OVI_TCON_M14B0_WrFL(tcon_dither_carry);
		}
		else
		{
			OVI_TCON_M14A0_RdFL(tcon_dither_carry);
			switch(enParam)
			{
				case LX_OVI_TCON_DITHER_TRUNC:
					OVI_TCON_M14A0_Wr(tcon_dither_carry, 0x0);
					break;
				case LX_OVI_TCON_DITHER_ROUND:
					OVI_TCON_M14A0_Wr(tcon_dither_carry, 0x1);
					break;
				case LX_OVI_TCON_DITHER_RANDOM:
				default :
					OVI_TCON_M14A0_Wr(tcon_dither_carry, 0x2);
					break;
			}
			OVI_TCON_M14A0_WrFL(tcon_dither_carry);
		}
	} while(0);

	return ret;
}

UINT32 OVI_HW_M14_GetGain(UINT32 gain)
{
	UINT32 ret_gain;
	UINT32 temp;

	temp = 10000 + (gain - 192) * _gGainStep;
	ret_gain = (temp * 16384) / 10000;

	return ret_gain;
}

SINT16 OVI_HW_M14_GetOffset(UINT32 offset)
{
	SINT16 ret_offset;

	ret_offset = ((SINT32)offset - 64) * _gOffsetStep;
	return ret_offset;
}

int OVI_HW_M14_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams)
{
	int ret = RET_OK;
	UINT32 r_gain, g_gain, b_gain;
	SINT16 r_offset, g_offset, b_offset;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		r_gain = OVI_HW_M14_GetGain(pstParams->r_gain);
		g_gain = OVI_HW_M14_GetGain(pstParams->g_gain);
		b_gain = OVI_HW_M14_GetGain(pstParams->b_gain);

		r_offset = OVI_HW_M14_GetOffset(pstParams->r_offset);
		g_offset = OVI_HW_M14_GetOffset(pstParams->g_offset);
		b_offset = OVI_HW_M14_GetOffset(pstParams->b_offset);

		if((_gPrevRedGain == r_gain) && (_gPrevGreenGain == g_gain) && (_gPrevBlueGain == b_gain)
			&& (_gPrevRedOffset == r_offset) && (_gPrevGreenOffset == g_offset) && (_gPrevBlueOffset == b_offset))
		{
			return RET_OK;
		}

		// Set red gain
		OVI_TCON_M14_RdFL(tcon_wb_rgain);
		OVI_TCON_M14_Wr(tcon_wb_rgain, r_gain);
		OVI_TCON_M14_WrFL(tcon_wb_rgain);

		// Set red offset
		OVI_TCON_M14_RdFL(tcon_wb_roffset);
		OVI_TCON_M14_Wr(tcon_wb_roffset, r_offset);
		OVI_TCON_M14_WrFL(tcon_wb_roffset);

		// Set green gain
		OVI_TCON_M14_RdFL(tcon_wb_ggain);
		OVI_TCON_M14_Wr(tcon_wb_ggain, g_gain);
		OVI_TCON_M14_WrFL(tcon_wb_ggain);

		// Set green offset
		OVI_TCON_M14_RdFL(tcon_wb_goffset);
		OVI_TCON_M14_Wr(tcon_wb_goffset, g_offset);
		OVI_TCON_M14_WrFL(tcon_wb_goffset);

		// Set blue gain
		OVI_TCON_M14_RdFL(tcon_wb_bgain);
		OVI_TCON_M14_Wr(tcon_wb_bgain, b_gain);
		OVI_TCON_M14_WrFL(tcon_wb_bgain);

		// Set blue offset
		OVI_TCON_M14_RdFL(tcon_wb_boffset);
		OVI_TCON_M14_Wr(tcon_wb_boffset, b_offset);
		OVI_TCON_M14_WrFL(tcon_wb_boffset);

		_gPrevRedGain = r_gain;
		_gPrevGreenGain = g_gain;
		_gPrevBlueGain = b_gain;
		_gPrevRedOffset = r_offset;
		_gPrevGreenOffset = g_offset;
		_gPrevBlueOffset = b_offset;

	} while(0);

	return ret;
}

int OVI_HW_M14_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams)
{
	int ret = RET_OK;
	UINT32 *rGammaTable = NULL;
	UINT32 *gGammaTable = NULL;
	UINT32 *bGammaTable = NULL;
	UINT32 *rGammaBuffer = NULL;
	UINT32 *gGammaBuffer = NULL;
	UINT32 *bGammaBuffer = NULL;
	UINT32 *pGamma = NULL;

	int i,j;
	UINT32 reg_addr_start;
	UINT32 previous_gamma;
	UINT32 gamma_data=0;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			if((pstParams->rGammaLut.sizeNum != OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM)
			|| (pstParams->gGammaLut.sizeNum != OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM)
			|| (pstParams->bGammaLut.sizeNum != OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM))
			{
				OVI_ERROR("[OVI] Gamma table size is not correct! \n");
				ret = RET_ERROR;
				break;
			}

			rGammaTable = OS_Malloc(OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			gGammaTable = OS_Malloc(OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			bGammaTable = OS_Malloc(OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			if(rGammaTable == NULL || gGammaTable == NULL || bGammaTable == NULL)
			{
				OVI_ERROR("[OVI] memory allocate failed! \n");
				ret = RET_ERROR;
				break;
			}

			ret = copy_from_user(rGammaTable, (void __user *)pstParams->rGammaLut.pData, OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			ret |= copy_from_user(gGammaTable, (void __user *)pstParams->gGammaLut.pData, OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			ret |= copy_from_user(bGammaTable, (void __user *)pstParams->bGammaLut.pData, OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM*4);
			if(ret)
			{
				OVI_ERROR("[OVI] Error in copying from user! \n");
				ret = RET_ERROR;
				break;
			}

			/* Setting Red Gamma Table */
			if(rGammaTable != NULL)
			{
				reg_addr_start = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_dga_low_rs);
				for(i=0; i<OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, rGammaTable[i] << 2);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}
			/* Setting Green Gamma Table */
			if(gGammaTable != NULL)
			{
				reg_addr_start = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_dga_low_gs);
				for(i=0; i<OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, gGammaTable[i] << 2);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}
			/* Setting Blue Gamma Table */
			if(bGammaTable != NULL)
			{
				reg_addr_start = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_dga_low_bs);
				for(i=0; i<OVI_HW_M14B0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, bGammaTable[i] << 2);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}

			// gamma table upload
			if (lx_chip_rev() >= LX_CHIP_REV( M14, C0))
			{
				OVI_TCON_M14_RdFL(tcon_dga_sel);
				OVI_TCON_M14_Wr01(tcon_dga_sel, dga_sel0, 0x0);	// dga_up_mode : 0 -> sync update
				OVI_TCON_M14_WrFL(tcon_dga_sel);
				OVI_TCON_M14_RdFL(tcon_dga_sel);
				OVI_TCON_M14_Wr01(tcon_dga_sel, dga_sel1, 0x1);	// dga_upload : 1 -> upload bit cleared by HW
				OVI_TCON_M14_WrFL(tcon_dga_sel);
			}
			else
			{
				OVI_TCON_M14_RdFL(tcon_dga_sel);
				OVI_TCON_M14_Wr01(tcon_dga_sel, dga_upload, 0x0);
				OVI_TCON_M14_WrFL(tcon_dga_sel);
				OVI_TCON_M14_RdFL(tcon_dga_sel);
				OVI_TCON_M14_Wr01(tcon_dga_sel, dga_upload, 0x1);
				OVI_TCON_M14_WrFL(tcon_dga_sel);
			}
		}
		else
		{
			if((pstParams->rGammaLut.sizeNum != OVI_HW_TCON_INPUT_GAMMA_SIZENUM)
			|| (pstParams->gGammaLut.sizeNum != OVI_HW_TCON_INPUT_GAMMA_SIZENUM)
			|| (pstParams->bGammaLut.sizeNum != OVI_HW_TCON_INPUT_GAMMA_SIZENUM))
			{
				OVI_ERROR("[OVI] Gamma table size is not correct! \n");
				ret = RET_ERROR;
				break;
			}

			rGammaTable = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			gGammaTable = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			bGammaTable = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			rGammaBuffer = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			gGammaBuffer = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			bGammaBuffer = OS_Malloc(OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			pGamma = OS_Malloc(OVI_HW_M14A0_TCON_GAMMA_TABLE_SIZENUM*4);
			if(rGammaTable == NULL || gGammaTable == NULL || bGammaTable == NULL
				|| rGammaBuffer == NULL || gGammaBuffer == NULL || bGammaBuffer == NULL || pGamma == NULL)
			{
				OVI_ERROR("[OVI] memory allocate failed! \n");
				ret = RET_ERROR;
				break;
			}

			ret = copy_from_user(rGammaTable, (void __user *)pstParams->rGammaLut.pData, OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			ret |= copy_from_user(gGammaTable, (void __user *)pstParams->gGammaLut.pData, OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			ret |= copy_from_user(bGammaTable, (void __user *)pstParams->bGammaLut.pData, OVI_HW_TCON_INPUT_GAMMA_SIZENUM*4);
			if(ret)
			{
				OVI_ERROR("[OVI] Error in copying from user! \n");
				ret = RET_ERROR;
				break;
			}

			/* Prevent Gamma Reversion */
			for(i=1; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++) {
				if(rGammaTable[i] < rGammaTable[i-1])
				{
					rGammaTable[i] = rGammaTable[i-1];
				}
			}
			for(i=1; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++) {
				if(gGammaTable[i] < gGammaTable[i-1])
				{
					gGammaTable[i] = gGammaTable[i-1];
				}
			}
			for(i=1; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++) {
				if(bGammaTable[i] < bGammaTable[i-1])
				{
					bGammaTable[i] = bGammaTable[i-1];
				}
			}

			/* Setting Red Gamma Table */
			if(rGammaTable != NULL)
			{
				previous_gamma = 0;
				for(i=0; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++)
				{
					rGammaTable[i] = rGammaTable[i] << 2;
					rGammaBuffer[i] = rGammaTable[i] - previous_gamma;
					previous_gamma = rGammaTable[i];
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_rs);	//Red Gamma Start Address
				i=0, j=0, gamma_data=0;

				while(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_re)) //Red Gamma End Address
				{
					if(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_rs) + 255 * 4)	//Red Gamma Medium Address((tcon_dga_med_re - tcon_dga_med_rs)/3)
					{
						gamma_data = (rGammaBuffer[i]<<4) | rGammaBuffer[i+1];
						i = i+2;
					}
					else
					{
						gamma_data = rGammaBuffer[i];
						i++;
					}

					pGamma[j]=gamma_data;
					j++;
					reg_addr_start+=4;
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_rs);
				for(i=0; i<OVI_HW_M14A0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, pGamma[i]);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}

			/* Setting Green Gamma Table */
			if(gGammaTable != NULL)
			{
				previous_gamma = 0;
				for(i=0; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++)
				{
					gGammaTable[i] = gGammaTable[i] << 2;
					gGammaBuffer[i] = gGammaTable[i] - previous_gamma;
					previous_gamma = gGammaTable[i];
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_gs); //Green Gamma Start Address
				i=0, j=0, gamma_data=0;

				while(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_ge)) //Green Gamma End Address
				{
					if(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_gs) + 255 * 4) //Green Gamma Medium Address((tcon_dga_med_ge - tcon_dga_med_gs)/3)
					{
						gamma_data = (gGammaBuffer[i]<<4) | gGammaBuffer[i+1];
						i = i+2;
					}
					else
					{
						gamma_data = gGammaBuffer[i];
						i++;
					}

					pGamma[j]=gamma_data;
					j++;
					reg_addr_start+=4;
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_gs);
				for(i=0; i<OVI_HW_M14A0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, pGamma[i]);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}

			/* Setting Blue Gamma Table */
			if(bGammaTable != NULL)
			{
				previous_gamma = 0;
				for(i=0; i<OVI_HW_TCON_INPUT_GAMMA_SIZENUM; i++)
				{
					bGammaTable[i] = bGammaTable[i] << 2;
					bGammaBuffer[i] = bGammaTable[i] - previous_gamma;
					previous_gamma = bGammaTable[i];
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_bs); //Blue Gamma Start Address
				i=0, j=0, gamma_data=0;

				while(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_be)) //Blue Gamma End Address
				{
					if(reg_addr_start <= OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_bs) + 255 * 4) //Blue Gamma Medium Address((tcon_dga_med_be - tcon_dga_med_bs)/3)
					{
						gamma_data = (bGammaBuffer[i]<<4) | bGammaBuffer[i+1];
						i = i+2;
					}
					else
					{
						gamma_data = bGammaBuffer[i];
						i++;
					}

					pGamma[j]=gamma_data;
					j++;
					reg_addr_start+=4;
				}

				reg_addr_start = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_dga_med_bs);
				for(i=0; i<OVI_HW_M14A0_TCON_GAMMA_TABLE_SIZENUM; i++)
				{
					OVI_REG_WR(reg_addr_start, pGamma[i]);
					reg_addr_start = reg_addr_start + 0x4;
				}
			}

			// gamma table upload
			OVI_TCON_M14_RdFL(tcon_dga_sel);
			OVI_TCON_M14_Wr01(tcon_dga_sel, dga_upload, 0x0);
			OVI_TCON_M14_WrFL(tcon_dga_sel);
			OVI_TCON_M14_RdFL(tcon_dga_sel);
			OVI_TCON_M14_Wr01(tcon_dga_sel, dga_upload, 0x1);
			OVI_TCON_M14_WrFL(tcon_dga_sel);
		}
	} while(0);

	if(rGammaTable != NULL)
		OS_Free(rGammaTable);
	if(gGammaTable != NULL)
		OS_Free(gGammaTable);
	if(bGammaTable != NULL)
		OS_Free(bGammaTable);
	if(rGammaBuffer != NULL)
		OS_Free(rGammaBuffer);
	if(gGammaBuffer != NULL)
		OS_Free(gGammaBuffer);
	if(bGammaBuffer != NULL)
		OS_Free(bGammaBuffer);
	if(pGamma != NULL)
		OS_Free(pGamma);

	return ret;
}

int OVI_HW_M14_GetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;
	UINT32 u32odcAddr=0;
	volatile UINT32 *pMappedAddr = NULL;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			switch(pstParams->eMode)
			{
				case LX_OVI_TCON_FRAMEOD:
					u32odcAddr = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_odc_start_2d);
					break;
				case LX_OVI_TCON_LINEOD:
					u32odcAddr = OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_lodc_start_3d);
					break;
				default:
					break;
			}
		}
		else
		{
			switch(pstParams->eMode)
			{
				case LX_OVI_TCON_FRAMEOD:
					u32odcAddr = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_odc_start_2d);
					break;
				case LX_OVI_TCON_LINEOD:
					u32odcAddr = OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_lodc_start_3d);
					break;
				default:
					break;
			}
		}
		pstParams->odLut.sizeNum = OVI_HW_TCON_INPUT_OD_SIZENUM;

		pMappedAddr = (volatile UINT32 *)ioremap(u32odcAddr, sizeof(UINT32)*OVI_HW_TCON_INPUT_OD_SIZENUM);
		if(pMappedAddr == NULL)
		{
			OVI_ERROR("[OVI] Error in ioremap! \n");
			break;
		}

		ret = copy_to_user((void __user *)pstParams->odLut.pData, pMappedAddr, sizeof(UINT32)*OVI_HW_TCON_INPUT_OD_SIZENUM);
		if(ret)
		{
			OVI_ERROR("[OVI] Error in copying to user! \n");
			ret = RET_ERROR;
			break;
		}
	}while(0);

	if(pMappedAddr != NULL)
		iounmap((void *)pMappedAddr);

	return ret;
}

int OVI_HW_M14_SetOdByteData(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams)
{
	int ret = RET_OK;
	UINT32 u32odcAddr = 0;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			switch(pstParams->mode)
			{
				case LX_OVI_TCON_FRAMEOD:
					u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_odc_start_2d);
					break;
				case LX_OVI_TCON_LINEOD:
					u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_M14B0_BASE + offsetof(OVI_TCON_REG_M14B0_T, tcon_lodc_start_3d);
					break;
				default :
					break;
			}
		}
		else
		{
			switch(pstParams->mode)
			{
				case LX_OVI_TCON_FRAMEOD:
					u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_odc_start_2d);
					break;
				case LX_OVI_TCON_LINEOD:
					u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_M14A0_BASE + offsetof(OVI_TCON_REG_M14A0_T, tcon_lodc_start_3d);
					break;
				default :
					break;
			}
		}

		OVI_REG_WR(u32odcAddr, pstParams->value);

		switch(pstParams->mode)
		{
			case LX_OVI_TCON_FRAMEOD:
				OVI_TCON_M14_RdFL(tcon_odc_opt2);
				OVI_TCON_M14_Wr01(tcon_odc_opt2, odc_upload, 0x1);
				OVI_TCON_M14_WrFL(tcon_odc_opt2);
				OVI_TCON_M14_RdFL(tcon_odc_opt2);
				OVI_TCON_M14_Wr01(tcon_odc_opt2, odc_upload, 0x0);
				OVI_TCON_M14_WrFL(tcon_odc_opt2);
				break;
			case LX_OVI_TCON_LINEOD:
				OVI_TCON_M14_RdFL(tcon_odc_opt2);
				OVI_TCON_M14_Wr01(tcon_odc_opt2, lodc_upload, 0x1);
				OVI_TCON_M14_WrFL(tcon_odc_opt2);
				OVI_TCON_M14_RdFL(tcon_odc_opt2);
				OVI_TCON_M14_Wr01(tcon_odc_opt2, lodc_upload, 0x0);
				OVI_TCON_M14_WrFL(tcon_odc_opt2);
				break;
			default:
				break;
		}
	} while(0);

	return ret;
}

int OVI_HW_M14_SetTconReverse(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_M14_RdFL(tcon_hor_mode);
		if(bParam) {
			OVI_TCON_M14_Wr01(tcon_hor_mode, hor_reverse, 0x0);
		}
		else {
			OVI_TCON_M14_Wr01(tcon_hor_mode, hor_reverse, 0x1);
		}
		OVI_TCON_M14_WrFL(tcon_hor_mode);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetLvdsReverse(BOOLEAN bParam)
{
	int ret = RET_OK;
	UINT32 lvdscheck = 0;

	do {
		OVI_OIF_M14_RdFL(oif_vch_config);
		OVI_OIF_M14_Rd01(oif_vch_config, vch_hb_width, lvdscheck);
		if(bParam) {
			OVI_OIF_M14_Wr01(oif_vch_config, vch_h_reverse, 0x1);
			OVI_OIF_M14_Wr01(oif_vch_config, vch_line_delay, 0x1);
		}
		else {
			OVI_OIF_M14_Wr01(oif_vch_config, vch_h_reverse, 0x0);
			if(lvdscheck != 4) // 60Hz LVDS condition
				OVI_OIF_M14_Wr01(oif_vch_config, vch_line_delay, 0x1);
			else
			OVI_OIF_M14_Wr01(oif_vch_config, vch_line_delay, 0x0);
		}
		OVI_OIF_M14_WrFL(oif_vch_config);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetPatternGenerator(LX_OVI_TCON_TPG_MODE_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_M14_RdFL(tcon_pgen_ctl);

		// pattern source color setting
		OVI_TCON_M14_RdFL(tcon_pgen_agpr1);
		OVI_TCON_M14_Wr(tcon_pgen_agpr1, 0x03FF);
		OVI_TCON_M14_WrFL(tcon_pgen_agpr1);

		OVI_TCON_M14_RdFL(tcon_pgen_agpg1);
		OVI_TCON_M14_Wr(tcon_pgen_agpg1, 0x03FF);
		OVI_TCON_M14_WrFL(tcon_pgen_agpg1);

		OVI_TCON_M14_RdFL(tcon_pgen_agpb1);
		OVI_TCON_M14_Wr(tcon_pgen_agpb1, 0x03FF);
		OVI_TCON_M14_WrFL(tcon_pgen_agpb1);

		switch(enParam)
		{
			case LX_OVI_TCON_TPG_OFF:		// pattern off
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x0);
				break;
			case LX_OVI_TCON_TPG_BLACK:		// black pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x4);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_WHITE:		// white pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HVBAR:		// HVBar pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x3);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR16GREY:		// Horizontal 16 grey pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x8);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR64GREY:		// Horizontal 64 grey pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x7);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR256GREY:		// Horizontal 256 grey pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x6);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR1024GREY:		// Horizontal 1024 grey pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x5);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_VER16GREY:		// Vertical 16 grey pattern
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0xC);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_M14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
		}
		OVI_TCON_M14_WrFL(tcon_pgen_ctl);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetChannelPower(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			CTOP_CTRL_M14B0_RdFL(LEFT, ctr91);
			if(!bParam && (CTOP_CTRL_M14B0_Rd(LEFT, ctr91) != 0x0)) // display output off
			{
				CTOP_CTRL_M14B0_Wr(LEFT, ctr91, 0x0);
				OVI_DEBUG("[OVI] display is changed to OFF status!\n");
			}
			else if(bParam && (CTOP_CTRL_M14B0_Rd(LEFT, ctr91) == 0x0))// display output on
			{
				CTOP_CTRL_M14B0_Wr(LEFT, ctr91, _gOviHwM14Info.chPowerOnRegValue);
				OVI_DEBUG("[OVI] display is changed to ON status!\n");
			}
			else
			{
				OVI_DEBUG("[OVI] channel power status is not changed - same as current status!\n");
			}
			CTOP_CTRL_M14B0_WrFL(LEFT, ctr91);
		}
		else
		{
			CTOP_CTRL_M14A0_RdFL(ctr111);
			if(!bParam && (CTOP_CTRL_M14A0_Rd(ctr111) != 0x0)) // display output off
			{
				CTOP_CTRL_M14A0_Wr(ctr111, 0x0);
				OVI_DEBUG("display is changed to OFF status!\n");
			}
			else if(bParam && (CTOP_CTRL_M14A0_Rd(ctr111) == 0x0))// display output on
			{
				CTOP_CTRL_M14A0_Wr(ctr111, _gOviHwM14Info.chPowerOnRegValue);
				OVI_DEBUG("display is changed to ON status!\n");
			}
			else
			{
				OVI_DEBUG("channel power status is not changed - same as current status!\n");
			}
			CTOP_CTRL_M14A0_WrFL(ctr111);
		}
	} while(0);

	return ret;
}

int OVI_HW_M14_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_M14_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_LVDS_OUT_JEIDA:
				OVI_OIF_M14_Wr01(oif_lvds_config0, lvds_lsb_first, 0x0);
				break;
			case LX_OVI_LVDS_OUT_VESA:
				OVI_OIF_M14_Wr01(oif_lvds_config0, lvds_lsb_first, 0x1);
				break;
			default:
				break;
		}
		OVI_OIF_M14_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_M14_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_8BIT:
				OVI_OIF_M14_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x1);
				break;
			case LX_OVI_10BIT:
				OVI_OIF_M14_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x0);
				break;
			default:
				break;
		}
		OVI_OIF_M14_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetLvdsBlack(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_M14_RdFL(oif_lvds_config1);
		OVI_OIF_M14_RdFL(oif_lvds_config2);

		if(!bParam) // original data out
		{
			OVI_OIF_M14_Wr01(oif_lvds_config1, lvds_force_even_data, 0x0);
			OVI_OIF_M14_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x0);
		}
		else // black data out
		{
			OVI_OIF_M14_Wr01(oif_lvds_config1, lvds_force_even_data, 0x40000000);
			OVI_OIF_M14_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x40000000);
		}
		OVI_OIF_M14_WrFL(oif_lvds_config1);
		OVI_OIF_M14_WrFL(oif_lvds_config2);
	} while(0);

	return ret;
}

int OVI_HW_M14_SetClock(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_M14_RdFL(tcon_gclk_control);
		OVI_TCON_M14_RdFL(tcon_mclk_control);

		if(bParam)
		{
			OVI_TCON_M14_Wr01(tcon_gclk_control, gclk_en, 0x1);
			OVI_TCON_M14_Wr01(tcon_mclk_control, mclk_en, 0x1);
		}
		else
		{
			OVI_TCON_M14_Wr01(tcon_gclk_control, gclk_en, 0x0);
			OVI_TCON_M14_Wr01(tcon_mclk_control, mclk_en, 0x0);
		}

		OVI_TCON_M14_WrFL(tcon_gclk_control);
		OVI_TCON_M14_WrFL(tcon_mclk_control);
	} while(0);

	return ret;
}

int OVI_HW_M14_DisplayResume(LX_OVI_DISPLAY_INFO_T *pstParams)
{
	int ret = RET_OK;
	int i;
	int count;
	const LX_OVI_REG_T *regs;
	struct OVI_OIF_CTOP_CTRL *oifCtopCtrl;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Step 0. TCON/EPI SW reset assert - workaround solution for boot sequence problem */
			if (pstParams->panelInterface == LX_OVI_PANEL_EPI)
			{
				OVI_REG_WR(0xC001F804, 0x11000);
			}

			/* Step 1. Set DE/FRC/DPPA */
			regs	= ovi_m14b0_disp_default_fhd;
			count = sizeof(ovi_m14b0_disp_default_fhd)/sizeof(LX_OVI_REG_T);

			// Spread Spectrum off
			if (pstParams->panelInterface == LX_OVI_PANEL_LVDS
				&& !pstParams->useMemc
				&& pstParams->lvdsType == LX_OVI_LVDS_OUT_JEIDA)
			{
				OVI_REG_WR(0xC0019008, 0x01000000);
				OVI_REG_WR(0xC001900C, 0x981D4000);
				OVI_REG_WR(0xC001FA34, 0xC0290000);
			}
			else if (pstParams->panelInterface == LX_OVI_PANEL_VX1)
			{
				OVI_REG_WR(0xC0019008, 0x81170000);
				OVI_REG_WR(0xC001900C, 0x83244009);
				OVI_REG_WR(0xC001FA34, 0xC0290000);
			}

			for(i=0; i<count; i++)
			{
				OVI_REG_WR(regs[i].addr, regs[i].value);
			}

			/* Step 2. Set TCON SOE Timing */
			switch(pstParams->epiPanelType)
			{
				case LX_OVI_EPI_PANEL_V12 :
					regs = (LX_OVI_REG_T*) ovi_m14b0_tcon_soe_v12;
					count = sizeof(ovi_m14b0_tcon_soe_v12)/sizeof(LX_OVI_REG_T);
					break;
				case LX_OVI_EPI_PANEL_V13 :
					switch(pstParams->panelInch)
					{
						case LX_OVI_PANEL_INCH_42 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v13_42;
							count = sizeof(ovi_m14b0_tcon_soe_v13_42)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_55 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v13_55;
							count = sizeof(ovi_m14b0_tcon_soe_v13_55)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_47 :
						default :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v13_47;
							count = sizeof(ovi_m14b0_tcon_soe_v13_47)/sizeof(LX_OVI_REG_T);
							break;
					}
					break;
				case LX_OVI_EPI_PANEL_V14 :
				default :
					switch(pstParams->panelInch)
					{
						case LX_OVI_PANEL_INCH_32 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_32_6lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_32_6lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_47 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_47_8lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_47_8lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_49 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_49_8lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_49_8lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_50 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_50_8lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_50_8lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_55 :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_55_8lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_55_8lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
						case LX_OVI_PANEL_INCH_42 :
						default :
							regs = (LX_OVI_REG_T*)ovi_m14b0_tcon_soe_v14_42_8lane_120Hz;
							count = sizeof(ovi_m14b0_tcon_soe_v14_42_8lane_120Hz)/sizeof(LX_OVI_REG_T);
							break;
					}
					break;
			}

			for(i=0; i<count; i++)
			{
				OVI_REG_WR(0xC0030000 + regs[i].addr, regs[i].value);
			}

			/* Step 3. Set OIF/CTOP for display */
			switch(pstParams->panelInterface)
			{
				case LX_OVI_PANEL_EPI:
					if(pstParams->epiPanelType == LX_OVI_EPI_PANEL_V13
						&& (pstParams->panelInch == LX_OVI_PANEL_INCH_42 || pstParams->panelInch == LX_OVI_PANEL_INCH_55))
					{
						oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE2];
					}
					else if(pstParams->epiPanelType == LX_OVI_EPI_PANEL_V14
						&& pstParams->panelInch != LX_OVI_PANEL_INCH_32)
					{
						oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE3];
					}
					else
					{
						oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE1];

					}
					break;
				case LX_OVI_PANEL_LVDS:
					if (pstParams->useMemc)
					{
						oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE4];
					}
					else
					{
						oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE5];
					}
					break;
				case LX_OVI_PANEL_VX1:
					oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE0];
					break;
				default:
					oifCtopCtrl = &ovi_m14b0_oif_ctop_ctrl_list[OIF_CTOP_CTRL_CASE4];
					break;
			}

			for(i=0; i<oifCtopCtrl->count; i++)
			{
				OVI_REG_WR(oifCtopCtrl->regs[i].addr, oifCtopCtrl->regs[i].value);
			}


			/* Step 4. Set LVDS mode when nessessary (default 8bit/VESA mode) */
			if (pstParams->panelInterface == LX_OVI_PANEL_LVDS)
			{
				UINT32 regVal;

				if(pstParams->lvdsBit == LX_OVI_10BIT)
				{
					OVI_REG_RD(0xC0039018, regVal);
					regVal &= ~(0x1 << 14);
					OVI_REG_WR(0xC0039018, regVal);

					OVI_REG_WR(0xC0030154, 0x66);

					if (!pstParams->useMemc)
					{
						oifCtopCtrl->channel_power |= 0x41;
					}
					else
					{
						oifCtopCtrl->channel_power |= 0x41000;
					}
				}

				if(pstParams->lvdsType == LX_OVI_LVDS_OUT_JEIDA)
				{
					OVI_REG_RD(0xC0039018, regVal);
					regVal &= ~(0x1 << 15);
					OVI_REG_WR(0xC0039018, regVal);
				}
			}

			/* Step 5. TCON/EPI SW reset de-assert - workaround solution for boot sequence problem */
			if(pstParams->panelInterface == LX_OVI_PANEL_EPI)
			{
				OVI_REG_WR(0xC001F804, 0x0);
			}

			/* Step 6. Set Combo TX Power */
			OS_MsecSleep(1);	// After 1ms sec, set CTR102

			OVI_REG_WR(0xC001FA08, oifCtopCtrl->combo_tx_power);

			/* Step 6-1. Vx1 Link reset */
			if (pstParams->panelInterface == LX_OVI_PANEL_VX1)
			{
				OS_UsecDelay(100);

				OVI_REG_WR(0xC001FA04, 0x0C014800);
			}

			/* Step 7. Set Channel Power */
			OS_UsecDelay(100);

			OVI_REG_WR(0xC001FA2C, oifCtopCtrl->channel_power);

			/* Step 8. Local Dimming SPI setting */
			count = 0;
			switch(pstParams->panelBacklight)
			{
				case LX_OVI_PANEL_BACKLIGHT_EDGE_LED :

					switch(pstParams->panelLedBar)
					{
						case LX_OVI_PANEL_LED_BAR_EDGE_6 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_49
								|| pstParams->panelInch == LX_OVI_PANEL_INCH_55
								|| pstParams->panelInch == LX_OVI_PANEL_INCH_60)
							{
								regs = ovi_m14b0_led_spi_edge_49_55_60_lgd;
								count = sizeof(ovi_m14b0_led_spi_edge_49_55_60_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						default :
							break;
					}
					break;
				case LX_OVI_PANEL_BACKLIGHT_DIRECT_M :

					switch(pstParams->panelLedBar)
					{
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_6 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_60)
							{
								regs = ovi_m14b0_led_spi_dm6_60_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm6_60_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_8 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_65)
							{
								regs = ovi_m14b0_led_spi_dm8_65_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm8_65_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_10 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_47)
							{
								regs = ovi_m14b0_led_spi_dm10_47_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm10_47_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_12 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_50
								|| pstParams->panelInch == LX_OVI_PANEL_INCH_55
								|| pstParams->panelInch == LX_OVI_PANEL_INCH_60)
							{
								regs = ovi_m14b0_led_spi_dm12_50_55_60_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm12_50_55_60_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_15 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_42)
							{
								regs = ovi_m14b0_led_spi_dm15_42_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm15_42_lgd)/sizeof(LX_OVI_REG_T);
							}
							break;
						case LX_OVI_PANEL_LED_BAR_DIRECT_M_16 :
							if (pstParams->panelInch == LX_OVI_PANEL_INCH_65)
							{
								regs = ovi_m14b0_led_spi_dm16_65_lgd;
								count = sizeof(ovi_m14b0_led_spi_dm16_65_lgd)/sizeof(LX_OVI_REG_T);
							}
							else if (pstParams->panelInch == LX_OVI_PANEL_INCH_70 && pstParams->panelMaker == LX_OVI_PANEL_MAKER_SHARP)
							{
								regs = ovi_m14b0_led_spi_dm16_70_sharp;
								count = sizeof(ovi_m14b0_led_spi_dm16_70_sharp)/sizeof(LX_OVI_REG_T);
							}
							break;
						default :
							break;
					}
					break;
				case LX_OVI_PANEL_BACKLIGHT_DIRECT_L :
					regs = ovi_m14b0_led_spi_dl;
					count = sizeof(ovi_m14b0_led_spi_dl)/sizeof(LX_OVI_REG_T);
					break;
				default :
					break;
			}

			for(i=0; i<count; i++)
			{
				OVI_REG_WR(0xC0040000 + regs[i].addr, regs[i].value);
			}
		}
	} while(0);

	return ret;
}

/** @} */
