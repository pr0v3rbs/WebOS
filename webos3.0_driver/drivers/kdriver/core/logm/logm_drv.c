/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  main driver implementation for logm device.
 *	logm device will teach you how to make device driver with new platform.
 *
 *  author		hwang.hayun (hwang.hayun@lge.com)
 *  version		1.0
 *  date		2013.10.29
 *  note		Additional information.
 *
 *  @addtogroup lg115x_logm
 *	@{
 */

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/file.h>
#include <linux/ctype.h>

#include "os_util.h"
#include "base_device.h"
#include "logm_drv.h"
#include "logm_cfg.h"

/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#define LOGM_MAJOR 	1810
#define LOGM_MINOR 	0

/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/

/**
 *	main control block for logm device.
 *	each minor device has unique control block
 *
 */
typedef struct
{
// BEGIN of common device
	int						dev_open_count;		///< check if device is opened or not
	dev_t					devno;			///< device number
	struct cdev				cdev;			///< char device structure
// END of command device

	// BEGIN of device specific data


	// END of device specific data
}
LOGM_DEVICE_T;

typedef struct
{
	int 			open_flags;
	int 			dummy;
}LOGM_UDATA_T;

/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/
extern	void	LOGM_PROC_Init(void);
extern	void	LOGM_PROC_Cleanup(void);

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Functions
  ----------------------------------------------------------------------------------------*/
int		LOGM_Init(void);
void	LOGM_Cleanup(void);

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
int		g_logm_debug_fd;
int 	g_logm_major = LOGM_MAJOR;
int 	g_logm_minor = LOGM_MINOR;

//logm buffer
struct  logm_buffer_ctrl g_logm_buffer_ctrl[LOGM_BUFFER_NUMBER];
LX_LOGM_LOGOBJ_T *g_logm_obj_map;
DEFINE_SPINLOCK(g_logm_obj_map_lock);

int g_logm_debug_level = 2;
module_param_named(debug, g_logm_debug_level, int ,0644);

int g_logm_global_ctrl_buffer = 1;
module_param_named(global, g_logm_global_ctrl_buffer, int, 0644);

unsigned int g_logm_path_ctrl;
unsigned int g_logm_short_format;
unsigned int g_logm_log_color;
/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/
static int      LOGM_Open(struct inode *, struct file *);
static int      LOGM_Close(struct inode *, struct file *);
static long		LOGM_Ioctl(struct file *, UINT32, ULONG );
static ssize_t  LOGM_Read(struct file *, char *, size_t, loff_t *);
//static ssize_t  LOGM_Write(struct file *, const char *, size_t, loff_t *);
static unsigned int LOGM_Poll(struct file *file,
		struct poll_table_struct *pt);
static int LOGM_Mmap (struct file *file, struct vm_area_struct *vm);
static int LOGM_ResourceInit(void);
static void LOGM_ResourceFinal(void);

static int _logm_ioctl_mask_ctrl(unsigned long arg,
									 unsigned int cmd);

static int _logm_ioctl_objmap_size(unsigned long arg,
									unsigned int cmd);

static int _logm_ioctl_obj_reg(unsigned long arg,
								unsigned int cmd);

static int _logm_ioctl_path_ctrl(unsigned long arg);

static int _logm_ioctl_user_write(unsigned long arg,
								unsigned int cmd);
static int _logm_mask_restore(void);
/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/**
 * main control block for logm device
 */
static LOGM_DEVICE_T*		g_logm_device;

/**
 * file I/O description for logm device
 *
 */
static struct file_operations g_logm_fops =
{
	.open 	= LOGM_Open,
	.release= LOGM_Close,
	.unlocked_ioctl	= LOGM_Ioctl,
	.read 	= LOGM_Read,
	//.write 	= LOGM_Write,
	.poll   = LOGM_Poll,
	.mmap 	= LOGM_Mmap,
};

/*========================================================================================
  Implementation Group
  ========================================================================================*/
#define debug(fmt, arg...)	do{if(g_logm_debug_level&0x01) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define trace(fmt, arg...)	do{if(g_logm_debug_level&0x02) printk (KERN_DEBUG "%s.%d: " fmt, __func__, __LINE__, ##arg);}while(0)
#define error(fmt, arg...)	printk (KERN_ERR "[ERROR]%s.%d: " fmt, __func__, __LINE__, ##arg)

#define LOGM_CHECK_ERROR(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, error, __if_action , fmt, ##args )

static int _logm_maskname2bit(char *mask)
{
	int i;

	struct
	{
		const char *name;
		int len;
		LX_LOGM_LOGLEVEL_T level;
	} level_table[] =
	{
#define lt(s,n)	{#s, sizeof(#s), LX_LOGM_LEVEL_##n, }
		lt(ERROR, ERROR),
		lt(WARN, WARNING),
		lt(NOTI, NOTI),
		lt(INFO, INFO),
		lt(DEBUG, DEBUG),
		lt(TRACE, TRACE),

		/* simple form */
		lt(E, ERROR),
		lt(W, WARNING),
		lt(N, NOTI),
		lt(I, INFO),
		lt(D, DEBUG),
		lt(T, TRACE),
#undef lt
	};

	//mask string to upper
	for(i = 0; i < strlen(mask); i++)
		mask[i] = toupper(mask[i]);

	if (*mask == 'B')
	{
		ulong bit;

		mask++;

		bit = simple_strtoul(mask, NULL, 10);

		if(bit <= 31)
			return bit;
		else
		{   
			error ("unknown format.\n");
			return -1;
		}
	}
	else
	{
		int a;

		for (a=0; a<ARRAY_SIZE (level_table); a++)
		{
			int len;

			debug ("name %s, len %d\n",
					level_table[a].name,
					level_table[a].len);

			len = level_table[a].len-1;

			if (!strncmp (mask,level_table[a].name, len))
			{
				debug ("got it\n");
				return level_table[a].level;
				//break;
			}
		}

		if (a >= ARRAY_SIZE(level_table))
		{
			error ("unknown format. %02x, %c\n", *mask, *mask);
			return -EINVAL;
		}
	}

	return RET_ERROR;
}

static int _logm_cmdline_parse(char *input)
{
	int ret = RET_ERROR;
	int fd;
	int input_length;
	int i;
	int sep;

	char *p;
	char *name;
	char *mask;

	u32 en_bit_mask;
	u32 dis_bit_mask;

	input_length = strlen(input);

	debug("input str len : %d\n", input_length);

	p = input;
	i = 0;

	while(i < input_length)
	{
		name = p;
		for(; i < input_length; i++, p++)
		{
			if(*p == ':')
			{
				*p = 0;
				p++;
				i++;
				debug("name : %s\n", name);
				break;
			}
			else if(*p == '\0')
				goto func_exit;
		}

		sep = 0;
		mask = 0;
		en_bit_mask = 0;//+
		dis_bit_mask = 0;//-
		for(;i < (input_length + 1); i++,p++)
		{
			if(*p == '+' && !sep)
			{
				mask = (p + 1);
				sep = 1;
			}
			else if(*p == '-' && !sep)
			{
				mask = (p + 1);
				sep = 2;
			}
			else if(sep && (*p == ',' || *p == '\0' || *p == '/'))
			{
				int bit = 0;

				if(*p == '/' || *p == '\0')
				{
					*p = 0;
					i++;
					p++;
					debug("mask : %s\n", mask);
					bit = _logm_maskname2bit(mask);
					if(bit >= 0 && bit < 32)
					{
						if(sep == 1)
							en_bit_mask |= (1 << bit);
						else
							dis_bit_mask |= (1 << bit);
					}
					mask = 0;
					sep = 0;
					break;
				}
				else
				{
					*p = 0;
					debug("mask : %s\n", mask);
					bit = _logm_maskname2bit(mask);
					if(bit >= 0 && bit < 32)
					{
						if(sep == 1)
							en_bit_mask |= (1 << bit);
						else
							dis_bit_mask |= (1 << bit);
					}
					mask = 0;
					sep = 0;
				}

			}
		}

		trace("name %s, en_mask %x, dis_mask %x\n", name, en_bit_mask, dis_bit_mask);

		fd = LOGM_ObjRegister(name);
		if(fd >= 0)
		{
			LOGM_ObjMaskSet(fd, en_bit_mask);
			LOGM_ObjUserMaskSet(fd, dis_bit_mask);
		}
	}

	ret = RET_OK;

	debug("end\n");

func_exit:
	return ret;
}

static int _logm_mask_restore(void)
{
	int ret;
	struct file *filp;
	mm_segment_t old_fs = get_fs();
	char *param;

	set_fs(KERNEL_DS);
	filp = filp_open("/mnt/lg/cmn_data/logm_mask", O_RDONLY, 0644);
	if(IS_ERR(filp))
		;
	else
	{
		int i;
		int rd_size = sizeof(LX_LOGM_LOGOBJ_T) * LOGM_LOGOBJ_MAX;

		ret = vfs_read(filp, (char *)g_logm_obj_map, rd_size, &filp->f_pos);
		if(ret != rd_size)
		{
			memset(g_logm_obj_map, 0, rd_size);
			error("fail read logm_mask file, clear logm obj table\n");
		}
		else
		{
			//clear ref_cnt, to create proc file
			for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
			{
				g_logm_obj_map[i].ref_cnt = 0;
				g_logm_obj_map[i].has_proc = OFF;
			}
		}
		filp_close(filp, NULL);
		trace("restore logm mask\n");
	}
	set_fs(old_fs);

	/*parse logm parameters from kernel cmdline */

	param = (char *)kmalloc(1024, GFP_KERNEL);
	if(!param)
	{
		error("fail kmalloc\n");
		return RET_ERROR;
	}

	if(OS_ScanKernelCmdline("logm=%s", param) == RET_OK)
	{
		debug("logm init param : %s\n", param);
		_logm_cmdline_parse(param);
	}

	kfree(param);

	return 0;
}
/**
 * Initialize resources of logm
 */
static int LOGM_ResourceInit(void)
{
	UINT32 i;
	UINT32 size;
	UINT32 order;
	char *b;

	//initialize logm buffer control variables
	memset(g_logm_buffer_ctrl, 0
			, sizeof(struct logm_buffer_ctrl) * LOGM_BUFFER_NUMBER);
	for(i = 0; i < LOGM_BUFFER_NUMBER; i++)
	{
		spin_lock_init(&(g_logm_buffer_ctrl[i].buffer_lock));
		init_waitqueue_head(&(g_logm_buffer_ctrl[i].buffer_wait));
		init_waitqueue_head(&(g_logm_buffer_ctrl[i].buffer_lmdump_wait));
		
		order = get_order(LOGM_BUFFER_SIZE);
		size = (1 << order) * PAGE_SIZE;
		LOGM_CHECK_ERROR(size != LOGM_BUFFER_SIZE, return RET_ERROR
				,"wrong logm buffer size : %u\n", size);
		trace("logm buffer pages : %u size : %lu\n"
				, 1 << order, (1 << order) * PAGE_SIZE);

		b = (char *)__get_free_pages(GFP_KERNEL,order);
		LOGM_CHECK_ERROR(b == NULL, return RET_ERROR
						, "fail get_free_pages\n");

		g_logm_buffer_ctrl[i].logm_buf = b;
	}

	//initialize logm module object table
	size = sizeof(LX_LOGM_LOGOBJ_T) * LOGM_LOGOBJ_MAX;
	order = get_order(size);
	g_logm_obj_map = 
			(LX_LOGM_LOGOBJ_T *)__get_free_pages(GFP_KERNEL, order);
	trace("logm obj map pages: %u size : %lu\n"
			,1 << order, (1 << order) * PAGE_SIZE);
	memset(g_logm_obj_map, 0, size);

	//initialize path ctrl
	g_logm_path_ctrl = 1; //force printk
	g_logm_log_color = 0; //non color log
	g_logm_short_format = 0;//off short format

	return RET_OK;
}

/**
 * Finalize resources of logm
 */
static void LOGM_ResourceFinal(void)
{
	UINT32 i;
	//TODO: Finalize Resource

	for(i = 0; i < LOGM_BUFFER_NUMBER; i++)
		free_pages((unsigned long)g_logm_buffer_ctrl[i].logm_buf
				,get_order(LOGM_BUFFER_SIZE));


	memset(g_logm_buffer_ctrl, 0
		, sizeof(struct logm_buffer_ctrl) * LOGM_BUFFER_NUMBER);

	free_pages((unsigned long)g_logm_obj_map
			, get_order(sizeof(LX_LOGM_LOGOBJ_T) * LOGM_LOGOBJ_MAX));
}

/** Initialize the device environment before the real H/W initialization
 *
 *  @note main usage of this function is to initialize the HAL layer and memory size adjustment
 *  @note it's natural to keep this function blank :)
 */
void LOGM_PreInit(void)
{
	/* TODO: do something */
}

//TODO: Call first LOGM_Init in base,because all module_init functions use functions of logm
//refence BASE_Init()
int LOGM_Init(void)
{
	int			i;
	int			err;
	dev_t		dev;

	/* allocate main device handler, register current device.
	 *
	 * If devie major is predefined then register device using that number.
	 * otherwise, major number of device is automatically assigned by Linux kernel.
	 *
	 */
	g_logm_device = (LOGM_DEVICE_T*)kmalloc( sizeof(LOGM_DEVICE_T)*LOGM_MAX_DEVICE , GFP_KERNEL);

	if ( NULL == g_logm_device )
	{
		error("out of memory. can't allocate %d bytes\n", sizeof(LOGM_DEVICE_T)* LOGM_MAX_DEVICE );
		return -ENOMEM;
	}

	memset( g_logm_device, 0x0, sizeof(LOGM_DEVICE_T)* LOGM_MAX_DEVICE );

	if (g_logm_major)
	{
		dev = MKDEV( g_logm_major, g_logm_minor );
		err = register_chrdev_region(dev, LOGM_MAX_DEVICE, LOGM_MODULE );
	}
	else
	{
		err = alloc_chrdev_region(&dev, g_logm_minor, LOGM_MAX_DEVICE, LOGM_MODULE );
		g_logm_major = MAJOR(dev);
	}

	if ( err < 0 )
	{
		error("can't register logm device\n" );
		return -EIO;
	}

	/* TODO : initialize your module not specific minor device */
	err = LOGM_ResourceInit();
	LOGM_CHECK_ERROR(err != RET_OK, return -EIO, "fail logm init\n");

	/* initialize proc system */
	LOGM_PROC_Init ( );
	/* END */

	for ( i=0; i<LOGM_MAX_DEVICE; i++ )
	{
		/* initialize cdev structure with predefined variable */
		dev = MKDEV( g_logm_major, g_logm_minor+i );
		cdev_init( &(g_logm_device[i].cdev), &g_logm_fops );
		g_logm_device[i].devno		= dev;
		g_logm_device[i].cdev.owner = THIS_MODULE;
		g_logm_device[i].cdev.ops   = &g_logm_fops;

		/* TODO: initialize minor device */


		/* END */

		err = cdev_add (&(g_logm_device[i].cdev), dev, 1 );

		if (err)
		{
			error("error (%d) while adding logm device (%d.%d)\n", err, MAJOR(dev), MINOR(dev) );
			return -EIO;
		}
#ifdef __ANDROID__
		OS_CreateDeviceClass ( g_logm_device[i].devno, "%s", LOGM_MODULE);
#else
		OS_CreateDeviceClass ( g_logm_device[i].devno, "%s%d", LOGM_MODULE, i );
#endif
	}

	//if exist logm_mask file, restore obj map table, just use to debug
	_logm_mask_restore();

	trace("logm device initialized\n");

	return 0;
}

void LOGM_Cleanup(void)
{
	int i;
	dev_t dev = MKDEV( g_logm_major, g_logm_minor );

	/* cleanup proc system */
	LOGM_PROC_Cleanup( );

	/* remove all minor devicies and unregister current device */
	for ( i=0; i<LOGM_MAX_DEVICE;i++)
	{
		/* TODO: cleanup each minor device */


		/* END */
		cdev_del( &(g_logm_device[i].cdev) );
	}

	/* TODO : cleanup your module not specific minor device */
	LOGM_ResourceFinal();

	unregister_chrdev_region(dev, LOGM_MAX_DEVICE );

	OS_Free( g_logm_device );
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * open handler for logm device
 *
 */
	static int
LOGM_Open(struct inode *inode, struct file *filp)
{
	int					major,minor;
	struct cdev 		*cdev;
	LOGM_DEVICE_T 		*my_dev;
	LOGM_UDATA_T 		*udata;

	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, LOGM_DEVICE_T, cdev);

	/* TODO : add your device specific code */
	udata = (LOGM_UDATA_T *)kmalloc(sizeof(LOGM_UDATA_T), GFP_KERNEL);
	LOGM_CHECK_ERROR(udata == NULL, return RET_ERROR
						, "fail kmalloc\n");

	udata->open_flags = filp->f_flags;
	filp->private_data = udata;
	/* END */

	my_dev->dev_open_count++;

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);
	trace("device opened (%d:%d)\n", major, minor );

	return 0;
}

/**
 * release handler for logm device
 *
 */
	static int
LOGM_Close(struct inode *inode, struct file *file)
{
	int					major,minor;
	LOGM_DEVICE_T 		*my_dev;
	struct cdev 		*cdev;
	LOGM_UDATA_T 		*udata = file->private_data;

	cdev	= inode->i_cdev;
	my_dev	= container_of ( cdev, LOGM_DEVICE_T, cdev);

	if ( my_dev->dev_open_count > 0 )
	{
		--my_dev->dev_open_count;
	}

	/* TODO : add your device specific code */
	if ( my_dev->dev_open_count <= 0 )
	{

	}

	if(udata->open_flags & O_TRUNC)
	{
		g_logm_path_ctrl = 1;

		trace("closed daemon, do force printk\n");
	}

	kfree(udata);

	/* END */

	/* some debug */
	major = imajor(inode);
	minor = iminor(inode);
	trace("device closed (%d:%d)\n", major, minor );
	return 0;
}

/**
 * ioctl handler for logm device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
	static long
LOGM_Ioctl (struct file* filp, UINT32 cmd, ULONG arg)
{
	int err = 0, ret = 0;

	/*
	 * get current logm device object
	 */
	//xxx = filp->private_data;

	/*
	 * check if IOCTL command is valid or not.
	 * - if magic value doesn't match, return error (-ENOTTY)
	 * - if command is out of range, return error (-ENOTTY)
	 *
	 * note) -ENOTTY means "Inappropriate ioctl for device.
	 */
	if (_IOC_TYPE(cmd) != LOGM_IOC_MAGIC)
	{
		error("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > LOGM_IOC_MAXNR)
	{
		error("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
		return -ENOTTY;
	}

	/* TODO : add some check routine for your device */

	/*
	 * check if user memory is valid or not.
	 * if memory can't be accessed from kernel, return error (-EFAULT)
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err)
	{
		error("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
				_IOC_NR(cmd),
				(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
				(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
				(void*)arg );
		return -EFAULT;
	}

	debug("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case LOGM_IOW_MASK_CTRL:
			ret = _logm_ioctl_mask_ctrl(arg, cmd);
			break;

		case LOGM_IOR_OBJMAP_SIZE:
			ret = _logm_ioctl_objmap_size(arg, cmd);
			break;

		case LOGM_IOWR_REG_OBJ:
			ret = _logm_ioctl_obj_reg(arg, cmd);
			break;

		case LOGM_IOW_PATH_CTRL:
			ret = _logm_ioctl_path_ctrl(arg);
			break;

		case LOGM_IOW_USER_WRITE:
			ret = _logm_ioctl_user_write(arg, cmd);
			break;

		default:
			{
				/* redundant check but it seems more readable */
				ret = -ENOTTY;
			}
	}

	return ret;
}

static int _logm_ioctl_user_write(unsigned long arg,
									 unsigned int cmd)
{
	int ret;
	struct logm_user_log u;
    LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;
	char *data;

	//use __copy, because "access_ok" check a precondition
	ret = __copy_from_user(&u, (void*)arg
			, _IOC_SIZE(cmd));
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_from\n");

	LOGM_CHECK_ERROR((u.fd < 0 || u.fd >= LOGM_LOGOBJ_MAX)
			, return RET_ERROR, "invalid fd = %d\n", u.fd);

	data = (char *)kmalloc(u.size, GFP_KERNEL);
	LOGM_CHECK_ERROR(data == NULL, return RET_ERROR,
			"fail kmalloc\n");

	ret = copy_from_user(data, u.data, u.size);
	LOGM_CHECK_ERROR(ret, goto func_exit, "fail copy_from\n");

	if(data[u.size-1] == '\n')
		u.size--;

	LOGM_PutLine(&(map[u.fd]), u.level, u.type, NULL, 0
						, data, u.size);

func_exit:
	kfree(data);
	return ret;
}

static int _logm_bit_name_wr(struct logm_mask_ctrl *m)
{
	int ret;
	char *name;

	name = (char*)vmalloc(m->name_size);
	LOGM_CHECK_ERROR(name == NULL, return RET_ERROR,
			"fail vmalloc\n");

	//m->name : user space string
	ret = copy_from_user(name, m->name, m->name_size);
	LOGM_CHECK_ERROR(ret, vfree(name); return -EFAULT, "fail copy_from\n");

	//m->name : kernel space string
	m->name = name;
	ret = LOGM_RegBitName(m->fd, m->bit, m->name);
	vfree(m->name);
	LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
			"fail reg bit name\n");

	return 0;
}

static int _logm_ioctl_mask_ctrl(unsigned long arg,
									 unsigned int cmd)
{
	int ret;
	struct logm_mask_ctrl m;

	//use __copy, becase "access_ok" check a precondition
	ret = __copy_from_user(&m, (void*)arg
			, _IOC_SIZE(cmd));
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_from\n");

	switch(m.ctrl)
	{
		case LOGM_MASK_CTRL_WRITE:
		{
			ret = LOGM_ObjMaskSet(m.fd, m.mask);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj mask set\n");
		}
		break;

		case LOGM_MASK_CTRL_READ:
		{
			ret = LOGM_ObjMaskGet(m.fd, &(m.mask));
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj mask get\n");

			ret = __copy_to_user((void *)arg, &m
					, _IOC_SIZE(cmd));
			LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_to\n");
		}
		break;

		case LOGM_MASK_CTRL_ALL_WR:
		{
			int i;
			unsigned long flag;
			LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;

			spin_lock_irqsave(&g_logm_obj_map_lock, flag);
			for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
			{
				if(map[i].name[0] == '\0')
					break;
				else
					map[i].mask = m.mask;
			}
			spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);
		}
		break;

		case LOGM_MASK_CTRL_BIT_ENABLE:
		{
			ret = LOGM_ObjBitMaskEnable(m.fd, m.bit);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj mask bit enable\n");
		}
		break;

		case LOGM_MASK_CTRL_BIT_DISABLE:
		{
			ret = LOGM_ObjBitMaskDisable(m.fd, m.bit);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj mask bit disable\n");
		}
		break;

		case LOGM_USER_MASK_CTRL_WRITE:
		{
			ret = LOGM_ObjUserMaskSet(m.fd, m.mask);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj user mask set\n");
		}
		break;
		
		case LOGM_USER_MASK_CTRL_READ:
		{
			ret = LOGM_ObjUserMaskGet(m.fd, &(m.mask));
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
					"fail obj user mask get\n");

			ret = __copy_to_user((void *)arg, &m
					, _IOC_SIZE(cmd));
			LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_to\n");
		}
		break;

		case LOGM_USER_MASK_CTRL_ALL_WR:
		{
			int i;
			unsigned long flag;
			LX_LOGM_LOGOBJ_T *map = g_logm_obj_map;

			spin_lock_irqsave(&g_logm_obj_map_lock, flag);
			for(i = 0; i < LOGM_LOGOBJ_MAX; i++)
			{
				if(map[i].name[0] == '\0')
					break;
				else
					map[i].user_mask = m.mask;
			}
			spin_unlock_irqrestore(&g_logm_obj_map_lock, flag);
		}
		break;

		case LOGM_USER_MASK_CTRL_BIT_ENABLE:
		{
			ret = LOGM_ObjBitUserMaskEnable(m.fd, m.bit);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj user mask bit enable\n");
		}
		break;

		case LOGM_USER_MASK_CTRL_BIT_DISABLE:
		{
			ret = LOGM_ObjBitUserMaskDisable(m.fd, m.bit);
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail obj user mask bit disable\n");
		}
		break;

		case LOGM_MASK_CTRL_BIT_NAME_WR:
		{
			ret = _logm_bit_name_wr(&m);		
			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail wr bit name\n");
		}
		break;

		case LOGM_MASK_CTRL_BIT_NAME_RD:
		{
			struct logm_obj_bit_name *bit_name;

			bit_name = (struct logm_obj_bit_name *)vmalloc(sizeof(struct logm_obj_bit_name));
			LOGM_CHECK_ERROR(bit_name == NULL, return RET_ERROR,
					"fail vmalloc\n");

			ret = LOGM_GetBitName(m.fd, bit_name);
			LOGM_CHECK_ERROR(ret != 0, vfree(bit_name); return RET_ERROR,
				"fail rd bit name\n");

			ret = copy_to_user(((struct logm_mask_ctrl *)arg)->bit_name
										, bit_name, sizeof(struct logm_obj_bit_name));
			vfree(bit_name);

			LOGM_CHECK_ERROR(ret != 0, return RET_ERROR,
				"fail copy to x\n");

		}
		break;

		default:
		{
			error("invalid cmd\n");
			return -EFAULT;
		}
	}

	return RET_OK;
}

static int _logm_ioctl_objmap_size(unsigned long arg,
									unsigned int cmd)
{
	int ret;
	unsigned int size = (1 << get_order(sizeof(LX_LOGM_LOGOBJ_T) 
								* LOGM_LOGOBJ_MAX)) * PAGE_SIZE;

	//trace("logm mmap size %u\n", size);
	ret = put_user(size, (unsigned int *)arg);
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail put_user\n");

	return RET_OK;
}

static int _logm_ioctl_obj_reg(unsigned long arg,
								unsigned int cmd)
{
	int ret;
	struct logm_obj_reg obj_reg;

	ret = __copy_from_user(&obj_reg, (void *)arg
			, _IOC_SIZE(cmd));
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_from\n");

	if(obj_reg.ctrl == LOGM_OBJ_REGISTER)
	{
		obj_reg.fd = LOGM_ObjRegister(obj_reg.name);
		LOGM_CHECK_ERROR(obj_reg.fd < 0, return RET_ERROR,
				"fail register obj : %s\n", obj_reg.name);
	}
	else if(obj_reg.ctrl == LOGM_OBJ_DEREGISTER)
	{
		ret = LOGM_ObjDeregister(obj_reg.name);
		LOGM_CHECK_ERROR(ret != RET_OK, return RET_ERROR,
				"fail deregister obj : %s\n", obj_reg.name);
	}

	ret = __copy_to_user((void *)arg, &obj_reg
			, _IOC_SIZE(cmd));
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail __copy_to\n");

	return RET_OK;
}

static int _logm_ioctl_path_ctrl(unsigned long arg)
{
	int ret;
	unsigned int ctrl;

	ret = get_user(ctrl, (unsigned int *)arg);
	LOGM_CHECK_ERROR(ret, return -EFAULT, "fail get_user\n");

	if(ctrl == LOGM_PATH_CTRL_ON)
		g_logm_path_ctrl = 1;
	else if(ctrl == LOGM_PATH_CTRL_OFF)
		g_logm_path_ctrl = 0;
	else if(ctrl == LOGM_SHORT_FORMAT_ON)
		g_logm_short_format = 1;
	else if(ctrl == LOGM_SHORT_FORMAT_OFF)
		g_logm_short_format = 0;
	else if(ctrl == LOGM_LOG_COLOR_ON)
		g_logm_log_color = 1;
	else if(ctrl == LOGM_LOG_COLOR_OFF)
		g_logm_log_color = 0;
	else
	{
		error("invalid cmd\n");
		return RET_ERROR;
	}

	return RET_OK;
}

/**
 * poll function, noti to read logs from buffer
 * TODO: when several buffers, how to control?
 */
static unsigned int LOGM_Poll(struct file *file,
		struct poll_table_struct *pt)
{
	struct logm_buffer_ctrl *bc = &(g_logm_buffer_ctrl[0]);

	poll_wait (file, &(bc->buffer_wait), pt);

	if (bc->buffer_tail != bc->buffer_head)
		return POLLIN | POLLRDNORM;
	else
		return 0;

}

/**
 *  fops read. read logs from logm buffer
 * TODO: when several buffers, how to control?
 *
 */
static ssize_t LOGM_Read(struct file *file,
		char __user *data, size_t size, loff_t *off)
{
	char *k_buf;
	unsigned int k_buf_size = size;
	int ret, readed;
	unsigned long flag;
	struct logm_buffer_ctrl *bc = &(g_logm_buffer_ctrl[0]);

	debug ("read %zd bytes\n", size);

	if (!access_ok (VERIFY_WRITE, data, size))
	{
		error ("access failed.\n");
		return -EINVAL;
	}

	/* wait a data */
	ret = wait_event_interruptible (bc->buffer_wait,
			(bc->buffer_tail != bc->buffer_head));
	LOGM_CHECK_ERROR(ret != 0, return ret
				, "fail wait_event_int! %d\n", ret);

	debug ("got data. %d, %d\n", bc->buffer_tail, bc->buffer_head);

	if(k_buf_size > 1024)
		k_buf_size = 1024;

	k_buf = (char *)kmalloc(k_buf_size, GFP_KERNEL);
	LOGM_CHECK_ERROR(k_buf == NULL, return RET_ERROR
						, "fail kmalloc\n");

	readed = 0;
	spin_lock_irqsave (&(bc->buffer_lock), flag);
	do
	{
		ret = LOGM_ReadLog(bc, k_buf, readed, k_buf_size);
		if (ret == -ENOMEM)
			break;

		if (ret > 0)
			readed += ret;
	}
	while (bc->buffer_tail != bc->buffer_head);
	spin_unlock_irqrestore (&(bc->buffer_lock), flag);

	if(__copy_to_user(data, k_buf, readed))
	{
		error("__copy_to fail\n");
		readed = -EFAULT;
	}

	kfree(k_buf);

	debug ("done\n");

	if (readed == 0)
	{
		debug ("no memory\n");
		readed = ret;
	}

	return readed;
}

static int LOGM_Mmap (struct file *file, struct vm_area_struct *vm)
{
	int ret;
	unsigned int size = (1 << get_order(sizeof(LX_LOGM_LOGOBJ_T) * LOGM_LOGOBJ_MAX)) * PAGE_SIZE;

	//trace("logm mmap size %u\n", size);

	if((vm->vm_end - vm->vm_start) != size)
	{
		error("fial mmap size\n");
		return -EINVAL;
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	vm->vm_flags |= VM_RESERVED;//to prevent swapout

#else
	vm->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;//to prevent swapout
#endif
	ret = remap_pfn_range (vm
				, vm->vm_start
				, __virt_to_phys((long unsigned int)g_logm_obj_map)
									>> PAGE_SHIFT
				, size
				, vm->vm_page_prot);

	return ret;
}

#if 0
static ssize_t LOGM_Write(struct file *file,
		const char __user *data, size_t size, loff_t *off)
{
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",LOGM_Init);
#else
module_init(LOGM_Init);
#endif
module_exit(LOGM_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */

