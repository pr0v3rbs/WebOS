
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <asm/cacheflush.h>

#include "include/vdec_kapi.h"
#include "include/vo_kapi.h"

#include "fwload.h"

#include "hma_alloc.h"
#include "ipc.h"
#include "ipc_vdec.h"
#include "audio_kapi_renderer.h"

//#define LOG_PRINTK
#include "log.h"

#if defined(CHIP_NAME_h15)
typedef unsigned int UINT32;
#include "chip/h15/sys/ctop_ctrl_reg_h15.h"
#endif

logm_define (ipc_vdummy, log_level_warning);
logm_define (vdecd, log_level_warning);
logm_define (vod, log_level_warning);

static char *vdec_default_dec;
module_param (vdec_default_dec, charp, 0644);

static struct ipc *ipc;

static LIST_HEAD (vdec_priv_list);
static DEFINE_SPINLOCK (vdec_priv_list_lock);
struct vdec_priv
{
	struct list_head list;

	/* vdec */
	uint32_t vdec;
	LX_VDEC_BUFFER_INFO_T noti, cpb;
	int vdec_poll;
	wait_queue_head_t vdec_poll_wq;
	unsigned long cpb_vaddr;

	/* vo */
	uint32_t vo;
	int vo_poll;
	wait_queue_head_t vo_poll_wq;
};

struct vdec_priv *new_priv (void)
{
	struct vdec_priv *p;

	p = kcalloc (1, sizeof (struct vdec_priv), GFP_KERNEL);
	if (!p)
	{
		logm_warning (ipc_vdummy, "no mem for vdec priv\n");
		return NULL;
	}
	INIT_LIST_HEAD (&p->list);
	init_waitqueue_head (&p->vdec_poll_wq);
	init_waitqueue_head (&p->vo_poll_wq);
	p->vdec_poll = POLLOUT;

	return p;
}

static long vdec_ioctl (struct vdec_priv *p, unsigned int cmd, unsigned long arg)
{
	int ret, call_ret;
	int dir;
	unsigned int *data;

	logm_trace (vdecd, "vdec:%x, cmd = %08x (nr=%d)\n",
			p->vdec, cmd, _IOC_NR(cmd));

	if (_IOC_TYPE(cmd) != LX_VDEC_IOC_MAGIC)
	{
		logm_warning (vdecd, "invalid type. %x(%x)\n",
				cmd, _IOC_TYPE(cmd) );
		return -ENOIOCTLCMD;
	}


	if (cmd == LX_VDEC_PUT_ES_BUFFER)
	{
		if (p->cpb.addr && p->cpb_vaddr)
		{
			LX_VDEC_ES_DATA_T es_data;
			int ret;

			ret = copy_from_user (&es_data, (void*)arg, sizeof (es_data));
			if (ret)
			{
				logm_warning (vdecd, "vdec:%x, fault\n", p->vdec);
				return -EFAULT;
			}
			else
			{
				logm_debug (vdecd, "vdec:%x, cache to device. %lx, %x, %d\n",
						p->vdec, p->cpb_vaddr,
						es_data.buf1_offset, es_data.buf1_size);

				dmac_map_area((void*)(p->cpb_vaddr+es_data.buf1_offset),
						es_data.buf1_size,
						DMA_TO_DEVICE);
				outer_clean_range((phys_addr_t)p->cpb.addr+es_data.buf1_offset,
						(phys_addr_t)p->cpb.addr+es_data.buf1_offset + es_data.buf1_size);
				if (es_data.buf2_size > 0)
				{
					logm_debug (vdecd, "vdec:%x, cache to device. %lx, %x, %d\n",
							p->vdec, p->cpb_vaddr,
							es_data.buf2_offset, es_data.buf2_size);

					dmac_map_area((void*)(p->cpb_vaddr+es_data.buf2_offset),
							es_data.buf2_size,
							DMA_TO_DEVICE);
					outer_clean_range((phys_addr_t)p->cpb.addr+es_data.buf2_offset,
							(phys_addr_t)p->cpb.addr+es_data.buf2_offset + es_data.buf2_size);
				}
			}
		}
		else
			logm_warning (vdecd, "vdec:%x, Oops\n",
					p->vdec);
	}


	/* make IPC command and copy argument to parameter area */
	dir = IPC_CALL_DATA_DIR_WR;
	data = kcalloc (8+_IOC_SIZE(cmd), 1, GFP_KERNEL);
	if (!data)
	{
		logm_warning (vdecd, "kcalloc failed for ipc argument\n");
		return -ENOMEM;
	}
	data[0] = cmd;
	data[1] = (unsigned int)arg;
	if (_IOC_SIZE(cmd) > 0)
	{
		if (_IOC_DIR(cmd) & _IOC_WRITE)
		{
			if (copy_from_user (data+2, (void*)arg, _IOC_SIZE(cmd)))
			{
				logm_warning (vdecd, "vdec:%x, copy_from_user failed\n", p->vdec);
				ret =  -EFAULT;
				goto done;
			}
		}
		else
			memset (data+2, 0, _IOC_SIZE(cmd));

		if (_IOC_DIR(cmd) & _IOC_READ)
			dir |= IPC_CALL_DATA_DIR_RD;
	}

	/* call IPC */
	ret = 0;
	call_ret = ipc_call2 (ipc, p->vdec, "dioctl", (char*)data, 8+_IOC_SIZE(cmd), dir, &ret);
	if (call_ret < 0)
	{
		logm_error (vdecd, "call failed\n");

		ret = call_ret;
		goto done;
	}

	/* copy the argument */
	if (_IOC_SIZE(cmd) > 0 && _IOC_DIR(cmd) & _IOC_READ)
	{
		if (copy_to_user ((void*)arg, data+2, _IOC_SIZE(cmd)))
		{
			logm_warning (vdecd, "vdec:%x, copy_to_user failed\n", p->vdec);
			ret = -EFAULT;
			goto done;
		}
	}


	switch (cmd)
	{
	case LX_VDEC_GET_NOTI_INFO:
	case LX_VDEC_GET_NOTI_INFO_WAIT:
		if (!p->noti.addr)
		{
			phys_addr_t phys;

			logm_info (vdecd, "vdec:%x, update noti address\n", p->vdec);
			memcpy (&p->noti, data+2, sizeof (p->noti));

			phys = ipc_vdec_to_phys (ipc, p->noti.addr);
			logm_info (vdecd, "vdec:%x, noti 0x%lx(0x%llx), size %d\n",
					p->vdec,
					p->noti.addr,
					(unsigned long long)phys,
					p->noti.size);

			p->noti.addr = phys;
		}
		break;

	case LX_VDEC_GET_CPB_INFO:
		if (!p->cpb.addr)
		{
			logm_info (vdecd, "vdec:%x, update cpb address\n",
					p->vdec);
			memcpy (&p->cpb, data+2, sizeof (p->cpb));

			logm_info (vdecd, "vdec:%x, cpb 0x%lx, size %d\n",
					p->vdec, p->cpb.addr,
					p->cpb.size);
		}
		break;

	default:
		break;
	}

done:
	kfree (data);

	return ret;
}

static int vdec_open (struct inode *inode, struct file *filp)
{
	int ret, call_ret;
	struct vdec_priv *p;
	unsigned long flag;

	logm_info (vdecd, "vdec:xxxxxxxx, open...\n");


	p = new_priv ();
	if (!p)
		return -ENOMEM;
	filp->private_data = p;


	/* open vdec */
	ret = 0;
	call_ret = ipc_call2 (ipc, 0, "dopen", NULL, 0, 0, &ret);
	if (call_ret < 0)
	{
		logm_error (vdecd, "call failed\n");
		kfree (p);
		return call_ret;
	}
	p->vdec = ret;
	logm_info (vdecd, "vdec:%x, open.\n", p->vdec);
	if (!p->vdec)
	{
		logm_warning (vdecd, "vdec:xxxxxxxx, open failed\n");
		kfree (p);
		return -EIO;
	}

	if (vdec_default_dec && strlen(vdec_default_dec)>3)
	{
		uint32_t fcc;

		logm_info (vdecd, "vdec:%x, set default decoder. \"%s\"\n",
				p->vdec, vdec_default_dec);

		fcc  = vdec_default_dec[0];
		fcc |= vdec_default_dec[1]<<8;
		fcc |= vdec_default_dec[2]<<16;
		fcc |= vdec_default_dec[3]<<24;

		vdec_ioctl (p, LX_VDEC_SET_PREFERRED_DEC, fcc);
	}

	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_add (&p->list, &vdec_priv_list);
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);

	return 0;
}

static int vdec_close (struct inode *inode, struct file *file)
{
	int ret, call_ret;
	struct vdec_priv *p = file->private_data;
	unsigned long flag;

	logm_info (vdecd, "vdec:%x, close\n", p->vdec);

	ret = 0;
	call_ret = ipc_call2 (ipc, p->vdec, "dclose", NULL, 0, 0, &ret);

	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_del (&p->list);
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);

	if (call_ret < 0)
	{
		logm_error (vdecd, "call failed\n");
		return call_ret;
	}

	return 0;
}

static long _vdec_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	return vdec_ioctl (file->private_data, cmd, arg);
}

static unsigned int vdec_poll (struct file *file, struct poll_table_struct *pt)
{
	int ret = 0;
	struct vdec_priv *p = file->private_data;

	poll_wait (file, &p->vdec_poll_wq, pt);

	if (p->vdec_poll & POLLIN)
		ret |= POLLIN;
	if (p->vdec_poll & POLLOUT)
		ret |= POLLOUT;

	return ret;
}


static int mmap_noti (struct vdec_priv *p, struct vm_area_struct *vma)
{
	int length = vma->vm_end - vma->vm_start;

	logm_info (vdecd, "vdec:%x, vm_start %lx, vm_end %lx, noti %lx\n",
			p->vdec, vma->vm_start, vma->vm_end,
			p->noti.addr);

	if (length != p->noti.size)
	{
		logm_warning (vdecd, "vdec:%x, wrong noti map size. %d\n",
				p->vdec, length);
		return -ENXIO;
	}

	vma->vm_flags |= VM_IO;
#ifdef CONFIG_MMU
	vma->vm_page_prot = pgprot_noncached (vma->vm_page_prot);
#endif

	return remap_pfn_range (vma, vma->vm_start,
			p->noti.addr>>PAGE_SHIFT,
			p->noti.size, vma->vm_page_prot);
}


static int mmap_cpb (struct vdec_priv *p, struct vm_area_struct *vma)
{
	int length = vma->vm_end - vma->vm_start;
	int ret;

	if (!p->cpb.addr)
	{
		logm_warning (vdecd, "vdec:%x, we dont have cpb yet\n", p->vdec);

		return -ENXIO;
	}

	if (p->cpb.size != length)
	{
		logm_warning (vdecd, "vdec:%x, wrong cpb size. %d\n",
				p->vdec, length);

		return -ENXIO;
	}

	vma->vm_flags |= VM_IO;

	ret = remap_pfn_range (vma, vma->vm_start,
			p->cpb.addr>>PAGE_SHIFT,
			p->cpb.size, vma->vm_page_prot);

	if (ret >= 0)
	{
		p->cpb_vaddr = vma->vm_start;
		logm_info (vdecd, "vdec:%x, cpb vaddr %lx\n",
				p->vdec, p->cpb_vaddr);
	}

	return ret;
}


static int vdec_mmap (struct file *file, struct vm_area_struct *vma)
{
	struct vdec_priv *p = file->private_data;
	int length = vma->vm_end - vma->vm_start;

	logm_info (vdecd, "vdec:%x, page offset %ld, length %d, map to %lx\n",
			p->vdec, vma->vm_pgoff,
			length, vma->vm_start);

	if (vma->vm_pgoff == 0)
		return mmap_noti (p, vma);

	if (vma->vm_pgoff == (p->noti.size>>PAGE_SHIFT))
		return mmap_cpb (p, vma);;


	logm_warning (vdecd, "vdec:%x, unknown page offset. %lx\n",
			p->vdec, vma->vm_pgoff);

	return -ENXIO;
}


static struct file_operations vdec_fops =
{
	.open = vdec_open,
	.release = vdec_close,
	.unlocked_ioctl = _vdec_ioctl,
	.poll = vdec_poll,
	.mmap = vdec_mmap,
};


static int vo_open (struct inode *inode, struct file *filp)
{
	int ret, call_ret;
	struct vdec_priv *p;
	unsigned long flag;

	logm_info (vod, "vo:xxxxxxxx, open...\n");


	p = new_priv ();
	if (!p)
		return -ENOMEM;
	filp->private_data = p;


	/* open vdec */
	ret = 0;
	call_ret = ipc_call2 (ipc, 0, "oopen", NULL, 0, 0, &ret);
	if (call_ret < 0)
	{
		logm_error (vod, "call failed\n");
		kfree (p);
		return call_ret;
	}
	p->vo = ret;
	logm_info (vod, "vo:%x, open.\n", p->vo);
	if (!p->vo)
	{
		logm_warning (vod, "vo:xxxxxxxx, open failed\n");
		kfree (p);
		return -EIO;
	}


	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_add (&p->list, &vdec_priv_list);
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);

	return 0;
}


static int vo_close (struct inode *inode, struct file *file)
{
	int ret, call_ret;
	struct vdec_priv *p = file->private_data;
	unsigned long flag;

	logm_info (vod, "vo:%x, close\n", p->vo);

	ret = 0;
	call_ret = ipc_call2 (ipc, p->vo, "oclose", NULL, 0, 0, &ret);

	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_del (&p->list);
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);

	if (call_ret < 0)
	{
		logm_error (vod, "call failed\n");
		return call_ret;
	}

	return 0;
}


static long vo_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct vdec_priv *p = file->private_data;
	int ret, call_ret;
	int dir;
	unsigned int *data;

	logm_trace (vod, "vo:%x, cmd = %08x (nr=%d)\n",
			p->vo, cmd, _IOC_NR(cmd));

	if (_IOC_TYPE(cmd) != VO_TYPE)
	{
		logm_warning (vod, "invalid type. %x(%x)\n",
				cmd, _IOC_TYPE(cmd) );
		return -ENOIOCTLCMD;
	}

	dir = IPC_CALL_DATA_DIR_WR;
	data = kcalloc (8+_IOC_SIZE(cmd), 1, GFP_KERNEL);
	if (!data)
	{
		logm_warning (vod, "kcalloc failed for ipc argument\n");
		return -ENOMEM;
	}
	data[0] = cmd;
	data[1] = (unsigned int)arg;
	if (_IOC_SIZE(cmd) > 0)
	{
		if (_IOC_DIR(cmd) & _IOC_WRITE)
		{
			if (copy_from_user (data+2, (void*)arg, _IOC_SIZE(cmd)))
			{
				logm_warning (vod, "vo:%x, copy_from_user failed\n", p->vo);
				ret =  -EFAULT;
				goto done;
			}
		}
		else
			memset (data+2, 0, _IOC_SIZE(cmd));

		if (_IOC_DIR(cmd) & _IOC_READ)
			dir |= IPC_CALL_DATA_DIR_RD;
	}

	/* call IPC */
	ret = 0;
	call_ret = ipc_call2 (ipc, p->vo, "oioctl", (char*)data, 8+_IOC_SIZE(cmd), dir, &ret);
	if (call_ret < 0)
	{
		logm_error (vod, "call failed\n");

		ret = call_ret;
		goto done;
	}

	/* copy the argument */
	if (_IOC_SIZE(cmd) > 0 && _IOC_DIR(cmd) & _IOC_READ)
	{
		if (copy_to_user ((void*)arg, data+2, _IOC_SIZE(cmd)))
		{
			logm_warning (vod, "vo:%x, copy_to_user failed\n", p->vo);
			ret = -EFAULT;
			goto done;
		}
	}

done:
	kfree (data);

	return ret;
}


static unsigned int vo_poll (struct file *file, struct poll_table_struct *pt)
{
	int ret = 0;
	struct vdec_priv *p = file->private_data;

	poll_wait (file, &p->vo_poll_wq, pt);

	if (p->vo_poll & POLLIN)
		ret |= POLLIN;
	if (p->vo_poll & POLLOUT)
		ret |= POLLOUT;

	return ret;
}


static struct file_operations vo_fops =
{
	.open = vo_open,
	.release = vo_close,
	.unlocked_ioctl = vo_ioctl,
	.poll = vo_poll,
};


struct _hmalist
{
	const char *pool;
	phys_addr_t addr;

	struct _hmalist *next;
};
static struct _hmalist *hmalist_root;
static DEFINE_SPINLOCK (hmalist_lock);

static int append_hmalist (const char *pool, phys_addr_t addr)
{
	struct _hmalist *new;
	unsigned long flag;

	new = kmalloc (sizeof (*new), GFP_KERNEL);
	if (!new)
	{
		logm_error (ipc_vdummy, "no memory for %llx on %s\n",
				(long long)addr, pool);
		return -ENOMEM;
	}
	new->pool = pool;
	new->addr = addr;

	spin_lock_irqsave (&hmalist_lock, flag);
	new->next = hmalist_root;
	hmalist_root = new;
	spin_unlock_irqrestore (&hmalist_lock, flag);

	return 0;
}


static int remove_hmalist (const char *pool, phys_addr_t addr)
{
	struct _hmalist *now, *pre;
	unsigned long flag;

	now = hmalist_root;
	pre = NULL;
	spin_lock_irqsave (&hmalist_lock, flag);
	while (now)
	{
		if (now->addr == addr && !strcmp (now->pool, pool))
		{
			if (now == hmalist_root)
				hmalist_root = now->next;
			else
				pre->next = now->next;
			break;
		}
		pre = now;
		now = now->next;
	}
	spin_unlock_irqrestore (&hmalist_lock, flag);

	if (!now)
	{
		logm_error (ipc_vdummy, "unknown memory to remove. %llx from %s\n",
				(long long)addr, pool);
		return -EINVAL;
	}

	kfree (now);

	return 0;
}


static int remove_hmalist_all (void)
{
	struct _hmalist *now;
	unsigned long flag;

	spin_lock_irqsave (&hmalist_lock, flag);
	now = hmalist_root;
	hmalist_root = NULL;
	spin_unlock_irqrestore (&hmalist_lock, flag);

	while (now)
	{
		struct _hmalist *next;

		logm_info (ipc_vdummy, "free %llx from %s\n",
				(long long)now->addr, now->pool);

		hma_free (now->pool, now->addr);
		next = now->next;
		kfree (now);

		now = next;
	}

	return 0;
}


static int ipc_handler (struct ipc *ipc, void *data, int size)
{
	struct ipc_param_arg *arg = data;

	if (size < sizeof (struct ipc_param_arg))
	{
		logm_error (ipc_vdummy, "Oops? size %d\n", size);
		return -1;
	}

	logm_debug (ipc_vdummy, "got %c%c%c%c(%x), priv %x, size %d\n",
			(arg->call>>0)&0xff,
			(arg->call>>8)&0xff,
			(arg->call>>16)&0xff,
			(arg->call>>24)&0xff,
			arg->call, arg->priv,
			size);

#define C(a,b,c,d)	((a<<0)|(b<<8)|(c<<16)|(d<<24))

	/* vdec_alloc() */
	if (arg->call == C('v','a','l','l'))
	{
		phys_addr_t a;
		uint32_t *ipc_arg = (uint32_t*)(arg+1);

		a = hma_alloc_user ("vdec", ipc_arg[0], ipc_arg[1], "vdec_mcu");
		logm_info (ipc_vdummy, "vdec_alloc size:%d(align:%x), ret %x\n",
				ipc_arg[0], ipc_arg[1], a);
		if (!a)
		{
			logm_warning (ipc_vdummy, "alloc failed from vdec\n");
			return 0;
		}

		append_hmalist ("vdec", a);

		return (int)a;
	}

	/* vdec_alloc_dpb() */
	if (arg->call == C('d','a','l','l'))
	{
		phys_addr_t a;
		uint32_t *ipc_arg = (uint32_t*)(arg+1);

		a = hma_alloc_user ("dpb", ipc_arg[0], ipc_arg[1], "vdec_mcu");
		logm_info (ipc_vdummy, "vdec_alloc_dpb size:%d(align:%x), ret %x\n",
				ipc_arg[0], ipc_arg[1], a);
		if (!a)
		{
			logm_warning (ipc_vdummy, "alloc failed from dpb\n");
			return 0;
		}

		append_hmalist ("dpb", a);

		return (int)a;
	}

	/* vdec_free() */
	if (arg->call == C('v','f','r','e'))
	{
		uint32_t *ipc_arg = (uint32_t*)(arg+1);

		logm_info (ipc_vdummy, "vdec_free %x\n", ipc_arg[0]);
		remove_hmalist ("vdec", ipc_arg[0]);
		hma_free ("vdec", ipc_arg[0]);

		return 0;
	}

	/* vdec_free_dpb() */
	if (arg->call == C('d','f','r','e'))
	{
		uint32_t *ipc_arg = (uint32_t*)(arg+1);

		logm_info (ipc_vdummy, "vdec_free_dpb %x\n", ipc_arg[0]);
		remove_hmalist ("dpb", ipc_arg[0]);
		hma_free ("dpb", ipc_arg[0]);

		return 0;
	}


	ipc__dump (data, size);

	return 0;
}


static void fwload_event (const char *name, fwload_event_t event)
{
	unsigned long flag;
	struct list_head *now;

	if (event != fwload_event_reset)
		return;

	logm_info (ipc_vdummy, "mcu, %s, reset\n", name);
	remove_hmalist_all ();
	ipc_reset (ipc);

	/* wake up all polling process */
	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_for_each (now, &vdec_priv_list)
	{
		struct vdec_priv *p =
			container_of (now, struct vdec_priv, list);

		if (p->vdec)
		{
			p->vdec_poll = POLLIN;
			wake_up_all (&p->vdec_poll_wq);
		}
		if (p->vo)
		{
			p->vo_poll = POLLIN;
			wake_up_all (&p->vo_poll_wq);
		}
	}
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);

	return;
}


static int ping_number, ping_number_received;
static int ping_timeout_count;
static int ping_gap = 3000;
module_param (ping_gap, int, 0644);
static void ping_expired (unsigned long arg);
static DEFINE_TIMER (ping_timer, ping_expired, 0, 0);
static DEFINE_SPINLOCK (ping_lock);


static void send_ping (void)
{
	volatile unsigned int *ipc_base = ipc->ipc_base;

	if (ipc_base[0])
	{
		ping_number ++;
		logm_noti (ipc_vdummy, "send ping %d\n", ping_number);

		ipc_send (ipc, ipc_vdec_type_ping, 0, ping_number, 0, NULL);
	}
	else
		logm_noti (ipc_vdummy, "mcu is not running.\n");

	mod_timer (&ping_timer, jiffies+HZ*ping_gap/1000);
}


static int do_reset = 1;
module_param (do_reset, int, 0644);

static void ping_check (struct work_struct *work)
{
	if (do_reset && ping_timeout_count >= 2)
	{
		logm_error (ipc_vdummy, "reset vdec MCU...\n");
		fwload_reset ("vdec");
	}

	send_ping ();
}
static DECLARE_WORK (ping_check_work, ping_check);


static void ping_expired (unsigned long arg)
{
	volatile unsigned int *ipc_base = ipc->ipc_base;
	unsigned long flag;

	spin_lock_irqsave (&ping_lock, flag);

	if (ping_number != ping_number_received)
	{
		ping_timeout_count ++;
		logm_error (ipc_vdummy, "MCU ping timeout. number:%d, count:%d\n",
				ping_number, ping_timeout_count);

		ping_number_received = ping_number;
	}
	else if (!ipc_base[0])
	{
		ping_timeout_count ++;

		logm_error (ipc_vdummy, "vdec MCU net yet ready. count:%d\n",
				ping_timeout_count);
	}
	else
		ping_timeout_count = 0;

	queue_work (system_wq, &ping_check_work);

	spin_unlock_irqrestore (&ping_lock, flag);
}


static int ipc_handler_irq (struct ipc *ipc,
		uint32_t type, uint32_t target_number,
		uint32_t offs, uint32_t size)
{
	uint32_t vdec = target_number;
	struct list_head *now;
	unsigned long flag;
	struct vdec_priv *p = NULL;

	static uint32_t audio_stc[4];

	logm_debug (ipc_vdummy, "got??!! type%d, target%d, offs%d, size%d\n",
			type, target_number,
			offs, size);


	if (!vdec)
	{
		switch (type)
		{
		case ipc_vdec_type_pong:
			spin_lock_irqsave (&ping_lock, flag);
			logm_noti (ipc_vdummy, "got pong. %d\n", offs);
			if (offs != ping_number)
				logm_warning (ipc_vdummy, "got wrong ping number. "
						"got %d, expected %d\n",
						offs, ping_number);

			ping_number_received = offs;
			spin_unlock_irqrestore (&ping_lock, flag);
			break;

		default:
			logm_warning (ipc_vdummy, "unknown type. %d\n", type);
			break;
		}

		return 0;
	}


	/* vdec, vo driver actions */
	spin_lock_irqsave (&vdec_priv_list_lock, flag);
	list_for_each (now, &vdec_priv_list)
	{
		p = container_of (now, struct vdec_priv, list);
		if (p->vdec == vdec)
			break;
		if (p->vo == vdec)
			break;
	}
	spin_unlock_irqrestore (&vdec_priv_list_lock, flag);
	if (now == &vdec_priv_list)
	{
		logm_warning (ipc_vdummy, "unknown vdec or vo instance. %x\n", vdec);
		return -1;
	}

	switch (type)
	{
	case ipc_vdec_type_poll_vdec:
		logm_debug (vdecd, "vdec:%x, change poll. %x->%x(%s%s%s%s%s)\n",
				p->vdec,
				p->vdec_poll, offs,
				(offs&POLLIN)?"IN":"",
				(offs&POLLIN)&&(offs&POLLOUT)?",":"",
				(offs&POLLOUT)?"OUT":"",
				(offs&POLLOUT)&&(offs&POLLERR)?",":"",
				(offs&POLLERR)?"ERR":"");
		p->vdec_poll = offs;
		wake_up_all (&p->vdec_poll_wq);
		break;

	case ipc_vdec_type_poll_vo:
		logm_debug (vod, "vo:%x, change poll. %x->%x(%s%s%s%s%s)\n",
				p->vo,
				p->vo_poll, offs,
				(offs&POLLIN)?"IN":"",
				(offs&POLLIN)&&(offs&POLLOUT)?",":"",
				(offs&POLLOUT)?"OUT":"",
				(offs&POLLOUT)&&(offs&POLLERR)?",":"",
				(offs&POLLERR)?"ERR":"");
		p->vo_poll = offs;
		wake_up_all (&p->vo_poll_wq);
		break;

	case ipc_vdec_type_audioinf_set_basetime_stc:
		if (offs < ARRAY_SIZE (audio_stc))
		{
			logm_info (vdecd, "audio%d stc 0x%08x\n",
					offs, size);
			audio_stc[offs] = size;
		}
		else
			logm_warning (vdecd, "wrong audio stc index. %08x, %08x\n",
					offs, size);

		break;

	case ipc_vdec_type_audioinf_set_basetime_pts:
		if (offs < ARRAY_SIZE (audio_stc))
		{
			extern SINT32 KDRV_AUDIO_SetBasetime(UINT32 index, LX_AUD_RENDER_PARAM_BASETIME_T *pParamBasetime);
			LX_AUD_RENDER_PARAM_BASETIME_T param;

			memset (&param, 0, sizeof (param));
			param.ui64ClockBaseTime = audio_stc[offs];
			param.ui64StreamBaseTime = size;

			logm_info (vdecd, "audio%d, basestc %08llx, basepts %08llx\n",
					offs, param.ui64ClockBaseTime,
					param.ui64StreamBaseTime);

			KDRV_AUDIO_SetBasetime (offs, &param);
		}
		else
			logm_warning (vdecd, "wrong audio pts index. %08x, %08x\n",
					offs, size);

		break;

	case ipc_vdec_type_audioinf_playto: {
		extern SINT32 KDRV_AUDIO_SetPlayToPts(UINT32 index, UINT32 pts);

		logm_info (vdecd, "audio%d, play to 0x%08x\n",
				offs, size);

		KDRV_AUDIO_SetPlayToPts (offs, size);
		break;
	}

	default:
		logm_warning (ipc_vdummy, "unknown type. %d\n", type);
		break;
	}

	return 0;
}


static int ipc_handler_ioctl (struct ipc *ipc,
		unsigned int cmd, unsigned long arg)
{
	logm_debug (ipc_vdummy, "cmd %08x, arg %08lx\n",
			cmd, arg);

	switch (cmd)
	{
	default:
		logm_warning (ipc_vdummy, "unknown cmd. %08x, %08lx\n",
				cmd, arg);
		return -1;

	case C('u','a','r','t'):
		logm_noti (ipc_vdummy, "switch uart to ARM\n");
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 2);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_vdec, 0);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		return 0;

	case C('j','t','a','g'):
		logm_noti (ipc_vdummy, "switch jtag port %ld\n", arg);
		switch (arg)
		{
		default:
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 0);
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, cpu_jtag, 0);
			CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
			break;
		case 0:
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 6);
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, vdec_jtag, 1);
			CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
			break;
		case 1:
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 6);
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, vdec_jtag, 0);
			CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
			break;
		}
		return 0;
	}
}


static int vdec_major = 161;
module_param (vdec_major, int, 0644);

static int vo_major = 1818;
module_param (vo_major, int, 0644);

static int probe (struct platform_device *dev)
{
	logm_info (ipc_vdummy, "...\n");

	return 0;
}

static int remove (struct platform_device *dev)
{
	logm_info (ipc_vdummy, "...\n");

	return 0;
}


/* suspend to ram */
static uint32_t *ipc_registers;
static int suspend (struct platform_device *dev)
{
	unsigned long start, now;

	printk ("vdec: suspend...\n");

	/* send ipc to xtensa */
	ipc_send (ipc, ipc_vdec_type_suspend, 0, 0, 0, NULL);

	/* wait xtensa suspend... */
	start = jiffies;
	do
	{
		volatile uint32_t *resume_addr;

		resume_addr = ((uint32_t*)ipc->ipc_base) + 0x80/4;

		if (*resume_addr != 0)
		{
			printk ("vdec: xtensa suspended. resume at 0x%x\n",
					*resume_addr);
			break;
		}

		now = jiffies;
		if (now - start > HZ)
		{
			printk (KERN_ERR"vdec: cannot stop xtensa. failed.\n");
			return -1;
		}
	}
	while (1);

	/* xtensa ipc register backup */
	ipc_registers = kmalloc (0x100, GFP_ATOMIC);
	if (!ipc_registers)
	{
		printk (KERN_ERR"vdec: cannot allocate register backup memory.\n");
		return -ENOMEM;
	}

	memcpy (ipc_registers, ipc->ipc_base, 0x100);

	printk ("vdec: suspend done.\n");

	return 0;
}

static int resume (struct platform_device *dev)
{
	return 0;
}

static int resume_noirq (struct platform_device *dev)
{
	printk ("vdec: resume_noirq...\n");

	/* restore ipc register */
	if (!ipc_registers)
	{
		printk (KERN_ERR"vdec: Oops\n");
		return -1;
	}
	memcpy (ipc->ipc_base, ipc_registers, 0x100);
	kfree (ipc_registers);
	ipc_registers = NULL;

	/* kick xtensa */

	printk ("vdec: resume_noirq done.\n");

	return 0;
}


/* hivernation */
static int freeze (struct platform_device *dev)
{
	struct list_head *now;

	printk ("vdec: freeze...\n");

	if (list_empty (&vdec_priv_list))
		return 0;


	printk (KERN_ERR"vdec: There are active connections to vdec MCU.\n");
	list_for_each (now, &vdec_priv_list)
	{
		struct vdec_priv *p = container_of (now, struct vdec_priv, list);

		printk (KERN_ERR"vdec: vdec(%08x), vo(%08x)\n",
				p->vdec, p->vo);

	}
	printk (KERN_ERR"vdec: Cannot freeze!\n");
	return -1;
}


static int restore (struct platform_device *dev)
{
	printk ("vdec: restore...\n");

	return 0;
}


static int vdec__prepare	(struct device *dev){printk("%s\n", __func__);return 0;}
static void vdec__complete	(struct device *dev){printk("%s\n", __func__);return;}
static int vdec__suspend	(struct device *dev){printk("%s\n", __func__);return suspend (to_platform_device (dev));}
static int vdec__resume		(struct device *dev){printk("%s\n", __func__);return resume (to_platform_device (dev));}
static int vdec__freeze		(struct device *dev){printk("%s\n", __func__);return freeze (to_platform_device (dev));}
static int vdec__thaw		(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__poweroff	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__restore	(struct device *dev){printk("%s\n", __func__);return restore (to_platform_device (dev));}
static int vdec__suspend_late	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__resume_early	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__freeze_late	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__thaw_early	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__poweroff_late	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__restore_early	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__suspend_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__resume_noirq	(struct device *dev){printk("%s\n", __func__);return resume_noirq (to_platform_device (dev));}
static int vdec__freeze_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__thaw_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__poweroff_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__restore_noirq	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__runtime_suspend(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__runtime_resume	(struct device *dev){printk("%s\n", __func__);return 0;}
static int vdec__runtime_idle	(struct device *dev){printk("%s\n", __func__);return 0;}

static struct dev_pm_ops pm_ops =
{
	.prepare	= vdec__prepare,
	.complete	= vdec__complete,
	.suspend	= vdec__suspend,
	.resume		= vdec__resume,
	.freeze		= vdec__freeze,
	.thaw		= vdec__thaw,
	.poweroff	= vdec__poweroff,
	.restore	= vdec__restore,
	.suspend_late	= vdec__suspend_late,
	.resume_early	= vdec__resume_early,
	.freeze_late	= vdec__freeze_late,
	.thaw_early	= vdec__thaw_early,
	.poweroff_late	= vdec__poweroff_late,
	.restore_early	= vdec__restore_early,
	.suspend_noirq	= vdec__suspend_noirq,
	.resume_noirq	= vdec__resume_noirq,
	.freeze_noirq	= vdec__freeze_noirq,
	.thaw_noirq	= vdec__thaw_noirq,
	.poweroff_noirq	= vdec__poweroff_noirq,
	.restore_noirq	= vdec__restore_noirq,
	.runtime_suspend= vdec__runtime_suspend,
	.runtime_resume	= vdec__runtime_resume,
	.runtime_idle	= vdec__runtime_idle,
};

static struct platform_driver platform_driver =
{
	.probe = probe,
	.remove = remove,
	//.suspend = suspend,
	//.resume = resume,
	.driver =
	{
		.name = "vdec",
		.pm = &pm_ops,
	},
};

static struct platform_device platform_device =
{
	.name = "vdec",
	.id = -1,
};

static int init = 1;
module_param (init, int, 0640);

static int ipc_vdec_dummy_init (void)
{
	int ret;

	printk ("vdec dummy init.\n");

	if (!init)
	{
		logm_error (ipc_vdummy, "no init flag. do not init.\n");
		return -1;
	}

	/* character device */
	ret = register_chrdev (vdec_major, "vdec", &vdec_fops);
	if (ret < 0)
	{
		logm_error (ipc_vdummy, "vdec register_chrdev failed.\n");
		return ret;
	}
	if (vdec_major == 0)
	{
		vdec_major = ret;
		logm_info (ipc_vdummy, "vdec major %d\n", ret);
	}

	ret = register_chrdev (vo_major, "vo", &vo_fops);
	if (ret < 0)
	{
		logm_error (ipc_vdummy, "vo register_chrdev failed.\n");
		return ret;
	}
	if (vo_major == 0)
	{
		vo_major = ret;
		logm_info (ipc_vdummy, "vo major %d\n", ret);
	}


	{
		struct class* class = NULL;

		class = class_create(THIS_MODULE, "vdec" );
		device_create (class, NULL, MKDEV(vdec_major, 0),
				NULL, "vdec");

		class = class_create(THIS_MODULE, "vo" );
		device_create (class, NULL, MKDEV(vo_major, 0),
				NULL, "vo");
	}

	ret = platform_device_register (&platform_device);
	if (ret)
	{
		logm_error (ipc_vdummy, "platform_device_register failed, %d\n", ret);
		goto failed;
	}

	ret = platform_driver_register (&platform_driver);
	if (ret)
	{
		logm_error (ipc_vdummy, "platform_driver_register failed, %d\n", ret);
		goto failed;
	}

	ipc = ipc_vdec_init (ipc_handler, ipc_handler_irq,
			ipc_handler_ioctl, NULL);
	if (ipc == NULL)
	{
		logm_error (ipc_vdummy, "Oops\n");
		goto failed;
	}

	fwload_register_callback ("vdec", fwload_event);

	/* send ping */
	send_ping ();

	return 0;

failed:
	unregister_chrdev (vdec_major, "vdec");

	return ret;
}

static void ipc_vdec_dummy_exit (void)
{
	printk ("vdec dummy exit.\n");

	ipc_vdec_exit ();

	unregister_chrdev (vdec_major, "vdec");
}

module_init (ipc_vdec_dummy_init);
module_exit (ipc_vdec_dummy_exit);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("LG IPC driver");
MODULE_LICENSE("GPL");

