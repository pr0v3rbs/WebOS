/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  main configuration file for slt device
 *	slt device will teach you how to make device driver with new platform.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
 *	@{
 */


/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< slt_debug() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */

#include <linux/interrupt.h>/** irqreturn_t */
#include <linux/irq.h>		/** platform interrupt */
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/sched.h>

#include <linux/delay.h>

#include "os_util.h"
#include "sys_regs.h"

#include "slt_drv.h"
#include "slt_hw.h"
#include "slt_h15.h"

/*----------------------------------------------------------------------------------------
  Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------------------------*/
//#define SLT_AUTO_RESET_DISABLE
//#define VSD_CRC

#ifdef VSD_CRC
#define H15_CRC_BASE	H15_CRC_VSD_BASE
#define H15_IRQ_SLT		H15_IRQ_SLT0
#else // DPE_CRC
#define H15_CRC_BASE	H15_CRC_DPE_BASE
#define H15_IRQ_SLT		H15_IRQ_SLT1
#endif

/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void slt_test_get_crc( void );
static void slt_test_check_fixed_crc( void );

/*----------------------------------------------------------------------------------------
  Static Variables
----------------------------------------------------------------------------------------*/
static UINT32	crc_check_count;
static UINT32	crc_count = 0;
static UINT32	prev_crc = 0;
static LX_SLT_CRC_DATA_T	crc_data;
static LX_SLT_TEST_MODE_E	slt_test_mode = SLT_TEST_GET_CRC;


/*========================================================================================
    Implementation Group
========================================================================================*/

/** SLT ISR handler * */

static void SLT_ISR_Handler( void )
{
	if( slt_test_mode == SLT_TEST_GET_CRC )
	{
		slt_test_get_crc();
	}
	else if( slt_test_mode == SLT_TEST_CHECK_FIXED_CRC )
	{
		slt_test_check_fixed_crc();
	}
	return;
}


static void slt_test_get_crc( void )
{

	/* Check CRC */
	UINT32 crc;
	SLT_RdFL( crc_data0 );
	crc = SLT_Rd( crc_data0 );

#ifdef SLT_AUTO_RESET_DISABLE
	/* CRC Reset */
	SLT_RdFL( crc_reset );
	SLT_Wr( crc_reset, 0xF );
	SLT_WrFL( crc_reset );
	SLT_Wr( crc_reset, 0x0 );
	SLT_WrFL( crc_reset );
#endif

	if( crc_check_count < SLT_CRC_NUM )
	{
		crc_data.data0[crc_check_count] = crc;
	}
	else
	{
		/* CRC Interrupt Disable */
		SLT_RdFL( crc_intr_enable );
		SLT_Wr( crc_intr_enable, 0x0 );
		SLT_WrFL( crc_intr_enable );
		SLT_INFO( "CRC Check Done\n" );
		crc_check_count = 0;
	}
	crc_check_count++;

	return;
}

static void slt_test_check_fixed_crc( void )
{
	UINT32 crc;

	/* Check CRC */
	SLT_RdFL( crc_data0 );
	crc = SLT_Rd( crc_data0 );

#ifdef SLT_AUTO_RESET_DISABLE
	/* CRC Reset */
	SLT_RdFL( crc_reset );
	SLT_Wr( crc_reset, 0xF );
	SLT_WrFL( crc_reset );
	SLT_Wr( crc_reset, 0x0 );
	SLT_WrFL( crc_reset );
#endif

	if( crc_check_count < SLT_CRC_NUM  || crc_count == 0 )
	{
		if( prev_crc != crc )
		{
			SLT_INFO( "#[%u] PREV_CRC = 0x%08X,  NOW_CRC = 0x%08X\n", crc_check_count, prev_crc, crc );
		}
		prev_crc = crc;
	}
	else
	{
		/* CRC Interrupt Disable */
		SLT_RdFL( crc_intr_enable );
		SLT_Wr( crc_intr_enable, 0x0 );
		SLT_WrFL( crc_intr_enable );
		SLT_INFO( "CRC Check Done\n" );
		crc_check_count = 0;
	}	
	crc_check_count++;
	
	return;
}


static irqreturn_t slt_isr( int irq, void *dev_id )
{
	UINT32 intr_status = 0;

	SLT_RdFL( crc_intr_status );
	intr_status = SLT_Rd( crc_intr_status );

	if( intr_status != 0x0 )
	{
		SLT_ISR_Handler();
		
		SLT_RdFL( crc_intr_clear );
		SLT_Wr( crc_intr_clear, 0x1 );
		SLT_WrFL( crc_intr_clear );
	}

	return IRQ_HANDLED;
}


static int slt_open( void )
{
	int ret = 0;

	SLT_PRINT("%s:%d\n", __FUNCTION__,__LINE__);

	SLT_REG = (volatile H15_SLT_REG_T*)ioremap(H15_CRC_BASE, sizeof(H15_SLT_REG_T) );
	ret |= request_irq( H15_IRQ_SLT, (irq_handler_t)slt_isr, IRQF_DISABLED, "SLT", NULL );

	if( ret != RET_OK )
	{
		SLT_ERROR("can't register slt irq\n");
		return RET_ERROR;
	}

	/* CRC Enable */
	SLT_RdFL( crc_enable );
	SLT_Wr( crc_enable, 0x1 );
	SLT_WrFL( crc_enable );


#ifdef SLT_AUTO_RESET_DISABLE
	/* Auto reset disable */
	SLT_RdFL( auto_reset_disable );
	SLT_Wr( auto_reset_disable, 0x1 );
	SLT_WrFL( auto_reset_disable );
#endif


	return RET_OK;
}

static int slt_close(void)
{
	iounmap( SLT_REG );

    free_irq(H15_IRQ_SLT, NULL );

	return RET_OK;
}


/** initialize slt
 *
 */
static int slt_init(void)
{
	SLT_INFO("SLT H15 Init.\n");

	return RET_OK;
}

static void slt_cleanup(void)
{
	SLT_INFO("SLT H15 Cleanup.\n");

	return;
}


static void slt_set_testmode(LX_SLT_TEST_MODE_E *mode)
{
	slt_test_mode = *mode;
	return;
}

static void slt_intr_enable( void )
{
	crc_check_count = 0;
	prev_crc = 0;
	SLT_RdFL( crc_intr_enable );
	SLT_Wr( crc_intr_enable, 0x1 );
	SLT_WrFL( crc_intr_enable );
	
	return;
}

static void slt_intr_disable( void )
{
	SLT_RdFL( crc_intr_enable );
	SLT_Wr( crc_intr_enable, 0x0 );
	SLT_WrFL( crc_intr_enable );
	
	return;
}

static int slt_get_ref_crc( LX_SLT_CRC_DATA_T *crc )
{
	UINT32 intr_status = 0;
	UINT32  count = 0;

	while( 1 )
	{
		msleep( 1000 );
		SLT_RdFL( crc_intr_enable );
		intr_status = SLT_Rd( crc_intr_enable );
		count++;
		if(intr_status == 0x0 || count > 10 )
			break;
	}
	

	if(count>10)
		return RET_ERROR;

	if( intr_status == 0x0 )
	{
		memset( crc, 0, sizeof(LX_SLT_CRC_DATA_T) );
		memcpy( crc, &crc_data, sizeof(LX_SLT_CRC_DATA_T) );
	}

	return RET_OK;
}


static SLT_HW_FUNC_T hw_func =
{
	.init			= slt_init,
	.cleanup		= slt_cleanup,
	.open			= slt_open,
	.close			= slt_close,

	.set_testmode	= slt_set_testmode,
	.intr_enable	= slt_intr_enable,
	.intr_disable	= slt_intr_disable,
	.get_ref_crc	= slt_get_ref_crc,

};

SLT_HW_FUNC_T* SLT_H15_Init(void)
{
	return &hw_func;
}

