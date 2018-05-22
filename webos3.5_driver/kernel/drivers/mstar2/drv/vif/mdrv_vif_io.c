////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
//
// You can redistribute it and/or modify it under the terms of the GNU General Public
// License version 2 as published by the Free Foundation. This program is distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_vif_io.c
/// @brief  VIF Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/version.h>
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <asm/io.h>

#include "mst_devid.h"

#include "mdrv_vif_io.h"
#include "mhal_vif_reg.h"
#include "mdrv_vif.h"
#include "mhal_vif.h"

//#include "mdrv_probe.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define VIF_PRINT(fmt, args...)      //printk("[VIF][%05d] " fmt, __LINE__, ## args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MOD_VIF_DEVICE_COUNT     1
#define MOD_VIF_NAME                 "ModVIF"


#define VIF_DEBUG
#ifdef VIF_DEBUG
#define DEBUG_VIF(x) (x)
#else
#define DEBUG_VIF(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------
static int                      _mod_vif_open (struct inode *inode, struct file *filp);
static int                      _mod_vif_release(struct inode *inode, struct file *filp);
static ssize_t                  _mod_vif_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t                  _mod_vif_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static unsigned int             _mod_vif_poll(struct file *filp, poll_table *wait);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long                      _mod_vif_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else
static int                      _mod_vif_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static int                      _mod_vif_fasync(int fd, struct file *filp, int mode);


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static VIF_ModHandle_t VIFDev=
{
    .s32VIFMajor=               MDRV_MAJOR_VIF,
    .s32VIFMinor=               MDRV_MINOR_VIF,
    .cDevice=
    {
        .kobj=                  {.name= MOD_VIF_NAME, },
        .owner  =               THIS_MODULE,
    },
    .VIFFop=
    {
        .open=                  _mod_vif_open,
        .release=               _mod_vif_release,
        .read=                  _mod_vif_read,
        .write=                 _mod_vif_write,
        .poll=                  _mod_vif_poll,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
        .unlocked_ioctl =       _mod_vif_ioctl,
#else
        .ioctl=                 _mod_vif_ioctl,
#endif
        .fasync =	            _mod_vif_fasync,
    },
};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static int _mod_vif_open (struct inode *inode, struct file *filp)
{
	VIF_ModHandle_t *dev;

    VIF_PRINT("%s is invoked\n", __FUNCTION__);

    dev = container_of(inode->i_cdev, VIF_ModHandle_t, cDevice);
	filp->private_data = dev;

    return 0;
}

static int _mod_vif_release(struct inode *inode, struct file *filp)
{
    VIF_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static ssize_t _mod_vif_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    // remove it if it's not required
    VIF_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static ssize_t _mod_vif_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    // remove it if it's not required
    VIF_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static unsigned int _mod_vif_poll(struct file *filp, poll_table *wait)
{
    VIF_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}


static int _mod_vif_fasync(int fd, struct file *filp, int mode)
{
    VIF_PRINT("%s is invoked\n", __FUNCTION__);

	return fasync_helper(fd, filp, mode, &VIFDev.async_queue);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long _mod_vif_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int _mod_vif_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    S32 s32Err = 0;
//User space - Kernel space variables
    VIF_Init_Info_st vifInitInfoVar;
    VIF_Para_Info_st vifParaInfoVar;

    BOOL bVar = 0;
    VIFSoundSystem vifSoundSystemVar;
    IfFrequencyType ifFreqTypeVar;
    BYTE VifShiftClkVar;
    U8 u8Var = 0;
    FrequencyBand freqBandVar;
    //void __user *argp = (void __user *)arg;

    VIF_PRINT("%s is invoked\n", __FUNCTION__);

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if ((VIF_IOC_MAGIC!= _IOC_TYPE(cmd)) || (_IOC_NR(cmd)> VIF_IOC_MAXNR))
    {
        return -ENOTTY;
    }

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        s32Err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        s32Err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }
    if (s32Err)
    {
        return -EFAULT;
    }

    //PROBE_IO_ENTRY(MDRV_MAJOR_VIF, _IOC_NR(cmd));

    switch(cmd)
    {
        case MDRV_VIF_INIT:
            VIF_PRINT("ioctl: MDRV_VIF_INIT\n");
            if (copy_from_user(&vifInitInfoVar, (VIF_Init_Info_st __user *) arg, sizeof(VIF_Init_Info_st)))
                return -EFAULT;
            VIF_PRINT("Debug====>vifInitInfoVar.u32InitDataLen=%02x\n",vifInitInfoVar.u32InitDataLen);
            MDRV_VIF_Init(vifInitInfoVar.pVIF_InitData, vifInitInfoVar.u32InitDataLen);
            break;
        case MDRV_VIF_SETCLOCK:
            VIF_PRINT("ioctl: MDRV_VIF_SETCLOCK\n");
            if (copy_from_user(&bVar, (BOOL __user *) arg, sizeof(BOOL)))
                return -EFAULT;
            MDRV_VIF_SetClock(bVar);
            break;
        case MDRV_VIF_RESET:
            VIF_PRINT("ioctl: MDRV_VIF_RESET\n");
            MDRV_VIF_Reset();
            break;
        case MDRV_VIF_HANDLER:
            VIF_PRINT("ioctl: MDRV_VIF_HANDLER\n");
            if (copy_from_user(&bVar, (BOOL __user *) arg, sizeof(BOOL)))
                return -EFAULT;
            MDRV_VIF_Handler(bVar);
            break;
        case MDRV_VIF_SET_SOUND_SYSTEM:
            VIF_PRINT("ioctl: MDRV_VIF_SET_SOUND_SYSTEM\n");
            if (copy_from_user(&vifSoundSystemVar, (VIFSoundSystem __user *) arg, sizeof(VIFSoundSystem)))
                return -EFAULT;
            MDRV_VIF_SetSoundSystem(vifSoundSystemVar);
            break;
        case MDRV_VIF_SET_IF_FREQ:
            VIF_PRINT("ioctl: MDRV_VIF_SET_IF_FREQ\n");
            if (copy_from_user(&ifFreqTypeVar, (IfFrequencyType __user *) arg, sizeof(IfFrequencyType)))
                return -EFAULT;
            MDRV_VIF_SetIfFreq(ifFreqTypeVar);
            break;
        case MDRV_VIF_READ_CR_FOE:
            VIF_PRINT("ioctl: MDRV_VIF_READ_CR_FOE\n");
            u8Var = MDRV_VIF_Read_CR_FOE();
            if (copy_to_user((U8 __user *) arg, &u8Var, sizeof(U8)))
                return -EFAULT;
            break;
        case MDRV_VIF_READ_CR_LOCK_STATUS:
            VIF_PRINT("ioctl: MDRV_VIF_READ_CR_LOCK_STATUS\n");
	     u8Var = MDRV_VIF_Read_CR_LOCK_STATUS();
	     if (copy_to_user((U8 __user *) arg, &u8Var, sizeof(U8)))
                return -EFAULT;
            break;
        case MDRV_VIF_BYPASS_DBB_AUDIO_FLT:
            VIF_PRINT("ioctl: MDRV_VIF_BYPASS_DBB_AUDIO_FLT\n");
            if (copy_from_user(&bVar, (BOOL __user *) arg, sizeof(BOOL)))
                return -EFAULT;
            MDRV_VIF_BypassDBBAudioFilter(bVar);
            break;
        case MDRV_VIF_SET_FREQ_BAND:
            VIF_PRINT("ioctl: MDRV_VIF_SET_FREQ_BAND\n");
            if (copy_from_user(&freqBandVar, (FrequencyBand __user *) arg, sizeof(FrequencyBand)))
                return -EFAULT;
            MDRV_VIF_SetFreqBand(freqBandVar);
            break;
        case MDRV_VIF_GET_INPUT_LEVEL_INDICATOR:
            VIF_PRINT("ioctl: MDRV_VIF_GET_INPUT_LEVEL_INDICATOR\n");
            bVar = MDRV_VIF_GetInputLevelIndicator();
            if (copy_to_user((BOOL __user *) arg, &bVar, sizeof(BOOL)))
                return -EFAULT;

            break;
        case MDRV_VIF_SET_PARAMETER:
            VIF_PRINT("ioctl: MDRV_VIF_SET_PARAMETER\n");

            if (copy_from_user(&vifParaInfoVar, (VIF_Para_Info_st __user *) arg, sizeof(VIF_Para_Info_st)))
                return -EFAULT;
            VIF_PRINT("Debug====>vifParaInfoVar.u32DataLen=%02x\n",vifParaInfoVar.u32DataLen);

            vifParaInfoVar.bResult = MDRV_VIF_SetParameter(vifParaInfoVar.paraGroup, vifParaInfoVar.pVIF_Para, vifParaInfoVar.u32DataLen);
            if (copy_to_user((VIF_Para_Info_st __user *) arg, &vifParaInfoVar, sizeof(VIF_Para_Info_st)))
                return -EFAULT;
            break;

         case MDRV_VIF_SHIFTCLK:
            VIF_PRINT("ioctl: MDRV_VIF_ShiftClk\n");
            if (copy_from_user(&VifShiftClkVar, (BYTE __user *) arg, sizeof(BYTE)))
                return -EFAULT;
            MDRV_VIF_ShiftClk(VifShiftClkVar);
            break;

        default:
            VIF_PRINT("ioctl: unknown command\n");
            //PROBE_IO_EXIT(MDRV_MAJOR_VIF, _IOC_NR(cmd));
            return -ENOTTY;
    }


    //PROBE_IO_EXIT(MDRV_MAJOR_VIF, _IOC_NR(cmd));
    return 0;
}

static int __init mod_vif_init(void)
{
    S32         s32Ret;
    dev_t       dev;

    VIF_PRINT("%s is invoked\n", __FUNCTION__);

    if (VIFDev.s32VIFMajor)
    {
        dev = MKDEV(VIFDev.s32VIFMajor, VIFDev.s32VIFMinor);
        s32Ret = register_chrdev_region(dev, MOD_VIF_DEVICE_COUNT, MDRV_NAME_VIF);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, VIFDev.s32VIFMinor, MOD_VIF_DEVICE_COUNT, MDRV_NAME_VIF);
        VIFDev.s32VIFMajor = MAJOR(dev);
    }

    if ( 0 > s32Ret)
    {
        VIF_PRINT("Unable to get major %d\n", VIFDev.s32VIFMajor);
        return s32Ret;
    }

    cdev_init(&VIFDev.cDevice, &VIFDev.VIFFop);
    if (0!= (s32Ret= cdev_add(&VIFDev.cDevice, dev, MOD_VIF_DEVICE_COUNT)))
    {
        VIF_PRINT("Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_VIF_DEVICE_COUNT);
        return s32Ret;
    }

    return 0;
}

static void __exit mod_vif_exit(void)
{
    VIF_PRINT("%s is invoked\n", __FUNCTION__);

    cdev_del(&VIFDev.cDevice);
    unregister_chrdev_region(MKDEV(VIFDev.s32VIFMajor, VIFDev.s32VIFMinor), MOD_VIF_DEVICE_COUNT);
}

module_init(mod_vif_init);
module_exit(mod_vif_exit);
