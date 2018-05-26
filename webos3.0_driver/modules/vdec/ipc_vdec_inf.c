
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>

#include "ves/ves_cpb.h"
#include "ves/ves_auib.h"
#include "decoder.h"
#include "vo.h"
#include "output.h"

#include "ipc.h"
#include "ipc_vdec.h"

//#define LOG_PRINTK
#define LOG_NAME ipc_vinf
#define LOG_LEVEL log_level_warning
#include "log.h"


#include "include/lxipc.h"



struct ipc *vdec_ipc;

static void dump (unsigned char *data, int len )
{
	int a, b;

#define dump_wrap       16
	for( a=0, b=0; a<len; )
	{
		if( a%dump_wrap == 0 )
			printk( "%08x:", (unsigned int)(data+a) );
		printk( " %02x", data[a] );
		a++;
		if( a%dump_wrap == 0 )
		{
			printk( "  " );
			for( ; b<a; b++ )
				printk( "%c", (' ' <= data[b] && data[b] <= '~')?data[b]:'.' );
			printk( "\n" );
		}
	}

	if( a%dump_wrap != 0 )
	{
		for( ; a%dump_wrap != 0; a++ )
			printk( "   " );

		printk( "  " );
		for( ; b<len; b++ )
			printk( "%c", (' ' <= data[b] && data[b] <= '~')?data[b]:'.' );
		printk( "\n" );
	}

	return;
}


struct handle
{
	struct list_head list;

	/* vdec */
	vdec_t *dec;
	int dec_poll;
	spinlock_t dec_poll_lock;

	/* vo */
	vo_t *out;
	int out_poll;
	spinlock_t out_poll_lock;
};


static LIST_HEAD (handle_list);
static DEFINE_SPINLOCK (handle_list_lock);


unsigned int ipc_vdec_alloc (int size, unsigned int align, const char *name)
{
	uint32_t arg[2];
	int ret, call_ret;

	arg[0] = size;
	arg[1] = align;
	ret = 0;

	log_debug ("vdec_alloc size:%d, align:%x, name:%s\n",
			size, align, name);
	call_ret = ipc_call2 (vdec_ipc, 0, "vall", arg, sizeof(arg),
			IPC_CALL_DATA_DIR_WR, &ret);
	if (call_ret < 0)
	{
		log_error ("call failed. %d\n", call_ret);
		return 0;
	}

	log_info ("vdec_alloc size:%d, align:%x, name:%s, addr:%x\n",
			size, align, name, ret);

	return (unsigned int)ret;
}
EXPORT_SYMBOL (ipc_vdec_alloc);


unsigned int ipc_vdec_alloc_dpb (int size, unsigned int align, const char *name)
{
	uint32_t arg[2];
	int ret, call_ret;

	arg[0] = size;
	arg[1] = align;
	ret = 0;

	log_debug ("vdec_alloc_dpb size:%d, align:%x, name:%s\n",
			size, align, name);
	call_ret = ipc_call2 (vdec_ipc, 0, "dall", arg, sizeof(arg),
			IPC_CALL_DATA_DIR_WR, &ret);
	if (call_ret < 0)
	{
		log_error ("call failed. %d\n", call_ret);
		return 0;
	}

	log_info ("vdec_alloc size:%d, align:%x, name:%s, addr:%x\n",
			size, align, name, ret);

	return (unsigned int)ret;
}
EXPORT_SYMBOL (ipc_vdec_alloc_dpb);


void ipc_vdec_free (unsigned int addr)
{
	uint32_t arg[1];
	int ret, call_ret;

	log_info ("vdec_free addr:%x\n", addr);

	arg[0] = addr;
	ret = 0;
	call_ret = ipc_call2 (vdec_ipc, 0, "vfre", arg, sizeof(arg),
			IPC_CALL_DATA_DIR_WR, &ret);
	if (call_ret < 0)
	{
		log_error ("call failed. %d\n", call_ret);
		return;
	}
}
EXPORT_SYMBOL (ipc_vdec_free);


void ipc_vdec_free_dpb (unsigned int addr)
{
	uint32_t arg[1];
	int ret, call_ret;

	log_info ("vdec_free_dpb addr:%x\n", addr);

	arg[0] = addr;
	ret = 0;
	call_ret = ipc_call2 (vdec_ipc, 0, "dfre", arg, sizeof(arg),
			IPC_CALL_DATA_DIR_WR, &ret);
	if (call_ret < 0)
	{
		log_error ("call failed. %d\n", call_ret);
		return;
	}
}
EXPORT_SYMBOL (ipc_vdec_free_dpb);


int ipc_vdec_update_poll (struct handle *h, vdec_t *d,
		int update_in, int update_out)
{
	int poll;
	unsigned long flag;

	if (!h)
	{
		struct list_head *now;

		spin_lock_irqsave (&handle_list_lock, flag);
		list_for_each (now, &handle_list)
		{
			h = container_of (now, struct handle, list);
			if (h->dec == d)
				break;
		}

		if (now == &handle_list)
		{
			log_warning ("unknown decoder instance. %p\n\n", d);
			spin_unlock_irqrestore (&handle_list_lock, flag);
			return -1;
		}

		spin_unlock_irqrestore (&handle_list_lock, flag);
	}

	spin_lock_irqsave (&h->dec_poll_lock, flag);
	poll = h->dec_poll;

	if (update_in)
	{
		if (d->noti.head == d->noti.tail)
			poll &= ~POLLIN;
		else
			poll |= POLLIN;
	}
	if (update_out)
	{
		if (d->ves == 0xff)
			poll |= POLLOUT;
		else if (
				d->decoding_queue_size > VES_CPB_GetUsedBuffer (d->ves) &&
				d->decoding_queue_slots > VES_AUIB_NumActive (d->ves)
		   )
			poll |= POLLOUT;
		else
			poll &= ~POLLOUT;
	}

	if (poll != h->dec_poll)
	{
		log_debug ("set decoder poll. %x->%x(%s%s%s%s%s)\n",
				h->dec_poll, poll,
				(poll&POLLIN)?"IN":"",
				(poll&POLLIN)&&(poll&POLLOUT)?",":"",
				(poll&POLLOUT)?"OUT":"",
				(poll&POLLOUT)&&(poll&POLLERR)?",":"",
				(poll&POLLERR)?"ERR":"");
		ipc_send (vdec_ipc, ipc_vdec_type_poll_vdec,
				(uint32_t)d, poll, 0, NULL);
		h->dec_poll = poll;
	}
	spin_unlock_irqrestore (&h->dec_poll_lock, flag);

	return 0;
}


static int ipc_vdec_ioctl (struct handle *h,
		unsigned int cmd, unsigned long arg)
{
	vdec_t *d = h->dec;
	int ret;
	//union ioctl_args *a = (void*)arg;

	log_debug ("vdec ioctl(%p, %x, %lx)\n",
			d, cmd, arg);

	ret = vdec_ioctl (d, cmd, arg, 0);

	log_debug ("vdec ioctl(%p, %x, %lx) = %d\n",
			d, cmd, arg, ret);

	switch (cmd)
	{
#if 0
		/* control cache */
	case LX_VDEC_GET_NOTI_INFO:
	case LX_VDEC_GET_NOTI_INFO_WAIT: {
		void *start;
		int size1, size2;
		dma_addr_t da;

		if (a->buffer_info.read_offset <=
				a->buffer_info.write_offset)
		{
			size1 = a->buffer_info.write_offset -
				a->buffer_info.read_offset;
			size2 = 0;
		}
		else
		{
			size1 = a->buffer_info.size -
				a->buffer_info.read_offset;
			size2 = a->buffer_info.write_offset;
		}

		printk ("noti info. %d, %d\n",
				a->buffer_info.read_offset,
				a->buffer_info.write_offset
				);
		start = (void*)a->buffer_info.addr+a->buffer_info.read_offset;
		if (size1 > 0)
		{
			printk ("map1 %p(%d)\n",
					start,
					size1);
			da = dma_map_single (NULL,
					start,
					size1,
					DMA_TO_DEVICE
					);
			dma_unmap_single (NULL,
					da, size1, DMA_TO_DEVICE);

			{
				int size = size1;
				static int diffcnt;

				if (size > 0x80)
					size = 0x80;

				*(uint32_t*)0xf0000870 = start;
				*(uint32_t*)0xf0000874 = size;

				ipc_call (vdec_ipc, NULL, 0);

				if (memcmp (0xf0000880, start, size))
				{
					diffcnt++;

					if (diffcnt < 16)
					{
						printk ("diff %d\n", diffcnt);
						dump (start, size);
						dump (0xf0000880, size);
					}
				}
			}
		}
		if (size2 > 0)
		{
			printk ("map2 %lx(%d)\n",
					a->buffer_info.addr,
					size2);
			da = dma_map_single (NULL,
					a->buffer_info.addr,
					size2,
					DMA_TO_DEVICE
				       );
			dma_unmap_single (NULL,
					da, size2, DMA_TO_DEVICE);
		}

		break;
	}
#endif

		/* update poll status */
	case LX_VDEC_SET_NOTI_TAIL:
	case LX_VDEC_PUT_ES_BUFFER:
		ipc_vdec_update_poll (h, h->dec,
				cmd==LX_VDEC_SET_NOTI_TAIL,
				cmd==LX_VDEC_PUT_ES_BUFFER);
		break;

	default:
		break;
	}

	return ret;
}


int ipc_vo_update_poll (struct handle *h, vo_t *o,
		int update_in, int update_out)
{
	int poll;
	unsigned long flag;

	if (!h)
	{
		struct list_head *now;

		spin_lock_irqsave (&handle_list_lock, flag);
		list_for_each (now, &handle_list)
		{
			h = container_of (now, struct handle, list);
			if (h->out == o)
				break;
		}

		if (now == &handle_list)
		{
			log_warning ("unknown output instance. %p\n\n", o);
			spin_unlock_irqrestore (&handle_list_lock, flag);
			return -1;
		}

		spin_unlock_irqrestore (&handle_list_lock, flag);
	}

	spin_lock_irqsave (&h->out_poll_lock, flag);
	poll = h->out_poll;

	if (unlikely (o->sync < 0))
	{
		log_trace ("vo:%p, no sync\n", o);
		poll = POLLERR;
	}
	else
	{
		if (update_in)
		{
			if (o->displayed_queue_head != o->displayed_queue_tail)
				poll |= POLLIN;
			else
				poll &= ~POLLIN;
		}
		if (update_out)
		{
			int display_queue_num;

			display_queue_num = output_cmd (o->sync,
					output_cmd_display_queued, 0);

			if (display_queue_num < o->queue_size)
				poll |= POLLOUT;
			else
				poll &= ~POLLOUT;
		}
	}

	if (poll != h->out_poll)
	{
		log_debug ("set output poll. %x->%x(%s%s%s%s%s)\n",
				h->out_poll, poll,
				(poll&POLLIN)?"IN":"",
				(poll&POLLIN)&&(poll&POLLOUT)?",":"",
				(poll&POLLOUT)?"OUT":"",
				(poll&POLLOUT)&&(poll&POLLERR)?",":"",
				(poll&POLLERR)?"ERR":"");
		ipc_send (vdec_ipc, ipc_vdec_type_poll_vo,
				(uint32_t)o, poll, 0, NULL);
		h->out_poll = poll;
	}
	spin_unlock_irqrestore (&h->out_poll_lock, flag);

	return 0;
}


static int ipc_vo_ioctl (struct handle *h,
		unsigned int cmd, unsigned long arg)
{
	vo_t *o = h->out;
	int ret;
	//union ioctl_args *a = (void*)arg;

	log_debug ("vo ioctl(%p, %x, %lx)\n",
			o, cmd, arg);

	ret = vo_ioctl (o, cmd, arg, 0);

	log_debug ("vo ioctl(%p, %x, %lx) = %d\n",
			o, cmd, arg, ret);

	switch (cmd)
	{
		/* update poll status */
	case VO_GET_INFO:
		ipc_vo_update_poll (h, h->out,
				1, 0);
		break;

	case VO_WRITE_PICTURE:
		ipc_vo_update_poll (h, h->out,
				0, 1);
		break;

	default:
		break;
	}

	return ret;
}


struct handle *new_handle (void)
{
	struct handle *h;
	unsigned long flag;

	h = kcalloc (1, sizeof (struct handle), GFP_KERNEL);
	if (!h)
	{
		log_error ("no mem for vdec priv\n");
		return NULL;
	}

	INIT_LIST_HEAD (&h->list);
	h->dec_poll = POLLOUT;
	spin_lock_init (&h->dec_poll_lock);

	spin_lock_irqsave (&handle_list_lock, flag);
	list_add (&h->list, &handle_list);
	spin_unlock_irqrestore (&handle_list_lock, flag);

	return h;
}


static struct handle *search_handle_locked (vdec_t *d, vo_t *o)
{
	struct list_head *now;
	struct handle *h = NULL;

	list_for_each (now, &handle_list)
	{
		h = container_of (now, struct handle, list);
		if (d && h->dec == d)
			break;

		if (o && h->out == o)
			break;
	}

	if (now == &handle_list)
	{
		log_warning ("unknown decoder instance. %p\n", d);
		return NULL;
	}

	return h;
}


static int ipc_handler (struct ipc *ipc, void *data, int size)
{
	struct ipc_param_arg *arg = data;
	unsigned long flag;

	if (size < sizeof (struct ipc_param_arg))
	{
		log_error ("Oops? size %d\n", size);
		return -1;
	}

	log_debug ("got %c%c%c%c(%x), priv %x, size %d\n",
			(arg->call>>0)&0xff,
			(arg->call>>8)&0xff,
			(arg->call>>16)&0xff,
			(arg->call>>24)&0xff,
			arg->call, arg->priv,
			size);

#define C(a,b,c,d)	((a<<0)|(b<<8)|(c<<16)|(d<<24))

	/* vdec driver */
	if (arg->call == C('d','o','p','e'))
	{
		vdec_t *d;

		log_info ("vdec open()\n");
		d = vdec_open (NULL);
		if (d)
		{
			struct handle *h;

			h = new_handle ();
			if (h)
				h->dec = d;
		}

		return (int)d;
	}
	if (arg->call == C('d','c','l','o'))
	{
		vdec_t *d = (void*)arg->priv;
		struct handle *h = NULL;

		spin_lock_irqsave (&handle_list_lock, flag);
		h = search_handle_locked (d, NULL);
		if (!h)
		{
			spin_unlock_irqrestore (&handle_list_lock, flag);
			log_warning ("Oops\n");
			return -EIO;
		}
		list_del (&h->list);
		spin_unlock_irqrestore (&handle_list_lock, flag);
		kfree (h);

		log_info ("vdec close()\n");
		return vdec_close (d);
	}
	if (arg->call == C('d','i','o','c'))
	{
		vdec_t *d = (void*)arg->priv;
		struct handle *h = NULL;

		unsigned int *ipc_arg = (unsigned int*)(arg+1);
		unsigned int ioctl_cmd = ipc_arg[0];
		unsigned long ioctl_arg = (unsigned long)ipc_arg[1];

		spin_lock_irqsave (&handle_list_lock, flag);
		h = search_handle_locked (d, NULL);
		if (!h)
		{
			log_warning ("unknown decoder instance. %p\n\n", d);
			spin_unlock_irqrestore (&handle_list_lock, flag);
			return -EIO;
		}
		spin_unlock_irqrestore (&handle_list_lock, flag);

		if (_IOC_SIZE(ioctl_cmd) > 0)
			ioctl_arg = (unsigned long)(ipc_arg+2);

		return ipc_vdec_ioctl (h, ioctl_cmd, ioctl_arg);
	}


	/* vo driver */
	if (arg->call == C('o','o','p','e'))
	{
		vo_t *o;

		log_info ("vo open()\n");
		o = vo_open ();
		if (o)
		{
			struct handle *h;

			h = new_handle ();
			if (h)
				h->out = o;
		}

		return (int)o;
	}
	if (arg->call == C('o','c','l','o'))
	{
		vo_t *o = (void*)arg->priv;
		struct handle *h = NULL;

		spin_lock_irqsave (&handle_list_lock, flag);
		h = search_handle_locked (NULL, o);
		if (!h)
		{
			spin_unlock_irqrestore (&handle_list_lock, flag);
			log_warning ("Oops\n");
			return -EIO;
		}
		list_del (&h->list);
		spin_unlock_irqrestore (&handle_list_lock, flag);
		kfree (h);

		log_info ("vo close()\n");
		return vo_release (o);
	}
	if (arg->call == C('o','i','o','c'))
	{
		vo_t *o = (void*)arg->priv;
		struct handle *h = NULL;

		unsigned int *ipc_arg = (unsigned int*)(arg+1);
		unsigned int ioctl_cmd = ipc_arg[0];
		unsigned long ioctl_arg = (unsigned long)ipc_arg[1];

		spin_lock_irqsave (&handle_list_lock, flag);
		h = search_handle_locked (NULL, o);
		if (!h)
		{
			log_warning ("unknown output instance. %p\n", o);
			spin_unlock_irqrestore (&handle_list_lock, flag);
			return -EIO;
		}
		spin_unlock_irqrestore (&handle_list_lock, flag);

		if (_IOC_SIZE(ioctl_cmd) > 0)
			ioctl_arg = (unsigned long)(ipc_arg+2);

		return ipc_vo_ioctl (h, ioctl_cmd, ioctl_arg);
	}


	log_warning ("unknown argument\n");
	dump (data, size);

	return 0;
}


static int ipc_handler_irq (struct ipc *ipc,
		uint32_t type, uint32_t target_number,
		uint32_t offs, uint32_t size)
{
	log_noti ("got %d\n", type);

	switch (type)
	{
	case ipc_vdec_type_suspend:
		log_noti ("suspend...\n");
		ipc_user_command (ipc, "suspend");
		break;

	case ipc_vdec_type_ping:
		log_debug ("got ping. %d\n", offs);
		ipc_send (ipc, ipc_vdec_type_pong, 0, offs, 0, NULL);
		break;

	default:
		log_warning ("unknown type. %d\n", type);
		break;
	}

	return 0;
}

static int ipc_ioctl (unsigned int cmd, unsigned long arg)
{
	if (cmd == LXIPC_IO_UART)
		return ipc_send_ioctl (vdec_ipc, C('u','a','r','t'), 0);

	if (cmd == LXIPC_IO_JTAG)
		return ipc_send_ioctl (vdec_ipc, C('j','t','a','g'), arg);

	return -ENOIOCTLCMD;
}

static int ipc_vdec_inf_init (void)
{
	printk ("vdec ipc interface\n");

	vdec_ipc = ipc_vdec_init (ipc_handler, ipc_handler_irq,
			NULL, ipc_ioctl);
	if (!vdec_ipc)
	{
		log_error ("Oops\n");
		return -1;
	}

	return 0;
}

static void ipc_vdec_inf_exit (void)
{
	ipc_vdec_exit ();
}

module_init (ipc_vdec_inf_init);
module_exit (ipc_vdec_inf_exit);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("LG IPC driver");
MODULE_LICENSE("GPL");

