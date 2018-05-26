
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>

#include "hma_alloc.h"
#include "fwload.h"
#include "fwload_drv.h"

#define debug(fmt,args...)	do{ if (debug_level > 0) printk(KERN_DEBUG"%16s.%d: "fmt,__func__,__LINE__,##args); }while(0)
#define info(fmt,args...)	printk(KERN_INFO"%16s.%d: "fmt,__func__,__LINE__,##args)
#define error(fmt,args...)	printk(KERN_ERR"%16s.%d: "fmt,__func__,__LINE__,##args)

static int debug_level = 1;
module_param (debug_level, int, 0640);

static int ram_size = 16*1024*1024;
module_param (ram_size, int, 0440);

static int major = 1212;
module_param (major, int, 0440);

static unsigned int rom_base;
module_param (rom_base, uint, 0440);


void free_image (struct image *r)
{
	iounmap (r->vaddr);
	hma_free (NULL, r->addr);
	vfree (r->backup);

	r->addr = 0;
	r->backup = NULL;
}

int copy_to_ddr (struct image *r)
{
	memcpy (r->vaddr, r->backup, r->backup_size);

	return 0;
}

static int load (struct mcu *m, struct fwload_image *image)
{
	struct image *r;
	char name[32];
	int ret;

	if (image->type == fwload_image_type_rom)
		snprintf (name, sizeof(name), "%s_rom", m->name);
	else
		snprintf (name, sizeof(name), "%s_ram", m->name);

	if (image->total_size == 0)
		image->total_size = image->size;

	if (image->size > image->total_size)
	{
		info ("bigger image size then memory, %d > %d\n",
				image->size, image->total_size);
		image->size = image->total_size;
	}

	debug ("load %s image, size %d, total %d\n",
			name, image->size, image->total_size);

	if (image->type == fwload_image_type_rom)
		r = &m->rom;
	else
		r = &m->ram;

	if (r->addr && r->size != image->total_size)
	{
		free_image (r);

		debug ("release %s memory\n", name);
	}

	if (!r->addr)
	{
		if (rom_base)
			r->addr = rom_base;
		else
		{
			r->addr = hma_alloc_user ("vdec", image->total_size, 4096, name);
			if (!r->addr)
			{
				error ("hma_alloc failed\n");
				return -ENOMEM;
			}
		}
		r->vaddr = ioremap (r->addr, image->total_size);
		if (!r->vaddr)
		{
			error ("ioremap failed\n");
			hma_free (NULL, r->addr);
			r->addr = 0;
			return -ENOMEM;
		}
		r->size = image->total_size;

		debug ("memory allocated for %s, 0x%08lx(%d), 0x%p\n",
				name, r->addr, r->size, r->vaddr);
	}

	if (image->data && image->size > 0)
	{
		if (r->backup)
		{
			vfree (r->backup);
			r->backup_size = 0;
		}

		r->backup = vmalloc (image->size);
		if (!r->backup)
		{
			error ("vmalloc failed. %d\n", image->size);
			return -ENOMEM;
		}

		ret = copy_from_user (r->backup,
				image->data, image->size);
		if (ret > 0)
		{
			error ("memcpy failed\n");
			return -EFAULT;
		}
		r->backup_size = image->size;

		copy_to_ddr (r);
	}
	else
		debug ("no loading image for %s\n", name);

	return 0;
}


extern struct mcu *init_vdec (void);
extern struct mcu *init_cip (void);
extern void cleanup_vdec (void);
extern void cleanup_cip (void);

static struct mcu_control
{
	struct mcu *(*init)(void);
	void (*cleanup)(void);

	fwload_callback_t cb;

	struct mcu *mcu;
	struct proc_dir_entry *proc;
} mcus[] =
{
	{ init_vdec, cleanup_vdec, },
	{ init_cip,  cleanup_cip, },
};


static int halt (struct mcu *m, int hold)
{
	info ("%s halt for %s\n",
			hold?"hold":"release",
			m->name);

	if (m->control->cb)
	{
		info ("event callback for halt %s, %s\n",
				hold?"hold":"release",
				m->name);
		m->control->cb (m->name,
				hold?fwload_event_halt_hold:fwload_event_halt_release);
	}

	if (m->halt)
		m->halt (hold);
	else
		error ("no halt operation for %s\n", m->name);

	return 0;
}


static int reset (struct mcu *m, int hold)
{
	info ("%s reset for %s\n",
			hold?"hold":"release",
			m->name);

	if (m->control->cb && hold)
	{
		info ("event callback for reset, %s\n",
				m->name);
		m->control->cb (m->name,
				fwload_event_reset);
	}

	if (m->reset)
		m->reset (hold);
	else
		error ("no reset operation for %s\n", m->name);

	return 0;
}


int fwload_register_callback (const char *name, fwload_callback_t cb)
{
	int a;

	for (a=0; a<ARRAY_SIZE (mcus); a++)
		if (mcus[a].mcu && !strcmp (mcus[a].mcu->name, name))
			break;
	if (a == ARRAY_SIZE (mcus))
	{
		error ("no such mcu, %s\n", name);
		return -ENODEV;
	}

	if (mcus[a].cb)
	{
		error ("callback already registered for %s\n", name);
		return -EBUSY;
	}

	mcus[a].cb = cb;

	return 0;
}
EXPORT_SYMBOL (fwload_register_callback);


int fwload_reset (const char *name)
{
	struct mcu_control *mc;
	struct mcu *m;
	int a;

	info ("reset %s...\n", name);
	for (a=0; a<ARRAY_SIZE (mcus); a++)
		if (mcus[a].mcu && !strcmp (mcus[a].mcu->name, name))
			break;
	if (a == ARRAY_SIZE (mcus))
	{
		error ("no such mcu, %s\n", name);
		return -ENODEV;
	}
	mc = &mcus[a];
	m = mc->mcu;

	if (!m->rom.vaddr || !m->rom.backup || !m->rom.backup_size)
	{
		error ("no firmware loaded for %s\n", name);
		return -1;
	}

	halt (m, 1);
	reset (m, 1);
	copy_to_ddr (&m->rom);
	reset (m, 0);
	halt (m, 0);
	info ("reset %s done.\n", name);

	return 0;
}
EXPORT_SYMBOL (fwload_reset);


static int open (struct inode *inode, struct file *file)
{
	int minor = iminor (inode);

	if (ARRAY_SIZE (mcus) <= minor)
	{
		error ("too big minor. %d\n", minor);
		return -EINVAL;
	}

	if (!try_module_get (mcus[minor].mcu->owner))
	{
		error ("no module for the MCU\n");
		return -EINVAL;
	}

	file->private_data = mcus[minor].mcu;

	return 0;
}

static long ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	union
	{
		struct fwload_image image;
	} a;
	int ret = 0;
	struct mcu *m = file->private_data;

	if (_IOC_TYPE(cmd) != FWLOAD_IOCTL_TYPE)
	{
		error ("invalid magic. magic=0x%02x\n",
				_IOC_TYPE(cmd) );
		return -ENOIOCTLCMD;
	}
	if (_IOC_DIR(cmd) & _IOC_WRITE)
	{
		int r;

		debug ("copy %d bytes\n", _IOC_SIZE(cmd));
		r = copy_from_user (&a, (void*)arg, _IOC_SIZE(cmd));
		if (r)
		{
			error ("copy_from_user failed\n");
			return -EFAULT;
		}
	}

	switch (cmd)
	{
	case FWLOAD_IOCTL_HALT:
		ret = halt (m, !!arg);
		break;

	case FWLOAD_IOCTL_RESET:
		ret = reset (m, !!arg);
		break;

	case FWLOAD_IOCTL_LOAD:
		ret = load (m, &a.image);
		break;

	default:
		error ("unkonwn ioctl command, %08x\n", cmd);
		return -ENOIOCTLCMD;
	}

	if (ret < 0)
		error ("ioctl error %d for cmd %08x\n", ret, cmd);

	if (ret >= 0 && _IOC_DIR(cmd) & _IOC_READ)
	{
		int r;

		r = copy_to_user ((void*)arg, &a, _IOC_SIZE(cmd));
		if (r)
		{
			error ("copy_to_user failed\n");
			return -EFAULT;
		}
	}

	return ret;
}

static ssize_t read (struct file *file, char __user *data, size_t size, loff_t *off)
{
	ssize_t reading;
	int ret;
	struct mcu *m = file->private_data;

	//debug ("off %lld\n", *off);
	if (!m->ram.addr || !m->ram.vaddr)
	{
		error ("no ram allocated\n");
		return -EFAULT;
	}

	//debug ("ram size %d\n", m->ram.size);
	if (*off >= m->ram.size)
		return 0;

	reading = m->ram.size - *off;
	if (reading > size)
		reading = size;

	//debug ("reading %d bytes from %p\n", reading, m->ram.vaddr);
	ret = copy_to_user (data, m->ram.vaddr+*off, reading);
	if (ret > 0)
	{
		error ("copy_to_user failed\n");
		return -EFAULT;
	}

	*off += reading;

	return reading;
}

static int release (struct inode *inode, struct file *file)
{
	struct mcu *m = file->private_data;

	module_put (m->owner);

	return 0;
}

static struct file_operations fops =
{
	.open = open,
	.unlocked_ioctl = ioctl,
	.read = read,
	.release = release,
};

static void reg_dump(struct seq_file *file,
		unsigned int *addr, unsigned int size)
{
	int i;

	for( i=0; i<size/4; )
	{
		if (i%4 == 0)
			seq_printf (file, "%08x:", i*4);
		seq_printf (file, " %08x",addr[i]);
		i++;
		if (i%4 == 0)
			seq_printf(file, "\n");
	}
	if (i%4 != 0)
		seq_printf(file, "\n");
}

static int cr_show (struct seq_file *m, void *data)
{
	struct mcu *mcu = m->private;

	reg_dump (m, (void*)mcu->control_register,
			mcu->control_register_size);

	return 0;
}

static int cr_open (struct inode *inode, struct file *file)
{
	int minor;
	int ret;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	minor = (int)dp->data;

#else
	minor = (int)PDE_DATA(inode);
#endif

	if (ARRAY_SIZE (mcus) <= minor)
	{
		error ("too big minor. %d\n", minor);
		return -EINVAL;
	}

	ret = single_open (file, cr_show, mcus[minor].mcu);
	if (ret < 0)
		return ret;

	return ret;
}

static struct file_operations control_registers_fops =
{
	.open = cr_open,
	.release = single_release,
	.read = seq_read,
	.llseek = seq_lseek,
};

static int uart_open (struct inode *inode, struct file *file)
{
	int minor;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	minor = (int)dp->data;

#else
	minor = (int)PDE_DATA(inode);
#endif

	if (ARRAY_SIZE (mcus) <= minor)
	{
		error ("too big minor. %d\n", minor);
		return -EINVAL;
	}

	file->private_data = mcus[minor].mcu;

	return 0;
}

static ssize_t uart_jtag_write (struct file *file,
		const char __user *data, size_t size, loff_t *off, int uart)
{
	char buf[16];
	int ret;
	int len;
	int port;
	struct mcu *m = file->private_data;
	long res;

	len = size>=sizeof(buf)?(sizeof(buf)-1):size;
	ret = copy_from_user (buf, data, len);
	buf[len] = 0;
	if (ret > 0)
		return -EFAULT;

	port = kstrtoul (buf, 0, &res);
	info ("mcu %s switch to %s %d\n",
			m->name, uart?"uart":"jtag", port);
	if (uart)
		m->uart (port);
	else
		m->jtag (port);

	return size;
}

static ssize_t uart_write (struct file *file,
		const char __user *data, size_t size, loff_t *off)
{
	return uart_jtag_write (file, data, size, off, 1);
}

static ssize_t jtag_write (struct file *file,
		const char __user *data, size_t size, loff_t *off)
{
	return uart_jtag_write (file, data, size, off, 0);
}

static struct file_operations uart_fops =
{
	.open = uart_open,
	.write = uart_write,
};

static struct file_operations jtag_fops =
{
	.open = uart_open,
	.write = jtag_write,
};

static struct proc_dir_entry *proc_root;


/* suspend to ram */
static int suspend (struct platform_device *dev)
{
	int a;

	printk ("fwload: suspend...\n");
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *mcu = mcus[a].mcu;

		if (mcu->suspend)
		{
			int ret;

			printk ("fwload: suspend %s...\n", mcu->name);
			ret = mcu->suspend ();
			if (ret < 0)
			{
				printk (KERN_ERR"fwload: suspend failed for %s\n",
						mcu->name);
				return ret;
			}
		}
	}

	printk ("fwload: suspend done.\n");

	return 0;
}


static int resume (struct platform_device *dev)
{
	int a;

	printk ("fwload: resume...\n");
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *mcu = mcus[a].mcu;

		if (mcu->resume)
		{
			int ret;

			printk ("fwload: resume %s...\n", mcu->name);
			ret = mcu->resume ();
			if (ret < 0)
			{
				printk (KERN_ERR"fwload: resume failed for %s\n",
						mcu->name);
				return ret;
			}
		}
	}

	printk ("fwload: resume done.\n");

	return 0;
}


/* hivernation */
static int freeze (struct platform_device *dev)
{
	int a;

	printk ("fwload: freeze...\n");
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *mcu = mcus[a].mcu;

		if (mcu->freeze)
		{
			int ret;

			printk ("fwload: freeze %s...\n", mcu->name);
			ret = mcu->freeze ();
			if (ret < 0)
			{
				printk (KERN_ERR"fwload: freeze failed for %s\n",
						mcu->name);
				return ret;
			}
		}
	}

	printk ("fwload: freeze done.\n");

	return 0;
}


static int thaw (struct platform_device *dev)
{
	int a;

	printk ("fwload: thaw...\n");
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *mcu = mcus[a].mcu;

		if (mcu->thaw)
		{
			int ret;

			printk ("fwload: thaw %s...\n", mcu->name);
			ret = mcu->thaw ();
			if (ret < 0)
			{
				printk (KERN_ERR"fwload: thaw failed for %s\n",
						mcu->name);
				return ret;
			}
		}
	}

	printk ("fwload: thaw done.\n");

	return 0;
}


static int restore (struct platform_device *dev)
{
	int a;

	printk ("fwload: restore...\n");
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *mcu = mcus[a].mcu;

		if (mcu->restore)
		{
			int ret;

			printk ("fwload: restore %s...\n", mcu->name);
			ret = mcu->restore ();
			if (ret < 0)
			{
				printk (KERN_ERR"fwload: restore failed for %s\n",
						mcu->name);
				return ret;
			}
		}
	}

	printk ("fwload: restore done.\n");

	return 0;
}


static int fwload_prepare		(struct device *dev){printk("%s\n", __func__);return 0;}
static void fwload_complete		(struct device *dev){printk("%s\n", __func__);return;}
static int fwload_suspend		(struct device *dev){printk("%s\n", __func__);return suspend (to_platform_device (dev));}
static int fwload_resume		(struct device *dev){printk("%s\n", __func__);return resume (to_platform_device (dev));}
static int fwload_freeze		(struct device *dev){printk("%s\n", __func__);return freeze (to_platform_device (dev));}
static int fwload_thaw			(struct device *dev){printk("%s\n", __func__);return thaw (to_platform_device (dev));}
static int fwload_poweroff		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_restore		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_suspend_late		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_resume_early		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_freeze_late		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_thaw_early		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_poweroff_late		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_restore_early		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_suspend_noirq		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_resume_noirq		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_freeze_noirq		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_thaw_noirq		(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_poweroff_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_restore_noirq		(struct device *dev){printk("%s\n", __func__);return restore (to_platform_device (dev));}
static int fwload_runtime_suspend	(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_runtime_resume	(struct device *dev){printk("%s\n", __func__);return 0;}
static int fwload_runtime_idle		(struct device *dev){printk("%s\n", __func__);return 0;}

static struct dev_pm_ops pm_ops =
{
	.prepare	= fwload_prepare,
	.complete	= fwload_complete,
	.suspend	= fwload_suspend,
	.resume		= fwload_resume,
	.freeze		= fwload_freeze,
	.thaw		= fwload_thaw,
	.poweroff	= fwload_poweroff,
	.restore	= fwload_restore,
	.suspend_late	= fwload_suspend_late,
	.resume_early	= fwload_resume_early,
	.freeze_late	= fwload_freeze_late,
	.thaw_early	= fwload_thaw_early,
	.poweroff_late	= fwload_poweroff_late,
	.restore_early	= fwload_restore_early,
	.suspend_noirq	= fwload_suspend_noirq,
	.resume_noirq	= fwload_resume_noirq,
	.freeze_noirq	= fwload_freeze_noirq,
	.thaw_noirq	= fwload_thaw_noirq,
	.poweroff_noirq	= fwload_poweroff_noirq,
	.restore_noirq	= fwload_restore_noirq,
	.runtime_suspend= fwload_runtime_suspend,
	.runtime_resume	= fwload_runtime_resume,
	.runtime_idle	= fwload_runtime_idle,
};

static struct platform_driver platform_driver =
{
	//.probe = probe,
	//.remove = remove,
	//.suspend = suspend,
	//.resume = resume,
	.driver =
	{
		.name = "fwload",
		.pm = &pm_ops,
	},
};

static struct platform_device platform_device =
{
	.name = "fwload",
	.id = -1,
};

static int fwload_init (void)
{
	int ret;
	int a;

	info ("init.\n");

	proc_root = proc_mkdir ("mcu", NULL);

	/* initialize each MCU */
	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		struct mcu *m;

		m = mcus[a].init ();
		if (!m)
		{
			info ("mcu initialize failed. %d\n", a);
			break;
		}
		mcus[a].mcu = m;
		m->control = &mcus[a];

		if (proc_root)
		{
			mcus[a].proc = proc_mkdir (m->name, proc_root);
			if (mcus[a].proc)
			{
				if (m->control_register && m->control_register_size > 0)
				{
					proc_create_data ("control_registers", 0440,
							mcus[a].proc,
							&control_registers_fops,
							(void*)a);
				}

				if (m->uart)
				{
					proc_create_data ("uart", 0220,
							mcus[a].proc,
							&uart_fops,
							(void*)a);
				}

				if (m->jtag)
				{
					proc_create_data ("jtag", 0220,
							mcus[a].proc,
							&jtag_fops,
							(void*)a);
				}
			}
		}

		info ("%s initialized.\n", m->name);
	}
	if (a != ARRAY_SIZE (mcus))
	{
		for (a--; a>=0; a--)
		{
			info ("cleanup mcu, %s\n", mcus[a].mcu->name);
			mcus[a].cleanup();
		}

		return -EINVAL;
	}


	/* register char. dev. */
	ret = register_chrdev (major, "fwload", &fops);
	if (ret < 0)
	{
		error ("register_chrdev failed\n");
		return ret;
	}
	if (major == 0)
		major = ret;
	debug ("major %d\n", major);

	{
		struct class* class = NULL;

		class = class_create(THIS_MODULE, "fwload" );
		device_create (class, NULL, MKDEV(major, 0),
				NULL, "fwload");
	}

	ret = platform_device_register (&platform_device);
	if (ret)
	{
		error ("platform_device_register failed, %d\n", ret);
		return ret;
	}

	ret = platform_driver_register (&platform_driver);
	if (ret)
	{
		error ("platform_driver_register failed, %d\n", ret);
		return ret;
	}


	return 0;
}

static void fwload_exit (void)
{
	int a;

	info ("exit.\n");

	for (a=0; a<ARRAY_SIZE(mcus); a++)
	{
		info ("cleanup %s\n", mcus[a].mcu->name);
		if (mcus[a].proc)
		{
			struct mcu *m = mcus[a].mcu;

			if (m->control_register && m->control_register_size > 0)
				remove_proc_entry ("control_registers", mcus[a].proc);

			if (m->uart)
				remove_proc_entry ("uart", mcus[a].proc);

			remove_proc_entry (mcus[a].mcu->name, proc_root);
		}
		mcus[a].cleanup ();
	}

	remove_proc_entry ("mcu", NULL);

	unregister_chrdev (major, "fwload");
}

MODULE_LICENSE ("GPL");
module_init (fwload_init);
module_exit (fwload_exit);

