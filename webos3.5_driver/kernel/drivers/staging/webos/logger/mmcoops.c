/* MMC oops/panic logger */

#include <linux/delay.h>
#include <linux/kmsg_dump.h>
#include <linux/sched.h>
#include <linux/lgemmc.h>
#include "ll_mmc.h"

#undef SANITY_CHECK_PARTITION
#define DRV_NAME "mmcoops"

#define BUFSIZE 1024
#define BLOCK_SIZE 512
#define MAX_LOG_CNT 3
#define KMSG_DUMP_MAGIC 0xBE1953D6

enum dump_reason {
	DUMP_WDT,
	DUMP_PANIC,
};

static unsigned long long save_addr;
static unsigned long long end_offset;
static unsigned long long dump_offset;
static char dump_part_name[16];

struct dump_hdr {
	u32 magic;
	int type;
	struct timespec timestamp;
};

static int get_partition_info(unsigned long long *start,
			      unsigned long long *end)
{
	int part;
	int psize;

	part = lgemmc_get_partnum(dump_part_name);
	if (part < 0)
		return -1;
	*start = lgemmc_get_partoffset(part);
	*end = *start;
	*start += dump_offset;
	*start = roundup(*start, BLOCK_SIZE);
	psize = lgemmc_get_partsize(part);
	*end += (u64)psize;
	return 0;
}

static void dump_log_buf(struct kmsg_dumper *dumper,
			 enum kmsg_dump_reason reason)
{
	int len = 0;
	int offset = 0;
	int need_to_write = 0;
	int last = 0;
	u32 left_len = 0;
	static char buf[BUFSIZE] = {0, };
	static char text[BUFSIZE];
	static int inited;
	s32 ret;

	if (!inited) {
		struct dump_hdr hdr;

		if (get_partition_info(&save_addr, &end_offset))
			return;

		if (reason == KMSG_DUMP_PANIC)
			hdr.type = DUMP_PANIC;
		else
			hdr.type = DUMP_WDT;

		ret = ll_mmc_init();
		if (ret) {
			pr_err(DRV_NAME ":mmc initialize error\n");
			return;
		}
#ifdef SANITY_CHECK_PARTITION
		ret = ll_mmc_read((u8 *)&hdr, save_addr,
					sizeof(struct dump_hdr));
		if (ret) {
			pr_err(DRV_NAME ":mmc read error\n");
			return;
		}

		if (hdr.magic != KMSG_DUMP_MAGIC)
			return;
		else
			return;
#endif
		kmsg_dump_rewind(dumper);

		if (__getnstimeofday(&hdr.timestamp)) {
			hdr.timestamp.tv_sec = 0;
			hdr.timestamp.tv_nsec = 0;
		}
		hdr.magic = KMSG_DUMP_MAGIC;
		memcpy(buf, &hdr, sizeof(struct dump_hdr));
		offset += sizeof(struct dump_hdr);

		inited = 1;
	}

	while (1) {
		if (!kmsg_dump_get_line(dumper, false, text,
					sizeof(text), &len)) {
			if (offset) {
				last = 1;
				need_to_write = 1;
				goto WRITE;
			}
			break;
		}

		if ((offset + len) >= BUFSIZE) {
			left_len = offset + len - BUFSIZE;
			memcpy(buf + offset, text, len - left_len);
			offset = 0;
			need_to_write = 1;
		} else {
			memcpy(buf + offset, text, len);
			offset += len;
		}

WRITE:
		if (need_to_write) {
			int length = BUFSIZE;

			need_to_write = 0;
			if ((save_addr + BUFSIZE) >= end_offset) {
				length = (int)(end_offset - save_addr);
				if (!length)
					break;
				length = rounddown(length, BLOCK_SIZE);
			}

			ret = ll_mmc_write(buf, save_addr, length);

			if (ret) {
				pr_err(DRV_NAME ":mmc write error\n");
				return;
			}

			save_addr += BUFSIZE;
			if (left_len) {
				memset(buf, 0, BUFSIZE);
				memcpy(buf, text + (len - left_len), left_len);
				offset += left_len;
			}
			if (last)
				break;
		}
	}
}

static void raw_kmsg_emmc_dump(struct kmsg_dumper *dumper,
			       enum kmsg_dump_reason reason)
{
	static atomic_t log_cnt = ATOMIC_INIT(0);
	unsigned long flags;
	static DEFINE_SPINLOCK(dump_lock);

	if (*dump_part_name == '\0')
		return;

	if (!ll_mmc_ready()) {
		pr_warn("No ops for mmcoops\n");
		return;
	}

	switch (reason) {
	case KMSG_DUMP_PANIC:
	case KMSG_DUMP_UNDEF:
		if (atomic_read(&log_cnt) < MAX_LOG_CNT) {
			if (!spin_trylock_irqsave(&dump_lock, flags))
				break;
			dump_log_buf(dumper, reason);
			spin_unlock_irqrestore(&dump_lock, flags);
			atomic_inc(&log_cnt);
		}
		break;
	default:
		break;
	}
}

static struct kmsg_dumper klog_dumper = {
	.dump = raw_kmsg_emmc_dump,
};

/* i.e. dump@1M : [partition name]@[offset] */
static int __init mmcoops_init(char *str)
{
	char *ptr = str;
	int len;

	if (!ptr)
		return 1;
	ptr = strchr(ptr, '@');

	if (ptr) {
		len = ptr++ - str;
		dump_offset = memparse(ptr, &ptr);
	} else {
		len = strlen(str);
	}

	if (len > (sizeof(dump_part_name) - 1))
		len = sizeof(dump_part_name) - 1;

	strncpy(dump_part_name, str, len);
	dump_part_name[len] = '\0';

	kmsg_dump_register(&klog_dumper);

	pr_info(DRV_NAME ":Oops/panic log will be stored at "
				"%s partition (offset: 0x%llx)\n",
				dump_part_name, dump_offset);
	return 0;
}
__setup("mmcoops=", mmcoops_init);
