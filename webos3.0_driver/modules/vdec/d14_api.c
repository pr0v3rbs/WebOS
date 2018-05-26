
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>
#include <linux/spi/spi.h>

#include "gpio/gpio_core.h"

#include "vdec_type_defs.h"

#define LOG_NAME	d14f
#include "log.h"


#define error(f,a...)	log_error(f,##a)
#define info(f,a...)	log_info(f,##a)
#define debug(f,a...)	log_debug(f,##a)


#define D14_REG_BASE_ADDR		0xE0005000
#define D14_REG_MCU0_IPC_INT		0xB0080034
#define D14_REG_MCU0_IPC14		0xB0080078

#define DX_REG_FW_VERSION			0x0000
#define DX_REG_HEVC_VERSION			0x0001
#define DX_REG_IPC1_BUSY			0x004a

extern struct spi_device *d14_device;
static void *firmware_data;
static int firmware_size;

static int d14_fwload_spi_speed = 10;
module_param (d14_fwload_spi_speed, int, 0644);

static int d14_hold_firmware;
module_param (d14_hold_firmware, int, 0644);

static int d14_spi_delay_us = 100;
module_param (d14_spi_delay_us, int, 0644);

static int d14_flash_erased;
module_param (d14_flash_erased, int, 0644);


/* wait enough time to boot mcu0, mcu1 using spi */
static int d14_delay_before_flash_erase = 500;
module_param (d14_delay_before_flash_erase, int, 0644);

static int d14_delay_after_flash_erase = 100;
module_param (d14_delay_after_flash_erase, int, 0644);

static int d14_no_reset;
module_param (d14_no_reset, int, 0644);


/*
 * D14 status.
 * 0: power up.
 * 1: hardware reset done.
 * 2: firmware loaded and boot done.
 */
static int status;


static int send (void *data, int len)
{
	struct spi_transfer t =
	{
		.tx_buf = data,
		.len = len,
		.delay_usecs = d14_spi_delay_us,
	};
	struct spi_message m;

	spi_message_init (&m);
	spi_message_add_tail (&t, &m);

	return spi_sync (d14_device, &m);
}

static int _mem_read (unsigned int addr,
		__user unsigned char *buf, int len, int user)
{
	unsigned int a;
	int c;
	struct spi_transfer t;
	struct spi_message m;
	unsigned char w[9], r[9];
	int ret = 0;

	debug ("addr %08x, size %d\n", addr, len);
	if ((addr & 3) || (len & 3))
	{
		error ("not aligned address or size. %08x, %d\n", addr, len);
		return -1;
	}

	a = addr;
	c = 0;
	while (c < len)
	{
		int ret;

		memset (w, 0, sizeof(w));
		memset (r, 0, sizeof(r));
		w[0] = (a>>24)&0xff;
		w[1] = (a>>16)&0xff;
		w[2] = (a>> 8)&0xff;
		w[3] = (a>> 0)&0xff;
		w[3] |= 1;
		w[4] = 0;


		/* make spi message */
		spi_message_init(&m);

		memset (&t, 0, sizeof (t));
		t.tx_buf = w;
		t.rx_buf = r;
		t.len = 9;
		t.delay_usecs = d14_spi_delay_us;
		spi_message_add_tail(&t, &m);
		ret = spi_sync(d14_device, &m);
		if (ret < 0)
		{
			error ("spi sync failed.\n");
			break;
		}

		if (user)
		{
			put_user (r[8], buf+c+0);
			put_user (r[7], buf+c+1);
			put_user (r[6], buf+c+2);
			put_user (r[5], buf+c+3);
		}
		else
		{
			buf[c+0] = r[8];
			buf[c+1] = r[7];
			buf[c+2] = r[6];
			buf[c+3] = r[5];
		}

		a += 4;
		c += 4;
	}

	return ret;
}

int mem_read (unsigned int addr, __user unsigned char *buf, int len)
{
	return _mem_read (addr, buf, len, 1);
}

static int _mem_write (unsigned int addr,
		__user unsigned char *buf, int len, int user)
{
	unsigned int a;
	int c;
	unsigned char *tmp;
	int ret = 0;

	debug ("address %08x, len %d\n", addr, len);

	len = (len+3)&~3;
	if (addr & 3)
	{
		error ("not aligned address. %08x\n", addr);
		return -1;
	}

	a = addr;
	c = 0;
	tmp = kmalloc (5+256*4, GFP_KERNEL);
	if (!tmp)
	{
		error ("no mem?\n");
		return -ENOMEM;
	}

	while (c < len)
	{
		int count;

		//debug ("address %08x, len %d, c %d\n", a, len, c);

		count = 256;
		if ((len - c)/4 < count)
			count = (len - c)/4;
		//debug ("count %d\n", count);

		tmp[0] = (a>>24)&0xff;
		tmp[1] = (a>>16)&0xff;
		tmp[2] = (a>> 8)&0xff;
		tmp[3] = (a>> 0)&0xff;
		tmp[3] |= 2;
		tmp[4] = count-1;

		if (user)
		{
			if (copy_from_user (tmp+5, buf+c, count*4) > 0)
			{
				error ("Oops?\n");
				ret = -EFAULT;
				break;
			}
		}
		else
			memcpy (tmp+5, buf+c, count*4);

		//if (!big_endian)
		{
			int i;

			for (i=0; i<count; i++)
			{
				swap (tmp[5+i*4+0], tmp[5+i*4+3]);
				swap (tmp[5+i*4+1], tmp[5+i*4+2]);
			}
		}

		send (tmp, 5 + count*4);

		a += count*4;
		c += count*4;
	}

	kfree (tmp);

	return ret;
}

int mem_write (unsigned int addr, __user unsigned char *buf, int len)
{
	return _mem_write (addr, buf, len, 1);
}

int reg_write (unsigned int addr, unsigned int reg)
{
	unsigned char tmp[8];

	tmp[0] = (addr>>24)&0xff;
	tmp[1] = (addr>>16)&0xff;
	tmp[2] = (addr>> 8)&0xff;
	tmp[3] = (addr>> 0)&0xff;
	tmp[4] = (reg>>24)&0xff;
	tmp[5] = (reg>>16)&0xff;
	tmp[6] = (reg>> 8)&0xff;
	tmp[7] = (reg>> 0)&0xff;

	send (tmp, 8);

	return 0;
}

int reg_read (int reg, unsigned int *data)
{
	int ret;

	ret = _mem_read (reg, (unsigned char*)data, 4, 0);

	return ret;
}


int ipc_read (int reg, unsigned int *data)
{
	return reg_read (D14_REG_BASE_ADDR+(reg*4), data);
}

int _ipc_write (int reg, unsigned int data)
{
	debug ("command 0x%x, data 0x%x\n", reg, data);
	reg_write (D14_REG_BASE_ADDR+reg*4, data);

	return 0;
}

static int ipc_wait_busy (void)
{
	int wait_start;
	unsigned int busy;

	wait_start = 0;

wait:
	busy = 0;
	ipc_read (DX_REG_IPC1_BUSY, &busy);
	if (busy)
	{
		if (!wait_start)
		{
			//info ("ipc busy... %x\n", busy);
			wait_start = jiffies;
		}

		if (jiffies - wait_start > 2*HZ)
		{
			error ("ipc not responding...\n");
			return -1;
		}

		goto wait;
	}
	return 0;
}

int ipc_write (int reg, unsigned int data)
{
	debug ("command 0x%x, data 0x%x\n", reg, data);

	reg_write (D14_REG_BASE_ADDR+DX_REG_IPC1_BUSY*4, 1);
	reg_write (D14_REG_BASE_ADDR+reg*4, data);
	reg_write (D14_REG_MCU0_IPC14, reg);
	reg_write (D14_REG_MCU0_IPC_INT, (1<<14));

	if (ipc_wait_busy () < 0)
	{
		error ("give up. do reset D14.\n");
		status = 1;
		return -1;
	}

	return 0;
}

#define D14_MCU0_BASE_ADDR		0x00000000
#define D14_MCU1_OFFSET			0xA0000

#define	D14_HEVC_SendWordBurstMode(a,p,s)	_mem_write(a, p, s, 0)
#define D14_HEVC_SendWord(a,d)			reg_write(a, d)
#define D14_HEVC_ReadWord(a,d)			reg_read(a, d)
#define OSA_SuspendTask(m)			schedule_timeout_uninterruptible ((m*HZ/1000)+1);

static int load_mcu0 (UINT8 *pbuffer, UINT32 size)
{
	UINT32 page_cnt=0;
	UINT8 *codeBuffer = NULL;
	UINT32 *tempbuffer = NULL;
	UINT32 codeLength, paramLength;

	info("live boot mcu0. size %d\n", size);
	if (size < 4096)
	{
		error ("too small firmware size, %d\n", size);
		return -1;
	}

	// Read Flash Header
	tempbuffer = (UINT32 *)&pbuffer[0];

	paramLength = tempbuffer[1];
	codeLength = tempbuffer[3];
	info ("firmware size %d, param %d, code %d\n",
			size, paramLength, codeLength);

	if (2048 + paramLength > 4096)
	{
		error ("Oops. paramlength %d, size %d\n",
				paramLength, size);
		return -1;
	}

	if (4096 + codeLength > size)
	{
		error ("Oops. codelength %d, size %d\n",
				codeLength, size);
		return -1;
	}


	// Get Param Data
	tempbuffer = (UINT32 *)&pbuffer[2048];

	for(page_cnt =0; page_cnt < (paramLength>>2) ;  page_cnt+=2)
	{
		if(tempbuffer[page_cnt]==0x6c6f6f70)
		{
			OSA_SuspendTask (tempbuffer[page_cnt+1]);
			continue;
		}
		D14_HEVC_SendWord(tempbuffer[page_cnt],tempbuffer[page_cnt+1]);
	}

	/* load firmware... */
	codeBuffer = (UINT8 *)&pbuffer[4096];

	D14_HEVC_SendWordBurstMode(D14_MCU0_BASE_ADDR, codeBuffer, codeLength);

	D14_HEVC_SendWord(0xB0080034,0x00000001);

	// End time check
	info("MCU0 live boot:\n");

	return 0;
}


static int load_mcu1 (unsigned char *data, int size)
{
	info ("write mcu1 firmware. size %d\n", size);
	_mem_write (0x40000000, data, size, 0);

	info ("set address switch\n");
	reg_write (0xb00800d8, 0x40000000);

	info ("do mcu1 reset\n");
	reg_write (0xb0080034+0xc0,0x00000001);

	return 0;
}



static int load_firmwares (int bin_ver)
{
	int ret;

	info ("load... firmware\n");

	if (firmware_size <= 0 || firmware_size <= D14_MCU1_OFFSET)
	{
		error("wrong firmware size. %d\n", firmware_size);
		return -1;
	}

	ret = load_mcu0 (firmware_data, D14_MCU1_OFFSET);
	if (ret < 0)
	{
		error ("mcu0 loading failed.\n");
		goto done;
	}

	if (bin_ver != 2)
	{
		ret = load_mcu1 (firmware_data+D14_MCU1_OFFSET,
				firmware_size-D14_MCU1_OFFSET);
		if (ret < 0)
		{
			error ("mcu1 loading failed.\n");
			goto done;
		}
	}
	else
		info ("encrypted mcu1 firmware. skip mcu1 loading.\n");

done:
	info("done.\n");

	return ret;
}


#define D14_SPI_BASE_OFFSET 		0xF0004000
#define D14_SPI_CS_GPIO_OUT		((D14_SPI_BASE_OFFSET) + (0x44))
#define D14_SPI_SSPCR0			((D14_SPI_BASE_OFFSET) + (0x00))
#define D14_SPI_SSPCR1			((D14_SPI_BASE_OFFSET) + (0x04))
#define D14_SPI_SSPDR			((D14_SPI_BASE_OFFSET) + (0x08))
#define D14_SPI_SSPSR			((D14_SPI_BASE_OFFSET) + (0x0C))
#define NOT_OK				(-1)
#define OK				(0)
#define DTV_STATUS_T			int

static BOOLEAN D14_HEVC_Spi_Check(void)
{
	UINT32 cnt,data  =0;

	for (cnt = 0; cnt < 1000; cnt++) {
		OSA_SuspendTask(10);
		D14_HEVC_ReadWord(D14_SPI_SSPSR, &data);

		//SPI Busy Check
		if (!(data&0x00000010)) { return TRUE;}
	}
	return FALSE;
}

static void D14_HEVC_Spi_BufferClear(void)
{
	UINT32 data;

	D14_HEVC_ReadWord(D14_SPI_SSPDR, &data);
}

static void D14_HEVC_Spi_RxFifoClear(void)
{
	UINT32 data = 0x00;
	UINT8 i;

	D14_HEVC_ReadWord(D14_SPI_SSPSR, &data);
	if(data != 0x00000003){
		for ( i=0;i<8;i++){ D14_HEVC_ReadWord(D14_SPI_SSPDR, &data); }
	}
}

static void D14_HEVC_Spi_WREN(BOOLEAN bOnOff)
{
	UINT32 data = 0x00;

	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000000); //gpio out low (CS Low)

	if(bOnOff)	{ D14_HEVC_SendWord(D14_SPI_SSPDR, 0x06);}
	else		{ D14_HEVC_SendWord(D14_SPI_SSPDR, 0x04);}

	D14_HEVC_Spi_Check();
	D14_HEVC_Spi_BufferClear();

	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high (CS High)

	D14_HEVC_ReadWord(D14_SPI_SSPSR, &data);
	if((data & 0x0000001F) != 0x00000003)	{ D14_HEVC_Spi_RxFifoClear();}
}

static UINT8 D14_HEVC_Spi_RDSR(void)
{
	UINT32 data = 0x00;
	UINT8 i;

	D14_HEVC_Spi_RxFifoClear();
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000000); //gpio out Low (CS Low)

	for(i=0;i<3;i++){
		D14_HEVC_SendWord(D14_SPI_SSPDR, 5);
		D14_HEVC_Spi_Check();
		D14_HEVC_Spi_BufferClear();
		D14_HEVC_ReadWord(D14_SPI_SSPDR, &data);
	}

	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high (CS High)

	return (UINT8)data;
}

static BOOLEAN D14_HEVC_Spi_BE(UINT8 block)
{
	UINT32 bCnt =0;
	UINT8 bread =0;

	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000000); //gpio out Low (CS Low)
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high (CS High)
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000000); //gpio out Low (CS Low)

	D14_HEVC_SendWord(D14_SPI_SSPDR, 0x000000D8);
	D14_HEVC_Spi_Check(); D14_HEVC_Spi_BufferClear();

	D14_HEVC_SendWord(D14_SPI_SSPDR, (UINT32)block);
	D14_HEVC_Spi_Check(); D14_HEVC_Spi_BufferClear();

	D14_HEVC_SendWord(D14_SPI_SSPDR, 0);
	D14_HEVC_Spi_Check(); D14_HEVC_Spi_BufferClear();

	D14_HEVC_SendWord(D14_SPI_SSPDR, 0);
	D14_HEVC_Spi_Check(); D14_HEVC_Spi_BufferClear();

	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high (CS High)

	while (bCnt < 10000 ){
		bread  = D14_HEVC_Spi_RDSR() & 0x03;
		bread |= D14_HEVC_Spi_RDSR() & 0x03;
		bread |= D14_HEVC_Spi_RDSR() & 0x03;

		if(!bread) {
			info("D14 Flash 1th block erase SUCCESS[%d][%d] = 0x%x\n",
					block,  bCnt, bread);
			return TRUE;
		}
		bCnt++;
		OSA_SuspendTask(5);
	};

	error("D14 Flash 1th block erase Fail[%d] = 0x%x\n",
			bCnt, D14_HEVC_Spi_RDSR());

	return FALSE;;
}

static DTV_STATUS_T D14_HEVC_SpiBlockErase (void)
{
	info("START\n");

	//spi_reset
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high
	D14_HEVC_SendWord(D14_SPI_SSPSR,		0x08);
	D14_HEVC_SendWord(D14_SPI_SSPCR0,	0x07);
	D14_HEVC_SendWord(D14_SPI_SSPCR1,	0x02);
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000000); //gpio out low
	D14_HEVC_SendWord(D14_SPI_CS_GPIO_OUT, 0x00000001); //gpio out high

	// check serial flash status
	OSA_SuspendTask(500);

	// check serial flash status
	D14_HEVC_Spi_Check();
	OSA_SuspendTask(10);
	D14_HEVC_Spi_WREN(0);
	OSA_SuspendTask(10);

	//for(i=0;i<5;i++) //flash 4block 만 지움 -> flash 0번지의 valid mark가 0xFFFF가 되어서 boot모드 진입 후 강제 flash erase로 됨.
	//{
	D14_HEVC_Spi_WREN(1);
	OSA_SuspendTask(10);
	if(!D14_HEVC_Spi_BE(0)) return NOT_OK;
	//}
	info("END\n");

	return OK;
}

static int kick_d14 (int bin_ver)
{
	int ret;
	int a;

	info ("kick d14, %d\n", bin_ver);
	/* clear version */
	_ipc_write (DX_REG_FW_VERSION, 0);
	_ipc_write (DX_REG_HEVC_VERSION, 0);

	/* load firmware */
	{
		int org_speed;

		org_speed = d14_device->max_speed_hz;
		d14_device->max_speed_hz = d14_fwload_spi_speed*1000000;
		if (spi_setup (d14_device) < 0)
			error ("speed setting failed. %d\n", d14_fwload_spi_speed);

		info ("spi speed %dMHz\n", d14_fwload_spi_speed);

		ret = load_firmwares (bin_ver);

		info ("restore spi speed to %dMHz\n", org_speed);
		d14_device->max_speed_hz = org_speed;
		if (spi_setup (d14_device) < 0)
			error ("speed restoring failed. %d\n", org_speed);

		if (ret < 0)
			return -1;
	}

	/* wait boot-up */
	info ("wait boot...\n");
	ret = -1;
	for (a=0; a<50; a++)
	{
		unsigned int ver0, ver1;

		OSA_SuspendTask (100);

		ipc_read (DX_REG_FW_VERSION, &ver0);
		ipc_read (DX_REG_HEVC_VERSION, &ver1);
		if (ver0 && ver1)
		{
			error ("got version %04x, %04x\n", ver0, ver1);

			ret = 0;
			break;
		}
	}
	if (ret < 0)
		error ("boot failed.\n");
	else
		info ("boot done.\n");

	return ret;
}


#if 0
#include <linux/io.h>
#define GPIONDIR(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x400)))
static UINT32 __gpioBaseAddr[10];
#define GPION_BASE(n)		(__gpioBaseAddr[n])
static int gpio_init(void)
{
	static int inited;
	if (inited)
		return 0;

#define H13_COREAPB_BASE	0xfd000000		/* CoreAPB */
#define H13_GPIO__BASE		(H13_COREAPB_BASE+0x400000)  /* GPIO Base */

	__gpioBaseAddr[1] = (UINT32)ioremap(H13_GPIO__BASE + 1*0x10000 , 0x10000);

	inited = 1;

	return 0;
}
static int HPIO_DevSetMode(UINT32 port, LX_GPIO_MODE_T mode)
{
	UINT32 direction=0, mask=0;

	gpio_init();

	direction = GPIONDIR(port/8);

	if(port < 136)	{ mask = 1 << (port % 8); }
	else	 if(port >= 136 && port < 144)		{ mask = 1 << (7 - (port % 8)); }


	direction = (mode == LX_GPIO_MODE_INPUT) ?
				direction & (~mask) : direction | mask;

	GPIONDIR(port/8) = direction;
	return 0;
}

static int HPIO_DevSetValue(UINT32 port, LX_GPIO_VALUE_T value)
{
	UINT32 mask=0, data=0;

	gpio_init();

	if(port < 136)	{ mask = 1 << (2 + (port % 8)); }
	else	 if(port >= 136 && port < 144)		{ mask = 1 << (2 + (7-(port % 8))); }

	data = (value == LX_GPIO_VALUE_HIGH) ? 0xff : 0x0;
#define STS_WRITE32(addr,value)	(*(volatile UINT32 *)(addr)) = (volatile UINT32)(value)
	STS_WRITE32(GPION_BASE(port/8) + mask, data);
	return 0;
}
#else
#define HPIO_DevSetMode(a,b)	GPIO_DevSetMode(a,b)
#define HPIO_DevSetValue(a,b)	GPIO_DevSetValue(a,b)
#endif


static int reset_pin = 8;
static int hardware_reset (void)
{
	info ("do hardware reset\n");

	HPIO_DevSetMode (reset_pin, LX_GPIO_MODE_OUTPUT);
	OSA_SuspendTask (10);
	HPIO_DevSetValue (reset_pin, LX_GPIO_VALUE_LOW);
	OSA_SuspendTask (10);
	HPIO_DevSetValue (reset_pin, LX_GPIO_VALUE_HIGH);

	info ("done\n");

	return 0;
}

static int read_firmware_binary_version (void)
{
	unsigned int version;

	if (!firmware_data)
		return -1;
	if (firmware_size <= 0 || firmware_size <= D14_MCU1_OFFSET)
		return -1;

	version  = ((unsigned char*)firmware_data)[256+0];
	version |= ((unsigned char*)firmware_data)[256+1]<<8;
	info ("firmware binary version 0x%04x\n", version);

	return (version>>8)&0xf;
}

int reset_mcu (void)
{
	int bin_ver;
	int ret;

	status = 0;

	error ("reset D1x chip.\n");
	/* do hardware reset */
	hardware_reset ();

	bin_ver = read_firmware_binary_version ();
	if (bin_ver < 0)
	{
		error ("Oops\n");
		return -1;
	}

	if (bin_ver == 1)
	{
		info ("self loading binary.\n");
		OSA_SuspendTask(2400);
		status = 2;
		error ("done.\n");
		return 0;
	}

	/* erase first flash block for first time */
	if (!d14_flash_erased)
	{
		info ("erase first flash block, delay %dms, %dms\n",
				d14_delay_before_flash_erase,
				d14_delay_after_flash_erase);

		OSA_SuspendTask (d14_delay_before_flash_erase);
		D14_HEVC_SpiBlockErase ();
		OSA_SuspendTask (d14_delay_after_flash_erase);

		hardware_reset ();

		d14_flash_erased = 1;
	}

	/* kick d14 */
	OSA_SuspendTask(100);
	ret = kick_d14 (bin_ver);
	if (ret < 0)
	{
		error ("reset failed. erase flash again.\n");
		d14_flash_erased = 0;
	}
	else
	{
		status = 2;
		error ("done.\n");
	}

	return ret;
}


int set_firmware (unsigned char *data, int size)
{
	int ret;

	info ("set firmware, %p, %d\n", data, size);

	if (firmware_data && d14_hold_firmware)
	{
		info ("hold current firmware.\n");
		return 0;
	}

	if (firmware_data)
		vfree (firmware_data);

	firmware_data = vmalloc (size);
	if (!firmware_data)
	{
		error ("no memory\n");
		ret = -ENOMEM;
	}
	else
	{
		ret = copy_from_user (firmware_data, data, size);
		if (ret)
		{
			error ("Oops\n");
			vfree (firmware_data);
			firmware_data = NULL;
			firmware_size = 0;

			ret = -EFAULT;
		}
		else
		{
			firmware_size = size;

			ret = 0;
		}
	}

	return ret;
}

int check_available (void)
{
	int ret;

	debug ("check D14...\n");

	if (d14_no_reset)
	{
		info ("do not reset Dx\n");
		return 0;
	}

	if (status < 2)
		ret = reset_mcu ();
	else
		ret = 0;

	return ret;
}
