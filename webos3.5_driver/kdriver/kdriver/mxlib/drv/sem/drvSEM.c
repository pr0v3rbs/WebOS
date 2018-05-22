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
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvSEM.h"
#include "drvMMIO.h"
#include "halSEM.h"

#include "utopia_dapi.h"
#include "drvSEM_priv.h"
#include "utopia.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SEM_MAX_NUM                       (16)

#define SEM_ERROR(fmt, args...)           printf("[SEM USER ERR][%06d]     " fmt, __LINE__, ## args)
#define SEM_WARN(fmt, args...)            printf("[SEM USER WARN][%06d]    " fmt, __LINE__, ## args)
#define SEM_PRINT(fmt, args...)           printf("[SEM USER][%06d]         " fmt, __LINE__, ## args)
#define SEM_DEBUG(fmt, args...)           printf("[SEM USER DEBUG][%06d]         " fmt, __LINE__, ## args)
										
#define SEM_PM51_ID                       0x01
#define SEM_AEON_ID                       0x02
#define SEM_ARM_MIPS_ID                   0x03

#if (defined(MCU_AEON))
    #define    SEM_RESOURCE_ID            SEM_AEON_ID
#else
    #define    SEM_RESOURCE_ID            SEM_ARM_MIPS_ID
#endif

//define mutex
#define SEM_MUTEX_CREATE(_index)          _s32SEMMutex[_index] = MsOS_CreateMutex(E_MSOS_FIFO, _SEMMutexName[_index], MSOS_PROCESS_SHARED)
#define SEM_MUTEX_LOCK(_index, _waitms)   OS_OBTAIN_MUTEX(_s32SEMMutex[_index], _waitms)
#define SEM_MUTEX_UNLOCK(_index)          OS_RELEASE_MUTEX(_s32SEMMutex[_index])
#define SEM_MUTEX_DELETE(_index)          OS_DELETE_MUTEX(_s32SEMMutex[_index])

static MS_S32 _s32SEMMutex[SEM_MAX_NUM + 1] = {-1};
static char _SEMMutexName[SEM_MAX_NUM][13] = {"SEMMUTEX0","SEMMUTEX1","SEMMUTEX2","SEMMUTEX3","SEMMUTEX4","SEMMUTEX5","SEMMUTEX6","SEMMUTEX7",\
                                              "SEMMUTEX8","SEMMUTEX9","SEMMUTEX10","SEMMUTEX11","SEMMUTEX12","SEMMUTEX13","SEMMUTEX14","SEMMUTEX15"};

static MSIF_Version _drv_sem_version = {
    .DDI = { SEM_DRV_VERSION },
};

//-------------------------------------------------------------------------------------------------
// Gloabal Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Attempt to get mmio base and create mutex
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Init(void)
{
    MS_U32 u32BaseAddr, u32BaseSize;
    MS_S32 s32Index;

    if (!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_SEM))
    {
        MS_CRITICAL_MSG(printf("[%s][%d] Get register base fail\n", __FUNCTION__, __LINE__););
        return FALSE;
    }

    for(s32Index = 0; s32Index < SEM_MAX_NUM; s32Index++)
    {
        SEM_MUTEX_CREATE(s32Index);
#ifdef PRINT
        SEM_DEBUG("_s32SEMMutex[%ld] = %ld\n", s32Index, _s32SEMMutex[s32Index]);
#endif
    }

    return HAL_SEM_SetBank(u32BaseAddr);
}

MS_BOOL MDrv_SEM_Get_Resource(MS_U8 u8SemID, MS_U16 u16ResId)
{
    return HAL_SEM_Get_Resource(u8SemID, u16ResId);
}

MS_BOOL MDrv_SEM_Free_Resource(MS_U8 u8SemID, MS_U16 u16ResId)
{
    return HAL_SEM_Free_Resource(u8SemID, u16ResId);
}


MS_BOOL MDrv_SEM_Reset_Resource(MS_U8 u8SemID)
{
    return HAL_SEM_Reset_Resource(u8SemID);
}

MS_BOOL MDrv_SEM_Get_ResourceID(MS_U8 u8SemID, MS_U16* pu16ResId)
{
    return HAL_SEM_Get_ResourceID(u8SemID, pu16ResId);
}

MS_U32 MDrv_SEM_Get_Num(void)
{
    return HAL_SEM_Get_Num();
}

MS_BOOL MDrv_SEM_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_sem_version;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to lock a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @param  u32WaitMs   \b IN: 0 ~ SEM_WAIT_FOREVER: suspend time (ms) if the mutex is locked
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Lock(eSemId SemId, MS_U32 u32WaitMs)
{
    MS_S16 s16SemId;
    MS_BOOL bRet;
    MS_U32  u32SysOldTime, u32Interval;

    SEM_DEBUG("Lock SemId = %d\n", SemId);
    SEM_DEBUG("Lock u32WaitMs = %ld\n", u32WaitMs);
  
    bRet = FALSE;
    s16SemId = HAL_SEM_GetSemId(SemId);
    u32SysOldTime = MsOS_GetSystemTime();
    u32Interval = 0;
    
    if(s16SemId < 0)
    {
        SEM_ERROR("Lock SemId%d invalid\n", SemId);
        return FALSE;
    }

    SEM_DEBUG("Lock s16SemId = %d\n", s16SemId);

    /*blocking*/
    if(u32WaitMs == SEM_WAIT_FOREVER) 
    {
        bRet = SEM_MUTEX_LOCK(s16SemId, MSOS_WAIT_FOREVER); 

        if(bRet == FALSE)
        {
           SEM_ERROR("Obtain mutex %s failed\n", _SEMMutexName[s16SemId]); 
        }
        else
        {
            do
            {
                bRet = HAL_SEM_Get_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);
            }while(bRet!= TRUE);

            if(bRet == FALSE)
            {
                SEM_ERROR("Obtain hardware semaphore %d failed\n", s16SemId); 
            }
        }
    }
    /*blocking with timeout*/
    else
    {
    
        bRet = SEM_MUTEX_LOCK(s16SemId, (u32WaitMs - u32Interval)); 

        if(bRet == FALSE)
        {
           SEM_ERROR("Obtain mutex %s failed\n", _SEMMutexName[s16SemId]); 
        }
        else
        {
            do
            {
                bRet = HAL_SEM_Get_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);
                u32Interval = MsOS_GetSystemTime() - u32SysOldTime ;
            }while((bRet!= TRUE) && (u32Interval < u32WaitMs));

            if(bRet == FALSE)
            {
                SEM_ERROR("Obtain hardware semaphore %d failed, timeout=%ld\n", s16SemId, u32WaitMs); 
            }
        }
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Unlock(eSemId SemId)
{
    MS_S16 s16SemId;
    MS_BOOL bRet;

    bRet = FALSE;
    SEM_DEBUG("Unlock SemId = %d\n", SemId);
    s16SemId = HAL_SEM_GetSemId(SemId); 
    
    if(s16SemId < 0)
    {
        SEM_ERROR("Unlock SemId%d invalid\n", SemId);
        return FALSE;
    }

    SEM_DEBUG("Unlock s16SemId = %d\n", s16SemId);

    bRet = SEM_MUTEX_UNLOCK(s16SemId);

    if(bRet == FALSE)
    {
        SEM_ERROR("Release mutex %s failed\n", _SEMMutexName[s16SemId]); 
    } 
    else
    {
        bRet = HAL_SEM_Free_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);

        if(bRet == FALSE)
        {
           SEM_ERROR("Release hardware semaphore %d failed\n", s16SemId); 
        }
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to delete a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Delete(eSemId SemId)
{
    MS_S16 s16SemId;
    MS_BOOL bRet;

    bRet = FALSE;
    SEM_DEBUG("Delete SemId = %d\n", SemId);
    s16SemId = HAL_SEM_GetSemId(SemId); 
    
    if(s16SemId < 0)
    {
        return FALSE;
    }

     bRet = SEM_MUTEX_UNLOCK(s16SemId);

    if(bRet == FALSE)
    {
        SEM_ERROR("Release mutex %s failed\n", _SEMMutexName[s16SemId]); 
    }
    else
    {
        bRet = HAL_SEM_Reset_Resource((MS_U8)s16SemId);

        if(bRet == FALSE)
        {
            SEM_ERROR("Reset hardware semaphore %d failed\n", s16SemId); 
        }
    }

    return bRet;
}

/////////////////////////////////////////////////////////////////
//////// UTOPIA2 CODE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////
enum
{
    SEM_POOL_ID_SEM0 = 0
} eSEM_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register module
//--------------------------------------------------------------------------------------------------
void SEMRegisterToUtopia(void)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_SEM, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)SEMOpen, (FUtopiaClose)SEMClose, (FUtopiaIOctl)SEMIoctl);

    // 2. Resource register
    void* psResource = NULL;
    
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, SEM_POOL_ID_SEM0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("sem0", sizeof(SEM_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, SEM_POOL_ID_SEM0);

    //UtopiaResourceCreate("sem1", sizeof(SEM_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, SEM_POOL_ID_SEM0);
    
    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, SEM_POOL_ID_SEM0);

    // Duplicated Init, check this(FIX LATER)
#ifdef PRINT
    printf("\033[35mFunction = %s, Line = %d, SEM_Init is opened\033[m\n", __PRETTY_FUNCTION__, __LINE__);
#endif
    MDrv_SEM_Init();
#ifdef PRINT
    printf("\033[35mFunction = %s, Line = %d, SEM Register finished\033[m\n", __PRETTY_FUNCTION__, __LINE__);
#endif
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use SEM
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 SEMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // for multi-process safe, check if already other Instance exist
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SEM, &pModule);

    if(UtopiaResourceObtain(pModule, SEM_POOL_ID_SEM0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSEMResPri = NULL;    
    UtopiaResourceGetPrivate(pResource, &pSEMResPri); 

    printf("\033[35mFunction = %s, Line = %d, current resource pri_shm content is: %lu\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(MS_U32 *)pSEMResPri);

    if(*(MS_U32 *)pSEMResPri == 0)
    {
        *(MS_U32 *)pSEMResPri = 1;
        UtopiaResourceRelease(pResource);
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, [SEM INFO] can not open an INSTANCE\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        UtopiaResourceRelease(pResource);
        return 1;
    }
#endif
    printf("\033[35mFunction = %s, Line = %d, [SEM INFO] OPEN INSTANCE...\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    SEM_INSTANT_PRIVATE *pSEMPri = NULL;

    // instance is allocated here, also can allocate private for internal use
    UtopiaInstanceCreate(sizeof(SEM_INSTANT_PRIVATE), ppInstance);
    // set the pSEMPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pSEMPri);
    
    // setup func in private and assign the calling func in func ptr in instance private
    pSEMPri->fpSEMGetResource = (IOCTL_SEM_GETRESOURCE)MDrv_SEM_Get_Resource;
    pSEMPri->fpSEMFreeResource = (IOCTL_SEM_FREERESOURCE)MDrv_SEM_Free_Resource;
    pSEMPri->fpSEMResetResource = (IOCTL_SEM_RESETRESOURCE)MDrv_SEM_Reset_Resource;
    pSEMPri->fpSEMGetResourceID = (IOCTL_SEM_GETRESOURCEID)MDrv_SEM_Get_ResourceID;
    pSEMPri->fpSEMGetNum = (IOCTL_SEM_GETNUM)MDrv_SEM_Get_Num;
    pSEMPri->fpSEMGetLibVer = (IOCTL_SEM_GETLIBVER)MDrv_SEM_GetLibVer;
    pSEMPri->fpSEMLock = (IOCTL_SEM_LOCK)MDrv_SEM_Lock;
    pSEMPri->fpSEMUnlock = (IOCTL_SEM_UNLOCK)MDrv_SEM_Unlock;
    pSEMPri->fpSEMDelete = (IOCTL_SEM_DELETE)MDrv_SEM_Delete;

    return 0;
}

MS_U32 SEMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    SEM_INSTANT_PRIVATE* psSEMInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psSEMInstPri);

    PSEM_GETRESOURCE_PARAM pGetResource = NULL;
    PSEM_FREERESOURCE_PARAM pFreeResource = NULL;
    PSEM_RESETRESOURCE_PARAM pResetResource = NULL;
    PSEM_GETRESOURCEID_PARAM pGetResourceID = NULL;
    PSEM_GETLIBVER_PARAM pGetLibVer = NULL;
    PSEM_LOCK_PARAM pLock = NULL;
    PSEM_UNLOCK_PARAM pUnlock = NULL;
    PSEM_DELETE_PARAM pDelete = NULL;

    MS_U32 u32Ret;

    switch(u32Cmd)
    {
        case MDrv_CMD_SEM_Get_Resource:
            printf("SEMIoctl - MDrv_CMD_SEM_Get_Resource\n");
            pGetResource = (PSEM_GETRESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetResource(pGetResource->u8SemID, pGetResource->u16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Free_Resource:
            printf("SEMIoctl - MDrv_CMD_SEM_Free_Resource\n");
            pFreeResource = (PSEM_FREERESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMFreeResource(pFreeResource->u8SemID, pFreeResource->u16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Reset_Resource:
            printf("SEMIoctl - MDrv_CMD_SEM_Reset_Resource\n");
            pResetResource = (PSEM_RESETRESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMResetResource(pResetResource->u8SemID);
            return u32Ret;
        case MDrv_CMD_SEM_Get_ResourceID:
            printf("SEMIoctl - MDrv_CMD_SEM_Get_ResourceID\n");
            pGetResourceID = (PSEM_GETRESOURCEID_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetResourceID(pGetResourceID->u8SemID, pGetResourceID->pu16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Get_Num:
            printf("SEMIoctl - MDrv_CMD_SEM_Get_Num\n");
            *((MS_U32 *)(pArgs)) = psSEMInstPri->fpSEMGetNum(); 
            return 0;
        case MDrv_CMD_SEM_GetLibVer:
            printf("SEMIoctl - MDrv_CMD_SEM_GetLibVer\n");
            pGetLibVer = (PSEM_GETLIBVER_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetLibVer(pGetLibVer->ppVersion);
            return u32Ret;
        case MDrv_CMD_SEM_Lock:
            printf("SEMIoctl - MDrv_CMD_SEM_Lock\n");
            pLock = (PSEM_LOCK_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMLock(pLock->u32SemID, pLock->u32WaitMs);
            return u32Ret;
        case MDrv_CMD_SEM_Unlock:
            printf("SEMIoctl - MDrv_CMD_SEM_Unlock\n");
            pUnlock = (PSEM_UNLOCK_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMUnlock(pUnlock->u32SemID);
            return u32Ret;
        case MDrv_CMD_SEM_Delete:
            printf("SEMIoctl - MDrv_CMD_SEM_Delete\n");
            pDelete = (PSEM_DELETE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMDelete(pDelete->u32SemID);
            return u32Ret;
        default:
            break;
    };

    return 1; // FIXME: error code
}

MS_U32 SEMClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // Restore resource pri_shm content
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SEM, &pModule);
    
    if(UtopiaResourceObtain(pModule, SEM_POOL_ID_SEM0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSEMResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pSEMResPri);
    
    if(*(MS_U32 *)pSEMResPri == 0)
    {
        printf("\033[35mFunction = %s, Line = %d, [SEMClose] Strange resource pri_shm content!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        UtopiaResourceRelease(pResource);
        return 1;
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, [Multi-process Safe] Release an Instance!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        *(MS_U32 *)pSEMResPri = 0;
        UtopiaResourceRelease(pResource);
        return 0;
    }
#endif
    return 0;
}

MS_U32 MDrv_SEM_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS; //SUSPEND_OK
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
       
           //MDrv_SEM_Get_Resource -> Need to store the Regiser value?
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS; //RESUME_OK
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We should not resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL; //SUSPEND_FAILED
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;
}
