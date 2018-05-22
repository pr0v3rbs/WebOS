//Compile parameter=================================================================================
//1. unit test:
#define SPT_UNIT_TEST 0
//2. compile options:
#define SPT_FORCE_MALLOC            0
//3. debug:
#define SPT_DB_SEQMM_MAX_USAGE      0
#define SPT_DB_MALLOC_FREE          0
#define SPT_DB_MALLOC_FREE_DETAIL   0
#define SPT_DB_MEMINFO 0
#define SPT_DB_DETAIL               0
//==================================================================================================
#if SPT_UNIT_TEST == 0
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/seq_file.h>

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#else
	typedef char * compat_uptr_t;
#endif

#define MALLOC(size)                    kmalloc(size,GFP_KERNEL)
#define FREE(ptr)                      kfree(ptr)
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
//#define spt_printf(argc,...)
#define spt_printf //printk

typedef unsigned long (*fcpy_t)(void *to, const void *from, unsigned long n);

#include "utopia.h"
#include "utopia_adp.h"

#else//SPT_UNIT_TEST==0
#include "ut_spt_inc.h"
#endif//SPT_UNIT_TEST==0
#define SPT_MAX_SIZE 100

//==================================internal tools===============================================
#define SRC_MODE_COPY       0x01
#define SRC_MODE_MALLOC     0x10
#define SRC_MODE_COPY_FROM_USER 0x02

static void SkipCpy(void *to, const void *from,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy)
{
    int i;
    char* pcTo   =(char*)to;
    char* pcFrom =(char*)from;
    unsigned int uiStart=0, uiEnd=0, uiLen=0, uiSelfSize=0;
    if(SPT_DB_DETAIL)spt_printf("to=%p, from=%p\n",pcTo, pcFrom);
    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
                    uiSelfSize = pTable[i].size_or_next;
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    uiEnd = pTable[i].offset - 1;
                    uiLen = uiEnd+1-uiStart;     //include uiEnd last byte
                    if(SPT_DB_DETAIL)spt_printf("UADP_SPT_POINTER_TO_NEXT uiStart=%u,uiEnd=%u,uiLen=%u,off=%u,ptr_size=%u\n",uiStart,uiEnd,uiLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+uiStart,pcFrom+uiStart,uiLen);
                    uiStart = pTable[i].offset+sizeof(pcTo);
                    break;
            case UADP_SPT_END:
                    uiEnd = uiSelfSize - 1;
                    uiLen = uiEnd+1-uiStart;     //include uiEnd last byte
                    if(SPT_DB_DETAIL)spt_printf("UADP_SPT_END uiStart=%u,uiEnd=%u,uiLen=%u,size=%u\n",uiStart,uiEnd,uiLen,uiSelfSize);
                    fcpy(pcTo+uiStart,pcFrom+uiStart,uiLen);
                    return;
        }
    }
}

static int StructRecursiveCpy(void** ppDest, void* pSrc,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy, int iMode)
{
    int ret=0;
    int i;
    char* pcBuffer=NULL;
    char* pcSrc=(char*)pSrc;

    if(SPT_DB_DETAIL)spt_printf("ppDest=%p,pTable=%p,pSrc=%p,",ppDest,pTable,pSrc);
    if(pTable==NULL)
    {
        spt_printf("TABLE is NULL\n");
        return ret;
    }

    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
                    if(iMode&SRC_MODE_MALLOC)
                    {
                        pcBuffer = MALLOC(pTable[i].size_or_next);
                        *ppDest = pcBuffer;
                        spt_printf("<malloc %p>\n",*ppDest);
						if(iMode&SRC_MODE_COPY)fcpy(pcBuffer,pSrc,pTable[i].size_or_next);
                    }
                    else
                    {
                        pcBuffer = *ppDest;
                        if(SPT_DB_MALLOC_FREE)spt_printf("<use buf %p>\n",*ppDest);

                        if(iMode&SRC_MODE_COPY)SkipCpy(pcBuffer,pSrc,pTable,fcpy);

                    }
                    if(SPT_DB_DETAIL)spt_printf("pTable[%d].size_or_next=%d\n",i,pTable[i].size_or_next);
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    if(SPT_DB_DETAIL)spt_printf("pTable[%d].offset=%p\n",i,(void*)pTable[i].offset);
                    if(SPT_DB_DETAIL)spt_printf("pcSrc+pTable[i].offset=%p\n",pcSrc+pTable[i].offset);
                    if (iMode & SRC_MODE_COPY_FROM_USER)
                    {
                        if((*((void**)(pcBuffer+pTable[i].offset)))==NULL)
                        {
                            if(SPT_DB_DETAIL)spt_printf("Next pointer is NULL pointer, ignore it.\n");
							break;
                        }
                    }
                    else
                    {
                        if((*((void**)(pSrc+pTable[i].offset)))==NULL)
                        {
                            if(SPT_DB_DETAIL)spt_printf("Next pointer is NULL pointer, ignore it.\n");
							break;
                        }

                    }

                    if (iMode & SRC_MODE_COPY_FROM_USER)
                    {
                        ret += StructRecursiveCpy((void**)(pcBuffer+pTable[i].offset),*((void**)(pcBuffer+pTable[i].offset)),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode);
                    }
                    else
                    {
                        compat_uptr_t pNextPtr_CopyFromUsr = NULL;
                        if (copy_from_user(&pNextPtr_CopyFromUsr, (void **)(pcBuffer+pTable[i].offset), sizeof(pNextPtr_CopyFromUsr)) != 0)
                            if(SPT_DB_DETAIL)spt_printf("Uer space pointer is NULL pointer:%p, %p.\n", pNextPtr_CopyFromUsr,  *((void **)(pcBuffer+pTable[i].offset)));
                        ret += StructRecursiveCpy((void**)(&pNextPtr_CopyFromUsr),*((void**)(pcSrc+pTable[i].offset)),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode);
                    }
                    break;
            case UADP_SPT_END:
                    return ret;
        }
    }

    spt_printf("SPT table size over flow > %d\n",SPT_MAX_SIZE);
    ret += 1;
    return ret;
}

static int StructRecursiveFree(void** ppDest,const UADP_STRUCT_POINTER_TABLE *pTable)
{
    int ret=0;
    int i;

    if(SPT_DB_DETAIL)spt_printf("ppDest=%p,pTable=%p,\n",ppDest,pTable);
    if(pTable==NULL)return ret;

    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    if(SPT_DB_DETAIL)spt_printf("pTable[%d].offset=%p\n",i,(void*)pTable[i].offset);
                    ret += StructRecursiveFree((void**)(*ppDest+pTable[i].offset),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next));
                    break;
            case UADP_SPT_END:
                    if(SPT_DB_DETAIL)spt_printf("<free *ppDest=%p>\n",*ppDest);
                    FREE(*ppDest);
                    *ppDest=NULL;
                    return ret;
        }
    }

    spt_printf("SPT table size over flow > %d\n",SPT_MAX_SIZE);
    ret += 1;
    return ret;
}

#if SPT_DB_MEMINFO
static void ShowMemInfo(char* str)
{
    char buff[100];
    spt_printf("%s\n",str);
    sprintf(buff,"pmap -x %d | grep total",getpid());
    system(buff);
    spt_printf("\n");
}
#else//SPT_DB_MEMINFO
#define ShowMemInfo(a)
#endif//SPT_DB_MEMINFO

#if SPT_DB_SEQMM_MAX_USAGE
static void OverFlowAssert(void* pInstance,unsigned int u32Cmd,void* pMemorySpace)
{

    unsigned int u32ModuleID;
    if(SequenceMMCheckFlag(pMemorySpace,SEQMM_FLAG_OVERFLOW))
    {
        UtopiaInstanceGetModuleID(pInstance,&u32ModuleID);
        spt_printf("UADPBypassIoctl overflow: MODULE=%x, CMD=%x, usage/space=%u/%u\n",u32ModuleID,u32Cmd,SequenceMMGetUsage(pMemorySpace),SequenceMMGetSpace(pMemorySpace));
        while(1);
    }
}
#else //SPT_DB_SEQMM_MAX_USAGE
#define SeqMMAssert(a)
#endif//SPT_DB_SEQMM_MAX_USAGE
//==================================public functions===============================================
int UADPBypassSetSPT(UADP_STRUCT_POINTER_TABLE* pSpt, unsigned int uiType, unsigned int uiOffset, unsigned int uiSizeOrNext)
{
    if(SPT_DB_DETAIL)spt_printf("uiType=%x,uiOffset=%x,uiSizeOrNext=%x\n",uiType,uiOffset,uiSizeOrNext);
    pSpt->type          = uiType;
    pSpt->offset       = uiOffset;
    pSpt->size_or_next   = uiSizeOrNext;

    return 0;   //FIXME: It didn't define return value.
}

int UADPBypassIoctl(void* pInstance, unsigned int u32Cmd, void* pArgs,const UADP_STRUCT_POINTER_TABLE *pInTable,const UADP_STRUCT_POINTER_TABLE *pOutTable, void* pWorkBuffer, int iWorkBufferSize)
{
    void* karg=NULL;
    const UADP_STRUCT_POINTER_TABLE *FreeTable=NULL;

    //1. generate (in_table_id,kernel_argc) space, and copy user_argc to (in_table_id,kernel_argc)
    if(SPT_DB_MEMINFO)ShowMemInfo("before malloc:\n");
    if(pInTable) //malloc(for in/out) and copy
    {
        FreeTable=pInTable;
        StructRecursiveCpy(&karg,pArgs,pInTable,CPY_FROM_USER,SRC_MODE_COPY|SRC_MODE_MALLOC|SRC_MODE_COPY_FROM_USER);
    }
    else if(pOutTable) //malloc(for out) only
    {
        //need malloc space for karg for output space
        FreeTable=pOutTable;
        StructRecursiveCpy(&karg,pArgs,pOutTable,CPY_FROM_USER,SRC_MODE_MALLOC|SRC_MODE_COPY_FROM_USER);
    }
    if(SPT_DB_MEMINFO)ShowMemInfo("after malloc:\n");
    //2. call fd->CMD_ID(fd, CMD_ID, kernel_argc)
    UtopiaIoctl(pInstance,u32Cmd,karg);
    //3. copy (out_table_id,kernel_argc) to user_argc
    if(pOutTable)
    {
        StructRecursiveCpy(&pArgs,karg,pOutTable,CPY_to_USER,SRC_MODE_COPY);
    }
    //4. free(in_table_id,kernel_argc);
    if(FreeTable)
    {
        StructRecursiveFree(&karg,FreeTable);
    }
    if(SPT_DB_MEMINFO)ShowMemInfo("after free:\n");

    return 0;   //FIXME: It didn't define return value.
}