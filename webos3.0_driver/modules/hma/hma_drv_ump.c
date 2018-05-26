
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>

#include <ump/ump_kernel_interface_ref_drv.h>

#include "hma_drv_ext.h"
#include "hma_drv.h"

static hma_drv_hook_open_t open_org;
static hma_drv_hook_ioctl_t ioctl_org;
static hma_drv_hook_release_t release_org;

static int priv_offset;
struct priv
{
	ump_dd_handle ump_handle;
};

static int open (struct inode *inode, struct file *file)
{
	struct priv *priv = (struct priv*)((int)file->private_data + priv_offset);

	priv->ump_handle = UMP_DD_HANDLE_INVALID;

	return 0;
}

static long ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct hma_drv_mem_desc *desc = file->private_data;
	struct priv *priv = (struct priv*)((int)file->private_data + priv_offset);

	switch (cmd)
	{
		case HMA_IOCTL_GET_UMP_SECURE_ID:
			if (desc->paddr == 0 || desc->size == 0)
				return -EINVAL;

			if (priv->ump_handle == UMP_DD_HANDLE_INVALID)
			{
				ump_dd_physical_block block;

				block.addr = desc->paddr;
				block.size = desc->size;

				priv->ump_handle = ump_dd_handle_create_from_phys_blocks (&block, 1);
			}

			if (priv->ump_handle != UMP_DD_HANDLE_INVALID)
			{
				ump_secure_id secure_id;

				secure_id = ump_dd_secure_id_get (priv->ump_handle);

				put_user (secure_id, (ump_secure_id*)arg);
			}
			else
				return -EINVAL;

			return 0;
	}

	return -EINVAL;
}

static int release (struct inode *inode, struct file *file)
{
	struct priv *priv = (struct priv*)((int)file->private_data + priv_offset);

	if (priv->ump_handle != UMP_DD_HANDLE_INVALID)
		ump_dd_reference_release (priv->ump_handle);

	return 0;
}

static int hma_drv_ump_init (void)
{
	open_org = hma_drv_hook_open;
	ioctl_org = hma_drv_hook_ioctl;
	release_org = hma_drv_hook_release;

	hma_drv_hook_open = open;
	hma_drv_hook_ioctl = ioctl;
	hma_drv_hook_release = release;

	priv_offset = hma_drv_priv_size;
	hma_drv_priv_size += sizeof(struct priv);

	return 0;
}

static void hma_drv_ump_exit (void)
{
	/*
	 * FIXME
	 * Invokation order of rmmod should be inverse order of insmod order.
	 */
	hma_drv_hook_open = open_org;
	hma_drv_hook_ioctl = ioctl_org;
	hma_drv_hook_release = release_org;

	hma_drv_priv_size -= sizeof(struct priv);
}

MODULE_LICENSE ("GPL");
module_init (hma_drv_ump_init);
module_exit (hma_drv_ump_exit);

