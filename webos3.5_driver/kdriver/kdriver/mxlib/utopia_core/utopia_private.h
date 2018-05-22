//*****************************************************************************
//THIS is only for utopia_core internal use only!!!!
//*****************************************************************************
#ifndef _UTOPIA_CORE_H_
#define _UTOPIA_CORE_H_

#include "MsTypes.h"
#include "utopia_dapi.h"

typedef struct _UTOPIA_SHM_ID UTOPIA_SHM_ID;
typedef struct _UTOPIA_TIME_STAMP UTOPIA_TIME_STAMP;
typedef struct _UTOPIA_RESOURCE UTOPIA_RESOURCE;
typedef struct _UTOPIA_RESOURCE_POOL UTOPIA_RESOURCE_POOL;
typedef struct _UTOPIA_MODULE_SHM UTOPIA_MODULE_SHM;
typedef struct _UTOPIA_MODULE UTOPIA_MODULE;
typedef struct _UTOPIA_INSTANCE UTOPIA_INSTANCE;
typedef struct _UTOPIA_PRIVATE UTOPIA_PRIVATE;
typedef struct _UTOPIA_USER_INSTANCE UTOPIA_USER_INSTANCE;
typedef struct _UTOPIA_DDI_OPEN_ARG UTOPIA_DDI_OPEN_ARG;
typedef struct _UTOPIA_DDI_IOCTL_ARG UTOPIA_DDI_IOCTL_ARG;
typedef struct _UTOPIA_PID_LIST UTOPIA_PID_LIST;





#define UTOPIA_SEM_NAME_SIZE 50
#define UTOPIA_RESOURCE_NO_OCCUPY 0
#define INSTANCE_MAX 200

struct _UTOPIA_SHM_ID
{
    char                    name[UTOPIA_SEM_NAME_SIZE];
    MS_U32                  ID;
};

struct _UTOPIA_TIME_STAMP
{
    MS_U32                  u32Time;
    MS_U32                  u32Owner;
};

struct _UTOPIA_RESOURCE
{
    UTOPIA_SHM_ID           shmid_private;
    UTOPIA_SHM_ID           shmid_next_resource;
	UTOPIA_SHM_ID			shmid_self;
	UTOPIA_SHM_ID			shmid_rpool;
    UTOPIA_TIME_STAMP       sLastAccess;
    MS_U32                  u32OccupyInstantPrt;
    MS_BOOL                 bInUse;
    MS_U32                  u32Pid;
};

struct _UTOPIA_RESOURCE_POOL
{
    UTOPIA_SHM_ID           shmid_next_rpool;
    UTOPIA_SHM_ID           shmid_resource_head;
    UTOPIA_SHM_ID           shmid_self;
    MS_U32                  u32ResourceTotal;
    MS_U32                  u32ResourceAvail;
    MS_U32                  u32PoolID;
    MS_U32                  u32ResourcesSemaphore;//semaphore for all resource in this pool
	MS_U32					u32MutexID;
};

struct _UTOPIA_MODULE_SHM
{
    UTOPIA_SHM_ID           shmid_self;
    UTOPIA_SHM_ID           shmid_rpool_head;
    UTOPIA_TIME_STAMP       sLastAccess;
    MS_U32                  u32ResourcePoolTotal;
};

struct _UTOPIA_MODULE
{
    UTOPIA_SHM_ID           shmid_private;
    MS_U32                  u32ModuleID;
    UTOPIA_MODULE*     		psNext;
    UTOPIA_MODULE_SHM* 		psModuleShm;
    FUtopiaOpen       		fpOpen;
    FUtopiaClose            fpClose;
    FUtopiaIOctl            fpIoctl;
    UTOPIA_RESOURCE*        psResource;
    MS_U32                  u32Version;
    MS_U32                  u32DeviceDebugLevel; //bit31 for MS_UTOPIA_DB_LEVEL_TRACE, other bit for user define
    UTOPIA_SHM_ID           shmid_str;
    FUtopiaSTR              fpSTR;
};

struct _UTOPIA_INSTANCE
{
    void*                   pPrivate;
    UTOPIA_INSTANCE*        psNext;
    MS_U32                  u32LastAccessTime;
    MS_U32                  u32OpenThreadName[10];
    UTOPIA_MODULE*     		psModule;
    UTOPIA_RESOURCE*        psResource;
	MS_U32                  u32ModuleVersion;
	MS_U32                  u32AppRequireModuleVersion;
    MS_U32                  u32Pid;
};

struct _UTOPIA_PRIVATE
{
    UTOPIA_MODULE*     		psModuleHead;//point to share memory
    UTOPIA_INSTANCE*        psUtopiaInstantHead;//point to normal memory
	MS_U32					u32MutexID;
};

struct _UTOPIA_USER_INSTANCE
{
    void*                  psUtopiaInstant;
    MS_U32				    u32KernelSpaceIdentify;
	MS_U32                 u32ModuleID;
};


struct _UTOPIA_DDI_OPEN_ARG
{
    MS_U32                  u32ModuleID;
    MS_U32                  u32ModuleVersion;
    void*        pAttribute;
};

struct _UTOPIA_DDI_IOCTL_ARG
{
	MS_U32                  u32Cmd;
    void*                   pArg;
};

struct _UTOPIA_PID_LIST
{
    MS_U32 dummy;
    MS_U32 reserve_begin[128];
    pid_t pid;
    void* instance_list[INSTANCE_MAX];
    int instance_count;
    UTOPIA_PID_LIST* pNext;
    MS_U32 reserve_end[128];
};



#ifndef MSOS_TYPE_LINUX_KERNEL
#define PUMA_BOOT 1
#if 1//PUMA_BOOT
#define MAX_FILE_NUM 200
#define BLOCK_SIZE 64
#define BLOCK_CONTACT_SIZE BLOCK_SIZE-4
#define END_OF_BLOCK_LINKLIST 0xFFFFFFFF
#define HIBERNATE_NAME_SIZE 30



typedef struct _UTOPIA_HIBERNATE_HEADER UTOPIA_HIBERNATE_HEADER;
typedef struct _UTOPIA_HIBERNATE_BLOCK UTOPIA_HIBERNATE_BLOCK;


struct _UTOPIA_HIBERNATE_HEADER
{
    MS_U32                 u32HibernateFileExist;
	char                   HibernateName[HIBERNATE_NAME_SIZE];
	MS_U32                 u32FileSize;
	MS_U32                 u32FirstBlock;
	MS_U32                 bUSE;
};

struct _UTOPIA_HIBERNATE_BLOCK
{ 
    MS_U8  u8File[BLOCK_CONTACT_SIZE];
	MS_U32 u32NextBlock;
};


#endif
#endif	//#ifndef MSOS_TYPE_LINUX_KERNEL


#endif
