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


/*! \file ADC_AutoAdjustfunc.c
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
//extern volatile VPORT_RegBx_T *gVportRegBx;

/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions 
 */
extern ST_WORK_QUEUE	*gWorkqueue;

typedef struct
{
	unsigned int uiWidthMaxR;
	unsigned int uiWidthMinR;
	unsigned int uiHeightMaxR;
	unsigned int uiHeightMinR;	
	
	unsigned int uiWidthMaxG;
	unsigned int uiWidthMinG;
	unsigned int uiHeightMaxG;
	unsigned int uiHeightMinG;	
	
	unsigned int uiWidthMaxB;
	unsigned int uiWidthMinB;
	unsigned int uiHeightMaxB;
	unsigned int uiHeightMinB;	
	
}ST_SCREEN_AREA_INFO;

/******************************************************************************
 *				Local function
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions 
 */

/**
 *
 * search phase
 *
 * @param	LX_AFE_ADC_SET_INPUT_T
 * @return	0 : OK , -1 : NOK
 *
*/	
int ADC_PHASE_Mesurement(int iIndex)
{

	ST_EXTAU *extau;
//	int i,j, loop, waiting_time = 20; //powerory 2010-10-05
	int i,j, loop, waiting_time = 10; //powerory 2011-03-14
	int iMax = 0;
	ST_SCREEN_AREA_INFO sScreen, sScreenConst, sScreenBackup;
	S_RGB_POSITION *pos;
	S_RGB_SIZE *size;
	S_RGB_OUT *result;

	//0325
	//S_RGB_SUM *sum;

//	unsigned int uiWidthStepR, uiWidthStepG, uiWidthStepB;
//	unsigned int uiHeightStepR, uiHeightStepG, uiHeightStepB;
	
//	unsigned int uiOutR, uiOutG, uiOutB;
	unsigned int uiOutMaxR = 0, uiOutMaxG = 0, uiOutMaxB = 0;
	
	unsigned int uiWidthMaxR = 0, uiWidthMaxG = 0,uiWidthMaxB = 0;
	unsigned int uiHeightMaxR = 0, uiHeightMaxG = 0,uiHeightMaxB = 0;
	
	
	pos = (S_RGB_POSITION *)kmalloc(sizeof(S_RGB_POSITION),GFP_KERNEL);
	size = (S_RGB_SIZE *)kmalloc(sizeof(S_RGB_SIZE),GFP_KERNEL);
	result = (S_RGB_OUT *)kmalloc(sizeof(S_RGB_OUT),GFP_KERNEL);
	
	//LLPLL_shift 일단 0으로 초기화
	ADC_SetPhaseValue(0);
	
	
	extau = kmalloc(sizeof(ST_EXTAU) , GFP_KERNEL);
	ADC_IBD_Mesurement(extau , iIndex);

	sScreen.uiWidthMinR = extau->extau_left + PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMinR = extau->extau_up +  PHASE_METHOD2_OFFSET;
	sScreen.uiWidthMaxR = extau->extau_right - PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMaxR = extau->extau_down - PHASE_METHOD2_OFFSET;

	sScreen.uiWidthMinG = extau->extau_left + PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMinG = extau->extau_up +  PHASE_METHOD2_OFFSET;
	sScreen.uiWidthMaxG = extau->extau_right - PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMaxG = extau->extau_down - PHASE_METHOD2_OFFSET;

	sScreen.uiWidthMinB = extau->extau_left + PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMinB = extau->extau_up +  PHASE_METHOD2_OFFSET;
	sScreen.uiWidthMaxB = extau->extau_right - PHASE_METHOD2_OFFSET;
	sScreen.uiHeightMaxB = extau->extau_down - PHASE_METHOD2_OFFSET;

	AFE_PRINT("extau up : %d , down : %d , left : %d , right : %d\n",
		extau->extau_up , extau->extau_down , extau->extau_left , extau->extau_right);


	// initial value backup
	memcpy(&sScreenConst , &sScreen , sizeof(ST_SCREEN_AREA_INFO));
	
	// why be necessary 3 times loop
	for(loop=0;loop<1;loop++){
		size->width_r = (sScreen.uiWidthMaxR - sScreen.uiWidthMinR)/PHASE_POS_X_STEP;
		size->height_r = (sScreen.uiHeightMaxR - sScreen.uiHeightMinR)/PHASE_POS_Y_STEP;
		size->width_g = (sScreen.uiWidthMaxG - sScreen.uiWidthMinG)/PHASE_POS_X_STEP;
		size->height_g = (sScreen.uiHeightMaxG - sScreen.uiHeightMinG)/PHASE_POS_Y_STEP;
		size->width_b = (sScreen.uiWidthMaxB - sScreen.uiWidthMinB)/PHASE_POS_X_STEP;
		size->height_b = (sScreen.uiHeightMaxB - sScreen.uiHeightMinB)/PHASE_POS_Y_STEP;

//	AFE_PRINT("SIZE WIDTH - R[%d] G[%d] B[%d], HEIGHT - R[%d] G[%d] B[%d]\n", size->width_r , size->width_g , size->width_b , size->height_r , size->height_g , size->height_b);

	
		ADC_SetRGBSize(size);
	
		memcpy(&sScreenBackup , &sScreen , sizeof(ST_SCREEN_AREA_INFO));
		
		for(i=0; i<PHASE_POS_X_STEP; i++){
			
			//ap_xpos_b
			pos->width_b = sScreen.uiWidthMinB + i*size->width_b;
			
			//ap_xpos_g
			pos->width_g = sScreen.uiWidthMinG + i*size->width_g;
			
			//ap_xpos_r
			pos->width_r = sScreen.uiWidthMinR + i*size->width_r;

			for(j=0; j<PHASE_POS_Y_STEP; j++){
				//ap_ypos_b
				pos->height_b = sScreen.uiHeightMinB + j*size->height_b;
				
				//ap_ypos_g
				pos->height_g = sScreen.uiHeightMinG + j*size->height_g;
				
				//ap_ypos_r
				pos->height_r = sScreen.uiHeightMinR + j*size->height_r;
//AFE_PRINT("%d @ %s ij[%d][%d] POS WIDTH - R[%d] G[%d] B[%d], HEIGHT - R[%d] G[%d] B[%d]\n", __LINE__ , __func__ , i,j,pos->width_r , pos->width_g , pos->width_b , pos->height_r , pos->height_g , pos->height_b);
				
				ADC_SetRGBPosition(pos);



				msleep(waiting_time);

				ADC_GetRGBResult(result);

				ADC_GetRGBPosition(pos);
//AFE_PRINT("%d @ %s ij[%d][%d] POS WIDTH - R[%d] G[%d] B[%d], HEIGHT - R[%d] G[%d] B[%d]\n", __LINE__ , __func__ , i,j,pos->width_r , pos->width_g , pos->width_b , pos->height_r , pos->height_g , pos->height_b);


//AFE_PRINT("%d @ %s ij[%d][%d] RESULT R[%d] G[%d] B[%d]\n", __LINE__ , __func__ , i,j,result->out_r , result->out_g , result->out_b );


				if(uiOutMaxR < result->out_r)
				{
					uiWidthMaxR = pos->width_r;
					uiHeightMaxR = pos->height_r;
					uiOutMaxR = result->out_r;
				}
				if(uiOutMaxG < result->out_g)
				{
					uiWidthMaxG = pos->width_g;
					uiHeightMaxG = pos->height_g;
					uiOutMaxG = result->out_g;
				}
				if(uiOutMaxB < result->out_b)
				{
					uiWidthMaxB = pos->width_b;
					uiHeightMaxB = pos->height_b;
					uiOutMaxB = result->out_b;
				}		
				
				
			//	AFE_PRINT("loop[%d][%d] x:%04d-%4d, y:%04d-%04d\n", i,j,uiWidthMaxR, uiWidthMaxR+size->width_r, uiHeightMaxR, uiHeightMaxR+size->height_r);
				
			//	AFE_PRINT("loop[%d][%d] x:%04d-%4d, y:%04d-%04d\n", i,j,uiWidthMaxG, uiWidthMaxG+size->width_g, uiHeightMaxG, uiHeightMaxG+size->height_g);
				
			//	AFE_PRINT("loop[%d][%d] x:%04d-%4d, y:%04d-%04d\n", i,j,uiWidthMaxB, uiWidthMaxB+size->width_b, uiHeightMaxB, uiHeightMaxB+size->height_b);
				
				
			} 
		}
		
		
		
		sScreen.uiWidthMinR = uiWidthMaxR - (int)(size->width_r/5.0);
		sScreen.uiWidthMaxR = uiWidthMaxR + (int)(size->width_r*1.6);
		sScreen.uiWidthMinG = uiWidthMaxG - (int)(size->width_g/5.0);
		sScreen.uiWidthMaxG = uiWidthMaxG + (int)(size->width_g*1.6);
		sScreen.uiWidthMinB = uiWidthMaxB - (int)(size->width_b/5.0);
		sScreen.uiWidthMaxB = uiWidthMaxB + (int)(size->width_b*1.6);
	
		if(sScreen.uiWidthMinR < sScreenConst.uiWidthMinR) sScreen.uiWidthMinR = sScreenConst.uiWidthMinR;
		if(sScreen.uiWidthMaxR > sScreenConst.uiWidthMaxR) sScreen.uiWidthMaxR = sScreenConst.uiWidthMaxR;
		if(sScreen.uiWidthMinG < sScreenConst.uiWidthMinG) sScreen.uiWidthMinG = sScreenConst.uiWidthMinG;
		if(sScreen.uiWidthMaxG > sScreenConst.uiWidthMaxG) sScreen.uiWidthMaxG = sScreenConst.uiWidthMaxG;
		if(sScreen.uiWidthMinB < sScreenConst.uiWidthMinB) sScreen.uiWidthMinB = sScreenConst.uiWidthMinB;
		if(sScreen.uiWidthMaxB > sScreenConst.uiWidthMaxB) sScreen.uiWidthMaxB = sScreenConst.uiWidthMaxB;

		sScreen.uiHeightMinR = uiHeightMaxR - (int)(size->height_r/5.0);
		sScreen.uiHeightMaxR = uiHeightMaxR + (int)(size->height_r*1.6);
		sScreen.uiHeightMinG = uiHeightMaxG - (int)(size->height_g/5.0);
		sScreen.uiHeightMaxG = uiHeightMaxG + (int)(size->height_g*1.6);
		sScreen.uiHeightMinB = uiHeightMaxB - (int)(size->height_b/5.0);
		sScreen.uiHeightMaxB = uiHeightMaxB + (int)(size->height_b*1.6);
	
		if(sScreen.uiHeightMinR < sScreenConst.uiHeightMinR) sScreen.uiHeightMinR = sScreenConst.uiHeightMinR;
		if(sScreen.uiHeightMaxR > sScreenConst.uiHeightMaxR) sScreen.uiHeightMaxR = sScreenConst.uiHeightMaxR;
		if(sScreen.uiHeightMinG < sScreenConst.uiHeightMinG) sScreen.uiHeightMinG = sScreenConst.uiHeightMinG;
		if(sScreen.uiHeightMaxG > sScreenConst.uiHeightMaxG) sScreen.uiHeightMaxG = sScreenConst.uiHeightMaxG;
		if(sScreen.uiHeightMinB < sScreenConst.uiHeightMinB) sScreen.uiHeightMinB = sScreenConst.uiHeightMinB;
		if(sScreen.uiHeightMaxB > sScreenConst.uiHeightMaxB) sScreen.uiHeightMaxB = sScreenConst.uiHeightMaxB;


		if((size->width_r == 1 && size->height_r == 1) && (size->width_g == 1 && size->height_g == 1)  && 
			(size->width_b == 1 && size->height_b == 1) )
		{
			break;
		}


		if(memcmp(&sScreen , &sScreenBackup , sizeof(ST_SCREEN_AREA_INFO)) == 0)
		{
			break;	
		}

	} //loop

	// ap_xpos_b
	pos->width_b = uiWidthMaxB;
	// ap_xpos_g
	pos->width_g = uiWidthMaxG;
	// ap_xpos_r
	pos->width_r = uiWidthMaxR;
	
	//ap_ypos_b
	pos->height_b = uiHeightMaxB;
	//ap_ypos_g
	pos->height_g = uiHeightMaxG;
	//ap_ypos_r
	pos->height_r = uiHeightMaxR;


	AFE_PRINT("%s xpos_r : %d , xpos_g : %d , xpos_b : %d\n", __func__ , uiWidthMaxR , uiWidthMaxG ,uiWidthMaxB );
	AFE_PRINT("%s ypos_r : %d , ypos_g : %d , ypos_b : %d\n", __func__ , uiHeightMaxR , uiHeightMaxG ,uiHeightMaxB );
	
	ADC_SetRGBPosition(pos);
	


	// PHASE 돌아가면서 확정하기 Method 1 : 간단하게 dev 최고값
	{
		//120103 wonsik.do : removed floating point in kernel driver
		//float dMax = -1.0;
		UINT64 dMax = 0;

		int iPhaseMin =0 ;
		//int iPhaseMax = (int) ((1.0/TB_FS[iIndex] * 1000000 / 130) * 1.5);
		//120103 wonsik.do : removed floating point in kernel driver
		//int iPhaseMax = (int) ((1.0/TB_FS[iIndex] * 1000000 / 130) +2); //0918 by powerory
		int iPhaseMax = (1000000000 / (TB_FS[iIndex] * 130) +2); //0918 by powerory
		int iPhaseMaxConst = iPhaseMax;
		int iPhaseOffset = PHASE_OFFSET ;
		//120103 wonsik.do : removed floating point in kernel driver
		//float dOutAvg;
		UINT64 dOutAvg;
		//int out_g_old = 0;

		AFE_PRINT("%d @ %s phase max %d\n", __LINE__ , __func__ , iPhaseMax );
	
	
		for(loop=0;loop<1;loop++)
		{
			int iPhaseStep = (iPhaseMax-iPhaseMin) / PHASE_STEP + 1;  
			int iMaxBackup = iMax;
			for(i=iPhaseMin;i<=iPhaseMax;i+=iPhaseStep)		// risky
			{
				//LLPLL_shift
				ADC_SetPhaseValue(i);
				
				msleep(waiting_time);
	
				ADC_GetRGBResult(result);

//AFE_PRINT("%s loop[%2d] PHASE %3d RESULT R[%10d] G[%10d] B[%10d] gap[%10d]\n", __func__ , loop ,i,result->out_r , result->out_g , result->out_b, abs(result->out_g - out_g_old) );
//                out_g_old = result->out_g;

				//0325
//				sum = (S_RGB_SUM *)kmalloc(sizeof(S_RGB_SUM),GFP_KERNEL);
//				ADC_GetSumRGB(sum);

//AFE_PRINT("%d @ %s loop[%02d] PHASE %02d SUM R[%010d] G[%010d] B[%010d]\n", __LINE__ , __func__ , loop ,i,sum->sum_R , sum->sum_G , sum->sum_B );


				//120103 wonsik.do : removed floating point in kernel driver
				//dOutAvg = (float)(result->out_r + result->out_g + result->out_b ) / 3.0 ;
				dOutAvg = result->out_r + result->out_g + result->out_b;
				if(dOutAvg > dMax) {
					dMax = dOutAvg;
					iMax = i;
				}
				//AFE_PRINT("%d/%d\n", i, iPhaseMaxConst);
				
			}
			iPhaseMin = iMax - iPhaseStep * iPhaseOffset;
			iPhaseMax = iMax + iPhaseStep * (1+iPhaseOffset) ;

AFE_PRINT("Phase max %d , Phase min %d\n", iPhaseMax, iPhaseMin);
	
			if(iPhaseMin < 0) iPhaseMin =0;
			if(iPhaseMax > iPhaseMaxConst) iPhaseMax = iPhaseMaxConst;
	
			if(iPhaseStep == 1 ) break;
			if(iMax == iMaxBackup) break;
		}
	}
	

	
	kfree(extau);
	
	
	kfree(pos);
	kfree(size);
	kfree(result);
//	kfree(sum);

	return iMax;

}

/**
 *
 * search phase for hsize detection
 *
 * @param	: iIndex
 * @return	: deviation value
 *
*/	
UINT64 ADC_PHASE_Mesurement_Simple(int iIndex, UINT16 htotal)
{

	int i, loop, waiting_time = 20;// 110819 by wonsik.do to speed-up auto config //50; //powerory 2010-10-05
	int iMax = 0;
	S_RGB_POSITION *pos;
	S_RGB_SIZE *size;
	S_RGB_OUT *result;
	//120103 wonsik.do : removed floating point in kernel driver
	//float dMax = -1.0;
	UINT64 dMax = 0;

	
	pos = (S_RGB_POSITION *)kmalloc(sizeof(S_RGB_POSITION),GFP_KERNEL);
	size = (S_RGB_SIZE *)kmalloc(sizeof(S_RGB_SIZE),GFP_KERNEL);
	result = (S_RGB_OUT *)kmalloc(sizeof(S_RGB_OUT),GFP_KERNEL);
	
	//LLPLL_shift 일단 0으로 초기화
	ADC_SetPhaseValue(0);

	//powerory
	size->width_r  = htotal;
	size->height_r = TB_SD_EXT_VSIZE[iIndex];
	size->width_g  = htotal;
	size->height_g = TB_SD_EXT_VSIZE[iIndex];
	size->width_b  = htotal;
	size->height_b = TB_SD_EXT_VSIZE[iIndex];
	
	ADC_SetRGBSize(size);
	
	pos->width_b  = 0;
	pos->width_g  = 0;
	pos->width_r  = 0;
	pos->height_b = 0;
	pos->height_g = 0;
	pos->height_r = 0;

	ADC_SetRGBPosition(pos);

	// PHASE 돌아가면서 확정하기 Method 1 : 간단하게 dev 최고값
	{

		int iPhaseMin =0 ;
		//int iPhaseMax = (int) ((1.0/TB_FS[iIndex] * 1000000 / 130) * 1.5);
		//120103 wonsik.do : removed floating point in kernel driver
		int iPhaseMax = (1000000000/ (TB_FS[iIndex] * 130) + 2); //0918 by powerory
		int iPhaseMaxConst = iPhaseMax;
		int iPhaseOffset = PHASE_OFFSET_SIMPLE ;
		//120103 wonsik.do : removed floating point in kernel driver
		//float dOutAvg;
		UINT64 dOutAvg;
		
		//AFE_PRINT("%d @ %s phase max %d\n", __LINE__ , __func__ , iPhaseMax );
	
		for(loop=0;loop<1;loop++)
		{
			int iPhaseStep = (iPhaseMax-iPhaseMin) / PHASE_STEP_SIMPLE + 1;  
			int iMaxBackup = iMax;
			for(i=iPhaseMin;i<=iPhaseMax;i+=iPhaseStep)		// risky
			{
				//LLPLL_shift
				ADC_SetPhaseValue(i);
				
				msleep(waiting_time);
	
				ADC_GetRGBResult(result);

//AFE_PRINT("%d @ %s loop[%d] PHASE %d RESULT R[%d] G[%d] B[%d]\n", __LINE__ , __func__ , loop ,i,result->out_r , result->out_g , result->out_b );
	
				//120103 wonsik.do : removed floating point in kernel driver
				dOutAvg = result->out_r + result->out_g + result->out_b;
				if(dOutAvg > dMax) {
					dMax = dOutAvg;
					iMax = i;
				}
				//AFE_PRINT("%d/%d\n", i, iPhaseMaxConst);
			}
			iPhaseMin = iMax - iPhaseStep * iPhaseOffset;
			iPhaseMax = iMax + iPhaseStep * (1+iPhaseOffset) ;

//AFE_PRINT("Phase max %d , Phase min %d\n", iPhaseMax, iPhaseMin);
	
			if(iPhaseMin < 0) iPhaseMin =0;
			if(iPhaseMax > iPhaseMaxConst) iPhaseMax = iPhaseMaxConst;
	
			if(iPhaseStep == 1 ) break;
			if(iMax == iMaxBackup) break;
		}
	}
	
	kfree(pos);
	kfree(size);
	kfree(result);

	return dMax;

}
/**
 *
 * set phase which came from ADC_PHASE_Mesurement
 *
 * @param	LX_AFE_ADC_SET_INPUT_T
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_PHASE_Setting(int iMaxPll)
{

	//LLPLL_shift
	ADC_SetPhaseValue((unsigned int)iMaxPll);

	return 0;
}



int ADC_PHASE_main(int iIndex)
{
	int ret;
	
//	AFE_PRINT("%s entered with index[%d]\n", __func__ , iIndex);
	
	if(iIndex == -1)
	{
		AFE_PRINT("not be set resolutuin yet\n\n");
		return -1;
	}
	
	ret = ADC_PHASE_Mesurement(ConvertTable[iIndex]); //by dws 0809

	AFE_PRINT("## AutoPhase Result is [%d][0x%x]\n", ret, ret);

	if(ret < 0)
	{
		AFE_ERROR("return value is %d\n",  ret);
		return -1;
	}
	else
		ADC_PHASE_Setting(ret);
	
	return 0;
	
}


/**
 *
 * search phase for component input
 *
 * @param	: iIndex
 * @return	: deviation value
 *
*/	
int ADC_PHASE_Mesurement_for_Component(int iIndex)
{

	int i, loop, waiting_time = 10;
	int iMax = 0;
	S_RGB_OUT *result;
	//120103 wonsik.do : removed floating point in kernel driver
	//float dMax = -1.0;
	UINT64 dMax = 0;

	
	result = (S_RGB_OUT *)kmalloc(sizeof(S_RGB_OUT),GFP_KERNEL);
	
	//LLPLL_shift 일단 0으로 초기화
	ADC_SetPhaseValue(0);

	// PHASE 돌아가면서 확정하기 Method 1 : 간단하게 dev 최고값
	{

		int iPhaseMin =0 ;
		//int iPhaseMax = (int) ((1.0/TB_FS[iIndex] * 1000000 / 130) * 1.5);
		//120103 wonsik.do : removed floating point in kernel driver
		//int iPhaseMax = (int) ((1.0/TB_FS[iIndex] * 1000000 / 130) + 2); //0918 by powerory
		int iPhaseMax = (1000000000 / (TB_FS[iIndex] * 130) + 2); //0918 by powerory
		int iPhaseMaxConst = iPhaseMax;
		int iPhaseOffset = PHASE_OFFSET_COMPONENT ;
		//120103 wonsik.do : removed floating point in kernel driver
		//float dOutAvg;
		UINT64 dOutAvg;
		
		//AFE_PRINT("%d @ %s phase max %d\n", __LINE__ , __func__ , iPhaseMax );
	
		for(loop=0;loop<1;loop++)
		{
			int iPhaseStep = (iPhaseMax-iPhaseMin) / PHASE_STEP_COMPONENT + 1;  
			int iMaxBackup = iMax;
			for(i=iPhaseMin;i<=iPhaseMax;i+=iPhaseStep)		// risky
			{
				//LLPLL_shift
				ADC_SetPhaseValue(i);
				
				msleep(waiting_time);
	
				ADC_GetRGBResult(result);

				//120103 wonsik.do : removed floating point in kernel driver
				//dOutAvg = (float)(result->out_r + result->out_g + result->out_b ) / 3.0 ;
				dOutAvg = result->out_r + result->out_g + result->out_b;

				ADC_DEBUG("PHASE %3d RESULT R[%7d] G[%7d] B[%7d] Avg[%7d]\n", i,result->out_r , result->out_g , result->out_b, (UINT32)dOutAvg );
	
				if(dOutAvg > dMax) {
					dMax = dOutAvg;
					iMax = i;
				}
				//AFE_PRINT("dOutAvg %d , dMax %d\n", (int)dOutAvg, (int)dMax);

				//AFE_PRINT("%d/%d\n", i, iPhaseMaxConst);
			}
			iPhaseMin = iMax - iPhaseStep * iPhaseOffset;
			iPhaseMax = iMax + iPhaseStep * (1+iPhaseOffset) ;

//AFE_PRINT("Phase max %d , Phase min %d\n", iPhaseMax, iPhaseMin);
	
			if(iPhaseMin < 0) iPhaseMin =0;
			if(iPhaseMax > iPhaseMaxConst) iPhaseMax = iPhaseMaxConst;
	
			if(iPhaseStep == 1 ) break;
			if(iMax == iMaxBackup) break;
		}
	}
	
	kfree(result);

	return (int)iMax;

}


int ADC_PHASE_for_Component(int Kadp_Index)
{
	int ret;
	
	//AFE_PRINT("%s entered with index[%d]\n", __func__ , Kadp_Index);
	
	if(Kadp_Index == -1)
	{
		AFE_PRINT("not be set resolutuin yet\n\n");
		return -1;
	}
	
	ret = ADC_PHASE_Mesurement_for_Component(ConvertTable[Kadp_Index]);
	//ret = ADC_PHASE_Mesurement(ConvertTable[iIndex]);

	AFE_PRINT("## AutoPhase for component Result is [%d][0x%x]\n", ret, ret);

	if(ret < 0)
	{
		AFE_ERROR("return value is %d\n",  ret);
		return -1;
	}
	else
		ADC_PHASE_Setting(ret);
	
	return 0;
	
}
