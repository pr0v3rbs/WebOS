#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include "fbdev_impl.h"
#include "fbdev_hal.h"
MODULE_LICENSE("GPL");

#define my_OS_OpenFile(pFile,fname,flags,mode)             my_OS_OpenFile_Tag(pFile, fname, flags, mode, " ", __F__, __L__)
#define my_OS_CloseFile(pFile)                             my_OS_CloseFile_Tag(pFile , " ", __F__, __L__)
#define my_OS_WriteFile(pFile,buffer,count)                my_OS_WriteFile_Tag(pFile, buffer, count,  " ", __F__, __L__)
#define my_OS_ReadFile(pFile,buffer,count)                 my_OS_ReadFile_Tag(pFile, buffer, count,  " ", __F__, __L__)

typedef FBDEV_WIN_CTX_T* (*FBDEV_GetWinCtxP) (int layer);
typedef int (*FBDEV_GetOSDEnableP) (int layer);
typedef int (*FBDEV_GetConfigP) (FBDEV_WIN_CTX_T *w, LX_FBDEV_CONFIG_T *config);
typedef int (*FBDEV_SetConfigP) (FBDEV_WIN_CTX_T *w, LX_FBDEV_CONFIG_T *config);
typedef void (*GFX_LockDeviceP) (void);
typedef void (*GFX_UnlockDeviceP) (void);
typedef void* (*vmallocP) (UINT32 length);
typedef void (*vfreeP) (UINT32 addr);
typedef int (*OS_OpenCacheMapP) (OS_CACHE_MAP_T* pMap, ULONG phys_addr, UINT32 length);
typedef int (*OS_InvalCacheMapP) (OS_CACHE_MAP_T* pMap, UINT32 off, UINT32 length);
typedef int (*OS_CleanCacheMapP) (OS_CACHE_MAP_T* pMap, UINT32 off, UINT32 length);
typedef int (*OS_CloseCacheMapP) (OS_CACHE_MAP_T* pMap);
typedef int (*OS_OpenFile_TagP) (OS_FILE_T* pFile, char* fname, int flags, mode_t mode, const char* szFile, const char* szFunc, const int nLine);
typedef int (*OS_CloseFile_TagP) ( OS_FILE_T* pFile, const char* szFile, const char* szFunc, const int nLine );
typedef int (*OS_WriteFile_TagP) (OS_FILE_T* pFile, char* buffer, int count, const char* szFile, const char* szFunc, const int nLine);
typedef int (*OS_ReadFile_TagP) (OS_FILE_T* pFile, char* buffer, int count, const char* szFile, const char* szFunc, const int nLine);

FBDEV_GetWinCtxP my_FBDEV_GetWinCtx = (FBDEV_GetWinCtxP)0x8061e9f4;
FBDEV_GetOSDEnableP my_FBDEV_GetOSDEnable = (FBDEV_GetOSDEnableP)0x8061ef04;
FBDEV_GetConfigP my_FBDEV_GetConfig = (FBDEV_GetConfigP)0x8061aa88;
FBDEV_SetConfigP my_FBDEV_SetConfig = (FBDEV_SetConfigP)0x8061ab44;
GFX_LockDeviceP my_GFX_LockDevice = (GFX_LockDeviceP)0x8062b1e8;
GFX_UnlockDeviceP my_GFX_UnlockDevice = (GFX_UnlockDeviceP)0x8062b220;
vmallocP vmalloc = (vmallocP)0x800f90dc;
vfreeP vfree = (vfreeP)0x800f8cc0;
OS_OpenCacheMapP my_OS_OpenCacheMap = (OS_OpenCacheMapP)0x80474490;
OS_InvalCacheMapP my_OS_InvalCacheMap = (OS_InvalCacheMapP)0x8047455c;
OS_CleanCacheMapP my_OS_CleanCacheMap = (OS_CleanCacheMapP)0x804745f4;
OS_CloseCacheMapP my_OS_CloseCacheMap = (OS_CloseCacheMapP)0x80474504;
OS_OpenFile_TagP my_OS_OpenFile_Tag = (OS_OpenFile_TagP)0x80473d8c;
OS_CloseFile_TagP my_OS_CloseFile_Tag = (OS_CloseFile_TagP)0x80473e18;
OS_WriteFile_TagP my_OS_WriteFile_Tag = (OS_WriteFile_TagP)0x80473b98;
OS_ReadFile_TagP my_OS_ReadFile_Tag = (OS_ReadFile_TagP)0x80473c34;

int fbdriver_open(struct inode *inode, struct file *filp);
int fbdriver_release(struct inode *inode, struct file *filp);
ssize_t fbdriver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t fbdriver_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

struct file_operations fbdriver_fops = {
  read: fbdriver_read,
  write: fbdriver_write,
  open: fbdriver_open,
  release: fbdriver_release
};


static void* FBDEV_SaveFBImage(UINT32 fbmem_addr, UINT32 length)
{
    OS_CACHE_MAP_T cmap;
    void* virt_img = NULL;

    virt_img = vmalloc(length);
    if (virt_img == NULL) return NULL;

    my_OS_OpenCacheMap(&cmap, (ULONG)fbmem_addr, length);
    my_OS_InvalCacheMap(&cmap, 0x0, length);

    memcpy((void*)virt_img, (void*)cmap.virt_addr, length);
    my_OS_CloseCacheMap(&cmap);

    return virt_img;
}

static void FBDEV_RestoreFBImage(UINT32 fbmem_addr, UINT32 idx, UINT32 length, const char* buf)
{
    OS_CACHE_MAP_T cmap;
    if (fbmem_addr == 0x0 || length == 0) return;

    my_OS_OpenCacheMap(&cmap, (ULONG)(fbmem_addr + (idx)), length);
    if (buf == NULL)
        memset((void*)cmap.virt_addr, 0x00, length);
    else
    {
        printk("%x %x %x %x\n", *(unsigned char*)(buf), *(unsigned char*)(buf + 1), *(unsigned char*)(buf + 2), *(unsigned char*)(buf + 3));
        memcpy((void*)cmap.virt_addr, buf, length);
    }

    my_OS_CleanCacheMap(&cmap, 0x0, length);
    my_OS_CloseCacheMap(&cmap);
}

int readFile(unsigned char* fileName, unsigned char* buf, UINT32 length)
{
    OS_FILE_T file;

    if (RET_OK != my_OS_OpenFile(&file, fileName, O_RDONLY | O_LARGEFILE, 0655))
    {
        printk("file open fail!\n");
        return -1;
    }

    //my_OS_WriteFile(&file, buf, length);
    my_OS_ReadFile(&file, buf, length);

    my_OS_CloseFile(&file);

    return 0;
}

int __init module_start(void)
{
    int result = 0;

    result = register_chrdev(30, "fbdriver", &fbdriver_fops);
    if (result < 0)
    {
        printk("obtain major number fail 30\n");
        return result;
    }

    printk("hello fb world!\n");
    return 0;
}

void __exit module_end(void)
{
    unregister_chrdev(30, "fbdriver");
    printk("Goodbye World!!\n");
}

int fbdriver_open(struct inode *inode, struct file *filp) 
{
  return 0;
}
int fbdriver_release(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t fbdriver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    FBDEV_WIN_CTX_T* wctx = NULL;
    UINT32* img = NULL;
    UINT32 addr;

    my_GFX_LockDevice();
    wctx = my_FBDEV_GetWinCtx(0);
    if (my_FBDEV_GetOSDEnable(0) != FALSE)
    {
        addr = *(UINT32*)((unsigned char*)&wctx->pixel + 16);
        img = FBDEV_SaveFBImage(addr, 8294400);
        printk("address = 0x%x\n", (UINT32)addr);
        if (img != NULL)
        {
            printk("virt_img = 0x%x\n", (UINT32)img);
            copy_to_user(buf, img, 8294400);
            vfree((UINT32)img);
        }
    }

    my_GFX_UnlockDevice();
    return 8294400;
}

ssize_t fbdriver_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    FBDEV_WIN_CTX_T* wctx = NULL;
    UINT32 addr;
    char command[1024];

    my_GFX_LockDevice();
    wctx = my_FBDEV_GetWinCtx(0);
    if (my_FBDEV_GetOSDEnable(0) != FALSE && count > 0)
    {
        addr = *(UINT32*)((unsigned char*)&wctx->pixel + 16);
        printk("input count = %d\n", count);

        if (count <= 1024)
        {
            memset(command, 0, 1024);
            copy_from_user(command, buf, count);
            command[count - 1] = '\0';
        }
        else
            command[0] = '0';

        if (command[0] == '0')
        {
            //int data;
            //sscanf(command, "%d", &data);
            //FBDEV_RestoreFBImage(addr, 1920 * 4 * 800, 8294400 - (1920 * 4 * 800), NULL);
            FBDEV_RestoreFBImage(addr, 0, 8294400, NULL);
        }
        // breaking news
        else if (command[0] == '1')
        {
            void* fileData = vmalloc(8294400 - (1920 * 4 * 800));
            if (fileData)
            {
                if (!readFile(command + 1, fileData, 8294400 - (1920 * 4 * 800)))
                {
                    FBDEV_RestoreFBImage(addr, 1920 * 4 * 800, 8294400 - (1920 * 4 * 800), fileData);
                }
                vfree((UINT32)fileData);
            }
        }
        // full screen
        else if (command[0] == '2')
        {
            void* fileData = vmalloc(8294400);
            if (fileData)
            {
                if (!readFile(command + 1, fileData, 8294400))
                {
                    FBDEV_RestoreFBImage(addr, 0, 8294400, fileData);
                }
                vfree((UINT32)fileData);
            }
        }
    }

    my_GFX_UnlockDevice();
    return count;
}

module_init(module_start);
module_exit(module_end);
