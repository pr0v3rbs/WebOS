
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/amba/pl022.h>

#include "include/vdec_kapi.h"
#include "d14_api.h"


#define LOG_NAME	d14
#include "log.h"

static struct spi_master *master;
struct spi_device *d14_device;

static DEFINE_MUTEX (lock);

int do_d14_ioctl (unsigned int cmd,
		union ioctl_args *a, unsigned long arg)
{
	int ret = 0;

	log_info ("command %08x, arg %lx\n", cmd, arg);

	if (!d14_device)
	{
		log_error ("no spi device\n");
		return -ENODEV;
	}

	mutex_lock (&lock);


	/* D14 running check */
	ret = 0;
	switch (cmd)
	{
	default:
		break;

	case D14_GET_IPC:
	case D14_SET_IPC:
	case D14_SET_IPC_:
	case D14_GET_REG:
	case D14_SET_REG:
		if (check_available () < 0)
		{
			log_warning ("Oops?\n");
			ret = -1;
		}
		break;
	}

	if (ret < 0)
	{
		mutex_unlock (&lock);
		return ret;
	}



	/* actural command */
	switch (cmd)
	{
	case D14_RESET:
		ret = reset_mcu ();
		break;

	case D14_SET_SPI_SPEED:
		if (a->d14_spi_speed > 0)
		{
			log_info ("set speed %dMHz\n", a->d14_spi_speed);
			d14_device->max_speed_hz = a->d14_spi_speed*1000000;
			ret = spi_setup (d14_device);
		}
		a->d14_spi_speed = d14_device->max_speed_hz/1000000;
		break;

	case D14_SET_FIRMWARE:
		ret = set_firmware (a->d14_firmware.data,
				a->d14_firmware.size);
		break;

	case D14_GET_IPC:
		ret = ipc_read (a->d14_register.addr,
				&a->d14_register.data);
		break;

	case D14_SET_IPC_:
		ret = _ipc_write (a->d14_register.addr,
				a->d14_register.data);
		break;

	case D14_SET_IPC:
		ret = ipc_write (a->d14_register.addr,
				a->d14_register.data);
		break;

	case D14_GET_REG:
		ret = reg_read (a->d14_register.addr,
				&a->d14_register.data);
		break;

	case D14_SET_REG:
		ret = reg_write (a->d14_register.addr,
				a->d14_register.data);
		break;

	case D14_GET_MEM:
		ret = mem_read (a->d14_memory.addr,
				a->d14_memory.data, a->d14_memory.size);
		break;

	case D14_SET_MEM:
		ret = mem_write (a->d14_memory.addr,
				a->d14_memory.data, a->d14_memory.size);
		break;

	default:
		ret = -ENOIOCTLCMD;
		log_error ("Uh?\n");
		break;
	}


	if (ret < 0)
		log_warning ("ioctl failed. %08x, %lx\n",
				cmd, arg);

	mutex_unlock (&lock);

	return ret;
}

static void select_dummy_chip(unsigned int chipselect) { }

static struct pl022_config_chip d14_chip_info = {
        .com_mode = POLLING_TRANSFER,
        .iface = SSP_INTERFACE_MOTOROLA_SPI,
        .hierarchy = SSP_MASTER,
        .slave_tx_disable = 0,
        .rx_lev_trig = SSP_RX_1_OR_MORE_ELEM,
        .tx_lev_trig = SSP_TX_1_OR_MORE_EMPTY_LOC,
        .ctrl_len = SSP_BITS_12,
        .wait_state = SSP_MWIRE_WAIT_ZERO,
        .duplex = SSP_MICROWIRE_CHANNEL_FULL_DUPLEX,
        .clkdelay = SSP_FEEDBACK_CLK_DELAY_NONE,
        .cs_control = select_dummy_chip,
};

static struct spi_board_info d14_chip =
{
	.modalias = "d14",
	.max_speed_hz = 2000000,
	.bus_num = 1,
	.chip_select = 0,
	.mode = SPI_MODE_0,
	.controller_data = &d14_chip_info,
};

int d14_init (void)
{
	master = spi_busnum_to_master (1);
	if (!master)
	{
		log_error ("no spi master\n");
		return -ENODEV;
	}

	d14_chip.chip_select = master->num_chipselect;
	master->num_chipselect ++;
	printk ("initialize D14... %d\n", master->num_chipselect);

	d14_device = spi_new_device (master, &d14_chip);
	if (!d14_device)
	{
		log_error ("no spi device\n");
		return -ENODEV;
	}

	/* kick initializer work */

	return 0;
}

