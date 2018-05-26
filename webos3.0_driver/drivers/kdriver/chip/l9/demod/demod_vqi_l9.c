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
#include "demod_reg_l9.h"
#include "demod_common_l9.h"
#include "demod_vqi_l9.h"

#if 0
#endif



//--------------------------------------------------------------------

//--------------------------------------------------------------------




#if 0
#endif

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Set_DefaultRegisterValue.
 * Initialize GBB VSB module .
 *
 * @param  NONE [IN] void
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_VSB_Set_DefaultRegisterValue(void)
{

	UINT8	phnboosten;
	UINT8	scsyncchken_v;
	UINT8	sceqchken_v;
	UINT8	acqfbf_2_0;
	UINT8	acqfdeq_3_0;
	UINT8	burstdeten;
	UINT8	burstwinen;
//	UINT8	chfast_7_0;
	UINT8	agcinsel;
//	UINT8	agcrefmode;
//	UINT8	agcen;

//	UINT8	agcref_15_8;
//	UINT8	agcref_7_0;

	UINT8	npncombw;
	UINT8	simfastenb;
	//UINT8 	adcsel1v;
	UINT8	srd1stchken;
	UINT8	qmden;
	UINT8	daften;

	UINT8	aiccfixfreq0_23_16;
	UINT8	aiccfixfreq0_15_8;
	UINT8	aiccfixfreq0_7_0;
	UINT8	aiccfixfreq1_23_16;
	UINT8	aiccfixfreq1_15_8;
	UINT8	aiccfixfreq1_7_0;
	UINT8	aiccfixfreq2_23_16;
	UINT8	aiccfixfreq2_15_8;
	UINT8	aiccfixfreq2_7_0;
	UINT8	agcoutpol;
	UINT8	autorptrsten;

	UINT8	fdeqstep_3_0;
	UINT8	msedynthd_1_0;
	UINT8	msestaticthd_1_0;

	UINT8	fbfstep_2_0;
	UINT8	phderotbwsel_3_0;

	UINT8	staticacqfbf_2_0;
	UINT8	staticacqfdeq_3_0;

	UINT8	statictrackfbf_2_0;
	UINT8	statictrackfdeq_3_0;

	UINT8	crptmsesela_v_2_0;
//	UINT8	burstwinmsediff_15_8;

	UINT8	adcclkpllsel;
	UINT8	pllsetauto;
	UINT8	pllctrl_13_8;
	UINT8	pllctrl_7_0;
	UINT8	stdopdettmode_2_0;
	UINT8	stdopdetcmode_1_0;
	UINT8	samclkauto;
	UINT8	samfreq_21_16;

	UINT8	agciffixb;
	UINT8	agcrffixb;
	UINT8	aiccopmode0_1_0;
	UINT8	aiccopmode1_1_0;
	UINT8	aiccopmode2_1_0;	
	UINT8	dagc2ndrefmode;

	UINT8	srdwaittime_7_0;
	UINT8	scneverlock1shoten;
	UINT8	sampling4xfen;


	UINT8	dagc2ndgearshen=0;
	UINT8	dagc2ndbw_3_0=9;


	UINT8	staticgo2gdcnt_3_0;
	UINT8	staticgo2badcnt_3_0;

	UINT8	badsinglestatictrackfdeq_3_0;
	UINT8	badstatictrackfdeq_3_0;
	UINT8	badphderotbwsel_3_0;
	UINT8	badstatictrackfbf_2_0;

	UINT8	vconfsel_1_0;
	UINT8	cirlockhold;

	UINT8	tpclkouten;
	UINT8	tpifen;
	UINT8	tpouten;

	UINT8	sclockcheck_v_23_16;
	UINT8	sclockcheck_v_15_8;
	UINT8	sclockcheck_v_7_0;
	UINT8	crdeltafcp2x_v_4_0;
	UINT8	crdeltafcp1x_v_4_0;
	UINT8	crdeltafcm2x_v_4_0;
	UINT8	crdeltafcm1x_v_4_0;
	UINT8	phderoten;
	UINT8	phderothold;
	UINT8	samcnormbp_v;

//	UINT8	thmainphnon_7_0;
//	UINT8	thmainphnoff_7_0;

	UINT8	trkexit_1_0;
	UINT8 oncetime = 0;


	DEMOD_PRINT("^p^[KADP_DEMOD DTV] ####chungiii check !!! %s =====\n",__F__);


	
	samcnormbp_v = 0;
	Sync_RdFL(sampling_ctrl_v0); // regAddr = 0x1047
	Sync_Wr01(sampling_ctrl_v0,samcnormbp_v, samcnormbp_v);
	Sync_WrFL(sampling_ctrl_v0);

	// Static echo control routine init
	acqfbf_2_0= 0x0;
	acqfdeq_3_0 = 0x1;
	EQ_V_RdFL(eq_control7); // regAddr = 0x2123[2:0] : "000" Registers for Equalizer controller
	EQ_V_Wr02(eq_control7,acqfbf_2_0, acqfbf_2_0,acqfdeq_3_0,acqfdeq_3_0);
	EQ_V_WrFL(eq_control7);

	staticacqfbf_2_0 = 0;
	staticacqfdeq_3_0 = 1;
	EQ_V_RdFL(eq_control8); // regAddr = 0x2124[2:0] : "000" Registers for Equalizer controller
	EQ_V_Wr02(eq_control8,staticacqfbf_2_0, staticacqfbf_2_0,staticacqfdeq_3_0,staticacqfdeq_3_0);
	EQ_V_WrFL(eq_control8);

	statictrackfbf_2_0 =  2;//			  : 3,	//	0: 2
	statictrackfdeq_3_0 = 1;//			  : 4;	//	4: 7
	EQ_V_RdFL(eq_control9); // regAddr = 0x2125[2:0] : "000" Registers for Equalizer controller
	EQ_V_Wr02(eq_control9,statictrackfbf_2_0, statictrackfbf_2_0,statictrackfdeq_3_0,statictrackfdeq_3_0);
	EQ_V_WrFL(eq_control9);



	crdeltafcp2x_v_4_0 = 0x6;
	Sync_RdFL(carrier_ctrl_v10); // regAddr = 0x106B
	Sync_Wr01(carrier_ctrl_v10,crdeltafcp2x_v_4_0, crdeltafcp2x_v_4_0);
	Sync_WrFL(carrier_ctrl_v10);

	crdeltafcp1x_v_4_0 = 0x6;
	Sync_RdFL(carrier_ctrl_v11); // regAddr = 0x106C
	Sync_Wr01(carrier_ctrl_v11,crdeltafcp1x_v_4_0, crdeltafcp1x_v_4_0);
	Sync_WrFL(carrier_ctrl_v11);


	crdeltafcm2x_v_4_0 = 0x6;
	Sync_RdFL(carrier_ctrl_v12); // regAddr = 0x106D
	Sync_Wr01(carrier_ctrl_v12,crdeltafcm2x_v_4_0, crdeltafcm2x_v_4_0);
	Sync_WrFL(carrier_ctrl_v12);

	crdeltafcm1x_v_4_0 = 0x6;
	Sync_RdFL(carrier_ctrl_v13); // regAddr = 0x106E
	Sync_Wr01(carrier_ctrl_v13,crdeltafcm1x_v_4_0, crdeltafcm1x_v_4_0);
	Sync_WrFL(carrier_ctrl_v13);



	sclockcheck_v_23_16 = 0xF;
	Sync_RdFL(v_top_ctrl3); // regAddr = 0x1027
	Sync_Wr01(v_top_ctrl3,sclockcheck_v_23_16, sclockcheck_v_23_16);
	Sync_WrFL(v_top_ctrl3);


	sclockcheck_v_15_8 = 0x42;
	Sync_RdFL(v_top_ctrl4); // regAddr = 0x1028
	Sync_Wr01(v_top_ctrl4,sclockcheck_v_15_8, sclockcheck_v_15_8);
	Sync_WrFL(v_top_ctrl4);

	sclockcheck_v_7_0 = 0x3C;
	Sync_RdFL(v_top_ctrl5); // regAddr = 0x1029
	Sync_Wr01(v_top_ctrl5,sclockcheck_v_7_0, sclockcheck_v_7_0);
	Sync_WrFL(v_top_ctrl3);



	scneverlock1shoten = 1;
	Sync_RdFL(daft_ctrl0); // regAddr = 0x1000[7] : 0->1
	Sync_Wr01(daft_ctrl0,scneverlock1shoten, scneverlock1shoten);
	Sync_WrFL(daft_ctrl0);

	//srdwaittime_7_0 = 0x32;
	srdwaittime_7_0 = 0x28;
	Sync_RdFL(srd_ctrl9); // regAddr = 0x1019 : 0x32
	Sync_Wr01(srd_ctrl9,srdwaittime_7_0, srdwaittime_7_0);
	Sync_WrFL(srd_ctrl9);


	simfastenb = 1;
	autorptrsten = 0;
	Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
	Common_Wr02(common_ctrl1, simfastenb, simfastenb, autorptrsten, autorptrsten);
	Common_WrFL(common_ctrl1);
	//SIMFASTENB: Chip operation mode control
	// 0 : simluation mode(for chip test only),  1 : normal operation mode
	//This signal should be set to '1' at the start of operation


	adcclkpllsel = 0;
	Common_RdFL(common_ctrl4);	 //regAddr = 0x0004
	Common_Wr01(common_ctrl4, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);


	pllsetauto = 0;
	pllctrl_13_8 = 0x25;
	Common_RdFL(common_ctrl5);	 //regAddr = 0x0005
	Common_Wr02(common_ctrl5, pllsetauto, pllsetauto, pllctrl_13_8, pllctrl_13_8);
	Common_WrFL(common_ctrl5);


	pllctrl_7_0 = 0x64;
	Common_RdFL(common_ctrl6);	 //regAddr = 0x0006
	Common_Wr01(common_ctrl6, pllctrl_7_0, pllctrl_7_0);
	Common_WrFL(common_ctrl6);


	stdopdettmode_2_0 = 0x3;
	stdopdetcmode_1_0 = 0;
	Common_RdFL(common_ctrl8);	 //regAddr = 0x0009
	Common_Wr02(common_ctrl8, stdopdetcmode_1_0, stdopdetcmode_1_0,stdopdettmode_2_0,stdopdettmode_2_0);
	Common_WrFL(common_ctrl8);


	samclkauto = 0;
	sampling4xfen = 0;
	samfreq_21_16 = 0x18;
	Common_RdFL(common_ctrl11);  //regAddr = 0x000d,
	Common_Wr03(common_ctrl11, samclkauto, samclkauto, sampling4xfen, sampling4xfen,samfreq_21_16,samfreq_21_16);
	Common_WrFL(common_ctrl11);


#if 0
	agcinsel = 1;
	Common_RdFL(agc_ctrl0); //regAddr = 0x0012, AGC control register
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
	//agcinsel : Selection of input signal for gain error calculation
	//	0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
	// 1 : use incoming signal This signal should be set to '1' at the start of operation
#endif

	agcinsel = 1;
	Common_RdFL(agc_ctrl0); //regAddr = 0x0012, AGC control register
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
	//agcinsel : Selection of input signal for gain error calculation
	//	0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
	// 1 : use incoming signal This signal should be set to '1' at the start of operation

	agciffixb=0;
	agcrffixb=0;
	Common_RdFL(agc_ctrl1);  //regAddr = 0x0013,
	Common_Wr02(agc_ctrl1, agciffixb, agciffixb, agcrffixb, agcrffixb);
	Common_WrFL(agc_ctrl1);


	aiccopmode0_1_0 = 0;
	aiccopmode1_1_0 = 0;
	aiccopmode2_1_0 = 0;
	Common_RdFL(mdp_ctrl8); //regAddr = 0x0024,
	Common_Wr03(mdp_ctrl8,aiccopmode0_1_0, aiccopmode0_1_0, aiccopmode1_1_0, aiccopmode1_1_0, aiccopmode2_1_0, aiccopmode2_1_0 );
	Common_WrFL(mdp_ctrl8);
	//AICCOPMODE0[1:0] :election of 1st AICC(continuous wave interference or analog TV signal) rejection function
	//This register should be set to 0x00 at DVB-C and QAM


	dagc2ndrefmode = 0;
	dagc2ndgearshen=0;
	dagc2ndbw_3_0=0x9;
	Common_RdFL(mdp_ctrl24);	 //regAddr = 0x0035,
	Common_Wr03(mdp_ctrl24, dagc2ndrefmode, dagc2ndrefmode,dagc2ndbw_3_0,dagc2ndbw_3_0,dagc2ndgearshen,dagc2ndgearshen);
	Common_WrFL(mdp_ctrl24);




	srd1stchken = 1;
	qmden = 0;
	daften = 0;
	Common_RdFL(common_ctrl9);	 //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Wr03(common_ctrl9, srd1stchken, srd1stchken, qmden, qmden, daften, daften);
	Common_WrFL(common_ctrl9);
	//SRD1STCHKEN: Control of fast start for demodulation with pre-defined symbol rate
				//0 : disable, 1 : enable, This signal should be set to '1' at the start of operation
	// DAFTEN : Digital Automatic Frequency Tuning(DAFT) function control
				// This is only valid for cable standard(DVB-C,QAM) (0 : disable, 1 : enable)
	//QMDEN : QAM Mode Detection(QMD) function control
				//This is only valid for cable standard(DVB-C,QAM) (0 : disable, 1 : enable)


#if 0
	agcen = 1;
	agcrefmode = 1;
	agcinsel = 1;
	Common_RdFL(agc_ctrl0); //regAddr = 0x0012, AGC control register
	Common_Wr03(agc_ctrl0, agcen, agcen, agcrefmode, agcrefmode, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
	//agcinsel : Selection of input signal for gain error calculation
	//	0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
	// 1 : use incoming signal This signal should be set to '1' at the start of operation


 // set manually agc ref vlaue[15:8]
	agcref_15_8 = 0x1c;
	Common_RdFL(agc_ctrl3);   //regAddr = 0x0015
	Common_Wr01(agc_ctrl3, agcref_15_8, agcref_15_8);
	Common_WrFL(agc_ctrl3);
// set manually agc ref vlaue[7:0]
	agcref_7_0 = 0x00;
	Common_RdFL(agc_ctrl4);   //regAddr = 0x0016
	Common_Wr01(agc_ctrl4, agcref_7_0, agcref_7_0);
	Common_WrFL(agc_ctrl4);
#endif


	agcoutpol = 1;
	Common_RdFL(common_ctrl3);	 //regAddr = 0x0003
	Common_Wr01(common_ctrl3, agcoutpol, agcoutpol);
	Common_WrFL(common_ctrl3);

	/*
	Selection of the gain control signal output polarity
	0 : low means gain up
	1 : high means gain up
	*/



	// Phase control routine init
	phnboosten = 0;
	EQ_V_RdFL(eq_control12); // regAddr = 0x2128[1] : '1'->'0' ,Registers for Equalizer controller
	EQ_V_Wr01(eq_control12,phnboosten, phnboosten );
	EQ_V_WrFL(eq_control12);


	phderothold = 0;
	phderoten = 1;
	EQ_V_RdFL(phase_derotator); // regAddr = 0x213b[5] :
	EQ_V_Wr02(phase_derotator,phderothold, phderothold,phderoten,phderoten);
	EQ_V_WrFL(phase_derotator);


	// AFC hopping control routine init
	scsyncchken_v = 1;
	sceqchken_v= 1;
	Sync_RdFL(v_top_ctrl0); // regAddr = 0x1024[4:3] : "11" ,VSB Top control register
	Sync_Wr02(v_top_ctrl0,scsyncchken_v, scsyncchken_v, sceqchken_v, sceqchken_v);
	Sync_WrFL(v_top_ctrl0);




	// Fast dynamic echo control routine init
//	chfast_7_0= 0x40;
//	EQ_V_RdFL(eq_control5); // regAddr = 0x2121[7:0] : 0x40 ,Registers for Equalizer controller
//	EQ_V_Wr01(eq_control5,chfast_7_0, chfast_7_0);
//	EQ_V_WrFL(eq_control5);


	// Burst noise control routine init
	burstdeten= 0x00;
	burstwinen = 0x00;
	EQ_V_RdFL(burst_detect0); // regAddr = 0x2148[7:6] : "00" ,Registers to control Burst Noise Detector
	EQ_V_Wr02(burst_detect0,burstdeten, burstdeten, burstwinen, burstwinen);
	EQ_V_WrFL(burst_detect0);



	//addding init register
	npncombw = 1;
	EQ_V_RdFL(comb0); // regAddr = 0x213e[7] : '0'->'1' ,NTSC is not automatically detected
	EQ_V_Wr01(comb0,npncombw, npncombw );
	EQ_V_WrFL(comb0);

	aiccfixfreq0_23_16 = 0x22;
	Common_RdFL(mdp_ctrl18); //regAddr = 0x002e, MDP control register
	Common_Wr01(mdp_ctrl18, aiccfixfreq0_23_16, aiccfixfreq0_23_16);
	Common_WrFL(mdp_ctrl18);

	aiccfixfreq0_15_8 = 0x0;
	Common_RdFL(mdp_ctrl19); //regAddr = 0x002f, MDP control register
	Common_Wr01(mdp_ctrl19, aiccfixfreq0_15_8, aiccfixfreq0_15_8);
	Common_WrFL(mdp_ctrl19);


	aiccfixfreq0_7_0 = 0x0;
	Common_RdFL(mdp_ctrl20); //regAddr = 0x0030, MDP control register
	Common_Wr01(mdp_ctrl20, aiccfixfreq0_7_0, aiccfixfreq0_7_0);
	Common_WrFL(mdp_ctrl20);


	aiccfixfreq1_23_16 = 0x3e;
	Common_RdFL(mdp_ctrl15); //regAddr = 0x002b, MDP control register
	Common_Wr01(mdp_ctrl15, aiccfixfreq1_23_16, aiccfixfreq1_23_16);
	Common_WrFL(mdp_ctrl15);

	aiccfixfreq1_15_8 = 0xcc;
	Common_RdFL(mdp_ctrl16); //regAddr = 0x002c, MDP control register
	Common_Wr01(mdp_ctrl16, aiccfixfreq1_15_8, aiccfixfreq1_15_8);
	Common_WrFL(mdp_ctrl16);

	aiccfixfreq1_7_0 = 0xf4;
	Common_RdFL(mdp_ctrl17); //regAddr = 0x002d, MDP control register
	Common_Wr01(mdp_ctrl17, aiccfixfreq1_7_0, aiccfixfreq1_7_0);
	Common_WrFL(mdp_ctrl17);

	aiccfixfreq2_23_16 = 0x46;
	Common_RdFL(mdp_ctrl12); //regAddr = 0x0028, MDP control register
	Common_Wr01(mdp_ctrl12, aiccfixfreq2_23_16, aiccfixfreq2_23_16);
	Common_WrFL(mdp_ctrl12);

	aiccfixfreq2_15_8 = 0x0;
	Common_RdFL(mdp_ctrl13); //regAddr = 0x0029, MDP control register
	Common_Wr01(mdp_ctrl13, aiccfixfreq2_15_8, aiccfixfreq2_15_8);
	Common_WrFL(mdp_ctrl13);

	aiccfixfreq2_7_0 = 0x0;
	Common_RdFL(mdp_ctrl14); //regAddr = 0x002a, MDP control register
	Common_Wr01(mdp_ctrl14, aiccfixfreq2_7_0, aiccfixfreq2_7_0);
	Common_WrFL(mdp_ctrl14);

	fdeqstep_3_0 = 7;		  //		  : 4,	 0: 3
	msedynthd_1_0 = 0;			   //16dB		   : 2,  4: 5
	msestaticthd_1_0 = 3;		   //		   : 2;  6: 7
	EQ_V_RdFL(eq_control1); // regAddr = 0x211d
	EQ_V_Wr03(eq_control1,fdeqstep_3_0, fdeqstep_3_0 ,msedynthd_1_0, msedynthd_1_0, msestaticthd_1_0, msestaticthd_1_0);
	EQ_V_WrFL(eq_control1);
	//MSESTATICTHD[1:0]:  Select the required MSE for static channel.(00 : 14dB, 01 : 15dB, 10 : 16dB, 11 : 17dB)
	//MSEDYNTHD[1:0]: Select the required MSE for dynamic channel.(00 : 16dB, 01 : 17dB, 10 : 18dB, 11 : 19dB)
	//FDEQSTEP[3:0]: Select the step size of Frequency Domain Equalizer(FDEQ). This register is activated when EQAUTO is zero.
	// 0000 : x1/16, 0001 : x1/8, 0010 : x1/4, 0011 : x1/2, 100 : x1, 0101 : x1.25, 0110 : x1.5, 0111 : x2, 1000 : x2.25, 1001 : x2.5
	// 1010 : x2.75, 1011 : x3, 100 : x3.25, 1101 : x3.5, 1110 : x3.75, 1111 : x4

	fbfstep_2_0 = 0;
	EQ_V_RdFL(eq_control2); // regAddr = 0x211e
	EQ_V_Wr01(eq_control2,fbfstep_2_0, fbfstep_2_0 );
	EQ_V_WrFL(eq_control2);

	phderotbwsel_3_0 = 0xC;
	EQ_V_RdFL(eq_control3); // regAddr = 0x211f
	EQ_V_Wr01(eq_control3,phderotbwsel_3_0, phderotbwsel_3_0 );
	EQ_V_WrFL(eq_control3);



	crptmsesela_v_2_0 = 3;
	Sync_RdFL(carrier_ctrl_v23); // regAddr = 0x1078[3] : 0
	Sync_Wr01(carrier_ctrl_v23,crptmsesela_v_2_0, crptmsesela_v_2_0);
	Sync_WrFL(carrier_ctrl_v23);


//	autorptrsten = 1;
//	Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
//	Common_Wr01(common_ctrl1, autorptrsten, autorptrsten);
//	Common_WrFL(common_ctrl1);

//	burstwinmsediff_15_8 = 1;
//	EQ_V_RdFL(burst_detect12); // regAddr = 0x2154[7:0] : 1
//	EQ_V_Wr01(burst_detect12,burstwinmsediff_15_8, burstwinmsediff_15_8);
//	EQ_V_WrFL(burst_detect12);



	staticgo2gdcnt_3_0=8;
	staticgo2badcnt_3_0=0xE;
	EQ_V_RdFL(eq_control16); // regAddr = 0x212c
	EQ_V_Wr02(eq_control16,staticgo2gdcnt_3_0, staticgo2gdcnt_3_0,staticgo2badcnt_3_0,staticgo2badcnt_3_0);
	EQ_V_WrFL(eq_control16);

#if 1
	badsinglestatictrackfdeq_3_0=1;
	badstatictrackfdeq_3_0=1;
	EQ_V_RdFL(eq_control10); // regAddr = 0x2126
	EQ_V_Wr02(eq_control10,badsinglestatictrackfdeq_3_0, badsinglestatictrackfdeq_3_0,badstatictrackfdeq_3_0,badstatictrackfdeq_3_0);
	EQ_V_WrFL(eq_control10);

	badstatictrackfbf_2_0 = 1;
	EQ_V_RdFL(eq_control11); // regAddr = 0x2127
	EQ_V_Wr01(eq_control11,badstatictrackfbf_2_0,badstatictrackfbf_2_0);
	EQ_V_WrFL(eq_control11);



	badphderotbwsel_3_0=1;
	EQ_V_RdFL(eeq_control15); // regAddr = 0x212b
	EQ_V_Wr01(eeq_control15,badphderotbwsel_3_0, badphderotbwsel_3_0);
	EQ_V_WrFL(eeq_control15);

#endif



	vconfsel_1_0=0;
	EQ_V_RdFL(cir0); // regAddr = 0x2100
	EQ_V_Wr01(cir0,vconfsel_1_0, vconfsel_1_0);
	EQ_V_WrFL(cir0);

	cirlockhold=1;
	EQ_V_RdFL(cir26); // regAddr = 0x211a
	EQ_V_Wr01(cir26,cirlockhold, cirlockhold);
	EQ_V_WrFL(cir26);


	tpclkouten=1;
	tpifen	=1;
	tpouten=1;
	Common_RdFL(tp_ctrl0);	//regAddr = 0x0070,
	Common_Wr03(tp_ctrl0, tpclkouten,tpclkouten,tpifen, tpifen, tpouten,tpouten);
	Common_WrFL(tp_ctrl0);


	trkexit_1_0 = 1;
	EQ_V_RdFL(eq_control18); // regAddr = 0x212e:
	EQ_V_Wr01(eq_control18,trkexit_1_0, trkexit_1_0);
	EQ_V_WrFL(eq_control18);


if(oncetime)
{
	UINT16	regAddr;
	UINT8	vqData=0;

	regAddr=0x1000;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x1019;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0001;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0004;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0005;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0006;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x0009;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x000d;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0012;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x0013;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0024;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0035;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x000a;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0003;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2128;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x1024;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2121;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x2148;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x213e;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x002e;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x002f;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0030;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x002b;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x002c;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x002d;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0028;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0029;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x002a;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x211d;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x211e;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x211f;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2123;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x2124;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2125;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x1078;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2154;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x212c;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);


	regAddr=0x2126;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x212b;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x2100;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x211a;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);

	regAddr=0x0070;
	if (RET_OK	!= DEMOD_I2C_Read((UINT16)regAddr , 1,  &vqData))
	{
		DEMOD_PRINT( "[debug] FAIL (%s:%d) DEMOD_I2C_Read() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("\n ### chungiii   address = 0x%x, value = 0x%x ", regAddr,vqData);
	oncetime = FALSE;

}

//	KADP_OSA_SleepMsec(500);

	return RET_OK;

	
	
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_CochannelExist.
 * Get the informantion if co-channel(NTSC) signal exists or not on VSB mode.
 *
 * @param  *pCochannel [OUT] BOOLEAN - exists /doesn't exist.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_VSB_CochannelExist(BOOLEAN *pCochannel)
{

	*pCochannel = FALSE;
	return RET_OK;


}





/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_PreMonitor.
 * Set registers against  various noise conditions before  monitoring.
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/


int DEMOD_L9_VSB_PreMonitor(void)
{


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Monitor.
 * Monitoring
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_VSB_Monitor(DEMOD_CTX_T *pDemodInfo)
{


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Get_MSEdynStatus.
 * Get the flag whether MSE would be under operation limit (snr > 16dB : TRUE)
 *
 * @param  *pbEnable [OUT] BOOLEAN - get signal noise ratio
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_VSB_Get_MSEdynStatus(BOOLEAN *pbEnable)
{
	UINT8 	msedyn;
	UINT8	tpmainstrongphn;

	DEMOD_PRINT("[DEMOD L9] == @ %s!!!\n",__F__);

	EQ_V_RdFL(eq_control23); // regAddr = 0x2190[7] :
	EQ_V_Rd01(eq_control23,msedyn, msedyn);

	EQ_V_RdFL(cir31); // regAddr = 0x2184[7:6] :
	EQ_V_Rd01(cir31,tpmainstrongphn, tpmainstrongphn);

	if((msedyn == 0) || (tpmainstrongphn==0))
	{
		*pbEnable = FALSE;
	}
	else
	{
		*pbEnable = TRUE;
	}
	DEMOD_PRINT("[DEMOD L9] MSEdynStatus is %s!!\n",(*pbEnable == TRUE) ? "Enable" : "Disable");
	DEMOD_PRINT("[DEMOD L9] tpmainstrongphn is [%d]!!\n",tpmainstrongphn);

	return RET_OK;


}





/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Get_TotalInfo
 * Get all information on VSB for debug
 *
 * @param NONE [IN].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_VSB_Get_TotalInfo( void)
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

	if (RET_OK  != DEMOD_L9_Get_NeverLockStatus(LX_DEMOD_ATSC, &chScanStatus))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_NeverLockStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_GetOperMode(&operMode))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_IFAGC(&ifAGC))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_IFAGC() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_Lock(LX_DEMOD_CARR_LOCK, &carrLock))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_Lock(&syncLock))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_CarrierFreqOffset( &offsetFrq))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_VSB_CochannelExist( &cochannel))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_VSB_CochannelExist() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

 	//MSE

	if (RET_OK != DEMOD_L9_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	if (RET_OK  != DEMOD_L9_GetCrAcquisionTime(&cracquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_GetTrAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_VABER(&viterbiBer))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_VABER() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	DEMOD_PRINT("[DEMOD L9 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	DEMOD_PRINT("[DEMOD L9 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													 (operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													 (operMode == LX_DEMOD_VSB_MODE) ? "VSB" :  "unknown" );

	DEMOD_PRINT("[DEMOD L9 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	DEMOD_PRINT("[DEMOD L9 DTV] Cochannel (NTSC) is  %s !!!\n", (cochannel == TRUE) ? " existent" : " Non-existent");
	DEMOD_PRINT("[DEMOD L9 DTV] Carrier LOCK = %s (%d ms)!!!\n",(carrLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", cracquisionTimeMs);
	DEMOD_PRINT("[DEMOD L9 DTV] Sync LOCK = %s !!!\n", (syncLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD L9 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);

	DEMOD_PRINT("[DEMOD L9] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);
	DEMOD_PRINT("[DEMOD L9 DTV] Viterbi BER = %d !!!\n",viterbiBer);

#ifndef  DEMOD_PRINT
	printk("[DEMOD L9 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	printk("[DEMOD L9 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													 (operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													 (operMode == LX_DEMOD_VSB_MODE) ? "VSB" :	"unknown" );

	printk("[DEMOD L9 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	printk("[DEMOD L9 DTV] Cochannel (NTSC) is  %s !!!\n", (cochannel == TRUE) ? " existent" : " Non-existent");
	printk("[DEMOD L9 DTV] Carrier LOCK = %s (%d ms)!!!\n",(carrLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", cracquisionTimeMs);
	printk("[DEMOD L9 DTV] Sync LOCK = %s !!!\n", (syncLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD L9 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);

	printk("[DEMOD L9] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD L9 DTV] Viterbi BER = %d !!!\n",viterbiBer);



#endif
	return RET_OK;

#endif

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_VSB_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime;


	if(pDemodInfo->bSetParams)
	{
		switch (pDemodInfo->controlState)
		{
												/***************************************************/
												/***** Configure demod  for RETUNE. *****/
			case LX_DEMOD_RETUNE_RF:			/***************************************************/
				{

					if(RET_OK != DEMOD_L9_VSB_Set_DefaultRegisterValue())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_VSB_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					pDemodInfo->controlState = LX_DEMOD_TUNE_START;
					DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
					g_RetuneStartTime = jiffies_to_msecs(jiffies);

				}

				break;

												/***************************************************/
												/***** Configure demod for tune start. *****/
			case LX_DEMOD_TUNE_START:			/***************************************************/
				{
					if(RET_OK != DEMOD_L9_SoftwareReset())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_SoftwareReset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.vsb.tuneMode)
					{

						if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");
					}
					else
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

						DEMOD_PRINT( "^c^[DEMOD L9] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
						return RET_OK;
					}


					if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					}
					else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
						pDemodInfo->bNotifiedUnlock	= TRUE;
						pDemodInfo->bNotifiedLock	= FALSE;

						pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


						currentTime = jiffies_to_msecs(jiffies);
						elapsedTime = currentTime - g_NewTuneReqeustTime;

						DEMOD_PRINT("\nMSG_ATSC_FRONT_END_NOT_LOCKED..\n") ;
						DEMOD_PRINT("Elapse time is %d msec\n", elapsedTime) ;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


					}
					else
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{
						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

						sigStablestartTime = jiffies_to_msecs(jiffies);

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
					}
					else
					{
						if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_CARR_LOCK, &lockState))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(LX_DEMOD_LOCKED == lockState)
						{

							notifyinglockTimeOut = LX_DEMOD_LOCK_DEALY_1100 + LX_DEMOD_LOCK_DEALY_800;
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
					SINT32							freqOffset;
					unsigned int 					elapsedTime;
					UINT32 							packetError;

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{

						if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.vsb.tuneMode)
						{
							elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

							if(elapsedTime < LX_DEMOD_VSB_SIG_STABLE_WAIT_TIMEOUT)
							{
								break;
							}
							else
							{
								if(RET_OK != DEMOD_L9_VSB_PreMonitor())
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_VSB_PreMonitor() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}

								if(RET_OK != DEMOD_L9_Get_Packet_Error(&packetError))
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Packet_Error() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
								else
								{
									if(packetError < LX_DEMOD_PACKET_ERROR_THD)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock	= FALSE;

										if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

									}
									else
									{
										elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
										if(elapsedTime > LX_DEMOD_LOCK_DEALY_200)
										{
											pDemodInfo->bNotifiedLock	= TRUE;
											pDemodInfo->bNotifiedUnlock = FALSE;

											if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
											{
												DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
												DEMOD_RETURN_FAIL(__F__);
											}
											pDemodInfo->freqOffset = freqOffset;

											pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

											elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

										}

									}

								}

							}
						}
						else
						{
							pDemodInfo->bNotifiedLock	= TRUE;
							pDemodInfo->bNotifiedUnlock = FALSE;

							if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
							{
								DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							pDemodInfo->freqOffset = freqOffset;

							pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

							elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);


						}

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}


					}
					else
					{
						pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

					}


		   		}
				break;

			case LX_DEMOD_SIGNAL_LOCKED:	break;

			default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

		}
	}

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_VSB_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_VSB_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;

		}
		if(pDemodInfo->lockCount == 4)
		{
			pDemodInfo->lockStatus = LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock = TRUE;
			if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
				pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		}
		if(pDemodInfo->lockCount > 4)
		{
			pDemodInfo->lockCount = 0;
		}


	}
	else
	{
		pDemodInfo->lockCount = 0;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;

			}
			if(pDemodInfo->unLockCount > 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 200)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount > 200)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1100;


		}



	}


	return RET_OK;



}




#if 0
#endif

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_SoftwareResetFEC.
 * Reset QAM FEC module.
 *
 * @param [in] NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_SoftwareResetFEC(void)
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
 * DEMOD_L9_QAM_Monitor.
 * Monitoring
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_QAM_Monitor(void)
{

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_ModeAutoDetection.
 * Enable/Disable the detection of QAM mode
 *
 * @param  bEnable [IN] - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_ModeAutoDetection(BOOLEAN bEnable)
{


	UINT8	stdopdettmode_2_0;
	UINT8	stdopdetcmode_1_0;


	stdopdettmode_2_0 = 0x0;
	stdopdetcmode_1_0 = 0x2;

	stdopdetcmode_1_0 = (bEnable == TRUE)?(0x2):(0x1);
	Common_RdFL(common_ctrl8);   //regAddr = 0x0009
	Common_Wr02(common_ctrl8, stdopdettmode_2_0, stdopdettmode_2_0,stdopdetcmode_1_0,stdopdetcmode_1_0);
	Common_WrFL(common_ctrl8);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_64Mode.
 * Set the QAM mode to QAM64 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_64Mode(void)
{


	UINT8	qmdqammode0_2_0;


	/* mode 64 QAM */

	// manually set to 64QAM

	qmdqammode0_2_0 = 0x0;      //QMDQAMMODE0[2] 0 means 64QAM, 1 means 256QAM
	Sync_Wr01(qmd_ctrl4, qmdqammode0_2_0, qmdqammode0_2_0);     //regAddr = 0x1020  QMD control register
	Sync_WrFL(qmd_ctrl4);



	return RET_OK;


}
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_256Mode.
 * Set the QAM mode to QAM256 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_256Mode(void)
{

	UINT8	qmdqammode0_2_0;


	/* mode 256QAM */

	// manually set to 256QAM

	qmdqammode0_2_0 = 0x4;      //QMDQAMMODE0[2] 0 means 64QAM, 1 means 256QAM
	Sync_Wr01(qmd_ctrl4, qmdqammode0_2_0, qmdqammode0_2_0);     //regAddr = 0x1020  QMD control register
	Sync_WrFL(qmd_ctrl4);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_Set_DefaultRegisterValue.
 * Initialize LG1150 GBB for QAM mode.
 *
 * @param  NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_Set_DefaultRegisterValue(void)
{
	UINT8	daften;
	UINT8	srd1stchken;
	UINT8	agcinsel;
	UINT8	aiccopmode0_1_0;
	UINT8	srdwaittime_7_0;


	UINT8	adcclkpllsel;
	UINT8	pllsetauto;
	UINT8	pllctrl_13_8;
	UINT8	pllctrl_7_0;
	UINT8	stdopdetcmode_1_0;
	UINT8	stdopdettmode_2_0;


	UINT8	samclkauto;
	UINT8	sampling4xfen;
	UINT8	samfreq_21_16;
	UINT8	agciffixb;
	UINT8	agcrffixb;
	UINT8	dagc2ndrefmode;
	UINT8	qmden;
	UINT8	scneverlock1shoten;


	UINT8	tpclkouten;
	UINT8	tpifen;
	UINT8	tpouten;

//	UINT8	tpifbw_3_0;
//	UINT8	tpifctrl_3_0;


	UINT8	sclockcheck_v_23_16;
	UINT8	sclockcheck_v_15_8;
	UINT8	sclockcheck_v_7_0;
	UINT8	crdeltafcp2x_v_4_0;
	UINT8	crdeltafcp1x_v_4_0;
	UINT8	crdeltafcm2x_v_4_0;
	UINT8	crdeltafcm1x_v_4_0;
//	UINT8	selfsyncfinddetth_cqs_7_0;
//	UINT8	selfsyncfindccth_cqs_7_0;

	UINT8	framelockmode_cqs_1_0;
//	UINT8	feclkbrk_cqs;
//	UINT8	selfsyncfinden_cqs;



	DEMOD_PRINT("^p^[KADP_DEMOD DTV] ####chungiii check !!! %s =====\n",__F__);

#if 0



	UINT8	simfastenb;
	UINT8	adcsel1v;
	UINT8	agcinsel;
	UINT8	aiccopmode0_1_0;

	simfastenb = 1;
	Common_RdFL(common_ctrl1);	 //regAddr = 0x0002, COMMON_CTRL[1]
	Common_Wr01(common_ctrl1, simfastenb, simfastenb);
	Common_WrFL(common_ctrl1);
	//SIMFASTENB: Chip operation mode control
	// 0 : simluation mode(for chip test only),  1 : normal operation mode
	//This signal should be set to '1' at the start of operation

	adcsel1v = 1;
	Common_RdFL(common_ctrl4);	 //regAddr = 0x0004, COMMON_CTRL[4]
	Common_Wr01(common_ctrl4, adcsel1v, adcsel1v);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	agcinsel = 1;
	Common_RdFL(agc_ctrl0); //regAddr = 0x0012, AGC control register
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
	//agcinsel : Selection of input signal for gain error calculation
	//	0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
	// 1 : use incoming signal This signal should be set to '1' at the start of operation

	aiccopmode0_1_0 = 0;
	Common_RdFL(mdp_ctrl8);   //regAddr = 0x0024, MDP_CTRL[8]
	Common_Wr01(mdp_ctrl8, aiccopmode0_1_0, aiccopmode0_1_0);
	Common_WrFL(mdp_ctrl8);
	//AICCOPMODE0[1:0] :election of 1st AICC(continuous wave interference or analog TV signal) rejection function
	//This register should be set to 0x00 at DVB-C and QAM


#endif


//	selfsyncfinden_cqs=0;
//	FEC_RdFL(fec_set5); // regAddr = 0x302b
//	FEC_Wr01(fec_set5,selfsyncfinden_cqs,selfsyncfinden_cqs);
//	FEC_WrFL(fec_set5);



	framelockmode_cqs_1_0 = 3;
//	feclkbrk_cqs=0;
	FEC_RdFL(fec_set10);	// regAddr = 0x3032
	FEC_Wr01(fec_set10,framelockmode_cqs_1_0,framelockmode_cqs_1_0);
	FEC_WrFL(fec_set10);

	scneverlock1shoten = 1;
	Sync_RdFL(daft_ctrl0); // regAddr = 0x1000[7] : 0->1
	Sync_Wr01(daft_ctrl0,scneverlock1shoten, scneverlock1shoten);
	Sync_WrFL(daft_ctrl0);


	srdwaittime_7_0 = 0x10;
	Sync_RdFL(srd_ctrl9); // regAddr = 0x1019 : 0x14
	Sync_Wr01(srd_ctrl9,srdwaittime_7_0, srdwaittime_7_0);
	Sync_WrFL(srd_ctrl9);

#if 0
	srdwaittime_7_0 = 0x1a; //250ms
	Sync_RdFL(srd_ctrl9);  //regAddr = 0x1019
	Sync_Wr01(srd_ctrl9,srdwaittime_7_0,srdwaittime_7_0);
	Sync_WrFL(srd_ctrl9);
#endif



// add 2011/10/12


	DEMOD_PRINT("^p^[KADP_DEMOD DTV] ####chungiii check !!! %s =====\n",__F__);



	crdeltafcp2x_v_4_0 = 0xE6;
	Sync_RdFL(carrier_ctrl_v10); // regAddr = 0x106B
	Sync_Wr01(carrier_ctrl_v10,crdeltafcp2x_v_4_0, crdeltafcp2x_v_4_0);
	Sync_WrFL(carrier_ctrl_v10);

	crdeltafcp1x_v_4_0 = 0xE6;
	Sync_RdFL(carrier_ctrl_v11); // regAddr = 0x106C
	Sync_Wr01(carrier_ctrl_v11,crdeltafcp1x_v_4_0, crdeltafcp1x_v_4_0);
	Sync_WrFL(carrier_ctrl_v11);


	crdeltafcm2x_v_4_0 = 0xE6;
	Sync_RdFL(carrier_ctrl_v12); // regAddr = 0x106D
	Sync_Wr01(carrier_ctrl_v12,crdeltafcm2x_v_4_0, crdeltafcm2x_v_4_0);
	Sync_WrFL(carrier_ctrl_v12);

	crdeltafcm1x_v_4_0 = 0xE6;
	Sync_RdFL(carrier_ctrl_v13); // regAddr = 0x106E
	Sync_Wr01(carrier_ctrl_v13,crdeltafcm1x_v_4_0, crdeltafcm1x_v_4_0);
	Sync_WrFL(carrier_ctrl_v13);



	sclockcheck_v_23_16 = 0xF;
	Sync_RdFL(v_top_ctrl3); // regAddr = 0x1027
	Sync_Wr01(v_top_ctrl3,sclockcheck_v_23_16, sclockcheck_v_23_16);
	Sync_WrFL(v_top_ctrl3);


	sclockcheck_v_15_8 = 0x42;
	Sync_RdFL(v_top_ctrl4); // regAddr = 0x1028
	Sync_Wr01(v_top_ctrl4,sclockcheck_v_15_8, sclockcheck_v_15_8);
	Sync_WrFL(v_top_ctrl4);

	sclockcheck_v_7_0 = 0x3C;
	Sync_RdFL(v_top_ctrl5); // regAddr = 0x1029
	Sync_Wr01(v_top_ctrl5,sclockcheck_v_7_0, sclockcheck_v_7_0);
	Sync_WrFL(v_top_ctrl3);


	adcclkpllsel = 0;
	Common_RdFL(common_ctrl4);	 //regAddr = 0x0004
	Common_Wr01(common_ctrl4, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);


	pllsetauto = 0;
	pllctrl_13_8 = 0x25;
	Common_RdFL(common_ctrl5);	 //regAddr = 0x0005
	Common_Wr02(common_ctrl5, pllsetauto, pllsetauto, pllctrl_13_8, pllctrl_13_8);
	Common_WrFL(common_ctrl5);

	pllctrl_7_0 = 0x64;
	Common_RdFL(common_ctrl6);	 //regAddr = 0x0006
	Common_Wr01(common_ctrl6, pllctrl_7_0, pllctrl_7_0);
	Common_WrFL(common_ctrl6);


	stdopdettmode_2_0 = 0x0;
	stdopdetcmode_1_0 = 0x2;
	Common_RdFL(common_ctrl8);	 //regAddr = 0x0009
	Common_Wr02(common_ctrl8, stdopdettmode_2_0, stdopdettmode_2_0,stdopdetcmode_1_0,stdopdetcmode_1_0);
	Common_WrFL(common_ctrl8);

	daften = 0;
	srd1stchken = 1;
	qmden = 1;
	Common_RdFL(common_ctrl9);	 //regAddr = 0x000a,
	Common_Wr03(common_ctrl9, daften, daften, srd1stchken, srd1stchken,qmden,qmden);
	Common_WrFL(common_ctrl9);


	samclkauto = 0;
	sampling4xfen = 1;
	samfreq_21_16 = 0x18;
	Common_RdFL(common_ctrl11);  //regAddr = 0x000d,
	Common_Wr03(common_ctrl11, samclkauto, samclkauto, sampling4xfen, sampling4xfen,samfreq_21_16,samfreq_21_16);
	Common_WrFL(common_ctrl11);


	agcinsel = 1;
	Common_RdFL(agc_ctrl0); //regAddr = 0x0012, AGC control register
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
	//agcinsel : Selection of input signal for gain error calculation
	//	0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
	// 1 : use incoming signal This signal should be set to '1' at the start of operation

	agciffixb=0;
	agcrffixb=0;
	Common_RdFL(agc_ctrl1);  //regAddr = 0x0013,
	Common_Wr02(agc_ctrl1, agciffixb, agciffixb, agcrffixb, agcrffixb);
	Common_WrFL(agc_ctrl1);


	aiccopmode0_1_0 = 0;
	Common_RdFL(mdp_ctrl8);   //regAddr = 0x0024, MDP_CTRL[8]
	Common_Wr01(mdp_ctrl8, aiccopmode0_1_0, aiccopmode0_1_0);
	Common_WrFL(mdp_ctrl8);
	//AICCOPMODE0[1:0] :election of 1st AICC(continuous wave interference or analog TV signal) rejection function
	//This register should be set to 0x00 at DVB-C and QAM


	dagc2ndrefmode = 0;
	Common_RdFL(mdp_ctrl24);	 //regAddr = 0x0035,
	Common_Wr01(mdp_ctrl24, dagc2ndrefmode, dagc2ndrefmode);
	Common_WrFL(mdp_ctrl24);

	tpclkouten=1;
	tpifen	=1;
	tpouten=1;
	Common_RdFL(tp_ctrl0);	//regAddr = 0x0070,
	Common_Wr03(tp_ctrl0, tpclkouten,tpclkouten,tpifen, tpifen, tpouten,tpouten);
	Common_WrFL(tp_ctrl0);

//	tpifbw_3_0	 = 0;				  //   : 4,   0: 3
//	tpifctrl_3_0 = 0x5; 			   //	: 4;	  4: 7
//	Common_RdFL(tp_ctrl2);	//regAddr = 0x0072,
//	Common_Wr02(tp_ctrl2, tpifbw_3_0,tpifbw_3_0,tpifctrl_3_0, tpifctrl_3_0);
//	Common_WrFL(tp_ctrl2);





#if 0
	selfsyncfinddetth_cqs_7_0 = 0x2;
	FEC_RdFL(fec_set6);
	FEC_Wr01(fec_set6,selfsyncfinddetth_cqs_7_0,selfsyncfinddetth_cqs_7_0);
	FEC_WrFL(fec_set6);

	selfsyncfindccth_cqs_7_0 = 0x4;
	FEC_RdFL(fec_set7);
	FEC_Wr01(fec_set7,selfsyncfindccth_cqs_7_0,selfsyncfindccth_cqs_7_0);
	FEC_WrFL(fec_set7);

#endif





	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_Get_TotalInfo
 * Get all information on QAM for debug
 *
 * @param NONE [IN / OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_QAM_Get_TotalInfo( void)
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

	if (RET_OK  != DEMOD_L9_Get_NeverLockStatus(LX_DEMOD_QAM, &chScanStatus))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_NeverLockStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_GetOperMode(&operMode))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_IFAGC(&ifAGC))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_IFAGC() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_Lock(LX_DEMOD_TR_LOCK, &timingLock))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) LX_DEMOD_LOCK_STATE_T() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLock))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_CarrierFreqOffset(&offsetFrq))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

 	//MSE

	if (RET_OK != DEMOD_L9_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  != DEMOD_L9_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_SpectrumStatus() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  !=  DEMOD_L9_GetTrAcquisionTime(&tracquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_GetTrAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	if (RET_OK  !=  DEMOD_L9_GetFecAcquisionTime(&fecacquisionTimeMs))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_GetFecAcquisionTime() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if (RET_OK  !=  DEMOD_L9_Get_VABER(&viterbiBer))
	{
		DEMOD_PRINT( "^r^[DEMOD L9 DTV] FAIL (%s:%d) DEMOD_L9_Get_VABER() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	DEMOD_PRINT("[DEMOD L9 DTV] Channel status  = %s !!!\n",(chScanStatus == LX_DEMOD_DETECTION_PROCESSING) ? "not completed" :
												(chScanStatus == LX_DEMOD_MODE_CHANGE) ? "mode change" :
												(chScanStatus == LX_DEMOD_NEXT_CHANNEL) ? "next channel" :
												(chScanStatus == LX_DEMOD_LOCKED) ? "channel exist" : "unkown" );

	DEMOD_PRINT("[DEMOD L9 DTV] Operation Mode  = %s !!!\n",(operMode == LX_DEMOD_64QAM) ? "64QAM" :
													(operMode == LX_DEMOD_QAM256_MODE) ? "256QAM" :
													(operMode == LX_DEMOD_VSB_MODE) ? "VSB" :  "unknown" );

	DEMOD_PRINT("[DEMOD L9 DTV] IF AGC Gain = 0x%x !!!\n",ifAGC);
	DEMOD_PRINT("[DEMOD L9 DTV] Timing LOCK = %s (%d ms)!!!\n",(timingLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked", tracquisionTimeMs);
	DEMOD_PRINT("[DEMOD L9 DTV] FEC LOCK = %s !!!\n",(fecLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD L9 DTV] Carrier offset Frequency = %d [KHz] !!!\n",offsetFrq);
	DEMOD_PRINT("[DEMOD L9 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD L9] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	DEMOD_PRINT("[DEMOD L9 DTV] Viterbi BER = %d !!!\n",viterbiBer);

	return RET_OK;

#endif

	return RET_OK;

}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_QAM_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	static unsigned int 			manualOperationTime = 0;
	static LX_DEMOD_TUNE_MODE_T		realTuneMode = LX_DEMOD_TUNE_UNKNOWN;


	if(pDemodInfo->bSetParams)
	{
		switch (pDemodInfo->controlState)
		{

												/***************************************************/
												/***** Configure demod	for RETUNE. *****/
			case LX_DEMOD_RETUNE_RF:			/***************************************************/
				{

					if(RET_OK != DEMOD_L9_QAM_Set_DefaultRegisterValue())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.qam.tuneMode)
					{
						if(RET_OK != DEMOD_L9_QAM_ModeAutoDetection(TRUE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else
					{
						if(RET_OK != DEMOD_L9_QAM_ModeAutoDetection(FALSE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_Set_SpectrumCtrl(FALSE, pDemodInfo->setCfgParam.qam.bSpectrumInv))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Set_SpectrumCtrl() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if(LX_DEMOD_64QAM == pDemodInfo->setCfgParam.qam.qamMode)
						{
							if(RET_OK != DEMOD_L9_QAM_64Mode())
							{
								DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_64Mode() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
						}
						else if(LX_DEMOD_256QAM == pDemodInfo->setCfgParam.qam.qamMode)
						{
							if(RET_OK != DEMOD_L9_QAM_256Mode())
							{
								DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_256Mode() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
						}
						else
						{
								DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) Invalid QAM Mode !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
						}

					}


					pDemodInfo->controlState = LX_DEMOD_TUNE_START;
					DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
					g_RetuneStartTime = jiffies_to_msecs(jiffies);

				}

				break;


												/***************************************************/
												/***** Configure tuner for desired channel No. *****/
			case LX_DEMOD_TUNE_START:			/***************************************************/
				{
					unsigned int 			elapsedTime;

					if(RET_OK != DEMOD_L9_SoftwareReset())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_SoftwareReset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.qam.tuneMode)
					{
						realTuneMode = LX_DEMOD_TUNE_AUTO;
						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

						if(elapsedTime >= LX_DEMOD_LOCK_DEALY_170)
						{
							if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
							{
								DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");

						}

					}
					else
					{
						realTuneMode = LX_DEMOD_TUNE_NORMAL;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;
						manualOperationTime = jiffies_to_msecs(jiffies);

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

						DEMOD_PRINT( "^c^[DEMOD L9] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
						return RET_OK;
					}

					if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					}
					else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
						pDemodInfo->bNotifiedUnlock	= TRUE;
						pDemodInfo->bNotifiedLock	= FALSE;

						pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


						currentTime = jiffies_to_msecs(jiffies);
						elapsedTime = currentTime - g_NewTuneReqeustTime;

						DEMOD_PRINT("\nMSG_QAM_FRONT_END_NOT_LOCKED..\n") ;
						DEMOD_PRINT("Elapse time is %d msec\n", elapsedTime) ;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


					}
					else
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{
						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

						sigStablestartTime = jiffies_to_msecs(jiffies);

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
					}
					else
					{
						if(LX_DEMOD_TUNE_NORMAL == realTuneMode)
						{
							elapsedTime = jiffies_to_msecs(jiffies) - manualOperationTime;


							if(elapsedTime > LX_DEMOD_LOCK_DEALY_300)
							{
								if(RET_OK != DEMOD_L9_QAM_ModeAutoDetection(TRUE))
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}

								if(RET_OK != DEMOD_L9_SoftwareReset())
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_SoftwareReset() !!!\n", __F__, __L__);
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
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
								if(RET_OK != DEMOD_L9_Get_Packet_Error(&packetError))
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Packet_Error() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
								else
								{
									if(packetError < LX_DEMOD_PACKET_ERROR_THD)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock	= FALSE;

										if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

									}
									else
									{
										elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
										if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
										{
											pDemodInfo->bNotifiedLock	= TRUE;
											pDemodInfo->bNotifiedUnlock = FALSE;

											if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
											{
												DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
												DEMOD_RETURN_FAIL(__F__);
											}
											pDemodInfo->freqOffset = freqOffset;

											pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

											elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

										}

									}

								}

							}
						}
						else
						{
							pDemodInfo->bNotifiedLock	= TRUE;
							pDemodInfo->bNotifiedUnlock = FALSE;

							if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
							{
								DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							pDemodInfo->freqOffset = freqOffset;

							pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

							elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);


						}

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}


					}
					else
					{
						pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

					}


		   		}
				break;

			case LX_DEMOD_SIGNAL_LOCKED:	break;

			default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

		}
	}


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_QAM_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_QAM_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;

		}
		if(pDemodInfo->lockCount == 4)
		{
			pDemodInfo->lockStatus = LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock = TRUE;
			if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
				pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		}
		if(pDemodInfo->lockCount > 4)
		{
			pDemodInfo->lockCount = 0;
		}


	}
	else
	{
		pDemodInfo->lockCount = 0;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;

			}
			if(pDemodInfo->unLockCount > 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 200)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount > 200)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;

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



#if 0
#endif


