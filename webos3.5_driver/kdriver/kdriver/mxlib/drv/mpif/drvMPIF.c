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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMPIF.c
/// @brief MPIF Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "regMPIF.h"
#include "halMPIF.h"
#include "drvMPIF.h"

#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MPIF_MUTEX_WAIT_TIME    						0xFFFFFFFF

//configure LC2X
#define MPIF_LC2X_CHEKCSUM_SEL							MPIF_CHECKSUM_ENABLE
#define MPIF_LC2X_RTX_SEL								MPIF_MAX_RTX_0

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern function
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_mpif_version = {
    .DDI = { MPIF_DRV_VERSION },
};

static MS_S32 _s32MPIF_Mutex;

static MPIF_DrvStatus _MPIFDriverStatus = {
	.bIsBusy = FALSE,
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _Drv_MPIF_GetSPIF_ChBusySts(MS_U8 u8slaveid, MS_U16 u16event_bit, MS_U32 u32timeout)
{
    MS_U16 u16data;
    MS_U32 u32i;
	MS_BOOL bRes = FALSE;

    // Use a limit value to prevent infinite loop
    for (u32i=0; u32i < u32timeout; u32i++)
    {
        if(!HAL_MPIF_LC2A_RW(FALSE, u8slaveid, MPIF_LC2X_CHEKCSUM_SEL, MPIF_LC2X_RTX_SEL, REG_SPIF_CHANNEL_BUSY_STATUS, &u16data))
            break;
        if((u16data & u16event_bit) == 0x00)        
            return TRUE;        
    }

	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("SPIF channel status busy.\n"));

    return bRes;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description: Show the MPIF driver version
/// @param ppVersion  \b OUT  Library version string
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_mpif_version;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Description: Get MPIF driver information
/// @return MPIF_CONFIG: mpif driver configuration informtion.
//------------------------------------------------------------------------------
const MPIF_CONFIG* MDrv_MPIF_GetInfo(void)
{       
    return (MPIF_CONFIG*)(&_hal_mpif_cfg);
}

//------------------------------------------------------------------------------
/// Description: Show MPIF driver status
/// @param pStatus  \b OUT  pointer to store status data
//------------------------------------------------------------------------------
void MDrv_MPIF_GetStatus(MPIF_DrvStatus *pStatus)
{
	memcpy(pStatus, &_MPIFDriverStatus, sizeof(MPIF_DrvStatus));
}

//------------------------------------------------------------------------------
/// Set detailed level of MPIF driver debug message
/// @param u8DbgLevel  \b IN  debug level for MPIF driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8MPIFDbgLevel = u8DbgLevel;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize MPIF driver
/// @param pcfg  \b IN  pointer stored configure data
/// @return None
/// @note   called only once at system initialization
//-------------------------------------------------------------------------------------------------
// struct MPIF_CONFIG must sync to HAL struct HAL_MPIF_CONFIG
void MDrv_MPIF_Init(MPIF_CONFIG* pcfg)
{
    MS_U32 u32NonPMBank, u32NonPMBankSize;	
    
	MS_ASSERT(sizeof(HAL_MPIF_CONFIG) == sizeof(MPIF_CONFIG)); 	
	
    _u8MPIFDbgLevel = MPIF_DBGLV_INFO; // init debug level first

    _s32MPIF_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MPIF", MSOS_PROCESS_SHARED);
    MS_ASSERT(_s32MPIF_Mutex >= 0);
    
    DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    //
    //  1. HAL init
    //
    if (!MDrv_MMIO_GetBASE( &u32NonPMBank, &u32NonPMBankSize, MS_MODULE_MPIF))
    {
        DEBUG_MPIF(MPIF_DBGLV_ERR, printf("IOMap failure to get MS_MODULE_MPIF\n"));
        u32NonPMBank = BASEADDR_RIU;
    }	
    HAL_MPIF_Config(u32NonPMBank);	
	
	memcpy(&_hal_mpif_cfg, pcfg, sizeof(_hal_mpif_cfg));
	
    HAL_MPIF_Init(&_hal_mpif_cfg);
    
}

void MDrv_MPIF_Uninit(void)
{
    MsOS_DeleteMutex(_s32MPIF_Mutex);
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 1A read/write
/// @param bWrite  	\b IN : If set to true, do writing; else do reading.
/// @param u8slaveid  	\b IN : Slave ID
/// @param u8index    	\b IN : index of 1A channel
/// @param pu8data    \b OUT : pointer which stored reading data or writing data
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC1A_RW(MS_U8 bWrite, MS_U8 u8slaveid, MS_U8 u8index, MS_U8 *pu8data)
{		
	MS_BOOL bRes = FALSE;	

	if(pu8data == NULL)
		return FALSE;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%p)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, (int)u8index, pu8data));
	
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC1A_RW fails!\n"));
		return bRes;
	}

	bRes = HAL_MPIF_LC1A_RW(bWrite, u8slaveid, u8index, pu8data);

	MsOS_ReleaseMutex(_s32MPIF_Mutex);
		
	return bRes; 
}

//-------------------------------------------------------------------------------------------------
/// MPIF set 2X path to be SPIF or XIU
/// @param u8slaveid  	\b IN : Slave ID
/// @param u8path      \b IN : 2X path data
/// @return TRUE if successed or return FALSE if failed.
/// @note   Please always call this function before calling LC2X read/write function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_Set_LC2XPath(MS_U8 u8slaveid, MS_U8 u8path)
{
	MS_BOOL bRes = FALSE;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d)\n", __FUNCTION__, (int)u8slaveid, (int)u8path));
	
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_Set_LC2XPath fails!\n"));
		return bRes;
	}

	bRes = HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path);

	MsOS_ReleaseMutex(_s32MPIF_Mutex);
		
	return bRes; 
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 2A read/write
/// @param bWrite  		 \b IN : If set to TRUE, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param bchecksum    	 \b IN : If set to TRUE, do checksum enabled; else do checksum disabled.
/// @param u8rtx    	 	 \b IN : Re-transmit/receive count limit. 0~3 times.
/// @param u16addr   		 \b IN : Reading address
/// @param pu16data   	 \b OUT : Pointer to stored reading data. It's a visual address.
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC2A_RW(MS_U8 bWrite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16* pu16data)
{		
	MS_BOOL bRes = FALSE;
		
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, 0x%x, 0x%p)\n", __FUNCTION__, (int)u8slaveid, u16addr, pu16data));

	if(pu16data == NULL)
		return FALSE;
	
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC2A_RW fails!\n"));
		return bRes;
	}	

	bRes = HAL_MPIF_LC2A_RW(bWrite, u8slaveid, bchecksum, u8rtx, u16addr, pu16data);	
	
	MsOS_ReleaseMutex(_s32MPIF_Mutex);
		
	return bRes; 
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 2B read/write
/// @param bWrite  		 \b IN : If set to TRUE, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param bchecksum    	 \b IN : If set to TRUE, do checksum enabled; else do checksum disabled.
/// @param u8rtx    	 	 \b IN : Re-transmit/receive count limit. 0~3 times.
/// @param u16addr   		 \b IN : Reading address
/// @param pu16data   	 \b OUT : Pointer to stored reading data. It's a visual address.
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC2B_RW(MS_U8 bWrite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16* pu16data)
{		
	MS_BOOL bRes = FALSE;
		
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, 0x%x, 0x%p)\n", __FUNCTION__, (int)u8slaveid, u16addr, pu16data));

	if(pu16data == NULL)
		return FALSE;
	
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC2B_RW fails!\n"));
		return bRes;
	}
	
	bRes = HAL_MPIF_LC2B_RW(bWrite, u8slaveid, bchecksum, u8rtx, u16addr, pu16data);
			
	MsOS_ReleaseMutex(_s32MPIF_Mutex);
		
	return bRes; 
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 3A RIU mode ead/write
/// @param bWrite	    	 \b IN : If set to true, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8datalen 		 \b IN : Data length. Unit is byte.
/// @param pu8data  		 \b IN/OUT : pointer to store writing/reading data. It's a visual address.
/// @param p3xcfg  		 \b IN/OUT : pointer to store 3X control configure data
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC3A_RIURW(MS_BOOL bWrite, MS_U8 u8slaveid, MS_U8 u8datalen, MS_U8 *pu8data, MPIF_3XCTL *p3xcfg)
{
	MS_BOOL bRes = FALSE;
	MS_U16 u16data;
	HAL_MPIF_3XCTL LC3xcfg = {0};	
	MS_U8 u8path;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%p)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, u8datalen, pu8data));

    if((pu8data == NULL) || (p3xcfg == NULL))
        return FALSE;   

	// Check the parameter
	if (u8datalen > MPIF_LC3X_PACKET_DATA_MAX_LENGTH)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("Data length can't be greater than %d bytes.\n", MPIF_LC3X_PACKET_DATA_MAX_LENGTH));
		return bRes;
	}

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC3A_RIURW fails!\n"));
		return bRes;
	}			

	LC3xcfg.bchksum_en = p3xcfg->bChkSum;
	LC3xcfg.bfmode = p3xcfg->bFastMode;
	LC3xcfg.bNotWaitMIUDone = 0; //always wait
	LC3xcfg.bretrx_idx = 0;
	LC3xcfg.u8MIUSel = 0; //only support MIU 0
	LC3xcfg.u8retrx = p3xcfg->u8rtx;
	LC3xcfg.u8Wcnt = p3xcfg->u8wcnt; 

	// Waiting for the channel being free
	if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_3A, MAX_TIMEOUT))	 
		return FALSE;
	
	//----------------------- Configure SPIF------------------------------	
	u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto LC3A_RIURW_END;

	if(!_Drv_MPIF_GetSPIF_ChBusySts(u8slaveid, SPIF_LC3A_BUSY, MAX_TIMEOUT))
		goto LC3A_RIURW_END;

	u16data = 1;
	if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_PACKET_LEN, &u16data))  
		goto LC3A_RIURW_END;	

	u16data = (MS_U16)(bWrite & 0x01) | (((MS_U16)LC3xcfg.bchksum_en & 0x01) << 1); //from RIU
	if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_PACKET_CONTROL, &u16data))  
		goto LC3A_RIURW_END;	
	//-------------------------------------------------------------------	
	
	bRes = HAL_MPIF_LC3A_RIURW(bWrite, u8slaveid, u8datalen, pu8data, &LC3xcfg); 
	
LC3A_RIURW_END:
	
	MsOS_ReleaseMutex(_s32MPIF_Mutex);

	return bRes;
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 3B RIU mode read/write
/// @param bWrite	    	 \b IN : If set to true, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8datalen 		 \b IN : Data length. Unit is byte.
/// @param pu8data  		 \b IN/OUT : pointer to store writing/reading data. It's a visual address.
/// @param p3xcfg  		 \b IN/OUT : pointer to store 3X control configure data
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC3B_RIURW(MS_BOOL bWrite, MS_U8 u8slaveid, MS_U8 u8datalen, MS_U8 *pu8data, MPIF_3XCTL *p3xcfg)
{
	MS_BOOL bRes = FALSE;
	MS_U16 u16data;
	HAL_MPIF_3XCTL LC3xcfg = {0};	
	MS_U8 u8path;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%p)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, u8datalen, pu8data));

    if((pu8data == NULL) || (p3xcfg == NULL))
        return FALSE;
	
	// Check the parameter
	if(u8datalen > MPIF_LC3X_PACKET_DATA_MAX_LENGTH)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("Data length can't be greater than %d bytes.\n", MPIF_LC3X_PACKET_DATA_MAX_LENGTH));
		return bRes;
	}

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC3B_RIURW fails!\n"));
		return bRes;
	}
	 
	LC3xcfg.bchksum_en = p3xcfg->bChkSum;
	LC3xcfg.bfmode = p3xcfg->bFastMode;
	LC3xcfg.bNotWaitMIUDone = 0; //always wait
	LC3xcfg.bretrx_idx = 0;
	LC3xcfg.u8MIUSel = 0; //only support MIU 0
	LC3xcfg.u8retrx = p3xcfg->u8rtx; 
	LC3xcfg.u8Wcnt = p3xcfg->u8wcnt; 

	// Waiting for the channel being free
	if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_3B, MAX_TIMEOUT))	 
		return FALSE;	  	

	//----------------------- Configure SPIF------------------------------	
	u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto LC3B_RIURW_END;

	if(!_Drv_MPIF_GetSPIF_ChBusySts(u8slaveid, SPIF_LC3B_BUSY, MAX_TIMEOUT))
		goto LC3B_RIURW_END;

	u16data= 1;
	if(!HAL_MPIF_LC2B_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_PACKET_LEN, &u16data))  
		goto LC3B_RIURW_END;	

	u16data = (MS_U16)(bWrite & 0x01) | (((MS_U16)LC3xcfg.bchksum_en & 0x01) << 1); //from RIU
	if(!HAL_MPIF_LC2B_RW(TRUE,u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_PACKET_CONTROL, &u16data))  
		goto LC3B_RIURW_END;	
	//-------------------------------------------------------------------	
	
	bRes = HAL_MPIF_LC3B_RIURW(bWrite, u8slaveid, u8datalen, pu8data, &LC3xcfg); 
	
LC3B_RIURW_END:
	
	MsOS_ReleaseMutex(_s32MPIF_Mutex);

	return bRes;
}

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 3A MIU mode read/write
/// @param bWrite	    	 \b IN : If set to true, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u32datalen	 \b IN : Data length. Unit by 16 bytes.
/// @param u32miu_addr	 \b IN : MPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @param u32spif_mdr	 \b IN : SPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @param p3xctl  		 \b IN/OUT : pointer to store 3X control configure data
/// @return TRUE if successed or return FALSE if failed.
/// @note   If using cache buffer, please do buffer flush first.
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_MPIF_LC3A_MIURW(MS_BOOL bWrite, MS_U8 u8slaveid, MS_U32 u32datalen, MS_PHYADDR u32miu_addr, MS_PHYADDR u32spif_mdr, MPIF_3XCTL *p3xctl)
{
	MS_BOOL bRes = FALSE;
    MS_U16 u16data;  	
	HAL_MPIF_3XCTL LC3xcfg = {0};	
	MS_U8 u8path;
//	MS_U32 u32systiem0, u32systiem1;    
    
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%x, 0x%x)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, (int)u32datalen,
				(unsigned int)u32miu_addr, (unsigned int)u32spif_mdr));	
			
	if(p3xctl == NULL)
		return FALSE;

//	u32systiem0 = MsOS_GetSystemTime();	
	
    if(u32datalen % MPIF_LC3X_PACKET_DATA_MAX_LENGTH)
    {
        DEBUG_MPIF(MPIF_DBGLV_ERR, printf("LC3X packet data length should be mutipled by 16\n"));
        return FALSE;
    }
    if((u32datalen >> 4) > MPIF_LC3X_MAX_PACKET)
    {
        DEBUG_MPIF(MPIF_DBGLV_ERR, printf("LC3X packet data length overflow\n"));
        return FALSE;
    }    

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC3A_MIURW fails!\n"));
		return bRes;
	}	

	LC3xcfg.bchksum_en = p3xctl->bChkSum;
	LC3xcfg.bfmode = p3xctl->bFastMode;
	LC3xcfg.bNotWaitMIUDone = 0; 
	LC3xcfg.bretrx_idx = 0;
	LC3xcfg.u8MIUSel = 0; //only support MIU 0
	LC3xcfg.u8retrx = p3xctl->u8rtx; 
	LC3xcfg.u8Wcnt = p3xctl->u8wcnt; 

	// Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_3A, MAX_TIMEOUT))    
        return FALSE;   
	
    //----------------------- Configure SPIF------------------------------
    u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto LC3A_MIURW_END;
    if(!_Drv_MPIF_GetSPIF_ChBusySts(u8slaveid, SPIF_LC3A_BUSY, MAX_TIMEOUT))
        goto LC3A_MIURW_END;
	
    u16data = ((MS_U16)bWrite&0x01) | (((MS_U16)LC3xcfg.bchksum_en &0x01) << 1) | 0x04 |
				((MS_U16)(LC3xcfg.u8MIUSel & 0x01) << 4); //MIU 0
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_PACKET_CONTROL, &u16data))
        goto LC3A_MIURW_END;
	
    u16data = (MS_U16)(u32datalen >> 4); //unit is 16 bytes
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_PACKET_LEN, &u16data))
        goto LC3A_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 3) & 0xFFFF);
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_MADR_L, &u16data))
        goto LC3A_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 19) & 0xFF);
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3A_MADR_H, &u16data))
        goto LC3A_MIURW_END;
    //--------------------------------------------------------------------	

	bRes = HAL_MPIF_LC3A_MIURW(bWrite, u8slaveid, u32datalen, u32miu_addr, u32spif_mdr, &LC3xcfg);	
    
LC3A_MIURW_END:
	
	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

//	u32systiem1 = MsOS_GetSystemTime();		

//	printf("\n\n3A MIU performance %d msn\n", (int)(u32systiem1 - u32systiem0));

    return bRes;
}// Read data from the SPIF to the MIU via LC3A

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 3B MIU mode read/write
/// @param bWrite	    	 \b IN : If set to true, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u32datalen	 \b IN : Data length. Unit by 16 bytes.
/// @param u32miu_addr	 \b IN : MPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @param u32spif_mdr	 \b IN : SPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @param p3xctl  		 \b IN/OUT : pointer to store 3X control configure data
/// @return TRUE if successed or return FALSE if failed.
/// @note   If using cache buffer, please do buffer flush first.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC3B_MIURW(MS_BOOL bWrite, MS_U8 u8slaveid, MS_U32 u32datalen, MS_PHYADDR u32miu_addr, MS_PHYADDR u32spif_mdr, MPIF_3XCTL *p3xctl)
{
	MS_BOOL bRes = FALSE;
    MS_U16 u16data;  	
	HAL_MPIF_3XCTL LC3xcfg = {0};
	MS_U8 u8path;    
    
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%x, 0x%x)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, (int)u32datalen,
				(unsigned int)u32miu_addr, (unsigned int)u32spif_mdr));
	
	if(p3xctl == NULL)
		return FALSE;

    if(u32datalen % MPIF_LC3X_PACKET_DATA_MAX_LENGTH)
    {
        DEBUG_MPIF(MPIF_DBGLV_ERR, printf("LC3X packet data length should be mutipled by 16\n"));
        return FALSE;
    }
    if((u32datalen >> 4) > MPIF_LC3X_MAX_PACKET)
    {
        DEBUG_MPIF(MPIF_DBGLV_ERR, printf("LC3X packet data length overflow\n"));
        return FALSE;
    }    

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC3B_MIURW fails!\n"));
		return bRes;
	}	 

	LC3xcfg.bchksum_en = p3xctl->bChkSum;
	LC3xcfg.bfmode = p3xctl->bFastMode;
	LC3xcfg.bNotWaitMIUDone = 0;
	LC3xcfg.bretrx_idx = 0;
	LC3xcfg.u8MIUSel = 0;
	LC3xcfg.u8retrx = p3xctl->u8rtx; 
	LC3xcfg.u8Wcnt = p3xctl->u8wcnt; 

	// Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_3B, MAX_TIMEOUT))    
        return FALSE;    
	
    //----------------------- Configure SPIF------------------------------
    u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto LC3B_MIURW_END;
	
    if(!_Drv_MPIF_GetSPIF_ChBusySts(u8slaveid, SPIF_LC3B_BUSY, MAX_TIMEOUT))
        goto LC3B_MIURW_END;
	
    u16data = ((MS_U16)bWrite&0x01) | (((MS_U16)LC3xcfg.bchksum_en &0x01) << 1) | 0x04 |
				((MS_U16)(LC3xcfg.u8MIUSel & 0x01) << 4); //MIU 0
    if(!HAL_MPIF_LC2B_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_PACKET_CONTROL, &u16data))
        goto LC3B_MIURW_END;
	
    u16data = (MS_U16)(u32datalen >> 4); //unit is 16 bytes
    if(!HAL_MPIF_LC2B_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_PACKET_LEN, &u16data))
        goto LC3B_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 3) & 0xFFFF);
    if(!HAL_MPIF_LC2B_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_MADR_L, &u16data))
        goto LC3B_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 19) & 0xFF);
    if(!HAL_MPIF_LC2B_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC3B_MADR_H, &u16data))
        goto LC3B_MIURW_END;
    //--------------------------------------------------------------------	

	bRes = HAL_MPIF_LC3B_MIURW(bWrite, u8slaveid, u32datalen, u32miu_addr, u32spif_mdr, &LC3xcfg);
    
LC3B_MIURW_END:
	
	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
}// Read data from the SPIF to the MIU via LC3A

//-------------------------------------------------------------------------------------------------
/// MPIF logical channel 3B MIU mode read/write
/// @param bWrite	    	 \b IN : If set to true, do writing; else do reading.
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u16datalen	 \b IN : Data length. Unit by 16 bytes.
/// @param u32miu_addr	 \b IN : MPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @param u32spif_mdr	 \b IN : SPIF MIU Start address. Unit is byte. Must be 8bytes aligmant.
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_LC4A_MIURW(MS_BOOL bWrite, MS_U8 u8slaveid, MS_U16 u16datalen, MS_PHYADDR u32miu_addr, MS_PHYADDR u32spif_mdr)
{
	MS_BOOL bRes = FALSE;
    MS_U16 u16data;      
	HAL_MPIF_4ACTL LC4XCfg = {0};
	MS_U8 u8path;
        
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d, 0x%x, 0x%x)\n", __FUNCTION__, (int)bWrite, (int)u8slaveid, (int)u16datalen,
				(unsigned int)u32miu_addr, (unsigned int)u32spif_mdr));

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_LC4A_MIURW fails!\n"));
		return bRes;
	}

	LC4XCfg.bNotWaitMIUDone = 0;
	LC4XCfg.u8granular = MPIF_LC4A_CHECKPOINT_3;
	LC4XCfg.u8MIUSel = 0; //only support miu 0 in T3
	LC4XCfg.u8retrx_limit = MPIF_MAX_RTX_3;
	LC4XCfg.u8Wcnt = 0x08;

	// Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_4A, MAX_TIMEOUT))    
        return FALSE;  
	
    //----------------------- Configure SPIF------------------------------
    u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto LC4A_MIURW_END;
	 
    if(!_Drv_MPIF_GetSPIF_ChBusySts(u8slaveid, SPIF_LC4A_BUSY, MAX_TIMEOUT))
        goto LC4A_MIURW_END;
	
    u16data = (((MS_U16)bWrite) & 0x01) | ((MS_U16)(LC4XCfg.u8granular & 0x03) << 1) | ((MS_U16)(LC4XCfg.u8MIUSel &0x01) << 4); 
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC4A_CONTROL, &u16data))
        goto LC4A_MIURW_END;
	
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC4A_STREAM_LEN, &u16datalen))
        goto LC4A_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 3) & 0xFFFF);
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC4A_MADR_L, &u16data))
        goto LC4A_MIURW_END;
	
    u16data = (MS_U16)((u32spif_mdr >> 19) & 0xFF);
    if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_LC4A_MADR_H, &u16data))
        goto LC4A_MIURW_END;
    //--------------------------------------------------------------------	   

	bRes = HAL_MPIF_LC4A_MIURW(bWrite, u8slaveid, u16datalen, u32miu_addr, u32spif_mdr, &LC4XCfg);     
    
	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

LC4A_MIURW_END:	

    return bRes;
}

//-------------------------------------------------------------------------------------------------
/// MPIF printf SPIF's configure information
/// @param u8slaveid    	 \b IN : Slave ID
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_Print_SPIFCfgInfo(MS_U8 u8slaveid)
{
	MS_U16 u16data;

	printf("\n------------SPIF----------------\n");
	
	//Read SPIF's MISC
    if(!MDrv_MPIF_LC2A_RW(FALSE, u8slaveid, TRUE, MPIF_MAX_RTX_0, REG_SPIF_MISC1, &u16data))
    	return FALSE;	
    
    printf("Slave ID: %d\n", (int)(u16data & 0x03));
    printf("Commend bit %d\n", (int)((u16data >> 4) & 0x03));
    printf("Data bit %d\n", (int)((u16data >> 6) & 0x03));
    printf("Turn around cycle %d\n", (int)((u16data >> 8) & 0x03));
    printf("Wait cycle %d\n", (int)((u16data >> 10) & 0x03));

	if(!MDrv_MPIF_LC2A_RW(FALSE, u8slaveid, TRUE, MPIF_MAX_RTX_0, REG_SPIF_MISC2, &u16data))
    	return FALSE;	

	printf("SPIF input clock delay %d\n", (int)(u16data & 0x0F));
	printf("SPIF input clock inv %d\n", (int)((u16data >> 4) & 0x01));
	printf("SPIF input clock inv delay %d\n", (int)((u16data >> 8) & 0x0F));
	printf("SPIF input clock inv inv enable %d\n", (int)((u16data >> 12) & 0x01));
	
    printf("---------------------------------\n\n");

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// MPIF Set commend and slave data-width
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8cmdwidth	 \b IN : Commend data width
/// @param u8datawidth	 \b IN : Slave data width
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SetCmdDataMode(MS_U8 u8slaveid, MS_U8 u8cmdwidth, MS_U8 u8datawidth)
{
	MS_BOOL bRes = FALSE;
    MS_U16 u16data;
	MS_U8 u8path;

	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %x, %x)\n", __FUNCTION__, (int)u8slaveid, u8cmdwidth, u8datawidth));				

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_SetCmdDataMode fails!\n"));
		return bRes;
	}

    //---- Configure SPIF first ---------
    u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		goto SET_CMDDATA_WIDTH_END;	
    if(!HAL_MPIF_LC2A_RW(FALSE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC1, &u16data))
        goto SET_CMDDATA_WIDTH_END;    
    u16data &= ~0xF0;
    u16data |= (((MS_U16)(u8cmdwidth & 0x03) << 4) | ((MS_U16)(u8datawidth & 0x03) << 6));
	if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC1, &u16data))
        goto SET_CMDDATA_WIDTH_END;    
    //---------------------------------

    //---- Then change MPIF setting -----------
    bRes = HAL_MPIF_SetCmdDataMode(u8slaveid, u8cmdwidth, u8datawidth);
    //-----------------------------------------

SET_CMDDATA_WIDTH_END:	

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// MPIF Set Clock
/// @param u8slaveid    			 \b IN : Slave ID
/// @param u8clk		    	 		 \b IN : Select MPIF clock
/// @param u8slaveClkInvDlyNum	 \b IN : Slave Delay buffer number. (0~15)
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_Set_Clock(MS_U8 u8slaveid, MS_U8 u8clk, MS_U8 u8slaveClkInvDlyNum)
{
	MS_BOOL bRes = FALSE;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d)\n", __FUNCTION__, (int)u8slaveid, (int)u8clk, (int)u8slaveClkInvDlyNum));	

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_Set_Clock fails!\n"));
		return bRes;
	}
	
	if(!HAL_MPIF_SetSlave_ClkInv_Delay(u8slaveid, u8slaveClkInvDlyNum))
		goto SET_CLK_END;
	
	HAL_MPIF_Set_Clock(u8clk);
	
	HAL_MPIF_SWReset();

SET_CLK_END:

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
}

//-------------------------------------------------------------------------------------------------
/// MPIF Clock Disable or Enable
/// @param bDisabled    			 \b IN : If true, enable MPIF clock; if false, disable MPIF clock 
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_Clock_Disable(MS_BOOL bDisabled)
{
	MS_BOOL bRes = FALSE;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, (int)bDisabled));	

	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_Clock_Disable fails!\n"));
		return bRes;
	}
	
	HAL_MPIF_CLK_Disable(bDisabled);

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
}


//-------------------------------------------------------------------------------------------------
/// MPIF Set Slave Clock Invers delay
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8DlyBufNum	 \b IN : Delay buffer number. (0~15)
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SetSlave_ClkInv_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum)
{
	MS_BOOL bRes = FALSE;	
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d)\n", __FUNCTION__, (int)u8slaveid, (int)u8DlyBufNum));				
		
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_SetSlave_ClkInv_Delay fails!\n"));
		return bRes;
	}

	bRes = HAL_MPIF_SetSlave_ClkInv_Delay(u8slaveid, u8DlyBufNum);  

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
}

//-------------------------------------------------------------------------------------------------
/// MPIF Set Slave Clock delay
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8DlyBufNum	 \b IN : Delay buffer number. (0~15)
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SetSlave_Clk_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum)
{
	MS_BOOL bRes = FALSE;	
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d)\n", __FUNCTION__, (int)u8slaveid, (int)u8DlyBufNum));				
		
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_SetSlave_Clk_Delay fails!\n"));
		return bRes;
	}

	bRes = HAL_MPIF_SetSlave_Clk_Delay(u8slaveid, u8DlyBufNum);  

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
}


//-------------------------------------------------------------------------------------------------
/// MPIF Set Slave turn around cycle and wait ack cycle
/// @param u8slaveid    	 \b IN : Slave ID
/// @param u8trc			 \b IN : Turn around cycle. (0T~3T)
/// @param u8wt			 \b IN : Wait ACK cycle. (0T~3T)
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SetSlave_Trc_Wt(MS_U8 u8slaveid, MS_U8 u8trc, MS_U8 u8wt)
{
	MS_BOOL bRes = FALSE;	
	MS_U16 u16data;
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(%d, %d, %d)\n", __FUNCTION__, (int)u8slaveid, (int)u8trc, (int)u8wt));		

	
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_SetSlave_Trc_Wt fails!\n"));
		return bRes;
	}
	
	if(!HAL_MPIF_LC2A_RW(FALSE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC1, &u16data))
    	goto SET_TRC_WC_END;    
    u16data &= ~0xF0;
    u16data |= (((MS_U16)(u8trc & 0x03) << 8) | ((MS_U16)(u8wt & 0x03) << 10));
	bRes = HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC1, &u16data);
       
	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

SET_TRC_WC_END:
	
    return bRes;
}


#ifdef MPIF_SPI_SUPPORT

//-------------------------------------------------------------------------------------------------
/// Configure MPIF SPI mode
/// @param u8slaveid    	 	\b IN : Slave ID
/// @param pcfg  				\b IN  pointer stored configure data
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
// struct MPIF_SPI_CONFIG must sync to HAL struct HAL_MPIF_SPI_CONFIG
MS_BOOL MDrv_MPIF_SPI_Configure(MS_U8 u8slaveid, MPIF_SPI_CONFIG* pcfg)
{
    MS_BOOL bRes = FALSE;	
	
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(0x%08x)\n", __FUNCTION__, (int)pcfg));				

	if(sizeof(HAL_MPIF_CONFIG) != sizeof(MPIF_CONFIG))
		return FALSE;

	_hal_mpif_spi_cfg.bSrcMIU = pcfg->bSrcMIU;
	_hal_mpif_spi_cfg.u8CMDLen = pcfg->u8CMDLen;
	_hal_mpif_spi_cfg.u8LeadingCyc = pcfg->u8LeadingCyc;
	_hal_mpif_spi_cfg.u8MIUSel = pcfg->u8MIUSel;
	_hal_mpif_spi_cfg.u8SerialMode = pcfg->u8SerialMode;
	_hal_mpif_spi_cfg.u8TrailingCyc = pcfg->u8TrailingCyc;
		
	if (MsOS_ObtainMutex(_s32MPIF_Mutex, MPIF_MUTEX_WAIT_TIME) == FALSE)
	{
		DEBUG_MPIF(MPIF_DBGLV_ERR, printf("ObtainMutex in MDrv_MPIF_SPI_Configure fails!\n"));
		return bRes;
	}
	
	bRes = HAL_MPIF_SPI_Config(u8slaveid, &_hal_mpif_spi_cfg);  

	MsOS_ReleaseMutex(_s32MPIF_Mutex);	

    return bRes;
    
}

//-------------------------------------------------------------------------------------------------
/// Use SPI RIU mode to read flash ID
/// @param bWrite    	 		\b IN : If TRUE, it is write data mode, otherwise is read data mode
/// @param pu32cmd    	 	\b IN : pointer stored commend
/// @param u8cmdlen    	 	\b IN : commend length to write
/// @param pudata			\b IN/OUT : pointer to store write/read data
/// @param u16datalen    	 	\b IN : data length to write/read
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SPI_RIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_U8 *pudata, MS_U16 u16datalen)
{
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s: CMD %08x\n", __FUNCTION__, (int)*pu32cmd));	
	
	return HAL_MPIF_SPI_RIU_RWData(bWrite, pu32cmd, u8cmdlen, pudata, u16datalen);	
}

//-------------------------------------------------------------------------------------------------
/// Use SPI RIU mode to read flash ID
/// @param bWrite    	 		\b IN : If TRUE, it is write data mode, otherwise is read data mode
/// @param pu32cmd    	 	\b IN : pointer stored commend
/// @param u8cmdlen    	 	\b IN : commend length to write
/// @param u32MiuAddr		\b IN : MIU write/read base address
/// @param u16datalen    	 	\b IN : data length to write/read
/// @return TRUE if successed or return FALSE if failed.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MPIF_SPI_MIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_PHYADDR u32MiuAddr, MS_U16 u16datalen)
{
	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s: CMD %08x\n", __FUNCTION__, (int)*pu32cmd));	
	
	return HAL_MPIF_SPI_MIU_RWData(bWrite, pu32cmd, u8cmdlen, u32MiuAddr, u16datalen);	
}

#endif //MPIF_SPI_SUPPORT

