#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/sched.h>

#include "wdt_log.h"

#define GPIODIR	0x0400	/* PL061: data direction register */
#define GPIOIS	0x0404	/* PL061: interrupt sense register */
#define GPIOIEV	0x040c	/* PL061: interrupt event register */
#define GPIOIE	0x0410	/* PL061: interrupt mask register */
#define GPIOMIS	0x0418	/* PL061: masked interrupt status register */
#define GPIOIC	0x041c	/* PL061: interrupt clear register */

enum wdt_event {
	WDT_EVENT_NONE,
	WDT_EVENT_INTR,
	WDT_EVENT_POLL,
};

struct wdt_detect_priv {
	struct platform_device *pdev;

	struct gpio_desc *gpio;
	struct task_struct *task;

	void __iomem *base;
	u8 mask;
	int irq;

	enum wdt_event event;
};

static irqreturn_t wdt_detect_isr(int irq, void *dev_id)
{
	struct wdt_detect_priv *priv = dev_id;
	u8 v;

	v = readb_relaxed(priv->base + GPIOMIS);
	if (!(v & priv->mask))
		return IRQ_NONE;

	/* disable and clear interrupt */
	v = readb_relaxed(priv->base + GPIOIE) & ~priv->mask;
	writeb_relaxed(v, priv->base + GPIOIE);
	writeb_relaxed(priv->mask, priv->base + GPIOIC);

	wdt_log_save();

	return IRQ_HANDLED;
}

static int wdt_detect_init_intr(struct platform_device *pdev)
{
	struct wdt_detect_priv *priv = platform_get_drvdata(pdev);
	struct device_node *np = pdev->dev.of_node;
	struct of_phandle_args args;
	u8 v;
	int ret;

	ret = of_parse_phandle_with_fixed_args(np, "gpios", 2, 0, &args);
	if (ret < 0)
		return -EINVAL;

	priv->mask = (0x01 << args.args[0]);

	priv->base = of_iomap(args.np, 0);
	if (!priv->base)
		return -EIO;

	/* not suppored if initially high-level */
	v = readb_relaxed(priv->base + GPIODIR) & ~priv->mask;
	writeb_relaxed(v, priv->base + GPIODIR);
	if (readb_relaxed(priv->base + (priv->mask << 2)))
		return -ENOTSUPP;

	/* disable and clear interrupts */
	v = readb_relaxed(priv->base + GPIOIE) & ~priv->mask;
	writeb_relaxed(v, priv->base + GPIOIE);
	writeb_relaxed(priv->mask, priv->base + GPIOIC);

	priv->irq = of_irq_get(np, 0);
	if (priv->irq < 0) {
		iounmap(priv->base);
		return priv->irq;
	}

	ret = request_irq(priv->irq, wdt_detect_isr, IRQF_SHARED, "wdt-detect",
			  priv);
	if (ret < 0)
		return ret;

	priv->event = WDT_EVENT_INTR;

	/* enable active-high level-triggered interrupt */
	v = readb_relaxed(priv->base + GPIOIS) | priv->mask;
	writeb_relaxed(v, priv->base + GPIOIS);
	v = readb_relaxed(priv->base + GPIOIEV) | priv->mask;
	writeb_relaxed(v, priv->base + GPIOIEV);
	v = readb_relaxed(priv->base + GPIOIE) | priv->mask;
	writeb_relaxed(v, priv->base + GPIOIE);

	return 0;
}

static int wdt_detect_proc(void *data)
{
	struct wdt_detect_priv *priv = data;
	int v;

	while (!kthread_should_stop()) {
		v = gpiod_get_value(priv->gpio);
		if (v < 0) {
			pr_err("%s: failed in gpiod_get_value\n", __func__);
			return v;
		}

		if (v)
			wdt_log_save();

		msleep(500);
	}

	return 0;
}

static int wdt_detect_init_poll(struct platform_device *pdev)
{
	struct wdt_detect_priv *priv = platform_get_drvdata(pdev);
	struct sched_param param = { .sched_priority = 99 };

	priv->gpio = devm_gpiod_get(&pdev->dev, NULL, GPIOD_IN);
	if (IS_ERR(priv->gpio))
		return PTR_ERR(priv->gpio);

	if (gpiod_get_value(priv->gpio))
		return -ENOTSUPP;

	priv->task = kthread_create(wdt_detect_proc, priv, "wdt-detect");
	if (IS_ERR(priv->task))
		return PTR_ERR(priv->task);

	sched_setscheduler_nocheck(priv->task, SCHED_FIFO, &param);

	priv->event = WDT_EVENT_POLL;

	wake_up_process(priv->task);

	return 0;
}

static int wdt_detect_probe(struct platform_device *pdev)
{
	struct wdt_detect_priv *priv;

	if (!wdt_log_ready())
		return -EPROBE_DEFER;

	priv = devm_kzalloc(&pdev->dev, sizeof(struct wdt_detect_priv),
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);
	priv->pdev = pdev;

	return !wdt_detect_init_intr(pdev) ? 0 : wdt_detect_init_poll(pdev);
}

static int wdt_detect_remove(struct platform_device *pdev)
{
	struct wdt_detect_priv *priv = platform_get_drvdata(pdev);
	u8 v;

	switch (priv->event) {
	case WDT_EVENT_INTR:
		/* disable and clear interrupt */
		v = readb_relaxed(priv->base + GPIOIE) & ~priv->mask;
		writeb_relaxed(v, priv->base + GPIOIE);
		writeb_relaxed(priv->mask, priv->base + GPIOIC);

		free_irq(priv->irq, priv);
		break;

	case WDT_EVENT_POLL:
		kthread_stop(priv->task);
		gpiod_put(priv->gpio);
		break;

	default:
		/* should never be reached */
		break;
	}

	return 0;
}

static struct of_device_id const wdt_detect_of_match[] = {
	{ .compatible = "lge,wdt-detect", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, wdt_detect_of_match);

static struct platform_driver wdt_detect_drv = {
	.probe	= wdt_detect_probe,
	.remove	= wdt_detect_remove,
	.driver	= {
		.name	= "wdt-detect",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(wdt_detect_of_match),
	},
};

#ifndef MODULE

static int __init wdt_detect_init(void)
{
	return platform_driver_register(&wdt_detect_drv);
}
late_initcall(wdt_detect_init);

#else

module_platform_driver(wdt_detect_drv);

#endif

MODULE_DESCRIPTION("LG1K system watchdog timer support");
MODULE_LICENSE("GPL");
