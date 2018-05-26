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


/*! \file ADC_AGOC.c
 * \brief ADC gain/offset control Drivers linux module
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

#include "adc_module.h"

/******************************************************************************
 *				DEFINES
 *****************************************************************************/
#define	ADC_AGC_NUM	30//99
#define	ADC_L9_AGC_NUM		6
#define	ADC_L9_AGC_AVG_NUM	3
#define SAME_GAIN_MAX 5
#define SAME_OFFSET_MAX 5

//120103 wonsik.do : removed floating point in kernel driver
//#define	GAP_TARGET	0.2
//#define	GAP_TARGET	200
//120507 wonsik.do : fix GAP_TARGET value
#define	GAP_TARGET	3276 	// 0.2 * 2^14

#if 0
#define	fabsf(f) ({	float _f=(f);	\
			(_f < 0.0) ? -_f : _f;	\
		})
#endif

/**
 *	Global variables of the driver
 */
//extern volatile VPORT_Reg_T *gVportReg;
extern LX_AFE_ADC_INPUT_SOURCE_TYPE_T gADC_type;

/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */


//#define weight (2.52/2.5)
#define weight 0.986



/******************************************************************************
 *				Local function
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */


//120103 wonsik.do : removed floating point in kernel driver
/*
float	fabsf(float f)
{
	if(f < 0.0)
		return -f;
	else
		return f;
}
*/
/**
 *
 * search gain value for R,G,B(L9)
 *
 * @param	ST_GAIN_INFORM
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_L9_AGC_Measurement (LX_AFE_ADC_CALIB_INFO_T *calib_param, ST_GAIN_INFORM *gain)
{
	int ret = 0;
	S_GAIN_TARGET gain_ire73 , gain_ire950 , gap;
	S_OFFSET_INFORM offset;
	static bool bStatusR,bStatusG,bStatusB;
	UINT64 temp, temp_div;
	S_GAIN_TARGET target;
	int iRepeatCount = 0;
	ST_GAIN_INFORM *gain_temp;
//	ST_GAIN_INFORM *gain_prev; //by dws
	ST_GAIN_INFORM *gain_sum; //by dws
	//120103 wonsik.do : removed floating point in kernel driver
//	UINT8 same_count = 0;
	UINT32	max_gain_value;
	int offset_adjust_count;

	max_gain_value = 0x3FFF;		// For L9 14bit Gain

	// initial code
	bStatusR = 1;
	bStatusG = 1;
	bStatusB = 1;

	gain_temp = (ST_GAIN_INFORM *)kmalloc(sizeof(ST_GAIN_INFORM),GFP_KERNEL);

	//by dws
	//gain_prev = (ST_GAIN_INFORM *)kmalloc(sizeof(ST_GAIN_INFORM),GFP_KERNEL);
	//memset ( gain_prev, 0, sizeof(ST_GAIN_INFORM));

	//by dws
	gain_sum = (ST_GAIN_INFORM *)kmalloc(sizeof(ST_GAIN_INFORM),GFP_KERNEL);
	memset ( gain_sum, 0, sizeof(ST_GAIN_INFORM));

	if( (calib_param->TargetForRGain > 0) || (calib_param->TargetForGGain > 0) || (calib_param->TargetForBGain > 0))
	{
		ADC_CALIB_DEBUG("User Target Value is set\n");

		target.gain_R = calib_param->TargetForRGain;
		target.gain_G = calib_param->TargetForGGain;
		target.gain_B = calib_param->TargetForBGain;
	}
	else
	{

		// By input signal type
		//if(ADC_GetSelectedPort() < 2)	// component
		if( gADC_type == LX_ADC_INPUT_SOURCE_YPBPR)
		{
			//110418
			//target.gain_R = 768.0*0.96;
			//target.gain_G = 750.0*0.96;
			//target.gain_B = 768.0*0.96;
			//	target.gain_R = 768.0*weight;
			//	target.gain_G = 750.0*weight;
			//	target.gain_B = 768.0*weight;
			if ( lx_chip_rev( ) < LX_CHIP_REV( H13, A0 ) )
			{
				//110428 by powerory
				// Target Value for MSPG
				target.gain_R = 761;
				target.gain_G = 745;
				target.gain_B = 761;
			}
			else if ( lx_chip_rev( ) < LX_CHIP_REV( M14, A0 ) )	// for H13B0
			{
				if(lx_chip_plt() != LX_CHIP_PLT_FHD)
				{
					//131231 : for 2014 H13 UD Board (VG-870B)
					target.gain_R = 770;
					target.gain_G = 756;
					target.gain_B = 770;
				}
				else
				{
					//120831 New Target Value for VG-870A
					target.gain_R = 753;
					target.gain_G = 735;
					target.gain_B = 753;
				}
			}
			else	//for M14, H14 
			{
				//120831 New Target Value for VG-870A
				target.gain_R = 753;
				target.gain_G = 735;
				target.gain_B = 753;
			}
			ADC_CALIB_DEBUG("AGC : input is comp\n");
		}
		else				// RGB
		{
			if ( lx_chip_rev( ) < LX_CHIP_REV( M14, A0 ) && (lx_chip_plt() != LX_CHIP_PLT_FHD) )
			{
				//140129 : for 2014 H13 UD Board (VG-870B) at PQ TEST Room
				target.gain_R = 883;	//872
				target.gain_G = 883;	//872
				target.gain_B = 883;	//872
			}
			else if ( lx_chip_rev( ) < LX_CHIP_REV( M14, C0 ) )	// for H13/M14A0/M14B0
			{
				//target.gain_R = 877.7*weight;
				//target.gain_G = 877.7*weight;
				//target.gain_B = 877.7*weight;
				//110428 by powerory
				target.gain_R = 857;
				target.gain_G = 857;
				target.gain_B = 857;
			}
			else if ( lx_chip_rev( ) < LX_CHIP_REV( H15, A0 ) )	// for M14C0
			{
				//110428 by powerory
				target.gain_R = 835;
				target.gain_G = 835;
				target.gain_B = 835;
			}
			else	// for H15
			{
				//target.gain_R = 877.7*weight;
				//target.gain_G = 877.7*weight;
				//target.gain_B = 877.7*weight;
				//110428 by powerory
				target.gain_R = 857;
				target.gain_G = 857;
				target.gain_B = 857;
			}
			ADC_CALIB_DEBUG("AGC : input is RGB\n");
		}
	}
	ADC_CALIB_DEBUG("ADC Target R[%d], G[%d], B[%d]\n", (int)target.gain_R, (int)target.gain_G, (int)target.gain_B);

	//120103 wonsik.do : removed floating point in kernel driver
	// target value should be increased
	target.gain_R = target.gain_R << 14;
	target.gain_G = target.gain_G << 14;
	target.gain_B = target.gain_B << 14;

	ADC_AGOCInit();
	ADC_CALIB_DEBUG("------------------------[L9 Auto Gain start]----------------------------\n");

	// Set ADC pre offset value to 0x800 (default)
	offset.offset_R = ADC_L9_R_PRE_OFFSET_VALUE;
	offset.offset_G = ADC_L9_G_PRE_OFFSET_VALUE;
	offset.offset_B = ADC_L9_B_PRE_OFFSET_VALUE;

	ADC_SetOffsetRGB( &offset); // added by won.hur(won.hur@lge.com) @2011.08.17
	//ADC_L9Ax_SetOffsetRGB( &offset);

	do{

		//AFE_PRINT("IRE level 73\n");
		// set IRE73 : first step
		for(offset_adjust_count  =0; offset_adjust_count < 3; offset_adjust_count ++)
		{
			ADC_SetIRELevel(IRE73 , &gain_ire73);

			ADC_CALIB_DEBUG("AGC gain R:73ire [%d], G:73ire [%d], B:73ire [%d] \n" , (UINT32)(gain_ire73.gain_R) , (UINT32)(gain_ire73.gain_G), (UINT32)(gain_ire73.gain_B));

			if( ( (gain_ire73.gain_R >> 14) == 0) || ( (gain_ire73.gain_G >> 14) == 0) || ( (gain_ire73.gain_B >> 14) == 0) )
			{
				ADC_CALIB_DEBUG("!!! Gain level is too low !!! \n");

				ADC_SetIRELevel(IRE950 , &gain_ire950);

				ADC_CALIB_DEBUG("AGC gain R:950ire [%d] G:950ire [%d] B:950ire [%d] \n" , (UINT32)(gain_ire950.gain_R) , (UINT32)(gain_ire950.gain_G), (UINT32)(gain_ire950.gain_B));

				ADC_GetOffsetRGB(&offset); // added by won.hur(won.hur@lge.com) @2011.08.17
				//ADC_L9Ax_GetOffsetRGB(&offset);

				if( (gain_ire73.gain_R>>14) == 0)
				{
					offset.offset_R += (1023 - (gain_ire950.gain_R>>14)) >> 1;
				}
				if( (gain_ire73.gain_G>>14) == 0)
				{
					offset.offset_G += (1023 - (gain_ire950.gain_G>>14)) >> 1;
				}
				if( (gain_ire73.gain_B>>14) == 0)
				{
					offset.offset_B += (1023 - (gain_ire950.gain_B>>14)) >> 1;
				}

				ADC_CALIB_DEBUG("Set Offset to R:[0x%x], G:[0x%x], B:[0x%x] \n" , offset.offset_R , offset.offset_G, offset.offset_B);
				ADC_SetOffsetRGB(&offset); // added by won.hur(won.hur@lge.com) @2011.08.17
				//ADC_L9Ax_SetOffsetRGB(&offset);

		//		ADC_SetIRELevel(IRE73 , &gain_ire73);
		//		ADC_APA_DEBUG("Adjusted AGC gain R:73ire [%d], G:73ire [%d], B:73ire [%d] \n" , (int)(gain_ire73.gain_R) , (int)(gain_ire73.gain_G), (int)(gain_ire73.gain_B));
			}
			else
				break;
		}

		//AFE_PRINT("IRE level 950\n");
		// set IRE950 : first step
		for(offset_adjust_count  =0; offset_adjust_count < 3; offset_adjust_count ++)
		{
			ADC_SetIRELevel(IRE950 , &gain_ire950);

			ADC_CALIB_DEBUG("AGC gain R:950ire [%d] G:950ire [%d] B:950ire [%d] \n" , (UINT32)(gain_ire950.gain_R) , (UINT32)(gain_ire950.gain_G), (UINT32)(gain_ire950.gain_B));

			if( ( (gain_ire950.gain_R>>14) >= 1023) || ( (gain_ire950.gain_G>>14) >= 1023) || ( (gain_ire950.gain_B>>14) >= 1023) )
			{
				ADC_CALIB_DEBUG("!!! Gain too Big !!! \n");

				ADC_SetIRELevel(IRE73 , &gain_ire73);

				ADC_CALIB_DEBUG("AGC gain R:73ire [%d], G:73ire [%d], B:73ire [%d] \n" , (UINT32)(gain_ire73.gain_R) , (UINT32)(gain_ire73.gain_G), (UINT32)(gain_ire73.gain_B));

				ADC_GetOffsetRGB(&offset); // added by won.hur(won.hur@lge.com) @2011.08.17
				//ADC_L9Ax_GetOffsetRGB(&offset);

				if( (gain_ire950.gain_R>>14) >= 1023)
					offset.offset_R -= (gain_ire73.gain_R>>14) >> 1;
				if( (gain_ire950.gain_G>>14) >= 1023)
					offset.offset_G -= (gain_ire73.gain_G>>14) >> 1;
				if( (gain_ire950.gain_B>>14) >= 1023)
					offset.offset_B -= (gain_ire73.gain_B>>14) >> 1;

				ADC_CALIB_DEBUG("Set Offset to R:[0x%x], G:[0x%x], B:[0x%x] \n" , offset.offset_R , offset.offset_G, offset.offset_B);
				ADC_SetOffsetRGB(&offset); // added by won.hur(won.hur@lge.com) @2011.08.17
				//ADC_L9Ax_SetOffsetRGB(&offset);

//				ADC_SetIRELevel(IRE73 , &gain_ire73);
//				ADC_APA_DEBUG("Adjusted AGC gain R:950ire [%d], G:950ire [%d], B:950ire [%d] \n" , (int)(gain_ire950.gain_R) , (int)(gain_ire950.gain_G), (int)(gain_ire950.gain_B));
			}
			else
				break;
		}


		ADC_GetGainRGB(gain_temp);

		if(bStatusR)
		{
			// difference IRE73 gain from IRE950 gain
			//120103 wonsik.do : removed floating point in kernel driver
			//gap.gain_R = fabsf(gain_ire73.gain_R - gain_ire950.gain_R);
			gap.gain_R = abs(gain_ire73.gain_R - gain_ire950.gain_R);
			//AFE_PRINT("AGC gain R: 73ire %.5f , 950ire %.f\n" , gain_ire73.gain_R , gain_ire950.gain_R);
			/*
			AFE_PRINT("AGC gain R: 73ire %d.%d , 950ire %d.%d , gap %d.%d\n" , (int)gain_ire73.gain_R , (int)(( (float)gain_ire73.gain_R - (float)(int)(gain_ire73.gain_R) )*10000.0)
			,(int)gain_ire950.gain_R , (int)(( (float)gain_ire950.gain_R - (float)(int)(gain_ire950.gain_R) )*10000.0)
			, (int)gap.gain_R , (int)(( (float)gap.gain_R - (float)(int)(gap.gain_R) )*10000.0) );
			*/
			ADC_CALIB_DEBUG("AGC gain R: 73ire %d, 950ire %d, gap %d\n" , (int)gain_ire73.gain_R ,(int)gain_ire950.gain_R , (int)gap.gain_R );

			if(abs(target.gain_R - gap.gain_R) < GAP_TARGET)
			{
				bStatusR = 0;	//  done the adjust!!
				ADC_CALIB_DEBUG("R gain adjust done!!\n");
			}
			else
			{
				// set the value again
				temp = (int)gain_temp->gain_R;

				ADC_CALIB_DEBUG("before gain R [0x%x]\n", (UINT32)temp);
				ADC_CALIB_DEBUG("GAP R[%d]\n", (int)gap.gain_R );
				ADC_CALIB_DEBUG("Target R[%d]\n", (int)target.gain_R);

					if(gap.gain_R > 0)
					{

						//120103 wonsik.do : removed floating point in kernel driver
						//gain_temp->gain_R = ((target.gain_R * temp) / gap.gain_R )  ;
						temp_div = (target.gain_R * temp);
						do_div(temp_div, gap.gain_R);
						gain_temp->gain_R = temp_div;
					}
						//gain_temp->gain_R = ((target.gain_R + 512.0) / (gap.gain_R + 512.0))  * temp;
						//gain_temp->gain_R = ((target.gain_R + 512.0) / (gap.gain_R + 512.0))  * 4096.0;

				if(gain_temp->gain_R >= max_gain_value) gain_temp->gain_R = max_gain_value;
			}
		}
	//	(int)(dx) , (int)(( (float)dx - (float)z )*10000.0) )
		if(bStatusG)
		{
			// difference IRE73 gain from IRE950 gain
			//120103 wonsik.do : removed floating point in kernel driver
			//gap.gain_G = fabsf(gain_ire73.gain_G - gain_ire950.gain_G);
			gap.gain_G = abs(gain_ire73.gain_G - gain_ire950.gain_G);
		//	AFE_PRINT("AGC gain G: 73ire %.5f , 950ire %.f\n" , gain_ire73.gain_G , gain_ire950.gain_G);

			//120103 wonsik.do : removed floating point in kernel driver
			/*
			AFE_PRINT("AGC gain G: 73ire %d.%d , 950ire %d.%d\n" , (int)gain_ire73.gain_G , (int)(( (float)gain_ire73.gain_G - (float)(int)(gain_ire73.gain_G) )*10000.0)
			,(int)gain_ire950.gain_G , (int)(( (float)gain_ire950.gain_G - (float)(int)(gain_ire950.gain_G) )*10000.0));
			*/
			ADC_CALIB_DEBUG("AGC gain G: 73ire %d , 950ire %d, gap %d\n" , (int)gain_ire73.gain_G , (int)gain_ire950.gain_G, (UINT32)gap.gain_G);

			if(abs(target.gain_G - gap.gain_G) < GAP_TARGET)
			{
				bStatusG = 0;	//  done the adjust!!
				ADC_CALIB_DEBUG("G gain adjust done!!\n");
			}
			else
			{
				// set the value again
				temp = (int)gain_temp->gain_G;

				ADC_CALIB_DEBUG("before gain G [0x%x]\n",(UINT32)temp);
				ADC_CALIB_DEBUG("GAP G[%d]\n", (UINT32)gap.gain_G );
				ADC_CALIB_DEBUG("Target G[%d]\n", (UINT32)target.gain_G);

					if(gap.gain_G > 0)
					{
						//120103 wonsik.do : removed floating point in kernel driver
						//gain_temp->gain_G = ((target.gain_G  * temp)/ (gap.gain_G )) ;
						temp_div = (target.gain_G * temp);
						do_div(temp_div, gap.gain_G);
						gain_temp->gain_G = temp_div;
						//gain_temp->gain_G = ((target.gain_G + 512.0) / (gap.gain_G + 512.0))  * temp;
						//gain_temp->gain_G = ((target.gain_G + 512.0) / (gap.gain_G + 512.0))  * 4096.0;
					}

				if(gain_temp->gain_G >= max_gain_value) gain_temp->gain_G = max_gain_value;
			}


		}

		if(bStatusB)
		{
			// difference IRE73 gain from IRE950 gain
			gap.gain_B = abs(gain_ire73.gain_B - gain_ire950.gain_B);
			//AFE_PRINT("AGC gain B: 73ire %.5f , 950ire %.f\n" , gain_ire73.gain_B , gain_ire950.gain_B);
			ADC_CALIB_DEBUG("AGC gain B: 73ire %d , 950ire %d\n" ,(UINT32) gain_ire73.gain_B , (UINT32)gain_ire950.gain_B);
			if(abs(target.gain_B - gap.gain_B) < GAP_TARGET)
			{
				bStatusB = 0;	//  done the adjust!!
				ADC_CALIB_DEBUG("B gain adjust done!!\n");
			}
			else
			{

				// set the value again
				temp = gain_temp->gain_B;
				ADC_CALIB_DEBUG("before gain B [0x%x]\n",(UINT32)temp);
				ADC_CALIB_DEBUG("GAP B[%d]\n", (UINT32)gap.gain_B );
				ADC_CALIB_DEBUG("Target B[%d]\n", (UINT32)target.gain_B);

					if(gap.gain_B > 0)
					{
						//120103 wonsik.do : removed floating point in kernel driver
						//gain_temp->gain_B = ((target.gain_B * temp) / (gap.gain_B)) ;
						temp_div = (target.gain_B * temp);
						do_div(temp_div, gap.gain_B);
						gain_temp->gain_B = temp_div;
						//gain_temp->gain_B = ((target.gain_B + 512.0) / (gap.gain_B + 512.0))  * temp;
						//gain_temp->gain_B = ((target.gain_B + 512.0) / (gap.gain_B + 512.0))  * 4096.0;
					}

				if(gain_temp->gain_B >= max_gain_value) gain_temp->gain_B = max_gain_value;
			}

		}

		ADC_CALIB_DEBUG("#Adjust R[0x%x] G[0x%x] B[0x%x]\n", gain_temp->gain_R, gain_temp->gain_G,gain_temp->gain_B);
		ADC_SetGainRGB(gain_temp);

		if( iRepeatCount >= (ADC_L9_AGC_NUM - ADC_L9_AGC_AVG_NUM))
		{
			gain_sum->gain_R += gain_temp->gain_R;
			gain_sum->gain_G += gain_temp->gain_G;
			gain_sum->gain_B += gain_temp->gain_B;
		}

		if( (bStatusR == 0)&&(bStatusG == 0)&& (bStatusB == 0) ) { 
			ADC_CALIB_DEBUG("### ALL Gain Converged in [%d]th loop !!! ###\n", iRepeatCount);
			break;
		}
		/*
		if( memcmp(gain_temp, gain_prev, sizeof(ST_GAIN_INFORM)) == 0)
		{
			same_count ++;
			if(same_count > SAME_GAIN_MAX)
			{
				AFE_PRINT("Gain Converged !!!\n");
				break;
			}
		}
		memcpy( (void *)gain_prev , (void *)gain_temp , sizeof(ST_GAIN_INFORM));
		*/
		ADC_CALIB_DEBUG("------------------------[%d]----------------------------\n", iRepeatCount);

	}while(++iRepeatCount < ADC_L9_AGC_NUM);

	ADC_GetGainRGB(gain);

	if( bStatusR != 0) {
		ADC_CALIB_DEBUG("### Red Gain not converged : Use average value ###\n");
		gain->gain_R = gain_sum->gain_R/(ADC_L9_AGC_AVG_NUM);
	}

	if( bStatusG != 0) {
		ADC_CALIB_DEBUG("### Green Gain not converged : Use average value ###\n");
		gain->gain_G = gain_sum->gain_G/(ADC_L9_AGC_AVG_NUM);
	}

	if( bStatusB != 0)  {
		ADC_CALIB_DEBUG("### Blue Gain not converged : Use average value ###\n");
		gain->gain_B = gain_sum->gain_B/(ADC_L9_AGC_AVG_NUM);
	}

#ifdef	SUPPORT_SCART_RGB_ULTRA_BLACK
	if( gADC_type != LX_ADC_INPUT_SOURCE_YPBPR)
	{
		ADC_CALIB_DEBUG("### SCART RGB Gain Adjust ###\n");
		ADC_CALIB_DEBUG("Before Gain RESULT R[%x] G[%x] B[%x]\n", gain->gain_R, gain->gain_G,gain->gain_B);

		gain->gain_R = gain->gain_R * 100 / 104;
		gain->gain_G = gain->gain_G * 100 / 104;
		gain->gain_B = gain->gain_B * 100 / 104;
	}
#endif

	ADC_SetGainRGB(gain);

	AFE_PRINT("Auto Gain RESULT R[%x] G[%x] B[%x]\n", gain->gain_R, gain->gain_G,gain->gain_B);

	/*
	if( (gain->gain_R > 0x3a0) || (gain->gain_R < 0x100) || (gain->gain_G > 0x3a0) || (gain->gain_G < 0x100) || (gain->gain_B > 0x3a0) || (gain->gain_B < 0x100) )
	{
		AFE_PRINT("Gain Result is too big or too small\n");
		ret = -1;
	}
	*/

#ifdef	SUPPORT_SCART_RGB_ULTRA_BLACK
	if( gADC_type != LX_ADC_INPUT_SOURCE_YPBPR)
	{
		offset.offset_R = ADC_L9_R_PRE_OFFSET_VALUE + SCART_RGB_OFFSET_ADD;
		offset.offset_G = ADC_L9_G_PRE_OFFSET_VALUE + SCART_RGB_OFFSET_ADD;
		offset.offset_B = ADC_L9_B_PRE_OFFSET_VALUE + SCART_RGB_OFFSET_ADD;
	}
	else
	{
		offset.offset_R = ADC_L9_R_PRE_OFFSET_VALUE;
		offset.offset_G = ADC_L9_G_PRE_OFFSET_VALUE;
		offset.offset_B = ADC_L9_B_PRE_OFFSET_VALUE;
	}
#else
	// Set ADC pre offset value to 0x800 (default)
	offset.offset_R = ADC_L9_R_PRE_OFFSET_VALUE;
	offset.offset_G = ADC_L9_G_PRE_OFFSET_VALUE;
	offset.offset_B = ADC_L9_B_PRE_OFFSET_VALUE;
#endif

	ADC_SetOffsetRGB(&offset);
	//ADC_L9Ax_SetOffsetRGB( &offset);


	kfree(gain_temp);
	//kfree(gain_prev);
	kfree(gain_sum);
	return ret;
}

int ADC_AOC_MeasurementByIRE(void)
{
	int ret = 0;
	S_GAIN_TARGET offset_ire0;
	static bool bStatusR ,bStatusG ,bStatusB ;
	int iRepeatCount = 0;
	S_OFFSET_INFORM *offset;
	S_OFFSET_INFORM *offset_prev;
	int port;
	UINT8 same_count=0;

	offset = (S_OFFSET_INFORM *)kmalloc(sizeof(S_OFFSET_INFORM),GFP_KERNEL);
	offset_prev = (S_OFFSET_INFORM *)kmalloc(sizeof(S_OFFSET_INFORM),GFP_KERNEL);

	memset ( offset_prev, 0, sizeof(S_OFFSET_INFORM));

	ADC_AGOCInit();

	bStatusR =1;
	bStatusG =1;
	bStatusB =1;

	port = ADC_GetSelectedPort();

	do{

		if(port < 2)
			ADC_SetIRELevel(IRE0_COMP , &offset_ire0);
		else
			ADC_SetIRELevel(IRE0_RGB , &offset_ire0);

		ADC_GetOffsetRGB(offset);
		AFE_PRINT("Get offset R[%x] , G[%x] , B[%x]\n",offset->offset_R,offset->offset_G, offset->offset_B );

		switch(port)
		{
			case 0:
			case 1:
			{

				if(bStatusR)
				{
					//120103 wonsik.do : removed floating point in kernel driver
					if( abs(offset_ire0.gain_R - 513750) < 100) bStatusR = 0;
					else offset->offset_R +=  (int)(513750 -offset_ire0.gain_R+750);

					if(offset->offset_R >= 0x3ff) offset->offset_R = 0x3ff;

					offset->offset_R &= 0x3ff;
				}
				if(bStatusG)
				{
	//120103 wonsik.do : removed floating point in kernel driver
					if( abs(offset_ire0.gain_G - 64000) < 100 ) bStatusG = 0;
					else offset->offset_G +=  (int)(64000 - offset_ire0.gain_G+750);

					if(offset->offset_G >= 0x3ff) offset->offset_G = 0x3ff;

					offset->offset_G &= 0x3ff;
				}
				if(bStatusB)
				{
	//120103 wonsik.do : removed floating point in kernel driver
					if( abs(offset_ire0.gain_B - 513750) < 100 ) bStatusB = 0;
					else offset->offset_B +=  (int)(513750 -offset_ire0.gain_B+750);

					if(offset->offset_B >= 0x3ff) offset->offset_B = 0x3ff;

					offset->offset_B &= 0x3ff;
				}

			break;
			}
			case 2:
			{
				if(bStatusR)
				{
					if(offset_ire0.gain_R == 0)
					{
					 	offset->offset_R += 128;
					 }
					else if(offset_ire0.gain_R < 150 )
					{
						offset->offset_R++;
					}
					else if(offset_ire0.gain_R > 1000 )
					{

					 	offset->offset_R -= (int)(offset_ire0.gain_R - 1000);

					}
					else if(offset_ire0.gain_R > 500 )
					{
						offset->offset_R--;
					}
					else
					{
						bStatusR = 0;
					}

					if(offset->offset_R >= 0x3ff) offset->offset_R = 0x3ff;
				}

				if(bStatusG)
				{
					if(offset_ire0.gain_G == 0)
					{
					 	offset->offset_G += 128;
					 }
					else if(offset_ire0.gain_G < 150 )
					{
						 offset->offset_G++;
					}
					else if(offset_ire0.gain_G > 1000 )
					{

					 	offset->offset_G -= (int)(offset_ire0.gain_G - 1000);

					}
					else if(offset_ire0.gain_G > 500 )
					{
						offset->offset_G--;
					}
					else{
						bStatusG = 0;
					}

					if(offset->offset_G >= 0x3ff) offset->offset_G = 0x3ff;

				}

				if(bStatusB)
				{
					if(offset_ire0.gain_B == 0)
					{
						offset->offset_B += 128;

					}
					else if(offset_ire0.gain_B < 150 )
					{
						 offset->offset_B++;
					}
					else if(offset_ire0.gain_B > 1000 )
					{
					 	offset->offset_B -= (int)(offset_ire0.gain_B - 1000);

					}
					else if(offset_ire0.gain_B > 500 )
					{
						offset->offset_B--;
					}
					else{
						bStatusB = 0;
					}

					if(offset->offset_B >= 0x3ff) offset->offset_B = 0x3ff;

				}
			break;
			}
		}

		ADC_SetOffsetRGB(offset);

		AFE_PRINT("Set offset R[%x] , G[%x] , B[%x]\n",offset->offset_R,offset->offset_G, offset->offset_B );

		if( (bStatusR == 0)&&(bStatusG == 0)&& (bStatusB == 0) ){
			AFE_PRINT("OFFSET adjust done\n");
			break;	// when RGB finish adjust...
		}

		if( memcmp(offset, offset_prev, sizeof(S_OFFSET_INFORM)) == 0)
		{
			same_count ++;
			if(same_count > SAME_OFFSET_MAX)
			{
				AFE_PRINT("Offset Converged !!!\n");
				break;
			}
		}
		memcpy( (void *)offset_prev , (void *)offset , sizeof(S_OFFSET_INFORM));

	}while(iRepeatCount++ < ADC_AGC_NUM);

	if( (offset->offset_R > 0x300) || (offset->offset_R < 0x100) || (offset->offset_G > 0x300) || (offset->offset_G < 0x100) || (offset->offset_B > 0x300) || (offset->offset_B < 0x100) )
	{
		AFE_PRINT("Offset result is too big or too small\n");
		ret = -1;
	}
	else
	{
				//by powerory
		if(ADC_GetSelectedPort() < 2)	// component
		{
				//offset->offset_R -= 3;
				//offset->offset_G -= 3;
				//offset->offset_B -= 3;
			// 110428 by powerory
				offset->offset_R -= 0;
				offset->offset_G -= 0;
				offset->offset_B -= 0;
		}
		else
		{
				offset->offset_R -= 7;
				offset->offset_G -= 7;
				offset->offset_B -= 7;
		}
				ADC_SetOffsetRGB(offset);

				AFE_PRINT("Set offset R[%x] , G[%x] , B[%x]\n",offset->offset_R,offset->offset_G, offset->offset_B );
	}

	kfree(offset);
	kfree(offset_prev);

	return ret;


}

int ADC_AOC_InitRange(unsigned short posx, unsigned short posy , unsigned short sizex , unsigned short sizey)
{
	S_RGB_POSITION *pos;
	S_RGB_SIZE *size;

	pos = (S_RGB_POSITION *)kmalloc(sizeof(S_RGB_POSITION) , GFP_KERNEL);
	size = (S_RGB_SIZE *)kmalloc( sizeof(S_RGB_SIZE), GFP_KERNEL);

	pos->width_r = posx;
	pos->width_g = posx;
	pos->width_b = posx;
	pos->height_r = posy;
	pos->height_g = posy;
	pos->height_b = posy;
	// blank area setting

	size->width_r = (unsigned int)sizex;
	size->width_g = (unsigned int)sizex;
	size->width_b = (unsigned int)sizex;
	size->height_r = (unsigned int)sizey;
	size->height_g = (unsigned int)sizey;
	size->height_b = (unsigned int)sizey;

	ADC_SetRGBSize(size);
	ADC_SetRGBPosition(pos);

	kfree(pos);
	kfree(size);

	return 0;
}


int ADC_AOC_Measurement(void)
{
#if 0
	static bool bStatusR ,bStatusG ,bStatusB ;
	int iRepeatCount = 0;
	S_RGB_SUM *sum;
	int i, port;
	S_OFFSET_INFORM *offset, *offset1, *offset2;

	double ire_avg_r, ire_avg_g, ire_avg_b;

	// initial code
	bStatusR = 1;
	bStatusG = 1;
	bStatusB = 1;

	sum = (S_RGB_SUM *)kmalloc(sizeof(S_RGB_SUM),GFP_KERNEL);
	offset = (S_OFFSET_INFORM *)kmalloc(sizeof(S_OFFSET_INFORM),GFP_KERNEL);
	offset1 = (S_OFFSET_INFORM *)kmalloc(sizeof(S_OFFSET_INFORM),GFP_KERNEL);
	offset2 = (S_OFFSET_INFORM *)kmalloc(sizeof(S_OFFSET_INFORM),GFP_KERNEL);

	ADC_AOC_InitRange(0x40 , 0xa , 0xa , 0xa);
	//by dws : remove mdelay
	//mdelay(200);
	OS_MsecSleep(200);
	port = ADC_GetSelectedPort();

	do{
		//120103 wonsik.do : removed floating point in kernel driver
		//ire_avg_r = 0.0;
		//ire_avg_g = 0.0;
		//ire_avg_b = 0.0;
		ire_avg_r = 0.0;
		ire_avg_g = 0.0;
		ire_avg_b = 0.0;

		//by dws : remove mdelay
		//mdelay(400);
		OS_MsecSleep(400);

		// sum RGB average
		for(i=0;i<100;i++)
		{
			ADC_GetSumRGB(sum);
			ire_avg_r += (double)sum->sum_R;
			ire_avg_g += (double)sum->sum_G;
			ire_avg_b += (double)sum->sum_B;
		}

		AFE_PRINT("SUM R[%d] , G[%d] , B[%d]\n", sum->sum_R , sum->sum_G, sum->sum_B);

		ire_avg_r /= 10000.0;
		ire_avg_g /= 10000.0;
		ire_avg_b /= 10000.0;

		ADC_GetOffsetRGB(offset);

		AFE_PRINT("Get offset R[%x] , G[%x] , B[%x]\n",offset->offset_R,offset->offset_G, offset->offset_B );

		switch(port){
			case 0:		// component 0
			case 1:		// component 1

				if(bStatusR)
				{
					//3rd Step : Comparing Component는 512로
					if( fabsf(ire_avg_r - 513.75) < 0.05 ) bStatusR = 0;					
					else offset->offset_R +=  (int)(513.75 -ire_avg_r+0.5);
				}

				if(bStatusG)
				{
					//3rd Step : Comparing Component는 512로
					if( fabsf(ire_avg_g - 64.0) < 0.05 ) bStatusG = 0;
					else offset->offset_G +=  (int)(513.75 - ire_avg_g+0.5);
				}

				if(bStatusB)
				{
					//3rd Step : Comparing Component는 512로
					if( fabsf(ire_avg_b - 513.75) < 0.05 ) bStatusB = 0;					
					else offset->offset_B +=  (int)(513.75 -ire_avg_b+0.5);

				}

			break;
			case 2:		// RGB
				if(bStatusR)
				{
					if(ire_avg_r ==0.0) offset->offset_R += 256;
					else{
						if(ire_avg_r <= 0.15) offset->offset_R++;
						else if(ire_avg_r >= 0.25) offset->offset_R -= ( int)(ire_avg_r+0.5);// 
						else bStatusR = 0;
					}
				}

				if(bStatusG)
				{
					if(ire_avg_g ==0.0) offset->offset_G += 256;
					else
					{
						if(ire_avg_g <= 0.15) offset->offset_G++;
						else if(ire_avg_g >= 0.25) offset->offset_G -= ( int)(ire_avg_g+0.5);// 
						else bStatusG = 0;
					}
				}

				if(bStatusB)
				{
					if(ire_avg_b ==0.0) offset->offset_B += 256;
					else
					{
						if(ire_avg_b <= 0.15) offset->offset_B++;
						else if(ire_avg_b >= 0.25) offset->offset_B -= ( int)(ire_avg_b+0.5);// 
						else bStatusB = 0;
					}

				}

			break;
			default:
				AFE_PRINT("%d:%s abnormal parameter\n",__LINE__ , __func__);
			break;




		}

		if(offset->offset_R >= 0x3ff) offset->offset_R = 0x3ff;
		if(offset->offset_G >= 0x3ff) offset->offset_G = 0x3ff;
		if(offset->offset_B >= 0x3ff) offset->offset_B = 0x3ff;

		AFE_PRINT("Set offset R[%x] , G[%x] , B[%x]\n",offset->offset_R,offset->offset_G, offset->offset_B );
		ADC_SetOffsetRGB(offset);

		AFE_PRINT("AVG : R[%d.%d] G[%d.%d] B[%d.%d]\n",
		(int)ire_avg_r, (int)(( (float)ire_avg_r - (float)(int)(ire_avg_r) )*10000.0),
		(int)ire_avg_g, (int)(( (float)ire_avg_g - (float)(int)(ire_avg_g) )*10000.0),
		(int)ire_avg_b, (int)(( (float)ire_avg_b - (float)(int)(ire_avg_b) )*10000.0));
		AFE_PRINT("OFFSET[0] : R[%x] G[%x] B[%x]\n", offset->offset_R,offset->offset_G,offset->offset_B);
		AFE_PRINT("OFFSET[1] : R[%x] G[%x] B[%x]\n", offset->offset_R - 0x200,offset->offset_G - 0x200 ,offset->offset_B - 0x200);

		if(offset->offset_R == offset1->offset_R && offset->offset_R == offset2->offset_R)
			bStatusR = 0;
		if(offset->offset_G == offset1->offset_G && offset->offset_G == offset2->offset_G)
			bStatusG = 0;
		if(offset->offset_B == offset1->offset_B && offset->offset_B == offset2->offset_B)
			bStatusB = 0;

		memcpy(offset2 , offset1 , sizeof(S_OFFSET_INFORM));
		memcpy(offset1 , offset , sizeof(S_OFFSET_INFORM));

		if( (bStatusR == 0)&&(bStatusG == 0)&& (bStatusB == 0) ) break;	// when RGB finish adjust...


	}while(iRepeatCount++ < ADC_AGC_NUM);

	AFE_PRINT("done serach. count is %d\n", iRepeatCount);
	AFE_PRINT("OFFSET : R[%x] G[%x] B[%x]\n", offset->offset_R,offset->offset_G,offset->offset_B);

	kfree(sum);

#endif
	return 0;
}


int ADC_AutoGainControl_main(LX_AFE_ADC_CALIB_INFO_T *calib_param, int iIndex)
{
	int ret = 0;

	ST_GAIN_INFORM gain;

//	AFE_PRINT("%s entered with index[%d]\n", __func__ , iIndex);

	ret = ADC_L9_AGC_Measurement(calib_param, &gain);

	return ret;
}


int ADC_AutoOffsetControl_main(int iIndex)
{
	int ret = 0;
//	AFE_PRINT("%s entered with index[%d]\n", __func__ , iIndex);

	ret = ADC_AOC_MeasurementByIRE();


	return ret;
}


int ADC_AutoOffsetControlByInput_main(int iIndex)
{
     int ret = 0;
 //    AFE_PRINT("%s entered with index[%d]\n", __func__ , iIndex);

     ret = ADC_AOC_Measurement();


     return ret;
}

