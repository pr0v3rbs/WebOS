/*
 * Mediatek 8250 driver.
 *
 * Copyright (c) 2014 MundoReader S.L.
 * Author: Matthias Brugger <matthias.bgg@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/serial_8250.h>
#include <linux/serial_reg.h>

#include "8250.h"
#include "chip_setup.h"

#define UART_KEY_SIZE   8
#define UART_KEY_MASK   0x7
#define UART_DATA_SIZE   64
#define UART_DATA_MASK   63 //related in mdrv_system.c, please sync it

int g_in_hotel_mode = 0 ;
EXPORT_SYMBOL(g_in_hotel_mode);

//#define DEBUG

#define MSTAR_UART_BUSY		(0x01)
#define MSTAR_UART_RX_BUSY		(0x18)

#define MSTAR_UART_SWRST			(0x14)
#define MSTAR_UART_RST_DISABLE		(1)
#define MSTAR_UART_RST_ENABLE		(0)

#define UART_DIVISOR_H(x)		(x >> 8)
#define UART_DIVISOR_L(x)		(x & 0xff)
#define UART_DIVISOR(l, h)		((h << 8) | l)

struct mst_uart_data {
	int last_mcr;
	int line;
	struct clk *clk;
};

struct
{
    wait_queue_head_t wq;
    struct semaphore sem;
}MUDI_dev;

int dataidx=0;
int uidx=0;
unsigned char datapool[UART_DATA_SIZE] = {0,} ;
static int MUDI_enabled=0;

void enable_MUDI(void)
{
    uidx=dataidx;
    MUDI_enabled=1;

}

void disable_MUDI(void)
{
    MUDI_enabled=0;
}

static inline int
mst_uart_modify_msr(struct uart_port *p, int offset, int value)
{
    struct mst_uart_data *d = p->private_data;

    /* If reading MSR, report CTS asserted when auto-CTS/RTS enabled */
    if (offset == UART_MSR && d->last_mcr & UART_MCR_AFE) {
        value |= UART_MSR_CTS;
        value &= ~UART_MSR_DCTS;
    }

    return value;
}

static void
mst_uart_force_idle(struct uart_port *p)
{
    serial8250_clear_and_reinit_fifos(
    container_of(p, struct uart_8250_port, port));
    (void)p->serial_in(p, UART_RX);
}

static unsigned int io_serial_in(struct uart_port *p, int offset)
{
    unsigned int value = inb(p->iobase + (offset << p->regshift));
    return mst_uart_modify_msr(p, offset, value);
}

static void io_serial_out(struct uart_port *p, int offset, int value)
{
    struct mst_uart_data *d = p->private_data;

    if (offset == UART_MCR)
        d->last_mcr = value;

    outb(value, p->iobase + (offset << p->regshift));

    /* Make sure LCR write wasn't ignored */
    if (offset == UART_LCR) {
        int tries = 1000;
        while (tries--) {
            unsigned int lcr = p->serial_in(p, UART_LCR);
            if ((value & ~UART_LCR_SPAR) == (lcr & ~UART_LCR_SPAR))
                return;
            mst_uart_force_idle(p);
            outb(value, p->iobase + (UART_LCR << p->regshift));
        }
        dev_err(p->dev, "Couldn't set LCR to %d\n", value);
    }
}

static int handle_irq(struct uart_port *port)
{
    struct uart_8250_port *up = up_to_u8250p(port);
    unsigned int iir;
    int ret = 0;

    serial8250_rpm_get(up);

    iir = serial_port_in(port, UART_IIR);

    if (!(iir & UART_IIR_NO_INT)) {
        serial8250_handle_irq(port, iir);
        ret = 1;
    } else if ((iir & UART_IIR_BUSY) == UART_IIR_BUSY) {
        /* Clear the USR */
        (void)port->serial_in(port, UART_SCR);
        port->serial_out(port, UART_LCR, up->lcr);
        ret = 1;
    }

    serial8250_rpm_put(up);
    return ret;
}

static void
mst_uart_do_pm(struct uart_port *port, unsigned int state, unsigned int old)
{
    if (!state)
        pm_runtime_get_sync(port->dev);

    serial8250_do_pm(port, state, old);

    if (state)
        pm_runtime_put_sync_suspend(port->dev);
}

static int mstar8250_probe(struct platform_device *pdev)
{
    struct uart_port *data = dev_get_platdata(&pdev->dev);
    struct mst_uart_data *private_data;
    struct uart_8250_port uart = {};

    data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    private_data = devm_kzalloc(&pdev->dev, sizeof(*private_data), GFP_KERNEL);
    if (!private_data)
        return -ENOMEM;

    data = dev_get_platdata(&pdev->dev);

    spin_lock_init(&uart.port.lock);
    uart.port.iobase = data->iobase;
    uart.port.irq = data->irq;
    uart.port.type = PORT_16550A;
    uart.port.flags = UPF_SHARE_IRQ | UPF_BOOT_AUTOCONF | UPF_FIXED_PORT;
    uart.port.fifosize = 16;
    uart.port.dev = &pdev->dev;
    uart.port.iotype = data->iotype;
    uart.port.regshift = data->regshift;
    uart.port.private_data = private_data;
    uart.port.uartclk = data->uartclk;
    uart.port.serial_in = io_serial_in;
    uart.port.serial_out = io_serial_out;
    uart.port.handle_irq = handle_irq;
    uart.port.pm = mst_uart_do_pm;

    data->line = serial8250_register_8250_port(&uart);
    private_data->line = data->line;

#ifdef DEBUG
        dev_notice(&pdev->dev, "%s %d, ttyS%d iobase=%x irq=%d\n",
                __func__, __LINE__, data->line,data->iobase, data->irq);
#endif

    if (data->line < 0)
        return data->line;

    platform_set_drvdata(pdev, private_data);

    pm_runtime_set_active(&pdev->dev);
    pm_runtime_enable(&pdev->dev);

    sema_init(&MUDI_dev.sem, 1);
    init_waitqueue_head(&MUDI_dev.wq);

    return 0;
}

static int mstar8250_remove(struct platform_device *pdev)
{
    struct uart_port *data = dev_get_platdata(&pdev->dev);
    serial8250_unregister_port(data->line);

    return 0;
}

static bool
UART16550_BaudRate_Change(struct uart_8250_port *up, int divisor)
{
    short count = 0;
    bool ret = 0;
    unsigned int mcr, lcr, usr;
    struct uart_port *p = &up->port;

    p = &up->port;
    mcr = up->mcr;
    lcr = up->lcr;

repeat:
	/* step 1. set MCR loopback */
	p->serial_out(p, UART_MCR, (mcr | UART_MCR_LOOP));

	/* step 2. poll USR Busy 10 times */
	while (count++ < 10) {
		usr = p->serial_in(p, UART_SCR);
		if ((usr & MSTAR_UART_BUSY) == 0 &&
				(usr & MSTAR_UART_RX_BUSY) == 0) {
			/* step 3. set LCR dl_access */
			p->serial_out(p, UART_LCR, (lcr | UART_LCR_DLAB));

			/* step 4. Check if LCR dl_access or return fail */
			if (!(p->serial_in(p, UART_LCR) & UART_LCR_DLAB))
				goto repeat;

			/* step 5. set DLL & DLH */
			up->dll = UART_DIVISOR_L(divisor);
			up->dlh = UART_DIVISOR_H(divisor);
			p->serial_out(p, UART_DLL, up->dll);
			p->serial_out(p, UART_DLM, up->dlh);
			dev_notice(p->dev, "Update UART%d divisor:0x%x\n",
					p->line, divisor);
			ret = 1;
			break;
		} else {
			/* clear RX FIFO */
			p->serial_out(p, UART_FCR, up->dlh);
			p->serial_out(p, UART_FCR, UART_FCR_ENABLE_FIFO);
			p->serial_out(p, UART_FCR, UART_FCR_ENABLE_FIFO |
					UART_FCR_CLEAR_RCVR |
					UART_FCR_CLEAR_XMIT);
			p->serial_out(p, UART_FCR, 0);
		}
	}

	if (count == 9) {
		dev_err(p->dev, "Uart%d is too busy (USR 0x%x) ",
				p->line, usr);
		dev_err(p->dev, "change baud rate: 0x%x !!\n",
				divisor);
	}

	/* step 6. clear LCR DL access */
	p->serial_out(p, UART_LCR, lcr);

	/* step 7. re-init FIFOs */
	/* p->serial_out(p, UART_FCR, uart_config[up->port.type].fcr); */
	serial8250_clear_and_reinit_fifos(up);

	/* step 8. clear MCR loopback */
	p->serial_out(p, UART_MCR, mcr);

	return ret;
}

extern bool
UART16550_Init(struct uart_8250_port *up, bool reinit)
{
    bool ret = 0;
    int divisor = 0;
    struct uart_port *p = &up->port;

    if(p->iobase == 0)
        return 0;

	if (reinit) {
		/* Toggle re-initial */
		p->serial_out(p, MSTAR_UART_SWRST, MSTAR_UART_RST_ENABLE);
		p->serial_out(p, MSTAR_UART_SWRST, MSTAR_UART_RST_DISABLE);
}

	/* reset baud rate */
	divisor = UART_DIVISOR(up->dll, up->dlh);
	if (p->line == 0) {
		dev_notice(p->dev, "Skip console UART0 (p->line = 0) ");
		dev_notice(p->dev, "baudrate change\n");
	} else
		ret = UART16550_BaudRate_Change(up, divisor);

	/* set FCR */
	/* p->serial_out(p, UART_FCR, uart_config[up->port.type].fcr); */
	serial8250_clear_and_reinit_fifos(up);

	/* set IER */
	p->serial_out(p, UART_IER, up->ier);

	return ret;
}
EXPORT_SYMBOL(UART16550_Init);

#ifdef CONFIG_PM
static void
UART16550_Suspend(struct uart_8250_port *up)
{
    struct uart_port *p = &up->port;
    unsigned char lcr;

    if(p->iobase == 0)
        return;

	if (p->line == 2) {
		while ((p->serial_in(p, UART_SCR)&MSTAR_UART_BUSY)
						== MSTAR_UART_BUSY) {
			if ((p->serial_in(p, UART_LSR)&UART_LSR_THRE)
							== UART_LSR_THRE) {
				up->mcr = p->serial_in(p, UART_MCR);
				lcr = p->serial_in(p, UART_LCR);
				up->ier = UART_IER_RLSI | UART_IER_RDI;
				return;
			}
		}
		up->mcr = p->serial_in(p, UART_MCR);
		lcr = p->serial_in(p, UART_LCR);
		/* up->ier = p->serial_in(up, UART_IER); */
		up->ier = UART_IER_RLSI | UART_IER_RDI;
	} else {
		up->mcr = p->serial_in(p, UART_MCR);
		lcr = p->serial_in(p, UART_LCR);
		up->ier = p->serial_in(p, UART_IER);
		p->serial_out(p, UART_LCR, UART_LCR_DLAB);
		up->dll = p->serial_in(p, UART_DLL);
		up->dlh = p->serial_in(p, UART_DLM);
		p->serial_out(p, UART_LCR, lcr);
		up->lcr = lcr;
	}
}

static int
mst_uart_suspend(struct device *dev)
{
    struct mst_uart_data *data = dev_get_drvdata(dev);
    struct uart_8250_port *up;

    dev_notice(dev, "%s line=%d\n", __func__, data->line);

    up = serial8250_get_port(data->line);
    UART16550_Suspend(up);
    serial8250_suspend_port(data->line);

    return 0;
}

static int
mst_uart_resume(struct device *dev)
{
    struct mst_uart_data *data = dev_get_drvdata(dev);
    struct uart_8250_port *up;

    up = serial8250_get_port(data->line);
    UART16550_Init(up, 0);
    serial8250_resume_port(data->line);

    dev_notice(dev, "%s line=%d\n", __func__, data->line);

    return 0;
}
#endif /* CONFIG_PM */

static const struct dev_pm_ops mst_uart_pm_ops = {
#ifdef CONFIG_PM
    SET_SYSTEM_SLEEP_PM_OPS(mst_uart_suspend, mst_uart_resume)
#endif /* CONFIG_PM */
};

static const struct of_device_id mstar8250_of_match[] = {
	{ .compatible = "mstar,mstar8250-uart" },
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, mstar8250_of_match);

static struct platform_driver mstar8250_platform_driver = {
	.driver = {
		.name		= "mstar8250-uart",
                .pm		= &mst_uart_pm_ops,
		.of_match_table	= mstar8250_of_match,
	},
	.probe			= mstar8250_probe,
	.remove			= mstar8250_remove,
};
module_platform_driver(mstar8250_platform_driver);


MODULE_AUTHOR("Vick Sun");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Mstar 8250 serial port driver");
