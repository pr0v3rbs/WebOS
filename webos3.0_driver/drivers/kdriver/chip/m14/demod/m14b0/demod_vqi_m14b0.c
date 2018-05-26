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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/timer.h>

#include "demod_impl.h"
#include "demod_reg_m14b0.h"
#include "demod_common_m14b0.h"
#include "demod_vqi_m14b0.h"

#if 0
#endif


/***********************************************************************************
	Global Type Definitions
 ************************************************************************************/

 UINT16 _gM14B0GBB_FullVQIPkerrCount = 7;
UINT16 _gM14B0GBB_FullVQPkerrThreshold = 300;
UINT16 _gM14B0GBB_FullISDBTPkerrThreshold = 200;

UINT8	_gM14B0GBB_Phn_state = 0;

extern UINT8 gM14B0GBB_PowerOnInit;
unsigned int gM14B0GBB_PowerOnInitStartTimeISDBT = 0;

UINT16	_gM14B0GBB_DelayForStableTime = LX_DEMOD_LOCK_DEALY_200;
UINT8	_gM14B0GBB_ChdiffTh = 0xf;
BOOLEAN _gM14B0GBB_Tpblockon = FALSE;


/***********************************************************************************
	Static Variable & Function Prototypes Declarations
 ************************************************************************************/
static UINT16	_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;






//--------------------------------------------------------------------
//static int _DEMOD_M14B0_VSB_Get_NarrowPacket_Error(UINT32 *pError);




//--------------------------------------------------------------------








#if 0
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_GetMSE.
 * Get VSB MSE
 *
 * @param  *pMSE [OUT] BOOLEAN - get signal noise ratio
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_GetMSE(UINT16 *pMSE)
{
	UINT16 	tempMseValue=0;
	UINT8	mse_15_8;
	UINT8	mse_7_0;


 	//MSE
	Common_RdFL(eqcommon_mon4);  //regAddr = 0xec, EQCOMMON_MON[4]
	Common_Rd01(eqcommon_mon4,mse_15_8,mse_15_8); //MSE[15:8]:Monitoring for Mean Square Error(MSE) value
	tempMseValue = (mse_15_8 << 8);

	Common_RdFL(eqcommon_mon5);  //regAddr = 0xed, EQCOMMON_MON[5]
	Common_Rd01(eqcommon_mon5,mse_7_0,mse_7_0); //MSE[7:0]
	tempMseValue += (mse_7_0 << 0);

	*pMSE = tempMseValue;
	DEMOD_PRINT( "^r^[DEMOD H13 DTV] check MSE chungiii### [%d](%s:%d)  \n",tempMseValue, __F__, __L__);

	return RET_OK;

}
#endif


#if 0
#endif

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Set_DefaultRegisterValue.
 * Initialize GBB VSB module .
 *
 * @param  NONE [IN] void
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_VSB_Set_DefaultRegisterValue(void)
{

	DEMOD_TRACE("[DEMOD M14B0 DTV] @ %s =====\n",__F__);

#if 1

#if 1
//	UINT8	crmapbwstl_v_3_0;
//	UINT8	samgsautostl_v_1_0;

	UINT8	agcgearshen;
	UINT8	agcifbw_3_0;


// Restore the following register values
// because of AGC speed performance
//	UINT8	dagc2ndqsen;
//	UINT8	dagc2ndrefmode;
//	UINT8	dagc2ndref_15_8;
//	UINT8	dagc2ndref_7_0;

	UINT8	phnboostphderotbwsel_3_0;

	UINT8	samgsautostl_v_1_0;
	UINT8	samcnormbp_v_1_0, sammodesel_v_1_0;
	UINT8	cnfcnttpif_7_0;
	UINT8	crdmapgainnormal_3_0;

	UINT8	shortgfbfoffen;
//	UINT8	badstatictrackfdeq_3_0;
//	UINT8	badphderotbwsel_3_0, baddynphderotbwsel_3_0;
	UINT8	samgsautoedl_v_1_0;
	UINT8	samgearshen;

	UINT8	acqstaticfbf_2_0;
	UINT8	chslowghost_7_0;

	UINT8	adcvrefsel_1;
	UINT8	adcvrefsel_0;
	UINT8	fbfmode;
	UINT8	samztedstart_v_3_0;

	UINT8	statictrackfbf_2_0;
	UINT8	badstatictrackfbf_2_0;

	// add reg because ponglim i one
	UINT8	crmapbwedl_v_3_0;
	UINT8	crpgain_v_3_0;

	UINT8	crgstep_vd_3_0;
	UINT8	dyntrackfdeq_3_0;
	UINT8	crdmapgainhigh_3_0;
	UINT8	tpifctrl_3_0;

	tpifctrl_3_0 = 0x1;
	Common_RdFL(common_ctrl114); // regAddr = 0x0072[7:4]
	Common_Wr01(common_ctrl114, tpifctrl_3_0, tpifctrl_3_0);
	Common_WrFL(common_ctrl114);



	adcvrefsel_1 = 0x1;
	Common_RdFL(common_ctrl0); // regAddr = 0x0000[2]
	Common_Wr01(common_ctrl0, adcvrefsel_1, adcvrefsel_1);
	Common_WrFL(common_ctrl0);

	adcvrefsel_0 = 0x1;
	Common_RdFL(common_ctrl4); // regAddr = 0x0004[7]
	Common_Wr01(common_ctrl4, adcvrefsel_0, adcvrefsel_0);
	Common_WrFL(common_ctrl4);


	fbfmode = 0x0;
	EQ_V_RdFL(eq_v_ctrl92); // regAddr = 0x215c[7]
	EQ_V_Wr01(eq_v_ctrl92, fbfmode, fbfmode);
	EQ_V_WrFL(eq_v_ctrl92);

	samztedstart_v_3_0 = 0x5;
	Sync_RdFL(sync_ctrl68); // regAddr = 0x1044[3:0]
	Sync_Wr01(sync_ctrl68, samztedstart_v_3_0, samztedstart_v_3_0);
	Sync_WrFL(sync_ctrl68);


	acqstaticfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl48); // regAddr = 0x2130[2:0]
	EQ_V_Wr01(eq_v_ctrl48, acqstaticfbf_2_0, acqstaticfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl48);

	chslowghost_7_0 = 0x20;
	EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
	EQ_V_Wr01(eq_v_ctrl33, chslowghost_7_0, chslowghost_7_0);
	EQ_V_WrFL(eq_v_ctrl33);





	#if 0
	UINT8	dagc2ndbw_3_0;
	UINT8	dagcbwoffset_3_0;
	UINT8	dagcbwfastdyn_1_0;
	UINT8	dagcdynbw_3_0;
	#endif


	#if 0
	crmapbwstl_v_3_0 = 0x0;
	Sync_RdFL(sync_ctrl97); // regAddr = 0x1061[6:4]
	Sync_Wr01(sync_ctrl97, crmapbwstl_v_3_0, crmapbwstl_v_3_0);
	Sync_WrFL(sync_ctrl97);

	samgsautostl_v_1_0 = 0x2;
	Sync_RdFL(sync_ctrl60); // regAddr = 0x103c[7:4]
	Sync_Wr01(sync_ctrl60, samgsautostl_v_1_0, samgsautostl_v_1_0);
	Sync_WrFL(sync_ctrl60);

	#endif



	// adding 2013/11/27 because of R2.2

	agcgearshen = 0x0;
	Common_RdFL(common_ctrl18); // regAddr = 0x0012[4]
	Common_Wr01(common_ctrl18, agcgearshen, agcgearshen);
	Common_WrFL(common_ctrl18);

	agcifbw_3_0 = 0x4;
	Common_RdFL(common_ctrl24); // regAddr = 0x0018[3:0]
	Common_Wr01(common_ctrl24, agcifbw_3_0, agcifbw_3_0);
	Common_WrFL(common_ctrl24);

#if 0
	dagc2ndqsen = 0x0;
	Common_RdFL(common_ctrl56); // regAddr = 0x0038[1]
	Common_Wr01(common_ctrl56, dagc2ndqsen, dagc2ndqsen);
	Common_WrFL(common_ctrl56);

	dagc2ndrefmode = 0x1;
	Common_RdFL(common_ctrl53); // regAddr = 0x0035[4]
	Common_Wr01(common_ctrl53, dagc2ndrefmode, dagc2ndrefmode);
	Common_WrFL(common_ctrl53);

	//dagc2ndref_15_8 = 0x20;
	dagc2ndref_15_8 = 0x16;
	Common_RdFL(common_ctrl54); // regAddr = 0x0036[7:0]
	Common_Wr01(common_ctrl54, dagc2ndref_15_8, dagc2ndref_15_8);
	Common_WrFL(common_ctrl54);

	dagc2ndref_7_0 = 0x0;
	Common_RdFL(common_ctrl55); // regAddr = 0x0037[7:0]
	Common_Wr01(common_ctrl55, dagc2ndref_7_0, dagc2ndref_7_0);
	Common_WrFL(common_ctrl55);

#endif



	#if 0
	dagc2ndbw_3_0 = 0x6;
	Common_RdFL(common_ctrl53); // regAddr = 0x0035[3:0]
	Common_Wr01(common_ctrl53, dagc2ndbw_3_0, dagc2ndbw_3_0);
	Common_WrFL(common_ctrl53);

	dagcbwoffset_3_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl84); // regAddr = 0x2154[3:0]
	EQ_V_Wr01(eq_v_ctrl84, dagcbwoffset_3_0, dagcbwoffset_3_0);
	EQ_V_WrFL(eq_v_ctrl84);

	dagcbwfastdyn_1_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl87); // regAddr = 0x2157[3:2]
	EQ_V_Wr01(eq_v_ctrl87, dagcbwfastdyn_1_0, dagcbwfastdyn_1_0);
	EQ_V_WrFL(eq_v_ctrl87);

	dagcdynbw_3_0 = 0x4;
	EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[7:4]
	EQ_V_Wr01(eq_v_ctrl85,dagcdynbw_3_0,dagcdynbw_3_0);
	EQ_V_WrFL(eq_v_ctrl85);

	#endif


	crmapbwedl_v_3_0 = 0x3;
	Sync_RdFL(sync_ctrl97); // regAddr = 0x1061[6:4]
	Sync_Wr01(sync_ctrl97, crmapbwedl_v_3_0, crmapbwedl_v_3_0);
	Sync_WrFL(sync_ctrl97);

	crpgain_v_3_0 = 0x7;
	Sync_RdFL(sync_ctrl115); // regAddr = 0x1073[7:4]
	Sync_Wr01(sync_ctrl115, crpgain_v_3_0, crpgain_v_3_0);
	Sync_WrFL(sync_ctrl115);


	samgearshen = 0x1;
	Common_RdFL(common_ctrl61); // regAddr = 0x003d[6]
	Common_Wr01(common_ctrl61, samgearshen, samgearshen);
	Common_WrFL(common_ctrl61);

	samgsautostl_v_1_0 = 0x2;
	Sync_RdFL(sync_ctrl60); // regAddr = 0x103c[7:4]
	Sync_Wr01(sync_ctrl60, samgsautostl_v_1_0, samgsautostl_v_1_0);
	Sync_WrFL(sync_ctrl60);

	samgsautoedl_v_1_0 = 0x5;
	Sync_RdFL(sync_ctrl60); // regAddr = 0x103c[3:0]
	Sync_Wr01(sync_ctrl60, samgsautoedl_v_1_0, samgsautoedl_v_1_0);
	Sync_WrFL(sync_ctrl60);

	phnboostphderotbwsel_3_0 = 0xa;
	EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
	EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0, phnboostphderotbwsel_3_0);
	EQ_V_WrFL(eq_v_ctrl59);

	samcnormbp_v_1_0 = 0x0;
	sammodesel_v_1_0 = 0x3;
	Sync_RdFL(sync_ctrl61); // regAddr = 0x103d[5:4]
	Sync_Wr02(sync_ctrl61, samcnormbp_v_1_0, samcnormbp_v_1_0, sammodesel_v_1_0, sammodesel_v_1_0);
	Sync_WrFL(sync_ctrl61);


	cnfcnttpif_7_0 = 0x4;
	EQ_V_RdFL(eq_v_ctrl126); // regAddr = 0x217e[7:0]
	EQ_V_Wr01(eq_v_ctrl126, cnfcnttpif_7_0, cnfcnttpif_7_0);
	EQ_V_WrFL(eq_v_ctrl126);

	crdmapgainnormal_3_0 = 0x6;
	EQ_V_RdFL(eq_v_ctrl97); // regAddr = 0x2161[3:0]
	EQ_V_Wr01(eq_v_ctrl97, crdmapgainnormal_3_0, crdmapgainnormal_3_0);
	EQ_V_WrFL(eq_v_ctrl97);


	shortgfbfoffen = 0x0;
	EQ_V_RdFL(eq_v_ctrl42); // regAddr = 0x212a[1]
	EQ_V_Wr01(eq_v_ctrl42, shortgfbfoffen, shortgfbfoffen);
	EQ_V_WrFL(eq_v_ctrl42);


//	sammodesel_v_1_0 = 0x1;
//	Sync_RdFL(sync_ctrl61); // regAddr = 0x103d[2:0]
//	Sync_Wr01(sync_ctrl61, sammodesel_v_1_0, sammodesel_v_1_0);
//	Sync_WrFL(sync_ctrl61);


	statictrackfbf_2_0 = 0x2;
	EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
	EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl50);


	badstatictrackfbf_2_0 = 0x2;
	EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
	EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl51);


	crgstep_vd_3_0 = 0x7;
	Common_RdFL(common_ctrl73); // regAddr = 0x0049[3:0]
	Common_Wr01(common_ctrl73, crgstep_vd_3_0, crgstep_vd_3_0);
	Common_WrFL(common_ctrl73);


	dyntrackfdeq_3_0 = 0xb;
	EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
	EQ_V_Wr01(eq_v_ctrl53, dyntrackfdeq_3_0, dyntrackfdeq_3_0);
	EQ_V_WrFL(eq_v_ctrl53);


	crdmapgainhigh_3_0 = 0x3;
	EQ_V_RdFL(eq_v_ctrl98); // regAddr = 0x2135[7:4]
	EQ_V_Wr01(eq_v_ctrl98, crdmapgainhigh_3_0, crdmapgainhigh_3_0);
	EQ_V_WrFL(eq_v_ctrl98);



#endif



	if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	return RET_OK;

#else

	UINT8	vconfsel_1_0;
	UINT8 	thmainphnon_7_0;
	UINT8 	thmainphnoff_7_0;
	UINT8	acqstaticfdeq_3_0;
	UINT8	statictrackfdeq_3_0;
	UINT8	dyntrackfdeq_3_0;
	UINT8	dyntrackfbf_2_0;
	UINT8	badphderotbwsel_3_0;
	UINT8	dagcncombbw_2_0;
	UINT8	dagcbwoffset_3_0;
	UINT8	vsbmsegdth_3_0;
	UINT8	vsbnprstth_3_0;
	UINT8	sysctrlth_3_0;
	UINT8	msesysctrlwaittime_5_0;
	UINT8	trkexit_1_0;

	//UINT8	steplockfcnt_7_0;

	UINT8	aiccfixfreq0_23_16, aiccfixfreq0_15_8, aiccfixfreq0_7_0;
	UINT8	aiccfixfreq1_23_16, aiccfixfreq1_15_8, aiccfixfreq1_7_0;
	UINT8	aiccfixfreq2_23_16, aiccfixfreq2_15_8, aiccfixfreq2_7_0;
	UINT8	aiccfixfreq3_23_16;
	UINT8	aiccfixfreq3_15_8;
	UINT8	aiccfixfreq3_7_0;
	UINT8	aiccopmode0_1_0;
//	UINT8	aiccopmode1_1_0;
	UINT8	aiccopmode2_1_0;
//	UINT8	aiccopmode3_1_0;
//	UINT8	aiccalpha_3_0;
	UINT8	diffmainphthd_15_8;
	UINT8	diffmainphthd_7_0;
	UINT8	phnboostphderotbwsel_3_0;
	UINT8	pn63invchken;
	UINT8	cirqen;

	UINT8	crmapbwedl_v_3_0;
	UINT8	burstondiff_5_0;
	UINT8	burststartsnr_5_0;
	UINT8	dagcnormbw_3_0;


	UINT8 	adcvrefsel_0;
	UINT8 	adcclkpllsel;
//	UINT8	burststartsnr_5_0;
	UINT8	crfastsweepcntth_v_2_0;

	UINT8	slowdyntrackfdeq_3_0;
	UINT8	slowdyntrackfbf_2_0;
	UINT8	baddyntrackfdeq_3_0;
	UINT8	baddyntrackfbf_2_0;
	UINT8	badslowdyntrackfbf_2_0;


//	UINT8	chslow_7_0;
//	UINT8	chslowghost_7_0;
	UINT8	chfast_7_0;

//	UINT8	npncombw;
//	UINT8	autocombrev;

	UINT8	samcnormbp_v_1_0;
	UINT8	aiccoffth_15_8, aiccoffth_7_0;




	aiccoffth_15_8 = 0x8;
	Common_RdFL(common_ctrl34); // regAddr = 0x0022[7:0]
	Common_Wr01(common_ctrl34, aiccoffth_15_8, aiccoffth_15_8);
	Common_WrFL(common_ctrl34);

	aiccoffth_7_0 = 0xff;
	Common_RdFL(common_ctrl35); // regAddr = 0x0023[7:0]
	Common_Wr01(common_ctrl35, aiccoffth_7_0, aiccoffth_7_0);
	Common_WrFL(common_ctrl35);

	chfast_7_0 = 0x40;
	EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
	EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
	EQ_V_WrFL(eq_v_ctrl34);

	slowdyntrackfdeq_3_0 = 0x7;
	slowdyntrackfbf_2_0 = 0;
	EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
	EQ_V_Wr02(eq_v_ctrl54,slowdyntrackfdeq_3_0,slowdyntrackfdeq_3_0, slowdyntrackfbf_2_0, slowdyntrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl54);

	baddyntrackfbf_2_0 = 0;
	baddyntrackfdeq_3_0 = 0x6;
	EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
	EQ_V_Wr02(eq_v_ctrl55,baddyntrackfdeq_3_0,baddyntrackfdeq_3_0,baddyntrackfbf_2_0,baddyntrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl55);

	badslowdyntrackfbf_2_0 = 0;
	EQ_V_RdFL(eq_v_ctrl56); // regAddr = 0x2138[2:0]
	EQ_V_Wr01(eq_v_ctrl56,badslowdyntrackfbf_2_0,badslowdyntrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl56);


	samcnormbp_v_1_0 = 0x3;
	Sync_RdFL(sync_ctrl61); // regAddr = 0x103d[5:4]
	Sync_Wr01(sync_ctrl61, samcnormbp_v_1_0, samcnormbp_v_1_0);
	Sync_WrFL(sync_ctrl61);


#if 0
	UINT8	staticacqfbf_2_0;
	UINT8	statictrackfbf_2_0;
	UINT8	badstatictrackfbf_2_0;

	staticacqfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl49); // regAddr = 0x2131[2:0]
	EQ_V_Wr01(eq_v_ctrl49,staticacqfbf_2_0,staticacqfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl49);

	statictrackfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
	EQ_V_Wr01(eq_v_ctrl50,statictrackfbf_2_0,statictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl50);

	badstatictrackfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
	EQ_V_Wr01(eq_v_ctrl51,badstatictrackfbf_2_0,badstatictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl51);

#endif





	DEMOD_TRACE("[DEMOD M14B0 DTV] @ %s =====\n",__F__);


	vconfsel_1_0 = 0; // for fast generation of synclock
	EQ_V_RdFL(eq_v_ctrl0); // regAddr = 0x2100[7:6]
	EQ_V_Wr01(eq_v_ctrl0,vconfsel_1_0,vconfsel_1_0);
	EQ_V_WrFL(eq_v_ctrl0);
	/*
	Select the maximum confidence value for nSynclock
	00 : 0
	01 : 1
	10 : 2
	11 : 3
	*/


	//thmainphnon_7_0 = 0x60;
	//thmainphnon_7_0 = 0x70; // changed because of single static echo 50us NG 2012.07.27
	thmainphnon_7_0 = 0x60;  //changed 2012.08.15
	EQ_V_RdFL(eq_v_ctrl13); // regAddr = 0x210d[7:0]
	EQ_V_Wr01(eq_v_ctrl13,thmainphnon_7_0,thmainphnon_7_0);
	EQ_V_WrFL(eq_v_ctrl13);
	// thmainphnon_7_0 : Turn-on threshold of phase noise control signal by CIR main power.

	//thmainphnoff_7_0 = 0x60;
	//thmainphnoff_7_0 = 0x70; // changed because of single static echo 50us NG 2012.07.27
	thmainphnoff_7_0 = 0x60; //changed 2012.08.15
	EQ_V_RdFL(eq_v_ctrl14); // regAddr = 0x210e[7:0]
	EQ_V_Wr01(eq_v_ctrl14,thmainphnoff_7_0,thmainphnoff_7_0);
	EQ_V_WrFL(eq_v_ctrl14);
	// thmainphnoff_7_0	: Turn-off threshold of phase noise control signal by CIR main power.

	acqstaticfdeq_3_0 = 0x1;
	EQ_V_RdFL(eq_v_ctrl48); // regAddr = 0x2131[7:4]
	EQ_V_Wr01(eq_v_ctrl48,acqstaticfdeq_3_0,acqstaticfdeq_3_0);
	EQ_V_WrFL(eq_v_ctrl48);

	statictrackfdeq_3_0 = 0x1;
	EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[7:4]
	EQ_V_Wr01(eq_v_ctrl50,statictrackfdeq_3_0,statictrackfdeq_3_0);
	EQ_V_WrFL(eq_v_ctrl50);


	////////////////////////add 2012/06/20 for R2.1, R2.2
	dyntrackfbf_2_0 = 0;
	dyntrackfdeq_3_0 = 0xa;
	EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
	EQ_V_Wr02(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0, dyntrackfbf_2_0, dyntrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl53);

//	scctrlveqen = 1;
//	EQ_V_RdFL(eq_v_ctrl103); // regAddr = 0x2167[7]
//	EQ_V_Wr01(eq_v_ctrl103, scctrlveqen, scctrlveqen);
//	EQ_V_WrFL(eq_v_ctrl103);

	/////////////////////////

	badphderotbwsel_3_0 = 0x2;
	EQ_V_RdFL(eq_v_ctrl58); // regAddr = 0x213a[7:4]
	EQ_V_Wr01(eq_v_ctrl58,badphderotbwsel_3_0,badphderotbwsel_3_0);
	EQ_V_WrFL(eq_v_ctrl58);


	//add 12/06/25 agc speed low freq



	dagcncombbw_2_0 = 0x5;
//	dagcbwoffset_3_0 = 0x2;
//  changed to that like below because of R2.2 #1 5Hz NG
//	dagcncombbw_2_0 = 0x4;  // used at NTSC co-channel
	dagcbwoffset_3_0 = 0x1;
	EQ_V_RdFL(eq_v_ctrl84); // regAddr = 0x2154
	EQ_V_Wr02(eq_v_ctrl84,dagcncombbw_2_0,dagcncombbw_2_0, dagcbwoffset_3_0, dagcbwoffset_3_0);
	EQ_V_WrFL(eq_v_ctrl84);


	vsbmsegdth_3_0 = 0xd; //10dB
	vsbnprstth_3_0 = 0xa; //7dB
	EQ_V_RdFL(eq_v_ctrl70); // regAddr = 0x2146
	EQ_V_Wr02(eq_v_ctrl70, vsbmsegdth_3_0, vsbmsegdth_3_0, vsbnprstth_3_0, vsbnprstth_3_0);
	EQ_V_WrFL(eq_v_ctrl70);


	sysctrlth_3_0 = 0x7;
	EQ_V_RdFL(eq_v_ctrl103); // regAddr = 0x2167[3:0]
	EQ_V_Wr01(eq_v_ctrl103, sysctrlth_3_0, sysctrlth_3_0);
	EQ_V_WrFL(eq_v_ctrl103);


	msesysctrlwaittime_5_0 = 0xa;
	EQ_V_RdFL(eq_v_ctrl104); // regAddr = 0x2168[5:0]
	EQ_V_Wr01(eq_v_ctrl104, msesysctrlwaittime_5_0, msesysctrlwaittime_5_0);
	EQ_V_WrFL(eq_v_ctrl104);


	trkexit_1_0 = 0x1;
	EQ_V_RdFL(eq_v_ctrl45); // regAddr = 0x212d[3:2]
	EQ_V_Wr01(eq_v_ctrl45, trkexit_1_0, trkexit_1_0);
	EQ_V_WrFL(eq_v_ctrl45);

//	steplockfcnt_7_0 = 0x8;
//	EQ_V_RdFL(eq_v_ctrl114); // regAddr = 0x2172[7:0]
//	EQ_V_Wr01(eq_v_ctrl114,steplockfcnt_7_0,steplockfcnt_7_0);
//	EQ_V_WrFL(eq_v_ctrl114);


    ////  Register setting for AICC control
	aiccfixfreq0_23_16 = 0x22;
	Common_RdFL(common_ctrl46); // regAddr = 0x002e[7:0]
	Common_Wr01(common_ctrl46, aiccfixfreq0_23_16, aiccfixfreq0_23_16);
	Common_WrFL(common_ctrl46);

	////  Register setting for AICC control
	aiccfixfreq0_15_8 = 0;
	Common_RdFL(common_ctrl47); // regAddr = 0x002f[7:0]
	Common_Wr01(common_ctrl47, aiccfixfreq0_15_8, aiccfixfreq0_15_8);
	Common_WrFL(common_ctrl47);

	////  Register setting for AICC control
	aiccfixfreq0_7_0 = 0;
	Common_RdFL(common_ctrl48); // regAddr = 0x0030[7:0]
	Common_Wr01(common_ctrl48, aiccfixfreq0_7_0, aiccfixfreq0_7_0);
	Common_WrFL(common_ctrl48);



    ////  Register setting for AICC control
	aiccfixfreq1_23_16 = 0x3e;
	Common_RdFL(common_ctrl43); // regAddr = 0x002b[7:0]
	Common_Wr01(common_ctrl43, aiccfixfreq1_23_16, aiccfixfreq1_23_16);
	Common_WrFL(common_ctrl43);

	////  Register setting for AICC control
	aiccfixfreq1_15_8 = 0xcc;
	Common_RdFL(common_ctrl44); // regAddr = 0x002c[7:0]
	Common_Wr01(common_ctrl44, aiccfixfreq1_15_8, aiccfixfreq1_15_8);
	Common_WrFL(common_ctrl44);

	////  Register setting for AICC control
	aiccfixfreq1_7_0 = 0xf4;
	Common_RdFL(common_ctrl45); // regAddr = 0x002d[7:0]
	Common_Wr01(common_ctrl45, aiccfixfreq1_7_0, aiccfixfreq1_7_0);
	Common_WrFL(common_ctrl45);


   ////  Register setting for AICC control
	aiccfixfreq2_23_16 = 0x46;
	Common_RdFL(common_ctrl40); // regAddr = 0x0028[7:0]
	Common_Wr01(common_ctrl40, aiccfixfreq2_23_16, aiccfixfreq2_23_16);
	Common_WrFL(common_ctrl40);

	////  Register setting for AICC control
	aiccfixfreq2_15_8 = 0;
	Common_RdFL(common_ctrl41); // regAddr = 0x0029[7:0]
	Common_Wr01(common_ctrl41, aiccfixfreq2_15_8, aiccfixfreq2_15_8);
	Common_WrFL(common_ctrl41);

	////  Register setting for AICC control
	aiccfixfreq2_7_0 = 0;
	Common_RdFL(common_ctrl42); // regAddr = 0x002a[7:0]
	Common_Wr01(common_ctrl42, aiccfixfreq2_7_0, aiccfixfreq2_7_0);
	Common_WrFL(common_ctrl42);


	////  Register setting for AICC control
	aiccfixfreq3_23_16 = 0x22;
	Common_RdFL(common_ctrl37); // regAddr = 0x0025[7:0]
	Common_Wr01(common_ctrl37, aiccfixfreq3_23_16, aiccfixfreq3_23_16);
	Common_WrFL(common_ctrl37);

	////  Register setting for AICC control
	aiccfixfreq3_15_8 = 0x0;
	Common_RdFL(common_ctrl38); // regAddr = 0x0026[7:0]
	Common_Wr01(common_ctrl38, aiccfixfreq3_15_8, aiccfixfreq3_15_8);
	Common_WrFL(common_ctrl38);

	////  Register setting for AICC control
	aiccfixfreq3_7_0 = 0x0;
	Common_RdFL(common_ctrl39); // regAddr = 0x0027[7:0]
	Common_Wr01(common_ctrl39, aiccfixfreq3_7_0, aiccfixfreq3_7_0);
	Common_WrFL(common_ctrl39);

#if 0
aiccopmode0_1_0 = 0;
aiccopmode1_1_0 = 0;
aiccopmode2_1_0 = 0;
aiccopmode3_1_0 = 0;
Common_RdFL(common_ctrl36); // regAddr = 0x0024[7:0]
Common_Wr04(common_ctrl36, aiccopmode0_1_0, aiccopmode0_1_0, aiccopmode1_1_0, aiccopmode1_1_0, aiccopmode2_1_0, aiccopmode2_1_0, aiccopmode3_1_0, aiccopmode3_1_0);
Common_WrFL(common_ctrl36);


aiccalpha_3_0 = 0xa;
Common_RdFL(common_ctrl30); // regAddr = 0x001e[7:4]
Common_Wr01(common_ctrl30, aiccalpha_3_0, aiccalpha_3_0);
Common_WrFL(common_ctrl30);
#else


aiccopmode0_1_0 = 2;
aiccopmode2_1_0 = 2;

Common_RdFL(common_ctrl36); // regAddr = 0x0024[7:0]
Common_Wr02(common_ctrl36, aiccopmode2_1_0, aiccopmode2_1_0,aiccopmode0_1_0,aiccopmode0_1_0);
Common_WrFL(common_ctrl36);


//aiccalpha_3_0 = 0xa;
//Common_RdFL(common_ctrl30); // regAddr = 0x001e[7:4]
//Common_Wr01(common_ctrl30, aiccalpha_3_0, aiccalpha_3_0);
//Common_WrFL(common_ctrl30);



#endif



    // added 2012.8.15
	// phase noise detector
   	diffmainphthd_15_8 = 0x6;
	EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
	EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
	EQ_V_WrFL(eq_v_ctrl38);

	diffmainphthd_7_0 = 0xff;
	EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
	EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
	EQ_V_WrFL(eq_v_ctrl39);


	phnboostphderotbwsel_3_0 = 0xc;
	EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
	EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0, phnboostphderotbwsel_3_0);
	EQ_V_WrFL(eq_v_ctrl59);

	//Disable PN63 inversion check
	pn63invchken = 0x0;
	EQ_V_RdFL(eq_v_ctrl105); // regAddr = 0x2169[0]
	EQ_V_Wr01(eq_v_ctrl105, pn63invchken, pn63invchken);
	EQ_V_WrFL(eq_v_ctrl105);


	//CIR loading
	cirqen = 0x0;
	EQ_V_RdFL(eq_v_ctrl31); // regAddr = 0x211f[4]
	EQ_V_Wr01(eq_v_ctrl31, cirqen, cirqen);
	EQ_V_WrFL(eq_v_ctrl31);


	crmapbwedl_v_3_0 = 0x4;
	Sync_RdFL(sync_ctrl97); // regAddr = 0x1061[3:0]
	Sync_Wr01(sync_ctrl97, crmapbwedl_v_3_0, crmapbwedl_v_3_0);
	Sync_WrFL(sync_ctrl97);

	burstondiff_5_0 = 0xd;
	EQ_V_RdFL(eq_v_ctrl75); // regAddr = 0x214b[5:0]
	EQ_V_Wr01(eq_v_ctrl75,burstondiff_5_0,burstondiff_5_0);
	EQ_V_WrFL(eq_v_ctrl75);

	burststartsnr_5_0 = 0x18;
	EQ_V_RdFL(eq_v_ctrl79); // regAddr = 0x214f[5:0]
	EQ_V_Wr01(eq_v_ctrl79,burststartsnr_5_0,burststartsnr_5_0);
	EQ_V_WrFL(eq_v_ctrl79);


	dagcnormbw_3_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[3:0]
	EQ_V_Wr01(eq_v_ctrl85,dagcnormbw_3_0,dagcnormbw_3_0);
	EQ_V_WrFL(eq_v_ctrl85);




	#if 0
	chslow_7_0 = 0x40;
	EQ_V_RdFL(eq_v_ctrl32); // regAddr = 0x2120[7:0]
	EQ_V_Wr01(eq_v_ctrl32,chslow_7_0,chslow_7_0);
	EQ_V_WrFL(eq_v_ctrl32);

	chslowghost_7_0 = 0x40;
	EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
	EQ_V_Wr01(eq_v_ctrl33,chslowghost_7_0,chslowghost_7_0);
	EQ_V_WrFL(eq_v_ctrl33);

	chfast_7_0 = 0x60;
	EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
	EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
	EQ_V_WrFL(eq_v_ctrl34);





	npncombw = 0x1;
	EQ_V_RdFL(eq_v_ctrl28); // regAddr = 0x211c[7]
	EQ_V_Wr01(eq_v_ctrl28,npncombw,npncombw);
	EQ_V_WrFL(eq_v_ctrl28);
	#endif

//	autocombrev = 0;
//	EQ_V_RdFL(eq_v_ctrl28); // regAddr = 0x211c[7]
//	EQ_V_Wr01(eq_v_ctrl28,autocombrev,autocombrev);
//	EQ_V_WrFL(eq_v_ctrl28);

	adcvrefsel_0 = 1;
	adcclkpllsel = 0;
	Common_RdFL(common_ctrl4);   //regAddr = 0x0004[7], COMMON_CTRL[4]
	Common_Wr02(common_ctrl4, adcvrefsel_0, adcvrefsel_0, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	//ADCCLKPLLSEL : Selection of ADC sampling clock source.  This is not validated at DTV SoC
	// 0 : use external oscillator input
	// 1 : use PLL output


	// adding because of pull in range 2012.07.24
	crfastsweepcntth_v_2_0 = 0;
	Sync_RdFL(sync_ctrl114); // regAddr = 0x1072[2:0]
	Sync_Wr01(sync_ctrl114, crfastsweepcntth_v_2_0, crfastsweepcntth_v_2_0);
	Sync_WrFL(sync_ctrl114);

	_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;
	_gM14B0GBB_Phn_state = 0;
	_gM14B0GBB_ChdiffTh = 0xf;
	_gM14B0GBB_Tpblockon = FALSE;

#if 0
#endif
	if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


	return RET_OK;


#endif

#if 0
#endif


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_VSB_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 scsyslockchktime_7_0;

	DEMOD_PRINT("[DEMOD M14B0 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = (UINT8)(waitMs / 10);

	Common_Wr01(common_ctrl11,scsyslockchktime_7_0,scsyslockchktime_7_0);
	Common_WrFL(common_ctrl11);  //regAddr = 0x000b,COMMON_CTRL[11]
	//SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in msec resolution

	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_CochannelExist.
 * Get the informantion if co-channel(NTSC) signal exists or not on VSB mode.
 *
 * @param  *pCochannel [OUT] BOOLEAN - exists /doesn't exist.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_VSB_CochannelExist(BOOLEAN *pCochannel)
{

#if 1

	*pCochannel = FALSE;
	return RET_OK;

#else
//	UINT8	fbdlycir_9_8, fbdlycir_7_0;
//	UINT16	ghost_length;
//	UINT8	ncombdet;
	UINT8	aiccrejstatus_3_0;





#if 0
	UINT8	maxpwrmain_7_0;
	UINT16	maxpwrmain;
	UINT8	thpathcnt_3_0;



	EQ_V_RdFL(eq_v_mon8); // regAddr = 0x2188[3:0]
	EQ_V_Rd01(eq_v_mon8,thpathcnt_3_0, thpathcnt_3_0);






	EQ_V_RdFL(eq_v_mon5); // regAddr = 0x2185[7:0] :
	EQ_V_Rd01(eq_v_mon5,maxpwrmain_7_0, maxpwrmain_7_0);

	maxpwrmain = (maxpwrmain_7_0 << 0);


	if( (maxpwrmain<0x550) && (thpathcnt_3_0<=4) )
	{
		//*pSingleStatic = TRUE;
		//printk("chungiiii ######### SingleStatic is 20us[0x%x]\n", maxpwrmain);
	}
	else
	{
		//*pSingleStatic = FALSE;
		//printk("chungiiii ######### SingleStatic is not 20us[0x%x]\n", maxpwrmain);
	}

#endif



#if 0
	EQ_V_RdFL(eq_v_mon0); // regAddr = 	0x2180[1:0]
	EQ_V_Rd01(eq_v_mon0,fbdlycir_9_8, fbdlycir_9_8);

	EQ_V_RdFL(eq_v_mon1); // regAddr = 	0x2181[7:0]
	EQ_V_Rd01(eq_v_mon1,fbdlycir_7_0, fbdlycir_7_0);

	ghost_length = (fbdlycir_9_8 << 8) + fbdlycir_7_0;

	if( (ghost_length<=0x9e) && (ghost_length>=0x68) )
	{
		*pSingleStatic = TRUE;
		printk("chungiiii ######### SingleStatic is 20us[0x%x]\n", ghost_length);
	}
	else
	{
		*pSingleStatic = FALSE;
		printk("chungiiii ######### SingleStatic is not 20us[0x%x]\n", ghost_length);
	}

#endif

#if 0
	EQ_V_RdFL(eq_v_mon17);  //regAddr = 0x2191[7],
	EQ_V_Rd01(eq_v_mon17, ncombdet, ncombdet);

	if(ncombdet)
	{
		*pCochannel = FALSE;
		printk(" ###chungiii DEBUG ## NTSC IS DETECTED [%s][%d]\n",ncombdet ? "NO" : "YES", ncombdet);
	}
	else
	{
		*pCochannel = TRUE;
		printk(" ###chungiii DEBUG ## NTSC IS DETECTED [%s][%d]\n",ncombdet ? "NO" : "YES", ncombdet);

	}
#else


	Common_RdFL(common_mon16);  //regAddr = 0x0090,
	Common_Rd01(common_mon16,aiccrejstatus_3_0,aiccrejstatus_3_0);


	if( ((aiccrejstatus_3_0 == 0x5) || (aiccrejstatus_3_0 == 0x1)) )
	{
		*pCochannel = TRUE;
		DEMOD_TRACE(" ###chungiii DEBUG ## PRE MONITOR  TRUE aiccrejstatus_3_0 value[%d]\n", aiccrejstatus_3_0);
	}
	else
	{
		*pCochannel = FALSE;
		DEMOD_TRACE(" ###chungiii DEBUG ## PRE MONITOR FALSE aiccrejstatus_3_0 value[%d]\n", aiccrejstatus_3_0);
	}
#endif





	return RET_OK;


#endif



}






/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_PreMonitor.
 * Set registers against  various noise conditions before  monitoring.
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/


int DEMOD_M14B0_VSB_PreMonitor(void)
{

	DEMOD_TRACE("[DEMOD M14B0 DTV] @ %s =====\n",__F__);

#if 1

//	UINT8	maxpwrmain_7_0;
//	UINT8	cnfcnttpif_7_0;
//	UINT8	badstatictrackfdeq_3_0;
//	UINT8	badphderotbwsel_3_0, baddynphderotbwsel_3_0;

//	UINT8	samcnormbp_v_1_0, sammodesel_v_1_0;

	UINT8	chmode_1_0;
	UINT8	preghost;
	UINT8	statictrackfbf_2_0;
	UINT8	badstatictrackfbf_2_0;

	UINT8	crdmapgainhigh_3_0;

	UINT8	crmapbwedl_v_3_0;
	UINT8	crpgain_v_3_0;


	UINT8	crgstep_vd_3_0;
	UINT8	agcifbw_3_0;

	UINT8	fbdlycir_9_8;
	UINT8	fbdlycir_7_0;
	UINT16	fbdly;


	EQ_V_RdFL(eq_v_mon0);  //regAddr = 0x2180[1]
	EQ_V_Rd01(eq_v_mon0, fbdlycir_9_8, fbdlycir_9_8);
	fbdly = ( fbdlycir_9_8 << 8);

	EQ_V_RdFL(eq_v_mon1);  //regAddr = 0x2181[7:0]
	EQ_V_Rd01(eq_v_mon1, fbdlycir_7_0, fbdlycir_7_0);
	fbdly += fbdlycir_7_0;


	EQ_V_RdFL(eq_v_mon19);  //regAddr = 0x2193[1:0],
	EQ_V_Rd01(eq_v_mon19, chmode_1_0, chmode_1_0);

	EQ_V_RdFL(eq_v_mon9);  //regAddr = 0x2189[7:0]
	EQ_V_Rd01(eq_v_mon9, preghost, preghost);



	crdmapgainhigh_3_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl98); // regAddr = 0x2135[7:4]
	EQ_V_Wr01(eq_v_ctrl98, crdmapgainhigh_3_0, crdmapgainhigh_3_0);
	EQ_V_WrFL(eq_v_ctrl98);


	if((chmode_1_0!=0x0) && (fbdly>0x70) && (fbdly!=0x3ff))
	{

		statictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}
	else
	{

		statictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}



#if 0
	if((chmode_1_0==1) && (preghost==1))
	{

		statictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}
	else
	{

		statictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}

#endif





	crmapbwedl_v_3_0 = 0x4;
	Sync_RdFL(sync_ctrl97); // regAddr = 0x1061[6:4]
	Sync_Wr01(sync_ctrl97, crmapbwedl_v_3_0, crmapbwedl_v_3_0);
	Sync_WrFL(sync_ctrl97);

	crpgain_v_3_0 = 0x9;
	Sync_RdFL(sync_ctrl115); // regAddr = 0x1073[7:4]
	Sync_Wr01(sync_ctrl115, crpgain_v_3_0, crpgain_v_3_0);
	Sync_WrFL(sync_ctrl115);


	agcifbw_3_0 = 0x8;
	Common_RdFL(common_ctrl24); // regAddr = 0x0018[3:0]
	Common_Wr01(common_ctrl24, agcifbw_3_0, agcifbw_3_0);
	Common_WrFL(common_ctrl24);

	crgstep_vd_3_0 = 0x9;
	Common_RdFL(common_ctrl73); // regAddr = 0x0049[3:0]
	Common_Wr01(common_ctrl73, crgstep_vd_3_0, crgstep_vd_3_0);
	Common_WrFL(common_ctrl73);



	return RET_OK;

#else  //////////////////////// end of start

	UINT8	chdiff_7_0;
	UINT8	snrref_5_0;
	UINT8	mainstrong;

	UINT8	dyntrackfdeq_3_0;
	UINT8	dyntrackfbf_2_0;
	UINT8	samzgsel_v_2_0;
	UINT8	statictrackfdeq_3_0;
//	UINT8	dagcnormbw_3_0;
	UINT8	burstondiff_5_0;
	UINT8	burststartsnr_5_0;
	UINT8	phnboostphderotbwsel_3_0;
//	UINT8	thmainphnon_7_0;
//	UINT8	thmainphnoff_7_0;
	UINT8	maxpwrmain_7_0;
	UINT16	maxpwrmain;
	UINT8	trmode_1_0;

	UINT8	diffmainphthd_15_8;
	UINT8	diffmainphthd_7_0;

	UINT8	trkexit_1_0;

	UINT8	slowdyntrackfdeq_3_0;
	UINT8	baddyntrackfdeq_3_0;

	UINT8	acqstaticfdeq_3_0;
	UINT8	ncombdet;

//	UINT8	chslow_7_0;
//	UINT8	chslowghost_7_0;
	UINT8	chfast_7_0;

	UINT8	delaydiff_7_0;
	UINT8	delaydiffcragc_10_8;
	UINT8	delaydiffcragc_7_0;
	UINT16	delaydiffcragc;
	UINT8	slowdyntrackfbf_2_0;

	UINT8	baddyntrackfbf_2_0;
	UINT8	badslowdyntrackfbf_2_0;
//	UINT8	aiccrejstatus_3_0;

//	UINT8	aiccopmode0_1_0;
//	UINT8	aiccopmode1_1_0;
//	UINT8	aiccopmode2_1_0;
//	UINT8	aiccopmode3_1_0;
//	UINT8	npncombw;

	UINT8	dlyspread_7_0;
	UINT8	preghost;
	UINT8	ncombstatevsb;

	EQ_V_RdFL(eq_v_mon17);  //regAddr = 0x2191[6],
	EQ_V_Rd01(eq_v_mon17, ncombstatevsb, ncombstatevsb);


	EQ_V_RdFL(eq_v_mon2);  //regAddr = 0x2182[2:0]
	EQ_V_Rd01(eq_v_mon2, delaydiffcragc_10_8, delaydiffcragc_10_8);

	EQ_V_RdFL(eq_v_mon3);  //regAddr = 0x2180[7:0]
	EQ_V_Rd01(eq_v_mon3, delaydiffcragc_7_0, delaydiffcragc_7_0);

	delaydiffcragc = (delaydiffcragc_10_8<<8) + (delaydiffcragc_7_0);

#if 0
	UINT8	fbdlycir_9_8;
	UINT8	fbdlycir_7_0;
	UINT16	fbdly;


	EQ_V_RdFL(eq_v_mon0);  //regAddr = 0x2180[1]
	EQ_V_Rd01(eq_v_mon0, fbdlycir_9_8, fbdlycir_9_8);
	printk(" ###chungiii DEBUG PRE MONITOR## Long Post Ghost[%s][%d]\n",(fbdlycir_9_8 >> 1) ? "NO" : "YES", (fbdlycir_9_8 >> 1));
	fbdly = ( fbdlycir_9_8 << 8);

	EQ_V_RdFL(eq_v_mon1);  //regAddr = 0x2181[7:0]
	EQ_V_Rd01(eq_v_mon1, fbdlycir_7_0, fbdlycir_7_0);
	fbdly += fbdlycir_7_0;
	printk(" ###chungiii DEBUG PRE MONITOR## Ghost length is [%d]\n", fbdly);

#endif
////////////////////////////////////////////////

	//printk(" ###chungiii DEBUG ## PRE MONITOR  _gM14B0GBB_ChdiffTh value[0x%x]\n", _gM14B0GBB_ChdiffTh);

	Common_RdFL(common_mon71);  //regAddr = 0x00c7[7:0],
	Common_Rd01(common_mon71, dlyspread_7_0, dlyspread_7_0);

	EQ_V_RdFL(eq_v_mon9);  //regAddr = 0x2189[7:0]
	EQ_V_Rd01(eq_v_mon9, preghost, preghost);
	DEMOD_TRACE(" ###chungiii DEBUG ## PRE MONITOR Preghost?[%s][%d]\n",preghost ? "YES" : "NO", preghost);

	EQ_V_RdFL(eq_v_mon13); // regAddr = 0x218d[7:0]
	EQ_V_Rd01(eq_v_mon13,delaydiff_7_0, delaydiff_7_0);


	EQ_V_RdFL(eq_v_mon20); // regAddr = 0x2194[7:0] : chdiff : Channel difference between current channel and previous channel.
	EQ_V_Rd01(eq_v_mon20,chdiff_7_0, chdiff_7_0);


	EQ_V_RdFL(eq_v_mon33); // regAddr = 0x21a1[5:0] : SNR measured by frame synchronization sequences.
	EQ_V_Rd01(eq_v_mon33,snrref_5_0, snrref_5_0);


	EQ_V_RdFL(eq_v_mon25); // regAddr = 0x2199[6]   1:Strong Main CIR
	EQ_V_Rd01(eq_v_mon25,mainstrong, mainstrong);



	EQ_V_RdFL(eq_v_mon5); // regAddr = 0x2185[7:0] :
	EQ_V_Rd01(eq_v_mon5,maxpwrmain_7_0, maxpwrmain_7_0);

	maxpwrmain = (maxpwrmain_7_0 << 0);

	if( (mainstrong == 1) && (chdiff_7_0>_gM14B0GBB_ChdiffTh) && (snrref_5_0<=0x12))
	{
		_gM14B0GBB_Tpblockon = TRUE;
		DEMOD_TRACE(" ###chungiii DEBUG ## _gM14B0GBB_Tpblockon  TRUE\n");
		DEMOD_TRACE(" ###chungiii DEBUG ## mainstrong[%d]\n",mainstrong );
		DEMOD_TRACE(" ###chungiii DEBUG ## delaydiff_7_0[0x%x]\n", delaydiff_7_0);
		DEMOD_TRACE(" ###chungiii DEBUG ## chdiff_7_0 [0x%x]\n", chdiff_7_0);
		DEMOD_TRACE(" ###chungiii DEBUG ## snrref_5_0 [0x%x]\n", snrref_5_0);


	}
	else
	{
		_gM14B0GBB_Tpblockon = FALSE;
		DEMOD_TRACE(" ###chungiii DEBUG ## _gM14B0GBB_Tpblockon  FALSE\n");
		DEMOD_TRACE(" ###chungiii DEBUG ## mainstrong[%d]\n",mainstrong );
		DEMOD_TRACE(" ###chungiii DEBUG ## delaydiff_7_0[0x%x]\n", delaydiff_7_0);
		DEMOD_TRACE(" ###chungiii DEBUG ## chdiff_7_0 [0x%x]\n", chdiff_7_0);
		DEMOD_TRACE(" ###chungiii DEBUG ## snrref_5_0 [0x%x]\n", snrref_5_0);
	}

		DEMOD_TRACE(" ###chungiii DEBUG ## maxpwrmain [0x%x]\n", maxpwrmain);
		DEMOD_TRACE(" ###chungiii DEBUG ## dlyspread_7_0 [0x%x]\n", dlyspread_7_0);
		DEMOD_TRACE(" ###chungiii DEBUG ## ncombstatevsb [%d]\n", ncombstatevsb);


	EQ_V_RdFL(eq_v_mon17);  //regAddr = 0x2191[7],
	EQ_V_Rd01(eq_v_mon17, ncombdet, ncombdet);
	//printk(" ###chungiii DEBUG ## PRE MONITOR  NTSC IS DETECTED [%s][%d]\n",ncombdet ? "NO" : "YES", ncombdet);

//	Common_RdFL(common_mon16);  //regAddr = 0x0090,
//	Common_Rd01(common_mon16,aiccrejstatus_3_0,aiccrejstatus_3_0);

	DEMOD_TRACE(" ###chungiii DEBUG ## chdiff_7_0[0x%x] snrref_5_0[0x%x] mainstrong[%d] maxpwrmain[0x%x]\n", chdiff_7_0, snrref_5_0, mainstrong, maxpwrmain);


	//if((mainstrong==1) && (delaydiff_7_0<0xc0))
	if((mainstrong==1) && (dlyspread_7_0<0xc0) && (preghost==0))
	{
		DEMOD_TRACE(" #######################chungiii No ghost or verey weak ghost signal\n");
		dyntrackfdeq_3_0 = 0x7;
		EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
		EQ_V_Wr01(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0);
		EQ_V_WrFL(eq_v_ctrl53);

		trmode_1_0=0x0;
		EQ_V_RdFL(eq_v_ctrl65); // regAddr = 0x2141[7:6]
		EQ_V_Wr01(eq_v_ctrl65, trmode_1_0, trmode_1_0);
		EQ_V_WrFL(eq_v_ctrl65);

		chfast_7_0 = 0x70;
		EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
		EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
		EQ_V_WrFL(eq_v_ctrl34);


		//if(chdiff_7_0>0x7)
		if(chdiff_7_0>_gM14B0GBB_ChdiffTh)
		{
			DEMOD_TRACE(" #######################chungiii slow dynamic or fast dynamic channel\n");


			samzgsel_v_2_0 = 0x3;
			Sync_RdFL(sync_ctrl63);  //regAddr = 0x103f[6:4]
			Sync_Wr01(sync_ctrl63, samzgsel_v_2_0, samzgsel_v_2_0);
			Sync_WrFL(sync_ctrl63);

			statictrackfdeq_3_0 = 0x7;
			EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[7:4]
			EQ_V_Wr01(eq_v_ctrl50,statictrackfdeq_3_0,statictrackfdeq_3_0);
			EQ_V_WrFL(eq_v_ctrl50);

//			dagcnormbw_3_0 = 0x0;
//			EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[3:0]
//			EQ_V_Wr01(eq_v_ctrl85,dagcnormbw_3_0,dagcnormbw_3_0);
//			EQ_V_WrFL(eq_v_ctrl85);

			burstondiff_5_0 = 0xd;
			EQ_V_RdFL(eq_v_ctrl75); // regAddr = 0x214b[5:0]
			EQ_V_Wr01(eq_v_ctrl75,burstondiff_5_0,burstondiff_5_0);
			EQ_V_WrFL(eq_v_ctrl75);

			burststartsnr_5_0 = 0x18;
			EQ_V_RdFL(eq_v_ctrl79); // regAddr = 0x214f[5:0]
			EQ_V_Wr01(eq_v_ctrl79,burststartsnr_5_0,burststartsnr_5_0);
			EQ_V_WrFL(eq_v_ctrl79);

	//		diffmainphthd_15_8=0x3;
	//		EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
	//		EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
	//		EQ_V_WrFL(eq_v_ctrl38);

	//		diffmainphthd_7_0=0xff;
	//		EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
	//		EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
	//		EQ_V_WrFL(eq_v_ctrl39);


			if(snrref_5_0>0x17)
			{


				phnboostphderotbwsel_3_0 = 0xc;
				EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
				EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0,phnboostphderotbwsel_3_0);
				EQ_V_WrFL(eq_v_ctrl59);

				diffmainphthd_15_8=0x3;
				EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
				EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
				EQ_V_WrFL(eq_v_ctrl38);

				diffmainphthd_7_0=0xff;
				EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
				EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
				EQ_V_WrFL(eq_v_ctrl39);

			}
			else
			{


				phnboostphderotbwsel_3_0 = 0x8;
				EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
				EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0,phnboostphderotbwsel_3_0);
				EQ_V_WrFL(eq_v_ctrl59);

				slowdyntrackfdeq_3_0 = 0x2;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54,slowdyntrackfdeq_3_0,slowdyntrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfdeq_3_0 = 0x2;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfdeq_3_0,baddyntrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl55);

				diffmainphthd_15_8=0x6;
				EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
				EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
				EQ_V_WrFL(eq_v_ctrl38);

				diffmainphthd_7_0=0xff;
				EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
				EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
				EQ_V_WrFL(eq_v_ctrl39);

			}

		}
		else
		{
			DEMOD_TRACE(" #######################chungiii static channel\n");



			samzgsel_v_2_0 = 0x0;
			Sync_RdFL(sync_ctrl63);  //regAddr = 0x103f[6:4]
			Sync_Wr01(sync_ctrl63, samzgsel_v_2_0, samzgsel_v_2_0);
			Sync_WrFL(sync_ctrl63);



			phnboostphderotbwsel_3_0 = 0xc;
			EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
			EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0,phnboostphderotbwsel_3_0);
			EQ_V_WrFL(eq_v_ctrl59);

			diffmainphthd_15_8=0x6;
			EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
			EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
			EQ_V_WrFL(eq_v_ctrl38);

			diffmainphthd_7_0=0xff;
			EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
			EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
			EQ_V_WrFL(eq_v_ctrl39);


			if(snrref_5_0>0x16)
			{
				statictrackfdeq_3_0 = 0x7;
				EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[7:4]
				EQ_V_Wr01(eq_v_ctrl50,statictrackfdeq_3_0,statictrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl50);

//				dagcnormbw_3_0 = 0x0;
//				EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[3:0]
//				EQ_V_Wr01(eq_v_ctrl85,dagcnormbw_3_0,dagcnormbw_3_0);
//				EQ_V_WrFL(eq_v_ctrl85);

				burstondiff_5_0 = 0xd;
				EQ_V_RdFL(eq_v_ctrl75); // regAddr = 0x214b[5:0]
				EQ_V_Wr01(eq_v_ctrl75,burstondiff_5_0,burstondiff_5_0);
				EQ_V_WrFL(eq_v_ctrl75);

				burststartsnr_5_0 = 0x18;
				EQ_V_RdFL(eq_v_ctrl79); // regAddr = 0x214f[5:0]
				EQ_V_Wr01(eq_v_ctrl79,burststartsnr_5_0,burststartsnr_5_0);
				EQ_V_WrFL(eq_v_ctrl79);

				trkexit_1_0 = 0x2;
				EQ_V_RdFL(eq_v_ctrl45); // regAddr = 0x213e[7:6]
				EQ_V_Wr01(eq_v_ctrl45, trkexit_1_0, trkexit_1_0);
				EQ_V_WrFL(eq_v_ctrl45);


			}
			else
			{
				statictrackfdeq_3_0 = 0x1;
				EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[7:4]
				EQ_V_Wr01(eq_v_ctrl50,statictrackfdeq_3_0,statictrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl50);

//				dagcnormbw_3_0 = 0x4;
//				EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[3:0]
//				EQ_V_Wr01(eq_v_ctrl85,dagcnormbw_3_0,dagcnormbw_3_0);
//				EQ_V_WrFL(eq_v_ctrl85);

				burstondiff_5_0 = 0x8;
				EQ_V_RdFL(eq_v_ctrl75); // regAddr = 0x214b[5:0]
				EQ_V_Wr01(eq_v_ctrl75,burstondiff_5_0,burstondiff_5_0);
				EQ_V_WrFL(eq_v_ctrl75);

				burststartsnr_5_0 = 0x12;
				EQ_V_RdFL(eq_v_ctrl79); // regAddr = 0x214f[5:0]
				EQ_V_Wr01(eq_v_ctrl79,burststartsnr_5_0,burststartsnr_5_0);
				EQ_V_WrFL(eq_v_ctrl79);


				slowdyntrackfdeq_3_0 = 0x2;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54,slowdyntrackfdeq_3_0,slowdyntrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfdeq_3_0 = 0x2;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfdeq_3_0,baddyntrackfdeq_3_0);
				EQ_V_WrFL(eq_v_ctrl55);




			}




		}


	}
	else
	{

		DEMOD_TRACE(" #######################chungiii ghost or co-channel interference exists\n");

//		dyntrackfdeq_3_0 = 0xa;
//		EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
//		EQ_V_Wr01(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0);
//		EQ_V_WrFL(eq_v_ctrl53);

		trmode_1_0=0x1;
		EQ_V_RdFL(eq_v_ctrl65); // regAddr = 0x2141[7:6]
		EQ_V_Wr01(eq_v_ctrl65, trmode_1_0, trmode_1_0);
		EQ_V_WrFL(eq_v_ctrl65);

		diffmainphthd_15_8=0x6;
		EQ_V_RdFL(eq_v_ctrl38); // regAddr = 0x2126[7:0]
		EQ_V_Wr01(eq_v_ctrl38, diffmainphthd_15_8, diffmainphthd_15_8);
		EQ_V_WrFL(eq_v_ctrl38);

		diffmainphthd_7_0=0xff;
		EQ_V_RdFL(eq_v_ctrl39); // regAddr = 0x2127[7:0]
		EQ_V_Wr01(eq_v_ctrl39, diffmainphthd_7_0, diffmainphthd_7_0);
		EQ_V_WrFL(eq_v_ctrl39);


		statictrackfdeq_3_0 = 0x7;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[7:4]
		EQ_V_Wr01(eq_v_ctrl50,statictrackfdeq_3_0,statictrackfdeq_3_0);
		EQ_V_WrFL(eq_v_ctrl50);

//		dagcnormbw_3_0 = 0x0;
//		EQ_V_RdFL(eq_v_ctrl85); // regAddr = 0x2155[3:0]
//		EQ_V_Wr01(eq_v_ctrl85,dagcnormbw_3_0,dagcnormbw_3_0);
//		EQ_V_WrFL(eq_v_ctrl85);



		phnboostphderotbwsel_3_0 = 0xc;
		EQ_V_RdFL(eq_v_ctrl59); // regAddr = 0x213b[7:4]
		EQ_V_Wr01(eq_v_ctrl59, phnboostphderotbwsel_3_0,phnboostphderotbwsel_3_0);
		EQ_V_WrFL(eq_v_ctrl59);

		burstondiff_5_0 = 0xd;
		EQ_V_RdFL(eq_v_ctrl75); // regAddr = 0x214b[5:0]
		EQ_V_Wr01(eq_v_ctrl75,burstondiff_5_0,burstondiff_5_0);
		EQ_V_WrFL(eq_v_ctrl75);

		burststartsnr_5_0 = 0x18;
		EQ_V_RdFL(eq_v_ctrl79); // regAddr = 0x214f[5:0]
		EQ_V_Wr01(eq_v_ctrl79,burststartsnr_5_0,burststartsnr_5_0);
		EQ_V_WrFL(eq_v_ctrl79);



		acqstaticfdeq_3_0 = 0x7;
		EQ_V_RdFL(eq_v_ctrl48); // regAddr = 0x2131[7:4]
		EQ_V_Wr01(eq_v_ctrl48,acqstaticfdeq_3_0,acqstaticfdeq_3_0);
		EQ_V_WrFL(eq_v_ctrl48);

		chfast_7_0 = 0x40;
		EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
		EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
		EQ_V_WrFL(eq_v_ctrl34);

		//if(chdiff_7_0>0xf)
		if((chdiff_7_0>_gM14B0GBB_ChdiffTh))
		{
			DEMOD_TRACE(" #######################chungiii slow dynamic or fast dynamic channel\n");



			samzgsel_v_2_0 = 0x3;
			Sync_RdFL(sync_ctrl63);  //regAddr = 0x103f[6:4]
			Sync_Wr01(sync_ctrl63, samzgsel_v_2_0, samzgsel_v_2_0);
			Sync_WrFL(sync_ctrl63);

			if(delaydiff_7_0>0xc0)
			{
				dyntrackfbf_2_0 = 2;
				dyntrackfdeq_3_0 = 0x7;
				EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
				EQ_V_Wr02(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0, dyntrackfbf_2_0, dyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl53);

				slowdyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54, slowdyntrackfbf_2_0, slowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfbf_2_0,baddyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl55);

				badslowdyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl56); // regAddr = 0x2138[2:0]
				EQ_V_Wr01(eq_v_ctrl56,badslowdyntrackfbf_2_0,badslowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl56);

			}
			else
			{
				dyntrackfbf_2_0 = 0;
				dyntrackfdeq_3_0 = 0xa;
				EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
				EQ_V_Wr02(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0, dyntrackfbf_2_0, dyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl53);

				slowdyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54, slowdyntrackfbf_2_0, slowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfbf_2_0,baddyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl55);

				badslowdyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl56); // regAddr = 0x2138[2:0]
				EQ_V_Wr01(eq_v_ctrl56,badslowdyntrackfbf_2_0,badslowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl56);


			}
		}
		else
		{


			samzgsel_v_2_0 = 0x0;
			Sync_RdFL(sync_ctrl63);  //regAddr = 0x103f[6:4]
			Sync_Wr01(sync_ctrl63, samzgsel_v_2_0, samzgsel_v_2_0);
			Sync_WrFL(sync_ctrl63);

			if(delaydiff_7_0>0xc0)
			{
				dyntrackfbf_2_0 = 2;
				dyntrackfdeq_3_0 = 0x7;
				EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
				EQ_V_Wr02(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0, dyntrackfbf_2_0, dyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl53);

				slowdyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54, slowdyntrackfbf_2_0, slowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfbf_2_0,baddyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl55);

				badslowdyntrackfbf_2_0 = 2;
				EQ_V_RdFL(eq_v_ctrl56); // regAddr = 0x2138[2:0]
				EQ_V_Wr01(eq_v_ctrl56,badslowdyntrackfbf_2_0,badslowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl56);

			}
			else
			{
				dyntrackfbf_2_0 = 0;
				dyntrackfdeq_3_0 = 0xa;
				EQ_V_RdFL(eq_v_ctrl53); // regAddr = 0x2135[7:4]
				EQ_V_Wr02(eq_v_ctrl53,dyntrackfdeq_3_0,dyntrackfdeq_3_0, dyntrackfbf_2_0, dyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl53);

				slowdyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl54); // regAddr = 0x2136[7:4]
				EQ_V_Wr01(eq_v_ctrl54, slowdyntrackfbf_2_0, slowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl54);

				baddyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl55); // regAddr = 0x2137[7:4]
				EQ_V_Wr01(eq_v_ctrl55,baddyntrackfbf_2_0,baddyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl55);

				badslowdyntrackfbf_2_0 = 0;
				EQ_V_RdFL(eq_v_ctrl56); // regAddr = 0x2138[2:0]
				EQ_V_Wr01(eq_v_ctrl56,badslowdyntrackfbf_2_0,badslowdyntrackfbf_2_0);
				EQ_V_WrFL(eq_v_ctrl56);


			}

		}


	}






#if 0
		thmainphnon_7_0 = 0x70;
		EQ_V_RdFL(eq_v_ctrl13); // regAddr = 0x210d[7:0]
		EQ_V_Wr01(eq_v_ctrl13,thmainphnon_7_0,thmainphnon_7_0);
		EQ_V_WrFL(eq_v_ctrl13);

		thmainphnoff_7_0 = 0x70;
		EQ_V_RdFL(eq_v_ctrl14); // regAddr = 0x210e[7:0]
		EQ_V_Wr01(eq_v_ctrl14,thmainphnoff_7_0,thmainphnoff_7_0);
		EQ_V_WrFL(eq_v_ctrl14);

#endif


	if(_gM14B0GBB_ChdiffTh == 0xf)
	{
		_gM14B0GBB_ChdiffTh = 0x12;
	}
	else if(_gM14B0GBB_ChdiffTh == 0x12)
	{
		_gM14B0GBB_ChdiffTh = 0x15;
	}
	else if(_gM14B0GBB_ChdiffTh == 0x15)
	{
		_gM14B0GBB_ChdiffTh = 0x18;
	}
	else
	{
		_gM14B0GBB_ChdiffTh = 0x18;
	}


#if 0
	if( (maxpwrmain<0x550) && (thpathcnt_3_0<=4) )
	{
		chslow_7_0 = 0x40;
		EQ_V_RdFL(eq_v_ctrl32); // regAddr = 0x2120[7:0]
		EQ_V_Wr01(eq_v_ctrl32,chslow_7_0,chslow_7_0);
		EQ_V_WrFL(eq_v_ctrl32);

		chslowghost_7_0 = 0x40;
		EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
		EQ_V_Wr01(eq_v_ctrl33,chslowghost_7_0,chslowghost_7_0);
		EQ_V_WrFL(eq_v_ctrl33);

		chfast_7_0 = 0x60;
		EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
		EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
		EQ_V_WrFL(eq_v_ctrl34);
	}
	else
	{
		chslow_7_0 = 0x18;
		EQ_V_RdFL(eq_v_ctrl32); // regAddr = 0x2120[7:0]
		EQ_V_Wr01(eq_v_ctrl32,chslow_7_0,chslow_7_0);
		EQ_V_WrFL(eq_v_ctrl32);

		chslowghost_7_0 = 0x38;
		EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
		EQ_V_Wr01(eq_v_ctrl33,chslowghost_7_0,chslowghost_7_0);
		EQ_V_WrFL(eq_v_ctrl33);

		chfast_7_0 = 0x40;
		EQ_V_RdFL(eq_v_ctrl34); // regAddr = 0x2122[7:0]
		EQ_V_Wr01(eq_v_ctrl34,chfast_7_0,chfast_7_0);
		EQ_V_WrFL(eq_v_ctrl34);
	}

#endif


#if 0
if(ncombdet==0)
{
	aiccopmode0_1_0 = 1;
	aiccopmode1_1_0 = 1;
	aiccopmode2_1_0 = 1;
	aiccopmode3_1_0 = 1;
	Common_RdFL(common_ctrl36); // regAddr = 0x0024[7:0]
	Common_Wr04(common_ctrl36, aiccopmode0_1_0, aiccopmode0_1_0, aiccopmode1_1_0, aiccopmode1_1_0, aiccopmode2_1_0, aiccopmode2_1_0, aiccopmode3_1_0, aiccopmode3_1_0);
	Common_WrFL(common_ctrl36);

	npncombw = 0x0;
	EQ_V_RdFL(eq_v_ctrl28); // regAddr = 0x211c[7]
	EQ_V_Wr01(eq_v_ctrl28,npncombw,npncombw);
	EQ_V_WrFL(eq_v_ctrl28);
	printk(" ###chungiii DEBUG ## PRE MONITOR  NTSC IS DETECTED notch filter on\n");
}
else
{
	aiccopmode0_1_0 = 0;
	aiccopmode1_1_0 = 0;
	aiccopmode2_1_0 = 0;
	aiccopmode3_1_0 = 0;
	Common_RdFL(common_ctrl36); // regAddr = 0x0024[7:0]
	Common_Wr04(common_ctrl36, aiccopmode0_1_0, aiccopmode0_1_0, aiccopmode1_1_0, aiccopmode1_1_0, aiccopmode2_1_0, aiccopmode2_1_0, aiccopmode3_1_0, aiccopmode3_1_0);
	Common_WrFL(common_ctrl36);

	npncombw = 0x1;
	EQ_V_RdFL(eq_v_ctrl28); // regAddr = 0x211c[7]
	EQ_V_Wr01(eq_v_ctrl28,npncombw,npncombw);
	EQ_V_WrFL(eq_v_ctrl28);

	printk(" ###chungiii DEBUG ## PRE MONITOR  NTSC IS DETECTED notch filter off\n");
}

#endif


	//OS_MsecSleep(200);

	return RET_OK;

#endif



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Monitor.
 * Monitoring
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_VSB_Monitor(DEMOD_CTX_T *pDemodInfo)
{

	DEMOD_TRACE("[DEMOD M14B0 DTV] @ %s =====\n",__F__);




#if 0

	UINT8	delaydiff_7_0;
	UINT8	snrref_5_0;
	UINT8	mainstrong;
	UINT8	phderotbwselout_3_0;
	UINT8	maxpwrmain_7_0;
	UINT16	maxpwrmain;
	UINT8	chmode_1_0;
	UINT8	eqstate_2_0;
	UINT8	burston;
	UINT8 	tperrcount25ms_7_0;
	UINT8	tperrcount25ms_15_8;
	UINT8	tperrcount25ms;
	UINT32	packeterror;
	UINT8	fbdlycir_9_8;
	UINT8	fbdlycir_7_0;
	UINT16	fbdly;
	UINT8	preghost;
	UINT8 	cirdrift_18_16, cirdrift_15_8, cirdrift_7_0;
	UINT32 	drift=0;
	UINT8	locktime_15_8, locktime_7_0;
	UINT16	locktime;
	UINT8	ncombdet;
	UINT8	chdiff_7_0;
	UINT8	fdeqstepout_3_0, fbfstepout_2_0;
	UINT8	fbfon;
	UINT8	agcdetflag;
	UINT8	daftmaxind_2_0, daftstatus_1_0;
	UINT8 	srdlock, qmdstatus_1_0, srdstatus_1_0;
	UINT8	crifolock_di, crfedstatus_di;
	UINT16	dlyspread;
	UINT8	dlyspread_11_8, dlyspread_7_0;
	UINT16	carrfreqoffset;
	UINT8	carrfreqoffset_15_8, carrfreqoffset_7_0;
	UINT32	samfreqoffset=0;
	UINT8	samfreqoffset_23_16, samfreqoffset_15_8, samfreqoffset_7_0;
	UINT8	samzgsel_v_2_0;
	UINT8	dagcbwoffset_3_0, dagcncombbw_2_0;
	UINT8	dagcnormbw_3_0, dagcdynbw_3_0;
	UINT8	dagc2ndbw_3_0;
	UINT16	vbber, vaber;
	UINT16	vbber_rate, vaber_rate;
	UINT8	vbber_15_8, vbber_7_0;
	UINT8	chberint;
	UINT8	bwoffset;
	UINT8	dagcbw_tot;

	UINT8	sammodesel_v_1_0, samcnormbp_v_1_0;
//	UINT8	samgearshen, samgsautoedl_v_1_0;
	UINT8	chslowghost_7_0;

	UINT8	statictrackfbf_2_0;
	UINT8	badstatictrackfbf_2_0;


	EQ_V_RdFL(eq_v_mon13); // regAddr = 0x218d[7:0]
	EQ_V_Rd01(eq_v_mon13,delaydiff_7_0, delaydiff_7_0);

	EQ_V_RdFL(eq_v_mon33);  //regAddr = 0x21a1[5:0]
	EQ_V_Rd01(eq_v_mon33, snrref_5_0, snrref_5_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## SNRREF is 0x%x\n", snrref_5_0);


	EQ_V_RdFL(eq_v_mon25);  //regAddr = 0x2199[6], 0x2199[3:0]
	EQ_V_Rd02(eq_v_mon25, mainstrong, mainstrong, phderotbwselout_3_0, phderotbwselout_3_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## Main CIR is strong? [%s][%d]\n",mainstrong ? "YES" : "NO", mainstrong);
	DEMOD_TRACE(" ###chungiii DEBUG ## Derotator BW is [0x%x]\n", phderotbwselout_3_0);


	EQ_V_RdFL(eq_v_mon5); // regAddr = 0x2185[7:0] :
	EQ_V_Rd01(eq_v_mon5,maxpwrmain_7_0, maxpwrmain_7_0);

	maxpwrmain = (maxpwrmain_7_0 << 0);
	DEMOD_TRACE(" ###chungiii DEBUG ## maxpwrmain[0x%x]\n", maxpwrmain);



	EQ_V_RdFL(eq_v_mon19);  //regAddr = 0x2193[1:0],
	EQ_V_Rd01(eq_v_mon19, chmode_1_0, chmode_1_0);
	DEMOD_TRACE(" ###chungiii DEBUG  chmode_1_0 ## Static/Dynamic?[0 or 1 : Static, 2 or 3 : Dynamic] : %d\n", chmode_1_0);

	EQ_V_RdFL(eq_v_mon24);  //regAddr = 0x2198[6:4],
	EQ_V_Rd01(eq_v_mon24, eqstate_2_0, eqstate_2_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## EQ state[0~3 : Acq mode, 4 or 5 : Static tracking, 6 or 7 : dynamic tracking] : %d\n", eqstate_2_0);


	EQ_V_RdFL(eq_v_mon36);  //regAddr = 0x21a4[6]
	EQ_V_Rd01(eq_v_mon36, burston, burston);
	DEMOD_TRACE(" ###chungiii Burst noise detected?[1 : Yes, 0 : No] : [%s][%d]\n",burston ? "YES" : "NO", burston);


	EQ_V_RdFL(eq_v_mon30);  //regAddr = 0x219e[7:0]
	EQ_V_Rd01(eq_v_mon30,tperrcount25ms_15_8,tperrcount25ms_15_8);

	EQ_V_RdFL(eq_v_mon31);  //regAddr = 0x219f[7:0]
	EQ_V_Rd01(eq_v_mon31,tperrcount25ms_7_0,tperrcount25ms_7_0);

	tperrcount25ms = (tperrcount25ms_15_8 << 8) + (tperrcount25ms_7_0 << 0);
	DEMOD_TRACE(" ###chungiii DEBUG ## tperrcount25ms[0x%x]\n", tperrcount25ms);



//	DEMOD_M14B0_Get_Packet_Error(&packeterror); //jeongpil.yun--> packet error  should be read at once within one second !! this line conflicts with other functions.
	packeterror = pDemodInfo->signalStatus.packetError; //jeongpil.yun
	DEMOD_TRACE(" ###chungiii DEBUG ## packeterror is [%d]\n", packeterror);


	EQ_V_RdFL(eq_v_mon0);  //regAddr = 0x2180[1]
	EQ_V_Rd01(eq_v_mon0, fbdlycir_9_8, fbdlycir_9_8);
	DEMOD_TRACE(" ###chungiii DEBUG MONITOR## Long Post Ghost[%s][%d]\n",(fbdlycir_9_8 >> 1) ? "NO" : "YES", (fbdlycir_9_8 >> 1));
	fbdly = ( fbdlycir_9_8 << 8);

	EQ_V_RdFL(eq_v_mon1);  //regAddr = 0x2181[7:0]
	EQ_V_Rd01(eq_v_mon1, fbdlycir_7_0, fbdlycir_7_0);
	fbdly += fbdlycir_7_0;
	DEMOD_TRACE(" ###chungiii DEBUG MONITOR## Ghost length is [%d]\n", fbdly);

	EQ_V_RdFL(eq_v_mon9);  //regAddr = 0x2189[7:0]
	EQ_V_Rd01(eq_v_mon9, preghost, preghost);
	DEMOD_TRACE(" ###chungiii DEBUG ## Preghost?[%s][%d]\n",preghost ? "YES" : "NO", preghost);


	EQ_V_RdFL(eq_v_mon10);  //regAddr = 0x218a[7:0],
	EQ_V_Rd01(eq_v_mon10, cirdrift_18_16, cirdrift_18_16);
	drift = (cirdrift_18_16 << 16);

	EQ_V_RdFL(eq_v_mon11);  //regAddr = 0x218b[7:0],
	EQ_V_Rd01(eq_v_mon11, cirdrift_15_8,cirdrift_15_8);
	drift += (cirdrift_15_8 << 8);

	EQ_V_RdFL(eq_v_mon12);  //regAddr = 0x218c[7:0],
	EQ_V_Rd01(eq_v_mon12, cirdrift_7_0, cirdrift_7_0);
	drift += (cirdrift_7_0 << 0);
	DEMOD_TRACE(" ###chungiii DEBUG ## THE AMOUNT OF CIR DRIFT IS 0x%x  \n",drift);


	EQ_V_RdFL(eq_v_mon15);  //regAddr = 0x218f[7:0],
	EQ_V_Rd01(eq_v_mon15, locktime_15_8, locktime_15_8);
	locktime = (locktime_15_8 << 8);

	EQ_V_RdFL(eq_v_mon16);  //regAddr = 0x2190[7:0],
	EQ_V_Rd01(eq_v_mon16, locktime_7_0, locktime_7_0);
	locktime += (locktime_7_0 << 0);
	DEMOD_TRACE(" ###chungiii DEBUG ## Lock Time is [0x%x]  \n",locktime);


	EQ_V_RdFL(eq_v_mon17);  //regAddr = 0x2191[7],
	EQ_V_Rd01(eq_v_mon17, ncombdet, ncombdet);
	//printk(" ###chungiii DEBUG ## NTSC IS DETECTED [%s][%d]\n",ncombdet ? "NO" : "YES", ncombdet);
	//printk(" ###chungiii _gM14B0GBB_DelayForStableTime [%d]\n",_gM14B0GBB_DelayForStableTime);

	EQ_V_RdFL(eq_v_mon20);  //regAddr = 0x2194[7:0],
	EQ_V_Rd01(eq_v_mon20, chdiff_7_0, chdiff_7_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## The amount of channel difference is [0x%x]\n", chdiff_7_0);

	EQ_V_RdFL(eq_v_mon26);  //regAddr = 0x219a[7:4], 0x219a[3:0]
	EQ_V_Rd02(eq_v_mon26, fdeqstepout_3_0, fdeqstepout_3_0, fbfstepout_2_0, fbfstepout_2_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## FDEQ step size is 0x%x\n", fdeqstepout_3_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## FBF step size  is 0x%x\n", fbfstepout_2_0);

	EQ_V_RdFL(eq_v_mon27);  //regAddr = 0x219b[6]
	EQ_V_Rd01(eq_v_mon27, fbfon, fbfon);
	DEMOD_TRACE(" ###chungiii FBF is ON?[1 : On, 0 : Off] : [%s][%d]\n",fbfon ? "ON" : "OFF", fbfon);

	EQ_V_RdFL(eq_v_mon46);  //regAddr = 0x21ae[5]
	EQ_V_Rd01(eq_v_mon46, agcdetflag, agcdetflag);
	DEMOD_TRACE(" ###chungiii AGC speed?[1 : Yes, 0 : No]  : [%s][%d]\n",agcdetflag ? "YES" : "NO", agcdetflag);


	// sync
	DEMOD_TRACE(" #######################chungiii ATSC  Synchronization Information\n");
	DEMOD_TRACE(" #################################################################\n\n\n");

	Sync_RdFL(sync_mon0);  //regAddr = 0x1080
	Sync_Rd02(sync_mon0, daftmaxind_2_0, daftmaxind_2_0, daftstatus_1_0, daftstatus_1_0);
	DEMOD_TRACE(" ###chungiii DAFT status [00: init, 01: processing, 10: lock, 11: fail] is %d\n", daftstatus_1_0);
	DEMOD_TRACE(" ###chungiii DAFT index is [daft hopping index] [%d]\n", daftmaxind_2_0);


	Sync_RdFL(sync_mon1);  //regAddr = 0x1081
	Sync_Rd03(sync_mon1, srdlock, srdlock, qmdstatus_1_0, qmdstatus_1_0, srdstatus_1_0, srdstatus_1_0);
	DEMOD_TRACE(" ###chungiii SRD LOCK[0x%x][0x%x][0x%x]\n", srdlock, qmdstatus_1_0, srdstatus_1_0);


	Sync_RdFL(sync_mon35);  //regAddr = 0x10a3
	Sync_Rd02(sync_mon35, crifolock_di, crifolock_di, crfedstatus_di, crfedstatus_di);
	DEMOD_TRACE(" ###chungiii CR LOCK[0x%x][0x%x]\n", crifolock_di, crfedstatus_di);


	Common_RdFL(common_mon0);  //regAddr = 0x10a3
	DEMOD_TRACE(" ###chungiii SYNC LOCK[0x%x]\n", Common_Rd(common_mon0));


	Common_RdFL(common_mon70);  //regAddr = 0x00c6[3:0],
	Common_Rd01(common_mon70, dlyspread_11_8, dlyspread_11_8);
	dlyspread = (dlyspread_11_8 << 8);

	Common_RdFL(common_mon71);  //regAddr = 0x00c7[7:0],
	Common_Rd01(common_mon71, dlyspread_7_0, dlyspread_7_0);
	dlyspread += (dlyspread_7_0 << 0);
	DEMOD_TRACE(" ###chungiii DEBUG ## Delay spread is [0x%x]  \n",dlyspread);



	Common_RdFL(common_mon28);  //regAddr = 0x009c[7:0],
	Common_Rd01(common_mon28, carrfreqoffset_15_8, carrfreqoffset_15_8);
	carrfreqoffset = (carrfreqoffset_15_8 << 8);

	Common_RdFL(common_mon29);  //regAddr = 0x009d[7:0],
	Common_Rd01(common_mon29, carrfreqoffset_7_0, carrfreqoffset_7_0);
	carrfreqoffset += (carrfreqoffset_7_0 << 0);
	carrfreqoffset = (carrfreqoffset / 16384) * 1000;
	DEMOD_TRACE(" ###chungiii DEBUG ## Carrier Frequency Offset is [%d kHz]\n",carrfreqoffset);


	Common_RdFL(common_mon33);  //regAddr = 0x00a1[7:0],
	Common_Rd01(common_mon33, samfreqoffset_23_16, samfreqoffset_23_16);
	samfreqoffset = (samfreqoffset_23_16 << 16);

	Common_RdFL(common_mon34);  //regAddr = 0x00a2[7:0],
	Common_Rd01(common_mon34, samfreqoffset_15_8, samfreqoffset_15_8);
	samfreqoffset = (samfreqoffset_15_8 << 8);


	Common_RdFL(common_mon35);  //regAddr = 0x00a3[7:0],
	Common_Rd01(common_mon35, samfreqoffset_7_0, samfreqoffset_7_0);
	samfreqoffset += (samfreqoffset_7_0 << 0);
	samfreqoffset = (samfreqoffset / 16777216) * 1000;
	DEMOD_TRACE(" ###chungiii DEBUG ## Sampling Frequency Offset is [%d kHz] \n",samfreqoffset);


	Sync_RdFL(sync_ctrl63);  //regAddr = 0x103f[6:4]
	Sync_Rd01(sync_ctrl63, samzgsel_v_2_0, samzgsel_v_2_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## ZTED gain [0 : low, 7 : high] is [%d]\n", samzgsel_v_2_0);



	EQ_V_RdFL(eq_v_ctrl84);  //regAddr = 0x2154[6:4]dagcncombbw_2_0,   0x2154[3:0]dagcbwoffset_3_0
	EQ_V_Rd02(eq_v_ctrl84, dagcbwoffset_3_0, dagcbwoffset_3_0, dagcncombbw_2_0, dagcncombbw_2_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## dagcbwoffset_3_0[%d]  dagcncombbw_2_0[%d] \n", dagcbwoffset_3_0, dagcncombbw_2_0);

	EQ_V_RdFL(eq_v_ctrl85);  //regAddr = 0x2155[7:4]dagcdynbw_3_0,   0x2155[3:0]dagcnormbw_3_0
	EQ_V_Rd02(eq_v_ctrl85, dagcnormbw_3_0, dagcnormbw_3_0, dagcdynbw_3_0, dagcdynbw_3_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## dagcnormbw_3_0[%d]  dagcdynbw_3_0[%d] \n", dagcnormbw_3_0, dagcdynbw_3_0);


	Common_RdFL(common_ctrl53);  //regAddr = 0x0035[3:0],
	Common_Rd01(common_ctrl53, dagc2ndbw_3_0, dagc2ndbw_3_0);
	DEMOD_TRACE(" ###chungiii DEBUG ## dagc2ndbw_3_0 is [%d] \n",dagc2ndbw_3_0);



	if(ncombdet==0)
	{
		bwoffset = (-1 * dagcncombbw_2_0);
	}
	else
	{
		if(agcdetflag==1)
		{
			bwoffset = dagcdynbw_3_0;
		}
		else
		{
			bwoffset = (-1 * dagcnormbw_3_0);
		}
	}


	dagcbw_tot = dagc2ndbw_3_0 + dagcbwoffset_3_0 + dagcbwoffset_3_0 + bwoffset;

	DEMOD_TRACE(" ###chungiii DEBUG ## dDAGC BW is 0x%x \n",dagcbw_tot);




	Common_RdFL(common_ctrl115);  //regAddr = 0x0073[4]
	Common_Rd01(common_ctrl115, chberint, chberint);
	DEMOD_TRACE(" ###chungiii DEBUG ## chberint is [%d] \n",chberint);



	Common_RdFL(common_mon118);  //regAddr = 0x00f6[7:0],
	Common_Rd01(common_mon118, vbber_15_8, vbber_15_8);
	vbber = (vbber_15_8 << 8);

	Common_RdFL(common_mon119);  //regAddr = 0x00f7[7:0],
	Common_Rd01(common_mon119, vbber_7_0, vbber_7_0);
	vbber += (vbber_7_0 << 0);

#if 0 //jeongpil.yun --> packet error  should be read at once within one second !! these lines  conflict with other functions.
	Common_RdFL(common_mon120);  //regAddr = 0x00f8[7:0],
	Common_Rd01(common_mon120, vaber_15_8, vaber_15_8);
	vaber = (vaber_15_8 << 8);

	Common_RdFL(common_mon121);  //regAddr = 0x00f9[7:0],
	Common_Rd01(common_mon121, vaber_7_0, vaber_7_0);
	vaber += (vaber_7_0 << 0);
#endif
		vaber = (UINT16) pDemodInfo->signalStatus.ber;	// jeongpil.yun

	if(chberint==1)
	{
		vbber_rate = vbber/1000000;
		vaber_rate = vaber/1000000;

	}
	else
	{
		vbber_rate = vbber/100000;
		vaber_rate = vaber/100000;
	}

	DEMOD_TRACE(" ###chungiii DEBUG ## Viterbi-Before BER is [%d]\n",vbber_rate);
	DEMOD_TRACE(" ###chungiii DEBUG ## Viterbi-After BER is [%d]\n",vaber_rate);
	DEMOD_TRACE(" ###chungiii DEBUG ## Viterbi-Before BER is [%d]\n",vbber_rate);


#endif




#if 0
UINT8	maxpwrmain_7_0;
UINT16	maxpwrmain;
UINT8	sammodesel_v_1_0, samcnormbp_v_1_0;
UINT8	chmode_1_0;
UINT8	preghost;
UINT8	statictrackfbf_2_0;
UINT8	badstatictrackfbf_2_0;
UINT8	chslowghost_7_0;




EQ_V_RdFL(eq_v_mon5); // regAddr = 0x2185[7:0] :
EQ_V_Rd01(eq_v_mon5,maxpwrmain_7_0, maxpwrmain_7_0);

maxpwrmain = (maxpwrmain_7_0 << 0);
DEMOD_TRACE(" ###chungiii DEBUG ## maxpwrmain[0x%x]\n", maxpwrmain);

EQ_V_RdFL(eq_v_mon19);	//regAddr = 0x2193[1:0],
EQ_V_Rd01(eq_v_mon19, chmode_1_0, chmode_1_0);

EQ_V_RdFL(eq_v_mon9);  //regAddr = 0x2189[7:0]
EQ_V_Rd01(eq_v_mon9, preghost, preghost);

if( (maxpwrmain>0x60) )
{
	samcnormbp_v_1_0 = 0x0;
	sammodesel_v_1_0 = 0x3;
	//sammodesel_v_1_0 = 0x2;
	Sync_RdFL(sync_ctrl61); // regAddr = 0x103d[5:4]
	Sync_Wr02(sync_ctrl61, samcnormbp_v_1_0, samcnormbp_v_1_0, sammodesel_v_1_0, sammodesel_v_1_0);
	Sync_WrFL(sync_ctrl61);
	DEMOD_TRACE(" ###chungiiiiii[0301] MAXPOWER MAIN OVER 0x60 [0x%x]\n",maxpwrmain);

}
else
{
	samcnormbp_v_1_0 = 0x2;
	sammodesel_v_1_0 = 0x1;
	Sync_RdFL(sync_ctrl61); // regAddr = 0x103d[5:4]
	Sync_Wr02(sync_ctrl61, samcnormbp_v_1_0, samcnormbp_v_1_0, sammodesel_v_1_0, sammodesel_v_1_0);
	Sync_WrFL(sync_ctrl61);
	DEMOD_TRACE(" ###chungiiiiii[0301] MAXPOWER MAIN NOT OVER 0x60 [0x%x]\n",maxpwrmain);
}


if((chmode_1_0==1) && (preghost==1))
{

	statictrackfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
	EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl50);


	badstatictrackfbf_2_0 = 0x0;
	EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
	EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl51);

}
else
{

	statictrackfbf_2_0 = 0x2;
	EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
	EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl50);


	badstatictrackfbf_2_0 = 0x2;
	EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
	EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
	EQ_V_WrFL(eq_v_ctrl51);

}


chslowghost_7_0 = 0x48;
EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
EQ_V_Wr01(eq_v_ctrl33, chslowghost_7_0, chslowghost_7_0);
EQ_V_WrFL(eq_v_ctrl33);

#endif

	UINT8	statictrackfbf_2_0;
	UINT8	badstatictrackfbf_2_0;
	UINT8	chslowghost_7_0;

	UINT8	chmode_1_0;
	UINT8	fbdlycir_9_8;
	UINT8	fbdlycir_7_0;
	UINT16	fbdly;


	EQ_V_RdFL(eq_v_mon0);  //regAddr = 0x2180[1]
	EQ_V_Rd01(eq_v_mon0, fbdlycir_9_8, fbdlycir_9_8);
	fbdly = ( fbdlycir_9_8 << 8);

	EQ_V_RdFL(eq_v_mon1);  //regAddr = 0x2181[7:0]
	EQ_V_Rd01(eq_v_mon1, fbdlycir_7_0, fbdlycir_7_0);
	fbdly += fbdlycir_7_0;

	EQ_V_RdFL(eq_v_mon19);	//regAddr = 0x2193[1:0],
	EQ_V_Rd01(eq_v_mon19, chmode_1_0, chmode_1_0);


	if((chmode_1_0!=0x0) && (fbdly>0x70) && (fbdly!=0x3ff))
	{

		statictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x2;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}
	else
	{

		statictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl50); // regAddr = 0x2132[2:0]
		EQ_V_Wr01(eq_v_ctrl50, statictrackfbf_2_0, statictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl50);


		badstatictrackfbf_2_0 = 0x0;
		EQ_V_RdFL(eq_v_ctrl51); // regAddr = 0x2133[2:0]
		EQ_V_Wr01(eq_v_ctrl51, badstatictrackfbf_2_0, badstatictrackfbf_2_0);
		EQ_V_WrFL(eq_v_ctrl51);

	}


	chslowghost_7_0 = 0x48;
	EQ_V_RdFL(eq_v_ctrl33); // regAddr = 0x2121[7:0]
	EQ_V_Wr01(eq_v_ctrl33, chslowghost_7_0, chslowghost_7_0);
	EQ_V_WrFL(eq_v_ctrl33);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Get_MSEdynStatus.
 * Get the flag whether MSE would be under operation limit (snr > 16dB : TRUE)
 *
 * @param  *pbEnable [OUT] BOOLEAN - get signal noise ratio
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_VSB_Get_MSEdynStatus(BOOLEAN *pbEnable)
{
	UINT8 	msedyn;
	UINT8	tpmainstrongphn;

	DEMOD_PRINT("[DEMOD M14B0 GBB] == @ %s!!!\n",__F__);

	EQ_V_RdFL(eq_v_mon32); // regAddr = 0x21a0[3] :EQ_V_MON32
	EQ_V_Rd01(eq_v_mon32,msedyn, msedyn);

	EQ_V_RdFL(eq_v_mon2); // regAddr = 0x2182[5] :
	EQ_V_Rd01(eq_v_mon2,tpmainstrongphn, tpmainstrongphn);

	if((msedyn == 0) || (tpmainstrongphn==0))
	{
		*pbEnable = FALSE;
	}
	else
	{
		*pbEnable = TRUE;
	}
	DEMOD_PRINT("[DEMOD H14 GBB] MSEdynStatus is %s!!\n",(*pbEnable == TRUE) ? "Enable" : "Disable");
	DEMOD_PRINT("[DEMOD H14 GBB] tpmainstrongphn is [%d]!!\n",tpmainstrongphn);

	return RET_OK;


}





/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Get_TotalInfo
 * Get all information on VSB for debug
 *
 * @param NONE [IN].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_VSB_Get_TotalInfo( void)
{

#if 0

	LX_DEMOD_LOCK_STATE_T	chScanStatus;
	LX_DEMOD_OPER_MODE_T operMode;
	UINT32 		ifAGC;
	LX_DEMOD_LOCK_STATE_T 	carrLock;
	LX_DEMOD_LOCK_STATE_T 	syncLock;
	SINT32 		offsetFrq;
	BOOLEAN 		cochannel;
	LX_DEMOD_MSE_T sigMse;

//	LX_DEMOD_VSB_AUTO_CTRL_SIG_STATUS 	bcMonSignal;

	UINT8 	cracquisionTimeMs;
	UINT32 	rsber;
	UINT32 	viterbiBer=0;
	UINT32 	bert;

	if (RET_OK  != DEMOD_M14B0_Get_NeverLockStatus(LX_DEMOD_ATSC, &chScanStatus))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_NeverLockStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_GetOperMode(&operMode))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_IFAGC(&ifAGC))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_IFAGC() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_Lock(LX_DEMOD_CARR_LOCK, &carrLock))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_Lock(&syncLock))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_CarrierFreqOffset( &offsetFrq))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_VSB_CochannelExist( &cochannel))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_VSB_CochannelExist() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

 	//MSE

	if (RET_OK != DEMOD_M14B0_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	if (RET_OK  != DEMOD_M14B0_GetCrAcquisionTime(&cracquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_GetTrAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_VABER(&viterbiBer))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_VABER() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	DEMOD_PRINT("[DEMOD M14B0 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	DEMOD_PRINT("[DEMOD M14B0 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													 (operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													 (operMode == LX_DEMOD_VSB_MODE) ? "VSB" :  "unknown" );

	DEMOD_PRINT("[DEMOD M14B0 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	DEMOD_PRINT("[DEMOD M14B0 DTV] Cochannel (NTSC) is  %s !!!\n", (cochannel == TRUE) ? " existent" : " Non-existent");
	DEMOD_PRINT("[DEMOD M14B0 DTV] Carrier LOCK = %s (%d ms)!!!\n",(carrLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", cracquisionTimeMs);
	DEMOD_PRINT("[DEMOD M14B0 DTV] Sync LOCK = %s !!!\n", (syncLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD M14B0 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);

	DEMOD_PRINT("[DEMOD M14B0 GBB] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);
	DEMOD_PRINT("[DEMOD M14B0 DTV] Viterbi BER = %d !!!\n",viterbiBer);

#ifndef  DEMOD_PRINT
	printk("[DEMOD M14B0 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	printk("[DEMOD M14B0 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													 (operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													 (operMode == LX_DEMOD_VSB_MODE) ? "VSB" :	"unknown" );

	printk("[DEMOD M14B0 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	printk("[DEMOD M14B0 DTV] Cochannel (NTSC) is  %s !!!\n", (cochannel == TRUE) ? " existent" : " Non-existent");
	printk("[DEMOD M14B0 DTV] Carrier LOCK = %s (%d ms)!!!\n",(carrLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", cracquisionTimeMs);
	printk("[DEMOD M14B0 DTV] Sync LOCK = %s !!!\n", (syncLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD M14B0 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);

	printk("[DEMOD M14B0 GBB] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD M14B0 DTV] Viterbi BER = %d !!!\n",viterbiBer);



#endif
	return RET_OK;

#endif

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_VSB_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime;

	//BOOLEAN 	staticGhost = FALSE;
	BOOLEAN	    Cochannel = FALSE ;


	switch (pDemodInfo->controlState)
	{
											/***************************************************/
											/***** Configure demod  for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_M14B0_VSB_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_VSB_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;

											/***************************************************/
											/***** Configure demod for tune start. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{

				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.vsb.tuneMode)
				{

					if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");
				}
				else
				{

					#if 0
					if (RET_OK != DEMOD_M14B0_TPOutEnable(FALSE))
					{
						DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(FALSE) !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					#else
					if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
					{
						DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(FALSE) !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					#endif


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					//pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;
					pDemodInfo->controlState = LX_DEMOD_SIGNAL_STABLE;
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;

											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;


				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD M14B0 GBB] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}


				if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
					pDemodInfo->signalStatus.bSignalValid = FALSE;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_RetuneStartTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_ATSC_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;




													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;


											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T			lockState;


				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
					if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_CARR_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{
						DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : catch only carrier lock..... ^0^\n");
						notifyinglockTimeOut = LX_DEMOD_LOCK_DEALY_2000 + LX_DEMOD_LOCK_DEALY_800;
							/* extending max delay */
					}
					else
					{

						/* nothing to do so far */
					}

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset=0;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.vsb.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;




						//if(elapsedTime < 160)
						//if(elapsedTime < LX_DEMOD_VSB_SIG_STABLE_WAIT_TIMEOUT)
						//if(elapsedTime < 40)
						if(0)
						{

							break;
						}
						else
						{

							#if 1
							if(RET_OK != DEMOD_M14B0_VSB_PreMonitor())
							{
								DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_VSB_PreMonitor() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							#endif

							//if(RET_OK != _DEMOD_M14B0_VSB_Get_NarrowPacket_Error(&packetError))
							if(RET_OK != DEMOD_M14B0_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{

								if(RET_OK != DEMOD_M14B0_VSB_CochannelExist(&Cochannel))
								{
									DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_VSB_CochannelExist() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}

								//if(packetError == 0)
								//if(packetError < LX_DEMOD_PACKET_ERROR_THD_100)
								if( ((packetError < LX_DEMOD_PACKET_ERROR_THD_32700) && (Cochannel == FALSE)) && (_gM14B0GBB_Tpblockon == FALSE) )
								{
									//for debug chungiii
									//OS_MsecSleep(200);

									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;


									if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}


									DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_ATSC_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);

									DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

									if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

								}
								else
								{

									#if 0
									if(Cochannel == TRUE)
									{
										printk( "^r^[DEMOD M14B0 GBB] (%s:%d) check() !!!\n", __F__, __L__);
										if(_gM14B0GBB_DelayForStableTime == LX_DEMOD_LOCK_DEALY_200)
										{
											printk("chungiiii ######### change to _gM14B0GBB_DelayForStableTime[400]\n");
											_gM14B0GBB_DelayForStableTime = LX_DEMOD_LOCK_DEALY_500;
											//break;
										}
									}

									#endif


									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_400)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;


										#if 0
										if(staticGhost == TRUE)
										{
											printk( "^r^[DEMOD M14B0 GBB] (%s:%d) check() !!!\n", __F__, __L__);
											if(_gM14B0GBB_DelayForStableTime == LX_DEMOD_LOCK_DEALY_200)
											{
												printk("chungiiii ######### change to _gM14B0GBB_DelayForStableTime[500]\n");
												_gM14B0GBB_DelayForStableTime = LX_DEMOD_LOCK_DEALY_400;
												break;
											}
										}
										#endif


										//printk( "^r^[DEMOD M14B0 GBB] (%s:%d) check() !!!\n", __F__, __L__);
										if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}

										DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : should be MSG_ATSC_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

										if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}

									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

						DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_ATSC_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

					}




				}
				else
				{
					//pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
					//DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_VSB_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_VSB_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;


	EQ_V_RdFL(eq_v_ctrl13); // regAddr = 0x210d[7:0]
	//printk(" ###chungiiii DEBUG REG 210d[0x%x]  \n",EQ_V_Rd(eq_v_ctrl13));


	if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
#if 1
	if(RET_OK != DEMOD_M14B0_VSB_Monitor(pDemodInfo))
	{
		DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_VSB_Monitor() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


#endif
	//printk( " ###chungiii debug monitoring func on \n");





	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= _gM14B0GBB_FullVQPkerrThreshold)
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr++;
			DEMOD_PRINT( "^r^[DEMOD M14B0 GBB](%s) _gM14B0GBB_VQI_UnlockCount_full_packeterr = %d !!!\n", __F__,_gM14B0GBB_VQI_UnlockCount_full_packeterr);

			if(_gM14B0GBB_VQI_UnlockCount_full_packeterr == _gM14B0GBB_FullVQIPkerrCount)
			{
				DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] oops !! (%s) Force to NOT LOCK !!\n", __F__);

				_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;


				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD M14B0 GBB](%s) DEMOD_SoftwareReset() !!!\n", __F__);

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

			}

		}
		else
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;


			if(pDemodInfo->bIsTPoutEn == TRUE)
			{
				if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
				{
					DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
			}


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}

			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus = LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock = TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
					pDemodInfo->signalStatus.bSignalValid = TRUE;

					DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] MONITORING FUNC : MSG_ATSC_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

			if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
			{
				DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}

	}
	else
	{

		pDemodInfo->lockCount = 0;

		_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedLock = FALSE;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] MONITORING FUNC : MSG_ATSC_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] MONITORING FUNC  : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pDemodInfo->bNotifiedLock 				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= TRUE;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1100;


		}



	}


	return RET_OK;



}

#if 0
#endif


#if 0


/**
 * DEMOD_M14B0_Get_VSB_NarrowPacket_Error
 * Get packet error on DVB
 *
 * @param *pSnr 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
static int _DEMOD_M14B0_VSB_Get_NarrowPacket_Error(UINT32 *pError)
{

	UINT8 	tperrcount25ms_7_0;
	UINT8	tperrcount25ms_15_8;


	*pError = 0xffffffff;



	EQ_V_RdFL(eq_v_mon30);  //regAddr = 0x219e[7:0]
	EQ_V_Rd01(eq_v_mon30,tperrcount25ms_15_8,tperrcount25ms_15_8);

	EQ_V_RdFL(eq_v_mon31);  //regAddr = 0x219f[7:0]
	EQ_V_Rd01(eq_v_mon31,tperrcount25ms_7_0,tperrcount25ms_7_0);



	*pError = (UINT32)tperrcount25ms_7_0 ;
	DEMOD_TRACE(" 24.2ms tperrcnt = %d, *pError = %d  \n",tperrcount25ms_7_0,*pError);


	return RET_OK;

}
#endif


#if 0
#endif

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_SoftwareResetFEC.
 * Reset QAM FEC module.
 *
 * @param [in] NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_SoftwareResetFEC(void)
{
	UINT8 	softrstfecb;

	softrstfecb = 0;   //softrstfecb : '0'-> FEC is reset
	Common_RdFL(common_ctrl0);  //regAddr = 0x0, Common control register
	Common_Wr01(common_ctrl0, softrstfecb, softrstfecb); //softrstfecb : '0'-> FEC is reset, '1' -> FEC is not reset.
	Common_WrFL(common_ctrl0);

	OS_MsecSleep(10);

	softrstfecb = 1;   //softrstfecb : '1' -> FEC is not reset
	Common_Wr01(common_ctrl0, softrstfecb, softrstfecb); //softrstfecb : '0'-> FEC is reset, '1' -> FEC is not reset.
	Common_WrFL(common_ctrl0);

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Monitor.
 * Monitoring
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_QAM_Monitor(void)
{

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_ModeAutoDetection.
 * Enable/Disable the detection of QAM mode
 *
 * @param  bEnable [IN] - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_ModeAutoDetection(BOOLEAN bEnable)
{


	UINT8	stdopdettmode_2_0;
	UINT8	stdopdetcmode_1_0;


	stdopdettmode_2_0 = 0x0;
	stdopdetcmode_1_0 = 0x2;

	stdopdetcmode_1_0 = (bEnable == TRUE)?(0x2):(0x1);
	Common_RdFL(common_ctrl9);   //regAddr = 0x0009[4:2] : stdopdettmode_2_0, 0x0009[1:0] : stdopdetcmode_1_0
	Common_Wr02(common_ctrl9, stdopdettmode_2_0, stdopdettmode_2_0,stdopdetcmode_1_0,stdopdetcmode_1_0);
	Common_WrFL(common_ctrl9);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_64Mode.
 * Set the QAM mode to QAM64 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_64Mode(void)
{


	UINT8	qmdqammode0_2_0;


	/* mode 64 QAM */

	// manually set to 64QAM

	qmdqammode0_2_0 = 0x0;      //QMDQAMMODE0[2] 0 means 64QAM, 1 means 256QAM
	Sync_Wr01(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0);     //regAddr = 0x101a  QMD control register
	Sync_WrFL(sync_ctrl26);



	return RET_OK;


}
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_256Mode.
 * Set the QAM mode to QAM256 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_256Mode(void)
{

	UINT8	qmdqammode0_2_0;


	/* mode 256QAM */

	// manually set to 256QAM

	qmdqammode0_2_0 = 0x4;      //QMDQAMMODE0[2] 0 means 64QAM, 1 means 256QAM
	Sync_Wr01(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0);      //regAddr = 0x101a  QMD control register
	Sync_WrFL(sync_ctrl26);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Set_DefaultRegisterValue.
 * Initialize LG1150 GBB for QAM mode.
 *
 * @param  NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_Set_DefaultRegisterValue(void)
{

	UINT8 srdchkalways;

	UINT8 adcvrefsel_0;
	UINT8 adcclkpllsel;

	UINT8	agcgearshen;
	UINT8	agcifbw_3_0;
	UINT8	samgsend_cqs_3_0, samgsstart_cqs_3_0;


	DEMOD_PRINT("[DEMOD M14B0 DTV] ####chungiii check !!! %s =====\n",__F__);


	adcvrefsel_0 = 1;
	adcclkpllsel = 1;
	Common_RdFL(common_ctrl4);   //regAddr = 0x0004[7], COMMON_CTRL[4]
	Common_Wr02(common_ctrl4, adcvrefsel_0, adcvrefsel_0, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	//ADCCLKPLLSEL : Selection of ADC sampling clock source.  This is not validated at DTV SoC
	// 0 : use external oscillator input
	// 1 : use PLL output

	srdchkalways = 0x1;
	Common_RdFL(common_ctrl10);	 //regAddr = 0x000a , COMMON_CTRL[10]
	Common_Wr01(common_ctrl10, srdchkalways, srdchkalways);
	Common_WrFL(common_ctrl10);


	agcgearshen = 0x1;
	Common_RdFL(common_ctrl18); // regAddr = 0x0012[4]
	Common_Wr01(common_ctrl18, agcgearshen, agcgearshen);
	Common_WrFL(common_ctrl18);

	agcifbw_3_0 = 0x8;
	Common_RdFL(common_ctrl24); // regAddr = 0x0018[3:0]
	Common_Wr01(common_ctrl24, agcifbw_3_0, agcifbw_3_0);
	Common_WrFL(common_ctrl24);

	samgsend_cqs_3_0 = 0xc;
	samgsstart_cqs_3_0 = 0x4;
	Sync_RdFL(sync_ctrl54); // regAddr = 0x1036[7:4]
	Sync_Wr02(sync_ctrl54, samgsend_cqs_3_0, samgsend_cqs_3_0, samgsstart_cqs_3_0, samgsstart_cqs_3_0);
	Sync_WrFL(sync_ctrl54);




	if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 scsyslockchktime_7_0;

	DEMOD_PRINT("[DEMOD M14B0 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = (UINT8)(waitMs / 10);
	Common_Wr01(common_ctrl11, scsyslockchktime_7_0, scsyslockchktime_7_0);  //regAddr = 0x000b, COMMON_CTRL[11]
	Common_WrFL(common_ctrl11);
	// SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in msec resolution


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Get_TotalInfo
 * Get all information on QAM for debug
 *
 * @param NONE [IN / OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_Get_TotalInfo( void)
{
#if 0

	LX_DEMOD_NEVERLOCK_STATUS_T 		chScanStatus;
	LX_DEMOD_RF_MODE_T 				operMode;
	UINT32 		ifAGC;
	LX_DEMOD_LOCK_STATE_T 	timingLock;
	LX_DEMOD_LOCK_STATE_T 	fecLock;
	SINT32 		offsetFrq;
	UINT32 		snr;
	BOOLEAN 	enableInv;
	UINT8 		tracquisionTimeMs;
	UINT8 		fecacquisionTimeMs;

	UINT32 		viterbiBer=0;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_MSE_T sigMse;

	if (RET_OK  != DEMOD_M14B0_Get_NeverLockStatus(LX_DEMOD_QAM, &chScanStatus))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_NeverLockStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_GetOperMode(&operMode))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_IFAGC(&ifAGC))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_IFAGC() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_Lock(LX_DEMOD_TR_LOCK, &timingLock))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) LX_DEMOD_LOCK_STATE_T() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLock))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_CarrierFreqOffset(&offsetFrq))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

 	//MSE

	if (RET_OK != DEMOD_M14B0_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_M14B0_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_SpectrumStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  !=  DEMOD_M14B0_GetTrAcquisionTime(&tracquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_GetTrAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	if (RET_OK  !=  DEMOD_M14B0_GetFecAcquisionTime(&fecacquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_GetFecAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  !=  DEMOD_M14B0_Get_VABER(&viterbiBer))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 DTV] FAIL (%s:%d) DEMOD_M14B0_Get_VABER() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	DEMOD_PRINT("[DEMOD M14B0 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	DEMOD_PRINT("[DEMOD M14B0 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													(operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													(operMode == LX_DEMOD_VSB_MODE) ? "VSB" :  "unknown" );

	DEMOD_PRINT("[DEMOD M14B0 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	DEMOD_PRINT("[DEMOD M14B0 DTV] Timing LOCK = %s (%d ms)!!!\n",(timingLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", tracquisionTimeMs);
	DEMOD_PRINT("[DEMOD M14B0 DTV] FEC LOCK = %s !!!\n",(fecLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD M14B0 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);
	DEMOD_PRINT("[DEMOD M14B0 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD M14B0 GBB] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	DEMOD_PRINT("[DEMOD M14B0 DTV] Viterbi BER = %d !!!\n",viterbiBer);

	return RET_OK;

#endif

	return RET_OK;

}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_QAM_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	static unsigned int 			manualOperationTime = 0;
	static LX_DEMOD_TUNE_MODE_T		realTuneMode = LX_DEMOD_TUNE_UNKNOWN;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_M14B0_QAM_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.qam.tuneMode)
				{
					if(RET_OK != DEMOD_M14B0_QAM_ModeAutoDetection(TRUE))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{
					if(RET_OK != DEMOD_M14B0_QAM_ModeAutoDetection(FALSE))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_M14B0_Set_SpectrumCtrl(FALSE, pDemodInfo->setCfgParam.qam.bSpectrumInv))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Set_SpectrumCtrl() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					if(LX_DEMOD_64QAM == pDemodInfo->setCfgParam.qam.qamMode)
					{
						if(RET_OK != DEMOD_M14B0_QAM_64Mode())
						{
							DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_64Mode() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else if(LX_DEMOD_256QAM == pDemodInfo->setCfgParam.qam.qamMode)
					{
						if(RET_OK != DEMOD_M14B0_QAM_256Mode())
						{
							DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_256Mode() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else
					{
							DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) Invalid QAM Mode !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
					}

				}


				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{
//				unsigned int 			elapsedTime;

				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_M14B0_QAM_EQ_Signal_Detector(TRUE))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_EQ_Signal_Detector() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.qam.tuneMode)
				{
					realTuneMode = LX_DEMOD_TUNE_AUTO;
//					elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

//					if(elapsedTime >= LX_DEMOD_LOCK_DEALY_170)
//					{
					if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "[DEMOD M14B0 GBB] LX_DEMOD_TUNE_START : (auto) Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");

//					}

				}
				else
				{
					realTuneMode = LX_DEMOD_TUNE_NORMAL;
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;
					manualOperationTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "[DEMOD M14B0 GBB] LX_DEMOD_TUNE_START :(manual) Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD M14B0 GBB] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
					pDemodInfo->signalStatus.bSignalValid = FALSE;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_RetuneStartTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_QAM_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD M14B0 GBB] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;




													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;
											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;
				unsigned int 			elapsedTime;

				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if(RET_OK != DEMOD_M14B0_QAM_EQ_Signal_Detector(FALSE))
					{
						DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_EQ_Signal_Detector() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
					if(LX_DEMOD_TUNE_NORMAL == realTuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - manualOperationTime;


						if(elapsedTime > LX_DEMOD_LOCK_DEALY_300)
						{
							if(RET_OK != DEMOD_M14B0_QAM_ModeAutoDetection(TRUE))
							{
								DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}

							if(RET_OK != DEMOD_M14B0_SoftwareReset())
							{
								DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_SoftwareReset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}

							realTuneMode = LX_DEMOD_TUNE_AUTO;
						}

					}


				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.qam.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_QAM_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_M14B0_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
								if(packetError < LX_DEMOD_PACKET_ERROR_THD)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_QAM_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);

									if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

								}
								else
								{
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_QAM_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

										if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}


									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

						DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_QAM_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

					}



				}
				else
				{
					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
					DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_QAM_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_QAM_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;


		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;


		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= _gM14B0GBB_FullVQPkerrThreshold)
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr++;
			DEMOD_PRINT( "^r^[DEMOD M14B0 GBB](%s) _gM14B0GBB_VQI_UnlockCount_full_packeterr = %d !!!\n", __F__,_gM14B0GBB_VQI_UnlockCount_full_packeterr);

			if(_gM14B0GBB_VQI_UnlockCount_full_packeterr == _gM14B0GBB_FullVQIPkerrCount)
			{
				DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] oops !! (%s) Force to NOT LOCK !!\n", __F__);

				_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;


				if(RET_OK != DEMOD_M14B0_QAM_ModeAutoDetection(TRUE))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD M14B0 GBB](%s) DEMOD_M14B0_QAM_ModeAutoDetection(TRUE) !!!\n", __F__);

				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD M14B0 GBB](%s) DEMOD_SoftwareReset() !!!\n", __F__);


				if(RET_OK != DEMOD_M14B0_QAM_EQ_Signal_Detector(TRUE))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_EQ_Signal_Detector() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

			}

		}
		else
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 2)
			{

			if(RET_OK != DEMOD_M14B0_QAM_EQ_Signal_Detector(FALSE))
			{
				DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_EQ_Signal_Detector() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}


				pDemodInfo->lockStatus 		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock 	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
					pDemodInfo->signalStatus.bSignalValid = TRUE;

					DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] MONITORING FUNC : MSG_QAM_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

			if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
			{
				DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}

	}
	else
	{

		pDemodInfo->lockCount = 0;
		_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;


		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				if(RET_OK != DEMOD_M14B0_QAM_EQ_Signal_Detector(TRUE))
				{
					DEMOD_PRINT("[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_QAM_EQ_Signal_Detector() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				DEMOD_PRINT_ALWAYS( "[DEMOD M14B0 GBB] MONITORING FUNC : MSG_QAM_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD M14B0 GBB] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= TRUE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1500;


		}



	}


	return RET_OK;



}



/**
 * DEMOD_M14B0_QAM_EQ_Signal_Detector.
 * Disable no signal detector function
 *
 * @param [in] NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_QAM_EQ_Signal_Detector(BOOLEAN OnOff)
{
	UINT8	nousenosigdet;


	if(OnOff)
	{
		nousenosigdet 	= 1;
	}
	else
	{
		nousenosigdet 	= 0;
	}


	EQ_CQS_RdFL(eq_cqs_ctl73);	 //regAddr = 0x2849
	EQ_CQS_Wr01(eq_cqs_ctl73, nousenosigdet, nousenosigdet);
	EQ_CQS_WrFL(eq_cqs_ctl73);

	/*
	Disable no signal detector function
	0 : enable no signal detector function
	1 : disable no signal detector function
	*/

	return RET_OK;

}







#if 0

/**
 * DEMOD_M14B0_ISDBT_SetDefaultRegisterValue
 * Set defualt register value on DVB-T.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/


int DEMOD_M14B0_ISDBT_Set_DefaultRegisterValue(void)
{

	UINT8 adcvrefsel_0;
	UINT8 adcclkpllsel;

	UINT8 fstoisiintv0_i_3_0;
	UINT8 fstoisithr0_i_3_0;

	UINT8 cirnrctrl_d_2_0;
	UINT8 cirnrctrl_i_2_0;
	UINT8 fstoupmode;
	UINT8 fstoisicnf_3_0;
	UINT8 fstoisithr2_3_0;

	UINT8 crgearshen;

	UINT8 crifoth_i_7_0;

	UINT8 crifolockfcc_di_7_0;

	UINT8 aiccopmode0_1_0;
	UINT8 aiccopmode1_1_0;

	UINT8 aiccfixfreq0_23_16;
	UINT8 aiccfixfreq0_15_8;
	UINT8 aiccfixfreq0_7_0;

	UINT8 aiccfixfreq1_23_16;
	UINT8 aiccfixfreq1_15_8;
	UINT8 aiccfixfreq1_7_0;

	unsigned int	powerOnElapsedTime;



	DEMOD_PRINT("[DEMOD M14B0 DTV] @ %s =====\n",__F__);

	adcvrefsel_0 = 1;
	adcclkpllsel = 0;

	Common_RdFL(common_ctrl4);   //regAddr = 0x0004[7], COMMON_CTRL[4]
	Common_Wr02(common_ctrl4, adcvrefsel_0, adcvrefsel_0, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	//ADCCLKPLLSEL : Selection of ADC sampling clock source.  This is not validated at DTV SoC
	// 0 : use external oscillator input
	// 1 : use PLL output

	fstoisiintv0_i_3_0 = 0x9;
	EQ_DI_RdFL(eq_di_ctrl29); //regAddr = 0x221d
	EQ_DI_Wr01(eq_di_ctrl29,fstoisiintv0_i_3_0, fstoisiintv0_i_3_0);
	EQ_DI_WrFL(eq_di_ctrl29);
	// FSTOISIINTV0_I[3:0] : Observation duration of stage 0 for ISDB-T ISI FSTO for 2k Duration = (64 + 32 x FSTOISIINTV0_I) x (CIRLOOPDLY + 1) symbols
	// 4k duration = 2k duration / 2, 8k duration = 2k duration / 4
	//  Change to "1001" by S/W

	fstoisithr0_i_3_0 = 0x7;
	EQ_DI_RdFL(eq_di_ctrl31); //regAddr = 0x221f
	EQ_DI_Wr01(eq_di_ctrl31,fstoisithr0_i_3_0, fstoisithr0_i_3_0);
	EQ_DI_WrFL(eq_di_ctrl31);
	// FSTOISITHR0_I[3:0] : Detection threshold of stage 0 for ISDB-T ISI FSTO
	// 0000 : 0
	// 0001 ~ 0100 : 1024 x 2^(FSTOISITHR0_D - 1)
	// 0101 ~ 1111 : 12288 + (4096 x (FSTOISITHR0_D - 5))
	// Change to "0111" by S/W


	cirnrctrl_d_2_0 = 0x2;
	cirnrctrl_i_2_0 = 0x2;
	EQ_DI_RdFL(eq_di_ctrl37); //regAddr = 0x2225
	EQ_DI_Wr02(eq_di_ctrl37,cirnrctrl_d_2_0, cirnrctrl_d_2_0, cirnrctrl_i_2_0, cirnrctrl_i_2_0 );
	EQ_DI_WrFL(eq_di_ctrl37);
	// CIRNRCTRL_D[2:0] : CIR noise reduction power level control for DVB-T (1x = 6bits round)
	// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x,  Change to "010" by S/W

	//CIRNRCTRL_I[2:0] : CIR noise reduction power level control for ISDB-T (1x = 6bits round)
	// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x


	fstoupmode = 0x0;
	EQ_DI_RdFL(eq_di_ctrl26); //regAddr = 0x221a
	EQ_DI_Wr01(eq_di_ctrl26,fstoupmode, fstoupmode);
	EQ_DI_WrFL(eq_di_ctrl26);
	//FSTOUPMODE : FSTO Post(Right) shift period 7bit shift enable (0 : 1x, 1 : 128x)

	fstoisicnf_3_0 = 0x5;
	EQ_DI_RdFL(eq_di_ctrl30); //regAddr = 0x221e
	EQ_DI_Wr01(eq_di_ctrl30,fstoisicnf_3_0, fstoisicnf_3_0);
	EQ_DI_WrFL(eq_di_ctrl30);
	//FSTOISICNF[3:0] : Confidence counter of stage 2 for ISI FSTO
	// 0000 ~ 0111 : (64 + 64 x FSTOISICNF) x (CIRLOOPDLY + 1) symbols
	// 1000 ~ 1111 : (640 x (FSTOISICNF - 7)) x (CIRLOOPDLY + 1) symbols
	// 4k counter = 2k counter / 2, 8k counter = 2k counter / 4
	// Change to "0101" by S/W

	fstoisithr2_3_0 = 0x2;
	EQ_DI_RdFL(eq_di_ctrl32); //regAddr = 0x2220
	EQ_DI_Wr01(eq_di_ctrl32,fstoisithr2_3_0, fstoisithr2_3_0);
	EQ_DI_WrFL(eq_di_ctrl32);
	// FSTOISITHR2[3:0] : Detection threshold of stage 2 for ISI FSTO
	// Threshold = 32768 x FSTOISITHR2
	// Change to "0010" by S/W

	crifoth_i_7_0 = 0x2;
	Sync_RdFL(sync_ctrl88);  //regAddr = 0x1058, SYNC_CTRL[88]
	Sync_Wr01(sync_ctrl88,crifoth_i_7_0,crifoth_i_7_0);
	Sync_WrFL(sync_ctrl88);
	//CRIFOTH_I[7:0]: IFO decision threshold set

	crifolockfcc_di_7_0 = 0x8;
	Sync_RdFL(sync_ctrl89);  //regAddr = 0x1059, SYNC_CTRL[89]
	Sync_Wr01(sync_ctrl89,crifolockfcc_di_7_0,crifolockfcc_di_7_0);
	Sync_WrFL(sync_ctrl89);
	//CRIFOLOCKFCC_DI[7:0]: IFO false lock confidence count reference value set.

	aiccopmode0_1_0 = 1;
	aiccopmode1_1_0 = 0;
	Common_RdFL(common_ctrl36);   //regAddr = 0x0024, COMMON_CTRL[36]
	Common_Wr02(common_ctrl36, aiccopmode0_1_0, aiccopmode0_1_0, aiccopmode1_1_0, aiccopmode1_1_0);
	Common_WrFL(common_ctrl36);
	//AICCOPMODE0[1:0]: Selection of 1st AICC(continuous wave interference or analog TV signal) rejection function
	// AICCOPMODE1[1:0] : Selection of 2nd AICC(continuous wave interference or analog TV signal) rejection function
	// 00 : always turn off
	// 01 : always turn for of pre-defined frequency which is set by AICCFIXFREQ0(@0x2E~0x30)
	// 10 : conditionally turn on or turn off for pre-defined frequency which is set by AICCFIXFREQ0(@0x2E~0x30)
	// 11 : scan mode which means turn on at the detected frequency in any frequency

	aiccfixfreq0_23_16 = 0x18;
	Common_RdFL(common_ctrl46);   //regAddr = 0x002e, COMMON_CTRL[46]
	Common_Wr01(common_ctrl46, aiccfixfreq0_23_16, aiccfixfreq0_23_16);
	Common_WrFL(common_ctrl46);

	aiccfixfreq0_15_8 = 0xdb;
	Common_RdFL(common_ctrl47);   //regAddr = 0x002f, COMMON_CTRL[47]
	Common_Wr01(common_ctrl47, aiccfixfreq0_15_8, aiccfixfreq0_15_8);
	Common_WrFL(common_ctrl47);

	aiccfixfreq0_7_0 = 0x23;
	Common_RdFL(common_ctrl48);   //regAddr = 0x0030, COMMON_CTRL[48]
	Common_Wr01(common_ctrl48, aiccfixfreq0_7_0, aiccfixfreq0_7_0);
	Common_WrFL(common_ctrl48);
	//AICCFIXFREQ0[23:16]
	//AICCFIXFREQ0[15:8]
	// AICCFIXFREQ0[7:0]
	// Set of fixed frequency for 1st AICC rejectot
	// rejection frequency = AICCFIXEDFREQ0 / 2^19

	aiccfixfreq1_23_16 = 0x3c;
	Common_RdFL(common_ctrl43);   //regAddr = 0x002b, COMMON_CTRL[43]
	Common_Wr01(common_ctrl43, aiccfixfreq1_23_16, aiccfixfreq1_23_16);
	Common_WrFL(common_ctrl43);

	aiccfixfreq1_15_8 = 0xdb;
	Common_RdFL(common_ctrl44);   //regAddr = 0x002c, COMMON_CTRL[47]
	Common_Wr01(common_ctrl44, aiccfixfreq1_15_8, aiccfixfreq1_15_8);
	Common_WrFL(common_ctrl44);

	aiccfixfreq1_7_0 = 0x23;
	Common_RdFL(common_ctrl45);   //regAddr = 0x002d, COMMON_CTRL[48]
	Common_Wr01(common_ctrl45, aiccfixfreq1_7_0, aiccfixfreq1_7_0);
	Common_WrFL(common_ctrl45);
	//AICCFIXFREQ1[23:16]
	//AICCFIXFREQ1[15:8]
	// AICCFIXFREQ1[7:0]
	// Set of fixed frequency for 2nd AICC rejectot
	// rejection frequency = AICCFIXEDFREQ1 / 2^19

	if (RET_OK != DEMOD_M14B0_TPOutEnable(TRUE))
	{
		DEMOD_PRINT( "^r^[DEMOD M14B0 GBB] FAIL (%s:%d) DEMOD_M14B0_TPOutEnable(TRUE) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


	if(gM14B0GBB_PowerOnInit == 1)
	{
		gM14B0GBB_PowerOnInit = 2;

		crgearshen = 0x0;
		Common_RdFL(common_ctrl68);	 //regAddr = 0x0044, COMMON_CTRL[68]
		Common_Wr01(common_ctrl68, crgearshen, crgearshen);
		Common_WrFL(common_ctrl68);
		// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
		// 0 : disable
		// 1 : enable

		gM14B0GBB_PowerOnInitStartTimeISDBT = jiffies_to_msecs(jiffies);

	}
	else if(gM14B0GBB_PowerOnInit == 0)
	{
		crgearshen = 0x1;
		Common_RdFL(common_ctrl68);  //regAddr = 0x0044, COMMON_CTRL[68]
		Common_Wr01(common_ctrl68, crgearshen, crgearshen);
		Common_WrFL(common_ctrl68);
		// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
		// 0 : disable
		// 1 : enable
	}
	else
	{
		powerOnElapsedTime = jiffies_to_msecs(jiffies) - gM14B0GBB_PowerOnInitStartTimeDVB;

		DEMOD_PRINT("[DEMOD M14 GBB](%s:%d) powerOnElapsedTime = %d !!!\n", __F__, __L__,powerOnElapsedTime);

		if(powerOnElapsedTime >= 15000)
		{
			gM14B0GBB_PowerOnInit = 0;

			if (RET_OK != DEMOD_M14B0_ISDBT_Set_CrGearshEn(TRUE ))
			{
				DEMOD_PRINT("[DEMOD M14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_CrGearshEn() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}

	}

	_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_Config_auto (BOOLEAN bAutoDetect)

{

	UINT8	txpauto_di;
	UINT8	mdauto_di;
	UINT8	specinvauto;
	UINT8	specinv;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	specinvauto = 0; //SPECINVAUTO OFF
	specinv = 0; //non-inversion

	Common_RdFL(common_ctrl2);  //regAddr = 0x0002, COMMON_CTRL[2]
	Common_Wr02(common_ctrl2, specinvauto, specinvauto,specinv,specinv); //SPECINVAUTO : 0->manual, 1-> auto
	Common_WrFL(common_ctrl2);											//SPECINV : 0->normal, 1 -> Inversion

	mdauto_di = (bAutoDetect == TRUE)?(1):(0);	//MDAUTO_DI
	Common_RdFL(common_ctrl83);	 //regAddr = 0x0053[5]
	Common_Wr01(common_ctrl83, mdauto_di, mdauto_di); //MDAUTO_DI : FFT/Guard Interval mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(common_ctrl83);

	txpauto_di = (bAutoDetect == TRUE)?(1):(0);  //QAMMODEAUTO
	Common_RdFL(common_ctrl87);	 //regAddr = 0x0057[7]
	Common_Wr01(common_ctrl87, txpauto_di, txpauto_di); //TXPAUTO_DI : Transmission mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(common_ctrl87);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when DEMOD_M14B0_ISDBT_Set_Config_auto is enabled.
 *
 * @param *pIsdbtConfigParam 	[IN] LX_DEMOD_ISDBT_CONFIG_T - ISDBT config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_PartialConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam)
{


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when DEMOD_M14B0_ISDBT_Set_Config_auto is disabled.
 *
 * @param *pIsdbtConfigParam 	[IN] LX_DEMOD_ISDBT_CONFIG_T - ISDB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_FullConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam)
{

	UINT8 	mdfftmode_di_1_0;
	UINT8 	mdgimode_di_1_0;

	UINT8 	tmcccoderatea_i_2_0;
	UINT8	tmccqammodea_i_2_0;

	UINT8 	tmcccoderateb_i_2_0;
	UINT8	tmccqammodeb_i_2_0;

	UINT8 	tmcccoderatec_i_2_0;
	UINT8	tmccqammodec_i_2_0;


	UINT8 tmccnumsega_i_3_0;
	UINT8 tmccnumsegb_i_3_0;
	UINT8 tmccnumsegc_i_3_0;

	UINT8 tmcctimeintlena_i_2_0;
	UINT8 tmcctimeintlenb_i_2_0;
	UINT8 tmcctimeintlenc_i_2_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);




	//FFT mode manual set
	switch(pIsdbtConfigParam->FFTMode)
	{
		case LX_DEMOD_DVBT_ISDBT_FFT_2K :
			{
				mdfftmode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set FFT Mode = %s !!!\n","2K ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_FFT_8K :
			{
				mdfftmode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set FFT Mode = %s !!!\n","8K");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_FFT_4K :
			{
				mdfftmode_di_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set FFT Mode = %s !!!\n","4K ");
			}
			break;

		default :
			{
				mdfftmode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d)UNKNOWN FFT mode !!!\n",__F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(common_ctrl83);   //regAddr = 0x0053[3:2]
	Common_Wr01(common_ctrl83, mdfftmode_di_1_0, mdfftmode_di_1_0); //MDFFTMODE_DI[1:0] : '00'->2K, '01'->8K, '10' : 4K, '11'->reserved.

	//GI(Guard Interval) mode manual set
	switch(pIsdbtConfigParam->gIMode)
	{
		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_4 :
			{
				mdgimode_di_1_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set Guard Interval = %s !!!\n","1_4 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_8 :
			{
				mdgimode_di_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set Guard Interval = %s !!!\n","1_8 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_16 :
			{
				mdgimode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set Guard Interval = %s !!!\n","1_16 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_32 :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set Guard Interval = %s !!!\n","1_32 ");
			}
			break;
		default :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN Guard Interval  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}


	Common_Wr01(common_ctrl83, mdgimode_di_1_0, mdgimode_di_1_0); //regAddr = 0x0053[1:0], MDGIMODE_DI[1:0]: '00'->1/32, '01'->1/16, '10' : 1/8, '11'->1/4.
	Common_WrFL(common_ctrl83);


	//Layer A QAM mode manual set
	switch(pIsdbtConfigParam->tmcc.layerAQamMode)
	{
		case LX_DEMOD_QPSK:
			{
				tmccqammodea_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","QPSK");
			}
			break;


		case LX_DEMOD_16QAM :
			{
				tmccqammodea_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","16QAM");
			}
			break;

		case LX_DEMOD_64QAM :
			{
				tmccqammodea_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","64QAM");
			}
			break;

		case LX_DEMOD_DQPSK:
			{
				tmccqammodea_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","DQPSK");
			}
			break;

		default :
			{
				tmccqammodea_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN QAM mode !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	tmccnumsega_i_3_0 = pIsdbtConfigParam->tmcc.layerASegmentNum;

	Common_RdFL(common_ctrl97);   ////regAddr = 0x0061[2:0]
	Common_Wr02(common_ctrl97, tmccqammodea_i_2_0, tmccqammodea_i_2_0, tmccnumsega_i_3_0, tmccnumsega_i_3_0); // TMCCQAMMODE_D[1:0] : '0'->QPSK, '1'->16QAM, '2' -> 64QAM, '3'->reserved.
	Common_WrFL(common_ctrl97); ////regAddr = 0x0061

	//Layer A Coder rate manual set
	switch(pIsdbtConfigParam->tmcc.layerACodeRate)
	{
		case LX_DEMOD_CODE_1_2 :
			{
				tmcccoderatea_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A code rate = %s !!!\n","1_2");
			}
			break;

		case LX_DEMOD_CODE_2_3 :
			{
				tmcccoderatea_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","2_3");
			}
			break;

		case LX_DEMOD_CODE_3_4 :
			{
				tmcccoderatea_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","3_4");
			}
			break;

		case LX_DEMOD_CODE_5_6 :
			{
				tmcccoderatea_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","5_6");
			}
			break;

		case LX_DEMOD_CODE_7_8 :
			{
				tmcccoderatea_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				tmcccoderatea_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN layer A  code rate !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}

	switch(pIsdbtConfigParam->tmcc.layerATimeInterleavingLen)
	{
		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_0 :
			{
				tmcctimeintlena_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A Lenth of time interleaving = %s !!!\n","0");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_4 :
			{
				tmcctimeintlena_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  Lenth of time interleaving  = %s !!!\n","4");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_8 :
			{
				tmcctimeintlena_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  Lenth of time interleaving  = %s !!!\n","8");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_16 :
			{
				tmcctimeintlena_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  Lenth of time interleaving  = %s !!!\n","16");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_32 :
			{
				tmcctimeintlena_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  Lenth of time interleaving  = %s !!!\n","32");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED:
			{
				tmcctimeintlena_i_2_0 = 0x7;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  Lenth of time interleaving  = %s !!!\n","unused");
			}
			break;

		default :
			{
				tmcccoderatea_i_2_0 = 0x6;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) layer A  Lenth of time interleaving reserved !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}


	Common_RdFL(common_ctrl98);   ////regAddr = 0x0062
	Common_Wr02(common_ctrl98, tmcccoderatea_i_2_0, tmcccoderatea_i_2_0, tmcctimeintlena_i_2_0, tmcctimeintlena_i_2_0); //TMCCCODERATEA_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	Common_WrFL(common_ctrl98); ////regAddr = 0x0062
	/*
	tmcctimeintlena_i_2_0
	Interleaving length manual set for hierarchical layer A
	000 : 0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 : 4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 : 8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 6(Mode 1), 8(Mode 2), 4(Mode 3)
	100-110 : reserved
	111 : unused hierarchical layer
	*/



	//Layer B QAM mode manual set
	switch(pIsdbtConfigParam->tmcc.layerBQamMode)
	{
		case LX_DEMOD_QPSK:
			{
				tmccqammodeb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","QPSK");
			}
			break;

		case LX_DEMOD_DQPSK:
			{
				tmccqammodeb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","DQPSK");
			}
			break;

		case LX_DEMOD_16QAM :
			{
				tmccqammodeb_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","16QAM");
			}
			break;

		case LX_DEMOD_64QAM :
			{
				tmccqammodeb_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				tmccqammodeb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN QAM mode !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	tmccnumsegb_i_3_0 = pIsdbtConfigParam->tmcc.layerBSegmentNum;

	Common_RdFL(common_ctrl99);   ////regAddr = 0x0063
	Common_Wr02(common_ctrl99, tmccqammodeb_i_2_0, tmccqammodeb_i_2_0, tmccnumsegb_i_3_0, tmccnumsegb_i_3_0);
	Common_WrFL(common_ctrl99); ////regAddr = 0x0063
	/*
	tmccqammodeb_i_2_0
	Carrier modulation scheme manual set for hierarchical layer B
	000 : DQPSK
	001 : QPSK
	010 : 16QAM
	011 : 64QAM
	100~110 : reserved
	111 : unused hierarchical layer

	tmccnumsegb_i_3_0
	Number of segments manual set for hierarchical layer B
	0000 : Reserved
	0001~1101 : 1 - 13 segment
	1110 : reserved
	1111 : unused hierarchical layer
	*/



	//Layer B Coder rate manual set
	switch(pIsdbtConfigParam->tmcc.layerBCodeRate)
	{
		case LX_DEMOD_CODE_1_2 :
			{
				tmcccoderateb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A code rate = %s !!!\n","1_2");
			}
			break;

		case LX_DEMOD_CODE_2_3 :
			{
				tmcccoderateb_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","2_3");
			}
			break;

		case LX_DEMOD_CODE_3_4 :
			{
				tmcccoderateb_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","3_4");
			}
			break;

		case LX_DEMOD_CODE_5_6 :
			{
				tmcccoderateb_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","5_6");
			}
			break;

		case LX_DEMOD_CODE_7_8 :
			{
				tmcccoderateb_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				tmcccoderateb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN layer A  code rate !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}

	switch(pIsdbtConfigParam->tmcc.layerBTimeInterleavingLen)
	{
		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_0 :
			{
				tmcctimeintlenb_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B Lenth of time interleaving = %s !!!\n","0");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_4 :
			{
				tmcctimeintlenb_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B  Lenth of time interleaving  = %s !!!\n","4");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_8 :
			{
				tmcctimeintlenb_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B  Lenth of time interleaving  = %s !!!\n","8");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_16 :
			{
				tmcctimeintlenb_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B Lenth of time interleaving  = %s !!!\n","16");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_32 :
			{
				tmcctimeintlenb_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B  Lenth of time interleaving  = %s !!!\n","32");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED:
			{
				tmcctimeintlenb_i_2_0 = 0x7;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer B  Lenth of time interleaving  = %s !!!\n","unused");
			}
			break;

		default :
			{
				tmcctimeintlenb_i_2_0 = 0x6;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) layer B  Lenth of time interleaving reserved !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}



	Common_RdFL(common_ctrl100);   ////regAddr = 0x0064
	Common_Wr02(common_ctrl100, tmcccoderateb_i_2_0, tmcccoderateb_i_2_0, tmcctimeintlenb_i_2_0, tmcctimeintlenb_i_2_0); //TMCCCODERATEB_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	Common_WrFL(common_ctrl100); ////regAddr = 0x0064
	/*
	tmcctimeintlenb_i_2_0
	Interleaving length manual set for hierarchical layer B
	000 : 0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 : 4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 : 8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 6(Mode 1), 8(Mode 2), 4(Mode 3)
	100~110 : reserved
	111 : unused hierarchical layer
	*/





	//Layer C QAM mode manual set
	switch(pIsdbtConfigParam->tmcc.layerCQamMode)
	{
		case LX_DEMOD_QPSK:
			{
				tmccqammodec_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","QPSK");
			}
			break;

		case LX_DEMOD_DQPSK:
			{
				tmccqammodec_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","DQPSK");
			}
			break;

		case LX_DEMOD_16QAM :
			{
				tmccqammodec_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","16QAM");
			}
			break;

		case LX_DEMOD_64QAM :
			{
				tmccqammodec_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				tmccqammodec_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN QAM mode !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	tmccnumsegc_i_3_0 = pIsdbtConfigParam->tmcc.layerCSegmentNum;

	Common_RdFL(common_ctrl101);   ////regAddr = 0x0065
	Common_Wr02(common_ctrl101, tmccqammodec_i_2_0, tmccqammodec_i_2_0, tmccnumsegc_i_3_0, tmccnumsegc_i_3_0);
	Common_WrFL(common_ctrl101); ////regAddr = 0x0065
	/*
	tmccqammodec_i_2_0
	Carrier modulation scheme manual set for hierarchical layer C
	000 : DQPSK
	001 : QPSK
	010 : 16QAM
	011 : 64QAM
	100~110 : reserved
	111 : unused hierarchical layer


	tmccnumsegc_i_3_0
	Number of segments manual set for hierarchical layer C
	0000 : Reserved
	0001~1101 : 1 - 13 segment
	1110 : reserved
	1111 : unused hierarchical layer
	*/


	//Layer C Coder rate manual set
	switch(pIsdbtConfigParam->tmcc.layerCCodeRate)
	{
		case LX_DEMOD_CODE_1_2 :
			{
				tmcccoderatec_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A code rate = %s !!!\n","1_2");
			}
			break;

		case LX_DEMOD_CODE_2_3 :
			{
				tmcccoderatec_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","2_3");
			}
			break;

		case LX_DEMOD_CODE_3_4 :
			{
				tmcccoderatec_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","3_4");
			}
			break;

		case LX_DEMOD_CODE_5_6 :
			{
				tmcccoderatec_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","5_6");
			}
			break;

		case LX_DEMOD_CODE_7_8 :
			{
				tmcccoderatec_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer A  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				tmcccoderatec_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) UNKNOWN layer A  code rate !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}

	switch(pIsdbtConfigParam->tmcc.layerCTimeInterleavingLen)
	{
		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_0 :
			{
				tmcctimeintlenc_i_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C Lenth of time interleaving = %s !!!\n","0");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_4 :
			{
				tmcctimeintlenc_i_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C  Lenth of time interleaving  = %s !!!\n","4");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_8 :
			{
				tmcctimeintlenc_i_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C  Lenth of time interleaving  = %s !!!\n","8");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_16 :
			{
				tmcctimeintlenc_i_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C Lenth of time interleaving  = %s !!!\n","16");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_32 :
			{
				tmcctimeintlenc_i_2_0 = 0x4;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C  Lenth of time interleaving  = %s !!!\n","32");
			}
			break;

		case LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED:
			{
				tmcctimeintlenc_i_2_0 = 0x7;
				DEMOD_PRINT("[DEMOD H14 GBB] Set layer C  Lenth of time interleaving  = %s !!!\n","unused");
			}
			break;

		default :
			{
				tmcctimeintlenc_i_2_0 = 0x6;
				DEMOD_PRINT("[DEMOD H14 GBB] FAIL,(%s:%d) layer C  Lenth of time interleaving reserved !!!\n", __F__, __L__);
			}
			return RET_ERROR;

	}

	Common_RdFL(common_ctrl102);   ////regAddr = 0x0066
	Common_Wr02(common_ctrl102, tmcccoderatec_i_2_0, tmcccoderatec_i_2_0,tmcctimeintlenc_i_2_0, tmcctimeintlenc_i_2_0); //TMCCCODERATEC_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	Common_WrFL(common_ctrl102); ////regAddr = 0x0066
	/*
	tmcctimeintlenc_i_2_0
	Interleaving length manual set for hierarchical layer C
	000 : 0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 : 4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 : 8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 6(Mode 1), 8(Mode 2), 4(Mode 3)
	100~110 : reserved
	111 : unused hierarchical layer
	*/


	return RET_OK;



}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 scsyslockchktime_7_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = (UINT8)(waitMs / 10);

	Common_Wr01(common_ctrl11,scsyslockchktime_7_0, scsyslockchktime_7_0);
	Common_WrFL(common_ctrl11);  //regAddr = 0x000b,COMMON_CTRL[11]
	//SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in msec resolution

	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_CrGearshEn
 * enable/disable Carrier recovery bandwidth gear shift
 *
 * @param bEnable 	[IN] BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_CrGearshEn(BOOLEAN bEnable)
{
	UINT8 crgearshen;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	crgearshen = (bEnable == TRUE) ? 0x1 : 0x0;
	Common_RdFL(common_ctrl68);  //regAddr = 0x0044, COMMON_CTRL[68]
	Common_Wr01(common_ctrl68, crgearshen, crgearshen);
	Common_WrFL(common_ctrl68);
	// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
	// 0 : disable
	// 1 : enable
	DEMOD_PRINT("[DEMOD H13 DTV](%s)(%s) \n",__F__, (bEnable == TRUE) ? "ON" : "OFF");

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement
 * Get delay spread state on ISDBT.
 *
 * @param [IN] bEnhanced - BOOLEAN 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement(BOOLEAN bEnhanced)
{

	UINT16 delaySpread;


	LX_DEMOD_LOCK_STATE_T txModeLock;
	LX_DEMOD_FFTMODE_T fftMode;

	UINT8 cirth_7_0 = 0;
	UINT8 cirth_9_8 = 0;

	DEMOD_PRINT("[DEMOD_13] == @ %s!!!\n",__F__);

	if(FALSE == bEnhanced)
		return RET_OK;

	if (RET_OK != DEMOD_M14B0_ISDBT_Get_FFTMode( &txModeLock, &fftMode ))
	{
		DEMOD_PRINT("[DEMOD_13] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Get_FFTMode() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if (RET_OK != DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD_13] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD_13] txmode lock =%s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? "LOCKED" : "NOT LOCKED" );
	DEMOD_PRINT("[DEMOD_13] FFT = %s!!!\n",(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "8K" :
												  (fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K) ? "4K" :
												  (fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "2K" : "UNKOWN");



	DEMOD_PRINT("[DEMOD_13]	Delay spread = %d !!!\n",delaySpread);

	if(LX_DEMOD_LOCKED != txModeLock)
	{
		DEMOD_PRINT("[DEMOD_13](%s, %d) FAIL, TX mode lock is NOT LOCKED !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if(LX_DEMOD_DVBT_ISDBT_FFT_8K == fftMode)
	{

		if((delaySpread > 2040) && (delaySpread < 2071))
		{
			cirth_7_0 = 0xe0;
			EQ_DI_RdFL(eq_di_ctrl39); //regAddr = 0x2227, EQ_DI_CTRL[39] : CIR control register
			EQ_DI_Wr01(eq_di_ctrl39,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl39);

			cirth_9_8 = 0x1;
			EQ_DI_RdFL(eq_di_ctrl38); //regAddr = 0x2226, EQ_DI_CTRL[38]: CIR control register
			EQ_DI_Wr01(eq_di_ctrl38,cirth_9_8, cirth_9_8 );  //CIRTH[9:8]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl38);
		}
		else if((delaySpread > 1343) && (delaySpread < 1388))
		{
			cirth_7_0 = 0xe0;
			EQ_DI_RdFL(eq_di_ctrl39); //regAddr = 0x2227, EQ_DI_CTRL[39] : CIR control register
			EQ_DI_Wr01(eq_di_ctrl39,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl39);

			cirth_9_8 = 0x1;
			EQ_DI_RdFL(eq_di_ctrl38); //regAddr = 0x2226, EQ_DI_CTRL[38]: CIR control register
			EQ_DI_Wr01(eq_di_ctrl38,cirth_9_8, cirth_9_8 );  //CIRTH[9:8]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl38);

		}
		else if((delaySpread > 660) && (delaySpread < 705))
		{
			cirth_7_0 = 0xe0;
			EQ_DI_RdFL(eq_di_ctrl39); //regAddr = 0x2227, EQ_DI_CTRL[39] : CIR control register
			EQ_DI_Wr01(eq_di_ctrl39,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl39);

			cirth_9_8 = 0x1;
			EQ_DI_RdFL(eq_di_ctrl38); //regAddr = 0x2226, EQ_DI_CTRL[38]: CIR control register
			EQ_DI_Wr01(eq_di_ctrl38,cirth_9_8, cirth_9_8 );  //CIRTH[9:8]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl38);

		}
		else if(delaySpread > 85)
		{
			cirth_7_0 = 0x27;
			EQ_DI_RdFL(eq_di_ctrl39); //regAddr = 0x2227, EQ_DI_CTRL[39] : CIR control register
			EQ_DI_Wr01(eq_di_ctrl39,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl39);

			cirth_9_8 = 0x0;
			EQ_DI_RdFL(eq_di_ctrl38); //regAddr = 0x2226, EQ_DI_CTRL[38]: CIR control register
			EQ_DI_Wr01(eq_di_ctrl38,cirth_9_8, cirth_9_8 );  //CIRTH[9:8]: CIR detection threshold value set
			EQ_DI_WrFL(eq_di_ctrl38);
		}
		else
		{
			/* do nothing ..... */
		}

	}
	else
	{
		cirth_7_0 = 0x27;
		EQ_DI_RdFL(eq_di_ctrl39); //regAddr = 0x2227, EQ_DI_CTRL[39] : CIR control register
		EQ_DI_Wr01(eq_di_ctrl39,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
		EQ_DI_WrFL(eq_di_ctrl39);

		cirth_9_8 = 0x0;
		EQ_DI_RdFL(eq_di_ctrl38); //regAddr = 0x2226, EQ_DI_CTRL[38]: CIR control register
		EQ_DI_Wr01(eq_di_ctrl38,cirth_9_8, cirth_9_8 );  //CIRTH[9:8]: CIR detection threshold value set
		EQ_DI_WrFL(eq_di_ctrl38);
	}


	DEMOD_PRINT("[DEMOD_13]	cirth_7_0 = 0x%x, cirth_9_8 = 0x%x !!!\n",cirth_7_0,cirth_9_8);
	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus
 * Get delay spread state on ISDB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{

	UINT8	dlyspread_7_0;
	UINT8 	dlyspread_11_8;

	UINT16 delaySpread = 0;

	*pDelaySpread = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon70);  //regAddr = 0x00c6
	Common_Rd01(common_mon70,dlyspread_11_8,dlyspread_11_8); //DLYSPREAD[11:8]

	delaySpread = (dlyspread_11_8  << 8);

	Common_RdFL(common_mon71);  //regAddr = 0x00c7, CIR_MON[1]
	Common_Rd01(common_mon71,dlyspread_7_0,dlyspread_7_0); //DLYSPREAD[7:0]

	delaySpread += (dlyspread_7_0 << 0);

	*pDelaySpread = delaySpread;

	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_FFTMode
 * Get the information of bandwidth on ISDB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{

	UINT8 	modelock_di;
	UINT8 	fftmoder_di_1_0;

	*pTxModeLock = LX_DEMOD_UNLOCKED;
	*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon42);  //regAddr = 0x00aa
	Common_Rd01(common_mon42,modelock_di,modelock_di); //MODELOCK_DI : Transmission mode lock status indication for DVB-T/ISDB-T
															// 0-> unlock, 1-> lock
	if(modelock_di)
	{
		*pTxModeLock = LX_DEMOD_LOCKED;

		DEMOD_PRINT("[DEMOD H14 GBB] TX mode LOCKED !!!\n");

		Common_RdFL(common_mon42);  //regAddr = 0x00aa, DIEQ_MON[0]
		Common_Rd01(common_mon42,fftmoder_di_1_0,fftmoder_di_1_0); //FFTMODER_DI[1:0] : '0'-> 2K, '1'-> 8K, '2'->4k(ISDBT only), '3' -> reserved.

		switch(fftmoder_di_1_0)
		{
			case 0x00 :
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_2K;
				break;

			case 0x01:
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_8K;
				break;

			case 0x02:
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_4K;
				break;

			default :
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;
				break;
		}

	}
	else
	{
		*pTxModeLock = LX_DEMOD_UNLOCKED;
		DEMOD_PRINT("[DEMOD H14 GBB] TX mode is NOT LOCKED !!!\n");
	}

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_GuradIntervalMode
 * Get the information of guard interval on ISDB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{

	UINT8 	gimoder_di_1_0;

	*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon42);  //regAddr = 0xaa
	Common_Rd01(common_mon42,gimoder_di_1_0,gimoder_di_1_0); //GIMODER_DI[1:0] : : '0'-> 1/32, '1'-> 1/16, '2' -> 1/8, '3' -> 1/4.

	switch(gimoder_di_1_0)
	{
		case 0x00 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_32;
			break;
		case 0x01:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_16;
			break;
		case 0x02 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_8;
			break;
		case 0x03:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_4;
			break;
		default :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;
			break;
	}


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_TMCCInfo
 * Get TPS information on ISDBT
 *
 * @param *pTpsParams [OUT] LX_DEMOD_ISDBT_TMCC_CONFIG_T - TMCC data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_TMCCInfo( LX_DEMOD_ISDBT_TMCC_CONFIG_T *pTmccParams)
{

	UINT8	tmcccoderatear_i_2_0;
	UINT8	tmccqammodear_i_2_0;

	UINT8 	tmcccoderatebr_i_2_0;
	UINT8	tmccqammodebr_i_2_0;

	UINT8 	tmcccoderatecr_i_2_0;
	UINT8	tmccqammodecr_i_2_0;

	UINT8 	tmccnumsegar_i_3_0;
	UINT8 	tmccnumsegbr_i_3_0;
	UINT8 	tmccnumsegcr_i_3_0;

	UINT8 	tmcctimeintlenar_i_2_0;
	UINT8 	tmcctimeintlenbr_i_2_0;
	UINT8 	tmcctimeintlencr_i_2_0;

	LX_DEMOD_LOCK_STATE_T txModeLock;
	LX_DEMOD_FFTMODE_T fftMode;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	if(NULL == pTmccParams)
	{
		DEMOD_PRINT("[DEMOD H14 GBB] ERROR, LX_DEMOD_ISDBT_TMCC_CONFIG_T pTmccParams is NULL !!!\n");
		return RET_ERROR;
	}

 	// Get FFT mode
 	if (RET_OK != DEMOD_M14B0_ISDBT_Get_FFTMode( &txModeLock, &fftMode ))
 	{
 		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Get_FFTMode() !!!\n", __F__, __L__);
 		return RET_ERROR;
	}

	//Layer A



	Common_RdFL(common_mon57);   ////regAddr = 0x00b9, IEQ_MON[4]
	Common_Rd02(common_mon57, tmcccoderatear_i_2_0, tmcccoderatear_i_2_0, tmcctimeintlenar_i_2_0, tmcctimeintlenar_i_2_0); //TMCCCODERATEA_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	/*
	tmcctimeintlenar_i_2_0
	Interleaving length indication for hierarchical layer A of ISDB-T
	000 :  0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 :  4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 :  8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 16(Mode 1), 8(Mode 2), 4(Mode 3)
	100~110 : reserved
	111 : unused hierarchical layer
	*/



	Common_RdFL(common_mon56);   ////regAddr = 0x00b8, IEQ_MON[3]
	Common_Rd02(common_mon56, tmccqammodear_i_2_0, tmccqammodear_i_2_0, tmccnumsegar_i_3_0, tmccnumsegar_i_3_0); // TMCCQAMMODE_D[1:0] : '0'->QPSK, '1'->16QAM, '2' -> 64QAM, '3'->reserved.
	/*
	tmccnumsegar_i_3_0
	Number of segments indication for hierarchical layer A of ISDB-T
	0000 : reserved
	0001~1101 : 1 - 13 segment
	1110 : reserved
	1111 : unused hierarchical layer
	*/


	//Layer A QAM mode manual set
	switch(tmccqammodear_i_2_0)
	{
		case 0x0:
			{
				pTmccParams->layerAQamMode = LX_DEMOD_DQPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  QAM mode = %s !!!\n","DQPSK");
			}
			break;


		case 0x1 :
			{
				pTmccParams->layerAQamMode = LX_DEMOD_QPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  QAM mode = %s !!!\n","QPSK");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerAQamMode = LX_DEMOD_16QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  QAM mode = %s !!!\n","16QAM");
			}
			break;

		case 0x3:
			{
				pTmccParams->layerAQamMode = LX_DEMOD_64QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN QAM mode !!!\n");
			}
			break;
	}

	pTmccParams->layerASegmentNum = (tmccnumsegar_i_3_0 <= 13) ? tmccnumsegar_i_3_0 : 0x00;
	DEMOD_PRINT("[DEMOD H14 GBB] Get layer A	Seg # = %d !!!\n",pTmccParams->layerASegmentNum);


	switch(tmcccoderatear_i_2_0)
	{
		case 0x0 :
			{
				pTmccParams->layerACodeRate = LX_DEMOD_CODE_1_2;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A code rate = %s !!!\n","1_2");
			}
			break;

		case 0x1 :
			{
				pTmccParams->layerACodeRate = LX_DEMOD_CODE_2_3;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  code rate = %s !!!\n","2_3");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerACodeRate = LX_DEMOD_CODE_3_4;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  code rate = %s !!!\n","3_4");
			}
			break;

		case 0x3 :
			{
				pTmccParams->layerACodeRate = LX_DEMOD_CODE_5_6;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  code rate = %s !!!\n","5_6");
			}
			break;

		case 0x4 :
			{
				pTmccParams->layerACodeRate = LX_DEMOD_CODE_7_8;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN Code rate !!!\n");
			}
			break;

	}

	if(txModeLock == LX_DEMOD_LOCKED)
	{

		switch(tmcctimeintlenar_i_2_0)
		{
			case 0x0 :
				{
					pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_0;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer A Lenth of time interleaving = %s !!!\n","0");
				}
				break;

			case 0x1 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","4");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","2");
					}
					else
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_1;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","1");
					}
				}
				break;

			case 0x2 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","8");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","4");
					}
					else
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","2");
					}
				}
				break;

			case 0x3 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_16;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","16");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","8");
					}
					else
					{
						pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","4");
					}
				}
				break;

			case 0x7:
				{
					pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving  = %s !!!\n","unused");
				}
				break;

			default :
				{
					pTmccParams->layerATimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_reserved;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer A  Lenth of time interleaving reserved !!!\n");
				}
				break;

		}
	}


	//Layer B

	Common_RdFL(common_mon59);   ////regAddr = 0x00bb, IEQ_MON[6]
	Common_Rd02(common_mon59, tmcccoderatebr_i_2_0, tmcccoderatebr_i_2_0, tmcctimeintlenbr_i_2_0, tmcctimeintlenbr_i_2_0); //TMCCCODERATEA_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	/*
	tmcctimeintlenbr_i_2_0
	Interleaving length indication for hierarchical layer A of ISDB-T
	000 :  0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 :  4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 :  8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 16(Mode 1), 8(Mode 2), 4(Mode 3)
	100~110 : reserved
	111 : unused hierarchical layer
	*/


	Common_RdFL(common_mon58);   ////regAddr = 0x00ba, IEQ_MON[5]
	Common_Rd02(common_mon58, tmccqammodebr_i_2_0, tmccqammodebr_i_2_0, tmccnumsegbr_i_3_0, tmccnumsegbr_i_3_0);
	/*
	tmccqammodebr_i_2_0
	Carrier modulation scheme indication for hierarchical layer B of ISDB-T
	000 : DQPSK
	001 : QPSK
	010 : 16QAM
	011 : 64QAM
	100~110 : reserved
	111 : unused hierarchical layer

	tmccnumsegbr_i_3_0
	Number of segments indication for hierarchical layer B of ISDB-T
	0000 : reserved
	0001~1101 : 1 - 13 segment
	1110 : reserved
	1111 : unused hierarchical layer
	*/



	switch(tmccqammodebr_i_2_0)
	{
		case 0x0:
			{
				pTmccParams->layerBQamMode = LX_DEMOD_DQPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  QAM mode = %s !!!\n","DQPSK");
			}
			break;


		case 0x1 :
			{
				pTmccParams->layerBQamMode = LX_DEMOD_QPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  QAM mode = %s !!!\n","QPSK");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerBQamMode = LX_DEMOD_16QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  QAM mode = %s !!!\n","16QAM");
			}
			break;

		case 0x3:
			{
				pTmccParams->layerBQamMode = LX_DEMOD_64QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN QAM mode !!!\n");
			}
			break;
	}

	pTmccParams->layerBSegmentNum = (tmccnumsegbr_i_3_0 <= 13) ? tmccnumsegbr_i_3_0 : 0x00;
	DEMOD_PRINT("[DEMOD H14 GBB] Get layer B	Seg # = %d !!!\n",pTmccParams->layerBSegmentNum);


	switch(tmcccoderatebr_i_2_0)
	{
		case 0x0 :
			{
				pTmccParams->layerBCodeRate = LX_DEMOD_CODE_1_2;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B code rate = %s !!!\n","1_2");
			}
			break;

		case 0x1 :
			{
				pTmccParams->layerBCodeRate = LX_DEMOD_CODE_2_3;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  code rate = %s !!!\n","2_3");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerBCodeRate = LX_DEMOD_CODE_3_4;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  code rate = %s !!!\n","3_4");
			}
			break;

		case 0x3 :
			{
				pTmccParams->layerBCodeRate = LX_DEMOD_CODE_5_6;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  code rate = %s !!!\n","5_6");
			}
			break;

		case 0x4 :
			{
				pTmccParams->layerBCodeRate = LX_DEMOD_CODE_7_8;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN Code rate !!!\n");
			}
			break;

	}

	if(txModeLock == LX_DEMOD_LOCKED)
	{

		switch(tmcctimeintlenbr_i_2_0)
		{
			case 0x0 :
				{
					pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_0;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer B Lenth of time interleaving = %s !!!\n","0");
				}
				break;

			case 0x1 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","4");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","2");
					}
					else
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_1;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","1");
					}
				}
				break;

			case 0x2 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","8");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","4");
					}
					else
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","2");
					}
				}
				break;

			case 0x3 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_16;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","16");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","8");
					}
					else
					{
						pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","4");
					}
				}
				break;

			case 0x7:
				{
					pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving  = %s !!!\n","unused");
				}
				break;

			default :
				{
					pTmccParams->layerBTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_reserved;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer B  Lenth of time interleaving reserved !!!\n");
				}
				break;

		}
	}

	//Layer C
	Common_RdFL(common_mon61);   ////regAddr = 0x00bd
	Common_Rd02(common_mon61, tmcccoderatecr_i_2_0, tmcccoderatecr_i_2_0, tmcctimeintlencr_i_2_0, tmcctimeintlencr_i_2_0); //TMCCCODERATEA_I[2:0] : '00'->1/2, '1'->2/3, '2'->3/4,'3'->5/6,'4'->7/8.
	/*
	tmcccoderatecr_i_2_0
	Convolutional-coding rate indication for hierarchical layer C of ISDB-T
	000 : 1/2
	001 : 2/3
	010 : 3/4
	011 : 5/6
	100 : 7/8
	101~110 : reserved
	111 : unused hierarchical layer

	tmcctimeintlencr_i_2_0
	Interleaving length indication for hierarchical layer C of ISDB-T
	000 :  0(Mode 1), 0(Mode 2), 0(Mode 3)
	001 :  4(Mode 1), 2(Mode 2), 1(Mode 3)
	010 :  8(Mode 1), 4(Mode 2), 2(Mode 3)
	011 : 16(Mode 1), 8(Mode 2), 4(Mode 3)
	100~110 : reserved
	111 : unused hierarchical layer
	*/


	Common_RdFL(common_mon60);   ////regAddr = 0x00bc
	Common_Rd02(common_mon60, tmccqammodecr_i_2_0, tmccqammodecr_i_2_0, tmccnumsegcr_i_3_0, tmccnumsegcr_i_3_0);
	/*
	tmccqammodecr_i_2_0
	Carrier modulation scheme indication for hierarchical layer C of ISDB-T
	000 : DQPSK
	001 : QPSK
	010 : 16QAM
	011 : 64QAM
	100~110 : reserved
	111 : unused hierarchical layer

	tmccnumsegcr_i_3_0
	Number of segments indication for hierarchical layer C of ISDB-T
	0000 : reserved
	0001~1101 : 1 - 13 segment
	1110 : reserved
	1111 : unused hierarchical layer
	*/

	switch(tmccqammodecr_i_2_0)
	{
		case 0x0:
			{
				pTmccParams->layerCQamMode = LX_DEMOD_DQPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  QAM mode = %s !!!\n","DQPSK");
			}
			break;


		case 0x1 :
			{
				pTmccParams->layerCQamMode = LX_DEMOD_QPSK;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  QAM mode = %s !!!\n","QPSK");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerCQamMode = LX_DEMOD_16QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  QAM mode = %s !!!\n","16QAM");
			}
			break;

		case 0x3:
			{
				pTmccParams->layerCQamMode = LX_DEMOD_64QAM;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  QAM mode = %s !!!\n","64QAM");
			}
			break;



		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN QAM mode !!!\n");
			}
			break;
	}

	pTmccParams->layerCSegmentNum = (tmccnumsegcr_i_3_0 <= 13) ? tmccnumsegcr_i_3_0 : 0x00;
	DEMOD_PRINT("[DEMOD H14 GBB] Get layer C	Seg # = %d !!!\n",pTmccParams->layerCSegmentNum);


	switch(tmcccoderatecr_i_2_0)
	{
		case 0x0 :
			{
				pTmccParams->layerCCodeRate = LX_DEMOD_CODE_1_2;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C code rate = %s !!!\n","1_2");
			}
			break;

		case 0x1 :
			{
				pTmccParams->layerCCodeRate = LX_DEMOD_CODE_2_3;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  code rate = %s !!!\n","2_3");
			}
			break;

		case 0x2 :
			{
				pTmccParams->layerCCodeRate = LX_DEMOD_CODE_3_4;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  code rate = %s !!!\n","3_4");
			}
			break;

		case 0x3 :
			{
				pTmccParams->layerCCodeRate = LX_DEMOD_CODE_5_6;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  code rate = %s !!!\n","5_6");
			}
			break;

		case 0x4 :
			{
				pTmccParams->layerCCodeRate = LX_DEMOD_CODE_7_8;
				DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  code rate = %s !!!\n","7_8");
			}
			break;

		default :
			{
				DEMOD_PRINT("[DEMOD H14 GBB] UNKNOWN Code rate !!!\n");
			}
			break;

	}

	if(txModeLock == LX_DEMOD_LOCKED)
	{

		switch(tmcctimeintlencr_i_2_0)
		{
			case 0x0 :
				{
					pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_0;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer C Lenth of time interleaving = %s !!!\n","0");
				}
				break;

			case 0x1 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","4");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","2");
					}
					else
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_1;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","1");
					}
				}
				break;

			case 0x2 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","8");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","4");
					}
					else
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_2;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","2");
					}
				}
				break;

			case 0x3 :
				{
					if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_2K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_16;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","16");
					}
					else if(fftMode == LX_DEMOD_DVBT_ISDBT_FFT_4K)
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_8;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","8");
					}
					else
					{
						pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_4;
						DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","4");
					}
				}
				break;

			case 0x7:
				{
					pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving  = %s !!!\n","unused");
				}
				break;

			default :
				{
					pTmccParams->layerCTimeInterleavingLen = LX_DEMOD_ISDBT_TIME_INTERLEAVING_reserved;
					DEMOD_PRINT("[DEMOD H14 GBB] Get layer C  Lenth of time interleaving reserved !!!\n");
				}
				break;

		}
	}


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_TotalInfo
 * Get all information on ISDB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_TotalInfo( void)
{

	LX_DEMOD_ISDBT_CONFIG_T				isdbtConfigParam;
	LX_DEMOD_ISDBT_TMCC_CONFIG_T		tmccParams;
	LX_DEMOD_LOCK_STATE_T 			agcLockStatus;
	LX_DEMOD_LOCK_STATE_T			neverLockStatus;
	LX_DEMOD_LOCK_STATE_T 			txModeLock;

	UINT16 								delaySpread;
	UINT16 								ifagcValue = 0;
	UINT32 								errorCount;
	SINT32 								FreqOff;

	LX_DEMOD_BWMODE_T 					channelBW = LX_DEMOD_BW_6MHZ;
	LX_DEMOD_FFTMODE_T 					FFTMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN ;
	LX_DEMOD_GIMODE_T 					gIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	LX_DEMOD_OPER_MODE_T 				scanMode;
	LX_DEMOD_LOCK_STATE_T 				spectrumAuto;
	LX_DEMOD_LOCK_STATE_T 				spectrumInv;

	LX_DEMOD_MSE_T 						sigMse;



	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);



 	// Get channelBW

 	if (RET_OK != DEMOD_M14B0_Get_BandWidthMode( &channelBW ))
 	{
 		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_DVBT_Get_BandWidthMode() !!!\n", __F__, __L__);
 		return RET_ERROR;
 	}

 	isdbtConfigParam.channelBW = channelBW;
 	DEMOD_PRINT("[DEMOD H14 GBB] Bandwidth = %s !!!\n",
 		(channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
 		(channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
 		(channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


 	// Get FFT mode
 	if (RET_OK != DEMOD_M14B0_ISDBT_Get_FFTMode( &txModeLock, &FFTMode ))
 	{
 		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Get_FFTMode() !!!\n", __F__, __L__);
 		return RET_ERROR;
	}

	isdbtConfigParam.FFTMode = FFTMode;
	DEMOD_PRINT("[DEMOD H14 GBB] Tx mode = %s, FFTMode = %s  !!!\n",
		(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked",
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	if (RET_OK != DEMOD_M14B0_ISDBT_Get_GuradIntervalMode( &gIMode ))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	isdbtConfigParam.gIMode = gIMode;
	DEMOD_PRINT("[DEMOD H14 GBB] Guard Intterval = %s  !!!\n",
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );



	if (RET_OK != DEMOD_M14B0_ISDBT_Get_TMCCInfo(&tmccParams))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_GetTMCCInfo() !!!\n", __F__, __L__);
	}

	isdbtConfigParam.tmcc = tmccParams;

	if (RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_AGC_LOCK, &agcLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
		agcLockStatus = LX_DEMOD_UNKOWN_STATE;
	}
	if(LX_DEMOD_LOCKED == agcLockStatus)
	{
		if (RET_OK != DEMOD_M14B0_Get_IFAGC(&ifagcValue))
		{
			DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_IFAGC() !!!\n", __F__, __L__);
			ifagcValue = 0;
		}
	}

	scanMode = LX_DEMOD_ISDBT;
	if (RET_OK != DEMOD_M14B0_Get_NeverLockStatus(scanMode, &neverLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_NeverLockStatus() !!!\n", __F__, __L__);
		neverLockStatus = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK !=  DEMOD_M14B0_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK != DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}

	//TP Error count
	if (RET_OK !=  DEMOD_M14B0_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_Packet_Error() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14 GBB] TP Error Count= %d !!!\n",errorCount);

	if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset( &FreqOff ))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		FreqOff = 0xffff;
	}

 	//MSE

	if (RET_OK != DEMOD_M14B0_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	DEMOD_PRINT("[DEMOD H14 GBB] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 GBB] IF AGC value = %d(0x%x) !!!\n",ifagcValue,ifagcValue);
	DEMOD_PRINT("[DEMOD H14 GBB] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 GBB] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	DEMOD_PRINT("[DEMOD H14 GBB] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD H14 GBB] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 GBB] offset = %d [KHz]!!!\n",FreqOff);
	DEMOD_PRINT("[DEMOD H14 GBB] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	DEMOD_PRINT("[DEMOD H14 GBB] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 GBB] IF AGC value = %d !!!\n",ifagcValue);
	DEMOD_PRINT("[DEMOD H14 GBB] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 GBB] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	DEMOD_PRINT("[DEMOD H14 GBB] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD H14 GBB] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");


	// Get channelBW
	DEMOD_PRINT("[DEMOD H14 GBB] Bandwidth = %s  !!!\n",
		(isdbtConfigParam.channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(isdbtConfigParam.channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(isdbtConfigParam.channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	DEMOD_PRINT("[DEMOD H14 GBB] FFTMode = %s  !!!\n",
		(isdbtConfigParam.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(isdbtConfigParam.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(isdbtConfigParam.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_4K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	DEMOD_PRINT("[DEMOD H14 GBB] Guard Intterval = %s  !!!\n",
		(isdbtConfigParam.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(isdbtConfigParam.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(isdbtConfigParam.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(isdbtConfigParam.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );

	// Get Layer A QAM mode
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer A] QAM mode  = %s  !!!\n",
		(isdbtConfigParam.tmcc.layerAQamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(isdbtConfigParam.tmcc.layerAQamMode == LX_DEMOD_DQPSK) ?  "DQPSK"  :
		(isdbtConfigParam.tmcc.layerAQamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(isdbtConfigParam.tmcc.layerAQamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );


	// Get Layer A coder rate
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer A] code rate = %s  !!!\n",
		(isdbtConfigParam.tmcc.layerACodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(isdbtConfigParam.tmcc.layerACodeRate == LX_DEMOD_CODE_2_3) ?  "Code_2_3" :
		(isdbtConfigParam.tmcc.layerACodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(isdbtConfigParam.tmcc.layerACodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(isdbtConfigParam.tmcc.layerACodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get Layer A  the length of time interleaving
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer A] time interleaveing len  = %s	!!!\n",
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_0) ?	 "0"  :
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_4) ?   "4"  :
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_8) ?   "8"  :
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_16) ?  "16" :
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_32) ?  "32" :
		(isdbtConfigParam.tmcc.layerATimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED) ?  "UNUSED" : " Reserved" );

	// Get Layer A num of segment
	DEMOD_PRINT("[DEMOD H14 GBB] [Layer A] the Num of Segment   = %d	!!!\n",isdbtConfigParam.tmcc.layerASegmentNum);


	// Get Layer B QAM mode
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer B] QAM mode  = %s	!!!\n",
		(isdbtConfigParam.tmcc.layerBQamMode == LX_DEMOD_QPSK) ?	"QPSK"	:
		(isdbtConfigParam.tmcc.layerBQamMode == LX_DEMOD_DQPSK) ?	"DQPSK"  :
		(isdbtConfigParam.tmcc.layerBQamMode == LX_DEMOD_16QAM) ?	"16QAM" :
		(isdbtConfigParam.tmcc.layerBQamMode == LX_DEMOD_64QAM) ?	"64QAM" : " UNKOWN MODE" );


	// Get Layer B coder rate
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer B] code rate = %s	!!!\n",
		(isdbtConfigParam.tmcc.layerBCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(isdbtConfigParam.tmcc.layerBCodeRate == LX_DEMOD_CODE_2_3) ?  "Code_2_3" :
		(isdbtConfigParam.tmcc.layerBCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(isdbtConfigParam.tmcc.layerBCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(isdbtConfigParam.tmcc.layerBCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );



	// Get Layer B the length of time interleaving
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer B] time interleaveing len	= %s	!!!\n",
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_0) ?	 "0"  :
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_4) ?	"4"  :
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_8) ?	"8"  :
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_16) ?	"16" :
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_32) ?	"32" :
		(isdbtConfigParam.tmcc.layerBTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED) ?	"UNUSED" : " Reserved" );

	// Get Get Layer B num of segment
	DEMOD_PRINT("[DEMOD H14 GBB] [Layer B] the Num of Segment	= %d	!!!\n",isdbtConfigParam.tmcc.layerBSegmentNum);


	// Get Layer C QAM mode
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer C] QAM mode  = %s	!!!\n",
		(isdbtConfigParam.tmcc.layerCQamMode == LX_DEMOD_QPSK) ?	"QPSK"	:
		(isdbtConfigParam.tmcc.layerCQamMode == LX_DEMOD_DQPSK) ?	"DQPSK"  :
		(isdbtConfigParam.tmcc.layerCQamMode == LX_DEMOD_16QAM) ?	"16QAM" :
		(isdbtConfigParam.tmcc.layerCQamMode == LX_DEMOD_64QAM) ?	"64QAM" : " UNKOWN MODE" );


	// Get Layer C coder rate
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer C] code rate = %s	!!!\n",
		(isdbtConfigParam.tmcc.layerCCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(isdbtConfigParam.tmcc.layerCCodeRate == LX_DEMOD_CODE_2_3) ?  "Code_2_3" :
		(isdbtConfigParam.tmcc.layerCCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(isdbtConfigParam.tmcc.layerCCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(isdbtConfigParam.tmcc.layerCCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );



	// Get Layer C the length of time interleaving
	DEMOD_PRINT("[DEMOD H14 GBB]  [Layer C] time interleaveing len	= %s	!!!\n",
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_0) ?	 "0"  :
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_4) ?	"4"  :
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_8) ?	"8"  :
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_16) ?	"16" :
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_32) ?	"32" :
		(isdbtConfigParam.tmcc.layerCTimeInterleavingLen == LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED) ?	"UNUSED" : " Reserved" );

	// Get Get Layer C num of segment
	DEMOD_PRINT("[DEMOD H14 GBB] [Layer C] the Num of Segment	= %d	!!!\n",isdbtConfigParam.tmcc.layerCSegmentNum);


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_IFO_LOCK
 * Get IFO lock on ISDB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{
	UINT8	syslock;
	UINT16 	intfreqoffsetvalue = 0;
	UINT8	crifo_di_7_0;
	UINT8	crifo_di_9_8;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon0);  //regAddr = 0x0080
	Common_Rd01(common_mon0,syslock,syslock); //SYSLOCK : System lock is come from ifolock for DVB-T, from  framelock for DVB-C


	Sync_RdFL(sync_mon36);  //regAddr = 0x10a4
	Sync_Rd01(sync_mon36,crifo_di_9_8,crifo_di_9_8); //CRIFO_DI[9:8]

	Sync_RdFL(sync_mon37);  //regAddr = 0x10a5
	Sync_Rd01(sync_mon37,crifo_di_7_0,crifo_di_7_0); //CRIFO_DI[7:0]

	intfreqoffsetvalue = (crifo_di_9_8 << 8) + crifo_di_7_0;

	DEMOD_PRINT("[DEMOD H14 GBB](%s) IFO LOCK = %s !!!\n", __F__, (syslock == 1) ? "LOCK" :"NOT LOCK");
	DEMOD_PRINT("[DEMOD H14 GBB](%s) IFO value = %d !!!\n", __F__,intfreqoffsetvalue);

	if(syslock == 1)
	{
		*pIfoLock = TRUE;
	}
	else
	{
		*pIfoLock = FALSE;
}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on ISDB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{
	UINT8 aiccrejbusy_3_0;
	UINT8 aiccrejstatus_3_0;

	Common_RdFL(common_mon16); //regAddr = 0x0090, COMMON_MON[16]
	Common_Rd02(common_mon16,aiccrejbusy_3_0,aiccrejbusy_3_0, aiccrejstatus_3_0,aiccrejstatus_3_0);
	//AICCREJSTATUS[3:0] : AICC rejector control status indication
		//AICCREJSTATUS[0] : for 1st rejector
		//AICCREJSTATUS[1] : for 2nd rejector
		//AICCREJSTATUS[2] : for 3rd rejector
		//AICCREJSTATUS[3] : for 4th rejector
	// 0 : off
	// 1 : on

	//AICCREJBUSY[3:0] : AICC rejector operation status indication
		//AICCREJSTATUS[0] : for 1st rejector
		//AICCREJSTATUS[1] : for 2nd rejector
		//AICCREJSTATUS[2] : for 3rd rejector
		//AICCREJSTATUS[3] : for 4th rejector
	// 0 : off
	// 1 : operation status

	if((aiccrejbusy_3_0 != 0x0) || (aiccrejstatus_3_0 != 0x0))
	{
		*pDetStatus = TRUE;
	}
	else
	{
		*pDetStatus = FALSE;

	}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ISDBT_EqualizereReset(void)
{

	UINT8 softrsteqb;

	softrsteqb = 0; //SOFTRSTEQB -> 0 : reset Equalizer

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl0);  //regAddr = 0x0000
	Common_Wr01(common_ctrl0, softrsteqb, softrsteqb);
	Common_WrFL(common_ctrl0);

	OS_MsecSleep(20);

	softrsteqb = 1; //SOFTRSTEQB -> 1 : normal

	Common_Wr01(common_ctrl0, softrsteqb, softrsteqb);
	Common_WrFL(common_ctrl0);

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_ISDBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_M14B0_ISDBT_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.isdbt.tuneMode)
				{
					if(RET_OK != DEMOD_M14B0_ISDBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_M14B0_ISDBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.isdbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) V() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{
					if(RET_OK != DEMOD_M14B0_ISDBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_M14B0_ISDBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.isdbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

				}

				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_RETUNE_RF : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.isdbt.tuneMode)
				{

					if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{

					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					return RET_OK;
				}

				if(RET_OK != DEMOD_M14B0_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^G^[DEMOD] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
					pDemodInfo->signalStatus.bSignalValid = FALSE;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_RetuneStartTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_ISDBT_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;


					DEMOD_PRINT( "^G^[DEMOD H14 GBB] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;

													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK :		/********************************************/
			{


			}
			break;
											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;

				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;
					pDemodInfo->unLockLongCount = 0;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
					if(pDemodInfo->bNotifiedUnlock == TRUE)
					{
						pDemodInfo->unLockLongCount++;
						if((pDemodInfo->unLockLongCount & 0x3) == 0x3)
						{
							if(RET_OK != DEMOD_M14B0_SoftwareReset())
							{
								DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							DEMOD_PRINT("[DEMOD H14 GBB](%s) DEMOD_SoftwareReset() !!!\n", __F__);
						}
						if(pDemodInfo->unLockLongCount > 250) pDemodInfo->unLockLongCount = 1;


					}

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

				if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.isdbt.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_ISDBT_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_M14B0_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
#if 0
								if(RET_OK != DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement(TRUE))
								{
									DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
#endif
								if(packetError < LX_DEMOD_PACKET_ERROR_THD)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_ISDBT_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

									if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

								}
								else
								{
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_ISDBT_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

										if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
										{
											DEMOD_PRINT( "^r^[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}


									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_M14B0_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_M14B0_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;


						DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : MSG_ISDBT_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H14 GBB] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

					}




				}
				else
				{
					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
					DEMOD_PRINT( "^R^[DEMOD] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ISDBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_ISDBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	unsigned int			powerOnElapsedTime;

	if(RET_OK != DEMOD_M14B0_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= _gM14B0GBB_FullISDBTPkerrThreshold)
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr++;
			DEMOD_PRINT( "^r^[DEMOD H14 GBB](%s) _gM14B0GBB_VQI_UnlockCount_full_packeterr = %d !!!\n", __F__,_gM14B0GBB_VQI_UnlockCount_full_packeterr);

			if(_gM14B0GBB_VQI_UnlockCount_full_packeterr == _gM14B0GBB_FullVQIPkerrCount)
			{
				DEMOD_PRINT( "^r^[DEMOD H14 GBB] oops !! (%s) Force to NOT LOCK !!\n", __F__);

				_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;


				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H14 GBB](%s) DEMOD_SoftwareReset() !!!\n", __F__);

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

			}

		}
		else
		{
			_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

			if(gM14B0GBB_PowerOnInit == 2)
			{

				powerOnElapsedTime = jiffies_to_msecs(jiffies) - gM14B0GBB_PowerOnInitStartTimeISDBT;

				DEMOD_PRINT("[DEMOD H14 GBB](%s:%d) powerOnElapsedTime = %d !!!\n", __F__, __L__,powerOnElapsedTime);

				if(powerOnElapsedTime >= 15000)
				{
					gM14B0GBB_PowerOnInit = 0;

					if (RET_OK != DEMOD_M14B0_ISDBT_Set_CrGearshEn(TRUE ))
					{
						DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_CrGearshEn() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
			}

			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 1)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock 	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
					pDemodInfo->signalStatus.bSignalValid = TRUE;

					DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] MONITORING FUNC : MSG_ISDBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 1)
			{
				pDemodInfo->lockCount = 0;
			}

#if 0
			if(RET_OK != DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_ISDBT_Set_PhaseNoiseEnhancement() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
#endif
			if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
			{
				DEMOD_PRINT( "^r^[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}

		}

	}

	else
	{
		pDemodInfo->lockCount = 0;
		_gM14B0GBB_VQI_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 2)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_M14B0_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				DEMOD_PRINT_ALWAYS( "[DEMOD H14 GBB] MONITORING FUNC : MSG_ISDBT_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 2)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if((pDemodInfo->unLockLongCount & 0x3) == 0x3)
			{
				if(RET_OK != DEMOD_M14B0_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14 GBB] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H14 GBB](%s) DEMOD_SoftwareReset() !!!\n", __F__);
			}
			if(pDemodInfo->unLockLongCount == 30)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD] MONITORING FUNC : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 30)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= TRUE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1500;


		}



	}


	return RET_OK;



}


#endif






#if 0
#endif



#if 0
#endif




