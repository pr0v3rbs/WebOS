#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init webos_init(void)
{
	return 0;
}

static void __exit webos_exit(void)
{
}

module_init(webos_init);
module_exit(webos_exit);

MODULE_LICENSE("GPL");

