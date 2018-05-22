#include "utopia_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "utopia_driver_id.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

UTOPIA_PRIVATE* psUtopiaPrivate;

char moduleNames[][40] = {
#define PREFIX(MODULE) "MODULE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char rpoolNames[][40] = {
#define PREFIX(MODULE) "RPOOL_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char ResourceNames[][40] = {
#define PREFIX(MODULE) "RESOURCE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

MS_U32 UTOPIARegisterToUtopia(FUtopiaOpen ModuleType);
#define PREFIX(MODULE) \
	extern __attribute__((weak)) MS_U32 MODULE##RegisterToUtopia(FUtopiaOpen ModuleType);
_INCLUDED_MODULE
#undef PREFIX

MS_U32 UtopiaInit()
{
	MS_U32 u32Ret = 0;
#ifdef PRINT
	printu("[utopia info] utopia init\n" );
#endif
    psUtopiaPrivate = (UTOPIA_PRIVATE*)malloc(sizeof(UTOPIA_PRIVATE));
	memset(psUtopiaPrivate, 0, sizeof(UTOPIA_PRIVATE));
	psUtopiaPrivate->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO
			, "UtopiaPrivateMutex", MSOS_PROCESS_SHARED);

	MsOS_ObtainMutex(psUtopiaPrivate->u32MutexID, MSOS_WAIT_FOREVER);
    UTOPIARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UTOPIA_FULL);
#define PREFIX(MODULE) \
	if (MODULE##RegisterToUtopia != NULL && MODULE_TYPE_##MODULE##_FULL) \
	{ \
		u32Ret |= MODULE##RegisterToUtopia((FUtopiaOpen)MODULE_TYPE_##MODULE##_FULL); \
	} 
	_INCLUDED_MODULE
#undef PREFIX
	MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);

	return u32Ret;
}


MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** ppInstanceTmp
		, MS_U32 u32ModuleVersion, const void* const pAttribute)
{
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;


#if 1//defined(MSOS_TYPE_LINUX_KERNEL)
    //UTOPIA_USER_INSTANCE ** ppInstance= (UTOPIA_USER_INSTANCE**)ppInstanceTmp;
    MS_S32              _s32UtopiaFd = -1;
	UTOPIA_DDI_OPEN_ARG sOpenArg;
	UTOPIA_USER_INSTANCE* pInstance = NULL;
	
	while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == (u32ModuleID&~KERNEL_MODE))        
        {
            pInstance= malloc(sizeof(UTOPIA_USER_INSTANCE));

            if(pInstance == NULL)
            {
                printu("utopia.c : malloc fail %d \n",__LINE__);
                return UTOPIA_STATUS_FAIL;
            }else
            {
                printu("utopia.c : malloc ok,Module= %s \n",moduleNames[u32ModuleID&~KERNEL_MODE]);
            }
		
             if(u32ModuleID & KERNEL_MODE)			    
			 {
			     if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
			     {
			         printu("aa : 0x%lx  Open /proc/utopia fail aa\n",u32ModuleID);
                     free(pInstance);
					 return UTOPIA_STATUS_FAIL;
			     }
                     printu("Module : %s  Open /proc/utopia ok\n",moduleNames[u32ModuleID&~KERNEL_MODE]);
    				 printu("_s32UtopiaFd : %ld  \n",_s32UtopiaFd);
				 sOpenArg.u32ModuleID = psUtopiaModule->u32ModuleID;
				 sOpenArg.u32ModuleVersion = u32ModuleVersion;
				 sOpenArg.pAttribute = (void*)pAttribute;
				 if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_SetMODULE,&sOpenArg)!=UTOPIA_STATUS_SUCCESS)
				 {
				     printu("utopia.c : UtopiaOpen fail %d \n",__LINE__);
				     free(pInstance);
					 close(_s32UtopiaFd);
				     return UTOPIA_STATUS_FAIL;
				 }
				 //while(1);
			     pInstance->psUtopiaInstant = (void*)_s32UtopiaFd;
			     pInstance->u32KernelSpaceIdentify =KERNEL_MODE;
				 pInstance->u32ModuleID = psUtopiaModule->u32ModuleID;
				 *ppInstanceTmp = pInstance;
				 return UTOPIA_STATUS_SUCCESS;
             }else
             {
                 int ret = psUtopiaModule->fpOpen(&(pInstance->psUtopiaInstant), pAttribute);
				 
				 if(ret)
                 {
                     printu("[utopia open error] fail to create instance\n");
			             free(pInstance);
				     RET_OR_BLOCK(ret);
                 }
                 ((UTOPIA_INSTANCE*)(pInstance->psUtopiaInstant))->psModule =psUtopiaModule;
				 ((UTOPIA_INSTANCE*)(pInstance->psUtopiaInstant))->u32AppRequireModuleVersion = u32ModuleVersion;
				 //(*ppInstance)->psModule = psUtopiaModule;
			     //(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
				 pInstance->u32KernelSpaceIdentify = 0;
				 
				 *ppInstanceTmp = pInstance;
				 return ret; /* depend on fpOpen, may not be UTOPIA_STATUS_SUCCESS */
             }
        }
		psUtopiaModule = psUtopiaModule->psNext;
    }
	return UTOPIA_STATUS_FAIL;
#else
    UTOPIA_INSTANCE** ppInstance = (UTOPIA_INSTANCE**)ppInstanceTmp;

    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == u32ModuleID)
        {
            int ret = psUtopiaModule->fpOpen((void**)ppInstance, pAttribute);
            
            if(ret)
            {
                printu("[utopia open error] fail to create instance\n");
				RET_OR_BLOCK(ret);
            }
            
            (*ppInstance)->psModule = psUtopiaModule;
			(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
            return ret; /* depend on fpOpen, may not be UTOPIA_STATUS_SUCCESS */
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return UTOPIA_STATUS_FAIL;
#endif
}
extern void print_trace(void);

MS_U32 UtopiaIoctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{

#if 1//defined(MSOS_TYPE_LINUX_KERNEL)
    UTOPIA_DDI_IOCTL_ARG sIOCTLArg;
    UTOPIA_USER_INSTANCE* pInstance = (UTOPIA_USER_INSTANCE*)pInstanceTmp;
    int ret = 0;
	
	/* check param. */
	if (pInstance == NULL)
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
			
	if((pInstance->u32KernelSpaceIdentify) & KERNEL_MODE)
	{
		sIOCTLArg.u32Cmd = u32Cmd;
		sIOCTLArg.pArg = pArgs;

		ret = ioctl((MS_S32)(pInstance->psUtopiaInstant),UTOPIA_IOCTL_IoctlMODULE,&sIOCTLArg);
		if(ret != 0)
		{
		    
		    printf("UtopiaIoctl error %d \n",ret);
			printf("Module: %s  Cmd: %ld \n",moduleNames[pInstance->u32ModuleID] , u32Cmd);
			//print_trace();
		    //RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
		}
		
	    //ioctl(((MS_S32)(pInstance->psUtopiaInstant)),u32Cmd,pArgs);
		return ret;
	}else
	{
	   return TO_INSTANCE_PTR((pInstance->psUtopiaInstant))->psModule->fpIoctl((pInstance->psUtopiaInstant), 
			u32Cmd, pArgs);
	}
#else
	UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstanceTmp;
	/* check param. */
	if (pInstance == NULL)
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	return TO_INSTANCE_PTR(pInstance)->psModule->fpIoctl(pInstance, 
			u32Cmd, pArgs);
#endif
}

MS_U32 UtopiaClose(void* pInstantTmp)
{
#if 1
    UTOPIA_USER_INSTANCE* pInstance = (UTOPIA_USER_INSTANCE*)pInstantTmp;
    //UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;

    if((pInstance->u32KernelSpaceIdentify) & KERNEL_MODE)
    {
        u32Ret = ioctl((MS_S32)(pInstance->psUtopiaInstant),UTOPIA_IOCTL_CloseMODULE,NULL);
        if(u32Ret==0)
        {
            close((int)(pInstance->psUtopiaInstant));
            free(pInstance);
        }
        else
        {
            printf("UtopiaClose fail\n");
        }
        return u32Ret;//fix me   
    }
    else
	{
        while(psUtopiaModule != NULL)
        {
            if(psUtopiaModule->u32ModuleID == (TO_INSTANCE_PTR(pInstance->psUtopiaInstant))->psModule->u32ModuleID)
            {
                u32Ret = psUtopiaModule->fpClose(pInstance->psUtopiaInstant);
				if(u32Ret==UTOPIA_STATUS_SUCCESS)
				{
                    //printf("utopia.c : UtopiaClose ok,Module= %s \n",moduleNames[psUtopiaModule->u32ModuleID]);
					free(pInstantTmp);
				}
				else
					printf("UtopiaClose fail : Module  = %s \n",moduleNames[psUtopiaModule->u32ModuleID]);
				
				
				return u32Ret;
            }
            psUtopiaModule = psUtopiaModule->psNext;
    }
	return 0;
}
#else
    UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    while(psUtopiaModule != NULL)
    {
	    if(psUtopiaModule->u32ModuleID == pInstant->psModule->u32ModuleID)
	    {
		    return psUtopiaModule->fpClose(pInstant);
	    }
	    psUtopiaModule = psUtopiaModule->psNext;
    }
    return 0;

#endif
}
