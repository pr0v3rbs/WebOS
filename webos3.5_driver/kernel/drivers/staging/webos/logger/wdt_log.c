/*
 * drivers/staging/webos/logger/wdt_detect.c
 */

#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/of_gpio.h>
#include <linux/lgemmc.h>
#include <linux/kmsg_dump.h>
#include "ll_mmc.h"
#include <linux/vmalloc.h>

#define BUF_SIZE (PAGE_SIZE * 4)
#define FULL_PATH_SIZE 1024
#define FILE_NAME_SIZE 16
#define MAGIC_NUM 0xFA14EB59
#define MAX_LOG_SIZE 0x89C000 /* 8.6MM */
#define TMP_LOG_DIR "/var/log/"
#define CRASH_LOG_DIR "/var/log/reports/librdx/"
#define MOD_NAME "wdtlog"

enum {
	LOG_CRASH = 1,
	LOG_KERNEL,
	LOG_PM,
	LOG_LEGACY,
	LOG_BOOT,
	LOG_MAX,
};

static unsigned long long save_addr;
static unsigned long long dump_offset;
static char dump_part_name[16];
static char read_buf[BUF_SIZE];
static u32 total_log_size;
static bool wdt_init;

struct wdt_log_header {
	unsigned long magic;
	char name[FILE_NAME_SIZE];
	size_t size;
	ulong written_size;
	struct timespec timestamp;
	char reserved[8];
};

struct dir_callback {
	struct dir_context ctx;
	int log_type;
};

static int get_partition_info(char *part_name, unsigned long long offset,
		unsigned long long *start)
{
	int part;

	part = lgemmc_get_partnum(part_name);
	if (part < 0)
		return -1;
	*start = lgemmc_get_partoffset(part);
	*start += offset;
	*start = roundup(*start, BLOCK_SIZE);
	return 0;
}



static int saveable_file(const char *name, int type)
{
	if (!strcmp(name, ".") || !strcmp(name, ".."))
		return -1;
	switch (type) {
	case LOG_LEGACY:
		if (!strcmp(name, "legacy-log") ||
				!strcmp(name, "legacy-log.0.gz"))
			return 0;

		break;
	case LOG_PM:
		if (!strcmp(name, "messages") ||
				!strcmp(name, "messages.0.gz"))
			return 0;

		break;
	case LOG_BOOT:
		if (!strcmp(name, "bootd.log"))
			return 0;

		break;
	case LOG_CRASH:
		return 0;
	}

	return -1;
}

static int copy_log_raw_drv(struct file *src_file, size_t size,
					struct wdt_log_header *wlh)
{
	int read_bytes;
	int len;
	loff_t offset = 0;

	if (size % BLOCK_SIZE)
		wlh->written_size = size + BLOCK_SIZE - (size % BLOCK_SIZE);

	total_log_size += wlh->written_size;
	memcpy(read_buf, wlh, sizeof(struct wdt_log_header));


	while (size > 0) {
		if (size > BUF_SIZE)
			len = BUF_SIZE;
		else
			len = size;

		if (offset) {
			read_bytes = kernel_read(src_file, offset, read_buf,
									len);
		} else {
			if ((len + sizeof(struct wdt_log_header)) <= BUF_SIZE)
				len += sizeof(struct wdt_log_header);
			else
				len -= sizeof(struct wdt_log_header);

			read_bytes = kernel_read(src_file, offset,
				(read_buf + sizeof(struct wdt_log_header)),
				len);

		}

		if (read_bytes) {

			if (!offset) {
				offset += (loff_t)read_bytes;
				read_bytes += sizeof(struct wdt_log_header);
			} else {
				offset += (loff_t)BUF_SIZE;
			}

			if (read_bytes % BLOCK_SIZE)
				read_bytes = roundup(read_bytes, BLOCK_SIZE);

			if (ll_mmc_write(read_buf, save_addr,
						read_bytes) < 0) {
				pr_err(MOD_NAME ":mmc write error\n");
				break;
			}

			save_addr += read_bytes;
		} else
			break;

		size -= len;
	}

	return 0;
}

static int iterate_copy_log(struct dir_context *ctx, const char *name,
		int namlen, loff_t off, u64 ino, unsigned int d_type)
{
	struct dir_callback *dc = container_of(ctx, struct dir_callback, ctx);

	if (!saveable_file(name, dc->log_type)) {

		struct file *src_file;
		static char src_name[FULL_PATH_SIZE];
		struct kstat ks;
		size_t size;
		struct wdt_log_header wlh;

		if ((dc->log_type == LOG_LEGACY) || (dc->log_type == LOG_PM)
				|| (dc->log_type == LOG_BOOT))
			snprintf(src_name, FULL_PATH_SIZE - 1,
						TMP_LOG_DIR"%s", name);
		else
			snprintf(src_name, FULL_PATH_SIZE - 1,
						CRASH_LOG_DIR"%s", name);

		src_file = filp_open(src_name, O_RDONLY, 0);
		if (unlikely(IS_ERR(src_file)))
			return -1;

		vfs_getattr_nosec(&src_file->f_path, &ks);
		size = (size_t)ks.size;

		if ((total_log_size + size) >= MAX_LOG_SIZE)
			goto OUT;

		wlh.magic = MAGIC_NUM;
		snprintf(wlh.name, FILE_NAME_SIZE - 1, "%s", name);

		if (__getnstimeofday(&wlh.timestamp)) {
			wlh.timestamp.tv_sec = 0;
			wlh.timestamp.tv_nsec = 0;
		}

		wlh.size = size;
		wlh.written_size = size;
		copy_log_raw_drv(src_file, size, &wlh);
OUT:
		filp_close(src_file, NULL);
	}

	return 0;
}

static void log_file_copy(int type)
{
	struct file *log_dir_file;
	char *path = NULL;
	struct dir_callback callback = {
		.ctx.actor = iterate_copy_log,
		.log_type = type
	};

	if (type == LOG_CRASH)
		path = CRASH_LOG_DIR;
	else if (type == LOG_KERNEL)
		kmsg_dump(KMSG_DUMP_UNDEF);
	else if (type == LOG_LEGACY || type == LOG_PM || type == LOG_BOOT)
		path = TMP_LOG_DIR;

	if (path) {
		log_dir_file = filp_open(path, O_RDONLY, 0);
		if (unlikely(IS_ERR(log_dir_file)))
			return;

		iterate_dir(log_dir_file, &callback.ctx);
		filp_close(log_dir_file, NULL);
	}
}

void wdt_log_save(void)
{
	int i;

	if (!ll_mmc_ready()) {
		pr_err(MOD_NAME ": no ops for mmcoops\n");
		goto freeze;
	}

	if (get_partition_info(dump_part_name, dump_offset, &save_addr)) {
		pr_err(MOD_NAME ": unable to get partition info\n");
		goto freeze;
	}

	local_irq_disable();
	preempt_disable();

	system_state = SYSTEM_HALT;
	smp_send_stop();

	if (ll_mmc_init() < 0) {
		pr_err(MOD_NAME ": mmc initialize error\n");
		goto freeze;
	}

	for (i = 1; i < LOG_MAX; i++)
		log_file_copy(i);

	pr_info(MOD_NAME ": file copy completed: %u bytes\n", total_log_size);

freeze:
	while (1)
		cpu_relax();
}

bool wdt_log_ready(void)
{
	return wdt_init;
}

/* i.e. dump@1M : [partition name]@[offset] */
static int __init wdt_log_init(char *str)
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

	wdt_init = true;
	pr_info(MOD_NAME ":pmlog/legacy log will be stored at "
			"%s partition (offset: 0x%llx)\n",
			dump_part_name, dump_offset);
	return 0;
}
__setup("wdtlog=", wdt_log_init);

