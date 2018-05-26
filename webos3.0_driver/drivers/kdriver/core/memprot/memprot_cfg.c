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
 *  main configuration file for memprot device
 *	memprot device will teach you how to make device driver with new platform.
 *
 *  author		sanghyeun.kim
 *  version		1.0
 *  date		2012.08.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <asm/page.h>
#include "memprot_cfg.h"
#include "memprot_drv.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

static void* 	MEMPROT_RegAddr[ENUM_MEMPROT_MAX]={NULL,};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

void 	MEMPROT_ReadViolation(unsigned monitor);
void	MEMPROT_ClearInterrupt(unsigned monitor);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

MONITOR_INFO_T monitors[ENUM_MEMPROT_MAX]= {
#if INCLUDE_KDRV_MEMPROT==0xB0
#include "./memprot_cfg_b0.tab"
#else
#include "./memprot_cfg_a0.tab"
#endif
};


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * Get memory protection monitor name.
 *
 * @param  monitor ENUM_MEMPROT_T;
 * @return return NULL if no match
*/

char*	MEMPROT_GetMonitorName(unsigned monitor)
{
	if(monitor >= ENUM_MEMPROT_AUDIO && monitor < ENUM_MEMPROT_MAX)
		return monitors[monitor].name;
	else
		return NULL;

}

/**
 * Get memory protection monitor register offset.
 *
 * @param  monitor ENUM_MEMPROT_T;
 * @return return 0 if no match
*/

unsigned int	MEMPROT_GetMonitorReg(unsigned monitor)
{
	if(monitor >= ENUM_MEMPROT_AUDIO && monitor < ENUM_MEMPROT_MAX)
		return monitors[monitor].reg_offset;
	else
		return 0;

}

/**
 * Get memory protection monitor interrupt bit.
 *
 * @param  monitor ENUM_MEMPROT_T;
 * @return return 0 if no match
*/

unsigned int	MEMPROT_GetMonitorInt(unsigned monitor)
{
	if(monitor >= ENUM_MEMPROT_AUDIO && monitor < ENUM_MEMPROT_MAX)
		return monitors[monitor].int_bit;
	else
		return 0;

}


/**
 * Set memory protection monitor.
 *
 * Set virtual address of monitor's register to manipulate it
 *
 * @param  monitor ENUM_MEMPROT_T;
*/

void	MEMPROT_SetProtectMonitor(unsigned monitor)
{

	if( ENUM_MEMPROT_AUDIO <= monitor  && monitor < ENUM_MEMPROT_DE_A )
	{
		MEMPROT_RegAddr[monitor]=ioremap(MEMPROT_LBUS_BASE+MEMPROT_GetMonitorReg(monitor), \
			MEMPROT_REG_SIZE);
		MEMPROT_PRINT("[ %-10.10s ] Monitor Address : %#x \n", MEMPROT_GetMonitorName(monitor), \
					MEMPROT_LBUS_BASE+MEMPROT_GetMonitorReg(monitor));
	}
	else if( ENUM_MEMPROT_DE_A <= monitor  && monitor < ENUM_MEMPROT_MAX )
	{
		MEMPROT_RegAddr[monitor]=ioremap(MEMPROT_GBUS_BASE+MEMPROT_GetMonitorReg(monitor), \
			MEMPROT_REG_SIZE);
		MEMPROT_PRINT("[ %-10.10s ] Monitor Address : %#x \n", MEMPROT_GetMonitorName(monitor), \
					MEMPROT_GBUS_BASE+MEMPROT_GetMonitorReg(monitor));
	}

/*
	if(MEMPROT_RegAddr[monitor])
		MEMPROT_PRINT("[ INFO ] %-10.10s monitor is enabled!!!\n",MEMPROT_GetMonitorName(monitor));
	else
		MEMPROT_PRINT("[ ERROR ] %-10.10s monitor is not enabled!!!\n",MEMPROT_GetMonitorName(monitor));
*/
}

/**
 * Release memory protection monitor.
 *
 * @param  monitor ENUM_MEMPROT_T;
*/

void	MEMPROT_RelProtectMonitor(unsigned monitor)
{

	if(MEMPROT_RegAddr[monitor])
	{
		iounmap(MEMPROT_RegAddr[monitor]);
		MEMPROT_RegAddr[monitor]=NULL;
	}

}

/**
 * Show memory protection register value.
 *
 * @param  monitor ENUM_MEMPROT_T;
*/

void	MEMPROT_ShowRegInfo(unsigned monitor)
{
	int i=0;

	if(!MEMPROT_RegAddr[monitor])
	{
		MEMPROT_PRINT("[ ERROR ] %-10.10s  monitor is not enabled !!! \n",MEMPROT_GetMonitorName(monitor));
		return;
	}

	/* show register value*/
	MEMPROT_PRINT("----------------------------------------------\n");
	MEMPROT_PRINT("[ %-10.10s ]  %#x reg value ; %#x \n",MEMPROT_GetMonitorName(monitor), MEMPROT_INTR_EN_OFFSET, \
			ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET));

	for(i= MEMPROT_BASE_OFFSET	; i <= MEMPROT_STATUS1_OFFSET ; i+=4)
		MEMPROT_PRINT("[ %-10.10s ]  %#x reg value ; %#x \n",MEMPROT_GetMonitorName(monitor), i, \
			ioread32(MEMPROT_RegAddr[monitor]+i));


}

/**
 * Set memory protection region.
 *
 * @param	monitor ENUM_MEMPROT_T
 * @param	reg1_s  start address of region 1
 * @param	reg1_e  end address of region 1
 * @param	reg2_s  start address of region 2
 * @param	reg2_e  edn address of region 2
*/

unsigned	MEMPROT_SetProtect(unsigned monitor, unsigned reg1_s, unsigned reg1_e,unsigned reg2_s,unsigned reg2_e)
{
	unsigned status0=0xff;


	/* disable monitor and clear interrupt*/
	if(reg1_s==0xFF && reg2_s==0xFF)
	{
		MEMPROT_ClearInterrupt(monitor);
		MEMPROT_RelProtectMonitor(monitor);
		return status0;
	}

	if(!MEMPROT_RegAddr[monitor])
		MEMPROT_SetProtectMonitor(monitor);
	else
		MEMPROT_PRINT("[ Info ] Address(%-10.10s) : %p\n",MEMPROT_GetMonitorName(monitor),MEMPROT_RegAddr[monitor]);


	// enable interrupt
	iowrite32( MEMPROT_INTR_EN_BIT  | ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET), \
			MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET);

	// set protection region
	iowrite32(((reg2_s & 0xff)<<8)|(reg1_s & 0xff) , MEMPROT_RegAddr[monitor]+MEMPROT_BASE_OFFSET);
	iowrite32(((reg2_e & 0xff)<<8)|(reg1_e & 0xff) , MEMPROT_RegAddr[monitor]+MEMPROT_END_OFFSET);

	//MEMPROT_ShowRegInfo(monitor);

	status0=ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_STATUS0_OFFSET);
	return status0;
}

/**
 * Clear memory protection interrupt .
 *
 * @param	monitor ENUM_MEMPROT_T
 *
*/

void	MEMPROT_ClearInterrupt(unsigned monitor)
{

	if(!MEMPROT_RegAddr[monitor])
	{
		MEMPROT_PRINT("[ ERROR ] %-10.10s  monitor is not enabled !!! \n",MEMPROT_GetMonitorName(monitor));
		return;
	}

	iowrite32((~MEMPROT_INTR_EN_BIT) & ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET), \
			MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET);

	iowrite32( MEMPROT_INTR_CLEAR_BIT  | ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET), \
			MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET);
}


/**
 * Read violation address and violation ID.
 *
 * @param	monitor	ENUM_MEMPROT_T
*/

void MEMPROT_ReadViolation(unsigned monitor)
{

	static unsigned int_count=0;

	unsigned mon_int_st=0; // interrupt status in the  monitor register
	unsigned status0=0xff;
	unsigned status1=0xff;

	if(!MEMPROT_RegAddr[monitor])
	{
		MEMPROT_PRINT("[ ERROR ] %-10.10s  monitor is not enabled !!! \n",MEMPROT_GetMonitorName(monitor));
		return;
	}

	//check interrupt status bit of monitor
	mon_int_st=ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_INTR_EN_OFFSET);
	if(!(mon_int_st & MEMPROT_INTR_STATUS_BIT))return;

	status0=ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_STATUS0_OFFSET);
	status1=ioread32(MEMPROT_RegAddr[monitor]+MEMPROT_STATUS1_OFFSET);

	MEMPROT_PRINT("[ violation ] monitor : %-10.10s, address  : %#12x, ID  : %#12x  \n", \
							MEMPROT_GetMonitorName(monitor),status0,status1);

	/* Display symbol information of violation command */
	/*
	struct page * v_page=0;
	void*	vaddr=0;

	v_page=(pfn_to_page(status0 >> PAGE_SHIFT));

	vaddr=kmap(v_page);
	MEMPROT_PRINT("[ violaton  ] Vaddr : %p,  count : %d \n", vaddr,  v_page->_count);
	if(vaddr)
		__print_symbol("[ violation ] symbol info. of voilation command :  %s \n", (unsigned long)vaddr);
	kunmap(v_page);
	*/



	// for debugging
	++int_count;

	// disable interrupt of memory protection
	if(int_count==20)
	{
		MEMPROT_ClearInterrupt(monitor);
		int_count=0;
	}


}


/** @} */

