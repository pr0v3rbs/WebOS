
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/kmsg_dump.h>

#include "hma_alloc.h"

//#define LOG_PRINTK
#include "log.h"

logm_define (ipc, log_level_warning);
logm_define (ipc_pipe, log_level_warning);

#include "ipc.h"


static int emergency;
static int message_logm;
module_param_named (ipc_message_logm, message_logm, int, 0644);



enum argument_type
{
	type_new,
	type_kill,
	type_return,

	type_pipe,
	type_log,
	type_ioctl,
};
static const char *type_name (enum argument_type t)
{
	switch (t)
	{
	case type_new:		return "new";
	case type_kill:		return "kill";
	case type_return:	return "return";
	case type_pipe:		return "pipe";
	case type_log:		return "log";
	case type_ioctl:	return "ioctl";
	}

	return "unknown";
}


struct queue
{
	volatile uint32_t addr;
	volatile uint32_t size;
	volatile uint32_t head;
	volatile uint32_t tail;
};

struct dev;
struct queues
{
	struct queue *send;
	struct queue *receive;
	wait_queue_head_t wait;

	struct dev *dev;
	enum argument_type ack_type;
};


/* structure that will be stored in queue area */
struct q_argument
{
	uint32_t target_number;
	uint32_t type;
	uint32_t size;
	uint32_t offs;

	/* ?? */
	uint32_t ipc_number;
};

struct waiting
{
	struct list_head list;

	wait_queue_head_t waiting_wq;

	int ipc_number;
	int got_return;
	int return_value;
};

struct working
{
	struct list_head list;

	struct work_struct work;
	struct dev *dev;
	struct q_argument arg;
	int killed;

	struct task_struct *worker;
};

struct dev
{
	struct ipc ipc;

	struct list_head dev_list;

	char *name;
	char *name_param;

	struct queue *send_queue;
	struct queue *receive_queue;
	struct mutex send_queue_lock;
	spinlock_t send_head_lock;

	struct queues pipe;
	struct queues log;

	struct list_head wait_list_head;
	spinlock_t wait_list_lock;

	struct list_head work_list_head;
	struct working *working_dump;
	int working_dump_num;
	spinlock_t work_list_lock;

	wait_queue_head_t connection_changed;
	struct proc_dir_entry *proc_entry;

	struct list_head user_command_head;
	spinlock_t user_command_head_lock;
	wait_queue_head_t user_command_wait;

	int ipc_count;

	int expected_ipc_number;
	int halt;
	int error_count;

	wait_queue_head_t irqed_wq;
};


void ipc__dump (unsigned char *data, int len )
{
	int a, b;

#define dump_wrap       16
	for( a=0, b=0; a<len; )
	{
		if( a%dump_wrap == 0 )
			printk( "%08x:", a );
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

static int queued_arguments (volatile struct queue *q)
{
	int ret;

	ret = q->head - q->tail;
	if (ret < 0)
		ret += q->size;

	return ret;
}

static int empty_arguments (volatile struct queue *q)
{
	int ret;

	ret = q->tail - q->head;
	if (ret <= 0)
		ret += q->size;

	return ret;
}


int ipc_send_noirq (struct ipc *ipc, uint32_t type,
		uint32_t target_number,
		uint32_t offs, uint32_t size,
		uint32_t *ipc_number)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	volatile struct q_argument arg;
	volatile struct q_argument *arg_queue;
	int head;
	unsigned long flag;
	int ret = 0;

	spin_lock_irqsave (&dev->send_head_lock, flag);

	if (unlikely (dev->halt))
	{
		logm_warning (ipc, "dev:%s, halted. ignore\n",
				ipc->name);
		spin_unlock_irqrestore (&dev->send_head_lock, flag);
		return -EINVAL;
	}

	/* setup argument */
	memset ((void*)&arg, 0, sizeof (arg));
	arg.ipc_number = dev->ipc_count ++;
	arg.type = type;
	arg.target_number = target_number;
	arg.size = size;
	arg.offs = offs;
	logm_debug (ipc, "dev:%s, number%d, type:%s, %d(%d), head %d\n",
			ipc->name,
			arg.ipc_number, type_name(arg.type),
			arg.offs, arg.size,
			dev->send_queue->head);

	/* copy the argument to send_queue */
	arg_queue = (struct q_argument *)(ipc->queue_base + dev->send_queue->addr);
	head = dev->send_queue->head;
	arg_queue[head] = arg;

	/* NOTICE
	 *
	 * If BUS have Fast BValid enabled, we need to re-read to ensure the
	 * write has beed finished.
	 */
	arg = arg_queue[head];

	head ++;
	head %= dev->send_queue->size;
	if (unlikely (emergency))
	{
		while (head == dev->send_queue->tail);

		dev->send_queue->head = head;
	}
	else
	{
		if (head == dev->send_queue->tail)
		{
			logm_error (ipc, "send queue full. tail:%d, head:%d\n",
					dev->send_queue->tail, head);
			ret = -EBUSY;
		}
		else
			dev->send_queue->head = head;
	}

	spin_unlock_irqrestore (&dev->send_head_lock, flag);

	if (ipc_number)
		*ipc_number = arg.ipc_number;

	return ret;
}

int ipc_send (struct ipc *ipc, uint32_t type,
		uint32_t target_number,
		uint32_t offs, uint32_t size,
		uint32_t *ipc_number)
{
	int ret;

	ret = ipc_send_noirq (ipc, type, target_number,
			offs, size, ipc_number);
	if (ret == -EINVAL)
		return ret;

	/* send irq */
	ipc->irq_send (ipc);

	return ret;
}

int ipc_send_ioctl (struct ipc *ipc, unsigned int cmd, unsigned long arg)
{
	ipc_send (ipc, type_ioctl, cmd, arg, 0, NULL);

	return 0;
}


/* Function call to IPC. Argument data must be inside of ipc->arg_base memory.
 */
static int ipc_call (struct ipc *ipc, void *data, int size)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	int ret;
	struct waiting wait;
	unsigned long flag;
	int offs;

	offs = data - ipc->arg_base;

	logm_debug (ipc, "dev:%s, call, offs %d(%d)\n", ipc->name,
			offs, size);

	memset (&wait, 0, sizeof (wait));
	init_waitqueue_head (&wait.waiting_wq);

	/* lock send queue */
	mutex_lock (&dev->send_queue_lock);
	logm_trace (ipc, "dev:%s\n", ipc->name);

	ret = wait_event_interruptible (dev->connection_changed,
			(empty_arguments(dev->send_queue) > 1));
	logm_trace (ipc, "dev:%s\n", ipc->name);
	if (ret < 0)
	{
		logm_info (ipc, "dev:%s, signaled? %d\n", ipc->name, ret);
		mutex_unlock (&dev->send_queue_lock);
		return ret;
	}

	/* add to waiting list */
	spin_lock_irqsave (&dev->wait_list_lock, flag);
	list_add (&wait.list, &dev->wait_list_head);
	ipc_send (ipc, type_new, 0, offs, size, &wait.ipc_number);
	spin_unlock_irqrestore (&dev->wait_list_lock, flag);

	mutex_unlock (&dev->send_queue_lock);


	/* wait IPC returns */
	ret = wait_event_interruptible (wait.waiting_wq,
			wait.got_return);
	logm_trace (ipc, "dev:%s, ret %d\n", ipc->name, ret);
	if (ret < 0)
	{
		logm_info (ipc, "dev:%s, signaled? %d\n", ipc->name, ret);

		mutex_lock (&dev->send_queue_lock);
		logm_trace (ipc, "dev:%s\n", ipc->name);

		/* wait queue have space */
		wait_event (dev->connection_changed,
			       	empty_arguments (dev->send_queue) > 1);
		logm_trace (ipc, "dev:%s\n", ipc->name);

		ipc_send (ipc, type_kill, wait.ipc_number, 0, 0, NULL);

		mutex_unlock (&dev->send_queue_lock);

		/* wait IPC returns */
		wait_event (wait.waiting_wq,
				wait.got_return);
		logm_trace (ipc, "dev:%s\n", ipc->name);
	}

	wake_up (&dev->connection_changed);

	logm_debug (ipc, "dev:%s, number%d, return %d\n", ipc->name,
			wait.ipc_number, wait.return_value);

	return wait.return_value;
}


int ipc_call2 (struct ipc *ipc, uint32_t priv, const char *call,
		void *data, int size,
		int data_dir, int *ret)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	phys_addr_t arg_base;
	int arg_size;
	struct ipc_param_arg *arg;

	logm_debug (ipc, "call \"%s\" (%x, size:%d, dir:%s%s%s)\n",
			call, priv, size,
			(data_dir&IPC_CALL_DATA_DIR_RD)?"read":"",
			data_dir?",":"",
			(data_dir&IPC_CALL_DATA_DIR_WR)?"write":""
			);

	/* allocate argument memory */
	arg_size = sizeof (struct ipc_param_arg) + size;
	arg_base = hma_alloc (dev->name_param, arg_size, 1);
	if (arg_base == 0)
	{
		logm_error (ipc, "no memory for param\n");
		return -ENOMEM;
	}

	/* fill argument */
	arg = ipc->arg_base + (arg_base - ipc->arg_base_phys);
	arg->call =
		(call[0] << 0) |
		(call[1] << 8) |
		(call[2] << 16) |
		(call[3] << 24);
	arg->priv = priv;
	arg->data_size = size;
	if (size > 0)
	{
		if (data_dir & IPC_CALL_DATA_DIR_WR)
			memcpy (arg+1, data, size);
		else
			memset (arg+1, 0, size);
	}

#ifndef __XTENSA__
	/* wait mcu ready */
	*ret = wait_event_interruptible (dev->irqed_wq,
			(((uint32_t*)ipc->ipc_base)[0] != 0));
	if (*ret)
	{
		logm_warning (ipc, "interrupt while mcu is not ready. %d\n",
				*ret);
		hma_free (dev->name_param, arg_base);
		return -ERESTARTSYS;
	}
#endif

	/* call ipc */
	*ret = ipc_call (ipc, arg, size+sizeof(*arg));

	if (size > 0)
	{
		if (data_dir & IPC_CALL_DATA_DIR_RD)
			memcpy (data, arg+1, size);
	}

	/* free argument memory */
	hma_free (dev->name_param, arg_base);

	logm_debug (ipc, "call \"%s\" (%x, size:%d) = %d\n",
			call, priv, size, *ret);

	return 0;
}


int ipc_reset (struct ipc *ipc)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	unsigned long flag;
	struct list_head *t, *now;

	logm_noti (ipc, "dev:%s, reset ipc.\n", ipc->name);

	spin_lock_irqsave (&dev->wait_list_lock, flag);
	list_for_each_safe (now, t, &dev->wait_list_head)
	{
		struct waiting *now_wait =
			container_of (now, struct waiting, list);

		list_del (&now_wait->list);
		now_wait->return_value = -EIO;
		now_wait->got_return = 1;
		wake_up (&now_wait->waiting_wq);
	}
	spin_unlock_irqrestore (&dev->wait_list_lock, flag);

	return 0;
}


static void ipc_worker(struct work_struct *_work)
{
	struct working *work = container_of (_work, struct working, work);
	struct dev *dev = work->dev;
	struct ipc *ipc = &dev->ipc;
	struct q_argument *arg = &work->arg;
	int ret;
	unsigned long flag;
	struct list_head *now;
	int in_list;

	logm_debug (ipc, "dev:%s\n", ipc->name);

	/* allow signal */
	allow_signal (SIGKILL);

	/* register myself to work list to get signal */
	spin_lock_irqsave (&dev->work_list_lock, flag);
	/* check the work is listed in work_list.
	 * before ipc_worker() called, kill signal will remove the work in
	 * work_list */
	in_list = 0;
	list_for_each (now, &dev->work_list_head)
	{
		if (container_of (now, struct working, list) == work)
		{
			in_list = 1;
			break;
		}
	}
	work->worker = current;
	spin_unlock_irqrestore (&dev->work_list_lock, flag);
	if (!in_list)
	{
		logm_error (ipc, "dev:%s, Oops! not in list\n", ipc->name);
		kfree (work);
		goto out;
	}
	if (work->killed)
	{
		logm_warning (ipc, "dev:%s, got kill signal. suicide.\n",
				ipc->name);
		send_sig_info (SIGKILL, SEND_SIG_NOINFO, current);
	}

	/* call handler */
	ret = ipc->handler (ipc,
			ipc->arg_base + arg->offs, arg->size);
	logm_debug (ipc, "dev:%s, got ipc return %d\n",
			ipc->name, ret);

	/* lock send queue */
	mutex_lock (&dev->send_queue_lock);
	logm_trace (ipc, "dev:%s\n", ipc->name);

	wait_event (dev->connection_changed,
			empty_arguments (dev->send_queue) > 1);
	logm_trace (ipc, "dev:%s\n", ipc->name);

	ipc_send (ipc, type_return, arg->ipc_number, ret, 0, NULL);
	mutex_unlock (&dev->send_queue_lock);

	/* remove myself from work_list */
	spin_lock_irqsave (&dev->work_list_lock, flag);
	list_del (&work->list);
	spin_unlock_irqrestore (&dev->work_list_lock, flag);

	logm_debug (ipc, "dev:%s, done.\n", ipc->name);
	kfree (work);

out:
	disallow_signal (SIGKILL);
	flush_signals (current);
}

static int do_new (struct dev *dev, struct q_argument *arg)
{
	struct ipc *ipc = &dev->ipc;
	unsigned long flag;
	struct working *work;

	logm_debug (ipc, "dev:%s, new ipc\n", ipc->name);

	work = kcalloc (1, sizeof (struct working), GFP_ATOMIC);
	if (!work)
	{
		logm_error (ipc, "dev:%s, kcalloc failed\n", ipc->name);
		return -1;
	}

	INIT_WORK (&work->work, ipc_worker);
	work->dev = dev;
	work->arg = *arg;

	spin_lock_irqsave (&dev->work_list_lock, flag);

	list_add (&work->list, &dev->work_list_head);
	queue_work (system_wq, &work->work);

	spin_unlock_irqrestore (&dev->work_list_lock, flag);

	logm_debug (ipc, "dev:%s, done\n", ipc->name);

	return 0;
}


static int do_return (struct dev *dev, struct q_argument *arg)
{
	struct ipc *ipc = &dev->ipc;
	unsigned long flag;
	struct waiting *wait;
	struct list_head *now;

	logm_debug (ipc, "dev:%s, return ipc\n", ipc->name);

	spin_lock_irqsave (&dev->wait_list_lock, flag);
	wait = NULL;
	list_for_each (now, &dev->wait_list_head)
	{
		struct waiting *now_wait =
			container_of (now, struct waiting, list);

		if (now_wait->ipc_number == arg->target_number)
		{
			wait = now_wait;
			break;
		}
	}
	if (likely (wait))
		list_del (&wait->list);
	else
	{
		logm_error (ipc, "dev:%s, no waiting for an argument. target %d, ipc_count %d\n",
				ipc->name, arg->target_number,
				dev->ipc_count);
		list_for_each (now, &dev->wait_list_head)
		{
			struct waiting *now_wait = container_of (now, struct waiting, list);
			logm_error (ipc, "dev:%s, ipc_number %d\n", ipc->name,
					now_wait->ipc_number);
		}
		logm_error (ipc, "dev:%s, list done\n", ipc->name);

		spin_unlock_irqrestore (&dev->wait_list_lock, flag);
		return -1;
	}
	spin_unlock_irqrestore (&dev->wait_list_lock, flag);

	wait->return_value = arg->offs;
	wait->got_return = 1;
	wake_up (&wait->waiting_wq);

	return 0;
}


static int do_kill (struct dev *dev, struct q_argument *arg)
{
	struct ipc *ipc = &dev->ipc;;
	struct list_head *now;
	unsigned long flag;

	logm_warning (ipc, "dev:%s, kill target %d\n", ipc->name,
			arg->target_number);

	/* search the killing work */
	spin_lock_irqsave (&dev->work_list_lock, flag);
	list_for_each (now, &dev->work_list_head)
	{
		struct working *work;
		work = container_of (now, struct working, list);

		if (work->arg.ipc_number == arg->target_number)
		{
			work->killed = 1;
			if (!work->worker)
			{
				logm_warning (ipc, "dev:%s, no worker yet\n",
						ipc->name);
				break;
			}
			send_sig_info (SIGKILL, SEND_SIG_NOINFO, work->worker);
			break;
		}
	}
	spin_unlock_irqrestore (&dev->work_list_lock, flag);

	if (now == &dev->work_list_head)
		logm_warning (ipc, "dev:%s, cannot find the work. "
				"already done?\n",
				ipc->name);

	return 0;
}


static int do_pipe (struct dev *dev)
{
	struct ipc *ipc = &dev->ipc;
	struct queues *qs;
	struct queue *r;

	qs = &dev->pipe;
	r = qs->receive;

	if (r->size == 0)
	{
		logm_warning (ipc, "dev:%s, no size\n", ipc->name);
		return -1;
	}

	logm_debug (ipc, "dev:%s, tail:%05x, head:%05x\n", ipc->name,
			r->tail, r->head);

	wake_up (&qs->wait);

	return 0;
}


#ifndef __XTENSA__
static int do_log (struct dev *dev)
{
	struct ipc *ipc = &dev->ipc;
	struct queues *qs;
	struct queue *r;
	int h, t;
	int size;

	qs = &dev->log;
	r = qs->receive;

	if (r->size == 0)
	{
		logm_warning (ipc, "dev:%s, no size\n", ipc->name);
		return -1;
	}
	size = r->size;

	logm_debug (ipc, "dev:%s, tail:%05x, head:%05x\n", ipc->name,
			r->tail, r->head);

	//wake_up (&qs->wait);
	h = r->head;
	t = r->tail;

	while (t != h)
	{
		int a;
		char *p = ipc->arg_base+r->addr;
		char *tmp;

		/* search newline */
		for (a=t; a != h && a < size; a++)
		{
			if (p[a] == '\n')
				break;
		}
		if (a == h)
		{
			logm_debug (ipc, "dev:%s, no newline.\n",
					ipc->name);
			break;
		}

		if (a != size)
		{
			p[a] = 0;

			if (unlikely(message_logm))
				logm_warning (ipc, "dev:%s, message : %s\n",
						ipc->name, p+t);
			else
				printk (KERN_INFO "dev:%s, message : %s\n",
						ipc->name, p+t);
			t = a+1;
			t %= size;
			continue;
		}

		/* message wrapped
		 * search newline */
		for (a=0; a != h; a++)
		{
			if (p[a] == '\n')
				break;
		}
		if (a == h)
		{
			logm_debug (ipc, "dev:%s, no newline.\n",
					ipc->name);
			break;
		}

		logm_info (ipc, "dev:%s, message wrapped. from %d to %d\n",
				ipc->name, t, a);
		tmp = kmalloc (size-t + a + 1, GFP_ATOMIC);
		if (tmp == NULL)
		{
			logm_warning (ipc, "dev:%s, no mem for wrapped message. from %d to %d\n",
					ipc->name, t, a);
		}
		else
		{
			p[a] = 0;
			memcpy (tmp, p + t, size-t);
			memcpy (tmp+size-t, p, a+1);

			if (unlikely(message_logm))
				logm_warning (ipc, "dev:%s, message : %s\n",
						ipc->name, p+t);
			else
				printk (KERN_INFO "dev:%s, message : %s\n",
						ipc->name, tmp);
			kfree (tmp);
		}
		t = a+1;
	}

	r->tail = t;
	logm_debug (ipc, "dev:%s, tail:%05x, head:%05x\n", ipc->name,
			r->tail, r->head);

	return 0;
}
#endif


static int do_ioctl (struct dev *dev, struct q_argument *arg)
{
	struct ipc *ipc = &dev->ipc;

	if (!ipc->handler_ioctl)
	{
		logm_warning (ipc, "dev:%s, no ioctl function\n",
				ipc->name);
		return -1;
	}

	logm_debug (ipc, "dev:%s, ioctl %08x, %08x\n",
			ipc->name, arg->offs, arg->size);

	return ipc->handler_ioctl (ipc,
			arg->target_number, arg->offs);
}


static int process_message (struct dev *dev, struct ipc *ipc,
		struct q_argument *arg)
{
	int ret;

	logm_debug (ipc, "dev:%s, number %d, target %d, %s ipc(%d), %d(%d), queued %d\n",
			ipc->name, arg->ipc_number, arg->target_number,
			type_name(arg->type), arg->type,
			arg->offs, arg->size,
			queued_arguments (dev->receive_queue));

	wake_up_all (&dev->irqed_wq);


	/* check ipc number increase monotonically */
	if (unlikely (dev->expected_ipc_number != arg->ipc_number))
	{
		logm_warning (ipc, "wrong ipc number??? expected:0x%x, now:0x%x\n",
				dev->expected_ipc_number,
				arg->ipc_number);

		dev->expected_ipc_number = arg->ipc_number;
	}


	/* handle each type of message */
	if (arg->type == type_new)
	{
		return do_new (dev, arg);
	}

	if (arg->type == type_return)
	{
		return do_return (dev, arg);
	}

	if (arg->type == type_kill)
	{
		do_kill (dev, arg);
		return 0;
	}

	if (arg->type == type_pipe)
	{
		do_pipe (dev);
		return 0;
	}

#ifndef __XTENSA__
	if (arg->type == type_log)
	{
		do_log (dev);
		return 0;
	}
#endif

	if (arg->type == type_ioctl)
	{
		do_ioctl (dev, arg);
		return 0;
	}

	if (ipc->handler_irq)
	{
		ret = ipc->handler_irq (ipc, arg->type,
				arg->target_number,
				arg->offs, arg->size);
		if (ret >= 0)
			return 0;
	}

	logm_error (ipc, "dev:%s, unknown ipc type 0x%08x. number 0x%x, target 0x%x, %d(%d)\n",
			ipc->name, arg->type, arg->ipc_number, arg->target_number,
			arg->offs, arg->size);

	return -1;
}


static irqreturn_t ipc_interrupt (int irq, void *_dev)
{
	struct dev *dev = _dev;
	struct ipc *ipc = &dev->ipc;
	struct q_argument arg;
	volatile struct q_argument *arg_queue;
	int tail;

	logm_debug (ipc, "dev:%s, %d\n", ipc->name, irq);

	if (!ipc->irq_status (ipc))
	{
		logm_trace (ipc, "dev:%s, none\n", ipc->name);
		return IRQ_NONE;
	}

	ipc->irq_clear (ipc);

	if (unlikely (dev->halt))
	{
		logm_error (ipc, "halted. ignore\n");
		return IRQ_HANDLED;
	}

	/* read message */
	arg_queue = (struct q_argument *)(ipc->queue_base + dev->receive_queue->addr);
	tail = dev->receive_queue->tail;
	dev->error_count = 0;
	while (tail != dev->receive_queue->head)
	{
		arg = arg_queue[tail];
		//memset (arg_queue+tail, 0xff, sizeof(arg));

		/* FIXME
		 * While decoding HEVC video, IPC argument is not written on
		 * DDR memory in rare case. I guess that this is due to the
		 * asynchronous operations between DDR memory and BUS. Need
		 * confirms from hardware engineer.
		 */
		if (process_message (dev, ipc, &arg) < 0)
		{
			logm_error (ipc, "cannot process a message. tail:%d, head:%d\n",
					tail, dev->receive_queue->head);
		}
		dev->expected_ipc_number = arg.ipc_number + 1;

		tail ++;
		tail %= dev->receive_queue->size;
	}
	dev->receive_queue->tail = tail;

	return IRQ_HANDLED;
}


struct user_command_entry
{
	struct list_head list;

	int len;
	char str[0];
};


int ipc_user_command (struct ipc *ipc, const char *str)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	unsigned long flag;
	int len = strlen (str);
	struct user_command_entry *c;

	logm_info (ipc, "got user command. %s\n", str);

	c = kmalloc (len+1+sizeof(struct user_command_entry), GFP_ATOMIC);
	if (!c)
	{
		logm_error (ipc, "no mem.\n");
		return -ENOMEM;
	}
	c->len = len;
	memcpy (c->str, str, len+1);

	spin_lock_irqsave (&dev->user_command_head_lock, flag);
	list_add_tail (&c->list, &dev->user_command_head);
	spin_unlock_irqrestore (&dev->user_command_head_lock, flag);

	wake_up (&dev->user_command_wait);

	return 0;
}


static void *worklist_start (struct seq_file *m, loff_t *pos)
{
	struct dev *dev = m->private;

	if (*pos >= dev->working_dump_num)
		return NULL;

	return dev->working_dump+*pos;
}


static int worklist_show (struct seq_file *m, void *v)
{
	struct dev *dev = m->private;
	struct working *w = v;

	return seq_printf (m, "dev:%s, target_number %08x, type %s(%d), size %08x, offs %08x, ipc_number %08x\n",
			dev->ipc.name,
			w->arg.target_number,
			type_name(w->arg.type),
			w->arg.type,
			w->arg.size,
			w->arg.offs,
			w->arg.ipc_number);
}


static void *worklist_next (struct seq_file *m, void *v, loff_t *pos)
{
	struct dev *dev = m->private;

	*pos += 1;
	if (*pos >= dev->working_dump_num)
		return NULL;

	return dev->working_dump+*pos;
}


static void worklist_stop (struct seq_file *m, void *v)
{
}


static struct seq_operations worklist_seq_op =
{
	.start = worklist_start,
	.stop = worklist_stop,
	.next = worklist_next,
	.show = worklist_show,
};


static int worklist_open (struct inode *inode, struct file *file)
{
	struct dev *dev;
	int ret = 0;
	unsigned long flag;
	struct seq_file *seq;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	dev = dp->data;

#else
	dev = PDE_DATA(inode);
#endif

	spin_lock_irqsave (&dev->work_list_lock, flag);
	if (dev->working_dump)
	{
		logm_error (ipc, "already opened?\n");
		ret = -EBUSY;
	}
	else
	{
		struct list_head *now;
		int num;

		num = 0;
		list_for_each (now, &dev->work_list_head)
			num ++;
		dev->working_dump_num = num;
		logm_debug (ipc, "working num:%d\n", num);

		dev->working_dump = kmalloc (num*sizeof(struct working),
				GFP_ATOMIC);
		if (dev->working_dump)
		{
			num = 0;
			list_for_each (now, &dev->work_list_head)
			{
				struct working *w = container_of (now, struct working, list);

				dev->working_dump[num] = *w;

				num ++;
			}
		}
		else
		{
			logm_error (ipc, "no mem\n");
			ret = -ENOMEM;
		}
	}
	spin_unlock_irqrestore (&dev->work_list_lock, flag);

	if (ret < 0)
		return ret;

	ret = seq_open (file, &worklist_seq_op);
	if (ret < 0)
		return ret;
	seq = file->private_data;
	seq->private = dev;

	return ret;
}


static int worklist_release (struct inode *inode, struct file *file)
{
	int ret;
	struct dev *dev;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
        struct proc_dir_entry *dp;
        dp = PDE (inode);
        dev = dp->data;

#else
	dev = PDE_DATA (inode);
#endif

	kfree (dev->working_dump);
	dev->working_dump = NULL;
	ret = seq_release (inode, file);

	return ret;
}


static struct file_operations proc_work_list_fops =
{
	.open = worklist_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = worklist_release,
};


static int command_open (struct inode *inode, struct file *file)
{
	struct ipc *ipc;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	ipc = dp->data;

#else
	ipc = PDE_DATA(inode);
#endif

	file->private_data = container_of (ipc, struct dev, ipc);

	return 0;
}


static ssize_t command_read (struct file *file,
		char __user *buf, size_t len, loff_t *off)
{
	struct dev *dev = file->private_data;
	struct list_head *_c, *n;
	int ret;
	unsigned long flag;

	ret = wait_event_interruptible (dev->user_command_wait,
			!list_empty (&dev->user_command_head));
	if (ret < 0)
		return ret;

	ret = 0;
	spin_lock_irqsave (&dev->user_command_head_lock, flag);
	list_for_each_safe (_c, n, &dev->user_command_head)
	{
		struct user_command_entry *c =
			container_of (_c, struct user_command_entry, list);
		int r;
		char newline = '\n';

		logm_debug (ipc, "command %s\n", c->str);

		if (len+1 < c->len)
			break;

		spin_unlock_irqrestore (&dev->user_command_head_lock, flag);
		r = copy_to_user (buf, c->str, c->len);
		spin_lock_irqsave (&dev->user_command_head_lock, flag);
		if (r)
		{
			ret = -EFAULT;
			break;
		}

		list_del (&c->list);

		buf += c->len;
		ret += c->len;
		len -= c->len;

		kfree (c);

		spin_unlock_irqrestore (&dev->user_command_head_lock, flag);
		r = put_user (newline, buf);
		spin_lock_irqsave (&dev->user_command_head_lock, flag);
		if (r)
		{
			ret = -EFAULT;
			break;
		}
		buf ++;
		ret ++;
		len --;
	}
	spin_unlock_irqrestore (&dev->user_command_head_lock, flag);

	return ret;
}


static ssize_t command_write (struct file *file,
		const char __user *buf, size_t len, loff_t *off)
{
	char *t, *p;
	int ret;
	int a;
	struct dev *dev = file->private_data;
	struct ipc *ipc = &dev->ipc;

	t = kmalloc (len+1, GFP_KERNEL);
	if (!t)
		return -ENOMEM;

	ret = copy_from_user (t, buf, len);
	if (ret)
	{
		ret = -EFAULT;
		goto done;
	}
	t[len] = 0;

	p = t;
	logm_debug (ipc, "buffer %p\n", p);
	for (a=0; a<len; a++)
	{
		if (t[a] == '\n' || t[a] == 0)
		{
			logm_debug (ipc, "got line break at %p\n", t+a);
			t[a] = 0;
			if (*p)
			{
				logm_debug (ipc, "user command, %p, %s\n", p, p);
				ipc_user_command (ipc, p);
			}

			p = t+a+1;
		}
	}
	if (*p)
	{
		logm_debug (ipc, "last user command, %p, %s\n", p, p);
		ipc_user_command (ipc, p);
	}
	ret = len;

done:
	kfree (t);

	return ret;
}


static int command_release (struct inode *inode, struct file *file)
{
	return 0;
}


static struct file_operations proc_user_command_fops =
{
	.open = command_open,
	.read = command_read,
	.write = command_write,
	.release = command_release,
};


static ssize_t queue_read (struct file *file,
		char __user *buf, size_t len, loff_t *off)
{
	int ret;
	struct queues *qs = file->private_data;
	struct dev *dev = qs->dev;
	struct ipc *ipc = &dev->ipc;
	struct queue *r = qs->receive;
	int c1, c2;
	int h, t;

	ret = wait_event_interruptible (qs->wait,
			r->size && (r->head != r->tail));
	if (ret < 0)
	{
		logm_noti (ipc_pipe, "dev:%s, signal\n", ipc->name);
		return ret;
	}

	h = r->head;
	t = r->tail;

	if (t < h)
	{
		c1 = h-t;
		c2 = 0;
	}
	else
	{
		c1 = r->size-t;
		c2 = h;
	}

	if (c1 > len)
	{
		c1 = len;
		c2 = 0;
	}
	else if (c1 + c2 > len)
	{
		c2 = len-c1;
	}

	logm_debug (ipc_pipe, "dev:%s, tail:%05x, head:%05x, c1:%d, c2:%d, len:%d\n",
			ipc->name, r->tail, r->head, c1, c2, len);

	if (c1 > 0)
	{
		ret = copy_to_user (buf, ipc->arg_base+r->addr+t, c1);
		if (ret)
			return -EFAULT;
		t += c1;
		t %= r->size;
	}
	if (c2 > 0)
	{
		ret = copy_to_user (buf+c1, ipc->arg_base+r->addr, c2);
		if (ret)
			return -EFAULT;
		t = c2;
	}

	r->tail = t;

	ipc_send (ipc, qs->ack_type, 0, 0, 0, NULL);

	return c1+c2;
}


static ssize_t queue_write (struct file *file,
		const char __user *buf, size_t len, loff_t *off)
{
	ssize_t written;
	struct queues *qs = file->private_data;
	struct dev *dev = qs->dev;
	struct ipc *ipc = &dev->ipc;
	struct queue *s = qs->send;

	written = 0;
	while (len > 0)
	{
		int ret;
		int c;
		int h, t;

		ret = wait_event_interruptible (qs->wait,
				s->tail != (s->head+1)%s->size);
		if (ret < 0)
			break;

		h = s->head;
		t = s->tail;

		if (t <= h)
			c = s->size-h;
		else
			c = t-h-1;

		if (c > len)
			c = len;

		logm_debug (ipc_pipe, "dev:%s, tail:%05x, head:%05x, c:%d\n",
				ipc->name, t, h, c);

		ret = copy_from_user (ipc->arg_base + s->addr + h,
				buf+written, c);
		if (ret)
		{
			written = -EFAULT;
			break;
		}

		written += c;
		h += c;
		h %= s->size;
		len -= c;

		s->head = h;
	}

	ipc_send (ipc, qs->ack_type, 0, 0, 0, NULL);

	logm_debug (ipc_pipe, "dev:%s, written %d\n", ipc->name, written);

	return written;
}


static int pipe_open (struct inode *inode, struct file *file)
{
	struct ipc *ipc;
	struct dev *dev;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	ipc = dp->data;

#else
	ipc = PDE_DATA(inode);
#endif

	dev = container_of (ipc, struct dev, ipc);
	file->private_data = &dev->pipe;

	return 0;
}


static int log_open (struct inode *inode, struct file *file)
{
	struct ipc *ipc;
	struct dev *dev;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	ipc = dp->data;

#else
	ipc = PDE_DATA(inode);
#endif

	dev = container_of (ipc, struct dev, ipc);
	file->private_data = &dev->log;

	return 0;
}


static struct file_operations proc_pipe_fops =
{
	.open = pipe_open,
	.read = queue_read,
	.write = queue_write,
};


static struct file_operations proc_log_fops =
{
	.open = log_open,
	.read = queue_read,
	.write = queue_write,
};


static LIST_HEAD (dev_list_head);

static int ipc_open (struct inode *inode, struct file *file)
{
	struct list_head *now;
	int minor;
	struct dev *dev;

	minor = 0;
	list_for_each (now, &dev_list_head)
	{
		if (iminor (inode) == minor)
			break;

		minor ++;
	}
	if (now == &dev_list_head)
	{
		logm_warning (ipc, "unknown minor. %d\n",
				iminor (inode));
		return -ENODEV;
	}

	dev = container_of (now, struct dev, dev_list);
	file->private_data = dev;

	return 0;
}


static long ipc_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct dev *dev = file->private_data;
	struct ipc *ipc = &dev->ipc;

	logm_debug (ipc, "dev:%s, ioctl %08x, arg %08lx\n",
			ipc->name, cmd, arg);

	if (ipc->ioctl)
		return ipc->ioctl (cmd, arg);

	logm_warning (ipc, "dev:%s, no ioctl\n", ipc->name);

	return -ENODEV;
}


static struct file_operations ipc_fops =
{
	.open = ipc_open,
	.unlocked_ioctl = ipc_ioctl,
};


static struct proc_dir_entry *proc_root_entry;


#ifdef __XTENSA__
static char dump_line[1024];

static inline void save_messoge (struct dev *dev, char *data, int len)
{
	struct ipc *ipc = &dev->ipc;
	struct queue *s = dev->log.send;
	int size = s->size;
	int c1, c2;
	int h, t;

	/* save to log buffer */
	/* enough buffer?? */
	do
	{
		int c;

		h = s->head;
		t = s->tail;

		if (t <= h)
			c = size - h + t - 1;
		else
			c = t-h-1;

		if (c < len)
			ipc_send (ipc, type_log, 0, 0, 0, NULL);
		else
			break;
	} while (1);

	c1 = c2 = 0;
	if (t <= h)
		c1 = size - h;
	else
		c1 = t - h - 1;

	if (c1 > len)
		c1 = len;
	c2 = len - c1;

	memcpy (ipc->arg_base + s->addr + h,
			data, c1);
	if (c2)
		memcpy (ipc->arg_base + s->addr,
				data, c2);

	h += len;
	h %= size;

	s->head = h;

	ipc_send (ipc, type_log, 0, 0, 0, NULL);
}

static void ipc_dump (struct kmsg_dumper *dumper,
		enum kmsg_dump_reason reason)
{
	struct dev *dev;
	struct list_head *list;
	char *msg;
	size_t len;

	list = dev_list_head.next;
	if (!list)
		return;

	dev = container_of (list, struct dev, dev_list);

	switch (reason)
	{
	default:			msg = "unknown"; break;
	case KMSG_DUMP_UNDEF:		msg = "undefined"; break;
	case KMSG_DUMP_PANIC:		msg = "panic"; break;
	case KMSG_DUMP_OOPS:		msg = "oops"; break;
	case KMSG_DUMP_EMERG:		msg = "emerg"; break;
	case KMSG_DUMP_RESTART:		msg = "restart"; break;
	case KMSG_DUMP_HALT:		msg = "halt"; break;
	case KMSG_DUMP_POWEROFF:	msg = "power"; break;
	}
	len = sprintf (dump_line, "dump kernel message. reason : %s\n", msg);


	emergency = 1;

	save_messoge (dev, "\n", 1);
	save_messoge (dev, "\n", 1);
	save_messoge (dev, "\n", 1);
	save_messoge (dev, dump_line, len);
	save_messoge (dev, "\n", 1);
	save_messoge (dev, "\n", 1);
	save_messoge (dev, "\n", 1);

	while (1)
	{
		bool ret;

		ret = kmsg_dump_get_line (dumper, false,
				dump_line, sizeof (dump_line), &len);
		if (ret == false)
			break;

		save_messoge (dev, dump_line, len);
	}
}


static struct kmsg_dumper dumper =
{
	.dump = ipc_dump,
};


#ifdef __XTENSA__
#include <asm/cacheflush.h>
#endif

static int panic_callback (struct notifier_block *nb, unsigned long val,
		void *v)
{
	struct dev *dev;
	struct list_head *list;
	char *buf = v;
	int len;

	list = dev_list_head.next;
	if (!list)
		return 0;

	dev = container_of (list, struct dev, dev_list);

	len = sprintf (dump_line, "panic : %s\n", buf);

	save_messoge (dev, "\n", 1);
	save_messoge (dev, dump_line, len);
	save_messoge (dev, "\n", 1);

	flush_cache_all();
	while (1);

	return 0;
}


static struct notifier_block panic_notifier =
{
	.notifier_call = panic_callback,
};
#endif


struct ipc* ipc_new (void)
{
	struct dev *dev;
	struct ipc *ipc;

	dev = kcalloc (1, sizeof (struct dev), GFP_KERNEL);
	if (!dev)
	{
		logm_error (ipc, "kcalloc failed\n");
		return NULL;
	}

	ipc = &dev->ipc;

	mutex_init (&dev->send_queue_lock);
	spin_lock_init (&dev->send_head_lock);
	spin_lock_init (&dev->wait_list_lock);
	spin_lock_init (&dev->work_list_lock);
	INIT_LIST_HEAD (&dev->wait_list_head);
	INIT_LIST_HEAD (&dev->work_list_head);
	INIT_LIST_HEAD (&dev->dev_list);

	list_add_tail (&dev->dev_list, &dev_list_head);

	return ipc;
}


int ipc_init (struct ipc *ipc)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);
	int ret;
	uint32_t *ipc_base;

	dev->name = kmalloc (8+strlen (ipc->name), GFP_KERNEL);
	dev->name_param = kmalloc (16+strlen (ipc->name), GFP_KERNEL);
	if (!dev->name || !dev->name_param)
	{
		logm_error (ipc, "dev:%s, no mem\n", ipc->name);
		return -ENOMEM;
	}
	sprintf (dev->name, "ipc_%s", ipc->name);
	sprintf (dev->name_param, "ipc_param_%s", ipc->name);
	init_waitqueue_head (&dev->connection_changed);


	/* initialize proc interface */
	dev->proc_entry = proc_mkdir (ipc->name, proc_root_entry);
	proc_create_data ("work_list", 0440, dev->proc_entry,
			&proc_work_list_fops, dev);
#if 0
	proc_create_data ("wait_list", 0440, dev->proc_entry,
			&proc_wait_list_fops, dev);
#endif
	proc_create_data ("user_command", 0440, dev->proc_entry,
			&proc_user_command_fops, ipc);
	proc_create_data ("pipe", 0640, dev->proc_entry,
			&proc_pipe_fops, ipc);
	proc_create_data ("log", 0640, dev->proc_entry,
			&proc_log_fops, ipc);
	INIT_LIST_HEAD (&dev->user_command_head);
	spin_lock_init (&dev->user_command_head_lock);
	init_waitqueue_head (&dev->user_command_wait);
	init_waitqueue_head (&dev->irqed_wq);
	init_waitqueue_head (&dev->pipe.wait);
	init_waitqueue_head (&dev->log.wait);



	/* initialize ipc queue area */
	ipc_base = ipc->ipc_base;

#ifdef __XTENSA__
	dev->send_queue =    (struct queue*)(ipc_base + 0x20/4);
	dev->receive_queue = (struct queue*)(ipc_base + 0x10/4);
	dev->send_queue->addr = ipc->queue_size/2;

	dev->pipe.send =     (struct queue*)(ipc_base + 0x40/4);
	dev->pipe.receive =  (struct queue*)(ipc_base + 0x30/4);
	dev->pipe.send->addr = 0xa0000;

	dev->log.send =      (struct queue*)(ipc_base + 0x60/4);
	dev->log.receive =   (struct queue*)(ipc_base + 0x50/4);
	dev->log.send->addr  = 0xe0000;
#else
	dev->send_queue =    (struct queue*)(ipc_base + 0x10/4);
	dev->receive_queue = (struct queue*)(ipc_base + 0x20/4);
	dev->send_queue->addr = 0;

	dev->pipe.send =     (struct queue*)(ipc_base + 0x30/4);
	dev->pipe.receive =  (struct queue*)(ipc_base + 0x40/4);
	dev->pipe.send->addr = 0x80000;

	dev->log.send =      (struct queue*)(ipc_base + 0x50/4);
	dev->log.receive =   (struct queue*)(ipc_base + 0x60/4);
	dev->log.send->addr  = 0xc0000;
#endif
	dev->send_queue->size = ipc->queue_size/2/sizeof(struct q_argument);
	dev->send_queue->head = 0;
	dev->send_queue->tail = 0;

	dev->pipe.send->head = 0;
	dev->pipe.send->tail = 0;
	dev->pipe.send->size = 0x20000;
	dev->pipe.dev = dev;
	dev->pipe.ack_type = type_pipe;

	dev->log.send->head = 0;
	dev->log.send->tail = 0;
	dev->log.send->size = 0x20000;
	dev->log.dev = dev;
	dev->log.ack_type = type_log;


	/* init argument area */
#ifdef __XTENSA__
	hma_pool_register (dev->name_param,
			ipc->arg_base_phys+ipc->arg_size/4, ipc->arg_size/4);
#else
	hma_pool_register (dev->name_param,
			ipc->arg_base_phys, ipc->arg_size/4);
#endif

	logm_info (ipc, "dev:%s, shared %p(%d)\n", ipc->name,
			ipc->queue_base, ipc->queue_size);


	/* initialize interrupt */
	ret = request_irq (ipc->irq_num, ipc_interrupt,
#if defined(CHIP_NAME_m16)
			IRQF_SHARED|IRQF_TRIGGER_HIGH,
#else
			IRQF_SHARED,
#endif
			dev->name, dev);
	if (ret < 0)
	{
		logm_error (ipc, "dev:%s, request_irq failed. %d\n", ipc->name, ret);
		return ret;
	}

#ifdef __XTENSA__
	kmsg_dump_register (&dumper);
	atomic_notifier_chain_register (&panic_notifier_list,
			&panic_notifier);
#endif

	return 0;
}

void ipc_del (struct ipc *ipc)
{
	struct dev *dev = container_of (ipc, struct dev, ipc);

	logm_info (ipc, "dev:%s, delete.\n", ipc->name);

	free_irq (ipc->irq_num, dev);
	hma_pool_unregister (dev->name_param);
	mutex_destroy (&dev->send_queue_lock);
	kfree (dev->name);
	kfree (dev->name_param);
	kfree (ipc);
}

static int ipc_major = 1213;
module_param (ipc_major, int, 0644);

int ipc_core_init (void)
{
	int ret;

	printk (KERN_INFO "lxipc: init\n");

	ret = register_chrdev (ipc_major, "ipc", &ipc_fops);
	if (ret < 0)
		logm_warning (ipc, "ipc register_chrdev failed. %d\n", ret);
	else if (ipc_major == 0)
		ipc_major = ret;

	/* make proc */
	proc_root_entry = proc_mkdir ("lxipc", NULL);

	return 0;
}

void ipc_core_exit (void)
{
	printk (KERN_INFO "lxipc: exit\n");

	remove_proc_entry ("lxipc", NULL);
}
