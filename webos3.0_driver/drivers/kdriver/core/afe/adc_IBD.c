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


/*! \file ADC_IBDfunc.c
 * \brief ADC module control Drivers linux module
 */


/**
 *	Necessary includes for device drivers
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/delay.h>	/** for mdelay	*/

#include "os_util.h"
#include "afe_drv.h"
#include <linux/interrupt.h>
#include "afe_kapi.h"

#include "resolution.h"
#include "adc_module.h"

/******************************************************************************
 *				DEFINES
 *****************************************************************************/


/**
 *	Global variables of the driver
 */
//extern volatile VPORT_Reg_T *gVportReg;

ST_EXTAU gExtau={0,0,0,0} ;

/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */

/******************************************************************************
 *				Local function
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */






int ADC_IBD_Mesurement(ST_EXTAU *inform , int iIndex)
{

	S_EXTAU_VALUE *value;
	//float up_offset, left_offset;
	//int up,left;

	//int width_offset = TB_WIDTH[iIndex] + 500;//* 99; // ??
	int width_offset = LLPLL_counter_max[iIndex] + 100;//powerory
	//int height_offset = TB_HEIGHT[iIndex] + 500;//* 99; // ??
	int height_offset = TB_SD_EXT_VSIZE[iIndex] + 100;//powerory

	if(width_offset > 4095) width_offset = 4095;
	if(height_offset > 4095) height_offset = 4095;

	value = (S_EXTAU_VALUE *)kmalloc(sizeof(S_EXTAU_VALUE) , GFP_KERNEL);

	// Height
	value->height_offset = height_offset;

	// WIDTH
	value->width_offset = width_offset;

	ADC_DEBUG("IBD: iIndex=[%d], TB_PC[iIndex] = [%d], TB_PI[iIndex]=[%d]\n",iIndex, TB_PC[iIndex], TB_PI[iIndex]);

	if( TB_PC[iIndex] ) {
		// BD On Progressive
		// BD On RGB
		//value->control = 0x7AC7;
		value->control = 0x7fff;
		//gVportReg->ch3reg_extau_level.ch3reg_extau_level = 0x60;// 0x40;
		value->level = 0x60;// 0x40;
	}
	else
	{
		if( TB_PI[iIndex] ) {
			// BD On Progressive
			// BD On Y
			value->control = 0x7AC1;
			//gVportReg->ch3reg_extau_level.ch3reg_extau_level = 0x200;
			value->level = 0x200;

		}
		else{
			// BD On Interlaced
			// BD On Y
			value->control = 0x3AC1;
			//gVportReg->ch3reg_extau_level.ch3reg_extau_level = 0x200;
			value->level = 0x200;

		}
	}

	ADC_DEBUG("%d@%s width_offset %d, height_offset %d, control %x, level %d \n",
		__LINE__ , __func__ , value->width_offset , value->height_offset ,value->control, value->level);


	ADC_SetTAUInform(value);

	OS_MsecSleep( 100 );	//?

	ADC_GetBoundaryInform(inform);

	ADC_DEBUG("%d@%s extup %d, extdown %d, extleft %d, extright %d\n",
	__LINE__ , __func__ , inform->extau_up , inform->extau_down , inform->extau_left , inform->extau_right);
#if 0	//by dws 2010/06/03
	up = (int)inform->extau_up;
	left = (int)inform->extau_left;

	if( TB_PC[iIndex] == 1){
			up_offset =  (float)(inform->extau_down - inform->extau_up - TB_HEIGHT[iIndex])/2.0;
			left_offset = (float)(inform->extau_right - inform->extau_left - TB_WIDTH[iIndex])/1;
			up += (int)(up_offset + 1.5);
			left += (int)(left_offset - 3 );
	}
	else
	{
		if( TB_PI[iIndex] == 1){
			up_offset =  (float)(inform->extau_down - inform->extau_up - TB_HEIGHT[iIndex])/2.0;
			left_offset = (float)(inform->extau_right - inform->extau_left - TB_WIDTH[iIndex])/1;
			up += (int)(up_offset + 7 );
			left += (int)(left_offset -40 );
		}else
		{
			up_offset =  (float)(inform->extau_down - inform->extau_up - TB_HEIGHT[iIndex]/2)/2.0;
			left_offset = (float)(inform->extau_right - inform->extau_left - TB_WIDTH[iIndex])/1.0;
			up += (int)(up_offset + 1.5);
			left += (int)(left_offset - 3 );
		}
	}

	if (up<0){
		 inform->extau_up = (unsigned int)(2048 + up);
	}
	else{
	 inform->extau_up = (unsigned int)(up);
	}
	inform->extau_up &= 0xffff;

	if (left<0) inform->extau_left = 4096  + left;
	else inform->extau_left = left;
	inform->extau_up &= 0xffff;

	inform->extau_left_offset = left_offset;
	inform->extau_up_offset = up_offset;

	AFE_PRINT("%d@%s extup %d, extdown %d, extleft %d, extright %d\n",
	__LINE__ , __func__ , inform->extau_up , inform->extau_down , inform->extau_left , inform->extau_right);
#endif

	kfree(value);
 	return 0;

}



#if 1
int ADC_IBD_Setting(ST_EXTAU *extau , int iIndex)
{

/*
	AFE_PRINT("extau up : %d , down : %d , left : %d , right : %d\n",
		extau->extau_up , extau->extau_down , extau->extau_left , extau->extau_right);

	AFE_PRINT("extau up offset : %8.3f , left offset : %8.3f\n" , extau->extau_up_offset , extau->extau_left_offset);
*/

	memcpy((void *)&gExtau , (void *)extau , sizeof(ST_EXTAU));	// for saving
	//gExtau.extau_up = extau->extau_up;
	//gExtau.extau_left = extau->extau_left;
	//gExtau.extau_down = extau->extau_down;
	//gExtau.extau_right = extau->extau_right;

#if 0
	ADC_SetDE(iIndex , extau->extau_up , extau->extau_left);
#endif
	// will be used at DE
	return 0;
}
#else
int ADC_IBD_Setting(ST_EXTAU *extau , int iIndex)
{
	extau->extau_up = (gPCTable+iIndex)->u16_HStart;
	extau->extau_left = (gPCTable+iIndex)->u16_VStart;

	memcpy((void *)&gExtau , (void *)&extau , sizeof(ST_EXTAU));	// for saving

	ADC_SetDE(iIndex , extau->extau_up , extau->extau_left);

	return 0;
}
#endif

int ADC_IBD_main(int iIndex)
{
	int ret = 0;
	ST_EXTAU extau;

	if(iIndex == -1)
	{
		AFE_PRINT("not be set resolutuin yet\n");
		return -1;
	}

	ADC_DEBUG("%s entered with index[%d]\n", __func__ , iIndex);

	ret = ADC_IBD_Mesurement(&extau , ConvertTable[iIndex]);

	if(ret < 0)
	{
		AFE_ERROR("return value is %d\n",  ret);
		return ret;
	}

	ret = ADC_IBD_Setting(&extau , ConvertTable[iIndex]);

	return ret;
}

int ADC_IBD_GetBackPorch(ST_EXTAU *extau)
{

	memcpy((void *)extau , (void *)&gExtau, sizeof(ST_EXTAU));

	return 0;
}
