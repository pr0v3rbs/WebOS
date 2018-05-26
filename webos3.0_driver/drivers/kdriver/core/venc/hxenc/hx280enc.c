/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 2013 by LG Electronics Inc.
 
This program is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
GNU General Public License for more details.
*/ 

/** @file
 *
 *  hal api implementation for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2014.05.15
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <asm/io.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ioport.h>

#include "os_util.h"
#include "venc_drv.h"
#include "venc_cfg.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define ENC_HW_ID1                  0x62800000
#define ENC_HW_ID2                  0x72800000
#define ENC_HW_ID3                  0x82700000
#define ENC_HW_ID4                  0x82900000
#define ENC_HW_ID5                  0x48310000

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef struct
{
//	char *buffer;
//	unsigned int buffsize;
	unsigned long iobaseaddr;
	unsigned int iosize;
	volatile u8 *hwregs;
} hx280enc_t;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static int ReserveIO(int id);
static void ReleaseIO(int id);
static void ResetAsic(hx280enc_t * dev);

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static hx280enc_t hx280enc_data[VENC_MAX_DEVICE];

/*========================================================================================
	Implementation Group
========================================================================================*/

int hx280enc_open( int id )
{
    int result;

    if ( id > gpstVencConfig->num_device )
    {
        return -EINVAL;
    }

	if ( hx280enc_data[id].hwregs != NULL )
	{
		VENC_WARN("%s :: [%d] hwreg is not NULL\n", __FUNCTION__, id);
	}

    hx280enc_data[id].iobaseaddr = gpstVencConfig->core[id].reg_base_core;
    hx280enc_data[id].iosize 	 = gpstVencConfig->core[id].reg_size_core;
    hx280enc_data[id].hwregs = NULL;

    VENC_INFO("%s :: [%d] iobaseaddr=0x%08x, iosize=0x%08x\n", __FUNCTION__, id, hx280enc_data[id].iobaseaddr, hx280enc_data[id].iosize );

    result = ReserveIO( id );
	
    if(result < 0)
    {
        goto err;
    }

    ResetAsic(&hx280enc_data[id]);  /* reset hardware */

    return 0;

  err:
    return result;
}

void hx280enc_close(int id)
{
    if ( id > gpstVencConfig->num_device )
    {
        return;
    }

    writel(0, hx280enc_data[id].hwregs + 0x38); /* disable HW */
    writel(0, hx280enc_data[id].hwregs + 0x04); /* clear enc IRQ */

    ReleaseIO(id);

    return;
}

int hx280enc_isr(int id)
{
	hx280enc_t *dev = NULL;
	u32 irq_status;

	if ( id > gpstVencConfig->num_device )
	{
		return RET_ERROR;
	}

	dev = &hx280enc_data[id];

	irq_status = readl(dev->hwregs + 0x04);

	if(irq_status & 0x01)
	{   
		/* clear enc IRQ and slice ready interrupt bit */
		writel(irq_status & (~0x101), dev->hwregs + 0x04);

		/* Handle slice ready interrupts. The reference implementation
		 * doesn't signal slice ready interrupts to EWL.
		 * The EWL will poll the slices ready register value. */
		if ((irq_status & 0x1FE) == 0x100)
		{   
			VENC_WARN("Slice ready IRQ handled!\n");
			return RET_NOT_SUPPORTED;
		}   

		/* All other interrupts will be signaled to EWL. */
#if 0
		if(dev->async_handler)
		{
			dev->async_handler(id);
		}
		else
		{
			VENC_WARN("hx280enc: IRQ received w/o anybody waiting for it!\n");
		}
#else
		
#endif

		//PDEBUG("IRQ handled!\n");
		return RET_OK;
	}   
	else
	{   
		//PDEBUG("IRQ received, but NOT handled!\n");
		return RET_ERROR;
	}   
}

int hx280enc_get_hwinfo(int id, hx280enc_t *param)
{
	if ( id > gpstVencConfig->num_device )
	{
		return RET_ERROR;
	}

	if ( param == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	*param = hx280enc_data[id];

	return RET_OK;
}
EXPORT_SYMBOL(hx280enc_get_hwinfo);

#if 0
void hx280enc_get_hwoffset(int id, unsigned long *pBase)
{
    if ( id > gpstVencConfig->num_device )
    {
        return;
    }

    *pBase = hx280enc_data[id].iobaseaddr;
}
EXPORT_SYMBOL(hx280enc_get_hwoffset);

void hx280enc_get_hwiosize(int id, unsigned long *pSize)
{
    if ( id > gpstVencConfig->num_device )
{
        return;
    }

    *pSize = hx280enc_data[id].iosize;
}
EXPORT_SYMBOL(hx280enc_get_hwiosize);
#endif

static int ReserveIO(int id)
{
    long int hwid;

    if ( id > gpstVencConfig->num_device )
    {
        return -EINVAL;
    }

#if 0
    if(!request_mem_region
       (hx280enc_data[id].iobaseaddr, hx280enc_data[id].iosize, "hx280enc"))
    {
        VENC_ERROR( "hx280enc[%d]: failed to reserve HW regs\n", id);
        return -EBUSY;
    }
#endif

    hx280enc_data[id].hwregs =
        (volatile u8 *) ioremap_nocache(hx280enc_data[id].iobaseaddr,
                                        hx280enc_data[id].iosize);

    if(hx280enc_data[id].hwregs == NULL)
    {
        VENC_ERROR( "hx280enc[%d]: failed to ioremap HW regs\n", id);
        ReleaseIO(id);
        return -EBUSY;
    }

    hwid = readl(hx280enc_data[id].hwregs);

#if 1
    /* check for encoder HW ID */
    if((((hwid >> 16) & 0xFFFF) != ((ENC_HW_ID1 >> 16) & 0xFFFF)) &&
       (((hwid >> 16) & 0xFFFF) != ((ENC_HW_ID2 >> 16) & 0xFFFF)) &&
       (((hwid >> 16) & 0xFFFF) != ((ENC_HW_ID3 >> 16) & 0xFFFF)) &&
       (((hwid >> 16) & 0xFFFF) != ((ENC_HW_ID4 >> 16) & 0xFFFF)) &&
       (((hwid >> 16) & 0xFFFF) != ((ENC_HW_ID5 >> 16) & 0xFFFF)))
    {
        VENC_ERROR( "hx280enc[%d]: HW not found at 0x%08lx\n",
            id, hx280enc_data[id].iobaseaddr);
#ifdef HX280ENC_DEBUG
        dump_regs((unsigned long) &hx280enc_data);
#endif
        ReleaseIO(id);
        return -EBUSY;
    }
#endif

    VENC_INFO(
           "hx280enc[%d]: HW at base <0x%08lx> with ID <0x%08lx>\n",
           id, hx280enc_data[id].iobaseaddr, hwid);

    return 0;
}

static void ReleaseIO(int id)
{
    if ( id > gpstVencConfig->num_device )
	{
        return;
    }

    if(hx280enc_data[id].hwregs)
	{
        iounmap((void *) hx280enc_data[id].hwregs);
		hx280enc_data[id].hwregs = NULL;
	}
#if 0
    release_mem_region(hx280enc_data[id].iobaseaddr, hx280enc_data[id].iosize);
#endif

	return;
}

void ResetAsic(hx280enc_t * dev)
{
    int i;

	if ( dev == NULL || dev->hwregs == NULL )
	{
		return;
	}
	
    writel(0, dev->hwregs + 0x38);

    for(i = 4; i < dev->iosize; i += 4)
    {
        writel(0, dev->hwregs + i);
    }
}

/** @} */


