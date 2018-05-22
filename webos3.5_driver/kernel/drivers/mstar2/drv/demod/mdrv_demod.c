
#define _MDRV_DEMOD_C_
#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "mst_devid.h"
#include "mdrv_types.h"

#include "mdrv_demod.h"
#include "mdrv_demod_io.h"
#include "mdrv_demod_st.h"
#include "mhal_demod.h"


//----------------------------------------------------------------------
#define OPT_DEMOD_DRV_DEBUG
#undef DEMOD_DRV_DBG
#ifdef OPT_DEMOD_DRV_DEBUG
    #define DEMOD_DRV_DBG(fmt, args...)      printk(KERN_WARNING "[DEMOD_DRV][%05d]" fmt, __LINE__, ## args)
#else
    #define DEMOD_DRV_DBG(fmt, args...)
#endif

#undef DEMOD_DRV_DBGX
#define DEMOD_DRV_DBGX(fmt, args...)
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//#define MDRV_NAME_DEMOD                 "demod"
//#define MDRV_MAJOR_DEMOD                0xff
//#define MDRV_MINOR_DEMOD                0x00
#define DEMOD_DEVICE_COUNT    1
//----------------------------------------------------------------------

static U32 _u32DemodMajor = MDRV_MAJOR_DEMOD;
static U32 _u32DemodMinor = MDRV_MINOR_DEMOD;

static U8 sg_dvb_ext_sdram_state = 0x0;
static U8 sg_dvb_ext_sram_state = 0x0;

static struct cdev _stDemodDev;


static int _MDrv_Demod_Open(struct inode *inode, struct file *filp)
{

    return 0;
}

static int _MDrv_Demod_Release(struct inode *inode, struct file *filp)
{

    return 0;
}

//static int _MDrv_Demod_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long _MDrv_Demod_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int _MDrv_Demod_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{

	int err = 0;
    /* don't even decode wrong cmds: better returning  ENOTTY than EFAULT */

    if (_IOC_TYPE(cmd) != DEMOD_IOC_MAGIC)
        return -ENOTTY;

    if (_IOC_NR(cmd) > DEMOD_IOC_MAXNR)
        return -ENOTTY;
    /*
         * the type is a bitmask, and VERIFY_WRITE catches R/W
         * transfers. Note that the type is user-oriented, while
         * verify_area is kernel-oriented, so the concept of "read" and
         * "write" is reversed
         */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (err)
        return -EFAULT;

    switch(cmd)
    {
        case DEMOD_IOC_INIT:

            break;

        case DEMOD_IOC_REG_WRITE:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            MHal_Demod_WriteReg(stReg.u32Address, stReg.u8Value);
        }
            break;

        case DEMOD_IOC_REG_READ:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            MHal_Demod_ReadReg(stReg.u32Address, &stReg.u8Value);
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_WRITE:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_WriteReg(stReg.u32Address, stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_READ:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_ReadReg(stReg.u32Address, &stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;
//Driver update 2009/11/20
       case DEMOD_IOC_MB_DSPREG_WRITE:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_WriteDspReg(stReg.u32Address, stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_MB_DSPREG_READ:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_ReadDspReg(stReg.u32Address, &stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_WRITE_ATSC:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_WriteReg_ATSC(stReg.u32Address, stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_READ_ATSC:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_ReadReg_ATSC(stReg.u32Address, &stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_WRITE_ISDBT:
        {

            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_WriteReg_ISDBT(stReg.u32Address, stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_READ_ISDBT:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_ReadReg_ISDBT(stReg.u32Address, &stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_WRITE_DTMB:
        {

            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_WriteReg_DTMB(stReg.u32Address, stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_MB_REG_READ_DTMB:
        {
            DEMOD_REG_t stReg;
            if (copy_from_user(&stReg, (DEMOD_REG_t __user *) arg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
            if (MHal_Demod_MB_ReadReg_DTMB(stReg.u32Address, &stReg.u8Value) == FALSE)
            {
                return -EIO;
            }
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stReg, sizeof(DEMOD_REG_t)))
            {
                return -EFAULT;
            }
        }
            break;

        case DEMOD_IOC_LOAD_DSP_CODE:
        {
            DEMOD_DSP_PARAM_t stParam;
            if (copy_from_user(&stParam, (DEMOD_DSP_PARAM_t __user *) arg, sizeof(DEMOD_DSP_PARAM_t)))
            {
                return -EFAULT;
            }
// 0402 change for tuner option
            if(MHal_Demod_LoadDSPCode(stParam) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_STOP://mstar 0901 update
        {
            DEMOD_STANDARD_TYPE_t eStd;
            if (copy_from_user(&eStd, (DEMOD_STANDARD_TYPE_t __user *) arg, sizeof(DEMOD_STANDARD_TYPE_t)))
            {
                return -EFAULT;
            }
            if(MHal_Demod_Stop(eStd) == FALSE)
            {
                return -EIO;
            }
        }
            break;

        case DEMOD_IOC_DVBEXT_SET_STATE:
        {
            DEMOD_DVBEXT_STATE_t stDVBEXTState;
            if (copy_from_user(&stDVBEXTState, (DEMOD_DVBEXT_STATE_t __user *) arg, sizeof(DEMOD_DVBEXT_STATE_t)))
            {
                return -EFAULT;
            }
            sg_dvb_ext_sdram_state = stDVBEXTState.sdram_state;
            sg_dvb_ext_sram_state = stDVBEXTState.sram_state;
        }
            break;

        case DEMOD_IOC_DVBEXT_GET_STATE:
        {
            DEMOD_DVBEXT_STATE_t stDVBEXTState;
            stDVBEXTState.sdram_state = sg_dvb_ext_sdram_state;
            stDVBEXTState.sram_state = sg_dvb_ext_sram_state;
            if (copy_to_user((DEMOD_REG_t __user *) arg, &stDVBEXTState, sizeof(DEMOD_DVBEXT_STATE_t)))
            {
                return -EFAULT;
            }
        }
            break;

        default:
            return -ENOTTY;
            break;
    }


    return err;
}


static struct file_operations demod_fops = {
    .owner =    THIS_MODULE,
    .open =     _MDrv_Demod_Open,
    .release =  _MDrv_Demod_Release,
    //.ioctl =    _MDrv_Demod_Ioctl,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
    .unlocked_ioctl =    _MDrv_Demod_Ioctl,
#else
    .ioctl =    _MDrv_Demod_Ioctl,
#endif

};

static int __init Demod_Init_Module(void)
{
    int result;
    dev_t dev = 0;

    if (_u32DemodMajor)
    {
        dev = MKDEV(_u32DemodMajor, _u32DemodMinor);
        result = register_chrdev_region(dev, DEMOD_DEVICE_COUNT, MDRV_NAME_DEMOD);
    }
    else
    {
        result = alloc_chrdev_region(&dev, _u32DemodMinor, DEMOD_DEVICE_COUNT, MDRV_NAME_DEMOD);
        _u32DemodMajor = MAJOR(dev);
    }
    if (result < 0)
    {
        DEMOD_DRV_DBG("can't get major %d\n", _u32DemodMajor);
        return result;
    }

    cdev_init(&_stDemodDev, &demod_fops);
    _stDemodDev.owner = THIS_MODULE;
    _stDemodDev.ops = &demod_fops;
    result = cdev_add (&_stDemodDev, dev, DEMOD_DEVICE_COUNT);

    if (result)
    {
        DEMOD_DRV_DBG("Error %d adding Demod", result);
        unregister_chrdev_region(dev, DEMOD_DEVICE_COUNT);
        return result;
    }

    DEMOD_DRV_DBG("init %d\n", _u32DemodMajor);
	return 0;

}

static void __exit Demod_Cleanup_Module(void)
{
	cdev_del(&_stDemodDev);
	unregister_chrdev_region(MKDEV(_u32DemodMajor, _u32DemodMinor), DEMOD_DEVICE_COUNT);

    DEMOD_DRV_DBG("exit %d\n", _u32DemodMajor);
}

int MDrv_Demod_DVBEXT_GetState(U16 *dvb_ext_state)
{
    *dvb_ext_state = (sg_dvb_ext_sram_state << 8) | sg_dvb_ext_sdram_state;
    return 0;
}
EXPORT_SYMBOL(MDrv_Demod_DVBEXT_GetState);

int MDrv_Demod_DVBEXT_SetState(U16 dvb_ext_state)
{
    sg_dvb_ext_sdram_state = dvb_ext_state & 0xff;
    sg_dvb_ext_sram_state = dvb_ext_state >> 8;
    return 0;
}
EXPORT_SYMBOL(MDrv_Demod_DVBEXT_SetState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////

module_init(Demod_Init_Module);
module_exit(Demod_Cleanup_Module);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("Demod driver");
MODULE_LICENSE("MSTAR");
