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
#include "ovi_hw_h14.h"
#include "ovi_reg_def_h14.h"
#include "ovi_reg_h14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"
#include "ovi_output_format_h14a0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define OVI_HW_H14_LVDS_MAX_PORT_NUM 4
#define OVI_HW_H14_VX1_CASE_A_LANE_NUM 4
#define OVI_HW_H14_VX1_CASE_B_LANE_NUM 6
#define OVI_HW_H14_VX1_RTOP_MAX_LANE_NUM 8 // RTOP에서는 Vx1이 8lane까지 지원하는 것으로 되어 있으나 CTOP에서 6lane으로 제한하여 최종 6lane 사용 가능
#define OVI_HW_H14_VX1_LANE0_PDB_EN_BIT 11
#define OVI_HW_H14_VX1_LINK11_EN_BIT 23
#define OVI_HW_H14_RGB_D0_VAL 0
#define OVI_HW_H14_OSD_D0_VAL 4

#define OVI_HW_H14_TCON_GAMMA_TABLE_SIZENUM 768
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
} OVI_HW_H14_INFO_T;

typedef struct{
	LX_OVI_OUTPUT_MODE_T	*pOVIOutputModes;
	UINT32 number_of_modes;
} OVI_HW_H14_OUTPUT_MODES_T;
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
OVI_TCON_REG_H14_T gOVI_TCON_H14;
OVI_OIF_REG_H14_T gOVI_OIF_H14;

static unsigned int *gMapped_address;

LX_OVI_OUTPUT_MODE_T _gOVIOutputModes_H14Ax[] =
{
	{"TCON_EPI_DETOUR_ODC_WITH_ODC_BLACK\n", ovi_h14a0_detour_odc_with_odc_black, sizeof(ovi_h14a0_detour_odc_with_odc_black)/sizeof(LX_OVI_REG_T)},
	{"TCON_EPI_RESTORE_ODC_FROM_BLACK_DETOUR\n", ovi_h14a0_restore_odc_from_black_detour, sizeof(ovi_h14a0_restore_odc_from_black_detour)/sizeof(LX_OVI_REG_T)},
};

OVI_HW_H14_OUTPUT_MODES_T _gOVIOutputModes_H14 =
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
static OVI_HW_H14_INFO_T _gOviHwH14Info;
static UINT32 _gGainStep = 26;	// 0.26% step per w/b gain 1
static SINT16 _gOffsetStep = 8;	// 8 step per w/b offset 1

#define DEFAULT_CTGAIN 16384
static UINT32 _gPrevRedGain = DEFAULT_CTGAIN, _gPrevGreenGain = DEFAULT_CTGAIN, _gPrevBlueGain = DEFAULT_CTGAIN;
static SINT16 _gPrevRedOffset = 0, _gPrevGreenOffset = 0, _gPrevBlueOffset = 0;

/*========================================================================================
	Implementation Group
========================================================================================*/
int OVI_HW_H14_Initialize(void)
{
	int ret = RET_OK;
	UINT32 u32BaseAddr = 0;

	do {
		if(lx_chip_rev() >= LX_CHIP_REV( H14, A0))
		{
			/* Set configuration of Output mode presets, for mode change on run time */
			_gOVIOutputModes_H14.pOVIOutputModes = _gOVIOutputModes_H14Ax;
			_gOVIOutputModes_H14.number_of_modes	 = sizeof(_gOVIOutputModes_H14Ax)/sizeof(LX_OVI_OUTPUT_MODE_T);

			gOVI_TCON_H14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_TCON_REG_H14A0_T));
			gOVI_OIF_H14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(OVI_OIF_REG_H14A0_T));

			gOVI_TCON_H14.phys.addr = (volatile UINT32 *)ioremap(OVI_TCON_REG_H14_BASE, sizeof(OVI_TCON_REG_H14A0_T));
			gOVI_OIF_H14.phys.addr = (volatile UINT32 *)ioremap(OVI_OIF_REG_H14_BASE, sizeof(OVI_OIF_REG_H14A0_T));

			_gOviHwH14Info.dppbVinCfgAddr = (volatile UINT32 *)ioremap(0xC0030010, sizeof(UINT32)); // 0xC0030010 -> AD_VIN_CONFIG of DPPB LED

			_gOviHwH14Info.dppaRegBaseAddr = (volatile UINT32 *)ioremap(0xC0030010, sizeof(UINT32));
			_gOviHwH14Info.tconRegBaseAddr = gOVI_TCON_H14.phys.addr;
			_gOviHwH14Info.oifRegBaseAddr = gOVI_OIF_H14.phys.addr;
			_gOviHwH14Info.ctopRegBaseAddr = gCTOP_CTRL_H14.phys.addr;

			/* Get tcon base address for Frame OD */
			u32BaseAddr = gpOviTconMem->base;
			if(u32BaseAddr)
			{
				OVI_TCON_H14_RdFL(tcon_base_address);
				OVI_TCON_H14_Wr(tcon_base_address, u32BaseAddr);
				OVI_TCON_H14_WrFL(tcon_base_address);
			}
			else
			{
				OVI_ERROR("[OVI] TCON memory is not configured!\n");
				OVI_BREAK_WRONG((UINT32)gpOviTconMem);
			}

			/* for copying value used in channel power on/off function */
			CTOP_CTRL_H14A0_RdFL(ctr111);
			_gOviHwH14Info.chPowerOnRegValue = CTOP_CTRL_H14A0_Rd(ctr111);
		}
	} while(0);

	return ret;
}

int OVI_HW_H14_Close(void)
{
	int ret = RET_OK;

	if(lx_chip_rev() >= LX_CHIP_REV( H14, A0))
	{
		if(gOVI_TCON_H14.shdw.addr)
		{
			OS_Free((void *)gOVI_TCON_H14.shdw.addr);
			gOVI_TCON_H14.shdw.addr = NULL;
		}
		if(gOVI_OIF_H14.shdw.addr)
		{
			OS_Free((void *)gOVI_OIF_H14.shdw.addr);
			gOVI_OIF_H14.shdw.addr = NULL;
		}

		if (gOVI_TCON_H14.phys.addr)
		{
			iounmap((void *)gOVI_TCON_H14.phys.addr);
			gOVI_TCON_H14.phys.addr = NULL;
		}
		if (gOVI_OIF_H14.phys.addr)
		{
			iounmap((void *)gOVI_OIF_H14.phys.addr);
			gOVI_OIF_H14.phys.addr = NULL;
		}

		if (_gOviHwH14Info.dppbVinCfgAddr)
		{
			iounmap((void *)_gOviHwH14Info.dppbVinCfgAddr);
			_gOviHwH14Info.dppbVinCfgAddr = NULL;
		}

		if (_gOviHwH14Info.dppaRegBaseAddr)
		{
			iounmap((void *)_gOviHwH14Info.dppaRegBaseAddr);
			_gOviHwH14Info.dppaRegBaseAddr = NULL;
		}
		_gOviHwH14Info.tconRegBaseAddr = NULL;
		_gOviHwH14Info.oifRegBaseAddr = NULL;
		_gOviHwH14Info.ctopRegBaseAddr = NULL;
	}

	return ret;
}

int OVI_HW_H14_Resume(void)
{
	int ret = RET_OK;
	UINT32 u32BaseAddr = 0;

	do {
		if(lx_chip_rev() >= LX_CHIP_REV( H14, A0))
		{
			/* Get tcon base address for Frame OD */
			u32BaseAddr = gpOviTconMem->base;
			if(u32BaseAddr)
			{
				OVI_TCON_H14_RdFL(tcon_base_address);
				OVI_TCON_H14_Wr(tcon_base_address, u32BaseAddr);
				OVI_TCON_H14_WrFL(tcon_base_address);
			}
			else
			{
				OVI_ERROR("[OVI] TCON memory is not configured!\n");
				OVI_BREAK_WRONG((UINT32)gpOviTconMem);
			}
		}
	} while(0);

	return ret;
}

int OVI_HW_H14_SetCombination(UINT32 param)
{
	int ret = RET_ERROR;
	int i,count;
	volatile LX_OVI_REG_T	*regs;
	char	*index;
	unsigned int read_data, same;

	do {
		/* Step 1. Check whether paramater is valid */
		if(param == 119) {	// Magic keyword to print available output modes...
			printk("\n[OVI] Display possible OVI mode index...\n");
			for(i=0; i<_gOVIOutputModes_H14.number_of_modes;i++)
			{
				printk("[OVI] Index[%d] : %s", i,_gOVIOutputModes_H14.pOVIOutputModes[i].index);
			}
			ret = RET_OK;
			break;
		}
		else if(param >= 10000)
		{
			param = param - 10000;
			printk("\n[OVI] Display current register value for items related to index [%d]\n", param);

			if(param > _gOVIOutputModes_H14.number_of_modes) {
				OVI_ERROR("Unknown OVI index. Index was [%d]\n", param);
				break;
			}

			regs  = _gOVIOutputModes_H14.pOVIOutputModes[param].regs;
			count = _gOVIOutputModes_H14.pOVIOutputModes[param].count;
			index = _gOVIOutputModes_H14.pOVIOutputModes[param].index;

			printk("\n[OVI] Reading registers values related to  index => %s", (char *)index);
			for(i=0; i<count; i++){
				OVI_REG_RD(regs[i].addr, read_data);
				if(read_data == regs[i].value) same = 1;
				else same = 0;

				printk("\n[%03d] Current value of [0x%08x] => [0x%8x] / index value [0x%8x] (%s)", i, regs[i].addr, read_data, regs[i].value, same ? "SAME":"DIFFER");
			}
			printk("\n[OVI] Register read complete!\n");

			ret = RET_OK;
			break;
		}
		else if(param > _gOVIOutputModes_H14.number_of_modes) {
			OVI_ERROR("\nUnknown OVI mode. Index was [%d]\n", param);
			break;
		}

		/* Step 2. Set mode */
		regs  = _gOVIOutputModes_H14.pOVIOutputModes[param].regs;
		count = _gOVIOutputModes_H14.pOVIOutputModes[param].count;
		index = _gOVIOutputModes_H14.pOVIOutputModes[param].index;

		printk("\n[OVI] Setting to preset index => %s", (char *)index);
		for(i=0; i<count; i++){
			printk("\n[%03d] Writing... [0x%08x] <= [0x%8x]", i, regs[i].addr, regs[i].value);
			OVI_REG_WR(regs[i].addr, regs[i].value);
		}
		printk("\n[OVI] Setting preset complete!\n");

		ret = RET_OK;
	} while(0);

	return ret;
}

int OVI_HW_H14_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams)
{
	int ret = RET_OK;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		CTOP_CTRL_H14A0_RdFL(ctr79);

		if(pstParams->bEnable)
		{
			if(/*pstParam->u16Period == LX_OVI_SPREAD_SPECTRUM_DEFAULT_PERIOD*/1)	// ignore pstParam->u16Period
			{
				switch(pstParams->eSpreadRatio)
				{
					case LX_OVI_SPREAD_RATIO_1 :
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x3);
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0x17);
					break;
					case LX_OVI_SPREAD_RATIO_2 :
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x4);
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0x12);
					break;
					case LX_OVI_SPREAD_RATIO_4 :
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x7);
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0xa);
					break;
					case LX_OVI_SPREAD_RATIO_3 :
					default :
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x6);
						CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0xc);
					break;
				}
			}
			else
			{
				CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x6);
				CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0xc);
			}
		}
		else
		{
			CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_udex_ctrl, 0x0);
			CTOP_CTRL_H14A0_Wr01(ctr79, m1_dr3p_msex_ctrl, 0x0);
		}

		CTOP_CTRL_H14A0_WrFL(ctr79);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;
	int i;
	UINT32 addrOffset=0;
	UINT8 *u8odLutData = NULL;
	UINT32 u32odLutData;

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

		// select address
		if(pstParams->eMode == LX_OVI_TCON_FRAMEOD)
			addrOffset = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_odc_start_2d);
		else if(pstParams->eMode == LX_OVI_TCON_LINEOD)
			addrOffset = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_lodc_start_3d);

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
			OVI_TCON_H14_RdFL(tcon_odc_opt2);
			OVI_TCON_H14_Wr01(tcon_odc_opt2, odc_upload, 0x0);
			OVI_TCON_H14_WrFL(tcon_odc_opt2);
			OVI_TCON_H14_RdFL(tcon_odc_opt2);
			OVI_TCON_H14_Wr01(tcon_odc_opt2, odc_upload, 0x1);
			OVI_TCON_H14_WrFL(tcon_odc_opt2);
		}
		else if(pstParams->eMode == LX_OVI_TCON_LINEOD)
		{
			OVI_TCON_H14_RdFL(tcon_odc_opt2);
			OVI_TCON_H14_Wr01(tcon_odc_opt2, lodc_upload, 0x0);
			OVI_TCON_H14_WrFL(tcon_odc_opt2);
			OVI_TCON_H14_RdFL(tcon_odc_opt2);
			OVI_TCON_H14_Wr01(tcon_odc_opt2, lodc_upload, 0x1);
			OVI_TCON_H14_WrFL(tcon_odc_opt2);
		}
	} while(0);

	if(u8odLutData != NULL)
		OS_Free(u8odLutData);

	return ret;
}

int OVI_HW_H14_SetFrameOdcEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_H14_Wr01(tcon_control, odc_en, 0x1);
		else
			OVI_TCON_H14_Wr01(tcon_control, odc_en, 0x0);
		OVI_TCON_H14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_H14_SetColorTempEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_H14_Wr01(tcon_control, wb_en, 0x1);
		else
			OVI_TCON_H14_Wr01(tcon_control, wb_en, 0x0);
		OVI_TCON_H14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_H14_SetGammaEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_H14_Wr01(tcon_control, dga_en, 0x1);
		else
			OVI_TCON_H14_Wr01(tcon_control, dga_en, 0x0);
		OVI_TCON_H14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_H14_SetDitherEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_control);
		if(bParam)
			OVI_TCON_H14_Wr01(tcon_control, dither_en, 0x1);
		else
			OVI_TCON_H14_Wr01(tcon_control, dither_en, 0x0);
		OVI_TCON_H14_WrFL(tcon_control);
	} while (0);

	return ret;
}

int OVI_HW_H14_SetLineOdcMode(LX_OVI_TCON_LINEOD_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		// check structure pointer
		if(pstParams == NULL)
		{
			OVI_ERROR("[OVI] structure is pointed Null pointer!\n");
			ret = RET_ERROR;
			break;
		}

		OVI_TCON_H14_RdFL(tcon_odc_opt1);

		if(pstParams->eVersion == LX_OVI_TCON_LODC_V2_0)	//PCID version2.0
		{
			if(pstParams->eRefLastSubPix == LX_OVI_TCON_LODC_NORMAL_REF)
				OVI_TCON_H14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x0);
			else
				OVI_TCON_H14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x1);

			if(pstParams->eRefPrevLine == LX_OVI_TCON_LODC_LEFT_REF)
				OVI_TCON_H14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x0);
			else
				OVI_TCON_H14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x1);
			OVI_TCON_H14_Wr01(tcon_odc_opt1, pcid20_en, 0x1);
		}
		else if(pstParams->eVersion == LX_OVI_TCON_LODC_V1_0)	//PCID version1.0
		{
			OVI_TCON_H14_Wr01(tcon_odc_opt1, first_last_subpix_en, 0x0);
			OVI_TCON_H14_Wr01(tcon_odc_opt1, subpix_shift_right_en, 0x0);

			OVI_TCON_H14_Wr01(tcon_odc_opt1, pcid20_en, 0x0);
		}

		if(pstParams->bEnable)	//PCID enable/disable
			OVI_TCON_H14_Wr01(tcon_odc_opt1, lodc_en_3d, 0x1);
		else
			OVI_TCON_H14_Wr01(tcon_odc_opt1, lodc_en_3d, 0x0);

		OVI_TCON_H14_WrFL(tcon_odc_opt1);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetDitherMode(LX_OVI_TCON_DITHER_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_dither_carry);
		switch(enParam)
		{
			case LX_OVI_TCON_DITHER_TRUNC:
				OVI_TCON_H14_Wr(tcon_dither_carry, 0x0);
				break;
			case LX_OVI_TCON_DITHER_ROUND:
				OVI_TCON_H14_Wr(tcon_dither_carry, 0x1);
				break;
			case LX_OVI_TCON_DITHER_RANDOM:
			default :
				OVI_TCON_H14_Wr(tcon_dither_carry, 0x2);
				break;
		}
		OVI_TCON_H14_WrFL(tcon_dither_carry);
	} while(0);

	return ret;
}

UINT32 OVI_HW_H14_GetGain(UINT32 gain)
{
	UINT32 ret_gain;
	UINT32 temp;

	temp = 10000 + (gain - 192) * _gGainStep;
	ret_gain = (temp * 16384) / 10000;

	return ret_gain;
}

SINT16 OVI_HW_H14_GetOffset(UINT32 offset)
{
	SINT16 ret_offset;

	ret_offset = ((SINT32)offset - 64) * _gOffsetStep;
	return ret_offset;
}

int OVI_HW_H14_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams)
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

		r_gain = OVI_HW_H14_GetGain(pstParams->r_gain);
		g_gain = OVI_HW_H14_GetGain(pstParams->g_gain);
		b_gain = OVI_HW_H14_GetGain(pstParams->b_gain);

		r_offset = OVI_HW_H14_GetOffset(pstParams->r_offset);
		g_offset = OVI_HW_H14_GetOffset(pstParams->g_offset);
		b_offset = OVI_HW_H14_GetOffset(pstParams->b_offset);

		if((_gPrevRedGain == r_gain) && (_gPrevGreenGain == g_gain) && (_gPrevBlueGain == b_gain)
			&& (_gPrevRedOffset == r_offset) && (_gPrevGreenOffset == g_offset) && (_gPrevBlueOffset == b_offset))
		{
			return RET_OK;
		}

		// Set red gain
		OVI_TCON_H14_RdFL(tcon_wb_rgain);
		OVI_TCON_H14_Wr(tcon_wb_rgain, r_gain);
		OVI_TCON_H14_WrFL(tcon_wb_rgain);

		// Set red offset
		OVI_TCON_H14_RdFL(tcon_wb_roffset);
		OVI_TCON_H14_Wr(tcon_wb_roffset, r_offset);
		OVI_TCON_H14_WrFL(tcon_wb_roffset);

		// Set green gain
		OVI_TCON_H14_RdFL(tcon_wb_ggain);
		OVI_TCON_H14_Wr(tcon_wb_ggain, g_gain);
		OVI_TCON_H14_WrFL(tcon_wb_ggain);

		// Set green offset
		OVI_TCON_H14_RdFL(tcon_wb_goffset);
		OVI_TCON_H14_Wr(tcon_wb_goffset, g_offset);
		OVI_TCON_H14_WrFL(tcon_wb_goffset);

		// Set blue gain
		OVI_TCON_H14_RdFL(tcon_wb_bgain);
		OVI_TCON_H14_Wr(tcon_wb_bgain, b_gain);
		OVI_TCON_H14_WrFL(tcon_wb_bgain);

		// Set blue offset
		OVI_TCON_H14_RdFL(tcon_wb_boffset);
		OVI_TCON_H14_Wr(tcon_wb_boffset, b_offset);
		OVI_TCON_H14_WrFL(tcon_wb_boffset);

		_gPrevRedGain = r_gain;
		_gPrevGreenGain = g_gain;
		_gPrevBlueGain = b_gain;
		_gPrevRedOffset = r_offset;
		_gPrevGreenOffset = g_offset;
		_gPrevBlueOffset = b_offset;

	} while(0);

	return ret;
}

int OVI_HW_H14_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams)
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
		pGamma = OS_Malloc(OVI_HW_H14_TCON_GAMMA_TABLE_SIZENUM*4);
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_rs); //Red Gamma Start Address
			i=0, j=0, gamma_data=0;

			while(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_re)) //Red Gamma End Address
			{
				if(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_rs) + 255 * 4) //Red Gamma Medium Address((tcon_dga_med_re - tcon_dga_med_rs)/3)
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_rs);
			for(i=0; i<OVI_HW_H14_TCON_GAMMA_TABLE_SIZENUM; i++)
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_gs); //Green Gamma Start Address
			i=0, j=0, gamma_data=0;

			while(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_ge)) //Green Gamma End Address
			{
				if(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_gs) + 255 * 4) //Green Gamma Medium Address((tcon_dga_med_ge - tcon_dga_med_gs)/3)
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_gs);
			for(i=0; i<OVI_HW_H14_TCON_GAMMA_TABLE_SIZENUM; i++)
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_bs); //Blue Gamma Start Address
			i=0, j=0, gamma_data=0;

			while(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_be)) //Blue Gamma End Address
			{
				if(reg_addr_start <= OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_bs) + 255 * 4) //Blue Gamma Medium Address((tcon_dga_med_be - tcon_dga_med_bs)/3)
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

			reg_addr_start = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_dga_med_bs);
			for(i=0; i<OVI_HW_H14_TCON_GAMMA_TABLE_SIZENUM; i++)
			{
				OVI_REG_WR(reg_addr_start, pGamma[i]);
				reg_addr_start = reg_addr_start + 0x4;
			}
		}

		// gamma table upload
		OVI_TCON_H14_RdFL(tcon_dga_sel);
		OVI_TCON_H14_Wr01(tcon_dga_sel, dga_upload, 0x0);
		OVI_TCON_H14_WrFL(tcon_dga_sel);
		OVI_TCON_H14_RdFL(tcon_dga_sel);
		OVI_TCON_H14_Wr01(tcon_dga_sel, dga_upload, 0x1);
		OVI_TCON_H14_WrFL(tcon_dga_sel);
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

int OVI_HW_H14_GetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;
	UINT32 u32odcAddr=0;
	volatile UINT32 *pMappedAddr = NULL;

	do {
		switch(pstParams->eMode)
		{
			case LX_OVI_TCON_FRAMEOD:
				u32odcAddr = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_odc_start_2d);
				break;
			case LX_OVI_TCON_LINEOD:
				u32odcAddr = OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_lodc_start_3d);
				break;
			default:
				break;
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

int OVI_HW_H14_SetOdByteData(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams)
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

		switch(pstParams->mode)
		{
			case LX_OVI_TCON_FRAMEOD:
				u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_odc_start_2d);
				break;
			case LX_OVI_TCON_LINEOD:
				u32odcAddr = (pstParams->index * 4) + OVI_TCON_REG_H14_BASE + offsetof(OVI_TCON_REG_H14A0_T, tcon_lodc_start_3d);
				break;
			default :
				break;
		}

		OVI_REG_WR(u32odcAddr, pstParams->value);

		switch(pstParams->mode)
		{
			case LX_OVI_TCON_FRAMEOD:
				OVI_TCON_H14_RdFL(tcon_odc_opt2);
				OVI_TCON_H14_Wr01(tcon_odc_opt2, odc_upload, 0x1);
				OVI_TCON_H14_WrFL(tcon_odc_opt2);
				OVI_TCON_H14_RdFL(tcon_odc_opt2);
				OVI_TCON_H14_Wr01(tcon_odc_opt2, odc_upload, 0x0);
				OVI_TCON_H14_WrFL(tcon_odc_opt2);
				break;
			case LX_OVI_TCON_LINEOD:
				OVI_TCON_H14_RdFL(tcon_odc_opt2);
				OVI_TCON_H14_Wr01(tcon_odc_opt2, lodc_upload, 0x1);
				OVI_TCON_H14_WrFL(tcon_odc_opt2);
				OVI_TCON_H14_RdFL(tcon_odc_opt2);
				OVI_TCON_H14_Wr01(tcon_odc_opt2, lodc_upload, 0x0);
				OVI_TCON_H14_WrFL(tcon_odc_opt2);
				break;
			default:
				break;
		}
	} while(0);

	return ret;
}

int OVI_HW_H14_SetTconReverse(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_hor_mode);
		if(bParam) {
			OVI_TCON_H14_Wr01(tcon_hor_mode, hor_reverse, 0x0);
		}
		else {
			OVI_TCON_H14_Wr01(tcon_hor_mode, hor_reverse, 0x1);
		}
		OVI_TCON_H14_WrFL(tcon_hor_mode);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetLvdsReverse(BOOLEAN bParam)
{
	int ret = RET_OK;
	UINT32 lvdscheck = 0;

	do {
		OVI_OIF_H14_RdFL(oif_vch_config);
		OVI_OIF_H14_Rd01(oif_vch_config, vch_hb_width, lvdscheck);
		if(bParam) {
			OVI_OIF_H14_Wr01(oif_vch_config, vch_h_reverse, 0x1);
			OVI_OIF_H14_Wr01(oif_vch_config, vch_line_delay, 0x1);
		}
		else {
			OVI_OIF_H14_Wr01(oif_vch_config, vch_h_reverse, 0x0);
			if(lvdscheck != 4) // 60Hz LVDS condition
				OVI_OIF_H14_Wr01(oif_vch_config, vch_line_delay, 0x1);
			else
			OVI_OIF_H14_Wr01(oif_vch_config, vch_line_delay, 0x0);
		}
		OVI_OIF_H14_WrFL(oif_vch_config);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetPatternGenerator(LX_OVI_TCON_TPG_MODE_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_pgen_ctl);

		// pattern source color setting
		OVI_TCON_H14_RdFL(tcon_pgen_agpr1);
		OVI_TCON_H14_Wr(tcon_pgen_agpr1, 0x03FF);
		OVI_TCON_H14_WrFL(tcon_pgen_agpr1);

		OVI_TCON_H14_RdFL(tcon_pgen_agpg1);
		OVI_TCON_H14_Wr(tcon_pgen_agpg1, 0x03FF);
		OVI_TCON_H14_WrFL(tcon_pgen_agpg1);

		OVI_TCON_H14_RdFL(tcon_pgen_agpb1);
		OVI_TCON_H14_Wr(tcon_pgen_agpb1, 0x03FF);
		OVI_TCON_H14_WrFL(tcon_pgen_agpb1);

		switch(enParam)
		{
			case LX_OVI_TCON_TPG_OFF:		// pattern off
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x0);
				break;
			case LX_OVI_TCON_TPG_BLACK:		// black pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x4);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_WHITE:		// white pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HVBAR:		// HVBar pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x3);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR16GREY:		// Horizontal 16 grey pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x8);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR64GREY:		// Horizontal 64 grey pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x7);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR256GREY:		// Horizontal 256 grey pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x6);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_HOR1024GREY:		// Horizontal 1024 grey pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0x5);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
			case LX_OVI_TCON_TPG_VER16GREY:		// Vertical 16 grey pattern
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_mode_4_0, 0xC);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_reverse, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_sync_mode, 0x0);
				OVI_TCON_H14_Wr01(tcon_pgen_ctl, pgen_test, 0x1);
				break;
		}
		OVI_TCON_H14_WrFL(tcon_pgen_ctl);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetChannelPower(BOOLEAN bParam)
{
	int ret = RET_OK;
	UINT32 chPdbVal = 0, chOffVal = 0;

	do {
		CTOP_CTRL_H14A0_RdFL(ctr111);

		//get current pdb bit value
		chPdbVal = CTOP_CTRL_H14A0_Rd(ctr111) & 0x00FFFFFF;

		if(!bParam && (chPdbVal != 0x0)) // display output off
		{
			chOffVal = CTOP_CTRL_H14A0_Rd(ctr111) & 0xFF000000; //set only pdb bit to zero
			CTOP_CTRL_H14A0_Wr(ctr111, chOffVal);
			OVI_DEBUG("[OVI] display is changed to OFF status!\n");
		}
		else if(bParam && (chPdbVal == 0x0)) // display output on
		{
			CTOP_CTRL_H14A0_Wr(ctr111, _gOviHwH14Info.chPowerOnRegValue);
			OVI_DEBUG("[OVI] display is changed to ON status!\n");
		}
		else
		{
			OVI_DEBUG("[OVI] channel power status is not changed - same as current status!\n");
		}
		CTOP_CTRL_H14A0_WrFL(ctr111);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H14_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_LVDS_OUT_JEIDA:
				OVI_OIF_H14_Wr01(oif_lvds_config0, lvds_lsb_first, 0x0);
				break;
			case LX_OVI_LVDS_OUT_VESA:
				OVI_OIF_H14_Wr01(oif_lvds_config0, lvds_lsb_first, 0x1);
				break;
			default:
				break;
		}
		OVI_OIF_H14_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H14_RdFL(oif_lvds_config0);
		switch(enParam)
		{
			case LX_OVI_8BIT:
				OVI_OIF_H14_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x1);
				break;
			case LX_OVI_10BIT:
				OVI_OIF_H14_Wr01(oif_lvds_config0, lvds_8bit_mode, 0x0);
				break;
			default:
				break;
		}
		OVI_OIF_H14_WrFL(oif_lvds_config0);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetLvdsBlack(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_OIF_H14_RdFL(oif_lvds_config1);
		OVI_OIF_H14_RdFL(oif_lvds_config2);

		if(!bParam)	// original data out
		{
			OVI_OIF_H14_Wr01(oif_lvds_config1, lvds_force_even_data, 0x0);
			OVI_OIF_H14_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x0);
		}
		else			// black data out
		{
			OVI_OIF_H14_Wr01(oif_lvds_config1, lvds_force_even_data, 0x40000000);
			OVI_OIF_H14_Wr01(oif_lvds_config2, lvds_force_odd_data, 0x40000000);
		}

		OVI_OIF_H14_WrFL(oif_lvds_config1);
		OVI_OIF_H14_WrFL(oif_lvds_config2);
	} while(0);

	return ret;
}

int OVI_HW_H14_SetClock(BOOLEAN bParam)
{
	int ret = RET_OK;

	do {
		OVI_TCON_H14_RdFL(tcon_gclk_control);
		OVI_TCON_H14_RdFL(tcon_mclk_control);

		if(bParam)
		{
			OVI_TCON_H14_Wr01(tcon_gclk_control, gclk_en, 0x1);
			OVI_TCON_H14_Wr01(tcon_mclk_control, mclk_en, 0x1);
		}
		else
		{
			OVI_TCON_H14_Wr01(tcon_gclk_control, gclk_en, 0x0);
			OVI_TCON_H14_Wr01(tcon_mclk_control, mclk_en, 0x0);
		}

		OVI_TCON_H14_WrFL(tcon_gclk_control);
		OVI_TCON_H14_WrFL(tcon_mclk_control);
	} while(0);

	return ret;
}

/** @} */
