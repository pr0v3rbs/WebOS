#include <linux/sysrq.h>
#include <linux/init.h>

extern void do_sysrq_print_klog(void);

static void sysrq_handle_print_klog(int key)
{
	do_sysrq_print_klog();
}
static struct sysrq_key_op sysrq_printklog_op = {
	.handler	= sysrq_handle_print_klog,
	.help_msg	= "print-kernel-log-buffer(y)",
	.action_msg	= "print-kernel-log-buffer",
	.enable_mask	= SYSRQ_ENABLE_LOG,
};

static int __init sysrq_klog_init(void)
{
	return register_sysrq_key('y', &sysrq_printklog_op);
}
device_initcall(sysrq_klog_init);



