//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   str_tdp.c
/// @brief  TSP Suspend-to-RAM (STR) related code
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../hal/mustang/tsp/halTSP.h"
#include "drvTSP.h"
#include "apiDMX.h"

#include "mst_str.h"
#include "str_tsp.h"

#define TSP_STR_DBG_ENABLE             0
#define TSP_FW_DRAM_BUFF_SIZE       0x40000

#if TSP_STR_DBG_ENABLE
#define DBG_TSP_STR(_f)               (_f)
#define DBG_TSP_STR_ERR(_f)           (_f)
#define DBG_TSP_STR_ENTRY(_f)         (_f)
#define DBG_TSP_STR_INFO(_f)          (_f)
#define DBG_TSP_STR_EXIT(_f)          (_f)
#else
#define DBG_TSP_STR(_f)               (_f)
#define DBG_TSP_STR_ERR(_f)           (_f)
#define DBG_TSP_STR_ENTRY(_f)
#define DBG_TSP_STR_INFO(_f)
#define DBG_TSP_STR_EXIT(_f)
#endif

static void* uInstantDmx = NULL;
static void* uAttributeDmx = NULL;

static MS_U8 _tsp_fw_dat[] =
{
   #include "fwTSP.dat"
};
DrvTSP_If_Set strIfSet_0,strIfSet_1,strIfSet_FI;
ST_VQ_INFO    stVqInfo_0,stVqInfo_1,stVqInfo_2,stVqInfo_3,stVqInfo_4,stVqInfo_5;

extern int suspend_tsp(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("[TSP STR Enter]\n");

    //application_tsp_init();//only for test
    TspSavePhaseTuning();
    TspSaveVqInfo();
    TspSaveCaCtrlInfo();
    TspSaveTsifInfo();

    tsp_utopia_suspend();

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("Function Exit\n");

    return 0;
}

extern int resume_tsp(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("[TSP STR Enter]\n");

    tsp_utopia_resume();
    TspLoadTsifInfo();
    TspLoadCaCtrlInfo();
    TspLoadVqInfo();
    TspLoadPhaseTuning();

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("Function Exit\n");

    return 0;
}
DMX_FILTER_STATUS tsp_utopia_suspend(void)
{
    int u8DmxIdIndex = 0;
    int u8DmxMaxSecFilters = 0;
    int u8DmxMaxPidFilters = 0;
    MS_U8 u8DmxPidFiltersMap[128] = {0};
    MS_U8 u8DmxSecFiltersMap[128] = {0};

    memset(u8DmxPidFiltersMap, 0x0, sizeof(MS_U8)*128);
    memset(u8DmxSecFiltersMap, 0x0, sizeof(MS_U8)*128);

    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    //suspend tsif info
    MDrv_TSP_GetTSIFStatus(E_DRVTSP_IF_TS0, &strIfSet_0);
    MDrv_TSP_GetTSIFStatus(E_DRVTSP_IF_TS1, &strIfSet_1);
    MDrv_TSP_GetTSIFStatus(E_DRVTSP_IF_FI, &strIfSet_FI);

    if( Utopia_MApi_Dmx_UtopiaOpen() != TRUE )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_Dmx_UtopiaOpen() != TRUE.\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM,(void*)&u8DmxMaxSecFilters) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM) %d Fail. \n", __FUNCTION__, __LINE__, u8DmxMaxSecFilters));
        return DMX_FILTER_STATUS_ERROR;
    }
    DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxMaxSecFilters = %d.\n",__FUNCTION__, __LINE__, u8DmxMaxSecFilters));

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM,(void*)&u8DmxMaxPidFilters) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM) %d Fail. \n", __FUNCTION__, __LINE__, u8DmxMaxPidFilters));
        return DMX_FILTER_STATUS_ERROR;
    }
    DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxMaxPidFilters = %d.\n",__FUNCTION__, __LINE__, u8DmxMaxPidFilters));

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_PIDFLT_MAP,(void*)&u8DmxPidFiltersMap) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_PIDFLT_MAP) Fail. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
    //for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxPidFilters; u8DmxIdIndex++ )
    //{
    //    if( u8DmxPidFiltersMap[u8DmxIdIndex] != 0xFF )
    //    {
    //        DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxPidFiltersMap[%d]=(%d).\n",__FUNCTION__, __LINE__, u8DmxIdIndex,u8DmxPidFiltersMap[u8DmxIdIndex]));
    //    }
    //}

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_SECFLT_MAP,(void*)&u8DmxSecFiltersMap) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SECFLT_MAP) Fail. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
    //for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxSecFilters; u8DmxIdIndex++ )
    //{
    //    if( u8DmxSecFiltersMap[u8DmxIdIndex] != 0xFF )
    //    {
    //        DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxSecFiltersMap[%d]=(%d).\n",__FUNCTION__, __LINE__, u8DmxIdIndex,u8DmxSecFiltersMap[u8DmxIdIndex]));
    //    }
    //}

    for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxPidFilters; u8DmxIdIndex++ )
    {
         if( u8DmxPidFiltersMap[u8DmxIdIndex] != 0xFF )
         {
             DBG_TSP_STR(printk("[TSP STR][%s][%d] (%d) Utopia_MApi_DMX_Stop(%d).\n",__FUNCTION__, __LINE__, u8DmxPidFiltersMap[u8DmxIdIndex], u8DmxIdIndex));

             if( Utopia_MApi_DMX_Stop(u8DmxIdIndex) != DMX_FILTER_STATUS_OK )
             {
                 DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_Stop(%d) Fail. \n", __FUNCTION__, __LINE__,u8DmxIdIndex));
                return DMX_FILTER_STATUS_ERROR;
             }
         }

		 if( u8DmxIdIndex < u8DmxMaxSecFilters )
		 {
		    if( u8DmxSecFiltersMap[u8DmxIdIndex] != 0xFF )
	        {
                DBG_TSP_STR(printk("[TSP STR][%s][%d] (%d) Utopia_MApi_DMX_SectReset(%d).\n",__FUNCTION__, __LINE__, u8DmxSecFiltersMap[u8DmxIdIndex], u8DmxIdIndex));

	        if( Utopia_MApi_DMX_SectReset(u8DmxIdIndex) != DMX_FILTER_STATUS_OK )
	        {
	            DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_SectReset(%d) Fail. \n", __FUNCTION__, __LINE__, u8DmxIdIndex));
	            return DMX_FILTER_STATUS_ERROR;
	        }
	     }
    }
    }

    if( Utopia_MApi_DMX_SetPowerState(E_POWER_SUSPEND,0,0) != DMX_FILTER_STATUS_OK )
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if( Utopia_MApi_DMX_UtopiaClose() != TRUE )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_UtopiaClose() != TRUE.\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS tsp_utopia_resume(void)
{
    int u8DmxIdIndex = 0;
    int u8DmxMaxPidFilters = 0 ;
    //int u8DmxMaxSecFilters  = 0 ;
    MS_U8 u8DmxPidFiltersMap[128] = {0};
    MS_U8 u8DmxSecFiltersMap[128] = {0};

    MS_U32  u32FwBinPA = 0;
    MS_U8*  u8BinVA = NULL;

    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( Utopia_MApi_Dmx_UtopiaOpen() != TRUE )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_Dmx_UtopiaOpen() != TRUE.\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_FW_BUF_ADDR,(void*)&u32FwBinPA) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_FW_BUF_ADDR) 0x%lx Fail. \n", __FUNCTION__, __LINE__, u32FwBinPA));
        return DMX_FILTER_STATUS_ERROR;
    }
    DBG_TSP_STR(printk("[TSP STR ] TSP u32FwBinPA is 0x%lx\n",u32FwBinPA));

    if( u32FwBinPA == 0 )
    {
        u32FwBinPA = BIN_MEM_ADR;
    }

    MsOS_MPool_Mapping_Dynamic(0,u32FwBinPA,BIN_MEM_LEN,TRUE);

    u8BinVA = (MS_U8*)MsOS_MPool_PA2KSEG1(u32FwBinPA);
    if( NULL == u8BinVA )
    {
        DBG_TSP_STR_ERR(printk("[%s][%d] Fail to convert PA to VA. \n",__FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_INFO(printk("[%s][%d] u8BinVA = (%lx). \n",__FUNCTION__, __LINE__, (MS_U32)u8BinVA));
    }

    memset(u8BinVA, 0, sizeof(_tsp_fw_dat));
    memcpy(u8BinVA, _tsp_fw_dat, sizeof(_tsp_fw_dat));

    DBG_TSP_STR_INFO(printk("FW size = 0x%x \n", sizeof(_tsp_fw_dat)));
    DBG_TSP_STR_INFO(printk("bin_mem = 0x%lx \n", (MS_U32)u8BinVA));

    Utopia_MApi_DMX_SetPowerState(E_POWER_RESUME,u32FwBinPA,sizeof(_tsp_fw_dat));

	Utopia_MApi_DMX_SetFwDataAddr(u32FwBinPA,TSP_FW_DRAM_BUFF_SIZE);

    if( Utopia_MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM,(void*)&u8DmxMaxPidFilters) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM) %d Fail. \n", __FUNCTION__, __LINE__,u8DmxMaxPidFilters ));
        return DMX_FILTER_STATUS_ERROR;
    }
    DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxMaxPidFilters = %d.\n",__FUNCTION__, __LINE__, u8DmxMaxPidFilters));

    //if( Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM,(void*)&u8DmxMaxSecFilters) != DMX_FILTER_STATUS_OK )
    //{
    //    DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM) %d Fail. \n", __FUNCTION__, __LINE__, u8DmxMaxSecFilters));
    //    return DMX_FILTER_STATUS_ERROR;
    //}
    //DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxMaxSecFilters = %d.\n",__FUNCTION__, __LINE__, u8DmxMaxSecFilters));

    memset(u8DmxPidFiltersMap, 0x0, sizeof(MS_U8)*128);
    if( Utopia_MApi_DMX_GetCap(DMX_CAP_PIDFLT_MAP,(void*)&u8DmxPidFiltersMap) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_PIDFLT_MAP) Fail. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    //for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxPidFilters; u8DmxIdIndex++ )
    //{
    //    if( u8DmxPidFiltersMap[u8DmxIdIndex] != 0xFF )
    //    {
    //        DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxPidFiltersMap[%d]=(%d).\n",__FUNCTION__, __LINE__, u8DmxIdIndex,u8DmxPidFiltersMap[u8DmxIdIndex]));
    //    }
    //}

    memset(u8DmxSecFiltersMap, 0x0, sizeof(MS_U8)*128);
    if( Utopia_MApi_DMX_GetCap(DMX_CAP_SECFLT_MAP,(void*)&u8DmxSecFiltersMap) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_GetCap(DMX_CAP_SECFLT_MAP) Fail. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
    //for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxSecFilters; u8DmxIdIndex++ )
    //{
    //    if( u8DmxSecFiltersMap[u8DmxIdIndex] != 0xFF )
    //    {
    //        DBG_TSP_STR(printk("[TSP STR][%s][%d] u8DmxSecFiltersMap[%d]=(%d).\n",__FUNCTION__, __LINE__, u8DmxIdIndex,u8DmxSecFiltersMap[u8DmxIdIndex]));
    //    }
    //}

    for( u8DmxIdIndex = 0; u8DmxIdIndex < u8DmxMaxPidFilters; u8DmxIdIndex++ )
    {
         if( u8DmxPidFiltersMap[u8DmxIdIndex] != 0xFF )
         {
             DBG_TSP_STR(printk("[TSP STR][%s][%d] ADP_MApi_DMX_Start_Ex(%d).\n",__FUNCTION__, __LINE__, u8DmxIdIndex));

         if( Utopia_MApi_DMX_Start(u8DmxIdIndex) != DMX_FILTER_STATUS_OK )
         {
             DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] ADP_MApi_DMX_Start_Ex(%d) != DMX_FILTER_STATUS_OK.\n",__FUNCTION__, __LINE__, u8DmxIdIndex));
                 return DMX_FILTER_STATUS_ERROR;
             }
         }
    }

    if( Utopia_MApi_DMX_UtopiaClose() != TRUE )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_DMX_UtopiaClose() != TRUE.\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
   //resumn tsif info
   MDrv_TSP_SelPad(0, E_DRVTSP_IF_TS0, &strIfSet_0);
   MDrv_TSP_SelPad(0, E_DRVTSP_IF_TS1, &strIfSet_1);
   MDrv_TSP_SelPad(0, E_DRVTSP_IF_FI, &strIfSet_FI);
    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    return DMX_FILTER_STATUS_OK;
}



MS_BOOL Utopia_MApi_Dmx_UtopiaOpen(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    if ( uInstantDmx != NULL)
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] uInstantDmx not null. \n",__FUNCTION__, __LINE__));
		return TRUE;
    }
    if( UtopiaOpen(MODULE_DMX, &uInstantDmx, 0, uAttributeDmx) !=  UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaOpen() MODULE_DMX fail. \n",__FUNCTION__, __LINE__));
        UtopiaClose(uInstantDmx);
		uInstantDmx = NULL;
        return FALSE;
    }

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));

    return TRUE;
}

DMX_FILTER_STATUS Utopia_MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput)
{
    DMX_CAP_PARAM CapParam;
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    CapParam.Quetype = DmxQueryType;
    CapParam.pdata   = pOutput;
    CapParam.u32Res = 0;
    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_GetCap, (void*)&CapParam) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_GetCap. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( CapParam.u32Res != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)CapParam.u32Res != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, CapParam.u32Res));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

DMX_FILTER_STATUS Utopia_MApi_DMX_Stop(MS_U8 u8DmxId)
{
    MS_U32 u32Param[2] = {0, 0};
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u32Param[0] = ((MS_U32)u8DmxId) & 0xFF;
    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_Stop,(void*)u32Param) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_Stop. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( u32Param[1] != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32Param[1] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Param[1]));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

DMX_FILTER_STATUS Utopia_MApi_DMX_SectReset(MS_U8 u8DmxId)
{
    MS_U32 u32Param[2] = {0, 0};
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u32Param[0] = ((MS_U32)u8DmxId) & 0xFF;
    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_Sec_Reset,(void*)u32Param) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_SetPowerState. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    //if( u32Param[1] != (MS_U32)DMX_FILTER_STATUS_OK )
    //{
    //    DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32Param[1] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Param[1]));
    //    return DMX_FILTER_STATUS_ERROR;
    //}
    //else
    //{
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    //}
}

DMX_FILTER_STATUS Utopia_MApi_DMX_Start(MS_U8 u8DmxId)
{
    MS_U32 u32Param[2] = {0, 0};
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u32Param[0] = ((MS_U32)u8DmxId) & 0xFF;
    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_Start,(void*)u32Param) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_Start. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( u32Param[1] != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32Param[1] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Param[1]));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

DMX_FILTER_STATUS Utopia_MApi_DMX_SetPowerState(EN_POWER_MODE u16PowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize)
{
    DMX_POWSTATE_PARAM PowSteParam;
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    PowSteParam.u16PowerState = u16PowerState;
    PowSteParam.u32FWAddr = phyFWAddr;
    PowSteParam.u32FWSize = u32FWSize;
    PowSteParam.u32Res = 0;

    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_SetPowerState, (void*)&PowSteParam) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_SetPowerState. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( PowSteParam.u32Res != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)PowSteParam.u32Res != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, PowSteParam.u32Res));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}
DMX_FILTER_STATUS Utopia_MApi_DMX_SetFwDataAddr(MS_U32 u32DataAddr, MS_U32 u32size)
{
	DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    MS_U32 u32param[3] = {0, 0, 0};

    u32param[0] = u32DataAddr;
    u32param[1] = u32size;

    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_SetFwMiuDataAddr, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_SetPowerState. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if(u32param[2] != (MS_U32)DMX_FILTER_STATUS_OK)
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32param[2] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32param[2]));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

MS_BOOL Utopia_MApi_DMX_UtopiaClose(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( UtopiaClose(uInstantDmx) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Error][%s][%d] UtopiaClose() MODULE_DMX fail. \n",__FUNCTION__, __LINE__));
        return FALSE;
    }

    uInstantDmx = NULL;
    uAttributeDmx = NULL;

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    return TRUE;
}

void TspSaveVqInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    // VQ 0
    stVqInfo_0.u16Reg_0 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ0_BASE_L);
    stVqInfo_0.u16Reg_1 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ0_BASE_H);
    stVqInfo_0.u16Reg_2 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ0_CTRL_L);
    stVqInfo_0.u16Reg_3 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ0_CTRL_H);

    // VQ 1
    stVqInfo_1.u16Reg_0 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ1_BASE_L);
    stVqInfo_1.u16Reg_1 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ1_BASE_H);
    stVqInfo_1.u16Reg_2 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ1_CTRL_L);
    stVqInfo_1.u16Reg_3 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ1_CTRL_H);

    // VQ 2
    stVqInfo_2.u16Reg_0 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ2_BASE_L);
    stVqInfo_2.u16Reg_1 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ2_BASE_H);
    stVqInfo_2.u16Reg_2 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ2_CTRL_L);
    stVqInfo_2.u16Reg_3 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ2_CTRL_H);

    // VQ 3
    stVqInfo_3.u16Reg_0 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ3_BASE_L);
    stVqInfo_3.u16Reg_1 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ3_BASE_H);
    stVqInfo_3.u16Reg_2 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ3_CTRL_L);
    stVqInfo_3.u16Reg_3 = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, VQ3_CTRL_H);

	//TSO
	stVqInfo_4.u16Reg_0 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ1_BASE_L);
    stVqInfo_4.u16Reg_1 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ1_BASE_H);
    stVqInfo_4.u16Reg_2 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ1_SIZE);
    stVqInfo_4.u16Reg_3 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ1_TX_CFG);

	stVqInfo_5.u16Reg_0 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ5_BASE_L);
    stVqInfo_5.u16Reg_1 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ5_BASE_H);
    stVqInfo_5.u16Reg_2 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ5_SIZE);
    stVqInfo_5.u16Reg_3 = MDrv_STR_Read2Byte(TSO1_BANK_ADDR, SVQ5_TX_CFG);

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

void TspLoadVqInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    // VQ 0
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ0_BASE_L, stVqInfo_0.u16Reg_0);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ0_BASE_H, stVqInfo_0.u16Reg_1);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ0_CTRL_L, stVqInfo_0.u16Reg_2);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ0_CTRL_H, stVqInfo_0.u16Reg_3);

    // VQ 1
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ1_BASE_L, stVqInfo_1.u16Reg_0);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ1_BASE_H, stVqInfo_1.u16Reg_1);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ1_CTRL_L, stVqInfo_1.u16Reg_2);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ1_CTRL_H, stVqInfo_1.u16Reg_3);

    // VQ 2
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ2_BASE_L, stVqInfo_2.u16Reg_0);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ2_BASE_H, stVqInfo_2.u16Reg_1);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ2_CTRL_L, stVqInfo_2.u16Reg_2);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ2_CTRL_H, stVqInfo_2.u16Reg_3);

    // VQ 3
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ3_BASE_L, stVqInfo_3.u16Reg_0);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ3_BASE_H, stVqInfo_3.u16Reg_1);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ3_CTRL_L, stVqInfo_3.u16Reg_2);
    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, VQ3_CTRL_H, stVqInfo_3.u16Reg_3);
	//TSO

	MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ1_BASE_L, stVqInfo_4.u16Reg_0);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ1_BASE_H, stVqInfo_4.u16Reg_1);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ1_SIZE,   stVqInfo_4.u16Reg_2);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ1_TX_CFG, stVqInfo_4.u16Reg_3);

	MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ5_BASE_L, stVqInfo_5.u16Reg_0);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ5_BASE_H, stVqInfo_5.u16Reg_1);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ5_SIZE,   stVqInfo_5.u16Reg_2);
    MDrv_STR_Write2Byte(TSO1_BANK_ADDR, SVQ5_TX_CFG, stVqInfo_5.u16Reg_3);

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

static MS_U16 u16CACtrl = 0;
void TspSaveCaCtrlInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u16CACtrl = MDrv_STR_Read2Byte(TSP1_BANK_ADDR, CA_CTRL_L);
    DBG_TSP_STR_INFO(printk("[TSP STR INFO][%s][%d] Save CA Ctrl = 0x%x \n", __FUNCTION__, __LINE__, (int)u16CACtrl ));

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

void TspLoadCaCtrlInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    MDrv_STR_Write2Byte(TSP1_BANK_ADDR, CA_CTRL_L, u16CACtrl);
    DBG_TSP_STR_INFO(printk(" [TSP STR INFO][%s][%d][reg = 0x%lx] = 0x%x \n", __FUNCTION__, __LINE__, (unsigned long)(TSP1_BANK_ADDR + CA_CTRL_L), (int)MDrv_STR_Read2Byte(TSP1_BANK_ADDR, CA_CTRL_L)));

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

static MS_U16 u16PhaseTuning = 0;
void TspSavePhaseTuning(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u16PhaseTuning = MDrv_STR_Read2Byte(CLKGEN1_BANK_ADDR, PHASE_TUNING);
    DBG_TSP_STR_INFO(printk("[TSP STR INFO][%s][%d] Save CA Ctrl = 0x%x \n", __FUNCTION__, __LINE__, (int)u16PhaseTuning ));

    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

void TspLoadPhaseTuning(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    MDrv_STR_Write2Byte(CLKGEN1_BANK_ADDR, PHASE_TUNING, u16PhaseTuning);
    DBG_TSP_STR_INFO(printk(" [TSP STR INFO][%s][%d][reg = 0x%lx] = 0x%x \n", __FUNCTION__, __LINE__, (unsigned long)(CLKGEN1_BANK_ADDR + PHASE_TUNING), (int)MDrv_STR_Read2Byte(CLKGEN1_BANK_ADDR, PHASE_TUNING)));
}

static MS_U16 u16TsifInfo = 0;
void TspSaveTsifInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u16TsifInfo = MDrv_STR_Read2Byte(TSP0_BANK_ADDR, TSIF_EN);
    DBG_TSP_STR_INFO(printk("[TSP STR INFO][%s][%d] Save TSIF EN = 0x%x \n", __FUNCTION__, __LINE__, (int)u16TsifInfo ));
    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}

void TspLoadTsifInfo(void)
{
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    MDrv_STR_Write2Byte(TSP0_BANK_ADDR, TSIF_EN, u16TsifInfo);

    DBG_TSP_STR_INFO(printk(" [TSP STR INFO][%s][%d][reg = 0x%lx] = 0x%x \n", __FUNCTION__, __LINE__, (unsigned long)(TSP0_BANK_ADDR + TSIF_EN), (int)MDrv_STR_Read2Byte(TSP0_BANK_ADDR, TSIF_EN)));
    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
}


DMX_FILTER_STATUS Utopia_MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize)
{
    MS_U32 u32Param[3] = {0, 0, 0};
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u32Param[0] = (MS_U32)pFwAddr;
    u32Param[1] = u32FwSize;

    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_SetFw, (void*)u32Param) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_SetFw. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( u32Param[2] != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32Param[2] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Param[2]));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

DMX_FILTER_STATUS Utopia_MApi_DMX_Init(void)
{
    MS_U32 u32Param = 0;
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_Init_1_0, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSD STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_Init_1_0. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( u32Param != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSD STR Error][%s][%d] (%ld)u32Param != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Param));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

DMX_FILTER_STATUS Utopia_MApi_DMX_SetHK(MS_BOOL bIsHK)
{
    MS_U32 u32Data[2] = {0, 0};
    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    u32Data[0]= (MS_U32)bIsHK;
    if( UtopiaIoctl(uInstantDmx, MApi_CMD_DMX_SetHK, (void*)u32Data) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] UtopiaIoctl MApi_CMD_DMX_SetHK. \n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if( u32Data[1] != (MS_U32)DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] (%ld)u32Data[1] != DMX_FILTER_STATUS_OK. \n", __FUNCTION__, __LINE__, u32Data[1]));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_OK;
    }
}

int application_tsp_init(void)
{
    MS_U8*  u8BinVA = NULL;
    MS_U8   u8Index = 0;
    MS_U32  u32BinPA = 0;
    MS_U32  u32FwAddrPhys = 0;
    MS_U32  u32VQPA = 0;
    MS_U32  u32FilterNum = 0;
    uint32_t u32BinSize = 0 , u32VQSize = 0;

    DMX_TSPParam stParam;

    DBG_TSP_STR_ENTRY(printk("[TSP STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));


    if( Utopia_MApi_Dmx_UtopiaOpen() != TRUE )
    {
        DBG_TSP_STR_ERR(printk("[TSP STR Error][%s][%d] Utopia_MApi_Dmx_UtopiaOpen() != TRUE.\n", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    u32BinPA = BIN_MEM_ADR;
    u32BinSize = BIN_MEM_LEN;

    MsOS_MPool_Mapping_Dynamic(0,BIN_MEM_ADR,BIN_MEM_LEN,TRUE);

    u8BinVA = (MS_U8*)MsOS_MPool_PA2KSEG1(u32BinPA);
    if( NULL == u8BinVA )
    {
        DBG_TSP_STR_ERR(printk("[%s][%d] Fail to convert PA to VA. \n",__FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        DBG_TSP_STR_INFO(printk("[%s][%d] u8BinVA = (%lx). \n",__FUNCTION__, __LINE__,(MS_U32)u8BinVA));
    }

    memset(u8BinVA, 0, sizeof(_tsp_fw_dat));
    memcpy(u8BinVA, _tsp_fw_dat, sizeof(_tsp_fw_dat));

    DBG_TSP_STR_INFO(printk("FW size = 0x%x \n", sizeof(_tsp_fw_dat)));
    DBG_TSP_STR_INFO(printk("bin_mem = 0x%lx \n", (MS_U32)u8BinVA));

    u32FwAddrPhys = u32BinPA;

    if( Utopia_MApi_DMX_SetFW(u32FwAddrPhys,sizeof(_tsp_fw_dat)) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP Error][%s][%d] Utopia_MApi_DMX_SetFW() Fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    if( Utopia_MApi_DMX_Init() != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP Error][%s][%d] Utopia_MApi_DMX_Init() Fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    if( Utopia_MApi_DMX_SetHK(TRUE) != DMX_FILTER_STATUS_OK )
    {
        DBG_TSP_STR_ERR(printk("[TSP Error][%s][%d] Utopia_MApi_DMX_SetHK() Fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }


    DBG_TSP_STR_EXIT(printk("[TSP STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    return TRUE;
}
