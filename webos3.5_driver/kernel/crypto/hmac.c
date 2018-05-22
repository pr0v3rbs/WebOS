/*
 * Cryptographic API.
 *
 * HMAC: Keyed-Hashing for Message Authentication (RFC2104).
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * The HMAC implementation is derived from USAGI.
 * Copyright (c) 2002 Kazunori Miyazawa <miyazawa@linux-ipv6.org> / USAGI
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

/*
#include <crypto/internal/hash.h>
#include <crypto/scatterwalk.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/string.h>
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/fb.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

/// data type unsigned char, data length 1 byte
typedef unsigned char                          U8;                                 // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short                         U16;                                // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned int                         U32;                                // 4 bytes
/// data type signed char, data length 1 byte
typedef signed char                          S8;                                 // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                        S16;                                // 2 bytes
/// data type signed int, data length 4 byte
typedef signed int                         S32;                                // 4 bytes

#define MSOS_MEMPOOL_MAX        8

typedef enum
{
    /// Color format RGB555 and Blink.
    E_DRV_GOP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_DRV_GOP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_DRV_GOP_COLOR_ARGB4444        =2,
    /// Color format alpha blink.
    E_DRV_GOP_COLOR_2266      =3,
    /// Color format I8 (256-entry palette).
    E_DRV_GOP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_DRV_GOP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_DRV_GOP_COLOR_ARGB1555        =6,
    /// Color format ARGB8888.  - Andriod format
    E_DRV_GOP_COLOR_ABGR8888        =7,
    /// Color format RGB555/YUV422.
    E_DRV_GOP_COLOR_RGB555YUV422    =8,
    /// Color format YUV422.
    E_DRV_GOP_COLOR_YUV422          =9,
    /// Color format ARGB8888.  - Andriod format
    E_DRV_GOP_COLOR_RGBA5551        =10,
    /// Color format ARGB8888.  - Andriod format
    E_DRV_GOP_COLOR_RGBA4444        =11,

    /// Invalid color format.
    E_DRV_GOP_COLOR_INVALID
} DRV_GOPColorType;

typedef struct
{
    U16 u16HStart;      //!< unit: pix
    U16 u16HEnd;        //!< unit: pix
    U16 u16VStart;      //!< unit: pix
    U16 u16VEnd;        //!< unit: pix
    U32 u16Pitch;       //!< unit: Byte
    U32 u32Addr;        //!< unit: pix
    DRV_GOPColorType clrType;       //!< color format of the buffer
} DRV_GWIN_INFO;

typedef struct
{
    U32 u32Addr;
    U32 u32Size;
    U32 u32PoolID;
    U32 align_unit;
    U8  u32CMD;
} DRV_MPOOL_RECORD;

typedef struct GFX_MEM_INFO_T
{
    U32 MemAddr;  //phy addr
    U32 MemSize;  //memory size
    U32 PoolId;
    U32 align;
    char sPoolName[100];
} GFX_MEM_INFO;

#define MS_FB_IOC_MAGIC  'F'
#define MS_FB_INIT    _IO(MS_FB_IOC_MAGIC,    0x21)
#define _FHD_FB_BUFFER_SIZE  0X800000

typedef void (*_MDrv_GOP_GetGwinInfoP) (int arg1, int arg2, DRV_GWIN_INFO* drvGwinInfo);
typedef void* (*vmallocP) (U32 length);
typedef void (*vfreeP) (U32 addr);
typedef void (*fb_set_suspendP)(struct fb_info* info, int state);
typedef void (*mstar_fb_savePMP)(void);
typedef void (*mstar_fb_restorePMP)(void);
typedef struct fb_info*  (*get_fb_infoP)(unsigned int idx);
typedef signed long (*MDrv_GOP_CreateMemoryPoolP)(unsigned long u32PoolSize,
                           unsigned long u32MinAllocation,
                           void * pPoolAddr,
                           char *pPoolName);
typedef U32 (*MDrv_GOP_AllocMemoryP)(unsigned long u32Size, U32 s32PoolId, U32 align_unit);

_MDrv_GOP_GetGwinInfoP my_MDrv_GOP_GetGwinInfo = (_MDrv_GOP_GetGwinInfoP)0x8052f7a4;
//vmallocP vmalloc = (vmallocP)0x8014e2b0;
//vfreeP vfree = (vfreeP)0x8014de0c;
fb_set_suspendP my_fb_set_suspend = 0x80379640;
mstar_fb_savePMP my_mstar_fb_savePM = 0x8052c880;
mstar_fb_restorePMP my_mstar_fb_restorePM = 0x8052c6d0;
get_fb_infoP my_get_fb_info = 0x8037ad0c;
MDrv_GOP_CreateMemoryPoolP my_MDrv_GOP_CreateMemoryPool = 0x8052f108;
MDrv_GOP_AllocMemoryP my_MDrv_GOP_AllocMemory = 0x8052f310;

U32* registered_fbPtr = 0x8098477c;
int* num_registered_fbPtr = 0x809847fc;

//DRV_MPOOL_RECORD* g_mpool_cmd_recordP;
U8* Gop0Win0VAddr = NULL;
U8* Gop0Win0FbImage = NULL;  //use to save framebuffer
U32* Gop0Win0VAddrPtr = (U32*)0x80adc738;
U8* addr = NULL;
unsigned long* _fb_phy_addrPtr = (unsigned long*)0x80adc79c;
unsigned long _fb_phy_addr;
// size == 500
DRV_MPOOL_RECORD* g_mpool_cmd_record = 0x80afc9b4;
int* bIsResume = 0x80afc9b0;

int fbdriver_open(struct inode *inode, struct file *filp);
int fbdriver_release(struct inode *inode, struct file *filp);
ssize_t fbdriver_read(struct file *file, char *buf, size_t count, loff_t *ppos);
ssize_t fbdriver_write(struct file *file, const char *buf, size_t count, loff_t *ppos);

struct file_operations fbdriver_fops = {
  read: fbdriver_read,
  write: fbdriver_write,
  open: fbdriver_open,
  release: fbdriver_release
};

int fbdriver_open(struct inode *inode, struct file *filp)
{
  printk("open!\n");
  return 0;
}
int fbdriver_release(struct inode *inode, struct file *filp)
{
  printk("close!\n");
  return 0;
}

int _create_pool(void)
{
    GFX_MEM_INFO* GaPoolinfo = 0x80adbec0;
    U32 bus_addr;
    if(GaPoolinfo->MemAddr >= 0xC0000000)
    {
        printk("[%s] GaPoolinfo.MemAddr is bigger than 0xC0000000\n",__FUNCTION__);
        return -EFAULT;
    }
    bus_addr = GaPoolinfo->MemAddr + 0x20000000;

    //pool_base = (U32)((char __iomem *) ioremap_nocache(bus_addr, GaPoolinfo.MemSize));
    //printk("[%s] Pool: %s bus_addr:%x Size:%x PA_Addr:%x pool_base:%x\n",__FUNCTION__,GaPoolinfo.sPoolName,bus_addr,GaPoolinfo.MemSize,GaPoolinfo.MemAddr,ga_buf_mapping_info.va_base);

    //memset((void*)ga_buf_mapping_info.va_base,0x00,GaPoolinfo.MemSize);
    GaPoolinfo->PoolId = my_MDrv_GOP_CreateMemoryPool(GaPoolinfo->MemSize,0x0,(void *)0xac200000, GaPoolinfo->sPoolName);

    if(GaPoolinfo->PoolId >= MSOS_MEMPOOL_MAX )
    {
        printk("[%s] Pool_id: %d\n",__FUNCTION__,GaPoolinfo->PoolId);
        return -ENOTTY;
    }
    printk("[%s] Pool_id: %d\n",__FUNCTION__,GaPoolinfo->PoolId);

    return 0;
}

ssize_t fbdriver_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
//    char command[1024];

//    addr = *(UINT32*)((unsigned char*)&wctx->pixel + 16);
//    printk("input count = %d\n", count);

//    memset(command, 0, 1024);
//    copy_from_user(command, buf, count);
//    command[count - 1] = '\0';

    return 0;
}

ssize_t fbdriver_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{

        unsigned long p = *ppos;
        struct fb_info *info = my_get_fb_info(0);
        u8 *buffer, *dst;
        u8 __iomem *src;
        int c, cnt = 0, err = 0;
        unsigned long total_size;

        printk("size - 0x%x\n", info->screen_size);

        if (!info || ! info->screen_base)
        {
                printk("ENODEV\n");
                return -ENODEV;
        }

        if (info->state != FBINFO_STATE_RUNNING)
        {
                printk("EPERM\n");
                return -EPERM;
        }

        if (info->fbops->fb_read)
        {
                printk("fb_read\n");
                return info->fbops->fb_read(info, buf, count, ppos);
        }

        total_size = info->screen_size;

        if (total_size == 0)
                total_size = info->fix.smem_len;

        if (p >= total_size)
                return 0;

        if (count >= total_size)
                count = total_size;

        if (count + p > total_size)
                count = total_size - p;

        buffer = kmalloc((count > PAGE_SIZE) ? PAGE_SIZE : count,
                         GFP_KERNEL);
        if (!buffer)
        {
                printk("kmalloc fail!\n");
                return -ENOMEM;
        }

        src = (u8 __iomem *) (info->screen_base + p);

        if (info->fbops->fb_sync)
                info->fbops->fb_sync(info);

        while (count) {
                c  = (count > PAGE_SIZE) ? PAGE_SIZE : count;
                dst = buffer;
                memcpy_fromio(dst, src, c);
                //fb_memcpy_fromfb(dst, src, c);
                dst += c;
                src += c;

                if (copy_to_user(buf, buffer, c)) {
                        err = -EFAULT;
                        break;
                }
                *ppos += c;
                buf += c;
                cnt += c;
                count -= c;
        }

        kfree(buffer);

        return (err) ? err : cnt;
}

static int __init module_start(void)
{
    int ret = 0;
    int i = 0;
    U8* image = NULL;

    ret = register_chrdev(30, "fbdriver", &fbdriver_fops);
    if (ret < 0)
    {
        printk("obtain major number fail 30\n");
        return ret;
    }

    //my_fb_set_suspend(registered_fbPtr[i], FBINFO_STATE_RUNNING);
    //my_mstar_fb_savePM();

/*
     *bIsResume = 1;   
    _create_pool();
    addr = (U8*)*Gop0Win0VAddrPtr;
    printk("fb addr = %x\n", addr);
    image = vmalloc(8294400);
    memset((void*)image, 0x55, 8294400);
    memcpy((void*)addr, image, 8294400);
    vfree(image);
*/

    //printk("fb num - %d\n", *num_registered_fbPtr);
    //for (i = 0; i < 32; i++)
    //    printk("fb %d - %x\n", i, registered_fbPtr[i]);

    printk("hello world!\n");
    return ret;
}

static void __exit module_end(void)
{
    int i = 12;
    DRV_MPOOL_RECORD* ptr;
//    *bIsResume = 1;   
//    _create_pool();

    //for (i = 0; i < 68; i++)
    //for (i = 0; i < 13; i++)
    {
        ptr = (DRV_MPOOL_RECORD*)(g_mpool_cmd_record + 12);
        printk("addr - 0x%x, size - %d\n", ptr->u32Addr, ptr->u32Size);
        for (i = 0; i < ptr->u32Size; i++)
            *((U8*)ptr->u32Addr + i) = (U8)i;
    }

    unregister_chrdev(30, "fbdriver");
    printk("goodbye world!\n");
}

module_init(module_start);
module_exit(module_end);
