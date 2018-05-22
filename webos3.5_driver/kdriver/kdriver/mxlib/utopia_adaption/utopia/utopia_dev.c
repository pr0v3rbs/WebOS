
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>

#else
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/mutex.h>
#include <linux/syscalls.h> // for syscall
#include <linux/uaccess.h>
#endif

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/seq_file.h>

#include "utopia_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "utopia_driver_id.h"
#include "MsTypes.h"
#include "utopia_adp.h"
#include "drvMIU.h"


#include "utopia_dev.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if 1
#define PREFIX(MODULE) \
	extern __attribute__((weak)) MS_U32 MODULE##_adp_Init(FUtopiaIOctl* pIoctl);
INCLUDED_MODULE
#undef PREFIX
#endif
//u32Ret |= MODULE##_adp_Init(((UTOPIA2K_ADP*)filp->private_data)->fpIoctl);

#define MPOOL_DIVIDE_32M_WORKAROUND_PATCH   0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static DEFINE_MUTEX(_Msos_Mapping_Mutex);

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
#define DIVIDE_SIZE     (32<<20)
static MS_BOOL _MPOOL_Divide_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache);
#endif
extern void Utopia2K_resource_collection_Register(void (*callback)(pid_t pid));

//--------------//
// Global Variable
//--------------//
UTOPIA_PID_LIST *g_Utopia_PID_root = NULL;
extern struct mutex _Utopia_Instance_Mutex;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static long utopia_proc_ioctl(struct file *filp, unsigned int cmd, void* arg)
{
    //UADPBypassIoctl(struct file *filp, unsigned int cmd, void* arg,UADP_STRUCT_POINTER_TABLE *pInTable, UADP_STRUCT_POINTER_TABLE *pOutTable);
    UTOPIA_DDI_OPEN_ARG sOpenArg;
    UTOPIA_DDI_IOCTL_ARG sIoctlArg;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    MS_U32 ret = UTOPIA_STATUS_FAIL;
    KDrvMPool_Info_t i;

    switch(cmd){
    case UTOPIA_IOCTL_SetMODULE :
		copy_from_user(&sOpenArg,arg,sizeof(UTOPIA_DDI_OPEN_ARG));

		switch(sOpenArg.u32ModuleID){

        #define PREFIX(MODULE) \
		case MODULE_##MODULE : \
				if (MODULE##_adp_Init != NULL) \
				{ \
					printk("utopia_proc_ioctl: setModule : %d  \n", MODULE_##MODULE); \
					u32Ret |= MODULE##_adp_Init(&(((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl)); \
				}else  \
				{ \
				    printk("utopia_proc_ioctl: setModule error : %d  \n", MODULE_##MODULE); \
				    u32Ret = UTOPIA_STATUS_FAIL; \
				    break; \
				} \
                u32Ret = UtopiaOpen(sOpenArg.u32ModuleID,&(((UTOPIA2K_ADP*)(filp->private_data))->pInstant),sOpenArg.u32ModuleVersion,sOpenArg.pAttribute); \
		        break;
        INCLUDED_MODULE
        #undef PREFIX


		default:
		        printk("utopia_proc_ioctl: setModule error, no this module: %ld  \n",sOpenArg.u32ModuleID);
			    break;
			}
		break;
	case UTOPIA_IOCTL_IoctlMODULE:
		copy_from_user(&sIoctlArg,arg,sizeof(UTOPIA_DDI_IOCTL_ARG));
		u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,sIoctlArg.u32Cmd,sIoctlArg.pArg);
        //u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,cmd,arg);
        break;

	case UTOPIA_IOCTL_CloseMODULE:
		//copy_from_user(&sIoctlArg,arg,sizeof(UTOPIA_DDI_IOCTL_ARG));
		u32Ret = UtopiaClose((((UTOPIA2K_ADP*)(filp->private_data))->pInstant));
		break;


    case UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING:

        mutex_lock(&_Msos_Mapping_Mutex);

        if(copy_from_user(&i, (void __user *)arg, sizeof(i)))
        {
            u32Ret = UTOPIA_STATUS_FAIL;
			mutex_unlock(&_Msos_Mapping_Mutex);
			break;
        }

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
		if(_MPOOL_Divide_Mapping(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached))
		{
		    u32Ret = UTOPIA_STATUS_SUCCESS;
		}else
		{
		    u32Ret = UTOPIA_STATUS_FAIL;
		}
#else
		if(MsOS_MPool_Mapping_Dynamic(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached))
		{
		    u32Ret = UTOPIA_STATUS_SUCCESS;
		}else
		{
		    u32Ret = UTOPIA_STATUS_FAIL;
		}
#endif


        //u32Ret = MsOS_MPool_Mapping_Dynamic(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached);
        mutex_unlock(&_Msos_Mapping_Mutex);

        break;


	default:
		printk("utopia_proc_ioctl error :default \n");
        //u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,cmd,arg);
        break;

	}

	return u32Ret;

}

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
static MS_BOOL _MPOOL_Divide_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache)
{
    MS_U32 u32MapAddr, u32RemainSize;

    u32MapAddr = u32Offset;
    u32RemainSize = u32MapSize;

    while(u32RemainSize > DIVIDE_SIZE)
    {
        if (MsOS_MPool_Mapping_Dynamic(u8MiuSel, u32MapAddr, DIVIDE_SIZE, bNonCache))
        {
            u32MapAddr += DIVIDE_SIZE;
            u32RemainSize -= DIVIDE_SIZE;
        }
        else
        {
            return FALSE;
        }
    }

    return MsOS_MPool_Mapping_Dynamic(u8MiuSel, u32MapAddr, u32RemainSize, bNonCache);
}
#endif

static ssize_t utopia_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
    return 0;
}

static int utopia_proc_open(struct inode *inode, struct file *file)
{
	UTOPIA2K_ADP *Tmp;

    Tmp = kzalloc(sizeof(UTOPIA2K_ADP), GFP_KERNEL);
	if (Tmp == NULL)
		return -ENOMEM;

	memset(Tmp,0,sizeof(UTOPIA2K_ADP));

	file->private_data = Tmp;
	return 0;
}

static int utopia_proc_release(struct inode *inode, struct file * file)
{
	 UTOPIA2K_ADP *mmapData = file->private_data ;

	 //file->private_data->fpClose((UTOPIA2K_DDI*)file->private_data);
	 //UtopiaClose((((UTOPIA2K_ADP*)(file->private_data))->pInstant));
	 kfree(mmapData);

	 // iounmap(dev->dmaBuf) ;
	 return 0;

}

static void dump_CurrNode_reserve(UTOPIA_PID_LIST *CurrNode)
{
	int i;
	
	printk("CurrNode->reserve_begin:\n");
	for (i = 0; i < 128; i+=4) {
		printk("0x%08x 0x%08x 0x%08x 0x%08x\n",
			CurrNode->reserve_begin[i], CurrNode->reserve_begin[i+1], CurrNode->reserve_begin[i+2], CurrNode->reserve_begin[i+3]);
	}
	printk("CurrNode->reserve_end:\n");
	for (i = 0; i < 128; i+=4) {
		printk("0x%08x 0x%08x 0x%08x 0x%08x\n",
			CurrNode->reserve_end[i], CurrNode->reserve_end[i+1], CurrNode->reserve_end[i+2], CurrNode->reserve_end[i+3]);
	}
}

extern char moduleNames[][40];
static void utopia_garbage_collection(pid_t pid)
{
    // 1. Trace PID root to search the pid which need to be garbage_collection
    MS_BOOL bFind = FALSE;
    UTOPIA_PID_LIST *CurrNode;
    int count, i;
    int instance_count = 0;
    int is_final;

    //if pid = tgid => process do_exit
    if (pid != current->tgid)
        return;

    //UTOPIA_PID_LIST could be modified by multi-process, so it need mutex to protect.
    //But, CurrNode->instance_list would not be changed, because this process is killed.
    //  so, CurrNode->instance_list does not need protect during garbage_collection.
    mutex_lock(&_Utopia_Instance_Mutex);
    CurrNode = g_Utopia_PID_root;

    while(CurrNode)
    {
        if (pid == CurrNode->pid)
        {
            bFind = TRUE;
            break;
        }
        CurrNode = CurrNode->pNext;
    }
    mutex_unlock(&_Utopia_Instance_Mutex);

    if (!bFind) return; //No need to do garbage_collection.

    // 2. Do Garbage_Collection
    for (count = 0; count < INSTANCE_MAX; count++)
    {
        void* pModule;
        void* pResource;
        MS_U32 u32ResourceStatus = UTOPIA_STATUS_SUCCESS;
        is_final = 1;

        //check if UTOPIA_PID_LIST is corrupted
        for (i = 0; i < 128; i++) {
            if ((CurrNode->reserve_begin[i] != 0xdeadbeef) || (CurrNode->reserve_end[i] != 0xdeadbeef)) {
                printk("%s() %d [Utopia2K] UTOPIA_PID_LIST is corrupted!!\n", __FUNCTION__, __LINE__);
                dump_CurrNode_reserve(CurrNode);
                break;
            }
        }

        if (NULL == CurrNode->instance_list[count])
//       if(0x80000000 > CurrNode->instance_list[count]) //skip user mode instant
            continue;

        pr_info("[DEBUG:%s] pid=%d CurrNode=%p, CurrNode->instance_list[count]=%p\n",
            __FUNCTION__, pid, CurrNode, CurrNode->instance_list[count]);
//       dump_stack();
        UtopiaInstanceGetModule(CurrNode->instance_list[count], &pModule);

        printk("pid=%d tgid=%d module=%s module_id=%d\n", pid, current->tgid, moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID], TO_MODULE_PTR(pModule)->u32ModuleID);

        if ((0==strcmp(moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID], "MODULE_AUDIO")) ||
            (0==strcmp(moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID], "MODULE_VDEC_EX")))
        {
            printk("%s do step3: %s\n", __FUNCTION__, moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
            UtopiaIoctl(CurrNode->instance_list[count], CMD_USIGNAL_PROCESS_KILL, &pid);
        }
        else
        {
            printk("%s skip step3: %s\n", __FUNCTION__, moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
        }

        if (NULL == CurrNode->instance_list[count])
        {
            printk("%s [NOTICE!] CurrNode->instance_list[%d]=%d",__FUNCTION__,count,CurrNode->instance_list[count]);
            continue;
        }

        for(i = 0; i < INSTANCE_MAX; i++)
        {
            if(NULL != CurrNode->instance_list[i] &&
                CurrNode->instance_list[i] != CurrNode->instance_list[count]) {
                is_final = 0;
                break;
            }
        }
        pr_info("[DEBUG] CurrNode->instance_count=%d\n", CurrNode->instance_count);
        // 5. Close
        UtopiaClose(CurrNode->instance_list[count]);

        if(is_final)
            break;
    }

    return;
}

const struct file_operations proc_utopia_operations = {
	//.read       = utopia_read,
	.write      = utopia_proc_write,
	.open       = utopia_proc_open,
	.release    = utopia_proc_release,
    .unlocked_ioctl  = (void*)utopia_proc_ioctl,
};

static int __init init_procfs_msg(void)
{

	proc_create("utopia", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH, NULL, &proc_utopia_operations);
	return 0;
}


#define MAJOR_NUM          99
#define MODULE_NAME                "UTOPIA"

void* pInstant = NULL;
void* pArg = NULL;

#ifdef CONFIG_MP_MSTAR_STR_BASE
typedef void (*UtopiaSTREntry)(void);
extern UTOPIA_PRIVATE* psUtopiaPrivate;
extern char moduleNames[][40];
extern void SerPrintf(char *fmt,...);
extern void mstar_set_utopia2k_cbf_pm(UtopiaSTREntry pfUtopiaSave, UtopiaSTREntry pfUtopiaRestore);

static void UtopiaSave_PM(void)
{
    MS_U32 SaveResult;
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;

    while(psUtopiaModule != NULL)
    {
        if (psUtopiaModule->fpSTR != NULL)
        {
            SaveResult = psUtopiaModule->fpSTR(E_POWER_SUSPEND, psUtopiaModule);
        }

        psUtopiaModule = psUtopiaModule->psNext;
    }
}
static void UtopiaRestore_PM(void)
{
    MS_U32 SaveResult;
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;

    //SerPrintf("\nUtopia2K STR Resuming...\n");

    while(psUtopiaModule != NULL)
    {
        if (psUtopiaModule->fpSTR != NULL)
        {
            SaveResult = psUtopiaModule->fpSTR(E_POWER_RESUME, psUtopiaModule);

            if (UTOPIA_STATUS_SUCCESS == SaveResult)
            {
                //SerPrintf("[Utopia2K] Resume %s done.\n", moduleNames[psUtopiaModule->u32ModuleID]);
            }
            else
            {
                SerPrintf("[Utopia2K] Resume %s Fail!\n", moduleNames[psUtopiaModule->u32ModuleID]);
            }
        }
        else
        {
            //SerPrintf("[Utopia2K] %s does not implement STR function\n", moduleNames[psUtopiaModule->u32ModuleID]);
        }

        psUtopiaModule = psUtopiaModule->psNext;
    }
}
#endif

UADP_SPT_0NXT_DEF(MS_U8);
UADP_SPT_0NXT_DEF(MS_U16);
UADP_SPT_0NXT_DEF(MS_U32);

#ifdef CONFIG_MP_MSTAR_STR_BASE
extern int __init mst_str_init(void);
extern void __exit mst_str_exit(void);
#endif

static int __init utopia_init(void)
{

	init_procfs_msg();

#if 0
    if (register_chrdev(MAJOR_NUM, "utopia", &proc_utopia_operations) < 0)
	{
      printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);
      return (-EBUSY);
    }
    printk("<1>%s: started\n", MODULE_NAME);
    // return 0;
#endif
	MsOS_MPool_Init();
	MsOS_Init();
	MDrv_MIU_Init();

#ifdef CONFIG_MP_MSTAR_STR_BASE
	mstar_set_utopia2k_cbf_pm(UtopiaSave_PM, UtopiaRestore_PM);
	mst_str_init();
#endif
    g_Utopia_PID_root = NULL;
    Utopia2K_resource_collection_Register(utopia_garbage_collection);

	UADP_SPT_0NXT(MS_U8);
	UADP_SPT_0NXT(MS_U16);
	UADP_SPT_0NXT(MS_U32);
	return 0;
}



EXPORT_SYMBOL(spt_MS_U8);
EXPORT_SYMBOL(spt_MS_U16);
EXPORT_SYMBOL(spt_MS_U32);



static void utopia_exit(void) {
   //unregister_chrdev(MAJOR_NUM, "utopia");
   //printk("<1>%s: removed\n", MODULE_NAME);
#ifdef CONFIG_MP_MSTAR_STR_BASE
	mst_str_exit();
#endif
}

module_init(utopia_init);
module_exit(utopia_exit);
MODULE_LICENSE("GPL");

