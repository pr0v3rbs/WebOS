
/**
 * \mainpage Hardware Memory Allocator
 *
 * HMA는 Hardware(Hyunwoo?) Memory Allocator의 약자로 gfx, jpeg decoder,
 * video decoder, display engine 등 hardware element가 사용하는 메모리를
 * 관리하는 기능을 갓는 모듈이다. 이 메모리 할당자는 CMA(<a
 * href="http://lwn.net/Articles/461849/">Contiguous Memory Allocator</a>)의
 * 초기 버전을 많이 참고하였다.
 *
 * GP4에서 gfx, jpeg decoder 등 각각의 hardware element는 고정된 위치의
 * 메모리를 갓고 있는데, jpeg 그림을 decoding하거나 decoding된 그림을 display
 * engine으로 출력할 때 그 상황에 맞는 크기의 하드웨어 메모리가 필요하며,
 * 필요한 메모리 갯수 또한 상황에 따라 달라진다.
 *
 * 예를들어 jpeg decoding의 경우 decoding된 그림을 저장하는 메모리를 항상
 * 고정된 위치의 메모리 주소로 사용하고 있는데, 이는 web browser, MJPEG video
 * play 등 다중 프로세서에서 동시에 jpeg decoder로 접근할 때 decoded memory의
 * 일관성이 깨지는 문제가 생길 수 있다. 또한, 현재 gfx에 구현된 graphic
 * surface용 hardware memory allocator의 경우 alloc 후 free를 하지 않고
 * 프로세서를 끝내면 메모리 누수가 생기는 문제를 갓고 있어 이에 대한 보안도
 * 필요하다.
 *
 * 이러한 문제를 해결하기 위해 hardware memory를 관리하는 간단한 모듈을 만들어
 * 사용하고자 한다.  항상 동작하지 않는 hardware에 고정으로 할당된 memory 또한
 * 동적 할당을 할 수 있을 것으로 기대하며, 이런 경우 현재 부족한 메모리 자원을
 * 효과적으로 사용할 수 있을 것으로 기대한다.
 *
 *
 * \section structural 구성 및 구현 내용
 *
 * \subsection memorypool memory pool
 *
 * hardware module은 특정 영역의 메모리만을 접근할 수 있는 제약을 갓고 있기도
 * 하다. 예를 들어 display engine의 경우 첫번째 hardware memory bank(?)를 주로
 * 접근할 수 있다. 또한, memory allocator에서 사용할 수 있는 memory 공간이
 * 연속적이지 않고 몇군데로 나누어져 있을 수 있다. 이러한 경우를 고려해 메모리
 * 영역의 특성과 사용가능한 메모리 공간을 적당한 구역으로 나누어 memory pool로
 * 구분하였다. memory pool은 커다란 memory 덩어리로써, memory allocator가
 * 메모리를 할당할 수 있는 영역이 된다.  즉, 메모리를 할당하고자 하는 수요자는
 * 자기가 메모리를 할당하고자 하는 memory pool을 미리 알고 있어야 한다.
 *
 * memory pool은 한개만 있을 수도 있지만, 두개 이상이 존재할 수도 있다. 당연히
 * memory pool이 없다면 memory allocator가 동작할 수 없으며, memory allocator는
 * memory pool에서 적당히 빈 공간을 찾아 메모리를 할당해 준다.
 *
 *
 * \subsection memoryallocator memory allocator
 *
 * memory allocator는 주어진 memory pool에서 요청된 크기의 메모리를 요청된
 * address align에 맞도록 메모리를 할당한다. 보통 하드웨어 메모리의 경우 빈번히
 * 할당/해제가 되지 않고, 모듈의 시작지점에서 필요한 memory를 할당하고 모듈의
 * 동작이 끝나면 할당된 memory를 해제하므로 그렇게 고성능의 allocation
 * algorithm이 필요하지 않다. 또한, 하드웨어에 따라 요구하는 address align이
 * 다르므로 allocation algorithm을 최적화 시키기가 힘들다. 이러한 이유 때문에,
 * HMA에서 구현된 memory allocaton의 동작은 최대한 간단하게 했으며, 수행 성능은
 * 특별히 고려하지 않았다.
 *
 * memory allocator는 chunk 를 최소 단위로 해 linked list로 관리한다.
 * chunk 는 메모리의 주소, 크기, 현재 할당 되어 사용중인지, 할당한
 * 프로세스 이름은 무엇인지 등의 정보를 갓고 있다. chunk 의 linked
 * list는 pool 에 속해 있으며, pool 은 memory pool의 이름,
 * 시작주소, 크기, chunk 를 접근할 때 사용하는 spin lock, chunk 의 root
 * pointer 등을 갓고 있다.
 *
 *
 * \subsection procinterface /proc interface
 *
 * /proc 는 kernel 에서 제공하는 파일 시스템으로, 커널 내부의 드라이버나 각종
 * 정보를 사용자에게 전달해주는 파일 시스템이다. HMA의 memory allocator는 내부
 * 상태, 메모리의 사용 정보, 할당 정보, 등의 모든 정보를 /proc/hma 디렉토리
 * 아래에 있는 파일들로 사용자에게 전달한다. 또한 할당된 메모리 내용을 dump
 * 할수 있도록 하는 기능또한 지원한다.
 *
 * memory allocator에서 사용하는 각각의 pool 은 /proc/hma 디렉토리 아래에서
 * 하나의 디렉토리로 나타나며, 디렉토리 이름은 그 pool의 이름이다. 그 디렉토리
 * 안의 status 파일의 내용은 그 pool 에서 할당된 메모리 상태를 나타낸다.
 *
 * enable_dump 모듈 파라미터가 0이 아닌 값을 가질 때에는 hma_alloc() 함수
 * 등에서 할당하는 메모리에 대해 /proc/hma/pool_name/ 디렉토리 아래에 메모리
 * 덤프 용도로 할당 된 시작 주소에 해당하는 파일 이름을 생성한다. 이 파일을
 * 읽음으로써 할당된 memory chunk 의 내용을 읽을 수 있으며, dd, cat 프로그램을
 * 이용해 해당 메모리 내용을 dump 할수 있다.
 *
 * \subsection hma_driver user level interface
 *
 * memory allocator는 hma_alloc_user(), hma_alloc(), hma_free() 등 메모리를
 * 할당하고 반납하는 두개의 함수를 하드웨어 메모리가 필요한 드라이버에게
 * 제공한다.  하지만, 이는 커널 내부 함수로써, 드라이버 등 커널 레벨 함수이기
 * 때문에, 사용자 어플리케이션에서 바로 사용할 수 없다. 사용자
 * 어플리케이션에서도 이러한 하드웨어 메모리를 사용할 일이 있는데, 이때
 * hma_alloc(), hma_free() 함수를 제공하기 위해 character device driver를
 * 만들었다.
 *
 * 여기서 제공하는 character device driver는 하나의 file descriptor에서 하나의
 * chunk 를 할당할 수 있다. 사용자 어플리케이션에서 여러개의 메모리
 * 할당을 원한다면, 각각의 메모리 할당을 할때마다 여기서 제공하는 character
 * device driver를 open 해야 할 것이다.
 *
 * 사용자 어플리케이션에서 하나의 메모리를 할당하기 위해서는 먼저 device
 * driver를 open 한 다음 ioctl을 통해 사용하고자 하는 pool 이름과
 * 할당하는 메모리 크기, 메모리 시작주소의 alignment 값을 \ref hma_ioctl_alloc
 * 구조체를 통해 전달하면 된다. 사용한 메모리를 반납하기 위해 device driver를
 * close 하면 된다.
 *
 *
 * \subsection hma_library user level library
 *
 * \ref hma_driver 의 경우 device driver를 통해 open, ioctl, close system call
 * 함수를 호출해야 하므로 사용하기가 번거롭다. 사용자 라이브러리에서 이를
 * 사용하기 쉽게 하기 위해 \ref hma_driver 를 간단히 감싸는 라이브러리를 제공한다.
 *
 * 사용자 어플리케이션에서 메모리를 할당하기 위해서는 libhma_alloc() 함수를
 * 이용해 메모리를 할당한다. 할당한 메모리를 반납하기 위해서는 libhma_free()
 * 함수를 이용한다.
 *
 * 만약 사용자 어플리케이션을 끝내기 전에 할당한 메모리를 반납하는 경우라면,
 * libhma_free 함수는 호출할 필요가 없다. 할당한 메모리를 그대로 둔 채 사용자
 * 프로그램을 끝내면(exit() 함수를 호출 하거나 main()함수를 끝내버리면) 리눅스
 * 커널에서 현재 open 된 파일을 자동으로 close 해주기 때문에 \ref hma_driver
 * 드라이버에서 자동으로 hma_free() 함수를 호출 해 줄 것이다.
 *
 *
 * \section reference 참고 링크
 *  -# <a href="https://www.linux.com/learn/linux-career-center/37985-the-kernel-newbie-corner-kernel-debugging-using-proc-qsequenceq-files-part-1">kernel debugging using proc "sequence" files</a>
 *  -# <a href="http://lwn.net/Articles/461849/">contiguous memory allocator</a>
 *
 */

/**
 * \defgroup hma HMA
 *
 * hardware memory를 관리하는 핵심 모듈이다. hma_pool_register(),
 * hma_pool_unregister() 등의 함수로 memory pool을 관리하며, hma_alloc_user(),
 * hma_alloc(), hma_free() 등의 함수로 특정 메모리 pool 안에서 원하는 크기의
 * 메메모리를 할당하고 반납한다.
 *
 * memory pool은 사용 가능한 빈 공간의 메모리 덩이를 말하며, HMA 모듈이
 * 동작하기 위해서는 하나 이상의 memory pool이 주어져야 한다. 각각의 memory
 * pool은 null byte를 포함해 HMA_POOL_LEN 크기의 이름을 가지며, 이 이름을
 * 이용해 hma_alloc_user(), hma_alloc(), hma_free() 에서 memory pool을 지정할
 * 수 있다.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/version.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "hma_alloc.h"


#define debug(fmt,args...)	do{ if (debug_level > 0) printk(KERN_DEBUG"%s.%d: "fmt,__func__,__LINE__,##args); }while(0)
#define info(fmt,args...)	printk(KERN_INFO"%s.%d: "fmt,__func__,__LINE__,##args)
#define error(fmt,args...)	printk(KERN_ERR"%s.%d: "fmt,__func__,__LINE__,##args)

static int debug_level;
module_param_named (debug_alloc, debug_level, int, 0640);

static int enable_dump;
module_param (enable_dump, int, 0644);

static int enable_profiling;
module_param (enable_profiling, int, 0644);

static int dump_status_on_no_memory;
module_param (dump_status_on_no_memory, int, 0644);

static int dump_stack_on_no_memory;
module_param (dump_stack_on_no_memory, int, 0644);

/*
 * 메모리 할당을 위해 나타내는 최소의 메모리 조각을 나타낸다.
 */
struct hma_chunk
{
	unsigned long addr;
	int size;

	char *user;
	atomic_t users;

	struct hma_chunk *prev, *next;

	/* for memory dump */
	char *dumpname;

	/* virtual address for proc memory dump */
	void *virt;
};

/*
 * 메모리 영역을 나타낸다.
 *
 * hma_pool 은 hma_chunk을 포함하며, 메모리 영역을 나타낸다.
 */
struct hma_pool
{
	/* pool informations */
	char name[HMA_POOL_LEN];
	unsigned long start;
	int size;

	/* memory chunk list */
	spinlock_t lock;
	struct hma_chunk using_root;
	struct hma_chunk free_root;

	/* for procfs interface */
	struct proc_dir_entry *dir_entry;
	struct hma_chunk **chunks;
	int chunks_num;

	/* for time optimization */
	u64 alloc_time, alloc_maxtime;
	u64 free_time, free_maxtime;
	int alloc_num, free_num;

	struct hma_pool *next;
};

static struct proc_dir_entry *hma_proc_entry;
static struct hma_pool hma_pool_root;

static struct hma_pool *hma_pool_search (const char *poolname)
{
	struct hma_pool *pool;

	pool = &hma_pool_root;
	while (pool->next != NULL && strcmp (pool->next->name, poolname))
		pool = pool->next;

	return pool->next;
}

static struct hma_chunk *search_best_match (struct hma_pool *pool,
		int size, int align, unsigned long prefered)
{
	struct hma_chunk *this, *best;
	int best_size;

	this = &pool->free_root;
	best = NULL;
	best_size = INT_MAX;

	while (this->next != NULL)
	{
		this = this->next;

		if (atomic_read (&this->users) == 0)
		{
			unsigned long aligned_addr;
			int free_size;

			if (prefered)
			{
				if (this->addr <= prefered &&
						prefered <= this->addr + this->size)
				{
					best = this;
					break;
				}
			}
			else
			{
				aligned_addr = (this->addr + align - 1) & ~(align-1);
				free_size = this->size - (aligned_addr - this->addr);

				if (
						free_size >= size &&
						best_size > free_size
				   )
				{
					best = this;
					best_size = free_size;
				}
			}
		}
	}

	return best;
}

static struct hma_chunk *_hma_alloc (struct hma_pool *pool,
		int size, int align, unsigned long prefered, const char *user)
{
	struct hma_chunk *best, *before, *after;
	unsigned long addr;
	int size_before, size_after;
	unsigned long flag;
	u64 time_start = 0;

	if (!prefered && (align&(align-1)) != 0)
	{
		error ("wrong align value\n");
		return NULL;
	}

	/* lock mutex */
	spin_lock_irqsave (&pool->lock, flag);

	if (enable_profiling)
		time_start = local_clock ();

	/* search the best matching free chunk */
	best = search_best_match (pool, size, align, prefered);
	if (best == NULL)
	{
		error ("no best match\n");
		goto nomem;
	}

	if (!prefered)
		addr = (best->addr + align - 1) & ~(align-1);
	else
		addr = prefered;
	size_before = addr - best->addr;
	size_after = best->size - size_before - size;

	/* 찾은 메모리 덩어리에서 주소 align을 맞춘 앞쪽과 뒷쪽 메모리의 조각에
	 * 대한 메모리 조각을 다시 등록한다. */
	if (size_before > 0)
	{
		before = kcalloc (1, sizeof (*before), GFP_ATOMIC);
		if (before == NULL)
			goto nomem;
		before->addr = best->addr;
		before->size = size_before;

		before->prev = best->prev;
		if (best->prev != NULL)
			best->prev->next = before;
	}
	else
		before = best->prev;

	/* 뒷쪽 메모리 조각을 등록한다 */
	if (size_after > 0)
	{
		after = kcalloc (1, sizeof (*after), GFP_ATOMIC);
		if (after == NULL)
			goto nomem;
		after->addr = addr + size;
		after->size = size_after;

		after->next = best->next;
		if (best->next != NULL)
			best->next->prev = after;
	}
	else
		after = best->next;

	/* 메모리 조각의 주소와 크기를 사용자 요청 값으로 변경한다. */
	best->addr = addr;
	best->size = size;
	atomic_inc (&best->users);
	if (best->user)
	{
		kfree (best->user);
		best->user = NULL;
	}
	if (user)
	{
		best->user = kmalloc (strlen(user)+1, GFP_ATOMIC);
		if (best->user)
			strcpy (best->user, user);
	}

	/* linked list를 갱신한다.
	 * best chunk를 using_root list로 옮긴다 */
	before->next = after;
	if (after)
		after->prev = before;

	best->next = pool->using_root.next;
	if (best->next)
		best->next->prev = best;
	pool->using_root.next = best;
	best->prev = &pool->using_root;

	if (enable_profiling)
	{
		time_start = local_clock() - time_start;

		pool->alloc_time += time_start;
		pool->alloc_num ++;

		if (pool->alloc_maxtime < time_start)
			pool->alloc_maxtime = time_start;
	}

	spin_unlock_irqrestore (&pool->lock, flag);

	return best;

nomem:
	if (dump_status_on_no_memory)
	{
		struct hma_chunk *chunk;

		chunk = pool->using_root.next;
		printk ("pool, %s, using list\n", pool->name);
		while (chunk)
		{
			printk ("%08lx - %07x, %s\n",
					chunk->addr, chunk->size, chunk->user);

			chunk = chunk->next;
		}

		chunk = pool->free_root.next;
		printk ("pool, %s, free list\n", pool->name);
		while (chunk)
		{
			printk ("%08lx - %07x, %s\n",
					chunk->addr, chunk->size, chunk->user);

			chunk = chunk->next;
		}

		printk ("done.\n");
	}
	spin_unlock_irqrestore (&pool->lock, flag);
	return NULL;
}

static int dump_open (struct inode *inode, struct file *file)
{
	struct hma_chunk *chunk;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;
	dp = PDE (inode);
	chunk = dp->data;

#else
	chunk = PDE_DATA(inode);
#endif
	debug ("hma dump_open: %08lx, %s\n",
			chunk->addr,
			chunk->user?chunk->user:"<null>");

	file->private_data = chunk;
	chunk->virt = ioremap (chunk->addr, chunk->size);

	return 0;
}

static ssize_t dump_read (struct file *file, char __user *buf, size_t len, loff_t *off)
{
	ssize_t read;
	long ret;
	struct hma_chunk *chunk = file->private_data;

	if (*off > chunk->size)
		return -EFAULT;

	if (*off + len >= chunk->size)
		len = chunk->size - *off;

	ret = copy_to_user (buf, chunk->virt+*off, len);
	read = len - ret;

	*off += read;
	return read;
}

static loff_t dump_llseek (struct file *file, loff_t off, int orig)
{
	loff_t ret;
	struct hma_chunk *chunk = file->private_data;

	switch (orig)
	{
	case SEEK_CUR:
		off += file->f_pos;
	case SEEK_SET:
		if (off > chunk->size)
			return -EINVAL;

		file->f_pos = off;
		ret = file->f_pos;
		break;
	default:
		return -EINVAL;
	}

	return off;
}

static int dump_release (struct inode *inode, struct file *file)
{
	struct hma_chunk *chunk = file->private_data;

	iounmap (chunk->virt);
	chunk->virt = NULL;

	return 0;
}

static struct file_operations dump_fops =
{
	.open = dump_open,
	.read = dump_read,
	.llseek = dump_llseek,
	.release = dump_release,
};


static unsigned long alloc (const char *poolname,
		int size, int align, unsigned long prefered, const char *user)
{
	struct hma_pool *pool;
	struct hma_chunk *chunk;

	debug ("0x%08x(0x%08x) on %s by %s, prefered %08lx\n",
			size, align,
			poolname==NULL?"<null>":poolname,
			user==NULL?"<null>":user,
			prefered
			);

	if (poolname == NULL || *poolname == 0)
	{
		pool = &hma_pool_root;

		while (pool->next != NULL)
		{
			pool = pool->next;

			chunk = _hma_alloc (pool, size, align, prefered, user);
			if (chunk != NULL)
				goto allocated;
		}

		pool = NULL;
	}
	else
	{
		/* search memory pool */
		pool = hma_pool_search (poolname);
		if (pool == NULL)
		{
			error ("no such a pool name. %s\n",
					poolname);
			return 0;
		}

		chunk = _hma_alloc (pool, size, align, prefered, user);
		if (chunk != NULL)
			goto allocated;
	}

	error ("no memory for %s. "
			"size:0x%x, align:0x%x, user:%s, pool:%s\n",
			user?user:"<null>",
			size, align, user, pool->name
	      );
	if (dump_stack_on_no_memory)
		dump_stack ();

	return 0;


allocated:
	if (enable_dump && pool->dir_entry)
	{
		chunk->dumpname = kmalloc (sizeof (unsigned long)*2 + 1,
				GFP_ATOMIC);

		if (chunk->dumpname)
		{
			sprintf (chunk->dumpname, "%08lx", chunk->addr);
			proc_create_data (chunk->dumpname, 0440,
					pool->dir_entry, &dump_fops, chunk);
		}
	}

	debug ("allocated 0x%08lx\n", chunk->addr);
	return chunk->addr;
}


/**
 * \ingroup hma
 * \brief 메모리 할당을 한다.
 *
 * \a poolname 에 해당하는 memory pool 이름에서 \a prefered 주소의 위치에서 \a
 * size 크기의 메모리를 할당한다.  hma_alloc_prefered() 함수와 같은 기능을 하나
 * 메모리 사용자 이름을 \a user 를 통해 정해줄 수 있다.
 *
 * 사용자 이름으로 정하는 \a user 는 디버깅 용도로 사용하게 된다. 사용자 이름을
 * 특별히 정하지 않으려면 hma_alloc() 함수를 사용할 수 있다.
 *
 * multithread에 안전하다.
 *
 * \param poolname	memory pool name
 * \param size		memory size
 * \param prefered	alloc address for specific area
 * \param user		memory user. used for debuging.
 */
unsigned long hma_alloc_prefered_user (const char *poolname,
		int size, unsigned long prefered, const char *user)
{
	return alloc (poolname, size, 0, prefered, user);
}
EXPORT_SYMBOL (hma_alloc_prefered_user);


/**
 * \ingroup hma
 * \brief 메모리 할당을 한다.
 *
 * \a poolname 에 해당하는 memory pool 이름에서 \a prefered 주소의 위치에서 \a
 * size 크기의 메모리를 할당한다.
 *
 * multithread에 안전하다.
 *
 * \param poolname	memory pool name
 * \param size		memory size
 * \param align		memory align. ex. 0x1000 for 12bit aligned.
 */
unsigned long hma_alloc_prefered (const char *poolname,
		int size, unsigned long prefered)
{
	char comm[TASK_COMM_LEN+1];

	get_task_comm (comm, current);
	comm[TASK_COMM_LEN] = 0;

	return alloc (poolname, size, 0, prefered, comm);
}
EXPORT_SYMBOL (hma_alloc_prefered);


/**
 * \ingroup hma
 * \brief 메모리 할당을 한다.
 *
 * \a poolname 에 해당하는 memory pool 이름에서 \a size 크기의 메모리를
 * 할당한다. 메모리의 시작주소는 \a align 에 align 시킨다. hma_alloc() 함수와
 * 같은 기능을 하나 메모리 사용자 이름을 \a user 를 통해 정해줄 수 있다.
 *
 * 사용자 이름으로 정하는 \a user 는 디버깅 용도로 사용하게 된다. 사용자 이름을
 * 특별히 정하지 않으려면 hma_alloc() 함수를 사용할 수 있다.
 *
 * multithread에 안전하다.
 *
 * \param poolname	memory pool name
 * \param size		memory size
 * \param align		memory align. 0x1000 for 12bit aligned.
 * \param user		memory user. used for debuging.
 */
unsigned long hma_alloc_user (const char *poolname,
		int size, int align, const char *user)
{
	return alloc (poolname, size, align, 0, user);
}
EXPORT_SYMBOL (hma_alloc_user);

/**
 * \ingroup hma
 * \brief 메모리 할당을 한다.
 *
 * \a poolname에 해당하는 memory pool 이름에서 \a size 크기의 메모리를
 * 할당한다. 메모리의 시작주소는 \a align 에 align 시킨다.
 *
 * multithread에 안전하다.
 *
 * \param poolname	memory pool name
 * \param size		memory size
 * \param align		memory align. ex. 0x1000 for 12bit aligned.
 */
unsigned long hma_alloc (const char *poolname,
		int size, int align)
{
	char comm[TASK_COMM_LEN+1];

	get_task_comm (comm, current);
	comm[TASK_COMM_LEN] = 0;

	return alloc (poolname, size, align, 0, comm);
}
EXPORT_SYMBOL (hma_alloc);

static struct hma_chunk* get_chunk_locked (struct hma_chunk *chunk)
{
	atomic_inc (&chunk->users);

	return chunk;
}

static void put_chunk_locked (struct hma_pool *pool, struct hma_chunk *chunk)
{
	struct hma_chunk *t, *prev, *next;

	if (!atomic_dec_and_test (&chunk->users))
		return;

	debug ("free %p, %08lx\n", chunk, chunk->addr );

	/* chunk 를 using_root 에서 free_root 로 옮긴다 */
	chunk->prev->next = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;

	t = &pool->free_root;
	while (t->next)
	{
		struct hma_chunk *n;
		n = t->next;
		if (
				t->addr+t->size <= chunk->addr &&
				chunk->addr+chunk->size <= n->addr
		   )
			break;

		t = n;
	}

	if (t->next)
		t->next->prev = chunk;
	chunk->next = t->next;
	t->next = chunk;
	chunk->prev = t;

	prev = chunk->prev;
	next = chunk->next;

	if (
			prev->addr+prev->size == chunk->addr &&
			atomic_read (&prev->users) == 0
	   )
	{
		debug ("merge with previous one\n");

		prev->size += chunk->size;
		prev->next = chunk->next;
		if (chunk->next != NULL)
			chunk->next->prev = prev;
		if (chunk->user)
			kfree (chunk->user);

		kfree (chunk);

		chunk = prev;
	}

	if (
			next != NULL &&
			chunk->addr+chunk->size == next->addr &&
			atomic_read (&next->users) == 0
	   )
	{
		debug ("merge with next one\n");

		chunk->size += next->size;
		chunk->next = next->next;
		if (next->next != NULL)
			next->next->prev = chunk;
		if (next->user)
			kfree (next->user);

		kfree (next);
	}
}

static int _hma_free (struct hma_pool *pool, unsigned long addr)
{
	struct hma_chunk *this, *chunk;
	unsigned long flag;
	char *dumpname;
	u64 time_start = 0;

	spin_lock_irqsave (&pool->lock, flag);

	if (enable_profiling)
		time_start = local_clock ();

	this = &pool->using_root;
	chunk = NULL;

	/* search the chunk */
	while (this->next != NULL)
	{
		this = this->next;

		if (this->addr == addr && atomic_read (&this->users) != 0)
		{
			chunk = this;
			break;
		}
	}

	if (chunk == NULL)
	{
		error ("no such a chunk for addr %lx\n", addr);
		goto failed;
	}

	dumpname = chunk->dumpname;
	chunk->dumpname = NULL;
	put_chunk_locked (pool, chunk);

	if (enable_profiling)
	{
		time_start = local_clock() - time_start;

		pool->free_time += time_start;
		pool->free_num ++;

		if (pool->free_maxtime < time_start)
			pool->free_maxtime = time_start;
	}

	spin_unlock_irqrestore (&pool->lock, flag);

	if (dumpname)
	{
		remove_proc_entry (dumpname, pool->dir_entry);

		kfree (dumpname);
	}

	return 0;

failed:
	spin_unlock_irqrestore (&pool->lock, flag);
	return -1;
}

/**
 * \ingroup hma
 * \brief 사용 후 메모리를 반환한다.
 *
 * hma_alloc() 혹은 hma_alloc_user() 함수에 의해 할당받은 메모리를 반환한다.
 * 반환 하고자 하는 메모리의 memory pool name을 모르고 있다면 \a poolname 을
 * NULL로 할 수 있다.
 *
 * multithread에 안전하다.
 *
 * \param poolname	memory pool name
 * \param addr		memory start address
 */
void hma_free (const char *poolname, unsigned long addr)
{
	struct hma_pool *pool;

	debug ("0x%08lx on %s\n",
			addr, poolname==NULL?"<null>":poolname);

	if (poolname == NULL)
	{
		pool = &hma_pool_root;

		while (pool->next != NULL)
		{
			pool = pool->next;

			if (_hma_free (pool, addr) == 0)
				return;
		}
	}
	else
	{
		/* search memory pool */
		pool = hma_pool_search (poolname);
		if (pool == NULL)
		{
			error ("no such a pool name. %s\n",
					poolname);
			return;
		}

		if (_hma_free (pool, addr) == 0)
			return;
	}

	error ("unknown address %lx\n", addr);
	dump_stack ();
	return;
}

EXPORT_SYMBOL (hma_free);

/* proc filesystem interface to dump the hma memory status */
static void *hma_seq_start (struct seq_file *m, loff_t *pos)
{
	struct hma_pool *pool;

	pool = m->private;

	if (pool->chunks_num <= (*pos))
		return NULL;

	return pool->chunks[(*pos)];
}

static void hma_seq_stop (struct seq_file *m, void *v)
{
	/* do nothing */
}

static void *hma_seq_next (struct seq_file *m, void *v, loff_t *pos)
{
	struct hma_pool *pool;

	(*pos) ++;

	pool = m->private;
	if (pool->chunks_num <= (*pos))
		return NULL;

	return pool->chunks[(*pos)];
}

static int hma_seq_show (struct seq_file *m, void *v)
{
	struct hma_chunk *this;
	int users;

	this = v;

	users = atomic_read (&this->users);

	seq_printf (m, "0x%lx\t0x%07x\t%sused\t%s\n",
			this->addr, this->size,
			users>1?"":"not",
			this->user?this->user:"<null>");

	return 0;
}

static struct seq_operations hma_seq_ops =
{
	.start = hma_seq_start,
	.stop = hma_seq_stop,
	.next = hma_seq_next,
	.show = hma_seq_show,
};

static int hma_proc_open (struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	struct hma_pool *pool;
	int ret;
	unsigned long flag;
	int num;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;
	dp = PDE (inode);
	pool = dp->data;
#else
	pool = PDE_DATA(inode);
#endif

	ret = seq_open (file, &hma_seq_ops);
	if (ret < 0)
		return ret;

	seq = file->private_data;
	seq->private = pool;

	/* make chunk list */
	spin_lock_irqsave (&pool->lock, flag);
	if (pool->chunks)
	{
		error ("already opened?\n");
		ret = -EBUSY;
	}
	else
	{
		struct hma_chunk *c;

		/* put all chunks to the using_root */
		for (num=0,c=&pool->using_root; c->next; c=c->next, num++)
			get_chunk_locked (c->next);

		debug ("%d chunks on using_root\n", num);

		c->next = pool->free_root.next;
		if (c->next)
			c->next->prev = c;
		pool->free_root.next = NULL;

		for (; c->next; c=c->next, num++)
			get_chunk_locked (c->next);

		debug ("%d chunks for all\n", num);

		pool->chunks = kmalloc (num*sizeof (struct hma_chunk*), GFP_ATOMIC);
		if (pool->chunks)
		{
			for (num=0,c=&pool->using_root; c->next; c=c->next, num++)
				pool->chunks[num] = c->next;
		}
		else
		{
			error ("Oops\n");
			ret = -ENOMEM;
		}
	}
	spin_unlock_irqrestore (&pool->lock, flag);

	if (ret < 0)
	{
		seq_release (inode, file);
		return ret;
	}

	pool->chunks_num = num;

	return ret;
}

static int hma_proc_release (struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	struct hma_pool *pool;
	int ret;
	int a;
	unsigned long flag;

	seq = file->private_data;
	pool = seq->private;

	/* release chunk list */
	spin_lock_irqsave (&pool->lock, flag);
	for (a=0; a<pool->chunks_num; a++)
		put_chunk_locked (pool, pool->chunks[a]);
	kfree (pool->chunks);
	pool->chunks = NULL;
	pool->chunks_num = 0;

	if (debug_level > 0)
	{
		struct hma_chunk *t;

		debug ("using list...\n");
		for (t=&pool->using_root; t->next; t=t->next)
			debug ("%p, %08lx, next %p, prev %p\n",
					t->next,
					t->next->addr,
					t->next->next,
					t->next->prev);

		debug ("free list...\n");
		for (t=&pool->free_root; t->next; t=t->next)
			debug ("%p, %08lx, next %p, prev %p\n",
					t->next,
					t->next->addr,
					t->next->next,
					t->next->prev);
	}
	spin_unlock_irqrestore (&pool->lock, flag);

	ret = seq_release (inode, file);

	return ret;
}

static struct file_operations hma_proc_fops =
{
	.open = hma_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = hma_proc_release,
};

static int debug_show (struct seq_file *m, void *data)
{
	struct hma_pool *pool = m->private;
	unsigned long flag;

#define seq_print_hma(v,f)	seq_printf (m, #v"=%"#f"\n", pool->v)
	spin_lock_irqsave (&pool->lock, flag);
	seq_print_hma (name, s);
	seq_print_hma (start, lx);
	seq_print_hma (size, x);
	seq_print_hma (alloc_maxtime, lld);
	seq_print_hma (alloc_time, lld);
	seq_print_hma (alloc_num, d);
	seq_print_hma (free_maxtime, lld);
	seq_print_hma (free_time, lld);
	seq_print_hma (free_num, d);
	spin_unlock_irqrestore (&pool->lock, flag);

	return 0;
}

static int hma_debug_open (struct inode *inode, struct file *file)
{
	int ret;
	struct hma_pool *pool;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;
	dp = PDE (inode);
	pool = dp->data;

#else
	pool = PDE_DATA (inode);
#endif
	ret = single_open (file, debug_show, pool);

	return ret;
}

static int hma_debug_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations hma_debug_fops =
{
	.open = hma_debug_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = hma_debug_release,
};


/**
 * \ingroup hma
 * \brief memory pool의 위치, 크기를 확인한다.
 */
int hma_pool_info (const char *name, unsigned long *start, int *size)
{
	struct hma_pool *p;

	p = hma_pool_search (name);
	if (!p)
	{
		error ("no such pool. %s\n", name);
		return -1;
	}

	if (start)
		*start = p->start;
	if (size)
		*size = p->size;

	return 0;
}
EXPORT_SYMBOL (hma_pool_info);

/**
 * \ingroup hma
 * \brief memory pool을 등록한다.
 *
 * memory pool을 등록한다. memory pool은 여러개가 등록될 수 있으며, hardware
 * memory를 사용하고자 하는 곳에서 원하는 memory pool에서 hma_alloc() 함수를
 * 이용해 memory를 할당한다.
 *
 * 보통 플랫폼 초기화 과정이나 driver module 초기화 과정(insmod)에서 사용하며,
 * multithread에 대해 안전하지 않다.
 *
 * 등록된 memory pool은 hma_pool_unregister() 함수를 이용해 등록을 취소한다.
 *
 * \param name		memory pool name
 * \param start		start address of the memory pool
 * \param size		size of the memory pool
 */
int hma_pool_register (const char *name, unsigned long start, int size)
{
	struct hma_pool *pool;
	struct hma_chunk *chunk;

	/* check name length */
	if (strlen (name)+1 >= HMA_POOL_LEN)
	{
		error ("too long name, %s\n", name);
		return -EINVAL;
	}

	/* allocate the pool structure and fill the parameter */
	pool = kcalloc (1, sizeof(*pool), GFP_KERNEL);
	if (pool == NULL)
		return -ENOMEM;
	strcpy (pool->name, name);
	pool->start = start;
	pool->size = size;
	spin_lock_init (&pool->lock);
	pool->free_root.next = chunk =
		(struct hma_chunk*)kcalloc (1, sizeof(*chunk), GFP_KERNEL);
	chunk->addr = start;
	chunk->size = size;
	chunk->prev = &pool->free_root;

	/* add to the root */
	pool->next = hma_pool_root.next;
	hma_pool_root.next = pool;

	/* add proc entry */
	if (hma_proc_entry)
	{
		pool->dir_entry = proc_mkdir (name, hma_proc_entry);
		proc_create_data ("status", 0440,
				pool->dir_entry, &hma_proc_fops, pool);
		proc_create_data ("debug", 0440,
				pool->dir_entry, &hma_debug_fops, pool);
	}
	else
		pool->dir_entry = NULL;

	return 0;
}

EXPORT_SYMBOL (hma_pool_register);

static void hma_pool_free (struct hma_pool *pool)
{
	while (pool->using_root.next != NULL)
		_hma_free (pool, pool->using_root.next->addr);

	kfree (pool->free_root.next);
}

/**
 * \ingroup hma
 * \brief memory pool을 더이상 사용하지 않게 등록을 취소한다.
 *
 * hma_pool_register() 함수를 이용해 등록한 memory pool을 더이상 사용하지
 * 않도록 한다. 현재 해당 memory pool에서 hma_alloc() 함수를 이용해 할당
 * 받은 메모리에 대해서는 더이상 메모리 관리의 책임을 지지 않는다.
 *
 * 보통 kernel driver module을 kernel에서 내릴 때(rmmod) 호출되며,
 * multithread에 대해 보호를 하지 않는다.
 *
 * \param name		memory pool name
 */
void hma_pool_unregister (const char *name)
{
	struct hma_pool *pool;

	pool = &hma_pool_root;
	while (pool->next != NULL)
	{
		if (!strcmp (pool->next->name, name))
		{
			struct hma_pool *one;

			one = pool->next;
			pool->next = one->next;

			if (hma_proc_entry)
			{
				remove_proc_entry ("status",
						one->dir_entry);
				remove_proc_entry ("debug",
						one->dir_entry);
				remove_proc_entry (name, hma_proc_entry);
			}

			hma_pool_free (one);
			kfree (one);

			return;
		}

		pool = pool->next;
	}

	error ("name, %s, not found\n", name);
}

EXPORT_SYMBOL (hma_pool_unregister);

/* something like pool="<poolname>:<poolbase>:<poolsize>,vdec:0x50000000:128M,jpeg:0x60000000:0x1000000" */
static char *pool;
module_param (pool, charp, 0440);

static char * get_a_pool (char *p,
		char **_name, unsigned long *_addr, unsigned int *_size)
{
	char *now, *name;
	unsigned long addr;
	unsigned int size;

	debug ("pool: %s\n", pool);

	if (*p == 0)
	{
		debug ("end.\n");
		return NULL;
	}

	now = p;

	/* name */
	p = strchr (now, ':');
	if (!p)
	{
		error ("wrong pool string. now:%s\n", now);
		return NULL;
	}

	name = kmalloc ((p-now)+1, GFP_KERNEL);
	if (!name)
	{
		error ("Oops\n");
		return NULL;
	}

	strncpy (name, now, p-now);
	name[p-now] = 0;
	debug ("name %s\n", name);

	/* address */
	now = p+1;
	addr = memparse (now, &p);
	if (*p != ':')
	{
		error ("wrong addres. %s\n", now);
		kfree (name);
		return NULL;
	}

	/* size */
	now = p+1;
	size = memparse (now, &p);
	if (!size)
	{
		error ("wrong size. %s\n", now);
		kfree (name);
		return NULL;
	}
	if (*p != 0 && *p != ',')
	{
		error ("wrong size. %s\n", now);
		kfree (name);
		return NULL;
	}

	info ("got pool %s from 0x%lx(0x%x)\n",
			name, addr, size);
	*_name = name;
	*_addr = addr;
	*_size = size;

	if (*p == 0)
		return p;
	else
		return p+1;
}

int hma_init (void)
{
	char *now, *name;
	unsigned long addr;
	unsigned int size;

	hma_proc_entry = proc_mkdir ("hma", NULL);

	/* parse *pool string */
	now = pool;
	while (now && *now)
	{
		name = NULL;
		addr = 0;
		size = 0;

		now = get_a_pool (now, &name, &addr, &size);
		if (name)
		{
			hma_pool_register (name, addr, size);
			kfree (name);
		}
		else
		{
			while (hma_pool_root.next)
				hma_pool_unregister (hma_pool_root.next->name);

			remove_proc_entry ("hma", NULL);
			return -EINVAL;
		}
	}

	return 0;
}

void hma_exit (void)
{
	while (hma_pool_root.next)
		hma_pool_unregister (hma_pool_root.next->name);

	remove_proc_entry ("hma", NULL);
}

