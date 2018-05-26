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
 * date       2010.04.15
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include "os_util.h"

#include "base_types.h"

#include "de_model.h"
#include "de_ver_def.h"

#ifdef USE_KDRV_CODES_FOR_H15
#include "de_def.h"
#include "de_drv.h"
#include "de_prm_def.h"

#ifndef _H15_FPGA_TEMPORAL_
#include "de_cvi_reg_h15.h"
#include "de_msc_reg_h15.h"
#include "de_ssc_reg_h15.h"
#include "de_ctr_reg_h15.h"
#include "de_pe1_reg_h15.h"
#include "de_dpp_reg_h15.h"
#endif

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
#ifndef _H15_FPGA_TEMPORAL_
extern DE_CVA_REG_H15_T	 gDE_CVA_H15;
extern DE_CVB_REG_H15_T	 gDE_CVB_H15;
extern DE_MSL_REG_H15_T	 gDE_MSL_H15;
extern DE_SSC_REG_H15_T	 gDE_SSC_H15;
extern DE_DEC_REG_H15_T  gDE_DEC_H15;
extern DE_P1L_REG_H15_T  gDE_P1L_H15;
extern DE_DPP_REG_H15_T  gDE_DPP_H15;
#endif

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
static int DE_PRM_H15_LoadCvmFirCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadCvsFirCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadCvmCscCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadCvsCscCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadMscrFirYCoef(    DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadMscrFirCCoef(    DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadSscrFirYCoef(    DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadMscrPhaseOffset( DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadSscrPhaseOffset( DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadDlrCoef(         DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadMdmCscCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadPostCscCoef(     DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
#if 0
static int DE_PRM_H15_LoadCvmHdnCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadMdmHdnCoef(      DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
static int DE_PRM_H15_LoadDlrPhase(        DE_PARAM_WR_T  bWrite, UINT32   index, UINT32 *pTable, UINT32 size );
#endif

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static DE_TABLE_PARAM_T saVideoTable_H15[] = {
    { DE_CVM_FIR_COEF      ,DE_PRM_H15_LoadCvmFirCoef      },
    { DE_CVS_FIR_COEF      ,DE_PRM_H15_LoadCvsFirCoef      },
    { DE_CVM_CSC_COEF      ,DE_PRM_H15_LoadCvmCscCoef      },
    { DE_CVS_CSC_COEF      ,DE_PRM_H15_LoadCvsCscCoef      },
    { DE_MSCR_FIRY_COEF    ,DE_PRM_H15_LoadMscrFirYCoef    },
    { DE_MSCR_FIRC_COEF    ,DE_PRM_H15_LoadMscrFirCCoef    },
    { DE_SSCR_FIRY_COEF    ,DE_PRM_H15_LoadSscrFirYCoef    },
    { DE_MSCR_PHASE_OFFSET ,DE_PRM_H15_LoadMscrPhaseOffset },
    { DE_SSCR_PHASE_OFFSET ,DE_PRM_H15_LoadSscrPhaseOffset },
    { DE_DLR_QX_COEF       ,DE_PRM_H15_LoadDlrCoef         },
    { DE_MDM_CSC_COEF      ,DE_PRM_H15_LoadMdmCscCoef      },
    { DE_POST_CSC_COEF     ,DE_PRM_H15_LoadPostCscCoef     },
#if 0
    { DE_CVM_HDN_COEF      ,DE_PRM_H15_LoadCvmHdnCoef      },
    { DE_MDM_HDN_COEF      ,DE_PRM_H15_LoadMdmHdnCoef      },
    { DE_DLR_PHASE_COEF    ,DE_PRM_H15_LoadDlrPhase        },
#endif
};

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
int DE_PRM_H15_LoadTable(DE_PARAM_TYPE_T tableId, DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 writeSize)
{
    int ret = RET_ERROR;

    do {
        CHECK_KNULL(pTable);
        if (tableId >= ARRAY_SIZE(saVideoTable_H15)) BREAK_WRONG(tableId);
        ret = saVideoTable_H15[tableId].func(bWrite, index, pTable, writeSize);
    } while (0);

    return ret;
}

static int DE_PRM_H15_LoadCvmFirCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {

            DE_CVA_H15_Wr01(cva_dig_filt_ctrl0 ,fir_coefficient0 ,pTable[0]);
            DE_CVA_H15_Wr01(cva_dig_filt_ctrl0 ,fir_coefficient1 ,pTable[1]);
            DE_CVA_H15_Wr01(cva_dig_filt_ctrl1 ,fir_coefficient2 ,pTable[2]);
            DE_CVA_H15_Wr01(cva_dig_filt_ctrl1 ,fir_coefficient3 ,pTable[3]);
            DE_CVA_H15_Wr01(cva_dig_filt_ctrl2 ,fir_coefficient4 ,pTable[4]);
            DE_CVA_H15_Wr01(cva_dig_filt_ctrl2 ,fir_coefficient5 ,pTable[5]);

            DE_CVA_H15_WrFL(cva_dig_filt_ctrl0);
            DE_CVA_H15_WrFL(cva_dig_filt_ctrl1);
            DE_CVA_H15_WrFL(cva_dig_filt_ctrl2);

			DE_CVA_H15_RdFL(cva_dig_filt_ctrl3);
			DE_CVA_H15_Wr01(cva_dig_filt_ctrl3 ,fir_coeff_load   ,TRUE);
			DE_CVA_H15_Wr01(cva_dig_filt_ctrl3 ,fir_en           ,(index)?TRUE:FALSE);
			DE_CVA_H15_WrFL(cva_dig_filt_ctrl3);
        } else {
            DE_CVA_H15_RdFL(cva_dig_filt_ctrl0);
            DE_CVA_H15_RdFL(cva_dig_filt_ctrl1);
            DE_CVA_H15_RdFL(cva_dig_filt_ctrl2);

            DE_CVA_H15_Rd01(cva_dig_filt_ctrl0 ,fir_coefficient0 ,pTable[0]);
            DE_CVA_H15_Rd01(cva_dig_filt_ctrl0 ,fir_coefficient1 ,pTable[1]);
            DE_CVA_H15_Rd01(cva_dig_filt_ctrl1 ,fir_coefficient2 ,pTable[2]);
            DE_CVA_H15_Rd01(cva_dig_filt_ctrl1 ,fir_coefficient3 ,pTable[3]);
            DE_CVA_H15_Rd01(cva_dig_filt_ctrl2 ,fir_coefficient4 ,pTable[4]);
            DE_CVA_H15_Rd01(cva_dig_filt_ctrl2 ,fir_coefficient5 ,pTable[5]);
        }

    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadCvsFirCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_CVB_H15_RdFL(cvb_dig_filt_ctrl3);

            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl0 ,fir_coefficient0 ,pTable[0]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl0 ,fir_coefficient1 ,pTable[1]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl1 ,fir_coefficient2 ,pTable[2]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl1 ,fir_coefficient3 ,pTable[3]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl2 ,fir_coefficient4 ,pTable[4]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl2 ,fir_coefficient5 ,pTable[5]);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl3 ,fir_coeff_load   ,TRUE);
            DE_CVB_H15_Wr01(cvb_dig_filt_ctrl3 ,fir_en           ,(index)?TRUE:FALSE);

            DE_CVB_H15_WrFL(cvb_dig_filt_ctrl0);
            DE_CVB_H15_WrFL(cvb_dig_filt_ctrl1);
            DE_CVB_H15_WrFL(cvb_dig_filt_ctrl2);
            DE_CVB_H15_WrFL(cvb_dig_filt_ctrl3);
        } else {
            DE_CVB_H15_RdFL(cvb_dig_filt_ctrl0);
            DE_CVB_H15_RdFL(cvb_dig_filt_ctrl1);
            DE_CVB_H15_RdFL(cvb_dig_filt_ctrl2);

            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl0 ,fir_coefficient0 ,pTable[0]);
            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl0 ,fir_coefficient1 ,pTable[1]);
            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl1 ,fir_coefficient2 ,pTable[2]);
            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl1 ,fir_coefficient3 ,pTable[3]);
            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl2 ,fir_coefficient4 ,pTable[4]);
            DE_CVB_H15_Rd01(cvb_dig_filt_ctrl2 ,fir_coefficient5 ,pTable[5]);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadCvmCscCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_CVA_H15_RdFL(cva_csc_ctrl7);

            DE_CVA_H15_Wr01(cva_csc_ctrl0 ,csc_coefficient0 ,pTable[0]);
            DE_CVA_H15_Wr01(cva_csc_ctrl0 ,csc_coefficient1 ,pTable[1]);
            DE_CVA_H15_Wr01(cva_csc_ctrl1 ,csc_coefficient2 ,pTable[2]);
            DE_CVA_H15_Wr01(cva_csc_ctrl1 ,csc_coefficient3 ,pTable[3]);
            DE_CVA_H15_Wr01(cva_csc_ctrl2 ,csc_coefficient4 ,pTable[4]);
            DE_CVA_H15_Wr01(cva_csc_ctrl2 ,csc_coefficient5 ,pTable[5]);
            DE_CVA_H15_Wr01(cva_csc_ctrl3 ,csc_coefficient6 ,pTable[6]);
            DE_CVA_H15_Wr01(cva_csc_ctrl3 ,csc_coefficient7 ,pTable[7]);
            DE_CVA_H15_Wr01(cva_csc_ctrl4 ,csc_coefficient8 ,pTable[8]);
            DE_CVA_H15_Wr01(cva_csc_ctrl4 ,csc_offset0      ,pTable[9]);
            DE_CVA_H15_Wr01(cva_csc_ctrl5 ,csc_offset1      ,pTable[10]);
            DE_CVA_H15_Wr01(cva_csc_ctrl5 ,csc_offset2      ,pTable[11]);
            DE_CVA_H15_Wr01(cva_csc_ctrl6 ,csc_offset3      ,pTable[12]);
            DE_CVA_H15_Wr01(cva_csc_ctrl6 ,csc_offset4      ,pTable[13]);
            DE_CVA_H15_Wr01(cva_csc_ctrl7 ,csc_offset5      ,pTable[14]);
            DE_CVA_H15_Wr01(cva_csc_ctrl7 ,csc_coeff_load   ,TRUE);
            DE_CVA_H15_Wr01(cva_csc_ctrl7 ,csc_en           ,(index)?TRUE:FALSE);

            DE_CVA_H15_WrFL(cva_csc_ctrl0);
            DE_CVA_H15_WrFL(cva_csc_ctrl1);
            DE_CVA_H15_WrFL(cva_csc_ctrl2);
            DE_CVA_H15_WrFL(cva_csc_ctrl3);
            DE_CVA_H15_WrFL(cva_csc_ctrl4);
            DE_CVA_H15_WrFL(cva_csc_ctrl5);
            DE_CVA_H15_WrFL(cva_csc_ctrl6);
            DE_CVA_H15_WrFL(cva_csc_ctrl7);
        } else {
            DE_CVA_H15_RdFL(cva_csc_ctrl0);
            DE_CVA_H15_RdFL(cva_csc_ctrl1);
            DE_CVA_H15_RdFL(cva_csc_ctrl2);
            DE_CVA_H15_RdFL(cva_csc_ctrl3);
            DE_CVA_H15_RdFL(cva_csc_ctrl4);
            DE_CVA_H15_RdFL(cva_csc_ctrl5);
            DE_CVA_H15_RdFL(cva_csc_ctrl6);
            DE_CVA_H15_RdFL(cva_csc_ctrl7);

            DE_CVA_H15_Rd01(cva_csc_ctrl0 ,csc_coefficient0 ,pTable[0]);
            DE_CVA_H15_Rd01(cva_csc_ctrl0 ,csc_coefficient1 ,pTable[1]);
            DE_CVA_H15_Rd01(cva_csc_ctrl1 ,csc_coefficient2 ,pTable[2]);
            DE_CVA_H15_Rd01(cva_csc_ctrl1 ,csc_coefficient3 ,pTable[3]);
            DE_CVA_H15_Rd01(cva_csc_ctrl2 ,csc_coefficient4 ,pTable[4]);
            DE_CVA_H15_Rd01(cva_csc_ctrl2 ,csc_coefficient5 ,pTable[5]);
            DE_CVA_H15_Rd01(cva_csc_ctrl3 ,csc_coefficient6 ,pTable[6]);
            DE_CVA_H15_Rd01(cva_csc_ctrl3 ,csc_coefficient7 ,pTable[7]);
            DE_CVA_H15_Rd01(cva_csc_ctrl4 ,csc_coefficient8 ,pTable[8]);
            DE_CVA_H15_Rd01(cva_csc_ctrl4 ,csc_offset0      ,pTable[9]);
            DE_CVA_H15_Rd01(cva_csc_ctrl5 ,csc_offset1      ,pTable[10]);
            DE_CVA_H15_Rd01(cva_csc_ctrl5 ,csc_offset2      ,pTable[11]);
            DE_CVA_H15_Rd01(cva_csc_ctrl6 ,csc_offset3      ,pTable[12]);
            DE_CVA_H15_Rd01(cva_csc_ctrl6 ,csc_offset4      ,pTable[13]);
            DE_CVA_H15_Rd01(cva_csc_ctrl7 ,csc_offset5      ,pTable[14]);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadCvsCscCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_CVB_H15_RdFL(cvb_csc_ctrl7);

            DE_CVB_H15_Wr01(cvb_csc_ctrl0 ,csc_coefficient0 ,pTable[0]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl0 ,csc_coefficient1 ,pTable[1]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl1 ,csc_coefficient2 ,pTable[2]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl1 ,csc_coefficient3 ,pTable[3]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl2 ,csc_coefficient4 ,pTable[4]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl2 ,csc_coefficient5 ,pTable[5]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl3 ,csc_coefficient6 ,pTable[6]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl3 ,csc_coefficient7 ,pTable[7]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl4 ,csc_coefficient8 ,pTable[8]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl4 ,csc_offset0      ,pTable[9]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl5 ,csc_offset1      ,pTable[10]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl5 ,csc_offset2      ,pTable[11]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl6 ,csc_offset3      ,pTable[12]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl6 ,csc_offset4      ,pTable[13]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl7 ,csc_offset5      ,pTable[14]);
            DE_CVB_H15_Wr01(cvb_csc_ctrl7 ,csc_coeff_load   ,TRUE);
            DE_CVB_H15_Wr01(cvb_csc_ctrl7 ,csc_en           ,(index)?TRUE:FALSE);

            DE_CVB_H15_WrFL(cvb_csc_ctrl0);
            DE_CVB_H15_WrFL(cvb_csc_ctrl1);
            DE_CVB_H15_WrFL(cvb_csc_ctrl2);
            DE_CVB_H15_WrFL(cvb_csc_ctrl3);
            DE_CVB_H15_WrFL(cvb_csc_ctrl4);
            DE_CVB_H15_WrFL(cvb_csc_ctrl5);
            DE_CVB_H15_WrFL(cvb_csc_ctrl6);
            DE_CVB_H15_WrFL(cvb_csc_ctrl7);
        } else {
            DE_CVB_H15_RdFL(cvb_csc_ctrl0);
            DE_CVB_H15_RdFL(cvb_csc_ctrl1);
            DE_CVB_H15_RdFL(cvb_csc_ctrl2);
            DE_CVB_H15_RdFL(cvb_csc_ctrl3);
            DE_CVB_H15_RdFL(cvb_csc_ctrl4);
            DE_CVB_H15_RdFL(cvb_csc_ctrl5);
            DE_CVB_H15_RdFL(cvb_csc_ctrl6);
            DE_CVB_H15_RdFL(cvb_csc_ctrl7);

            DE_CVB_H15_Rd01(cvb_csc_ctrl0 ,csc_coefficient0 ,pTable[0]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl0 ,csc_coefficient1 ,pTable[1]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl1 ,csc_coefficient2 ,pTable[2]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl1 ,csc_coefficient3 ,pTable[3]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl2 ,csc_coefficient4 ,pTable[4]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl2 ,csc_coefficient5 ,pTable[5]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl3 ,csc_coefficient6 ,pTable[6]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl3 ,csc_coefficient7 ,pTable[7]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl4 ,csc_coefficient8 ,pTable[8]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl4 ,csc_offset0      ,pTable[9]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl5 ,csc_offset1      ,pTable[10]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl5 ,csc_offset2      ,pTable[11]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl6 ,csc_offset3      ,pTable[12]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl6 ,csc_offset4      ,pTable[13]);
            DE_CVB_H15_Rd01(cvb_csc_ctrl7 ,csc_offset5      ,pTable[14]);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadMscrFirYCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        DE_MSL_H15_RdFL(msc_y_fir_coef_ctrl_l);
        DE_MSL_H15_Wr01(msc_y_fir_coef_ctrl_l ,addr    ,GET_BITS(index, 0, 4));
        DE_MSL_H15_Wr01(msc_y_fir_coef_ctrl_l ,h_v_sel ,GET_BITS(index, 4, 1));
        DE_MSL_H15_Wr01(msc_y_fir_coef_ctrl_l ,update  ,1);
        if (bWrite) {
            DE_MSL_H15_FLWr(msc_y_fir_coef_data0_l, pTable[0]);
            DE_MSL_H15_FLWr(msc_y_fir_coef_data1_l, pTable[1]);
            DE_MSL_H15_FLWr(msc_y_fir_coef_data2_l, pTable[2]);
            DE_MSL_H15_FLWr(msc_y_fir_coef_data3_l, pTable[3]);
            do {
                if (size == 4) break;
                DE_MSL_H15_FLWr(msc_y_fir_coef_data4_l, pTable[4]);
                DE_MSL_H15_FLWr(msc_y_fir_coef_data5_l, pTable[5]);
            } while (0);
            DE_MSL_H15_Wr01(msc_y_fir_coef_ctrl_l ,rnw     ,0);
            DE_MSL_H15_WrFL(msc_y_fir_coef_ctrl_l);
        } else {
            DE_MSL_H15_Wr01(msc_y_fir_coef_ctrl_l ,rnw     ,1);
            DE_MSL_H15_WrFL(msc_y_fir_coef_ctrl_l);

            DE_MSL_H15_FLRd(msc_y_fir_coef_data0_l, pTable[0]);
            DE_MSL_H15_FLRd(msc_y_fir_coef_data1_l, pTable[1]);
            DE_MSL_H15_FLRd(msc_y_fir_coef_data2_l, pTable[2]);
            DE_MSL_H15_FLRd(msc_y_fir_coef_data3_l, pTable[3]);
            do {
                if (size == 4) break;
                DE_MSL_H15_FLRd(msc_y_fir_coef_data4_l, pTable[4]);
                DE_MSL_H15_FLRd(msc_y_fir_coef_data5_l, pTable[5]);
            } while (0);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadMscrFirCCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        DE_MSL_H15_RdFL(msc_c_fir_coef_ctrl_l);
        DE_MSL_H15_Wr01(msc_c_fir_coef_ctrl_l ,addr    ,GET_BITS(index, 0, 4));
        DE_MSL_H15_Wr01(msc_c_fir_coef_ctrl_l ,h_v_sel ,GET_BITS(index, 4, 1));
        DE_MSL_H15_Wr01(msc_c_fir_coef_ctrl_l ,update  ,1);
        if (bWrite) {
            DE_MSL_H15_FLWr(msc_c_fir_coef_data0_l, pTable[0]);
            DE_MSL_H15_FLWr(msc_c_fir_coef_data1_l, pTable[1]);
            DE_MSL_H15_FLWr(msc_c_fir_coef_data2_l, pTable[2]);
            DE_MSL_H15_FLWr(msc_c_fir_coef_data3_l, pTable[3]);

            DE_MSL_H15_Wr01(msc_c_fir_coef_ctrl_l ,rnw ,0);
            DE_MSL_H15_WrFL(msc_c_fir_coef_ctrl_l);
        } else {
            DE_MSL_H15_Wr01(msc_c_fir_coef_ctrl_l ,rnw ,1);
            DE_MSL_H15_WrFL(msc_c_fir_coef_ctrl_l);

            DE_MSL_H15_FLRd(msc_c_fir_coef_data0_l, pTable[0]);
            DE_MSL_H15_FLRd(msc_c_fir_coef_data1_l, pTable[1]);
            DE_MSL_H15_FLRd(msc_c_fir_coef_data2_l, pTable[2]);
            DE_MSL_H15_FLRd(msc_c_fir_coef_data3_l, pTable[3]);
        }
    } while (0);
#endif
    return ret;

}

static int DE_PRM_H15_LoadSscrFirYCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        DE_SSC_H15_RdFL(ssc_h_fir_coef_ctrl);
        DE_SSC_H15_Wr01(ssc_h_fir_coef_ctrl ,addr    ,GET_BITS(index, 0, 4));
        DE_SSC_H15_Wr01(ssc_h_fir_coef_ctrl ,h_v_sel ,GET_BITS(index, 4, 1));
        DE_SSC_H15_Wr01(ssc_h_fir_coef_ctrl ,update  ,1);
        if (bWrite) {
            DE_SSC_H15_FLWr(ssc_h_fir_coef_data0, pTable[0]);
            DE_SSC_H15_FLWr(ssc_h_fir_coef_data1, pTable[1]);
            do {
                if (size == 2) break;
                DE_SSC_H15_FLWr(ssc_h_fir_coef_data2, pTable[2]);
                DE_SSC_H15_FLWr(ssc_h_fir_coef_data3, pTable[3]);
            } while (0);

            DE_SSC_H15_Wr01(ssc_h_fir_coef_ctrl ,rnw     ,0);
            DE_SSC_H15_WrFL(ssc_h_fir_coef_ctrl);
        } else {
            DE_SSC_H15_Wr01(ssc_h_fir_coef_ctrl ,rnw     ,1);
            DE_SSC_H15_WrFL(ssc_h_fir_coef_ctrl);

            DE_SSC_H15_FLRd(ssc_h_fir_coef_data0, pTable[0]);
            DE_SSC_H15_FLRd(ssc_h_fir_coef_data1, pTable[1]);

            do {
                if (size == 2) break;
                DE_SSC_H15_FLRd(ssc_h_fir_coef_data0, pTable[2]);
                DE_SSC_H15_FLRd(ssc_h_fir_coef_data1, pTable[3]);
            } while (0);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadMscrPhaseOffset(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_MSL_H15_FLWr(msc_y_init_pos_h_l, pTable[0]);
            DE_MSL_H15_FLWr(msc_y_init_pos_v_l, pTable[1]);
            DE_MSL_H15_FLWr(msc_c_init_pos_h_l, pTable[2]);
            DE_MSL_H15_FLWr(msc_c_init_pos_v_l, pTable[3]);
        } else {
            DE_MSL_H15_FLRd(msc_y_init_pos_h_l, pTable[0]);
            DE_MSL_H15_FLRd(msc_y_init_pos_v_l, pTable[1]);
            DE_MSL_H15_FLRd(msc_c_init_pos_h_l, pTable[2]);
            DE_MSL_H15_FLRd(msc_c_init_pos_v_l, pTable[3]);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadSscrPhaseOffset(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_SSC_H15_FLWr(ssc_y_init_pos_h, pTable[0]);
            DE_SSC_H15_FLWr(ssc_y_init_pos_v, pTable[1]);
            DE_SSC_H15_FLWr(ssc_c_init_pos_h, pTable[2]);
            DE_SSC_H15_FLWr(ssc_c_init_pos_v, pTable[3]);
        } else {
            DE_SSC_H15_FLRd(ssc_y_init_pos_h, pTable[0]);
            DE_SSC_H15_FLRd(ssc_y_init_pos_v, pTable[1]);
            DE_SSC_H15_FLRd(ssc_c_init_pos_h, pTable[2]);
            DE_SSC_H15_FLRd(ssc_c_init_pos_v, pTable[3]);
        }
    } while (0);
#endif
    return ret;
}

static int DE_PRM_H15_LoadDlrCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

	do {
		CHECK_KNULL(pTable);
    } while (0);

    return ret;
}

static int DE_PRM_H15_LoadMdmCscCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

    return ret;
}

static int DE_PRM_H15_LoadPostCscCoef(DE_PARAM_WR_T bWrite, UINT32 index, UINT32 *pTable, UINT32 size)
{
    int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
    do {
        CHECK_KNULL(pTable);
        if (bWrite) {
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_07);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_00, r_csc_coef0, pTable[0]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_00, r_csc_coef1, pTable[1]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_01, r_csc_coef2, pTable[2]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_01, r_csc_coef3, pTable[3]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_02, r_csc_coef4, pTable[4]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_02, r_csc_coef5, pTable[5]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_03, r_csc_coef6, pTable[6]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_03, r_csc_coef7, pTable[7]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_04, r_csc_coef8, pTable[8]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_04, r_csc_ofst0, pTable[9]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_05, r_csc_ofst1, pTable[10]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_05, r_csc_ofst2, pTable[11]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_06, r_csc_ofst3, pTable[12]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_06, r_csc_ofst4, pTable[13]);
            DE_DPP_H15_Wr01(debd_pp_l_csc_ctrl_07, r_csc_ofst5, pTable[14]);

            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_00);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_01);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_02);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_03);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_04);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_05);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_06);
            DE_DPP_H15_WrFL(debd_pp_l_csc_ctrl_07);
        } else {
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_00);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_01);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_02);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_03);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_04);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_05);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_06);
            DE_DPP_H15_RdFL(debd_pp_l_csc_ctrl_07);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_00, r_csc_coef0, pTable[0]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_00, r_csc_coef1, pTable[1]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_01, r_csc_coef2, pTable[2]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_01, r_csc_coef3, pTable[3]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_02, r_csc_coef4, pTable[4]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_02, r_csc_coef5, pTable[5]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_03, r_csc_coef6, pTable[6]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_03, r_csc_coef7, pTable[7]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_04, r_csc_coef8, pTable[8]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_04, r_csc_ofst0, pTable[9]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_05, r_csc_ofst1, pTable[10]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_05, r_csc_ofst2, pTable[11]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_06, r_csc_ofst3, pTable[12]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_06, r_csc_ofst4, pTable[13]);
            DE_DPP_H15_Rd01(debd_pp_l_csc_ctrl_07, r_csc_ofst5, pTable[14]);
        }
    } while (0);
#endif
    return ret;
}
#endif

/**  @} */
