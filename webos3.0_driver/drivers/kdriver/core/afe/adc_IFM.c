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


/*! \file ADC_IFMfunc.c
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

#include "os_util.h"
#include "afe_drv.h"
#include <linux/interrupt.h>
#include "afe_kapi.h"
//#include "vport_reg.h"

//#include "vport_reg.h"
#include "resolution.h"
#include "adc_module.h"
#include "adc_module.h"
#include "cvd_module.h"

/******************************************************************************
 *				DEFINES
 *****************************************************************************/
//#define ADC_1080I_WORKAROUND
//#define ADC_POLARITY_OVERRIDE
//#define ADC_SIZE_DETECTION_WORKAROUND
#define ADC_EXT_CLAMP_WORKAROUND

//#define ADC_MSPG_114_TIMING_WORKAROUND
//#define ADC_COMP_NO_SYNC_WORKAOURND
//extern volatile VPORT_Reg_T *gVportReg;
/**
 *	Global variables of the driver
 */
extern LX_AFE_PCMODE_MODETABLE_T *gPCTable;

extern unsigned int gPCTableNum;

extern LX_AFE_ADC_INPUT_SOURCE_TYPE_T	gADC_InputSrc; //added by dws
extern ST_WORK_QUEUE	*gWorkqueue;
extern int gResolution;

int gPrevious_index = 0xffff;
int gADC_IN_IFM = 0;

extern int _g_Enable_HTotal_Diff_Workaround	;
extern int		g_Fast_IFM_mode;
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
S_VIDEO_SIZE_INFORM g_SignalInform = {0xffffffff,};

static LX_AFE_RESOLUTION_TYPE_T	g_User_Resolution = LX_RES_1024X768;
/**
 *
 * Input format measurement
 * vsize, hsize, ext_vsize deteting
 *
 * @param	S_VIDEO_SIZE_INFORM
 * @return	0 : OK , -1 : NOK
 *
*/

int ADC_GetInputSignalInform(S_VIDEO_SIZE_INFORM *inform)
{
	S_VIDEO_SIZE_INFORM inform_proc, *inform_temp;

//	AFE_PRINT("%s entered\n" , __func__);
	// memory clear
	memset((void *)&inform_proc , 0 , sizeof(S_VIDEO_SIZE_INFORM));
	inform_temp = (S_VIDEO_SIZE_INFORM *)kmalloc(sizeof(S_VIDEO_SIZE_INFORM),GFP_KERNEL);

#ifndef USE_ADC_TIMER
	//add by dws
	//OS_MsecSleep(100); //0731 dws
///
#endif

#if 0
	for(i = 0 ; i < INPUT_SIGNAL_INFORM_REPEAT ; i++)	// getting the inform for INPUT_SIGNAL_INFORM_REPEAT times
	{
		ADC_GetSizeValue(inform_temp);

		inform_proc.ext_vsize += inform_temp->ext_vsize;
		inform_proc.vd_vsize += inform_temp->vd_vsize;
		inform_proc.vd_hsize += inform_temp->vd_hsize;
/*
		AFE_PRINT("temp [%d] ext_vsize : %d , vd_vsize : %d , vd_hsize : %d\n", i ,
		inform_temp->ext_vsize , inform_temp->vd_vsize , inform_temp->vd_hsize);
		AFE_PRINT("proc [%d] ext_vsize : %d , vd_vsize : %d , vd_hsize : %d\n", i ,
		inform_proc.ext_vsize , inform_proc.vd_vsize , inform_proc.vd_hsize);
*/

	//	udelay(0);	// delay should be used?
	}

	// calculate average vlaue
	inform_proc.ext_vsize = (inform_proc.ext_vsize + (INPUT_SIGNAL_INFORM_REPEAT>>1))>>INPUT_SIGNAL_INFORM_REPEAT_POW;
	inform_proc.vd_vsize = (inform_proc.vd_vsize + (INPUT_SIGNAL_INFORM_REPEAT>>1))>>INPUT_SIGNAL_INFORM_REPEAT_POW;
	inform_proc.vd_hsize = (inform_proc.vd_hsize + (INPUT_SIGNAL_INFORM_REPEAT>>1))>>INPUT_SIGNAL_INFORM_REPEAT_POW;

#else
	ADC_GetSizeValue(inform_temp);

	inform_proc.ext_vsize += inform_temp->ext_vsize;
	inform_proc.vd_vsize += inform_temp->vd_vsize;
	inform_proc.vd_hsize += inform_temp->vd_hsize;
#endif
	inform->vd_vsize = inform_proc.vd_vsize;
	inform->vd_hsize = inform_proc.vd_hsize;
	inform->ext_vsize = inform_proc.ext_vsize;

	inform->polarity = inform_temp->polarity;


//	AFE_PRINT("ext_v:%ld,vd_v:%ld,vd_h:%ld\n", inform->ext_vsize , inform->vd_vsize , inform->vd_hsize);

	kfree(inform_temp);

	return 0;

}


/**
 *
 * search correct setting value from the table
 *
 * @param	S_VIDEO_SIZE_INFORM
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_IFM_SearchTable(S_VIDEO_SIZE_INFORM *inform)
{
	int step , iSearchedIndex = -1;
	int iMinGapValue = 0xffffff;
	int iHorGap , iVerGap , iVTotalGap, iHSGap;
	S_VIDEO_SIZE_INFORM *par;
	int iLoopNum;
	//unsigned int iInterlace;
	UINT32 vs_width, hs_width;

	int iVtotal_Tolerance;

	// translate the gotten parameter
	par = (S_VIDEO_SIZE_INFORM *)kmalloc(sizeof(S_VIDEO_SIZE_INFORM) , GFP_KERNEL);

	par->ext_vsize = inform->ext_vsize;
	if((inform->vd_hsize+2) != 0)
	{
		par->vd_hsize = (FREQ_CONV_HZ + (inform->vd_hsize>>1) )/(inform->vd_hsize + 2);
	}
	else
	{
		 par->vd_hsize = 0;
	}
	if(inform->vd_vsize != 0)
		par->vd_vsize = (FREQ_CONV_HZ*1000 + (inform->vd_vsize>>1))/(inform->vd_vsize);
	else
		par->vd_vsize = 0;

	par->polarity = (0x08)>>(inform->polarity); // convert register value to AFE_PCMODE_FLAGs

	//iInterlace = (ADC_GetScanType()^1);
	//iLoopNum = PARAM_NUM;
	iLoopNum = gPCTableNum; //0622 dws

	ADC_IFM_DEBUG("calculated ext_vsize : %ld , vd_vsize : %ld , vd_hsize : %ld , pol : %ld , src : %d\n",
		par->ext_vsize , par->vd_vsize , par->vd_hsize , par->polarity, gADC_InputSrc);

	//AFE_PRINT("interace mode is %d\n", iInterlace);
#define	HOR_TOLERANCE	3
#define	VER_TOLERANCE	3
#define	TOTAL_TOLERANCE	0xfff
#define VSYNC_WIDTH_TOLERANCE	1
#define HSYNC_WIDTH_TOLERANCE	5
#define VTOTAL_TOLERANCE	50

	for(step = 0 ; step < iLoopNum ; step++)
	{
		if((gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)||(gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART)) // YPbPr Source
		{
		   if(!((gPCTable+step)->u8_Status_Flag & AFE_PCMODE_FLAG_YPBPR_BIT))
			  continue;
		}
		else	// RGB PC mode
		{
		   if((gPCTable+step)->u8_Status_Flag & AFE_PCMODE_FLAG_YPBPR_BIT)
			  continue;
		   else if(!(par->polarity & ((gPCTable+step)->u8_Status_Flag & 0x0F))) continue;	//0622 dws
		}


		// check horizontal freq
		iHorGap = abs((gPCTable+step)->u16_HFreq - par->vd_hsize);

		if( iHorGap <= HOR_TOLERANCE)
		{
			// check vertical freq
			iVerGap = abs((gPCTable+step)->u16_VFreq - par->vd_vsize);
			if( iVerGap <= VER_TOLERANCE)
			{
				if((gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)||(gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART)) // YPbPr Source
				{
					// check vertical total
					iVTotalGap = ((gPCTable+step)->u16_VTotal - par->ext_vsize);

					//added 110622 : for abnormal component signal to be invalide signal, Vtotal must be measured
					//					: measured vototal can be increased due to serration and macrovision pulses.
					//added 120925 : for 576i Component with M/V CP, vtotal measured by upto 60.
					if (step == 61) 
						iVtotal_Tolerance = VTOTAL_TOLERANCE<<1;
					else 
						iVtotal_Tolerance = VTOTAL_TOLERANCE;
 
					if ( iVTotalGap >= (-(iVtotal_Tolerance)) && (iVTotalGap <= (gPCTable+step)->u8_VTotal_Tolerance) )
					{
						if(iMinGapValue > (iHorGap + iVerGap))
						{
							iMinGapValue = (iHorGap + iVerGap);
							iSearchedIndex = step;
							AFE_PRINT("1st Match Table : step=[%d], flag=[0x%x], pol=[0x%ld], HorGap=[%d], VerGap=[%d], VtotalGap=[%d], MinGap=[%d] \n",step ,(gPCTable+step)->u8_Status_Flag, par->polarity, iHorGap, iVerGap, iVTotalGap, iMinGapValue);
						}
					}
				}
				else	// RGB PC mode
				{

					// check vertical total
					iVTotalGap = abs((gPCTable+step)->u16_VTotal - par->ext_vsize);
					if(iVTotalGap <= (gPCTable+step)->u8_VTotal_Tolerance)
					{
						// add code  here for extra option

						//If Vsync Width is not zero
						if((gPCTable+step)->u8_VSync_Width > 0)
						{
							ADC_Read_VSync_Width(&vs_width);

							AFE_PRINT("Step [%d], Table Vsync Width = [%d], Read Vsync Width = [%d]\n ", step, (gPCTable+step)->u8_VSync_Width, vs_width);

							if((abs((gPCTable+step)->u8_VSync_Width - vs_width)) <= VSYNC_WIDTH_TOLERANCE)
							{
								if(iMinGapValue > (iHorGap + iVerGap + iVTotalGap))
								{
									iMinGapValue = (iHorGap + iVerGap + iVTotalGap);
									iSearchedIndex = step;
									AFE_PRINT("1st Match Table : step=[%d], flag=[0x%x], pol=[0x%ld], HorGap=[%d], VerGap=[%d], VtotalGap=[%d], MinGap=[%d] \n",step ,(gPCTable+step)->u8_Status_Flag, par->polarity, iHorGap, iVerGap, iVTotalGap, iMinGapValue);
								}
							}
						}

						//If Vsync Width is zero, do not compare vsync width value
						else
						{
							if((gPCTable+step)->u8_HSync_Width > 0)
							{
								ADC_Read_HSync_Width(&hs_width);

								AFE_PRINT("Step [%d], Table Hsync Width = [%d], Read Hsync Width = [%d]\n ", step, (gPCTable+step)->u8_HSync_Width, hs_width);

								iHSGap = abs((gPCTable+step)->u8_HSync_Width - hs_width);
							}
							else
								iHSGap = 0;


							if(iMinGapValue > (iHorGap + iVerGap + iVTotalGap + iHSGap))
							{
								iMinGapValue = (iHorGap + iVerGap + iVTotalGap + iHSGap);
								iSearchedIndex = step;
								AFE_PRINT("1st Match Table : step=[%d], flag=[0x%x], pol=[0x%ld], HorGap=[%d], VerGap=[%d], VtotalGap=[%d], HSGap=[%d], MinGap=[%d] \n",step ,(gPCTable+step)->u8_Status_Flag, par->polarity, iHorGap, iVerGap, iVTotalGap, iHSGap, iMinGapValue);
							}

						}

					}
				}
			}
		}
	}

	if(iSearchedIndex < 0)
	{
	ADC_IFM_DEBUG("! 2ND Search Loop\n");

		for(step = 0; step < iLoopNum ; step++)
		{


			if((gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)||(gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART)) // YPbPr Source
			{
				if(!((gPCTable+step)->u8_Status_Flag & AFE_PCMODE_FLAG_YPBPR_BIT))
					continue;
			}
			else	// RGB PC mode
			{
				if((gPCTable+step)->u8_Status_Flag & AFE_PCMODE_FLAG_YPBPR_BIT)
					continue;
			}
			//	if(iInterlace != ((LX_Default_ModeTable[step].u8_Status_Flag&0x10) >> 4) ) continue;
			//	if(par->polarity != (par->u8_Status_Flag&0x3) ) continue;

			//			if(par->ext_vsize < (gPCTable+step)->u16_VTotal)
			if(par->ext_vsize > LX_Default_Resolution[(gPCTable+step)->u8_Res_Idx].u16_Height)
			{
				iHorGap = abs((gPCTable+step)->u16_HFreq - par->vd_hsize);
				iVerGap = abs((gPCTable+step)->u16_VFreq - par->vd_vsize);
				iVTotalGap = abs((gPCTable+step)->u16_VTotal - par->ext_vsize);

				if(iHorGap + iVerGap > 10) continue;

				if(iMinGapValue > (iHorGap + iVerGap))
				{

					iMinGapValue = (iHorGap + iVerGap);
					if(iMinGapValue < TOTAL_TOLERANCE)
					{

						iSearchedIndex = step;
						AFE_PRINT("2nd Match Table : step=[%d], flag=[0x%x], pol=[0x%ld], HorGap=[%d], VerGap=[%d], VtotalGap=[%d], MinGap=[%d] \n",step ,(gPCTable+step)->u8_Status_Flag, par->polarity, iHorGap, iVerGap, iVTotalGap, iMinGapValue);
					}
				}

			}
		}
	}
	if( (abs((gPCTable+iSearchedIndex)->u16_VFreq - 600) <= VER_TOLERANCE) && ( ((gPCTable+iSearchedIndex)->u8_Res_Idx == LX_RES_1024X768) || ((gPCTable+iSearchedIndex)->u8_Res_Idx == LX_RES_1280X768) || ((gPCTable+iSearchedIndex)->u8_Res_Idx == LX_RES_1360X768) ) )// User PCMode Resolution Setting
	{
		if((iSearchedIndex != 24) && (iSearchedIndex != 98) /*&&(iSearchedIndex != 36)*/) // 1280x768 59Hz or 1360x768 60Hz(VESA)
		{
			if (g_User_Resolution == LX_RES_1024X768)
			{
				if(iSearchedIndex != 16) //1024x768 65MHz
					iSearchedIndex = 15; //1024X768 60Hz (VESA)
			}
			else if (g_User_Resolution == LX_RES_1280X768)
				iSearchedIndex = 25; //1280X768 60Hz (VESA-GTF)
			else if (g_User_Resolution == LX_RES_1360X768)
				if(iSearchedIndex != 36)
					iSearchedIndex = 35; //1360X768 60Hz (VESA)
			AFE_PRINT("3rd User Resolution : Change SearchdeIndex to = [%d]\n",iSearchedIndex );
		}
	}

//	AFE_PRINT("^^ Status Flag [0x%x]\n", (gPCTable+step)->u8_Status_Flag);

	kfree(par);
#undef	HOR_TOLERANCE
#undef	VER_TOLERANCE
#undef	TOTAL_TOLERANCE
	return iSearchedIndex;
}

int ADC_IFM_GetInputSignal(S_VIDEO_SIZE_INFORM *inform)
{
	memcpy((void *)inform , (void *)&g_SignalInform , sizeof(S_VIDEO_SIZE_INFORM));

	return 0;
}

int ADC_Set_PCMODE_Resoultion(UINT32 arg)
{
	//int unused;

	g_User_Resolution = (LX_AFE_RESOLUTION_TYPE_T)arg;

	AFE_PRINT("Set User PCMODE Resolution to [%d]\n", g_User_Resolution);

	//ADC_IFM_ISR((void *)&unused);
	ADC_Clear_Previous_Size_Info();

	return 0;

}

int ADC_IFM_Routine(S_VIDEO_SIZE_INFORM *p_adc_size_info)
{
	int Kadp_Index = 0;
	ST_EXTAU 	extau;

	Kadp_Index = ADC_IFM_SearchTable(p_adc_size_info);

	if(Kadp_Index > -1)
	{

		{
			if(TB_PC[ConvertTable[Kadp_Index]] == 0) // Component Input
			{
				// Component Clamp Position Setting moved to ADC_H13Ax_Set_Comp_Params
				/*
				if(TB_WIDTH[ConvertTable[Kadp_Index]] < 1024) // for SD Video
					ADC_Set_Blank(0x20);
					//ADC_L9Ax_Set_Blank(0x20);
				else
					ADC_Set_Blank(0x40);
					//ADC_L9Ax_Set_Blank(0x40);
					*/
			}
			else		// For RGB
				ADC_Set_Blank(0x30);

		}

		if(gPrevious_index != Kadp_Index)
		{
			if(( gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART) || (gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)) // YPbPr Source
				ADC_Set_Comp_Params(TB_WIDTH[ConvertTable[Kadp_Index]], TB_PI[ConvertTable[Kadp_Index]], TB_FV[ConvertTable[Kadp_Index]] );

			//	ADC_Reset_LLPLL();
			if(ADC_SetInputVideo(ConvertTable[Kadp_Index]) == 0)
				gPrevious_index = Kadp_Index;
			else
				gPrevious_index = 0xffff;

			AFE_PRINT("Setting done index [%d] [%d] size=[%dx%d %d Hz] P(1)/I(0)=[%d]\n", Kadp_Index , ConvertTable[Kadp_Index], TB_WIDTH[ConvertTable[Kadp_Index]], TB_HEIGHT[ConvertTable[Kadp_Index]], TB_FV[ConvertTable[Kadp_Index]] /1000 , TB_PI[ConvertTable[Kadp_Index]] );

			//131112 by ds.park;
			ADC_Reset_LLPLL();
		}
		else
		{
			if(( gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART) || (gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)) // YPbPr Source
				ADC_Set_Comp_Params(TB_WIDTH[ConvertTable[Kadp_Index]], TB_PI[ConvertTable[Kadp_Index]], TB_FV[ConvertTable[Kadp_Index]] );

			ADC_IFM_DEBUG("SAME Index again, gPrevious_index = [%d], Kadp_Index = [%d]\n", gPrevious_index, Kadp_Index);

		}

		// Set gResolution Index to New Index found.
		gResolution = Kadp_Index;

		memcpy((void *)&g_SignalInform , (void *)p_adc_size_info , sizeof(S_VIDEO_SIZE_INFORM));

		extau.extau_up = (gPCTable+gResolution)->u16_HStart;
		extau.extau_left = (gPCTable+gResolution)->u16_VStart;

		ADC_IBD_Setting(&extau , gResolution);

		//Component Phase setting is done at ADC_Component_APA()
		if(g_Fast_IFM_mode == 0)
			ADC_Component_APA(ConvertTable[Kadp_Index]);

		//Set Parameters needed for Component CP
		if(gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR)
			ADC_Component_Set_CP_Params(Kadp_Index);

		//at 1124 Workaround for video horizontal line artifact when 1080P 24Hz to 1080i 50Hz conversion on divx
#if 0
#ifdef ADC_PERIODIC_HTOTAL_READ
		if(( Kadp_Index > -1 ) && (TB_PC[ConvertTable[Kadp_Index]] == 0)) // Component Input
		{
			OS_MsecSleep(30);
			ADC_Check_Htotal_Diff(5, Kadp_Index);
		}
#endif
#endif
		// Added for SCART RGB. If llpll is unlocked , Fast Blanking is not detected in CVD, resulting in only SCART CVBS mode.
		if(_g_Enable_HTotal_Diff_Workaround && ( Kadp_Index > -1 ) && ( gADC_InputSrc == LX_ADC_INPUT_SOURCE_RGB_SCART))
		{
			OS_MsecSleep(30);
			ADC_Check_Htotal_Diff(25, Kadp_Index);
		}

		if( (gADC_InputSrc == LX_ADC_INPUT_SOURCE_YPBPR) && ( (Kadp_Index == 59) || (Kadp_Index == 61) ) )
			ADC_Enable_Sync_Low_Level_Read(1);

		return 0;

	}
	else
	{
		// Set gResolution Index to -1 (Resolution not found).
		gResolution = Kadp_Index;

		memcpy((void *)&g_SignalInform , (void *)p_adc_size_info , sizeof(S_VIDEO_SIZE_INFORM));
		ADC_IFM_DEBUG("ext_vsize : %ld, vd_vsize : %ld, vd_hsize : %ld\n", g_SignalInform.ext_vsize, g_SignalInform.vd_vsize, g_SignalInform.vd_hsize);

		ADC_IFM_DEBUG("%s can't find index\n" , __func__);

		return -1;
	}

}
