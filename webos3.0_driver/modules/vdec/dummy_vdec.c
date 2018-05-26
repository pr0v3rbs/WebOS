
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int dummy_init (void)
{
	printk (KERN_INFO "%s.%d, dummy vdec init...\n", __func__, __LINE__);

	return 0;
}

static void dummy_cleanup (void)
{
	printk (KERN_INFO "%s.%d, dummy vdec cleanup...\n", __func__, __LINE__);

	return;
}

module_init(dummy_init);
module_exit(dummy_cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("LG dummy video decoder driver");
MODULE_LICENSE("GPL");
